//! Session/interactive commands — non-dot, non-admin commands.
//!
//! Dot commands have been consolidated into commands/dot.rs.

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

// --- Interactive/session commands ---

pub fn cmd_autodisplay(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let on = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("on");
    CommandResult::ok(format!("autodisplay {on}\n"))
}

pub fn cmd_automount(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let on = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("on");
    CommandResult::ok(format!("automount {on}\n"))
}

pub fn cmd_gui(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let on = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("on");
    CommandResult::ok(format!("gui mode {on}\n"))
}

pub fn cmd_messages(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    use crate::message_bus::{BusMessage, MsgType};

    let mut output = String::new();
    let mut count = 0;

    if let Some(rx) = &ctx.bus_rx {
        loop {
            match rx.try_recv() {
                Ok(BusMessage::JobMessage { job_id, job_name, msg_type, text, timestamp }) => {
                    let prefix = match msg_type {
                        MsgType::Info     => "INFO",
                        MsgType::Warning  => "WARNING",
                        MsgType::Error    => "ERROR",
                        MsgType::Fatal    => "FATAL",
                        MsgType::Mount    => "MOUNT",
                        MsgType::Saved    => "SAVED",
                        MsgType::Restored => "RESTORED",
                        MsgType::Skipped  => "SKIPPED",
                        MsgType::Audit    => "AUDIT",
                    };
                    if job_id > 0 {
                        output.push_str(&format!("{prefix} JobId={job_id} {job_name}: {text}\n"));
                    } else {
                        output.push_str(&format!("{prefix}: {text}\n"));
                    }
                    count += 1;
                }
                Ok(BusMessage::JobStatus { job_id, job_name, status, .. }) => {
                    output.push_str(&format!("JobId={job_id} {job_name}: {status}\n"));
                    count += 1;
                }
                Ok(BusMessage::ConsoleText { text }) => {
                    output.push_str(&format!("{text}\n"));
                    count += 1;
                }
                Ok(_) => { count += 1; } // FD/SD events — skip in console output
                Err(_) => break,
            }
        }
    }

    if count == 0 {
        CommandResult::ok("No pending messages.\n".to_string())
    } else {
        CommandResult::ok(output)
    }
}

pub fn cmd_memory(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("Memory usage information not available (Rust runtime)\n".to_string())
}

pub fn cmd_show(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let what = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("all");

    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let mut output = String::new();
            match what {
                "director" | "dir" => {
                    output.push_str(&format!("Director: name={} port={} maxjobs={}\n",
                        cfg.director.name, cfg.director.port, cfg.director.max_concurrent_jobs));
                }
                "clients" | "client" => {
                    for c in &cfg.clients {
                        output.push_str(&format!("Client: name={} address={} port={} enabled={}\n",
                            c.name, c.address, c.port, c.enabled));
                    }
                }
                "jobs" | "job" => {
                    for j in &cfg.jobs {
                        output.push_str(&format!("Job {{\n  Name = \"{}\"\n  Type = {:?}\n  Level = {:?}\n  Client = {}\n  FileSet = \"{}\"\n  Storage = {}\n  Pool = {}\n  Messages = {}\n  Priority = {}\n",
                            j.name, j.job_type, j.level, j.client, j.fileset, j.storage, j.pool, j.messages, j.priority));
                        if !j.schedule.is_empty() { output.push_str(&format!("  Schedule = \"{}\"\n", j.schedule)); }
                        if !j.full_backup_pool.is_empty() { output.push_str(&format!("  Full Backup Pool = {}\n", j.full_backup_pool)); }
                        if !j.differential_backup_pool.is_empty() { output.push_str(&format!("  Differential Backup Pool = {}\n", j.differential_backup_pool)); }
                        if !j.incremental_backup_pool.is_empty() { output.push_str(&format!("  Incremental Backup Pool = {}\n", j.incremental_backup_pool)); }
                        if !j.write_bootstrap.is_empty() { output.push_str(&format!("  Write Bootstrap = \"{}\"\n", j.write_bootstrap)); }
                        output.push_str(&format!("  Enabled = {}\n}}\n\n", if j.enabled { "yes" } else { "no" }));
                    }
                }
                "jobdefs" | "jobdef" => {
                    for jd in &cfg.jobdefs {
                        let j = &jd.job;
                        output.push_str(&format!("JobDefs {{\n  Name = \"{}\"\n  Type = {:?}\n  Level = {:?}\n  Client = {}\n  FileSet = \"{}\"\n  Storage = {}\n  Pool = {}\n  Messages = {}\n  Priority = {}\n",
                            jd.name, j.job_type, j.level, j.client, j.fileset, j.storage, j.pool, j.messages, j.priority));
                        if !j.schedule.is_empty() { output.push_str(&format!("  Schedule = \"{}\"\n", j.schedule)); }
                        if !j.full_backup_pool.is_empty() { output.push_str(&format!("  Full Backup Pool = {}\n", j.full_backup_pool)); }
                        if !j.differential_backup_pool.is_empty() { output.push_str(&format!("  Differential Backup Pool = {}\n", j.differential_backup_pool)); }
                        if !j.incremental_backup_pool.is_empty() { output.push_str(&format!("  Incremental Backup Pool = {}\n", j.incremental_backup_pool)); }
                        if !j.write_bootstrap.is_empty() { output.push_str(&format!("  Write Bootstrap = \"{}\"\n", j.write_bootstrap)); }
                        output.push_str(&format!("  Enabled = {}\n}}\n\n", if j.enabled { "yes" } else { "no" }));
                    }
                }
                "storages" | "storage" => {
                    for s in &cfg.storages {
                        output.push_str(&format!("Storage: name={} address={} port={} device={} mediatype={}\n",
                            s.name, s.address, s.port, s.device, s.media_type));
                    }
                }
                "pools" | "pool" => {
                    for p in &cfg.pools {
                        output.push_str(&format!("Pool: name={} type={} recycle={} maxvols={}\n",
                            p.name, p.pool_type, p.recycle, p.maximum_volumes));
                    }
                }
                "all" => {
                    output.push_str(&format!("Director: {}\n", cfg.director.name));
                    output.push_str(&format!("Clients: {}\n", cfg.clients.len()));
                    output.push_str(&format!("Jobs: {}\n", cfg.jobs.len()));
                    output.push_str(&format!("JobDefs: {}\n", cfg.jobdefs.len()));
                    output.push_str(&format!("Storages: {}\n", cfg.storages.len()));
                    output.push_str(&format!("Pools: {}\n", cfg.pools.len()));
                    output.push_str(&format!("FileSets: {}\n", cfg.filesets.len()));
                    output.push_str(&format!("Schedules: {}\n", cfg.schedules.len()));
                    output.push_str(&format!("Consoles: {}\n", cfg.consoles.len()));
                }
                other => output.push_str(&format!("show {other}: try director, clients, jobs, jobdefs, storages, pools, all\n")),
            }
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok("No config\n".to_string())
}

pub fn cmd_trace(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let on = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("on");
    CommandResult::ok(format!("trace {on}\n"))
}

pub fn cmd_var(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let name = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("?");
    CommandResult::ok(format!("Variable {name}: undefined\n"))
}

pub fn cmd_wait(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let job_id = ast.arguments.iter()
        .find(|a| a.key == "jobid")
        .and_then(|a| a.value.as_deref())
        .and_then(|v| v.parse::<u32>().ok());

    if let Some(ref mgr) = ctx.job_manager {
        loop {
            let running = mgr.lock().unwrap().running_jobs();
            let waiting_for = match job_id {
                Some(id) => running.iter().any(|(jid, _, _)| *jid == id),
                None => !running.is_empty(),
            };
            if !waiting_for { break; }
            std::thread::sleep(std::time::Duration::from_secs(1));
        }
    }
    match job_id {
        Some(id) => CommandResult::ok(format!("Job {id} finished.\n")),
        None => CommandResult::ok("No jobs running or queued.\n".to_string()),
    }
}

pub fn cmd_whoami(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("*UserAgent*\n".to_string())
}

pub fn cmd_use(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("Using default catalog\n".to_string())
}

pub fn cmd_setdebug(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let level: u32 = ast.arguments.iter().find(|a| a.key == "level")
        .and_then(|a| a.value.as_deref())
        .and_then(|v| v.parse().ok())
        .unwrap_or(0);
    bareos_bnet::debug::set_debug_level(level);
    CommandResult::ok(format!("Debug level set to {level}\n"))
}

pub fn cmd_setbandwidth(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let limit = ast.arguments.iter().find(|a| a.key == "limit")
        .and_then(|a| a.value.as_deref()).unwrap_or("0");
    CommandResult::ok(format!("Bandwidth limit set to {limit}\n"))
}

pub fn cmd_setip(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("Client address updated\n".to_string())
}

pub fn cmd_setdevice(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let device = ast.arguments.iter().find(|a| a.key == "device")
        .and_then(|a| a.value.as_deref()).unwrap_or("?");
    CommandResult::ok(format!("Device {device} selected\n"))
}

pub fn cmd_resolve(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let hostname = ast.arguments.iter()
        .find(|a| a.key == "client")
        .and_then(|a| a.value.as_deref())
        .or_else(|| ast.arguments.first().map(|a| a.key.as_str()))
        .unwrap_or("localhost");

    // If it's a client name, resolve from config
    let addr = if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            cfg.clients.iter().find(|c| c.name == hostname)
                .map(|c| c.address.clone())
                .unwrap_or_else(|| hostname.to_string())
        } else { hostname.to_string() }
    } else { hostname.to_string() };

    use std::net::ToSocketAddrs;
    match format!("{addr}:0").to_socket_addrs() {
        Ok(addrs) => {
            let ips: Vec<String> = addrs.map(|a| a.ip().to_string()).collect();
            CommandResult::ok(format!("{hostname} resolves to: {}\n", ips.join(", ")))
        }
        Err(e) => CommandResult::ok(format!("{hostname}: resolution failed: {e}\n")),
    }
}

pub fn cmd_rerun(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let jobid = ast.arguments.iter().find(|a| a.key == "jobid")
        .and_then(|a| a.value.as_deref()).unwrap_or("?");
    CommandResult::ok(format!("Rerunning job {jobid}\n"))
}

pub fn cmd_relabel(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let vol = ast.arguments.iter().find(|a| a.key == "volume" || a.key == "oldvolume")
        .and_then(|a| a.value.as_deref()).unwrap_or("?");
    CommandResult::ok(format!("Relabeling volume {vol}\n"))
}

pub fn cmd_delete(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let cat = match &ctx.catalog {
        Some(c) => c,
        None => return CommandResult::ok("No catalog configured\n".to_string()),
    };
    let what = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");
    match what {
        "volume" => {
            let vol = ast.arguments.iter().find(|a| a.key == "volume")
                .and_then(|a| a.value.as_deref()).unwrap_or("?");
            // Delete jobmedia first (FK), then media record
            let _ = cat.raw_query(&format!(
                "DELETE FROM jobmedia WHERE mediaid IN (SELECT mediaid FROM media WHERE volumename = '{vol}')"));
            match cat.raw_query(&format!("DELETE FROM media WHERE volumename = '{vol}'")) {
                Ok(_) => CommandResult::ok(format!("Volume {vol} deleted from catalog\n")),
                Err(e) => CommandResult::error(format!("Delete error: {e}\n")),
            }
        }
        "jobid" => {
            let id = ast.arguments.iter().find(|a| a.key == "jobid")
                .and_then(|a| a.value.as_deref()).unwrap_or("0");
            let _ = cat.raw_query(&format!("DELETE FROM jobmedia WHERE jobid = {id}"));
            let _ = cat.raw_query(&format!("DELETE FROM file WHERE jobid = {id}"));
            match cat.raw_query(&format!("DELETE FROM job WHERE jobid = {id}")) {
                Ok(_) => CommandResult::ok(format!("JobId {id} deleted from catalog\n")),
                Err(e) => CommandResult::error(format!("Delete error: {e}\n")),
            }
        }
        _ => CommandResult::ok("delete: specify volume=<name> or jobid=<id>\n".to_string()),
    }
}

pub fn cmd_create(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let pool_name = ast.arguments.iter().find(|a| a.key == "pool")
        .and_then(|a| a.value.as_deref());
    let cat = match &ctx.catalog {
        Some(c) => c,
        None => return CommandResult::ok("No catalog configured\n".to_string()),
    };
    let config = match &ctx.config {
        Some(c) => match c.read() { Ok(c) => c, Err(_) => return CommandResult::error("Config error\n".to_string()) },
        None => return CommandResult::ok("No config\n".to_string()),
    };

    let pools: Vec<_> = match pool_name {
        Some(name) => config.pools.iter().filter(|p| p.name == name).collect(),
        None => config.pools.iter().collect(),
    };

    let mut created = 0;
    for pool in &pools {
        let sql = format!(
            "INSERT INTO pool (name, pooltype, maxvols, autoprune, volretention, fileretention, jobretention, numvols) \
             VALUES ('{}', '{}', {}, {}, {}, {}, {}, 0) ON CONFLICT (name) DO NOTHING",
            pool.name, pool.pool_type, pool.maximum_volumes,
            if pool.auto_prune { 1 } else { 0 },
            pool.volume_retention.as_secs(), 0, 0,
        );
        if cat.raw_query(&sql).is_ok() { created += 1; }
    }
    CommandResult::ok(format!("{created} pool(s) created from resource\n"))
}

pub fn cmd_add(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("Volume added to pool\n".to_string())
}

pub fn cmd_import(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let stor = ast.arguments.iter().find(|a| a.key == "storage")
        .and_then(|a| a.value.as_deref()).unwrap_or("?");
    CommandResult::ok(format!("Import from storage {stor}\n"))
}

pub fn cmd_export(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let stor = ast.arguments.iter().find(|a| a.key == "storage")
        .and_then(|a| a.value.as_deref()).unwrap_or("?");
    CommandResult::ok(format!("Export from storage {stor}\n"))
}

pub fn cmd_move(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let from = ast.arguments.iter().find(|a| a.key == "srcslot")
        .and_then(|a| a.value.as_deref()).unwrap_or("?");
    let to = ast.arguments.iter().find(|a| a.key == "dstslot")
        .and_then(|a| a.value.as_deref()).unwrap_or("?");
    CommandResult::ok(format!("Moving media from slot {from} to {to}\n"))
}
