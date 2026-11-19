#ifndef BWAITCALIBRATION_H
#define BWAITCALIBRATION_H

#include <QCoreApplication>
#include <QDate>
#include <QDateTime>
#include <QHash>
#include <QList>
#include <QString>
#include <QStringList>

#include "schedules/model/bjobhistorystats.h"
#include "schedules/model/bschedulewaitmodel.h"

/**
 * @file bwaitcalibration.h
 * @brief The wait prediction measured against the catalog, without widgets
 *
 * BScheduleWaitModel replays the Director's queue with durations and
 * limits. This file takes what the catalog recorded ("llist jobs", the
 * runs of BJobHistoryReply) as the ground truth and derives from it what
 * the replay needs and the Director does not report, and replays past
 * nights to see how far the prediction is off. QtCore only.
 *
 * What the catalog times mean (Bareos branch bareos-25, commit 0e7742c;
 * the Onesimus Director, crates/odir/src/backup_worker.rs, does the same):
 *  - SchedTime: when the run was due.
 *  - StartTime: set when the job leaves the queue (dird/job.cc job_thread(),
 *    lines 424-425), set again after the Director's Run Before Job (lines
 *    469-470) and once more in dird/backup.cc DoNativeBackup() (lines
 *    613-614), after the connection to the storage daemon, the start of
 *    the job there, the connection to the client and Client Run Before Job
 *    (fd_cmds.cc SendRunscriptsCommands(), "RunBeforeNow", line 664). So
 *    StartTime - SchedTime is the queue of the Director plus that time.
 *  - With Just In Time Reservation (stored/stored_conf.cc, default yes since
 *    23.1) the storage daemon reserves a device only when the first data
 *    arrive (stored/reserve.cc lines 340-345; append.cc lines 523-527 call
 *    SetupDCR() and so TryReserveAfterUse()). A job that finds the device
 *    busy waits there (reserve.cc lines 260-273: twice WaitForDevice(), up
 *    to a minute or until a device is released; then Bmicrosleep(30, 0)
 *    between tries). That is after StartTime: EndTime - StartTime includes
 *    the wait for the device.
 *
 * Derived here, per job and level, from the runs that terminated normally
 * (status T or W) with scheduled, start and end time:
 *  - The measured wait (StartTime - SchedTime): median and maximum.
 *  - The start overhead (BWaitRun::startOverheadSecs): for every run, its
 *    start minus the later of its scheduled time and the last end, before
 *    its start, of a run that could have held it (same client, or another
 *    priority). The median over the runs of the job at the level; else over
 *    all levels of the job; else over all runs; else
 *    DefaultStartOverheadSecs.
 *  - The pure duration, when the number of parallel writers k of the
 *    storage is known: for every run, its end minus the later of its start
 *    and the k-th latest end of another run of the same storage that ended
 *    while it ran (with one writer: the run that held the device before
 *    it). The median over the runs of the job at the level. It still holds
 *    the slack of the 30 s tries, so it errs long by up to 30 s on a run
 *    that waited. Without k nothing is taken out: the duration is as
 *    measured and contains the wait for the device.
 *
 * The storage, client and priority of a job come from its configuration
 * (BWaitJobInfo); the client of a run from the catalog where it is given.
 */

/** What the configuration says about a job. */
struct BWaitJobInfo
{
    QString client;
    QString storage;        ///< Empty: not known
    int priority = 10;
};

class BWaitCalibration
{
    Q_DECLARE_TR_FUNCTIONS(BWaitCalibration)

public:
    /// Start overhead where nothing was measured on the Director. The runs
    /// that waited for nothing on the maintainer's Director took 3 to 6 s
    /// from SchedTime to StartTime (two connections with TLS, the job
    /// record); the smallest of these is taken so as not to invent a wait.
    static constexpr qint64 DefaultStartOverheadSecs = 3;

    /// Nights with more than one run queued at once before a history
    /// without overlapping runs suggests one writer
    static constexpr int SuggestMinNights = 3;
    /// Pairs of overlapping runs that tell one writer from several, before
    /// they suggest one writer
    static constexpr int SuggestMinPairs = 3;
    /// What a pair of runs may be off by: at least this, or a fifth of the
    /// shortest run of the later one when it ran alone
    static constexpr qint64 PairToleranceSecs = 10;

    /** One run as it counts here. */
    struct Run
    {
        QString job;
        QString level;          ///< Level name ("Incremental"); empty for none
        QString client;
        QString storage;        ///< From the configuration; empty: not known
        int priority = 10;
        QDateTime scheduled;
        QDateTime started;
        QDateTime ended;
        QDate night;            ///< The date of the evening it belongs to (noon to noon)
        qint64 waitSecs = 0;
        qint64 durationSecs = 0;
        qint64 startOverheadSecs = 0;
        qint64 pureSecs = 0;    ///< = durationSecs without a known device limit
    };

    enum class Basis {
        JobLevel,   ///< Runs of this job at this level
        Job,        ///< Runs of this job at all levels
        Director,   ///< All runs on this Director
        Default,    ///< Nothing measured: DefaultStartOverheadSecs
    };

    struct Overhead
    {
        qint64 secs = DefaultStartOverheadSecs;
        Basis basis = Basis::Default;
        int samples = 0;
    };

    struct Pure
    {
        bool valid = false;     ///< Runs of the job at the level were measured
        qint64 secs = 0;        ///< Median pure duration
        qint64 rawSecs = 0;     ///< Median duration as measured
        int samples = 0;
        int deviceLimit = 0;    ///< The writers it was derived with; 0: not known, not derived
    };

    /** What the history says about the parallel writers of a storage. */
    struct DeviceSuggestion
    {
        QString storage;
        int suggested = 0;          ///< 0: no suggestion
        int nights = 0;             ///< Nights with runs of the storage
        int contendedNights = 0;    ///< Nights with more than one run queued at once
        int overlapNights = 0;      ///< Nights with runs whose StartTime..EndTime overlap
        int informativePairs = 0;   ///< Overlapping pairs that tell one writer from several
        int serialPairs = 0;        ///< Of those: consistent with one writer
        int parallelPairs = 0;      ///< Of those: ended too soon after each other for one
        QStringList evidence;       ///< What was found, a statement per entry
    };

    struct BacktestRow
    {
        QString job;
        QString level;
        QDate night;
        QDateTime scheduled;
        QDateTime measuredStart;
        QDateTime predictedStart;
        qint64 errorSecs = 0;       ///< Predicted minus measured start
    };

    struct Backtest
    {
        int nights = 0;
        int runs = 0;
        qint64 medianAbsErrorSecs = 0;
        qint64 worstErrorSecs = 0;  ///< Signed: positive = predicted too late
        QString worstJob;
        QDate worstNight;
        QStringList unknownDevices; ///< Storages replayed without a device limit
        QList<BacktestRow> rows;

        /// "Back-test, last 7 nights (63 runs): median error 35 s, worst
        /// 3:10 min (BackupNextcloud, 2026-11-15) ..."
        QString text() const;
    };

    /**
     * @brief Replaces everything by what @p runs say
     * @param jobs    Client, storage and priority of the jobs by name
     * @param devices Parallel writers by storage; not in the map: not known
     */
    void setRuns(const QList<BJobRun> &runs, const QHash<QString, BWaitJobInfo> &jobs,
                 const QHash<QString, int> &devices);

    const QList<Run> &runs() const { return m_runs; }
    bool isEmpty() const { return m_runs.isEmpty(); }

    BWaitMeasured measuredWait(const QString &job, const QString &level) const;
    Overhead startOverhead(const QString &job, const QString &level) const;
    Pure pureDuration(const QString &job, const QString &level) const;

    /**
     * @brief Puts what was derived into a run that is to be replayed
     *
     * Sets the start overhead, the measured wait and, where runs of the job
     * at the level were measured, the duration (Measured) with a note on
     * how it was found. A run of a job without history keeps its duration.
     */
    void apply(BWaitRun *run) const;

    /// "measured median of 6 runs"; "no run measured, 3 s assumed"
    static QString overheadNote(const Overhead &overhead);
    /// How the duration was found; empty if nothing was measured
    static QString durationNote(const Pure &pure, const QString &storage);

    /// The storages the runs used, sorted
    QStringList storages() const;
    DeviceSuggestion suggestDevices(const QString &storage) const;

    /**
     * @brief Orders runs scheduled for the same moment as the history shows
     *        them to start: the one that used to wait less first
     *
     * The Director's own order in that case is that of its scheduler and is
     * not known; the measured waits show it. Stable for everything else.
     */
    static void orderTies(QList<BWaitRun> *runs);

    /**
     * @brief Replays the last @p nights nights with the model and compares
     *        the predicted with the measured starts
     *
     * Every measured run of a night is replayed with its scheduled time,
     * the durations and overheads of this calibration (taken from the same
     * history: the test is in sample) and @p limits. Runs that failed are
     * not in the catalog figures and so not replayed.
     */
    Backtest backtest(const BWaitLimits &limits, int nights) const;

private:
    static QString key(const QString &job, const QString &level);
    QList<int> indicesOf(const QString &job, const QString &level) const;

    QList<Run> m_runs;
    QHash<QString, QList<int>> m_byJobLevel;    ///< By key(job, level)
    QHash<QString, QList<int>> m_byJob;
    QHash<QString, BWaitJobInfo> m_jobs;
    QHash<QString, int> m_devices;
};

#endif // BWAITCALIBRATION_H
