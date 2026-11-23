// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bdirectorqueue_test.cpp
 * @brief Command queue of BareosDirector without a connection
 *
 * Covers what can be reached without a Director: a command that cannot be
 * sent is dropped and reported instead of staying in the queue as "sent".
 * Clearing the queue when an established session ends and the API mode
 * timeout need an authenticated session and are not covered here. The same
 * goes for the way a reply to the job history request (Command::LlistJobs)
 * leaves through jobHistoryResult() only: a reply needs a sent command.
 *
 * The console line of the job history request is checked here. Its form is
 * taken from the Bareos sources (branch bareos-25, core/src/dird/
 * ua_output.cc: "days=", "limit=", "reverse" of "list jobs"/"llist jobs")
 * and from the Onesimus Director (crates/odir/src/commands/list.rs); it was
 * not sent to a Director.
 *
 * Runs without a Director and without network traffic (registered with CTest).
 */

#include <QtTest>
#include <QSignalSpy>

#include "director/bareosdirector.h"

class BDirectorQueueTest : public QObject
{
    Q_OBJECT

private slots:
    void refusedCommandLeavesNoQueueEntry();
    void everyRefusedCommandIsReported();
    void disconnectWithoutConnectionKeepsQueueEmpty();
    void jobHistoryCommandLine_data();
    void jobHistoryCommandLine();
    void refusedJobHistoryRequestIsReportedWithItsArguments();
};

void BDirectorQueueTest::refusedCommandLeavesNoQueueEntry()
{
    BareosDirector director;
    director.initialize();   // socket and timers in this thread; no connection
    QSignalSpy failed(&director, &BareosDirector::commandFailed);
    QSignalSpy status(&director, &BareosDirector::statusMessage);

    director.doSend(BareosDirector::Command::ListJobId, "42");

    // Before the fix the entry stayed in the queue marked "sent" and took
    // the reply of the next command
    QCOMPARE(director.pendingCommandCount(), 0);
    QCOMPARE(failed.count(), 1);
    QCOMPARE(failed.at(0).at(0).value<BareosDirector::Command>(), BareosDirector::Command::ListJobId);
    QCOMPARE(failed.at(0).at(1).toString(), QString("42"));
    QVERIFY(!failed.at(0).at(2).toString().isEmpty());
    QCOMPARE(status.count(), 1);
}

void BDirectorQueueTest::everyRefusedCommandIsReported()
{
    BareosDirector director;
    director.initialize();
    QSignalSpy failed(&director, &BareosDirector::commandFailed);

    director.doSend(BareosDirector::Command::DotJobs);
    director.doSend(BareosDirector::Command::DotClients);
    director.doSend(BareosDirector::Command::Custom, "status director");

    QCOMPARE(director.pendingCommandCount(), 0);
    QCOMPARE(failed.count(), 3);
    QCOMPARE(failed.at(0).at(0).value<BareosDirector::Command>(), BareosDirector::Command::DotJobs);
    QCOMPARE(failed.at(1).at(0).value<BareosDirector::Command>(), BareosDirector::Command::DotClients);
    QCOMPARE(failed.at(2).at(1).toString(), QString("status director"));
}

void BDirectorQueueTest::disconnectWithoutConnectionKeepsQueueEmpty()
{
    BareosDirector director;
    director.initialize();
    QSignalSpy failed(&director, &BareosDirector::commandFailed);

    director.disconnect();

    QCOMPARE(director.pendingCommandCount(), 0);
    QCOMPARE(failed.count(), 0);
    QCOMPARE(director.connectionState(), BareosDirector::Disconnected);
}

void BDirectorQueueTest::jobHistoryCommandLine_data()
{
    QTest::addColumn<QString>("args");
    QTest::addColumn<QString>("line");

    // What the schedule view sends: "<days>,<limit>"
    QTest::newRow("default window") << "60,5000" << "llist jobs days=60 reverse limit=5000";
    QTest::newRow("one week") << "7,5000" << "llist jobs days=7 reverse limit=5000";
    QTest::newRow("a year") << "365,20000" << "llist jobs days=365 reverse limit=20000";
    QTest::newRow("spaces") << " 30 , 100 " << "llist jobs days=30 reverse limit=100";

    // Never unbounded: a missing or unusable number becomes its default
    QTest::newRow("no arguments") << "" << "llist jobs days=60 reverse limit=5000";
    QTest::newRow("days only") << "14" << "llist jobs days=14 reverse limit=5000";
    QTest::newRow("no days") << ",200" << "llist jobs days=60 reverse limit=200";
    QTest::newRow("zero") << "0,0" << "llist jobs days=60 reverse limit=5000";
    QTest::newRow("negative") << "-5,-1" << "llist jobs days=60 reverse limit=5000";
    QTest::newRow("words") << "all,everything" << "llist jobs days=60 reverse limit=5000";
    // Nothing of the arguments reaches the line but two numbers
    QTest::newRow("injection") << "60 jobstatus=T,5000 yes" << "llist jobs days=60 reverse limit=5000";
    // Bareos computes the window in a C int; huge values are capped
    QTest::newRow("too large") << "999999,99999999" << "llist jobs days=3650 reverse limit=100000";
}

void BDirectorQueueTest::jobHistoryCommandLine()
{
    QFETCH(QString, args);
    QFETCH(QString, line);

    QCOMPARE(BareosDirector::jobHistoryCommandLine(args), line);
}

void BDirectorQueueTest::refusedJobHistoryRequestIsReportedWithItsArguments()
{
    BareosDirector director;
    director.initialize();
    QSignalSpy failed(&director, &BareosDirector::commandFailed);
    QSignalSpy history(&director, &BareosDirector::jobHistoryResult);

    director.doSend(BareosDirector::Command::LlistJobs, "60,5000");

    // The schedule view recognises its request by these arguments and
    // shows "no history" with the reason
    QCOMPARE(director.pendingCommandCount(), 0);
    QCOMPARE(failed.count(), 1);
    QCOMPARE(failed.at(0).at(0).value<BareosDirector::Command>(), BareosDirector::Command::LlistJobs);
    QCOMPARE(failed.at(0).at(1).toString(), QString("60,5000"));
    QVERIFY(!failed.at(0).at(2).toString().isEmpty());
    QCOMPARE(history.count(), 0);
}

QTEST_GUILESS_MAIN(BDirectorQueueTest)
#include "bdirectorqueue_test.moc"
