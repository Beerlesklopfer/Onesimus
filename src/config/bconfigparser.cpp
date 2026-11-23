// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

/**
 * @file bconfigparser.cpp
 * @brief Implementation of Bareos/Bacula configuration parser
 *
 * The lexer is a port of LexGetToken() of Bareos 25 (core/src/lib/lex.cc,
 * Release/25.1.2); the resource level follows
 * core/src/lib/parse_conf_state_machine.cc. Comments name the Bareos
 * function each part follows.
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2025
 */

#include "config/bconfigparser.h"
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QDirIterator>

#include <functional>

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
// Keys and repeated values
// ============================================================================

namespace {

/// The key of @p map that names the same directive as @p key, or an empty string
QString findKey(const QMap<QString, BConfigValue> &map, const QString &key)
{
    const QString lower = key.toLower();
    if (map.contains(lower))
        return lower;
    const QString normalized = BConfigParser::normalizeKey(key);
    for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
        if (BConfigParser::normalizeKey(it.key()) == normalized)
            return it.key();
    }
    return QString();
}

/**
 * @brief @p value added to what @p existing already holds
 *
 * A repeated directive adds its items (StoreAlistStr() appends to the list;
 * so it is for Command ACL, File, Run); a repeated block becomes a list of
 * blocks (several Options in an Include, several ipv4 in DirAddresses). A
 * value and a block under the same name: the later one counts.
 */
BConfigValue merged(const BConfigValue &existing, const BConfigValue &value)
{
    const auto items = [](const BConfigValue &v) {
        return v.type() == BConfigValue::List ? v.listValue() : QStringList{v.simpleValue()};
    };
    const auto blocks = [](const BConfigValue &v) {
        return v.type() == BConfigValue::BlockList ? v.blockListValue()
                                                   : QList<QMap<QString, BConfigValue>>{v.blockValue()};
    };
    const bool existingIsBlock = existing.type() == BConfigValue::Block || existing.type() == BConfigValue::BlockList;
    const bool valueIsBlock = value.type() == BConfigValue::Block || value.type() == BConfigValue::BlockList;
    if (existingIsBlock != valueIsBlock)
        return value;
    if (valueIsBlock)
        return BConfigValue(blocks(existing) + blocks(value));
    return BConfigValue(items(existing) + items(value));
}

void insertMerged(QMap<QString, BConfigValue> &map, const QString &key, const BConfigValue &value)
{
    const QString stored = findKey(map, key);
    if (stored.isEmpty())
        map.insert(key.toLower(), value);
    else
        map[stored] = merged(map.value(stored), value);
}

QString tooLarge(const QString &where)
{
    return QCoreApplication::translate("BConfigParser", "%1: more than %2 MiB of configuration text")
        .arg(where)
        .arg(BConfigParser::MaxTextSize / (1024 * 1024));
}

} // namespace

// ============================================================================
// BConfigResource Implementation
// ============================================================================

QString BConfigResource::storedKey(const QString &key) const
{
    const QString stored = findKey(m_values, key);
    return stored.isEmpty() ? key.toLower() : stored;
}

void BConfigResource::setValue(const QString &key, const BConfigValue &value)
{
    m_values[storedKey(key)] = value;

    // Special handling for Name key
    if (key.compare("name", Qt::CaseInsensitive) == 0 && value.type() == BConfigValue::Simple) {
        m_name = value.simpleValue();
    }
}

void BConfigResource::appendValue(const QString &key, const QString &value)
{
    const QString normalizedKey = storedKey(key);
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
    const QString stored = findKey(m_values, key);
    return stored.isEmpty() ? BConfigValue() : m_values.value(stored);
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
    return !findKey(m_values, key).isEmpty();
}

QStringList BConfigResource::keys() const
{
    return m_values.keys();
}

// ============================================================================
// Lexer: port of LexGetToken() (core/src/lib/lex.cc)
// ============================================================================

/// One configuration text on the include stack
struct BConfigSource {
    QString text;
    QString name;       ///< For messages: file name or the name given to parseString()
    QString canonical;  ///< Canonical path of a file; empty for other text
    int pos = 0;
    int line = 1;
};

class BConfigLexer
{
public:
    enum class Token { Eof, Eol, Bob, Eob, Equals, Comma, Identifier, Unquoted, Quoted, Number, Error };

    /**
     * Called for "@spec"; appends the files to read to @p files (none to
     * read nothing) or returns false with @p error. @p from is the source
     * holding the include, @p openFiles the canonical paths of the files
     * being read, @p depth the number of includes open.
     */
    using IncludeHandler = std::function<bool(const QString &spec, const BConfigSource &from,
                                              const QStringList &openFiles, int depth,
                                              QList<BConfigSource> *files, QString *error)>;

    BConfigLexer(const QString &text, const QString &name, const QString &canonical, IncludeHandler handler)
        : m_handler(std::move(handler))
    {
        m_stack.append(source(text, name, canonical));
    }

    /// A source ready for the stack: no byte order mark, ends with a newline
    static BConfigSource source(QString text, const QString &name, const QString &canonical)
    {
        // A UTF-8 byte order mark is accepted at the very start only (BCT_UTF8_BOM)
        if (text.startsWith(QChar(0xFEFF)))
            text.remove(0, 1);
        // Every line ends with L_EOL before L_EOF (LexGetChar())
        if (!text.endsWith(QLatin1Char('\n')))
            text += QLatin1Char('\n');
        BConfigSource s;
        s.text = text;
        s.name = name;
        s.canonical = canonical;
        return s;
    }

    /// The text of the last token
    QString str;
    /// The reason of the last Token::Error
    QString error;

    /// "file:line" of the current position
    QString where() const
    {
        const BConfigSource &s = m_stack.last();
        return QStringLiteral("%1:%2").arg(s.name).arg(s.line);
    }

    /// The next token after any EOL (BCT_SKIP_EOL)
    Token nextSkipEol()
    {
        Token t;
        do {
            t = next();
        } while (t == Token::Eol);
        return t;
    }

    /// Puts @p token (with the text in str) back; the next call returns it again
    void pushBack(Token token)
    {
        m_pushedBack = true;
        m_pushedToken = token;
        m_pushedStr = str;
    }

    /// The next token (LexGetToken(lf, BCT_ALL))
    Token next()
    {
        if (m_pushedBack) {
            m_pushedBack = false;
            str = m_pushedStr;
            return m_pushedToken;
        }
        str.clear();
        enum class State { None, Comment, Number, String, Identifier, Quoted, Include, IncludeQuoted };
        State state = State::None;
        bool escNext = false;

        while (true) {
            const int ch = get();
            switch (state) {
            case State::None:
                if (ch == Eof)
                    return Token::Eof;
                if (ch == Eol)
                    return Token::Eol;
                if (isSpace(ch))
                    break;
                if (isAlpha(ch)) {
                    state = State::Identifier;
                    str += QChar(ch);
                    break;
                }
                if (isDigit(ch)) {
                    state = State::Number;
                    str += QChar(ch);
                    break;
                }
                switch (ch) {
                case '#': state = State::Comment; break;
                case '{': str = QStringLiteral("{"); return Token::Bob;
                case '}': str = QStringLiteral("}"); return Token::Eob;
                case '"': state = State::Quoted; break;
                case '=': str = QStringLiteral("="); return Token::Equals;
                case ',': str = QStringLiteral(","); return Token::Comma;
                case ';': return Token::Eol;   // treat ; like EOL
                case '@': state = State::Include; break;
                default:
                    state = State::String;
                    str += QChar(ch);
                    break;
                }
                break;

            case State::Comment:
                if (ch == Eol)
                    return Token::Eol;
                if (ch == Eof)
                    return Token::Eof;
                break;

            case State::Number:
                if (isDigit(ch)) {
                    str += QChar(ch);
                    break;
                }
                // A valid number can be terminated by the following
                if (isSpace(ch) || ch == Eol || ch == Eof || ch == ',' || ch == ';') {
                    unget();
                    return Token::Number;
                }
                state = State::String;
                unget();
                break;

            case State::String:
                if (ch == Eol || ch == Eof || ch == '=' || ch == '}' || ch == '{' || ch == '\r' || ch == ';'
                    || ch == ',' || ch == '#' || isSpace(ch) || ch == '"') {
                    unget();
                    return Token::Unquoted;
                }
                str += QChar(ch);
                break;

            case State::Identifier:
                if (isAlpha(ch)) {
                    str += QChar(ch);
                    break;
                }
                if (isSpace(ch))
                    break;   // blanks inside a name are dropped: "FD Port" is "FDPort"
                if (ch == Eol || ch == Eof || ch == '=' || ch == '}' || ch == '{' || ch == ';' || ch == ','
                    || ch == '"' || ch == '#') {
                    unget();
                    return Token::Identifier;
                }
                // Some non-alpha character => string
                state = State::String;
                str += QChar(ch);
                break;

            case State::Quoted:
                if (ch == Eof) {
                    error = QCoreApplication::translate("BConfigParser", "%1: the quoted string has no closing quote").arg(where());
                    return Token::Error;
                }
                if (ch == Eol) {
                    str += QLatin1Char('\n');
                    escNext = false;
                    break;
                }
                if (escNext) {
                    str += QChar(ch);
                    escNext = false;
                    break;
                }
                if (ch == '\\') {
                    escNext = true;
                    break;
                }
                if (ch == '"') {
                    // "a" "b", or "a" and "b" at the start of the next line:
                    // one string (CurrentLineContinuesWithQuotes(),
                    // NextLineContinuesWithQuotes())
                    if (continuesWithQuote())
                        break;
                    return Token::Quoted;
                }
                str += QChar(ch);
                break;

            case State::Include:
                if (ch == Eof) {
                    error = QCoreApplication::translate("BConfigParser", "%1: the file name after @ is missing").arg(where());
                    return Token::Error;
                }
                if (ch == '"' && str.isEmpty()) {
                    state = State::IncludeQuoted;
                    break;
                }
                if (isSpace(ch) || ch == Eol || ch == '}' || ch == '{' || ch == ';' || ch == ',' || ch == '"'
                    || ch == '#') {
                    unget();
                    if (!include(str))
                        return Token::Error;
                    str.clear();
                    state = State::None;
                    break;
                }
                str += QChar(ch);
                break;

            case State::IncludeQuoted:
                if (ch == Eof) {
                    error = QCoreApplication::translate("BConfigParser", "%1: the quoted string has no closing quote").arg(where());
                    return Token::Error;
                }
                if (escNext) {
                    str += ch == Eol ? QChar('\n') : QChar(ch);
                    escNext = false;
                    break;
                }
                if (ch == '\\') {
                    escNext = true;
                    break;
                }
                if (ch == '"') {
                    if (!include(str))
                        return Token::Error;
                    str.clear();
                    state = State::None;
                    break;
                }
                str += ch == Eol ? QChar('\n') : QChar(ch);
                break;
            }
        }
    }

private:
    static constexpr int Eof = -1;
    static constexpr int Eol = -2;

    // B_ISSPACE, B_ISALPHA, B_ISDIGIT: ASCII only; a newline is Eol
    static bool isSpace(int ch) { return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\v' || ch == '\f'; }
    static bool isAlpha(int ch) { return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'); }
    static bool isDigit(int ch) { return ch >= '0' && ch <= '9'; }

    /// The next character; Eol for a newline, Eof at the end of the outer text
    int get()
    {
        while (true) {
            BConfigSource &s = m_stack.last();
            if (s.pos < s.text.size()) {
                const QChar c = s.text.at(s.pos++);
                if (c == QLatin1Char('\n')) {
                    ++s.line;
                    return Eol;
                }
                return c.unicode();
            }
            if (m_stack.size() == 1) {
                s.pos = int(s.text.size()) + 1;   // unget() after Eof stays at the end
                return Eof;
            }
            // End of an included file: on in the file that included it
            m_stack.removeLast();
        }
    }

    void unget()
    {
        BConfigSource &s = m_stack.last();
        if (s.pos > 0) {
            --s.pos;
            if (s.pos < s.text.size() && s.text.at(s.pos) == QLatin1Char('\n'))
                --s.line;
        }
    }

    /// After a closing quote: blanks and another quote on this line, or only
    /// blanks left and the next line starting with blanks and a quote. If
    /// so, everything up to and including that quote is consumed.
    bool continuesWithQuote()
    {
        BConfigSource &s = m_stack.last();
        int p = s.pos;
        const int n = int(s.text.size());
        const auto skipBlanks = [&]() {
            while (p < n && (s.text.at(p) == QLatin1Char(' ') || s.text.at(p) == QLatin1Char('\t')))
                ++p;
        };
        skipBlanks();
        int lines = 0;
        if (p < n && s.text.at(p) == QLatin1Char('\n')) {
            ++p;
            lines = 1;
            skipBlanks();
        }
        if (p < n && s.text.at(p) == QLatin1Char('"')) {
            s.pos = p + 1;
            s.line += lines;
            return true;
        }
        return false;
    }

    bool include(const QString &spec)
    {
        QStringList open;
        for (const BConfigSource &s : std::as_const(m_stack)) {
            if (!s.canonical.isEmpty())
                open << s.canonical;
        }
        QList<BConfigSource> files;
        if (!m_handler(spec, m_stack.last(), open, int(m_stack.size()) - 1, &files, &error))
            return false;
        // Pushed in order, so the last one is read first: lex_open_file()
        // pushes each match of a wildcard in front of the others
        for (const BConfigSource &f : std::as_const(files))
            m_stack.append(f);
        return true;
    }

    IncludeHandler m_handler;
    QList<BConfigSource> m_stack;
    bool m_pushedBack = false;
    Token m_pushedToken = Token::Eof;
    QString m_pushedStr;
};

// ============================================================================
// BConfigParser Implementation
// ============================================================================

BConfigParser::BConfigParser(QObject *parent)
    : QObject(parent)
{
}

QString BConfigParser::normalizeKey(const QString &key)
{
    QString out;
    out.reserve(key.size());
    for (const QChar c : key) {
        if (!c.isSpace())
            out += c.toLower();
    }
    return out;
}

void BConfigParser::clear()
{
    m_resources.clear();
    m_lastError.clear();
    m_skippedEntries.clear();
}

bool BConfigParser::fail(const QString &message)
{
    m_lastError = message;
    emit parsingError(m_lastError);
    return false;
}

bool BConfigParser::parseString(const QString &content, const QString &sourceName)
{
    return parseContent(content, sourceName);
}

bool BConfigParser::readConfigFile(const QString &filePath, qsizetype maxSize, QString *text, QString *error) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        *error = QString("Cannot open file: %1 - %2").arg(filePath, file.errorString());
        return false;
    }
    // A UTF-8 byte is at most one character: within the limit in bytes is
    // within it in characters
    const QByteArray bytes = file.read(maxSize + 1);
    if (bytes.size() > maxSize) {
        *error = tooLarge(filePath);
        return false;
    }
    // Bareos reads UTF-8 only (BCT_UTF16_BOM is an error)
    if (bytes.startsWith("\xFF\xFE") || bytes.startsWith("\xFE\xFF")) {
        *error = tr("%1: UTF-16 files cannot be read; save the file as UTF-8").arg(filePath);
        return false;
    }
    *text = QString::fromUtf8(bytes);
    return true;
}

bool BConfigParser::parseFile(const QString &filePath)
{
    QString content, error;
    if (!readConfigFile(filePath, MaxTextSize, &content, &error))
        return fail(error);
    return parseContent(content, filePath, true);
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
        const QString filePath = it.next();
        QString content, error;
        if (!readConfigFile(filePath, MaxTextSize, &content, &error)) {
            fail(error);
            continue;
        }
        if (parseContent(content, filePath)) {
            parsedCount++;
        }
    }

    emit parsingComplete(m_resources.size());
    return parsedCount > 0;
}

// parseZipArchive() is defined in bzipimport.cpp

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

namespace {

bool hasWildcard(const QString &text)
{
    return text.contains(QLatin1Char('*')) || text.contains(QLatin1Char('?')) || text.contains(QLatin1Char('['));
}

/// The files matching @p pattern (an absolute path, wildcards in any part),
/// sorted by name as glob() sorts them
QStringList globFiles(const QString &pattern)
{
    const QStringList parts = QDir::fromNativeSeparators(pattern).split(QLatin1Char('/'));
    // "/a/b" splits into "", "a", "b"; "C:/a" into "C:", "a"
    QStringList current{parts.first().isEmpty() ? QStringLiteral("/") : parts.first() + QLatin1Char('/')};
    for (int i = 1; i < parts.size(); ++i) {
        const QString &part = parts.at(i);
        const bool last = (i == parts.size() - 1);
        if (part.isEmpty() && !last)
            continue;
        QStringList nextLevel;
        for (const QString &base : std::as_const(current)) {
            const QDir dir(base);
            if (!hasWildcard(part)) {
                const QString path = dir.filePath(part);
                if (last ? QFileInfo(path).isFile() : QFileInfo(path).isDir())
                    nextLevel << path;
                continue;
            }
            // As glob(): names starting with a dot are not matched by a wildcard
            const QDir::Filters filters = last ? QDir::Files : (QDir::Dirs | QDir::NoDotAndDotDot);
            for (const QString &name : dir.entryList({part}, filters, QDir::Name))
                nextLevel << dir.filePath(name);
        }
        current = nextLevel;
        if (current.isEmpty())
            break;
    }
    return current;
}

} // namespace

bool BConfigParser::parseContent(const QString &content, const QString &sourceFile, bool followIncludes)
{
    if (content.size() > MaxTextSize)
        return fail(tooLarge(sourceFile));

    qsizetype textRead = content.size();
    int filesRead = 0;
    const QString topFile = followIncludes ? QFileInfo(sourceFile).canonicalFilePath() : QString();

    BConfigLexer::IncludeHandler handler = [&](const QString &spec, const BConfigSource &from,
                                               const QStringList &openFiles, int depth,
                                               QList<BConfigSource> *files, QString *error) {
        const QString where = QStringLiteral("%1:%2").arg(from.name).arg(from.line);
        if (!followIncludes) {
            m_skippedEntries << tr("%1: @%2 was not read (includes are only followed when one file is opened)")
                                    .arg(where, spec);
            return true;
        }
        // lex_open_file() runs a command after '|': never here
        if (spec.startsWith(QLatin1Char('|'))) {
            m_skippedEntries << tr("%1: @%2 was not run (commands are never run)").arg(where, spec);
            return true;
        }
        if (QDir::isRelativePath(spec)) {
            m_skippedEntries << tr("%1: @%2 was not read (relative path)").arg(where, spec);
            return true;
        }
        if (depth >= MaxIncludeDepth) {
            *error = tr("%1: includes nested deeper than %2 levels").arg(where).arg(MaxIncludeDepth);
            return false;
        }
        const QStringList paths = globFiles(spec);
        if (paths.isEmpty()) {
            if (hasWildcard(spec))
                return true;   // a wildcard without a match is ignored
            *error = tr("%1: cannot open the included file %2").arg(where, spec);
            return false;
        }
        for (const QString &path : paths) {
            const QString canonical = QFileInfo(path).canonicalFilePath();
            if (openFiles.contains(canonical)) {
                *error = tr("%1: %2 includes itself").arg(where, path);
                return false;
            }
            if (++filesRead > MaxIncludedFiles) {
                *error = tr("%1: more than %2 included files").arg(where).arg(MaxIncludedFiles);
                return false;
            }
            QString text;
            if (!readConfigFile(path, MaxTextSize - textRead, &text, error))
                return false;
            textRead += text.size();
            files->append(BConfigLexer::source(text, path, canonical));
        }
        return true;
    };

    BConfigLexer lex(content, sourceFile, topFile, handler);

    using Token = BConfigLexer::Token;
    while (true) {
        // ParserInitResource(): EOLs, then a resource type and '{'
        Token t = lex.next();
        if (t == Token::Eof)
            break;
        if (t == Token::Eol)
            continue;
        if (t == Token::Error)
            return fail(lex.error);
        const QString where = lex.where();
        if (t != Token::Identifier)
            return fail(tr("%1: expected a resource type, got \"%2\"").arg(where, lex.str));
        const QString resourceType = lex.str;

        t = lex.nextSkipEol();
        if (t == Token::Error)
            return fail(lex.error);
        if (t != Token::Bob)
            return fail(tr("%1: expected \"{\" after the resource type %2").arg(lex.where(), resourceType));

        QMap<QString, BConfigValue> body;
        if (!parseBody(lex, body, 1))
            return false;

        BConfigResource resource;
        resource.setType(resourceType);
        resource.setSourceFile(sourceFile);
        for (auto it = body.constBegin(); it != body.constEnd(); ++it)
            resource.setValue(it.key(), it.value());

        // "Name not specified for resource" is an error for Bareos; here the
        // resource is left out and reported
        if (resource.name().isEmpty()) {
            m_skippedEntries << tr("%1: the %2 resource has no Name and was left out").arg(where, resourceType);
            continue;
        }
        m_resources.append(resource);
        emit resourceParsed(resource.type(), resource.name());
    }

    return true;
}

bool BConfigParser::parseBody(BConfigLexer &lex, QMap<QString, BConfigValue> &body, int depth)
{
    using Token = BConfigLexer::Token;
    if (depth > MaxNestingDepth)
        return fail(tr("%1: blocks nested deeper than %2 levels").arg(lex.where()).arg(MaxNestingDepth));

    while (true) {
        const Token t = lex.next();
        switch (t) {
        case Token::Eol:
            continue;
        case Token::Eob:
            return true;
        case Token::Eof:
            // "End of conf file reached with unclosed resource."
            return fail(tr("%1: the text ends before the closing \"}\"").arg(lex.where()));
        case Token::Error:
            return fail(lex.error);
        case Token::Identifier:
        case Token::Unquoted: {
            const QString key = lex.str;
            const QString where = lex.where();
            const Token after = lex.nextSkipEol();
            BConfigValue value;
            if (after == Token::Equals) {
                if (!parseValue(lex, key, depth, &value))
                    return false;
            } else if (after == Token::Bob) {
                // A block without '=' (Include { ... } of a FileSet)
                QMap<QString, BConfigValue> block;
                if (!parseBody(lex, block, depth + 1))
                    return false;
                value = BConfigValue(block);
            } else if (after == Token::Error) {
                return fail(lex.error);
            } else {
                return fail(tr("%1: expected \"=\" after %2").arg(where, key));
            }
            insertMerged(body, key, value);
            continue;
        }
        default:
            return fail(tr("%1: unexpected \"%2\"").arg(lex.where(), lex.str));
        }
    }
}

bool BConfigParser::parseValue(BConfigLexer &lex, const QString &key, int depth, BConfigValue *value)
{
    using Token = BConfigLexer::Token;
    const auto isValueToken = [](Token t) {
        return t == Token::Identifier || t == Token::Unquoted || t == Token::Quoted || t == Token::Number;
    };

    // A value starts on the line of its '=' (StoreStr(), StoreName() expect
    // no EOL); only a block may start on a later line (StoreAddresses())
    Token t = lex.next();
    if (t == Token::Eol) {
        t = lex.nextSkipEol();
        if (t != Token::Bob && t != Token::Error)
            return fail(tr("%1: expected a value for %2").arg(lex.where(), key));
    }
    if (t == Token::Error)
        return fail(lex.error);
    if (t == Token::Bob) {
        QMap<QString, BConfigValue> block;
        if (!parseBody(lex, block, depth + 1))
            return false;
        *value = BConfigValue(block);
        return true;
    }
    if (!isValueToken(t))
        return fail(tr("%1: expected a value for %2").arg(lex.where(), key));

    QStringList items;
    QString current = lex.str;
    // StoreTime()/StoreSize(): "30 days" is read as "30days"
    bool joinParts = (t == Token::Number);
    while (true) {
        t = lex.next();
        if (t == Token::Error)
            return fail(lex.error);
        if (t == Token::Eol || t == Token::Eof)
            break;
        if (t == Token::Eob) {
            // ScanToEol() leaves the '}' to the block
            lex.pushBack(t);
            break;
        }
        if (t == Token::Comma) {
            // StoreAlistStr(): the next item follows the comma
            items << current;
            current.clear();
            t = lex.next();
            if (isValueToken(t)) {
                current = lex.str;
                joinParts = (t == Token::Number);
                continue;
            }
            if (t == Token::Error)
                return fail(lex.error);
            if (t == Token::Eob)
                lex.pushBack(t);
            break;
        }
        // The rest of the line: joined after a number, otherwise ignored
        // (ScanToEol())
        if (joinParts && isValueToken(t))
            current += lex.str;
        else
            joinParts = false;
    }
    if (!current.isEmpty() || items.isEmpty())
        items << current;
    *value = items.size() == 1 ? BConfigValue(items.first()) : BConfigValue(items);
    return true;
}
