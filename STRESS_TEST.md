# baReos Stress-Test Protokoll

**Datum:** 2026-03-21
**Director:** baReos-dird (Rust) v0.1.0
**System:** Linux 6.12.74+deb13+1-amd64, 32 CPUs, WS-JOERG

## 1. Parallele C++ bconsole Verbindungen (Klartext BNET)

| N | OK | FAIL | Dauer | Director |
|---|-----|------|-------|----------|
| 4 | 4 | 0 | 239ms | ALIVE |
| 8 | 8 | 0 | 203ms | ALIVE |
| 16 | 16 | 0 | 199ms | ALIVE |
| **32** | **32** | **0** | **203ms** | **ALIVE** |

**Exzellent:** 32 parallele Verbindungen in 203ms, kein Fehler, kein Crash.

## 2. Einzelverbindungen

| Modus | Ergebnis | Dauer |
|-------|----------|-------|
| C++ bconsole Klartext | OK | ~200ms |
| C++ bconsole TLS-PSK | OK (PSK-AES128-CBC-SHA TLSv1.2) | ~250ms |
| Rust bconsole Klartext | OK | ~376ms |
| Rust bconsole gRPC | OK | ~100ms |

## 3. Cross-Daemon TLS-PSK

| Test | Ergebnis |
|------|----------|
| Rust Dir → C++ FD (status client) | OK |
| TLS-PSK Identity | R_DIRECTOR\x1ebareos-dir |
| FD Version | 54 (25.0.0) |
| Cipher | PSK-AES128-CBC-SHA TLSv1.2 |

## 4. Alle Protokoll-Modi

| Protokoll | Console → Dir | Dir → FD | Dir → SD |
|-----------|---------------|----------|----------|
| BNET Klartext | OK | - | - |
| BNET TLS-PSK | OK | OK | OK |
| gRPC | OK | - | - |

## 5. Director-Stabilitaet

- **Kein Crash** bei 32 parallelen Verbindungen
- Alle Sessions terminieren sauber (<4ms pro Batch)
- Named Threads sichtbar in pstree (dir:session:N)
- SIGHUP Config-Reload funktioniert

## 6. Fruehere Fehldiagnosen

Die anfaenglichen "CRASHED" Meldungen waren durch externe `sudo pkill -9`
verursacht, nicht durch Director-Bugs. Nach sauberem Test ohne externen
Kill: **0 Crashes bei 4+8+16+32 parallelen Verbindungen.**
