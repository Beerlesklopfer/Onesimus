# .defaults — Show Default Values for a Resource

Machine-interface command used by bconsole/oconsole and the WebUI to retrieve default directive values for a named resource. Returns key=value pairs, one per line.

## Syntax

```
.defaults [job=<name>] [client=<name>] [storage=<name>] [pool=<name>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| *(none)* | List summary defaults for all jobs |
| `job=<name>` | Show defaults for the named Job resource |
| `client=<name>` | Show defaults for the named Client resource |
| `storage=<name>` | Show defaults for the named Storage resource |
| `pool=<name>` | Show defaults for the named Pool resource |

## Examples

### Show job defaults
```
* .defaults job=backup-bareos-fd
job=backup-bareos-fd
pool=Full
client=bareos-fd
storage=File
fileset=SelfTest
level=Incremental
type=Backup
enabled=1
schedule=WeeklyCycle
```

### Show client defaults
```
* .defaults client=bareos-fd
client=bareos-fd
address=127.0.0.1
port=9102
enabled=1
```

### Show storage defaults
```
* .defaults storage=File
storage=File
address=127.0.0.1
port=9103
enabled=1
mediatype=File
device=FileStorage
```

## Notes

- Output is key=value pairs, one per line, no headers.
- Output is terminated by the EOD signal.
- If the named resource does not exist, returns an error message like `Job 'xyz' not found`.
- Without arguments, returns a summary line per job: `job=<name> level=<level> client=<client> storage=<storage>`.
- Heavily used by the Bareos WebUI to pre-populate form fields when running jobs.

## Related Commands

- [.jobs](dot-jobs.md) -- List job names
- [.clients](dot-clients.md) -- List client names
- [.storage](dot-storage.md) -- List storage names
- [.pools](dot-pools.md) -- List pool names
- [show](show.md) -- Show full resource configuration
