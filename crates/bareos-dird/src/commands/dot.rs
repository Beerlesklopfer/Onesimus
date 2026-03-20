//! Dot commands: .clients, .jobs, .pools, .storage, .filesets, .defaults, .jobstatus

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_dot_clients(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let output: String = cfg.clients.iter()
                .map(|c| format!("{}\n", c.name))
                .collect();
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_jobs(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let output: String = cfg.jobs.iter()
                .map(|j| format!("{}\n", j.name))
                .collect();
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_pools(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let output: String = cfg.pools.iter()
                .map(|p| format!("{}\n", p.name))
                .collect();
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_storage(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let output: String = cfg.storages.iter()
                .map(|s| format!("{}\n", s.name))
                .collect();
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_filesets(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    if let Some(config) = &ctx.config {
        if let Ok(cfg) = config.read() {
            let output: String = cfg.filesets.iter()
                .map(|f| format!("{}\n", f.name))
                .collect();
            return CommandResult::ok(output);
        }
    }
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_defaults(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    CommandResult::ok("".to_string())
}

pub fn cmd_dot_jobstatus(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let output = "\
A\tCanceled by user\n\
B\tBlocked\n\
C\tCreated, not yet running\n\
D\tVerify found differences\n\
E\tTerminated with errors\n\
F\tWaiting for FD\n\
I\tIncomplete job\n\
M\tWaiting for mount\n\
R\tRunning\n\
S\tWaiting for SD\n\
T\tCompleted successfully\n\
W\tCompleted with warnings\n\
a\tSD despooling attributes\n\
c\tWaiting for client\n\
d\tWaiting for max jobs\n\
e\tNon-fatal error\n\
f\tFatal error\n\
j\tWaiting for job\n\
m\tWaiting for media\n\
p\tWaiting for priority\n\
q\tQueued\n\
s\tWaiting for storage\n\
t\tWaiting for start time\n";
    CommandResult::ok(output.to_string())
}
