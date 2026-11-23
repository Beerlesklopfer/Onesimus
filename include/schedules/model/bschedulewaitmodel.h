// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

#ifndef BSCHEDULEWAITMODEL_H
#define BSCHEDULEWAITMODEL_H

#include <QCoreApplication>
#include <QDateTime>
#include <QHash>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>

#include <optional>

/**
 * @file bschedulewaitmodel.h
 * @brief Predicted waiting time of scheduled runs, without widgets
 *
 * A run waits only when a limit of the Director is exceeded. This file
 * replays the job queue of the Director for the runs of a period and says,
 * for every run, when it is expected to start, how long it waits before
 * that, what holds it and whether the answer is certain. QtCore only; no
 * widgets and no Director.
 *
 * What is replayed (read on 2026-11-19 in Bareos, branch bareos-25, commit
 * 0e7742c: core/src/dird/jobq.cc JobqAdd(), jobq_server(),
 * AcquireResources(); core/src/dird/job.cc; core/src/dird/dird_conf.cc;
 * and in the Onesimus Director crates/odir/src/job_manager.rs schedule()
 * and acquire_resources(), which follow the same rules):
 *
 *  1. A run enters the wait queue at its scheduled time. The queue is kept
 *     sorted by priority (the lower number first); among runs of the same
 *     priority the one that entered first is first. Two runs of the same
 *     priority that are scheduled for the same moment keep the order in
 *     which they were handed to predict(); the Director's own order in that
 *     case is the order of its scheduler and is not known here.
 *  2. The queue is looked at whenever a run enters it or a run ends, and
 *     only while the Director has a free job slot (below). Looking at it:
 *     - The wanted priority is that of the run that has been running the
 *       longest; if nothing is running, that of the first waiting run.
 *     - The queue is walked from the front. A run is considered if its
 *       priority is the wanted one, or if it is higher (a lower number),
 *       the run has Allow Mixed Priority and so has every run that is
 *       running. The first run that is not considered ends the walk: no
 *       run behind it is started, whatever its priority. A run of a lower
 *       priority (a higher number) therefore waits until no run of another
 *       priority is running; Allow Mixed Priority does not help it.
 *     - A run that is considered needs a free place at its storage, its
 *       client and its Job resource, tried in this order (Maximum
 *       Concurrent Jobs of each; Bareos default 1). If one is full the walk
 *       goes on with the next run: a run that needs other resources may
 *       overtake.
 *  3. Maximum Concurrent Jobs of the Director is the number of runs that
 *     run at the same time. A run that has its places but no slot of the
 *     Director keeps the places and starts, in queue order, as soon as a
 *     slot is free; while every slot is taken the queue is not looked at.
 *  4. Jobs of the types Copy, Migrate and Consolidate take no place at a
 *     client or a storage (BWaitRun::usesClient, usesStorage).
 *
 *  5. The storage daemon (Bareos core/src/stored/reserve.cc
 *     TryReserveAfterUse(), wait.cc WaitForDevice(), append.cc SetupDCR()).
 *     With Just In Time Reservation (the default since Bareos 23.1,
 *     stored_conf.cc) a job the Director has started reserves a device only
 *     when its first data arrives (append.cc, "JustInTime Reservation:
 *     Finding drive to reserve"). That is after the Director recorded its
 *     start (dird/backup.cc DoNativeBackup() sets StartTime again after
 *     the connections and Client Run Before Job). A device writes as many
 *     jobs at a time as its Maximum Concurrent Jobs in the storage daemon
 *     (BWaitLimits::devices: "parallel writers" of a storage; the Director
 *     cannot read it). A job that finds every device busy keeps its places
 *     at the Director and waits inside the storage daemon: the first two
 *     times until a device is released or a minute has passed, from then
 *     on it sleeps 30 seconds between tries and is not woken
 *     (BWaitLimits::Sd). Its run therefore lasts longer by that wait, and
 *     so does the wait of every run that waits for its client or slot.
 *     Among tries at the same moment the one that asked first wins.
 *  6. Overheads. The catalog start of a run (StartTime) is set after the
 *     Director has connected to the storage daemon and the client and after
 *     Client Run Before Job (fd_cmds.cc SendRunscriptsCommands() runs it
 *     with "RunBeforeNow"). The run holds its places from the moment the
 *     Director starts it, so that time (BWaitRun::startOverheadSecs) is
 *     part of the measured wait, and the run has not started yet in the
 *     sense of the catalog. A run that writes asks for a device
 *     BWaitLimits::Sd::requestSecs after its start.
 *
 * Not replayed, and so not predicted: volume mounts and the other limits
 * in the storage daemon (Maximum Concurrent Read Jobs, autochangers,
 * several devices of different sizes), loss of throughput when runs share
 * a storage, spooling, cancelled
 * duplicate jobs, Max Start Delay and Max Wait Time, rescheduling after an
 * error, RunScripts, jobs started by hand, and the two seconds between two
 * looks of the Director at its queue. A Verify job at level Data takes two
 * places of its storage in the Director; it is counted with one here.
 * Schedules that no enabled job uses start nothing and are not passed in.
 *
 * Durations. A run lasts as long as it was measured (the caller passes the
 * median of BJobHistoryStats at the level of the run). Where nothing was
 * measured at the level there is no length to rely on, and where there is
 * no history at all there is no length. Limits that the Director did not
 * report are not known either. For all of these nothing is assumed in the
 * answer. Instead the queue is replayed twice:
 * (A run "lasts" here the time it writes when it does not have to wait
 * for a device: BWaitCalibration takes the device wait out of the measured
 * durations where it can, see there.)
 *  - once in favour of the runs: an estimate from other levels is taken
 *    as it is, a run without history ends the moment it starts, a limit
 *    that is not known does not limit, a run whose Allow Mixed Priority is
 *    not known has it;
 *  - once against them: a run that was not measured at its level may be
 *    running as long as its job is allowed to (below) and for at least its
 *    estimate, a limit that is not known is 1, Allow Mixed Priority that
 *    is not known is off.
 * A number of parallel writers that is not known (the default) is taken
 * the same way: no limit in favour of the runs, 1 against them.
 * The start and the wait of BWaitResult are those of the first replay. A
 * run whose start is the same in both is certain; otherwise it is not, and
 * BWaitResult::unknowns names what it depends on.
 *
 * How long a run that was not measured may be running. The Director cancels
 * a job that runs longer than its job allows (Bareos job.cc,
 * JobMonitorWatchdog(), JobCheckMaxruntime(), JobCheckMaxrunschedtime()):
 *  - Max Run Time, counted from the start of the run (jcr->start_time,
 *    set when the run has left the queue), for every level;
 *  - Full Max Runtime, Incremental Max Runtime and Differential Max
 *    Runtime, counted the same way, for a run of that level only (a
 *    Virtual Full is not a Full here). Where both a level limit and Max
 *    Run Time are set, the run is cancelled at the smaller one;
 *  - Max Run Sched Time, counted from the scheduled time, so that waiting
 *    is part of it. A Run line may override the job's value.
 * The smallest of these is the bound (BWaitRun::maxRunSecs and
 * maxRunSchedSecs, read by BWaitLimits::runTimeLimits()). A job without any
 * of them may be running for up to UncertainSpanSecs (a day). Neither is a
 * duration that enters a prediction: it bounds how far the mark "uncertain"
 * reaches behind a run of unknown length. The Director checks these limits
 * once a minute and a cancelled job needs a moment to end; that is not
 * added to the bound. A run the Director upgrades to another level (an
 * Incremental without a Full) is taken with the level of its Run line.
 *
 * Dates. Only real time counts: two runs wait for each other if one is
 * still running, or waiting ahead in the queue, when the other is due.
 * Runs on different dates that do not last into each other never do. A
 * caller that shows a period should pass the runs of the day before it as
 * well, because they may still be running when the period begins.
 */

/** What the catalog says this job waited before, at the level of a run. */
struct BWaitMeasured
{
    int count = 0;          ///< Runs with a known wait; 0: nothing measured
    qint64 medianSecs = 0;
    qint64 maxSecs = 0;

    bool isValid() const { return count > 0; }
};

/** One scheduled start of one job. */
struct BWaitRun
{
    enum class Duration {
        Measured,   ///< Median of measured runs of this job at this level
        Estimated,  ///< Median over the job's other levels: an estimate
        Unknown,    ///< No history: the run has no known length
    };

    /// The directive that bounds the length of a run (see the file comment)
    enum class Bound {
        None,                   ///< No limit is set: UncertainSpanSecs applies
        MaxRunTime,
        FullMaxRunTime,
        IncrementalMaxRunTime,
        DifferentialMaxRunTime,
        MaxRunSchedTime,
    };

    QString id;             ///< The caller's own key of the series this start belongs to
    QString job;            ///< Name of the Job resource
    QString client;         ///< Empty: the job has none
    QString storage;        ///< Empty with usesStorage: not known, and so is its limit
    QString level;          ///< For display only
    int priority = 10;
    std::optional<bool> allowMixedPriority;     ///< nullopt: not known
    bool usesClient = true;     ///< false: takes no place at its client
    bool usesStorage = true;    ///< false: takes no place at its storage
    QDateTime scheduled;
    Duration durationBasis = Duration::Unknown;
    qint64 durationSecs = 0;    ///< Not looked at for Duration::Unknown

    // The longest the Director lets this run last. Looked at only for a
    // run that was not measured at its level, and only in the replay
    // against the runs.
    qint64 maxRunSecs = 0;      ///< From its start; 0: no such limit
    Bound maxRunBound = Bound::None;    ///< The directive maxRunSecs is from
    qint64 maxRunSchedSecs = 0; ///< Max Run Sched Time, from the scheduled time; 0: none

    /// From the Director starting the run to its catalog start (connections,
    /// Client Run Before Job); the run holds its places meanwhile
    qint64 startOverheadSecs = 0;

    // For display only: the comparison with the past and where the figures
    // come from (BWaitCalibration)
    BWaitMeasured measuredWait;     ///< Not valid: nothing measured at this level
    QString durationNote;           ///< How durationSecs was found; empty: as measured
    QString overheadNote;           ///< How startOverheadSecs was found; empty: none
};

/**
 * @brief Maximum Concurrent Jobs of the Director and of its resources
 *
 * A name that is not in a map has a limit that is not known. A limit is
 * never made up: the read functions put a value in only where the
 * Director's reply settles it.
 *
 * How "show" replies are read (Bareos core/src/lib/res.cc,
 * PrintResourceItem(); Onesimus Director crates/odir/src/commands/show.rs):
 * without "verbose" the Director prints a directive only if it differs
 * from its default, and a Job does not print what it inherits from its
 * JobDefs. A resource that is in the reply without the directive therefore
 * has the default, which Bareos documents as 1 for Maximum Concurrent Jobs
 * (Director, Job, Client and Storage) and "no" for Allow Mixed Priority. A
 * resource that is not in a reply, and every resource while there is no
 * reply, is not known.
 */
struct BWaitLimits
{
    std::optional<int> director;        ///< nullopt: not known
    QHash<QString, int> jobs;
    QHash<QString, int> clients;
    QHash<QString, int> storages;
    /// Parallel writers of the devices of a storage, as the storage daemon
    /// allows them (Maximum Concurrent Jobs of its Device resources). The
    /// Director cannot read it: only the user sets it. Not in the map: not
    /// known.
    QHash<QString, int> devices;

    /** Timing of the storage daemon's reservation (see the file comment). */
    struct Sd
    {
        qint64 requestSecs = 1;     ///< From the catalog start to the first try
        int wakeWaits = 2;          ///< Tries after which it waits to be woken
        qint64 wakeWaitSecs = 60;   ///< wait.cc WaitForDevice(): max_wait_time
        qint64 retrySecs = 30;      ///< reserve.cc TryReserveAfterUse(): Bmicrosleep(30, 0)
    };
    Sd sd;

    /// Reads the reply of "show directors" (API mode 2). The limit is
    /// known only if the reply names exactly one Director.
    void readDirector(const QString &json);
    /// Reads the reply of "show storages"; replaces what was known
    void readStorages(const QString &json);
    /// Reads the reply of "show clients" or "show client=<name>"; a reply
    /// with several clients replaces what was known, one client is merged
    void readClients(const QString &json);
    /// The same from a reply that was parsed already (its root object)
    void readClients(const QJsonObject &reply);

    /**
     * @brief A directive of a job, looked for in the job and then in the
     *        JobDefs it names, one after the other
     * @param key   The directive as "show" spells it in JSON: lower case,
     *              no blanks ("maximumconcurrentjobs")
     * @param known false if a JobDefs the chain names is not in @p jobDefs:
     *              then the directive may be set there, and the result is
     *              an undefined value. true with an undefined value: the
     *              directive is set nowhere, the default applies.
     */
    static QJsonValue jobDirective(const QMap<QString, QJsonObject> &jobs,
                                   const QMap<QString, QJsonObject> &jobDefs,
                                   const QString &job, const QString &key,
                                   bool *known = nullptr);

    /// Fills @ref jobs from the replies of "show jobs" and "show jobdefs"
    void readJobs(const QMap<QString, QJsonObject> &jobConfigs,
                  const QMap<QString, QJsonObject> &jobDefs);

    /// A resource reference of a "show" reply: a string, {"name": ...}, or
    /// a list of these, of which the first counts
    static QString resourceName(const QJsonValue &value);
    /// A boolean of a "show" reply: true/false, or "yes"/"no"/"true"/"false"
    static std::optional<bool> boolValue(const QJsonValue &value);

    /**
     * @brief A time period of a "show" reply or of a Run line, in seconds
     *
     * Bareos prints a period as "12 hours " or "1 days 6 hours " (res.cc
     * PrintConfigTime()) and reads numbers with the modifiers seconds,
     * minutes (mins, n), hours, days, weeks, months (30 days), quarters
     * (91 days) and years (365 days), also abbreviated; a number without a
     * modifier is seconds (edit.cc DurationToUtime()). A JSON number is
     * seconds. nullopt for anything else.
     */
    static std::optional<qint64> timeValue(const QJsonValue &value);

    /** The limits of a job on how long one of its runs may last. */
    struct RunTimeLimits
    {
        qint64 maxRunSecs = 0;      ///< The smaller of Max Run Time and the level's limit; 0: none
        BWaitRun::Bound maxRunBound = BWaitRun::Bound::None;
        qint64 maxRunSchedSecs = 0; ///< Max Run Sched Time of the job; 0: none
    };

    /**
     * @brief Reads them from the job and the JobDefs it names
     * @param level The level of the run ("Full", "Incremental",
     *              "Differential"; anything else has no level limit)
     *
     * A value of 0, a value that cannot be read and a JobDefs that was not
     * reported all give "no limit": the caller then falls back to
     * BScheduleWaitModel::UncertainSpanSecs, which is the longer bound.
     */
    static RunTimeLimits runTimeLimits(const QMap<QString, QJsonObject> &jobs,
                                       const QMap<QString, QJsonObject> &jobDefs,
                                       const QString &job, const QString &level);

    static constexpr int DefaultMaximumConcurrentJobs = 1;  ///< Bareos, all four resources

private:
    static QHash<QString, int> readResourceLimits(const QJsonObject &reply, const QString &kind,
                                                  bool *ok);
    static QHash<QString, int> readResourceLimits(const QString &json, const QString &kind,
                                                  bool *ok);
};

/** Why a run did not start for a while. */
struct BWaitHold
{
    enum class Reason {
        DirectorLimit,  ///< Every job slot of the Director was taken
        StorageLimit,   ///< No free place at the storage
        ClientLimit,    ///< No free place at the client
        JobLimit,       ///< The Job resource was running as often as it may
        Priority,       ///< Runs of another priority were running
        QueueOrder,     ///< A run of a higher priority was waiting ahead in the queue
    };

    Reason reason = Reason::Priority;
    QString resource;       ///< The storage, client or job of a limit; otherwise empty
    int limit = 0;          ///< The limit that was reached; 0 for the priority reasons
    bool limitKnown = true; ///< false: the limit is not known and 1 was tried
    int otherPriority = 0;  ///< Priority, QueueOrder: the priority that was in the way
    QDateTime from;
    QDateTime until;
    QList<int> blockers;    ///< Indices of the runs it was held by, in ascending order

    qint64 secs() const { return from.secsTo(until); }
};

/** Something a prediction depends on that is not known. */
struct BWaitUnknown
{
    enum class Kind {
        DirectorLimit,
        StorageLimit,
        ClientLimit,
        JobLimit,
        MixedPriority,      ///< Allow Mixed Priority of a job
        EstimatedDuration,  ///< A job measured at other levels only
        NoHistory,          ///< A job that was never measured
        DeviceLimit,        ///< Parallel writers of the devices of a storage
    };

    Kind kind = Kind::NoHistory;
    QString name;           ///< The storage, client or job; empty for the Director

    // EstimatedDuration, NoHistory: how long the run of that job that is in
    // the way may be running, and which limit of its job says so. Not part
    // of the comparison: two runs of the job are one unknown.
    QDateTime until;        ///< Invalid: not worked out
    BWaitRun::Bound bound = BWaitRun::Bound::None;
    qint64 boundSecs = 0;   ///< The value of that limit; UncertainSpanSecs for Bound::None

    bool operator==(const BWaitUnknown &other) const
    {
        return kind == other.kind && name == other.name;
    }
};

/** The prediction for one run. */
struct BWaitResult
{
    /// Predicted start as the catalog records it (StartTime): the Director's
    /// start plus the start overhead; the scheduled time if neither applies
    QDateTime start;
    qint64 waitSecs = 0;        ///< start minus scheduled time, comparable with the measured wait
    QList<BWaitHold> holds;     ///< What held it, in order of time; empty if it does not wait
    bool certain = true;
    /// Not certain: what the start depends on. The holds that would apply
    /// if the unknowns turned out against the run are in @ref possibleHolds
    /// (their blockers and times are those of that replay, not a prediction).
    QList<BWaitUnknown> unknowns;
    QList<BWaitHold> possibleHolds;

    // Inside the run: the wait for a device of the storage daemon
    QDateTime end;              ///< Predicted end (invalid for a run of unknown length)
    qint64 deviceWaitSecs = 0;  ///< Waited inside the storage daemon for a device
    int deviceLimit = 0;        ///< The parallel writers that applied; 0: none
    QList<int> deviceBlockers;  ///< Runs that held the device when it first asked

    /// Held by something. A run whose wait is only its start overhead
    /// (BWaitRun::startOverheadSecs) does not wait in this sense.
    bool waits() const { return waitSecs > 0 && !holds.isEmpty(); }
    /// The hold that lasted longest; nullptr if it does not wait
    const BWaitHold *mainHold() const;
};

/** Figures over a set of results. */
struct BWaitSummary
{
    int runs = 0;
    int waiting = 0;            ///< Runs with a predicted wait
    int uncertain = 0;          ///< Runs whose prediction is not certain
    qint64 longestSecs = 0;
    qint64 totalSecs = 0;
};

class BScheduleWaitModel
{
    Q_DECLARE_TR_FUNCTIONS(BScheduleWaitModel)

public:
    /// How long a run that was not measured at its level may be running in
    /// the replay against the runs if its job sets no limit (see the file
    /// comment)
    static constexpr qint64 UncertainSpanSecs = 24 * 3600;

    /// A wait shorter than this is shown with seconds, and so is its start
    static constexpr qint64 SecondsShownBelow = 10 * 60;

    /**
     * @brief How long a run that was not measured may be running at most
     * @param startSecs Seconds from the scheduled time to its start (its wait)
     * @param which     If given: the limit that says so
     * @return seconds from its start; UncertainSpanSecs without a limit
     */
    static qint64 upperBoundSecs(const BWaitRun &run, qint64 startSecs,
                                 BWaitRun::Bound *which = nullptr);

    /**
     * @brief Replays the queue for these runs
     * @return one result per run, in the order of @p runs. A run without a
     *         valid scheduled time gets a default result.
     */
    static QList<BWaitResult> predict(const QList<BWaitRun> &runs, const BWaitLimits &limits);

    /// Figures over the results whose index is in @p indices
    static BWaitSummary summary(const QList<BWaitResult> &results, const QList<int> &indices);

    // --- Texts (plain, one statement per string) ---------------------------

    /**
     * @brief A waiting time for display, the same everywhere
     *
     * Under a minute "27 s"; under ten minutes minutes and seconds,
     * "1:35 min"; from there on hours and minutes, rounded up to the
     * minute, "0:53 h". The unit is always written.
     */
    static QString formatWait(qint64 secs);

    /// A predicted start is shown with seconds if its wait is shorter than
    /// SecondsShownBelow: otherwise it reads like the scheduled time
    static bool startShowsSeconds(qint64 waitSecs)
    {
        return waitSecs > 0 && waitSecs < SecondsShownBelow;
    }

    /// A limit for display: "12 h", "90 min", "1 d 6 h"
    static QString formatSpan(qint64 secs);

    /// "Max Run Time 12 h", or "no Max Run Time set, 24 h assumed"
    static QString boundText(BWaitRun::Bound bound, qint64 secs);

    /// The counter line: "3 run(s) wait, longest 0:53, 2 uncertain" or
    /// "No waiting predicted"
    static QString summaryText(const BWaitSummary &summary);

    /// What a hold was about, with the limit and its value or the priority
    /// rule: "Storage File runs 1 job at a time (Maximum Concurrent Jobs = 1)"
    static QString holdText(const BWaitHold &hold, const BWaitRun &run);

    /// "backup-a (Full, 23:09 to 00:03), ..." for the runs a hold names;
    /// at most @p most of them, then "and N more"
    static QString blockersText(const BWaitHold &hold, const QList<BWaitRun> &runs,
                                const QList<BWaitResult> &results, int most = 4);

    /**
     * @brief What is not known, in words
     *
     * "the duration of backup-a was never measured: it may run until 14:00
     * (Max Run Time 12 h)". The time is left out if @p withUntil is false
     * (a statement about several runs) or if it was not worked out; then
     * the text is "... never measured (Max Run Time 12 h)".
     * @param reference Times on this date are written without a date
     */
    static QString unknownText(const BWaitUnknown &unknown, const QDate &reference = QDate(),
                               bool withUntil = true);

    /**
     * @brief The unknowns that several runs of a list depend on
     * @param indices The runs of the list
     * @param atLeast An unknown that fewer runs depend on is not shared
     * @return in the order of their first appearance; the list states each
     *         of them once and marks its rows with "[1]", "[2]", ...
     *         (reasonLine())
     */
    static QList<BWaitUnknown> sharedUnknowns(const QList<BWaitResult> &results,
                                              const QList<int> &indices, int atLeast = 3);

    /// How many of the runs depend on the unknown
    static int dependingRuns(const BWaitUnknown &unknown, const QList<BWaitResult> &results,
                             const QList<int> &indices);

    /**
     * @brief One line for a list: the main reason of the wait, or what an
     *        uncertain run without a wait depends on; empty for neither
     * @param shared Unknowns that are stated once beside the list: named
     *               here by their number, "[1]", instead of in words
     */
    static QString reasonLine(const BWaitRun &run, const BWaitResult &result,
                              const QList<BWaitUnknown> &shared = QList<BWaitUnknown>());

    /**
     * @brief Everything about the wait of one run, a statement per entry
     *
     * Empty if the run does not wait and is certain. Used for the tooltip
     * of a bar and for the details of a row in the list.
     */
    static QStringList describe(int index, const QList<BWaitRun> &runs,
                                const QList<BWaitResult> &results);

    /**
     * @brief The timing of a run, a statement per entry: the wait for a
     *        device inside the run, its end, the start overhead, how its
     *        duration was found, the measured wait and a discrepancy
     */
    static QStringList describeTiming(int index, const QList<BWaitRun> &runs,
                                      const QList<BWaitResult> &results,
                                      const BWaitLimits &limits);

    // --- Predicted against measured ----------------------------------------

    enum class Discrepancy {
        None,       ///< Close enough, or nothing measured
        TooLow,     ///< Measured waits are much longer than predicted
        TooHigh,    ///< Measured waits are much shorter than predicted
    };

    /// A difference counts if it is more than a minute and more than half
    /// of the measured median (DiscrepancyMinSecs, DiscrepancyShare)
    static constexpr qint64 DiscrepancyMinSecs = 60;
    static constexpr double DiscrepancyShare = 0.5;

    static Discrepancy discrepancy(const BWaitRun &run, const BWaitResult &result);

    /// "median 4:45 min, max 6:55 min (6 runs)"; "not measured"
    static QString measuredText(const BWaitMeasured &measured);

    /// "Prediction too low: measured median 4:45 min, predicted 40 s - a
    /// limit the GUI does not know, ..."; empty for Discrepancy::None
    static QString discrepancyText(const BWaitRun &run, const BWaitResult &result);
};

#endif // BSCHEDULEWAITMODEL_H
