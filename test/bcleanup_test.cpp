// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bcleanup_test.cpp
 * @brief Cleanup dialog logic: selection, analysis invalidation, command sequencing
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>
#include <QJsonArray>
#include <QJsonObject>
#include <QSignalSpy>

#include "bcleanupplan.h"
#include "bcleanuprunner.h"

using BCleanup::Options;
using BCleanup::Plan;
using Outcome = BCleanupRunner::Outcome;
using StepKind = BCleanupRunner::StepKind;

namespace {

/// A job row as the job list holds it
QJsonObject job(int id, const QString &name, const QString &client, const QString &fileset,
                const QString &level, const QString &start, const QString &status = "T",
                int files = 100, const QString &type = "B")
{
    QJsonObject row;
    row["jobid"] = QString::number(id);
    row["name"] = name;
    row["client"] = client;
    if (!fileset.isEmpty())
        row["fileset"] = fileset;
    row["type"] = type;
    row["level"] = level;
    row["starttime"] = start;
    row["jobstatus"] = status;
    row["jobfiles"] = QString::number(files);
    row["jobbytes"] = QString::number(1000);
    return row;
}

Options oldFulls(int keep)
{
    Options options;
    options.removeOldFulls = true;
    options.keepFulls = keep;
    return options;
}

QStringList ids(const QList<BCleanup::JobRef> &jobs)
{
    QStringList result;
    for (const BCleanup::JobRef &ref : jobs)
        result.append(ref.jobId);
    return result;
}

const QDateTime Now = QDateTime::fromString("2026-11-18 12:00:00", "yyyy-MM-dd HH:mm:ss");

/// Records what the runner asks to send
struct FakeDirector {
    QStringList sent;
    void attach(BCleanupRunner &runner)
    {
        runner.setSender([this](StepKind kind, const QString &jobId) {
            sent.append(kind == StepKind::DeleteJob ? "delete " + jobId : QString("prune"));
        });
    }
};

QString deleted(const QString &jobId)
{
    return QString("{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{\"deleted\":{\"jobids\":[\"%1\"]}}}")
        .arg(jobId);
}

const char *ErrorReply =
    "{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":{\"code\":1,\"message\":\"failed\","
    "\"data\":{\"messages\":{\"error\":[\"delete: is an invalid command.\"]}}}}";

} // namespace

class BCleanupTest : public QObject
{
    Q_OBJECT

private slots:
    // Old Fulls
    void twoJobsOnOneClientAreSeparate();
    void sameJobWithTwoFilesetsIsSeparate();
    void fullWithDependentsIsKept();
    void dependentsInOtherGroupsDoNotCount();
    void fullIsSelectedWhenItsDependentsAreDeletedToo();
    void failedDependentsDoNotBlock();
    void keepCountEdges_data();
    void keepCountEdges();
    void keepCountBelowOneIsRefused();
    void onlySuccessfulBackupsCount();
    void fullDeletedAsEmptyDoesNotCountAsKept();
    void rowsWithoutTypeRefuseTheAnalysis();
    void rowsWithoutJobIdRefuseTheAnalysis();
    void jobWithoutFilesetIsSkipped();
    void jobWithUnknownLevelOrTimeIsSkipped();
    void equalStartTimesAreOrderedByJobId();

    // Other options
    void emptyJobsNeedAFileCount();
    void failedJobsOlderThanTheLimit();
    void deletionOrderAndDuplicates();
    void nothingSelectedWithoutOptions();

    // Stale analysis
    void analysisIsInvalidatedByOptionChange();
    void analysisWithoutWorkCannotBeExecuted();

    // Sequencing
    void runnerSendsOneCommandAtATime();
    void runnerStopsAtAnError();
    void runnerStopsWhenDeletionIsNotConfirmed();
    void runnerIgnoresRepliesItDoesNotWaitFor();
    void runnerPrunesLast();
    void runnerAbortLeavesTheRestUnsent();
    void runnerStopRequestFinishesAfterTheAwaitedReply();
    void evaluateDeleteReply();
    void evaluatePruneReply();
};

// ---------------------------------------------------------------------------
// Old Fulls
// ---------------------------------------------------------------------------

void BCleanupTest::twoJobsOnOneClientAreSeparate()
{
    // The reported failure: job A has weekly Fulls, job B monthly Fulls, same
    // client. Grouped by client only, the three newest Fulls are all A's and
    // every Full of B would be deleted.
    QJsonArray jobs;
    jobs.append(job(1, "B", "c1-fd", "fsB", "F", "2026-09-01 01:00:00"));
    jobs.append(job(2, "B", "c1-fd", "fsB", "I", "2026-09-02 01:00:00"));
    jobs.append(job(3, "B", "c1-fd", "fsB", "F", "2026-10-01 01:00:00"));
    jobs.append(job(4, "B", "c1-fd", "fsB", "I", "2026-10-02 01:00:00"));
    jobs.append(job(10, "A", "c1-fd", "fsA", "F", "2026-10-12 02:00:00"));
    jobs.append(job(11, "A", "c1-fd", "fsA", "F", "2026-10-19 02:00:00"));
    jobs.append(job(12, "A", "c1-fd", "fsA", "F", "2026-10-26 02:00:00"));
    jobs.append(job(13, "A", "c1-fd", "fsA", "F", "2026-11-02 02:00:00"));
    jobs.append(job(14, "A", "c1-fd", "fsA", "F", "2026-11-09 02:00:00"));

    const Plan plan = BCleanup::buildPlan(jobs, oldFulls(3), Now);

    QCOMPARE(plan.oldFullsRefusal, BCleanup::Refusal::None);
    QCOMPARE(ids(plan.oldFulls), QStringList({"10", "11"}));
    QCOMPARE(plan.jobIdsToDelete(), QStringList({"10", "11"}));
    QCOMPARE(plan.oldFullsBytes, qint64(2000));

    // Only A's group is reported; B has fewer Fulls than the keep count
    QCOMPARE(plan.fullGroups.size(), 1);
    QCOMPARE(plan.fullGroups[0].name, QString("A"));
    QCOMPARE(plan.fullGroups[0].fullCount, 5);
}

void BCleanupTest::sameJobWithTwoFilesetsIsSeparate()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs1", "F", "2026-01-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "fs2", "F", "2026-02-01 01:00:00"));
    jobs.append(job(3, "A", "c1-fd", "fs2", "F", "2026-03-01 01:00:00"));
    // Same job name on another client
    jobs.append(job(4, "A", "c2-fd", "fs1", "F", "2026-04-01 01:00:00"));
    jobs.append(job(5, "A", "c2-fd", "fs1", "F", "2026-05-01 01:00:00"));

    const Plan plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);

    // Per group the newest stays: 1 (only one), 3, 5
    QCOMPARE(ids(plan.oldFulls), QStringList({"2", "4"}));
}

void BCleanupTest::fullWithDependentsIsKept()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "fs", "I", "2026-01-02 01:00:00"));
    jobs.append(job(3, "A", "c1-fd", "fs", "D", "2026-01-03 01:00:00"));
    jobs.append(job(4, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));
    jobs.append(job(5, "A", "c1-fd", "fs", "F", "2026-03-01 01:00:00"));
    jobs.append(job(6, "A", "c1-fd", "fs", "I", "2026-03-02 01:00:00"));
    jobs.append(job(7, "A", "c1-fd", "fs", "F", "2026-04-01 01:00:00"));

    // keep 3: only Full 1 is beyond the count, and 2 and 3 depend on it
    Plan plan = BCleanup::buildPlan(jobs, oldFulls(3), Now);
    QVERIFY(plan.oldFulls.isEmpty());
    QVERIFY(!plan.hasWork());
    QCOMPARE(plan.fullGroups.size(), 1);
    QCOMPARE(plan.fullGroups[0].blocked.size(), 1);
    QCOMPARE(plan.fullGroups[0].blocked[0].job.jobId, QString("1"));
    QCOMPARE(plan.fullGroups[0].blocked[0].dependentJobIds, QStringList({"2", "3"}));

    // keep 2: Full 4 has nothing between it and Full 5 and goes; Full 1 stays
    plan = BCleanup::buildPlan(jobs, oldFulls(2), Now);
    QCOMPARE(ids(plan.oldFulls), QStringList({"4"}));

    // keep 1: Full 5 is beyond the count too, but Incremental 6 depends on it
    plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);
    QCOMPARE(ids(plan.oldFulls), QStringList({"4"}));
    QCOMPARE(plan.fullGroups[0].blocked.size(), 2);
}

void BCleanupTest::dependentsInOtherGroupsDoNotCount()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    // Incrementals of another job, another client and another fileset
    jobs.append(job(2, "B", "c1-fd", "fs", "I", "2026-01-02 01:00:00"));
    jobs.append(job(3, "A", "c2-fd", "fs", "I", "2026-01-03 01:00:00"));
    jobs.append(job(4, "A", "c1-fd", "other", "I", "2026-01-04 01:00:00"));
    jobs.append(job(5, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));
    // An Incremental after the next Full belongs to that Full
    jobs.append(job(6, "A", "c1-fd", "fs", "I", "2026-02-02 01:00:00"));

    const Plan plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);
    QCOMPARE(ids(plan.oldFulls), QStringList({"1"}));
}

void BCleanupTest::fullIsSelectedWhenItsDependentsAreDeletedToo()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "fs", "I", "2026-01-02 01:00:00", "T", 0));  // empty
    jobs.append(job(3, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));

    // Without the empty-jobs option the Incremental stays and protects the Full
    Plan plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);
    QVERIFY(plan.oldFulls.isEmpty());

    Options options = oldFulls(1);
    options.removeEmptyJobs = true;
    plan = BCleanup::buildPlan(jobs, options, Now);
    QCOMPARE(ids(plan.oldFulls), QStringList({"1"}));
    QCOMPARE(plan.oldFulls[0].requiredDeletions, QStringList({"2"}));
    // The dependent is deleted before its Full
    QCOMPARE(plan.jobIdsToDelete(), QStringList({"2", "1"}));

    // One dependent selected, one not: the Full stays
    jobs.insert(2, job(9, "A", "c1-fd", "fs", "I", "2026-01-03 01:00:00"));
    plan = BCleanup::buildPlan(jobs, options, Now);
    QVERIFY(plan.oldFulls.isEmpty());
    QCOMPARE(plan.fullGroups[0].blocked[0].dependentJobIds, QStringList({"9"}));
}

void BCleanupTest::failedDependentsDoNotBlock()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "fs", "I", "2026-01-02 01:00:00", "f"));
    jobs.append(job(3, "A", "c1-fd", "fs", "I", "2026-01-03 01:00:00", "E"));
    jobs.append(job(4, "A", "c1-fd", "fs", "I", "2026-01-04 01:00:00", "A"));
    jobs.append(job(5, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));
    QCOMPARE(ids(BCleanup::buildPlan(jobs, oldFulls(1), Now).oldFulls), QStringList({"1"}));

    // A running Incremental is a dependent
    jobs.insert(1, job(6, "A", "c1-fd", "fs", "I", "2026-01-05 01:00:00", "R"));
    QVERIFY(BCleanup::buildPlan(jobs, oldFulls(1), Now).oldFulls.isEmpty());
}

void BCleanupTest::keepCountEdges_data()
{
    QTest::addColumn<int>("keep");
    QTest::addColumn<QStringList>("expected");

    // Four Fulls 1..4, oldest first
    QTest::newRow("keep 1") << 1 << QStringList({"1", "2", "3"});
    QTest::newRow("keep 2") << 2 << QStringList({"1", "2"});
    QTest::newRow("keep 3 (one less than present)") << 3 << QStringList({"1"});
    QTest::newRow("keep 4 (all present)") << 4 << QStringList();
    QTest::newRow("keep 5 (more than present)") << 5 << QStringList();
    QTest::newRow("keep 100") << 100 << QStringList();
}

void BCleanupTest::keepCountEdges()
{
    QFETCH(int, keep);
    QFETCH(QStringList, expected);

    // Not in time order, to make sure the start time decides
    QJsonArray jobs;
    jobs.append(job(3, "A", "c1-fd", "fs", "F", "2026-03-01 01:00:00"));
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(4, "A", "c1-fd", "fs", "F", "2026-04-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));

    const Plan plan = BCleanup::buildPlan(jobs, oldFulls(keep), Now);
    QCOMPARE(plan.oldFullsRefusal, BCleanup::Refusal::None);
    QCOMPARE(ids(plan.oldFulls), expected);
}

void BCleanupTest::keepCountBelowOneIsRefused()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));

    for (int keep : {0, -1}) {
        const Plan plan = BCleanup::buildPlan(jobs, oldFulls(keep), Now);
        QCOMPARE(plan.oldFullsRefusal, BCleanup::Refusal::KeepCountTooSmall);
        QVERIFY(plan.oldFulls.isEmpty());
        QVERIFY(!plan.hasWork());
    }
}

void BCleanupTest::onlySuccessfulBackupsCount()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00", "W"));   // warnings: usable
    jobs.append(job(3, "A", "c1-fd", "fs", "F", "2026-03-01 01:00:00", "f"));   // failed
    jobs.append(job(4, "A", "c1-fd", "fs", "F", "2026-04-01 01:00:00", "R"));   // running
    jobs.append(job(5, "A", "c1-fd", "fs", "F", "2026-05-01 01:00:00", "T", 100, "R"));  // restore
    jobs.append(job(6, "A", "c1-fd", "fs", "F", "2026-06-01 01:00:00", "T", 100, "C"));  // copy
    jobs.append(job(7, "A", "c1-fd", "fs", "F", "2026-07-01 01:00:00", "T", 100, "D"));  // admin

    // Two usable Fulls (1 and 2): with keep 2 nothing goes, although seven
    // rows have level F
    Plan plan = BCleanup::buildPlan(jobs, oldFulls(2), Now);
    QVERIFY(plan.oldFulls.isEmpty());

    // keep 1: only Full 1; the failed, running and non-backup rows are never selected
    plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);
    QCOMPARE(ids(plan.oldFulls), QStringList({"1"}));
}

void BCleanupTest::fullDeletedAsEmptyDoesNotCountAsKept()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));
    jobs.append(job(3, "A", "c1-fd", "fs", "F", "2026-03-01 01:00:00", "T", 0));  // empty, newest

    Options options = oldFulls(1);
    options.removeEmptyJobs = true;
    const Plan plan = BCleanup::buildPlan(jobs, options, Now);

    // Full 3 goes as an empty job, so Full 2 is the one that is kept
    QCOMPARE(ids(plan.emptyJobs), QStringList({"3"}));
    QCOMPARE(ids(plan.oldFulls), QStringList({"1"}));
}

void BCleanupTest::rowsWithoutTypeRefuseTheAnalysis()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));
    QJsonObject noType = job(3, "A", "c1-fd", "fs", "F", "2026-03-01 01:00:00");
    noType.remove("type");
    jobs.append(noType);

    Plan plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);
    QCOMPARE(plan.oldFullsRefusal, BCleanup::Refusal::RowsWithoutType);
    QCOMPARE(plan.oldFullsRefusalRows, 1);
    QVERIFY(plan.oldFulls.isEmpty());

    // A backup without client or job name cannot be assigned to a group
    jobs.removeLast();
    jobs.append(job(3, "", "c1-fd", "fs", "F", "2026-03-01 01:00:00"));
    plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);
    QCOMPARE(plan.oldFullsRefusal, BCleanup::Refusal::BackupRowsWithoutClientOrName);
    QVERIFY(plan.oldFulls.isEmpty());
}

void BCleanupTest::rowsWithoutJobIdRefuseTheAnalysis()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));
    QJsonObject noId = job(3, "A", "c1-fd", "fs", "I", "2026-01-02 01:00:00");
    noId.remove("jobid");
    jobs.append(noId);

    const Plan plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);
    QCOMPARE(plan.oldFullsRefusal, BCleanup::Refusal::RowsWithoutJobId);
    QVERIFY(plan.oldFulls.isEmpty());
}

void BCleanupTest::jobWithoutFilesetIsSkipped()
{
    QJsonArray jobs;
    // Job A: one row has no fileset; it could be a dependent of any Full of A
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "", "I", "2026-01-02 01:00:00"));
    jobs.append(job(3, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));
    // Job B is complete
    jobs.append(job(4, "B", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(5, "B", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));

    const Plan plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);
    QCOMPARE(plan.oldFullsRefusal, BCleanup::Refusal::None);
    QCOMPARE(ids(plan.oldFulls), QStringList({"4"}));
    QCOMPARE(plan.skippedJobs.size(), 1);
    QCOMPARE(plan.skippedJobs[0].name, QString("A"));
    QCOMPARE(plan.skippedJobs[0].client, QString("c1-fd"));
    QCOMPARE(plan.skippedJobs[0].reason, BCleanup::SkipReason::MissingFileset);
    QCOMPARE(plan.skippedJobs[0].rows, 1);

    // No row has a fileset (plain `list jobs` data): nothing is selected at all
    QJsonArray bare;
    for (const QJsonValue &value : jobs) {
        QJsonObject row = value.toObject();
        row.remove("fileset");
        bare.append(row);
    }
    const Plan barePlan = BCleanup::buildPlan(bare, oldFulls(1), Now);
    QVERIFY(barePlan.oldFulls.isEmpty());
    QCOMPARE(barePlan.skippedJobs.size(), 2);
}

void BCleanupTest::jobWithUnknownLevelOrTimeIsSkipped()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "fs", "", "2026-01-02 01:00:00"));
    jobs.append(job(3, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));

    Plan plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);
    QVERIFY(plan.oldFulls.isEmpty());
    QCOMPARE(plan.skippedJobs[0].reason, BCleanup::SkipReason::UnknownLevel);

    jobs.replace(1, job(2, "A", "c1-fd", "fs", "I", "not a time"));
    plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);
    QVERIFY(plan.oldFulls.isEmpty());
    QCOMPARE(plan.skippedJobs[0].reason, BCleanup::SkipReason::InvalidStartTime);

    QJsonObject noStatus = job(2, "A", "c1-fd", "fs", "I", "2026-01-02 01:00:00");
    noStatus.remove("jobstatus");
    jobs.replace(1, noStatus);
    plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);
    QVERIFY(plan.oldFulls.isEmpty());
    QCOMPARE(plan.skippedJobs[0].reason, BCleanup::SkipReason::MissingStatus);
}

void BCleanupTest::equalStartTimesAreOrderedByJobId()
{
    QJsonArray jobs;
    jobs.append(job(12, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(9, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    const Plan plan = BCleanup::buildPlan(jobs, oldFulls(1), Now);
    QCOMPARE(ids(plan.oldFulls), QStringList({"9"}));
}

// ---------------------------------------------------------------------------
// Other options
// ---------------------------------------------------------------------------

void BCleanupTest::emptyJobsNeedAFileCount()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "I", "2026-01-01 01:00:00", "T", 0));
    jobs.append(job(2, "A", "c1-fd", "fs", "I", "2026-01-02 01:00:00", "T", 5));
    jobs.append(job(3, "A", "c1-fd", "fs", "I", "2026-01-03 01:00:00", "f", 0));
    QJsonObject noCount = job(4, "A", "c1-fd", "fs", "I", "2026-01-04 01:00:00", "T", 0);
    noCount.remove("jobfiles");
    jobs.append(noCount);

    Options options;
    options.removeEmptyJobs = true;
    const Plan plan = BCleanup::buildPlan(jobs, options, Now);
    QCOMPARE(ids(plan.emptyJobs), QStringList({"1"}));
    QCOMPARE(plan.emptyRowsWithoutFileCount, 1);
}

void BCleanupTest::failedJobsOlderThanTheLimit()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "I", "2026-10-18 11:59:59", "f"));  // just older than 31 days
    jobs.append(job(2, "A", "c1-fd", "fs", "I", "2026-10-18 12:00:00", "f"));  // exactly 31 days
    jobs.append(job(3, "A", "c1-fd", "fs", "I", "2026-10-18 12:00:01", "f"));
    jobs.append(job(4, "A", "c1-fd", "fs", "I", "2026-01-01 01:00:00", "E"));
    jobs.append(job(5, "A", "c1-fd", "fs", "I", "2026-01-01 01:00:00", "A"));
    jobs.append(job(6, "A", "c1-fd", "fs", "I", "2026-01-01 01:00:00", "T"));
    jobs.append(job(7, "A", "c1-fd", "fs", "I", "", "f"));                    // no start time

    Options options;
    options.removeFailedJobs = true;
    options.failedAgeDays = 31;
    const Plan plan = BCleanup::buildPlan(jobs, options, Now);
    QCOMPARE(ids(plan.failedJobs), QStringList({"1", "4", "5"}));
}

void BCleanupTest::deletionOrderAndDuplicates()
{
    QJsonArray jobs;
    // Old, failed and empty at once: listed by one option only in the run
    jobs.append(job(1, "A", "c1-fd", "fs", "I", "2026-01-01 01:00:00", "T", 0));
    jobs.append(job(2, "A", "c1-fd", "fs", "F", "2026-01-02 01:00:00", "T", 0));
    jobs.append(job(3, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));
    jobs.append(job(4, "A", "c1-fd", "fs", "F", "2026-03-01 01:00:00"));
    jobs.append(job(5, "A", "c1-fd", "fs", "I", "2026-01-05 01:00:00", "f"));

    Options options = oldFulls(1);
    options.removeEmptyJobs = true;
    options.removeFailedJobs = true;
    options.failedAgeDays = 30;
    const Plan plan = BCleanup::buildPlan(jobs, options, Now);

    const QStringList order = plan.jobIdsToDelete();
    QCOMPARE(order, QStringList({"5", "1", "2", "3"}));
    QCOMPARE(QSet<QString>(order.begin(), order.end()).size(), order.size());
}

void BCleanupTest::nothingSelectedWithoutOptions()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2020-01-01 01:00:00", "T", 0));
    jobs.append(job(2, "A", "c1-fd", "fs", "F", "2020-02-01 01:00:00", "f"));
    jobs.append(job(3, "A", "c1-fd", "fs", "F", "2020-03-01 01:00:00"));

    const Plan plan = BCleanup::buildPlan(jobs, Options(), Now);
    QVERIFY(!plan.hasWork());
    QVERIFY(plan.jobIdsToDelete().isEmpty());
    QVERIFY(!Options().anySelected());
}

// ---------------------------------------------------------------------------
// Stale analysis
// ---------------------------------------------------------------------------

void BCleanupTest::analysisIsInvalidatedByOptionChange()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));
    jobs.append(job(2, "A", "c1-fd", "fs", "F", "2026-02-01 01:00:00"));
    jobs.append(job(3, "A", "c1-fd", "fs", "F", "2026-03-01 01:00:00"));

    const Options analyzed = oldFulls(2);
    BCleanup::Analysis analysis;
    QVERIFY(!analysis.isValid());
    QVERIFY(!analysis.canExecute(analyzed));

    analysis.set(BCleanup::buildPlan(jobs, analyzed, Now));
    QVERIFY(analysis.isValid());
    QVERIFY(analysis.canExecute(analyzed));
    QCOMPARE(analysis.plan().jobIdsToDelete(), QStringList({"1"}));

    // Every single option differs from the analyzed set: not executable,
    // even if nobody called invalidate()
    Options changed = analyzed;
    changed.keepFulls = 1;
    QVERIFY(!analysis.canExecute(changed));
    changed = analyzed;
    changed.removeOldFulls = false;
    QVERIFY(!analysis.canExecute(changed));
    changed = analyzed;
    changed.removeEmptyJobs = true;
    QVERIFY(!analysis.canExecute(changed));
    changed = analyzed;
    changed.removeFailedJobs = true;
    QVERIFY(!analysis.canExecute(changed));
    changed = analyzed;
    changed.failedAgeDays = 7;
    QVERIFY(!analysis.canExecute(changed));
    changed = analyzed;
    changed.pruneVolumes = true;
    QVERIFY(!analysis.canExecute(changed));

    // What the dialog does on an option change
    analysis.invalidate();
    QVERIFY(!analysis.isValid());
    QVERIFY(!analysis.canExecute(analyzed));
    QVERIFY(analysis.plan().jobIdsToDelete().isEmpty());
}

void BCleanupTest::analysisWithoutWorkCannotBeExecuted()
{
    QJsonArray jobs;
    jobs.append(job(1, "A", "c1-fd", "fs", "F", "2026-01-01 01:00:00"));

    BCleanup::Analysis analysis;
    analysis.set(BCleanup::buildPlan(jobs, oldFulls(3), Now));
    QVERIFY(analysis.isValid());
    QVERIFY(!analysis.canExecute(oldFulls(3)));

    Options prune;
    prune.pruneVolumes = true;
    analysis.set(BCleanup::buildPlan(jobs, prune, Now));
    QVERIFY(analysis.canExecute(prune));
}

// ---------------------------------------------------------------------------
// Sequencing
// ---------------------------------------------------------------------------

namespace {

Plan planFor(const QStringList &jobIds, bool prune = false)
{
    Plan plan;
    for (const QString &id : jobIds) {
        BCleanup::JobRef ref;
        ref.jobId = id;
        plan.failedJobs.append(ref);
    }
    plan.pruneVolumes = prune;
    return plan;
}

} // namespace

void BCleanupTest::runnerSendsOneCommandAtATime()
{
    BCleanupRunner runner;
    FakeDirector director;
    director.attach(runner);
    QSignalSpy finished(&runner, &BCleanupRunner::finished);

    runner.start(planFor({"7", "8", "9"}));
    QVERIFY(runner.isRunning());
    QCOMPARE(director.sent, QStringList({"delete 7"}));   // nothing queued ahead

    runner.handleReply(StepKind::DeleteJob, deleted("7"));
    QCOMPARE(director.sent, QStringList({"delete 7", "delete 8"}));
    QCOMPARE(finished.count(), 0);

    // Text mode confirmation
    runner.handleReply(StepKind::DeleteJob, "Jobid 8 and associated records deleted from the catalog.\n");
    QCOMPARE(director.sent, QStringList({"delete 7", "delete 8", "delete 9"}));

    runner.handleReply(StepKind::DeleteJob, deleted("9"));
    QCOMPARE(finished.count(), 1);
    QVERIFY(!runner.isRunning());
    QVERIFY(runner.allSucceeded());
    QCOMPARE(runner.confirmedJobIds(), QStringList({"7", "8", "9"}));
    QCOMPARE(director.sent.size(), 3);
}

void BCleanupTest::runnerStopsAtAnError()
{
    BCleanupRunner runner;
    FakeDirector director;
    director.attach(runner);
    QSignalSpy finished(&runner, &BCleanupRunner::finished);

    runner.start(planFor({"7", "8", "9"}));
    runner.handleReply(StepKind::DeleteJob, deleted("7"));
    runner.handleReply(StepKind::DeleteJob, QString::fromLatin1(ErrorReply));

    QCOMPARE(finished.count(), 1);
    QCOMPARE(director.sent, QStringList({"delete 7", "delete 8"}));   // 9 was never sent
    QVERIFY(!runner.allSucceeded());
    QCOMPARE(runner.steps()[0].outcome, Outcome::Confirmed);
    QCOMPARE(runner.steps()[1].outcome, Outcome::Failed);
    QVERIFY(runner.steps()[1].reply.contains("invalid command"));
    QCOMPARE(runner.steps()[2].outcome, Outcome::NotSent);
    // Only the confirmed job may leave the job model
    QCOMPARE(runner.confirmedJobIds(), QStringList({"7"}));
}

void BCleanupTest::runnerStopsWhenDeletionIsNotConfirmed()
{
    BCleanupRunner runner;
    FakeDirector director;
    director.attach(runner);

    runner.start(planFor({"7", "8"}));
    // No error, but the answer names another job: not a confirmation for 7
    runner.handleReply(StepKind::DeleteJob, deleted("70"));

    QVERIFY(!runner.isRunning());
    QCOMPARE(runner.steps()[0].outcome, Outcome::Unconfirmed);
    QCOMPARE(runner.steps()[1].outcome, Outcome::NotSent);
    QVERIFY(runner.confirmedJobIds().isEmpty());
    QCOMPARE(director.sent, QStringList({"delete 7"}));
}

void BCleanupTest::runnerIgnoresRepliesItDoesNotWaitFor()
{
    BCleanupRunner runner;
    FakeDirector director;
    director.attach(runner);

    // Not running
    runner.handleReply(StepKind::DeleteJob, deleted("7"));
    QVERIFY(runner.steps().isEmpty());

    runner.start(planFor({"7"}, true));
    // A prune reply while a delete is awaited
    runner.handleReply(StepKind::PruneVolumes, "{\"result\":{}}");
    QVERIFY(runner.isWaitingFor(StepKind::DeleteJob));
    QCOMPARE(runner.steps()[0].outcome, Outcome::Pending);

    runner.handleReply(StepKind::DeleteJob, deleted("7"));
    QVERIFY(runner.isWaitingFor(StepKind::PruneVolumes));
    // A late delete reply while prune is awaited
    runner.handleReply(StepKind::DeleteJob, deleted("7"));
    QVERIFY(runner.isRunning());

    runner.handleReply(StepKind::PruneVolumes, "{\"result\":{}}");
    QVERIFY(!runner.isRunning());

    // After the run
    runner.handleReply(StepKind::DeleteJob, QString::fromLatin1(ErrorReply));
    QVERIFY(runner.allSucceeded());
}

void BCleanupTest::runnerPrunesLast()
{
    BCleanupRunner runner;
    FakeDirector director;
    director.attach(runner);

    runner.start(planFor({"7"}, true));
    QCOMPARE(director.sent, QStringList({"delete 7"}));
    runner.handleReply(StepKind::DeleteJob, deleted("7"));
    QCOMPARE(director.sent, QStringList({"delete 7", "prune"}));
    runner.handleReply(StepKind::PruneVolumes, "{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{}}");

    QVERIFY(runner.allSucceeded());
    QCOMPARE(runner.steps()[1].outcome, Outcome::Answered);
    QCOMPARE(runner.count(Outcome::Confirmed), 1);

    // A failed delete means prune is not sent
    FakeDirector second;
    second.attach(runner);
    runner.start(planFor({"7"}, true));
    runner.handleReply(StepKind::DeleteJob, QString::fromLatin1(ErrorReply));
    QCOMPARE(second.sent, QStringList({"delete 7"}));
    QCOMPARE(runner.steps()[1].outcome, Outcome::NotSent);
}

void BCleanupTest::runnerAbortLeavesTheRestUnsent()
{
    BCleanupRunner runner;
    FakeDirector director;
    director.attach(runner);
    QSignalSpy finished(&runner, &BCleanupRunner::finished);

    runner.start(planFor({"7", "8"}));
    runner.abort("no answer");

    QCOMPARE(finished.count(), 1);
    QCOMPARE(runner.steps()[0].outcome, Outcome::NoReply);
    QCOMPARE(runner.steps()[1].outcome, Outcome::NotSent);
    QCOMPARE(runner.abortReason(), QString("no answer"));

    // The late answer changes nothing and sends nothing
    runner.handleReply(StepKind::DeleteJob, deleted("7"));
    QCOMPARE(director.sent, QStringList({"delete 7"}));
    QVERIFY(runner.confirmedJobIds().isEmpty());
    QCOMPARE(finished.count(), 1);
}

void BCleanupTest::runnerStopRequestFinishesAfterTheAwaitedReply()
{
    BCleanupRunner runner;
    FakeDirector director;
    director.attach(runner);

    runner.start(planFor({"7", "8"}));
    runner.requestStop();
    QVERIFY(runner.isRunning());   // The answer for 7 is still evaluated

    runner.handleReply(StepKind::DeleteJob, deleted("7"));
    QVERIFY(!runner.isRunning());
    QCOMPARE(director.sent, QStringList({"delete 7"}));
    QCOMPARE(runner.confirmedJobIds(), QStringList({"7"}));
    QCOMPARE(runner.steps()[1].outcome, Outcome::NotSent);
    QVERIFY(!runner.allSucceeded());
}

void BCleanupTest::evaluateDeleteReply()
{
    QCOMPARE(BCleanupRunner::evaluateDeleteReply("7", deleted("7")), Outcome::Confirmed);
    QCOMPARE(BCleanupRunner::evaluateDeleteReply(
                 "7", "{\"result\":{\"deleted\":{\"jobids\":[7]}}}"), Outcome::Confirmed);
    QCOMPARE(BCleanupRunner::evaluateDeleteReply(
                 "7", "Jobid 7 and associated records deleted from the catalog."), Outcome::Confirmed);

    // Another job, a prefix of the ID, an empty result, no answer
    QCOMPARE(BCleanupRunner::evaluateDeleteReply("7", deleted("77")), Outcome::Unconfirmed);
    QCOMPARE(BCleanupRunner::evaluateDeleteReply(
                 "7", "Jobid 77 and associated records deleted from the catalog."), Outcome::Unconfirmed);
    QCOMPARE(BCleanupRunner::evaluateDeleteReply(
                 "7", "{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{}}"), Outcome::Unconfirmed);
    QCOMPARE(BCleanupRunner::evaluateDeleteReply("7", "{\"result\":{\"jobs\":[\"7\"]}}"),
             Outcome::Unconfirmed);
    QCOMPARE(BCleanupRunner::evaluateDeleteReply("7", ""), Outcome::Unconfirmed);
    QCOMPARE(BCleanupRunner::evaluateDeleteReply("", deleted("")), Outcome::Unconfirmed);

    // Errors win over anything else in the answer
    QCOMPARE(BCleanupRunner::evaluateDeleteReply("7", QString::fromLatin1(ErrorReply)), Outcome::Failed);
    QCOMPARE(BCleanupRunner::evaluateDeleteReply(
                 "7", "{\"result\":{\"error\":\"x\",\"deleted\":{\"jobids\":[\"7\"]}}}"), Outcome::Failed);
    QCOMPARE(BCleanupRunner::evaluateDeleteReply("7", "Error: command not authorized"), Outcome::Failed);
    QCOMPARE(BCleanupRunner::evaluateDeleteReply("7", "Fatal ERR=no such job"), Outcome::Failed);
}

void BCleanupTest::evaluatePruneReply()
{
    QCOMPARE(BCleanupRunner::evaluatePruneReply("{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{}}"),
             Outcome::Answered);
    QCOMPARE(BCleanupRunner::evaluatePruneReply("Pruned 3 Jobs on Volume \"Full-0001\" from catalog."),
             Outcome::Answered);
    QCOMPARE(BCleanupRunner::evaluatePruneReply(QString::fromLatin1(ErrorReply)), Outcome::Failed);
    QCOMPARE(BCleanupRunner::evaluatePruneReply("  "), Outcome::Unconfirmed);
}

QTEST_APPLESS_MAIN(BCleanupTest)
#include "bcleanup_test.moc"
