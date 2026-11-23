// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BCLIENTREACHABILITY_H
#define BCLIENTREACHABILITY_H

#include <QDateTime>
#include <QJsonObject>
#include <QList>
#include <QString>
#include <QTimeZone>

#include "director/bclientevent.h"

/**
 * @brief Whether a client is reachable: the sources and their order, without widgets
 *
 * Issue #9. The status column of the clients table has four sources:
 *
 * - Connection: the client holds a client-initiated connection to the
 *   Director. "status director" lists those connections (Bareos
 *   core/src/dird/ua_status.cc ListConnectedClients(), array
 *   "client-connection"; the Onesimus Director commands/status.rs
 *   list_connected_clients() in the same shape). A fact for as long as the
 *   client is in the list.
 * - Event: client events the Onesimus Director pushes over gRPC
 *   (FdConnected, FdDisconnected, FdStatus; see BClientEvent).
 * - Probe: the user's "Check now" ("status client=<name>"), a single
 *   observation at a time.
 * - Catalog: the age of the last backup or contact (BClientStatus), an
 *   estimate and shown as one.
 *
 * Order (resolve()): an open connection (Connection, or an FdConnected that
 * was not yet followed by FdDisconnected) is a fact and wins. Otherwise the
 * newest observation still valid (an event or a probe, whichever came last)
 * wins. Otherwise the catalog estimate is shown.
 *
 * Everything here is QtCore only and unit-tested (bclientreachability_test).
 */
namespace BClientReachability {

// ---------------------------------------------------------------------------
// "status director": client-initiated connections
// ---------------------------------------------------------------------------

/** One entry of "client-connection". */
struct ClientConnection {
    QString name;               ///< Name of the Client resource
    QString connectTimeRaw;     ///< As the Director wrote it ("23-Nov-26 10:15")
    QDateTime connectTime;      ///< Parsed; invalid if unreadable
    int protocolVersion = -1;   ///< -1: not given
};

struct ConnectionList {
    /// The reply carries the list (it may be empty). False: the reply is not
    /// a "status director" reply this parser knows; nothing may be concluded.
    bool recognized = false;
    QList<ClientConnection> clients;
};

/**
 * @brief Reads the client-initiated connections from a "status director" reply
 *
 * Accepts the API 2 document ({"result": {..., "client-connection": [...]}}),
 * the result object alone, a failed reply that carries a result
 * ({"error": {"data": {"result": ...}}}), and the key spellings
 * "client-connection" (Bareos, Onesimus Director), "client_connection" and
 * "clientConnection" (protobuf field of DirectorStatusResponse).
 * Entries without a name, or marked as not authenticated (false or 0), are
 * skipped. Anything else (not JSON, no such key, not an array) gives
 * recognized=false.
 */
ConnectionList parseStatusDirector(const QString &reply, const QTimeZone &directorZone);
ConnectionList parseStatusDirector(const QJsonObject &root, const QTimeZone &directorZone);

/**
 * @brief Reads a connect time of "status director"
 *
 * Bareos writes it with bstrftime_nc() ("%d-%b-%y %H:%M", e.g.
 * "23-Nov-26 10:15"); a four-digit year is read too. Interpreted in
 * @p directorZone. Invalid if unreadable.
 */
QDateTime parseConnectTime(const QString &text, const QTimeZone &directorZone);

// ---------------------------------------------------------------------------
// "Check now": the reply of "status client=<name>"
// ---------------------------------------------------------------------------

enum class Probe {
    Unknown,        ///< Nothing to conclude (timeout, error, unreadable reply)
    Reachable,      ///< The file daemon answered
    Unreachable     ///< The Director reports that it could not connect
};

enum class ProbeReason {
    Answered,       ///< The file daemon's status header is in the reply
    ConnectFailed,  ///< "Failed to connect to Client <name>."
    Disabled,       ///< The Client resource is disabled; the Director skipped the call
    NoEvidence,     ///< A reply without either sign
    Timeout,        ///< No reply in time
    Error,          ///< The request failed (rights, unknown client, connection)
    NotAvailable    ///< This connection cannot carry the probe
};

struct ProbeOutcome {
    Probe result = Probe::Unknown;
    ProbeReason reason = ProbeReason::NoEvidence;
    QString detail;     ///< The line it rests on, or the error text
};

/**
 * @brief Classifies the reply of "status client=<name>"
 *
 * The reply is not reliable JSON on Bareos (bareos/bareos#2325), so it is
 * read defensively, as text; a JSON document is searched for its strings.
 * - Reachable: a line "<name> Version: <version>" and "Daemon started" (the
 *   header the file daemon sends; bareos-fd and the Rust file daemon alike).
 * - Unreachable: "Failed to connect to Client <name>" (Bareos
 *   dird/fd_cmds.cc DoNativeClientStatus(); the Onesimus Director the same).
 * - The file daemon's header wins over the connect failure: it can only
 *   come from the file daemon.
 * - Disabled client, or neither sign: Unknown.
 */
ProbeOutcome classifyProbeReply(const QString &reply);

/** The outcome of a probe as the Director layer reported it. */
ProbeOutcome classifyProbe(BClientProbeStatus status, const QString &text, const QString &error);

// ---------------------------------------------------------------------------
// Order of the sources
// ---------------------------------------------------------------------------

enum class Source { None, Catalog, Probe, Event, Connection };
enum class State { None, Online, Offline };

/** What one source says about a client. */
struct Fact {
    State state = State::None;
    Source source = Source::None;
    QDateTime at;           ///< The time it rests on (connect time, event, probe)
    QDateTime validUntil;   ///< Invalid: until replaced

    bool holdsAt(const QDateTime &now) const
    {
        return state != State::None && (!validUntil.isValid() || now < validUntil);
    }
};

/** Everything known about one client, apart from the catalog. */
struct Inputs {
    /// Listed by the latest "status director" (Source::Connection, Online)
    Fact listed;
    /// FdConnected not yet followed by FdDisconnected (Source::Event, Online)
    Fact openEvent;
    /// The newest single observation: an event (Alive, Unreachable, the end
    /// of a connection) or a probe
    Fact observation;
};

/**
 * @brief The fact the status column shows at @p now
 *
 * 1. Listed by "status director": Online (a client-initiated connection).
 * 2. An open connection reported by an event: Online.
 * 3. The newest observation if it still holds. An Offline observation ends
 *    when the catalog shows a backup or contact after it
 *    (@p lastCatalogActivity): that was a contact.
 * 4. Nothing (State::None): the caller shows the catalog estimate.
 */
Fact resolve(const Inputs &inputs, const QDateTime &now,
             const QDateTime &lastCatalogActivity = QDateTime());

/** Online after an answer lasts this long; then the estimate is shown again. */
constexpr int OnlineValidSecs = 10 * 60;

} // namespace BClientReachability

#endif // BCLIENTREACHABILITY_H
