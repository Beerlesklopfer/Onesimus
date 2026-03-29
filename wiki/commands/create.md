# create -- Create Pool from Resource Definition

Create a pool record in the catalog database based on the Pool resource definition in the Director configuration.

## Syntax

```
create [pool=<pool-name>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `pool=<pool-name>` | Pool resource name to create in the catalog |

## Examples

### Create a specific pool
```
* create pool=Full
Pool created from resource
```

### Create default pool
```
* create
Pool created from resource
```

### Create after adding a new pool resource
```
* reload
reloaded
* create pool=Offsite
Pool created from resource
```

## Behavior

- Reads the Pool resource definition from the Director configuration and creates a matching record in the catalog database.
- If the pool record already exists in the catalog, it is not modified -- use `update pool` to synchronize changes.
- Pool creation is typically needed after adding a new Pool resource to the configuration and reloading.
- The Director automatically creates pool records for configured pools at startup, so this command is mainly needed for pools added at runtime.
- This is an audited command.

## Related Commands

- [update](update.md) -- Update an existing pool record from resource
- [show](show.md) -- Show pool resource definitions
- [list](list.md) -- List pool records in catalog
- [reload](reload.md) -- Reload configuration before creating
