# messages -- Display Pending Console Messages

Drain and display all pending messages from the Director's message bus for the current console session.

## Syntax

```
messages
```

## Arguments

None.

## Examples

### No pending messages
```
* messages
No pending messages.
```

### Messages after running a backup
```
* run job=backup-bareos-fd level=Full yes
Job queued. JobId=15
* messages
INFO JobId=15 backup-bareos-fd: Start Backup JobId=15, Job=backup-bareos-fd, Level=full
INFO JobId=15 backup-bareos-fd: Connected to Storage daemon File at 127.0.0.1:9103
INFO JobId=15 backup-bareos-fd: Connected to Client bareos-fd at 127.0.0.1:9102
INFO JobId=15 backup-bareos-fd: Backup OK -- 458 files, 71,683,878 bytes
JobId=15 backup-bareos-fd: Terminated
```

### Messages with errors
```
* messages
ERROR JobId=16 backup-bareos-fd: Fatal error: FD connection refused
JobId=16 backup-bareos-fd: Canceled
```

## Behavior

- Drains all queued messages from the session's message bus receiver.
- Messages include job status changes, info, warnings, errors, and fatal messages.
- Each message is prefixed with its type (`INFO`, `WARNING`, `ERROR`, `FATAL`, `MOUNT`, etc.) and the JobId/job name if applicable.
- Once displayed, messages are consumed and will not appear again.
- If `autodisplay on` is set, messages are displayed automatically after each command.
- This is not an audited command.

## Related Commands

- [autodisplay](autodisplay.md) -- Toggle automatic message display
- [status](status.md) -- Show Director/Client/SD status
- [.messages](dot-messages.md) -- Machine-readable message retrieval
