# .sql — Execute SQL with Sub-Commands

Machine-interface command to execute raw SQL queries against the catalog database or retrieve schema information.

## Syntax

```
.sql query="<SQL statement>"
.sql tables
.sql schema
```

## Arguments

| Argument | Description |
|----------|-------------|
| `query="<SQL>"` | Execute an arbitrary SQL statement |
| `tables` | List all public table names (shortcut for `.sqltables`) |
| `schema` | Show full schema: table_name, column_name, data_type (shortcut for `.sqlschema`) |

## Examples

### Execute a query
```
* .sql query="SELECT jobid, name, jobstatus FROM job ORDER BY jobid DESC LIMIT 3"
10	backup-bareos-fd	T
9	backup-bareos-fd	T
8	RestoreFiles	T
```

### List tables
```
* .sql tables
basefiles
client
file
fileset
job
jobmedia
log
media
path
pool
storage
version
```

### Show schema
```
* .sql schema
basefiles	baseid	bigint
basefiles	jobid	integer
basefiles	fileid	bigint
client	clientid	integer
client	name	text
client	uname	text
...
```

## Notes

- Query results are returned as tab-separated values, one row per line.
- Output is terminated by the EOD signal.
- Requires a catalog database connection. Returns an error if no catalog is configured.
- The SQL is executed directly against PostgreSQL; use with caution.
- For read-only exploration, prefer `.sql tables` and `.sql schema` over raw queries.

## Related Commands

- [.sqltables](dot-sqltables.md) -- List catalog table names
- [.sqlschema](dot-sqlschema.md) -- Show full catalog schema
- [.sqlcount](dot-sqlcount.md) -- Count records in job table
- [sqlquery](sqlquery.md) -- Interactive SQL query command
- [.catalogs](dot-catalogs.md) -- List catalog names
