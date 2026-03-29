# Onesimus Director -- Critical Review & Test Plan

Date: 2026-03-29
Reviewer: KRITIKER (automated adversarial review)

---

## Executive Summary

The Onesimus Director has functional backup and restore pipelines. However,
a large number of commands that an admin would use in production **lie about
their results**. They accept input, print convincing success messages, and
do absolutely nothing. A backup system that says "purged" when it has not
purged is more dangerous than one that says "not implemented", because the
admin builds false trust.

**Verdict: NOT production-ready.** The core data path works, but the
management plane is a Potemkin village.

---

## 1. DANGEROUS GAPS -- Commands That Lie

These commands return success messages without performing the actual operation.
An admin who relies on them will accumulate silent failures.

### 1.1 CRITICAL SEVERITY (data loss / integrity risk)

| Command | What It Says | What It Actually Does | Risk |
|---------|-------------|----------------------|------|
| `cancel jobid=N` | "Job N marked for cancellation" | Posts a bus message. **Never calls `job_manager.cancel_job()`**. SIGTERM is never sent to the worker process. | Runaway jobs cannot be stopped. A stuck backup holds SD device locks forever. Admin thinks it is canceled. |
| `purge files jobid=N` | "Purged files from JobId N" | Returns hardcoded string. No catalog DELETE. No DB interaction whatsoever. | Catalog grows without bound. Admin believes disk space was reclaimed when it was not. |
| `purge jobs client=X` | "Purged jobs for client X" | Same -- pure string. No DB. | Job records accumulate. Volume retention logic (if implemented) sees stale data. |
| `purge volume=V` | "Volume V purged from catalog" | No DB update. Volume status unchanged. | Volume cannot be reused/recycled. SD thinks volume is still in use. |
| `prune files` | "Pruned 0 files, 0 jobs" | Always returns "0". No retention check. No DB query. | Files past retention are never pruned. Catalog grows indefinitely. |
| `prune jobs` | "Pruned 0 jobs" | Same. Hardcoded "0". | Same. |
| `prune volumes` | "Pruned 0 volumes" | Same. | Volumes past retention never transition to Purged status. Recycling breaks. |
| `delete volume=V` | "Volume V deleted from catalog" | No DB interaction. | Ghost volumes in catalog. Subsequent label/mount operations reference non-existent media. |
| `delete jobid=N` | "JobId N deleted from catalog" | No DB interaction. | Admin cannot clean up failed jobs. Catalog integrity degrades. |
| `update volume=V` | "Volume V updated in catalog" | No DB interaction. | Volume attributes (slot, inchanger, volstatus, maxbytes) cannot be corrected. |
| `update pool=P` | "Pool P updated from resource" | No DB interaction. | Pool attributes in DB diverge from config. New volumes get wrong defaults. |
| `label volume=V storage=S` | "Label command sent to SD" | Never connects to SD. No TLS-PSK. No label protocol. | Volumes cannot be labeled. New tape/disk volumes are unusable. |
| `truncate volume=V` | "Volume V truncated" | Never connects to SD. | Purged volumes retain their data on disk. Disk space is never reclaimed. |
| `reload` | "reloaded" + resource counts | **Never re-reads the config file.** Just dumps current in-memory counts. | Config changes require full daemon restart. Admin believes hot-reload works. |

### 1.2 HIGH SEVERITY (operational risk)

| Command | What It Says | What It Actually Does | Risk |
|---------|-------------|----------------------|------|
| `mount storage=S` | "Device S mounted" | Never connects to SD. | Tape cannot be mounted. Admin must use SD console directly. |
| `unmount storage=S` | "Device S unmounted" | Never connects to SD. | Tape remains mounted. Eject fails. Physical media handling breaks. |
| `release storage=S` | "Device S released" | Never connects to SD. | SD device stays locked to previous job. |
| `relabel volume=V` | "Relabeling volume V" | No SD interaction. | Volume name conflicts. Old label remains. |
| `estimate job=J` | "Estimate pending" | Admits it needs FD but never connects. | Admins cannot estimate backup size before running. |
| `rerun jobid=N` | "Rerunning job N" | No job is actually started. | Failed jobs cannot be retried without manually using `run`. |
| `create` (pool) | "Pool created from resource" | No DB interaction. | Pool exists in config but not in catalog. Jobs referencing pool fail. |
| `add` (volume to pool) | "Volume added to pool" | No DB interaction. | Same -- media record missing from catalog. |
| `setdebug level=N` | "Debug level set to N" | No effect on tracing/logging. | Cannot diagnose issues in production. |
| `setbandwidth limit=N` | "Bandwidth limit set to N" | No effect. Not passed to FD/SD. | Network throttling impossible. |
| `setip` | "Client address updated" | No config or DB change. | Dynamic client IP update broken. |
| `resolve client=X` | "Resolving X: pending" | No DNS resolution. | Cannot verify name resolution from director. |

### 1.3 MEDIUM SEVERITY (WebUI / automation risk)

| Command | What It Says | What It Actually Does | Risk |
|---------|-------------|----------------------|------|
| `update slots` | "Updating slots... Done." | Never queries autochanger. | Slot-to-volume mapping is unknown. Barcode scans impossible. |
| `import storage=S` | "Import from storage S" | Nothing. | Autochangers: tapes cannot be imported from I/E slot. |
| `export storage=S` | "Export from storage S" | Nothing. | Tapes cannot be exported to I/E slot. |
| `move srcslot=X dstslot=Y` | "Moving media from slot X to Y" | Nothing. | Autochanger slot operations broken. |
| `.bvfs_update` | "Updating BVFS cache... Done." | No catalog interaction. | WebUI restore file browser shows nothing or stale data. |
| `.bvfs_lsdirs` / `.bvfs_lsfiles` | Returns format string, no data | No catalog query. | WebUI file restore browsing is completely non-functional. |
| `.bvfs_restore` | Returns format string | No restore table created. | WebUI-initiated restores fail. |
| `.bvfs_versions` | Returns format string | No catalog query. | File version selection in WebUI broken. |
| `.acl set/delete/clear/copy` | Success messages | No ACL data stored anywhere. | Console access control is unenforced. Every console has full access. |
| `.dot_authorized` | "authorized" | Checks resource existence, not ACL. `cmd` always returns true. | Any console can run any command regardless of ACL restrictions. |
| `.dot_media` | Empty string | No catalog query. | Volume listing from `.media` returns nothing. |
| `configure add/update/delete` | Success messages, modifies in-memory config | **Never persisted to disk.** Lost on restart. | Admin adds a client via `configure add`, restarts daemon, client is gone. |

---

## 2. DATA INTEGRITY RISKS

### 2.1 Catalog Divergence

The catalog (PostgreSQL) and in-memory config can diverge silently:
- `configure add client` creates in-memory only -- no DB record
- `update pool` says "updated from resource" but DB pool attributes unchanged
- `purge`/`prune` say they cleaned up but DB retains all records
- Volume status transitions (Append -> Full -> Purged -> Recycled) only happen
  partially through the backup path; manual corrections via `update volume` are fake

### 2.2 Volume Lifecycle is Broken

The complete volume lifecycle requires these commands to actually work:
1. `label` -- create volume (FAKE)
2. backup writes to volume (WORKS)
3. `update volume volstatus=Full` -- mark full (FAKE)
4. `prune volume` -- check retention (FAKE)
5. `purge volume` -- mark purgeable (FAKE)
6. `truncate volume` -- reclaim disk space (FAKE)
7. `relabel volume` -- reuse (FAKE)

Only step 2 works. An admin cannot manage the volume lifecycle at all.

### 2.3 Retention is Non-Functional

- `prune` always returns "0 pruned" regardless of retention settings
- Volume Retention, File Retention, Job Retention from pool config are parsed but
  never evaluated
- AutoPrune from pool config is parsed but never triggered
- Result: catalog grows forever, volumes are never recycled

### 2.4 Cancel is a Lie

`cmd_cancel` in `job.rs` line 218-237:
- Posts `BusMessage::JobStatus { status: "Canceled" }` to the message bus
- Has `// TODO: Send SIGTERM to job worker process`
- **Never calls `ctx.job_manager.cancel_job()`** which is the actual method
  that sends SIGTERM via `libc::kill(pid, SIGTERM)`
- The worker process continues running obliviously
- The job appears canceled in `messages` output but is still executing

This is the single most dangerous bug. An admin who cancels a job that is
overwriting files during restore has NO way to stop it.

---

## 3. PROTOCOL INCOMPATIBILITIES

### 3.1 Incremental/Differential Backup

`cmd_run` (job.rs line 69-72) silently upgrades ALL non-full levels to Full:
```
if level != "full" {
    info!("Upgrading level '{}' -> 'full'", level);
    level = "full".to_string();
}
```
This means:
- Scheduled incremental backups run as full backups (massive storage waste)
- Scheduled differential backups run as full backups
- The job record in the catalog says level=F when the schedule said level=I
- Restore point calculations based on backup levels are wrong
- **No log warning is visible to the admin** (only tracing::info, not a console message)

### 3.2 Reload Does Not Reload

C++ Bareos `reload` re-reads all config files and applies changes.
Onesimus `reload` (admin.rs line 449-469) just reads the current in-memory
config and prints counts. The SIGHUP handler in `reload.rs` IS implemented
and DOES re-read config, but the `reload` command does not trigger it.

### 3.3 Wait Does Not Wait

C++ Bareos `wait` blocks until all running jobs complete.
Onesimus `wait` (extra.rs line 162-164) returns immediately with
"No jobs running or queued." regardless of actual job state.

### 3.4 .api Mode Not Applied

`cmd_dot_api` returns "api: mode=N" but the mode value is not stored or
applied to the session. Output format does not change. WebUI expects
`.api 2` to switch to JSON output -- this does not happen.

### 3.5 Status Director -- Missing Running/Scheduled Jobs

`status_director` (status.rs line 127-129) always shows:
```
Scheduled Jobs:
  No scheduled jobs.
Running Jobs:
  No running jobs.
```
Even when jobs ARE running and schedules ARE active. The scheduler and
job_manager have this data but status_director does not query them.

### 3.6 Console ACL Not Enforced

`.authorized` checks resource existence, not whether the current console
has permission. `cmd` type always returns true (dot.rs line 297).
A restricted console can execute any command.

---

## 4. MINIMUM COMMAND SET FOR PRODUCTION

These commands MUST actually work before any production deployment:

### Tier 1 -- Without these, the system is dangerous:
- [ ] `cancel` -- must send SIGTERM (connect to job_manager)
- [ ] `prune` -- must query catalog with retention, delete expired records
- [ ] `purge` -- must DELETE from catalog tables
- [ ] `delete` -- must DELETE from catalog
- [ ] `label` -- must connect to SD and run label protocol
- [ ] `reload` -- must re-read config files (trigger SIGHUP or inline reload)
- [ ] `update volume` -- must UPDATE media table in catalog
- [ ] `status dir` -- must show actual running/scheduled jobs

### Tier 2 -- Without these, daily operations break:
- [ ] `update pool` -- must sync pool config to catalog
- [ ] `truncate` -- must connect to SD and truncate volume file
- [ ] `mount`/`unmount` -- must connect to SD
- [ ] `create` (pool) -- must INSERT pool record in catalog
- [ ] `wait` -- must block until no running jobs
- [ ] `configure` persistence -- must write changes to config files
- [ ] `.api 2` -- must switch session to JSON output (WebUI depends on this)
- [ ] Incremental/Differential -- must compute since_utime, not force Full
- [ ] `.bvfs_*` -- must query catalog (WebUI restore browsing depends on this)

### Tier 3 -- Without these, automation/monitoring breaks:
- [ ] `.authorized` -- must check console ACLs
- [ ] `estimate` -- must connect to FD
- [ ] `rerun` -- must actually start a new job
- [ ] `setdebug` -- must change log level
- [ ] ACL enforcement across all commands

---

## 5. NEGATIVE TEST CASES

Tests that should FAIL but currently succeed (false positives):

### 5.1 Cancel Without Effect
```
# Start a long backup
run job=BackupClient1 level=full
# Note the JobId
cancel jobid=<N>
# EXPECTED: Job worker receives SIGTERM, backup stops
# ACTUAL: "marked for cancellation" but backup continues to completion
# VERIFY: Check if worker PID is still running after cancel
```

### 5.2 Purge Without Purging
```
purge files jobid=1
# EXPECTED: File records deleted from catalog (SELECT COUNT(*) FROM file WHERE jobid=1 returns 0)
# ACTUAL: "Purged files from JobId 1" but records still exist
# VERIFY: .sql query="SELECT COUNT(*) FROM file WHERE jobid=1"
```

### 5.3 Prune Ignores Retention
```
# Set pool retention to 1 second, run a backup, wait 2 seconds
prune files client=bareos-fd
# EXPECTED: Files from expired jobs pruned, count > 0
# ACTUAL: Always "Pruned 0 files, 0 jobs from catalog"
# VERIFY: Records still exist in DB
```

### 5.4 Delete Does Not Delete
```
# After a backup completes:
delete jobid=<N>
list jobid=<N>
# EXPECTED: "No records found" or error
# ACTUAL: delete says "deleted", list still shows the job
```

### 5.5 Label Never Labels
```
label volume=TestVol001 storage=File pool=Full
# EXPECTED: SD creates volume file on disk
# ACTUAL: "Label command sent to SD" but no file created
# VERIFY: ls /var/lib/bareos/storage/ -- no TestVol001
```

### 5.6 Update Volume Is Cosmetic
```
update volume=Vol-0001 volstatus=Purged
.sql query="SELECT volstatus FROM media WHERE volumename='Vol-0001'"
# EXPECTED: volstatus = 'Purged'
# ACTUAL: volstatus unchanged (still 'Append' or 'Full')
```

### 5.7 Reload Does Not Reload
```
# Edit config file to add a new client
reload
show clients
# EXPECTED: New client appears
# ACTUAL: Old client list (reload did not re-read file)
```

### 5.8 Wait Returns Immediately
```
run job=BackupClient1 level=full
wait
# EXPECTED: Blocks until backup completes
# ACTUAL: Returns immediately "No jobs running or queued"
```

### 5.9 Configure Changes Lost on Restart
```
configure add client name=NewClient address=10.0.0.5 password=secret
show clients
# Shows NewClient -- good
# Restart daemon
show clients
# EXPECTED: NewClient still present
# ACTUAL: NewClient gone (never written to disk)
```

### 5.10 Incremental Runs As Full
```
run job=BackupClient1 level=incremental
list jobs
# EXPECTED: Level column shows 'I' (Incremental)
# ACTUAL: Level shows 'F' (Full) -- silently upgraded
# IMPACT: 10x more storage used than expected
```

### 5.11 BVFS Returns No Data
```
# After a successful backup:
.bvfs_update jobid=<N>
.bvfs_lsdirs jobid=<N> path=/
# EXPECTED: Directory listing from backup
# ACTUAL: "lsdirs jobid=N path=/" -- just echoes parameters
```

### 5.12 ACL Bypass
```
# Connect as restricted console (e.g., webui-consol with limited JobACL)
.authorized cmd=sqlquery
# EXPECTED: "not authorized" (restricted console should not have SQL access)
# ACTUAL: "authorized" (cmd type always returns true)
sqlquery SELECT * FROM client
# EXPECTED: Permission denied
# ACTUAL: Returns all client data
```

### 5.13 Truncate Without Truncating
```
# After purging a volume:
truncate volume=Vol-0001 storage=File
# Check file size on disk
# EXPECTED: Volume file truncated to 0 bytes
# ACTUAL: File unchanged, original size retained
```

### 5.14 Status Shows Phantom State
```
run job=BackupClient1 level=full
status dir
# EXPECTED: "Running Jobs: JobId=N BackupClient1 ..."
# ACTUAL: "Running Jobs: No running jobs." (while job IS running)
```

---

## 6. RECOMMENDATIONS

1. **Immediate**: Make every fake command return an error with "NOT IMPLEMENTED"
   instead of a success message. A backup system must never lie. The current
   behavior is worse than not having the command at all.

2. **Immediate**: Fix `cancel` -- one-line fix: call
   `ctx.job_manager.lock().unwrap().cancel_job(id)` instead of just posting
   a bus message. This is a safety issue.

3. **Short-term**: Implement prune/purge/delete against the catalog. These are
   straightforward SQL DELETEs with retention logic.

4. **Short-term**: Fix `reload` to either trigger SIGHUP or inline the reload
   logic from `reload.rs`.

5. **Short-term**: Fix `status dir` to query job_manager for running jobs and
   scheduler for scheduled jobs.

6. **Medium-term**: Implement SD protocol for label/mount/unmount/truncate.
   Without these, volume management is manual.

7. **Medium-term**: Implement BVFS against catalog. Without this, WebUI
   restore browsing is non-functional.

8. **Principle**: Every command should either (a) do the thing, or
   (b) return an error saying it cannot do the thing. Never (c) pretend
   to do the thing while doing nothing.

---

## Appendix: Command Classification

### Actually Implemented (do real work):
- `run` (backup) -- full pipeline with SD+FD
- `restore` -- full pipeline with bootstrap generation
- `status client` -- real FD connection + status query
- `status storage` -- real SD connection + status query
- `list` (with catalog) -- real SQL queries
- `show` -- reads config (in-memory, but real)
- `enable`/`disable` -- toggles in-memory config (not persisted)
- `.defaults`, `.clients`, `.jobs`, `.pools`, etc. -- config queries (real)
- `.sql` / `sqlquery` -- real SQL execution
- `.catalog` -- real schema management
- `configure add/delete/update` -- modifies in-memory config (not persisted)
- `messages` -- real bus message consumption
- `help` / `.help` -- real command/arg listings

### Pure Theater (say OK, do nothing):
- `cancel`, `purge`, `prune`, `delete`, `truncate`
- `label`, `relabel`, `mount`, `unmount`, `release`
- `update volume`, `update pool`, `update slots`
- `reload`, `wait`, `rerun`, `create`, `add`
- `import`, `export`, `move`
- `estimate`, `resolve`
- `setdebug`, `setbandwidth`, `setip`, `setdevice`
- `.bvfs_*` (all 8 commands)
- `.acl set/delete/clear/copy`
- `trace`, `var`, `autodisplay`, `automount`, `gui`
