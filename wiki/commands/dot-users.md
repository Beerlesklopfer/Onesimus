# .users — List User Resource Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one User resource name per line from the Director configuration.

## Syntax

```
.users
```

## Arguments

None.

## Examples

### List users
```
* .users
admin
operator
```

### No user resources configured (typical)
```
* .users
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- User resources are optional and rarely configured in most Bareos installations. Access control is typically handled via Console and Profile resources instead.
- Returns empty output when no User resources exist.

## Related Commands

- [.consoles](dot-consoles.md) -- List console names
- [.profiles](dot-profiles.md) -- List profile names
- [whoami](whoami.md) -- Show current console identity
