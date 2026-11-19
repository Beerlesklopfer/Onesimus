#ifndef BCAPABILITIES_H
#define BCAPABILITIES_H

#include <QList>
#include <QSet>
#include <QString>

#include <functional>

/**
 * @brief Which functions the connected Director provides, without widgets
 *
 * Onesimus talks to classic Bareos and to the Onesimus Director. Some
 * functions the GUI offers cannot be carried out by the Director it is
 * connected to. The table in bcapabilities.cpp lists them; this header
 * answers for a server description whether a function is available and
 * produces the text of the notice the user gets when asking for it.
 *
 * Everything here is QtCore only (unit-tested in bcapabilities_test). The
 * message box lives in bcapabilitynotice.h.
 *
 * Backward compatibility: a server that cannot be identified is treated as
 * classic Bareos and the notice names no product for it.
 */
namespace BCapabilities {

/** Functions listed in the capability table. */
enum class Function {
    ModifySchedule,     ///< Write a change of an existing Schedule resource (a dropped bar)
    PurgeAllVolumes,    ///< "Prune and purge" over all volumes (cleanup dialog)
    DeployFileSet,      ///< Send a FileSet with nested blocks to the Director
};

/** What the Onesimus Director provides. */
enum class Support {
    No,
    Planned,    ///< Not there yet; the notice says "planned", never more
    Yes,
};

/**
 * @brief One row of the capability table
 *
 * The texts are English source strings, translated in the context
 * "BCapabilities" when a notice is built.
 */
struct Entry {
    Function function;
    const char *id;         ///< Stable identifier, also used in the settings key
    const char *name;       ///< Short user-visible name
    bool bareos;            ///< Classic Bareos provides the function
    Support onesimus;       ///< The Onesimus Director provides the function
    const char *reason;     ///< One sentence; %1 is the Director ("This Director (Bareos 25.2)")
    const char *manualWay;  ///< One sentence on the manual way, or nullptr
};

/** The table row of @p function; nullptr if the function is not listed. */
const Entry *entry(Function function);

/** All listed functions, in table order. */
QList<Function> allFunctions();

// ---------------------------------------------------------------------------
// Server description
// ---------------------------------------------------------------------------

/** How the GUI is connected. */
enum class Connection {
    None,       ///< Not connected, or nothing known about the server
    Classic,    ///< Bareos console protocol (BNET)
    Grpc,       ///< gRPC service of the Onesimus Director
};

enum class ServerKind {
    Unknown,            ///< Not identified: treated as classic Bareos, not named
    Bareos,
    OnesimusDirector,
};

struct Server {
    ServerKind kind = ServerKind::Unknown;
    /// Version for display: "25.2" for Bareos, the Onesimus Director's own
    /// version ("0.2.0") if it reports one, otherwise empty
    QString version;
};

/**
 * @brief Build the server description from what the login already delivered
 *
 * @param connection Transport of the session
 * @param identity   Classic: the Director's greeting after authentication
 *                   ("1000 OK: bareos-dir Version: 25.2.1 (...)") or the
 *                   version taken from it ("25.2.1"). gRPC: the version
 *                   field of the login response.
 *
 * Rules:
 * - Connection::None: Unknown.
 * - A version token "<a>.<b>.<c>+onesimus[.<own version>]": Onesimus
 *   Director. That is the version every Onesimus Director build reports
 *   in the greeting and over gRPC.
 * - Connection::Grpc: Onesimus Director even without that token, because
 *   only the Onesimus Director offers the gRPC service.
 * - Connection::Classic with a version "<a>.<b>.<c>" and no marker: Bareos.
 * - Anything else ("unknown", empty, unreadable): Unknown.
 */
Server describeServer(Connection connection, const QString &identity);

/**
 * @brief Whether @p server provides the function
 *
 * Bareos and Unknown follow the Bareos column; an Onesimus Director provides
 * a function only if its column says Yes.
 */
bool isAvailable(const Entry &entry, const Server &server);
bool isAvailable(Function function, const Server &server);

// ---------------------------------------------------------------------------
// Notice text
// ---------------------------------------------------------------------------

struct Notice {
    QString title;  ///< The function's name
    QString text;   ///< One short paragraph
};

/** "This Director (Bareos 25.2)", or just "This Director" if not identified. */
QString serverLabel(const Server &server);

/**
 * @brief The notice for a function that @p server does not provide
 *
 * Reason, then the manual way if the table has one, then what the Onesimus
 * Director offers: "supports this" only for Support::Yes, "planned" for
 * Support::Planned, nothing for Support::No.
 */
Notice notice(const Entry &entry, const Server &server);
Notice notice(Function function, const Server &server);

// ---------------------------------------------------------------------------
// When a notice is shown
// ---------------------------------------------------------------------------

/** How the user reached the function. */
enum class Request {
    /// Part of a longer flow (a dialog opens, a wizard finishes): the notice
    /// appears at most once per session
    Implicit,
    /// The user asked for exactly this function with a click and gets an
    /// answer every time
    Explicit,
};

/**
 * @brief Decides whether a notice is shown; holds the per-session memory
 *
 * "Do not show again" is a stored setting and is asked through the lookup,
 * so the class can be tested without settings and without a message box.
 */
class NoticePolicy
{
public:
    /// Returns true if the user ticked "Do not show again" for the entry id
    using SuppressedLookup = std::function<bool(const QString &id)>;

    explicit NoticePolicy(SuppressedLookup suppressed = SuppressedLookup());

    bool shouldShow(const Entry &entry, Request request) const;
    void recordShown(const Entry &entry);
    bool shownThisSession(const Entry &entry) const;

private:
    SuppressedLookup m_suppressed;
    QSet<QString> m_shown;
};

} // namespace BCapabilities

#endif // BCAPABILITIES_H
