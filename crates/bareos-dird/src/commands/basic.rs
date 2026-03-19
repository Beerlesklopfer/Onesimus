//! Basic commands: version, time, quit, help.

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

const VERSION: &str = env!("CARGO_PKG_VERSION");

pub fn cmd_version(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok(format!("baReos Director Version: {VERSION}"))
}

pub fn cmd_time(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let now = chrono::Local::now();
    CommandResult::ok(format!("{}", now.format("%Y-%m-%d %H:%M:%S")))
}

pub fn cmd_quit(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("bye\n".to_string())
}

pub fn cmd_help(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let topic = ast.arguments.first().and_then(|a| a.value.as_deref());

    if let Some(cmd) = topic {
        // Help for specific command
        let help = COMMAND_HELP.iter()
            .find(|(name, _, _)| *name == cmd)
            .map(|(name, help, usage)| format!("{name}: {help}\n  Usage: {usage}\n"))
            .unwrap_or_else(|| format!("Unknown command: {cmd}\n"));
        CommandResult::ok(help)
    } else {
        let mut output = String::from("Available commands:\n");
        for (name, help, _) in COMMAND_HELP {
            output.push_str(&format!("  {name:<20} {help}\n"));
        }
        CommandResult::ok(output)
    }
}

pub fn cmd_dot_help(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    // Machine-readable help: one command per line
    let mut output = String::new();
    for (name, help, usage) in COMMAND_HELP {
        output.push_str(&format!("{name}\t{help}\t{usage}\n"));
    }
    CommandResult::ok(output)
}

static COMMAND_HELP: &[(&str, &str, &str)] = &[
    ("version", "Print Director version", "version"),
    ("time", "Print current time", "time"),
    ("quit", "Terminate bconsole", "quit"),
    ("exit", "Terminate bconsole", "exit"),
    ("help", "Print help on command", "help [command]"),
    (".help", "Print machine-readable help", ".help [command]"),
    ("status", "Report status", "status [dir | client=<name> | storage=<name>]"),
    (".status", "Report status (machine)", ".status dir"),
    (".clients", "List clients", ".clients"),
    (".jobs", "List jobs", ".jobs"),
    (".pools", "List pools", ".pools"),
    (".storage", "List storage", ".storage"),
    (".filesets", "List filesets", ".filesets"),
    (".defaults", "List defaults", ".defaults [job=<name>]"),
    (".jobstatus", "List job status codes", ".jobstatus"),
    ("run", "Run a job", "run [job=<name>] [level=<type>] [client=<name>]"),
    ("cancel", "Cancel a job", "cancel [jobid=<id>]"),
    ("enable", "Enable resource", "enable [client=<name> | job=<name>]"),
    ("disable", "Disable resource", "disable [client=<name> | job=<name>]"),
    ("list", "List catalog contents", "list [jobs | volumes | pools | clients]"),
    ("llist", "Long list catalog contents", "llist [jobs | volumes]"),
    ("label", "Label a volume", "label [volume=<name>] [storage=<name>]"),
    ("mount", "Mount storage device", "mount [storage=<name>]"),
    ("unmount", "Unmount storage device", "unmount [storage=<name>]"),
    ("release", "Release storage device", "release [storage=<name>]"),
    ("update", "Update catalog records", "update [pool | volume | slots]"),
    ("restore", "Restore files", "restore [where=<path>]"),
    ("estimate", "Estimate backup size", "estimate [job=<name>]"),
    ("prune", "Prune catalog records", "prune [files | jobs | volumes]"),
    ("purge", "Purge catalog records", "purge [files | jobs | volumes]"),
    ("truncate", "Truncate purged volumes", "truncate [volume=<name>]"),
    ("reload", "Reload configuration", "reload"),
    ("configure", "Configure resources", "configure [add | delete | update]"),
    (".sql", "Execute SQL", ".sql query=\"...\""),
    (".acl", "Manage ACLs", ".acl [list | set | delete]"),
];
