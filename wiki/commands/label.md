# label -- Label a New Volume

Write a Bareos label to a new volume on a Storage Daemon device and register it in the catalog.

## Syntax

```
label [volume=<name>] [storage=<name>] [pool=<name>] [barcodes] [encrypt]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `volume=<name>` | Name for the new volume |
| `storage=<name>` | Storage daemon to label on (required) |
| `pool=<name>` | Pool to assign the volume to |
| `barcodes` | Use barcode reader to label all unregistered slots |
| `encrypt` | Enable volume encryption |

## Examples

### Label a single volume
```
* label volume=Full-0001 storage=File pool=Full
Connecting to Storage daemon File ...
Sending label command for Volume "Full-0001" Pool "Full" Slot 0 drive 0 ...
Label command sent to SD. Waiting for response...
3000 OK label. VolBytes=237 Volume="Full-0001" Device="FileStorage" (/srv/BACKUP/bareos-storage)
```

### Label using barcodes (autochanger)
```
* label storage=Tape barcodes
Sending barcode label command to Storage Tape...
Labeled 12 volumes from barcode reader.
```

### Without storage (interactive selection)
```
* label
label: specify storage=<name>
Available: File, Tape
```

## Behavior

- The Director connects to the specified Storage Daemon via TLS-PSK.
- The SD writes a Bareos volume label to the physical medium (file, tape, etc.).
- A new media record is created in the catalog with status `Append`.
- With `barcodes`, the SD reads barcodes from all autochanger slots and labels each unregistered volume.
- The volume name must be unique within the catalog.
- If `automount` is enabled, the device is automatically mounted after labeling.
- This is an audited command.

## Related Commands

- [relabel](relabel.md) -- Re-label an existing volume
- [list](list.md) -- List volumes in catalog (`list volumes`)
- [mount](mount.md) -- Mount a device after labeling
- [add](add.md) -- Add a volume to a pool without labeling
