# .bvfs_update -- Update BVFS Cache

Populates or refreshes the BVFS (Bareos Virtual File System) cache for one or all jobs. The BVFS cache materializes the file/directory tree from the catalog so that restore browsing (via `.bvfs_lsdirs`, `.bvfs_lsfiles`) can work efficiently. Must be called before browsing files from a specific job.

## Syntax

```
.bvfs_update [jobid=<N>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `jobid=<N>` | Job ID to update the cache for. If omitted, all jobs are updated. |

## Examples

### Update cache for a specific job
```
* .bvfs_update jobid=42
Updating BVFS cache for JobId 42...
Done.
```

### Update cache for all jobs
```
* .bvfs_update
Updating BVFS cache for all jobs...
Done.
```

### Typical restore workflow: update then browse
```
* .bvfs_update jobid=10
Updating BVFS cache for JobId 10...
Done.
* .bvfs_lsdirs jobid=10 path=/
```

## Notes

- This is typically the first step in the BVFS restore workflow: update -> browse dirs/files -> select -> restore.
- Updating all jobs (no `jobid=` argument) can be slow on large catalogs.
- The cache is stored in temporary tables in the catalog database.
- The WebUI calls this automatically before presenting the restore file browser.
- If the cache is already up to date for a job, this is a no-op.

## Related Commands

- [.bvfs_lsdirs](dot-bvfs-lsdirs.md) -- Browse directories after cache update
- [.bvfs_lsfiles](dot-bvfs-lsfiles.md) -- Browse files after cache update
- [.bvfs_get_jobids](dot-bvfs-get-jobids.md) -- Get the job chain before updating
- [.bvfs_clear_cache](dot-bvfs-clear-cache.md) -- Clear the entire BVFS cache
- [.bvfs_cleanup](dot-bvfs-cleanup.md) -- Cleanup temporary restore tables
