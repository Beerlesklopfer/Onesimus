//! File Daemon protocol handler.
//!
//! Bareos 18.2+ Director→FD connection sequence:
//! 1. TCP connect
//! 2. TLS-PSK handshake (BEFORE any BNET messages!)
//! 3. Hello: "Hello Director <name> calling\n" (encrypted)
//! 4. CRAM-MD5 Two-Way (encrypted)
//! 5. FD response: "2000 OK Hello [version]"
//! 6. Commands on encrypted channel

use std::net::TcpStream;
use std::time::Duration;

use tracing::{debug, warn};

use bareos_bnet::{BnetStream, BnetSignal, BnetMessage, BnetError, CramMd5Auth};
use bareos_bnet::hello::{make_director_hello, FD_OK_HELLO};
use bareos_bnet::tls_psk;

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

    // 1. TCP Connect
    let stream = match TcpStream::connect_timeout(
        &{
            use std::net::ToSocketAddrs;
            addr.to_socket_addrs().ok().and_then(|mut a| a.next())
                .unwrap_or_else(|| format!("127.0.0.1:{port}").parse().unwrap())
        },
        Duration::from_secs(10),
    ) {
        Ok(s) => s,
        Err(e) => return fail(client_name, &format!("Connect to {addr} failed: {e}")),
    };
    stream.set_read_timeout(Some(Duration::from_secs(30))).ok();

    // 2. TLS-PSK handshake FIRST (Bareos 18.2+ requirement)
    // Identity format: "R_DIRECTOR\x1e<name>" with Record Separator
    let psk_identity = format!("R_DIRECTOR\x1e{director_name}");
    let psk = tls_psk::password_to_psk(password);
    let psk_result = tls_psk::psk_connect(stream, &psk_identity, &psk);

    // Branch based on TLS result — different stream types
    match psk_result {
        Ok(psk_stream) => {
            debug!("FD {client_name}: TLS-PSK handshake OK");
            return do_fd_status(BnetStream::new(psk_stream), password, director_name, client_name);
        }
        Err(e) => {
            debug!("FD {client_name}: TLS-PSK failed ({e}), trying cleartext");
            let stream2 = match TcpStream::connect_timeout(
                &addr.parse().unwrap(), Duration::from_secs(10),
            ) {
                Ok(s) => s,
                Err(e2) => return fail(client_name, &format!("Reconnect failed: {e2}")),
            };
            stream2.set_read_timeout(Some(Duration::from_secs(30))).ok();
            return do_fd_status(BnetStream::new(stream2), password, director_name, client_name);
        }
    }

}

/// Common FD status flow after stream is established (TLS or plaintext).
fn do_fd_status<S: std::io::Read + std::io::Write>(
    mut bnet: BnetStream<S>,
    password: &str,
    director_name: &str,
    client_name: &str,
) -> FdStatusResult {
    // 3. Send Hello
    let hello = make_director_hello(director_name);
    if let Err(e) = bnet.send_str(&hello) {
        return fail(client_name, &format!("Hello send failed: {e}"));
    }

    // 4. CRAM-MD5 Two-Way (Director is client = initiated_by_remote=false)
    let auth = CramMd5Auth::new(password, director_name, 2);
    match auth.two_way_authenticate(&mut bnet, false) {
        Ok(r) if r.success => debug!("FD {client_name}: CRAM-MD5 auth OK"),
        Ok(_) => return fail(client_name, "CRAM-MD5 authentication failed"),
        Err(e) => return fail(client_name, &format!("CRAM-MD5 error: {e}")),
    };

    // 5. Receive FD hello response: "2000 OK Hello [version]"
    let fd_hello = match bnet.recv_str() {
        Ok(msg) => msg,
        Err(e) => return fail(client_name, &format!("FD hello recv failed: {e}")),
    };

    let version = if fd_hello.starts_with(FD_OK_HELLO) {
        fd_hello.trim_start_matches(FD_OK_HELLO).trim().to_string()
    } else {
        debug!("FD {client_name}: unexpected response: {fd_hello}");
        String::new()
    };

    // 6. Send "status" command
    if let Err(e) = bnet.send_str("status") {
        return fail(client_name, &format!("Status send failed: {e}"));
    }

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

    let _ = bnet.send_signal(BnetSignal::Terminate);

    FdStatusResult {
        client_name: client_name.to_string(),
        version,
        status_text,
        success: true,
    }
}

fn fail(client_name: &str, msg: &str) -> FdStatusResult {
    FdStatusResult {
        client_name: client_name.to_string(),
        version: String::new(),
        status_text: msg.to_string(),
        success: false,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn fd_status_connect_refused() {
        let result = query_fd_status("127.0.0.1", 19999, "pass", "dir", "test-fd");
        assert!(!result.success);
    }
}
