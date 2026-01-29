#ifndef BSETTINGS_H
#define BSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QColor>

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

    QString connectionHost() const;
    void setConnectionHost(const QString& host);

    int connectionPort() const;
    void setConnectionPort(int port);

    QString connectionDirector() const;
    void setConnectionDirector(const QString& director);

    QString connectionConsole() const;
    void setConnectionConsole(const QString& console);

    QString connectionPassword() const;
    void setConnectionPassword(const QString& password);

    bool connectionSavePassword() const;
    void setConnectionSavePassword(bool save);

    bool connectionAutoConnect() const;
    void setConnectionAutoConnect(bool autoConnect);

    int connectionTimeout() const;
    void setConnectionTimeout(int seconds);

    // TLS Settings
    bool tlsEnabled() const;
    void setTlsEnabled(bool enabled);

    bool tlsUsePSK() const;
    void setTlsUsePSK(bool usePSK);

    QString tlsCaCertFile() const;
    void setTlsCaCertFile(const QString& path);

    QString tlsCertFile() const;
    void setTlsCertFile(const QString& path);

    QString tlsKeyFile() const;
    void setTlsKeyFile(const QString& path);

    QString tlsPfxFile() const;
    void setTlsPfxFile(const QString& path);

    bool tlsVerifyPeer() const;
    void setTlsVerifyPeer(bool verify);

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

    // ========================================================================
    // Behavior Settings
    // ========================================================================

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

    // ========================================================================
    // Utility Methods
    // ========================================================================

    /**
     * @brief Check if stored connection settings exist
     * @return true if connection settings are available
     */
    bool hasStoredConnection() const;

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
