# Änderungsübersicht - Bacula Qt UI

## Geänderte Dateien

### Header-Dateien (.h)

1. **include/baculadirector.h**
   - ✅ TLSConfig Struktur hinzugefügt
   - ✅ QSslSocket Support hinzugefügt
   - ✅ TLS-Methoden (setTLSConfig, tlsConfig, setupTLSConnection, loadTLSCertificates)
   - ✅ SSL-Error Slots (onSslErrors, onEncrypted)
   - ✅ Verbindungseinstellungen-Methoden (saveConnectionSettings, loadConnectionSettings, hasStoredConnection)
   - ✅ Erweiterte connectBConsole Signatur mit TLSConfig Parameter

2. **include/mainwindow.h**
   - ✅ Neue Methode: loadAndConnectLastUsed()
   - ✅ Neue Methode: onConnectLastUsed()
   - ✅ Neue Action: m_connectLastAction

3. **include/jobwidget.h**
   - Keine wesentlichen Änderungen (nur Basisfunktionalität)

4. **include/clientwidget.h**
   - Keine wesentlichen Änderungen (nur Basisfunktionalität)

5. **include/storagewidget.h**
   - Keine wesentlichen Änderungen (nur Basisfunktionalität)

### Implementierungs-Dateien (.cpp)

1. **src/baculadirector.cpp** ⭐ HAUPTÄNDERUNGEN
   - ✅ QSslSocket Initialisierung im Konstruktor
   - ✅ TLS-Verbindungslogik in connectBConsole()
   - ✅ setupTLSConnection() - TLS-Konfiguration
   - ✅ loadTLSCertificates() - CA, Client-Cert und Key laden mit intelligenter Algorithmus-Erkennung (EC, RSA, DSA)
   - ✅ onSslErrors() - SSL-Fehlerbehandlung
   - ✅ onEncrypted() - TLS-Erfolgsbestätigung
   - ✅ Dual-Socket Unterstützung (m_socket vs m_sslSocket)
   - ✅ saveConnectionSettings() - TLS-Config speichern
   - ✅ loadConnectionSettings() - TLS-Config laden
   - ✅ hasStoredConnection() - Prüfung auf gespeicherte Einstellungen
   - ✅ Anpassungen in sendCommand(), onBConsoleReadyRead(), authenticateBConsole(), disconnect()

2. **src/mainwindow.cpp** ⭐ HAUPTÄNDERUNGEN
   - ✅ showConnectionDialog() erweitert mit:
     - TLS-Checkbox
     - TLS CA Certificate File Browser
     - TLS Certificate File Browser
     - TLS Key File Browser
     - Dynamisches Aktivieren/Deaktivieren der TLS-Felder
   - ✅ loadAndConnectLastUsed() - Lädt gespeicherte Verbindung inkl. TLS-Config
   - ✅ onConnectLastUsed() - Handler für "Letzte Verbindung"
   - ✅ createActions() - "Letzte Verbindung" Action hinzugefügt
   - ✅ createMenus() - "Letzte Verbindung" im Menü
   - ✅ createToolBar() - "Letzte Verbindung" in Toolbar
   - ✅ Konstruktor - Automatische Verbindung beim Start
   - ✅ QSettings, QCheckBox, QFileDialog Includes

3. **src/main.cpp**
   - Keine Änderungen (Standard Qt Applikation)

4. **src/jobwidget.cpp**
   - Basisfunktionalität (keine TLS-spezifischen Änderungen)

5. **src/clientwidget.cpp**
   - Basisfunktionalität (keine TLS-spezifischen Änderungen)

6. **src/storagewidget.cpp**
   - Basisfunktionalität (keine TLS-spezifischen Änderungen)

## Neue Dateien

### Dokumentation

1. **QSETTINGS.md**
   - Vollständige Dokumentation zur QSettings-Integration
   - Sicherheitshinweise zu Passwort-Speicherung
   - Beispiele für Verschlüsselung
   - Speicherorte (Linux/Windows/macOS)

2. **TLS.md** ⭐
   - Komplette TLS/SSL-Anleitung
   - OpenSSL-Zertifikatserstellung (Schritt-für-Schritt)
   - Bacula Director TLS-Konfiguration
   - Fehlerbehebung für häufige Probleme
   - Sicherheits-Best-Practices

3. **QUICKSTART.md**
   - Schnellstart-Anleitung
   - Installation und Kompilierung
   - Erste Schritte

4. **CHANGES.md** (diese Datei)
   - Übersicht aller Änderungen

### Build-System

1. **CMakeLists.txt**
   - CMAKE_AUTOUIC_SEARCH_PATHS hinzugefügt (für UI-Verzeichnis)

2. **build.sh**
   - Automatisches Build-Script

3. **.gitignore**
   - Ignoriert Build-Artefakte

## Funktionale Zusammenfassung

### QSettings-Integration (Persistente Verbindungseinstellungen)
- Automatisches Speichern nach erfolgreicher Verbindung
- Laden beim Dialog-Öffnen (vorausgefüllte Felder)
- Automatische Verbindung beim Programmstart
- "Letzte Verbindung" Funktion (Ctrl+L)

### TLS/SSL-Verschlüsselung
- Checkbox zur TLS-Aktivierung
- CA-Zertifikat (Server-Validierung)
- Client-Zertifikat (Mutual TLS)
- Private Key (RSA, EC, DSA Support)
- File-Browser für alle Zertifikate
- Intelligente Algorithmus-Erkennung
- Ausführliche Fehlerbehandlung

### Dual-Socket-Architektur
- QTcpSocket für unverschlüsselte Verbindungen
- QSslSocket für TLS-verschlüsselte Verbindungen
- Automatische Auswahl basierend auf TLS-Config
- Alle Bconsole-Befehle funktionieren mit beiden

## Windows-spezifische Probleme

### Aktuelles Problem: Schannel Private Key Import

**Fehlermeldung:**
```
qt.tlsbackend.schannel: Failed to import private key: "Unknown error occurred: -2146885630"
```

**Ursache:**
- Windows verwendet standardmäßig Schannel (natives Windows TLS)
- Schannel hat Probleme mit PEM-Format EC Keys
- Der Fehlercode -2146885630 ist ein kryptischer Windows-Fehler

**Lösungsansätze:**

1. **OpenSSL-Backend erzwingen (empfohlen):**
   - Qt für Windows mit OpenSSL kompilieren
   - Oder OpenSSL-DLLs bereitstellen (libssl-3-x64.dll, libcrypto-3-x64.dll)

2. **Key-Format konvertieren:**
   - Von EC zu RSA konvertieren
   - PKCS#12 (.pfx) Format verwenden

3. **Alternative: TLS-Verbindung vom Director handhaben:**
   - TLS-Tunnel auf Director-Seite (stunnel, nginx)
   - Client verbindet sich ohne TLS zum Tunnel

## Nächste Schritte

1. OpenSSL-Backend für Windows testen
2. Alternative Key-Formate unterstützen (PKCS#12)
3. Bessere Fehlermeldungen für Windows-spezifische Probleme
4. Optional: Passwort-Verschlüsselung in QSettings
