// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bfilesetdocument.cpp
 * @brief Data model of the FileSet wizard (see bfilesetdocument.h)
 */

#include "jobs/fileset/bfilesetdocument.h"
#include "bquote.h"
#include "blogging.h"

#include <QJsonArray>
#include <QRegularExpression>
#include <QSet>

namespace {

using Directive = BFileSetDocument::Directive;
using DirectiveList = BFileSetDocument::DirectiveList;

// ---------------------------------------------------------------------------
// Keywords (Bareos 25: dird/inc_conf.cc options_items, newinc_items)
// ---------------------------------------------------------------------------

QString normalized(const QString &key)
{
    QString out;
    out.reserve(key.size());
    for (const QChar c : key) {
        if (!c.isSpace() && c != QLatin1Char('_'))
            out.append(c.toLower());
    }
    return out;
}

const QSet<QString> &booleanOptions()
{
    static const QSet<QString> set = {
        QStringLiteral("onefs"), QStringLiteral("recurse"), QStringLiteral("sparse"),
        QStringLiteral("hardlinks"), QStringLiteral("readfifo"), QStringLiteral("portable"),
        QStringLiteral("mtimeonly"), QStringLiteral("keepatime"), QStringLiteral("exclude"),
        QStringLiteral("aclsupport"), QStringLiteral("ignorecase"), QStringLiteral("hfsplussupport"),
        QStringLiteral("noatime"), QStringLiteral("enhancedwild"), QStringLiteral("checkfilechanges"),
        QStringLiteral("honornodumpflag"), QStringLiteral("xattrsupport"), QStringLiteral("autoexclude"),
        QStringLiteral("forceencryption")};
    return set;
}

// Directives that may stand several times in one Options block
const QSet<QString> &listOptions()
{
    static const QSet<QString> set = {
        QStringLiteral("regex"), QStringLiteral("regexdir"), QStringLiteral("regexfile"),
        QStringLiteral("wild"), QStringLiteral("wilddir"), QStringLiteral("wildfile"),
        QStringLiteral("fstype"), QStringLiteral("drivetype"), QStringLiteral("plugin"),
        QStringLiteral("meta")};
    return set;
}

const QSet<QString> &valueOptions()
{
    static const QSet<QString> set = {
        QStringLiteral("compression"), QStringLiteral("signature"), QStringLiteral("accurate"),
        QStringLiteral("verify"), QStringLiteral("replace"), QStringLiteral("strippath"),
        QStringLiteral("size"), QStringLiteral("shadowing")};
    return set;
}

const QMap<QString, QString> &directiveNames()
{
    static const QMap<QString, QString> map = {
        {QStringLiteral("signature"), QStringLiteral("Signature")},
        {QStringLiteral("compression"), QStringLiteral("Compression")},
        {QStringLiteral("accurate"), QStringLiteral("Accurate")},
        {QStringLiteral("verify"), QStringLiteral("Verify")},
        {QStringLiteral("onefs"), QStringLiteral("One FS")},
        {QStringLiteral("recurse"), QStringLiteral("Recurse")},
        {QStringLiteral("sparse"), QStringLiteral("Sparse")},
        {QStringLiteral("hardlinks"), QStringLiteral("Hard Links")},
        {QStringLiteral("readfifo"), QStringLiteral("Read Fifo")},
        {QStringLiteral("replace"), QStringLiteral("Replace")},
        {QStringLiteral("portable"), QStringLiteral("Portable")},
        {QStringLiteral("mtimeonly"), QStringLiteral("Mtime Only")},
        {QStringLiteral("keepatime"), QStringLiteral("Keep Atime")},
        {QStringLiteral("regex"), QStringLiteral("Regex")},
        {QStringLiteral("regexdir"), QStringLiteral("Regex Dir")},
        {QStringLiteral("regexfile"), QStringLiteral("Regex File")},
        {QStringLiteral("wild"), QStringLiteral("Wild")},
        {QStringLiteral("wilddir"), QStringLiteral("Wild Dir")},
        {QStringLiteral("wildfile"), QStringLiteral("Wild File")},
        {QStringLiteral("exclude"), QStringLiteral("Exclude")},
        {QStringLiteral("aclsupport"), QStringLiteral("ACL Support")},
        {QStringLiteral("plugin"), QStringLiteral("Plugin")},
        {QStringLiteral("ignorecase"), QStringLiteral("Ignore Case")},
        {QStringLiteral("fstype"), QStringLiteral("FS Type")},
        {QStringLiteral("hfsplussupport"), QStringLiteral("HFS Plus Support")},
        {QStringLiteral("noatime"), QStringLiteral("No Atime")},
        {QStringLiteral("enhancedwild"), QStringLiteral("Enhanced Wild")},
        {QStringLiteral("drivetype"), QStringLiteral("Drive Type")},
        {QStringLiteral("checkfilechanges"), QStringLiteral("Check File Changes")},
        {QStringLiteral("strippath"), QStringLiteral("Strip Path")},
        {QStringLiteral("honornodumpflag"), QStringLiteral("Honor No Dump Flag")},
        {QStringLiteral("xattrsupport"), QStringLiteral("XAttr Support")},
        {QStringLiteral("size"), QStringLiteral("Size")},
        {QStringLiteral("shadowing"), QStringLiteral("Shadowing")},
        {QStringLiteral("autoexclude"), QStringLiteral("Auto Exclude")},
        {QStringLiteral("forceencryption"), QStringLiteral("Force Encryption")},
        {QStringLiteral("meta"), QStringLiteral("Meta")}};
    return map;
}

// Keys of the options form (resources/directives/fileset.json) that are
// not the Bareos keyword
const QMap<QString, QString> &formAliases()
{
    static const QMap<QString, QString> map = {
        {QStringLiteral("xattr"), QStringLiteral("xattrsupport")},
        {QStringLiteral("acl"), QStringLiteral("aclsupport")}};
    return map;
}

bool parseBool(const QString &value, bool *ok)
{
    const QString v = value.trimmed().toLower();
    *ok = true;
    if (v == QLatin1String("yes") || v == QLatin1String("true") || v == QLatin1String("on") || v == QLatin1String("1"))
        return true;
    if (v == QLatin1String("no") || v == QLatin1String("false") || v == QLatin1String("off") || v == QLatin1String("0"))
        return false;
    *ok = false;
    return false;
}

// ---------------------------------------------------------------------------
// Reader for the text of one resource. It follows the Bareos lexer where a
// FileSet needs it: '#' starts a comment outside quotes, ';' or the end of
// the line ends a directive, a backslash escapes the next character inside
// quotes, blocks are "Key {" (an '=' before the brace is accepted).
// ---------------------------------------------------------------------------

struct Node {
    QString key;
    QString value;         ///< Without quotes and escapes, or the raw text (see quoted)
    bool quoted = false;   ///< value was one quoted string
    bool block = false;
    QList<Node> children;
    QString text;          ///< A block: its text as it stands
};

class Reader
{
public:
    explicit Reader(const QString &text) : m_s(text) {}

    bool parse(QList<Node> *out, QString *error)
    {
        const bool ok = items(out, 0);
        if (!ok && error)
            *error = m_error;
        return ok;
    }

private:
    QChar peek() const { return m_i < m_s.size() ? m_s[m_i] : QChar(); }
    bool atEnd() const { return m_i >= m_s.size(); }

    bool fail(const QString &why)
    {
        m_error = BFileSetDocument::tr("Line %1: %2").arg(m_line).arg(why);
        return false;
    }

    void skipComment()
    {
        while (!atEnd() && peek() != QLatin1Char('\n'))
            ++m_i;
    }

    // Blanks, line ends, ';' and comments between directives
    void skipSeparators()
    {
        while (!atEnd()) {
            const QChar c = peek();
            if (c == QLatin1Char('\n')) {
                ++m_line;
                ++m_i;
            } else if (c.isSpace() || c == QLatin1Char(';')) {
                ++m_i;
            } else if (c == QLatin1Char('#')) {
                skipComment();
            } else {
                break;
            }
        }
    }

    void skipBlanks()
    {
        while (!atEnd() && peek() != QLatin1Char('\n') && peek().isSpace())
            ++m_i;
    }

    bool quotedString(QString *out)
    {
        ++m_i;   // opening quote
        while (!atEnd()) {
            const QChar c = m_s[m_i++];
            if (c == QLatin1Char('\\') && !atEnd()) {
                out->append(m_s[m_i++]);
            } else if (c == QLatin1Char('"')) {
                return true;
            } else {
                if (c == QLatin1Char('\n'))
                    ++m_line;
                out->append(c);
            }
        }
        return fail(BFileSetDocument::tr("a quoted value is not closed"));
    }

    // The rest of the directive: up to the line end, ';', '}' or a comment
    // outside quotes
    QString restOfDirective()
    {
        const int start = m_i;
        bool inQuote = false;
        while (!atEnd()) {
            const QChar c = peek();
            if (inQuote) {
                if (c == QLatin1Char('\\'))
                    ++m_i;
                else if (c == QLatin1Char('"'))
                    inQuote = false;
            } else if (c == QLatin1Char('"')) {
                inQuote = true;
            } else if (c == QLatin1Char('\n') || c == QLatin1Char(';') || c == QLatin1Char('}')
                       || c == QLatin1Char('#')) {
                break;
            }
            ++m_i;
        }
        return m_s.mid(start, m_i - start).trimmed();
    }

    bool items(QList<Node> *out, int depth)
    {
        if (depth > 16)
            return fail(BFileSetDocument::tr("blocks are nested too deeply"));
        while (true) {
            skipSeparators();
            if (atEnd()) {
                if (depth > 0)
                    return fail(BFileSetDocument::tr("a closing brace is missing"));
                return true;
            }
            const QChar c = peek();
            if (c == QLatin1Char('}')) {
                if (depth == 0)
                    return fail(BFileSetDocument::tr("a closing brace has no opening brace"));
                ++m_i;
                return true;
            }
            if (c == QLatin1Char('@'))
                return fail(BFileSetDocument::tr("@include is not read here"));

            const int keyStart = m_i;
            while (!atEnd()) {
                const QChar k = peek();
                if (k == QLatin1Char('=') || k == QLatin1Char('{') || k == QLatin1Char('\n')
                    || k == QLatin1Char('#') || k == QLatin1Char(';') || k == QLatin1Char('}')
                    || k == QLatin1Char('"'))
                    break;
                ++m_i;
            }
            Node node;
            node.key = m_s.mid(keyStart, m_i - keyStart).trimmed();
            if (node.key.isEmpty())
                return fail(BFileSetDocument::tr("a directive name is missing"));
            skipBlanks();
            if (peek() == QLatin1Char('=')) {
                ++m_i;
                skipBlanks();
            } else if (peek() != QLatin1Char('{')) {
                return fail(BFileSetDocument::tr("\"%1\" is followed by neither = nor {").arg(node.key));
            }
            if (peek() == QLatin1Char('{')) {
                ++m_i;
                node.block = true;
                if (!items(&node.children, depth + 1))
                    return false;
                node.text = m_s.mid(keyStart, m_i - keyStart);
            } else if (peek() == QLatin1Char('"')) {
                const int valueStart = m_i;
                QString value;
                if (!quotedString(&value))
                    return false;
                const QString rest = restOfDirective();
                if (rest.isEmpty()) {
                    node.value = value;
                    node.quoted = true;
                } else {
                    // More than one value ("a", "b"): kept as it stands
                    node.value = m_s.mid(valueStart, m_i - valueStart).trimmed();
                }
            } else {
                node.value = restOfDirective();
            }
            out->append(node);
        }
    }

    const QString &m_s;
    int m_i = 0;
    int m_line = 1;
    QString m_error;
};

Directive directiveOf(const Node &node)
{
    Directive d;
    d.key = node.key;
    d.value = node.value;
    d.quoted = node.quoted;
    return d;
}

// ---------------------------------------------------------------------------
// Writer
// ---------------------------------------------------------------------------

bool isPlainWord(const QString &value)
{
    static const QRegularExpression plain(QStringLiteral("^[A-Za-z0-9_.+:/-]+$"));
    return plain.match(value).hasMatch();
}

struct Writer {
    QString text;
    bool ok = true;

    void line(const QString &indent, const QString &key, const QString &value)
    {
        text += indent + key + QStringLiteral(" = ") + value + QLatin1Char('\n');
    }
    void quoted(const QString &indent, const QString &key, const QString &value)
    {
        bool valueOk = true;
        const QString q = BQuote::quoted(value, &valueOk);
        ok = ok && valueOk;
        line(indent, key, q);
    }
    void verbatim(const QString &indent, const Directive &d)
    {
        if (d.quoted) {
            quoted(indent, d.key, d.value);
        } else {
            ok = ok && !BQuote::hasControlCharacter(d.value);
            line(indent, d.key, d.value);
        }
    }
    void option(const QString &indent, const QString &key, const QVariant &value)
    {
        const QString directive = BFileSetDocument::optionDirective(key);
        if (value.typeId() == QMetaType::Bool) {
            line(indent, directive, value.toBool() ? QStringLiteral("yes") : QStringLiteral("no"));
        } else if (value.typeId() == QMetaType::Int || value.typeId() == QMetaType::LongLong) {
            line(indent, directive, QString::number(value.toLongLong()));
        } else if (value.typeId() == QMetaType::QStringList) {
            for (const QString &item : value.toStringList()) {
                if (!item.isEmpty())
                    quoted(indent, directive, item);
            }
        } else {
            const QString v = value.toString();
            if (v.isEmpty())
                return;
            if (!listOptions().contains(key) && isPlainWord(v))
                line(indent, directive, v);
            else
                quoted(indent, directive, v);
        }
    }
};

// One Options block into @p block: the first without "Exclude = yes" gives
// the options of the files, the first with it the exclude lists; any
// further block is kept as it stands
void addOptionsBlock(BFileSetDocument::IncludeBlock *block, const DirectiveList &directives,
                     bool *primaryTaken, bool *excludeTaken)
{
    bool isExclude = false;
    for (const Directive &d : directives) {
        bool ok = false;
        if (normalized(d.key) == QLatin1String("exclude") && parseBool(d.value, &ok) && ok)
            isExclude = true;
    }

    if (isExclude && !*excludeTaken) {
        *excludeTaken = true;
        QStringList wild = block->excludeFilesModel->items();
        QStringList wildDir = block->excludePatternsModel->items();
        QStringList wildFile = block->excludeWildFileModel->items();
        for (const Directive &d : directives) {
            const QString k = normalized(d.key);
            if (k == QLatin1String("exclude"))
                continue;
            if (k == QLatin1String("wild"))
                wild.append(d.value);
            else if (k == QLatin1String("wilddir"))
                wildDir.append(d.value);
            else if (k == QLatin1String("wildfile"))
                wildFile.append(d.value);
            else
                block->excludeOptionsExtra.append(d);
        }
        block->excludeFilesModel->setItems(wild);
        block->excludePatternsModel->setItems(wildDir);
        block->excludeWildFileModel->setItems(wildFile);
        return;
    }
    if (!isExclude && !*primaryTaken) {
        *primaryTaken = true;
        block->options.clear();
        for (const Directive &d : directives) {
            const QString k = BFileSetDocument::optionKey(d.key);
            bool ok = false;
            const bool b = parseBool(d.value, &ok);
            if (booleanOptions().contains(k) && ok) {
                block->options[k] = b;
            } else if (listOptions().contains(k) || block->options.contains(k)) {
                // Several values: a list, one line each when written
                QStringList list;
                const QVariant old = block->options.value(k);
                if (old.typeId() == QMetaType::QStringList)
                    list = old.toStringList();
                else if (old.isValid())
                    list.append(old.toString());
                list.append(d.value);
                block->options[k] = list;
            } else if (k == QLatin1String("strippath") && (d.value.toInt(&ok), ok)) {
                block->options[k] = d.value.toInt();
            } else {
                block->options[k] = d.value;
            }
        }
        return;
    }
    block->extraOptionsBlocks.append(directives);
}

QStringList jsonStrings(const QJsonValue &v)
{
    QStringList out;
    if (v.isArray()) {
        for (const QJsonValue &item : v.toArray())
            out.append(item.toString());
    } else if (v.isString()) {
        out.append(v.toString());
    }
    return out;
}

} // namespace

BFileSetDocument::BFileSetDocument(QObject *parent)
    : QObject(parent)
    , m_undoStack(new QUndoStack(this))
    , m_excludePathModel(new BEditableListModel(this))
{
    m_excludePathModel->setUndoStack(m_undoStack);

    connect(m_excludePathModel, &BEditableListModel::modelModified,
            this, &BFileSetDocument::onAnyModelChanged);

    connect(m_undoStack, &QUndoStack::cleanChanged, this, [this](bool clean) {
        emit modifiedChanged(!clean);
    });
}

BFileSetDocument::~BFileSetDocument()
{
    // The undo stack may still reference the models
    m_undoStack->clear();
    for (IncludeBlock *block : std::as_const(m_includeBlocks))
        deleteBlock(block);
}

// ============================================================================
// Basic Properties
// ============================================================================

QString BFileSetDocument::name() const
{
    return m_name;
}

void BFileSetDocument::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged(m_name);
        emit documentChanged();
    }
}

QString BFileSetDocument::description() const
{
    return m_description;
}

void BFileSetDocument::setDescription(const QString &desc)
{
    if (m_description != desc) {
        m_description = desc;
        emit descriptionChanged(m_description);
        emit documentChanged();
    }
}

bool BFileSetDocument::enableVss() const
{
    return m_enableVss;
}

void BFileSetDocument::setEnableVss(bool enable)
{
    if (m_enableVss != enable) {
        m_enableVss = enable;
        emit documentChanged();
    }
}

bool BFileSetDocument::ignoreFileSetChanges() const
{
    return m_ignoreFileSetChanges;
}

void BFileSetDocument::setIgnoreFileSetChanges(bool ignore)
{
    if (m_ignoreFileSetChanges != ignore) {
        m_ignoreFileSetChanges = ignore;
        emit documentChanged();
    }
}

// ============================================================================
// Include Block Management
// ============================================================================

int BFileSetDocument::includeBlockCount() const
{
    return m_includeBlocks.count();
}

BFileSetDocument::IncludeBlock *BFileSetDocument::includeBlock(int index)
{
    if (index < 0 || index >= m_includeBlocks.count())
        return nullptr;
    return m_includeBlocks.at(index);
}

const BFileSetDocument::IncludeBlock *BFileSetDocument::includeBlock(int index) const
{
    if (index < 0 || index >= m_includeBlocks.count())
        return nullptr;
    return m_includeBlocks.at(index);
}

BFileSetDocument::IncludeBlock *BFileSetDocument::newIncludeBlock(bool withDefaults)
{
    auto *block = new IncludeBlock();
    block->id = m_nextBlockId++;

    block->pathsModel = new BEditableListModel(this);
    block->pathsModel->setUndoStack(m_undoStack);
    block->excludeFilesModel = new BEditableListModel(this);
    block->excludeFilesModel->setUndoStack(m_undoStack);
    block->excludePatternsModel = new BEditableListModel(this);
    block->excludePatternsModel->setUndoStack(m_undoStack);
    block->excludeWildFileModel = new BEditableListModel(this);
    block->excludeWildFileModel->setUndoStack(m_undoStack);

    if (withDefaults)
        initDefaultOptions(block);
    connectBlockSignals(block);

    const int index = m_includeBlocks.count();
    m_includeBlocks.append(block);
    emit includeBlockAdded(index);
    emit documentChanged();
    return block;
}

BFileSetDocument::IncludeBlock *BFileSetDocument::addIncludeBlock()
{
    return newIncludeBlock(true);
}

void BFileSetDocument::removeIncludeBlock(int index)
{
    if (index < 0 || index >= m_includeBlocks.count())
        return;
    // Commands on the stack may reference the block's models
    m_undoStack->clear();
    deleteBlock(m_includeBlocks.takeAt(index));
    emit includeBlockRemoved(index);
    emit documentChanged();
}

void BFileSetDocument::deleteBlock(IncludeBlock *block)
{
    delete block->pathsModel;
    delete block->excludeFilesModel;
    delete block->excludePatternsModel;
    delete block->excludeWildFileModel;
    delete block;
}

// ============================================================================
// Global Exclude, undo
// ============================================================================

BEditableListModel *BFileSetDocument::excludePathModel() const
{
    return m_excludePathModel;
}

QUndoStack *BFileSetDocument::undoStack() const
{
    return m_undoStack;
}

bool BFileSetDocument::isModified() const
{
    return !m_undoStack->isClean();
}

void BFileSetDocument::markClean()
{
    m_undoStack->setClean();
}

// ============================================================================
// Options
// ============================================================================

QString BFileSetDocument::optionKey(const QString &key)
{
    const QString n = normalized(key);
    return formAliases().value(n, n);
}

QString BFileSetDocument::optionDirective(const QString &key)
{
    return directiveNames().value(optionKey(key), key);
}

bool BFileSetDocument::isOptionKeyword(const QString &key)
{
    const QString k = optionKey(key);
    return booleanOptions().contains(k) || listOptions().contains(k) || valueOptions().contains(k);
}

void BFileSetDocument::setFormOption(IncludeBlock *block, const QString &formKey, const QVariant &value)
{
    if (!block)
        return;
    const QString n = normalized(formKey);
    if (n == QLatin1String("vss"))
        return;   // not an Options directive; the FileSet's Enable VSS is on the settings page
    if (n == QLatin1String("excludedircontaining")) {
        const QString v = value.toString().trimmed();
        block->excludeDirContaining = v.isEmpty() ? QStringList() : QStringList(v);
        emit documentChanged();
        return;
    }
    const QString key = optionKey(formKey);
    bool remove = !value.isValid();
    if (value.typeId() == QMetaType::QStringList)
        remove = value.toStringList().isEmpty();
    else if (value.typeId() == QMetaType::QString)
        remove = value.toString().trimmed().isEmpty()
              || (key == QLatin1String("compression") && value.toString() == QLatin1String("None"));
    else if (key == QLatin1String("strippath"))
        remove = value.toInt() == 0;
    if (value.typeId() == QMetaType::QString && listOptions().contains(key)) {
        // The form edits the first value; further ones (loaded) stay
        QStringList list = block->options.value(key).toStringList();
        if (remove) {
            if (!list.isEmpty())
                list.removeFirst();
        } else if (list.isEmpty()) {
            list.append(value.toString().trimmed());
        } else {
            list[0] = value.toString().trimmed();
        }
        if (list.isEmpty())
            block->options.remove(key);
        else
            block->options[key] = list;
    } else if (remove) {
        block->options.remove(key);
    } else {
        block->options[key] = value;
    }
    emit documentChanged();
}

QMap<QString, QVariant> BFileSetDocument::formOptions(const IncludeBlock *block)
{
    QMap<QString, QVariant> out;
    if (!block)
        return out;
    // The form's names for the Bareos keywords
    QMap<QString, QString> reverse;
    for (auto it = formAliases().constBegin(); it != formAliases().constEnd(); ++it)
        reverse.insert(it.value(), it.key());
    reverse.insert(QStringLiteral("strippath"), QStringLiteral("strip_path"));
    for (auto it = block->options.constBegin(); it != block->options.constEnd(); ++it) {
        QVariant v = it.value();
        // The form shows one text for the pattern options
        if (v.typeId() == QMetaType::QStringList
            && (it.key() == QLatin1String("regex") || it.key() == QLatin1String("wild")
                || it.key() == QLatin1String("wilddir") || it.key() == QLatin1String("wildfile")))
            v = v.toStringList().value(0);
        out.insert(reverse.value(it.key(), it.key()), v);
    }
    if (!block->excludeDirContaining.isEmpty())
        out.insert(QStringLiteral("exclude_dir_containing"), block->excludeDirContaining.first());
    return out;
}

void BFileSetDocument::initDefaultOptions(IncludeBlock *block)
{
    block->options[QStringLiteral("signature")] = QStringLiteral("SHA1");
    block->options[QStringLiteral("compression")] = QStringLiteral("LZ4");
    block->options[QStringLiteral("onefs")] = true;
    block->options[QStringLiteral("aclsupport")] = true;
    block->options[QStringLiteral("xattrsupport")] = true;
}

// ============================================================================
// Serialization
// ============================================================================

bool BFileSetDocument::loadFromConfigText(const QString &text, QString *error)
{
    QList<Node> top;
    Reader reader(text);
    QString why;
    if (!reader.parse(&top, &why)) {
        if (error) *error = why;
        return false;
    }
    if (top.size() != 1 || !top.first().block || normalized(top.first().key) != QLatin1String("fileset")) {
        if (error) *error = tr("The text is not one FileSet resource.");
        return false;
    }

    clear();
    const Node &fs = top.first();
    for (const Node &n : fs.children) {
        const QString k = normalized(n.key);
        if (!n.block) {
            bool ok = false;
            const bool flag = parseBool(n.value, &ok);
            if (k == QLatin1String("name"))
                m_name = n.value;
            else if (k == QLatin1String("description"))
                m_description = n.value;
            else if (k == QLatin1String("enablevss") && ok)
                m_enableVss = flag;
            else if (k == QLatin1String("ignorefilesetchanges") && ok)
                m_ignoreFileSetChanges = flag;
            else
                m_extraDirectives.append(directiveOf(n));
            continue;
        }
        if (k == QLatin1String("include")) {
            IncludeBlock *block = newIncludeBlock(false);
            bool primaryTaken = false;
            bool excludeTaken = false;
            QStringList paths;
            for (const Node &c : n.children) {
                const QString ck = normalized(c.key);
                if (c.block) {
                    bool nested = false;
                    DirectiveList directives;
                    for (const Node &o : c.children) {
                        if (o.block)
                            nested = true;
                        else
                            directives.append(directiveOf(o));
                    }
                    if (ck == QLatin1String("options") && !nested)
                        addOptionsBlock(block, directives, &primaryTaken, &excludeTaken);
                    else
                        block->extraBlocks.append(c.text);   // not read here; kept as it stands
                } else if (ck == QLatin1String("file")) {
                    paths.append(c.value);
                } else if (ck == QLatin1String("plugin")) {
                    block->plugins.append(c.value);
                } else if (ck == QLatin1String("excludedircontaining")) {
                    block->excludeDirContaining.append(c.value);
                } else {
                    block->extraDirectives.append(directiveOf(c));
                }
            }
            block->pathsModel->setItems(paths);
        } else if (k == QLatin1String("exclude")) {
            QStringList files = m_excludePathModel->items();
            for (const Node &c : n.children) {
                if (c.block)
                    m_extraBlocks.append(c.text);
                else if (normalized(c.key) == QLatin1String("file"))
                    files.append(c.value);
                else
                    m_excludeExtra.append(directiveOf(c));
            }
            m_excludePathModel->setItems(files);
        } else {
            m_extraBlocks.append(n.text);
        }
    }

    markClean();
    emit documentChanged();
    return true;
}

void BFileSetDocument::loadFromTemplate(const QJsonObject &fileset)
{
    clear();
    m_name = fileset.value(QStringLiteral("Name")).toString();
    m_description = fileset.value(QStringLiteral("Description")).toString();
    m_enableVss = fileset.value(QStringLiteral("Enable VSS")).toBool(true);
    m_ignoreFileSetChanges = fileset.value(QStringLiteral("Ignore FileSet Changes")).toBool(false);

    // "File" is the directive; some templates said "Files"
    auto files = [](const QJsonObject &o) {
        return jsonStrings(o.value(QStringLiteral("File"))) + jsonStrings(o.value(QStringLiteral("Files")));
    };

    const QJsonObject include = fileset.value(QStringLiteral("Include")).toObject();
    IncludeBlock *block = newIncludeBlock(false);
    block->pathsModel->setItems(files(include));
    block->plugins = jsonStrings(include.value(QStringLiteral("Plugin")))
                   + jsonStrings(fileset.value(QStringLiteral("Plugin")));   // older templates put it here
    block->excludeDirContaining = jsonStrings(include.value(QStringLiteral("Exclude Dir Containing")));

    const QJsonObject opts = include.value(QStringLiteral("Options")).toObject();
    for (auto it = opts.begin(); it != opts.end(); ++it) {
        if (it.key().compare(QLatin1String("Exclude"), Qt::CaseInsensitive) == 0 && it.value().isObject()) {
            // The templates group the patterns to exclude; they become an
            // Options block with "Exclude = yes"
            const QJsonObject excl = it.value().toObject();
            block->excludeFilesModel->setItems(files(excl) + jsonStrings(excl.value(QStringLiteral("Wild"))));
            block->excludePatternsModel->setItems(jsonStrings(excl.value(QStringLiteral("WildDir"))));
            block->excludeWildFileModel->setItems(jsonStrings(excl.value(QStringLiteral("WildFile"))));
            for (const char *regex : {"Regex", "RegexDir", "RegexFile"}) {
                for (const QString &v : jsonStrings(excl.value(QLatin1String(regex))))
                    block->excludeOptionsExtra.append(Directive{QLatin1String(regex), v, true});
            }
            continue;
        }
        const QString key = optionKey(it.key());
        const QJsonValue val = it.value();
        if (val.isBool())
            block->options[key] = val.toBool();
        else if (val.isArray())
            block->options[key] = jsonStrings(val);
        else if (val.isDouble())
            block->options[key] = val.toInt();
        else if (val.isString())
            block->options[key] = listOptions().contains(key) ? QVariant(QStringList(val.toString()))
                                                              : QVariant(val.toString());
    }

    m_excludePathModel->setItems(files(fileset.value(QStringLiteral("Exclude")).toObject()));

    markClean();
    emit documentChanged();
}

void BFileSetDocument::loadFromBareosJson(const QJsonObject &fsObj)
{
    BLOG_DEBUG() << "BFileSetDocument::loadFromBareosJson() — name:" << fsObj["name"].toString();
    clear();

    m_name = fsObj.value(QStringLiteral("name")).toString();
    m_description = fsObj.value(QStringLiteral("description")).toString();

    for (const QJsonValue &incVal : fsObj.value(QStringLiteral("include")).toArray()) {
        const QJsonObject incObj = incVal.toObject();
        IncludeBlock *block = newIncludeBlock(false);
        bool primaryTaken = false;
        bool excludeTaken = false;

        for (const QJsonValue &optVal : incObj.value(QStringLiteral("options")).toArray()) {
            DirectiveList directives;
            const QJsonObject optObj = optVal.toObject();
            for (auto it = optObj.begin(); it != optObj.end(); ++it) {
                const QJsonValue v = it.value();
                const QString key = optionDirective(it.key());   // "regexdir" -> "Regex Dir"
                if (v.isArray()) {
                    for (const QString &item : jsonStrings(v))
                        directives.append(Directive{key, item, true});
                } else if (v.isBool()) {
                    directives.append(Directive{key, v.toBool() ? QStringLiteral("yes") : QStringLiteral("no"), false});
                } else if (v.isDouble()) {
                    directives.append(Directive{key, QString::number(v.toInteger()), false});
                } else {
                    directives.append(Directive{key, v.toString(), !isPlainWord(v.toString())});
                }
            }
            addOptionsBlock(block, directives, &primaryTaken, &excludeTaken);
        }
        block->pathsModel->setItems(jsonStrings(incObj.value(QStringLiteral("file"))));
        block->plugins = jsonStrings(incObj.value(QStringLiteral("plugin")));
        block->excludeDirContaining = jsonStrings(incObj.value(QStringLiteral("excludedircontaining")));
    }

    QStringList excludePaths;
    for (const QJsonValue &excVal : fsObj.value(QStringLiteral("exclude")).toArray())
        excludePaths.append(jsonStrings(excVal.toObject().value(QStringLiteral("file"))));
    m_excludePathModel->setItems(excludePaths);

    markClean();
    emit documentChanged();
}

QString BFileSetDocument::toConfigText(QString *error) const
{
    Writer w;
    const QString in1 = QStringLiteral("  ");
    const QString in2 = QStringLiteral("    ");
    const QString in3 = QStringLiteral("      ");

    w.text += QStringLiteral("FileSet {\n");
    w.quoted(in1, QStringLiteral("Name"), m_name);
    if (!m_description.isEmpty())
        w.quoted(in1, QStringLiteral("Description"), m_description);
    // Enable VSS defaults to yes in Bareos: "no" must be written to stay no
    w.line(in1, QStringLiteral("Enable VSS"), m_enableVss ? QStringLiteral("yes") : QStringLiteral("no"));
    if (m_ignoreFileSetChanges)
        w.line(in1, QStringLiteral("Ignore FileSet Changes"), QStringLiteral("yes"));
    for (const Directive &d : m_extraDirectives)
        w.verbatim(in1, d);

    for (const IncludeBlock *block : m_includeBlocks) {
        w.text += in1 + QStringLiteral("Include {\n");

        // The options of the files
        bool hasOptions = false;
        for (auto it = block->options.constBegin(); it != block->options.constEnd(); ++it) {
            const QVariant &v = it.value();
            if ((v.typeId() == QMetaType::QString && v.toString().isEmpty())
                || (v.typeId() == QMetaType::QStringList && v.toStringList().isEmpty()))
                continue;
            if (!hasOptions) {
                w.text += in2 + QStringLiteral("Options {\n");
                hasOptions = true;
            }
            w.option(in3, it.key(), v);
        }
        if (hasOptions)
            w.text += in2 + QStringLiteral("}\n");

        // What to leave out: an Options block with "Exclude = yes"
        const QStringList wild = block->excludeFilesModel->items();
        const QStringList wildDir = block->excludePatternsModel->items();
        const QStringList wildFile = block->excludeWildFileModel->items();
        if (!wild.isEmpty() || !wildDir.isEmpty() || !wildFile.isEmpty() || !block->excludeOptionsExtra.isEmpty()) {
            w.text += in2 + QStringLiteral("Options {\n");
            w.line(in3, QStringLiteral("Exclude"), QStringLiteral("yes"));
            for (const QString &p : wild)
                if (!p.isEmpty()) w.quoted(in3, QStringLiteral("Wild"), p);
            for (const QString &p : wildDir)
                if (!p.isEmpty()) w.quoted(in3, QStringLiteral("Wild Dir"), p);
            for (const QString &p : wildFile)
                if (!p.isEmpty()) w.quoted(in3, QStringLiteral("Wild File"), p);
            for (const Directive &d : block->excludeOptionsExtra)
                w.verbatim(in3, d);
            w.text += in2 + QStringLiteral("}\n");
        }

        for (const DirectiveList &extra : block->extraOptionsBlocks) {
            w.text += in2 + QStringLiteral("Options {\n");
            for (const Directive &d : extra)
                w.verbatim(in3, d);
            w.text += in2 + QStringLiteral("}\n");
        }
        for (const QString &raw : block->extraBlocks)
            w.text += in2 + raw.trimmed() + QLatin1Char('\n');

        for (const QString &path : block->pathsModel->items())
            if (!path.isEmpty()) w.quoted(in2, QStringLiteral("File"), path);
        for (const QString &plugin : block->plugins)
            if (!plugin.isEmpty()) w.quoted(in2, QStringLiteral("Plugin"), plugin);
        for (const QString &marker : block->excludeDirContaining)
            if (!marker.isEmpty()) w.quoted(in2, QStringLiteral("Exclude Dir Containing"), marker);
        for (const Directive &d : block->extraDirectives)
            w.verbatim(in2, d);

        w.text += in1 + QStringLiteral("}\n");
    }

    const QStringList excluded = m_excludePathModel->items();
    if (!excluded.isEmpty() || !m_excludeExtra.isEmpty()) {
        w.text += in1 + QStringLiteral("Exclude {\n");
        for (const QString &path : excluded)
            if (!path.isEmpty()) w.quoted(in2, QStringLiteral("File"), path);
        for (const Directive &d : m_excludeExtra)
            w.verbatim(in2, d);
        w.text += in1 + QStringLiteral("}\n");
    }
    for (const QString &raw : m_extraBlocks)
        w.text += in1 + raw.trimmed() + QLatin1Char('\n');

    w.text += QStringLiteral("}\n");

    if (!w.ok) {
        if (error)
            *error = tr("A value contains a line break or another control character.");
        return QString();
    }
    return w.text;
}

// ============================================================================
// Validation
// ============================================================================

BFileSetDocument::ValidationResult BFileSetDocument::validate() const
{
    ValidationResult result;
    result.valid = true;

    if (m_name.isEmpty()) {
        result.valid = false;
        result.errors.append(tr("FileSet name is required"));
    } else if (!BQuote::isValidName(m_name)) {
        result.valid = false;
        result.errors.append(tr("The name may contain only letters, digits, blanks and the characters "
                                ": . - _ / (no blank at the start or end, at most 127 characters)"));
    }

    if (m_includeBlocks.isEmpty()) {
        result.valid = false;
        result.errors.append(tr("At least one Include block is required"));
    } else {
        bool hasAnyPath = false;
        for (int i = 0; i < m_includeBlocks.count(); ++i) {
            const IncludeBlock *block = m_includeBlocks.at(i);
            if (block->pathsModel->rowCount() > 0 || !block->plugins.isEmpty())
                hasAnyPath = true;
            else
                result.warnings.append(tr("Include block %1 has no file paths").arg(i + 1));
        }
        if (!hasAnyPath) {
            result.valid = false;
            result.errors.append(tr("At least one Include block must have file paths"));
        }
    }

    QString error;
    if (toConfigText(&error).isEmpty()) {
        result.valid = false;
        result.errors.append(error);
    }
    return result;
}

void BFileSetDocument::clear()
{
    m_name.clear();
    m_description.clear();
    m_enableVss = true;
    m_ignoreFileSetChanges = false;
    m_extraDirectives.clear();
    m_excludeExtra.clear();
    m_extraBlocks.clear();

    // Clear undo stack FIRST (commands may reference models)
    m_undoStack->clear();

    for (IncludeBlock *block : std::as_const(m_includeBlocks))
        deleteBlock(block);
    m_includeBlocks.clear();

    m_excludePathModel->setItems(QStringList());
    m_nextBlockId = 1;

    emit documentChanged();
}

// ============================================================================
// Private Methods
// ============================================================================

void BFileSetDocument::connectBlockSignals(IncludeBlock *block)
{
    connect(block->pathsModel, &BEditableListModel::modelModified,
            this, &BFileSetDocument::onAnyModelChanged);
    connect(block->excludeFilesModel, &BEditableListModel::modelModified,
            this, &BFileSetDocument::onAnyModelChanged);
    connect(block->excludePatternsModel, &BEditableListModel::modelModified,
            this, &BFileSetDocument::onAnyModelChanged);
    connect(block->excludeWildFileModel, &BEditableListModel::modelModified,
            this, &BFileSetDocument::onAnyModelChanged);
}

void BFileSetDocument::onAnyModelChanged()
{
    emit documentChanged();
}
