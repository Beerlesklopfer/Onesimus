//! File Daemon protocol handler.
//!
//! Implements the Director-side of FD communication:
//! - Connect outbound to FD (TCP + TLS + CRAM-MD5)
//! - Send hello, authenticate, get status
//! - Job info exchange (backup/restore)

use std::net::TcpStream;
use std::time::Duration;

use tracing::{info, debug, warn, error};

use bareos_bnet::{BnetStream, BnetSignal, BnetMessage, BnetError, CramMd5Auth};
use bareos_bnet::hello::{make_director_hello, FD_OK_HELLO};

/// Result of an FD status query.
#[derive(Debug)]
pub struct FdStatusResult {
    pub client_name: String,
    pub version: String,
    pub status_text: String,
    pub success: bool,
}

/// Connect to a File Daemon and get its status.
pub fn query_fd_status(
    address: &str,
    port: u16,
    password: &str,
    director_name: &str,
    client_name: &str,
) -> FdStatusResult {
    let addr = format!("{address}:{port}");

    // Connect
    let stream = match TcpStream::connect_timeout(
        &addr.parse().unwrap_or_else(|_| format!("127.0.0.1:{port}").parse().unwrap()),
        Duration::from_secs(10),
    ) {
        Ok(s) => s,
        Err(e) => {
            return FdStatusResult {
                client_name: client_name.to_string(),
                version: String::new(),
                status_text: format!("Failed to connect to {addr}: {e}"),
                success: false,
            };
        }
    };

    stream.set_read_timeout(Some(Duration::from_secs(30))).ok();
    let mut bnet = BnetStream::new(stream);

    // TODO: TLS handshake if configured

    // Send Director hello
    let hello = make_director_hello(director_name);
    if let Err(e) = bnet.send_str(&hello) {
        return FdStatusResult {
            client_name: client_name.to_string(),
            version: String::new(),
            status_text: format!("Failed to send hello: {e}"),
            success: false,
        };
    }

    // CRAM-MD5 authentication (Director is client = initiated_by_remote=false)
    let auth = CramMd5Auth::new(password, director_name, 0);
    match auth.two_way_authenticate(&mut bnet, false) {
        Ok(result) if result.success => {
            debug!("FD {client_name}: auth OK");
        }
        Ok(_) => {
            return FdStatusResult {
                client_name: client_name.to_string(),
                version: String::new(),
                status_text: "Authentication failed".to_string(),
                success: false,
            };
        }
        Err(e) => {
            return FdStatusResult {
                client_name: client_name.to_string(),
                version: String::new(),
                status_text: format!("Auth error: {e}"),
                success: false,
            };
        }
    }

    // Receive FD hello response: "2000 OK Hello" or "2000 OK Hello <version>"
    let fd_hello = match bnet.recv_str() {
        Ok(msg) => msg,
        Err(e) => {
            return FdStatusResult {
                client_name: client_name.to_string(),
                version: String::new(),
                status_text: format!("Failed to receive FD hello: {e}"),
                success: false,
            };
        }
    };

    let version = if fd_hello.starts_with(FD_OK_HELLO) {
        fd_hello.trim_start_matches(FD_OK_HELLO).trim().to_string()
    } else {
        String::new()
    };

    // Send "status" command to FD
    if let Err(e) = bnet.send_str("status") {
        return FdStatusResult {
            client_name: client_name.to_string(),
            version: version.clone(),
            status_text: format!("Failed to send status: {e}"),
            success: false,
        };
    }

    // Read status response until EOD
    let mut status_text = String::new();
    loop {
        match bnet.recv() {
            Ok(BnetMessage::Data(data)) => {
                status_text.push_str(&String::from_utf8_lossy(&data));
            }
            Ok(BnetMessage::Signal(BnetSignal::Eod)) => break,
            Ok(BnetMessage::Signal(BnetSignal::Terminate)) => break,
            Ok(BnetMessage::Signal(_)) => continue,
            Err(_) => break,
        }
    }

    // Cleanup
    let _ = bnet.send_signal(BnetSignal::Terminate);

    FdStatusResult {
        client_name: client_name.to_string(),
        version,
        status_text,
        success: true,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn fd_status_connect_refused() {
        // Try to connect to a port that's not listening
        let result = query_fd_status("127.0.0.1", 19999, "pass", "dir", "test-fd");
        assert!(!result.success);
        assert!(result.status_text.contains("Failed to connect") ||
                result.status_text.contains("Connection refused"));
    }
}
