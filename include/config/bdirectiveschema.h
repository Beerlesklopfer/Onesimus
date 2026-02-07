#ifndef BDIRECTIVESCHEMA_H
#define BDIRECTIVESCHEMA_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>

/**
 * @file bdirectiveschema.h
 * @brief Schema for Bareos/Bacula resource directives loaded from JSON
 *
 * This class loads directive schemas from Qt resources and provides
 * validation and metadata for resource directives.
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 */

/**
 * @struct BVisibleWhen
 * @brief Conditional visibility for a directive
 */
struct BVisibleWhen
{
    QString directive;      ///< Name of controlling directive
    QVariant value;         ///< Single value that makes this visible
    QVariantList values;    ///< List of values that make this visible

    bool isValid() const { return !directive.isEmpty(); }
};

/**
 * @struct BDirective
 * @brief Represents a single directive definition
 */
struct BDirective
{
    QString name;
    QString type;
    bool required = false;
    bool bareos = true;
    bool bacula = true;
    QString description;
    QVariant defaultValue;
    QString example;
    QStringList excludes;
    QStringList must;
    QString referenceType;
    int minValue = 0;
    int maxValue = 0;
    QString group;
    bool use = true;
    QStringList validValues;
    QStringList synonyms;
    QString minVersion;
    QStringList platforms;      ///< Platforms where directive applies (empty = all)
    BVisibleWhen visibleWhen;   ///< Conditional visibility

    bool isValid() const { return !name.isEmpty(); }

    /**
     * @brief Check if directive applies to current platform
     * @return true if platforms is empty or contains current platform
     */
    bool appliesToCurrentPlatform() const;
};

/**
 * @class BDirectiveSchema
 * @brief Loads and provides access to directive schemas for all resource types
 */
class BDirectiveSchema : public QObject
{
    Q_OBJECT

public:
    static BDirectiveSchema &instance();

    /**
     * @brief Load all directive schemas from Qt resources
     * @return true if all schemas loaded successfully
     */
    bool loadSchemas();

    /**
     * @brief Get all directives for a resource type
     * @param resourceType Resource type (e.g., "Director", "Client", "Job")
     * @return Map of directive name to BDirective
     */
    QMap<QString, BDirective> directives(const QString &resourceType) const;

    /**
     * @brief Get a specific directive definition
     * @param resourceType Resource type
     * @param directiveName Directive name
     * @return BDirective or invalid directive if not found
     */
    BDirective directive(const QString &resourceType, const QString &directiveName) const;

    /**
     * @brief Check if a directive is required
     */
    bool isRequired(const QString &resourceType, const QString &directiveName) const;

    /**
     * @brief Get directive description
     */
    QString description(const QString &resourceType, const QString &directiveName) const;

    /**
     * @brief Get default value for a directive
     */
    QVariant defaultValue(const QString &resourceType, const QString &directiveName) const;

    /**
     * @brief Get all available resource types
     */
    QStringList resourceTypes() const;

    /**
     * @brief Get resource description
     */
    QString resourceDescription(const QString &resourceType) const;

    /**
     * @brief Validate a resource value
     * @param resourceType Resource type
     * @param directiveName Directive name
     * @param value The value to validate
     * @return Empty string if valid, error message if invalid
     */
    QString validate(const QString &resourceType, const QString &directiveName,
                     const QVariant &value) const;

private:
    BDirectiveSchema(QObject *parent = nullptr);
    ~BDirectiveSchema();

    bool loadSchema(const QString &resourcePath);

    QMap<QString, QMap<QString, BDirective>> m_schemas;
    QMap<QString, QString> m_resourceDescriptions;
    bool m_loaded = false;
};

#endif // BDIRECTIVESCHEMA_H
