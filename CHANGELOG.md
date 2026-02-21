# Changelog

All notable changes to Onesimus will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.2.0] - 2026-02-21

### Added
- **Gantt Scheduler**: Full timeline/Gantt visualization for backup schedule planning
  - BScheduleGanttWidget: horizontal timeline with job duration bars
  - Day view (24h) and Week view (7x24h) with smooth toggle
  - Color-coded backup levels: Full (green), Differential (orange), Incremental (blue), VirtualFull (purple)
  - Bar width represents estimated job duration from historical data
  - Zoom control (Ctrl+Scroll or slider, 30-240 px/hour)
  - Now-marker: red vertical line showing current time
  - Group by Schedule or by Client
  - Rich tooltips: schedule name, level, time, client, duration, pool
- **Utilization Heatmap**: color-coded strip at bottom of Gantt view
  - Green (0-1 jobs), Yellow (2-3 jobs), Red (4+ parallel jobs)
  - 15-minute resolution, hover tooltips with concurrent job count
- **Collision Detection**: automatic detection of scheduling conflicts
  - Client collisions: multiple jobs on same client simultaneously
  - Storage collisions: multiple jobs writing to same storage
  - Visual: red border on conflicting bars, collision count in toolbar
- **BScheduleModel**: proper Qt model for schedule resources (like BFilesetModel, BPoolModel)
  - Parses `.schedule` response into structured BScheduleEntry objects
  - Automatic Run directive parsing (level, days, time, pool, storage, priority)
- **BJobDurationStats**: historical job duration statistics engine
  - Collects min/avg/max duration per job from `list jobs` data
  - Duration uncertainty bands on Gantt bars (dark=avg, light=max)
- **i18n**: all Schedule widget strings wrapped in `tr()` (previously hardcoded German)

### Changed
- **Version bumped to 0.2.0** — Gantt Scheduler milestone
- **BScheduleWidget**: completely rebuilt with Gantt as default view
  - Toolbar: view toggle (Timeline/Grid), Day/Week, day navigation, zoom, grouping
  - Grid view (BWeeklyPlanner) retained as compact alternative
- **BWeeklyPlanner**: day names and tooltips now use `tr()` for i18n

---

## [0.1.0] - 2026-02-21 (Build 345)

### Added
- **Restore Wizard**: Full BVFS-based 5-page QWizard (SelectJob → Browse → Options → Preview → Execute)
  - BVFS file browser with Windows Explorer-style directory tree and file list
  - Checkbox propagation (parent ↔ children) for file/directory selection
  - Restore options: Where, Replace mode, Client selection
  - Preview page with size estimate and file count summary
  - Live restore execution with progress bar and auto-refresh
  - Step indicators showing wizard progress
  - Permission checks via `.help all` to verify required Bareos ACLs before restore
- **ACL Visualization**: My Permissions dialog (Help menu → My Permissions)
  - Queries `.help all` and displays all Bareos console ACLs
  - Grouped by category: Backup & Restore, Job Control, Administration, Media
  - Color-coded permission status (granted / denied) per command
  - Restore details with available arguments
  - Other available commands overview
  - Raw Director response view for debugging
- **Website**: [onesimus.io](https://onesimus.io) — project website with documentation, blog, and legal pages (EN/DE)

### Changed
- **Qt >= 6.8 now required** (enforced in CMake)
- **Dynamic `USE_STATIC_OPENSSL`** — CMake auto-detects whether to use static or system OpenSSL based on availability
- **Default API mode** set to `json compact=yes` for better performance
- **Disclaimer links** updated to point to onesimus.io instead of GitHub wiki

### Fixed
- **WhereAcl**: Added missing WhereAcl for restore functionality
- **Restore browser**: Various fixes to BVFS file browser navigation

---

## [0.1.0] - 2026-02-04

### Added
- **New Client Wizard**: 3-page wizard (Director Info, Settings, Preview & Export) for adding new backup clients
  - Auto-populates Director info from active connection
  - Generates FD-side config files (director.conf, myself.conf, messages.conf) with TLS directives
  - Generates Director-side client.conf
  - Executes `configure add client` on Director with automatic `reload`
  - ZIP export for client deployment
  - Schema-driven preview using BResourceWidget (structured resource display with raw text pane)
  - ACL permission check for `configure` command
  - 30-second timeout with retry dialog for configure commands
  - Hides "Execute configure" checkbox when console lacks configure ACL
  - Hides Director command preview when execute checkbox is unchecked
- **BResourceDialog**: Dynamic schema-driven dialog for editing resource configurations
  - Single dialog class handles all resource types (Client, Job, Director, etc.)
  - Generates form fields dynamically from JSON directive schemas
  - Type-to-widget mapping (string, integer, boolean, password, file, directory, resource_reference, time, size, etc.)
  - Directive grouping by schema `group` field with QGroupBox sections
  - "Show advanced directives" toggle for directives with `use=false`
  - Pre-populates fields from existing BConfigResource values
  - Returns modified BConfigResource on accept
- **BResourceWidget Edit Button**: "Edit..." button on every resource widget opens BResourceDialog
  - Enabled when a resource is selected
  - Updates resource in-place and emits `resourceModified` signal
- **Client Details Settings Tab**: New "Einstellungen" tab in client details dialog (double-click)
  - Shows client directives in BResourceWidget with Edit button
  - Maps JSON client data to BConfigResource for schema-driven display
- **Client ZIP Export**: Right-click "Export as ZIP..." on client table
  - Creates ZIP archive with Bareos directory structure (`etc/bareos/bareos-fd.d/...`)
  - Includes FD-side director.conf, client/myself.conf, messages/Standard.conf
  - Includes Director-side client resource
- **Extended Directive Schema**: BDirective struct now parses `group`, `use`, `validValues`, `synonyms`, `minVersion` from JSON schemas
- **Messages Directive Schema**: New `messages.json` schema with 13 directives and German translations (`messages_de.json`)
- **Client Directive Translations**: German translations for Client resource directives (`client_de.json`)
- **Client Config Export**: Context menu on client table with "Export Configuration..." (Save As dialog)
- **BConfigParser::parseString()**: Public method for parsing config text from strings
- **Application Icon**: Window icon set from bundled Bareos/Bacula logos (build-time selection)
- **Debian Package**: CPack DEB generator with automatic dependency detection (`SHLIBDEPS`)
  - Desktop entry file for Linux application menu integration
  - Icon installation to hicolor icon theme

### Changed
- **Schema-Driven Validation**: New Client Wizard validates generated configs against directive schemas (subset validation — only validates present directives)
- **TLS-Only Authentication**: Removed legacy (CRAM-MD5 without TLS) option from New Client Wizard; only TLS-PSK and TLS Certificate modes supported
- **CPack Configuration**: Extended from Windows-only NSIS to cross-platform packaging (DEB, NSIS)
- **Build Number System**: Auto-incrementing build number on every build (not just configure). Version scheme changed to `MAJOR.MINOR.PATCH.BUILD` (triplet + auto build number)
- **README.md**: Updated authentication description (removed legacy auth reference)
- **Removed deprecated keyword**: Removed `Maximum Concurrent Jobs` from generated FD client config (deprecated in Bareos)

### Fixed
- **Configure Success Detection**: Fixed Bareos JSON API response parsing — now correctly detects `result.configure.add` instead of searching for "created"/"success" text
- **Director Reload**: Wizard now sends `reload` command after successful `configure add client`; handles reload response (success/failure) and keeps dialog open
- **Dialog Auto-Close**: Wizard dialog no longer auto-closes after configure; stays open to show result
- **Unhandled JSON Response**: Suppressed spurious "Unhandled JSON response" warnings for configure and reload commands

## [0.1.0.4] - 2026-01-31

### Added
- **Running (R) Status Filter**: Added Running status checkbox to basic job filters
- **Canceled (A) Status Filter**: Added Canceled status checkbox to basic job filters
- **Job Preselection**: Run Job dialog now auto-selects FileSet, Pool, Storage, Client, and Level from job configuration when a job is selected
- **Job Configuration Access**: New `jobConfiguration()` method in BJobWidget to access stored job defaults

### Fixed
- **Run Command Duplication**: Fixed issue where "run job=run job=..." appeared in command (changed from Command::Run to Command::Custom)
- **Run Job Button State**: Button now correctly enabled/disabled based on job selection and data load state
- **Status Filter Completeness**: Job list filter now includes all job status types

### Changed
- **Status Filter Layout**: Reorganized status checkboxes to include Running and Canceled options
- **Filter Persistence**: Running and Canceled filter states are now saved to settings

## [0.1.0.3] - 2026-01-30

### Added
- **Run New Job Dialog**: Full-featured dialog for configuring and running backup jobs
  - Basic tab: Job, Client, Level, FileSet, Pool, Storage, Priority selection
  - Advanced tab: Scheduling (when), Bootstrap file, Replace options
  - Command preview with live updates
  - Estimate functionality
- **BVFS File Browser**: Browse backed up files using Bareos Virtual File System
  - Directory tree navigation
  - File list with details (name, size, type, modified)
  - Toggle between single job files and full restore chain
  - Selection for restore operations
- **Delete Job Options**: Choose between delete record only or purge with volume data
- **Dependent Job Detection**: Warning when deleting Full backups with dependent Incremental/Differential jobs
- **English Translation**: Complete English translation for Run New Job dialog

### Fixed
- **Job Log Switching**: Fixed rapid job selection causing wrong log to display
- **Job Log Display**: Fixed signal handler to use current row instead of checkbox selection
- **Folder Selection Count**: Fixed checkbox propagation for folder selection in BVFS browser
- **Restore Button Width**: Added dynamic width calculation using QFontMetrics
- **Run Job Button State**: Added minimum requirements check (job must be selected)

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

### Planned
- 🚧 **Add Job/JobDefs Wizard**: 5-page QWizard with RunScript editor (WIP, Build 274)
- 🔜 **Apply Resource Changes**: `configure add`/`configure update` from resource dialogs
- 🔜 **Live Job Monitoring**: Real-time progress bars for running jobs
- 🔜 **Job Control**: Start, stop, cancel jobs from UI
- 🔜 **Volume Management**: Label, mount, unmount volumes
- 🔜 **Enhanced Statistics**: Charts and graphs (job success rate, backup size trends)
- 🔜 **Job Templates**: Pre-configured backup job templates
- 🔜 **Email Notifications**: Alert system for job failures
- 🔜 **Dashboard**: Overview page with key metrics
- 🔜 **Schedule Calendar View**: Visual schedule representation
- 🔜 **Dynamic Language Switching**: Change language without restart
- 💤 **Bacula Support** *(future)*

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
- Add Job/JobDefs Wizard not yet runtime-tested against live Director

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
