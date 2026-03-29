# enable -- Enable a Job, Client, Schedule, or Storage

Re-enable a previously disabled resource so it can participate in scheduled and manual operations again.

## Syntax

```
enable [job=<name>|client=<name>|schedule=<name>|storage=<name>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `job=<name>` | Enable a Job resource |
| `client=<name>` | Enable a Client resource |
| `schedule=<name>` | Enable a Schedule resource |
| `storage=<name>` | Enable a Storage resource |

## Examples

### Enable a job
```
* enable job=backup-bareos-fd
Job 'backup-bareos-fd' enabled
```

### Enable a client
```
* enable client=bareos-fd
Client 'bareos-fd' enabled
```

### Enable a schedule
```
* enable schedule=WeeklyCycle
Schedule 'WeeklyCycle' enabled
```

## Behavior

- Sets the `Enabled` flag to `yes` in the in-memory configuration for the named resource.
- The change is effective immediately but does not persist across Director restarts unless the configuration is also saved (via `configure` or manual edit).
- Enabling an already-enabled resource succeeds silently.
- If the resource name is not found, an error is returned.
- Without arguments, the command prints usage help.
- This is an audited command.

## Related Commands

- [disable](disable.md) -- Disable a job/client/schedule/storage
- [show](show.md) -- Show resource configuration (includes enabled state)
- [reload](reload.md) -- Reload configuration from disk
