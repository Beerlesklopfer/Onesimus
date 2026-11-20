/**
 * @file bliveoverlay_test.cpp
 * @brief The jobs that run or wait now (BLiveJobOverlay), fed with
 *        synthetic push events as odir sends them
 *
 * Runs without a Director and without widgets (registered with CTest).
 */

#include <QtTest>

#include <QTimeZone>

#include "schedules/model/bliveoverlay.h"
#include "schedules/model/bjobhistorystats.h"

namespace {

QDateTime at(int hour, int minute, int second = 0)
{
    return QDateTime(QDate(2026, 11, 20), QTime(hour, minute, second));
}

// A history time as BJobHistoryReply::parseTime() gives it: wall clock
// with the UTC spec
QDateTime wall(int hour, int minute)
{
    return QDateTime(QDate(2026, 11, 20), QTime(hour, minute), QTimeZone::utc());
}

BJobRun historyRun(quint64 id, const QString &name, QChar status,
                   const QDateTime &scheduled, const QDateTime &started = QDateTime())
{
    BJobRun run;
    run.jobId = id;
    run.name = name;
    run.status = status;
    run.type = QLatin1Char('B');
    run.level = QLatin1Char('I');
    run.scheduled = scheduled;
    run.started = started;
    return run;
}

} // namespace

class BLiveOverlayTest : public QObject
{
    Q_OBJECT

private slots:
    void classifiesBothVocabularies();
    void jobOfUniqueName();
    void recognisesStartMessages();
    void queuedStartedEnded();
    void runningEventStartsAWaitingJob();
    void startMessageOfUnknownJob();
    void otherMessagesChangeNothing();
    void cancelingKeepsTheJob();
    void endOfUnknownJobChangesNothing();
    void seedTakesRunningAndWaitingJobs();
    void seedSkipsKnownAndEndedJobs();
    void waitSoFar();
};

void BLiveOverlayTest::classifiesBothVocabularies()
{
    using K = BLiveJobOverlay::StatusKind;
    // odir
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("Queued")), K::Waiting);
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("Success")), K::Ended);
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("Error")), K::Ended);
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("Canceling")), K::Other);
    // the proto comment
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("Running")), K::Running);
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("OK")), K::Ended);
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("Canceled")), K::Ended);
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("WaitingMount")), K::Waiting);
    // catalog codes, case matters
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("R")), K::Running);
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("C")), K::Waiting);
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("T")), K::Ended);
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("f")), K::Ended);
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("F")), K::Waiting);
    QCOMPARE(BLiveJobOverlay::classify(QString()), K::Other);
    QCOMPARE(BLiveJobOverlay::classify(QStringLiteral("something new")), K::Other);
}

void BLiveOverlayTest::jobOfUniqueName()
{
    QCOMPARE(BLiveJobOverlay::jobOfName(QStringLiteral("BackupPDC.2026-11-20_21.00.00_03")),
             QStringLiteral("BackupPDC"));
    QCOMPARE(BLiveJobOverlay::jobOfName(QStringLiteral("BackupPDC")), QStringLiteral("BackupPDC"));
    // A dot in the job name itself stays
    QCOMPARE(BLiveJobOverlay::jobOfName(QStringLiteral("backup.host")), QStringLiteral("backup.host"));
}

void BLiveOverlayTest::recognisesStartMessages()
{
    QVERIFY(BLiveJobOverlay::isStartMessage(QStringLiteral("Start Backup JobId 626, Job=backup-fd.2026-07-09_21.00.00_02\n")));
    QVERIFY(BLiveJobOverlay::isStartMessage(QStringLiteral("Start Restore Job RestoreFiles.2026-07-09_21.00.00_02\n")));
    QVERIFY(BLiveJobOverlay::isStartMessage(QStringLiteral("Start Verify JobId=9 Level=Catalog Job=v.1\n")));
    QVERIFY(BLiveJobOverlay::isStartMessage(QStringLiteral("Start Virtual Backup JobId 3, Job=x\n")));
    QVERIFY(BLiveJobOverlay::isStartMessage(QStringLiteral("bareos-dir JobId 5: Start Backup JobId 5, Job=x\n")));
    QVERIFY(!BLiveJobOverlay::isStartMessage(QStringLiteral("Using Device \"FileStorage\" to write.\n")));
    QVERIFY(!BLiveJobOverlay::isStartMessage(QStringLiteral("Version: 25.2.0 (date)\n")));
}

void BLiveOverlayTest::queuedStartedEnded()
{
    BLiveJobOverlay overlay;
    // Queued at 21:00
    QVERIFY(overlay.statusEvent(42, QStringLiteral("BackupPDC"), QStringLiteral("Queued"), at(21, 0)));
    QCOMPARE(overlay.jobs().size(), 1);
    const BLiveJobOverlay::Job *job = overlay.job(42);
    QVERIFY(job);
    QCOMPARE(job->job, QStringLiteral("BackupPDC"));
    QCOMPARE(job->phase, BLiveJobOverlay::Phase::Waiting);
    QCOMPARE(job->queued, at(21, 0));
    QVERIFY(!job->started.isValid());
    QCOMPARE(job->since(), at(21, 0));

    // It starts at 21:07: odir's first message of the running job
    QVERIFY(overlay.jobMessage(42, QStringLiteral("BackupPDC"),
                               QStringLiteral("Start Backup JobId 42, Job=BackupPDC.2026-11-20_21.00.00_03\n"),
                               at(21, 7)));
    job = overlay.job(42);
    QCOMPARE(job->phase, BLiveJobOverlay::Phase::Running);
    QCOMPARE(job->started, at(21, 7));
    QCOMPARE(job->queued, at(21, 0));
    QCOMPARE(job->waitSecs(at(22, 0)), qint64(7 * 60));

    // A second start message does not move the start
    QVERIFY(!overlay.jobMessage(42, QStringLiteral("BackupPDC"),
                                QStringLiteral("Start Backup JobId 42, Job=x\n"), at(21, 9)));
    QCOMPARE(overlay.job(42)->started, at(21, 7));

    // It ends: gone, and an end of it again changes nothing
    QVERIFY(overlay.statusEvent(42, QStringLiteral("BackupPDC"), QStringLiteral("Success"), at(21, 40)));
    QVERIFY(overlay.isEmpty());
    QVERIFY(!overlay.statusEvent(42, QStringLiteral("BackupPDC"), QStringLiteral("Success"), at(21, 41)));
    // A late message of the ended job does not bring it back
    QVERIFY(!overlay.jobMessage(42, QStringLiteral("BackupPDC"),
                                QStringLiteral("Start Backup JobId 42, Job=x\n"), at(21, 42)));
    QVERIFY(overlay.isEmpty());
}

void BLiveOverlayTest::runningEventStartsAWaitingJob()
{
    BLiveJobOverlay overlay;
    overlay.statusEvent(7, QStringLiteral("J"), QStringLiteral("Queued"), at(1, 0));
    QVERIFY(overlay.statusEvent(7, QStringLiteral("J"), QStringLiteral("Running"), at(1, 2)));
    QCOMPARE(overlay.job(7)->phase, BLiveJobOverlay::Phase::Running);
    QCOMPARE(overlay.job(7)->started, at(1, 2));
    // Waiting for a mount while it runs: it stays a running job
    QVERIFY(overlay.statusEvent(7, QStringLiteral("J"), QStringLiteral("WaitingMount"), at(1, 5)));
    QCOMPARE(overlay.job(7)->phase, BLiveJobOverlay::Phase::Running);
    QCOMPARE(overlay.job(7)->started, at(1, 2));
    QCOMPARE(overlay.job(7)->status, QStringLiteral("WaitingMount"));

    // A running event of a job not seen before: it runs, its wait is unknown
    QVERIFY(overlay.statusEvent(8, QStringLiteral("K"), QStringLiteral("R"), at(1, 3)));
    QCOMPARE(overlay.job(8)->phase, BLiveJobOverlay::Phase::Running);
    QVERIFY(!overlay.job(8)->queued.isValid());
    QCOMPARE(overlay.job(8)->since(), at(1, 3));
    QCOMPARE(overlay.job(8)->waitSecs(at(2, 0)), qint64(-1));
}

void BLiveOverlayTest::startMessageOfUnknownJob()
{
    // Queued before the connection, started after it
    BLiveJobOverlay overlay;
    QVERIFY(overlay.jobMessage(9, QStringLiteral("Nightly.2026-11-20_02.00.00_10"),
                               QStringLiteral("Start Backup JobId 9, Job=Nightly.2026-11-20_02.00.00_10\n"),
                               at(2, 30)));
    QCOMPARE(overlay.job(9)->job, QStringLiteral("Nightly"));
    QCOMPARE(overlay.job(9)->phase, BLiveJobOverlay::Phase::Running);
    QCOMPARE(overlay.job(9)->started, at(2, 30));
}

void BLiveOverlayTest::otherMessagesChangeNothing()
{
    BLiveJobOverlay overlay;
    overlay.statusEvent(3, QStringLiteral("J"), QStringLiteral("Queued"), at(3, 0));
    QVERIFY(!overlay.jobMessage(3, QStringLiteral("J"), QStringLiteral("Connected Client: j-fd\n"), at(3, 1)));
    QCOMPARE(overlay.job(3)->phase, BLiveJobOverlay::Phase::Waiting);
    // Nor do they add a job
    QVERIFY(!overlay.jobMessage(4, QStringLiteral("K"), QStringLiteral("Elapsed time: 1 min\n"), at(3, 1)));
    QVERIFY(!overlay.job(4));
    // Job id 0 is never a job
    QVERIFY(!overlay.statusEvent(0, QStringLiteral("K"), QStringLiteral("Queued"), at(3, 1)));
}

void BLiveOverlayTest::cancelingKeepsTheJob()
{
    BLiveJobOverlay overlay;
    overlay.statusEvent(5, QStringLiteral("J"), QStringLiteral("Queued"), at(4, 0));
    QVERIFY(overlay.statusEvent(5, QStringLiteral("J"), QStringLiteral("Canceling"), at(4, 1)));
    QVERIFY(overlay.job(5));
    QCOMPARE(overlay.job(5)->status, QStringLiteral("Canceling"));
    // "Canceling" of an unknown job adds nothing
    QVERIFY(!overlay.statusEvent(6, QStringLiteral("K"), QStringLiteral("Canceling"), at(4, 1)));
    QVERIFY(overlay.statusEvent(5, QStringLiteral("J"), QStringLiteral("Error"), at(4, 2)));
    QVERIFY(overlay.isEmpty());
}

void BLiveOverlayTest::endOfUnknownJobChangesNothing()
{
    BLiveJobOverlay overlay;
    QVERIFY(!overlay.statusEvent(11, QStringLiteral("J"), QStringLiteral("Success"), at(5, 0)));
    QVERIFY(overlay.isEmpty());
}

void BLiveOverlayTest::seedTakesRunningAndWaitingJobs()
{
    BLiveJobOverlay overlay;
    const QList<BJobRun> runs = {
        historyRun(20, QStringLiteral("Done"), QLatin1Char('T'), wall(1, 0), wall(1, 0)),
        historyRun(21, QStringLiteral("Runs"), QLatin1Char('R'), wall(2, 0), wall(2, 5)),
        historyRun(22, QStringLiteral("Waits"), QLatin1Char('C'), wall(2, 0)),
        // Waiting for a mount after its start: running
        historyRun(23, QStringLiteral("Mounts"), QLatin1Char('M'), wall(2, 0), wall(2, 1)),
        // No time at all: nothing to draw it from
        historyRun(24, QStringLiteral("NoTime"), QLatin1Char('C'), QDateTime()),
        // Start before the scheduled time (copied job): the wait is unknown
        historyRun(25, QStringLiteral("Copy"), QLatin1Char('R'), wall(3, 0), wall(1, 0)),
    };
    QVERIFY(overlay.seed(runs));
    QCOMPARE(overlay.jobs().size(), 4);
    QVERIFY(!overlay.job(20));
    QVERIFY(!overlay.job(24));

    const BLiveJobOverlay::Job *runsJob = overlay.job(21);
    QCOMPARE(runsJob->phase, BLiveJobOverlay::Phase::Running);
    QCOMPARE(runsJob->queued, at(2, 0));          // read as local time
    QCOMPARE(runsJob->started, at(2, 5));
    QVERIFY(runsJob->seeded);

    QCOMPARE(overlay.job(22)->phase, BLiveJobOverlay::Phase::Waiting);
    QCOMPARE(overlay.job(22)->queued, at(2, 0));
    QCOMPARE(overlay.job(23)->phase, BLiveJobOverlay::Phase::Running);
    QCOMPARE(overlay.job(23)->started, at(2, 1));

    QVERIFY(!overlay.job(25)->queued.isValid());
    QCOMPARE(overlay.job(25)->since(), at(1, 0));
}

void BLiveOverlayTest::seedSkipsKnownAndEndedJobs()
{
    BLiveJobOverlay overlay;
    // Known from an event: the event's times stay
    overlay.statusEvent(30, QStringLiteral("A"), QStringLiteral("Queued"), at(6, 1));
    // Ended after the history was produced, before its reply arrived
    overlay.statusEvent(31, QStringLiteral("B"), QStringLiteral("Queued"), at(6, 0));
    overlay.statusEvent(31, QStringLiteral("B"), QStringLiteral("Success"), at(6, 30));

    const QList<BJobRun> runs = {
        historyRun(30, QStringLiteral("A"), QLatin1Char('C'), wall(6, 0)),
        historyRun(31, QStringLiteral("B"), QLatin1Char('R'), wall(6, 0), wall(6, 2)),
    };
    QVERIFY(!overlay.seed(runs));
    QCOMPARE(overlay.jobs().size(), 1);
    QCOMPARE(overlay.job(30)->queued, at(6, 1));
    QVERIFY(!overlay.job(30)->seeded);

    // After clear() nothing is remembered
    overlay.clear();
    QVERIFY(overlay.seed(runs));
    QCOMPARE(overlay.jobs().size(), 2);
}

void BLiveOverlayTest::waitSoFar()
{
    BLiveJobOverlay::Job job;
    job.queued = at(21, 0);
    QCOMPARE(job.waitSecs(at(21, 10)), qint64(600));
    job.started = at(21, 4);
    QCOMPARE(job.waitSecs(at(21, 10)), qint64(240));
    // A clock that is behind gives no negative wait
    BLiveJobOverlay::Job early;
    early.queued = at(21, 0);
    QCOMPARE(early.waitSecs(at(20, 59)), qint64(0));
}

QTEST_GUILESS_MAIN(BLiveOverlayTest)
#include "bliveoverlay_test.moc"
