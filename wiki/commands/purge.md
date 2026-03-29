# purge -- Purge All Records from a Volume

Forcefully remove all catalog records associated with a volume, job, or client, ignoring retention periods.

## Syntax

```
purge [files jobid=<id>|jobs client=<name>|volume=<name>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `files` | Purge file records for a specific job |
| `jobs` | Purge job records for a specific client |
| `volume=<name>` | Purge all records associated with this volume |
| `jobid=<id>` | Job ID (used with `files`) |
| `client=<name>` | Client name (used with `jobs`) |

## Examples

### Purge a volume
```
* purge volume=Full-0001
Volume "Full-0001" purged from catalog
```

### Purge file records for a job
```
* purge files jobid=10
Purged files from JobId 10
```

### Purge all jobs for a client
```
* purge jobs client=bareos-fd
Purged jobs for client bareos-fd
```

## Behavior

- Unlike `prune`, purge ignores retention periods and immediately removes all records.
- Purging a volume removes all associated file and job records and sets the volume status to `Purged`.
- A purged volume can be relabeled, truncated, or recycled for new backups.
- The physical data on the volume is not affected -- use `truncate` to reclaim disk space.
- **Use with caution**: purged records cannot be recovered, and restore from the affected backups will no longer be possible via the catalog.
- Without arguments, prints available sub-commands.
- This is an audited command.

## Related Commands

- [prune](prune.md) -- Prune records respecting retention periods
- [truncate](truncate.md) -- Truncate a purged volume to reclaim space
- [relabel](relabel.md) -- Relabel a purged volume
- [delete](delete.md) -- Delete individual records
