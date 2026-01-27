#include "jobs/bjobsmodel.h"
#include <QBrush>
#include <QColor>
#include <QJsonDocument>
#include <QLocale>

BJobsModel::BJobsModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_paginationEnabled(false)
    , m_currentPage(0)
    , m_pageSize(50)
{
}

int BJobsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    
    if (m_paginationEnabled) {
        int totalRows = m_jobs.size();
        int startRow = m_currentPage * m_pageSize;
        if (startRow >= totalRows)
            return 0;
        return qMin(m_pageSize, totalRows - startRow);
    }
    
    return m_jobs.size();
}

int BJobsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return COL_COUNT;
}

QVariant BJobsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount())
        return QVariant();
    
    int dataIndex = rowToDataIndex(index.row());
    if (dataIndex >= m_jobs.size())
        return QVariant();
    
    QJsonObject job = m_jobs[dataIndex].toObject();
    
    // Handle checkbox column
    if (index.column() == COL_SELECTED) {
        if (role == Qt::CheckStateRole) {
            QString jobId = job["jobid"].toString();
            return m_selectedJobs.contains(jobId) ? Qt::Checked : Qt::Unchecked;
        }
        return QVariant();
    }
    
    // Display role for other columns
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case COL_JOBID:
                return job["jobid"].toString();
            case COL_NAME:
                return job["name"].toString();
            case COL_CLIENT:
                return job["client"].toString();
            case COL_STARTTIME: {
                // Use locale-aware date/time formatting
                QString startTimeStr = job["starttime"].toString();
                QDateTime startTime = QDateTime::fromString(startTimeStr, Qt::ISODate);
                if (!startTime.isValid()) {
                    startTime = QDateTime::fromString(startTimeStr, "yyyy-MM-dd HH:mm:ss");
                }
                if (startTime.isValid()) {
                    return QLocale().toString(startTime, QLocale::ShortFormat);
                }
                return startTimeStr;
            }
            case COL_ENDTIME: {
                // Calculate end time from start time + duration with locale formatting
                QString startTimeStr = job["starttime"].toString();
                QString durationStr = job["duration"].toString();
                
                QDateTime startTime = QDateTime::fromString(startTimeStr, Qt::ISODate);
                if (!startTime.isValid()) {
                    startTime = QDateTime::fromString(startTimeStr, "yyyy-MM-dd HH:mm:ss");
                }
                
                if (startTime.isValid() && !durationStr.isEmpty()) {
                    // Parse duration (format: HH:MM:SS)
                    QStringList parts = durationStr.split(':');
                    if (parts.size() == 3) {
                        int hours = parts[0].toInt();
                        int minutes = parts[1].toInt();
                        int seconds = parts[2].toInt();
                        int totalSeconds = hours * 3600 + minutes * 60 + seconds;
                        
                        QDateTime endTime = startTime.addSecs(totalSeconds);
                        // Use locale-aware formatting
                        return QLocale().toString(endTime, QLocale::ShortFormat);
                    }
                }
                return QString("-");
            }
            case COL_DURATION:
                return job["duration"].toString();
            case COL_TYPE:
                return job["type"].toString();
            case COL_LEVEL:
                return job["level"].toString();
            case COL_FILES: {
                // Use locale-aware number formatting
                qint64 files = job["jobfiles"].toString().toLongLong();
                return QLocale().toString(files);
            }
            case COL_BYTES:
                return formatBytes(job["jobbytes"].toString().toLongLong());
            case COL_STATUS:
                return formatStatus(job["jobstatus"].toString());
        }
    }
    
    // Background color based on status
    if (role == Qt::BackgroundRole && index.column() == COL_STATUS) {
        QString status = job["jobstatus"].toString();
        if (status == "T") {
            return QBrush(QColor(200, 255, 200)); // Light green for success
        } else if (status == "W") {
            return QBrush(QColor(255, 255, 200)); // Light yellow for warning
        } else if (status == "f" || status == "E") {
            return QBrush(QColor(255, 200, 200)); // Light red for failure
        }
    }
    
    // Tooltip
    if (role == Qt::ToolTipRole) {
        QString status = job["jobstatus"].toString();
        QString tooltip = formatStatus(status);
        if (index.column() == COL_BYTES) {
            tooltip = QString("%1 bytes").arg(job["jobbytes"].toString());
        }
        return tooltip;
    }
    
    return QVariant();
}

QVariant BJobsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal)
        return QVariant();
    
    // Handle checkbox column header
    if (section == COL_SELECTED) {
        if (role == Qt::CheckStateRole) {
            // Return tri-state: checked/unchecked/partially checked
            if (m_jobs.isEmpty()) {
                return Qt::Unchecked;
            }
            
            int selectedCount = 0;
            for (const QJsonValue &job : m_jobs) {
                QJsonObject jobObj = job.toObject();
                QString jobId = jobObj["jobid"].toString();
                if (m_selectedJobs.contains(jobId)) {
                    selectedCount++;
                }
            }
            
            if (selectedCount == 0) {
                return Qt::Unchecked;
            } else if (selectedCount == m_jobs.size()) {
                return Qt::Checked;
            } else {
                return Qt::PartiallyChecked;  // Tri-state!
            }
        }
        if (role == Qt::DisplayRole) {
            return QVariant(); // No text, only checkbox
        }
        return QVariant();
    }
    
    // Other columns
    if (role != Qt::DisplayRole)
        return QVariant();
    
    switch (section) {
        case COL_JOBID: return tr("Job ID");
        case COL_NAME: return tr("Name");
        case COL_CLIENT: return tr("Client");
        case COL_STARTTIME: return tr("Start Time");
        case COL_ENDTIME: return tr("End Time");
        case COL_DURATION: return tr("Duration");
        case COL_TYPE: return tr("Type");
        case COL_LEVEL: return tr("Level");
        case COL_FILES: return tr("Files");
        case COL_BYTES: return tr("Size");
        case COL_STATUS: return tr("Status");
        default: return QVariant();
    }
}

Qt::ItemFlags BJobsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    
    // Make checkbox column checkable
    if (index.column() == COL_SELECTED) {
        flags |= Qt::ItemIsUserCheckable;
    }
    
    return flags;
}

bool BJobsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= rowCount())
        return false;
    
    // Handle checkbox toggle
    if (index.column() == COL_SELECTED && role == Qt::CheckStateRole) {
        int dataIndex = rowToDataIndex(index.row());
        if (dataIndex >= m_jobs.size())
            return false;
            
        QJsonObject job = m_jobs[dataIndex].toObject();
        QString jobId = job["jobid"].toString();
        
        if (value.toInt() == Qt::Checked) {
            m_selectedJobs.insert(jobId);
        } else {
            m_selectedJobs.remove(jobId);
        }
        
        emit dataChanged(index, index, {Qt::CheckStateRole});
        emit selectionChanged();
        return true;
    }
    
    return false;
}

void BJobsModel::appendJobs(const QJsonArray &jobs)
{
    if (jobs.isEmpty())
        return;
    
    int oldSize = m_jobs.size();
    int newSize = oldSize + jobs.size();
    
    beginInsertRows(QModelIndex(), oldSize, newSize - 1);
    
    for (const QJsonValue &job : jobs) {
        m_jobs.append(job);
    }
    
    endInsertRows();
    
    emit jobsAppended(jobs.size());
}

QJsonArray BJobsModel::selectedJobs() const
{
    QJsonArray result;
    
    for (const QJsonValue &job : m_jobs) {
        QJsonObject jobObj = job.toObject();
        QString jobId = jobObj["jobid"].toString();
        if (m_selectedJobs.contains(jobId)) {
            result.append(job);
        }
    }
    
    return result;
}

void BJobsModel::selectAll()
{
    for (const QJsonValue &job : m_jobs) {
        QJsonObject jobObj = job.toObject();
        QString jobId = jobObj["jobid"].toString();
        m_selectedJobs.insert(jobId);
    }
    
    emit dataChanged(index(0, COL_SELECTED), 
                     index(rowCount() - 1, COL_SELECTED),
                     {Qt::CheckStateRole});
    emit headerDataChanged(Qt::Horizontal, COL_SELECTED, COL_SELECTED);
    emit selectionChanged();
}

void BJobsModel::invertSelection()
{
    QSet<QString> newSelection;
    
    for (const QJsonValue &job : m_jobs) {
        QJsonObject jobObj = job.toObject();
        QString jobId = jobObj["jobid"].toString();
        if (!m_selectedJobs.contains(jobId)) {
            newSelection.insert(jobId);
        }
    }
    
    m_selectedJobs = newSelection;
    
    emit dataChanged(index(0, COL_SELECTED), 
                     index(rowCount() - 1, COL_SELECTED),
                     {Qt::CheckStateRole});
    emit headerDataChanged(Qt::Horizontal, COL_SELECTED, COL_SELECTED);
    emit selectionChanged();
}

void BJobsModel::toggleAllSelection()
{
    if (m_jobs.isEmpty()) {
        return;
    }
    
    // Check if all are selected
    bool allSelected = true;
    for (const QJsonValue &job : m_jobs) {
        QJsonObject jobObj = job.toObject();
        QString jobId = jobObj["jobid"].toString();
        if (!m_selectedJobs.contains(jobId)) {
            allSelected = false;
            break;
        }
    }
    
    if (allSelected) {
        // Deselect all
        clearSelection();
    } else {
        // Select all
        selectAll();
    }
}

BJobsModel::Statistics BJobsModel::calculateStatistics() const
{
    Statistics stats;
    stats.totalJobs = m_jobs.size();
    stats.successfulJobs = 0;
    stats.warningJobs = 0;
    stats.failedJobs = 0;
    stats.totalBytes = 0;
    stats.totalFiles = 0;
    stats.selectedCount = m_selectedJobs.size();
    
    QDateTime earliest = QDateTime::currentDateTime();
    QDateTime latest = QDateTime::fromString("1970-01-01", Qt::ISODate);
    
    for (const QJsonValue &job : m_jobs) {
        QJsonObject jobObj = job.toObject();
        
        // Count status
        QString status = jobObj["jobstatus"].toString();
        if (status == "T") {
            stats.successfulJobs++;
        } else if (status == "W") {
            stats.warningJobs++;
        } else if (status == "f" || status == "E") {
            stats.failedJobs++;
        }
        
        // Sum bytes and files
        stats.totalBytes += jobObj["jobbytes"].toString().toLongLong();
        stats.totalFiles += jobObj["jobfiles"].toString().toLongLong();
        
        // Find earliest and latest
        QDateTime startTime = QDateTime::fromString(
            jobObj["starttime"].toString(), "yyyy-MM-dd HH:mm:ss");
        if (startTime.isValid()) {
            if (startTime < earliest) {
                earliest = startTime;
            }
            if (startTime > latest) {
                latest = startTime;
            }
        }
    }
    
    stats.earliestJob = earliest;
    stats.latestJob = latest;
    
    return stats;
}

QJsonDocument BJobsModel::exportToJson(bool selectedOnly) const
{
    QJsonObject root;
    root["jsonrpc"] = "2.0";
    root["id"] = QJsonValue::Null;
    
    QJsonObject result;
    if (selectedOnly) {
        result["jobs"] = selectedJobs();
    } else {
        result["jobs"] = m_jobs;
    }
    
    root["result"] = result;
    
    return QJsonDocument(root);
}

QString BJobsModel::exportToCsv(bool selectedOnly) const
{
    QString csv;
    
    // Header
    csv += "Job ID,Name,Client,Start Time,Duration,Type,Level,Files,Bytes,Status\n";
    
    // Data
    QJsonArray jobsToExport = selectedOnly ? selectedJobs() : m_jobs;
    
    for (const QJsonValue &job : jobsToExport) {
        QJsonObject jobObj = job.toObject();
        
        csv += QString("\"%1\",\"%2\",\"%3\",\"%4\",\"%5\",\"%6\",\"%7\",\"%8\",\"%9\",\"%10\"\n")
            .arg(jobObj["jobid"].toString())
            .arg(jobObj["name"].toString())
            .arg(jobObj["client"].toString())
            .arg(jobObj["starttime"].toString())
            .arg(jobObj["duration"].toString())
            .arg(jobObj["type"].toString())
            .arg(jobObj["level"].toString())
            .arg(jobObj["jobfiles"].toString())
            .arg(jobObj["jobbytes"].toString())
            .arg(jobObj["jobstatus"].toString());
    }
    
    return csv;
}

void BJobsModel::setPaginationEnabled(bool enabled, int pageSize)
{
    if (m_paginationEnabled == enabled && m_pageSize == pageSize)
        return;
    
    beginResetModel();
    m_paginationEnabled = enabled;
    m_pageSize = pageSize;
    m_currentPage = 0;
    endResetModel();
    
    emit currentPageChanged(0);
}

void BJobsModel::setCurrentPage(int page)
{
    if (page < 0 || page >= pageCount() || page == m_currentPage)
        return;
    
    beginResetModel();
    m_currentPage = page;
    endResetModel();
    
    emit currentPageChanged(page);
}

int BJobsModel::pageCount() const
{
    if (!m_paginationEnabled || m_jobs.isEmpty())
        return 1;
    
    return (m_jobs.size() + m_pageSize - 1) / m_pageSize;
}

int BJobsModel::rowToDataIndex(int row) const
{
    if (!m_paginationEnabled)
        return row;
    
    return (m_currentPage * m_pageSize) + row;
}

void BJobsModel::setJobs(const QJsonArray &jobs)
{
    beginResetModel();
    m_jobs = jobs;
    m_selectedJobs.clear();
    endResetModel();
}

QJsonObject BJobsModel::jobAt(int row) const
{
    int dataIndex = rowToDataIndex(row);
    if (dataIndex >= 0 && dataIndex < m_jobs.size()) {
        return m_jobs[dataIndex].toObject();
    }
    return QJsonObject();
}

void BJobsModel::clearSelection()
{
    if (m_selectedJobs.isEmpty())
        return;
    
    m_selectedJobs.clear();
    emit dataChanged(index(0, COL_SELECTED), 
                     index(rowCount() - 1, COL_SELECTED),
                     {Qt::CheckStateRole});
    emit headerDataChanged(Qt::Horizontal, COL_SELECTED, COL_SELECTED);
    emit selectionChanged();
}

void BJobsModel::setJobSelected(int row, bool selected)
{
    QModelIndex idx = index(row, COL_SELECTED);
    setData(idx, selected ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
}

QString BJobsModel::formatBytes(qint64 bytes) const
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    const qint64 TB = GB * 1024;
    
    QLocale locale;
    
    if (bytes >= TB) {
        double value = bytes / (double)TB;
        return QString("%1 TB").arg(locale.toString(value, 'f', 2));
    } else if (bytes >= GB) {
        double value = bytes / (double)GB;
        return QString("%1 GB").arg(locale.toString(value, 'f', 2));
    } else if (bytes >= MB) {
        double value = bytes / (double)MB;
        return QString("%1 MB").arg(locale.toString(value, 'f', 2));
    } else if (bytes >= KB) {
        double value = bytes / (double)KB;
        return QString("%1 KB").arg(locale.toString(value, 'f', 2));
    } else {
        return QString("%1 B").arg(locale.toString(bytes));
    }
}

QString BJobsModel::formatStatus(const QString &status) const
{
    if (status == "T") return "Terminated normally";
    if (status == "W") return "Terminated with warnings";
    if (status == "f") return "Failed";
    if (status == "E") return "Terminated in Error";
    if (status == "e") return "Non-fatal error";
    if (status == "A") return "Canceled by user";
    if (status == "R") return "Running";
    if (status == "C") return "Created";
    if (status == "F") return "Waiting on File Daemon";
    if (status == "S") return "Waiting on Storage Daemon";
    if (status == "m") return "Waiting for new media";
    if (status == "M") return "Waiting for Mount";
    if (status == "s") return "Waiting for Storage resource";
    if (status == "j") return "Waiting for Job resource";
    if (status == "c") return "Waiting for Client resource";
    if (status == "d") return "Waiting for Maximum jobs";
    if (status == "t") return "Waiting for Start Time";
    if (status == "p") return "Waiting for higher priority job";
    if (status == "i") return "Doing batch insert file records";
    
    return status;
}
