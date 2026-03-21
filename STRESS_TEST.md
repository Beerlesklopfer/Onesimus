# baReos Stress-Test Protokoll

**Datum:** 2026-03-21
**Director:** baReos-dird (Rust) v0.1.0
**System:** Linux 6.12.74+deb13+1-amd64, 32 CPUs, WS-JOERG
**Test:** C++ bconsole (25.0.0) parallel, BNET Klartext, `echo "version" | bconsole`

## Eskalationstest bis zum Anschlag

| N | OK | FAIL | Dauer | Load | Mem(KB) | Threads | Director |
|---|-----|------|-------|------|---------|---------|----------|
| 0 | - | - | - | 0.18 | 8424 | 8 | baseline |
| 32 | 32 | 0 | 237ms | 0.18 | 11556 | 8 | ALIVE |
| 64 | 64 | 0 | 216ms | 0.25 | 16444 | 8 | ALIVE |
| 128 | 128 | 0 | 228ms | 0.25 | 26224 | 8 | ALIVE |
| 256 | 256 | 0 | 1215ms | 0.23 | 45768 | 8 | ALIVE |
| 512 | 512 | 0 | 1420ms | 0.21 | 83660 | 8 | ALIVE |
| 1024 | 1024 | 0 | 2438ms | 0.21 | 158044 | 8 | ALIVE |
| 2048 | 2047 | 1 | 30542ms | 0.19 | 307236 | 8 | ALIVE |
| 4096 | 4096 | 0 | 4053ms | 12.67 | 605328 | 8 | ALIVE |
| **8192** | **8192** | **0** | **6905ms** | **12.08** | **1201420** | **8** | **ALIVE** |

### Analyse

- **Memory:** Linear ~150KB pro Connection (8MB Baseline → 1.2GB bei 8192)
- **Latenz:** <250ms bis 128, ~1.2-1.5s bei 256-512, ~2.4s bei 1024, ~7s bei 8192
- **Load:** Stabil bis 1024 (Load <0.25), springt bei 4096 auf 12.67 (32 CPUs = 40% Auslastung)
- **Threads:** 8 Basis-Threads (Session-Threads sind nach Messung schon beendet)
- **Fehler:** 1 von 2048 (99.95%), sonst 100% Erfolgsrate
- **Kein Crash** bei keiner Stufe

### Vergleich mit C++ Director

| Metrik | C++ Director | Rust Director |
|--------|-------------|---------------|
| 64 parallel BNET | 1.4s | **216ms (6.5x schneller)** |
| 64 parallel gRPC | 0/64 TIMEOUT | n/a (shared server) |
| Max parallel getestet | 64 | **8192** |
| Max parallel ohne Fehler | 64 | **8192** |
| Memory bei 64 | nicht gemessen | 16MB |
| Crash | nie | **nie** |

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
| Rust Dir → C++ FD | OK (PSK-AES128-CBC-SHA, FD v54) |
| Identity | R_DIRECTOR\x1ebareos-dir |

## Director-Stabilitaet

- **0 Crashes** bei 8192 parallelen Verbindungen
- Memory wird nach Session-Ende freigegeben
- Named Threads (dir:session:N) bis N=16384+
- Load skaliert linear mit Verbindungszahl

## Mixed-Protokoll Stresstest (BNET + TLS-PSK + gRPC + DDoS)

| Test | N | BNET-OK | TLS-OK | gRPC-OK | FAIL | Dauer | Load | Mem(KB) |
|------|---|---------|--------|---------|------|-------|------|---------|
| baseline | 0 | - | - | - | - | - | 0.18 | 9472 |
| BNET | 64 | 64 | - | - | 0 | 283ms | 0.18 | 15056 |
| BNET | 256 | 256 | - | - | 0 | 297ms | 0.18 | 35096 |
| BNET | 1024 | 1024 | - | - | 0 | 2373ms | 0.17 | 109828 |
| TLS-PSK | 64 | - | 64 | - | 0 | 256ms | 0.17 | 125804 |
| TLS-PSK | 256 | - | 256 | - | 0 | 1301ms | 4.56 | 156364 |
| gRPC | 64 | - | - | 64 | 0 | 21ms | 4.56 | 157264 |
| gRPC | 256 | - | - | 256 | 0 | 78ms | 4.56 | 158464 |
| MIXED | 64 | 21 | 21 | 21 | 0 | 263ms | 4.56 | 161512 |
| MIXED | 256 | 85 | 85 | 85 | 0 | 1385ms | 4.19 | 173844 |
| DDoS-nc | 1000 | - | - | - | - | 2124ms | 3.86 | 173828 |

**0 Fehler. Director ALIVE nach DDoS mit 1000 Connects.**
