// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BJOBLOGVIEW_H
#define BJOBLOGVIEW_H

#include <QListView>
#include <QPointer>
#include "director/bdirector.h"
#include "jobs/bjoblogmodel.h"

class BJobLogSource;

/**
 * @file bjoblogview.h
 * @brief List view for a job log, shared by the job log dialog, the log pane
 *        of BJobWidget and the job details dialog
 *
 * The view owns the source that feeds its BJobLogModel:
 * @code
 *   showJob(id, running) ──► BPushJobLogSource     (gRPC connection)
 *                            BPollingJobLogSource  (BNET connection)
 *        source.fetchRequested ──► BDirector::doSend(ListJobId)
 *        BDirector::jobLogResult / jobMessage / jobStatusEvent ──► source ──► model
 * @endcode
 *
 * Behaviour:
 * - Scrolls per pixel, one row per log line.
 * - Follows the tail only if the view was at the bottom before rows were
 *   inserted; the first load of a job ends at the bottom.
 * - The source only runs while the view is visible; it is resumed when the
 *   view is shown again.
 * - "Loading" and "empty" are painted as a placeholder, not as model rows.
 */
class BJobLogView : public QListView
{
    Q_OBJECT

public:
    explicit BJobLogView(QWidget *parent = nullptr);

    /**
     * @brief Set the Director connection and the log model to feed
     *
     * @p logModel need not be the view's model(): the pane of BJobWidget
     * puts a filter proxy in between.
     */
    void attach(BDirector *director, BJobLogModel *logModel);

    /// Show the log of a job; @p running selects the live log
    void showJob(quint64 jobId, bool running);
    /// Stop feeding the model (no job selected, or a snapshot is shown)
    void stopJob();
    /// The owner learned that the shown job has ended (e.g. from the job list)
    void notifyJobEnded(quint64 jobId);

    quint64 jobId() const { return m_jobId; }
    /// true while more lines are expected for the shown job
    bool isLive() const;

    void setPlaceholderText(const QString &text);
    /// Selected lines in row order, joined with newlines
    QString selectedText() const;

    void setModel(QAbstractItemModel *model) override;
    void reset() override;

signals:
    /// The model content for @p jobId changed (catalog load or pushed lines)
    void logUpdated(quint64 jobId);
    /// No more lines are expected for @p jobId
    void logFinished(quint64 jobId);
    void logFailed(quint64 jobId, const QString &error);

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

protected slots:
    void rowsInserted(const QModelIndex &parent, int start, int end) override;

private:
    void sampleFollowTail();
    void startSource();
    void ensureSource();

    QPointer<BDirector> m_director;
    QPointer<BJobLogModel> m_logModel;
    BJobLogSource *m_source = nullptr;
    bool m_sourceIsPush = false;

    quint64 m_jobId = 0;        ///< Job to show, 0 = none
    bool m_running = false;     ///< The job is expected to produce more lines
    bool m_complete = false;    ///< The log of m_jobId was loaded completely

    QString m_placeholder;
    QMetaObject::Connection m_aboutToInsert;  ///< model()'s rowsAboutToBeInserted
    bool m_forceBottom = true;  ///< Next insert ends at the bottom (first load)
    bool m_followTail = false;  ///< Sampled before rows are inserted
};

#endif // BJOBLOGVIEW_H
