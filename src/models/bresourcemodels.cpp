// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

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

QStringList BJobConfigModel::messagesNames() const
{
    QStringList names;

    auto collect = [&names](const QMap<QString, QJsonObject> &configs) {
        for (const QJsonObject &config : configs) {
            for (auto it = config.constBegin(); it != config.constEnd(); ++it) {
                if (it.key().compare(QLatin1String("messages"), Qt::CaseInsensitive) != 0)
                    continue;
                // A reference is either the name or an object with the name
                const QString name = it.value().isObject()
                    ? it.value().toObject().value("name").toString()
                    : it.value().toString();
                if (!name.isEmpty() && !names.contains(name))
                    names.append(name);
            }
        }
    };

    collect(m_jobConfigs);
    collect(m_jobDefsConfigs);
    names.sort(Qt::CaseInsensitive);
    return names;
}


// ============================================================================
// BScheduleEntry Implementation
// ============================================================================

QString BScheduleEntry::levelLabel() const
{
    return levelName.isEmpty() ? levelToString(level) : levelName;
}

std::optional<BRunSpec> BScheduleEntry::toSpec() const
{
    BRunSpec spec;

    // Level: none at all, or one Bareos knows
    const QString name = levelLabel();
    if (!name.isEmpty()) {
        const QString canonical = BRunParser::canonicalLevel(name);
        if (canonical.isEmpty())
            return std::nullopt;
        spec.overrides.level = canonical;
    }
    if (!pool.isEmpty()) spec.overrides.pool = pool;
    if (!storage.isEmpty()) spec.overrides.storage = storage;
    if (!messages.isEmpty()) spec.overrides.messages = messages;
    if (priority != 10) spec.overrides.priority = priority;

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
        return std::nullopt;
    spec.mask.hour = (1u << hour);
    spec.minute = minute;
    spec.haveAt = spec.haveHour = true;

    // Days: 0=Mon..6=Sun here, bit 0 = Sunday in Bareos. No day is every day.
    if (!daysOfWeek.isEmpty()) {
        quint8 wday = 0;
        for (int d : daysOfWeek) {
            if (d < 0 || d > 6)
                return std::nullopt;
            wday = quint8(wday | (1u << ((d + 1) % 7)));
        }
        spec.mask.wday = wday;
        spec.haveWday = true;
    }

    // Week of month: read by the parser, and nothing else may come out of it
    if (!weekSpec.isEmpty()) {
        const BRunLine week = BRunParser::parse(weekSpec + QStringLiteral(" at 0:00"));
        if (!week.spec || !week.spec->overrides.isEmpty())
            return std::nullopt;
        BDateTimeMask rest = week.spec->mask;
        rest.wom = BDateTimeMask::AllWoms;
        rest.last7DaysOfMonth = false;
        rest.hour = BDateTimeMask::AllHours;
        if (rest != BDateTimeMask::all())
            return std::nullopt;
        spec.mask.wom = week.spec->mask.wom;
        spec.mask.last7DaysOfMonth = week.spec->mask.last7DaysOfMonth;
        spec.haveWom = true;
    }

    return spec;
}

bool BScheduleEntry::matchesSource() const
{
    if (!source.spec)
        return false;
    const std::optional<BRunSpec> mine = toSpec();
    return mine && *mine == *source.spec;
}

BRunLine BScheduleEntry::toRunLine() const
{
    // An entry read from a Run line is written as that line, whatever its
    // fields say: they cannot express everything a line can, and a line
    // that is not understood is never rewritten. Only an entry made in the
    // application is written from its fields.
    if (notUnderstood() || source.spec)
        return source;

    BRunLine line;
    line.spec = toSpec();
    line.dirty = true;
    if (!line.spec) {
        line.parseError = QCoreApplication::translate(
            "BScheduleEntry", "The entry does not describe a run.");
    }
    return line;
}

QString BScheduleEntry::toRunDirective(QString *error) const
{
    return BRunWriter::lineText(toRunLine(), error);
}

BScheduleEntry BScheduleEntry::fromRunDirective(const QString &run)
{
    return fromRunLine(BRunParser::parse(run));
}

BScheduleEntry BScheduleEntry::fromRunLine(const BRunLine &line)
{
    BScheduleEntry entry;
    entry.source = line;
    if (!line.spec)
        return entry;   // not understood: no field is filled in

    const BRunSpec &spec = *line.spec;
    const BRunOverrides &o = spec.overrides;

    entry.levelName = o.level.value_or(QString());
    entry.level = levelFromString(entry.levelName);

    const QList<int> hours = spec.hours();
    entry.hour = hours.isEmpty() ? 0 : hours.first();
    entry.minute = spec.minute;

    // Bit 0 = Sunday in Bareos; 0=Mon..6=Sun here
    for (int d = 0; d < 7; ++d) {
        if (spec.mask.wday & (1u << ((d + 1) % 7)))
            entry.daysOfWeek.append(d);
    }
    entry.weekSpec = BRunWriter::weekOfMonthText(spec.mask);

    entry.pool = o.pool.value_or(QString());
    entry.storage = o.storage.value_or(QString());
    entry.messages = o.messages.value_or(QString());
    entry.priority = o.priority.value_or(10);

    return entry;
}

QList<BScheduleEntry> BScheduleEntry::fromOccurrences(const BSchedule &schedule,
                                                      const QList<BOccurrence> &occurrences)
{
    // Key: Run line, hour of the day. A line with several hours has one
    // minute for all of them and gives one entry per hour.
    QMap<QPair<int, int>, BScheduleEntry> byLineAndHour;

    for (const BOccurrence &occurrence : occurrences) {
        if (occurrence.run < 0 || occurrence.run >= schedule.runs.size())
            continue;
        const BRunLine &line = schedule.runs[occurrence.run];
        if (!line.spec)
            continue;

        const int hour = occurrence.when.time().hour();
        const QPair<int, int> key(occurrence.run, hour);
        auto it = byLineAndHour.find(key);
        if (it == byLineAndHour.end()) {
            BScheduleEntry entry = fromRunLine(line);
            entry.scheduleName = schedule.name;
            entry.runIndex = occurrence.run;
            entry.hour = hour;
            it = byLineAndHour.insert(key, entry);
        }
        it->occurrences.append(occurrence.when);
    }

    QList<BScheduleEntry> result = byLineAndHour.values();
    for (BScheduleEntry &entry : result)
        std::sort(entry.occurrences.begin(), entry.occurrences.end());
    return result;
}

bool BScheduleEntry::occursOn(const QDate &date) const
{
    for (const QDateTime &when : occurrences) {
        if (when.date() == date)
            return true;
    }
    return false;
}

bool BScheduleEntry::sharesDateWith(const BScheduleEntry &other) const
{
    for (const QDateTime &when : occurrences) {
        if (other.occursOn(when.date()))
            return true;
    }
    return false;
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
    const QString name = str.trimmed();
    for (BackupLevel level : {Full, Differential, Incremental, VirtualFull}) {
        if (name.compare(levelToString(level), Qt::CaseInsensitive) == 0)
            return level;
    }
    // No level, or one this enum has no value for: never a guess
    return None;
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
    rebuildSchedules();

    BLOG_DEBUG() << "BScheduleModel: Loaded" << m_scheduleConfigs.size()
                 << "schedules with" << runLineCount() << "Run lines";
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

BSchedule BScheduleModel::schedule(const QString &name) const
{
    return m_schedules.value(name);
}

int BScheduleModel::runLineCount() const
{
    int count = 0;
    for (const BSchedule &schedule : m_schedules)
        count += schedule.runs.size();
    return count;
}

QStringList BScheduleModel::runDirectives(const QString &scheduleName) const
{
    const QJsonObject schedule = m_scheduleConfigs.value(scheduleName);
    // Same keys and the same filter as rebuildSchedules()
    QJsonArray runs = schedule["run"].toArray();
    if (runs.isEmpty()) {
        runs = schedule["Run"].toArray();
    }

    QStringList lines;
    for (const QJsonValue &runVal : runs) {
        if (runVal.isString()) {
            lines.append(runVal.toString());
        }
    }
    return lines;
}

QString BScheduleModel::getDisplayText(const QJsonObject &item) const
{
    return item["name"].toString();
}

void BScheduleModel::rebuildSchedules()
{
    m_schedules.clear();
    m_runProblems.clear();

    for (auto it = m_scheduleConfigs.constBegin(); it != m_scheduleConfigs.constEnd(); ++it) {
        BSchedule schedule;
        schedule.name = it.key();
        schedule.description = it.value()["description"].toString();
        // Only an explicit "no" disables; the form the Director reports it
        // in is not known for certain, so both are read
        const QJsonValue enabled = it.value()["enabled"];
        if (enabled.isBool()) {
            schedule.enabled = enabled.toBool();
        } else if (enabled.isString()) {
            const QString text = enabled.toString().trimmed().toLower();
            schedule.enabled = !(text == QLatin1String("no") || text == QLatin1String("false")
                                 || text == QLatin1String("0"));
        }

        // One BRunLine per Run directive, in the Director's order: the
        // index of a line here is its index in runDirectives()
        const QStringList runs = runDirectives(schedule.name);
        for (int i = 0; i < runs.size(); ++i) {
            const BRunLine line = BRunParser::parse(runs[i]);
            schedule.runs.append(line);

#ifdef DEBUG_JSON
            BLOG_DEBUG() << "BScheduleModel: Schedule" << schedule.name
                         << "Run directive:" << runs[i];
#endif

            // A line that is not understood has no time and no day to show.
            // It is reported, not dropped and not shown as something else.
            if (!line.understood()) {
                BRunProblem problem;
                problem.scheduleName = schedule.name;
                problem.line = i + 1;
                problem.text = line.original;
                problem.error = line.parseError;
                m_runProblems.append(problem);
                BLOG_WARNING() << "BScheduleModel: Run line" << problem.line << "of schedule"
                               << schedule.name << "not understood:" << problem.text
                               << "-" << problem.error;
            }
        }

        m_schedules.insert(schedule.name, schedule);
    }
}
