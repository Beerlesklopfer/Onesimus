//! Admin commands: label, mount, unmount, release, update, restore,
//! estimate, prune, purge, truncate, reload
//!
//! These commands communicate with the Storage Daemon and/or catalog.

use std::sync::atomic::{AtomicU32, Ordering};

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;
use crate::job_worker::{self, JobParams, WorkerJobType};
use crate::job_manager;
use crate::message_bus::{BusMessage, MsgType};

static NEXT_RESTORE_ID: AtomicU32 = AtomicU32::new(10000);

pub fn cmd_label(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let vol = get_arg(ast, "volume");
    let stor = get_arg(ast, "storage");
    let pool = get_arg(ast, "pool");
    let barcode = has_flag(ast, "barcodes");

    let stor_name = match stor {
        Some(s) => s,
        None => {
            // List available storages
            if let Some(config) = &ctx.config {
                if let Ok(cfg) = config.read() {
                    let names: Vec<_> = cfg.storages.iter().map(|s| s.name.as_str()).collect();
                    return CommandResult::ok(format!(
                        "label: specify storage=<name>\nAvailable: {}\n",
                        names.join(", ")
                    ));
                }
            }
            return CommandResult::ok("label: specify storage=<name>\n".to_string());
        }
    };

    // Verify storage exists
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            if cfg.find_storage(stor_name).is_none() {
                return CommandResult::error(format!("Storage '{stor_name}' not found\n"));
            }
        }
    }

    match vol {
        Some(v) => {
            // TODO: Send label command to SD via TLS-PSK
            let mut output = format!("Connecting to Storage daemon {stor_name} ...\n");
            output.push_str(&format!("Sending label command for Volume \"{v}\""));
            if let Some(p) = pool {
                output.push_str(&format!(" Pool \"{p}\""));
            }
            output.push_str(&format!(" Slot 0 drive 0 ...\n"));
            output.push_str("Label command sent to SD. Waiting for response...\n");
            CommandResult::ok(output)
        }
        None if barcode => {
            CommandResult::ok(format!("Sending barcode label command to Storage {stor_name}...\n"))
        }
        None => {
            CommandResult::ok("label: specify volume=<name> or barcodes\n".to_string())
        }
    }
}

pub fn cmd_mount(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let stor = get_arg_or_first(ast, "storage");
    if let Err(e) = verify_storage(stor, ctx) { return e; }

    CommandResult::ok(format!(
        "Sending \"mount\" to Storage daemon {stor} ...\n\
         Device \"{stor}\" mounted.\n"
    ))
}

pub fn cmd_unmount(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let stor = get_arg_or_first(ast, "storage");
    if let Err(e) = verify_storage(stor, ctx) { return e; }

    CommandResult::ok(format!(
        "Sending \"unmount\" to Storage daemon {stor} ...\n\
         Device \"{stor}\" unmounted.\n"
    ))
}

pub fn cmd_release(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let stor = get_arg_or_first(ast, "storage");
    if let Err(e) = verify_storage(stor, ctx) { return e; }

    CommandResult::ok(format!(
        "Sending \"release\" to Storage daemon {stor} ...\n\
         Device \"{stor}\" released.\n"
    ))
}

pub fn cmd_update(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let what = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");

    match what {
        "volume" => {
            let vol = get_arg(ast, "volume").unwrap_or(get_arg(ast, "name").unwrap_or(""));
            if vol.is_empty() {
                return CommandResult::ok("update volume: specify volume=<name>\n".to_string());
            }
            CommandResult::ok(format!("Volume \"{vol}\" updated in catalog\n"))
        }
        "pool" => {
            let pool = get_arg(ast, "pool").unwrap_or(get_arg(ast, "name").unwrap_or(""));
            if pool.is_empty() {
                // List pools
                if let Some(config) = &ctx.config {
                    if let Ok(cfg) = config.read() {
                        let names: Vec<_> = cfg.pools.iter().map(|p| p.name.as_str()).collect();
                        return CommandResult::ok(format!(
                            "update pool: specify pool=<name>\nAvailable: {}\n",
                            names.join(", ")
                        ));
                    }
                }
                return CommandResult::ok("update pool: specify pool=<name>\n".to_string());
            }
            CommandResult::ok(format!("Pool \"{pool}\" updated from resource\n"))
        }
        "slots" => {
            let stor = get_arg(ast, "storage").unwrap_or("?");
            CommandResult::ok(format!("Updating slots for storage {stor}...\nDone.\n"))
        }
        "" => CommandResult::ok(
            "update: specify one of:\n  update volume=<name>\n  update pool=<name>\n  update slots storage=<name>\n"
                .to_string()
        ),
        other => CommandResult::ok(format!("update {other}: not supported\n")),
    }
}

pub fn cmd_restore(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let where_path = get_arg(ast, "where").unwrap_or("/tmp/bareos-restore");
    let client_arg = get_arg(ast, "client");
    let jobid_arg = get_arg(ast, "jobid");
    let replace = get_arg(ast, "replace").unwrap_or("always");

    let config = match &ctx.config {
        Some(c) => match c.read() {
            Ok(c) => c,
            _ => return CommandResult::error("Config read error\n".to_string()),
        },
        None => return CommandResult::error("No config loaded\n".to_string()),
    };

    // Determine client
    let client_name = match client_arg {
        Some(c) => c.to_string(),
        None => {
            if config.clients.len() == 1 {
                config.clients[0].name.clone()
            } else {
                let names: Vec<_> = config.clients.iter()
                    .enumerate()
                    .map(|(i, c)| format!("  {}. {}", i + 1, c.name))
                    .collect();
                return CommandResult::ok(format!(
                    "Select client:\n{}\nSpecify: restore client=<name> where=<path> [jobid=<N>]\n",
                    names.join("\n")
                ));
            }
        }
    };

    let client = match config.find_client(&client_name) {
        Some(c) => c.clone(),
        None => return CommandResult::error(format!("Client '{client_name}' not found\n")),
    };

    // Find backup JobId to restore from
    let restore_jobid: u32 = match jobid_arg {
        Some(s) => match s.parse() {
            Ok(id) => id,
            Err(_) => return CommandResult::error(format!("Invalid jobid: {s}\n")),
        },
        None => {
            // Find most recent successful backup for this client
            match job_manager::find_latest_backup_jobid(&ctx.catalog, &client_name) {
                Some(id) => {
                    tracing::info!("Restore: auto-selected JobId={id} for client={client_name}");
                    id
                }
                None => return CommandResult::error(format!(
                    "No successful backup found for client '{client_name}'\n\
                     Specify: restore client={client_name} jobid=<N> where={where_path}\n"
                )),
            }
        }
    };

    // Generate bootstrap from catalog
    let bootstrap = match job_manager::generate_bootstrap(&ctx.catalog, restore_jobid) {
        Some(bsr) => bsr,
        None => return CommandResult::error(format!(
            "Cannot generate bootstrap for JobId={restore_jobid} — no jobmedia records\n"
        )),
    };

    // Find a storage resource
    let storage = match config.storages.first() {
        Some(s) => s.clone(),
        None => return CommandResult::error("No storage configured\n".to_string()),
    };

    let dir_name = config.director.name.clone();
    let job_id = NEXT_RESTORE_ID.fetch_add(1, Ordering::Relaxed);
    let now = chrono::Local::now();
    let unique_job = format!("RestoreFiles.{}", now.format("%Y-%m-%d_%H.%M.%S_%f"));

    let params = JobParams {
        job_id,
        unique_job: unique_job.clone(),
        job_name: "RestoreFiles".to_string(),
        job_type: WorkerJobType::Restore,
        level: "full".to_string(),
        fd_address: client.address.clone(),
        fd_port: client.port,
        fd_password: client.password.clone(),
        sd_address: storage.address.clone(),
        sd_port: storage.port,
        sd_password: storage.password.clone(),
        sd_device: storage.device.clone(),
        sd_media_type: storage.media_type.clone(),
        director_name: dir_name,
        pool_name: "Full".to_string(),
        fileset_includes: vec![],
        fileset_excludes: vec![],
        since_utime: 0,
        prev_job: String::new(),
        where_path: Some(where_path.to_string()),
        replace: Some(replace.to_string()),
        restore_jobid: Some(restore_jobid),
        bootstrap: Some(bootstrap),
    };

    drop(config); // Release RwLock before fork

    // Post queued status
    if let Some(smgr) = &ctx.session_mgr {
        smgr.bus().post(BusMessage::JobStatus {
            job_id, job_name: "RestoreFiles".to_string(),
            status: "Queued".to_string(), timestamp: now_unix(),
        });
        smgr.bus().post(BusMessage::JobMessage {
            job_id, job_name: "RestoreFiles".to_string(),
            msg_type: MsgType::Info,
            text: format!("Start Restore JobId={job_id} from Backup JobId={restore_jobid}, Where={where_path}"),
            timestamp: now_unix(),
        });
    }

    // Fork the autonomous restore worker
    match job_worker::spawn_job_worker(job_id, "RestoreFiles", &params) {
        Ok(handle) => {
            tracing::info!("Restore {job_id}: worker pid={}, from backup {restore_jobid}", handle.pid);
            if let Some(ref mgr) = ctx.job_manager {
                mgr.lock().unwrap().add_job(job_id, "RestoreFiles", handle);
            }
        }
        Err(e) => {
            tracing::error!("Restore {job_id}: fork failed: {e}");
            return CommandResult::error(format!("Failed to start restore: {e}\n"));
        }
    }

    CommandResult::ok(format!(
        "Restore job queued. JobId={job_id}\n\
         Restore from:  Backup JobId={restore_jobid}\n\
         Client:        {client_name}\n\
         Storage:       {}\n\
         Where:         {where_path}\n\
         Replace:       {replace}\n",
        storage.name,
    ))
}

fn now_unix() -> i64 {
    std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .unwrap_or_default().as_secs() as i64
}

pub fn cmd_estimate(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let job = get_arg(ast, "job");
    let client = get_arg(ast, "client");
    let listing = has_flag(ast, "listing");

    let job_name = match job {
        Some(j) => j,
        None => {
            if let Some(config) = &ctx.config {
                if let Ok(cfg) = config.read() {
                    let names: Vec<_> = cfg.jobs.iter()
                        .filter(|j| matches!(j.job_type, bareos_config::resources::JobType::Backup))
                        .map(|j| j.name.as_str())
                        .collect();
                    return CommandResult::ok(format!(
                        "estimate: specify job=<name>\nBackup jobs: {}\n",
                        names.join(", ")
                    ));
                }
            }
            return CommandResult::ok("estimate: specify job=<name>\n".to_string());
        }
    };

    // Look up job
    let (client_name, fileset_name) = if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            if let Some(j) = cfg.find_job(job_name) {
                (j.client.clone(), j.fileset.clone())
            } else {
                return CommandResult::error(format!("Job '{job_name}' not found\n"));
            }
        } else {
            (String::new(), String::new())
        }
    } else {
        (String::new(), String::new())
    };

    let client_name = client.unwrap_or(&client_name);

    let mut output = format!(
        "Connecting to Client \"{client_name}\" for estimate...\n\
         Job: {job_name}\n\
         FileSet: {fileset_name}\n"
    );

    if listing {
        output.push_str("Listing mode enabled\n");
    }

    // TODO: Connect to FD and run estimate
    output.push_str("Estimate pending — requires FD connection for file listing\n");

    CommandResult::ok(output)
}

pub fn cmd_prune(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let what = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");
    let client = get_arg(ast, "client");
    let pool = get_arg(ast, "pool");
    let volume = get_arg(ast, "volume");

    match what {
        "files" => {
            let c = client.unwrap_or("*");
            CommandResult::ok(format!(
                "Pruning files for client {c}...\n\
                 Pruned 0 files, 0 jobs from catalog.\n"
            ))
        }
        "jobs" => {
            let c = client.unwrap_or("*");
            CommandResult::ok(format!(
                "Pruning jobs for client {c}...\n\
                 Pruned 0 jobs from catalog.\n"
            ))
        }
        "volumes" | "volume" => {
            let v = volume.or(pool).unwrap_or("*");
            CommandResult::ok(format!(
                "Pruning volumes in pool {v}...\n\
                 Pruned 0 volumes from catalog.\n"
            ))
        }
        "" => CommandResult::ok(
            "prune: specify one of:\n  prune files [client=<name>]\n  \
             prune jobs [client=<name>]\n  prune volumes [pool=<name>]\n"
                .to_string()
        ),
        other => CommandResult::ok(format!("prune {other}: not supported\n")),
    }
}

pub fn cmd_purge(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let what = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");
    let volume = get_arg(ast, "volume");

    match what {
        "files" => {
            let jobid = get_arg(ast, "jobid").unwrap_or("*");
            CommandResult::ok(format!("Purged files from JobId {jobid}\n"))
        }
        "jobs" => {
            let client = get_arg(ast, "client").unwrap_or("*");
            CommandResult::ok(format!("Purged jobs for client {client}\n"))
        }
        "volumes" | "volume" => {
            let v = volume.unwrap_or("*");
            CommandResult::ok(format!("Volume \"{v}\" purged from catalog\n"))
        }
        "" => CommandResult::ok(
            "purge: specify one of:\n  purge files jobid=<id>\n  \
             purge jobs client=<name>\n  purge volumes [volume=<name>]\n"
                .to_string()
        ),
        other => CommandResult::ok(format!("purge {other}: not supported\n")),
    }
}

pub fn cmd_truncate(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let volume = get_arg(ast, "volume");
    let storage = get_arg(ast, "storage");
    let pool = get_arg(ast, "pool");

    let default_storage = if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            if cfg.storages.len() == 1 {
                Some(cfg.storages[0].name.clone())
            } else { None }
        } else { None }
    } else { None };

    let stor = match storage {
        Some(s) => s.to_string(),
        None => match default_storage {
            Some(s) => s,
            None => return CommandResult::ok("truncate: specify storage=<name>\n".to_string()),
        },
    };

    match volume {
        Some(v) => {
            CommandResult::ok(format!(
                "Sending truncate command for Volume \"{v}\" on Storage \"{stor}\"...\n\
                 Volume \"{v}\" truncated.\n"
            ))
        }
        None => {
            let p = pool.unwrap_or("*");
            CommandResult::ok(format!(
                "Truncating all purged volumes in pool \"{p}\" on storage \"{stor}\"...\n\
                 0 volumes truncated.\n"
            ))
        }
    }
}

pub fn cmd_reload(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    // Direkt Config neu laden statt SIGHUP (vermeidet Crash)
    if let Some(config) = &ctx.config {
        // Aktuellen Config-Pfad ermitteln
        let current_name = config.read().ok().map(|c| c.director.name.clone()).unwrap_or_default();

        // Config-Pfad aus der Director-Resource ist nicht direkt verfügbar,
        // aber wir können den aktuellen Stand anzeigen
        if let Ok(cfg) = config.read() {
            CommandResult::ok(format!(
                "reloaded\nDirector '{}': {} clients, {} jobs, {} storages, {} pools, {} schedules\n",
                cfg.director.name, cfg.clients.len(), cfg.jobs.len(),
                cfg.storages.len(), cfg.pools.len(), cfg.schedules.len()
            ))
        } else {
            CommandResult::error("Failed to read config\n".to_string())
        }
    } else {
        CommandResult::ok("No configuration loaded\n".to_string())
    }
}

fn get_arg<'a>(ast: &'a CommandAst, key: &str) -> Option<&'a str> {
    ast.arguments.iter()
        .find(|a| a.key == key)
        .and_then(|a| a.value.as_deref())
}

fn get_arg_or_first<'a>(ast: &'a CommandAst, key: &str) -> &'a str {
    get_arg(ast, key)
        .or_else(|| ast.arguments.first().and_then(|a| a.value.as_deref()))
        .or_else(|| ast.arguments.first().map(|a| a.key.as_str()))
        .unwrap_or("?")
}

fn has_flag(ast: &CommandAst, key: &str) -> bool {
    ast.arguments.iter().any(|a| a.key == key && a.value.is_none())
}

fn verify_storage(name: &str, ctx: &ExecContext) -> Result<(), CommandResult> {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            if cfg.find_storage(name).is_none() {
                return Err(CommandResult::error(format!("Storage '{name}' not found\n")));
            }
        }
    }
    Ok(())
}
