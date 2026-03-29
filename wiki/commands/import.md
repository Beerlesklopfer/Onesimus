# import -- Import Volumes from Autochanger Magazine

Import volumes from the autochanger's import/export (mail) slots into storage slots.

## Syntax

```
import storage=<name> [srcslots=<slot-selection>] [dstslots=<slot-selection>] [scan]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `storage=<name>` | Storage daemon with autochanger (required) |
| `srcslots=<slot-selection>` | Source slot numbers (e.g. `1-5` or `1,3,5`) |
| `dstslots=<slot-selection>` | Destination slot numbers |
| `scan` | Scan barcodes after import |

## Examples

### Import from default mail slots
```
* import storage=Tape
Import from storage Tape
```

### Import specific slots with scan
```
* import storage=Tape srcslots=1-5 scan
Import from storage Tape
Scanning imported volumes...
```

### Import to specific destination slots
```
* import storage=Tape srcslots=101-105 dstslots=1-5
Import from storage Tape
```

## Behavior

- The Director instructs the SD to move volumes from the autochanger's import/export slots (mail slots) to regular storage slots.
- If `srcslots` is not specified, the SD uses the configured import/export slot range.
- The `scan` option triggers a barcode scan after the import, automatically labeling unregistered volumes.
- This command is only relevant for tape autochangers (libraries) -- file-based storage does not use import.
- This is an audited command.

## Related Commands

- [export](export.md) -- Export volumes to autochanger mail slots
- [label](label.md) -- Label imported volumes
- [update](update.md) -- Update slot information (`update slots`)
- [move](move.md) -- Move volumes between slots
