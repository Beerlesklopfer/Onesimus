#ifndef BRESOURCEMODEL_H
#define BRESOURCEMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QString>
#include <QVariant>
#include "bsettingshistory.h"

/**
 * @brief Base model for Bareos resources with settings history integration
 *
 * Extends QSqlTableModel to provide:
 * - Automatic settings_history tracking for custom config values
 * - User attribution for changes
 * - Change reason documentation
 * - Integration with BSettingsHistory
 *
 * Subclasses: BDirectorModel, BClientModel, BStorageModel, BConsoleModel
 *
 * @since 0.1.0.5
 */
class BResourceModel : public QSqlTableModel
{
    Q_OBJECT

public:
    /**
     * @brief Construct resource model
     *
     * @param parent Parent QObject
     * @param db Database connection
     * @param tableName SQL table name (e.g., "directors", "clients")
     * @param resourceType Resource type for settings_history (e.g., "director", "client")
     */
    explicit BResourceModel(QObject *parent,
                           QSqlDatabase db,
                           const QString &tableName,
                           const QString &resourceType);

    /**
     * @brief Set current username for change attribution
     *
     * All subsequent changes will be attributed to this user.
     *
     * @param username Username or system identifier
     */
    void setCurrentUser(const QString &username);

    /**
     * @brief Get current username
     * @return Current username for change attribution
     */
    QString currentUser() const { return m_currentUser; }

    /**
     * @brief Set change reason for next operation
     *
     * The reason will be used for the next insert/update/delete operation,
     * then automatically cleared.
     *
     * @param reason Reason for change
     */
    void setChangeReason(const QString &reason);

    /**
     * @brief Get resource type
     * @return Resource type string (e.g., "director", "client")
     */
    QString resourceType() const { return m_resourceType; }

    /**
     * @brief Submit all changes to database
     *
     * Overrides QSqlTableModel::submitAll() to add history tracking.
     *
     * @return true if successful
     */
    bool submitAll();

    /**
     * @brief Set custom setting value for a resource
     *
     * Stores setting in settings_history table with user attribution.
     *
     * @param resourceId Resource ID (primary key)
     * @param key Setting key
     * @param value Setting value
     * @param reason Optional reason for change
     * @return true if successful
     */
    bool setCustomSetting(int resourceId,
                         const QString &key,
                         const QString &value,
                         const QString &reason = QString());

    /**
     * @brief Get custom setting value
     *
     * @param resourceId Resource ID
     * @param key Setting key
     * @param defaultValue Default value if not found
     * @return Setting value
     */
    QString getCustomSetting(int resourceId,
                            const QString &key,
                            const QString &defaultValue = QString()) const;

    /**
     * @brief Get all custom settings for a resource
     *
     * @param resourceId Resource ID
     * @param filterBySystem If true, only returns settings compatible with resource's backup system
     * @return Map of setting key -> value
     */
    QMap<QString, QString> getCustomSettings(int resourceId, bool filterBySystem = true) const;

    /**
     * @brief Get backup system for a resource's director
     *
     * For Director resources, returns own backup_system.
     * For Client/Storage/Console, returns parent Director's backup_system.
     *
     * @param resourceId Resource ID
     * @return "bareos", "bacula", or empty string if not found
     */
    QString getDirectorBackupSystem(int resourceId) const;

    /**
     * @brief Delete custom setting
     *
     * @param resourceId Resource ID
     * @param key Setting key
     * @param reason Optional reason for deletion
     * @return true if successful
     */
    bool deleteCustomSetting(int resourceId,
                            const QString &key,
                            const QString &reason = QString());

    /**
     * @brief Get settings history for a resource
     *
     * @param resourceId Resource ID
     * @param key Optional setting key (if empty, returns all settings history)
     * @return List of history entries
     */
    QList<BSettingHistoryEntry> getSettingsHistory(int resourceId,
                                                   const QString &key = QString()) const;

signals:
    /**
     * @brief Emitted when a custom setting is changed
     *
     * @param resourceId Resource ID
     * @param key Setting key
     * @param oldValue Previous value
     * @param newValue New value
     * @param changedBy Username who made the change
     */
    void customSettingChanged(int resourceId,
                             const QString &key,
                             const QString &oldValue,
                             const QString &newValue,
                             const QString &changedBy);

    /**
     * @brief Emitted when resource data is modified
     *
     * @param resourceId Resource ID
     * @param changedBy Username who made the change
     */
    void resourceModified(int resourceId, const QString &changedBy);

protected:
    QString m_currentUser;      ///< Current username for change attribution
    QString m_changeReason;     ///< Reason for next change (auto-cleared after use)
    QString m_resourceType;     ///< Resource type ("director", "client", etc.)
};

#endif // BRESOURCEMODEL_H
