#ifndef BCLIENTDETAILSDIALOG_H
#define BCLIENTDETAILSDIALOG_H

#include <QAbstractTableModel>
#include <QDialog>
#include <QJsonArray>
#include <QJsonObject>
#include "director/bdirector.h"

// Forward declarations
class QTabWidget;
class QTableView;
class QLabel;
class QPushButton;
class QRadioButton;
class BClientsModel;
class BResourceWidget;

/**
 * @brief Table model for client backup jobs
 * @since 2.9
 */
class BClientJobsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns {
        COL_JOBID = 0,
        COL_NAME,
        COL_START,
        COL_DURATION,
        COL_LEVEL,
        COL_FILES,
        COL_BYTES,
        COL_STATUS,
        COL_COUNT
    };

    struct Statistics {
        int totalJobs = 0;
        int successfulJobs = 0;
        int failedJobs = 0;
        qint64 totalFiles = 0;
        qint64 totalBytes = 0;
    };

    explicit BClientJobsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setJobs(const QJsonArray &jobs);
    Statistics statistics() const { return m_stats; }

private:
    QJsonArray m_jobs;
    Statistics m_stats;
    QString formatBytes(qint64 bytes) const;
};

/**
 * @brief Dialog displaying detailed client information
 * @version 2.0
 * @since 2026-01-28
 *
 * Multi-tab dialog showing:
 * - General client information
 * - Director config syntax
 * - Jobs performed by this client
 * - Statistics
 */
class BClientDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BClientDetailsDialog(BDirector *director, BClientsModel *model, int row, QWidget *parent = nullptr);

    /**
     * @brief Switch to the Settings tab (useful for export from context menu)
     */
    void showSettingsTab();

private:
    void setupUI();
    void setupInfoTab();
    void setupSettingsTab();
    void setupJobsTab();
    void setupStatisticsTab();
    void loadClientJobs();
    void onTlsModeChanged();

private slots:
    void onJobsReceived(BDirector::Command cmd, const QString &response);

private:
    QJsonObject m_client;
    BDirector *m_director;
    BClientsModel *m_model;
    QString m_clientName;
    int m_row;

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
    QTableView *m_jobsTable;
    BClientJobsModel *m_jobsModel;
    QPushButton *m_refreshJobsButton;

    // Settings tab
    QWidget *m_settingsTab;
    BResourceWidget *m_settingsWidget;

    // Statistics tab
    QLabel *m_totalJobsLabel;
    QLabel *m_successfulJobsLabel;
    QLabel *m_failedJobsLabel;
    QLabel *m_totalBytesLabel;
    QLabel *m_totalFilesLabel;
};

#endif // BCLIENTDETAILSDIALOG_H
