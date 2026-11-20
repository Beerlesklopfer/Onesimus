/**
 * @file bstaggerplan_test.cpp
 * @brief The schedules and job texts of a stagger proposal (BStaggerPlan)
 *
 * Runs without a Director and without widgets (registered with CTest).
 */

#include <QtTest>

#include "schedules/bstaggerplan.h"

namespace {

BSchedule schedule(const QString &name, const QStringList &runs)
{
    BSchedule s;
    s.name = name;
    for (const QString &run : runs) s.runs.append(BRunParser::parse(run));
    return s;
}

} // namespace

class BStaggerPlanTest : public QObject
{
    Q_OBJECT

private slots:
    void linesMoveLaterKeepingTheirDays();
    void linesThatCannotMove();
    void namesCarryTheNewTime();
    void scheduleTextReadsBack();
    void jobScheduleReplaced();
    void jobScheduleInserted();
    void jobScheduleValueRead();
};

void BStaggerPlanTest::linesMoveLaterKeepingTheirDays()
{
    const BSchedule weekly = schedule("WeeklyCycle", {"Full 1st Sun at 23:05",
                                                      "Differential 2nd-5th Sun at 23:05",
                                                      "Incremental Mon-Sat at 23:05"});
    QStringList lines;
    QString error;
    QVERIFY2(BStaggerPlan::shiftedRunLines(weekly, 3, &lines, &error), qPrintable(error));
    QCOMPARE(lines.size(), 3);
    for (int i = 0; i < 3; ++i) {
        const BRunLine back = BRunParser::parse(lines[i]);
        QVERIFY2(back.spec, qPrintable(lines[i]));
        QCOMPARE(back.spec->hours(), QList<int>{23});
        QCOMPARE(back.spec->minute, 8);
        // Days, weeks and level as before
        QCOMPARE(back.spec->mask.wday, weekly.runs[i].spec->mask.wday);
        QCOMPARE(back.spec->mask.wom, weekly.runs[i].spec->mask.wom);
        QCOMPARE(back.spec->overrides, weekly.runs[i].spec->overrides);
    }
    // Across the hour
    QVERIFY(BStaggerPlan::shiftedRunLines(schedule("S", {"daily at 07:58"}), 5, &lines));
    QCOMPARE(BRunParser::parse(lines[0]).spec->hours(), QList<int>{8});
    QCOMPARE(BRunParser::parse(lines[0]).spec->minute, 3);
    QCOMPARE(BStaggerPlan::firstStart(weekly, 3), QTime(23, 8));
}

void BStaggerPlanTest::linesThatCannotMove()
{
    QString error;
    QVERIFY(!BStaggerPlan::shiftedRunLines(schedule("S", {"daily at 23:58"}), 5, nullptr, &error));
    QVERIFY(error.contains("next day"));
    QVERIFY(!BStaggerPlan::shiftedRunLines(schedule("S", {"hourly at 0:15"}), 5, nullptr, &error));
    QVERIFY(error.contains("several hours"));
    QVERIFY(!BStaggerPlan::shiftedRunLines(schedule("S", {}), 5, nullptr, &error));
}

void BStaggerPlanTest::namesCarryTheNewTime()
{
    QCOMPARE(BStaggerPlan::scheduleName("WeeklyCycle", QTime(23, 6), {"WeeklyCycle"}),
             QString("WeeklyCycle-2306"));
    QCOMPARE(BStaggerPlan::scheduleName("WeeklyCycle", QTime(23, 6),
                                        {"WeeklyCycle", "weeklycycle-2306", "WeeklyCycle-2306-2"}),
             QString("WeeklyCycle-2306-3"));
}

void BStaggerPlanTest::scheduleTextReadsBack()
{
    const QString text = BStaggerPlan::scheduleText("WeeklyCycle-2306", "Copy of \"WeeklyCycle\"",
                                                    {"Full 1st sun at 23:06", "Incremental mon-sat at 23:06"});
    QCOMPARE(text, QString("Schedule {\n"
                           "  Name = \"WeeklyCycle-2306\"\n"
                           "  Description = \"Copy of \\\"WeeklyCycle\\\"\"\n"
                           "  Run = Full 1st sun at 23:06\n"
                           "  Run = Incremental mon-sat at 23:06\n"
                           "}\n"));
}

void BStaggerPlanTest::jobScheduleReplaced()
{
    const QString job = "Job {\n  Name = \"forgejo\"\n  JobDefs = \"DefaultJob\"\n"
                        "    sche dule = WeeklyCycle  # nightly\n  Client = ForgeIEC-fd\n}\n";
    QCOMPARE(BStaggerPlan::jobWithSchedule(job, "WeeklyCycle-2306"),
             QString("Job {\n  Name = \"forgejo\"\n  JobDefs = \"DefaultJob\"\n"
                     "    Schedule = \"WeeklyCycle-2306\"\n  Client = ForgeIEC-fd\n}\n"));
    // A RunScript's or Run directive's text is not the Schedule directive
    const QString other = "Job {\n  Name = a\n  Run = \"b level=Full\"\n}\n";
    QCOMPARE(BStaggerPlan::jobWithSchedule(other, "X"),
             QString("Job {\n  Name = a\n  Run = \"b level=Full\"\n  Schedule = \"X\"\n}\n"));
}

void BStaggerPlanTest::jobScheduleInserted()
{
    QCOMPARE(BStaggerPlan::jobWithSchedule("Job {\r\n\tName = a\r\n}\r\n", "X"),
             QString("Job {\r\n\tName = a\r\n\tSchedule = \"X\"\r\n}\r\n"));
    QCOMPARE(BStaggerPlan::jobWithSchedule("Job { Name = a }", "X"),
             QString("Job { Name = a\n  Schedule = \"X\"\n }"));   // the blank before the brace stays
    QCOMPARE(BStaggerPlan::jobWithSchedule("Job { Name = a", "X"), QString());
}

void BStaggerPlanTest::jobScheduleValueRead()
{
    // Quoted, with escapes; unquoted, up to a comment or a semicolon; none
    QCOMPARE(BStaggerPlan::scheduleValue("Job {\n  Name = a\n  Schedule = \"Weekly \\\"A\\\"\"  # x\n}\n"),
             QString("Weekly \"A\""));
    QCOMPARE(BStaggerPlan::scheduleValue("Job {\n  schedule=WeeklyCycle# x\n}\n"), QString("WeeklyCycle"));
    QCOMPARE(BStaggerPlan::scheduleValue("Job {\r\n  Sche dule = Nightly;\r\n}\r\n"), QString("Nightly"));
    QCOMPARE(BStaggerPlan::scheduleValue("Job {\n  Name = a\n  JobDefs = D\n}\n"), QString());
}

QTEST_GUILESS_MAIN(BStaggerPlanTest)
#include "bstaggerplan_test.moc"
