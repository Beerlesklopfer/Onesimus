/**
 * @file bschedulegridtext_test.cpp
 * @brief What the schedule grid says in words, its calendar and its geometry
 *
 * Four groups:
 *  - the spellings the Run writer lends to the scope bar (months, days of
 *    the month, weeks of the year);
 *  - labels, badges and phrases of scopes, level letters, brush tags, the
 *    names of cells for a screen reader and the plain-language reading of
 *    calendar rules;
 *  - the mini calendar: the levels per date against BScheduleOccurrences,
 *    for the usual schedule with three scopes and for a calendar rule, and
 *    its navigation by month;
 *  - the geometry of the grid: from positions to cells and headers and
 *    back, with scrolling and with the band of calendar rules.
 *
 * Rows are those of a view that starts the week on Monday. Runs without a
 * window and without a Director (registered with CTest).
 */

#include <QtTest>

#include "schedules/bschedulegridgeometry.h"
#include "schedules/model/bschedulegridmodel.h"
#include "schedules/model/bschedulegridtext.h"
#include "schedules/model/bscheduleoccurrences.h"

namespace {

enum Row { Mon = 0, Tue, Wed, Thu, Fri, Sat, Sun };

BSchedule scheduleOf(const QStringList &runs)
{
    BSchedule schedule;
    schedule.name = QStringLiteral("S");
    for (const QString &run : runs)
        schedule.runs.append(BRunParser::parse(run));
    return schedule;
}

/// Full on the first Saturday, Differential on the others, Incremental on
/// working days; the Full goes to its own pool
BSchedule typicalSchedule()
{
    return scheduleOf({
        QStringLiteral("Level=Full Pool=Monthly 1st sat at 21:00"),
        QStringLiteral("Level=Differential 2nd-5th sat at 21:00"),
        QStringLiteral("Level=Incremental mon-fri at 21:05"),
    });
}

BGridScope scopeOf(quint8 weeks, bool last = false, quint16 months = BDateTimeMask::AllMonths)
{
    BGridScope scope;
    scope.weeksOfMonth = weeks;
    scope.last = last;
    scope.months = months;
    return scope;
}

BDateTimeMask maskOf(const QString &run)
{
    const BRunLine line = BRunParser::parse(run);
    return line.spec ? line.spec->mask : BDateTimeMask();
}

QStringList levelNames(const BScheduleCalendarDay &day)
{
    QStringList names;
    for (const std::optional<QString> &level : day.levels())
        names.append(level ? *level : QStringLiteral("(job)"));
    return names;
}

} // namespace

class BScheduleGridTextTest : public QObject
{
    Q_OBJECT

private slots:
    // --- Spellings of the Run writer ---
    void writerHelpers_data();
    void writerHelpers();
    void writerStillWritesTheSame();

    // --- Scopes ---
    void scopeLabels_data();
    void scopeLabels();
    void scopeLabelsOfAModel();

    // --- Levels, brushes, cells ---
    void levelLetters();
    void brushTexts();
    void brushTags();
    void cellNames();

    // --- Calendar rules ---
    void ruleReadings_data();
    void ruleReadings();

    // --- Mini calendar ---
    void calendarOfTheTypicalSchedule();
    void calendarAgreesWithOccurrences_data();
    void calendarAgreesWithOccurrences();
    void calendarOfACalendarRule();
    void calendarNavigation();

    // --- Geometry ---
    void geometryCellsThereAndBack();
    void geometryHeadersAndCorner();
    void geometryScrolled();
    void geometryRuleBand();
    void geometryNearestCell();
};

// --- Spellings of the Run writer ------------------------------------------------

void BScheduleGridTextTest::writerHelpers_data()
{
    QTest::addColumn<QString>("run");
    QTest::addColumn<QString>("months");
    QTest::addColumn<QString>("days");
    QTest::addColumn<QString>("weeks");

    QTest::newRow("nothing") << "Full sat at 21:00" << "" << "" << "";
    QTest::newRow("month range") << "Full jan-mar 1st sat at 21:00" << "jan-mar" << "" << "";
    QTest::newRow("month list") << "Full jan,jul sun at 4:00" << "jan,jul" << "" << "";
    QTest::newRow("one day") << "Full on 1 at 21:00" << "" << "1" << "";
    QTest::newRow("day range") << "Full on 2-31 at 2:05" << "" << "2-31" << "";
    QTest::newRow("day modulo") << "Full 1/2 at 23:10" << "" << "1/2" << "";
    QTest::newRow("week") << "Full w03 mon at 1:00" << "" << "" << "w03";
    QTest::newRow("week modulo") << "Full w01/w02 sun at 23:10" << "" << "" << "w01/w02";
}

void BScheduleGridTextTest::writerHelpers()
{
    QFETCH(QString, run);
    QFETCH(QString, months);
    QFETCH(QString, days);
    QFETCH(QString, weeks);

    const BRunLine line = BRunParser::parse(run);
    QVERIFY2(line.spec.has_value(), qPrintable(line.parseError));
    QCOMPARE(BRunWriter::monthText(line.spec->mask), months);
    QCOMPARE(BRunWriter::dayOfMonthText(line.spec->mask), days);
    QCOMPARE(BRunWriter::weekOfYearText(line.spec->mask), weeks);
}

void BScheduleGridTextTest::writerStillWritesTheSame()
{
    // write() is put together from the helpers: the canonical texts stay
    const QStringList canonical = {
        QStringLiteral("Level=Full Pool=Monthly jan-mar 1st sat at 21:00"),
        QStringLiteral("Level=Full w01/w02 sun at 23:10"),
        QStringLiteral("Level=Full on 1/2 at 23:10"),
        QStringLiteral("Level=Full jan,jul on 1,15 at 04:00"),
        QStringLiteral("Level=Incremental hourly at 0:05"),
    };
    for (const QString &text : canonical) {
        const BRunLine line = BRunParser::parse(text);
        QVERIFY2(line.spec.has_value(), qPrintable(text));
        QCOMPARE(BRunWriter::write(*line.spec), text);
    }
}

// --- Scopes ---------------------------------------------------------------------

void BScheduleGridTextTest::scopeLabels_data()
{
    QTest::addColumn<QString>("run");
    QTest::addColumn<QString>("label");
    QTest::addColumn<QString>("badge");
    QTest::addColumn<QString>("phrase");

    QTest::newRow("every week") << "Full sat at 21:00"
        << "Every week" << "all" << "";
    QTest::newRow("first") << "Full 1st sat at 21:00"
        << "1st" << "1st" << "first week only";
    QTest::newRow("third") << "Full 3rd sat at 21:00"
        << "3rd" << "3rd" << "third week only";
    QTest::newRow("second to fifth") << "Full 2nd-5th sat at 21:00"
        << "2nd-5th" << "2-5" << "only 2nd-5th";
    QTest::newRow("first and third") << "Full 1st,3rd sat at 21:00"
        << "1st,3rd" << "1,3" << "only 1st,3rd";
    QTest::newRow("last") << "Full last fri at 21:00"
        << "last" << "last" << "last week only";
    QTest::newRow("months") << "Full jan-mar sat at 21:00"
        << "jan-mar" << "jan-mar" << "only jan-mar";
    QTest::newRow("month list") << "Full jan,jul sat at 21:00"
        << "jan,jul" << "jan,jul" << "only jan,jul";
    QTest::newRow("months and week") << "Full jan-mar 1st sat at 21:00"
        << "jan-mar 1st" << "jan-mar 1st" << "only jan-mar 1st";
    QTest::newRow("months and last") << "Full dec last fri at 21:00"
        << "dec last" << "dec last" << "only dec last";
}

void BScheduleGridTextTest::scopeLabels()
{
    QFETCH(QString, run);
    QFETCH(QString, label);
    QFETCH(QString, badge);
    QFETCH(QString, phrase);

    const BRunLine line = BRunParser::parse(run);
    QVERIFY2(line.spec.has_value(), qPrintable(line.parseError));
    const BGridScope scope = BGridScope::of(line.spec->mask);
    QCOMPARE(BScheduleGridText::scopeLabel(scope), label);
    QCOMPARE(BScheduleGridText::scopeBadge(scope), badge);
    QCOMPARE(BScheduleGridText::scopePhrase(scope), phrase);
}

void BScheduleGridTextTest::scopeLabelsOfAModel()
{
    BScheduleGridModel model;
    BSchedule schedule = typicalSchedule();
    schedule.runs.append(BRunParser::parse(QStringLiteral("Level=Full last fri at 23:00")));
    schedule.runs.append(BRunParser::parse(QStringLiteral("Level=Full apr-jun 1st sun at 3:00")));
    model.setSchedule(schedule);

    QStringList labels;
    for (const BGridScopeInfo &info : model.scopes())
        labels.append(QStringLiteral("%1 (%2)").arg(BScheduleGridText::scopeLabel(info.scope))
                          .arg(info.runCount));
    QCOMPARE(labels, QStringList({"Every week (1)", "1st (1)", "2nd-5th (1)", "last (1)",
                                  "apr-jun 1st (1)"}));

    // A scope without lines keeps its label
    const int added = model.addScope(scopeOf(1 << 2));
    QVERIFY(added >= 0);
    QCOMPARE(BScheduleGridText::scopeLabel(model.scopes()[added].scope), QStringLiteral("3rd"));
    QCOMPARE(model.scopes()[added].runCount, 0);
}

// --- Levels, brushes, cells -------------------------------------------------------

void BScheduleGridTextTest::levelLetters()
{
    using Level = std::optional<QString>;
    QCOMPARE(BScheduleGridText::levelLetter(Level("Full")), QStringLiteral("F"));
    QCOMPARE(BScheduleGridText::levelLetter(Level("Differential")), QStringLiteral("D"));
    QCOMPARE(BScheduleGridText::levelLetter(Level("Incremental")), QStringLiteral("I"));
    QCOMPARE(BScheduleGridText::levelLetter(Level("VirtualFull")), QStringLiteral("V"));
    QCOMPARE(BScheduleGridText::levelLetter(Level("Base")), QStringLiteral("B"));
    // Not to be taken for VirtualFull
    QCOMPARE(BScheduleGridText::levelLetter(Level("VolumeToCatalog")), QStringLiteral("Vo"));
    QCOMPARE(BScheduleGridText::levelLetter(Level("Catalog")), QStringLiteral("Ca"));
    // No level: the job's own
    QCOMPARE(BScheduleGridText::levelLetter(Level()), QStringLiteral("J"));
    QCOMPARE(BScheduleGridText::levelName(Level()), QStringLiteral("level of the job"));
    QCOMPARE(BScheduleGridText::levelName(Level("Full")), QStringLiteral("Full"));

    QCOMPARE(BScheduleGridText::levelColorCode(Level("Full")), QStringLiteral("F"));
    QCOMPARE(BScheduleGridText::levelColorCode(Level("Catalog")), QString());
    QCOMPARE(BScheduleGridText::levelColorCode(Level()), QString());
}

void BScheduleGridTextTest::brushTexts()
{
    BGridBrush brush;
    brush.overrides.level = QStringLiteral("Full");
    brush.minute = 5;
    QCOMPARE(BScheduleGridText::brushText(brush), QStringLiteral("Full at :05"));
    QVERIFY(!BScheduleGridText::hasOverridesBesideLevel(brush.overrides));

    brush.overrides.pool = QStringLiteral("Monthly");
    brush.overrides.storage = QStringLiteral("Tape");
    brush.overrides.priority = 5;
    brush.overrides.spoolData = true;
    QVERIFY(BScheduleGridText::hasOverridesBesideLevel(brush.overrides));
    QCOMPARE(BScheduleGridText::overridesText(brush.overrides),
             QStringLiteral("pool Monthly, storage Tape, priority 5, spool data yes"));
    QCOMPARE(BScheduleGridText::brushText(brush),
             QStringLiteral("Full at :05, pool Monthly, storage Tape, priority 5, spool data yes"));

    BGridRun run;
    run.overrides = brush.overrides;
    run.minute = 5;
    QCOMPARE(BScheduleGridText::runText(run, 21),
             QStringLiteral("Full at 21:05, pool Monthly, storage Tape, priority 5, spool data yes"));

    BGridBrush job;
    QCOMPARE(BScheduleGridText::brushText(job), QStringLiteral("level of the job at :00"));
}

void BScheduleGridTextTest::brushTags()
{
    QCOMPARE(BScheduleGridText::brushTag(0), QStringLiteral("A"));
    QCOMPARE(BScheduleGridText::brushTag(25), QStringLiteral("Z"));
    QCOMPARE(BScheduleGridText::brushTag(26), QStringLiteral("AA"));
    QCOMPARE(BScheduleGridText::brushTag(27), QStringLiteral("AB"));

    // Only a brush that overrides more than the level has a tag
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        QStringLiteral("Level=Full Pool=Monthly 1st sat at 21:00"),
        QStringLiteral("Level=Differential 2nd-5th sat at 21:00"),
        QStringLiteral("Level=Incremental Storage=Tape mon-fri at 21:05"),
        QStringLiteral("Level=Full Pool=Monthly jan 1st sun at 21:00"),
    }));
    const QList<BGridBrush> brushes = model.brushes();
    QCOMPARE(brushes.size(), 3);
    QCOMPARE(BScheduleGridText::brushTags(brushes), QStringList({"A", "", "B"}));
}

void BScheduleGridTextTest::cellNames()
{
    BScheduleGridModel model;
    model.setSchedule(typicalSchedule());

    // "Every week": the Incremental is in scope, Saturday has the two others
    QCOMPARE(BScheduleGridText::cellName(model, Mon, 21),
             QStringLiteral("Monday 21 hours: Incremental at 21:05"));
    QCOMPARE(BScheduleGridText::cellName(model, Mon, 3), QStringLiteral("Monday 3 hours: no run"));
    QCOMPARE(BScheduleGridText::cellName(model, Sat, 21),
             QStringLiteral("Saturday 21 hours: no run in this scope; in other scopes: "
                            "Full at 21:00, pool Monthly, first week only; "
                            "Differential at 21:00, only 2nd-5th"));

    // "1st": the wording of the plan
    model.setActiveScope(model.indexOfScope(scopeOf(1 << 0)));
    QCOMPARE(BScheduleGridText::cellName(model, Sat, 21),
             QStringLiteral("Saturday 21 hours: Full at 21:00, pool Monthly, first week only; "
                            "in other scopes: Differential at 21:00, only 2nd-5th"));

    // A view that starts on Sunday names its rows accordingly
    model.setFirstDayOfWeek(Qt::Sunday);
    QCOMPARE(BScheduleGridText::cellName(model, 0, 3), QStringLiteral("Sunday 3 hours: no run"));

    // A line without a time says so
    BScheduleGridModel untimed;
    untimed.setSchedule(scheduleOf({QStringLiteral("Level=Full sat")}));
    QCOMPARE(BScheduleGridText::cellName(untimed, Sat, 4),
             QStringLiteral("Saturday 4 hours: Full at 04:00, the Run line names no time"));
}

// --- Calendar rules -----------------------------------------------------------------

void BScheduleGridTextTest::ruleReadings_data()
{
    QTest::addColumn<QString>("run");
    QTest::addColumn<QString>("reading");

    QTest::newRow("first of the month") << "Level=Full on 1 at 21:00"
        << "on the 1st of every month at 21:00";
    QTest::newRow("all other days") << "Level=Differential on 2-31 at 2:05"
        << "on the 2nd to 31st of every month at 02:05";
    QTest::newRow("two days") << "Level=Full on 1,15 at 3:00"
        << "on the 1st and 15th of every month at 03:00";
    QTest::newRow("days and a range") << "Level=Full on 1,3,10-12 at 3:00"
        << "on the 1st, 3rd and 10th to 12th of every month at 03:00";
    QTest::newRow("twenty-second") << "Level=Full on 22,23 at 3:00"
        << "on the 22nd and 23rd of every month at 03:00";
    QTest::newRow("in some months") << "Level=Full jan,jul on 1 at 4:00"
        << "on the 1st of January and July at 04:00";
    QTest::newRow("month range") << "Level=Full jan-mar on 1 at 4:00"
        << "on the 1st of January to March at 04:00";
    QTest::newRow("day modulo") << "Level=Full 1/2 at 23:10"
        << "on every 2nd day (starting with the 1st) of every month at 23:10";
    QTest::newRow("other day modulo") << "Level=Full 2/2 at 23:10"
        << "on every 2nd day (starting with the 2nd) of every month at 23:10";
    QTest::newRow("week modulo") << "Level=Full w01/w02 sun at 23:10"
        << "in every 2nd week of the year (starting with week 1), on Sunday, at 23:10";
    QTest::newRow("one week") << "Level=Full w03 mon at 1:00"
        << "in week 3 of the year, on Monday, at 01:00";
    QTest::newRow("weeks") << "Level=Full w01-w04 at 1:00"
        << "in weeks 1 to 4 of the year at 01:00";
    QTest::newRow("friday the 13th") << "Level=Full fri on 13 at 6:00"
        << "on the 13th of every month, only if it is a Friday, at 06:00";
    QTest::newRow("in the first week") << "Level=Full 1st on 1-3 at 6:00"
        << "on the 1st to 3rd of every month, in the 1st week of the month, at 06:00";
    QTest::newRow("two times") << "Level=Full on 1 at 8:00 at 20:00"
        << "on the 1st of every month at 08:00 and 20:00";
    QTest::newRow("hourly") << "Level=Incremental on 1 hourly at 0:15"
        << "on the 1st of every month, every hour at :15";
    QTest::newRow("no time") << "Level=Full on 1"
        << "on the 1st of every month, every hour at :00 (the line names no time)";
}

void BScheduleGridTextTest::ruleReadings()
{
    QFETCH(QString, run);
    QFETCH(QString, reading);

    BScheduleGridModel model;
    model.setSchedule(scheduleOf({run}));
    QVERIFY2(model.notUnderstood().isEmpty(),
             qPrintable(model.notUnderstood().value(0).reason));
    QCOMPARE(model.calendarRules().size(), 1);
    QCOMPARE(BScheduleGridText::ruleReading(model.calendarRules().first()), reading);
    QCOMPARE(BScheduleGridText::ruleTag(0), QStringLiteral("R1"));
}

// --- Mini calendar --------------------------------------------------------------------

void BScheduleGridTextTest::calendarOfTheTypicalSchedule()
{
    // Five weeks from Monday 16 November 2026; the first Saturday of
    // December is the 5th
    const QDate first(2026, 11, 16);
    const QList<BScheduleCalendarDay> days = BScheduleCalendar::days(typicalSchedule(), first);
    QCOMPARE(days.size(), 35);

    for (const BScheduleCalendarDay &day : days) {
        QStringList expected;
        switch (day.date.dayOfWeek()) {
        case Qt::Saturday:
            expected << (day.date.day() <= 7 ? QStringLiteral("Full") : QStringLiteral("Differential"));
            break;
        case Qt::Sunday:
            break;
        default:
            expected << QStringLiteral("Incremental");
            break;
        }
        QVERIFY2(levelNames(day) == expected,
                 qPrintable(QStringLiteral("%1: %2").arg(day.date.toString(Qt::ISODate),
                                                         levelNames(day).join(QLatin1Char(',')))));
    }

    // The check the calendar is there for
    QCOMPARE(days[19].date, QDate(2026, 12, 5));
    QCOMPARE(levelNames(days[19]), QStringList({"Full"}));
    QCOMPARE(days[19].runs.size(), 1);
    QCOMPARE(days[19].runs.first().run, 0);
    QCOMPARE(days[19].runs.first().when, QDateTime(QDate(2026, 12, 5), QTime(21, 0)));
    QCOMPARE(levelNames(days[5]), QStringList({"Differential"}));      // 21 November
    QCOMPARE(days[0].runs.first().when.time(), QTime(21, 5));          // Incremental at :05
}

void BScheduleGridTextTest::calendarAgreesWithOccurrences_data()
{
    QTest::addColumn<QStringList>("runs");
    QTest::addColumn<QDate>("first");

    QTest::newRow("typical") << QStringList({"Level=Full Pool=Monthly 1st sat at 21:00",
                                             "Level=Differential 2nd-5th sat at 21:00",
                                             "Level=Incremental mon-fri at 21:05"})
                             << QDate(2026, 11, 16);
    QTest::newRow("last friday and a rule") << QStringList({"Level=Full last fri at 21:00",
                                                            "Level=Differential on 1 at 3:00",
                                                            "sun at 2:00",
                                                            "not a run line"})
                                            << QDate(2026, 12, 28);
    QTest::newRow("hourly, over a change of clocks") << QStringList({"Level=Incremental hourly at 0:15"})
                                                     << QDate(2027, 3, 22);
}

void BScheduleGridTextTest::calendarAgreesWithOccurrences()
{
    QFETCH(QStringList, runs);
    QFETCH(QDate, first);

    const BSchedule schedule = scheduleOf(runs);
    const QList<BScheduleCalendarDay> days = BScheduleCalendar::days(schedule, first);
    QCOMPARE(days.size(), 35);

    // The same question the timeline asks, one date at a time
    for (const BScheduleCalendarDay &day : days) {
        const QList<BOccurrence> occurrences = BScheduleOccurrences::occurrences(
            schedule, day.date.startOfDay(), day.date.addDays(1).startOfDay());
        QCOMPARE(day.runs.size(), occurrences.size());
        QList<std::optional<QString>> levels;
        for (int i = 0; i < occurrences.size(); ++i) {
            QCOMPARE(day.runs[i].when, occurrences[i].when);
            QCOMPARE(day.runs[i].run, occurrences[i].run);
            const std::optional<QString> level = schedule.runs[occurrences[i].run].spec->overrides.level;
            QVERIFY(day.runs[i].level == level);
            if (!levels.contains(level))
                levels.append(level);
        }
        QVERIFY(day.levels() == levels);
    }
}

void BScheduleGridTextTest::calendarOfACalendarRule()
{
    // By day of the month: not in the grid, but in the calendar
    const BSchedule schedule = scheduleOf({QStringLiteral("Level=Full on 1 at 21:00"),
                                           QStringLiteral("Level=Incremental on 2-31 at 21:00")});
    const QList<BScheduleCalendarDay> days = BScheduleCalendar::days(schedule, QDate(2026, 11, 16));
    for (const BScheduleCalendarDay &day : days) {
        QCOMPARE(levelNames(day), QStringList({day.date.day() == 1 ? "Full" : "Incremental"}));
    }
    QCOMPARE(days[15].date, QDate(2026, 12, 1));
    QCOMPARE(levelNames(days[15]), QStringList({"Full"}));

    // A run without a level is the job's own level
    const BSchedule job = scheduleOf({QStringLiteral("on 20 at 1:00")});
    const QList<BScheduleCalendarDay> jobDays = BScheduleCalendar::days(job, QDate(2026, 11, 16));
    QCOMPARE(jobDays[4].date, QDate(2026, 11, 20));
    QCOMPARE(jobDays[4].levels().size(), 1);
    QVERIFY(!jobDays[4].levels().first().has_value());
    QVERIFY(jobDays[3].levels().isEmpty());
}

void BScheduleGridTextTest::calendarNavigation()
{
    const QDate today(2026, 11, 19);    // a Thursday
    QCOMPARE(BScheduleCalendar::weekStart(today, Qt::Monday), QDate(2026, 11, 16));
    QCOMPARE(BScheduleCalendar::weekStart(today, Qt::Sunday), QDate(2026, 11, 15));
    QCOMPARE(BScheduleCalendar::weekStart(QDate(2026, 11, 16), Qt::Monday), QDate(2026, 11, 16));

    // Next: the week with the 1st of December (a Tuesday)
    const QDate current(2026, 11, 16);
    const QDate december = BScheduleCalendar::nextMonth(current, Qt::Monday);
    QCOMPARE(december, QDate(2026, 11, 30));
    QCOMPARE(BScheduleCalendar::nextMonth(december, Qt::Monday), QDate(2026, 12, 28));  // 1 January 2027

    // Previous: first to the start of the month on display (1 November is a
    // Sunday), then month by month
    const QDate november = BScheduleCalendar::previousMonth(current, Qt::Monday);
    QCOMPARE(november, QDate(2026, 10, 26));
    QCOMPARE(BScheduleCalendar::previousMonth(november, Qt::Monday), QDate(2026, 9, 28));
    QCOMPARE(BScheduleCalendar::nextMonth(november, Qt::Monday), december);

    // There and back
    QCOMPARE(BScheduleCalendar::previousMonth(december, Qt::Monday), november);
}

// --- Geometry ---------------------------------------------------------------------------

void BScheduleGridTextTest::geometryCellsThereAndBack()
{
    BScheduleGridGeometry geometry;
    geometry.setHeaderSize(100, 24);
    geometry.setViewportSize(QSize(1000, 400));

    // The cells take the room there is
    QCOMPARE(geometry.cellWidth(), 37);     // (1000 - 100) / 24
    QCOMPARE(geometry.cellHeight(), 53);    // (400 - 24) / 7
    QCOMPARE(geometry.scrollRange(), QSize(0, 0));

    for (int row = 0; row < 7; ++row) {
        for (int column = 0; column < 24; ++column) {
            const QRect rect = geometry.cellRect(row, column);
            QCOMPARE(rect.size(), QSize(37, 53));
            const BScheduleGridGeometry::Hit expected{BScheduleGridGeometry::Cell, row, column};
            QVERIFY(geometry.hitTest(rect.center()) == expected);
            QVERIFY(geometry.hitTest(rect.topLeft()) == expected);
            QVERIFY(geometry.hitTest(rect.bottomRight()) == expected);
        }
    }
    QCOMPARE(geometry.cellRect(0, 0).topLeft(), QPoint(100, 24));
    QCOMPARE(geometry.cellRect(6, 23).bottomRight(), QPoint(100 + 24 * 37 - 1, 24 + 7 * 53 - 1));

    // Right of the last hour and below the last day there is nothing
    QCOMPARE(geometry.hitTest(QPoint(100 + 24 * 37, 100)).part, BScheduleGridGeometry::Outside);
    QCOMPARE(geometry.hitTest(QPoint(200, 24 + 7 * 53)).part, BScheduleGridGeometry::Outside);
    QCOMPARE(geometry.hitTest(QPoint(-1, 50)).part, BScheduleGridGeometry::Outside);
    QCOMPARE(geometry.hitTest(QPoint(1000, 50)).part, BScheduleGridGeometry::Outside);

    // A very high window does not stretch the cells without limit
    geometry.setViewportSize(QSize(1000, 2000));
    QCOMPARE(geometry.cellHeight(), int(BScheduleGridGeometry::MaxCellHeight));
}

void BScheduleGridTextTest::geometryHeadersAndCorner()
{
    BScheduleGridGeometry geometry;
    geometry.setHeaderSize(100, 24);
    geometry.setViewportSize(QSize(1000, 400));

    QCOMPARE(geometry.cornerRect(), QRect(0, 0, 100, 24));
    QCOMPARE(geometry.hitTest(QPoint(50, 12)).part, BScheduleGridGeometry::Corner);

    for (int row = 0; row < 7; ++row) {
        const BScheduleGridGeometry::Hit hit = geometry.hitTest(geometry.rowHeaderRect(row).center());
        QCOMPARE(hit.part, BScheduleGridGeometry::RowHeader);
        QCOMPARE(hit.row, row);
        // A header is as high as its cells and next to them
        QCOMPARE(geometry.rowHeaderRect(row).top(), geometry.cellRect(row, 0).top());
        QCOMPARE(geometry.rowHeaderRect(row).height(), geometry.cellRect(row, 0).height());
    }
    for (int column = 0; column < 24; ++column) {
        const BScheduleGridGeometry::Hit hit = geometry.hitTest(geometry.columnHeaderRect(column).center());
        QCOMPARE(hit.part, BScheduleGridGeometry::ColumnHeader);
        QCOMPARE(hit.column, column);
        QCOMPARE(geometry.columnHeaderRect(column).left(), geometry.cellRect(0, column).left());
        QCOMPARE(geometry.columnHeaderRect(column).width(), geometry.cellRect(0, column).width());
    }
}

void BScheduleGridTextTest::geometryScrolled()
{
    // Too small for the cells at their minimum size: they scroll
    BScheduleGridGeometry geometry;
    geometry.setHeaderSize(100, 24);
    geometry.setViewportSize(QSize(500, 200));
    QCOMPARE(geometry.cellWidth(), int(BScheduleGridGeometry::MinCellWidth));
    QCOMPARE(geometry.cellHeight(), int(BScheduleGridGeometry::MinCellHeight));
    QCOMPARE(geometry.contentSize(), QSize(100 + 24 * 30, 24 + 7 * 34));
    QCOMPARE(geometry.contentSize(), geometry.minimumContentSize());
    QCOMPARE(geometry.scrollRange(), QSize(320, 62));

    geometry.setScrollOffset(QPoint(60, 34));
    // The cells move ...
    QCOMPARE(geometry.cellRect(0, 0).topLeft(), QPoint(40, -10));
    BScheduleGridGeometry::Hit hit = geometry.hitTest(QPoint(100, 24));
    QCOMPARE(hit.part, BScheduleGridGeometry::Cell);
    QCOMPARE(hit.row, 1);
    QCOMPARE(hit.column, 2);
    // ... the headers stay at the edges and follow in one direction only
    hit = geometry.hitTest(QPoint(50, 30));
    QCOMPARE(hit.part, BScheduleGridGeometry::RowHeader);
    QCOMPARE(hit.row, 1);
    hit = geometry.hitTest(QPoint(130, 10));
    QCOMPARE(hit.part, BScheduleGridGeometry::ColumnHeader);
    QCOMPARE(hit.column, 3);
    QCOMPARE(geometry.hitTest(QPoint(50, 10)).part, BScheduleGridGeometry::Corner);
    QCOMPARE(geometry.rowHeaderRect(1).left(), 0);
    QCOMPARE(geometry.columnHeaderRect(3).top(), 0);

    // There and back for every cell that is in view
    for (int row = 0; row < 7; ++row) {
        for (int column = 0; column < 24; ++column) {
            const QPoint center = geometry.cellRect(row, column).center();
            if (!geometry.scrollAreaRect().contains(center))
                continue;
            const BScheduleGridGeometry::Hit expected{BScheduleGridGeometry::Cell, row, column};
            QVERIFY(geometry.hitTest(center) == expected);
        }
    }
}

void BScheduleGridTextTest::geometryRuleBand()
{
    BScheduleGridGeometry geometry;
    geometry.setHeaderSize(100, 24);
    geometry.setViewportSize(QSize(1000, 400));
    QVERIFY(!geometry.hasRuleBand());
    QVERIFY(geometry.ruleCellRect(3).isNull());
    const int heightWithout = geometry.contentSize().height();

    geometry.setRuleBandHeight(22);
    QVERIFY(geometry.hasRuleBand());
    // The band takes its room from the cells: (400 - 24 - 6 - 22) / 7
    QCOMPARE(geometry.cellHeight(), 49);
    QVERIFY(geometry.contentSize().height() <= 400);
    QVERIFY(heightWithout <= 400);

    const int bandTop = 24 + 7 * 49 + BScheduleGridGeometry::RuleBandGap;
    QCOMPARE(geometry.ruleCellRect(0).topLeft(), QPoint(100, bandTop));
    QCOMPARE(geometry.ruleCellRect(21).left(), geometry.cellRect(0, 21).left());

    BScheduleGridGeometry::Hit hit = geometry.hitTest(geometry.ruleCellRect(21).center());
    QCOMPARE(hit.part, BScheduleGridGeometry::RuleCell);
    QCOMPARE(hit.column, 21);
    QCOMPARE(hit.row, -1);      // not a cell
    QCOMPARE(geometry.hitTest(geometry.ruleHeaderRect().center()).part,
             BScheduleGridGeometry::RuleHeader);
    // The gap between the last day and the band belongs to neither
    QCOMPARE(geometry.hitTest(QPoint(300, bandTop - 3)).part, BScheduleGridGeometry::Outside);
    QCOMPARE(geometry.hitTest(QPoint(300, bandTop + 22)).part, BScheduleGridGeometry::Outside);
}

void BScheduleGridTextTest::geometryNearestCell()
{
    BScheduleGridGeometry geometry;
    geometry.setHeaderSize(100, 24);
    geometry.setViewportSize(QSize(1000, 400));

    // Dragging beyond the cells stays on the nearest one
    QVERIFY(geometry.nearestCell(QPoint(-50, -50)) == (BGridPos{0, 0}));
    QVERIFY(geometry.nearestCell(QPoint(5000, 5000)) == (BGridPos{6, 23}));
    QVERIFY(geometry.nearestCell(QPoint(50, 200)) == (BGridPos{3, 0}));        // on a row header
    QVERIFY(geometry.nearestCell(QPoint(300, 5)) == (BGridPos{0, 5}));         // on a column header
    QVERIFY(geometry.nearestCell(geometry.cellRect(4, 17).center()) == (BGridPos{4, 17}));
}

QTEST_APPLESS_MAIN(BScheduleGridTextTest)
#include "bschedulegridtext_test.moc"
