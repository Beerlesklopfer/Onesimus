/**
 * @file bwaitstagger_test.cpp
 * @brief Later start times for jobs that wait for each other (BWaitStagger)
 *
 * The runs are constructed; the queue is BScheduleWaitModel's replay.
 * Runs without a Director and without widgets (registered with CTest).
 */

#include <QtTest>

#include <QTimeZone>

#include "schedules/model/bwaitstagger.h"

namespace {

QDateTime at(int day, int hour, int minute)
{
    return QDateTime(QDate(2026, 11, day), QTime(hour, minute), QTimeZone::utc());
}

BWaitRun run(const QString &job, const QDateTime &scheduled, qint64 minutes, int priority = 10,
             const QString &client = QString())
{
    BWaitRun r;
    r.id = job;
    r.job = job;
    r.client = client.isEmpty() ? job + QStringLiteral("-fd") : client;
    r.storage = QStringLiteral("File");
    r.level = QStringLiteral("Incremental");
    r.priority = priority;
    r.allowMixedPriority = false;
    r.scheduled = scheduled;
    r.durationBasis = BWaitRun::Duration::Measured;
    r.durationSecs = minutes * 60;
    return r;
}

/// One run at a time at the storage, everything else ample
BWaitLimits oneAtTheStorage(const QList<BWaitRun> &runs)
{
    BWaitLimits limits;
    limits.director = 20;
    for (const BWaitRun &r : runs) {
        limits.jobs.insert(r.job, 1);
        limits.clients.insert(r.client, 10);
    }
    limits.storages.insert(QStringLiteral("File"), 1);
    return limits;
}

const BWaitStagger::Shift *shiftOf(const BWaitStagger::Proposal &p, const QString &job)
{
    for (const BWaitStagger::Shift &s : p.shifts) {
        if (s.job == job) return &s;
    }
    return nullptr;
}

} // namespace

class BWaitStaggerTest : public QObject
{
    Q_OBJECT

private slots:
    void twoRunsAtOneStorageAreSpread();
    void threeRunsEachGetTheirTurn();
    void catalogStaysLast();
    void waitForPriorityIsNotMoved();
    void jobThatMayNotMoveStays();
    void runsOfTheDayBeforeAreNotCounted();
    void deviceWaitCounts();
};

void BWaitStaggerTest::twoRunsAtOneStorageAreSpread()
{
    const QList<BWaitRun> runs = {run("a", at(20, 23, 5), 10), run("b", at(20, 23, 5), 10)};
    const BWaitStagger::Proposal p =
        BWaitStagger::propose(runs, oneAtTheStorage(runs), at(20, 0, 0));
    QCOMPARE(p.totalBeforeSecs, qint64(600));
    QCOMPARE(p.totalAfterSecs, qint64(0));
    QCOMPARE(p.moved(), 1);
    // The one that waited moves exactly as far as the other runs: no further
    QCOMPARE(shiftOf(p, "a")->minutes, 0);
    QCOMPARE(shiftOf(p, "b")->minutes, 10);
    QCOMPARE(shiftOf(p, "b")->waitBeforeSecs, qint64(600));
    QCOMPARE(shiftOf(p, "b")->waitAfterSecs, qint64(0));
    QCOMPARE(p.runs[1].scheduled, at(20, 23, 15));
    QVERIFY(!p.results[1].waits());
}

void BWaitStaggerTest::threeRunsEachGetTheirTurn()
{
    const QList<BWaitRun> runs = {run("a", at(20, 23, 5), 4), run("b", at(20, 23, 5), 4),
                                  run("c", at(20, 23, 5), 4)};
    const BWaitStagger::Proposal p =
        BWaitStagger::propose(runs, oneAtTheStorage(runs), at(20, 0, 0));
    QCOMPARE(p.totalBeforeSecs, qint64(4 * 60 + 8 * 60));
    QCOMPARE(p.totalAfterSecs, qint64(0));
    QList<int> minutes;
    for (const BWaitStagger::Shift &s : p.shifts) minutes.append(s.minutes);
    std::sort(minutes.begin(), minutes.end());
    QCOMPARE(minutes, (QList<int>{0, 4, 8}));
}

void BWaitStaggerTest::catalogStaysLast()
{
    // The catalog backup five minutes later, at a lower priority: the
    // backups may move up to its moment, not beyond it
    const QList<BWaitRun> runs = {run("a", at(20, 23, 5), 10), run("b", at(20, 23, 5), 10),
                                  run("catalog", at(20, 23, 10), 1, 11)};
    const BWaitStagger::Proposal p =
        BWaitStagger::propose(runs, oneAtTheStorage(runs), at(20, 0, 0));
    const BWaitStagger::Shift *b = shiftOf(p, "b");
    QVERIFY(b);
    QCOMPARE(b->boundMinutes, 5);
    QCOMPARE(b->boundJob, QString("catalog"));
    QCOMPARE(b->boundAt, at(20, 23, 10));
    QCOMPARE(b->minutes, 5);
    QCOMPARE(b->waitAfterSecs, qint64(300));
    QCOMPARE(shiftOf(p, "catalog")->minutes, 0);
    // Still in that order: the catalog starts after both backups
    const QDateTime catalogStart = p.results[2].start;
    QVERIFY(catalogStart >= p.results[0].end);
    QVERIFY(catalogStart >= p.results[1].end);
}

void BWaitStaggerTest::waitForPriorityIsNotMoved()
{
    const QList<BWaitRun> runs = {run("a", at(20, 23, 5), 10),
                                  run("catalog", at(20, 23, 5), 1, 11)};
    BWaitLimits limits = oneAtTheStorage(runs);
    limits.storages.insert(QStringLiteral("File"), 10);
    const BWaitStagger::Proposal p = BWaitStagger::propose(runs, limits, at(20, 0, 0));
    QVERIFY(p.results.value(1).waits());     // it waits, for the priority
    QCOMPARE(p.totalBeforeSecs, qint64(0));
    QCOMPARE(p.moved(), 0);
}

void BWaitStaggerTest::jobThatMayNotMoveStays()
{
    const QList<BWaitRun> runs = {run("a", at(20, 23, 5), 10), run("b", at(20, 23, 5), 10)};
    BWaitStagger::Options options;
    options.jobMaxMinutes.insert(QStringLiteral("b"), 0);
    const BWaitStagger::Proposal p =
        BWaitStagger::propose(runs, oneAtTheStorage(runs), at(20, 0, 0), options);
    // b may not move; a moving does not help b, which then waits for nothing
    // longer, and a starts first anyway
    QCOMPARE(shiftOf(p, "b")->minutes, 0);
    QCOMPARE(shiftOf(p, "b")->boundMinutes, 0);
    QCOMPARE(p.totalAfterSecs, p.totalBeforeSecs);
    QCOMPARE(p.moved(), 0);
}

void BWaitStaggerTest::runsOfTheDayBeforeAreNotCounted()
{
    const QList<BWaitRun> runs = {run("a", at(19, 23, 5), 10), run("b", at(19, 23, 5), 10)};
    const BWaitStagger::Proposal p =
        BWaitStagger::propose(runs, oneAtTheStorage(runs), at(20, 0, 0));
    QCOMPARE(p.totalBeforeSecs, qint64(0));
    QCOMPARE(p.moved(), 0);
}

void BWaitStaggerTest::deviceWaitCounts()
{
    // Two places at the storage, one writer at its device: the second run
    // starts and waits inside the storage daemon
    const QList<BWaitRun> runs = {run("a", at(20, 23, 5), 10), run("b", at(20, 23, 5), 10)};
    BWaitLimits limits = oneAtTheStorage(runs);
    limits.storages.insert(QStringLiteral("File"), 2);
    limits.devices.insert(QStringLiteral("File"), 1);
    const BWaitStagger::Proposal p = BWaitStagger::propose(runs, limits, at(20, 0, 0));
    QVERIFY(p.totalBeforeSecs > 0);
    QVERIFY2(p.totalAfterSecs < p.totalBeforeSecs,
             qPrintable(QStringLiteral("%1 -> %2").arg(p.totalBeforeSecs).arg(p.totalAfterSecs)));
    QCOMPARE(p.moved(), 1);
}

QTEST_GUILESS_MAIN(BWaitStaggerTest)
#include "bwaitstagger_test.moc"
