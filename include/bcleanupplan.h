#ifndef BCLEANUPPLAN_H
#define BCLEANUPPLAN_H

#include <QDateTime>
#include <QJsonArray>
#include <QList>
#include <QString>
#include <QStringList>

/**
 * @brief Selection logic of the cleanup dialog, without widgets and without a Director
 *
 * buildPlan() decides from the job rows of the job list which catalog
 * records the cleanup would delete. The dialog shows, confirms and
 * executes exactly one Plan object, so the three can never disagree.
 *
 * Job rows are the objects of `list jobs` as BJobsModel holds them
 * (jobid, name, client, starttime, type, level, jobfiles, jobbytes,
 * jobstatus) plus the `fileset` the job widget adds from the job
 * configuration.
 */
namespace BCleanup {

struct Options {
    bool removeOldFulls = false;
    int keepFulls = 3;              ///< Fulls to keep per client, job and fileset
    bool removeEmptyJobs = false;
    bool removeFailedJobs = false;
    int failedAgeDays = 30;
    bool pruneVolumes = false;

    bool operator==(const Options &other) const;
    bool operator!=(const Options &other) const { return !(*this == other); }
    bool anySelected() const;
};

struct JobRef {
    QString jobId;
    QString name;
    QString client;
    QString fileset;
    QString startTime;
    QString status;
    qint64 bytes = 0;
    /// Old Fulls only: Incrementals/Differentials based on this Full that the
    /// same plan deletes as empty or failed jobs. They are deleted first.
    QStringList requiredDeletions;
};

/// A Full beyond the keep count that stays because other backups need it
struct BlockedFull {
    JobRef job;
    QStringList dependentJobIds;    ///< Dependents the plan does not delete
};

/// Fulls of one client, job and fileset
struct FullGroup {
    QString client;
    QString name;
    QString fileset;
    int fullCount = 0;              ///< Successful Fulls found
    QList<JobRef> selected;
    QList<BlockedFull> blocked;
};

/// Why no old Full of one job on one client is selected
enum class SkipReason {
    MissingFileset,
    MissingStatus,
    UnknownLevel,
    InvalidStartTime
};

struct SkippedJob {
    QString client;
    QString name;
    SkipReason reason = SkipReason::MissingFileset;
    int rows = 0;                   ///< Rows with this defect
};

/// Why the old Full analysis as a whole selects nothing
enum class Refusal {
    None,
    KeepCountTooSmall,              ///< keepFulls < 1
    RowsWithoutJobId,
    RowsWithoutType,
    BackupRowsWithoutClientOrName
};

struct Plan {
    Options options;                ///< The options this plan was computed for
    int jobRows = 0;                ///< Rows looked at

    Refusal oldFullsRefusal = Refusal::None;
    int oldFullsRefusalRows = 0;    ///< Rows that caused the refusal
    QList<SkippedJob> skippedJobs;
    QList<FullGroup> fullGroups;    ///< Only groups with selected or blocked Fulls
    QList<JobRef> oldFulls;         ///< All selected Fulls, oldest first per group
    qint64 oldFullsBytes = 0;

    QList<JobRef> emptyJobs;
    int emptyRowsWithoutFileCount = 0;  ///< Successful rows skipped: no file count
    QList<JobRef> failedJobs;

    bool pruneVolumes = false;

    /// Job IDs in the order they are deleted: failed, empty, old Fulls; no duplicates
    QStringList jobIdsToDelete() const;
    bool hasWork() const;
};

/**
 * @brief Compute what the cleanup would delete
 * @param jobs Job rows (see namespace description)
 * @param options Options as selected in the dialog
 * @param now Current time, for the age of failed jobs
 *
 * Old Fulls: only backup jobs (type B) that terminated successfully
 * (status T or W) count. They are grouped by client, job name and fileset;
 * the newest keepFulls of each group stay. An older Full is selected only
 * if no Incremental or Differential of the same group lies between it and
 * the next Full, or all of those are deleted by this plan as well.
 * Rows that lack the fields needed for this decision are never guessed:
 * see Refusal and SkipReason.
 */
Plan buildPlan(const QJsonArray &jobs, const Options &options, const QDateTime &now);

/**
 * @brief An analysis result and whether it may still be executed
 *
 * The dialog invalidates it on every option change; canExecute() also
 * compares the options itself, so a missed signal cannot lead to a
 * deletion that was computed for other values.
 */
class Analysis
{
public:
    void set(const Plan &plan);
    void invalidate();
    bool isValid() const { return m_valid; }
    bool canExecute(const Options &current) const;
    const Plan &plan() const { return m_plan; }

private:
    Plan m_plan;
    bool m_valid = false;
};

} // namespace BCleanup

#endif // BCLEANUPPLAN_H
