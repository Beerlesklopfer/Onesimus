#include "jobs/bjsonjobview.h"
#include "blogging.h"
#include "jobs/bjobdetailsdialog.h"
#include "jobs/bjobfileswidget.h"
#include "jobs/bjoblogdialog.h"
#include "jobs/brestorewizard.h"
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QRadioButton>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

BJsonJobView::BJsonJobView(BDirector *director, QWidget *parent)
    : QTableView(parent)
    , m_model(new BJobsModel(director, this))
    , m_filterModel(new BJobsFilterModel(this))
    , m_checkBoxDelegate(new BCheckBoxDelegate(this))
    , m_headerView(new BCheckableHeaderView(Qt::Horizontal, this))
    , m_contextMenu(new QMenu(this))
    , m_liveUpdateTimer(new QTimer(this))
    , m_columnConfig(new BColumnConfiguration(this))
    , m_autoSaveTimer(new QTimer(this))
    , m_director(director)
{
    // Setup filter model
    m_filterModel->setSourceModel(m_model);
    setModel(m_filterModel);
    
    // Set custom header
    setHorizontalHeader(m_headerView);
    
    setupView();
    createContextMenu();
    
    // Connect signals
    connect(m_model, &BJobsModel::selectionChanged,
            this, &BJsonJobView::onSelectionChanged);
    
    connect(m_model, &BJobsModel::jobsAppended,
            this, &BJsonJobView::onJobsAppended);

    connect(m_model, &BJobsModel::modelReset,
            this, [this]() {
        connectSelectionModel();
    });

    connect(m_liveUpdateTimer, &QTimer::timeout,
            this, &BJsonJobView::onLiveUpdateTimeout);
    
    // Connect header checkbox click
    connect(m_headerView, &BCheckableHeaderView::checkboxHeaderClicked,
            [this](int column) {
                if (column == BJobsModel::COL_SELECTED) {
                    m_model->toggleAllSelection();
                }
            });
    
    // Setup auto-save timer (debounce column changes)
    m_autoSaveTimer->setSingleShot(true);
    m_autoSaveTimer->setInterval(500);  // 500ms delay
    connect(m_autoSaveTimer, &QTimer::timeout, [this]() {
        m_columnConfig->saveHeaderState(horizontalHeader());
    });

    // Connect selection model signals
    connectSelectionModel();

    // Restore last saved state
    QTimer::singleShot(100, this, &BJsonJobView::restoreLastState);
}void BJsonJobView::setupView()
{
    // Set checkbox delegate for the selection column
    setItemDelegateForColumn(BJobsModel::COL_SELECTED, m_checkBoxDelegate);
    
    // Configure table appearance
    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSortingEnabled(true);
    setContextMenuPolicy(Qt::DefaultContextMenu);
    
    // Configure headers
    horizontalHeader()->setStretchLastSection(false);
    horizontalHeader()->setSectionsMovable(true);  // Enable drag & drop reordering
    horizontalHeader()->setDragEnabled(true);
    horizontalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
    verticalHeader()->setVisible(false);

    // Set column resize modes
    // Fixed width columns
    horizontalHeader()->setSectionResizeMode(BJobsModel::COL_SELECTED, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(BJobsModel::COL_JOBID, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(BJobsModel::COL_CLIENT, QHeaderView::Interactive);
    horizontalHeader()->setSectionResizeMode(BJobsModel::COL_STARTTIME, QHeaderView::Interactive);
    horizontalHeader()->setSectionResizeMode(BJobsModel::COL_ENDTIME, QHeaderView::Interactive);
    horizontalHeader()->setSectionResizeMode(BJobsModel::COL_DURATION, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(BJobsModel::COL_TYPE, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(BJobsModel::COL_LEVEL, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(BJobsModel::COL_FILES, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(BJobsModel::COL_BYTES, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(BJobsModel::COL_STATUS, QHeaderView::Fixed);

    // Job Name column stretches to fill available space
    horizontalHeader()->setSectionResizeMode(BJobsModel::COL_NAME, QHeaderView::Stretch);

    // Set column widths
    setColumnWidth(BJobsModel::COL_SELECTED, 40);
    setColumnWidth(BJobsModel::COL_JOBID, 60);
    // COL_NAME will stretch automatically
    setColumnWidth(BJobsModel::COL_CLIENT, 100);
    setColumnWidth(BJobsModel::COL_STARTTIME, 150);
    setColumnWidth(BJobsModel::COL_ENDTIME, 150);
    setColumnWidth(BJobsModel::COL_DURATION, 90);
    setColumnWidth(BJobsModel::COL_TYPE, 50);
    setColumnWidth(BJobsModel::COL_LEVEL, 50);
    setColumnWidth(BJobsModel::COL_FILES, 80);
    setColumnWidth(BJobsModel::COL_BYTES, 100);
    setColumnWidth(BJobsModel::COL_STATUS, 120);
    
    // Enable tooltips
    setMouseTracking(true);
    
    // Auto-save on column resize and move (debounced)
    connect(horizontalHeader(), &QHeaderView::sectionResized,
            this, [this]() { m_autoSaveTimer->start(); });
    connect(horizontalHeader(), &QHeaderView::sectionMoved,
            this, [this]() { m_autoSaveTimer->start(); });
    
    // Also save when visibility changes
    connect(m_headerView, &BCheckableHeaderView::columnVisibilityChanged,
            this, [this]() { m_autoSaveTimer->start(); });
}

void BJsonJobView::createContextMenu()
{
    // Job actions
    m_actionDetails = m_contextMenu->addAction("Show Details...");
    connect(m_actionDetails, &QAction::triggered, this, &BJsonJobView::showJobDetails);
    
    m_contextMenu->addSeparator();
    
    m_actionRetry = m_contextMenu->addAction("Retry Job");
    connect(m_actionRetry, &QAction::triggered, this, &BJsonJobView::retryJob);
    
    m_actionCancel = m_contextMenu->addAction("Cancel Job");
    connect(m_actionCancel, &QAction::triggered, this, &BJsonJobView::cancelJob);
    
    m_actionDelete = m_contextMenu->addAction("Delete Job");
    connect(m_actionDelete, &QAction::triggered, this, &BJsonJobView::deleteJob);
    
    m_contextMenu->addSeparator();
    
    m_actionViewLog = m_contextMenu->addAction("View Log...");
    connect(m_actionViewLog, &QAction::triggered, this, &BJsonJobView::viewJobLog);

    m_actionRestoreFiles = m_contextMenu->addAction("Restore Files...");
    m_actionRestoreFiles->setIcon(QIcon::fromTheme("edit-undo"));
    connect(m_actionRestoreFiles, &QAction::triggered, this, &BJsonJobView::restoreFiles);

    m_contextMenu->addSeparator();

    // Export actions
    m_actionExportJson = m_contextMenu->addAction("Export to JSON...");
    connect(m_actionExportJson, &QAction::triggered, 
            [this]() { exportToJson(true); });
    
    m_actionExportCsv = m_contextMenu->addAction("Export to CSV...");
    connect(m_actionExportCsv, &QAction::triggered,
            [this]() { exportToCsv(true); });
    
    m_contextMenu->addSeparator();
    
    // Selection actions
    m_actionSelectAll = m_contextMenu->addAction("Select All");
    connect(m_actionSelectAll, &QAction::triggered,
            this, &BJsonJobView::selectAllVisible);
    
    m_actionInvertSelection = m_contextMenu->addAction("Invert Selection");
    connect(m_actionInvertSelection, &QAction::triggered,
            m_model, &BJobsModel::invertSelection);
    
    m_actionClearSelection = m_contextMenu->addAction("Clear Selection");
    connect(m_actionClearSelection, &QAction::triggered,
            this, &BJsonJobView::clearSelection);
}

void BJsonJobView::setJobsData(const QJsonArray &jobs)
{
    m_model->setJobs(jobs);

    // Auto-resize columns to content
    resizeColumnsToContents();

    // But keep selection column fixed
    setColumnWidth(BJobsModel::COL_SELECTED, 40);
}

void BJsonJobView::appendJobsData(const QJsonArray &jobs)
{
    m_model->appendJobs(jobs);
}

QSet<QString> BJsonJobView::selectedJobIds() const
{
    return m_model->selectedJobIds();
}

QJsonObject BJsonJobView::getSelectedJob() const
{

    QSet<QString> selectedIds = selectedJobIds();

    if (selectedIds.isEmpty()) {
        return QJsonObject();
    }

    // Get first selected job ID
    QString firstJobId = *selectedIds.begin();

    // Find the job in the model
    for (int row = 0; row < m_model->rowCount(); ++row) {
        QJsonObject job = m_model->jobAt(row);
        QString jobId = job["jobid"].toString();

        if (jobId == firstJobId) {
            return job;
        }
    }

    return QJsonObject();
}

void BJsonJobView::clearSelection()
{
    m_model->clearSelection();
}

void BJsonJobView::selectAllVisible()
{
    // Select all jobs visible in the filter
    for (int row = 0; row < m_filterModel->rowCount(); ++row) {
        QModelIndex proxyIndex = m_filterModel->index(row, BJobsModel::COL_SELECTED);
        QModelIndex sourceIndex = m_filterModel->mapToSource(proxyIndex);
        m_model->setData(sourceIndex, Qt::Checked, Qt::CheckStateRole);
    }
}

void BJsonJobView::setLiveUpdateEnabled(bool enabled, int intervalMs)
{
    if (enabled) {
        m_liveUpdateTimer->start(intervalMs);
    } else {
        m_liveUpdateTimer->stop();
    }
}

void BJsonJobView::exportToJson(bool selectedOnly)
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        selectedOnly ? "Export Selected Jobs to JSON" : "Export All Jobs to JSON",
        "jobs_export.json",
        "JSON Files (*.json);;All Files (*)"
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QJsonDocument doc = m_model->exportToJson(selectedOnly);
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Export Error",
            QString("Could not write to file: %1").arg(file.errorString()));
        return;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    QMessageBox::information(this, "Export Successful",
        QString("Exported %1 job(s) to %2")
            .arg(selectedOnly ? m_model->selectedJobs().size() : m_model->rowCount())
            .arg(fileName));
    
    emit exportRequested("json", selectedOnly);
}

void BJsonJobView::exportToCsv(bool selectedOnly)
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        selectedOnly ? "Export Selected Jobs to CSV" : "Export All Jobs to CSV",
        "jobs_export.csv",
        "CSV Files (*.csv);;All Files (*)"
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QString csv = m_model->exportToCsv(selectedOnly);
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Export Error",
            QString("Could not write to file: %1").arg(file.errorString()));
        return;
    }
    
    file.write(csv.toUtf8());
    file.close();
    
    QMessageBox::information(this, "Export Successful",
        QString("Exported %1 job(s) to %2")
            .arg(selectedOnly ? m_model->selectedJobs().size() : m_model->rowCount())
            .arg(fileName));
    
    emit exportRequested("csv", selectedOnly);
}

void BJsonJobView::showStatistics()
{
    BJobsModel::Statistics stats = m_model->calculateStatistics();
    
    QString message;
    message += QString("Total Jobs: %1\n").arg(stats.totalJobs);
    message += QString("Successful: %1\n").arg(stats.successfulJobs);
    message += QString("Warnings: %1\n").arg(stats.warningJobs);
    message += QString("Failed: %1\n\n").arg(stats.failedJobs);
    
    message += QString("Total Files: %1\n").arg(stats.totalFiles);
    message += QString("Total Size: %1 bytes\n\n").arg(stats.totalBytes);
    
    if (stats.earliestJob.isValid() && stats.latestJob.isValid()) {
        message += QString("Date Range:\n");
        message += QString("  From: %1\n").arg(stats.earliestJob.toString("yyyy-MM-dd HH:mm"));
        message += QString("  To: %1\n\n").arg(stats.latestJob.toString("yyyy-MM-dd HH:mm"));
    }
    
    message += QString("Selected: %1").arg(stats.selectedCount);
    
    QMessageBox::information(this, "Job Statistics", message);
}

void BJsonJobView::mousePressEvent(QMouseEvent *event)
{
    BLOG_DEBUG() << "  Mouse position:" << event->pos();
    BLOG_DEBUG() << "  Button:" << event->button();

    QModelIndex proxyIndex = indexAt(event->pos());
    BLOG_DEBUG() << "  Index at position - valid:" << proxyIndex.isValid();
    if (proxyIndex.isValid()) {
        BLOG_DEBUG() << "  Index row:" << proxyIndex.row() << "column:" << proxyIndex.column();
    }

    // Call base implementation
    QTableView::mousePressEvent(event);
}

void BJsonJobView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex proxyIndex = indexAt(event->pos());

    if (proxyIndex.isValid()) {
        // Don't trigger double-click on checkbox column
        if (proxyIndex.column() == BJobsModel::COL_SELECTED) {
            QTableView::mouseDoubleClickEvent(event);
            return;
        }
        
        // Map to source model
        QModelIndex sourceIndex = m_filterModel->mapToSource(proxyIndex);
        QJsonObject job = m_model->jobAt(sourceIndex.row());
        
        if (!job.isEmpty()) {
            emit jobDoubleClicked(job);
        }
    }
    
    QTableView::mouseDoubleClickEvent(event);
}

void BJsonJobView::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    
    if (index.isValid()) {
        // Enable/disable actions based on job status
        QModelIndex sourceIndex = m_filterModel->mapToSource(index);
        QJsonObject job = m_model->jobAt(sourceIndex.row());
        QString status = job["jobstatus"].toString();
        
        // Enable retry for failed jobs
        m_actionRetry->setEnabled(status == "F" || status == "f" || status == "E" || status == "e");
        
        // Enable cancel for running jobs
        m_actionCancel->setEnabled(status == "R");
        
        m_contextMenu->exec(event->globalPos());
    } else {
        // No job selected - show only global actions
        QMenu menu(this);
        menu.addAction(m_actionSelectAll);
        menu.addAction(m_actionClearSelection);
        menu.addSeparator();
        menu.addAction("Export All to JSON...", [this]() { exportToJson(false); });
        menu.addAction("Export All to CSV...", [this]() { exportToCsv(false); });
        menu.exec(event->globalPos());
    }
}

void BJsonJobView::onSelectionChanged()
{
    emit selectionChanged();
}

void BJsonJobView::onJobsAppended(int count)
{
    emit liveDataReceived(count);
}

void BJsonJobView::onLiveUpdateTimeout()
{
    // This would connect to a live data source
    // For now, it's just a placeholder
    // Implementers would override this or connect to it
}

QJsonObject BJsonJobView::getJobAtRow(int row)
{
    QModelIndex proxyIndex = m_filterModel->index(row, 0);
    QModelIndex sourceIndex = m_filterModel->mapToSource(proxyIndex);
    return m_model->jobAt(sourceIndex.row());
}

void BJsonJobView::showJobDetails()
{
    QModelIndexList selection = selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        return;
    }
    
    QModelIndex proxyIndex = selection.first();
    QModelIndex sourceIndex = m_filterModel->mapToSource(proxyIndex);
    QJsonObject job = m_model->jobAt(sourceIndex.row());
    
    if (!job.isEmpty()) {
        emit jobDoubleClicked(job);
    }
}

void BJsonJobView::deleteJob()
{
    // Collect jobs to delete: if checkboxes are checked among visible rows, use those;
    // otherwise use the right-clicked row
    QList<QJsonObject> jobsToDelete;

    QSet<QString> checkedIds = m_model->selectedJobIds();
    if (!checkedIds.isEmpty()) {
        // Batch mode: only include checked jobs that are currently visible (pass filter)
        for (int row = 0; row < m_filterModel->rowCount(); ++row) {
            QModelIndex sourceIndex = m_filterModel->mapToSource(m_filterModel->index(row, 0));
            QJsonObject job = m_model->jobAt(sourceIndex.row());
            if (checkedIds.contains(job["jobid"].toString())) {
                jobsToDelete.append(job);
            }
        }
    }

    if (jobsToDelete.isEmpty()) {
        // No visible checked jobs — fall back to right-clicked row
        QModelIndexList selection = selectionModel()->selectedRows();
        if (selection.isEmpty()) {
            return;
        }
        QModelIndex proxyIndex = selection.first();
        QModelIndex sourceIndex = m_filterModel->mapToSource(proxyIndex);
        jobsToDelete.append(m_model->jobAt(sourceIndex.row()));
    }

    if (jobsToDelete.isEmpty()) {
        return;
    }

    bool batchMode = (jobsToDelete.size() > 1);

    // Check for dependent jobs across all jobs to delete
    int dependentCount = 0;
    for (const QJsonObject &job : jobsToDelete) {
        QString level = job["level"].toString();
        if (level == "F") {
            qint64 thisJobId = job["jobid"].toString().toLongLong();
            QString jobName = job["name"].toString();
            QString clientName = job["client"].toString();

            for (int i = 0; i < m_model->rowCount(); ++i) {
                QJsonObject otherJob = m_model->jobAt(i);
                if (otherJob["name"].toString() == jobName &&
                    otherJob["client"].toString() == clientName) {
                    QString otherLevel = otherJob["level"].toString();
                    qint64 otherJobId = otherJob["jobid"].toString().toLongLong();
                    if ((otherLevel == "I" || otherLevel == "D") && otherJobId > thisJobId) {
                        dependentCount++;
                    }
                }
            }
        }
    }

    // Create dialog with delete options
    QDialog dialog(this);
    dialog.setWindowTitle(batchMode ? tr("Delete %1 Jobs").arg(jobsToDelete.size()) : tr("Delete Job"));
    dialog.setMinimumWidth(450);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Info label
    if (batchMode) {
        // Batch: list job IDs
        QStringList jobSummaries;
        for (const QJsonObject &job : jobsToDelete) {
            jobSummaries << tr("ID %1: %2 (%3)")
                .arg(job["jobid"].toString())
                .arg(job["name"].toString())
                .arg(job["level"].toString());
        }
        QLabel *infoLabel = new QLabel(
            tr("<b>%1 jobs selected for deletion:</b><br>%2")
                .arg(jobsToDelete.size())
                .arg(jobSummaries.join("<br>")));
        infoLabel->setWordWrap(true);
        layout->addWidget(infoLabel);
    } else {
        // Single job: show details
        const QJsonObject &job = jobsToDelete.first();
        QString level = job["level"].toString();
        QString levelText = (level == "F") ? tr("Full") :
                            (level == "I") ? tr("Incremental") :
                            (level == "D") ? tr("Differential") : level;
        QLabel *infoLabel = new QLabel(
            tr("<b>Job:</b> %1 (ID: %2)<br><b>Client:</b> %3<br><b>Level:</b> %4<br><b>Start:</b> %5")
                .arg(job["name"].toString())
                .arg(job["jobid"].toString())
                .arg(job["client"].toString())
                .arg(levelText)
                .arg(job["starttime"].toString()));
        layout->addWidget(infoLabel);
    }

    // Show warning if there are dependent jobs
    if (dependentCount > 0) {
        layout->addSpacing(10);

        QLabel *dependentWarning = new QLabel(
            tr("<div style='background-color: #fff3cd; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;'>"
               "<b>⚠ Attention:</b> %1 dependent Incremental/Differential job(s) found!<br><br>"
               "Deleting Full backups may make dependent backups unusable for restore.</div>")
                .arg(dependentCount));
        dependentWarning->setWordWrap(true);
        layout->addWidget(dependentWarning);
    }

    layout->addSpacing(10);

    // Delete mode selection
    QGroupBox *modeGroup = new QGroupBox(tr("Delete Mode"), &dialog);
    QVBoxLayout *modeLayout = new QVBoxLayout(modeGroup);

    QRadioButton *deleteRadio = new QRadioButton(tr("Delete job record only"), modeGroup);
    deleteRadio->setToolTip(tr("Removes the job entry from the catalog database.\n"
                               "The backup data on the storage media remains intact."));
    deleteRadio->setChecked(true);

    QRadioButton *purgeRadio = new QRadioButton(tr("Purge job (delete record and volume data)"), modeGroup);
    purgeRadio->setToolTip(tr("Removes the job entry AND marks the associated volume data as purgeable.\n"
                              "This frees up space on the storage media."));

    QRadioButton *pruneRadio = new QRadioButton(tr("Prune (remove expired jobs by retention policy)"), modeGroup);
    pruneRadio->setToolTip(tr("Removes job records that have exceeded their configured retention period.\n"
                              "Only expired jobs for the affected client(s) will be removed."));

    modeLayout->addWidget(deleteRadio);
    modeLayout->addWidget(purgeRadio);
    modeLayout->addWidget(pruneRadio);
    layout->addWidget(modeGroup);

    // Warning label
    QLabel *warningLabel = new QLabel(
        tr("<span style='color: #cc0000;'><b>Warning:</b> This action cannot be undone!</span>"));
    layout->addWidget(warningLabel);

    layout->addSpacing(10);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    buttonBox->button(QDialogButtonBox::Ok)->setText(
        batchMode ? tr("Delete %1 Jobs").arg(jobsToDelete.size()) : tr("Delete"));
    buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon::fromTheme("edit-delete"));
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    // Extra confirmation for jobs with dependents
    if (dependentCount > 0) {
        QStringList jobLines;
        for (const QJsonObject &job : jobsToDelete) {
            jobLines << tr("  ID %1: %2 (%3)")
                .arg(job["jobid"].toString())
                .arg(job["name"].toString())
                .arg(job["level"].toString());
        }
        int confirm = QMessageBox::warning(this, tr("Confirm Delete"),
            tr("Are you sure you want to delete %1 job(s)?\n\n%2\n\n"
               "%3 dependent Incremental/Differential backup(s) may become unusable for restore.")
                .arg(jobsToDelete.size())
                .arg(jobLines.join("\n"))
                .arg(dependentCount),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);

        if (confirm != QMessageBox::Yes) {
            return;
        }
    }

    // Send commands based on selected mode
    if (pruneRadio->isChecked()) {
        // Prune: remove expired jobs per client retention policy
        QSet<QString> prunedClients;
        for (const QJsonObject &job : jobsToDelete) {
            QString client = job["client"].toString();
            if (!client.isEmpty() && !prunedClients.contains(client)) {
                prunedClients.insert(client);
                emit jobActionRequested("prune", QString("jobs client=%1 yes").arg(client));
            }
        }
    } else {
        for (const QJsonObject &job : jobsToDelete) {
            QString jobId = job["jobid"].toString();
            if (purgeRadio->isChecked()) {
                emit jobActionRequested("delete", jobId);
                emit jobActionRequested("purge", QString("jobs jobid=%1 yes").arg(jobId));
            } else {
                emit jobActionRequested("delete", jobId);
            }
        }
    }

    // Clear checkbox selection after batch delete
    if (batchMode) {
        m_model->clearSelection();
    }

    // Request refresh after short delay to allow Director to process commands
    QTimer::singleShot(1500, this, [this]() {
        emit refreshRequested();
    });
}

void BJsonJobView::retryJob()
{
    QModelIndexList selection = selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        return;
    }

    QModelIndex proxyIndex = selection.first();
    QModelIndex sourceIndex = m_filterModel->mapToSource(proxyIndex);
    QJsonObject job = m_model->jobAt(sourceIndex.row());

    QString jobId = job["jobid"].toString();
    QString jobName = job["name"].toString();

    int ret = QMessageBox::question(this, "Job erneut ausführen",
        QString("Möchten Sie Job %1 (%2) erneut ausführen?")
            .arg(jobId)
            .arg(jobName),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // Send rerun command to Director
        emit jobActionRequested("rerun", QString("jobid=%1 yes").arg(jobId));

        // Request refresh after short delay to allow Director to process command
        QTimer::singleShot(1500, this, [this]() {
            emit refreshRequested();
        });
    }
}

void BJsonJobView::cancelJob()
{
    QModelIndexList selection = selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        return;
    }

    QModelIndex proxyIndex = selection.first();
    QModelIndex sourceIndex = m_filterModel->mapToSource(proxyIndex);
    QJsonObject job = m_model->jobAt(sourceIndex.row());

    QString jobId = job["jobid"].toString();
    QString jobName = job["name"].toString();

    int ret = QMessageBox::question(this, "Job abbrechen",
        QString("Möchten Sie den laufenden Job %1 (%2) wirklich abbrechen?")
            .arg(jobId)
            .arg(jobName),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // Send cancel command to Director
        emit jobActionRequested("cancel", QString("jobid=%1").arg(jobId));

        // Request refresh after short delay to allow Director to process command
        QTimer::singleShot(1500, this, [this]() {
            emit refreshRequested();
        });
    }
}

void BJsonJobView::viewJobLog()
{
    QModelIndexList selection = selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        return;
    }

    QModelIndex proxyIndex = selection.first();
    QModelIndex sourceIndex = m_filterModel->mapToSource(proxyIndex);
    QJsonObject job = m_model->jobAt(sourceIndex.row());

    if (!m_director) {
        QMessageBox::warning(this, tr("Keine Verbindung"),
                           tr("Keine Verbindung zum Director verfügbar."));
        return;
    }

    // Open job log dialog
    BJobLogDialog dialog(job, m_director, this);
    dialog.exec();
}

void BJsonJobView::restoreFiles()
{
    QModelIndexList selection = selectionModel()->selectedRows();
    if (selection.isEmpty()) return;

    QModelIndex proxyIndex = selection.first();
    QModelIndex sourceIndex = m_filterModel->mapToSource(proxyIndex);
    QJsonObject job = m_model->jobAt(sourceIndex.row());

    if (job.isEmpty()) return;

    if (!m_director) {
        QMessageBox::warning(this, tr("Not Connected"),
            tr("No director connection available."));
        return;
    }

    BRestoreWizard wizard(job, m_director, this);
    wizard.exec();

    if (wizard.restoreSucceeded()) {
        // Refresh job list after a short delay to let the Director register the new job
        QTimer::singleShot(2000, this, [this]() {
            emit refreshRequested();
        });
    }
}

void BJsonJobView::saveViewPreset(const QString &name)
{
    BViewPresets::Preset preset = currentPreset();
    preset.name = name;
    BViewPresets::instance()->savePreset(name, preset);
}

bool BJsonJobView::loadViewPreset(const QString &name)
{
    if (!BViewPresets::instance()->hasPreset(name)) {
        return false;
    }
    
    BViewPresets::Preset preset = BViewPresets::instance()->loadPreset(name);
    applyPreset(preset);
    return true;
}

BViewPresets::Preset BJsonJobView::currentPreset() const
{
    BViewPresets::Preset preset;
    
    QHeaderView *header = horizontalHeader();
    int columnCount = m_model->columnCount();
    
    // Save column widths
    for (int i = 0; i < columnCount; ++i) {
        preset.columnWidths.append(columnWidth(i));
    }
    
    // Save column order
    for (int visual = 0; visual < columnCount; ++visual) {
        int logical = header->logicalIndex(visual);
        preset.columnOrder.append(logical);
    }
    
    // Save column visibility
    for (int i = 0; i < columnCount; ++i) {
        preset.columnVisibility.append(!header->isSectionHidden(i));
    }
    
    // Save sort configuration
    preset.sortColumn = header->sortIndicatorSection();
    preset.sortOrder = header->sortIndicatorOrder();
    
    return preset;
}

void BJsonJobView::applyPreset(const BViewPresets::Preset &preset)
{
    if (preset.columnWidths.isEmpty()) {
        return;  // Invalid preset
    }
    
    QHeaderView *header = horizontalHeader();
    
    // Apply column widths
    for (int i = 0; i < qMin(preset.columnWidths.size(), m_model->columnCount()); ++i) {
        setColumnWidth(i, preset.columnWidths[i]);
    }
    
    // Apply column order
    if (!preset.columnOrder.isEmpty()) {
        for (int visual = 0; visual < preset.columnOrder.size(); ++visual) {
            int logical = preset.columnOrder[visual];
            if (logical >= 0 && logical < m_model->columnCount()) {
                int currentVisual = header->visualIndex(logical);
                if (currentVisual != visual) {
                    header->moveSection(currentVisual, visual);
                }
            }
        }
    }
    
    // Apply column visibility
    if (!preset.columnVisibility.isEmpty()) {
        for (int i = 0; i < qMin(preset.columnVisibility.size(), m_model->columnCount()); ++i) {
            header->setSectionHidden(i, !preset.columnVisibility[i]);
        }
    }
    
    // Apply sort configuration
    if (preset.sortColumn >= 0 && preset.sortColumn < m_model->columnCount()) {
        sortByColumn(preset.sortColumn, preset.sortOrder);
    }
}

void BJsonJobView::saveCurrentState()
{
    // Legacy method - now uses BColumnConfiguration
    m_columnConfig->saveHeaderState(horizontalHeader());
}

void BJsonJobView::restoreLastState()
{
    // Restore using BColumnConfiguration
    m_columnConfig->restoreHeaderState(horizontalHeader());
}

void BJsonJobView::connectSelectionModel()
{

    // Disconnect any previous connections to avoid duplicates
    static QMetaObject::Connection s_connection;
    if (s_connection) {
        disconnect(s_connection);
    }

    // Connect to selectionModel's currentChanged signal
    QItemSelectionModel *selModel = selectionModel();

    if (selModel) {

        s_connection = connect(selModel, &QItemSelectionModel::currentChanged,
                              this, [this](const QModelIndex &current, const QModelIndex &previous) {
            BLOG_DEBUG() << "  Previous row:" << previous.row() << "column:" << previous.column();
            BLOG_DEBUG() << "  Current row:" << current.row() << "column:" << current.column();
            BLOG_DEBUG() << "  Current valid:" << current.isValid();
            BLOG_DEBUG() << "  -> Emitting currentRowChanged signal from BJsonJobView";
            emit currentRowChanged(current, previous);
        });

    } else {
    }
}
