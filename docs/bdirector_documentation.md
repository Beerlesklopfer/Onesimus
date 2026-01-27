# BDirector Klasse - Ausführliche Dokumentation

## Inhaltsverzeichnis

1. [Überblick](#überblick)
2. [Architektur](#architektur)
3. [Backend-Auswahl](#backend-auswahl)
4. [Enumerations](#enumerations)
5. [Datenstrukturen](#datenstrukturen)
6. [Öffentliche API](#öffentliche-api)
7. [Signals](#signals)
8. [Verwendungsbeispiele](#verwendungsbeispiele)
9. [TLS/SSL-Konfiguration](#tlsssl-konfiguration)
10. [Fehlerbehandlung](#fehlerbehandlung)
11. [Bareos vs. Bacula](#bareos-vs-bacula)

---

## Überblick

Die `BDirector`-Klasse ist das zentrale Interface für die Kommunikation mit Bacula/Bareos Director-Daemons. Sie bietet eine Qt-basierte, asynchrone API für:

- **Verbindungsverwaltung**: TCP/IP mit optionaler TLS-Verschlüsselung
- **Authentifizierung**: CRAM-MD5 Challenge-Response
- **Kommando-Ausführung**: Type-safe Enum + String-basiert
- **Daten-Parsing**: JSON und Text-Modi
- **Persistenz**: Verbindungseinstellungen speichern/laden

### Hauptmerkmale

| Feature | Beschreibung |
|---------|-------------|
| **Asynchron** | Alle Netzwerk-Operationen blockieren nicht |
| **Type-Safe** | Command-Enum verhindert Tippfehler |
| **Flexibel** | Unterstützt Bareos UND Bacula |
| **Sicher** | TLS-PSK und Zertifikat-basiert |
| **Qt-integriert** | Signal/Slot-basierte API |

---

## Architektur

### Komponenten-Übersicht

```
┌──────────────────────────────────────────────────────┐
│                    MainWindow                         │
│              (UI, verwendet BDirector)                │
└────────────────────┬─────────────────────────────────┘
                     │
                     │ connect() signals
                     ▼
┌──────────────────────────────────────────────────────┐
│                   BDirector                           │
│         (Koordiniert Verbindung & Kommandos)          │
└─────┬──────────────┬──────────────┬──────────────────┘
      │              │              │
      │              │              │
      ▼              ▼              ▼
┌──────────┐  ┌──────────────┐  ┌───────────────┐
│QSslSocket│  │ BareosAuth   │  │ Kommando-     │
│(Netzwerk)│  │oder BaculaAuth│  │ Verarbeitung  │
└──────────┘  └──────────────┘  └───────────────┘
```

### Datenfluss

#### Verbindungsaufbau

```
1. MainWindow::connect()
   └─> BDirector::connect(host, port, name, pass)
       └─> QSslSocket::connectToHost()
           └─> [Optional] TLS-Handshake
               └─> BareosAuth::authenticateDirector()
                   └─> CRAM-MD5 Challenge-Response
                       └─> BDirector::onAuthenticationSucceeded()
                           └─> Signal: authentificationSucceeded(true, msg)
```

#### Kommando-Ausführung

```
1. MainWindow: doSendCommand(Command::ListJobs)
   └─> BDirector::doSendCommand()
       └─> commandToString() → "list jobs"
           └─> QSslSocket::write()
               └─> [Director verarbeitet]
                   └─> QSslSocket::readyRead()
                       └─> BDirector::onReadyRead()
                           └─> processResponse()
                               └─> Signal: commandResponse(response, "")
```

### Thread-Modell

**Wichtig**: Die BDirector-Klasse ist **NICHT thread-safe**!

- Alle Methoden müssen aus dem **Qt-Event-Loop-Thread** aufgerufen werden
- Socket-Operationen sind asynchron, blockieren aber nicht
- Für Thread-sichere Aufrufe aus anderen Threads:

```cpp
// Aus anderem Thread
QMetaObject::invokeMethod(director, "doSendCommand",
    Qt::QueuedConnection,
    Q_ARG(BDirector::Command, BDirector::Command::ListJobs),
    Q_ARG(QString, ""));
```

---

## Backend-Auswahl

### Compile-Time Backend-Selektion

Die BDirector-Klasse unterstützt zwei Backends, die zur Compile-Zeit ausgewählt werden:

```cpp
// CMakeLists.txt oder qmake .pro
add_definitions(-DUSE_BAREOS_ONLY)  // Für Bareos
# ODER
add_definitions(-DUSE_BACULA_ONLY)  // Für Bacula
```

#### Makro-Expansion

```cpp
#ifdef USE_BACULA_ONLY
    #include "baculaauth.h"
    #define AUTH_CLASS BaculaAuth
#elif defined(USE_BAREOS_ONLY)
    #include "bareosauth.h"
    #define AUTH_CLASS BareosAuth
#else
    // Default: Bareos
    #include "bareosauth.h"
    #define AUTH_CLASS BareosAuth
#endif
```

### Unterschiede Bareos/Bacula

| Feature | Bareos | Bacula |
|---------|--------|--------|
| **TLS-PSK** | Standard seit 18.2 | Optional |
| **TLS-Reihenfolge** | PSK → Hello → CRAM-MD5 | Hello → CRAM-MD5 → TLS |
| **JSON-API** | Ausgereift, stabil | Experimentell |
| **Zusatz-Kommandos** | Configure, Export, Import | Nicht vorhanden |
| **Resource-Format** | `R_CONSOLE::name` | `Console::name` |

---

## Enumerations

### ConnectionState

Beschreibt den aktuellen Zustand der Director-Verbindung.

```cpp
enum ConnectionState {
    Disconnected,    // Keine Verbindung
    Connecting,      // TCP-Verbindung wird aufgebaut
    Authenticating,  // CRAM-MD5-Authentifizierung läuft
    Ready            // Verbunden, authentifiziert, bereit
};
```

#### State-Übergänge

```
Normal:      Disconnected → Connecting → Authenticating → Ready
Disconnect:  Ready → Disconnected
Fehler:      Authenticating → Disconnected (mit Error-Signal)
```

#### Verwendung

```cpp
// UI-Update basierend auf State
void MainWindow::updateUI() {
    switch (director->connectionState()) {
        case BDirector::Disconnected:
            connectButton->setEnabled(true);
            commandButtons->setEnabled(false);
            statusLabel->setText("Nicht verbunden");
            break;

        case BDirector::Connecting:
            connectButton->setEnabled(false);
            statusLabel->setText("Verbinde...");
            break;

        case BDirector::Authenticating:
            statusLabel->setText("Authentifiziere...");
            break;

        case BDirector::Ready:
            connectButton->setEnabled(false);
            commandButtons->setEnabled(true);
            statusLabel->setText("Verbunden");
            break;
    }
}
```

### ApiMode

Steuert das Format der Director-Antworten.

```cpp
enum ApiMode {
    Off = 0,         // Text (wie bconsole)
    Json = 1,        // Kompakt-JSON
    JsonPretty = 2   // Pretty-Print JSON
};
```

#### Modi im Detail

**Off (0) - Text-Modus**
```
JobId | Name        | Status
------+-------------+--------
1     | BackupJob   | OK
2     | RestoreJob  | Running
```
- **Vorteile**: Human-readable, kompatibel mit alten Clients
- **Nachteile**: Schwer zu parsen, inkonsistentes Format

**Json (1) - Kompakt-JSON**
```json
{"jobs":[{"jobid":1,"name":"BackupJob","status":"OK"},{"jobid":2,"name":"RestoreJob","status":"Running"}]}
```
- **Vorteile**: Einfaches Parsing, minimale Größe
- **Nachteile**: Schwer lesbar für Menschen
- **Empfohlen für**: Production, Netzwerk-Übertragung

**JsonPretty (2) - Pretty-Print JSON**
```json
{
  "jobs": [
    {
      "jobid": 1,
      "name": "BackupJob",
      "status": "OK"
    },
    {
      "jobid": 2,
      "name": "RestoreJob",
      "status": "Running"
    }
  ]
}
```
- **Vorteile**: Gut lesbar, einfaches Debugging
- **Nachteile**: Größere Datenmenge
- **Empfohlen für**: Development, Debugging

#### API-Modus setzen

```cpp
// Nach erfolgreicher Verbindung
connect(director, &BDirector::authentificationSucceeded, this,
    [this](bool success, const QString &msg) {
        if (success) {
            // JSON-Modus für einfaches Parsing
            director->setApiMode(BDirector::ApiMode::Json);

            // Jetzt Kommandos senden
            director->doSendCommand(BDirector::Command::ListJobs);
        }
    });
```

**Wichtig**: API-Modus MUSS nach Authentifizierung gesetzt werden!

### Command

Type-safe Enum für alle Director-Kommandos (>70 Kommandos).

#### Kategorien

| Kategorie | Kommandos | Verwendung |
|-----------|-----------|------------|
| **Connection** | ApiMode, Quit, Exit | Session-Management |
| **Status** | StatusDirector, StatusClient, StatusStorage | Laufzeit-Info |
| **List** | ListJobs, ListClients, ListVolumes | Catalog-Abfragen |
| **Job Control** | Run, RunYes, Cancel, Enable, Disable | Job-Steuerung |
| **Restore** | Restore, RestoreAll, RestoreSelect | Wiederherstellung |
| **Volume** | Label, Mount, Purge, Prune | Media-Verwaltung |
| **Console** | Show, ShowJobs, ShowClients | Config-Info |
| **Debug** | Estimate, Version, Memory, Trace | Testing/Debug |
| **Config** | Reload, Configure, Export, Import | Konfiguration |

#### Kommandos mit Parametern

```cpp
// Ohne Parameter
director->doSendCommand(BDirector::Command::ListJobs);
director->doSendCommand(BDirector::Command::StatusDirector);

// Mit Parameter
director->doSendCommand(BDirector::Command::ListJobsLast, "10");
director->doSendCommand(BDirector::Command::StatusClient, "client=client1-fd");
director->doSendCommand(BDirector::Command::Cancel, "jobid=123");

// Mehrere Parameter
director->doSendCommand(BDirector::Command::Run, "job=BackupJob level=Full yes");
```

#### Parameter-Formate

| Kommando | Format | Beispiel |
|----------|--------|----------|
| StatusClient | `client=<name>` | `client=client1-fd` |
| StatusStorage | `storage=<name>` | `storage=File` |
| Cancel | `jobid=<id>` | `jobid=123` |
| ListJobsLast | `<days>` | `10` |
| ListFiles | `jobid=<id>` | `jobid=456` |
| Run | `job=<name> [options]` | `job=BackupJob level=Full yes` |

### JobStatus

Status-Codes für Backup-Jobs (entsprechend Bareos/Bacula Catalog).

```cpp
enum JobStatus {
    Created,      // C - Erstellt, nicht gestartet
    Running,      // R - Läuft aktuell
    Blocked,      // B - Blockiert (wartet auf Resource)
    Terminated,   // T - Beendet (Status unklar)
    Waiting,      // F - Wartet auf Start
    Successful,   // T - Erfolgreich (mit OK-Flag)
    Error,        // E - Mit Fehlern beendet
    Fatal,        // f - Fataler Fehler
    Canceled,     // A - Abgebrochen
    Unknown       // Unbekannt
};
```

#### Single-Character-Codes (Bareos/Bacula)

| Code | Bedeutung | JobStatus-Enum |
|------|-----------|----------------|
| **C** | Created | Created |
| **R** | Running | Running |
| **B** | Blocked | Blocked |
| **T** | Terminated | Successful (wenn OK-Meldung) |
| **E** | Error | Error |
| **e** | Non-fatal error | Error |
| **f** | Fatal error | Fatal |
| **A** | Canceled (Aborted) | Canceled |
| **F** | Waiting for Client | Waiting |
| **S** | Waiting for Storage | Waiting |
| **m** | Waiting for Mount | Blocked |
| **M** | Waiting for Mount | Blocked |

#### UI-Darstellung

```cpp
QString getJobStatusText(BDirector::JobStatus status) {
    switch (status) {
        case BDirector::Running:
            return "🔄 Läuft";
        case BDirector::Successful:
            return "✅ Erfolgreich";
        case BDirector::Error:
            return "⚠️ Fehler";
        case BDirector::Fatal:
            return "❌ Fatal";
        case BDirector::Canceled:
            return "🚫 Abgebrochen";
        default:
            return "❓ Unbekannt";
    }
}

QColor getJobStatusColor(BDirector::JobStatus status) {
    switch (status) {
        case BDirector::Running:
            return QColor(0, 122, 255);  // Blau
        case BDirector::Successful:
            return QColor(52, 199, 89);  // Grün
        case BDirector::Error:
            return QColor(255, 149, 0);  // Orange
        case BDirector::Fatal:
        case BDirector::Canceled:
            return QColor(255, 59, 48);  // Rot
        default:
            return QColor(142, 142, 147); // Grau
    }
}
```

---

## Datenstrukturen

### JobInfo

Vollständige Informationen über einen Backup-Job aus der Catalog-Datenbank.

```cpp
struct JobInfo {
    quint64 jobId;          // Eindeutige Job-ID (Primary Key)
    QString name;           // Job-Name aus Config
    QString type;           // B/R/V/M/C/A
    QString level;          // F/I/D
    QString clientName;     // Client-Name
    QString status;         // Job-Status (Single-Char)
    QDateTime startTime;    // Startzeit
    QString duration;       // "HH:MM:SS"
    qint64 jobBytes;        // Bytes gesichert
    qint64 jobFiles;        // Dateien gesichert
};
```

#### Verwendung

```cpp
void MyWindow::displayJobs(const QList<BDirector::JobInfo> &jobs) {
    tableWidget->setRowCount(jobs.size());

    for (int i = 0; i < jobs.size(); ++i) {
        const auto &job = jobs[i];

        // Job-ID
        tableWidget->setItem(i, 0,
            new QTableWidgetItem(QString::number(job.jobId)));

        // Name
        tableWidget->setItem(i, 1,
            new QTableWidgetItem(job.name));

        // Status mit Icon
        QString statusText = getJobStatusText(parseStatus(job.status));
        auto statusItem = new QTableWidgetItem(statusText);
        statusItem->setForeground(getJobStatusColor(parseStatus(job.status)));
        tableWidget->setItem(i, 2, statusItem);

        // Größe formatiert
        QString size = formatBytes(job.jobBytes);
        tableWidget->setItem(i, 3,
            new QTableWidgetItem(size));

        // Startzeit formatiert
        QString time = job.startTime.toString("dd.MM.yyyy HH:mm");
        tableWidget->setItem(i, 4,
            new QTableWidgetItem(time));
    }
}

// Hilfsfunktion
QString formatBytes(qint64 bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    double size = bytes;

    while (size >= 1024 && unitIndex < 4) {
        size /= 1024;
        unitIndex++;
    }

    return QString("%1 %2")
        .arg(size, 0, 'f', 2)
        .arg(units[unitIndex]);
}
```

### ClientInfo

Informationen über einen Backup-Client (File Daemon).

```cpp
struct ClientInfo {
    QString name;           // Client-Name
    QString address;        // IP/Hostname
    int port;               // FD-Port (9102)
    QString os;             // Betriebssystem
    bool autoprune;         // Auto-Pruning aktiv
    int fileRetention;      // File-Retention (Sekunden)
    int jobRetention;       // Job-Retention (Sekunden)
};
```

#### Retention-Umrechnung

```cpp
QString formatRetention(int seconds) {
    int days = seconds / (60 * 60 * 24);

    if (days >= 365) {
        int years = days / 365;
        return QString("%1 Jahr(e)").arg(years);
    } else if (days >= 30) {
        int months = days / 30;
        return QString("%1 Monat(e)").arg(months);
    } else {
        return QString("%1 Tag(e)").arg(days);
    }
}

// Verwendung
QString fileRet = formatRetention(client.fileRetention);
QString jobRet = formatRetention(client.jobRetention);
```

### TLSConfig

Umfassende TLS/SSL-Konfiguration für verschlüsselte Verbindungen.

```cpp
struct TLSConfig {
    bool tlsEnable;          // TLS aktivieren
    bool tlsRequire;         // TLS erzwingen
    bool tlsPSKEnable;       // PSK-Modus (vs. Cert)
    bool tlsVerifyPeer;      // Peer-Zertifikat prüfen

    QSharedPointer<QFile> tlsCaCertFile;  // CA-Zertifikat

#ifdef Q_OS_WINDOWS
    QSharedPointer<QFile> tlsPfxFile;     // PFX-Datei
    QString tlsPfxPassword;               // PFX-Passwort
#else
    QSharedPointer<QFile> tlsCertFile;    // Client-Cert
    QSharedPointer<QFile> tlsKeyFile;     // Private Key
#endif
};
```

#### TLS-Modi

**PSK-Modus (empfohlen für Bareos)**
```cpp
BDirector::TLSConfig config;
config.tlsEnable = true;
config.tlsRequire = true;
config.tlsPSKEnable = true;      // PSK!
config.tlsVerifyPeer = false;    // Keine Zertifikat-Prüfung im PSK

director->setTLSConfig(config);
```

**Zertifikat-Modus (Linux/Unix)**
```cpp
BDirector::TLSConfig config;
config.tlsEnable = true;
config.tlsRequire = true;
config.tlsPSKEnable = false;     // Cert-Modus
config.tlsVerifyPeer = true;     // Zertifikat prüfen

// Zertifikate laden
config.tlsCaCertFile = QSharedPointer<QFile>(
    new QFile("/etc/bareos/tls/ca.crt"));
config.tlsCertFile = QSharedPointer<QFile>(
    new QFile("/etc/bareos/tls/console.crt"));
config.tlsKeyFile = QSharedPointer<QFile>(
    new QFile("/etc/bareos/tls/console.key"));

director->setTLSConfig(config);
```

**Zertifikat-Modus (Windows PFX)**
```cpp
BDirector::TLSConfig config;
config.tlsEnable = true;
config.tlsRequire = true;
config.tlsPSKEnable = false;
config.tlsVerifyPeer = true;

// PFX-Datei (enthält Cert + Key)
config.tlsPfxFile = QSharedPointer<QFile>(
    new QFile("C:/bareos/certs/console.pfx"));
config.tlsPfxPassword = "mypassword";

// Optional: CA
config.tlsCaCertFile = QSharedPointer<QFile>(
    new QFile("C:/bareos/certs/ca.crt"));

director->setTLSConfig(config);
```

### VolumeInfo

Informationen über Storage-Volumes (Bänder/Festplatten).

```cpp
struct VolumeInfo {
    QString volumeName;      // Volume-Label
    QString poolName;        // Pool-Name
    QString mediaType;       // File/Tape/Cloud
    QString status;          // Append/Full/Used/etc.
    qint64 volumeBytes;      // Belegt
    qint64 maxVolumeBytes;   // Maximum (0=unbegrenzt)
    int volRetention;        // Retention (Sekunden)
};
```

#### Volume-Status-Codes

| Status | Bedeutung | Action |
|--------|-----------|--------|
| **Append** | Bereit für Backups | ✅ Kann verwendet werden |
| **Full** | Voll | ❌ Recyclen oder neues Volume |
| **Used** | Enthält Daten, Retention abgelaufen | ⚠️ Kann gepruned/recycled werden |
| **Recycle** | Bereit zum Recyclen | ✅ Wird automatisch wiederverwendet |
| **Purged** | Catalog-Daten gelöscht | ✅ Kann gelabelt werden |
| **Archive** | Archiviert | ❌ Nicht überschreiben! |
| **Error** | Fehler | ⚠️ Prüfen! |

#### Volume-Auslastung

```cpp
void displayVolume(const BDirector::VolumeInfo &vol) {
    // Prozent berechnen
    double usagePercent = 0;
    if (vol.maxVolumeBytes > 0) {
        usagePercent = (vol.volumeBytes * 100.0) / vol.maxVolumeBytes;
    }

    // Farbe basierend auf Auslastung
    QColor color;
    if (usagePercent < 70) {
        color = Qt::green;
    } else if (usagePercent < 90) {
        color = Qt::yellow;
    } else {
        color = Qt::red;
    }

    // Anzeigen
    qDebug() << vol.volumeName
             << "(" << vol.status << ")"
             << formatBytes(vol.volumeBytes)
             << "/"
             << formatBytes(vol.maxVolumeBytes)
             << QString("(%1%)").arg(usagePercent, 0, 'f', 1);
}
```

---

## Öffentliche API

### Konstruktor / Destruktor

```cpp
explicit BDirector(QObject *parent = nullptr);
~BDirector();
```

#### Beispiel

```cpp
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent)
    {
        // BDirector erstellen (this = parent für auto-cleanup)
        m_director = new BDirector(this);

        // Signals verbinden
        setupSignals();
    }

private:
    BDirector *m_director;
};
```

### Verbindungsverwaltung

#### connect()

```cpp
void connect(const QString &host,
             int port,
             const QString &directorName,
             const QString &password);
```

**Parameter:**
- `host`: IP-Adresse oder Hostname (z.B. "192.168.1.10", "bareos.example.com")
- `port`: TCP-Port (Standard: 9101)
- `directorName`: Name aus Director-Config
- `password`: Console-Passwort

**Wichtig:**
- Funktion ist **asynchron**, kehrt sofort zurück
- TLS-Config MUSS vorher gesetzt werden
- Ergebnis kommt über `authentificationSucceeded` Signal

**Beispiel:**
```cpp
// TLS konfigurieren
BDirector::TLSConfig tlsConfig;
tlsConfig.tlsEnable = true;
tlsConfig.tlsPSKEnable = true;
director->setTLSConfig(tlsConfig);

// Signals verbinden
connect(director, &BDirector::authentificationSucceeded, this,
    [this](bool success, const QString &msg) {
        if (success) {
            qDebug() << "Connected!";
            director->setApiMode(BDirector::ApiMode::Json);
        } else {
            QMessageBox::critical(this, "Error", msg);
        }
    });

// Verbinden
director->connect("192.168.1.10", 9101, "bareos-dir", "mypassword");
```

#### disconnect()

```cpp
void disconnect();
```

Trennt die Verbindung zum Director.

**Was passiert:**
1. "quit" Kommando an Director senden
2. Socket schließen
3. State → Disconnected
4. Signal `disconnected()` emittieren

**Beispiel:**
```cpp
connect(director, &BDirector::disconnected, this,
    []() {
        qDebug() << "Disconnected";
    });

director->disconnect();
```

#### isConnected()

```cpp
bool isConnected() const;
```

**Returns:** `true` wenn ConnectionState == Ready

**Beispiel:**
```cpp
if (director->isConnected()) {
    director->doSendCommand(BDirector::Command::ListJobs);
} else {
    QMessageBox::warning(this, "Not Connected",
        "Please connect to Director first");
}
```

#### connectionState()

```cpp
ConnectionState connectionState() const;
```

**Returns:** Aktueller ConnectionState

**Beispiel:**
```cpp
void updateStatusBar() {
    QString text;
    QColor color;

    switch (director->connectionState()) {
        case BDirector::Disconnected:
            text = "Nicht verbunden";
            color = Qt::red;
            break;
        case BDirector::Connecting:
            text = "Verbinde...";
            color = Qt::yellow;
            break;
        case BDirector::Authenticating:
            text = "Authentifiziere...";
            color = Qt::yellow;
            break;
        case BDirector::Ready:
            text = "Verbunden";
            color = Qt::green;
            break;
    }

    statusLabel->setText(text);
    statusLabel->setStyleSheet(
        QString("color: %1; font-weight: bold;").arg(color.name()));
}
```

### TLS-Konfiguration

#### setTLSConfig()

```cpp
void setTLSConfig(const TLSConfig &config);
```

MUSS vor `connect()` aufgerufen werden!

**Beispiel:**
```cpp
// PSK-Modus (einfach)
BDirector::TLSConfig config;
config.tlsEnable = true;
config.tlsRequire = true;
config.tlsPSKEnable = true;
director->setTLSConfig(config);

// Jetzt verbinden
director->connect(...);
```

#### tlsConfig()

```cpp
TLSConfig *tlsConfig() const;
```

Gibt Pointer auf aktuelle TLS-Config zurück.

**Beispiel:**
```cpp
TLSConfig *config = director->tlsConfig();
qDebug() << "TLS enabled:" << config->tlsEnable;
qDebug() << "PSK mode:" << config->tlsPSKEnable;
```

### Persistenz

#### saveConnectionSettings()

```cpp
void saveConnectionSettings();
```

Speichert Verbindungseinstellungen in QSettings.

**Gespeichert:**
- Host
- Port
- Director-Name
- TLS-Einstellungen

**NICHT gespeichert:**
- Passwort (Sicherheit!)

**Speicherort:**
- Linux: `~/.config/Onesimus/Onesimus.conf`
- Windows: Registry `HKEY_CURRENT_USER\Software\Onesimus`
- macOS: `~/Library/Preferences/com.onesimus.Onesimus.plist`

**Beispiel:**
```cpp
// Nach erfolgreicher Verbindung speichern
connect(director, &BDirector::authentificationSucceeded, this,
    [this](bool success, const QString &msg) {
        if (success) {
            director->saveConnectionSettings();
            qDebug() << "Connection settings saved";
        }
    });
```

#### loadConnectionSettings()

```cpp
void loadConnectionSettings();
```

Lädt gespeicherte Einstellungen. Stellt KEINE Verbindung her!

**Beispiel:**
```cpp
if (director->hasStoredConnection()) {
    director->loadConnectionSettings();

    // Passwort neu abfragen
    bool ok;
    QString password = QInputDialog::getText(this,
        "Password",
        "Enter password:",
        QLineEdit::Password,
        "",
        &ok);

    if (ok && !password.isEmpty()) {
        // Mit geladenem Config + neuem Passwort verbinden
        director->connect(/* host aus config */,
                         /* port aus config */,
                         /* director aus config */,
                         password);
    }
}
```

#### hasStoredConnection()

```cpp
bool hasStoredConnection() const;
```

Prüft ob gespeicherte Einstellungen existieren.

**Beispiel:**
```cpp
void MainWindow::setupUI() {
    if (director->hasStoredConnection()) {
        // "Zuletzt verwendet"-Button anzeigen
        connectLastUsedButton->setVisible(true);
        connectLastUsedButton->setText("Zuletzt verwendet");
    } else {
        connectLastUsedButton->setVisible(false);
    }
}
```

### API-Modus

#### setApiMode()

```cpp
void setApiMode(ApiMode mode);
```

Setzt API-Ausgabemodus. MUSS nach Authentifizierung aufgerufen werden!

**Beispiel:**
```cpp
connect(director, &BDirector::authentificationSucceeded, this,
    [this](bool success, const QString &msg) {
        if (success) {
            // JSON-Modus für strukturierte Antworten
            director->setApiMode(BDirector::ApiMode::Json);

            // Jetzt Kommandos senden
            director->doSendCommand(BDirector::Command::ListJobs);
        }
    });
```

#### apiMode()

```cpp
ApiMode apiMode() const;
```

Gibt aktuellen API-Modus zurück.

**Beispiel:**
```cpp
if (director->apiMode() == BDirector::ApiMode::Json) {
    // JSON-Parsing
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
} else {
    // Text-Parsing
    QStringList lines = response.split('\n');
}
```

### Kommando-Ausführung

#### doSendCommand() - Enum-basiert

```cpp
void doSendCommand(const BDirector::Command cmd,
                   const QString &args = QString());
```

Haupt-Methode zum Senden von Kommandos.

**Beispiele:**
```cpp
// Ohne Parameter
director->doSendCommand(BDirector::Command::ListJobs);
director->doSendCommand(BDirector::Command::StatusDirector);
director->doSendCommand(BDirector::Command::Messages);

// Mit Parameter
director->doSendCommand(BDirector::Command::ListJobsLast, "10");
director->doSendCommand(BDirector::Command::StatusClient, "client=client1-fd");
director->doSendCommand(BDirector::Command::Cancel, "jobid=123");

// Mehrere Parameter
director->doSendCommand(BDirector::Command::Run, "job=BackupJob level=Full yes");
```

**Antwort empfangen:**
```cpp
// Text-Antwort
connect(director, &BDirector::commandResponse, this,
    [](const QString &response, const QString &error) {
        if (error.isEmpty()) {
            qDebug() << "Response:" << response;
        } else {
            qWarning() << "Error:" << error;
        }
    });

// JSON-Antwort (wenn API-Mode = Json)
connect(director, &BDirector::jsonResponse, this,
    [](const QString &response, const QString &error) {
        if (error.isEmpty()) {
            QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
            // Verarbeite JSON
        }
    });
```

### Backend-Info

#### backupSystemName()

```cpp
QString backupSystemName() const;
```

Gibt "Bareos" oder "Bacula" zurück (je nach kompiliertem Backend).

**Beispiel:**
```cpp
QString system = director->backupSystemName();
aboutDialog->setText(QString("Connected to %1").arg(system));
```

---

## Signals

### authentificationSucceeded

```cpp
void authentificationSucceeded(const bool result, const QString &msg);
```

Emittiert nach Authentifizierungs-Versuch.

**Parameter:**
- `result`: `true` bei Erfolg, `false` bei Fehler
- `msg`: Erfolgs-/Fehlermeldung

**Beispiel:**
```cpp
connect(director, &BDirector::authentificationSucceeded, this,
    [this](bool result, const QString &msg) {
        if (result) {
            qDebug() << "Connected:" << msg;
            statusLabel->setText("Verbunden");

            // API-Modus setzen
            director->setApiMode(BDirector::ApiMode::Json);

            // Erste Kommandos senden
            director->doSendCommand(BDirector::Command::ListJobs);
        } else {
            qCritical() << "Connection failed:" << msg;
            QMessageBox::critical(this, "Verbindungsfehler", msg);
        }
    });
```

### protocolError

```cpp
void protocolError(const QString &msg);
```

Emittiert bei Protokoll- oder Netzwerk-Fehlern.

**Beispiele für Fehler:**
- Socket-Fehler (Connection refused, Timeout)
- TLS-Fehler (Handshake fehlgeschlagen)
- Protokoll-Fehler (Ungültige Nachricht)

**Beispiel:**
```cpp
connect(director, &BDirector::protocolError, this,
    [this](const QString &msg) {
        qCritical() << "Protocol error:" << msg;

        // Optional: Auto-Reconnect
        if (autoReconnect) {
            QTimer::singleShot(5000, this, [this]() {
                qDebug() << "Attempting reconnect...";
                director->connect(host, port, dirName, password);
            });
        }
    });
```

### disconnected

```cpp
void disconnected();
```

Emittiert bei Verbindungsabbruch.

**Gründe:**
- `disconnect()` aufgerufen
- Director hat Verbindung beendet
- Socket-Fehler

**Beispiel:**
```cpp
connect(director, &BDirector::disconnected, this,
    [this]() {
        qDebug() << "Disconnected";
        statusLabel->setText("Nicht verbunden");
        statusLabel->setStyleSheet("color: red;");
        commandButtons->setEnabled(false);
    });
```

### commandResponse

```cpp
void commandResponse(const QString &response, const QString &error);
```

Emittiert bei Text-Antworten (wenn ApiMode != Json).

**Parameter:**
- `response`: Antwort-Text vom Director
- `error`: Fehlermeldung (leer bei Erfolg)

**Beispiel:**
```cpp
connect(director, &BDirector::commandResponse, this,
    [this](const QString &response, const QString &error) {
        if (error.isEmpty()) {
            // Erfolgreiche Antwort
            outputTextEdit->setPlainText(response);
        } else {
            // Fehler
            QMessageBox::warning(this, "Command Error", error);
        }
    });
```

### jsonResponse

```cpp
void jsonResponse(const QString &response, const QString &error);
```

Emittiert bei JSON-Antworten (wenn ApiMode == Json oder JsonPretty).

**Parameter:**
- `response`: JSON-String
- `error`: Fehlermeldung (leer bei Erfolg)

**Beispiel:**
```cpp
connect(director, &BDirector::jsonResponse, this,
    [this](const QString &response, const QString &error) {
        if (error.isEmpty()) {
            // JSON parsen
            QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());

            if (doc.isNull()) {
                qWarning() << "Invalid JSON:" << response;
                return;
            }

            QJsonObject obj = doc.object();

            // Jobs verarbeiten
            if (obj.contains("jobs")) {
                QJsonArray jobs = obj["jobs"].toArray();
                processJobs(jobs);
            }
        } else {
            QMessageBox::warning(this, "JSON Error", error);
        }
    });
```

### statusMessage

```cpp
void statusMessage(const QString &message);
```

Emittiert für Status-Updates während Operationen.

**Beispiel-Nachrichten:**
- "Connecting..."
- "Authenticating..."
- "Sending command..."
- "Waiting for response..."

**Beispiel:**
```cpp
// Direkt an Status-Label binden
connect(director, &BDirector::statusMessage,
        statusLabel, &QLabel::setText);
```

### commandError

```cpp
void commandError(const QString &command, const QString &error);
```

Emittiert bei Kommando-spezifischen Fehlern.

**Beispiel:**
```cpp
connect(director, &BDirector::commandError, this,
    [](const QString &command, const QString &error) {
        qWarning() << "Command failed:" << command;
        qWarning() << "Error:" << error;

        // User-friendly Fehlermeldung
        QMessageBox::warning(nullptr, "Kommando fehlgeschlagen",
            QString("Kommando '%1' fehlgeschlagen:\n%2")
                .arg(command, error));
    });
```

### jobStatusChanged

```cpp
void jobStatusChanged(int jobId, BDirector::JobStatus status);
```

Emittiert bei Job-Status-Änderungen.

**Hinweis:** Derzeit nicht implementiert (geplant für zukünftige Versionen).

### authenticationRequired

```cpp
void authenticationRequired();
```

Emittiert wenn Authentifizierung benötigt wird.

**Hinweis:** Derzeit nicht verwendet (reserved for future use).

---

## Verwendungsbeispiele

### Basis-Setup

```cpp
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent)
    {
        setupUI();

        // BDirector erstellen
        m_director = new BDirector(this);

        // TLS konfigurieren
        BDirector::TLSConfig tlsConfig;
        tlsConfig.tlsEnable = true;
        tlsConfig.tlsRequire = true;
        tlsConfig.tlsPSKEnable = true;
        m_director->setTLSConfig(tlsConfig);

        // Signals verbinden
        connect(m_director, &BDirector::authentificationSucceeded,
                this, &MainWindow::onConnected);
        connect(m_director, &BDirector::disconnected,
                this, &MainWindow::onDisconnected);
        connect(m_director, &BDirector::protocolError,
                this, &MainWindow::onError);
        connect(m_director, &BDirector::jsonResponse,
                this, &MainWindow::onJsonResponse);
        connect(m_director, &BDirector::statusMessage,
                m_statusLabel, &QLabel::setText);
    }

private slots:
    void onConnectClicked() {
        m_director->connect("192.168.1.10", 9101,
                           "bareos-dir", "password");
    }

    void onConnected(bool success, const QString &msg) {
        if (success) {
            qDebug() << "Connected!";
            m_director->setApiMode(BDirector::ApiMode::Json);
            m_director->doSendCommand(BDirector::Command::ListJobs);
        } else {
            QMessageBox::critical(this, "Error", msg);
        }
    }

    void onDisconnected() {
        qDebug() << "Disconnected";
    }

    void onError(const QString &error) {
        QMessageBox::critical(this, "Error", error);
    }

    void onJsonResponse(const QString &json, const QString &error) {
        if (!error.isEmpty()) {
            qWarning() << "Error:" << error;
            return;
        }

        // JSON verarbeiten
        QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
        // ...
    }

private:
    BDirector *m_director;
    QLabel *m_statusLabel;
};
```

### Job-Liste anzeigen

```cpp
void MainWindow::loadJobs() {
    // Signal für JSON-Antwort verbinden
    connect(m_director, &BDirector::jsonResponse, this,
        [this](const QString &json, const QString &error) {
            if (!error.isEmpty()) {
                qWarning() << "Error loading jobs:" << error;
                return;
            }

            // JSON parsen
            QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
            QJsonObject obj = doc.object();

            if (!obj.contains("jobs")) {
                qWarning() << "No jobs in response";
                return;
            }

            QJsonArray jobs = obj["jobs"].toArray();

            // Tabelle leeren
            m_jobTable->setRowCount(0);
            m_jobTable->setRowCount(jobs.size());

            // Jobs anzeigen
            for (int i = 0; i < jobs.size(); ++i) {
                QJsonObject job = jobs[i].toObject();

                // Job-ID
                m_jobTable->setItem(i, 0,
                    new QTableWidgetItem(
                        QString::number(job["jobid"].toInt())));

                // Name
                m_jobTable->setItem(i, 1,
                    new QTableWidgetItem(job["name"].toString()));

                // Status
                QString status = job["jobstatus"].toString();
                QTableWidgetItem *statusItem = new QTableWidgetItem(
                    getStatusText(status));
                statusItem->setForeground(getStatusColor(status));
                m_jobTable->setItem(i, 2, statusItem);

                // Größe
                qint64 bytes = job["jobbytes"].toString().toLongLong();
                m_jobTable->setItem(i, 3,
                    new QTableWidgetItem(formatBytes(bytes)));
            }
        });

    // Kommando senden
    m_director->doSendCommand(BDirector::Command::ListJobs);
}
```

### Job starten

```cpp
void MainWindow::runJob(const QString &jobName) {
    // Bestätigung
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Job starten",
        QString("Job '%1' starten?").arg(jobName),
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    // Signal für Antwort
    connect(m_director, &BDirector::commandResponse, this,
        [this, jobName](const QString &response, const QString &error) {
            if (!error.isEmpty()) {
                QMessageBox::critical(this, "Fehler",
                    QString("Job konnte nicht gestartet werden:\n%1")
                        .arg(error));
                return;
            }

            // Erfolgsmeldung
            if (response.contains("Job queued")) {
                QMessageBox::information(this, "Job gestartet",
                    QString("Job '%1' wurde gestartet").arg(jobName));
            }
        });

    // Job-Befehl zusammenstellen
    QString command = QString("job=%1 yes").arg(jobName);
    m_director->doSendCommand(BDirector::Command::RunYes, command);
}
```

### Job abbrechen

```cpp
void MainWindow::cancelJob(quint64 jobId) {
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Job abbrechen",
        QString("Job #%1 wirklich abbrechen?").arg(jobId),
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    // Antwort-Handler
    connect(m_director, &BDirector::commandResponse, this,
        [this, jobId](const QString &response, const QString &error) {
            if (!error.isEmpty()) {
                QMessageBox::warning(this, "Fehler",
                    QString("Job konnte nicht abgebrochen werden:\n%1")
                        .arg(error));
                return;
            }

            QMessageBox::information(this, "Job abgebrochen",
                QString("Job #%1 wurde abgebrochen").arg(jobId));

            // Job-Liste aktualisieren
            loadJobs();
        });

    // Cancel-Kommando
    QString command = QString("jobid=%1").arg(jobId);
    m_director->doSendCommand(BDirector::Command::Cancel, command);
}
```

### Auto-Reconnect

```cpp
class MainWindow : public QMainWindow {
public:
    MainWindow() {
        m_director = new BDirector(this);
        m_reconnectTimer = new QTimer(this);
        m_reconnectTimer->setSingleShot(true);

        connect(m_director, &BDirector::protocolError, this,
            &MainWindow::onConnectionLost);
        connect(m_director, &BDirector::disconnected, this,
            &MainWindow::onConnectionLost);
        connect(m_reconnectTimer, &QTimer::timeout, this,
            &MainWindow::attemptReconnect);
    }

private slots:
    void onConnectionLost() {
        if (m_autoReconnect && !m_reconnecting) {
            qDebug() << "Connection lost, reconnecting in 5 seconds...";
            m_reconnecting = true;
            m_reconnectTimer->start(5000);
        }
    }

    void attemptReconnect() {
        if (!m_lastHost.isEmpty()) {
            qDebug() << "Attempting reconnect...";
            m_director->connect(m_lastHost, m_lastPort,
                               m_lastDirector, m_lastPassword);
        }
        m_reconnecting = false;
    }

    void onConnectSuccess(bool success, const QString &msg) {
        if (success) {
            m_reconnecting = false;
            qDebug() << "Reconnected successfully";
        } else if (m_autoReconnect && m_reconnecting) {
            // Erneut versuchen
            qDebug() << "Reconnect failed, retrying in 10 seconds...";
            m_reconnectTimer->start(10000);
        }
    }

private:
    BDirector *m_director;
    QTimer *m_reconnectTimer;
    bool m_autoReconnect = true;
    bool m_reconnecting = false;

    // Letzte Verbindungsdaten
    QString m_lastHost;
    int m_lastPort;
    QString m_lastDirector;
    QString m_lastPassword;
};
```

---

## TLS/SSL-Konfiguration

### Überblick

Die BDirector-Klasse unterstützt zwei TLS-Modi:

1. **TLS-PSK** (Pre-Shared Key): Einfach, keine Zertifikate
2. **TLS-Cert** (Zertifikat-basiert): Sicherer, aber komplexer

### TLS-PSK (empfohlen für Bareos)

**Vorteile:**
- Einfaches Setup, keine PKI-Infrastruktur
- Password = Pre-Shared Key
- Standard bei Bareos seit 18.2

**Setup:**
```cpp
BDirector::TLSConfig config;
config.tlsEnable = true;       // TLS aktivieren
config.tlsRequire = true;      // TLS erzwingen
config.tlsPSKEnable = true;    // PSK-Modus
config.tlsVerifyPeer = false;  // Keine Zertifikat-Prüfung

director->setTLSConfig(config);
director->connect("192.168.1.10", 9101, "bareos-dir", "password");
```

**PSK-Details:**
- Identity: `R_CONSOLE::<console-name>`
- Key: `MD5(password)`
- Cipher: TLS-PSK-WITH-AES-xxx (je nach OpenSSL)

### TLS-Cert (Linux/Unix)

**Setup:**
```cpp
BDirector::TLSConfig config;
config.tlsEnable = true;
config.tlsRequire = true;
config.tlsPSKEnable = false;    // Cert-Modus!
config.tlsVerifyPeer = true;    // Zertifikat prüfen

// Zertifikate
config.tlsCaCertFile = QSharedPointer<QFile>(
    new QFile("/etc/bareos/tls/ca.crt"));
config.tlsCertFile = QSharedPointer<QFile>(
    new QFile("/etc/bareos/tls/console.crt"));
config.tlsKeyFile = QSharedPointer<QFile>(
    new QFile("/etc/bareos/tls/console.key"));

// Zertifikate müssen existieren!
if (!config.tlsCaCertFile->exists() ||
    !config.tlsCertFile->exists() ||
    !config.tlsKeyFile->exists()) {
    qCritical() << "Certificate files missing!";
    return;
}

director->setTLSConfig(config);
```

**Zertifikate erstellen:**
```bash
# CA erstellen
openssl genrsa -out ca.key 4096
openssl req -new -x509 -days 3650 -key ca.key -out ca.crt

# Console-Zertifikat
openssl genrsa -out console.key 2048
openssl req -new -key console.key -out console.csr
openssl x509 -req -in console.csr -CA ca.crt -CAkey ca.key \
    -CAcreateserial -out console.crt -days 365
```

### TLS-Cert (Windows PFX)

**Setup:**
```cpp
BDirector::TLSConfig config;
config.tlsEnable = true;
config.tlsRequire = true;
config.tlsPSKEnable = false;
config.tlsVerifyPeer = true;

// PFX-Datei (enthält Cert + Private Key)
config.tlsPfxFile = QSharedPointer<QFile>(
    new QFile("C:/bareos/certs/console.pfx"));
config.tlsPfxPassword = "mypfxpassword";

// Optional: CA-Zertifikat
config.tlsCaCertFile = QSharedPointer<QFile>(
    new QFile("C:/bareos/certs/ca.crt"));

director->setTLSConfig(config);
```

**PFX erstellen (aus PEM):**
```bash
# Cert + Key zu PFX kombinieren
openssl pkcs12 -export -out console.pfx \
    -inkey console.key \
    -in console.crt \
    -certfile ca.crt \
    -password pass:mypfxpassword
```

### Director-Konfiguration

Die TLS-Einstellungen im Client müssen mit der Director-Config übereinstimmen!

**Director (bareos-dir.conf) mit PSK:**
```
Director {
  Name = bareos-dir
  ...
  TLS Enable = yes
  TLS Require = yes
  TLS PSK Enable = yes
}
```

**Director mit Zertifikaten:**
```
Director {
  Name = bareos-dir
  ...
  TLS Enable = yes
  TLS Require = yes
  TLS Certificate = /etc/bareos/tls/bareos-dir.crt
  TLS Key = /etc/bareos/tls/bareos-dir.key
  TLS CA Certificate File = /etc/bareos/tls/ca.crt
  TLS Verify Peer = yes
}
```

### Fehlerbehandlung

```cpp
// SSL-Fehler abfangen
connect(m_director, &BDirector::protocolError, this,
    [](const QString &error) {
        if (error.contains("SSL") || error.contains("TLS")) {
            qCritical() << "TLS Error:" << error;

            // Häufige TLS-Fehler:
            if (error.contains("certificate")) {
                qDebug() << "Hint: Check certificate files and permissions";
            } else if (error.contains("handshake")) {
                qDebug() << "Hint: Check TLS configuration on Director";
            } else if (error.contains("cipher")) {
                qDebug() << "Hint: OpenSSL PSK support required";
            }
        }
    });
```

---

## Fehlerbehandlung

### Fehler-Arten

| Fehler-Typ | Signal | Behandlung |
|------------|--------|------------|
| **Verbindungsfehler** | `protocolError` | Prüfe Netzwerk, Port, Firewall |
| **Auth-Fehler** | `authentificationSucceeded(false, ...)` | Prüfe Passwort, Director-Name |
| **TLS-Fehler** | `protocolError` | Prüfe Zertifikate, TLS-Config |
| **Kommando-Fehler** | `commandError` | Prüfe Kommando-Syntax, Parameter |
| **Socket-Fehler** | `protocolError` + `disconnected` | Netzwerk-Problem, Auto-Reconnect |

### Best Practices

**1. Alle Fehler-Signals verbinden:**
```cpp
void setupErrorHandling() {
    // Verbindungsfehler
    connect(m_director, &BDirector::protocolError, this,
        &MainWindow::onProtocolError);

    // Auth-Fehler
    connect(m_director, &BDirector::authentificationSucceeded, this,
        &MainWindow::onAuthResult);

    // Kommando-Fehler
    connect(m_director, &BDirector::commandError, this,
        &MainWindow::onCommandError);

    // Disconnect
    connect(m_director, &BDirector::disconnected, this,
        &MainWindow::onDisconnected);
}
```

**2. User-friendly Error-Messages:**
```cpp
void MainWindow::onProtocolError(const QString &error) {
    QString userMessage;

    if (error.contains("Connection refused")) {
        userMessage = "Verbindung abgelehnt.\n"
                     "Ist der Director erreichbar?\n"
                     "Port: 9101 (Standard)";
    } else if (error.contains("Connection timed out")) {
        userMessage = "Verbindungs-Timeout.\n"
                     "Prüfen Sie die Firewall-Einstellungen.";
    } else if (error.contains("SSL") || error.contains("TLS")) {
        userMessage = "TLS-Fehler.\n"
                     "Prüfen Sie die TLS-Konfiguration.";
    } else {
        userMessage = QString("Netzwerk-Fehler:\n%1").arg(error);
    }

    QMessageBox::critical(this, "Verbindungsfehler", userMessage);
}
```

**3. Logging:**
```cpp
void setupLogging() {
    // Alle wichtigen Signals loggen
    connect(m_director, &BDirector::authentificationSucceeded, this,
        [](bool success, const QString &msg) {
            if (success) {
                qInfo() << "Auth succeeded:" << msg;
            } else {
                qWarning() << "Auth failed:" << msg;
            }
        });

    connect(m_director, &BDirector::protocolError, this,
        [](const QString &error) {
            qCritical() << "Protocol error:" << error;
        });

    connect(m_director, &BDirector::commandError, this,
        [](const QString &cmd, const QString &error) {
            qWarning() << "Command" << cmd << "failed:" << error;
        });
}
```

**4. Graceful Degradation:**
```cpp
void MainWindow::onConnectionLost() {
    // UI in "Disconnected"-Modus
    updateUI(false);

    // Daten-Cache behalten (für Offline-Ansicht)
    // ...

    // Optional: Auto-Reconnect
    if (m_autoReconnect) {
        QTimer::singleShot(5000, this, &MainWindow::reconnect);
    }

    // User benachrichtigen
    m_statusBar->showMessage("Verbindung verloren", 5000);
}
```

---

## Bareos vs. Bacula

### Hauptunterschiede

| Feature | Bareos | Bacula |
|---------|--------|--------|
| **Fork-Datum** | 2010 | Original (1999) |
| **TLS-PSK** | Standard seit 18.2 | Optional, weniger verbreitet |
| **Authentifizierung** | PSK → CRAM-MD5 | CRAM-MD5 → (optional) TLS |
| **JSON-API** | Native Unterstützung, stabil | Experimentell |
| **WebUI** | Bareos WebUI | Baculum, BWeb |
| **Zusatz-Features** | Configure, Export, Import | Nicht vorhanden |
| **Community** | Bareos GmbH, aktive Community | Original-Team, kleinere Community |
| **Lizenz** | AGPL-3.0 | AGPL-3.0 |

### Code-Unterschiede

**Resource-Namen:**
```cpp
// Bareos
QString identity = "R_CONSOLE::*UserAgent*";

// Bacula
QString identity = "Console::*UserAgent*";
```

**TLS-Reihenfolge:**
```
Bareos 18.2+:
TCP → TLS-PSK → Hello → CRAM-MD5 → Ready

Bacula / Bareos < 18.2:
TCP → Hello → CRAM-MD5 → TLS (optional) → Ready
```

**Kommandos:**
```cpp
// Nur in Bareos
director->doSendCommand(BDirector::Command::Configure);
director->doSendCommand(BDirector::Command::Export);
director->doSendCommand(BDirector::Command::Import);
director->doSendCommand(BDirector::Command::StatusSubscriptions);

// In beiden
director->doSendCommand(BDirector::Command::ListJobs);
director->doSendCommand(BDirector::Command::StatusDirector);
```

### Migration Bareos → Bacula

```cpp
// 1. CMake-Flag ändern
add_definitions(-DUSE_BACULA_ONLY)  // Statt USE_BAREOS_ONLY

// 2. TLS-Konfiguration anpassen
BDirector::TLSConfig config;
config.tlsEnable = true;
config.tlsPSKEnable = false;  // Bacula bevorzugt Zertifikate
config.tlsVerifyPeer = true;
// Zertifikate laden...

// 3. Bareos-spezifische Kommandos entfernen
// Nicht verwenden: Configure, Export, Import, StatusSubscriptions

// 4. Neu kompilieren
```

### Empfehlung

**Verwenden Sie Bareos wenn:**
- Neues Projekt
- Moderne Features benötigt (JSON-API, WebUI)
- Einfaches Setup gewünscht (PSK)
- Community-Support wichtig

**Verwenden Sie Bacula wenn:**
- Legacy-System
- Zertifikat-Infrastruktur vorhanden
- Original-Bacula-Features benötigt
- Stabilität über Features

---

## Anhang

### Debugging-Tipps

**1. Qt-Debug-Output aktivieren:**
```cpp
// In main.cpp
qSetMessagePattern("[%{time yyyy-MM-dd HH:mm:ss}] "
                   "[%{type}] %{function}: %{message}");
```

**2. Developer-Modus:**
```cmake
# CMakeLists.txt
add_definitions(-DIS_DEVELOPER)  # Aktiviert zusätzliche Debug-Ausgaben
```

**3. Netzwerk-Traffic mitschneiden:**
```bash
# Wireshark/tcpdump
sudo tcpdump -i any -s 0 -w bareos.pcap port 9101

# Analyse mit Wireshark
wireshark bareos.pcap
```

**4. Director-Logs:**
```bash
# Bareos Director Log
tail -f /var/log/bareos/bareos.log

# Bareos mit erhöhtem Debug-Level
# In bareos-dir.conf:
Director {
  ...
  Debug Level = 200
}
```

### Häufige Probleme

**Problem: "Connection refused"**
- **Ursache:** Director läuft nicht oder Port falsch
- **Lösung:**
  ```bash
  # Bareos-Director-Status prüfen
  systemctl status bareos-dir

  # Port prüfen
  netstat -tlnp | grep 9101
  ```

**Problem: "Authentication failed"**
- **Ursache:** Falsches Passwort oder Console-Name
- **Lösung:**
  - Prüfe Director-Config: `/etc/bareos/bareos-dir.d/console/*.conf`
  - Prüfe Console-Name und Passwort

**Problem: "TLS handshake failed"**
- **Ursache:** TLS-Konfiguration stimmt nicht überein
- **Lösung:**
  - Client und Director müssen gleichen TLS-Modus verwenden
  - Bei PSK: OpenSSL-PSK-Support prüfen
  - Bei Cert: Zertifikate validieren

**Problem: "No PSK ciphers available"**
- **Ursache:** OpenSSL ohne PSK-Support kompiliert
- **Lösung:**
  ```bash
  # OpenSSL-PSK-Support prüfen
  openssl ciphers -v | grep PSK

  # Wenn leer: OpenSSL neu kompilieren mit --enable-psk
  ```

### Performance-Tipps

**1. JSON-Modus verwenden:**
```cpp
// Effizienter als Text-Parsing
director->setApiMode(BDirector::ApiMode::Json);
```

**2. Kommandos batch-en:**
```cpp
// Ineffizient:
director->doSendCommand(BDirector::Command::ListJobs);
// Warte auf Antwort...
director->doSendCommand(BDirector::Command::ListClients);
// Warte auf Antwort...

// Besser: Lade alles in einem JSON-Call
// (benötigt spezielle Director-Unterstützung)
```

**3. Connection-Pooling:**
```cpp
// Für viele Kommandos: Verbindung offen lassen
// Statt: connect → command → disconnect
// Besser: connect → command1 → command2 → ... → disconnect
```

### Weiterführende Links

- [Bareos Documentation](https://docs.bareos.org/)
- [Bareos GitHub](https://github.com/bareos/bareos)
- [Bacula Documentation](https://www.bacula.org/documentation/documentation/)
- [Qt Documentation](https://doc.qt.io/)
- [OpenSSL PSK](https://www.openssl.org/docs/man1.1.1/man3/SSL_CTX_use_psk_identity_hint.html)

---

**Letzte Aktualisierung:** 2025-01-27
**Version:** 1.0.0
**Autor:** Jörg Bernau
