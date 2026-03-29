# export -- Export Volumes to Autochanger Magazine

Export volumes from storage slots to the autochanger's import/export (mail) slots for physical removal.

## Syntax

```
export storage=<name> srcslots=<slot-selection> [dstslots=<slot-selection>] [scan]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `storage=<name>` | Storage daemon with autochanger (required) |
| `srcslots=<slot-selection>` | Source storage slot numbers (e.g. `1-5` or `1,3,5`) (required) |
| `dstslots=<slot-selection>` | Destination import/export slot numbers |
| `scan` | Scan barcodes before export |

## Examples

### Export volumes from specific slots
```
* export storage=Tape srcslots=1-5
Export from storage Tape
```

### Export to specific mail slots
```
* export storage=Tape srcslots=1-3 dstslots=101-103
Export from storage Tape
```

### Export with barcode scan
```
* export storage=Tape srcslots=1-5 scan
Export from storage Tape
Scanning volumes before export...
```

## Behavior

- The Director instructs the SD to move volumes from regular storage slots to the import/export (mail) slots.
- Once in the mail slots, the tapes can be physically removed from the autochanger for offsite storage or disposal.
- If `dstslots` is not specified, the SD uses the configured import/export slot range.
- The `scan` option triggers a barcode scan before the export.
- This command is only relevant for tape autochangers.
- This is an audited command.

## Related Commands

- [import](import.md) -- Import volumes from autochanger mail slots
- [move](move.md) -- Move volumes between slots
- [list](list.md) -- List volumes to identify which slots to export
