# .jobs — List Job Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one job resource name per line from the Director configuration.

## Syntax

```
.jobs [type=<jobtype>] [enabled|disabled]
```

## Arguments

| Argument | Description |
|----------|-------------|
| *(none)* | List all jobs |
| `type=<jobtype>` | Filter by job type (Backup, Restore, Verify, Admin, etc.) |
| `enabled` | List only enabled jobs |
| `disabled` | List only disabled jobs |

## Examples

### List all jobs
```
* .jobs
backup-bareos-fd
backup-webserver
RestoreFiles
```

### Filter by type
```
* .jobs type=Backup
backup-bareos-fd
backup-webserver
```

### List only enabled jobs
```
* .jobs enabled
backup-bareos-fd
RestoreFiles
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- The `type=` filter is case-insensitive.
- Used by console programs to populate the `job=` argument completion list.

## Related Commands

- [run](run.md) -- Run a job
- [.defaults](dot-defaults.md) -- Show default values for a job
- [.types](dot-types.md) -- List all job types
- [.jobdefs](dot-jobdefs.md) -- List JobDefs names
- [.clients](dot-clients.md) -- List client names
