#include "models/bresourcemodels.h"
#include "blogging.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

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
