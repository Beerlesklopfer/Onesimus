# .bvfs_lsfiles -- List Files in BVFS Directory

Lists files at a given path in the BVFS virtual file tree for one or more jobs. Used by the WebUI and restore tools to display backed-up files within a directory.

## Syntax

```
.bvfs_lsfiles jobid=<N> path=<path> [limit=<n>] [offset=<n>]
.bvfs_lsfiles jobid=<N> pathid=<id> [limit=<n>] [offset=<n>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `jobid=<N>` | Job ID (or comma-separated list of job IDs from `.bvfs_get_jobids`). |
| `path=<path>` | Directory path to list files in (e.g., `/etc/`). |
| `pathid=<id>` | Numeric path ID (alternative to `path`). |
| `limit=<n>` | Maximum number of entries to return (for pagination). |
| `offset=<n>` | Number of entries to skip (for pagination). |

## Examples

### List files in /etc/
```
* .bvfs_lsfiles jobid=1,15,28,42 path=/etc/
lsfiles jobid=1,15,28,42 path=/etc/
```

### List with pagination
```
* .bvfs_lsfiles jobid=42 path=/home/user/ limit=100 offset=0
lsfiles jobid=42 path=/home/user/
```

### Using pathid
```
* .bvfs_lsfiles jobid=42 pathid=5
lsfiles jobid=42 path=5
```

## Notes

- Requires `.bvfs_update` to have been run for the specified job IDs first.
- Output format is tab-separated: `pathid \t fileid \t jobid \t lstat \t name`.
- The `fileid` in the output is used to select files for restore via `.bvfs_restore`.
- Only files (not directories) are returned; use `.bvfs_lsdirs` for directories.
- When using multiple job IDs, the most recent version of each file across the job chain is shown.
- Pagination via `limit` and `offset` is recommended for directories with many files.

## Related Commands

- [.bvfs_lsdirs](dot-bvfs-lsdirs.md) -- List directories (companion command)
- [.bvfs_versions](dot-bvfs-versions.md) -- Show all versions of a specific file
- [.bvfs_restore](dot-bvfs-restore.md) -- Restore selected files by fileid
- [.bvfs_update](dot-bvfs-update.md) -- Update cache (prerequisite)
- [.bvfs_get_jobids](dot-bvfs-get-jobids.md) -- Get job IDs for the jobid argument
