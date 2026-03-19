//! Job worker — fork per job for process isolation.
//!
//! Each job runs in its own OS process (fork). Communication
//! with the Director is via pipe() for status updates and
//! exit code for final result.

use std::io::{self, Read, Write, BufRead, BufReader};
use std::os::unix::io::{AsRawFd, FromRawFd, IntoRawFd, RawFd};

use tracing::{info, error, debug};

/// Handle to a forked job worker process.
#[derive(Debug)]
pub struct JobHandle {
    pub pid: i32,
    pub job_id: u32,
    pub job_name: String,
    status_reader: Option<std::fs::File>,
}

/// Status update sent from worker to director via pipe.
#[derive(Debug, Clone, serde::Serialize, serde::Deserialize)]
pub struct JobStatusUpdate {
    pub job_id: u32,
    pub status: String,
    pub files: i64,
    pub bytes: i64,
    pub errors: i32,
}

impl JobHandle {
    /// Read the next status update (non-blocking if no data available).
    pub fn try_read_status(&mut self) -> Option<JobStatusUpdate> {
        let reader = self.status_reader.as_mut()?;
        let mut buf = String::new();
        let mut buf_reader = BufReader::new(reader);

        // Try to read a line (JSON-encoded status)
        match buf_reader.read_line(&mut buf) {
            Ok(0) => None, // EOF
            Ok(_) => {
                serde_json::from_str(&buf).ok()
            }
            Err(_) => None,
        }
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
        {
            None
        }
    }
}

/// Spawn a job worker process.
///
/// Returns a JobHandle for the parent, or exits in the child.
/// The child should call `run_job_in_worker()`.
#[cfg(unix)]
pub fn spawn_job_worker(job_id: u32, job_name: &str) -> io::Result<JobHandle> {
    use nix::unistd::{fork, ForkResult, pipe};

    // Create pipe for status communication
    let (read_fd, write_fd) = pipe().map_err(|e| io::Error::new(io::ErrorKind::Other, e))?;
    let read_raw = read_fd.as_raw_fd();
    let write_raw = write_fd.as_raw_fd();

    match unsafe { fork() } {
        Ok(ForkResult::Parent { child }) => {
            // Parent: close write end, keep read end
            drop(write_fd);
            let status_reader = unsafe { std::fs::File::from_raw_fd(read_fd.into_raw_fd()) };

            info!("spawned job worker pid={} for job {} '{}'", child, job_id, job_name);

            Ok(JobHandle {
                pid: child.as_raw(),
                job_id,
                job_name: job_name.to_string(),
                status_reader: Some(status_reader),
            })
        }
        Ok(ForkResult::Child) => {
            // Child: close read end, keep write end for status reporting
            drop(read_fd);

            // Set thread name for visibility in pstree
            let name = format!("job:{job_id}");
            #[cfg(target_os = "linux")]
            {
                use std::ffi::CString;
                if let Ok(c_name) = CString::new(name.as_str()) {
                    unsafe { libc::prctl(libc::PR_SET_NAME, c_name.as_ptr()); }
                }
            }

            // TODO(R6-full): Call run_job() with FD/SD connections
            // For now, exit immediately with success
            std::process::exit(0);
        }
        Err(e) => {
            Err(io::Error::new(io::ErrorKind::Other, format!("fork failed: {e}")))
        }
    }
}

#[cfg(not(unix))]
pub fn spawn_job_worker(job_id: u32, job_name: &str) -> io::Result<JobHandle> {
    Err(io::Error::new(io::ErrorKind::Unsupported, "fork not available on this platform"))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    #[cfg(unix)]
    fn spawn_and_wait() {
        let handle = spawn_job_worker(99, "test-job").unwrap();
        assert!(handle.pid > 0);

        // Wait for child to exit
        std::thread::sleep(std::time::Duration::from_millis(100));
        let exit_code = handle.try_wait();
        assert_eq!(exit_code, Some(0));
    }

    #[test]
    fn job_status_update_serde() {
        let update = JobStatusUpdate {
            job_id: 42,
            status: "Running".into(),
            files: 100,
            bytes: 1024000,
            errors: 0,
        };
        let json = serde_json::to_string(&update).unwrap();
        let parsed: JobStatusUpdate = serde_json::from_str(&json).unwrap();
        assert_eq!(parsed.job_id, 42);
        assert_eq!(parsed.status, "Running");
    }
}
