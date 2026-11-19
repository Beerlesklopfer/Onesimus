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
//   configure commands take a flat map of directives; `purge` follows the
//   Bareos command (no form for all volumes).
// - Changing a schedule is to come through methods of the gRPC interface
//   of the Onesimus Director (onesimus-dird) that do not exist yet. Until
//   then no connection can do it, the console protocol never will, and the
//   GUI sends nothing: the notice says that nothing was changed.
const Entry Table[] = {
    {
        Function::ModifySchedule,
        "schedule.modify",
        QT_TRANSLATE_NOOP("BCapabilities", "Change an existing schedule"),
        false,
        Support::Planned,
        QT_TRANSLATE_NOOP("BCapabilities",
                          "%1 cannot change an existing schedule on this connection: that "
                          "needs the schedule methods of the gRPC interface of the Onesimus "
                          "Director (onesimus-dird)."),
        QT_TRANSLATE_NOOP("BCapabilities",
                          "Nothing was changed. For now a schedule is changed in its "
                          "configuration file on the Director."),
    },
    {
        Function::PurgeAllVolumes,
        "volumes.purge_all",
        QT_TRANSLATE_NOOP("BCapabilities", "Prune and purge all volumes"),
        false,
        Support::No,
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

Server describeServer(Connection connection, const QString &identity)
{
    Server server;
    if (connection == Connection::None) {
        return server;
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

bool isAvailable(const Entry &entry, const Server &server)
{
    if (server.kind == ServerKind::OnesimusDirector) {
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
    sentences.append(translated(entry.reason).arg(serverLabel(server)));

    if (entry.manualWay && entry.manualWay[0] != '\0') {
        sentences.append(translated(entry.manualWay));
    }

    // What the Onesimus Director offers: only what the table says
    const bool onOnesimus = (server.kind == ServerKind::OnesimusDirector);
    switch (entry.onesimus) {
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
