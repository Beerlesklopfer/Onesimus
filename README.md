# baReos Director

Rust rewrite of the Bareos Director daemon.

**Status:** Phase R0 — Repository skeleton

## Architecture

- **Named threads** (`dir:accept`, `dir:scheduler`, `dir:watchdog`, `dir:grpc`, `dir:monitor`)
- **fork() per job** for process isolation
- **crossbeam channels** for core threads, tokio only for gRPC
- **BNET + gRPC** dual protocol (native Rust, no FFI)

## Crates

| Crate | Description |
|-------|-------------|
| `bareos-bnet` | BNET wire protocol, TLS, CRAM-MD5 |
| `bareos-config` | Bareos configuration parser (nom) |
| `bareos-catalog` | PostgreSQL catalog (sqlx) |
| `bareos-proto-rs` | gRPC codegen from proto definitions |
| `bareos-dird` | Director binary |

## Build

```bash
cargo build
cargo test
```

## Proto submodule

```bash
git submodule update --init
```

## License

AGPL-3.0 (same as Bareos)
