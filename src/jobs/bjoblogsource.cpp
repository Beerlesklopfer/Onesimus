#include "jobs/bjoblogsource.h"

namespace {

const int DefaultPollIntervalMs = 3000;
/// A poll is skipped while a reply is outstanding, but not more often than this
const int MaxSkippedTicks = 10;
/// Delay of the final load after the termination summary was pushed
const int SummaryFinalFetchDelayMs = 1000;

} // namespace

// ============================================================================
// BJobLogSource
// ============================================================================

BJobLogSource::BJobLogSource(BJobLogModel *model, QObject *parent)
    : QObject(parent)
    , m_model(model)
{
}

void BJobLogSource::start(quint64 jobId, bool running)
{
    if (m_active) {
        stop();
    }
    if (!m_model || jobId == 0) {
        m_jobId = 0;
        return;
    }

    if (m_model->jobId() != jobId) {
        m_model->setJob(jobId);
    }

    m_jobId = jobId;
    m_active = true;
    m_live = running;
    m_finished = false;
    m_outstanding = 0;
    onStarted();
}

void BJobLogSource::stop()
{
    const bool wasActive = m_active;
    m_active = false;
    m_live = false;
    m_outstanding = 0;
    if (wasActive) {
        onStopped();
    }
}

void BJobLogSource::requestFetch()
{
    if (!m_active) {
        return;
    }
    ++m_outstanding;
    emit fetchRequested(m_jobId);
}

void BJobLogSource::setLive(bool live)
{
    m_live = live;
    if (live) {
        m_finished = false;
    }
}

void BJobLogSource::finish()
{
    m_live = false;
    if (!m_finished) {
        m_finished = true;
        emit finished(m_jobId);
    }
}

void BJobLogSource::catalogReply(quint64 jobId, const QString &reply)
{
    // Replies for another job (requested earlier, or by another view) are
    // not ours. A finished source still merges late replies for its job.
    if (!m_active || !m_model || jobId != m_jobId) {
        return;
    }
    if (m_outstanding > 0) {
        --m_outstanding;
    }

    const BJobLog::ParsedReply parsed = BJobLog::parseCatalogReply(reply);
    if (parsed.isError) {
        emit failed(jobId, parsed.error);
    } else {
        m_model->appendCatalogEntries(jobId, parsed.entries, keepPushFrom());
        emit updated(jobId);
    }
    onCatalogReply();
}

void BJobLogSource::jobMessage(quint64 jobId, const QString &text)
{
    if (!m_active || !m_model || jobId != m_jobId) {
        return;
    }
    onJobMessage(text);
}

void BJobLogSource::jobStatusEvent(quint64 jobId, const QString &status)
{
    if (BJobLog::isTerminalStatusEvent(status)) {
        jobEnded(jobId);
    }
}

void BJobLogSource::jobEnded(quint64 jobId)
{
    if (!m_active || !m_live || jobId != m_jobId) {
        return;
    }
    onJobEnded();
}


// ============================================================================
// BPollingJobLogSource
// ============================================================================

BPollingJobLogSource::BPollingJobLogSource(BJobLogModel *model, QObject *parent)
    : BJobLogSource(model, parent)
{
    m_timer.setInterval(DefaultPollIntervalMs);
    connect(&m_timer, &QTimer::timeout, this, &BPollingJobLogSource::poll);
}

void BPollingJobLogSource::setPollInterval(int msec)
{
    m_timer.setInterval(qMax(1, msec));
}

void BPollingJobLogSource::onStarted()
{
    m_finalFetch = !isLive();   // a finished job is fetched once
    m_endSeen = false;
    m_skippedTicks = 0;
    requestFetch();
    if (isLive()) {
        m_timer.start();
    }
}

void BPollingJobLogSource::onStopped()
{
    m_timer.stop();
    m_finalFetch = false;
    m_endSeen = false;
}

void BPollingJobLogSource::poll()
{
    if (!isLive()) {
        m_timer.stop();
        return;
    }
    if (m_endSeen) {
        // The summary was in the previous load: one more load, then done
        onJobEnded();
        return;
    }
    // Do not pile up requests behind a slow Director, but do not wait
    // forever for a reply that got lost either
    if (outstandingFetches() > 0 && ++m_skippedTicks < MaxSkippedTicks) {
        return;
    }
    m_skippedTicks = 0;
    requestFetch();
}

void BPollingJobLogSource::onCatalogReply()
{
    if (m_finalFetch) {
        m_finalFetch = false;
        m_timer.stop();
        finish();
        return;
    }
    if (isLive() && model()->hasTermination()) {
        m_endSeen = true;
    }
}

void BPollingJobLogSource::onJobEnded()
{
    m_endSeen = false;
    m_finalFetch = true;
    m_timer.stop();
    requestFetch();
}


// ============================================================================
// BPushJobLogSource
// ============================================================================

BPushJobLogSource::BPushJobLogSource(BJobLogModel *model, QObject *parent)
    : BJobLogSource(model, parent)
{
}

void BPushJobLogSource::onStarted()
{
    m_clock.start();
    m_recentPushes.clear();
    m_lastCatalogAt = -1;
    m_finalFetch = false;
    requestFetch();
}

void BPushJobLogSource::onStopped()
{
    m_recentPushes.clear();
    m_finalFetch = false;
}

void BPushJobLogSource::pruneRecentPushes(qint64 now)
{
    while (!m_recentPushes.isEmpty() && now - m_recentPushes.constFirst().at > OverlapWindowMs) {
        m_recentPushes.removeFirst();
    }
}

int BPushJobLogSource::keepPushFrom()
{
    // A message pushed just before the reply may be missing from the load
    // (the query ran first); keep those behind the catalog rows.
    pruneRecentPushes(m_clock.elapsed());
    return m_recentPushes.isEmpty() ? -1 : m_recentPushes.constFirst().seq;
}

void BPushJobLogSource::onJobMessage(const QString &text)
{
    const qint64 now = m_clock.elapsed();

    // The other side of the same race: the load already contained it
    if (m_lastCatalogAt >= 0 && now - m_lastCatalogAt <= OverlapWindowMs
        && model()->recentCatalogContains(text)) {
        return;
    }

    const int seq = model()->appendPushText(jobId(), text);
    if (seq < 0) {
        return;
    }
    setLive(true);   // a job that sends messages is running, whatever the job list said
    pruneRecentPushes(now);
    m_recentPushes.append({seq, now});
    emit updated(jobId());

    // Push is lossy, the terminal status event may never arrive: the
    // termination summary is as good a sign that the job has ended.
    static const QLatin1String termination("Termination:");
    const QStringList lines = text.split(QLatin1Char('\n'));
    for (const QString &line : lines) {
        if (line.trimmed().startsWith(termination, Qt::CaseInsensitive)) {
            const quint64 job = jobId();
            QTimer::singleShot(SummaryFinalFetchDelayMs, this, [this, job]() { jobEnded(job); });
            break;
        }
    }
}

void BPushJobLogSource::onCatalogReply()
{
    m_lastCatalogAt = m_clock.elapsed();
    if (m_finalFetch || !isLive()) {
        m_finalFetch = false;
        finish();
    }
}

void BPushJobLogSource::onJobEnded()
{
    m_finalFetch = true;
    requestFetch();
}
