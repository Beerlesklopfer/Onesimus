#include "schedules/model/bschedulewaitmodel.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QSet>
#include <QVector>

#include <algorithm>
#include <limits>

// ============================================================================
// Limits: reading "show" replies
// ============================================================================

namespace {

/// "Maximum Concurrent Jobs", "MaximumConcurrentJobs" -> "maximumconcurrentjobs"
QString normalizedKey(const QString &key)
{
    QString out;
    out.reserve(key.size());
    for (const QChar c : key) {
        if (!c.isSpace())
            out.append(c.toLower());
    }
    return out;
}

/// The value of a directive whatever the spelling of its key; undefined if
/// the object does not have it
QJsonValue directiveOf(const QJsonObject &object, const QString &key)
{
    const auto direct = object.constFind(key);
    if (direct != object.constEnd())
        return direct.value();
    for (auto it = object.constBegin(); it != object.constEnd(); ++it) {
        if (normalizedKey(it.key()) == key)
            return it.value();
    }
    return QJsonValue(QJsonValue::Undefined);
}

/// A positive number of a "show" reply, given as a number or as a string
std::optional<int> positiveInt(const QJsonValue &value)
{
    bool ok = false;
    int number = 0;
    if (value.isDouble()) {
        number = value.toInt(0);
        ok = true;
    } else if (value.isString()) {
        number = value.toString().trimmed().toInt(&ok);
    }
    if (!ok || number < 1)
        return std::nullopt;
    return number;
}

const QString kMaximumConcurrentJobs = QStringLiteral("maximumconcurrentjobs");

/// Maximum Concurrent Jobs of a resource that is in a reply: its value, or
/// the default if the reply does not print it. nullopt for a value that
/// cannot be read: then nothing is known.
std::optional<int> limitOfResource(const QJsonObject &resource)
{
    const QJsonValue value = directiveOf(resource, kMaximumConcurrentJobs);
    if (value.isUndefined() || value.isNull())
        return BWaitLimits::DefaultMaximumConcurrentJobs;
    return positiveInt(value);
}

} // namespace

QString BWaitLimits::resourceName(const QJsonValue &value)
{
    // A list of resources (the storages of a job): the first is the one used
    if (value.isArray())
        return value.toArray().isEmpty() ? QString() : resourceName(value.toArray().first());
    if (value.isObject())
        return value.toObject().value(QLatin1String("name")).toString();
    return value.toString();
}

std::optional<bool> BWaitLimits::boolValue(const QJsonValue &value)
{
    if (value.isBool())
        return value.toBool();
    if (value.isString()) {
        const QString text = value.toString().trimmed().toLower();
        if (text == QLatin1String("yes") || text == QLatin1String("true") || text == QLatin1String("1"))
            return true;
        if (text == QLatin1String("no") || text == QLatin1String("false") || text == QLatin1String("0"))
            return false;
    }
    return std::nullopt;
}

QHash<QString, int> BWaitLimits::readResourceLimits(const QString &json, const QString &kind,
                                                    bool *ok)
{
    QHash<QString, int> limits;
    *ok = false;

    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isObject())
        return limits;
    const QJsonValue resources = doc.object().value(QLatin1String("result")).toObject().value(kind);
    // An error reply, a text reply or another command's reply has no such object
    if (!resources.isObject())
        return limits;

    *ok = true;
    const QJsonObject byName = resources.toObject();
    for (auto it = byName.constBegin(); it != byName.constEnd(); ++it) {
        if (!it.value().isObject())
            continue;
        const QJsonObject resource = it.value().toObject();
        QString name = resource.value(QLatin1String("name")).toString();
        if (name.isEmpty())
            name = it.key();
        const std::optional<int> limit = limitOfResource(resource);
        if (limit)
            limits.insert(name, *limit);
    }
    return limits;
}

void BWaitLimits::readDirector(const QString &json)
{
    bool ok = false;
    const QHash<QString, int> found = readResourceLimits(json, QStringLiteral("directors"), &ok);
    // A Director has one Director resource. Anything else is not understood.
    if (ok && found.size() == 1)
        director = found.constBegin().value();
    else
        director.reset();
}

void BWaitLimits::readStorages(const QString &json)
{
    bool ok = false;
    const QHash<QString, int> found = readResourceLimits(json, QStringLiteral("storages"), &ok);
    storages = ok ? found : QHash<QString, int>();
}

void BWaitLimits::readClients(const QString &json)
{
    bool ok = false;
    const QHash<QString, int> found = readResourceLimits(json, QStringLiteral("clients"), &ok);
    if (!ok)
        return;     // not a reply about clients: what is known stays
    if (found.size() == 1) {
        // "show client=<name>", or a Director with one client
        clients.insert(found.constBegin().key(), found.constBegin().value());
    } else {
        clients = found;
    }
}

QJsonValue BWaitLimits::jobDirective(const QMap<QString, QJsonObject> &jobs,
                                     const QMap<QString, QJsonObject> &jobDefs,
                                     const QString &job, const QString &key, bool *known)
{
    const QJsonValue undefined(QJsonValue::Undefined);
    if (known) *known = false;

    const auto jobIt = jobs.constFind(job);
    if (jobIt == jobs.constEnd())
        return undefined;

    QJsonObject resource = jobIt.value();
    // The JobDefs of a JobDefs: followed a few steps, never in a circle
    for (int depth = 0; depth < 8; ++depth) {
        const QJsonValue value = directiveOf(resource, key);
        if (!value.isUndefined()) {
            if (known) *known = true;
            return value;
        }
        const QString defs = resourceName(directiveOf(resource, QStringLiteral("jobdefs")));
        if (defs.isEmpty()) {
            // Set nowhere: the default applies
            if (known) *known = true;
            return undefined;
        }
        const auto defsIt = jobDefs.constFind(defs);
        if (defsIt == jobDefs.constEnd())
            return undefined;   // may be set in a JobDefs that was not reported
        resource = defsIt.value();
    }
    return undefined;
}

void BWaitLimits::readJobs(const QMap<QString, QJsonObject> &jobConfigs,
                           const QMap<QString, QJsonObject> &jobDefs)
{
    jobs.clear();
    for (auto it = jobConfigs.constBegin(); it != jobConfigs.constEnd(); ++it) {
        bool known = false;
        const QJsonValue value = jobDirective(jobConfigs, jobDefs, it.key(),
                                              kMaximumConcurrentJobs, &known);
        if (!known)
            continue;
        if (value.isUndefined() || value.isNull()) {
            jobs.insert(it.key(), DefaultMaximumConcurrentJobs);
        } else if (const std::optional<int> limit = positiveInt(value)) {
            jobs.insert(it.key(), *limit);
        }
    }
}

// ============================================================================
// The replay
// ============================================================================

namespace {

const int kNoLimit = std::numeric_limits<int>::max();

struct Limit
{
    int value = kNoLimit;
    bool known = true;
};

/**
 * One replay of the queue. Times are seconds since the epoch.
 *
 * @p against selects how what is not known is taken: in favour of the runs
 * (false) or against them (true); see the file comment of the header.
 */
class Replay
{
public:
    Replay(const QList<BWaitRun> &runs, const BWaitLimits &limits, bool against)
        : m_runs(runs), m_limits(limits), m_against(against), m_state(runs.size())
    {
        run();
    }

    bool started(int i) const { return m_state[i].started; }
    qint64 start(int i) const { return m_state[i].start; }
    const QList<BWaitHold> &holds(int i) const { return m_state[i].holds; }

private:
    struct State
    {
        qint64 scheduled = 0;
        qint64 start = 0;
        qint64 end = 0;
        bool started = false;
        bool holdOpen = false;
        QList<BWaitHold> holds;
    };

    /// What a waiting run is held by at the moment
    struct Held
    {
        BWaitHold::Reason reason = BWaitHold::Reason::Priority;
        QString resource;
        Limit limit;
        int otherPriority = 0;
        QList<int> blockers;
    };

    // --- What is not known, taken one way or the other ---

    qint64 duration(int i) const
    {
        const BWaitRun &r = m_runs[i];
        switch (r.durationBasis) {
        case BWaitRun::Duration::Measured:
            return qMax<qint64>(1, r.durationSecs);
        case BWaitRun::Duration::Estimated:
            return m_against ? qMax<qint64>(r.durationSecs, BScheduleWaitModel::UncertainSpanSecs)
                             : qMax<qint64>(1, r.durationSecs);
        case BWaitRun::Duration::Unknown:
            break;
        }
        return m_against ? BScheduleWaitModel::UncertainSpanSecs : 0;
    }

    bool mixes(int i) const
    {
        const std::optional<bool> &mix = m_runs[i].allowMixedPriority;
        return mix ? *mix : !m_against;
    }

    Limit unknownLimit() const
    {
        Limit limit;
        limit.known = false;
        limit.value = m_against ? BWaitLimits::DefaultMaximumConcurrentJobs : kNoLimit;
        return limit;
    }

    Limit directorLimit() const
    {
        if (!m_limits.director)
            return unknownLimit();
        Limit limit;
        limit.value = qMax(1, *m_limits.director);
        return limit;
    }

    Limit limitIn(const QHash<QString, int> &map, const QString &name) const
    {
        const auto it = map.constFind(name);
        if (it == map.constEnd())
            return unknownLimit();
        Limit limit;
        limit.value = qMax(1, it.value());
        return limit;
    }

    // --- Places ---

    /// A run that uses a storage whose name is not known (empty) is taken
    /// to share one storage of unknown limit with the other such runs
    bool usesStorage(int i) const { return m_runs[i].usesStorage; }
    bool usesClient(int i) const { return m_runs[i].usesClient && !m_runs[i].client.isEmpty(); }

    /// The runs that hold a place at the resource the run @p i asks for:
    /// the running ones and those that only wait for a slot of the Director
    QList<int> holders(int i, BWaitHold::Reason what) const
    {
        QList<int> found;
        auto look = [&](const QList<int> &list) {
            for (int other : list) {
                bool same = false;
                switch (what) {
                case BWaitHold::Reason::StorageLimit:
                    same = usesStorage(other) && m_runs[other].storage == m_runs[i].storage;
                    break;
                case BWaitHold::Reason::ClientLimit:
                    same = usesClient(other) && m_runs[other].client == m_runs[i].client;
                    break;
                case BWaitHold::Reason::JobLimit:
                    same = m_runs[other].job == m_runs[i].job;
                    break;
                default:
                    break;
                }
                if (same) found.append(other);
            }
        };
        look(m_running);
        look(m_ready);
        return found;
    }

    /// jobq.cc AcquireResources(): storage, client, job. true: all free.
    bool placesFree(int i, Held *held) const
    {
        struct Check { bool applies; BWaitHold::Reason reason; Limit limit; QString name; };
        const Check checks[] = {
            { usesStorage(i), BWaitHold::Reason::StorageLimit,
              m_runs[i].storage.isEmpty() ? unknownLimit()
                                          : limitIn(m_limits.storages, m_runs[i].storage),
              m_runs[i].storage },
            { usesClient(i), BWaitHold::Reason::ClientLimit,
              limitIn(m_limits.clients, m_runs[i].client), m_runs[i].client },
            { true, BWaitHold::Reason::JobLimit,
              limitIn(m_limits.jobs, m_runs[i].job), m_runs[i].job },
        };
        for (const Check &check : checks) {
            if (!check.applies)
                continue;
            const QList<int> taken = holders(i, check.reason);
            if (taken.size() >= check.limit.value) {
                if (held) {
                    held->reason = check.reason;
                    held->resource = check.name;
                    held->limit = check.limit;
                    held->blockers = taken;
                }
                return false;
            }
        }
        return true;
    }

    // --- The queue ---

    /// jobq.cc JobqAdd(): before the first waiting run of a lower priority
    void enqueue(int i)
    {
        int at = m_waiting.size();
        for (int k = 0; k < m_waiting.size(); ++k) {
            if (m_runs[m_waiting[k]].priority > m_runs[i].priority) {
                at = k;
                break;
            }
        }
        m_waiting.insert(at, i);
    }

    void begin(int i, qint64 now)
    {
        State &s = m_state[i];
        s.started = true;
        s.start = now;
        s.end = now + duration(i);
        if (s.holdOpen) {
            s.holds.last().until = QDateTime::fromSecsSinceEpoch(now);
            s.holdOpen = false;
        }
        m_running.append(i);
    }

    /// The priority the Director wants now, and whether every running run
    /// allows mixing. false: nothing runs and nothing waits.
    bool wanted(int *priority, bool *runningMix) const
    {
        if (!m_running.isEmpty()) {
            *priority = m_runs[m_running.first()].priority;
            *runningMix = true;
            for (int i : m_running) {
                if (!mixes(i)) {
                    *runningMix = false;
                    break;
                }
            }
            return true;
        }
        if (m_waiting.isEmpty())
            return false;
        *priority = m_runs[m_waiting.first()].priority;
        *runningMix = false;
        return true;
    }

    bool considered(int i, int priority, bool runningMix) const
    {
        const int mine = m_runs[i].priority;
        return mine == priority || (mine < priority && mixes(i) && runningMix);
    }

    /// One look at the wait queue (jobq_server()). Runs that get their
    /// places move to the ready list. true: at least one did.
    bool look()
    {
        int priority = 0;
        bool runningMix = false;
        if (!wanted(&priority, &runningMix))
            return false;

        bool moved = false;
        int k = 0;
        while (k < m_waiting.size()) {
            const int i = m_waiting[k];
            if (!considered(i, priority, runningMix))
                break;
            if (!placesFree(i, nullptr)) {
                ++k;
                continue;
            }
            m_waiting.removeAt(k);
            m_ready.append(i);
            moved = true;
        }
        return moved;
    }

    /// Everything that happens at one moment, until nothing changes
    void settle(qint64 now)
    {
        const int jobSlots = directorLimit().value;
        for (;;) {
            bool progress = false;

            for (int k = m_running.size() - 1; k >= 0; --k) {
                if (m_state[m_running[k]].end <= now) {
                    m_running.removeAt(k);
                    progress = true;
                }
            }
            while (!m_ready.isEmpty() && m_running.size() < jobSlots) {
                begin(m_ready.takeFirst(), now);
                progress = true;
            }
            // Only a free worker looks at the wait queue
            if (m_running.size() < jobSlots && look())
                progress = true;

            if (!progress)
                break;
        }
        noteHolds(now);
    }

    // --- Why the runs that are left are held ---

    void note(int i, qint64 now, Held held)
    {
        std::sort(held.blockers.begin(), held.blockers.end());
        State &s = m_state[i];
        const QDateTime when = QDateTime::fromSecsSinceEpoch(now);

        if (s.holdOpen) {
            BWaitHold &last = s.holds.last();
            if (last.reason == held.reason && last.resource == held.resource
                && last.otherPriority == held.otherPriority) {
                for (int blocker : std::as_const(held.blockers)) {
                    if (!last.blockers.contains(blocker))
                        last.blockers.append(blocker);
                }
                std::sort(last.blockers.begin(), last.blockers.end());
                return;
            }
            last.until = when;
        }

        BWaitHold hold;
        hold.reason = held.reason;
        hold.resource = held.resource;
        const bool isLimit = held.reason != BWaitHold::Reason::Priority
                             && held.reason != BWaitHold::Reason::QueueOrder;
        hold.limit = isLimit ? held.limit.value : 0;
        hold.limitKnown = !isLimit || held.limit.known;
        hold.otherPriority = held.otherPriority;
        hold.from = when;
        hold.blockers = held.blockers;
        s.holds.append(hold);
        s.holdOpen = true;
    }

    void noteHolds(qint64 now)
    {
        const Limit jobSlots = directorLimit();

        Held noSlot;
        noSlot.reason = BWaitHold::Reason::DirectorLimit;
        noSlot.limit = jobSlots;
        noSlot.blockers = m_running;

        // Has its places, waits for a slot of the Director
        for (int i : std::as_const(m_ready))
            note(i, now, noSlot);

        if (m_waiting.isEmpty())
            return;

        // Every slot taken: the queue is not looked at
        if (m_running.size() >= jobSlots.value) {
            for (int i : std::as_const(m_waiting))
                note(i, now, noSlot);
            return;
        }

        int priority = 0;
        bool runningMix = false;
        wanted(&priority, &runningMix);

        int stoppedAt = -1;     // the run that ended the walk
        for (int k = 0; k < m_waiting.size(); ++k) {
            const int i = m_waiting[k];
            const int mine = m_runs[i].priority;
            Held held;

            if (stoppedAt >= 0 && mine == priority) {
                // Would be considered, but the walk ended before it
                held.reason = BWaitHold::Reason::QueueOrder;
                held.otherPriority = m_runs[stoppedAt].priority;
                held.blockers.append(stoppedAt);
            } else if (stoppedAt >= 0 || !considered(i, priority, runningMix)) {
                if (stoppedAt < 0)
                    stoppedAt = i;
                held.reason = BWaitHold::Reason::Priority;
                held.otherPriority = priority;
                for (int other : std::as_const(m_running)) {
                    if (m_runs[other].priority != mine)
                        held.blockers.append(other);
                }
                // A higher priority that may not mix is held by all of them
                if (held.blockers.isEmpty())
                    held.blockers = m_running;
            } else if (placesFree(i, &held)) {
                continue;   // cannot be after settle(); nothing to note
            }
            note(i, now, held);
        }
    }

    void run()
    {
        QVector<int> order;
        for (int i = 0; i < m_runs.size(); ++i) {
            if (!m_runs[i].scheduled.isValid())
                continue;
            m_state[i].scheduled = m_runs[i].scheduled.toSecsSinceEpoch();
            order.append(i);
        }
        std::stable_sort(order.begin(), order.end(), [this](int a, int b) {
            return m_state[a].scheduled < m_state[b].scheduled;
        });

        int next = 0;
        for (;;) {
            bool have = false;
            qint64 now = 0;
            if (next < order.size()) {
                now = m_state[order[next]].scheduled;
                have = true;
            }
            for (int i : std::as_const(m_running)) {
                if (!have || m_state[i].end < now) {
                    now = m_state[i].end;
                    have = true;
                }
            }
            if (!have)
                break;

            while (next < order.size() && m_state[order[next]].scheduled <= now)
                enqueue(order[next++]);
            settle(now);
        }
    }

    const QList<BWaitRun> &m_runs;
    const BWaitLimits &m_limits;
    const bool m_against;
    QVector<State> m_state;
    QList<int> m_waiting;   ///< Sorted by priority, then by arrival
    QList<int> m_ready;     ///< Have their places, wait for a slot of the Director
    QList<int> m_running;   ///< In the order in which they started
};

void addUnknown(QList<BWaitUnknown> *list, BWaitUnknown::Kind kind, const QString &name)
{
    BWaitUnknown unknown;
    unknown.kind = kind;
    unknown.name = name;
    if (!list->contains(unknown))
        list->append(unknown);
}

/// What the start of run @p index depends on that is not known: followed
/// from its holds to the runs that held it and on to what held those
QList<BWaitUnknown> unknownsOf(int index, const QList<BWaitRun> &runs,
                               const Replay &favour, const Replay &against)
{
    QList<BWaitUnknown> unknowns;
    QSet<int> seen;
    QList<int> todo;
    todo.append(index);
    seen.insert(index);

    while (!todo.isEmpty()) {
        const int i = todo.takeFirst();
        const BWaitRun &run = runs[i];

        for (const Replay *replay : { &against, &favour }) {
            for (const BWaitHold &hold : replay->holds(i)) {
                switch (hold.reason) {
                case BWaitHold::Reason::DirectorLimit:
                    if (!hold.limitKnown) addUnknown(&unknowns, BWaitUnknown::Kind::DirectorLimit, QString());
                    break;
                case BWaitHold::Reason::StorageLimit:
                    if (!hold.limitKnown) addUnknown(&unknowns, BWaitUnknown::Kind::StorageLimit, hold.resource);
                    break;
                case BWaitHold::Reason::ClientLimit:
                    if (!hold.limitKnown) addUnknown(&unknowns, BWaitUnknown::Kind::ClientLimit, hold.resource);
                    break;
                case BWaitHold::Reason::JobLimit:
                    if (!hold.limitKnown) addUnknown(&unknowns, BWaitUnknown::Kind::JobLimit, hold.resource);
                    break;
                case BWaitHold::Reason::Priority:
                    // Only a higher priority can start beside others
                    if (run.priority < hold.otherPriority) {
                        if (!run.allowMixedPriority)
                            addUnknown(&unknowns, BWaitUnknown::Kind::MixedPriority, run.job);
                        for (int blocker : hold.blockers) {
                            if (!runs[blocker].allowMixedPriority)
                                addUnknown(&unknowns, BWaitUnknown::Kind::MixedPriority, runs[blocker].job);
                        }
                    }
                    break;
                case BWaitHold::Reason::QueueOrder:
                    break;
                }

                for (int blocker : hold.blockers) {
                    const BWaitRun &other = runs[blocker];
                    if (other.durationBasis == BWaitRun::Duration::Estimated)
                        addUnknown(&unknowns, BWaitUnknown::Kind::EstimatedDuration, other.job);
                    else if (other.durationBasis == BWaitRun::Duration::Unknown)
                        addUnknown(&unknowns, BWaitUnknown::Kind::NoHistory, other.job);
                    // Its own start is not certain: look at what held it
                    if (favour.start(blocker) != against.start(blocker) && !seen.contains(blocker)) {
                        seen.insert(blocker);
                        todo.append(blocker);
                    }
                }
            }
        }
    }
    return unknowns;
}

QString clock(const QDateTime &when, const QDate &reference)
{
    if (!when.isValid())
        return QStringLiteral("?");
    return when.date() == reference ? when.toString(QStringLiteral("HH:mm"))
                                    : when.toString(QStringLiteral("yyyy-MM-dd HH:mm"));
}

} // namespace

// ============================================================================
// BScheduleWaitModel
// ============================================================================

const BWaitHold *BWaitResult::mainHold() const
{
    const BWaitHold *longest = nullptr;
    for (const BWaitHold &hold : holds) {
        if (!longest || hold.secs() > longest->secs())
            longest = &hold;
    }
    return longest;
}

QList<BWaitResult> BScheduleWaitModel::predict(const QList<BWaitRun> &runs,
                                               const BWaitLimits &limits)
{
    QList<BWaitResult> results;
    results.reserve(runs.size());

    const Replay favour(runs, limits, false);
    const Replay against(runs, limits, true);

    for (int i = 0; i < runs.size(); ++i) {
        BWaitResult result;
        const BWaitRun &run = runs[i];
        if (!run.scheduled.isValid() || !favour.started(i)) {
            result.start = run.scheduled;
            results.append(result);
            continue;
        }

        const qint64 scheduled = run.scheduled.toSecsSinceEpoch();
        result.waitSecs = favour.start(i) - scheduled;
        // In the time zone the caller gave the scheduled time in
        result.start = run.scheduled.addSecs(result.waitSecs);
        result.holds = favour.holds(i);

        result.certain = against.started(i) && against.start(i) == favour.start(i);
        if (!result.certain) {
            result.possibleHolds = against.holds(i);
            result.unknowns = unknownsOf(i, runs, favour, against);
        }
        results.append(result);
    }

    // The holds carry times of the replay; give them the caller's time zone
    for (int i = 0; i < results.size(); ++i) {
        const QDateTime &scheduled = runs[i].scheduled;
        if (!scheduled.isValid())
            continue;
        for (QList<BWaitHold> *holds : { &results[i].holds, &results[i].possibleHolds }) {
            for (BWaitHold &hold : *holds) {
                if (hold.from.isValid())
                    hold.from = scheduled.addSecs(scheduled.secsTo(hold.from));
                if (hold.until.isValid())
                    hold.until = scheduled.addSecs(scheduled.secsTo(hold.until));
            }
        }
    }
    return results;
}

BWaitSummary BScheduleWaitModel::summary(const QList<BWaitResult> &results,
                                         const QList<int> &indices)
{
    BWaitSummary sum;
    for (int index : indices) {
        if (index < 0 || index >= results.size())
            continue;
        const BWaitResult &result = results[index];
        ++sum.runs;
        if (result.waits()) {
            ++sum.waiting;
            sum.totalSecs += result.waitSecs;
            sum.longestSecs = qMax(sum.longestSecs, result.waitSecs);
        }
        if (!result.certain)
            ++sum.uncertain;
    }
    return sum;
}

// ============================================================================
// Texts
// ============================================================================

QString BScheduleWaitModel::formatWait(qint64 secs)
{
    const qint64 minutes = (qMax<qint64>(0, secs) + 59) / 60;
    return QStringLiteral("%1:%2").arg(minutes / 60).arg(minutes % 60, 2, 10, QLatin1Char('0'));
}

QString BScheduleWaitModel::summaryText(const BWaitSummary &summary)
{
    QString text = summary.waiting > 0
        ? tr("%1 run(s) wait, longest %2").arg(summary.waiting).arg(formatWait(summary.longestSecs))
        : tr("No waiting predicted");
    if (summary.uncertain > 0)
        text += QStringLiteral(", ") + tr("%1 uncertain").arg(summary.uncertain);
    return text;
}

QString BScheduleWaitModel::holdText(const BWaitHold &hold, const BWaitRun &run)
{
    switch (hold.reason) {
    case BWaitHold::Reason::DirectorLimit:
        return hold.limitKnown
            ? tr("Director limit: the Director runs %1 job(s) at a time "
                 "(Maximum Concurrent Jobs = %1) and all of them are taken.").arg(hold.limit)
            : tr("Director limit: Maximum Concurrent Jobs of the Director is not known. "
                 "With the Bareos default of 1 every job slot would be taken.");
    case BWaitHold::Reason::StorageLimit:
        return hold.limitKnown
            ? tr("Storage limit: storage %1 takes %2 job(s) at a time "
                 "(Maximum Concurrent Jobs = %2) and all places are taken.")
                  .arg(hold.resource).arg(hold.limit)
            : hold.resource.isEmpty()
                ? tr("Storage limit: the storage of this job is not known, and so is its "
                     "Maximum Concurrent Jobs. With one shared storage and the Bareos default "
                     "of 1 it would be full.")
                : tr("Storage limit: Maximum Concurrent Jobs of storage %1 is not known. "
                     "With the Bareos default of 1 it would be full.").arg(hold.resource);
    case BWaitHold::Reason::ClientLimit:
        return hold.limitKnown
            ? tr("Client limit: client %1 takes %2 job(s) at a time "
                 "(Maximum Concurrent Jobs = %2) and all places are taken.")
                  .arg(hold.resource).arg(hold.limit)
            : tr("Client limit: Maximum Concurrent Jobs of client %1 is not known. "
                 "With the Bareos default of 1 it would be full.").arg(hold.resource);
    case BWaitHold::Reason::JobLimit:
        return hold.limitKnown
            ? tr("Job limit: job %1 may run %2 time(s) at once "
                 "(Maximum Concurrent Jobs = %2) and an earlier start of it has not finished.")
                  .arg(hold.resource).arg(hold.limit)
            : tr("Job limit: Maximum Concurrent Jobs of job %1 is not known. With the Bareos "
                 "default of 1 an earlier start of it would be in the way.").arg(hold.resource);
    case BWaitHold::Reason::Priority:
        if (run.priority > hold.otherPriority) {
            return tr("Priority: this run has priority %1 and runs of priority %2 are running. "
                      "The Director starts a run of a lower priority (a higher number) only "
                      "when no run of another priority is running.")
                .arg(run.priority).arg(hold.otherPriority);
        }
        return tr("Priority: this run has priority %1 and runs of priority %2 are running. "
                  "It starts beside them only if it and all of them have Allow Mixed Priority.")
            .arg(run.priority).arg(hold.otherPriority);
    case BWaitHold::Reason::QueueOrder:
        return tr("Queue order: a run of priority %1 is waiting ahead in the queue and cannot "
                  "start yet. The Director starts no run that is behind it in the queue.")
            .arg(hold.otherPriority);
    }
    return QString();
}

QString BScheduleWaitModel::blockersText(const BWaitHold &hold, const QList<BWaitRun> &runs,
                                         const QList<BWaitResult> &results, int most)
{
    QStringList names;
    int shown = 0;
    for (int index : hold.blockers) {
        if (index < 0 || index >= runs.size() || index >= results.size())
            continue;
        if (shown == most)
            break;
        ++shown;

        const BWaitRun &other = runs[index];
        const BWaitResult &result = results[index];
        const QDate day = hold.from.date();
        QString when;
        if (other.durationBasis == BWaitRun::Duration::Unknown) {
            when = tr("from %1, duration unknown").arg(clock(result.start, day));
        } else {
            const QDateTime end = result.start.addSecs(qMax<qint64>(1, other.durationSecs));
            when = other.durationBasis == BWaitRun::Duration::Estimated
                ? tr("%1 to about %2, estimated").arg(clock(result.start, day), clock(end, day))
                : tr("%1 to %2").arg(clock(result.start, day), clock(end, day));
        }
        names << (other.level.isEmpty()
            ? QStringLiteral("%1 (%2)").arg(other.job, when)
            : QStringLiteral("%1 (%2, %3)").arg(other.job, other.level, when));
    }
    QString text = names.join(QStringLiteral(", "));
    const int more = hold.blockers.size() - shown;
    if (more > 0)
        text += QStringLiteral(" ") + tr("and %1 more").arg(more);
    return text;
}

QString BScheduleWaitModel::unknownText(const BWaitUnknown &unknown)
{
    switch (unknown.kind) {
    case BWaitUnknown::Kind::DirectorLimit:
        return tr("Maximum Concurrent Jobs of the Director is not known");
    case BWaitUnknown::Kind::StorageLimit:
        return unknown.name.isEmpty()
            ? tr("the storage of the job is not known (the Job and its JobDefs name none)")
            : tr("Maximum Concurrent Jobs of storage %1 is not known").arg(unknown.name);
    case BWaitUnknown::Kind::ClientLimit:
        return tr("Maximum Concurrent Jobs of client %1 is not known").arg(unknown.name);
    case BWaitUnknown::Kind::JobLimit:
        return tr("Maximum Concurrent Jobs of job %1 is not known").arg(unknown.name);
    case BWaitUnknown::Kind::MixedPriority:
        return tr("Allow Mixed Priority of job %1 is not known").arg(unknown.name);
    case BWaitUnknown::Kind::EstimatedDuration:
        return tr("the duration of %1 was not measured at this level").arg(unknown.name);
    case BWaitUnknown::Kind::NoHistory:
        return tr("the duration of %1 was never measured").arg(unknown.name);
    }
    return QString();
}

namespace {

QString unknownsText(const QList<BWaitUnknown> &unknowns)
{
    QStringList parts;
    for (const BWaitUnknown &unknown : unknowns)
        parts << BScheduleWaitModel::unknownText(unknown);
    return parts.join(QStringLiteral("; "));
}

} // namespace

QString BScheduleWaitModel::reasonLine(const BWaitRun &run, const BWaitResult &result)
{
    QString line;
    if (const BWaitHold *hold = result.mainHold()) {
        switch (hold->reason) {
        case BWaitHold::Reason::DirectorLimit:
            line = tr("Director limit (Maximum Concurrent Jobs = %1)").arg(hold->limit);
            break;
        case BWaitHold::Reason::StorageLimit:
            line = tr("Storage limit (%1: Maximum Concurrent Jobs = %2)")
                       .arg(hold->resource).arg(hold->limit);
            break;
        case BWaitHold::Reason::ClientLimit:
            line = tr("Client limit (%1: Maximum Concurrent Jobs = %2)")
                       .arg(hold->resource).arg(hold->limit);
            break;
        case BWaitHold::Reason::JobLimit:
            line = tr("Job limit (%1: Maximum Concurrent Jobs = %2)")
                       .arg(hold->resource).arg(hold->limit);
            break;
        case BWaitHold::Reason::Priority:
            line = tr("Priority %1, runs of priority %2 are running")
                       .arg(run.priority).arg(hold->otherPriority);
            break;
        case BWaitHold::Reason::QueueOrder:
            line = tr("Queue order: a run of priority %1 waits ahead").arg(hold->otherPriority);
            break;
        }
    }

    if (!result.certain) {
        const QString why = result.unknowns.isEmpty()
            ? tr("depends on values that are not known")
            : unknownsText(result.unknowns);
        if (line.isEmpty())
            line = tr("May wait: %1").arg(why);
        else
            line += QStringLiteral("; ") + tr("may be longer: %1").arg(why);
    }
    return line;
}

QStringList BScheduleWaitModel::describe(int index, const QList<BWaitRun> &runs,
                                         const QList<BWaitResult> &results)
{
    QStringList lines;
    if (index < 0 || index >= runs.size() || index >= results.size())
        return lines;

    const BWaitRun &run = runs[index];
    const BWaitResult &result = results[index];
    const QDate day = run.scheduled.date();

    auto holdLines = [&](const QList<BWaitHold> &holds) {
        for (const BWaitHold &hold : holds) {
            QString text = holdText(hold, run);
            if (holds.size() > 1 && hold.until.isValid()) {
                text = tr("%1 to %2: %3").arg(clock(hold.from, day), clock(hold.until, day), text);
            }
            lines << text;
            const QString blockers = blockersText(hold, runs, results);
            if (!blockers.isEmpty())
                lines << tr("Waits for: %1").arg(blockers);
        }
    };

    if (result.waits()) {
        lines << tr("Predicted start: %1, %2 after the scheduled time (%3)")
                     .arg(clock(result.start, day), formatWait(result.waitSecs),
                          clock(run.scheduled, day));
        holdLines(result.holds);
    }

    if (!result.certain) {
        const QString why = result.unknowns.isEmpty()
            ? tr("it depends on values that are not known")
            : unknownsText(result.unknowns);
        if (result.waits()) {
            lines << tr("Uncertain, the wait may be longer: %1.").arg(why);
        } else {
            lines << tr("Uncertain, this run may have to wait: %1.").arg(why);
            // What would hold it then. The runs it would wait for are not
            // named: which they are depends on the same unknowns.
            if (!result.possibleHolds.isEmpty())
                lines << tr("If so: %1").arg(holdText(result.possibleHolds.first(), run));
        }
    }
    return lines;
}
