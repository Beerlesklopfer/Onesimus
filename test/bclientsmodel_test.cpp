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

    // "status director" and "Check now" (issue #9, P3-12)
    void clientInitiatedConnectionIsConnected();
    void droppedFromTheListStaysOnlineForAWhile();
    void replyWithoutTheListChangesNothing();
    void probeResultsAndTheirOrder();
    void unknownProbeKeepsTheStatus();
    void noProbeWithoutADirector();
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

    // A client without a backups reply has no figures yet, not those of
    // the page (P1-13: they would flip when the reply comes)
    const QJsonObject b = row(model, QStringLiteral("b-fd"));
    QVERIFY(!b.contains("jobcount"));
    QVERIFY(!b.contains("totalbytes"));
    QCOMPARE(b["lastconnection"].toString(), QString("2026-11-23 01:30:00"));

    // The order of the replies does not matter
    model.setClientBackups(QStringLiteral("b-fd"), QJsonArray{ job("b-fd", "2026-11-01 01:30:00", 9) });
    const QJsonObject b2 = row(model, QStringLiteral("b-fd"));
    QCOMPARE(b2["jobcount"].toInt(), 1);
    QCOMPARE(b2["totalbytes"].toString(), QString("9"));
    QCOMPARE(b2["lastconnection"].toString(), QString("2026-11-23 01:30:00"));
    QCOMPARE(row(model, QStringLiteral("a-fd"))["jobcount"].toInt(), 3);

    // Another page does not change the figures
    model.enrichWithJobData(QJsonArray{ job("b-fd", "2026-11-23 02:30:00", 123456) });
    QCOMPARE(row(model, QStringLiteral("b-fd"))["jobcount"].toInt(), 1);
    QCOMPARE(row(model, QStringLiteral("b-fd"))["totalbytes"].toString(), QString("9"));
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

    // A refresh: while the round runs, the old figures stay (no flip to
    // other figures in between) ...
    model.beginBackupRound({QStringLiteral("a-fd"), QStringLiteral("b-fd")});
    QCOMPARE(row(model, QStringLiteral("b-fd"))["jobcount"].toInt(), 2);
    model.setClientBackups(QStringLiteral("a-fd"), QJsonArray{ job("a-fd", "2026-11-03 01:30:00", 5) });
    QCOMPARE(row(model, QStringLiteral("b-fd"))["jobcount"].toInt(), 2);

    // ... b-fd's backups have been pruned, so no reply names it: when the
    // round is over it has none
    model.finishBackupRound();
    const QJsonObject b = row(model, QStringLiteral("b-fd"));
    QCOMPARE(b["jobcount"].toInt(), 0);
    QCOMPARE(b["totalbytes"].toString(), QString("0"));
    QCOMPARE(row(model, QStringLiteral("a-fd"))["jobcount"].toInt(), 1);
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
    // An open connection is shown as such
    QCOMPARE(a.data().toString(), QStringLiteral("Connected"));
    QCOMPARE(a.data(BStatus::ToneRole).toInt(), int(BStatus::Tone::Success));
    QVERIFY(a.data(Qt::ToolTipRole).toString().contains("open connection"));

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

namespace {
const char *BareosReply =
    R"({"jsonrpc":"2.0","id":null,"result":{"header":{},"scheduled":[],"running":[],"terminated":[],)"
    R"("client-connection":[{"connecttime":"23-Nov-26 10:15","protocol_version":2,"authenticated":1,"name":"a-fd"}]}})";
const char *EmptyListReply =
    R"({"jsonrpc":"2.0","id":null,"result":{"header":{},"scheduled":[],"running":[],"terminated":[],"client-connection":[]}})";
} // namespace

void BClientsModelTest::clientInitiatedConnectionIsConnected()
{
    BClientsModel model(nullptr);
    model.setAllClients(twoClients());
    model.setClients(model.allClients());
    QSignalSpy resets(&model, &QAbstractItemModel::modelReset);

    QVERIFY(model.setStatusDirectorReply(QString::fromLatin1(BareosReply)));
    const QModelIndex a = statusIndex(model, "a-fd");
    QCOMPARE(a.data().toString(), QStringLiteral("Connected"));
    QCOMPARE(a.data(BStatus::ToneRole).toInt(), int(BStatus::Tone::Success));
    QVERIFY(a.data(Qt::ToolTipRole).toString().contains("client-initiated"));
    const BClientsModel::LiveStatus live = model.liveStatus("a-fd", QDateTime::currentDateTime());
    QCOMPARE(live.source, BClientsModel::Source::Connection);
    QVERIFY(live.connected);
    QCOMPARE(live.at.time(), QTime(10, 15));
    // b-fd is not listed: no fact, the estimate stays
    QCOMPARE(model.liveStatus("b-fd", QDateTime::currentDateTime()).state, BClientsModel::LiveStatus::None);
    QCOMPARE(resets.count(), 0);

    // The connection wins over a failed check
    BClientReachability::ProbeOutcome failed;
    failed.result = BClientReachability::Probe::Unreachable;
    failed.reason = BClientReachability::ProbeReason::ConnectFailed;
    model.setProbeResult("a-fd", failed);
    QCOMPARE(statusIndex(model, "a-fd").data().toString(), QStringLiteral("Connected"));
}

void BClientsModelTest::droppedFromTheListStaysOnlineForAWhile()
{
    BClientsModel model(nullptr);
    model.setAllClients(twoClients());
    const QDateTime t0 = QDateTime::currentDateTime().addSecs(-60);

    model.setStatusDirectorReply(QString::fromLatin1(BareosReply), t0);
    model.setStatusDirectorReply(QString::fromLatin1(EmptyListReply), t0.addSecs(30));

    // Seen connected at t0; Bareos takes the connection out of the list for a job
    const BClientsModel::LiveStatus live = model.liveStatus("a-fd", t0.addSecs(31));
    QCOMPARE(live.state, BClientsModel::LiveStatus::Online);
    QVERIFY(!live.connected);
    QCOMPARE(live.at, t0);
    QCOMPARE(model.liveStatus("a-fd", t0.addSecs(BClientsModel::OnlineValidSecs)).state,
             BClientsModel::LiveStatus::None);
}

void BClientsModelTest::replyWithoutTheListChangesNothing()
{
    BClientsModel model(nullptr);
    model.setAllClients(twoClients());
    model.setStatusDirectorReply(QString::fromLatin1(BareosReply));

    // A reply in the wrong place (queue order, backlog P3-1) or of another
    // shape: the connection stays as it was
    QVERIFY(!model.setStatusDirectorReply(QStringLiteral(R"({"jsonrpc":"2.0","id":null,"result":{"clients":[]}})")));
    QVERIFY(!model.setStatusDirectorReply(QStringLiteral("not json")));
    QVERIFY(model.liveStatus("a-fd", QDateTime::currentDateTime()).connected);
}

void BClientsModelTest::probeResultsAndTheirOrder()
{
    using namespace BClientReachability;
    BClientsModel model(nullptr);
    model.setAllClients(twoClients());
    model.setClients(model.allClients());
    const QDateTime t0 = QDateTime::currentDateTime().addSecs(-120);

    ProbeOutcome answered;
    answered.result = Probe::Reachable;
    answered.reason = ProbeReason::Answered;
    ProbeOutcome failed;
    failed.result = Probe::Unreachable;
    failed.reason = ProbeReason::ConnectFailed;

    model.setProbeResult("b-fd", failed, t0);
    QCOMPARE(statusIndex(model, "b-fd").data().toString(), QStringLiteral("Offline"));
    QVERIFY(statusIndex(model, "b-fd").data(Qt::ToolTipRole).toString().contains("Check now"));

    // A newer client event replaces the probe; a newer probe the event
    model.setClientLiveEvent("b-fd", BClientEvent::Alive, t0.addSecs(10));
    QCOMPARE(model.liveStatus("b-fd", t0.addSecs(20)).source, BClientsModel::Source::Event);
    model.setProbeResult("b-fd", answered, t0.addSecs(20));
    const BClientsModel::LiveStatus live = model.liveStatus("b-fd", t0.addSecs(30));
    QCOMPARE(live.state, BClientsModel::LiveStatus::Online);
    QCOMPARE(live.source, BClientsModel::Source::Probe);
    QCOMPARE(statusIndex(model, "b-fd").data().toString(), QStringLiteral("Online"));
    QCOMPARE(model.liveStatus("b-fd", t0.addSecs(20 + BClientsModel::OnlineValidSecs)).state,
             BClientsModel::LiveStatus::None);
}

void BClientsModelTest::unknownProbeKeepsTheStatus()
{
    using namespace BClientReachability;
    BClientsModel model(nullptr);
    model.setAllClients(twoClients());
    model.setClients(model.allClients());

    ProbeOutcome failed;
    failed.result = Probe::Unreachable;
    failed.reason = ProbeReason::ConnectFailed;
    model.setProbeResult("a-fd", failed);

    // A timeout says nothing: still Offline, the tooltip tells about the check
    model.setProbeResult("a-fd", classifyProbe(BClientProbeStatus::Timeout, QString(), QString()));
    QCOMPARE(statusIndex(model, "a-fd").data().toString(), QStringLiteral("Offline"));
    QVERIFY(statusIndex(model, "a-fd").data(Qt::ToolTipRole).toString().contains("no answer in time"));

    // On a client without facts the estimate stays, with the check in the tooltip
    model.setProbeResult("b-fd", classifyProbe(BClientProbeStatus::Error, QString(), QStringLiteral("NOT_FOUND")));
    QCOMPARE(model.liveStatus("b-fd", QDateTime::currentDateTime()).state, BClientsModel::LiveStatus::None);
    QVERIFY(statusIndex(model, "b-fd").data(Qt::ToolTipRole).toString().contains("NOT_FOUND"));
    QCOMPARE(model.probeOutcome("b-fd").first.reason, ProbeReason::Error);
}

void BClientsModelTest::noProbeWithoutADirector()
{
    BClientsModel model(nullptr);
    model.setAllClients(twoClients());
    QSignalSpy started(&model, &BClientsModel::probeStateChanged);
    QVERIFY(!model.checkClientNow("a-fd"));
    QVERIFY(model.probingClient().isEmpty());
    QCOMPARE(started.count(), 0);
}

QTEST_GUILESS_MAIN(BClientsModelTest)
#include "bclientsmodel_test.moc"
