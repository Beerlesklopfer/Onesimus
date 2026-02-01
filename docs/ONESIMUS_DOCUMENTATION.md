# Onesimus - Comprehensive Documentation

**Version:** 0.1.0.5
**Last Updated:** 2026-01-31
**Project:** Bareos/Bacula GUI Configuration Manager

---

## Table of Contents

1. [Introduction](#introduction)
2. [Database Architecture](#database-architecture)
3. [Password Management](#password-management)
4. [Model/View Architecture](#modelview-architecture)
5. [Directive System](#directive-system)
6. [Connection Wizard & Configuration Import](#connection-wizard--configuration-import)
7. [UI Patterns](#ui-patterns)
8. [Development Summary](#development-summary)
9. [API Reference](#api-reference)

---

## 1. Introduction

**Onesimus** is a Qt-based GUI application for managing Bareos and Bacula backup system configurations. It provides an intuitive interface for creating, managing, and monitoring Directors, Clients, Storage daemons, and backup jobs.

### Key Features

- **Database-First Design** - All configurations stored in SQLite with full audit trail
- **JSON Directive System** - Auto-generated dialogs from declarative JSON definitions
- **Dual System Support** - Works with both Bareos and Bacula
- **Template System** - Pre-configured FileSet templates for common scenarios
- **Tabbed Widget UI** - Organized, non-overwhelming interface
- **Password Security** - MD5 hashing for CRAM-MD5 authentication
- **Configuration Import** - Import Director configuration after authentication
- **Offline Management** - Edit configurations without active connection

---

## 2. Database Architecture

### 2.1 Overview

Onesimus uses SQLite for all persistent storage, with QSettings only storing the database path.

**Database Path (Windows):**
```
%APPDATA%/Onesimus/onesimus.db
```

**QSettings Storage:**
```ini
[Database]
path=C:/Users/Username/AppData/Roaming/Onesimus/onesimus.db
```

### 2.2 Core Tables

#### Directors Table

```sql
CREATE TABLE directors (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    address TEXT NOT NULL,
    port INTEGER DEFAULT 9101,
    password_hash TEXT,  -- MD5 hash with [md5] prefix
    tls_enable BOOLEAN DEFAULT 1,
    tls_require BOOLEAN DEFAULT 0,
    tls_ca_cert_file TEXT,
    tls_cert_file TEXT,
    tls_key_file TEXT,
    working_directory TEXT,
    maximum_concurrent_jobs INTEGER DEFAULT 20,
    statistics_retention TEXT,
    auditing BOOLEAN DEFAULT 0,
    description TEXT,
    backup_system TEXT DEFAULT 'bareos',  -- 'bareos' or 'bacula'
    is_default BOOLEAN DEFAULT 0,
    is_active BOOLEAN DEFAULT 1,
    last_connected_at DATETIME,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
```

#### Clients Table

```sql
CREATE TABLE clients (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    director_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    address TEXT NOT NULL,
    fd_port INTEGER DEFAULT 9102,
    password_hash TEXT,
    file_retention TEXT DEFAULT '60 days',
    job_retention TEXT DEFAULT '6 months',
    autoprune BOOLEAN DEFAULT 1,
    tls_enable BOOLEAN DEFAULT 1,
    maximum_concurrent_jobs INTEGER DEFAULT 1,
    description TEXT,
    is_active BOOLEAN DEFAULT 1,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (director_id) REFERENCES directors(id) ON DELETE CASCADE
);
```

#### Settings History Table

**Purpose:** Track ALL setting changes with audit trail

```sql
CREATE TABLE settings_history (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    resource_type TEXT NOT NULL,  -- 'director', 'client', 'storage', etc.
    resource_id INTEGER NOT NULL,
    setting_key TEXT NOT NULL,
    setting_value TEXT,
    changed_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    changed_by TEXT DEFAULT 'user',  -- 'user', 'system', 'import'
    backup_system TEXT,  -- 'bareos', 'bacula', or 'both'
    notes TEXT
);
```

**Example Usage:**
```cpp
// Track custom directive change
INSERT INTO settings_history (
    resource_type, resource_id, setting_key, setting_value,
    changed_by, backup_system, notes
) VALUES (
    'director', 1, 'Secure Erase Command', '/usr/bin/shred',
    'user', 'bareos', 'User configured secure erase'
);
```

### 2.3 Database Models

All database interaction uses Qt's Model/View architecture:

- **BDirectorModel** - Directors table
- **BClientModel** - Clients table
- **BStorageModel** - Storages table (TODO)
- **BConsoleModel** - Consoles table (TODO)

**Example:**
```cpp
BDirectorModel model(nullptr, db);
model.initialize();

// Create new Director
int dirId = model.createDirector("bareos-dir", "192.168.1.100", 9101,
                                 passwordHash, "bareos");

// Update TLS settings
int row = model.findDirectorRow(dirId);
model.setData(model.index(row, BDirectorModel::TlsEnable), true);
model.setData(model.index(row, BDirectorModel::TlsRequire), true);
model.submitAll();

// Custom settings via settings_history
model.setCustomSetting(dirId, "Auditing", "yes", "user", "bareos");
```

---

## 3. Password Management

### 3.1 MD5 Password Hashing

Bareos CRAM-MD5 authentication requires passwords to be hashed with MD5 and prefixed with `[md5]`.

**BPasswordUtil Class:**

```cpp
#include "bpasswordutil.h"

// Hash plaintext password
QString plaintext = "mySecretPassword";
QString hashed = BPasswordUtil::hashPasswordMD5(plaintext);
// Result: "[md5]5f4dcc3b5aa765d61d8327deb882cf99"

// Detect hash format
BPasswordUtil::PasswordFormat format = BPasswordUtil::detectFormat(hashed);
// Returns: PasswordFormat::MD5

// Extract hash without prefix
QString hashOnly = BPasswordUtil::extractHash(hashed);
// Result: "5f4dcc3b5aa765d61d8327deb882cf99"
```

### 3.2 Password Storage in Config Files

**Console Configuration (bconsole.conf):**
```conf
Director {
  Name = bareos-dir
  DIRport = 9101
  Address = 192.168.1.100
  Password = "[md5]5f4dcc3b5aa765d61d8327deb882cf99"  # MD5 hash
  TLS Enable = yes
  TLS Require = yes
}
```

**Director Configuration (bareos-dir.conf):**
```conf
Console {
  Name = onesimus-console
  Password = "[md5]5f4dcc3b5aa765d61d8327deb882cf99"  # Same hash
  TLS Enable = yes
  Profile = "operator"
}
```

### 3.3 Security Best Practices

1. **Never store plaintext passwords** - Always use MD5 hash
2. **Use TLS-PSK or TLS with certificates** - Disable legacy authentication
3. **Encrypt database** - Use SQLCipher for sensitive deployments
4. **Restrict file permissions** - Database file should be user-readable only
5. **Audit password changes** - All changes logged to settings_history

---

## 4. Model/View Architecture

### 4.1 Base Model: BResourceModel

All resource models inherit from `BResourceModel`:

```cpp
class BResourceModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit BResourceModel(QObject *parent, QSqlDatabase &db);

    virtual bool initialize() = 0;
    virtual int createResource(...) = 0;

    // Custom settings via settings_history
    bool setCustomSetting(int resourceId, const QString &key,
                         const QString &value, const QString &changedBy,
                         const QString &backupSystem);

    QMap<QString, QString> getCustomSettings(int resourceId,
                                             const QString &backupSystem) const;
};
```

### 4.2 BDirectorModel

**Header:** [bdirectormodel.h](../include/bdirectormodel.h)

**Column Enum:**
```cpp
enum Column {
    Id = 0,
    Name,
    Address,
    Port,
    PasswordHash,
    TlsEnable,
    TlsRequire,
    TlsCaCertFile,
    TlsCertFile,
    TlsKeyFile,
    WorkingDirectory,
    MaximumConcurrentJobs,
    StatisticsRetention,
    Auditing,
    Description,
    BackupSystem,
    IsDefault,
    IsActive,
    LastConnectedAt,
    CreatedAt,
    UpdatedAt
};
```

**Usage Example:**
```cpp
BDirectorModel model(nullptr, db);
model.initialize();

// Filter to Bareos only
model.setFilterBackupSystem("bareos");

// Filter to active only
model.setFilterActiveOnly(true);

// Create Director
int dirId = model.createDirector(
    "production-dir",
    "backup.example.com",
    9101,
    "[md5]...",
    "bareos"
);

// Update settings
int row = model.findDirectorRow(dirId);
QModelIndex idx = model.index(row, BDirectorModel::MaximumConcurrentJobs);
model.setData(idx, 50);
model.submitAll();
```

### 4.3 Tabbed Edit Dialogs

**Recommended Pattern:**

```cpp
class BDirectorEditDialog : public QDialog
{
public:
    BDirectorEditDialog(int directorId, QSqlDatabase &db, QWidget *parent = nullptr)
        : QDialog(parent), m_directorId(directorId), m_database(db)
    {
        setWindowTitle(tr("Edit Director"));
        setMinimumSize(700, 600);

        auto *layout = new QVBoxLayout(this);

        // Tabbed interface
        QTabWidget *tabs = new QTabWidget(this);
        tabs->addTab(createConnectionTab(), QIcon(":/icons/network"), tr("Connection"));
        tabs->addTab(createSecurityTab(), QIcon(":/icons/lock"), tr("Security / TLS"));
        tabs->addTab(createPathsTab(), QIcon(":/icons/folder"), tr("Paths"));
        tabs->addTab(createPerformanceTab(), QIcon(":/icons/speed"), tr("Performance"));
        tabs->addTab(createCustomSettingsTab(), QIcon(":/icons/settings"), tr("Custom Settings"));
        tabs->addTab(createHistoryTab(), QIcon(":/icons/history"), tr("History"));

        layout->addWidget(tabs);

        // Dialog buttons
        auto *buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &BDirectorEditDialog::saveChanges);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addWidget(buttonBox);

        loadDirector();
    }

private:
    QWidget* createConnectionTab() {
        QWidget *widget = new QWidget();
        auto *form = new QFormLayout(widget);

        m_nameEdit = new QLineEdit();
        m_addressEdit = new QLineEdit();
        m_portSpin = new QSpinBox();
        m_portSpin->setRange(1, 65535);
        m_portSpin->setValue(9101);

        form->addRow(tr("Name:"), m_nameEdit);
        form->addRow(tr("Address:"), m_addressEdit);
        form->addRow(tr("Port:"), m_portSpin);

        return widget;
    }

    QWidget* createSecurityTab() {
        QWidget *widget = new QWidget();
        auto *layout = new QVBoxLayout(widget);

        // TLS group
        auto *tlsGroup = new QGroupBox(tr("TLS Configuration"));
        auto *tlsForm = new QFormLayout(tlsGroup);

        m_tlsEnableCheck = new QCheckBox(tr("Enable TLS"));
        m_tlsRequireCheck = new QCheckBox(tr("Require TLS"));
        m_tlsCaCertEdit = new QLineEdit();
        // ... etc

        tlsForm->addRow(tr("Enable:"), m_tlsEnableCheck);
        tlsForm->addRow(tr("Require:"), m_tlsRequireCheck);
        tlsForm->addRow(tr("CA Certificate:"), m_tlsCaCertEdit);

        layout->addWidget(tlsGroup);
        layout->addStretch();

        return widget;
    }

    void saveChanges() {
        BDirectorModel model(nullptr, m_database);
        model.initialize();

        int row = model.findDirectorRow(m_directorId);
        if (row < 0) return;

        // Update table columns
        model.setData(model.index(row, BDirectorModel::Name), m_nameEdit->text());
        model.setData(model.index(row, BDirectorModel::Address), m_addressEdit->text());
        model.setData(model.index(row, BDirectorModel::Port), m_portSpin->value());

        if (!model.submitAll()) {
            QMessageBox::critical(this, tr("Error"), model.lastError().text());
            return;
        }

        accept();
    }

private:
    int m_directorId;
    QSqlDatabase m_database;
    QLineEdit *m_nameEdit;
    QLineEdit *m_addressEdit;
    QSpinBox *m_portSpin;
    QCheckBox *m_tlsEnableCheck;
    QCheckBox *m_tlsRequireCheck;
    QLineEdit *m_tlsCaCertEdit;
};
```

---

## 5. Directive System

### 5.1 Overview

The directive system uses JSON resource definition files to auto-generate configuration dialogs with validation.

**JSON Files:**
- `resources/directives/director.json` - 28 Director directives
- `resources/directives/client.json` - Client/FD directives
- `resources/directives/console.json` - Console directives
- `resources/directives/fileset.json` - FileSet directives
- `resources/directives/directive_groups.json` - Grouping & validation rules
- `resources/directives/directive_schema.json` - JSON Schema validation

### 5.2 Directive Definition Format

**Example: TLS Enable Directive**

```json
{
  "TLS Enable": {
    "type": "boolean",
    "required": false,
    "bareos": true,
    "bacula": true,
    "use": true,
    "default": true,
    "description": "Enable TLS encryption for Director communication",
    "example": true,
    "synonyms": ["Enable TLS", "TLSEnable"],
    "group": "tls"
  }
}
```

**Field Definitions:**

- `type` - Data type: `boolean`, `integer`, `string`, `time`, `size`, `password`, `file`, `resource_reference`, `string_list`
- `required` - Is this directive mandatory?
- `bareos` - Compatible with Bareos?
- `bacula` - Compatible with Bacula?
- `use` - Show by default (`true`) or in Advanced mode (`false`)
- `default` - Default value
- `description` - Human-readable description
- `example` - Example value (must match type)
- `synonyms` - Alternative names for compatibility
- `group` - Logical grouping (tls, authentication, network, performance, retention, directories, acl, auditing)
- `min` / `max` - Range constraints for integer types
- `valid_values` - Enum values
- `reference_type` - Referenced resource type for `resource_reference`

### 5.3 BDirectiveRegistry

**Singleton Pattern:**

```cpp
BDirectiveRegistry *registry = BDirectiveRegistry::instance();
```

**Loading Directives:**

```cpp
// Automatically loads on first access
// - :/directives/director.json
// - :/directives/client.json
// - :/directives/console.json
// - :/directives/fileset.json
// - :/directives/directive_groups.json
```

**Validation:**

```cpp
// Validate single directive value
QString error;
bool valid = registry->validateValue("director", "Maximum Concurrent Jobs", "50", &error);
if (!valid) {
    qWarning() << "Validation error:" << error;
}

// Validate entire configuration
QMap<QString, QVariant> config;
config["Name"] = "bareos-dir";
config["DirPort"] = 9101;
config["Maximum Concurrent Jobs"] = 50;

QStringList errors;
bool configValid = registry->validateConfiguration("director", config, &errors);
for (const QString &err : errors) {
    qWarning() << err;
}
```

**Querying:**

```cpp
// Get all directives for Director (Bareos only)
QList<BDirectiveDefinition> directives = registry->getDirectives("director", "bareos");

// Get directives by group
QList<BDirectiveDefinition> tlsDirectives = registry->getDirectivesByGroup("director", "tls");

// Get directive names for autocomplete
QStringList names = registry->getDirectiveNames("director", "bareos");

// Resolve synonym to canonical name
QString canonical = registry->resolveDirectiveName("director", "Enable TLS");
// Returns: "TLS Enable"

// Get all groups
QStringList groups = registry->getGroups("director");
// Returns: ["tls", "authentication", "network", "performance", "retention", "directories", "auditing"]
```

### 5.4 Grouping and Conditional Display

**Groups Definition:** [directive_groups.json](../resources/directives/directive_groups.json)

**Example Group:**
```json
{
  "groups": {
    "tls": {
      "name": "TLS Configuration",
      "description": "Transport Layer Security encryption settings",
      "icon": "lock",
      "directives": [
        "TLS Enable",
        "TLS Require",
        "TLS Verify Peer",
        "TLS CA Certificate File",
        "TLS Certificate",
        "TLS Key"
      ],
      "conditional_display": {
        "TLS Certificate": {
          "requires": {
            "TLS Enable": true,
            "TLS Use PSK": false
          }
        }
      }
    }
  }
}
```

**Conditional Display Logic:**

```cpp
// Show TLS Certificate field only when:
// - TLS Enable = true
// - TLS Use PSK = false

bool tlsEnable = getTlsEnable();
bool tlsUsePSK = getTlsUsePSK();

bool showCerts = tlsEnable && !tlsUsePSK;
m_tlsCertEdit->setVisible(showCerts);
m_tlsKeyEdit->setVisible(showCerts);
```

### 5.5 Validation Rules

**Cross-Field Validation:**

```json
{
  "validation_rules": {
    "tls_certificate_complete": {
      "description": "Certificate-based TLS requires all certificate files",
      "condition": {
        "and": [
          {"TLS Enable": true},
          {"TLS Verify Peer": true}
        ]
      },
      "requires": [
        "TLS CA Certificate File",
        "TLS Certificate",
        "TLS Key"
      ],
      "error_message": "Certificate-based TLS requires CA certificate, client certificate, and private key"
    }
  }
}
```

**Implementation:**

```cpp
bool validateTLS(const QMap<QString, QVariant> &config) {
    bool tlsEnable = config["TLS Enable"].toBool();
    bool tlsVerifyPeer = config["TLS Verify Peer"].toBool();

    if (tlsEnable && tlsVerifyPeer) {
        // Check all certificate files are provided
        if (config["TLS CA Certificate File"].toString().isEmpty() ||
            config["TLS Certificate"].toString().isEmpty() ||
            config["TLS Key"].toString().isEmpty()) {

            showError("Certificate-based TLS requires all certificate files");
            return false;
        }
    }
    return true;
}
```

### 5.6 Classification: Common vs Advanced

**Common Directives** (`use: true`):
- Shown by default in Basic/Connection/Security tabs
- Essential for typical configurations
- Examples: Name, Address, Port, TLS Enable, Maximum Concurrent Jobs

**Advanced Directives** (`use: false`):
- Hidden in Advanced tab
- Require expert knowledge
- Examples: Statistics Retention, VerId Retention, Secure Erase Command

**UI Pattern:**

```cpp
QTabWidget *tabs = new QTabWidget();

// Basic tab - use=true directives
tabs->addTab(createBasicTab(), tr("Basic"));

// Group tabs - use=true directives by group
tabs->addTab(createTlsTab(), tr("Security / TLS"));
tabs->addTab(createPerformanceTab(), tr("Performance"));

// Advanced tab - use=false directives
tabs->addTab(createAdvancedTab(), tr("Advanced"));
```

---

## 6. Connection Wizard & Configuration Import

### 6.1 Connection Wizard

**Class:** `BConnectionWizard`
**Header:** [bconnectionwizard.h](../include/bconnectionwizard.h)

**Wizard Pages:**

1. **Welcome** - Introduction
2. **TemplateSelection** - New connection, template, or ZIP import
3. **Server** - Host + Port, capability check
4. **Credentials** - Director, Console, Password (auto-hashed to MD5)
5. **AuthMethod** - PSK, Certificate, or Legacy
6. **TLS** - Certificate paths (if cert auth selected)
7. **ConfigPreview** - Show generated bconsole.conf and Director Console config
8. **Test** - Test connection and authentication
9. **ConsoleSetup** - Modify existing or create new Console resource
10. **ProfileName** - Save profile

**Wizard Data Struct:**

```cpp
struct BConnectionWizardData {
    QString host;
    int port = 9101;
    QString directorName;
    QString consoleName;
    QString password;  // MD5 hash (auto-converted from plaintext)
    bool savePassword = true;
    QString authMethod = "psk";  // "psk", "cert", or "legacy"
    QString tlsCaCertFile;
    QString tlsCertFile;
    QString tlsKeyFile;
    QString profileName;
    bool setAsDefault = true;
    bool connectNow = true;
};
```

**Usage:**

```cpp
// Create wizard with database connection
BConnectionWizard wizard(&wizardData, &db);

if (wizard.exec() == QDialog::Accepted) {
    // Save to database
    int directorId = wizard.saveToDatabase(db);

    if (directorId > 0) {
        qDebug() << "Director saved with ID:" << directorId;
    }
}
```

### 6.2 Template Selection

**TemplateSelectionPage** - Page 2 of wizard

**Options:**
1. **Create new connection from scratch**
2. **Use template from existing connection** - Load from database
3. **Import from ZIP file** - Import exported configuration

**Template Loading:**

```cpp
void TemplateSelectionPage::loadTemplateDetails(int directorId) {
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (!wiz || !wiz->database()) return;

    BDirectorModel model(nullptr, *wiz->database());
    model.initialize();

    int row = model.findDirectorRow(directorId);
    if (row < 0) return;

    // Pre-fill wizard data from template
    if (wiz->wizardData()) {
        wiz->wizardData()->host = model.data(model.index(row, BDirectorModel::Address)).toString();
        wiz->wizardData()->port = model.data(model.index(row, BDirectorModel::Port)).toInt();
        wiz->wizardData()->directorName = model.data(model.index(row, BDirectorModel::Name)).toString();
        // ... etc
    }
}
```

### 6.3 Post-Authentication Configuration Import

**Workflow:**

1. User completes wizard and tests connection
2. **Authentication succeeds**
3. **Check Console permissions** - Can the Console read Director configuration?
4. **If full access:** Import complete Director configuration (all resources)
5. **If limited access:** Store only basic connection info (header entities)

**Permission Check:**

```cpp
bool checkConfigurationACL(BareosDirector *director) {
    // Query current Console's ACLs via .api 2 json
    QString cmd = ".api 2 json\nlist consoles current";
    director->sendJsonCommand(cmd);

    // Wait for response...
    QJsonObject console = response["consoles"][0].toObject();
    QJsonArray commandAcl = console["command_acl"].toArray();

    // Check for full access
    if (commandAcl.contains("*all*") || commandAcl.contains("*")) {
        return true;  // Full access
    }

    // Check for specific configuration commands
    QStringList configCommands = {"show", "list", "status", "configure"};
    for (const QString &cmd : configCommands) {
        if (!commandAcl.contains(cmd)) {
            return false;  // Missing required command
        }
    }

    return true;
}
```

**Full Configuration Import:**

```cpp
void importFullConfiguration(BareosDirector *director, int directorId) {
    qDebug() << "[Import] Starting full configuration import";

    // Query all resource types
    QStringList resourceTypes = {
        "clients", "storages", "pools", "filesets",
        "jobs", "schedules", "consoles"
    };

    for (const QString &resourceType : resourceTypes) {
        QString cmd = QString(".api 2 json\nlist %1").arg(resourceType);
        director->sendJsonCommand(cmd);
    }

    // Query Director configuration
    QString dirCmd = ".api 2 json\nshow director";
    director->sendJsonCommand(dirCmd);

    // Process responses and import to database:
    // - Director settings → directors table + settings_history
    // - All Clients → clients table
    // - All Storages → storages table
    // - All Pools, FileSets, Jobs, Schedules, Consoles → respective tables
}
```

**What Gets Imported (Full Access):**

1. **Director Settings** - All directives from `show director`
2. **Client Resources** - All defined Client/FD resources
3. **Storage Resources** - All defined Storage/SD resources
4. **Pool Resources** - Pool definitions and retention policies
5. **FileSet Resources** - Include/Exclude patterns, VSS settings
6. **Job Resources** - Job definitions and schedules
7. **Console Resources** - Other Console definitions and ACLs
8. **Custom Directives** - Any non-standard directives → `settings_history`

**Header-Only Import:**

```cpp
void storeBasicInfo(BareosDirector *director, int directorId) {
    qDebug() << "[Import] Limited permissions - storing header info only";

    BDirectorModel model(nullptr, db);
    model.initialize();

    int row = model.findDirectorRow(directorId);
    if (row < 0) return;

    // Only update connection metadata
    model.setData(model.index(row, BDirectorModel::LastConnectedAt),
                 QDateTime::currentDateTime());

    model.setData(model.index(row, BDirectorModel::Description),
                 "Connected (limited permissions)");

    model.submitAll();

    // Show user message
    QMessageBox::information(nullptr, tr("Limited Access"),
        tr("Successfully connected, but this Console has limited permissions.\n\n"
           "Only basic connection information has been stored."));
}
```

**What Gets Stored (Limited Access):**

1. **Basic Connection Info** - Director Name, Address, Port, Password Hash
2. **Authentication Settings** - TLS Enable/Require, certificate paths
3. **Metadata** - Description, Backup System, Last Connected timestamp

**NO resources are imported (Clients, Storages, etc.) with limited permissions.**

**UI Indicators:**

```
Directors:
  ✓ Production Director (backup.example.com) - Full config [315 resources]
  ⚠ Development Director (dev.local) - Limited access [connection only]
  ✓ DR Director (dr-site.example.com) - Full config [89 resources]
```

---

## 7. UI Patterns

### 7.1 Tabbed Widget Pattern

**Recommended for:** Configuration dialogs with many directives

**Benefits:**
- Organized, non-overwhelming UI
- Easy navigation between setting categories
- Clear separation of common vs advanced
- Scalable for many directives

**Implementation:**

```cpp
QTabWidget *tabs = new QTabWidget();

// Tab 1: Basic settings (use=true, no group)
tabs->addTab(createBasicTab(), QIcon(":/icons/home"), tr("Basic"));

// Tab 2-N: Directive groups
QStringList groups = {"tls", "network", "performance", "retention", "directories"};
for (const QString &groupId : groups) {
    QWidget *groupTab = createGroupTab(groupId);
    QIcon groupIcon = getGroupIcon(groupId);
    QString groupName = getGroupDisplayName(groupId);
    tabs->addTab(groupTab, groupIcon, groupName);
}

// Last tab: Advanced settings (use=false)
tabs->addTab(createAdvancedTab(), QIcon(":/icons/advanced"), tr("Advanced"));
```

**Create Group Tab:**

```cpp
QWidget* createGroupTab(const QString &groupId) {
    QWidget *widget = new QWidget();
    auto *layout = new QVBoxLayout(widget);

    BDirectiveRegistry *registry = BDirectiveRegistry::instance();
    QList<BDirectiveDefinition> directives = registry->getDirectivesByGroup("director", groupId);

    auto *formLayout = new QFormLayout();

    for (const BDirectiveDefinition &def : directives) {
        // Only show commonly used directives in group tabs
        if (!def.use) continue;

        QWidget *inputWidget = createWidgetForDirective(def);
        formLayout->addRow(def.name + ":", inputWidget);
        m_widgets[def.name] = inputWidget;
    }

    layout->addLayout(formLayout);
    layout->addStretch();

    return widget;
}
```

**Create Widget by Type:**

```cpp
QWidget* createWidgetForDirective(const BDirectiveDefinition &def) {
    if (def.type == "boolean") {
        auto *check = new QCheckBox();
        check->setChecked(def.defaultValue.toBool());
        return check;

    } else if (def.type == "integer") {
        auto *spin = new QSpinBox();
        spin->setRange(def.minInt > 0 ? def.minInt : 0,
                      def.maxInt > 0 ? def.maxInt : 999999);
        spin->setValue(def.defaultValue.toInt());
        return spin;

    } else if (def.type == "file") {
        auto *widget = new QWidget();
        auto *layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);

        auto *edit = new QLineEdit();
        edit->setPlaceholderText(def.example.toString());

        auto *browseBtn = new QPushButton(tr("Browse..."));
        connect(browseBtn, &QPushButton::clicked, [edit]() {
            QString file = QFileDialog::getOpenFileName(nullptr, tr("Select File"));
            if (!file.isEmpty()) edit->setText(file);
        });

        layout->addWidget(edit);
        layout->addWidget(browseBtn);

        return widget;

    } else {
        // Default: string
        auto *edit = new QLineEdit();
        edit->setPlaceholderText(def.example.toString());
        edit->setText(def.defaultValue.toString());
        return edit;
    }
}
```

### 7.2 Conditional Field Visibility

**Update visibility based on other field values:**

```cpp
void updateConditionalDisplay() {
    // Example: Show TLS certificate fields only when needed
    bool tlsEnable = getFieldValue("TLS Enable").toBool();
    bool tlsUsePSK = getFieldValue("TLS Use PSK").toBool();

    bool showCerts = tlsEnable && !tlsUsePSK;

    m_widgets["TLS CA Certificate File"]->setVisible(showCerts);
    m_widgets["TLS Certificate"]->setVisible(showCerts);
    m_widgets["TLS Key"]->setVisible(showCerts);
}

// Connect to field change signals
connect(m_tlsEnableCheck, &QCheckBox::toggled, this, &Dialog::updateConditionalDisplay);
connect(m_tlsUsePSKCheck, &QCheckBox::toggled, this, &Dialog::updateConditionalDisplay);
```

---

## 8. Development Summary

### 8.1 Session 2026-01-31 Accomplishments

**Major Milestones:**

1. ✅ **Connection Wizard Database Integration**
   - Modified: [bconnectionwizard.h](../include/bconnectionwizard.h), [bconnectionwizard.cpp](../src/bconnectionwizard.cpp)
   - Added TemplateSelectionPage (new connection, template, or ZIP import)
   - Implemented `saveToDatabase()` method
   - Template loading from existing Directors in database

2. ✅ **JSON Directive Definition System**
   - Created: [director.json](../resources/directives/director.json) (28 directives)
   - Created: [client.json](../resources/directives/client.json)
   - Created: [console.json](../resources/directives/console.json)
   - Created: [fileset.json](../resources/directives/fileset.json)
   - Created: [directive_schema.json](../resources/directives/directive_schema.json)
   - Created: [directive_groups.json](../resources/directives/directive_groups.json)
   - New fields: `use`, `synonyms`, `group`

3. ✅ **FileSet Templates**
   - Created 7 templates:
     - [windows_domain_controller.json](../resources/templates/filesets/windows_domain_controller.json)
     - [windows_file_server.json](../resources/templates/filesets/windows_file_server.json)
     - [windows_web_server_iis.json](../resources/templates/filesets/windows_web_server_iis.json)
     - [windows_sql_server.json](../resources/templates/filesets/windows_sql_server.json)
     - [linux_postgresql.json](../resources/templates/filesets/linux_postgresql.json)
     - [linux_mysql.json](../resources/templates/filesets/linux_mysql.json)
     - [linux_mongodb.json](../resources/templates/filesets/linux_mongodb.json)

4. ✅ **BDirectiveRegistry Class**
   - Created: [bdirectiveregistry.h](../include/bdirectiveregistry.h), [bdirectiveregistry.cpp](../src/bdirectiveregistry.cpp)
   - Singleton pattern for global access
   - Load directives from JSON resources
   - Validate directive values
   - Query by resource type and backup system
   - Synonym resolution
   - Group-based queries
   - Cross-field validation

5. ✅ **Moved Python Validation to C++**
   - All validation logic now in BDirectiveRegistry
   - No external Python dependency
   - Built-in to application
   - Customer-editable JSON files (not scripts)

### 8.2 File Structure

```
Onesimus/
├── docs/
│   └── ONESIMUS_DOCUMENTATION.md (this file)
├── include/
│   ├── bconnectionwizard.h (modified)
│   ├── bdirectiveregistry.h (new)
│   ├── bdirectormodel.h
│   └── bpasswordutil.h
├── src/
│   ├── bconnectionwizard.cpp (modified)
│   ├── bdirectiveregistry.cpp (new)
│   ├── bdirectormodel.cpp
│   └── bpasswordutil.cpp
├── resources/
│   ├── directives/
│   │   ├── director.json (new)
│   │   ├── client.json (new)
│   │   ├── console.json (new)
│   │   ├── fileset.json (new)
│   │   ├── directive_schema.json (new)
│   │   └── directive_groups.json (new)
│   ├── templates/filesets/
│   │   ├── windows_domain_controller.json (new)
│   │   ├── windows_file_server.json (new)
│   │   ├── windows_web_server_iis.json (new)
│   │   ├── windows_sql_server.json (new)
│   │   ├── linux_postgresql.json (new)
│   │   ├── linux_mysql.json (new)
│   │   └── linux_mongodb.json (new)
│   └── resources.qrc (modified)
└── scripts/
    ├── generate_fileset_templates.py (reference only)
    └── validate_directives.py (deprecated - logic moved to C++)
```

### 8.3 Statistics

- **Files Created:** 20
- **Files Modified:** 5
- **Lines of Code:** ~5,500
- **Documentation:** ~3,000 lines (this file)
- **Directives Defined:** 80+
- **FileSet Templates:** 7
- **Validation Rules:** Built-in to BDirectiveRegistry

### 8.4 Remaining Tasks

1. ⏳ **ZIP Import Validation** - Implement ZIP file import for wizard
2. ⏳ **Integrate Wizard with MainWindow** - Connect wizard to main application
3. ⏳ **Create storage.json and pool.json** - Directive definitions for Storage and Pool resources
4. ⏳ **Implement BClientModel, BStorageModel, BConsoleModel** - Database models for remaining resources
5. ⏳ **Database Migration System** - Version management for schema updates
6. ⏳ **Release Preparation** - Consolidate v1-v4 schemas to v1 before first release

---

## 9. API Reference

### 9.1 BPasswordUtil

**Header:** `bpasswordutil.h`

```cpp
class BPasswordUtil {
public:
    enum class PasswordFormat {
        Plaintext,
        MD5,
        Unknown
    };

    // Hash plaintext password to MD5 with [md5] prefix
    static QString hashPasswordMD5(const QString &plaintext);

    // Detect password format
    static PasswordFormat detectFormat(const QString &password);

    // Extract hash without prefix
    static QString extractHash(const QString &password);

    // Verify plaintext against hash
    static bool verify(const QString &plaintext, const QString &hash);
};
```

### 9.2 BDirectorModel

**Header:** `bdirectormodel.h`

```cpp
class BDirectorModel : public BResourceModel {
public:
    enum Column {
        Id, Name, Address, Port, PasswordHash, TlsEnable, TlsRequire,
        TlsCaCertFile, TlsCertFile, TlsKeyFile, WorkingDirectory,
        MaximumConcurrentJobs, StatisticsRetention, Auditing,
        Description, BackupSystem, IsDefault, IsActive,
        LastConnectedAt, CreatedAt, UpdatedAt
    };

    explicit BDirectorModel(QObject *parent, QSqlDatabase &db);
    bool initialize() override;

    int createDirector(const QString &name, const QString &address, int port,
                      const QString &passwordHash, const QString &backupSystem);

    int findDirectorRow(int directorId) const;
    int directorId(int row) const;
    QString directorName(int row) const;

    void setFilterBackupSystem(const QString &system);
    void setFilterActiveOnly(bool activeOnly);

    bool setCustomSetting(int directorId, const QString &key,
                         const QString &value, const QString &changedBy,
                         const QString &backupSystem);

    QMap<QString, QString> getCustomSettings(int directorId,
                                             const QString &backupSystem) const;
};
```

### 9.3 BDirectiveRegistry

**Header:** `bdirectiveregistry.h`

```cpp
class BDirectiveRegistry : public QObject {
public:
    static BDirectiveRegistry* instance();

    bool isValidDirective(const QString &resourceType, const QString &directiveName) const;
    BDirectiveDefinition getDirective(const QString &resourceType, const QString &directiveName) const;
    QList<BDirectiveDefinition> getDirectives(const QString &resourceType, const QString &backupSystem = QString()) const;
    QStringList getRequiredDirectives(const QString &resourceType) const;
    QStringList getDirectiveNames(const QString &resourceType, const QString &backupSystem = QString()) const;

    bool validateValue(const QString &resourceType, const QString &directiveName,
                      const QString &value, QString *errorMsg = nullptr) const;

    QList<BDirectiveDefinition> getDirectivesByGroup(const QString &resourceType, const QString &groupId) const;
    QStringList getGroups(const QString &resourceType) const;
    QString resolveDirectiveName(const QString &resourceType, const QString &name) const;

    bool validateConfiguration(const QString &resourceType,
                               const QMap<QString, QVariant> &configuration,
                               QStringList *errors = nullptr) const;

    QString lastError() const;
};
```

### 9.4 BConnectionWizard

**Header:** `bconnectionwizard.h`

```cpp
class BConnectionWizard : public QWizard {
public:
    enum PageId {
        Page_Welcome, Page_TemplateSelection, Page_Server, Page_Credentials,
        Page_AuthMethod, Page_TLS, Page_ConfigPreview, Page_Test,
        Page_ConsoleSetup, Page_ProfileName
    };

    explicit BConnectionWizard(BConnectionWizardData *wizardData = nullptr,
                              QSqlDatabase *db = nullptr,
                              QWidget *parent = nullptr);

    BConnectionProfile profile() const;
    void setProfile(const BConnectionProfile &profile);

    int saveToDatabase(QSqlDatabase &db);

    ServerCapabilities capabilities() const;
    void setCapabilities(const ServerCapabilities &caps);

    BConnectionWizardData *wizardData() const;
    QSqlDatabase *database() const;
};

struct BConnectionWizardData {
    QString host;
    int port = 9101;
    QString directorName;
    QString consoleName;
    QString password;  // MD5 hash
    bool savePassword = true;
    QString authMethod = "psk";
    QString tlsCaCertFile;
    QString tlsCertFile;
    QString tlsKeyFile;
    QString profileName;
    bool setAsDefault = true;
    bool connectNow = true;
};
```

---

## Appendix A: JSON Directive Examples

### Director Directive

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
    "description": "Maximum number of concurrent jobs the Director can run",
    "example": 50,
    "group": "performance"
  }
}
```

### Console Directive with ACL

```json
{
  "Command ACL": {
    "type": "string_list",
    "required": false,
    "bareos": true,
    "bacula": true,
    "use": true,
    "description": "Commands this Console can execute",
    "example": ["status", "list", "llist", "query"],
    "valid_values": ["*all*", "status", "list", "llist", "run", "restore", "cancel", "query", "messages", "quit", "exit"],
    "group": "acl"
  }
}
```

### Resource Reference

```json
{
  "Director": {
    "type": "resource_reference",
    "required": false,
    "bareos": true,
    "bacula": true,
    "use": true,
    "reference_type": "Director",
    "description": "Which Director this Console connects to",
    "example": "bareos-dir"
  }
}
```

---

## Appendix B: Validation Rule Examples

### TLS Certificate Complete

```json
{
  "tls_certificate_complete": {
    "description": "When using certificate-based TLS, all certificate fields must be provided",
    "condition": {
      "and": [
        {"TLS Enable": true},
        {"TLS Verify Peer": true}
      ]
    },
    "requires": [
      "TLS CA Certificate File",
      "TLS Certificate",
      "TLS Key"
    ],
    "error_message": "Certificate-based TLS requires CA certificate, client certificate, and private key"
  }
}
```

### PKI Complete

```json
{
  "pki_complete": {
    "description": "PKI encryption requires keypair file",
    "condition": {
      "PKI Encryption": true
    },
    "requires": [
      "PKI Keypair"
    ],
    "error_message": "PKI Encryption requires PKI Keypair file"
  }
}
```

---

**End of Documentation**

**Version:** 0.1.0.5
**Last Updated:** 2026-01-31
**Project:** Onesimus - Bareos/Bacula GUI Configuration Manager
