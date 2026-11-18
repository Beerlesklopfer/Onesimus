/**
 * @file bjobhistorystats_test.cpp
 * @brief Measured durations and waits of past job runs
 *
 * Two parts: reading the reply of "llist jobs" (BJobHistoryReply) and the
 * statistics per job and level (BJobHistoryStats).
 *
 * Where the reply samples come from. None of them was captured from a
 * running Director; each is written out from source code and marked:
 *
 * - [bareos-source] The form Bareos produces according to its sources,
 *   branch bareos-25 at commit 0e7742c: the columns of query
 *   0007_list_jobs_long (core/src/cats/postgresql_queries.inc), every value
 *   a JSON string under the column name in lower case
 *   (core/src/cats/sql.cc ListResult, core/src/lib/output_formatter.cc
 *   JsonKeyValueAdd), in an array "jobs" (core/src/cats/sql_list.cc
 *   ListJobRecords) inside the JSON-RPC envelope with "meta.range" for a
 *   limit (output_formatter.cc JsonFinalizeResult). Times as PostgreSQL
 *   prints a timestamp without zone that was written by bstrutime()
 *   ("%Y-%m-%d %H:%M:%S", core/src/lib/btime.cc). The Onesimus Director
 *   builds the same reply (crates/odir/src/commands/list.rs LIST_JOBS_LONG,
 *   crates/odir/src/sql_list.rs json_rows).
 *   ASSUMED, not confirmed against a Director: the exact text of the
 *   "duration" column (an interval; not used by the code under test) and
 *   that a NULL column arrives as an empty string (stated for the Onesimus
 *   Director in sql_list.rs, "NULL becomes "" (C++ semantics)").
 * - [bareos-source, error] The error envelope of JsonFinalizeResult with the
 *   message of ua_cmds.cc Do_a_command() for a command the console ACL does
 *   not allow or the Director does not know ("%s: is an invalid
 *   command."). ASSUMED: the exact content of "data".
 * - [constructed] Not the form of any known Director: written to check
 *   that a missing value is treated as missing.
 *
 * Runs without a Director and without widgets (registered with CTest).
 */

#include <QtTest>

#include <QTimeZone>

#include "schedules/model/bjobhistorystats.h"

namespace {

QDateTime at(const QString &text)
{
    const QDateTime time = BJobHistoryReply::parseTime(text);
    Q_ASSERT(time.isValid());
    return time;
}

/// A run of @p seconds that started @p waitSeconds after its scheduled time
BJobRun run(const QString &name, char level, char status, const QString &scheduled,
            qint64 waitSeconds, qint64 seconds)
{
    static quint64 nextId = 1;
    BJobRun r;
    r.jobId = nextId++;
    r.name = name;
    r.type = QLatin1Char('B');
    r.level = QLatin1Char(level);
    r.status = QLatin1Char(status);
    r.scheduled = at(scheduled);
    r.started = r.scheduled.addSecs(waitSeconds);
    r.ended = r.started.addSecs(seconds);
    r.reallyEnded = r.ended;
    return r;
}

const QDateTime now = QDateTime(QDate(2026, 11, 19), QTime(12, 0), QTimeZone::UTC);

// [bareos-source] Two rows of "llist jobs days=60 reverse limit=5000",
// newest first: an Incremental that waited 12 s and ran 3 min 20 s, and a
// Full that waited 30 min and ran 2 h 10 min 5 s and ended with warnings.
const char *const bareosReply = R"JSON({
  "jsonrpc": "2.0",
  "id": null,
  "result": {
    "jobs": [
      {
        "jobid": "4712",
        "job": "backup-pdc.2026-11-18_02.05.00_07",
        "name": "backup-pdc",
        "purgedfiles": "0",
        "type": "B",
        "level": "I",
        "clientid": "3",
        "client": "pdc-fd",
        "jobstatus": "T",
        "schedtime": "2026-11-18 02:05:00",
        "starttime": "2026-11-18 02:05:12",
        "endtime": "2026-11-18 02:08:32",
        "realendtime": "2026-11-18 02:08:32",
        "duration": "00:03:20",
        "jobtdate": "1795482512",
        "volsessionid": "41",
        "volsessiontime": "1794000000",
        "jobfiles": "1204",
        "jobbytes": "88123456",
        "joberrors": "0",
        "jobmissingfiles": "0",
        "poolid": "2",
        "poolname": "Incremental",
        "priorjobid": "0",
        "filesetid": "1",
        "fileset": "LinuxAll"
      },
      {
        "jobid": "4698",
        "job": "backup-pdc.2026-11-07_21.00.00_55",
        "name": "backup-pdc",
        "purgedfiles": "0",
        "type": "B",
        "level": "F",
        "clientid": "3",
        "client": "pdc-fd",
        "jobstatus": "W",
        "schedtime": "2026-11-07 21:00:00",
        "starttime": "2026-11-07 21:30:00",
        "endtime": "2026-11-07 23:40:05",
        "realendtime": "2026-11-07 23:40:05",
        "duration": "02:10:05",
        "jobtdate": "1794601805",
        "volsessionid": "12",
        "volsessiontime": "1794000000",
        "jobfiles": "310224",
        "jobbytes": "91234567890",
        "joberrors": "2",
        "jobmissingfiles": "0",
        "poolid": "1",
        "poolname": "Full",
        "priorjobid": "0",
        "filesetid": "1",
        "fileset": "LinuxAll"
      }
    ],
    "meta": {
      "range": {
        "limit": 5000,
        "filtered": 0
      }
    }
  }
})JSON";

// [bareos-source, error] "llist" is not in the console's Command ACL, or
// the Director does not know the command
const char *const refusedReply = R"JSON({"jsonrpc":"2.0","id":null,"error":{"code":1,
"message":"failed","data":{"result":{},"messages":{"error":["llist: is an invalid command.\n"]}}}})JSON";

// [constructed] A Director whose rows have no "schedtime" key at all
const char *const noSchedTimeKeyReply = R"JSON({"jsonrpc":"2.0","id":null,"result":{"jobs":[
{"jobid":"10","name":"nightly","type":"B","level":"I","jobstatus":"T",
 "starttime":"2026-11-18 01:00:10","endtime":"2026-11-18 01:10:10","realendtime":"2026-11-18 01:10:10"},
{"jobid":"11","name":"nightly","type":"B","level":"I","jobstatus":"T",
 "starttime":"2026-11-17 01:00:20","endtime":"2026-11-17 01:20:20","realendtime":"2026-11-17 01:20:20"}
]}})JSON";

// [bareos-source] with [constructed] values: one row with an empty
// scheduled time (NULL in the catalog, ASSUMED to arrive as ""), one still
// running (no end time), one with the zero date of old catalogs, and one
// with a number where Bareos sends a string
const char *const mixedReply = R"JSON({"jsonrpc":"2.0","id":null,"result":{"jobs":[
{"jobid":"20","name":"mail","type":"B","level":"F","jobstatus":"T",
 "schedtime":"","starttime":"2026-11-18 03:00:00","endtime":"2026-11-18 04:00:00","realendtime":"2026-11-18 04:00:00"},
{"jobid":"21","name":"mail","type":"B","level":"I","jobstatus":"R",
 "schedtime":"2026-11-19 03:00:00","starttime":"2026-11-19 03:00:05","endtime":"","realendtime":""},
{"jobid":"22","name":"mail","type":"B","level":"I","jobstatus":"T",
 "schedtime":"0000-00-00 00:00:00","starttime":"2026-11-17 03:00:05","endtime":"2026-11-17 03:05:05","realendtime":"2026-11-17 03:05:05"},
{"jobid":23,"name":"mail","type":"B","level":"I","jobstatus":"T",
 "schedtime":"2026-11-16 03:00:00","starttime":"2026-11-16 03:00:07.123456","endtime":"2026-11-16 03:04:07","realendtime":"2026-11-16 03:04:07"},
{"jobid":"24","name":"","type":"B","level":"I","jobstatus":"T"}
]}})JSON";

} // namespace

class BJobHistoryStatsTest : public QObject
{
    Q_OBJECT

private slots:
    // --- Reading the reply ---
    void parsesTimes_data();
    void parsesTimes();
    void readsBareosReply();
    void refusedCommandIsNotAnEmptyHistory();
    void textReplyIsUnreadable();
    void emptyRepliesAreEmptyHistories();
    void replyWithoutScheduledTimeHasUnknownWaits();
    void missingValuesStayMissing();

    // --- Single runs ---
    void waitIsStartMinusScheduled();
    void waitWithoutScheduledTimeIsUnknown();
    void startBeforeScheduledTimeIsForeign();
    void durationNeedsStartAndEnd();
    void onlyTerminatedAndWarningsCount();
    void levelNames();

    // --- Quantiles ---
    void quantiles_data();
    void quantiles();
    void quantilesOfNothing();

    // --- Statistics ---
    void groupsPerJobAndLevel();
    void failedAndCancelledRunsAreLeftOut();
    void runsOutsideTheWindowAreLeftOut();
    void missingScheduledTimeCountsForDurationOnly();
    void startBeforeScheduledTimeIsLeftOut();
    void jobWithoutHistory();
    void levelFallbackIsMarkedAsEstimate();
    void emptyLevelAsksForTheJob();
    void lastRunsNewestFirst();
    void statisticsFromBareosReply();
    void clearLeavesNoHistory();

    // --- Text ---
    void formatsDurations();
};

// ============================================================================
// Reading the reply
// ============================================================================

void BJobHistoryStatsTest::parsesTimes_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("iso");

    QTest::newRow("bstrutime") << "2026-11-18 02:05:12" << true << "2026-11-18T02:05:12";
    QTest::newRow("fractions") << "2026-11-18 02:05:12.250000" << true << "2026-11-18T02:05:12";
    QTest::newRow("spaces") << "  2026-11-18 02:05:12 " << true << "2026-11-18T02:05:12";
    QTest::newRow("empty") << "" << false << "";
    QTest::newRow("zero") << "0" << false << "";
    QTest::newRow("zero date") << "0000-00-00 00:00:00" << false << "";
    QTest::newRow("epoch") << "1970-01-01 01:00:00" << false << "";
    QTest::newRow("date only") << "2026-11-18" << false << "";
    QTest::newRow("words") << "yesterday" << false << "";
    QTest::newRow("no such day") << "2026-02-30 10:00:00" << false << "";
}

void BJobHistoryStatsTest::parsesTimes()
{
    QFETCH(QString, text);
    QFETCH(bool, valid);
    QFETCH(QString, iso);

    const QDateTime time = BJobHistoryReply::parseTime(text);
    QCOMPARE(time.isValid(), valid);
    if (valid) {
        // Wall-clock time, no conversion
        QCOMPARE(time.toString("yyyy-MM-ddTHH:mm:ss"), iso);
        QCOMPARE(time.offsetFromUtc(), 0);
    }
}

void BJobHistoryStatsTest::readsBareosReply()
{
    const BJobHistoryReply reply = BJobHistoryReply::parse(QString::fromUtf8(bareosReply));

    QCOMPARE(reply.outcome, BJobHistoryReply::Outcome::Runs);
    QVERIFY(reply.error.isEmpty());
    QCOMPARE(reply.rows, 2);
    QCOMPARE(reply.rowsWithScheduledTime, 2);
    QCOMPARE(reply.runs.size(), 2);

    const BJobRun &inc = reply.runs.at(0);
    QCOMPARE(inc.jobId, quint64(4712));
    QCOMPARE(inc.name, QString("backup-pdc"));      // "name", not the unique "job"
    QCOMPARE(inc.type, QChar('B'));
    QCOMPARE(inc.level, QChar('I'));
    QCOMPARE(inc.status, QChar('T'));
    QCOMPARE(inc.scheduled, at("2026-11-18 02:05:00"));
    QCOMPARE(inc.started, at("2026-11-18 02:05:12"));
    QCOMPARE(inc.ended, at("2026-11-18 02:08:32"));
    QCOMPARE(inc.reallyEnded, at("2026-11-18 02:08:32"));
    QCOMPARE(BJobHistoryStats::waitSecs(inc), std::optional<qint64>(12));
    QCOMPARE(BJobHistoryStats::durationSecs(inc), std::optional<qint64>(200));

    const BJobRun &full = reply.runs.at(1);
    QCOMPARE(full.level, QChar('F'));
    QCOMPARE(full.status, QChar('W'));
    QCOMPARE(BJobHistoryStats::waitSecs(full), std::optional<qint64>(1800));
    QCOMPARE(BJobHistoryStats::durationSecs(full), std::optional<qint64>(7805));
}

void BJobHistoryStatsTest::refusedCommandIsNotAnEmptyHistory()
{
    const BJobHistoryReply reply = BJobHistoryReply::parse(QString::fromUtf8(refusedReply));

    QCOMPARE(reply.outcome, BJobHistoryReply::Outcome::Refused);
    QCOMPARE(reply.error, QString("llist: is an invalid command."));
    QVERIFY(reply.runs.isEmpty());

    // An error without messages still says something
    const BJobHistoryReply bare = BJobHistoryReply::parse(
        R"({"jsonrpc":"2.0","id":null,"error":{"code":1,"message":"Command failed"}})");
    QCOMPARE(bare.outcome, BJobHistoryReply::Outcome::Refused);
    QCOMPARE(bare.error, QString("Command failed"));
}

void BJobHistoryStatsTest::textReplyIsUnreadable()
{
    // A Director that is not in API mode answers in text
    const BJobHistoryReply text = BJobHistoryReply::parse("llist: is an invalid command.\n");
    QCOMPARE(text.outcome, BJobHistoryReply::Outcome::Unreadable);
    QCOMPARE(text.error, QString("llist: is an invalid command."));

    const BJobHistoryReply empty = BJobHistoryReply::parse(QString());
    QCOMPARE(empty.outcome, BJobHistoryReply::Outcome::Unreadable);
    QVERIFY(!empty.error.isEmpty());

    // JSON, but not a list of jobs (the reply of another command)
    const BJobHistoryReply other = BJobHistoryReply::parse(
        R"({"jsonrpc":"2.0","id":null,"result":{"clients":[{"name":"pdc-fd"}]}})");
    QCOMPARE(other.outcome, BJobHistoryReply::Outcome::Unreadable);
    QVERIFY(other.runs.isEmpty());
}

void BJobHistoryStatsTest::emptyRepliesAreEmptyHistories()
{
    // [bareos-source] No job in the window: an empty array
    const BJobHistoryReply none = BJobHistoryReply::parse(
        R"({"jsonrpc":"2.0","id":null,"result":{"jobs":[],"meta":{"range":{"limit":5000,"filtered":0}}}})");
    QCOMPARE(none.outcome, BJobHistoryReply::Outcome::Runs);
    QCOMPARE(none.rows, 0);
    QVERIFY(none.runs.isEmpty());

    // [bareos-source] ua_output.cc sends an empty object "jobs" for a job id
    // that does not exist; not expected for this command, read all the same
    const BJobHistoryReply object = BJobHistoryReply::parse(
        R"({"jsonrpc":"2.0","id":null,"result":{"jobs":{}}})");
    QCOMPARE(object.outcome, BJobHistoryReply::Outcome::Runs);
    QVERIFY(object.runs.isEmpty());
}

void BJobHistoryStatsTest::replyWithoutScheduledTimeHasUnknownWaits()
{
    const BJobHistoryReply reply = BJobHistoryReply::parse(QString::fromUtf8(noSchedTimeKeyReply));

    QCOMPARE(reply.outcome, BJobHistoryReply::Outcome::Runs);
    QCOMPARE(reply.rows, 2);
    QCOMPARE(reply.rowsWithScheduledTime, 0);   // the view says so once
    for (const BJobRun &r : reply.runs) {
        QVERIFY(!r.scheduled.isValid());
        QVERIFY(!BJobHistoryStats::waitSecs(r).has_value());
    }

    BJobHistoryStats stats;
    stats.setRuns(reply.runs, 60, now);
    const QList<BJobHistoryStats::Group> groups = stats.groups();
    QCOMPARE(groups.size(), 1);
    QCOMPARE(groups.at(0).runs, 2);
    QCOMPARE(groups.at(0).duration.median, qint64(900));    // 600 s and 1200 s
    QVERIFY(!groups.at(0).wait.isValid());                  // unknown, not zero
    QCOMPARE(groups.at(0).wait.count, 0);
    QCOMPARE(groups.at(0).waitUnknown, 2);
    QCOMPARE(stats.summary().waitUnknown, 2);
}

void BJobHistoryStatsTest::missingValuesStayMissing()
{
    const BJobHistoryReply reply = BJobHistoryReply::parse(QString::fromUtf8(mixedReply));

    QCOMPARE(reply.outcome, BJobHistoryReply::Outcome::Runs);
    QCOMPARE(reply.rows, 5);
    QCOMPARE(reply.runs.size(), 4);             // the row without a name is dropped
    QCOMPARE(reply.rowsWithScheduledTime, 2);   // 21 and 23

    // 20: empty scheduled time
    QVERIFY(!reply.runs.at(0).scheduled.isValid());
    QVERIFY(!BJobHistoryStats::waitSecs(reply.runs.at(0)).has_value());
    QCOMPARE(BJobHistoryStats::durationSecs(reply.runs.at(0)), std::optional<qint64>(3600));

    // 21: still running, no end
    QVERIFY(!reply.runs.at(1).ended.isValid());
    QVERIFY(!BJobHistoryStats::durationSecs(reply.runs.at(1)).has_value());

    // 22: zero date is no scheduled time
    QVERIFY(!reply.runs.at(2).scheduled.isValid());

    // 23: a number as job id, fractions of a second in the time
    QCOMPARE(reply.runs.at(3).jobId, quint64(23));
    QCOMPARE(BJobHistoryStats::waitSecs(reply.runs.at(3)), std::optional<qint64>(7));

    BJobHistoryStats stats;
    stats.setRuns(reply.runs, 60, now);
    QCOMPARE(stats.summary().notSuccessful, 1);     // 21 is running
    QCOMPARE(stats.summary().counted, 3);
    QCOMPARE(stats.summary().waitUnknown, 2);

    // Incremental: 22 (wait unknown) and 23 (7 s): the wait is from one run
    const BJobHistoryStats::Estimate inc = stats.estimate("mail", "Incremental");
    QCOMPARE(inc.runs, 2);
    QCOMPARE(inc.wait.count, 1);
    QCOMPARE(inc.wait.median, qint64(7));
    QCOMPARE(inc.waitUnknown, 1);
}

// ============================================================================
// Single runs
// ============================================================================

void BJobHistoryStatsTest::waitIsStartMinusScheduled()
{
    QCOMPARE(BJobHistoryStats::waitSecs(run("j", 'F', 'T', "2026-11-18 21:00:00", 0, 60)),
             std::optional<qint64>(0));     // started on time: a measured zero
    QCOMPARE(BJobHistoryStats::waitSecs(run("j", 'F', 'T', "2026-11-18 21:00:00", 5400, 60)),
             std::optional<qint64>(5400));
    // Across midnight
    QCOMPARE(BJobHistoryStats::waitSecs(run("j", 'F', 'T', "2026-11-18 23:59:30", 60, 60)),
             std::optional<qint64>(60));
}

void BJobHistoryStatsTest::waitWithoutScheduledTimeIsUnknown()
{
    BJobRun r = run("j", 'F', 'T', "2026-11-18 21:00:00", 30, 60);
    r.scheduled = QDateTime();
    QVERIFY(!BJobHistoryStats::waitSecs(r).has_value());

    BJobRun noStart = run("j", 'F', 'T', "2026-11-18 21:00:00", 30, 60);
    noStart.started = QDateTime();
    QVERIFY(!BJobHistoryStats::waitSecs(noStart).has_value());
}

void BJobHistoryStatsTest::startBeforeScheduledTimeIsForeign()
{
    // A virtual full scheduled for 21:00 whose record carries the start of
    // the incremental it consolidated, from the night before
    BJobRun r = run("j", 'f', 'T', "2026-11-18 21:00:00", 0, 600);
    r.started = at("2026-11-17 22:00:00");
    r.ended = at("2026-11-17 22:10:00");
    r.reallyEnded = at("2026-11-18 21:40:00");

    QVERIFY(BJobHistoryStats::hasForeignTimes(r));
    QVERIFY(!BJobHistoryStats::waitSecs(r).has_value());    // never negative, never zero

    QVERIFY(!BJobHistoryStats::hasForeignTimes(run("j", 'F', 'T', "2026-11-18 21:00:00", 0, 60)));

    // Without a scheduled time there is nothing to compare with
    BJobRun unknown = r;
    unknown.scheduled = QDateTime();
    QVERIFY(!BJobHistoryStats::hasForeignTimes(unknown));
}

void BJobHistoryStatsTest::durationNeedsStartAndEnd()
{
    BJobRun r = run("j", 'F', 'T', "2026-11-18 21:00:00", 0, 3725);
    QCOMPARE(BJobHistoryStats::durationSecs(r), std::optional<qint64>(3725));

    // Only the real end time delivered
    BJobRun onlyReal = r;
    onlyReal.ended = QDateTime();
    QCOMPARE(BJobHistoryStats::durationSecs(onlyReal), std::optional<qint64>(3725));

    BJobRun noEnd = r;
    noEnd.ended = QDateTime();
    noEnd.reallyEnded = QDateTime();
    QVERIFY(!BJobHistoryStats::durationSecs(noEnd).has_value());

    // End before start (a run across the end of daylight saving time)
    BJobRun backwards = r;
    backwards.ended = r.started.addSecs(-10);
    QVERIFY(!BJobHistoryStats::durationSecs(backwards).has_value());
}

void BJobHistoryStatsTest::onlyTerminatedAndWarningsCount()
{
    // core/src/include/job_status.h
    QVERIFY(BJobHistoryStats::terminatedSuccessfully('T'));
    QVERIFY(BJobHistoryStats::terminatedSuccessfully('W'));

    const QString others = QStringLiteral("ABCDEFILMRSacdefijlmpqst");
    for (const QChar status : others) {
        QVERIFY2(!BJobHistoryStats::terminatedSuccessfully(status),
                 qPrintable(QString(status)));
    }
    QVERIFY(!BJobHistoryStats::terminatedSuccessfully(QChar()));
}

void BJobHistoryStatsTest::levelNames()
{
    // core/src/include/job_level.h
    QCOMPARE(BJobHistoryStats::levelName('F'), QString("Full"));
    QCOMPARE(BJobHistoryStats::levelName('I'), QString("Incremental"));
    QCOMPARE(BJobHistoryStats::levelName('D'), QString("Differential"));
    QCOMPARE(BJobHistoryStats::levelName('f'), QString("VirtualFull"));
    QCOMPARE(BJobHistoryStats::levelName('B'), QString("Base"));
    QCOMPARE(BJobHistoryStats::levelName('d'), QString("DiskToCatalog"));
    QCOMPARE(BJobHistoryStats::levelName(' '), QString());   // restore, admin
    QCOMPARE(BJobHistoryStats::levelName(QChar()), QString());
    QCOMPARE(BJobHistoryStats::levelName('x'), QString("x")); // unknown: shown as sent
}

// ============================================================================
// Quantiles
// ============================================================================

void BJobHistoryStatsTest::quantiles_data()
{
    QTest::addColumn<QList<qint64>>("values");
    QTest::addColumn<qint64>("p25");
    QTest::addColumn<qint64>("median");
    QTest::addColumn<qint64>("p75");
    QTest::addColumn<qint64>("p90");

    // Linear interpolation between the nearest ranks, rounded to seconds
    QTest::newRow("one") << QList<qint64>{600} << 600LL << 600LL << 600LL << 600LL;
    QTest::newRow("two") << QList<qint64>{10, 20} << 13LL << 15LL << 18LL << 19LL;
    QTest::newRow("three") << QList<qint64>{30, 10, 20} << 15LL << 20LL << 25LL << 28LL;
    QTest::newRow("four") << QList<qint64>{40, 10, 30, 20} << 18LL << 25LL << 33LL << 37LL;
    QTest::newRow("five") << QList<qint64>{50, 40, 10, 30, 20} << 20LL << 30LL << 40LL << 46LL;
    QTest::newRow("equal") << QList<qint64>{7, 7, 7} << 7LL << 7LL << 7LL << 7LL;
    // One long run among short ones moves the 90th percentile, not the median
    QTest::newRow("outlier") << QList<qint64>{100, 100, 100, 100, 100, 100, 100, 100, 100, 3600}
                             << 100LL << 100LL << 100LL << 450LL;
    QTest::newRow("eleven") << QList<qint64>{0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100}
                            << 25LL << 50LL << 75LL << 90LL;
}

void BJobHistoryStatsTest::quantiles()
{
    QFETCH(QList<qint64>, values);
    QFETCH(qint64, p25);
    QFETCH(qint64, median);
    QFETCH(qint64, p75);
    QFETCH(qint64, p90);

    const BJobHistoryStats::Quantiles q = BJobHistoryStats::quantiles(values);
    QVERIFY(q.isValid());
    QCOMPARE(q.count, int(values.size()));
    QCOMPARE(q.p25, p25);
    QCOMPARE(q.median, median);
    QCOMPARE(q.p75, p75);
    QCOMPARE(q.p90, p90);
}

void BJobHistoryStatsTest::quantilesOfNothing()
{
    const BJobHistoryStats::Quantiles q = BJobHistoryStats::quantiles({});
    QVERIFY(!q.isValid());
    QCOMPARE(q.count, 0);
}

// ============================================================================
// Statistics
// ============================================================================

void BJobHistoryStatsTest::groupsPerJobAndLevel()
{
    QList<BJobRun> runs;
    runs << run("alpha", 'I', 'T', "2026-11-16 02:00:00", 10, 300)
         << run("alpha", 'I', 'T', "2026-11-17 02:00:00", 20, 400)
         << run("alpha", 'I', 'W', "2026-11-18 02:00:00", 60, 500)
         << run("alpha", 'F', 'T', "2026-11-07 21:00:00", 1800, 7200)
         << run("beta", 'I', 'T', "2026-11-18 03:00:00", 0, 90);

    BJobHistoryStats stats;
    QVERIFY(stats.isEmpty());
    stats.setRuns(runs, 60, now);
    QVERIFY(!stats.isEmpty());
    QCOMPARE(stats.windowDays(), 60);

    const QList<BJobHistoryStats::Group> groups = stats.groups();
    QCOMPARE(groups.size(), 3);

    // Ordered by job, then level
    QCOMPARE(groups.at(0).job, QString("alpha"));
    QCOMPARE(groups.at(0).level, QString("Full"));
    QCOMPARE(groups.at(0).runs, 1);
    QCOMPARE(groups.at(0).duration.median, qint64(7200));
    QCOMPARE(groups.at(0).duration.p90, qint64(7200));
    QCOMPARE(groups.at(0).wait.median, qint64(1800));

    QCOMPARE(groups.at(1).job, QString("alpha"));
    QCOMPARE(groups.at(1).level, QString("Incremental"));
    QCOMPARE(groups.at(1).runs, 3);
    QCOMPARE(groups.at(1).duration.count, 3);
    QCOMPARE(groups.at(1).duration.p25, qint64(350));
    QCOMPARE(groups.at(1).duration.median, qint64(400));
    QCOMPARE(groups.at(1).duration.p75, qint64(450));
    QCOMPARE(groups.at(1).duration.p90, qint64(480));
    QCOMPARE(groups.at(1).wait.count, 3);
    QCOMPARE(groups.at(1).wait.median, qint64(20));
    QCOMPARE(groups.at(1).wait.p90, qint64(52));
    QCOMPARE(groups.at(1).waitUnknown, 0);
    QCOMPARE(groups.at(1).lastRun, at("2026-11-18 02:01:00"));

    QCOMPARE(groups.at(2).job, QString("beta"));
    QCOMPARE(groups.at(2).wait.median, qint64(0));  // measured: started on time

    const BJobHistoryStats::Summary sum = stats.summary();
    QCOMPARE(sum.rows, 5);
    QCOMPARE(sum.counted, 5);
    QCOMPARE(sum.notSuccessful, 0);
    QCOMPARE(sum.outsideWindow, 0);
}

void BJobHistoryStatsTest::failedAndCancelledRunsAreLeftOut()
{
    QList<BJobRun> runs;
    runs << run("alpha", 'I', 'T', "2026-11-16 02:00:00", 10, 300)
         << run("alpha", 'I', 'E', "2026-11-17 02:00:00", 10, 5)        // error
         << run("alpha", 'I', 'f', "2026-11-17 03:00:00", 10, 7)        // fatal error
         << run("alpha", 'I', 'A', "2026-11-18 02:00:00", 7200, 1)      // cancelled after waiting
         << run("alpha", 'I', 'R', "2026-11-19 02:00:00", 10, 36000)    // running
         << run("alpha", 'I', 'I', "2026-11-15 02:00:00", 10, 20);      // incomplete

    BJobHistoryStats stats;
    stats.setRuns(runs, 60, now);

    const BJobHistoryStats::Estimate e = stats.estimate("alpha", "Incremental");
    QCOMPARE(e.runs, 1);
    QCOMPARE(e.duration.median, qint64(300));
    QCOMPARE(e.wait.median, qint64(10));    // the two hours of the cancelled job are not in it
    QCOMPARE(stats.summary().notSuccessful, 5);
    QCOMPARE(stats.summary().counted, 1);
}

void BJobHistoryStatsTest::runsOutsideTheWindowAreLeftOut()
{
    // now is 2026-11-19 12:00; a window of 7 days starts 2026-11-12 12:00
    QList<BJobRun> runs;
    runs << run("alpha", 'I', 'T', "2026-11-18 02:00:00", 0, 100)
         << run("alpha", 'I', 'T', "2026-11-12 12:00:00", 0, 200)   // on the edge: inside
         << run("alpha", 'I', 'T', "2026-11-12 11:59:59", 0, 9000)  // one second too old
         << run("alpha", 'I', 'T', "2026-09-01 02:00:00", 0, 9000);

    // A run without a scheduled time is placed by its start
    BJobRun old = run("alpha", 'I', 'T', "2026-10-01 02:00:00", 0, 9000);
    old.scheduled = QDateTime();
    runs << old;

    BJobHistoryStats stats;
    stats.setRuns(runs, 7, now);

    const BJobHistoryStats::Estimate e = stats.estimate("alpha", "Incremental");
    QCOMPARE(e.runs, 2);
    QCOMPARE(e.duration.median, qint64(150));
    QCOMPARE(stats.summary().outsideWindow, 3);

    // The same runs in a window of 90 days
    stats.setRuns(runs, 90, now);
    QCOMPARE(stats.estimate("alpha", "Incremental").runs, 5);
    QCOMPARE(stats.summary().outsideWindow, 0);
}

void BJobHistoryStatsTest::missingScheduledTimeCountsForDurationOnly()
{
    BJobRun unknown = run("alpha", 'I', 'T', "2026-11-17 02:00:00", 999, 500);
    unknown.scheduled = QDateTime();

    QList<BJobRun> runs;
    runs << run("alpha", 'I', 'T', "2026-11-16 02:00:00", 40, 300) << unknown;

    BJobHistoryStats stats;
    stats.setRuns(runs, 60, now);

    const QList<BJobHistoryStats::Group> groups = stats.groups();
    QCOMPARE(groups.size(), 1);
    QCOMPARE(groups.at(0).runs, 2);
    QCOMPARE(groups.at(0).duration.count, 2);
    QCOMPARE(groups.at(0).duration.median, qint64(400));
    QCOMPARE(groups.at(0).wait.count, 1);           // not computed from a missing value
    QCOMPARE(groups.at(0).wait.median, qint64(40)); // and not pulled towards zero
    QCOMPARE(groups.at(0).waitUnknown, 1);
}

void BJobHistoryStatsTest::startBeforeScheduledTimeIsLeftOut()
{
    // The record of a copied job: start and end of the original, an hour
    // long, a day before the copy was scheduled
    BJobRun copied = run("alpha", 'F', 'T', "2026-11-18 21:00:00", 0, 3600);
    copied.started = at("2026-11-17 21:00:00");
    copied.ended = at("2026-11-17 22:00:00");

    QList<BJobRun> runs;
    runs << run("alpha", 'F', 'T', "2026-11-10 21:00:00", 120, 600) << copied;

    BJobHistoryStats stats;
    stats.setRuns(runs, 60, now);

    const BJobHistoryStats::Estimate e = stats.estimate("alpha", "Full");
    QCOMPARE(e.runs, 1);                            // neither duration nor wait of the copy
    QCOMPARE(e.duration.median, qint64(600));
    QCOMPARE(e.wait.median, qint64(120));
    QCOMPARE(e.waitUnknown, 0);
    QCOMPARE(stats.summary().foreignTimes, 1);
}

void BJobHistoryStatsTest::jobWithoutHistory()
{
    QList<BJobRun> runs;
    runs << run("alpha", 'I', 'T', "2026-11-16 02:00:00", 10, 300)
         << run("gamma", 'I', 'E', "2026-11-16 02:00:00", 10, 300);     // only ever failed

    BJobHistoryStats stats;
    stats.setRuns(runs, 60, now);

    for (const QString &job : {QString("unknown-job"), QString("gamma"), QString()}) {
        const BJobHistoryStats::Estimate e = stats.estimate(job, "Incremental");
        QCOMPARE(e.basis, BJobHistoryStats::Basis::NoHistory);
        QVERIFY(!e.hasHistory());
        QVERIFY(!e.isMeasured());
        QCOMPARE(e.runs, 0);
        QVERIFY(!e.duration.isValid());     // no figure, not a default one
        QVERIFY(!e.wait.isValid());
        QVERIFY(stats.lastRuns(job, "Incremental", 5).isEmpty());
    }

    // No history at all
    BJobHistoryStats empty;
    QVERIFY(empty.isEmpty());
    QCOMPARE(empty.estimate("alpha", "Incremental").basis, BJobHistoryStats::Basis::NoHistory);
    QVERIFY(empty.groups().isEmpty());
}

void BJobHistoryStatsTest::levelFallbackIsMarkedAsEstimate()
{
    QList<BJobRun> runs;
    runs << run("alpha", 'I', 'T', "2026-11-16 02:00:00", 10, 300)
         << run("alpha", 'I', 'T', "2026-11-17 02:00:00", 30, 500)
         << run("alpha", 'D', 'T', "2026-11-15 02:00:00", 50, 1300);

    BJobHistoryStats stats;
    stats.setRuns(runs, 60, now);

    // Measured at the level asked for; the level name in any case
    const BJobHistoryStats::Estimate inc = stats.estimate("alpha", "incremental");
    QCOMPARE(inc.basis, BJobHistoryStats::Basis::JobAndLevel);
    QVERIFY(inc.isMeasured());
    QCOMPARE(inc.runs, 2);
    QCOMPARE(inc.duration.median, qint64(400));

    // No Full in the window: all levels of the job, marked as an estimate
    const BJobHistoryStats::Estimate full = stats.estimate("alpha", "Full");
    QCOMPARE(full.basis, BJobHistoryStats::Basis::AllLevels);
    QVERIFY(full.hasHistory());
    QVERIFY(!full.isMeasured());
    QCOMPARE(full.runs, 3);
    QCOMPARE(full.duration.median, qint64(500));
    QCOMPARE(full.wait.median, qint64(30));

    // Never from another job
    QCOMPARE(stats.estimate("beta", "Incremental").basis, BJobHistoryStats::Basis::NoHistory);
    // Job names are compared exactly
    QCOMPARE(stats.estimate("Alpha", "Incremental").basis, BJobHistoryStats::Basis::NoHistory);
}

void BJobHistoryStatsTest::emptyLevelAsksForTheJob()
{
    QList<BJobRun> runs;
    runs << run("one-level", 'I', 'T', "2026-11-16 02:00:00", 10, 300)
         << run("one-level", 'I', 'T', "2026-11-17 02:00:00", 10, 500)
         << run("two-levels", 'I', 'T', "2026-11-16 02:00:00", 10, 300)
         << run("two-levels", 'F', 'T', "2026-11-17 02:00:00", 10, 5000)
         << run("admin", ' ', 'T', "2026-11-17 05:00:00", 2, 4);

    BJobHistoryStats stats;
    stats.setRuns(runs, 60, now);

    // All runs at one level: that is what was measured
    const BJobHistoryStats::Estimate one = stats.estimate("one-level", QString());
    QCOMPARE(one.basis, BJobHistoryStats::Basis::JobAndLevel);
    QCOMPARE(one.duration.median, qint64(400));

    // Runs at several levels: a mix, marked as an estimate
    const BJobHistoryStats::Estimate two = stats.estimate("two-levels", QString());
    QCOMPARE(two.basis, BJobHistoryStats::Basis::AllLevels);
    QCOMPARE(two.runs, 2);

    // A job without level (admin, restore) is a group of its own
    const BJobHistoryStats::Estimate admin = stats.estimate("admin", QString());
    QCOMPARE(admin.basis, BJobHistoryStats::Basis::JobAndLevel);
    QCOMPARE(admin.duration.median, qint64(4));

    bool found = false;
    for (const BJobHistoryStats::Group &g : stats.groups()) {
        if (g.job == "admin") {
            QVERIFY(g.level.isEmpty());
            found = true;
        }
    }
    QVERIFY(found);
}

void BJobHistoryStatsTest::lastRunsNewestFirst()
{
    QList<BJobRun> runs;
    runs << run("alpha", 'I', 'T', "2026-11-16 02:00:00", 0, 300)
         << run("alpha", 'F', 'T', "2026-11-18 02:00:00", 0, 9000)
         << run("alpha", 'I', 'T', "2026-11-17 02:00:00", 0, 500)
         << run("alpha", 'I', 'T', "2026-11-15 02:00:00", 0, 100);

    BJobHistoryStats stats;
    stats.setRuns(runs, 60, now);

    const QList<BJobHistoryStats::MeasuredRun> inc = stats.lastRuns("alpha", "Incremental", 2);
    QCOMPARE(inc.size(), 2);
    QCOMPARE(inc.at(0).durationSecs, qint64(500));
    QCOMPARE(inc.at(1).durationSecs, qint64(300));

    const QList<BJobHistoryStats::MeasuredRun> all = stats.lastRuns("alpha", QString(), 10);
    QCOMPARE(all.size(), 4);
    QCOMPARE(all.at(0).level, QString("Full"));
    QCOMPARE(all.at(3).durationSecs, qint64(100));
}

void BJobHistoryStatsTest::statisticsFromBareosReply()
{
    const BJobHistoryReply reply = BJobHistoryReply::parse(QString::fromUtf8(bareosReply));

    BJobHistoryStats stats;
    stats.setRuns(reply.runs, 60, now);

    const BJobHistoryStats::Estimate inc = stats.estimate("backup-pdc", "Incremental");
    QCOMPARE(inc.basis, BJobHistoryStats::Basis::JobAndLevel);
    QCOMPARE(inc.duration.median, qint64(200));
    QCOMPARE(inc.wait.median, qint64(12));

    // The Full ended with warnings and counts
    const BJobHistoryStats::Estimate full = stats.estimate("backup-pdc", "Full");
    QCOMPARE(full.basis, BJobHistoryStats::Basis::JobAndLevel);
    QCOMPARE(full.duration.median, qint64(7805));
    QCOMPARE(full.wait.median, qint64(1800));

    // In a window of 7 days the Full of 7 November is gone and a Full is an estimate
    stats.setRuns(reply.runs, 7, now);
    QCOMPARE(stats.estimate("backup-pdc", "Full").basis, BJobHistoryStats::Basis::AllLevels);
    QCOMPARE(stats.estimate("backup-pdc", "Full").duration.median, qint64(200));
}

void BJobHistoryStatsTest::clearLeavesNoHistory()
{
    BJobHistoryStats stats;
    stats.setRuns({run("alpha", 'I', 'T', "2026-11-16 02:00:00", 0, 300)}, 60, now);
    QVERIFY(!stats.isEmpty());

    stats.clear();
    QVERIFY(stats.isEmpty());
    QCOMPARE(stats.summary().rows, 0);
    QCOMPARE(stats.estimate("alpha", "Incremental").basis, BJobHistoryStats::Basis::NoHistory);
}

// ============================================================================
// Text
// ============================================================================

void BJobHistoryStatsTest::formatsDurations()
{
    QCOMPARE(BJobHistoryStats::formatDuration(0), QString("0s"));   // a measured zero
    QCOMPARE(BJobHistoryStats::formatDuration(45), QString("45s"));
    QCOMPARE(BJobHistoryStats::formatDuration(725), QString("12m 05s"));
    QCOMPARE(BJobHistoryStats::formatDuration(11220), QString("3h 07m"));
    QCOMPARE(BJobHistoryStats::formatDuration(187200), QString("2d 04h"));
}

QTEST_GUILESS_MAIN(BJobHistoryStatsTest)
#include "bjobhistorystats_test.moc"
