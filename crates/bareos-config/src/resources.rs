//! Bareos configuration resource types.

use std::time::Duration;

/// All resource types in a Bareos Director configuration.
#[derive(Debug, Clone)]
pub enum Resource {
    Director(DirectorResource),
    Client(ClientResource),
    Job(JobResource),
    Storage(StorageResource),
    Catalog(CatalogResource),
    Schedule(ScheduleResource),
    FileSet(FileSetResource),
    Pool(PoolResource),
    Console(ConsoleResource),
}

#[derive(Debug, Clone)]
pub struct DirectorResource {
    pub name: String,
    pub password: String,
    pub port: u16,
    pub max_concurrent_jobs: u32,
    pub fd_connect_timeout: Duration,
    pub sd_connect_timeout: Duration,
    pub heartbeat_interval: Duration,
    pub enable_grpc: bool,
}

impl Default for DirectorResource {
    fn default() -> Self {
        Self {
            name: String::new(),
            password: String::new(),
            port: 9101,
            max_concurrent_jobs: 1,
            fd_connect_timeout: Duration::from_secs(180),
            sd_connect_timeout: Duration::from_secs(1800),
            heartbeat_interval: Duration::ZERO,
            enable_grpc: false,
        }
    }
}

#[derive(Debug, Clone)]
pub struct ClientResource {
    pub name: String,
    pub address: String,
    pub port: u16,
    pub password: String,
    pub catalog: String,
    pub max_concurrent_jobs: u32,
    pub enabled: bool,
}

impl Default for ClientResource {
    fn default() -> Self {
        Self {
            name: String::new(),
            address: String::new(),
            port: 9102,
            password: String::new(),
            catalog: String::new(),
            max_concurrent_jobs: 1,
            enabled: true,
        }
    }
}

#[derive(Debug, Clone)]
pub struct JobResource {
    pub name: String,
    pub job_type: JobType,
    pub level: JobLevel,
    pub client: String,
    pub fileset: String,
    pub schedule: String,
    pub storage: String,
    pub pool: String,
    pub messages: String,
    pub priority: i32,
    pub max_concurrent_jobs: u32,
    pub enabled: bool,
}

impl Default for JobResource {
    fn default() -> Self {
        Self {
            name: String::new(),
            job_type: JobType::Backup,
            level: JobLevel::Full,
            client: String::new(),
            fileset: String::new(),
            schedule: String::new(),
            storage: String::new(),
            pool: String::new(),
            messages: String::new(),
            priority: 10,
            max_concurrent_jobs: 1,
            enabled: true,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum JobType {
    Backup,
    Restore,
    Verify,
    Admin,
    Archive,
    Copy,
    Migrate,
    Consolidate,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum JobLevel {
    Full,
    Incremental,
    Differential,
    VirtualFull,
    InitCatalog,
    Catalog,
    VolumeToCatalog,
    DiskToCatalog,
    Data,
}

#[derive(Debug, Clone)]
pub struct StorageResource {
    pub name: String,
    pub address: String,
    pub port: u16,
    pub password: String,
    pub device: String,
    pub media_type: String,
    pub max_concurrent_jobs: u32,
    pub enabled: bool,
}

impl Default for StorageResource {
    fn default() -> Self {
        Self {
            name: String::new(),
            address: String::new(),
            port: 9103,
            password: String::new(),
            device: String::new(),
            media_type: String::new(),
            max_concurrent_jobs: 1,
            enabled: true,
        }
    }
}

#[derive(Debug, Clone)]
pub struct CatalogResource {
    pub name: String,
    pub db_driver: String,
    pub db_name: String,
    pub db_address: String,
    pub db_port: u16,
    pub db_user: String,
    pub db_password: String,
}

impl Default for CatalogResource {
    fn default() -> Self {
        Self {
            name: String::new(),
            db_driver: "postgresql".into(),
            db_name: "bareos".into(),
            db_address: "localhost".into(),
            db_port: 5432,
            db_user: String::new(),
            db_password: String::new(),
        }
    }
}

#[derive(Debug, Clone)]
pub struct ScheduleResource {
    pub name: String,
    pub enabled: bool,
    pub runs: Vec<RunDirective>,
}

#[derive(Debug, Clone)]
pub struct RunDirective {
    pub level: Option<JobLevel>,
    pub pool: Option<String>,
    pub storage: Option<String>,
    pub priority: Option<i32>,
    pub hour: u32,
    pub minute: u32,
}

#[derive(Debug, Clone)]
pub struct FileSetResource {
    pub name: String,
    pub include_blocks: Vec<String>,
    pub exclude_blocks: Vec<String>,
}

#[derive(Debug, Clone)]
pub struct PoolResource {
    pub name: String,
    pub pool_type: String,
    pub recycle: bool,
    pub auto_prune: bool,
    pub volume_retention: Duration,
    pub maximum_volume_bytes: u64,
    pub maximum_volumes: u32,
    pub label_format: String,
}

#[derive(Debug, Clone)]
pub struct ConsoleResource {
    pub name: String,
    pub password: String,
}
