#include "bcleanuprunner.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

namespace {

/// True if an array named "jobids" anywhere below @p value lists @p jobId
bool listsJobId(const QJsonValue &value, const QString &jobId, bool inJobIds = false)
{
    if (value.isArray()) {
        const QJsonArray array = value.toArray();
        for (const QJsonValue &item : array) {
            if (inJobIds) {
                if (item.isString() && item.toString().trimmed() == jobId)
                    return true;
                if (item.isDouble() && QString::number(static_cast<qint64>(item.toDouble())) == jobId)
                    return true;
            }
            if (listsJobId(item, jobId, false))
                return true;
        }
        return false;
    }
    if (value.isObject()) {
        const QJsonObject object = value.toObject();
        for (auto it = object.begin(); it != object.end(); ++it) {
            if (listsJobId(it.value(), jobId, it.key() == QLatin1String("jobids")))
                return true;
        }
    }
    return false;
}

QString shortened(const QString &reply)
{
    const QString simple = reply.simplified();
    return simple.size() > 400 ? simple.left(400) + QStringLiteral("...") : simple;
}

} // namespace

BCleanupRunner::BCleanupRunner(QObject *parent)
    : QObject(parent)
{
}

void BCleanupRunner::setSender(Sender sender)
{
    m_sender = std::move(sender);
}

void BCleanupRunner::start(const BCleanup::Plan &plan)
{
    if (m_running)
        return;

    m_steps.clear();
    const QStringList jobIds = plan.jobIdsToDelete();
    for (const QString &jobId : jobIds) {
        Step step;
        step.kind = StepKind::DeleteJob;
        step.jobId = jobId;
        m_steps.append(step);
    }
    if (plan.pruneVolumes) {
        // Last, so that nothing of this run follows it
        Step step;
        step.kind = StepKind::PruneVolumes;
        m_steps.append(step);
    }

    m_current = -1;
    m_stopRequested = false;
    m_abortReason.clear();
    m_running = true;
    sendNext();
}

bool BCleanupRunner::isWaitingFor(StepKind kind) const
{
    return m_running && m_current >= 0 && m_current < m_steps.size()
        && m_steps[m_current].outcome == Outcome::Pending
        && m_steps[m_current].kind == kind;
}

void BCleanupRunner::sendNext()
{
    const int next = m_current + 1;
    if (next >= m_steps.size() || m_stopRequested || !m_sender) {
        finish();
        return;
    }

    m_current = next;
    m_steps[m_current].outcome = Outcome::Pending;
    emit stepStarted(m_current);
    m_sender(m_steps[m_current].kind, m_steps[m_current].jobId);
}

void BCleanupRunner::handleReply(StepKind kind, const QString &reply)
{
    if (!isWaitingFor(kind))
        return;

    Step &step = m_steps[m_current];
    step.reply = shortened(reply);
    step.outcome = kind == StepKind::DeleteJob ? evaluateDeleteReply(step.jobId, reply)
                                               : evaluatePruneReply(reply);

    const bool proceed = step.outcome == Outcome::Confirmed || step.outcome == Outcome::Answered;
    emit stepFinished(m_current);

    if (proceed)
        sendNext();
    else
        finish();
}

void BCleanupRunner::requestStop()
{
    if (m_running)
        m_stopRequested = true;
}

void BCleanupRunner::abort(const QString &reason)
{
    if (!m_running)
        return;

    m_abortReason = reason;
    if (m_current >= 0 && m_current < m_steps.size()
        && m_steps[m_current].outcome == Outcome::Pending) {
        m_steps[m_current].outcome = Outcome::NoReply;
        m_steps[m_current].reply = reason;
        emit stepFinished(m_current);
    }
    finish();
}

void BCleanupRunner::finish()
{
    if (!m_running)
        return;
    m_running = false;
    emit finished();
}

int BCleanupRunner::count(Outcome outcome) const
{
    int n = 0;
    for (const Step &step : m_steps) {
        if (step.outcome == outcome)
            ++n;
    }
    return n;
}

QStringList BCleanupRunner::confirmedJobIds() const
{
    QStringList ids;
    for (const Step &step : m_steps) {
        if (step.kind == StepKind::DeleteJob && step.outcome == Outcome::Confirmed)
            ids.append(step.jobId);
    }
    return ids;
}

bool BCleanupRunner::allSucceeded() const
{
    for (const Step &step : m_steps) {
        if (step.outcome != Outcome::Confirmed && step.outcome != Outcome::Answered)
            return false;
    }
    return true;
}

bool BCleanupRunner::replyHasError(const QString &reply)
{
    // Same text markers as BareosDirector::detectCommandError(), so that both
    // layers agree on what a failed command is
    if (reply.contains(QLatin1String("\"error\""), Qt::CaseInsensitive)
        || reply.contains(QLatin1String("ERR="), Qt::CaseSensitive)
        || reply.contains(QLatin1String("Error:"), Qt::CaseInsensitive)) {
        return true;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(reply.toUtf8());
    if (doc.isObject()) {
        const QJsonObject root = doc.object();
        if (root.contains(QLatin1String("error")))
            return true;
        if (root.value(QLatin1String("result")).toObject().contains(QLatin1String("error")))
            return true;
    }
    return false;
}

BCleanupRunner::Outcome BCleanupRunner::evaluateDeleteReply(const QString &jobId, const QString &reply)
{
    if (jobId.isEmpty() || reply.trimmed().isEmpty())
        return Outcome::Unconfirmed;
    if (replyHasError(reply))
        return Outcome::Failed;

    // API mode: {"result":{"deleted":{"jobids":["<id>"]}}}
    const QJsonDocument doc = QJsonDocument::fromJson(reply.toUtf8());
    if (doc.isObject()
        && listsJobId(doc.object().value(QLatin1String("result")), jobId)) {
        return Outcome::Confirmed;
    }

    // Text mode: "Jobid <id> and associated records deleted from the catalog."
    const QRegularExpression confirmation(
        QStringLiteral("\\bJobid\\s+%1\\s+and associated records deleted\\b")
            .arg(QRegularExpression::escape(jobId)),
        QRegularExpression::CaseInsensitiveOption);
    if (confirmation.match(reply).hasMatch())
        return Outcome::Confirmed;

    return Outcome::Unconfirmed;
}

BCleanupRunner::Outcome BCleanupRunner::evaluatePruneReply(const QString &reply)
{
    if (reply.trimmed().isEmpty())
        return Outcome::Unconfirmed;
    return replyHasError(reply) ? Outcome::Failed : Outcome::Answered;
}
