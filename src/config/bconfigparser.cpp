/**
 * @file bconfigparser.cpp
 * @brief Implementation of Bareos/Bacula configuration parser
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 */

#include "config/bconfigparser.h"
#include <QFile>
#include <QTextStream>
#include <QDirIterator>
#include <QDebug>

// ============================================================================
// BConfigValue Implementation
// ============================================================================

QString BConfigValue::toString() const
{
    switch (m_type) {
    case Simple:
        return m_simpleValue;
    case List:
        return m_listValue.join(", ");
    case Block:
        return QString("[Block with %1 keys]").arg(m_blockValue.size());
    case BlockList:
        return QString("[BlockList with %1 blocks]").arg(m_blockListValue.size());
    }
    return QString();
}

bool BConfigValue::isEmpty() const
{
    switch (m_type) {
    case Simple:
        return m_simpleValue.isEmpty();
    case List:
        return m_listValue.isEmpty();
    case Block:
        return m_blockValue.isEmpty();
    case BlockList:
        return m_blockListValue.isEmpty();
    }
    return true;
}

// ============================================================================
// BConfigResource Implementation
// ============================================================================

void BConfigResource::setValue(const QString &key, const BConfigValue &value)
{
    m_values[key.toLower()] = value;

    // Special handling for Name key
    if (key.compare("name", Qt::CaseInsensitive) == 0 && value.type() == BConfigValue::Simple) {
        m_name = value.simpleValue();
    }
}

void BConfigResource::appendValue(const QString &key, const QString &value)
{
    QString normalizedKey = key.toLower();
    if (m_values.contains(normalizedKey)) {
        BConfigValue &existing = m_values[normalizedKey];
        if (existing.type() == BConfigValue::List) {
            QStringList list = existing.listValue();
            list.append(value);
            m_values[normalizedKey] = BConfigValue(list);
        } else if (existing.type() == BConfigValue::Simple) {
            QStringList list;
            list.append(existing.simpleValue());
            list.append(value);
            m_values[normalizedKey] = BConfigValue(list);
        }
    } else {
        m_values[normalizedKey] = BConfigValue(value);
    }
}

BConfigValue BConfigResource::value(const QString &key) const
{
    return m_values.value(key.toLower());
}

QString BConfigResource::simpleValue(const QString &key, const QString &defaultValue) const
{
    BConfigValue val = value(key);
    if (val.type() == BConfigValue::Simple && !val.isEmpty()) {
        return val.simpleValue();
    }
    return defaultValue;
}

QStringList BConfigResource::listValue(const QString &key) const
{
    BConfigValue val = value(key);
    if (val.type() == BConfigValue::List) {
        return val.listValue();
    } else if (val.type() == BConfigValue::Simple && !val.isEmpty()) {
        return QStringList() << val.simpleValue();
    }
    return QStringList();
}

bool BConfigResource::hasKey(const QString &key) const
{
    return m_values.contains(key.toLower());
}

QStringList BConfigResource::keys() const
{
    return m_values.keys();
}

// ============================================================================
// BConfigParser Implementation
// ============================================================================

BConfigParser::BConfigParser(QObject *parent)
    : QObject(parent)
{
}

void BConfigParser::clear()
{
    m_resources.clear();
    m_lastError.clear();
}

bool BConfigParser::parseString(const QString &content, const QString &sourceName)
{
    return parseContent(content, sourceName);
}

bool BConfigParser::parseFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_lastError = QString("Cannot open file: %1 - %2").arg(filePath, file.errorString());
        emit parsingError(m_lastError);
        return false;
    }

    QTextStream stream(&file);
    QString content = stream.readAll();
    file.close();

    return parseContent(content, filePath);
}

bool BConfigParser::parseDirectory(const QString &dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists()) {
        m_lastError = QString("Directory does not exist: %1").arg(dirPath);
        emit parsingError(m_lastError);
        return false;
    }

    QDirIterator it(dirPath, QStringList() << "*.conf",
                    QDir::Files, QDirIterator::Subdirectories);

    int parsedCount = 0;
    while (it.hasNext()) {
        QString filePath = it.next();
        if (parseFile(filePath)) {
            parsedCount++;
        }
    }

    emit parsingComplete(m_resources.size());
    return parsedCount > 0;
}

bool BConfigParser::parseZipArchive(const QString &zipPath, const QString &basePath)
{
    // TODO: Implement ZIP archive parsing using QuaZip or similar
    // For now, extract to temp directory and parse
    Q_UNUSED(zipPath)
    Q_UNUSED(basePath)
    m_lastError = "ZIP archive parsing not yet implemented";
    emit parsingError(m_lastError);
    return false;
}

QList<BConfigResource> BConfigParser::resourcesByType(const QString &type) const
{
    QList<BConfigResource> result;
    for (const BConfigResource &res : m_resources) {
        if (res.type().compare(type, Qt::CaseInsensitive) == 0) {
            result.append(res);
        }
    }
    return result;
}

BConfigResource BConfigParser::resource(const QString &type, const QString &name) const
{
    for (const BConfigResource &res : m_resources) {
        if (res.type().compare(type, Qt::CaseInsensitive) == 0 &&
            res.name().compare(name, Qt::CaseInsensitive) == 0) {
            return res;
        }
    }
    return BConfigResource();
}

bool BConfigParser::parseContent(const QString &content, const QString &sourceFile)
{
    int pos = 0;
    int length = content.length();

    while (pos < length) {
        skipWhitespaceAndComments(content, pos);
        if (pos >= length) break;

        // Parse resource type
        QString resourceType = parseIdentifier(content, pos);
        if (resourceType.isEmpty()) {
            break;
        }

        skipWhitespaceAndComments(content, pos);

        // Expect opening brace
        if (pos >= length || content[pos] != '{') {
            m_lastError = QString("Expected '{' after resource type '%1' in %2")
                              .arg(resourceType, sourceFile);
            emit parsingError(m_lastError);
            return false;
        }
        pos++; // Skip '{'

        // Parse the resource - this returns a resource with all key-value pairs
        BConfigResource resource = parseResource(content, pos, sourceFile);
        // Set the type (parseResource doesn't know the type)
        resource.setType(resourceType);
        resource.setSourceFile(sourceFile);

        m_resources.append(resource);
        emit resourceParsed(resource.type(), resource.name());
    }

    return true;
}

BConfigResource BConfigParser::parseResource(const QString &content, int &pos, const QString &sourceFile)
{
    Q_UNUSED(sourceFile)
    BConfigResource resource;
    int length = content.length();

    while (pos < length) {
        skipWhitespaceAndComments(content, pos);
        if (pos >= length) break;

        if (content[pos] == '}') {
            pos++;
            break;
        }

        QString key = parseIdentifier(content, pos);
        if (key.isEmpty()) break;

        skipWhitespaceAndComments(content, pos);

        if (pos < length && content[pos] == '=') {
            pos++;
            skipWhitespaceAndComments(content, pos);
            BConfigValue value = parseValue(content, pos);
            // Use appendValue for simple values to handle duplicates (e.g., multiple File = entries)
            if (value.type() == BConfigValue::Simple) {
                resource.appendValue(key, value.simpleValue());
            } else {
                resource.setValue(key, value);
            }
        } else if (pos < length && content[pos] == '{') {
            pos++;
            QMap<QString, BConfigValue> block = parseBlock(content, pos);
            QString normalizedKey = key.toLower();
            BConfigValue existing = resource.value(normalizedKey);
            if (existing.type() == BConfigValue::Block) {
                // Convert single Block to BlockList with both blocks
                QList<QMap<QString, BConfigValue>> blockList;
                blockList.append(existing.blockValue());
                blockList.append(block);
                resource.setValue(key, BConfigValue(blockList));
            } else if (existing.type() == BConfigValue::BlockList) {
                // Append to existing BlockList
                QList<QMap<QString, BConfigValue>> blockList = existing.blockListValue();
                blockList.append(block);
                resource.setValue(key, BConfigValue(blockList));
            } else {
                resource.setValue(key, BConfigValue(block));
            }
        }
    }

    return resource;
}

QMap<QString, BConfigValue> BConfigParser::parseBlock(const QString &content, int &pos)
{
    QMap<QString, BConfigValue> block;
    int length = content.length();

    while (pos < length) {
        skipWhitespaceAndComments(content, pos);
        if (pos >= length) break;

        if (content[pos] == '}') {
            pos++;
            break;
        }

        QString key = parseIdentifier(content, pos);
        if (key.isEmpty()) break;

        skipWhitespaceAndComments(content, pos);

        if (pos < length && content[pos] == '=') {
            pos++;
            skipWhitespaceAndComments(content, pos);
            BConfigValue value = parseValue(content, pos);
            QString normalizedKey = key.toLower();
            if (block.contains(normalizedKey)) {
                // Duplicate key: accumulate into a list
                BConfigValue &existing = block[normalizedKey];
                if (existing.type() == BConfigValue::List && value.type() == BConfigValue::Simple) {
                    QStringList list = existing.listValue();
                    list.append(value.simpleValue());
                    block[normalizedKey] = BConfigValue(list);
                } else if (existing.type() == BConfigValue::Simple && value.type() == BConfigValue::Simple) {
                    block[normalizedKey] = BConfigValue(QStringList{existing.simpleValue(), value.simpleValue()});
                } else {
                    block[normalizedKey] = value;  // Fallback: overwrite
                }
            } else {
                block[normalizedKey] = value;
            }
        } else if (pos < length && content[pos] == '{') {
            pos++;
            QMap<QString, BConfigValue> nestedBlock = parseBlock(content, pos);
            block[key.toLower()] = BConfigValue(nestedBlock);
        }

        // Skip semicolon if present
        if (pos < length && content[pos] == ';') {
            pos++;
        }
    }

    return block;
}

void BConfigParser::skipWhitespaceAndComments(const QString &content, int &pos)
{
    int length = content.length();
    while (pos < length) {
        QChar c = content[pos];

        // Skip whitespace
        if (c.isSpace()) {
            pos++;
            continue;
        }

        // Skip comments (# to end of line)
        if (c == '#') {
            while (pos < length && content[pos] != '\n') {
                pos++;
            }
            if (pos < length) pos++; // Skip newline
            continue;
        }

        break;
    }
}

QString BConfigParser::parseIdentifier(const QString &content, int &pos)
{
    QString result;
    int length = content.length();

    while (pos < length) {
        QChar c = content[pos];

        // Identifier can contain letters, digits, underscores, hyphens, and spaces
        // (spaces are allowed in Bareos key names like "Command ACL")
        if (c.isLetterOrNumber() || c == '_' || c == '-' || c == ' ') {
            result += c;
            pos++;
        } else {
            break;
        }
    }

    return result.trimmed();
}

BConfigValue BConfigParser::parseValue(const QString &content, int &pos)
{
    int length = content.length();
    skipWhitespaceAndComments(content, pos);

    if (pos >= length) {
        return BConfigValue();
    }

    QChar c = content[pos];

    // Quoted string
    if (c == '"') {
        QString value = parseQuotedString(content, pos);
        return BConfigValue(value);
    }

    // Block value (for DirAddresses etc.)
    if (c == '{') {
        pos++; // Skip '{'
        QMap<QString, BConfigValue> block = parseBlock(content, pos);
        return BConfigValue(block);
    }

    // Simple unquoted value - read until newline, semicolon, or closing brace
    QString value;
    while (pos < length) {
        c = content[pos];
        if (c == '\n' || c == ';' || c == '}' || c == '#') {
            break;
        }
        value += c;
        pos++;
    }

    return BConfigValue(value.trimmed());
}

QString BConfigParser::parseQuotedString(const QString &content, int &pos)
{
    QString result;
    int length = content.length();

    if (pos >= length || content[pos] != '"') {
        return result;
    }
    pos++; // Skip opening quote

    while (pos < length) {
        QChar c = content[pos];

        if (c == '"') {
            pos++; // Skip closing quote
            break;
        }

        if (c == '\\' && pos + 1 < length) {
            pos++;
            QChar escaped = content[pos];
            if (escaped == 'n') {
                result += '\n';
            } else if (escaped == 't') {
                result += '\t';
            } else if (escaped == '\\') {
                result += '\\';
            } else if (escaped == '"') {
                result += '"';
            } else {
                result += escaped;
            }
            pos++;
        } else {
            result += c;
            pos++;
        }
    }

    return result;
}

QString BConfigParser::normalizeKey(const QString &key) const
{
    return key.toLower().remove(' ').remove('_').remove('-');
}
