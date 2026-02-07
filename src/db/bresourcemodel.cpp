// Modified to fix QSqlDatabase reference passing
#include "db/bresourcemodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

BResourceModel::BResourceModel(QObject *parent,
                               QSqlDatabase db,
                               const QString &tableName,
                               const QString &resourceType)
    : QSqlTableModel(parent, db)
    , m_currentUser("system")
    , m_resourceType(resourceType)
{
    setTable(tableName);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void BResourceModel::setCurrentUser(const QString &username)
{
    m_currentUser = username;
}

void BResourceModel::setChangeReason(const QString &reason)
{
    m_changeReason = reason;
}

bool BResourceModel::submitAll()
{
    // Submit changes to main table
    bool success = QSqlTableModel::submitAll();

    if (success) {
        // Clear change reason after successful submit
        m_changeReason.clear();
    }

    return success;
}

bool BResourceModel::setCustomSetting(int resourceId,
                                       const QString &key,
                                       const QString &value,
                                       const QString &reason)
{
    QString oldValue = getCustomSetting(resourceId, key);

    QString effectiveReason = reason.isEmpty() ? m_changeReason : reason;

    QSqlDatabase db = database();
    bool success = BSettingsHistory::setSetting(
        db,
        m_resourceType,
        resourceId,
        key,
        value,
        m_currentUser,
        effectiveReason
    );

    if (success) {
        emit customSettingChanged(resourceId, key, oldValue, value, m_currentUser);
        emit resourceModified(resourceId, m_currentUser);
    }

    return success;
}

QString BResourceModel::getCustomSetting(int resourceId,
                                          const QString &key,
                                          const QString &defaultValue) const
{
    QSqlDatabase db = const_cast<BResourceModel*>(this)->database();
    return BSettingsHistory::getSetting(
        db,
        m_resourceType,
        resourceId,
        key,
        defaultValue
    );
}

QMap<QString, QString> BResourceModel::getCustomSettings(int resourceId, bool filterBySystem) const
{
    QSqlDatabase db = const_cast<BResourceModel*>(this)->database();
    QMap<QString, QString> allSettings = BSettingsHistory::getAllSettings(
        db,
        m_resourceType,
        resourceId
    );

    // If filtering disabled, return all settings
    if (!filterBySystem) {
        return allSettings;
    }

    // Get director's backup system
    QString directorSystem = getDirectorBackupSystem(resourceId);
    if (directorSystem.isEmpty()) {
        return allSettings;  // No filtering if system unknown
    }

    // Filter settings by compatibility
    QMap<QString, QString> filtered;
    QSqlQuery query(db);
    query.prepare(
        "SELECT setting_key, setting_value FROM current_settings "
        "WHERE resource_type = :type AND resource_id = :id "
        "AND (backup_system = :system OR backup_system = 'both')"
    );
    query.bindValue(":type", m_resourceType);
    query.bindValue(":id", resourceId);
    query.bindValue(":system", directorSystem);

    if (query.exec()) {
        while (query.next()) {
            QString key = query.value(0).toString();
            QString value = query.value(1).toString();
            filtered[key] = value;
        }
    }

    return filtered;
}

QString BResourceModel::getDirectorBackupSystem(int resourceId) const
{
    QSqlDatabase db = const_cast<BResourceModel*>(this)->database();
    QSqlQuery query(db);

    if (m_resourceType == "director") {
        // For Director, get own backup_system
        query.prepare("SELECT backup_system FROM directors WHERE id = :id");
        query.bindValue(":id", resourceId);
    } else if (m_resourceType == "client") {
        // For Client, get parent Director's backup_system
        query.prepare(
            "SELECT d.backup_system FROM directors d "
            "JOIN clients c ON c.director_id = d.id "
            "WHERE c.id = :id"
        );
        query.bindValue(":id", resourceId);
    } else if (m_resourceType == "storage") {
        // For Storage, get parent Director's backup_system
        query.prepare(
            "SELECT d.backup_system FROM directors d "
            "JOIN storages s ON s.director_id = d.id "
            "WHERE s.id = :id"
        );
        query.bindValue(":id", resourceId);
    } else if (m_resourceType == "console") {
        // For Console, get parent Director's backup_system
        query.prepare(
            "SELECT d.backup_system FROM directors d "
            "JOIN consoles c ON c.director_id = d.id "
            "WHERE c.id = :id"
        );
        query.bindValue(":id", resourceId);
    } else {
        return QString();  // Unknown resource type
    }

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return QString();  // Not found
}

bool BResourceModel::deleteCustomSetting(int resourceId,
                                          const QString &key,
                                          const QString &reason)
{
    QString effectiveReason = reason.isEmpty() ? m_changeReason : reason;

    QSqlDatabase db = database();
    return BSettingsHistory::deleteSetting(
        db,
        m_resourceType,
        resourceId,
        key,
        m_currentUser,
        effectiveReason
    );
}

QList<BSettingHistoryEntry> BResourceModel::getSettingsHistory(int resourceId,
                                                                const QString &key) const
{
    QSqlDatabase db = const_cast<BResourceModel*>(this)->database();
    if (key.isEmpty()) {
        return BSettingsHistory::getAllHistory(
            db,
            m_resourceType,
            resourceId
        );
    } else {
        return BSettingsHistory::getHistory(
            db,
            m_resourceType,
            resourceId,
            key
        );
    }
}
