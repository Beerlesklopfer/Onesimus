//! Status commands: status, .status

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_status(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let target = ast.arguments.first()
        .and_then(|a| a.value.as_deref())
        .or_else(|| ast.arguments.first().map(|a| a.key.as_str()))
        .unwrap_or("dir");

    match target.to_lowercase().as_str() {
        "dir" | "director" => status_director(ctx),
        "client" => {
            let name = ast.arguments.iter()
                .find(|a| a.key == "client")
                .and_then(|a| a.value.as_deref())
                .unwrap_or("*");
            status_client(name, ctx)
        }
        "storage" => {
            let name = ast.arguments.iter()
                .find(|a| a.key == "storage")
                .and_then(|a| a.value.as_deref())
                .unwrap_or("*");
            status_storage(name, ctx)
        }
        "all" => {
            let mut out = status_director(ctx).text_output;
            out.push_str("\n");
            out.push_str(&status_client("*", ctx).text_output);
            out.push_str("\n");
            out.push_str(&status_storage("*", ctx).text_output);
            CommandResult::ok(out)
        }
        other => CommandResult::ok(format!("status {other}: not yet implemented\n")),
    }
}

pub fn cmd_dot_status(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    // Machine-readable status
    cmd_status(ast, ctx)
}

fn status_director(_ctx: &ExecContext) -> CommandResult {
    let version = env!("CARGO_PKG_VERSION");
    let now = chrono::Local::now();
    let output = format!(
        "baReos Director Version: {version}\n\
         Status: running\n\
         Time: {}\n\
         Scheduled Jobs: 0\n\
         Running Jobs: 0\n\
         Console Connections: 0\n\
         FD Connections: 0\n\
         SD Connections: 0\n",
        now.format("%Y-%m-%d %H:%M:%S")
    );
    // TODO(R7-full): Pull real data from SessionManager, Scheduler
    CommandResult::ok(output)
}

fn status_client(name: &str, _ctx: &ExecContext) -> CommandResult {
    // TODO(R7-full): Connect to FD and get status
    CommandResult::ok(format!("Client {name}: status query pending\n"))
}

fn status_storage(name: &str, _ctx: &ExecContext) -> CommandResult {
    // TODO(R7-full): Connect to SD and get status
    CommandResult::ok(format!("Storage {name}: status query pending\n"))
}
