# llist -- Long List (Full Catalog Query)

Display catalog records in long (verbose) format. This command is an alias for `list` and produces the same output.

## Syntax

```
llist [jobs|clients|pools|volumes|media] [jobid=<id>] [limit=<N>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `jobs` | List job records (default) |
| `clients` | List client records |
| `pools` | List pool records |
| `volumes` / `media` | List volume records |
| `jobid=<id>` | Show a specific job by ID |
| `limit=<N>` | Limit number of results |

## Examples

### Long list of jobs
```
* llist jobs
jobid  jobname           type level jobstatus clientname  starttime            endtime              jobfiles jobbytes
10     backup-bareos-fd  B    F     T         bareos-fd   2026-03-28 21:20:40  2026-03-28 21:20:42  458      71683878
```

### Long list of volumes
```
* llist volumes
mediaid volumename volstatus volbytes   voljobs volfiles mediatype poolname pct_full
1       Full-0001  Append    839002751  4       0        File      Full     0.0
```

### Long list with limit
```
* llist jobs limit=5
```

## Behavior

- In standard Bareos, `llist` displays all columns from the catalog while `list` shows a condensed view. In this implementation, `llist` and `list` produce identical output.
- Both commands query the same catalog views and tables.
- Accepts the same arguments as `list`.
- This is not an audited command.

## Related Commands

- [list](list.md) -- List catalog contents (same output)
- [sqlquery](sqlquery.md) -- Raw SQL queries for custom reports
