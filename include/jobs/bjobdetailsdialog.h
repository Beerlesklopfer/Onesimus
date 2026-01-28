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

/**
 * @brief Dialog to display detailed information about a backup job
 *
 * Features two tabs:
 * - Files: Windows Explorer-style tree view with file list
 * - Status: Job information, statistics, and logs
 */
class BJobDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BJobDetailsDialog(const QJsonObject &job, BDirector *director, QWidget *parent = nullptr);

private slots:
    void onTreeItemClicked(const QModelIndex &index);
    void onFilesDataReceived(const QString &command, const QString &jsonData);
    void onJobLogReceived(const QString &command, const QString &response);

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
    QStandardItemModel *m_logModel;

    // Data
    QJsonObject m_job;
    BDirector *m_director;
    quint64 m_jobId;
};

#endif // BJOBDETAILSDIALOG_H
