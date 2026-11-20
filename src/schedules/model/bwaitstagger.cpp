#include "schedules/model/bwaitstagger.h"

#include <algorithm>
#include <optional>

namespace BWaitStagger {

int Proposal::moved() const
{
    int n = 0;
    for (const Shift &shift : shifts) {
        if (shift.minutes > 0) ++n;
    }
    return n;
}

qint64 avoidableSecs(const BWaitResult &result)
{
    qint64 secs = result.deviceWaitSecs;
    for (const BWaitHold &hold : result.holds) {
        switch (hold.reason) {
        case BWaitHold::Reason::DirectorLimit:
        case BWaitHold::Reason::StorageLimit:
        case BWaitHold::Reason::ClientLimit:
        case BWaitHold::Reason::JobLimit:
            secs += qMax<qint64>(0, hold.secs());
            break;
        case BWaitHold::Reason::Priority:
        case BWaitHold::Reason::QueueOrder:
            break;
        }
    }
    return secs;
}

namespace {

using Offsets = QHash<QString, int>;

struct Replay
{
    Offsets offsets;            ///< In effect, with the jobs taken along
    QHash<QString, QString> follows;
    QList<BWaitRun> runs;
    QList<BWaitResult> results;
    qint64 total = 0;
};

/// A run of a lower priority that must stay after a run of another job
struct Order
{
    QString higher;
    QString lower;
    qint64 gapSecs = 0;     ///< From the higher's run to the lower's, as scheduled now
};

class Search
{
public:
    Search(const QList<BWaitRun> &runs, const BWaitLimits &limits, const QDateTime &from,
           const Options &options)
        : m_runs(runs), m_limits(limits), m_from(from), m_options(options)
    {
        QHash<QString, int> latestCounted, latestAny;
        for (int i = 0; i < runs.size(); ++i) {
            const BWaitRun &run = runs[i];
            if (run.job.isEmpty()) continue;
            if (!m_jobs.contains(run.job)) {
                m_jobs.append(run.job);
                m_bound.insert(run.job, qMax(0, qMin(options.maxMinutes,
                                                     options.jobMaxMinutes.value(run.job, options.maxMinutes))));
            }
            if (!run.scheduled.isValid()) continue;
            const QTime t = run.scheduled.time();
            const int minute = t.hour() * 60 + t.minute();
            // The grid is laid at the first counted run of the job
            if (!m_anchor.contains(run.job) && counted(i))
                m_anchor.insert(run.job, minute);
            // The latest start on the clock, of the counted runs if there are any
            if (counted(i)) {
                latestCounted.insert(run.job, qMax(latestCounted.value(run.job, -1), minute));
            } else {
                latestAny.insert(run.job, qMax(latestAny.value(run.job, -1), minute));
            }
        }
        // The user's constraints narrow the bounds
        for (const QString &job : std::as_const(m_jobs)) {
            if (options.fixedJobs.contains(job)) {
                m_bound.insert(job, 0);
                m_fixed.insert(job);
                continue;
            }
            const auto latest = options.latestStartMinutes.constFind(job);
            if (latest == options.latestStartMinutes.cend()) continue;
            const int last = latestCounted.value(job, latestAny.value(job, -1));
            if (last < 0) continue;
            // Starts after it already: stays (it is never moved earlier)
            const int room = qMax(0, *latest - last);
            if (room <= m_bound.value(job)) {
                m_bound.insert(job, room);
                m_latestBounded.insert(job, *latest);
            }
        }
        // Who must stay after whom: the smallest gap per pair of jobs
        QHash<QPair<QString, QString>, qint64> gaps;
        for (const BWaitRun &run : runs) {
            if (!run.scheduled.isValid()) continue;
            for (const BWaitRun &other : runs) {
                if (other.job == run.job || other.priority <= run.priority || !other.scheduled.isValid())
                    continue;
                const qint64 gap = run.scheduled.secsTo(other.scheduled);
                if (gap < 0 || gap >= 24 * 3600) continue;
                const QPair<QString, QString> key(run.job, other.job);
                if (!gaps.contains(key) || gap < gaps.value(key)) gaps.insert(key, gap);
            }
        }
        for (auto it = gaps.cbegin(); it != gaps.cend(); ++it)
            m_orders.append(Order{it.key().first, it.key().second, it.value()});
    }

    const QStringList &jobs() const { return m_jobs; }
    int bound(const QString &job) const { return m_bound.value(job); }
    bool fixed(const QString &job) const { return m_fixed.contains(job); }
    int latestBounded(const QString &job) const { return m_latestBounded.value(job, -1); }

    /// The offsets a job may take, ascending, without 0
    QList<int> candidates(const QString &job) const
    {
        QList<int> out;
        const int bound = m_bound.value(job);
        const int grid = m_options.gridMinutes;
        if (grid > 0) {
            const int anchor = m_anchor.value(job, 0);
            int first = (grid - anchor % grid) % grid;
            if (first == 0) first = grid;
            for (int m = first; m <= bound; m += grid) out.append(m);
        } else {
            const int step = qMax(1, m_options.stepMinutes);
            for (int m = step; m <= bound; m += step) out.append(m);
        }
        return out;
    }

    /// The smallest offset of @p job that is at least @p minutes; -1: none
    int atLeast(const QString &job, int minutes) const
    {
        if (minutes <= 0) return 0;
        for (int m : candidates(job)) {
            if (m >= minutes) return m;
        }
        return -1;
    }

    /// The chosen offsets with the jobs of lower priority taken along;
    /// nullopt if one of them would have to move beyond its bound
    std::optional<Replay> settle(const Offsets &chosen) const
    {
        Replay r;
        r.offsets = chosen;
        for (int round = 0; round <= m_jobs.size(); ++round) {
            bool changed = false;
            for (const Order &order : m_orders) {
                const qint64 need = qint64(r.offsets.value(order.higher)) * 60 - order.gapSecs;
                if (need <= qint64(r.offsets.value(order.lower)) * 60) continue;
                const int minutes = atLeast(order.lower, int((need + 59) / 60));
                if (minutes < 0) return std::nullopt;
                r.offsets.insert(order.lower, minutes);
                r.follows.insert(order.lower, order.higher);
                changed = true;
            }
            if (!changed) return r;
        }
        return std::nullopt;    // no rest: not expected with orders by priority
    }

    std::optional<Replay> replay(const Offsets &chosen) const
    {
        std::optional<Replay> r = settle(chosen);
        if (!r) return r;
        r->runs = m_runs;
        for (BWaitRun &run : r->runs) {
            const int minutes = r->offsets.value(run.job);
            if (minutes > 0 && run.scheduled.isValid())
                run.scheduled = run.scheduled.addSecs(qint64(minutes) * 60);
        }
        r->results = BScheduleWaitModel::predict(r->runs, m_limits);
        for (int i = 0; i < r->results.size(); ++i) {
            if (counted(i)) r->total += avoidableSecs(r->results[i]);
        }
        return r;
    }

    /// Counted are the runs as scheduled now from @ref m_from on
    bool counted(int i) const
    {
        return i < m_runs.size() && m_runs[i].scheduled.isValid()
               && (!m_from.isValid() || m_runs[i].scheduled >= m_from);
    }

    qint64 jobWait(const QString &job, const Replay &r) const
    {
        qint64 secs = 0;
        for (int i = 0; i < r.runs.size() && i < r.results.size(); ++i) {
            if (r.runs[i].job == job && counted(i)) secs += avoidableSecs(r.results[i]);
        }
        return secs;
    }

private:
    const QList<BWaitRun> &m_runs;
    const BWaitLimits &m_limits;
    QDateTime m_from;
    Options m_options;
    QStringList m_jobs;
    QHash<QString, int> m_bound;
    QHash<QString, int> m_anchor;   ///< Minute of the day of the job's first counted run
    QSet<QString> m_fixed;          ///< Options::fixedJobs among the jobs
    QHash<QString, int> m_latestBounded;    ///< Jobs bounded by their latest start: that minute
    QList<Order> m_orders;
};

} // namespace

Proposal propose(const QList<BWaitRun> &runs, const BWaitLimits &limits, const QDateTime &from,
                 const Options &options)
{
    Proposal proposal;
    const Search search(runs, limits, from, options);

    Offsets chosen;
    const Replay before = *search.replay(chosen);     // nothing moves: always settles
    proposal.totalBeforeSecs = before.total;
    Replay current = before;

    for (int pass = 0; pass < qMax(1, options.passes) && current.total > 0; ++pass) {
        // Longest waiting first; a job that does not wait is not moved for itself
        QList<QPair<qint64, QString>> order;
        for (const QString &job : search.jobs()) {
            const qint64 secs = search.jobWait(job, current);
            if (secs > 0 && search.bound(job) > 0) order.append({secs, job});
        }
        std::stable_sort(order.begin(), order.end(),
                         [](const auto &a, const auto &b) { return a.first > b.first; });

        bool improved = false;
        for (const auto &entry : order) {
            const QString &job = entry.second;
            int bestMinutes = chosen.value(job);
            std::optional<Replay> best;
            for (int minutes : search.candidates(job)) {
                if (minutes == chosen.value(job)) continue;
                Offsets tried = chosen;
                tried.insert(job, minutes);
                std::optional<Replay> r = search.replay(tried);
                if (r && r->total < (best ? best->total : current.total)) {
                    best = std::move(r);
                    bestMinutes = minutes;
                }
            }
            if (best) {
                chosen.insert(job, bestMinutes);
                current = std::move(*best);
                improved = true;
            }
        }
        if (!improved) break;
    }

    // Moved no further than it has to: a smaller offset that waits no more
    for (const QString &job : search.jobs()) {
        const int moved = chosen.value(job);
        if (moved <= 0) continue;
        QList<int> smaller{0};
        for (int m : search.candidates(job)) {
            if (m < moved) smaller.append(m);
        }
        for (int minutes : smaller) {
            Offsets tried = chosen;
            if (minutes == 0) tried.remove(job);
            else tried.insert(job, minutes);
            std::optional<Replay> r = search.replay(tried);
            if (r && r->total <= current.total) {
                chosen = tried;
                current = std::move(*r);
                break;
            }
        }
    }

    proposal.totalAfterSecs = current.total;
    for (const QString &job : search.jobs()) {
        Shift shift;
        shift.job = job;
        shift.boundMinutes = search.bound(job);
        shift.fixed = search.fixed(job);
        shift.latestStartMinute = search.latestBounded(job);
        shift.minutes = current.offsets.value(job);
        if (shift.minutes > chosen.value(job)) shift.follows = current.follows.value(job);
        shift.waitBeforeSecs = search.jobWait(job, before);
        shift.waitAfterSecs = search.jobWait(job, current);
        proposal.shifts.append(shift);
    }
    proposal.runs = current.runs;
    proposal.results = current.results;
    return proposal;
}

} // namespace BWaitStagger
