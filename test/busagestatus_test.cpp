// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file busagestatus_test.cpp
 * @brief The free tier usage in the status bar: text, level, projection, tool tip
 *
 * With gRPC also the conversion of usage.proto's UsageInfo, including the
 * fields that are absent on the wire because they are zero.
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include <QtTest>
#include <QTimeZone>

#include "busagestatus.h"

#ifdef HAVE_GRPC
#include "director/busageproto.h"
#include "usage.qpb.h"
#include <QProtobufSerializer>
#endif

namespace {

constexpr quint64 GB = 1000ULL * 1000 * 1000;
constexpr quint64 TB = 1000 * GB;

// 2026-12-01 00:00 UTC: the reset of November in a Director on UTC
qint64 decemberFirst()
{
    return QDateTime(QDate(2026, 12, 1), QTime(0, 0), QTimeZone::UTC).toSecsSinceEpoch();
}

QDateTime utc(int day, int hour = 0)
{
    return QDateTime(QDate(2026, 11, day), QTime(hour, 0), QTimeZone::UTC);
}

BUsage::Info freeTier(quint64 written, quint64 running = 0,
                      BUsage::State state = BUsage::State::Ok)
{
    BUsage::Info info;
    info.present = true;
    info.month = QStringLiteral("2026-11");
    info.bytesWritten = written;
    info.bytesRunning = running;
    info.limitBytes = TB;
    info.state = state;
    info.resetsAt = decemberFirst();
    info.licence = BUsage::Licence::Free;
    return info;
}

} // namespace

class BUsageStatusTest : public QObject
{
    Q_OBJECT

private slots:
    void licenceHiddenForFreeTierAndFarEnd();
    void licenceEndsSoon();
    void licenceExpiredAndInvalid();
    void hiddenWhenNotReported();
    void textWithLimit();
    void textCountsRunningJobs();
    void textWithoutLimit();
    void textWithoutMonth();
    void levelFromDirector();
    void levelFallback_data();
    void levelFallback();
    void projectionHalfMonth();
    void projectionNotInFirstDay();
    void projectionNotAfterReset();
    void projectionNeedsMonthAndReset();
    void toolTipFreeTier();
    void toolTipProjectionAboveLimit();
    void toolTipReached();
    void toolTipLicensedUnlimited();
    void toolTipExpired();
#ifdef HAVE_GRPC
    void protoFull();
    void protoAbsentZeroFields();
#endif
};

void BUsageStatusTest::hiddenWhenNotReported()
{
    const BUsage::Info none;
    QCOMPARE(BUsageStatus::level(none), BUsageStatus::Level::Hidden);
    QVERIFY(BUsageStatus::text(none, QLocale::c()).isEmpty());
    QVERIFY(BUsageStatus::toolTip(none, utc(16), QLocale::c()).isEmpty());
    QVERIFY(!BUsageStatus::projection(none, utc(16)));
}

void BUsageStatusTest::textWithLimit()
{
    QCOMPARE(BUsageStatus::text(freeTier(73600000000ULL), QLocale::c()),
             QStringLiteral("Usage: 73.6 GB of 1 TB (Nov)"));
}

void BUsageStatusTest::textCountsRunningJobs()
{
    QCOMPARE(BUsageStatus::text(freeTier(70 * GB, 3600000000ULL), QLocale::c()),
             QStringLiteral("Usage: 73.6 GB of 1 TB (Nov)"));
}

void BUsageStatusTest::textWithoutLimit()
{
    BUsage::Info info = freeTier(2300 * GB);
    info.licence = BUsage::Licence::Licensed;
    info.limitBytes = 0;
    QCOMPARE(BUsageStatus::text(info, QLocale::c()), QStringLiteral("Usage: 2.3 TB (Nov)"));
}

void BUsageStatusTest::textWithoutMonth()
{
    BUsage::Info info = freeTier(73600000000ULL);
    info.month.clear();
    QCOMPARE(BUsageStatus::text(info, QLocale::c()), QStringLiteral("Usage: 73.6 GB of 1 TB"));
    info.limitBytes = 0;
    QCOMPARE(BUsageStatus::text(info, QLocale::c()), QStringLiteral("Usage: 73.6 GB"));
}

void BUsageStatusTest::levelFromDirector()
{
    // odir's state wins, also where the bytes would say otherwise
    QCOMPARE(BUsageStatus::level(freeTier(0, 0, BUsage::State::Ok)), BUsageStatus::Level::Ok);
    QCOMPARE(BUsageStatus::level(freeTier(0, 0, BUsage::State::Near)), BUsageStatus::Level::Near);
    QCOMPARE(BUsageStatus::level(freeTier(0, 0, BUsage::State::Reached)), BUsageStatus::Level::Reached);
}

void BUsageStatusTest::levelFallback_data()
{
    QTest::addColumn<quint64>("written");
    QTest::addColumn<quint64>("running");
    QTest::addColumn<quint64>("limit");
    QTest::addColumn<int>("level");

    QTest::newRow("below 80 %") << 799 * GB << quint64(0) << TB << int(BUsageStatus::Level::Ok);
    QTest::newRow("80 %") << 800 * GB << quint64(0) << TB << int(BUsageStatus::Level::Near);
    QTest::newRow("80 % with running") << 700 * GB << 100 * GB << TB << int(BUsageStatus::Level::Near);
    QTest::newRow("at the limit") << TB << quint64(0) << TB << int(BUsageStatus::Level::Reached);
    QTest::newRow("over the limit") << 2 * TB << quint64(0) << TB << int(BUsageStatus::Level::Reached);
    QTest::newRow("no limit") << 5 * TB << quint64(0) << quint64(0) << int(BUsageStatus::Level::Ok);
}

void BUsageStatusTest::levelFallback()
{
    QFETCH(quint64, written);
    QFETCH(quint64, running);
    QFETCH(quint64, limit);
    QFETCH(int, level);
    BUsage::Info info = freeTier(written, running, BUsage::State::Unspecified);
    info.limitBytes = limit;
    QCOMPARE(int(BUsageStatus::level(info)), level);
}

void BUsageStatusTest::projectionHalfMonth()
{
    // November has 30 days: on the 16th at 00:00 half of it has passed
    const auto projected = BUsageStatus::projection(freeTier(80 * GB, 20 * GB), utc(16));
    QVERIFY(projected);
    QCOMPARE(*projected, 200 * GB);
}

void BUsageStatusTest::projectionNotInFirstDay()
{
    QVERIFY(!BUsageStatus::projection(freeTier(10 * GB), utc(1, 23)));
    QVERIFY(BUsageStatus::projection(freeTier(10 * GB), utc(2, 0)));
}

void BUsageStatusTest::projectionNotAfterReset()
{
    const QDateTime after = QDateTime::fromSecsSinceEpoch(decemberFirst(), QTimeZone::UTC);
    QVERIFY(!BUsageStatus::projection(freeTier(10 * GB), after));
}

void BUsageStatusTest::projectionNeedsMonthAndReset()
{
    BUsage::Info noReset = freeTier(10 * GB);
    noReset.resetsAt = 0;
    QVERIFY(!BUsageStatus::projection(noReset, utc(16)));
    BUsage::Info noMonth = freeTier(10 * GB);
    noMonth.month = QStringLiteral("November");
    QVERIFY(!BUsageStatus::projection(noMonth, utc(16)));
}

void BUsageStatusTest::toolTipFreeTier()
{
    BUsage::Info info = freeTier(80 * GB, 20 * GB);
    info.heldJobs = 0;
    const QString tip = BUsageStatus::toolTip(info, utc(16), QLocale::c());
    const QString resets = QLocale::c().toString(
        QDateTime::fromSecsSinceEpoch(decemberFirst(), QTimeZone::UTC), QLocale::ShortFormat);
    const QStringList expected = {
        QStringLiteral("Licence: free tier"),
        QStringLiteral("Limit: 1 TB per month"),
        QStringLiteral("Written by finished jobs: 80 GB"),
        QStringLiteral("Written by running jobs so far: 20 GB"),
        QStringLiteral("Used: 10 % of the limit"),
        QStringLiteral("Projected by the end of the month: 200 GB"),
        QStringLiteral("Backup jobs held this month: 0"),
        QStringLiteral("Resets on ") + resets,
    };
    QCOMPARE(tip.split(QLatin1Char('\n')), expected);
}

void BUsageStatusTest::toolTipProjectionAboveLimit()
{
    const QString tip = BUsageStatus::toolTip(freeTier(600 * GB), utc(16), QLocale::c());
    QVERIFY2(tip.contains(QStringLiteral("Projected by the end of the month: 1.2 TB, above the limit")),
             qPrintable(tip));
}

void BUsageStatusTest::toolTipReached()
{
    BUsage::Info info = freeTier(TB + 5 * GB, 0, BUsage::State::Reached);
    info.heldJobs = 3;
    const QString tip = BUsageStatus::toolTip(info, utc(20), QLocale::c());
    QVERIFY(tip.contains(QStringLiteral("Used: 100 % of the limit")));
    QVERIFY(tip.contains(QStringLiteral("Backup jobs held this month: 3")));
    QVERIFY(tip.endsWith(QStringLiteral(
        "Limit reached: new backup jobs are held until the reset. Restores are not affected.")));
}

void BUsageStatusTest::toolTipLicensedUnlimited()
{
    BUsage::Info info = freeTier(2300 * GB);
    info.licence = BUsage::Licence::Licensed;
    info.customer = QStringLiteral("Example Ltd");
    info.limitBytes = 0;
    const QString tip = BUsageStatus::toolTip(info, utc(16), QLocale::c());
    const QStringList lines = tip.split(QLatin1Char('\n'));
    QCOMPARE(lines.value(0), QStringLiteral("Licence: Example Ltd"));
    QCOMPARE(lines.value(1), QStringLiteral("Limit: none"));
    QVERIFY(!tip.contains(QStringLiteral("% of the limit")));
    QVERIFY(!tip.contains(QStringLiteral("above the limit")));

    info.validUntil = QDateTime(QDate(2027, 12, 31), QTime(0, 0), QTimeZone::UTC).toSecsSinceEpoch();
    const QString until = BUsageStatus::toolTip(info, utc(16), QLocale::c()).split(QLatin1Char('\n')).value(0);
    QVERIFY2(until.startsWith(QStringLiteral("Licence: Example Ltd, valid until ")), qPrintable(until));
}

void BUsageStatusTest::toolTipExpired()
{
    BUsage::Info info = freeTier(10 * GB);
    info.licence = BUsage::Licence::Expired;
    info.licenceMessage = QStringLiteral("licence expired on 2026-10-31");
    const QStringList lines = BUsageStatus::toolTip(info, utc(16), QLocale::c()).split(QLatin1Char('\n'));
    QCOMPARE(lines.value(0), QStringLiteral("Licence: expired, the free tier applies"));
    QCOMPARE(lines.value(1), QStringLiteral("licence expired on 2026-10-31"));
    QCOMPARE(lines.value(2), QStringLiteral("Limit: 1 TB per month"));
}

#ifdef HAVE_GRPC
void BUsageStatusTest::protoFull()
{
    using namespace onesimus::director::usage;
    LicenceInfo licence;
    licence.setKind(LicenceKindGadget::LicenceKind::LICENCE_KIND_LICENSED);
    licence.setCustomer(QStringLiteral("Example Ltd"));
    licence.setLicenceId(QStringLiteral("L-1"));
    licence.setValidUntil(1830000000);
    UsageInfo message;
    message.setMonth(QStringLiteral("2026-11"));
    message.setBytesWritten(73600000000ULL);
    message.setLimitBytes(10 * TB);
    message.setState(UsageStateGadget::UsageState::USAGE_STATE_NEAR);
    message.setResetsAt(decemberFirst());
    message.setLicence(licence);
    message.setBytesRunning(5 * GB);
    message.setHeldJobsThisMonth(2);

    const BUsage::Info info = BUsageProto::fromMessage(message);
    QVERIFY(info.present);
    QCOMPARE(info.month, QStringLiteral("2026-11"));
    QCOMPARE(info.bytesWritten, 73600000000ULL);
    QCOMPARE(info.limitBytes, 10 * TB);
    QCOMPARE(info.state, BUsage::State::Near);
    QCOMPARE(info.resetsAt, decemberFirst());
    QCOMPARE(info.bytesRunning, 5 * GB);
    QCOMPARE(info.heldJobs, quint32(2));
    QCOMPARE(info.licence, BUsage::Licence::Licensed);
    QCOMPARE(info.customer, QStringLiteral("Example Ltd"));
    QCOMPARE(info.licenceId, QStringLiteral("L-1"));
    QCOMPARE(info.validUntil, qint64(1830000000));
}

void BUsageStatusTest::protoAbsentZeroFields()
{
    // Over the wire, as odir sends it: zero fields are not encoded at all
    using namespace onesimus::director::usage;
    LicenceInfo licence;
    licence.setKind(LicenceKindGadget::LicenceKind::LICENCE_KIND_FREE);
    UsageInfo sent;
    sent.setMonth(QStringLiteral("2026-11"));
    sent.setBytesWritten(73600000000ULL);
    sent.setLimitBytes(TB);
    sent.setState(UsageStateGadget::UsageState::USAGE_STATE_OK);
    sent.setResetsAt(decemberFirst());
    sent.setLicence(licence);

    QProtobufSerializer serializer;
    const QByteArray wire = sent.serialize(&serializer);
    UsageInfo received;
    QVERIFY(received.deserialize(&serializer, wire));

    const BUsage::Info info = BUsageProto::fromMessage(received);
    QCOMPARE(info.bytesRunning, quint64(0));
    QCOMPARE(info.heldJobs, quint32(0));
    QCOMPARE(info.licence, BUsage::Licence::Free);
    QVERIFY(info.customer.isEmpty());
    QCOMPARE(BUsageStatus::text(info, QLocale::c()), QStringLiteral("Usage: 73.6 GB of 1 TB (Nov)"));
    QCOMPARE(BUsageStatus::level(info), BUsageStatus::Level::Ok);
}
#endif

void BUsageStatusTest::licenceHiddenForFreeTierAndFarEnd()
{
    BUsage::Info info = freeTier(10 * GB);
    QCOMPARE(BUsageStatus::licenceLevel(info, utc(16)), BUsageStatus::Level::Hidden);
    QVERIFY(BUsageStatus::licenceText(info, utc(16), QLocale::c()).isEmpty());

    info.licence = BUsage::Licence::Licensed;
    QCOMPARE(BUsageStatus::licenceLevel(info, utc(16)), BUsageStatus::Level::Hidden);   // no end date
    info.validUntil = utc(16).addDays(31).toSecsSinceEpoch();
    QCOMPARE(BUsageStatus::licenceDaysLeft(info, utc(16)), std::optional<int>(31));
    QCOMPARE(BUsageStatus::licenceLevel(info, utc(16)), BUsageStatus::Level::Hidden);

    BUsage::Info none;
    QCOMPARE(BUsageStatus::licenceLevel(none, utc(16)), BUsageStatus::Level::Hidden);
}

void BUsageStatusTest::licenceEndsSoon()
{
    BUsage::Info info = freeTier(10 * GB);
    info.licence = BUsage::Licence::Licensed;
    info.validUntil = utc(16).addDays(30).toSecsSinceEpoch();
    QCOMPARE(BUsageStatus::licenceLevel(info, utc(16)), BUsageStatus::Level::Near);
    const QString text = BUsageStatus::licenceText(info, utc(16), QLocale::c());
    QVERIFY2(text.startsWith(QStringLiteral("Licence ends in 30 day")), qPrintable(text));

    info.validUntil = utc(16).addDays(8).toSecsSinceEpoch();
    QCOMPARE(BUsageStatus::licenceLevel(info, utc(16)), BUsageStatus::Level::Near);
    info.validUntil = utc(16).addDays(7).toSecsSinceEpoch();
    QCOMPARE(BUsageStatus::licenceLevel(info, utc(16)), BUsageStatus::Level::Reached);

    // Partial days round up: 2 hours left is still "1 day"
    info.validUntil = utc(16, 2).toSecsSinceEpoch();
    QCOMPARE(BUsageStatus::licenceDaysLeft(info, utc(16)), std::optional<int>(1));

    // Past its date before the Director says Expired
    info.validUntil = utc(15).toSecsSinceEpoch();
    QCOMPARE(BUsageStatus::licenceDaysLeft(info, utc(16)), std::optional<int>(0));
    QCOMPARE(BUsageStatus::licenceText(info, utc(16), QLocale::c()),
             QStringLiteral("Licence expired: free tier"));
}

void BUsageStatusTest::licenceExpiredAndInvalid()
{
    BUsage::Info info = freeTier(10 * GB);
    info.licence = BUsage::Licence::Expired;
    QCOMPARE(BUsageStatus::licenceLevel(info, utc(16)), BUsageStatus::Level::Reached);
    QCOMPARE(BUsageStatus::licenceText(info, utc(16), QLocale::c()),
             QStringLiteral("Licence expired: free tier"));
    info.licence = BUsage::Licence::Invalid;
    QCOMPARE(BUsageStatus::licenceText(info, utc(16), QLocale::c()),
             QStringLiteral("Licence not valid: free tier"));
}

QTEST_GUILESS_MAIN(BUsageStatusTest)
#include "busagestatus_test.moc"
