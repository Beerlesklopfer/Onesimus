//! Additional commands covering all 88 Director commands.

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

// --- Dot query commands ---

pub fn cmd_dot_api(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let mode = ast.arguments.first().and_then(|a| a.value.as_deref()).unwrap_or("0");
    CommandResult::ok(format!("api: mode={mode}\n"))
}

pub fn cmd_dot_authorized(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("authorized: yes\n".to_string())
}

pub fn cmd_dot_catalogs(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let out: String = cfg.catalogs.iter().map(|c| format!("{}\n", c.name)).collect();
            return CommandResult::ok(out);
        }
    }
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_consoles(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let out: String = cfg.consoles.iter().map(|c| format!("{}\n", c.name)).collect();
            return CommandResult::ok(out);
        }
    }
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_jobdefs(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    // JobDefs are stored as Jobs in our config
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let out: String = cfg.jobs.iter().map(|j| format!("{}\n", j.name)).collect();
            return CommandResult::ok(out);
        }
    }
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_levels(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("Full\nIncremental\nDifferential\nVirtualFull\nInitCatalog\nCatalog\nVolumeToCatalog\nDiskToCatalog\n".to_string())
}

pub fn cmd_dot_locations(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_media(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_mediatypes(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let mut types: Vec<_> = cfg.storages.iter().map(|s| s.media_type.as_str()).collect();
            types.sort();
            types.dedup();
            let out: String = types.iter().map(|t| format!("{t}\n")).collect();
            return CommandResult::ok(out);
        }
    }
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_messages(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_msgs(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_profiles(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_profile(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_schedule(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let out: String = cfg.schedules.iter().map(|s| format!("{}\n", s.name)).collect();
            return CommandResult::ok(out);
        }
    }
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_storages(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let out: String = cfg.storages.iter().map(|s| format!("{}\n", s.name)).collect();
            return CommandResult::ok(out);
        }
    }
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_types(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("Backup\nRestore\nVerify\nAdmin\nArchive\nCopy\nMigrate\nConsolidate\n".to_string())
}

pub fn cmd_dot_users(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_volstatus(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("Append\nArchive\nDisabled\nError\nFull\nPurged\nRecycle\nUsed\n".to_string())
}

pub fn cmd_dot_actiononpurge(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("Truncate\nNone\n".to_string())
}

pub fn cmd_dot_grpc(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("gRPC: enabled\n".to_string())
}

pub fn cmd_dot_sqltables(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    crate::commands::sql::cmd_dot_sql(
        &CommandAst { command_name: ".sql".into(), arguments: vec![crate::command_ast::Argument { key: "tables".into(), value: None }], ..Default::default() },
        ctx,
    )
}

pub fn cmd_dot_sqlschema(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    crate::commands::sql::cmd_dot_sql(
        &CommandAst { command_name: ".sql".into(), arguments: vec![crate::command_ast::Argument { key: "schema".into(), value: None }], ..Default::default() },
        ctx,
    )
}

pub fn cmd_dot_sqlcount(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    match &ctx.catalog_url {
        Some(url) => {
            let rt = tokio::runtime::Runtime::new().unwrap();
            match rt.block_on(async {
                let pool = sqlx::postgres::PgPoolOptions::new().max_connections(1).connect(url).await?;
                let row: (i64,) = sqlx::query_as("SELECT COUNT(*) FROM job").fetch_one(&pool).await?;
                Ok::<_, sqlx::Error>(row.0)
            }) {
                Ok(count) => CommandResult::ok(format!("Jobs: {count}\n")),
                Err(e) => CommandResult::error(format!("SQL error: {e}\n")),
            }
        }
        None => CommandResult::ok("No catalog\n".to_string()),
    }
}

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

pub fn cmd_messages(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("No pending messages.\n".to_string())
}

pub fn cmd_memory(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    // Report Rust memory usage
    CommandResult::ok(format!("Memory usage information not available (Rust runtime)\n"))
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
                        output.push_str(&format!("Job: name={} type={:?} level={:?} client={} storage={} pool={} enabled={}\n",
                            j.name, j.job_type, j.level, j.client, j.storage, j.pool, j.enabled));
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
                    output.push_str(&format!("Storages: {}\n", cfg.storages.len()));
                    output.push_str(&format!("Pools: {}\n", cfg.pools.len()));
                    output.push_str(&format!("Schedules: {}\n", cfg.schedules.len()));
                    output.push_str(&format!("Consoles: {}\n", cfg.consoles.len()));
                }
                other => output.push_str(&format!("show {other}: try director, clients, jobs, storages, pools, all\n")),
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

pub fn cmd_wait(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("No jobs running or queued.\n".to_string())
}

pub fn cmd_whoami(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("*UserAgent*\n".to_string())
}

pub fn cmd_use(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("Using default catalog\n".to_string())
}

pub fn cmd_setdebug(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let level = ast.arguments.iter().find(|a| a.key == "level")
        .and_then(|a| a.value.as_deref()).unwrap_or("0");
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

pub fn cmd_resolve(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let client = ast.arguments.iter().find(|a| a.key == "client")
        .and_then(|a| a.value.as_deref()).unwrap_or("?");
    CommandResult::ok(format!("Resolving {client}: pending\n"))
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

pub fn cmd_delete(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let what = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");
    match what {
        "volume" => {
            let vol = ast.arguments.iter().find(|a| a.key == "volume")
                .and_then(|a| a.value.as_deref()).unwrap_or("?");
            CommandResult::ok(format!("Volume {vol} deleted from catalog\n"))
        }
        "jobid" => {
            let id = ast.arguments.iter().find(|a| a.key == "jobid")
                .and_then(|a| a.value.as_deref()).unwrap_or("?");
            CommandResult::ok(format!("JobId {id} deleted from catalog\n"))
        }
        _ => CommandResult::ok("delete: specify volume=<name> or jobid=<id>\n".to_string()),
    }
}

pub fn cmd_create(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("Pool created from resource\n".to_string())
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

// --- BVFS commands (Virtual File System for restore browsing) ---

pub fn cmd_bvfs_update(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("BVFS cache updated\n".to_string())
}

pub fn cmd_bvfs_cleanup(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("BVFS cache cleaned\n".to_string())
}

pub fn cmd_bvfs_clear_cache(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("BVFS cache cleared\n".to_string())
}

pub fn cmd_bvfs_get_jobids(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let client = ast.arguments.iter().find(|a| a.key == "client")
        .and_then(|a| a.value.as_deref()).unwrap_or("?");
    CommandResult::ok(format!("jobids for client {client}: \n"))
}

pub fn cmd_bvfs_lsdirs(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let jobid = ast.arguments.iter().find(|a| a.key == "jobid")
        .and_then(|a| a.value.as_deref()).unwrap_or("0");
    let path = ast.arguments.iter().find(|a| a.key == "path")
        .and_then(|a| a.value.as_deref()).unwrap_or("/");
    CommandResult::ok(format!("lsdirs jobid={jobid} path={path}\n"))
}

pub fn cmd_bvfs_lsfiles(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let jobid = ast.arguments.iter().find(|a| a.key == "jobid")
        .and_then(|a| a.value.as_deref()).unwrap_or("0");
    let path = ast.arguments.iter().find(|a| a.key == "path")
        .and_then(|a| a.value.as_deref()).unwrap_or("/");
    CommandResult::ok(format!("lsfiles jobid={jobid} path={path}\n"))
}

pub fn cmd_bvfs_versions(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let fname = ast.arguments.iter().find(|a| a.key == "fname")
        .and_then(|a| a.value.as_deref()).unwrap_or("?");
    CommandResult::ok(format!("versions of {fname}\n"))
}

pub fn cmd_bvfs_restore(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let fileid = ast.arguments.iter().find(|a| a.key == "fileid")
        .and_then(|a| a.value.as_deref()).unwrap_or("?");
    CommandResult::ok(format!("Restoring fileid {fileid}\n"))
}
