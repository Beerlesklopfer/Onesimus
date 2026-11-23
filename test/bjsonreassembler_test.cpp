// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bjsonreassembler_test.cpp
 * @brief A JSON reply reassembled from BNET telegrams (P1-17)
 *
 * A Director cuts a long reply into telegrams at its packet size, which can
 * fall inside a UTF-8 character or between two spaces of a file name
 * (onesimus-sim src/server/bnet.ts FrameStream.send() cuts a Buffer at
 * MAX_MESSAGE_LEN bytes; Bareos bsock does the same with its buffer). Before
 * the fix every telegram was decoded and trimmed on its own, and the growing
 * text was parsed again after every telegram.
 *
 * Runs without a Director and without network traffic (registered with CTest).
 */

#include <QtTest>
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "director/bjsonreassembler.h"

namespace {

/// A reply with everything a cut can damage: two- three- and four-byte
/// characters, spaces at the ends of string values, braces, brackets and
/// escaped quotes inside strings
QByteArray sampleReply()
{
    return QStringLiteral(
               "{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{\"files\":["
               "{\"name\":\"/home/müller/Bücher \",\"size\":1},"
               "{\"name\":\"  two  spaces  \",\"size\":2},"
               "{\"name\":\"price 5 € {not} [an] \\\"array\\\"\",\"size\":3},"
               "{\"name\":\"clef \U0001D11E and \\\\ backslash\",\"size\":4}]}}")
        .toUtf8();
}

QString fileName(const QByteArray &document, int index)
{
    const QJsonObject result = QJsonDocument::fromJson(document).object().value("result").toObject();
    return result.value("files").toArray().at(index).toObject().value("name").toString();
}

} // namespace

class BJsonReassemblerTest : public QObject
{
    Q_OBJECT

private slots:
    void wholeReplyInOneTelegram();
    void splitAtEveryBytePosition();
    void splitIntoThreeAtEveryPair();
    void oneBytePerTelegram();
    void whitespaceAroundTheDocumentIsDropped();
    void twoDocumentsInOneTelegram();
    void textIsNotTakenForJson();
    void incompleteReplyIsTakenWhenTheReplyEnds();
    void largeReplyIsScannedOnce();
};

void BJsonReassemblerTest::wholeReplyInOneTelegram()
{
    BJsonReassembler r;
    QVERIFY(BJsonReassembler::startsJson(sampleReply()));
    r.append(sampleReply());
    QByteArray doc;
    QVERIFY(r.takeDocument(&doc));
    QCOMPARE(doc, sampleReply());
    QVERIFY(r.isEmpty());
    QVERIFY(!r.takeDocument(&doc));
}

void BJsonReassemblerTest::splitAtEveryBytePosition()
{
    const QByteArray reply = sampleReply();
    const QString expected = QString::fromUtf8(reply);

    for (int cut = 1; cut < reply.size(); ++cut) {
        BJsonReassembler r;
        QByteArray doc;
        r.append(reply.left(cut));
        QVERIFY2(!r.takeDocument(&doc), qPrintable(QString("complete too early at %1").arg(cut)));
        QVERIFY(!r.isEmpty());
        r.append(reply.mid(cut));
        QVERIFY2(r.takeDocument(&doc), qPrintable(QString("not complete, cut at %1").arg(cut)));

        // Byte for byte the reply, decoded once: no U+FFFD, no lost space
        QCOMPARE(doc, reply);
        const QString text = QString::fromUtf8(doc);
        QVERIFY(!text.contains(QChar::ReplacementCharacter));
        QCOMPARE(text, expected);
        QCOMPARE(fileName(doc, 0), QStringLiteral("/home/müller/Bücher "));
        QCOMPARE(fileName(doc, 1), QStringLiteral("  two  spaces  "));
        QVERIFY(r.isEmpty());
    }
}

void BJsonReassemblerTest::splitIntoThreeAtEveryPair()
{
    const QByteArray reply = sampleReply();
    for (int a = 1; a < reply.size() - 1; ++a) {
        for (int b = a + 1; b < reply.size(); ++b) {
            BJsonReassembler r;
            QByteArray doc;
            r.append(reply.left(a));
            QVERIFY(!r.takeDocument(&doc));
            r.append(reply.mid(a, b - a));
            QVERIFY(!r.takeDocument(&doc));
            r.append(reply.mid(b));
            QVERIFY(r.takeDocument(&doc));
            if (doc != reply) {
                QFAIL(qPrintable(QString("cuts at %1 and %2 changed the reply").arg(a).arg(b)));
            }
        }
    }
}

void BJsonReassemblerTest::oneBytePerTelegram()
{
    const QByteArray reply = sampleReply();
    BJsonReassembler r;
    QByteArray doc;
    for (int i = 0; i < reply.size(); ++i) {
        r.append(reply.mid(i, 1));
        const bool complete = r.takeDocument(&doc);
        QCOMPARE(complete, i == reply.size() - 1);
    }
    QCOMPARE(doc, reply);
    QCOMPARE(fileName(doc, 3), QStringLiteral("clef \U0001D11E and \\ backslash"));
}

void BJsonReassemblerTest::whitespaceAroundTheDocumentIsDropped()
{
    BJsonReassembler r;
    QByteArray doc;
    QVERIFY(BJsonReassembler::startsJson("\n  [1"));
    r.append("\n  [1, ");
    QVERIFY(!r.takeDocument(&doc));
    r.append(" 2]\n\n");
    QVERIFY(r.takeDocument(&doc));
    QCOMPARE(doc, QByteArray("[1,  2]"));   // the spaces inside stay
    QVERIFY(!r.takeDocument(&doc));
    QVERIFY(r.isEmpty());
}

void BJsonReassemblerTest::twoDocumentsInOneTelegram()
{
    BJsonReassembler r;
    QByteArray doc;
    r.append("{\"a\":1}\n{\"b\":\"}\"}{\"c\":");
    QVERIFY(r.takeDocument(&doc));
    QCOMPARE(doc, QByteArray("{\"a\":1}"));
    QVERIFY(r.takeDocument(&doc));
    QCOMPARE(doc, QByteArray("{\"b\":\"}\"}"));
    QVERIFY(!r.takeDocument(&doc));
    QVERIFY(!r.isEmpty());
    r.append("3}");
    QVERIFY(r.takeDocument(&doc));
    QCOMPARE(doc, QByteArray("{\"c\":3}"));
}

void BJsonReassemblerTest::textIsNotTakenForJson()
{
    QVERIFY(!BJsonReassembler::startsJson("1000 OK"));
    QVERIFY(!BJsonReassembler::startsJson("   "));
    QVERIFY(!BJsonReassembler::startsJson(""));

    BJsonReassembler r;
    QByteArray doc;
    r.append("You have messages.\n");
    QVERIFY(!r.takeDocument(&doc));
    QCOMPARE(r.takeAll(), QByteArray("You have messages."));
    QVERIFY(r.isEmpty());
}

void BJsonReassemblerTest::incompleteReplyIsTakenWhenTheReplyEnds()
{
    // The Director ended the reply (a BNET signal) before the document was
    // complete: what came belongs to that reply and is handed on, decoded once
    BJsonReassembler r;
    QByteArray doc;
    r.append(QStringLiteral("{\"error\":\"Bücher").toUtf8().left(15));
    r.append(QStringLiteral("{\"error\":\"Bücher").toUtf8().mid(15));
    QVERIFY(!r.takeDocument(&doc));
    QCOMPARE(QString::fromUtf8(r.takeAll()), QStringLiteral("{\"error\":\"Bücher"));
    QVERIFY(r.isEmpty());
}

void BJsonReassemblerTest::largeReplyIsScannedOnce()
{
    // About 8 MiB in 1000-byte telegrams. The old code parsed the whole
    // text again after each of the ~8400 telegrams (tens of GB of parsing).
    QByteArray reply = "{\"result\":{\"files\":[";
    const QByteArray row = QStringLiteral("{\"name\":\"/data/Ärger € \U0001D11E/file\",\"size\":12345},").toUtf8();
    while (reply.size() < 8 * 1024 * 1024) {
        reply += row;
    }
    reply.chop(1);
    reply += "]}}";

    BJsonReassembler r;
    QByteArray doc;
    QElapsedTimer timer;
    timer.start();
    int telegrams = 0;
    bool complete = false;
    for (qsizetype off = 0; off < reply.size(); off += 1000) {
        r.append(reply.mid(off, 1000));
        ++telegrams;
        complete = r.takeDocument(&doc);
        if (!complete) {
            QVERIFY(off + 1000 < reply.size());
        }
    }
    const qint64 elapsed = timer.elapsed();

    QVERIFY(complete);
    QCOMPARE(doc.size(), reply.size());
    QVERIFY(doc == reply);
    QVERIFY(telegrams > 8000);
    // Linear: every byte was looked at exactly once
    QCOMPARE(r.bytesScanned(), qint64(reply.size()));
    // And it is quick even in a Debug build on a busy CI runner
    QVERIFY2(elapsed < 10000, qPrintable(QString("took %1 ms").arg(elapsed)));
    QVERIFY(QJsonDocument::fromJson(doc).isObject());
}

QTEST_GUILESS_MAIN(BJsonReassemblerTest)
#include "bjsonreassembler_test.moc"
