#include "config/bsettings.h"
#include "blogging.h"
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

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
    , m_settings()  // Uses QCoreApplication::organizationName() and applicationName()
#elif defined(USE_BAREOS)
    , m_settings("Bareos", "Onesimus")  // Bareos organization name
#else
    , m_settings("Bareos", "Onesimus")  // Default to Bareos
#endif
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BSettings: Initializing settings";
    BLOG_DEBUG() << "  Settings file:" << m_settings.fileName();
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

bool BSettings::connectionSavePassword() const
{
    return value("Connection/save_password", true).toBool();
}

void BSettings::setConnectionSavePassword(bool save)
{
    setValue("Connection/save_password", save);
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
// Connection Profiles
// ========================================================================

QList<BConnectionProfile> BSettings::connectionProfiles() const
{
    QList<BConnectionProfile> profiles;

    QByteArray jsonData = value("ConnectionProfiles/data").toByteArray();
    if (jsonData.isEmpty()) {
        return profiles;
    }

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isArray()) {
        return profiles;
    }

    QJsonArray array = doc.array();
    for (const QJsonValue &val : array) {
        if (val.isObject()) {
            profiles.append(BConnectionProfile::fromJson(val.toObject()));
        }
    }

    return profiles;
}

void BSettings::setConnectionProfiles(const QList<BConnectionProfile> &profiles)
{
    QJsonArray array;
    for (const BConnectionProfile &profile : profiles) {
        array.append(profile.toJson());
    }

    QJsonDocument doc(array);
    setValue("ConnectionProfiles/data", doc.toJson(QJsonDocument::Compact));
    emit connectionSettingsChanged();
}

void BSettings::addConnectionProfile(const BConnectionProfile &profile)
{
    QList<BConnectionProfile> profiles = connectionProfiles();
    profiles.append(profile);
    setConnectionProfiles(profiles);
}

bool BSettings::updateConnectionProfile(const BConnectionProfile &profile)
{
    QList<BConnectionProfile> profiles = connectionProfiles();
    for (int i = 0; i < profiles.size(); ++i) {
        if (profiles[i].id == profile.id) {
            profiles[i] = profile;
            setConnectionProfiles(profiles);
            return true;
        }
    }
    return false;
}

bool BSettings::removeConnectionProfile(const QString &profileId)
{
    QList<BConnectionProfile> profiles = connectionProfiles();
    for (int i = 0; i < profiles.size(); ++i) {
        if (profiles[i].id == profileId) {
            profiles.removeAt(i);
            setConnectionProfiles(profiles);

            // Clear last used if this profile was removed
            if (lastUsedProfileId() == profileId) {
                setLastUsedProfileId(QString());
            }
            return true;
        }
    }
    return false;
}

BConnectionProfile BSettings::connectionProfile(const QString &profileId) const
{
    QList<BConnectionProfile> profiles = connectionProfiles();
    for (const BConnectionProfile &profile : profiles) {
        if (profile.id == profileId) {
            return profile;
        }
    }
    return BConnectionProfile();  // Invalid profile
}

QString BSettings::lastUsedProfileId() const
{
    return value("ConnectionProfiles/lastUsed").toString();
}

void BSettings::setLastUsedProfileId(const QString &profileId)
{
    setValue("ConnectionProfiles/lastUsed", profileId);
}

BConnectionProfile BSettings::lastUsedProfile() const
{
    QString id = lastUsedProfileId();
    if (id.isEmpty()) {
        return BConnectionProfile();
    }
    return connectionProfile(id);
}

void BSettings::migrateOldConnectionSettings()
{
    // Check if we already have profiles
    if (!connectionProfiles().isEmpty()) {
        return;  // Already migrated
    }

    // Read old settings directly from QSettings
    QString host = value("Connection/host", "localhost").toString();
    if (host.isEmpty() || host == "localhost") {
        // No meaningful old settings to migrate - wipe any leftover cleartext
        m_settings.remove("Connection/password");
        return;
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "Migrating old connection settings to profile system...";
#endif

    // Create a profile from old settings
    BConnectionProfile profile = BConnectionProfile::create(tr("Default"));
    profile.host = host;
    profile.port = value("Connection/port", 9101).toInt();
    profile.directorName = value("Connection/director", "bareos-dir").toString();
    profile.consoleName = value("Connection/console", "onesimus").toString();

    // MANDATORY: Transform old cleartext password to MD5 hash
    QString oldPassword = value("Connection/password").toString();
    if (!oldPassword.isEmpty()) {
        profile.setPasswordFromCleartext(oldPassword);
    }

    profile.legacyAuth = value("Connection/legacy_auth", false).toBool();
    profile.tlsEnabled = value("Connection/tls_enabled", true).toBool();
    profile.tlsUsePSK = value("Connection/tls_use_psk", true).toBool();
    profile.tlsCaCertFile = value("Connection/tls_ca_cert_file").toString();
    profile.tlsCertFile = value("Connection/tls_cert_file").toString();
    profile.tlsKeyFile = value("Connection/tls_key_file").toString();
    profile.tlsPfxFile = value("Connection/tls_pfx_file").toString();
    profile.tlsVerifyPeer = value("Connection/tls_verify_peer", true).toBool();

    // Save the profile
    addConnectionProfile(profile);
    setLastUsedProfileId(profile.id);

    // Wipe old cleartext password and legacy individual settings
    m_settings.remove("Connection/password");
    m_settings.remove("Connection/host");
    m_settings.remove("Connection/port");
    m_settings.remove("Connection/director");
    m_settings.remove("Connection/console");
    m_settings.remove("Connection/tls_enabled");
    m_settings.remove("Connection/tls_use_psk");
    m_settings.remove("Connection/legacy_auth");
    m_settings.remove("Connection/tls_ca_cert_file");
    m_settings.remove("Connection/tls_cert_file");
    m_settings.remove("Connection/tls_key_file");
    m_settings.remove("Connection/tls_pfx_file");
    m_settings.remove("Connection/tls_verify_peer");

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "  Created profile:" << profile.name << "with id:" << profile.id;
    BLOG_DEBUG() << "  Old cleartext password and legacy settings wiped";
#endif
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

QColor BSettings::statusBarConnectedColor() const
{
    // Default: bright green visible on blue status bar
    QString colorString = value("Appearance/statusbar_connected_color", "#7fff7f").toString();
    return QColor(colorString);
}

void BSettings::setStatusBarConnectedColor(const QColor& color)
{
    setValue("Appearance/statusbar_connected_color", color.name());
    emit appearanceSettingsChanged();
}

QColor BSettings::statusBarDisconnectedColor() const
{
    // Default: gray
    QString colorString = value("Appearance/statusbar_disconnected_color", "#aaaaaa").toString();
    return QColor(colorString);
}

void BSettings::setStatusBarDisconnectedColor(const QColor& color)
{
    setValue("Appearance/statusbar_disconnected_color", color.name());
    emit appearanceSettingsChanged();
}

// ========================================================================
// Behavior Settings
// ========================================================================

QStringList BSettings::visibleLevels() const
{
    // Default: Full, Incremental, Differential, VirtualFull
    QStringList defaultLevels = {"Full", "Incremental", "Differential", "VirtualFull"};
    return value("Behavior/visible_levels", defaultLevels).toStringList();
}

void BSettings::setVisibleLevels(const QStringList &levels)
{
    setValue("Behavior/visible_levels", levels);
    emit behaviorSettingsChanged();
}

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

bool BSettings::behaviorJobsNewestFirst() const
{
    return value("Behavior/jobs_newest_first", true).toBool();
}

void BSettings::setBehaviorJobsNewestFirst(bool newestFirst)
{
    setValue("Behavior/jobs_newest_first", newestFirst);
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

// Jobs Pagination
bool BSettings::jobsPaginationEnabled() const
{
    return value("Widgets/Jobs/pagination_enabled", true).toBool();
}

void BSettings::setJobsPaginationEnabled(bool enabled)
{
    setValue("Widgets/Jobs/pagination_enabled", enabled);
}

int BSettings::jobsPaginationPageSize() const
{
    return value("Widgets/Jobs/pagination_page_size", 100).toInt();
}

void BSettings::setJobsPaginationPageSize(int pageSize)
{
    setValue("Widgets/Jobs/pagination_page_size", pageSize);
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

int BSettings::schedulesViewMode() const
{
    return value("Widgets/Schedules/view_mode", 0).toInt();
}

void BSettings::setSchedulesViewMode(int index)
{
    setValue("Widgets/Schedules/view_mode", index);
}

int BSettings::schedulesDayViewMode() const
{
    return value("Widgets/Schedules/day_view_mode", 0).toInt();
}

void BSettings::setSchedulesDayViewMode(int index)
{
    setValue("Widgets/Schedules/day_view_mode", index);
}

int BSettings::schedulesCurrentDay() const
{
    return value("Widgets/Schedules/current_day", QDate::currentDate().dayOfWeek() - 1).toInt();
}

void BSettings::setSchedulesCurrentDay(int day)
{
    setValue("Widgets/Schedules/current_day", day);
}

int BSettings::schedulesSnapIndex() const
{
    return value("Widgets/Schedules/snap_index", 0).toInt();
}

void BSettings::setSchedulesSnapIndex(int index)
{
    setValue("Widgets/Schedules/snap_index", index);
}

QByteArray BSettings::schedulesSplitterState() const
{
    return value("Widgets/Schedules/splitter_state").toByteArray();
}

void BSettings::setSchedulesSplitterState(const QByteArray& state)
{
    setValue("Widgets/Schedules/splitter_state", state);
}

QByteArray BSettings::schedulesGanttSplitterState() const
{
    return value("Widgets/Schedules/gantt_splitter_state").toByteArray();
}

void BSettings::setSchedulesGanttSplitterState(const QByteArray& state)
{
    setValue("Widgets/Schedules/gantt_splitter_state", state);
}

bool BSettings::schedulesFilterCheckbox(const QString& key, bool defaultValue) const
{
    return value(QString("Widgets/Schedules/FilterCheckboxes/%1").arg(key), defaultValue).toBool();
}

void BSettings::setSchedulesFilterCheckbox(const QString& key, bool value)
{
    setValue(QString("Widgets/Schedules/FilterCheckboxes/%1").arg(key), value);
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
// BVFS (Job Details Dialog) Settings
// ========================================================================

bool BSettings::bvfsShowAllRelatedJobs() const
{
    return value("BVFS/showAllRelatedJobs", false).toBool();
}

void BSettings::setBvfsShowAllRelatedJobs(bool showAll)
{
    setValue("BVFS/showAllRelatedJobs", showAll);
}

// ========================================================================
// Main Window Settings
// ========================================================================

int BSettings::mainWindowActiveTab() const
{
    return value("MainWindow/activeTab", 0).toInt();
}

void BSettings::setMainWindowActiveTab(int index)
{
    setValue("MainWindow/activeTab", index);
}

QByteArray BSettings::mainWindowSplitterState() const
{
    return value("MainWindow/splitterState").toByteArray();
}

void BSettings::setMainWindowSplitterState(const QByteArray& state)
{
    setValue("MainWindow/splitterState", state);
}

bool BSettings::lowerPanelVisible() const
{
    return value("MainWindow/lowerPanelVisible", true).toBool();
}

void BSettings::setLowerPanelVisible(bool visible)
{
    setValue("MainWindow/lowerPanelVisible", visible);
}

// ========================================================================
// Messages Widget Settings
// ========================================================================

int BSettings::messagesPollInterval() const
{
    return value("Messages/pollInterval", 30000).toInt();
}

void BSettings::setMessagesPollInterval(int intervalMs)
{
    setValue("Messages/pollInterval", intervalMs);
}

int BSettings::messagesMaxHistory() const
{
    return value("Messages/maxHistory", 1000).toInt();
}

void BSettings::setMessagesMaxHistory(int maxMessages)
{
    setValue("Messages/maxHistory", maxMessages);
}

// ========================================================================
// Utility Methods
// ========================================================================

void BSettings::resetToDefaults()
{
    m_settings.clear();

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BSettings: All settings reset to defaults";
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
