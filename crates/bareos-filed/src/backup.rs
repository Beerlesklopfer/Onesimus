//! Backup protocol handler — FD side.
//!
//! Protocol flow (Director → FD):
//! 1. Director sends: "JobId=N Job=name SDid=1 SDtime=T Authorization=key"
//! 2. FD responds: "2000 OK Job ..."
//! 3. Director sends FileSet (include/exclude)
//! 4. Director sends: "backup"
//! 5. FD connects to SD, scans files, sends data
//! 6. FD sends end-of-job status

use tracing::{info, debug, warn, error};
use bareos_bnet::{BnetStream, BnetSignal, BnetMessage};

use crate::config::FdConfig;

/// Handle a job command from the Director.
pub fn handle_job_command<S: std::io::Read + std::io::Write>(
    cmd: &str,
    bnet: &mut BnetStream<S>,
    session_id: u64,
    config: &FdConfig,
) {
    // Parse: "JobId=N Job=name SDid=1 SDtime=T Authorization=key"
    let mut job_id = 0u32;
    let mut job_name = String::new();
    let mut sd_auth_key = String::new();

    for part in cmd.split_whitespace() {
        if let Some(val) = part.strip_prefix("JobId=") {
            job_id = val.parse().unwrap_or(0);
        } else if let Some(val) = part.strip_prefix("Job=") {
            job_name = val.to_string();
        } else if let Some(val) = part.strip_prefix("Authorization=") {
            sd_auth_key = val.to_string();
        }
    }

    info!("session {session_id}: job {job_id} '{job_name}' starting");

    // Respond OK
    let ok = format!("2000 OK Job {job_name} ({job_id})\n");
    if bnet.send_str(&ok).is_err() { return; }

    // Read further commands from Director (FileSet, backup, etc.)
    loop {
        match bnet.recv() {
            Ok(BnetMessage::Data(data)) => {
                let subcmd = String::from_utf8_lossy(&data).trim().to_string();
                debug!("session {session_id}: job subcmd '{subcmd}'");

                if subcmd.starts_with("backup") {
                    // Start backup — scan files and send to SD
                    // TODO: implement actual file scanning and SD data transfer
                    info!("session {session_id}: backup command received for job {job_id}");
                    let _ = bnet.send_str("2000 OK backup\n");

                    // Send end-of-job status
                    let _ = bnet.send_str(&format!(
                        "2800 End Job TermCode=0 JobFiles=0 ReadBytes=0 JobBytes=0 Errors=0 VSS=0 Encrypt=0\n"
                    ));
                    let _ = bnet.send_signal(BnetSignal::Eod);
                    break;
                } else if subcmd.starts_with("restore") {
                    info!("session {session_id}: restore command for job {job_id}");
                    let _ = bnet.send_str("2000 OK restore\n");
                    let _ = bnet.send_signal(BnetSignal::Eod);
                    break;
                } else if subcmd.starts_with("estimate") {
                    let _ = bnet.send_str("2000 OK estimate files=0 bytes=0\n");
                    let _ = bnet.send_signal(BnetSignal::Eod);
                    break;
                }
                // Other sub-commands (FileSet include/exclude, etc.) — accumulate
            }
            Ok(BnetMessage::Signal(BnetSignal::Eod)) => break,
            Ok(BnetMessage::Signal(BnetSignal::Terminate)) => break,
            Ok(BnetMessage::Signal(_)) => continue,
            Err(_) => break,
        }
    }
}
