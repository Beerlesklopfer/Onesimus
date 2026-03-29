# configure — Manage Resources

Add, modify, delete, or read Director resources at runtime.

## Syntax

```
configure add <resource-type> name=<name> [key=value ...]
configure read <resource-type>=<name>
configure delete <resource-type>=<name>
```

## Sub-Commands

| Sub-Command | Description |
|-------------|-------------|
| `add` | Add a new resource (client, job, pool, storage, schedule, fileset) |
| `read` | Show current resource configuration |
| `delete` | Remove a resource |

## Examples

### Add a new client
```
* configure add client name=webserver address=10.0.0.5 password="secret123"
Client 'webserver' added.
```

### Add a new job
```
* configure add job name=backup-webserver client=webserver pool=Full fileset=LinuxAll schedule=WeeklyCycle
Job 'backup-webserver' added.
```

### Read a resource
```
* configure read client=bareos-fd
Client {
  Name = bareos-fd
  Address = localhost
  Port = 9102
  Password = "SlC3mJTFz3DnQX9ZEJwWJgAIoxe5GbD/OmyQADlFrN9A"
  Enabled = yes
}
```

### Delete a resource
```
* configure delete client=webserver
Client 'webserver' deleted.
```

## Notes

- Changes are applied immediately (no restart needed)
- A `reload` is triggered automatically after `configure add/delete`
- Resources are written to the config directory (`bareos-dir.d/`)

## Related Commands

- [reload](reload.md) — Reload configuration manually
- [show](show.md) — Show resource details
- [list](list.md) — List catalog records
