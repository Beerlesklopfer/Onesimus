//! TCP accept loop — dir:accept thread.
//!
//! Listens for incoming BNET connections from consoles and file daemons.
//! Each connection gets its own named thread.

use std::net::{TcpListener, TcpStream};
use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

use tracing::{info, error, debug, warn};

use bareos_bnet::{BnetStream, Hello};
use bareos_bnet::hello::parse_hello;

use crate::session_manager::SessionManager;
use crate::command_queue::CommandQueue;
use crate::session;

/// Accept loop configuration.
pub struct AcceptConfig {
    pub bind_address: String,
    pub port: u16,
}

impl Default for AcceptConfig {
    fn default() -> Self {
        Self {
            bind_address: "0.0.0.0".to_string(),
            port: 9101,
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

    // Set non-blocking so we can check shutdown flag
    listener.set_nonblocking(true).ok();

    while !shutdown.load(Ordering::Relaxed) {
        match listener.accept() {
            Ok((stream, peer_addr)) => {
                debug!("new connection from {peer_addr}");
                stream.set_nonblocking(false).ok();

                let smgr = session_mgr.clone();
                let q = cmd_queue.clone();
                let session_id = crate::session_manager::next_session_id();

                thread::Builder::new()
                    .name(format!("dir:session:{session_id}"))
                    .spawn(move || {
                        handle_connection(stream, session_id, &smgr, &q);
                    })
                    .ok();
            }
            Err(ref e) if e.kind() == std::io::ErrorKind::WouldBlock => {
                // No connection available, sleep briefly
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
    session_mgr: &SessionManager,
    cmd_queue: &CommandQueue,
) {
    let mut bnet = BnetStream::new(stream);

    // TODO(R8-full): TLS handshake first (if configured)

    // Read hello message
    let hello_msg = match bnet.recv_str() {
        Ok(msg) => msg,
        Err(e) => {
            debug!("failed to read hello: {e}");
            return;
        }
    };

    let hello = match parse_hello(&hello_msg) {
        Some(h) => h,
        None => {
            warn!("invalid hello message: {hello_msg}");
            return;
        }
    };

    match hello {
        Hello::DefaultConsole => {
            debug!("default console connected (session {session_id})");
            session::handle_console_session(
                bnet, session_id, "*UserAgent*", session_mgr, cmd_queue,
            );
        }
        Hello::NamedConsole { name, version, wants_grpc } => {
            debug!("named console '{name}' v{version} connected (grpc={wants_grpc})");
            session::handle_console_session(
                bnet, session_id, &name, session_mgr, cmd_queue,
            );
        }
        Hello::FileDaemon { client_name, fd_protocol_version } => {
            debug!("FD '{client_name}' connected (proto={fd_protocol_version})");
            session_mgr.add_fd_connection(&client_name, fd_protocol_version);
            // FD connections are pooled — the socket waits for a job to use it
            // TODO(R8-full): Store the BnetStream in the FD connection pool
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::net::TcpListener;

    #[test]
    fn accept_config_defaults() {
        let config = AcceptConfig::default();
        assert_eq!(config.bind_address, "0.0.0.0");
        assert_eq!(config.port, 9101);
    }

    #[test]
    fn hello_routing() {
        // Test that parse_hello correctly identifies connection types
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
