// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bjobcancel_test.cpp
 * @brief The toolbar Cancel of the job list: every selected job, real result (P1-20)
 *
 * Before the fix the toolbar took an arbitrary element of the selection (a
 * QSet) and always reported "cancelled". The error replies follow the
 * Onesimus Director (crates/odir/src/json_output.rs, "JobId 1 is not
 * running.").
 *
 * Runs without a Director and without widgets (registered with CTest).
 */

#include <QtTest>

#include "jobs/bjobcancel.h"

namespace {

const QString Accepted = QStringLiteral(
    R"({"jsonrpc":"2.0","id":null,"result":{"cancel":{"jobid":7}}})");
const QString NotRunning = QStringLiteral(
    R"({"jsonrpc":"2.0","id":null,"error":{"code":1,"message":"failed","data":{"result":{},"messages":{"error":["JobId 12 is not running.\n"]}}}})");

} // namespace

class BJobCancelTest : public QObject
{
    Q_OBJECT

private slots:
    void everySelectedJobInOrder();
    void repliesAreMatchedInOrder();
    void failureWithoutReply();
    void errorReplyIsNotCountedTwice();
    void foreignReplyIsIgnored();
};

void BJobCancelTest::everySelectedJobInOrder()
{
    const QSet<QString> selection = {"12", "7", "100", " 9 ", "abc", "0", "7"};
    QCOMPARE(BJobCancel::orderedJobIds(selection), QStringList({"7", "9", "12", "100"}));
    QVERIFY(BJobCancel::orderedJobIds({}).isEmpty());
}

void BJobCancelTest::repliesAreMatchedInOrder()
{
    BJobCancel cancel;
    cancel.start({"7", "12"});
    QVERIFY(cancel.isRunning());

    QVERIFY(cancel.takeReply(Accepted));
    QVERIFY(cancel.isRunning());
    QVERIFY(cancel.takeReply(NotRunning));
    QVERIFY(!cancel.isRunning());

    QCOMPARE(cancel.requested(), QStringList({"7"}));
    QCOMPARE(cancel.failed().size(), 1);
    QCOMPARE(cancel.failed().at(0).first, QStringLiteral("12"));
    QCOMPARE(cancel.failed().at(0).second, QStringLiteral("JobId 12 is not running."));
}

void BJobCancelTest::failureWithoutReply()
{
    // Refused before it was sent, or the connection was lost
    BJobCancel cancel;
    cancel.start({"7", "12"});
    QVERIFY(cancel.takeFailure("12", "not connected"));
    QVERIFY(cancel.takeReply(Accepted));   // the remaining reply is 7's
    QVERIFY(!cancel.isRunning());
    QCOMPARE(cancel.requested(), QStringList({"7"}));
    QCOMPARE(cancel.failed().at(0).first, QStringLiteral("12"));
}

void BJobCancelTest::errorReplyIsNotCountedTwice()
{
    // An error reply comes as jsonResult and then as commandFailed: the
    // second finds the job no longer open
    BJobCancel cancel;
    cancel.start({"12"});
    QVERIFY(cancel.takeReply(NotRunning));
    QVERIFY(!cancel.takeFailure("12", NotRunning));
    QCOMPARE(cancel.failed().size(), 1);
}

void BJobCancelTest::foreignReplyIsIgnored()
{
    BJobCancel cancel;
    QVERIFY(!cancel.takeReply(Accepted));
    QVERIFY(!cancel.takeFailure("7", "x"));
    QVERIFY(cancel.requested().isEmpty());
}

QTEST_APPLESS_MAIN(BJobCancelTest)
#include "bjobcancel_test.moc"
