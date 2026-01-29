# Changelog

All notable changes to Onesimus will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-01-29

### Added - Core Infrastructure

#### Authentication Layer (BaculaAuth / BareosAuth)
- ✅ **BaculaAuth**: Bacula authentication with CRAM-MD5 challenge-response
  - PSK (Pre-Shared Key) authentication support
  - Certificate-based TLS authentication
  - Windows PFX file support (combined cert+key)
  - Linux/macOS separate cert/key file support
- ✅ **BareosAuth**: Bareos authentication extending BaculaAuth
  - Additional PSK identity support for Bareos Directors
  - Bareos-specific handshake protocol
- ✅ **OpenSSL 3.6 Integration**: Static linking via Git submodule
  - Automatic download and build
  - Cross-platform support (Windows, Linux, macOS)
  - TLS 1.2+ protocol support
  - EC and RSA key support

#### Director Communication Layer
- ✅ **BDirector**: Abstract base class for Director communication
  - TCP/TLS socket communication
  - Command queue management
  - Asynchronous request/response handling
  - Connection state management
  - Stored connection credentials
- ✅ **BareosDirector**: Bareos-specific implementation
  - JSON-RPC support for Bareos API mode
  - `.api json` command initialization
  - Streaming JSON response handling with BJsonStreamReader
  - Dot-command support (.jobs, .clients, .storages, .filesets, etc.)
  - Real-time event signaling (connected, disconnected, error, dataReceived)

#### Main Application Window
- ✅ **MainWindow**: Central application hub
  - Modern Qt6 design with Industrial Dark theme
  - Multi-tab interface (Jobs, Clients, Storage, Schedules)
  - Menu bar with File, Edit, View, Jobs, Clients, Storage, Schedules, Help menus
  - Toolbar with quick-access actions
  - Status bar with connection status and operation feedback
  - Statistics dock widget (collapsible, persistent state)
  - Auto-refresh timer for periodic data updates
  - Signal/slot connections to Director and Widgets
  - Settings dialog integration
  - About dialog

### Added - Widget Implementations (Left to Right)

#### 1. Jobs Widget (BJobWidget)
- ✅ **Job Table View**: Complete job listing with BJobsModel
  - Columns: Selection, JobId, Name, Client, Start Time, End Time, Duration, Type, Level, Files, Bytes, Status
  - Color-coded status indicators (Success=green, Warning=yellow, Failed=red)
  - Sortable columns
  - Multi-selection with checkboxes
- ✅ **Advanced Filtering** (BJobsFilterModel):
  - Filter by job name (text search)
  - Filter by client name (text search)
  - Filter by status (Successful, Warning, Failed)
  - Filter by level (Full, Incremental, Differential, VirtualFull)
  - Filter by date range (From/To with QDateTimeEdit)
  - Filter by file count range
  - Filter by byte size range
  - Combined filters (all active simultaneously)
- ✅ **Statistics Panel**:
  - Total jobs count
  - Successful jobs count
  - Warning jobs count
  - Failed jobs count
  - Total bytes backed up (human-readable format)
  - Total files backed up
  - Earliest job date
  - Latest job date
  - Selected jobs count
- ✅ **Job Log Viewer**:
  - Detailed log view for selected job (BJobLogModel)
  - Monospace font for log readability
  - Auto-scroll to latest entries
  - Collapsible panel
- ✅ **Job Details Dialog** (BJobDetailsDialog):
  - Complete job information
  - Job log with syntax highlighting
  - Job files list (if available)
  - Job metadata (JobId, Name, Type, Level, Client, FileSet, Pool, Storage)
- ✅ **Pagination Support**:
  - Configurable page size (default: 50 jobs per page)
  - Page navigation controls
  - Efficient handling of thousands of jobs
- ✅ **Export Functionality**:
  - Export selected jobs as JSON
  - Export selected jobs as CSV
  - Export all jobs or filtered subset
- ✅ **Context Menu Actions**:
  - Run Job
  - Cancel Job
  - Show Details
  - Refresh Jobs
  - Select All
  - Clear Selection

#### 2. Clients Widget (BClientsWidget / ClientWidget)
- ✅ **Client List View**:
  - All configured backup clients
  - Client name, version, OS information
  - Online/Offline status with visual indicators
  - Last connection timestamp
- ✅ **Client Filtering**:
  - Filter by client name (combo box)
  - Show/hide offline clients (checkbox)
- ✅ **Client Details**:
  - Client configuration display
  - Recent job history for client
  - File retention statistics
- ✅ **Context Menu Actions**:
  - Show client details
  - Run backup job for client
  - Refresh client list

#### 3. Storage Widget (StorageWidget)
- ✅ **Storage Daemon List**:
  - All configured storage daemons
  - Device status (Available, Busy, Offline)
  - Storage capacity information
- ✅ **Pool Management**:
  - List all pools (BPoolModel)
  - Pool details (name, type, volumes, bytes)
  - Volume statistics per pool
- ✅ **Volume Management**:
  - List volumes in selected pool
  - Volume status (Append, Full, Used, Error)
  - Volume capacity and usage
  - Media type information
- ✅ **Device Status**:
  - Device availability
  - Current operations
  - Mount/Unmount status
- ✅ **Context Menu Actions**:
  - Refresh storage list
  - Show pool details
  - Label new volume (planned)
  - Mount/Unmount device (planned)

#### 4. Schedules Widget (BScheduleWidget)
- ✅ **Schedule List View**:
  - All configured backup schedules
  - Schedule name and description
  - Run times and frequency
- ✅ **Schedule Details**:
  - Run times (daily, weekly, monthly patterns)
  - Backup level assignment (Full, Incremental, Differential)
  - Pool assignment
  - Associated jobs
- ✅ **Visual Representation**:
  - Calendar view of scheduled backups (planned)
  - Timeline view of next runs (planned)
- ✅ **Context Menu Actions**:
  - Refresh schedules
  - Show schedule details
  - Enable/Disable schedule (planned)

### Added - Data Models (MVC Architecture)

#### Base Models
- ✅ **BListModel**: Base class for list-based data models
  - JSON array parsing
  - Item access by index
  - Display text customization
  - Data update signals
- ✅ **BTableModel**: Base class for table-based data models
  - Configurable columns (key, header, alignment)
  - JSON array parsing
  - Cell data formatting
  - Header management
- ✅ **BTreeModel**: Base class for tree-based data models
  - Hierarchical data support
  - Parent-child relationships
  - Recursive tree building

#### Resource Models
- ✅ **BFilesetModel**: Fileset management (extends BListModel)
  - Parse `.filesets` dot-command response
  - Fileset names extraction
- ✅ **BStorageModel**: Storage daemon management (extends BListModel)
  - Parse `.storages` dot-command response
  - Storage names extraction
- ✅ **BPoolModel**: Pool management (extends BListModel)
  - Parse `.pools` dot-command response
  - Pool names and details extraction
  - Pool info by name lookup
- ✅ **BLevelModel**: Backup level management (extends BListModel)
  - Parse `.levels` dot-command response
  - Level codes (F, I, D, V) and descriptions

#### Job Models
- ✅ **BJobsModel**: Main job table model (extends QAbstractTableModel)
  - 12 columns with custom rendering
  - Checkbox selection tracking (QSet)
  - Live data updates via `appendJobs()`
  - Pagination support
  - Statistics calculation
  - Export to JSON/CSV
- ✅ **BJobsFilterModel**: Filter proxy model for BJobsModel
  - Multiple filter criteria (name, client, status, level, date, files, bytes)
  - Combined filter logic
  - Real-time filtering
- ✅ **BJobLogModel**: Job log list model (extends QAbstractListModel)
  - Parse JSON-RPC log responses
  - Plain text fallback
  - Monospace font rendering
- ✅ **BFilterComboModel**: Combo box data model
  - Extract unique job names from job array
  - Extract unique client names from client array
  - Update from dot-command responses

### Added - Support Infrastructure

#### JSON Processing
- ✅ **BJsonStreamReader**: Streaming JSON parser
  - Efficient parsing of large responses (10,000+ jobs)
  - Line-by-line processing
  - Memory-efficient (no full document load)
  - Support for Bareos JSON-RPC format

#### Settings Management
- ✅ **BSettings**: Centralized settings singleton
  - **Connection Settings**: Host, port, director, password, auto-connect, timeout
  - **TLS Settings**: PSK/Certificate mode, CA cert, client cert, client key, PFX file, verify peer
  - **Appearance Settings**: Theme, font size, animations, compact mode, **language**
  - **Level Colors**: Customizable colors for Full, Incremental, Differential, VirtualFull
  - **Behavior Settings**: Confirm job cancel, auto-refresh, refresh interval, max jobs display
  - **Advanced Settings**: Debug logging, log file, max log size, tooltips
  - **Widget State**: Jobs statistics visibility, filter states, client filters, storage filters
  - QSettings-based persistence
  - Signal emissions on setting changes

#### Internationalization (i18n)
- ✅ **BTranslations**: Translation management singleton
  - **6 Languages**: English (en), German (de), Spanish (es), French (fr), Italian (it), Russian (ru)
  - **Automatic Language Detection**: Detects system locale on first start
  - **Language Switching**: Runtime language change (requires restart for full effect)
  - **Flag Emojis**: Visual language indicators (🇬🇧 🇩🇪 🇪🇸 🇫🇷 🇮🇹 🇷🇺)
  - Qt Linguist integration (.ts/.qm files)
  - QLocale management for date/time/number formatting
  - **Settings Integration**: Language preference saved and restored
- ✅ **Translation Files** (Qt Linguist .ts files):
  - `translations/onesimus_de.ts` - German
  - `translations/onesimus_en.ts` - English
  - `translations/onesimus_es.ts` - Spanish
  - `translations/onesimus_fr.ts` - French
  - `translations/onesimus_it.ts` - Italian
  - `translations/onesimus_ru.ts` - Russian
- ✅ **CMake Integration**: Automatic .qm file generation with `qt_add_translations()`
- ✅ **Source String Normalization**: All UI strings in English with `tr()` wrapper

#### Settings Dialog
- ✅ **SettingsDialog**: Modern categorized settings UI
  - Industrial Dark Design with sidebar navigation
  - **Connection Page**: Director connection settings, TLS configuration
  - **Appearance Page**: Theme selection, font size, animations, compact mode, **language selector with flags**
  - **Behavior Page**: Auto-refresh, job display limits, confirmation dialogs
  - **Advanced Page**: Debug logging, tooltips, log file management
  - **Level Colors Page**: Customizable backup level colors with color pickers
  - Apply/Cancel/Reset to Defaults buttons
  - Settings validation and immediate feedback

#### Utility Components
- ✅ **BCheckableHeaderView**: Table header with "select all" checkbox
  - Master checkbox in first column
  - Synchronizes with model selection state
- ✅ **BCheckboxDelegate**: Custom item delegate for checkbox rendering
  - Centered checkbox in table cells
  - Mouse click handling
- ✅ **BColumnConfiguration**: Table column visibility and order management
  - Show/hide columns
  - Reorder columns
  - Column width persistence
- ✅ **BPaginationWidget**: Pagination controls for large datasets
  - Previous/Next buttons
  - Page number display
  - Jump to page
  - Page size configuration

### Added - Build System

#### CMake Configuration
- ✅ **Backup System Selection**: `-DBACKUP_SYSTEM=BACULA|BAREOS|BOTH`
  - Compile-time or runtime system selection
  - Conditional compilation with preprocessor defines
- ✅ **OpenSSL Integration**: Static OpenSSL 3.6 via Git submodule
  - Automatic download: `git submodule update --init --recursive`
  - Automatic build via ExternalProject
  - Cross-platform configuration
  - Alternative: System OpenSSL with `-DUSE_STATIC_OPENSSL=OFF`
- ✅ **Qt Linguist Tools**: Automatic translation file processing
  - `lupdate`: Extract translatable strings from source
  - `lrelease`: Compile .ts files to .qm binaries
  - Resource embedding: `:/translations` prefix
- ✅ **Debug Options**:
  - `-DDEBUG_PACKETS=ON`: Packet-level debugging
  - `-DDEBUG_JSON=ON`: JSON parsing debugging
  - `-DLOG_JSON=ON`: Log all JSON responses
- ✅ **AppImage Support**: Linux deployment with linuxdeploy
  - Automatic Qt plugin bundling
  - Automatic library dependency resolution
  - Desktop file and icon integration

#### Build Scripts
- ✅ **build-windows.ps1**: PowerShell build script for Windows
  - Visual Studio 2022 environment detection
  - Automatic Perl detection (required for OpenSSL)
  - Qt6 path configuration
  - MSVC compiler invocation
  - Error handling and user feedback
- ✅ **build.sh**: Bash build script for Linux/macOS
  - Dependency checking
  - CMake configuration
  - Parallel build with `-j` flag
  - Post-build verification

### Added - Documentation

- ✅ **README.md**: Comprehensive project overview
  - Feature showcase with categorized sections
  - Installation instructions per platform
  - Build options and examples
  - Quick start guides
  - Internationalization documentation
  - MVC architecture explanation
  - Contributor guidelines
- ✅ **BUILD_WINDOWS.md**: Detailed Windows build instructions
- ✅ **BUILD_LINUX.md**: Detailed Linux build instructions
- ✅ **BUILD_OSX.md**: Detailed macOS build instructions
- ✅ **BACKUP_SYSTEMS.md**: Bacula vs Bareos comparison
- ✅ **STATIC_OPENSSL.md**: OpenSSL integration documentation
- ✅ **VISUAL_STUDIO_ENV.md**: Visual Studio environment setup

### Changed

#### From Initial Release
- 🔄 **Project Rename**: "Bacula UI" → "Onesimus"
- 🔄 **Backup System Support**: Bacula-only → Bacula + Bareos with flexible configuration
- 🔄 **Default Language**: German → English (with German translation)
- 🔄 **Translation Files**: `bacula_*.ts` → `onesimus_*.ts`
- 🔄 **OpenSSL Integration**: System OpenSSL → Static Git submodule (optional system fallback)

### Technical Stack

#### Frontend
- **Qt 6.8+**: Cross-platform UI framework
- **C++17**: Modern C++ standard
- **MOC**: Meta-Object Compiler for signals/slots
- **Qt Linguist**: Translation management

#### Backend Communication
- **TCP/TLS Sockets**: Encrypted Director communication
- **JSON-RPC**: Bareos API mode
- **Streaming Parser**: Large dataset handling

#### Security
- **OpenSSL 3.6**: TLS/SSL encryption
- **CRAM-MD5**: Challenge-response authentication
- **PSK**: Pre-shared key authentication
- **X.509 Certificates**: Certificate-based authentication

#### Build System
- **CMake 3.16+**: Cross-platform build configuration
- **ExternalProject**: Git submodule integration
- **Qt Resource System**: Embedded translations and assets

### Platform Support
- ✅ **Windows 10/11**: MSVC 2022, native PFX support
- ✅ **Linux**: GCC/Clang, Ubuntu 20.04+, Debian 11+
- ✅ **macOS**: Clang, macOS 11+, Homebrew dependencies

---

## [Unreleased] - Future Features

### Planned for v1.1
- 🔜 **Live Job Monitoring**: Real-time progress bars for running jobs
- 🔜 **Job Control**: Start, stop, cancel jobs from UI
- 🔜 **Volume Management**: Label, mount, unmount volumes
- 🔜 **Enhanced Statistics**: Charts and graphs (job success rate, backup size trends)
- 🔜 **Job Templates**: Pre-configured backup job templates
- 🔜 **Email Notifications**: Alert system for job failures
- 🔜 **Dashboard**: Overview page with key metrics
- 🔜 **Schedule Calendar View**: Visual schedule representation
- 🔜 **Restore Interface**: File browsing and restore wizard
- 🔜 **Client Management**: Add/edit/remove clients
- 🔜 **Dynamic Language Switching**: Change language without restart

### Under Consideration
- 🤔 **Multi-Director Support**: Manage multiple Directors simultaneously
- 🤔 **Report Generator**: Automated backup reports (PDF/HTML)
- 🤔 **Bareos WebUI Integration**: Embed Bareos WebUI views
- 🤔 **Plugin System**: Extensibility for custom widgets/commands
- 🤔 **Command History**: Saved command palette with history
- 🤔 **Backup Verification**: Verify backup integrity from UI
- 🤔 **Role-Based Access Control**: User permissions for different operations

---

## Development Timeline

### Phase 1: Foundation (Week 1-2)
1. ✅ **BaculaAuth**: CRAM-MD5 authentication
2. ✅ **BareosAuth**: Extended authentication with PSK identity
3. ✅ **OpenSSL Integration**: Static linking, TLS setup
4. ✅ **BDirector**: Abstract Director communication
5. ✅ **BareosDirector**: JSON-RPC implementation

### Phase 2: Core UI (Week 3-4)
6. ✅ **MainWindow**: Application shell with tabs, menus, toolbar, status bar
7. ✅ **Settings Infrastructure**: BSettings singleton with comprehensive settings
8. ✅ **SettingsDialog**: Categorized settings UI with Industrial Dark theme

### Phase 3: Widget Development (Week 5-8)
9. ✅ **BJobWidget** (Left): Jobs table, filters, statistics, log viewer, export
10. ✅ **ClientWidget** (Center-Left): Client list, status, filtering
11. ✅ **StorageWidget** (Center-Right): Storage daemons, pools, volumes
12. ✅ **BScheduleWidget** (Right): Schedule list and details

### Phase 4: Models & Data (Week 9-10)
13. ✅ **Base Models**: BListModel, BTableModel, BTreeModel
14. ✅ **Resource Models**: BFilesetModel, BStorageModel, BPoolModel, BLevelModel
15. ✅ **Job Models**: BJobsModel, BJobsFilterModel, BJobLogModel, BFilterComboModel
16. ✅ **JSON Streaming**: BJsonStreamReader for efficient large data parsing

### Phase 5: Internationalization (Week 11)
17. ✅ **BTranslations**: Singleton with 6 languages
18. ✅ **Language Detection**: Automatic system locale detection
19. ✅ **Translation Files**: .ts files for all languages
20. ✅ **Source Normalization**: Convert all German strings to English
21. ✅ **Settings Integration**: Language selector with flags

### Phase 6: Polish & Documentation (Week 12)
22. ✅ **Build Scripts**: Windows PowerShell, Linux/macOS bash
23. ✅ **Documentation**: README, BUILD guides, technical docs
24. ✅ **CHANGELOG**: Complete development history
25. ✅ **Testing**: Cross-platform testing, translation testing

---

## Known Issues

### Current Limitations
- Translation files incomplete (English source complete, German ~30%, Spanish/French/Italian/Russian 0%)
- Some German comments remain in code
- Job control (run/cancel) not yet implemented (UI present, backend missing)
- Volume management (label/mount) not yet implemented
- Client details dialog not yet implemented
- Schedule enable/disable not yet implemented
- Restore functionality not yet implemented

### Platform-Specific
- **Windows**: PFX password prompt not yet implemented (uses empty password)
- **Windows**: Schannel backend may fail with PEM-format EC keys
  - Error: `Failed to import private key: "Unknown error occurred: -2146885630"`
  - Cause: Windows Schannel has limited support for certain PEM key formats
  - Solutions:
    1. Use OpenSSL backend instead of Schannel (compile Qt with OpenSSL or provide OpenSSL DLLs)
    2. Convert key format from EC to RSA or use PKCS#12 (.pfx) format
    3. Use TLS tunnel on server-side (stunnel, nginx) and connect without client TLS
- **Linux**: AppImage creation requires manual linuxdeploy setup
- **macOS**: .app bundle creation not automated

---

## Credits

### Development
- **Author**: Joerg Bernau <Joerg@bernau.family>
- **Project**: Onesimus
- **License**: GPL-3.0

### Technologies
- [Qt Framework](https://www.qt.io/) - Cross-platform UI
- [OpenSSL](https://www.openssl.org/) - TLS/SSL Encryption
- [CMake](https://cmake.org/) - Build System

### Backup Systems
- [Bacula](https://www.bacula.org/) - Open Source Backup
- [Bareos](https://www.bareos.com/) - Bacula Fork with Enterprise Features

---

*"Managing backups should be simple, secure, and accessible in your language."*
