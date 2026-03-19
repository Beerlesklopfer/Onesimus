//! BNET wire protocol: length-prefixed framing over TCP.
//!
//! The Bareos network protocol uses a simple framing:
//!
//! ```text
//! [4 bytes: i32 big-endian length] [length bytes: payload]
//! ```
//!
//! - Positive length: data message (payload follows)
//! - Negative length: out-of-band signal (no payload)
//! - Zero length: keepalive / empty message

use std::io::{self, Read, Write};
use std::net::TcpStream;

use crate::signal::BnetSignal;

/// A message on the BNET wire: either data or a signal.
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum BnetMessage {
    /// Data payload
    Data(Vec<u8>),
    /// Out-of-band signal
    Signal(BnetSignal),
}

/// A BNET connection over a TCP stream (plaintext or TLS).
pub struct BnetStream {
    stream: TcpStream,
}

impl BnetStream {
    /// Wrap an existing TCP stream.
    pub fn new(stream: TcpStream) -> Self {
        Self { stream }
    }

    /// Send a data message.
    pub fn send(&mut self, data: &[u8]) -> io::Result<()> {
        let len = data.len() as i32;
        self.stream.write_all(&len.to_be_bytes())?;
        if !data.is_empty() {
            self.stream.write_all(data)?;
        }
        self.stream.flush()
    }

    /// Send a signal (negative length, no payload).
    pub fn send_signal(&mut self, signal: BnetSignal) -> io::Result<()> {
        let len = signal as i32;
        self.stream.write_all(&len.to_be_bytes())?;
        self.stream.flush()
    }

    /// Receive the next message.
    pub fn recv(&mut self) -> io::Result<BnetMessage> {
        let mut len_buf = [0u8; 4];
        self.stream.read_exact(&mut len_buf)?;
        let len = i32::from_be_bytes(len_buf);

        if len < 0 {
            match BnetSignal::from_i32(len) {
                Some(sig) => Ok(BnetMessage::Signal(sig)),
                None => Err(io::Error::new(
                    io::ErrorKind::InvalidData,
                    format!("unknown BNET signal: {len}"),
                )),
            }
        } else if len == 0 {
            Ok(BnetMessage::Data(Vec::new()))
        } else {
            let mut buf = vec![0u8; len as usize];
            self.stream.read_exact(&mut buf)?;
            Ok(BnetMessage::Data(buf))
        }
    }

    /// Send a UTF-8 string as data message.
    pub fn send_str(&mut self, s: &str) -> io::Result<()> {
        self.send(s.as_bytes())
    }

    /// Receive a data message and decode as UTF-8 string.
    /// Returns an error if the message is a signal.
    pub fn recv_str(&mut self) -> io::Result<String> {
        match self.recv()? {
            BnetMessage::Data(data) => String::from_utf8(data).map_err(|e| {
                io::Error::new(io::ErrorKind::InvalidData, e)
            }),
            BnetMessage::Signal(sig) => Err(io::Error::new(
                io::ErrorKind::UnexpectedEof,
                format!("expected data, got signal: {sig:?}"),
            )),
        }
    }

    /// Access the underlying TCP stream.
    pub fn inner(&self) -> &TcpStream {
        &self.stream
    }

    /// Consume and return the underlying TCP stream.
    pub fn into_inner(self) -> TcpStream {
        self.stream
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::net::TcpListener;

    fn make_pair() -> (BnetStream, BnetStream) {
        let listener = TcpListener::bind("127.0.0.1:0").unwrap();
        let addr = listener.local_addr().unwrap();
        let client = TcpStream::connect(addr).unwrap();
        let (server, _) = listener.accept().unwrap();
        (BnetStream::new(client), BnetStream::new(server))
    }

    #[test]
    fn data_roundtrip() {
        let (mut a, mut b) = make_pair();
        a.send(b"hello bareos").unwrap();
        let msg = b.recv().unwrap();
        assert_eq!(msg, BnetMessage::Data(b"hello bareos".to_vec()));
    }

    #[test]
    fn signal_roundtrip() {
        let (mut a, mut b) = make_pair();
        a.send_signal(BnetSignal::Eod).unwrap();
        let msg = b.recv().unwrap();
        assert_eq!(msg, BnetMessage::Signal(BnetSignal::Eod));
    }

    #[test]
    fn string_roundtrip() {
        let (mut a, mut b) = make_pair();
        a.send_str("status dir").unwrap();
        let s = b.recv_str().unwrap();
        assert_eq!(s, "status dir");
    }

    #[test]
    fn empty_message() {
        let (mut a, mut b) = make_pair();
        a.send(b"").unwrap();
        let msg = b.recv().unwrap();
        assert_eq!(msg, BnetMessage::Data(Vec::new()));
    }

    #[test]
    fn multiple_messages() {
        let (mut a, mut b) = make_pair();
        a.send_str("msg1").unwrap();
        a.send_str("msg2").unwrap();
        a.send_signal(BnetSignal::Eod).unwrap();

        assert_eq!(b.recv_str().unwrap(), "msg1");
        assert_eq!(b.recv_str().unwrap(), "msg2");
        assert_eq!(b.recv().unwrap(), BnetMessage::Signal(BnetSignal::Eod));
    }

    #[test]
    fn terminate_signal() {
        let (mut a, mut b) = make_pair();
        a.send_signal(BnetSignal::Terminate).unwrap();
        let msg = b.recv().unwrap();
        assert_eq!(msg, BnetMessage::Signal(BnetSignal::Terminate));
    }
}
