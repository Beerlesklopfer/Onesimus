/**
 * @file bdirectorqueue_test.cpp
 * @brief Command queue of BareosDirector without a connection
 *
 * Covers what can be reached without a Director: a command that cannot be
 * sent is dropped and reported instead of staying in the queue as "sent".
 * Clearing the queue when an established session ends and the API mode
 * timeout need an authenticated session and are not covered here.
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

QTEST_GUILESS_MAIN(BDirectorQueueTest)
#include "bdirectorqueue_test.moc"
