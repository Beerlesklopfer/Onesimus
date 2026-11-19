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
 * Not replayed, and so not predicted: volume mounts and device limits in
 * the storage daemon (Maximum Concurrent Read Jobs, devices, autochangers),
 * loss of throughput when runs share a storage, spooling, cancelled
 * duplicate jobs, Max Start Delay and Max Wait Time, rescheduling after an
 * error, RunScripts, jobs started by hand, and the two seconds between two
 * looks of the Director at its queue. A Verify job at level Data takes two
 * places of its storage in the Director; it is counted with one here.
 *
 * Durations. A run lasts as long as it was measured (the caller passes the
 * median of BJobHistoryStats at the level of the run). Where nothing was
 * measured at the level there is no length to rely on, and where there is
 * no history at all there is no length. Limits that the Director did not
 * report are not known either. For all of these nothing is assumed in the
 * answer. Instead the queue is replayed twice:
 *  - once in favour of the runs: an estimate from other levels is taken
 *    as it is, a run without history ends the moment it starts, a limit
 *    that is not known does not limit, a run whose Allow Mixed Priority is
 *    not known has it;
 *  - once against them: a run that was not measured at its level may be
 *    running for up to UncertainSpanSecs (a day) and for at least its
 *    estimate, a limit that is not known is 1, Allow Mixed Priority that
 *    is not known is off.
 * The start and the wait of BWaitResult are those of the first replay. A
 * run whose start is the same in both is certain; otherwise it is not, and
 * BWaitResult::unknowns names what it depends on. UncertainSpanSecs is not
 * a duration that enters a prediction: it bounds how far the mark
 * "uncertain" reaches behind a run of unknown length.
 *
 * Dates. Only real time counts: two runs wait for each other if one is
 * still running, or waiting ahead in the queue, when the other is due.
 * Runs on different dates that do not last into each other never do. A
 * caller that shows a period should pass the runs of the day before it as
 * well, because they may still be running when the period begins.
 */

/** One scheduled start of one job. */
struct BWaitRun
{
    enum class Duration {
        Measured,   ///< Median of measured runs of this job at this level
        Estimated,  ///< Median over the job's other levels: an estimate
        Unknown,    ///< No history: the run has no known length
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

    /// Reads the reply of "show directors" (API mode 2). The limit is
    /// known only if the reply names exactly one Director.
    void readDirector(const QString &json);
    /// Reads the reply of "show storages"; replaces what was known
    void readStorages(const QString &json);
    /// Reads the reply of "show clients" or "show client=<name>"; a reply
    /// with several clients replaces what was known, one client is merged
    void readClients(const QString &json);

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

    static constexpr int DefaultMaximumConcurrentJobs = 1;  ///< Bareos, all four resources

private:
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
    };

    Kind kind = Kind::NoHistory;
    QString name;           ///< The storage, client or job; empty for the Director

    bool operator==(const BWaitUnknown &other) const
    {
        return kind == other.kind && name == other.name;
    }
};

/** The prediction for one run. */
struct BWaitResult
{
    QDateTime start;            ///< Predicted start; the scheduled time if it does not wait
    qint64 waitSecs = 0;        ///< start minus scheduled time
    QList<BWaitHold> holds;     ///< What held it, in order of time; empty if it does not wait
    bool certain = true;
    /// Not certain: what the start depends on. The holds that would apply
    /// if the unknowns turned out against the run are in @ref possibleHolds
    /// (their blockers and times are those of that replay, not a prediction).
    QList<BWaitUnknown> unknowns;
    QList<BWaitHold> possibleHolds;

    bool waits() const { return waitSecs > 0; }
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
    /// the replay against the runs (see the file comment)
    static constexpr qint64 UncertainSpanSecs = 24 * 3600;

    /**
     * @brief Replays the queue for these runs
     * @return one result per run, in the order of @p runs. A run without a
     *         valid scheduled time gets a default result.
     */
    static QList<BWaitResult> predict(const QList<BWaitRun> &runs, const BWaitLimits &limits);

    /// Figures over the results whose index is in @p indices
    static BWaitSummary summary(const QList<BWaitResult> &results, const QList<int> &indices);

    // --- Texts (plain, one statement per string) ---------------------------

    /// "0:53" (hours:minutes, rounded up to the minute)
    static QString formatWait(qint64 secs);

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

    /// "the duration of backup-a was never measured"
    static QString unknownText(const BWaitUnknown &unknown);

    /// One line for a list: the main reason of the wait, or what an
    /// uncertain run without a wait depends on; empty for neither
    static QString reasonLine(const BWaitRun &run, const BWaitResult &result);

    /**
     * @brief Everything about the wait of one run, a statement per entry
     *
     * Empty if the run does not wait and is certain. Used for the tooltip
     * of a bar and for the details of a row in the list.
     */
    static QStringList describe(int index, const QList<BWaitRun> &runs,
                                const QList<BWaitResult> &results);
};

#endif // BSCHEDULEWAITMODEL_H
