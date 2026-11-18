/**
 * @file bscheduledragplan_test.cpp
 * @brief Generated against original Run lines after a drag in the schedule view
 *
 * The drag dialog regenerates the Run lines of a schedule through
 * BScheduleEntry, which does not keep every form of a Run line. These tests
 * pin down when the result may be written, when it needs a confirmation and
 * when it is refused. Also covers the Messages names offered by the job
 * wizard. Runs without a Director (registered with CTest).
 */

#include <QtTest>

#include "models/bresourcemodels.h"
#include "schedules/bscheduledragplan.h"

using BScheduleDragPlan::Plan;
using BScheduleDragPlan::Reason;
using BScheduleDragPlan::Verdict;

Q_DECLARE_METATYPE(BScheduleDragPlan::Reason)

namespace {

/// The dragged entry as the Gantt view has it: parsed from its Run line
BScheduleEntry entryOf(const QString &run)
{
    BScheduleEntry entry = BScheduleEntry::fromRunDirective(run);
    entry.scheduleName = "WeeklyCycle";
    return entry;
}

QString roundTrip(const QString &run)
{
    return BScheduleEntry::fromRunDirective(run).toRunDirective();
}

} // namespace

class BScheduleDragPlanTest : public QObject
{
    Q_OBJECT

private slots:
    void plainScheduleChangesOnlyTheDraggedTime();
    void spellingOfTheDirectorIsNotADifference();
    void oneLinePerRunLineNotPerJob();
    void weekOfMonthLinesThatSurviveAreAccepted();
    void lossyFormsAreRefused_data();
    void lossyFormsAreRefused();
    void overridesAreAlwaysRefused();
    void otherDifferencesNeedConfirmation_data();
    void otherDifferencesNeedConfirmation();
    void undeterminableCasesAreRefused();
    void sameRun_data();
    void sameRun();
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
    QCOMPARE(plan.generatedRuns,
             QStringList({"Full sat at 21:00", "Incremental mon-fri at 22:30"}));
}

void BScheduleDragPlanTest::spellingOfTheDirectorIsNotADifference()
{
    // Capitals, level=, a one-digit hour and a day list in another order
    const QStringList runs = {"Level=Full Sat at 2:05", "Incremental Fri,Mon at 23:05",
                              "Differential daily at 4:00", "VirtualFull at 5:00"};
    const Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 3, 15);

    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.draggedLine, 0);
    QCOMPARE(plan.generatedRuns,
             QStringList({"Full sat at 03:15", "Incremental mon,fri at 23:05",
                          "Differential daily at 04:00", "VirtualFull daily at 05:00"}));
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
    QCOMPARE(plan.generatedRuns[1], QString("Incremental mon-fri at 22:00"));
}

void BScheduleDragPlanTest::weekOfMonthLinesThatSurviveAreAccepted()
{
    // The default schedule of a Bareos installation. One week-of-month token
    // (also a range) in front of the day comes back unchanged, so nothing
    // but the dragged time differs and there is no reason to refuse.
    const QStringList runs = {"Full 1st sat at 21:00", "Differential 2nd-5th sat at 21:00",
                              "Incremental mon-fri at 21:00"};
    for (const QString &run : runs)
        QCOMPARE(roundTrip(run), run);

    const Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 20, 0);
    QCOMPARE(plan.verdict, Verdict::Exact);
    QCOMPARE(plan.draggedLine, 0);
    QCOMPARE(plan.generatedRuns,
             QStringList({"Full 1st sat at 20:00", "Differential 2nd-5th sat at 21:00",
                          "Incremental mon-fri at 21:00"}));

    // Full and Differential differ in level and week only: the dragged line
    // is still found
    const Plan second = BScheduleDragPlan::plan(runs, entryOf(runs[1]), 22, 0);
    QCOMPARE(second.verdict, Verdict::Exact);
    QCOMPARE(second.draggedLine, 1);
    QCOMPARE(second.generatedRuns[1], QString("Differential 2nd-5th sat at 22:00"));
}

void BScheduleDragPlanTest::lossyFormsAreRefused_data()
{
    QTest::addColumn<QString>("run");
    QTest::addColumn<Reason>("reason");
    QTest::addColumn<QString>("token");

    QTest::newRow("day range") << "Incremental tue-thu at 23:05" << Reason::DayRange << "tue-thu";
    QTest::newRow("day range, capitals") << "Incremental Tue-Thu at 23:05" << Reason::DayRange << "Tue-Thu";
    QTest::newRow("two week-of-month") << "Full 1st 3rd sat at 21:00" << Reason::WeekOfMonth << "1st";
    QTest::newRow("week-of-month word") << "Full first sat at 21:00" << Reason::WeekOfMonth << "first";
    QTest::newRow("day of month") << "Full on 1 at 2:00" << Reason::DayOfMonth << "1";
    QTest::newRow("day of month range") << "Full 1-7 at 2:00" << Reason::DayOfMonth << "1-7";
}

void BScheduleDragPlanTest::lossyFormsAreRefused()
{
    QFETCH(QString, run);
    QFETCH(Reason, reason);
    QFETCH(QString, token);

    // The lossy line is not the dragged one: the whole schedule is regenerated
    const QStringList runs = {"Incremental mon at 20:00", run};
    // The premise: the regenerated line does not say what the original says
    QVERIFY(!BScheduleDragPlan::sameRun(run, roundTrip(run)));
    const Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 19, 0);

    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, reason);
    QCOMPARE(plan.reasonLine, 1);
    QCOMPARE(plan.reasonToken, token);
    QVERIFY(plan.generatedRuns.isEmpty());
}

void BScheduleDragPlanTest::overridesAreAlwaysRefused()
{
    // pool=Monthly: the parser reads "monthly" out of the value and makes the
    // line run every day
    const QStringList runs = {"Incremental mon at 20:00", "Full 1st sat at 21:00 pool=Monthly"};
    QCOMPARE(roundTrip(runs[1]), QString("Full 1st daily at 21:00 pool=Monthly"));
    Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 19, 0);
    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, Reason::Override);
    QCOMPARE(plan.reasonLine, 1);
    QCOMPARE(plan.reasonToken, QString("pool=Monthly"));
    QVERIFY(plan.generatedRuns.isEmpty());

    // Refused even where the tokens would survive: the override would move
    // behind the time
    const QStringList harmless = {"Storage=File Full sat at 21:00"};
    plan = BScheduleDragPlan::plan(harmless, entryOf(harmless[0]), 19, 0);
    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, Reason::Override);
    QCOMPARE(plan.reasonLine, 0);
    QCOMPARE(plan.reasonToken, QString("Storage=File"));

    // level= is the level, not an override
    const QStringList level = {"level=Full sat at 21:00"};
    plan = BScheduleDragPlan::plan(level, entryOf(level[0]), 19, 0);
    QCOMPARE(plan.verdict, Verdict::Exact);
}

void BScheduleDragPlanTest::otherDifferencesNeedConfirmation_data()
{
    QTest::addColumn<QString>("run");
    QTest::addColumn<QString>("generated");

    // Unknown levels become Incremental
    QTest::newRow("unknown level") << "Base sun at 1:00" << "Incremental sun at 01:00";
    // So does a line without a level, which leaves the level to the job
    QTest::newRow("no level") << "sun at 1:00" << "Incremental sun at 01:00";
    QTest::newRow("hourly") << "Incremental hourly at 0:15" << "Incremental daily at 00:15";
}

void BScheduleDragPlanTest::otherDifferencesNeedConfirmation()
{
    QFETCH(QString, run);
    QFETCH(QString, generated);

    const QStringList runs = {"Full sat at 20:00", run};
    const Plan plan = BScheduleDragPlan::plan(runs, entryOf(runs[0]), 19, 0);

    QCOMPARE(plan.verdict, Verdict::NeedsConfirmation);
    QCOMPARE(plan.reason, Reason::None);
    QCOMPARE(plan.draggedLine, 0);
    QCOMPARE(plan.differingLines, QList<int>({1}));
    // Both versions are there to be shown side by side
    QCOMPARE(plan.originalRuns, runs);
    QCOMPARE(plan.generatedRuns, QStringList({"Full sat at 19:00", generated}));
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

    // Two lines match it
    const QStringList twice = {"Full sat at 21:00", "Full sat at 21:00"};
    plan = BScheduleDragPlan::plan(twice, entryOf(twice[0]), 20, 0);
    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, Reason::DraggedLineAmbiguous);
    QCOMPARE(plan.reasonLine, 1);

    // A line without a readable time
    const QStringList noTime = {"Full sat at 21:00", "Incremental mon-fri"};
    plan = BScheduleDragPlan::plan(noTime, entryOf(noTime[0]), 20, 0);
    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, Reason::NoTime);
    QCOMPARE(plan.reasonLine, 1);

    const QStringList oddTime = {"Full sat at 21:00", "Incremental mon-fri at 9pm"};
    plan = BScheduleDragPlan::plan(oddTime, entryOf(oddTime[0]), 20, 0);
    QCOMPARE(plan.verdict, Verdict::Refused);
    QCOMPARE(plan.reason, Reason::NoTime);
    QVERIFY(plan.generatedRuns.isEmpty());
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

    QTest::newRow("time") << "Full sat at 21:00" << "Full sat at 21:15" << false;
    QTest::newRow("level") << "Full sat at 21:00" << "Incremental sat at 21:00" << false;
    QTest::newRow("no level") << "sat at 21:00" << "Incremental sat at 21:00" << false;
    QTest::newRow("range lost its middle") << "Full tue-thu at 2:05" << "Full tue,thu at 02:05" << false;
    QTest::newRow("week-of-month lost") << "Full 1st sat at 21:00" << "Full sat at 21:00" << false;
    QTest::newRow("week-of-month changed") << "Full 2nd-5th sat at 21:00" << "Full 2nd sat at 21:00" << false;
    QTest::newRow("day of month lost") << "Full on 1 at 2:00" << "Full daily at 02:00" << false;
    QTest::newRow("pool lost") << "Full sat at 21:00 pool=Monthly" << "Full sat at 21:00" << false;
    QTest::newRow("pool made daily") << "Full sat at 21:00 pool=Monthly" << "Full daily at 21:00 pool=Monthly" << false;
    QTest::newRow("no time") << "Full sat" << "Full sat" << false;
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
