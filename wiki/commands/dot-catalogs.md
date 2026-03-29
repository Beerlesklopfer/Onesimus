# .catalogs — List Catalog Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one catalog resource name per line from the Director configuration.

## Syntax

```
.catalogs
```

## Arguments

None.

## Examples

### List all catalogs
```
* .catalogs
MyCatalog
```

### Typical single-catalog setup
```
* .catalogs
bareos
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Most installations have exactly one catalog.
- Used by console programs to populate the `catalog=` argument completion list.

## Related Commands

- [.catalog](dot-catalog.md) -- Catalog schema management
- [.sql](dot-sql.md) -- Execute SQL against the catalog
- [.sqltables](dot-sqltables.md) -- List catalog table names
- [use](use.md) -- Switch active catalog
