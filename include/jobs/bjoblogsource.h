// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BJOBLOGSOURCE_H
#define BJOBLOGSOURCE_H

#include <QObject>
#include <QElapsedTimer>
#include <QList>
#include <QPointer>
#include <QString>
#include <QTimer>
#include "jobs/bjoblogmodel.h"

/**
 * @file bjoblogsource.h
 * @brief Sources that feed a BJobLogModel
 *
 * A source knows nothing about the Director classes: it asks for a catalog
 * load with fetchRequested() and gets the answers through its slots. The
 * owner (BJobLogView) wires both ends to BDirector. That keeps the sources
 * testable without a connection.
 *
 * @code
 *   start(jobId, running) ──► model.setJob() ──► fetchRequested(jobId)
 *        catalogReply(jobId, json) ──► model (merge) ──► updated(jobId)
 *        ...                                              finished(jobId)
 * @endcode
 *
 * Replies and events for another job than the one started are ignored.
 */
class BJobLogSource : public QObject
{
    Q_OBJECT

public:
    explicit BJobLogSource(BJobLogModel *model, QObject *parent = nullptr);

    /**
     * @brief Show the log of a job
     * @param jobId Catalog JobId
     * @param running true if the job has not ended yet (live log)
     *
     * Resets the model unless it already shows this job (resume after stop()).
     */
    void start(quint64 jobId, bool running);

    /// Stop all activity; the model keeps its rows
    void stop();

    quint64 jobId() const { return m_jobId; }
    bool isActive() const { return m_active; }
    /// true while more lines are expected for the job
    bool isLive() const { return m_active && m_live; }

public slots:
    /// Reply to `list joblog jobid=<jobId>`
    void catalogReply(quint64 jobId, const QString &reply);
    /// Pushed job message (gRPC only)
    void jobMessage(quint64 jobId, const QString &text);
    /// Pushed job status (gRPC only), status as sent by the Director
    void jobStatusEvent(quint64 jobId, const QString &status);
    /// The owner learned that the job has ended (e.g. from the job list)
    void jobEnded(quint64 jobId);

signals:
    /// Ask the owner to send `list joblog jobid=<jobId>`
    void fetchRequested(quint64 jobId);
    /// A catalog load was merged or pushed lines were appended
    void updated(quint64 jobId);
    /// The Director answered a catalog load with an error
    void failed(quint64 jobId, const QString &error);
    /// No more lines are expected for this job
    void finished(quint64 jobId);

protected:
    virtual void onStarted() = 0;
    virtual void onStopped() {}
    /// A catalog reply for the current job was handled (merged, or failed())
    virtual void onCatalogReply() = 0;
    virtual void onJobMessage(const QString &text) { Q_UNUSED(text) }
    virtual void onJobEnded() = 0;
    /// Pushed messages from this sequence number on survive a catalog load
    virtual int keepPushFrom() { return -1; }

    void requestFetch();
    void setLive(bool live);
    /// Mark the job as ended and emit finished() (once per live phase)
    void finish();
    BJobLogModel *model() const { return m_model; }
    int outstandingFetches() const { return m_outstanding; }

private:
    QPointer<BJobLogModel> m_model;
    quint64 m_jobId = 0;
    bool m_active = false;
    bool m_live = false;
    bool m_finished = false;
    int m_outstanding = 0;
};


/**
 * @brief Catalog polling (BNET, and any Director without job message push)
 *
 * A finished job is fetched once. A running job is fetched again every
 * pollInterval() until it ends, followed by one final fetch. The end is
 * known when the owner reports it (jobEnded()/jobStatusEvent()) or when the
 * termination summary shows up in the log.
 */
class BPollingJobLogSource : public BJobLogSource
{
    Q_OBJECT

public:
    explicit BPollingJobLogSource(BJobLogModel *model, QObject *parent = nullptr);

    int pollInterval() const { return m_timer.interval(); }
    void setPollInterval(int msec);

protected:
    void onStarted() override;
    void onStopped() override;
    void onCatalogReply() override;
    void onJobEnded() override;

private slots:
    void poll();

private:
    QTimer m_timer;
    bool m_finalFetch = false;      ///< The next reply is the last one
    bool m_endSeen = false;         ///< Termination summary seen; final fetch on the next tick
    int m_skippedTicks = 0;         ///< Ticks skipped while a reply was outstanding
};


/**
 * @brief Pushed job messages (gRPC, Onesimus Director)
 *
 * One catalog load at the start, then pushed lines are appended, then one
 * catalog load when the job has ended. Push is lossy and carries neither
 * time nor message type, so the final catalog load replaces the pushed rows.
 */
class BPushJobLogSource : public BJobLogSource
{
    Q_OBJECT

public:
    explicit BPushJobLogSource(BJobLogModel *model, QObject *parent = nullptr);

    /// Time span around a catalog load in which pushed and loaded lines may overlap
    static constexpr int OverlapWindowMs = 3000;

protected:
    void onStarted() override;
    void onStopped() override;
    void onCatalogReply() override;
    void onJobMessage(const QString &text) override;
    void onJobEnded() override;
    int keepPushFrom() override;

private:
    struct Pushed
    {
        int seq;
        qint64 at;
    };

    void pruneRecentPushes(qint64 now);

    QElapsedTimer m_clock;
    QList<Pushed> m_recentPushes;   ///< Pushed messages of the last OverlapWindowMs
    qint64 m_lastCatalogAt = -1;    ///< Time of the last merged catalog load
    bool m_finalFetch = false;
};

#endif // BJOBLOGSOURCE_H
