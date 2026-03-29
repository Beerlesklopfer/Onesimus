# .media — List Volume/Media Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one volume name per line from the catalog database.

## Syntax

```
.media
```

## Arguments

None.

## Examples

### List all volumes
```
* .media
Full-0001
Full-0002
Incremental-0001
Incremental-0002
```

### No volumes labeled yet
```
* .media
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Requires a catalog database connection. Returns empty output if no catalog is configured.
- Lists volumes (media) registered in the catalog, not just currently mounted ones.
- Used by console programs to populate the `volume=` argument completion list.

## Related Commands

- [.mediatypes](dot-mediatypes.md) -- List media type names
- [.volstatus](dot-volstatus.md) -- List volume status values
- [.pools](dot-pools.md) -- List pool names
- [label](label.md) -- Label a new volume
- [list](list.md) -- List volumes from catalog
