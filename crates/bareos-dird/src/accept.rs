//! TCP accept loop — dir:accept thread.
//!
//! Listens for incoming BNET connections from consoles and file daemons.
//! Each connection gets its own named thread.
//! Supports optional TLS (required for C++ bconsole compatibility).

use std::net::{TcpListener, TcpStream};
use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

use tracing::{info, error, debug, warn};

use bareos_bnet::{BnetStream, Hello};
use bareos_bnet::hello::parse_hello;
use bareos_bnet::tls::{self, TlsConfig, TlsOrPlain};

use crate::session_manager::SessionManager;
use crate::command_queue::CommandQueue;
use crate::session;

/// Accept loop configuration.
pub struct AcceptConfig {
    pub bind_address: String,
    pub port: u16,
    pub password: String,
    pub tls_config: Option<TlsConfig>,
}

impl Default for AcceptConfig {
    fn default() -> Self {
        Self {
            bind_address: "0.0.0.0".to_string(),
            port: 9101,
            password: String::new(),
            tls_config: None,
        }
    }
}

/// Main accept loop. Runs in the dir:accept named thread.
pub fn accept_loop(
    session_mgr: &Arc<SessionManager>,
    cmd_queue: &Arc<CommandQueue>,
    config: &AcceptConfig,
    shutdown: &AtomicBool,
) {
    let addr = format!("{}:{}", config.bind_address, config.port);

    let listener = match TcpListener::bind(&addr) {
        Ok(l) => {
            info!("listening on {addr}");
            l
        }
        Err(e) => {
            error!("failed to bind {addr}: {e}");
            return;
        }
    };

    // Build TLS server config if certs are provided
    let tls_server_config = config.tls_config.as_ref().and_then(|tc| {
        if tc.certfile.is_some() && tc.keyfile.is_some() {
            match tls::build_server_config(tc) {
                Ok(cfg) => {
                    info!("TLS enabled");
                    Some(cfg)
                }
                Err(e) => {
                    error!("TLS config error: {e}");
                    None
                }
            }
        } else {
            None
        }
    });

    // Set non-blocking so we can check shutdown flag
    listener.set_nonblocking(true).ok();

    while !shutdown.load(Ordering::Relaxed) {
        match listener.accept() {
            Ok((stream, peer_addr)) => {
                debug!("new connection from {peer_addr}");
                stream.set_nonblocking(false).ok();

                let smgr = session_mgr.clone();
                let q = cmd_queue.clone();
                let password = config.password.clone();
                let tls_cfg = tls_server_config.clone();
                let session_id = crate::session_manager::next_session_id();

                thread::Builder::new()
                    .name(format!("dir:session:{session_id}"))
                    .spawn(move || {
                        handle_connection(stream, session_id, &password, tls_cfg, &smgr, &q);
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
fn handle_connection(
    stream: TcpStream,
    session_id: u64,
    password: &str,
    tls_config: Option<Arc<rustls::ServerConfig>>,
    session_mgr: &SessionManager,
    cmd_queue: &CommandQueue,
) {
    // TLS handshake if configured (before reading any BNET messages)
    let tls_stream: TlsOrPlain = if let Some(tls_cfg) = tls_config {
        match tls::tls_accept(stream, &tls_cfg) {
            Ok(tls) => {
                debug!("session {session_id}: TLS handshake OK");
                TlsOrPlain::TlsServer(tls)
            }
            Err(e) => {
                // TLS failed — try plaintext fallback (non-TLS client)
                debug!("session {session_id}: TLS failed ({e}), connection dropped");
                return;
            }
        }
    } else {
        TlsOrPlain::Plain(stream)
    };

    let mut bnet = BnetStream::new(tls_stream);

    // Read hello message
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

    match hello {
        Hello::DefaultConsole => {
            debug!("default console connected (session {session_id})");
            session::handle_console_session(
                bnet, session_id, "*UserAgent*", password, session_mgr, cmd_queue,
            );
        }
        Hello::NamedConsole { name, version, wants_grpc } => {
            debug!("named console '{name}' v{version} connected (grpc={wants_grpc})");
            session::handle_console_session(
                bnet, session_id, &name, password, session_mgr, cmd_queue,
            );
        }
        Hello::FileDaemon { client_name, fd_protocol_version } => {
            debug!("FD '{client_name}' connected (proto={fd_protocol_version})");
            session_mgr.add_fd_connection(&client_name, fd_protocol_version);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn accept_config_defaults() {
        let config = AcceptConfig::default();
        assert_eq!(config.bind_address, "0.0.0.0");
        assert_eq!(config.port, 9101);
    }

    #[test]
    fn hello_routing() {
        assert!(matches!(
            parse_hello("Hello *UserAgent* calling"),
            Some(Hello::DefaultConsole)
        ));
        assert!(matches!(
            parse_hello("Hello admin calling version 25.0.0"),
            Some(Hello::NamedConsole { .. })
        ));
        assert!(matches!(
            parse_hello("Hello Client bareos-fd FdProtocolVersion=54 calling"),
            Some(Hello::FileDaemon { .. })
        ));
    }
}
