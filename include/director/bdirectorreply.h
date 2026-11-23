// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BDIRECTORREPLY_H
#define BDIRECTORREPLY_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QString>
#include <QStringList>

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

namespace Detail {

/// A JSON reply: the text after leading whitespace (and a BOM) opens an
/// object or an array
inline bool looksLikeJson(const QString &trimmed)
{
    return trimmed.startsWith(QLatin1Char('{')) || trimmed.startsWith(QLatin1Char('['));
}

/// The error lines of a text reply (API mode 0). Only whole lines in the
/// forms Bareos uses for a refused command count; the word "error" inside a
/// job log line, a file name or a message ("... ERR=...") does not.
inline QStringList textErrorLines(const QString &reply)
{
    static const QRegularExpression errorLine(
        QStringLiteral("^(?:(?:Fatal )?Error: .*|\\S+: is an invalid command\\.?|1999 .*)$"),
        QRegularExpression::CaseInsensitiveOption);
    QStringList lines;
    const QStringList all = reply.split(QLatin1Char('\n'));
    for (const QString &line : all) {
        const QString trimmed = line.trimmed();
        if (errorLine.match(trimmed).hasMatch()) {
            lines << trimmed;
        }
    }
    return lines;
}

} // namespace Detail

/**
 * @brief Whether a Director reply reports a failed command
 *
 * Decided by the structure of the reply, never by a word somewhere in it:
 *
 * - JSON (API mode 2, and the gRPC transport, which wraps a failure the
 *   same way): the JSON-RPC "error" object at the top level, as Bareos
 *   OutputFormatter::JsonFinalizeResult and odir json_output.rs write it
 *   ({"jsonrpc":"2.0","id":null,"error":{"code":1,"message":"failed",...}}).
 *   "error" anywhere below "result" is data (a job log line, a column).
 *   A reply that opens like JSON but does not parse (cut off) is a failure.
 * - Text (API mode 0): a line that is a Bareos error message as a whole
 *   ("Error: ...", "Fatal error: ...", "<word>: is an invalid command.",
 *   "1999 ..."), not a line of a job log that contains the word.
 */
inline bool isErrorReply(const QString &reply)
{
    QString trimmed = reply.trimmed();
    if (trimmed.startsWith(QChar(0xFEFF))) {
        trimmed = trimmed.mid(1);
    }
    if (Detail::looksLikeJson(trimmed)) {
        QJsonParseError parseError;
        const QJsonDocument doc = QJsonDocument::fromJson(trimmed.toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            return true;
        }
        return doc.isObject() && doc.object().contains(QLatin1String("error"));
    }
    return !Detail::textErrorLines(trimmed).isEmpty();
}

/**
 * @brief The reason of a failed command, for a message to the user
 *
 * JSON: the "error" messages the Director collected, else the "message" of
 * the error object. Text: the error lines. Empty if there is none.
 */
inline QString errorText(const QString &reply)
{
    const QString trimmed = reply.trimmed();
    if (Detail::looksLikeJson(trimmed)) {
        const QJsonObject error = QJsonDocument::fromJson(trimmed.toUtf8())
                                      .object().value(QLatin1String("error")).toObject();
        QStringList texts;
        const QJsonArray messages = error.value(QLatin1String("data")).toObject()
                                        .value(QLatin1String("messages")).toObject()
                                        .value(QLatin1String("error")).toArray();
        for (const QJsonValue &message : messages) {
            texts << message.toString().trimmed();
        }
        texts.removeAll(QString());
        if (texts.isEmpty()) {
            texts << error.value(QLatin1String("message")).toString().trimmed();
        }
        return texts.join(QLatin1Char(' ')).trimmed();
    }
    return Detail::textErrorLines(trimmed).join(QLatin1Char(' '));
}

} // namespace BDirectorReply

#endif // BDIRECTORREPLY_H
