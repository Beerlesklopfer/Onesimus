//! Configure commands: configure add/delete/update

use bareos_config::resources::*;

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_configure(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let action = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");

    match action {
        "add" => configure_add(ast, ctx),
        "read" | "show" | "get" => configure_read(ast, ctx),
        "update" => configure_update(ast, ctx),
        "delete" => configure_delete(ast, ctx),
        "export" => configure_export(ast, ctx),
        _ => CommandResult::ok(
            "configure: CRUD operations on resources:\n  \
             configure add client name=<n> address=<a> password=<p>\n  \
             configure add job name=<n> client=<c> type=Backup\n  \
             configure add pool name=<n>\n  \
             configure read client=<name>\n  \
             configure update client=<name> address=<new>\n  \
             configure delete client=<name>\n  \
             configure export\n"
                .to_string(),
        ),
    }
}

fn configure_add(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let resource_type = ast.arguments.get(1).map(|a| a.key.as_str()).unwrap_or("");

    let config = match &ctx.config {
        Some(c) => c,
        None => return CommandResult::error("No config loaded\n".to_string()),
    };

    let get_arg = |key: &str| -> Option<&str> {
        ast.arguments.iter()
            .find(|a| a.key == key)
            .and_then(|a| a.value.as_deref())
    };

    match resource_type {
        "client" => {
            let name = match get_arg("name") {
                Some(n) => n,
                None => return CommandResult::error("configure add client: name= required\n".to_string()),
            };
            let address = get_arg("address").unwrap_or("localhost");
            let port: u16 = get_arg("port").and_then(|p| p.parse().ok()).unwrap_or(9102);
            let password = get_arg("password").unwrap_or("");

            let mut cfg = config.write().unwrap();
            if cfg.find_client(name).is_some() {
                return CommandResult::error(format!("Client '{name}' already exists\n"));
            }

            cfg.clients.push(ClientResource {
                name: name.to_string(),
                address: address.to_string(),
                port,
                password: password.to_string(),
                catalog: String::new(),
                max_concurrent_jobs: 1,
                enabled: true,
            });

            CommandResult::ok(format!("Created resource Client '{name}' address={address} port={port}\n"))
        }
        "pool" => {
            let name = match get_arg("name") {
                Some(n) => n,
                None => return CommandResult::error("configure add pool: name= required\n".to_string()),
            };

            let mut cfg = config.write().unwrap();
            if cfg.find_pool(name).is_some() {
                return CommandResult::error(format!("Pool '{name}' already exists\n"));
            }

            cfg.pools.push(PoolResource {
                name: name.to_string(),
                pool_type: get_arg("pooltype").unwrap_or("Backup").to_string(),
                recycle: true,
                auto_prune: true,
                volume_retention: std::time::Duration::from_secs(365 * 86400),
                maximum_volume_bytes: 0,
                maximum_volumes: 0,
                label_format: String::new(),
            });

            CommandResult::ok(format!("Created resource Pool '{name}'\n"))
        }
        "job" => {
            let name = match get_arg("name") {
                Some(n) => n,
                None => return CommandResult::error("configure add job: name= required\n".to_string()),
            };
            let client = get_arg("client").unwrap_or("");
            let job_type = match get_arg("type").unwrap_or("Backup").to_lowercase().as_str() {
                "backup" => JobType::Backup,
                "restore" => JobType::Restore,
                "verify" => JobType::Verify,
                "admin" => JobType::Admin,
                _ => JobType::Backup,
            };

            let mut cfg = config.write().unwrap();
            if cfg.find_job(name).is_some() {
                return CommandResult::error(format!("Job '{name}' already exists\n"));
            }

            cfg.jobs.push(JobResource {
                name: name.to_string(),
                job_type,
                client: client.to_string(),
                storage: get_arg("storage").unwrap_or("").to_string(),
                pool: get_arg("pool").unwrap_or("").to_string(),
                fileset: get_arg("fileset").unwrap_or("").to_string(),
                ..JobResource::default()
            });

            CommandResult::ok(format!("Created resource Job '{name}' type={job_type:?} client={client}\n"))
        }
        _ => CommandResult::ok(format!("configure add {resource_type}: unknown resource type\nValid: client, job, pool, storage, schedule, console\n")),
    }
}

fn configure_delete(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let config = match &ctx.config {
        Some(c) => c,
        None => return CommandResult::error("No config loaded\n".to_string()),
    };

    // Check for client=, job=, pool= arguments
    for arg in &ast.arguments {
        if let Some(name) = &arg.value {
            let mut cfg = config.write().unwrap();
            match arg.key.as_str() {
                "client" => {
                    let before = cfg.clients.len();
                    cfg.clients.retain(|c| c.name != *name);
                    if cfg.clients.len() < before {
                        return CommandResult::ok(format!("Deleted Client '{name}'\n"));
                    }
                    return CommandResult::error(format!("Client '{name}' not found\n"));
                }
                "job" => {
                    let before = cfg.jobs.len();
                    cfg.jobs.retain(|j| j.name != *name);
                    if cfg.jobs.len() < before {
                        return CommandResult::ok(format!("Deleted Job '{name}'\n"));
                    }
                    return CommandResult::error(format!("Job '{name}' not found\n"));
                }
                "pool" => {
                    let before = cfg.pools.len();
                    cfg.pools.retain(|p| p.name != *name);
                    if cfg.pools.len() < before {
                        return CommandResult::ok(format!("Deleted Pool '{name}'\n"));
                    }
                    return CommandResult::error(format!("Pool '{name}' not found\n"));
                }
                "storage" => {
                    let before = cfg.storages.len();
                    cfg.storages.retain(|s| s.name != *name);
                    if cfg.storages.len() < before {
                        return CommandResult::ok(format!("Deleted Storage '{name}'\n"));
                    }
                    return CommandResult::error(format!("Storage '{name}' not found\n"));
                }
                "console" => {
                    let before = cfg.consoles.len();
                    cfg.consoles.retain(|c| c.name != *name);
                    if cfg.consoles.len() < before {
                        return CommandResult::ok(format!("Deleted Console '{name}'\n"));
                    }
                    return CommandResult::error(format!("Console '{name}' not found\n"));
                }
                "schedule" => {
                    let before = cfg.schedules.len();
                    cfg.schedules.retain(|s| s.name != *name);
                    if cfg.schedules.len() < before {
                        return CommandResult::ok(format!("Deleted Schedule '{name}'\n"));
                    }
                    return CommandResult::error(format!("Schedule '{name}' not found\n"));
                }
                _ => {}
            }
        }
    }

    CommandResult::ok("configure delete: specify client=<name>, job=<name>, pool=<name>, etc.\n".to_string())
}

fn configure_read(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let config = match &ctx.config {
        Some(c) => match c.read() {
            Ok(c) => c,
            Err(_) => return CommandResult::error("Config error\n".to_string()),
        },
        None => return CommandResult::error("No config\n".to_string()),
    };

    let get_arg = |key: &str| -> Option<&str> {
        ast.arguments.iter().find(|a| a.key == key).and_then(|a| a.value.as_deref())
    };

    if let Some(name) = get_arg("client") {
        if let Some(c) = config.find_client(name) {
            return CommandResult::ok(format!(
                "Client {{\n  Name = \"{}\"\n  Address = \"{}\"\n  Port = {}\n  Enabled = {}\n}}\n",
                c.name, c.address, c.port, if c.enabled { "yes" } else { "no" }
            ));
        }
        return CommandResult::error(format!("Client '{name}' not found\n"));
    }
    if let Some(name) = get_arg("job") {
        if let Some(j) = config.find_job(name) {
            return CommandResult::ok(format!(
                "Job {{\n  Name = \"{}\"\n  Type = {:?}\n  Level = {:?}\n  Client = \"{}\"\n  \
                 Storage = \"{}\"\n  Pool = \"{}\"\n  FileSet = \"{}\"\n  Priority = {}\n  Enabled = {}\n}}\n",
                j.name, j.job_type, j.level, j.client, j.storage, j.pool, j.fileset, j.priority,
                if j.enabled { "yes" } else { "no" }
            ));
        }
        return CommandResult::error(format!("Job '{name}' not found\n"));
    }
    if let Some(name) = get_arg("pool") {
        if let Some(p) = config.find_pool(name) {
            return CommandResult::ok(format!(
                "Pool {{\n  Name = \"{}\"\n  Pool Type = \"{}\"\n  Recycle = {}\n  \
                 Auto Prune = {}\n  Maximum Volumes = {}\n}}\n",
                p.name, p.pool_type, if p.recycle { "yes" } else { "no" },
                if p.auto_prune { "yes" } else { "no" }, p.maximum_volumes
            ));
        }
        return CommandResult::error(format!("Pool '{name}' not found\n"));
    }
    if let Some(name) = get_arg("storage") {
        if let Some(s) = config.find_storage(name) {
            return CommandResult::ok(format!(
                "Storage {{\n  Name = \"{}\"\n  Address = \"{}\"\n  Port = {}\n  \
                 Device = \"{}\"\n  Media Type = \"{}\"\n}}\n",
                s.name, s.address, s.port, s.device, s.media_type
            ));
        }
        return CommandResult::error(format!("Storage '{name}' not found\n"));
    }

    CommandResult::ok("configure read: specify client=, job=, pool=, or storage=\n".to_string())
}

fn configure_export(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let config = match &ctx.config {
        Some(c) => match c.read() {
            Ok(c) => c,
            Err(_) => return CommandResult::error("Config error\n".to_string()),
        },
        None => return CommandResult::error("No config\n".to_string()),
    };

    let mut output = String::new();

    // Director
    output.push_str(&format!(
        "Director {{\n  Name = \"{}\"\n  Port = {}\n  Maximum Concurrent Jobs = {}\n}}\n\n",
        config.director.name, config.director.port, config.director.max_concurrent_jobs
    ));

    // Clients
    for c in &config.clients {
        output.push_str(&format!(
            "Client {{\n  Name = \"{}\"\n  Address = \"{}\"\n  Port = {}\n  Enabled = {}\n}}\n\n",
            c.name, c.address, c.port, if c.enabled { "yes" } else { "no" }
        ));
    }

    // Jobs
    for j in &config.jobs {
        output.push_str(&format!(
            "Job {{\n  Name = \"{}\"\n  Type = {:?}\n  Level = {:?}\n  Client = \"{}\"\n  \
             Storage = \"{}\"\n  Pool = \"{}\"\n}}\n\n",
            j.name, j.job_type, j.level, j.client, j.storage, j.pool
        ));
    }

    // Storages
    for s in &config.storages {
        output.push_str(&format!(
            "Storage {{\n  Name = \"{}\"\n  Address = \"{}\"\n  Port = {}\n  \
             Device = \"{}\"\n  Media Type = \"{}\"\n}}\n\n",
            s.name, s.address, s.port, s.device, s.media_type
        ));
    }

    // Pools
    for p in &config.pools {
        output.push_str(&format!(
            "Pool {{\n  Name = \"{}\"\n  Pool Type = \"{}\"\n  Recycle = {}\n  \
             Maximum Volumes = {}\n}}\n\n",
            p.name, p.pool_type, if p.recycle { "yes" } else { "no" }, p.maximum_volumes
        ));
    }

    CommandResult::ok(output)
}

fn configure_update(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let config = match &ctx.config {
        Some(c) => c,
        None => return CommandResult::error("No config loaded\n".to_string()),
    };

    let get_arg = |key: &str| -> Option<&str> {
        ast.arguments.iter()
            .find(|a| a.key == key)
            .and_then(|a| a.value.as_deref())
    };

    // configure update client=<name> address=<new> port=<new>
    if let Some(name) = get_arg("client") {
        let mut cfg = config.write().unwrap();
        if let Some(c) = cfg.clients.iter_mut().find(|c| c.name == name) {
            let mut changes = Vec::new();
            if let Some(addr) = get_arg("address") {
                c.address = addr.to_string();
                changes.push(format!("address={addr}"));
            }
            if let Some(port) = get_arg("port") {
                if let Ok(p) = port.parse() {
                    c.port = p;
                    changes.push(format!("port={port}"));
                }
            }
            if let Some(pw) = get_arg("password") {
                c.password = pw.to_string();
                changes.push("password=***".to_string());
            }
            if changes.is_empty() {
                return CommandResult::ok(format!("Client '{name}': no changes specified\n"));
            }
            return CommandResult::ok(format!("Client '{name}' updated: {}\n", changes.join(", ")));
        }
        return CommandResult::error(format!("Client '{name}' not found\n"));
    }

    if let Some(name) = get_arg("job") {
        let mut cfg = config.write().unwrap();
        if let Some(j) = cfg.jobs.iter_mut().find(|j| j.name == name) {
            let mut changes = Vec::new();
            if let Some(client) = get_arg("newclient") {
                j.client = client.to_string();
                changes.push(format!("client={client}"));
            }
            if let Some(storage) = get_arg("newstorage") {
                j.storage = storage.to_string();
                changes.push(format!("storage={storage}"));
            }
            if let Some(pool) = get_arg("newpool") {
                j.pool = pool.to_string();
                changes.push(format!("pool={pool}"));
            }
            if changes.is_empty() {
                return CommandResult::ok(format!("Job '{name}': no changes specified\n"));
            }
            return CommandResult::ok(format!("Job '{name}' updated: {}\n", changes.join(", ")));
        }
        return CommandResult::error(format!("Job '{name}' not found\n"));
    }

    CommandResult::ok(
        "configure update: specify resource to update:\n  \
         configure update client=<name> address=<new> port=<new>\n  \
         configure update job=<name> newclient=<c> newstorage=<s>\n"
            .to_string()
    )
}
