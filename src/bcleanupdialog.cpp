#include "bcleanupdialog.h"
#include "director/bdirector.h"
#include "jobs/bjobmodels.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDateTime>
#include <QJsonObject>
#include <QTimer>
#include <algorithm>

BCleanupDialog::BCleanupDialog(BDirector *director, BJobsModel *jobsModel, QWidget *parent)
    : QDialog(parent)
    , m_director(director)
    , m_jobsModel(jobsModel)
{
    setWindowTitle(tr("Database Cleanup"));
    resize(800, 600);

    setupUI();
}

BCleanupDialog::~BCleanupDialog()
{
}

void BCleanupDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Info Label
    QLabel *infoLabel = new QLabel(
        tr("This tool helps maintain your Bareos database by removing old or unnecessary data.\n"
           "⚠️ Warning: Cleanup operations cannot be undone. Review options carefully before proceeding."));
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("QLabel { padding: 10px; background-color: #3a2d1a; border-left: 3px solid #8f6a2d; border-radius: 4px; }");
    mainLayout->addWidget(infoLabel);

    // Options Group
    QGroupBox *optionsGroup = new QGroupBox(tr("Cleanup Options"));
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);

    // Old Full Backups
    QHBoxLayout *fullBackupsLayout = new QHBoxLayout();
    m_removeOldFullBackupsCheck = new QCheckBox(tr("Remove old full backups"));
    m_removeOldFullBackupsCheck->setChecked(false);
    m_removeOldFullBackupsCheck->setToolTip(tr("Delete old full backup jobs if newer full backups exist for each client"));
    fullBackupsLayout->addWidget(m_removeOldFullBackupsCheck);

    QLabel *keepLabel = new QLabel(tr("Keep last:"));
    fullBackupsLayout->addWidget(keepLabel);

    m_keepFullBackupsSpin = new QSpinBox();
    m_keepFullBackupsSpin->setMinimum(1);
    m_keepFullBackupsSpin->setMaximum(100);
    m_keepFullBackupsSpin->setValue(3);
    m_keepFullBackupsSpin->setSuffix(tr(" full backups"));
    m_keepFullBackupsSpin->setToolTip(tr("Number of recent full backups to keep per client"));
    fullBackupsLayout->addWidget(m_keepFullBackupsSpin);
    fullBackupsLayout->addStretch();

    optionsLayout->addLayout(fullBackupsLayout);

    // Empty Jobs
    m_removeEmptyJobsCheck = new QCheckBox(tr("Remove successful jobs with no files"));
    m_removeEmptyJobsCheck->setChecked(false);
    m_removeEmptyJobsCheck->setToolTip(tr("Delete jobs that completed successfully but backed up 0 files"));
    optionsLayout->addWidget(m_removeEmptyJobsCheck);

    // Prune Volumes
    QHBoxLayout *volumesLayout = new QHBoxLayout();
    m_pruneVolumesCheck = new QCheckBox(tr("Prune volumes"));
    m_pruneVolumesCheck->setChecked(false);
    m_pruneVolumesCheck->setToolTip(tr("Apply retention policy to volumes (removes expired job records)"));
    volumesLayout->addWidget(m_pruneVolumesCheck);

    QLabel *actionLabel = new QLabel(tr("Action:"));
    volumesLayout->addWidget(actionLabel);

    m_volumeActionCombo = new QComboBox();
    m_volumeActionCombo->addItem(tr("Prune only"), "prune");
    m_volumeActionCombo->addItem(tr("Prune and purge"), "purge");
    m_volumeActionCombo->setToolTip(tr("Prune removes expired jobs from volumes, Purge also removes volume from catalog"));
    volumesLayout->addWidget(m_volumeActionCombo);
    volumesLayout->addStretch();

    optionsLayout->addLayout(volumesLayout);

    // Failed Jobs
    QHBoxLayout *failedJobsLayout = new QHBoxLayout();
    m_removeFailedJobsCheck = new QCheckBox(tr("Remove failed jobs older than"));
    m_removeFailedJobsCheck->setChecked(false);
    m_removeFailedJobsCheck->setToolTip(tr("Delete jobs that failed or were canceled"));
    failedJobsLayout->addWidget(m_removeFailedJobsCheck);

    m_failedJobsAgeSpin = new QSpinBox();
    m_failedJobsAgeSpin->setMinimum(1);
    m_failedJobsAgeSpin->setMaximum(365);
    m_failedJobsAgeSpin->setValue(30);
    m_failedJobsAgeSpin->setSuffix(tr(" days"));
    failedJobsLayout->addWidget(m_failedJobsAgeSpin);
    failedJobsLayout->addStretch();

    optionsLayout->addLayout(failedJobsLayout);

    mainLayout->addWidget(optionsGroup);

    // Status Label
    m_statusLabel = new QLabel(tr("Select cleanup options and click 'Analyze' to preview"));
    m_statusLabel->setStyleSheet("QLabel { padding: 5px; background-color: #e8f4f8; border-radius: 3px; }");
    mainLayout->addWidget(m_statusLabel);

    // Log/Preview Area
    QGroupBox *logGroup = new QGroupBox(tr("Analysis / Log"));
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);

    m_logTextEdit = new QTextEdit();
    m_logTextEdit->setReadOnly(true);
    m_logTextEdit->setFont(QFont("Monospace", 9));
    logLayout->addWidget(m_logTextEdit);

    mainLayout->addWidget(logGroup);

    // Progress Bar
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    mainLayout->addWidget(m_progressBar);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    m_analyzeButton = new QPushButton(tr("Analyze"));
    m_analyzeButton->setToolTip(tr("Preview what would be cleaned up"));
    connect(m_analyzeButton, &QPushButton::clicked, this, &BCleanupDialog::onAnalyzeClicked);
    buttonLayout->addWidget(m_analyzeButton);

    m_cleanupButton = new QPushButton(tr("Cleanup"));
    m_cleanupButton->setEnabled(false);
    m_cleanupButton->setToolTip(tr("Perform cleanup operations (requires analysis first)"));
    connect(m_cleanupButton, &QPushButton::clicked, this, &BCleanupDialog::onCleanupClicked);
    buttonLayout->addWidget(m_cleanupButton);

    buttonLayout->addStretch();

    m_closeButton = new QPushButton(tr("Cancel"));
    connect(m_closeButton, &QPushButton::clicked, this, &BCleanupDialog::onCloseClicked);
    buttonLayout->addWidget(m_closeButton);

    mainLayout->addLayout(buttonLayout);
}

void BCleanupDialog::onAnalyzeClicked()
{
    if (!m_director || !m_director->isConnected()) {
        QMessageBox::warning(this, tr("Not Connected"),
                           tr("Please connect to Bareos Director first."));
        return;
    }

    // Check if at least one option is selected
    if (!m_removeOldFullBackupsCheck->isChecked() &&
        !m_removeEmptyJobsCheck->isChecked() &&
        !m_pruneVolumesCheck->isChecked() &&
        !m_removeFailedJobsCheck->isChecked()) {
        QMessageBox::information(this, tr("No Options Selected"),
                               tr("Please select at least one cleanup option."));
        return;
    }

    m_logTextEdit->clear();
    m_analyzed = false;
    m_cleanupButton->setEnabled(false);

    logMessage(tr("=== Analysis ==="), "info");
    analyzeJobs();
}

void BCleanupDialog::onCleanupClicked()
{
    if (!m_analyzed) {
        QMessageBox::warning(this, tr("Analysis Required"),
                           tr("Please run analysis before cleanup."));
        return;
    }

    // Show confirmation dialog
    QString message = tr("⚠️ This will permanently delete data from the Bareos catalog.\n\n");
    message += tr("Operations to perform:\n");

    if (m_removeOldFullBackupsCheck->isChecked()) {
        message += tr("- Delete old full backup jobs (keep %1 per client)\n")
                   .arg(m_keepFullBackupsSpin->value());
    }

    if (m_removeEmptyJobsCheck->isChecked()) {
        message += tr("- Delete empty jobs (successful with 0 files)\n");
    }

    if (m_removeFailedJobsCheck->isChecked()) {
        message += tr("- Delete failed jobs older than %1 days\n")
                   .arg(m_failedJobsAgeSpin->value());
    }

    if (m_pruneVolumesCheck->isChecked()) {
        QString action = m_volumeActionCombo->currentText();
        message += tr("- %1 volumes\n").arg(action);
    }

    message += tr("\nDo you want to continue?");

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Confirm Cleanup"), message,
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        performCleanup();
    }
}

void BCleanupDialog::onCloseClicked()
{
    accept();
}

void BCleanupDialog::analyzeJobs()
{
    m_results = AnalysisResults();

    logMessage(tr("Analyzing cleanup options..."), "info");
    logMessage("", "info");

    // Check if we have job data
    if (!m_jobsModel || m_jobsModel->allJobs().isEmpty()) {
        logMessage(tr("⚠ No job data available!"), "error");
        logMessage(tr("Please load the job list first (click 'Refresh' in the main window)."), "warning");
        logMessage("", "info");
        m_statusLabel->setText(tr("No job data. Please refresh job list first."));
        m_statusLabel->setStyleSheet("QLabel { padding: 5px; background-color: #f8d7da; border-radius: 3px; color: #721c24; }");
        return;
    }

    logMessage(tr("Found %1 jobs in database.").arg(m_jobsModel->allJobs().size()), "info");
    logMessage("", "info");

    // Analyze old full backups
    if (m_removeOldFullBackupsCheck->isChecked()) {
        analyzeOldFullBackups();
    }

    // Analyze empty jobs
    if (m_removeEmptyJobsCheck->isChecked()) {
        analyzeEmptyJobs();
    }

    // Analyze failed jobs
    if (m_removeFailedJobsCheck->isChecked()) {
        analyzeFailedJobs();
    }

    // Analyze volumes (no job model needed)
    if (m_pruneVolumesCheck->isChecked()) {
        QString action = m_volumeActionCombo->currentData().toString();
        if (action == "prune") {
            logMessage(tr("✓ Will prune all volumes (apply retention policy)"), "info");
            logMessage(tr("  Command: prune volume allpools yes"), "info");
        } else {
            logMessage(tr("✓ Will prune and purge all volumes"), "info");
            logMessage(tr("  Command: prune volume allpools yes + purge volume allpools yes"), "info");
        }
    }

    // Summary
    logMessage("", "info");
    logMessage(tr("=== Analysis Summary ==="), "success");

    int totalJobsToDelete = m_results.oldFullBackupsCount + m_results.emptyJobsCount + m_results.failedJobsCount;
    if (totalJobsToDelete > 0) {
        logMessage(tr("Total jobs to delete: %1").arg(totalJobsToDelete), "info");
        if (m_results.oldFullBackupsCount > 0) {
            logMessage(tr("  - Old full backups: %1 (%2)")
                       .arg(m_results.oldFullBackupsCount)
                       .arg(formatBytes(m_results.oldFullBackupsSize)), "info");
        }
        if (m_results.emptyJobsCount > 0) {
            logMessage(tr("  - Empty jobs: %1").arg(m_results.emptyJobsCount), "info");
        }
        if (m_results.failedJobsCount > 0) {
            logMessage(tr("  - Failed jobs: %1").arg(m_results.failedJobsCount), "info");
        }
    } else if (!m_pruneVolumesCheck->isChecked()) {
        logMessage(tr("No jobs found matching cleanup criteria."), "info");
    }

    m_statusLabel->setText(tr("Analysis complete. Click 'Cleanup' to proceed."));
    m_statusLabel->setStyleSheet("QLabel { padding: 5px; background-color: #d4edda; border-radius: 3px; color: #155724; }");

    m_analyzed = true;
    m_cleanupButton->setEnabled(totalJobsToDelete > 0 || m_pruneVolumesCheck->isChecked());
}

QString BCleanupDialog::formatBytes(qint64 bytes)
{
    if (bytes < 1024) return QString("%1 B").arg(bytes);
    if (bytes < 1024 * 1024) return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    if (bytes < 1024 * 1024 * 1024) return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
    return QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
}

void BCleanupDialog::analyzeOldFullBackups()
{
    int keepCount = m_keepFullBackupsSpin->value();
    logMessage(tr("Analyzing old full backups (keeping %1 most recent per client)...").arg(keepCount), "info");

    if (!m_jobsModel) {
        logMessage(tr("  ⚠ No job data - will use prune command"), "warning");
        logMessage(tr("  Command: prune jobs type=full yes"), "info");
        return;
    }

    // Get all jobs and group full backups by client
    QJsonArray allJobs = m_jobsModel->allJobs();
    QMap<QString, QList<QJsonObject>> fullBackupsByClient;

    for (const QJsonValue &val : allJobs) {
        QJsonObject job = val.toObject();
        QString level = job["level"].toString();
        QString status = job["jobstatus"].toString();

        // Only consider successful full backups (level "F" and status "T")
        if (level == "F" && status == "T") {
            QString client = job["client"].toString();
            fullBackupsByClient[client].append(job);
        }
    }

    // Sort each client's backups by date (newest first) and mark old ones for deletion
    for (auto it = fullBackupsByClient.begin(); it != fullBackupsByClient.end(); ++it) {
        QString client = it.key();
        QList<QJsonObject> &jobs = it.value();

        // Sort by starttime descending (newest first)
        std::sort(jobs.begin(), jobs.end(), [](const QJsonObject &a, const QJsonObject &b) {
            return a["starttime"].toString() > b["starttime"].toString();
        });

        // Mark jobs beyond keepCount for deletion
        if (jobs.size() > keepCount) {
            int toDelete = jobs.size() - keepCount;
            logMessage(tr("  Client '%1': %2 full backups, will delete %3")
                       .arg(client)
                       .arg(jobs.size())
                       .arg(toDelete), "info");

            for (int i = keepCount; i < jobs.size(); ++i) {
                QString jobId = jobs[i]["jobid"].toString();
                QString name = jobs[i]["name"].toString();
                QString startTime = jobs[i]["starttime"].toString();
                qint64 bytes = jobs[i]["jobbytes"].toVariant().toLongLong();

                m_results.oldFullBackupsJobIds.append(jobId);
                m_results.oldFullBackupsCount++;
                m_results.oldFullBackupsSize += bytes;

                // Show preview of affected jobs (max 5 per client)
                if (i - keepCount < 5) {
                    logMessage(tr("    → Job %1: %2 (%3, %4)")
                               .arg(jobId)
                               .arg(name)
                               .arg(startTime)
                               .arg(formatBytes(bytes)), "warning");
                } else if (i - keepCount == 5) {
                    logMessage(tr("    ... and %1 more").arg(toDelete - 5), "warning");
                }
            }
        }
    }

    if (m_results.oldFullBackupsCount > 0) {
        logMessage(tr("✓ Found %1 old full backups to delete (%2)")
                   .arg(m_results.oldFullBackupsCount)
                   .arg(formatBytes(m_results.oldFullBackupsSize)), "success");
    } else {
        logMessage(tr("  No old full backups found to delete"), "info");
    }
}

void BCleanupDialog::analyzeEmptyJobs()
{
    logMessage(tr("Analyzing empty jobs (successful with 0 files)..."), "info");

    if (!m_jobsModel) {
        logMessage(tr("  ⚠ No job data available"), "warning");
        return;
    }

    QJsonArray allJobs = m_jobsModel->allJobs();
    int previewCount = 0;
    const int maxPreview = 10;

    for (const QJsonValue &val : allJobs) {
        QJsonObject job = val.toObject();
        QString status = job["jobstatus"].toString();
        qint64 files = job["jobfiles"].toVariant().toLongLong();

        // Successful jobs with 0 files
        if (status == "T" && files == 0) {
            QString jobId = job["jobid"].toString();
            QString name = job["name"].toString();
            QString client = job["client"].toString();
            QString startTime = job["starttime"].toString();
            qint64 bytes = job["jobbytes"].toVariant().toLongLong();

            m_results.emptyJobIds.append(jobId);
            m_results.emptyJobsCount++;
            m_results.emptyJobsSize += bytes;

            // Show preview
            if (previewCount < maxPreview) {
                logMessage(tr("  → Job %1: %2 [%3] (%4)")
                           .arg(jobId)
                           .arg(name)
                           .arg(client)
                           .arg(startTime), "warning");
                previewCount++;
            } else if (previewCount == maxPreview) {
                previewCount++; // Increment to only show "... and X more" once
            }
        }
    }

    if (m_results.emptyJobsCount > maxPreview) {
        logMessage(tr("  ... and %1 more").arg(m_results.emptyJobsCount - maxPreview), "warning");
    }

    if (m_results.emptyJobsCount > 0) {
        logMessage(tr("✓ Found %1 empty jobs to delete").arg(m_results.emptyJobsCount), "success");
    } else {
        logMessage(tr("  No empty jobs found"), "info");
    }
}

void BCleanupDialog::analyzeFailedJobs()
{
    int ageDays = m_failedJobsAgeSpin->value();
    QDateTime cutoffDate = QDateTime::currentDateTime().addDays(-ageDays);

    logMessage(tr("Analyzing failed jobs older than %1 days (before %2)...")
               .arg(ageDays)
               .arg(cutoffDate.toString("yyyy-MM-dd")), "info");

    if (!m_jobsModel) {
        logMessage(tr("  ⚠ No job data available"), "warning");
        return;
    }

    QJsonArray allJobs = m_jobsModel->allJobs();
    int previewCount = 0;
    const int maxPreview = 10;

    for (const QJsonValue &val : allJobs) {
        QJsonObject job = val.toObject();
        QString status = job["jobstatus"].toString();

        // Failed, Error, or Canceled jobs
        if (status == "f" || status == "E" || status == "A" || status == "e") {
            QString startTimeStr = job["starttime"].toString();
            QDateTime startTime = QDateTime::fromString(startTimeStr, Qt::ISODate);
            if (!startTime.isValid()) {
                startTime = QDateTime::fromString(startTimeStr, "yyyy-MM-dd HH:mm:ss");
            }

            if (startTime.isValid() && startTime < cutoffDate) {
                QString jobId = job["jobid"].toString();
                QString name = job["name"].toString();
                QString client = job["client"].toString();

                m_results.failedJobIds.append(jobId);
                m_results.failedJobsCount++;

                // Show preview
                if (previewCount < maxPreview) {
                    QString statusText;
                    if (status == "F") statusText = tr("Failed");
                    else if (status == "E") statusText = tr("Error");
                    else if (status == "A") statusText = tr("Canceled");
                    else statusText = tr("Non-fatal error");

                    logMessage(tr("  → Job %1: %2 [%3] (%4) - %5")
                               .arg(jobId)
                               .arg(name)
                               .arg(client)
                               .arg(startTimeStr)
                               .arg(statusText), "warning");
                    previewCount++;
                } else if (previewCount == maxPreview) {
                    previewCount++;
                }
            }
        }
    }

    if (m_results.failedJobsCount > maxPreview) {
        logMessage(tr("  ... and %1 more").arg(m_results.failedJobsCount - maxPreview), "warning");
    }

    if (m_results.failedJobsCount > 0) {
        logMessage(tr("✓ Found %1 failed jobs to delete").arg(m_results.failedJobsCount), "success");
    } else {
        logMessage(tr("  No failed jobs found matching criteria"), "info");
    }
}

void BCleanupDialog::performCleanup()
{
    logMessage("", "info");
    logMessage(tr("=== Starting Cleanup ==="), "info");

    // Calculate total jobs for progress bar
    int totalJobs = m_results.oldFullBackupsJobIds.size()
                  + m_results.emptyJobIds.size()
                  + m_results.failedJobIds.size();
    int volumeSteps = m_pruneVolumesCheck->isChecked() ? 1 : 0;
    int totalSteps = totalJobs + volumeSteps;

    m_progressBar->setVisible(true);
    m_progressBar->setMaximum(totalSteps > 0 ? totalSteps : 1);
    m_progressBar->setValue(0);

    int progress = 0;

    // Delete old full backups by specific job IDs
    if (m_removeOldFullBackupsCheck->isChecked() && !m_results.oldFullBackupsJobIds.isEmpty()) {
        logMessage(tr("Deleting %1 old full backup jobs...").arg(m_results.oldFullBackupsJobIds.size()), "info");

        for (const QString &jobId : m_results.oldFullBackupsJobIds) {
            QString command = QString("delete job jobid=%1 yes").arg(jobId);
            m_director->sendCommand(command);
            progress++;
            m_progressBar->setValue(progress);
        }
        logMessage(tr("  ✓ Sent delete commands for %1 jobs").arg(m_results.oldFullBackupsJobIds.size()), "success");
    }

    // Delete empty jobs by specific job IDs
    if (m_removeEmptyJobsCheck->isChecked() && !m_results.emptyJobIds.isEmpty()) {
        logMessage(tr("Deleting %1 empty jobs...").arg(m_results.emptyJobIds.size()), "info");

        for (const QString &jobId : m_results.emptyJobIds) {
            QString command = QString("delete job jobid=%1 yes").arg(jobId);
            m_director->sendCommand(command);
            progress++;
            m_progressBar->setValue(progress);
        }
        logMessage(tr("  ✓ Sent delete commands for %1 jobs").arg(m_results.emptyJobIds.size()), "success");
    }

    // Delete failed jobs by specific job IDs
    if (m_removeFailedJobsCheck->isChecked() && !m_results.failedJobIds.isEmpty()) {
        logMessage(tr("Deleting %1 failed jobs...").arg(m_results.failedJobIds.size()), "info");

        for (const QString &jobId : m_results.failedJobIds) {
            QString command = QString("delete job jobid=%1 yes").arg(jobId);
            m_director->sendCommand(command);
            progress++;
            m_progressBar->setValue(progress);
        }
        logMessage(tr("  ✓ Sent delete commands for %1 jobs").arg(m_results.failedJobIds.size()), "success");
    }

    // Prune/purge volumes
    if (m_pruneVolumesCheck->isChecked()) {
        QString action = m_volumeActionCombo->currentData().toString();
        logMessage(tr("Processing volumes..."), "info");

        if (action == "prune") {
            QString command = "prune volume allpools yes";
            m_director->sendCommand(command);
            logMessage(tr("  Sent: %1").arg(command), "success");
        } else if (action == "purge") {
            QString command1 = "prune volume allpools yes";
            QString command2 = "purge volume allpools yes";
            m_director->sendCommand(command1);
            m_director->sendCommand(command2);
            logMessage(tr("  Sent: %1").arg(command1), "success");
            logMessage(tr("  Sent: %1").arg(command2), "success");
        }

        progress++;
        m_progressBar->setValue(progress);
    }

    m_progressBar->setValue(m_progressBar->maximum());

    logMessage("", "info");
    logMessage(tr("=== Cleanup Commands Sent ==="), "success");
    logMessage(tr("Database cleanup operations have been initiated."), "success");

    m_analyzed = false;
    m_cleanupButton->setEnabled(false);

    // Collect all deleted job IDs
    QStringList deletedJobIds;
    deletedJobIds << m_results.oldFullBackupsJobIds
                  << m_results.emptyJobIds
                  << m_results.failedJobIds;

    // Emit signal with deleted job IDs to update model directly
    emit cleanupCompleted(deletedJobIds);

    // Close dialog after short delay to allow model update to propagate to view
    QTimer::singleShot(100, this, &QDialog::accept);
}

void BCleanupDialog::logMessage(const QString &message, const QString &type)
{
    QString color = "#000000";

    if (type == "error") {
        color = "#d32f2f";
    } else if (type == "success") {
        color = "#388e3c";
    } else if (type == "warning") {
        color = "#f57c00";
    } else if (type == "info") {
        color = "#1976d2";
    }

    QString html = QString("<span style='color: %1;'>%2</span><br>")
                   .arg(color)
                   .arg(message.toHtmlEscaped());

    m_logTextEdit->insertHtml(html);
    m_logTextEdit->ensureCursorVisible();
}
