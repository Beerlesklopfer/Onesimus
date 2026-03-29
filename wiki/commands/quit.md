# quit -- Disconnect from Director

Terminate the current bconsole session and close the connection to the Director.

## Syntax

```
quit
exit
.quit
```

## Arguments

None.

## Examples

### Normal disconnect
```
* quit
bye
```

### Using exit alias
```
* exit
bye
```

### Using dot-command variant
```
* .quit
bye
```

## Behavior

- All three forms (`quit`, `exit`, `.quit`) are equivalent and handled by the same code path.
- The Director responds with `bye` and closes the connection.
- Any running jobs are **not** affected -- they continue in the background.
- The session is removed from the Director's session manager.
- This is not an audited command.

## Related Commands

- [whoami](whoami.md) -- Show current console identity before disconnecting
- [status](status.md) -- Check running jobs before quitting
