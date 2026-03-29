# .messages — List Message Resource Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one Messages resource name per line from the Director configuration.

## Syntax

```
.messages
```

## Arguments

None.

## Examples

### List message resources
```
* .messages
Standard
Daemon
```

### Single messages resource
```
* .messages
Standard
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Messages resources define how and where the Director sends log messages (mail, file, console, etc.).
- Used by console programs to populate the `messages=` argument completion list.

## Related Commands

- [messages](messages.md) -- Display pending console messages
- [.msgs](dot-msgs.md) -- List pending message types
- [show](show.md) -- Show full resource configuration
