//! List commands: list, llist
//!
//! Queries the PostgreSQL catalog for job, volume, pool, and client records.

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_list(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let what = ast.arguments.first()
        .map(|a| a.key.as_str())
        .unwrap_or("jobs");

    let limit = ast.arguments.iter()
        .find(|a| a.key == "limit")
        .and_then(|a| a.value.as_deref())
        .and_then(|v| v.parse::<i64>().ok())
        .unwrap_or(25);

    // Config-only Ressourcen — stehen NICHT in der DB
    match what {
        "filesets" | "fileset" => return list_filesets_from_config(ctx),
        _ => {}
    }

    // DB-Query wenn Catalog vorhanden
    if let Some(url) = &ctx.catalog_url {
        // Check for argument-based queries first (jobid=N, job=NAME)
        if let Some(result) = list_from_db_with_args(ast, url) {
            return result;
        }
        return list_from_db(what, limit, url);
    }

    // Kein Catalog — Config-Fallback
    match what {
        "jobs" => list_jobs_from_config(ctx),
        "clients" => list_clients_from_config(ctx),
        "pools" => list_pools_from_config(ctx),
        "storages" | "storage" => list_storages_from_config(ctx),
        "schedules" | "schedule" => list_schedules_from_config(ctx),
        "volumes" | "media" => CommandResult::ok("No catalog — use .sql for volume queries\n".to_string()),
        "files" => CommandResult::ok("No catalog — use .sql for file queries\n".to_string()),
        "jobtotals" => CommandResult::ok("No catalog — jobtotals requires database\n".to_string()),
        other => CommandResult::ok(format!("list {other}: try jobs, clients, pools, storages, schedules, filesets, volumes\n")),
    }
}

pub fn cmd_llist(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    cmd_list(ast, ctx)
}

fn list_jobs_from_config(ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            if cfg.jobs.is_empty() {
                return CommandResult::ok("No jobs configured\n".to_string());
            }
            let mut output = format!("{:<30} {:<10} {:<15} {:<15} {:<10}\n",
                "Name", "Type", "Client", "Storage", "Enabled");
            output.push_str(&"-".repeat(80));
            output.push('\n');
            for j in &cfg.jobs {
                output.push_str(&format!("{:<30} {:<10} {:<15} {:<15} {:<10}\n",
                    j.name,
                    format!("{:?}", j.job_type),
                    if j.client.is_empty() { "-" } else { &j.client },
                    if j.storage.is_empty() { "-" } else { &j.storage },
                    if j.enabled { "yes" } else { "no" },
                ));
            }
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok("No configuration loaded\n".to_string())
}

fn list_clients_from_config(ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            if cfg.clients.is_empty() {
                return CommandResult::ok("No clients configured\n".to_string());
            }
            let mut output = format!("{:<25} {:<20} {:<10} {:<10}\n",
                "Name", "Address", "Port", "Enabled");
            output.push_str(&"-".repeat(65));
            output.push('\n');
            for c in &cfg.clients {
                output.push_str(&format!("{:<25} {:<20} {:<10} {:<10}\n",
                    c.name, c.address, c.port,
                    if c.enabled { "yes" } else { "no" },
                ));
            }
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok("No configuration loaded\n".to_string())
}

fn list_pools_from_config(ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            if cfg.pools.is_empty() {
                return CommandResult::ok("No pools configured\n".to_string());
            }
            let mut output = format!("{:<20} {:<15} {:<10} {:<10} {:<15}\n",
                "Name", "Type", "Recycle", "AutoPrune", "MaxVolBytes");
            output.push_str(&"-".repeat(70));
            output.push('\n');
            for p in &cfg.pools {
                output.push_str(&format!("{:<20} {:<15} {:<10} {:<10} {:<15}\n",
                    p.name, p.pool_type,
                    if p.recycle { "yes" } else { "no" },
                    if p.auto_prune { "yes" } else { "no" },
                    format_bytes(p.maximum_volume_bytes),
                ));
            }
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok("No configuration loaded\n".to_string())
}

fn list_storages_from_config(ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            if cfg.storages.is_empty() {
                return CommandResult::ok("No storages configured\n".to_string());
            }
            let mut output = format!("{:<20} {:<20} {:<10} {:<15} {:<10}\n",
                "Name", "Address", "Port", "MediaType", "Enabled");
            output.push_str(&"-".repeat(75));
            output.push('\n');
            for s in &cfg.storages {
                output.push_str(&format!("{:<20} {:<20} {:<10} {:<15} {:<10}\n",
                    s.name, s.address, s.port, s.media_type,
                    if s.enabled { "yes" } else { "no" },
                ));
            }
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok("No configuration loaded\n".to_string())
}

fn list_schedules_from_config(ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            if cfg.schedules.is_empty() {
                return CommandResult::ok("No schedules configured\n".to_string());
            }
            let mut output = String::new();
            for s in &cfg.schedules {
                output.push_str(&format!("Schedule: {} ({})\n", s.name,
                    if s.enabled { "enabled" } else { "disabled" }));
                for run in &s.runs {
                    let level = run.level.map(|l| format!("{l:?}")).unwrap_or_default();
                    output.push_str(&format!("  Run {level} at {:02}:{:02}", run.hour, run.minute));
                    if let Some(pool) = &run.pool {
                        output.push_str(&format!(" pool={pool}"));
                    }
                    if let Some(stor) = &run.storage {
                        output.push_str(&format!(" storage={stor}"));
                    }
                    output.push('\n');
                }
            }
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok("No configuration loaded\n".to_string())
}

fn list_filesets_from_config(ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            if cfg.filesets.is_empty() {
                return CommandResult::ok("No filesets configured\n".to_string());
            }
            let mut output = format!("{:<25} {:<40} {:<40}\n", "Name", "Include", "Exclude");
            output.push_str(&"-".repeat(105));
            output.push('\n');
            for f in &cfg.filesets {
                let inc = if f.include_blocks.is_empty() { "-".to_string() } else { f.include_blocks.join(", ") };
                let exc = if f.exclude_blocks.is_empty() { "-".to_string() } else { f.exclude_blocks.join(", ") };
                output.push_str(&format!("{:<25} {:<40} {:<40}\n", f.name, inc, exc));
            }
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok("No configuration loaded\n".to_string())
}

fn list_from_db_with_args(ast: &CommandAst, url: &str) -> Option<CommandResult> {
    // Handle "list jobid=N"
    let jobid = ast.arguments.iter()
        .find(|a| a.key == "jobid")
        .and_then(|a| a.value.as_deref())
        .and_then(|v| v.parse::<i64>().ok());
    if let Some(id) = jobid {
        let query = format!(
            "SELECT j.jobid, j.name, j.type, j.level, j.jobstatus, \
             j.starttime, j.endtime, j.jobfiles, j.jobbytes, j.joberrors \
             FROM job j WHERE j.jobid = {id}"
        );
        return Some(run_db_query(&query, url));
    }

    // Handle "list job=NAME"
    let jobname = ast.arguments.iter()
        .find(|a| a.key == "job")
        .and_then(|a| a.value.as_deref());
    if let Some(name) = jobname {
        let query = format!(
            "SELECT j.jobid, j.name, j.type, j.level, j.jobstatus, \
             j.starttime, j.endtime, j.jobfiles, j.jobbytes \
             FROM job j WHERE j.name = '{}' ORDER BY j.jobid DESC", name.replace('\'', "''")
        );
        return Some(run_db_query(&query, url));
    }

    None
}

fn run_db_query(query: &str, url: &str) -> CommandResult {
    let rt = match tokio::runtime::Runtime::new() {
        Ok(rt) => rt,
        Err(e) => return CommandResult::error(format!("Runtime error: {e}\n")),
    };
    match rt.block_on(run_query(url, query)) {
        Ok(output) => CommandResult::ok(output),
        Err(e) => CommandResult::error(format!("Catalog error: {e}\n")),
    }
}

fn list_from_db(what: &str, limit: i64, url: &str) -> CommandResult {
    let query = match what {
        "jobs" => format!(
            "SELECT j.jobid, j.name, j.type, j.level, j.jobstatus, \
             j.starttime, j.endtime, j.jobfiles, j.jobbytes \
             FROM job j ORDER BY j.jobid DESC LIMIT {limit}"
        ),
        "volumes" | "media" => format!(
            "SELECT mediaid, volumename, volstatus, volbytes, volfiles, mediatype \
             FROM media ORDER BY mediaid DESC LIMIT {limit}"
        ),
        "clients" => "SELECT clientid, name, uname FROM client ORDER BY name".to_string(),
        "pools" => "SELECT poolid, name, pooltype, numvols, maxvols FROM pool ORDER BY name".to_string(),
        "files" => format!(
            "SELECT f.fileid, f.jobid, p.path || f.name AS filename \
             FROM file f JOIN path p ON f.pathid = p.pathid \
             ORDER BY f.fileid DESC LIMIT {limit}"
        ),
        "storages" | "storage" => "SELECT storageid, name, autochanger FROM storage ORDER BY name".to_string(),
        "schedules" | "schedule" => "SELECT scheduleid, name, enabled FROM schedule ORDER BY name".to_string(),
        "jobtotals" => "SELECT COUNT(*) AS jobs, COALESCE(SUM(jobfiles),0) AS files, COALESCE(SUM(jobbytes),0) AS bytes FROM job".to_string(),
        other => return CommandResult::ok(format!("list {other}: unsupported catalog query\n")),
    };

    run_db_query(&query, url)
}

async fn run_query(url: &str, query: &str) -> Result<String, String> {
    let pool = sqlx::postgres::PgPoolOptions::new()
        .max_connections(1)
        .connect(url)
        .await
        .map_err(|e| format!("DB connect: {e}"))?;

    let rows = sqlx::query(query)
        .fetch_all(&pool)
        .await
        .map_err(|e| format!("Query: {e}"))?;

    use sqlx::{Row, Column};
    let mut output = String::new();

    if rows.is_empty() {
        output.push_str("No results.\n");
        return Ok(output);
    }

    // Header
    let columns = rows[0].columns();
    for col in columns {
        output.push_str(&format!("{:<20}", col.name()));
    }
    output.push('\n');
    output.push_str(&"-".repeat(columns.len() * 20));
    output.push('\n');

    // Data
    for row in &rows {
        for col in columns {
            let val: String = row.try_get::<String, _>(col.ordinal())
                .or_else(|_| row.try_get::<i64, _>(col.ordinal()).map(|v| v.to_string()))
                .or_else(|_| row.try_get::<i32, _>(col.ordinal()).map(|v| v.to_string()))
                .or_else(|_| row.try_get::<bool, _>(col.ordinal()).map(|v| v.to_string()))
                .unwrap_or_else(|_| "NULL".to_string());
            output.push_str(&format!("{:<20}", val));
        }
        output.push('\n');
    }

    Ok(output)
}

fn format_bytes(bytes: u64) -> String {
    if bytes == 0 {
        "0".to_string()
    } else if bytes >= 1_000_000_000 {
        format!("{:.1}G", bytes as f64 / 1_000_000_000.0)
    } else if bytes >= 1_000_000 {
        format!("{:.1}M", bytes as f64 / 1_000_000.0)
    } else if bytes >= 1_000 {
        format!("{:.1}K", bytes as f64 / 1_000.0)
    } else {
        format!("{bytes}")
    }
}
