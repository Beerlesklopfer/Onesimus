#ifndef BJOBMODELS_H
#define BJOBMODELS_H

#include <QAbstractTableModel>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QSet>
#include <QDateTime>
#include <QFont>
#include "blogging.h"
#include "director/bdirector.h"

// Debug logging prefixes for Job Models
#define JOBS_DEBUG BLOG_DEBUG()
#define JOBS_WARNING BLOG_WARNING()
#define JOBS_CRITICAL BLOG_ERROR()

// ============================================================================
// BJobsModel - Main table model for displaying backup jobs
// ============================================================================

/**
 * @brief Table model for Bacula backup jobs with filtering and pagination support
 * @version 2.0
 * @since 2026-01-26
 *
 * Displays job information in a table format with columns for:
 * - Selection checkbox
 * - Job ID
 * - Name
 * - Client
 * - Start Time
 * - Duration
 * - Type
 * - Level
 * - Files
 * - Bytes
 * - Status
 *
 * Features:
 * - Checkbox selection with QSet tracking
 * - Live data updates via appendJobs()
 * - Pagination support
 * - Export functionality for selected jobs
 * - Comprehensive statistics calculation
 */
class BJobsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    /**
     * @brief Column indices for the table
     * @since 1.0
     */
    enum Columns {
        COL_SELECTED = 0,  ///< Checkbox selection column
        COL_JOBID,         ///< Job ID column
        COL_NAME,          ///< Job name column
        COL_CLIENT,        ///< Client name column
        COL_STARTTIME,     ///< Start time column
        COL_ENDTIME,       ///< End time column (calculated from start + duration)
        COL_DURATION,      ///< Duration column
        COL_TYPE,          ///< Job type column
        COL_LEVEL,         ///< Backup level column
        COL_FILES,         ///< File count column
        COL_BYTES,         ///< Byte size column
        COL_STATUS,        ///< Job status column
        COL_COUNT          ///< Total column count
    };

    /**
     * @brief Job statistics structure
     * @since 2.0
     */
    struct Statistics {
        int totalJobs;              ///< Total number of jobs
        int successfulJobs;         ///< Jobs with status 'T'
        int warningJobs;            ///< Jobs with status 'W'
        int failedJobs;             ///< Jobs with status 'f' or 'E'
        qint64 totalBytes;          ///< Total bytes backed up
        qint64 totalFiles;          ///< Total files backed up
        QDateTime earliestJob;      ///< Earliest job start time
        QDateTime latestJob;        ///< Latest job start time
        int selectedCount;          ///< Number of selected jobs
    };

    explicit BJobsModel(BDirector *director, QObject *parent = nullptr);

    // QAbstractTableModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setJobs(const QJsonArray &jobs);
    void appendJobs(const QJsonArray &jobs);
    void removeJobsByIds(const QStringList &jobIds);
    QJsonObject jobAt(int row) const;
    QJsonArray allJobs() const { return m_jobs; }
    QSet<QString> selectedJobIds() const { return m_selectedJobs; }
    QJsonArray selectedJobs() const;
    void clearSelection();
    void setJobSelected(int row, bool selected);
    void selectAll();
    void invertSelection();
    void toggleAllSelection();
    Statistics calculateStatistics() const;
    QJsonDocument exportToJson(bool selectedOnly = false) const;
    QString exportToCsv(bool selectedOnly = false) const;

    // Pagination support
    void setPaginationEnabled(bool enabled, int pageSize = 50);
    bool isPaginationEnabled() const { return m_paginationEnabled; }
    void setCurrentPage(int page);
    int currentPage() const { return m_currentPage; }
    int pageCount() const;
    int pageSize() const { return m_pageSize; }

signals:
    void selectionChanged();
    void jobsAppended(int count);
    void currentPageChanged(int page);

private:
    QString formatBytes(qint64 bytes) const;
    QString formatStatus(const QString &status) const;
    int rowToDataIndex(int row) const;

    BDirector *m_director;              ///< Director connection
    QJsonArray m_jobs;
    QSet<QString> m_selectedJobs;
    bool m_paginationEnabled;
    int m_currentPage;
    int m_pageSize;
};


// ============================================================================
// BJobsFilterModel - Filter proxy model for BJobsModel
// ============================================================================

/**
 * @brief Filter proxy model for BJobsModel with comprehensive filtering options
 * @version 1.0
 * @since 2026-01-26
 *
 * Provides filtering capabilities for:
 * - Job name (text search)
 * - Client name (text search)
 * - Job status (single or multiple)
 * - Job level (F/I/D)
 * - Date range
 * - File count range
 * - Byte size range
 */
class BJobsFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit BJobsFilterModel(QObject *parent = nullptr);

    void setNameFilter(const QString &name);
    void setClientFilter(const QString &client);
    void setStatusFilter(const QSet<QString> &statuses);
    void setLevelFilter(const QSet<QString> &levels);
    void setDateFilter(const QDateTime &from, const QDateTime &to);
    void setFileCountFilter(qint64 min, qint64 max);
    void setByteSizeFilter(qint64 min, qint64 max);
    void setZeroBytesFilter(bool enabled);
    void clearAllFilters();

    QString nameFilter() const { return m_nameFilter; }
    QString clientFilter() const { return m_clientFilter; }
    bool hasActiveFilters() const;

public slots:
    void setDateRange(const QDateTime &from, const QDateTime &to);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_nameFilter;
    QString m_clientFilter;
    QSet<QString> m_statusFilter;
    QSet<QString> m_levelFilter;
    QDateTime m_dateFrom;
    QDateTime m_dateTo;
    qint64 m_fileCountMin;
    qint64 m_fileCountMax;
    qint64 m_byteSizeMin;
    qint64 m_byteSizeMax;
    bool m_zeroBytesFilter;
};


// ============================================================================
// BJobLogModel - List model for displaying job log entries
// ============================================================================

/**
 * @brief Model for displaying job log entries
 * @version 1.0
 * @since 2026-01-28
 *
 * Parses JSON-RPC responses from Bareos Director and extracts log entries.
 * Supports multiple JSON response structures and falls back to plain text.
 */
class BJobLogModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BJobLogModel(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool parseJsonResponse(const QString &jsonResponse);
    void setLogLines(const QStringList &logLines);
    void clear();
    QStringList logLines() const { return m_logLines; }

private:
    QStringList m_logLines;
    QFont m_logFont;
};


// ============================================================================
// BFilterComboModel - Model for filter ComboBox data
// ============================================================================

/**
 * @brief Model class for filter ComboBox data
 *
 * Extracts unique values from job JSON data for use in filter ComboBoxes.
 * Provides lists of unique job names and client names.
 *
 * @since 1.0
 */
class BFilterComboModel : public QObject
{
    Q_OBJECT

public:
    explicit BFilterComboModel(QObject *parent = nullptr);

    void updateFromJobsArray(const QJsonArray &jobsArray);
    void updateJobNamesFromDotCommand(const QString &dotJobsResponse);
    void updateClientNamesFromDotCommand(const QString &dotClientsResponse);

    QStringList jobNames() const { return m_jobNames; }
    QStringList clientNames() const { return m_clientNames; }
    void clear();

signals:
    void dataUpdated();

private:
    QStringList m_jobNames;
    QStringList m_clientNames;
};

#endif // BJOBMODELS_H
