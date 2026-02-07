#ifndef BDIRECTIVEREGISTRY_H
#define BDIRECTIVEREGISTRY_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>
#include <QStringList>

/**
 * @brief Directive definition loaded from JSON
 *
 * Represents a single Bareos/Bacula configuration directive
 * with its type, constraints, and compatibility information.
 */
struct BDirectiveDefinition
{
    QString name;                   ///< Directive name (e.g., "Maximum Concurrent Jobs")
    QString type;                   ///< Data type: "integer", "string", "boolean", "time", "size", "password", etc.
    bool required = false;          ///< Is this directive required?
    bool bareos = true;             ///< Compatible with Bareos?
    bool bacula = true;             ///< Compatible with Bacula?
    bool use = true;                ///< Show by default (true) or in Advanced mode (false)
    QString group;                  ///< Logical group (tls, authentication, network, etc.)
    QStringList synonyms;           ///< Alternative names for this directive
    QString minVersion;             ///< Minimum version required (e.g., "18.2.0")
    QString description;            ///< Human-readable description
    QVariant defaultValue;          ///< Default value
    QVariant example;               ///< Example value

    // Type-specific constraints
    int minInt = 0;                 ///< Minimum value for integer types
    int maxInt = 0;                 ///< Maximum value for integer types
    QStringList validValues;        ///< Valid values for enum types
    QString referenceType;          ///< Referenced resource type (for resource_reference)

    // Cross-field validation
    QStringList excludes;           ///< Directives that cannot be used together with this one (mutually exclusive)
    QStringList must;               ///< Directives that are required when this one is used (dependencies)

    /**
     * @brief Check if directive is compatible with a backup system
     * @param system "bareos", "bacula", or "both"
     * @return true if compatible
     */
    bool isCompatibleWith(const QString &system) const
    {
        if (system == "both") {
            return bareos && bacula;
        } else if (system == "bareos") {
            return bareos;
        } else if (system == "bacula") {
            return bacula;
        }
        return false;
    }

    /**
     * @brief Get compatibility string
     * @return "bareos", "bacula", or "both"
     */
    QString compatibilityString() const
    {
        if (bareos && bacula) return "both";
        if (bareos) return "bareos";
        if (bacula) return "bacula";
        return "none";
    }
};

/**
 * @brief Registry of Bareos/Bacula directive definitions
 *
 * Loads directive definitions from JSON resource files and provides
 * validation, querying, and UI generation capabilities.
 *
 * JSON files are loaded from Qt resources:
 * - :/directives/director.json
 * - :/directives/client.json
 * - :/directives/console.json
 * - :/directives/fileset.json
 * - :/directives/schedule.json
 * - :/directives/storage.json
 * - :/directives/pool.json
 *
 * @since 0.1.0.5
 */
class BDirectiveRegistry : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Get singleton instance
     * @return Global directive registry
     */
    static BDirectiveRegistry* instance();

    /**
     * @brief Load directive definitions from JSON resources
     * @return true if successful
     */
    bool loadDirectives();

    /**
     * @brief Check if a directive is valid for a resource type
     * @param resourceType "director", "client", "storage", or "console"
     * @param directiveName Directive name (case-insensitive)
     * @return true if valid
     */
    bool isValidDirective(const QString &resourceType, const QString &directiveName) const;

    /**
     * @brief Get directive definition
     * @param resourceType Resource type
     * @param directiveName Directive name (case-insensitive)
     * @return Directive definition, or empty struct if not found
     */
    BDirectiveDefinition getDirective(const QString &resourceType, const QString &directiveName) const;

    /**
     * @brief Get all directives for a resource type
     * @param resourceType Resource type
     * @param backupSystem Filter by system: "bareos", "bacula", or "both" (default: all)
     * @return List of directive definitions
     */
    QList<BDirectiveDefinition> getDirectives(const QString &resourceType,
                                              const QString &backupSystem = QString()) const;

    /**
     * @brief Get required directives for a resource type
     * @param resourceType Resource type
     * @return List of required directive names
     */
    QStringList getRequiredDirectives(const QString &resourceType) const;

    /**
     * @brief Get directive names for autocomplete
     * @param resourceType Resource type
     * @param backupSystem Filter by system (optional)
     * @return List of directive names
     */
    QStringList getDirectiveNames(const QString &resourceType,
                                  const QString &backupSystem = QString()) const;

    /**
     * @brief Validate a directive value
     * @param resourceType Resource type
     * @param directiveName Directive name
     * @param value Value to validate
     * @param errorMsg Output: error message if validation fails
     * @return true if valid
     */
    bool validateValue(const QString &resourceType,
                      const QString &directiveName,
                      const QString &value,
                      QString *errorMsg = nullptr) const;

    /**
     * @brief Get last error message
     * @return Error description
     */
    QString lastError() const { return m_lastError; }

    /**
     * @brief Get directives by group
     * @param resourceType Resource type
     * @param groupId Group identifier (tls, authentication, network, etc.)
     * @return List of directives in that group
     */
    QList<BDirectiveDefinition> getDirectivesByGroup(const QString &resourceType,
                                                     const QString &groupId) const;

    /**
     * @brief Get all available groups for a resource type
     * @param resourceType Resource type
     * @return List of group IDs
     */
    QStringList getGroups(const QString &resourceType) const;

    /**
     * @brief Resolve directive name (including synonyms)
     * @param resourceType Resource type
     * @param name Directive name or synonym
     * @return Canonical directive name, or empty if not found
     */
    QString resolveDirectiveName(const QString &resourceType, const QString &name) const;

    /**
     * @brief Validate resource configuration (cross-field validation)
     * @param resourceType Resource type
     * @param configuration Map of directive names to values
     * @param errors Output: list of validation errors
     * @return true if all validation rules pass
     */
    bool validateConfiguration(const QString &resourceType,
                               const QMap<QString, QVariant> &configuration,
                               QStringList *errors = nullptr) const;

    /**
     * @brief Set current language for directive descriptions
     * @param locale Language code (e.g., "de", "en", "fr")
     */
    void setLanguage(const QString &locale);

    /**
     * @brief Get current language
     * @return Current locale code
     */
    QString currentLanguage() const { return m_currentLocale; }

    /**
     * @brief Get localized description for a directive
     * @param resourceType Resource type
     * @param directiveName Directive name
     * @return Localized description, or English fallback if not available
     */
    QString getLocalizedDescription(const QString &resourceType,
                                    const QString &directiveName) const;

    /**
     * @brief Get localized group name
     * @param groupId Group identifier
     * @return Localized group name
     */
    QString getLocalizedGroupName(const QString &groupId) const;

    /**
     * @brief Get localized resource type name
     * @param resourceType Resource type
     * @return Localized resource type name
     */
    QString getLocalizedResourceType(const QString &resourceType) const;

private:
    explicit BDirectiveRegistry(QObject *parent = nullptr);
    ~BDirectiveRegistry() override = default;

    // Prevent copying
    BDirectiveRegistry(const BDirectiveRegistry&) = delete;
    BDirectiveRegistry& operator=(const BDirectiveRegistry&) = delete;

    /**
     * @brief Load directives from a JSON resource file
     * @param resourcePath Path to JSON file (e.g., ":/directives/director.json")
     * @param resourceType Resource type key
     * @return true if successful
     */
    bool loadDirectivesFromJson(const QString &resourcePath, const QString &resourceType);

    /**
     * @brief Parse directive definition from JSON object
     * @param name Directive name
     * @param obj JSON object with directive properties
     * @return Parsed directive definition
     */
    BDirectiveDefinition parseDirective(const QString &name, const QVariantMap &obj) const;

    /**
     * @brief Normalize directive name (lowercase, trim)
     * @param name Directive name
     * @return Normalized name
     */
    QString normalizeDirectiveName(const QString &name) const;

private:
    /**
     * @brief Load directive groups and validation rules
     * @return true if successful
     */
    bool loadDirectiveGroups();

    /**
     * @brief Load translations for a specific locale
     * @param locale Language code (e.g., "de", "en")
     */
    void loadTranslations(const QString &locale);

private:
    // Map: resource_type -> (directive_name_lowercase -> definition)
    QMap<QString, QMap<QString, BDirectiveDefinition>> m_directives;

    // Map: resource_type -> (synonym_lowercase -> canonical_name_lowercase)
    QMap<QString, QMap<QString, QString>> m_synonyms;

    // Map: resource_type -> Set of group IDs
    QMap<QString, QStringList> m_groups;

    // Validation rules (loaded from directive_groups.json)
    QVariantMap m_validationRules;

    // Localization
    QString m_currentLocale = "en";

    // Map: resource_type -> directive_name -> localized_strings
    QMap<QString, QMap<QString, QVariantMap>> m_translations;

    // Map: group_id -> localized_name
    QMap<QString, QString> m_groupTranslations;

    // Map: resource_type -> localized_name
    QMap<QString, QString> m_resourceTypeTranslations;

    QString m_lastError;

    static BDirectiveRegistry *s_instance;
};

#endif // BDIRECTIVEREGISTRY_H
