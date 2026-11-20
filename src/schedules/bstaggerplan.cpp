#include "schedules/bstaggerplan.h"

#include <QRegularExpression>

namespace BStaggerPlan {

namespace {

QString tr(const char *text)
{
    return QCoreApplication::translate("BStaggerPlan", text);
}

QString quoted(const QString &value)
{
    QString escaped = value;
    escaped.replace(QLatin1Char('\\'), QLatin1String("\\\\"));
    escaped.replace(QLatin1Char('"'), QLatin1String("\\\""));
    return QLatin1Char('"') + escaped + QLatin1Char('"');
}

} // namespace

bool shiftedRunLines(const BSchedule &schedule, int minutes, QStringList *lines, QString *error)
{
    auto fail = [error](const QString &why) {
        if (error) *error = why;
        return false;
    };
    QStringList out;
    if (schedule.runs.isEmpty())
        return fail(tr("Schedule %1 has no Run lines.").arg(schedule.name));
    for (int i = 0; i < schedule.runs.size(); ++i) {
        const BRunLine &line = schedule.runs[i];
        if (!line.spec)
            return fail(tr("Run line %1 of schedule %2 is not understood.").arg(i + 1).arg(schedule.name));
        const QList<int> hours = line.spec->hours();
        if (hours.size() != 1)
            return fail(tr("Run line %1 of schedule %2 runs at several hours.").arg(i + 1).arg(schedule.name));
        const int at = hours.first() * 60 + line.spec->minute + minutes;
        if (at >= 24 * 60)
            return fail(tr("Run line %1 of schedule %2 would move into the next day.")
                            .arg(i + 1).arg(schedule.name));
        BRunSpec spec = *line.spec;
        spec.mask.hour = 1u << (at / 60);
        spec.minute = at % 60;
        spec.haveAt = true;
        QString writeError;
        const QString text = BRunWriter::write(spec, &writeError);
        const BRunLine back = BRunParser::parse(text);
        if (text.isEmpty() || !back.spec || *back.spec != spec)
            return fail(tr("Run line %1 of schedule %2 did not read back as written (%3).")
                            .arg(i + 1).arg(schedule.name, writeError.isEmpty() ? text : writeError));
        out.append(text);
    }
    if (lines) *lines = out;
    return true;
}

QTime firstStart(const BSchedule &schedule, int minutes)
{
    int first = -1;
    for (const BRunLine &line : schedule.runs) {
        if (!line.spec) continue;
        for (int hour : line.spec->hours()) {
            const int at = hour * 60 + line.spec->minute + minutes;
            if (first < 0 || at < first) first = at;
        }
    }
    return first < 0 || first >= 24 * 60 ? QTime() : QTime(first / 60, first % 60);
}

QString scheduleName(const QString &base, const QTime &start, const QStringList &taken)
{
    const QString stem = start.isValid() ? base + QLatin1Char('-') + start.toString(QStringLiteral("HHmm"))
                                         : base + QLatin1String("-later");
    auto isTaken = [&taken](const QString &name) {
        for (const QString &t : taken) {
            if (t.compare(name, Qt::CaseInsensitive) == 0) return true;
        }
        return false;
    };
    QString name = stem;
    for (int n = 2; isTaken(name); ++n)
        name = stem + QLatin1Char('-') + QString::number(n);
    return name;
}

QString scheduleText(const QString &name, const QString &description, const QStringList &runLines)
{
    QString text = QStringLiteral("Schedule {\n  Name = %1\n").arg(quoted(name));
    if (!description.isEmpty())
        text += QStringLiteral("  Description = %1\n").arg(quoted(description));
    for (const QString &line : runLines)
        text += QStringLiteral("  Run = %1\n").arg(line);
    text += QStringLiteral("}\n");
    return text;
}

namespace {

/// "Schedule", "schedule", "Sche dule": Bareos ignores case and blanks in keywords
QRegularExpressionMatch scheduleDirective(const QString &jobText)
{
    static const QRegularExpression directive(
        QStringLiteral("^([ \\t]*)s[ \\t]*c[ \\t]*h[ \\t]*e[ \\t]*d[ \\t]*u[ \\t]*l[ \\t]*e[ \\t]*=[^\\r\\n]*"),
        QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
    return directive.match(jobText);
}

} // namespace

QString scheduleLine(const QString &jobText)
{
    const QRegularExpressionMatch match = scheduleDirective(jobText);
    return match.hasMatch() ? match.captured(0).trimmed() : QString();
}

QString jobWithSchedule(const QString &jobText, const QString &schedule)
{
    const int close = jobText.lastIndexOf(QLatin1Char('}'));
    if (close < 0)
        return QString();
    const QRegularExpressionMatch match = scheduleDirective(jobText);
    if (match.hasMatch() && match.capturedStart() < close) {
        QString text = jobText;
        text.replace(match.capturedStart(), match.capturedLength(),
                     match.captured(1) + QStringLiteral("Schedule = ") + quoted(schedule));
        return text;
    }
    // Before the closing brace, indented like the other directives
    const QString newline = jobText.contains(QLatin1String("\r\n")) ? QStringLiteral("\r\n")
                                                                     : QStringLiteral("\n");
    QString indent = QStringLiteral("  ");
    static const QRegularExpression firstDirective(QStringLiteral("\\n([ \\t]+)\\S"));
    const QRegularExpressionMatch m = firstDirective.match(jobText);
    if (m.hasMatch()) indent = m.captured(1);
    int insertAt = close;
    while (insertAt > 0 && (jobText[insertAt - 1] == QLatin1Char(' ') || jobText[insertAt - 1] == QLatin1Char('\t')))
        --insertAt;
    QString text = jobText;
    const bool atLineStart = insertAt == 0 || jobText[insertAt - 1] == QLatin1Char('\n');
    text.insert(insertAt, (atLineStart ? QString() : newline) + indent + QStringLiteral("Schedule = ")
                              + quoted(schedule) + newline);
    return text;
}

} // namespace BStaggerPlan
