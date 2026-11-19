#ifndef BRESOURCEMODELS_H
#define BRESOURCEMODELS_H

#include "models/bbasemodels.h"
#include "config/bconfigparser.h"
#include "schedules/model/brunspec.h"
#include "schedules/model/bscheduleoccurrences.h"
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
    /// The JobDefs as the Director reported them ("show jobdefs"), by name
    const QMap<QString, QJsonObject>& jobDefsConfigsRaw() const { return m_jobDefsConfigs; }

    /**
     * @brief Names of the Messages resources the loaded Jobs and JobDefs refer to
     *
     * Sorted, without duplicates. The Director is not asked for its Messages
     * resources, so one that no Job and no JobDefs uses is not in the list.
     */
    QStringList messagesNames() const;

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
 * @brief Display adapter for a single Bareos schedule Run directive
 *
 * One "Run" line of a Schedule resource, reduced to what the Gantt view, the
 * grid view and the schedule wizard show: one level, one time, the days of
 * the week. The line itself is read by BRunParser and kept in @ref source;
 * the fields here are derived from it and cannot express everything a Run
 * line can say (months, days of the month, weeks of the year, several
 * hours). An entry read from a line is therefore always written as that
 * line (toRunLine()); to change a run, make a new entry without a source.
 *
 * The views do not show an entry by its days of the week: they show its
 * @ref occurrences, the concrete starts in the dates on display
 * (fromOccurrences()).
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

    BackupLevel level = None;      ///< None: no level, or one without a value here (see levelName)
    QString levelName;             ///< Level as Bareos spells it; empty: the job's own level applies
    int hour = 0;
    int minute = 0;
    QVector<int> daysOfWeek;      ///< 0=Mon..6=Sun
    QString weekSpec;              ///< "1st", "2nd-5th", etc.
    QString pool;
    QString storage;
    QString messages;
    int priority = 10;

    /**
     * @brief The Run line this entry was read from
     *
     * original is empty for an entry made in the application. Without a
     * spec the line was not understood: the other fields then say nothing.
     */
    BRunLine source;

    /// Position of @ref source among the Run lines of its schedule
    /// (BScheduleModel::runDirectives()); -1 if not known
    int runIndex = -1;

    /// The starts of this run at @ref hour in the dates a view displays,
    /// in ascending order. Filled by fromOccurrences() only.
    QList<QDateTime> occurrences;

    /// No enabled job uses the schedule (BJobScheduleIndex::entriesFor()):
    /// at these times nothing starts. Such an entry has no job.
    bool unused = false;

    /// Does it start on this date?
    bool occursOn(const QDate &date) const;

    /// Do both have a start on the same date?
    bool sharesDateWith(const BScheduleEntry &other) const;

    /// Read from a Run line that the parser does not understand (also an empty one)
    bool notUnderstood() const
    {
        return !source.spec && (!source.original.isEmpty() || !source.parseError.isEmpty());
    }

    /// The level for display: levelName, or the name of @ref level
    QString levelLabel() const;

    /**
     * @brief What the fields of this entry say, as a run
     * @return nothing if they do not describe a run (a bad time, an unknown
     *         level or week specification)
     */
    std::optional<BRunSpec> toSpec() const;

    /// The fields still say exactly what the source line says
    bool matchesSource() const;

    /**
     * @brief The Run line to write for this entry
     *
     * The source line, untouched, for an entry that was read from a Run
     * line. A line made from the fields and marked dirty for an entry made
     * in the application.
     */
    BRunLine toRunLine() const;

    /**
     * @brief The Run directive text to write (BRunWriter::lineText())
     * @return an empty string with @p error set if the line cannot be written
     */
    QString toRunDirective(QString *error = nullptr) const;

    /**
     * @brief Reads a Bareos Run directive through BRunParser
     *
     * A line with several hours gives the first one in @ref hour.
     */
    static BScheduleEntry fromRunDirective(const QString &run);
    static BScheduleEntry fromRunLine(const BRunLine &line);

    /**
     * @brief The entries a view shows for concrete dates
     * @param schedule    The schedule
     * @param occurrences Its starts in the dates on display
     *                    (BScheduleOccurrences::occurrences())
     * @return one entry per Run line and hour of the day that has a start,
     *         ordered by line and hour, each with its starts. A line that
     *         does not run in these dates gives no entry.
     */
    static QList<BScheduleEntry> fromOccurrences(const BSchedule &schedule,
                                                 const QList<BOccurrence> &occurrences);

    /**
     * @brief Returns human-readable level string
     */
    static QString levelToString(BackupLevel level);

    /**
     * @brief The enum value of a level name
     * @return None for an empty string and for every level without a value
     *         here; never a guess
     */
    static BackupLevel levelFromString(const QString &str);
};

/// A Run line of a schedule that was not understood
struct BRunProblem
{
    QString scheduleName;
    int line = 0;       ///< 1-based position among the Run lines of the schedule
    QString text;       ///< The line as the Director reported it
    QString error;
};


// ============================================================================
// BScheduleModel - Model for Bareos schedules
// ============================================================================

/**
 * @brief List model for displaying Bareos schedules with parsed run entries
 *
 * Stores schedule data from .schedule dot-command and provides each
 * schedule as a BSchedule with its Run lines read by BRunParser. What runs
 * when is worked out from that for concrete dates (BScheduleOccurrences).
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
     * @brief A schedule with its Run lines, understood or not
     *
     * The lines are in the Director's order: index i of BSchedule::runs is
     * index i of runDirectives(). An unknown name gives an empty schedule.
     */
    BSchedule schedule(const QString &name) const;

    /// All schedules, ordered by name
    QList<BSchedule> schedules() const { return m_schedules.values(); }

    /// The number of Run lines over all schedules, understood or not
    int runLineCount() const;

    /**
     * @brief Returns the Run directives of a schedule as the Director reported them
     *
     * One string per Run line, unparsed and in the Director's order.
     */
    QStringList runDirectives(const QString &scheduleName) const;

    /**
     * @brief The Run lines that were not understood, over all schedules
     *
     * They are shown as not understood and block writing their schedule.
     */
    QList<BRunProblem> runProblems() const { return m_runProblems; }

protected:
    QString getDisplayText(const QJsonObject &item) const override;

private:
    void rebuildSchedules();

    QMap<QString, QJsonObject> m_scheduleConfigs;
    QMap<QString, BSchedule> m_schedules;
    QList<BRunProblem> m_runProblems;
};


#endif // BRESOURCEMODELS_H
