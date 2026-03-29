# .consoles — List Console Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one console resource name per line from the Director configuration.

## Syntax

```
.consoles
```

## Arguments

None.

## Examples

### List all consoles
```
* .consoles
bareos-mon
admin-console
webui-console
```

### Single console setup
```
* .consoles
bareos-mon
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Console resources define named console connections with optional ACL restrictions.
- The default console (no Console resource match) connects as `*UserAgent*` with full access.

## Related Commands

- [.acl](dot-acl.md) -- Manage console ACLs
- [.profiles](dot-profiles.md) -- List profile names
- [whoami](whoami.md) -- Show current console identity
