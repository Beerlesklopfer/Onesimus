# relabel -- Re-label an Existing Volume

Write a new label to an existing volume, effectively recycling it for new data. The old volume data is overwritten.

## Syntax

```
relabel [oldvolume=<name>] [volume=<name>] [storage=<name>] [pool=<name>] [encrypt]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `oldvolume=<name>` | Name of the existing volume to relabel |
| `volume=<name>` | New name for the volume |
| `storage=<name>` | Storage daemon where the volume resides |
| `pool=<name>` | Pool to assign the relabeled volume to |
| `encrypt` | Enable volume encryption on the relabeled volume |

## Examples

### Relabel a volume with a new name
```
* relabel oldvolume=Full-0001 volume=Full-0001-recycled storage=File pool=Full
Connecting to Storage daemon File ...
Sending relabel command ...
3000 OK relabel. VolBytes=237 Volume="Full-0001-recycled"
```

### Relabel keeping the same name (recycle)
```
* relabel oldvolume=Full-0001 volume=Full-0001 storage=File
Relabeling volume Full-0001
```

### Relabel with pool change
```
* relabel oldvolume=Inc-0005 volume=Full-0010 storage=File pool=Full
Relabeling volume Inc-0005
```

## Behavior

- The old volume must be in `Purged` status before it can be relabeled.
- The Director connects to the SD, which overwrites the volume header with a new label.
- The catalog record is updated with the new volume name, pool, and reset byte/file counters.
- All previous job references to the old volume name are invalidated.
- This is an audited command.

## Related Commands

- [label](label.md) -- Label a brand new volume
- [purge](purge.md) -- Purge a volume before relabeling
- [update](update.md) -- Update volume attributes without relabeling
