# baReos Stress-Test Protokoll

**Datum:** 2026-03-21
**Director:** baReos-dird (Rust) v0.1.0
**System:** Linux 6.12.74+deb13+1-amd64, 32 CPUs, WS-JOERG

## 1. Sequentielle Verbindungen (Rust bconsole → Rust Director)

| # | Dauer | Ergebnis |
|---|-------|----------|
| 1 | 356ms | OK |
| 2 | 359ms | OK |
| 3 | 399ms | OK |
| 4 | 391ms | OK |

**Durchschnitt:** ~376ms pro Verbindung

## 2. Parallele Verbindungen (C++ bconsole → Rust Director, Klartext)

| N | Ergebnis | Bemerkung |
|---|----------|-----------|
| 1 | OK | Einzelverbindung |
| 3 | 3/3 OK | Parallel, alle erfolgreich |
| 8 | 8/8 OK | Alle Sessions sauber beendet |
| 16 | teilweise Timeouts | Einige bconsole warten >30s |
| 64 | 64/64 OK | Erster Batch-Test erfolgreich (C++ bconsole) |

**Anmerkung:** Der 64er-Test war der allererste Test ohne vorherige
Belastung. Bei wiederholten Tests mit 16+ Verbindungen treten Timeouts auf.
Die Ursache ist der synchrone peek()/psk_accept() im Accept-Thread.

## 3. TLS-PSK Verbindungen (C++ bconsole)

| Test | Ergebnis |
|------|----------|
| 1x TLS-PSK | OK (PSK-AES128-CBC-SHA TLSv1.2) |
| 1x Klartext | OK (Encryption: None) |

Beide Modi funktionieren dank TLS-Probing (0x16 Byte Detection).

## 4. gRPC Verbindungen (Rust bconsole → Rust Director)

| Test | Ergebnis |
|------|----------|
| version | OK |
| status dir | OK (zeigt Config-Daten) |

## 5. Cross-Daemon TLS-PSK (Rust Director → C++ FD)

| Test | Ergebnis |
|------|----------|
| status client=bareos-fd | OK |
| TLS-PSK Handshake | PSK Identity: R_DIRECTOR\x1ebareos-dir |
| CRAM-MD5 | OK |
| FD Version | 54 (bareos-fd 25.0.0) |

## 6. Bekannte Limitierungen

1. **Parallele Connections >8**: Der synchrone peek()+psk_accept() im
   Accept-Thread wird zum Bottleneck. Jede Connection blockiert den
   Accept-Thread waehrend TLS-Handshake + CRAM-MD5 (4 Round-Trips).

2. **Loesung (geplant)**: Accept-Thread nur fuer TCP accept, TLS+Auth
   in den per-Session Thread verschieben.

## 7. Director-Stabilitaet

- **Kein Crash** bei regulaerem Betrieb
- Alle Session-Threads terminieren sauber (BNET_TERMINATE/EOF)
- SIGHUP Config-Reload funktioniert waehrend aktiver Sessions
- Director wurde nur durch externen `kill -9` beendet
