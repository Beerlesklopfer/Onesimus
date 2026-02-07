#include "bdirectiveregistry.h"
#include "blogging.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

BDirectiveRegistry *BDirectiveRegistry::s_instance = nullptr;

BDirectiveRegistry::BDirectiveRegistry(QObject *parent)
    : QObject(parent)
{
}

BDirectiveRegistry* BDirectiveRegistry::instance()
{
    if (!s_instance) {
        s_instance = new BDirectiveRegistry();
        s_instance->loadDirectives();
    }
    return s_instance;
}

bool BDirectiveRegistry::loadDirectives()
{
    m_directives.clear();
    m_synonyms.clear();
    m_groups.clear();
    m_lastError.clear();

    // Load directive definitions from JSON resources
    bool success = true;
    success &= loadDirectivesFromJson(":/directives/director.json", "director");
    success &= loadDirectivesFromJson(":/directives/client.json", "client");
    success &= loadDirectivesFromJson(":/directives/console.json", "console");
    success &= loadDirectivesFromJson(":/directives/fileset.json", "fileset");
    success &= loadDirectivesFromJson(":/directives/schedule.json", "schedule");
    success &= loadDirectivesFromJson(":/directives/storage.json", "storage");
    success &= loadDirectivesFromJson(":/directives/pool.json", "pool");
    success &= loadDirectivesFromJson(":/directives/messages.json", "messages");

    // Load groups and validation rules
    success &= loadDirectiveGroups();

    // Load default language (English)
    loadTranslations("en");

    if (success) {
        BLOG_DEBUG() << "[DirectiveRegistry] Loaded" << m_directives.size() << "resource types";
    }

    return success;
}

bool BDirectiveRegistry::loadDirectivesFromJson(const QString &resourcePath, const QString &resourceType)
{
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly)) {
        m_lastError = tr("Failed to open %1: %2").arg(resourcePath, file.errorString());
        BLOG_WARNING() << m_lastError;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        m_lastError = tr("JSON parse error in %1: %2")
                          .arg(resourcePath, parseError.errorString());
        BLOG_WARNING() << m_lastError;
        return false;
    }

    if (!doc.isObject()) {
        m_lastError = tr("Invalid JSON format in %1: expected object").arg(resourcePath);
        BLOG_WARNING() << m_lastError;
        return false;
    }

    QJsonObject root = doc.object();
    QJsonObject directives = root["directives"].toObject();

    QMap<QString, BDirectiveDefinition> resourceDirectives;
    QMap<QString, QString> resourceSynonyms;
    QStringList resourceGroups;

    for (auto it = directives.constBegin(); it != directives.constEnd(); ++it) {
        QString name = it.key();
        QJsonObject obj = it.value().toObject();

        BDirectiveDefinition def = parseDirective(name, obj.toVariantMap());
        QString normalizedName = normalizeDirectiveName(name);

        resourceDirectives[normalizedName] = def;

        // Build synonym map
        for (const QString &synonym : def.synonyms) {
            QString normalizedSynonym = normalizeDirectiveName(synonym);
            resourceSynonyms[normalizedSynonym] = normalizedName;
        }

        // Collect groups
        if (!def.group.isEmpty() && !resourceGroups.contains(def.group)) {
            resourceGroups.append(def.group);
        }
    }

    m_directives[resourceType] = resourceDirectives;
    m_synonyms[resourceType] = resourceSynonyms;
    m_groups[resourceType] = resourceGroups;

    BLOG_DEBUG() << "[DirectiveRegistry] Loaded" << resourceDirectives.size()
             << "directives for" << resourceType;

    return true;
}

BDirectiveDefinition BDirectiveRegistry::parseDirective(const QString &name, const QVariantMap &obj) const
{
    BDirectiveDefinition def;
    def.name = name;
    def.type = obj.value("type").toString();
    def.required = obj.value("required", false).toBool();
    def.bareos = obj.value("bareos", true).toBool();
    def.bacula = obj.value("bacula", true).toBool();
    def.use = obj.value("use", true).toBool();  // Show by default
    def.group = obj.value("group").toString();
    def.minVersion = obj.value("min_version").toString();
    def.description = obj.value("description").toString();
    def.defaultValue = obj.value("default");
    def.example = obj.value("example");

    // Synonyms (alternative names)
    if (obj.contains("synonyms")) {
        QVariantList syns = obj.value("synonyms").toList();
        for (const QVariant &syn : syns) {
            def.synonyms.append(syn.toString());
        }
    }

    // Cross-field validation arrays
    if (obj.contains("excludes")) {
        QVariantList excludesList = obj.value("excludes").toList();
        for (const QVariant &exclude : excludesList) {
            def.excludes.append(exclude.toString());
        }
    }
    if (obj.contains("must")) {
        QVariantList mustList = obj.value("must").toList();
        for (const QVariant &mustItem : mustList) {
            def.must.append(mustItem.toString());
        }
    }

    // Type-specific constraints
    if (obj.contains("min")) {
        def.minInt = obj.value("min").toInt();
    }
    if (obj.contains("max")) {
        def.maxInt = obj.value("max").toInt();
    }
    if (obj.contains("valid_values")) {
        def.validValues = obj.value("valid_values").toStringList();
    }
    if (obj.contains("reference_type")) {
        def.referenceType = obj.value("reference_type").toString();
    }

    return def;
}

QString BDirectiveRegistry::normalizeDirectiveName(const QString &name) const
{
    return name.trimmed().toLower();
}

bool BDirectiveRegistry::isValidDirective(const QString &resourceType, const QString &directiveName) const
{
    QString normalized = normalizeDirectiveName(directiveName);

    if (!m_directives.contains(resourceType)) {
        return false;
    }

    return m_directives[resourceType].contains(normalized);
}

BDirectiveDefinition BDirectiveRegistry::getDirective(const QString &resourceType,
                                                      const QString &directiveName) const
{
    QString normalized = normalizeDirectiveName(directiveName);

    if (!m_directives.contains(resourceType)) {
        return BDirectiveDefinition();
    }

    return m_directives[resourceType].value(normalized);
}

QList<BDirectiveDefinition> BDirectiveRegistry::getDirectives(const QString &resourceType,
                                                              const QString &backupSystem) const
{
    QList<BDirectiveDefinition> result;

    if (!m_directives.contains(resourceType)) {
        return result;
    }

    const QMap<QString, BDirectiveDefinition> &directives = m_directives[resourceType];

    for (const BDirectiveDefinition &def : directives) {
        // Filter by backup system if specified
        if (!backupSystem.isEmpty() && !def.isCompatibleWith(backupSystem)) {
            continue;
        }

        result.append(def);
    }

    return result;
}

QStringList BDirectiveRegistry::getRequiredDirectives(const QString &resourceType) const
{
    QStringList result;

    if (!m_directives.contains(resourceType)) {
        return result;
    }

    const QMap<QString, BDirectiveDefinition> &directives = m_directives[resourceType];

    for (const BDirectiveDefinition &def : directives) {
        if (def.required) {
            result.append(def.name);
        }
    }

    return result;
}

QStringList BDirectiveRegistry::getDirectiveNames(const QString &resourceType,
                                                  const QString &backupSystem) const
{
    QStringList result;

    QList<BDirectiveDefinition> directives = getDirectives(resourceType, backupSystem);

    for (const BDirectiveDefinition &def : directives) {
        result.append(def.name);
    }

    result.sort(Qt::CaseInsensitive);

    return result;
}

bool BDirectiveRegistry::validateValue(const QString &resourceType,
                                       const QString &directiveName,
                                       const QString &value,
                                       QString *errorMsg) const
{
    BDirectiveDefinition def = getDirective(resourceType, directiveName);

    if (def.name.isEmpty()) {
        if (errorMsg) {
            *errorMsg = tr("Unknown directive: %1").arg(directiveName);
        }
        return false;
    }

    // Basic validation based on type
    if (def.type == "integer") {
        bool ok;
        int intValue = value.toInt(&ok);

        if (!ok) {
            if (errorMsg) {
                *errorMsg = tr("Invalid integer value: %1").arg(value);
            }
            return false;
        }

        if (def.minInt > 0 && intValue < def.minInt) {
            if (errorMsg) {
                *errorMsg = tr("Value %1 is less than minimum %2").arg(intValue).arg(def.minInt);
            }
            return false;
        }

        if (def.maxInt > 0 && intValue > def.maxInt) {
            if (errorMsg) {
                *errorMsg = tr("Value %1 exceeds maximum %2").arg(intValue).arg(def.maxInt);
            }
            return false;
        }
    } else if (def.type == "boolean") {
        QString lower = value.toLower();
        if (lower != "yes" && lower != "no" && lower != "true" && lower != "false" &&
            lower != "1" && lower != "0") {
            if (errorMsg) {
                *errorMsg = tr("Invalid boolean value: %1 (expected yes/no)").arg(value);
            }
            return false;
        }
    }

    // Enum validation
    if (!def.validValues.isEmpty()) {
        if (!def.validValues.contains(value, Qt::CaseInsensitive)) {
            if (errorMsg) {
                *errorMsg = tr("Invalid value: %1 (expected one of: %2)")
                                .arg(value, def.validValues.join(", "));
            }
            return false;
        }
    }

    return true;
}

bool BDirectiveRegistry::loadDirectiveGroups()
{
    // Load directive groups and validation rules from directive_groups.json
    QFile file(":/directives/directive_groups.json");
    if (!file.open(QIODevice::ReadOnly)) {
        BLOG_WARNING() << "[DirectiveRegistry] Failed to load directive_groups.json (optional)";
        return true;  // Not critical
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "[DirectiveRegistry] Parse error in directive_groups.json:" << parseError.errorString();
        return true;  // Not critical
    }

    if (doc.isObject()) {
        QJsonObject root = doc.object();

        // Load validation rules
        if (root.contains("validation_rules")) {
            m_validationRules = root["validation_rules"].toObject().toVariantMap();
            BLOG_DEBUG() << "[DirectiveRegistry] Loaded" << m_validationRules.size() << "validation rules";
        }
    }

    return true;
}

QList<BDirectiveDefinition> BDirectiveRegistry::getDirectivesByGroup(const QString &resourceType,
                                                                     const QString &groupId) const
{
    QList<BDirectiveDefinition> result;

    if (!m_directives.contains(resourceType)) {
        return result;
    }

    const QMap<QString, BDirectiveDefinition> &directives = m_directives[resourceType];

    for (const BDirectiveDefinition &def : directives) {
        if (def.group == groupId) {
            result.append(def);
        }
    }

    return result;
}

QStringList BDirectiveRegistry::getGroups(const QString &resourceType) const
{
    return m_groups.value(resourceType);
}

QString BDirectiveRegistry::resolveDirectiveName(const QString &resourceType, const QString &name) const
{
    QString normalized = normalizeDirectiveName(name);

    // Check if it's a synonym
    if (m_synonyms.contains(resourceType)) {
        const QMap<QString, QString> &synonyms = m_synonyms[resourceType];
        if (synonyms.contains(normalized)) {
            // Return canonical name
            QString canonical = synonyms[normalized];
            // Look up the actual directive to get the proper case
            if (m_directives[resourceType].contains(canonical)) {
                return m_directives[resourceType][canonical].name;
            }
        }
    }

    // Check if it's already a canonical name
    if (m_directives.contains(resourceType) && m_directives[resourceType].contains(normalized)) {
        return m_directives[resourceType][normalized].name;
    }

    return QString();  // Not found
}

bool BDirectiveRegistry::validateConfiguration(const QString &resourceType,
                                               const QMap<QString, QVariant> &configuration,
                                               QStringList *errors) const
{
    bool allValid = true;

    // Check required directives
    QStringList required = getRequiredDirectives(resourceType);
    for (const QString &reqName : required) {
        QString normalized = normalizeDirectiveName(reqName);
        bool found = false;

        // Check if directive exists in configuration
        for (auto it = configuration.constBegin(); it != configuration.constEnd(); ++it) {
            if (normalizeDirectiveName(it.key()) == normalized) {
                found = true;
                break;
            }
        }

        if (!found) {
            allValid = false;
            if (errors) {
                errors->append(tr("Required directive missing: %1").arg(reqName));
            }
        }
    }

    // Validate each provided directive
    for (auto it = configuration.constBegin(); it != configuration.constEnd(); ++it) {
        QString directiveName = it.key();
        QString value = it.value().toString();

        QString errorMsg;
        if (!validateValue(resourceType, directiveName, value, &errorMsg)) {
            allValid = false;
            if (errors) {
                errors->append(tr("%1: %2").arg(directiveName, errorMsg));
            }
        }
    }

    // Check "excludes" and "must" constraints for cross-field validation
    for (auto it = configuration.constBegin(); it != configuration.constEnd(); ++it) {
        QString directiveName = it.key();
        BDirectiveDefinition def = getDirective(resourceType, directiveName);

        if (def.name.isEmpty()) {
            continue;  // Unknown directive, already reported above
        }

        // Check "excludes" - directives that cannot be used together
        for (const QString &excludedName : def.excludes) {
            QString normalizedExcluded = normalizeDirectiveName(excludedName);

            // Check if excluded directive exists in configuration
            for (auto confIt = configuration.constBegin(); confIt != configuration.constEnd(); ++confIt) {
                if (normalizeDirectiveName(confIt.key()) == normalizedExcluded) {
                    allValid = false;
                    if (errors) {
                        errors->append(tr("%1 cannot be used together with %2 (mutually exclusive)")
                                          .arg(directiveName, excludedName));
                    }
                    break;
                }
            }
        }

        // Check "must" - directives that are required when this directive is used
        for (const QString &requiredName : def.must) {
            QString normalizedRequired = normalizeDirectiveName(requiredName);
            bool found = false;

            // Check if required directive exists in configuration
            for (auto confIt = configuration.constBegin(); confIt != configuration.constEnd(); ++confIt) {
                if (normalizeDirectiveName(confIt.key()) == normalizedRequired) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                allValid = false;
                if (errors) {
                    errors->append(tr("%1 requires %2 to be set")
                                      .arg(directiveName, requiredName));
                }
            }
        }
    }

    return allValid;
}

void BDirectiveRegistry::setLanguage(const QString &locale)
{
    if (m_currentLocale == locale) {
        return;  // Already loaded
    }

    m_currentLocale = locale;
    loadTranslations(locale);

    BLOG_DEBUG() << "[DirectiveRegistry] Language changed to:" << locale;
}

void BDirectiveRegistry::loadTranslations(const QString &locale)
{
    m_translations.clear();
    m_groupTranslations.clear();
    m_resourceTypeTranslations.clear();

    QStringList resourceTypes = {"director", "client", "console", "fileset", "schedule", "storage", "pool", "messages"};

    for (const QString &resourceType : resourceTypes) {
        QString path = QString(":/translations/directives/%1_%2.json")
                          .arg(resourceType, locale);

        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            // No translation file for this resource type in this locale
            // English descriptions are in the base directive files, so no fallback needed
            BLOG_DEBUG() << "[DirectiveRegistry] No translation for" << resourceType
                     << "in" << locale << "(using base descriptions)";
            continue;
        }

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
        file.close();

        if (parseError.error != QJsonParseError::NoError) {
            BLOG_WARNING() << "[DirectiveRegistry] Parse error in translation file:" << parseError.errorString();
            continue;
        }

        if (!doc.isObject()) continue;

        QJsonObject root = doc.object();

        // Load resource type translation
        if (root.contains("resource_type")) {
            m_resourceTypeTranslations[resourceType] = root["resource_type"].toString();
        }

        // Load directive translations
        if (root.contains("directives")) {
            QJsonObject directives = root["directives"].toObject();

            QMap<QString, QVariantMap> resourceTranslations;

            for (auto it = directives.constBegin(); it != directives.constEnd(); ++it) {
                QString directiveName = it.key();
                QString normalized = normalizeDirectiveName(directiveName);
                QVariantMap translation = it.value().toObject().toVariantMap();
                resourceTranslations[normalized] = translation;
            }

            m_translations[resourceType] = resourceTranslations;
        }

        // Load group translations
        if (root.contains("groups")) {
            QJsonObject groups = root["groups"].toObject();
            for (auto it = groups.constBegin(); it != groups.constEnd(); ++it) {
                m_groupTranslations[it.key()] = it.value().toString();
            }
        }
    }

    BLOG_DEBUG() << "[DirectiveRegistry] Loaded translations for locale:" << locale;
}

QString BDirectiveRegistry::getLocalizedDescription(const QString &resourceType,
                                                    const QString &directiveName) const
{
    QString normalized = normalizeDirectiveName(directiveName);

    // Try to get localized description
    if (m_translations.contains(resourceType)) {
        const QMap<QString, QVariantMap> &resourceTranslations = m_translations[resourceType];

        if (resourceTranslations.contains(normalized)) {
            QVariantMap translation = resourceTranslations[normalized];
            if (translation.contains("description")) {
                return translation.value("description").toString();
            }
        }
    }

    // Fallback to directive definition description (from JSON)
    BDirectiveDefinition def = getDirective(resourceType, directiveName);
    if (!def.description.isEmpty()) {
        return def.description;
    }

    // Last resort: return directive name
    return directiveName;
}

QString BDirectiveRegistry::getLocalizedGroupName(const QString &groupId) const
{
    if (m_groupTranslations.contains(groupId)) {
        return m_groupTranslations[groupId];
    }

    // Fallback: capitalize group ID
    if (!groupId.isEmpty()) {
        return groupId.left(1).toUpper() + groupId.mid(1);
    }

    return groupId;
}

QString BDirectiveRegistry::getLocalizedResourceType(const QString &resourceType) const
{
    if (m_resourceTypeTranslations.contains(resourceType)) {
        return m_resourceTypeTranslations[resourceType];
    }

    // Fallback: capitalize resource type
    if (!resourceType.isEmpty()) {
        return resourceType.left(1).toUpper() + resourceType.mid(1);
    }

    return resourceType;
}
