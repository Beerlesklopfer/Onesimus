/**
 * @file bcapabilities_test.cpp
 * @brief Capability table, server detection, notice texts and notice rules
 *
 * The inputs of the server detection are the strings the application keeps
 * from the login: on the console protocol the Director's greeting
 * ("1000 OK: <name> Version: <version> (<date>)", BareosAuth) or the version
 * taken from it, over gRPC the version field of the login response.
 *
 * Runs without a Director and without widgets (registered with CTest).
 */

#include <QtTest>

#include "bcapabilities.h"

using namespace BCapabilities;

Q_DECLARE_METATYPE(BCapabilities::Connection)
Q_DECLARE_METATYPE(BCapabilities::ServerKind)
Q_DECLARE_METATYPE(BCapabilities::Function)

namespace {

Server bareos()
{
    return describeServer(Connection::Classic,
                          QStringLiteral("1000 OK: bareos-dir Version: 25.2.1 (12 March 2026)"));
}

Server onesimus()
{
    return describeServer(Connection::Grpc, QStringLiteral("25.1.2+onesimus.0.2.0"));
}

Server unidentified()
{
    return describeServer(Connection::Classic, QStringLiteral("unknown"));
}

// Entries that are not in the table: the texts must follow the columns
Entry sample(Support onesimusSupport, const char *manualWay)
{
    return Entry{Function::ModifySchedule, "test.sample", "Sample function", false,
                 onesimusSupport, "%1 cannot do the sample.", manualWay};
}

const char ManualWay[] = "Do it by hand.";

QString noticeOf(Function function, const Server &server)
{
    return notice(function, server).text;
}

} // namespace

class BCapabilitiesTest : public QObject
{
    Q_OBJECT

private slots:
    void describeServer_data();
    void describeServer();

    void tableIsComplete();
    void lookup_data();
    void lookup();
    void unlistedServerKindsFollowTheColumns();
    void liveCapabilitiesDecideOverGrpc();

    void serverLabels();
    void noticeTextOfTheTable();
    void noticeText_data();
    void noticeText();
    void noticeNeverClaimsMoreThanTheTable();

    void implicitRequestOncePerSession();
    void explicitRequestEveryTime();
    void doNotShowAgain();
    void policyIsPerFunction();
};

// ---------------------------------------------------------------------------
// Server detection
// ---------------------------------------------------------------------------

void BCapabilitiesTest::describeServer_data()
{
    QTest::addColumn<Connection>("connection");
    QTest::addColumn<QString>("identity");
    QTest::addColumn<ServerKind>("kind");
    QTest::addColumn<QString>("version");

    // --- Console protocol, greeting as BareosAuth receives it ---
    QTest::newRow("bareos greeting")
        << Connection::Classic
        << QStringLiteral("1000 OK: bareos-dir Version: 25.2.1 (12 March 2026)")
        << ServerKind::Bareos << QStringLiteral("25.2");
    QTest::newRow("bareos greeting, record separator (console 18.2+)")
        << Connection::Classic
        << QStringLiteral("1000\x1eOK: bareos-dir Version: 24.0.3 (01 April 2025)")
        << ServerKind::Bareos << QStringLiteral("24.0");
    QTest::newRow("bareos greeting, bashed spaces")
        << Connection::Classic
        << QStringLiteral("1000\x01OK:\x01pdc-dir\x01Version:\x01" "23.1.0\x01(04\x01June\x01" "2024)")
        << ServerKind::Bareos << QStringLiteral("23.1");
    QTest::newRow("bareos pre-release")
        << Connection::Classic
        << QStringLiteral("1000 OK: bareos-dir Version: 25.0.0~pre1203.5d1b1bf7a (10 October 2025)")
        << ServerKind::Bareos << QStringLiteral("25.0");
    QTest::newRow("bareos, version only (parseDirectorVersion)")
        << Connection::Classic << QStringLiteral("25.2.1")
        << ServerKind::Bareos << QStringLiteral("25.2");
    QTest::newRow("old bareos")
        << Connection::Classic
        << QStringLiteral("1000 OK: bareos-dir Version: 17.2.4 (21 Sep 2017)")
        << ServerKind::Bareos << QStringLiteral("17.2");

    QTest::newRow("onesimus greeting")
        << Connection::Classic
        << QStringLiteral("1000 OK: PDC-dir Version: 25.1.2+onesimus.0.2.0 (09 October 2026)")
        << ServerKind::OnesimusDirector << QStringLiteral("0.2.0");
    QTest::newRow("onesimus greeting, record separator")
        << Connection::Classic
        << QStringLiteral("1000\x1eOK: bareos-dir Version: 25.1.2+onesimus.0.2.0 (09 October 2026)")
        << ServerKind::OnesimusDirector << QStringLiteral("0.2.0");
    QTest::newRow("onesimus marker without own version")
        << Connection::Classic
        << QStringLiteral("1000 OK: d Version: 25.1.2+onesimus (09 October 2026)")
        << ServerKind::OnesimusDirector << QString();

    // The Director's name is free text and identifies nothing
    QTest::newRow("bareos director named like the product")
        << Connection::Classic
        << QStringLiteral("1000 OK: onesimus-dir Version: 25.2.1 (12 March 2026)")
        << ServerKind::Bareos << QStringLiteral("25.2");
    // An Onesimus Director that does not report the marker cannot be told
    // from Bareos on the console protocol: it is described as what it
    // reports, and treated as classic
    QTest::newRow("onesimus without marker reads as bareos")
        << Connection::Classic
        << QStringLiteral("1000 OK: PDC-dir Version: 25.1.2 (09 October 2026)")
        << ServerKind::Bareos << QStringLiteral("25.1");

    // --- Console protocol, nothing usable ---
    QTest::newRow("version not parsed (\"unknown\")")
        << Connection::Classic << QStringLiteral("unknown")
        << ServerKind::Unknown << QString();
    QTest::newRow("empty") << Connection::Classic << QString()
                           << ServerKind::Unknown << QString();
    QTest::newRow("greeting without version")
        << Connection::Classic << QStringLiteral("1000 OK auth")
        << ServerKind::Unknown << QString();
    QTest::newRow("unknown version format")
        << Connection::Classic
        << QStringLiteral("1000 OK: bareos-dir Version: next (01 January 2030)")
        << ServerKind::Unknown << QString();
    QTest::newRow("two-part version")
        << Connection::Classic << QStringLiteral("1000 OK: x Version: 25.2 (date)")
        << ServerKind::Unknown << QString();

    // --- gRPC, version of the login response ---
    QTest::newRow("grpc onesimus")
        << Connection::Grpc << QStringLiteral("25.1.2+onesimus.0.2.0")
        << ServerKind::OnesimusDirector << QStringLiteral("0.2.0");
    QTest::newRow("grpc, version without marker")
        << Connection::Grpc << QStringLiteral("25.1.2")
        << ServerKind::OnesimusDirector << QString();
    QTest::newRow("grpc, empty version")
        << Connection::Grpc << QString()
        << ServerKind::OnesimusDirector << QString();

    // --- No connection: nothing is known, whatever text is around ---
    QTest::newRow("not connected")
        << Connection::None << QString() << ServerKind::Unknown << QString();
    QTest::newRow("not connected, stale text")
        << Connection::None << QStringLiteral("25.1.2+onesimus.0.2.0")
        << ServerKind::Unknown << QString();
}

void BCapabilitiesTest::describeServer()
{
    QFETCH(Connection, connection);
    QFETCH(QString, identity);
    QFETCH(ServerKind, kind);
    QFETCH(QString, version);

    const Server server = BCapabilities::describeServer(connection, identity);
    QCOMPARE(server.kind, kind);
    QCOMPARE(server.version, version);
}

// ---------------------------------------------------------------------------
// Table
// ---------------------------------------------------------------------------

void BCapabilitiesTest::tableIsComplete()
{
    const QList<Function> functions = allFunctions();
    QCOMPARE(functions.size(), 11);

    QSet<QString> ids;
    for (Function function : functions) {
        const Entry *row = entry(function);
        QVERIFY(row);
        QCOMPARE(row->function, function);
        QVERIFY(row->id && row->id[0]);
        QVERIFY(row->name && row->name[0]);
        // Exactly one place for the Director in the reason
        QCOMPARE(QString::fromLatin1(row->reason).count(QStringLiteral("%1")), 1);
        ids.insert(QString::fromLatin1(row->id));
    }
    QCOMPARE(ids.size(), 11);

    // The identifiers are settings keys and must not change
    QCOMPARE(QString::fromLatin1(entry(Function::ModifySchedule)->id),
             QStringLiteral("schedule.modify"));
    QCOMPARE(QString::fromLatin1(entry(Function::PurgeAllVolumes)->id),
             QStringLiteral("volumes.purge_all"));
    QCOMPARE(QString::fromLatin1(entry(Function::DeployFileSet)->id),
             QStringLiteral("fileset.deploy"));
    QCOMPARE(QString::fromLatin1(entry(Function::DeleteSchedule)->id),
             QStringLiteral("schedule.delete"));
    QCOMPARE(QString::fromLatin1(entry(Function::CreateSchedule)->id),
             QStringLiteral("schedule.create"));
    QCOMPARE(QString::fromLatin1(entry(Function::ModifyJob)->id), QStringLiteral("job.modify"));
    QCOMPARE(QString::fromLatin1(entry(Function::DeleteJob)->id), QStringLiteral("job.delete"));
    QCOMPARE(QString::fromLatin1(entry(Function::CreateJob)->id), QStringLiteral("job.create"));
    QCOMPARE(QString::fromLatin1(entry(Function::ModifyJobDefs)->id), QStringLiteral("jobdefs.modify"));
    QCOMPARE(QString::fromLatin1(entry(Function::DeleteJobDefs)->id), QStringLiteral("jobdefs.delete"));
    QCOMPARE(QString::fromLatin1(entry(Function::CreateJobDefs)->id), QStringLiteral("jobdefs.create"));

    // What the two Directors provide
    QCOMPARE(entry(Function::ModifySchedule)->bareos, false);
    QCOMPARE(entry(Function::ModifySchedule)->onesimus, Support::Live);
    QCOMPARE(entry(Function::ModifySchedule)->liveOperation, BConfig::Operation::UpdateRaw);
    QCOMPARE(entry(Function::DeleteSchedule)->bareos, false);
    QCOMPARE(entry(Function::DeleteSchedule)->onesimus, Support::Live);
    QCOMPARE(entry(Function::DeleteSchedule)->liveOperation, BConfig::Operation::Delete);
    // "configure add" everywhere; CreateResource where it is listed
    QCOMPARE(entry(Function::CreateSchedule)->bareos, true);
    QCOMPARE(entry(Function::CreateSchedule)->onesimus, Support::Yes);
    QCOMPARE(entry(Function::CreateSchedule)->liveOperation, BConfig::Operation::CreateRaw);
    QCOMPARE(entry(Function::PurgeAllVolumes)->bareos, false);
    QCOMPARE(entry(Function::PurgeAllVolumes)->onesimus, Support::No);
    QCOMPARE(entry(Function::DeployFileSet)->bareos, false);
    QCOMPARE(entry(Function::DeployFileSet)->onesimus, Support::Planned);
    // Job definitions: change and delete over gRPC only, create everywhere
    for (Function function : {Function::ModifyJob, Function::DeleteJob, Function::ModifyJobDefs,
                              Function::DeleteJobDefs}) {
        QCOMPARE(entry(function)->bareos, false);
        QCOMPARE(entry(function)->onesimus, Support::Live);
    }
    QCOMPARE(entry(Function::ModifyJob)->liveType, BConfig::ResourceType::Job);
    QCOMPARE(entry(Function::ModifyJobDefs)->liveType, BConfig::ResourceType::JobDefs);
    QCOMPARE(entry(Function::DeleteJobDefs)->liveOperation, BConfig::Operation::Delete);
    QCOMPARE(entry(Function::CreateJob)->bareos, true);
    QCOMPARE(entry(Function::CreateJobDefs)->liveOperation, BConfig::Operation::CreateRaw);
}

void BCapabilitiesTest::lookup_data()
{
    QTest::addColumn<Function>("function");

    QTest::newRow("change schedule") << Function::ModifySchedule;
    QTest::newRow("purge all volumes") << Function::PurgeAllVolumes;
    QTest::newRow("deploy fileset") << Function::DeployFileSet;
    QTest::newRow("delete schedule") << Function::DeleteSchedule;
}

void BCapabilitiesTest::lookup()
{
    QFETCH(Function, function);

    QCOMPARE(bareos().kind, ServerKind::Bareos);
    QCOMPARE(onesimus().kind, ServerKind::OnesimusDirector);
    QCOMPARE(unidentified().kind, ServerKind::Unknown);

    // None is available without the capabilities of a gRPC session:
    // planned is not yes, and live needs the Director's word
    QVERIFY(!isAvailable(function, bareos()));
    QVERIFY(!isAvailable(function, onesimus()));
    QVERIFY(!isAvailable(function, unidentified()));
    QVERIFY(!isAvailable(function, Server()));

    // An unidentified server gets the answer of classic Bareos
    QCOMPARE(isAvailable(function, unidentified()), isAvailable(function, bareos()));
}

void BCapabilitiesTest::unlistedServerKindsFollowTheColumns()
{
    // Onesimus only
    Entry onesimusOnly = sample(Support::Yes, nullptr);
    QVERIFY(!isAvailable(onesimusOnly, bareos()));
    QVERIFY(!isAvailable(onesimusOnly, unidentified()));
    QVERIFY(isAvailable(onesimusOnly, onesimus()));

    // Bareos only: an unidentified server is treated as classic
    Entry bareosOnly = sample(Support::No, nullptr);
    bareosOnly.bareos = true;
    QVERIFY(isAvailable(bareosOnly, bareos()));
    QVERIFY(isAvailable(bareosOnly, unidentified()));
    QVERIFY(!isAvailable(bareosOnly, onesimus()));

    // Planned is not available
    QVERIFY(!isAvailable(sample(Support::Planned, nullptr), onesimus()));
}

void BCapabilitiesTest::liveCapabilitiesDecideOverGrpc()
{
    BConfig::Capabilities crud;
    crud.known = true;
    crud.interfaceVersion = 1;
    crud.operations[int(BConfig::ResourceType::Schedule)] = {
        int(BConfig::Operation::ReadRaw), int(BConfig::Operation::CreateRaw),
        int(BConfig::Operation::UpdateRaw), int(BConfig::Operation::Delete)};

    const Server grpc = BCapabilities::describeServer(Connection::Grpc,
                                                      QStringLiteral("25.1.2+onesimus.0.3.0"), crud);
    QVERIFY(isAvailable(Function::ModifySchedule, grpc));
    QVERIFY(isAvailable(Function::DeleteSchedule, grpc));
    QVERIFY(usesConfigMethod(Function::ModifySchedule, grpc));
    QVERIFY(usesConfigMethod(Function::CreateSchedule, grpc));

    // A session without the Schedule ACL: the type is not listed
    BConfig::Capabilities jobsOnly = crud;
    jobsOnly.operations.remove(int(BConfig::ResourceType::Schedule));
    jobsOnly.operations[int(BConfig::ResourceType::Job)] = {int(BConfig::Operation::UpdateRaw)};
    const Server noSchedule = BCapabilities::describeServer(Connection::Grpc, QString(), jobsOnly);
    QVERIFY(!isAvailable(Function::ModifySchedule, noSchedule));
    QVERIFY(!usesConfigMethod(Function::CreateSchedule, noSchedule));
    // ... creating still works, with "configure add"
    QVERIFY(isAvailable(Function::CreateSchedule, noSchedule));
    QVERIFY(noticeOf(Function::ModifySchedule, noSchedule).contains(QStringLiteral("\"configure\"")));
    // Job definitions follow the types the session lists: Job yes, JobDefs no
    QVERIFY(isAvailable(Function::ModifyJob, noSchedule));
    QVERIFY(!isAvailable(Function::DeleteJob, noSchedule));
    QVERIFY(!isAvailable(Function::ModifyJobDefs, noSchedule));
    QVERIFY(!usesConfigMethod(Function::CreateJob, noSchedule));
    QVERIFY(isAvailable(Function::CreateJobDefs, noSchedule));   // the wizard, "configure add"
    QVERIFY(!isAvailable(Function::ModifyJob, BCapabilities::describeServer(
                             Connection::Classic, QStringLiteral("25.2.1"), jobsOnly)));
    QVERIFY(!isAvailable(Function::ModifyJobDefs, BCapabilities::describeServer(
                             Connection::Classic, QStringLiteral("25.1.2+onesimus.0.3.0"), crud)));

    // Update listed, delete not
    BConfig::Capabilities updateOnly;
    updateOnly.known = true;
    updateOnly.operations[int(BConfig::ResourceType::Schedule)] = {int(BConfig::Operation::UpdateRaw)};
    const Server partial = BCapabilities::describeServer(Connection::Grpc, QString(), updateOnly);
    QVERIFY(isAvailable(Function::ModifySchedule, partial));
    QVERIFY(!isAvailable(Function::DeleteSchedule, partial));

    // The capabilities count over gRPC only; BNET never, not even with them
    const Server bnet = BCapabilities::describeServer(Connection::Classic,
                                                      QStringLiteral("1000 OK: d Version: 25.1.2+onesimus.0.3.0 (x)"), crud);
    QCOMPARE(bnet.kind, ServerKind::OnesimusDirector);
    QVERIFY(!bnet.config.known);
    QVERIFY(!isAvailable(Function::ModifySchedule, bnet));
    QVERIFY(!isAvailable(Function::DeleteSchedule, bnet));
    QVERIFY(!usesConfigMethod(Function::CreateSchedule, bnet));
    QVERIFY(isAvailable(Function::CreateSchedule, bnet));
    QVERIFY(noticeOf(Function::ModifySchedule, bnet).endsWith(QStringLiteral("Connect to it over gRPC to use this.")));
    QVERIFY(!isAvailable(Function::ModifySchedule, BCapabilities::describeServer(Connection::Classic, QStringLiteral("25.2.1"), crud)));

    // Not known yet (before the answer): nothing
    QVERIFY(!isAvailable(Function::ModifySchedule, BCapabilities::describeServer(Connection::Grpc, QString())));
}

// ---------------------------------------------------------------------------
// Notice text
// ---------------------------------------------------------------------------

void BCapabilitiesTest::serverLabels()
{
    QCOMPARE(serverLabel(bareos()), QStringLiteral("This Director (Bareos 25.2)"));
    QCOMPARE(serverLabel(onesimus()), QStringLiteral("This Director (Onesimus Director 0.2.0)"));
    QCOMPARE(serverLabel(BCapabilities::describeServer(Connection::Grpc, QString())),
             QStringLiteral("This Director (Onesimus Director)"));
    // No product is named for a server that was not identified
    QCOMPARE(serverLabel(unidentified()), QStringLiteral("This Director"));
    QCOMPARE(serverLabel(Server()), QStringLiteral("This Director"));
}

void BCapabilitiesTest::noticeTextOfTheTable()
{
    const QString copyText = QStringLiteral(
        " You can copy the configuration text and place it on the Director yourself.");
    const QString plannedElsewhere =
        QStringLiteral(" The Onesimus Director is planned to support this.");
    const QString plannedHere = QStringLiteral(
        " Support for this is planned for a later version of the Onesimus Director.");

    // --- Change an existing schedule ---
    // The answer to a bar dropped in the schedule timeline, on every
    // connection: it needs the gRPC interface, and nothing was changed.
    // No word about copying a configuration text: no such text is offered.
    QString reason = QStringLiteral(" cannot change an existing schedule on this connection: that "
                                    "needs the configuration methods of the gRPC interface of "
                                    "the Onesimus Director (onesimus-dird).");
    const QString nothingChanged = QStringLiteral(
        " Nothing was changed. A schedule can also be changed in its configuration file on "
        "the Director.");
    const QString overGrpc = QStringLiteral(" The Onesimus Director supports this over its gRPC interface.");
    QCOMPARE(notice(Function::ModifySchedule, bareos()).title,
             QStringLiteral("Change an existing schedule"));
    QCOMPARE(notice(Function::ModifySchedule, bareos()).text,
             QStringLiteral("This Director (Bareos 25.2)") + reason + nothingChanged + overGrpc);
    QCOMPARE(notice(Function::ModifySchedule, unidentified()).text,
             QStringLiteral("This Director") + reason + nothingChanged + overGrpc);
    // An Onesimus Director over gRPC whose capabilities are not known (or
    // do not list the method): this session is not offered it
    QCOMPARE(notice(Function::ModifySchedule, onesimus()).text,
             QStringLiteral("This Director (Onesimus Director 0.2.0) does not offer this to this "
                            "session: its configuration capabilities do not list it. The console "
                            "needs the command ACL \"configure\" and the ACL of the resource type, "
                            "or the Director is older than its configuration methods. "
                            "The capabilities were not received for this session.")
                 + nothingChanged);
    // Over the console protocol: gRPC is the way
    const Server onesimusBnet = BCapabilities::describeServer(
        Connection::Classic, QStringLiteral("1000 OK: PDC-dir Version: 25.1.2+onesimus.0.2.0 (x)"));
    QCOMPARE(notice(Function::ModifySchedule, onesimusBnet).text,
             QStringLiteral("This Director (Onesimus Director 0.2.0)") + reason + nothingChanged
                 + QStringLiteral(" Connect to it over gRPC to use this."));
    // Not available without capabilities that list it
    QVERIFY(!isAvailable(Function::ModifySchedule, bareos()));
    QVERIFY(!isAvailable(Function::ModifySchedule, onesimus()));
    QVERIFY(!isAvailable(Function::ModifySchedule, BCapabilities::describeServer(Connection::Grpc, QString())));

    // --- Prune and purge: no word about the Onesimus Director ---
    reason = QStringLiteral(" has no command that purges all volumes at once; purge works on "
                            "named volumes only and ignores retention periods.");
    const QString pruneText = QStringLiteral(
        " \"Prune only\" applies the retention period of every volume; a single volume can be "
        "purged by its name with the purge command.");
    QCOMPARE(notice(Function::PurgeAllVolumes, bareos()).title,
             QStringLiteral("Prune and purge all volumes"));
    QCOMPARE(notice(Function::PurgeAllVolumes, bareos()).text,
             QStringLiteral("This Director (Bareos 25.2)") + reason + pruneText);
    QCOMPARE(notice(Function::PurgeAllVolumes, onesimus()).text,
             QStringLiteral("This Director (Onesimus Director 0.2.0)") + reason + pruneText);
    QCOMPARE(notice(Function::PurgeAllVolumes, unidentified()).text,
             QStringLiteral("This Director") + reason + pruneText);

    // --- FileSet wizard ---
    reason = QStringLiteral(" cannot take a FileSet through the console, because its configure "
                            "command does not accept nested blocks (Include, Options, Exclude).");
    QCOMPARE(notice(Function::DeployFileSet, bareos()).title,
             QStringLiteral("Send a FileSet to the Director"));
    QCOMPARE(notice(Function::DeployFileSet, bareos()).text,
             QStringLiteral("This Director (Bareos 25.2)") + reason + copyText + plannedElsewhere);
    QCOMPARE(notice(Function::DeployFileSet, onesimus()).text,
             QStringLiteral("This Director (Onesimus Director 0.2.0)") + reason + copyText
                 + plannedHere);
    QCOMPARE(notice(Function::DeployFileSet, unidentified()).text,
             QStringLiteral("This Director") + reason + copyText + plannedElsewhere);
}

void BCapabilitiesTest::noticeText_data()
{
    QTest::addColumn<int>("support");
    QTest::addColumn<bool>("withManualWay");
    QTest::addColumn<bool>("onOnesimus");
    QTest::addColumn<QString>("expected");

    const QString onBareos = QStringLiteral("This Director (Bareos 25.2) cannot do the sample.");
    const QString onOnesimus =
        QStringLiteral("This Director (Onesimus Director 0.2.0) cannot do the sample.");
    const QString manual = QStringLiteral(" Do it by hand.");

    QTest::newRow("yes, manual way")
        << int(Support::Yes) << true << false
        << onBareos + manual + QStringLiteral(" The Onesimus Director supports this.");
    QTest::newRow("yes, no manual way")
        << int(Support::Yes) << false << false
        << onBareos + QStringLiteral(" The Onesimus Director supports this.");
    QTest::newRow("planned, manual way")
        << int(Support::Planned) << true << false
        << onBareos + manual + QStringLiteral(" The Onesimus Director is planned to support this.");
    QTest::newRow("planned, no manual way")
        << int(Support::Planned) << false << false
        << onBareos + QStringLiteral(" The Onesimus Director is planned to support this.");
    QTest::newRow("no, manual way") << int(Support::No) << true << false << onBareos + manual;
    QTest::newRow("no, no manual way") << int(Support::No) << false << false << onBareos;

    QTest::newRow("planned, on the onesimus director")
        << int(Support::Planned) << true << true
        << onOnesimus + manual
               + QStringLiteral(" Support for this is planned for a later version of the "
                                "Onesimus Director.");
    QTest::newRow("no, on the onesimus director")
        << int(Support::No) << false << true << onOnesimus;
}

void BCapabilitiesTest::noticeText()
{
    QFETCH(int, support);
    QFETCH(bool, withManualWay);
    QFETCH(bool, onOnesimus);
    QFETCH(QString, expected);

    const Entry row = sample(Support(support), withManualWay ? ManualWay : nullptr);
    const Notice result = notice(row, onOnesimus ? onesimus() : bareos());
    QCOMPARE(result.title, QStringLiteral("Sample function"));
    QCOMPARE(result.text, expected);
}

void BCapabilitiesTest::noticeNeverClaimsMoreThanTheTable()
{
    const QList<Server> servers = {bareos(), onesimus(), unidentified()};
    for (Function function : allFunctions()) {
        const Entry *row = entry(function);
        for (const Server &server : servers) {
            const QString text = notice(function, server).text;
            // "supports this" only for Support::Yes and Live, and only
            // about the Onesimus Director when it is not the one connected
            QCOMPARE(text.contains(QStringLiteral("supports this")),
                     (row->onesimus == Support::Yes || row->onesimus == Support::Live)
                         && server.kind != ServerKind::OnesimusDirector);
            QCOMPARE(text.contains(QStringLiteral("planned")), row->onesimus == Support::Planned);
            if (row->onesimus == Support::No) {
                // Nothing is said about another product
                QCOMPARE(text.count(QStringLiteral("Onesimus Director")),
                         server.kind == ServerKind::OnesimusDirector ? 1 : 0);
            }
            // An unidentified server is not called Bareos
            if (server.kind == ServerKind::Unknown) {
                QVERIFY(!text.contains(QStringLiteral("Bareos")));
            }
        }
    }
}

// ---------------------------------------------------------------------------
// When a notice is shown
// ---------------------------------------------------------------------------

void BCapabilitiesTest::implicitRequestOncePerSession()
{
    const Entry &row = *entry(Function::DeployFileSet);
    NoticePolicy policy;

    QVERIFY(!policy.shownThisSession(row));
    QVERIFY(policy.shouldShow(row, Request::Implicit));
    // Asking does not count; showing does
    QVERIFY(policy.shouldShow(row, Request::Implicit));

    policy.recordShown(row);
    QVERIFY(policy.shownThisSession(row));
    QVERIFY(!policy.shouldShow(row, Request::Implicit));

    // A new session starts with a new policy
    NoticePolicy nextSession;
    QVERIFY(nextSession.shouldShow(row, Request::Implicit));
}

void BCapabilitiesTest::explicitRequestEveryTime()
{
    const Entry &row = *entry(Function::PurgeAllVolumes);
    NoticePolicy policy;

    QVERIFY(policy.shouldShow(row, Request::Explicit));
    policy.recordShown(row);
    // The user asks again with the same click and gets the answer again
    QVERIFY(policy.shouldShow(row, Request::Explicit));
    // ... but nothing appears on its own any more
    QVERIFY(!policy.shouldShow(row, Request::Implicit));
}

void BCapabilitiesTest::doNotShowAgain()
{
    QSet<QString> hidden;
    QStringList asked;
    NoticePolicy policy([&](const QString &id) {
        asked.append(id);
        return hidden.contains(id);
    });

    const Entry &purge = *entry(Function::PurgeAllVolumes);
    const Entry &fileSet = *entry(Function::DeployFileSet);

    // Default: nothing is hidden
    QVERIFY(policy.shouldShow(purge, Request::Explicit));
    QCOMPARE(asked, QStringList{QStringLiteral("volumes.purge_all")});

    // Ticked for one function: neither kind of request shows it again
    hidden.insert(QStringLiteral("volumes.purge_all"));
    QVERIFY(!policy.shouldShow(purge, Request::Explicit));
    QVERIFY(!policy.shouldShow(purge, Request::Implicit));
    // The other function is not affected
    QVERIFY(policy.shouldShow(fileSet, Request::Implicit));
    QVERIFY(policy.shouldShow(fileSet, Request::Explicit));

    // The setting is read every time, so clearing it brings the notice back
    hidden.clear();
    QVERIFY(policy.shouldShow(purge, Request::Explicit));
}

void BCapabilitiesTest::policyIsPerFunction()
{
    NoticePolicy policy;
    policy.recordShown(*entry(Function::ModifySchedule));

    QVERIFY(!policy.shouldShow(*entry(Function::ModifySchedule), Request::Implicit));
    QVERIFY(policy.shouldShow(*entry(Function::DeployFileSet), Request::Implicit));
    QVERIFY(policy.shouldShow(*entry(Function::PurgeAllVolumes), Request::Implicit));
}

QTEST_APPLESS_MAIN(BCapabilitiesTest)
#include "bcapabilities_test.moc"
