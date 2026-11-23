// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BBVFSMODEL_H
#define BBVFSMODEL_H

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QPointer>
#include <QQueue>
#include <QJsonArray>
#include <QJsonObject>
#include "director/bdirector.h"
#include "jobs/bbvfsreplyrouter.h"

/**
 * @brief Custom QAbstractItemModel for BVFS (Bareos Virtual File System)
 *
 * Handles async BVFS loading with a command queue. Provides data for both
 * tree view (directories only, via BBvfsDirFilterProxy) and list view
 * (all items via setRootIndex).
 *
 * Commands go through a BBvfsReplyRouter, so a reply is taken only for the
 * request this model is waiting for (P0-15). Directory listings are read in
 * pages of PageSize rows until a page comes back short (P0-16): the Director
 * returns at most 2000 rows per listing when no limit is given.
 *
 * Architecture:
 *   BBvfsModel (source) ── tree view uses BBvfsDirFilterProxy (dirs only)
 *                       └─ list view uses BBvfsModel directly with setRootIndex()
 */
class BBvfsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Column { ColName = 0, ColSize, ColType, ColModified, ColCount };
    enum Role {
        PathIdRole = Qt::UserRole + 1,
        FullPathRole,
        IsDirectoryRole,
        FileIdRole,
        SortRole
    };

    /// Rows asked for per .bvfs_lsdirs/.bvfs_lsfiles command (the Bareos WebUI uses the same)
    static constexpr int PageSize = 1000;

    /**
     * @brief Arguments of one page of .bvfs_lsdirs or .bvfs_lsfiles
     * @param jobIds The job chain ("1,2,3")
     * @param pathId The directory; 0 is the root of the catalog's path tree,
     *        sent as "path=" with an empty value (Bareos Bvfs::get_root(),
     *        the path record "" above "/" and "C:/")
     * @param offset Rows to skip
     * @param limit Rows to return at most
     */
    static QString listArgs(const QString &jobIds, int pathId, int offset, int limit = PageSize);

    /**
     * @brief Whether a page of this many rows (including "." and "..") is the last one
     *
     * A full page means there may be more. A page with more rows than asked
     * for comes from a Director that ignores the limit and returned everything.
     */
    static bool isLastPage(int rows, int limit = PageSize);

    /// Sends through the Director's router (created on first use)
    explicit BBvfsModel(BDirector *director, QObject *parent = nullptr);
    /// Sends through the given router (tests use one with a recording send function)
    explicit BBvfsModel(BBvfsReplyRouter *router, QObject *parent = nullptr);
    ~BBvfsModel();

    // Checkbox support (disabled by default — existing uses unaffected)
    void setCheckable(bool enabled);
    bool isCheckable() const { return m_checkable; }
    QStringList selectedFileIds() const;
    QList<int> selectedDirIds() const;
    void clearSelection();
    int selectedCount() const;

    /**
     * @brief Start loading a job's BVFS data
     * @param jobId The job ID to load
     * @param resolveAllRelatedJobs If true, resolve all related job IDs first (full restore chain)
     */
    void loadJob(quint64 jobId, bool resolveAllRelatedJobs = false);

    /**
     * @brief Reset model, clear all nodes and queued commands
     *
     * A request already sent is abandoned: its reply is dropped when it comes.
     */
    void resetModel();

    /// A command is sent or queued; the selection may still change with what arrives
    bool isBusy() const { return m_commandPending || !m_commandQueue.isEmpty(); }

    /// PathId of the root directory as the Director reported it ("." of the root listing), 0 before
    int rootPathId() const { return m_rootPathId; }

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Load files for a directory (called when tree item is clicked)
     *
     * Enqueues bvfs_lsfiles if files haven't been loaded yet.
     * Files are appended as children after directory children.
     */
    void loadFilesForDirectory(const QModelIndex &dirIndex);

    /// Expose resolved BVFS job IDs (available after loadJob completes)
    QString bvfsJobIds() const { return m_bvfsJobIds; }

    /**
     * @brief Total size of the selected files and directories as far as they are loaded
     * @param complete Set to false when a selected directory is not loaded
     *        completely, so the real size is larger
     */
    qint64 computeSelectedSize(bool *complete = nullptr) const;

signals:
    void loadingStarted();
    void loadingFinished();
    void errorOccurred(const QString &message);

    /**
     * @brief Emitted when the command queue becomes empty and idle.
     *
     * Use this to know when it's safe to send other commands to the Director
     * without causing m_lastCommand race conditions.
     */
    void commandQueueIdle();

    /// isBusy() changed
    void busyChanged(bool busy);

    /// Emitted when checkbox selection count changes (only in checkable mode)
    void selectionCountChanged(int count);

private slots:
    void onRouterReply(quint64 ticket, BDirector::Command cmd, const QString &data, bool isJson);
    void onRouterFailed(quint64 ticket, BDirector::Command cmd, const QString &reason);

private:
    // Internal tree node
    struct BvfsNode {
        QString name;
        QString fullPath;
        int pathId = 0;
        qint64 fileSize = -1;   // -1 for directories
        qint64 mtime = 0;
        int mode = 0;
        QString fileId;         // From bvfs_lsfiles (for future restore)
        bool isDirectory = true;
        Qt::CheckState checkState = Qt::Unchecked;  // For restore wizard checkboxes
        /// Check state of children that arrive later: a checked directory
        /// stays selected for everything the user did not uncheck himself
        bool childrenChecked = false;

        enum LoadState { NotLoaded, Loading, Loaded };
        LoadState dirLoadState = NotLoaded;
        LoadState fileLoadState = NotLoaded;

        BvfsNode *parentNode = nullptr;
        QVector<BvfsNode*> children;

        ~BvfsNode() { qDeleteAll(children); }
        int row() const {
            if (parentNode) {
                return parentNode->children.indexOf(const_cast<BvfsNode*>(this));
            }
            return 0;
        }
    };

    // Command queue entry (Director handles one command at a time)
    struct PendingCommand {
        BDirector::Command cmd = BDirector::Command::Custom;
        QString args;           ///< Not used for listings: built from node and offset when sent
        BvfsNode *targetNode = nullptr;
        int offset = 0;         ///< Listings: rows to skip
    };

    QQueue<PendingCommand> m_commandQueue;
    bool m_commandPending = false;

    void connectRouter();
    void enqueueCommand(const PendingCommand &cmd);
    void enqueueListing(BDirector::Command cmd, BvfsNode *node, int offset = 0);
    void processNextCommand();
    void setBusy(bool busy);

    // Response handlers
    void handleJsonReply(BDirector::Command cmd, const QString &jsonData);
    void handleTextReply(BDirector::Command cmd, const QString &text);
    void handleGetJobIds(const QStringList &jobIds);
    void handleBvfsUpdate();
    void handleLsDirs(BvfsNode *node, const QJsonArray &dirs, int offset);
    void handleLsFiles(BvfsNode *node, const QJsonArray &files, int offset);
    void finishPendingListing(BDirector::Command cmd, bool retryLater);

    // Helpers
    BvfsNode *nodeFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromNode(BvfsNode *node, int column = 0) const;
    qint64 computeSubtreeSize(BvfsNode *node) const;
    qint64 computeSelectedSizeHelper(BvfsNode *node, bool *complete) const;
    bool subtreeLoaded(BvfsNode *node) const;
    void emitSizeChangedUpward(BvfsNode *node);

    // Checkbox helpers
    void propagateCheckState(BvfsNode *node, Qt::CheckState state);
    void updateParentCheckState(BvfsNode *parentNode);
    void collectSelected(BvfsNode *node, QStringList &fileIds, QList<int> &dirIds) const;
    int countSelected(BvfsNode *node) const;
    void enqueueChildLoading(BvfsNode *node);

    BvfsNode *m_rootNode = nullptr;
    QPointer<BBvfsReplyRouter> m_router;
    QString m_bvfsJobIds;
    quint64 m_jobId = 0;
    int m_rootPathId = 0;
    bool m_busy = false;

    // Track pending command context
    BvfsNode *m_pendingNode = nullptr;
    BDirector::Command m_pendingCmd = BDirector::Command::Custom;
    int m_pendingOffset = 0;
    quint64 m_pendingTicket = 0;    ///< Ticket of the sent request; 0 = none

    // Checkbox mode
    bool m_checkable = false;
};

// ============================================================================
// Directory-only filter proxy for tree view
// ============================================================================

/**
 * @brief Proxy model that only shows directories (filters out files)
 *
 * Used by the tree view to show only the directory structure.
 * The list view uses the source BBvfsModel directly.
 */
class BBvfsDirFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    using QSortFilterProxyModel::QSortFilterProxyModel;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override {
        QModelIndex idx = sourceModel()->index(sourceRow, 0, sourceParent);
        return idx.data(BBvfsModel::IsDirectoryRole).toBool();
    }
};

#endif // BBVFSMODEL_H
