/**
 * @file bliveoverlay.cpp
 * @brief The jobs that run or wait right now, from push events
 */

#include "schedules/model/bliveoverlay.h"
#include "schedules/model/bjobhistorystats.h"

#include <QRegularExpression>

namespace {

// Ended job ids remembered against stale seeds; far more than ever end
// between a history request and its reply
constexpr int MaxEndedRemembered = 4096;

// A catalog time (wall-clock, kept with the UTC spec) as local time
QDateTime localOf(const QDateTime &wallClock)
{
    if (!wallClock.isValid()) return QDateTime();
    return QDateTime(wallClock.date(), wallClock.time());
}

} // namespace

qint64 BLiveJobOverlay::Job::waitSecs(const QDateTime &now) const
{
    if (!queued.isValid()) return -1;
    const QDateTime end = started.isValid() ? started : now;
    return qMax<qint64>(0, queued.secsTo(end));
}

BLiveJobOverlay::StatusKind BLiveJobOverlay::classify(const QString &status)
{
    const QString s = status.trimmed();
    if (s.isEmpty()) return StatusKind::Other;

    // One-letter catalog codes (Bareos job_status.h, JobStatus in
    // proto/director_api.proto). Case matters here: 'f' is not 'F'.
    if (s.size() == 1) {
        switch (s.at(0).toLatin1()) {
        case 'R': case 'L': case 'a': case 'i': case 'l':
            return StatusKind::Running;
        case 'C': case 'B': case 'F': case 'S': case 'M': case 'm': case 's':
        case 'j': case 'c': case 'd': case 't': case 'p': case 'q':
            return StatusKind::Waiting;
        case 'T': case 'W': case 'E': case 'f': case 'A': case 'I': case 'D':
            return StatusKind::Ended;
        default:
            return StatusKind::Other;
        }
    }

    const QString l = s.toLower();
    if (l == QLatin1String("queued") || l == QLatin1String("created")
        || l == QLatin1String("blocked") || l.startsWith(QLatin1String("wait"))) {
        return StatusKind::Waiting;
    }
    if (l == QLatin1String("running") || l == QLatin1String("started")) {
        return StatusKind::Running;
    }
    // odir: "Success", "Error"; the proto comment: "OK", "Error",
    // "Canceled"; the status texts of "status dir" for terminated jobs
    static const char *const ended[] = {
        "ok", "success", "completed", "terminated", "error", "failed", "fatal",
        "fatal error", "canceled", "cancelled", "warnings", "incomplete", "differences",
    };
    for (const char *e : ended) {
        if (l == QLatin1String(e)) return StatusKind::Ended;
    }
    if (l.startsWith(QLatin1String("ok ")) || l.startsWith(QLatin1String("terminated"))
        || l.startsWith(QLatin1String("error "))) {
        return StatusKind::Ended;
    }
    // "Canceling": the job still holds its slots until its end event
    return StatusKind::Other;
}

QString BLiveJobOverlay::jobOfName(const QString &name)
{
    static const QRegularExpression unique(
        QStringLiteral("\\.\\d{4}-\\d{2}-\\d{2}_\\d{2}\\.\\d{2}\\.\\d{2}_\\d+$"));
    QString job = name.trimmed();
    job.remove(unique);
    return job;
}

bool BLiveJobOverlay::isStartMessage(const QString &text)
{
    // job_messages.rs, migrate.rs, verify.rs, vbackup.rs, internal_jobs.rs
    // as Bareos: "Start Backup JobId 5, Job=...", "Start Restore Job ...",
    // "Start Verify JobId=...", "Start Virtual Backup JobId ...". Bareos
    // puts "<director> JobId <n>: " in front.
    static const QRegularExpression start(
        QStringLiteral("(?:^|: )Start [A-Za-z][A-Za-z ]* Job"),
        QRegularExpression::MultilineOption);
    return start.match(text).hasMatch();
}

const BLiveJobOverlay::Job *BLiveJobOverlay::job(quint64 jobId) const
{
    const auto it = m_jobs.constFind(jobId);
    return it == m_jobs.constEnd() ? nullptr : &it.value();
}

void BLiveJobOverlay::markEnded(quint64 jobId)
{
    m_jobs.remove(jobId);
    if (m_ended.contains(jobId)) return;
    m_ended.insert(jobId);
    m_endedOrder.enqueue(jobId);
    while (m_endedOrder.size() > MaxEndedRemembered) {
        m_ended.remove(m_endedOrder.dequeue());
    }
}

bool BLiveJobOverlay::statusEvent(quint64 jobId, const QString &jobName, const QString &status,
                                  const QDateTime &when, const QDateTime &scheduled)
{
    if (jobId == 0) return false;
    const StatusKind kind = classify(status);

    if (kind == StatusKind::Ended) {
        const bool known = m_jobs.contains(jobId);
        markEnded(jobId);
        return known;
    }

    auto it = m_jobs.find(jobId);
    if (it == m_jobs.end()) {
        if (kind == StatusKind::Other || m_ended.contains(jobId)) return false;
        Job job;
        job.jobId = jobId;
        job.job = jobOfName(jobName);
        job.status = status.trimmed();
        if (kind == StatusKind::Waiting) {
            job.phase = Phase::Waiting;
            job.queued = scheduled.isValid() && scheduled <= when ? scheduled : when;
        } else {
            // Running without a queue event first: the wait is known only
            // from the scheduled start
            job.phase = Phase::Running;
            job.started = when;
            if (scheduled.isValid() && scheduled <= when) job.queued = scheduled;
        }
        m_jobs.insert(jobId, job);
        return true;
    }

    Job &job = it.value();
    bool changed = job.status != status.trimmed();
    job.status = status.trimmed();
    if (job.job.isEmpty() && !jobName.isEmpty()) {
        job.job = jobOfName(jobName);
        changed = true;
    }
    // The scheduled start, once the Director says it, is where the wait began
    if (scheduled.isValid() && scheduled <= when && job.queued != scheduled
        && (!job.started.isValid() || scheduled <= job.started)) {
        job.queued = scheduled;
        changed = true;
    }
    if (kind == StatusKind::Running && job.phase == Phase::Waiting) {
        job.phase = Phase::Running;
        job.started = when;
        changed = true;
    }
    // A running job that waits for a mount or a volume has started: it
    // stays a running job, only its status text changes
    return changed;
}

bool BLiveJobOverlay::jobMessage(quint64 jobId, const QString &jobName, const QString &text,
                                 const QDateTime &when)
{
    if (jobId == 0 || !isStartMessage(text)) return false;

    auto it = m_jobs.find(jobId);
    if (it == m_jobs.end()) {
        if (m_ended.contains(jobId)) return false;
        Job job;
        job.jobId = jobId;
        job.job = jobOfName(jobName);
        job.phase = Phase::Running;
        job.started = when;
        m_jobs.insert(jobId, job);
        return true;
    }
    Job &job = it.value();
    if (job.phase == Phase::Running) return false;
    job.phase = Phase::Running;
    job.started = when;
    return true;
}

bool BLiveJobOverlay::seed(const QList<BJobRun> &runs)
{
    bool changed = false;
    for (const BJobRun &run : runs) {
        if (run.jobId == 0 || m_jobs.contains(run.jobId) || m_ended.contains(run.jobId)) continue;
        StatusKind kind = classify(QString(run.status));
        if (kind != StatusKind::Waiting && kind != StatusKind::Running) continue;

        const QDateTime scheduled = localOf(run.scheduled);
        const QDateTime started = localOf(run.started);
        // Waiting for the client, the storage, a mount or a volume happens
        // after the job left the queue (Bareos job.cc): with a start time
        // such a job is running
        const QChar code = run.status;
        if (kind == StatusKind::Waiting && started.isValid()
            && (code == QLatin1Char('F') || code == QLatin1Char('S')
                || code == QLatin1Char('M') || code == QLatin1Char('m'))) {
            kind = StatusKind::Running;
        }

        Job job;
        job.jobId = run.jobId;
        job.job = run.name;
        job.status = QString(run.status);
        job.seeded = true;
        job.queued = scheduled;
        if (kind == StatusKind::Running) {
            job.phase = Phase::Running;
            job.started = started.isValid() ? started : scheduled;
        } else {
            job.phase = Phase::Waiting;
        }
        if (!job.since().isValid()) continue;   // nothing to draw it from
        // A start before the scheduled time belongs to another job (copied
        // jobs, see BJobHistoryStats): the wait is not known then
        if (job.started.isValid() && job.queued.isValid() && job.started < job.queued) {
            job.queued = QDateTime();
        }
        m_jobs.insert(job.jobId, job);
        changed = true;
    }
    return changed;
}

void BLiveJobOverlay::clear()
{
    m_jobs.clear();
    m_ended.clear();
    m_endedOrder.clear();
}
