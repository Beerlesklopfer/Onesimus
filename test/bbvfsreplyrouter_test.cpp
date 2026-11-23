// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bbvfsreplyrouter_test.cpp
 * @brief Replies of BVFS and restore commands matched to their requests (P0-15)
 *
 * The router assigns replies by their order per command, as the Director
 * answers them (BareosDirector::dequeueCurrentCommand() takes the oldest sent
 * command). An error reply is followed by commandFailed() for the same
 * command (BareosDirector::processDirectorMessage()); a refused command gets
 * only commandFailed() (flushQueue()).
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>
#include <QSignalSpy>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "jobs/bbvfsreplyrouter.h"

using Command = BDirector::Command;

namespace {

struct Sent {
    Command cmd;
    QString args;
};

QString jobIdsReply(const QStringList &ids)
{
    QJsonArray array;
    for (const QString &id : ids)
        array.append(QJsonObject{{"id", id}});
    return QString::fromUtf8(QJsonDocument(QJsonObject{{"result", QJsonObject{{"jobids", array}}}})
                                 .toJson(QJsonDocument::Compact));
}

} // namespace

class BBvfsReplyRouterTest : public QObject
{
    Q_OBJECT

private slots:
    void routerAssignsRepliesInOrderPerCommand();
    void routerDropsReplyNobodyAskedFor();
    void routerIgnoresFailureReportOfAnErrorReply();
    void routerFailsRefusedCommand();
    void routerResetFailsEverythingPending();
    void routerDoesNotTrackOtherCommands();
};

void BBvfsReplyRouterTest::routerAssignsRepliesInOrderPerCommand()
{
    QList<Sent> sent;
    BBvfsReplyRouter router([&](Command c, const QString &a) { sent.append({c, a}); });
    QSignalSpy replied(&router, &BBvfsReplyRouter::replied);

    const quint64 a = router.send(Command::BvfsLsDirs, "A");
    const quint64 b = router.send(Command::BvfsLsFiles, "B");
    const quint64 c = router.send(Command::BvfsLsDirs, "C");
    QCOMPARE(sent.size(), 3);
    QVERIFY(a && b && c && a != b && b != c);

    // A reply of another kind in between does not shift the lsdirs replies
    router.handleReply(Command::BvfsLsDirs, "ra", true);
    router.handleReply(Command::BvfsLsFiles, "rb", true);
    router.handleReply(Command::BvfsLsDirs, "rc", true);

    QCOMPARE(replied.size(), 3);
    QCOMPARE(replied.at(0).at(0).toULongLong(), a);
    QCOMPARE(replied.at(0).at(2).toString(), QStringLiteral("ra"));
    QCOMPARE(replied.at(1).at(0).toULongLong(), b);
    QCOMPARE(replied.at(2).at(0).toULongLong(), c);
    QCOMPARE(router.pendingCount(), 0);
}

void BBvfsReplyRouterTest::routerDropsReplyNobodyAskedFor()
{
    BBvfsReplyRouter router([](Command, const QString &) {});
    QSignalSpy replied(&router, &BBvfsReplyRouter::replied);

    router.handleReply(Command::BvfsGetJobIds, jobIdsReply({"1"}), true);
    QCOMPARE(replied.size(), 0);
}

void BBvfsReplyRouterTest::routerIgnoresFailureReportOfAnErrorReply()
{
    BBvfsReplyRouter router([](Command, const QString &) {});
    QSignalSpy replied(&router, &BBvfsReplyRouter::replied);
    QSignalSpy failed(&router, &BBvfsReplyRouter::failed);

    router.send(Command::BvfsLsDirs, "jobid=1 pathid=2");
    const quint64 second = router.send(Command::BvfsLsDirs, "jobid=1 pathid=2");

    // The Director emits the error reply, then commandFailed for the same command
    router.handleReply(Command::BvfsLsDirs, R"({"error":{"message":"failed"}})", true);
    router.handleCommandFailed(Command::BvfsLsDirs, "jobid=1 pathid=2", "failed");

    QCOMPARE(replied.size(), 1);
    QCOMPARE(failed.size(), 0);
    QCOMPARE(router.pendingCount(), 1);

    // The second identical request still takes its own reply
    router.handleReply(Command::BvfsLsDirs, "ok", true);
    QCOMPARE(replied.at(1).at(0).toULongLong(), second);
}

void BBvfsReplyRouterTest::routerFailsRefusedCommand()
{
    BBvfsReplyRouter router([](Command, const QString &) {});
    QSignalSpy failed(&router, &BBvfsReplyRouter::failed);
    QSignalSpy replied(&router, &BBvfsReplyRouter::replied);

    const quint64 first = router.send(Command::BvfsLsFiles, "x");
    const quint64 second = router.send(Command::BvfsLsFiles, "y");

    // "y" was refused (no connection): no reply will come for it
    router.handleCommandFailed(Command::BvfsLsFiles, "y", "not sent");
    QCOMPARE(failed.size(), 1);
    QCOMPARE(failed.at(0).at(0).toULongLong(), second);

    router.handleReply(Command::BvfsLsFiles, "rx", true);
    QCOMPARE(replied.size(), 1);
    QCOMPARE(replied.at(0).at(0).toULongLong(), first);
}

void BBvfsReplyRouterTest::routerResetFailsEverythingPending()
{
    BBvfsReplyRouter router([](Command, const QString &) {});
    QSignalSpy failed(&router, &BBvfsReplyRouter::failed);
    QSignalSpy replied(&router, &BBvfsReplyRouter::replied);

    router.send(Command::BvfsUpdate, "jobid=1");
    router.send(Command::Restore, "file=?b21");
    router.reset("lost");

    QCOMPARE(failed.size(), 2);
    QCOMPARE(router.pendingCount(), 0);
    router.handleReply(Command::Restore, "late", false);
    QCOMPARE(replied.size(), 0);
}

void BBvfsReplyRouterTest::routerDoesNotTrackOtherCommands()
{
    QList<Sent> sent;
    BBvfsReplyRouter router([&](Command c, const QString &a) { sent.append({c, a}); });
    QCOMPARE(router.send(Command::Custom, ".help all"), quint64(0));
    QVERIFY(sent.isEmpty());
    QVERIFY(BBvfsReplyRouter::isTracked(Command::Restore));
    QVERIFY(BBvfsReplyRouter::isTracked(Command::BvfsCleanup));
    QVERIFY(!BBvfsReplyRouter::isTracked(Command::Custom));
}

QTEST_MAIN(BBvfsReplyRouterTest)
#include "bbvfsreplyrouter_test.moc"
