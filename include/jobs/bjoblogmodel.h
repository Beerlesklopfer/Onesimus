// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BJOBLOGMODEL_H
#define BJOBLOGMODEL_H

#include <QAbstractListModel>
#include <QFont>
#include <QList>
#include <QString>
#include <QStringList>

/**
 * @file bjoblogmodel.h
 * @brief Append-only job log model and the pure helpers around it
 *
 * No dependency on the Director classes, so the model and the helpers can
 * be unit tested without a connection (test/bjoblogmodel_test.cpp).
 */

namespace BJobLog {

/// One record of `list joblog`
struct Entry
{
    qint64 logId = 0;   ///< Catalog LogId; 0 if the reply does not carry one
    QString time;       ///< Time column, may be empty
    QString text;       ///< Log text, may contain embedded newlines
};

/// Result of parseCatalogReply()
struct ParsedReply
{
    bool isError = false;   ///< The Director answered with an error object
    QString error;          ///< Error text if isError
    QList<Entry> entries;   ///< All records of the reply, in reply order
};

/**
 * @brief Parse a `list joblog` reply
 *
 * Understands {"result":{"joblog":[..]}}, {"joblog":[..]}, {"log":[..]} and
 * a top-level array; records are objects (time, logtext, optional logid) or
 * plain strings. Anything that is not JSON is taken as plain text, one
 * record per line. Records are returned unfiltered (also empty ones), so
 * that their count can be used to merge repeated loads.
 */
ParsedReply parseCatalogReply(const QString &reply);

/**
 * @brief Split one record into display lines
 *
 * The "[time] " prefix goes on the first line, continuation lines are
 * indented. Returns an empty list for a record without text.
 */
QStringList entryLines(const Entry &entry);

/// Severity of one display line (drives the colouring)
enum class LineKind { Normal, Success, Warning, Error, Fatal };

/**
 * @brief Classify one display line
 * @param line Display line
 * @param continuation true for the second and later lines of a record
 *
 * Keywords match whole words only. Continuation lines of the form
 * "<label>: <value>" (the termination summary, e.g. "Non-fatal FD errors: 0")
 * are never flagged by keyword; only their "Termination:" line is rated.
 */
LineKind classifyLine(const QString &line, bool continuation);

/// Follow-tail decision: is a scroll bar at (or within tolerance of) its end?
bool isAtBottom(int value, int maximum, int tolerance = 4);

/// Catalog JobStatus code ("R", "T", ...) of a job that has not ended yet
bool isRunningJobStatus(const QString &catalogStatus);

/// Status text of a pushed JobStatusEvent that means "the job has ended"
bool isTerminalStatusEvent(const QString &status);

} // namespace BJobLog


/**
 * @brief Job log, one row per text line, append-only
 *
 * - setJob() is the only place the live model is reset.
 * - Catalog loads (`list joblog`) are merged: only records that are not held
 *   yet are appended. Records are identified by their LogId if the reply
 *   carries one, otherwise by their position ("records beyond the count
 *   already held").
 * - Pushed lines have no LogId. They always form the tail of the model; a
 *   catalog load is authoritative and replaces that tail in place.
 * - Calls for another job than the one set with setJob() are ignored.
 *
 * setLogLines() shows a stored snapshot (log history); it detaches the
 * model from its job.
 */
class BJobLogModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        LogIdRole = Qt::UserRole + 1,   ///< qint64, 0 for pushed rows and snapshots
        PushRole                        ///< bool, row came from a push event
    };

    explicit BJobLogModel(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /// Select the job whose log is shown; resets the model (also for the same job)
    void setJob(quint64 jobId);
    quint64 jobId() const { return m_jobId; }

    /**
     * @brief Merge the records of a catalog load
     * @param jobId Job the records belong to; ignored unless it is jobId()
     * @param entries All records of the reply
     * @param keepPushFrom Pushed messages with this sequence number or a
     *        later one survive the load unless one of the new records
     *        contains their text; -1 replaces all pushed rows
     * @return false if the call was ignored (other job)
     */
    bool appendCatalogEntries(quint64 jobId, const QList<BJobLog::Entry> &entries,
                              int keepPushFrom = -1);

    /**
     * @brief Append a pushed job message
     * @return Sequence number of the message, -1 if ignored (other job, no text)
     */
    int appendPushText(quint64 jobId, const QString &text);

    /// true if one of the newest catalog records contains @p text
    bool recentCatalogContains(const QString &text, int lastEntries = 50) const;

    int catalogEntryCount() const { return m_catalogEntryCount; }
    qint64 lastLogId() const { return m_lastLogId; }
    int pushRowCount() const { return m_rows.size() - m_catalogRowCount; }
    /// A catalog record with a "Termination:" line was seen (job summary)
    bool hasTermination() const { return m_hasTermination; }

    /// Show a stored snapshot (history); detaches the model from its job
    void setLogLines(const QStringList &logLines);
    void clear();
    QStringList logLines() const;

private:
    struct Row
    {
        QString text;
        qint64 logId = 0;
        bool push = false;
        BJobLog::LineKind kind = BJobLog::LineKind::Normal;
    };

    struct PushEntry
    {
        int seq = 0;
        QString text;
    };

    static void appendRows(QList<Row> &rows, const QStringList &lines, qint64 logId, bool push);
    void replaceTail(const QList<Row> &tail);

    quint64 m_jobId = 0;
    QList<Row> m_rows;                  ///< Catalog rows first, pushed rows at the end
    int m_catalogRowCount = 0;          ///< Number of leading rows that came from the catalog
    int m_catalogEntryCount = 0;        ///< Number of catalog records merged so far
    qint64 m_lastLogId = 0;             ///< Highest LogId merged so far (0: none delivered)
    QStringList m_recentCatalogTexts;   ///< Texts of the newest catalog records
    QList<PushEntry> m_pushEntries;     ///< Pushed messages currently shown
    int m_pushSeq = 0;                  ///< Sequence number of the next pushed message
    bool m_hasTermination = false;
};

#endif // BJOBLOGMODEL_H
