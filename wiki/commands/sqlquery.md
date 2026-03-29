# sqlquery — Execute Raw SQL

Execute a raw SQL query against the PostgreSQL catalog database.

## Syntax

```
sqlquery <SQL statement>
```

## Examples

### Count jobs
```
* sqlquery SELECT COUNT(*) AS total FROM job
total
--------------------
10
```

### Find large backups
```
* sqlquery SELECT jobid, name, jobbytes FROM job WHERE jobbytes > 50000000 ORDER BY jobbytes DESC
jobid               name                jobbytes
------------------------------------------------------------
10                  backup-bareos-fd    71683878
9                   backup-bareos-fd    71683878
```

### Check volume usage
```
* sqlquery SELECT volumename, volstatus, volbytes, voljobs FROM media
volumename          volstatus           volbytes            voljobs
--------------------------------------------------------------------------------
Full-0001           Append              839002751           4
```

### Use views
```
* sqlquery SELECT * FROM vw_last_backup_per_client
clientname          jobid  jobname           level starttime            endtime              jobfiles jobbytes
------------------------------------------------------------------------------------------------------------------
bareos-fd           10     backup-bareos-fd  F     2026-03-28 21:20:40  2026-03-28 21:20:42  458      71683878
```

### Call stored procedures
```
* sqlquery SELECT fn_find_appendable_volume('Full')
fn_find_appendable_volume
--------------------
Full-0001

* sqlquery SELECT fn_find_latest_backup('bareos-fd')
fn_find_latest_backup
--------------------
10
```

## Available Views

| View | Description |
|------|-------------|
| `vw_job_summary` | Jobs with client + pool names |
| `vw_volume_status` | Volumes with pool name + fill % |
| `vw_file_list` | Files with full path |
| `vw_last_backup_per_client` | Latest successful backup per client |

## Available Functions

| Function | Returns | Description |
|----------|---------|-------------|
| `fn_find_appendable_volume(pool)` | `text` | Find writable volume |
| `fn_find_latest_backup(client)` | `integer` | Latest successful backup JobId |
| `fn_since_utime(client, level)` | `(bigint, text)` | Reference time for incr/diff |

## Security

SQL queries run with the Director's database user privileges. Only SELECT is typically safe. INSERT/UPDATE/DELETE can corrupt the catalog.

## Related Commands

- [.sql](dot-sql.md) — Dot-command SQL (with sub-commands: tables, schema)
- [list](list.md) — Structured catalog queries
- [.sqlcount](dot-sqlcount.md) — Quick record count
