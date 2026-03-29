# whoami -- Show Current Console Identity

Display the name of the console resource that is currently logged in.

## Syntax

```
whoami
```

## Arguments

None.

## Examples

### Default console
```
* whoami
*UserAgent*
```

### Named console
```
* whoami
admin-console
```

### Restricted console
```
* whoami
readonly-console
```

## Behavior

- Returns the name of the Console resource used to authenticate the current session.
- `*UserAgent*` indicates the default console (connected with the Director's password, not a named Console resource).
- Named consoles have ACLs that restrict which commands and resources they can access.
- Useful for verifying which identity is in use, especially when multiple Console resources are configured.
- This is not an audited command.

## Related Commands

- [help](help.md) -- List available commands for the current console
- [quit](quit.md) -- Disconnect the current session
- [.authorized](dot-authorized.md) -- Check ACL authorization for specific resources
