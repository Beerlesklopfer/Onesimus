//! baReos Director — Rust rewrite of the Bareos Director daemon.
//!
//! Named threads, fork-per-job, crossbeam channels for core,
//! tokio only for gRPC.

mod accept;
mod command_ast;
mod command_executor;
mod command_queue;
mod commands;
mod connection_monitor;
mod fd_protocol;
mod fsm;
mod grpc;
mod job_worker;
mod message_bus;
mod scheduler;
mod sd_protocol;
mod session;
mod session_manager;
mod watchdog;

use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

use tracing::{info, error};
use tracing_subscriber::EnvFilter;

use crate::message_bus::MessageBus;
use crate::session_manager::SessionManager;

/// Global shutdown flag.
static SHUTDOWN: AtomicBool = AtomicBool::new(false);

fn main() {
    // Logging
    tracing_subscriber::fmt()
        .with_env_filter(
            EnvFilter::try_from_default_env()
                .unwrap_or_else(|_| EnvFilter::new("info")),
        )
        .init();

    info!("baReos Director starting");

    // TODO(R2): Load configuration from file
    // let config = bareos_config::parse_config(&config_path)?;

    // Shared state
    let bus = Arc::new(MessageBus::new(1024));
    let session_mgr = Arc::new(SessionManager::new(bus.clone()));

    // Named threads
    let threads = spawn_named_threads(session_mgr.clone(), bus.clone());

    // Wait for shutdown signal (SIGTERM / SIGINT)
    wait_for_shutdown();

    info!("baReos Director shutting down");
    SHUTDOWN.store(true, Ordering::SeqCst);

    // Join all threads
    for (name, handle) in threads {
        if let Err(e) = handle.join() {
            error!("Thread {name} panicked: {e:?}");
        }
    }

    info!("baReos Director stopped");
}

fn spawn_named_threads(
    session_mgr: Arc<SessionManager>,
    bus: Arc<MessageBus>,
) -> Vec<(String, thread::JoinHandle<()>)> {
    let mut threads = Vec::new();

    // dir:accept — TCP connection listener
    {
        let smgr = session_mgr.clone();
        let handle = thread::Builder::new()
            .name("dir:accept".into())
            .spawn(move || {
                info!("dir:accept thread started");
                accept::accept_loop(&smgr, &SHUTDOWN);
            })
            .expect("failed to spawn dir:accept");
        threads.push(("dir:accept".into(), handle));
    }

    // dir:scheduler — job scheduling
    {
        let handle = thread::Builder::new()
            .name("dir:scheduler".into())
            .spawn(move || {
                info!("dir:scheduler thread started");
                scheduler::scheduler_loop(&SHUTDOWN);
            })
            .expect("failed to spawn dir:scheduler");
        threads.push(("dir:scheduler".into(), handle));
    }

    // dir:watchdog — health monitoring
    {
        let handle = thread::Builder::new()
            .name("dir:watchdog".into())
            .spawn(move || {
                info!("dir:watchdog thread started");
                watchdog::watchdog_loop(&SHUTDOWN);
            })
            .expect("failed to spawn dir:watchdog");
        threads.push(("dir:watchdog".into(), handle));
    }

    // dir:grpc — tonic gRPC server (owns its own tokio runtime)
    {
        let smgr = session_mgr.clone();
        let b = bus.clone();
        let handle = thread::Builder::new()
            .name("dir:grpc".into())
            .spawn(move || {
                info!("dir:grpc thread started");
                grpc::run_grpc_server(&smgr, &b, &SHUTDOWN);
            })
            .expect("failed to spawn dir:grpc");
        threads.push(("dir:grpc".into(), handle));
    }

    // dir:monitor — FD/SD connection health checks
    {
        let smgr = session_mgr;
        let b = bus;
        let handle = thread::Builder::new()
            .name("dir:monitor".into())
            .spawn(move || {
                info!("dir:monitor thread started");
                connection_monitor::monitor_loop(&smgr, &b, &SHUTDOWN);
            })
            .expect("failed to spawn dir:monitor");
        threads.push(("dir:monitor".into(), handle));
    }

    threads
}

fn wait_for_shutdown() {
    use std::sync::mpsc;
    let (tx, rx) = mpsc::channel();

    ctrlc_handler(tx);
    let _ = rx.recv(); // Block until signal
}

fn ctrlc_handler(tx: std::sync::mpsc::Sender<()>) {
    let _ = ctrlc::set_handler(move || {
        let _ = tx.send(());
    });
}
