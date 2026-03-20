//! Admin commands: label, mount, unmount, release, update, restore,
//! estimate, prune, purge, truncate, reload

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_label(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let vol = get_arg(ast, "volume").unwrap_or("?");
    let stor = get_arg(ast, "storage").unwrap_or("?");
    CommandResult::ok(format!("label: volume={vol} storage={stor} (pending SD connection)\n"))
}

pub fn cmd_mount(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let stor = get_arg(ast, "storage").unwrap_or("?");
    CommandResult::ok(format!("mount: storage={stor} (pending SD connection)\n"))
}

pub fn cmd_unmount(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let stor = get_arg(ast, "storage").unwrap_or("?");
    CommandResult::ok(format!("unmount: storage={stor} (pending SD connection)\n"))
}

pub fn cmd_release(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let stor = get_arg(ast, "storage").unwrap_or("?");
    CommandResult::ok(format!("release: storage={stor} (pending SD connection)\n"))
}

pub fn cmd_update(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let what = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("?");
    CommandResult::ok(format!("update {what}: pending catalog update\n"))
}

pub fn cmd_restore(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let where_path = get_arg(ast, "where").unwrap_or("/tmp/bareos-restore");
    CommandResult::ok(format!("restore: where={where_path} (pending file selection)\n"))
}

pub fn cmd_estimate(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let job = get_arg(ast, "job").unwrap_or("?");
    CommandResult::ok(format!("estimate: job={job} (pending FD connection)\n"))
}

pub fn cmd_prune(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let what = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("?");
    CommandResult::ok(format!("prune {what}: pending catalog operation\n"))
}

pub fn cmd_purge(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let what = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("?");
    CommandResult::ok(format!("purge {what}: pending catalog operation\n"))
}

pub fn cmd_truncate(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let vol = get_arg(ast, "volume").unwrap_or("?");
    CommandResult::ok(format!("truncate: volume={vol} (pending SD connection)\n"))
}

pub fn cmd_reload(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            CommandResult::ok(format!(
                "Configuration reloaded: Director '{}', {} clients, {} jobs, {} storages\n",
                cfg.director.name, cfg.clients.len(), cfg.jobs.len(), cfg.storages.len()
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
