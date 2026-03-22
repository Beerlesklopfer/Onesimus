//! Command worker thread — processes commands from the CommandQueue.
//!
//! Takes commands submitted by gRPC handlers or other producers,
//! dispatches them through the command executor, and sends results back.

use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};

use tracing::{info, debug, warn};

use crate::command_executor::{self, ExecContext};
use crate::command_queue::CommandQueue;
use crate::director_config::SharedConfig;
use crate::session_manager::SessionManager;

/// Command worker loop. Runs in the dir:cmdworker named thread.
pub fn command_worker_loop(
    cmd_queue: &Arc<CommandQueue>,
    config: Option<SharedConfig>,
    session_mgr: Option<Arc<SessionManager>>,
    shutdown: &AtomicBool,
) {
    info!("command worker started");

    while !shutdown.load(Ordering::Relaxed) {
        if let Some(cmd) = cmd_queue.take() {
            let ctx = ExecContext {
                session_id: cmd.session_id,
                config: config.clone(),
                session_mgr: session_mgr.clone(),
                catalog_url: None,
                api_mode: command_executor::ApiMode::Api,
                bus_rx: None,
                job_manager: None,
            };

            debug!("executing command '{}' from queue", cmd.ast.command_name);
            let result = command_executor::dispatch(&cmd.ast, &ctx);

            // Send result back to the submitter
            if cmd.result_tx.send(result).is_err() {
                warn!("command result receiver dropped");
            }
        }
    }

    info!("command worker stopped");
}
