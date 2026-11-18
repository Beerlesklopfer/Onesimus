/**
 * @file bscheduledragplan_test.cpp
 * @brief The Run lines written after a drag in the schedule view
 *
 * The lines of the new schedule are the Director's own lines, copied
 * verbatim; only the dragged line is generated from its parsed run
 * (BRunSpec). These tests pin down when the result may be written, when it
 * needs a confirmation and when it is refused. Also covers BScheduleEntry
 * as the display adapter over the Run line model, and the Messages names
 * offered by the job wizard. Runs without a Director (registered with CTest).
 */

#include <QtTest>

#include "models/bresourcemodels.h"
#include "schedules/bscheduledragplan.h"

using BScheduleDragPlan::Plan;
using BScheduleDragPlan::Reason;
using BScheduleDragPlan::Verdict;

namespace {

/// The dragged entry as the Gantt view has it: parsed from its Run line
BScheduleEntry entryOf(const QString &run)
{
    BScheduleEntry entry = BScheduleEntry::fromRunDirective(run);
    entry.scheduleName = "WeeklyCycle";
    return entry;
}

} // namespace

class BScheduleDragPlanTest : public QObject
{
    Q_OBJECT

private slots:
    void plainScheduleChangesOnlyTheDraggedTime();
    void untouchedLinesAreCopiedVerbatim();
    void oneLinePerRunLineNotPerJob();
    void weekOfMonthLinesAreKept();
    void formsTheOldParserLostAreKept_data();
    void formsTheOldParserLostAreKept();
    void overridesStayInFrontOfTheDate();
    void linesTheOldParserChangedAreCopied_data();
    void linesTheOldParserChangedAreCopied();
    void respelledLinesNeedConfirmation_data();
    void respelledLinesNeedConfirmation();
    void lineNotUnderstoodRefusesTheSchedule_data();
    void lineNotUnderstoodRefusesTheSchedule();
    void undeterminableCasesAreRefused();
    void sameRun_data();
    void sameRun();
    void lineWithoutATimeRunsEveryHour();
    void entryFromRunLine();
    void entryNeverGuessesALevel();
    void entryNotUnderstood();
    void entryReadFromALineIsWrittenAsThatLine();
    void entryMadeInTheApplication();
    void modelKeepsLinesNotUnderstoodApart();
    void messagesNamesFromJobsAndJobDefs();
};

void BScheduleDragPlanTest::plainScheduleChangesOnlyTheDraggedTime()
{
    const QStringList runs = {"Full sat at 21:00", "Incremental mon-fri at 23:05"};
    const Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[1]), 22, 30);

    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.reason, Reason::None);
    QCOMPARE(plan.draggedLine, 1);
    QVERIFY(plan.differingLines.isEmpty());
    QCOMPARE(plan.originalRuns, runs);
    // The dragged line is written by BRunWriter (canonical: Level=)
    QCOMPARE(plan.generatedRuns,
             QStringList({"Full sat at 21:00", "Level=Incremental mon-fri at 22:30"}));
}

void BScheduleDragPlanTest::untouchedLinesAreCopiedVerbatim()
{
    // Capitals, level=, a one-digit hour and a day list in another order:
    // nothing of it is touched, except on the dragged line
    const QStringList runs = {"Level=Full Sat at 2:05", "Incremental Fri,Mon at 23:05",
                              "Differential daily at 4:00", "VirtualFull at 5:00  # weekly"};
    const Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 3, 15);

    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.draggedLine, 0);
    QCOMPARE(plan.generatedRuns,
             QStringList({"Level=Full sat at 03:15", "Incremental Fri,Mon at 23:05",
                          "Differential daily at 4:00", "VirtualFull at 5:00  # weekly"}));
}

void BScheduleDragPlanTest::oneLinePerRunLineNotPerJob()
{
    // The Gantt view clones an entry for every job that uses the schedule and
    // fills in the job's pool, storage and priority. Neither the clones nor
    // these values may reach the generated lines.
    const QStringList runs = {"Full sat at 21:00", "Incremental mon-fri at 23:05"};
    BScheduleEntry dragged = entryOf(runs[1]);
    dragged.jobName = "backup-pdc";
    dragged.client = "pdc-fd";
    dragged.pool = "Monthly";
    dragged.storage = "File";
    dragged.priority = 5;

    const Plan plan = BScheduleDragPlan::plan(runs, dragged, 22, 0);
    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.generatedRuns.size(), runs.size());
    QCOMPARE(plan.generatedRuns[1], QString("Level=Incremental mon-fri at 22:00"));
}

void BScheduleDragPlanTest::weekOfMonthLinesAreKept()
{
    // The default schedule of a Bareos installation
    const QStringList runs = {"Full 1st sat at 21:00", "Differential 2nd-5th sat at 21:00",
                              "Incremental mon-fri at 21:00"};

    const Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 20, 0);
    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.draggedLine, 0);
    QCOMPARE(plan.generatedRuns,
             QStringList({"Level=Full 1st sat at 20:00", "Differential 2nd-5th sat at 21:00",
                          "Incremental mon-fri at 21:00"}));

    // Full and Differential differ in level and week only: the dragged line
    // is still found
    const Plan second = BScheduleDragPlan::plan(runs, entryOf(runs[1]), 22, 0);
    QCOMPARE(second.verdict, Verdict::Exact);
    QCOMPARE(second.draggedLine, 1);
    QCOMPARE(second.generatedRuns[1], QString("Level=Differential 2nd-5th sat at 22:00"));
}

void BScheduleDragPlanTest::formsTheOldParserLostAreKept_data()
{
    QTest::addColumn<QString>("run");

    QTest::newRow("day range") << "Incremental tue-thu at 23:05";
    QTest::newRow("day range, capitals") << "Incremental Tue-Thu at 23:05";
    QTest::newRow("two week-of-month") << "Full 1st 3rd sat at 21:00";
    QTest::newRow("week-of-month word") << "Full first sat at 21:00";
    QTest::newRow("day of month") << "Full on 1 at 2:00";
    QTest::newRow("day of month range") << "Full 1-7 at 2:00";
    QTest::newRow("months and week of year") << "Full jan-mar w02 sat at 2:00";
    QTest::newRow("last week") << "Full last fri at 21:00";
}

void BScheduleDragPlanTest::formsTheOldParserLostAreKept()
{
    QFETCH(QString, run);
    const QStringList runs = {"Incremental mon at 20:00", run};

    // Another line is dragged: this one is copied as it is
    Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 19, 0);
    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.draggedLine, 0);
    QCOMPARE(plan.generatedRuns, QStringList({"Level=Incremental mon at 19:00", run}));

    // The line itself is dragged: everything but the time is kept
    plan = BScheduleDragPlan::plan(runs, entryOf(run), 19, 30);
    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.draggedLine, 1);
    QCOMPARE(plan.generatedRuns[0], runs[0]);
    QVERIFY2(BScheduleDragPlan::sameRun(run, plan.generatedRuns[1], 19, 30),
             qPrintable(plan.generatedRuns[1]));
    QVERIFY(!BScheduleDragPlan::sameRun(run, plan.generatedRuns[1]));
}

void BScheduleDragPlanTest::overridesStayInFrontOfTheDate()
{
    // pool=Monthly is a pool, not "monthly": the line keeps its week and day
    const QStringList runs = {"Incremental mon at 20:00",
                              "Level=Full Pool=Monthly Storage=File 1st sat at 21:00"};
    Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 19, 0);
    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.generatedRuns[1], runs[1]);

    // Dragged, the overrides are written in front of the date, where Bareos
    // reads them
    plan = BScheduleDragPlan::plan(runs, entryOf(runs[1]), 22, 15);
    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.draggedLine, 1);
    QCOMPARE(plan.generatedRuns,
             QStringList({"Incremental mon at 20:00",
                          "Level=Full Pool=Monthly Storage=File 1st sat at 22:15"}));

    // In the spelling of "show schedules"
    const QStringList shown = {"pool=\"Monthly\" storage=\"File\" Full 1st Sat at 21:00"};
    plan = BScheduleDragPlan::plan(shown, entryOf(shown[0]), 19, 0);
    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.generatedRuns,
             QStringList({"Level=Full Pool=Monthly Storage=File 1st sat at 19:00"}));

    // level= is the level
    const QStringList level = {"level=Full sat at 21:00"};
    plan = BScheduleDragPlan::plan(level, entryOf(level[0]), 19, 0);
    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.generatedRuns, QStringList({"Level=Full sat at 19:00"}));
}

void BScheduleDragPlanTest::linesTheOldParserChangedAreCopied_data()
{
    QTest::addColumn<QString>("run");

    // The old parser made Incremental of a level it did not know ...
    QTest::newRow("another level") << "Base sun at 1:00";
    // ... and of a line without a level, which leaves the level to the job
    QTest::newRow("no level") << "sun at 1:00";
    // ... and one run a day of "hourly"
    QTest::newRow("hourly") << "Incremental hourly at 0:15";
    QTest::newRow("no time") << "Incremental mon-fri";
}

void BScheduleDragPlanTest::linesTheOldParserChangedAreCopied()
{
    QFETCH(QString, run);

    const QStringList runs = {"Full sat at 20:00", run};
    const Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 19, 0);

    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.reason, Reason::None);
    QCOMPARE(plan.draggedLine, 0);
    QVERIFY(plan.differingLines.isEmpty());
    QCOMPARE(plan.generatedRuns, QStringList({"Level=Full sat at 19:00", run}));
}

void BScheduleDragPlanTest::respelledLinesNeedConfirmation_data()
{
    QTest::addColumn<QString>("run");
    QTest::addColumn<QString>("generated");

    // Two forms the Director prints and Bareos does not read back
    QTest::newRow("day of month behind the level") << "Full 1 at 02:05" << "Level=Full on 1 at 02:05";
    QTest::newRow("glued hours") << "Incremental at 01:15at 13:15"
                                 << "Level=Incremental at 01:15 at 13:15";
}

void BScheduleDragPlanTest::respelledLinesNeedConfirmation()
{
    QFETCH(QString, run);
    QFETCH(QString, generated);

    const QStringList runs = {"Full sat at 20:00", run};
    const Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 19, 0);

    QCOMPARE(plan.verdict, Verdict::NeedsConfirmation);
    QCOMPARE(plan.reason, Reason::None);
    QCOMPARE(plan.draggedLine, 0);
    QCOMPARE(plan.differingLines, QList<int>({1}));
    // Both versions are there to be shown side by side, and they say the same
    QCOMPARE(plan.originalRuns, runs);
    QCOMPARE(plan.generatedRuns, QStringList({"Level=Full sat at 19:00", generated}));
    QVERIFY(BScheduleDragPlan::sameRun(run, generated));
}

void BScheduleDragPlanTest::lineNotUnderstoodRefusesTheSchedule_data()
{
    QTest::addColumn<QString>("run");
    QTest::addColumn<QString>("errorContains");

    // What the old wizard wrote: an override behind the time
    QTest::newRow("override behind the time") << "Full 1st sat at 21:00 pool=Monthly" << "pool";
    QTest::newRow("unknown level") << "Bogus sun at 1:00" << "Bogus";
    QTest::newRow("unknown level=") << "Level=Bogus sun at 1:00" << "Bogus";
    QTest::newRow("hour without minutes") << "Incremental mon-fri at 9pm" << "9pm";
    QTest::newRow("time without at") << "Incremental mon-fri 21:00" << "AT";
}

void BScheduleDragPlanTest::lineNotUnderstoodRefusesTheSchedule()
{
    QFETCH(QString, run);
    QFETCH(QString, errorContains);

    // The line is not the dragged one; the schedule is refused all the same
    const QStringList runs = {"Incremental mon at 20:00", run};
    const Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 19, 0);

    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, Reason::NotUnderstood);
    QCOMPARE(plan.reasonLine, 1);
    QVERIFY2(plan.reasonText.contains(errorContains), qPrintable(plan.reasonText));
    QVERIFY(plan.generatedRuns.isEmpty());
    QCOMPARE(plan.originalRuns, runs);
}

void BScheduleDragPlanTest::undeterminableCasesAreRefused()
{
    const QStringList runs = {"Full sat at 21:00", "Incremental mon-fri at 23:05"};

    // The Run lines are not known
    Plan plan = BScheduleDragPlan::plan({}, entryOf(runs[0]), 20, 0);
    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, Reason::NoRunLines);

    // No line matches the dragged entry
    plan = BScheduleDragPlan::plan(runs, entryOf("Differential wed at 12:00"), 20, 0);
    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, Reason::DraggedLineNotFound);

    // Nor an entry that was not read from a Run line
    BScheduleEntry made;
    made.level = BScheduleEntry::Full;
    made.hour = 21;
    made.daysOfWeek = {5};
    plan = BScheduleDragPlan::plan(runs, made, 20, 0);
    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, Reason::DraggedLineNotFound);

    // Two lines match it, also in different spellings
    const QStringList twice = {"Full sat at 21:00", "Level=Full Saturday at 9:00pm"};
    plan = BScheduleDragPlan::plan(twice, entryOf(twice[0]), 20, 0);
    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, Reason::DraggedLineAmbiguous);
    QCOMPARE(plan.reasonLine, 1);

    // A line that runs at several hours has one minute for all of them
    const QStringList hourly = {"Full sat at 21:00", "Incremental hourly at 0:15"};
    BScheduleEntry bar = entryOf(hourly[1]);
    bar.hour = 7;       // the Gantt view shows one bar per hour
    plan = BScheduleDragPlan::plan(hourly, bar, 8, 0);
    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, Reason::SeveralHours);
    QCOMPARE(plan.reasonLine, 1);
    QVERIFY(plan.generatedRuns.isEmpty());

    // Not a time of day
    plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 24, 0);
    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, Reason::BadTime);
    plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 20, 60);
    QCOMPARE(plan.reason, Reason::BadTime);
}

void BScheduleDragPlanTest::sameRun_data()
{
    QTest::addColumn<QString>("original");
    QTest::addColumn<QString>("generated");
    QTest::addColumn<bool>("same");

    QTest::newRow("identical") << "Full sat at 21:00" << "Full sat at 21:00" << true;
    QTest::newRow("case and spacing") << "FULL  Sat  at 21:00" << "Full sat at 21:00" << true;
    QTest::newRow("level=") << "Level=Full sat at 21:00" << "Full sat at 21:00" << true;
    QTest::newRow("hour digits") << "Full sat at 2:05" << "Full sat at 02:05" << true;
    QTest::newRow("day order") << "Full fri,mon at 2:05" << "Full mon,fri at 02:05" << true;
    QTest::newRow("days by space") << "Full mon fri at 2:05" << "Full mon,fri at 02:05" << true;
    QTest::newRow("no day is daily") << "Full at 2:05" << "Full daily at 02:05" << true;
    QTest::newRow("sun-sat is daily") << "Full sun-sat at 2:05" << "Full daily at 02:05" << true;
    QTest::newRow("range against list") << "Full tue-thu at 2:05" << "Full tue,wed,thu at 02:05" << true;
    QTest::newRow("wrapping range") << "Full sun-fri at 2:05" << "Full sun,mon,tue,wed,thu,fri at 02:05" << true;
    QTest::newRow("long day names") << "Full Saturday at 2:05" << "Full sat at 02:05" << true;
    QTest::newRow("am/pm") << "Full sat at 9:00pm" << "Full sat at 21:00" << true;
    QTest::newRow("quoted pool") << "pool=\"Monthly\" Full sat at 21:00"
                                 << "Level=Full Pool=Monthly sat at 21:00" << true;
    QTest::newRow("on 1") << "Full 1 at 2:05" << "Level=Full on 1 at 02:05" << true;

    QTest::newRow("time") << "Full sat at 21:00" << "Full sat at 21:15" << false;
    QTest::newRow("level") << "Full sat at 21:00" << "Incremental sat at 21:00" << false;
    QTest::newRow("no level") << "sat at 21:00" << "Incremental sat at 21:00" << false;
    QTest::newRow("range lost its middle") << "Full tue-thu at 2:05" << "Full tue,thu at 02:05" << false;
    QTest::newRow("week-of-month lost") << "Full 1st sat at 21:00" << "Full sat at 21:00" << false;
    QTest::newRow("week-of-month changed") << "Full 2nd-5th sat at 21:00" << "Full 2nd sat at 21:00" << false;
    QTest::newRow("day of month lost") << "Full on 1 at 2:00" << "Full daily at 02:00" << false;
    QTest::newRow("month lost") << "Full jan sat at 21:00" << "Full sat at 21:00" << false;
    QTest::newRow("pool lost") << "Pool=Monthly Full sat at 21:00" << "Full sat at 21:00" << false;
    QTest::newRow("pool changed") << "Pool=Monthly Full sat at 21:00" << "Pool=Daily Full sat at 21:00" << false;
    QTest::newRow("pool made daily") << "Pool=Monthly Full sat at 21:00" << "Pool=Monthly Full daily at 21:00" << false;
    // A line that is not understood equals nothing, not even itself
    QTest::newRow("not understood") << "Full sat at 21:00 pool=Monthly"
                                    << "Full sat at 21:00 pool=Monthly" << false;
    QTest::newRow("not understood against understood") << "Full sat at 21:00 pool=Monthly"
                                                       << "Full sat at 21:00" << false;
}

void BScheduleDragPlanTest::sameRun()
{
    QFETCH(QString, original);
    QFETCH(QString, generated);
    QFETCH(bool, same);
    QCOMPARE(BScheduleDragPlan::sameRun(original, generated), same);

    // With a dragged time the original's own time does not count
    if (same) {
        QVERIFY(!BScheduleDragPlan::sameRun(original, generated, 13, 45));
        QString moved = generated;
        moved.replace(QRegularExpression("at \\d+:\\d+"), "at 13:45");
        QVERIFY(BScheduleDragPlan::sameRun(original, moved, 13, 45));
    }
}

void BScheduleDragPlanTest::lineWithoutATimeRunsEveryHour()
{
    // Bareos: without a time a run happens at the beginning of every hour
    QVERIFY(BScheduleDragPlan::sameRun("Full sat", "Full sat"));
    QVERIFY(BScheduleDragPlan::sameRun("Full sat", "Level=Full sat hourly at 0:00"));
    QVERIFY(!BScheduleDragPlan::sameRun("Full sat", "Full sat at 0:00"));
}

void BScheduleDragPlanTest::entryFromRunLine()
{
    // Days are 0=Mon..6=Sun in the entry, bit 0 = Sunday in Bareos
    BScheduleEntry e = BScheduleEntry::fromRunDirective("Level=Full Pool=Monthly 1st sun at 2:05");
    QVERIFY(!e.notUnderstood());
    QCOMPARE(e.level, BScheduleEntry::Full);
    QCOMPARE(e.levelName, QString("Full"));
    QCOMPARE(e.hour, 2);
    QCOMPARE(e.minute, 5);
    QCOMPARE(e.daysOfWeek, QVector<int>({6}));     // Sunday only, not every day
    QCOMPARE(e.weekSpec, QString("1st"));
    QCOMPARE(e.pool, QString("Monthly"));
    QCOMPARE(e.priority, 10);
    QVERIFY(e.matchesSource());

    e = BScheduleEntry::fromRunDirective("Incremental tue-thu at 23:05");
    QCOMPARE(e.level, BScheduleEntry::Incremental);
    QCOMPARE(e.daysOfWeek, QVector<int>({1, 2, 3}));   // with Wednesday
    QCOMPARE(e.weekSpec, QString());

    e = BScheduleEntry::fromRunDirective("Level=Incremental Pool=Daily mon-sat at 2:05");
    QCOMPARE(e.daysOfWeek, QVector<int>({0, 1, 2, 3, 4, 5}));
    QCOMPARE(e.pool, QString("Daily"));

    e = BScheduleEntry::fromRunDirective("Differential 2nd-5th sat at 21:00");
    QCOMPARE(e.weekSpec, QString("2nd-5th"));
    QCOMPARE(e.daysOfWeek, QVector<int>({5}));

    e = BScheduleEntry::fromRunDirective(
        "Storage=File Messages=Standard Priority=5 VirtualFull last fri at 9:30pm");
    QCOMPARE(e.level, BScheduleEntry::VirtualFull);
    QCOMPARE(e.hour, 21);
    QCOMPARE(e.minute, 30);
    QCOMPARE(e.weekSpec, QString("last"));
    QCOMPARE(e.storage, QString("File"));
    QCOMPARE(e.messages, QString("Standard"));
    QCOMPARE(e.priority, 5);
    QVERIFY(e.matchesSource());

    // What the fields cannot express is still in the source line
    e = BScheduleEntry::fromRunDirective("Full jan on 1 at 2:00");
    QCOMPARE(e.daysOfWeek.size(), 7);
    QVERIFY(!e.matchesSource());
    QCOMPARE(e.source.spec->mask.month, quint16(1));
    QCOMPARE(e.source.spec->mask.mday, quint32(1));

    // Several hours: the first one; the model makes one entry per hour
    e = BScheduleEntry::fromRunDirective("Full at 13:30 at 1:30");
    QCOMPARE(e.hour, 1);
    QCOMPARE(e.minute, 30);
    QVERIFY(!e.matchesSource());
}

void BScheduleDragPlanTest::entryNeverGuessesALevel()
{
    // No level on the line: none in the entry; the job's level applies
    BScheduleEntry e = BScheduleEntry::fromRunDirective("sun at 1:00");
    QVERIFY(!e.notUnderstood());
    QCOMPARE(e.level, BScheduleEntry::None);
    QCOMPARE(e.levelName, QString());
    QCOMPARE(e.levelLabel(), QString());

    // A level of Bareos without an enum value keeps its name
    e = BScheduleEntry::fromRunDirective("Base sun at 1:00");
    QCOMPARE(e.level, BScheduleEntry::None);
    QCOMPARE(e.levelName, QString("Base"));
    QCOMPARE(e.levelLabel(), QString("Base"));

    QCOMPARE(BScheduleEntry().level, BScheduleEntry::None);
    QCOMPARE(BScheduleEntry::levelFromString(""), BScheduleEntry::None);
    QCOMPARE(BScheduleEntry::levelFromString("garbage"), BScheduleEntry::None);
    QCOMPARE(BScheduleEntry::levelFromString("Base"), BScheduleEntry::None);
    QCOMPARE(BScheduleEntry::levelFromString("full"), BScheduleEntry::Full);
    QCOMPARE(BScheduleEntry::levelFromString("VirtualFull"), BScheduleEntry::VirtualFull);
    QCOMPARE(BScheduleEntry::levelFromString("Differential"), BScheduleEntry::Differential);
    QCOMPARE(BScheduleEntry::levelFromString("Incremental"), BScheduleEntry::Incremental);
    // A name, not a search in a line: "pool=Full" is no level
    QCOMPARE(BScheduleEntry::levelFromString("pool=Full sat at 21:00"), BScheduleEntry::None);
}

void BScheduleDragPlanTest::entryNotUnderstood()
{
    const QString text = "Full 1st sat at 21:00 pool=Monthly";
    const BScheduleEntry e = BScheduleEntry::fromRunDirective(text);

    QVERIFY(e.notUnderstood());
    QVERIFY(!e.source.parseError.isEmpty());
    QCOMPARE(e.source.original, text);
    // Nothing is filled in that a view could show as something else
    QCOMPARE(e.level, BScheduleEntry::None);
    QVERIFY(e.daysOfWeek.isEmpty());
    QVERIFY(e.pool.isEmpty());
    QVERIFY(!e.matchesSource());

    // Never rewritten, and not written at all
    QCOMPARE(e.toRunLine().original, text);
    QVERIFY(!e.toRunLine().spec.has_value());
    QString error;
    QVERIFY(e.toRunDirective(&error).isEmpty());
    QVERIFY2(error.contains(text), qPrintable(error));

    // An empty line is not understood either; it is not an entry made here
    const BScheduleEntry empty = BScheduleEntry::fromRunDirective("");
    QVERIFY(empty.notUnderstood());
    QVERIFY(empty.toRunDirective(&error).isEmpty());
    QVERIFY(!error.isEmpty());
}

void BScheduleDragPlanTest::entryReadFromALineIsWrittenAsThatLine()
{
    const QString text = "pool=\"Monthly\" Full jan-mar 1st Sat at 2:05  # quarter start";
    BScheduleEntry e = BScheduleEntry::fromRunDirective(text);
    QVERIFY(!e.notUnderstood());
    QCOMPARE(e.toRunDirective(), text);

    // The Gantt view fills in the job's pool, storage and priority; the
    // line does not change by that
    e.jobName = "backup-pdc";
    e.pool = "Other";
    e.storage = "File";
    e.priority = 5;
    e.hour = 7;
    QCOMPARE(e.toRunDirective(), text);
    QVERIFY(!e.toRunLine().dirty);
}

void BScheduleDragPlanTest::entryMadeInTheApplication()
{
    // As the Run dialog of the schedule wizard builds it
    BScheduleEntry e;
    e.levelName = "Incremental";
    e.level = BScheduleEntry::Incremental;
    e.hour = 23;
    e.minute = 5;
    e.daysOfWeek = {0, 1, 2, 3, 4};
    e.pool = "Daily";
    e.storage = "File";
    e.priority = 5;

    // The overrides come first. The old wizard wrote them behind the time,
    // where Bareos does not accept them.
    QString error;
    QCOMPARE(e.toRunDirective(&error),
             QString("Level=Incremental Pool=Daily Storage=File Priority=5 mon-fri at 23:05"));
    QVERIFY(error.isEmpty());
    QVERIFY(e.toRunLine().dirty);
    QVERIFY(BRunParser::parse(e.toRunDirective()).understood());

    e.weekSpec = "2nd-5th";
    e.daysOfWeek = {6};
    e.pool.clear();
    e.storage.clear();
    e.priority = 10;
    QCOMPARE(e.toRunDirective(), QString("Level=Incremental 2nd-5th sun at 23:05"));

    // Every day, and no level: no "Level=" is invented
    e = BScheduleEntry();
    e.hour = 1;
    e.daysOfWeek = {0, 1, 2, 3, 4, 5, 6};
    QCOMPARE(e.toRunDirective(), QString("at 01:00"));

    // Only the enum set: its name is the level
    e.level = BScheduleEntry::Full;
    QCOMPARE(e.toRunDirective(), QString("Level=Full at 01:00"));

    // What does not describe a run is not written
    e.weekSpec = "sometimes";
    QVERIFY(e.toRunDirective(&error).isEmpty());
    QVERIFY(!error.isEmpty());
    e.weekSpec = "jan";     // a month is not a week of the month
    QVERIFY(e.toRunDirective(&error).isEmpty());
    e.weekSpec.clear();
    e.levelName = "Bogus";
    QVERIFY(e.toRunDirective(&error).isEmpty());
    e.levelName.clear();
    e.hour = 24;
    QVERIFY(e.toRunDirective(&error).isEmpty());
}

void BScheduleDragPlanTest::modelKeepsLinesNotUnderstoodApart()
{
    BScheduleModel model;
    model.parseSchedules(R"({"result":{"schedules":[
        {"name":"Cycle","run":[
            "pool=\"Monthly\" Full 1st Sat at 21:00",
            "Full 1st sat at 21:00 pool=Monthly",
            "Incremental at 01:15at 13:15",
            "Mon-Fri at 23:05",
            ""
        ]}
    ]}})");

    // The lines as the Director reported them, all of them
    QCOMPARE(model.runDirectives("Cycle").size(), 5);
    QCOMPARE(model.runDirectives("Cycle")[1], QString("Full 1st sat at 21:00 pool=Monthly"));

    // One entry per understood line and hour
    const QList<BScheduleEntry> entries = model.entriesForSchedule("Cycle");
    QCOMPARE(entries.size(), 4);
    QCOMPARE(entries[0].level, BScheduleEntry::Full);
    QCOMPARE(entries[0].pool, QString("Monthly"));
    QCOMPARE(entries[0].daysOfWeek, QVector<int>({5}));
    QCOMPARE(entries[1].hour, 1);
    QCOMPARE(entries[2].hour, 13);
    QCOMPARE(entries[2].minute, 15);
    QCOMPARE(entries[2].source.original, entries[1].source.original);
    QCOMPARE(entries[3].level, BScheduleEntry::None);   // not Incremental
    QCOMPARE(entries[3].daysOfWeek, QVector<int>({0, 1, 2, 3, 4}));
    for (const BScheduleEntry &e : entries)
        QVERIFY(!e.notUnderstood());

    // The line that is not understood is reported, not dropped and not shown
    const QList<BRunProblem> problems = model.runProblems();
    QCOMPARE(problems.size(), 2);
    QCOMPARE(problems[1].line, 5);
    QCOMPARE(problems[1].text, QString());
    QCOMPARE(problems[0].scheduleName, QString("Cycle"));
    QCOMPARE(problems[0].line, 2);
    QCOMPARE(problems[0].text, QString("Full 1st sat at 21:00 pool=Monthly"));
    QVERIFY2(problems[0].error.contains("pool"), qPrintable(problems[0].error));

    // Read again: no stale problems
    model.parseSchedules(R"({"result":{"schedules":[{"name":"Cycle","run":["Full sat at 21:00"]}]}})");
    QVERIFY(model.runProblems().isEmpty());
    QCOMPARE(model.allEntries().size(), 1);
}

void BScheduleDragPlanTest::messagesNamesFromJobsAndJobDefs()
{
    BJobConfigModel model;
    QVERIFY(model.messagesNames().isEmpty());

    // "show jobs" names a referenced resource by a string or by an object
    model.parseShowJobs(R"({"result":{"jobs":{
        "backup-a":{"name":"backup-a","messages":"Standard","client":"a-fd"},
        "backup-b":{"name":"backup-b","Messages":{"name":"Daemon"}},
        "backup-c":{"name":"backup-c","messages":"Standard"},
        "backup-d":{"name":"backup-d"}
    }}})");
    QCOMPARE(model.messagesNames(), QStringList({"Daemon", "Standard"}));

    model.parseShowJobDefs(R"({"result":{"jobdefs":{
        "DefaultJob":{"name":"DefaultJob","messages":"Quiet"}
    }}})");
    QCOMPARE(model.messagesNames(), QStringList({"Daemon", "Quiet", "Standard"}));
}

QTEST_APPLESS_MAIN(BScheduleDragPlanTest)
#include "bscheduledragplan_test.moc"
