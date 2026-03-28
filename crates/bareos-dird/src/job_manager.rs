//! Job Manager — Director-side tracking of all running job workers.
//!
//! Maintains a Vec of active jobs (up to 1000+ concurrent).
//! One monitor thread polls all worker pipes and handles CatReq.

use std::sync::{Arc, Mutex};
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

use tracing::{info, debug, warn, error};

use crate::catalog_worker::CatalogHandle;
use crate::job_worker::{JobHandle, PipeMessage};
use crate::message_bus::{MessageBus, BusMessage, MsgType};
use crate::director_config::SharedConfig;

/// Director-side view of a running job.
pub struct DirJob {
    pub job_id: u32,
    pub job_name: String,
    pub phase: DirJobPhase,
    pub handle: JobHandle,
    pub start_time: i64,
    pub last_files: i64,
    pub last_bytes: i64,
}

/// Director-side job phases.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum DirJobPhase {
    Queued,
    Starting,
    Running,
    Paused,
    WaitingCatReq,
    Finishing,
    Completed,
    Error,
}

impl DirJobPhase {
    pub fn name(&self) -> &str {
        match self {
            Self::Queued => "Queued",
            Self::Starting => "Starting",
            Self::Running => "Running",
            Self::Paused => "Paused",
            Self::WaitingCatReq => "WaitingCatReq",
            Self::Finishing => "Finishing",
            Self::Completed => "Completed",
            Self::Error => "Error",
        }
    }
}

/// Manages all active job workers — Vec-based for up to 1000+ jobs.
pub struct JobManager {
    jobs: Vec<DirJob>,
    bus: Arc<MessageBus>,
    config: Option<SharedConfig>,
    catalog: Option<CatalogHandle>,
}

impl JobManager {
    pub fn new(bus: Arc<MessageBus>, config: Option<SharedConfig>, catalog: Option<CatalogHandle>) -> Self {
        Self { jobs: Vec::new(), bus, config, catalog }
    }

    pub fn add_job(&mut self, job_id: u32, job_name: &str, handle: JobHandle) {
        self.jobs.push(DirJob {
            job_id,
            job_name: job_name.to_string(),
            phase: DirJobPhase::Starting,
            handle,
            start_time: now_unix(),
            last_files: 0,
            last_bytes: 0,
        });
        debug!("JobManager: added job {job_id} '{job_name}' (total: {})", self.jobs.len());
    }

    pub fn active_count(&self) -> usize {
        self.jobs.iter().filter(|j| !matches!(j.phase, DirJobPhase::Completed | DirJobPhase::Error)).count()
    }

    pub fn get_job(&self, job_id: u32) -> Option<&DirJob> {
        self.jobs.iter().find(|j| j.job_id == job_id)
    }

    pub fn cancel_job(&mut self, job_id: u32) -> bool {
        if let Some(job) = self.jobs.iter_mut().find(|j| j.job_id == job_id) {
            job.handle.cancel();
            job.phase = DirJobPhase::Error;
            true
        } else {
            false
        }
    }

    pub fn pause_job(&mut self, job_id: u32) -> bool {
        if let Some(job) = self.jobs.iter_mut().find(|j| j.job_id == job_id) {
            job.handle.pause();
            job.phase = DirJobPhase::Paused;
            true
        } else {
            false
        }
    }

    pub fn resume_job(&mut self, job_id: u32) -> bool {
        if let Some(job) = self.jobs.iter_mut().find(|j| j.job_id == job_id) {
            job.handle.resume();
            job.phase = DirJobPhase::Running;
            true
        } else {
            false
        }
    }

    /// Poll all active worker pipes. Non-blocking.
    /// Handles CatReq, status updates, state changes, job completion.
    pub fn poll_all(&mut self) {
        for job in &mut self.jobs {
            if matches!(job.phase, DirJobPhase::Completed | DirJobPhase::Error) {
                continue;
            }

            // Check if worker exited
            if let Some(exit_code) = job.handle.try_wait() {
                if exit_code == 0 {
                    job.phase = DirJobPhase::Completed;
                } else {
                    job.phase = DirJobPhase::Error;
                }
                info!("JobManager: job {} '{}' worker exited (code={})",
                    job.job_id, job.job_name, exit_code);
                continue;
            }

            // Read pipe messages (non-blocking — try_read_message returns None if no data)
            while let Some(msg) = job.handle.try_read_message() {
                match msg {
                    PipeMessage::State { state, .. } => {
                        debug!("JobManager: job {} state={state}", job.job_id);
                        job.phase = match state.as_str() {
                            "Running" => DirJobPhase::Running,
                            "Paused" => DirJobPhase::Paused,
                            "Finishing" => DirJobPhase::Finishing,
                            "Completed" => DirJobPhase::Completed,
                            "Error" => DirJobPhase::Error,
                            _ => job.phase,
                        };
                    }

                    PipeMessage::Status { files, bytes, errors, .. } => {
                        job.last_files = files;
                        job.last_bytes = bytes;
                    }

                    PipeMessage::CatReq { request, .. } => {
                        debug!("JobManager: job {} CatReq: {}", job.job_id, &request[..request.len().min(60)]);
                        job.phase = DirJobPhase::WaitingCatReq;
                        let response = handle_cat_req(&request, &self.catalog);
                        job.handle.send_message(&PipeMessage::CatResp { response });
                        job.phase = DirJobPhase::Running;
                    }

                    PipeMessage::JobEnd { term_code, files, bytes, errors, .. } => {
                        let status = if term_code == 0 { "Success" } else { "Error" };
                        info!("JobManager: job {} '{}' ended — {status} files={files} bytes={bytes} errors={errors}",
                            job.job_id, job.job_name);
                        job.last_files = files;
                        job.last_bytes = bytes;
                        job.phase = if term_code == 0 { DirJobPhase::Completed } else { DirJobPhase::Error };

                        // Post to message bus
                        let msg_type = if term_code == 0 { MsgType::Info } else { MsgType::Error };
                        self.bus.post(BusMessage::JobMessage {
                            job_id: job.job_id,
                            job_name: job.job_name.clone(),
                            msg_type,
                            text: format!("Backup {status}: files={files} bytes={bytes} errors={errors}"),
                            timestamp: now_unix(),
                        });
                        self.bus.post(BusMessage::JobStatus {
                            job_id: job.job_id,
                            job_name: job.job_name.clone(),
                            status: status.to_string(),
                            timestamp: now_unix(),
                        });
                    }

                    _ => {}
                }
            }
        }

        // Cleanup completed jobs older than 5 minutes
        let cutoff = now_unix() - 300;
        self.jobs.retain(|j| {
            if matches!(j.phase, DirJobPhase::Completed | DirJobPhase::Error) && j.start_time < cutoff {
                debug!("JobManager: removing finished job {}", j.job_id);
                false
            } else {
                true
            }
        });
    }
}

/// CatReq handler — queries DB via CatalogHandle (MPSC channel).
fn handle_cat_req(request: &str, catalog: &Option<CatalogHandle>) -> String {
    let Some(cat) = catalog else {
        return "1000 OK\n".to_string();
    };

    if request.contains("FindMedia") {
        let pool = extract_field(request, "pool_name=").unwrap_or("Full".into());
        debug!("CatReq: FindMedia pool={pool}");
        match cat.find_appendable_volume(&pool) {
            Some(name) => cat.get_volume_info(&name).unwrap_or("1901 No Media.\n".into()),
            None => "1901 No Media.\n".to_string(),
        }
    } else if request.contains("GetVolInfo") {
        let vol = extract_field(request, "VolName=").unwrap_or("Full-0001".into());
        debug!("CatReq: GetVolInfo vol={vol}");
        cat.get_volume_info(&vol).unwrap_or("1901 No Media.\n".into())
    } else if request.contains("UpdateMedia") {
        let vol = extract_field(request, "VolName=").unwrap_or("Full-0001".into());
        debug!("CatReq: UpdateMedia vol={vol}");
        let mounts: i64 = extract_field(request, "VolMounts=").and_then(|s| s.parse().ok()).unwrap_or(0);
        let bytes: i64 = extract_field(request, "VolBytes=").and_then(|s| s.parse().ok()).unwrap_or(0);
        let jobs: i32 = extract_field(request, "VolJobs=").and_then(|s| s.parse().ok()).unwrap_or(0);
        let files: i64 = extract_field(request, "VolFiles=").and_then(|s| s.parse().ok()).unwrap_or(0);
        let errors: i64 = extract_field(request, "VolErrors=").and_then(|s| s.parse().ok()).unwrap_or(0);
        let writes: i64 = extract_field(request, "VolWrites=").and_then(|s| s.parse().ok()).unwrap_or(0);
        cat.update_media(&vol, mounts, bytes, jobs, files, errors, writes);
        cat.get_volume_info(&vol).unwrap_or("1901 No Media.\n".into())
    } else if request.contains("CreateJobMedia") {
        "1000 OK CreateJobMedia\n".to_string()
    } else if request.contains("UpdateJobRecord") {
        "1000 OK\n".to_string()
    } else if request.contains("UpdateFileList") || request.contains("Filelist") {
        "1000 OK\n".to_string()
    } else if request.contains("DeleteNullJobmediaRecords") {
        "1000 OK DeleteNullJobmediaRecords\n".to_string()
    } else {
        debug!("CatReq: unknown: {}", &request[..request.len().min(80)]);
        "1000 OK\n".to_string()
    }
}


fn extract_field(request: &str, prefix: &str) -> Option<String> {
    request.split_whitespace()
        .find(|s| s.starts_with(prefix))
        .map(|s| s[prefix.len()..].to_string())
}

/// Generate bootstrap data for a restore from a given backup JobId.
pub fn generate_bootstrap(catalog: &Option<CatalogHandle>, job_id: u32) -> Option<String> {
    catalog.as_ref()?.generate_bootstrap(job_id)
}

/// Find the most recent successful backup job for a client.
pub fn find_latest_backup_jobid(catalog: &Option<CatalogHandle>, client_name: &str) -> Option<u32> {
    catalog.as_ref()?.find_latest_backup_jobid(client_name)
}

/// Job monitor thread — polls all worker pipes periodically.
pub fn monitor_loop(
    manager: Arc<Mutex<JobManager>>,
    shutdown: &AtomicBool,
) {
    debug!("job monitor started");
    while !shutdown.load(Ordering::Relaxed) {
        {
            let mut mgr = manager.lock().unwrap();
            mgr.poll_all();
        }
        thread::sleep(std::time::Duration::from_millis(10));
    }
    debug!("job monitor stopped");
}

fn now_unix() -> i64 {
    std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .unwrap_or_default().as_secs() as i64
}
