# .bvfs_versions -- List File Versions

Lists all backed-up versions of a specific file across jobs. Shows which jobs contain which version of the file, along with volume information. Used to select a particular version for restore.

## Syntax

```
.bvfs_versions client=<name> pathid=<id> filename=<name> [copies] [versions]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `client=<name>` | Client name to search versions for. |
| `pathid=<id>` | Numeric path ID of the directory containing the file. |
| `filename=<name>` | File name to list versions for (also accepts `fname=<name>`). |
| `copies` | Optional flag: include copy job versions. |
| `versions` | Optional flag: include all versions (not just the latest per job). |

## Examples

### List versions of a specific file
```
* .bvfs_versions client=bareos-fd pathid=5 filename=hosts
versions client=bareos-fd pathid=5 filename=hosts
```

### Using fname alias
```
* .bvfs_versions client=bareos-fd pathid=5 fname=resolv.conf
versions client=bareos-fd pathid=5 filename=resolv.conf
```

### Include copy job versions
```
* .bvfs_versions client=bareos-fd pathid=5 filename=passwd copies
versions client=bareos-fd pathid=5 filename=passwd
```

## Notes

- Output format is tab-separated: `pathid \t fileid \t jobid \t lstat \t name \t volume \t inchanger`.
- The `pathid` is obtained from `.bvfs_lsdirs` or `.bvfs_lsfiles` output.
- The `fileid` from the output can be used with `.bvfs_restore` to restore a specific version.
- The `client` argument is required to scope the search to a specific client's backups.
- The `copies` flag includes versions from copy jobs (useful when original volumes are unavailable).
- The `lstat` field contains the encoded file attributes (size, mtime, permissions).

## Related Commands

- [.bvfs_lsfiles](dot-bvfs-lsfiles.md) -- List files to find pathid and filename
- [.bvfs_lsdirs](dot-bvfs-lsdirs.md) -- Browse directories to find pathid
- [.bvfs_restore](dot-bvfs-restore.md) -- Restore a specific version by fileid
- [.bvfs_update](dot-bvfs-update.md) -- Update cache (prerequisite)
