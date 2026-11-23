// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

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
    void gridOfAQuarterHour();
    void takenAlongNoFurtherThanItsBound();
    void waitForPriorityIsNotMoved();
    void jobThatMayNotMoveStays();
    void runsOfTheDayBeforeAreNotCounted();
    void deviceWaitCounts();
    void fixedJobIsNotMoved();
    void fixedLowerPriorityBlocksThePassing();
    void latestStartBoundsTheOffset();
    void latestStartAlreadyPassedStays();
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
    // The catalog backup five minutes later, at a lower priority: a backup
    // that moves past it takes it along, so that it still runs last
    const QList<BWaitRun> runs = {run("a", at(20, 23, 5), 10), run("b", at(20, 23, 5), 10),
                                  run("catalog", at(20, 23, 10), 1, 11)};
    const BWaitStagger::Proposal p =
        BWaitStagger::propose(runs, oneAtTheStorage(runs), at(20, 0, 0));
    QCOMPARE(p.totalAfterSecs, qint64(0));
    QCOMPARE(shiftOf(p, "b")->minutes, 10);
    QVERIFY(shiftOf(p, "b")->follows.isEmpty());
    QCOMPARE(shiftOf(p, "catalog")->minutes, 5);           // 23:15, with b
    QCOMPARE(shiftOf(p, "catalog")->follows, QString("b"));
    const QDateTime catalogStart = p.results[2].start;
    QVERIFY(catalogStart >= p.results[0].end);
    QVERIFY(catalogStart >= p.results[1].end);
}

void BWaitStaggerTest::gridOfAQuarterHour()
{
    BWaitStagger::Options options;
    options.gridMinutes = 15;
    // 23:05: the next places are 23:15, 23:30, 23:45
    const QList<BWaitRun> two = {run("a", at(20, 23, 5), 10), run("b", at(20, 23, 5), 10)};
    BWaitStagger::Proposal p = BWaitStagger::propose(two, oneAtTheStorage(two), at(20, 0, 0), options);
    QCOMPARE(p.totalAfterSecs, qint64(0));
    QCOMPARE(shiftOf(p, "b")->minutes, 10);
    QCOMPARE(p.runs[1].scheduled, at(20, 23, 15));

    // Three of 20 minutes: one stays, the others go to the next free places
    const QList<BWaitRun> three = {run("a", at(20, 23, 5), 20), run("b", at(20, 23, 5), 20),
                                   run("c", at(20, 23, 5), 20)};
    p = BWaitStagger::propose(three, oneAtTheStorage(three), at(20, 0, 0), options);
    QCOMPARE(p.totalAfterSecs, qint64(0));
    for (const BWaitRun &r : p.runs) {
        QVERIFY2(r.scheduled.time() == QTime(23, 5) || r.scheduled.time().minute() % 15 == 0,
                 qPrintable(r.scheduled.toString()));
    }
    QList<int> minutes;
    for (const BWaitStagger::Shift &s : p.shifts) minutes.append(s.minutes);
    std::sort(minutes.begin(), minutes.end());
    // 23:05 to 23:25, 23:30 to 23:50, then 00:00 (23:45 is taken)
    QCOMPARE(minutes, (QList<int>{0, 25, 55}));

    // The catalog at 23:10 goes along to the latest of them, on its grid
    const QList<BWaitRun> withCatalog = {run("a", at(20, 23, 5), 10), run("b", at(20, 23, 5), 10),
                                         run("catalog", at(20, 23, 10), 1, 11)};
    p = BWaitStagger::propose(withCatalog, oneAtTheStorage(withCatalog), at(20, 0, 0), options);
    QCOMPARE(shiftOf(p, "b")->minutes, 10);
    QCOMPARE(p.runs[2].scheduled, at(20, 23, 15));
    QVERIFY(p.results[2].start >= p.results[1].end);
}

void BWaitStaggerTest::takenAlongNoFurtherThanItsBound()
{
    // The catalog may not move: b may then not pass it
    const QList<BWaitRun> runs = {run("a", at(20, 23, 5), 10), run("b", at(20, 23, 5), 10),
                                  run("catalog", at(20, 23, 10), 1, 11)};
    BWaitStagger::Options options;
    options.jobMaxMinutes.insert(QStringLiteral("catalog"), 0);
    const BWaitStagger::Proposal p =
        BWaitStagger::propose(runs, oneAtTheStorage(runs), at(20, 0, 0), options);
    QCOMPARE(shiftOf(p, "catalog")->minutes, 0);
    QVERIFY(shiftOf(p, "b")->minutes <= 5);
    QVERIFY(p.runs[1].scheduled <= p.runs[2].scheduled);
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

void BWaitStaggerTest::fixedJobIsNotMoved()
{
    // Without the constraint b moves 10 minutes (twoRunsAtOneStorageAreSpread)
    const QList<BWaitRun> runs = {run("a", at(20, 23, 5), 10), run("b", at(20, 23, 5), 10)};
    BWaitStagger::Options options;
    options.fixedJobs.insert(QStringLiteral("b"));
    const BWaitStagger::Proposal p =
        BWaitStagger::propose(runs, oneAtTheStorage(runs), at(20, 0, 0), options);
    QCOMPARE(shiftOf(p, "b")->minutes, 0);
    QVERIFY(shiftOf(p, "b")->fixed);
    QCOMPARE(shiftOf(p, "b")->boundMinutes, 0);
    QVERIFY(!shiftOf(p, "a")->fixed);
    QCOMPARE(p.moved(), 0);
    QCOMPARE(p.totalAfterSecs, p.totalBeforeSecs);
}

void BWaitStaggerTest::fixedLowerPriorityBlocksThePassing()
{
    // The catalog at 23:10 is fixed: b may not pass it, since it would have
    // to take it along; without the constraint b moves 10 and the catalog 5
    // (catalogStaysLast)
    const QList<BWaitRun> runs = {run("a", at(20, 23, 5), 10), run("b", at(20, 23, 5), 10),
                                  run("catalog", at(20, 23, 10), 1, 11)};
    BWaitStagger::Options options;
    options.fixedJobs.insert(QStringLiteral("catalog"));
    const BWaitStagger::Proposal p =
        BWaitStagger::propose(runs, oneAtTheStorage(runs), at(20, 0, 0), options);
    QCOMPARE(shiftOf(p, "catalog")->minutes, 0);
    QVERIFY(shiftOf(p, "catalog")->fixed);
    QVERIFY(shiftOf(p, "b")->minutes <= 5);
    QVERIFY(shiftOf(p, "b")->follows.isEmpty());
    for (const BWaitRun &r : p.runs) {
        if (r.job != QLatin1String("catalog")) QVERIFY(r.scheduled <= at(20, 23, 10));
    }
}

void BWaitStaggerTest::latestStartBoundsTheOffset()
{
    // Three of 20 minutes at 23:05 on a quarter hour grid: unbounded, two of
    // them go to 23:30 and 23:45 (gridOfAQuarterHour). c may not start after
    // 23:30.
    const QList<BWaitRun> runs = {run("a", at(20, 23, 5), 20), run("b", at(20, 23, 5), 20),
                                  run("c", at(20, 23, 5), 20)};
    BWaitStagger::Options options;
    options.gridMinutes = 15;
    options.latestStartMinutes.insert(QStringLiteral("c"), 23 * 60 + 30);
    const BWaitStagger::Proposal p =
        BWaitStagger::propose(runs, oneAtTheStorage(runs), at(20, 0, 0), options);
    const BWaitStagger::Shift *c = shiftOf(p, "c");
    QCOMPARE(c->boundMinutes, 25);
    QCOMPARE(c->latestStartMinute, 23 * 60 + 30);
    QVERIFY(c->minutes <= 25);
    QVERIFY(p.runs[2].scheduled <= at(20, 23, 30));
    QCOMPARE(shiftOf(p, "a")->latestStartMinute, -1);

    // Only one place left before 23:30: c alone is bounded to 23:15
    options.latestStartMinutes.insert(QStringLiteral("c"), 23 * 60 + 20);
    const BWaitStagger::Proposal q =
        BWaitStagger::propose(runs, oneAtTheStorage(runs), at(20, 0, 0), options);
    QCOMPARE(shiftOf(q, "c")->boundMinutes, 15);
    QVERIFY(q.runs[2].scheduled <= at(20, 23, 20));
}

void BWaitStaggerTest::latestStartAlreadyPassedStays()
{
    const QList<BWaitRun> runs = {run("a", at(20, 23, 5), 10), run("b", at(20, 23, 5), 10)};
    BWaitStagger::Options options;
    options.latestStartMinutes.insert(QStringLiteral("b"), 22 * 60);
    options.latestStartMinutes.insert(QStringLiteral("a"), 22 * 60);
    const BWaitStagger::Proposal p =
        BWaitStagger::propose(runs, oneAtTheStorage(runs), at(20, 0, 0), options);
    QCOMPARE(p.moved(), 0);
    QCOMPARE(shiftOf(p, "b")->boundMinutes, 0);
    QCOMPARE(shiftOf(p, "b")->latestStartMinute, 22 * 60);
}

QTEST_GUILESS_MAIN(BWaitStaggerTest)
#include "bwaitstagger_test.moc"
