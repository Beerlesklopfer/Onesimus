/**
 * @file bscheduleoccurrences_test.cpp
 * @brief When the Run lines of a schedule trigger, on concrete dates
 *
 * Where the expected dates come from, named at each group:
 *  - a calendar of 2020 to 2028 worked out by hand (the weekday of each
 *    date used here was checked against a second calendar), with the rules
 *    of Bareos applied to it: week of the month = (day - 1) / 7, "last" =
 *    the last seven days of the month, week of the year as lib/btime.cc
 *    TmWoy() counts it;
 *  - the tests of the Rust port (onesimus-dird, crates/odir/src/
 *    run_time.rs: weekdays_and_positions, month_and_year_boundaries,
 *    daylight_saving_changes), marked "Rust port";
 *  - for the week of the year in a zone where 4 January is in summer time:
 *    lib/btime.cc TmWoy() of Bareos, retyped and run on glibc with
 *    TZ=Australia/Sydney (the Director's own arithmetic, not a calendar).
 *
 * Every time zone is given explicitly; the zone of the machine is not used.
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>
#include <QTimeZone>

#include "models/bresourcemodels.h"
#include "schedules/bjobscheduleindex.h"
#include "schedules/model/bscheduleoccurrences.h"

namespace {

BSchedule scheduleOf(const QStringList &runs, const QString &name = QStringLiteral("S"))
{
    BSchedule schedule;
    schedule.name = name;
    for (const QString &run : runs)
        schedule.runs.append(BRunParser::parse(run));
    return schedule;
}

BRunSpec specOf(const QString &run)
{
    const BRunLine line = BRunParser::parse(run);
    if (!line.spec)
        qFatal("Run line not understood: %s (%s)", qPrintable(run), qPrintable(line.parseError));
    return *line.spec;
}

QDateTime at(const QDate &date, const QTimeZone &zone = QTimeZone::utc())
{
    return date.startOfDay(zone);
}

/// The starts in [from, to), dates from midnight to midnight, as text
QStringList starts(const BSchedule &schedule, const QDate &from, const QDate &to,
                   const QTimeZone &zone = QTimeZone::utc(),
                   const QString &format = QStringLiteral("yyyy-MM-dd HH:mm"))
{
    QStringList result;
    for (const BOccurrence &o : BScheduleOccurrences::occurrences(schedule, at(from, zone), at(to, zone)))
        result.append(o.when.toString(format));
    return result;
}

/// The dates on which one Run line starts in [from, to)
QStringList dates(const QString &run, const QDate &from, const QDate &to,
                  const QTimeZone &zone = QTimeZone::utc())
{
    const BSchedule schedule = scheduleOf({run});
    if (!schedule.runs.first().spec)
        qFatal("Run line not understood: %s", qPrintable(run));
    return starts(schedule, from, to, zone, QStringLiteral("yyyy-MM-dd"));
}

/// The starts of one Run line of a schedule in [from, to)
QList<QDateTime> startsOfRun(const BSchedule &schedule, int run, const QDate &from, const QDate &to)
{
    QList<QDateTime> result;
    for (const BOccurrence &o : BScheduleOccurrences::occurrences(schedule, at(from), at(to))) {
        if (o.run == run)
            result.append(o.when);
    }
    return result;
}

} // namespace

class BScheduleOccurrencesTest : public QObject
{
    Q_OBJECT

private slots:
    void weekOfMonthIsCountedFromTheFirstOfTheMonth();
    void firstWeekdayOfAMonth();
    void secondToFifthWeekdayOfAMonth();
    void lastWeekdayOfAMonth();
    void dayOfMonth();
    void monthRestriction();
    void sundayIsBitZero();
    void everyRestrictionMustMatch();
    void moduloDays();
    void moduloWeeks();
    void weekOfYear_data();
    void weekOfYear();
    void week00AndWeek53AtAYearBoundary();
    void weekOfYearWhereJanuaryIsSummer();
    void hourlyLine();
    void lineWithSeveralHours();
    void rangeIsHalfOpenAndSorted();
    void daylightSavingSpring();
    void daylightSavingAutumn();
    void nextRun();
    void firstAndOtherSaturdaysDoNotCollide();
    void realCollision();
    void lineNotUnderstoodHasNoOccurrence();
    void entriesOncePerJobAndRun();
};

// --- Week of the month: hand calendar and the Bareos rule ---

void BScheduleOccurrencesTest::weekOfMonthIsCountedFromTheFirstOfTheMonth()
{
    // date_time.cc: day_of_month / 7 with the day from 0. Not the calendar
    // week of the month: the 1st to the 7th are "1st" whatever weekday the
    // month starts with.
    QCOMPARE(BScheduleOccurrences::weekOfMonth(QDate(2026, 5, 1)), 0);
    QCOMPARE(BScheduleOccurrences::weekOfMonth(QDate(2026, 5, 7)), 0);
    QCOMPARE(BScheduleOccurrences::weekOfMonth(QDate(2026, 5, 8)), 1);
    QCOMPARE(BScheduleOccurrences::weekOfMonth(QDate(2026, 5, 14)), 1);
    QCOMPARE(BScheduleOccurrences::weekOfMonth(QDate(2026, 5, 21)), 2);
    QCOMPARE(BScheduleOccurrences::weekOfMonth(QDate(2026, 5, 28)), 3);
    QCOMPARE(BScheduleOccurrences::weekOfMonth(QDate(2026, 5, 29)), 4);
    QCOMPARE(BScheduleOccurrences::weekOfMonth(QDate(2026, 5, 31)), 4);

    // The last seven days: 25 to 31, 24 to 30, 22 to 28, 23 to 29
    QVERIFY(!BScheduleOccurrences::inLast7DaysOfMonth(QDate(2026, 5, 24)));
    QVERIFY(BScheduleOccurrences::inLast7DaysOfMonth(QDate(2026, 5, 25)));
    QVERIFY(BScheduleOccurrences::inLast7DaysOfMonth(QDate(2026, 5, 31)));
    QVERIFY(!BScheduleOccurrences::inLast7DaysOfMonth(QDate(2026, 6, 23)));
    QVERIFY(BScheduleOccurrences::inLast7DaysOfMonth(QDate(2026, 6, 24)));
    QVERIFY(!BScheduleOccurrences::inLast7DaysOfMonth(QDate(2026, 2, 21)));
    QVERIFY(BScheduleOccurrences::inLast7DaysOfMonth(QDate(2026, 2, 22)));
    QVERIFY(!BScheduleOccurrences::inLast7DaysOfMonth(QDate(2028, 2, 22)));   // leap year
    QVERIFY(BScheduleOccurrences::inLast7DaysOfMonth(QDate(2028, 2, 23)));
    QVERIFY(BScheduleOccurrences::inLast7DaysOfMonth(QDate(2028, 2, 29)));
}

void BScheduleOccurrencesTest::firstWeekdayOfAMonth()
{
    // Saturdays of 2026: May 2 9 16 23 30 (five), June 6 13 20 27 (four),
    // July 4 11 18 25, August 1 8 15 22 29 (five)
    QCOMPARE(dates("Full 1st sat at 21:00", QDate(2026, 5, 1), QDate(2026, 9, 1)),
             QStringList({"2026-05-02", "2026-06-06", "2026-07-04", "2026-08-01"}));

    // A month that starts on a Tuesday: the first Monday is the 7th
    QCOMPARE(dates("Full first mon at 21:00", QDate(2026, 9, 1), QDate(2026, 10, 1)),
             QStringList({"2026-09-07"}));

    // Rust port: October 2026, the 3rd and no other day
    const BRunSpec firstSat = specOf("Full 1st sat at 21:00");
    QVERIFY(BScheduleOccurrences::triggersOnDay(firstSat.mask, QDate(2026, 10, 3)));
    QVERIFY(!BScheduleOccurrences::triggersOnDay(firstSat.mask, QDate(2026, 10, 10)));
    QVERIFY(!BScheduleOccurrences::triggersOnDay(firstSat.mask, QDate(2026, 10, 17)));
    QVERIFY(!BScheduleOccurrences::triggersOnDay(firstSat.mask, QDate(2026, 10, 4)));
    QVERIFY(BScheduleOccurrences::triggersOn(firstSat, QDate(2026, 10, 3), 21));
    QVERIFY(!BScheduleOccurrences::triggersOn(firstSat, QDate(2026, 10, 3), 20));
}

void BScheduleOccurrencesTest::secondToFifthWeekdayOfAMonth()
{
    // Every Saturday but the first: four in May (five Saturdays), three in
    // June (four Saturdays)
    QCOMPARE(dates("Differential 2nd-5th sat at 21:00", QDate(2026, 5, 1), QDate(2026, 7, 1)),
             QStringList({"2026-05-09", "2026-05-16", "2026-05-23", "2026-05-30",
                          "2026-06-13", "2026-06-20", "2026-06-27"}));

    // "5th" is the 29th to the 31st: a fifth Saturday exists in May and
    // August 2026, not in June or July
    QCOMPARE(dates("Full 5th sat at 21:00", QDate(2026, 5, 1), QDate(2026, 9, 1)),
             QStringList({"2026-05-30", "2026-08-29"}));

    // Together the two standard lines are every Saturday, each exactly once
    const BSchedule both = scheduleOf({"Full 1st sat at 21:00", "Differential 2nd-5th sat at 21:00"});
    const QStringList all = starts(both, QDate(2026, 1, 1), QDate(2027, 1, 1),
                                   QTimeZone::utc(), "yyyy-MM-dd");
    QCOMPARE(all.size(), 52);   // 2026 has 52 Saturdays
    QCOMPARE(QSet<QString>(all.begin(), all.end()).size(), 52);

    // Rust port: October 2026
    const BRunSpec others = specOf("Differential 2nd-5th sat at 21:00");
    QVERIFY(!BScheduleOccurrences::triggersOnDay(others.mask, QDate(2026, 10, 3)));
    QVERIFY(BScheduleOccurrences::triggersOnDay(others.mask, QDate(2026, 10, 10)));
    QVERIFY(BScheduleOccurrences::triggersOnDay(others.mask, QDate(2026, 10, 17)));
    QVERIFY(BScheduleOccurrences::triggersOnDay(others.mask, QDate(2026, 10, 31)));
}

void BScheduleOccurrencesTest::lastWeekdayOfAMonth()
{
    // The Saturday in the last seven days: the fifth one in May and August,
    // the fourth one in June and July
    QCOMPARE(dates("Full last sat at 21:00", QDate(2026, 5, 1), QDate(2026, 9, 1)),
             QStringList({"2026-05-30", "2026-06-27", "2026-07-25", "2026-08-29"}));

    // February: Sundays 1 8 15 22 in 2026, 6 13 20 27 in the leap year 2028
    QCOMPARE(dates("Full last sun at 21:00", QDate(2026, 2, 1), QDate(2026, 3, 1)),
             QStringList({"2026-02-22"}));
    QCOMPARE(dates("Full last sun at 21:00", QDate(2028, 2, 1), QDate(2028, 3, 1)),
             QStringList({"2028-02-27"}));

    // "last" beside a week: either of them is enough
    QCOMPARE(dates("Full 1st last sat at 21:00", QDate(2026, 5, 1), QDate(2026, 7, 1)),
             QStringList({"2026-05-02", "2026-05-30", "2026-06-06", "2026-06-27"}));

    // Rust port: the last Friday of October 2026 is the 30th, not the 23rd
    const BRunSpec lastFri = specOf("Full last fri at 23:00");
    QVERIFY(BScheduleOccurrences::triggersOnDay(lastFri.mask, QDate(2026, 10, 30)));
    QVERIFY(!BScheduleOccurrences::triggersOnDay(lastFri.mask, QDate(2026, 10, 23)));
}

// --- Day of the month and month: hand calendar ---

void BScheduleOccurrencesTest::dayOfMonth()
{
    QCOMPARE(dates("Full on 1 at 2:05", QDate(2026, 1, 1), QDate(2026, 4, 1)),
             QStringList({"2026-01-01", "2026-02-01", "2026-03-01"}));

    // "on 2-31": every other day, also in a month with 28 days
    const QStringList rest = dates("Incremental on 2-31 at 2:05", QDate(2026, 1, 1), QDate(2026, 4, 1));
    QCOMPARE(rest.size(), 30 + 27 + 30);
    QCOMPARE(rest.first(), QString("2026-01-02"));
    QVERIFY(!rest.contains("2026-02-01"));
    QVERIFY(rest.contains("2026-02-02"));
    QVERIFY(rest.contains("2026-02-28"));
    QCOMPARE(rest.last(), QString("2026-03-31"));

    // The 31st exists in seven months (Rust port: from November the next is 31 December)
    QCOMPARE(dates("Full on 31 at 1:00", QDate(2026, 1, 1), QDate(2027, 1, 1)),
             QStringList({"2026-01-31", "2026-03-31", "2026-05-31", "2026-07-31",
                          "2026-08-31", "2026-10-31", "2026-12-31"}));
}

void BScheduleOccurrencesTest::monthRestriction()
{
    // First Saturdays of 2026: Jan 3, Feb 7, Mar 7, Apr 4, May 2, Jun 6,
    // Jul 4, Aug 1, Sep 5, Oct 3, Nov 7, Dec 5
    QCOMPARE(dates("Full jun-aug 1st sat at 21:00", QDate(2026, 1, 1), QDate(2027, 1, 1)),
             QStringList({"2026-06-06", "2026-07-04", "2026-08-01"}));
    QCOMPARE(dates("Full jan on 1 at 0:05", QDate(2026, 1, 1), QDate(2028, 1, 1)),
             QStringList({"2026-01-01", "2027-01-01"}));
}

void BScheduleOccurrencesTest::sundayIsBitZero()
{
    // 22 November 2026 is a Sunday
    const BRunSpec sunday = specOf("Full sun at 2:05");
    QCOMPARE(int(sunday.mask.wday), 1);
    QVERIFY(BScheduleOccurrences::triggersOn(sunday, QDate(2026, 11, 22), 2));
    QVERIFY(!BScheduleOccurrences::triggersOn(sunday, QDate(2026, 11, 22), 3));
    QVERIFY(!BScheduleOccurrences::triggersOn(sunday, QDate(2026, 11, 23), 2));   // Monday
    QVERIFY(!BScheduleOccurrences::triggersOn(sunday, QDate(2026, 11, 21), 2));   // Saturday
    QVERIFY(!BScheduleOccurrences::triggersOn(sunday, QDate(2026, 11, 22), 24));
    QVERIFY(!BScheduleOccurrences::triggersOn(sunday, QDate(2026, 11, 22), -1));
    QVERIFY(!BScheduleOccurrences::triggersOn(sunday, QDate(), 2));

    // Rust port: Monday to Friday
    const BRunSpec weekdays = specOf("Incremental mon-fri at 21:00");
    QVERIFY(BScheduleOccurrences::triggersOn(weekdays, QDate(2026, 10, 15), 21));    // Thursday
    QVERIFY(BScheduleOccurrences::triggersOn(weekdays, QDate(2026, 10, 16), 21));    // Friday
    QVERIFY(!BScheduleOccurrences::triggersOn(weekdays, QDate(2026, 10, 17), 21));   // Saturday
    QVERIFY(!BScheduleOccurrences::triggersOn(weekdays, QDate(2026, 10, 18), 21));   // Sunday
    QVERIFY(BScheduleOccurrences::triggersOn(weekdays, QDate(2026, 10, 19), 21));    // Monday
}

void BScheduleOccurrencesTest::everyRestrictionMustMatch()
{
    // A Friday the 13th in February: 13 February 2026 is one (second week
    // of the month); 13 March 2026 is a Friday too but not in February;
    // 13 February 2027 is a Saturday
    QCOMPARE(dates("Full feb 2nd fri on 13 at 4:00", QDate(2026, 1, 1), QDate(2028, 1, 1)),
             QStringList({"2026-02-13"}));

    // One part that never matches the others: the 13th is never in the first week
    QVERIFY(dates("Full 1st fri on 13 at 4:00", QDate(2026, 1, 1), QDate(2028, 1, 1)).isEmpty());

    // Week of the year and weekday: week 1 of 2027 starts on Monday 4 January (Rust port)
    QCOMPARE(dates("Full w01 mon at 3:00", QDate(2026, 12, 1), QDate(2027, 2, 1)),
             QStringList({"2027-01-04"}));
}

// --- Modulo: the examples of the Bareos documentation, hand calendar ---

void BScheduleOccurrencesTest::moduloDays()
{
    // "Odd Days": the 1st, 3rd, ..., 31st of each month; the 31st and the
    // 1st follow each other
    const QStringList odd = dates("1/2 at 23:10", QDate(2026, 1, 1), QDate(2026, 3, 1));
    QCOMPARE(odd.size(), 16 + 14);
    QCOMPARE(odd.mid(0, 3), QStringList({"2026-01-01", "2026-01-03", "2026-01-05"}));
    QCOMPARE(odd.mid(14, 4), QStringList({"2026-01-29", "2026-01-31", "2026-02-01", "2026-02-03"}));
    QCOMPARE(odd.last(), QString("2026-02-27"));

    // "Even Days": the 2nd, 4th, ..., 30th
    const QStringList even = dates("2/2 at 23:10", QDate(2026, 1, 1), QDate(2026, 3, 1));
    QCOMPARE(even.size(), 15 + 14);
    QCOMPARE(even.first(), QString("2026-01-02"));
    QCOMPARE(even.mid(14, 2), QStringList({"2026-01-30", "2026-02-02"}));
    QCOMPARE(even.last(), QString("2026-02-28"));

    // Between them every day, none twice
    QSet<QString> all(odd.begin(), odd.end());
    for (const QString &day : even)
        QVERIFY(!all.contains(day));
    QCOMPARE(odd.size() + even.size(), 31 + 28);
}

void BScheduleOccurrencesTest::moduloWeeks()
{
    // "Odd Weeks". 2026: 1 January is a Thursday, so week 1 is 29 December
    // 2025 to 4 January, week 2 is 5 to 11 January, week 3 is 12 to 18.
    QCOMPARE(dates("w01/w02 at 23:10", QDate(2026, 1, 1), QDate(2026, 1, 26)),
             QStringList({"2026-01-01", "2026-01-02", "2026-01-03", "2026-01-04",
                          "2026-01-12", "2026-01-13", "2026-01-14", "2026-01-15",
                          "2026-01-16", "2026-01-17", "2026-01-18"}));

    // "Even Weeks" are the days in between
    QCOMPARE(dates("w02/w02 at 23:10", QDate(2026, 1, 1), QDate(2026, 1, 26)),
             QStringList({"2026-01-05", "2026-01-06", "2026-01-07", "2026-01-08",
                          "2026-01-09", "2026-01-10", "2026-01-11", "2026-01-19",
                          "2026-01-20", "2026-01-21", "2026-01-22", "2026-01-23",
                          "2026-01-24", "2026-01-25"}));

    // At the end of 2026: week 52 is 21 to 27 December, week 53 is 28 to
    // 31 December, 1 to 3 January 2027 are week 0, week 1 is 4 to 10
    // January. Two odd weeks follow each other, with week 0 in neither.
    QCOMPARE(dates("w01/w02 at 23:10", QDate(2026, 12, 21), QDate(2027, 1, 12)),
             QStringList({"2026-12-28", "2026-12-29", "2026-12-30", "2026-12-31",
                          "2027-01-04", "2027-01-05", "2027-01-06", "2027-01-07",
                          "2027-01-08", "2027-01-09", "2027-01-10"}));
    QCOMPARE(dates("w02/w02 at 23:10", QDate(2026, 12, 21), QDate(2027, 1, 12)),
             QStringList({"2026-12-21", "2026-12-22", "2026-12-23", "2026-12-24",
                          "2026-12-25", "2026-12-26", "2026-12-27", "2027-01-11"}));
}

// --- Week of the year: TmWoy() applied by hand; two values from the Rust port ---

void BScheduleOccurrencesTest::weekOfYear_data()
{
    QTest::addColumn<QDate>("date");
    QTest::addColumn<int>("week");

    // Week 1 is the week (Monday to Sunday) with 4 January in it
    QTest::newRow("2026-01-01 Thu, week 1 began in December") << QDate(2026, 1, 1) << 1;
    QTest::newRow("2026-01-04 Sun") << QDate(2026, 1, 4) << 1;
    QTest::newRow("2026-01-05 Mon") << QDate(2026, 1, 5) << 2;
    QTest::newRow("2026-11-19 Thu") << QDate(2026, 11, 19) << 47;
    QTest::newRow("2026-12-27 Sun") << QDate(2026, 12, 27) << 52;
    QTest::newRow("2026-12-28 Mon") << QDate(2026, 12, 28) << 53;
    QTest::newRow("2026-12-31 Thu") << QDate(2026, 12, 31) << 53;
    // Rust port: the days before week 1 are week 0, not week 53 of 2026
    QTest::newRow("2027-01-01 Fri (Rust port)") << QDate(2027, 1, 1) << 0;
    QTest::newRow("2027-01-03 Sun") << QDate(2027, 1, 3) << 0;
    QTest::newRow("2027-01-04 Mon (Rust port)") << QDate(2027, 1, 4) << 1;
    // Not ISO: 30 and 31 December 2024 are in ISO week 1 of 2025; Bareos
    // keeps counting in 2024
    QTest::newRow("2024-12-29 Sun") << QDate(2024, 12, 29) << 52;
    QTest::newRow("2024-12-30 Mon") << QDate(2024, 12, 30) << 53;
    QTest::newRow("2024-12-31 Tue") << QDate(2024, 12, 31) << 53;
    QTest::newRow("2025-01-01 Wed") << QDate(2025, 1, 1) << 1;
    QTest::newRow("2025-01-05 Sun") << QDate(2025, 1, 5) << 1;
    QTest::newRow("2025-01-06 Mon") << QDate(2025, 1, 6) << 2;
    // A year that begins on a Monday has no week 0
    QTest::newRow("2024-01-01 Mon") << QDate(2024, 1, 1) << 1;
    QTest::newRow("2024-01-07 Sun") << QDate(2024, 1, 7) << 1;
    QTest::newRow("2024-01-08 Mon") << QDate(2024, 1, 8) << 2;
    // 2020 (leap, 1 January a Wednesday) ends in week 53; 2021 begins with week 0
    QTest::newRow("2020-12-31 Thu") << QDate(2020, 12, 31) << 53;
    QTest::newRow("2021-01-01 Fri") << QDate(2021, 1, 1) << 0;
    QTest::newRow("2021-01-03 Sun") << QDate(2021, 1, 3) << 0;
    QTest::newRow("2021-01-04 Mon") << QDate(2021, 1, 4) << 1;
}

void BScheduleOccurrencesTest::weekOfYear()
{
    QFETCH(QDate, date);
    QFETCH(int, week);
    QCOMPARE(BScheduleOccurrences::weekOfYear(date), week);
}

void BScheduleOccurrencesTest::week00AndWeek53AtAYearBoundary()
{
    // 2026 into 2027: week 53 is Monday 28 to Thursday 31 December, week 0
    // is Friday 1 to Sunday 3 January
    QCOMPARE(dates("Full w53 at 3:00", QDate(2026, 12, 1), QDate(2027, 2, 1)),
             QStringList({"2026-12-28", "2026-12-29", "2026-12-30", "2026-12-31"}));
    QCOMPARE(dates("Full w00 at 3:00", QDate(2026, 12, 1), QDate(2027, 2, 1)),
             QStringList({"2027-01-01", "2027-01-02", "2027-01-03"}));

    // 2025 begins on a Wednesday in week 1: no week 0 that year, and the
    // two days before it are week 53 of 2024
    QVERIFY(dates("Full w00 at 3:00", QDate(2024, 12, 1), QDate(2025, 12, 31)).isEmpty());
    QCOMPARE(dates("Full w53 at 3:00", QDate(2024, 12, 1), QDate(2025, 12, 1)),
             QStringList({"2024-12-30", "2024-12-31"}));

    // 2025 itself has no week 53: 28 December 2025 is a Sunday in week 52,
    // and week 53 is 29 to 31 December
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2025, 12, 28)), 52);
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2025, 12, 29)), 53);
}

void BScheduleOccurrencesTest::weekOfYearWhereJanuaryIsSummer()
{
    // TmWoy() asks for "4 January, same time of day, not daylight saving".
    // In Sydney 4 January is in summer time, glibc answers with the moment
    // an hour later, and from 23:00 on that is 5 January. In the last hour
    // of the day the weeks are therefore counted from the weekday of
    // 5 January. 2026: 4 January is a Sunday, 5 January a Monday, so at
    // 23:xx week 1 is 4 to 10 January (not 29 December to 4 January) and
    // 1 to 3 January are week 0.
    // Expected values: TmWoy() run on glibc with TZ=Australia/Sydney.
    const QTimeZone sydney("Australia/Sydney");
    const QTimeZone berlin("Europe/Berlin");
    QVERIFY2(sydney.isValid() && berlin.isValid(), "time zones not available");

    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 1, 1), 22, sydney), 1);
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 1, 1), 23, sydney), 0);
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 1, 3), 23, sydney), 0);
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 1, 4), 23, sydney), 1);
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 1, 5), 22, sydney), 2);
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 1, 5), 23, sydney), 1);
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 3, 2), 22, sydney), 10);
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 3, 2), 23, sydney), 9);
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 3, 8), 23, sydney), 10);
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 3, 14), 23, sydney), 10);
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 3, 15), 23, sydney), 11);
    // 2027: 4 January is a Monday, 5 January a Tuesday: no difference that year
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2027, 1, 5), 23, sydney), 1);

    // Where 4 January is in winter time the hour makes no difference
    for (int hour : {0, 22, 23}) {
        QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 1, 1), hour, berlin), 1);
        QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 3, 2), hour, berlin), 10);
        QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 3, 2), hour, QTimeZone::utc()), 10);
    }
    // The calendar's week, without a zone
    QCOMPARE(BScheduleOccurrences::weekOfYear(QDate(2026, 3, 2)), 10);

    // The runs follow: week 10 of 2026 is 2 to 8 March, but at 23:00 in
    // Sydney the Director runs it from 8 to 14 March
    const QStringList calendarWeek = {"2026-03-02", "2026-03-03", "2026-03-04", "2026-03-05",
                                      "2026-03-06", "2026-03-07", "2026-03-08"};
    QCOMPARE(dates("Full w10 at 22:00", QDate(2026, 2, 1), QDate(2026, 4, 1), sydney), calendarWeek);
    QCOMPARE(dates("Full w10 at 23:00", QDate(2026, 2, 1), QDate(2026, 4, 1), berlin), calendarWeek);
    QCOMPARE(dates("Full w10 at 23:00", QDate(2026, 2, 1), QDate(2026, 4, 1), sydney),
             QStringList({"2026-03-08", "2026-03-09", "2026-03-10", "2026-03-11",
                          "2026-03-12", "2026-03-13", "2026-03-14"}));

    // The next run is found although the calendar says week 11
    QCOMPARE(BScheduleOccurrences::next(specOf("Full w10 at 23:00"), at(QDate(2026, 3, 9), sydney)),
             QDateTime(QDate(2026, 3, 9), QTime(23, 0), sydney));
    QCOMPARE(BScheduleOccurrences::next(specOf("Full w10 at 23:00"), at(QDate(2026, 3, 9), berlin)),
             QDateTime(QDate(2027, 3, 8), QTime(23, 0), berlin));
}

// --- Hours ---

void BScheduleOccurrencesTest::hourlyLine()
{
    const BSchedule hourly = scheduleOf({"Full hourly at 0:15"});
    const QStringList day = starts(hourly, QDate(2026, 11, 19), QDate(2026, 11, 20));
    QCOMPARE(day.size(), 24);
    QCOMPARE(day.first(), QString("2026-11-19 00:15"));
    QCOMPARE(day.at(1), QString("2026-11-19 01:15"));
    QCOMPARE(day.last(), QString("2026-11-19 23:15"));

    // A line without a time runs at the start of every hour of its days
    const BSchedule noTime = scheduleOf({"Full sun"});
    const QStringList week = starts(noTime, QDate(2026, 11, 16), QDate(2026, 11, 23));
    QCOMPARE(week.size(), 24);
    QCOMPARE(week.first(), QString("2026-11-22 00:00"));
    QCOMPARE(week.last(), QString("2026-11-22 23:00"));
}

void BScheduleOccurrencesTest::lineWithSeveralHours()
{
    const BSchedule twice = scheduleOf({"Incremental at 1:30 at 13:30"});
    QCOMPARE(starts(twice, QDate(2026, 11, 19), QDate(2026, 11, 21)),
             QStringList({"2026-11-19 01:30", "2026-11-19 13:30",
                          "2026-11-20 01:30", "2026-11-20 13:30"}));
}

void BScheduleOccurrencesTest::rangeIsHalfOpenAndSorted()
{
    const BSchedule schedule = scheduleOf({"Incremental mon-fri at 21:00", "Full at 6:45"});
    const QTimeZone utc = QTimeZone::utc();

    // Thursday 19 November 2026 from 06:45 to 21:00: the start at the
    // beginning is in, the one at the end is not; a start earlier in the
    // first hour is not
    const QDate day(2026, 11, 19);
    QList<BOccurrence> found = BScheduleOccurrences::occurrences(
        schedule, QDateTime(day, QTime(6, 45), utc), QDateTime(day, QTime(21, 0), utc));
    QCOMPARE(found.size(), 1);
    QCOMPARE(found[0].run, 1);
    QCOMPARE(found[0].when, QDateTime(day, QTime(6, 45), utc));

    found = BScheduleOccurrences::occurrences(
        schedule, QDateTime(day, QTime(6, 46), utc), QDateTime(day, QTime(21, 1), utc));
    QCOMPARE(found.size(), 1);
    QCOMPARE(found[0].run, 0);
    QCOMPARE(found[0].when, QDateTime(day, QTime(21, 0), utc));

    // By time, then by line
    found = BScheduleOccurrences::occurrences(schedule, at(day), at(day.addDays(2)));
    QCOMPARE(found.size(), 4);
    QCOMPARE(found[0].run, 1);
    QCOMPARE(found[1].run, 0);
    QCOMPARE(found[2].run, 1);
    QCOMPARE(found[3].run, 0);
    QVERIFY(found[0].when < found[1].when && found[1].when < found[2].when);

    // Nothing for an empty or an invalid range
    QVERIFY(BScheduleOccurrences::occurrences(schedule, at(day), at(day)).isEmpty());
    QVERIFY(BScheduleOccurrences::occurrences(schedule, at(day.addDays(1)), at(day)).isEmpty());
    QVERIFY(BScheduleOccurrences::occurrences(schedule, QDateTime(), at(day)).isEmpty());
}

// --- Daylight saving: Europe/Berlin 2026, as the Rust port tests it ---

void BScheduleOccurrencesTest::daylightSavingSpring()
{
    const QTimeZone berlin("Europe/Berlin");
    QVERIFY2(berlin.isValid(), "time zone Europe/Berlin not available");

    // 29 March 2026: 02:00 becomes 03:00. A run at 02:30 does not exist on
    // that day and does not run (Rust port: "never sees hour 2").
    const BSchedule at230 = scheduleOf({"Full daily at 2:30"});
    QCOMPARE(starts(at230, QDate(2026, 3, 28), QDate(2026, 3, 31), berlin),
             QStringList({"2026-03-28 02:30", "2026-03-30 02:30"}));

    // The day has 23 hours
    const BSchedule hourly = scheduleOf({"Full hourly at 0:15"});
    const QStringList day = starts(hourly, QDate(2026, 3, 29), QDate(2026, 3, 30), berlin);
    QCOMPARE(day.size(), 23);
    QCOMPARE(day.mid(0, 4), QStringList({"2026-03-29 00:15", "2026-03-29 01:15",
                                         "2026-03-29 03:15", "2026-03-29 04:15"}));

    // Rust port: the daily 21:00 stays 21:00 local time, 23 hours later
    const BSchedule daily = scheduleOf({"Incremental daily at 21:00"});
    const QList<BOccurrence> found = BScheduleOccurrences::occurrences(
        daily, at(QDate(2026, 3, 28), berlin), at(QDate(2026, 3, 30), berlin));
    QCOMPARE(found.size(), 2);
    QCOMPARE(found[0].when.toString("yyyy-MM-dd HH:mm"), QString("2026-03-28 21:00"));
    QCOMPARE(found[1].when.toString("yyyy-MM-dd HH:mm"), QString("2026-03-29 21:00"));
    QCOMPARE(found[0].when.secsTo(found[1].when), qint64(23 * 3600));
    QCOMPARE(found[0].when.offsetFromUtc(), 3600);
    QCOMPARE(found[1].when.offsetFromUtc(), 7200);
}

void BScheduleOccurrencesTest::daylightSavingAutumn()
{
    const QTimeZone berlin("Europe/Berlin");
    QVERIFY2(berlin.isValid(), "time zone Europe/Berlin not available");

    // 25 October 2026: 03:00 becomes 02:00. 02:30 exists twice, and the
    // run runs twice, one hour apart (Rust port: "sees hour 2 twice").
    const BSchedule at230 = scheduleOf({"Full daily at 2:30"});
    const QList<BOccurrence> found = BScheduleOccurrences::occurrences(
        at230, at(QDate(2026, 10, 24), berlin), at(QDate(2026, 10, 27), berlin));
    QCOMPARE(found.size(), 4);
    QCOMPARE(found[0].when.toString("yyyy-MM-dd HH:mm"), QString("2026-10-24 02:30"));
    QCOMPARE(found[1].when.toString("yyyy-MM-dd HH:mm"), QString("2026-10-25 02:30"));
    QCOMPARE(found[2].when.toString("yyyy-MM-dd HH:mm"), QString("2026-10-25 02:30"));
    QCOMPARE(found[3].when.toString("yyyy-MM-dd HH:mm"), QString("2026-10-26 02:30"));
    QCOMPARE(found[1].when.offsetFromUtc(), 7200);   // summer time first
    QCOMPARE(found[2].when.offsetFromUtc(), 3600);
    QCOMPARE(found[1].when.secsTo(found[2].when), qint64(3600));
    QCOMPARE(found[1].when.toUTC().time(), QTime(0, 30));
    QCOMPARE(found[2].when.toUTC().time(), QTime(1, 30));

    // The day has 25 hours
    const BSchedule hourly = scheduleOf({"Full hourly at 0:15"});
    QCOMPARE(starts(hourly, QDate(2026, 10, 25), QDate(2026, 10, 26), berlin).size(), 25);

    // Rust port: the daily 21:00 comes 25 hours after the one before
    const BSchedule daily = scheduleOf({"Incremental daily at 21:00"});
    const QList<BOccurrence> evenings = BScheduleOccurrences::occurrences(
        daily, at(QDate(2026, 10, 24), berlin), at(QDate(2026, 10, 26), berlin));
    QCOMPARE(evenings.size(), 2);
    QCOMPARE(evenings[1].when.toString("yyyy-MM-dd HH:mm"), QString("2026-10-25 21:00"));
    QCOMPARE(evenings[0].when.secsTo(evenings[1].when), qint64(25 * 3600));

    // A zone without daylight saving has neither: one run a day
    QCOMPARE(starts(at230, QDate(2026, 10, 24), QDate(2026, 10, 27)).size(), 3);
}

// --- The next run: values of the Rust port's next_schedule_time tests ---

void BScheduleOccurrencesTest::nextRun()
{
    const QTimeZone utc = QTimeZone::utc();

    // From 31 October the next "last fri" is 27 November
    QCOMPARE(BScheduleOccurrences::next(specOf("Full last fri at 23:00"), at(QDate(2026, 10, 31))),
             QDateTime(QDate(2026, 11, 27), QTime(23, 0), utc));
    // Over the end of the year
    QCOMPARE(BScheduleOccurrences::next(specOf("Full 1 at 0:05"),
                                        QDateTime(QDate(2026, 12, 31), QTime(23, 0), utc)),
             QDateTime(QDate(2027, 1, 1), QTime(0, 5), utc));
    QCOMPARE(BScheduleOccurrences::next(specOf("Full on 31 at 1:00"), at(QDate(2026, 11, 1))),
             QDateTime(QDate(2026, 12, 31), QTime(1, 0), utc));

    // On the day itself: a start that is still to come counts, one that has passed does not
    const BRunSpec evening = specOf("Incremental mon-fri at 21:00");
    QCOMPARE(BScheduleOccurrences::next(evening, QDateTime(QDate(2026, 10, 15), QTime(20, 47), utc)),
             QDateTime(QDate(2026, 10, 15), QTime(21, 0), utc));
    QCOMPARE(BScheduleOccurrences::next(evening, QDateTime(QDate(2026, 10, 15), QTime(21, 0), utc)),
             QDateTime(QDate(2026, 10, 15), QTime(21, 0), utc));
    // Friday after 21:00: Saturday and Sunday are left out
    QCOMPARE(BScheduleOccurrences::next(evening, QDateTime(QDate(2026, 10, 16), QTime(22, 0), utc)),
             QDateTime(QDate(2026, 10, 19), QTime(21, 0), utc));
    // Not within the days asked for
    QVERIFY(!BScheduleOccurrences::next(evening, QDateTime(QDate(2026, 10, 16), QTime(22, 0), utc), 1)
                 .isValid());

    // 30 February: never
    QVERIFY(!BScheduleOccurrences::next(specOf("Full feb on 30 at 1:00"), at(QDate(2026, 1, 1)))
                 .isValid());
    // 29 February: in the next leap year
    QCOMPARE(BScheduleOccurrences::next(specOf("Full feb on 29 at 1:00"), at(QDate(2026, 1, 1))),
             QDateTime(QDate(2028, 2, 29), QTime(1, 0), utc));
}

// --- Collisions on concrete dates ---

void BScheduleOccurrencesTest::firstAndOtherSaturdaysDoNotCollide()
{
    // The standard pair: by weekday and time alone both run "Saturday at
    // 21:00". They never start on the same date.
    const BSchedule cycle = scheduleOf({"Full 1st sat at 21:00", "Differential 2nd-5th sat at 21:00"});
    const QList<QDateTime> full = startsOfRun(cycle, 0, QDate(2026, 1, 1), QDate(2027, 1, 1));
    const QList<QDateTime> diff = startsOfRun(cycle, 1, QDate(2026, 1, 1), QDate(2027, 1, 1));
    QCOMPARE(full.size(), 12);
    QCOMPARE(diff.size(), 40);
    QVERIFY(BScheduleOccurrences::overlaps(full, 3600, diff, 3600).isEmpty());
    // Not even if each ran for six days
    QVERIFY(BScheduleOccurrences::overlaps(full, 6 * 86400, diff, 6 * 86400).isEmpty());
    // Only a run of more than a week reaches the next Saturday
    QVERIFY(!BScheduleOccurrences::overlaps(full, 7 * 86400 + 1, diff, 3600).isEmpty());
}

void BScheduleOccurrencesTest::realCollision()
{
    const QTimeZone utc = QTimeZone::utc();

    // A full backup on the first Saturday and an incremental one every
    // Saturday half an hour later: they meet on the first Saturday of each
    // month and on no other
    const BSchedule both = scheduleOf({"Full 1st sat at 21:00", "Incremental sat at 21:30"});
    const QList<QDateTime> full = startsOfRun(both, 0, QDate(2026, 5, 1), QDate(2026, 7, 1));
    const QList<QDateTime> incr = startsOfRun(both, 1, QDate(2026, 5, 1), QDate(2026, 7, 1));
    QCOMPARE(incr.size(), 9);

    const QList<BRunOverlap> hour = BScheduleOccurrences::overlaps(full, 3600, incr, 900);
    QCOMPARE(hour.size(), 2);
    QCOMPARE(hour[0].a, QDateTime(QDate(2026, 5, 2), QTime(21, 0), utc));
    QCOMPARE(hour[0].b, QDateTime(QDate(2026, 5, 2), QTime(21, 30), utc));
    QCOMPARE(hour[0].start(), QDateTime(QDate(2026, 5, 2), QTime(21, 30), utc));
    QCOMPARE(hour[1].start(), QDateTime(QDate(2026, 6, 6), QTime(21, 30), utc));

    // The same from the other side
    const QList<BRunOverlap> reversed = BScheduleOccurrences::overlaps(incr, 900, full, 3600);
    QCOMPARE(reversed.size(), 2);
    QCOMPARE(reversed[0].a, hour[0].b);
    QCOMPARE(reversed[0].start(), hour[0].start());

    // The duration decides: half an hour ends when the other one starts,
    // and touching is no collision
    QVERIFY(BScheduleOccurrences::overlaps(full, 1800, incr, 900).isEmpty());
    QCOMPARE(BScheduleOccurrences::overlaps(full, 1801, incr, 900).size(), 2);

    // Over midnight: Friday 23:30 for an hour and Saturday 00:15
    const BSchedule night = scheduleOf({"Full fri at 23:30", "Incremental sat at 0:15"});
    const QList<BRunOverlap> late = BScheduleOccurrences::overlaps(
        startsOfRun(night, 0, QDate(2026, 11, 16), QDate(2026, 11, 23)), 3600,
        startsOfRun(night, 1, QDate(2026, 11, 16), QDate(2026, 11, 23)), 900);
    QCOMPARE(late.size(), 1);
    QCOMPARE(late[0].start(), QDateTime(QDate(2026, 11, 21), QTime(0, 15), utc));

    // Nothing to compare, nothing found
    QVERIFY(BScheduleOccurrences::overlaps({}, 3600, incr, 900).isEmpty());
}

// --- Lines that are not understood ---

void BScheduleOccurrencesTest::lineNotUnderstoodHasNoOccurrence()
{
    // Overrides behind the time, an unknown level, an empty line
    const BSchedule bad = scheduleOf({"Full 1st sat at 21:00 pool=Monthly", "Bogus sat at 21:00", ""});
    for (const BRunLine &line : bad.runs)
        QVERIFY(!line.understood());
    QVERIFY(BScheduleOccurrences::occurrences(bad, at(QDate(2026, 1, 1)), at(QDate(2027, 1, 1))).isEmpty());

    // The understood line beside it keeps its position
    const BSchedule mixed = scheduleOf({"Full 1st sat at 21:00 pool=Monthly", "Incremental sat at 21:30"});
    const QList<BOccurrence> found = BScheduleOccurrences::occurrences(
        mixed, at(QDate(2026, 5, 1)), at(QDate(2026, 6, 1)));
    QCOMPARE(found.size(), 5);
    for (const BOccurrence &o : found)
        QCOMPARE(o.run, 1);

    // And gives no entry to a view
    const QList<BScheduleEntry> entries = BScheduleEntry::fromOccurrences(mixed, found);
    QCOMPARE(entries.size(), 1);
    QCOMPARE(entries[0].runIndex, 1);
    QCOMPARE(entries[0].source.original, QString("Incremental sat at 21:30"));
    QVERIFY(BScheduleEntry::fromOccurrences(bad, {}).isEmpty());
}

// --- What the views get: one entry per job and run that starts in the dates ---

void BScheduleOccurrencesTest::entriesOncePerJobAndRun()
{
    const QTimeZone utc = QTimeZone::utc();
    const QList<BSchedule> schedules = {
        scheduleOf({"Full 1st sat at 21:00",
                    "Differential 2nd-5th sat at 21:00",
                    "Incremental Storage=Tape mon-fri at 21:00 at 23:00"}, "Cycle"),
        scheduleOf({"Full on 1 at 3:00"}, "Unused"),
    };

    // Two jobs use "Cycle", none uses "Unused"
    BJobConfigModel jobs;
    jobs.parseShowJobs(R"({"result":{"jobs":{
        "backup-a":{"name":"backup-a","schedule":"Cycle","client":"a-fd","storage":"File","pool":"Daily"},
        "backup-b":{"name":"backup-b","schedule":{"name":"Cycle"},"client":"b-fd","storage":"File","priority":12},
        "manual":{"name":"manual","client":"a-fd","storage":"File"}
    }}})");
    BJobScheduleIndex index;
    index.rebuild(&jobs);
    QVERIFY(index.isValid());

    auto rows = [](const QList<BScheduleEntry> &entries) {
        QStringList result;
        for (const BScheduleEntry &e : entries) {
            result.append(QString("%1 %2 line %3 %4:%5 x%6")
                .arg(e.jobName.isEmpty() ? QString("-") : e.jobName, e.levelLabel())
                .arg(e.runIndex).arg(e.hour).arg(e.minute, 2, 10, QChar('0'))
                .arg(e.occurrences.size()));
        }
        result.sort();
        return result;
    };

    // Saturday 7 November 2026, the first of the month: the Full once per
    // job. Not the Differential, although it also says "sat at 21:00".
    QList<BScheduleEntry> entries = index.entriesFor(
        schedules, at(QDate(2026, 11, 7)), at(QDate(2026, 11, 8)));
    QCOMPARE(rows(entries), QStringList({"backup-a Full line 0 21:00 x1",
                                         "backup-b Full line 0 21:00 x1"}));
    for (const BScheduleEntry &e : entries) {
        QCOMPARE(e.scheduleName, QString("Cycle"));
        QCOMPARE(e.occurrences.first(), QDateTime(QDate(2026, 11, 7), QTime(21, 0), utc));
        QCOMPARE(e.source.original, QString("Full 1st sat at 21:00"));
        QCOMPARE(e.storage, QString("File"));
        QCOMPARE(e.client, e.jobName == "backup-a" ? QString("a-fd") : QString("b-fd"));
        QCOMPARE(e.pool, e.jobName == "backup-a" ? QString("Daily") : QString());
        QCOMPARE(e.priority, e.jobName == "backup-a" ? 10 : 12);
    }

    // The Saturday after: the Differential, once per job
    entries = index.entriesFor(schedules, at(QDate(2026, 11, 14)), at(QDate(2026, 11, 15)));
    QCOMPARE(rows(entries), QStringList({"backup-a Differential line 1 21:00 x1",
                                         "backup-b Differential line 1 21:00 x1"}));

    // The week from Monday 2 November: the Full on its one date, the
    // Incremental at each of its two hours with five starts, each once per
    // job; nothing of the Differential
    entries = index.entriesFor(schedules, at(QDate(2026, 11, 2)), at(QDate(2026, 11, 9)));
    QCOMPARE(rows(entries), QStringList({"backup-a Full line 0 21:00 x1",
                                         "backup-a Incremental line 2 21:00 x5",
                                         "backup-a Incremental line 2 23:00 x5",
                                         "backup-b Full line 0 21:00 x1",
                                         "backup-b Incremental line 2 21:00 x5",
                                         "backup-b Incremental line 2 23:00 x5"}));
    // What the Run line overrides wins over the job
    for (const BScheduleEntry &e : entries) {
        if (e.runIndex == 2)
            QCOMPARE(e.storage, QString("Tape"));
    }

    // Sunday 1 November: only the schedule no job uses, once and without a job
    entries = index.entriesFor(schedules, at(QDate(2026, 11, 1)), at(QDate(2026, 11, 2)));
    QCOMPARE(rows(entries), QStringList({"- Full line 0 3:00 x1"}));
    QCOMPARE(entries[0].scheduleName, QString("Unused"));

    // A dragged bar finds its line: the entry's own line at its position
    entries = index.entriesFor(schedules, at(QDate(2026, 11, 14)), at(QDate(2026, 11, 15)));
    QCOMPARE(schedules[0].runs[entries[0].runIndex].original, entries[0].source.original);
    QVERIFY(entries[0].occursOn(QDate(2026, 11, 14)));
    QVERIFY(!entries[0].occursOn(QDate(2026, 11, 7)));
    QVERIFY(entries[0].sharesDateWith(entries[1]));

    // Before the jobs are known: every run once, without a job
    BJobScheduleIndex empty;
    entries = empty.entriesFor(schedules, at(QDate(2026, 11, 7)), at(QDate(2026, 11, 8)));
    QCOMPARE(rows(entries), QStringList({"- Full line 0 21:00 x1"}));
}

QTEST_APPLESS_MAIN(BScheduleOccurrencesTest)
#include "bscheduleoccurrences_test.moc"
