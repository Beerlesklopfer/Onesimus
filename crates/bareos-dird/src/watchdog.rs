//! Watchdog — dir:watchdog thread.

use std::sync::atomic::{AtomicBool, Ordering};

use tracing::info;

pub fn watchdog_loop(shutdown: &AtomicBool) {
    // TODO(R6): Monitor scheduler heartbeat, restart on timeout
    while !shutdown.load(Ordering::Relaxed) {
        std::thread::sleep(std::time::Duration::from_secs(1));
    }
    info!("dir:watchdog thread stopping");
}
