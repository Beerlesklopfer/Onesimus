#ifndef BJOBDETAILSDIALOG_H
#define BJOBDETAILSDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QTabWidget>
#include <QTreeView>
#include <QTableView>
#include <QListView>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTextEdit>
#include <QProgressBar>
#include "bdirector.h"
#include "jobs/bjobmodels.h"

// Forward declarations
class BJobWidget;

/**
 * @brief Dialog to display detailed information about a backup job
 *
 * Features two tabs:
 * - Files: Windows Explorer-style tree view with file list
 * - Status: Job information, statistics, and logs
 *
 * @note Now uses shared log model from BJobWidget to avoid duplicate data loading
 * @since 2.9
 */
class BJobDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BJobDetailsDialog(const QJsonObject &job, BJobWidget *jobWidget, BDirector *director, QWidget *parent = nullptr);

private slots:
    void onTreeItemClicked(const QModelIndex &index);
    void onFilesDataReceived(const QString &command, const QString &jsonData);

private:
    void setupUi(const QJsonObject &job);
    void setupFilesTab();
    void setupStatusTab(const QJsonObject &job);
    void setupLogTab();
    void loadFilesFromDirector();
    void populateFileTree(const QJsonArray &filesArray);

    QString formatBytes(qint64 bytes) const;
    QString formatStatus(const QString &status) const;
    QString getStatusColor(const QString &status) const;

    // UI Components
    QTabWidget *m_tabWidget;

    // Files Tab
    QTreeView *m_fileTreeView;
    QTableView *m_fileListView;
    QStandardItemModel *m_treeModel;
    QStandardItemModel *m_fileListModel;
    QSplitter *m_filesSplitter;
    QProgressBar *m_loadingProgress;

    // Status Tab
    QWidget *m_statusWidget;

    // Log Tab
    QListView *m_logListView;
    // Note: Log model is now shared from BJobWidget (m_jobWidget->logModel())

    // Data
    QJsonObject m_job;
    BJobWidget *m_jobWidget;    // Access to shared log model
    BDirector *m_director;
    quint64 m_jobId;
    QString m_bvfsJobIds;  // Comma-separated list of jobids from bvfs_get_jobids
};

#endif // BJOBDETAILSDIALOG_H
