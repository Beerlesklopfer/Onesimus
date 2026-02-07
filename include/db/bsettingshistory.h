#ifndef BSETTINGSHISTORY_H
#define BSETTINGSHISTORY_H

#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QSqlDatabase>
#include <QList>

/**
 * @brief Entry in settings history
 */
struct BSettingHistoryEntry
{
    int id;
    QString resourceType;       // 'director', 'client', 'storage', 'console'
    int resourceId;
    QString settingKey;
    QString settingValue;
    QString changedBy;
    QDateTime changedAt;
    bool isCurrent;
    int version;
    QString changeReason;
    QString previousValue;
    QString backupSystem;       // 'bareos', 'bacula', or 'both'
};

/**
 * @brief Utility class for managing settings history
 *
 * Provides convenient methods for setting, getting, and tracking configuration
 * changes across all resource types (Directors, Clients, Storages, Consoles).
 *
 * Features:
 * - Automatic history tracking
 * - User attribution (who changed what)
 * - Timestamp tracking (when changes occurred)
 * - Full audit trail
 * - Easy rollback to previous values
 * - Change reason documentation
 *
 * @since 0.1.0.5
 */
class BSettingsHistory
{
public:
    /**
     * @brief Set a setting value (creates history entry)
     *
     * Automatically marks previous value as historical and creates new current value.
     *
     * @param db Database connection
     * @param resourceType Resource type ('director', 'client', 'storage', 'console')
     * @param resourceId Resource ID
     * @param key Setting key
     * @param value Setting value
     * @param changedBy Username or system identifier
     * @param reason Optional reason for change
     * @param backupSystem Backup system ('bareos', 'bacula', or 'both')
     * @return true if successful
     *
     * @par Example:
     * @code
     * BSettingsHistory::setSetting(db, "director", 1, "max_jobs", "50", "admin", "Increased capacity", "bareos");
     * @endcode
     */
    static bool setSetting(QSqlDatabase &db,
                          const QString &resourceType,
                          int resourceId,
                          const QString &key,
                          const QString &value,
                          const QString &changedBy,
                          const QString &reason = QString(),
                          const QString &backupSystem = "both");

    /**
     * @brief Get current setting value
     *
     * @param db Database connection
     * @param resourceType Resource type
     * @param resourceId Resource ID
     * @param key Setting key
     * @param defaultValue Default value if setting not found
     * @return Current setting value, or defaultValue if not found
     *
     * @par Example:
     * @code
     * QString maxJobs = BSettingsHistory::getSetting(db, "director", 1, "max_jobs", "20");
     * @endcode
     */
    static QString getSetting(QSqlDatabase &db,
                             const QString &resourceType,
                             int resourceId,
                             const QString &key,
                             const QString &defaultValue = QString());

    /**
     * @brief Get all current settings for a resource
     *
     * @param db Database connection
     * @param resourceType Resource type
     * @param resourceId Resource ID
     * @return Map of setting key -> value
     *
     * @par Example:
     * @code
     * QMap<QString, QString> settings = BSettingsHistory::getAllSettings(db, "director", 1);
     * QString maxJobs = settings.value("max_jobs", "20");
     * @endcode
     */
    static QMap<QString, QString> getAllSettings(QSqlDatabase &db,
                                                  const QString &resourceType,
                                                  int resourceId);

    /**
     * @brief Get full history for a setting
     *
     * Returns all historical values in reverse chronological order (newest first).
     *
     * @param db Database connection
     * @param resourceType Resource type
     * @param resourceId Resource ID
     * @param key Setting key
     * @return List of history entries (newest first)
     *
     * @par Example:
     * @code
     * QList<BSettingHistoryEntry> history = BSettingsHistory::getHistory(db, "director", 1, "max_jobs");
     * for (const auto &entry : history) {
     *     qDebug() << entry.changedAt << entry.changedBy << ":" << entry.settingValue;
     * }
     * @endcode
     */
    static QList<BSettingHistoryEntry> getHistory(QSqlDatabase &db,
                                                   const QString &resourceType,
                                                   int resourceId,
                                                   const QString &key);

    /**
     * @brief Get all history for a resource
     *
     * Returns all settings history for a resource.
     *
     * @param db Database connection
     * @param resourceType Resource type
     * @param resourceId Resource ID
     * @return List of all history entries for this resource
     */
    static QList<BSettingHistoryEntry> getAllHistory(QSqlDatabase &db,
                                                      const QString &resourceType,
                                                      int resourceId);

    /**
     * @brief Rollback setting to previous value
     *
     * Restores the previous value by creating a new history entry with the old value.
     *
     * @param db Database connection
     * @param resourceType Resource type
     * @param resourceId Resource ID
     * @param key Setting key
     * @param changedBy Username performing rollback
     * @param reason Reason for rollback
     * @return true if successful
     *
     * @par Example:
     * @code
     * BSettingsHistory::rollbackSetting(db, "director", 1, "max_jobs", "admin", "Reverted capacity increase");
     * @endcode
     */
    static bool rollbackSetting(QSqlDatabase &db,
                               const QString &resourceType,
                               int resourceId,
                               const QString &key,
                               const QString &changedBy,
                               const QString &reason = QString());

    /**
     * @brief Delete a setting (marks as deleted, preserves history)
     *
     * Marks current value as historical without setting a new current value.
     *
     * @param db Database connection
     * @param resourceType Resource type
     * @param resourceId Resource ID
     * @param key Setting key
     * @param changedBy Username performing deletion
     * @param reason Reason for deletion
     * @return true if successful
     */
    static bool deleteSetting(QSqlDatabase &db,
                             const QString &resourceType,
                             int resourceId,
                             const QString &key,
                             const QString &changedBy,
                             const QString &reason = QString());

    /**
     * @brief Delete all settings for a resource
     *
     * @param db Database connection
     * @param resourceType Resource type
     * @param resourceId Resource ID
     * @return true if successful
     */
    static bool deleteAllSettings(QSqlDatabase &db,
                                  const QString &resourceType,
                                  int resourceId);

    /**
     * @brief Get changes made by a specific user
     *
     * @param db Database connection
     * @param changedBy Username
     * @param limit Maximum number of entries to return (0 = no limit)
     * @return List of history entries
     */
    static QList<BSettingHistoryEntry> getChangesByUser(QSqlDatabase &db,
                                                        const QString &changedBy,
                                                        int limit = 100);

    /**
     * @brief Get recent changes across all resources
     *
     * @param db Database connection
     * @param limit Maximum number of entries to return
     * @return List of recent history entries (newest first)
     */
    static QList<BSettingHistoryEntry> getRecentChanges(QSqlDatabase &db,
                                                        int limit = 100);

    /**
     * @brief Check if a setting exists
     *
     * @param db Database connection
     * @param resourceType Resource type
     * @param resourceId Resource ID
     * @param key Setting key
     * @return true if setting exists (has current value)
     */
    static bool hasSetting(QSqlDatabase &db,
                          const QString &resourceType,
                          int resourceId,
                          const QString &key);

    /**
     * @brief Get current version number for a setting
     *
     * @param db Database connection
     * @param resourceType Resource type
     * @param resourceId Resource ID
     * @param key Setting key
     * @return Version number (1-based), or 0 if not found
     */
    static int getVersion(QSqlDatabase &db,
                         const QString &resourceType,
                         int resourceId,
                         const QString &key);

private:
    /**
     * @brief Internal helper to read history entry from query
     */
    static BSettingHistoryEntry readEntry(QSqlQuery &query);
};

#endif // BSETTINGSHISTORY_H
