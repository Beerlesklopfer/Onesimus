#ifndef BBVFSMODEL_H
#define BBVFSMODEL_H

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QQueue>
#include <QJsonArray>
#include <QJsonObject>
#include "director/bdirector.h"

/**
 * @brief Custom QAbstractItemModel for BVFS (Bareos Virtual File System)
 *
 * Handles async BVFS loading with a command queue. Provides data for both
 * tree view (directories only, via BBvfsDirFilterProxy) and list view
 * (all items via setRootIndex).
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
        FileIdRole
    };

    explicit BBvfsModel(BDirector *director, QObject *parent = nullptr);
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
     */
    void resetModel();

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

    /// Emitted when checkbox selection count changes (only in checkable mode)
    void selectionCountChanged(int count);

private slots:
    void onJsonResponse(const QString &command, const QString &jsonData);
    void onCommandResponse(const QString &command, const QString &response);

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
        QString command;
        BvfsNode *targetNode = nullptr;
        enum Type { GetJobIds, Update, ListDirs, ListFiles };
        Type type;
    };

    QQueue<PendingCommand> m_commandQueue;
    bool m_commandPending = false;

    void enqueueCommand(const PendingCommand &cmd);
    void processNextCommand();

    // Response handlers
    void handleGetJobIds(const QJsonObject &result);
    void handleBvfsUpdate(const QString &response);
    void handleLsDirs(BvfsNode *node, const QJsonArray &dirs);
    void handleLsFiles(BvfsNode *node, const QJsonArray &files);

    // Helpers
    BvfsNode *nodeFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromNode(BvfsNode *node, int column = 0) const;
    QString formatBytes(qint64 bytes) const;

    // Checkbox helpers
    void propagateCheckState(BvfsNode *node, Qt::CheckState state);
    void updateParentCheckState(BvfsNode *parentNode);
    void collectSelected(BvfsNode *node, QStringList &fileIds, QList<int> &dirIds) const;
    int countSelected(BvfsNode *node) const;

    BvfsNode *m_rootNode = nullptr;
    BDirector *m_director;
    QString m_bvfsJobIds;
    quint64 m_jobId = 0;

    // Track pending command context
    BvfsNode *m_pendingNode = nullptr;
    PendingCommand::Type m_pendingType = PendingCommand::Update;

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
