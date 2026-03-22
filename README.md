# baReos Director

Rust rewrite of the Bareos Director daemon — drop-in replacement for `bareos-dir`.

**Status:** Backup and Restore working end-to-end with C++ FD/SD.

## What works

- **TLS-PSK** authentication (TLSv1.2, compatible with C++ bconsole/FD/SD)
- **CRAM-MD5** two-way authentication
- **Full Backup**: Dir→SD→FD, CatReq proxy, 451 files / 67MB verified
- **Full Restore**: bootstrap generation, Dir→SD(read)→FD, 451 files / 67MB / 0 errors
- **Console commands** via C++ `bconsole`: version, status, restore, run, cancel, list, ...
- **Job Worker FSM**: 17 states, fork() per job, pipe-based IPC to Director
- **SD CatReq proxy**: FindMedia, GetVolInfo, UpdateMedia, CreateJobMedia
- **Config parser**: Bareos `bareos-dir.d/*/*.conf` directory format
- **PostgreSQL catalog**: schema migration, job/media/client/pool queries
- **Scheduler**, **Watchdog**, **Message Bus**

## Architecture

- **Named threads** (`dir:accept`, `dir:scheduler`, `dir:watchdog`, `dir:grpc`, `dir:monitor`)
- **fork() per job** for process isolation
- **crossbeam channels** for core threads, tokio only for gRPC
- **BNET + gRPC** dual protocol (native Rust, no FFI)
- **TLS-PSK** via `openssl` crate (TLSv1.2 — see `tls_psk.rs` for TLSv1.3 limitation docs)

## Crates

| Crate | Description |
|-------|-------------|
| `bareos-bnet` | BNET wire protocol, TLS-PSK, CRAM-MD5 |
| `bareos-config` | Bareos configuration parser |
| `bareos-catalog` | PostgreSQL catalog queries (sqlx) |
| `bareos-orm` | ORM + schema migration |
| `bareos-proto-rs` | gRPC codegen from proto definitions |
| `bareos-dird` | Director binary |

## Build & Run

```bash
cargo build
cargo test

# Run with test config (requires PostgreSQL + bareos-fd + bareos-sd)
target/debug/bareos-dird -c tests/ -f --port 29101 --name bareos-dir

# Connect with C++ bconsole
echo "version" | bconsole -c tests/bconsole.conf
```

## Drop-in Replacement

Both packages can be installed side-by-side, only one runs at a time:

| | Binary | Package |
|--|--------|---------|
| C++ | `/usr/sbin/bareos-dir` | `bareos-director` |
| Rust | `/usr/sbin/bareos-dird` | `bareos-director-rust` |

## Proto submodule

```bash
git submodule update --init
```

## License

AGPL-3.0 (same as Bareos)
