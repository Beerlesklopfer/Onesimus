# .authorized — Check Command Authorization

Machine-interface command used by bconsole/oconsole to check whether the current console session has access to a specific resource. Returns `authorized` or `not authorized`.

## Syntax

```
.authorized job=<name> | client=<name> | storage=<name> | pool=<name> | fileset=<name> | schedule=<name> | catalog=<name> | cmd=<command>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `job=<name>` | Check access to a Job resource |
| `client=<name>` | Check access to a Client resource |
| `storage=<name>` | Check access to a Storage resource |
| `pool=<name>` | Check access to a Pool resource |
| `fileset=<name>` | Check access to a FileSet resource |
| `schedule=<name>` | Check access to a Schedule resource |
| `catalog=<name>` | Check access to a Catalog resource |
| `cmd=<command>` | Check access to a console command |

Multiple arguments can be combined; all must be authorized for the result to be `authorized`.

## Examples

### Check job authorization
```
* .authorized job=backup-bareos-fd
authorized
```

### Check a non-existent resource
```
* .authorized client=nonexistent-fd
not authorized
```

### Check multiple resources at once
```
* .authorized job=backup-bareos-fd client=bareos-fd storage=File
authorized
```

## Notes

- Returns a single line: either `authorized` or `not authorized`.
- Output is terminated by the EOD signal.
- Currently checks whether the named resource exists in the configuration. Full ACL-based checks (per Console resource) are planned.
- Used by the WebUI to enable/disable UI elements based on the connected console's permissions.

## Related Commands

- [.consoles](dot-consoles.md) -- List console names
- [.acl](dot-acl.md) -- Manage console ACLs
- [.profiles](dot-profiles.md) -- List profile names
- [whoami](whoami.md) -- Show current console identity
