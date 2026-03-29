# .profiles — List Profile Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one profile resource name per line from the Director configuration.

## Syntax

```
.profiles
```

## Arguments

None.

## Examples

### List all profiles
```
* .profiles
operator
webui-admin
webui-readonly
```

### No profiles configured
```
* .profiles
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Profile resources define reusable ACL sets that can be assigned to Console resources.
- Used by console programs to populate the `profile=` argument completion list.

## Related Commands

- [.consoles](dot-consoles.md) -- List console names
- [.acl](dot-acl.md) -- Manage console ACLs
