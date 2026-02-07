#ifndef BJOBFILESWIDGET_H
#define BJOBFILESWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include <QTreeView>
#include <QTableView>
#include <QSplitter>
#include <QProgressBar>
#include "director/bdirector.h"
#include "jobs/bbvfsmodel.h"

/**
 * @brief Widget for browsing files from a backup job
 *
 * This widget provides a read-only file browser interface using BVFS
 * (Bareos Virtual File System) to navigate through backed up files.
 *
 * Features:
 * - Windows Explorer-style tree view (dirs only) with file list
 * - Shared BBvfsModel for both tree and list views
 *
 * Can be used standalone (e.g., in context menu action) or embedded in dialogs.
 *
 * @since 2.9
 */
class BJobFilesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BJobFilesWidget(const QJsonObject &job, BDirector *director, QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when initial BVFS directory loading is complete.
     *
     * Other components should wait for this before sending commands
     * to the Director to avoid m_lastCommand race conditions.
     */
    void filesLoaded();

private slots:
    void onTreeItemClicked(const QModelIndex &proxyIndex);
    void onFileListDoubleClicked(const QModelIndex &index);
    void onLoadingStarted();
    void onLoadingFinished();

private:
    void setupUi();

    // UI Components
    QTreeView *m_fileTreeView;
    QTableView *m_fileListView;
    QSplitter *m_filesSplitter;
    QProgressBar *m_loadingProgress;

    // Model
    BBvfsModel *m_bvfsModel;
    BBvfsDirFilterProxy *m_dirProxy;

    // Data
    QJsonObject m_job;
    BDirector *m_director;
    quint64 m_jobId;
    QString m_clientName;
    bool m_initialLoadDone = false;
};

#endif // BJOBFILESWIDGET_H
