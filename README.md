# Onesimus - Modern Backup Management UI

<p align="center">
  <strong>A modern, cross-platform management interface for Bacula and Bareos backup systems</strong><br>
  <a href="https://onesimus.io">🌐 onesimus.io</a>
</p>

> *In the Letter to Philemon, the Apostle Paul sends back Onesimus — a runaway slave whose name means "the useful one" in Greek. Once lost, now returned with purpose: no longer useless, but indispensable.*
>
> *Backups share that story. Data slips away — through failure, accident, or time. What matters is that it comes back, intact and useful, when you need it most. Onesimus helps you manage that journey: keeping watch over your Bareos environment, so that nothing stays lost for long.*

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
> **Deutsch:** Dieses Projekt befindet sich in aktiver Entwicklung und dient derzeit nur zu Testzwecken. Es ist **nicht für den produktiven Einsatz** geeignet. Funktionen können sich jederzeit ändern oder unvollständig sein. Nutzung auf eigenes Risiko. Siehe [Haftungsausschluss](https://onesimus.io/de/legal/disclaimer/).
>
> **English:** This project is under active development and is currently for **testing purposes only**. It is **not suitable for production use**. Features may change or be incomplete at any time. Use at your own risk. See [Disclaimer](https://onesimus.io/legal/disclaimer/).
>
> **Aktuelle Entwicklung / Current Development:** [`development` branch](https://github.com/Beerlesklopfer/Onesimus/tree/development)

---

## 🆕 What's New (v0.1.0 - Build 345, 2026-02-21)

### Restore Wizard (NEW)
- **Full BVFS-based Restore Wizard** — 5-page QWizard: SelectJob → Browse → Options → Preview → Execute
  - BVFS file browser with Windows Explorer-style directory tree and file list
  - Checkbox propagation (parent ↔ children) for file/directory selection
  - Restore options: Where, Replace mode, Client selection
  - Preview page with size estimate and file count summary
  - Live restore execution with progress bar and auto-refresh
  - Step indicators showing wizard progress
- **Permission Checks** — Uses `.help all` to verify user has required Bareos ACLs before restore
- **My Permissions Dialog** — View all Bareos console permissions at a glance

### Build Improvements
- **Qt >= 6.8 now required** (enforced in CMake)
- **Dynamic `USE_STATIC_OPENSSL`** — CMake auto-detects whether to use static or system OpenSSL
- **Default API mode** set to `json compact=yes` for better performance

### Previous Highlights (v0.1.0 - 2026-02-08)
- **Schema-Driven Resource Editing** — BResourceDialog + BResourceForm for all 11 resource types
- **New Client Wizard** — 3-page wizard with schema-driven preview and ZIP export
- **Catalog Resource Support** — Full catalog model with Bareos object-format parsing
- **Unified Job/JobDefs Model** — BJobConfigModel handles both `show jobs` and `show jobdefs`
- **Debian Package** — CPack DEB generator with automatic dependency detection
- **Application Icon** — Window icon from bundled Bareos/Bacula logos

### Previous Release (v0.1.0.4 - 2026-01-31)
- Job Preselection in Run Job dialog
- Complete Status Filters (Running, Canceled)
- Run New Job Dialog with command preview
- BVFS File Browser in job details
- Job Delete Options (delete vs purge)
- Connection Wizard with auto-detection
- TLS Certificate Authentication (X.509)
- Connection Profiles for multiple Directors

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
- **Authentication:** TLS-PSK or X.509 Certificate authentication
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
- **Restore Functionality:** BVFS file browsing and restore wizard (5-page QWizard)

### 🛠️ Technical Features
- **MVC Architecture:** Clean separation of data and presentation
- **Qt Model/View:** BListModel, BTableModel, BTreeModel for flexible data models
- **Schema-Driven Forms:** BResourceForm auto-generates UI from JSON directive schemas
- **JSON Streaming:** Efficient parsing of large JSON responses
- **Asynchronous Operations:** Non-blocking network communication
- **Error Handling:** Robust error handling with user feedback
- **Settings Persistence:** QSettings-based configuration storage

### 📦 Deployment
- **Static OpenSSL:** Automatic download and build via Git submodule
- **Cross-Platform:** Native builds for Windows, Linux, macOS
- **AppImage Support:** Linux deployment with linuxdeploy
- **Debian Package:** `.deb` package generation with automatic dependency detection
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
# Bash
./build.sh

# Fish
./build.fish
```

Both scripts accept the same command-line arguments:

```bash
./build.sh [OPTIONS]
./build.fish [OPTIONS]

Options:
  -t, --type TYPE        Build type: Release (default), Debug, RelWithDebInfo
  -s, --system-openssl   Use system OpenSSL instead of static submodule
  -c, --clean            Remove build directory before building
  -j, --jobs N           Parallel build jobs (default: nproc)
  -d, --build-dir DIR    Build directory (default: build)
  -h, --help             Show this help

Examples:
  ./build.sh                     # Release, static OpenSSL
  ./build.sh -t Debug            # Debug build
  ./build.sh -s -c               # System OpenSSL, clean rebuild
  ./build.sh -j4 -t Debug        # Debug, 4 jobs
```

### macOS

```bash
brew install qt@6 cmake
./build.sh
```

## 📦 Prerequisites

### Windows
- **Visual Studio 2022** (C++ Desktop Development) - **REQUIRED** (VS 2019/2017 not supported)
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

- [Local Test Director](https://github.com/Beerlesklopfer/Onesimus/wiki/Wiki-Local-Test-Director) - Set up a local Bareos Director for development and testing
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
├── CMakeLists.txt              # Build configuration
├── CMakePresets.json            # CMake presets (Linux/Windows/macOS)
├── .build_number               # Auto-incrementing build number
├── include/                    # Header files
│   ├── bmainwindow.h           # Main window
│   ├── bcertificategenerator.h # Certificate generator
│   ├── bcheckableheaderview.h  # Checkable table header
│   ├── bcleanupdialog.h        # Job cleanup dialog
│   ├── bconnectionprofile.h    # Connection profile data
│   ├── bconnectionwizard.h     # Connection wizard
│   ├── bconsolemodel.h         # Console command model
│   ├── bdirectiveregistry.h    # Directive registry
│   ├── blogging.h              # Logging utilities
│   ├── bpaginationwidget.h     # Pagination widget
│   ├── bpasswordutil.h         # Password utilities
│   ├── bpfxconverter.h         # PFX certificate converter
│   ├── bstoragemodel.h         # Storage model
│   ├── btranslations.h         # i18n system
│   ├── storagewidget.h         # Storage widget
│   ├── version.h               # Auto-generated version header
│   ├── version.h.in            # Version header template
│   ├── clients/                # Client management
│   │   ├── bclientdetailsdialog.h  # Client details dialog
│   │   ├── bclientsmodel.h     # Clients data model
│   │   ├── bclientswidget.h    # Clients list widget
│   │   ├── bclientwidget.h     # Single client widget
│   │   └── bnewclientdialog.h  # New Client Wizard
│   ├── config/                 # Configuration & settings
│   │   ├── bcolumnconfiguration.h  # Column configuration
│   │   ├── bconfigexporter.h   # Config export logic
│   │   ├── bconfigparser.h     # Bareos/Bacula config parser
│   │   ├── bdirectiveschema.h  # Directive JSON schema loader
│   │   ├── beditablelistdelegate.h # Editable list delegate
│   │   ├── beditablelistmodel.h    # Editable list model
│   │   ├── beditablelistwidget.h   # Editable list widget
│   │   ├── bincludeoptionsform.h   # FileSet Include options form
│   │   ├── bprofilesettingsdialog.h # Profile settings dialog
│   │   ├── bresourceform.h     # Schema-driven form widget
│   │   ├── bsettings.h         # Application settings
│   │   ├── bsettingsdialog.h   # Settings dialog
│   │   ├── bsettingshistory.h  # Settings change history
│   │   └── bundocommands.h     # Undo/redo commands
│   ├── db/                     # Database layer
│   │   ├── bdatabase.h         # SQLite database management
│   │   ├── bdirectormodel.h    # Director config model (DB)
│   │   └── bresourcemodel.h    # Resource model (DB)
│   ├── director/               # Director communication
│   │   ├── baculaauth.h        # Bacula authentication
│   │   ├── bareosauth.h        # Bareos CRAM-MD5/TLS auth
│   │   ├── bareosdirector.h    # Bareos JSON-RPC protocol
│   │   ├── bconfigimportdialog.h # Config import dialog
│   │   ├── bdirector.h         # Director thread-safe wrapper
│   │   ├── bjsonstreamreader.h # JSON streaming parser
│   │   ├── bresourcedialog.h   # Schema-driven resource edit dialog
│   │   ├── bresourcewidget.h   # Resource display widget
│   │   └── bresourcewidgets.h  # Specialized resource widgets
│   ├── jobs/                   # Job management
│   │   ├── bbvfsmodel.h        # BVFS file browser model
│   │   ├── bcheckboxdelegate.h # Checkbox delegate
│   │   ├── bfilesetwizard.h    # FileSet creation wizard
│   │   ├── bjobdetailsdialog.h # Job details dialog
│   │   ├── bjobfileswidget.h   # Job files browser
│   │   ├── bjoblogdialog.h     # Job log dialog
│   │   ├── bjobmodels.h        # Job data models
│   │   ├── bjobsstatisticswidget.h # Job statistics widget
│   │   ├── bjobwidget.h        # Main job management widget
│   │   ├── bjobwizard.h        # Add Job/JobDefs Wizard (WIP)
│   │   ├── bjsonjobview.h      # JSON job view
│   │   ├── blevelcolors.h      # Backup level color scheme
│   │   ├── bnewjobdialog.h     # Run New Job dialog
│   │   ├── brestorewizard.h    # Restore wizard
│   │   ├── bviewpresets.h      # View presets
│   │   └── fileset/            # FileSet sub-components
│   │       ├── bfilesetdocument.h    # FileSet document model
│   │       └── bincludeblockwidget.h # Include block editor
│   ├── messages/               # Messages management
│   │   └── bmessageswidget.h   # Messages widget
│   ├── models/                 # Shared data models
│   │   ├── bbasemodels.h       # Base models (BListModel, BTableModel, BTreeModel)
│   │   ├── bclientmodel.h      # Client data model
│   │   └── bresourcemodels.h   # Resource models (Fileset, Storage, Pool, Catalog, Job, etc.)
│   └── schedules/              # Schedule management
│       ├── bschedulewidget.h   # Schedule widget
│       └── bweeklyplanner.h    # Weekly planner widget
├── src/                        # Implementations
│   ├── main.cpp                # Entry point with i18n init
│   ├── bcertificategenerator.cpp
│   ├── bcheckableheaderview.cpp
│   ├── bcheckboxdelegate.cpp
│   ├── bcleanupdialog.cpp
│   ├── bconnectionwizard.cpp
│   ├── bdirectiveregistry.cpp
│   ├── bmainwindow.cpp         # Main window logic (~2000 lines)
│   ├── bpfxconverter.cpp
│   ├── btranslations.cpp
│   ├── storagewidget.cpp
│   ├── clients/                # Client management
│   │   ├── bclientdetailsdialog.cpp
│   │   ├── bclientsmodel.cpp
│   │   ├── bclientswidget.cpp
│   │   ├── bclientwidget.cpp
│   │   └── bnewclientdialog.cpp
│   ├── config/                 # Configuration & settings
│   │   ├── bcolumnconfiguration.cpp
│   │   ├── bconfigexporter.cpp
│   │   ├── bconfigparser.cpp
│   │   ├── bdirectiveschema.cpp
│   │   ├── beditablelistdelegate.cpp
│   │   ├── beditablelistmodel.cpp
│   │   ├── beditablelistwidget.cpp
│   │   ├── bincludeoptionsform.cpp
│   │   ├── bprofilesettingsdialog.cpp
│   │   ├── bresourceform.cpp   # Schema-driven form widget
│   │   ├── bsettings.cpp
│   │   ├── bsettingsdialog.cpp
│   │   ├── bsettingshistory.cpp
│   │   └── bundocommands.cpp
│   ├── db/                     # Database layer
│   │   ├── bdatabase.cpp
│   │   ├── bdirectormodel.cpp
│   │   └── bresourcemodel.cpp
│   ├── director/               # Director communication
│   │   ├── baculaauth.cpp
│   │   ├── bareosauth.cpp      # Bareos CRAM-MD5/TLS auth
│   │   ├── bareosdirector.cpp  # Bareos JSON-RPC protocol
│   │   ├── bconfigimportdialog.cpp
│   │   ├── bdirector.cpp       # Thread-safe Director wrapper
│   │   ├── bjsonstreamreader.cpp
│   │   ├── bresourcedialog.cpp
│   │   ├── bresourcewidget.cpp
│   │   └── bresourcewidgets.cpp
│   ├── jobs/                   # Job management
│   │   ├── bbvfsmodel.cpp      # BVFS file browser
│   │   ├── bfilesetwizard.cpp  # FileSet wizard
│   │   ├── bjobdetailsdialog.cpp
│   │   ├── bjobfileswidget.cpp
│   │   ├── bjoblogdialog.cpp
│   │   ├── bjobmodels.cpp
│   │   ├── bjobsstatisticswidget.cpp
│   │   ├── bjobwidget.cpp
│   │   ├── bjobwizard.cpp      # Add Job/JobDefs Wizard (WIP)
│   │   ├── bjsonjobview.cpp
│   │   ├── bnewjobdialog.cpp
│   │   ├── bpaginationwidget.cpp
│   │   ├── brestorewizard.cpp
│   │   ├── bviewpresets.cpp
│   │   └── fileset/            # FileSet sub-components
│   │       ├── bfilesetdocument.cpp
│   │       └── bincludeblockwidget.cpp
│   ├── messages/               # Messages management
│   │   └── bmessageswidget.cpp
│   ├── models/                 # Shared data models
│   │   ├── bbasemodels.cpp
│   │   └── bresourcemodels.cpp
│   └── schedules/              # Schedule management
│       ├── bschedulewidget.cpp
│       └── bweeklyplanner.cpp
├── resources/                  # Qt resources
│   ├── resources.qrc           # Resource collection file
│   ├── onesimus.rc             # Windows resource file
│   ├── directives/             # JSON directive schemas (13 files)
│   │   ├── catalog.json        #   Catalog resource schema
│   │   ├── client.json         #   Client resource schema
│   │   ├── console.json        #   Console resource schema
│   │   ├── director.json       #   Director resource schema
│   │   ├── fileset.json        #   FileSet resource schema
│   │   ├── job.json            #   Job resource schema
│   │   ├── jobdef.json         #   JobDefs resource schema
│   │   ├── messages.json       #   Messages resource schema
│   │   ├── pool.json           #   Pool resource schema
│   │   ├── schedule.json       #   Schedule resource schema
│   │   ├── storage.json        #   Storage resource schema
│   │   ├── directive_groups.json    # Group definitions
│   │   └── directive_schema.json    # Meta-schema
│   ├── templates/filesets/     # FileSet templates (22 presets)
│   ├── translations/directives/ # Directive translations (de)
│   ├── icons/                  # SVG/PNG icons (50+ icons)
│   ├── themes/                 # QSS stylesheets (dark/light)
│   └── sql/                    # Database schemas and migrations
│       ├── schema/v1_initial.sql
│       └── migrations/         # v2–v5 migration scripts
├── ui/                         # Qt Designer UI files
│   ├── bmainwindow.ui
│   ├── bclientwidget.ui
│   ├── bsettingsdialog.ui
│   ├── jobwidget.ui
│   └── storagewidget.ui
├── translations/               # Qt Linguist .ts files (6 languages)
│   ├── onesimus_de.ts
│   ├── onesimus_en.ts
│   ├── onesimus_es.ts
│   ├── onesimus_fr.ts
│   ├── onesimus_it.ts
│   └── onesimus_ru.ts
├── test/                       # Test suite
│   ├── bareos-dir.d/           # Bareos Director test config
│   │   ├── bareos-dir.d/       #   Full Director config tree
│   │   └── tls/                #   TLS test certificates
│   ├── bjobfileswidget_test/   # Job files widget unit test
│   ├── configs/                # Test console configs (PSK/cert/legacy)
│   ├── bareosauth_test.cpp     # Auth unit test
│   ├── director_test.cpp       # Director unit test
│   ├── generate_bareos_testdata.py  # Test data generator
│   └── *.sh                    # Shell test scripts
├── cmake/                      # CMake modules
│   └── IncrementBuildNumber.cmake
├── packaging/                  # Package generation
│   └── onesimus.desktop.in     # Linux desktop entry
├── scripts/                    # Build & utility scripts
│   ├── build-openssl.bat       # Windows OpenSSL build
│   ├── generate_fileset_templates.py
│   └── validate_directives.py
├── examples/                   # Example code
│   └── example_usage.cpp
├── docs/                       # Documentation
│   ├── ONESIMUS_DOCUMENTATION.md
│   └── WORK_SUMMARY.md
├── res/img/                    # Legacy logo images
│   ├── logo_bacula.png
│   └── logo_bareos.png
├── external/                   # Git submodules
│   └── openssl/                # OpenSSL 3.x
├── wiki/                       # GitHub Wiki (Git submodule)
├── build.sh                    # Linux build script (Bash)
├── build.fish                  # Linux build script (Fish)
├── build-windows.ps1           # Windows build script
├── BUILD_LINUX.md              # Linux build instructions
├── BUILD_WINDOWS.md            # Windows build instructions
├── BUILD_OSX.md                # macOS build instructions
├── CHANGELOG.md                # Changelog
├── KNOWN_BUGS.md               # Known bugs tracker
├── LICENSE                     # License file
└── todo.md                     # Development TODO
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

- 🌐 Website: [onesimus.io](https://onesimus.io)
- 📖 Documentation: See [docs/](docs/)
- 🐛 Bugs: [GitHub Issues](https://github.com/Beerlesklopfer/Onesimus/issues)
- 💬 Discussions: [GitHub Discussions](https://github.com/Beerlesklopfer/Onesimus/discussions)

## 🔄 Version History

### v0.1.0 (February 2026)
- ✅ **Restore Wizard** — Full BVFS-based 5-page QWizard (SelectJob → Browse → Options → Preview → Execute) (Build 345)
  - Permission checks via `.help all`, My Permissions dialog
  - Step indicators, progress bar, size estimate, auto-refresh
  - Checkbox propagation in file browser
- ✅ Build improvements: Qt >= 6.8 required, dynamic `USE_STATIC_OPENSSL`, default API mode `json compact=yes`
- 🚧 Add Job/JobDefs Wizard (BJobWizard) — 5-page QWizard with BRunScriptEditor (WIP, Build 274)
- ✅ Schema-driven resource editing (BResourceDialog + BResourceForm) for all 11 resource types
- ✅ Directive JSON schemas (director, console, client, job, jobdef, storage, fileset, pool, catalog, schedule, messages)
- ✅ Catalog resource model with Bareos object-format parsing
- ✅ Unified BJobConfigModel (Jobs + JobDefs in one class)
- ✅ New Client Wizard (3-page wizard with schema-driven preview)
- ✅ Application Icon (Bareos/Bacula build-time selection)
- ✅ Debian Package (.deb with automatic dependency detection)
- ✅ Client Config Export (context menu on client table)
- ✅ Messages Directive Schema with German translations
- ✅ Configure success detection fix for Bareos JSON API
- ✅ Build number auto-increment system (triplet.build versioning)

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

### Planned Features
- 🚧 Add Job/JobDefs Wizard (5-page QWizard with RunScript editor — WIP, Build 274)
- 🔜 Apply resource changes to Director (`configure add`/`configure update`)
- 🔜 Live job monitoring with progress bars
- 🔜 Job start/stop/cancel functions
- 🔜 Volume management (label, mount, unmount)
- 🔜 Enhanced statistics and charts
- 🔜 Backup job templates
- 🔜 Email notifications
- 🔜 Dashboard with overview
- 💤 Bacula support *(future)*

---

<p align="center">
  Made with ❤️ using Qt6 and C++17
</p>
