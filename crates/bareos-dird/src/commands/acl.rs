//! ACL commands: .acl list/set/delete/clear/copy

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_dot_acl(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let action = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("list");

    match action {
        "list" => CommandResult::ok(".acl list: pending catalog query\n".to_string()),
        "set" => CommandResult::ok(".acl set: pending catalog update\n".to_string()),
        "delete" => CommandResult::ok(".acl delete: pending catalog update\n".to_string()),
        "clear" => CommandResult::ok(".acl clear: pending catalog update\n".to_string()),
        "copy" => CommandResult::ok(".acl copy: pending catalog update\n".to_string()),
        _ => CommandResult::ok(".acl: specify list, set, delete, clear, or copy\n".to_string()),
    }
}
