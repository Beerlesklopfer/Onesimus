# estimate -- Estimate Backup Size

Contact the File Daemon to estimate the number of files and bytes that would be backed up, without actually running a backup job.

## Syntax

```
estimate [job=<name>] [client=<name>] [listing]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `job=<name>` | Job resource to estimate (required) |
| `client=<name>` | Override the client from the Job resource |
| `listing` | Show individual file listing with sizes |

## Examples

### Basic estimate
```
* estimate job=backup-bareos-fd
Connecting to Client "bareos-fd" for estimate...
Job: backup-bareos-fd
FileSet: SelfTest
2,458 files, 156,234,567 bytes (149.0 MB)
```

### Estimate with file listing
```
* estimate job=backup-bareos-fd listing
Connecting to Client "bareos-fd" for estimate...
Job: backup-bareos-fd
FileSet: SelfTest
Listing mode enabled
/usr/sbin/bareos-dird      1,234,567
/usr/sbin/bconsole           456,789
/usr/sbin/bareos-fd          987,654
...
2,458 files, 156,234,567 bytes (149.0 MB)
```

### Without arguments (shows available jobs)
```
* estimate
estimate: specify job=<name>
Backup jobs: backup-bareos-fd, BackupCatalog
```

## Behavior

- The Director connects to the File Daemon via TLS-PSK and requests a file listing based on the Job's FileSet.
- No data is transferred to the Storage Daemon -- this is a dry run.
- The `listing` flag causes individual files and their sizes to be printed.
- Useful for capacity planning and verifying FileSet Include/Exclude patterns.
- This is not an audited command.

## Related Commands

- [run](run.md) -- Actually run the backup job
- [show](show.md) -- Show FileSet configuration
- [status](status.md) -- Check client connectivity
