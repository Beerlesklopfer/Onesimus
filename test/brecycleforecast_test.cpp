/**
 * @file brecycleforecast_test.cpp
 * @brief Test for BRecycleForecast: the "reusable from" rule of the storage plan
 *
 * Bareos 25 semantics (core/src/dird): Full/Used reusable from LastWritten +
 * VolRetention (next_vol.cc:345); earlier through the job retention when it
 * is shorter (ua_prune.cc 1021-1110), min() of both; never for Error,
 * Read-Only, Disabled, Archive, Recycle = no and Append without use duration
 * (autoprune.cc:170, ua_prune.cc:1179/1188); Purged now.
 *
 * Cases on the real PDC reply (test/fixtures/pdc-list-volumes-api2.json) and
 * on single volumes. "Now" is fixed at 19.11.2026 12:00. Runs with TZ=UTC.
 */

#include <QtTest>

#include <QFile>

#include "storage/model/brecycleforecast.h"
#include "storage/model/bvolumelist.h"

using Kind = BRecycleForecast::Kind;
using Path = BRecycleForecast::Path;

namespace {

const QDateTime kNow(QDate(2026, 11, 19), QTime(12, 0));
constexpr qint64 kDay = 86400;

BVolume volume(const QString &status)
{
    BVolume v;
    v.name = QStringLiteral("TEST-0001");
    v.pool = QStringLiteral("CLOUD");
    v.status = status;
    v.enabled = 1;
    v.recycle = true;
    v.volBytes = 53686042829LL;
    v.volRetention = 365 * kDay;
    v.lastWritten = QDateTime(QDate(2026, 3, 2), QTime(4, 16, 19));
    return v;
}

BVolumeList pdcList()
{
    QFile file(QStringLiteral(STORAGE_FIXTURE_DIR "/pdc-list-volumes-api2.json"));
    if (!file.open(QIODevice::ReadOnly)) {
        return BVolumeList();
    }
    return BVolumeList::fromJson(file.readAll(),
                                 {"Tapir", "Scratch", "Incremental", "Full", "Forgejo",
                                  "Differential", "Default", "CLOUD", "BGLAGE"});
}

} // namespace

class BRecycleForecastTest : public QObject
{
    Q_OBJECT

private slots:
    void fullByVolumeRetention();
    void fullDatePassed();
    void usedLikeFull();
    void fullMissingFields();
    void jobRetentionEarlier();
    void jobRetentionLaterKeepsVolumeRetention();
    void jobRetentionWithNewestJob();
    void minRule_data();
    void minRule();
    void error();
    void neverCases_data();
    void neverCases();
    void appendWithoutUseDuration();
    void appendShortListUnknown();
    void appendWithUseDuration();
    void appendUseDurationPassed();
    void purged();
    void purgedSmall();
    void unknownStatus();
    void pdcFixture();
    void pdcLlistFixture();
    void formatting();
};

void BRecycleForecastTest::fullByVolumeRetention()
{
    const BVolume v = volume(QStringLiteral("Full"));
    const auto r = BRecycleForecast::forecast(v, kNow);
    QCOMPARE(r.kind, Kind::Date);
    QCOMPARE(r.path, Path::VolumeRetention);
    QCOMPARE(r.when, QDateTime(QDate(2027, 3, 2), QTime(4, 16, 19)));
    QVERIFY(!r.isDue(kNow));
    QVERIFY2(r.reason.contains(QLatin1String("365 d")), qPrintable(r.reason));
    QVERIFY2(r.reason.contains(QLatin1String("2 Mar 2026")), qPrintable(r.reason));
    QCOMPARE(BRecycleForecast::columnText(r), QStringLiteral("2 Mar 2027"));
}

void BRecycleForecastTest::fullDatePassed()
{
    BVolume v = volume(QStringLiteral("Full"));
    v.volRetention = 30 * kDay;
    v.lastWritten = QDateTime(QDate(2026, 10, 1), QTime(20, 0));
    const auto r = BRecycleForecast::forecast(v, kNow);
    QCOMPARE(r.kind, Kind::Date);
    QCOMPARE(r.when, QDateTime(QDate(2026, 10, 31), QTime(20, 0)));
    QVERIFY(r.isDue(kNow));
    // "reusable from", never "recycled on": pruning waits for the pool's need
    QVERIFY2(r.reason.contains(QLatin1String("next needs a volume")), qPrintable(r.reason));
}

void BRecycleForecastTest::usedLikeFull()
{
    const auto r = BRecycleForecast::forecast(volume(QStringLiteral("Used")), kNow);
    QCOMPARE(r.kind, Kind::Date);
    QCOMPARE(r.path, Path::VolumeRetention);
    QVERIFY(r.reason.startsWith(QLatin1String("Used")));
}

void BRecycleForecastTest::fullMissingFields()
{
    BVolume v = volume(QStringLiteral("Full"));
    v.lastWritten = QDateTime();
    QCOMPARE(BRecycleForecast::forecast(v, kNow).kind, Kind::Unknown);
    v = volume(QStringLiteral("Full"));
    v.volRetention.reset();
    QCOMPARE(BRecycleForecast::forecast(v, kNow).kind, Kind::Unknown);
}

void BRecycleForecastTest::jobRetentionEarlier()
{
    // CLOUD on PDC: volume retention 365 d, client job retention 180 d.
    // The jobs go after 180 d; the volume becomes Purged then, not at 365 d.
    const BVolume v = volume(QStringLiteral("Full"));
    BRecycleForecast::JobRetention jobs;
    jobs.seconds = 180 * kDay;
    const auto r = BRecycleForecast::forecast(v, kNow, jobs);
    QCOMPARE(r.kind, Kind::Date);
    QCOMPARE(r.path, Path::JobRetention);
    QCOMPARE(r.when, v.lastWritten.addSecs(180 * kDay));   // 29 Aug 2026
    QCOMPARE(r.when.date(), QDate(2026, 8, 29));
    QVERIFY(r.isDue(kNow));
    QVERIFY2(r.reason.contains(QLatin1String("job retention of 180 d")), qPrintable(r.reason));
}

void BRecycleForecastTest::jobRetentionLaterKeepsVolumeRetention()
{
    // BGLAGE: volume retention 30 d, job retention 180 d -> volume path
    BVolume v = volume(QStringLiteral("Full"));
    v.volRetention = 30 * kDay;
    v.lastWritten = QDateTime(QDate(2026, 10, 29), QTime(23, 6, 6));
    BRecycleForecast::JobRetention jobs;
    jobs.seconds = 180 * kDay;
    const auto r = BRecycleForecast::forecast(v, kNow, jobs);
    QCOMPARE(r.path, Path::VolumeRetention);
    QCOMPARE(r.when, QDateTime(QDate(2026, 11, 28), QTime(23, 6, 6)));
}

void BRecycleForecastTest::jobRetentionWithNewestJob()
{
    // The newest job on the volume may be older than its last write
    // (a job that only appended a few blocks later is not the newest JobTDate)
    const BVolume v = volume(QStringLiteral("Full"));
    BRecycleForecast::JobRetention jobs;
    jobs.seconds = 180 * kDay;
    jobs.newestJob = QDateTime(QDate(2026, 2, 20), QTime(23, 0));
    const auto r = BRecycleForecast::forecast(v, kNow, jobs);
    QCOMPARE(r.path, Path::JobRetention);
    QCOMPARE(r.when, jobs.newestJob.addSecs(180 * kDay));
}

void BRecycleForecastTest::minRule_data()
{
    QTest::addColumn<qint64>("volumeDays");
    QTest::addColumn<qint64>("jobDays");
    QTest::addColumn<int>("expectedPath");
    QTest::addColumn<qint64>("expectedDays");

    QTest::newRow("job shorter") << qint64(365) << qint64(180) << int(Path::JobRetention) << qint64(180);
    QTest::newRow("volume shorter") << qint64(30) << qint64(180) << int(Path::VolumeRetention) << qint64(30);
    QTest::newRow("equal: volume path") << qint64(60) << qint64(60) << int(Path::VolumeRetention) << qint64(60);
    QTest::newRow("job 0 days") << qint64(30) << qint64(0) << int(Path::JobRetention) << qint64(0);
}

void BRecycleForecastTest::minRule()
{
    QFETCH(qint64, volumeDays);
    QFETCH(qint64, jobDays);
    QFETCH(int, expectedPath);
    QFETCH(qint64, expectedDays);
    BVolume v = volume(QStringLiteral("Full"));
    v.volRetention = volumeDays * kDay;
    BRecycleForecast::JobRetention jobs;
    jobs.seconds = jobDays * kDay;
    const auto r = BRecycleForecast::forecast(v, kNow, jobs);
    QCOMPARE(int(r.path), expectedPath);
    QCOMPARE(r.when, v.lastWritten.addSecs(expectedDays * kDay));
}

void BRecycleForecastTest::error()
{
    BVolume v = volume(QStringLiteral("Error"));
    const auto r = BRecycleForecast::forecast(v, kNow);
    QCOMPARE(r.kind, Kind::Never);
    QVERIFY2(r.reason.contains(QLatin1String("Error")), qPrintable(r.reason));
    QCOMPARE(BRecycleForecast::columnText(r), QStringLiteral("never"));
    // A job retention does not change it: the jobs expire, the volume stays
    BRecycleForecast::JobRetention jobs;
    jobs.seconds = kDay;
    QCOMPARE(BRecycleForecast::forecast(v, kNow, jobs).kind, Kind::Never);
}

void BRecycleForecastTest::neverCases_data()
{
    QTest::addColumn<QString>("status");
    QTest::addColumn<int>("enabled");
    QTest::addColumn<bool>("recycle");
    QTest::addColumn<QString>("reasonPart");

    QTest::newRow("Read-Only") << "Read-Only" << 1 << true << "Read-Only";
    QTest::newRow("Disabled status") << "Disabled" << 1 << true << "Disabled";
    QTest::newRow("Enabled = 0") << "Full" << 0 << true << "Disabled";
    QTest::newRow("Archive status") << "Archive" << 1 << true << "Archived";
    QTest::newRow("Enabled = 2") << "Full" << 2 << true << "Archived";
    QTest::newRow("Recycle = no, Full") << "Full" << 1 << false << "Recycle = no";
    QTest::newRow("Recycle = no, Purged") << "Purged" << 1 << false << "Recycle = no";
    QTest::newRow("Cleaning") << "Cleaning" << 1 << true << "Cleaning";
}

void BRecycleForecastTest::neverCases()
{
    QFETCH(QString, status);
    QFETCH(int, enabled);
    QFETCH(bool, recycle);
    QFETCH(QString, reasonPart);
    BVolume v = volume(status);
    v.enabled = enabled;
    v.recycle = recycle;
    const auto r = BRecycleForecast::forecast(v, kNow);
    QCOMPARE(r.kind, Kind::Never);
    QVERIFY2(r.reason.contains(reasonPart), qPrintable(r.reason));
}

void BRecycleForecastTest::appendWithoutUseDuration()
{
    BVolume v = volume(QStringLiteral("Append"));
    v.volUseDuration = 0;
    v.maxVolBytes = 53687091200LL;
    const auto r = BRecycleForecast::forecast(v, kNow);
    QCOMPARE(r.kind, Kind::Never);
    QVERIFY2(r.reason.contains(QLatin1String("until it is full")), qPrintable(r.reason));
    QVERIFY2(r.reason.contains(QLatin1String("50 GiB")), qPrintable(r.reason));
}

void BRecycleForecastTest::appendShortListUnknown()
{
    // The short "list volumes" has no voluseduration: do not claim "never"
    BVolume v = volume(QStringLiteral("Append"));
    const auto r = BRecycleForecast::forecast(v, kNow);
    QCOMPARE(r.kind, Kind::Unknown);
    QCOMPARE(BRecycleForecast::columnText(r), QStringLiteral("unknown"));
}

void BRecycleForecastTest::appendWithUseDuration()
{
    BVolume v = volume(QStringLiteral("Append"));
    v.volUseDuration = 7 * kDay;
    v.volRetention = 30 * kDay;
    v.firstWritten = QDateTime(QDate(2026, 11, 17), QTime(23, 0));
    v.lastWritten = QDateTime(QDate(2026, 11, 18), QTime(23, 0));
    const auto r = BRecycleForecast::forecast(v, kNow);
    QCOMPARE(r.kind, Kind::Date);
    QCOMPARE(r.path, Path::UseDuration);
    // Used at 24 Nov 23:00, then 30 days
    QCOMPARE(r.when, QDateTime(QDate(2026, 12, 24), QTime(23, 0)));
}

void BRecycleForecastTest::appendUseDurationPassed()
{
    // Use duration over, still Append (Bareos marks it Used at its next look):
    // the last write starts the retention
    BVolume v = volume(QStringLiteral("Append"));
    v.volUseDuration = kDay;
    v.volRetention = 30 * kDay;
    v.firstWritten = QDateTime(QDate(2026, 11, 1), QTime(23, 0));
    v.lastWritten = QDateTime(QDate(2026, 11, 2), QTime(1, 0));
    const auto r = BRecycleForecast::forecast(v, kNow);
    QCOMPARE(r.kind, Kind::Date);
    QCOMPARE(r.when, QDateTime(QDate(2026, 12, 2), QTime(1, 0)));
    // Without the first write the date cannot be told
    v.firstWritten = QDateTime();
    QCOMPARE(BRecycleForecast::forecast(v, kNow).kind, Kind::Unknown);
}

void BRecycleForecastTest::purged()
{
    BVolume v = volume(QStringLiteral("Purged"));
    v.volBytes = 53686042829LL;    // BGLAGE-0019
    const auto r = BRecycleForecast::forecast(v, kNow);
    QCOMPARE(r.kind, Kind::Now);
    QCOMPARE(r.when, kNow);
    QVERIFY2(r.reason.contains(QLatin1String("50 GiB on disk")), qPrintable(r.reason));
    QCOMPARE(BRecycleForecast::columnText(r), QStringLiteral("now"));
}

void BRecycleForecastTest::purgedSmall()
{
    BVolume v = volume(QStringLiteral("Purged"));
    v.volBytes = 207;              // BGLAGE-0013: already truncated
    const auto r = BRecycleForecast::forecast(v, kNow);
    QCOMPARE(r.kind, Kind::Now);
    QVERIFY2(!r.reason.contains(QLatin1String("on disk")), qPrintable(r.reason));
}

void BRecycleForecastTest::unknownStatus()
{
    QCOMPARE(BRecycleForecast::forecast(volume(QString()), kNow).kind, Kind::Unknown);
    QCOMPARE(BRecycleForecast::forecast(volume(QStringLiteral("Strange")), kNow).kind, Kind::Unknown);
    QCOMPARE(BRecycleForecast::forecast(volume(QStringLiteral("Recycle")), kNow).kind, Kind::Now);
    QCOMPARE(BRecycleForecast::forecast(volume(QStringLiteral("Busy")), kNow).kind, Kind::Unknown);
}

void BRecycleForecastTest::pdcFixture()
{
    const BVolumeList list = pdcList();
    QCOMPARE(list.size(), 23);
    int never = 0, now = 0, unknown = 0, dated = 0;
    for (const BVolume &v : list.volumes()) {
        const auto r = BRecycleForecast::forecast(v, kNow);
        QVERIFY2(!r.reason.isEmpty(), qPrintable(v.name));
        switch (r.kind) {
        case Kind::Never: ++never; QCOMPARE(v.status, QStringLiteral("Error")); break;
        case Kind::Now: ++now; QCOMPARE(v.status, QStringLiteral("Purged")); break;
        case Kind::Unknown: ++unknown; QCOMPARE(v.status, QStringLiteral("Append")); break;
        case Kind::Date: ++dated; QCOMPARE(v.status, QStringLiteral("Full")); break;
        }
    }
    QCOMPARE(never, 4);
    QCOMPARE(now, 2);
    QCOMPARE(unknown, 5);     // short list: use duration not reported
    QCOMPARE(dated, 12);

    for (const BVolume &v : list.volumes()) {
        const auto r = BRecycleForecast::forecast(v, kNow);
        if (v.name == QLatin1String("CLOUD-0002")) {
            QCOMPARE(r.when, QDateTime(QDate(2027, 1, 22), QTime(3, 58, 27)));
        } else if (v.name == QLatin1String("BGLAGE-0020")) {
            QCOMPARE(r.when, QDateTime(QDate(2026, 11, 28), QTime(23, 6, 6)));
            QVERIFY(!r.isDue(kNow));
        } else if (v.name == QLatin1String("BGLAGE-0019")) {
            QVERIFY2(r.reason.contains(QLatin1String("50 GiB on disk")), qPrintable(r.reason));
        }
    }
}

void BRecycleForecastTest::pdcLlistFixture()
{
    // The real "llist volumes" of PDC: no pool has a use duration, so every
    // Append volume stays open until it is full (50 GiB)
    QFile file(QStringLiteral(STORAGE_FIXTURE_DIR "/pdc-llist-volumes-api2.json"));
    QVERIFY(file.open(QIODevice::ReadOnly));
    const BVolumeList list = BVolumeList::fromJson(file.readAll(),
                                                   {"Tapir", "Scratch", "Incremental", "Full", "Forgejo",
                                                    "Differential", "Default", "CLOUD", "BGLAGE"});
    QCOMPARE(list.size(), 23);
    int never = 0, now = 0, dated = 0;
    for (const BVolume &v : list.volumes()) {
        const auto r = BRecycleForecast::forecast(v, kNow);
        QVERIFY2(r.kind != Kind::Unknown, qPrintable(v.name + ": " + r.reason));
        if (v.status == QLatin1String("Append")) {
            QCOMPARE(r.kind, Kind::Never);
            QVERIFY2(r.reason.contains(QLatin1String("until it is full (50 GiB)")), qPrintable(r.reason));
            ++never;
        } else if (v.status == QLatin1String("Error")) {
            QCOMPARE(r.kind, Kind::Never);
            ++never;
        } else if (v.status == QLatin1String("Purged")) {
            QCOMPARE(r.kind, Kind::Now);
            ++now;
        } else {
            QCOMPARE(r.kind, Kind::Date);
            QCOMPARE(r.when, v.lastWritten.addSecs(*v.volRetention));
            ++dated;
        }
    }
    QCOMPARE(never, 9);
    QCOMPARE(now, 2);
    QCOMPARE(dated, 12);
}

void BRecycleForecastTest::formatting()
{
    QCOMPARE(BRecycleForecast::formatDuration(31536000), QStringLiteral("365 d"));
    QCOMPARE(BRecycleForecast::formatDuration(2592000), QStringLiteral("30 d"));
    QCOMPARE(BRecycleForecast::formatDuration(129600), QStringLiteral("1.5 d"));
    QCOMPARE(BRecycleForecast::formatDuration(7200), QStringLiteral("2 h"));
    QCOMPARE(BRecycleForecast::formatDuration(5400), QStringLiteral("90 min"));
    QCOMPARE(BRecycleForecast::formatDuration(45), QStringLiteral("45 s"));
    QCOMPARE(BRecycleForecast::formatDuration(0), QStringLiteral("0 s"));
    QCOMPARE(BRecycleForecast::formatDate(QDateTime(QDate(2026, 3, 9), QTime(1, 0))),
             QStringLiteral("9 Mar 2026"));
    QVERIFY(BRecycleForecast::formatDate(QDateTime()).isEmpty());
}

QTEST_GUILESS_MAIN(BRecycleForecastTest)
#include "brecycleforecast_test.moc"
