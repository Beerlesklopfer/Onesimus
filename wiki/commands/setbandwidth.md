# setbandwidth -- Set Bandwidth Limit

Set a bandwidth limit for data transfer to/from a specific client or running job.

## Syntax

```
setbandwidth limit=<speed> [client=<name>] [jobid=<id>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `limit=<speed>` | Bandwidth limit in bytes/second (required). Use suffixes: `k` (KB/s), `m` (MB/s) |
| `client=<name>` | Apply limit to all jobs for this client |
| `jobid=<id>` | Apply limit to this specific running job |

## Examples

### Limit a client to 10 MB/s
```
* setbandwidth limit=10m client=bareos-fd
Bandwidth limit set to 10m
```

### Limit a specific running job
```
* setbandwidth limit=5m jobid=15
Bandwidth limit set to 5m
```

### Remove bandwidth limit
```
* setbandwidth limit=0 client=bareos-fd
Bandwidth limit set to 0
```

## Behavior

- The Director sends the bandwidth limit to the File Daemon via TLS-PSK.
- When `client` is specified, the limit applies to all current and future jobs for that client.
- When `jobid` is specified, the limit applies only to that specific running job.
- Setting `limit=0` removes the bandwidth restriction.
- The limit is not persistent -- it resets when the FD restarts or the job completes.
- Useful for throttling backups during business hours to reduce network impact.
- This is an audited command.

## Related Commands

- [status](status.md) -- Check running jobs and their throughput
- [setdebug](setdebug.md) -- Set debug level on daemons
