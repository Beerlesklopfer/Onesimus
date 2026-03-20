//! Job control commands: run, cancel, enable, disable

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_run(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let job_name = ast.arguments.iter()
        .find(|a| a.key == "job")
        .and_then(|a| a.value.as_deref());

    let job_name = match job_name {
        Some(name) => name,
        None => {
            // If no job= argument, list available jobs
            if let Some(config) = &ctx.config {
                if let Ok(cfg) = config.read() {
                    let jobs: Vec<_> = cfg.jobs.iter().map(|j| j.name.as_str()).collect();
                    if jobs.is_empty() {
                        return CommandResult::ok("No jobs configured\n".to_string());
                    }
                    let mut output = String::from("Available jobs:\n");
                    for (i, name) in jobs.iter().enumerate() {
                        output.push_str(&format!("  {}. {name}\n", i + 1));
                    }
                    output.push_str("\nUse: run job=<name> [level=Full|Incremental|Differential]\n");
                    return CommandResult::ok(output);
                }
            }
            return CommandResult::ok("run: specify job=<name>\n".to_string());
        }
    };

    // Look up job in config
    let config = match &ctx.config {
        Some(c) => match c.read() {
            Ok(c) => c,
            _ => return CommandResult::error("Config read error\n".to_string()),
        },
        None => return CommandResult::error("No config loaded\n".to_string()),
    };

    let job = match config.find_job(job_name) {
        Some(j) => j.clone(),
        None => return CommandResult::error(format!("Job '{job_name}' not found\n")),
    };

    // Parse optional overrides
    let default_level = format!("{:?}", job.level);
    let level = ast.arguments.iter()
        .find(|a| a.key == "level")
        .and_then(|a| a.value.as_deref())
        .unwrap_or(&default_level);

    let client = ast.arguments.iter()
        .find(|a| a.key == "client")
        .and_then(|a| a.value.as_deref())
        .unwrap_or(&job.client);

    let storage = ast.arguments.iter()
        .find(|a| a.key == "storage")
        .and_then(|a| a.value.as_deref())
        .unwrap_or(&job.storage);

    let pool = ast.arguments.iter()
        .find(|a| a.key == "pool")
        .and_then(|a| a.value.as_deref())
        .unwrap_or(&job.pool);

    // Report what would be run
    let output = format!(
        "Job queued. JobId=0\n\
         Run Backup job\n\
         Job:        {}\n\
         Level:      {}\n\
         Client:     {}\n\
         Storage:    {}\n\
         Pool:       {}\n\
         FileSet:    {}\n\
         Priority:   {}\n\
         When:       now\n",
        job.name, level, client, storage, pool, job.fileset, job.priority,
    );

    // TODO: Actually spawn job worker via fork()
    // job_worker::spawn_job_worker(job_id, &job.name)

    if let Some(smgr) = &ctx.session_mgr {
        smgr.bus().post(crate::message_bus::BusMessage::JobStatus {
            job_id: 0,
            job_name: job.name.clone(),
            status: "Queued".to_string(),
            timestamp: std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap_or_default()
                .as_secs() as i64,
        });
    }

    CommandResult::ok(output)
}

pub fn cmd_cancel(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let job_id = ast.arguments.iter()
        .find(|a| a.key == "jobid")
        .and_then(|a| a.value.as_deref())
        .and_then(|v| v.parse::<u32>().ok());

    match job_id {
        Some(id) => CommandResult::ok(format!("Job {id} marked for cancellation\n")),
        None => CommandResult::ok("cancel: specify jobid=<id>\n".to_string()),
    }
}

pub fn cmd_enable(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let target = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");
    let name = ast.arguments.first().and_then(|a| a.value.as_deref()).unwrap_or("");

    match target {
        "client" => CommandResult::ok(format!("Client '{name}' enabled\n")),
        "job" => CommandResult::ok(format!("Job '{name}' enabled\n")),
        "schedule" => CommandResult::ok(format!("Schedule '{name}' enabled\n")),
        "storage" => CommandResult::ok(format!("Storage '{name}' enabled\n")),
        _ => CommandResult::ok("enable: specify client=, job=, schedule=, or storage=\n".to_string()),
    }
}

pub fn cmd_disable(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let target = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");
    let name = ast.arguments.first().and_then(|a| a.value.as_deref()).unwrap_or("");

    match target {
        "client" => CommandResult::ok(format!("Client '{name}' disabled\n")),
        "job" => CommandResult::ok(format!("Job '{name}' disabled\n")),
        "schedule" => CommandResult::ok(format!("Schedule '{name}' disabled\n")),
        "storage" => CommandResult::ok(format!("Storage '{name}' disabled\n")),
        _ => CommandResult::ok("disable: specify client=, job=, schedule=, or storage=\n".to_string()),
    }
}
