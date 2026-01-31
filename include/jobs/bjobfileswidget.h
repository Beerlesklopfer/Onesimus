#ifndef BJOBFILESWIDGET_H
#define BJOBFILESWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QTreeView>
#include <QTableView>
#include <QSplitter>
#include <QStandardItemModel>
#include <QProgressBar>
#include <QCheckBox>
#include <QPushButton>
#include "bdirector.h"

/**
 * @brief Widget for browsing and restoring files from a backup job
 *
 * This widget provides a file browser interface using BVFS (Bareos Virtual File System)
 * to navigate through backed up files and initiate restores.
 *
 * Features:
 * - Windows Explorer-style tree view with file list
 * - Checkbox selection for files and directories
 * - Option to show files from all related jobs (full restore chain)
 * - Restore selected files functionality
 *
 * Can be used standalone (e.g., in context menu action) or embedded in dialogs.
 *
 * @since 2.9
 */
class BJobFilesWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct the files widget
     * @param job The job JSON object containing jobid, client, name, etc.
     * @param director Pointer to the BDirector for BVFS commands
     * @param parent Parent widget
     */
    explicit BJobFilesWidget(const QJsonObject &job, BDirector *director, QWidget *parent = nullptr);

    /**
     * @brief Get the number of selected items (files + directories)
     * @return Count of selected items
     */
    int selectedItemCount() const;

    /**
     * @brief Check if any items are selected for restore
     * @return true if at least one item is selected
     */
    bool hasSelection() const;

signals:
    /**
     * @brief Emitted when the selection changes
     * @param count Number of selected items
     */
    void selectionChanged(int count);

    /**
     * @brief Emitted when a restore operation is initiated
     */
    void restoreRequested();

private slots:
    void onTreeItemClicked(const QModelIndex &index);
    void onTreeItemExpanded(const QModelIndex &index);
    void onTreeItemCheckChanged(QStandardItem *item);
    void onFilesDataReceived(const QString &command, const QString &jsonData);
    void onAllJobsToggled(bool checked);
    void onRestoreClicked();
    void onSelectionChanged();
    void onFileListDoubleClicked(const QModelIndex &index);

private:
    void setupUi();
    void loadFilesFromDirector();
    void populateFileTree(const QJsonArray &filesArray);
    void loadFilesForDirectory(int pathId, const QString &path);
    void populateFileList(const QJsonArray &filesArray);

    QString formatBytes(qint64 bytes) const;

    // Restore helpers
    QStringList getSelectedFileIds() const;
    QStringList getSelectedDirIds() const;
    int countSelectedItems() const;
    void updateRestoreButtonState();

    // UI Components
    QTreeView *m_fileTreeView;
    QTableView *m_fileListView;
    QStandardItemModel *m_treeModel;
    QStandardItemModel *m_fileListModel;
    QSplitter *m_filesSplitter;
    QProgressBar *m_loadingProgress;
    QCheckBox *m_allJobsCheckBox;
    QPushButton *m_restoreButton;

    // Data
    QJsonObject m_job;
    BDirector *m_director;
    quint64 m_jobId;
    QString m_bvfsJobIds;
    QString m_currentPath;
    int m_currentPathId;
    QString m_clientName;

    // BVFS state tracking
    enum class BvfsState {
        Idle,
        GettingJobIds,
        UpdatingCache,
        ListingDirs,
        ListingSubDirs,
        ListingFiles,
        CreatingRestoreTable,
        ExecutingRestore
    };
    BvfsState m_bvfsState = BvfsState::Idle;
    QString m_pendingExpandPath;
    QStandardItem *m_pendingExpandItem = nullptr;
    QString m_restoreTableName;

    // Restore parameters
    QString m_restoreClient;
    QString m_restoreWhere;
    QString m_restoreReplace;
};

#endif // BJOBFILESWIDGET_H
