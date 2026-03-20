//! TLS-PSK support for Bareos daemon-to-daemon connections.
//!
//! Bareos uses TLS with Pre-Shared Keys (PSK) instead of certificates.
//! The PSK identity is the daemon name, the PSK is derived from the
//! configured password (MD5-hashed).
//!
//! Uses the `openssl` crate since rustls does not support TLS-PSK.

use std::io::{self, Read, Write};
use std::net::TcpStream;

use openssl::ssl::{SslMethod, SslConnector, SslAcceptor, SslStream, SslFiletype};

/// TLS-PSK connection to a Bareos daemon.
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

/// Connect to a Bareos daemon using TLS-PSK (client mode).
///
/// - `identity`: The daemon name (e.g., "bareos-dir")
/// - `psk`: The pre-shared key (MD5-hashed password as bytes)
pub fn psk_connect(
    stream: TcpStream,
    identity: &str,
    psk: &[u8],
) -> io::Result<PskStream> {
    let mut builder = SslConnector::builder(SslMethod::tls())
        .map_err(|e| io::Error::new(io::ErrorKind::Other, format!("SSL builder: {e}")))?;

    // Set PSK callback
    let identity_owned = identity.to_string();
    let psk_owned = psk.to_vec();
    builder.set_psk_client_callback(move |_ssl, _hint, identity_buf, psk_buf| {
        // Write identity
        let id_bytes = identity_owned.as_bytes();
        let id_len = id_bytes.len().min(identity_buf.len() - 1);
        identity_buf[..id_len].copy_from_slice(&id_bytes[..id_len]);
        identity_buf[id_len] = 0; // null terminate

        // Write PSK
        let psk_len = psk_owned.len().min(psk_buf.len());
        psk_buf[..psk_len].copy_from_slice(&psk_owned[..psk_len]);

        Ok(psk_len)
    });

    // Match Bareos default cipher list (includes PSK ciphers)
    builder.set_cipher_list("ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")
        .map_err(|e| io::Error::new(io::ErrorKind::Other, format!("cipher list: {e}")))?;

    let connector = builder.build();

    // Connect (no hostname verification for PSK)
    let ssl_stream = connector.connect("bareos", stream)
        .map_err(|e| io::Error::new(io::ErrorKind::Other, format!("PSK connect: {e}")))?;

    Ok(PskStream { inner: ssl_stream })
}

/// Accept a TLS-PSK connection (server mode).
pub fn psk_accept(
    stream: TcpStream,
    psk: &[u8],
) -> io::Result<PskStream> {
    let mut builder = SslAcceptor::mozilla_intermediate(SslMethod::tls())
        .map_err(|e| io::Error::new(io::ErrorKind::Other, format!("SSL acceptor: {e}")))?;

    let psk_owned = psk.to_vec();
    builder.set_psk_server_callback(move |_ssl, _identity, psk_buf| {
        let psk_len = psk_owned.len().min(psk_buf.len());
        psk_buf[..psk_len].copy_from_slice(&psk_owned[..psk_len]);
        Ok(psk_len)
    });

    builder.set_cipher_list("PSK-AES128-CBC-SHA:PSK-AES256-CBC-SHA:PSK-AES128-GCM-SHA256:PSK-AES256-GCM-SHA384:PSK-CHACHA20-POLY1305")
        .map_err(|e| io::Error::new(io::ErrorKind::Other, format!("cipher list: {e}")))?;

    let acceptor = builder.build();

    let ssl_stream = acceptor.accept(stream)
        .map_err(|e| io::Error::new(io::ErrorKind::Other, format!("PSK accept: {e}")))?;

    Ok(PskStream { inner: ssl_stream })
}

/// Derive the PSK from a Bareos password.
/// The PSK is the raw bytes of the MD5-hashed password hex string.
pub fn password_to_psk(password: &str) -> Vec<u8> {
    crate::cram_md5::md5_hash_password(password).into_bytes()
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
            let mut psk_stream = psk_accept(stream, &psk_server).unwrap();
            let mut buf = [0u8; 100];
            let n = psk_stream.read(&mut buf).unwrap();
            assert_eq!(&buf[..n], b"hello psk");
            psk_stream.write_all(b"hello back").unwrap();
        });

        let stream = TcpStream::connect(format!("127.0.0.1:{port}")).unwrap();
        let mut psk_stream = psk_connect(stream, "test-dir", &psk).unwrap();
        psk_stream.write_all(b"hello psk").unwrap();
        psk_stream.flush().unwrap();

        let mut buf = [0u8; 100];
        let n = psk_stream.read(&mut buf).unwrap();
        assert_eq!(&buf[..n], b"hello back");

        server.join().unwrap();
    }

    #[test]
    fn password_to_psk_deterministic() {
        let psk1 = password_to_psk("test");
        let psk2 = password_to_psk("test");
        assert_eq!(psk1, psk2);
        assert_eq!(psk1.len(), 32); // MD5 hex = 32 chars
    }
}
