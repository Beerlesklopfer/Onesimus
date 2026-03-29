# automount -- Toggle Automatic Device Mounting

Enable or disable automatic mounting of storage devices after a `label` command.

## Syntax

```
automount [on|off]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `on` | Enable automatic mounting (default) |
| `off` | Disable automatic mounting |

## Examples

### Enable automount
```
* automount on
automount on
```

### Disable automount
```
* automount off
automount off
```

### Default (enable)
```
* automount
automount on
```

## Behavior

- When `on`, the Storage Daemon automatically mounts a device after a successful `label` command, making the volume immediately available for writing.
- When `off`, a manual `mount` command is required after labeling.
- The default state is `on`.
- This setting affects the current console session.
- This is not an audited command.

## Related Commands

- [mount](mount.md) -- Manually mount a device
- [label](label.md) -- Label a volume (auto-mounts if enabled)
- [unmount](unmount.md) -- Unmount a device
