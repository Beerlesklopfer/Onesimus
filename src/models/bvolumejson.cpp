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
    // Flat form: list volumes pool=X / list volumes all / llist volumes
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
        const QJsonArray groupVolumes = it.value().toArray();
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

} // namespace BVolumeJson
