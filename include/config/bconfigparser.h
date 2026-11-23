// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

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
 * The tokens are those of the Bareos 25 lexer (core/src/lib/lex.cc
 * LexGetToken()): a directive name drops its blanks and is compared without
 * case ("FD Port" is "FDPort"), ';' ends a directive like a newline, '#'
 * starts a comment, a backslash in a quoted string takes the next character
 * as it is, a quoted string followed by another one (on the same or the
 * next line) continues it, ',' separates list items, '@file' includes a
 * file. Resources follow core/src/lib/parse_conf_state_machine.cc; as the
 * parser does not know the type of each directive, a value is its first
 * token or its comma-separated list (core/src/lib/res.cc StoreStr(),
 * StoreAlistStr()), and blank-separated parts after a leading number are
 * joined as StoreTime()/StoreSize() join them ("30 days" is "30days").
 * Repeated directives and repeated blocks are kept as lists. Blocks may be
 * nested MaxNestingDepth levels deep.
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
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2025
 */

class BConfigResource;
class BConfigLexer;

/**
 * @class BConfigValue
 * @brief Represents a configuration value which can be simple, a list, or a nested block
 */
class BConfigValue
{
public:
    enum Type { Simple, List, Block, BlockList };

    BConfigValue() : m_type(Simple) {}
    explicit BConfigValue(const QString &value) : m_type(Simple), m_simpleValue(value) {}
    explicit BConfigValue(const QStringList &list) : m_type(List), m_listValue(list) {}
    explicit BConfigValue(const QMap<QString, BConfigValue> &block) : m_type(Block), m_blockValue(block) {}
    explicit BConfigValue(const QList<QMap<QString, BConfigValue>> &blockList) : m_type(BlockList), m_blockListValue(blockList) {}

    Type type() const { return m_type; }
    QString simpleValue() const { return m_simpleValue; }
    QStringList listValue() const { return m_listValue; }
    QMap<QString, BConfigValue> blockValue() const { return m_blockValue; }
    QList<QMap<QString, BConfigValue>> blockListValue() const { return m_blockListValue; }

    QString toString() const;
    bool isEmpty() const;

private:
    Type m_type;
    QString m_simpleValue;
    QStringList m_listValue;
    QMap<QString, BConfigValue> m_blockValue;
    QList<QMap<QString, BConfigValue>> m_blockListValue;
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

    /// Keys are compared as Bareos compares directive names: without case
    /// and blanks ("TLS Enable", "tlsenable"); keys() keeps the first spelling
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
    /// The key under which @p key is stored, or its lower-case form if none
    QString storedKey(const QString &key) const;

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
    /// Deepest nesting of blocks, the resource body counting as level 1
    static constexpr int MaxNestingDepth = 32;
    /// Deepest nesting of @include files (parseFile() only)
    static constexpr int MaxIncludeDepth = 32;
    /// Most files one parseFile() reads through @include
    static constexpr int MaxIncludedFiles = 1000;
    /// Most characters of configuration text one parse reads, includes counted
    static constexpr qsizetype MaxTextSize = 16 * 1024 * 1024;

    explicit BConfigParser(QObject *parent = nullptr);

    /// @p key as Bareos compares directive names: lower case, without blanks
    static QString normalizeKey(const QString &key);

    /**
     * @brief Parse configuration from a string
     * @param content The configuration text to parse
     * @param sourceName Virtual source name for error reporting
     * @return true on success, false on error
     */
    bool parseString(const QString &content, const QString &sourceName = "string");

    /**
     * @brief Parse a single configuration file
     *
     * The only parse that follows @include: absolute paths and wildcards
     * as Bareos reads them (lex_open_file(): a wildcard without a match is
     * no error, a missing file is). A command ("@|...") is never run and a
     * relative path is not followed; both are reported in skippedEntries().
     * A file that includes itself, directly or through others, is an error,
     * as are more than MaxIncludeDepth levels, more than MaxIncludedFiles
     * files or more than MaxTextSize characters.
     * @param filePath Path to the .conf file
     * @return true on success, false on error
     */
    bool parseFile(const QString &filePath);

    /**
     * @brief Parse an entire configuration directory tree
     *
     * Every *.conf file is parsed on its own; @include is not followed (the
     * included files are usually in the tree as well) and is reported in
     * skippedEntries().
     * @param dirPath Path to the configuration directory (e.g., /etc/bareos/bareos-dir.d/)
     * @return true on success, false on error
     */
    bool parseDirectory(const QString &dirPath);

    /**
     * @brief Parse the configuration files of a ZIP archive
     *
     * The files are read into memory by BZipImport, within its limits (size,
     * number of entries, no symbolic links, no path leaving the archive);
     * nothing is extracted. Defined in bzipimport.cpp, so that only the
     * targets that import archives need QZipReader.
     * @param zipPath Path to the ZIP file
     * @param basePath Optional directory within the ZIP; only files below it are parsed
     * @return true if at least one resource was parsed; skippedEntries()
     *         says which entries were refused
     */
    bool parseZipArchive(const QString &zipPath, const QString &basePath = QString());

    /**
     * @brief What was not read: "path: reason" for the archive entries the
     *        last parseZipArchive() refused, and "file:line: ..." for each
     *        @include not followed and each resource left out
     *
     * @include is only followed by parseFile(); parseString(),
     * parseDirectory() and parseZipArchive() report it here. Cleared by
     * clear() and at the start of parseZipArchive().
     */
    QStringList skippedEntries() const { return m_skippedEntries; }

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
     * @param followIncludes true to read @include files (parseFile() only)
     */
    bool parseContent(const QString &content, const QString &sourceFile, bool followIncludes = false);

    /// Reads a file as Bareos does (UTF-8, no UTF-16), at most @p maxSize characters
    bool readConfigFile(const QString &filePath, qsizetype maxSize, QString *text, QString *error) const;

    /// Directives and blocks up to the closing '}' of a block at @p depth
    bool parseBody(BConfigLexer &lex, QMap<QString, BConfigValue> &body, int depth);

    /// The value after '=': a block, one token or a comma-separated list
    bool parseValue(BConfigLexer &lex, const QString &key, int depth, BConfigValue *value);

    /// Sets m_lastError and emits parsingError()
    bool fail(const QString &message);

    QList<BConfigResource> m_resources;
    QString m_lastError;
    QStringList m_skippedEntries;
};

#endif // BCONFIGPARSER_H
