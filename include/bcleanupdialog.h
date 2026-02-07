#ifndef BCLEANUPDIALOG_H
#define BCLEANUPDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QMap>
#include <QJsonArray>

class BDirector;
class BJobsModel;

class BCleanupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BCleanupDialog(BDirector *director, BJobsModel *jobsModel, QWidget *parent = nullptr);
    ~BCleanupDialog();

signals:
    /**
     * @brief Emitted after cleanup is completed with the list of deleted job IDs
     * @param deletedJobIds List of job IDs that were deleted
     */
    void cleanupCompleted(const QStringList &deletedJobIds);

private slots:
    void onAnalyzeClicked();
    void onCleanupClicked();
    void onCloseClicked();

private:
    void setupUI();
    void analyzeJobs();
    void analyzeOldFullBackups();
    void analyzeEmptyJobs();
    void analyzeFailedJobs();
    void performCleanup();
    void logMessage(const QString &message, const QString &type = "info");
    QString formatBytes(qint64 bytes);

    BDirector *m_director;
    BJobsModel *m_jobsModel;

    // Options widgets
    QCheckBox *m_removeOldFullBackupsCheck;
    QSpinBox *m_keepFullBackupsSpin;
    QCheckBox *m_removeEmptyJobsCheck;
    QCheckBox *m_pruneVolumesCheck;
    QComboBox *m_volumeActionCombo;
    QCheckBox *m_removeFailedJobsCheck;
    QSpinBox *m_failedJobsAgeSpin;

    // Preview/Log area
    QTextEdit *m_logTextEdit;
    QLabel *m_statusLabel;

    // Action buttons
    QPushButton *m_analyzeButton;
    QPushButton *m_cleanupButton;
    QPushButton *m_closeButton;

    // Progress
    QProgressBar *m_progressBar;

    // Analysis results
    struct AnalysisResults {
        int oldFullBackupsCount = 0;
        qint64 oldFullBackupsSize = 0;
        QStringList oldFullBackupsJobIds;

        int emptyJobsCount = 0;
        qint64 emptyJobsSize = 0;
        QStringList emptyJobIds;

        int failedJobsCount = 0;
        QStringList failedJobIds;

        int purgableVolumesCount = 0;
        qint64 reclaimableSpace = 0;
    };

    AnalysisResults m_results;
    bool m_analyzed = false;
};

#endif // BCLEANUPDIALOG_H
