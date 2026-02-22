# Onesimus — Modern Backup Management UI

<p align="center">
  <strong>Every backup admin asks the same 5 questions. No tool answers them visually. Until now.</strong><br>
  <a href="https://onesimus.io">onesimus.io</a> · <a href="https://onesimus.io/docs/">Documentation</a> · <a href="https://github.com/Beerlesklopfer/Onesimus/issues">Report Bug</a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Qt-6.8%2B-green?logo=qt" alt="Qt 6.8+">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue?logo=c%2B%2B" alt="C++ 17">
  <img src="https://img.shields.io/badge/OpenSSL-3.6-orange?logo=openssl" alt="OpenSSL 3.6">
  <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey" alt="Cross-Platform">
  <img src="https://img.shields.io/badge/Status-Alpha-red" alt="Alpha">
  <img src="https://img.shields.io/badge/License-GPL--3.0-blue" alt="GPL-3.0">
</p>

---

## The 5 Questions

| # | Question | Answer | Tier |
|---|----------|--------|------|
| 1 | **Where does my storage go?** | Pool overview & visualization | Community (free) |
| 2 | **When will the pool be full?** | Growth trends & capacity forecasts | Pro |
| 3 | **Which jobs/clients consume the most?** | Storage consumption per job & client | Community (free) |
| 4 | **What happens if I change retention?** | Retention analysis & what-if simulation | Enterprise |
| 5 | **Which volumes can I recycle?** | Smart recycling recommendations | Pro |

Onesimus is a native Qt6 desktop application that connects directly to your Bareos Director. It replaces bconsole terminal workflows with a visual interface that gives you immediate answers — no scripting, no spreadsheets, no guesswork.

> **Alpha Notice:** This project is under active development. Not suitable for production use. Features may change. Use at your own risk. See [Disclaimer](https://onesimus.io/legal/disclaimer/). Current development: [`development` branch](https://github.com/Beerlesklopfer/Onesimus/tree/development).

---

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

### 🗄️ Pool & Storage — *"Where does my storage go?"*
- **Pool Overview:** Visual pool overview with volume status
- **Storage per Job/Client:** See exactly which jobs and clients consume storage
- **Volume Management:** Pools, volumes, media status, capacity per pool
- **Storage Daemons:** Device status and availability
- ~~**Volume Operations:** Label, mount, unmount~~ *(not yet implemented)*

### 📅 Schedule Visualization — *"When do my jobs run?"*
- **Gantt Timeline:** Horizontal timeline with job duration bars, zoom, dual FD/SD view
- **Weekly Planner:** Compact 7x24 grid showing backup windows at a glance
- **Utilization Heatmap:** Color-coded concurrent job load per 15-min slot
- **Collision Detection:** Client/storage conflict warnings with visual indicators
- **Duration Statistics:** Click a job to see min/avg/max duration, trend, last runs
- **Schedule Wizard:** 3-page QWizard with 5 templates and Run directive editor

### 🔐 Security & Connection
- **Connection Wizard:** Step-by-step setup with auto-detection of server capabilities
- **Connection Profiles:** Save and manage multiple Director connections
- **TLS/SSL Encryption:** Secure connections with OpenSSL 3.6
- **Authentication:** TLS-PSK or X.509 Certificate authentication
- **Certificate Management:** CA, client certificate and key files
- **Director Config Export:** Export console configurations for Bareos server
- **Windows PFX Support:** Native .pfx file support on Windows
- **ACL Visualization:** My Permissions dialog shows all console ACLs grouped by category
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
- **Qt Model/View:** BListModel, BTableModel base classes + BBvfsModel, BJobsModel, BClientsModel, BResourceModel and 10+ specialized models
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



## 🚀 Quick Start

### Windows

```powershell
# Open Developer PowerShell for VS 2022
.\build-windows.ps1
```

### Linux / macOS

macOS: `brew install qt@6 cmake` (see [Prerequisites](#-prerequisites))

```bash
./build.sh [OPTIONS]

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

Also available as `./build.fish` for Fish shell users (same options).

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

| Option | Default | Description |
|--------|---------|-------------|
| `IS_DEVELOPER` | Auto (`Debug` builds) | Enables developer menus, extra logging, and diagnostic UI. Automatically set when `CMAKE_BUILD_TYPE=Debug` |
| `DEBUG_PACKETS=ON` | OFF | Packet-level debugging — logs raw binary telegrams sent/received over the TCP connection |
| `DEBUG_JSON=ON` | OFF | JSON parsing debugging — logs JSON-RPC request/response message contents |
| `LOG_JSON=ON` | OFF | Writes all JSON responses to a log file for offline analysis |
| `ONESIMUS_FILE_LOGGING=ON` | OFF | Enables `BLOG_*` macros (DEBUG/INFO/WARNING/ERROR) writing to `AppDataLocation/onesimus.log` |

### Example

```bash
cmake .. \
    -DBACKUP_SYSTEM=BAREOS \
    -DUSE_STATIC_OPENSSL=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DDEBUG_JSON=OFF
```

## 📚 Documentation

Full documentation is available at **[onesimus.io/docs/](https://onesimus.io/docs/)**

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
├── include/                             # Headers (.h)
│   ├── bmainwindow.h                    #   Application shell, menus, toolbar, ACL dialog
│   ├── bconnectionwizard.h              #   Connection wizard (auto-detect, TLS)
│   ├── blogging.h                       #   Conditional file logging (BLOG_* macros)
│   ├── bpasswordutil.h                  #   MD5 password hashing for CRAM-MD5 auth
│   ├── btranslations.h                  #   i18n singleton (6 languages)
│   ├── bcleanupdialog.h                 #   Old job cleanup dialog with filters
│   ├── bcheckableheaderview.h           #   Table header with checkboxes
│   ├── bconnectionprofile.h             #   Connection profile data structure
│   ├── bconsolemodel.h                  #   Console resource model
│   ├── bpaginationwidget.h              #   Pagination controls for large tables
│   ├── storagewidget.h                  #   Storage/volume management tab
│   ├── version.h                        #   Auto-generated version, build number, git info
│   ├── clients/                         # Client management
│   │   ├── bclientswidget.h             #   Client list + filtering
│   │   ├── bclientwidget.h              #   Single client detail widget
│   │   ├── bclientdetailsdialog.h       #   Client details dialog
│   │   ├── bclientsmodel.h              #   Client list model with online/offline status
│   │   └── bnewclientdialog.h           #   New Client Wizard (3-page)
│   ├── config/                          # Configuration & settings
│   │   ├── bdirectiveschema.h           #   JSON directive schema loader (singleton)
│   │   ├── bresourceform.h              #   Schema-driven form widget (auto-generated UI)
│   │   ├── bconfigexporter.h            #   Config export (bconsole.conf, client config)
│   │   ├── bconfigparser.h              #   Config file parser
│   │   ├── bcolumnconfiguration.h       #   Table column visibility/order settings
│   │   ├── bincludeoptionsform.h        #   FileSet include options form
│   │   ├── beditablelistwidget.h        #   Editable list widget (add/remove/reorder)
│   │   ├── bprofilesettingsdialog.h     #   Connection profile settings dialog
│   │   ├── bsettings.h                  #   Application settings (QSettings singleton)
│   │   └── bsettingsdialog.h            #   Settings UI (categorized)
│   ├── db/                              # SQLite database layer (deprecated)
│   │   └── bresourcemodel.h             #   Resource model (QSqlTableModel)
│   ├── director/                        # Director communication
│   │   ├── bdirector.h                  #   Abstract base — thread-safe command queue
│   │   ├── bareosdirector.h             #   Bareos JSON-RPC protocol implementation
│   │   ├── bareosauth.h                 #   CRAM-MD5 / TLS-PSK / X.509 auth
│   │   ├── bjsonstreamreader.h          #   Streaming JSON parser (large responses)
│   │   ├── bresourcedialog.h            #   Schema-driven resource edit dialog
│   │   ├── bresourcewidget.h            #   Resource browser widget
│   │   └── bresourcewidgets.h           #   Specialized resource edit widgets
│   ├── jobs/                            # Job management
│   │   ├── bjobwidget.h                 #   Main job tab (table, filters, stats, log)
│   │   ├── bjobmodels.h                 #   BJobsModel, BJobsFilterModel, BJobLogModel
│   │   ├── bjobdetailsdialog.h          #   Job details dialog with BVFS browser
│   │   ├── bjoblogdialog.h              #   Job log viewer dialog
│   │   ├── bjobfileswidget.h            #   BVFS file browser widget
│   │   ├── bjobsstatisticswidget.h      #   Job statistics charts/counters
│   │   ├── bbvfsmodel.h                 #   BVFS tree model (QAbstractItemModel)
│   │   ├── brestorewizard.h             #   Restore Wizard (5-page QWizard)
│   │   ├── bjobwizard.h                 #   Add Job/JobDefs Wizard (WIP)
│   │   ├── bnewjobdialog.h              #   Run New Job dialog
│   │   ├── bfilesetwizard.h             #   FileSet creation wizard
│   │   ├── bjsonjobview.h               #   Raw JSON job data viewer
│   │   ├── blevelcolors.h               #   Backup level color definitions
│   │   ├── bviewpresets.h               #   Table view presets (column sets)
│   │   └── fileset/                     # FileSet sub-components
│   │       ├── bfilesetdocument.h       #   FileSet document model
│   │       └── bincludeblockwidget.h    #   Include/Exclude block editor
│   ├── messages/                        # Messages management
│   │   └── bmessageswidget.h            #   Messages resource tab
│   ├── models/                          # Shared data models
│   │   ├── bbasemodels.h                #   BListModel, BTableModel (base classes)
│   │   └── bresourcemodels.h            #   BScheduleModel, BJobDurationStats, BFilesetModel, ...
│   └── schedules/                       # Schedule visualization (WIP)
│       ├── bschedulewidget.h            #   Schedule tab (left panel + Gantt/Grid views)
│       ├── bscheduleganttwidget.h       #   Gantt timeline widget (dual FD/SD)
│       ├── bweeklyplanner.h             #   Compact 7x24 weekly grid view
│       ├── bschedulewizard.h            #   Schedule Wizard (3-page QWizard)
│       ├── bscheduledragresultdialog.h  #   Drag result dialog (configure commands)
│       └── bjobscheduleindex.h          #   Schedule-to-job mapping index
├── src/                                 # Implementations (.cpp) — mirrors include/
├── resources/                           # Qt resources (embedded at build time)
│   ├── directives/                      #   JSON schemas for 13 resource types
│   ├── templates/filesets/              #   22 FileSet presets
│   ├── icons/                           #   53 SVG/PNG icons
│   ├── themes/                          #   QSS stylesheets (dark/light)
│   ├── translations/                    #   Directive translation files
│   └── sql/                             #   DB schema + migrations (v1–v5)
├── translations/                        # Qt Linguist .ts files (6 languages)
├── test/                                # Tests, test configs, test data generator
├── external/openssl/                    # OpenSSL 3.x (Git submodule)
├── wiki/                                # GitHub Wiki (Git submodule)
├── docs/                                # Technical documentation
├── build.sh / build.fish                # Linux build scripts
├── build-windows.ps1                    # Windows build script
├── CMakeLists.txt                       # Build configuration
└── CHANGELOG.md / LICENSE / todo.md     # Project docs
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
- 📖 Documentation: [onesimus.io/docs/](https://onesimus.io/docs/)
- 🐛 Bugs: [GitHub Issues](https://github.com/Beerlesklopfer/Onesimus/issues)
- 💬 Discussions: [GitHub Discussions](https://github.com/Beerlesklopfer/Onesimus/discussions)

## 🔄 Version History

### v0.2.0 (February 2026)
- ✅ **Gantt Scheduler** — Timeline/Gantt visualization for backup schedule planning
  - Day (24h) and Week (7x24h) views with zoom, grouping, now-marker
  - Job duration bars from historical data, color-coded by backup level
  - Utilization heatmap (15-min slots, green/yellow/red)
  - Collision detection (client/storage conflicts, visual warnings)
  - Duration statistics panel (min/avg/max, trend, last 5 runs)
- ✅ **Schedule Wizard** — Schema-driven 3-page QWizard for creating schedules
  - 5 predefined templates from `schedule.json` metadata
  - Run directive editor with live preview
  - Config text generation and `configure add` execution
- ✅ **BScheduleModel** + **BJobDurationStats** — data foundation for schedule analysis
- ✅ **BDirectiveSchema** extended with schedule metadata parsing
- ✅ **i18n fix** — Schedule widgets now translatable (previously hardcoded German)

### v0.1.0 (February 2026)
- ✅ **Restore Wizard** — Full BVFS-based 5-page QWizard (SelectJob → Browse → Options → Preview → Execute) (Build 345)
  - Permission checks via `.help all`, My Permissions dialog
  - Step indicators, progress bar, size estimate, auto-refresh
  - Checkbox propagation in file browser
- ✅ **ACL Visualization** — My Permissions dialog with grouped command permissions (Backup & Restore, Job Control, Administration, Media)
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

### Roadmap

**Community (free, MIT)** — See what IS:
- ✅ Pool overview & visualization
- ✅ Storage consumption per job/client
- ✅ Gantt timeline & weekly planner
- 🚧 Job/JobDefs Wizard (WIP)
- 🔜 Live job monitoring, volume operations

**Pro** — See what COULD BE:
- 🔜 Storage growth trends & capacity forecasts
- 🔜 Optimization suggestions & volume recycling recommendations
- 🔜 Schedule conflict detection & backup window optimization

**Enterprise** — Scale across infrastructure:
- 🔜 Multi-Director support
- 🔜 Capacity planning
- 🔜 Retention analysis & what-if simulation
- 🔜 RBAC, LDAP/AD integration

---

<p align="center">
  Made with ❤️ using Qt6 and C++17
</p>
