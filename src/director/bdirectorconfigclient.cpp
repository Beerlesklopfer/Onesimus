// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bdirectorconfigclient.cpp
 * @brief The configuration CRUD methods of the Director over BDirector
 */

#include "director/bdirectorconfigclient.h"
#include "director/bdirector.h"

#include <QTimer>

BDirectorConfigClient::BDirectorConfigClient(BDirector *director, QObject *parent)
    : BConfigClient(parent)
    , m_director(director)
{
    if (director) {
        connect(director, &BDirector::configReply, this, &BDirectorConfigClient::onReply);
        connect(director, &BDirector::configCapabilitiesChanged,
                this, &BConfigClient::capabilitiesChanged);
    }
}

BConfig::Capabilities BDirectorConfigClient::capabilities() const
{
    return m_director ? m_director->configCapabilities() : BConfig::Capabilities();
}

void BDirectorConfigClient::call(const BConfig::Request &request, QObject *context, Callback callback)
{
    if (!m_director) {
        BConfig::Reply reply;
        reply.status = BConfig::Status::NotAvailable;
        QPointer<QObject> guard(context);
        QTimer::singleShot(0, this, [guard, callback, reply]() {
            if (guard && callback)
                callback(reply);
        });
        return;
    }
    const quint64 id = m_director->configCall(request);
    m_pending.insert(id, Pending{QPointer<QObject>(context), std::move(callback)});
}

void BDirectorConfigClient::onReply(quint64 id, const BConfig::Reply &reply)
{
    const auto it = m_pending.find(id);
    if (it == m_pending.end())
        return;
    const Pending pending = it.value();
    m_pending.erase(it);
    if (pending.context && pending.callback)
        pending.callback(reply);
}
