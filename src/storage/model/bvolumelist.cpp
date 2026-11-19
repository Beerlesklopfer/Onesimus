#include "storage/model/bvolumelist.h"

#include "models/bvolumejson.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QSet>

#include <algorithm>

namespace {

// Columns of the short listing (list_volumes_select_0, Bareos 25). In the
// typed form a missing one of these is a protobuf default (0, false).
const char *const kShortColumns[] = {
    "mediaid", "volbytes", "volfiles", "volretention", "recycle", "enabled",
};

// Further columns of the long listing (list_volumes_select_long_0) that
// BVolume keeps
const char *const kLongColumns[] = {
    "maxvolbytes", "voljobs", "maxvoljobs", "maxvolfiles", "volerrors",
    "recyclecount", "voluseduration",
};

// Keys only a long listing has; one of them marks a typed record as long
const char *const kLongMarkers[] = {"poolid", "labeldate", "voljobs", "volmounts"};

std::optional<qint64> toInteger(const QJsonValue &value)
{
    if (value.isDouble()) {
        return qint64(value.toDouble());
    }
    if (value.isBool()) {
        return value.toBool() ? 1 : 0;
    }
    if (value.isString()) {
        const QString text = value.toString().trimmed();
        if (text.isEmpty()) {
            return std::nullopt;
        }
        bool ok = false;
        const qint64 number = text.toLongLong(&ok);
        if (ok) {
            return number;
        }
        // "1.0e10" or "12.0" from a JSON writer that prints doubles
        const double real = text.toDouble(&ok);
        if (ok) {
            return qint64(real);
        }
    }
    return std::nullopt;
}

std::optional<bool> toBool(const QJsonValue &value)
{
    if (value.isBool()) {
        return value.toBool();
    }
    if (value.isString()) {
        const QString text = value.toString().trimmed().toLower();
        if (text == QLatin1String("yes") || text == QLatin1String("true")) {
            return true;
        }
        if (text == QLatin1String("no") || text == QLatin1String("false")) {
            return false;
        }
    }
    if (const std::optional<qint64> number = toInteger(value)) {
        return *number != 0;
    }
    return std::nullopt;
}

bool isTypedRecord(const QJsonObject &object)
{
    // API mode 2 sends every column as a string; the typed form numbers and
    // booleans
    for (const char *key : {"mediaid", "volbytes", "volretention", "enabled"}) {
        if (object.value(QLatin1String(key)).isDouble()) {
            return true;
        }
    }
    return object.value(QStringLiteral("recycle")).isBool();
}

} // namespace

QDateTime BVolumeList::parseDateTime(const QJsonValue &value)
{
    if (value.isDouble()) {
        const qint64 seconds = qint64(value.toDouble());
        return seconds > 0 ? QDateTime::fromSecsSinceEpoch(seconds) : QDateTime();
    }
    const QString text = value.toString().trimmed();
    if (text.isEmpty() || text.startsWith(QLatin1String("0000"))) {
        return QDateTime();
    }
    // Catalog time is the Director's local time, written without a zone
    QDateTime when = QDateTime::fromString(text, QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    if (!when.isValid()) {
        when = QDateTime::fromString(text, Qt::ISODate);
    }
    if (!when.isValid()) {
        bool ok = false;
        const qint64 seconds = text.toLongLong(&ok);
        if (ok && seconds > 0) {
            when = QDateTime::fromSecsSinceEpoch(seconds);
        }
    }
    return when;
}

BVolume BVolumeList::volumeFromJson(const QJsonObject &object)
{
    BVolume v;
    const auto str = [&object](const char *key) {
        return object.value(QLatin1String(key)).toString().trimmed();
    };
    const auto num = [&object](const char *key) {
        return toInteger(object.value(QLatin1String(key)));
    };

    v.name = str("volumename");
    v.pool = str("pool");
    v.status = str("volstatus");
    if (v.status.isEmpty() && object.value(QStringLiteral("volstatus")).isDouble()) {
        v.status = BVolumeJson::volumeStatusName(object.value(QStringLiteral("volstatus")).toInt());
    }
    v.mediaType = str("mediatype");
    v.storage = str("storage");
    v.recyclePool = str("recyclepool");
    v.scratchPool = str("scratchpool");

    v.mediaId = num("mediaid");
    v.volBytes = num("volbytes");
    v.maxVolBytes = num("maxvolbytes");
    v.volJobs = num("voljobs");
    v.maxVolJobs = num("maxvoljobs");
    v.volFiles = num("volfiles");
    v.maxVolFiles = num("maxvolfiles");
    v.volErrors = num("volerrors");
    v.recycleCount = num("recyclecount");
    v.volRetention = num("volretention");
    v.volUseDuration = num("voluseduration");
    v.recycle = toBool(object.value(QStringLiteral("recycle")));
    if (const std::optional<qint64> enabled = num("enabled")) {
        v.enabled = int(*enabled);
    } else if (const std::optional<bool> enabledFlag = toBool(object.value(QStringLiteral("enabled")))) {
        v.enabled = *enabledFlag ? 1 : 0;
    }
    if (const std::optional<qint64> action = num("actiononpurge")) {
        v.actionOnPurge = int(*action);
    }

    v.firstWritten = parseDateTime(object.value(QStringLiteral("firstwritten")));
    v.lastWritten = parseDateTime(object.value(QStringLiteral("lastwritten")));
    v.labelDate = parseDateTime(object.value(QStringLiteral("labeldate")));

    bool longForm = false;
    for (const char *key : kLongMarkers) {
        longForm = longForm || object.contains(QLatin1String(key));
    }
    v.longForm = longForm;

    // Protobuf JSON leaves out what holds its default: in the typed form a
    // missing column of the listing is 0 or false
    if (isTypedRecord(object)) {
        for (const char *key : kShortColumns) {
            if (object.contains(QLatin1String(key))) {
                continue;
            }
            const QLatin1String k(key);
            if (k == QLatin1String("mediaid")) v.mediaId = 0;
            else if (k == QLatin1String("volbytes")) v.volBytes = 0;
            else if (k == QLatin1String("volfiles")) v.volFiles = 0;
            else if (k == QLatin1String("volretention")) v.volRetention = 0;
            else if (k == QLatin1String("recycle")) v.recycle = false;
            else if (k == QLatin1String("enabled")) v.enabled = 0;
        }
        if (longForm) {
            for (const char *key : kLongColumns) {
                if (object.contains(QLatin1String(key))) {
                    continue;
                }
                const QLatin1String k(key);
                if (k == QLatin1String("maxvolbytes")) v.maxVolBytes = 0;
                else if (k == QLatin1String("voljobs")) v.volJobs = 0;
                else if (k == QLatin1String("maxvoljobs")) v.maxVolJobs = 0;
                else if (k == QLatin1String("maxvolfiles")) v.maxVolFiles = 0;
                else if (k == QLatin1String("volerrors")) v.volErrors = 0;
                else if (k == QLatin1String("recyclecount")) v.recycleCount = 0;
                else if (k == QLatin1String("voluseduration")) v.volUseDuration = 0;
            }
        }
    }
    return v;
}

BVolumeList BVolumeList::fromResult(const QJsonObject &result, const QStringList &knownPools,
                                    bool *recognized)
{
    BVolumeList list;
    list.m_knownPools = knownPools;
    const QJsonArray volumes = BVolumeJson::volumesFromResult(result, knownPools, recognized);
    // The grouped forms name every pool the console may see, also those
    // without a volume
    for (const char *key : {"volumes", "pools"}) {
        const QJsonValue groups = result.value(QLatin1String(key));
        if (groups.isObject()) {
            const QStringList keys = groups.toObject().keys();
            for (const QString &group : keys) {
                list.m_replyPools.append(BVolumeJson::resolvePoolName(group, knownPools));
            }
            break;
        }
    }
    list.m_volumes.reserve(volumes.size());
    for (const QJsonValue &value : volumes) {
        BVolume volume = volumeFromJson(value.toObject());
        if (!volume.pool.isEmpty()) {
            volume.pool = BVolumeJson::resolvePoolName(volume.pool, knownPools);
        }
        list.m_volumes.append(volume);
    }
    return list;
}

BVolumeList BVolumeList::fromJson(const QByteArray &json, const QStringList &knownPools,
                                  QString *error)
{
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(json, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        if (error) {
            *error = parseError.error != QJsonParseError::NoError
                ? parseError.errorString()
                : QStringLiteral("not a JSON object");
        }
        BVolumeList empty;
        empty.m_knownPools = knownPools;
        return empty;
    }
    bool recognized = false;
    BVolumeList list = fromResult(doc.object().value(QStringLiteral("result")).toObject(),
                                  knownPools, &recognized);
    if (error) {
        *error = recognized ? QString() : QStringLiteral("no volume list in the reply");
    }
    return list;
}

QStringList BVolumeList::pools() const
{
    QStringList result;
    QSet<QString> seen;
    const auto add = [&](const QString &pool) {
        if (pool.isEmpty() || seen.contains(pool.toLower())) {
            return;
        }
        seen.insert(pool.toLower());
        result.append(pool);
    };
    for (const QString &pool : m_knownPools) {
        add(pool.trimmed());
    }
    for (const QString &pool : m_replyPools) {
        add(pool);
    }
    for (const BVolume &volume : m_volumes) {
        add(volume.pool);
    }
    std::sort(result.begin(), result.end(), [](const QString &a, const QString &b) {
        return a.compare(b, Qt::CaseInsensitive) < 0;
    });
    return result;
}

int BVolumeList::countInPool(const QString &pool) const
{
    return int(std::count_if(m_volumes.cbegin(), m_volumes.cend(), [&pool](const BVolume &v) {
        return v.pool.compare(pool, Qt::CaseInsensitive) == 0;
    }));
}

int BVolumeList::countWithStatus(const QString &status) const
{
    return int(std::count_if(m_volumes.cbegin(), m_volumes.cend(),
                             [&status](const BVolume &v) { return v.status == status; }));
}

bool BVolumeList::hasLongForm() const
{
    return std::any_of(m_volumes.cbegin(), m_volumes.cend(),
                       [](const BVolume &v) { return v.longForm; });
}

QStringList BVolumeList::allStatuses()
{
    return {QStringLiteral("Append"),   QStringLiteral("Full"),     QStringLiteral("Used"),
            QStringLiteral("Purged"),   QStringLiteral("Recycle"),  QStringLiteral("Error"),
            QStringLiteral("Read-Only"), QStringLiteral("Disabled"), QStringLiteral("Archive"),
            QStringLiteral("Busy"),     QStringLiteral("Cleaning"), QStringLiteral("Unlabeled")};
}

QString BVolumeList::statusLetter(const QString &status)
{
    static const struct {
        const char *status;
        const char *letter;
    } letters[] = {
        {"Append", "A"}, {"Full", "F"},      {"Used", "U"},     {"Purged", "P"},
        {"Recycle", "R"}, {"Error", "E"},    {"Read-Only", "O"}, {"Disabled", "D"},
        {"Archive", "V"}, {"Busy", "B"},     {"Cleaning", "C"}, {"Unlabeled", "L"},
    };
    for (const auto &entry : letters) {
        if (status.compare(QLatin1String(entry.status), Qt::CaseInsensitive) == 0) {
            return QLatin1String(entry.letter);
        }
    }
    return QStringLiteral("?");
}
