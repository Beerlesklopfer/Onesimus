//! Job Manager — Director-side tracking of all running job workers.
//!
//! Maintains a Vec of active jobs (up to 1000+ concurrent).
//! One monitor thread polls all worker pipes and handles CatReq.

use std::sync::{Arc, Mutex};
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

use tracing::{info, debug, warn, error};

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
}

impl JobManager {
    pub fn new(bus: Arc<MessageBus>, config: Option<SharedConfig>) -> Self {
        Self { jobs: Vec::new(), bus, config }
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
                        let response = handle_cat_req(&request, &self.config);
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

/// Minimal CatReq handler — enough for first working backup.
fn handle_cat_req(request: &str, _config: &Option<SharedConfig>) -> String {
    // TODO: Implement proper catalog queries (DB access)
    if request.contains("get_volume") || request.contains("find_next_volume") {
        debug!("CatReq: get_volume → auto-label");
        "1000 OK VolName=Full-0002 VolJobs=0 VolFiles=0 VolBlocks=0 VolBytes=0 \
         VolMounts=0 VolErrors=0 VolWrites=0 MaxVolBytes=53687091200 VolCapBytes=0 \
         VolStatus=Append Slot=0 MaxVolJobs=0 MaxVolFiles=0 InChanger=0 \
         VolReadTime=0 VolWriteTime=0 EndFile=0 EndBlock=0 \
         LabelType=0 MediaId=2 PoolId=1 MediaType=File StorageId=0 \
         DeviceId=0 LocationId=0 RecycleCount=0 InitialWrite=0 \
         ScratchPoolId=0 RecyclePoolId=0 Enabled=1 ActionOnPurge=0 \
         VolRetention=31536000 VolUseDuration=0 MaxVolJobs=0 MaxVolFiles=0\n".to_string()
    } else if request.contains("update_vol") || request.contains("UpdateMedia") {
        "1000 OK\n".to_string()
    } else {
        debug!("CatReq: unknown, returning OK: {}", &request[..request.len().min(60)]);
        "1000 OK\n".to_string()
    }
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
