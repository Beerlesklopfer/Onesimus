/**
 * @file bschedulewaitmodel_test.cpp
 * @brief Predicted waiting time of scheduled runs (BScheduleWaitModel)
 *
 * The queue rules under test are those of Bareos core/src/dird/jobq.cc
 * (branch bareos-25, commit 0e7742c) as the header of the model states
 * them. Nothing here was captured from a running Director.
 *
 * The "show" replies are written out from source code: the form of
 * OutputFormatterResource in API mode 2 (resources of a kind in an object
 * keyed by name, directive names in lower case without blanks), and the
 * rule of PrintResourceItem() that a directive at its default is not
 * printed. [constructed] marks replies no known Director sends.
 *
 * Runs without a Director and without widgets (registered with CTest).
 */

#include <QtTest>

#include <QJsonDocument>
#include <QTimeZone>

#include "schedules/model/bschedulewaitmodel.h"

namespace {

QDateTime at(int day, int hour, int minute)
{
    // November 2026: no change of daylight saving anywhere near
    return QDateTime(QDate(2026, 11, day), QTime(hour, minute), QTimeZone::utc());
}

BWaitRun run(const QString &job, const QDateTime &scheduled, qint64 minutes,
             const QString &client = QStringLiteral("fd1"),
             const QString &storage = QStringLiteral("File"), int priority = 10)
{
    BWaitRun r;
    r.id = job;
    r.job = job;
    r.client = client;
    r.storage = storage;
    r.level = QStringLiteral("Incremental");
    r.priority = priority;
    r.allowMixedPriority = false;
    r.scheduled = scheduled;
    r.durationBasis = BWaitRun::Duration::Measured;
    r.durationSecs = minutes * 60;
    return r;
}

/// Limits under which nothing waits, for every name the runs use
BWaitLimits ample(const QList<BWaitRun> &runs, int value = 100)
{
    BWaitLimits limits;
    limits.director = value;
    for (const BWaitRun &r : runs) {
        limits.jobs.insert(r.job, value);
        limits.clients.insert(r.client, value);
        limits.storages.insert(r.storage, value);
    }
    return limits;
}

QJsonObject object(const char *json)
{
    return QJsonDocument::fromJson(QByteArray(json)).object();
}

} // namespace

class BScheduleWaitModelTest : public QObject
{
    Q_OBJECT

private slots:
    // --- Limits ---
    void storageLimitTenNineParallel();
    void storageLimitOneSerialises();
    void clientLimit();
    void jobLimit();
    void directorLimit();
    void directorLimitKeepsQueueOrder();
    void otherResourcesOvertake();
    void copyJobsTakeNoPlace();
    void unknownStorageIsUncertain();

    // --- Priority ---
    void lowerPriorityWaitsForRunning();
    void maintainerCatalogBackup();
    void higherPriorityNeedsMixedOnAll();
    void waitingHigherPriorityEndsTheWalk();
    void queueIsSortedByPriorityThenTime();

    // --- Certainty ---
    void unknownLimitIsUncertain();
    void unknownLimitWithoutCompanyIsCertain();
    void unknownDirectorLimitIsUncertain();
    void noHistoryIsUncertain();
    void estimateIsUncertain();
    void uncertaintyFollowsTheChain();
    void unknownMixedPriorityIsUncertain();

    // --- Dates ---
    void differentDatesNeverWait();
    void runOverMidnightHoldsTheNextDay();
    void resultKeepsTheTimeZone();

    // --- Reading the limits ---
    void readStorages();
    void readClients();
    void readDirector();
    void readJobsWithJobDefs();
    void refusedReplyLeavesLimitsUnknown();

    // --- Summary and texts ---
    void summaryAndTexts();
    void describeNamesLimitAndRuns();
};

// ============================================================================
// Limits
// ============================================================================

void BScheduleWaitModelTest::storageLimitTenNineParallel()
{
    // The maintainer's case: nine jobs at 23:05 on a storage with
    // Maximum Concurrent Jobs = 10. Nothing waits.
    QList<BWaitRun> runs;
    for (int i = 0; i < 9; ++i)
        runs << run(QStringLiteral("job%1").arg(i), at(16, 23, 5), 30 + i,
                    QStringLiteral("fd%1").arg(i));
    BWaitLimits limits = ample(runs);
    limits.storages.insert(QStringLiteral("File"), 10);
    limits.director = 10;
    for (const BWaitRun &r : runs) {
        limits.jobs.insert(r.job, 1);
        limits.clients.insert(r.client, 1);
    }

    const QList<BWaitResult> results = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(results.size(), 9);
    for (int i = 0; i < 9; ++i) {
        QCOMPARE(results[i].waitSecs, qint64(0));
        QCOMPARE(results[i].start, runs[i].scheduled);
        QVERIFY(results[i].certain);
        QVERIFY(results[i].holds.isEmpty());
    }
}

void BScheduleWaitModelTest::storageLimitOneSerialises()
{
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 20, "fd1")
         << run("b", at(16, 23, 0), 30, "fd2")
         << run("c", at(16, 23, 0), 10, "fd3");
    BWaitLimits limits = ample(runs);
    limits.storages.insert("File", 1);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[0].start, at(16, 23, 0));
    QCOMPARE(r[1].start, at(16, 23, 20));
    QCOMPARE(r[2].start, at(16, 23, 50));
    QCOMPARE(r[1].waitSecs, qint64(20 * 60));
    QCOMPARE(r[2].waitSecs, qint64(50 * 60));
    QVERIFY(r[1].certain && r[2].certain);

    QCOMPARE(r[1].holds.size(), 1);
    QCOMPARE(r[1].holds[0].reason, BWaitHold::Reason::StorageLimit);
    QCOMPARE(r[1].holds[0].resource, QString("File"));
    QCOMPARE(r[1].holds[0].limit, 1);
    QCOMPARE(r[1].holds[0].blockers, QList<int>({0}));
    QCOMPARE(r[1].holds[0].from, at(16, 23, 0));
    QCOMPARE(r[1].holds[0].until, at(16, 23, 20));
    // c was held by a, then by b: one hold, both named
    QCOMPARE(r[2].holds.size(), 1);
    QCOMPARE(r[2].holds[0].blockers, QList<int>({0, 1}));
}

void BScheduleWaitModelTest::clientLimit()
{
    // Two jobs of one client, a third on another client. Storage is free.
    QList<BWaitRun> runs;
    runs << run("a", at(16, 22, 0), 60, "fd1")
         << run("b", at(16, 22, 30), 10, "fd1")
         << run("c", at(16, 22, 30), 10, "fd2");
    BWaitLimits limits = ample(runs);
    limits.clients.insert("fd1", 1);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[1].start, at(16, 23, 0));
    QCOMPARE(r[1].waitSecs, qint64(30 * 60));
    QCOMPARE(r[1].holds[0].reason, BWaitHold::Reason::ClientLimit);
    QCOMPARE(r[1].holds[0].resource, QString("fd1"));
    QCOMPARE(r[2].waitSecs, qint64(0));

    // Two at a time: no wait
    limits.clients.insert("fd1", 2);
    QCOMPARE(BScheduleWaitModel::predict(runs, limits)[1].waitSecs, qint64(0));
}

void BScheduleWaitModelTest::jobLimit()
{
    // An hourly job that runs 90 minutes: the second start waits for the first
    QList<BWaitRun> runs;
    runs << run("hourly", at(16, 1, 0), 90) << run("hourly", at(16, 2, 0), 90);
    BWaitLimits limits = ample(runs);
    limits.jobs.insert("hourly", 1);

    QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[1].start, at(16, 2, 30));
    QCOMPARE(r[1].holds[0].reason, BWaitHold::Reason::JobLimit);
    QCOMPARE(r[1].holds[0].resource, QString("hourly"));
    QCOMPARE(r[1].holds[0].blockers, QList<int>({0}));

    limits.jobs.insert("hourly", 2);
    r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[1].waitSecs, qint64(0));
}

void BScheduleWaitModelTest::directorLimit()
{
    // Three jobs that share nothing, a Director with two slots
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 40, "fd1", "S1")
         << run("b", at(16, 23, 0), 20, "fd2", "S2")
         << run("c", at(16, 23, 0), 10, "fd3", "S3");
    BWaitLimits limits = ample(runs);
    limits.director = 2;

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[0].waitSecs, qint64(0));
    QCOMPARE(r[1].waitSecs, qint64(0));
    QCOMPARE(r[2].start, at(16, 23, 20));
    QCOMPARE(r[2].holds.size(), 1);
    QCOMPARE(r[2].holds[0].reason, BWaitHold::Reason::DirectorLimit);
    QCOMPARE(r[2].holds[0].limit, 2);
    QCOMPARE(r[2].holds[0].blockers, QList<int>({0, 1}));
    QVERIFY(r[2].certain);
}

void BScheduleWaitModelTest::directorLimitKeepsQueueOrder()
{
    // One slot. b and c wait; they start in the order they came in.
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 30, "fd1", "S1")
         << run("b", at(16, 23, 5), 30, "fd2", "S2")
         << run("c", at(16, 23, 1), 30, "fd3", "S3");
    BWaitLimits limits = ample(runs);
    limits.director = 1;

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[2].start, at(16, 23, 30));
    QCOMPARE(r[1].start, at(17, 0, 0));
}

void BScheduleWaitModelTest::otherResourcesOvertake()
{
    // b waits for its client; c came later, needs another client and starts
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 60, "fd1")
         << run("b", at(16, 23, 10), 10, "fd1")
         << run("c", at(16, 23, 20), 10, "fd2");
    BWaitLimits limits = ample(runs);
    limits.clients.insert("fd1", 1);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[2].waitSecs, qint64(0));
    QCOMPARE(r[1].start, at(17, 0, 0));
}

void BScheduleWaitModelTest::copyJobsTakeNoPlace()
{
    // A Copy job is not counted at the client or the storage
    QList<BWaitRun> runs;
    runs << run("backup", at(16, 23, 0), 60) << run("copy", at(16, 23, 10), 10);
    runs[1].usesClient = false;
    runs[1].usesStorage = false;
    BWaitLimits limits = ample(runs);
    limits.clients.insert("fd1", 1);
    limits.storages.insert("File", 1);

    QCOMPARE(BScheduleWaitModel::predict(runs, limits)[1].waitSecs, qint64(0));

    runs[1].usesClient = true;
    QCOMPARE(BScheduleWaitModel::predict(runs, limits)[1].waitSecs, qint64(50 * 60));
}

void BScheduleWaitModelTest::unknownStorageIsUncertain()
{
    // The jobs name no storage (it may come from the Pool): that is not
    // "no limit". Two such runs at the same time may share a storage.
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 30, "fd1", QString())
         << run("b", at(16, 23, 10), 10, "fd2", QString())
         << run("c", at(16, 23, 10), 10, "fd3", "File");
    BWaitLimits limits = ample(runs);
    limits.storages.remove(QString());

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[1].waitSecs, qint64(0));
    QVERIFY(!r[1].certain);
    QCOMPARE(r[1].unknowns.size(), 1);
    QCOMPARE(r[1].unknowns[0].kind, BWaitUnknown::Kind::StorageLimit);
    QVERIFY(r[1].unknowns[0].name.isEmpty());
    QVERIFY(BScheduleWaitModel::describe(1, runs, r).join('\n')
                .contains("the storage of the job is not known"));
    // A run on a storage that is known is not drawn into it
    QVERIFY(r[2].certain);

    // A job that uses no storage at all is certain
    runs[1].usesStorage = false;
    QVERIFY(BScheduleWaitModel::predict(runs, limits)[1].certain);
}

// ============================================================================
// Priority
// ============================================================================

void BScheduleWaitModelTest::lowerPriorityWaitsForRunning()
{
    // Nothing is shared and every limit is ample: only the priority holds
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 30, "fd1", "S1", 10)
         << run("b", at(16, 23, 0), 45, "fd2", "S2", 10)
         << run("low", at(16, 23, 5), 5, "fd3", "S3", 11);
    const BWaitLimits limits = ample(runs);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    // Until the last run of priority 10 has ended
    QCOMPARE(r[2].start, at(16, 23, 45));
    QCOMPARE(r[2].holds.size(), 1);
    QCOMPARE(r[2].holds[0].reason, BWaitHold::Reason::Priority);
    QCOMPARE(r[2].holds[0].otherPriority, 10);
    QCOMPARE(r[2].holds[0].blockers, QList<int>({0, 1}));
    QVERIFY(r[2].certain);

    // Allow Mixed Priority does not help the lower priority
    for (BWaitRun &each : runs) each.allowMixedPriority = true;
    QCOMPARE(BScheduleWaitModel::predict(runs, limits)[2].start, at(16, 23, 45));
}

void BScheduleWaitModelTest::maintainerCatalogBackup()
{
    // From the maintainer's Director: BackupCatalog (priority 11) scheduled
    // for 23:10 started at 00:03, because a Full of priority 10 that took
    // 54 minutes was running. Its start is taken as 23:09 here.
    QList<BWaitRun> runs;
    runs << run("backup-pdc", at(16, 23, 9), 54, "pdc-fd", "File", 10)
         << run("BackupCatalog", at(16, 23, 10), 2, "dir-fd", "File", 11);
    runs[0].level = "Full";
    BWaitLimits limits;
    limits.director = 10;
    limits.storages.insert("File", 10);
    limits.clients.insert("pdc-fd", 1);
    limits.clients.insert("dir-fd", 1);
    limits.jobs.insert("backup-pdc", 1);
    limits.jobs.insert("BackupCatalog", 1);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[0].waitSecs, qint64(0));
    QCOMPARE(r[1].start, at(17, 0, 3));
    QCOMPARE(r[1].waitSecs, qint64(53 * 60));
    QVERIFY(r[1].certain);
    QCOMPARE(r[1].holds.size(), 1);
    QCOMPARE(r[1].holds[0].reason, BWaitHold::Reason::Priority);
    QCOMPARE(r[1].holds[0].blockers, QList<int>({0}));
    QCOMPARE(BScheduleWaitModel::formatWait(r[1].waitSecs), QString("0:53"));
}

void BScheduleWaitModelTest::higherPriorityNeedsMixedOnAll()
{
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 60, "fd1", "S1", 10)
         << run("high", at(16, 23, 10), 5, "fd2", "S2", 5);
    const BWaitLimits limits = ample(runs);

    // Nobody mixes: waits
    QCOMPARE(BScheduleWaitModel::predict(runs, limits)[1].start, at(17, 0, 0));

    // Only the high one mixes: still waits
    runs[1].allowMixedPriority = true;
    QCOMPARE(BScheduleWaitModel::predict(runs, limits)[1].start, at(17, 0, 0));

    // Both mix: starts beside it
    runs[0].allowMixedPriority = true;
    QCOMPARE(BScheduleWaitModel::predict(runs, limits)[1].waitSecs, qint64(0));
}

void BScheduleWaitModelTest::waitingHigherPriorityEndsTheWalk()
{
    // a (10) runs. high (5, no mixing) waits for it. same (10) comes later:
    // it has the wanted priority and free places, but it is behind high in
    // the queue, and the Director stops at high.
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 60, "fd1", "S1", 10)
         << run("high", at(16, 23, 10), 20, "fd2", "S2", 5)
         << run("same", at(16, 23, 20), 5, "fd3", "S3", 10);
    const BWaitLimits limits = ample(runs);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[1].start, at(17, 0, 0));
    QCOMPARE(r[1].holds[0].reason, BWaitHold::Reason::Priority);
    // same: first behind high in the queue, then high (priority 5) is running
    QCOMPARE(r[2].start, at(17, 0, 20));
    QCOMPARE(r[2].holds.size(), 2);
    QCOMPARE(r[2].holds[0].reason, BWaitHold::Reason::QueueOrder);
    QCOMPARE(r[2].holds[0].otherPriority, 5);
    QCOMPARE(r[2].holds[0].blockers, QList<int>({1}));
    QCOMPARE(r[2].holds[0].until, at(17, 0, 0));
    QCOMPARE(r[2].holds[1].reason, BWaitHold::Reason::Priority);
    QCOMPARE(r[2].holds[1].blockers, QList<int>({1}));
}

void BScheduleWaitModelTest::queueIsSortedByPriorityThenTime()
{
    // One place at the storage. While a runs, three wait: priority first,
    // then the scheduled time.
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 30, "fd0", "File", 10)
         << run("late10", at(16, 23, 20), 10, "fd1", "File", 10)
         << run("early10", at(16, 23, 5), 10, "fd2", "File", 10)
         << run("prio9", at(16, 23, 25), 10, "fd3", "File", 9);
    BWaitLimits limits = ample(runs);
    limits.storages.insert("File", 1);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    // At 23:30 a ends and nothing runs: the first in the queue is prio9
    QCOMPARE(r[3].start, at(16, 23, 30));
    QCOMPARE(r[2].start, at(16, 23, 40));
    QCOMPARE(r[1].start, at(16, 23, 50));
}

// ============================================================================
// Certainty
// ============================================================================

void BScheduleWaitModelTest::unknownLimitIsUncertain()
{
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 30, "fd1") << run("b", at(16, 23, 10), 10, "fd1");
    BWaitLimits limits = ample(runs);
    limits.clients.remove("fd1");

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    // Nothing is made up: no wait is predicted, and it is not certain
    QCOMPARE(r[1].waitSecs, qint64(0));
    QVERIFY(!r[1].certain);
    QCOMPARE(r[1].unknowns.size(), 1);
    QCOMPARE(r[1].unknowns[0].kind, BWaitUnknown::Kind::ClientLimit);
    QCOMPARE(r[1].unknowns[0].name, QString("fd1"));
    QCOMPARE(r[1].possibleHolds.size(), 1);
    QCOMPARE(r[1].possibleHolds[0].reason, BWaitHold::Reason::ClientLimit);
    QVERIFY(!r[1].possibleHolds[0].limitKnown);
    // a starts first whatever the limit is
    QVERIFY(r[0].certain);

    // The same for the storage and for the job
    limits = ample(runs);
    limits.storages.clear();
    QList<BWaitResult> again = BScheduleWaitModel::predict(runs, limits);
    QVERIFY(!again[1].certain);
    QCOMPARE(again[1].unknowns[0].kind, BWaitUnknown::Kind::StorageLimit);

    runs[1].job = "a";
    limits = ample(runs);
    limits.jobs.clear();
    again = BScheduleWaitModel::predict(runs, limits);
    QVERIFY(!again[1].certain);
    QCOMPARE(again[1].unknowns[0].kind, BWaitUnknown::Kind::JobLimit);
}

void BScheduleWaitModelTest::unknownLimitWithoutCompanyIsCertain()
{
    // A limit that is not known does not matter where nothing shares it
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 30, "fd1", "S1") << run("b", at(16, 23, 10), 10, "fd2", "S2");
    BWaitLimits limits;
    limits.director = 10;

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QVERIFY(r[0].certain);
    QVERIFY(r[1].certain);
    QCOMPARE(r[1].waitSecs, qint64(0));
}

void BScheduleWaitModelTest::unknownDirectorLimitIsUncertain()
{
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 30, "fd1", "S1") << run("b", at(16, 23, 10), 10, "fd2", "S2");
    BWaitLimits limits = ample(runs);
    limits.director.reset();

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[1].waitSecs, qint64(0));
    QVERIFY(!r[1].certain);
    QCOMPARE(r[1].unknowns.size(), 1);
    QCOMPARE(r[1].unknowns[0].kind, BWaitUnknown::Kind::DirectorLimit);
}

void BScheduleWaitModelTest::noHistoryIsUncertain()
{
    // a was never measured: it has no length. b shares its client.
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 0, "fd1") << run("b", at(16, 23, 30), 10, "fd1")
         << run("c", at(16, 23, 30), 10, "fd2");
    runs[0].durationBasis = BWaitRun::Duration::Unknown;
    BWaitLimits limits = ample(runs);
    limits.clients.insert("fd1", 1);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    // No length is assumed for a: no wait is predicted for b
    QCOMPARE(r[1].waitSecs, qint64(0));
    QVERIFY(!r[1].certain);
    QCOMPARE(r[1].unknowns.size(), 1);
    QCOMPARE(r[1].unknowns[0].kind, BWaitUnknown::Kind::NoHistory);
    QCOMPARE(r[1].unknowns[0].name, QString("a"));
    // a itself and c, which shares nothing with a, are certain
    QVERIFY(r[0].certain);
    QVERIFY(r[2].certain);

    // A run that is due a day and more after a is not marked any more
    runs[1].scheduled = at(17, 23, 30);
    QVERIFY(BScheduleWaitModel::predict(runs, limits)[1].certain);
}

void BScheduleWaitModelTest::estimateIsUncertain()
{
    // a has an estimate from other levels: 20 minutes
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 20, "fd1") << run("b", at(16, 23, 10), 10, "fd1");
    runs[0].durationBasis = BWaitRun::Duration::Estimated;
    BWaitLimits limits = ample(runs);
    limits.clients.insert("fd1", 1);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    // The estimate is used, and the wait is marked
    QCOMPARE(r[1].start, at(16, 23, 20));
    QCOMPARE(r[1].waitSecs, qint64(10 * 60));
    QVERIFY(!r[1].certain);
    QCOMPARE(r[1].unknowns.size(), 1);
    QCOMPARE(r[1].unknowns[0].kind, BWaitUnknown::Kind::EstimatedDuration);
    QCOMPARE(r[1].unknowns[0].name, QString("a"));

    // Measured: certain
    runs[0].durationBasis = BWaitRun::Duration::Measured;
    QVERIFY(BScheduleWaitModel::predict(runs, limits)[1].certain);
}

void BScheduleWaitModelTest::uncertaintyFollowsTheChain()
{
    // c waits for b at the storage, b waits for a at the client, and a is
    // an estimate: the start of c is not certain either, and it names a
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 20, "fd1", "S1")
         << run("b", at(16, 23, 10), 30, "fd1", "S2")
         << run("c", at(16, 23, 25), 10, "fd9", "S2");
    runs[0].durationBasis = BWaitRun::Duration::Estimated;
    BWaitLimits limits = ample(runs);
    limits.clients.insert("fd1", 1);
    limits.storages.insert("S2", 1);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[1].start, at(16, 23, 20));
    QVERIFY(!r[1].certain);
    QCOMPARE(r[2].start, at(16, 23, 50));
    QVERIFY(!r[2].certain);
    BWaitUnknown expected;
    expected.kind = BWaitUnknown::Kind::EstimatedDuration;
    expected.name = "a";
    QVERIFY(r[2].unknowns.contains(expected));
}

void BScheduleWaitModelTest::unknownMixedPriorityIsUncertain()
{
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 60, "fd1", "S1", 10)
         << run("high", at(16, 23, 10), 5, "fd2", "S2", 5);
    runs[0].allowMixedPriority.reset();
    runs[1].allowMixedPriority = true;
    const BWaitLimits limits = ample(runs);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[1].waitSecs, qint64(0));
    QVERIFY(!r[1].certain);
    QCOMPARE(r[1].unknowns.size(), 1);
    QCOMPARE(r[1].unknowns[0].kind, BWaitUnknown::Kind::MixedPriority);
    QCOMPARE(r[1].unknowns[0].name, QString("a"));
}

// ============================================================================
// Dates
// ============================================================================

void BScheduleWaitModelTest::differentDatesNeverWait()
{
    // The same job, client and storage on seven nights, every limit 1, and
    // a job of a lower priority after it every night
    QList<BWaitRun> runs;
    for (int day = 16; day < 23; ++day) {
        runs << run("nightly", at(day, 23, 0), 120, "fd1", "File", 10);
        runs << run("after", at(day, 3, 0), 30, "fd1", "File", 11);
    }
    BWaitLimits limits = ample(runs, 1);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    for (int i = 0; i < r.size(); ++i) {
        QCOMPARE(r[i].waitSecs, qint64(0));
        QVERIFY(r[i].certain);
        QVERIFY(r[i].holds.isEmpty());
    }
}

void BScheduleWaitModelTest::runOverMidnightHoldsTheNextDay()
{
    // What counts is real time, not the date: a run that lasts into the
    // next day holds a run that is due then
    QList<BWaitRun> runs;
    runs << run("long", at(16, 23, 0), 120) << run("next", at(17, 0, 30), 10);
    BWaitLimits limits = ample(runs);
    limits.storages.insert("File", 1);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[1].start, at(17, 1, 0));
}

void BScheduleWaitModelTest::resultKeepsTheTimeZone()
{
    const QTimeZone zone("Europe/Berlin");
    if (!zone.isValid())
        QSKIP("time zone data not available");

    QList<BWaitRun> runs;
    runs << run("a", QDateTime(QDate(2026, 11, 16), QTime(23, 0), zone), 30)
         << run("b", QDateTime(QDate(2026, 11, 16), QTime(23, 10), zone), 10);
    BWaitLimits limits = ample(runs);
    limits.storages.insert("File", 1);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(r[1].start, QDateTime(QDate(2026, 11, 16), QTime(23, 30), zone));
    QCOMPARE(r[1].start.time(), QTime(23, 30));
    QCOMPARE(r[1].holds[0].until.time(), QTime(23, 30));
    QCOMPARE(r[1].holds[0].from.time(), QTime(23, 10));
}

// ============================================================================
// Reading the limits
// ============================================================================

void BScheduleWaitModelTest::readStorages()
{
    // [bareos-source] "show storages": File has the directive, Tape is at
    // the default and does not print it
    BWaitLimits limits;
    limits.readStorages(R"({"jsonrpc":"2.0","id":null,"result":{"storages":{
        "File":{"name":"File","address":"sd.example","password":"x","device":["FileStorage"],
                "mediatype":"File","maximumconcurrentjobs":10},
        "Tape":{"name":"Tape","address":"sd.example","password":"x","device":["LTO"],
                "mediatype":"LTO"}}}})");
    QCOMPARE(limits.storages.value("File", -1), 10);
    QCOMPARE(limits.storages.value("Tape", -1), 1);
    QVERIFY(!limits.storages.contains("Other"));

    // [constructed] other spellings of the key and a number as a string
    limits.readStorages(R"({"result":{"storages":{
        "A":{"name":"A","MaximumConcurrentJobs":"4"},
        "B":{"name":"B","Maximum Concurrent Jobs":7},
        "C":{"name":"C","maximumconcurrentjobs":"many"}}}})");
    QCOMPARE(limits.storages.value("A", -1), 4);
    QCOMPARE(limits.storages.value("B", -1), 7);
    // A value that cannot be read is not replaced by the default
    QVERIFY(!limits.storages.contains("C"));
    // The reply replaced the one before
    QVERIFY(!limits.storages.contains("File"));
}

void BScheduleWaitModelTest::readClients()
{
    BWaitLimits limits;
    limits.readClients(R"({"result":{"clients":{
        "pdc-fd":{"name":"pdc-fd","address":"pdc","password":"x","maximumconcurrentjobs":4},
        "dir-fd":{"name":"dir-fd","address":"dir","password":"x"}}}})");
    QCOMPARE(limits.clients.value("pdc-fd", -1), 4);
    QCOMPARE(limits.clients.value("dir-fd", -1), 1);

    // "show client=<name>": one client, merged
    limits.readClients(R"({"result":{"clients":{
        "new-fd":{"name":"new-fd","address":"new","password":"x","maximumconcurrentjobs":2}}}})");
    QCOMPARE(limits.clients.size(), 3);
    QCOMPARE(limits.clients.value("new-fd", -1), 2);
}

void BScheduleWaitModelTest::readDirector()
{
    BWaitLimits limits;
    limits.readDirector(R"({"result":{"directors":{
        "bareos-dir":{"name":"bareos-dir","querycommand":"x","maximumconcurrentjobs":10,
                      "password":"x","messages":"Daemon"}}}})");
    QVERIFY(limits.director.has_value());
    QCOMPARE(*limits.director, 10);

    // At the default the directive is not printed
    limits.readDirector(R"({"result":{"directors":{"d":{"name":"d","password":"x"}}}})");
    QCOMPARE(limits.director.value_or(-1), 1);

    // [constructed] two Director resources: not understood, not known
    limits.readDirector(R"({"result":{"directors":{"a":{"name":"a"},"b":{"name":"b"}}}})");
    QVERIFY(!limits.director.has_value());
}

void BScheduleWaitModelTest::readJobsWithJobDefs()
{
    QMap<QString, QJsonObject> jobs;
    jobs.insert("own", object(R"({"name":"own","maximumconcurrentjobs":3})"));
    jobs.insert("plain", object(R"({"name":"plain","client":"fd1"})"));
    jobs.insert("inherits", object(R"({"name":"inherits","jobdefs":"Defaults"})"));
    jobs.insert("chain", object(R"({"name":"chain","jobdefs":{"name":"Second"}})"));
    jobs.insert("orphan", object(R"({"name":"orphan","jobdefs":"Missing"})"));

    QMap<QString, QJsonObject> defs;
    defs.insert("Defaults", object(R"({"name":"Defaults","maximumconcurrentjobs":5,
                                       "priority":12,"allowmixedpriority":true})"));
    defs.insert("Second", object(R"({"name":"Second","jobdefs":"Defaults"})"));

    BWaitLimits limits;
    limits.readJobs(jobs, defs);
    QCOMPARE(limits.jobs.value("own", -1), 3);
    QCOMPARE(limits.jobs.value("plain", -1), 1);        // set nowhere: the default
    QCOMPARE(limits.jobs.value("inherits", -1), 5);
    QCOMPARE(limits.jobs.value("chain", -1), 5);
    QVERIFY(!limits.jobs.contains("orphan"));           // its JobDefs was not reported

    bool known = false;
    QJsonValue value = BWaitLimits::jobDirective(jobs, defs, "inherits", "priority", &known);
    QVERIFY(known);
    QCOMPARE(value.toInt(), 12);
    QCOMPARE(BWaitLimits::boolValue(
                 BWaitLimits::jobDirective(jobs, defs, "chain", "allowmixedpriority", &known)),
             std::optional<bool>(true));
    value = BWaitLimits::jobDirective(jobs, defs, "plain", "priority", &known);
    QVERIFY(known);
    QVERIFY(value.isUndefined());
    value = BWaitLimits::jobDirective(jobs, defs, "orphan", "priority", &known);
    QVERIFY(!known);
    value = BWaitLimits::jobDirective(jobs, defs, "nosuchjob", "priority", &known);
    QVERIFY(!known);

    // References: a string, an object with a name, a list (the first counts)
    QCOMPARE(BWaitLimits::resourceName(QJsonValue("File")), QString("File"));
    QCOMPARE(BWaitLimits::resourceName(object(R"({"v":{"name":"File"}})").value("v")), QString("File"));
    QCOMPARE(BWaitLimits::resourceName(object(R"({"v":["File","Tape"]})").value("v")), QString("File"));
    QCOMPARE(BWaitLimits::resourceName(object(R"({"v":[]})").value("v")), QString());

    QCOMPARE(BWaitLimits::boolValue(QJsonValue("yes")), std::optional<bool>(true));
    QCOMPARE(BWaitLimits::boolValue(QJsonValue(false)), std::optional<bool>(false));
    QVERIFY(!BWaitLimits::boolValue(QJsonValue(QJsonValue::Undefined)).has_value());
}

void BScheduleWaitModelTest::refusedReplyLeavesLimitsUnknown()
{
    // [bareos-source, error] a console that may not use "show"
    const QString refused = R"({"jsonrpc":"2.0","id":null,"error":{"code":1,
        "message":"failed","data":{"result":{},"messages":{"error":["show: is an invalid command."]}}}})";

    BWaitLimits limits;
    limits.storages.insert("File", 10);
    limits.director = 10;
    limits.clients.insert("fd1", 2);

    limits.readStorages(refused);
    limits.readDirector(refused);
    limits.readClients(refused);
    QVERIFY(limits.storages.isEmpty());
    QVERIFY(!limits.director.has_value());
    // The client reply shares its route with others: what is known stays
    QCOMPARE(limits.clients.value("fd1", -1), 2);

    limits.readStorages("You have messages.");
    limits.readDirector(QString());
    QVERIFY(limits.storages.isEmpty());
    QVERIFY(!limits.director.has_value());
}

// ============================================================================
// Summary and texts
// ============================================================================

void BScheduleWaitModelTest::summaryAndTexts()
{
    QCOMPARE(BScheduleWaitModel::formatWait(0), QString("0:00"));
    QCOMPARE(BScheduleWaitModel::formatWait(30), QString("0:01"));
    QCOMPARE(BScheduleWaitModel::formatWait(53 * 60), QString("0:53"));
    QCOMPARE(BScheduleWaitModel::formatWait(3 * 3600 + 7 * 60), QString("3:07"));
    QCOMPARE(BScheduleWaitModel::formatWait(26 * 3600), QString("26:00"));

    BWaitSummary sum;
    QCOMPARE(BScheduleWaitModel::summaryText(sum), QString("No waiting predicted"));
    sum.uncertain = 2;
    QCOMPARE(BScheduleWaitModel::summaryText(sum), QString("No waiting predicted, 2 uncertain"));
    sum.waiting = 3;
    sum.longestSecs = 53 * 60;
    QCOMPARE(BScheduleWaitModel::summaryText(sum),
             QString("3 run(s) wait, longest 0:53, 2 uncertain"));
    sum.uncertain = 0;
    QCOMPARE(BScheduleWaitModel::summaryText(sum), QString("3 run(s) wait, longest 0:53"));

    // Counted over the chosen runs only
    QList<BWaitRun> runs;
    runs << run("a", at(16, 23, 0), 20, "fd1") << run("b", at(16, 23, 0), 30, "fd2")
         << run("c", at(16, 23, 0), 10, "fd3");
    BWaitLimits limits = ample(runs);
    limits.storages.insert("File", 1);
    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);

    BWaitSummary all = BScheduleWaitModel::summary(r, {0, 1, 2});
    QCOMPARE(all.runs, 3);
    QCOMPARE(all.waiting, 2);
    QCOMPARE(all.uncertain, 0);
    QCOMPARE(all.longestSecs, qint64(50 * 60));
    QCOMPARE(all.totalSecs, qint64(70 * 60));
    BWaitSummary some = BScheduleWaitModel::summary(r, {0, 1, 7});
    QCOMPARE(some.runs, 2);
    QCOMPARE(some.waiting, 1);
    QCOMPARE(some.longestSecs, qint64(20 * 60));
}

void BScheduleWaitModelTest::describeNamesLimitAndRuns()
{
    QList<BWaitRun> runs;
    runs << run("backup-pdc", at(16, 23, 9), 54, "pdc-fd", "File", 10)
         << run("BackupCatalog", at(16, 23, 10), 2, "dir-fd", "File", 11)
         << run("second", at(16, 23, 20), 5, "pdc-fd", "Other", 10);
    runs[0].level = "Full";
    BWaitLimits limits = ample(runs);
    limits.clients.insert("pdc-fd", 1);

    const QList<BWaitResult> r = BScheduleWaitModel::predict(runs, limits);

    // Nothing to say about a run that does not wait
    QVERIFY(BScheduleWaitModel::describe(0, runs, r).isEmpty());
    QVERIFY(BScheduleWaitModel::reasonLine(runs[0], r[0]).isEmpty());

    // The priority rule, with both priorities and the run it waits for
    const QString priority = BScheduleWaitModel::describe(1, runs, r).join('\n');
    QVERIFY2(priority.contains("Predicted start: 2026-11-17 00:08, 0:58 after the scheduled time (23:10)"),
             qPrintable(priority));
    QVERIFY2(priority.contains("priority 11"), qPrintable(priority));
    QVERIFY2(priority.contains("priority 10"), qPrintable(priority));
    QVERIFY2(priority.contains("backup-pdc (Full, 23:09 to 2026-11-17 00:03)"), qPrintable(priority));
    QVERIFY2(priority.contains("second (Incremental, 2026-11-17 00:03 to 2026-11-17 00:08)"),
             qPrintable(priority));
    QCOMPARE(BScheduleWaitModel::reasonLine(runs[1], r[1]),
             QString("Priority 11, runs of priority 10 are running"));

    // A limit, with its value and the resource
    const QString client = BScheduleWaitModel::describe(2, runs, r).join('\n');
    QVERIFY2(client.contains("client pdc-fd takes 1 job(s) at a time (Maximum Concurrent Jobs = 1)"),
             qPrintable(client));
    QVERIFY2(client.contains("Waits for: backup-pdc (Full, 23:09 to 2026-11-17 00:03)"),
             qPrintable(client));
    QCOMPARE(BScheduleWaitModel::reasonLine(runs[2], r[2]),
             QString("Client limit (pdc-fd: Maximum Concurrent Jobs = 1)"));

    // Uncertain without a wait: says what it depends on and what would hold it
    limits.clients.remove("pdc-fd");
    const QList<BWaitResult> u = BScheduleWaitModel::predict(runs, limits);
    const QString unknown = BScheduleWaitModel::describe(2, runs, u).join('\n');
    QVERIFY2(unknown.contains("Uncertain, this run may have to wait: "
                              "Maximum Concurrent Jobs of client pdc-fd is not known."),
             qPrintable(unknown));
    QVERIFY2(unknown.contains("If so: Client limit: Maximum Concurrent Jobs of client pdc-fd is not known."),
             qPrintable(unknown));
    QCOMPARE(BScheduleWaitModel::reasonLine(runs[2], u[2]),
             QString("May wait: Maximum Concurrent Jobs of client pdc-fd is not known"));
}

QTEST_GUILESS_MAIN(BScheduleWaitModelTest)
#include "bschedulewaitmodel_test.moc"
