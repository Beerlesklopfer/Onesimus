# .filesets — List FileSet Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one FileSet resource name per line from the Director configuration.

## Syntax

```
.filesets
```

## Arguments

None.

## Examples

### List all filesets
```
* .filesets
LinuxAll
SelfTest
WindowsAll
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- No filtering options are available for this command.
- Used by console programs to populate the `fileset=` argument completion list.

## Related Commands

- [.jobs](dot-jobs.md) -- List job names
- [.defaults](dot-defaults.md) -- Show default values for a job (includes fileset)
- [show](show.md) -- Show full resource configuration
