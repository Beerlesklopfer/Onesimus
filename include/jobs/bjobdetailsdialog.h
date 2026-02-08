#ifndef BJOBDETAILSDIALOG_H
#define BJOBDETAILSDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QTabWidget>
#include <QListView>
#include "director/bdirector.h"
#include "jobs/bjobmodels.h"

// Forward declarations
class BJobWidget;
class BJobFilesWidget;

/**
 * @brief Dialog to display detailed information about a backup job
 *
 * Features three tabs:
 * - Files: Windows Explorer-style tree view with file list (using BJobFilesWidget)
 * - Status: Job information, statistics
 * - Logs: Job log entries
 *
 * @since 2.9
 */
class BJobDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BJobDetailsDialog(const QJsonObject &job, BJobWidget *jobWidget, BDirector *director, QWidget *parent = nullptr);

private slots:
    void onJobLogReceived(BDirector::Command cmd, const QString &response);

private:
    void setupUi(const QJsonObject &job);
    void setupFilesTab();
    void setupStatusTab(const QJsonObject &job);
    void setupLogTab();

    QString formatBytes(qint64 bytes) const;
    QString formatStatus(const QString &status) const;
    QString getStatusColor(const QString &status) const;

    // UI Components
    QTabWidget *m_tabWidget;

    // Files Tab - now uses BJobFilesWidget
    BJobFilesWidget *m_filesWidget;

    // Status Tab
    QWidget *m_statusWidget;

    // Log Tab
    QListView *m_logListView;
    BJobLogModel *m_logModel;

    // Data
    QJsonObject m_job;
    BJobWidget *m_jobWidget;
    BDirector *m_director;
    quint64 m_jobId;
    QString m_clientName;
};

#endif // BJOBDETAILSDIALOG_H
