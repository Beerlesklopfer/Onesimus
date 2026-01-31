# Onesimus - Modern Backup Management UI

<p align="center">
  <strong>A modern, cross-platform management interface for Bacula and Bareos backup systems</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Qt-6.8%2B-green?logo=qt" alt="Qt 6.8+">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue?logo=c%2B%2B" alt="C++ 17">
  <img src="https://img.shields.io/badge/OpenSSL-3.6-orange?logo=openssl" alt="OpenSSL 3.6">
  <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey" alt="Cross-Platform">
  <img src="https://img.shields.io/badge/i18n-6%20Languages-blue" alt="6 Languages">
  <img src="https://img.shields.io/badge/Status-Alpha-red" alt="Alpha">
</p>

---

> **⚠️ ENTWICKLUNGSHINWEIS / DEVELOPMENT NOTICE**
>
> **Deutsch:** Dieses Projekt befindet sich in aktiver Entwicklung und dient derzeit nur zu Testzwecken. Es ist **nicht für den produktiven Einsatz** geeignet. Funktionen können sich jederzeit ändern oder unvollständig sein. Nutzung auf eigenes Risiko. Siehe [Haftungsausschluss](https://github.com/Beerlesklopfer/Onesimus/wiki/Disclaimer).
>
> **English:** This project is under active development and is currently for **testing purposes only**. It is **not suitable for production use**. Features may change or be incomplete at any time. Use at your own risk. See [Disclaimer](https://github.com/Beerlesklopfer/Onesimus/wiki/Disclaimer).
>
> **Aktuelle Entwicklung / Current Development:** [`development` branch](https://github.com/Beerlesklopfer/Onesimus/tree/development)

---

## 🆕 What's New (v0.1.0.4 - 2026-01-31)

### New Features
- **Job Preselection** - Run Job dialog auto-selects FileSet, Pool, Storage, Client, Level from job defaults
- **Complete Status Filters** - Added Running (R) and Canceled (A) status checkboxes to job filters
- **Run New Job Dialog** - Create and run backup jobs with a modern dialog interface
  - Job configuration with Job, Client, Level selection
  - Resource selection for FileSet, Pool, Storage
  - Advanced scheduling and bootstrap options
  - Command preview with validation
- **BVFS File Browser** - Browse backed up files directly in job details dialog
  - Toggle between "Current Job" and "All Related Jobs" (full restore chain)
  - Windows Explorer-style tree view with file list
  - File details: Name, Size, Type, Modification Time
  - Checkbox selection for files and directories with Restore button
- **Job Delete Options** - Choose between delete (record only) or purge (with volume data)
  - Dependent job detection for Full backups
  - Safety confirmations before destructive operations
- **Connection Wizard** - Step-by-step wizard for Director connection setup with auto-detection
- **TLS Certificate Authentication** - Full X.509 certificate support alongside TLS-PSK
- **Connection Profiles** - Save and manage multiple Director connections
- **About Qt** - Added "About Qt" option in Help menu

### Bug Fixes
- Fixed: Run command duplication ("run job=run job=..." issue)
- Fixed: Delete command duplication
- Fixed: Purge command not working
- Fixed: Job log flipping between jobs when rapidly selecting different jobs
- Fixed: Job log not displaying in BJobWidget bottom panel
- Fixed: Folder selection in file browser not counting subfolders
- Fixed: Restore button width too narrow
- Fixed: Run Job button not being enabled after data loads

### Improvements
- Filter persistence for Running and Canceled status checkboxes
- Refactored codebase (`mainwindow` → `bmainwindow`, `settingsdialog` → `bsettingsdialog`)
- Improved wizard data persistence across page navigation
- Enhanced status bar with configurable colors
- Better debug logging with component prefixes
- Complete German translation
- English translation for Run New Job dialog

### Test Infrastructure
- Comprehensive test suite with authentication and state-machine tests
- BVFS Explorer test command for learning the BVFS API
- Test data generator for realistic Bareos database entries
- Support for Legacy, TLS-PSK, and TLS-Certificate authentication modes

## ✨ Features

### 🎨 User Interface
- **Modern Qt6 Design:** Dark Industrial theme with intuitive navigation
- **Multi-Tab Interface:** Jobs, Clients, Storage, Schedules in separate tabs
- **Flexible Layouts:** Resizable widgets and customizable views
- **Responsive Design:** Optimized for various screen sizes

### 🌍 Internationalization (i18n)
- **6 Languages:** 🇬🇧 English *(complete)* • 🇩🇪 Deutsch *(~30% translated)* • ~~🇪🇸 Español~~ *(0%)* • ~~🇫🇷 Français~~ *(0%)* • ~~🇮🇹 Italiano~~ *(0%)* • ~~🇷🇺 Русский~~ *(0%)*
- **Automatic Language Detection:** Automatically selects system language on first start
- **Live Language Switching:** Change language via Settings (restart required for full effect)
- **Native Language Names:** All languages in their native spelling with flags
- *Note: Only MainWindow is fully translated. ~325 strings in dialogs/widgets still need translation.*

### 📊 Job Management
- **Job Overview:** All backup jobs in a clear table
- **Advanced Filters:** Filter by status, level, date, client, name
- **Statistics:** Real-time statistics for successful/failed jobs
- **Job Details:** Detailed view with logs, files, bytes, duration
- **Multi-Selection:** Select jobs via checkbox and export
- **Pagination:** Efficient display even with thousands of jobs
- **Export Functions:** Export jobs as JSON or CSV
- ~~**Job Control:** Start, stop, cancel jobs~~ *(UI present, backend missing)*

### 💾 Client Management
- **Client Overview:** All configured backup clients
- **Status Display:** Online/Offline status with visual indicators
- ~~**Client Details Dialog:** Full client information~~ *(not yet implemented)*
- **Filtering:** Filter clients by status and name

### 🗄️ Storage Management
- **Storage Overview:** All storage daemons and devices
- **Volume Management:** Pools, volumes, media status
- ~~**Volume Operations:** Label, mount, unmount volumes~~ *(not yet implemented)*
- **Capacities:** Free/Used storage space per pool
- **Device Status:** Status and availability of storage devices

### 📅 Schedule Management
- **Schedule Overview:** All configured backup schedules
- **Schedule Details:** Run times, level, pool assignment
- **Visual Representation:** Clear display of backup windows
- ~~**Schedule Control:** Enable/disable schedules~~ *(not yet implemented)*

### 🔐 Security & Connection
- **Connection Wizard:** Step-by-step setup with auto-detection of server capabilities
- **Connection Profiles:** Save and manage multiple Director connections
- **TLS/SSL Encryption:** Secure connections with OpenSSL 3.6
- **Triple Authentication:** Legacy (CRAM-MD5), TLS-PSK, or X.509 Certificates
- **Certificate Management:** CA, client certificate and key files
- **Director Config Export:** Export console configurations for Bareos server
- **Windows PFX Support:** Native .pfx file support on Windows
- **Connection Timeout:** Configurable timeout settings
- **Auto-Connect:** Automatic connection on startup (optional)

### ⚙️ Settings
- **Categorized Settings:** Connection, Appearance, Behavior, Advanced
- **Theme Selection:** Dark (Industrial), Light, System
- **Font Size:** Adjustable base font size (8-16pt)
- **UI Options:** Animations, compact mode
- **Language Selection:** 6 languages with flag icons
- **Level Colors:** Customizable colors for Full, Incremental, Differential, VirtualFull
- **Auto-Refresh:** Automatic refresh of job list
- **Debug Logging:** Extensive logging options for troubleshooting

### 🔌 Backup System Support
- ~~**Bacula:** Full support via bconsole (TCP/TLS)~~ *(not yet implemented)*
- **Bareos:** Full support via bconsole (TCP/TLS) with JSON-RPC
- **Flexible Configuration:** System selection at build-time or runtime
- ~~**Restore Functionality:** File browsing and restore wizard~~ *(not yet implemented)*

### 🛠️ Technical Features
- **MVC Architecture:** Clean separation of data and presentation
- **Qt Model/View:** BListModel, BTableModel, BTreeModel for flexible data models
- **JSON Streaming:** Efficient parsing of large JSON responses
- **Asynchronous Operations:** Non-blocking network communication
- **Error Handling:** Robust error handling with user feedback
- **Settings Persistence:** QSettings-based configuration storage

### 📦 Deployment
- **Static OpenSSL:** Automatic download and build via Git submodule
- **Cross-Platform:** Native builds for Windows, Linux, macOS
- **AppImage Support:** Linux deployment with linuxdeploy
- **Installer:** NSIS-based Windows installer (planned)

## 🎯 Supported Backup Systems

| System | bconsole | JSON-RPC | Status |
|--------|----------|----------|--------|
| **Bacula** | ❌ Not yet | ❌ Not yet | Not implemented |
| **Bareos** | ✅ TCP/TLS | ✅ Native | Full |

### Choose Build Option

```bash
# Bareos (default and currently only supported)
cmake .. -DBACKUP_SYSTEM=BAREOS

# Bacula only (not yet implemented)
cmake .. -DBACKUP_SYSTEM=BACULA

# Both systems (not yet fully implemented)
cmake .. -DBACKUP_SYSTEM=BOTH
```

📖 **Details:** [BACKUP_SYSTEMS.md](BACKUP_SYSTEMS.md)

## 🚀 Quick Start

### Windows

```powershell
# Open Developer PowerShell for VS 2022
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

## 📦 Prerequisites

### Windows
- Visual Studio 2022 (C++ Desktop Development)
- Qt 6.8+
- CMake 3.16+
- Perl (Strawberry Perl)
- Git

### Linux
```bash
sudo apt-get install build-essential cmake git qt6-base-dev qt6-tools-dev qt6-tools-dev-tools perl
```

### macOS
```bash
brew install cmake git qt@6 perl
```

## 🔧 Build Options

### Backup System

| Option | Description |
|--------|-------------|
| `BAREOS` | Bareos support only **(default, currently only supported)** |
| `BACULA` | Bacula support only *(not yet implemented)* |
| `BOTH` | Bacula AND Bareos *(not yet fully implemented)* |

### OpenSSL

| Option | Description |
|--------|-------------|
| `USE_STATIC_OPENSSL=ON` | Static (Git submodule, default) |
| `USE_STATIC_OPENSSL=OFF` | System OpenSSL |

### Build Type

| Option | Description |
|--------|-------------|
| `Release` | Optimized (default) |
| `Debug` | With debug symbols |
| `RelWithDebInfo` | Optimized + debug info |

### Debugging Options

| Option | Description |
|--------|-------------|
| `DEBUG_PACKETS=ON` | Packet-level debugging |
| `DEBUG_JSON=ON` | JSON parsing debugging |
| `LOG_JSON=ON` | Log JSON responses |

### Example

```bash
cmake .. \
    -DBACKUP_SYSTEM=BAREOS \
    -DUSE_STATIC_OPENSSL=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DDEBUG_JSON=OFF
```

## 📚 Documentation

- [BUILD_WINDOWS.md](BUILD_WINDOWS.md) - Windows build instructions
- [BUILD_LINUX.md](BUILD_LINUX.md) - Linux build instructions
- [BUILD_OSX.md](BUILD_OSX.md) - macOS build instructions
- [BACKUP_SYSTEMS.md](BACKUP_SYSTEMS.md) - Bacula vs. Bareos
- [STATIC_OPENSSL.md](STATIC_OPENSSL.md) - OpenSSL integration
- [VISUAL_STUDIO_ENV.md](VISUAL_STUDIO_ENV.md) - VS environment setup

## 🎨 Screenshots

### Main Window
*Modern Qt6 interface with tab navigation for Jobs, Clients, Storage, Schedules*

### Job Management
- Clear table with all jobs
- Filter by status (Successful, Warning, Failed), level (F/I/D/V), date
- Real-time statistics
- Multi-selection and export functions

### Settings Dialog
- Connection settings with TLS/SSL configuration
- Appearance: Theme, font size, language (6 languages with flags)
- Behavior: Auto-refresh, job display options
- Advanced: Debug logging, tooltips

### Client Management
*All backup clients with status display*

### Storage Management
*Storage daemons, pools and volumes*

## 🔐 TLS/SSL Support

Onesimus supports encrypted connections with:
- **PSK (Pre-Shared Key):** Simple authentication with shared key
- **Certificate-based:** CA certificate validation + client certificate
- **TLS 1.2+ Protocols:** Modern encryption standards
- **EC and RSA Keys:** Support for various key types
- **Windows PFX:** Native .pfx file support (combined cert+key)

Create certificates with:
```bash
# Windows
.\create-bacula-certs.ps1

# Linux/macOS
./create-bacula-certs.sh
```

## 🛠️ Development

### Project Structure

```
onesimus/
├── include/                    # Header files
│   ├── bmainwindow.h           # Main window
│   ├── bsettingsdialog.h       # Settings dialog
│   ├── bconnectionwizard.h     # Connection wizard
│   ├── bcleanupdialog.h        # Job cleanup dialog
│   ├── bprofilesettingsdialog.h# Profile settings
│   ├── bconfigexporter.h       # Config export
│   ├── bcertificategenerator.h # Certificate generator
│   ├── bdirector.h             # Director base class
│   ├── bareosdirector.h        # Bareos-specific
│   ├── bareosauth.h            # Bareos authentication
│   ├── bbasemodels.h           # Base models (List, Table, Tree)
│   ├── bresourcemodels.h       # Resource models (Fileset, Storage, Pool, Level)
│   ├── btranslations.h         # i18n system
│   ├── bsettings.h             # Settings management
│   ├── clientwidget.h          # Client widget
│   ├── storagewidget.h         # Storage widget
│   └── jobs/                   # Job-specific headers
│       ├── bjobmodels.h        # Job models
│       └── bjobwidget.h        # Job widget
├── src/                        # Implementations
│   ├── main.cpp                # Entry point with i18n init
│   ├── bmainwindow.cpp         # Main window logic
│   ├── bsettingsdialog.cpp     # Settings dialog logic
│   ├── bconnectionwizard.cpp   # Connection wizard logic
│   ├── bcleanupdialog.cpp      # Job cleanup logic
│   ├── bprofilesettingsdialog.cpp # Profile settings
│   ├── bconfigexporter.cpp     # Config export logic
│   ├── bcertificategenerator.cpp # Certificate generation
│   ├── bdirector.cpp           # Director communication
│   ├── bareosdirector.cpp      # Bareos JSON-RPC
│   ├── bareosauth.cpp          # Bareos CRAM-MD5/TLS auth
│   ├── bbasemodels.cpp         # Base model implementations
│   ├── bresourcemodels.cpp     # Resource model implementations
│   ├── btranslations.cpp       # i18n implementation
│   ├── bjsonstreamreader.cpp   # JSON stream parser
│   └── jobs/                   # Job implementations
├── test/                       # Test suite
│   ├── bareosauth_test.cpp     # Auth class tests
│   ├── director_test.cpp       # State-machine tests
│   ├── bareos_auth.sh          # Auth test script
│   ├── director_test.sh        # Director test script
│   ├── run_all_tests.sh        # Full test runner
│   ├── generate_bareos_testdata.py # Test data generator
│   ├── configs/                # Console configurations
│   └── bareos-dir.d/           # Test director config
├── translations/               # Qt Linguist .ts files
│   ├── onesimus_de.ts          # German (complete)
│   ├── onesimus_en.ts          # English
│   ├── onesimus_es.ts          # Spanish
│   ├── onesimus_fr.ts          # French
│   ├── onesimus_it.ts          # Italian
│   └── onesimus_ru.ts          # Russian
├── ui/                         # Qt UI files
│   ├── bmainwindow.ui          # Main window UI
│   ├── bsettingsdialog.ui      # Settings dialog UI
│   ├── jobwidget.ui            # Job widget UI
│   ├── clientwidget.ui         # Client widget UI
│   └── storagewidget.ui        # Storage widget UI
├── external/                   # Git submodules
│   └── openssl/                # OpenSSL 3.6
└── CMakeLists.txt              # Build configuration
```

### Architecture

#### MVC Pattern
- **Models:** BListModel, BTableModel, BTreeModel as base classes
- **Views:** Qt ListView, TableView with custom delegates
- **Controllers:** Widget classes (BJobWidget, ClientWidget, etc.)

#### Communication Layer
- **BDirector:** Abstract base class for Director communication
- **BareosDirector:** Bareos-specific implementation with JSON-RPC
- **BJsonStreamReader:** Streaming JSON parser for large responses

#### Internationalization
- **BTranslations:** Singleton for language management
- **Qt Linguist:** .ts/.qm files for translations
- **Automatic Language Detection:** System locale detection on first start

### Code Style

- C++17 Standard
- Qt6 Coding Conventions
- MOC-based Meta-Object System
- Signal/Slot mechanism for event handling
- Smart pointers where possible
- RAII principle

### Contributing Translations

```bash
# Update .ts files (extract new strings)
cd build
cmake ..
make  # Runs lupdate automatically

# Translate with Qt Linguist
linguist translations/onesimus_de.ts
```

### Contributing

Pull requests are welcome! Please:
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a pull request

**Coding Guidelines:**
- Follow Qt Coding Conventions
- Use `tr()` for all UI strings (i18n)
- Write meaningful commit messages
- Test on at least one platform

## 📄 License

This project is licensed under the GPL-3.0 License - see [LICENSE](LICENSE) for details.

## 🙏 Credits

### Technologies

- [Qt Framework](https://www.qt.io/) - Cross-platform UI
- [OpenSSL](https://www.openssl.org/) - TLS/SSL Encryption
- [CMake](https://cmake.org/) - Build System

### Backup Systems

- [Bacula](https://www.bacula.org/) - Open Source Backup
- [Bareos](https://www.bareos.com/) - Bacula Fork with Enterprise Features

## 📞 Support

- 📖 Documentation: See [docs/](docs/)
- 🐛 Bugs: [GitHub Issues](https://github.com/your-repo/issues)
- 💬 Discussions: [GitHub Discussions](https://github.com/your-repo/discussions)

## 🔄 Version History

### v1.1.0 (January 2026)
- ✅ BVFS File Browser in Job Details Dialog
  - Toggle "Current Job" / "All Related Jobs" mode
  - Windows Explorer-style directory tree and file list
  - File details with size, type, and modification time
- ✅ Connection Wizard with step-by-step Director setup
- ✅ TLS Certificate Authentication (X.509) alongside TLS-PSK
- ✅ Connection Profiles for multiple Directors
- ✅ Director Configuration Export for server setup
- ✅ Old Job Cleanup Dialog with filters
- ✅ Refactored codebase (bmainwindow, bsettingsdialog naming convention)
- ✅ Comprehensive test suite (bareosauth_test, director_test, BVFS explorer)
- ✅ Test data generator for Bareos database
- ✅ Complete German translation
- ✅ Enhanced status bar with configurable colors
- ✅ Improved wizard data persistence
- ✅ Better debug logging with component prefixes

### v1.0.0 (January 2026)
- ✅ Renamed to "Onesimus"
- ✅ Internationalization (i18n): 6 languages with automatic language detection
- ✅ Bareos support with JSON-RPC added
- ✅ CMake backup system selection (Bacula/Bareos/Both)
- ✅ Job Management: Overview, filters, statistics, export
- ✅ Client Management: Status overview, details
- ✅ Storage Management: Pools, volumes, devices
- ✅ Schedule Management: Schedule overview
- ✅ Settings dialog with Industrial Dark design
- ✅ MVC architecture with reusable models
- ✅ Static OpenSSL 3.6 via Git submodule
- ✅ Cross-platform build scripts (Windows/Linux/macOS)
- ✅ Comprehensive documentation

### Planned Features (v1.2+)
- 🔜 Live job monitoring with progress bars
- 🔜 Job start/stop/cancel functions
- 🔜 Volume management (label, mount, unmount)
- 🔜 Enhanced statistics and charts
- 🔜 Backup job templates
- 🔜 Email notifications
- 🔜 Dashboard with overview
- 🔜 Restore wizard (BVFS file browser ✅ implemented)

---

<p align="center">
  Made with ❤️ using Qt6 and C++17
</p>
