# gui -- Enable/Disable GUI Mode

Switch the console session to GUI mode, which modifies output formatting for use by graphical user interfaces.

## Syntax

```
gui [on|off]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `on` | Enable GUI mode |
| `off` | Disable GUI mode (return to interactive mode) |

## Examples

### Enable GUI mode
```
* gui on
gui mode on
```

### Disable GUI mode
```
* gui off
gui mode off
```

### Default (enable)
```
* gui
gui mode on
```

## Behavior

- In GUI mode, the Director modifies output to be more suitable for parsing by graphical frontends:
  - Suppresses interactive prompts
  - Disables ANSI color codes
  - Sends messages automatically (implicitly enables `autodisplay`)
- Used by tools like `bat` (Bareos Administration Tool) and web-based interfaces.
- This setting is per-session.
- This is not an audited command.

## Related Commands

- [autodisplay](autodisplay.md) -- Control automatic message display
- [.api](dot-api.md) -- Switch between API output modes (0/1/2)
