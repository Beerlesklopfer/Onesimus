#include "bresourcemodels.h"
#include "blogging.h"
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
