/**
 * @file bdirectiveschema.cpp
 * @brief Implementation of directive schema loader
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 */

#include "config/bdirectiveschema.h"
#include "blogging.h"
#include <QFile>
#include <QJsonDocument>
#include <QDir>

BDirectiveSchema &BDirectiveSchema::instance()
{
    static BDirectiveSchema instance;
    return instance;
}

BDirectiveSchema::BDirectiveSchema(QObject *parent)
    : QObject(parent)
{
}

BDirectiveSchema::~BDirectiveSchema()
{
}

bool BDirectiveSchema::loadSchemas()
{
    if (m_loaded) return true;

    QStringList resources = {
        ":/directives/directives/director.json",
        ":/directives/directives/console.json",
        ":/directives/directives/client.json",
        ":/directives/directives/job.json",
        ":/directives/directives/jobdef.json",
        ":/directives/directives/storage.json",
        ":/directives/directives/fileset.json",
        ":/directives/directives/pool.json",
        ":/directives/directives/catalog.json",
        ":/directives/directives/schedule.json",
        ":/directives/directives/messages.json"
    };

    bool allLoaded = true;
    for (const QString &resource : resources) {
        if (!loadSchema(resource)) {
            BLOG_WARNING() << "Failed to load schema:" << resource;
            allLoaded = false;
        }
    }

    m_loaded = allLoaded;
    return allLoaded;
}

bool BDirectiveSchema::loadSchema(const QString &resourcePath)
{
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly)) {
        BLOG_WARNING() << "Cannot open resource:" << resourcePath << file.errorString();
        return false;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();

    if (error.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "JSON parse error in" << resourcePath << ":" << error.errorString();
        return false;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "Invalid schema format in" << resourcePath;
        return false;
    }

    QJsonObject root = doc.object();
    QString resourceType = root["resource_type"].toString();
    QString description = root["description"].toString();

    if (resourceType.isEmpty()) {
        BLOG_WARNING() << "Missing resource_type in" << resourcePath;
        return false;
    }

    m_resourceDescriptions[resourceType] = description;

    QJsonObject directives = root["directives"].toObject();
    QMap<QString, BDirective> directiveMap;

    for (auto it = directives.begin(); it != directives.end(); ++it) {
        QString name = it.key();
        QJsonObject def = it.value().toObject();

        BDirective directive;
        directive.name = name;
        directive.type = def["type"].toString();
        directive.required = def["required"].toBool(false);
        directive.bareos = def["bareos"].toBool(true);
        directive.bacula = def["bacula"].toBool(true);
        directive.description = def["description"].toString();
        directive.example = def["example"].toString();
        directive.referenceType = def["reference_type"].toString();

        // Default value
        if (def.contains("default")) {
            QJsonValue defaultVal = def["default"];
            if (defaultVal.isDouble()) {
                directive.defaultValue = defaultVal.toInt();
            } else if (defaultVal.isBool()) {
                directive.defaultValue = defaultVal.toBool();
            } else {
                directive.defaultValue = defaultVal.toString();
            }
        }

        // Min/max for integers
        if (def.contains("min")) {
            directive.minValue = def["min"].toInt();
        }
        if (def.contains("max")) {
            directive.maxValue = def["max"].toInt();
        }

        // Excludes and must arrays
        QJsonArray excludesArr = def["excludes"].toArray();
        for (const QJsonValue &v : excludesArr) {
            directive.excludes.append(v.toString());
        }

        QJsonArray mustArr = def["must"].toArray();
        for (const QJsonValue &v : mustArr) {
            directive.must.append(v.toString());
        }

        // Extended fields
        directive.group = def["group"].toString();
        directive.use = def["use"].toBool(true);
        directive.minVersion = def["min_version"].toString();

        QJsonArray validArr = def["valid_values"].toArray();
        for (const QJsonValue &v : validArr) {
            directive.validValues.append(v.toString());
        }

        QJsonArray synArr = def["synonyms"].toArray();
        for (const QJsonValue &v : synArr) {
            directive.synonyms.append(v.toString());
        }

        // Platform restrictions
        QJsonArray platformArr = def["platform"].toArray();
        for (const QJsonValue &v : platformArr) {
            directive.platforms.append(v.toString().toLower());
        }

        // Conditional visibility
        if (def.contains("visible_when")) {
            QJsonObject vw = def["visible_when"].toObject();
            directive.visibleWhen.directive = vw["directive"].toString();
            if (vw.contains("value")) {
                QJsonValue val = vw["value"];
                if (val.isBool())
                    directive.visibleWhen.value = val.toBool();
                else if (val.isDouble())
                    directive.visibleWhen.value = val.toInt();
                else
                    directive.visibleWhen.value = val.toString();
            }
            if (vw.contains("values")) {
                QJsonArray valArr = vw["values"].toArray();
                for (const QJsonValue &v : valArr) {
                    if (v.isBool())
                        directive.visibleWhen.values.append(v.toBool());
                    else if (v.isDouble())
                        directive.visibleWhen.values.append(v.toInt());
                    else
                        directive.visibleWhen.values.append(v.toString());
                }
            }
        }

        directiveMap[name.toLower()] = directive;
    }

    m_schemas[resourceType] = directiveMap;
    BLOG_DEBUG() << "Loaded schema for" << resourceType << "with" << directiveMap.size() << "directives";
    return true;
}

QMap<QString, BDirective> BDirectiveSchema::directives(const QString &resourceType) const
{
    return m_schemas.value(resourceType);
}

BDirective BDirectiveSchema::directive(const QString &resourceType, const QString &directiveName) const
{
    return m_schemas.value(resourceType).value(directiveName.toLower());
}

bool BDirectiveSchema::isRequired(const QString &resourceType, const QString &directiveName) const
{
    return directive(resourceType, directiveName).required;
}

QString BDirectiveSchema::description(const QString &resourceType, const QString &directiveName) const
{
    return directive(resourceType, directiveName).description;
}

QVariant BDirectiveSchema::defaultValue(const QString &resourceType, const QString &directiveName) const
{
    return directive(resourceType, directiveName).defaultValue;
}

QStringList BDirectiveSchema::resourceTypes() const
{
    return m_schemas.keys();
}

QString BDirectiveSchema::resourceDescription(const QString &resourceType) const
{
    return m_resourceDescriptions.value(resourceType);
}

QString BDirectiveSchema::validate(const QString &resourceType, const QString &directiveName,
                                   const QVariant &value) const
{
    BDirective dir = directive(resourceType, directiveName);
    if (!dir.isValid()) {
        return QString(); // Unknown directive, skip validation
    }

    // Check required
    if (dir.required && !value.isValid()) {
        return tr("Required directive '%1' is missing").arg(directiveName);
    }

    if (!value.isValid()) {
        return QString(); // Optional and not provided, OK
    }

    // Type-specific validation
    if (dir.type == "integer") {
        bool ok;
        int intVal = value.toInt(&ok);
        if (!ok) {
            return tr("'%1' must be an integer").arg(directiveName);
        }
        if (dir.minValue != 0 && intVal < dir.minValue) {
            return tr("'%1' must be >= %2").arg(directiveName).arg(dir.minValue);
        }
        if (dir.maxValue != 0 && intVal > dir.maxValue) {
            return tr("'%1' must be <= %2").arg(directiveName).arg(dir.maxValue);
        }
    }

    // TODO: Add more type validations (path, directory, boolean, etc.)

    return QString(); // Valid
}

bool BDirective::appliesToCurrentPlatform() const
{
    // Empty platform list means applies to all platforms
    if (platforms.isEmpty())
        return true;

    // Check for "all"
    if (platforms.contains("all"))
        return true;

    // Determine current platform
#ifdef Q_OS_WIN
    QString currentPlatform = "windows";
#elif defined(Q_OS_MACOS)
    QString currentPlatform = "darwin";
#elif defined(Q_OS_LINUX)
    QString currentPlatform = "linux";
#elif defined(Q_OS_FREEBSD)
    QString currentPlatform = "freebsd";
#else
    QString currentPlatform = "unknown";
#endif

    return platforms.contains(currentPlatform);
}
