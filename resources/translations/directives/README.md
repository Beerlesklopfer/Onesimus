# Directive Translation System

This directory contains localized descriptions for Bareos/Bacula directives.

## File Structure

```
translations/directives/
├── director_en.json    # English (default)
├── director_de.json    # German (Deutsch)
├── client_en.json      # Client directives (English)
├── client_de.json      # Client directives (German)
└── ...
```

## Translation File Format

Each translation file contains:

```json
{
  "resource_type": "Localized Resource Type Name",
  "resource_type_description": "Description of this resource type",
  "directives": {
    "Directive Name": {
      "description": "Localized description of what this directive does",
      "example_description": "Explanation of the example value"
    }
  },
  "groups": {
    "group_id": "Localized Group Name"
  }
}
```

## Usage in Code

### Set Language

```cpp
BDirectiveRegistry *registry = BDirectiveRegistry::instance();

// Set language to German
registry->setLanguage("de");

// Set language to English
registry->setLanguage("en");
```

### Get Localized Description

```cpp
BDirectiveRegistry *registry = BDirectiveRegistry::instance();

// Get localized description for a directive
QString description = registry->getLocalizedDescription("director", "TLS Enable");
// Returns: "TLS-Verschlüsselung für Director-Kommunikation aktivieren" (if locale is "de")
// Returns: "Enable TLS encryption for Director communication" (if locale is "en")

// Get localized group name
QString groupName = registry->getLocalizedGroupName("tls");
// Returns: "TLS / Sicherheit" (German)
// Returns: "TLS / Security" (English)
```

### UI Integration

```cpp
void BDirectorEditDialog::createTabs() {
    BDirectiveRegistry *registry = BDirectiveRegistry::instance();

    // Set language based on user preference
    QSettings settings;
    QString locale = settings.value("Language", "en").toString();
    registry->setLanguage(locale);

    // Create tabs with localized group names
    QStringList groupIds = registry->getGroups("director");

    for (const QString &groupId : groupIds) {
        QString localizedName = registry->getLocalizedGroupName(groupId);
        QWidget *tab = createGroupTab(groupId);
        m_tabs->addTab(tab, localizedName);
    }
}

void BDirectorEditDialog::createFormField(const BDirectiveDefinition &def) {
    // Get localized description
    QString description = registry->getLocalizedDescription("director", def.name);

    // Create label with tooltip
    QLabel *label = new QLabel(def.name + ":");
    label->setToolTip(description);  // Show localized description as tooltip

    // Create input widget...
}
```

## Adding New Languages

To add a new language (e.g., French):

1. **Create translation file**: `director_fr.json`

```json
{
  "resource_type": "Director",
  "directives": {
    "TLS Enable": {
      "description": "Activer le chiffrement TLS pour la communication du Director"
    },
    "Maximum Concurrent Jobs": {
      "description": "Nombre maximum de jobs simultanés que le Director peut exécuter"
    }
  },
  "groups": {
    "tls": "Configuration TLS",
    "authentication": "Authentification",
    "network": "Configuration réseau"
  }
}
```

2. **Add to resources.qrc**:

```xml
<qresource prefix="/translations/directives">
    <file>translations/directives/director_fr.json</file>
</qresource>
```

3. **Use in application**:

```cpp
registry->setLanguage("fr");
```

## Fallback Mechanism

If a translation is missing:

1. **Tries requested locale** (e.g., "de")
2. **Falls back to English** ("en") if not found
3. **Falls back to directive definition** (from directive JSON)
4. **Last resort**: Returns directive name itself

Example:
```cpp
// German translation exists
registry->setLanguage("de");
registry->getLocalizedDescription("director", "TLS Enable");
// → "TLS-Verschlüsselung für Director-Kommunikation aktivieren"

// French translation doesn't exist yet
registry->setLanguage("fr");
registry->getLocalizedDescription("director", "TLS Enable");
// → Falls back to English: "Enable TLS encryption for Director communication"
```

## Translation Guidelines

1. **Technical Accuracy** - Maintain technical precision
2. **Consistency** - Use consistent terminology across all translations
3. **Brevity** - Keep descriptions concise
4. **Context** - Provide enough context for users to understand the directive
5. **Examples** - Translate example descriptions to help users

## Common Terms

| English | German (Deutsch) | Notes |
|---------|------------------|-------|
| Director | Director | Keep as-is (proper noun) |
| Client | Client | Keep as-is |
| FileSet | FileSet | Keep as-is |
| TLS | TLS | Acronym, keep as-is |
| Backup | Backup / Sicherung | Use "Backup" in technical context |
| Certificate | Zertifikat | |
| Encryption | Verschlüsselung | |
| Authentication | Authentifizierung | |

## Customer Customization

Customers can customize translations by:

1. Editing the JSON files in `resources/translations/directives/`
2. Recompiling the application (translations are embedded in Qt resources)

Alternatively, for runtime customization:
- Store translation files externally
- Load from file system instead of resources
- Implement custom translation loader in BDirectiveRegistry

---

**Note**: Translation files are compiled into the application via Qt's resource system. After modifying translations, rebuild the project to see changes.
