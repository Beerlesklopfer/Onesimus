# Onesimus - Development Roadmap

## Priority 1: Foundation (CRITICAL) ✓ COMPLETE
*Database and core architecture*

### 1.1 Database Schema ✓
- [x] Create SQL schema generation scripts in Qt resources
- [x] Create SQL migration scripts in Qt resources
- [x] Add resources to Qt .qrc file
- [x] Design SQLite database schema with versioning support
- [x] Design unified settings table with history
- [x] Define tables for Directors, Clients, Storages
- [x] Implement version tracking table
- [x] Document database schema

### 1.2 Password Security ✓
- [x] Research Bareos password hashing (MD5 vs alternatives)
- [x] Check Bareos/Bacula documentation for hash methods
- [x] Implement secure password hashing (MD5 compatible with Bareos/Bacula)
- [x] Add password validation functions
- [x] Detect already-hashed passwords (32-char hex MD5) to avoid double-hashing

### 1.3 Migration System ✓
- [x] Implement database migration framework
- [x] Add version detection and automatic migration
- [x] Load and execute SQL scripts from Qt resources
- [x] SQL parser handles BEGIN...END blocks (triggers)
- [x] SQL parser handles -- comments with apostrophes
- [x] Test migration from v1 to v5 schemas
- [x] Preset database in connections wizard (BMainWindow initializes BDatabase, passes to wizard)

### 1.4 Release Preparation
- [ ] **BEFORE RELEASE**: Reset database schema to v1 (consolidate v1-v5 into single v1_initial.sql)
- [ ] Remove development migrations (v2, v3, v4, v5) before first release
- [ ] Ensure LATEST_VERSION = 1 in bdatabase.h for initial release
- [ ] Document: Future releases will use v2, v3, etc. for migrations

---

## Priority 2: Director Management (CORE FEATURE)
*Director connection, import, and management*

### 2.0 Application Scenarios

#### A) First Run
- [ ] No local DB exists → Create database, run migrations
- [ ] Show welcome/setup wizard
- [ ] Offer: Connect to Director OR Import config files
- [ ] Guide user through initial setup

#### B) Director Online → Sync
- [ ] Connect to live Director via API
- [ ] Fetch current configuration (resources, versions)
- [ ] Compare with local DB version
- [ ] Sync/update local cache (Director is source of truth)
- [ ] Handle version mismatch gracefully

#### C) Director Offline → Online / New Config
- [ ] Previously saved Director reconnects
- [ ] Import new config from files (ZIP/directory)
- [ ] Validate configuration before storing
- [ ] Update local DB with new/changed resources

#### D) Config Conflict/Drift Detection
- [ ] Detect when local DB differs from live Director
- [ ] Show diff/comparison dialog
- [ ] Let user choose: Accept remote, Keep local, Merge
- [ ] Log conflict resolution for audit

#### E) Export/Backup
- [ ] Export current config to files (ZIP archive)
- [ ] Include all Director resources
- [ ] Option to include passwords (encrypted) or exclude
- [ ] Timestamp and version metadata

#### F) Director Removal
- [ ] Delete Director profile from local DB
- [ ] Cascade delete related resources (Consoles, cached data)
- [ ] Confirm dialog with impact summary
- [ ] Option to export before deletion

#### G) Multi-Director Switch
- [ ] Switch active Director while others remain cached
- [ ] Disconnect current, connect to selected
- [ ] Preserve UI state per Director
- [ ] Quick-switch dropdown/menu

---

### 2.1 Director Import

#### 2.1.1 Config Parser (Backend) ✓
- [x] Create BConfigParser class for Bareos/Bacula config files
- [x] Parse resource blocks (Director, Console, Client, Job, etc.)
- [x] Handle key-value pairs and nested blocks
- [x] Handle comments and quoted strings
- [x] Implement ZIP extraction using QTemporaryDir with automatic cleanup
- [x] Parse entire config directory tree recursively
- [x] Create BDirectiveSchema class for loading JSON directive schemas
- [x] Validate parsed resources against directive JSON schemas (schema-driven subset validation)
- [x] Messages directive schema (messages.json) with German translations

#### 2.1.2 Config Import Dialog (GUI) ✓
- [x] Create BConfigImportDialog widget (tabbed dialog)
- [x] Source selection panel (browse directory or ZIP file)
- [x] Resource widgets for each type (Director, Console, Client, Job, Storage, FileSet, Pool, Schedule, Messages, Catalog)
- [x] Director selection combo box (from parsed Directors)
- [x] Console selection combo box (from parsed Consoles)
- [x] Connection preview panel (shows selected Director/Console details)
- [x] Progress bar and status feedback during parsing
- [x] Remember last opened directory in resource dialog (QSettings)
- [x] Validation feedback (warnings/errors for missing required fields)
- [x] Address and port input fields for connection (user specifies remote host)

#### 2.1.3 Menu Integration ✓
- [x] Add "Import Config..." action to Tools menu in BMainWindow
- [x] Connect dialog to main window
- [x] Handle dialog result and create connection profile
- [x] Option to connect immediately after import

#### 2.1.4 Database Storage
- [ ] Integrate with BDirectorModel to store imported connection
- [x] Extract Console passwords (MD5 hash with [md5] prefix support)
- [ ] Store TLS settings if present in config
- [ ] Link imported Console to Director

### 2.2 Multi-Director Support
- [ ] Support multiple Directors in database
- [ ] Allow switching between Directors
- [ ] Implement Director selection UI
- [ ] Handle concurrent connections

### 2.3 Director Export
- [ ] Implement Director export to ZIP
- [ ] Include all settings and metadata
- [ ] Implement import from ZIP

---

## Priority 3: Client Management (ESSENTIAL)
*Client/FD configuration*

### 3.1 Client Import
- [x] Implement Client and FD config import
- [X] Parse Client resource definitions
- [X] Link Clients to Directors

### 3.2 Client Wizard (Quick Win) ✓
- [x] Create fast Client wizard (BNewClientWizard — 3 pages: Director Info, Settings, Preview & Export)
- [x] Implement quick setup with defaults (auto-populate Director info from connection)
- [x] Add validation (schema-driven subset validation against directive schemas)
- [x] Generate FD-side config files (director.conf, myself.conf, messages.conf) with TLS directives
- [x] Generate Director-side client.conf
- [x] Execute `configure add client` on Director with automatic reload
- [x] ZIP export for client deployment
- [x] Preview page uses BResourceWidget for structured resource display
- [x] Context menu "Export Configuration..." on client table (Save As dialog)
- [x] ACL permission check — hides configure checkbox when console lacks access
- [x] 30-second timeout with retry dialog for configure commands
- [x] Removed deprecated `Maximum Concurrent Jobs` from generated FD config

### 3.3 Client Dialog (Detailed) ✓ (partial)
- [x] Client details dialog with Settings tab (BResourceWidget with schema-driven Edit dialog)
- [x] Dynamic resource editing via BResourceDialog (schema-driven form fields from JSON)
- [x] Settings tab shows Director-side Client resource; exports include all FD + Director resources
- [x] Model as single source of truth (`BClientsModel::clientResources()`, `enrichedClient()`)
- [x] Enrichment logic moved from widget to model (`enrichWithJobData`, `enrichWithShowClientData`, `setAllClients`)
- [ ] Full client configuration editing with Director API integration
- [x] Retrieve actual passwords from Director for export (fixed: no longer uses "CHANGE_ME" placeholder)
- [x] Client online/offline verification via `status client=<name>` active probe (replaces timestamp heuristic)
- [x] Toggle TLS certificate file fields (CA, Cert, Key) when TLS Require is checked in settings (via `visible_when` in directive schema + BResourceForm)

### 3.4 Client Export ✓
- [x] Export to ZIP with FD settings (from New Client Wizard)
- [x] Export to .conf from client table context menu
- [x] Export as ZIP from client table context menu (Bareos directory structure)
- [x] Import from ZIP

---

## Priority 4: Storage Management (ESSENTIAL)
*Storage/SD configuration*

### 4.1 Storage Import
- [ ] Implement Storage and SD config import
- [ ] Parse Storage resource definitions
- [ ] Link Storages to Directors

### 4.2 Storage Wizard (Quick Win)
- [ ] Create fast Storage wizard
- [ ] Quick setup for common types
- [ ] Add validation

### 4.3 Storage Dialog (Detailed)
- [ ] Create detailed Storage dialog
- [ ] Autochanger, devices, etc.
- [ ] Tooltips and help

### 4.4 Storage Export
- [ ] Export to ZIP with SD settings
- [ ] Import from ZIP

---

## Priority 4a: Job Management (ESSENTIAL)
*Job resource configuration*

### 4a.1 Job Dialog-Wizard
- [x] Add "Add Job..." menu item in Jobs menu (placeholder with "Not implemented yet" message)
- [ ] Create Job dialog-wizard (schema-driven form from job.json)
- [ ] Job type selection (Backup, Restore, Verify, Admin, Copy, Migrate)
- [ ] Client selection (from Director's client list)
- [ ] FileSet selection (from Director's fileset list)
- [ ] Storage selection (from Director's storage list)
- [ ] Pool selection (from Director's pool list)
- [ ] Schedule selection (from Director's schedule list)
- [ ] Messages resource selection
- [ ] Preview & export generated job config
- [ ] Execute `configure add job` on Director with reload

---

## Priority 4b: FileSet Management (ESSENTIAL)
*FileSet resource configuration*

### 4b.0 Generic Editable List Components ✓ COMPLETE
- [x] Create BEditableListModel with undo/redo support (QAbstractListModel + QUndoStack)
- [x] Create BUndoCommands (Add/Remove/Edit/Move with merge support for continuous typing)
- [x] Create BEditableListDelegate with inline editing, delete button, optional browse button
- [x] Create BEditableListWidget with context menu, Delete key handling, multi-select
- [x] Validation callback support for list items
- [x] Reusable for all resources with list-type directives (not just FileSet)
- Files: `include/config/beditablelistmodel.h`, `beditablelistdelegate.h`, `beditablelistwidget.h`, `bundocommands.h`

### 4b.1 FileSet Dialog-Wizard
- [x] Add "Add FileSet..." menu item in Jobs menu
- [x] Add "Edit FileSet..." menu item in Jobs menu
- [x] Create BFileSetDialog (tabbed dialog: Basic, Include, Exclude)
- [x] Create BIncludeOptionsForm (schema-driven form for Include Options)
- [x] Export FileSet to ZIP (with proper directory structure) and .conf file
- [x] FileSet templates stored in database (v5 migration: fileset_templates table)
- [x] Import built-in templates from Qt resources on first run

### 4b.2 Unified FileSet Wizard ✓ COMPLETE
- [x] Create unified BFileSetWizard (replaces separate wizard and dialog)
- [x] BFileSetDocument as pure data model (no Director reference)
- [x] BIncludeBlockWidget for Include paths + options + nested exclude
- [x] Integrate BEditableListWidget for path editing with undo/redo
- [x] Edit mode: skip template selection, load existing FileSet
- [x] Edit mode: FileSet selection combobox (fetch from Director)
- [x] Options form: support for string_list (fstype), string, integer types
- [x] Compact mode for list widgets in options form
- [ ] Plugin directive support
- [x] Preview & export generated fileset config
- [x] Execute `configure add fileset` on Director with reload

### 4b.3 FileSet Remote Browse Plugin (FUTURE)
- [ ] Write bareos-fd plugin to use mlocate/plocate for FileSet path overview
- [ ] Plugin provides file listing from client filesystem index
- [ ] Enables remote browsing of client paths when creating FileSets
- [ ] Reduces need to manually type paths

---

## Priority 5: Wizard Refactoring (INTEGRATION)
*Update existing wizards to use new database*

### 5.1 Update Import Wizard
- [ ] Refactor wizard to use new Director approach
- [ ] Update UI for new database structure
- [ ] Migrate wizard logic to new architecture
- [ ] Test with new database backend

---

## Priority 6: UI Enhancements (NICE TO HAVE)
*Polish and additional features*

### 6.1 Backup System Selection
- [ ] Add UI for Bareos/Bacula selection (BOTH mode)
- [ ] Settings dialog with dropdown
- [ ] Runtime switching between systems
- [ ] Save selection in QSettings

### 6.2 Console Enhancements
- [ ] Add colorized console using ftxui (similar to bconsole)
- [ ] Implement command highlighting
- [ ] Add output syntax highlighting for different message types

### 6.3 Messages & Job Log UI ✓ DONE
- [x] Create BMessagesWidget for Director messages display
- [x] Integrate MessagesWidget into BJobWidget (tabbed lower panel with Job Log)
- [x] Add settings for message timer update interval (messagesPollInterval in BSettings)
- [x] Add settings for messages history retention (messagesMaxHistory in BSettings)
- [x] Add message polling timer with configurable interval
- [x] Route messages JSON response to MessagesWidget via BJobWidget
- [x] Move messages widget from MainWindow to BJobWidget
- [x] Implement job/director message type detection (joblog → JobLog widget; messages → MessagesWidget)
- [x] Add job log history with dropdown selection (max 10 logs), search filter, and copy functionality

### 6.4 TLS-PSK Authentication Fixes ✓ DONE
- [x] Fix "ssl/tls alert bad record mac" error during TLS-PSK handshake
- [x] Add proper socket error handling for authentication failures
- [x] Improve PSK debug logging for troubleshooting
- [x] Detect already-hashed passwords (32-char hex MD5) to avoid double-hashing

### 6.5 Restore Wizard
- [ ] Create Restore Wizard dialog (separate from BJobFilesWidget file browser)
- [ ] Select target client for restore
- [ ] Select destination directory (restore where)
- [ ] BVFS file selection with checkboxes (using custom QAbstractItemModel)
- [ ] Replace policy selection (always, never, if newer, if older)
- [ ] Command preview before execution
- [ ] Execute restore via BVFS restore table + restore command

### 6.6 Connection Wizard Improvements ✓ DONE
- [x] Add ImportConsoleSelectionPage for ZIP import (Director/Console selection with ACL pre-fill)
- [x] Fix empty console combo in ConsoleSetupPage (use `.consoles` dot-command)
- [x] Fix console details parsing (`show console=<name>` returns object, not array)
- [x] Consolidate duplicate error dialogs on startup into one
- [x] Centralize Director query API: `queryConsoles()`, `queryShowConsole()`, `queryConfigureAddConsole()`
- [x] Add typed signals: `consolesResult(QJsonArray)`, `showConsoleResult(QString, QJsonObject)`, `configureResult(bool, QString)`
- [x] Forward query methods and signals through BDirector wrapper
- [x] Extract `buildTLSConfig()` and `createConnectedDirector()` helpers (deduplicate TLS boilerplate)
- [x] Add `*UserAgent*` hint: signal admin to create personalized Console resource
- [x] Remove all `sendRawCommand()` usage from production code
- [x] Simplify wizard flow: Test → ProfileName (skip ConsoleSetupPage)
- [x] Fix ProfileNamePage nextId() to return -1 (end wizard properly)
- [x] Hide "Use template from existing" option when no saved profiles exist
- [x] Handle `[md5]` password prefix in config files (strip before sending to Director)
- [x] Add SVG icon set with transparent backgrounds for menus and toolbar
- [x] Add Windows .ico file and resource file for executable icon

### 6.7 Console ACL Enforcement
- [ ] Query current console's Profile and ACLs after authentication (`show console=<name>`)
- [ ] Parse ACL fields: CommandACL, JobACL, ClientACL, StorageACL, ScheduleACL, PoolACL, FileSetACL, CatalogACL
- [ ] Store current console ACLs in BDirector (accessible via getter methods)
- [ ] Implement ACL checking helper: `bool BDirector::hasPermission(AclType type, const QString &resourceName)`
- [ ] Disable/hide menu items based on CommandACL (e.g., hide "Run Job" if `run` not in CommandACL)
- [ ] Disable/hide resource actions based on resource ACLs (e.g., hide client if not in ClientACL)
- [ ] Filter resource lists based on ACLs (only show permitted Jobs, Clients, etc.)
- [ ] Show "Permission denied" tooltip on disabled actions
- [ ] Handle `*all*` ACL wildcard (grants full access)
- [ ] Handle `!resource` negative ACL patterns (explicit deny)
- [ ] Update UI dynamically when switching consoles/profiles

---

## Suggested Implementation Order

**Sprint 1: Database Foundation** ✓ COMPLETE
1. ✓ SQL resources setup
2. ✓ Database schema implementation
3. ✓ Password hashing research
4. ✓ Migration system (v1-v5 with proper SQL parsing)

**Sprint 2: Director Core**
5. Application scenarios (A-G) - state machine / flow logic
6. Director import (config parser, ZIP handling)
7. Multi-Director support
8. Config sync and conflict detection
9. Basic Director UI

**Sprint 3: Client Management**
10. Client import
11. Client wizard (fast)
12. Client dialog (detailed)

**Sprint 4: Storage Management**
13. Storage import
14. Storage wizard (fast)
15. Storage dialog (detailed)

**Sprint 5: Integration & Polish**
16. Wizard refactoring
17. Export/Import functionality
18. System selection UI
19. Testing and bug fixes

---

## Quick Wins
1. ✓ SQL resources in Qt (.qrc) - DONE
2. ✓ Database schema creation from resources - DONE
3. ✓ Database migrations working (v1-v5) - DONE
4. Simple Director wizard working
5. First successful Director connection test

---

## Blockers to Watch For
- **Password hashing compatibility** - ✓ Implemented (MD5 compatible with Bareos/Bacula)
- **Config file parsing** - ✓ BConfigParser with ZIP extraction, tabbed import dialog complete
- **Directive validation** - ✓ BDirectiveSchema loads JSON schemas from Qt resources
- **TLS/SSL handling** - Especially on Windows with Schannel
- **Database migrations** - ✓ Robust SQL parser handles triggers and comments
- **Auto-connect** - ✓ Fixed: password field check changed to passwordHash (deprecated field was always empty)
- **Status client JSON output** - ⚠ Bareos limitation: `status client` doesn't return proper JSON (GitHub Issue #2325). FD can't output JSON, Director can't parse JSON from FD. **Status probing DISABLED** — now uses timestamp heuristic (lastconnection within 24h = Online)

---

## Key Features Summary

**Database**
- SQLite with database versioning
- Schema migration support (v1-v5)

**Security**
- Password hashing (MD5 compatible with Bareos/Bacula)
- [md5] prefix support for already-hashed passwords
- Secure password storage
- TLS-PSK authentication

**Configuration Management**
- Multiple Directors supported (planned)
- Config parser with ZIP extraction
- Tabbed import dialog with resource widgets
- Director/Client/Storage + Daemons

**User Interface**
- Wizard (fast) for quick setup
- Dialog (exact) for detailed configuration
- Messages and Job Log widgets
- Connection wizard with ZIP import support

**Import/Export**
- Config file import (directory or ZIP)
- ZIP export for Client configurations
- ZIP export for Director configurations (planned)
- ZIP export for Storage configurations (planned)

**Packaging**
- Debian .deb package with automatic dependency detection
- Linux desktop entry and icon integration
- NSIS Windows installer (planned)
- AppImage support

---

## Development Notes

### Database Versioning Strategy
- Each schema change increments version number
- Migrations are applied sequentially
- Backward compatibility maintained where possible
- Version table tracks current schema version

### Password Security
- MD5 hashing compatible with Bareos/Bacula
- [md5] prefix support for already-hashed passwords
- Secure connection handling via TLS-PSK

### Configuration Parsing
- BConfigParser supports standard Bareos/Bacula config syntax
- Handles includes and nested configurations
- Validates configuration before import
- BDirectiveSchema loads JSON schemas from Qt resources

### Export Format
- ZIP container for portability
- Include version metadata
- JSON or XML for configuration data
- Support for incremental exports

---

*Last Updated: 2026-02-05*
*Priority: Focus on getting Director management working first, then expand to Client/Storage*
*Note: Using ninja build system. Config import dialog with tabbed resource widgets implemented.*
*Files: src/config/ (parser, schema), src/director/ (import dialog, resource widgets)*
