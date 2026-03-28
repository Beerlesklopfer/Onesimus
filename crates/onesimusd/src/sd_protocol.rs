//! Storage Daemon protocol handler — thin wrapper around bareos-stored crate.

pub use bareos_stored::status::{query_sd_status, SdStatusResult};
