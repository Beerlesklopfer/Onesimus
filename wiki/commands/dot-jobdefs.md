# .jobdefs — List JobDefs Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one JobDefs resource name per line from the Director configuration.

## Syntax

```
.jobdefs
```

## Arguments

None.

## Examples

### List all jobdefs
```
* .jobdefs
DefaultJob
DefaultRestore
LinuxBackup
```

### No jobdefs configured
```
* .jobdefs
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- JobDefs are template resources from which Job resources inherit directives.
- Used by console programs to populate the `jobdefs=` argument completion list.

## Related Commands

- [.jobs](dot-jobs.md) -- List job names
- [.defaults](dot-defaults.md) -- Show default values for a job
- [show](show.md) -- Show full resource configuration
