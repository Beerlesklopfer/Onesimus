# prune -- Prune Expired Records from Catalog

Remove expired file, job, or volume records from the catalog based on configured retention periods.

## Syntax

```
prune [files|jobs|volumes] [client=<name>] [pool=<name>] [volume=<name>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `files` | Prune expired file records |
| `jobs` | Prune expired job records |
| `volumes` | Prune expired volume records |
| `client=<name>` | Restrict pruning to this client |
| `pool=<name>` | Restrict pruning to this pool |
| `volume=<name>` | Prune records for this specific volume |

## Examples

### Prune expired file records for a client
```
* prune files client=bareos-fd
Pruning files for client bareos-fd...
Pruned 1,234 files, 5 jobs from catalog.
```

### Prune expired job records
```
* prune jobs client=bareos-fd
Pruning jobs for client bareos-fd...
Pruned 3 jobs from catalog.
```

### Prune expired volume records
```
* prune volumes pool=Full
Pruning volumes in pool Full...
Pruned 2 volumes from catalog.
```

## Behavior

- Prune respects the retention periods configured in the Client, Pool, and Volume resources:
  - `File Retention`: how long to keep file records
  - `Job Retention`: how long to keep job records
  - `Volume Retention`: how long before a volume can be recycled
- Records older than the retention period are removed from the catalog.
- Pruning does not delete physical data from volumes -- it only cleans up the catalog.
- Volumes whose records have all been pruned are set to `Purged` status, making them eligible for reuse.
- Without arguments, prints available sub-commands.
- This is an audited command.

## Related Commands

- [purge](purge.md) -- Forcefully purge all records (ignores retention)
- [delete](delete.md) -- Delete specific records by name/id
- [list](list.md) -- List records before pruning
