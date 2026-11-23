// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bdirectorsession_test.cpp
 * @brief BareosDirector and BDirector in a real BNET session with a fake Director
 *
 * The fake Director (bfakebnetdirector.h) logs the console in over the
 * loopback interface and answers like the Onesimus Director in API mode 2.
 *
 * - P1-17: a JSON reply cut into telegrams inside UTF-8 characters and
 *   between spaces arrives whole.
 * - P1-5 d: disconnect() says "quit" before it closes the connection.
 * - P1-5 e: a command without a reply is reported after the command timeout;
 *   its late reply is not taken for the reply of the next command.
 * - P1-19: the wrapper destroyed while connected or connecting deletes the
 *   Director in its own thread (the Director closes the connection then).
 *
 * Runs without a real Director (registered with CTest).
 */

#include <QtTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "director/bareosdirector.h"
#include "director/bdirector.h"
#include "bfakebnetdirector.h"

namespace {

const QString Password = QStringLiteral("console-secret");

/// Waits until @p condition holds or @p ms pass
template <typename Condition>
bool waitFor(Condition condition, int ms = 10000)
{
    QDeadlineTimer deadline(ms);
    while (!condition() && !deadline.hasExpired()) {
        QTest::qWait(10);
    }
    return condition();
}

/// The jsonResult replies to Command::Custom seen by @p spy
QStringList customReplies(const QSignalSpy &spy)
{
    QStringList out;
    for (const QList<QVariant> &args : spy) {
        if (args.at(0).value<BareosDirector::Command>() == BareosDirector::Command::Custom) {
            out << args.at(1).toString();
        }
    }
    return out;
}

} // namespace

class BDirectorSessionTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void jsonReplyCutInsideCharactersArrivesWhole();
    void disconnectSaysQuitBeforeClosing();
    void commandWithoutReplyTimesOut();
    void wrapperDestroyedWhileConnected();
    void wrapperDestroyedWhileConnecting();

private:
    /// A BareosDirector in this thread, logged in to @p fake and Ready
    bool connectReady(BareosDirector &director, BFakeBnetDirector &fake);
};

void BDirectorSessionTest::initTestCase()
{
    qRegisterMetaType<BareosDirector::Command>();
}

bool BDirectorSessionTest::connectReady(BareosDirector &director, BFakeBnetDirector &fake)
{
    director.initialize();
    QSignalSpy ready(&director, &BareosDirector::allResourcesLoaded);
    director.connect(QStringLiteral("127.0.0.1"), fake.port(), QStringLiteral("test-dir"),
                     QStringLiteral("admin"), Password);
    return waitFor([&]() { return ready.count() > 0; })
           && director.connectionState() == BareosDirector::Ready;
}

void BDirectorSessionTest::jsonReplyCutInsideCharactersArrivesWhole()
{
    BFakeBnetDirector fake(Password);
    QVERIFY(fake.listen());

    // Names with two-, three- and four-byte characters and spaces at the
    // ends, repeated so that 7-byte telegrams cut through every one of them
    QJsonArray files;
    const QStringList names = {
        QStringLiteral("/home/jörg/Bücher "),
        QStringLiteral("  two  spaces  "),
        QStringLiteral("price 5 €"),
        QStringLiteral("clef \U0001D11E"),
    };
    for (int i = 0; i < 50; ++i) {
        files.append(QJsonObject{{"name", names.at(i % names.size())}, {"fileid", i}});
    }
    const QByteArray reply = QJsonDocument(QJsonObject{
        {"jsonrpc", "2.0"}, {"id", QJsonValue::Null},
        {"result", QJsonObject{{"files", files}}}}).toJson(QJsonDocument::Indented);
    fake.setReply("list files jobid=1", reply, 7);

    BareosDirector director;
    QVERIFY(connectReady(director, fake));
    QCOMPARE(fake.apiMode, 2);

    QSignalSpy json(&director, &BareosDirector::jsonResult);
    director.doSend(BareosDirector::Command::Custom, QStringLiteral("list files jobid=1"));
    QVERIFY(waitFor([&]() { return !customReplies(json).isEmpty(); }));

    const QStringList replies = customReplies(json);
    QCOMPARE(replies.size(), 1);
    QVERIFY(!replies.first().contains(QChar::ReplacementCharacter));
    const QJsonArray got = QJsonDocument::fromJson(replies.first().toUtf8())
                               .object().value("result").toObject().value("files").toArray();
    QCOMPARE(got.size(), 50);
    for (int i = 0; i < got.size(); ++i) {
        QCOMPARE(got.at(i).toObject().value("name").toString(), names.at(i % names.size()));
    }
    QCOMPARE(director.pendingCommandCount(), 0);
}

void BDirectorSessionTest::disconnectSaysQuitBeforeClosing()
{
    BFakeBnetDirector fake(Password);
    QVERIFY(fake.listen());
    BareosDirector director;
    QVERIFY(connectReady(director, fake));

    QSignalSpy failed(&director, &BareosDirector::commandFailed);
    director.disconnect();
    QCOMPARE(director.connectionState(), BareosDirector::Disconnected);
    QVERIFY(waitFor([&]() { return fake.sessionClosed; }));

    // Before the fix "quit" was queued and written after the socket was
    // closed: the Director never saw it
    QVERIFY(!fake.commands.isEmpty());
    QCOMPARE(fake.commands.last(), QByteArray("quit"));
    QCOMPARE(fake.commands.count("quit"), 1);
    // Nobody waits for the farewell
    QCOMPARE(failed.count(), 0);
    QCOMPARE(director.pendingCommandCount(), 0);
}

void BDirectorSessionTest::commandWithoutReplyTimesOut()
{
    BFakeBnetDirector fake(Password);
    QVERIFY(fake.listen());
    fake.setSilent("status director");
    fake.setReply("version", "{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{\"version\":{\"name\":\"test-dir\"}}}");

    BareosDirector director;
    QVERIFY(connectReady(director, fake));
    director.setCommandTimeout(300);

    QSignalSpy failed(&director, &BareosDirector::commandFailed);
    QSignalSpy json(&director, &BareosDirector::jsonResult);
    director.doSend(BareosDirector::Command::Custom, QStringLiteral("status director"));
    QVERIFY(waitFor([&]() { return failed.count() > 0; }, 5000));

    QCOMPARE(failed.count(), 1);
    QCOMPARE(failed.at(0).at(1).toString(), QStringLiteral("status director"));
    QVERIFY(failed.at(0).at(2).toString().contains(QStringLiteral("did not answer")));
    // Kept, so that its late reply finds it
    QCOMPARE(director.pendingCommandCount(), 1);

    // The Director answers late, then the next command
    fake.sendLateReply("{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{\"late\":true}}");
    director.doSend(BareosDirector::Command::Custom, QStringLiteral("version"));
    QVERIFY(waitFor([&]() { return !customReplies(json).isEmpty(); }));
    QTest::qWait(100);

    const QStringList replies = customReplies(json);
    QCOMPARE(replies.size(), 1);
    QVERIFY2(replies.first().contains(QStringLiteral("test-dir")), qPrintable(replies.first()));
    QVERIFY(!replies.first().contains(QStringLiteral("late")));
    QCOMPARE(director.pendingCommandCount(), 0);
    // Reported once, not again when the session ends
    director.disconnect();
    QCOMPARE(failed.count(), 1);
}

void BDirectorSessionTest::wrapperDestroyedWhileConnected()
{
    BFakeBnetDirector fake(Password);
    QVERIFY(fake.listen());

    auto *wrapper = new BDirector;
    QSignalSpy ready(wrapper, &BDirector::allResourcesLoaded);
    wrapper->connect(QStringLiteral("127.0.0.1"), fake.port(), QStringLiteral("test-dir"),
                     QStringLiteral("admin"), Password);
    QVERIFY(waitFor([&]() { return ready.count() > 0; }));
    QVERIFY(wrapper->isConnected());
    QCOMPARE(wrapper->currentHost(), QStringLiteral("127.0.0.1"));
    QCOMPARE(wrapper->currentDirectorName(), QStringLiteral("test-dir"));

    // Keep the wrapper busy while it goes
    wrapper->doSend(BDirector::Command::Custom, QStringLiteral("status director"));
    delete wrapper;

    // ~BareosDirector ran in its thread and closed the connection. Before
    // the fix it never ran: deleteLater() after quit() had no event loop.
    QVERIFY(waitFor([&]() { return fake.sessionClosed; }, 5000));
}

void BDirectorSessionTest::wrapperDestroyedWhileConnecting()
{
    BFakeBnetDirector fake(Password);
    QVERIFY(fake.listen());

    for (int i = 0; i < 5; ++i) {
        auto *wrapper = new BDirector;
        wrapper->connect(QStringLiteral("127.0.0.1"), fake.port(), QStringLiteral("test-dir"),
                         QStringLiteral("admin"), Password);
        QTest::qWait(i * 5);
        delete wrapper;
    }
    // A wrapper that never connected
    delete new BDirector;
    QTest::qWait(50);
}

QTEST_GUILESS_MAIN(BDirectorSessionTest)
#include "bdirectorsession_test.moc"
