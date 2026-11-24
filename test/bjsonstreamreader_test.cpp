// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bjsonstreamreader_test.cpp
 * @brief BJsonStreamReader: buffered parsing, the jobs array in the shapes
 *        a Director sends it, malformed input, and the live device mode
 *
 * The Jobs tab parses "llist jobs" replies through this class
 * (BJobWidget), so a reply that is cut, empty or of an unexpected shape
 * must give an error or an empty array, never a crash or stale jobs.
 *
 * Runs without a Director and without network traffic (registered with CTest).
 */

#include <QtTest>
#include <QBuffer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>

#include "director/bjsonstreamreader.h"

class BJsonStreamReaderTest : public QObject
{
    Q_OBJECT

private slots:
    void parsesChunksAsOneDocument();
    void parsesMultiByteCharacterCutBetweenChunks();
    void jobsArrayFromDirectArray();
    void jobsArrayFromResultJobs();
    void jobsArrayFromRootJobs();
    void resultJobsWinsOverRootJobs();
    void jobsArrayEmptyForOtherShapes_data();
    void jobsArrayEmptyForOtherShapes();
    void malformedInputReportsError_data();
    void malformedInputReportsError();
    void failedParseDropsTheOldDocument();
    void clearForgetsBufferAndDocument();
    void setDocumentReplacesParsedDocument();
    void liveModeReadsAvailableDataAtOnce();
    void liveModeWaitsForTheCompleteDocument();
    void disconnectStopsReading();
};

namespace {

QByteArray jobsReply()
{
    return QByteArrayLiteral(
        "{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{\"jobs\":["
        "{\"jobid\":\"1\",\"name\":\"BackupCatalog\"},"
        "{\"jobid\":\"2\",\"name\":\"Backup Client1\"}]}}");
}

} // namespace

void BJsonStreamReaderTest::parsesChunksAsOneDocument()
{
    BJsonStreamReader reader;
    QSignalSpy parsed(&reader, &BJsonStreamReader::jsonParsed);
    QSignalSpy errors(&reader, &BJsonStreamReader::parseError);

    const QByteArray reply = jobsReply();
    // Cut at every 7th byte: inside keys, values and between braces
    for (int i = 0; i < reply.size(); i += 7)
        reader.receiveData(reply.mid(i, 7));

    QVERIFY(reader.parseJson());
    QCOMPARE(parsed.count(), 1);
    QCOMPARE(parsed.at(0).at(0).toBool(), true);
    QCOMPARE(errors.count(), 0);
    QVERIFY(reader.jsonDocument().isObject());
    QCOMPARE(reader.jobsArray().size(), 2);
    QCOMPARE(reader.jobsArray().at(1).toObject().value("name").toString(),
             QStringLiteral("Backup Client1"));
}

void BJsonStreamReaderTest::parsesMultiByteCharacterCutBetweenChunks()
{
    // The buffer holds bytes, so a UTF-8 character cut by the transport
    // is joined again before decoding
    const QByteArray reply =
        QStringLiteral("[{\"name\":\"Bücher € \U0001D11E\"}]").toUtf8();
    const int cut = reply.indexOf("\xe2\x82\xac") + 1;   // inside the euro sign

    BJsonStreamReader reader;
    reader.receiveData(reply.left(cut));
    reader.receiveData(reply.mid(cut));
    QVERIFY(reader.parseJson());
    QCOMPARE(reader.jobsArray().at(0).toObject().value("name").toString(),
             QStringLiteral("Bücher € \U0001D11E"));
}

void BJsonStreamReaderTest::jobsArrayFromDirectArray()
{
    BJsonStreamReader reader;
    reader.receiveData("[{\"jobid\":\"7\"},{\"jobid\":\"8\"},{\"jobid\":\"9\"}]");
    QVERIFY(reader.parseJson());
    QCOMPARE(reader.jobsArray().size(), 3);

    // An empty array is a valid reply without jobs
    reader.clear();
    reader.receiveData("[]");
    QVERIFY(reader.parseJson());
    QVERIFY(reader.jobsArray().isEmpty());
}

void BJsonStreamReaderTest::jobsArrayFromResultJobs()
{
    BJsonStreamReader reader;
    reader.receiveData(jobsReply());
    QVERIFY(reader.parseJson());
    QCOMPARE(reader.jobsArray().size(), 2);
    QCOMPARE(reader.jobsArray().at(0).toObject().value("jobid").toString(), QStringLiteral("1"));
}

void BJsonStreamReaderTest::jobsArrayFromRootJobs()
{
    BJsonStreamReader reader;
    reader.receiveData("{\"jobs\":[{\"jobid\":\"3\"}]}");
    QVERIFY(reader.parseJson());
    QCOMPARE(reader.jobsArray().size(), 1);

    // result without jobs falls back to the jobs of the root
    reader.clear();
    reader.receiveData("{\"result\":{\"volumes\":[]},\"jobs\":[{\"jobid\":\"4\"}]}");
    QVERIFY(reader.parseJson());
    QCOMPARE(reader.jobsArray().size(), 1);
    QCOMPARE(reader.jobsArray().at(0).toObject().value("jobid").toString(), QStringLiteral("4"));
}

void BJsonStreamReaderTest::resultJobsWinsOverRootJobs()
{
    BJsonStreamReader reader;
    reader.receiveData("{\"jobs\":[{\"jobid\":\"root\"}],"
                       "\"result\":{\"jobs\":[{\"jobid\":\"nested\"}]}}");
    QVERIFY(reader.parseJson());
    QCOMPARE(reader.jobsArray().size(), 1);
    QCOMPARE(reader.jobsArray().at(0).toObject().value("jobid").toString(),
             QStringLiteral("nested"));
}

void BJsonStreamReaderTest::jobsArrayEmptyForOtherShapes_data()
{
    QTest::addColumn<QByteArray>("json");
    QTest::newRow("empty object") << QByteArray("{}");
    QTest::newRow("result is an array") << QByteArray("{\"result\":[{\"jobid\":\"1\"}]}");
    QTest::newRow("result is a string") << QByteArray("{\"result\":\"jobs\"}");
    QTest::newRow("result.jobs is an object") << QByteArray("{\"result\":{\"jobs\":{\"jobid\":\"1\"}}}");
    QTest::newRow("result.jobs is null") << QByteArray("{\"result\":{\"jobs\":null}}");
    QTest::newRow("jobs is a number") << QByteArray("{\"jobs\":3}");
    QTest::newRow("error reply") << QByteArray(
        "{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":{\"code\":1,\"message\":\"failed\"}}");
}

void BJsonStreamReaderTest::jobsArrayEmptyForOtherShapes()
{
    QFETCH(QByteArray, json);
    BJsonStreamReader reader;
    reader.receiveData(json);
    QVERIFY(reader.parseJson());
    QVERIFY(reader.jobsArray().isEmpty());
}

void BJsonStreamReaderTest::malformedInputReportsError_data()
{
    QTest::addColumn<QByteArray>("json");
    QTest::newRow("nothing received") << QByteArray();
    QTest::newRow("only blanks") << QByteArray(" \r\n\t");
    QTest::newRow("cut reply") << jobsReply().left(jobsReply().size() / 2);
    QTest::newRow("missing closing brace") << QByteArray("{\"jobs\":[]");
    QTest::newRow("trailing comma") << QByteArray("{\"jobs\":[1,2,],}");
    QTest::newRow("single quotes") << QByteArray("{'jobs':[]}");
    QTest::newRow("bconsole text") << QByteArray("You have messages.\n");
    QTest::newRow("two documents") << QByteArray("{\"jobs\":[]}{\"jobs\":[]}");
    QTest::newRow("garbage after document") << QByteArray("[1,2]xyz");
    QTest::newRow("invalid UTF-8") << QByteArray("[\"\xff\xfe\"]");
    QTest::newRow("NUL byte") << QByteArray("[1,\0 2]", 7);
}

void BJsonStreamReaderTest::malformedInputReportsError()
{
    QFETCH(QByteArray, json);
    BJsonStreamReader reader;
    QSignalSpy parsed(&reader, &BJsonStreamReader::jsonParsed);
    QSignalSpy errors(&reader, &BJsonStreamReader::parseError);

    reader.receiveData(json);
    QVERIFY(!reader.parseJson());

    QCOMPARE(errors.count(), 1);
    QVERIFY(errors.at(0).at(0).toString().startsWith(QStringLiteral("JSON Parse Error at offset ")));
    QCOMPARE(parsed.count(), 1);
    QCOMPARE(parsed.at(0).at(0).toBool(), false);
    QVERIFY(reader.jsonDocument().isNull());
    QVERIFY(reader.jobsArray().isEmpty());
}

void BJsonStreamReaderTest::failedParseDropsTheOldDocument()
{
    // A broken reply after a good one must not leave the old jobs behind
    BJsonStreamReader reader;
    reader.receiveData(jobsReply());
    QVERIFY(reader.parseJson());
    QCOMPARE(reader.jobsArray().size(), 2);

    reader.clear();
    reader.receiveData("{\"result\":{\"jobs\":[");
    QVERIFY(!reader.parseJson());
    QVERIFY(reader.jobsArray().isEmpty());
}

void BJsonStreamReaderTest::clearForgetsBufferAndDocument()
{
    BJsonStreamReader reader;
    reader.receiveData(jobsReply());
    QVERIFY(reader.parseJson());

    reader.clear();
    QVERIFY(reader.jsonDocument().isNull());
    QVERIFY(reader.jobsArray().isEmpty());

    // Without clear() the second reply would be appended to the first
    reader.receiveData("[{\"jobid\":\"5\"}]");
    QVERIFY(reader.parseJson());
    QCOMPARE(reader.jobsArray().size(), 1);
}

void BJsonStreamReaderTest::setDocumentReplacesParsedDocument()
{
    BJsonStreamReader reader;
    reader.receiveData(jobsReply());
    QVERIFY(reader.parseJson());

    QJsonArray jobs;
    jobs.append(QJsonObject{{"jobid", "42"}});
    reader.setDocument(QJsonDocument(jobs));
    QCOMPARE(reader.jobsArray().size(), 1);
    QCOMPARE(reader.jobsArray().at(0).toObject().value("jobid").toString(), QStringLiteral("42"));
}

void BJsonStreamReaderTest::liveModeReadsAvailableDataAtOnce()
{
    QBuffer device;
    device.setData("[{\"jobid\":\"1\"},{\"jobid\":\"2\"}]");
    QVERIFY(device.open(QIODevice::ReadOnly));

    BJsonStreamReader reader;
    QSignalSpy live(&reader, &BJsonStreamReader::liveDataReceived);
    QVERIFY(!reader.isLiveStreaming());
    reader.connectToDevice(&device, 50);
    QVERIFY(reader.isLiveStreaming());

    // Read inside connectToDevice(), without waiting for readyRead
    QCOMPARE(live.count(), 1);
    QCOMPARE(live.at(0).at(0).toJsonArray().size(), 2);
    QCOMPARE(reader.jobsArray().size(), 2);
    reader.disconnectFromDevice();
}

void BJsonStreamReaderTest::liveModeWaitsForTheCompleteDocument()
{
    QBuffer device;
    QVERIFY(device.open(QIODevice::ReadWrite));

    BJsonStreamReader reader;
    QSignalSpy live(&reader, &BJsonStreamReader::liveDataReceived);
    QSignalSpy errors(&reader, &BJsonStreamReader::parseError);
    reader.connectToDevice(&device, 20);
    QCOMPARE(live.count(), 0);   // nothing available yet

    const QByteArray reply = jobsReply();
    const int half = reply.size() / 2;
    const qint64 readPos = device.pos();
    device.write(reply.left(half));
    device.seek(readPos);
    // The first half alone is no document: an error, no jobs
    QTRY_VERIFY(errors.count() >= 1);
    QCOMPARE(live.count(), 0);

    const qint64 pos = device.pos();
    device.seek(device.size());
    device.write(reply.mid(half));
    device.seek(pos);
    QTRY_COMPARE(live.count(), 1);
    QCOMPARE(live.at(0).at(0).toJsonArray().size(), 2);
    QCOMPARE(reader.jobsArray().size(), 2);
    reader.disconnectFromDevice();
}

void BJsonStreamReaderTest::disconnectStopsReading()
{
    QBuffer device;
    QVERIFY(device.open(QIODevice::ReadWrite));

    BJsonStreamReader reader;
    QSignalSpy live(&reader, &BJsonStreamReader::liveDataReceived);
    reader.connectToDevice(&device, 10);
    reader.disconnectFromDevice();
    QVERIFY(!reader.isLiveStreaming());

    device.write("[{\"jobid\":\"1\"}]");
    device.seek(0);
    QTest::qWait(100);   // several polling intervals and the queued readyRead
    QCOMPARE(live.count(), 0);
    QCOMPARE(device.bytesAvailable(), qint64(15));   // nothing was read

    // Connecting again reads what is waiting
    reader.connectToDevice(&device, 10);
    QCOMPARE(live.count(), 1);
    reader.disconnectFromDevice();
}

QTEST_GUILESS_MAIN(BJsonStreamReaderTest)
#include "bjsonstreamreader_test.moc"
