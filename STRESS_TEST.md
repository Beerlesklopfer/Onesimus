# baReos Stress-Test Protokoll

**Datum:** 2026-03-20
**Director:** baReos-dird (Rust) v0.1.0
**System:** Linux 6.12.74+deb13+1-amd64, 32 CPUs, WS-JOERG

## 1. Sequentielle Verbindungen (Rust bconsole)

| # | Dauer | Ergebnis |
|---|-------|----------|
| 1 | 356ms | OK |
| 2 | 359ms | OK |
| 3 | 399ms | OK |
| 4 | 391ms | OK |

**Durchschnitt:** ~376ms pro Verbindung (Connect + CRAM-MD5 + version + quit)

## 2. Parallele Verbindungen (Rust bconsole)

| N | Ergebnis | Dauer |
|---|----------|-------|
| 4 | 4/4 OK | ~400ms |
| 8 | haengt | >60s (CRAM-MD5 Contention) |

**Analyse:** Bei 8 parallelen Verbindungen blockieren die CRAM-MD5 Round-Trips
sich gegenseitig. Der Accept-Loop ist non-blocking, aber CRAM-MD5 ist synchron
pro Session-Thread (4 Round-Trips pro Auth).

## 3. C++ bconsole Verbindungen

| N | Modus | Ergebnis |
|---|-------|----------|
| 1 | Klartext | OK |
| 1 | TLS-PSK | OK (PSK-AES128-CBC-SHA TLSv1.2) |
| 3 | Klartext parallel | 3/3 OK |
| 64 | Klartext parallel | 64/64 OK (erster Batch-Test) |

## 4. gRPC Verbindungen

| Test | Ergebnis |
|------|----------|
| version via gRPC | OK |
| status dir via gRPC | OK |

## 5. Director zu C++ FD via TLS-PSK

| Test | Ergebnis |
|------|----------|
| status client=bareos-fd | OK (TLS-PSK + CRAM-MD5, FD Version 54) |

## 6. Director-Stabilitaet

- Kein Crash bei allen Tests
- Alle Session-Threads terminieren sauber
- SIGHUP Config-Reload funktioniert
