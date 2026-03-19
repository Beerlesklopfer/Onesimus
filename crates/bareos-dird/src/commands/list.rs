//! List commands: list, llist

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_list(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let what = ast.arguments.first()
        .map(|a| a.key.as_str())
        .unwrap_or("jobs");

    // TODO(R7-full): Query from CatalogDb
    match what {
        "jobs" => CommandResult::ok("No jobs found\n".to_string()),
        "volumes" | "media" => CommandResult::ok("No volumes found\n".to_string()),
        "pools" => CommandResult::ok("No pools found\n".to_string()),
        "clients" => CommandResult::ok("No clients found\n".to_string()),
        "files" => CommandResult::ok("No files found\n".to_string()),
        other => CommandResult::ok(format!("list {other}: not implemented\n")),
    }
}

pub fn cmd_llist(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    // Long list — same as list but with all fields
    cmd_list(ast, ctx)
}
