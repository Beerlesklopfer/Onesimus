// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BJOBHISTORYSTATS_H
#define BJOBHISTORYSTATS_H

#include <QChar>
#include <QDateTime>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>

#include <optional>

/**
 * @file bjobhistorystats.h
 * @brief Measured durations and waits of past job runs, without widgets
 *
 * The schedule views draw bars, predict waits and fill a heat map. For
 * that they need to know how long a job runs and how long it waits before
 * it starts. Both are measured here from the catalog: the reply of
 * "llist jobs days=<n> reverse limit=<m>" is read into runs
 * (BJobHistoryReply) and the runs are condensed per job and level
 * (BJobHistoryStats). Where nothing was measured the answer is "no
 * history"; there is no default figure anywhere in this file.
 *
 * Where the facts come from (read on 2026-11-19):
 * - Bareos, branch bareos-25 (commit 0e7742c):
 *   core/src/dird/ua_output.cc (DoListCmd for jobs: "days=", "hours=",
 *   "limit=", "offset=", "reverse"), core/src/cats/sql_list.cc
 *   (BareosDb::ListJobRecords: "AND Job.SchedTime > '<now - days>'",
 *   "ORDER BY StartTime [DESC] LIMIT n"), core/src/cats/
 *   postgresql_queries.inc (0007_list_jobs_long: the columns),
 *   core/src/cats/sql.cc with core/src/lib/output_formatter.cc (every
 *   column is one JSON string, the key is the column name in lower case),
 *   core/src/include/job_status.h, job_level.h, job_types.h (the codes),
 *   core/src/lib/btime.cc (bstrutime: "%Y-%m-%d %H:%M:%S", local time of
 *   the Director), core/src/dird/job.cc (job_thread sets the start time
 *   when the job leaves the queue), core/src/dird/ua_run.cc (a job started
 *   by hand is scheduled for the moment of the run command),
 *   core/src/dird/migrate.cc and vbackup.cc (copied, migrated and virtual
 *   full jobs get the start and end time of another job).
 * - Onesimus Director: crates/odir/src/commands/list.rs (list_jobs,
 *   LIST_JOBS_LONG, schedtime) and crates/odir/src/sql_list.rs (json_rows),
 *   which follow the Bareos sources above.
 *
 * Nothing here was checked against a running Director.
 *
 * Times: the catalog holds times without a zone, in the local time of the
 * Director. They are kept as that wall-clock time (stored with the UTC
 * spec so that Qt does no conversion) and only ever compared with each
 * other. A run that crosses a daylight-saving change is therefore measured
 * an hour too long or too short; a run whose times come out negative is
 * left out.
 */

/** One row of "llist jobs": a job run as the catalog recorded it. */
struct BJobRun
{
    quint64 jobId = 0;
    QString name;           ///< Job.Name, the name of the Job resource
    QString client;         ///< Client.Name; empty if not delivered
    QChar type;             ///< Job.Type ('B' backup, 'C' copy of a job, ...)
    QChar level;            ///< Job.Level ('F', 'I', 'D', 'f', ' ' = none, ...)
    QChar status;           ///< Job.JobStatus ('T', 'W', 'E', 'A', ...)
    QDateTime scheduled;    ///< Job.SchedTime; invalid if not delivered
    QDateTime started;      ///< Job.StartTime; invalid if not delivered
    QDateTime ended;        ///< Job.EndTime; invalid if not delivered
    QDateTime reallyEnded;  ///< Job.RealEndTime; invalid if not delivered
};

/** What the Director answered to the history command. */
struct BJobHistoryReply
{
    enum class Outcome {
        Runs,       ///< A list of runs, possibly empty
        Refused,    ///< The Director answered with an error (unknown or forbidden command, ...)
        Unreadable, ///< Not a reply of the expected form
    };

    Outcome outcome = Outcome::Unreadable;
    QString error;          ///< Refused, Unreadable: why, for the user
    QList<BJobRun> runs;    ///< Runs: every row with a job name
    int rows = 0;           ///< Runs: number of rows in the reply
    /// Runs: number of rows that carry a scheduled time. 0 with rows > 0
    /// means that this Director does not deliver it.
    int rowsWithScheduledTime = 0;

    /**
     * @brief Reads the reply of "llist jobs" in API mode 2
     *
     * Expected: {"jsonrpc":"2.0","id":null,"result":{"jobs":[{...},...]}}
     * with every value a string. An "error" object gives Refused with the
     * Director's messages as the text. "jobs" as an empty object (Bareos
     * answers so for an unknown job id) is an empty list.
     */
    static BJobHistoryReply parse(const QString &reply);

    /**
     * @brief A catalog time "yyyy-MM-dd HH:mm:ss" as wall-clock time
     * @return invalid for an empty value, for "0", for a time that cannot
     *         be read and for the zero dates of old catalogs (year < 1971)
     */
    static QDateTime parseTime(const QString &text);
};

/**
 * @brief Durations and waits per job and level, from measured runs
 *
 * Which runs count (isMeasured()):
 * - status 'T' (terminated normally) or 'W' (terminated normally with
 *   warnings); nothing else, so failed, cancelled, running and waiting jobs
 *   are left out;
 * - start and end are delivered and the end is not before the start;
 * - the start is not before the scheduled time. Bareos gives the catalog
 *   records of copied, migrated and virtual full jobs the start and end
 *   time of another, older job, so such a row says nothing about its own
 *   run. That is the only way the sources show for a start before the
 *   scheduled time: a job started by hand or rerun is scheduled for the
 *   moment of the command;
 * - the run is inside the window: scheduled (or, without a scheduled
 *   time, started) no longer than the window before "now".
 *
 * Duration = end minus start (Job.EndTime, as the Director's own Duration
 * column; Job.RealEndTime only if there is no EndTime).
 *
 * Wait = start minus scheduled time. It is unknown, and never zero, for a
 * run without a scheduled time; such a run still counts for the duration.
 * The catalog does not say whether a run was started by the scheduler or by
 * hand, so both are in the figures; for a job started by hand the wait is
 * the time between the run command and the start.
 *
 * Quantiles: linear interpolation between the two nearest ranks (the
 * common "type 7" definition), rounded to whole seconds. One run gives
 * that run for every quantile; the median of two runs is their mean.
 */
class BJobHistoryStats
{
public:
    struct Quantiles
    {
        int count = 0;      ///< Number of values; 0: nothing measured
        qint64 p25 = 0;
        qint64 median = 0;
        qint64 p75 = 0;
        qint64 p90 = 0;
        qint64 min = 0;     ///< Smallest value
        qint64 max = 0;     ///< Largest value

        bool isValid() const { return count > 0; }
    };

    /** A run that counted, with what was measured on it. */
    struct MeasuredRun
    {
        quint64 jobId = 0;
        QString level;                  ///< Level name, empty for none
        QDateTime started;
        qint64 durationSecs = 0;
        std::optional<qint64> waitSecs; ///< nullopt: unknown
    };

    /** One job at one level. */
    struct Group
    {
        QString job;
        QString level;          ///< "Full", "Incremental", ...; empty for a job without level
        int runs = 0;           ///< Runs that counted (= duration.count)
        Quantiles duration;     ///< Seconds
        Quantiles wait;         ///< Seconds; invalid if no run has a known wait
        int waitUnknown = 0;    ///< Runs without a known wait
        QDateTime lastRun;      ///< Start of the newest run that counted
    };

    /** Where an answer of estimate() comes from. */
    enum class Basis {
        NoHistory,      ///< Nothing measured for this job: there is no figure
        JobAndLevel,    ///< Measured runs of this job at this level
        AllLevels,      ///< Measured runs of this job at all levels: an estimate
    };

    struct Estimate
    {
        Basis basis = Basis::NoHistory;
        int runs = 0;
        Quantiles duration;
        Quantiles wait;
        int waitUnknown = 0;

        bool hasHistory() const { return basis != Basis::NoHistory; }
        /// Measured at exactly the level asked for
        bool isMeasured() const { return basis == Basis::JobAndLevel; }
    };

    /** What was left out and why, for the line above the table. */
    struct Summary
    {
        int rows = 0;               ///< Rows handed in
        int counted = 0;            ///< Runs that counted
        int notSuccessful = 0;      ///< Status other than T or W
        int outsideWindow = 0;
        int foreignTimes = 0;       ///< Start before the scheduled time
        int unusableTimes = 0;      ///< Start or end missing, or end before start
        int waitUnknown = 0;        ///< Counted runs without a scheduled time
    };

    /**
     * @brief Replaces the statistics by those of @p runs
     * @param windowDays Runs scheduled longer ago than this are left out
     * @param now        Wall-clock time of the Director, as near as known
     *                   (the caller's local time, read with wallClockNow())
     */
    void setRuns(const QList<BJobRun> &runs, int windowDays, const QDateTime &now);

    void clear();

    /// No run counted: there is no history at all
    bool isEmpty() const { return m_groups.isEmpty(); }

    int windowDays() const { return m_windowDays; }
    Summary summary() const { return m_summary; }

    /// One entry per job and level, ordered by job, then level
    QList<Group> groups() const;

    /**
     * @brief Typical duration and spread of a job at a level
     *
     * Rule:
     * 1. Runs of this job at this level were measured: their quantiles,
     *    Basis::JobAndLevel.
     * 2. Otherwise, runs of this job at other levels were measured: the
     *    quantiles over all its runs, Basis::AllLevels. That is an
     *    estimate and has to be shown as one: a Full that never ran in the
     *    window is not as short as the Incrementals that did.
     * 3. Otherwise Basis::NoHistory and no figure.
     *
     * An empty @p level (the Run line names none and the job's own is not
     * known, or the job has none) asks for the job as a whole: measured
     * (Basis::JobAndLevel) if all its runs in the window were at one level,
     * an estimate over all levels (Basis::AllLevels) if they were not.
     *
     * There is no step from the job to other jobs or to a default.
     * @p level is compared without regard to case.
     */
    Estimate estimate(const QString &job, const QString &level) const;

    /// The newest measured runs of a job, newest first; an empty level
    /// means all levels
    QList<MeasuredRun> lastRuns(const QString &job, const QString &level, int count) const;

    // --- Single runs --------------------------------------------------------

    /// Status 'T' or 'W'
    static bool terminatedSuccessfully(QChar status);

    /// Start minus scheduled time; nullopt if either is missing or the
    /// start is before the scheduled time. Never negative, never a guess.
    static std::optional<qint64> waitSecs(const BJobRun &run);

    /// End minus start; nullopt if either is missing or the end is before
    /// the start
    static std::optional<qint64> durationSecs(const BJobRun &run);

    /// The start is before the scheduled time: the times belong to
    /// another job (see the class comment)
    static bool hasForeignTimes(const BJobRun &run);

    // --- Helpers ------------------------------------------------------------

    /// Quantiles of a sample (see the class comment); count 0 for none
    static Quantiles quantiles(QList<qint64> values);

    /**
     * @brief The name of a catalog level code as Bareos spells it
     * @return "Full" for 'F', "Incremental" for 'I', "Differential" for
     *         'D', "VirtualFull" for 'f', "Base" for 'B', "Since" for 'S',
     *         the verify levels for 'C', 'V', 'O', 'd', 'A'; an empty
     *         string for ' ' (none: restore, admin) and for a null QChar;
     *         the code itself for any other character
     */
    static QString levelName(QChar code);

    /// "45s", "12m 05s", "3h 07m", "2d 04h"
    static QString formatDuration(qint64 secs);

    /// The local time of this computer as wall-clock time, comparable
    /// with BJobRun times if the Director is in the same time zone
    static QDateTime wallClockNow();

private:
    struct GroupData
    {
        QString job;
        QString level;
        QList<MeasuredRun> runs;
    };

    static Group condense(const QString &job, const QString &level,
                          const QList<MeasuredRun> &runs);
    static QString key(const QString &job, const QString &level);

    QMap<QString, GroupData> m_groups;          ///< By key(job, level)
    QMap<QString, QStringList> m_levelsOfJob;   ///< Job name -> its level names
    int m_windowDays = 0;
    Summary m_summary;
};

#endif // BJOBHISTORYSTATS_H
