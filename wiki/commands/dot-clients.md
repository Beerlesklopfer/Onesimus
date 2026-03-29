# .clients — List Client Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one client resource name per line from the Director configuration.

## Syntax

```
.clients [enabled|disabled]
```

## Arguments

| Argument | Description |
|----------|-------------|
| *(none)* | List all clients |
| `enabled` | List only enabled clients |
| `disabled` | List only disabled clients |

## Examples

### List all clients
```
* .clients
bareos-fd
webserver-fd
dbserver-fd
```

### List only enabled clients
```
* .clients enabled
bareos-fd
webserver-fd
```

### List only disabled clients
```
* .clients disabled
dbserver-fd
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Used by console programs to populate the `client=` argument completion list.

## Related Commands

- [status](status.md) -- Show client status
- [.defaults](dot-defaults.md) -- Show default values for a client
- [.authorized](dot-authorized.md) -- Check access to a client resource
- [.jobs](dot-jobs.md) -- List job names
