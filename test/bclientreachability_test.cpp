// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bclientreachability_test.cpp
 * @brief Client reachability (issue #9): "status director", "Check now", order of sources
 *
 * Fixtures in test/fixtures, where they come from:
 * - status-director-bareos-api2.json: built from the Bareos source
 *   (core/src/dird/ua_status.cc ListDirStatusHeader() and
 *   ListConnectedClients(); OutputFormatter lowercases the keys, so
 *   "ConnectTime" arrives as "connecttime", and "authenticated" is written
 *   with the integer overload of ObjectKeyValue(), so it is 1). Not captured
 *   from a running Bareos: no Bareos with client-initiated connections was
 *   at hand.
 * - status-director-onesimus-api2.json, status-director-no-connections-api2.json:
 *   the shape of the Onesimus Director (onesimus-dird
 *   crates/odir/src/commands/status.rs list_connected_clients():
 *   "authenticated" is true).
 * - status-director-typed-without-list.json: the typed GetDirectorStatus
 *   reply of an Onesimus Director (protobuf JSON), which leaves out an empty
 *   client_connection; captured, names changed.
 * - status-client-reachable-*.txt: ClientStatusResponse.text of the
 *   Onesimus Director for a bareos-fd and for its Rust file daemon;
 *   captured, names and addresses changed.
 * - status-client-unreachable.txt, status-client-disabled.txt: the texts of
 *   Bareos dird/fd_cmds.cc DoNativeClientStatus() and dird/ua_status.cc
 *   ClientStatus() (the Onesimus Director writes the same).
 * - status-client-api2-empty.json: what the Bareos source lets "status
 *   client" send in API mode 2 (the text goes to the JSON messages, which
 *   are not sent): an empty result.
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>

#include <QFile>

#include "clients/bclientreachability.h"

using namespace BClientReachability;

namespace {

QString fixture(const char *name)
{
    QFile file(QStringLiteral(FIXTURE_DIR "/") + QLatin1String(name));
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "missing fixture" << file.fileName();
        return QString();
    }
    return QString::fromUtf8(file.readAll());
}

const QTimeZone Utc = QTimeZone::UTC;

} // namespace

class BClientReachabilityTest : public QObject
{
    Q_OBJECT

private slots:
    // "status director"
    void bareosConnections();
    void onesimusConnections();
    void emptyListIsRecognized();
    void replyWithoutListChangesNothing();
    void unknownShapesAreTolerated_data();
    void unknownShapesAreTolerated();
    void unauthenticatedAndNamelessEntriesAreSkipped();
    void otherKeySpellings();
    void connectTimes_data();
    void connectTimes();

    // "Check now"
    void probeReplies_data();
    void probeReplies();
    void probeTransport();

    // Order of the sources
    void openConnectionWins();
    void newestObservationWins();
    void observationsRunOut();
    void offlineEndsWithANewerBackup();
    void nothingLeavesTheEstimate();
};

// ---------------------------------------------------------------------------
// "status director"
// ---------------------------------------------------------------------------

void BClientReachabilityTest::bareosConnections()
{
    const ConnectionList list = parseStatusDirector(fixture("status-director-bareos-api2.json"), Utc);
    QVERIFY(list.recognized);
    QCOMPARE(list.clients.size(), 2);
    QCOMPARE(list.clients.at(0).name, QStringLiteral("laptop-fd"));
    QCOMPARE(list.clients.at(0).connectTimeRaw, QStringLiteral("23-Nov-26 10:15"));
    QCOMPARE(list.clients.at(0).connectTime, QDateTime(QDate(2026, 11, 23), QTime(10, 15), Utc));
    QCOMPARE(list.clients.at(0).protocolVersion, 2);
    QCOMPARE(list.clients.at(1).name, QStringLiteral("phone-fd"));
}

void BClientReachabilityTest::onesimusConnections()
{
    const ConnectionList list = parseStatusDirector(fixture("status-director-onesimus-api2.json"), Utc);
    QVERIFY(list.recognized);
    QCOMPARE(list.clients.size(), 1);
    QCOMPARE(list.clients.at(0).name, QStringLiteral("laptop-fd"));
    QCOMPARE(list.clients.at(0).protocolVersion, 54);
    QCOMPARE(list.clients.at(0).connectTime, QDateTime(QDate(2026, 11, 23), QTime(18, 31), Utc));
}

void BClientReachabilityTest::emptyListIsRecognized()
{
    // An empty list is news: nobody is connected right now
    const ConnectionList list =
        parseStatusDirector(fixture("status-director-no-connections-api2.json"), Utc);
    QVERIFY(list.recognized);
    QVERIFY(list.clients.isEmpty());
}

void BClientReachabilityTest::replyWithoutListChangesNothing()
{
    const ConnectionList list =
        parseStatusDirector(fixture("status-director-typed-without-list.json"), Utc);
    QVERIFY(!list.recognized);
    QVERIFY(list.clients.isEmpty());
}

void BClientReachabilityTest::unknownShapesAreTolerated_data()
{
    QTest::addColumn<QString>("reply");
    QTest::newRow("empty") << QString();
    QTest::newRow("text") << QStringLiteral("Client Initiated Connections (waiting for jobs):\n====\n");
    QTest::newRow("broken json") << QStringLiteral("{\"result\":{\"client-connection\":[");
    QTest::newRow("array root") << QStringLiteral("[1,2,3]");
    QTest::newRow("list is an object") << QStringLiteral(R"({"result":{"client-connection":{"name":"a-fd"}}})");
    QTest::newRow("list is a string") << QStringLiteral(R"({"result":{"client-connection":"a-fd"}})");
    QTest::newRow("error reply") << QStringLiteral(
        R"({"jsonrpc":"2.0","id":null,"error":{"code":1,"message":"failed","data":{"messages":{"error":["status: is an invalid command."]}}}})");
    QTest::newRow("other reply") << fixture("status-client-api2-empty.json");
}

void BClientReachabilityTest::unknownShapesAreTolerated()
{
    QFETCH(QString, reply);
    const ConnectionList list = parseStatusDirector(reply, Utc);
    QVERIFY(!list.recognized);
    QVERIFY(list.clients.isEmpty());
}

void BClientReachabilityTest::unauthenticatedAndNamelessEntriesAreSkipped()
{
    const ConnectionList list = parseStatusDirector(QStringLiteral(
        R"({"result":{"client-connection":[
            {"name":"a-fd","authenticated":0},
            {"name":"b-fd","authenticated":false},
            {"name":"","authenticated":1},
            {"authenticated":1},
            "c-fd",
            {"name":"d-fd"},
            {"name":"e-fd","authenticated":"1","connecttime":"not a time","protocol_version":"7"}
        ]}})"), Utc);
    QVERIFY(list.recognized);
    QCOMPARE(list.clients.size(), 2);
    QCOMPARE(list.clients.at(0).name, QStringLiteral("d-fd"));
    QCOMPARE(list.clients.at(0).protocolVersion, -1);
    QCOMPARE(list.clients.at(1).name, QStringLiteral("e-fd"));
    QVERIFY(!list.clients.at(1).connectTime.isValid());
    QCOMPARE(list.clients.at(1).connectTimeRaw, QStringLiteral("not a time"));
    QCOMPARE(list.clients.at(1).protocolVersion, 7);
}

void BClientReachabilityTest::otherKeySpellings()
{
    // The protobuf field (typed reply) and a failed reply that carries a result
    const ConnectionList typed = parseStatusDirector(QStringLiteral(
        R"({"clientConnection":[{"connecttime":"x","protocolVersion":54,"authenticated":true,"name":"fd"}]})"), Utc);
    QVERIFY(typed.recognized);
    QCOMPARE(typed.clients.size(), 1);
    QCOMPARE(typed.clients.at(0).protocolVersion, 54);

    const ConnectionList snake = parseStatusDirector(QStringLiteral(
        R"({"client_connection":[{"name":"fd"}]})"), Utc);
    QVERIFY(snake.recognized);
    QCOMPARE(snake.clients.size(), 1);

    const ConnectionList failed = parseStatusDirector(QStringLiteral(
        R"({"error":{"code":1,"message":"failed","data":{"result":{"client-connection":[{"name":"fd"}]},"messages":{}}}})"), Utc);
    QVERIFY(failed.recognized);
    QCOMPARE(failed.clients.size(), 1);
}

void BClientReachabilityTest::connectTimes_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QDateTime>("expected");
    QTest::newRow("bstrftime_nc") << QStringLiteral("23-Nov-26 10:15")
                                  << QDateTime(QDate(2026, 11, 23), QTime(10, 15), Utc);
    QTest::newRow("single-digit day") << QStringLiteral("3-Jan-27 07:05")
                                      << QDateTime(QDate(2027, 1, 3), QTime(7, 5), Utc);
    QTest::newRow("four-digit year") << QStringLiteral("23-Nov-2026 18:31")
                                     << QDateTime(QDate(2026, 11, 23), QTime(18, 31), Utc);
    QTest::newRow("iso") << QStringLiteral("2026-11-23 10:15:42")
                         << QDateTime(QDate(2026, 11, 23), QTime(10, 15, 42), Utc);
    QTest::newRow("empty") << QString() << QDateTime();
    QTest::newRow("garbage") << QStringLiteral("x") << QDateTime();
    QTest::newRow("localized month") << QStringLiteral("23-Okt-26 10:15") << QDateTime();
}

void BClientReachabilityTest::connectTimes()
{
    QFETCH(QString, text);
    QFETCH(QDateTime, expected);
    QCOMPARE(parseConnectTime(text, Utc), expected);
}

// ---------------------------------------------------------------------------
// "Check now"
// ---------------------------------------------------------------------------

void BClientReachabilityTest::probeReplies_data()
{
    QTest::addColumn<QString>("reply");
    QTest::addColumn<int>("result");
    QTest::addColumn<int>("reason");
    QTest::addColumn<QString>("detail");

    QTest::newRow("bareos-fd answered")
        << fixture("status-client-reachable-bareos-fd.txt") << int(Probe::Reachable)
        << int(ProbeReason::Answered)
        << QStringLiteral("server-fd Version: 25.1.2~pre49.d5742f640 (16 October 2026) Debian GNU/Linux 13 (trixie)");
    QTest::newRow("rust fd answered")
        << fixture("status-client-reachable-rust-fd.txt") << int(Probe::Reachable)
        << int(ProbeReason::Answered)
        << QStringLiteral("laptop.example.org-fd Version: 25.1.2+onesimus.0.2.0 (23 November 2026) linux");
    QTest::newRow("connect failed")
        << fixture("status-client-unreachable.txt") << int(Probe::Unreachable)
        << int(ProbeReason::ConnectFailed) << QStringLiteral("Failed to connect to Client laptop-fd.");
    QTest::newRow("connect failed, job message form")
        << QStringLiteral("\nFailed to connect to client \"laptop-fd\".\n") << int(Probe::Unreachable)
        << int(ProbeReason::ConnectFailed) << QStringLiteral("Failed to connect to client \"laptop-fd\".");
    QTest::newRow("disabled")
        << fixture("status-client-disabled.txt") << int(Probe::Unknown) << int(ProbeReason::Disabled)
        << QStringLiteral("Client \"old-fd\" is disabled; skipping status call");
    QTest::newRow("bareos api 2: empty result")
        << fixture("status-client-api2-empty.json") << int(Probe::Unknown)
        << int(ProbeReason::NoEvidence) << QString();
    QTest::newRow("empty") << QString() << int(Probe::Unknown) << int(ProbeReason::NoEvidence) << QString();
    QTest::newRow("only the connect line")
        << QStringLiteral("Connecting to Client laptop-fd at 192.0.2.20:9102\n") << int(Probe::Unknown)
        << int(ProbeReason::NoEvidence) << QStringLiteral("Connecting to Client laptop-fd at 192.0.2.20:9102");
    // A version line alone is not enough (the Director's own header has one)
    QTest::newRow("version without daemon started")
        << QStringLiteral("bareos-dir Version: 25.2.1 (12 March 2026)\n") << int(Probe::Unknown)
        << int(ProbeReason::NoEvidence) << QStringLiteral("bareos-dir Version: 25.2.1 (12 March 2026)");
    // The file daemon's header can only come from the file daemon
    QTest::newRow("failed first, then answered")
        << QStringLiteral("Failed to connect to client \"a-fd\".\n")
               + fixture("status-client-reachable-rust-fd.txt")
        << int(Probe::Reachable) << int(ProbeReason::Answered)
        << QStringLiteral("laptop.example.org-fd Version: 25.1.2+onesimus.0.2.0 (23 November 2026) linux");
    // A Director that wraps the text in JSON
    QTest::newRow("json messages, failed")
        << QStringLiteral(R"({"jsonrpc":"2.0","id":null,"result":{},"messages":{"normal":["\nFailed to connect to Client a-fd.\n====\n"]}})")
        << int(Probe::Unreachable) << int(ProbeReason::ConnectFailed)
        << QStringLiteral("Failed to connect to Client a-fd.");
    QTest::newRow("json text, answered")
        << QStringLiteral(R"({"text":"a-fd Version: 25.2.1 (12 March 2026) Linux\nDaemon started 23-Nov-26 08:00. Jobs: run=0 running=0\n"})")
        << int(Probe::Reachable) << int(ProbeReason::Answered)
        << QStringLiteral("a-fd Version: 25.2.1 (12 March 2026) Linux");
}

void BClientReachabilityTest::probeReplies()
{
    QFETCH(QString, reply);
    QFETCH(int, result);
    QFETCH(int, reason);
    QFETCH(QString, detail);

    const ProbeOutcome outcome = classifyProbeReply(reply);
    QCOMPARE(int(outcome.result), result);
    QCOMPARE(int(outcome.reason), reason);
    if (outcome.reason != ProbeReason::NoEvidence || !detail.isEmpty())
        QCOMPARE(outcome.detail, detail);
}

void BClientReachabilityTest::probeTransport()
{
    // Timeouts and errors are never taken as "unreachable"
    ProbeOutcome outcome = classifyProbe(BClientProbeStatus::Timeout, QString(), QStringLiteral("DEADLINE_EXCEEDED"));
    QCOMPARE(outcome.result, Probe::Unknown);
    QCOMPARE(outcome.reason, ProbeReason::Timeout);

    outcome = classifyProbe(BClientProbeStatus::Error, QString(), QStringLiteral("PERMISSION_DENIED: ACL"));
    QCOMPARE(outcome.result, Probe::Unknown);
    QCOMPARE(outcome.reason, ProbeReason::Error);
    QCOMPARE(outcome.detail, QStringLiteral("PERMISSION_DENIED: ACL"));

    outcome = classifyProbe(BClientProbeStatus::NotAvailable, QString(), QStringLiteral("console protocol"));
    QCOMPARE(outcome.result, Probe::Unknown);
    QCOMPARE(outcome.reason, ProbeReason::NotAvailable);

    outcome = classifyProbe(BClientProbeStatus::Busy, QString(), QString());
    QCOMPARE(outcome.result, Probe::Unknown);

    // Replied: the text decides
    outcome = classifyProbe(BClientProbeStatus::Replied, fixture("status-client-unreachable.txt"), QString());
    QCOMPARE(outcome.result, Probe::Unreachable);
    outcome = classifyProbe(BClientProbeStatus::Replied, fixture("status-client-reachable-bareos-fd.txt"), QString());
    QCOMPARE(outcome.result, Probe::Reachable);
}

// ---------------------------------------------------------------------------
// Order of the sources
// ---------------------------------------------------------------------------

void BClientReachabilityTest::openConnectionWins()
{
    const QDateTime t0(QDate(2026, 11, 23), QTime(12, 0), Utc);
    Inputs inputs;
    inputs.observation = {State::Offline, Source::Probe, t0.addSecs(60), QDateTime()};

    // An open connection reported by an event beats a newer failed probe
    inputs.openEvent = {State::Online, Source::Event, t0, QDateTime()};
    Fact fact = resolve(inputs, t0.addSecs(120));
    QCOMPARE(fact.state, State::Online);
    QCOMPARE(fact.source, Source::Event);

    // A client-initiated connection beats the event
    inputs.listed = {State::Online, Source::Connection, t0.addSecs(-3600), QDateTime()};
    fact = resolve(inputs, t0.addSecs(120));
    QCOMPARE(fact.state, State::Online);
    QCOMPARE(fact.source, Source::Connection);
    QCOMPARE(fact.at, t0.addSecs(-3600));
}

void BClientReachabilityTest::newestObservationWins()
{
    const QDateTime t0(QDate(2026, 11, 23), QTime(12, 0), Utc);
    Inputs inputs;

    // The model keeps the newest observation; event and probe weigh the same
    inputs.observation = {State::Offline, Source::Event, t0, QDateTime()};
    QCOMPARE(resolve(inputs, t0.addSecs(30)).state, State::Offline);
    inputs.observation = {State::Online, Source::Probe, t0.addSecs(60), t0.addSecs(60 + OnlineValidSecs)};
    const Fact fact = resolve(inputs, t0.addSecs(90));
    QCOMPARE(fact.state, State::Online);
    QCOMPARE(fact.source, Source::Probe);
    QCOMPARE(fact.at, t0.addSecs(60));
}

void BClientReachabilityTest::observationsRunOut()
{
    const QDateTime t0(QDate(2026, 11, 23), QTime(12, 0), Utc);
    Inputs inputs;
    inputs.observation = {State::Online, Source::Probe, t0, t0.addSecs(OnlineValidSecs)};
    QCOMPARE(resolve(inputs, t0.addSecs(OnlineValidSecs - 1)).state, State::Online);
    QCOMPARE(resolve(inputs, t0.addSecs(OnlineValidSecs)).state, State::None);

    // Offline holds until the next contact
    inputs.observation = {State::Offline, Source::Probe, t0, QDateTime()};
    QCOMPARE(resolve(inputs, t0.addDays(30)).state, State::Offline);
}

void BClientReachabilityTest::offlineEndsWithANewerBackup()
{
    const QDateTime t0(QDate(2026, 11, 23), QTime(12, 0), Utc);
    Inputs inputs;
    inputs.observation = {State::Offline, Source::Probe, t0, QDateTime()};
    // A backup before the failed check changes nothing
    QCOMPARE(resolve(inputs, t0.addDays(1), t0.addSecs(-60)).state, State::Offline);
    // A backup after it was a contact: back to the estimate
    QCOMPARE(resolve(inputs, t0.addDays(1), t0.addSecs(3600)).state, State::None);
    // Online is not cut short by the catalog
    inputs.observation = {State::Online, Source::Probe, t0, t0.addSecs(OnlineValidSecs)};
    QCOMPARE(resolve(inputs, t0.addSecs(60), t0.addSecs(30)).state, State::Online);
}

void BClientReachabilityTest::nothingLeavesTheEstimate()
{
    const QDateTime now(QDate(2026, 11, 23), QTime(12, 0), Utc);
    const Fact fact = resolve(Inputs(), now);
    QCOMPARE(fact.state, State::None);
    QCOMPARE(fact.source, Source::None);
}

QTEST_GUILESS_MAIN(BClientReachabilityTest)
#include "bclientreachability_test.moc"
