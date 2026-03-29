# .pools — List Pool Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one pool resource name per line from the Director configuration.

## Syntax

```
.pools [type=<pooltype>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| *(none)* | List all pools |
| `type=<pooltype>` | Filter by pool type (e.g., Backup, Scratch) |

## Examples

### List all pools
```
* .pools
Default
Full
Incremental
Scratch
```

### Filter by type
```
* .pools type=Backup
Default
Full
Incremental
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- The `type=` filter is case-insensitive and matches the PoolType directive.
- Used by console programs to populate the `pool=` argument completion list.

## Related Commands

- [.defaults](dot-defaults.md) -- Show default values for a pool
- [.media](dot-media.md) -- List volume names
- [.volstatus](dot-volstatus.md) -- List volume status values
