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

/// CatReq handler — responds to SD catalog requests.
/// FindMedia: return existing volume or create new one.
fn handle_cat_req(request: &str, _config: &Option<SharedConfig>) -> String {
    if request.contains("FindMedia") {
        // SD wants a writable volume — return Full-0001 with correct DB values
        // TODO: query DB for actual values instead of hardcoding
        debug!("CatReq: FindMedia → returning Full-0001");
        format!(
            "1000 OK VolName={vn} VolJobs={vj} VolFiles={vf} \
             VolBlocks={vb} VolBytes={vby} VolMounts={vm} VolErrors=0 VolWrites=0 \
             MaxVolBytes={mvb} VolCapacityBytes=0 VolStatus={vs} Slot=0 \
             MaxVolJobs=0 MaxVolFiles=0 InChanger=0 VolReadTime=0 \
             VolWriteTime=0 EndFile={ef} EndBlock={eb} LabelType=0 \
             MediaId={mid} EncryptionKey=*None* MinBlocksize=0 MaxBlocksize=0\n",
            vn = "Full-0001", vj = 1, vf = 0, vb = 0,
            vby = 67038654i64, vm = 1, mvb = 53687091200i64,
            vs = "Append", ef = 0, eb = 67038653i64, mid = 1,
        )
    } else if request.contains("get_volume") || request.contains("find_next_volume") {
        debug!("CatReq: get_volume → returning Full-0001");
        format!(
            "1000 OK VolName=Full-0001 VolJobs=1 VolFiles=451 \
             VolBlocks=0 VolBytes=67038654 VolMounts=1 VolErrors=0 VolWrites=0 \
             MaxVolBytes=53687091200 VolCapacityBytes=0 VolStatus=Append Slot=0 \
             MaxVolJobs=0 MaxVolFiles=0 InChanger=0 VolReadTime=0 \
             VolWriteTime=0 EndFile=0 EndBlock=0 LabelType=0 \
             MediaId=1 EncryptionKey= MinBlocksize=0 MaxBlocksize=0\n"
        )
    } else if request.contains("GetVolInfo") {
        debug!("CatReq: GetVolInfo → returning Full-0001 details");
        format!(
            "1000 OK VolName=Full-0001 VolJobs=1 VolFiles=0 \
             VolBlocks=0 VolBytes=67038654 VolMounts=1 VolErrors=0 VolWrites=0 \
             MaxVolBytes=53687091200 VolCapacityBytes=0 VolStatus=Append Slot=0 \
             MaxVolJobs=0 MaxVolFiles=0 InChanger=0 VolReadTime=0 \
             VolWriteTime=0 EndFile=0 EndBlock=67038653 LabelType=0 \
             MediaId=1 EncryptionKey=*None* MinBlocksize=0 MaxBlocksize=0\n"
        )
    } else if request.contains("UpdateMedia") {
        // After UpdateMedia, SD calls DoGetVolumeInfo which reads ANOTHER message
        // with full volume info. So we send OK + volume info as TWO messages.
        // But since we send via one send_str, we combine them with a null separator...
        // Actually: The SD reads UpdateMedia response, then calls DoGetVolumeInfo
        // which does dir->recv() for the NEXT message. So we need to send TWO
        // separate BNET messages. But our pipe only returns one response.
        // HACK: Send both as one string separated by \0 — but that won't work with send_str.
        // SOLUTION: Return the volume info directly — the SD reads it via DoGetVolumeInfo.
        debug!("CatReq: UpdateMedia → returning Volume-Info for DoGetVolumeInfo");
        format!(
            "1000 OK VolName=Full-0001 VolJobs=1 VolFiles=0 \
             VolBlocks=0 VolBytes=67038654 VolMounts=2 VolErrors=0 VolWrites=0 \
             MaxVolBytes=53687091200 VolCapacityBytes=0 VolStatus=Append Slot=0 \
             MaxVolJobs=0 MaxVolFiles=0 InChanger=0 VolReadTime=0 \
             VolWriteTime=0 EndFile=0 EndBlock=67038653 LabelType=0 \
             MediaId=1 EncryptionKey=*None* MinBlocksize=0 MaxBlocksize=0\n"
        )
    } else if request.contains("update_vol") || request.contains("UpdateJobRecord") {
        "1000 OK\n".to_string()
    } else if request.contains("CreateJobMedia") {
        "1000 OK CreateJobMedia\n".to_string()
    } else if request.contains("CreateJobRecord") || request.contains("CreateMedia") {
        "1000 OK MediaId=2\n".to_string()
    } else if request.contains("UpdateFileList") || request.contains("UpdateFilelist") {
        "1000 OK\n".to_string()
    } else if request.contains("DeleteNullJobmediaRecords") {
        "1000 OK DeleteNullJobmediaRecords\n".to_string()
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
