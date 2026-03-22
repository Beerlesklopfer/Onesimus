//! Job Manager — Director-side tracking of all running job workers.
//!
//! Maintains a Vec of active jobs (up to 1000+ concurrent).
//! One monitor thread polls all worker pipes and handles CatReq.

use std::sync::{Arc, Mutex};
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

use tracing::{info, debug, warn, error};

use crate::job_worker::{JobHandle, PipeMessage};
use crate::message_bus::{MessageBus, BusMessage, MsgType};
use crate::director_config::SharedConfig;

/// Director-side view of a running job.
pub struct DirJob {
    pub job_id: u32,
    pub job_name: String,
    pub phase: DirJobPhase,
    pub handle: JobHandle,
    pub start_time: i64,
    pub last_files: i64,
    pub last_bytes: i64,
}

/// Director-side job phases.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum DirJobPhase {
    Queued,
    Starting,
    Running,
    Paused,
    WaitingCatReq,
    Finishing,
    Completed,
    Error,
}

impl DirJobPhase {
    pub fn name(&self) -> &str {
        match self {
            Self::Queued => "Queued",
            Self::Starting => "Starting",
            Self::Running => "Running",
            Self::Paused => "Paused",
            Self::WaitingCatReq => "WaitingCatReq",
            Self::Finishing => "Finishing",
            Self::Completed => "Completed",
            Self::Error => "Error",
        }
    }
}

/// Manages all active job workers — Vec-based for up to 1000+ jobs.
pub struct JobManager {
    jobs: Vec<DirJob>,
    bus: Arc<MessageBus>,
    config: Option<SharedConfig>,
}

impl JobManager {
    pub fn new(bus: Arc<MessageBus>, config: Option<SharedConfig>) -> Self {
        Self { jobs: Vec::new(), bus, config }
    }

    pub fn add_job(&mut self, job_id: u32, job_name: &str, handle: JobHandle) {
        self.jobs.push(DirJob {
            job_id,
            job_name: job_name.to_string(),
            phase: DirJobPhase::Starting,
            handle,
            start_time: now_unix(),
            last_files: 0,
            last_bytes: 0,
        });
        debug!("JobManager: added job {job_id} '{job_name}' (total: {})", self.jobs.len());
    }

    pub fn active_count(&self) -> usize {
        self.jobs.iter().filter(|j| !matches!(j.phase, DirJobPhase::Completed | DirJobPhase::Error)).count()
    }

    pub fn get_job(&self, job_id: u32) -> Option<&DirJob> {
        self.jobs.iter().find(|j| j.job_id == job_id)
    }

    pub fn cancel_job(&mut self, job_id: u32) -> bool {
        if let Some(job) = self.jobs.iter_mut().find(|j| j.job_id == job_id) {
            job.handle.cancel();
            job.phase = DirJobPhase::Error;
            true
        } else {
            false
        }
    }

    pub fn pause_job(&mut self, job_id: u32) -> bool {
        if let Some(job) = self.jobs.iter_mut().find(|j| j.job_id == job_id) {
            job.handle.pause();
            job.phase = DirJobPhase::Paused;
            true
        } else {
            false
        }
    }

    pub fn resume_job(&mut self, job_id: u32) -> bool {
        if let Some(job) = self.jobs.iter_mut().find(|j| j.job_id == job_id) {
            job.handle.resume();
            job.phase = DirJobPhase::Running;
            true
        } else {
            false
        }
    }

    /// Poll all active worker pipes. Non-blocking.
    /// Handles CatReq, status updates, state changes, job completion.
    pub fn poll_all(&mut self) {
        for job in &mut self.jobs {
            if matches!(job.phase, DirJobPhase::Completed | DirJobPhase::Error) {
                continue;
            }

            // Check if worker exited
            if let Some(exit_code) = job.handle.try_wait() {
                if exit_code == 0 {
                    job.phase = DirJobPhase::Completed;
                } else {
                    job.phase = DirJobPhase::Error;
                }
                info!("JobManager: job {} '{}' worker exited (code={})",
                    job.job_id, job.job_name, exit_code);
                continue;
            }

            // Read pipe messages (non-blocking — try_read_message returns None if no data)
            while let Some(msg) = job.handle.try_read_message() {
                match msg {
                    PipeMessage::State { state, .. } => {
                        debug!("JobManager: job {} state={state}", job.job_id);
                        job.phase = match state.as_str() {
                            "Running" => DirJobPhase::Running,
                            "Paused" => DirJobPhase::Paused,
                            "Finishing" => DirJobPhase::Finishing,
                            "Completed" => DirJobPhase::Completed,
                            "Error" => DirJobPhase::Error,
                            _ => job.phase,
                        };
                    }

                    PipeMessage::Status { files, bytes, errors, .. } => {
                        job.last_files = files;
                        job.last_bytes = bytes;
                    }

                    PipeMessage::CatReq { request, .. } => {
                        debug!("JobManager: job {} CatReq: {}", job.job_id, &request[..request.len().min(60)]);
                        job.phase = DirJobPhase::WaitingCatReq;
                        let response = handle_cat_req(&request, &self.config);
                        job.handle.send_message(&PipeMessage::CatResp { response });
                        job.phase = DirJobPhase::Running;
                    }

                    PipeMessage::JobEnd { term_code, files, bytes, errors, .. } => {
                        let status = if term_code == 0 { "Success" } else { "Error" };
                        info!("JobManager: job {} '{}' ended — {status} files={files} bytes={bytes} errors={errors}",
                            job.job_id, job.job_name);
                        job.last_files = files;
                        job.last_bytes = bytes;
                        job.phase = if term_code == 0 { DirJobPhase::Completed } else { DirJobPhase::Error };

                        // Post to message bus
                        let msg_type = if term_code == 0 { MsgType::Info } else { MsgType::Error };
                        self.bus.post(BusMessage::JobMessage {
                            job_id: job.job_id,
                            job_name: job.job_name.clone(),
                            msg_type,
                            text: format!("Backup {status}: files={files} bytes={bytes} errors={errors}"),
                            timestamp: now_unix(),
                        });
                        self.bus.post(BusMessage::JobStatus {
                            job_id: job.job_id,
                            job_name: job.job_name.clone(),
                            status: status.to_string(),
                            timestamp: now_unix(),
                        });
                    }

                    _ => {}
                }
            }
        }

        // Cleanup completed jobs older than 5 minutes
        let cutoff = now_unix() - 300;
        self.jobs.retain(|j| {
            if matches!(j.phase, DirJobPhase::Completed | DirJobPhase::Error) && j.start_time < cutoff {
                debug!("JobManager: removing finished job {}", j.job_id);
                false
            } else {
                true
            }
        });
    }
}

/// CatReq handler — queries real DB for volume info.
fn handle_cat_req(request: &str, config: &Option<SharedConfig>) -> String {
    let catalog_url = catalog_url(config);

    if request.contains("FindMedia") {
        let pool = extract_field(request, "pool_name=").unwrap_or("Full".into());
        debug!("CatReq: FindMedia pool={pool}");
        let vol = query_appendable_volume(&catalog_url, &pool);
        match vol {
            Some(name) => query_volume_info(&catalog_url, &name).unwrap_or("1901 No Media.\n".into()),
            None => "1901 No Media.\n".to_string(),
        }
    } else if request.contains("GetVolInfo") {
        let vol = extract_field(request, "VolName=").unwrap_or("Full-0001".into());
        debug!("CatReq: GetVolInfo vol={vol}");
        query_volume_info(&catalog_url, &vol).unwrap_or("1901 No Media.\n".into())
    } else if request.contains("UpdateMedia") {
        let vol = extract_field(request, "VolName=").unwrap_or("Full-0001".into());
        debug!("CatReq: UpdateMedia vol={vol}");
        // Update DB with values from SD, then return fresh volume info
        update_media_in_db(&catalog_url, request);
        query_volume_info(&catalog_url, &vol).unwrap_or("1901 No Media.\n".into())
    } else if request.contains("CreateJobMedia") {
        "1000 OK CreateJobMedia\n".to_string()
    } else if request.contains("UpdateJobRecord") {
        "1000 OK\n".to_string()
    } else if request.contains("UpdateFileList") || request.contains("Filelist") {
        "1000 OK\n".to_string()
    } else if request.contains("DeleteNullJobmediaRecords") {
        "1000 OK DeleteNullJobmediaRecords\n".to_string()
    } else {
        debug!("CatReq: unknown: {}", &request[..request.len().min(80)]);
        "1000 OK\n".to_string()
    }
}

/// Query full volume info from DB in OK_media format (24 fields).
fn query_volume_info(url: &Option<String>, vol_name: &str) -> Option<String> {
    let url = url.as_ref()?;
    let rt = tokio::runtime::Runtime::new().ok()?;
    rt.block_on(async {
        let pool = sqlx::postgres::PgPoolOptions::new().max_connections(1).connect(url).await.ok()?;
        let row: Option<(
            String, i32, i64, i64, i64, i64, i64, i64, i64,
            String, i64, i32, i64, i64, i64, i64,
        )> = sqlx::query_as(
            "SELECT volumename, COALESCE(voljobs,0)::int, COALESCE(volfiles,0)::bigint, \
             COALESCE(volblocks,0)::bigint, COALESCE(volbytes,0)::bigint, COALESCE(volmounts,0)::bigint, \
             COALESCE(volerrors,0)::bigint, COALESCE(volwrites,0)::bigint, COALESCE(maxvolbytes,0)::bigint, \
             COALESCE(volstatus,'Append'), COALESCE(slot,0)::bigint, mediaid, \
             COALESCE(endfile,0)::bigint, COALESCE(endblock,0)::bigint, \
             COALESCE(minblocksize,0)::bigint, COALESCE(maxblocksize,0)::bigint \
             FROM media WHERE volumename = $1"
        ).bind(vol_name).fetch_optional(&pool).await.ok()?;

        row.map(|(vn, vj, vf, vbl, vby, vm, ve, vw, mvb, vs, sl, mid, ef, eb, minbs, maxbs)| {
            format!(
                "1000 OK VolName={vn} VolJobs={vj} VolFiles={vf} \
                 VolBlocks={vbl} VolBytes={vby} VolMounts={vm} VolErrors={ve} VolWrites={vw} \
                 MaxVolBytes={mvb} VolCapacityBytes=0 VolStatus={vs} Slot={sl} \
                 MaxVolJobs=0 MaxVolFiles=0 InChanger=0 VolReadTime=0 \
                 VolWriteTime=0 EndFile={ef} EndBlock={eb} LabelType=0 \
                 MediaId={mid} EncryptionKey=*None* MinBlocksize={minbs} MaxBlocksize={maxbs}\n"
            )
        })
    })
}

/// Find an appendable volume in a pool.
fn query_appendable_volume(url: &Option<String>, pool_name: &str) -> Option<String> {
    let url = url.as_ref()?;
    let rt = tokio::runtime::Runtime::new().ok()?;
    rt.block_on(async {
        let pool = sqlx::postgres::PgPoolOptions::new().max_connections(1).connect(url).await.ok()?;
        let row: Option<(String,)> = sqlx::query_as(
            "SELECT m.volumename FROM media m JOIN pool p ON m.poolid = p.poolid \
             WHERE p.name = $1 AND m.volstatus = 'Append' ORDER BY m.mediaid LIMIT 1"
        ).bind(pool_name).fetch_optional(&pool).await.ok()?;
        row.map(|(name,)| name)
    })
}

/// Update media record in DB from SD UpdateMedia values.
fn update_media_in_db(url: &Option<String>, request: &str) {
    let Some(url) = url.as_ref() else { return };
    let vol = extract_field(request, "VolName=").unwrap_or_default();
    if vol.is_empty() { return; }

    let mounts: i64 = extract_field(request, "VolMounts=").and_then(|s| s.parse().ok()).unwrap_or(0);
    let bytes: i64 = extract_field(request, "VolBytes=").and_then(|s| s.parse().ok()).unwrap_or(0);
    let jobs: i32 = extract_field(request, "VolJobs=").and_then(|s| s.parse().ok()).unwrap_or(0);
    let files: i64 = extract_field(request, "VolFiles=").and_then(|s| s.parse().ok()).unwrap_or(0);
    let errors: i64 = extract_field(request, "VolErrors=").and_then(|s| s.parse().ok()).unwrap_or(0);
    let writes: i64 = extract_field(request, "VolWrites=").and_then(|s| s.parse().ok()).unwrap_or(0);

    let rt = tokio::runtime::Runtime::new().unwrap();
    let _ = rt.block_on(async {
        let pool = sqlx::postgres::PgPoolOptions::new().max_connections(1).connect(url).await?;
        sqlx::query(
            "UPDATE media SET volmounts=$1, volbytes=$2, voljobs=$3, volfiles=$4, \
             volerrors=$5, volwrites=$6 WHERE volumename=$7"
        )
        .bind(mounts).bind(bytes).bind(jobs).bind(files).bind(errors).bind(writes).bind(&vol)
        .execute(&pool).await
    });
    debug!("CatReq: UpdateMedia DB updated vol={vol} bytes={bytes} mounts={mounts}");
}

fn extract_field(request: &str, prefix: &str) -> Option<String> {
    request.split_whitespace()
        .find(|s| s.starts_with(prefix))
        .map(|s| s[prefix.len()..].to_string())
}

/// Generate bootstrap data for a restore from a given backup JobId.
/// Queries the catalog DB for jobmedia + media to build the BSR.
pub fn generate_bootstrap(config: &Option<SharedConfig>, job_id: u32) -> Option<String> {
    let url = catalog_url(config)?;
    let rt = tokio::runtime::Runtime::new().ok()?;
    rt.block_on(async {
        let pool = sqlx::postgres::PgPoolOptions::new()
            .max_connections(1)
            .connect(&url).await.ok()?;

        // Get job info (VolSessionId, VolSessionTime) from the job table
        let job_row: Option<(i32, i32, String)> = sqlx::query_as(
            "SELECT COALESCE(volsessionid,0)::int, COALESCE(volsessiontime,0)::int, \
             COALESCE(name,'') FROM job WHERE jobid = $1"
        ).bind(job_id as i32).fetch_optional(&pool).await.ok()?;

        let (vol_session_id, vol_session_time, _job_name) = job_row?;

        // Get jobmedia records (which volumes hold this job's data)
        let jm_rows: Vec<(i32, i32, i32, i64, i64)> = sqlx::query_as(
            "SELECT jm.mediaid, COALESCE(jm.firstindex,0)::int, COALESCE(jm.lastindex,0)::int, \
             COALESCE(jm.startblock,0)::bigint, COALESCE(jm.endblock,0)::bigint \
             FROM jobmedia jm WHERE jm.jobid = $1 ORDER BY jm.jobmediaid"
        ).bind(job_id as i32).fetch_all(&pool).await.ok()?;

        if jm_rows.is_empty() {
            info!("No jobmedia records for JobId={job_id}");
            return None;
        }

        let mut bsr = String::new();

        for (media_id, first_idx, last_idx, start_block, end_block) in &jm_rows {
            // Get volume name and media type
            let vol_row: Option<(String, String)> = sqlx::query_as(
                "SELECT volumename, COALESCE(mediatype,'File') FROM media WHERE mediaid = $1"
            ).bind(media_id).fetch_optional(&pool).await.ok()?;

            let (vol_name, media_type) = vol_row?;

            // Get storage + device from config (we only have one storage typically)
            let storage_name = "File"; // TODO: resolve from config
            let device_name = "FileStorage"; // TODO: resolve from config

            bsr.push_str(&format!("Storage=\"{storage_name}\"\n"));
            bsr.push_str(&format!("Volume=\"{vol_name}\"\n"));
            bsr.push_str(&format!("MediaType=\"{media_type}\"\n"));
            bsr.push_str(&format!("Device=\"{device_name}\"\n"));
            bsr.push_str(&format!("VolSessionId={vol_session_id}\n"));
            bsr.push_str(&format!("VolSessionTime={vol_session_time}\n"));
            // VolAddr: byte-offsets in the volume (startblock-endblock from jobmedia)
            if *start_block > 0 || *end_block > 0 {
                bsr.push_str(&format!("VolAddr={start_block}-{end_block}\n"));
            }
            if *first_idx > 0 && *last_idx > 0 {
                bsr.push_str(&format!("FileIndex={first_idx}-{last_idx}\n"));
            }
            bsr.push_str(&format!("Count={}\n", last_idx - first_idx + 1));
        }

        debug!("Bootstrap for JobId={job_id}: {} lines", bsr.lines().count());
        Some(bsr)
    })
}

/// Find the most recent successful backup job for a client.
pub fn find_latest_backup_jobid(config: &Option<SharedConfig>, client_name: &str) -> Option<u32> {
    let url = catalog_url(config)?;
    let rt = tokio::runtime::Runtime::new().ok()?;
    rt.block_on(async {
        let pool = sqlx::postgres::PgPoolOptions::new()
            .max_connections(1)
            .connect(&url).await.ok()?;

        let row: Option<(i32,)> = sqlx::query_as(
            "SELECT j.jobid FROM job j \
             JOIN client c ON j.clientid = c.clientid \
             WHERE c.name = $1 AND j.type = 'B' AND j.jobstatus = 'T' \
             ORDER BY j.jobid DESC LIMIT 1"
        ).bind(client_name).fetch_optional(&pool).await.ok()?;

        row.map(|(id,)| id as u32)
    })
}

fn catalog_url(config: &Option<SharedConfig>) -> Option<String> {
    config.as_ref().and_then(|c| c.read().ok()).and_then(|cfg| {
        cfg.catalogs.first().map(|cat| {
            if cat.db_address.is_empty() || cat.db_address == "localhost" || cat.db_address == "127.0.0.1" {
                format!("postgres://{}@localhost/{}?host=/var/run/postgresql", cat.db_user, cat.db_name)
            } else {
                format!("postgres://{}@{}:{}/{}", cat.db_user, cat.db_address, cat.db_port, cat.db_name)
            }
        })
    })
}

/// Job monitor thread — polls all worker pipes periodically.
pub fn monitor_loop(
    manager: Arc<Mutex<JobManager>>,
    shutdown: &AtomicBool,
) {
    debug!("job monitor started");
    while !shutdown.load(Ordering::Relaxed) {
        {
            let mut mgr = manager.lock().unwrap();
            mgr.poll_all();
        }
        thread::sleep(std::time::Duration::from_millis(10));
    }
    debug!("job monitor stopped");
}

fn now_unix() -> i64 {
    std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .unwrap_or_default().as_secs() as i64
}
