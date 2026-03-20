//! TLS support for Bareos — PSK and X.509 certificate modes.
//!
//! Bareos supports two TLS modes:
//! - PSK (Pre-Shared Key): Default for daemon-to-daemon, key from password
//! - X.509 Certificates: Optional, configured via TLS Certificate/Key/CA
//!
//! Uses OpenSSL via the `openssl` crate (rustls doesn't support PSK).

use std::io::{self, Read, Write};
use std::net::TcpStream;

use openssl::ssl::{SslMethod, SslConnector, SslAcceptor, SslStream, SslFiletype, SslVerifyMode};

/// TLS stream (PSK or certificate-based) wrapping a TCP connection.
pub struct PskStream {
    inner: SslStream<TcpStream>,
}

impl Read for PskStream {
    fn read(&mut self, buf: &mut [u8]) -> io::Result<usize> {
        self.inner.read(buf)
    }
}

impl Write for PskStream {
    fn write(&mut self, buf: &[u8]) -> io::Result<usize> {
        self.inner.write(buf)
    }

    fn flush(&mut self) -> io::Result<()> {
        self.inner.flush()
    }
}

const BAREOS_CIPHERS: &str = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH";

// ============================================================================
// PSK mode
// ============================================================================

/// Connect via TLS-PSK (client mode).
pub fn psk_connect(stream: TcpStream, identity: &str, psk: &[u8]) -> io::Result<PskStream> {
    let mut builder = SslConnector::builder(SslMethod::tls())
        .map_err(ssl_err)?;

    let identity_owned = identity.to_string();
    let psk_owned = psk.to_vec();
    builder.set_psk_client_callback(move |_ssl, _hint, identity_buf, psk_buf| {
        let id_bytes = identity_owned.as_bytes();
        let id_len = id_bytes.len().min(identity_buf.len() - 1);
        identity_buf[..id_len].copy_from_slice(&id_bytes[..id_len]);
        identity_buf[id_len] = 0;
        let psk_len = psk_owned.len().min(psk_buf.len());
        psk_buf[..psk_len].copy_from_slice(&psk_owned[..psk_len]);
        Ok(psk_len)
    });

    builder.set_cipher_list(BAREOS_CIPHERS).map_err(ssl_err)?;
    let connector = builder.build();
    let ssl_stream = connector.connect("bareos", stream)
        .map_err(|e| io::Error::new(io::ErrorKind::Other, format!("PSK connect: {e}")))?;
    Ok(PskStream { inner: ssl_stream })
}

/// Accept via TLS-PSK (server mode).
pub fn psk_accept(stream: TcpStream, psk: &[u8]) -> io::Result<PskStream> {
    let mut builder = SslAcceptor::mozilla_intermediate(SslMethod::tls())
        .map_err(ssl_err)?;

    let psk_owned = psk.to_vec();
    builder.set_psk_server_callback(move |_ssl, _identity, psk_buf| {
        let psk_len = psk_owned.len().min(psk_buf.len());
        psk_buf[..psk_len].copy_from_slice(&psk_owned[..psk_len]);
        Ok(psk_len)
    });

    builder.set_cipher_list(BAREOS_CIPHERS).map_err(ssl_err)?;
    let acceptor = builder.build();
    let ssl_stream = acceptor.accept(stream)
        .map_err(|e| io::Error::new(io::ErrorKind::Other, format!("PSK accept: {e}")))?;
    Ok(PskStream { inner: ssl_stream })
}

// ============================================================================
// X.509 Certificate mode
// ============================================================================

/// Connect via TLS with X.509 certificates (client mode).
pub fn cert_connect(
    stream: TcpStream,
    cert_file: Option<&str>,
    key_file: Option<&str>,
    ca_file: Option<&str>,
    verify_peer: bool,
) -> io::Result<PskStream> {
    let mut builder = SslConnector::builder(SslMethod::tls()).map_err(ssl_err)?;

    if let (Some(cert), Some(key)) = (cert_file, key_file) {
        builder.set_certificate_chain_file(cert).map_err(ssl_err)?;
        builder.set_private_key_file(key, SslFiletype::PEM).map_err(ssl_err)?;
    }
    if let Some(ca) = ca_file {
        builder.set_ca_file(ca).map_err(ssl_err)?;
    }
    if !verify_peer {
        builder.set_verify(SslVerifyMode::NONE);
    }

    builder.set_cipher_list(BAREOS_CIPHERS).map_err(ssl_err)?;
    let connector = builder.build();
    let ssl_stream = connector.connect("bareos", stream)
        .map_err(|e| io::Error::new(io::ErrorKind::Other, format!("cert connect: {e}")))?;
    Ok(PskStream { inner: ssl_stream })
}

/// Accept via TLS with X.509 certificates (server mode).
pub fn cert_accept(
    stream: TcpStream,
    cert_file: &str,
    key_file: &str,
    ca_file: Option<&str>,
    verify_peer: bool,
) -> io::Result<PskStream> {
    let mut builder = SslAcceptor::mozilla_intermediate(SslMethod::tls()).map_err(ssl_err)?;

    builder.set_certificate_chain_file(cert_file).map_err(ssl_err)?;
    builder.set_private_key_file(key_file, SslFiletype::PEM).map_err(ssl_err)?;
    if let Some(ca) = ca_file {
        builder.set_ca_file(ca).map_err(ssl_err)?;
    }
    if verify_peer {
        builder.set_verify(SslVerifyMode::PEER | SslVerifyMode::FAIL_IF_NO_PEER_CERT);
    } else {
        builder.set_verify(SslVerifyMode::NONE);
    }

    builder.set_cipher_list(BAREOS_CIPHERS).map_err(ssl_err)?;
    let acceptor = builder.build();
    let ssl_stream = acceptor.accept(stream)
        .map_err(|e| io::Error::new(io::ErrorKind::Other, format!("cert accept: {e}")))?;
    Ok(PskStream { inner: ssl_stream })
}

// ============================================================================
// Combined mode: PSK + certificates in one acceptor
// ============================================================================

/// Accept TLS — supports both PSK and certificate clients.
pub fn accept_any_tls(
    stream: TcpStream,
    psk: &[u8],
    cert_file: Option<&str>,
    key_file: Option<&str>,
    ca_file: Option<&str>,
) -> io::Result<PskStream> {
    let mut builder = SslAcceptor::mozilla_intermediate(SslMethod::tls()).map_err(ssl_err)?;

    // PSK callback
    let psk_owned = psk.to_vec();
    builder.set_psk_server_callback(move |_ssl, _identity, psk_buf| {
        let psk_len = psk_owned.len().min(psk_buf.len());
        psk_buf[..psk_len].copy_from_slice(&psk_owned[..psk_len]);
        Ok(psk_len)
    });

    // Certificates (optional)
    if let (Some(cert), Some(key)) = (cert_file, key_file) {
        let _ = builder.set_certificate_chain_file(cert);
        let _ = builder.set_private_key_file(key, SslFiletype::PEM);
    }
    if let Some(ca) = ca_file {
        let _ = builder.set_ca_file(ca);
    }

    builder.set_verify(SslVerifyMode::NONE);
    builder.set_cipher_list(BAREOS_CIPHERS).map_err(ssl_err)?;

    let acceptor = builder.build();
    let ssl_stream = acceptor.accept(stream)
        .map_err(|e| io::Error::new(io::ErrorKind::Other, format!("TLS accept: {e}")))?;
    Ok(PskStream { inner: ssl_stream })
}

// ============================================================================
// Helpers
// ============================================================================

/// Derive PSK from a Bareos password (MD5 hash as hex bytes).
pub fn password_to_psk(password: &str) -> Vec<u8> {
    crate::cram_md5::md5_hash_password(password).into_bytes()
}

fn ssl_err(e: openssl::error::ErrorStack) -> io::Error {
    io::Error::new(io::ErrorKind::Other, format!("{e}"))
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::net::TcpListener;

    #[test]
    fn psk_client_server_roundtrip() {
        let psk = password_to_psk("testpassword");
        let listener = TcpListener::bind("127.0.0.1:0").unwrap();
        let port = listener.local_addr().unwrap().port();

        let psk_server = psk.clone();
        let server = std::thread::spawn(move || {
            let (stream, _) = listener.accept().unwrap();
            let mut s = psk_accept(stream, &psk_server).unwrap();
            let mut buf = [0u8; 100];
            let n = s.read(&mut buf).unwrap();
            assert_eq!(&buf[..n], b"hello psk");
            s.write_all(b"hello back").unwrap();
        });

        let stream = TcpStream::connect(format!("127.0.0.1:{port}")).unwrap();
        let mut c = psk_connect(stream, "test-dir", &psk).unwrap();
        c.write_all(b"hello psk").unwrap();
        c.flush().unwrap();
        let mut buf = [0u8; 100];
        let n = c.read(&mut buf).unwrap();
        assert_eq!(&buf[..n], b"hello back");
        server.join().unwrap();
    }

    #[test]
    fn cert_client_server_roundtrip() {
        // Generate self-signed cert for testing
        use std::process::Command;
        let dir = std::env::temp_dir().join("bareos-cert-test");
        std::fs::create_dir_all(&dir).ok();
        let cert = dir.join("cert.pem");
        let key = dir.join("key.pem");

        Command::new("openssl")
            .args(["req", "-x509", "-newkey", "rsa:2048",
                   "-keyout", key.to_str().unwrap(),
                   "-out", cert.to_str().unwrap(),
                   "-days", "1", "-nodes", "-subj", "/CN=test"])
            .output().ok();

        if !cert.exists() {
            // Skip if openssl not available
            return;
        }

        let listener = TcpListener::bind("127.0.0.1:0").unwrap();
        let port = listener.local_addr().unwrap().port();
        let cert_s = cert.to_str().unwrap().to_string();
        let key_s = key.to_str().unwrap().to_string();
        let cert_c = cert_s.clone();

        let server = std::thread::spawn(move || {
            let (stream, _) = listener.accept().unwrap();
            let mut s = cert_accept(stream, &cert_s, &key_s, None, false).unwrap();
            let mut buf = [0u8; 100];
            let n = s.read(&mut buf).unwrap();
            assert_eq!(&buf[..n], b"cert hello");
            s.write_all(b"cert reply").unwrap();
        });

        let stream = TcpStream::connect(format!("127.0.0.1:{port}")).unwrap();
        let mut c = cert_connect(stream, None, None, Some(&cert_c), false).unwrap();
        c.write_all(b"cert hello").unwrap();
        c.flush().unwrap();
        let mut buf = [0u8; 100];
        let n = c.read(&mut buf).unwrap();
        assert_eq!(&buf[..n], b"cert reply");
        server.join().unwrap();

        std::fs::remove_dir_all(&dir).ok();
    }

    #[test]
    fn password_to_psk_deterministic() {
        let psk1 = password_to_psk("test");
        let psk2 = password_to_psk("test");
        assert_eq!(psk1, psk2);
        assert_eq!(psk1.len(), 32);
    }
}
