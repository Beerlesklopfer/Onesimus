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
    cmd_status(ast, ctx)
}

fn status_director(ctx: &ExecContext) -> CommandResult {
    let version = env!("CARGO_PKG_VERSION");
    let now = chrono::Local::now();

    let mut output = format!(
        "bareos-dir Version: {version}\n\
         Daemon started {}\n\
         \n",
        now.format("%d-%b-%Y %H:%M:%S")
    );

    // Config info
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            output.push_str(&format!(
                "Director: {}\n\
                 Clients: {}\n\
                 Jobs: {}\n\
                 Storages: {}\n\
                 Pools: {}\n\
                 Schedules: {}\n\
                 Catalogs: {}\n\
                 \n",
                cfg.director.name,
                cfg.clients.len(),
                cfg.jobs.len(),
                cfg.storages.len(),
                cfg.pools.len(),
                cfg.schedules.len(),
                cfg.catalogs.len(),
            ));
        }
    }

    // Session info
    if let Some(smgr) = &ctx.session_mgr {
        let sessions = smgr.active_session_count();
        let fds = smgr.fd_connection_count();
        let sds = smgr.sd_connection_count();

        output.push_str(&format!(
            "Console Connections: {sessions}\n\
             FD Connections: {fds}\n\
             SD Connections: {sds}\n\
             \n"
        ));

        // List connected clients
        let fd_list = smgr.get_fd_connections();
        if !fd_list.is_empty() {
            output.push_str("Connected File Daemons:\n");
            for fd in &fd_list {
                output.push_str(&format!(
                    "  {} (proto={}, alive={})\n",
                    fd.client_name, fd.protocol_version, fd.alive
                ));
            }
            output.push('\n');
        }

        // List sessions
        let session_list = smgr.get_session_names();
        if !session_list.is_empty() {
            output.push_str("Console Sessions:\n");
            for (id, name) in &session_list {
                output.push_str(&format!("  #{id}: {name}\n"));
            }
            output.push('\n');
        }
    } else {
        output.push_str(
            "Console Connections: 0\n\
             FD Connections: 0\n\
             SD Connections: 0\n\n"
        );
    }

    output.push_str("Scheduled Jobs:\n");
    output.push_str("  No scheduled jobs.\n\n");
    output.push_str("Running Jobs:\n");
    output.push_str("  No running jobs.\n");
    output.push_str("====\n");

    CommandResult::ok(output)
}

fn status_client(name: &str, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok(format!("Client {name}: status query pending\n"))
}

fn status_storage(name: &str, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok(format!("Storage {name}: status query pending\n"))
}
