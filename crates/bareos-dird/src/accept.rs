//! TCP accept loop — dir:accept thread.

use std::sync::atomic::{AtomicBool, Ordering};

use tracing::info;

use crate::session_manager::SessionManager;

pub fn accept_loop(_session_mgr: &SessionManager, shutdown: &AtomicBool) {
    // TODO(R8): Bind TCP listener, accept connections, TLS, BNET hello
    while !shutdown.load(Ordering::Relaxed) {
        std::thread::sleep(std::time::Duration::from_secs(1));
    }
    info!("dir:accept thread stopping");
}
