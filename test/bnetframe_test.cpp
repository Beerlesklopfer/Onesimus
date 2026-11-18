/**
 * @file bnetframe_test.cpp
 * @brief Decision on the length header of a BNET telegram (frame length limit)
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>

#include <limits>

#include "director/bnetframe.h"

using BnetFrame::HeaderKind;

Q_DECLARE_METATYPE(BnetFrame::HeaderKind)

class BNetFrameTest : public QObject
{
    Q_OBJECT

private slots:
    void classify_data();
    void classify();
    void announcedLengthIsBigEndianAndSigned();
    void telegramSizeDoesNotOverflow();
    void limits();
};

void BNetFrameTest::classify_data()
{
    QTest::addColumn<qint32>("length");
    QTest::addColumn<qint64>("limit");
    QTest::addColumn<HeaderKind>("expected");

    const qint64 session = BnetFrame::MaxSessionPayload;
    const qint64 auth = BnetFrame::MaxAuthPayload;
    const qint32 intMax = std::numeric_limits<qint32>::max();
    const qint32 intMin = std::numeric_limits<qint32>::min();

    QTest::newRow("normal") << qint32(120) << session << HeaderKind::Data;
    QTest::newRow("zero") << qint32(0) << session << HeaderKind::Data;
    QTest::newRow("one byte") << qint32(1) << session << HeaderKind::Data;
    QTest::newRow("large JSON telegram (1 MB)") << qint32(1000000) << session << HeaderKind::Data;

    QTest::newRow("signal EOD") << qint32(-1) << session << HeaderKind::Signal;
    QTest::newRow("signal MAIN_PROMPT") << qint32(-18) << session << HeaderKind::Signal;
    QTest::newRow("signal TEXT_INPUT") << qint32(-28) << session << HeaderKind::Signal;
    QTest::newRow("unknown signal") << qint32(-1000) << session << HeaderKind::Signal;
    QTest::newRow("INT_MIN is a signal, not a length") << intMin << session << HeaderKind::Signal;
    QTest::newRow("signal during authentication") << qint32(-1) << auth << HeaderKind::Signal;

    QTest::newRow("session: just under the limit") << qint32(session - 1) << session << HeaderKind::Data;
    QTest::newRow("session: at the limit") << qint32(session) << session << HeaderKind::Data;
    QTest::newRow("session: just over the limit") << qint32(session + 1) << session << HeaderKind::TooLarge;
    QTest::newRow("session: 2 GB") << qint32(0x7ff00000) << session << HeaderKind::TooLarge;
    QTest::newRow("session: INT_MAX - 4") << qint32(intMax - 4) << session << HeaderKind::TooLarge;
    QTest::newRow("session: INT_MAX - 3") << qint32(intMax - 3) << session << HeaderKind::TooLarge;
    QTest::newRow("session: INT_MAX - 1") << qint32(intMax - 1) << session << HeaderKind::TooLarge;
    QTest::newRow("session: INT_MAX") << intMax << session << HeaderKind::TooLarge;

    QTest::newRow("auth: challenge") << qint32(80) << auth << HeaderKind::Data;
    QTest::newRow("auth: just under the limit") << qint32(auth - 1) << auth << HeaderKind::Data;
    QTest::newRow("auth: at the limit") << qint32(auth) << auth << HeaderKind::Data;
    QTest::newRow("auth: just over the limit") << qint32(auth + 1) << auth << HeaderKind::TooLarge;
    QTest::newRow("auth: a session-sized telegram") << qint32(1000000) << auth << HeaderKind::TooLarge;
    QTest::newRow("auth: INT_MAX") << intMax << auth << HeaderKind::TooLarge;
}

void BNetFrameTest::classify()
{
    QFETCH(qint32, length);
    QFETCH(qint64, limit);
    QFETCH(HeaderKind, expected);

    QCOMPARE(BnetFrame::classifyHeader(length, limit), expected);
}

void BNetFrameTest::announcedLengthIsBigEndianAndSigned()
{
    const char data[] = {0x00, 0x00, 0x01, 0x02, 'x'};
    QCOMPARE(BnetFrame::announcedLength(data), qint32(258));

    const char eod[] = {char(0xff), char(0xff), char(0xff), char(0xff)};
    QCOMPARE(BnetFrame::announcedLength(eod), qint32(-1));

    const char mainPrompt[] = {char(0xff), char(0xff), char(0xff), char(0xee)};
    QCOMPARE(BnetFrame::announcedLength(mainPrompt), qint32(-18));

    const char huge[] = {0x7f, char(0xff), char(0xff), char(0xff)};
    QCOMPARE(BnetFrame::announcedLength(huge), std::numeric_limits<qint32>::max());

    const char zero[] = {0x00, 0x00, 0x00, 0x00};
    QCOMPARE(BnetFrame::announcedLength(zero), qint32(0));
}

void BNetFrameTest::telegramSizeDoesNotOverflow()
{
    QCOMPARE(BnetFrame::telegramSize(0), qint64(4));
    QCOMPARE(BnetFrame::telegramSize(100), qint64(104));

    // 4 + INT_MAX overflows a 32-bit int; the result must stay positive
    const qint32 intMax = std::numeric_limits<qint32>::max();
    QCOMPARE(BnetFrame::telegramSize(intMax), qint64(intMax) + 4);
    QVERIFY(BnetFrame::telegramSize(intMax) > qint64(intMax));
    QVERIFY(BnetFrame::telegramSize(intMax - 3) > 0);
}

void BNetFrameTest::limits()
{
    QCOMPARE(BnetFrame::MaxSessionPayload, qint64(16) * 1024 * 1024);
    QCOMPARE(BnetFrame::MaxAuthPayload, qint64(4096));
    // Bareos itself sends at most 1 000 000 bytes per telegram
    QVERIFY(BnetFrame::MaxSessionPayload >= 1000000);
    QVERIFY(BnetFrame::MaxAuthPayload < BnetFrame::MaxSessionPayload);
}

QTEST_APPLESS_MAIN(BNetFrameTest)
#include "bnetframe_test.moc"
