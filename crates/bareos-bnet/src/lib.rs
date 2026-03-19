//! BNET wire protocol, TLS, and CRAM-MD5 authentication for Bareos.
//!
//! This crate implements the custom Bareos network protocol used for
//! communication between Director, File Daemon, and Storage Daemon.

pub mod wire;
pub mod signal;
pub mod tls;
pub mod cram_md5;

// Re-exports
pub use signal::BnetSignal;
pub use wire::{BnetMessage, BnetStream};
