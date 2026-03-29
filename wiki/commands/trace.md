# trace -- Toggle Trace File Output

Enable or disable writing Director output to a trace file for debugging purposes.

## Syntax

```
trace [on|off]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `on` | Enable trace file output |
| `off` | Disable trace file output |

## Examples

### Enable trace
```
* trace on
trace on
```

### Disable trace
```
* trace off
trace off
```

### Typical debugging workflow
```
* trace on
trace on
* run job=backup-bareos-fd level=Full yes
Job queued. JobId=15
* wait jobid=15
* trace off
trace off
```

## Behavior

- When enabled, the Director writes a copy of all console output to a trace file in the working directory (typically `/var/lib/bareos/bareos-dir.<pid>.trace`).
- Useful for capturing command output and debugging information for later analysis.
- The trace file is appended to -- it is not truncated when tracing is re-enabled.
- This setting is per-session and does not affect other connected consoles.
- For daemon-level debug tracing, use `setdebug` with the `trace` option instead.
- This is not an audited command.

## Related Commands

- [setdebug](setdebug.md) -- Set debug level with trace option
- [messages](messages.md) -- Display pending messages
