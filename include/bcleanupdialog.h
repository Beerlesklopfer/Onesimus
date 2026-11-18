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
#include <QGroupBox>
#include <QStringList>

#include "bcleanupplan.h"

class BDirector;
class BJobsModel;
class BCleanupRunner;
class QTimer;
class QCloseEvent;

/**
 * @brief Catalog cleanup: analyze, confirm, delete
 *
 * What is deleted is decided by BCleanup::buildPlan(); the dialog shows,
 * confirms and executes that one plan. Any change of an option or of the
 * job list invalidates it. The commands are sent one at a time by
 * BCleanupRunner and every answer of the Director is evaluated.
 */
class BCleanupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BCleanupDialog(BDirector *director, BJobsModel *jobsModel, QWidget *parent = nullptr);
    ~BCleanupDialog();

    void reject() override;

signals:
    /**
     * @brief Emitted after a cleanup run with the job IDs the Director confirmed as deleted
     * @param deletedJobIds Job IDs whose deletion was confirmed
     */
    void cleanupCompleted(const QStringList &deletedJobIds);

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onAnalyzeClicked();
    void onCleanupClicked();
    void onCloseClicked();
    void onOptionChanged();
    void onJobDataChanged();
    void onStepStarted(int index);
    void onStepFinished(int index);
    void onRunFinished();
    void onReplyTimeout();

private:
    void setupUI();
    BCleanup::Options currentOptions() const;
    void analyzeJobs();
    void logOldFullBackups(const BCleanup::Plan &plan);
    void logEmptyJobs(const BCleanup::Plan &plan);
    void logFailedJobs(const BCleanup::Plan &plan);
    void invalidateAnalysis(const QString &statusText);
    void updateCleanupButton();
    void performCleanup();
    void setRunning(bool running);
    void setStatus(const QString &text, const QString &kind);
    void logMessage(const QString &message, const QString &type = "info");
    QString formatBytes(qint64 bytes);

    BDirector *m_director;
    BJobsModel *m_jobsModel;

    // Options widgets
    QGroupBox *m_optionsGroup;
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

    // The one analysis result that is shown, confirmed and executed
    BCleanup::Analysis m_analysis;

    BCleanupRunner *m_runner;
    QTimer *m_replyTimer;
};

#endif // BCLEANUPDIALOG_H
