# .api — Set API Output Mode

Switches the output format for the current console session. Used by bconsole, oconsole, and the WebUI to select between human-readable, machine-readable, and JSON output.

## Syntax

```
.api [0|1|2|off|on|json] [compact=<yes|no>]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `0` / `off` | Human-readable output (colored, formatted) |
| `1` / `on` | Machine-readable output (tab-separated, for automation) |
| `2` / `json` | JSON output |
| `compact=yes` | Use compact JSON (no extra whitespace) |
| `compact=no` | Use pretty-printed JSON |

## Examples

### Switch to machine-readable mode
```
* .api 1
api: mode=1
```

### Switch to JSON mode
```
* .api json
api: mode=2
```

### Switch back to human-readable
```
* .api 0
api: mode=0
```

## Notes

- Returns `api: mode=<N>` confirming the new mode.
- Output is terminated by the EOD signal.
- The mode persists for the entire session (until changed or disconnected).
- Mode 0 is the default for interactive console use.
- Mode 1 is used by bconsole internally for tab-completion (via `.help` output parsing).
- Mode 2 is used by the Bareos WebUI for all API calls.

## Related Commands

- [.help](dot-help.md) -- List commands (output format depends on API mode)
- [.grpc](dot-grpc.md) -- Toggle gRPC streaming mode
