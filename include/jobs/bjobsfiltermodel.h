#ifndef BJOBSFILTERMODEL_H
#define BJOBSFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QSet>
#include <QDateTime>

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
    
    /**
     * @brief Sets the job name filter (case-insensitive substring match)
     * @param name Name pattern to filter
     * @since 1.0
     */
    void setNameFilter(const QString &name);
    
    /**
     * @brief Sets the client name filter (case-insensitive substring match)
     * @param client Client pattern to filter
     * @since 1.0
     */
    void setClientFilter(const QString &client);
    
    /**
     * @brief Sets status filter (can filter multiple statuses)
     * @param statuses Set of status codes to show (empty = show all)
     * @since 1.0
     */
    void setStatusFilter(const QSet<QString> &statuses);
    
    /**
     * @brief Sets backup level filter
     * @param levels Set of level codes (F/I/D) to show (empty = show all)
     * @since 1.0
     */
    void setLevelFilter(const QSet<QString> &levels);
    
    /**
     * @brief Sets date range filter
     * @param from Start date (inclusive), invalid QDateTime = no limit
     * @param to End date (inclusive), invalid QDateTime = no limit
     * @since 1.0
     */
    void setDateFilter(const QDateTime &from, const QDateTime &to);
    
    /**
     * @brief Sets file count range filter
     * @param min Minimum file count (-1 = no limit)
     * @param max Maximum file count (-1 = no limit)
     * @since 1.0
     */
    void setFileCountFilter(qint64 min, qint64 max);
    
    /**
     * @brief Sets byte size range filter
     * @param min Minimum bytes (-1 = no limit)
     * @param max Maximum bytes (-1 = no limit)
     * @since 1.0
     */
    void setByteSizeFilter(qint64 min, qint64 max);
    
    /**
     * @brief Clears all filters
     * @since 1.0
     */
    void clearAllFilters();
    
    /**
     * @brief Returns current name filter
     * @return Name filter pattern
     * @since 1.0
     */
    QString nameFilter() const { return m_nameFilter; }
    
    /**
     * @brief Returns current client filter
     * @return Client filter pattern
     * @since 1.0
     */
    QString clientFilter() const { return m_clientFilter; }
    
    /**
     * @brief Returns whether any filter is active
     * @return True if at least one filter is active
     * @since 1.0
     */
    bool hasActiveFilters() const;

public slots:

    /**
     * @brief Sets date range filter
     * @param from Start date (inclusive), invalid QDateTime = no limit
     * @param to End date (inclusive), invalid QDateTime = no limit
     * @since 1.0
     */
    void setDateRange(const QDateTime &from, const QDateTime &to);

protected:
    /**
     * @brief Determines if a row should be shown based on filters
     * @param sourceRow Row in source model
     * @param sourceParent Parent index in source model
     * @return True if row passes all filters
     * @since 1.0
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_nameFilter;           ///< Job name filter pattern
    QString m_clientFilter;         ///< Client name filter pattern
    QSet<QString> m_statusFilter;   ///< Status codes to show
    QSet<QString> m_levelFilter;    ///< Level codes to show
    QDateTime m_dateFrom;           ///< Start date filter
    QDateTime m_dateTo;             ///< End date filter
    qint64 m_fileCountMin;          ///< Minimum file count
    qint64 m_fileCountMax;          ///< Maximum file count
    qint64 m_byteSizeMin;           ///< Minimum byte size
    qint64 m_byteSizeMax;           ///< Maximum byte size
};

#endif // BJOBSFILTERMODEL_H
