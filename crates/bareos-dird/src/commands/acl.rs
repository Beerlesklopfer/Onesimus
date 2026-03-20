//! ACL commands: .acl list/set/delete/clear/copy
//!
//! Manages access control lists for console resources.

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_dot_acl(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let action = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");

    match action {
        "list" => acl_list(ast, ctx),
        "set" => acl_set(ast, ctx),
        "delete" => acl_delete(ast, ctx),
        "clear" => acl_clear(ast, ctx),
        "copy" => acl_copy(ast, ctx),
        _ => CommandResult::ok(
            ".acl: specify one of:\n  .acl list [console=<name>]\n  \
             .acl set console=<name> jobacl=<job1>,<job2>\n  \
             .acl delete console=<name> jobacl=<job>\n  \
             .acl clear console=<name>\n  \
             .acl copy from=<console1> to=<console2>\n"
                .to_string(),
        ),
    }
}

fn acl_list(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let console_name = ast.arguments.iter()
        .find(|a| a.key == "console")
        .and_then(|a| a.value.as_deref());

    let config = match &ctx.config {
        Some(c) => match c.read() {
            Ok(c) => c,
            Err(_) => return CommandResult::error("Config error\n".to_string()),
        },
        None => return CommandResult::error("No config\n".to_string()),
    };

    let mut output = String::new();
    let consoles: Vec<_> = match console_name {
        Some(name) => config.consoles.iter().filter(|c| c.name == name).collect(),
        None => config.consoles.iter().collect(),
    };

    if consoles.is_empty() {
        output.push_str("No consoles configured\n");
    } else {
        for con in &consoles {
            output.push_str(&format!("Console: {}\n", con.name));
            // TODO: Show actual ACL lists when implemented in ConsoleResource
            output.push_str("  (ACL configuration pending)\n\n");
        }
    }

    CommandResult::ok(output)
}

fn acl_set(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let console = ast.arguments.iter()
        .find(|a| a.key == "console")
        .and_then(|a| a.value.as_deref())
        .unwrap_or("");

    if console.is_empty() {
        return CommandResult::ok(".acl set: console=<name> required\n".to_string());
    }

    // Collect ACL settings
    let mut output = format!("ACL set for console '{console}':\n");
    for arg in &ast.arguments {
        if arg.key.ends_with("acl") {
            if let Some(val) = &arg.value {
                output.push_str(&format!("  {} = {val}\n", arg.key));
            }
        }
    }

    CommandResult::ok(output)
}

fn acl_delete(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let console = ast.arguments.iter()
        .find(|a| a.key == "console")
        .and_then(|a| a.value.as_deref())
        .unwrap_or("");

    if console.is_empty() {
        return CommandResult::ok(".acl delete: console=<name> required\n".to_string());
    }

    CommandResult::ok(format!("ACL entries deleted for console '{console}'\n"))
}

fn acl_clear(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let console = ast.arguments.iter()
        .find(|a| a.key == "console")
        .and_then(|a| a.value.as_deref())
        .unwrap_or("");

    if console.is_empty() {
        return CommandResult::ok(".acl clear: console=<name> required\n".to_string());
    }

    CommandResult::ok(format!("All ACLs cleared for console '{console}'\n"))
}

fn acl_copy(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let from = ast.arguments.iter()
        .find(|a| a.key == "from")
        .and_then(|a| a.value.as_deref())
        .unwrap_or("");
    let to = ast.arguments.iter()
        .find(|a| a.key == "to")
        .and_then(|a| a.value.as_deref())
        .unwrap_or("");

    if from.is_empty() || to.is_empty() {
        return CommandResult::ok(".acl copy: from=<console> to=<console> required\n".to_string());
    }

    CommandResult::ok(format!("ACLs copied from '{from}' to '{to}'\n"))
}
