// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bbyteformat_test.cpp
 * @brief Test for BByteFormat: byte counts in binary units with IEC names,
 *        and the size suffixes of Bareos directives
 *
 * Bareos reads k/m/g/t as binary and kb/mb/gb/tb as decimal
 * (core/src/lib/edit.cc, size_to_uint64): "Maximum Volume Bytes = 50G" must
 * appear as "50 GiB", not as "53.7 GB".
 */

#include <QtTest>

#include <limits>

#include "bbyteformat.h"

class BByteFormatTest : public QObject
{
    Q_OBJECT

private slots:
    void bareosFiftyGIsFiftyGiB();
    void format_data();
    void format();
    void formatDecimals();
    void formatDecimal_data();
    void formatDecimal();
    void parse_data();
    void parse();
    void parseRejects_data();
    void parseRejects();
    void realVolumeSizes();
};

void BByteFormatTest::bareosFiftyGIsFiftyGiB()
{
    quint64 bytes = 0;
    QVERIFY(BByteFormat::parseBareosSize(QStringLiteral("50G"), &bytes));
    QCOMPARE(bytes, quint64(50) * 1024 * 1024 * 1024);
    QCOMPARE(BByteFormat::format(qint64(bytes)), QStringLiteral("50 GiB"));
}

void BByteFormatTest::format_data()
{
    QTest::addColumn<qint64>("bytes");
    QTest::addColumn<QString>("text");

    QTest::newRow("zero") << qint64(0) << "0 B";
    QTest::newRow("one byte") << qint64(1) << "1 B";
    QTest::newRow("largest byte count") << qint64(1023) << "1023 B";
    QTest::newRow("one KiB") << qint64(1024) << "1 KiB";
    QTest::newRow("one and a half KiB") << qint64(1536) << "1.5 KiB";
    QTest::newRow("rounds down") << qint64(1024 + 51) << "1 KiB";          // 1.0498
    QTest::newRow("rounds up") << qint64(1024 + 52) << "1.1 KiB";          // 1.0508
    QTest::newRow("just below MiB moves up") << qint64(1048575) << "1 MiB";
    QTest::newRow("one MiB") << qint64(1048576) << "1 MiB";
    QTest::newRow("just below GiB moves up") << qint64(1073741823) << "1 GiB";
    QTest::newRow("one TiB") << qint64(1099511627776LL) << "1 TiB";
    QTest::newRow("one PiB") << qint64(1125899906842624LL) << "1 PiB";
    QTest::newRow("one EiB") << qint64(1152921504606846976LL) << "1 EiB";
    QTest::newRow("largest qint64") << std::numeric_limits<qint64>::max() << "8 EiB";
    QTest::newRow("negative") << qint64(-1536) << "-1.5 KiB";
    QTest::newRow("negative bytes") << qint64(-5) << "-5 B";
    QTest::newRow("smallest qint64") << std::numeric_limits<qint64>::min() << "-8 EiB";
}

void BByteFormatTest::format()
{
    QFETCH(qint64, bytes);
    QFETCH(QString, text);
    QCOMPARE(BByteFormat::format(bytes), text);
}

void BByteFormatTest::formatDecimals()
{
    // 1.2345 GiB
    const qint64 bytes = qint64(1.2345 * 1073741824.0);
    QCOMPARE(BByteFormat::format(bytes, 0), QStringLiteral("1 GiB"));
    QCOMPARE(BByteFormat::format(bytes, 1), QStringLiteral("1.2 GiB"));
    QCOMPARE(BByteFormat::format(bytes, 2), QStringLiteral("1.23 GiB"));
    QCOMPARE(BByteFormat::format(bytes, 3), QStringLiteral("1.234 GiB"));
    // Out of range is clamped, never a crash or a scientific notation
    QCOMPARE(BByteFormat::format(bytes, -4), QStringLiteral("1 GiB"));
    QCOMPARE(BByteFormat::format(bytes, 12), QStringLiteral("1.234 GiB"));
    // Trailing zeros go, the point with them
    QCOMPARE(BByteFormat::format(qint64(2) * 1048576, 2), QStringLiteral("2 MiB"));
    QCOMPARE(BByteFormat::format(qint64(2.5 * 1048576), 3), QStringLiteral("2.5 MiB"));
}

void BByteFormatTest::parse_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<quint64>("bytes");

    QTest::newRow("plain number") << "512" << quint64(512);
    QTest::newRow("binary k") << "1k" << quint64(1024);
    QTest::newRow("decimal kb") << "1kb" << quint64(1000);
    QTest::newRow("upper case") << "2M" << quint64(2 * 1048576);
    QTest::newRow("decimal MB upper") << "2MB" << quint64(2000000);
    QTest::newRow("space before suffix") << "50 G" << quint64(50) * 1073741824ULL;
    QTest::newRow("decimal gb") << "50gb" << quint64(50000000000ULL);
    QTest::newRow("binary t") << "3t" << quint64(3) * 1099511627776ULL;
    QTest::newRow("fraction") << "1.5g" << quint64(1610612736ULL);
    QTest::newRow("two parts") << "1g 512m" << quint64(1073741824ULL + 536870912ULL);
    QTest::newRow("surrounding space") << "  100G  " << quint64(100) * 1073741824ULL;
    QTest::newRow("zero") << "0" << quint64(0);
}

void BByteFormatTest::parse()
{
    QFETCH(QString, text);
    QFETCH(quint64, bytes);
    quint64 parsed = 1;
    QVERIFY(BByteFormat::parseBareosSize(text, &parsed));
    QCOMPARE(parsed, bytes);
}

void BByteFormatTest::parseRejects_data()
{
    QTest::addColumn<QString>("text");
    QTest::newRow("empty") << "";
    QTest::newRow("blank") << "   ";
    QTest::newRow("unknown suffix") << "50x";
    QTest::newRow("IEC name is not Bareos syntax") << "50GiB";
    QTest::newRow("suffix without number") << "G";
    QTest::newRow("two points") << "1.2.3g";
    QTest::newRow("negative") << "-5g";
    QTest::newRow("too large") << "20e";
}

void BByteFormatTest::parseRejects()
{
    QFETCH(QString, text);
    quint64 parsed = 77;
    QVERIFY(!BByteFormat::parseBareosSize(text, &parsed));
    QCOMPARE(parsed, quint64(77));   // untouched on failure
}

void BByteFormatTest::realVolumeSizes()
{
    // Volume sizes from the production Director (PDC, 19.11.2026): a full
    // 50G volume holds a few bytes more than 50 GiB of payload headers
    QCOMPARE(BByteFormat::format(53686042829LL), QStringLiteral("50 GiB"));
    QCOMPARE(BByteFormat::format(51233279615LL), QStringLiteral("47.7 GiB"));
    QCOMPARE(BByteFormat::format(14009488LL), QStringLiteral("13.4 MiB"));
    QCOMPARE(BByteFormat::format(207LL), QStringLiteral("207 B"));
}

void BByteFormatTest::formatDecimal_data()
{
    QTest::addColumn<quint64>("bytes");
    QTest::addColumn<QString>("text");

    QTest::newRow("zero") << quint64(0) << "0 B";
    QTest::newRow("largest byte count") << quint64(999) << "999 B";
    QTest::newRow("one kB") << quint64(1000) << "1 kB";
    QTest::newRow("just below MB moves up") << quint64(999999) << "1 MB";
    QTest::newRow("usage of the month") << quint64(73600000000ULL) << "73.6 GB";
    QTest::newRow("free tier limit") << quint64(1000000000000ULL) << "1 TB";
    QTest::newRow("binary TiB in decimal") << quint64(1099511627776ULL) << "1.1 TB";
    QTest::newRow("largest quint64") << std::numeric_limits<quint64>::max() << "18.4 EB";
}

void BByteFormatTest::formatDecimal()
{
    QFETCH(quint64, bytes);
    QFETCH(QString, text);
    QCOMPARE(BByteFormat::formatDecimal(bytes), text);
}

QTEST_GUILESS_MAIN(BByteFormatTest)
#include "bbyteformat_test.moc"
