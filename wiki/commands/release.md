# release -- Release a Storage Device

Release a storage device from the current job or session, freeing it for use by other jobs.

## Syntax

```
release [storage=<name>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `storage=<name>` | Storage daemon resource name |

## Examples

### Release a file storage device
```
* release storage=File
Sending "release" to Storage daemon File ...
Device "File" released.
```

### Release after a manual operation
```
* mount storage=Tape
Sending "mount" to Storage daemon Tape ...
Device "Tape" mounted.
* release storage=Tape
Sending "release" to Storage daemon Tape ...
Device "Tape" released.
```

### Release the default storage
```
* release storage=File
Sending "release" to Storage daemon File ...
Device "File" released.
```

## Behavior

- The Director sends a `release` command to the Storage Daemon via TLS-PSK.
- The SD marks the device as available for other jobs.
- Unlike `unmount`, release does not eject or physically disconnect the medium.
- Typically used after manual tape operations or when a device is held by a console session.
- This is an audited command.

## Related Commands

- [mount](mount.md) -- Mount a storage device
- [unmount](unmount.md) -- Unmount a storage device
- [status](status.md) -- Check device status
