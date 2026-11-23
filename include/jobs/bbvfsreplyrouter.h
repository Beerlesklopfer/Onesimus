// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BBVFSREPLYROUTER_H
#define BBVFSREPLYROUTER_H

#include <QObject>
#include <QList>
#include <functional>

#include "director/bdirector.h"

/**
 * @brief Matches the replies of BVFS and restore commands to their requests
 *
 * The Director answers commands in the order they were sent, and its reply
 * signals name only the command enum, not the request (see P3-1 in the
 * review backlog). A request of a restore wizard that was closed is still
 * answered after the wizard is gone. Without this class the next wizard
 * waiting for the same command took that answer as its own and browsed and
 * restored the wrong job (P0-15).
 *
 * Every command of the tracked kinds (isTracked()) is sent through send(),
 * which returns a ticket. A reply of a tracked kind belongs to the oldest
 * unanswered request of that kind; replied() carries that request's ticket,
 * and a receiver acts only on the ticket it is waiting for. A request that
 * nobody waits for any more (its sender was destroyed or reset) still takes
 * its own reply, so that reply cannot move on to a later request.
 *
 * This is valid as long as nothing else sends the tracked commands: the BVFS
 * browser (BBvfsModel) and the restore wizard are their only senders.
 */
class BBvfsReplyRouter : public QObject
{
    Q_OBJECT

public:
    using Command = BDirector::Command;
    using SendFunction = std::function<void(Command, const QString &)>;

    /// @param send Hands a command to the Director (in tests: records it)
    explicit BBvfsReplyRouter(SendFunction send, QObject *parent = nullptr);

    /**
     * @brief The router of a Director, created on first use as its child
     *
     * Connected to the Director's reply, failure and connection signals.
     * Returns nullptr for a null Director.
     */
    static BBvfsReplyRouter *forDirector(BDirector *director);

    /// Commands whose replies this router assigns
    static bool isTracked(Command cmd);

    /**
     * @brief Send a command and remember it as unanswered
     * @return The ticket of the request (never 0), or 0 for an untracked command
     */
    quint64 send(Command cmd, const QString &args);

    /// Number of requests still waiting for their reply
    int pendingCount() const { return m_pending.size(); }

public slots:
    /// A reply of the Director (JSON or text)
    void handleReply(BDirector::Command cmd, const QString &data, bool isJson);

    /**
     * @brief A command the Director reports as failed
     *
     * A command that was refused or dropped gets no reply; its request is
     * removed and failed() is emitted. The Director also reports an error
     * reply here, right after the reply itself; that report is ignored,
     * because the request was already answered.
     */
    void handleCommandFailed(BDirector::Command cmd, const QString &args, const QString &reason);

    /// The connection ended or started again: no reply will come for any request
    void reset(const QString &reason);

signals:
    /// The reply to the request with this ticket
    void replied(quint64 ticket, BDirector::Command cmd, const QString &data, bool isJson);

    /// The request with this ticket will not be answered
    void failed(quint64 ticket, BDirector::Command cmd, const QString &reason);

private:
    struct Request {
        quint64 ticket = 0;
        Command cmd = Command::Custom;
        QString args;
    };

    SendFunction m_send;
    QList<Request> m_pending;   ///< In the order they were sent
    quint64 m_nextTicket = 1;

    /// The request answered last, to recognise the failure report of an error reply
    bool m_lastAnsweredValid = false;
    Request m_lastAnswered;
};

#endif // BBVFSREPLYROUTER_H
