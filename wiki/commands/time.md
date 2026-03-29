# time -- Show Current Date and Time

Display the Director's current local date and time.

## Syntax

```
time
```

## Arguments

None.

## Examples

### Show current time
```
* time
2026-03-29 14:35:22
```

### Use in a script to timestamp output
```
* time
2026-03-29 01:00:00
* run job=backup-bareos-fd level=Full yes
Job queued. JobId=15
```

### Verify timezone after Director restart
```
* time
2026-03-29 22:10:05
```

## Behavior

- Returns the Director's local system time in `YYYY-MM-DD HH:MM:SS` format.
- The timezone is the Director host's configured timezone (not UTC).
- No arguments are accepted; any extra input is ignored.

## Related Commands

- [version](version.md) -- Show Director version
- [status](status.md) -- Show Director status including uptime
