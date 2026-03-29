# setdevice -- Set Device Parameters

Modify runtime parameters of a storage device on the Storage Daemon.

## Syntax

```
setdevice storage=<name> device=<name> autoselect=<bool>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `storage=<name>` | Storage daemon resource name (required) |
| `device=<name>` | Device name on the SD (required) |
| `autoselect=<bool>` | Enable/disable automatic device selection (`yes`/`no`) |

## Examples

### Disable autoselect for a device
```
* setdevice storage=Tape device=Drive-0 autoselect=no
Device Drive-0 selected
```

### Enable autoselect
```
* setdevice storage=Tape device=Drive-0 autoselect=yes
Device Drive-0 selected
```

### Set device on file storage
```
* setdevice storage=File device=FileStorage autoselect=yes
Device FileStorage selected
```

## Behavior

- The Director sends the device parameter change to the Storage Daemon via TLS-PSK.
- `autoselect=no` prevents the SD from automatically choosing this device for new jobs, useful for reserving a drive for restores.
- `autoselect=yes` re-enables automatic selection.
- The change is effective immediately but does not persist across SD restarts.
- This is an audited command.

## Related Commands

- [status](status.md) -- Check storage device status
- [mount](mount.md) -- Mount a device
- [unmount](unmount.md) -- Unmount a device
