// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "models/bvolumejson.h"

#include <QJsonObject>

namespace BVolumeJson {

QString resolvePoolName(const QString &key, const QStringList &knownPools)
{
    for (const QString &pool : knownPools) {
        if (pool.compare(key, Qt::CaseInsensitive) == 0) {
            return pool;
        }
    }
    return key;
}

QJsonArray normalizeVolumes(const QJsonValue &volumes, const QStringList &knownPools)
{
    // Flat form: [l]list volumes pool=X / [l]list volumes all
    if (volumes.isArray()) {
        return volumes.toArray();
    }

    QJsonArray flat;
    if (!volumes.isObject()) {
        return flat;
    }

    // Grouped form: { "<lowercase pool name>": [ ... ], ... }
    const QJsonObject groups = volumes.toObject();
    for (auto it = groups.constBegin(); it != groups.constEnd(); ++it) {
        const QString poolName = resolvePoolName(it.key(), knownPools);
        // API mode 2: "<pool>": [ ... ]; typed gRPC form: "<pool>": { "volumes": [ ... ] }
        const QJsonArray groupVolumes = it.value().isArray()
            ? it.value().toArray()
            : it.value().toObject().value(QStringLiteral("volumes")).toArray();
        for (const QJsonValue &volumeVal : groupVolumes) {
            QJsonObject volume = volumeVal.toObject();
            if (volume["pool"].toString().trimmed().isEmpty()) {
                volume["pool"] = poolName;
            }
            flat.append(volume);
        }
    }
    return flat;
}

QString volumeStatusName(int status)
{
    // VolumeStatus in onesimus-dird proto/director_api.proto
    static const char *const names[] = {
        "", "Append", "Full", "Used", "Recycle", "Purged", "Error", "Busy",
        "Cleaning", "Archive", "Read-Only", "Disabled", "Unlabeled"};
    if (status < 1 || status >= int(sizeof(names) / sizeof(names[0]))) {
        return QString();
    }
    return QString::fromLatin1(names[status]);
}

QJsonArray volumesFromResult(const QJsonObject &result, const QStringList &knownPools,
                             bool *recognized)
{
    QJsonArray volumes;
    bool found = true;
    if (result.contains(QStringLiteral("volumes"))) {
        volumes = normalizeVolumes(result.value(QStringLiteral("volumes")), knownPools);
    } else if (result.value(QStringLiteral("pools")).isObject()) {
        volumes = normalizeVolumes(result.value(QStringLiteral("pools")), knownPools);
    } else {
        found = false;
    }
    if (recognized) {
        *recognized = found;
    }

    // Numeric volstatus of the typed form -> the API mode 2 name
    for (int i = 0; i < volumes.size(); ++i) {
        QJsonObject volume = volumes.at(i).toObject();
        const QJsonValue status = volume.value(QStringLiteral("volstatus"));
        if (status.isDouble()) {
            const QString name = volumeStatusName(status.toInt());
            if (!name.isEmpty()) {
                volume[QStringLiteral("volstatus")] = name;
                volumes[i] = volume;
            }
        }
    }
    return volumes;
}

} // namespace BVolumeJson
