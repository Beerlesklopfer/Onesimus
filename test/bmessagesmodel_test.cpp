/**
 * @file bmessagesmodel_test.cpp
 * @brief Row signals of the Director messages list at its history limit
 *
 * The model used to call beginRemoveRows() inside beginResetModel() once
 * more than the history limit of messages was held. QAbstractItemModelTester
 * fails the test on any inconsistent begin/end sequence.
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>
#include <QAbstractItemModelTester>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "messages/bmessageswidget.h"

namespace {

QString reply(int first, int count)
{
    QJsonArray messages;
    for (int i = 0; i < count; ++i) {
        QJsonObject message;
        message["type"] = "info";
        message["logtext"] = QString("message %1").arg(first + i);
        messages.append(message);
    }
    QJsonObject result;
    result["messages"] = messages;
    QJsonObject root;
    root["result"] = result;
    return QString::fromUtf8(QJsonDocument(root).toJson());
}

QString textAt(const BMessagesModel &model, int row)
{
    return model.messages().at(row).text;
}

} // namespace

class BMessagesModelTest : public QObject
{
    Q_OBJECT

private slots:
    void repliesAreAppendedWithoutReset();
    void historyLimitRemovesTheOldestRows();
    void replyLargerThanTheLimit();
    void emptyRepliesChangeNothing();
    void singleMessagesKeepTheLimit();
};

void BMessagesModelTest::repliesAreAppendedWithoutReset()
{
    BMessagesModel model;
    QAbstractItemModelTester tester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    QSignalSpy reset(&model, &QAbstractItemModel::modelReset);
    QSignalSpy inserted(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy removed(&model, &QAbstractItemModel::rowsRemoved);

    QVERIFY(model.parseJsonResponse(reply(0, 3)));
    QVERIFY(model.parseJsonResponse(reply(3, 2)));

    QCOMPARE(model.rowCount(), 5);
    QCOMPARE(model.lastAddedCount(), 2);
    QCOMPARE(reset.count(), 0);
    QCOMPARE(removed.count(), 0);
    QCOMPARE(inserted.count(), 2);
    QCOMPARE(inserted[1][1].toInt(), 3);
    QCOMPARE(inserted[1][2].toInt(), 4);
    QCOMPARE(textAt(model, 4), QString("message 4"));
}

void BMessagesModelTest::historyLimitRemovesTheOldestRows()
{
    BMessagesModel model;
    model.setMaxHistory(10);
    QAbstractItemModelTester tester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    QSignalSpy reset(&model, &QAbstractItemModel::modelReset);
    QSignalSpy removed(&model, &QAbstractItemModel::rowsRemoved);

    QVERIFY(model.parseJsonResponse(reply(0, 8)));
    QCOMPARE(removed.count(), 0);

    // 8 + 5 = 13: the three oldest go
    QVERIFY(model.parseJsonResponse(reply(8, 5)));
    QCOMPARE(model.rowCount(), 10);
    QCOMPARE(model.lastAddedCount(), 5);
    QCOMPARE(reset.count(), 0);
    QCOMPARE(removed.count(), 1);
    QCOMPARE(removed[0][1].toInt(), 0);
    QCOMPARE(removed[0][2].toInt(), 2);
    QCOMPARE(textAt(model, 0), QString("message 3"));
    QCOMPARE(textAt(model, 9), QString("message 12"));

    // At the limit the list no longer grows, the new messages still count
    QVERIFY(model.parseJsonResponse(reply(13, 2)));
    QCOMPARE(model.rowCount(), 10);
    QCOMPARE(model.lastAddedCount(), 2);
    QCOMPARE(textAt(model, 9), QString("message 14"));
}

void BMessagesModelTest::replyLargerThanTheLimit()
{
    BMessagesModel model;   // Default limit: 1000
    QAbstractItemModelTester tester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);

    QVERIFY(model.parseJsonResponse(reply(0, 900)));
    QVERIFY(model.parseJsonResponse(reply(900, 1500)));
    QCOMPARE(model.rowCount(), 1000);
    QCOMPARE(textAt(model, 0), QString("message 1400"));
    QCOMPARE(textAt(model, 999), QString("message 2399"));
}

void BMessagesModelTest::emptyRepliesChangeNothing()
{
    BMessagesModel model;
    QAbstractItemModelTester tester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    QVERIFY(model.parseJsonResponse(reply(0, 2)));

    QSignalSpy reset(&model, &QAbstractItemModel::modelReset);
    QSignalSpy inserted(&model, &QAbstractItemModel::rowsInserted);

    // Bareos answers {} when there are no messages
    QVERIFY(!model.parseJsonResponse(R"({"result":{}})"));
    QVERIFY(!model.parseJsonResponse(reply(0, 0)));
    QVERIFY(!model.parseJsonResponse("not json"));

    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(model.lastAddedCount(), 0);
    QCOMPARE(reset.count(), 0);
    QCOMPARE(inserted.count(), 0);
}

void BMessagesModelTest::singleMessagesKeepTheLimit()
{
    BMessagesModel model;
    model.setMaxHistory(3);
    QAbstractItemModelTester tester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);

    for (int i = 0; i < 5; ++i)
        model.addMessage(QString("single %1").arg(i));

    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(textAt(model, 0), QString("single 2"));
    QCOMPARE(textAt(model, 2), QString("single 4"));

    model.clear();
    QCOMPARE(model.rowCount(), 0);
}

QTEST_MAIN(BMessagesModelTest)
#include "bmessagesmodel_test.moc"
