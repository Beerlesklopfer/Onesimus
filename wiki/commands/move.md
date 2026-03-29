# move -- Move Volume Between Autochanger Slots

Move a volume from one autochanger slot to another within the same autochanger.

## Syntax

```
move [storage=<name>] srcslot=<num> dstslot=<num>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `storage=<name>` | Storage daemon with autochanger |
| `srcslot=<num>` | Source slot number (required) |
| `dstslot=<num>` | Destination slot number (required) |

## Examples

### Move a volume between slots
```
* move storage=Tape srcslot=3 dstslot=7
Moving media from slot 3 to 7
```

### Move within default storage
```
* move srcslot=1 dstslot=10
Moving media from slot 1 to 10
```

### Reorganize after import
```
* import storage=Tape srcslots=101-103
* move storage=Tape srcslot=1 dstslot=20
Moving media from slot 1 to 20
```

## Behavior

- The Director instructs the SD to use the autochanger robot to physically move a volume between slots.
- The destination slot must be empty.
- The catalog slot assignment is updated automatically.
- This command is only relevant for tape autochangers.
- This is an audited command.

## Related Commands

- [import](import.md) -- Import volumes from mail slots
- [export](export.md) -- Export volumes to mail slots
- [update](update.md) -- Update slot information (`update slots`)
