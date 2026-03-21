//! Storage Daemon protocol handler.
//!
//! Bareos Director→SD connection sequence (same as FD):
//! 1. TCP connect
//! 2. TLS-PSK handshake (R_DIRECTOR\x1e<name> identity)
//! 3. Hello: "Hello Director <name> calling\n"
//! 4. CRAM-MD5 Two-Way
//! 5. SD response: "3000 OK Hello"
//! 6. Commands on encrypted channel

use std::net::TcpStream;
use std::time::Duration;

use tracing::{debug, warn};

use bareos_bnet::{BnetStream, BnetSignal, BnetMessage, BnetError, CramMd5Auth};
use bareos_bnet::hello::{make_director_hello, SD_OK_HELLO};
use bareos_bnet::tls_psk;

/// Result of an SD status query.
#[derive(Debug)]
pub struct SdStatusResult {
    pub storage_name: String,
    pub status_text: String,
    pub success: bool,
}

/// Connect to a Storage Daemon and get its status.
pub fn query_sd_status(
    address: &str,
    port: u16,
    password: &str,
    director_name: &str,
    storage_name: &str,
) -> SdStatusResult {
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
        Err(e) => return fail(storage_name, &format!("Connect to {addr} failed: {e}")),
    };
    stream.set_read_timeout(Some(Duration::from_secs(30))).ok();

    // 2. TLS-PSK handshake (R_DIRECTOR\x1e<name>)
    let psk_identity = format!("R_DIRECTOR\x1e{director_name}");
    let psk = tls_psk::password_to_psk(password);
    let psk_result = tls_psk::psk_connect(stream, &psk_identity, &psk);

    match psk_result {
        Ok(psk_stream) => {
            debug!("SD {storage_name}: TLS-PSK OK");
            do_sd_status(BnetStream::new(psk_stream), password, director_name, storage_name)
        }
        Err(e) => {
            debug!("SD {storage_name}: TLS-PSK failed ({e}), trying cleartext");
            match TcpStream::connect_timeout(&addr.parse().unwrap(), Duration::from_secs(10)) {
                Ok(s2) => {
                    s2.set_read_timeout(Some(Duration::from_secs(30))).ok();
                    do_sd_status(BnetStream::new(s2), password, director_name, storage_name)
                }
                Err(e2) => fail(storage_name, &format!("Reconnect failed: {e2}")),
            }
        }
    }
}

fn do_sd_status<S: std::io::Read + std::io::Write>(
    mut bnet: BnetStream<S>,
    password: &str,
    director_name: &str,
    storage_name: &str,
) -> SdStatusResult {
    // 3. Hello
    let hello = make_director_hello(director_name);
    if let Err(e) = bnet.send_str(&hello) {
        return fail(storage_name, &format!("Hello failed: {e}"));
    }

    // 4. CRAM-MD5
    let auth = CramMd5Auth::new(password, director_name, 2);
    match auth.two_way_authenticate(&mut bnet, false) {
        Ok(r) if r.success => debug!("SD {storage_name}: CRAM-MD5 OK"),
        Ok(_) => return fail(storage_name, "CRAM-MD5 failed"),
        Err(e) => return fail(storage_name, &format!("CRAM-MD5 error: {e}")),
    };

    // 5. SD response: "3000 OK Hello"
    match bnet.recv_str() {
        Ok(msg) if msg.starts_with(SD_OK_HELLO) => {
            debug!("SD {storage_name}: hello OK");
        }
        Ok(msg) => {
            debug!("SD {storage_name}: unexpected: {msg}");
        }
        Err(e) => return fail(storage_name, &format!("SD hello recv failed: {e}")),
    }

    // 6. Send status
    if let Err(e) = bnet.send_str(".status") {
        return fail(storage_name, &format!("Status send failed: {e}"));
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

    SdStatusResult {
        storage_name: storage_name.to_string(),
        status_text,
        success: true,
    }
}

fn fail(name: &str, msg: &str) -> SdStatusResult {
    SdStatusResult {
        storage_name: name.to_string(),
        status_text: msg.to_string(),
        success: false,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn sd_status_connect_refused() {
        let result = query_sd_status("127.0.0.1", 19999, "pass", "dir", "File");
        assert!(!result.success);
    }
}
