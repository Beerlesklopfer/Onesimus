//! Basic commands: version, time, quit, help, .help
//!
//! `.help` is the authoritative command reference — it lists ALL commands
//! (regular + dot-commands) in a machine-readable, tab-separated format.
//! `help` lists only regular commands and refers the user to `.help`.

use crate::command_ast::CommandAst;
use crate::command_executor::{ExecContext, ApiMode};
use crate::command_queue::CommandResult;

const VERSION: &str = env!("CARGO_PKG_VERSION");

pub fn cmd_version(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let os = std::fs::read_to_string("/etc/os-release").ok()
        .and_then(|s| s.lines()
            .find(|l| l.starts_with("PRETTY_NAME="))
            .map(|l| l.trim_start_matches("PRETTY_NAME=").trim_matches('"').to_string()))
        .unwrap_or_else(|| "Linux".to_string());
    CommandResult::ok(format!("bareos-dir Version: {VERSION} ({os})\n"))
}

pub fn cmd_time(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let now = chrono::Local::now();
    CommandResult::ok(format!("{}\n", now.format("%Y-%m-%d %H:%M:%S")))
}

pub fn cmd_quit(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("bye\n".to_string())
}

/// `help [command]` — human-readable help for regular commands.
/// Ends with a reference to `.help` for machine-readable format and dot-commands.
pub fn cmd_help(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let topic = ast.arguments.first().map(|a| {
        a.value.as_deref().unwrap_or(a.key.as_str())
    });

    if let Some(cmd) = topic {
        // Search both tables
        let entry = REGULAR_COMMANDS.iter()
            .find(|e| e.name == cmd)
            .or_else(|| DOT_COMMANDS.iter().find(|e| e.name == cmd));

        match entry {
            Some(e) => CommandResult::ok(format!(
                "{}: {}\n  Usage: {}\n  Arguments: {}\n",
                e.name, e.description, e.usage,
                if e.arguments.is_empty() { "none" } else { e.arguments }
            )),
            None => CommandResult::ok(format!("Unknown command: {cmd}\nUse \".help\" for complete command list.\n")),
        }
    } else {
        let mut output = String::from("  \x1b[1mAvailable commands:\x1b[0m\n\n");
        for e in REGULAR_COMMANDS {
            output.push_str(&format!("  \x1b[32m{:<20}\x1b[0m {}\n", e.name, e.description));
        }
        output.push_str(&format!("\n  \x1b[2mUse \".help\" for dot-commands. \"help <cmd>\" for details.\x1b[0m\n"));
        CommandResult::ok(output)
    }
}

/// `.help [all | item=<cmd>]` — dot-command help.
///
/// Output format depends on session API mode:
/// - Mode 0 (human): formatted colored table
/// - Mode 1 (api):   tab-separated (for brconsole tab-completion)
/// - Mode 2 (json):  JSON array
///
/// `all` includes regular commands. Default = dot-commands only.
/// `item=<cmd>` shows detail for one command.
pub fn cmd_dot_help(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let args: Vec<&str> = ast.arguments.iter()
        .map(|a| a.value.as_deref().unwrap_or(a.key.as_str()))
        .collect();

    let all = args.contains(&"all");
    let item = args.iter().find(|a| a.starts_with("item="))
        .or_else(|| args.iter().find(|a| **a != "all" && !a.is_empty()));

    // item=<cmd> — detail view
    if let Some(item) = item {
        let cmd = item.strip_prefix("item=").unwrap_or(item);
        let entry = REGULAR_COMMANDS.iter().chain(DOT_COMMANDS.iter())
            .find(|e| e.name == cmd);
        return match entry {
            Some(e) => match ctx.api_mode {
                ApiMode::Json => CommandResult::ok(format!(
                    "{{\"command\":\"{}\",\"description\":\"{}\",\"usage\":\"{}\",\"arguments\":\"{}\",\"audit\":{}}}\n",
                    e.name, e.description, e.usage, e.arguments, e.audit
                )),
                ApiMode::Api => CommandResult::ok(format!(
                    "{}\t{}\t{}\t{}\t{}\n",
                    e.name, e.description, e.usage, e.arguments, e.audit
                )),
                ApiMode::Human => CommandResult::ok(format!(
                    "  Command:     {}\n  Description: {}\n  Usage:       {}\n  Arguments:   {}\n  Audit:       {}\n",
                    e.name, e.description, e.usage,
                    if e.arguments.is_empty() { "none" } else { e.arguments },
                    if e.audit { "yes" } else { "no" }
                )),
            },
            None => CommandResult::ok(format!("Command '{cmd}' not found.\n")),
        };
    }

    let commands: Vec<&CmdHelp> = if all {
        REGULAR_COMMANDS.iter().chain(DOT_COMMANDS.iter()).collect()
    } else {
        DOT_COMMANDS.iter().collect()
    };

    let mut output = String::new();

    match ctx.api_mode {
        ApiMode::Json => {
            // JSON array
            output.push_str("[\n");
            for (i, e) in commands.iter().enumerate() {
                output.push_str(&format!(
                    "  {{\"command\":\"{}\",\"description\":\"{}\",\"usage\":\"{}\",\"arguments\":\"{}\",\"audit\":{}}}",
                    e.name, e.description, e.usage, e.arguments, e.audit
                ));
                if i + 1 < commands.len() { output.push(','); }
                output.push('\n');
            }
            output.push_str("]\n");
        }
        ApiMode::Api => {
            // Tab-separated (machine-readable, for brconsole/webui)
            for e in &commands {
                output.push_str(&format!(
                    "{}\t{}\t{}\t{}\t{}\n",
                    e.name, e.description, e.usage, e.arguments, e.audit
                ));
            }
        }
        ApiMode::Human => {
            // Colored table
            output.push_str(&format!("  \x1b[1m{:<22} {}\x1b[0m\n", "Command", "Description"));
            output.push_str(&format!("  {:<22} {}\n", "\u{2500}".repeat(20), "\u{2500}".repeat(50)));
            for e in &commands {
                let color = if e.name.starts_with('.') { "\x1b[33m" } else { "\x1b[32m" };
                output.push_str(&format!("  {color}{:<22}\x1b[0m {}\n", e.name, e.description));
            }
            if all {
                output.push_str(&format!("\n  \x1b[2m{} commands total.\x1b[0m\n", commands.len()));
            } else {
                output.push_str(&format!("\n  \x1b[2m{} dot-commands. Use \".help all\" for all commands.\x1b[0m\n", commands.len()));
            }
        }
    }

    CommandResult::ok(output)
}

/// Command help entry — compatible with C++ generated_commands.h format.
struct CmdHelp {
    name: &'static str,
    description: &'static str,
    usage: &'static str,
    arguments: &'static str,
    audit: bool,
}

// ============================================================================
// Regular commands (non-dot)
// ============================================================================
static REGULAR_COMMANDS: &[CmdHelp] = &[
    CmdHelp { name: "add", description: "Add media to a pool", usage: "add [pool=<pool-name>] [storage=<storage-name>] [jobid=<id>]", arguments: "pool,storage,jobid", audit: true },
    CmdHelp { name: "autodisplay", description: "Autodisplay console messages", usage: "autodisplay [on|off]", arguments: "!on,!off", audit: false },
    CmdHelp { name: "automount", description: "Automount after label", usage: "automount [on|off]", arguments: "!on,!off", audit: false },
    CmdHelp { name: "cancel", description: "Cancel a job", usage: "cancel [jobid=<id>] [job=<name>] [ujobid=<unique-id>]", arguments: "jobid,job,ujobid,!all", audit: true },
    CmdHelp { name: "configure", description: "Configure director resources", usage: "configure [add|delete|update|read|export] ...", arguments: "!add,!delete,!update,!read,!export", audit: true },
    CmdHelp { name: "create", description: "Create DB pool from resource", usage: "create [pool=<pool-name>]", arguments: "pool", audit: true },
    CmdHelp { name: "delete", description: "Delete catalog records", usage: "delete [volume=<name>|pool=<name>|jobid=<id>]", arguments: "volume,pool,jobid", audit: true },
    CmdHelp { name: "disable", description: "Disable a job/client/schedule", usage: "disable [job=<name>|client=<name>|schedule=<name>]", arguments: "job,client,schedule", audit: true },
    CmdHelp { name: "enable", description: "Enable a job/client/schedule", usage: "enable [job=<name>|client=<name>|schedule=<name>]", arguments: "job,client,schedule", audit: true },
    CmdHelp { name: "estimate", description: "Estimate backup size", usage: "estimate [job=<name>] [client=<name>] [listing]", arguments: "job,client,listing", audit: false },
    CmdHelp { name: "exit", description: "Terminate bconsole session", usage: "exit", arguments: "", audit: false },
    CmdHelp { name: "export", description: "Export volumes from autochanger", usage: "export storage=<name> srcslots=<slot-sel> [dstslots=<slot-sel>] [scan]", arguments: "storage,srcslots,dstslots,scan", audit: true },
    CmdHelp { name: "gui", description: "Switch to non-interactive gui mode", usage: "gui [on|off]", arguments: "!on,!off", audit: false },
    CmdHelp { name: "help", description: "Print help on a specific command", usage: "help [command]", arguments: "", audit: false },
    CmdHelp { name: "import", description: "Import volumes to autochanger", usage: "import storage=<name> [srcslots=<slot-sel>] [dstslots=<slot-sel>] [scan]", arguments: "storage,srcslots,dstslots,scan", audit: true },
    CmdHelp { name: "label", description: "Label a volume", usage: "label [volume=<name>] [storage=<name>] [pool=<name>] [barcodes] [encrypt]", arguments: "volume,storage,pool,barcodes,encrypt", audit: true },
    CmdHelp { name: "list", description: "List catalog contents", usage: "list [jobs|clients|pools|volumes|media|storages|schedules|jobtotals|files jobid=<id>]", arguments: "!jobs,!clients,!pools,!volumes,!media,!storages,!schedules,!jobtotals,!files,jobid,limit", audit: false },
    CmdHelp { name: "llist", description: "Full or long list like list command", usage: "llist [jobs|clients|pools|volumes|media]", arguments: "!jobs,!clients,!pools,!volumes,!media,jobid,limit", audit: false },
    CmdHelp { name: "memory", description: "Print memory usage", usage: "memory", arguments: "", audit: false },
    CmdHelp { name: "messages", description: "Display pending messages", usage: "messages", arguments: "", audit: false },
    CmdHelp { name: "mount", description: "Mount storage", usage: "mount [storage=<name>] [slot=<num>] [drive=<num>]", arguments: "storage,slot,drive", audit: true },
    CmdHelp { name: "move", description: "Move slots in autochanger", usage: "move [storage=<name>] srcslot=<num> dstslot=<num>", arguments: "storage,srcslot,dstslot", audit: true },
    CmdHelp { name: "prune", description: "Prune expired records from catalog", usage: "prune [files|jobs|volumes] [client=<name>] [pool=<name>] [volume=<name>]", arguments: "!files,!jobs,!volumes,client,pool,volume", audit: true },
    CmdHelp { name: "purge", description: "Purge records from catalog", usage: "purge [files jobid=<id>|jobs client=<name>|volume=<name>]", arguments: "!files,!jobs,!volumes,jobid,client,volume", audit: true },
    CmdHelp { name: "quit", description: "Terminate bconsole session", usage: "quit", arguments: "", audit: false },
    CmdHelp { name: "relabel", description: "Relabel a volume", usage: "relabel [oldvolume=<name>] [volume=<name>] [storage=<name>] [pool=<name>] [encrypt]", arguments: "oldvolume,volume,storage,pool,encrypt", audit: true },
    CmdHelp { name: "release", description: "Release storage", usage: "release [storage=<name>]", arguments: "storage", audit: true },
    CmdHelp { name: "reload", description: "Reload configuration file", usage: "reload", arguments: "", audit: true },
    CmdHelp { name: "rerun", description: "Rerun a job", usage: "rerun jobid=<id> [since_jobid=<id>] [until_jobid=<id>] [days=<days>] [hours=<hours>]", arguments: "jobid,since_jobid,until_jobid,days,hours", audit: true },
    CmdHelp { name: "resolve", description: "Resolve a hostname", usage: "resolve [client=<name>] <hostname>", arguments: "client", audit: false },
    CmdHelp { name: "restore", description: "Restore files", usage: "restore [where=<path>] [client=<name>] [jobid=<id>] [fileset=<name>] [select|all]", arguments: "where,client,jobid,fileset,select,all", audit: true },
    CmdHelp { name: "run", description: "Run a job", usage: "run [job=<name>] [level=<level>] [client=<name>] [storage=<name>] [pool=<name>] [when=<time>] [yes]", arguments: "job,level,client,storage,pool,when,yes", audit: true },
    CmdHelp { name: "setbandwidth", description: "Set bandwidth limit", usage: "setbandwidth limit=<speed> [client=<name>] [jobid=<id>]", arguments: "limit,client,jobid", audit: true },
    CmdHelp { name: "setdebug", description: "Set debug level", usage: "setdebug level=<nn> [trace=0/1] [timestamp=0/1] [client=<name>] [storage=<name>] [dir]", arguments: "level,trace,timestamp,client,storage,dir", audit: true },
    CmdHelp { name: "setdevice", description: "Set device parameters", usage: "setdevice storage=<name> device=<name> autoselect=<bool>", arguments: "storage,device,autoselect", audit: true },
    CmdHelp { name: "setip", description: "Set client address for a client", usage: "setip", arguments: "", audit: false },
    CmdHelp { name: "show", description: "Show resource records", usage: "show [all|director|clients|jobs|storages|pools|schedules|consoles|filesets|catalogs|disabled]", arguments: "!all,!director,!clients,!jobs,!storages,!pools,!schedules,!consoles,!filesets,!catalogs,!disabled", audit: false },
    CmdHelp { name: "sqlquery", description: "Use SQL to query catalog", usage: "sqlquery", arguments: "", audit: true },
    CmdHelp { name: "status", description: "Report status", usage: "status [dir|client=<name>|storage=<name>|scheduler|all]", arguments: "!dir,!scheduler,!all,client,storage", audit: false },
    CmdHelp { name: "trace", description: "Turn on/off trace to file", usage: "trace [on|off]", arguments: "!on,!off", audit: false },
    CmdHelp { name: "truncate", description: "Truncate purged volumes", usage: "truncate [volume=<name>] [storage=<name>] [pool=<name>] [yes]", arguments: "volume,storage,pool,yes", audit: true },
    CmdHelp { name: "unmount", description: "Unmount storage", usage: "unmount [storage=<name>] [drive=<num>]", arguments: "storage,drive", audit: true },
    CmdHelp { name: "update", description: "Update volume, pool, or slots", usage: "update [volume=<name>|pool=<name>|slots storage=<name>]", arguments: "volume,pool,!slots,storage", audit: true },
    CmdHelp { name: "use", description: "Use specific catalog", usage: "use [catalog=<name>]", arguments: "catalog", audit: false },
    CmdHelp { name: "var", description: "Variable expansion", usage: "var <variable-name>", arguments: "", audit: false },
    CmdHelp { name: "version", description: "Print Director version", usage: "version", arguments: "", audit: false },
    CmdHelp { name: "wait", description: "Wait until no jobs are running", usage: "wait [jobid=<id>] [jobuid=<unique-id>]", arguments: "jobid,jobuid", audit: false },
    CmdHelp { name: "whoami", description: "Print the currently logged in user", usage: "whoami", arguments: "", audit: false },
];

// ============================================================================
// Dot commands (machine-readable / API)
// Compatible with C++ ua_dotcmds.cc / generated_commands.h
// ============================================================================
static DOT_COMMANDS: &[CmdHelp] = &[
    CmdHelp { name: ".", description: "No op", usage: ".", arguments: "", audit: false },
    CmdHelp { name: ".actiononpurge", description: "List possible actions on purge", usage: ".actiononpurge", arguments: "", audit: false },
    CmdHelp { name: ".api", description: "Switch between different api modes", usage: ".api [0|1|2|off|on|json] [compact=<yes|no>]", arguments: "compact", audit: false },
    CmdHelp { name: ".authorized", description: "Check for authorization", usage: ".authorized job=<name> | client=<name> | storage=<name> | schedule=<name> | pool=<name> | cmd=<command> | fileset=<name> | catalog=<name>", arguments: "job,client,storage,schedule,pool,cmd,fileset,catalog", audit: false },
    CmdHelp { name: ".bvfs_cleanup", description: "Cleanup BVFS cache for a certain handle", usage: ".bvfs_cleanup path=<handle>", arguments: "path", audit: true },
    CmdHelp { name: ".bvfs_clear_cache", description: "Clear BVFS cache", usage: ".bvfs_clear_cache yes", arguments: "", audit: true },
    CmdHelp { name: ".bvfs_get_jobids", description: "Get jobids required for a restore", usage: ".bvfs_get_jobids jobid=<id> | ujobid=<unique-id> [all]", arguments: "jobid,ujobid,all", audit: true },
    CmdHelp { name: ".bvfs_lsdirs", description: "List directories using BVFS", usage: ".bvfs_lsdirs jobid=<id> path=<path> | pathid=<id> [limit=<n>] [offset=<n>]", arguments: "jobid,path,pathid,limit,offset", audit: true },
    CmdHelp { name: ".bvfs_lsfiles", description: "List files using BVFS", usage: ".bvfs_lsfiles jobid=<id> path=<path> | pathid=<id> [limit=<n>] [offset=<n>]", arguments: "jobid,path,pathid,limit,offset", audit: true },
    CmdHelp { name: ".bvfs_restore", description: "Mark BVFS files/directories for restore", usage: ".bvfs_restore path=<handle> jobid=<id> [fileid=<id>] [dirid=<id>] [hardlink=<id>]", arguments: "path,jobid,fileid,dirid,hardlink", audit: true },
    CmdHelp { name: ".bvfs_update", description: "Update BVFS cache", usage: ".bvfs_update [jobid=<id>]", arguments: "jobid", audit: true },
    CmdHelp { name: ".bvfs_versions", description: "List versions of a file", usage: ".bvfs_versions jobid=0 client=<name> pathid=<id> filename=<name> [copies] [versions]", arguments: "client,pathid,filename,copies,versions", audit: true },
    CmdHelp { name: ".catalog", description: "Catalog schema management", usage: ".catalog [version|tables|schema|migrate]", arguments: "", audit: true },
    CmdHelp { name: ".catalogs", description: "List all catalog resources", usage: ".catalogs", arguments: "", audit: false },
    CmdHelp { name: ".clients", description: "List all client resources", usage: ".clients [enabled|disabled]", arguments: "enabled,disabled", audit: false },
    CmdHelp { name: ".consoles", description: "List all console resources", usage: ".consoles", arguments: "", audit: false },
    CmdHelp { name: ".defaults", description: "Get default settings", usage: ".defaults job=<name> | client=<name> | storage=<name> | pool=<name>", arguments: "job,client,storage,pool", audit: false },
    CmdHelp { name: ".filesets", description: "List all fileset resources", usage: ".filesets", arguments: "", audit: false },
    CmdHelp { name: ".grpc", description: "Switch to gRPC/Protobuf response mode", usage: ".grpc [on|off] [format=<proto|json|text>] [compact=<yes|no>]", arguments: "format,compact", audit: true },
    CmdHelp { name: ".help", description: "Print parsable information about a command", usage: ".help [all | item=<cmd>]", arguments: "", audit: false },
    CmdHelp { name: ".jobdefs", description: "List all job defaults resources", usage: ".jobdefs", arguments: "", audit: false },
    CmdHelp { name: ".jobs", description: "List all job resources", usage: ".jobs [type=<jobtype>] [enabled|disabled]", arguments: "type,enabled,disabled", audit: false },
    CmdHelp { name: ".jobstatus", description: "List jobstatus information", usage: ".jobstatus [=<status>]", arguments: "", audit: false },
    CmdHelp { name: ".levels", description: "List all backup levels", usage: ".levels", arguments: "", audit: false },
    CmdHelp { name: ".locations", description: "List all location resources", usage: ".locations", arguments: "", audit: false },
    CmdHelp { name: ".media", description: "List all media", usage: ".media", arguments: "", audit: false },
    CmdHelp { name: ".mediatypes", description: "List all media types", usage: ".mediatypes", arguments: "", audit: false },
    CmdHelp { name: ".messages", description: "Display pending messages", usage: ".messages", arguments: "", audit: false },
    CmdHelp { name: ".msgs", description: "List all message resources", usage: ".msgs", arguments: "", audit: false },
    CmdHelp { name: ".pools", description: "List all pool resources", usage: ".pools [type=<pooltype>]", arguments: "type", audit: false },
    CmdHelp { name: ".profiles", description: "List all profile resources", usage: ".profiles", arguments: "", audit: false },
    CmdHelp { name: ".quit", description: "Close connection", usage: ".quit", arguments: "", audit: false },
    CmdHelp { name: ".schedule", description: "List all schedule resources", usage: ".schedule [enabled|disabled]", arguments: "enabled,disabled", audit: false },
    CmdHelp { name: ".sql", description: "Send an arbitrary SQL command", usage: ".sql query=\"...\"", arguments: "query", audit: true },
    CmdHelp { name: ".sqlcount", description: "Show row count for tables", usage: ".sqlcount [table=<name>] [catalog=<name>]", arguments: "table,catalog", audit: true },
    CmdHelp { name: ".sqlschema", description: "Show table schema", usage: ".sqlschema [table=<name>] [catalog=<name>]", arguments: "table,catalog", audit: true },
    CmdHelp { name: ".sqltables", description: "List catalog tables", usage: ".sqltables [catalog=<name>]", arguments: "catalog", audit: true },
    CmdHelp { name: ".status", description: "Report status (machine-readable)", usage: ".status dir [current|last|header|scheduled|running|terminated] | storage=<name> | client=<name>", arguments: "storage,client", audit: true },
    CmdHelp { name: ".storages", description: "List all storage resources", usage: ".storages [enabled|disabled]", arguments: "enabled,disabled", audit: true },
    CmdHelp { name: ".types", description: "List all job types", usage: ".types", arguments: "", audit: false },
    CmdHelp { name: ".users", description: "List all user resources", usage: ".users", arguments: "", audit: false },
    CmdHelp { name: ".volstatus", description: "List all volume status", usage: ".volstatus", arguments: "", audit: false },
    // ACL management
    CmdHelp { name: ".acl", description: "Manage console ACLs", usage: ".acl [list|set|delete] [console=<name>] [type=<acl>] [value=<val>]", arguments: "console,type,value,from,to", audit: true },
    CmdHelp { name: ".profile", description: "Manage ACL profiles", usage: ".profile [list|set|delete] [name=<name>] [console=<name>]", arguments: "name,type,value,console,profile", audit: true },
];
