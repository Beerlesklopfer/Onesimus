//! Plugin variable IDs — maps to C++ bDirVariable / bwDirVariable.

use std::collections::HashMap;

/// Read-only job variables accessible via GetValue().
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
#[repr(i32)]
pub enum DirVariable {
    Job = 1,
    Level = 2,
    Type = 3,
    JobId = 4,
    Client = 5,
    NumVols = 6,
    Pool = 7,
    Storage = 8,
    WriteStorage = 9,
    ReadStorage = 10,
    Catalog = 11,
    MediaType = 12,
    JobName = 13,
    JobStatus = 14,
    Priority = 15,
    VolumeName = 16,
    CatalogRes = 17,
    JobErrors = 18,
    JobFiles = 19,
    SDJobFiles = 20,
    SDErrors = 21,
    FDJobStatus = 22,
    SDJobStatus = 23,
    PluginDir = 24,
    LastRate = 25,
    JobBytes = 26,
    ReadBytes = 27,
}

/// Writable job variables accessible via SetValue().
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
#[repr(i32)]
pub enum DirWriteVariable {
    VolumeName = 1,
    Priority = 2,
    JobLevel = 3,
    JobReport = 4,
}

/// Job context — holds current job state accessible to plugins.
#[derive(Debug, Clone, Default)]
pub struct JobContext {
    pub job_id: i32,
    pub job_name: String,
    pub job_level: String,
    pub job_type: String,
    pub job_status: String,
    pub client: String,
    pub pool: String,
    pub storage: String,
    pub catalog: String,
    pub media_type: String,
    pub volume_name: String,
    pub priority: i32,
    pub num_vols: i32,
    pub job_errors: i64,
    pub job_files: i64,
    pub job_bytes: i64,
    pub read_bytes: i64,
    pub fd_job_status: String,
    pub sd_job_status: String,
    pub sd_errors: i64,
    pub sd_job_files: i64,
    pub last_rate: i64,
    pub plugin_dir: String,
}

impl JobContext {
    /// Get a variable value as string.
    pub fn get_string(&self, var: DirVariable) -> String {
        match var {
            DirVariable::Job | DirVariable::JobName => self.job_name.clone(),
            DirVariable::Level => self.job_level.clone(),
            DirVariable::Type => self.job_type.clone(),
            DirVariable::Client => self.client.clone(),
            DirVariable::Pool => self.pool.clone(),
            DirVariable::Storage | DirVariable::WriteStorage | DirVariable::ReadStorage => {
                self.storage.clone()
            }
            DirVariable::Catalog | DirVariable::CatalogRes => self.catalog.clone(),
            DirVariable::MediaType => self.media_type.clone(),
            DirVariable::VolumeName => self.volume_name.clone(),
            DirVariable::JobStatus => self.job_status.clone(),
            DirVariable::FDJobStatus => self.fd_job_status.clone(),
            DirVariable::SDJobStatus => self.sd_job_status.clone(),
            DirVariable::PluginDir => self.plugin_dir.clone(),
            _ => String::new(),
        }
    }

    /// Get a variable value as i64.
    pub fn get_int(&self, var: DirVariable) -> i64 {
        match var {
            DirVariable::JobId => self.job_id as i64,
            DirVariable::NumVols => self.num_vols as i64,
            DirVariable::Priority => self.priority as i64,
            DirVariable::JobErrors => self.job_errors,
            DirVariable::JobFiles => self.job_files,
            DirVariable::JobBytes => self.job_bytes,
            DirVariable::ReadBytes => self.read_bytes,
            DirVariable::SDErrors => self.sd_errors,
            DirVariable::SDJobFiles => self.sd_job_files,
            DirVariable::LastRate => self.last_rate,
            _ => 0,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn job_context_get_string() {
        let ctx = JobContext {
            job_name: "BackupJob".into(),
            client: "bareos-fd".into(),
            pool: "Full".into(),
            ..Default::default()
        };
        assert_eq!(ctx.get_string(DirVariable::JobName), "BackupJob");
        assert_eq!(ctx.get_string(DirVariable::Client), "bareos-fd");
        assert_eq!(ctx.get_string(DirVariable::Pool), "Full");
    }

    #[test]
    fn job_context_get_int() {
        let ctx = JobContext {
            job_id: 42,
            priority: 10,
            job_files: 1000,
            job_bytes: 1048576,
            ..Default::default()
        };
        assert_eq!(ctx.get_int(DirVariable::JobId), 42);
        assert_eq!(ctx.get_int(DirVariable::Priority), 10);
        assert_eq!(ctx.get_int(DirVariable::JobFiles), 1000);
        assert_eq!(ctx.get_int(DirVariable::JobBytes), 1048576);
    }
}
