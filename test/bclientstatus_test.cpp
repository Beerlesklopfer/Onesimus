/**
 * @file bclientstatus_test.cpp
 * @brief Age and state of a client's last backup or contact
 *
 * Runs without a Director (registered with CTest). All cases use a fixed
 * clock and fixed-offset time zones, so the result does not depend on the
 * machine's zone or on the time of day.
 */

#include <QtTest>

#include "clients/bclientstatus.h"

using BClientStatus::Activity;
using BClientStatus::Info;

class BClientStatusTest : public QObject
{
    Q_OBJECT

private slots:
    void oneHourAgoEastOfUtcIsRecent();
    void directorTimeIsNotReadAsUtc();
    void missingTimestampIsNever_data();
    void missingTimestampIsNever();
    void unreadableTimestampIsUnknown();
    void thresholdBoundary_data();
    void thresholdBoundary();
    void thresholdIsConfigurable();
    void invalidThresholdFallsBackToDefault();
    void futureTimestampNeverGivesNegativeAge();
    void timestampWithOwnZoneKeepsIt();
    void isoTimestampWithoutZoneIsDirectorTime();
    void westOfUtc();
    void ageText_data();
    void ageText();
    void labelAndToolTip();
    void sortKeyOrdersYoungestFirst();

private:
    // UTC+2, as Central Europe in summer; a fixed offset needs no tz database
    const QTimeZone m_east = QTimeZone(2 * 3600);
    const QTimeZone m_west = QTimeZone(-5 * 3600);
    // 10:00 UTC = 12:00 at UTC+2 = 05:00 at UTC-5
    const QDateTime m_now = QDateTime(QDate(2026, 6, 1), QTime(10, 0, 0), QTimeZone::utc());
};

void BClientStatusTest::oneHourAgoEastOfUtcIsRecent()
{
    // The Director (UTC+2) writes its wall-clock time: 11:00 is one hour before 12:00
    const Info info = BClientStatus::classify("2026-06-01 11:00:00", m_now, 24, m_east);

    QCOMPARE(info.activity, Activity::Recent);
    QCOMPARE(info.ageSecs, qint64(3600));
    QCOMPARE(info.timestamp.toUTC(), QDateTime(QDate(2026, 6, 1), QTime(9, 0, 0), QTimeZone::utc()));
}

void BClientStatusTest::directorTimeIsNotReadAsUtc()
{
    // The old code relabelled the Director's local time as UTC. For a job
    // started one hour ago at UTC+2 that is 11:00 UTC, one hour after "now"
    // (10:00 UTC): a negative age, shown as Offline.
    const QDateTime relabelled(QDate(2026, 6, 1), QTime(11, 0, 0), QTimeZone::utc());
    QVERIFY(relabelled.secsTo(m_now) < 0);

    const Info info = BClientStatus::classify("2026-06-01 11:00:00", m_now, 24, m_east);
    QVERIFY(info.ageSecs >= 0);
    QCOMPARE(info.activity, Activity::Recent);
}

void BClientStatusTest::missingTimestampIsNever_data()
{
    QTest::addColumn<QString>("timestamp");

    QTest::newRow("empty") << QString();
    QTest::newRow("blank") << QStringLiteral("   ");
    QTest::newRow("zero") << QStringLiteral("0");
    QTest::newRow("zero date") << QStringLiteral("0000-00-00 00:00:00");
}

void BClientStatusTest::missingTimestampIsNever()
{
    QFETCH(QString, timestamp);

    const Info info = BClientStatus::classify(timestamp, m_now, 24, m_east);

    QCOMPARE(info.activity, Activity::Never);
    QVERIFY(!info.timestamp.isValid());
    QCOMPARE(info.ageSecs, qint64(-1));
    QVERIFY(BClientStatus::isMissingTimestamp(timestamp));
}

void BClientStatusTest::unreadableTimestampIsUnknown()
{
    const Info info = BClientStatus::classify("yesterday", m_now, 24, m_east);

    QCOMPARE(info.activity, Activity::Unknown);
    QVERIFY(!info.timestamp.isValid());
    QCOMPARE(info.ageSecs, qint64(-1));
    QCOMPARE(info.raw, QStringLiteral("yesterday"));
}

void BClientStatusTest::thresholdBoundary_data()
{
    QTest::addColumn<QString>("timestamp");
    QTest::addColumn<qint64>("ageSecs");
    QTest::addColumn<bool>("recent");

    // now is 2026-06-01 12:00:00 at UTC+2; the threshold is 24 h = 86400 s
    QTest::newRow("one second inside") << "2026-05-31 12:00:01" << qint64(86399) << true;
    QTest::newRow("exactly the threshold") << "2026-05-31 12:00:00" << qint64(86400) << false;
    QTest::newRow("one second beyond") << "2026-05-31 11:59:59" << qint64(86401) << false;
    QTest::newRow("this very second") << "2026-06-01 12:00:00" << qint64(0) << true;
}

void BClientStatusTest::thresholdBoundary()
{
    QFETCH(QString, timestamp);
    QFETCH(qint64, ageSecs);
    QFETCH(bool, recent);

    const Info info = BClientStatus::classify(timestamp, m_now, 24, m_east);

    QCOMPARE(info.ageSecs, ageSecs);
    QCOMPARE(info.activity, recent ? Activity::Recent : Activity::Stale);
}

void BClientStatusTest::thresholdIsConfigurable()
{
    // Three hours old
    const QString timestamp = "2026-06-01 09:00:00";

    QCOMPARE(BClientStatus::classify(timestamp, m_now, 2, m_east).activity, Activity::Stale);
    QCOMPARE(BClientStatus::classify(timestamp, m_now, 3, m_east).activity, Activity::Stale);
    QCOMPARE(BClientStatus::classify(timestamp, m_now, 4, m_east).activity, Activity::Recent);

    // Six days old, limit one week
    QCOMPARE(BClientStatus::classify("2026-05-26 12:00:00", m_now, 168, m_east).activity,
             Activity::Recent);
}

void BClientStatusTest::invalidThresholdFallsBackToDefault()
{
    QCOMPARE(BClientStatus::DefaultThresholdHours, 24);

    // 23 h and 25 h old
    QCOMPARE(BClientStatus::classify("2026-05-31 13:00:00", m_now, 0, m_east).activity,
             Activity::Recent);
    QCOMPARE(BClientStatus::classify("2026-05-31 11:00:00", m_now, 0, m_east).activity,
             Activity::Stale);
    QCOMPARE(BClientStatus::classify("2026-05-31 11:00:00", m_now, -5, m_east).activity,
             Activity::Stale);
}

void BClientStatusTest::futureTimestampNeverGivesNegativeAge()
{
    // Two hours ahead of the clock: skew, or a Director further east than assumed
    const Info info = BClientStatus::classify("2026-06-01 14:00:00", m_now, 24, m_east);

    QCOMPARE(info.ageSecs, qint64(0));
    QCOMPARE(info.activity, Activity::Recent);
}

void BClientStatusTest::timestampWithOwnZoneKeepsIt()
{
    // 09:30 UTC is 30 minutes ago, whatever zone the Director is assumed to be in
    const Info utc = BClientStatus::classify("2026-06-01T09:30:00Z", m_now, 24, m_east);
    QCOMPARE(utc.ageSecs, qint64(1800));

    // 13:30 at +05:00 is 08:30 UTC
    const Info offset = BClientStatus::classify("2026-06-01T13:30:00+05:00", m_now, 24, m_east);
    QCOMPARE(offset.ageSecs, qint64(5400));
}

void BClientStatusTest::isoTimestampWithoutZoneIsDirectorTime()
{
    const Info info = BClientStatus::classify("2026-06-01T11:00:00", m_now, 24, m_east);

    QCOMPARE(info.activity, Activity::Recent);
    QCOMPARE(info.ageSecs, qint64(3600));
}

void BClientStatusTest::westOfUtc()
{
    // 04:00 at UTC-5 is one hour before 05:00
    const Info info = BClientStatus::classify("2026-06-01 04:00:00", m_now, 24, m_west);

    QCOMPARE(info.activity, Activity::Recent);
    QCOMPARE(info.ageSecs, qint64(3600));
}

void BClientStatusTest::ageText_data()
{
    QTest::addColumn<qint64>("ageSecs");
    QTest::addColumn<QString>("text");

    QTest::newRow("now") << qint64(0) << "just now";
    QTest::newRow("59 s") << qint64(59) << "just now";
    QTest::newRow("1 min") << qint64(60) << "1 min ago";
    QTest::newRow("59 min") << qint64(3599) << "59 min ago";
    QTest::newRow("1 h") << qint64(3600) << "1 h ago";
    QTest::newRow("47 h") << qint64(47 * 3600 + 3599) << "47 h ago";
    QTest::newRow("2 d") << qint64(48 * 3600) << "2 d ago";
    QTest::newRow("30 d") << qint64(30 * 86400 + 5) << "30 d ago";
    QTest::newRow("no age") << qint64(-1) << "";
}

void BClientStatusTest::ageText()
{
    QFETCH(qint64, ageSecs);
    QFETCH(QString, text);

    QCOMPARE(BClientStatus::ageText(ageSecs), text);
}

void BClientStatusTest::labelAndToolTip()
{
    const Info recent = BClientStatus::classify("2026-06-01 09:00:00", m_now, 24, m_east);
    QCOMPARE(BClientStatus::label(recent), QStringLiteral("3 h ago"));
    QVERIFY(BClientStatus::toolTip(recent, 24).contains("3 h ago"));
    QVERIFY(BClientStatus::toolTip(recent, 24).contains("24 h"));
    // The tooltip names the timestamp as the Director wrote it (09:00, not UTC 07:00)
    QVERIFY(BClientStatus::toolTip(recent, 24)
                .contains(QLocale().toString(recent.timestamp, QLocale::ShortFormat)));
    QCOMPARE(recent.timestamp.time(), QTime(9, 0, 0));

    const Info stale = BClientStatus::classify("2026-05-20 12:00:00", m_now, 24, m_east);
    QCOMPARE(BClientStatus::label(stale), QStringLiteral("12 d ago"));
    QVERIFY(BClientStatus::toolTip(stale, 24).contains("Stale"));

    const Info never = BClientStatus::classify(QString(), m_now, 24, m_east);
    QCOMPARE(BClientStatus::label(never), QStringLiteral("Never"));
    QVERIFY(!BClientStatus::toolTip(never, 24).isEmpty());

    const Info unknown = BClientStatus::classify("garbage", m_now, 24, m_east);
    QCOMPARE(BClientStatus::label(unknown), QStringLiteral("Unknown"));
    QVERIFY(BClientStatus::toolTip(unknown, 24).contains("garbage"));

    // No state is called online or offline
    for (Activity activity : {Activity::Recent, Activity::Stale, Activity::Never, Activity::Unknown}) {
        QVERIFY(!BClientStatus::stateName(activity).contains("line", Qt::CaseInsensitive));
    }
}

void BClientStatusTest::sortKeyOrdersYoungestFirst()
{
    const Info young = BClientStatus::classify("2026-06-01 11:00:00", m_now, 24, m_east);
    const Info old = BClientStatus::classify("2026-05-01 11:00:00", m_now, 24, m_east);
    const Info never = BClientStatus::classify(QString(), m_now, 24, m_east);
    const Info unknown = BClientStatus::classify("garbage", m_now, 24, m_east);

    QVERIFY(BClientStatus::sortKey(young) < BClientStatus::sortKey(old));
    QVERIFY(BClientStatus::sortKey(old) < BClientStatus::sortKey(never));
    QVERIFY(BClientStatus::sortKey(never) < BClientStatus::sortKey(unknown));
}

QTEST_APPLESS_MAIN(BClientStatusTest)
#include "bclientstatus_test.moc"
