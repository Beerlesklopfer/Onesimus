# .mediatypes — List Media Type Names

Machine-interface command used by bconsole/oconsole for tab-completion. Returns one unique media type per line, derived from configured Storage resources.

## Syntax

```
.mediatypes
```

## Arguments

None.

## Examples

### List all media types
```
* .mediatypes
File
LTO-8
```

### Single storage type
```
* .mediatypes
File
```

## Notes

- Output is one name per line, no headers, no formatting.
- Output is terminated by the EOD signal.
- Values are extracted from the MediaType directive of all Storage resources, sorted and deduplicated.
- Used by console programs to populate the `mediatype=` argument completion list.

## Related Commands

- [.storage](dot-storage.md) -- List storage names
- [.media](dot-media.md) -- List volume names
- [.defaults](dot-defaults.md) -- Show storage defaults (includes mediatype)
