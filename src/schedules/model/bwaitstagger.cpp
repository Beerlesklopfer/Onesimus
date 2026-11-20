#include "schedules/model/bwaitstagger.h"

#include <algorithm>

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

struct Replay
{
    QList<BWaitRun> runs;
    QList<BWaitResult> results;
    qint64 total = 0;
};

class Search
{
public:
    Search(const QList<BWaitRun> &runs, const BWaitLimits &limits, const QDateTime &from)
        : m_runs(runs), m_limits(limits), m_from(from)
    {
    }

    Replay replay(const QHash<QString, int> &offsets) const
    {
        Replay r;
        r.runs = m_runs;
        for (BWaitRun &run : r.runs) {
            const int minutes = offsets.value(run.job);
            if (minutes > 0 && run.scheduled.isValid())
                run.scheduled = run.scheduled.addSecs(qint64(minutes) * 60);
        }
        r.results = BScheduleWaitModel::predict(r.runs, m_limits);
        for (int i = 0; i < r.results.size(); ++i) {
            if (counted(i)) r.total += avoidableSecs(r.results[i]);
        }
        return r;
    }

    /// Counted are the runs as scheduled now from @ref m_from on
    bool counted(int i) const
    {
        return i < m_runs.size() && m_runs[i].scheduled.isValid()
               && (!m_from.isValid() || m_runs[i].scheduled >= m_from);
    }

private:
    const QList<BWaitRun> &m_runs;
    const BWaitLimits &m_limits;
    QDateTime m_from;
};

qint64 jobWait(const QString &job, const QList<BWaitRun> &runs, const QList<BWaitResult> &results,
               const Search &search)
{
    qint64 secs = 0;
    for (int i = 0; i < runs.size() && i < results.size(); ++i) {
        if (runs[i].job == job && search.counted(i)) secs += avoidableSecs(results[i]);
    }
    return secs;
}

} // namespace

Proposal propose(const QList<BWaitRun> &runs, const BWaitLimits &limits, const QDateTime &from,
                 const Options &options)
{
    Proposal proposal;
    const Search search(runs, limits, from);
    const int step = qMax(1, options.stepMinutes);

    // The jobs in the order of their first run, with their bounds
    QStringList jobs;
    QHash<QString, Shift> shifts;
    for (const BWaitRun &run : runs) {
        if (run.job.isEmpty() || shifts.contains(run.job)) continue;
        jobs.append(run.job);
        Shift shift;
        shift.job = run.job;
        shift.boundMinutes = qMax(0, qMin(options.maxMinutes,
                                          options.jobMaxMinutes.value(run.job, options.maxMinutes)));
        shifts.insert(run.job, shift);
    }
    // A run stays before (or at) every later run of a lower priority on its day
    for (const BWaitRun &run : runs) {
        if (!run.scheduled.isValid()) continue;
        Shift &shift = shifts[run.job];
        for (const BWaitRun &other : runs) {
            if (other.job == run.job || other.priority <= run.priority || !other.scheduled.isValid())
                continue;
            const qint64 gap = run.scheduled.secsTo(other.scheduled);
            if (gap < 0 || gap >= 24 * 3600) continue;
            const int minutes = int(gap / 60);
            if (minutes < shift.boundMinutes) {
                shift.boundMinutes = minutes;
                shift.boundJob = other.job;
                shift.boundAt = other.scheduled;
            }
        }
    }

    QHash<QString, int> offsets;
    const Replay before = search.replay(offsets);
    proposal.totalBeforeSecs = before.total;
    Replay current = before;

    for (int pass = 0; pass < qMax(1, options.passes) && current.total > 0; ++pass) {
        // Longest waiting first; a job that does not wait is not moved
        QList<QPair<qint64, QString>> order;
        for (const QString &job : jobs) {
            const qint64 secs = jobWait(job, current.runs, current.results, search);
            if (secs > 0 && shifts[job].boundMinutes > 0) order.append({secs, job});
        }
        std::stable_sort(order.begin(), order.end(),
                         [](const auto &a, const auto &b) { return a.first > b.first; });

        bool improved = false;
        for (const auto &entry : order) {
            const QString &job = entry.second;
            const int bound = shifts[job].boundMinutes;
            int bestMinutes = offsets.value(job);
            Replay best = current;
            for (int minutes = step; minutes <= bound; minutes += step) {
                if (minutes == offsets.value(job)) continue;
                QHash<QString, int> tried = offsets;
                tried.insert(job, minutes);
                Replay r = search.replay(tried);
                if (r.total < best.total) {
                    best = std::move(r);
                    bestMinutes = minutes;
                }
            }
            if (bestMinutes != offsets.value(job)) {
                offsets.insert(job, bestMinutes);
                current = std::move(best);
                improved = true;
            }
        }
        if (!improved) break;
    }

    // Moved no further than it has to: a smaller offset that waits no more
    for (const QString &job : jobs) {
        const int moved = offsets.value(job);
        for (int minutes = 0; minutes < moved; minutes += step) {
            QHash<QString, int> tried = offsets;
            if (minutes == 0) tried.remove(job);
            else tried.insert(job, minutes);
            Replay r = search.replay(tried);
            if (r.total <= current.total) {
                offsets = tried;
                current = std::move(r);
                break;
            }
        }
    }

    proposal.totalAfterSecs = current.total;
    for (const QString &job : jobs) {
        Shift shift = shifts.value(job);
        shift.minutes = offsets.value(job);
        shift.waitBeforeSecs = jobWait(job, before.runs, before.results, search);
        shift.waitAfterSecs = jobWait(job, current.runs, current.results, search);
        proposal.shifts.append(shift);
    }
    proposal.runs = current.runs;
    proposal.results = current.results;
    return proposal;
}

} // namespace BWaitStagger
