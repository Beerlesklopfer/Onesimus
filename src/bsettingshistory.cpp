#include "bsettingshistory.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool BSettingsHistory::setSetting(QSqlDatabase &db,
                                   const QString &resourceType,
                                   int resourceId,
                                   const QString &key,
                                   const QString &value,
                                   const QString &changedBy,
                                   const QString &reason)
{
    // Get previous value for audit trail
    QString previousValue = getSetting(db, resourceType, resourceId, key);

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO settings_history "
        "(resource_type, resource_id, setting_key, setting_value, changed_by, change_reason, previous_value, is_current) "
        "VALUES (:resource_type, :resource_id, :setting_key, :setting_value, :changed_by, :change_reason, :previous_value, 1)"
    );

    query.bindValue(":resource_type", resourceType);
    query.bindValue(":resource_id", resourceId);
    query.bindValue(":setting_key", key);
    query.bindValue(":setting_value", value);
    query.bindValue(":changed_by", changedBy);
    query.bindValue(":change_reason", reason.isEmpty() ? QVariant() : reason);
    query.bindValue(":previous_value", previousValue.isEmpty() ? QVariant() : previousValue);

    if (!query.exec()) {
        qWarning() << "Failed to set setting:" << query.lastError().text();
        return false;
    }

    return true;
}

QString BSettingsHistory::getSetting(QSqlDatabase &db,
                                      const QString &resourceType,
                                      int resourceId,
                                      const QString &key,
                                      const QString &defaultValue)
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT setting_value FROM current_settings "
        "WHERE resource_type = :resource_type "
        "  AND resource_id = :resource_id "
        "  AND setting_key = :setting_key"
    );

    query.bindValue(":resource_type", resourceType);
    query.bindValue(":resource_id", resourceId);
    query.bindValue(":setting_key", key);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return defaultValue;
}

QMap<QString, QString> BSettingsHistory::getAllSettings(QSqlDatabase &db,
                                                         const QString &resourceType,
                                                         int resourceId)
{
    QMap<QString, QString> settings;

    QSqlQuery query(db);
    query.prepare(
        "SELECT setting_key, setting_value FROM current_settings "
        "WHERE resource_type = :resource_type AND resource_id = :resource_id"
    );

    query.bindValue(":resource_type", resourceType);
    query.bindValue(":resource_id", resourceId);

    if (query.exec()) {
        while (query.next()) {
            QString key = query.value(0).toString();
            QString value = query.value(1).toString();
            settings[key] = value;
        }
    } else {
        qWarning() << "Failed to get all settings:" << query.lastError().text();
    }

    return settings;
}

QList<BSettingHistoryEntry> BSettingsHistory::getHistory(QSqlDatabase &db,
                                                          const QString &resourceType,
                                                          int resourceId,
                                                          const QString &key)
{
    QList<BSettingHistoryEntry> history;

    QSqlQuery query(db);
    query.prepare(
        "SELECT * FROM settings_history "
        "WHERE resource_type = :resource_type "
        "  AND resource_id = :resource_id "
        "  AND setting_key = :setting_key "
        "ORDER BY changed_at DESC"
    );

    query.bindValue(":resource_type", resourceType);
    query.bindValue(":resource_id", resourceId);
    query.bindValue(":setting_key", key);

    if (query.exec()) {
        while (query.next()) {
            history.append(readEntry(query));
        }
    } else {
        qWarning() << "Failed to get history:" << query.lastError().text();
    }

    return history;
}

QList<BSettingHistoryEntry> BSettingsHistory::getAllHistory(QSqlDatabase &db,
                                                             const QString &resourceType,
                                                             int resourceId)
{
    QList<BSettingHistoryEntry> history;

    QSqlQuery query(db);
    query.prepare(
        "SELECT * FROM settings_history "
        "WHERE resource_type = :resource_type AND resource_id = :resource_id "
        "ORDER BY changed_at DESC"
    );

    query.bindValue(":resource_type", resourceType);
    query.bindValue(":resource_id", resourceId);

    if (query.exec()) {
        while (query.next()) {
            history.append(readEntry(query));
        }
    } else {
        qWarning() << "Failed to get all history:" << query.lastError().text();
    }

    return history;
}

bool BSettingsHistory::rollbackSetting(QSqlDatabase &db,
                                        const QString &resourceType,
                                        int resourceId,
                                        const QString &key,
                                        const QString &changedBy,
                                        const QString &reason)
{
    // Get history (need at least 2 entries: current and previous)
    QList<BSettingHistoryEntry> history = getHistory(db, resourceType, resourceId, key);

    if (history.size() < 2) {
        qWarning() << "Cannot rollback: no previous value exists";
        return false;
    }

    // Previous value is the second entry (first is current)
    QString previousValue = history[1].settingValue;

    QString rollbackReason = reason.isEmpty()
        ? QString("Rollback to version %1").arg(history[1].version)
        : reason;

    return setSetting(db, resourceType, resourceId, key, previousValue, changedBy, rollbackReason);
}

bool BSettingsHistory::deleteSetting(QSqlDatabase &db,
                                      const QString &resourceType,
                                      int resourceId,
                                      const QString &key,
                                      const QString &changedBy,
                                      const QString &reason)
{
    QSqlQuery query(db);
    query.prepare(
        "UPDATE settings_history "
        "SET is_current = 0 "
        "WHERE resource_type = :resource_type "
        "  AND resource_id = :resource_id "
        "  AND setting_key = :setting_key "
        "  AND is_current = 1"
    );

    query.bindValue(":resource_type", resourceType);
    query.bindValue(":resource_id", resourceId);
    query.bindValue(":setting_key", key);

    if (!query.exec()) {
        qWarning() << "Failed to delete setting:" << query.lastError().text();
        return false;
    }

    // Log deletion in history
    query.prepare(
        "INSERT INTO settings_history "
        "(resource_type, resource_id, setting_key, setting_value, changed_by, change_reason, is_current) "
        "VALUES (:resource_type, :resource_id, :setting_key, NULL, :changed_by, :change_reason, 0)"
    );

    query.bindValue(":resource_type", resourceType);
    query.bindValue(":resource_id", resourceId);
    query.bindValue(":setting_key", key);
    query.bindValue(":changed_by", changedBy);
    query.bindValue(":change_reason", reason.isEmpty() ? "Setting deleted" : reason);

    return query.exec();
}

bool BSettingsHistory::deleteAllSettings(QSqlDatabase &db,
                                          const QString &resourceType,
                                          int resourceId)
{
    QSqlQuery query(db);
    query.prepare(
        "UPDATE settings_history "
        "SET is_current = 0 "
        "WHERE resource_type = :resource_type "
        "  AND resource_id = :resource_id "
        "  AND is_current = 1"
    );

    query.bindValue(":resource_type", resourceType);
    query.bindValue(":resource_id", resourceId);

    if (!query.exec()) {
        qWarning() << "Failed to delete all settings:" << query.lastError().text();
        return false;
    }

    return true;
}

QList<BSettingHistoryEntry> BSettingsHistory::getChangesByUser(QSqlDatabase &db,
                                                                const QString &changedBy,
                                                                int limit)
{
    QList<BSettingHistoryEntry> history;

    QSqlQuery query(db);
    QString sql = "SELECT * FROM settings_history "
                  "WHERE changed_by = :changed_by "
                  "ORDER BY changed_at DESC";

    if (limit > 0) {
        sql += QString(" LIMIT %1").arg(limit);
    }

    query.prepare(sql);
    query.bindValue(":changed_by", changedBy);

    if (query.exec()) {
        while (query.next()) {
            history.append(readEntry(query));
        }
    } else {
        qWarning() << "Failed to get changes by user:" << query.lastError().text();
    }

    return history;
}

QList<BSettingHistoryEntry> BSettingsHistory::getRecentChanges(QSqlDatabase &db, int limit)
{
    QList<BSettingHistoryEntry> history;

    QSqlQuery query(db);
    query.prepare(
        "SELECT * FROM settings_history "
        "ORDER BY changed_at DESC "
        "LIMIT :limit"
    );

    query.bindValue(":limit", limit);

    if (query.exec()) {
        while (query.next()) {
            history.append(readEntry(query));
        }
    } else {
        qWarning() << "Failed to get recent changes:" << query.lastError().text();
    }

    return history;
}

bool BSettingsHistory::hasSetting(QSqlDatabase &db,
                                   const QString &resourceType,
                                   int resourceId,
                                   const QString &key)
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT COUNT(*) FROM current_settings "
        "WHERE resource_type = :resource_type "
        "  AND resource_id = :resource_id "
        "  AND setting_key = :setting_key"
    );

    query.bindValue(":resource_type", resourceType);
    query.bindValue(":resource_id", resourceId);
    query.bindValue(":setting_key", key);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

int BSettingsHistory::getVersion(QSqlDatabase &db,
                                  const QString &resourceType,
                                  int resourceId,
                                  const QString &key)
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT version FROM current_settings "
        "WHERE resource_type = :resource_type "
        "  AND resource_id = :resource_id "
        "  AND setting_key = :setting_key"
    );

    query.bindValue(":resource_type", resourceType);
    query.bindValue(":resource_id", resourceId);
    query.bindValue(":setting_key", key);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

BSettingHistoryEntry BSettingsHistory::readEntry(QSqlQuery &query)
{
    BSettingHistoryEntry entry;
    entry.id = query.value("id").toInt();
    entry.resourceType = query.value("resource_type").toString();
    entry.resourceId = query.value("resource_id").toInt();
    entry.settingKey = query.value("setting_key").toString();
    entry.settingValue = query.value("setting_value").toString();
    entry.changedBy = query.value("changed_by").toString();
    entry.changedAt = query.value("changed_at").toDateTime();
    entry.isCurrent = query.value("is_current").toBool();
    entry.version = query.value("version").toInt();
    entry.changeReason = query.value("change_reason").toString();
    entry.previousValue = query.value("previous_value").toString();
    return entry;
}
