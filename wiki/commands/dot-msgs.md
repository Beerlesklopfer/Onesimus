# .msgs — List Pending Message Types

Machine-interface command used by bconsole/oconsole. Returns pending messages queued for the current console session.

## Syntax

```
.msgs
```

## Arguments

None.

## Examples

### No pending messages
```
* .msgs
```

### Pending messages available
```
* .msgs
28-Mar 21:20 bareos-dir JobId 10: Backup OK
28-Mar 21:20 bareos-dir JobId 10: Sending spooled attrs to catalog
```

## Notes

- Output is one message per line (if any), no headers.
- Output is terminated by the EOD signal.
- Returns the messages queued for the console, then clears them.
- Functionally similar to `messages` but in dot-command (machine) format.

## Related Commands

- [messages](messages.md) -- Display pending console messages (human format)
- [.messages](dot-messages.md) -- List Messages resource names
- [autodisplay](autodisplay.md) -- Toggle automatic message display
