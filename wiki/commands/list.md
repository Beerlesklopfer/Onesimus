# list — List Catalog Records

Query the PostgreSQL catalog for job, volume, client, pool, and file records.

## Syntax

```
list <what> [limit=<N>] [jobid=<N>] [job=<name>]
```

## Sub-Commands

| What | Description | Source |
|------|-------------|--------|
| `jobs` | Job records (default) | `vw_job_summary` view |
| `jobid=<N>` | Single job by ID | `job` table |
| `job=<name>` | Jobs by name | `job` table |
| `volumes` / `media` | Volume records | `vw_volume_status` view |
| `clients` | Client records | `client` table |
| `pools` | Pool records | `pool` table |
| `files` | File records | `vw_file_list` view |
| `storages` | Storage records | `storage` table |
| `schedules` | Schedule records | `schedule` table |
| `filesets` | FileSet resources | config (not DB) |
| `jobtotals` | Summary statistics | `job` table (aggregate) |

## Examples

### List recent jobs
```
* list jobs
jobid  jobname           type level jobstatus clientname  starttime            endtime              jobfiles jobbytes
10     backup-bareos-fd  B    F     T         bareos-fd   2026-03-28 21:20:40  2026-03-28 21:20:42  458      71683878
9      backup-bareos-fd  B    F     T         bareos-fd   2026-03-28 21:18:16  2026-03-28 21:18:18  458      71683878
```

### List specific job
```
* list jobid=10
jobid  name              type level jobstatus starttime            endtime              jobfiles jobbytes  joberrors
10     backup-bareos-fd  B    F     T         2026-03-28 21:20:40  2026-03-28 21:20:42  458      71683878  0
```

### List with limit
```
* list jobs limit=3
```

### List volumes with fill percentage
```
* list volumes
mediaid volumename volstatus volbytes   voljobs volfiles mediatype poolname pct_full
1       Full-0001  Append    839002751  4       0        File      Full     0.0
```

### List files from a backup
```
* list files
fileid jobid filepath
902    2     /usr/sbin/
901    2     /usr/sbin/bareos-dird
900    2     /usr/sbin/bconsole
```

### List filesets (from config, not DB)
```
* list filesets
Name                      Include                                  Exclude
SelfTest                  /usr/sbin                                -
LinuxAll                  /                                        /proc, /tmp, /sys
```

### Job totals
```
* list jobtotals
jobs  files  bytes
10    4580   716838780
```

## Job Status Codes

| Code | Meaning |
|------|---------|
| `C` | Created (not yet running) |
| `R` | Running |
| `T` | Terminated successfully |
| `E` | Error |
| `f` | Fatal error |
| `W` | Warnings |
| `A` | Canceled by user |

## Related Commands

- [llist](llist.md) — Same as list (alias)
- [sqlquery](sqlquery.md) — Raw SQL queries
- [.sql](dot-sql.md) — Dot-command SQL interface
