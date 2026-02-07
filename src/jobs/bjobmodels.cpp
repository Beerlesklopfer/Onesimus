#include "jobs/bjobmodels.h"
#include "jobs/blevelcolors.h"
#include "blogging.h"
#include <QBrush>
#include <QColor>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QLocale>
#include <QSet>

// ============================================================================
// BJobsModel Implementation
// ============================================================================

BJobsModel::BJobsModel(BDirector *director, QObject *parent)
    : QAbstractTableModel(parent)
    , m_director(director)
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
                QString startTimeStr = job["starttime"].toString();
                QString durationStr = job["duration"].toString();

                QDateTime startTime = QDateTime::fromString(startTimeStr, Qt::ISODate);
                if (!startTime.isValid()) {
                    startTime = QDateTime::fromString(startTimeStr, "yyyy-MM-dd HH:mm:ss");
                }

                if (startTime.isValid() && !durationStr.isEmpty()) {
                    QStringList parts = durationStr.split(':');
                    if (parts.size() == 3) {
                        int hours = parts[0].toInt();
                        int minutes = parts[1].toInt();
                        int seconds = parts[2].toInt();
                        int totalSeconds = hours * 3600 + minutes * 60 + seconds;

                        QDateTime endTime = startTime.addSecs(totalSeconds);
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
                qint64 files = job["jobfiles"].toString().toLongLong();
                return QLocale().toString(files);
            }
            case COL_BYTES:
                return formatBytes(job["jobbytes"].toString().toLongLong());
            case COL_STATUS:
                return formatStatus(job["jobstatus"].toString());
        }
    }

    // Background color based on backup level
    if (role == Qt::BackgroundRole) {
        QString level = job["level"].toString();
        QColor levelColor = BLevelColors::getLevelColor(level);
        if (levelColor.isValid()) {
            return QBrush(levelColor);
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

    // Text alignment
    if (role == Qt::TextAlignmentRole) {
        if (index.column() == COL_TYPE || index.column() == COL_LEVEL) {
            return int(Qt::AlignCenter);
        }
        if (index.column() == COL_JOBID || index.column() == COL_FILES || index.column() == COL_BYTES) {
            return int(Qt::AlignRight | Qt::AlignVCenter);
        }
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
                return Qt::PartiallyChecked;
            }
        }
        if (role == Qt::DisplayRole) {
            return QVariant();
        }
        return QVariant();
    }

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

    if (index.column() == COL_SELECTED) {
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

bool BJobsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= rowCount())
        return false;

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

void BJobsModel::setJobs(const QJsonArray &jobs)
{
    beginResetModel();
    m_jobs = jobs;

    // Preserve selections: keep only job IDs that still exist in the new data
    if (!m_selectedJobs.isEmpty()) {
        QSet<QString> newJobIds;
        for (const QJsonValue &job : m_jobs) {
            newJobIds.insert(job.toObject()["jobid"].toString());
        }
        m_selectedJobs.intersect(newJobIds);
    }

    endResetModel();
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

void BJobsModel::removeJobsByIds(const QStringList &jobIds)
{
    if (jobIds.isEmpty())
        return;

    // Convert to set for faster lookup
    QSet<QString> idsToRemove(jobIds.begin(), jobIds.end());

    // Build new array without removed jobs
    QJsonArray newJobs;
    for (const QJsonValue &val : m_jobs) {
        QJsonObject job = val.toObject();
        QString jobId = job["jobid"].toString();
        if (!idsToRemove.contains(jobId)) {
            newJobs.append(val);
        } else {
            // Also remove from selection
            m_selectedJobs.remove(jobId);
        }
    }

    // Reset model with filtered data
    beginResetModel();
    m_jobs = newJobs;
    endResetModel();

    emit selectionChanged();
}

QJsonObject BJobsModel::jobAt(int row) const
{
    int dataIndex = rowToDataIndex(row);
    if (dataIndex >= 0 && dataIndex < m_jobs.size()) {
        return m_jobs[dataIndex].toObject();
    }
    return QJsonObject();
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
        clearSelection();
    } else {
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

        QString status = jobObj["jobstatus"].toString();
        if (status == "T") {
            stats.successfulJobs++;
        } else if (status == "W") {
            stats.warningJobs++;
        } else if (status == "f" || status == "E") {
            stats.failedJobs++;
        }

        stats.totalBytes += jobObj["jobbytes"].toString().toLongLong();
        stats.totalFiles += jobObj["jobfiles"].toString().toLongLong();

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

    csv += "Job ID,Name,Client,Start Time,Duration,Type,Level,Files,Bytes,Status\n";

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
    if (status == "F") return "Failed";
    if (status == "f") return "Failed";
    if (status == "E") return "Terminated in Error";
    if (status == "e") return "Non-fatal error";
    if (status == "A") return "Canceled by user";
    if (status == "R") return "Running";
    if (status == "C") return "Created";
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


// ============================================================================
// BJobsFilterModel Implementation
// ============================================================================

BJobsFilterModel::BJobsFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_fileCountMin(-1)
    , m_fileCountMax(-1)
    , m_byteSizeMin(-1)
    , m_byteSizeMax(-1)
    , m_zeroBytesFilter(false)
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

void BJobsFilterModel::setZeroBytesFilter(bool enabled)
{
    m_zeroBytesFilter = enabled;
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
    m_zeroBytesFilter = false;
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
           m_byteSizeMax >= 0 ||
           m_zeroBytesFilter;
}

void BJobsFilterModel::setDateRange(const QDateTime &from, const QDateTime &to)
{
    m_dateFrom = from;
    m_dateTo = to;
    invalidateFilter();
}

bool BJobsFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
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

    // Status filter (exclusion: m_statusFilter contains statuses to HIDE)
    if (!m_statusFilter.isEmpty()) {
        QString status = job["jobstatus"].toString();
        if (m_statusFilter.contains(status)) {
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

    // Zero bytes filter - only show jobs with 0 bytes when enabled
    if (m_zeroBytesFilter && byteSize != 0) {
        return false;
    }

    return true;
}


// ============================================================================
// BJobLogModel Implementation
// ============================================================================

BJobLogModel::BJobLogModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_logFont("Courier", 9)
{
}

int BJobLogModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_logLines.size();
}

QVariant BJobLogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_logLines.size())
        return QVariant();

    const QString &logLine = m_logLines.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return logLine;

    case Qt::FontRole:
        return m_logFont;

    case Qt::ForegroundRole:
        if (logLine.isEmpty() ||
            logLine.startsWith("No log") ||
            logLine.startsWith("Loading")) {
            return QBrush(Qt::gray);
        }
        return QVariant();

    case Qt::BackgroundRole:
        {
            QString lowerLine = logLine.toLower();

            int lastNonEmptyIndex = m_logLines.size() - 1;
            while (lastNonEmptyIndex >= 0 && m_logLines.at(lastNonEmptyIndex).isEmpty()) {
                lastNonEmptyIndex--;
            }

            bool isLastNonEmptyEntry = (index.row() == lastNonEmptyIndex);
            if (isLastNonEmptyEntry &&
                lowerLine.contains("termination") &&
                lowerLine.contains("backup ok")) {
                return QBrush(QColor(200, 255, 200));
            }

            if (lowerLine.contains("fatal") ||
                lowerLine.contains("critical") ||
                lowerLine.contains("kritisch")) {
                return QBrush(QColor(255, 200, 200));
            }

            if (lowerLine.contains("error") ||
                lowerLine.contains("fehler") ||
                lowerLine.contains("failed") ||
                lowerLine.contains("fehlgeschlagen")) {
                return QBrush(QColor(255, 230, 230));
            }

            if (lowerLine.contains("warning") ||
                lowerLine.contains("warnung") ||
                lowerLine.contains("warn")) {
                return QBrush(QColor(255, 255, 200));
            }

            return QVariant();
        }

    default:
        return QVariant();
    }
}

bool BJobLogModel::parseJsonResponse(const QString &jsonResponse)
{
    if (jsonResponse.isEmpty()) {
        beginResetModel();
        m_logLines.clear();
        m_logLines.append(tr("No log data available."));
        endResetModel();
        return true;
    }

    QString cleanedResponse = jsonResponse.trimmed();

    // Remove BOM if present
    if (!cleanedResponse.isEmpty() && cleanedResponse.at(0) == QChar(0xFEFF)) {
        cleanedResponse = cleanedResponse.mid(1);
    }

    // Find first JSON delimiter
    int jsonStart = -1;
    for (int i = 0; i < cleanedResponse.length(); ++i) {
        QChar c = cleanedResponse.at(i);
        if (c == '{' || c == '[') {
            jsonStart = i;
            break;
        }
    }

    if (jsonStart > 0) {
        cleanedResponse = cleanedResponse.mid(jsonStart);
    }

    // Remove duplicate opening braces
    while (cleanedResponse.length() > 1) {
        if (cleanedResponse.startsWith("{{") || cleanedResponse.startsWith("[[")) {
            cleanedResponse = cleanedResponse.mid(1);
        } else {
            break;
        }
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(cleanedResponse.toUtf8(), &parseError);

    QStringList logLines;

    if (parseError.error == QJsonParseError::NoError) {
        if (doc.isObject()) {
            QJsonObject root = doc.object();

            // Try different JSON structures
            if (root.contains("result") && root["result"].isObject()) {
                QJsonObject result = root["result"].toObject();
                if (result.contains("joblog") && result["joblog"].isArray()) {
                    QJsonArray logArray = result["joblog"].toArray();
                    for (const QJsonValue &val : logArray) {
                        QString line;

                        if (val.isObject()) {
                            QJsonObject logEntry = val.toObject();
                            QString time = logEntry["time"].toString();
                            QString logtext = logEntry["logtext"].toString();

                            if (!time.isEmpty() && !logtext.isEmpty()) {
                                line = QString("[%1] %2").arg(time, logtext);
                            } else if (!logtext.isEmpty()) {
                                line = logtext;
                            }
                        } else {
                            line = val.toString();
                        }

                        line = line.trimmed();
                        while (!line.isEmpty() && line.at(0).unicode() < 32 && line.at(0) != '\n' && line.at(0) != '\r') {
                            line = line.mid(1);
                        }
                        if (!line.isEmpty()) {
                            logLines.append(line);
                        }
                    }
                }
            }
            else if (root.contains("joblog") && root["joblog"].isArray()) {
                QJsonArray logArray = root["joblog"].toArray();
                for (const QJsonValue &val : logArray) {
                    QString line;

                    if (val.isObject()) {
                        QJsonObject logEntry = val.toObject();
                        QString time = logEntry["time"].toString();
                        QString logtext = logEntry["logtext"].toString();

                        if (!time.isEmpty() && !logtext.isEmpty()) {
                            line = QString("[%1] %2").arg(time, logtext);
                        } else if (!logtext.isEmpty()) {
                            line = logtext;
                        }
                    } else {
                        line = val.toString();
                    }

                    line = line.trimmed();
                    while (!line.isEmpty() && line.at(0).unicode() < 32 && line.at(0) != '\n' && line.at(0) != '\r') {
                        line = line.mid(1);
                    }
                    if (!line.isEmpty()) {
                        logLines.append(line);
                    }
                }
            }
            else if (root.contains("log") && root["log"].isArray()) {
                QJsonArray logArray = root["log"].toArray();
                for (const QJsonValue &val : logArray) {
                    QString line;

                    if (val.isObject()) {
                        QJsonObject logEntry = val.toObject();
                        QString time = logEntry["time"].toString();
                        QString logtext = logEntry["logtext"].toString();

                        if (!time.isEmpty() && !logtext.isEmpty()) {
                            line = QString("[%1] %2").arg(time, logtext);
                        } else if (!logtext.isEmpty()) {
                            line = logtext;
                        }
                    } else {
                        line = val.toString();
                    }

                    line = line.trimmed();
                    while (!line.isEmpty() && line.at(0).unicode() < 32 && line.at(0) != '\n' && line.at(0) != '\r') {
                        line = line.mid(1);
                    }
                    if (!line.isEmpty()) {
                        logLines.append(line);
                    }
                }
            }
        }
        else if (doc.isArray()) {
            QJsonArray logArray = doc.array();
            for (const QJsonValue &val : logArray) {
                QString line;

                if (val.isObject()) {
                    QJsonObject logEntry = val.toObject();
                    QString time = logEntry["time"].toString();
                    QString logtext = logEntry["logtext"].toString();

                    if (!time.isEmpty() && !logtext.isEmpty()) {
                        line = QString("[%1] %2").arg(time, logtext);
                    } else if (!logtext.isEmpty()) {
                        line = logtext;
                    }
                } else {
                    line = val.toString();
                }

                line = line.trimmed();
                while (!line.isEmpty() && line.at(0).unicode() < 32 && line.at(0) != '\n' && line.at(0) != '\r') {
                    line = line.mid(1);
                }
                if (!line.isEmpty()) {
                    logLines.append(line);
                }
            }
        }

        if (logLines.isEmpty()) {
            logLines.append(tr("No log entries found in JSON response."));
        }
    } else {
        BLOG_WARNING() << "BJobLogModel: Not valid JSON, treating as plain text";
        BLOG_WARNING() << "  Parse error:" << parseError.errorString() << "at offset" << parseError.offset;

        QStringList rawLines = cleanedResponse.split('\n');

        for (QString line : rawLines) {
            line = line.trimmed();
            while (!line.isEmpty() && line.at(0).unicode() < 32 && line.at(0) != '\n' && line.at(0) != '\r') {
                line = line.mid(1);
            }
            if (!line.isEmpty()) {
                logLines.append(line);
            }
        }
    }

    if (!logLines.isEmpty()) {
        logLines.append("");
        logLines.append("");
    }

    beginResetModel();
    m_logLines = logLines;
    endResetModel();

    return true;
}

void BJobLogModel::setLogLines(const QStringList &logLines)
{
    beginResetModel();
    m_logLines = logLines;
    endResetModel();
}

void BJobLogModel::clear()
{
    beginResetModel();
    m_logLines.clear();
    endResetModel();
}


// ============================================================================
// BFilterComboModel Implementation
// ============================================================================

BFilterComboModel::BFilterComboModel(QObject *parent)
    : QObject(parent)
{
}

void BFilterComboModel::updateFromJobsArray(const QJsonArray &jobsArray)
{
    QSet<QString> jobNamesSet;
    QSet<QString> clientNamesSet;

    for (const QJsonValue &jobVal : jobsArray) {
        if (!jobVal.isObject()) {
            continue;
        }

        QJsonObject job = jobVal.toObject();

        QString jobName = job["name"].toString().trimmed();
        if (!jobName.isEmpty()) {
            jobNamesSet.insert(jobName);
        }

        QString clientName = job["client"].toString().trimmed();
        if (!clientName.isEmpty()) {
            clientNamesSet.insert(clientName);
        }
    }

    m_jobNames = jobNamesSet.values();
    m_jobNames.sort(Qt::CaseInsensitive);

    m_clientNames = clientNamesSet.values();
    m_clientNames.sort(Qt::CaseInsensitive);

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BFilterComboModel: Updated with" << m_jobNames.size() << "unique job names and"
             << m_clientNames.size() << "unique client names";
#endif

    emit dataUpdated();
}

void BFilterComboModel::updateJobNamesFromDotCommand(const QString &dotJobsResponse)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(dotJobsResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BFilterComboModel: Failed to parse .jobs response:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "BFilterComboModel: .jobs response is not a JSON object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonArray jobsArray = result["jobs"].toArray();

    QSet<QString> jobNamesSet;

    for (const QJsonValue &jobVal : jobsArray) {
        if (!jobVal.isObject()) {
            continue;
        }

        QJsonObject job = jobVal.toObject();
        QString jobName = job["name"].toString().trimmed();

        if (!jobName.isEmpty()) {
            jobNamesSet.insert(jobName);
        }
    }

    m_jobNames = jobNamesSet.values();
    m_jobNames.sort(Qt::CaseInsensitive);

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BFilterComboModel: Updated job names from .jobs -" << m_jobNames.size() << "jobs";
#endif

    emit dataUpdated();
}

void BFilterComboModel::updateClientNamesFromDotCommand(const QString &dotClientsResponse)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(dotClientsResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BFilterComboModel: Failed to parse .clients response:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "BFilterComboModel: .clients response is not a JSON object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonArray clientsArray = result["clients"].toArray();

    QSet<QString> clientNamesSet;

    for (const QJsonValue &clientVal : clientsArray) {
        if (!clientVal.isObject()) {
            continue;
        }

        QJsonObject client = clientVal.toObject();
        QString clientName = client["name"].toString().trimmed();

        if (!clientName.isEmpty()) {
            clientNamesSet.insert(clientName);
        }
    }

    m_clientNames = clientNamesSet.values();
    m_clientNames.sort(Qt::CaseInsensitive);

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BFilterComboModel: Updated client names from .clients -" << m_clientNames.size() << "clients";
#endif

    emit dataUpdated();
}

void BFilterComboModel::clear()
{
    m_jobNames.clear();
    m_clientNames.clear();
    emit dataUpdated();
}
