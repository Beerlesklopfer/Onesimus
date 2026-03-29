# update -- Update Volume, Pool, or Slot Attributes

Modify volume or pool attributes in the catalog, or update slot information from an autochanger.

## Syntax

```
update [volume=<name>|pool=<name>|slots storage=<name>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `volume=<name>` | Update attributes for this volume |
| `pool=<name>` | Update pool record from the resource definition |
| `slots` | Scan autochanger and update slot assignments |
| `storage=<name>` | Storage daemon for slot updates |

## Examples

### Update a volume
```
* update volume=Full-0001
Volume "Full-0001" updated in catalog
```

### Update a pool from resource definition
```
* update pool=Full
Pool "Full" updated from resource
```

### Update autochanger slots
```
* update slots storage=Tape
Updating slots for storage Tape...
Done.
```

## Behavior

- `update volume`: opens an interactive dialog (in C++ Bareos) to change volume attributes such as VolStatus, MaxVolBytes, MaxVolJobs, Recycle, Retention, etc. With the Director, the catalog record is updated directly.
- `update pool`: synchronizes the catalog pool record with the current Pool resource definition (e.g. after editing and reloading the config).
- `update slots`: queries the autochanger for current slot contents and updates the catalog accordingly.
- Without arguments, prints available sub-commands.
- This is an audited command.

## Related Commands

- [label](label.md) -- Label a new volume
- [list](list.md) -- List volumes to see current attributes
- [show](show.md) -- Show pool resource definitions
- [reload](reload.md) -- Reload config before updating pools
