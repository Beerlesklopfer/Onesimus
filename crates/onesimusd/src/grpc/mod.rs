//! gRPC server — dir:grpc thread.
//!
//! Runs a shared tonic gRPC server in its own tokio runtime.
//! Unlike the C++ per-client model, this uses a single server with
//! session multiplexing (fixes the >16 connection scaling issue).

pub mod service;
pub mod auth;
pub mod push_events;

use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};

use tonic::transport::Server;
use tracing::{info, error};

use crate::message_bus::MessageBus;
use crate::session_manager::SessionManager;
use crate::command_queue::CommandQueue;

use service::DirectorServiceImpl;
use bareos_proto_rs::bareos::director::director_server::DirectorServer;

/// Run the gRPC server. Called from the dir:grpc named thread.
/// Creates its own tokio runtime.
pub fn run_grpc_server(
    session_mgr: &Arc<SessionManager>,
    bus: &Arc<MessageBus>,
    cmd_queue: &Arc<CommandQueue>,
    port: u16,
    shutdown: &AtomicBool,
) {
    let rt = tokio::runtime::Builder::new_multi_thread()
        .worker_threads(2)
        .thread_name("grpc-worker")
        .enable_all()
        .build();

    let rt = match rt {
        Ok(rt) => rt,
        Err(e) => {
            error!("failed to create tokio runtime for gRPC: {e}");
            return;
        }
    };

    let smgr = session_mgr.clone();
    let b = bus.clone();
    let q = cmd_queue.clone();
    let shutdown_flag = shutdown as *const AtomicBool;

    rt.block_on(async move {
        let addr = format!("0.0.0.0:{port}").parse().unwrap();
        let svc = DirectorServiceImpl::new(smgr, b, q);

        info!("gRPC server listening on {addr}");

        // Use a channel to signal shutdown
        let (shutdown_tx, shutdown_rx) = tokio::sync::oneshot::channel::<()>();

        // Spawn a task to poll the shutdown flag
        let shutdown_flag = unsafe { &*shutdown_flag };
        tokio::spawn(async move {
            loop {
                if shutdown_flag.load(Ordering::Relaxed) {
                    let _ = shutdown_tx.send(());
                    break;
                }
                tokio::time::sleep(tokio::time::Duration::from_millis(500)).await;
            }
        });

        let result = Server::builder()
            .add_service(DirectorServer::new(svc))
            .serve_with_shutdown(addr, async { let _ = shutdown_rx.await; });

        if let Err(e) = result.await {
            error!("gRPC server error: {e}");
        }

        info!("gRPC server stopped");
    });
}
