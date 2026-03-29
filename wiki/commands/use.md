# use -- Switch to a Different Catalog

Select a different catalog database for the current console session.

## Syntax

```
use [catalog=<name>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `catalog=<name>` | Name of the Catalog resource to switch to |

## Examples

### Switch to a named catalog
```
* use catalog=MyCatalog
Using catalog MyCatalog
```

### Use default catalog
```
* use
Using default catalog
```

### Switch and verify
```
* use catalog=ArchiveCatalog
Using catalog ArchiveCatalog
* list jobs
(results from ArchiveCatalog database)
```

## Behavior

- Switches the current session's catalog connection to the specified Catalog resource.
- Subsequent `list`, `llist`, `sqlquery`, and other catalog-querying commands will use the new catalog.
- The Catalog resource must be defined in the Director configuration.
- Without arguments, resets to the default catalog.
- Most installations use a single catalog, making this command rarely needed.
- This is not an audited command.

## Related Commands

- [.catalogs](dot-catalogs.md) -- List all configured catalog resources
- [list](list.md) -- List records from the active catalog
- [sqlquery](sqlquery.md) -- Run SQL queries against the active catalog
