// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file brunwriter.cpp
 * @brief Writes a Bareos Run line in canonical form
 *
 * Bareos reads the overrides first and only date and time words after them,
 * so the overrides are written first. What is written is always checked by
 * parsing it again (lineText()).
 */

#include "schedules/model/brunspec.h"
#include "bquote.h"

#include <QRegularExpression>

namespace {

const char *const wdayNames[] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
const char *const monthNames[] = {"jan", "feb", "mar", "apr", "may", "jun",
                                  "jul", "aug", "sep", "oct", "nov", "dec"};
const char *const womNames[] = {"1st", "2nd", "3rd", "4th", "5th"};

/**
 * The set bits as a comma separated list; three or more neighbours become a
 * range. Ranges are written in ascending order only, never around the end.
 */
template <typename Name>
QString listOf(quint64 mask, int count, Name name)
{
    QStringList parts;
    int i = 0;
    while (i < count) {
        if (!(mask & (Q_UINT64_C(1) << i))) {
            ++i;
            continue;
        }
        int end = i;
        while (end + 1 < count && (mask & (Q_UINT64_C(1) << (end + 1))))
            ++end;
        if (end - i >= 2) {
            parts.append(name(i) + QLatin1Char('-') + name(end));
        } else {
            for (int k = i; k <= end; ++k)
                parts.append(name(k));
        }
        i = end + 1;
    }
    return parts.join(QLatin1Char(','));
}

QString weekName(int week)
{
    return QStringLiteral("w%1").arg(week, 2, 10, QLatin1Char('0'));
}

/// "<first>/<interval>" if the days are exactly what a day modulo sets
QString mdayModulo(quint32 mdays)
{
    for (int interval = 2; interval <= 30; ++interval) {
        for (int first = 0; first <= interval && first <= 30; ++first) {
            quint32 mask = 0;
            for (int i = 0; i < 31; ++i) {
                if (i % interval == 0 && i + first <= 30)
                    mask |= (1u << (i + first));
            }
            if (mask == mdays)
                return QStringLiteral("%1/%2").arg(first + 1).arg(interval);
        }
    }
    return QString();
}

/// "wNN/wNN" if the weeks are exactly what a week modulo sets
QString woyModulo(quint64 weeks)
{
    for (int interval = 2; interval <= 53; ++interval) {
        for (int first = 0; first <= interval; ++first) {
            quint64 mask = 0;
            for (int week = first; week <= 53; week += interval)
                mask |= (Q_UINT64_C(1) << week);
            if (mask == weeks)
                return weekName(first) + QLatin1Char('/') + weekName(interval);
        }
    }
    return QString();
}

int bitCount(quint64 mask)
{
    int count = 0;
    for (; mask; mask >>= 1)
        count += int(mask & 1);
    return count;
}

/// A resource name or other value, quoted unless it is a plain word
QString value(const QString &text)
{
    static const QRegularExpression plain(QStringLiteral("^[A-Za-z][A-Za-z0-9_.-]*$"));
    if (plain.match(text).hasMatch())
        return text;
    QString escaped = text;
    escaped.replace(QLatin1Char('\\'), QLatin1String("\\\\"));
    escaped.replace(QLatin1Char('"'), QLatin1String("\\\""));
    return QLatin1Char('"') + escaped + QLatin1Char('"');
}

void setError(QString *error, const QString &message)
{
    if (error)
        *error = message;
}

} // namespace

QString BRunWriter::weekOfMonthText(const BDateTimeMask &mask)
{
    const quint8 wom = mask.wom & BDateTimeMask::AllWoms;
    QStringList parts;
    // With "last" the weeks are always spelled out: "last" clears them first
    if (wom != BDateTimeMask::AllWoms || mask.last7DaysOfMonth) {
        const QString weeks = listOf(wom, 5, [](int i) { return QString::fromLatin1(womNames[i]); });
        if (!weeks.isEmpty())
            parts.append(weeks);
    }
    if (mask.last7DaysOfMonth)
        parts.append(QStringLiteral("last"));
    return parts.join(QLatin1Char(' '));
}

QString BRunWriter::monthText(const BDateTimeMask &mask)
{
    const quint16 months = mask.month & BDateTimeMask::AllMonths;
    if (months == BDateTimeMask::AllMonths)
        return QString();
    return listOf(months, 12, [](int i) { return QString::fromLatin1(monthNames[i]); });
}

QString BRunWriter::dayOfMonthText(const BDateTimeMask &mask)
{
    const quint32 mdays = mask.mday & BDateTimeMask::AllMdays;
    if (mdays == BDateTimeMask::AllMdays)
        return QString();
    QString days;
    if (bitCount(mdays) >= 3)
        days = mdayModulo(mdays);
    if (days.isEmpty())
        days = listOf(mdays, 31, [](int i) { return QString::number(i + 1); });
    return days;
}

QString BRunWriter::weekOfYearText(const BDateTimeMask &mask)
{
    const quint64 woys = mask.woy & BDateTimeMask::AllWoys;
    if (woys == BDateTimeMask::AllWoys)
        return QString();
    QString weeks;
    if (bitCount(woys) >= 3)
        weeks = woyModulo(woys);
    if (weeks.isEmpty())
        weeks = listOf(woys, 54, weekName);
    return weeks;
}

QString BRunWriter::write(const BRunSpec &spec, QString *error)
{
    setError(error, QString());
    const BDateTimeMask &m = spec.mask;

    const quint32 hours = m.hour & BDateTimeMask::AllHours;
    const quint32 mdays = m.mday & BDateTimeMask::AllMdays;
    const quint16 months = m.month & BDateTimeMask::AllMonths;
    const quint8 wdays = m.wday & BDateTimeMask::AllWdays;
    const quint8 woms = m.wom & BDateTimeMask::AllWoms;
    const quint64 woys = m.woy & BDateTimeMask::AllWoys;

    // A mask without a bit never triggers and no Run line says so
    if (!hours || !mdays || !months || !wdays || !woys || (!woms && !m.last7DaysOfMonth)) {
        setError(error, tr("The run has an empty hour, day, month or week set and cannot "
                           "be written as a Run line."));
        return QString();
    }
    if (spec.minute < 0 || spec.minute > 59) {
        setError(error, tr("The minute %1 is out of range.").arg(spec.minute));
        return QString();
    }

    QStringList parts;

    // --- Overrides ---
    const BRunOverrides &o = spec.overrides;
    if (o.level) parts.append(QStringLiteral("Level=") + *o.level);
    if (o.pool) parts.append(QStringLiteral("Pool=") + value(*o.pool));
    if (o.fullPool) parts.append(QStringLiteral("FullPool=") + value(*o.fullPool));
    if (o.incrementalPool) parts.append(QStringLiteral("IncrementalPool=") + value(*o.incrementalPool));
    if (o.differentialPool) parts.append(QStringLiteral("DifferentialPool=") + value(*o.differentialPool));
    if (o.nextPool) parts.append(QStringLiteral("NextPool=") + value(*o.nextPool));
    if (o.storage) parts.append(QStringLiteral("Storage=") + value(*o.storage));
    if (o.messages) parts.append(QStringLiteral("Messages=") + value(*o.messages));
    if (o.priority) parts.append(QStringLiteral("Priority=%1").arg(*o.priority));
    if (o.spoolData) parts.append(QStringLiteral("SpoolData=") + QLatin1String(*o.spoolData ? "yes" : "no"));
    if (o.accurate) parts.append(QStringLiteral("Accurate=") + QLatin1String(*o.accurate ? "yes" : "no"));
    if (o.maxRunSchedTime) {
        // Always quoted: a period may contain spaces ("1 hour 30 minutes")
        QString period = *o.maxRunSchedTime;
        period.replace(QLatin1Char('\\'), QLatin1String("\\\\"));
        period.replace(QLatin1Char('"'), QLatin1String("\\\""));
        parts.append(QStringLiteral("MaxRunSchedTime=\"") + period + QLatin1Char('"'));
    }

    // --- Date ---
    if (months != BDateTimeMask::AllMonths)
        parts.append(monthText(m));
    if (woys != BDateTimeMask::AllWoys)
        parts.append(weekOfYearText(m));
    const QString weekOfMonth = weekOfMonthText(m);
    if (!weekOfMonth.isEmpty())
        parts.append(weekOfMonth);
    if (wdays != BDateTimeMask::AllWdays) {
        parts.append(listOf(wdays, 7, [](int i) { return QString::fromLatin1(wdayNames[i]); }));
    }
    if (mdays != BDateTimeMask::AllMdays) {
        // "on": Bareos does not take a bare number directly behind the overrides
        parts.append(QStringLiteral("on ") + dayOfMonthText(m));
    }

    // --- Time ---
    const QString minute = QStringLiteral("%1").arg(spec.minute, 2, 10, QLatin1Char('0'));
    if (hours == BDateTimeMask::AllHours) {
        parts.append(QStringLiteral("hourly at 0:") + minute);
    } else {
        for (int h = 0; h < 24; ++h) {
            if (hours & (1u << h)) {
                parts.append(QStringLiteral("at %1:%2")
                                 .arg(h, 2, 10, QLatin1Char('0')).arg(minute));
            }
        }
    }

    return parts.join(QLatin1Char(' '));
}

QString BRunWriter::lineText(const BRunLine &line, QString *error)
{
    setError(error, QString());

    if (!line.spec) {
        setError(error, tr("Run line \"%1\" is not understood (%2) and is never rewritten.")
                            .arg(line.original, line.parseError));
        return QString();
    }

    // Untouched: byte-identical
    if (!line.dirty && !line.needsRewrite && !line.original.isEmpty())
        return line.original;

    QString writeError;
    const QString text = write(*line.spec, &writeError);
    if (text.isEmpty()) {
        setError(error, writeError);
        return QString();
    }

    // Read back what was written and compare it with what was intended
    const BRunLine check = BRunParser::parse(text);
    if (!check.spec) {
        setError(error, tr("The generated Run line \"%1\" does not read back: %2")
                            .arg(text, check.parseError));
        return QString();
    }
    if (*check.spec != *line.spec || check.needsRewrite) {
        setError(error, tr("The generated Run line \"%1\" does not say what was intended.")
                            .arg(text));
        return QString();
    }
    return text;
}

bool BRunWriter::runTexts(const QList<BRunLine> &runs, QStringList *texts, QString *error)
{
    setError(error, QString());
    QStringList result;
    result.reserve(runs.size());
    for (int i = 0; i < runs.size(); ++i) {
        QString lineError;
        const QString text = lineText(runs[i], &lineError);
        if (text.isEmpty()) {
            setError(error, tr("Run line %1: %2").arg(i + 1).arg(lineError));
            if (texts)
                texts->clear();
            return false;
        }
        result.append(text);
    }
    if (texts)
        *texts = result;
    return true;
}

bool BRunWriter::runTexts(const QStringList &reported, QStringList *texts, QString *error)
{
    QList<BRunLine> runs;
    runs.reserve(reported.size());
    for (const QString &text : reported)
        runs.append(BRunParser::parse(text));
    return runTexts(runs, texts, error);
}

QString BRunWriter::quotedForCommand(const QString &runText)
{
    return BQuote::quoted(runText);
}
