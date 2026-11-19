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
 * @author Joerg Bernau <support@onesimus.io>
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

    /// Interface zoom in percent (BUiScale): 70 to 250 in steps of 10
    int appearanceUiZoom() const;
    void setAppearanceUiZoom(int percent);

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

    /**
     * @brief Age in hours from which a client's last backup counts as stale
     *
     * Default 24. Used by the clients table and the client details dialog.
     */
    int behaviorClientStaleHours() const;
    void setBehaviorClientStaleHours(int hours);

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

    // Capability notices ("not available on this Director"), one flag per
    // function id of BCapabilities; default false = the notice is shown
    bool capabilityNoticeHidden(const QString& functionId) const;
    void setCapabilityNoticeHidden(const QString& functionId, bool hidden);

    // Schedules Widget State
    QString schedulesLastSelected() const;
    void setSchedulesLastSelected(const QString& scheduleName);

    int schedulesViewMode() const;
    void setSchedulesViewMode(int index);

    int schedulesDayViewMode() const;
    void setSchedulesDayViewMode(int index);

    int schedulesCurrentDay() const;
    void setSchedulesCurrentDay(int day);

    int schedulesSnapIndex() const;
    void setSchedulesSnapIndex(int index);
    /// The list of predicted waits below the timeline is open
    bool schedulesWaitListShown() const;
    void setSchedulesWaitListShown(bool shown);

    /// Header of the client table: column order, widths, hidden columns and
    /// sort column (QHeaderView::saveState()), plus the interface scale the
    /// widths were saved at, so they follow a later interface zoom
    QByteArray clientsHeaderState() const;
    void setClientsHeaderState(const QByteArray& state);
    int clientsHeaderScale() const;
    void setClientsHeaderScale(int pixelsPer1000);

    /// Header of the jobs table, the same way (Widgets/Jobs/header_state).
    /// The single source of the table's live layout; named view presets
    /// (BViewPresets) are snapshots that are applied to it.
    QByteArray jobsHeaderState() const;
    void setJobsHeaderState(const QByteArray& state);
    int jobsHeaderScale() const;
    void setJobsHeaderScale(int pixelsPer1000);

    /// Header of the table of Job and JobDefs resources (Job Definitions
    /// tab), the same way (Widgets/JobDefinitions/header_state), and the
    /// splitter between the table and the detail panel
    QByteArray jobDefinitionsHeaderState() const;
    void setJobDefinitionsHeaderState(const QByteArray& state);
    int jobDefinitionsHeaderScale() const;
    void setJobDefinitionsHeaderScale(int pixelsPer1000);
    QByteArray jobDefinitionsSplitterState() const;
    void setJobDefinitionsSplitterState(const QByteArray& state);

    QByteArray schedulesSplitterState() const;
    void setSchedulesSplitterState(const QByteArray& state);

    QByteArray schedulesGanttSplitterState() const;
    void setSchedulesGanttSplitterState(const QByteArray& state);

    bool schedulesFilterCheckbox(const QString& key, bool defaultValue = true) const;
    void setSchedulesFilterCheckbox(const QString& key, bool value);

    /// Job history of the schedule view: how many days back the measured
    /// durations and waits are taken from (default 60, 1 to 365)
    int schedulesHistoryDays() const;
    void setSchedulesHistoryDays(int days);

    /// Job history of the schedule view: most rows asked for in one request
    /// (default 5000, 100 to 100000). No field in the settings dialog.
    int schedulesHistoryRowLimit() const;
    void setSchedulesHistoryRowLimit(int rows);

    /// Wait prediction: how many jobs the devices of a storage write at a
    /// time (Maximum Concurrent Jobs of the storage daemon's Device, which
    /// the Director cannot read), per Director profile and storage. 0: not
    /// known (the default); 1 to 1000 otherwise.
    int schedulesDeviceWriters(const QString& profileId, const QString& storage) const;
    void setSchedulesDeviceWriters(const QString& profileId, const QString& storage, int writers);

    /// 3-2-1 verifier: the data on the client counts as one of the three
    /// copies (default on: production data + 2 backups)
    bool verify321CountOriginalData() const;
    void setVerify321CountOriginalData(bool count);

    /// 3-2-1 verifier: the user marked @p storage of a Director profile as
    /// off site (Bareos itself has no such flag). Default false.
    bool verify321OffSite(const QString& profileId, const QString& storage) const;
    void setVerify321OffSite(const QString& profileId, const QString& storage, bool offSite);

    /// 3-2-1 verifier: the user said that the storages of a profile that are
    /// not marked off site are on site. Default false: they are unknown.
    bool verify321UnmarkedOnSite(const QString& profileId) const;
    void setVerify321UnmarkedOnSite(const QString& profileId, bool onSite);

    /// 3-2-1 verifier: header of its table (QHeaderView::saveState()) and
    /// the interface scale the widths were saved at (BHeaderState)
    QByteArray verify321HeaderState() const;
    void setVerify321HeaderState(const QByteArray& state);
    int verify321HeaderScale() const;
    void setVerify321HeaderScale(int pixelsPer1000);

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
    int mainWindowActiveTab() const;
    void setMainWindowActiveTab(int index);

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
