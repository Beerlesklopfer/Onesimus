# baReos Stress-Test Protokoll

**Datum:** 2026-03-21
**Director:** baReos-dird (Rust) v0.1.0
**System:** Linux 6.12.74+deb13+1-amd64, 32 CPUs, WS-JOERG

## Parallele C++ bconsole Verbindungen (BNET Klartext)

| N | OK | FAIL | Dauer | Director |
|---|-----|------|-------|----------|
| 32 | 32 | 0 | 247ms | ALIVE |
| 64 | 64 | 0 | 238ms | ALIVE |
| 128 | 128 | 0 | 250ms | ALIVE |
| 256 | 256 | 0 | 1302ms | ALIVE |
| 512 | 512 | 0 | 1562ms | ALIVE |
| **1024** | **1024** | **0** | **2429ms** | **ALIVE** |

**Ergebnis:** 1024 parallele Verbindungen in 2.4s, 0 Fehler, kein Crash.

## Vergleich mit C++ Director (aus TESTPROTOKOLL.md)

| Metrik | C++ Director | Rust Director |
|--------|-------------|---------------|
| 64 parallel Legacy | 1.4s | 238ms |
| 64 parallel gRPC | 0/64 TIMEOUT (>16 Limit) | nicht getestet |
| Director Crash | Nie | Nie |
| Max getestet | 64 | **1024** |

Der Rust Director ist bei 64 Verbindungen **~6x schneller** als der C++ Director
und skaliert bis mindestens 1024 ohne Fehler.

## Einzelverbindungen

| Modus | Dauer |
|-------|-------|
| C++ bconsole Klartext | ~200ms |
| C++ bconsole TLS-PSK | ~250ms (PSK-AES128-CBC-SHA TLSv1.2) |
| Rust bconsole Klartext | ~376ms |
| Rust bconsole gRPC | ~100ms |

## Cross-Daemon TLS-PSK

| Test | Ergebnis |
|------|----------|
| Rust Dir → C++ FD (status client) | OK |
| TLS-PSK Identity | R_DIRECTOR\x1ebareos-dir |
| FD Version | 54 (25.0.0) |

## Director-Stabilitaet

- **0 Crashes** bei 1024 parallelen Verbindungen
- Alle Sessions terminieren sauber
- Named Threads (dir:session:N) bis N=2048+
- Kein Memory-Leak beobachtet
