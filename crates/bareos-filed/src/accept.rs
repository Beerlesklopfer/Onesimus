//! TCP accept loop — fd:accept thread.
//!
//! Listens for incoming BNET connections from Directors.
//! Each connection gets its own named thread (fd:session:N).
//! Supports TLS-PSK (required for Director connections).

use std::net::{TcpListener, TcpStream};
use std::sync::Arc;
use std::sync::atomic::{AtomicBool, AtomicU64, Ordering};
use std::thread;

use tracing::{info, error, debug, warn};

use bareos_bnet::{BnetStream, BnetMessage, BnetSignal, BnetError, CramMd5Auth};
use bareos_bnet::hello::parse_hello;
use bareos_bnet::tls_psk;

use crate::config::FdConfig;
use crate::status;
use crate::backup;

static NEXT_SESSION: AtomicU64 = AtomicU64::new(1);

/// Main accept loop for the File Daemon.
pub fn accept_loop(config: &Arc<FdConfig>, shutdown: &AtomicBool) {
    // Bind dual-stack (IPv4 + IPv6) — localhost kann ::1 oder 127.0.0.1 sein
    let addr = format!("[::]:{}", config.port);

    let listener = match TcpListener::bind(&addr) {
        Ok(l) => {
            info!("listening on {addr}");
            l
        }
        Err(e) => {
            error!("FATAL: failed to bind {addr}: {e}");
            error!("Another file daemon already running? Check: ss -tlnp | grep {}", config.port);
            std::process::exit(1);
        }
    };

    listener.set_nonblocking(true).ok();

    while !shutdown.load(Ordering::Relaxed) {
        match listener.accept() {
            Ok((stream, peer_addr)) => {
                debug!("new connection from {peer_addr}");
                stream.set_nonblocking(false).ok();

                let cfg = config.clone();
                let session_id = NEXT_SESSION.fetch_add(1, Ordering::Relaxed);

                thread::Builder::new()
                    .name(format!("fd:session:{session_id}"))
                    .spawn(move || {
                        handle_connection(stream, session_id, &cfg);
                    })
                    .ok();
            }
            Err(ref e) if e.kind() == std::io::ErrorKind::WouldBlock => {
                std::thread::sleep(std::time::Duration::from_millis(100));
            }
            Err(e) => {
                warn!("accept error: {e}");
                std::thread::sleep(std::time::Duration::from_millis(100));
            }
        }
    }

    info!("accept loop stopped");
}

/// Handle a single incoming connection.
fn handle_connection(stream: TcpStream, session_id: u64, config: &FdConfig) {
    stream.set_read_timeout(Some(std::time::Duration::from_secs(30))).ok();

    // Peek first byte: 0x16 = TLS ClientHello, else plaintext BNET
    let mut peek_buf = [0u8; 1];
    match stream.peek(&mut peek_buf) {
        Ok(1) => {
            if peek_buf[0] == 0x16 {
                // TLS-PSK — try all configured director passwords
                for dir in &config.directors {
                    let psk = tls_psk::password_to_psk(&dir.password);
                    match tls_psk::psk_accept(stream.try_clone().unwrap(), &psk) {
                        Ok(psk_stream) => {
                            debug!("session {session_id}: TLS-PSK accept OK");
                            handle_session(BnetStream::new(psk_stream), session_id, config);
                            return;
                        }
                        Err(_) => continue,
                    }
                }
                // No PSK matched — try first password as fallback
                if let Some(dir) = config.directors.first() {
                    let psk = tls_psk::password_to_psk(&dir.password);
                    if let Ok(psk_stream) = tls_psk::psk_accept(stream, &psk) {
                        handle_session(BnetStream::new(psk_stream), session_id, config);
                        return;
                    }
                }
                debug!("session {session_id}: TLS-PSK failed for all directors");
            } else {
                // Plaintext BNET
                debug!("session {session_id}: plaintext connection");
                handle_session(BnetStream::new(stream), session_id, config);
            }
        }
        _ => {
            debug!("session {session_id}: peek failed");
        }
    }
}

/// Handle authenticated session — Hello + CRAM-MD5 + command loop.
fn handle_session<S: std::io::Read + std::io::Write>(
    mut bnet: BnetStream<S>,
    session_id: u64,
    config: &FdConfig,
) {
    // 1. Read Hello from Director
    let hello_msg = match bnet.recv_str() {
        Ok(msg) => msg,
        Err(e) => {
            debug!("session {session_id}: failed to read hello: {e}");
            return;
        }
    };

    let hello = match parse_hello(&hello_msg) {
        Some(h) => h,
        None => {
            warn!("session {session_id}: invalid hello: {hello_msg}");
            return;
        }
    };

    // Extract director name from Hello
    let dir_name = match &hello {
        bareos_bnet::Hello::DefaultConsole => "*UserAgent*".to_string(),
        bareos_bnet::Hello::NamedConsole { name, .. } => name.clone(),
        bareos_bnet::Hello::FileDaemon { .. } => {
            warn!("session {session_id}: FD-to-FD connection not supported");
            return;
        }
    };

    debug!("session {session_id}: hello from director '{dir_name}'");

    // 2. Find Director auth
    let dir_auth = match config.find_director(&dir_name) {
        Some(d) => d,
        None => {
            warn!("session {session_id}: unknown director '{dir_name}'");
            return;
        }
    };

    // 3. CRAM-MD5 two-way authentication (FD is server side)
    let auth = CramMd5Auth::new(&dir_auth.password, &config.name, 0);
    match auth.two_way_authenticate(&mut bnet, true) {
        Ok(result) if result.success => {
            debug!("session {session_id}: CRAM-MD5 auth OK for director '{dir_name}'");
        }
        Ok(_) => {
            warn!("session {session_id}: CRAM-MD5 auth failed for '{dir_name}'");
            return;
        }
        Err(e) => {
            warn!("session {session_id}: CRAM-MD5 error: {e}");
            return;
        }
    }

    // 4. Send OK Hello response
    let version = env!("CARGO_PKG_VERSION");
    let hostname = hostname();
    let ok_hello = format!("2000 OK Hello {version} {hostname}");
    if bnet.send_str(&ok_hello).is_err() { return; }

    info!("session {session_id}: director '{dir_name}' authenticated");

    // 5. Command loop
    loop {
        match bnet.recv() {
            Ok(BnetMessage::Data(data)) => {
                let cmd = String::from_utf8_lossy(&data).trim().to_string();
                if cmd.is_empty() { continue; }

                debug!("session {session_id}: command '{cmd}'");

                match cmd.split_whitespace().next().unwrap_or("") {
                    "status" | ".status" => {
                        let output = status::fd_status(config);
                        let _ = bnet.send_str(&output);
                        let _ = bnet.send_signal(BnetSignal::Eod);
                    }
                    "quit" | ".quit" => {
                        let _ = bnet.send_str("2000 OK bye\n");
                        let _ = bnet.send_signal(BnetSignal::Terminate);
                        break;
                    }
                    "JobId" => {
                        // Director sends job info: "JobId=N Job=name SDid=1 ..."
                        // This starts a backup/restore session
                        backup::handle_job_command(&cmd, &mut bnet, session_id, config);
                    }
                    "estimate" => {
                        let _ = bnet.send_str("2000 OK estimate files=0 bytes=0\n");
                        let _ = bnet.send_signal(BnetSignal::Eod);
                    }
                    "setdebug" => {
                        let _ = bnet.send_str("2000 OK setdebug\n");
                        let _ = bnet.send_signal(BnetSignal::Eod);
                    }
                    other => {
                        debug!("session {session_id}: unknown command '{other}'");
                        let _ = bnet.send_str(&format!("2902 Error: unknown command {other}\n"));
                        let _ = bnet.send_signal(BnetSignal::Eod);
                    }
                }
            }
            Ok(BnetMessage::Signal(BnetSignal::Terminate)) => {
                debug!("session {session_id}: director disconnected");
                break;
            }
            Ok(BnetMessage::Signal(_)) => continue,
            Err(BnetError::HardEof) | Err(BnetError::Terminated) => {
                debug!("session {session_id}: connection closed");
                break;
            }
            Err(e) => {
                debug!("session {session_id}: recv: {e}");
                break;
            }
        }
    }

    info!("session {session_id} ({dir_name}) ended");
}

fn hostname() -> String {
    nix::unistd::gethostname()
        .ok()
        .and_then(|h| h.into_string().ok())
        .unwrap_or_else(|| "unknown".to_string())
}
