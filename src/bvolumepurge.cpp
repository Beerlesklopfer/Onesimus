// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "bvolumepurge.h"

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QRegularExpression>

namespace BVolumePurge {

namespace {

QString translated(const char *text)
{
    return QCoreApplication::translate("BVolumePurge", text);
}

// Counts may come as numbers or as strings (catalog values)
qint64 toCount(const QJsonValue &value)
{
    if (value.isDouble()) {
        return qint64(value.toDouble());
    }
    if (value.isString()) {
        bool ok = false;
        const qint64 number = value.toString().toLongLong(&ok);
        return ok ? number : 0;
    }
    return 0;
}

Action toAction(const QString &text)
{
    if (text == QLatin1String("would_purge")) return Action::WouldPurge;
    if (text == QLatin1String("purged")) return Action::Purged;
    if (text == QLatin1String("skipped")) return Action::Skipped;
    return Action::Unknown;
}

QStringList stringList(const QJsonValue &value)
{
    QStringList list;
    if (value.isArray()) {
        for (const QJsonValue &item : value.toArray()) {
            list.append(item.toString());
        }
    } else if (value.isString()) {
        list = value.toString().split(QLatin1Char(','), Qt::SkipEmptyParts);
    }
    return list;
}

// The refusal: {"error":{"code":1,"message":"failed","data":{"messages":{"error":[...]}}}}
QString errorText(const QJsonObject &error)
{
    QStringList messages;
    const QJsonValue listed = error.value(QLatin1String("data")).toObject()
                                  .value(QLatin1String("messages")).toObject()
                                  .value(QLatin1String("error"));
    for (const QString &line : stringList(listed)) {
        if (!line.trimmed().isEmpty()) {
            messages.append(line.trimmed());
        }
    }
    if (messages.isEmpty()) {
        const QString message = error.value(QLatin1String("message")).toString().trimmed();
        if (!message.isEmpty()) {
            messages.append(message);
        }
    }
    if (messages.isEmpty()) {
        return translated(QT_TRANSLATE_NOOP("BVolumePurge",
                                            "The Director refused the command without a message."));
    }
    return messages.join(QLatin1Char(' '));
}

} // namespace

QStringList allowedStatuses()
{
    return {QStringLiteral("Full"), QStringLiteral("Used"), QStringLiteral("Error"),
            QStringLiteral("Append"), QStringLiteral("Archive"), QStringLiteral("Disabled"),
            QStringLiteral("Read-Only")};
}

QStringList defaultStatuses()
{
    return {QStringLiteral("Full"), QStringLiteral("Used"), QStringLiteral("Error")};
}

QString buildCommand(const Selection &selection, Mode mode, QString *error)
{
    auto refuse = [error](const char *text) {
        if (error) {
            *error = translated(text);
        }
        return QString();
    };

    const QString pool = selection.pool.trimmed();
    if (selection.allPools && !pool.isEmpty()) {
        return refuse(QT_TRANSLATE_NOOP("BVolumePurge",
                                        "Choose either one pool or all pools, not both."));
    }
    if (!selection.allPools && pool.isEmpty()) {
        return refuse(QT_TRANSLATE_NOOP("BVolumePurge", "Choose a pool or all pools."));
    }
    // A quote, backslash or control character would end the argument or
    // the command line early; such a name is not sent at all
    static const QRegularExpression unsafeRx(QStringLiteral("[\"\\\\\\x00-\\x1f\\x7f]"));
    if (!pool.isEmpty() && pool.contains(unsafeRx)) {
        return refuse(QT_TRANSLATE_NOOP("BVolumePurge",
                                        "The pool name contains characters that cannot be sent "
                                        "on a command line."));
    }

    if (selection.volStatus.isEmpty()) {
        return refuse(QT_TRANSLATE_NOOP("BVolumePurge", "Choose at least one volume status."));
    }
    const QStringList allowed = allowedStatuses();
    for (const QString &status : selection.volStatus) {
        if (!allowed.contains(status)) {
            return refuse(QT_TRANSLATE_NOOP("BVolumePurge", "Unknown volume status."));
        }
    }
    QStringList ordered;
    for (const QString &status : allowed) {
        if (selection.volStatus.contains(status)) {
            ordered.append(status);
        }
    }

    QString line = QStringLiteral("purge volume all ");
    if (selection.allPools) {
        line += QStringLiteral("allpools");
    } else {
        static const QRegularExpression plainRx(QStringLiteral("^[A-Za-z0-9_.:-]+$"));
        line += plainRx.match(pool).hasMatch()
                    ? QStringLiteral("pool=%1").arg(pool)
                    : QStringLiteral("pool=\"%1\"").arg(pool);
    }
    line += QStringLiteral(" volstatus=") + ordered.join(QLatin1Char(','));
    line += mode == Mode::DryRun ? QStringLiteral(" dryrun") : QStringLiteral(" yes");
    if (error) {
        error->clear();
    }
    return line;
}

Reply parseReply(const QString &reply)
{
    Reply result;
    const QString trimmed = reply.trimmed();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(trimmed.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        result.error = trimmed.isEmpty()
                           ? translated(QT_TRANSLATE_NOOP("BVolumePurge",
                                                          "The Director sent an empty answer."))
                           : translated(QT_TRANSLATE_NOOP("BVolumePurge",
                                                          "The Director did not answer with a "
                                                          "purge result: %1")).arg(trimmed);
        return result;
    }

    QJsonObject root = doc.object();
    if (root.contains(QLatin1String("error")) && root.value(QLatin1String("error")).isObject()) {
        result.error = errorText(root.value(QLatin1String("error")).toObject());
        return result;
    }
    if (root.value(QLatin1String("result")).isObject()) {
        root = root.value(QLatin1String("result")).toObject();
    }
    if (!root.value(QLatin1String("purge")).isObject()) {
        result.error = translated(QT_TRANSLATE_NOOP("BVolumePurge",
                                                    "The Director's answer has no purge result: %1"))
                           .arg(trimmed);
        return result;
    }

    const QJsonObject purge = root.value(QLatin1String("purge")).toObject();
    const QString mode = purge.value(QLatin1String("mode")).toString();
    if (mode == QLatin1String("dryrun")) {
        result.modeKnown = true;
        result.mode = Mode::DryRun;
    } else if (mode == QLatin1String("yes")) {
        result.modeKnown = true;
        result.mode = Mode::Purge;
    }
    result.pool = purge.value(QLatin1String("pool")).toString();
    result.allPools = purge.value(QLatin1String("allpools")).toBool();
    result.volStatus = stringList(purge.value(QLatin1String("volstatus")));

    for (const QJsonValue &item : purge.value(QLatin1String("volumes")).toArray()) {
        const QJsonObject volume = item.toObject();
        VolumeRow row;
        row.volume = volume.value(QLatin1String("volume")).toString();
        row.pool = volume.value(QLatin1String("pool")).toString();
        row.volStatus = volume.value(QLatin1String("volstatus")).toString();
        row.jobs = toCount(volume.value(QLatin1String("jobs")));
        row.files = toCount(volume.value(QLatin1String("files")));
        row.bytes = toCount(volume.value(QLatin1String("bytes")));
        row.actionText = volume.value(QLatin1String("action")).toString();
        row.action = toAction(row.actionText);
        row.reason = volume.value(QLatin1String("reason")).toString();
        result.volumes.append(row);
    }

    const QJsonObject summary = purge.value(QLatin1String("summary")).toObject();
    if (summary.isEmpty()) {
        // Without a summary the counts come from the rows
        result.selected = int(result.volumes.size());
        for (const VolumeRow &row : result.volumes) {
            if (row.action == Action::Purged || row.action == Action::WouldPurge) ++result.purged;
            if (row.action == Action::Skipped) ++result.skipped;
        }
    } else {
        result.selected = int(toCount(summary.value(QLatin1String("selected"))));
        result.purged = int(toCount(summary.value(QLatin1String("purged"))));
        result.skipped = int(toCount(summary.value(QLatin1String("skipped"))));
    }

    if (!result.modeKnown) {
        result.error = translated(QT_TRANSLATE_NOOP("BVolumePurge",
                                                    "The Director's purge result names no mode."));
        return result;
    }
    result.ok = true;
    return result;
}

} // namespace BVolumePurge
