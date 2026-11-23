// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BDIRECTORREPLY_H
#define BDIRECTORREPLY_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

/**
 * @brief Small helpers that judge Director replies, without widgets and without a Director
 */
namespace BDirectorReply {

/**
 * @brief Whether the reply of a "reload" command reports success
 *
 * API mode:  {"result":{"reload":{"success":true}}}; a failed reload is
 *            reported as {"error":{..,"data":{"result":{"reload":{"success":false}}}}}
 * Text mode: "reloaded" or "failed to reload"
 *
 * A JSON reply with neither an error nor a reload object (an empty result,
 * as the gRPC transport delivers it for a command without output) counts
 * as success: the command was accepted and nothing was reported as failed.
 */
inline bool reloadSucceeded(const QString &reply)
{
    const QString trimmed = reply.trimmed();

    if (trimmed.startsWith('{')) {
        QJsonParseError parseError;
        const QJsonDocument doc = QJsonDocument::fromJson(trimmed.toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
            return false;
        }
        const QJsonObject root = doc.object();
        if (root.contains("error")) {
            const QJsonObject data = root["error"].toObject()["data"].toObject();
            return data["result"].toObject()["reload"].toObject()["success"].toBool(false);
        }
        const QJsonObject result = root["result"].toObject();
        if (result.contains("reload")) {
            return result["reload"].toObject()["success"].toBool(false);
        }
        return true;
    }

    if (trimmed.contains("fail", Qt::CaseInsensitive)
        || trimmed.contains("error", Qt::CaseInsensitive)) {
        return false;
    }
    return trimmed.contains("reloaded", Qt::CaseInsensitive)
           || trimmed.contains("success", Qt::CaseInsensitive);
}

} // namespace BDirectorReply

#endif // BDIRECTORREPLY_H
