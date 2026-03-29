# delete -- Delete Volume or Job Records

Remove volume or job records from the catalog database. This does not delete the physical volume data.

## Syntax

```
delete [volume=<name>|pool=<name>|jobid=<id>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `volume=<name>` | Delete this volume record from the catalog |
| `pool=<name>` | Delete all volumes in this pool from the catalog |
| `jobid=<id>` | Delete this job record and all associated file records |

## Examples

### Delete a volume from catalog
```
* delete volume=Full-0001
Volume Full-0001 deleted from catalog
```

### Delete a job record
```
* delete jobid=10
JobId 10 deleted from catalog
```

### Without arguments
```
* delete
delete: specify volume=<name> or jobid=<id>
```

## Behavior

- `delete volume`: removes the media record from the catalog. The physical volume file/tape is not affected.
- `delete jobid`: removes the job record and all associated file and jobmedia records from the catalog.
- Deleted records cannot be recovered -- use with caution.
- The volume must not be in use by a running job.
- For removing expired records based on retention periods, use `prune` instead.
- This is an audited command.

## Related Commands

- [prune](prune.md) -- Prune expired records based on retention
- [purge](purge.md) -- Purge all records from a volume
- [list](list.md) -- List volumes and jobs before deleting
