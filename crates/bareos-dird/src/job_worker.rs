//! Job worker — fork per job for process isolation.
//!
//! Each job runs in its own OS process (fork). The job is **autonomous** —
//! it builds its own FD/SD connections, runs the full backup/restore
//! protocol, and communicates with the Director via two pipes:
//!
//! - **Worker→Director pipe**: Status updates, CatReq forwarding, JobEnd
//! - **Director→Worker pipe**: CatReq responses from Director
//!
//! ## Pipe Protocol (JSON, one line per message)
//!
//! Worker→Director:
//! ```json
//! {"type":"status","job_id":1,"files":100,"bytes":5242880}
//! {"type":"cat_req","job_id":1,"request":"CatReq Job=... get_volume ..."}
//! {"type":"job_end","job_id":1,"term_code":0,"files":451,"bytes":66973531,"errors":0}
//! ```
//!
//! Director→Worker:
//! ```json
//! {"type":"cat_resp","response":"1000 OK VolName=Full-0002 ..."}
//! ```
//!
//! ## Signal Protocol (Director→Worker)
//!
//! | Signal  | Meaning       | Worker Reaction                          |
//! |---------|---------------|------------------------------------------|
//! | SIGTERM | Cancel        | Terminate FD/SD, cleanup, exit 3         |
//! | SIGUSR1 | Pause         | Pause data transfer, keep connections    |
//! | SIGUSR2 | Resume        | Resume data transfer                     |
//! | SIGHUP  | Status dump   | Write current status to pipe             |

use std::io::{self, Read, Write, BufRead, BufReader};
use std::os::unix::io::{AsRawFd, FromRawFd, IntoRawFd};
use std::sync::atomic::{AtomicBool, Ordering};

use tracing::{info, error, debug};

// --- Worker-side signal flags ---

static WORKER_CANCEL: AtomicBool = AtomicBool::new(false);
static WORKER_PAUSED: AtomicBool = AtomicBool::new(false);
static WORKER_INFO: AtomicBool = AtomicBool::new(false);

pub const EXIT_SUCCESS: i32 = 0;    // T
pub const EXIT_ERROR: i32 = 1;      // E
pub const EXIT_FATAL: i32 = 2;      // f
pub const EXIT_CANCELED: i32 = 3;   // A
pub const EXIT_INCOMPLETE: i32 = 4; // I

/// Parameters passed from Director to Worker via pipe after fork().
#[derive(Debug, Clone, serde::Serialize, serde::Deserialize)]
pub struct JobParams {
    pub job_id: u32,
    pub unique_job: String,
    pub job_name: String,
    pub level: String,
    // FD connection
    pub fd_address: String,
    pub fd_port: u16,
    pub fd_password: String,
    // SD connection
    pub sd_address: String,
    pub sd_port: u16,
    pub sd_password: String,
    pub sd_device: String,
    pub sd_media_type: String,
    // Job config
    pub director_name: String,
    pub pool_name: String,
    pub fileset_includes: Vec<String>,
    pub fileset_excludes: Vec<String>,
}

/// Pipe message types (Worker↔Director)
#[derive(Debug, Clone, serde::Serialize, serde::Deserialize)]
#[serde(tag = "type")]
pub enum PipeMessage {
    /// Worker→Dir: periodic status update
    #[serde(rename = "status")]
    Status { job_id: u32, files: i64, bytes: i64, errors: i32 },
    /// Worker→Dir: CatReq from SD that needs Director response
    #[serde(rename = "cat_req")]
    CatReq { job_id: u32, request: String },
    /// Dir→Worker: CatReq response
    #[serde(rename = "cat_resp")]
    CatResp { response: String },
    /// Worker→Dir: job finished
    #[serde(rename = "job_end")]
    JobEnd { job_id: u32, term_code: i32, files: i64, bytes: i64, errors: i32 },
}

/// Handle to a forked job worker process (held by the Director).
#[derive(Debug)]
pub struct JobHandle {
    pub pid: i32,
    pub job_id: u32,
    pub job_name: String,
    /// Read end: Worker→Director (status, cat_req, job_end)
    from_worker: Option<std::fs::File>,
    /// Write end: Director→Worker (cat_resp)
    to_worker: Option<std::fs::File>,
}

impl JobHandle {
    /// Read the next message from the worker (non-blocking).
    pub fn try_read_message(&mut self) -> Option<PipeMessage> {
        let reader = self.from_worker.as_mut()?;
        let mut buf = String::new();
        let mut buf_reader = BufReader::new(reader);
        match buf_reader.read_line(&mut buf) {
            Ok(0) => None,
            Ok(_) => serde_json::from_str(&buf).ok(),
            Err(_) => None,
        }
    }

    /// Send a message to the worker (CatResp).
    pub fn send_message(&mut self, msg: &PipeMessage) -> bool {
        if let Some(writer) = &mut self.to_worker {
            if let Ok(json) = serde_json::to_string(msg) {
                return writeln!(writer, "{json}").is_ok();
            }
        }
        false
    }

    /// Check if the worker process has exited (non-blocking).
    pub fn try_wait(&self) -> Option<i32> {
        #[cfg(unix)]
        {
            use nix::sys::wait::{waitpid, WaitPidFlag};
            use nix::unistd::Pid;
            match waitpid(Pid::from_raw(self.pid), Some(WaitPidFlag::WNOHANG)) {
                Ok(nix::sys::wait::WaitStatus::Exited(_, code)) => Some(code),
                Ok(nix::sys::wait::WaitStatus::Signaled(_, sig, _)) => Some(128 + sig as i32),
                _ => None,
            }
        }
        #[cfg(not(unix))]
        { None }
    }

    pub fn cancel(&self)  { self.send_signal(libc::SIGTERM); }
    pub fn pause(&self)   { self.send_signal(libc::SIGUSR1); }
    pub fn resume(&self)  { self.send_signal(libc::SIGUSR2); }
    pub fn request_status(&self) { self.send_signal(libc::SIGHUP); }

    fn send_signal(&self, sig: libc::c_int) {
        #[cfg(unix)]
        unsafe { libc::kill(self.pid, sig); }
    }
}

/// Spawn a job worker process.
///
/// Creates two pipe pairs for bidirectional communication, forks,
/// and sends JobParams to the child. Returns a JobHandle for the parent.
#[cfg(unix)]
pub fn spawn_job_worker(job_id: u32, job_name: &str, params: &JobParams) -> io::Result<JobHandle> {
    use nix::unistd::{fork, ForkResult, pipe};

    // Pipe 1: Worker→Director (status, cat_req, job_end)
    let (w2d_read, w2d_write) = pipe().map_err(|e| io::Error::new(io::ErrorKind::Other, e))?;
    // Pipe 2: Director→Worker (cat_resp)
    let (d2w_read, d2w_write) = pipe().map_err(|e| io::Error::new(io::ErrorKind::Other, e))?;

    match unsafe { fork() } {
        Ok(ForkResult::Parent { child }) => {
            // Parent: keep read-from-worker + write-to-worker
            drop(w2d_write);
            drop(d2w_read);

            let from_worker = unsafe { std::fs::File::from_raw_fd(w2d_read.into_raw_fd()) };
            let to_worker = unsafe { std::fs::File::from_raw_fd(d2w_write.into_raw_fd()) };

            info!("spawned job worker pid={} for job {} '{}'", child, job_id, job_name);

            Ok(JobHandle {
                pid: child.as_raw(),
                job_id,
                job_name: job_name.to_string(),
                from_worker: Some(from_worker),
                to_worker: Some(to_worker),
            })
        }
        Ok(ForkResult::Child) => {
            // Child: keep write-to-director + read-from-director
            drop(w2d_read);
            drop(d2w_write);

            let proc_name = format!("job:{job_id}");
            #[cfg(target_os = "linux")]
            {
                use std::ffi::CString;
                if let Ok(c_name) = CString::new(proc_name.as_str()) {
                    unsafe { libc::prctl(libc::PR_SET_NAME, c_name.as_ptr()); }
                }
            }

            install_worker_signal_handlers();

            // Close inherited FDs (TCP listener, etc.) — child must not hold them
            // Close all FDs > 2 except our pipe FDs
            let keep_fds = [w2d_write.as_raw_fd(), d2w_read.as_raw_fd()];
            for fd in 3..1024 {
                if !keep_fds.contains(&fd) {
                    unsafe { libc::close(fd); }
                }
            }

            eprintln!("[worker:{job_id}] child started, pid={}", std::process::id());

            let mut to_director = unsafe { std::fs::File::from_raw_fd(w2d_write.into_raw_fd()) };
            let mut from_director = unsafe { std::fs::File::from_raw_fd(d2w_read.into_raw_fd()) };

            // Run the job autonomously
            let exit_code = crate::job_runner::run_backup_autonomous(
                params, &mut to_director, &mut from_director,
            );

            eprintln!("[worker:{job_id}] exiting with code {exit_code}");
            std::process::exit(exit_code);
        }
        Err(e) => {
            Err(io::Error::new(io::ErrorKind::Other, format!("fork failed: {e}")))
        }
    }
}

#[cfg(not(unix))]
pub fn spawn_job_worker(_job_id: u32, _job_name: &str, _params: &JobParams) -> io::Result<JobHandle> {
    Err(io::Error::new(io::ErrorKind::Unsupported, "fork not available"))
}

// --- Signal handlers ---

fn install_worker_signal_handlers() {
    unsafe {
        libc::signal(libc::SIGTERM, worker_sigterm as libc::sighandler_t);
        libc::signal(libc::SIGUSR1, worker_sigusr1 as libc::sighandler_t);
        libc::signal(libc::SIGUSR2, worker_sigusr2 as libc::sighandler_t);
        libc::signal(libc::SIGHUP, worker_sighup as libc::sighandler_t);
    }
}

extern "C" fn worker_sigterm(_: libc::c_int) { WORKER_CANCEL.store(true, Ordering::SeqCst); }
extern "C" fn worker_sigusr1(_: libc::c_int) { WORKER_PAUSED.store(true, Ordering::SeqCst); }
extern "C" fn worker_sigusr2(_: libc::c_int) { WORKER_PAUSED.store(false, Ordering::SeqCst); }
extern "C" fn worker_sighup(_: libc::c_int) { WORKER_INFO.store(true, Ordering::SeqCst); }

pub fn is_canceled() -> bool { WORKER_CANCEL.load(Ordering::SeqCst) }

pub fn wait_if_paused() -> bool {
    while WORKER_PAUSED.load(Ordering::SeqCst) {
        if WORKER_CANCEL.load(Ordering::SeqCst) { return false; }
        std::thread::sleep(std::time::Duration::from_millis(100));
    }
    true
}

pub fn status_requested() -> bool { WORKER_INFO.swap(false, Ordering::SeqCst) }

/// Write a pipe message (Worker→Director).
pub fn send_pipe_msg(pipe: &mut std::fs::File, msg: &PipeMessage) {
    if let Ok(json) = serde_json::to_string(msg) {
        let _ = writeln!(pipe, "{json}");
    }
}

/// Read a pipe message (Director→Worker). Blocks until a line arrives.
pub fn recv_pipe_msg(pipe: &mut std::fs::File) -> Option<PipeMessage> {
    let mut buf = String::new();
    let mut reader = BufReader::new(pipe);
    match reader.read_line(&mut buf) {
        Ok(0) => None,
        Ok(_) => serde_json::from_str(&buf).ok(),
        Err(_) => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn pipe_message_serde() {
        let msg = PipeMessage::Status { job_id: 1, files: 100, bytes: 5000, errors: 0 };
        let json = serde_json::to_string(&msg).unwrap();
        let parsed: PipeMessage = serde_json::from_str(&json).unwrap();
        match parsed {
            PipeMessage::Status { job_id, files, .. } => {
                assert_eq!(job_id, 1);
                assert_eq!(files, 100);
            }
            _ => panic!("wrong type"),
        }
    }

    #[test]
    fn job_params_serde() {
        let params = JobParams {
            job_id: 42, unique_job: "test.2026".into(), job_name: "test".into(),
            level: "full".into(),
            fd_address: "localhost".into(), fd_port: 9102, fd_password: "p".into(),
            sd_address: "localhost".into(), sd_port: 9103, sd_password: "p".into(),
            sd_device: "FileStorage".into(), sd_media_type: "File".into(),
            director_name: "bareos-dir".into(), pool_name: "Full".into(),
            fileset_includes: vec!["/usr/sbin".into()],
            fileset_excludes: vec![],
        };
        let json = serde_json::to_string(&params).unwrap();
        let parsed: JobParams = serde_json::from_str(&json).unwrap();
        assert_eq!(parsed.job_id, 42);
        assert_eq!(parsed.fileset_includes, vec!["/usr/sbin"]);
    }

    #[test]
    fn exit_codes() {
        assert_eq!(EXIT_SUCCESS, 0);
        assert_eq!(EXIT_FATAL, 2);
        assert_eq!(EXIT_CANCELED, 3);
    }
}
