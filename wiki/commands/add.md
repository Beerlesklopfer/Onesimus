# add -- Add Volume to a Pool

Add an existing volume record to a pool in the catalog without physically labeling the medium.

## Syntax

```
add [pool=<pool-name>] [storage=<storage-name>] [jobid=<id>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `pool=<pool-name>` | Pool to add the volume to |
| `storage=<storage-name>` | Storage daemon for the volume |
| `jobid=<id>` | Associate with a specific job |

## Examples

### Add a volume to a pool
```
* add pool=Full storage=File
Volume added to pool
```

### Add interactively
```
* add
Volume added to pool
```

### Add after manual volume creation
```
* add pool=Offsite storage=Tape
Volume added to pool
```

## Behavior

- Creates a volume catalog record and assigns it to the specified pool.
- Unlike `label`, this does not write a physical label to the medium -- it only updates the catalog.
- Useful for manually registering volumes that were created outside of Bareos (e.g. pre-labeled tapes).
- The volume name is prompted interactively if not provided.
- This is an audited command.

## Related Commands

- [label](label.md) -- Label and register a new volume (physical + catalog)
- [delete](delete.md) -- Delete a volume record from the catalog
- [list](list.md) -- List volumes in a pool
- [update](update.md) -- Update volume attributes
