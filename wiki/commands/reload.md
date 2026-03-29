# reload -- Reload Director Configuration

Reload the Director's configuration files without restarting the daemon. Equivalent to sending SIGHUP.

## Syntax

```
reload
```

## Arguments

None.

## Examples

### Reload after config change
```
* reload
reloaded
Director 'bareos-dir': 1 clients, 3 jobs, 1 storages, 4 pools, 1 schedules
```

### Verify reload with show
```
* reload
reloaded
Director 'bareos-dir': 1 clients, 3 jobs, 1 storages, 4 pools, 1 schedules
* show all
Director: bareos-dir
Clients: 1
Jobs: 3
Storages: 1
Pools: 4
FileSets: 2
Schedules: 1
Consoles: 1
```

### Reload after adding a new job
```
* reload
reloaded
Director 'bareos-dir': 2 clients, 4 jobs, 1 storages, 4 pools, 1 schedules
```

## Behavior

- Re-reads all Director configuration files from disk.
- Running jobs are not affected -- they continue with the configuration they were started with.
- New or modified resources become available immediately.
- Deleted resources are removed from the in-memory config (but running jobs using them continue).
- If the configuration has errors, the reload fails and the previous configuration remains active.
- Reports a summary of loaded resources after a successful reload.
- This is an audited command.

## Related Commands

- [show](show.md) -- Show resource configuration after reload
- [create](create.md) -- Create pool records for newly added pools
- [update](update.md) -- Update catalog pool records after reload
