// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bscheduleresource.cpp
 * @brief The text of a Schedule resource and how a change of its Run lines
 *        is written into it
 */

#include "schedules/model/bscheduleresource.h"

#include "config/bresourcetext.h"

#include <QHash>
#include <QRegularExpression>

namespace {

// The lexical helpers are those of every resource text (BResourceText)
int commentStart(const QString &line) { return BResourceText::commentStart(line); }
QString body(const QString &line) { return BResourceText::lineBody(line); }
QString unquotedPart(const QString &line) { return BResourceText::unquotedPart(line); }
QString unescape(const QString &quotedBody) { return BResourceText::unescape(quotedBody); }

/// Everything of the text that is not a Run line, for the check after a rewrite
QString withoutRunLines(const BScheduleResource::Parsed &parsed)
{
    QString out;
    QList<bool> isRun(parsed.lines.size(), false);
    for (const BScheduleResource::RunDirective &run : parsed.runs)
        isRun[run.lineIndex] = true;
    for (int i = 0; i < parsed.lines.size(); ++i) {
        if (!isRun[i])
            out += parsed.lines[i];
    }
    return out;
}

} // namespace

QString BScheduleResource::quoted(const QString &value)
{
    return BResourceText::quoted(value);
}

QString BScheduleResource::nameProblem(const QString &name, const QStringList &taken)
{
    if (name.isEmpty())
        return tr("The name is empty.");
    if (name.size() > 127)
        return tr("The name is longer than 127 characters.");
    static const QRegularExpression valid(QStringLiteral("^[A-Za-z0-9_.:-]+$"));
    if (!valid.match(name).hasMatch())
        return tr("A name may contain only letters, digits and the characters _ - . : (no blanks).");
    for (const QString &other : taken) {
        if (other.compare(name, Qt::CaseInsensitive) == 0)
            return tr("There is already a schedule named %1.").arg(other);
    }
    return QString();
}

QString BScheduleResource::withName(const QString &raw, const QString &name)
{
    const Parsed parsed = parse(raw);
    if (!parsed.ok || parsed.name.isEmpty())
        return QString();
    // The value of the one Name directive: quoted (with escapes) or a word
    static const QRegularExpression nameRx(
        QStringLiteral("^([ \\t]*name[ \\t]*=[ \\t]*)(\"(?:[^\"\\\\\\r\\n]|\\\\.)*\"|[^\\s;#\"]+)"),
        QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
    QRegularExpressionMatchIterator it = nameRx.globalMatch(raw);
    if (!it.hasNext())
        return QString();
    const QRegularExpressionMatch match = it.next();
    if (it.hasNext())
        return QString();
    QString text = raw;
    text.replace(match.capturedStart(2), match.capturedLength(2), quoted(name));
    const Parsed check = parse(text);
    if (!check.ok || check.name != name || check.runs.size() != parsed.runs.size())
        return QString();
    return text;
}

BScheduleResource::Parsed BScheduleResource::parse(const QString &text)
{
    Parsed parsed;
    auto fail = [&parsed](const QString &error) {
        parsed.ok = false;
        parsed.error = error;
        return parsed;
    };

    // Lines, each with its own line end
    int start = 0;
    while (start < text.size()) {
        const int newline = text.indexOf(QLatin1Char('\n'), start);
        if (newline < 0) {
            parsed.lines.append(text.mid(start));
            parsed.lineEnds.append(QString());
            break;
        }
        const QString line = text.mid(start, newline - start + 1);
        parsed.lines.append(line);
        parsed.lineEnds.append(line.endsWith(QLatin1String("\r\n")) ? QStringLiteral("\r\n")
                                                                     : QStringLiteral("\n"));
        start = newline + 1;
    }
    parsed.lineEnd = QStringLiteral("\n");
    for (const QString &end : std::as_const(parsed.lineEnds)) {
        if (!end.isEmpty()) {
            parsed.lineEnd = end;
            break;
        }
    }

    static const QRegularExpression openRx(QStringLiteral("^\\s*schedule\\s*\\{(.*)$"),
                                           QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression runRx(QStringLiteral("^(\\s*run\\s*=\\s*)"),
                                          QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression anyRunRx(QStringLiteral("(^|[;{\\s])run\\s*="),
                                             QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression nameRx(QStringLiteral("^\\s*name\\s*=\\s*(.*?)\\s*;?\\s*$"),
                                           QRegularExpression::CaseInsensitiveOption);

    int depth = 0;
    bool opened = false;
    for (int i = 0; i < parsed.lines.size(); ++i) {
        const QString line = body(parsed.lines[i]);
        const int comment = commentStart(line);
        const QString code = comment >= 0 ? line.left(comment) : line;
        const QString blank = unquotedPart(code);

        if (!opened) {
            if (code.trimmed().isEmpty())
                continue;
            const QRegularExpressionMatch open = openRx.match(blank);
            if (!open.hasMatch())
                return fail(tr("The text does not start with a Schedule resource (line %1).").arg(i + 1));
            opened = true;
            depth = 1;
            if (!open.captured(1).trimmed().isEmpty())
                return fail(tr("Directives on the line of \"Schedule {\" cannot be edited safely (line %1).")
                                .arg(i + 1));
            continue;
        }
        if (depth == 0) {
            if (!code.trimmed().isEmpty())
                return fail(tr("Text after the closing brace of the resource (line %1).").arg(i + 1));
            continue;
        }
        if (blank.contains(QLatin1Char('{')))
            return fail(tr("A nested block in a Schedule resource (line %1).").arg(i + 1));
        if (blank.contains(QLatin1Char('}'))) {
            if (blank.trimmed() != QLatin1String("}"))
                return fail(tr("The closing brace shares its line with a directive (line %1).").arg(i + 1));
            depth = 0;
            parsed.closingLine = i;
            continue;
        }

        const QRegularExpressionMatch run = runRx.match(blank);
        if (run.hasMatch()) {
            RunDirective directive;
            directive.lineIndex = i;
            directive.valueStart = run.capturedEnd(1);
            // The value runs to the comment or the end of the line. A ";"
            // ends a directive; another one behind it is refused.
            QString rest = code.mid(directive.valueStart);
            int restEnd = rest.size();
            while (restEnd > 0 && rest[restEnd - 1].isSpace())
                --restEnd;
            rest.truncate(restEnd);
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
                if (close < 0)
                    return fail(tr("A quoted Run value does not end on its line (line %1).").arg(i + 1));
                const QString after = rest.mid(close + 1).trimmed();
                if (!after.isEmpty() && after != QLatin1String(";"))
                    return fail(tr("Something follows the Run value on its line (line %1).").arg(i + 1));
                directive.quoted = true;
                directive.value = unescape(rest.mid(1, close - 1));
                directive.valueEnd = directive.valueStart + close + 1;
            } else {
                const int semicolon = unquotedPart(rest).indexOf(QLatin1Char(';'));
                if (semicolon >= 0) {
                    if (!rest.mid(semicolon + 1).trimmed().isEmpty())
                        return fail(tr("A Run directive shares its line with another directive (line %1).")
                                        .arg(i + 1));
                    rest.truncate(semicolon);
                    while (!rest.isEmpty() && rest.back().isSpace())
                        rest.chop(1);
                }
                if (rest.isEmpty())
                    return fail(tr("A Run directive without a value (line %1).").arg(i + 1));
                directive.value = rest;
                directive.valueEnd = directive.valueStart + int(rest.size());
            }
            parsed.runs.append(directive);
            continue;
        }
        // A Run directive anywhere else on a line: not where this code
        // would look for it
        if (anyRunRx.match(blank).hasMatch())
            return fail(tr("A Run directive that does not start its line (line %1).").arg(i + 1));

        const QRegularExpressionMatch name = nameRx.match(code);
        if (name.hasMatch()) {
            QString value = name.captured(1);
            if (value.size() >= 2 && value.startsWith(QLatin1Char('"')) && value.endsWith(QLatin1Char('"')))
                value = unescape(value.mid(1, value.size() - 2));
            parsed.name = value;
        }
    }
    if (!opened)
        return fail(tr("The text contains no Schedule resource."));
    if (parsed.closingLine < 0)
        return fail(tr("The Schedule resource has no closing brace."));
    parsed.ok = true;
    return parsed;
}

bool BScheduleResource::runValues(const QString &text, QStringList *values, QString *error)
{
    const Parsed parsed = parse(text);
    if (!parsed.ok) {
        if (error)
            *error = parsed.error;
        return false;
    }
    if (values) {
        values->clear();
        for (const RunDirective &run : parsed.runs)
            values->append(run.value);
    }
    return true;
}

bool BScheduleResource::matchesLoaded(const Parsed &parsed, const QList<BRunLine> &loaded, QString *error)
{
    auto fail = [error](const QString &text) {
        if (error)
            *error = text;
        return false;
    };
    if (!parsed.ok)
        return fail(parsed.error);
    if (parsed.runs.size() != loaded.size()) {
        return fail(tr("The configuration file has %1 Run line(s), the schedule as it was loaded %2.")
                        .arg(parsed.runs.size()).arg(loaded.size()));
    }
    for (int i = 0; i < loaded.size(); ++i) {
        const QString &file = parsed.runs[i].value;
        if (file == loaded[i].original)
            continue;
        const BRunLine read = BRunParser::parse(file);
        if (read.spec && loaded[i].spec && *read.spec == *loaded[i].spec)
            continue;
        return fail(tr("Run line %1 differs: the file says \"%2\", the schedule as it was loaded \"%3\".")
                        .arg(i + 1).arg(file, loaded[i].original));
    }
    return true;
}

bool BScheduleResource::rewrite(const QString &raw, const QList<Line> &lines, QString *result, QString *error)
{
    auto fail = [error](const QString &text) {
        if (error)
            *error = text;
        return false;
    };
    const Parsed parsed = parse(raw);
    if (!parsed.ok)
        return fail(parsed.error);

    // Origins: in range, each once, rising
    int lastOrigin = -1;
    for (const Line &line : lines) {
        if (line.origin < 0) {
            if (line.text.trimmed().isEmpty())
                return fail(tr("A new Run line without text."));
            continue;
        }
        if (line.origin >= parsed.runs.size())
            return fail(tr("A Run line refers to line %1, which the file does not have.").arg(line.origin + 1));
        if (line.origin <= lastOrigin)
            return fail(tr("The Run lines would change their order; that is not written."));
        lastOrigin = line.origin;
        if (!line.keep && line.text.trimmed().isEmpty())
            return fail(tr("A changed Run line without text."));
    }
    for (const Line &line : lines) {
        if (line.text.contains(QLatin1Char('\n')) || line.text.contains(QLatin1Char('\r')))
            return fail(tr("A Run line contains a line break."));
    }

    // The new Run lines that follow each line of the file (key: index of
    // the file line, -1 = in front of the first kept line)
    QHash<int, QList<int>> followers;   // file line -> indices into lines
    int anchorLine = -1;                // the line the next new Run lines follow
    QList<int> leading;                 // new lines before any line of the file
    for (int i = 0; i < lines.size(); ++i) {
        const Line &line = lines[i];
        if (line.origin >= 0) {
            anchorLine = parsed.runs[line.origin].lineIndex;
            continue;
        }
        if (anchorLine < 0)
            leading.append(i);
        else
            followers[anchorLine].append(i);
    }

    // Where the leading new lines go: in front of the first Run line of the
    // file that stays, else where the first Run line was, else in front of
    // the closing brace
    int leadingBefore = parsed.closingLine;
    for (const Line &line : lines) {
        if (line.origin >= 0) {
            leadingBefore = parsed.runs[line.origin].lineIndex;
            break;
        }
    }
    if (leadingBefore == parsed.closingLine && !parsed.runs.isEmpty())
        leadingBefore = parsed.runs.first().lineIndex;

    // The style of new lines: that of the nearest Run line, else two spaces
    auto styleLine = [&parsed](int near) -> const RunDirective * {
        const RunDirective *best = nullptr;
        for (const RunDirective &run : parsed.runs) {
            if (!best || qAbs(run.lineIndex - near) < qAbs(best->lineIndex - near))
                best = &run;
        }
        return best;
    };
    auto newLine = [&](const QString &value, int near) {
        const RunDirective *style = styleLine(near);
        QString prefix = QStringLiteral("  Run = ");
        bool quote = false;
        QString end = parsed.lineEnd;
        if (style) {
            prefix = body(parsed.lines[style->lineIndex]).left(style->valueStart);
            quote = style->quoted;
            end = parsed.lineEnds[style->lineIndex].isEmpty() ? parsed.lineEnd
                                                               : parsed.lineEnds[style->lineIndex];
        }
        return prefix + (quote ? quoted(value) : value) + end;
    };

    // Which file lines stay, change or go
    QHash<int, const Line *> byFileLine;
    for (const Line &line : lines) {
        if (line.origin >= 0)
            byFileLine.insert(parsed.runs[line.origin].lineIndex, &line);
    }
    QHash<int, const RunDirective *> runAt;
    for (const RunDirective &run : parsed.runs)
        runAt.insert(run.lineIndex, &run);

    QString out;
    QStringList expected;   // the Run values the new text must have
    for (int i = 0; i < parsed.lines.size(); ++i) {
        if (i == leadingBefore) {
            for (int index : std::as_const(leading)) {
                out += newLine(lines[index].text, i);
                expected.append(lines[index].text);
            }
        }
        const RunDirective *run = runAt.value(i, nullptr);
        if (!run) {
            out += parsed.lines[i];
            continue;
        }
        const Line *line = byFileLine.value(i, nullptr);
        if (line) {
            if (line->keep) {
                out += parsed.lines[i];
                expected.append(run->value);
            } else {
                const QString &text = parsed.lines[i];
                const QString value = run->quoted ? quoted(line->text) : line->text;
                out += text.left(run->valueStart) + value + text.mid(run->valueEnd);
                expected.append(line->text);
            }
        }
        // else: removed, with its line end
        for (int index : followers.value(i)) {
            out += newLine(lines[index].text, i);
            expected.append(lines[index].text);
        }
    }

    // Read back and check
    const Parsed check = parse(out);
    if (!check.ok)
        return fail(tr("The new text could not be read back: %1").arg(check.error));
    QStringList written;
    for (const RunDirective &run : check.runs)
        written.append(run.value);
    if (written != expected)
        return fail(tr("The new text does not have the intended Run lines."));
    if (withoutRunLines(check) != withoutRunLines(parsed))
        return fail(tr("The new text would change more than the Run lines."));
    if (check.name != parsed.name)
        return fail(tr("The new text would change the name of the schedule."));

    if (result)
        *result = out;
    return true;
}

QString BScheduleResource::createText(const QString &name, const QString &description,
                                      const QStringList &runs, const QString &lineEnd)
{
    return createText(name, description, true, runs, lineEnd);
}

QString BScheduleResource::createText(const QString &name, const QString &description, bool enabled,
                                      const QStringList &runs, const QString &lineEnd)
{
    QString text = QStringLiteral("Schedule {") + lineEnd;
    text += QStringLiteral("  Name = ") + quoted(name) + lineEnd;
    if (!description.isEmpty())
        text += QStringLiteral("  Description = ") + quoted(description) + lineEnd;
    if (!enabled)
        text += QStringLiteral("  Enabled = no") + lineEnd;
    for (const QString &run : runs)
        text += QStringLiteral("  Run = ") + run + lineEnd;
    text += QStringLiteral("}") + lineEnd;
    return text;
}
