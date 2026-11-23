// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bclientsmodel_test.cpp
 * @brief Job figures of the client table (BClientsModel)
 *
 * The replies are written out in the form of "llist clients", "list jobs"
 * and "list backups client=<name>" in API mode 2 (a "result" object with
 * the rows in an array), with only the fields the model reads.
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>

#include <QJsonArray>
#include <QJsonObject>
#include <QSignalSpy>

#include "clients/bclientsmodel.h"
#include "bstatuschip.h"

namespace {

QJsonObject job(const QString &client, const QString &start, qint64 bytes)
{
    QJsonObject j;
    j["client"] = client;
    j["starttime"] = start;
    j["jobbytes"] = QString::number(bytes);
    return j;
}

QJsonArray twoClients()
{
    QJsonObject a;
    a["name"] = QStringLiteral("a-fd");
    QJsonObject b;
    b["name"] = QStringLiteral("b-fd");
    return QJsonArray{ a, b };
}

/// The enriched row of a client in the unfiltered table
QJsonObject row(BClientsModel &model, const QString &name)
{
    model.setClients(model.allClients());
    for (int r = 0; r < model.rowCount(); ++r) {
        const QJsonObject client = model.enrichedClient(r);
        if (client["name"].toString() == name) return client;
    }
    return QJsonObject();
}

} // namespace

class BClientsModelTest : public QObject
{
    Q_OBJECT

private slots:
    void backupsAreNotOverwrittenByTheJobList();
    void backupRepliesOfAllClientsResetOnce();
    void backupsOfAnEarlierRoundAreForgotten();

    // Client events of the Director (P1-12)
    void clientStatusDrivenByEvents();
    void eventUpdatesTheRowWithoutReset();
    void onlineRunsOutAfterTheConnection();
    void unreachableStaysUntilTheNextContact();
    void withoutEventsNoOnlineIsShown();
};

void BClientsModelTest::backupsAreNotOverwrittenByTheJobList()
{
    BClientsModel model(nullptr);
    model.setAllClients(twoClients());

    // All backups of a-fd ...
    model.setClientBackups(QStringLiteral("a-fd"), QJsonArray{
        job("a-fd", "2026-11-20 23:05:00", 1000),
        job("a-fd", "2026-11-21 23:05:00", 2000),
        job("a-fd", "2026-11-22 23:05:00", 3000) });
    // ... and then a page of the job list, with one job of each client and
    // a newer start of a-fd than its last backup
    model.enrichWithJobData(QJsonArray{
        job("a-fd", "2026-11-23 06:00:00", 7),
        job("b-fd", "2026-11-22 01:30:00", 500),
        job("b-fd", "2026-11-23 01:30:00", 600) });

    const QJsonObject a = row(model, QStringLiteral("a-fd"));
    QCOMPARE(a["jobcount"].toInt(), 3);                         // the backups, not the page
    QCOMPARE(a["totalbytes"].toString(), QString("6000"));
    QCOMPARE(a["lastconnection"].toString(), QString("2026-11-23 06:00:00"));  // the newest of both

    // A client without a backups reply has the figures of the page
    const QJsonObject b = row(model, QStringLiteral("b-fd"));
    QCOMPARE(b["jobcount"].toInt(), 2);
    QCOMPARE(b["totalbytes"].toString(), QString("1100"));

    // The order of the replies does not matter
    model.setClientBackups(QStringLiteral("b-fd"), QJsonArray{ job("b-fd", "2026-11-01 01:30:00", 9) });
    const QJsonObject b2 = row(model, QStringLiteral("b-fd"));
    QCOMPARE(b2["jobcount"].toInt(), 1);
    QCOMPARE(b2["totalbytes"].toString(), QString("9"));
    QCOMPARE(b2["lastconnection"].toString(), QString("2026-11-23 01:30:00"));
    QCOMPARE(row(model, QStringLiteral("a-fd"))["jobcount"].toInt(), 3);
}

void BClientsModelTest::backupRepliesOfAllClientsResetOnce()
{
    BClientsModel model(nullptr);
    QJsonArray clients;
    for (int i = 0; i < 50; ++i) {
        QJsonObject c;
        c["name"] = QStringLiteral("c%1-fd").arg(i);
        clients.append(c);
    }
    model.setAllClients(clients);

    // One reply per client after a connect: the table (filtered and reset
    // on allClientsChanged) is not reset fifty times
    QSignalSpy changed(&model, &BClientsModel::allClientsChanged);
    for (int i = 0; i < 50; ++i) {
        const QString name = QStringLiteral("c%1-fd").arg(i);
        model.setClientBackups(name, QJsonArray{ job(name, "2026-11-22 23:05:00", i) });
    }
    QCOMPARE(changed.count(), 0);
    QTRY_COMPARE_WITH_TIMEOUT(changed.count(), 1, 5000);
    QTest::qWait(2 * BClientsModel::BackupsResetDelayMs);
    QCOMPARE(changed.count(), 1);
    QCOMPARE(row(model, QStringLiteral("c49-fd"))["totalbytes"].toString(), QString("49"));
}

void BClientsModelTest::backupsOfAnEarlierRoundAreForgotten()
{
    BClientsModel model(nullptr);
    model.setAllClients(twoClients());
    model.enrichWithJobData(QJsonArray{ job("b-fd", "2026-11-23 01:30:00", 600) });
    model.setClientBackups(QStringLiteral("b-fd"), QJsonArray{
        job("b-fd", "2026-11-01 01:30:00", 9),
        job("b-fd", "2026-11-02 01:30:00", 9) });
    QCOMPARE(row(model, QStringLiteral("b-fd"))["jobcount"].toInt(), 2);

    // A refresh: b-fd's backups have been pruned, so no reply names it
    model.forgetClientBackups();
    const QJsonObject b = row(model, QStringLiteral("b-fd"));
    QCOMPARE(b["jobcount"].toInt(), 1);                         // the page again
    QCOMPARE(b["totalbytes"].toString(), QString("600"));
}

namespace {
QModelIndex statusIndex(BClientsModel &model, const QString &name)
{
    for (int r = 0; r < model.rowCount(); ++r) {
        if (model.index(r, BClientsModel::COL_NAME).data().toString() == name)
            return model.index(r, BClientsModel::COL_STATUS);
    }
    return QModelIndex();
}
} // namespace

void BClientsModelTest::clientStatusDrivenByEvents()
{
    BClientsModel model(nullptr);
    model.setAllClients(twoClients());
    model.setClients(model.allClients());
    const QDateTime now = QDateTime::currentDateTime();

    model.setClientLiveEvent("a-fd", BClientEvent::Connected, now);
    const QModelIndex a = statusIndex(model, "a-fd");
    QCOMPARE(a.data().toString(), QStringLiteral("Online"));
    QCOMPARE(a.data(BStatus::ToneRole).toInt(), int(BStatus::Tone::Success));
    QVERIFY(a.data(Qt::ToolTipRole).toString().contains("reached"));

    model.setClientLiveEvent("b-fd", BClientEvent::Unreachable, now);
    const QModelIndex b = statusIndex(model, "b-fd");
    QCOMPARE(b.data().toString(), QStringLiteral("Offline"));
    QCOMPARE(b.data(BStatus::ToneRole).toInt(), int(BStatus::Tone::Error));

    model.setClientLiveEvent("b-fd", BClientEvent::Alive, now);
    QCOMPARE(b.data().toString(), QStringLiteral("Online"));
}

void BClientsModelTest::eventUpdatesTheRowWithoutReset()
{
    BClientsModel model(nullptr);
    model.setAllClients(twoClients());
    model.setClients(model.allClients());
    QSignalSpy resets(&model, &QAbstractItemModel::modelReset);
    QSignalSpy changes(&model, &QAbstractItemModel::dataChanged);
    QSignalSpy all(&model, &BClientsModel::allClientsChanged);

    model.setClientLiveEvent("b-fd", BClientEvent::Connected);

    QCOMPARE(resets.count(), 0);
    QCOMPARE(all.count(), 0);
    QCOMPARE(changes.count(), 1);
    const QModelIndex changed = changes.at(0).at(0).toModelIndex();
    QCOMPARE(changed, statusIndex(model, "b-fd"));
}

void BClientsModelTest::onlineRunsOutAfterTheConnection()
{
    BClientsModel model(nullptr);
    model.setAllClients(twoClients());
    const QDateTime t0 = QDateTime::currentDateTime().addSecs(-3600);

    model.setClientLiveEvent("a-fd", BClientEvent::Connected, t0);
    // A job may run for hours: online while the connection is open
    QCOMPARE(model.liveStatus("a-fd", t0.addSecs(3 * 3600)).state, BClientsModel::LiveStatus::Online);

    model.setClientLiveEvent("a-fd", BClientEvent::Disconnected, t0.addSecs(600));
    QCOMPARE(model.liveStatus("a-fd", t0.addSecs(600 + BClientsModel::OnlineValidSecs - 1)).state,
             BClientsModel::LiveStatus::Online);
    QCOMPARE(model.liveStatus("a-fd", t0.addSecs(600 + BClientsModel::OnlineValidSecs)).state,
             BClientsModel::LiveStatus::None);

    // An answer to a check holds as long
    model.setClientLiveEvent("b-fd", BClientEvent::Alive, t0);
    QCOMPARE(model.liveStatus("b-fd", t0.addSecs(BClientsModel::OnlineValidSecs)).state,
             BClientsModel::LiveStatus::None);

    // Ran out: the cell shows the last backup again, not "Online"
    model.setClients(model.allClients());
    QVERIFY(statusIndex(model, "a-fd").data().toString() != QStringLiteral("Online"));
}

void BClientsModelTest::unreachableStaysUntilTheNextContact()
{
    BClientsModel model(nullptr);
    model.setAllClients(twoClients());
    const QDateTime t0 = QDateTime::currentDateTime().addDays(-1);

    model.setClientLiveEvent("a-fd", BClientEvent::Unreachable, t0);
    QCOMPARE(model.liveStatus("a-fd", t0.addDays(1)).state, BClientsModel::LiveStatus::Offline);
    QCOMPARE(model.liveStatus("a-fd", t0.addDays(1)).at, t0);

    // The end of a connection says nothing new about an unreachable client
    model.setClientLiveEvent("a-fd", BClientEvent::Disconnected, t0.addSecs(5));
    QCOMPARE(model.liveStatus("a-fd", t0.addDays(1)).state, BClientsModel::LiveStatus::Offline);

    model.setClientLiveEvent("a-fd", BClientEvent::Connected, t0.addSecs(60));
    QCOMPARE(model.liveStatus("a-fd", t0.addDays(1)).state, BClientsModel::LiveStatus::Online);

    // A new connection forgets the events
    model.clear();
    QCOMPARE(model.liveStatus("a-fd", t0.addDays(1)).state, BClientsModel::LiveStatus::None);
}

void BClientsModelTest::withoutEventsNoOnlineIsShown()
{
    BClientsModel model(nullptr);
    QJsonArray clients = twoClients();
    QJsonObject a = clients.at(0).toObject();
    a["lastconnection"] = QDateTime::currentDateTime().addSecs(-60)
                              .toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    clients[0] = a;
    model.setAllClients(clients);
    model.setClients(model.allClients());

    // A contact a minute ago is a recent backup or contact, not "Online"
    const QString text = statusIndex(model, "a-fd").data().toString();
    QVERIFY2(text != QStringLiteral("Online") && text != QStringLiteral("Offline"), qPrintable(text));
    QCOMPARE(statusIndex(model, "a-fd").data(BStatus::ToneRole).toInt(), int(BStatus::Tone::Info));
}

QTEST_GUILESS_MAIN(BClientsModelTest)
#include "bclientsmodel_test.moc"
