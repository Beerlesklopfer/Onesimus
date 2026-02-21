# Known Bugs and Issues

This document lists known bugs and issues in Onesimus.

## Severity Levels

- 🔴 **Critical**: System crashes, data loss, or complete feature failure
- 🟠 **High**: Major functionality broken, no workaround available
- 🟡 **Medium**: Significant issue with workaround available
- 🟢 **Low**: Minor issue, cosmetic problem, or enhancement request
- ⚪ **Trivial**: Very minor issue with minimal impact

## Current Issues

### Statistics Widget
- **Severity**: 🟡 Medium
- **Description**: verify values.
- **Workaround**: none
- **Status**: Under investigation

### Job Tree View - Table Update
- **Severity**: 🟠 High
- **Description**: When a job has been run the table is not being updated
- **Workaround**: none, maybe add a filter in basic filters 0> status
- **Status**: Under development

### Job Tree View - Functionality
- **Severity**: 🟡 Medium
- **Description**: The job tree view is not fully functional due to ongoing development.
- **Workaround**: Use the table view for job management.
- **Status**: Under development

### Job Log Panel Scrolling
- **Severity**: 🟡 Medium
- **Description**: The job log panel may become empty when scrolling to the bottom with multiple dialogs open.
- **Workaround**: Close other job dialogs before scrolling through the main log panel.
- **Status**: Under investigation

### BVFS File Browser
- **Severity**: 🟢 Low
- **Description**: Large directory trees may take time to load due to sequential API calls.
- **Workaround**: Be patient when browsing large backup sets.
- **Status**: Known limitation

### Add FileSet - Wizard/Detail Dialog Sync
- **Severity**: 🟡 Medium
- **Description**: Synchronization between the FileSet wizard and detail dialog does not work correctly. Data entered in the wizard may not appear in the detail dialog and vice versa.
- **Workaround**: Use either the wizard or the detail dialog for complete editing, not both.
- **Status**: Will be replaced by schema-driven BResourceDialog for editing (BFileSetWizard kept for guided creation)
- **Files**: `src/jobs/bnewfilesetdialog.cpp`, `src/jobs/bfilesetdialog.cpp` (will be consolidated)

### FileSet Not Updated After Editing
- **Severity**: 🟡 Medium
- **Description**: FileSet is not updated in the Director after editing via BResourceDialog or BFileSetWizard. Changes are collected locally but not sent back to the Director.
- **Workaround**: Manually apply changes via bconsole
- **Status**: Under development
- **Files**: `src/director/bresourcewidget.cpp`, `src/director/bresourcedialog.cpp`

### Client Online/Offline Status
- **Severity**: 🟡 Medium
- **Description**: Client online/offline status detection is unreliable. The `status client=<name>` response is marked as ONLINE by checking for "header" and "terminated" keys, but Director returns these even for offline clients (cached data). According to Bareos GitHub Issue #2325, JSON output for status client is still a feature request - FD can't output JSON.
- **Workaround**: Manual verification via bconsole `status client=<name>`
- **Status**: Under investigation (Bareos limitation)
- **Files**: `src/bmainwindow.cpp`, `src/clients/bclientsmodel.cpp`

### Add Job/JobDefs Wizard — Incomplete
- **Severity**: 🟡 Medium
- **Description**: BJobWizard (Build 274) compiles but is not yet runtime-tested. Known gaps: Schedule/Messages combos empty, RunScript blocks can't be sent via `configure add`, Pool overrides not in configure output.
- **Workaround**: Use bconsole `configure add job` directly, or export config text from wizard Preview page
- **Status**: WIP — needs runtime testing and Director integration verification
- **Files**: `src/jobs/bjobwizard.cpp`, `include/jobs/bjobwizard.h`

## Resolved Issues

### Build 390+

- Added: Schedule Management — dual Gantt timeline (FD/SD) and weekly planner grid
  - Gantt view: grouped by Client (FD) and Storage (SD), day/week modes, zoom, snap, drag & drop
  - Weekly planner: 7×24 grid with side-by-side level display, legend, hover tooltips
  - Files: `src/schedules/bscheduleganttwidget.cpp`, `src/schedules/bweeklyplanner.cpp`, `src/schedules/bschedulewidget.cpp`
- Fixed: Schedule grid empty — `.schedule` dot-command only returns names, not run directives
  - Root cause: Bareos `.schedule` returns only `name` + `enabled`; `show schedules` returns full config with `Run` arrays
  - Added `ShowSchedules` command dispatch through BareosDirector → BDirector → BScheduleWidget
  - Also fixed: `show schedules` returns object format `{"name1": {...}}` not array — added converter
  - Files: `src/director/bareosdirector.cpp`, `src/director/bdirector.cpp`, `src/schedules/bschedulewidget.cpp`
- Fixed: Schedules not loading on startup when Schedules tab was last active
  - Root cause: `allResourcesLoaded` lambda only refreshed jobs and clients, not schedules
  - Added `m_scheduleWidget->triggerRefresh()` to initial connect flow
  - File: `src/bmainwindow.cpp`
- Fixed: `MIN_PIXELS_PER_HOUR` linker error in Debug build
  - Root cause: `static const int` ODR-used in `qMax()` needs out-of-line definition in Debug mode
  - Changed to `static constexpr int`
  - File: `include/schedules/bscheduleganttwidget.h`
- Added: Settings persistence for schedule widget state (view mode, day/week, splitter, checkboxes)
  - Files: `include/config/bsettings.h`, `src/config/bsettings.cpp`
- Added: Main window tab persistence across sessions
  - Files: `src/bmainwindow.cpp`, `include/config/bsettings.h`, `src/config/bsettings.cpp`

### Build 274

- Added: Add Job/JobDefs Wizard (BJobWizard) — 5-page QWizard (WIP)
  - Pages: Basics, Resources, Scripts (BRunScriptEditor), Advanced (BResourceForm), Preview & Execute
  - `configure add job`/`configure add jobdefs` command generation
  - Files: `src/jobs/bjobwizard.cpp`, `include/jobs/bjobwizard.h`
- Added: `BResourceForm::setExcludedDirectives()` — skip directives already shown on other pages
  - Files: `include/config/bresourceform.h`, `src/config/bresourceform.cpp`
- Fixed: Wiki submodule not registered in git index
  - Root cause: `.gitmodules` had entry but submodule was never staged (no gitlink in index)

### Build 268

- Fixed: Catalog combo box empty in Edit Jobs dialog
  - Root cause: `ResourceType::Catalog` missing from `m_requiredResources` — `.catalogs` dot-command was never sent. Additionally, `parseCatalogs` expected array format but Bareos returns object format `{"PDC-18": {"name":..., "dbuser":...}}`
  - Files: `src/director/bareosdirector.cpp`, `src/models/bresourcemodels.cpp`

- Fixed: JobDefs reference dropdown showing Job names instead of JobDef names
  - Root cause: `refData["JobDefs"] = model->jobNames()` used the wrong accessor method
  - Added `parseShowJobDefs()` / `jobDefsNames()` to unified `BJobConfigModel`
  - Files: `src/bmainwindow.cpp`, `include/models/bresourcemodels.h`, `src/models/bresourcemodels.cpp`, `include/jobs/bjobwidget.h`, `src/jobs/bjobwidget.cpp`

- Fixed: Storage preset not loading in BResourceForm
  - Root cause: Bareos `show jobs` returns storage as string array `["PDC-sd"]`, not object. Form must unwrap first element for `resource_reference` combos
  - File: `src/director/bresourceform.cpp`

- Fixed: RunScript display as flat text in BResourceForm
  - Now shows multiple RunScript blocks with `{ }` delimiters for clarity
  - File: `src/director/bresourceform.cpp`

- Added: Schema-driven resource editing (BResourceDialog + BResourceForm)
  - Directive JSON schemas for all 11 resource types
  - Auto-generated form widgets from schema definitions
  - Group filtering, advanced toggle, reference data population

- Added: Catalog directive schema (`resources/directives/catalog.json`)

### Build 226

- Fixed: Port not showing in client details dialog
  - Root cause: Port from `m_addresses` enrichment was only applied if address was empty; default port 9102 was never set
  - Now always uses port from enrichment map if available, defaults to 9102 if not found
  - File: `src/clients/bclientsmodel.cpp`

### Build 225

- Fixed: Client list empty on startup after connecting to Director
  - Root cause: `onRefreshAll()` sends 10+ commands simultaneously; when Bareos server sends responses back-to-back, multiple JSON objects arrive in a single TCP read. The JSON text accumulation parser tried to parse the entire buffer as one JSON document, failed with `GarbageAtEnd`, and cleared the entire buffer - losing all responses.
  - File: `src/clients/bclientswidget.cpp`

### Build 193

- Fixed: Client export password always "CHANGE_ME" — now retrieves actual passwords from Director
- Fixed: Job status filter `f` (lowercase) changed to `F` (uppercase) across all files to match Bareos status codes
  - Updated `bjobwidget.cpp`, `bjobmodels.cpp`, `bjsonjobview.cpp`, `bjobdetailsdialog.cpp`, `bjoblogdialog.cpp`, `bclientdetailsdialog.cpp`, `bcleanupdialog.cpp`
- Fixed: Error and Running job status filters not working
  - Root cause: inclusion logic hid jobs with status codes not covered by any checkbox (`B`, `C`, `S`, etc.)
  - Changed to exclusion logic: unchecked boxes hide their statuses, unknown statuses always shown
- Fixed: Retry action only enabled for lowercase `f` — now also enables for `F`, `E`, and `e`
- Fixed: Hardcoded German strings in UI replaced with English `tr()` calls
  - `bjobwidget.cpp`: formatJobStatus() — Erstellt, Läuft, Blockiert, etc. → Created, Running, Blocked, etc.
  - `bjoblogdialog.cpp`: Erfolgreich, Mit Warnungen, Fehler → OK, Warning, Error
  - `storagewidget.cpp`, `bareosdirector.cpp`: error messages translated
- Added: "Add Job..." and "Add FileSet..." menu items in Jobs menu (placeholder, not implemented yet)

### Build 186

- Fixed: Client online/offline status unreliable with TLS-PSK (PSK does not update `lastconnection`)
  - Added active status probing via `status client=<name>` command (`Command::StatusClient`)
  - `getClientStatus()` checks active probe result first, falls back to timestamp heuristic
  - `requestStatusChecks()` sends probe for all clients after `setAllClients()`
  - `commandError` handler marks unreachable clients as OFFLINE
- Fixed: PSK cipher list included hybrid ciphers (RSA-PSK, DHE-PSK, ECDHE-PSK) causing `no suitable signature algorithm` on FD
  - Changed `name.contains("PSK")` to `name.startsWith("PSK-")` in `bareosauth.cpp`
- Added: TLS Cipher List directive in exported client configs (from auth connection)
- Added: Director name extraction from CRAM-MD5 challenge (Bareos `R_DIRECTOR::` prefix, Bacula `@DirectorName`)

### Build 151

- Fixed: BResourceDialog empty when opened from Client Details Settings tab
  - `loadSchemas()` was only called from `bnewclientdialog.cpp` and `bconfigimportdialog.cpp`
  - Added `BDirectiveSchema::instance().loadSchemas()` call in `BResourceDialog::buildForm()`
- Fixed: Client online/offline status inverted due to timezone mismatch
  - Parse timestamps as UTC (`setTimeSpec(Qt::UTC)`) and compare against `QDateTime::currentDateTimeUtc()`

### Version 0.1.0

- Fixed: Configure success detection for Bareos JSON API response format
  - `onJsonResponse()` searched for "created"/"success" text, but Bareos JSON API returns `result.configure.add` object
  - Now correctly parses `configureObj.contains("add")` and extracts client name and filename from the response

### Version 0.1.0.4

- Fixed: Run command duplication ("run job=run job=..." issue)
  - Changed from Command::Run to Command::Custom for Run Job dialog
- Fixed: Delete command duplication ("delete job jobid=job jobid=XX")
  - Fixed args passing in bjsonjobview.cpp and bareosdirector.cpp
- Fixed: Purge command not working ("Unknown job action command: purge")
  - Added purge handler in bjobwidget.cpp using Command::Custom
- Fixed: Status filter missing Running (R) and Canceled (A) options
  - Added Running and Canceled checkboxes to basic filters
- Added: Job preselection in Run Job dialog
  - Uses .defaults command to fetch job configuration
  - Auto-selects FileSet, Pool, Storage, Client, Level from defaults

### Version 0.1.0.3

- Fixed: Job log flipping/switching between jobs when rapidly selecting different jobs
  - Added pending job ID tracking to ensure correct log is displayed
- Fixed: Job log not displaying in BJobWidget bottom panel
  - Fixed signal handler to use current row instead of checkbox selection
- Fixed: Folder selection in file browser not counting subfolders
  - Added propagation of tree checkbox state to file list items
- Fixed: Restore button width too narrow (only showing "1 file")
  - Added dynamic width calculation using QFontMetrics
- Fixed: Run Job button not being enabled in new job dialog
  - Added button state update after data loading
- Fixed: Run Job dialog allows starting without selecting a job
  - Added minimum requirement check (job must be selected)
- Added: Delete job options (delete record vs purge with volume data)
- Added: Dependent job detection when deleting Full backups
- Added: About Qt option in Help menu
- Added: English translation for Run New Job dialog

### Version 0.1.0

- Fixed: About dialog now shows correct version with build number
- Fixed: Version header auto-generation via CMake

## Reporting Bugs

Please report new bugs by creating an issue in the project repository.

Include the following information:
- **Severity**: Critical / High / Medium / Low / Trivial
- **Onesimus version**: (see About dialog)
- **Operating system**: (name and version)
- **Steps to reproduce**: Detailed steps to trigger the issue
- **Expected behavior**: What should happen
- **Actual behavior**: What actually happens
- **Error messages**: Any error messages or log output
- **Workaround**: If any workaround exists

### Severity Assessment Guide

- **🔴 Critical**: Application crashes, data corruption, security vulnerabilities, complete loss of core functionality
- **🟠 High**: Major feature is broken or unusable, no reasonable workaround exists
- **🟡 Medium**: Feature partially works, workaround available but inconvenient
- **🟢 Low**: Minor inconvenience, cosmetic issue, or missing convenience feature
- **⚪ Trivial**: Typos, minor UI inconsistencies, very minor issues
