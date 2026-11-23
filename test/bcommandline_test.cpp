// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bcommandline_test.cpp
 * @brief Command lines built from values: every value quoted or refused
 *        (P1-24)
 *
 * BareosDirector::commandToString() for the commands whose argument is a
 * resource name, a number or an argument string built by the caller;
 * doSend() refusing what cannot be written; BBvfsArgs for the .bvfs
 * commands, whose values come from the Director.
 *
 * Runs without a Director and without network traffic (registered with CTest).
 */

#include <QtTest>
#include <QSignalSpy>

#include "bquote.h"
#include "director/bareosdirector.h"
#include "jobs/bbvfsargs.h"

using Command = BareosDirector::Command;

class BCommandLineTest : public QObject
{
    Q_OBJECT

private slots:
    void namesAreQuoted_data();
    void namesAreQuoted();
    void numbersOnly_data();
    void numbersOnly();
    void builtArgumentsMustBeWellFormed_data();
    void builtArgumentsMustBeWellFormed();
    void doSendRefusesWhatCannotBeWritten_data();
    void doSendRefusesWhatCannotBeWritten();
    void bvfsIdsAreNumbers();
    void bvfsRestoreTable();
    void bvfsListing();
    void bvfsRestore();
};

void BCommandLineTest::namesAreQuoted_data()
{
    QTest::addColumn<int>("cmd");
    QTest::addColumn<QString>("args");
    QTest::addColumn<QString>("line");
    QTest::newRow("status client blank")
        << int(Command::StatusClient) << "my fd" << "status client=\"my fd\"";
    QTest::newRow("run job keyword inside")
        << int(Command::Run) << "x yes" << "run job=\"x yes\"";
    QTest::newRow("defaults quote")
        << int(Command::DotDefaults) << "a\"b" << ".defaults job=\"a\\\"b\"";
    QTest::newRow("defaults backslash")
        << int(Command::DotDefaults) << "a\\" << ".defaults job=\"a\\\\\"";
    QTest::newRow("show client")
        << int(Command::ShowClient) << "c 1" << "show client=\"c 1\"";
}

void BCommandLineTest::namesAreQuoted()
{
    QFETCH(int, cmd);
    QFETCH(QString, args);
    QFETCH(QString, line);
    QCOMPARE(BareosDirector::commandToString(Command(cmd), args), line);
}

void BCommandLineTest::numbersOnly_data()
{
    QTest::addColumn<int>("cmd");
    QTest::addColumn<QString>("args");
    QTest::addColumn<QString>("line");   // empty: refused
    QTest::newRow("cancel") << int(Command::Cancel) << "42" << "cancel jobid=42";
    QTest::newRow("cancel injected") << int(Command::Cancel) << "42 yes" << "";
    QTest::newRow("cancel empty") << int(Command::Cancel) << "" << "";
    QTest::newRow("delete") << int(Command::DeleteJob) << "7" << "delete job jobid=7 yes";
    QTest::newRow("delete list") << int(Command::DeleteJob) << "7,8" << "";
    QTest::newRow("joblog") << int(Command::ListJobId) << "9" << "list joblog jobid=9";
    QTest::newRow("joblog name") << int(Command::ListJobId) << "x" << "";
    QTest::newRow("files") << int(Command::ListFiles) << "9" << "list files jobid=9";
    QTest::newRow("files injected") << int(Command::ListFiles) << "9 client=x" << "";
    QTest::newRow("rerun") << int(Command::Rerun) << "5" << "rerun jobid=5";
    QTest::newRow("rerun yes") << int(Command::Rerun) << "5 yes" << "rerun jobid=5 yes";
    QTest::newRow("rerun other") << int(Command::Rerun) << "5 level=Full" << "";
    QTest::newRow("list jobs") << int(Command::ListJobs) << "100" << "list jobs limit=100";
    QTest::newRow("list jobs page") << int(Command::ListJobs) << "100,200" << "list jobs limit=100 offset=200";
    QTest::newRow("list jobs default") << int(Command::ListJobs) << "" << "list jobs limit=100";
    QTest::newRow("list jobs bad") << int(Command::ListJobs) << "100,x" << "";
    QTest::newRow("last") << int(Command::ListJobsLast) << "" << "list jobs last=100";
    QTest::newRow("last bad") << int(Command::ListJobsLast) << "-1" << "";
    QTest::newRow("api") << int(Command::ApiMode) << "2" << ".api 2";
    QTest::newRow("api word") << int(Command::ApiMode) << "json" << ".api json";
    QTest::newRow("api injected") << int(Command::ApiMode) << "json compact=yes" << "";
    QTest::newRow("trace") << int(Command::Trace) << "off" << "trace off";
    QTest::newRow("trace bad") << int(Command::Trace) << "on x" << "";
}

void BCommandLineTest::numbersOnly()
{
    QFETCH(int, cmd);
    QFETCH(QString, args);
    QFETCH(QString, line);
    QCOMPARE(BareosDirector::commandToString(Command(cmd), args), line);
}

void BCommandLineTest::builtArgumentsMustBeWellFormed_data()
{
    QTest::addColumn<int>("cmd");
    QTest::addColumn<QString>("args");
    QTest::addColumn<bool>("accepted");
    QTest::newRow("show directors") << int(Command::Show) << "directors" << true;
    QTest::newRow("show quoted") << int(Command::Show) << "client=" + BQuote::quoted("a \"b\"") << true;
    QTest::newRow("show raw quote") << int(Command::Show) << "client=a\"b" << false;
    QTest::newRow("restore built")
        << int(Command::Restore)
        << "file=?b2123 client=\"fd 1\" restoreclient=\"fd 1\" where=\"/tmp/a b\" replace=never yes" << true;
    QTest::newRow("restore unclosed")
        << int(Command::Restore) << "file=?b2123 where=\"/tmp yes" << false;
    QTest::newRow("restore backslash bare")
        << int(Command::Restore) << "where=C:\\x" << false;
    QTest::newRow("restore text after quote")
        << int(Command::Restore) << "where=\"/a\"b" << false;
    QTest::newRow("label quoted")
        << int(Command::Label) << "volume=" + BQuote::quoted("V 1") + " pool=" + BQuote::quoted("P") << true;
    QTest::newRow("label raw backslash in value")
        << int(Command::Label) << "volume=V\\1" << false;
    QTest::newRow("configure built")
        << int(Command::Configure) << "add client name=\"c\" address=\"h\" password=\"[md5]x\" fdport=9102" << true;
    QTest::newRow("configure newline")
        << int(Command::Configure) << "add client name=\"c\nreload\"" << false;
    QTest::newRow("bvfs restore")
        << int(Command::BvfsRestore) << "path=b2123 jobid=1,2 fileid=3" << true;
    QTest::newRow("bvfs root") << int(Command::BvfsLsDirs) << "jobid=1 path= limit=1000 offset=0" << true;
    QTest::newRow("estimate quote mid") << int(Command::Estimate) << "job=a\"b c\"" << false;
}

void BCommandLineTest::builtArgumentsMustBeWellFormed()
{
    QFETCH(int, cmd);
    QFETCH(QString, args);
    QFETCH(bool, accepted);
    const QString line = BareosDirector::commandToString(Command(cmd), args);
    QCOMPARE(!line.isEmpty(), accepted);
    if (accepted)
        QVERIFY(line.endsWith(args));
}

void BCommandLineTest::doSendRefusesWhatCannotBeWritten_data()
{
    QTest::addColumn<int>("cmd");
    QTest::addColumn<QString>("args");
    QTest::newRow("name with newline") << int(Command::StatusClient) << "fd\nstatus dir";
    QTest::newRow("custom with newline") << int(Command::Custom) << "list jobs\nquit";
    QTest::newRow("cancel injected") << int(Command::Cancel) << "1 yes";
    QTest::newRow("show raw quote") << int(Command::Show) << "client=\"x";
    QTest::newRow("empty custom") << int(Command::Custom) << "";
}

void BCommandLineTest::doSendRefusesWhatCannotBeWritten()
{
    QFETCH(int, cmd);
    QFETCH(QString, args);

    BareosDirector director;
    director.initialize();
    QSignalSpy failed(&director, &BareosDirector::commandFailed);

    director.doSend(Command(cmd), args);

    QCOMPARE(director.pendingCommandCount(), 0);
    QCOMPARE(failed.count(), 1);
    QCOMPARE(failed.at(0).at(0).value<Command>(), Command(cmd));
    QCOMPARE(failed.at(0).at(1).toString(), args);
    // Refused for its value, not for the missing connection
    QVERIFY(failed.at(0).at(2).toString().contains(QLatin1String("cannot be written")));
}

void BCommandLineTest::bvfsIdsAreNumbers()
{
    QVERIFY(BBvfsArgs::isIdList("1"));
    QVERIFY(BBvfsArgs::isIdList("1,22,333"));
    QVERIFY(!BBvfsArgs::isIdList(""));
    QVERIFY(!BBvfsArgs::isIdList("1, 2"));
    QVERIFY(!BBvfsArgs::isIdList("1,,2"));
    QVERIFY(!BBvfsArgs::isIdList("1,"));
    QVERIFY(!BBvfsArgs::isIdList("-1"));
    QVERIFY(!BBvfsArgs::isIdList("1 path=/etc"));
    QVERIFY(!BBvfsArgs::isIdList(QString::fromUtf8("\u0661")));   // Arabic-Indic digit one
    QCOMPARE(BBvfsArgs::idList({"4", "5"}), QStringLiteral("4,5"));
    QCOMPARE(BBvfsArgs::idList({"4", "5 dirid=1"}), QString());
    QCOMPARE(BBvfsArgs::jobIds("3,4"), QStringLiteral("jobid=3,4"));
    QCOMPARE(BBvfsArgs::jobIds("3 limit=1"), QString());
}

void BCommandLineTest::bvfsRestoreTable()
{
    // CheckTempBvfsTableName() (core/src/cats/bvfs.cc): "b2" and digits
    QVERIFY(BBvfsArgs::isRestoreTable("b2123456"));
    QVERIFY(!BBvfsArgs::isRestoreTable("b2"));
    QVERIFY(!BBvfsArgs::isRestoreTable("b2x"));
    QVERIFY(!BBvfsArgs::isRestoreTable("File"));
    QVERIFY(!BBvfsArgs::isRestoreTable("b21 jobid=1"));
    QCOMPARE(BBvfsArgs::cleanup("b2123"), QStringLiteral("path=b2123"));
    QCOMPARE(BBvfsArgs::cleanup("Job; drop"), QString());
}

void BCommandLineTest::bvfsListing()
{
    QCOMPARE(BBvfsArgs::listing("1,2", 0, 0, 1000), QStringLiteral("jobid=1,2 path= limit=1000 offset=0"));
    QCOMPARE(BBvfsArgs::listing("7", 42, 2000, 500), QStringLiteral("jobid=7 pathid=42 limit=500 offset=2000"));
    QCOMPARE(BBvfsArgs::listing("7 path=/", 42, 0, 500), QString());
    QCOMPARE(BBvfsArgs::listing("7", 42, -1, 500), QString());
    QCOMPARE(BBvfsArgs::listing("7", 42, 0, 0), QString());
}

void BCommandLineTest::bvfsRestore()
{
    QCOMPARE(BBvfsArgs::restore("b2100", "1,2", {"10", "11"}, {"5"}),
             QStringLiteral("path=b2100 jobid=1,2 fileid=10,11 dirid=5"));
    QCOMPARE(BBvfsArgs::restore("b2100", "1", {}, {"5"}), QStringLiteral("path=b2100 jobid=1 dirid=5"));
    QCOMPARE(BBvfsArgs::restore("b2100", "1", {"10"}, {}), QStringLiteral("path=b2100 jobid=1 fileid=10"));
    // Nothing selected, a file id from the Director that is not a number,
    // a bad table or bad job ids: nothing to send
    QCOMPARE(BBvfsArgs::restore("b2100", "1", {}, {}), QString());
    QCOMPARE(BBvfsArgs::restore("b2100", "1", {"10 dirid=1"}, {}), QString());
    QCOMPARE(BBvfsArgs::restore("tbl", "1", {"10"}, {}), QString());
    QCOMPARE(BBvfsArgs::restore("b2100", "1;2", {"10"}, {}), QString());
}

QTEST_GUILESS_MAIN(BCommandLineTest)
#include "bcommandline_test.moc"
