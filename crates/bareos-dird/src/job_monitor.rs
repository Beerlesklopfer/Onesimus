//! Job monitor — Director-side pipe reader for forked job workers.
//!
//! One monitor thread per running job. Reads Worker→Director pipe,
//! handles CatReq (via DB/Config), posts status to MessageBus.

use std::io::Write;
use std::sync::Arc;
use std::thread;

use tracing::{info, debug, warn, error};

use crate::job_worker::{JobHandle, PipeMessage};
use crate::message_bus::{BusMessage, MsgType};
use crate::session_manager::SessionManager;

/// Start a monitor thread for a job worker.
/// Returns a JoinHandle that completes when the job ends.
pub fn start_monitor(
    mut handle: JobHandle,
    session_mgr: Option<Arc<SessionManager>>,
) -> thread::JoinHandle<()> {
    let job_id = handle.job_id;
    let job_name = handle.job_name.clone();

    thread::Builder::new()
        .name(format!("dir:monitor:{job_id}"))
        .spawn(move || {
            monitor_loop(&mut handle, job_id, &job_name, &session_mgr);
        })
        .expect("failed to spawn job monitor")
}

fn monitor_loop(
    handle: &mut JobHandle,
    job_id: u32,
    job_name: &str,
    session_mgr: &Option<Arc<SessionManager>>,
) {
    debug!("monitor:{job_id}: started for '{job_name}'");

    loop {
        match handle.try_read_message() {
            Some(PipeMessage::Status { files, bytes, errors, .. }) => {
                debug!("monitor:{job_id}: files={files} bytes={bytes} errors={errors}");
                post_bus(session_mgr, job_id, job_name, MsgType::Info,
                    &format!("files={files} bytes={bytes}"));
            }

            Some(PipeMessage::CatReq { request, .. }) => {
                debug!("monitor:{job_id}: CatReq: {}", request.trim());
                let response = handle_cat_req(&request);
                handle.send_message(&PipeMessage::CatResp { response });
            }

            Some(PipeMessage::JobEnd { term_code, files, bytes, errors, .. }) => {
                let status_str = match term_code {
                    0 => "Success",
                    1 => "Error",
                    2 => "FatalError",
                    3 => "Canceled",
                    _ => "Unknown",
                };
                info!("monitor:{job_id}: job ended — {status_str} files={files} bytes={bytes} errors={errors}");

                let msg_type = if term_code == 0 { MsgType::Info } else { MsgType::Error };
                post_bus(session_mgr, job_id, job_name, msg_type,
                    &format!("Backup {status_str}: files={files} bytes={bytes} errors={errors}"));
                post_bus_status(session_mgr, job_id, job_name, status_str);
                break;
            }

            None => {
                // EOF or pipe closed — check if worker exited
                if let Some(exit_code) = handle.try_wait() {
                    info!("monitor:{job_id}: worker exited with code {exit_code}");
                    if exit_code != 0 {
                        post_bus(session_mgr, job_id, job_name, MsgType::Error,
                            &format!("Worker exited with code {exit_code}"));
                    }
                    break;
                }
                // Worker still running but no message — brief sleep
                std::thread::sleep(std::time::Duration::from_millis(50));
            }

            _ => {}
        }
    }

    debug!("monitor:{job_id}: ended");
}

/// Handle a CatReq from the SD (via worker pipe).
/// Minimal implementation for first working backup.
fn handle_cat_req(request: &str) -> String {
    // TODO: Implement proper catalog queries (DB access)
    // For now, return minimal OK responses that let the SD proceed.

    if request.contains("get_volume") || request.contains("find_next_volume") {
        // SD wants a volume — we need to return volume info
        // For now, tell SD to create/use a volume
        debug!("CatReq: get_volume — returning auto-label response");
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
    } else if request.contains("catalog") || request.contains("Catalog") {
        "1000 OK\n".to_string()
    } else {
        debug!("CatReq: unknown request, returning OK: {request}");
        "1000 OK\n".to_string()
    }
}

fn post_bus(smgr: &Option<Arc<SessionManager>>, job_id: u32, job_name: &str, msg_type: MsgType, text: &str) {
    if let Some(s) = smgr {
        s.bus().post(BusMessage::JobMessage {
            job_id, job_name: job_name.to_string(), msg_type, text: text.to_string(),
            timestamp: std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap_or_default().as_secs() as i64,
        });
    }
}

fn post_bus_status(smgr: &Option<Arc<SessionManager>>, job_id: u32, job_name: &str, status: &str) {
    if let Some(s) = smgr {
        s.bus().post(BusMessage::JobStatus {
            job_id, job_name: job_name.to_string(), status: status.to_string(),
            timestamp: std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap_or_default().as_secs() as i64,
        });
    }
}
