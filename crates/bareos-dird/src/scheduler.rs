//! Job scheduler — dir:scheduler thread.

use std::sync::atomic::{AtomicBool, Ordering};

use tracing::info;

pub fn scheduler_loop(shutdown: &AtomicBool) {
    // TODO(R6): Scan config resources, fill priority queue, dispatch jobs
    while !shutdown.load(Ordering::Relaxed) {
        std::thread::sleep(std::time::Duration::from_secs(1));
    }
    info!("dir:scheduler thread stopping");
}
