//! gRPC server — dir:grpc thread.
//!
//! Runs a tonic gRPC server in its own tokio runtime.

use std::sync::atomic::{AtomicBool, Ordering};

use tracing::info;

use crate::message_bus::MessageBus;
use crate::session_manager::SessionManager;

pub fn run_grpc_server(
    _session_mgr: &SessionManager,
    _bus: &MessageBus,
    shutdown: &AtomicBool,
) {
    // TODO(R4): Create tokio runtime, start tonic server, serve DirectorService
    while !shutdown.load(Ordering::Relaxed) {
        std::thread::sleep(std::time::Duration::from_secs(1));
    }
    info!("dir:grpc thread stopping");
}
