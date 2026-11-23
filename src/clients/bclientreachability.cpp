// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "clients/bclientreachability.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QLocale>
#include <QRegularExpression>
#include <QStringList>

namespace BClientReachability {

namespace {

/// Every string of a JSON value, depth first
void collectStrings(const QJsonValue &value, QStringList &out, int depth = 0)
{
    if (depth > 16) {
        return;
    }
    if (value.isString()) {
        out.append(value.toString());
    } else if (value.isArray()) {
        for (const QJsonValue &item : value.toArray()) {
            collectStrings(item, out, depth + 1);
        }
    } else if (value.isObject()) {
        const QJsonObject object = value.toObject();
        for (auto it = object.constBegin(); it != object.constEnd(); ++it) {
            collectStrings(it.value(), out, depth + 1);
        }
    }
}

/// A value meaning "not authenticated": false, 0, "0", "false", "no"
bool isFalse(const QJsonValue &value)
{
    if (value.isBool()) {
        return !value.toBool();
    }
    if (value.isDouble()) {
        return value.toDouble() == 0.0;
    }
    if (value.isString()) {
        const QString text = value.toString().trimmed().toLower();
        return text == QLatin1String("0") || text == QLatin1String("false")
            || text == QLatin1String("no");
    }
    return false;
}

QJsonValue firstOf(const QJsonObject &object, std::initializer_list<const char *> keys)
{
    for (const char *key : keys) {
        const auto it = object.constFind(QLatin1String(key));
        if (it != object.constEnd()) {
            return it.value();
        }
    }
    return QJsonValue(QJsonValue::Undefined);
}

} // namespace

// ---------------------------------------------------------------------------
// "status director"
// ---------------------------------------------------------------------------

QDateTime parseConnectTime(const QString &text, const QTimeZone &directorZone)
{
    const QString trimmed = text.simplified();
    if (trimmed.isEmpty()) {
        return QDateTime();
    }
    // Bareos runs strftime() in its locale; the C locale is the usual one
    // and the only one read here
    const QLocale c = QLocale::c();
    static const char *const formats[] = {"dd-MMM-yy HH:mm", "d-MMM-yy HH:mm",
                                          "dd-MMM-yyyy HH:mm", "d-MMM-yyyy HH:mm",
                                          "yyyy-MM-dd HH:mm:ss", "yyyy-MM-dd HH:mm"};
    for (const char *format : formats) {
        QDateTime parsed = c.toDateTime(trimmed, QLatin1String(format));
        if (!parsed.isValid()) {
            continue;
        }
        // "yy" reads as 19yy; Bareos means 20yy
        if (QString::fromLatin1(format).contains(QLatin1String("-yy ")) && parsed.date().year() < 1970) {
            parsed = parsed.addYears(100);
        }
        return QDateTime(parsed.date(), parsed.time(), directorZone);
    }
    return QDateTime();
}

ConnectionList parseStatusDirector(const QJsonObject &root, const QTimeZone &directorZone)
{
    ConnectionList list;

    // Where the result may sit: the document itself, "result", or the result
    // a failed reply carries
    QList<QJsonObject> containers{root};
    if (root.value(QLatin1String("result")).isObject()) {
        containers.append(root.value(QLatin1String("result")).toObject());
    }
    const QJsonObject errorData = root.value(QLatin1String("error")).toObject()
                                      .value(QLatin1String("data")).toObject();
    if (errorData.value(QLatin1String("result")).isObject()) {
        containers.append(errorData.value(QLatin1String("result")).toObject());
    }

    for (const QJsonObject &container : std::as_const(containers)) {
        const QJsonValue array =
            firstOf(container, {"client-connection", "client_connection", "clientConnection"});
        if (!array.isArray()) {
            continue;
        }
        list.recognized = true;
        for (const QJsonValue &item : array.toArray()) {
            const QJsonObject entry = item.toObject();
            const QString name = entry.value(QLatin1String("name")).toString().trimmed();
            if (name.isEmpty()) {
                continue;
            }
            const QJsonValue authenticated = entry.value(QLatin1String("authenticated"));
            if (!authenticated.isUndefined() && !authenticated.isNull() && isFalse(authenticated)) {
                continue;
            }
            ClientConnection connection;
            connection.name = name;
            connection.connectTimeRaw =
                firstOf(entry, {"connecttime", "ConnectTime", "connect_time", "connectTime"}).toString();
            connection.connectTime = parseConnectTime(connection.connectTimeRaw, directorZone);
            const QJsonValue protocol = firstOf(entry, {"protocol_version", "protocolVersion"});
            if (protocol.isDouble()) {
                connection.protocolVersion = protocol.toInt(-1);
            } else if (protocol.isString()) {
                bool ok = false;
                const int value = protocol.toString().toInt(&ok);
                connection.protocolVersion = ok ? value : -1;
            }
            list.clients.append(connection);
        }
        return list;
    }
    return list;
}

ConnectionList parseStatusDirector(const QString &reply, const QTimeZone &directorZone)
{
    QJsonParseError error;
    const QJsonDocument doc = QJsonDocument::fromJson(reply.trimmed().toUtf8(), &error);
    if (error.error != QJsonParseError::NoError || !doc.isObject()) {
        return ConnectionList();
    }
    return parseStatusDirector(doc.object(), directorZone);
}

// ---------------------------------------------------------------------------
// "Check now"
// ---------------------------------------------------------------------------

ProbeOutcome classifyProbeReply(const QString &reply)
{
    QString text = reply;
    // A JSON document (a Director that wraps the text): search its strings
    const QString trimmed = reply.trimmed();
    if (trimmed.startsWith(QLatin1Char('{')) || trimmed.startsWith(QLatin1Char('['))) {
        const QJsonDocument doc = QJsonDocument::fromJson(trimmed.toUtf8());
        if (!doc.isNull()) {
            QStringList strings;
            collectStrings(doc.isObject() ? QJsonValue(doc.object()) : QJsonValue(doc.array()), strings);
            text = strings.join(QLatin1Char('\n'));
        }
    }
    // Bareos sends "\r\v" around the handshake line and may use \x01 for spaces
    text.replace(QChar(0x01), QLatin1Char(' '));

    ProbeOutcome outcome;

    // The file daemon's header: "<name> Version: <version> (<date>) <os>"
    // followed by "Daemon started <time>. Jobs: run=... running=..."
    static const QRegularExpression versionRx(QStringLiteral("^\\s*\\S.*\\sVersion:\\s*\\S+.*$"),
                                              QRegularExpression::MultilineOption);
    static const QRegularExpression startedRx(QStringLiteral("^\\s*Daemon started\\b"),
                                              QRegularExpression::MultilineOption);
    const QRegularExpressionMatch version = versionRx.match(text);
    if (version.hasMatch() && startedRx.match(text).hasMatch()) {
        outcome.result = Probe::Reachable;
        outcome.reason = ProbeReason::Answered;
        outcome.detail = version.captured(0).trimmed();
        return outcome;
    }

    // DoNativeClientStatus(): "\nFailed to connect to Client %s.\n====\n";
    // ConnectToFileDaemon(): "Failed to connect to client \"%s\"."
    static const QRegularExpression failedRx(QStringLiteral("^.*Failed to connect to client\\b.*$"),
                                             QRegularExpression::MultilineOption
                                                 | QRegularExpression::CaseInsensitiveOption);
    const QRegularExpressionMatch failed = failedRx.match(text);
    if (failed.hasMatch()) {
        outcome.result = Probe::Unreachable;
        outcome.reason = ProbeReason::ConnectFailed;
        outcome.detail = failed.captured(0).trimmed();
        return outcome;
    }

    // ClientStatus(): "Client \"%s\" is disabled; skipping status call"
    static const QRegularExpression disabledRx(QStringLiteral("^.*is disabled; skipping status call.*$"),
                                               QRegularExpression::MultilineOption);
    const QRegularExpressionMatch disabled = disabledRx.match(text);
    if (disabled.hasMatch()) {
        outcome.reason = ProbeReason::Disabled;
        outcome.detail = disabled.captured(0).trimmed();
        return outcome;
    }

    outcome.reason = ProbeReason::NoEvidence;
    outcome.detail = text.simplified().left(200);
    return outcome;
}

ProbeOutcome classifyProbe(BClientProbeStatus status, const QString &text, const QString &error)
{
    ProbeOutcome outcome;
    switch (status) {
    case BClientProbeStatus::Replied:
        return classifyProbeReply(text);
    case BClientProbeStatus::Timeout:
        outcome.reason = ProbeReason::Timeout;
        break;
    case BClientProbeStatus::Error:
        outcome.reason = ProbeReason::Error;
        break;
    case BClientProbeStatus::NotAvailable:
    case BClientProbeStatus::Busy:
        outcome.reason = ProbeReason::NotAvailable;
        break;
    }
    outcome.detail = error;
    return outcome;
}

// ---------------------------------------------------------------------------
// Order
// ---------------------------------------------------------------------------

Fact resolve(const Inputs &inputs, const QDateTime &now, const QDateTime &lastCatalogActivity)
{
    if (inputs.listed.state == State::Online && inputs.listed.holdsAt(now)) {
        return inputs.listed;
    }
    if (inputs.openEvent.state == State::Online && inputs.openEvent.holdsAt(now)) {
        return inputs.openEvent;
    }
    const Fact &observation = inputs.observation;
    if (observation.holdsAt(now)) {
        // A backup or contact after "could not connect" was a contact
        if (observation.state == State::Offline && lastCatalogActivity.isValid()
            && observation.at.isValid() && lastCatalogActivity > observation.at) {
            return Fact();
        }
        return observation;
    }
    return Fact();
}

} // namespace BClientReachability
