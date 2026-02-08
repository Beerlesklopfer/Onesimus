#ifndef BSETTINGS_H
#define BSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QColor>
#include "bconnectionprofile.h"

/**
 * @brief Centralized settings management for Onesimus
 *
 * This singleton class provides a unified interface for all application settings.
 * It ensures consistent access to settings across all components and provides
 * type-safe getters/setters for all configuration values.
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 * @version 1.0.0
 */
class BSettings : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the singleton instance
     */
    static BSettings& instance();

    // Prevent copy and assignment
    BSettings(const BSettings&) = delete;
    BSettings& operator=(const BSettings&) = delete;

    // ========================================================================
    // Connection Settings
    // ========================================================================

    bool connectionSavePassword() const;
    void setConnectionSavePassword(bool save);

    bool connectionAutoConnect() const;
    void setConnectionAutoConnect(bool autoConnect);

    int connectionTimeout() const;
    void setConnectionTimeout(int seconds);

    // ========================================================================
    // Connection Profiles
    // ========================================================================

    /**
     * @brief Get all saved connection profiles
     * @return List of connection profiles
     */
    QList<BConnectionProfile> connectionProfiles() const;

    /**
     * @brief Save all connection profiles
     * @param profiles List of profiles to save
     */
    void setConnectionProfiles(const QList<BConnectionProfile> &profiles);

    /**
     * @brief Add a new connection profile
     * @param profile Profile to add
     */
    void addConnectionProfile(const BConnectionProfile &profile);

    /**
     * @brief Update an existing connection profile
     * @param profile Profile with updated data (matched by id)
     * @return true if profile was found and updated
     */
    bool updateConnectionProfile(const BConnectionProfile &profile);

    /**
     * @brief Remove a connection profile
     * @param profileId ID of the profile to remove
     * @return true if profile was found and removed
     */
    bool removeConnectionProfile(const QString &profileId);

    /**
     * @brief Get a connection profile by ID
     * @param profileId ID of the profile
     * @return Profile if found, invalid profile otherwise
     */
    BConnectionProfile connectionProfile(const QString &profileId) const;

    /**
     * @brief Get the last used connection profile ID
     * @return Profile ID or empty string if none
     */
    QString lastUsedProfileId() const;

    /**
     * @brief Set the last used connection profile ID
     * @param profileId Profile ID
     */
    void setLastUsedProfileId(const QString &profileId);

    /**
     * @brief Get the last used connection profile
     * @return Profile if found, invalid profile otherwise
     */
    BConnectionProfile lastUsedProfile() const;

    /**
     * @brief Migrate old single-connection settings to a profile
     *
     * Call this once to convert existing settings to the new profile system.
     * Creates a "Default" profile from the old settings if they exist.
     */
    void migrateOldConnectionSettings();

    // ========================================================================
    // Appearance Settings
    // ========================================================================

    QString appearanceTheme() const;
    void setAppearanceTheme(const QString& theme);

    int appearanceFontSize() const;
    void setAppearanceFontSize(int size);

    bool appearanceAnimations() const;
    void setAppearanceAnimations(bool enabled);

    bool appearanceCompactMode() const;
    void setAppearanceCompactMode(bool compact);

    QString appearanceLanguage() const;
    void setAppearanceLanguage(const QString& languageCode);
    bool hasLanguagePreference() const;

    // Level Colors
    QColor levelColor(const QString& level) const;
    void setLevelColor(const QString& level, const QColor& color);

    // Status Bar Colors
    QColor statusBarConnectedColor() const;
    void setStatusBarConnectedColor(const QColor& color);

    QColor statusBarDisconnectedColor() const;
    void setStatusBarDisconnectedColor(const QColor& color);

    // ========================================================================
    // Behavior Settings
    // ========================================================================

    /**
     * @brief Returns the list of visible backup level codes
     * @return QStringList of level codes (e.g., "F", "I", "D", "V")
     * @since 2.9
     */
    QStringList visibleLevels() const;

    /**
     * @brief Sets the list of visible backup level codes
     * @param levels List of level codes to show in UI
     * @since 2.9
     */
    void setVisibleLevels(const QStringList &levels);

    bool behaviorConfirmJobCancel() const;
    void setBehaviorConfirmJobCancel(bool confirm);

    bool behaviorConfirmJobStart() const;
    void setBehaviorConfirmJobStart(bool confirm);

    bool behaviorAutoRefresh() const;
    void setBehaviorAutoRefresh(bool enabled);

    int behaviorRefreshInterval() const;
    void setBehaviorRefreshInterval(int seconds);

    int behaviorMaxJobsDisplay() const;
    void setBehaviorMaxJobsDisplay(int maxJobs);

    bool behaviorJobsNewestFirst() const;
    void setBehaviorJobsNewestFirst(bool newestFirst);

    // ========================================================================
    // Advanced Settings
    // ========================================================================

    bool advancedDebugLogging() const;
    void setAdvancedDebugLogging(bool enabled);

    QString advancedLogFile() const;
    void setAdvancedLogFile(const QString& path);

    int advancedMaxLogSize() const;
    void setAdvancedMaxLogSize(int sizeMB);

    bool advancedEnableTooltips() const;
    void setAdvancedEnableTooltips(bool enabled);

    // ========================================================================
    // Widget State Settings (grouped by widget type)
    // ========================================================================

    // Jobs Widget State
    bool jobsStatisticsVisible() const;
    void setJobsStatisticsVisible(bool visible);

    int jobsRefreshInterval() const;
    void setJobsRefreshInterval(int milliseconds);

    bool jobsAutoRefresh() const;
    void setJobsAutoRefresh(bool enabled);

    // Jobs Pagination
    bool jobsPaginationEnabled() const;
    void setJobsPaginationEnabled(bool enabled);

    int jobsPaginationPageSize() const;
    void setJobsPaginationPageSize(int pageSize);

    // Jobs Widget Filters
    QMap<QString, bool> jobsFilterCheckboxes() const;
    void setJobsFilterCheckboxes(const QMap<QString, bool>& checkboxes);
    bool jobsFilterCheckbox(const QString& key, bool defaultValue = true) const;
    void setJobsFilterCheckbox(const QString& key, bool value);

    QMap<QString, int> jobsFilterComboboxes() const;
    void setJobsFilterComboboxes(const QMap<QString, int>& comboboxes);
    int jobsFilterCombobox(const QString& key, int defaultValue = 0) const;
    void setJobsFilterCombobox(const QString& key, int value);

    bool jobsFilterDateEnabled() const;
    void setJobsFilterDateEnabled(bool enabled);

    QDateTime jobsFilterDateFrom() const;
    void setJobsFilterDateFrom(const QDateTime& dateTime);

    QDateTime jobsFilterDateTo() const;
    void setJobsFilterDateTo(const QDateTime& dateTime);

    // Clients Widget State
    bool clientsShowOffline() const;
    void setClientsShowOffline(bool show);

    QString clientsLastFilter() const;
    void setClientsLastFilter(const QString& filter);

    // Storage Widget State
    bool storageShowAllVolumes() const;
    void setStorageShowAllVolumes(bool showAll);

    QString storageLastPool() const;
    void setStorageLastPool(const QString& pool);

    // Schedules Widget State
    QString schedulesLastSelected() const;
    void setSchedulesLastSelected(const QString& scheduleName);

    // Global Statistics Widget State
    bool statisticsWidgetVisible() const;
    void setStatisticsWidgetVisible(bool visible);

    // BVFS (Job Details Dialog) Settings
    /**
     * @brief Whether to show files from all related jobs by default
     * @return true to show all related jobs, false for current job only
     * @since 2.9
     */
    bool bvfsShowAllRelatedJobs() const;
    void setBvfsShowAllRelatedJobs(bool showAll);

    // ========================================================================
    // Main Window Settings
    // ========================================================================

    /**
     * @brief Get the main window splitter state
     * @return QByteArray with splitter state, empty if not saved
     * @since 2.10
     */
    QByteArray mainWindowSplitterState() const;
    void setMainWindowSplitterState(const QByteArray& state);

    /**
     * @brief Get the lower panel (Messages) visibility
     * @return true if lower panel should be visible
     * @since 2.10
     */
    bool lowerPanelVisible() const;
    void setLowerPanelVisible(bool visible);

    // ========================================================================
    // Messages Widget Settings
    // ========================================================================

    /**
     * @brief Get the message poll interval in milliseconds
     * @return Poll interval in ms (default: 30000 = 30 seconds)
     * @since 2.10
     */
    int messagesPollInterval() const;
    void setMessagesPollInterval(int intervalMs);

    /**
     * @brief Get the maximum message history count
     * @return Maximum number of messages to keep (default: 1000, 0 = unlimited)
     * @since 2.10
     */
    int messagesMaxHistory() const;
    void setMessagesMaxHistory(int maxMessages);

    // ========================================================================
    // Utility Methods
    // ========================================================================

    /**
     * @brief Reset all settings to defaults
     */
    void resetToDefaults();

    /**
     * @brief Force sync settings to disk
     */
    void sync();

signals:
    /**
     * @brief Emitted when any setting changes
     * @param key The setting key that changed
     * @param value The new value
     */
    void settingChanged(const QString& key, const QVariant& value);

    /**
     * @brief Emitted when connection settings change
     */
    void connectionSettingsChanged();

    /**
     * @brief Emitted when appearance settings change
     */
    void appearanceSettingsChanged();

    /**
     * @brief Emitted when behavior settings change
     */
    void behaviorSettingsChanged();

    /**
     * @brief Emitted when auto-refresh settings change
     */
    void autoRefreshSettingsChanged(bool enabled, int intervalSeconds);

private:
    BSettings();
    ~BSettings();

    QSettings m_settings;

    // Helper to emit signals when settings change
    void setValue(const QString& key, const QVariant& value);
    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;
};

#endif // BSETTINGS_H
