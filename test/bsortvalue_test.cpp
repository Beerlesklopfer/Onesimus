// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bsortvalue_test.cpp
 * @brief Table sorting by raw values through a sort role
 *
 * The jobs and clients tables sort a QSortFilterProxyModel by the model's
 * sort role; the storage table sorts QTableWidget items by their key. The
 * tests build the same arrangement from the keys of BSortValue and check
 * the resulting order against the order the display text would give.
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>

#include <QJsonValue>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QTableWidgetItem>

#include "bsorttableitem.h"
#include "bsortvalue.h"

namespace {

const int SortRole = Qt::UserRole;

struct Cell {
    QString display;    ///< What the table shows
    qint64 key;         ///< What the model returns under the sort role
};

/** Display texts of column 0 in the order the proxy shows them. */
QStringList sortedDisplay(const QList<Cell> &cells, int sortRole, Qt::SortOrder order)
{
    QStandardItemModel model;
    for (const Cell &cell : cells) {
        auto *item = new QStandardItem(cell.display);
        item->setData(cell.key, SortRole);
        model.appendRow(item);
    }

    QSortFilterProxyModel proxy;
    proxy.setSourceModel(&model);
    proxy.setSortRole(sortRole);
    proxy.sort(0, order);

    QStringList result;
    for (int row = 0; row < proxy.rowCount(); ++row) {
        result << proxy.index(row, 0).data(Qt::DisplayRole).toString();
    }
    return result;
}

QString formatBytes(qint64 bytes)
{
    if (bytes >= 1024LL * 1024 * 1024) {
        return QString::number(bytes / (1024.0 * 1024 * 1024), 'f', 2) + " GB";
    }
    if (bytes >= 1024 * 1024) {
        return QString::number(bytes / (1024.0 * 1024), 'f', 2) + " MB";
    }
    if (bytes >= 1024) {
        return QString::number(bytes / 1024.0, 'f', 2) + " KB";
    }
    return QString::number(bytes) + " B";
}

} // namespace

class BSortValueTest : public QObject
{
    Q_OBJECT

private slots:
    void integerFromText();
    void integerFromJson();
    void timestampKeys();
    void durationKeys_data();
    void durationKeys();
    void jobIdsSortNumerically();
    void sizesSortByBytes();
    void datesSortChronologically();
    void durationsSortByLength();
    void missingValuesSortFirst();
    void tableItemsSortByKey();
    void tableItemWithoutKeyFallsBackToText();
};

void BSortValueTest::integerFromText()
{
    QCOMPARE(BSortValue::integer(QStringLiteral("42")), qint64(42));
    QCOMPARE(BSortValue::integer(QStringLiteral(" 1024 ")), qint64(1024));
    QCOMPARE(BSortValue::integer(QStringLiteral("0")), qint64(0));
    QCOMPARE(BSortValue::integer(QStringLiteral("98765432109876")), qint64(98765432109876));
    QCOMPARE(BSortValue::integer(QString()), BSortValue::Missing);
    QCOMPARE(BSortValue::integer(QStringLiteral("1.5 GB")), BSortValue::Missing);
}

void BSortValueTest::integerFromJson()
{
    // The Director sends numbers as strings in API mode and as numbers over gRPC
    QCOMPARE(BSortValue::integer(QJsonValue(QStringLiteral("2163"))), qint64(2163));
    QCOMPARE(BSortValue::integer(QJsonValue(2163)), qint64(2163));
    QCOMPARE(BSortValue::integer(QJsonValue(5368709120.0)), qint64(5368709120));
    QCOMPARE(BSortValue::integer(QJsonValue()), BSortValue::Missing);
    QCOMPARE(BSortValue::integer(QJsonValue(QJsonValue::Undefined)), BSortValue::Missing);
}

void BSortValueTest::timestampKeys()
{
    const qint64 earlier = BSortValue::timestamp(QStringLiteral("2026-01-31 23:59:59"));
    const qint64 later = BSortValue::timestamp(QStringLiteral("2026-02-01 00:00:00"));
    QVERIFY(earlier != BSortValue::Missing);
    QCOMPARE(later - earlier, qint64(1000));

    // ISO form of the same instant
    QCOMPARE(BSortValue::timestamp(QStringLiteral("2026-02-01T00:00:00")), later);

    QCOMPARE(BSortValue::timestamp(QString()), BSortValue::Missing);
    QCOMPARE(BSortValue::timestamp(QStringLiteral("0")), BSortValue::Missing);
    QCOMPARE(BSortValue::timestamp(QStringLiteral("0000-00-00 00:00:00")), BSortValue::Missing);
    QCOMPARE(BSortValue::timestamp(QStringLiteral("soon")), BSortValue::Missing);
}

void BSortValueTest::durationKeys_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<qint64>("seconds");

    QTest::newRow("seconds") << "00:00:07" << qint64(7);
    QTest::newRow("minutes") << "00:12:00" << qint64(720);
    QTest::newRow("hours") << "03:00:01" << qint64(10801);
    QTest::newRow("more than a day") << "120:05:00" << qint64(432300);
    QTest::newRow("one day prefix") << "1 day 02:00:00" << qint64(93600);
    QTest::newRow("days prefix") << "3 days 00:00:10" << qint64(259210);
    QTest::newRow("fraction") << "00:00:07.5" << qint64(7);
    QTest::newRow("empty") << "" << BSortValue::Missing;
    QTest::newRow("dash") << "-" << BSortValue::Missing;
}

void BSortValueTest::durationKeys()
{
    QFETCH(QString, text);
    QFETCH(qint64, seconds);

    QCOMPARE(BSortValue::durationSeconds(text), seconds);
}

void BSortValueTest::jobIdsSortNumerically()
{
    QList<Cell> cells;
    for (const char *id : {"100", "2", "10", "1", "21"}) {
        cells << Cell{QString::fromLatin1(id), BSortValue::integer(QString::fromLatin1(id))};
    }

    QCOMPARE(sortedDisplay(cells, SortRole, Qt::AscendingOrder),
             QStringList({"1", "2", "10", "21", "100"}));
    QCOMPARE(sortedDisplay(cells, SortRole, Qt::DescendingOrder),
             QStringList({"100", "21", "10", "2", "1"}));

    // What the table did before: the display text orders 1, 10, 100, 2, 21
    QCOMPARE(sortedDisplay(cells, Qt::DisplayRole, Qt::AscendingOrder),
             QStringList({"1", "10", "100", "2", "21"}));
}

void BSortValueTest::sizesSortByBytes()
{
    QList<Cell> cells;
    for (const char *bytes : {"1048576", "999", "5368709120", "2048", "10"}) {
        const qint64 key = BSortValue::integer(QJsonValue(QString::fromLatin1(bytes)));
        cells << Cell{formatBytes(key), key};
    }

    QCOMPARE(sortedDisplay(cells, SortRole, Qt::AscendingOrder),
             QStringList({"10 B", "999 B", "2.00 KB", "1.00 MB", "5.00 GB"}));

    // As text "1.00 MB" comes before "10 B" and "999 B" is the largest
    const QStringList asText = sortedDisplay(cells, Qt::DisplayRole, Qt::AscendingOrder);
    QCOMPARE(asText.first(), QStringLiteral("1.00 MB"));
    QCOMPARE(asText.last(), QStringLiteral("999 B"));
}

void BSortValueTest::datesSortChronologically()
{
    // Shown day first, as a German locale does: text order is not time order
    QList<Cell> cells;
    for (const char *stamp : {"2026-02-01 08:00:00", "2025-12-31 23:00:00",
                              "2026-01-15 12:00:00", "2026-01-15 09:30:00"}) {
        const QString text = QString::fromLatin1(stamp);
        const QDateTime when = QDateTime::fromString(text, "yyyy-MM-dd HH:mm:ss");
        cells << Cell{when.toString("dd.MM.yy HH:mm"), BSortValue::timestamp(text)};
    }

    QCOMPARE(sortedDisplay(cells, SortRole, Qt::AscendingOrder),
             QStringList({"31.12.25 23:00", "15.01.26 09:30", "15.01.26 12:00", "01.02.26 08:00"}));
    QCOMPARE(sortedDisplay(cells, SortRole, Qt::DescendingOrder).first(),
             QStringLiteral("01.02.26 08:00"));

    // As text the newest entry sorts first
    QCOMPARE(sortedDisplay(cells, Qt::DisplayRole, Qt::AscendingOrder).first(),
             QStringLiteral("01.02.26 08:00"));
}

void BSortValueTest::durationsSortByLength()
{
    QList<Cell> cells;
    for (const char *duration : {"100:00:00", "00:59:59", "1 day 00:00:00", "02:00:00"}) {
        const QString text = QString::fromLatin1(duration);
        cells << Cell{text, BSortValue::durationSeconds(text)};
    }

    QCOMPARE(sortedDisplay(cells, SortRole, Qt::AscendingOrder),
             QStringList({"00:59:59", "02:00:00", "1 day 00:00:00", "100:00:00"}));
}

void BSortValueTest::missingValuesSortFirst()
{
    const QList<Cell> cells = {
        {"7", BSortValue::integer(QStringLiteral("7"))},
        {"-", BSortValue::integer(QStringLiteral("-"))},
        {"0", BSortValue::integer(QStringLiteral("0"))},
    };

    QCOMPARE(sortedDisplay(cells, SortRole, Qt::AscendingOrder), QStringList({"-", "0", "7"}));
}

void BSortValueTest::tableItemsSortByKey()
{
    // The storage table: formatted sizes, compared by bytes
    const BSortTableItem kilobytes(formatBytes(2048), 2048);
    const BSortTableItem megabyte(formatBytes(1048576), 1048576);
    const BSortTableItem bytes(formatBytes(999), 999);

    QVERIFY(bytes < kilobytes);
    QVERIFY(kilobytes < megabyte);
    QVERIFY(bytes < megabyte);
    QVERIFY(!(megabyte < bytes));
    QVERIFY(!(kilobytes < kilobytes));

    // The text alone would put "1.00 MB" before "999 B"
    QVERIFY(megabyte.text() < bytes.text());

    QCOMPARE(megabyte.text(), QStringLiteral("1.00 MB"));
    QCOMPARE(megabyte.data(BSortTableItem::SortRole).toLongLong(), qint64(1048576));
}

void BSortValueTest::tableItemWithoutKeyFallsBackToText()
{
    const BSortTableItem keyed(QStringLiteral("b"), 1);
    const QTableWidgetItem plain(QStringLiteral("a"));

    // No key on the other side: the item behaves like a plain one
    QVERIFY(!(keyed < plain));
}

QTEST_APPLESS_MAIN(BSortValueTest)
#include "bsortvalue_test.moc"
