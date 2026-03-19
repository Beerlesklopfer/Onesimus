//! Command dispatcher — routes CommandAST to command handlers.

use crate::command_ast::CommandAst;
use crate::command_queue::CommandResult;
use crate::commands;

/// Execution context passed to each command handler.
pub struct ExecContext {
    pub session_id: u64,
    // TODO(R7-full): Add references to SessionManager, CatalogDb, Config, etc.
}

/// Dispatch a command to the appropriate handler.
pub fn dispatch(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    match ast.command_name.to_lowercase().as_str() {
        // Basic commands
        "version" => commands::basic::cmd_version(ast, ctx),
        "time" => commands::basic::cmd_time(ast, ctx),
        "quit" | "exit" | ".quit" => commands::basic::cmd_quit(ast, ctx),
        "help" => commands::basic::cmd_help(ast, ctx),
        ".help" => commands::basic::cmd_dot_help(ast, ctx),

        // Status commands
        "status" => commands::status::cmd_status(ast, ctx),
        ".status" => commands::status::cmd_dot_status(ast, ctx),

        // Dot commands (machine-readable queries)
        ".clients" => commands::dot::cmd_dot_clients(ast, ctx),
        ".jobs" => commands::dot::cmd_dot_jobs(ast, ctx),
        ".pools" => commands::dot::cmd_dot_pools(ast, ctx),
        ".storage" => commands::dot::cmd_dot_storage(ast, ctx),
        ".filesets" => commands::dot::cmd_dot_filesets(ast, ctx),
        ".defaults" => commands::dot::cmd_dot_defaults(ast, ctx),
        ".jobstatus" => commands::dot::cmd_dot_jobstatus(ast, ctx),

        // Job control
        "run" => commands::job::cmd_run(ast, ctx),
        "cancel" => commands::job::cmd_cancel(ast, ctx),
        "enable" => commands::job::cmd_enable(ast, ctx),
        "disable" => commands::job::cmd_disable(ast, ctx),

        // List/catalog commands
        "list" => commands::list::cmd_list(ast, ctx),
        "llist" => commands::list::cmd_llist(ast, ctx),

        // Admin commands
        "label" => commands::admin::cmd_label(ast, ctx),
        "mount" => commands::admin::cmd_mount(ast, ctx),
        "unmount" => commands::admin::cmd_unmount(ast, ctx),
        "release" => commands::admin::cmd_release(ast, ctx),
        "update" => commands::admin::cmd_update(ast, ctx),
        "restore" => commands::admin::cmd_restore(ast, ctx),
        "estimate" => commands::admin::cmd_estimate(ast, ctx),
        "prune" => commands::admin::cmd_prune(ast, ctx),
        "purge" => commands::admin::cmd_purge(ast, ctx),
        "truncate" => commands::admin::cmd_truncate(ast, ctx),
        "reload" => commands::admin::cmd_reload(ast, ctx),

        // Configure commands
        "configure" => commands::configure::cmd_configure(ast, ctx),

        // SQL commands
        ".sql" => commands::sql::cmd_dot_sql(ast, ctx),
        "sqlquery" | "query" => commands::sql::cmd_sql_query(ast, ctx),

        // ACL commands
        ".acl" => commands::acl::cmd_dot_acl(ast, ctx),

        _ => CommandResult::error(format!("Unknown command: {}", ast.command_name)),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::command_ast::{CommandSource, ResponseFormat, Argument};

    fn make_ast(name: &str) -> CommandAst {
        CommandAst {
            command_name: name.to_string(),
            arguments: Vec::new(),
            session_id: String::new(),
            source: CommandSource::Cli,
            response_format: ResponseFormat::Default,
        }
    }

    fn make_ctx() -> ExecContext {
        ExecContext { session_id: 1 }
    }

    #[test]
    fn dispatch_version() {
        let result = dispatch(&make_ast("version"), &make_ctx());
        assert!(result.success);
        assert!(result.text_output.contains("baReos"));
    }

    #[test]
    fn dispatch_time() {
        let result = dispatch(&make_ast("time"), &make_ctx());
        assert!(result.success);
    }

    #[test]
    fn dispatch_help() {
        let result = dispatch(&make_ast("help"), &make_ctx());
        assert!(result.success);
        assert!(result.text_output.contains("Available commands"));
    }

    #[test]
    fn dispatch_unknown() {
        let result = dispatch(&make_ast("nonexistent"), &make_ctx());
        assert!(!result.success);
        assert!(result.error_message.contains("Unknown command"));
    }

    #[test]
    fn dispatch_status() {
        let result = dispatch(&make_ast("status"), &make_ctx());
        assert!(result.success);
    }

    #[test]
    fn dispatch_dot_commands() {
        for cmd in &[".clients", ".jobs", ".pools", ".storage", ".filesets", ".defaults", ".jobstatus"] {
            let result = dispatch(&make_ast(cmd), &make_ctx());
            assert!(result.success, "command {cmd} should succeed");
        }
    }

    #[test]
    fn dispatch_quit() {
        let result = dispatch(&make_ast("quit"), &make_ctx());
        assert!(result.success);
    }

    #[test]
    fn dispatch_case_insensitive() {
        let result = dispatch(&make_ast("VERSION"), &make_ctx());
        assert!(result.success);
        let result = dispatch(&make_ast("Status"), &make_ctx());
        assert!(result.success);
    }
}
