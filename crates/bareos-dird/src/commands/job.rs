//! Job control commands: run, cancel, enable, disable

use std::sync::atomic::{AtomicU32, Ordering};

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;
use crate::job_runner;
use crate::message_bus::BusMessage;

static NEXT_JOB_ID: AtomicU32 = AtomicU32::new(1);

pub fn cmd_run(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let job_name = ast.arguments.iter()
        .find(|a| a.key == "job")
        .and_then(|a| a.value.as_deref());

    let job_name = match job_name {
        Some(name) => name,
        None => {
            if let Some(config) = &ctx.config {
                if let Ok(cfg) = config.read() {
                    let jobs: Vec<_> = cfg.jobs.iter().map(|j| j.name.as_str()).collect();
                    if jobs.is_empty() {
                        return CommandResult::ok("No jobs configured\n".to_string());
                    }
                    let mut output = String::from("The defined Job resources are:\n");
                    for (i, name) in jobs.iter().enumerate() {
                        output.push_str(&format!("     {}. {name}\n", i + 1));
                    }
                    output.push_str("Select Job resource (1): ");
                    return CommandResult::ok(output);
                }
            }
            return CommandResult::ok("run: specify job=<name>\n".to_string());
        }
    };

    let config = match &ctx.config {
        Some(c) => match c.read() {
            Ok(c) => c,
            _ => return CommandResult::error("Config read error\n".to_string()),
        },
        None => return CommandResult::error("No config loaded\n".to_string()),
    };

    let job = match config.find_job(job_name) {
        Some(j) => j.clone(),
        None => return CommandResult::error(format!("Job '{job_name}' not found in configuration\n")),
    };

    if !job.enabled {
        return CommandResult::error(format!("Job '{job_name}' is disabled\n"));
    }

    let default_level = format!("{:?}", job.level);
    let level = ast.arguments.iter()
        .find(|a| a.key == "level")
        .and_then(|a| a.value.as_deref())
        .unwrap_or(&default_level);

    let client = ast.arguments.iter()
        .find(|a| a.key == "client")
        .and_then(|a| a.value.as_deref())
        .unwrap_or(&job.client);

    let storage = ast.arguments.iter()
        .find(|a| a.key == "storage")
        .and_then(|a| a.value.as_deref())
        .unwrap_or(&job.storage);

    let pool = ast.arguments.iter()
        .find(|a| a.key == "pool")
        .and_then(|a| a.value.as_deref())
        .unwrap_or(&job.pool);

    // Verify client exists
    if !client.is_empty() {
        if config.find_client(client).is_none() {
            return CommandResult::error(format!("Client '{client}' not found in configuration\n"));
        }
    }

    // Verify storage exists
    if !storage.is_empty() {
        if config.find_storage(storage).is_none() {
            return CommandResult::error(format!("Storage '{storage}' not found in configuration\n"));
        }
    }

    let output = format!(
        "Job queued. JobId=1\n\
         Run {:?} job\n\
         Job:        {}\n\
         Level:      {}\n\
         Client:     {}\n\
         Storage:    {}\n\
         Pool:       {}\n\
         FileSet:    {}\n\
         Priority:   {}\n\
         When:       now\n",
        job.job_type, job.name, level, client, storage, pool, job.fileset, job.priority,
    );

    let job_id = NEXT_JOB_ID.fetch_add(1, Ordering::Relaxed);

    // Post job queued event
    if let Some(smgr) = &ctx.session_mgr {
        smgr.bus().post(BusMessage::JobStatus {
            job_id,
            job_name: job.name.clone(),
            status: "Queued".to_string(),
            timestamp: std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap_or_default()
                .as_secs() as i64,
        });
    }

    // Spawn job in background thread (fork will happen inside)
    let job_clone = job.clone();
    let client_res = config.find_client(client).cloned();
    let storage_res = config.find_storage(storage).cloned();
    let dir_name = config.director.name.clone();
    drop(config); // Release RwLock before spawning

    if let (Some(client_res), Some(storage_res)) = (client_res, storage_res) {
        let bus = ctx.session_mgr.as_ref().map(|s| s.bus().clone());
        std::thread::Builder::new()
            .name(format!("job:{job_id}"))
            .spawn(move || {
                let result = job_runner::run_backup(
                    job_id, &job_clone, &client_res, &storage_res, &dir_name,
                );
                tracing::info!("Job {job_id} '{}': status={:?} files={} bytes={} errors={}",
                    result.job_name, result.status, result.files, result.bytes, result.errors);
            })
            .ok();
    }

    let output = format!(
        "Job queued. JobId={job_id}\n\
         Run {:?} job\n\
         Job:        {}\n\
         Level:      {level}\n\
         Client:     {client}\n\
         Storage:    {storage}\n\
         Pool:       {pool}\n\
         FileSet:    {}\n\
         Priority:   {}\n\
         When:       now\n",
        job.job_type, job.name, job.fileset, job.priority,
    );

    CommandResult::ok(output)
}

pub fn cmd_cancel(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let job_id = ast.arguments.iter()
        .find(|a| a.key == "jobid")
        .and_then(|a| a.value.as_deref())
        .and_then(|v| v.parse::<u32>().ok());

    match job_id {
        Some(id) => {
            // Post cancellation event
            if let Some(smgr) = &ctx.session_mgr {
                smgr.bus().post(BusMessage::JobStatus {
                    job_id: id,
                    job_name: String::new(),
                    status: "Canceled".to_string(),
                    timestamp: std::time::SystemTime::now()
                        .duration_since(std::time::UNIX_EPOCH)
                        .unwrap_or_default()
                        .as_secs() as i64,
                });
            }

            // TODO: Send SIGTERM to job worker process
            CommandResult::ok(format!("Job {id} marked for cancellation\n"))
        }
        None => {
            // List running jobs for selection
            CommandResult::ok("cancel: specify jobid=<id>\nNo running jobs to cancel.\n".to_string())
        }
    }
}

pub fn cmd_enable(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let target = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");
    let name = ast.arguments.first().and_then(|a| a.value.as_deref()).unwrap_or("");

    if name.is_empty() {
        return CommandResult::ok(
            "enable: specify one of:\n  client=<name>\n  job=<name>\n  schedule=<name>\n  storage=<name>\n"
                .to_string(),
        );
    }

    let config = match &ctx.config {
        Some(c) => c,
        None => return CommandResult::error("No config loaded\n".to_string()),
    };

    match target {
        "client" => {
            let mut cfg = config.write().unwrap();
            if let Some(c) = cfg.clients.iter_mut().find(|c| c.name == name) {
                if c.enabled {
                    CommandResult::ok(format!("Client '{name}' is already enabled\n"))
                } else {
                    c.enabled = true;
                    CommandResult::ok(format!("Client '{name}' enabled\n"))
                }
            } else {
                CommandResult::error(format!("Client '{name}' not found\n"))
            }
        }
        "job" => {
            let mut cfg = config.write().unwrap();
            if let Some(j) = cfg.jobs.iter_mut().find(|j| j.name == name) {
                if j.enabled {
                    CommandResult::ok(format!("Job '{name}' is already enabled\n"))
                } else {
                    j.enabled = true;
                    CommandResult::ok(format!("Job '{name}' enabled\n"))
                }
            } else {
                CommandResult::error(format!("Job '{name}' not found\n"))
            }
        }
        "schedule" => {
            let mut cfg = config.write().unwrap();
            if let Some(s) = cfg.schedules.iter_mut().find(|s| s.name == name) {
                if s.enabled {
                    CommandResult::ok(format!("Schedule '{name}' is already enabled\n"))
                } else {
                    s.enabled = true;
                    CommandResult::ok(format!("Schedule '{name}' enabled\n"))
                }
            } else {
                CommandResult::error(format!("Schedule '{name}' not found\n"))
            }
        }
        "storage" => {
            let mut cfg = config.write().unwrap();
            if let Some(s) = cfg.storages.iter_mut().find(|s| s.name == name) {
                if s.enabled {
                    CommandResult::ok(format!("Storage '{name}' is already enabled\n"))
                } else {
                    s.enabled = true;
                    CommandResult::ok(format!("Storage '{name}' enabled\n"))
                }
            } else {
                CommandResult::error(format!("Storage '{name}' not found\n"))
            }
        }
        _ => CommandResult::ok("enable: specify client=, job=, schedule=, or storage=\n".to_string()),
    }
}

pub fn cmd_disable(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let target = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");
    let name = ast.arguments.first().and_then(|a| a.value.as_deref()).unwrap_or("");

    if name.is_empty() {
        return CommandResult::ok(
            "disable: specify one of:\n  client=<name>\n  job=<name>\n  schedule=<name>\n  storage=<name>\n"
                .to_string(),
        );
    }

    let config = match &ctx.config {
        Some(c) => c,
        None => return CommandResult::error("No config loaded\n".to_string()),
    };

    match target {
        "client" => {
            let mut cfg = config.write().unwrap();
            if let Some(c) = cfg.clients.iter_mut().find(|c| c.name == name) {
                if !c.enabled {
                    CommandResult::ok(format!("Client '{name}' is already disabled\n"))
                } else {
                    c.enabled = false;
                    CommandResult::ok(format!("Client '{name}' disabled\n"))
                }
            } else {
                CommandResult::error(format!("Client '{name}' not found\n"))
            }
        }
        "job" => {
            let mut cfg = config.write().unwrap();
            if let Some(j) = cfg.jobs.iter_mut().find(|j| j.name == name) {
                if !j.enabled {
                    CommandResult::ok(format!("Job '{name}' is already disabled\n"))
                } else {
                    j.enabled = false;
                    CommandResult::ok(format!("Job '{name}' disabled\n"))
                }
            } else {
                CommandResult::error(format!("Job '{name}' not found\n"))
            }
        }
        "schedule" => {
            let mut cfg = config.write().unwrap();
            if let Some(s) = cfg.schedules.iter_mut().find(|s| s.name == name) {
                if !s.enabled {
                    CommandResult::ok(format!("Schedule '{name}' is already disabled\n"))
                } else {
                    s.enabled = false;
                    CommandResult::ok(format!("Schedule '{name}' disabled\n"))
                }
            } else {
                CommandResult::error(format!("Schedule '{name}' not found\n"))
            }
        }
        "storage" => {
            let mut cfg = config.write().unwrap();
            if let Some(s) = cfg.storages.iter_mut().find(|s| s.name == name) {
                if !s.enabled {
                    CommandResult::ok(format!("Storage '{name}' is already disabled\n"))
                } else {
                    s.enabled = false;
                    CommandResult::ok(format!("Storage '{name}' disabled\n"))
                }
            } else {
                CommandResult::error(format!("Storage '{name}' not found\n"))
            }
        }
        _ => CommandResult::ok("disable: specify client=, job=, schedule=, or storage=\n".to_string()),
    }
}
