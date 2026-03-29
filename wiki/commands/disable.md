# disable -- Disable a Job, Client, Schedule, or Storage

Disable a resource so it will not be used for scheduled operations or manual runs.

## Syntax

```
disable [job=<name>|client=<name>|schedule=<name>|storage=<name>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `job=<name>` | Disable a Job resource |
| `client=<name>` | Disable a Client resource |
| `schedule=<name>` | Disable a Schedule resource |
| `storage=<name>` | Disable a Storage resource |

## Examples

### Disable a job during maintenance
```
* disable job=backup-bareos-fd
Job 'backup-bareos-fd' disabled
```

### Disable a client
```
* disable client=bareos-fd
Client 'bareos-fd' disabled
```

### Verify with show
```
* disable job=backup-bareos-fd
Job 'backup-bareos-fd' disabled
* show jobs
Job {
  Name = "backup-bareos-fd"
  ...
  Enabled = no
}
```

## Behavior

- Sets the `Enabled` flag to `no` in the in-memory configuration for the named resource.
- Disabled jobs will not run via the scheduler and `run job=<name>` will be rejected with "Job is disabled".
- Disabled clients are excluded from automatic job selection.
- Disabled schedules stop triggering their associated jobs.
- The change is effective immediately but does not persist across Director restarts unless saved.
- Without arguments, the command prints usage help.
- This is an audited command.

## Related Commands

- [enable](enable.md) -- Enable a job/client/schedule/storage
- [show](show.md) -- Show resource configuration
- [reload](reload.md) -- Reload configuration from disk
