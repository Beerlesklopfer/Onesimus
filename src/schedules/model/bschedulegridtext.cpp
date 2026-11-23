// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bschedulegridtext.cpp
 * @brief What the schedule grid says in words, and the dates of its calendar
 */

#include "schedules/model/bschedulegridtext.h"

#include "schedules/model/bscheduleoccurrences.h"

#include <functional>

namespace {

struct BitRange
{
    int first = 0;
    int last = 0;
};

/// The set bits as runs of neighbours, in ascending order
QList<BitRange> rangesOf(quint64 mask, int count)
{
    QList<BitRange> ranges;
    int i = 0;
    while (i < count) {
        if (!(mask & (Q_UINT64_C(1) << i))) {
            ++i;
            continue;
        }
        int end = i;
        while (end + 1 < count && (mask & (Q_UINT64_C(1) << (end + 1))))
            ++end;
        ranges.append(BitRange{i, end});
        i = end + 1;
    }
    return ranges;
}

BDateTimeMask maskOf(const BGridScope &scope)
{
    BDateTimeMask mask = BDateTimeMask::all();
    mask.month = scope.months;
    mask.wom = scope.weeksOfMonth;
    mask.last7DaysOfMonth = scope.last;
    return mask;
}

/// "<first>/<interval>" of a modulo as the writer spells it
bool moduloOf(const QString &text, int *first, int *interval)
{
    const int slash = text.indexOf(QLatin1Char('/'));
    if (slash < 0)
        return false;
    QString a = text.left(slash);
    QString b = text.mid(slash + 1);
    if (a.startsWith(QLatin1Char('w')))
        a.remove(0, 1);
    if (b.startsWith(QLatin1Char('w')))
        b.remove(0, 1);
    bool okA = false;
    bool okB = false;
    *first = a.toInt(&okA);
    *interval = b.toInt(&okB);
    return okA && okB;
}

} // namespace

// --- Names -------------------------------------------------------------------

QString BScheduleGridText::dayName(Qt::DayOfWeek day)
{
    switch (day) {
    case Qt::Monday: return tr("Monday");
    case Qt::Tuesday: return tr("Tuesday");
    case Qt::Wednesday: return tr("Wednesday");
    case Qt::Thursday: return tr("Thursday");
    case Qt::Friday: return tr("Friday");
    case Qt::Saturday: return tr("Saturday");
    case Qt::Sunday: return tr("Sunday");
    }
    return QString();
}

QString BScheduleGridText::shortDayName(Qt::DayOfWeek day)
{
    switch (day) {
    case Qt::Monday: return tr("Mon");
    case Qt::Tuesday: return tr("Tue");
    case Qt::Wednesday: return tr("Wed");
    case Qt::Thursday: return tr("Thu");
    case Qt::Friday: return tr("Fri");
    case Qt::Saturday: return tr("Sat");
    case Qt::Sunday: return tr("Sun");
    }
    return QString();
}

QString BScheduleGridText::monthName(int month)
{
    switch (month) {
    case 1: return tr("January");
    case 2: return tr("February");
    case 3: return tr("March");
    case 4: return tr("April");
    case 5: return tr("May");
    case 6: return tr("June");
    case 7: return tr("July");
    case 8: return tr("August");
    case 9: return tr("September");
    case 10: return tr("October");
    case 11: return tr("November");
    case 12: return tr("December");
    }
    return QString();
}

QString BScheduleGridText::ordinal(int number)
{
    const int lastTwo = number % 100;
    if (lastTwo >= 11 && lastTwo <= 13)
        return tr("%1th").arg(number);
    switch (number % 10) {
    case 1: return tr("%1st").arg(number);
    case 2: return tr("%1nd").arg(number);
    case 3: return tr("%1rd").arg(number);
    }
    return tr("%1th").arg(number);
}

QString BScheduleGridText::timeText(int hour, int minute)
{
    return QStringLiteral("%1:%2").arg(hour, 2, 10, QLatin1Char('0'))
                                  .arg(minute, 2, 10, QLatin1Char('0'));
}

// --- Scopes ------------------------------------------------------------------

QString BScheduleGridText::scopeLabel(const BGridScope &scope)
{
    if (scope.isEveryWeek())
        return tr("Every week");
    const BDateTimeMask mask = maskOf(scope);
    QStringList parts;
    const QString months = BRunWriter::monthText(mask);
    if (!months.isEmpty())
        parts.append(months);
    const QString weeks = BRunWriter::weekOfMonthText(mask);
    if (!weeks.isEmpty())
        parts.append(weeks);
    return parts.join(QLatin1Char(' '));
}

QString BScheduleGridText::scopeBadge(const BGridScope &scope)
{
    if (scope.isEveryWeek())
        return tr("all");
    const BDateTimeMask mask = maskOf(scope);
    QStringList parts;
    const QString months = BRunWriter::monthText(mask);
    if (!months.isEmpty())
        parts.append(months);
    // The weeks without "last", so that shortening them leaves that word alone
    BDateTimeMask weeksOnly = mask;
    weeksOnly.last7DaysOfMonth = false;
    QString weeks = BRunWriter::weekOfMonthText(weeksOnly);
    if (scope.last && scope.weeksOfMonth == BDateTimeMask::AllWoms)
        weeks = QStringLiteral("1st-5th");
    if (weeks.contains(QLatin1Char('-')) || weeks.contains(QLatin1Char(','))) {
        // "2nd-5th" becomes "2-5": the digits say it
        for (const char *suffix : {"st", "nd", "rd", "th"})
            weeks.remove(QLatin1String(suffix));
    }
    if (scope.last)
        weeks += (weeks.isEmpty() ? QString() : QStringLiteral(" ")) + QStringLiteral("last");
    if (!weeks.isEmpty())
        parts.append(weeks);
    return parts.join(QLatin1Char(' '));
}

QString BScheduleGridText::scopePhrase(const BGridScope &scope)
{
    if (scope.isEveryWeek())
        return QString();
    if (scope.months == BDateTimeMask::AllMonths) {
        if (scope.last && scope.weeksOfMonth == 0)
            return tr("last week only");
        if (!scope.last) {
            switch (scope.weeksOfMonth) {
            case 1 << 0: return tr("first week only");
            case 1 << 1: return tr("second week only");
            case 1 << 2: return tr("third week only");
            case 1 << 3: return tr("fourth week only");
            case 1 << 4: return tr("fifth week only");
            default: break;
            }
        }
    }
    return tr("only %1").arg(scopeLabel(scope));
}

// --- Levels and brushes --------------------------------------------------------

QString BScheduleGridText::levelName(const std::optional<QString> &level)
{
    if (!level || level->isEmpty())
        return tr("level of the job");
    return *level;
}

QString BScheduleGridText::levelLetter(const std::optional<QString> &level)
{
    if (!level || level->isEmpty())
        return QStringLiteral("J");
    const QString code = levelColorCode(level);
    if (!code.isEmpty())
        return code;
    if (level->compare(QLatin1String("Base"), Qt::CaseInsensitive) == 0)
        return QStringLiteral("B");
    // Verify levels and whatever else: two letters, so that they are not
    // taken for one of the backup levels
    return level->left(1).toUpper() + level->mid(1, 1).toLower();
}

QString BScheduleGridText::levelColorCode(const std::optional<QString> &level)
{
    if (!level)
        return QString();
    if (level->compare(QLatin1String("Full"), Qt::CaseInsensitive) == 0)
        return QStringLiteral("F");
    if (level->compare(QLatin1String("Incremental"), Qt::CaseInsensitive) == 0)
        return QStringLiteral("I");
    if (level->compare(QLatin1String("Differential"), Qt::CaseInsensitive) == 0)
        return QStringLiteral("D");
    if (level->compare(QLatin1String("VirtualFull"), Qt::CaseInsensitive) == 0)
        return QStringLiteral("V");
    return QString();
}

bool BScheduleGridText::hasOverridesBesideLevel(const BRunOverrides &overrides)
{
    BRunOverrides other = overrides;
    other.level.reset();
    return !other.isEmpty();
}

QString BScheduleGridText::overridesText(const BRunOverrides &o)
{
    QStringList parts;
    if (o.pool) parts.append(tr("pool %1").arg(*o.pool));
    if (o.fullPool) parts.append(tr("full pool %1").arg(*o.fullPool));
    if (o.incrementalPool) parts.append(tr("incremental pool %1").arg(*o.incrementalPool));
    if (o.differentialPool) parts.append(tr("differential pool %1").arg(*o.differentialPool));
    if (o.nextPool) parts.append(tr("next pool %1").arg(*o.nextPool));
    if (o.storage) parts.append(tr("storage %1").arg(*o.storage));
    if (o.messages) parts.append(tr("messages %1").arg(*o.messages));
    if (o.priority) parts.append(tr("priority %1").arg(*o.priority));
    if (o.spoolData) parts.append(*o.spoolData ? tr("spool data yes") : tr("spool data no"));
    if (o.accurate) parts.append(*o.accurate ? tr("accurate yes") : tr("accurate no"));
    if (o.maxRunSchedTime) parts.append(tr("max run sched time %1").arg(*o.maxRunSchedTime));
    return parts.join(QLatin1String(", "));
}

QString BScheduleGridText::brushText(const BGridBrush &brush)
{
    QString text = tr("%1 at :%2").arg(levelName(brush.overrides.level))
                                  .arg(brush.minute, 2, 10, QLatin1Char('0'));
    const QString overrides = overridesText(brush.overrides);
    if (!overrides.isEmpty())
        text += QLatin1String(", ") + overrides;
    return text;
}

QString BScheduleGridText::runText(const BGridRun &run, int hour)
{
    QString text = tr("%1 at %2").arg(levelName(run.overrides.level), timeText(hour, run.minute));
    const QString overrides = overridesText(run.overrides);
    if (!overrides.isEmpty())
        text += QLatin1String(", ") + overrides;
    return text;
}

QString BScheduleGridText::brushTag(int index)
{
    QString tag;
    int n = index;
    do {
        tag.prepend(QChar(ushort('A' + n % 26)));
        n = n / 26 - 1;
    } while (n >= 0);
    return tag;
}

QStringList BScheduleGridText::brushTags(const QList<BGridBrush> &brushes)
{
    QStringList tags;
    int next = 0;
    for (const BGridBrush &brush : brushes) {
        tags.append(hasOverridesBesideLevel(brush.overrides) ? brushTag(next++) : QString());
    }
    return tags;
}

// --- Cells ---------------------------------------------------------------------

QString BScheduleGridText::cellPlace(Qt::DayOfWeek day, int hour)
{
    return tr("%1 %2 hours").arg(dayName(day)).arg(hour);
}

QString BScheduleGridText::cellName(const BScheduleGridModel &model, int row, int column)
{
    const BGridCell cell = model.cell(row, column);
    const QString place = cellPlace(model.dayOfWeekOfRow(row), column);
    if (cell.isEmpty())
        return tr("%1: no run").arg(place);

    const QList<BGridScopeInfo> &scopes = model.scopes();
    auto describe = [&](const BGridRun &run) {
        QString text = runText(run, column);
        const QString phrase = scopePhrase(scopes.value(run.scopeIndex).scope);
        if (!phrase.isEmpty())
            text += QLatin1String(", ") + phrase;
        if (run.withoutTime)
            text += QLatin1String(", ") + tr("the Run line names no time");
        return text;
    };

    QStringList inScope;
    for (const BGridRun &run : cell.inScope)
        inScope.append(describe(run));
    QStringList outOfScope;
    for (const BGridRun &run : cell.outOfScope)
        outOfScope.append(describe(run));

    QString text = place + QLatin1String(": ");
    if (inScope.isEmpty())
        text += tr("no run in this scope");
    else
        text += inScope.join(QLatin1String("; "));
    if (!outOfScope.isEmpty())
        text += QLatin1String("; ") + tr("in other scopes: %1").arg(outOfScope.join(QLatin1String("; ")));
    return text;
}

// --- Calendar rules --------------------------------------------------------------

QString BScheduleGridText::ruleTag(int index)
{
    return QStringLiteral("R%1").arg(index + 1);
}

namespace {

/// "a", "a and b", "a, b and c" (or with another last word)
QString joined(const QStringList &items, const QString &lastWord)
{
    if (items.size() <= 1)
        return items.value(0);
    return items.mid(0, items.size() - 1).join(QLatin1String(", "))
        + QLatin1Char(' ') + lastWord + QLatin1Char(' ') + items.last();
}

} // namespace

QString BScheduleGridText::ruleReading(const BGridCalendarRule &rule)
{
    const BDateTimeMask &mask = rule.spec.mask;
    const QString andWord = tr("and");
    const QString orWord = tr("or");

    // Runs of three or more neighbours are read as "a to b"
    auto listed = [&](quint64 bits, int count, const std::function<QString(int)> &name,
                      const QString &lastWord) {
        QStringList items;
        for (const BitRange &range : rangesOf(bits, count)) {
            if (range.last - range.first >= 2) {
                items.append(tr("%1 to %2").arg(name(range.first), name(range.last)));
            } else {
                for (int i = range.first; i <= range.last; ++i)
                    items.append(name(i));
            }
        }
        return joined(items, lastWord);
    };

    const quint16 months = mask.month & BDateTimeMask::AllMonths;
    const bool allMonths = months == BDateTimeMask::AllMonths;
    const QString monthList = listed(months, 12, [](int i) { return monthName(i + 1); }, andWord);

    QStringList parts;

    // --- Days of the month ---
    const QString mdayText = BRunWriter::dayOfMonthText(mask);
    if (!mdayText.isEmpty()) {
        int first = 0;
        int interval = 0;
        QString days;
        if (moduloOf(mdayText, &first, &interval)) {
            days = tr("on every %1 day (starting with the %2)").arg(ordinal(interval), ordinal(first));
        } else {
            days = tr("on the %1").arg(listed(mask.mday & BDateTimeMask::AllMdays, 31,
                                              [](int i) { return ordinal(i + 1); }, andWord));
        }
        parts.append(allMonths ? tr("%1 of every month").arg(days)
                               : tr("%1 of %2").arg(days, monthList));
    } else if (!allMonths) {
        parts.append(tr("in %1").arg(monthList));
    }

    // --- Weeks of the year ---
    const QString woyText = BRunWriter::weekOfYearText(mask);
    if (!woyText.isEmpty()) {
        int first = 0;
        int interval = 0;
        if (moduloOf(woyText, &first, &interval)) {
            parts.append(tr("in every %1 week of the year (starting with week %2)")
                             .arg(ordinal(interval)).arg(first));
        } else {
            const quint64 weeks = mask.woy & BDateTimeMask::AllWoys;
            const QString list = listed(weeks, 54, [](int i) { return QString::number(i); }, andWord);
            int count = 0;
            for (int i = 0; i < 54; ++i)
                count += int((weeks >> i) & 1);
            parts.append(count == 1 ? tr("in week %1 of the year").arg(list)
                                    : tr("in weeks %1 of the year").arg(list));
        }
    }

    // --- Weeks of the month ---
    const quint8 woms = mask.wom & BDateTimeMask::AllWoms;
    if (mask.last7DaysOfMonth && woms == 0) {
        parts.append(tr("in the last seven days of the month"));
    } else if (woms != BDateTimeMask::AllWoms || mask.last7DaysOfMonth) {
        QString text = tr("in the %1 week of the month")
                           .arg(listed(woms, 5, [](int i) { return ordinal(i + 1); }, andWord));
        if (mask.last7DaysOfMonth)
            text += QLatin1Char(' ') + tr("or in its last seven days");
        parts.append(text);
    }

    // --- Days of the week ---
    const quint8 wdays = mask.wday & BDateTimeMask::AllWdays;
    if (wdays != BDateTimeMask::AllWdays) {
        // Bit 0 is Sunday; read from Monday on
        QStringList names;
        for (int day = Qt::Monday; day <= Qt::Sunday; ++day) {
            if (wdays & (1u << (day % 7)))
                names.append(dayName(Qt::DayOfWeek(day)));
        }
        if (rule.byDayOfMonth)
            parts.append(tr("only if it is a %1").arg(joined(names, orWord)));
        else
            parts.append(tr("on %1").arg(joined(names, andWord)));
    }

    // --- Time ---
    QString time;
    bool plainTime = false;     // "at 21:00": follows a single part without a comma
    const quint32 hours = mask.hour & BDateTimeMask::AllHours;
    if (rule.withoutTime) {
        time = tr("every hour at :00 (the line names no time)");
    } else if (hours == BDateTimeMask::AllHours) {
        time = tr("every hour at :%1").arg(rule.spec.minute, 2, 10, QLatin1Char('0'));
    } else {
        plainTime = true;
        QStringList times;
        for (int hour = 0; hour < 24; ++hour) {
            if (hours & (1u << hour))
                times.append(timeText(hour, rule.spec.minute));
        }
        time = tr("at %1").arg(joined(times, andWord));
    }

    QString text = parts.join(QLatin1String(", "));
    if (!text.isEmpty())
        text += parts.size() == 1 && plainTime ? QLatin1String(" ") : QLatin1String(", ");
    return text + time;
}

// --- Calendar ----------------------------------------------------------------------

QList<std::optional<QString>> BScheduleCalendarDay::levels() const
{
    QList<std::optional<QString>> result;
    for (const BScheduleCalendarRun &run : runs) {
        if (!result.contains(run.level))
            result.append(run.level);
    }
    return result;
}

QList<BScheduleCalendarDay> BScheduleCalendar::days(const BSchedule &schedule, const QDate &first,
                                                    int count)
{
    QList<BScheduleCalendarDay> result;
    if (!first.isValid() || count <= 0)
        return result;
    result.reserve(count);
    for (int i = 0; i < count; ++i) {
        BScheduleCalendarDay day;
        day.date = first.addDays(i);
        result.append(day);
    }

    const QList<BOccurrence> occurrences = BScheduleOccurrences::occurrences(
        schedule, first.startOfDay(), first.addDays(count).startOfDay());
    for (const BOccurrence &occurrence : occurrences) {
        const qint64 index = first.daysTo(occurrence.when.date());
        if (index < 0 || index >= count)
            continue;
        if (occurrence.run < 0 || occurrence.run >= schedule.runs.size())
            continue;
        const BRunLine &line = schedule.runs[occurrence.run];
        if (!line.spec)
            continue;
        BScheduleCalendarRun run;
        run.when = occurrence.when;
        run.run = occurrence.run;
        run.level = line.spec->overrides.level;
        result[int(index)].runs.append(run);
    }
    return result;
}

QDate BScheduleCalendar::weekStart(const QDate &date, Qt::DayOfWeek firstDayOfWeek)
{
    const int back = (date.dayOfWeek() - int(firstDayOfWeek) + 7) % 7;
    return date.addDays(-back);
}

QDate BScheduleCalendar::nextMonth(const QDate &first, Qt::DayOfWeek firstDayOfWeek)
{
    // The last day of the first week is in the month that week belongs to:
    // the week with the 1st of a month ends in that month
    const QDate reference = first.addDays(6);
    const QDate target = QDate(reference.year(), reference.month(), 1).addMonths(1);
    return weekStart(target, firstDayOfWeek);
}

QDate BScheduleCalendar::previousMonth(const QDate &first, Qt::DayOfWeek firstDayOfWeek)
{
    const QDate reference = first.addDays(6);
    QDate target = QDate(reference.year(), reference.month(), 1);
    if (weekStart(target, firstDayOfWeek) >= first)
        target = target.addMonths(-1);
    return weekStart(target, firstDayOfWeek);
}
