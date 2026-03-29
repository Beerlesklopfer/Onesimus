# var -- Display Variable Expansion

Expand and display the value of a Bareos variable. Used for testing variable substitution in job definitions.

## Syntax

```
var <variable-name>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<variable-name>` | Name of the variable to expand |

## Examples

### Expand a variable
```
* var Client
Variable Client: bareos-fd
```

### Undefined variable
```
* var FooBar
Variable FooBar: undefined
```

### Check job-related variables
```
* var Job
Variable Job: backup-bareos-fd
```

## Behavior

- Expands Bareos runtime variables as they would be substituted in RunScript or other contexts.
- Common variables include: `%c` (Client), `%n` (Job name), `%l` (Level), `%t` (Job type), `%d` (Director name).
- If the variable is not defined or not in a job context, it shows as `undefined`.
- Useful for verifying RunBefore/RunAfter scripts and path templates.
- This is not an audited command.

## Related Commands

- [show](show.md) -- Show resource configuration
- [run](run.md) -- Run a job (where variables are expanded)
