//! CRAM-MD5 authentication for Bareos daemon connections.
//!
//! Implements the two-way CRAM-MD5 handshake used between Director,
//! File Daemon, and Storage Daemon.
//! Implementation will be filled in Phase R1c.

/// Result of a CRAM-MD5 two-way authentication.
#[derive(Debug)]
pub struct AuthResult {
    /// Whether authentication succeeded.
    pub success: bool,
    /// The server's challenge string (used for session token derivation).
    pub server_challenge: String,
}
