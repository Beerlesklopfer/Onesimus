#include "bsettings.h"
#include <QDebug>
#include <QDateTime>

// ============================================================================
// Singleton Implementation
// ============================================================================

BSettings& BSettings::instance()
{
    static BSettings instance;
    return instance;
}

BSettings::BSettings()
    : QObject(nullptr)
#if USE_BACULA
    , m_settings("Bacula", "Onesimus")  // Bacula organization name
#elif defined(USE_BAREOS)
    , m_settings("Bareos", "Onesimus")  // Bareos organization name
#else
    , m_settings("Bareos", "Onesimus")  // Default to Bareos
#endif
{
#ifdef IS_DEVELOPER
    qDebug() << "BSettings: Initializing settings";
    qDebug() << "  Settings file:" << m_settings.fileName();
#endif
}

BSettings::~BSettings()
{
    sync();
}

// ============================================================================
// Helper Methods
// ============================================================================

void BSettings::setValue(const QString& key, const QVariant& value)
{
    m_settings.setValue(key, value);
    emit settingChanged(key, value);
}

QVariant BSettings::value(const QString& key, const QVariant& defaultValue) const
{
    return m_settings.value(key, defaultValue);
}

// ============================================================================
// Connection Settings
// ============================================================================

QString BSettings::connectionHost() const
{
    return value("Connection/host", "localhost").toString();
}

void BSettings::setConnectionHost(const QString& host)
{
    setValue("Connection/host", host);
    emit connectionSettingsChanged();
}

int BSettings::connectionPort() const
{
    return value("Connection/port", 9101).toInt();
}

void BSettings::setConnectionPort(int port)
{
    setValue("Connection/port", port);
    emit connectionSettingsChanged();
}

QString BSettings::connectionDirector() const
{
    return value("Connection/director", "bareos-dir").toString();
}

void BSettings::setConnectionDirector(const QString& director)
{
    setValue("Connection/director", director);
    emit connectionSettingsChanged();
}

QString BSettings::connectionConsole() const
{
    return value("Connection/console", "onesimus").toString();
}

void BSettings::setConnectionConsole(const QString& console)
{
    setValue("Connection/console", console);
    emit connectionSettingsChanged();
}

QString BSettings::connectionPassword() const
{
    return value("Connection/password").toString();
}

void BSettings::setConnectionPassword(const QString& password)
{
    if (connectionSavePassword()) {
        setValue("Connection/password", password);
    } else {
        m_settings.remove("Connection/password");
    }
    emit connectionSettingsChanged();
}

bool BSettings::connectionSavePassword() const
{
    return value("Connection/save_password", true).toBool();
}

void BSettings::setConnectionSavePassword(bool save)
{
    setValue("Connection/save_password", save);
    if (!save) {
        m_settings.remove("Connection/password");
    }
    emit connectionSettingsChanged();
}

bool BSettings::connectionAutoConnect() const
{
    return value("Connection/auto_connect", false).toBool();
}

void BSettings::setConnectionAutoConnect(bool autoConnect)
{
    setValue("Connection/auto_connect", autoConnect);
    emit connectionSettingsChanged();
}

int BSettings::connectionTimeout() const
{
    return value("Connection/connection_timeout", 30).toInt();
}

void BSettings::setConnectionTimeout(int seconds)
{
    setValue("Connection/connection_timeout", seconds);
    emit connectionSettingsChanged();
}

// ========================================================================
// TLS Settings
// ========================================================================

bool BSettings::tlsEnabled() const
{
    return value("Connection/tls_enabled", true).toBool();
}

void BSettings::setTlsEnabled(bool enabled)
{
    setValue("Connection/tls_enabled", enabled);
    emit connectionSettingsChanged();
}

bool BSettings::tlsUsePSK() const
{
    return value("Connection/tls_use_psk", true).toBool();
}

void BSettings::setTlsUsePSK(bool usePSK)
{
    setValue("Connection/tls_use_psk", usePSK);
    emit connectionSettingsChanged();
}

QString BSettings::tlsCaCertFile() const
{
    return value("Connection/tls_ca_cert_file").toString();
}

void BSettings::setTlsCaCertFile(const QString& path)
{
    setValue("Connection/tls_ca_cert_file", path);
    emit connectionSettingsChanged();
}

QString BSettings::tlsCertFile() const
{
    return value("Connection/tls_cert_file").toString();
}

void BSettings::setTlsCertFile(const QString& path)
{
    setValue("Connection/tls_cert_file", path);
    emit connectionSettingsChanged();
}

QString BSettings::tlsKeyFile() const
{
    return value("Connection/tls_key_file").toString();
}

void BSettings::setTlsKeyFile(const QString& path)
{
    setValue("Connection/tls_key_file", path);
    emit connectionSettingsChanged();
}

QString BSettings::tlsPfxFile() const
{
    return value("Connection/tls_pfx_file").toString();
}

void BSettings::setTlsPfxFile(const QString& path)
{
    setValue("Connection/tls_pfx_file", path);
    emit connectionSettingsChanged();
}

bool BSettings::tlsVerifyPeer() const
{
    return value("Connection/tls_verify_peer", true).toBool();
}

void BSettings::setTlsVerifyPeer(bool verify)
{
    setValue("Connection/tls_verify_peer", verify);
    emit connectionSettingsChanged();
}

// ========================================================================
// Appearance Settings
// ========================================================================

QString BSettings::appearanceTheme() const
{
    return value("Appearance/theme", "dark").toString();
}

void BSettings::setAppearanceTheme(const QString& theme)
{
    setValue("Appearance/theme", theme);
    emit appearanceSettingsChanged();
}

int BSettings::appearanceFontSize() const
{
    return value("Appearance/font_size", 10).toInt();
}

void BSettings::setAppearanceFontSize(int size)
{
    setValue("Appearance/font_size", size);
    emit appearanceSettingsChanged();
}

bool BSettings::appearanceAnimations() const
{
    return value("Appearance/animations", true).toBool();
}

void BSettings::setAppearanceAnimations(bool enabled)
{
    setValue("Appearance/animations", enabled);
    emit appearanceSettingsChanged();
}

bool BSettings::appearanceCompactMode() const
{
    return value("Appearance/compact_mode", false).toBool();
}

void BSettings::setAppearanceCompactMode(bool compact)
{
    setValue("Appearance/compact_mode", compact);
    emit appearanceSettingsChanged();
}

QString BSettings::appearanceLanguage() const
{
    return value("Appearance/language", "en").toString();
}

void BSettings::setAppearanceLanguage(const QString& languageCode)
{
    setValue("Appearance/language", languageCode);
    emit appearanceSettingsChanged();
}

bool BSettings::hasLanguagePreference() const
{
    return m_settings.contains("Appearance/language");
}

QColor BSettings::levelColor(const QString& level) const
{
    // Default colors for each level
    QColor defaultColor;
    if (level == "F") {
        defaultColor = QColor(200, 220, 255);  // Light blue - Full backup
    } else if (level == "I") {
        defaultColor = QColor(200, 255, 200);  // Light green - Incremental
    } else if (level == "D") {
        defaultColor = QColor(255, 240, 200);  // Light orange - Differential
    } else if (level == "V") {
        defaultColor = QColor(230, 200, 255);  // Light purple - Virtual Full
    } else {
        return QColor();  // Invalid color for unknown levels
    }

    QString key = QString("Appearance/level_color_%1").arg(level);
    QString colorString = value(key, defaultColor.name()).toString();
    return QColor(colorString);
}

void BSettings::setLevelColor(const QString& level, const QColor& color)
{
    QString key = QString("Appearance/level_color_%1").arg(level);
    setValue(key, color.name());
    emit appearanceSettingsChanged();
}

// ========================================================================
// Behavior Settings
// ========================================================================

bool BSettings::behaviorConfirmJobCancel() const
{
    return value("Behavior/confirm_job_cancel", true).toBool();
}

void BSettings::setBehaviorConfirmJobCancel(bool confirm)
{
    setValue("Behavior/confirm_job_cancel", confirm);
    emit behaviorSettingsChanged();
}

bool BSettings::behaviorConfirmJobStart() const
{
    return value("Behavior/confirm_job_start", true).toBool();
}

void BSettings::setBehaviorConfirmJobStart(bool confirm)
{
    setValue("Behavior/confirm_job_start", confirm);
    emit behaviorSettingsChanged();
}

bool BSettings::behaviorAutoRefresh() const
{
    return value("Behavior/auto_refresh", false).toBool();
}

void BSettings::setBehaviorAutoRefresh(bool enabled)
{
    setValue("Behavior/auto_refresh", enabled);
    int interval = behaviorRefreshInterval();
    emit behaviorSettingsChanged();
    emit autoRefreshSettingsChanged(enabled, interval);
}

int BSettings::behaviorRefreshInterval() const
{
    return value("Behavior/refresh_interval", 30).toInt();
}

void BSettings::setBehaviorRefreshInterval(int seconds)
{
    setValue("Behavior/refresh_interval", seconds);
    emit behaviorSettingsChanged();
    if (behaviorAutoRefresh()) {
        emit autoRefreshSettingsChanged(true, seconds);
    }
}

int BSettings::behaviorMaxJobsDisplay() const
{
    return value("Behavior/max_jobs_display", 100).toInt();
}

void BSettings::setBehaviorMaxJobsDisplay(int maxJobs)
{
    setValue("Behavior/max_jobs_display", maxJobs);
    emit behaviorSettingsChanged();
}

// ========================================================================
// Advanced Settings
// ========================================================================

bool BSettings::advancedDebugLogging() const
{
    return value("Advanced/debug_logging", false).toBool();
}

void BSettings::setAdvancedDebugLogging(bool enabled)
{
    setValue("Advanced/debug_logging", enabled);
}

QString BSettings::advancedLogFile() const
{
    return value("Advanced/log_file", "onesimus.log").toString();
}

void BSettings::setAdvancedLogFile(const QString& path)
{
    setValue("Advanced/log_file", path);
}

int BSettings::advancedMaxLogSize() const
{
    return value("Advanced/max_log_size", 10).toInt();
}

void BSettings::setAdvancedMaxLogSize(int sizeMB)
{
    setValue("Advanced/max_log_size", sizeMB);
}

bool BSettings::advancedEnableTooltips() const
{
    return value("Advanced/enable_tooltips", true).toBool();
}

void BSettings::setAdvancedEnableTooltips(bool enabled)
{
    setValue("Advanced/enable_tooltips", enabled);
}

// ========================================================================
// Widget State Settings (grouped by widget type)
// ========================================================================

// Jobs Widget State
bool BSettings::jobsStatisticsVisible() const
{
    return value("Widgets/Jobs/statistics_visible", true).toBool();
}

void BSettings::setJobsStatisticsVisible(bool visible)
{
    setValue("Widgets/Jobs/statistics_visible", visible);
}

int BSettings::jobsRefreshInterval() const
{
    return value("Widgets/Jobs/refresh_interval", 10000).toInt();
}

void BSettings::setJobsRefreshInterval(int milliseconds)
{
    setValue("Widgets/Jobs/refresh_interval", milliseconds);
}

bool BSettings::jobsAutoRefresh() const
{
    return value("Widgets/Jobs/auto_refresh", false).toBool();
}

void BSettings::setJobsAutoRefresh(bool enabled)
{
    setValue("Widgets/Jobs/auto_refresh", enabled);
}

// Jobs Widget Filters
QMap<QString, bool> BSettings::jobsFilterCheckboxes() const
{
    QMap<QString, bool> checkboxes;

    // Load all saved checkboxes from settings
    // Note: We can't use beginGroup/endGroup in const methods
    // So we'll iterate through all keys and filter for our prefix
    QStringList allKeys = m_settings.allKeys();
    QString prefix = "Widgets/Jobs/FilterCheckboxes/";

    for (const QString &key : allKeys) {
        if (key.startsWith(prefix)) {
            QString shortKey = key.mid(prefix.length());
            checkboxes[shortKey] = m_settings.value(key, true).toBool();
        }
    }

    return checkboxes;
}

void BSettings::setJobsFilterCheckboxes(const QMap<QString, bool>& checkboxes)
{
    // Clear existing checkboxes
    m_settings.beginGroup("Widgets/Jobs/FilterCheckboxes");
    m_settings.remove("");  // Remove all keys in this group
    m_settings.endGroup();

    // Save all checkboxes
    for (auto it = checkboxes.constBegin(); it != checkboxes.constEnd(); ++it) {
        setValue(QString("Widgets/Jobs/FilterCheckboxes/%1").arg(it.key()), it.value());
    }
}

bool BSettings::jobsFilterCheckbox(const QString& key, bool defaultValue) const
{
    return value(QString("Widgets/Jobs/FilterCheckboxes/%1").arg(key), defaultValue).toBool();
}

void BSettings::setJobsFilterCheckbox(const QString& key, bool value)
{
    setValue(QString("Widgets/Jobs/FilterCheckboxes/%1").arg(key), value);
}

QMap<QString, int> BSettings::jobsFilterComboboxes() const
{
    QMap<QString, int> comboboxes;

    // Load all saved comboboxes from settings
    // Note: We can't use beginGroup/endGroup in const methods
    // So we'll iterate through all keys and filter for our prefix
    QStringList allKeys = m_settings.allKeys();
    QString prefix = "Widgets/Jobs/FilterComboboxes/";

    for (const QString &key : allKeys) {
        if (key.startsWith(prefix)) {
            QString shortKey = key.mid(prefix.length());
            comboboxes[shortKey] = m_settings.value(key, 0).toInt();
        }
    }

    return comboboxes;
}

void BSettings::setJobsFilterComboboxes(const QMap<QString, int>& comboboxes)
{
    // Clear existing comboboxes
    m_settings.beginGroup("Widgets/Jobs/FilterComboboxes");
    m_settings.remove("");  // Remove all keys in this group
    m_settings.endGroup();

    // Save all comboboxes
    for (auto it = comboboxes.constBegin(); it != comboboxes.constEnd(); ++it) {
        setValue(QString("Widgets/Jobs/FilterComboboxes/%1").arg(it.key()), it.value());
    }
}

int BSettings::jobsFilterCombobox(const QString& key, int defaultValue) const
{
    return value(QString("Widgets/Jobs/FilterComboboxes/%1").arg(key), defaultValue).toInt();
}

void BSettings::setJobsFilterCombobox(const QString& key, int value)
{
    setValue(QString("Widgets/Jobs/FilterComboboxes/%1").arg(key), value);
}

bool BSettings::jobsFilterDateEnabled() const
{
    return value("Widgets/Jobs/filter_date_enabled", false).toBool();
}

void BSettings::setJobsFilterDateEnabled(bool enabled)
{
    setValue("Widgets/Jobs/filter_date_enabled", enabled);
}

QDateTime BSettings::jobsFilterDateFrom() const
{
    return value("Widgets/Jobs/filter_date_from", QDateTime::currentDateTime().addDays(-30)).toDateTime();
}

void BSettings::setJobsFilterDateFrom(const QDateTime& dateTime)
{
    setValue("Widgets/Jobs/filter_date_from", dateTime);
}

QDateTime BSettings::jobsFilterDateTo() const
{
    return value("Widgets/Jobs/filter_date_to", QDateTime::currentDateTime()).toDateTime();
}

void BSettings::setJobsFilterDateTo(const QDateTime& dateTime)
{
    setValue("Widgets/Jobs/filter_date_to", dateTime);
}

// Clients Widget State
bool BSettings::clientsShowOffline() const
{
    return value("Widgets/Clients/show_offline", true).toBool();
}

void BSettings::setClientsShowOffline(bool show)
{
    setValue("Widgets/Clients/show_offline", show);
}

QString BSettings::clientsLastFilter() const
{
    return value("Widgets/Clients/last_filter", "").toString();
}

void BSettings::setClientsLastFilter(const QString& filter)
{
    setValue("Widgets/Clients/last_filter", filter);
}

// Storage Widget State
bool BSettings::storageShowAllVolumes() const
{
    return value("Widgets/Storage/show_all_volumes", true).toBool();
}

void BSettings::setStorageShowAllVolumes(bool showAll)
{
    setValue("Widgets/Storage/show_all_volumes", showAll);
}

QString BSettings::storageLastPool() const
{
    return value("Widgets/Storage/last_pool", "").toString();
}

void BSettings::setStorageLastPool(const QString& pool)
{
    setValue("Widgets/Storage/last_pool", pool);
}

// Schedules Widget State
QString BSettings::schedulesLastSelected() const
{
    return value("Widgets/Schedules/last_selected", "").toString();
}

void BSettings::setSchedulesLastSelected(const QString& scheduleName)
{
    setValue("Widgets/Schedules/last_selected", scheduleName);
}

// Global Statistics Widget State
bool BSettings::statisticsWidgetVisible() const
{
    return value("Widgets/Global/statistics_visible", false).toBool();
}

void BSettings::setStatisticsWidgetVisible(bool visible)
{
    setValue("Widgets/Global/statistics_visible", visible);
}

// ========================================================================
// Utility Methods
// ========================================================================

bool BSettings::hasStoredConnection() const
{
    QString host = connectionHost();
    QString director = connectionDirector();
    QString password = connectionPassword();

    return !host.isEmpty() && !director.isEmpty() && !password.isEmpty();
}

void BSettings::resetToDefaults()
{
    m_settings.clear();

#ifdef IS_DEVELOPER
    qDebug() << "BSettings: All settings reset to defaults";
#endif

    // Emit all change signals
    emit connectionSettingsChanged();
    emit appearanceSettingsChanged();
    emit behaviorSettingsChanged();
    emit autoRefreshSettingsChanged(false, 30);
}

void BSettings::sync()
{
    m_settings.sync();
}
