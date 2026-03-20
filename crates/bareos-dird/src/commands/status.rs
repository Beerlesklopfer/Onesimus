//! Status commands: status, .status

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_status(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    // "status dir" → bare arg "dir"
    // "status client=bareos-fd" → key="client" value="bareos-fd"
    let target = ast.arguments.first()
        .map(|a| a.key.as_str())
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

fn status_client(name: &str, ctx: &ExecContext) -> CommandResult {
    let config = match &ctx.config {
        Some(c) => c.read().ok(),
        None => return CommandResult::ok(format!("Client {name}: no config loaded\n")),
    };
    let config = match config {
        Some(c) => c,
        None => return CommandResult::ok(format!("Client {name}: config lock error\n")),
    };

    let dir_name = config.director.name.clone();

    // Find all matching clients
    let clients: Vec<_> = if name == "*" {
        config.clients.iter().collect()
    } else {
        config.clients.iter().filter(|c| c.name == name).collect()
    };

    if clients.is_empty() {
        return CommandResult::ok(format!("Client '{name}' not found in configuration\n"));
    }

    let mut output = String::new();
    for client in &clients {
        output.push_str(&format!("Connecting to Client {} at {}:{}\n",
                                  client.name, client.address, client.port));

        let result = crate::fd_protocol::query_fd_status(
            &client.address,
            client.port,
            &client.password,
            &dir_name,
            &client.name,
        );

        if result.success {
            output.push_str(&format!("Connected to Client {}", client.name));
            if !result.version.is_empty() {
                output.push_str(&format!(" Version: {}", result.version));
            }
            output.push('\n');
            output.push_str(&result.status_text);
        } else {
            output.push_str(&format!("Failed: {}\n", result.status_text));
        }
        output.push('\n');
    }

    CommandResult::ok(output)
}

fn status_storage(name: &str, ctx: &ExecContext) -> CommandResult {
    let config = match &ctx.config {
        Some(c) => match c.read() {
            Ok(c) => c,
            Err(_) => return CommandResult::ok(format!("Storage {name}: config error\n")),
        },
        None => return CommandResult::ok(format!("Storage {name}: no config\n")),
    };

    let dir_name = config.director.name.clone();
    let storages: Vec<_> = if name == "*" {
        config.storages.iter().collect()
    } else {
        config.storages.iter().filter(|s| s.name == name).collect()
    };

    if storages.is_empty() {
        return CommandResult::ok(format!("Storage '{name}' not found\n"));
    }

    let mut output = String::new();
    for stor in &storages {
        output.push_str(&format!("Connecting to Storage {} at {}:{}\n",
                                  stor.name, stor.address, stor.port));

        let result = crate::sd_protocol::query_sd_status(
            &stor.address, stor.port, &stor.password, &dir_name, &stor.name,
        );

        if result.success {
            output.push_str(&format!("Connected to Storage {}\n", stor.name));
            output.push_str(&result.status_text);
        } else {
            output.push_str(&format!("Failed: {}\n", result.status_text));
        }
        output.push('\n');
    }

    CommandResult::ok(output)
}
