#include "models/bresourcemodels.h"
#include "blogging.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QRegularExpression>
#include <algorithm>

// ============================================================================
// BFilesetModel Implementation
// ============================================================================

BFilesetModel::BFilesetModel(QObject *parent)
    : BListModel(parent)
{
}

void BFilesetModel::parseFilesets(const QString &jsonResponse)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BFilesetModel: Failed to parse .filesets response:" << parseError.errorString();
        clear();
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "BFilesetModel: .filesets response is not a JSON object";
        clear();
        return;
    }

    setData(doc, "filesets");
}

QStringList BFilesetModel::filesetNames() const
{
    QStringList names;
    for (int i = 0; i < m_items.size(); ++i) {
        QJsonObject item = m_items[i].toObject();
        QString name = item["name"].toString();
        if (!name.isEmpty()) {
            names.append(name);
        }
    }
    return names;
}

void BFilesetModel::parseShowFilesets(const QString &jsonResponse)
{
    m_filesetConfigs.clear();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BFilesetModel: Failed to parse show filesets response:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "BFilesetModel: show filesets response is not a JSON object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonObject filesets = result["filesets"].toObject();

    for (auto it = filesets.begin(); it != filesets.end(); ++it) {
        QJsonObject fsObj = it.value().toObject();
        QString name = fsObj["name"].toString();
        if (!name.isEmpty()) {
            m_filesetConfigs[name] = fsObj;
        }
    }

    BLOG_DEBUG() << "BFilesetModel: Cached" << m_filesetConfigs.size() << "fileset configs from JSON";
}

QJsonObject BFilesetModel::filesetConfig(const QString &name) const
{
    return m_filesetConfigs.value(name);
}

QString BFilesetModel::getDisplayText(const QJsonObject &item) const
{
    return item["name"].toString();
}


// ============================================================================
// BStorageModel Implementation
// ============================================================================

BStorageModel::BStorageModel(QObject *parent)
    : BListModel(parent)
{
}

void BStorageModel::parseStorages(const QString &jsonResponse)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BStorageModel: Failed to parse .storages response:" << parseError.errorString();
        clear();
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "BStorageModel: .storages response is not a JSON object";
        clear();
        return;
    }

    setData(doc, "storages");
}

QStringList BStorageModel::storageNames() const
{
    QStringList names;
    for (int i = 0; i < m_items.size(); ++i) {
        QJsonObject item = m_items[i].toObject();
        QString name = item["name"].toString();
        if (!name.isEmpty()) {
            names.append(name);
        }
    }
    return names;
}

QString BStorageModel::getDisplayText(const QJsonObject &item) const
{
    QString name = item["name"].toString();
    QString address = item["address"].toString();

    if (!address.isEmpty()) {
        return QString("%1 (%2)").arg(name, address);
    }

    return name;
}


// ============================================================================
// BPoolModel Implementation
// ============================================================================

BPoolModel::BPoolModel(QObject *parent)
    : BListModel(parent)
{
}

void BPoolModel::parsePools(const QString &jsonResponse)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BPoolModel: Failed to parse .pools response:" << parseError.errorString();
        clear();
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "BPoolModel: .pools response is not a JSON object";
        clear();
        return;
    }

    setData(doc, "pools");
}

QStringList BPoolModel::poolNames() const
{
    QStringList names;
    for (int i = 0; i < m_items.size(); ++i) {
        QJsonObject item = m_items[i].toObject();
        QString name = item["name"].toString();
        if (!name.isEmpty()) {
            names.append(name);
        }
    }
    return names;
}

QJsonObject BPoolModel::poolInfo(const QString &poolName) const
{
    for (int i = 0; i < m_items.size(); ++i) {
        QJsonObject item = m_items[i].toObject();
        if (item["name"].toString() == poolName) {
            return item;
        }
    }
    return QJsonObject();
}

QString BPoolModel::getDisplayText(const QJsonObject &item) const
{
    QString name = item["name"].toString();
    QString poolType = item["pooltype"].toString();

    if (!poolType.isEmpty()) {
        return QString("%1 (%2)").arg(name, poolType);
    }

    return name;
}


// ============================================================================
// BCatalogModel Implementation
// ============================================================================

BCatalogModel::BCatalogModel(QObject *parent)
    : BListModel(parent)
{
}

void BCatalogModel::parseCatalogs(const QString &jsonResponse)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BCatalogModel: Failed to parse .catalogs response:" << parseError.errorString();
        clear();
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "BCatalogModel: .catalogs response is not a JSON object";
        clear();
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonValue catalogsValue = result["catalogs"];

    if (catalogsValue.isArray()) {
        // Array format: [{"name": "MyCatalog"}, ...]
        setData(catalogsValue.toArray());
    } else if (catalogsValue.isObject()) {
        // Object format: {"MyCatalog": {"name": "MyCatalog", ...}, ...}
        QJsonObject catalogsObj = catalogsValue.toObject();
        QJsonArray arr;
        for (auto it = catalogsObj.begin(); it != catalogsObj.end(); ++it) {
            if (it.value().isObject()) {
                arr.append(it.value());
            } else {
                // Simple name entry
                QJsonObject item;
                item["name"] = it.key();
                arr.append(item);
            }
        }
        setData(arr);
    } else {
        BLOG_WARNING() << "BCatalogModel: No catalogs found in response";
        clear();
    }
}

QStringList BCatalogModel::catalogNames() const
{
    QStringList names;
    for (int i = 0; i < m_items.size(); ++i) {
        QJsonObject item = m_items[i].toObject();
        QString name = item["name"].toString();
        if (!name.isEmpty()) {
            names.append(name);
        }
    }
    return names;
}

QString BCatalogModel::getDisplayText(const QJsonObject &item) const
{
    return item["name"].toString();
}


// ============================================================================
// BLevelModel Implementation
// ============================================================================

BLevelModel::BLevelModel(QObject *parent)
    : BListModel(parent)
{
}

void BLevelModel::parseLevels(const QString &jsonResponse)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BLevelModel: Failed to parse .levels response:" << parseError.errorString();
        clear();
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "BLevelModel: .levels response is not a JSON object";
        clear();
        return;
    }

#ifdef IS_DEVELOPER
    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonArray levelsArray = result["levels"].toArray();
    BLOG_DEBUG() << "BLevelModel: Found" << levelsArray.size() << "levels in response";
    for (int i = 0; i < levelsArray.size() && i < 5; ++i) {
        BLOG_DEBUG() << "  Level" << i << ":" << levelsArray[i].toObject();
    }
#endif

    setData(doc, "levels");
}

QStringList BLevelModel::levelCodes() const
{
    QStringList codes;
    for (int i = 0; i < m_items.size(); ++i) {
        QJsonObject item = m_items[i].toObject();

        // Bareos returns level as ASCII code (e.g., 70='F', 73='I', 68='D')
        // Try string first (for compatibility), then numeric
        QString code = item["level"].toString();

        if (code.isEmpty()) {
            // Convert numeric ASCII code to character
            int levelNum = item["level"].toInt();
            if (levelNum > 0 && levelNum < 128) {
                code = QChar(levelNum);
            }
        }

        if (!code.isEmpty()) {
            codes.append(code);
        }
    }
    return codes;
}

QStringList BLevelModel::levelDescriptions() const
{
    QStringList descriptions;
    for (int i = 0; i < m_items.size(); ++i) {
        QJsonObject item = m_items[i].toObject();
        QString name = item["name"].toString();
        if (!name.isEmpty()) {
            descriptions.append(name);
        }
    }
    return descriptions;
}

QString BLevelModel::getDisplayText(const QJsonObject &item) const
{
    QString level = item["level"].toString();

    // Bareos returns level as ASCII code - convert if needed
    if (level.isEmpty()) {
        int levelNum = item["level"].toInt();
        if (levelNum > 0 && levelNum < 128) {
            level = QChar(levelNum);
        }
    }

    QString name = item["name"].toString();

    if (!level.isEmpty() && !name.isEmpty()) {
        return QString("%1 - %2").arg(level, name);
    }

    if (!name.isEmpty()) {
        return name;
    }

    return level;
}


// ============================================================================
// BJobConfigModel Implementation
// ============================================================================

BJobConfigModel::BJobConfigModel(QObject *parent)
    : QObject(parent)
{
}

void BJobConfigModel::parseShowJobs(const QString &jsonResponse)
{
    m_jobConfigs.clear();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BJobConfigModel: Failed to parse show jobs response:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "BJobConfigModel: show jobs response is not a JSON object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonObject jobs = result["jobs"].toObject();

    for (auto it = jobs.begin(); it != jobs.end(); ++it) {
        QJsonObject jobObj = it.value().toObject();
        QString name = jobObj["name"].toString();
        if (!name.isEmpty()) {
            m_jobConfigs[name] = jobObj;
        }
    }

    BLOG_DEBUG() << "BJobConfigModel: Cached" << m_jobConfigs.size() << "job configs from show jobs";
}

QList<BConfigResource> BJobConfigModel::jobResources() const
{
    QList<BConfigResource> resources;
    for (auto it = m_jobConfigs.constBegin(); it != m_jobConfigs.constEnd(); ++it) {
        resources.append(jsonToResource("Job", it.key(), it.value()));
    }
    return resources;
}

QStringList BJobConfigModel::jobNames() const
{
    return m_jobConfigs.keys();
}

BConfigResource BJobConfigModel::jsonToResource(const QString &resourceType,
                                                const QString &name, const QJsonObject &obj)
{
    BConfigResource res(resourceType, name);

    for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
        QString key = it.key();
        if (key == "name") continue;  // Already set as resource name

        res.setValue(key, jsonValueToBConfigValue(it.value()));
    }

    return res;
}

BConfigValue BJobConfigModel::jsonValueToBConfigValue(const QJsonValue &val)
{
    if (val.isString()) {
        return BConfigValue(val.toString());
    }
    if (val.isBool()) {
        return BConfigValue(val.toBool() ? "yes" : "no");
    }
    if (val.isDouble()) {
        // Integer check
        double d = val.toDouble();
        if (d == static_cast<long long>(d)) {
            return BConfigValue(QString::number(static_cast<long long>(d)));
        }
        return BConfigValue(QString::number(d));
    }
    if (val.isArray()) {
        QJsonArray arr = val.toArray();
        QStringList list;
        for (const QJsonValue &item : arr) {
            if (item.isString()) {
                list.append(item.toString());
            } else if (item.isObject()) {
                // Array of objects → BlockList
                QList<QMap<QString, BConfigValue>> blockList;
                for (const QJsonValue &blockItem : arr) {
                    if (!blockItem.isObject()) continue;
                    QMap<QString, BConfigValue> block;
                    QJsonObject blockObj = blockItem.toObject();
                    for (auto bit = blockObj.constBegin(); bit != blockObj.constEnd(); ++bit) {
                        block[bit.key()] = jsonValueToBConfigValue(bit.value());
                    }
                    blockList.append(block);
                }
                return BConfigValue(blockList);
            } else {
                list.append(item.toVariant().toString());
            }
        }
        return BConfigValue(list);
    }
    if (val.isObject()) {
        QJsonObject obj = val.toObject();
        // Bareos "show" commands return resource references as {"name": "ResourceName", ...}
        // Convert these to simple strings so resource_reference fields work correctly
        if (obj.contains("name") && obj["name"].isString()) {
            return BConfigValue(obj["name"].toString());
        }
        QMap<QString, BConfigValue> block;
        for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
            block[it.key()] = jsonValueToBConfigValue(it.value());
        }
        return BConfigValue(block);
    }
    return BConfigValue(QString());
}


// ============================================================================
// BJobConfigModel — JobDefs methods
// ============================================================================

void BJobConfigModel::parseShowJobDefs(const QString &jsonResponse)
{
    m_jobDefsConfigs.clear();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BJobConfigModel: Failed to parse show jobdefs response:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "BJobConfigModel: show jobdefs response is not a JSON object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonObject jobdefs = result["jobdefs"].toObject();

    for (auto it = jobdefs.begin(); it != jobdefs.end(); ++it) {
        QJsonObject jdObj = it.value().toObject();
        QString name = jdObj["name"].toString();
        if (!name.isEmpty()) {
            m_jobDefsConfigs[name] = jdObj;
        }
    }

    BLOG_DEBUG() << "BJobConfigModel: Cached" << m_jobDefsConfigs.size() << "jobdefs configs";
}

QList<BConfigResource> BJobConfigModel::jobDefsResources() const
{
    QList<BConfigResource> resources;
    for (auto it = m_jobDefsConfigs.constBegin(); it != m_jobDefsConfigs.constEnd(); ++it) {
        resources.append(jsonToResource("JobDefs", it.key(), it.value()));
    }
    return resources;
}

QStringList BJobConfigModel::jobDefsNames() const
{
    return m_jobDefsConfigs.keys();
}


// ============================================================================
// BScheduleEntry Implementation
// ============================================================================

QString BScheduleEntry::toRunDirective() const
{
    QStringList parts;

    // Level
    if (level != None) {
        parts.append(levelToString(level));
    }

    // Week specification (1st, 2nd-5th, etc.)
    if (!weekSpec.isEmpty()) {
        parts.append(weekSpec);
    }

    // Days
    static const QStringList dayAbbrev = {"mon", "tue", "wed", "thu", "fri", "sat", "sun"};

    if (daysOfWeek.size() == 7) {
        parts.append("daily");
    } else if (daysOfWeek.size() == 6 && !daysOfWeek.contains(6)) {
        parts.append("mon-sat");
    } else if (daysOfWeek.size() == 5 && !daysOfWeek.contains(5) && !daysOfWeek.contains(6)) {
        parts.append("mon-fri");
    } else {
        QStringList dayParts;
        for (int d : daysOfWeek) {
            if (d >= 0 && d < 7) {
                dayParts.append(dayAbbrev[d]);
            }
        }
        parts.append(dayParts.join(","));
    }

    // Time
    parts.append(QString("at %1:%2")
                 .arg(hour, 2, 10, QChar('0'))
                 .arg(minute, 2, 10, QChar('0')));

    // Optional overrides
    if (!pool.isEmpty()) {
        parts.append(QString("pool=%1").arg(pool));
    }
    if (!storage.isEmpty()) {
        parts.append(QString("storage=%1").arg(storage));
    }
    if (!messages.isEmpty()) {
        parts.append(QString("messages=%1").arg(messages));
    }
    if (priority != 10) {
        parts.append(QString("priority=%1").arg(priority));
    }

    return parts.join(" ");
}

BScheduleEntry BScheduleEntry::fromRunDirective(const QString &run)
{
    BScheduleEntry entry;
    QString lower = run.toLower().trimmed();

    // Extract level
    entry.level = levelFromString(run);

    // Extract time
    QRegularExpression timeRx("at\\s+(\\d{1,2}):(\\d{2})");
    QRegularExpressionMatch timeMatch = timeRx.match(lower);
    if (timeMatch.hasMatch()) {
        entry.hour = timeMatch.captured(1).toInt();
        entry.minute = timeMatch.captured(2).toInt();
    }

    // Extract optional overrides: pool=, storage=, messages=, priority=
    QRegularExpression poolRx("pool=(\\S+)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch poolMatch = poolRx.match(run);
    if (poolMatch.hasMatch()) {
        entry.pool = poolMatch.captured(1);
    }

    QRegularExpression storageRx("storage=(\\S+)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch storageMatch = storageRx.match(run);
    if (storageMatch.hasMatch()) {
        entry.storage = storageMatch.captured(1);
    }

    QRegularExpression messagesRx("messages=(\\S+)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch messagesMatch = messagesRx.match(run);
    if (messagesMatch.hasMatch()) {
        entry.messages = messagesMatch.captured(1);
    }

    QRegularExpression prioRx("priority=(\\d+)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch prioMatch = prioRx.match(run);
    if (prioMatch.hasMatch()) {
        entry.priority = prioMatch.captured(1).toInt();
    }

    // Extract week specification (1st, 2nd-5th, last, etc.)
    QRegularExpression weekRx("\\b((?:1st|2nd|3rd|4th|5th|last)(?:-(?:1st|2nd|3rd|4th|5th|last))?)\\b",
                              QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch weekMatch = weekRx.match(run);
    if (weekMatch.hasMatch()) {
        entry.weekSpec = weekMatch.captured(1).toLower();
    }

    // Extract days
    if (lower.contains("daily")) {
        for (int i = 0; i < 7; ++i) entry.daysOfWeek.append(i);
    } else if (lower.contains("hourly")) {
        for (int i = 0; i < 7; ++i) entry.daysOfWeek.append(i);
    } else if (lower.contains("monthly")) {
        // Monthly: runs once per month, show on all days in Gantt for visibility
        for (int i = 0; i < 7; ++i) entry.daysOfWeek.append(i);
    } else if (lower.contains("mon-sat")) {
        for (int i = 0; i < 6; ++i) entry.daysOfWeek.append(i);
    } else if (lower.contains("mon-fri")) {
        for (int i = 0; i < 5; ++i) entry.daysOfWeek.append(i);
    } else if (lower.contains("sun-fri")) {
        entry.daysOfWeek = {6, 0, 1, 2, 3, 4};
    } else if (lower.contains("sun-sat")) {
        for (int i = 0; i < 7; ++i) entry.daysOfWeek.append(i);
    } else {
        // Check individual day names
        // Order: mon=0, tue=1, wed=2, thu=3, fri=4, sat=5, sun=6
        if (lower.contains("mon")) entry.daysOfWeek.append(0);
        if (lower.contains("tue")) entry.daysOfWeek.append(1);
        if (lower.contains("wed")) entry.daysOfWeek.append(2);
        if (lower.contains("thu")) entry.daysOfWeek.append(3);
        if (lower.contains("fri")) entry.daysOfWeek.append(4);
        if (lower.contains("sat")) entry.daysOfWeek.append(5);
        if (lower.contains("sun")) entry.daysOfWeek.append(6);
    }

    // Fallback: if no days detected (e.g. "weekly" without day, or weekSpec-only),
    // show on all days so the entry is always visible in the Gantt
    if (entry.daysOfWeek.isEmpty()) {
        for (int i = 0; i < 7; ++i) entry.daysOfWeek.append(i);
    }

    return entry;
}

QString BScheduleEntry::levelToString(BackupLevel level)
{
    switch (level) {
    case Full:         return QStringLiteral("Full");
    case Differential: return QStringLiteral("Differential");
    case Incremental:  return QStringLiteral("Incremental");
    case VirtualFull:  return QStringLiteral("VirtualFull");
    case None:
    default:           return QString();
    }
}

BScheduleEntry::BackupLevel BScheduleEntry::levelFromString(const QString &str)
{
    QString lower = str.toLower();

    if (lower.contains("virtualfull") || lower.contains("level=virtualfull")) {
        return VirtualFull;
    }
    if (lower.contains("differential") || lower.contains("level=differential")) {
        return Differential;
    }
    if (lower.contains("incremental") || lower.contains("level=incremental")) {
        return Incremental;
    }
    if (lower.contains("full") || lower.contains("level=full")) {
        return Full;
    }

    // Default to Incremental if no level specified
    return Incremental;
}


// ============================================================================
// BScheduleModel Implementation
// ============================================================================

BScheduleModel::BScheduleModel(QObject *parent)
    : BListModel(parent)
{
}

void BScheduleModel::parseSchedules(const QString &jsonResponse)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BScheduleModel: Failed to parse .schedule response:" << parseError.errorString();
        clear();
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "BScheduleModel: .schedule response is not a JSON object";
        clear();
        return;
    }

    // Store full configs
    m_scheduleConfigs.clear();
    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonArray schedulesArray = result["schedules"].toArray();

    for (const QJsonValue &val : schedulesArray) {
        if (val.isObject()) {
            QJsonObject sched = val.toObject();
            QString name = sched["name"].toString();
            if (!name.isEmpty()) {
                m_scheduleConfigs[name] = sched;
            }
        }
    }

    setData(doc, "schedules");
    rebuildEntries();

    BLOG_DEBUG() << "BScheduleModel: Loaded" << m_scheduleConfigs.size()
                 << "schedules with" << m_entries.size() << "run entries";
}

QStringList BScheduleModel::scheduleNames() const
{
    QStringList names;
    for (int i = 0; i < m_items.size(); ++i) {
        QJsonObject item = m_items[i].toObject();
        QString name = item["name"].toString();
        if (!name.isEmpty()) {
            names.append(name);
        }
    }
    return names;
}

QJsonObject BScheduleModel::scheduleByName(const QString &name) const
{
    return m_scheduleConfigs.value(name);
}

QList<BScheduleEntry> BScheduleModel::entriesForSchedule(const QString &scheduleName) const
{
    QList<BScheduleEntry> result;
    for (const BScheduleEntry &entry : m_entries) {
        if (entry.scheduleName == scheduleName) {
            result.append(entry);
        }
    }
    return result;
}

QString BScheduleModel::getDisplayText(const QJsonObject &item) const
{
    return item["name"].toString();
}

void BScheduleModel::rebuildEntries()
{
    m_entries.clear();

    for (auto it = m_scheduleConfigs.constBegin(); it != m_scheduleConfigs.constEnd(); ++it) {
        const QString &scheduleName = it.key();
        // Bareos API may use "run" (compact) or "Run" (standard)
        QJsonArray runs = it.value()["run"].toArray();
        if (runs.isEmpty()) {
            runs = it.value()["Run"].toArray();
        }

#ifdef DEBUG_JSON
        BLOG_DEBUG() << "BScheduleModel::rebuildEntries: Schedule" << scheduleName
                     << "has" << runs.size() << "run directives";
#endif

        for (const QJsonValue &runVal : runs) {
            if (!runVal.isString()) continue;

#ifdef DEBUG_JSON
            BLOG_DEBUG() << "  Run directive:" << runVal.toString();
#endif

            BScheduleEntry entry = BScheduleEntry::fromRunDirective(runVal.toString());
            entry.scheduleName = scheduleName;
            m_entries.append(entry);

#ifdef DEBUG_JSON
            BLOG_DEBUG() << "    -> level:" << BScheduleEntry::levelToString(entry.level)
                         << "hour:" << entry.hour << "min:" << entry.minute
                         << "days:" << entry.daysOfWeek;
#endif
        }
    }
}


// ============================================================================
// BJobDurationStats Implementation
// ============================================================================

BJobDurationStats::BJobDurationStats(QObject *parent)
    : QObject(parent)
{
}

void BJobDurationStats::feedJobs(const QString &jsonResponse)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BJobDurationStats: Failed to parse response:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) return;

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonArray jobsArray = result["jobs"].toArray();

    for (const QJsonValue &val : jobsArray) {
        if (!val.isObject()) continue;

        QJsonObject job = val.toObject();
        QString name = job["name"].toString();
        if (name.isEmpty()) continue;

        // Parse duration (format: "00:02:35" or seconds as int)
        int durationSecs = 0;
        QJsonValue durVal = job["duration"];
        if (durVal.isString()) {
            durationSecs = parseDuration(durVal.toString());
        } else if (durVal.isDouble()) {
            durationSecs = durVal.toInt();
        }

        // Parse start time
        QDateTime startTime;
        QString startStr = job["starttime"].toString();
        if (!startStr.isEmpty()) {
            startTime = QDateTime::fromString(startStr, Qt::ISODate);
            if (!startTime.isValid()) {
                startTime = QDateTime::fromString(startStr, "yyyy-MM-dd HH:mm:ss");
            }
        }

        // Parse level
        QString level = job["level"].toString();

        // Only record completed jobs with positive duration
        if (durationSecs > 0) {
            JobRunRecord record;
            record.startTime = startTime;
            record.durationSecs = durationSecs;
            record.level = level;

            // Store under jobName (aggregate across all levels)
            m_stats[name].runs.append(record);
            recalculate(m_stats[name]);

            // Store under jobName|level (level-specific)
            if (!level.isEmpty()) {
                QString levelKey = name + QLatin1Char('|') + level;
                m_stats[levelKey].runs.append(record);
                recalculate(m_stats[levelKey]);
            }
        }
    }

    BLOG_DEBUG() << "BJobDurationStats: Tracking" << m_stats.size() << "unique jobs";
}

void BJobDurationStats::clear()
{
    m_stats.clear();
}

int BJobDurationStats::averageDuration(const QString &jobName) const
{
    auto it = m_stats.find(jobName);
    if (it == m_stats.end()) return 0;
    return it->avgSecs;
}

int BJobDurationStats::averageDuration(const QString &jobName, const QString &level) const
{
    if (level.isEmpty()) return averageDuration(jobName);
    int result = averageDuration(jobName + QLatin1Char('|') + level);
    return result > 0 ? result : averageDuration(jobName); // fallback to aggregate
}

int BJobDurationStats::minDuration(const QString &jobName) const
{
    auto it = m_stats.find(jobName);
    if (it == m_stats.end()) return 0;
    return it->minSecs;
}

int BJobDurationStats::minDuration(const QString &jobName, const QString &level) const
{
    if (level.isEmpty()) return minDuration(jobName);
    int result = minDuration(jobName + QLatin1Char('|') + level);
    return result > 0 ? result : minDuration(jobName);
}

int BJobDurationStats::maxDuration(const QString &jobName) const
{
    auto it = m_stats.find(jobName);
    if (it == m_stats.end()) return 0;
    return it->maxSecs;
}

int BJobDurationStats::maxDuration(const QString &jobName, const QString &level) const
{
    if (level.isEmpty()) return maxDuration(jobName);
    int result = maxDuration(jobName + QLatin1Char('|') + level);
    return result > 0 ? result : maxDuration(jobName);
}

int BJobDurationStats::sampleCount(const QString &jobName) const
{
    auto it = m_stats.find(jobName);
    if (it == m_stats.end()) return 0;
    return it->runs.size();
}

int BJobDurationStats::sampleCount(const QString &jobName, const QString &level) const
{
    if (level.isEmpty()) return sampleCount(jobName);
    return sampleCount(jobName + QLatin1Char('|') + level);
}

QStringList BJobDurationStats::jobNames() const
{
    return m_stats.keys();
}

int BJobDurationStats::parseDuration(const QString &durationStr)
{
    // Format: "HH:MM:SS" or "H:MM:SS"
    QRegularExpression rx("(\\d+):(\\d{2}):(\\d{2})");
    QRegularExpressionMatch match = rx.match(durationStr);
    if (match.hasMatch()) {
        int hours = match.captured(1).toInt();
        int minutes = match.captured(2).toInt();
        int seconds = match.captured(3).toInt();
        return hours * 3600 + minutes * 60 + seconds;
    }

    // Try plain number (seconds)
    bool ok;
    int secs = durationStr.toInt(&ok);
    return ok ? secs : 0;
}

void BJobDurationStats::recalculate(DurationData &data)
{
    if (data.runs.isEmpty()) {
        data.minSecs = data.maxSecs = data.avgSecs = 0;
        return;
    }

    data.minSecs = data.runs.first().durationSecs;
    data.maxSecs = data.runs.first().durationSecs;
    long long total = 0;

    for (const JobRunRecord &r : data.runs) {
        if (r.durationSecs < data.minSecs) data.minSecs = r.durationSecs;
        if (r.durationSecs > data.maxSecs) data.maxSecs = r.durationSecs;
        total += r.durationSecs;
    }

    data.avgSecs = static_cast<int>(total / data.runs.size());
}

QList<BJobDurationStats::JobRunRecord> BJobDurationStats::lastRuns(const QString &jobName, int count) const
{
    auto it = m_stats.find(jobName);
    if (it == m_stats.end()) return {};

    QVector<JobRunRecord> sorted = it->runs;
    std::sort(sorted.begin(), sorted.end(), [](const JobRunRecord &a, const JobRunRecord &b) {
        return a.startTime > b.startTime;  // newest first
    });

    QList<JobRunRecord> result;
    for (int i = 0; i < qMin(count, static_cast<int>(sorted.size())); ++i) {
        result.append(sorted[i]);
    }
    return result;
}

QList<BJobDurationStats::JobRunRecord> BJobDurationStats::lastRuns(const QString &jobName, const QString &level, int count) const
{
    if (level.isEmpty()) return lastRuns(jobName, count);
    QList<JobRunRecord> result = lastRuns(jobName + QLatin1Char('|') + level, count);
    return result.isEmpty() ? lastRuns(jobName, count) : result;
}

double BJobDurationStats::trend(const QString &jobName) const
{
    auto it = m_stats.find(jobName);
    if (it == m_stats.end() || it->runs.size() < 4) return 0.0;

    // Compare average of recent half vs older half
    QVector<JobRunRecord> sorted = it->runs;
    std::sort(sorted.begin(), sorted.end(), [](const JobRunRecord &a, const JobRunRecord &b) {
        return a.startTime < b.startTime;  // oldest first
    });

    int half = sorted.size() / 2;
    long long olderTotal = 0, recentTotal = 0;
    int olderCount = 0, recentCount = 0;

    for (int i = 0; i < half; ++i) {
        olderTotal += sorted[i].durationSecs;
        olderCount++;
    }
    for (int i = half; i < sorted.size(); ++i) {
        recentTotal += sorted[i].durationSecs;
        recentCount++;
    }

    if (olderCount == 0 || recentCount == 0) return 0.0;

    double olderAvg = static_cast<double>(olderTotal) / olderCount;
    double recentAvg = static_cast<double>(recentTotal) / recentCount;

    return recentAvg - olderAvg;  // positive = getting slower
}

double BJobDurationStats::trend(const QString &jobName, const QString &level) const
{
    if (level.isEmpty()) return trend(jobName);
    double result = trend(jobName + QLatin1Char('|') + level);
    return result != 0.0 ? result : trend(jobName);
}

QString BJobDurationStats::formatDuration(int secs)
{
    if (secs <= 0) return QStringLiteral("-");
    int h = secs / 3600;
    int m = (secs % 3600) / 60;
    int s = secs % 60;
    if (h > 0) return QString("%1h %2m").arg(h).arg(m, 2, 10, QChar('0'));
    if (m > 0) return QString("%1m %2s").arg(m).arg(s, 2, 10, QChar('0'));
    return QString("%1s").arg(s);
}
