/**
 * @file brunjobcommand_test.cpp
 * @brief Command line built by the "Run Job" dialog
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>

#include "jobs/brunjobcommand.h"

using BRunJobCommand::Error;
using BRunJobCommand::Params;
using BRunJobCommand::Result;

class BRunJobCommandTest : public QObject
{
    Q_OBJECT

private slots:
    void jobOnly();
    void allInputsAppearInTheCommand();
    void namesWithSpacesStayOneArgument();
    void valueCannotAddKeywords();
    void quoteEscapesQuoteAndBackslash();
    void priorityOnlyWhenGiven();
    void levelIsQuotedWhenNotAPlainWord();
    void emptyValuesAreLeftOut();
    void whenMustBeACompleteTime_data();
    void whenMustBeACompleteTime();
    void whenAcceptsTypedQuotes();
    void missingJobIsAnError();
    void controlCharactersAreRejected();
};

void BRunJobCommandTest::jobOnly()
{
    Params params;
    params.job = "backup-pdc";
    const Result result = BRunJobCommand::build(params);
    QVERIFY(result.ok());
    QCOMPARE(result.command, QString("run job=\"backup-pdc\" yes"));
}

void BRunJobCommandTest::allInputsAppearInTheCommand()
{
    Params params;
    params.job = "backup-pdc";
    params.client = "pdc-fd";
    params.fileset = "LinuxAll";
    params.pool = "Full";
    params.storage = "File";
    params.level = "Full";
    params.priority = 5;
    params.when = "2026-11-18 23:00:00";
    params.bootstrap = "/var/lib/bareos/pdc.bsr";

    const Result result = BRunJobCommand::build(params);
    QVERIFY(result.ok());
    QCOMPARE(result.command,
             QString("run job=\"backup-pdc\" client=\"pdc-fd\" fileset=\"LinuxAll\" pool=\"Full\" "
                     "storage=\"File\" level=Full priority=5 when=\"2026-11-18 23:00:00\" "
                     "bootstrap=\"/var/lib/bareos/pdc.bsr\" yes"));
}

void BRunJobCommandTest::namesWithSpacesStayOneArgument()
{
    Params params;
    params.job = "Backup File Server";
    params.client = "file server-fd";
    params.fileset = "Windows All Drives";
    params.pool = "Full Pool";
    params.storage = "Tape Library 1";
    params.level = "Incremental";
    params.when = "2026-11-18 23:00:00";
    params.bootstrap = "C:\\Program Files\\Bareos\\my job.bsr";

    const Result result = BRunJobCommand::build(params);
    QVERIFY(result.ok());
    QCOMPARE(result.command,
             QString("run job=\"Backup File Server\" client=\"file server-fd\" "
                     "fileset=\"Windows All Drives\" pool=\"Full Pool\" storage=\"Tape Library 1\" "
                     "level=Incremental when=\"2026-11-18 23:00:00\" "
                     "bootstrap=\"C:\\\\Program Files\\\\Bareos\\\\my job.bsr\" yes"));

    // Outside of quoted values the command has exactly these keywords
    QString unquoted = result.command;
    unquoted.replace(QRegularExpression("\"(?:[^\"\\\\]|\\\\.)*\""), "V");
    QCOMPARE(unquoted, QString("run job=V client=V fileset=V pool=V storage=V "
                               "level=Incremental when=V bootstrap=V yes"));
}

void BRunJobCommandTest::valueCannotAddKeywords()
{
    Params params;
    params.job = "a\" level=Full yes";
    params.pool = "x yes";
    const Result result = BRunJobCommand::build(params);
    QVERIFY(result.ok());
    QCOMPARE(result.command, QString("run job=\"a\\\" level=Full yes\" pool=\"x yes\" yes"));

    QString unquoted = result.command;
    unquoted.replace(QRegularExpression("\"(?:[^\"\\\\]|\\\\.)*\""), "V");
    QCOMPARE(unquoted, QString("run job=V pool=V yes"));
}

void BRunJobCommandTest::quoteEscapesQuoteAndBackslash()
{
    bool ok = false;
    QCOMPARE(BRunJobCommand::quote("plain", &ok), QString("\"plain\""));
    QVERIFY(ok);
    QCOMPARE(BRunJobCommand::quote("two words", &ok), QString("\"two words\""));
    QVERIFY(ok);
    QCOMPARE(BRunJobCommand::quote("a\"b", &ok), QString("\"a\\\"b\""));
    QVERIFY(ok);
    QCOMPARE(BRunJobCommand::quote("a\\b", &ok), QString("\"a\\\\b\""));
    QVERIFY(ok);
    QCOMPARE(BRunJobCommand::quote("", &ok), QString("\"\""));
    QVERIFY(ok);
    BRunJobCommand::quote("line\nbreak", &ok);
    QVERIFY(!ok);
}

void BRunJobCommandTest::priorityOnlyWhenGiven()
{
    Params params;
    params.job = "j";
    QVERIFY(!BRunJobCommand::build(params).command.contains("priority"));

    // 10 is a value like any other once it is given
    params.priority = 10;
    QCOMPARE(BRunJobCommand::build(params).command, QString("run job=\"j\" priority=10 yes"));

    params.priority = -1;
    const Result result = BRunJobCommand::build(params);
    QCOMPARE(result.error, Error::InvalidPriority);
    QVERIFY(result.command.isEmpty());
}

void BRunJobCommandTest::levelIsQuotedWhenNotAPlainWord()
{
    Params params;
    params.job = "j";
    params.level = "VirtualFull";
    QCOMPARE(BRunJobCommand::build(params).command, QString("run job=\"j\" level=VirtualFull yes"));

    params.level = "Full yes";
    QCOMPARE(BRunJobCommand::build(params).command, QString("run job=\"j\" level=\"Full yes\" yes"));
}

void BRunJobCommandTest::emptyValuesAreLeftOut()
{
    Params params;
    params.job = "  j  ";
    params.client = "   ";
    params.level = "";
    params.when = "  ";
    params.bootstrap = "";
    QCOMPARE(BRunJobCommand::build(params).command, QString("run job=\"j\" yes"));
}

void BRunJobCommandTest::whenMustBeACompleteTime_data()
{
    QTest::addColumn<QString>("when");
    QTest::addColumn<bool>("valid");

    QTest::newRow("complete") << "2026-11-18 23:00:00" << true;
    QTest::newRow("date only") << "2026-11-18" << false;
    QTest::newRow("no seconds") << "2026-11-18 23:00" << false;
    QTest::newRow("words") << "tonight" << false;
    QTest::newRow("month 13") << "2026-13-18 23:00:00" << false;
    QTest::newRow("hour 25") << "2026-11-18 25:00:00" << false;
    QTest::newRow("trailing keyword") << "2026-11-18 23:00:00 yes" << false;
}

void BRunJobCommandTest::whenMustBeACompleteTime()
{
    QFETCH(QString, when);
    QFETCH(bool, valid);

    Params params;
    params.job = "j";
    params.when = when;
    const Result result = BRunJobCommand::build(params);

    QCOMPARE(result.ok(), valid);
    if (valid) {
        QCOMPARE(result.command, QString("run job=\"j\" when=\"%1\" yes").arg(when));
    } else {
        // An unreadable time must not turn into "start now"
        QCOMPARE(result.error, Error::InvalidWhen);
        QCOMPARE(result.field, QString("when"));
        QVERIFY(result.command.isEmpty());
    }
}

void BRunJobCommandTest::whenAcceptsTypedQuotes()
{
    Params params;
    params.job = "j";
    params.when = " \"2026-11-18 23:00:00\" ";
    QCOMPARE(BRunJobCommand::build(params).command,
             QString("run job=\"j\" when=\"2026-11-18 23:00:00\" yes"));
}

void BRunJobCommandTest::missingJobIsAnError()
{
    Params params;
    params.client = "pdc-fd";
    const Result result = BRunJobCommand::build(params);
    QCOMPARE(result.error, Error::MissingJob);
    QVERIFY(result.command.isEmpty());
}

void BRunJobCommandTest::controlCharactersAreRejected()
{
    Params params;
    params.job = "j";
    params.pool = "Full\nrun job=other yes";
    const Result result = BRunJobCommand::build(params);
    QCOMPARE(result.error, Error::ControlCharacter);
    QCOMPARE(result.field, QString("pool"));
    QVERIFY(result.command.isEmpty());
}

QTEST_APPLESS_MAIN(BRunJobCommandTest)
#include "brunjobcommand_test.moc"
