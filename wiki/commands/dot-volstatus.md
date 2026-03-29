# .volstatus — List Volume Status Values

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one possible volume status value per line.

## Syntax

```
.volstatus
```

## Arguments

None.

## Examples

### List all volume statuses
```
* .volstatus
Append
Archive
Disabled
Error
Full
Purged
Recycle
Read-Only
Used
Cleaning
```

## Notes

- Output is one status name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Returns a static list of all possible VolStatus values in the Media table.
- Used by console programs to populate the `volstatus=` argument completion list (e.g., for `update volume=... volstatus=`).

## Related Commands

- [.media](dot-media.md) -- List volume names
- [.pools](dot-pools.md) -- List pool names
- [update](update.md) -- Update volume attributes
- [list](list.md) -- List volumes from catalog
