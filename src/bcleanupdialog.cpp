#include "bcleanupdialog.h"
#include "buiscale.h"
#include "bcapabilitynotice.h"
#include "blabelstate.h"
#include "bcleanuprunner.h"
#include "director/bdirector.h"
#include "jobs/bjobmodels.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDateTime>
#include <QJsonObject>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QTimer>

namespace {

// Seconds to wait for the Director's answer to one command
const int DeleteReplyTimeoutSeconds = 120;
const int PruneReplyTimeoutSeconds = 600;

// The command BDirector::Command::PruneVolumeAll sends (see commandToString())
const char *PruneAllVolumesCommand = "prune volume all yes";

} // namespace

BCleanupDialog::BCleanupDialog(BDirector *director, BJobsModel *jobsModel, QWidget *parent)
    : QDialog(parent)
    , m_director(director)
    , m_jobsModel(jobsModel)
    , m_runner(new BCleanupRunner(this))
    , m_replyTimer(new QTimer(this))
{
    setWindowTitle(tr("Database Cleanup"));
    resize(BUiScale::px(800), BUiScale::px(600));

    setupUI();

    m_replyTimer->setSingleShot(true);
    connect(m_replyTimer, &QTimer::timeout, this, &BCleanupDialog::onReplyTimeout);

    connect(m_runner, &BCleanupRunner::stepStarted, this, &BCleanupDialog::onStepStarted);
    connect(m_runner, &BCleanupRunner::stepFinished, this, &BCleanupDialog::onStepFinished);
    connect(m_runner, &BCleanupRunner::finished, this, &BCleanupDialog::onRunFinished);

    m_runner->setSender([this](BCleanupRunner::StepKind kind, const QString &jobId) {
        if (!m_director) {
            return;
        }
        if (kind == BCleanupRunner::StepKind::DeleteJob) {
            m_director->doSend(BDirector::Command::DeleteJob, jobId);
        } else {
            m_director->doSend(BDirector::Command::PruneVolumeAll);
        }
    });

    if (m_director) {
        // Replies are tagged with the command enum only. The runner has at most
        // one command outstanding and accepts a reply only while it waits for
        // one of that kind.
        auto onReply = [this](BDirector::Command cmd, const QString &data) {
            if (cmd == BDirector::Command::DeleteJob) {
                m_runner->handleReply(BCleanupRunner::StepKind::DeleteJob, data);
            } else if (cmd == BDirector::Command::PruneVolumeAll) {
                m_runner->handleReply(BCleanupRunner::StepKind::PruneVolumes, data);
            }
        };
        connect(m_director, &BDirector::jsonResult, this, onReply);
        connect(m_director, &BDirector::textResult, this, onReply);
        connect(m_director, &BDirector::disconnected, this, [this]() {
            m_runner->abort(tr("The connection to the Director was lost"));
        });
    }

    if (m_jobsModel) {
        // The plan was computed from the job list; a changed list makes it stale
        connect(m_jobsModel, &QAbstractItemModel::modelReset, this, &BCleanupDialog::onJobDataChanged);
        connect(m_jobsModel, &QAbstractItemModel::rowsInserted, this, &BCleanupDialog::onJobDataChanged);
        connect(m_jobsModel, &QAbstractItemModel::rowsRemoved, this, &BCleanupDialog::onJobDataChanged);
    }
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
    // Colour from the theme (QLabel[state="warning"]); a background set here
    // would need a matching text colour for each theme
    infoLabel->setStyleSheet("QLabel { padding: 10px; }");
    bSetLabelState(infoLabel, "warning");
    mainLayout->addWidget(infoLabel);

    // Options Group
    m_optionsGroup = new QGroupBox(tr("Cleanup Options"));
    QVBoxLayout *optionsLayout = new QVBoxLayout(m_optionsGroup);

    // Old Full Backups
    QHBoxLayout *fullBackupsLayout = new QHBoxLayout();
    m_removeOldFullBackupsCheck = new QCheckBox(tr("Remove old full backups"));
    m_removeOldFullBackupsCheck->setChecked(false);
    m_removeOldFullBackupsCheck->setToolTip(
        tr("Delete old successful full backups of a job if newer full backups of the same client, "
           "job and FileSet exist. A full backup that incremental or differential backups still "
           "depend on is kept."));
    fullBackupsLayout->addWidget(m_removeOldFullBackupsCheck);

    QLabel *keepLabel = new QLabel(tr("Keep last:"));
    fullBackupsLayout->addWidget(keepLabel);

    m_keepFullBackupsSpin = new QSpinBox();
    m_keepFullBackupsSpin->setMinimum(1);
    m_keepFullBackupsSpin->setMaximum(100);
    m_keepFullBackupsSpin->setValue(3);
    m_keepFullBackupsSpin->setSuffix(tr(" full backups"));
    m_keepFullBackupsSpin->setToolTip(tr("Number of recent full backups to keep per client, job and FileSet"));
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
    m_volumeActionCombo->setToolTip(tr("Prune removes the expired job records of all volumes "
                                       "according to their retention period"));

    // "purge volume allpools yes" is not a documented Bareos command: purge
    // works on named volumes only and ignores retention periods. The entry
    // stays visible but cannot be selected and is never sent.
    if (auto *comboModel = qobject_cast<QStandardItemModel *>(m_volumeActionCombo->model())) {
        if (QStandardItem *purgeItem = comboModel->item(1)) {
            purgeItem->setEnabled(false);
            purgeItem->setToolTip(
                tr("Not available: Bareos has no command that purges all volumes at once. "
                   "Purge works on named volumes only and ignores retention periods, "
                   "so it is not sent from this dialog."));
        }
    }
    m_volumeActionCombo->setCurrentIndex(0);
    // A click on the disabled entry selects nothing; eventFilter() answers it
    // with the reason why the entry is not available
    m_volumeActionCombo->view()->viewport()->installEventFilter(this);
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

    mainLayout->addWidget(m_optionsGroup);

    // Every option change makes an existing analysis stale
    const QList<QCheckBox *> checks = {m_removeOldFullBackupsCheck, m_removeEmptyJobsCheck,
                                       m_pruneVolumesCheck, m_removeFailedJobsCheck};
    for (QCheckBox *check : checks) {
        connect(check, &QCheckBox::toggled, this, &BCleanupDialog::onOptionChanged);
    }
    connect(m_keepFullBackupsSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &BCleanupDialog::onOptionChanged);
    connect(m_failedJobsAgeSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &BCleanupDialog::onOptionChanged);
    connect(m_volumeActionCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BCleanupDialog::onOptionChanged);

    // Status Label
    m_statusLabel = new QLabel(tr("Select cleanup options and click 'Analyze' to preview"));
    m_statusLabel->setStyleSheet("QLabel { padding: 5px; }");
    mainLayout->addWidget(m_statusLabel);

    // Log/Preview Area
    QGroupBox *logGroup = new QGroupBox(tr("Analysis / Log"));
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);

    m_logTextEdit = new QTextEdit();
    m_logTextEdit->setReadOnly(true);
    BUiScale::bind(m_logTextEdit, BUiScale::Normal, BUiScale::Monospace);
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

bool BCleanupDialog::eventFilter(QObject *watched, QEvent *event)
{
    // A click on the disabled "Prune and purge" entry of the action list:
    // the user asks for a function the Director does not have. The entry
    // stays disabled and nothing is sent; the user only gets the reason.
    // (The filter is installed on the list of m_volumeActionCombo only.)
    if (event->type() == QEvent::MouseButtonRelease
        && watched == m_volumeActionCombo->view()->viewport()) {
        const auto *mouse = static_cast<QMouseEvent *>(event);
        const QModelIndex index = m_volumeActionCombo->view()->indexAt(mouse->position().toPoint());
        if (index.isValid() && !(index.flags() & Qt::ItemIsEnabled)
            && index.data(Qt::UserRole).toString() == QLatin1String("purge")) {
            m_volumeActionCombo->hidePopup();
            // After the popup is gone, so that the box is not opened from
            // inside the popup's own event handling
            QTimer::singleShot(0, this, [this]() {
                BCapabilityNotice::check(this, m_director,
                                         BCapabilities::Function::PurgeAllVolumes,
                                         BCapabilities::Request::Explicit);
            });
            return true;
        }
    }
    return QDialog::eventFilter(watched, event);
}

BCleanup::Options BCleanupDialog::currentOptions() const
{
    BCleanup::Options options;
    options.removeOldFulls = m_removeOldFullBackupsCheck->isChecked();
    options.keepFulls = m_keepFullBackupsSpin->value();
    options.removeEmptyJobs = m_removeEmptyJobsCheck->isChecked();
    options.removeFailedJobs = m_removeFailedJobsCheck->isChecked();
    options.failedAgeDays = m_failedJobsAgeSpin->value();
    // Only "Prune only" can be executed; see the disabled purge entry in setupUI()
    options.pruneVolumes = m_pruneVolumesCheck->isChecked()
                           && m_volumeActionCombo->currentData().toString() == "prune";
    return options;
}

void BCleanupDialog::setStatus(const QString &text, const QString &kind)
{
    // The kinds map to the themes' QLabel[state=...] colours; "info" and
    // anything else use the theme's normal text colour
    const char *state = "";
    if (kind == "success") {
        state = "success";
    } else if (kind == "error") {
        state = "error";
    } else if (kind == "warning") {
        state = "warning";
    }
    m_statusLabel->setText(text);
    bSetLabelState(m_statusLabel, state);
}

void BCleanupDialog::updateCleanupButton()
{
    m_cleanupButton->setEnabled(!m_runner->isRunning() && m_analysis.canExecute(currentOptions()));
}

void BCleanupDialog::invalidateAnalysis(const QString &statusText)
{
    const bool hadAnalysis = m_analysis.isValid();
    m_analysis.invalidate();
    updateCleanupButton();

    if (hadAnalysis) {
        logMessage("", "info");
        logMessage(statusText, "warning");
        setStatus(statusText, "warning");
    }
}

void BCleanupDialog::onOptionChanged()
{
    if (m_runner->isRunning()) {
        return;  // Options are locked while a run is in progress
    }
    invalidateAnalysis(tr("The options were changed. The analysis above is no longer valid; "
                          "click 'Analyze' again."));
}

void BCleanupDialog::onJobDataChanged()
{
    if (m_runner->isRunning()) {
        return;
    }
    invalidateAnalysis(tr("The job list has changed. The analysis above is no longer valid; "
                          "click 'Analyze' again."));
}

void BCleanupDialog::onAnalyzeClicked()
{
    if (m_runner->isRunning()) {
        return;
    }

    if (!m_director || !m_director->isConnected()) {
        QMessageBox::warning(this, tr("Not Connected"),
                           tr("Please connect to Bareos Director first."));
        return;
    }

    // Check if at least one option is selected
    if (!currentOptions().anySelected()) {
        QMessageBox::information(this, tr("No Options Selected"),
                               tr("Please select at least one cleanup option."));
        return;
    }

    m_logTextEdit->clear();
    m_analysis.invalidate();
    updateCleanupButton();

    logMessage(tr("=== Analysis ==="), "info");
    analyzeJobs();
}

void BCleanupDialog::onCleanupClicked()
{
    if (m_runner->isRunning()) {
        return;
    }

    // The plan must have been computed for exactly the options shown now
    if (!m_analysis.canExecute(currentOptions())) {
        updateCleanupButton();
        QMessageBox::warning(this, tr("Analysis Required"),
                           tr("Please run analysis before cleanup."));
        return;
    }

    // Everything below is taken from the plan, not from the widgets
    const BCleanup::Plan &plan = m_analysis.plan();

    QString message = tr("⚠️ This will permanently delete data from the Bareos catalog.\n\n");
    message += tr("Operations to perform:\n");

    if (!plan.oldFulls.isEmpty()) {
        message += tr("- Delete %1 old full backup jobs (keeping %2 per client, job and FileSet)\n")
                   .arg(plan.oldFulls.size())
                   .arg(plan.options.keepFulls);
    }

    if (!plan.emptyJobs.isEmpty()) {
        message += tr("- Delete %1 empty jobs (successful with 0 files)\n")
                   .arg(plan.emptyJobs.size());
    }

    if (!plan.failedJobs.isEmpty()) {
        message += tr("- Delete %1 failed jobs older than %2 days\n")
                   .arg(plan.failedJobs.size())
                   .arg(plan.options.failedAgeDays);
    }

    const int totalJobs = plan.jobIdsToDelete().size();
    if (totalJobs > 0) {
        message += tr("  Job records to delete in total: %1\n").arg(totalJobs);
    }

    if (plan.pruneVolumes) {
        message += tr("- Prune all volumes (command: %1)\n").arg(QString::fromLatin1(PruneAllVolumesCommand));
    }

    message += tr("\nDo you want to continue?");

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Confirm Cleanup"), message,
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    // The job list may have been refreshed while the question was open
    if (!m_analysis.canExecute(currentOptions())) {
        QMessageBox::warning(this, tr("Analysis Required"),
                           tr("The job list changed in the meantime. Nothing was deleted; "
                              "please run the analysis again."));
        return;
    }

    performCleanup();
}

void BCleanupDialog::onCloseClicked()
{
    if (m_runner->isRunning()) {
        m_runner->requestStop();
        m_closeButton->setEnabled(false);
        logMessage(tr("Stopping: no further commands are sent after the answer that is still awaited."), "warning");
        return;
    }
    accept();
}

void BCleanupDialog::reject()
{
    if (m_runner->isRunning()) {
        return;  // Use "Stop"; the dialog stays open until the run has ended
    }
    QDialog::reject();
}

void BCleanupDialog::closeEvent(QCloseEvent *event)
{
    if (m_runner->isRunning()) {
        event->ignore();
        return;
    }
    QDialog::closeEvent(event);
}

void BCleanupDialog::analyzeJobs()
{
    logMessage(tr("Analyzing cleanup options..."), "info");
    logMessage("", "info");

    // Check if we have job data
    if (!m_jobsModel || m_jobsModel->allJobs().isEmpty()) {
        logMessage(tr("⚠ No job data available!"), "error");
        logMessage(tr("Please load the job list first (click 'Refresh' in the main window)."), "warning");
        logMessage("", "info");
        setStatus(tr("No job data. Please refresh job list first."), "error");
        return;
    }

    const BCleanup::Plan plan = BCleanup::buildPlan(m_jobsModel->allJobs(), currentOptions(),
                                                    QDateTime::currentDateTime());

    logMessage(tr("Looking at the %1 jobs currently loaded in the job list. "
                  "Jobs that are not loaded there are not considered.").arg(plan.jobRows), "info");
    logMessage("", "info");

    if (plan.options.removeOldFulls) {
        logOldFullBackups(plan);
    }

    if (plan.options.removeEmptyJobs) {
        logEmptyJobs(plan);
    }

    if (plan.options.removeFailedJobs) {
        logFailedJobs(plan);
    }

    if (plan.pruneVolumes) {
        logMessage(tr("✓ Will prune all volumes (apply each volume's retention period)"), "info");
        logMessage(tr("  Command: %1").arg(QString::fromLatin1(PruneAllVolumesCommand)), "info");
    } else if (m_pruneVolumesCheck->isChecked()) {
        logMessage(tr("  'Prune and purge' is not available and will not be sent."), "warning");
    }

    // Summary
    logMessage("", "info");
    logMessage(tr("=== Analysis Summary ==="), "success");

    const int totalJobsToDelete = plan.jobIdsToDelete().size();
    if (totalJobsToDelete > 0) {
        logMessage(tr("Total jobs to delete: %1").arg(totalJobsToDelete), "info");
        if (!plan.oldFulls.isEmpty()) {
            logMessage(tr("  - Old full backups: %1 (%2)")
                       .arg(plan.oldFulls.size())
                       .arg(formatBytes(plan.oldFullsBytes)), "info");
        }
        if (!plan.emptyJobs.isEmpty()) {
            logMessage(tr("  - Empty jobs: %1").arg(plan.emptyJobs.size()), "info");
        }
        if (!plan.failedJobs.isEmpty()) {
            logMessage(tr("  - Failed jobs: %1").arg(plan.failedJobs.size()), "info");
        }
    } else if (!plan.pruneVolumes) {
        logMessage(tr("No jobs found matching cleanup criteria."), "info");
    }

    m_analysis.set(plan);
    updateCleanupButton();

    if (plan.hasWork()) {
        setStatus(tr("Analysis complete. Click 'Cleanup' to proceed."), "success");
    } else {
        setStatus(tr("Analysis complete. Nothing to clean up."), "info");
    }
}

QString BCleanupDialog::formatBytes(qint64 bytes)
{
    if (bytes < 1024) return QString("%1 B").arg(bytes);
    if (bytes < 1024 * 1024) return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    if (bytes < 1024 * 1024 * 1024) return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
    return QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
}

void BCleanupDialog::logOldFullBackups(const BCleanup::Plan &plan)
{
    logMessage(tr("Analyzing old full backups (keeping the %1 most recent per client, job and FileSet)...")
               .arg(plan.options.keepFulls), "info");

    // The analysis refuses rather than guesses when rows are incomplete
    if (plan.oldFullsRefusal != BCleanup::Refusal::None) {
        QString reason;
        switch (plan.oldFullsRefusal) {
        case BCleanup::Refusal::KeepCountTooSmall:
            reason = tr("at least one full backup must be kept");
            break;
        case BCleanup::Refusal::RowsWithoutJobId:
            reason = tr("%1 job rows have no job ID").arg(plan.oldFullsRefusalRows);
            break;
        case BCleanup::Refusal::RowsWithoutType:
            reason = tr("%1 job rows have no job type, so backups cannot be told from other jobs")
                     .arg(plan.oldFullsRefusalRows);
            break;
        case BCleanup::Refusal::BackupRowsWithoutClientOrName:
            reason = tr("%1 backup jobs have no client or job name").arg(plan.oldFullsRefusalRows);
            break;
        case BCleanup::Refusal::None:
            break;
        }
        logMessage(tr("  ⚠ No old full backups are selected: %1.").arg(reason), "error");
        return;
    }

    logMessage(tr("  Note: the FileSet of a job is taken from its current job configuration, "
                  "not from the catalog record of each run."), "info");

    for (const BCleanup::SkippedJob &skipped : plan.skippedJobs) {
        QString reason;
        switch (skipped.reason) {
        case BCleanup::SkipReason::MissingFileset:
            reason = tr("%1 runs have no known FileSet").arg(skipped.rows);
            break;
        case BCleanup::SkipReason::MissingStatus:
            reason = tr("%1 runs have no status").arg(skipped.rows);
            break;
        case BCleanup::SkipReason::UnknownLevel:
            reason = tr("%1 runs have a level other than Full, Differential or Incremental").arg(skipped.rows);
            break;
        case BCleanup::SkipReason::InvalidStartTime:
            reason = tr("%1 runs have no readable start time").arg(skipped.rows);
            break;
        }
        logMessage(tr("  ⚠ Client '%1', job '%2': skipped, nothing selected (%3)")
                   .arg(skipped.client, skipped.name, reason), "warning");
    }

    const int maxPreview = 5;
    for (const BCleanup::FullGroup &group : plan.fullGroups) {
        logMessage(tr("  Client '%1', job '%2', FileSet '%3': %4 full backups, will delete %5")
                   .arg(group.client, group.name, group.fileset)
                   .arg(group.fullCount)
                   .arg(group.selected.size()), "info");

        for (int i = 0; i < group.selected.size(); ++i) {
            if (i == maxPreview) {
                logMessage(tr("    ... and %1 more").arg(group.selected.size() - maxPreview), "warning");
                break;
            }
            const BCleanup::JobRef &job = group.selected[i];
            logMessage(tr("    → Job %1: %2 (%3, %4)")
                       .arg(job.jobId, job.name, job.startTime, formatBytes(job.bytes)), "warning");
            if (!job.requiredDeletions.isEmpty()) {
                logMessage(tr("      its dependent jobs %1 are deleted by this cleanup as well")
                           .arg(job.requiredDeletions.join(", ")), "warning");
            }
        }

        for (const BCleanup::BlockedFull &blocked : group.blocked) {
            logMessage(tr("    ⚠ Job %1 (%2) is kept: incremental/differential jobs %3 depend on it")
                       .arg(blocked.job.jobId, blocked.job.startTime,
                            blocked.dependentJobIds.join(", ")), "info");
        }
    }

    if (!plan.oldFulls.isEmpty()) {
        logMessage(tr("✓ Found %1 old full backups to delete (%2)")
                   .arg(plan.oldFulls.size())
                   .arg(formatBytes(plan.oldFullsBytes)), "success");
    } else {
        logMessage(tr("  No old full backups found to delete"), "info");
    }
}

void BCleanupDialog::logEmptyJobs(const BCleanup::Plan &plan)
{
    logMessage(tr("Analyzing empty jobs (successful with 0 files)..."), "info");

    const int maxPreview = 10;
    for (int i = 0; i < plan.emptyJobs.size() && i < maxPreview; ++i) {
        const BCleanup::JobRef &job = plan.emptyJobs[i];
        logMessage(tr("  → Job %1: %2 [%3] (%4)")
                   .arg(job.jobId, job.name, job.client, job.startTime), "warning");
    }

    if (plan.emptyJobs.size() > maxPreview) {
        logMessage(tr("  ... and %1 more").arg(plan.emptyJobs.size() - maxPreview), "warning");
    }

    if (plan.emptyRowsWithoutFileCount > 0) {
        logMessage(tr("  %1 successful jobs have no file count and are not selected")
                   .arg(plan.emptyRowsWithoutFileCount), "info");
    }

    if (!plan.emptyJobs.isEmpty()) {
        logMessage(tr("✓ Found %1 empty jobs to delete").arg(plan.emptyJobs.size()), "success");
    } else {
        logMessage(tr("  No empty jobs found"), "info");
    }
}

void BCleanupDialog::logFailedJobs(const BCleanup::Plan &plan)
{
    const QDateTime cutoffDate = QDateTime::currentDateTime().addDays(-plan.options.failedAgeDays);

    logMessage(tr("Analyzing failed jobs older than %1 days (before %2)...")
               .arg(plan.options.failedAgeDays)
               .arg(cutoffDate.toString("yyyy-MM-dd")), "info");

    const int maxPreview = 10;
    for (int i = 0; i < plan.failedJobs.size() && i < maxPreview; ++i) {
        const BCleanup::JobRef &job = plan.failedJobs[i];

        QString statusText;
        if (job.status == "f") statusText = tr("Failed");
        else if (job.status == "E") statusText = tr("Error");
        else if (job.status == "A") statusText = tr("Canceled");
        else statusText = tr("Non-fatal error");

        logMessage(tr("  → Job %1: %2 [%3] (%4) - %5")
                   .arg(job.jobId, job.name, job.client, job.startTime, statusText), "warning");
    }

    if (plan.failedJobs.size() > maxPreview) {
        logMessage(tr("  ... and %1 more").arg(plan.failedJobs.size() - maxPreview), "warning");
    }

    if (!plan.failedJobs.isEmpty()) {
        logMessage(tr("✓ Found %1 failed jobs to delete").arg(plan.failedJobs.size()), "success");
    } else {
        logMessage(tr("  No failed jobs found matching criteria"), "info");
    }
}

void BCleanupDialog::setRunning(bool running)
{
    m_optionsGroup->setEnabled(!running);
    m_analyzeButton->setEnabled(!running);
    m_closeButton->setEnabled(true);
    m_closeButton->setText(running ? tr("Stop") : tr("Close"));
    updateCleanupButton();
}

void BCleanupDialog::performCleanup()
{
    // Copy: the analysis is used up, the same plan cannot be executed twice
    const BCleanup::Plan plan = m_analysis.plan();
    m_analysis.invalidate();

    if (!m_director || !m_director->isConnected()) {
        updateCleanupButton();
        QMessageBox::warning(this, tr("Not Connected"),
                           tr("Please connect to Bareos Director first."));
        return;
    }

    logMessage("", "info");
    logMessage(tr("=== Starting Cleanup ==="), "info");

    const int totalSteps = plan.jobIdsToDelete().size() + (plan.pruneVolumes ? 1 : 0);
    m_progressBar->setVisible(true);
    m_progressBar->setMaximum(totalSteps > 0 ? totalSteps : 1);
    m_progressBar->setValue(0);

    setStatus(tr("Cleanup running: commands are sent one at a time..."), "info");

    setRunning(true);
    m_runner->start(plan);
}

void BCleanupDialog::onStepStarted(int index)
{
    const BCleanupRunner::Step &step = m_runner->steps().at(index);
    if (step.kind == BCleanupRunner::StepKind::PruneVolumes) {
        logMessage(tr("Sent: %1").arg(QString::fromLatin1(PruneAllVolumesCommand)), "info");
        m_replyTimer->start(PruneReplyTimeoutSeconds * 1000);
    } else {
        m_replyTimer->start(DeleteReplyTimeoutSeconds * 1000);
    }
}

void BCleanupDialog::onStepFinished(int index)
{
    m_replyTimer->stop();

    const BCleanupRunner::Step &step = m_runner->steps().at(index);
    const QString reply = step.reply.isEmpty() ? tr("(empty answer)") : step.reply;

    if (step.kind == BCleanupRunner::StepKind::DeleteJob) {
        switch (step.outcome) {
        case BCleanupRunner::Outcome::Confirmed:
            logMessage(tr("✓ Job %1: deletion confirmed by the Director").arg(step.jobId), "success");
            break;
        case BCleanupRunner::Outcome::Failed:
            logMessage(tr("✗ Job %1: the Director reported an error: %2").arg(step.jobId, reply), "error");
            break;
        case BCleanupRunner::Outcome::Unconfirmed:
            logMessage(tr("? Job %1: the Director did not confirm the deletion. Answer: %2")
                       .arg(step.jobId, reply), "error");
            break;
        case BCleanupRunner::Outcome::NoReply:
            logMessage(tr("? Job %1: result unknown (%2)").arg(step.jobId, reply), "error");
            break;
        default:
            break;
        }
    } else {
        switch (step.outcome) {
        case BCleanupRunner::Outcome::Answered:
            logMessage(tr("Prune volumes: the Director answered without an error: %1").arg(reply), "success");
            break;
        case BCleanupRunner::Outcome::Failed:
            logMessage(tr("✗ Prune volumes: the Director reported an error: %1").arg(reply), "error");
            break;
        case BCleanupRunner::Outcome::NoReply:
            logMessage(tr("? Prune volumes: result unknown (%1)").arg(reply), "error");
            break;
        default:
            logMessage(tr("? Prune volumes: no usable answer: %1").arg(reply), "error");
            break;
        }
    }

    m_progressBar->setValue(index + 1);
}

void BCleanupDialog::onReplyTimeout()
{
    m_runner->abort(tr("no answer from the Director within the time limit"));
}

void BCleanupDialog::onRunFinished()
{
    m_replyTimer->stop();

    const int confirmed = m_runner->count(BCleanupRunner::Outcome::Confirmed);
    const int answered = m_runner->count(BCleanupRunner::Outcome::Answered);
    const int failed = m_runner->count(BCleanupRunner::Outcome::Failed);
    const int unclear = m_runner->count(BCleanupRunner::Outcome::Unconfirmed)
                      + m_runner->count(BCleanupRunner::Outcome::NoReply);
    const int notSent = m_runner->count(BCleanupRunner::Outcome::NotSent);

    logMessage("", "info");
    logMessage(tr("=== Cleanup Result ==="), m_runner->allSucceeded() ? "success" : "error");
    logMessage(tr("Job deletions confirmed by the Director: %1").arg(confirmed),
               confirmed > 0 ? "success" : "info");
    if (answered > 0) {
        logMessage(tr("Prune volumes: answered without an error (see the answer above)"), "success");
    }
    if (failed > 0) {
        logMessage(tr("Commands the Director answered with an error: %1").arg(failed), "error");
    }
    if (unclear > 0) {
        logMessage(tr("Commands with an unknown result: %1. Refresh the job list to see the actual state.")
                   .arg(unclear), "error");
    }
    if (notSent > 0) {
        logMessage(tr("Commands not sent because the run was stopped: %1").arg(notSent), "warning");
    }

    if (m_runner->allSucceeded()) {
        setStatus(tr("Cleanup finished: the Director confirmed all %1 commands.")
                  .arg(m_runner->steps().size()), "success");
    } else {
        setStatus(tr("Cleanup did not complete: %1 confirmed, %2 failed, %3 unknown, %4 not sent. "
                     "See the log.")
                  .arg(confirmed + answered).arg(failed).arg(unclear).arg(notSent), "error");
    }

    setRunning(false);

    // Only rows whose deletion the Director confirmed leave the job model.
    // The dialog stays open so that the result can be read.
    const QStringList confirmedIds = m_runner->confirmedJobIds();
    if (!confirmedIds.isEmpty()) {
        emit cleanupCompleted(confirmedIds);
    }
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
