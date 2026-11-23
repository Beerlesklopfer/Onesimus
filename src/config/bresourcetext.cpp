// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bresourcetext.cpp
 * @brief The text of one configuration resource and how a change of single
 *        directives is written into it
 */

#include "config/bresourcetext.h"
#include "bquote.h"

#include <QHash>
#include <QRegularExpression>
#include <QSet>

// ============================================================================
// Lexical helpers
// ============================================================================

int BResourceText::commentStart(const QString &line)
{
    bool inQuote = false;
    for (int i = 0; i < line.size(); ++i) {
        const QChar c = line[i];
        if (inQuote) {
            if (c == QLatin1Char('\\'))
                ++i;
            else if (c == QLatin1Char('"'))
                inQuote = false;
            continue;
        }
        if (c == QLatin1Char('"'))
            inQuote = true;
        else if (c == QLatin1Char('#'))
            return i;
        else if (c == QLatin1Char('/') && i + 1 < line.size() && line[i + 1] == QLatin1Char('/'))
            return i;
    }
    return -1;
}

QString BResourceText::lineBody(const QString &line)
{
    QString text = line;
    if (text.endsWith(QLatin1Char('\n')))
        text.chop(1);
    if (text.endsWith(QLatin1Char('\r')))
        text.chop(1);
    return text;
}

QString BResourceText::unquotedPart(const QString &line)
{
    QString out = line;
    bool inQuote = false;
    for (int i = 0; i < out.size(); ++i) {
        const QChar c = out[i];
        if (inQuote) {
            if (c == QLatin1Char('\\') && i + 1 < out.size()) {
                out[i] = QLatin1Char(' ');
                out[++i] = QLatin1Char(' ');
                continue;
            }
            if (c == QLatin1Char('"'))
                inQuote = false;
            else
                out[i] = QLatin1Char(' ');
            continue;
        }
        if (c == QLatin1Char('"'))
            inQuote = true;
    }
    return out;
}

QString BResourceText::unescape(const QString &quotedBody)
{
    return BQuote::unescape(quotedBody);
}

QString BResourceText::normalizedKey(const QString &key)
{
    QString out;
    out.reserve(key.size());
    for (const QChar c : key) {
        if (!c.isSpace() && c != QLatin1Char('_'))
            out.append(c.toLower());
    }
    return out;
}

bool BResourceText::needsQuotes(const QString &value)
{
    static const QRegularExpression plainRx(QStringLiteral("^[A-Za-z0-9_.:@+/-]+$"));
    return !plainRx.match(value).hasMatch();
}

void BResourceText::splitLines(const QString &text, QStringList *lines, QStringList *lineEnds,
                               QString *lineEnd)
{
    QStringList outLines;
    QStringList outEnds;
    int start = 0;
    while (start < text.size()) {
        const int newline = text.indexOf(QLatin1Char('\n'), start);
        if (newline < 0) {
            outLines.append(text.mid(start));
            outEnds.append(QString());
            break;
        }
        const QString line = text.mid(start, newline - start + 1);
        outLines.append(line);
        outEnds.append(line.endsWith(QLatin1String("\r\n")) ? QStringLiteral("\r\n") : QStringLiteral("\n"));
        start = newline + 1;
    }
    QString style = QStringLiteral("\n");
    for (const QString &end : std::as_const(outEnds)) {
        if (!end.isEmpty()) {
            style = end;
            break;
        }
    }
    if (lines)
        *lines = outLines;
    if (lineEnds)
        *lineEnds = outEnds;
    if (lineEnd)
        *lineEnd = style;
}

// ============================================================================
// Parse
// ============================================================================

int BResourceText::Parsed::find(const QString &key) const
{
    const QString wanted = normalizedKey(key);
    int found = -1;
    for (int i = 0; i < directives.size(); ++i) {
        if (directives[i].normalized != wanted)
            continue;
        if (found >= 0)
            return -2;
        found = i;
    }
    return found;
}

QString BResourceText::Parsed::value(const QString &key) const
{
    const int index = find(key);
    return index >= 0 ? directives[index].value : QString();
}

namespace {

/// Every "key =" on a line, normalized (to know which keys a line holds)
QStringList keysOnLine(const QString &blank)
{
    static const QRegularExpression keyRx(QStringLiteral("([A-Za-z][A-Za-z0-9 _]*?)\\s*="));
    QStringList keys;
    QRegularExpressionMatchIterator it = keyRx.globalMatch(blank);
    while (it.hasNext()) {
        const QRegularExpressionMatch match = it.next();
        // The words in front of "=" (separators are not part of the match)
        const QString key = match.captured(1).trimmed();
        if (!key.isEmpty())
            keys.append(BResourceText::normalizedKey(key));
    }
    return keys;
}

/// The words in front of a "{" on a line ("Run Script {" -> "runscript")
QString blockKey(const QString &blank)
{
    const int brace = blank.indexOf(QLatin1Char('{'));
    QString key = blank.left(brace).trimmed();
    if (key.endsWith(QLatin1Char('=')))
        key.chop(1);
    return BResourceText::normalizedKey(key);
}

} // namespace

BResourceText::Parsed BResourceText::parse(const QString &text)
{
    Parsed parsed;
    auto fail = [&parsed](const QString &error) {
        parsed.ok = false;
        parsed.error = error;
        return parsed;
    };

    splitLines(text, &parsed.lines, &parsed.lineEnds, &parsed.lineEnd);

    static const QRegularExpression openRx(QStringLiteral("^\\s*([A-Za-z]+)\\s*\\{(.*)$"));
    static const QRegularExpression directiveRx(QStringLiteral("^(\\s*)([A-Za-z][A-Za-z0-9 _]*?)\\s*=\\s*"));

    int depth = 0;
    bool opened = false;
    for (int i = 0; i < parsed.lines.size(); ++i) {
        const QString line = lineBody(parsed.lines[i]);
        const int comment = commentStart(line);
        const QString code = comment >= 0 ? line.left(comment) : line;
        const QString blank = unquotedPart(code);

        if (!opened) {
            if (code.trimmed().isEmpty())
                continue;
            const QRegularExpressionMatch open = openRx.match(blank);
            if (!open.hasMatch())
                return fail(tr("The text does not start with a resource (line %1).").arg(i + 1));
            if (!open.captured(2).trimmed().isEmpty())
                return fail(tr("Directives on the line of \"%1 {\" cannot be edited safely (line %2).")
                                .arg(open.captured(1)).arg(i + 1));
            opened = true;
            depth = 1;
            parsed.typeWord = open.captured(1);
            parsed.openLine = i;
            continue;
        }
        if (depth == 0) {
            if (!code.trimmed().isEmpty())
                return fail(tr("Text after the closing brace of the resource (line %1).").arg(i + 1));
            continue;
        }

        const int opens = int(blank.count(QLatin1Char('{')));
        const int closes = int(blank.count(QLatin1Char('}')));

        if (depth > 1) {
            // Inside a nested block: kept as it is
            depth += opens - closes;
            if (depth < 1)
                return fail(tr("The closing brace of the resource shares its line with a nested block "
                               "(line %1).").arg(i + 1));
            parsed.opaqueLines.append(i);
            continue;
        }

        // Top level
        if (blank.trimmed() == QLatin1String("}")) {
            depth = 0;
            parsed.closingLine = i;
            continue;
        }
        if (opens > 0 || closes > 0) {
            if (closes > opens)
                return fail(tr("The closing brace shares its line with a directive (line %1).").arg(i + 1));
            // A nested block begins here (or is all on this line)
            depth += opens - closes;
            parsed.opaqueLines.append(i);
            const QString key = blockKey(blank);
            if (!key.isEmpty())
                parsed.opaqueKeys.append(key);
            parsed.opaqueKeys.append(keysOnLine(blank));
            continue;
        }
        if (code.trimmed().isEmpty())
            continue;   // empty or a comment

        const QRegularExpressionMatch match = directiveRx.match(blank);
        auto opaque = [&]() {
            parsed.opaqueLines.append(i);
            parsed.opaqueKeys.append(keysOnLine(blank));
        };
        if (!match.hasMatch()) {
            opaque();
            continue;
        }

        Directive directive;
        directive.lineIndex = i;
        directive.key = match.captured(2).trimmed();
        directive.normalized = normalizedKey(directive.key);
        directive.valueStart = match.capturedEnd(0);
        QString rest = code.mid(directive.valueStart);
        while (!rest.isEmpty() && rest.back().isSpace())
            rest.chop(1);
        bool understood = true;
        if (rest.startsWith(QLatin1Char('"'))) {
            int close = -1;
            for (int k = 1; k < rest.size(); ++k) {
                if (rest[k] == QLatin1Char('\\')) {
                    ++k;
                    continue;
                }
                if (rest[k] == QLatin1Char('"')) {
                    close = k;
                    break;
                }
            }
            const QString after = close >= 0 ? rest.mid(close + 1).trimmed() : QString();
            if (close < 0 || (!after.isEmpty() && after != QLatin1String(";"))) {
                understood = false;     // a list, or a value that does not end on its line
            } else {
                directive.quoted = true;
                directive.value = unescape(rest.mid(1, close - 1));
                directive.valueEnd = directive.valueStart + close + 1;
            }
        } else {
            const int semicolon = unquotedPart(rest).indexOf(QLatin1Char(';'));
            if (semicolon >= 0) {
                if (!rest.mid(semicolon + 1).trimmed().isEmpty())
                    understood = false; // another directive on the line
                rest.truncate(semicolon);
                while (!rest.isEmpty() && rest.back().isSpace())
                    rest.chop(1);
            }
            if (rest.contains(QLatin1Char('"')))
                understood = false;     // a mixed list
            directive.value = rest;
            directive.valueEnd = directive.valueStart + int(rest.size());
        }
        if (!understood) {
            opaque();
            continue;
        }
        parsed.directives.append(directive);
    }
    if (!opened)
        return fail(tr("The text contains no resource."));
    if (parsed.closingLine < 0)
        return fail(tr("The resource has no closing brace."));

    const int nameIndex = parsed.find(QStringLiteral("Name"));
    if (nameIndex >= 0)
        parsed.name = parsed.directives[nameIndex].value;
    parsed.ok = true;
    return parsed;
}

// ============================================================================
// Edit
// ============================================================================

namespace {

/// The text without the lines of the given keys: what an edit must not change
QString restOf(const BResourceText::Parsed &parsed, const QSet<QString> &keys)
{
    QSet<int> skip;
    for (const BResourceText::Directive &directive : parsed.directives) {
        if (keys.contains(directive.normalized))
            skip.insert(directive.lineIndex);
    }
    QString out;
    for (int i = 0; i < parsed.lines.size(); ++i) {
        if (!skip.contains(i))
            out += parsed.lines[i];
    }
    return out;
}

} // namespace

bool BResourceText::edit(const QString &raw, const QList<Change> &changes, QString *result, QString *error)
{
    auto fail = [error](const QString &text) {
        if (error)
            *error = text;
        return false;
    };
    const Parsed parsed = parse(raw);
    if (!parsed.ok)
        return fail(parsed.error);

    QHash<int, QString> replaced;   // line index -> new line ("" removes it)
    QStringList inserted;           // new lines in front of the closing brace
    QSet<QString> keys;
    for (const Change &change : changes) {
        const QString key = normalizedKey(change.key);
        if (key.isEmpty())
            return fail(tr("A change without a directive name."));
        if (keys.contains(key))
            return fail(tr("Directive %1 is changed twice.").arg(change.key));
        keys.insert(key);
        if (!change.remove) {
            if (change.value.contains(QLatin1Char('\n')) || change.value.contains(QLatin1Char('\r')))
                return fail(tr("The value of %1 contains a line break.").arg(change.key));
            if (change.value.isEmpty())
                return fail(tr("An empty value for %1; remove the directive instead.").arg(change.key));
        }
        if (parsed.opaqueKeys.contains(key))
            return fail(tr("Directive %1 is on a line that cannot be edited safely (several directives "
                           "on one line, a list or a nested block). Change it in the text.").arg(change.key));
        const int index = parsed.find(change.key);
        if (index == -2)
            return fail(tr("Directive %1 appears more than once. Change it in the text.").arg(change.key));
        if (index >= 0) {
            const Directive &directive = parsed.directives[index];
            const QString &line = parsed.lines[directive.lineIndex];
            if (change.remove) {
                replaced.insert(directive.lineIndex, QString());
                continue;
            }
            if (directive.value == change.value)
                continue;
            const bool quote = directive.quoted || needsQuotes(change.value);
            const QString value = quote ? BQuote::quoted(change.value) : change.value;
            replaced.insert(directive.lineIndex,
                            line.left(directive.valueStart) + value + line.mid(directive.valueEnd));
            continue;
        }
        if (change.remove)
            continue;   // not set here: nothing to remove

        // New: the indentation and line end of the last directive
        QString indent = QStringLiteral("  ");
        QString end = parsed.lineEnd;
        if (!parsed.directives.isEmpty()) {
            const Directive &last = parsed.directives.last();
            const QString body = lineBody(parsed.lines[last.lineIndex]);
            int k = 0;
            while (k < body.size() && body[k].isSpace())
                ++k;
            indent = body.left(k);
            if (!parsed.lineEnds[last.lineIndex].isEmpty())
                end = parsed.lineEnds[last.lineIndex];
        }
        const bool quote = needsQuotes(change.value) || key == QLatin1String("name");
        inserted.append(indent + change.key + QStringLiteral(" = ")
                        + (quote ? BQuote::quoted(change.value) : change.value) + end);
    }

    QString out;
    for (int i = 0; i < parsed.lines.size(); ++i) {
        if (i == parsed.closingLine) {
            for (const QString &line : std::as_const(inserted))
                out += line;
        }
        const auto it = replaced.constFind(i);
        if (it == replaced.constEnd())
            out += parsed.lines[i];
        else
            out += it.value();
    }

    // Read back and check
    const Parsed check = parse(out);
    if (!check.ok)
        return fail(tr("The new text could not be read back: %1").arg(check.error));
    for (const Change &change : changes) {
        const int index = check.find(change.key);
        if (change.remove) {
            if (index != -1)
                return fail(tr("The new text still has directive %1.").arg(change.key));
        } else if (index < 0 || check.directives[index].value != change.value) {
            return fail(tr("The new text does not have the intended value of %1.").arg(change.key));
        }
    }
    if (restOf(check, keys) != restOf(parsed, keys))
        return fail(tr("The new text would change more than the edited directives."));
    if (check.typeWord != parsed.typeWord)
        return fail(tr("The new text would change the type of the resource."));
    if (!keys.contains(QStringLiteral("name")) && check.name != parsed.name)
        return fail(tr("The new text would change the name of the resource."));

    if (result)
        *result = out;
    return true;
}

QString BResourceText::createText(const QString &typeWord, const QList<Change> &directives,
                                  const QString &lineEnd)
{
    QString text = typeWord + QStringLiteral(" {") + lineEnd;
    for (const Change &change : directives) {
        if (change.remove || change.value.isEmpty())
            continue;
        const bool quote = needsQuotes(change.value) || normalizedKey(change.key) == QLatin1String("name");
        text += QStringLiteral("  ") + change.key + QStringLiteral(" = ")
              + (quote ? BQuote::quoted(change.value) : change.value) + lineEnd;
    }
    text += QStringLiteral("}") + lineEnd;
    return text;
}

// ============================================================================
// Diff
// ============================================================================

QList<BResourceText::DiffLine> BResourceText::diff(const QString &oldText, const QString &newText)
{
    QStringList a;
    QStringList b;
    splitLines(oldText, &a, nullptr, nullptr);
    splitLines(newText, &b, nullptr, nullptr);
    for (QString &line : a)
        line = lineBody(line);
    for (QString &line : b)
        line = lineBody(line);

    QList<DiffLine> out;
    const int n = int(a.size());
    const int m = int(b.size());
    // Resource texts are short; a long one is shown as replaced
    if (qint64(n) * qint64(m) > 4000000) {
        for (const QString &line : std::as_const(a))
            out.append(DiffLine{DiffLine::Removed, line});
        for (const QString &line : std::as_const(b))
            out.append(DiffLine{DiffLine::Added, line});
        return out;
    }
    // lcs[i][j]: length of the common subsequence of a[i..] and b[j..]
    QList<QList<int>> lcs(n + 1, QList<int>(m + 1, 0));
    for (int i = n - 1; i >= 0; --i) {
        for (int j = m - 1; j >= 0; --j)
            lcs[i][j] = a[i] == b[j] ? lcs[i + 1][j + 1] + 1 : qMax(lcs[i + 1][j], lcs[i][j + 1]);
    }
    int i = 0;
    int j = 0;
    while (i < n && j < m) {
        if (a[i] == b[j]) {
            out.append(DiffLine{DiffLine::Same, a[i]});
            ++i;
            ++j;
        } else if (lcs[i + 1][j] >= lcs[i][j + 1]) {
            out.append(DiffLine{DiffLine::Removed, a[i++]});
        } else {
            out.append(DiffLine{DiffLine::Added, b[j++]});
        }
    }
    while (i < n)
        out.append(DiffLine{DiffLine::Removed, a[i++]});
    while (j < m)
        out.append(DiffLine{DiffLine::Added, b[j++]});
    return out;
}
