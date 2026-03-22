//! Command dispatcher — routes CommandAST to command handlers.

use std::sync::Arc;

use crossbeam_channel::Receiver;

use crate::command_ast::CommandAst;
use crate::command_queue::CommandResult;
use crate::commands;
use crate::director_config::SharedConfig;
use crate::message_bus::BusMessage;
use crate::session_manager::SessionManager;

/// API output mode (set by `.api` command, per session).
/// 0 = human-readable, 1 = machine (tab-separated), 2 = JSON
#[derive(Debug, Clone, Copy, PartialEq, Eq, Default)]
pub enum ApiMode {
    #[default]
    Human = 0,
    Api = 1,
    Json = 2,
}

/// Execution context passed to each command handler.
pub struct ExecContext {
    pub session_id: u64,
    pub config: Option<SharedConfig>,
    pub session_mgr: Option<Arc<SessionManager>>,
    pub catalog_url: Option<String>,
    pub api_mode: ApiMode,
    /// Bus receiver for this session — drains pending messages.
    pub bus_rx: Option<Receiver<BusMessage>>,
}

/// Dispatch a command to the appropriate handler.
pub fn dispatch(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    match ast.command_name.to_lowercase().as_str() {
        // === Basic commands ===
        "version" => commands::basic::cmd_version(ast, ctx),
        "time" => commands::basic::cmd_time(ast, ctx),
        "quit" | "exit" => commands::basic::cmd_quit(ast, ctx),
        "help" => commands::basic::cmd_help(ast, ctx),

        // === Status ===
        "status" => commands::status::cmd_status(ast, ctx),

        // === Job control ===
        "run" => commands::job::cmd_run(ast, ctx),
        "cancel" => commands::job::cmd_cancel(ast, ctx),
        "enable" => commands::job::cmd_enable(ast, ctx),
        "disable" => commands::job::cmd_disable(ast, ctx),

        // === List/catalog ===
        "list" => commands::list::cmd_list(ast, ctx),
        "llist" => commands::list::cmd_llist(ast, ctx),

        // === Admin ===
        "label" => commands::admin::cmd_label(ast, ctx),
        "mount" => commands::admin::cmd_mount(ast, ctx),
        "unmount" | "umount" => commands::admin::cmd_unmount(ast, ctx),
        "release" => commands::admin::cmd_release(ast, ctx),
        "update" => commands::admin::cmd_update(ast, ctx),
        "restore" => commands::admin::cmd_restore(ast, ctx),
        "estimate" => commands::admin::cmd_estimate(ast, ctx),
        "prune" => commands::admin::cmd_prune(ast, ctx),
        "purge" => commands::admin::cmd_purge(ast, ctx),
        "truncate" => commands::admin::cmd_truncate(ast, ctx),
        "reload" => commands::admin::cmd_reload(ast, ctx),

        // === Configure ===
        "configure" => commands::configure::cmd_configure(ast, ctx),

        // === SQL (regular) ===
        "sqlquery" | "query" => commands::sql::cmd_sql_query(ast, ctx),

        // === Session/interactive ===
        "autodisplay" => commands::extra::cmd_autodisplay(ast, ctx),
        "automount" => commands::extra::cmd_automount(ast, ctx),
        "gui" => commands::extra::cmd_gui(ast, ctx),
        "messages" => commands::extra::cmd_messages(ast, ctx),
        "memory" => commands::extra::cmd_memory(ast, ctx),
        "show" => commands::extra::cmd_show(ast, ctx),
        "trace" => commands::extra::cmd_trace(ast, ctx),
        "var" => commands::extra::cmd_var(ast, ctx),
        "wait" => commands::extra::cmd_wait(ast, ctx),
        "whoami" => commands::extra::cmd_whoami(ast, ctx),
        "use" => commands::extra::cmd_use(ast, ctx),
        "setdebug" => commands::extra::cmd_setdebug(ast, ctx),
        "setbandwidth" => commands::extra::cmd_setbandwidth(ast, ctx),
        "setip" => commands::extra::cmd_setip(ast, ctx),
        "setdevice" => commands::extra::cmd_setdevice(ast, ctx),
        "resolve" => commands::extra::cmd_resolve(ast, ctx),
        "rerun" => commands::extra::cmd_rerun(ast, ctx),
        "relabel" => commands::extra::cmd_relabel(ast, ctx),
        "delete" => commands::extra::cmd_delete(ast, ctx),
        "create" => commands::extra::cmd_create(ast, ctx),
        "add" => commands::extra::cmd_add(ast, ctx),
        "import" => commands::extra::cmd_import(ast, ctx),
        "export" => commands::extra::cmd_export(ast, ctx),
        "move" => commands::extra::cmd_move(ast, ctx),

        // =================================================================
        // DOT COMMANDS — all 38+ from C++ ua_dotcmds.cc
        // =================================================================
        "." => commands::dot::cmd_dot_noop(ast, ctx),
        ".help" => commands::basic::cmd_dot_help(ast, ctx),
        ".quit" => commands::basic::cmd_quit(ast, ctx),
        ".status" => commands::status::cmd_dot_status(ast, ctx),

        // Resource listing
        ".clients" => commands::dot::cmd_dot_clients(ast, ctx),
        ".jobs" => commands::dot::cmd_dot_jobs(ast, ctx),
        ".pools" => commands::dot::cmd_dot_pools(ast, ctx),
        ".storage" | ".storages" => commands::dot::cmd_dot_storage(ast, ctx),
        ".filesets" => commands::dot::cmd_dot_filesets(ast, ctx),
        ".schedule" => commands::dot::cmd_dot_schedule(ast, ctx),
        ".catalogs" => commands::dot::cmd_dot_catalogs(ast, ctx),
        ".consoles" => commands::dot::cmd_dot_consoles(ast, ctx),
        ".jobdefs" => commands::dot::cmd_dot_jobdefs(ast, ctx),
        ".profiles" => commands::dot::cmd_dot_profiles(ast, ctx),
        ".locations" => commands::dot::cmd_dot_locations(ast, ctx),
        ".media" => commands::dot::cmd_dot_media(ast, ctx),
        ".mediatypes" => commands::dot::cmd_dot_mediatypes(ast, ctx),
        ".messages" => commands::dot::cmd_dot_messages(ast, ctx),
        ".msgs" => commands::dot::cmd_dot_msgs(ast, ctx),
        ".users" => commands::dot::cmd_dot_users(ast, ctx),

        // Query commands
        ".defaults" => commands::dot::cmd_dot_defaults(ast, ctx),
        ".jobstatus" => commands::dot::cmd_dot_jobstatus(ast, ctx),
        ".authorized" => commands::dot::cmd_dot_authorized(ast, ctx),
        ".levels" => commands::dot::cmd_dot_levels(ast, ctx),
        ".types" => commands::dot::cmd_dot_types(ast, ctx),
        ".volstatus" => commands::dot::cmd_dot_volstatus(ast, ctx),
        ".actiononpurge" => commands::dot::cmd_dot_actiononpurge(ast, ctx),

        // API/protocol switching
        ".api" => commands::dot::cmd_dot_api(ast, ctx),
        ".grpc" => commands::dot::cmd_dot_grpc(ast, ctx),

        // SQL dot-commands
        ".sql" => commands::sql::cmd_dot_sql(ast, ctx),
        ".sqltables" => commands::dot::cmd_dot_sqltables(ast, ctx),
        ".sqlschema" => commands::dot::cmd_dot_sqlschema(ast, ctx),
        ".sqlcount" => commands::dot::cmd_dot_sqlcount(ast, ctx),

        // ACL management
        ".acl" => commands::acl::cmd_dot_acl(ast, ctx),
        ".profile" => commands::acl::cmd_dot_acl(ast, ctx), // same handler

        // Catalog schema management
        ".catalog" => commands::catalog::cmd_dot_catalog(ast, ctx),

        // BVFS (Virtual File System for restore browsing)
        ".bvfs_update" => commands::dot::cmd_bvfs_update(ast, ctx),
        ".bvfs_cleanup" => commands::dot::cmd_bvfs_cleanup(ast, ctx),
        ".bvfs_clear_cache" => commands::dot::cmd_bvfs_clear_cache(ast, ctx),
        ".bvfs_get_jobids" => commands::dot::cmd_bvfs_get_jobids(ast, ctx),
        ".bvfs_lsdirs" => commands::dot::cmd_bvfs_lsdirs(ast, ctx),
        ".bvfs_lsfiles" => commands::dot::cmd_bvfs_lsfiles(ast, ctx),
        ".bvfs_versions" => commands::dot::cmd_bvfs_versions(ast, ctx),
        ".bvfs_restore" => commands::dot::cmd_bvfs_restore(ast, ctx),

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
        ExecContext { session_id: 1, config: None, session_mgr: None, catalog_url: None, api_mode: ApiMode::Human, bus_rx: None }
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
        assert!(result.text_output.contains(".help"));
    }

    #[test]
    fn dispatch_dot_help() {
        let result = dispatch(&make_ast(".help"), &make_ctx());
        assert!(result.success);
        // .help must list all dot-commands
        assert!(result.text_output.contains(".clients"));
        assert!(result.text_output.contains(".jobs"));
        assert!(result.text_output.contains(".bvfs_update"));
        assert!(result.text_output.contains(".authorized"));
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
        for cmd in &[
            ".clients", ".jobs", ".pools", ".storage", ".storages",
            ".filesets", ".defaults", ".jobstatus", ".levels", ".types",
            ".volstatus", ".actiononpurge", ".", ".authorized",
            ".catalogs", ".consoles", ".jobdefs", ".profiles",
            ".locations", ".media", ".mediatypes", ".messages",
            ".msgs", ".users", ".schedule", ".api", ".grpc",
        ] {
            let result = dispatch(&make_ast(cmd), &make_ctx());
            assert!(result.success, "command {cmd} should succeed");
        }
    }

    #[test]
    fn dispatch_bvfs_commands() {
        for cmd in &[
            ".bvfs_update", ".bvfs_cleanup", ".bvfs_clear_cache",
            ".bvfs_get_jobids", ".bvfs_lsdirs", ".bvfs_lsfiles",
            ".bvfs_versions", ".bvfs_restore",
        ] {
            let result = dispatch(&make_ast(cmd), &make_ctx());
            assert!(result.success, "command {cmd} should succeed");
        }
    }

    #[test]
    fn dispatch_quit_variants() {
        for cmd in &["quit", "exit", ".quit"] {
            let result = dispatch(&make_ast(cmd), &make_ctx());
            assert!(result.success, "command {cmd} should succeed");
        }
    }

    #[test]
    fn dispatch_case_insensitive() {
        let result = dispatch(&make_ast("VERSION"), &make_ctx());
        assert!(result.success);
        let result = dispatch(&make_ast("Status"), &make_ctx());
        assert!(result.success);
    }
}
