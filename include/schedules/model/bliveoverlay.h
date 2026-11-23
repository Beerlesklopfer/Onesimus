// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BLIVEOVERLAY_H
#define BLIVEOVERLAY_H

#include <QDateTime>
#include <QList>
#include <QMap>
#include <QQueue>
#include <QSet>
#include <QString>

struct BJobRun;

/**
 * @file bliveoverlay.h
 * @brief The jobs that run or wait right now, from the Director's push
 *        events, without widgets
 *
 * The timeline shows what is planned. Over a gRPC connection the Onesimus
 * Director (odir) also says what happens: every client receives push events
 * on its InteractiveSession stream (proto/director_service.proto). This
 * model keeps the jobs that are active now, so that the day view can draw
 * them over today's bars: a job that waits from the moment it was queued
 * until now, a job that runs from its start until now. A job leaves the
 * model when it ends.
 *
 * What the events carry (onesimus-dird, read on 2026-11-20 at the proto
 * commit 72abd06; crates/odir/src/grpc/push_events.rs, job_manager.rs,
 * commands/job.rs, commands/cancel.rs, job_messages.rs):
 * - JobStatusEvent {job_id, job_name, status, timestamp}. job_name is the
 *   name of the Job resource. odir sends "Queued" when a job enters the
 *   queue, "Running" when it really starts (since proto 3302131; older
 *   odir sent no start event), "Canceling" when it is cancelled and
 *   "Success" or "Error" when it is complete. Both vocabularies of the
 *   proto are understood, and the one-letter catalog codes as well
 *   (classify()). Since proto 3302131 the event also carries the job's
 *   scheduled start: then the wait is counted from there, as the catalog
 *   counts it (StartTime minus SchedTime), and not from the queue event.
 * - Job messages arrive as console text "JobId=<id> <name>: <text>"
 *   without a time. The first message a running job writes is
 *   "Start Backup JobId ...", "Start Restore Job ...", "Start Verify ..."
 *   and so on: that is taken as the start (isStartMessage()), at the time
 *   the message arrives.
 * - Neither says why a job waits.
 *
 * Jobs that were already active when the connection was made produce no
 * event until they end. seed() takes them from the job history of the
 * schedule view ("llist jobs", which lists running and waiting jobs with
 * their scheduled and start times). A job that ended in the meantime is
 * remembered and never seeded again, because the history may have been
 * produced before its end event arrived.
 *
 * Times are local QDateTime values of this computer.
 */
class BLiveJobOverlay
{
public:
    enum class Phase {
        Waiting,    ///< Queued, not started yet
        Running,    ///< Started (it may wait for a mount meanwhile: it holds its slots)
    };

    /** What a status says about the job's life. */
    enum class StatusKind {
        Waiting,    ///< Queued or waiting for a resource, not started
        Running,    ///< Running
        Ended,      ///< Complete, successful or not
        Other,      ///< Says nothing about the phase ("Canceling", unknown text)
    };

    struct Job
    {
        quint64 jobId = 0;
        QString job;            ///< Name of the Job resource
        Phase phase = Phase::Waiting;
        QDateTime queued;       ///< Waiting since: the scheduled start if known, else the queue event; invalid if not known
        QDateTime started;      ///< Running since; invalid while it waits
        QString status;         ///< Last status text of the Director
        bool seeded = false;    ///< Taken from the job history, not from an event

        /// Where the overlay of this job begins: the queue time, or the
        /// start when the queue time is not known
        QDateTime since() const { return queued.isValid() ? queued : started; }
        /// Seconds it waited (until now while it waits); -1 if unknown
        qint64 waitSecs(const QDateTime &now) const;
    };

    /**
     * @brief A JobStatusEvent
     * @param when      The event's timestamp (the Director's clock)
     * @param scheduled The job's scheduled start; invalid if not known
     * @return true if the jobs changed
     */
    bool statusEvent(quint64 jobId, const QString &jobName, const QString &status,
                     const QDateTime &when, const QDateTime &scheduled = QDateTime());

    /**
     * @brief A job message pushed by the Director
     * @param when When it arrived (the message carries no time)
     * @return true if the jobs changed: a start message of a waiting job,
     *         or of a job that is not known yet
     */
    bool jobMessage(quint64 jobId, const QString &jobName, const QString &text,
                    const QDateTime &when);

    /**
     * @brief Adds the running and waiting jobs of a job history
     *
     * Only jobs that are neither known nor ended since clear(). Times of
     * BJobRun are wall-clock times (BJobHistoryReply::parseTime()); they
     * are read as local times of this computer.
     * @return true if a job was added
     */
    bool seed(const QList<BJobRun> &runs);

    void clear();
    bool isEmpty() const { return m_jobs.isEmpty(); }
    /// The active jobs, by job id
    QList<Job> jobs() const { return m_jobs.values(); }
    /// The job with that id, or nullptr
    const Job *job(quint64 jobId) const;

    static StatusKind classify(const QString &status);
    /// The Job resource name of "Name" or of a unique job name
    /// "Name.2026-11-20_21.00.00_03"
    static QString jobOfName(const QString &name);
    /// A job's first message once it runs ("Start Backup JobId 5, ...")
    static bool isStartMessage(const QString &text);

private:
    void markEnded(quint64 jobId);

    QMap<quint64, Job> m_jobs;
    QSet<quint64> m_ended;          ///< Ended since clear(); never seeded again
    QQueue<quint64> m_endedOrder;   ///< To keep m_ended bounded
};

#endif // BLIVEOVERLAY_H
