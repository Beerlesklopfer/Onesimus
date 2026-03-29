# .locations — List Location Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one location resource name per line from the Director configuration.

## Syntax

```
.locations
```

## Arguments

None.

## Examples

### List locations
```
* .locations
Vault-A
Offsite-B
```

### No locations configured (typical)
```
* .locations
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Location resources are rarely used in modern Bareos installations. They map to physical volume storage locations.
- Returns empty output when no Location resources are configured.

## Related Commands

- [.media](dot-media.md) -- List volume names
- [.storage](dot-storage.md) -- List storage names
