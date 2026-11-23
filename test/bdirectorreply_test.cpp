// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bdirectorreply_test.cpp
 * @brief Verdicts on Director replies: "reload" (job and client wizards) and
 *        whether a reply reports a failed command (P1-20)
 *
 * The error replies follow Bareos OutputFormatter::JsonFinalizeResult and the
 * Onesimus Director (crates/odir/src/json_output.rs): a failure is the
 * JSON-RPC "error" object at the top level; text mode (API 0) error lines
 * as Bareos writes them ("<word>: is an invalid command.").
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>

#include "director/bdirectorreply.h"

class BDirectorReplyTest : public QObject
{
    Q_OBJECT

private slots:
    void reload_data();
    void reload();
    void isErrorReply_data();
    void isErrorReply();
    void errorText();
};

void BDirectorReplyTest::reload_data()
{
    QTest::addColumn<QString>("reply");
    QTest::addColumn<bool>("succeeded");

    QTest::newRow("api success")
        << R"({"jsonrpc":"2.0","id":null,"result":{"reload":{"success":true}}})" << true;
    QTest::newRow("api failure in result")
        << R"({"jsonrpc":"2.0","id":null,"result":{"reload":{"success":false}}})" << false;
    QTest::newRow("api failure as error")
        << R"({"jsonrpc":"2.0","id":null,"error":{"code":1,"message":"failed","data":{"result":{"reload":{"success":false}}}}})"
        << false;
    QTest::newRow("api error without details")
        << R"({"jsonrpc":"2.0","id":null,"error":{"code":1,"message":"Command failed"}})" << false;
    QTest::newRow("empty result (gRPC, no output)")
        << R"({"jsonrpc":"2.0","id":null,"result":{}})" << true;
    QTest::newRow("broken json") << R"({"result":{"reload":)" << false;
    QTest::newRow("text success") << "reloaded\n" << true;
    QTest::newRow("text failure") << "failed to reload\n" << false;
    QTest::newRow("text error") << "1999 Error: reload not allowed" << false;
    QTest::newRow("text success wording") << "Reload successful" << true;
    QTest::newRow("empty text") << "" << false;
    QTest::newRow("unrelated text") << "You have messages." << false;
}

void BDirectorReplyTest::reload()
{
    QFETCH(QString, reply);
    QFETCH(bool, succeeded);

    QCOMPARE(BDirectorReply::reloadSucceeded(reply), succeeded);
}

void BDirectorReplyTest::isErrorReply_data()
{
    QTest::addColumn<QString>("reply");
    QTest::addColumn<bool>("error");

    QTest::newRow("api success") << R"({"jsonrpc":"2.0","id":null,"result":{"jobs":[]}})" << false;
    QTest::newRow("api error") << R"({"jsonrpc":"2.0","id":null,"error":{"code":1,"message":"failed","data":{"result":{},"messages":{"error":["JobId 1 is not running.\n"]}}}})"
                               << true;
    QTest::newRow("gRPC failure wrapper") << R"({"jsonrpc":"2.0","id":null,"error":{"code":1,"message":"Permission denied"}})"
                                          << true;
    QTest::newRow("api error, leading whitespace") << "\n  {\"error\":{\"code\":1}}" << true;
    QTest::newRow("api array") << "[1,2]" << false;
    // What the old detection took for failures: the word inside data
    QTest::newRow("job log with error lines")
        << R"({"jsonrpc":"2.0","id":null,"result":{"joblog":[{"logtext":"bareos-sd JobId 5: Error: bsock.c:388 Write error. ERR=Broken pipe\n"}]}})"
        << false;
    QTest::newRow("file named error") << R"({"result":{"files":[{"name":"/var/log/error.log"}]}})" << false;
    QTest::newRow("a column named error") << R"({"result":{"error":"x"}})" << false;
    QTest::newRow("messages error below result") << R"({"result":{"messages":{"error":["x"]}}})" << false;
    QTest::newRow("cut-off json") << R"({"result":{"jobs":[)" << true;
    QTest::newRow("text invalid command") << "foo: is an invalid command.\n" << true;
    QTest::newRow("text error line") << "Using Catalog \"MyCatalog\"\nError: Pool \"x\" not found.\n" << true;
    QTest::newRow("text fatal error line") << "Fatal error: no such job\n" << true;
    QTest::newRow("text auth failure") << "1999 You are not authorized.\n" << true;
    QTest::newRow("text job log line") << " 2026-11-23 10:00:00 bareos-dir JobId 5: Error: Bareos bareos-dir 25.1.2: ERR=No space\n"
                                       << false;
    QTest::newRow("text ERR= in a message") << "Connecting to Client x at y:9102\nFailed: ERR=Connection refused\n" << false;
    QTest::newRow("text plain") << "You have messages.\n" << false;
    QTest::newRow("empty") << "" << false;
}

void BDirectorReplyTest::isErrorReply()
{
    QFETCH(QString, reply);
    QFETCH(bool, error);

    QCOMPARE(BDirectorReply::isErrorReply(reply), error);
}

void BDirectorReplyTest::errorText()
{
    QCOMPARE(BDirectorReply::errorText(QString::fromUtf8(
                 R"({"error":{"code":1,"message":"failed","data":{"messages":{"error":["JobId 1 is not running.\n"]}}}})")),
             QStringLiteral("JobId 1 is not running."));
    QCOMPARE(BDirectorReply::errorText(QStringLiteral(R"({"error":{"code":1,"message":"Permission denied"}})")),
             QStringLiteral("Permission denied"));
    QCOMPARE(BDirectorReply::errorText(QStringLiteral("x\nfoo: is an invalid command.\n")),
             QStringLiteral("foo: is an invalid command."));
    QCOMPARE(BDirectorReply::errorText(QStringLiteral(R"({"result":{}})")), QString());
}

QTEST_APPLESS_MAIN(BDirectorReplyTest)
#include "bdirectorreply_test.moc"
