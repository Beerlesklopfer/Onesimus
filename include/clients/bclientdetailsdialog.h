#ifndef BCLIENTDETAILSDIALOG_H
#define BCLIENTDETAILSDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include "bdirector.h"

// Forward declarations
class QTabWidget;
class QTableWidget;
class QLabel;
class QPushButton;

/**
 * @brief Dialog displaying detailed client information
 * @version 1.0
 * @since 2026-01-28
 *
 * Multi-tab dialog showing:
 * - General client information
 * - Connection details
 * - Jobs performed by this client
 * - Statistics
 */
class BClientDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BClientDetailsDialog(const QJsonObject &client, BDirector *director = nullptr, QWidget *parent = nullptr);

private:
    void setupUI();
    void setupInfoTab();
    void setupJobsTab();
    void setupStatisticsTab();
    void loadClientJobs();

private slots:
    void onJobsReceived(const QString &command, const QString &response);

private:
    QJsonObject m_client;
    BDirector *m_director;
    QString m_clientName;

    // UI Components
    QTabWidget *m_tabWidget;

    // Info tab
    QLabel *m_nameLabel;
    QLabel *m_addressLabel;
    QLabel *m_portLabel;
    QLabel *m_versionLabel;
    QLabel *m_osLabel;
    QLabel *m_statusLabel;
    QLabel *m_lastConnLabel;
    QLabel *m_autoprune;
    QLabel *m_fileRetention;
    QLabel *m_jobRetention;

    // Jobs tab
    QTableWidget *m_jobsTable;
    QPushButton *m_refreshJobsButton;

    // Statistics tab
    QLabel *m_totalJobsLabel;
    QLabel *m_successfulJobsLabel;
    QLabel *m_failedJobsLabel;
    QLabel *m_totalBytesLabel;
    QLabel *m_totalFilesLabel;
};

#endif // BCLIENTDETAILSDIALOG_H
