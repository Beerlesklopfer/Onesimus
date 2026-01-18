# Onesimus - Modern Backup Management UI

<p align="center">
  <strong>Eine moderne, plattformübergreifende Verwaltungsoberfläche für Bacula und Bareos Backup-Systeme</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Qt-6.8%2B-green?logo=qt" alt="Qt 6.8+">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue?logo=c%2B%2B" alt="C++ 17">
  <img src="https://img.shields.io/badge/OpenSSL-3.6-orange?logo=openssl" alt="OpenSSL 3.6">
  <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey" alt="Cross-Platform">
</p>

## ✨ Features

- 🔌 **Multi-System Support:** Bacula und/oder Bareos
- 🔒 **TLS/SSL Encryption:** Sichere verschlüsselte Verbindungen mit OpenSSL
- 🌐 **Dual Connectivity:** bconsole (TCP/TLS) und REST API
- 🎨 **Modern UI:** Dunkles Industrial Design mit Settings-Dialog
- ⚙️ **Flexible Configuration:** CMake Build-Optionen für Backup-System-Auswahl
- 📦 **Static OpenSSL:** Automatischer Download und Build via Git-Submodule
- 🖥️ **Cross-Platform:** Windows, Linux, macOS

## 🎯 Unterstützte Backup-Systeme

| System | bconsole | REST API | Status |
|--------|----------|----------|--------|
| **Bacula** | ✅ TCP/TLS | ⚠️ Experimentell | Vollständig |
| **Bareos** | ✅ TCP/TLS | ✅ Native JSON | Vollständig |

### Build-Option wählen

```bash
# Nur Bacula (Standard)
cmake .. -DBACKUP_SYSTEM=BACULA

# Nur Bareos  
cmake .. -DBACKUP_SYSTEM=BAREOS

# Beide Systeme
cmake .. -DBACKUP_SYSTEM=BOTH
```

📖 **Details:** [BACKUP_SYSTEMS.md](BACKUP_SYSTEMS.md)

## 🚀 Schnellstart

### Windows

```powershell
# Developer PowerShell for VS 2022 öffnen
.\build-windows.ps1
```

### Linux

```bash
./build.sh
```

### macOS

```bash
brew install qt@6 cmake
./build.sh
```

## 📦 Voraussetzungen

### Windows
- Visual Studio 2022 (C++ Desktop Development)
- Qt 6.8+
- CMake 3.16+
- Perl (Strawberry Perl)
- Git

### Linux
```bash
sudo apt-get install build-essential cmake git qt6-base-dev qt6-tools-dev perl
```

### macOS
```bash
brew install cmake git qt@6 perl
```

## 🔧 Build-Optionen

### Backup System

| Option | Beschreibung |
|--------|-------------|
| `BACULA` | Nur Bacula-Unterstützung (Standard) |
| `BAREOS` | Nur Bareos-Unterstützung |
| `BOTH` | Bacula UND Bareos |

### OpenSSL

| Option | Beschreibung |
|--------|-------------|
| `USE_STATIC_OPENSSL=ON` | Statisch (Git-Submodule, Standard) |
| `USE_STATIC_OPENSSL=OFF` | System-OpenSSL |

### Build-Typ

| Option | Beschreibung |
|--------|-------------|
| `Release` | Optimiert (Standard) |
| `Debug` | Mit Debug-Symbolen |
| `RelWithDebInfo` | Optimiert + Debug |

### Beispiel

```bash
cmake .. \
    -DBACKUP_SYSTEM=BOTH \
    -DUSE_STATIC_OPENSSL=ON \
    -DCMAKE_BUILD_TYPE=Release
```

## 📚 Dokumentation

- [BUILD_WINDOWS.md](BUILD_WINDOWS.md) - Windows Build-Anleitung
- [BUILD_LINUX.md](BUILD_LINUX.md) - Linux Build-Anleitung
- [BUILD_OSX.md](BUILD_OSX.md) - macOS Build-Anleitung
- [BACKUP_SYSTEMS.md](BACKUP_SYSTEMS.md) - Bacula vs. Bareos
- [STATIC_OPENSSL.md](STATIC_OPENSSL.md) - OpenSSL-Integration
- [VISUAL_STUDIO_ENV.md](VISUAL_STUDIO_ENV.md) - VS-Umgebung laden

## 🎨 Screenshots

### Main Window
*Moderne Qt6-Oberfläche mit Tab-Navigation*

### Settings Dialog
*Industrial Dark Design mit Kategorien-Sidebar*

### Job Management
*Jobs anzeigen, starten, überwachen*

## 🔐 TLS/SSL Support

Onesimus unterstützt verschlüsselte Verbindungen mit:
- CA-Zertifikat Validierung
- Client-Zertifikat Authentifizierung
- TLS 1.2+ Protokolle
- EC und RSA Keys

Erstellen Sie Zertifikate mit:
```bash
# Windows
.\create-bacula-certs.ps1

# Linux/macOS
./create-bacula-certs.sh
```

## 🛠️ Entwicklung

### Projektstruktur

```
onesimus/
├── include/           # Header-Dateien
│   ├── baculadirector.h
│   ├── bareosdirector.h
│   ├── mainwindow.h
│   └── settingsdialog.h
├── src/              # Implementierungen
├── ui/               # Qt UI-Dateien
├── external/         # Git-Submodules (OpenSSL)
└── CMakeLists.txt    # Build-Konfiguration
```

### Code-Stil

- C++17 Standard
- Qt6 Coding Conventions
- MOC-basiertes Meta-Object-System

### Beitragen

Pull Requests sind willkommen! Bitte:
1. Fork das Repository
2. Erstellen Sie einen Feature-Branch
3. Commit Ihre Änderungen
4. Push zum Branch
5. Öffnen Sie einen Pull Request

## 📄 Lizenz

Dieses Projekt steht unter der GPL-3.0 Lizenz - siehe [LICENSE](LICENSE) für Details.

## 🙏 Credits

### Technologien

- [Qt Framework](https://www.qt.io/) - Cross-platform UI
- [OpenSSL](https://www.openssl.org/) - TLS/SSL Encryption
- [CMake](https://cmake.org/) - Build System

### Backup-Systeme

- [Bacula](https://www.bacula.org/) - Open Source Backup
- [Bareos](https://www.bareos.com/) - Bacula Fork mit Enterprise-Features

## 📞 Support

- 📖 Dokumentation: Siehe [docs/](docs/)
- 🐛 Bugs: [GitHub Issues](https://github.com/your-repo/issues)
- 💬 Diskussionen: [GitHub Discussions](https://github.com/your-repo/discussions)

## 🔄 Version History

### v1.0.0 (Januar 2026)
- ✅ Umbenennung zu "Onesimus"
- ✅ Bareos-Support hinzugefügt
- ✅ CMake Backup-System-Auswahl
- ✅ Settings-Dialog mit Industrial Dark Design
- ✅ Statisches OpenSSL via Git-Submodule
- ✅ Cross-Platform Build-Scripts (Windows/Linux/macOS)
- ✅ PowerShell Build-Script für Windows
- ✅ Umfassende Dokumentation

---

<p align="center">
  Made with ❤️ using Qt6 and C++17
</p>
