# .schedule — List Schedule Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one schedule resource name per line from the Director configuration.

## Syntax

```
.schedule [enabled|disabled]
```

## Arguments

| Argument | Description |
|----------|-------------|
| *(none)* | List all schedules |
| `enabled` | List only enabled schedules |
| `disabled` | List only disabled schedules |

## Examples

### List all schedules
```
* .schedule
WeeklyCycle
WeeklyCycleAfterBackup
MonthlyCycle
```

### List only enabled schedules
```
* .schedule enabled
WeeklyCycle
WeeklyCycleAfterBackup
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Used by console programs to populate the `schedule=` argument completion list.

## Related Commands

- [.jobs](dot-jobs.md) -- List job names
- [.defaults](dot-defaults.md) -- Show default values for a job (includes schedule)
- [status](status.md) -- Show scheduled jobs
