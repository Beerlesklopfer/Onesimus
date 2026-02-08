#include "models/bbvfsmodel.h"
#include "blogging.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDateTime>
#include <QIcon>

#define BVFS_DEBUG BLOG_DEBUG()

// ============================================================================
// Constructor / Destructor
// ============================================================================

BBvfsModel::BBvfsModel(BDirector *director, QObject *parent)
    : QAbstractItemModel(parent)
    , m_director(director)
{
    BVFS_DEBUG << "Model created";

    if (m_director) {
        connect(m_director, &BDirector::jsonResult,
                this, &BBvfsModel::onJsonResponse);
        connect(m_director, &BDirector::textResult,
                this, &BBvfsModel::onCommandResponse);
        BVFS_DEBUG << "Connected to Director jsonResult + textResult signals";
    }
}

BBvfsModel::~BBvfsModel()
{
    BVFS_DEBUG << "Model destroyed";
    delete m_rootNode;
}

// ============================================================================
// Public API
// ============================================================================

void BBvfsModel::loadJob(quint64 jobId, bool resolveAllRelatedJobs)
{
    BVFS_DEBUG << "========================================";
    BVFS_DEBUG << "loadJob: jobId=" << jobId
               << " resolveAll=" << resolveAllRelatedJobs;
    BVFS_DEBUG << "========================================";

    resetModel();

    m_jobId = jobId;

    // Create invisible root node
    m_rootNode = new BvfsNode;
    m_rootNode->name = "";
    m_rootNode->fullPath = "/";
    m_rootNode->pathId = 0;
    m_rootNode->isDirectory = true;
    BVFS_DEBUG << "Root node created (invisible)";

    emit loadingStarted();

    if (resolveAllRelatedJobs) {
        // First resolve all related job IDs via bvfs_get_jobids
        QString args = QString("jobid=%1 all").arg(jobId);
        enqueueCommand({BDirector::Command::BvfsGetJobIds, args, m_rootNode});
    } else {
        m_bvfsJobIds = QString::number(jobId);
        BVFS_DEBUG << "Using single job ID: " << m_bvfsJobIds;

        // Send bvfs_update (returns text response via textResult signal)
        QString args = QString("jobid=%1").arg(m_bvfsJobIds);
        enqueueCommand({BDirector::Command::BvfsUpdate, args, m_rootNode});
    }
}

void BBvfsModel::resetModel()
{
    BVFS_DEBUG << "resetModel()";

    beginResetModel();

    m_commandQueue.clear();
    m_commandPending = false;
    m_pendingNode = nullptr;
    m_bvfsJobIds.clear();
    m_jobId = 0;

    delete m_rootNode;
    m_rootNode = nullptr;

    endResetModel();
}

// ============================================================================
// QAbstractItemModel interface
// ============================================================================

QModelIndex BBvfsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!m_rootNode || !hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    BvfsNode *parentNode = parent.isValid()
        ? nodeFromIndex(parent)
        : m_rootNode;

    if (!parentNode || row < 0 || row >= parentNode->children.size()) {
        return QModelIndex();
    }

    BvfsNode *childNode = parentNode->children.at(row);
    return createIndex(row, column, childNode);
}

QModelIndex BBvfsModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || !m_rootNode) {
        return QModelIndex();
    }

    BvfsNode *childNode = nodeFromIndex(child);
    if (!childNode || !childNode->parentNode || childNode->parentNode == m_rootNode) {
        return QModelIndex();
    }

    BvfsNode *parentNode = childNode->parentNode;
    return createIndex(parentNode->row(), 0, parentNode);
}

int BBvfsModel::rowCount(const QModelIndex &parent) const
{
    if (!m_rootNode) {
        return 0;
    }

    // Only column 0 has children in tree models
    if (parent.column() > 0) {
        return 0;
    }

    BvfsNode *parentNode = parent.isValid()
        ? nodeFromIndex(parent)
        : m_rootNode;

    return parentNode ? parentNode->children.size() : 0;
}

int BBvfsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColCount;
}

Qt::ItemFlags BBvfsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractItemModel::flags(index);
    if (m_checkable && index.isValid() && index.column() == ColName) {
        f |= Qt::ItemIsUserCheckable;
    }
    return f;
}

bool BBvfsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_checkable || role != Qt::CheckStateRole || !index.isValid() || index.column() != ColName) {
        return false;
    }

    BvfsNode *node = nodeFromIndex(index);
    if (!node) return false;

    Qt::CheckState newState = static_cast<Qt::CheckState>(value.toInt());
    node->checkState = newState;

    BVFS_DEBUG << "setData checkState: " << node->name
               << " -> " << (newState == Qt::Checked ? "Checked" : newState == Qt::PartiallyChecked ? "Partial" : "Unchecked");

    // Propagate to children (if directory, check/uncheck all loaded children)
    if (node->isDirectory) {
        propagateCheckState(node, newState);
    }

    // Update parent tri-state
    updateParentCheckState(node->parentNode);

    emit dataChanged(index, index, {Qt::CheckStateRole});
    emit selectionCountChanged(selectedCount());
    return true;
}

QVariant BBvfsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_rootNode) {
        return QVariant();
    }

    BvfsNode *node = nodeFromIndex(index);
    if (!node) {
        return QVariant();
    }

    switch (role) {
    case Qt::CheckStateRole:
        if (m_checkable && index.column() == ColName) {
            return node->checkState;
        }
        return QVariant();

    case Qt::DisplayRole:
        switch (index.column()) {
        case ColName:
            return node->name;
        case ColSize:
            return node->isDirectory ? QVariant(QString("-")) : QVariant(formatBytes(node->fileSize));
        case ColType:
            if (node->isDirectory) return tr("Directory");
            if (node->mode > 0 && (node->mode & 0170000) == 0120000) return tr("Symlink");
            return tr("File");
        case ColModified:
            if (node->mtime > 0) {
                return QDateTime::fromSecsSinceEpoch(node->mtime).toString("yyyy-MM-dd hh:mm:ss");
            }
            return QString("-");
        }
        break;

    case Qt::DecorationRole:
        if (index.column() == ColName) {
            if (node->isDirectory) {
                // Drive letters get a different icon
                if (node->name.length() == 2 && node->name[1] == ':') {
                    return QIcon::fromTheme("drive-harddisk");
                }
                return QIcon::fromTheme("folder");
            }
            if (node->mode > 0 && (node->mode & 0170000) == 0120000) {
                return QIcon::fromTheme("emblem-symbolic-link");
            }
            return QIcon::fromTheme("text-x-generic");
        }
        break;

    case PathIdRole:
        return node->pathId;

    case FullPathRole:
        return node->fullPath;

    case IsDirectoryRole:
        return node->isDirectory;

    case FileIdRole:
        return node->fileId;
    }

    return QVariant();
}

QVariant BBvfsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case ColName:     return tr("Name");
        case ColSize:     return tr("Size");
        case ColType:     return tr("Type");
        case ColModified: return tr("Modified");
        }
    }
    return QVariant();
}

bool BBvfsModel::canFetchMore(const QModelIndex &parent) const
{
    if (!m_rootNode) {
        return false;
    }

    BvfsNode *node = parent.isValid() ? nodeFromIndex(parent) : m_rootNode;
    if (!node || !node->isDirectory) {
        return false;
    }

    bool result = (node->dirLoadState == BvfsNode::NotLoaded);

    if (result) {
        BVFS_DEBUG << "canFetchMore(" << node->fullPath << ") = true"
                   << " (dirLoadState=NotLoaded)";
    }

    return result;
}

void BBvfsModel::fetchMore(const QModelIndex &parent)
{
    if (!m_rootNode || !m_director) {
        return;
    }

    BvfsNode *node = parent.isValid() ? nodeFromIndex(parent) : m_rootNode;
    if (!node || !node->isDirectory || node->dirLoadState != BvfsNode::NotLoaded) {
        return;
    }

    BVFS_DEBUG << "fetchMore(" << node->fullPath << ") — enqueuing bvfs_lsdirs";

    node->dirLoadState = BvfsNode::Loading;
    BVFS_DEBUG << "Node " << node->fullPath << ": dirLoadState NotLoaded -> Loading";

    QString args = QString("jobid=%1 pathid=%2")
                       .arg(m_bvfsJobIds)
                       .arg(node->pathId);

    enqueueCommand({BDirector::Command::BvfsLsDirs, args, node});
}

bool BBvfsModel::hasChildren(const QModelIndex &parent) const
{
    if (!m_rootNode) {
        return false;
    }

    BvfsNode *node = parent.isValid() ? nodeFromIndex(parent) : m_rootNode;
    if (!node) {
        return false;
    }

    // Directories always report having children (lazy loading)
    if (node->isDirectory) {
        return true;
    }

    return false;
}

// ============================================================================
// File loading
// ============================================================================

void BBvfsModel::loadFilesForDirectory(const QModelIndex &dirIndex)
{
    if (!m_rootNode || !m_director || !dirIndex.isValid()) {
        return;
    }

    BvfsNode *node = nodeFromIndex(dirIndex);
    if (!node || !node->isDirectory) {
        return;
    }

    if (node->fileLoadState != BvfsNode::NotLoaded) {
        BVFS_DEBUG << "loadFilesForDirectory(" << node->fullPath
                   << ") — already " << (node->fileLoadState == BvfsNode::Loading ? "loading" : "loaded")
                   << ", skipping";
        return;
    }

    BVFS_DEBUG << "loadFilesForDirectory(" << node->fullPath
               << ") — enqueuing bvfs_lsfiles";

    node->fileLoadState = BvfsNode::Loading;
    BVFS_DEBUG << "Node " << node->fullPath << ": fileLoadState NotLoaded -> Loading";

    QString args = QString("jobid=%1 pathid=%2")
                       .arg(m_bvfsJobIds)
                       .arg(node->pathId);

    enqueueCommand({BDirector::Command::BvfsLsFiles, args, node});
}

// ============================================================================
// Command queue
// ============================================================================

void BBvfsModel::enqueueCommand(const PendingCommand &cmd)
{
    BVFS_DEBUG << "Enqueue: cmd=" << static_cast<int>(cmd.cmd)
               << " args=" << cmd.args;

    m_commandQueue.enqueue(cmd);
    BVFS_DEBUG << "Queue size: " << m_commandQueue.size();

    if (!m_commandPending) {
        processNextCommand();
    }
}

void BBvfsModel::processNextCommand()
{
    if (m_commandQueue.isEmpty()) {
        m_commandPending = false;
        BVFS_DEBUG << "Queue empty, idle";
        emit commandQueueIdle();
        return;
    }

    m_commandPending = true;
    PendingCommand cmd = m_commandQueue.dequeue();
    m_pendingNode = cmd.targetNode;
    m_pendingCmd = cmd.cmd;

    BVFS_DEBUG << "Sending: cmd=" << static_cast<int>(cmd.cmd)
               << " args=" << cmd.args
               << " node=" << (m_pendingNode ? m_pendingNode->fullPath : "null");

    QMetaObject::invokeMethod(m_director, "doSend",
                              Qt::QueuedConnection,
                              Q_ARG(BDirector::Command, cmd.cmd),
                              Q_ARG(QString, cmd.args));
}

// ============================================================================
// Signal handlers
// ============================================================================

void BBvfsModel::onJsonResponse(BDirector::Command cmd, const QString &jsonData)
{
    if (!m_commandPending) return;
    if (cmd != m_pendingCmd) return;  // Not our command

    BVFS_DEBUG << "onJsonResponse: cmd=" << static_cast<int>(cmd)
               << " dataSize=" << jsonData.size();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        BVFS_DEBUG << "JSON parse error: " << error.errorString();
        emit errorOccurred(tr("JSON parse error: %1").arg(error.errorString()));
        processNextCommand();
        return;
    }

    if (!doc.isObject()) {
        BVFS_DEBUG << "Response is not a JSON object";
        processNextCommand();
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    switch (cmd) {
    case BDirector::Command::BvfsUpdate:
        BVFS_DEBUG << "-> Handling bvfs_update JSON response";
        handleBvfsUpdate(QString());
        break;

    case BDirector::Command::BvfsGetJobIds:
        BVFS_DEBUG << "-> Handling bvfs_get_jobids response";
        handleGetJobIds(result);
        break;

    case BDirector::Command::BvfsLsDirs: {
        BVFS_DEBUG << "-> Handling bvfs_lsdirs response for node: "
                   << (m_pendingNode ? m_pendingNode->fullPath : "null");

        QJsonArray dirsArray = result["directories"].toArray();

        // Handle alternative format: result is directly an array
        if (dirsArray.isEmpty() && root.contains("result")) {
            QJsonValue resultVal = root["result"];
            if (resultVal.isArray()) {
                dirsArray = resultVal.toArray();
            }
        }

        BVFS_DEBUG << "Got " << dirsArray.size() << " directory entries";

        if (m_pendingNode) {
            handleLsDirs(m_pendingNode, dirsArray);
        }
        break;
    }

    case BDirector::Command::BvfsLsFiles: {
        BVFS_DEBUG << "-> Handling bvfs_lsfiles response for node: "
                   << (m_pendingNode ? m_pendingNode->fullPath : "null");

        QJsonArray filesArray = result["files"].toArray();
        BVFS_DEBUG << "Got " << filesArray.size() << " file entries";

        if (m_pendingNode) {
            handleLsFiles(m_pendingNode, filesArray);
        }
        break;
    }

    default:
        return;  // Not a BVFS command — don't consume
    }

    processNextCommand();
}

void BBvfsModel::onCommandResponse(BDirector::Command cmd, const QString &response)
{
    if (!m_commandPending) return;
    if (cmd != m_pendingCmd) return;

    // Handle bvfs_update text response
    if (cmd == BDirector::Command::BvfsUpdate) {
        BVFS_DEBUG << "-> Handling bvfs_update response: " << response.trimmed();
        handleBvfsUpdate(response);
        processNextCommand();
        return;
    }
}

// ============================================================================
// Response handlers
// ============================================================================

void BBvfsModel::handleGetJobIds(const QJsonObject &result)
{
    QJsonArray jobidsArray = result["jobids"].toArray();
    QStringList jobIdList;

    for (const QJsonValue &val : jobidsArray) {
        QJsonObject obj = val.toObject();
        QString id = obj["id"].toString();
        if (!id.isEmpty()) {
            jobIdList << id;
        }
    }

    if (jobIdList.isEmpty()) {
        jobIdList << QString::number(m_jobId);
    }

    m_bvfsJobIds = jobIdList.join(",");
    BVFS_DEBUG << "Resolved job IDs: " << m_bvfsJobIds;

    // Now enqueue bvfs_update with resolved IDs
    QString args = QString("jobid=%1").arg(m_bvfsJobIds);
    enqueueCommand({BDirector::Command::BvfsUpdate, args, m_rootNode});
}

void BBvfsModel::handleBvfsUpdate(const QString &response)
{
    Q_UNUSED(response);
    BVFS_DEBUG << "Cache updated, enqueuing root directory listing (pathid=1)";

    // Enqueue bvfs_lsdirs for root
    QString args = QString("jobid=%1 pathid=1").arg(m_bvfsJobIds);
    enqueueCommand({BDirector::Command::BvfsLsDirs, args, m_rootNode});
}

void BBvfsModel::handleLsDirs(BvfsNode *node, const QJsonArray &dirs)
{
    if (!node) return;

    // Filter out "." and "..", collect real directories
    QJsonArray filteredDirs;
    int parentPathId = -1;

    for (const QJsonValue &v : dirs) {
        QJsonObject obj = v.toObject();
        QString name = obj["name"].toString();
        if (name == ".") continue;
        if (name == "..") {
            parentPathId = obj["pathid"].toVariant().toInt();
            continue;
        }
        if (name.isEmpty()) continue;
        filteredDirs.append(v);
    }

    BVFS_DEBUG << "Filtered dirs: " << filteredDirs.size()
               << " (excluded . and .., parentPathId=" << parentPathId << ")";

    // Special case: only . and .. found at root level -> try parent pathid
    if (filteredDirs.isEmpty() && node == m_rootNode && parentPathId > 0) {
        BVFS_DEBUG << "Only . and .. at root, retrying with parent pathid=" << parentPathId;
        node->dirLoadState = BvfsNode::NotLoaded;  // Reset so we can retry
        QString args = QString("jobid=%1 pathid=%2")
                           .arg(m_bvfsJobIds)
                           .arg(parentPathId);
        enqueueCommand({BDirector::Command::BvfsLsDirs, args, node});
        return;
    }

    if (filteredDirs.isEmpty()) {
        BVFS_DEBUG << "No directories found under " << node->fullPath;
        node->dirLoadState = BvfsNode::Loaded;
        if (node == m_rootNode) {
            emit loadingFinished();
        }
        return;
    }

    int insertStart = node->children.size();
    int insertCount = filteredDirs.size();

    BVFS_DEBUG << "Inserting " << insertCount << " directories under "
               << node->fullPath << " at position ["
               << insertStart << ".." << (insertStart + insertCount - 1) << "]";

    beginInsertRows(indexFromNode(node), insertStart, insertStart + insertCount - 1);

    for (const QJsonValue &v : filteredDirs) {
        QJsonObject obj = v.toObject();

        BvfsNode *child = new BvfsNode;
        child->name = obj["name"].toString();
        child->fullPath = obj["fullpath"].toString();
        child->pathId = obj["pathid"].toVariant().toInt();
        child->isDirectory = true;
        child->parentNode = node;

        // Clean up trailing slash from name
        if (child->name.endsWith('/')) {
            child->name.chop(1);
        }

        // Build fullPath if not provided by BVFS
        if (child->fullPath.isEmpty()) {
            QString parentPath = node->fullPath;
            if (!parentPath.endsWith('/')) parentPath += '/';
            child->fullPath = parentPath + child->name;
        }

        BVFS_DEBUG << "Dir node created: " << child->name
                   << " (pathId=" << child->pathId
                   << ", fullPath=" << child->fullPath << ")";

        node->children.append(child);
    }

    endInsertRows();

    node->dirLoadState = BvfsNode::Loaded;
    BVFS_DEBUG << "Node " << node->fullPath << ": dirLoadState -> Loaded";

    if (node == m_rootNode) {
        emit loadingFinished();
    }
}

void BBvfsModel::handleLsFiles(BvfsNode *node, const QJsonArray &files)
{
    if (!node) return;

    // Filter out empty/dot entries
    QJsonArray filteredFiles;
    for (const QJsonValue &v : files) {
        QJsonObject obj = v.toObject();
        QString name = obj["name"].toString();
        if (!name.isEmpty() && name != "." && name != "..") {
            filteredFiles.append(v);
        }
    }

    if (filteredFiles.isEmpty()) {
        BVFS_DEBUG << "No files found under " << node->fullPath;
        node->fileLoadState = BvfsNode::Loaded;
        return;
    }

    int insertStart = node->children.size();
    int insertCount = filteredFiles.size();

    BVFS_DEBUG << "Inserting " << insertCount << " files under "
               << node->fullPath << " at position ["
               << insertStart << ".." << (insertStart + insertCount - 1) << "]";

    beginInsertRows(indexFromNode(node), insertStart, insertStart + insertCount - 1);

    for (const QJsonValue &v : filteredFiles) {
        QJsonObject obj = v.toObject();
        QJsonObject stat = obj["stat"].toObject();

        BvfsNode *child = new BvfsNode;
        child->name = obj["name"].toString();
        child->isDirectory = false;
        child->fileSize = stat["size"].toVariant().toLongLong();
        child->mtime = stat["mtime"].toVariant().toLongLong();
        child->mode = stat["mode"].toInt();
        child->fileId = obj["fileid"].toString();
        child->parentNode = node;

        // Build fullPath
        QString parentPath = node->fullPath;
        if (!parentPath.endsWith('/')) parentPath += '/';
        child->fullPath = parentPath + child->name;

        // Check if it's actually a directory by mode
        if (child->mode > 0 && (child->mode & 0170000) == 0040000) {
            child->isDirectory = true;
            child->fileSize = -1;
        }

        BVFS_DEBUG << "File node created: " << child->name
                   << " (size=" << child->fileSize
                   << ", mtime=" << child->mtime << ")";

        node->children.append(child);
    }

    endInsertRows();

    node->fileLoadState = BvfsNode::Loaded;
    BVFS_DEBUG << "Node " << node->fullPath << ": fileLoadState -> Loaded";
}

// ============================================================================
// Helpers
// ============================================================================

BBvfsModel::BvfsNode *BBvfsModel::nodeFromIndex(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return m_rootNode;
    }
    return static_cast<BvfsNode*>(index.internalPointer());
}

QModelIndex BBvfsModel::indexFromNode(BvfsNode *node, int column) const
{
    if (!node || node == m_rootNode) {
        return QModelIndex();
    }
    return createIndex(node->row(), column, node);
}

// ============================================================================
// Checkbox support
// ============================================================================

void BBvfsModel::setCheckable(bool enabled)
{
    if (m_checkable == enabled) return;
    m_checkable = enabled;
    BVFS_DEBUG << "setCheckable(" << enabled << ")";
    if (m_rootNode) {
        emit dataChanged(index(0, 0), index(rowCount() - 1, 0), {Qt::CheckStateRole});
    }
}

QStringList BBvfsModel::selectedFileIds() const
{
    QStringList fileIds;
    QList<int> dirIds;
    if (m_rootNode) {
        collectSelected(m_rootNode, fileIds, dirIds);
    }
    return fileIds;
}

QList<int> BBvfsModel::selectedDirIds() const
{
    QStringList fileIds;
    QList<int> dirIds;
    if (m_rootNode) {
        collectSelected(m_rootNode, fileIds, dirIds);
    }
    return dirIds;
}

void BBvfsModel::clearSelection()
{
    if (!m_rootNode) return;
    propagateCheckState(m_rootNode, Qt::Unchecked);
    emit dataChanged(index(0, 0), index(rowCount() - 1, 0), {Qt::CheckStateRole});
    emit selectionCountChanged(0);
}

int BBvfsModel::selectedCount() const
{
    return m_rootNode ? countSelected(m_rootNode) : 0;
}

void BBvfsModel::propagateCheckState(BvfsNode *node, Qt::CheckState state)
{
    if (!node) return;
    for (BvfsNode *child : node->children) {
        child->checkState = state;
        // Emit dataChanged for each child's row
        QModelIndex childIdx = indexFromNode(child, ColName);
        if (childIdx.isValid()) {
            emit dataChanged(childIdx, childIdx, {Qt::CheckStateRole});
        }
        if (child->isDirectory) {
            propagateCheckState(child, state);
        }
    }
}

void BBvfsModel::updateParentCheckState(BvfsNode *parentNode)
{
    if (!parentNode || parentNode == m_rootNode) return;

    int checked = 0, unchecked = 0, partial = 0;
    for (BvfsNode *child : parentNode->children) {
        switch (child->checkState) {
        case Qt::Checked: ++checked; break;
        case Qt::Unchecked: ++unchecked; break;
        case Qt::PartiallyChecked: ++partial; break;
        }
    }

    Qt::CheckState newState;
    if (partial > 0 || (checked > 0 && unchecked > 0)) {
        newState = Qt::PartiallyChecked;
    } else if (checked > 0) {
        newState = Qt::Checked;
    } else {
        newState = Qt::Unchecked;
    }

    if (parentNode->checkState != newState) {
        parentNode->checkState = newState;
        QModelIndex parentIdx = indexFromNode(parentNode, ColName);
        if (parentIdx.isValid()) {
            emit dataChanged(parentIdx, parentIdx, {Qt::CheckStateRole});
        }
        // Recurse up
        updateParentCheckState(parentNode->parentNode);
    }
}

void BBvfsModel::collectSelected(BvfsNode *node, QStringList &fileIds, QList<int> &dirIds) const
{
    if (!node) return;

    for (BvfsNode *child : node->children) {
        if (child->checkState == Qt::Checked) {
            if (child->isDirectory) {
                // Whole directory selected — use dirid (includes all contents)
                dirIds.append(child->pathId);
                // Don't recurse into children — dirid covers everything
            } else {
                // Individual file selected
                if (!child->fileId.isEmpty()) {
                    fileIds.append(child->fileId);
                }
            }
        } else if (child->checkState == Qt::PartiallyChecked && child->isDirectory) {
            // Partially checked directory — recurse to find individual selections
            collectSelected(child, fileIds, dirIds);
        }
    }
}

int BBvfsModel::countSelected(BvfsNode *node) const
{
    if (!node) return 0;
    int count = 0;
    for (BvfsNode *child : node->children) {
        if (child->checkState == Qt::Checked) {
            ++count;
        } else if (child->checkState == Qt::PartiallyChecked && child->isDirectory) {
            count += countSelected(child);
        }
    }
    return count;
}

QString BBvfsModel::formatBytes(qint64 bytes) const
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    const qint64 TB = GB * 1024;

    if (bytes >= TB) {
        return QString("%1 TB").arg(bytes / (double)TB, 0, 'f', 2);
    } else if (bytes >= GB) {
        return QString("%1 GB").arg(bytes / (double)GB, 0, 'f', 2);
    } else if (bytes >= MB) {
        return QString("%1 MB").arg(bytes / (double)MB, 0, 'f', 2);
    } else if (bytes >= KB) {
        return QString("%1 KB").arg(bytes / (double)KB, 0, 'f', 2);
    } else {
        return QString("%1 B").arg(bytes);
    }
}
