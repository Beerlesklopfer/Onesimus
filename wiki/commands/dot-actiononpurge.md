# .actiononpurge — List Action-on-Purge Options

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one action-on-purge option per line.

## Syntax

```
.actiononpurge
```

## Arguments

None.

## Examples

### List all options
```
* .actiononpurge
Truncate
None
```

## Notes

- Output is one option per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Returns a static list: `Truncate` and `None`.
- `Truncate` causes the volume file to be truncated after all jobs on it are purged, reclaiming disk space.
- `None` disables the automatic action (default behavior).
- Used by console programs to populate the `actiononpurge=` argument completion list.

## Related Commands

- [.volstatus](dot-volstatus.md) -- List volume status values
- [.pools](dot-pools.md) -- List pool names
- [purge](purge.md) -- Purge jobs/volumes from catalog
- [truncate](truncate.md) -- Truncate purged volumes
