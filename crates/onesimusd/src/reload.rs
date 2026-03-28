//! Configuration hot-reload via SIGHUP.
//!
//! When the Director receives SIGHUP, it reloads the configuration file
//! and swaps the SharedConfig atomically via RwLock.

use std::path::PathBuf;
use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};

use tracing::{info, error, warn};

use crate::director_config::{DirectorConfig, SharedConfig};

/// Set up SIGHUP handler that triggers config reload.
pub fn setup_sighup_handler(
    config_path: PathBuf,
    shared_config: SharedConfig,
) {
    std::thread::Builder::new()
        .name("dir:sighup".into())
        .spawn(move || {
            sighup_loop(config_path, shared_config);
        })
        .expect("failed to spawn SIGHUP handler");
}

#[cfg(unix)]
fn sighup_loop(config_path: PathBuf, shared_config: SharedConfig) {
    use nix::sys::signal::{self, SigSet, Signal};

    let mut sigset = SigSet::empty();
    sigset.add(Signal::SIGHUP);

    // Block SIGHUP in this thread so we can wait for it
    sigset.thread_block().ok();

    loop {
        // Wait for SIGHUP
        match sigset.wait() {
            Ok(Signal::SIGHUP) => {
                info!("SIGHUP received, reloading configuration from {}",
                      config_path.display());

                match DirectorConfig::load(&config_path) {
                    Ok(new_config) => {
                        match shared_config.write() {
                            Ok(mut cfg) => {
                                let old_name = cfg.director.name.clone();
                                *cfg = new_config;
                                info!("configuration reloaded: Director '{}', {} clients, {} jobs",
                                      cfg.director.name, cfg.clients.len(), cfg.jobs.len());
                            }
                            Err(e) => {
                                error!("failed to acquire config write lock: {e}");
                            }
                        }
                    }
                    Err(e) => {
                        error!("configuration reload failed: {e}");
                        warn!("keeping previous configuration");
                    }
                }
            }
            Ok(_) => {} // Other signal, ignore
            Err(e) => {
                error!("sigwait error: {e}");
                break;
            }
        }
    }
}

#[cfg(not(unix))]
fn sighup_loop(_config_path: PathBuf, _shared_config: SharedConfig) {
    // No SIGHUP on non-Unix platforms
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::sync::RwLock;

    #[test]
    fn config_swap_via_rwlock() {
        let config = DirectorConfig::from_str(
            "Director { Name = old-dir\n Port = 9101\n Password = pass\n }"
        ).unwrap();
        let shared: SharedConfig = Arc::new(RwLock::new(config));

        // Verify initial
        assert_eq!(shared.read().unwrap().director.name, "old-dir");

        // Swap
        let new_config = DirectorConfig::from_str(
            "Director { Name = new-dir\n Port = 9101\n Password = pass\n }"
        ).unwrap();
        *shared.write().unwrap() = new_config;

        // Verify swapped
        assert_eq!(shared.read().unwrap().director.name, "new-dir");
    }

    #[test]
    fn concurrent_reads_during_write() {
        let config = DirectorConfig::from_str(
            "Director { Name = test-dir\n Port = 9101\n Password = p\n }"
        ).unwrap();
        let shared: SharedConfig = Arc::new(RwLock::new(config));

        let handles: Vec<_> = (0..10).map(|_| {
            let c = shared.clone();
            std::thread::spawn(move || {
                for _ in 0..100 {
                    let cfg = c.read().unwrap();
                    assert!(!cfg.director.name.is_empty());
                }
            })
        }).collect();

        // Write while readers are active
        let new = DirectorConfig::from_str(
            "Director { Name = updated\n Port = 9101\n Password = p\n }"
        ).unwrap();
        *shared.write().unwrap() = new;

        for h in handles {
            h.join().unwrap();
        }
    }
}
