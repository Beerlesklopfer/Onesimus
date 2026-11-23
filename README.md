# Onesimus — Modern Backup Management UI

<p align="center">
  <strong>Every backup admin asks the same 5 questions. Onesimus is being built to answer them visually.</strong><br>
  <a href="https://onesimus.io">onesimus.io</a> · <a href="https://onesimus.io/docs/">Documentation</a> · <a href="https://github.com/Onesimus-io/Onesimus/issues">Report Bug</a>
</p>

<p align="center">
  <a href="https://github.com/Onesimus-io/Onesimus/actions/workflows/ci.yml"><img src="https://github.com/Onesimus-io/Onesimus/actions/workflows/ci.yml/badge.svg" alt="CI"></a>
  <img src="https://img.shields.io/badge/Qt-6.8%2B-green?logo=qt" alt="Qt 6.8+">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue?logo=c%2B%2B" alt="C++ 17">
  <img src="https://img.shields.io/badge/OpenSSL-3.6-orange?logo=openssl" alt="OpenSSL 3.6">
  <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey" alt="Cross-Platform">
  <img src="https://img.shields.io/badge/Status-Alpha-red" alt="Alpha">
  <img src="https://img.shields.io/badge/License-AGPL--3.0-blue" alt="AGPL-3.0">
</p>

---

## The 5 Questions

| # | Question | Answer | Status |
|---|----------|--------|--------|
| 1 | **Where does my storage go?** | Pool overview & visualization | planned |
| 2 | **When will the pool be full?** | Growth trends & capacity forecasts | planned |
| 3 | **Which jobs/clients consume the most?** | Storage consumption per job & client | planned |
| 4 | **What happens if I change retention?** | Retention analysis & what-if simulation | planned |
| 5 | **Which volumes can I recycle?** | Smart recycling recommendations | partly: the volume table shows when a volume is reusable |

Every feature is part of every edition. The editions differ only in how much the Director may write per month and in
support; up to 1 TB a month is free. See [Pricing](https://onesimus.io/pricing/).

Onesimus is a native Qt6 desktop application that connects directly to your Bareos Director via the bconsole protocol.

### 🖥️ Desktop-Native — No Server Required

Onesimus is a native desktop application that runs on **your workstation**: no web server, no PHP, no browser session. Install it, enter your Director credentials, done. It installs nothing on the backup server and connects to the Director like a console does.

### 🔒 Native Bareos API — No Direct SQL

Onesimus **never accesses the database directly**. No SQL queries, no direct catalog access. All data flows through the native **Bareos JSON-RPC API** — the same protocol bconsole uses. Every action passes through the Director's **ACL checks and audit trail**. Onesimus can't bypass permissions, and everything it does is logged.

> **Alpha Notice:** This project is under active development. Not suitable for production use. Features may change. Use at your own risk. See [Disclaimer](https://onesimus.io/legal/disclaimer/). Current development: [`development` branch](https://github.com/Onesimus-io/Onesimus/tree/development).

---

## ✨ Features

### 🎨 User Interface
- **Modern Qt6 Design:** Dark Industrial theme with intuitive navigation
- **Multi-Tab Interface:** Jobs, Clients, Storage, Schedules in separate tabs
- **Flexible Layouts:** Resizable widgets and customizable views
- **Responsive Design:** Optimized for various screen sizes

### 🌍 Internationalization (i18n)
- **6 Languages** (View > Language, the system language by default): 🇬🇧 English *(source)* • 🇩🇪 Deutsch *(99.9 %)* • 🇪🇸 Español • 🇫🇷 Français • 🇮🇹 Italiano • 🇷🇺 Русский *(99 % each)*
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

### 🗄️ Pool & Storage *(v0.3.0 — planned)*
- **Pool Overview:** Visual pool overview with volume status *(planned)*
- **Storage per Job/Client:** See exactly which jobs and clients consume storage *(planned)*
- **Volume Management:** Pools, volumes, media status, capacity per pool *(planned)*
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
- ~~**Bacula:** support via bconsole (TCP/TLS)~~ *(deprecated for now, not available)*
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
- **Windows Installer:** MSI (WiX) with the GUI, and optionally oconsole and the ofd File Daemon service

## 🎯 Supported Backup Systems

| System | bconsole | JSON-RPC | Status |
|--------|----------|----------|--------|
| **Bacula** | ❌ | ❌ | Deprecated for now |
| **Bareos** | ✅ TCP/TLS | ✅ Native | Full |

### Choose Build Option

```bash
# Bareos (default and currently only supported)
cmake .. -DBACKUP_SYSTEM=BAREOS

# Bacula only (deprecated for now, not available)
cmake .. -DBACKUP_SYSTEM=BACULA
```



## 🚀 Quick Start

### Windows

```powershell
# Developer PowerShell for VS 2022 (deploy.ps1 loads the VS environment itself if needed)
.\deploy.ps1            # build + windeployqt into build\deploy
.\deploy.ps1 installer  # MSI (alias: msi); also zip, run, test, status, clean, help
```

`deploy.bat` is the double-click launcher (`deploy.bat rebuild` for a clean rebuild).

### Linux / macOS

macOS: `brew install qt@6 cmake` (see [Prerequisites](#-prerequisites))

```bash
./deploy.sh [flags] [command]

Commands:
  all | onesimus | gui   Build, then copy binary, desktop entry and icons to
                         /usr (Linux, sudo per step) or /usr/local/bin (macOS) - default
  build | rebuild        Build only (nothing outside the build tree)
  run [args] | test      Build and start onesimus / run ctest
  install                cmake --install into PREFIX (default /usr/local)
  package | appimage     .deb (Linux) or .dmg (macOS) / AppImage in dist/appimage
  configure | status | clean | help

Flags:
  -t, --type TYPE        Build type: Release (default), Debug, RelWithDebInfo
  -s, --system-openssl   Use system OpenSSL instead of static submodule
  -c, --clean            Remove build directory before building
  -j, --jobs N           Parallel build jobs (default: nproc)
  -d, --build-dir DIR    Build directory (default: build)
  -n, --dry-run          Print every command instead of running it
  -h, --help             Show the full command table

Examples:
  ./deploy.sh                    # build + deploy to /usr
  ./deploy.sh -n                 # show exactly what a deploy would run
  ./deploy.sh build -t Debug     # Debug build, no deploy
  ./deploy.sh build -s -c        # System OpenSSL, clean rebuild
```

`./deploy.fish` has the same commands, flags and environment variables for Fish
users. Every build increments `.build_number` and regenerates `include/version.h`.

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
| `BACULA` | Bacula support only *(deprecated for now, not available)* |

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

## 🔁 CI and Releases

Every push and pull request runs [`.github/workflows/ci.yml`](.github/workflows/ci.yml):

| Job | What it does |
|---|---|
| Linux | Debian 13 container (distribution Qt 6.8 with QtGrpc): `./deploy.sh configure`, `build`, `test` (ctest offscreen), lupdate check (warning only) |
| Windows | MSVC 2022, Qt 6.8.3: `deploy.ps1 build`, `test`, `installer` (GUI-only MSI as an artifact) |
| macOS | Build only, allowed to fail (the app bundle is still open) |

The static OpenSSL build is cached per OpenSSL submodule commit
(`OPENSSL_INSTALL_DIR`), so only the first run on a new OpenSSL commit compiles it.

### Cutting a release

The first public release through this pipeline is **v1.0.0**.

1. Bump the version in `CMakeLists.txt`: `project(onesimus VERSION 1.0.0 ...)`.
   Update `CHANGELOG.md`, commit, push, and wait for a green CI run.
2. Tag that commit and push the tag:
   ```bash
   git tag -a v1.0.0 -m "Onesimus 1.0.0"
   git push origin v1.0.0
   ```
   A tag with a suffix (`v1.0.0-rc.1`) becomes a pre-release.
3. [`.github/workflows/release.yml`](.github/workflows/release.yml) checks that the
   tag matches the project version (it stops with an error if not), builds the
   `.deb`, the AppImage, the MSI and the portable ZIP, writes `SHA256SUMS`, and
   creates a **draft** GitHub Release for the tag with all files attached.
4. Review the draft on the Releases page (notes, files, checksums) and publish it by hand.

The MSI contains oconsole and the ofd service only when a Windows build of
onesimus-dird is given: set the repository variables `DIRD_WINDOWS_ZIP_URL` and
`DIRD_WINDOWS_ZIP_SHA256` (URL and SHA-256 of `onesimus-dird-<version>-windows-x86_64.zip`),
or start the workflow by hand (Actions → Release → Run workflow) with the tag and
those two values. Without them the MSI holds the GUI only.

Silent MSI installation (features `Gui`, `DesktopShortcut`, `Oconsole`, `Ofd`):

```powershell
msiexec /i Onesimus-1.0.0-windows-x86_64.msi /qn                                    # GUI, shortcuts, oconsole
msiexec /i Onesimus-1.0.0-windows-x86_64.msi ADDLOCAL=Gui,Oconsole,Ofd /qn /l*v install.log
```

`ADDLOCAL` installs exactly the listed features (add `DesktopShortcut` for the
desktop icon). The ofd service is registered with start mode *manual*; it is not
started by the installer because it needs a configuration in
`C:\ProgramData\Onesimus\bareos-fd.d\` first.

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
│   ├── bpaginationwidget.h              #   Pagination controls for large tables
│   ├── storagewidget.h                  #   Storage/volume management tab
│   ├── version.h                        #   Auto-generated version, build number, git info
│   ├── clients/                         # Client management
│   │   ├── bclientswidget.h             #   Client list + filtering
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
│       └── bjobscheduleindex.h          #   Schedule-to-job mapping index
├── src/                                 # Implementations (.cpp) — mirrors include/
├── resources/                           # Qt resources (embedded at build time)
│   ├── directives/                      #   JSON schemas for 13 resource types
│   ├── templates/filesets/              #   22 FileSet presets
│   ├── icons/                           #   53 SVG/PNG icons
│   ├── themes/                          #   QSS stylesheets (dark/light)
│   └── sql/                             #   DB schema + migrations (v1–v5)
├── translations/                        # Qt Linguist .ts files (6 languages)
├── test/                                # Tests, test configs, test data generator
├── external/openssl/                    # OpenSSL 3.x (Git submodule)
├── wiki/                                # GitHub Wiki (Git submodule)
├── docs/                                # Technical documentation
├── deploy.sh / deploy.fish              # Linux/macOS build + deploy scripts
├── deploy.ps1 / deploy.bat              # Windows build + deploy script
├── CMakeLists.txt                       # Build configuration
└── CHANGELOG.md / TODO.md / LICENSE      # Project docs
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

Onesimus is licensed under the GNU Affero General Public License, version 3 (see [LICENSE](LICENSE)). Its own code is also available under any later version (AGPL-3.0-or-later); files that follow the Bareos source are AGPL-3.0-only with the Bareos copyright holders, so the program as a whole is distributed under AGPL-3.0. Each file states its licence in an SPDX header; [LICENSING.md](LICENSING.md) lists the Bareos-related files.

Third-party components of the packages (Qt, Qt gRPC, OpenSSL, Feather icons, and the libraries bundled in the AppImage) are listed with their licences in [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md); the licence texts are in [LICENSES/](LICENSES/).

## 🙏 Credits

### Technologies

- [Qt Framework](https://www.qt.io/) - Cross-platform UI
- [OpenSSL](https://www.openssl.org/) - TLS/SSL Encryption
- [CMake](https://cmake.org/) - Build System

### Backup Systems

- [Bacula](https://www.bacula.org/) - Open Source Backup
- [Bareos](https://www.bareos.com/) - Bacula Fork with Enterprise Features

## 💛 Support the Project

Onesimus is built by one developer in spare time. If it saves you hours of `bconsole` work, consider buying me a coffee:

**PayPal:** [Donate via PayPal Pool](https://www.paypal.com/pool/9mSSenuhxX?sr=wccr)

Every contribution helps keep development going — and shows that open source backup tooling matters.

## 📞 Support

- 🌐 Website: [onesimus.io](https://onesimus.io)
- 📖 Documentation: [onesimus.io/docs/](https://onesimus.io/docs/)
- 🐛 Known bugs and planned work: [TODO.md](TODO.md); report new ones on [GitHub Issues](https://github.com/Onesimus-io/Onesimus/issues)
- 💬 Discussions: [GitHub Discussions](https://github.com/Onesimus-io/Onesimus/discussions)

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

### v0.0.1 (January 2026)
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
- ✅ Better debug logging with component prefixes

### Roadmap

Every feature below is part of every edition ([Pricing](https://onesimus.io/pricing/)).

**Management interface:**
- ✅ Job, Client & Schedule management
- ✅ Gantt timeline & weekly planner
- ✅ Restore wizard with BVFS file browser
- ✅ Predicted waiting times and proposed start times (stagger)
- ✅ Multi-Director support (connection profiles)
- ✅ Edit Job and Edit JobDefs over gRPC
- ✅ Live job log
- ✅ ACL matrix of consoles and profiles
- ✅ 3-2-1 Verifier
- 🔜 Pool management, volume operations

**Storage intelligence:**
- 🔜 Pool overview & storage visualization
- 🔜 Storage consumption per job/client
- 🔜 Growth trends & capacity forecasts
- 🚧 Volume recycle recommendations ("reusable from" in the volume table)
- 🚧 Pre-built binaries: .deb, AppImage, MSI and a signed and notarised macOS dmg; the Linux and Windows packages are not signed yet

**Scale across infrastructure:**
- 🔜 Simultaneous Director connections
- 🔜 Capacity planning
- 🔜 Retention analysis & what-if simulation
- 🔜 RBAC, LDAP/AD integration *(depends on Bareos team upstream support)*

---

<p align="center">
  Made with ❤️ using Qt6 and C++17 <a href="https://onesimus.io/about/">to praise my Lord</a>
</p>
