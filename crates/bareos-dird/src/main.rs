//! baReos Director — Rust rewrite of the Bareos Director daemon.
//!
//! Named threads, fork-per-job, crossbeam channels for core,
//! tokio only for gRPC.

mod accept;
mod command_ast;
mod command_executor;
mod command_queue;
mod commands;
mod command_worker;
mod connection_monitor;
mod director_config;
mod fd_protocol;
mod fsm;
mod grpc;
mod job_worker;
mod message_bus;
mod reload;
mod scheduler;
mod sd_protocol;
mod session;
mod session_manager;
mod watchdog;

use std::path::PathBuf;
use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

use clap::Parser;
use tracing::{info, error, warn};
use tracing_subscriber::EnvFilter;

use crate::command_queue::CommandQueue;
use crate::director_config::{DirectorConfig, SharedConfig};
use crate::message_bus::MessageBus;
use crate::session_manager::SessionManager;

/// Global shutdown flag.
static SHUTDOWN: AtomicBool = AtomicBool::new(false);

/// baReos Director — Backup Archiving REcovery Open Sourced
#[derive(Parser, Debug)]
#[command(name = "bareos-dird", version, about)]
struct Cli {
    /// Configuration file path
    #[arg(short, long, default_value = "/etc/bareos/bareos-dir.conf")]
    config: PathBuf,

    /// Foreground mode (don't daemonize)
    #[arg(short, long, default_value_t = true)]
    foreground: bool,

    /// Test config only, then exit
    #[arg(short, long)]
    test_config: bool,

    /// Debug level (0-999)
    #[arg(short, long, default_value_t = 0)]
    debug_level: u32,

    /// TLS certificate file (PEM)
    #[arg(long)]
    tls_cert: Option<String>,

    /// TLS private key file (PEM)
    #[arg(long)]
    tls_key: Option<String>,

    /// TLS CA certificate file (PEM)
    #[arg(long)]
    tls_ca: Option<String>,
}

fn main() {
    let cli = Cli::parse();

    // Logging
    let filter = if cli.debug_level > 0 {
        format!("debug,hyper=info,h2=info,tower=info")
    } else {
        "info,hyper=warn,h2=warn,tower=warn".to_string()
    };
    tracing_subscriber::fmt()
        .with_env_filter(
            EnvFilter::try_from_default_env()
                .unwrap_or_else(|_| EnvFilter::new(&filter)),
        )
        .init();

    // Install rustls CryptoProvider for TLS support
    let _ = rustls::crypto::ring::default_provider().install_default();

    info!("baReos Director Version {} starting", env!("CARGO_PKG_VERSION"));

    // Load configuration
    let config = if cli.config.exists() {
        match DirectorConfig::load(&cli.config) {
            Ok(c) => {
                info!("loaded configuration from {}", cli.config.display());
                c
            }
            Err(e) => {
                error!("failed to load config {}: {e}", cli.config.display());
                std::process::exit(1);
            }
        }
    } else {
        warn!("config file {} not found, using defaults", cli.config.display());
        DirectorConfig::from_str("Director { Name = bareos-dir\n Port = 9101\n Password = default\n }").unwrap()
    };

    if cli.test_config {
        info!("configuration OK: Director '{}', {} clients, {} jobs",
              config.director.name, config.clients.len(), config.jobs.len());
        std::process::exit(0);
    }

    let port = config.director.port;
    let grpc_enabled = config.director.enable_grpc;
    let dir_name = config.director.name.clone();
    let dir_password = config.director.password.clone();

    // TLS config from CLI args
    let tls_config = if cli.tls_cert.is_some() && cli.tls_key.is_some() {
        Some(bareos_bnet::tls::TlsConfig {
            certfile: cli.tls_cert.clone(),
            keyfile: cli.tls_key.clone(),
            ca_certfile: cli.tls_ca.clone(),
            verify_peer: false,
        })
    } else {
        None
    };

    // Shared state
    let shared_config: SharedConfig = Arc::new(std::sync::RwLock::new(config));
    let bus = Arc::new(MessageBus::new(1024));
    let session_mgr = Arc::new(SessionManager::new(bus.clone()));
    let cmd_queue = Arc::new(CommandQueue::new(256));

    info!("Director '{}' listening on port {}", dir_name, port);

    // Named threads
    let threads = spawn_named_threads(
        session_mgr.clone(), bus.clone(), cmd_queue.clone(),
        shared_config.clone(), port, grpc_enabled, dir_password, tls_config,
    );

    // SIGHUP handler for config reload
    reload::setup_sighup_handler(cli.config.clone(), shared_config.clone());

    // Wait for shutdown signal (SIGTERM / SIGINT)
    wait_for_shutdown();

    info!("baReos Director shutting down");
    SHUTDOWN.store(true, Ordering::SeqCst);
    cmd_queue.shutdown();

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
    cmd_queue: Arc<CommandQueue>,
    config: SharedConfig,
    port: u16,
    grpc_enabled: bool,
    password: String,
    tls_config: Option<bareos_bnet::tls::TlsConfig>,
) -> Vec<(String, thread::JoinHandle<()>)> {
    let mut threads = Vec::new();

    // dir:accept — TCP connection listener
    {
        let smgr = session_mgr.clone();
        let q = cmd_queue.clone();
        let pw = password.clone();
        let tls_cfg = tls_config.clone();
        let shared_cfg = config.clone();
        let handle = thread::Builder::new()
            .name("dir:accept".into())
            .spawn(move || {
                let config = accept::AcceptConfig {
                    bind_address: "0.0.0.0".to_string(),
                    port,
                    password: pw,
                    tls_config: tls_cfg,
                    shared_config: Some(shared_cfg),
                };
                accept::accept_loop(&smgr, &q, &config, &SHUTDOWN);
            })
            .expect("failed to spawn dir:accept");
        threads.push(("dir:accept".into(), handle));
    }

    // dir:scheduler — job scheduling
    {
        let sched_config = config.clone();
        let handle = thread::Builder::new()
            .name("dir:scheduler".into())
            .spawn(move || {
                scheduler::scheduler_loop(Some(sched_config), &SHUTDOWN);
            })
            .expect("failed to spawn dir:scheduler");
        threads.push(("dir:scheduler".into(), handle));
    }

    // dir:watchdog — health monitoring
    {
        let handle = thread::Builder::new()
            .name("dir:watchdog".into())
            .spawn(move || {
                watchdog::watchdog_loop(&SHUTDOWN);
            })
            .expect("failed to spawn dir:watchdog");
        threads.push(("dir:watchdog".into(), handle));
    }

    // dir:grpc — tonic gRPC server (optional)
    if grpc_enabled {
        let smgr = session_mgr.clone();
        let b = bus.clone();
        let q = cmd_queue.clone();
        let grpc_port = port; // TODO: separate gRPC port
        let handle = thread::Builder::new()
            .name("dir:grpc".into())
            .spawn(move || {
                grpc::run_grpc_server(&smgr, &b, &q, grpc_port + 1000, &SHUTDOWN);
            })
            .expect("failed to spawn dir:grpc");
        threads.push(("dir:grpc".into(), handle));
    }

    // dir:cmdworker — processes commands from gRPC and other queue submitters
    {
        let q = cmd_queue.clone();
        let cfg = config.clone();
        let smgr = session_mgr.clone();
        let handle = thread::Builder::new()
            .name("dir:cmdworker".into())
            .spawn(move || {
                command_worker::command_worker_loop(&q, Some(cfg), Some(smgr), &SHUTDOWN);
            })
            .expect("failed to spawn dir:cmdworker");
        threads.push(("dir:cmdworker".into(), handle));
    }

    // dir:monitor — FD/SD connection health checks
    {
        let smgr = session_mgr;
        let b = bus;
        let handle = thread::Builder::new()
            .name("dir:monitor".into())
            .spawn(move || {
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

    let _ = ctrlc::set_handler(move || {
        let _ = tx.send(());
    });
    let _ = rx.recv();
}
