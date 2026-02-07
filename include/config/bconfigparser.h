#ifndef BCONFIGPARSER_H
#define BCONFIGPARSER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVariant>
#include <QDir>

/**
 * @file bconfigparser.h
 * @brief Parser for Bareos/Bacula configuration files
 *
 * Parses the Bareos/Bacula configuration file format which consists of
 * resource blocks containing key-value pairs and nested sub-blocks.
 *
 * Example format:
 * @code
 * Director {
 *   Name = PDC-dir
 *   Messages = Daemon
 *   DirAddresses = {
 *     ipv4 = { addr = 192.168.1.1; port = 9101; }
 *   }
 * }
 * @endcode
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 */

class BConfigResource;

/**
 * @class BConfigValue
 * @brief Represents a configuration value which can be simple, a list, or a nested block
 */
class BConfigValue
{
public:
    enum Type { Simple, List, Block };

    BConfigValue() : m_type(Simple) {}
    explicit BConfigValue(const QString &value) : m_type(Simple), m_simpleValue(value) {}
    explicit BConfigValue(const QStringList &list) : m_type(List), m_listValue(list) {}
    explicit BConfigValue(const QMap<QString, BConfigValue> &block) : m_type(Block), m_blockValue(block) {}

    Type type() const { return m_type; }
    QString simpleValue() const { return m_simpleValue; }
    QStringList listValue() const { return m_listValue; }
    QMap<QString, BConfigValue> blockValue() const { return m_blockValue; }

    QString toString() const;
    bool isEmpty() const;

private:
    Type m_type;
    QString m_simpleValue;
    QStringList m_listValue;
    QMap<QString, BConfigValue> m_blockValue;
};

/**
 * @class BConfigResource
 * @brief Represents a parsed configuration resource (Director, Client, Job, etc.)
 */
class BConfigResource
{
public:
    BConfigResource() {}
    BConfigResource(const QString &type, const QString &name = QString())
        : m_type(type), m_name(name) {}

    QString type() const { return m_type; }
    void setType(const QString &type) { m_type = type; }
    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    void setValue(const QString &key, const BConfigValue &value);
    void appendValue(const QString &key, const QString &value);
    BConfigValue value(const QString &key) const;
    QString simpleValue(const QString &key, const QString &defaultValue = QString()) const;
    QStringList listValue(const QString &key) const;
    bool hasKey(const QString &key) const;
    QStringList keys() const;

    QString sourceFile() const { return m_sourceFile; }
    void setSourceFile(const QString &file) { m_sourceFile = file; }

private:
    QString m_type;
    QString m_name;
    QString m_sourceFile;
    QMap<QString, BConfigValue> m_values;
};

/**
 * @class BConfigParser
 * @brief Parses Bareos/Bacula configuration files and directory trees
 */
class BConfigParser : public QObject
{
    Q_OBJECT

public:
    explicit BConfigParser(QObject *parent = nullptr);

    /**
     * @brief Parse configuration from a string
     * @param content The configuration text to parse
     * @param sourceName Virtual source name for error reporting
     * @return true on success, false on error
     */
    bool parseString(const QString &content, const QString &sourceName = "string");

    /**
     * @brief Parse a single configuration file
     * @param filePath Path to the .conf file
     * @return true on success, false on error
     */
    bool parseFile(const QString &filePath);

    /**
     * @brief Parse an entire configuration directory tree
     * @param dirPath Path to the configuration directory (e.g., /etc/bareos/bareos-dir.d/)
     * @return true on success, false on error
     */
    bool parseDirectory(const QString &dirPath);

    /**
     * @brief Parse configuration from a ZIP archive
     * @param zipPath Path to the ZIP file
     * @param basePath Optional base path within the ZIP
     * @return true on success, false on error
     */
    bool parseZipArchive(const QString &zipPath, const QString &basePath = QString());

    /**
     * @brief Get all parsed resources
     */
    QList<BConfigResource> resources() const { return m_resources; }

    /**
     * @brief Get resources of a specific type
     * @param type Resource type (e.g., "Director", "Client", "Job")
     */
    QList<BConfigResource> resourcesByType(const QString &type) const;

    /**
     * @brief Get a specific resource by type and name
     */
    BConfigResource resource(const QString &type, const QString &name) const;

    /**
     * @brief Get the last error message
     */
    QString lastError() const { return m_lastError; }

    /**
     * @brief Clear all parsed resources
     */
    void clear();

signals:
    /**
     * @brief Emitted when a resource is parsed
     */
    void resourceParsed(const QString &type, const QString &name);

    /**
     * @brief Emitted when parsing is complete
     */
    void parsingComplete(int resourceCount);

    /**
     * @brief Emitted on parsing error
     */
    void parsingError(const QString &error);

private:
    /**
     * @brief Parse configuration content
     * @param content The configuration file content
     * @param sourceFile Source file name for error reporting
     */
    bool parseContent(const QString &content, const QString &sourceFile);

    /**
     * @brief Parse a resource block
     * @param content Content starting at resource type
     * @param pos Current position (updated on return)
     * @param sourceFile Source file name
     */
    BConfigResource parseResource(const QString &content, int &pos, const QString &sourceFile);

    /**
     * @brief Parse a nested block
     */
    QMap<QString, BConfigValue> parseBlock(const QString &content, int &pos);

    /**
     * @brief Skip whitespace and comments
     */
    void skipWhitespaceAndComments(const QString &content, int &pos);

    /**
     * @brief Parse an identifier (key name or resource type)
     */
    QString parseIdentifier(const QString &content, int &pos);

    /**
     * @brief Parse a value (simple, quoted, or block)
     */
    BConfigValue parseValue(const QString &content, int &pos);

    /**
     * @brief Parse a quoted string
     */
    QString parseQuotedString(const QString &content, int &pos);

    /**
     * @brief Normalize a key name (lowercase, remove spaces)
     */
    QString normalizeKey(const QString &key) const;

    QList<BConfigResource> m_resources;
    QString m_lastError;
};

#endif // BCONFIGPARSER_H
