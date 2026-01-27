#include "jobs/bjobsfiltermodel.h"
#include "jobs/bjobsmodel.h"
#include <QJsonObject>

BJobsFilterModel::BJobsFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_fileCountMin(-1)
    , m_fileCountMax(-1)
    , m_byteSizeMin(-1)
    , m_byteSizeMax(-1)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortCaseSensitivity(Qt::CaseInsensitive);
}

void BJobsFilterModel::setNameFilter(const QString &name)
{
    m_nameFilter = name;
    invalidateFilter();
}

void BJobsFilterModel::setClientFilter(const QString &client)
{
    m_clientFilter = client;
    invalidateFilter();
}

void BJobsFilterModel::setStatusFilter(const QSet<QString> &statuses)
{
    m_statusFilter = statuses;
    invalidateFilter();
}

void BJobsFilterModel::setLevelFilter(const QSet<QString> &levels)
{
    m_levelFilter = levels;
    invalidateFilter();
}

void BJobsFilterModel::setDateFilter(const QDateTime &from, const QDateTime &to)
{
    m_dateFrom = from;
    m_dateTo = to;
    invalidateFilter();
}

void BJobsFilterModel::setFileCountFilter(qint64 min, qint64 max)
{
    m_fileCountMin = min;
    m_fileCountMax = max;
    invalidateFilter();
}

void BJobsFilterModel::setByteSizeFilter(qint64 min, qint64 max)
{
    m_byteSizeMin = min;
    m_byteSizeMax = max;
    invalidateFilter();
}

void BJobsFilterModel::clearAllFilters()
{
    m_nameFilter.clear();
    m_clientFilter.clear();
    m_statusFilter.clear();
    m_levelFilter.clear();
    m_dateFrom = QDateTime();
    m_dateTo = QDateTime();
    m_fileCountMin = -1;
    m_fileCountMax = -1;
    m_byteSizeMin = -1;
    m_byteSizeMax = -1;
    invalidateFilter();
}

bool BJobsFilterModel::hasActiveFilters() const
{
    return !m_nameFilter.isEmpty() ||
           !m_clientFilter.isEmpty() ||
           !m_statusFilter.isEmpty() ||
           !m_levelFilter.isEmpty() ||
           m_dateFrom.isValid() ||
           m_dateTo.isValid() ||
           m_fileCountMin >= 0 ||
           m_fileCountMax >= 0 ||
           m_byteSizeMin >= 0 ||
           m_byteSizeMax >= 0;
}

bool BJobsFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    
    // Get job data from source model
    BJobsModel *model = qobject_cast<BJobsModel*>(sourceModel());
    if (!model) {
        return true;
    }
    
    QJsonObject job = model->jobAt(sourceRow);
    if (job.isEmpty()) {
        return false;
    }
    
    // Name filter
    if (!m_nameFilter.isEmpty()) {
        QString name = job["name"].toString();
        if (!name.contains(m_nameFilter, Qt::CaseInsensitive)) {
            return false;
        }
    }
    
    // Client filter
    if (!m_clientFilter.isEmpty()) {
        QString client = job["client"].toString();
        if (!client.contains(m_clientFilter, Qt::CaseInsensitive)) {
            return false;
        }
    }
    
    // Status filter
    if (!m_statusFilter.isEmpty()) {
        QString status = job["jobstatus"].toString();
        if (!m_statusFilter.contains(status)) {
            return false;
        }
    }
    
    // Level filter
    if (!m_levelFilter.isEmpty()) {
        QString level = job["level"].toString();
        if (!m_levelFilter.contains(level)) {
            return false;
        }
    }
    
    // Date range filter
    QString startTimeStr = job["starttime"].toString();
    QDateTime startTime = QDateTime::fromString(startTimeStr, "yyyy-MM-dd HH:mm:ss");
    
    if (m_dateFrom.isValid() && startTime.isValid()) {
        if (startTime < m_dateFrom) {
            return false;
        }
    }
    
    if (m_dateTo.isValid() && startTime.isValid()) {
        if (startTime > m_dateTo) {
            return false;
        }
    }
    
    // File count filter
    qint64 fileCount = job["jobfiles"].toString().toLongLong();
    
    if (m_fileCountMin >= 0 && fileCount < m_fileCountMin) {
        return false;
    }
    
    if (m_fileCountMax >= 0 && fileCount > m_fileCountMax) {
        return false;
    }
    
    // Byte size filter
    qint64 byteSize = job["jobbytes"].toString().toLongLong();
    
    if (m_byteSizeMin >= 0 && byteSize < m_byteSizeMin) {
        return false;
    }
    
    if (m_byteSizeMax >= 0 && byteSize > m_byteSizeMax) {
        return false;
    }

    // Date range filter
    if (m_dateFrom.isValid() || m_dateTo.isValid()) {
        QString startTimeStr = job["starttime"].toString();
        QDateTime startTime = QDateTime::fromString(startTimeStr, Qt::ISODate);
        if (!startTime.isValid()) {
            startTime = QDateTime::fromString(startTimeStr, "yyyy-MM-dd HH:mm:ss");
        }

        if (startTime.isValid()) {
            if (m_dateFrom.isValid() && startTime < m_dateFrom) {
                return false;
            }
            if (m_dateTo.isValid() && startTime > m_dateTo) {
                return false;
            }
        }
    }

    return true;
}

void BJobsFilterModel::setDateRange(const QDateTime &from, const QDateTime &to)
{
    m_dateFrom = from;
    m_dateTo = to;
    invalidateFilter();
}

