# Onesimus - Technical Documentation

**Version:** 0.2.0 (Build 445)
**Last Updated:** 2026-02-22
**Project:** Bareos/Bacula GUI Configuration Manager

---

## Table of Contents

1. [Introduction](#1-introduction)
2. [Architecture](#2-architecture)
3. [Director Communication](#3-director-communication)
4. [Directive Schema System](#4-directive-schema-system)
5. [Schema-Driven Forms](#5-schema-driven-forms)
6. [Wizards](#6-wizards)
7. [Model/View Architecture](#7-modelview-architecture)
8. [Schedule Visualization (WIP)](#8-schedule-visualization-wip)
9. [Password & Authentication](#9-password--authentication)
10. [Connection Wizard](#10-connection-wizard)
11. [UI Patterns](#11-ui-patterns)
12. [Deprecated: Database Approach](#12-deprecated-database-approach)

---

## 1. Introduction

**Onesimus** is a Qt6 C++17 GUI application for managing Bareos and Bacula backup systems. It connects to a Bareos/Bacula Director via TCP/TLS and provides a graphical interface for monitoring jobs, managing clients, editing resources, and creating new configurations.

### Key Features

- **Live Director Communication** — Real-time connection to Bareos Director via JSON-RPC (`.api 2`)
- **JSON Directive Schemas** — Auto-generated forms from declarative JSON definitions (13 resource types)
- **Schema-Driven Resource Editing** — BResourceForm + BResourceDialog for any resource type
- **Wizard-Based Creation** — New Client Wizard, FileSet Wizard, Job/JobDefs Wizard
- **BVFS File Browser** — Browse backup files via Bareos Virtual File System
- **Template System** — 22 pre-configured FileSet templates for common scenarios
- **Cross-Platform** — Windows, Linux, macOS via Qt6 + CMake
- **TLS Support** — PSK, Certificate-based, and Legacy authentication

---

## 2. Architecture

### Thread Model

```
┌──────────────────┐        ┌──────────────────┐
│   Main Thread     │        │  Worker Thread    │
│                  │        │                  │
│  BDirector       │──Qt──→│  BareosDirector   │
│  (thread-safe    │ Queued │  (TCP socket,     │
│   wrapper)       │ Invoke │   JSON-RPC)       │
│                  │        │                  │
│  UI Widgets      │←signals│  Auth (CRAM-MD5)  │
│  Models          │        │  BJsonStreamReader│
└──────────────────┘        └──────────────────┘
```

- **BDirector** (MainThread) — Thread-safe API for UI code. Uses `QMetaObject::invokeMethod()` with `Qt::QueuedConnection` to forward calls to the worker thread.
- **BareosDirector** (WorkerThread) — Actual TCP socket, binary protocol, CRAM-MD5/TLS authentication, JSON-RPC parsing.

### Enum-Driven Command Architecture

Commands and resource types use strongly-typed enums:

```cpp
// BDirector::Command — all Director commands
enum class Command {
    DotClients, DotJobs, DotFilesets, DotPools, DotStorage,
    DotSchedules, DotMessages, DotCatalogs,
    ShowJobs, ShowJobDefs, ShowFilesets, ShowClients,
    Configure, Reload, Run, Cancel, Delete, Purge, ...
};

// BDirector::ResourceType — resource types
enum class ResourceType {
    Client, FileSet, Storage, Pool, Schedule, Messages, Catalog, Job, JobDefs, ...
};
```

### Signal Flow

```
BareosDirector::jsonResult(Command, QString)
    → BDirector::jsonResult(Command, QString)
        → Model parse slots (e.g., BJobConfigModel::parseShowJobs)
        → Widget update slots
```

Phase 6 typed signals (`dotFilesetsResult`, `dotJobsResult`, etc.) connect directly to model parse slots, bypassing the routing switch.

---

## 3. Director Communication

### Bareos `.api 2` JSON-RPC

Onesimus uses `.api 2` mode for structured JSON responses:

```
.api 2           → Switches Director to JSON-RPC mode
.clients         → List client names (dot-command)
show jobs        → Full job resource details (JSON object)
configure add... → Create new resources
reload           → Reload Director configuration
```

### Key Implementation Details

- **Dual-response for `.api 2`**: Director sends TWO responses — binary text status + JSON-RPC confirmation. Must consume the JSON confirmation via `m_consumeApiConfirmation` flag.
- **Multi-telegram JSON**: A single JSON response can span multiple binary telegrams. Accumulated in `m_binaryJsonAccumulator` before processing.
- **`m_requiredResources`**: Controls which dot-commands are sent during startup. If a resource type isn't listed, its command never fires.

### Bareos Response Formats

| Command | Format |
|---------|--------|
| `.clients` | JSON array of strings |
| `.catalogs` | JSON **object** keyed by name (NOT array) |
| `show jobs` | `result.jobs` — object keyed by name |
| `show filesets` | `result.filesets` — object keyed by name |
| `show jobs` storage | String **array** `["PDC-sd"]`, NOT object |
| `show jobs` client/fileset/pool/messages | String values |
| `show jobs` runscript | Array of objects |

---

## 4. Directive Schema System

### Overview

BDirectiveSchema is a singleton that loads JSON schemas from `resources/directives/*.json` for all 13 resource types:

```
catalog.json, client.json, console.json, director.json,
fileset.json, job.json, jobdef.json, messages.json,
pool.json, schedule.json, storage.json,
directive_groups.json, directive_schema.json
```

### Schema Format

Each directive is defined as a JSON object:

```json
{
  "Maximum Concurrent Jobs": {
    "type": "integer",
    "required": false,
    "bareos": true,
    "bacula": true,
    "use": true,
    "min": 1,
    "max": 1000,
    "default": 20,
    "description": "Maximum number of concurrent jobs",
    "example": 50,
    "synonyms": ["MaximumConcurrentJobs"],
    "group": "performance"
  }
}
```

### Field Definitions

| Field | Description |
|-------|-------------|
| `type` | `boolean`, `integer`, `string`, `enum`, `resource_reference`, `path`, `directory`, `block`, `blocklist` |
| `required` | Mandatory directive |
| `bareos` / `bacula` | System compatibility flags |
| `use` | `true` = shown by default (basic), `false` = advanced |
| `default` | Default value |
| `description` | Human-readable description |
| `synonyms` | Alternative directive names |
| `group` | Logical grouping (tls, authentication, network, performance, retention, etc.) |
| `min` / `max` | Range constraints for integers |
| `valid_values` | Enum choices |
| `reference_type` | Referenced resource type for `resource_reference` |

### Usage

```cpp
BDirectiveSchema &schema = BDirectiveSchema::instance();
schema.loadSchemas();  // Loads from Qt resources

// Get all directives for a resource type
QList<BDirective> directives = schema.directives("Job");

// Individual directive lookup
BDirective dir = schema.directive("Job", "Client");
```

---

## 5. Schema-Driven Forms

### BResourceForm

Auto-generates form widgets from directive schemas:

```cpp
BResourceForm *form = new BResourceForm("Job", parent);

// Filter to specific groups
form->setGroupFilter({"basic", "tls"});

// Skip directives shown elsewhere (e.g., wizard pages)
form->setExcludedDirectives({"Name", "Type", "Client", "FileSet"});

// Populate reference dropdowns (Client, Pool, Storage, etc.)
form->setReferenceData(referenceData);

// Toggle advanced directives
form->setAdvancedVisible(true);

// Pre-populate from existing resource
form->setExistingResource(existingResource);

// Collect values
form->collectValues();
BConfigResource resource = form->resource();
```

### BResourceDialog

Standard QDialog wrapping BResourceForm for editing any resource type:

```cpp
BResourceDialog dialog("Client", existingResource, parent);
dialog.setReferenceData(referenceData);
if (dialog.exec() == QDialog::Accepted) {
    BConfigResource edited = dialog.resource();
}
```

### BConfigResource / BConfigValue

Key-value containers for resource data:

```cpp
BConfigResource resource;
resource.setValue("Name", BConfigValue("MyJob"));

// BConfigValue types (NOT String/Integer/Boolean):
BConfigValue::Simple     // → simpleValue() returns QString
BConfigValue::List       // → listValue() returns QStringList
BConfigValue::Block      // → blockValue() returns QMap<QString, BConfigValue>
BConfigValue::BlockList  // → blockListValue() returns QList<QMap<...>>
```

---

## 6. Wizards

### New Client Wizard (BNewClientDialog)

3-page wizard for adding backup clients:
1. **Connection** — Client name, address, port, password
2. **Configuration** — TLS settings, retention, auto-prune
3. **Preview** — Generated FD-side and Director-side config, `configure add client`, ZIP export

### FileSet Wizard (BFileSetWizard)

Multi-page wizard for creating FileSet resources:
- Include/Exclude block editor with `BIncludeBlockWidget`
- Options form (`BIncludeOptionsForm`) with directive name mapping
- Template selection from 22 predefined templates
- Deploy mode: `configure add fileset` or manual config export

### Job/JobDefs Wizard (BJobWizard)

5-page wizard for creating Job or JobDefs resources:

```cpp
BJobWizard wizard(BJobWizard::JobType, m_director, this);
wizard.setReferenceData(referenceData);
wizard.exec();
```

**Pages:**
1. **Basics** — Name, Type (Backup/Restore/Verify/Admin/...), JobDefs, Enabled
2. **Resources** — Client, FileSet, Storage, Pool, Messages, Schedule, Catalog, Level, Priority + simple script fields (Run Before/After Job)
3. **Scripts** — `BRunScriptEditor` for multiple RunScript blocks
4. **Advanced** — `BResourceForm("Job")` with excluded directives from pages 1-3
5. **Preview** — Config text, `configure add` command, Copy/Execute

**RunScript Editor:**
- `BRunScriptEditor` — Table widget for managing multiple RunScript blocks
- `BRunScriptDialog` — Edit dialog for a single RunScript entry (Command, RunsWhen, RunsOnClient, RunsOnFailure, AbortJobOnError, FailJobOnError)

**Known Limitations:**
- RunScript blocks not supported by `configure add` — manual deployment notice shown
- Schedule/Messages combos not yet populated
- Pool overrides not yet wired to configure add output

### `configure add` Format

Bareos `configure add` uses flat key=value pairs:

```
configure add job name="MyBackup" type="Backup" client="server1-fd" \
  fileset="LinuxAll" storage="File" pool="Full" messages="Standard"
```

Does NOT support nested blocks (RunScript, Include/Exclude). These must be deployed manually via config files.

---

## 7. Model/View Architecture

### Base Models

All data models inherit from custom base classes:

- **BListModel** — Simple list model (single column)
- **BTableModel** — Multi-column table model
- **BTreeModel** — Hierarchical tree model

### Resource Models

| Model | Data Source | Notes |
|-------|------------|-------|
| `BJobConfigModel` | `show jobs` + `show jobdefs` | Unified: `parseShowJobs()`, `parseShowJobDefs()`, `jobNames()`, `jobDefsNames()` |
| `BFilesetModel` | `show filesets` | Caches full JSON, `loadFromBareosJson()` for wizard |
| `BStorageModel` | `.storage` | Storage daemon list |
| `BPoolModel` | `.pools` | Pool list |
| `BCatalogModel` | `.catalogs` | Bareos object format (NOT array) |
| `BClientsModel` | `.clients` + status | Client list with online/offline status |

### Data Flow

```
Director Command → BareosDirector → JSON Response
    → Signal (jsonResult / typed signal)
        → Model::parse*() slot
            → Model data updated
                → View automatically refreshed (Qt Model/View)
```

---

## 8. Schedule Visualization (WIP)

> **Work in Progress** — The schedule visualization module is under active development. Features described here are partially implemented and subject to change.

### Overview

The Schedules tab (`BScheduleWidget`) provides visual tools for inspecting and planning backup schedules. It queries schedule data via `.schedules` and `show schedules` commands and cross-references job configurations to build a complete picture of when backups run.

### Views

| View | Widget | Description |
|------|--------|-------------|
| **Timeline (Gantt)** | `BScheduleGanttWidget` | Dual stacked horizontal timelines — FD/Client (top) and SD/Storage (bottom) |
| **Grid** | `BWeeklyPlanner` | Compact 7-day x 24-hour weekly overview grid |

### Gantt Timeline

- **Dual timelines** — FD grouped by client, SD grouped by storage, in a vertical splitter
- **Job bars** colored by backup level: Full (green), Differential (orange), Incremental (blue)
- **Bar width** represents estimated duration from historical job run statistics (`BJobDurationStats`)
- **Day / Week mode** — Day shows a single 24h timeline; Week shows 7x24h with day headers
- **Two-row header** — Row 1: weekday names (tinted blue), Row 2: hour labels
- **Zoom slider** — In Week mode, controls pixels-per-hour (10–60 px/h) with horizontal scrollbar
- **Snap granularity** — Day mode: 15min/30min/1h; Week mode: 1h/2h/4h
- **Heatmap** — Bottom bar showing concurrent job count per 15-min time slot
- **Collision detection** — Highlights overlapping jobs on the same client or storage
- **Now marker** — Red vertical line at the current time
- **Drag & drop** — Drag job bars to reschedule; shows dependency edges and generates `configure` commands via `BScheduleDragResultDialog`

### Left Panel

- **Schedule list** (top) — Checkboxes to filter which schedules appear on the timeline
- **Job list** (bottom) — Table showing jobs associated with the selected schedule (Name, Client, Duration)
- Selecting a job triggers the statistics panel for that job

### Statistics Panel

Clicking a job bar or selecting a job in the job list opens a collapsible stats panel showing:
- Min / Avg / Max duration from historical runs
- Trend indicator
- Recent run history table

### Data Models

| Class | Purpose |
|-------|---------|
| `BScheduleModel` | Parses `.schedules` and `show schedules` JSON into `BScheduleEntry` list |
| `BJobDurationStats` | Collects historical job durations for bar width and stats panel |
| `BJobScheduleIndex` | Maps schedule names to their associated jobs via `QMultiMap<QString, JobRef>` |

### Not Yet Implemented

- Drag & drop from job list onto the timeline to assign jobs to schedule time slots
- Multi-select jobs for batch schedule changes
- Schedule conflict resolution wizard
- Export/print of schedule overview

### Key Files

| File | Description |
|------|-------------|
| `include/schedules/bschedulewidget.h` | Main schedule widget (left panel + right views) |
| `src/schedules/bschedulewidget.cpp` | Layout, toolbar, data wiring, job list |
| `include/schedules/bscheduleganttwidget.h` | Gantt timeline widget |
| `src/schedules/bscheduleganttwidget.cpp` | Painting, drag & drop, collision detection |
| `include/schedules/bweeklyplanner.h` | Weekly grid planner widget |
| `src/schedules/bweeklyplanner.cpp` | Grid painting, schedule parsing |
| `include/schedules/bjobscheduleindex.h` | Schedule-to-job mapping index |
| `include/models/bresourcemodels.h` | `BScheduleModel`, `BScheduleEntry`, `BJobDurationStats` |

---

## 9. Password & Authentication

### MD5 Password Hashing

Bareos CRAM-MD5 authentication requires passwords hashed with MD5 and prefixed with `[md5]`:

```cpp
#include "bpasswordutil.h"

QString hashed = BPasswordUtil::hashPasswordMD5("mySecretPassword");
// Result: "[md5]5f4dcc3b5aa765d61d8327deb882cf99"

BPasswordUtil::PasswordFormat format = BPasswordUtil::detectFormat(hashed);
// Returns: PasswordFormat::MD5
```

### Authentication Methods

| Method | Description |
|--------|-------------|
| **TLS-PSK** | Pre-Shared Key — simple shared secret, most common for Bareos |
| **TLS-Certificate** | CA certificate + client certificate validation |
| **Legacy** | Plain CRAM-MD5 without TLS (insecure, deprecated) |

### TLS Configuration

```conf
Console {
  Name = onesimus-console
  Password = "[md5]5f4dcc3b5aa765d61d8327deb882cf99"
  TLS Enable = yes
  TLS Require = yes
  TLS CA Certificate File = /etc/bareos/tls/ca.pem
  TLS Certificate = /etc/bareos/tls/client.pem
  TLS Key = /etc/bareos/tls/client.key
}
```

**PSK Cipher Note:** PSK cipher list must use `startsWith("PSK-")` filter — hybrid ciphers (RSA-PSK, DHE-PSK, ECDHE-PSK) cause `no suitable signature algorithm` errors on FD.

---

## 10. Connection Wizard

### BConnectionWizard

Multi-page wizard for establishing Director connections:

**Pages:**
1. **Welcome** — Introduction
2. **Server** — Host, Port, capability check
3. **Credentials** — Director name, Console name, Password (auto-hashed to MD5)
4. **AuthMethod** — PSK, Certificate, or Legacy
5. **TLS** — Certificate paths (if cert auth selected)
6. **ConfigPreview** — Generated bconsole.conf preview
7. **Test** — Test connection and authentication
8. **ProfileName** — Save connection profile

**Connection Profile Data:**

```cpp
struct BConnectionProfile {
    QString host;
    int port = 9101;
    QString directorName;
    QString consoleName;
    QString password;       // MD5 hash
    QString authMethod;     // "psk", "cert", "legacy"
    QString tlsCaCertFile;
    QString tlsCertFile;
    QString tlsKeyFile;
    QString profileName;
};
```

---

## 11. UI Patterns

### Conditional Field Visibility

```cpp
void updateConditionalDisplay() {
    bool tlsEnable = getFieldValue("TLS Enable").toBool();
    bool tlsUsePSK = getFieldValue("TLS Use PSK").toBool();
    bool showCerts = tlsEnable && !tlsUsePSK;

    m_widgets["TLS CA Certificate File"]->setVisible(showCerts);
    m_widgets["TLS Certificate"]->setVisible(showCerts);
    m_widgets["TLS Key"]->setVisible(showCerts);
}
```

### Tabbed Widget Pattern

For configuration dialogs with many directives:

```cpp
QTabWidget *tabs = new QTabWidget();
tabs->addTab(createBasicTab(), tr("Basic"));
tabs->addTab(createTlsTab(), tr("Security / TLS"));
tabs->addTab(createPerformanceTab(), tr("Performance"));
tabs->addTab(createAdvancedTab(), tr("Advanced"));
```

### Reference Data Population

Wizards and forms receive reference data for dropdown population:

```cpp
QMap<QString, QStringList> refData;
refData["Client"] = m_jobWidget->clientNames();
refData["FileSet"] = m_jobWidget->filesetNames();
refData["Storage"] = m_jobWidget->storageNames();
refData["Pool"] = m_jobWidget->poolNames();
refData["Catalog"] = m_jobWidget->catalogNames();
refData["JobDefs"] = m_jobWidget->jobDefsNames();
refData["Schedule"] = QStringList();  // TODO: populate
refData["Messages"] = QStringList();  // TODO: populate

wizard.setReferenceData(refData);
```

---

## 12. Deprecated: Database Approach

> **Note:** The original design (January 2026) used a "database-first" architecture with SQLite for all persistent storage. This approach has been **deprecated** in favor of direct Director communication. The current architecture queries the Director in real-time via `.api 2` JSON-RPC commands.

### What Was Deprecated

- **SQLite database** for storing Director, Client, Storage, Pool configurations
- **BDirectorModel**, **BClientModel** (QSqlTableModel-based) — replaced by in-memory models parsing Director JSON responses
- **BResourceModel** (QSqlTableModel base class) — replaced by BListModel/BTableModel/BTreeModel
- **settings_history table** for audit trail
- **BDirectiveRegistry** singleton — replaced by **BDirectiveSchema** singleton
- **Post-authentication database import** — resources now queried live from Director
- **"Offline management"** concept — application requires active Director connection

### What Remains from That Era

- **BPasswordUtil** — MD5 password hashing (still used for CRAM-MD5 auth)
- **JSON directive schemas** — format unchanged, loader class renamed to BDirectiveSchema
- **FileSet templates** — 22 preset templates in `resources/templates/filesets/`
- **Connection profiles** — saved via QSettings (not SQLite)
- **bdirectiveregistry.h/.cpp** — still exists in codebase but superseded by BDirectiveSchema

### Files (db/ directory)

The `include/db/` and `src/db/` directories still contain the database classes. They are not actively used for the main application flow but may be retained for future local caching or offline mode.

---

**End of Documentation**

**Version:** 0.2.0 (Build 445)
**Last Updated:** 2026-02-22
