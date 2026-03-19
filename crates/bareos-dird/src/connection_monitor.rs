//! Connection health monitor — dir:monitor thread.

use std::sync::atomic::{AtomicBool, Ordering};

use tracing::info;

use crate::message_bus::MessageBus;
use crate::session_manager::SessionManager;

pub fn monitor_loop(
    _session_mgr: &SessionManager,
    _bus: &MessageBus,
    shutdown: &AtomicBool,
) {
    // TODO(R3): Periodic FD/SD health checks, publish status to bus
    while !shutdown.load(Ordering::Relaxed) {
        std::thread::sleep(std::time::Duration::from_secs(1));
    }
    info!("dir:monitor thread stopping");
}
