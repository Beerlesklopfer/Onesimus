// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "jobs/bbvfsreplyrouter.h"

#include <QPointer>

BBvfsReplyRouter::BBvfsReplyRouter(SendFunction send, QObject *parent)
    : QObject(parent)
    , m_send(std::move(send))
{
}

BBvfsReplyRouter *BBvfsReplyRouter::forDirector(BDirector *director)
{
    if (!director)
        return nullptr;

    if (auto *existing = director->findChild<BBvfsReplyRouter *>(QString(), Qt::FindDirectChildrenOnly))
        return existing;

    QPointer<BDirector> target(director);
    auto *router = new BBvfsReplyRouter([target](Command cmd, const QString &args) {
        if (target)
            target->doSend(cmd, args);
    }, director);

    connect(director, &BDirector::jsonResult, router,
            [router](BDirector::Command cmd, const QString &data) { router->handleReply(cmd, data, true); });
    connect(director, &BDirector::textResult, router,
            [router](BDirector::Command cmd, const QString &data) { router->handleReply(cmd, data, false); });
    connect(director, &BDirector::commandFailed, router, &BBvfsReplyRouter::handleCommandFailed);
    // The Director empties its command queue when a session ends; requests
    // of an earlier session are never answered
    connect(director, &BDirector::disconnected, router,
            [router]() { router->reset(tr("The connection to the Director was lost.")); });
    connect(director, &BDirector::connected, router,
            [router]() { router->reset(tr("The connection to the Director was established again.")); });

    return router;
}

bool BBvfsReplyRouter::isTracked(Command cmd)
{
    switch (cmd) {
    case Command::BvfsGetJobIds:
    case Command::BvfsUpdate:
    case Command::BvfsLsDirs:
    case Command::BvfsLsFiles:
    case Command::BvfsRestore:
    case Command::BvfsCleanup:
    case Command::Restore:
        return true;
    default:
        return false;
    }
}

quint64 BBvfsReplyRouter::send(Command cmd, const QString &args)
{
    if (!isTracked(cmd))
        return 0;

    Request request;
    request.ticket = m_nextTicket++;
    request.cmd = cmd;
    request.args = args;
    m_pending.append(request);

    if (m_send)
        m_send(cmd, args);
    return request.ticket;
}

void BBvfsReplyRouter::handleReply(BDirector::Command cmd, const QString &data, bool isJson)
{
    // Any other reply lies between an error reply and its failure report
    m_lastAnsweredValid = false;

    if (!isTracked(cmd))
        return;

    for (int i = 0; i < m_pending.size(); ++i) {
        if (m_pending.at(i).cmd != cmd)
            continue;
        const Request request = m_pending.takeAt(i);
        m_lastAnswered = request;
        m_lastAnsweredValid = true;
        emit replied(request.ticket, cmd, data, isJson);
        return;
    }
    // No request of ours is waiting for this kind: someone else's reply
}

void BBvfsReplyRouter::handleCommandFailed(BDirector::Command cmd, const QString &args,
                                           const QString &reason)
{
    if (!isTracked(cmd))
        return;

    if (m_lastAnsweredValid && m_lastAnswered.cmd == cmd && m_lastAnswered.args == args) {
        // The failure report of the error reply just delivered
        m_lastAnsweredValid = false;
        return;
    }

    for (int i = 0; i < m_pending.size(); ++i) {
        const Request &request = m_pending.at(i);
        if (request.cmd != cmd || request.args != args)
            continue;
        const quint64 ticket = request.ticket;
        m_pending.removeAt(i);
        emit failed(ticket, cmd, reason);
        return;
    }
}

void BBvfsReplyRouter::reset(const QString &reason)
{
    m_lastAnsweredValid = false;
    const QList<Request> dropped = m_pending;
    m_pending.clear();
    for (const Request &request : dropped)
        emit failed(request.ticket, request.cmd, reason);
}
