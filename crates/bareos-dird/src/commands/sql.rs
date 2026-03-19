//! SQL commands: .sql, sqlquery

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_dot_sql(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let query = ast.arguments.iter()
        .find(|a| a.key == "query")
        .and_then(|a| a.value.as_deref());

    match query {
        Some(sql) => {
            // TODO(R7-full): Execute via CatalogDb::raw_query
            CommandResult::ok(format!(".sql: query accepted (pending catalog)\n"))
        }
        None => CommandResult::ok(".sql: specify query=\"...\"\n".to_string()),
    }
}

pub fn cmd_sql_query(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    cmd_dot_sql(ast, ctx)
}
