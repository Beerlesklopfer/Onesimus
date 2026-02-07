# Onesimus Test-Suite

Umfassende Testinfrastruktur für Onesimus - Bareos/Bacula GUI.

## Übersicht

| Datei | Typ | Beschreibung |
|-------|-----|--------------|
| `bareosauth_test.cpp` | C++ | BareosAuth Klassen-Test (CRAM-MD5) |
| `director_test.cpp` | C++ | BareosDirector State-Machine-Test |
| `bareos_auth.sh` | Shell | Automatisiertes Auth-Testskript |
| `director_test.sh` | Shell | Automatisiertes Director-Testskript |
| `generate_bareos_testdata.py` | Python | Testdaten-Generator für Bareos-DB |
| `configs/` | Verzeichnis | Director Console-Konfigurationen |

---

## 1. Authentifizierungs-Tests

### bareosauth_test

Testet die `BareosAuth`-Klasse mit allen Authentifizierungsmodi.

**Unterstützte Modi:**
| Modus | TLS | Beschreibung |
|-------|-----|--------------|
| `legacy` | Nein | CRAM-MD5 ohne Verschlüsselung |
| `psk` | TLS-PSK | Pre-Shared Key + CRAM-MD5 |
| `cert` | TLS-Cert | X.509 Zertifikate + CRAM-MD5 |

**Verwendung:**
```bash
./bareosauth_test [options]

Optionen:
  --host <host>            Director-Host (default: localhost)
  -p, --port <port>        Director-Port (default: 9101)
  -d, --director <name>    Director-Name (default: bareos-dir)
  -c, --console <name>     Console-Name (default: onesimus)
  -P, --password <pwd>     Passwort (erforderlich)
  -m, --mode <mode>        Auth-Modus: legacy, psk, cert (default: legacy)
  --ca <file>              CA-Zertifikat (für cert-Modus)
  --cert <file>            Client-Zertifikat (für cert-Modus)
  --key <file>             Client-Schlüssel (für cert-Modus)
  --verbose                Ausführliche Ausgabe
```

**Beispiele:**
```bash
# Legacy-Modus
./bareosauth_test -m legacy -c onesimus -P "mypassword"

# PSK-Modus
./bareosauth_test -m psk -c onesimus-psk -P "mypassword"

# Zertifikat-Modus
./bareosauth_test -m cert -c onesimus-cert -P "mypassword" \
    --ca /etc/bareos/ssl/bareos-ca.pem \
    --cert /etc/bareos/ssl/bareos-client.pem \
    --key /etc/bareos/ssl/bareos-client.key
```

### bareos_auth.sh

Automatisiertes Testskript für mehrere Authentifizierungszyklen.

**Verwendung:**
```bash
./bareos_auth.sh [options]

Optionen:
  -m, --mode <mode>     Test-Modus: legacy, psk, cert, all (default: all)
  -n, --count <n>       Anzahl Iterationen (default: 10)
  -p, --password <pwd>  Passwort (oder BAREOS_PASSWORD env)
  -H, --host <host>     Director-Host (default: localhost)
  -P, --port <port>     Director-Port (default: 9101)
  -v, --verbose         Ausführliche Ausgabe
```

**Beispiele:**
```bash
# 20 Legacy-Tests
BAREOS_PASSWORD=mypassword ./bareos_auth.sh -m legacy -n 20

# Alle Modi testen
./bareos_auth.sh -m all -p mypassword -n 10
```

---

## 2. Director State-Machine-Tests

### director_test

Testet die `BareosDirector`-Klasse und deren State-Machine.

**State Machine Flow:**
```
Disconnected → Connecting → Authenticating → SettingApiMode
    → LoadingResources → Ready
```

**Geladene Resources:**
| Resource | Dot-Command | Beschreibung |
|----------|-------------|--------------|
| Catalog | `.catalogs` | Datenbank-Kataloge |
| Client | `.clients` | Backup-Clients (File Daemons) |
| Fileset | `.filesets` | Dateiset-Definitionen |
| Job | `.jobs` | Job-Definitionen |
| Pool | `.pools` | Speicher-Pools |
| Schedule | `.schedule` | Backup-Zeitpläne |
| Storage | `.storages` | Storage-Daemons |
| Level | `.levels` | Backup-Level (F, I, D) |

**Verfügbare Test-Befehle:**
| Befehl | Beschreibung |
|--------|--------------|
| `connect` | Nur Verbinden und Authentifizieren |
| `status` | Status Director abfragen |
| `jobs` | Job-Liste abrufen (.jobs) |
| `clients` | Client-Liste abrufen (.clients) |
| `bvfs` | BVFS (Backup Virtual File System) erkunden |
| `interactive` | Interaktiver Modus |

**Verwendung:**
```bash
./director_test [options] [command]

Optionen:
  --host <host>         Director-Host (default: localhost)
  -p, --port <port>     Director-Port (default: 9101)
  -d, --director <name> Director-Name (default: bareos-dir)
  -c, --console <name>  Console-Name (default: onesimus)
  -P, --password <pwd>  Passwort (erforderlich)
  --legacy              Legacy-Modus (kein TLS)
  --psk                 TLS-PSK-Modus
  --verbose             Ausführliche Ausgabe
```

**Beispiele:**
```bash
# Einfacher Verbindungstest
./director_test --legacy -P "mypassword" connect

# Status abfragen
./director_test --legacy -P "mypassword" status

# Jobs auflisten
./director_test --legacy -P "mypassword" --verbose jobs

# BVFS erkunden
./director_test --legacy -P "mypassword" --verbose bvfs
```

### BVFS Explorer

Der `bvfs` Befehl erkundet das Backup Virtual File System:

1. **Jobs abrufen** - Findet aktuelle Backup-Jobs mit JobIDs
2. **Cache aktualisieren** - `.bvfs_update jobid=<id>`
3. **Restore-Chain ermitteln** - `.bvfs_get_jobids jobid=<id> all`
4. **Verzeichnisse auflisten** - `.bvfs_lsdirs jobid=<ids> path=/`
5. **Dateien auflisten** - `.bvfs_lsfiles jobid=<ids> path=/`
6. **Datei-Versionen anzeigen** - `.bvfs_versions client=<c> pathid=<id> filename=<f>`

**Ausgabe-Beispiel:**
```
=== BVFS Explorer ===
[BVFS] Step 1: Getting recent backup jobs...

[BVFS] Recent Backup Jobs:
  JobId | Name                | Client              | Level | Status | StartTime
  ------|---------------------|---------------------|-------|--------|--------------------
    123 | BackupClient1       | client1-fd          | F     | T      | 2026-01-30 02:00:00

[BVFS] Step 2: Updating BVFS cache for JobId 123...
[BVFS] Cache updated successfully

[BVFS] Step 3: Getting restore chain JobIDs...
[BVFS] Restore chain JobIDs: 123,122,121

[BVFS] Step 4: Listing directories at root (/)...
[BVFS] Directories at /:
  PathId | Name
  -------|--------------------
       1 | etc
       2 | var
       3 | home

[BVFS] Step 5: Listing files...
[BVFS] Files:
  FileId | Name                          | Size       | MTime
  -------|-------------------------------|------------|--------------------
     456 | passwd                        |       2048 | 2026-01-29 10:30:00

[BVFS] Step 6: Getting versions of file: passwd
[BVFS] File Versions:
  JobId | FileId | MTime               | Size
  ------|--------|---------------------|------------
    123 |    456 | 2026-01-29 10:30:00 |       2048
    120 |    400 | 2026-01-22 10:30:00 |       2000

====================================
[BVFS] Exploration complete
```

### director_test.sh

Automatisiertes Testskript für Director State-Machine-Tests.

**Verwendung:**
```bash
./director_test.sh [options]

Optionen:
  -m, --mode <mode>     Auth-Modus: legacy, psk, all (default: legacy)
  -n, --count <n>       Anzahl Iterationen (default: 5)
  -t, --test <test>     Test: connect, status, jobs, clients, all
  -p, --password <pwd>  Passwort (oder BAREOS_PASSWORD env)
  -H, --host <host>     Director-Host (default: localhost)
  -P, --port <port>     Director-Port (default: 9101)
  -v, --verbose         Ausführliche Ausgabe
```

**Beispiele:**
```bash
# Volltest Legacy-Modus
BAREOS_PASSWORD=mypassword ./director_test.sh -m legacy -t all -n 5

# Nur Verbindungstests
./director_test.sh -m legacy -t connect -p mypassword -n 10
```

---

## 3. Director Console-Konfigurationen

Im Verzeichnis `configs/` befinden sich Beispielkonfigurationen für den Bareos Director.

### onesimus-legacy.conf

Legacy-Modus (ohne TLS):
```
Console {
  Name = onesimus
  Password = "..."
  TLS Enable = no
  TLS Require = no
  CommandACL = *all*
  ...
}
```

### onesimus-psk.conf

TLS-PSK-Modus:
```
Console {
  Name = onesimus-psk
  Password = "..."
  TLS Enable = yes
  TLS Require = yes
  CommandACL = *all*
  ...
}
```

### onesimus-cert.conf

Zertifikat-Modus:
```
Console {
  Name = onesimus-cert
  Password = "..."
  TLS Enable = yes
  TLS Require = yes
  TLS Verify Peer = yes
  TLS CA Certificate File = /etc/bareos/ssl/bareos-ca.pem
  TLS Certificate = /etc/bareos/ssl/bareos-client.pem
  TLS Key = /etc/bareos/ssl/bareos-client.key
  ...
}
```

**Installation:**
```bash
sudo cp configs/onesimus-*.conf /etc/bareos/bareos-dir.d/console/
sudo systemctl reload bareos-dir
```

---

## 4. Testdaten-Generator

### generate_bareos_testdata.py

Generiert realistische Testdaten für die Bareos-Datenbank.

**Unternehmensgrößen:**
| Größe | Server | Tage | Jobs (geschätzt) |
|-------|--------|------|------------------|
| `small` | ~5 | 30 | ~150 |
| `medium` | ~12 | 60 | ~500 |
| `large` | 50-200 | 90 | ~5.000 |

**Generierte Ressourcen:**
- Clients (Server mit File Daemon)
- Jobs (Backup-Jobs mit realistischen Daten)
- Job Logs (Detaillierte Log-Einträge)
- Pools und Filesets

**Verwendung:**
```bash
# Voraussetzungen
pip install psycopg2-binary  # PostgreSQL
pip install mysql-connector-python  # MySQL

# Mittelgroße Firma generieren
python generate_bareos_testdata.py --size medium \
    --db-type postgresql --user bareos --password geheim

# Mit Umgebungsvariablen
export DB_TYPE=postgresql
export DB_USER=bareos
export DB_PASSWORD=geheim
python generate_bareos_testdata.py --size large

# Testdaten löschen
python generate_bareos_testdata.py --cleanup
```

**Weitere Optionen:**
| Parameter | Beschreibung |
|-----------|--------------|
| `--success-rate <n>` | Erfolgsrate in % (default: realistisch) |
| `--days <n>` | Tage Backup-Historie |
| `--no-logs` | Keine Job-Logs (schneller) |
| `--cleanup` | Testdaten löschen |

---

## 5. Build-Anleitung

```bash
cd build

# Nur Auth-Test bauen
make bareosauth_test

# Nur Director-Test bauen
make director_test

# Beide Tests bauen
make bareosauth_test director_test
```

---

## 6. Schnellstart

```bash
# 1. Tests bauen
cd build
make bareosauth_test director_test

# 2. Skripte kopieren
cp ../test/*.sh .

# 3. Auth-Test ausführen
BAREOS_PASSWORD=mypassword ./bareos_auth.sh -m legacy -n 5

# 4. Director-Test ausführen
BAREOS_PASSWORD=mypassword ./director_test.sh -m legacy -t all -n 3
```

---

## Lizenz

Teil des Onesimus-Projekts - Copyright (C) 2026 Joerg Bernau
