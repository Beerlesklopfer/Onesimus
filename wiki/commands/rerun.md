# rerun -- Re-run a Previously Failed Job

Re-run a job that previously failed or was canceled, using the same parameters as the original run.

## Syntax

```
rerun jobid=<id> [since_jobid=<id>] [until_jobid=<id>] [days=<days>] [hours=<hours>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `jobid=<id>` | Re-run this specific failed job |
| `since_jobid=<id>` | Re-run all failed jobs since this JobId |
| `until_jobid=<id>` | Re-run all failed jobs up to this JobId |
| `days=<days>` | Re-run all failed jobs from the last N days |
| `hours=<hours>` | Re-run all failed jobs from the last N hours |

## Examples

### Re-run a specific failed job
```
* rerun jobid=15
Rerunning job 15
```

### Re-run all failures from the last 24 hours
```
* rerun jobid=0 hours=24
Rerunning job 15
Rerunning job 18
```

### Re-run failures in a range
```
* rerun since_jobid=10 until_jobid=20
Rerunning job 15
Rerunning job 18
```

## Behavior

- The Director looks up the original job record in the catalog and starts a new job with the same Job resource, level, client, and other parameters.
- Only jobs with a failed status (`E`, `f`, `A`) are eligible for rerun.
- The new job receives a new JobId.
- When using `since_jobid`/`until_jobid` or `days`/`hours`, multiple jobs may be rerun at once.
- This is an audited command.

## Related Commands

- [run](run.md) -- Start a new job manually
- [cancel](cancel.md) -- Cancel a running job
- [list](list.md) -- List jobs to find failed ones
