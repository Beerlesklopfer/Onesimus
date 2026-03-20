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

    // If we have a catalog URL, query the database
    if let Some(url) = &ctx.catalog_url {
        return list_from_db(what, limit, url);
    }

    // Otherwise show what we know from config
    match what {
        "jobs" => list_jobs_from_config(ctx),
        "clients" => list_clients_from_config(ctx),
        "pools" => list_pools_from_config(ctx),
        "volumes" | "media" => CommandResult::ok("No catalog connection — cannot list volumes\n".to_string()),
        "files" => CommandResult::ok("No catalog connection — cannot list files\n".to_string()),
        other => CommandResult::ok(format!("list {other}: not implemented\n")),
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

fn list_from_db(what: &str, limit: i64, url: &str) -> CommandResult {
    // TODO: Async DB query via sqlx
    CommandResult::ok(format!("list {what} limit={limit}: catalog query pending (url: {url})\n"))
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
