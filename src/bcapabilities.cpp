// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "bcapabilities.h"

#include <QCoreApplication>
#include <QRegularExpression>
#include <QStringList>

namespace BCapabilities {

namespace {

const char Context[] = "BCapabilities";

// The capability table. Sources for the columns:
// - Bareos 25.2: `configure` has the subcommands add and export only, and
//   `configure add` takes flat directives (no Include/Options/Exclude);
//   `purge` works on named jobs, clients and volumes.
// - Onesimus Director: `configure update` covers client and job; the typed
//   configure commands take a flat map of directives; besides the Bareos
//   forms of `purge` it has "purge volume all pool=<p>|allpools
//   [volstatus=...] dryrun|yes" (odir 69cd11b), on both protocols.
// - Changing and deleting a schedule go through the configuration methods
//   of the gRPC interface DirectorApi of the Onesimus Director
//   (UpdateResource, DeleteResource; onesimus-dird
//   docs/plans/CONFIG-CRUD-GRPC.md). Whether a session may use them is what
//   GetConfigCapabilities says after the login (Support::Live). The console
//   protocol never can, and over it the GUI sends nothing: the notice says
//   that nothing was changed.
// - Creating a schedule works everywhere with "configure add"; where the
//   capabilities list CreateResource, that is taken instead.
const Entry Table[] = {
    {
        Function::ModifySchedule,
        "schedule.modify",
        QT_TRANSLATE_NOOP("BCapabilities", "Change an existing schedule"),
        false,
        Support::Live,
        QT_TRANSLATE_NOOP("BCapabilities",
                          "%1 cannot change an existing schedule on this connection: that "
                          "needs the configuration methods of the gRPC interface of the Onesimus "
                          "Director (onesimus-dird)."),
        QT_TRANSLATE_NOOP("BCapabilities",
                          "Nothing was changed. A schedule can also be changed in its "
                          "configuration file on the Director."),
        BConfig::ResourceType::Schedule,
        BConfig::Operation::UpdateRaw,
    },
    {
        Function::PurgeAllVolumes,
        "volumes.purge_all",
        QT_TRANSLATE_NOOP("BCapabilities", "Prune and purge all volumes"),
        false,
        Support::Yes,
        QT_TRANSLATE_NOOP("BCapabilities",
                          "%1 has no command that purges all volumes at once; purge works on "
                          "named volumes only and ignores retention periods."),
        QT_TRANSLATE_NOOP("BCapabilities",
                          "\"Prune only\" applies the retention period of every volume; a "
                          "single volume can be purged by its name with the purge command."),
    },
    {
        Function::DeployFileSet,
        "fileset.deploy",
        QT_TRANSLATE_NOOP("BCapabilities", "Send a FileSet to the Director"),
        false,
        Support::Planned,
        QT_TRANSLATE_NOOP("BCapabilities",
                          "%1 cannot take a FileSet through the console, because its configure "
                          "command does not accept nested blocks (Include, Options, Exclude)."),
        QT_TRANSLATE_NOOP("BCapabilities",
                          "You can copy the configuration text and place it on the Director "
                          "yourself."),
    },
    {
        Function::DeleteSchedule,
        "schedule.delete",
        QT_TRANSLATE_NOOP("BCapabilities", "Delete a schedule"),
        false,
        Support::Live,
        QT_TRANSLATE_NOOP("BCapabilities",
                          "%1 cannot delete a schedule on this connection: that needs the "
                          "configuration methods of the gRPC interface of the Onesimus Director "
                          "(onesimus-dird)."),
        QT_TRANSLATE_NOOP("BCapabilities",
                          "Nothing was deleted. A schedule can also be removed from the "
                          "configuration files on the Director."),
        BConfig::ResourceType::Schedule,
        BConfig::Operation::Delete,
    },
    {
        Function::CreateSchedule,
        "schedule.create",
        QT_TRANSLATE_NOOP("BCapabilities", "Create a schedule"),
        true,
        Support::Yes,
        QT_TRANSLATE_NOOP("BCapabilities", "%1 cannot create a schedule."),
        nullptr,
        BConfig::ResourceType::Schedule,
        BConfig::Operation::CreateRaw,
    },
    // Job definitions (backlog P3-18): the same as schedules. Changing and
    // deleting need the configuration methods over gRPC; creating works
    // everywhere with "configure add" (the job wizard), and through
    // CreateResource where the session lists it.
    {
        Function::ModifyJob,
        "job.modify",
        QT_TRANSLATE_NOOP("BCapabilities", "Change an existing job"),
        false,
        Support::Live,
        QT_TRANSLATE_NOOP("BCapabilities",
                          "%1 cannot change an existing job on this connection: that needs the "
                          "configuration methods of the gRPC interface of the Onesimus Director "
                          "(onesimus-dird)."),
        QT_TRANSLATE_NOOP("BCapabilities",
                          "Nothing was changed. A job can also be changed in its configuration "
                          "file on the Director."),
        BConfig::ResourceType::Job,
        BConfig::Operation::UpdateRaw,
    },
    {
        Function::DeleteJob,
        "job.delete",
        QT_TRANSLATE_NOOP("BCapabilities", "Delete a job"),
        false,
        Support::Live,
        QT_TRANSLATE_NOOP("BCapabilities",
                          "%1 cannot delete a job on this connection: that needs the configuration "
                          "methods of the gRPC interface of the Onesimus Director (onesimus-dird)."),
        QT_TRANSLATE_NOOP("BCapabilities",
                          "Nothing was deleted. A job can also be removed from the configuration "
                          "files on the Director."),
        BConfig::ResourceType::Job,
        BConfig::Operation::Delete,
    },
    {
        Function::CreateJob,
        "job.create",
        QT_TRANSLATE_NOOP("BCapabilities", "Create a job"),
        true,
        Support::Yes,
        QT_TRANSLATE_NOOP("BCapabilities", "%1 cannot create a job."),
        nullptr,
        BConfig::ResourceType::Job,
        BConfig::Operation::CreateRaw,
    },
    {
        Function::ModifyJobDefs,
        "jobdefs.modify",
        QT_TRANSLATE_NOOP("BCapabilities", "Change an existing JobDefs"),
        false,
        Support::Live,
        QT_TRANSLATE_NOOP("BCapabilities",
                          "%1 cannot change an existing JobDefs on this connection: that needs the "
                          "configuration methods of the gRPC interface of the Onesimus Director "
                          "(onesimus-dird)."),
        QT_TRANSLATE_NOOP("BCapabilities",
                          "Nothing was changed. A JobDefs can also be changed in its configuration "
                          "file on the Director."),
        BConfig::ResourceType::JobDefs,
        BConfig::Operation::UpdateRaw,
    },
    {
        Function::DeleteJobDefs,
        "jobdefs.delete",
        QT_TRANSLATE_NOOP("BCapabilities", "Delete a JobDefs"),
        false,
        Support::Live,
        QT_TRANSLATE_NOOP("BCapabilities",
                          "%1 cannot delete a JobDefs on this connection: that needs the "
                          "configuration methods of the gRPC interface of the Onesimus Director "
                          "(onesimus-dird)."),
        QT_TRANSLATE_NOOP("BCapabilities",
                          "Nothing was deleted. A JobDefs can also be removed from the configuration "
                          "files on the Director."),
        BConfig::ResourceType::JobDefs,
        BConfig::Operation::Delete,
    },
    {
        Function::CreateJobDefs,
        "jobdefs.create",
        QT_TRANSLATE_NOOP("BCapabilities", "Create a JobDefs"),
        true,
        Support::Yes,
        QT_TRANSLATE_NOOP("BCapabilities", "%1 cannot create a JobDefs."),
        nullptr,
        BConfig::ResourceType::JobDefs,
        BConfig::Operation::CreateRaw,
    },
    // "Check now" (issue #9, backlog P3-12): the Onesimus Director answers
    // the typed GetClientStatus, which has its own reply. Over the console
    // protocol "status client" in API mode 2 does not say whether the file
    // daemon answered: Bareos collects the text as JSON messages, which it
    // does not send (dird/ua_status.cc DoNativeClientStatus() with
    // UaContext::SendMsg(), lib/output_formatter.cc; bareos/bareos#2325),
    // and replies are matched by queue position only (backlog P3-1).
    {
        Function::ProbeClient,
        "client.probe",
        QT_TRANSLATE_NOOP("BCapabilities", "Check whether a client is reachable"),
        false,
        Support::Grpc,
        QT_TRANSLATE_NOOP("BCapabilities",
                          "%1 cannot check a client on this connection: in the API mode the GUI "
                          "uses, the reply of \"status client\" does not say whether the client "
                          "answered."),
        QT_TRANSLATE_NOOP("BCapabilities",
                          "Nothing was sent. You can run \"status client=<name>\" in a console "
                          "(bconsole) and read the reply."),
    },
};

QString translated(const char *text)
{
    return QCoreApplication::translate(Context, text);
}

} // namespace

const Entry *entry(Function function)
{
    for (const Entry &row : Table) {
        if (row.function == function) {
            return &row;
        }
    }
    return nullptr;
}

QList<Function> allFunctions()
{
    QList<Function> functions;
    for (const Entry &row : Table) {
        functions.append(row.function);
    }
    return functions;
}

Server describeServer(Connection connection, const QString &identity,
                      const BConfig::Capabilities &config)
{
    Server server;
    if (connection == Connection::None) {
        return server;
    }
    server.connection = connection;
    if (connection == Connection::Grpc) {
        server.config = config;
    }

    // The greeting separates its fields with a record separator or with the
    // Bareos space encoding (\x01), depending on the console protocol version
    QString text = identity;
    text.replace(QChar(0x1e), QLatin1Char(' ')).replace(QChar(0x01), QLatin1Char(' '));
    text = text.trimmed();

    // "1000 OK: <director name> Version: <token> (<date>)". The Director name
    // is free text, so only the token after "Version:" is looked at.
    static const QRegularExpression greetingRx(QStringLiteral("Version:\\s*(\\S+)"),
                                               QRegularExpression::CaseInsensitiveOption);
    QString token = text;
    const QRegularExpressionMatch greeting = greetingRx.match(text);
    if (greeting.hasMatch()) {
        token = greeting.captured(1);
    }

    static const QRegularExpression tokenRx(QStringLiteral("^(\\d+)\\.(\\d+)\\.(\\d+)(\\S*)$"));
    const QRegularExpressionMatch version = tokenRx.match(token);

    // Onesimus Director builds report "<Bareos version>+onesimus.<own version>"
    bool onesimusMarker = false;
    QString ownVersion;
    if (version.hasMatch()) {
        static const QRegularExpression markerRx(
            QStringLiteral("^\\+onesimus(?:\\.(\\d+(?:\\.\\d+)*))?"),
            QRegularExpression::CaseInsensitiveOption);
        const QRegularExpressionMatch marker = markerRx.match(version.captured(4));
        onesimusMarker = marker.hasMatch();
        ownVersion = marker.captured(1);
    }

    if (connection == Connection::Grpc || onesimusMarker) {
        server.kind = ServerKind::OnesimusDirector;
        server.version = ownVersion;
        return server;
    }

    if (version.hasMatch()) {
        server.kind = ServerKind::Bareos;
        server.version = version.captured(1) + QLatin1Char('.') + version.captured(2);
    }
    return server;
}

bool usesConfigMethod(const Entry &entry, const Server &server)
{
    return entry.liveOperation != BConfig::Operation::Unspecified
        && server.kind == ServerKind::OnesimusDirector && server.connection == Connection::Grpc
        && server.config.supports(entry.liveType, entry.liveOperation);
}

bool usesConfigMethod(Function function, const Server &server)
{
    const Entry *row = entry(function);
    return row && usesConfigMethod(*row, server);
}

bool isAvailable(const Entry &entry, const Server &server)
{
    if (server.kind == ServerKind::OnesimusDirector) {
        if (entry.onesimus == Support::Live) {
            return usesConfigMethod(entry, server);
        }
        if (entry.onesimus == Support::Grpc) {
            return server.connection == Connection::Grpc;
        }
        return entry.onesimus == Support::Yes;
    }
    // Bareos, and every server that was not identified
    return entry.bareos;
}

bool isAvailable(Function function, const Server &server)
{
    const Entry *row = entry(function);
    // A function that is not listed is not restricted
    return row ? isAvailable(*row, server) : true;
}

QString serverLabel(const Server &server)
{
    switch (server.kind) {
    case ServerKind::Bareos:
        return server.version.isEmpty()
                   ? translated(QT_TRANSLATE_NOOP("BCapabilities", "This Director (Bareos)"))
                   : translated(QT_TRANSLATE_NOOP("BCapabilities", "This Director (Bareos %1)"))
                         .arg(server.version);
    case ServerKind::OnesimusDirector:
        return server.version.isEmpty()
                   ? translated(QT_TRANSLATE_NOOP("BCapabilities",
                                                  "This Director (Onesimus Director)"))
                   : translated(QT_TRANSLATE_NOOP("BCapabilities",
                                                  "This Director (Onesimus Director %1)"))
                         .arg(server.version);
    case ServerKind::Unknown:
        break;
    }
    return translated(QT_TRANSLATE_NOOP("BCapabilities", "This Director"));
}

Notice notice(const Entry &entry, const Server &server)
{
    Notice result;
    result.title = translated(entry.name);

    QStringList sentences;
    const bool onOnesimus = (server.kind == ServerKind::OnesimusDirector);
    if (entry.onesimus == Support::Live && onOnesimus && server.connection == Connection::Grpc) {
        // The interface is there; this session is not offered the method
        sentences.append(translated(QT_TRANSLATE_NOOP(
                             "BCapabilities",
                             "%1 does not offer this to this session: its configuration "
                             "capabilities do not list it. The console needs the command ACL "
                             "\"configure\" and the ACL of the resource type, or the Director is "
                             "older than its configuration methods."))
                             .arg(serverLabel(server)));
        // What the query really returned, so the cause can be told apart
        if (!server.config.error.isEmpty()) {
            sentences.append(translated(QT_TRANSLATE_NOOP("BCapabilities",
                                 "The capabilities query failed: %1."))
                                 .arg(server.config.error));
        } else if (!server.config.known) {
            sentences.append(translated(QT_TRANSLATE_NOOP("BCapabilities",
                                 "The capabilities were not received for this session.")));
        } else {
            QStringList types;
            for (auto it = server.config.operations.constBegin();
                 it != server.config.operations.constEnd(); ++it) {
                types.append(BConfig::typeName(BConfig::ResourceType(it.key())));
            }
            types.sort();
            sentences.append(translated(QT_TRANSLATE_NOOP("BCapabilities",
                                 "The Director lists for this session: %1."))
                                 .arg(types.isEmpty() ? QStringLiteral("-") : types.join(QStringLiteral(", "))));
        }
    } else {
        sentences.append(translated(entry.reason).arg(serverLabel(server)));
    }

    if (entry.manualWay && entry.manualWay[0] != '\0') {
        sentences.append(translated(entry.manualWay));
    }

    // What the Onesimus Director offers: only what the table says
    switch (entry.onesimus) {
    case Support::Live:
    case Support::Grpc:
        if (!onOnesimus) {
            sentences.append(translated(QT_TRANSLATE_NOOP(
                "BCapabilities", "The Onesimus Director supports this over its gRPC interface.")));
        } else if (server.connection != Connection::Grpc) {
            sentences.append(translated(QT_TRANSLATE_NOOP(
                "BCapabilities", "Connect to it over gRPC to use this.")));
        }
        break;
    case Support::Yes:
        if (!onOnesimus) {
            sentences.append(translated(
                QT_TRANSLATE_NOOP("BCapabilities", "The Onesimus Director supports this.")));
        }
        break;
    case Support::Planned:
        sentences.append(onOnesimus
            ? translated(QT_TRANSLATE_NOOP("BCapabilities",
                                           "Support for this is planned for a later version of "
                                           "the Onesimus Director."))
            : translated(QT_TRANSLATE_NOOP("BCapabilities",
                                           "The Onesimus Director is planned to support this.")));
        break;
    case Support::No:
        break;
    }

    result.text = sentences.join(QLatin1Char(' '));
    return result;
}

Notice notice(Function function, const Server &server)
{
    const Entry *row = entry(function);
    return row ? notice(*row, server) : Notice();
}

NoticePolicy::NoticePolicy(SuppressedLookup suppressed)
    : m_suppressed(std::move(suppressed))
{
}

bool NoticePolicy::shouldShow(const Entry &entry, Request request) const
{
    const QString id = QString::fromLatin1(entry.id);
    if (m_suppressed && m_suppressed(id)) {
        return false;
    }
    if (request == Request::Explicit) {
        return true;
    }
    return !m_shown.contains(id);
}

void NoticePolicy::recordShown(const Entry &entry)
{
    m_shown.insert(QString::fromLatin1(entry.id));
}

bool NoticePolicy::shownThisSession(const Entry &entry) const
{
    return m_shown.contains(QString::fromLatin1(entry.id));
}

} // namespace BCapabilities
