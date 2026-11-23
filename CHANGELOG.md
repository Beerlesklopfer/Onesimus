# Changelog

All notable changes to Onesimus will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.3.0] - 2026-11-23

### Added
- **Onesimus Director over gRPC**: typed transport to the Rust Director (`odir`) with mDNS
  discovery; functions the connected Director does not offer are named as unavailable.
- **Schedules**: grid editor by weekday and hour (brushes, undo, Description), a wizard of a
  name page plus the grid, save/change/delete over gRPC, copy/rename/move of one job.
- **Schedule timeline**: measured job history instead of fixed durations, predicted waiting
  time calibrated against the catalog (device writers, SD retry, start overhead), a live
  overlay with the Director's times, a legend, day view of who waits for whom, calendar.
- **Stagger**: proposed later start times for waiting jobs, on the quarter hour, with
  constraints per Director profile, applied after one confirmation.
- **ACL matrix**: the consoles' ACLs and profiles in one matrix; catalog ACLs, new consoles
  and new profiles are changed as proposed configuration text.
- **Edit Job and Edit JobDefs** (Jobs menu) write over gRPC; JobDefs can be deleted.
- **3-2-1 Verifier** (Tools menu): checks the 3-2-1 rule per backup job.
- **Prune and purge** with a preview on the Onesimus Director.
- **Status bar**: free-tier usage and the end of the annual licence.
- **Job log**: one append-only model with a live log in all three views.
- **Languages**: View > Language; the system language by default; translations into German,
  Spanish, French, Italian and Russian (a few newer texts are not translated yet).
- **Interface**: the desktop font as base with an interface zoom; status chips in the tables;
  volume table with "reusable from"; resizable, sortable client columns.
- **Windows**: MSI installer with features for the GUI, oconsole and the ofd service, plus a
  portable ZIP. This release's MSI holds the GUI only.
- **macOS**: `Onesimus.app` for Apple Silicon (arm64, macOS 12 or later) in
  `Onesimus-<version>-macos-arm64.dmg`, with the Qt frameworks inside, signed with a Developer ID
  certificate (hardened runtime), notarised by Apple and stapled.
- **CI**: Linux and Windows builds with tests, a macOS app bundle and dmg; a release workflow for
  .deb, AppImage, MSI, ZIP and the macOS dmg.
- `SECURITY.md`: how to report a vulnerability; from v1.0.0 each version gets security fixes
  for 2 years.

### Changed
- Licence: AGPL-3.0; own code also under any later version (AGPL-3.0-or-later), files that follow
  the Bareos source AGPL-3.0-only (see `LICENSING.md`).
- Byte counts in the jobs and clients tables, job and client details, job statistics, restore
  wizard and cleanup dialog are shown in binary units with IEC names (KiB, MiB, GiB, TiB) and one
  decimal, like the storage view; before they were labelled KB/MB/GB with two decimals. The
  decimal separator follows the application language ("1,5 GiB" in German), also in the storage
  view and the free-tier usage.
- The Bacula backend is deprecated; the BOTH build option is removed.
- **Build scripts became deploy scripts** (modelled on forgeiec-studio):
  `build.sh`/`build.fish` → `deploy.sh`/`deploy.fish`, `build-windows.ps1` → `deploy.ps1`,
  `run-build.bat`/`rebuild.bat` → `deploy.bat`. Verbs: `all` (default: build + deploy into
  `/usr/local` with sudo per step; on Windows build + windeployqt), `configure`, `build`, `rebuild`,
  `run`, `test`, `install`, `package`, `appimage` (Linux), `installer`/`zip`/`msi` (Windows),
  `status`, `clean`, `help`. All former flags keep their meaning; `--dry-run`/`-DryRun`
  prints every command instead of running it. `./deploy.sh build` is the former `./build.sh`.
- Windows helper scripts moved to `scripts/windows/` (`check-qt6.ps1`, `package-windows.ps1`, which
  `deploy.ps1 zip` calls); the generated `qt6-quickfix.ps1`, `examples/example_usage.cpp`,
  `resources/icons/create_ico.bat` and `external/README.md` are removed.
- CTest also registers the live-Director tests `bareosauth_live_test` and `director_live_test`
  (label `live-director`); they are skipped unless `ONESIMUS_LIVE_DIRECTOR` names a test Director.
- A build no longer writes into the source tree (#7): `version.h` and the build number are generated
  into `<build>/generated/`, `include/version.h` and `.build_number` are no longer tracked, and an
  unchanged build compiles nothing. The build number is the CI run number, else the commit count;
  `-DONESIMUS_INCREMENT_BUILD=ON` counts it up per build. About shows the source (commit) date.
- Build: one CMake minimum, 3.25 (`CMakePresets.json` is a version 6 presets file); the Windows
  presets use Ninja and `build\` like `deploy.ps1`; the build docs list QtGrpc/QtProtobuf and `protoc`.
- Build: compiler warnings (`-Wall -Wextra`, MSVC `/W4`) for the Onesimus targets, option
  `ONESIMUS_WARNINGS` (default ON; not for OpenSSL or the generated gRPC code); the cheap warnings
  they showed are fixed.
- Build: the application sources are compiled once into the static library `onesimus_core`, which the
  program and the app-wide tests link; `bareosauth.cpp` is compiled once (`bareosauth_lib`).
- Build: `Qt6::GuiPrivate` is linked on every platform (needed from Qt 6.9 on; Linux and macOS used
  `Qt6Gui_PRIVATE_INCLUDE_DIRS`).
- Build: the bundled OpenSSL is the default of CMake as in the deploy scripts (before, CMake took a
  system OpenSSL when it found one); it is built `no-shared` (DLLs on Windows only, for Qt's TLS
  plugin), out of source, into `lib/`, and its DLLs are no longer copied into the Qt installation.
  An empty `OPENSSL_INSTALL_DIR` defaults to `<build>/openssl-install` again.

### Security
- **BNET: the Director must prove the password.** The console refuses a Director whose answer to
  its CRAM-MD5 challenge is wrong, that greets without answering it, or that sends the console's
  own challenge back; before, these were accepted. Bareos 25 and the Onesimus Director always send
  this answer (checked in their source and against onesimus-sim).
- **BNET: TLS requirements are kept.** A profile that requires TLS never falls back to clear text,
  and a Director that requires TLS is refused before the password proof is sent in clear text.
  TLS-PSK uses only PSK-* cipher suites, also from a configured cipher list.
- **Certificate TLS with verification** needs a readable CA file, trusts only that CA (not the
  system store), and checks that the Director's certificate names the host connected to, or, with
  TLS Allowed CN set, that its common name is listed. Before, a certificate from any trusted CA was
  accepted for any host. Windows profiles now pass the CA file and the PKCS#12 client certificate.
  `--tls` with certificate options needs `--tls-ca`.
- **gRPC: the certificate pin applies to the connection that carries the login.** Before, it was
  checked on a separate pre-flight connection and the channel itself ran without verification.
- Each refused connection says why (wrong password, Director without the password, certificate
  not matching the pin, host name not in the certificate, CA file unreadable).
- Certificate generator: private keys are written owner-only (0600, an existing file is
  tightened first); the CA key is kept only encrypted with a passphrase (PKCS#8, AES-256) or not
  at all; a PFX is written only with a password; client certificates carry CA:FALSE, key usage,
  clientAuth and the common name as DNS name; serial numbers are 159 random bits instead of the
  time of day.
- New-client dialog: the File Daemon password comes from the system's secure random source.
- ZIP import (connection wizard and import dialog): configuration files are read in memory with
  limits (16 MiB archive, 1000 entries, 1 MiB per file, 16 MiB together); symbolic links and paths
  leaving the archive are refused; no external `unzip`. The "Copy Debug Info" button, which put
  the start of passwords on the clipboard, is removed.
- One quoting for console commands and generated configuration: names, passwords and paths
  with a blank, `"` or `\` stay one value (the ACL matrix dropped backslashes, the schedule wizard
  and the new-client dialog did not escape); a value with a line break is not sent; values that
  `configure add` cannot write (`;`, `@`, `"`, `\`) are refused before sending.
- **Passwords of the connection profiles in the system keychain.** The password hash (all that is
  needed to log in) and the PFX password are kept in the Secret Service (GNOME Keyring, KWallet) on
  Linux, the login keychain on macOS and the Credential Manager on Windows, no longer in the
  settings file. At the first start, secrets an earlier version stored there are moved into the
  keychain and removed from the file. Where no keychain can be used they stay in the file, which is
  then made readable by the owner only, and a warning says so at every start (bsettingsstore_test).
- Test tree: the console password of the test configuration is an obvious dummy, and the sudoers
  file that gave passwordless root `cp`, `chown` and `kill` to a named user is removed; the
  live-Director scripts need no root (`test/README.md`).
- Test tree: the test PKI (CA, server and client keys) is no longer committed; `test/make-test-pki.sh`
  generates it on first use of the live-Director scripts, and the stray `.srl` serial files are gone.

### Fixed
- **MSI:** the installer put the program in as `!(bindpath.Gui)onesimus.exe` instead of `onesimus.exe`
  (the WiX `File` element had no `Name`); found by the new notices check of the MSI.
- The GUI no longer freezes after connecting: the wait prediction runs in a worker thread and
  waits for the job history (measured with the simulator, 80 clients and 120 jobs: a 30 s stall
  of the GUI thread before, none above 50 ms after connecting).
- Client table: the job list (one page of jobs) no longer overwrites a client's job count and
  backed-up bytes reported by "list backups client=".
- **Restore wizard, restore command** (P0-4): client names, the restore path and the Restore
  Job are quoted, so a path with spaces or quotes stays one value; the client of the backup is
  sent as `client=` and the target client as `restoreclient=`; the Restore Job is chosen on the
  options page and sent as `restorejob=`, so the Director no longer asks which one to use.
- **Restore wizard, waiting and cleanup** (P0-4): after 60 seconds without an answer the wizard
  no longer reports a failed restore; it says the restore may still start, points to the job
  list and keeps waiting. The restore table is dropped (`.bvfs_cleanup`) on every way out of
  the wizard, also after a failure.
- **Restore wizard, page 1** (P0-5): Client, FileSet and Before Date are hidden; they did not
  change which backups were restored.
- **Late replies** (P0-15): the reply to a request of a closed restore wizard or file browser is
  no longer taken by the next one; the client details dialog takes only its own client's job
  list.
- **File browser and restore selection** (P0-16): directory listings are read in pages of 1000
  until they are complete instead of stopping at the Director's default of 2000 entries; the
  root directory is asked for by path instead of assuming PathId 1; checking a directory no
  longer loads its whole subtree, and a partly selected directory restores every entry that was
  not unchecked.
- FileSet wizard: the generated configuration is accepted by `bareos-dir -t` (Bareos 25.1) and
  `odir -t` for all 22 templates and for edited FileSets: patterns to leave out go into an Options
  block with `Exclude = yes` instead of an Exclude block inside Options, empty Regex/Wild lines,
  VSS and Exclude Dir Containing inside Options and Enable Snapshot are no longer written, and
  the templates no longer use "Files", `Accurate = yes` or `Verify = yes`.
- FileSet wizard: loading and writing a FileSet keeps every directive (several Options blocks,
  exclude regexes, Plugin, Enable VSS, Ignore FileSet Changes, unknown directives); the options
  form changes only the option that was changed; names with blanks are kept.
- PFX converter: it writes the PFX file (with the CA certificate, if given), reads it back with
  the password, and otherwise says why; before it reported success without writing a file.
- "Edit..." of a resource list is offered only for Job and JobDefs, which are written to the
  Director; for the other types it only changed the list in the window and lost lists and nested
  blocks.
- **Long replies over BNET** (P1-17): a JSON reply split into several telegrams, also inside a
  UTF-8 character, keeps every character and every space (before, a split character became
  "\uFFFD" and spaces at the split were lost); the reply is read in time proportional to its size
  instead of being parsed again after every telegram (8 MiB in 1000-byte telegrams: each byte
  read once).
- **Disconnect over BNET** (P1-5): the console sends "quit" before it closes the connection;
  before, the farewell was written after the socket was closed.
- **Commands without an answer** (P1-5): a command the Director does not answer within 5 minutes
  is reported as failed, and its late reply is no longer taken for the reply of the next command.
- **Closing a connected window or program** (P1-19): the Director connection is torn down in its
  own thread; before, it was never deleted and its socket stayed open. The unused keep-alive
  timer created at every login is gone; every change of the connection state reaches the status
  bar.
- **Failed commands** (P1-20): a reply counts as failed only when the Director reports an error
  (the JSON-RPC error object, or a Bareos error line in text mode), no longer when a job log, a
  file name or another value contains the word "error".
- **Cancel in the job list toolbar** (P1-20): cancels every selected job instead of one picked
  at random, and reports the Director's answer for each instead of "Job N was cancelled" before
  any answer.
- **Headless mode** (P1-29): `--headless` sends the command (`-e` or the arguments) and the
  commands of `--script` (one per line), prints each reply (the Director's JSON) and exits with
  0 (all succeeded), 1 (a command failed) or 2 (no session); it needs no display and needs
  `--host`. Before, the command was never sent. The unused `--config` option is removed.
- Settings dialog: Cancel reverts everything, also added, deleted, duplicated and imported
  profiles and the level and status bar colours; they are written when the settings are saved.
  Editing a profile and selecting another no longer renames the other row. Reset asks whether the
  connection profiles go as well and keeps them by default. "Edit" and "Save Profile", which did
  nothing of their own, are gone (bsettingsdialog_test).
- Export and import of the connection profiles work on the application's settings: export wrote
  column widths and no profiles, import reported success and added nothing. The export asks
  whether the password hashes go into the file; import also reads files of the old export
  (bsettingsstore_test, bsettingsdialog_test).
- Jobs tab: the job and client filter lists keep every configured job and client instead of the
  names on the current page, so the Run dialog and the job wizard offer all of them; the selected
  filter is kept by name, not by position (bjobfilter_test).
- Reconnect connects also when auto-connect at start is off. Disconnect cancels a connection
  attempt, the wait cursor no longer stays after an error or a certificate question, and every
  connection gets its TLS settings from the profile alone, without CA or certificate files of the
  profile used before (bconnecthelpers_test).
- Clients tab: with the Onesimus Director over gRPC, the status column shows Online or Offline from
  the Director's client events (connected, disconnected, check result) and returns to the age of
  the last backup when no event holds (bclientsmodel_test). "Backups" and "Total Data" come from
  "list backups client=" only and keep their values during a refresh instead of falling back to the
  figures of the current job page.
- Run dialog: Estimate shows the files and bytes the Director estimates; the unused Replace options
  are gone (brunjobcommand_test). Edit > Copy, Select All, Clear Selection work in the Clients tab,
  and Edit > Find puts the focus into the name filter of the Jobs or Clients tab.
- Settings: "Show tooltips" and the maximum log size take effect; the connection timeout, animation
  and compact mode options, which nothing read, are gone. Directive values of the new client dialog
  are checked as yes/no, size and time, not only as integers (bdirectiveschema_test).
- Dark mode (issue #6): what the style sheet leaves out is drawn by the Fusion style from a palette
  with the theme's colours instead of the desktop's; a "System" theme follows the desktop's light or
  dark scheme (Settings > Appearance); combo and spin boxes get arrow images (btheme_test).
- Text contrast in both themes (issue #6): state, hint, warning and error texts, placeholders,
  group box titles, the status bar, the messages list and the logs of the certificate generator,
  cleanup, connection and restore dialogs take colours of the theme with at least 4.5:1 contrast
  instead of fixed colours that were dark on dark (black or dark grey log lines) or pale on light
  (bthemecontrast_test measures every theme from the palette and from a rendering).
- **Console commands built from values** (P1-24): job ids, file ids and limits are sent only as
  numbers, resource names (also `.defaults`, and the client of "prune jobs" in the job list) are
  quoted, and the argument strings of restore, show, label, configure and the `.bvfs` commands
  are refused when a value in them is not quoted; nothing that contains a newline is sent. Job,
  file and directory ids from the Director's BVFS replies are taken only when they are numbers,
  and a restore table only as `b2` and digits, as the Director checks them.
- **Configuration import reads files as Bareos does** (P1-25): `FD Port`, `FDPort` and `fdport`
  are one directive, `;` ends a directive (before, the rest of the resource was dropped),
  repeated blocks (several `ipv4` addresses, several `Options`) are kept as a list instead of
  the last one, a backslash in a quoted string takes the next character as it is, and quoted
  strings in a row are joined. `@include` is followed when a single file is imported (absolute
  paths and wildcards; a command after `@|` is never run), otherwise it is reported instead of
  ending the import; include loops, nesting deeper than 32 levels and more than 16 MiB of text
  are refused.
- CRAM-MD5 login: the buffer of each answer is freed (memory leak found by LeakSanitizer).
- **Third-party notices in every package** (P2-7): the `.deb`, the AppImage, the Windows MSI and
  ZIP and the macOS dmg carry `THIRD-PARTY-NOTICES.md` (Qt, Qt gRPC, OpenSSL, Feather icons, the
  libraries bundled in the AppImage) with the licence texts in `LICENSES/` and the AGPL; the
  `.deb` also a Debian `copyright` file. The AppImage adds the licence texts its bundled Debian
  libraries refer to and a list of their package versions. CI fails a package without them.
- **Source archives with every release** (P2-7): Onesimus with its OpenSSL and proto submodules,
  the Debian source packages of all libraries in the AppImage and the Qt sources of the Windows
  and macOS packages are attached to the GitHub release; the release is not drafted without them.
- About dialog: a "Third-party licences..." button shows the notices and licence texts.
- The Bareos and Bacula logos are no longer built into the program or kept in the source tree
  (they were not shown anywhere).

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
- **Duration Statistics Panel**: detailed stats when clicking a job in the Gantt view
  - Min / Avg / Max duration with formatted display (Xh Ym, Xm Ys)
  - Trend indicator: faster (green) or slower (orange/red) compared to history
  - Last 5 runs table with date, level, and duration
  - Enhanced Gantt tooltips showing min/max duration and sample count
- **Schedule Wizard**: schema-driven 3-page QWizard for creating new schedules
  - BScheduleRunDialog: editor for single Run directive (level, days, time, pool, priority)
  - BScheduleRunEditor: table with Add/Edit/Remove/Duplicate operations
  - BScheduleBasicsPage: name, description, enabled, 5 predefined templates from `schedule.json`
  - BScheduleRunsPage: embedded run editor with live preview
  - BSchedulePreviewPage: config text, `configure add` command, copy/execute buttons
  - All dropdown values loaded from `schedule.json` metadata (no hardcoded values)
  - Menu integration: Schedules → Add Schedule... (Ctrl+Shift+S)
  - Wizards toolbar section with Add Job and Add Schedule buttons
- **BDirectiveSchema**: extended to parse schedule metadata sections
  - `run_directive_syntax`: level values, pool/storage format, priority default
  - `common_schedules`: 5 predefined schedule templates
  - `time_specification_reference`: day/week/month values, time format
  - Convenience methods: `scheduleLevelValues()`, `dayOfWeekValues()`, `weekOfMonthValues()`, `monthValues()`

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
- **Author**: Joerg Bernau <support@onesimus.io>
- **Project**: Onesimus
- **License**: AGPL-3.0 (own code AGPL-3.0-or-later, see `LICENSING.md`)

### Technologies
- [Qt Framework](https://www.qt.io/) - Cross-platform UI
- [OpenSSL](https://www.openssl.org/) - TLS/SSL Encryption
- [CMake](https://cmake.org/) - Build System

### Backup Systems
- [Bacula](https://www.bacula.org/) - Open Source Backup
- [Bareos](https://www.bareos.com/) - Bacula Fork with Enterprise Features

---

*"Managing backups should be simple, secure, and accessible in your language."*
