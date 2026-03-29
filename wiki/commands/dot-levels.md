# .levels — List Backup Levels

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one backup level name per line.

## Syntax

```
.levels
```

## Arguments

None.

## Examples

### List all levels
```
* .levels
Full
Incremental
Differential
VirtualFull
InitCatalog
Catalog
VolumeToCatalog
DiskToCatalog
```

## Notes

- Output is one level name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Returns a static list of all supported backup levels.
- Used by console programs to populate the `level=` argument completion list (e.g., for `run job=... level=`).

## Related Commands

- [run](run.md) -- Run a job at a specified level
- [.jobs](dot-jobs.md) -- List job names
- [.types](dot-types.md) -- List job types
- [.defaults](dot-defaults.md) -- Show default level for a job
