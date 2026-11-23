// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bheadlessrunner_test.cpp
 * @brief Headless mode against a fake Director (P1-29)
 *
 * Before the fix headless mode never sent its command (the call was
 * commented out), waited ten seconds and printed nothing; --script was
 * parsed and ignored.
 *
 * - BHeadlessRunner with a real BDirector in a BNET session with the fake
 *   Director (bfakebnetdirector.h): commands and script lines in order,
 *   the replies on standard output, the exit status, "quit" at the end.
 * - The onesimus binary itself, when CTest passes its path in
 *   ONESIMUS_BINARY (Linux builds;
 *   elsewhere the binary needs its deployment to start): --headless with
 *   -e and with --script, and the exit status.
 *
 * Runs without a real Director (registered with CTest).
 */

#include <QtTest>
#include <QBuffer>
#include <QProcess>
#include <QSignalSpy>
#include <QTemporaryFile>

#include "bheadlessrunner.h"
#include "director/bdirector.h"
#include "bfakebnetdirector.h"

namespace {

const QString Password = QStringLiteral("console-secret");
const QByteArray VersionReply =
    "{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{\"version\":{\"name\":\"test-dir\",\"version\":\"25.1.2\"}}}";
const QByteArray StatusReply =
    "{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{\"status\":\"Bücher \\u20ac\"}}";
const QByteArray ErrorReply =
    "{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":{\"code\":1,\"message\":\"failed\","
    "\"data\":{\"result\":{},\"messages\":{\"error\":[\"foo: is an invalid command.\\n\"]}}}}";

QString binaryPath;

struct Run
{
    int exitCode = -1;
    QString out;
    QString err;
};

Run runRunner(quint16 port, const QStringList &commands, const QString &password = Password)
{
    BDirector director;
    QBuffer out;
    QBuffer err;
    out.open(QIODevice::WriteOnly);
    err.open(QIODevice::WriteOnly);
    BHeadlessRunner runner(&director, commands, &out, &err);
    runner.setConnectTimeout(15000);
    QSignalSpy finished(&runner, &BHeadlessRunner::finished);
    runner.start(QStringLiteral("127.0.0.1"), port, QStringLiteral("test-dir"),
                 QStringLiteral("admin"), password);
    Run run;
    if (finished.wait(30000)) {
        run.exitCode = finished.at(0).at(0).toInt();
    }
    run.out = QString::fromUtf8(out.data());
    run.err = QString::fromUtf8(err.data());
    return run;
}

} // namespace

class BHeadlessRunnerTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void scriptCommands();
    void commandsAreSentInOrderAndPrinted();
    void errorReplyGivesStatusOne();
    void loginOnlyGivesStatusZero();
    void noDirectorGivesStatusTwo();
    void wrongPasswordGivesStatusTwo();
    void binaryExecutesCommand();
    void binaryExecutesScript();
    void binaryWithoutHostIsAUsageError();
};

void BHeadlessRunnerTest::initTestCase()
{
    binaryPath = qEnvironmentVariable("ONESIMUS_BINARY");
}

void BHeadlessRunnerTest::scriptCommands()
{
    const QString script = QStringLiteral("# nightly check\n"
                                          "\n"
                                          "  version  \n"
                                          "status director\r\n"
                                          "   # indented comment\n"
                                          "list jobs last");
    QCOMPARE(BHeadlessRunner::scriptCommands(script),
             QStringList({"version", "status director", "list jobs last"}));
    QVERIFY(BHeadlessRunner::scriptCommands(QString()).isEmpty());
}

void BHeadlessRunnerTest::commandsAreSentInOrderAndPrinted()
{
    BFakeBnetDirector fake(Password);
    QVERIFY(fake.listen());
    fake.setReply("version", VersionReply);
    fake.setReply("status director", StatusReply, 5);   // cut inside "ü" and "€"

    const Run run = runRunner(fake.port(), {"version", "status director"});
    QCOMPARE(run.exitCode, int(BHeadlessRunner::Success));
    QVERIFY2(run.err.isEmpty(), qPrintable(run.err));
    QCOMPARE(run.out, QString::fromUtf8(VersionReply + "\n" + StatusReply + "\n"));

    // The login's resource commands first, then ours in order, then "quit"
    const int version = fake.commands.indexOf("version");
    QVERIFY(version > 0);
    QCOMPARE(fake.commands.mid(version), QList<QByteArray>({"version", "status director", "quit"}));
}

void BHeadlessRunnerTest::errorReplyGivesStatusOne()
{
    BFakeBnetDirector fake(Password);
    QVERIFY(fake.listen());
    fake.setReply("foo", ErrorReply);
    fake.setReply("version", VersionReply);

    // The failure does not stop the commands after it
    const Run run = runRunner(fake.port(), {"foo", "version"});
    QCOMPARE(run.exitCode, int(BHeadlessRunner::CommandFailed));
    QCOMPARE(run.out, QString::fromUtf8(ErrorReply + "\n" + VersionReply + "\n"));
    QVERIFY2(run.err.isEmpty(), qPrintable(run.err));
    QVERIFY(fake.commands.contains("version"));
}

void BHeadlessRunnerTest::loginOnlyGivesStatusZero()
{
    BFakeBnetDirector fake(Password);
    QVERIFY(fake.listen());
    const Run run = runRunner(fake.port(), {});
    QCOMPARE(run.exitCode, int(BHeadlessRunner::Success));
    QVERIFY(run.out.isEmpty());
    QVERIFY(fake.loggedIn);
    QCOMPARE(fake.commands.last(), QByteArray("quit"));
}

void BHeadlessRunnerTest::noDirectorGivesStatusTwo()
{
    // A port nobody listens on
    QTcpServer probe;
    QVERIFY(probe.listen(QHostAddress::LocalHost));
    const quint16 port = probe.serverPort();
    probe.close();

    const Run run = runRunner(port, {"version"});
    QCOMPARE(run.exitCode, int(BHeadlessRunner::NoSession));
    QVERIFY(run.out.isEmpty());
    QVERIFY(!run.err.isEmpty());
}

void BHeadlessRunnerTest::wrongPasswordGivesStatusTwo()
{
    BFakeBnetDirector fake(Password);
    QVERIFY(fake.listen());
    const Run run = runRunner(fake.port(), {"version"}, QStringLiteral("wrong"));
    QCOMPARE(run.exitCode, int(BHeadlessRunner::NoSession));
    QVERIFY(run.out.isEmpty());
    QVERIFY(!run.err.isEmpty());
    QVERIFY(!fake.commands.contains("version"));
}

void BHeadlessRunnerTest::binaryExecutesCommand()
{
    if (binaryPath.isEmpty()) {
        QSKIP("ONESIMUS_BINARY not set (only Linux builds run the binary)");
    }
    BFakeBnetDirector fake(Password);
    QVERIFY(fake.listen());
    fake.setReply("version", VersionReply);

    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(QStringLiteral("ONESIMUS_PASSWORD"), Password);
    env.remove(QStringLiteral("DISPLAY"));            // headless needs no display
    env.remove(QStringLiteral("WAYLAND_DISPLAY"));
    env.remove(QStringLiteral("QT_QPA_PLATFORM"));
    process.setProcessEnvironment(env);
    process.start(binaryPath, {"--headless", "-H", "127.0.0.1", "-p", QString::number(fake.port()),
                               "-C", "admin", "-e", "version"});
    QVERIFY(process.waitForStarted());
    QVERIFY(QTest::qWaitFor([&]() { return process.state() == QProcess::NotRunning; }, 30000));

    QCOMPARE(process.exitStatus(), QProcess::NormalExit);
    const QByteArray err = process.readAllStandardError();
    QVERIFY2(process.exitCode() == 0, err.constData());
    QCOMPARE(process.readAllStandardOutput(), VersionReply + "\n");
    QVERIFY(fake.commands.contains("version"));
    QCOMPARE(fake.commands.last(), QByteArray("quit"));
}

void BHeadlessRunnerTest::binaryExecutesScript()
{
    if (binaryPath.isEmpty()) {
        QSKIP("ONESIMUS_BINARY not set (only Linux builds run the binary)");
    }
    BFakeBnetDirector fake(Password);
    QVERIFY(fake.listen());
    fake.setReply("version", VersionReply);
    fake.setReply("foo", ErrorReply);

    QTemporaryFile script;
    QVERIFY(script.open());
    script.write("# check\nversion\n\nfoo\n");
    script.flush();

    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(QStringLiteral("ONESIMUS_PASSWORD"), Password);
    process.setProcessEnvironment(env);
    process.start(binaryPath, {"--headless", "-H", "127.0.0.1", "-p", QString::number(fake.port()),
                               "-C", "admin", "--script", script.fileName()});
    QVERIFY(process.waitForStarted());
    QVERIFY(QTest::qWaitFor([&]() { return process.state() == QProcess::NotRunning; }, 30000));

    QCOMPARE(process.exitStatus(), QProcess::NormalExit);
    QCOMPARE(process.exitCode(), 1);   // "foo" failed
    QCOMPARE(process.readAllStandardOutput(), VersionReply + "\n" + ErrorReply + "\n");
    const int version = fake.commands.indexOf("version");
    QVERIFY(version > 0);
    QCOMPARE(fake.commands.mid(version), QList<QByteArray>({"version", "foo", "quit"}));
}

void BHeadlessRunnerTest::binaryWithoutHostIsAUsageError()
{
    if (binaryPath.isEmpty()) {
        QSKIP("ONESIMUS_BINARY not set (only Linux builds run the binary)");
    }
    QProcess process;
    process.start(binaryPath, {"--headless", "-e", "version"});
    QVERIFY(process.waitForFinished(30000));
    QCOMPARE(process.exitCode(), 2);
    QVERIFY(process.readAllStandardError().contains("--host"));
}

QTEST_GUILESS_MAIN(BHeadlessRunnerTest)
#include "bheadlessrunner_test.moc"
