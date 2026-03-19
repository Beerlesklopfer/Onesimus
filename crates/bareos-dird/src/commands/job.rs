//! Job control commands: run, cancel, enable, disable

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_run(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let job_name = ast.arguments.iter()
        .find(|a| a.key == "job")
        .and_then(|a| a.value.as_deref());

    match job_name {
        Some(name) => {
            // TODO(R7-full): Submit job to scheduler
            CommandResult::ok(format!("Job '{name}' queued for execution\n"))
        }
        None => CommandResult::ok("run: specify job=<name>\n".to_string()),
    }
}

pub fn cmd_cancel(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let job_id = ast.arguments.iter()
        .find(|a| a.key == "jobid")
        .and_then(|a| a.value.as_deref())
        .and_then(|v| v.parse::<u32>().ok());

    match job_id {
        Some(id) => {
            // TODO(R7-full): Signal job worker process
            CommandResult::ok(format!("Job {id} marked for cancellation\n"))
        }
        None => CommandResult::ok("cancel: specify jobid=<id>\n".to_string()),
    }
}

pub fn cmd_enable(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let target = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");
    let name = ast.arguments.first().and_then(|a| a.value.as_deref()).unwrap_or("");

    match target {
        "client" => CommandResult::ok(format!("Client '{name}' enabled\n")),
        "job" => CommandResult::ok(format!("Job '{name}' enabled\n")),
        "schedule" => CommandResult::ok(format!("Schedule '{name}' enabled\n")),
        "storage" => CommandResult::ok(format!("Storage '{name}' enabled\n")),
        _ => CommandResult::ok("enable: specify client=, job=, schedule=, or storage=\n".to_string()),
    }
}

pub fn cmd_disable(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let target = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");
    let name = ast.arguments.first().and_then(|a| a.value.as_deref()).unwrap_or("");

    match target {
        "client" => CommandResult::ok(format!("Client '{name}' disabled\n")),
        "job" => CommandResult::ok(format!("Job '{name}' disabled\n")),
        "schedule" => CommandResult::ok(format!("Schedule '{name}' disabled\n")),
        "storage" => CommandResult::ok(format!("Storage '{name}' disabled\n")),
        _ => CommandResult::ok("disable: specify client=, job=, schedule=, or storage=\n".to_string()),
    }
}
