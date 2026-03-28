//! Catalog Worker — single DB connection pool shared via MPSC channel.
//!
//! All database access goes through `CatalogHandle::request()`. The worker
//! thread owns the only `PgPool` and processes requests sequentially via
//! `flume::recv_async().await`. This replaces the previous pattern of creating
//! a throwaway `PgPoolOptions::new().max_connections(1)` + `Runtime::new()`
//! for every single query.
//!
//! Architecture:
//! ```text
//! Session/Worker (sync)  ──flume::Sender<(Req, oneshot)>──→  dir:catalog (async tokio)
//!                        ←──────flume::bounded(1)────────────┘
//! ```

use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};
use tracing::{info, debug, error, warn};

// ---------------------------------------------------------------------------
// Request / Response types
// ---------------------------------------------------------------------------

/// A catalog request sent through the MPSC channel.
pub enum CatalogRequest {
    /// Raw SQL query — returns formatted text output.
    RawQuery { sql: String },
    /// Get full volume info in OK_media format (24-field protocol string).
    GetVolumeInfo { vol_name: String },
    /// Find an appendable volume in a named pool.
    FindAppendableVolume { pool_name: String },
    /// Update media record from SD UpdateMedia values.
    UpdateMedia {
        vol_name: String,
        mounts: i64,
        bytes: i64,
        jobs: i32,
        files: i64,
        errors: i64,
        writes: i64,
    },
    /// Generate bootstrap data for a restore.
    GenerateBootstrap { job_id: u32 },
    /// Find the most recent successful backup job for a client.
    FindLatestBackupJobId { client_name: String },
    /// Ping / health check.
    Ping,
}

/// Response from the catalog worker.
pub enum CatalogResponse {
    /// Text output (query results, formatted strings).
    Text(String),
    /// Optional text (volume info, bootstrap, etc.).
    OptionalText(Option<String>),
    /// Optional job ID.
    OptionalJobId(Option<u32>),
    /// Success with rows affected.
    Ok(u64),
    /// Error message.
    Error(String),
    /// Pong.
    Pong,
}

// ---------------------------------------------------------------------------
// CatalogHandle — cloneable sender side
// ---------------------------------------------------------------------------

/// Cloneable handle for sending DB requests. All methods are blocking.
#[derive(Clone)]
pub struct CatalogHandle {
    tx: flume::Sender<(CatalogRequest, flume::Sender<CatalogResponse>)>,
}

impl CatalogHandle {
    /// Send a request and wait for the response (blocking).
    fn request(&self, req: CatalogRequest) -> Result<CatalogResponse, String> {
        let (resp_tx, resp_rx) = flume::bounded(1);
        self.tx.send((req, resp_tx))
            .map_err(|_| "catalog worker shut down".to_string())?;
        resp_rx.recv()
            .map_err(|_| "catalog worker dropped response".to_string())
    }

    /// Execute a raw SQL query, returning formatted text.
    pub fn raw_query(&self, sql: &str) -> Result<String, String> {
        match self.request(CatalogRequest::RawQuery { sql: sql.to_string() })? {
            CatalogResponse::Text(t) => Ok(t),
            CatalogResponse::Error(e) => Err(e),
            other => Err(format!("unexpected response: {:?}", resp_debug(&other))),
        }
    }

    /// Get full volume info in OK_media format.
    pub fn get_volume_info(&self, vol_name: &str) -> Option<String> {
        match self.request(CatalogRequest::GetVolumeInfo { vol_name: vol_name.to_string() }) {
            Ok(CatalogResponse::OptionalText(t)) => t,
            _ => None,
        }
    }

    /// Find an appendable volume in a pool.
    pub fn find_appendable_volume(&self, pool_name: &str) -> Option<String> {
        match self.request(CatalogRequest::FindAppendableVolume { pool_name: pool_name.to_string() }) {
            Ok(CatalogResponse::OptionalText(t)) => t,
            _ => None,
        }
    }

    /// Update media record in DB.
    pub fn update_media(
        &self, vol_name: &str, mounts: i64, bytes: i64, jobs: i32,
        files: i64, errors: i64, writes: i64,
    ) {
        let _ = self.request(CatalogRequest::UpdateMedia {
            vol_name: vol_name.to_string(), mounts, bytes, jobs, files, errors, writes,
        });
    }

    /// Generate bootstrap data for a restore.
    pub fn generate_bootstrap(&self, job_id: u32) -> Option<String> {
        match self.request(CatalogRequest::GenerateBootstrap { job_id }) {
            Ok(CatalogResponse::OptionalText(t)) => t,
            _ => None,
        }
    }

    /// Find the most recent successful backup job for a client.
    pub fn find_latest_backup_jobid(&self, client_name: &str) -> Option<u32> {
        match self.request(CatalogRequest::FindLatestBackupJobId { client_name: client_name.to_string() }) {
            Ok(CatalogResponse::OptionalJobId(id)) => id,
            _ => None,
        }
    }

    /// Health check.
    pub fn ping(&self) -> bool {
        matches!(self.request(CatalogRequest::Ping), Ok(CatalogResponse::Pong))
    }
}

fn resp_debug(r: &CatalogResponse) -> &'static str {
    match r {
        CatalogResponse::Text(_) => "Text",
        CatalogResponse::OptionalText(_) => "OptionalText",
        CatalogResponse::OptionalJobId(_) => "OptionalJobId",
        CatalogResponse::Ok(_) => "Ok",
        CatalogResponse::Error(_) => "Error",
        CatalogResponse::Pong => "Pong",
    }
}

// ---------------------------------------------------------------------------
// Worker thread
// ---------------------------------------------------------------------------

/// Spawn the catalog worker. Returns the CatalogHandle for callers.
pub fn spawn_catalog_worker(
    catalog_url: String,
    shutdown: &'static AtomicBool,
) -> (CatalogHandle, std::thread::JoinHandle<()>) {
    let (tx, rx) = flume::bounded::<(CatalogRequest, flume::Sender<CatalogResponse>)>(256);
    let handle = CatalogHandle { tx };

    let join = std::thread::Builder::new()
        .name("dir:catalog".into())
        .spawn(move || {
            let rt = tokio::runtime::Builder::new_current_thread()
                .enable_all()
                .build()
                .expect("catalog worker: tokio runtime");

            rt.block_on(async {
                let pool = match sqlx::postgres::PgPoolOptions::new()
                    .max_connections(8)
                    .connect(&catalog_url)
                    .await
                {
                    Ok(p) => p,
                    Err(e) => {
                        error!("catalog worker: DB connect failed: {e}");
                        return;
                    }
                };

                info!("catalog worker connected (pool max=8)");

                while let Ok((req, resp_tx)) = rx.recv_async().await {
                    if shutdown.load(Ordering::Relaxed) { break; }

                    let response = match req {
                        CatalogRequest::RawQuery { sql } =>
                            handle_raw_query(&pool, &sql).await,
                        CatalogRequest::GetVolumeInfo { vol_name } =>
                            handle_get_volume_info(&pool, &vol_name).await,
                        CatalogRequest::FindAppendableVolume { pool_name } =>
                            handle_find_appendable_volume(&pool, &pool_name).await,
                        CatalogRequest::UpdateMedia { vol_name, mounts, bytes, jobs, files, errors, writes } =>
                            handle_update_media(&pool, &vol_name, mounts, bytes, jobs, files, errors, writes).await,
                        CatalogRequest::GenerateBootstrap { job_id } =>
                            handle_generate_bootstrap(&pool, job_id).await,
                        CatalogRequest::FindLatestBackupJobId { client_name } =>
                            handle_find_latest_backup_jobid(&pool, &client_name).await,
                        CatalogRequest::Ping => CatalogResponse::Pong,
                    };

                    let _ = resp_tx.send(response);
                }

                pool.close().await;
                info!("catalog worker stopped");
            });
        })
        .expect("failed to spawn dir:catalog");

    (handle, join)
}

// ---------------------------------------------------------------------------
// Async handlers — one per CatalogRequest variant
// ---------------------------------------------------------------------------

async fn handle_raw_query(pool: &sqlx::PgPool, sql: &str) -> CatalogResponse {
    let rows = match sqlx::query(sql).fetch_all(pool).await {
        Ok(r) => r,
        Err(e) => return CatalogResponse::Error(format!("Query: {e}")),
    };

    use sqlx::{Row, Column};
    let mut output = String::new();

    if rows.is_empty() {
        output.push_str("No results.\n");
        return CatalogResponse::Text(output);
    }

    let columns = rows[0].columns();
    for col in columns {
        output.push_str(&format!("{:<20}", col.name()));
    }
    output.push('\n');
    output.push_str(&"-".repeat(columns.len() * 20));
    output.push('\n');

    for row in &rows {
        for col in columns {
            let val: String = row.try_get::<String, _>(col.ordinal())
                .or_else(|_| row.try_get::<i64, _>(col.ordinal()).map(|v| v.to_string()))
                .or_else(|_| row.try_get::<i32, _>(col.ordinal()).map(|v| v.to_string()))
                .or_else(|_| row.try_get::<bool, _>(col.ordinal()).map(|v| v.to_string()))
                .unwrap_or_else(|_| "NULL".to_string());
            output.push_str(&format!("{:<20}", val));
        }
        output.push('\n');
    }

    CatalogResponse::Text(output)
}

async fn handle_get_volume_info(pool: &sqlx::PgPool, vol_name: &str) -> CatalogResponse {
    let row: Option<(
        String, i32, i64, i64, i64, i64, i64, i64, i64,
        String, i64, i32, i64, i64, i64, i64,
    )> = match sqlx::query_as(
        "SELECT volumename, COALESCE(voljobs,0)::int, COALESCE(volfiles,0)::bigint, \
         COALESCE(volblocks,0)::bigint, COALESCE(volbytes,0)::bigint, COALESCE(volmounts,0)::bigint, \
         COALESCE(volerrors,0)::bigint, COALESCE(volwrites,0)::bigint, COALESCE(maxvolbytes,0)::bigint, \
         COALESCE(volstatus,'Append'), COALESCE(slot,0)::bigint, mediaid, \
         COALESCE(endfile,0)::bigint, COALESCE(endblock,0)::bigint, \
         COALESCE(minblocksize,0)::bigint, COALESCE(maxblocksize,0)::bigint \
         FROM media WHERE volumename = $1"
    ).bind(vol_name).fetch_optional(pool).await {
        Ok(r) => r,
        Err(e) => return CatalogResponse::Error(format!("GetVolumeInfo: {e}")),
    };

    CatalogResponse::OptionalText(row.map(|(vn, vj, vf, vbl, vby, vm, ve, vw, mvb, vs, sl, mid, ef, eb, minbs, maxbs)| {
        format!(
            "1000 OK VolName={vn} VolJobs={vj} VolFiles={vf} \
             VolBlocks={vbl} VolBytes={vby} VolMounts={vm} VolErrors={ve} VolWrites={vw} \
             MaxVolBytes={mvb} VolCapacityBytes=0 VolStatus={vs} Slot={sl} \
             MaxVolJobs=0 MaxVolFiles=0 InChanger=0 VolReadTime=0 \
             VolWriteTime=0 EndFile={ef} EndBlock={eb} LabelType=0 \
             MediaId={mid} EncryptionKey=*None* MinBlocksize={minbs} MaxBlocksize={maxbs}\n"
        )
    }))
}

async fn handle_find_appendable_volume(pool: &sqlx::PgPool, pool_name: &str) -> CatalogResponse {
    let row: Option<(String,)> = match sqlx::query_as(
        "SELECT m.volumename FROM media m JOIN pool p ON m.poolid = p.poolid \
         WHERE p.name = $1 AND m.volstatus = 'Append' ORDER BY m.mediaid LIMIT 1"
    ).bind(pool_name).fetch_optional(pool).await {
        Ok(r) => r,
        Err(e) => return CatalogResponse::Error(format!("FindAppendableVolume: {e}")),
    };

    CatalogResponse::OptionalText(row.map(|(name,)| name))
}

async fn handle_update_media(
    pool: &sqlx::PgPool, vol_name: &str,
    mounts: i64, bytes: i64, jobs: i32, files: i64, errors: i64, writes: i64,
) -> CatalogResponse {
    match sqlx::query(
        "UPDATE media SET volmounts=$1, volbytes=$2, voljobs=$3, volfiles=$4, \
         volerrors=$5, volwrites=$6 WHERE volumename=$7"
    )
    .bind(mounts).bind(bytes).bind(jobs).bind(files).bind(errors).bind(writes).bind(vol_name)
    .execute(pool).await {
        Ok(r) => {
            debug!("UpdateMedia: vol={vol_name} bytes={bytes} mounts={mounts}");
            CatalogResponse::Ok(r.rows_affected())
        }
        Err(e) => CatalogResponse::Error(format!("UpdateMedia: {e}")),
    }
}

async fn handle_generate_bootstrap(pool: &sqlx::PgPool, job_id: u32) -> CatalogResponse {
    // Get job info
    let job_row: Option<(i32, i32, String)> = match sqlx::query_as(
        "SELECT COALESCE(volsessionid,0)::int, COALESCE(volsessiontime,0)::int, \
         COALESCE(name,'') FROM job WHERE jobid = $1"
    ).bind(job_id as i32).fetch_optional(pool).await {
        Ok(r) => r,
        Err(e) => return CatalogResponse::Error(format!("GenerateBootstrap: {e}")),
    };

    let Some((vol_session_id, vol_session_time, _job_name)) = job_row else {
        return CatalogResponse::OptionalText(None);
    };

    // Get jobmedia records
    let jm_rows: Vec<(i32, i32, i32, i64, i64)> = match sqlx::query_as(
        "SELECT jm.mediaid, COALESCE(jm.firstindex,0)::int, COALESCE(jm.lastindex,0)::int, \
         COALESCE(jm.startblock,0)::bigint, COALESCE(jm.endblock,0)::bigint \
         FROM jobmedia jm WHERE jm.jobid = $1 ORDER BY jm.jobmediaid"
    ).bind(job_id as i32).fetch_all(pool).await {
        Ok(r) => r,
        Err(e) => return CatalogResponse::Error(format!("GenerateBootstrap jobmedia: {e}")),
    };

    if jm_rows.is_empty() {
        info!("No jobmedia records for JobId={job_id}");
        return CatalogResponse::OptionalText(None);
    }

    let mut bsr = String::new();

    for (media_id, first_idx, last_idx, start_block, end_block) in &jm_rows {
        let vol_row: Option<(String, String)> = match sqlx::query_as(
            "SELECT volumename, COALESCE(mediatype,'File') FROM media WHERE mediaid = $1"
        ).bind(media_id).fetch_optional(pool).await {
            Ok(r) => r,
            Err(e) => return CatalogResponse::Error(format!("GenerateBootstrap media: {e}")),
        };

        let Some((vol_name, media_type)) = vol_row else {
            continue;
        };

        let storage_name = "File"; // TODO: resolve from config
        let device_name = "FileStorage"; // TODO: resolve from config

        bsr.push_str(&format!("Storage=\"{storage_name}\"\n"));
        bsr.push_str(&format!("Volume=\"{vol_name}\"\n"));
        bsr.push_str(&format!("MediaType=\"{media_type}\"\n"));
        bsr.push_str(&format!("Device=\"{device_name}\"\n"));
        bsr.push_str(&format!("VolSessionId={vol_session_id}\n"));
        bsr.push_str(&format!("VolSessionTime={vol_session_time}\n"));
        if *start_block > 0 || *end_block > 0 {
            bsr.push_str(&format!("VolAddr={start_block}-{end_block}\n"));
        }
        if *first_idx > 0 && *last_idx > 0 {
            bsr.push_str(&format!("FileIndex={first_idx}-{last_idx}\n"));
        }
        bsr.push_str(&format!("Count={}\n", last_idx - first_idx + 1));
    }

    debug!("Bootstrap for JobId={job_id}: {} lines", bsr.lines().count());
    CatalogResponse::OptionalText(Some(bsr))
}

async fn handle_find_latest_backup_jobid(pool: &sqlx::PgPool, client_name: &str) -> CatalogResponse {
    let row: Option<(i32,)> = match sqlx::query_as(
        "SELECT j.jobid FROM job j \
         JOIN client c ON j.clientid = c.clientid \
         WHERE c.name = $1 AND j.type = 'B' AND j.jobstatus = 'T' \
         ORDER BY j.jobid DESC LIMIT 1"
    ).bind(client_name).fetch_optional(pool).await {
        Ok(r) => r,
        Err(e) => return CatalogResponse::Error(format!("FindLatestBackupJobId: {e}")),
    };

    CatalogResponse::OptionalJobId(row.map(|(id,)| id as u32))
}
