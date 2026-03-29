# autodisplay -- Toggle Automatic Message Display

Enable or disable automatic display of console messages after each command.

## Syntax

```
autodisplay [on|off]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `on` | Enable automatic message display (default) |
| `off` | Disable automatic message display |

## Examples

### Enable autodisplay
```
* autodisplay on
autodisplay on
```

### Disable autodisplay
```
* autodisplay off
autodisplay off
```

### Default (enable)
```
* autodisplay
autodisplay on
```

## Behavior

- When `on`, the Director automatically sends any pending messages to the console after each command completes, without requiring an explicit `messages` command.
- When `off`, messages accumulate and must be retrieved manually with `messages`.
- The default state is `on`.
- This setting is per-session -- it does not affect other connected consoles.
- This is not an audited command.

## Related Commands

- [messages](messages.md) -- Manually display pending messages
- [gui](gui.md) -- Switch to GUI mode (also affects message display)
