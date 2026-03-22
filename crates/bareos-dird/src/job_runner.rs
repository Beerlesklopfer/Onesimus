//! Job runner — autonomous backup execution in fork()-child.
//!
//! Runs inside the forked worker process. Builds its own FD/SD
//! connections, executes the full backup protocol, and communicates
//! with the Director via pipes for CatReq and status.
//!
//! See project_backup_protocol.md for the complete protocol sequence.

use std::io::{Write, BufRead, BufReader};
use std::thread;
use std::time::{SystemTime, UNIX_EPOCH};

use tracing::{info, debug, warn, error};

use bareos_bnet::{BnetStream, BnetSignal, BnetMessage, CramMd5Auth};
use bareos_bnet::hello::make_director_hello;
use bareos_bnet::tls_psk;

use crate::job_worker::{JobParams, PipeMessage, EXIT_SUCCESS, EXIT_ERROR, EXIT_FATAL};

/// Run a backup job autonomously in the fork()-child process.
/// Returns an exit code (0=Success, 1=Error, 2=Fatal).
pub fn run_backup_autonomous(
    params: &JobParams,
    to_director: &mut std::fs::File,
    from_director: &mut std::fs::File,
) -> i32 {
    let start_time = now_unix();
    info!("Worker {}: starting backup (level={}), client={}:{}, storage={}:{}",
          params.job_id, params.level, params.fd_address, params.fd_port,
          params.sd_address, params.sd_port);

    // 1. Connect to FD
    let mut fd = match connect_fd(params) {
        Ok(b) => b,
        Err(e) => {
            error!("Worker {}: FD connect failed: {e}", params.job_id);
            send_job_end(to_director, params.job_id, EXIT_FATAL, 0, 0, 1);
            return EXIT_FATAL;
        }
    };
    info!("Worker {}: connected to FD", params.job_id);

    // 2. Connect to SD
    let mut sd = match connect_sd(params) {
        Ok(b) => b,
        Err(e) => {
            error!("Worker {}: SD connect failed: {e}", params.job_id);
            let _ = fd.send_signal(BnetSignal::Terminate);
            send_job_end(to_director, params.job_id, EXIT_FATAL, 0, 0, 1);
            return EXIT_FATAL;
        }
    };
    info!("Worker {}: connected to SD", params.job_id);

    // 3. SD: JobId → 3000 OK Job + Auth Key
    let sd_auth_key = match sd_job_setup(&mut sd, params) {
        Ok(key) => key,
        Err(e) => {
            error!("Worker {}: SD job setup failed: {e}", params.job_id);
            let _ = fd.send_signal(BnetSignal::Terminate);
            let _ = sd.send_signal(BnetSignal::Terminate);
            send_job_end(to_director, params.job_id, EXIT_FATAL, 0, 0, 1);
            return EXIT_FATAL;
        }
    };

    // 4. SD: use storage + use device → 3000 OK use device
    if let Err(e) = sd_use_device(&mut sd, params) {
        error!("Worker {}: SD use device failed: {e}", params.job_id);
        let _ = fd.send_signal(BnetSignal::Terminate);
        let _ = sd.send_signal(BnetSignal::Terminate);
        send_job_end(to_director, params.job_id, EXIT_FATAL, 0, 0, 1);
        return EXIT_FATAL;
    }

    // 5. SD: "run" BEFORE FD storage — SD enters WaitFD() and is ready when FD connects
    debug!("Worker {}: SD run", params.job_id);
    let _ = sd.send_str("run\n");

    // 6. FD: JobId + level + fileset + storage → 2000 OK *
    //    (storage command triggers FD→SD connect which wakes WaitFD)
    if let Err(e) = fd_job_setup(&mut fd, params, &sd_auth_key) {
        error!("Worker {}: FD setup failed: {e}", params.job_id);
        let _ = fd.send_signal(BnetSignal::Terminate);
        let _ = sd.send_signal(BnetSignal::Terminate);
        send_job_end(to_director, params.job_id, EXIT_FATAL, 0, 0, 1);
        return EXIT_FATAL;
    }

    // 7. FD: session + backup
    if let Err(e) = fd_start_backup(&mut fd, params) {
        error!("Worker {}: FD backup start failed: {e}", params.job_id);
        let _ = fd.send_signal(BnetSignal::Terminate);
        let _ = sd.send_signal(BnetSignal::Terminate);
        send_job_end(to_director, params.job_id, EXIT_FATAL, 0, 0, 1);
        return EXIT_FATAL;
    }

    info!("Worker {}: backup running, waiting for completion...", params.job_id);

    // 8. Two parallel tasks:
    //    a) SD thread: reads CatReqs, forwards to Director via pipe
    //    b) Main thread: reads FD until "2800 End Job"
    let job_id = params.job_id;
    let mut to_dir_clone = to_director.try_clone().unwrap();
    let mut from_dir_clone = from_director.try_clone().unwrap();

    let sd_handle = thread::Builder::new()
        .name(format!("sd-cat:{job_id}"))
        .spawn(move || {
            sd_catrec_loop(&mut sd, job_id, &mut to_dir_clone, &mut from_dir_clone);
        })
        .ok();

    // Main thread: wait for FD end-of-job
    let (files, bytes, errors, term_code) = wait_for_fd_end_job(&mut fd, job_id);

    // Cleanup
    let _ = fd.send_signal(BnetSignal::Terminate);
    if let Some(h) = sd_handle { h.join().ok(); }

    let exit_code = if term_code == 0 && errors == 0 { EXIT_SUCCESS }
                    else if term_code == 0 { EXIT_ERROR }
                    else { EXIT_FATAL };

    info!("Worker {}: backup finished — files={files} bytes={bytes} errors={errors} term={term_code}",
          params.job_id);

    send_job_end(to_director, params.job_id, exit_code, files, bytes, errors);
    exit_code
}

// ============================================================================
// SD Setup
// ============================================================================

fn sd_job_setup(
    sd: &mut BnetStream<tls_psk::PskStream>,
    p: &JobParams,
) -> Result<String, String> {
    let level_int = match p.level.as_str() {
        "full" => 70, "incremental" => 73, "differential" => 68, _ => 70,
    };
    let cmd = format!(
        "JobId={} job={} job_name={} client_name={} \
         type=66 level={level_int} FileSet=Unknown NoAttr=0 SpoolAttr=0 FileSetMD5=0 \
         SpoolData=0 PreferMountedVols=1 SpoolSize=0 \
         rerunning=0 VolSessionId=0 VolSessionTime=0 Quota=0 \
         Protocol=0 BackupFormat=NativeBackup\n",
        p.job_id, p.unique_job, p.job_name, p.fd_address
    );
    sd.send_str(&cmd).map_err(|e| format!("SD job send: {e}"))?;

    let resp = sd.recv_str().map_err(|e| format!("SD job recv: {e}"))?;
    debug!("Worker {} SD job: {}", p.job_id, resp.trim());

    resp.split_whitespace()
        .find(|s| s.starts_with("Authorization="))
        .and_then(|s| s.strip_prefix("Authorization="))
        .map(|s| s.to_string())
        .ok_or_else(|| format!("SD job: no Authorization in response: {resp}"))
}

fn sd_use_device(
    sd: &mut BnetStream<tls_psk::PskStream>,
    p: &JobParams,
) -> Result<(), String> {
    let use_stor = format!(
        "use storage=File media_type={} pool_name={} pool_type=Backup append=1 copy=0 stripe=0\n",
        p.sd_media_type, p.pool_name
    );
    sd.send_str(&use_stor).map_err(|e| format!("SD use storage: {e}"))?;

    let use_dev = format!("use device={}\n", p.sd_device);
    sd.send_str(&use_dev).map_err(|e| format!("SD use device: {e}"))?;
    sd.send_signal(BnetSignal::Eod).map_err(|e| format!("SD EOD1: {e}"))?;
    sd.send_signal(BnetSignal::Eod).map_err(|e| format!("SD EOD2: {e}"))?;

    // Read response, skip Jmsgs
    loop {
        match sd.recv_str() {
            Ok(r) if r.contains("3000 OK") => { debug!("Worker {} SD use: {}", p.job_id, r.trim()); return Ok(()); }
            Ok(r) if r.starts_with("Jmsg") => { debug!("Worker {} SD Jmsg: {}", p.job_id, r.trim()); continue; }
            Ok(r) => return Err(format!("SD use: {r}")),
            Err(e) => return Err(format!("SD use recv: {e}")),
        }
    }
}

// ============================================================================
// FD Setup
// ============================================================================

fn fd_job_setup(
    fd: &mut BnetStream<tls_psk::PskStream>,
    p: &JobParams,
    sd_auth_key: &str,
) -> Result<(), String> {
    // JobId with SD auth key + TLS policy
    let job_cmd = format!(
        "JobId={} Job={} SDid=1 SDtime={} Authorization={sd_auth_key} ssl=4\n",
        p.job_id, p.unique_job, now_unix()
    );
    send_expect(fd, &job_cmd, "2000 OK Job", p.job_id, "FD JobId")?;

    // Level
    let level_cmd = match p.level.as_str() {
        "incremental" | "differential" => format!("level = since_utime=0 mtime_only=0 \n"),
        _ => format!("level = {}\n", p.level),
    };
    send_expect(fd, &level_cmd, "2000 OK level", p.job_id, "FD level")?;

    // FileSet: I + F /path + N per include, E + F /path + N per exclude
    fd.send_str("fileset vss=0\n").map_err(|e| format!("fileset header: {e}"))?;
    for inc in &p.fileset_includes {
        fd.send_str("I\n").map_err(|e| format!("I: {e}"))?;
        for path in inc.lines() {
            let path = path.trim();
            if !path.is_empty() {
                fd.send_str(&format!("F {path}\n")).map_err(|e| format!("F: {e}"))?;
            }
        }
        fd.send_str("N\n").map_err(|e| format!("N: {e}"))?;
    }
    for exc in &p.fileset_excludes {
        fd.send_str("E\n").map_err(|e| format!("E: {e}"))?;
        for path in exc.lines() {
            let path = path.trim();
            if !path.is_empty() {
                fd.send_str(&format!("F {path}\n")).map_err(|e| format!("F: {e}"))?;
            }
        }
        fd.send_str("N\n").map_err(|e| format!("N: {e}"))?;
    }
    fd.send_signal(BnetSignal::Eod).map_err(|e| format!("fileset EOD: {e}"))?;
    // Read fileset response, skip Jmsgs
    loop {
        match fd.recv_str() {
            Ok(r) if r.contains("2000 OK") => { debug!("Worker {}: fileset accepted", p.job_id); break; }
            Ok(r) if r.starts_with("Jmsg") => continue,
            Ok(r) => return Err(format!("fileset: {r}")),
            Err(e) => return Err(format!("fileset recv: {e}")),
        }
    }

    // Storage address (FD connects to SD autonomously)
    let stor_cmd = format!("storage address={} port={} ssl=4\n", p.sd_address, p.sd_port);
    send_expect(fd, &stor_cmd, "2000 OK storage", p.job_id, "FD storage")?;

    Ok(())
}

fn fd_start_backup(
    fd: &mut BnetStream<tls_psk::PskStream>,
    p: &JobParams,
) -> Result<(), String> {
    // Session
    let session_cmd = format!("session {} 1 {} 0 0 0 0\n", "", now_unix());
    send_expect(fd, &session_cmd, "2000 OK session", p.job_id, "FD session")?;

    // Backup
    send_expect(fd, "backup\n", "2000 OK backup", p.job_id, "FD backup")?;

    Ok(())
}

// ============================================================================
// SD CatReq Proxy (runs in separate thread inside worker)
// ============================================================================

fn sd_catrec_loop(
    sd: &mut BnetStream<tls_psk::PskStream>,
    job_id: u32,
    to_director: &mut std::fs::File,
    from_director: &mut std::fs::File,
) {
    debug!("Worker {job_id}: SD CatReq thread started");
    loop {
        match sd.recv() {
            Ok(BnetMessage::Data(data)) => {
                let msg = String::from_utf8_lossy(&data).trim().to_string();

                if msg.starts_with("CatReq") {
                    // Forward CatReq to Director via pipe, wait for response
                    debug!("Worker {job_id} SD CatReq: {msg}");
                    crate::job_worker::send_pipe_msg(to_director, &PipeMessage::CatReq {
                        job_id, request: msg,
                    });
                    // Read response from Director
                    match crate::job_worker::recv_pipe_msg(from_director) {
                        Some(PipeMessage::CatResp { response }) => {
                            debug!("Worker {job_id} CatResp: {}", response.trim());
                            let _ = sd.send_str(&response);
                        }
                        other => {
                            warn!("Worker {job_id}: unexpected pipe msg: {other:?}");
                            let _ = sd.send_str("1000 OK\n");
                        }
                    }
                } else if msg.starts_with("3099 Job") {
                    debug!("Worker {job_id} SD: job end: {msg}");
                    break;
                } else if msg.starts_with("3010 Job") {
                    debug!("Worker {job_id} SD: job start: {msg}");
                } else if msg.starts_with("Jmsg") {
                    debug!("Worker {job_id} SD Jmsg: {msg}");
                } else if msg.starts_with("Update") {
                    let _ = sd.send_str("1000 OK\n");
                } else {
                    debug!("Worker {job_id} SD: {msg}");
                }
            }
            Ok(BnetMessage::Signal(BnetSignal::Eod)) => continue,
            Ok(BnetMessage::Signal(BnetSignal::Terminate)) => {
                debug!("Worker {job_id}: SD terminated");
                break;
            }
            Ok(BnetMessage::Signal(sig)) => {
                debug!("Worker {job_id} SD signal: {sig:?}");
            }
            Err(e) => {
                debug!("Worker {job_id} SD recv: {e}");
                break;
            }
        }
    }
    debug!("Worker {job_id}: SD CatReq thread ended");
    let _ = sd.send_signal(BnetSignal::Terminate);
}

// ============================================================================
// Wait for FD End-of-Job
// ============================================================================

fn wait_for_fd_end_job(
    fd: &mut BnetStream<tls_psk::PskStream>,
    job_id: u32,
) -> (i64, i64, i32, i32) {
    loop {
        match fd.recv() {
            Ok(BnetMessage::Data(data)) => {
                let msg = String::from_utf8_lossy(&data).trim().to_string();
                debug!("Worker {job_id} FD: {msg}");

                if msg.starts_with("2800 End Job") {
                    let mut term_code = 0i32;
                    let mut files = 0i64;
                    let mut bytes = 0i64;
                    let mut errors = 0i32;
                    for part in msg.split_whitespace() {
                        if let Some(v) = part.strip_prefix("TermCode=") { term_code = v.parse().unwrap_or(0); }
                        else if let Some(v) = part.strip_prefix("JobFiles=") { files = v.parse().unwrap_or(0); }
                        else if let Some(v) = part.strip_prefix("JobBytes=") { bytes = v.parse().unwrap_or(0); }
                        else if let Some(v) = part.strip_prefix("Errors=") { errors = v.parse().unwrap_or(0); }
                    }
                    return (files, bytes, errors, term_code);
                }
            }
            Ok(BnetMessage::Signal(BnetSignal::Eod)) => continue,
            Ok(BnetMessage::Signal(BnetSignal::Terminate)) => {
                warn!("Worker {job_id}: FD terminated before end-of-job");
                return (0, 0, 1, 2);
            }
            Ok(BnetMessage::Signal(_)) => continue,
            Err(e) => {
                error!("Worker {job_id}: FD recv error: {e}");
                return (0, 0, 1, 2);
            }
        }
    }
}

// ============================================================================
// Connection helpers
// ============================================================================

fn connect_fd(p: &JobParams) -> Result<BnetStream<tls_psk::PskStream>, String> {
    let addr = format!("{}:{}", p.fd_address, p.fd_port);
    let sock = resolve_and_connect(&addr, 3)?;
    sock.set_read_timeout(None).ok(); // No timeout — backup can take hours

    let psk_id = format!("R_DIRECTOR\x1e{}", p.director_name);
    let psk = tls_psk::password_to_psk(&p.fd_password);
    let stream = tls_psk::psk_connect(sock, &psk_id, &psk)
        .map_err(|e| format!("FD TLS-PSK: {e}"))?;

    let mut bnet = BnetStream::new(stream);
    bnet.send_str(&make_director_hello(&p.director_name))
        .map_err(|e| format!("FD hello: {e}"))?;

    let auth = CramMd5Auth::new(&p.fd_password, &p.director_name, 2);
    let result = auth.two_way_authenticate(&mut bnet, false)
        .map_err(|e| format!("FD CRAM-MD5: {e}"))?;
    if !result.success { return Err("FD CRAM-MD5 failed".into()); }

    let hello = bnet.recv_str().map_err(|e| format!("FD hello resp: {e}"))?;
    if !hello.contains("2000 OK Hello") {
        return Err(format!("FD unexpected: {hello}"));
    }
    Ok(bnet)
}

fn connect_sd(p: &JobParams) -> Result<BnetStream<tls_psk::PskStream>, String> {
    let addr = format!("{}:{}", p.sd_address, p.sd_port);
    let sock = resolve_and_connect(&addr, 3)?;
    sock.set_read_timeout(None).ok();

    let psk_id = format!("R_DIRECTOR\x1e{}", p.director_name);
    let psk = tls_psk::password_to_psk(&p.sd_password);
    let stream = tls_psk::psk_connect(sock, &psk_id, &psk)
        .map_err(|e| format!("SD TLS-PSK: {e}"))?;

    let mut bnet = BnetStream::new(stream);
    bnet.send_str(&make_director_hello(&p.director_name))
        .map_err(|e| format!("SD hello: {e}"))?;

    let auth = CramMd5Auth::new(&p.sd_password, &p.director_name, 2);
    let result = auth.two_way_authenticate(&mut bnet, false)
        .map_err(|e| format!("SD CRAM-MD5: {e}"))?;
    if !result.success { return Err("SD CRAM-MD5 failed".into()); }

    let hello = bnet.recv_str().map_err(|e| format!("SD hello resp: {e}"))?;
    debug!("SD hello: {}", hello.trim());
    Ok(bnet)
}

fn resolve_and_connect(addr: &str, timeout_secs: u64) -> Result<std::net::TcpStream, String> {
    use std::net::ToSocketAddrs;
    let sock_addr = addr.to_socket_addrs()
        .map_err(|e| format!("DNS {addr}: {e}"))?
        .next()
        .ok_or_else(|| format!("DNS {addr}: no addresses"))?;
    std::net::TcpStream::connect_timeout(&sock_addr, std::time::Duration::from_secs(timeout_secs))
        .map_err(|e| format!("connect {addr}: {e}"))
}

// ============================================================================
// Helpers
// ============================================================================

/// Send command, skip Jmsgs, expect specific response prefix.
fn send_expect(
    bnet: &mut BnetStream<tls_psk::PskStream>,
    cmd: &str,
    expect: &str,
    job_id: u32,
    label: &str,
) -> Result<String, String> {
    debug!("Worker {job_id} {label}: sending '{}'", cmd.trim());
    bnet.send_str(cmd).map_err(|e| format!("{label} send: {e}"))?;

    loop {
        match bnet.recv() {
            Ok(BnetMessage::Data(data)) => {
                let resp = String::from_utf8_lossy(&data).to_string();
                if resp.contains(expect) {
                    debug!("Worker {job_id} {label}: {}", resp.trim());
                    return Ok(resp);
                } else if resp.starts_with("Jmsg") || resp.starts_with("2001") {
                    debug!("Worker {job_id} {label} Jmsg: {}", resp.trim());
                    continue;
                } else {
                    error!("Worker {job_id} {label}: unexpected: {}", resp.trim());
                    return Err(format!("{label}: {resp}"));
                }
            }
            Ok(BnetMessage::Signal(BnetSignal::Eod)) => continue,
            Ok(BnetMessage::Signal(sig)) => { debug!("Worker {job_id} {label}: signal {sig:?}"); continue; }
            Err(e) => {
                error!("Worker {job_id} {label}: recv: {e}");
                return Err(format!("{label}: {e}"));
            }
        }
    }
}

fn send_job_end(pipe: &mut std::fs::File, job_id: u32, term_code: i32, files: i64, bytes: i64, errors: i32) {
    crate::job_worker::send_pipe_msg(pipe, &PipeMessage::JobEnd {
        job_id, term_code, files, bytes, errors,
    });
}

fn now_unix() -> i64 {
    SystemTime::now().duration_since(UNIX_EPOCH).unwrap_or_default().as_secs() as i64
}
