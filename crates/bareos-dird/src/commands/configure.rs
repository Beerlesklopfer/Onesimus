//! Configure commands: configure add/delete/update

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_configure(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let action = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("?");

    match action {
        "add" => {
            let resource_type = ast.arguments.get(1).map(|a| a.key.as_str()).unwrap_or("?");
            CommandResult::ok(format!("configure add {resource_type}: pending\n"))
        }
        "delete" => {
            let resource_type = ast.arguments.get(1).map(|a| a.key.as_str()).unwrap_or("?");
            CommandResult::ok(format!("configure delete {resource_type}: pending\n"))
        }
        "update" => {
            let resource_type = ast.arguments.get(1).map(|a| a.key.as_str()).unwrap_or("?");
            CommandResult::ok(format!("configure update {resource_type}: pending\n"))
        }
        _ => CommandResult::ok("configure: specify add, delete, or update\n".to_string()),
    }
}
