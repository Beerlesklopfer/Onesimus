//! Bareos Storage Daemon protocol library.
//!
//! Provides the Director→SD communication protocol:
//! - Connection (TLS-PSK + CRAM-MD5)
//! - Job setup (JobId, use device, bootstrap)
//! - CatReq relay loop (SD↔Director catalog requests)
//! - Status queries
//!
//! This is a protocol library, not a daemon binary.
//! The daemon will be `onesimus-stored` (gRPC-based).

pub mod connect;
pub mod job;
pub mod catrec;
pub mod status;
