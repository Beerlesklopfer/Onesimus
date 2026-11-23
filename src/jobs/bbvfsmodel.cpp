// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "jobs/bbvfsmodel.h"
#include "bbyteformat.h"
#include "jobs/bbvfsargs.h"
#include "blogging.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDateTime>
#include <QIcon>
#include <QLocale>
#include <QGuiApplication>
#include <QCursor>
#include <QRegularExpression>

#define BVFS_DEBUG BLOG_DEBUG()

namespace {

/// The message of a JSON-RPC error reply, or an empty string
QString jsonErrorMessage(const QJsonObject &root)
{
    const QJsonObject error = root.value("error").toObject();
    if (error.isEmpty())
        return QString();
    const QJsonArray messages = error.value("data").toObject()
                                    .value("messages").toObject()
                                    .value("error").toArray();
    QStringList parts;
    for (const QJsonValue &v : messages)
        parts << v.toString().trimmed();
    parts.removeAll(QString());
    if (!parts.isEmpty())
        return parts.join("; ");
    const QString message = error.value("message").toString();
    return message.isEmpty() ? QStringLiteral("error") : message;
}

} // namespace

// ============================================================================
// Command arguments
// ============================================================================

QString BBvfsModel::listArgs(const QString &jobIds, int pathId, int offset, int limit)
{
    // "path=" with an empty value is the root (Bareos ua_dotcmds.cc
    // BvfsParseArg/Bvfs::ChDir(""); the WebUI asks for the root the same way).
    // Empty when the job ids are not a list of numbers (BBvfsArgs)
    return BBvfsArgs::listing(jobIds, pathId, offset, limit);
}

bool BBvfsModel::isLastPage(int rows, int limit)
{
    return rows != limit;
}

// ============================================================================
// Constructor / Destructor
// ============================================================================

BBvfsModel::BBvfsModel(BDirector *director, QObject *parent)
    : QAbstractItemModel(parent)
    , m_router(BBvfsReplyRouter::forDirector(director))
{
    BVFS_DEBUG << "Model created";
    connectRouter();
}

BBvfsModel::BBvfsModel(BBvfsReplyRouter *router, QObject *parent)
    : QAbstractItemModel(parent)
    , m_router(router)
{
    connectRouter();
}

BBvfsModel::~BBvfsModel()
{
    BVFS_DEBUG << "Model destroyed";
    if (m_commandPending) {
        QGuiApplication::restoreOverrideCursor();
    }
    delete m_rootNode;
}

void BBvfsModel::connectRouter()
{
    if (!m_router)
        return;
    connect(m_router, &BBvfsReplyRouter::replied, this, &BBvfsModel::onRouterReply);
    connect(m_router, &BBvfsReplyRouter::failed, this, &BBvfsModel::onRouterFailed);
}

// ============================================================================
// Public API
// ============================================================================

void BBvfsModel::loadJob(quint64 jobId, bool resolveAllRelatedJobs)
{
    BVFS_DEBUG << "loadJob: jobId=" << jobId
               << " resolveAll=" << resolveAllRelatedJobs;

    resetModel();

    m_jobId = jobId;

    // Create invisible root node; pathId 0 lists the root by "path="
    beginResetModel();
    m_rootNode = new BvfsNode;
    m_rootNode->name = "";
    m_rootNode->fullPath = "";
    m_rootNode->pathId = 0;
    m_rootNode->isDirectory = true;
    endResetModel();

    emit loadingStarted();

    if (resolveAllRelatedJobs) {
        // Resolve related job IDs via bvfs_get_jobids (same fileset only)
        const QString args = BBvfsArgs::jobIds(QString::number(jobId));
        enqueueCommand({BDirector::Command::BvfsGetJobIds, args, m_rootNode, 0});
    } else {
        m_bvfsJobIds = QString::number(jobId);
        BVFS_DEBUG << "Using single job ID: " << m_bvfsJobIds;

        const QString args = BBvfsArgs::jobIds(m_bvfsJobIds);
        enqueueCommand({BDirector::Command::BvfsUpdate, args, m_rootNode, 0});
    }
}

void BBvfsModel::resetModel()
{
    BVFS_DEBUG << "resetModel()";

    beginResetModel();

    m_commandQueue.clear();
    if (m_commandPending) {
        QGuiApplication::restoreOverrideCursor();
    }
    // The router still assigns the reply of a sent request to its ticket;
    // nobody holds that ticket any more, so the reply is dropped
    m_commandPending = false;
    m_pendingTicket = 0;
    m_pendingNode = nullptr;
    m_bvfsJobIds.clear();
    m_jobId = 0;
    m_rootPathId = 0;

    delete m_rootNode;
    m_rootNode = nullptr;

    endResetModel();
    setBusy(false);
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
    if (newState == Qt::PartiallyChecked) {
        newState = Qt::Checked;  // the user selects or deselects, never "partly"
    }
    node->checkState = newState;

    BVFS_DEBUG << "setData checkState: " << node->name
               << " -> " << (newState == Qt::Checked ? "Checked" : "Unchecked");

    // A checked directory is restored by its directory ID, which covers its
    // whole subtree on the Director (.bvfs_restore dirid=). Nothing below it
    // has to be loaded; children that are loaded later arrive checked.
    if (node->isDirectory) {
        node->childrenChecked = (newState == Qt::Checked);
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
            if (node->isDirectory) {
                if (!node->children.isEmpty()) {
                    qint64 dirSize = computeSubtreeSize(node);
                    return dirSize > 0 ? BByteFormat::format(dirSize) : QString("-");
                }
                return QString("-");
            }
            return BByteFormat::format(node->fileSize);
        case ColType:
            if (node->isDirectory) return tr("Directory");
            if (node->mode > 0 && (node->mode & 0170000) == 0120000) return tr("Symlink");
            return tr("File");
        case ColModified:
            if (node->mtime > 0) {
                return QLocale().toString(
                    QDateTime::fromSecsSinceEpoch(node->mtime),
                    QLocale::ShortFormat);
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

    case SortRole:
        switch (index.column()) {
        case ColName:
            return node->name.toLower();
        case ColSize:
            if (node->isDirectory) {
                return node->children.isEmpty() ? (qint64)-1 : computeSubtreeSize(node);
            }
            return node->fileSize;
        case ColType:
            return node->isDirectory ? 0 : 1;
        case ColModified:
            return node->mtime;
        }
        break;
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

    return node->dirLoadState == BvfsNode::NotLoaded;
}

void BBvfsModel::fetchMore(const QModelIndex &parent)
{
    if (!m_rootNode || !m_router) {
        return;
    }

    BvfsNode *node = parent.isValid() ? nodeFromIndex(parent) : m_rootNode;
    if (!node || !node->isDirectory || node->dirLoadState != BvfsNode::NotLoaded) {
        return;
    }

    BVFS_DEBUG << "fetchMore(" << node->fullPath << ") — enqueuing bvfs_lsdirs";
    node->dirLoadState = BvfsNode::Loading;
    enqueueListing(BDirector::Command::BvfsLsDirs, node);
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
    return node->isDirectory;
}

// ============================================================================
// File loading
// ============================================================================

void BBvfsModel::loadFilesForDirectory(const QModelIndex &dirIndex)
{
    if (!m_rootNode || !m_router || !dirIndex.isValid()) {
        return;
    }

    BvfsNode *node = nodeFromIndex(dirIndex);
    if (!node || !node->isDirectory) {
        return;
    }

    if (node->fileLoadState != BvfsNode::NotLoaded) {
        return;
    }

    BVFS_DEBUG << "loadFilesForDirectory(" << node->fullPath
               << ") — enqueuing bvfs_lsfiles";

    node->fileLoadState = BvfsNode::Loading;
    enqueueListing(BDirector::Command::BvfsLsFiles, node);
}

// ============================================================================
// Command queue
// ============================================================================

void BBvfsModel::enqueueCommand(const PendingCommand &cmd)
{
    m_commandQueue.enqueue(cmd);

    if (!m_commandPending) {
        processNextCommand();
    }
}

void BBvfsModel::enqueueListing(BDirector::Command cmd, BvfsNode *node, int offset)
{
    enqueueCommand({cmd, QString(), node, offset});
}

void BBvfsModel::setBusy(bool busy)
{
    if (m_busy == busy)
        return;
    m_busy = busy;
    emit busyChanged(busy);
}

void BBvfsModel::processNextCommand()
{
    if (m_commandQueue.isEmpty()) {
        if (m_commandPending) {
            QGuiApplication::restoreOverrideCursor();
        }
        m_commandPending = false;
        m_pendingTicket = 0;
        BVFS_DEBUG << "Queue empty, idle";
        setBusy(false);
        emit commandQueueIdle();
        return;
    }

    if (!m_router) {
        m_commandQueue.clear();
        processNextCommand();
        return;
    }

    if (!m_commandPending) {
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    }
    m_commandPending = true;
    setBusy(true);
    PendingCommand cmd = m_commandQueue.dequeue();
    m_pendingNode = cmd.targetNode;
    m_pendingCmd = cmd.cmd;
    m_pendingOffset = cmd.offset;

    QString args = cmd.args;
    if (cmd.cmd == BDirector::Command::BvfsLsDirs || cmd.cmd == BDirector::Command::BvfsLsFiles) {
        const bool root = (cmd.targetNode == m_rootNode);
        if (!root && cmd.targetNode->pathId <= 0) {
            // Without its PathId this would list the root instead
            BVFS_DEBUG << "No PathId for " << cmd.targetNode->fullPath << ", not listed";
            finishPendingListing(cmd.cmd, false);
            processNextCommand();
            return;
        }
        args = listArgs(m_bvfsJobIds, root ? 0 : cmd.targetNode->pathId, cmd.offset);
    }
    if (args.isEmpty()) {
        // A value that is not a number (BBvfsArgs): not sent
        BVFS_DEBUG << "Refused: cmd=" << static_cast<int>(cmd.cmd) << " has no valid arguments";
        finishPendingListing(cmd.cmd, false);
        if (cmd.cmd == BDirector::Command::BvfsGetJobIds || cmd.cmd == BDirector::Command::BvfsUpdate)
            emit loadingFinished();   // the root listing will not follow
        processNextCommand();
        return;
    }

    BVFS_DEBUG << "Sending: cmd=" << static_cast<int>(cmd.cmd)
               << " args=" << args
               << " node=" << (m_pendingNode ? m_pendingNode->fullPath : "null");

    m_pendingTicket = m_router->send(cmd.cmd, args);
}

// ============================================================================
// Signal handlers
// ============================================================================

void BBvfsModel::onRouterReply(quint64 ticket, BDirector::Command cmd, const QString &data, bool isJson)
{
    // The reply to an abandoned request, another model's or another wizard's
    if (!m_commandPending || ticket == 0 || ticket != m_pendingTicket) return;
    m_pendingTicket = 0;

    BVFS_DEBUG << "Reply: cmd=" << static_cast<int>(cmd)
               << " json=" << isJson << " dataSize=" << data.size();

    if (isJson) {
        handleJsonReply(cmd, data);
    } else {
        handleTextReply(cmd, data);
    }

    processNextCommand();
}

void BBvfsModel::onRouterFailed(quint64 ticket, BDirector::Command cmd, const QString &reason)
{
    if (!m_commandPending || ticket == 0 || ticket != m_pendingTicket) return;
    m_pendingTicket = 0;

    BVFS_DEBUG << "Request failed: cmd=" << static_cast<int>(cmd) << " " << reason;
    emit errorOccurred(reason);
    finishPendingListing(cmd, true);
    if (m_pendingNode == m_rootNode
        && (cmd == BDirector::Command::BvfsGetJobIds || cmd == BDirector::Command::BvfsUpdate)) {
        emit loadingFinished();
    }
    processNextCommand();
}

// ============================================================================
// Response handlers
// ============================================================================

void BBvfsModel::handleJsonReply(BDirector::Command cmd, const QString &jsonData)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError || !doc.isObject()) {
        BVFS_DEBUG << "JSON parse error: " << error.errorString();
        emit errorOccurred(tr("JSON parse error: %1").arg(error.errorString()));
        finishPendingListing(cmd, false);
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    const QString errorMessage = jsonErrorMessage(root);
    if (!errorMessage.isEmpty()) {
        emit errorOccurred(errorMessage);
    }

    switch (cmd) {
    case BDirector::Command::BvfsUpdate:
        handleBvfsUpdate();
        break;

    case BDirector::Command::BvfsGetJobIds: {
        QStringList ids;
        for (const QJsonValue &val : result["jobids"].toArray()) {
            // Only numbers go into later commands (BBvfsArgs)
            const QString id = val.toObject()["id"].toVariant().toString();
            if (BBvfsArgs::isNumber(id)) ids << id;
        }
        handleGetJobIds(ids);
        break;
    }

    case BDirector::Command::BvfsLsDirs: {
        if (!errorMessage.isEmpty()) {
            finishPendingListing(cmd, false);
            break;
        }
        QJsonArray dirsArray = result["directories"].toArray();
        // Handle alternative format: result is directly an array
        if (dirsArray.isEmpty() && root["result"].isArray()) {
            dirsArray = root["result"].toArray();
        }
        if (m_pendingNode) {
            handleLsDirs(m_pendingNode, dirsArray, m_pendingOffset);
        }
        break;
    }

    case BDirector::Command::BvfsLsFiles: {
        if (!errorMessage.isEmpty()) {
            finishPendingListing(cmd, false);
            break;
        }
        if (m_pendingNode) {
            handleLsFiles(m_pendingNode, result["files"].toArray(), m_pendingOffset);
        }
        break;
    }

    default:
        break;
    }
}

void BBvfsModel::handleTextReply(BDirector::Command cmd, const QString &text)
{
    switch (cmd) {
    case BDirector::Command::BvfsUpdate:
        BVFS_DEBUG << "-> Handling bvfs_update response: " << text.trimmed();
        handleBvfsUpdate();
        break;

    case BDirector::Command::BvfsGetJobIds: {
        // Without API mode the Director answers with one line "1,2,3"
        static const QRegularExpression idList(QStringLiteral("^\\s*(\\d+(?:\\s*,\\s*\\d+)*)\\s*$"),
                                               QRegularExpression::MultilineOption);
        QStringList ids;
        const QRegularExpressionMatch match = idList.match(text);
        if (match.hasMatch()) {
            for (const QString &id : match.captured(1).split(','))
                ids << id.trimmed();
        }
        handleGetJobIds(ids);
        break;
    }

    default:
        // A listing answered in text is an error message
        if (!text.trimmed().isEmpty())
            emit errorOccurred(text.trimmed());
        finishPendingListing(cmd, false);
        break;
    }
}

void BBvfsModel::finishPendingListing(BDirector::Command cmd, bool retryLater)
{
    BvfsNode *node = m_pendingNode;
    if (!node) return;

    const BvfsNode::LoadState state = retryLater ? BvfsNode::NotLoaded : BvfsNode::Loaded;
    if (cmd == BDirector::Command::BvfsLsDirs) {
        node->dirLoadState = state;
        if (node == m_rootNode) {
            emit loadingFinished();
        }
    } else if (cmd == BDirector::Command::BvfsLsFiles) {
        node->fileLoadState = state;
    }
}

void BBvfsModel::handleGetJobIds(const QStringList &jobIds)
{
    QStringList jobIdList = jobIds;
    if (jobIdList.isEmpty()) {
        jobIdList << QString::number(m_jobId);
    }

    m_bvfsJobIds = jobIdList.join(",");
    BVFS_DEBUG << "Resolved job IDs: " << m_bvfsJobIds;

    // Now enqueue bvfs_update with resolved IDs; empty (not sent) if one is
    // not a number
    const QString args = BBvfsArgs::jobIds(m_bvfsJobIds);
    enqueueCommand({BDirector::Command::BvfsUpdate, args, m_rootNode, 0});
}

void BBvfsModel::handleBvfsUpdate()
{
    if (!m_rootNode) return;
    BVFS_DEBUG << "Cache updated, enqueuing root directory listing";

    m_rootNode->dirLoadState = BvfsNode::Loading;
    enqueueListing(BDirector::Command::BvfsLsDirs, m_rootNode);
}

void BBvfsModel::handleLsDirs(BvfsNode *node, const QJsonArray &dirs, int offset)
{
    if (!node) return;

    // Filter out "." and "..", collect real directories
    QJsonArray filteredDirs;

    for (const QJsonValue &v : dirs) {
        QJsonObject obj = v.toObject();
        QString name = obj["name"].toString();
        if (name == ".") {
            // The directory itself: this is where the root's PathId comes from
            if (node == m_rootNode) {
                m_rootPathId = obj["pathid"].toVariant().toInt();
            }
            continue;
        }
        if (name == ".." || name.isEmpty()) continue;
        filteredDirs.append(v);
    }

    BVFS_DEBUG << "lsdirs page at offset " << offset << ": " << dirs.size()
               << " rows, " << filteredDirs.size() << " directories under " << node->fullPath;

    const bool inheritCheck = m_checkable && node->childrenChecked;

    if (!filteredDirs.isEmpty()) {
        int insertStart = node->children.size();
        int insertCount = filteredDirs.size();

        beginInsertRows(indexFromNode(node), insertStart, insertStart + insertCount - 1);

        for (const QJsonValue &v : filteredDirs) {
            QJsonObject obj = v.toObject();

            BvfsNode *child = new BvfsNode;
            child->name = obj["name"].toString();
            child->fullPath = obj["fullpath"].toString();
            child->pathId = obj["pathid"].toVariant().toInt();
            child->isDirectory = true;
            child->parentNode = node;

            if (inheritCheck) {
                child->checkState = Qt::Checked;
                child->childrenChecked = true;
            }

            // "etc/" -> "etc", "C:/" -> "C:"; the Unix root "/" keeps its name
            if (child->name.size() > 1 && child->name.endsWith('/')) {
                child->name.chop(1);
            }

            // Build fullPath if not provided by BVFS
            if (child->fullPath.isEmpty()) {
                QString parentPath = node->fullPath;
                if (!parentPath.isEmpty() && !parentPath.endsWith('/')) parentPath += '/';
                child->fullPath = parentPath + child->name;
            }

            node->children.append(child);
        }

        endInsertRows();

        if (inheritCheck) {
            updateParentCheckState(node);
            emit selectionCountChanged(selectedCount());
        }
    }

    if (!isLastPage(dirs.size())) {
        // Read the next page before anything else, so the directory is
        // complete as soon as possible
        m_commandQueue.prepend({BDirector::Command::BvfsLsDirs, QString(), node, offset + PageSize});
        return;
    }

    node->dirLoadState = BvfsNode::Loaded;
    if (node == m_rootNode) {
        emit loadingFinished();
    }
}

void BBvfsModel::handleLsFiles(BvfsNode *node, const QJsonArray &files, int offset)
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

    BVFS_DEBUG << "lsfiles page at offset " << offset << ": " << files.size()
               << " rows, " << filteredFiles.size() << " files under " << node->fullPath;

    const bool inheritCheck = m_checkable && node->childrenChecked;

    if (!filteredFiles.isEmpty()) {
        int insertStart = node->children.size();
        int insertCount = filteredFiles.size();

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
            // A file id that is not a number is never put into .bvfs_restore
            const QString fileId = obj["fileid"].toVariant().toString();
            child->fileId = BBvfsArgs::isNumber(fileId) ? fileId : QString();
            child->parentNode = node;

            if (inheritCheck) {
                child->checkState = Qt::Checked;
            }

            // Build fullPath
            QString parentPath = node->fullPath;
            if (!parentPath.endsWith('/')) parentPath += '/';
            child->fullPath = parentPath + child->name;

            // Check if it's actually a directory by mode
            if (child->mode > 0 && (child->mode & 0170000) == 0040000) {
                child->isDirectory = true;
                child->fileSize = -1;
            }

            node->children.append(child);
        }

        endInsertRows();

        // Update directory size display up through parent chain
        emitSizeChangedUpward(node);

        if (inheritCheck) {
            updateParentCheckState(node);
            emit selectionCountChanged(selectedCount());
        }
    }

    if (!isLastPage(files.size())) {
        m_commandQueue.prepend({BDirector::Command::BvfsLsFiles, QString(), node, offset + PageSize});
        return;
    }

    node->fileLoadState = BvfsNode::Loaded;
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
    if (m_rootNode && rowCount() > 0) {
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
    m_rootNode->childrenChecked = false;
    propagateCheckState(m_rootNode, Qt::Unchecked);
    if (rowCount() > 0) {
        emit dataChanged(index(0, 0), index(rowCount() - 1, 0), {Qt::CheckStateRole});
    }
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
        child->childrenChecked = (state == Qt::Checked);
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
        // A partly selected directory is restored entry by entry, so all
        // its direct entries must be known; their own subtrees need not be
        if (newState == Qt::PartiallyChecked) {
            enqueueChildLoading(parentNode);
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

qint64 BBvfsModel::computeSubtreeSize(BvfsNode *node) const
{
    if (!node) return 0;
    if (!node->isDirectory) return qMax(node->fileSize, (qint64)0);

    qint64 total = 0;
    for (BvfsNode *child : node->children) {
        if (child->isDirectory) {
            total += computeSubtreeSize(child);
        } else {
            total += qMax(child->fileSize, (qint64)0);
        }
    }
    return total;
}

bool BBvfsModel::subtreeLoaded(BvfsNode *node) const
{
    if (!node || !node->isDirectory) return true;
    if (node->dirLoadState != BvfsNode::Loaded || node->fileLoadState != BvfsNode::Loaded)
        return false;
    for (BvfsNode *child : node->children) {
        if (child->isDirectory && !subtreeLoaded(child))
            return false;
    }
    return true;
}

qint64 BBvfsModel::computeSelectedSize(bool *complete) const
{
    bool allLoaded = true;
    const qint64 size = m_rootNode ? computeSelectedSizeHelper(m_rootNode, &allLoaded) : 0;
    if (complete) *complete = allLoaded;
    return size;
}

qint64 BBvfsModel::computeSelectedSizeHelper(BvfsNode *node, bool *complete) const
{
    if (!node) return 0;
    qint64 total = 0;
    for (BvfsNode *child : node->children) {
        if (child->checkState == Qt::Checked) {
            if (child->isDirectory) {
                if (!subtreeLoaded(child)) *complete = false;
                total += computeSubtreeSize(child);
            } else {
                total += qMax(child->fileSize, (qint64)0);
            }
        } else if (child->checkState == Qt::PartiallyChecked && child->isDirectory) {
            total += computeSelectedSizeHelper(child, complete);
        }
    }
    return total;
}

void BBvfsModel::emitSizeChangedUpward(BvfsNode *node)
{
    while (node && node != m_rootNode) {
        QModelIndex idx = indexFromNode(node, ColSize);
        if (idx.isValid()) {
            emit dataChanged(idx, idx, {Qt::DisplayRole, SortRole});
        }
        node = node->parentNode;
    }
}

void BBvfsModel::enqueueChildLoading(BvfsNode *node)
{
    if (!node || !node->isDirectory || !m_router) return;

    // Load subdirectories if not yet loaded
    if (node->dirLoadState == BvfsNode::NotLoaded) {
        node->dirLoadState = BvfsNode::Loading;
        enqueueListing(BDirector::Command::BvfsLsDirs, node);
    }

    // Load files if not yet loaded
    if (node->fileLoadState == BvfsNode::NotLoaded) {
        node->fileLoadState = BvfsNode::Loading;
        enqueueListing(BDirector::Command::BvfsLsFiles, node);
    }
}
