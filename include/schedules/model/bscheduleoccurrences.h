#ifndef BSCHEDULEOCCURRENCES_H
#define BSCHEDULEOCCURRENCES_H

#include <QDate>
#include <QDateTime>
#include <QList>
#include <QTimeZone>

#include "schedules/model/brunspec.h"

/**
 * @file bscheduleoccurrences.h
 * @brief When the Run lines of a schedule trigger, on concrete dates
 *
 * A port of what the Bareos Director evaluates: dird/date_time.cc
 * (DateTime), dird/date_time_mask.cc (TriggersOnDay, TriggersOnDayAndHour),
 * lib/btime.cc (TmWoy) and the hourly stepping of dird/scheduler_private.cc
 * (AddJobsForThisAndNextHourToQueue, CalculateRuntime), read together with
 * the Rust port in onesimus-dird (crates/odir/src/run_time.rs).
 * QtCore only; no widgets and no Director.
 *
 * The rules:
 *  - Every mask must match: day of the month, day of the week (bit 0 =
 *    Sunday), month, week of the month, week of the year, hour. A part a
 *    Run line does not name has all its bits set.
 *  - Week of the month is (day of the month - 1) / 7: "1st" is the 1st to
 *    the 7th, "2nd" the 8th to the 14th, ..., "5th" the 29th to the 31st.
 *    It does not depend on the day of the week the month starts with.
 *  - "last" is the last seven days of the month and counts as a week of the
 *    month: a day matches if its week bit is set or if it is in the last
 *    seven days of a line that says "last".
 *  - Week of the year is the ISO week (Monday first, week 1 contains
 *    4 January) with two differences: the days of January before week 1 are
 *    week 0, not week 52 or 53 of the year before, and the days of December
 *    that ISO counts to week 1 of the next year keep counting (week 53).
 *  - Where 4 January is in daylight-saving time (the southern hemisphere),
 *    the Director counts the weeks differently in the last hour of the day:
 *    TmWoy() asks for "4 January at the same time of day, not daylight
 *    saving", which on glibc is one hour later and so, from 23:00 on,
 *    5 January. In that hour the weeks are numbered from the weekday of
 *    5 January. Both Directors (Bareos and the Rust port, same calls) do
 *    this; occurrences() follows them, because it has to say what will run.
 *  - Time is local time. The Director steps through the hours as they
 *    really pass, so at a daylight-saving change a run at a local time that
 *    does not exist on that date does not run, and a run at a local time
 *    that exists twice runs twice, one hour apart.
 */

/// One concrete start of one Run line
struct BOccurrence
{
    QDateTime when;     ///< The start, in the time zone the range was given in
    int run = -1;       ///< Index into BSchedule::runs
};

/// Two starts whose runs are running at the same time
struct BRunOverlap
{
    QDateTime a;    ///< Start of the run of the first series
    QDateTime b;    ///< Start of the run of the second series

    /// From when on both are running
    QDateTime start() const { return a < b ? b : a; }
};

class BScheduleOccurrences
{
public:
    /// 0 = "1st" (the 1st to the 7th) ... 4 = "5th" (the 29th to the 31st)
    static int weekOfMonth(const QDate &date);

    /// What "last" means: one of the last seven days of the month
    static bool inLast7DaysOfMonth(const QDate &date);

    /// 0 to 53 as Bareos counts (TmWoy), see the rules above
    static int weekOfYear(const QDate &date);

    /**
     * @brief The week of the year as the Director works it out in an hour
     *        of a date in a time zone
     *
     * The same as weekOfYear(date) unless 4 January is in daylight-saving
     * time in that zone and the hour is the last of the day (see the rules
     * above). A shift that is not a whole hour (Lord Howe Island) is
     * applied as if the minute were 0; the Director's result depends on
     * the minute at which its scheduler happens to look.
     */
    static int weekOfYear(const QDate &date, int hour, const QTimeZone &zone);

    /// Do all the date masks match on this date? The hour is not looked at;
    /// the week of the year is weekOfYear(date).
    static bool triggersOnDay(const BDateTimeMask &mask, const QDate &date);

    /// Does the run trigger in this hour (0 to 23, local time) of this date?
    /// The week of the year is weekOfYear(date).
    static bool triggersOn(const BRunSpec &spec, const QDate &date, int hour);

    /// The same with the week of the year as the Director works it out in
    /// that zone: weekOfYear(date, hour, zone). occurrences() uses this one.
    static bool triggersOn(const BRunSpec &spec, const QDate &date, int hour,
                           const QTimeZone &zone);

    /**
     * @brief The starts of all understood Run lines in [from, to)
     *
     * Local time is the time zone of @p from. Sorted by time, then by line.
     * A line that is not understood has no occurrence. The Enabled setting
     * of the schedule is not looked at.
     */
    static QList<BOccurrence> occurrences(const BSchedule &schedule,
                                          const QDateTime &from, const QDateTime &to);

    /**
     * @brief The first start of a run at or after @p from
     * @param days How many days to look ahead
     * @return an invalid QDateTime if there is none in that time (a line
     *         like "on 31 feb" never runs)
     */
    static QDateTime next(const BRunSpec &spec, const QDateTime &from, int days = 3660);

    /**
     * @brief Which runs of two series are running at the same time
     * @param startsA      Starts of the first series
     * @param durationA    How long each of them runs, in seconds
     * @param startsB      Starts of the second series
     * @param durationB    How long each of them runs, in seconds
     * @return every pair of starts whose runs overlap, ordered by the
     *         moment from which both are running. Two runs that only touch
     *         (one ends when the other starts) do not overlap.
     *
     * Works on concrete starts, so "1st sat" and "2nd-5th sat" at the same
     * time of day do not overlap. The durations are the caller's: this
     * function does not know where they come from.
     */
    static QList<BRunOverlap> overlaps(const QList<QDateTime> &startsA, int durationA,
                                       const QList<QDateTime> &startsB, int durationB);
};

#endif // BSCHEDULEOCCURRENCES_H
