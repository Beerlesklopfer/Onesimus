// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bjobpaging_test.cpp
 * @brief Page arithmetic of the job list pagination
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>
#include <QJsonDocument>
#include <QJsonObject>

#include "jobs/bjobpaging.h"

using BJobPaging::Range;

class BJobPagingTest : public QObject
{
    Q_OBJECT

private slots:
    void pageCount_data();
    void pageCount();
    void refreshKeepsThePage();
    void shorterListFallsBackToTheLastPage();
    void emptyListIsPageZero();
    void negativePageIsPageZero();
    void oldestFirstRange();
    void newestFirstRange();
    void newestFirstLastPageHoldsTheRemainder();
    void rangeClampsThePage();
    void rangeWithoutTotals();
    void everyJobIsOnExactlyOnePage_data();
    void everyJobIsOnExactlyOnePage();
    void jobCountFromReply_data();
    void jobCountFromReply();
    void firstNewestPageEndsAtTheNewestJob();
};

void BJobPagingTest::pageCount_data()
{
    QTest::addColumn<int>("total");
    QTest::addColumn<int>("pageSize");
    QTest::addColumn<int>("pages");

    QTest::newRow("empty") << 0 << 50 << 0;
    QTest::newRow("one job") << 1 << 50 << 1;
    QTest::newRow("exactly one page") << 50 << 50 << 1;
    QTest::newRow("one more") << 51 << 50 << 2;
    QTest::newRow("2163 by 50") << 2163 << 50 << 44;
    QTest::newRow("negative total") << -3 << 50 << 0;
    QTest::newRow("no page size") << 100 << 0 << 0;
}

void BJobPagingTest::pageCount()
{
    QFETCH(int, total);
    QFETCH(int, pageSize);
    QFETCH(int, pages);

    QCOMPARE(BJobPaging::pageCount(total, pageSize), pages);
}

void BJobPagingTest::refreshKeepsThePage()
{
    // The user is on page 4 (index 3) of 2163 jobs; a refresh finds 2170
    QCOMPARE(BJobPaging::clampPage(3, 2170, 50), 3);
    // The last page stays the last page while it exists
    QCOMPARE(BJobPaging::clampPage(43, 2163, 50), 43);
}

void BJobPagingTest::shorterListFallsBackToTheLastPage()
{
    // Page 44 (index 43) of 2163 jobs; after a prune 120 jobs are left: 3 pages
    QCOMPARE(BJobPaging::clampPage(43, 120, 50), 2);
    // Exactly full pages: 100 jobs are pages 0 and 1
    QCOMPARE(BJobPaging::clampPage(2, 100, 50), 1);
}

void BJobPagingTest::emptyListIsPageZero()
{
    QCOMPARE(BJobPaging::clampPage(7, 0, 50), 0);
}

void BJobPagingTest::negativePageIsPageZero()
{
    QCOMPARE(BJobPaging::clampPage(-1, 500, 50), 0);
}

void BJobPagingTest::oldestFirstRange()
{
    Range range = BJobPaging::pageRange(0, 50, 120, false);
    QCOMPARE(range.limit, 50);
    QCOMPARE(range.offset, 0);

    range = BJobPaging::pageRange(2, 50, 120, false);
    QCOMPARE(range.limit, 50);
    QCOMPARE(range.offset, 100);
}

void BJobPagingTest::newestFirstRange()
{
    // Page 0 is the end of the Director's list
    Range range = BJobPaging::pageRange(0, 50, 120, true);
    QCOMPARE(range.limit, 50);
    QCOMPARE(range.offset, 70);

    range = BJobPaging::pageRange(1, 50, 120, true);
    QCOMPARE(range.limit, 50);
    QCOMPARE(range.offset, 20);
}

void BJobPagingTest::newestFirstLastPageHoldsTheRemainder()
{
    Range range = BJobPaging::pageRange(2, 50, 120, true);
    QCOMPARE(range.limit, 20);
    QCOMPARE(range.offset, 0);

    // Fewer jobs than one page
    range = BJobPaging::pageRange(0, 50, 7, true);
    QCOMPARE(range.limit, 7);
    QCOMPARE(range.offset, 0);
}

void BJobPagingTest::rangeClampsThePage()
{
    // Page 9 does not exist for 120 jobs: the request is the last page, as the label shows
    const Range oldest = BJobPaging::pageRange(9, 50, 120, false);
    QCOMPARE(oldest.offset, 100);

    const Range newest = BJobPaging::pageRange(9, 50, 120, true);
    QCOMPARE(newest.limit, 20);
    QCOMPARE(newest.offset, 0);
}

void BJobPagingTest::rangeWithoutTotals()
{
    const Range range = BJobPaging::pageRange(3, 50, 0, true);
    QCOMPARE(range.limit, 50);
    QCOMPARE(range.offset, 0);
}

void BJobPagingTest::everyJobIsOnExactlyOnePage_data()
{
    QTest::addColumn<int>("total");
    QTest::addColumn<int>("pageSize");
    QTest::addColumn<bool>("newestFirst");

    QTest::newRow("120/50 oldest") << 120 << 50 << false;
    QTest::newRow("120/50 newest") << 120 << 50 << true;
    QTest::newRow("100/25 newest") << 100 << 25 << true;
    QTest::newRow("1/200 newest") << 1 << 200 << true;
    QTest::newRow("2163/100 newest") << 2163 << 100 << true;
}

void BJobPagingTest::everyJobIsOnExactlyOnePage()
{
    QFETCH(int, total);
    QFETCH(int, pageSize);
    QFETCH(bool, newestFirst);

    QVector<int> seen(total, 0);
    const int pages = BJobPaging::pageCount(total, pageSize);
    for (int page = 0; page < pages; ++page) {
        const Range range = BJobPaging::pageRange(page, pageSize, total, newestFirst);
        // The Director returns at most what is left after the offset
        for (int i = range.offset; i < qMin(total, range.offset + range.limit); ++i) {
            seen[i]++;
        }
    }
    QCOMPARE(seen.count(1), total);
}

void BJobPagingTest::jobCountFromReply_data()
{
    QTest::addColumn<QByteArray>("result");
    QTest::addColumn<int>("count");

    QTest::newRow("bareos string") << QByteArray(R"({"jobs":[{"count":"720"}]})") << 720;
    QTest::newRow("bareos number") << QByteArray(R"({"jobs":[{"count":720}]})") << 720;
    QTest::newRow("typed") << QByteArray(R"({"count":720})") << 720;
    QTest::newRow("typed string") << QByteArray(R"({"count":" 42 "})") << 42;
    QTest::newRow("zero") << QByteArray(R"({"jobs":[{"count":"0"}]})") << 0;
    QTest::newRow("empty result") << QByteArray(R"({})") << -1;
    QTest::newRow("jobtotals shape") << QByteArray(R"({"jobtotals":{"jobs":"526"}})") << -1;
    QTest::newRow("job rows") << QByteArray(R"({"jobs":[{"jobid":"1"},{"jobid":"2"}]})") << -1;
    QTest::newRow("two count rows") << QByteArray(R"({"jobs":[{"count":"1"},{"count":"2"}]})") << -1;
    QTest::newRow("negative") << QByteArray(R"({"count":-5})") << -1;
    QTest::newRow("fraction") << QByteArray(R"({"count":1.5})") << -1;
    QTest::newRow("text") << QByteArray(R"({"count":"many"})") << -1;
    QTest::newRow("too large") << QByteArray(R"({"count":"99999999999"})") << -1;
}

void BJobPagingTest::jobCountFromReply()
{
    QFETCH(QByteArray, result);
    QFETCH(int, count);
    const QJsonObject obj = QJsonDocument::fromJson(result).object();
    QCOMPARE(BJobPaging::jobCountFromReply(obj), count);
}

// Issue #13: with the 526 of "list jobtotals" the newest first page ended
// at job 526 of 720; with the count of "list jobs" it ends at the newest.
void BJobPagingTest::firstNewestPageEndsAtTheNewestJob()
{
    const int listed = 720;  // list jobs count
    const Range range = BJobPaging::pageRange(0, 200, listed, true);
    QCOMPARE(range.offset, 520);
    QCOMPARE(range.offset + range.limit, listed);
}

QTEST_APPLESS_MAIN(BJobPagingTest)
#include "bjobpaging_test.moc"
