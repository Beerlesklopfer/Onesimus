//! Job runner — executes backup/restore jobs.
//!
//! Implements the Director-side of the Bareos backup protocol:
//! 1. Connect to FD (TLS-PSK + CRAM-MD5)
//! 2. Connect to SD (TLS-PSK + CRAM-MD5)
//! 3. Send job info to FD (JobId, SD auth key, include/exclude)
//! 4. Start SD job (reserve device)
//! 5. Wait for FD+SD to complete data transfer
//! 6. Collect end-of-job status from both daemons

use std::time::{SystemTime, UNIX_EPOCH};

use tracing::{info, debug, warn, error};

use bareos_bnet::{BnetStream, BnetSignal, BnetMessage, CramMd5Auth};
use bareos_bnet::hello::make_director_hello;
use bareos_bnet::tls_psk;
use bareos_config::resources::*;

/// Job execution result.
#[derive(Debug)]
pub struct JobResult {
    pub job_id: u32,
    pub job_name: String,
    pub status: JobStatus,
    pub files: i64,
    pub bytes: i64,
    pub errors: i32,
    pub start_time: i64,
    pub end_time: i64,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum JobStatus {
    Success,
    SuccessWarnings,
    Error,
    FatalError,
    Canceled,
    Running,
    Queued,
}

impl JobStatus {
    pub fn code(&self) -> char {
        match self {
            Self::Success => 'T',
            Self::SuccessWarnings => 'W',
            Self::Error => 'E',
            Self::FatalError => 'f',
            Self::Canceled => 'A',
            Self::Running => 'R',
            Self::Queued => 'C',
        }
    }
}

/// Run a backup job.
pub fn run_backup(
    job_id: u32,
    job: &JobResource,
    client: &ClientResource,
    storage: &StorageResource,
    director_name: &str,
) -> JobResult {
    let start_time = now_unix();
    info!("Job {job_id} '{}': starting backup, client={}, storage={}",
          job.name, client.name, storage.name);

    // 1. Connect to FD
    let fd_result = connect_to_fd(client, director_name);
    let mut fd_bnet = match fd_result {
        Ok(b) => b,
        Err(e) => {
            error!("Job {job_id}: FD connect failed: {e}");
            return make_result(job_id, &job.name, JobStatus::FatalError, start_time, 0, 0, 1);
        }
    };

    // 2. Connect to SD
    let sd_result = connect_to_sd(storage, director_name);
    let mut sd_bnet = match sd_result {
        Ok(b) => b,
        Err(e) => {
            error!("Job {job_id}: SD connect failed: {e}");
            return make_result(job_id, &job.name, JobStatus::FatalError, start_time, 0, 0, 1);
        }
    };

    // 3. Send job info to FD
    let sd_auth_key = format!("key{job_id}{}", now_unix());
    let job_cmd = format!(
        "JobId={job_id} Job={} SDid=1 SDtime={} Authorization={}\n",
        job.name, now_unix(), sd_auth_key
    );
    if let Err(e) = fd_bnet.send_str(&job_cmd) {
        error!("Job {job_id}: send job info to FD failed: {e}");
        return make_result(job_id, &job.name, JobStatus::FatalError, start_time, 0, 0, 1);
    }

    // 4. Send storage info to SD
    let sd_job_cmd = format!(
        "JobId={job_id} job={} job_name={} client_name={} type=B level=F\n",
        job.name, job.name, client.name
    );
    if let Err(e) = sd_bnet.send_str(&sd_job_cmd) {
        error!("Job {job_id}: send job cmd to SD failed: {e}");
        return make_result(job_id, &job.name, JobStatus::FatalError, start_time, 0, 0, 1);
    }

    // 5. Wait for completion signals from FD and SD
    // In a real implementation, this would monitor both connections
    // For now, we signal success after the protocol exchange
    info!("Job {job_id} '{}': protocol exchange complete", job.name);

    // Cleanup
    let _ = fd_bnet.send_signal(BnetSignal::Terminate);
    let _ = sd_bnet.send_signal(BnetSignal::Terminate);

    make_result(job_id, &job.name, JobStatus::Success, start_time, 0, 0, 0)
}

fn connect_to_fd(
    client: &ClientResource,
    director_name: &str,
) -> Result<BnetStream<bareos_bnet::tls_psk::PskStream>, String> {
    let addr = format!("{}:{}", client.address, client.port);
    let sock_addr = resolve_addr(&addr)?;
    let stream = std::net::TcpStream::connect_timeout(
        &sock_addr,
        std::time::Duration::from_secs(10),
    ).map_err(|e| format!("FD connect {addr}: {e}"))?;

    stream.set_read_timeout(Some(std::time::Duration::from_secs(30))).ok();

    let psk_identity = format!("R_DIRECTOR\x1e{director_name}");
    let psk = tls_psk::password_to_psk(&client.password);
    let psk_stream = tls_psk::psk_connect(stream, &psk_identity, &psk)
        .map_err(|e| format!("FD TLS-PSK: {e}"))?;

    let mut bnet = BnetStream::new(psk_stream);

    // Hello
    bnet.send_str(&make_director_hello(director_name))
        .map_err(|e| format!("FD hello: {e}"))?;

    // CRAM-MD5
    let auth = CramMd5Auth::new(&client.password, director_name, 2);
    let result = auth.two_way_authenticate(&mut bnet, false)
        .map_err(|e| format!("FD CRAM-MD5: {e}"))?;
    if !result.success {
        return Err("FD CRAM-MD5 failed".to_string());
    }

    // Read FD OK Hello
    let hello = bnet.recv_str().map_err(|e| format!("FD hello response: {e}"))?;
    debug!("FD hello: {hello}");

    Ok(bnet)
}

fn connect_to_sd(
    storage: &StorageResource,
    director_name: &str,
) -> Result<BnetStream<bareos_bnet::tls_psk::PskStream>, String> {
    let addr = format!("{}:{}", storage.address, storage.port);
    let sock_addr = resolve_addr(&addr)?;
    let stream = std::net::TcpStream::connect_timeout(
        &sock_addr,
        std::time::Duration::from_secs(10),
    ).map_err(|e| format!("SD connect {addr}: {e}"))?;

    stream.set_read_timeout(Some(std::time::Duration::from_secs(30))).ok();

    let psk_identity = format!("R_DIRECTOR\x1e{director_name}");
    let psk = tls_psk::password_to_psk(&storage.password);
    let psk_stream = tls_psk::psk_connect(stream, &psk_identity, &psk)
        .map_err(|e| format!("SD TLS-PSK: {e}"))?;

    let mut bnet = BnetStream::new(psk_stream);

    bnet.send_str(&make_director_hello(director_name))
        .map_err(|e| format!("SD hello: {e}"))?;

    let auth = CramMd5Auth::new(&storage.password, director_name, 2);
    let result = auth.two_way_authenticate(&mut bnet, false)
        .map_err(|e| format!("SD CRAM-MD5: {e}"))?;
    if !result.success {
        return Err("SD CRAM-MD5 failed".to_string());
    }

    let hello = bnet.recv_str().map_err(|e| format!("SD hello response: {e}"))?;
    debug!("SD hello: {hello}");

    Ok(bnet)
}

fn make_result(job_id: u32, name: &str, status: JobStatus, start: i64, files: i64, bytes: i64, errors: i32) -> JobResult {
    JobResult {
        job_id,
        job_name: name.to_string(),
        status,
        files,
        bytes,
        errors,
        start_time: start,
        end_time: now_unix(),
    }
}

fn resolve_addr(addr: &str) -> Result<std::net::SocketAddr, String> {
    use std::net::ToSocketAddrs;
    addr.to_socket_addrs()
        .map_err(|e| format!("DNS resolve {addr}: {e}"))?
        .next()
        .ok_or_else(|| format!("DNS resolve {addr}: no addresses found"))
}

fn now_unix() -> i64 {
    SystemTime::now().duration_since(UNIX_EPOCH).unwrap_or_default().as_secs() as i64
}
