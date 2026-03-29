# unmount -- Unmount a Storage Device

Instruct the Storage Daemon to unmount a device, making it safe to remove media or perform maintenance.

## Syntax

```
unmount [storage=<name>] [drive=<num>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `storage=<name>` | Storage daemon resource name |
| `drive=<num>` | Drive number for autochangers with multiple drives |

## Examples

### Unmount a file storage device
```
* unmount storage=File
Sending "unmount" to Storage daemon File ...
Device "File" unmounted.
```

### Unmount a specific drive
```
* unmount storage=Tape drive=0
Sending "unmount" to Storage daemon Tape ...
Device "Tape" unmounted.
```

### Unmount before physical tape swap
```
* unmount storage=Tape
Sending "unmount" to Storage daemon Tape ...
Device "Tape" unmounted.
(swap tape, then mount again)
* mount storage=Tape slot=3
```

## Behavior

- The Director sends an `unmount` command to the Storage Daemon via TLS-PSK.
- The SD closes any open volume on the device and makes the medium removable.
- For tape devices, the tape is rewound and ejected from the drive.
- If a job is actively writing to the device, the unmount will fail.
- The `umount` alias is also accepted.
- This is an audited command.

## Related Commands

- [mount](mount.md) -- Mount a storage device
- [release](release.md) -- Release a storage device
- [status](status.md) -- Check if a device is in use before unmounting
