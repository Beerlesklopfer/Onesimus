// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bdirectorreply_test.cpp
 * @brief Verdict on the reply of a "reload" command (used by the job and client wizards)
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

QTEST_APPLESS_MAIN(BDirectorReplyTest)
#include "bdirectorreply_test.moc"
