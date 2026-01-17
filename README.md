# Bacula Qt UI

Eine moderne Qt-Benutzeroberfläche für Bacula Backup Management System.

## Features

- **Duale Verbindungsmöglichkeiten:**
  - Bconsole TCP-Verbindung (Port 9101)
  - REST-API-Verbindung (HTTP/HTTPS)

- **TLS/SSL-Verschlüsselung:**
  - Verschlüsselte bconsole-Verbindungen
  - Client-Zertifikat-Authentifizierung
  - CA-Validierung

- **Persistente Verbindungseinstellungen:**
  - Automatisches Speichern der letzten Verbindung
  - Schnelles Wiederverbinden mit Ctrl+L
  - Automatische Verbindung beim Start

- **Job-Management:**
  - Jobs anzeigen und filtern
  - Jobs starten und abbrechen
  - Job-Details anzeigen
  - Status-Überwachung mit Farbcodierung

- **Client-Management:**
  - Client-Liste anzeigen
  - Client-Status abfragen
  - Client-Informationen verwalten

- **Storage/Volume-Management:**
  - Volume-Übersicht
  - Pool-Verwaltung
  - Speicherplatz-Anzeige

## Voraussetzungen

### System-Anforderungen

- CMake >= 3.16
- Qt6 (Core, Gui, Widgets, Network, Sql)
- C++17-kompatibler Compiler (GCC, Clang, MSVC)
- Bacula Director >= 9.x

### Ubuntu/Debian Installation

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    qt6-base-dev \
    qt6-tools-dev \
    libqt6network6 \
    libqt6sql6
```

### Fedora/RHEL Installation

```bash
sudo dnf install -y \
    gcc-c++ \
    cmake \
    qt6-qtbase-devel \
    qt6-qttools-devel
```

## Kompilierung

```bash
# Repository klonen oder entpacken
cd bacula-qt-ui

# Build-Verzeichnis erstellen
mkdir build
cd build

# CMake konfigurieren
cmake ..

# Kompilieren
make -j$(nproc)

# Optional: Installieren
sudo make install
```

## Verwendung

### Starten der Anwendung

```bash
# Aus dem Build-Verzeichnis
./BaculaQtUI

# Oder nach Installation
BaculaQtUI
```

### Verbindung herstellen

Die Anwendung versucht beim Start automatisch, mit der zuletzt verwendeten Verbindung zu verbinden.

#### Option 1: Letzte Verbindung (empfohlen)

- Klicken Sie auf "Letzte Verbindung" (oder drücken Sie Ctrl+L)
- Die App verbindet automatisch mit den gespeicherten Einstellungen

#### Option 2: Neue/Geänderte Verbindung

#### Bconsole-Verbindung (TCP)

1. Klicken Sie auf "Verbinden" oder Ctrl+O
2. Wählen Sie "Bconsole (TCP)"
3. Geben Sie die Verbindungsdaten ein:
   - **Host:** IP-Adresse oder Hostname des Directors (Standard: localhost)
   - **Port:** Bconsole-Port (Standard: 9101)
   - **Director Name:** Name des Directors (z.B. "bacula-dir")
   - **Passwort:** Bconsole-Passwort aus der Konfiguration
4. Klicken Sie auf "OK"

#### REST-API-Verbindung

1. Klicken Sie auf "Verbinden" oder Ctrl+O
2. Wählen Sie "REST API (HTTP/HTTPS)"
3. Geben Sie die Verbindungsdaten ein:
   - **Base URL:** API-Basis-URL (z.B. "http://localhost:9101" oder "https://bacula.example.com")
   - **Benutzername:** API-Benutzername
   - **Passwort:** API-Passwort
4. Klicken Sie auf "OK"

## BaculaDirector-Klasse

Die `BaculaDirector`-Klasse ist das Herzstück der Anwendung und bietet eine einheitliche Schnittstelle für beide Verbindungstypen.

### Wichtige Methoden

#### Verbindungsverwaltung

```cpp
// Bconsole-Verbindung
director->connectBConsole("localhost", 9101, "bacula-dir", "password");

// REST-API-Verbindung
director->connectRestAPI("http://localhost:9101", "admin", "password");

// Verbindung trennen
director->disconnect();

// Status prüfen
bool connected = director->isConnected();

// Gespeicherte Verbindung laden
if (director->hasStoredConnection()) {
    director->loadConnectionSettings();
    // Verbindung wird mit geladenen Einstellungen hergestellt
}
```

#### Bconsole-Befehle

```cpp
// Jobs auflisten
director->listJobs(100);  // Limit: 100 Jobs

// Clients auflisten
director->listClients();

// Volumes auflisten
director->listVolumes();

// Job starten
director->runJob("BackupClient1");

// Job abbrechen
director->cancelJob(jobId);

// Status abfragen
director->statusDirector();
director->statusClient("client-fd");
director->statusStorage("File-Storage");
```

#### REST-API-Methoden

```cpp
// Jobs abrufen
director->restGetJobs();
director->restGetJobs("status=R");  // Filter für laufende Jobs

// Clients abrufen
director->restGetClients();

// Volumes abrufen
director->restGetVolumes();

// Job starten
QJsonObject params;
params["level"] = "Full";
director->restRunJob("BackupClient1", params);

// Job abbrechen
director->restCancelJob(jobId);
```

#### Signals

```cpp
// Verbunden
connect(director, &BaculaDirector::connected, this, &MyClass::onConnected);

// Getrennt
connect(director, &BaculaDirector::disconnected, this, &MyClass::onDisconnected);

// Fehler
connect(director, &BaculaDirector::connectionError, this, &MyClass::onError);

// Jobs empfangen
connect(director, &BaculaDirector::jobsReceived, this, &MyClass::onJobsReceived);

// Clients empfangen
connect(director, &BaculaDirector::clientsReceived, this, &MyClass::onClientsReceived);

// Volumes empfangen
connect(director, &BaculaDirector::volumesReceived, this, &MyClass::onVolumesReceived);
```

## Konfiguration des Bacula Directors

### Gespeicherte Verbindungseinstellungen

Die Anwendung speichert Verbindungsinformationen in QSettings:
- **Linux**: `~/.config/Bacula/BaculaQtUI.conf`
- **Windows**: Registry unter `HKEY_CURRENT_USER\Software\Bacula\BaculaQtUI`
- **macOS**: `~/Library/Preferences/com.Bacula.BaculaQtUI.plist`

⚠️ **Sicherheitshinweis**: Passwörter werden im Klartext gespeichert. Für Produktionsumgebungen siehe `QSETTINGS.md` für Verschlüsselungsempfehlungen.

Einstellungen zurücksetzen:
```bash
# Linux/macOS
rm ~/.config/Bacula/BaculaQtUI.conf

# Windows: Registry-Editor verwenden
```

### Bconsole-Zugriff

Stellen Sie sicher, dass die `bconsole.conf` korrekt konfiguriert ist:

```
Director {
  Name = bacula-dir
  DIRport = 9101
  address = localhost
  Password = "IhrPasswortHier"
}
```

### REST-API (Optional)

Wenn Sie die Bacula REST-API verwenden möchten, müssen Sie diese separat konfigurieren. 
Die API ist Teil von Bacula Enterprise oder kann über Drittanbieter-Lösungen bereitgestellt werden.

## Projekt-Struktur

```
bacula-qt-ui/
├── CMakeLists.txt          # CMake-Konfiguration
├── README.md               # Diese Datei
├── include/                # Header-Dateien
│   ├── baculadirector.h   # BaculaDirector-Klasse
│   ├── mainwindow.h       # Hauptfenster
│   ├── jobwidget.h        # Job-Widget
│   ├── clientwidget.h     # Client-Widget
│   └── storagewidget.h    # Storage-Widget
├── src/                    # Implementierungen
│   ├── main.cpp           # Hauptprogramm
│   ├── baculadirector.cpp
│   ├── mainwindow.cpp
│   ├── jobwidget.cpp
│   ├── clientwidget.cpp
│   └── storagewidget.cpp
└── ui/                     # Qt Designer UI-Dateien
    ├── mainwindow.ui
    ├── jobwidget.ui
    ├── clientwidget.ui
    └── storagewidget.ui
```

## Erweiterungsmöglichkeiten

### Neue Features hinzufügen

1. **Restore-Funktionalität erweitern:**
   - Datei-Browser für Restore
   - Restore-Jobs verwalten

2. **Scheduling:**
   - Schedule-Editor
   - Job-Zeitpläne verwalten

3. **Reporting:**
   - Statistiken und Berichte
   - Export-Funktionen

4. **Benachrichtigungen:**
   - Job-Benachrichtigungen
   - E-Mail-Integration

### BaculaDirector-Klasse erweitern

```cpp
// Beispiel: Neue Methode hinzufügen
void BaculaDirector::customCommand(const QString &cmd) {
    if (m_connectionType == RestAPI) {
        restRequest("/api/v1/custom", "POST", 
                   QJsonObject{{"command", cmd}});
    } else {
        sendCommand(cmd);
    }
}
```

## Fehlerbehebung

### Verbindungsprobleme

- Überprüfen Sie Firewall-Einstellungen (Port 9101)
- Verifizieren Sie die Bacula-Director-Konfiguration
- Prüfen Sie die Logs: `/var/log/bacula/bacula.log`

### Build-Fehler

```bash
# Qt6-Pakete finden
qmake6 --version

# CMake-Cache löschen
rm -rf build/
mkdir build && cd build
cmake ..
```

### Authentifizierung schlägt fehl

- Überprüfen Sie das Passwort in der `bconsole.conf`
- Stellen Sie sicher, dass der Director läuft: `systemctl status bacula-dir`

## Lizenz

Dieses Projekt ist Open Source. Bitte beachten Sie die Lizenzbedingungen von Bacula.

## Beiträge

Beiträge sind willkommen! Bitte erstellen Sie Pull Requests oder Issues auf GitHub.

## Support

Bei Fragen oder Problemen:
- Bacula-Dokumentation: https://www.bacula.org/documentation/
- Bacula-Community: https://www.bacula.org/community/
