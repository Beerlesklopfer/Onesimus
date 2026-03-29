# .types — List Job Types

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one job type name per line.

## Syntax

```
.types
```

## Arguments

None.

## Examples

### List all job types
```
* .types
Backup
Restore
Verify
Admin
Archive
Copy
Migrate
Consolidate
```

## Notes

- Output is one type name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Returns a static list of all supported job types.
- Used by console programs to populate the `type=` argument completion list and to filter `.jobs type=<T>`.

## Related Commands

- [.jobs](dot-jobs.md) -- List job names (supports `type=` filter)
- [.levels](dot-levels.md) -- List backup levels
- [.jobstatus](dot-jobstatus.md) -- List job status codes
- [run](run.md) -- Run a job
