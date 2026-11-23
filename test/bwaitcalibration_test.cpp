// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bwaitcalibration_test.cpp
 * @brief The wait prediction measured against the catalog (BWaitCalibration)
 *
 * Fixture: the table the maintainer measured on his production Director
 * (catalog of the last 8 days, wait = StartTime - SchedTime, run = EndTime
 * - StartTime; mean values). It is not a catalog dump: from the table the
 * runs of seven Incremental nights are constructed, every job scheduled at
 * 23:05 (BackupCatalog at 23:10, priority 11), each run starting its mean
 * wait after the scheduled time and lasting its mean run. Because the means
 * come from different nights, the constructed nights are not quite
 * consistent (tapir-ackervielfalt and tapir-shared overlap on one client);
 * that is left as it is.
 *
 * Setup of that Director: Maximum Concurrent Jobs 20, storage PDC-sd 10,
 * every client and job 1, one File device that writes one job at a time
 * (the Director cannot read that), Just In Time Reservation.
 *
 * Runs without a Director and without widgets (registered with CTest).
 */

#include <QtTest>

#include <QTimeZone>

#include "schedules/model/bwaitcalibration.h"

namespace {

const QString kStorage = QStringLiteral("PDC-sd");

struct Row
{
    const char *job;
    const char *client;
    int waitSecs;   // mean wait at Incremental (Full for BackupCatalog)
    int runSecs;    // mean run
    int priority;
};

// From the table (Incremental where the table has it)
const Row kTable[] = {
    {"BackupMail",          "CLOUD-fd",    6,   55, 10},
    {"BackupPassbolt",      "CLOUD-fd",    75,  30, 10},
    {"BackupNextcloud",     "CLOUD-fd",    285, 38, 10},
    {"forgejo",             "ForceIEC-fd", 3,   16, 10},
    {"tapir-ackervielfalt", "ForceIEC-fd", 39,  45, 10},
    {"tapir-shared",        "ForceIEC-fd", 77,  12, 10},
    {"Samba4-Weekly",       "PDC-fd",      3,   0,  10},
    {"BackupEleLa",         "PDC-fd",      4,   5,  10},
    {"BackupBgLage",        "BGLAGE-fd",   4,   155, 10},
    {"BackupCatalog",       "PDC-fd",      1073, 5, 11},
};

QDateTime at(const QDate &date, int hour, int minute)
{
    return QDateTime(date, QTime(hour, minute), QTimeZone::utc());
}

QString levelOf(const Row &row)
{
    return row.priority == 11 ? QStringLiteral("Full") : QStringLiteral("Incremental");
}

/// Seven nights, 2026-11-10 to 2026-11-16, from the table
QList<BJobRun> fixtureHistory()
{
    QList<BJobRun> runs;
    quint64 id = 1000;
    for (int n = 0; n < 7; ++n) {
        const QDate date(2026, 11, 10 + n);
        for (const Row &row : kTable) {
            BJobRun run;
            run.jobId = ++id;
            run.name = QString::fromLatin1(row.job);
            run.client = QString::fromLatin1(row.client);
            run.type = QLatin1Char('B');
            run.level = row.priority == 11 ? QLatin1Char('F') : QLatin1Char('I');
            run.status = QLatin1Char('T');
            run.scheduled = row.priority == 11 ? at(date, 23, 10) : at(date, 23, 5);
            run.started = run.scheduled.addSecs(row.waitSecs);
            run.ended = run.started.addSecs(row.runSecs);
            runs.append(run);
        }
    }
    return runs;
}

QHash<QString, BWaitJobInfo> fixtureJobs()
{
    QHash<QString, BWaitJobInfo> jobs;
    for (const Row &row : kTable) {
        BWaitJobInfo info;
        info.client = QString::fromLatin1(row.client);
        info.storage = kStorage;
        info.priority = row.priority;
        jobs.insert(QString::fromLatin1(row.job), info);
    }
    return jobs;
}

BWaitLimits fixtureLimits(int deviceWriters)
{
    BWaitLimits limits;
    limits.director = 20;
    limits.storages.insert(kStorage, 10);
    for (const Row &row : kTable) {
        limits.clients.insert(QString::fromLatin1(row.client), 1);
        limits.jobs.insert(QString::fromLatin1(row.job), 1);
    }
    if (deviceWriters > 0)
        limits.devices.insert(kStorage, deviceWriters);
    return limits;
}

/// The runs of the night to predict, 2026-11-19, as the schedule view
/// builds them, calibrated with @p calibration (nullptr: as before this
/// calibration existed, measured durations as they are, no overhead)
QList<BWaitRun> nightToPredict(const BWaitCalibration *calibration)
{
    QList<BWaitRun> runs;
    const QDate date(2026, 11, 19);
    for (const Row &row : kTable) {
        BWaitRun run;
        run.id = QString::fromLatin1(row.job);
        run.job = run.id;
        run.client = QString::fromLatin1(row.client);
        run.storage = kStorage;
        run.level = levelOf(row);
        run.priority = row.priority;
        run.allowMixedPriority = false;
        run.scheduled = row.priority == 11 ? at(date, 23, 10) : at(date, 23, 5);
        run.durationBasis = BWaitRun::Duration::Measured;
        run.durationSecs = qMax(1, row.runSecs);
        if (calibration)
            calibration->apply(&run);
        runs.append(run);
    }
    if (calibration)
        BWaitCalibration::orderTies(&runs);
    return runs;
}

int indexOf(const QList<BWaitRun> &runs, const char *job)
{
    for (int i = 0; i < runs.size(); ++i) {
        if (runs[i].job == QLatin1String(job)) return i;
    }
    return -1;
}

BJobRun simpleRun(const QString &job, const QDateTime &scheduled, int waitSecs, int runSecs,
                  const QString &client)
{
    BJobRun run;
    run.name = job;
    run.client = client;
    run.type = QLatin1Char('B');
    run.level = QLatin1Char('I');
    run.status = QLatin1Char('T');
    run.scheduled = scheduled;
    run.started = scheduled.addSecs(waitSecs);
    run.ended = run.started.addSecs(runSecs);
    return run;
}

} // namespace

class BWaitCalibrationTest : public QObject
{
    Q_OBJECT

private slots:
    void measuredWaitsFromHistory();
    void startOverheadFromHistory();
    void pureDurationWithDeviceLimit();
    void pureDurationWithoutDeviceLimit();
    void predictionWithoutDeviceIsTooLow();
    void predictionWithOneWriterMatchesMeasured();
    void unknownDeviceIsUncertainAndNamed();
    void backtestOnFixture();
    void suggestionNoOverlapSuggestsOne();
    void suggestionSerialEndsSuggestOne();
    void suggestionParallelGivesNone();
    void suggestionOnFixtureCannotTell();
    void deviceRetryEveryThirtySeconds();
    void historyParsesClient();
};

void BWaitCalibrationTest::measuredWaitsFromHistory()
{
    BWaitCalibration cal;
    cal.setRuns(fixtureHistory(), fixtureJobs(), {});
    const BWaitMeasured nextcloud = cal.measuredWait(QStringLiteral("BackupNextcloud"),
                                                     QStringLiteral("Incremental"));
    QCOMPARE(nextcloud.count, 7);
    QCOMPARE(nextcloud.medianSecs, qint64(285));
    QCOMPARE(nextcloud.maxSecs, qint64(285));
    QVERIFY(!cal.measuredWait(QStringLiteral("BackupNextcloud"), QStringLiteral("Full")).isValid());
    QCOMPARE(BScheduleWaitModel::measuredText(nextcloud),
             QStringLiteral("median 4:45 min, max 4:45 min (7 run(s))"));
}

void BWaitCalibrationTest::startOverheadFromHistory()
{
    BWaitCalibration cal;
    cal.setRuns(fixtureHistory(), fixtureJobs(), {});
    // First on its client: its whole wait
    QCOMPARE(cal.startOverhead(QStringLiteral("BackupMail"), QStringLiteral("Incremental")).secs,
             qint64(6));
    // Behind BackupMail (ends 23:06:01, starts 23:06:15)
    QCOMPARE(cal.startOverhead(QStringLiteral("BackupPassbolt"), QStringLiteral("Incremental")).secs,
             qint64(14));
    // Behind BackupPassbolt (ends 23:06:45, starts 23:09:45): three minutes
    // that are no queue - Client Run Before Job, by the look of it
    const BWaitCalibration::Overhead nextcloud =
        cal.startOverhead(QStringLiteral("BackupNextcloud"), QStringLiteral("Incremental"));
    QCOMPARE(nextcloud.secs, qint64(180));
    QCOMPARE(nextcloud.basis, BWaitCalibration::Basis::JobLevel);
    QCOMPARE(nextcloud.samples, 7);
    // Nothing of the job: the Director's median; nothing at all: the default
    QCOMPARE(cal.startOverhead(QStringLiteral("unknown"), QString()).basis,
             BWaitCalibration::Basis::Director);
    BWaitCalibration empty;
    QCOMPARE(empty.startOverhead(QStringLiteral("x"), QString()).secs,
             BWaitCalibration::DefaultStartOverheadSecs);
}

void BWaitCalibrationTest::pureDurationWithDeviceLimit()
{
    BWaitCalibration cal;
    cal.setRuns(fixtureHistory(), fixtureJobs(), {{kStorage, 1}});
    // BackupMail 23:05:06 to 23:06:01; forgejo ended 23:05:19 meanwhile
    const BWaitCalibration::Pure mail =
        cal.pureDuration(QStringLiteral("BackupMail"), QStringLiteral("Incremental"));
    QVERIFY(mail.valid);
    QCOMPARE(mail.rawSecs, qint64(55));
    QCOMPARE(mail.secs, qint64(42));
    QCOMPARE(mail.deviceLimit, 1);
    // BackupBgLage 23:05:04 to 23:07:39; BackupPassbolt ended 23:06:45
    QCOMPARE(cal.pureDuration(QStringLiteral("BackupBgLage"), QStringLiteral("Incremental")).secs,
             qint64(54));
    // BackupNextcloud ran alone: as measured
    QCOMPARE(cal.pureDuration(QStringLiteral("BackupNextcloud"), QStringLiteral("Incremental")).secs,
             qint64(38));
}

void BWaitCalibrationTest::pureDurationWithoutDeviceLimit()
{
    BWaitCalibration cal;
    cal.setRuns(fixtureHistory(), fixtureJobs(), {});
    const BWaitCalibration::Pure mail =
        cal.pureDuration(QStringLiteral("BackupMail"), QStringLiteral("Incremental"));
    QCOMPARE(mail.secs, qint64(55));
    QCOMPARE(mail.deviceLimit, 0);
    QVERIFY(BWaitCalibration::durationNote(mail, kStorage).contains(QStringLiteral("not taken out")));
}

void BWaitCalibrationTest::predictionWithoutDeviceIsTooLow()
{
    // The model as it was: catalog durations, no device, no overhead. This
    // is the "about a minute" the maintainer saw.
    const QList<BWaitRun> runs = nightToPredict(nullptr);
    const QList<BWaitResult> results = BScheduleWaitModel::predict(runs, fixtureLimits(0));
    const int passbolt = indexOf(runs, "BackupPassbolt");
    const int nextcloud = indexOf(runs, "BackupNextcloud");
    QCOMPARE(results[passbolt].waitSecs, qint64(55));
    QCOMPARE(results[nextcloud].waitSecs, qint64(85));

    // Given the measured waits, the list flags it
    BWaitRun withMeasured = runs[nextcloud];
    withMeasured.measuredWait.count = 6;
    withMeasured.measuredWait.medianSecs = 285;
    withMeasured.measuredWait.maxSecs = 415;
    QCOMPARE(BScheduleWaitModel::discrepancy(withMeasured, results[nextcloud]),
             BScheduleWaitModel::Discrepancy::TooLow);
    QVERIFY(BScheduleWaitModel::discrepancyText(withMeasured, results[nextcloud])
                .startsWith(QStringLiteral("Prediction too low: measured median 4:45 min, "
                                           "predicted 1:25 min")));
}

void BWaitCalibrationTest::predictionWithOneWriterMatchesMeasured()
{
    BWaitCalibration cal;
    cal.setRuns(fixtureHistory(), fixtureJobs(), {{kStorage, 1}});
    const QList<BWaitRun> runs = nightToPredict(&cal);
    const QList<BWaitResult> results = BScheduleWaitModel::predict(runs, fixtureLimits(1));

    for (int i = 0; i < runs.size(); ++i) {
        qInfo("%-20s predicted %5lld s  measured %5lld s  device wait %3lld s%s",
              qPrintable(runs[i].job), results[i].waitSecs, runs[i].measuredWait.medianSecs,
              results[i].deviceWaitSecs, results[i].certain ? "" : "  (uncertain)");
    }

    // Tolerance: 90 s. Which waiting job wins a released device is a race in
    // the storage daemon; the model gives it to the one that asked first,
    // and the 30 s between tries move a start by up to that much.
    const qint64 tolerance = 90;
    const int passbolt = indexOf(runs, "BackupPassbolt");
    const int nextcloud = indexOf(runs, "BackupNextcloud");
    QVERIFY2(qAbs(results[passbolt].waitSecs - 75) <= tolerance,
             qPrintable(QString::number(results[passbolt].waitSecs)));
    QVERIFY2(qAbs(results[nextcloud].waitSecs - 285) <= tolerance,
             qPrintable(QString::number(results[nextcloud].waitSecs)));
    QVERIFY(results[passbolt].certain);
    QVERIFY(results[nextcloud].certain);
    QCOMPARE(BScheduleWaitModel::discrepancy(runs[nextcloud], results[nextcloud]),
             BScheduleWaitModel::Discrepancy::None);

    // The tooltip says what went into it
    const QString timing = BScheduleWaitModel::describeTiming(nextcloud, runs, results,
                                                              fixtureLimits(1)).join('\n');
    QVERIFY(timing.contains(QStringLiteral("Start overhead 3:00 min")));
    QVERIFY(timing.contains(QStringLiteral("Measured wait at this level: median 4:45 min")));
}

void BWaitCalibrationTest::unknownDeviceIsUncertainAndNamed()
{
    BWaitCalibration cal;
    cal.setRuns(fixtureHistory(), fixtureJobs(), {});
    const QList<BWaitRun> runs = nightToPredict(&cal);
    const QList<BWaitResult> results = BScheduleWaitModel::predict(runs, fixtureLimits(0));

    BWaitUnknown device;
    device.kind = BWaitUnknown::Kind::DeviceLimit;
    device.name = kStorage;
    for (const char *job : {"BackupPassbolt", "BackupNextcloud"}) {
        const int i = indexOf(runs, job);
        QVERIFY2(!results[i].certain, job);
        QVERIFY2(results[i].unknowns.contains(device), job);
        const QString line = BScheduleWaitModel::reasonLine(runs[i], results[i]);
        QVERIFY2(line.contains(QStringLiteral("parallel writers of the device of storage PDC-sd")),
                 qPrintable(line));
    }
}

void BWaitCalibrationTest::backtestOnFixture()
{
    BWaitCalibration one;
    one.setRuns(fixtureHistory(), fixtureJobs(), {{kStorage, 1}});
    const BWaitCalibration::Backtest withDevice = one.backtest(fixtureLimits(1), 7);
    qInfo("%s", qPrintable(withDevice.text()));

    BWaitCalibration none;
    none.setRuns(fixtureHistory(), fixtureJobs(), {});
    const BWaitCalibration::Backtest withoutDevice = none.backtest(fixtureLimits(0), 7);
    qInfo("%s", qPrintable(withoutDevice.text()));

    QCOMPARE(withDevice.nights, 7);
    QCOMPARE(withDevice.runs, 70);
    QVERIFY(withoutDevice.unknownDevices.contains(kStorage));
    QVERIFY(withDevice.unknownDevices.isEmpty());
}

void BWaitCalibrationTest::suggestionNoOverlapSuggestsOne()
{
    // Without Just In Time Reservation: three jobs queued together, run one
    // after the other, never overlapping
    QList<BJobRun> runs;
    QHash<QString, BWaitJobInfo> jobs;
    for (const char *job : {"a", "b", "c"})
        jobs.insert(QString::fromLatin1(job), {QString::fromLatin1(job) + "-fd", kStorage, 10});
    for (int n = 0; n < 4; ++n) {
        const QDateTime due = at(QDate(2026, 11, 10 + n), 22, 0);
        runs << simpleRun("a", due, 2, 60, "a-fd")
             << simpleRun("b", due, 63, 60, "b-fd")
             << simpleRun("c", due, 124, 60, "c-fd");
    }
    BWaitCalibration cal;
    cal.setRuns(runs, jobs, {});
    const BWaitCalibration::DeviceSuggestion s = cal.suggestDevices(kStorage);
    QCOMPARE(s.suggested, 1);
    QCOMPARE(s.contendedNights, 4);
    QCOMPARE(s.overlapNights, 0);
}

void BWaitCalibrationTest::suggestionSerialEndsSuggestOne()
{
    // Just In Time Reservation: a and b start together, b waits for the
    // device inside its run. Alone (on other nights) b takes 60 s.
    QList<BJobRun> runs;
    QHash<QString, BWaitJobInfo> jobs;
    jobs.insert("a", {"a-fd", kStorage, 10});
    jobs.insert("b", {"b-fd", kStorage, 10});
    for (int n = 0; n < 3; ++n) {
        const QDateTime due = at(QDate(2026, 11, 10 + n), 22, 0);
        runs << simpleRun("a", due, 3, 100, "a-fd")
             << simpleRun("b", due, 3, 100 + 65, "b-fd");   // 65 s after a's end
    }
    runs << simpleRun("b", at(QDate(2026, 11, 14), 22, 0), 3, 60, "b-fd");
    runs << simpleRun("a", at(QDate(2026, 11, 15), 22, 0), 3, 100, "a-fd");
    BWaitCalibration cal;
    cal.setRuns(runs, jobs, {});
    const BWaitCalibration::DeviceSuggestion s = cal.suggestDevices(kStorage);
    QCOMPARE(s.overlapNights, 3);
    QCOMPARE(s.informativePairs, 3);
    QCOMPARE(s.serialPairs, 3);
    QCOMPARE(s.suggested, 1);
}

void BWaitCalibrationTest::suggestionParallelGivesNone()
{
    // Two writers: b ends its own 60 s after its start, while a still runs
    QList<BJobRun> runs;
    QHash<QString, BWaitJobInfo> jobs;
    jobs.insert("a", {"a-fd", kStorage, 10});
    jobs.insert("b", {"b-fd", kStorage, 10});
    for (int n = 0; n < 3; ++n) {
        const QDateTime due = at(QDate(2026, 11, 10 + n), 22, 0);
        runs << simpleRun("a", due, 3, 100, "a-fd")
             << simpleRun("b", due, 50, 60, "b-fd");
    }
    runs << simpleRun("b", at(QDate(2026, 11, 14), 22, 0), 3, 60, "b-fd");
    BWaitCalibration cal;
    cal.setRuns(runs, jobs, {});
    const BWaitCalibration::DeviceSuggestion s = cal.suggestDevices(kStorage);
    QCOMPARE(s.suggested, 0);
    QCOMPARE(s.parallelPairs, 3);
}

void BWaitCalibrationTest::suggestionOnFixtureCannotTell()
{
    BWaitCalibration cal;
    cal.setRuns(fixtureHistory(), fixtureJobs(), {});
    const BWaitCalibration::DeviceSuggestion s = cal.suggestDevices(kStorage);
    for (const QString &line : s.evidence) qInfo("%s", qPrintable(line));
    QCOMPARE(s.nights, 7);
    QCOMPARE(s.overlapNights, 7);
}

void BWaitCalibrationTest::deviceRetryEveryThirtySeconds()
{
    // Three runs on three clients at 22:00, 100 s each, one writer, no
    // overhead. All ask at 22:00:01; a gets the device until 22:01:40. b and
    // c wait to be woken: the first wait ends after a minute (22:01:01, a
    // try, the second wait), the second with the release at 22:01:40. b
    // (asked first, in the order handed in) gets it and writes until
    // 22:03:19. c has used both waits; it tries every 30 s from then on and
    // is not woken: 22:02:10, 22:02:40, 22:03:10, 22:03:40 - free.
    QList<BWaitRun> runs;
    for (const char *job : {"a", "b", "c"}) {
        BWaitRun run;
        run.id = run.job = QString::fromLatin1(job);
        run.client = run.job + QStringLiteral("-fd");
        run.storage = kStorage;
        run.allowMixedPriority = false;
        run.scheduled = at(QDate(2026, 11, 19), 22, 0);
        run.durationBasis = BWaitRun::Duration::Measured;
        run.durationSecs = 100;
        runs << run;
    }
    BWaitLimits limits;
    limits.director = 10;
    limits.storages.insert(kStorage, 10);
    for (const BWaitRun &run : runs) {
        limits.clients.insert(run.client, 1);
        limits.jobs.insert(run.job, 1);
    }
    limits.devices.insert(kStorage, 1);
    const QList<BWaitResult> results = BScheduleWaitModel::predict(runs, limits);
    QCOMPARE(results[0].deviceWaitSecs, qint64(0));
    QCOMPARE(results[1].deviceWaitSecs, qint64(99));
    QCOMPARE(results[2].deviceWaitSecs, qint64(219));
    QCOMPARE(results[2].end, at(QDate(2026, 11, 19), 22, 5).addSecs(19));
    // The waits are inside the runs: nobody waits at the Director
    for (const BWaitResult &result : results) QCOMPARE(result.waitSecs, qint64(0));
}

void BWaitCalibrationTest::historyParsesClient()
{
    const QString reply = QStringLiteral(
        "{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{\"jobs\":[{\"jobid\":\"7\",\"name\":\"BackupMail\","
        "\"client\":\"CLOUD-fd\",\"type\":\"B\",\"level\":\"I\",\"jobstatus\":\"T\","
        "\"schedtime\":\"2026-11-15 23:05:00\",\"starttime\":\"2026-11-15 23:05:06\","
        "\"endtime\":\"2026-11-15 23:06:01\"}]}}");
    const BJobHistoryReply parsed = BJobHistoryReply::parse(reply);
    QCOMPARE(parsed.runs.size(), 1);
    QCOMPARE(parsed.runs.first().client, QStringLiteral("CLOUD-fd"));
}

QTEST_GUILESS_MAIN(BWaitCalibrationTest)
#include "bwaitcalibration_test.moc"
