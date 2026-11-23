// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BCLEANUPRUNNER_H
#define BCLEANUPRUNNER_H

#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>
#include <functional>

#include "bcleanupplan.h"

/**
 * @brief Executes a cleanup plan one command at a time
 *
 * The runner sends nothing itself: it calls the sender for one step, then
 * waits until handleReply() delivers the Director's answer, evaluates it
 * and only then moves on. The run stops at the first answer that is an
 * error or does not confirm the step, so later commands are never sent
 * into an unknown state.
 *
 * It has no dependency on BDirector and can be driven by a test.
 */
class BCleanupRunner : public QObject
{
    Q_OBJECT

public:
    enum class StepKind {
        DeleteJob,
        PruneVolumes
    };

    enum class Outcome {
        NotSent,        ///< Never sent (run stopped earlier)
        Pending,        ///< Sent, no answer yet
        Confirmed,      ///< DeleteJob: the Director named the job ID as deleted
        Answered,       ///< PruneVolumes: answered without an error
        Failed,         ///< The Director answered with an error
        Unconfirmed,    ///< No error, but no confirmation either
        NoReply         ///< Aborted while waiting for the answer
    };

    struct Step {
        StepKind kind = StepKind::DeleteJob;
        QString jobId;                  ///< DeleteJob only
        Outcome outcome = Outcome::NotSent;
        QString reply;                  ///< What the Director answered, shortened
    };

    using Sender = std::function<void(StepKind kind, const QString &jobId)>;

    explicit BCleanupRunner(QObject *parent = nullptr);

    void setSender(Sender sender);

    /// Start with the steps of @p plan: job deletions first, volume pruning last
    void start(const BCleanup::Plan &plan);

    bool isRunning() const { return m_running; }

    /// True while the answer to a step of this kind is awaited
    bool isWaitingFor(StepKind kind) const;

    /// The Director's answer to the step that was sent last
    void handleReply(StepKind kind, const QString &reply);

    /// Finish after the answer that is still awaited; send nothing more
    void requestStop();

    /// Stop now (timeout, connection lost); the awaited step becomes NoReply
    void abort(const QString &reason);

    const QList<Step> &steps() const { return m_steps; }
    int count(Outcome outcome) const;
    QStringList confirmedJobIds() const;
    bool allSucceeded() const;
    QString abortReason() const { return m_abortReason; }

    /// Evaluate the answer to `delete job jobid=<jobId> yes`
    static Outcome evaluateDeleteReply(const QString &jobId, const QString &reply);
    /// Evaluate the answer to the prune command
    static Outcome evaluatePruneReply(const QString &reply);
    /// True if the answer carries one of the Director's error markers
    static bool replyHasError(const QString &reply);

signals:
    void stepStarted(int index);
    void stepFinished(int index);
    void finished();

private:
    void sendNext();
    void finish();

    Sender m_sender;
    QList<Step> m_steps;
    int m_current = -1;
    bool m_running = false;
    bool m_stopRequested = false;
    QString m_abortReason;
};

#endif // BCLEANUPRUNNER_H
