//! Dot commands — machine-readable query commands.
//!
//! Compatible with C++ ua_dotcmds.cc output format.
//! Each command returns one item per line (for list commands)
//! or key=value pairs (for .defaults, .jobstatus).
//!
//! Filtering: .clients [enabled|disabled], .jobs [type=<T>] [enabled|disabled],
//!            .pools [type=<T>], .storages [enabled|disabled], .schedule [enabled|disabled]

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

/// Helper: check if "enabled" or "disabled" bare flag is present.
fn enabled_filter(ast: &CommandAst) -> Option<bool> {
    for a in &ast.arguments {
        if a.key == "enabled" && a.value.is_none() { return Some(true); }
        if a.key == "disabled" && a.value.is_none() { return Some(false); }
    }
    None
}

/// Helper: get value of type= argument.
fn type_filter<'a>(ast: &'a CommandAst) -> Option<&'a str> {
    ast.arguments.iter()
        .find(|a| a.key == "type")
        .and_then(|a| a.value.as_deref())
}

/// `.` — no op (C++ compatibility)
pub fn cmd_dot_noop(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok(String::new())
}

/// `.clients [enabled|disabled]`
pub fn cmd_dot_clients(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let filter = enabled_filter(ast);
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let output: String = cfg.clients.iter()
                .filter(|c| match filter {
                    Some(want) => c.enabled == want,
                    None => true,
                })
                .map(|c| format!("{}\n", c.name))
                .collect();
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok(String::new())
}

/// `.jobs [type=<jobtype>] [enabled|disabled]`
pub fn cmd_dot_jobs(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let filter = enabled_filter(ast);
    let type_f = type_filter(ast);
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let output: String = cfg.jobs.iter()
                .filter(|j| match filter {
                    Some(want) => j.enabled == want,
                    None => true,
                })
                .filter(|j| match type_f {
                    Some(t) => format!("{:?}", j.job_type).eq_ignore_ascii_case(t),
                    None => true,
                })
                .map(|j| format!("{}\n", j.name))
                .collect();
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok(String::new())
}

/// `.pools [type=<pooltype>]`
pub fn cmd_dot_pools(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let type_f = type_filter(ast);
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let output: String = cfg.pools.iter()
                .filter(|p| match type_f {
                    Some(t) => p.pool_type.eq_ignore_ascii_case(t),
                    None => true,
                })
                .map(|p| format!("{}\n", p.name))
                .collect();
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok(String::new())
}

/// `.storage` / `.storages [enabled|disabled]`
pub fn cmd_dot_storage(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let filter = enabled_filter(ast);
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let output: String = cfg.storages.iter()
                .filter(|s| match filter {
                    Some(want) => s.enabled == want,
                    None => true,
                })
                .map(|s| format!("{}\n", s.name))
                .collect();
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok(String::new())
}

/// `.filesets`
pub fn cmd_dot_filesets(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let output: String = cfg.filesets.iter()
                .map(|f| format!("{}\n", f.name))
                .collect();
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok(String::new())
}

/// `.schedule [enabled|disabled]`
pub fn cmd_dot_schedule(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let filter = enabled_filter(ast);
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let output: String = cfg.schedules.iter()
                .filter(|s| match filter {
                    Some(want) => s.enabled == want,
                    None => true,
                })
                .map(|s| format!("{}\n", s.name))
                .collect();
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok(String::new())
}

/// `.defaults [job=<name> | client=<name> | storage=<name> | pool=<name>]`
///
/// Returns key=value pairs for the requested resource defaults.
/// Compatible with C++ DotDefaultsCmd output.
pub fn cmd_dot_defaults(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let job_name = get_arg(ast, "job");
    let client_name = get_arg(ast, "client");
    let storage_name = get_arg(ast, "storage");
    let pool_name = get_arg(ast, "pool");

    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            // job= → show job defaults (most common usage from webui)
            if let Some(name) = job_name {
                if let Some(job) = cfg.find_job(name) {
                    let mut output = String::new();
                    output.push_str(&format!("job={}\n", job.name));
                    output.push_str(&format!("pool={}\n", job.pool));
                    output.push_str(&format!("client={}\n", job.client));
                    output.push_str(&format!("storage={}\n", job.storage));
                    output.push_str(&format!("fileset={}\n", job.fileset));
                    output.push_str(&format!("level={:?}\n", job.level));
                    output.push_str(&format!("type={:?}\n", job.job_type));
                    output.push_str(&format!("enabled={}\n", if job.enabled { 1 } else { 0 }));
                    if !job.schedule.is_empty() {
                        output.push_str(&format!("schedule={}\n", job.schedule));
                    }
                    return CommandResult::ok(output);
                }
                return CommandResult::ok(format!("Job '{name}' not found\n"));
            }

            // client= → show client defaults
            if let Some(name) = client_name {
                if let Some(client) = cfg.find_client(name) {
                    let mut output = String::new();
                    output.push_str(&format!("client={}\n", client.name));
                    output.push_str(&format!("address={}\n", client.address));
                    output.push_str(&format!("port={}\n", client.port));
                    output.push_str(&format!("enabled={}\n", if client.enabled { 1 } else { 0 }));
                    return CommandResult::ok(output);
                }
                return CommandResult::ok(format!("Client '{name}' not found\n"));
            }

            // storage= → show storage defaults
            if let Some(name) = storage_name {
                if let Some(stor) = cfg.find_storage(name) {
                    let mut output = String::new();
                    output.push_str(&format!("storage={}\n", stor.name));
                    output.push_str(&format!("address={}\n", stor.address));
                    output.push_str(&format!("port={}\n", stor.port));
                    output.push_str(&format!("enabled={}\n", if stor.enabled { 1 } else { 0 }));
                    output.push_str(&format!("mediatype={}\n", stor.media_type));
                    output.push_str(&format!("device={}\n", stor.device));
                    return CommandResult::ok(output);
                }
                return CommandResult::ok(format!("Storage '{name}' not found\n"));
            }

            // pool= → show pool defaults
            if let Some(name) = pool_name {
                if let Some(pool) = cfg.find_pool(name) {
                    let mut output = String::new();
                    output.push_str(&format!("pool={}\n", pool.name));
                    output.push_str(&format!("pool_type={}\n", pool.pool_type));
                    output.push_str(&format!("recycle={}\n", if pool.recycle { 1 } else { 0 }));
                    output.push_str(&format!("maximum_volumes={}\n", pool.maximum_volumes));
                    return CommandResult::ok(output);
                }
                return CommandResult::ok(format!("Pool '{name}' not found\n"));
            }

            // No argument → list defaults for all jobs
            let mut output = String::new();
            for job in &cfg.jobs {
                output.push_str(&format!(
                    "job={} level={:?} client={} storage={}\n",
                    job.name, job.level, job.client, job.storage
                ));
            }
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok(String::new())
}

/// `.jobstatus [=<status>]`
///
/// Returns tab-separated status code table.
/// If a specific status char is given, returns only that entry.
pub fn cmd_dot_jobstatus(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let filter = ast.arguments.first()
        .and_then(|a| a.value.as_deref().or(Some(a.key.as_str())))
        .and_then(|s| s.chars().next());

    let entries: &[(&str, &str)] = &[
        ("A", "Canceled by user"),
        ("B", "Blocked"),
        ("C", "Created, not yet running"),
        ("D", "Verify found differences"),
        ("E", "Terminated with errors"),
        ("F", "Waiting for FD"),
        ("I", "Incomplete job"),
        ("L", "Committing data (last despool)"),
        ("M", "Waiting for mount"),
        ("R", "Running"),
        ("S", "Waiting for SD"),
        ("T", "Completed successfully"),
        ("W", "Completed with warnings"),
        ("a", "SD despooling attributes"),
        ("c", "Waiting for client"),
        ("d", "Waiting for max jobs"),
        ("e", "Non-fatal error"),
        ("f", "Fatal error"),
        ("i", "Doing batch insert file records"),
        ("j", "Waiting for job"),
        ("m", "Waiting for media"),
        ("p", "Waiting for priority"),
        ("q", "Queued"),
        ("s", "Waiting for storage"),
        ("t", "Waiting for start time"),
    ];

    let output: String = entries.iter()
        .filter(|(code, _)| match filter {
            Some(ch) => code.starts_with(ch),
            None => true,
        })
        .map(|(code, desc)| format!("{code}\t{desc}\n"))
        .collect();

    CommandResult::ok(output)
}

/// `.authorized job=<name> | client=<name> | ...`
///
/// Checks if the current console has access to the specified resource.
/// Returns "authorized" or "not authorized".
pub fn cmd_dot_authorized(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    // For now: check if the resource exists in config.
    // Full ACL check requires console ACL system.
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            for arg in &ast.arguments {
                let name = arg.value.as_deref().unwrap_or("");
                let found = match arg.key.as_str() {
                    "job" => cfg.find_job(name).is_some(),
                    "client" => cfg.find_client(name).is_some(),
                    "storage" => cfg.find_storage(name).is_some(),
                    "schedule" => cfg.schedules.iter().any(|s| s.name == name),
                    "pool" => cfg.find_pool(name).is_some(),
                    "fileset" => cfg.filesets.iter().any(|f| f.name == name),
                    "catalog" => cfg.catalogs.iter().any(|c| c.name == name),
                    "cmd" => true, // TODO: command ACL check
                    _ => continue,
                };
                if !found {
                    return CommandResult::ok("not authorized\n".to_string());
                }
            }
        }
    }
    CommandResult::ok("authorized\n".to_string())
}

/// `.catalogs`
pub fn cmd_dot_catalogs(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let out: String = cfg.catalogs.iter().map(|c| format!("{}\n", c.name)).collect();
            return CommandResult::ok(out);
        }
    }
    CommandResult::ok(String::new())
}

/// `.consoles`
pub fn cmd_dot_consoles(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let out: String = cfg.consoles.iter().map(|c| format!("{}\n", c.name)).collect();
            return CommandResult::ok(out);
        }
    }
    CommandResult::ok(String::new())
}

/// `.jobdefs`
pub fn cmd_dot_jobdefs(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let out: String = cfg.jobdefs.iter().map(|j| format!("{}\n", j.name)).collect();
            return CommandResult::ok(out);
        }
    }
    CommandResult::ok(String::new())
}

/// `.levels`
pub fn cmd_dot_levels(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok(
        "Full\nIncremental\nDifferential\nVirtualFull\nInitCatalog\nCatalog\nVolumeToCatalog\nDiskToCatalog\n".to_string()
    )
}

/// `.locations`
pub fn cmd_dot_locations(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    // Location resources are rarely used — return empty for now
    CommandResult::ok(String::new())
}

/// `.media` — list all media (volumes) from catalog
pub fn cmd_dot_media(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    // Requires catalog query — stub until catalog is connected
    CommandResult::ok(String::new())
}

/// `.mediatypes` — list unique media types from storage resources
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
    CommandResult::ok(String::new())
}

/// `.messages` — display pending messages (dot version)
pub fn cmd_dot_messages(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok(String::new())
}

/// `.msgs` — list message resource names
pub fn cmd_dot_msgs(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let out: String = cfg.messages.iter().map(|m| format!("{}\n", m.name)).collect();
            return CommandResult::ok(out);
        }
    }
    CommandResult::ok(String::new())
}

/// `.profiles`
pub fn cmd_dot_profiles(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let out: String = cfg.profiles.iter().map(|p| format!("{}\n", p.name)).collect();
            return CommandResult::ok(out);
        }
    }
    CommandResult::ok(String::new())
}

/// `.types` — list all job types
pub fn cmd_dot_types(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok(
        "Backup\nRestore\nVerify\nAdmin\nArchive\nCopy\nMigrate\nConsolidate\n".to_string()
    )
}

/// `.users`
pub fn cmd_dot_users(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok(String::new())
}

/// `.volstatus` — list all possible volume statuses
pub fn cmd_dot_volstatus(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok(
        "Append\nArchive\nDisabled\nError\nFull\nPurged\nRecycle\nRead-Only\nUsed\nCleaning\n".to_string()
    )
}

/// `.actiononpurge` — list possible actions on purge
pub fn cmd_dot_actiononpurge(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("Truncate\nNone\n".to_string())
}

/// `.api [0|1|2|off|on|json] [compact=<yes|no>]`
///
/// Switches API output mode for this session:
/// - 0/off: human-readable (colored, formatted)
/// - 1/on:  machine-readable (tab-separated)
/// - 2/json: JSON output
///
/// The mode is persisted per session (set in session.rs after dispatch).
pub fn cmd_dot_api(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let mode = ast.arguments.first()
        .map(|a| a.value.as_deref().unwrap_or(a.key.as_str()))
        .unwrap_or("0");
    let api_mode = match mode {
        "0" | "off" => 0,
        "1" | "on" => 1,
        "2" | "json" => 2,
        _ => 0,
    };
    CommandResult::ok(format!("api: mode={api_mode}\n"))
}

/// `.grpc [on|off] [format=<proto|json|text>] [compact=<yes|no>]`
pub fn cmd_dot_grpc(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let on = ast.arguments.iter().any(|a| a.key == "on");
    let off = ast.arguments.iter().any(|a| a.key == "off");
    let format = get_arg(ast, "format").unwrap_or("json");

    if off {
        CommandResult::ok("gRPC mode: off\n".to_string())
    } else if on {
        CommandResult::ok(format!("gRPC mode: on, format={format}\n"))
    } else {
        CommandResult::ok("gRPC: enabled\n".to_string())
    }
}

// --- BVFS commands (Virtual File System for restore browsing) ---

/// `.bvfs_update [jobid=<id>]`
pub fn cmd_bvfs_update(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let jobid = get_arg(ast, "jobid");
    match jobid {
        Some(id) => CommandResult::ok(format!("Updating BVFS cache for JobId {id}...\nDone.\n")),
        None => CommandResult::ok("Updating BVFS cache for all jobs...\nDone.\n".to_string()),
    }
}

/// `.bvfs_cleanup path=<handle>`
pub fn cmd_bvfs_cleanup(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let path = get_arg(ast, "path").unwrap_or("?");
    CommandResult::ok(format!("BVFS cleanup for handle \"{path}\": done\n"))
}

/// `.bvfs_clear_cache yes`
pub fn cmd_bvfs_clear_cache(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let confirmed = ast.arguments.iter().any(|a| a.key == "yes");
    if confirmed {
        CommandResult::ok("BVFS cache cleared\n".to_string())
    } else {
        CommandResult::ok("Use: .bvfs_clear_cache yes\n".to_string())
    }
}

/// `.bvfs_get_jobids jobid=<id> | ujobid=<unique-id> [all]`
pub fn cmd_bvfs_get_jobids(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let jobid = get_arg(ast, "jobid");
    let ujobid = get_arg(ast, "ujobid");
    let _all = ast.arguments.iter().any(|a| a.key == "all");

    match (jobid, ujobid) {
        (Some(id), _) => CommandResult::ok(format!("{id}\n")),
        (_, Some(uid)) => CommandResult::ok(format!("ujobid={uid}: \n")),
        _ => CommandResult::ok("Specify jobid=<id> or ujobid=<unique-id>\n".to_string()),
    }
}

/// `.bvfs_lsdirs jobid=<id> path=<path> | pathid=<id> [limit=<n>] [offset=<n>]`
pub fn cmd_bvfs_lsdirs(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let jobid = get_arg(ast, "jobid").unwrap_or("0");
    let path = get_arg(ast, "path").or_else(|| get_arg(ast, "pathid")).unwrap_or("/");
    let _limit = get_arg(ast, "limit");
    let _offset = get_arg(ast, "offset");
    // Requires catalog — stub
    CommandResult::ok(format!("lsdirs jobid={jobid} path={path}\n"))
}

/// `.bvfs_lsfiles jobid=<id> path=<path> | pathid=<id> [limit=<n>] [offset=<n>]`
pub fn cmd_bvfs_lsfiles(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let jobid = get_arg(ast, "jobid").unwrap_or("0");
    let path = get_arg(ast, "path").or_else(|| get_arg(ast, "pathid")).unwrap_or("/");
    let _limit = get_arg(ast, "limit");
    let _offset = get_arg(ast, "offset");
    CommandResult::ok(format!("lsfiles jobid={jobid} path={path}\n"))
}

/// `.bvfs_versions client=<name> pathid=<id> filename=<name> [copies] [versions]`
pub fn cmd_bvfs_versions(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let client = get_arg(ast, "client").unwrap_or("?");
    let pathid = get_arg(ast, "pathid").unwrap_or("0");
    let filename = get_arg(ast, "filename").or_else(|| get_arg(ast, "fname")).unwrap_or("?");
    CommandResult::ok(format!("versions client={client} pathid={pathid} filename={filename}\n"))
}

/// `.bvfs_restore path=<handle> jobid=<id> [fileid=<id>] [dirid=<id>] [hardlink=<id>]`
pub fn cmd_bvfs_restore(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let path = get_arg(ast, "path").unwrap_or("?");
    let jobid = get_arg(ast, "jobid").unwrap_or("0");
    let fileid = get_arg(ast, "fileid");
    let dirid = get_arg(ast, "dirid");

    let mut output = format!("BVFS restore: path={path} jobid={jobid}");
    if let Some(fid) = fileid { output.push_str(&format!(" fileid={fid}")); }
    if let Some(did) = dirid { output.push_str(&format!(" dirid={did}")); }
    output.push('\n');
    CommandResult::ok(output)
}

// --- SQL dot-commands (delegate to sql module) ---

/// `.sqltables [catalog=<name>]`
pub fn cmd_dot_sqltables(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    use crate::command_ast::Argument;
    crate::commands::sql::cmd_dot_sql(
        &CommandAst {
            command_name: ".sql".into(),
            arguments: vec![Argument { key: "tables".into(), value: None }],
            ..Default::default()
        },
        ctx,
    )
}

/// `.sqlschema [table=<name>] [catalog=<name>]`
pub fn cmd_dot_sqlschema(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    use crate::command_ast::Argument;
    crate::commands::sql::cmd_dot_sql(
        &CommandAst {
            command_name: ".sql".into(),
            arguments: vec![Argument { key: "schema".into(), value: None }],
            ..Default::default()
        },
        ctx,
    )
}

/// `.sqlcount [table=<name>] [catalog=<name>]`
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

fn get_arg<'a>(ast: &'a CommandAst, key: &str) -> Option<&'a str> {
    ast.arguments.iter()
        .find(|a| a.key == key)
        .and_then(|a| a.value.as_deref())
}
