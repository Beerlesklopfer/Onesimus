#ifndef BJOBSMODEL_H
#define BJOBSMODEL_H

#include <QAbstractTableModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QSet>
#include <QDateTime>

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

    explicit BJobsModel(QObject *parent = nullptr);
    
    // QAbstractTableModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    
    /**
     * @brief Loads jobs from a JSON array (replaces existing data)
     * @param jobs JSON array containing job objects
     * @since 1.0
     */
    void setJobs(const QJsonArray &jobs);
    
    /**
     * @brief Appends new jobs to existing data (for live updates)
     * @param jobs JSON array containing new job objects
     * @since 2.0
     */
    void appendJobs(const QJsonArray &jobs);
    
    /**
     * @brief Returns the job object at the given row
     * @param row Row index
     * @return QJsonObject containing job data
     * @since 1.0
     */
    QJsonObject jobAt(int row) const;
    
    /**
     * @brief Returns all job objects
     * @return QJsonArray containing all jobs
     * @since 2.0
     */
    QJsonArray allJobs() const { return m_jobs; }
    
    /**
     * @brief Returns the set of selected job IDs
     * @return QSet of selected job ID strings
     * @since 1.0
     */
    QSet<QString> selectedJobIds() const { return m_selectedJobs; }
    
    /**
     * @brief Returns job objects for all selected jobs
     * @return QJsonArray containing selected job objects
     * @since 2.0
     */
    QJsonArray selectedJobs() const;
    
    /**
     * @brief Clears all selections
     * @since 1.0
     */
    void clearSelection();
    
    /**
     * @brief Selects or deselects a job by row index
     * @param row Row index
     * @param selected True to select, false to deselect
     * @since 1.0
     */
    void setJobSelected(int row, bool selected);
    
    /**
     * @brief Selects all jobs
     * @since 2.0
     */
    void selectAll();
    
    /**
     * @brief Inverts the current selection
     * @since 2.0
     */
    void invertSelection();
    
    /**
     * @brief Toggles all selection (for header checkbox)
     * @since 2.1
     */
    void toggleAllSelection();
    
    /**
     * @brief Calculates statistics for all jobs
     * @return Statistics structure with aggregated data
     * @since 2.0
     */
    Statistics calculateStatistics() const;
    
    /**
     * @brief Exports jobs to JSON format
     * @param selectedOnly If true, exports only selected jobs
     * @return QJsonDocument containing exported jobs
     * @since 2.0
     */
    QJsonDocument exportToJson(bool selectedOnly = false) const;
    
    /**
     * @brief Exports jobs to CSV format
     * @param selectedOnly If true, exports only selected jobs
     * @return QString containing CSV data
     * @since 2.0
     */
    QString exportToCsv(bool selectedOnly = false) const;
    
    // Pagination support
    /**
     * @brief Enables or disables pagination
     * @param enabled True to enable pagination
     * @param pageSize Number of rows per page (default: 50)
     * @since 2.0
     */
    void setPaginationEnabled(bool enabled, int pageSize = 50);
    
    /**
     * @brief Returns whether pagination is enabled
     * @return True if pagination is enabled
     * @since 2.0
     */
    bool isPaginationEnabled() const { return m_paginationEnabled; }
    
    /**
     * @brief Sets the current page (0-indexed)
     * @param page Page number
     * @since 2.0
     */
    void setCurrentPage(int page);
    
    /**
     * @brief Returns the current page number
     * @return Current page (0-indexed)
     * @since 2.0
     */
    int currentPage() const { return m_currentPage; }
    
    /**
     * @brief Returns the total number of pages
     * @return Total page count
     * @since 2.0
     */
    int pageCount() const;
    
    /**
     * @brief Returns the page size
     * @return Number of rows per page
     * @since 2.0
     */
    int pageSize() const { return m_pageSize; }

signals:
    /**
     * @brief Emitted when the selection changes
     * @since 1.0
     */
    void selectionChanged();
    
    /**
     * @brief Emitted when new jobs are appended (live update)
     * @param count Number of jobs added
     * @since 2.0
     */
    void jobsAppended(int count);
    
    /**
     * @brief Emitted when the current page changes
     * @param page New page number
     * @since 2.0
     */
    void currentPageChanged(int page);

private:
    /**
     * @brief Formats bytes into human-readable string
     * @param bytes Byte count
     * @return Formatted string (e.g., "1.5 GB")
     * @since 1.0
     */
    QString formatBytes(qint64 bytes) const;
    
    /**
     * @brief Converts status code to descriptive string
     * @param status Single character status code
     * @return Descriptive status string
     * @since 1.0
     */
    QString formatStatus(const QString &status) const;
    
    /**
     * @brief Converts logical row to actual data index
     * @param row Logical row in view
     * @return Actual index in m_jobs array
     * @since 2.0
     */
    int rowToDataIndex(int row) const;
    
    // Data members
    QJsonArray m_jobs;                  ///< All job data (complete dataset)
    QSet<QString> m_selectedJobs;       ///< Set of selected job IDs
    
    // Pagination members
    bool m_paginationEnabled;           ///< Whether pagination is active
    int m_currentPage;                  ///< Current page number (0-indexed)
    int m_pageSize;                     ///< Number of rows per page
};

#endif // BJOBSMODEL_H
