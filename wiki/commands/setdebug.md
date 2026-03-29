# setdebug -- Set Debug Level

Set the debug verbosity level on the Director, a File Daemon, or a Storage Daemon.

## Syntax

```
setdebug level=<nn> [trace=0|1] [timestamp=0|1] [client=<name>] [storage=<name>] [dir]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `level=<nn>` | Debug level (0=off, higher=more verbose, up to 999) |
| `trace=0\|1` | Enable/disable trace file output |
| `timestamp=0\|1` | Enable/disable timestamps in debug output |
| `client=<name>` | Set debug on this File Daemon |
| `storage=<name>` | Set debug on this Storage Daemon |
| `dir` | Set debug on the Director itself |

## Examples

### Set Director debug level
```
* setdebug level=100 dir
Debug level set to 100
```

### Set debug on a File Daemon
```
* setdebug level=200 client=bareos-fd
Debug level set to 200
```

### Enable trace file on Storage Daemon
```
* setdebug level=150 trace=1 storage=File
Debug level set to 150
```

## Behavior

- Without a target (`client`, `storage`, `dir`), sets the debug level on the Director.
- When targeting a client or storage, the Director connects to the daemon via TLS-PSK and sets the debug level remotely.
- Debug level 0 disables debug output.
- Common useful levels: 50 (basic tracing), 100 (protocol), 200 (detailed), 500+ (very verbose).
- `trace=1` writes debug output to a trace file in `/var/lib/bareos/` (or the working directory).
- `timestamp=1` prefixes each debug line with a timestamp.
- This is an audited command.

## Related Commands

- [trace](trace.md) -- Toggle trace file output for the console session
- [status](status.md) -- Check daemon status
- [setdevice](setdevice.md) -- Set device parameters
