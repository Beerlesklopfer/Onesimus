// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

/**
 * @file bscheduleoccurrences.cpp
 * @brief When the Run lines of a schedule trigger, on concrete dates
 *
 * See the header for the rules and where they are taken from.
 */

#include "schedules/model/bscheduleoccurrences.h"

#include <QTimeZone>

#include <algorithm>

namespace {

const qint64 secondsPerHour = 3600;

} // namespace

int BScheduleOccurrences::weekOfMonth(const QDate &date)
{
    // date_time.cc DateTime::WeekOfMonth(): day_of_month / 7, day from 0
    return (date.day() - 1) / 7;
}

bool BScheduleOccurrences::inLast7DaysOfMonth(const QDate &date)
{
    // date_time.cc DateTime::OnLast7DaysOfMonth(): last_day - 7 < day_of_year.
    // Bareos takes the leap year from tm_year (years since 1900); that is
    // the calendar's leap year for every year from 1901 to 2299, and the
    // calendar is used here.
    return date.day() > date.daysInMonth() - 7;
}

namespace {

/// lib/btime.cc TmWoy(), kept step by step. @p jan4 is the day TmWoy()
/// gets when it asks for 4 January. fty - 4 is the day of the year (from 0,
/// it can be negative) of the Monday of the week that contains that day:
/// the first day of week 1.
int tmWoy(const QDate &date, const QDate &jan4)
{
    const int wday = jan4.dayOfWeek() % 7;   // 0 = Sunday
    int fty = 1 - wday;
    if (fty <= 0)
        fty += 7;
    const int woy = (date.dayOfYear() - 1) - fty + 4;
    if (woy < 0)
        return 0;
    return 1 + woy / 7;
}

/// Every date mask but the week of the year
bool dayMasksMatch(const BDateTimeMask &mask, const QDate &date)
{
    // date_time_mask.cc DateTimeMask::TriggersOnDay()
    const int wday = date.dayOfWeek() % 7;   // Qt: 1 = Monday ... 7 = Sunday; Bareos: 0 = Sunday
    return (mask.mday & (1u << (date.day() - 1)))
        && (mask.wday & (1u << wday))
        && (mask.month & (1u << (date.month() - 1)))
        && ((mask.wom & (1u << BScheduleOccurrences::weekOfMonth(date)))
            || (mask.last7DaysOfMonth && BScheduleOccurrences::inLast7DaysOfMonth(date)));
}

bool weekMatches(const BDateTimeMask &mask, int week)
{
    return mask.woy & (Q_UINT64_C(1) << week);
}

} // namespace

int BScheduleOccurrences::weekOfYear(const QDate &date)
{
    return tmWoy(date, QDate(date.year(), 1, 4));
}

int BScheduleOccurrences::weekOfYear(const QDate &date, int hour, const QTimeZone &zone)
{
    // TmWoy() takes the broken-down local time, sets the date to 4 January
    // and tm_isdst to 0 ("4 Jan is not DST") and lets mktime() normalise
    // it. Where 4 January is in daylight-saving time, glibc reads that as
    // standard time and returns the moment that is later by the shift: in
    // the last hour of the day, 5 January.
    QDate jan4(date.year(), 1, 4);
    if (zone.isValid()) {
        const int shift = zone.daylightTimeOffset(QDateTime(jan4, QTime(12, 0), zone));
        const int seconds = hour * 3600 + shift;
        if (seconds >= 24 * 3600)
            jan4 = jan4.addDays(1);
        else if (seconds < 0)
            jan4 = jan4.addDays(-1);
    }
    return tmWoy(date, jan4);
}

bool BScheduleOccurrences::triggersOnDay(const BDateTimeMask &mask, const QDate &date)
{
    if (!date.isValid())
        return false;
    return dayMasksMatch(mask, date) && weekMatches(mask, weekOfYear(date));
}

bool BScheduleOccurrences::triggersOn(const BRunSpec &spec, const QDate &date, int hour)
{
    if (hour < 0 || hour > 23)
        return false;
    // date_time_mask.cc DateTimeMask::TriggersOnDayAndHour()
    return triggersOnDay(spec.mask, date) && (spec.mask.hour & (1u << hour));
}

bool BScheduleOccurrences::triggersOn(const BRunSpec &spec, const QDate &date, int hour,
                                      const QTimeZone &zone)
{
    if (hour < 0 || hour > 23 || !date.isValid())
        return false;
    return (spec.mask.hour & (1u << hour))
        && dayMasksMatch(spec.mask, date)
        && weekMatches(spec.mask, weekOfYear(date, hour, zone));
}

QList<BOccurrence> BScheduleOccurrences::occurrences(const BSchedule &schedule,
                                                     const QDateTime &from, const QDateTime &to)
{
    QList<BOccurrence> result;
    if (!from.isValid() || !to.isValid() || from >= to)
        return result;

    const QTimeZone zone = from.timeZone();
    const qint64 first = from.toSecsSinceEpoch();
    const qint64 end = to.toSecsSinceEpoch();

    // scheduler_private.cc: the Director looks at the hours as they pass
    // and asks for each whether its local date and hour match. Stepping
    // through absolute time does the same: a local hour skipped by a
    // daylight-saving change is never asked for, a repeated one twice.
    const QTime firstTime = QDateTime::fromSecsSinceEpoch(first, zone).time();
    qint64 t = first - firstTime.minute() * 60 - firstTime.second();

    for (; t < end; t += secondsPerHour) {
        const QDateTime local = QDateTime::fromSecsSinceEpoch(t, zone);
        const QDate date = local.date();
        const QTime time = local.time();
        // Start of this local hour, in the offset that holds in it
        // (CalculateRuntime(): mktime with tm_min and tm_sec replaced)
        const qint64 hourStart = t - time.minute() * 60 - time.second();

        for (int i = 0; i < schedule.runs.size(); ++i) {
            const BRunLine &line = schedule.runs[i];
            if (!line.spec || !triggersOn(*line.spec, date, time.hour(), zone))
                continue;
            const qint64 start = hourStart + qint64(line.spec->minute) * 60;
            if (start < first || start >= end)
                continue;
            BOccurrence occurrence;
            occurrence.when = QDateTime::fromSecsSinceEpoch(start, zone);
            occurrence.run = i;
            result.append(occurrence);
        }
    }

    std::stable_sort(result.begin(), result.end(),
                     [](const BOccurrence &a, const BOccurrence &b) {
        if (a.when != b.when)
            return a.when < b.when;
        return a.run < b.run;
    });
    return result;
}

QDateTime BScheduleOccurrences::next(const BRunSpec &spec, const QDateTime &from, int days)
{
    if (!from.isValid())
        return QDateTime();

    BSchedule single;
    BRunLine line;
    line.spec = spec;
    single.runs.append(line);

    const QTimeZone zone = from.timeZone();
    const QDate firstDay = from.date();
    for (int d = 0; d <= days; ++d) {
        const QDate day = firstDay.addDays(d);
        // A quick look at the day before the hours are gone through. The
        // Director's week of the year can differ by one from the calendar's
        // (weekOfYear(date, hour, zone)), so a neighbouring week passes too.
        BDateTimeMask dayMask = spec.mask;
        const int week = weekOfYear(day);
        for (int w = qMax(0, week - 1); w <= qMin(53, week + 1); ++w) {
            if (weekMatches(dayMask, w))
                dayMask.woy = BDateTimeMask::AllWoys;
        }
        if (!triggersOnDay(dayMask, day))
            continue;
        const QDateTime dayStart = day.startOfDay(zone);
        const QDateTime dayEnd = day.addDays(1).startOfDay(zone);
        const QList<BOccurrence> found =
            occurrences(single, qMax(from, dayStart), dayEnd);
        if (!found.isEmpty())
            return found.first().when;
    }
    return QDateTime();
}

QList<BRunOverlap> BScheduleOccurrences::overlaps(
    const QList<QDateTime> &startsA, int durationA,
    const QList<QDateTime> &startsB, int durationB)
{
    QList<BRunOverlap> result;
    for (const QDateTime &a : startsA) {
        const QDateTime aEnd = a.addSecs(durationA);
        for (const QDateTime &b : startsB) {
            if (a < b.addSecs(durationB) && b < aEnd) {
                BRunOverlap overlap;
                overlap.a = a;
                overlap.b = b;
                result.append(overlap);
            }
        }
    }
    std::stable_sort(result.begin(), result.end(),
                     [](const BRunOverlap &x, const BRunOverlap &y) {
        return x.start() < y.start();
    });
    return result;
}
