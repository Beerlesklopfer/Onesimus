#ifndef BRESOURCEMODELS_H
#define BRESOURCEMODELS_H

#include "models/bbasemodels.h"
#include "config/bconfigparser.h"
#include <QMap>
#include <QJsonObject>
#include <QVector>
#include <QDateTime>

// ============================================================================
// BFilesetModel - Model for Bareos filesets
// ============================================================================

/**
 * @brief List model for displaying Bareos filesets with full config caching
 *
 * Stores fileset names from .filesets dot-command AND full resource configs
 * from "show filesets" for use in the Edit FileSet wizard.
 */
class BFilesetModel : public BListModel
{
    Q_OBJECT

public:
    explicit BFilesetModel(QObject *parent = nullptr);

    void parseFilesets(const QString &jsonResponse);
    void parseShowFilesets(const QString &response);

    QStringList filesetNames() const;
    QJsonObject filesetConfig(const QString &name) const;
    bool hasFilesetConfigs() const { return !m_filesetConfigs.isEmpty(); }

protected:
    QString getDisplayText(const QJsonObject &item) const override;

private:
    QMap<QString, QJsonObject> m_filesetConfigs;
};


// ============================================================================
// BStorageModel - Model for Bareos storages
// ============================================================================

/**
 * @brief List model for displaying Bareos storage daemons
 * @version 1.0
 * @since 2026-01-29
 *
 * Displays storage information from .storages dot-command response.
 * Inherits from BListModel for basic JSON handling.
 */
class BStorageModel : public BListModel
{
    Q_OBJECT

public:
    explicit BStorageModel(QObject *parent = nullptr);

    /**
     * @brief Parses .storages dot-command response
     * @param jsonResponse JSON response from Director
     */
    void parseStorages(const QString &jsonResponse);

    /**
     * @brief Returns storage names as string list
     * @return QStringList of storage names
     */
    QStringList storageNames() const;

protected:
    QString getDisplayText(const QJsonObject &item) const override;
};


// ============================================================================
// BPoolModel - Model for Bareos pools
// ============================================================================

/**
 * @brief List model for displaying Bareos pools
 * @version 1.0
 * @since 2026-01-29
 *
 * Displays pool information from .pools dot-command response.
 * Inherits from BListModel for basic JSON handling.
 */
class BPoolModel : public BListModel
{
    Q_OBJECT

public:
    explicit BPoolModel(QObject *parent = nullptr);

    /**
     * @brief Parses .pools dot-command response
     * @param jsonResponse JSON response from Director
     */
    void parsePools(const QString &jsonResponse);

    /**
     * @brief Returns pool names as string list
     * @return QStringList of pool names
     */
    QStringList poolNames() const;

    /**
     * @brief Returns detailed pool information for a specific pool
     * @param poolName Name of the pool
     * @return QJsonObject containing pool details
     */
    QJsonObject poolInfo(const QString &poolName) const;

protected:
    QString getDisplayText(const QJsonObject &item) const override;
};


// ============================================================================
// BCatalogModel - Model for Bareos catalogs
// ============================================================================

/**
 * @brief List model for displaying Bareos catalog resources
 *
 * Displays catalog information from .catalogs dot-command response.
 * Inherits from BListModel for basic JSON handling.
 */
class BCatalogModel : public BListModel
{
    Q_OBJECT

public:
    explicit BCatalogModel(QObject *parent = nullptr);

    /**
     * @brief Parses .catalogs dot-command response
     * @param jsonResponse JSON response from Director
     */
    void parseCatalogs(const QString &jsonResponse);

    /**
     * @brief Returns catalog names as string list
     * @return QStringList of catalog names
     */
    QStringList catalogNames() const;

protected:
    QString getDisplayText(const QJsonObject &item) const override;
};


// ============================================================================
// BLevelModel - Model for Bareos backup levels
// ============================================================================

/**
 * @brief List model for displaying Bareos backup levels
 * @version 1.0
 * @since 2026-01-29
 *
 * Displays level information from .levels dot-command response.
 * Levels: Full (F), Incremental (I), Differential (D), VirtualFull (V)
 */
class BLevelModel : public BListModel
{
    Q_OBJECT

public:
    explicit BLevelModel(QObject *parent = nullptr);

    /**
     * @brief Parses .levels dot-command response
     * @param jsonResponse JSON response from Director
     */
    void parseLevels(const QString &jsonResponse);

    /**
     * @brief Returns level codes as string list (F, I, D, V)
     * @return QStringList of level codes
     */
    QStringList levelCodes() const;

    /**
     * @brief Returns level descriptions as string list
     * @return QStringList of level descriptions
     */
    QStringList levelDescriptions() const;

protected:
    QString getDisplayText(const QJsonObject &item) const override;
};

// ============================================================================
// BJobConfigModel - Model for Bareos job configurations
// ============================================================================

/**
 * @brief Model for Bareos job configurations from "show jobs" command
 *
 * Parses full job resource configs from "show jobs" JSON response and
 * converts them to BConfigResource objects for use with BJobResourceWidget.
 */
class BJobConfigModel : public QObject
{
    Q_OBJECT

public:
    explicit BJobConfigModel(QObject *parent = nullptr);

    // --- Jobs ---
    void parseShowJobs(const QString &jsonResponse);
    QList<BConfigResource> jobResources() const;
    QStringList jobNames() const;
    bool hasConfigs() const { return !m_jobConfigs.isEmpty(); }
    const QMap<QString, QJsonObject>& jobConfigsRaw() const { return m_jobConfigs; }

    // --- JobDefs ---
    void parseShowJobDefs(const QString &jsonResponse);
    QList<BConfigResource> jobDefsResources() const;
    QStringList jobDefsNames() const;

    // Reusable JSON → BConfigResource conversion helpers
    static BConfigResource jsonToResource(const QString &resourceType,
                                          const QString &name, const QJsonObject &obj);
    static BConfigValue jsonValueToBConfigValue(const QJsonValue &val);

private:
    QMap<QString, QJsonObject> m_jobConfigs;
    QMap<QString, QJsonObject> m_jobDefsConfigs;
};


// ============================================================================
// BScheduleEntry - Parsed schedule run entry
// ============================================================================

/**
 * @brief Parsed representation of a single Bareos schedule Run directive
 *
 * Represents one "Run" line from a Schedule resource, broken into its
 * components for use in Gantt visualization and collision detection.
 */
struct BScheduleEntry
{
    QString scheduleName;
    QString jobName;
    QString client;

    enum BackupLevel {
        None = 0,
        Full = 1,
        Differential = 2,
        Incremental = 3,
        VirtualFull = 4
    };

    BackupLevel level = Incremental;
    int hour = 0;
    int minute = 0;
    QVector<int> daysOfWeek;      ///< 0=Mon..6=Sun
    QString weekSpec;              ///< "1st", "2nd-5th", etc.
    int estimatedDurationSecs = 0; ///< from historical data
    QString pool;
    QString storage;
    QString messages;
    int priority = 10;

    /**
     * @brief Returns the original Run directive string
     */
    QString toRunDirective() const;

    /**
     * @brief Parses a Bareos Run directive string into a BScheduleEntry
     */
    static BScheduleEntry fromRunDirective(const QString &run);

    /**
     * @brief Returns human-readable level string
     */
    static QString levelToString(BackupLevel level);

    /**
     * @brief Parses level string to enum value
     */
    static BackupLevel levelFromString(const QString &str);
};


// ============================================================================
// BScheduleModel - Model for Bareos schedules
// ============================================================================

/**
 * @brief List model for displaying Bareos schedules with parsed run entries
 *
 * Stores schedule data from .schedule dot-command and provides parsed
 * BScheduleEntry objects for use by Gantt visualization.
 */
class BScheduleModel : public BListModel
{
    Q_OBJECT

public:
    explicit BScheduleModel(QObject *parent = nullptr);

    /**
     * @brief Parses .schedule dot-command response
     * @param jsonResponse JSON response from Director
     */
    void parseSchedules(const QString &jsonResponse);

    /**
     * @brief Returns schedule names as string list
     */
    QStringList scheduleNames() const;

    /**
     * @brief Returns the full JSON object for a schedule by name
     */
    QJsonObject scheduleByName(const QString &name) const;

    /**
     * @brief Returns all parsed schedule entries across all schedules
     *
     * Each Run directive in each Schedule becomes one BScheduleEntry.
     */
    QList<BScheduleEntry> allEntries() const { return m_entries; }

    /**
     * @brief Returns parsed entries for a specific schedule
     */
    QList<BScheduleEntry> entriesForSchedule(const QString &scheduleName) const;

protected:
    QString getDisplayText(const QJsonObject &item) const override;

private:
    void rebuildEntries();

    QMap<QString, QJsonObject> m_scheduleConfigs;
    QList<BScheduleEntry> m_entries;
};


// ============================================================================
// BJobDurationStats - Historical job duration statistics
// ============================================================================

/**
 * @brief Collects and aggregates historical job durations per job name
 *
 * Parses "list jobs" responses and provides min/avg/max duration statistics
 * for use in Gantt bar width estimation.
 */
class BJobDurationStats : public QObject
{
    Q_OBJECT

public:
    explicit BJobDurationStats(QObject *parent = nullptr);

    /**
     * @brief Feeds job data from "list jobs" JSON response
     * @param jsonResponse JSON response from Director
     */
    void feedJobs(const QString &jsonResponse);

    /**
     * @brief Clears all collected statistics
     */
    void clear();

    /**
     * @brief Returns average duration in seconds for a job name
     * @return Average duration, or 0 if no data
     */
    int averageDuration(const QString &jobName) const;
    int averageDuration(const QString &jobName, const QString &level) const;

    /**
     * @brief Returns minimum duration in seconds
     */
    int minDuration(const QString &jobName) const;
    int minDuration(const QString &jobName, const QString &level) const;

    /**
     * @brief Returns maximum duration in seconds
     */
    int maxDuration(const QString &jobName) const;
    int maxDuration(const QString &jobName, const QString &level) const;

    /**
     * @brief Returns number of recorded durations for a job
     */
    int sampleCount(const QString &jobName) const;
    int sampleCount(const QString &jobName, const QString &level) const;

    /**
     * @brief Returns all known job names
     */
    QStringList jobNames() const;

    /**
     * @brief Record of a single historical job run
     */
    struct JobRunRecord {
        QDateTime startTime;
        int durationSecs = 0;
        QString level;
    };

    /**
     * @brief Returns the most recent runs for a job, sorted newest first
     */
    QList<JobRunRecord> lastRuns(const QString &jobName, int count = 5) const;
    QList<JobRunRecord> lastRuns(const QString &jobName, const QString &level, int count = 5) const;

    /**
     * @brief Returns duration trend (seconds change, positive = getting slower)
     * @return Difference in seconds between recent avg and older avg, or 0 if insufficient data
     */
    double trend(const QString &jobName) const;
    double trend(const QString &jobName, const QString &level) const;

    /**
     * @brief Parses a Bareos duration string (HH:MM:SS) to seconds
     */
    static int parseDuration(const QString &durationStr);

    /**
     * @brief Formats seconds as human-readable duration string
     */
    static QString formatDuration(int secs);

private:
    struct DurationData {
        QVector<JobRunRecord> runs;  ///< individual runs with timestamps
        int minSecs = 0;
        int maxSecs = 0;
        int avgSecs = 0;
    };

    void recalculate(DurationData &data);

    QMap<QString, DurationData> m_stats;
};

#endif // BRESOURCEMODELS_H
