//! Runtime configuration for the Director.
//!
//! Wraps the parsed config resources into an Arc<RwLock<>> for
//! thread-safe access and hot-reload support.

use std::path::Path;
use std::sync::{Arc, RwLock};

use bareos_config::resources::*;
use bareos_config::parser::{self, ConfigError};

/// All parsed configuration resources, shareable across threads.
#[derive(Debug, Clone)]
pub struct DirectorConfig {
    pub director: DirectorResource,
    pub clients: Vec<ClientResource>,
    pub jobs: Vec<JobResource>,
    pub storages: Vec<StorageResource>,
    pub catalogs: Vec<CatalogResource>,
    pub schedules: Vec<ScheduleResource>,
    pub pools: Vec<PoolResource>,
    pub filesets: Vec<FileSetResource>,
    pub consoles: Vec<ConsoleResource>,
}

impl DirectorConfig {
    /// Load configuration from a file or directory tree.
    /// If path is a directory (e.g. /etc/bareos/bareos-dir.d/), reads all *.conf recursively.
    /// If path is a file, reads it as a single config file.
    pub fn load(path: &Path) -> Result<Self, ConfigError> {
        if path.is_dir() {
            let resources = parser::parse_config_dir(path)?;
            Self::from_resources(resources)
        } else {
            let content = std::fs::read_to_string(path)
                .map_err(|e| ConfigError::ParseError(format!("cannot read {}: {e}", path.display())))?;
            Self::from_str(&content)
        }
    }

    /// Parse configuration from a string.
    pub fn from_str(input: &str) -> Result<Self, ConfigError> {
        let resources = parser::parse_config(input)?;
        Self::from_resources(resources)
    }

    /// Build config from parsed resources.
    fn from_resources(resources: Vec<bareos_config::Resource>) -> Result<Self, ConfigError> {
        let mut config = DirectorConfig {
            director: DirectorResource::default(),
            clients: Vec::new(),
            jobs: Vec::new(),
            storages: Vec::new(),
            catalogs: Vec::new(),
            schedules: Vec::new(),
            pools: Vec::new(),
            filesets: Vec::new(),
            consoles: Vec::new(),
        };

        for resource in resources {
            match resource {
                Resource::Director(d) => config.director = d,
                Resource::Client(c) => config.clients.push(c),
                Resource::Job(j) => config.jobs.push(j),
                Resource::Storage(s) => config.storages.push(s),
                Resource::Catalog(c) => config.catalogs.push(c),
                Resource::Schedule(s) => config.schedules.push(s),
                Resource::Pool(p) => config.pools.push(p),
                Resource::FileSet(f) => config.filesets.push(f),
                Resource::Console(c) => config.consoles.push(c),
            }
        }

        Ok(config)
    }

    /// Find a client by name.
    pub fn find_client(&self, name: &str) -> Option<&ClientResource> {
        self.clients.iter().find(|c| c.name == name)
    }

    /// Find a job by name.
    pub fn find_job(&self, name: &str) -> Option<&JobResource> {
        self.jobs.iter().find(|j| j.name == name)
    }

    /// Find a storage by name.
    pub fn find_storage(&self, name: &str) -> Option<&StorageResource> {
        self.storages.iter().find(|s| s.name == name)
    }

    /// Find a pool by name.
    pub fn find_pool(&self, name: &str) -> Option<&PoolResource> {
        self.pools.iter().find(|p| p.name == name)
    }

    /// Find a console by name.
    pub fn find_console(&self, name: &str) -> Option<&ConsoleResource> {
        self.consoles.iter().find(|c| c.name == name)
    }
}

/// Thread-safe shared config handle.
pub type SharedConfig = Arc<RwLock<DirectorConfig>>;

#[cfg(test)]
mod tests {
    use super::*;

    const TEST_CONFIG: &str = r#"
Director {
  Name = bareos-dir
  Port = 9101
  Password = "dirpass"
  Maximum Concurrent Jobs = 10
  Enable Grpc = yes
}

Client {
  Name = bareos-fd
  Address = localhost
  Port = 9102
  Password = "clientpass"
  Catalog = MyCatalog
}

Client {
  Name = remote-fd
  Address = 10.0.0.2
  Port = 9102
  Password = "remotepass"
  Catalog = MyCatalog
}

Storage {
  Name = File
  Address = localhost
  Port = 9103
  Password = "storpass"
  Device = FileStorage
  Media Type = File
}

Catalog {
  Name = MyCatalog
  dbname = bareos
  dbuser = bareos
  dbpassword = "dbpass"
}

Job {
  Name = "BackupClient1"
  Type = Backup
  Level = Full
  Client = bareos-fd
  FileSet = "Full Set"
  Storage = File
  Pool = Full
  Priority = 10
}

Pool {
  Name = Full
  Pool Type = Backup
  Recycle = yes
  AutoPrune = yes
  Volume Retention = 365 days
  Maximum Volume Bytes = 50G
  Maximum Volumes = 100
  Label Format = "Full-"
}

Schedule {
  Name = "WeeklyCycle"
  Run = Full 1st sun at 23:05
  Run = Differential 2nd-5th sun at 23:05
  Run = Incremental mon-sat at 23:05
}

Console {
  Name = admin
  Password = "adminpass"
}
"#;

    #[test]
    fn load_full_config() {
        let config = DirectorConfig::from_str(TEST_CONFIG).unwrap();

        assert_eq!(config.director.name, "bareos-dir");
        assert_eq!(config.director.port, 9101);
        assert_eq!(config.director.password, "dirpass");
        assert_eq!(config.director.max_concurrent_jobs, 10);
        assert!(config.director.enable_grpc);

        assert_eq!(config.clients.len(), 2);
        assert_eq!(config.clients[0].name, "bareos-fd");
        assert_eq!(config.clients[1].name, "remote-fd");

        assert_eq!(config.storages.len(), 1);
        assert_eq!(config.storages[0].name, "File");

        assert_eq!(config.catalogs.len(), 1);
        assert_eq!(config.catalogs[0].db_name, "bareos");

        assert_eq!(config.jobs.len(), 1);
        assert_eq!(config.jobs[0].name, "BackupClient1");

        assert_eq!(config.pools.len(), 1);
        assert_eq!(config.pools[0].name, "Full");
        assert_eq!(config.pools[0].maximum_volumes, 100);

        assert_eq!(config.schedules.len(), 1);
        assert_eq!(config.schedules[0].runs.len(), 3);

        assert_eq!(config.consoles.len(), 1);
        assert_eq!(config.consoles[0].name, "admin");
    }

    #[test]
    fn find_resources() {
        let config = DirectorConfig::from_str(TEST_CONFIG).unwrap();

        assert!(config.find_client("bareos-fd").is_some());
        assert!(config.find_client("nonexistent").is_none());

        assert!(config.find_job("BackupClient1").is_some());
        assert!(config.find_storage("File").is_some());
        assert!(config.find_pool("Full").is_some());
        assert!(config.find_console("admin").is_some());
    }

    #[test]
    fn shared_config_thread_safe() {
        let config = DirectorConfig::from_str(TEST_CONFIG).unwrap();
        let shared: SharedConfig = Arc::new(RwLock::new(config));

        let handles: Vec<_> = (0..10).map(|_| {
            let c = shared.clone();
            std::thread::spawn(move || {
                let config = c.read().unwrap();
                assert_eq!(config.director.name, "bareos-dir");
            })
        }).collect();

        for h in handles {
            h.join().unwrap();
        }
    }
}
