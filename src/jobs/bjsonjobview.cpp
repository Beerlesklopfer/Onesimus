#include "jobs/bjsonjobview.h"
#include "buiscale.h"
#include "blogging.h"
#include "bheaderstate.h"
#include "config/bsettings.h"
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
    , m_statusDelegate(new BStatusDelegate(this))
    , m_autoSaveTimer(new QTimer(this))
    , m_director(director)
{
    // Setup filter model
    m_filterModel->setSourceModel(m_model);
    // Sort by the raw values, not by the formatted text (1, 10, 100, 2)
    m_filterModel->setSortRole(BJobsModel::SortRole);
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
    connect(m_autoSaveTimer, &QTimer::timeout, this, &BJsonJobView::saveCurrentState);

    // Connect selection model signals
    connectSelectionModel();

    // Restore the saved header: order, widths, hidden columns, sort column
    restoreLastState();
    // The default widths set above are not a layout of the user's: nothing
    // to save until he changes something (or the first data fits the columns)
    m_autoSaveTimer->stop();
}

void BJsonJobView::setupView()
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
    horizontalHeader()->setSectionsMovable(true);  // Enable drag & drop reordering
    horizontalHeader()->setDragEnabled(true);
    horizontalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
    verticalHeader()->setVisible(false);

    // Every column can be resized by the user; the last visible column takes
    // the remaining width. Columns wider than the view scroll sideways.
    BHeaderState::makeInteractive(horizontalHeader());
    horizontalHeader()->setMinimumSectionSize(BUiScale::px(24));

    // Status and level as chips: code, word, colour (never colour alone)
    setItemDelegateForColumn(BJobsModel::COL_STATUS, m_statusDelegate);
    setItemDelegateForColumn(BJobsModel::COL_LEVEL, m_statusDelegate);

    // Default column widths (used until the user changes them)
    setColumnWidth(BJobsModel::COL_SELECTED, BUiScale::px(40));
    setColumnWidth(BJobsModel::COL_JOBID, BUiScale::px(60));
    setColumnWidth(BJobsModel::COL_NAME, BUiScale::px(200));
    setColumnWidth(BJobsModel::COL_CLIENT, BUiScale::px(100));
    setColumnWidth(BJobsModel::COL_STARTTIME, BUiScale::px(150));
    setColumnWidth(BJobsModel::COL_ENDTIME, BUiScale::px(150));
    setColumnWidth(BJobsModel::COL_DURATION, BUiScale::px(90));
    setColumnWidth(BJobsModel::COL_TYPE, BUiScale::px(50));
    setColumnWidth(BJobsModel::COL_LEVEL, BUiScale::px(80));
    setColumnWidth(BJobsModel::COL_FILES, BUiScale::px(80));
    setColumnWidth(BJobsModel::COL_BYTES, BUiScale::px(100));
    setColumnWidth(BJobsModel::COL_STATUS, BUiScale::px(130));

    // Enable tooltips
    setMouseTracking(true);

    // Auto-save on column resize, move, visibility and sort (debounced)
    connect(horizontalHeader(), &QHeaderView::sectionResized,
            this, [this]() { m_autoSaveTimer->start(); });
    connect(horizontalHeader(), &QHeaderView::sectionMoved,
            this, [this]() { m_autoSaveTimer->start(); });
    connect(horizontalHeader(), &QHeaderView::sortIndicatorChanged,
            this, [this]() { m_autoSaveTimer->start(); });
    connect(m_headerView, &BCheckableHeaderView::columnVisibilityChanged,
            this, [this]() { m_autoSaveTimer->start(); });
}

void BJsonJobView::createContextMenu()
{
    // Job actions
    m_actionDetails = m_contextMenu->addAction(tr("Show Details..."));
    connect(m_actionDetails, &QAction::triggered, this, &BJsonJobView::showJobDetails);
    
    m_contextMenu->addSeparator();
    
    m_actionRetry = m_contextMenu->addAction(tr("Retry Job"));
    connect(m_actionRetry, &QAction::triggered, this, &BJsonJobView::retryJob);
    
    m_actionCancel = m_contextMenu->addAction(tr("Cancel Job"));
    connect(m_actionCancel, &QAction::triggered, this, &BJsonJobView::cancelJob);
    
    m_actionDelete = m_contextMenu->addAction(tr("Delete Job"));
    connect(m_actionDelete, &QAction::triggered, this, &BJsonJobView::deleteJob);
    
    m_contextMenu->addSeparator();
    
    m_actionViewLog = m_contextMenu->addAction(tr("View Log..."));
    connect(m_actionViewLog, &QAction::triggered, this, &BJsonJobView::viewJobLog);

    m_actionRestoreFiles = m_contextMenu->addAction(tr("Restore Files..."));
    m_actionRestoreFiles->setIcon(QIcon::fromTheme("edit-undo"));
    connect(m_actionRestoreFiles, &QAction::triggered, this, &BJsonJobView::restoreFiles);

    m_contextMenu->addSeparator();

    // Export actions
    m_actionExportJson = m_contextMenu->addAction(tr("Export to JSON..."));
    connect(m_actionExportJson, &QAction::triggered, 
            [this]() { exportToJson(true); });
    
    m_actionExportCsv = m_contextMenu->addAction(tr("Export to CSV..."));
    connect(m_actionExportCsv, &QAction::triggered,
            [this]() { exportToCsv(true); });
    
    m_contextMenu->addSeparator();
    
    // Selection actions
    m_actionSelectAll = m_contextMenu->addAction(tr("Select All"));
    connect(m_actionSelectAll, &QAction::triggered,
            this, &BJsonJobView::selectAllVisible);
    
    m_actionInvertSelection = m_contextMenu->addAction(tr("Invert Selection"));
    connect(m_actionInvertSelection, &QAction::triggered,
            m_model, &BJobsModel::invertSelection);
    
    m_actionClearSelection = m_contextMenu->addAction(tr("Clear Selection"));
    connect(m_actionClearSelection, &QAction::triggered,
            this, &BJsonJobView::clearSelection);
}

void BJsonJobView::setJobsData(const QJsonArray &jobs)
{
    m_model->setJobs(jobs);

    // Fit the columns to the first data only while the user has no layout
    // of his own; a saved layout is never overwritten by a refresh
    if (!m_hasSavedLayout && !jobs.isEmpty()) {
        m_hasSavedLayout = true;
        resizeColumnsToContents();
        setColumnWidth(BJobsModel::COL_SELECTED, BUiScale::px(40));
    }
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
        selectedOnly ? tr("Export Selected Jobs to JSON") : tr("Export All Jobs to JSON"),
        "jobs_export.json",
        "JSON Files (*.json);;All Files (*)"
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QJsonDocument doc = m_model->exportToJson(selectedOnly);
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Export Error"),
            tr("Could not write to file: %1").arg(file.errorString()));
        return;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    QMessageBox::information(this, tr("Export Successful"),
        tr("Exported %n job(s) to %1", nullptr,
           int(selectedOnly ? m_model->selectedJobs().size() : m_model->rowCount()))
            .arg(fileName));
    
    emit exportRequested("json", selectedOnly);
}

void BJsonJobView::exportToCsv(bool selectedOnly)
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        selectedOnly ? tr("Export Selected Jobs to CSV") : tr("Export All Jobs to CSV"),
        "jobs_export.csv",
        "CSV Files (*.csv);;All Files (*)"
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QString csv = m_model->exportToCsv(selectedOnly);
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Export Error"),
            tr("Could not write to file: %1").arg(file.errorString()));
        return;
    }
    
    file.write(csv.toUtf8());
    file.close();
    
    QMessageBox::information(this, tr("Export Successful"),
        tr("Exported %n job(s) to %1", nullptr,
           int(selectedOnly ? m_model->selectedJobs().size() : m_model->rowCount()))
            .arg(fileName));
    
    emit exportRequested("csv", selectedOnly);
}

void BJsonJobView::showStatistics()
{
    BJobsModel::Statistics stats = m_model->calculateStatistics();
    
    QString message;
    message += tr("Total Jobs: %1\n").arg(stats.totalJobs);
    message += tr("Successful: %1\n").arg(stats.successfulJobs);
    message += tr("Warnings: %1\n").arg(stats.warningJobs);
    message += tr("Failed: %1\n\n").arg(stats.failedJobs);
    
    message += tr("Total Files: %1\n").arg(stats.totalFiles);
    message += tr("Total Size: %1 bytes\n\n").arg(stats.totalBytes);
    
    if (stats.earliestJob.isValid() && stats.latestJob.isValid()) {
        message += tr("Date Range:\n");
        message += tr("  From: %1\n").arg(stats.earliestJob.toString("yyyy-MM-dd HH:mm"));
        message += tr("  To: %1\n\n").arg(stats.latestJob.toString("yyyy-MM-dd HH:mm"));
    }
    
    message += tr("Selected: %1").arg(stats.selectedCount);
    
    QMessageBox::information(this, tr("Job Statistics"), message);
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
        // ('F' is "waiting on the File daemon", not a failure)
        m_actionRetry->setEnabled(BStatus::isJobError(status));
        
        // Enable cancel for running jobs
        m_actionCancel->setEnabled(status == "R");
        
        m_contextMenu->exec(event->globalPos());
    } else {
        // No job selected - show only global actions
        QMenu menu(this);
        menu.addAction(m_actionSelectAll);
        menu.addAction(m_actionClearSelection);
        menu.addSeparator();
        menu.addAction(tr("Export All to JSON..."), [this]() { exportToJson(false); });
        menu.addAction(tr("Export All to CSV..."), [this]() { exportToCsv(false); });
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
    dialog.setMinimumWidth(BUiScale::px(450));

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
            tr("<div style='background-color: #fff3cd; color: #664d03; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;'>"
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

    // There is no "purge" mode: Bareos 25 does not accept "purge jobs jobid=",
    // it falls back to its interactive "Choose item to purge" prompt, and on a
    // console connection the next queued command would answer it (it can purge
    // all jobs of a client). Purge belongs in the volume view with a preview.

    QRadioButton *pruneRadio = new QRadioButton(tr("Prune (remove expired jobs by retention policy)"), modeGroup);
    pruneRadio->setToolTip(tr("Removes job records that have exceeded their configured retention period.\n"
                              "Only expired jobs for the affected client(s) will be removed."));

    modeLayout->addWidget(deleteRadio);
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
            emit jobActionRequested("delete", jobId);
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

    int ret = QMessageBox::question(this, tr("Rerun Job"),
        tr("Do you want to run job %1 (%2) again?")
            .arg(jobId)
            .arg(jobName),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // Send rerun command to Director
        // commandToString() prefixes "jobid=" itself - pass the plain id
        emit jobActionRequested("rerun", QString("%1 yes").arg(jobId));

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

    int ret = QMessageBox::question(this, tr("Cancel Job"),
        tr("Do you really want to cancel the running job %1 (%2)?")
            .arg(jobId)
            .arg(jobName),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // Send cancel command to Director
        // commandToString() prefixes "jobid=" itself - pass the plain id
        emit jobActionRequested("cancel", jobId);

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
        QMessageBox::warning(this, tr("Not connected"),
                           tr("No connection to the Director."));
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
        // Stored for a 10 pt font, so that it fits any interface zoom later
        preset.columnWidths.append(BUiScale::designPx(columnWidth(i)));
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
        setColumnWidth(i, BUiScale::px(preset.columnWidths[i]));
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
    // Order, widths, hidden columns and the sort column, in BSettings,
    // with the interface scale the widths were measured at
    BSettings::instance().setJobsHeaderState(horizontalHeader()->saveState());
    BSettings::instance().setJobsHeaderScale(BHeaderState::currentScale());
    m_hasSavedLayout = true;
}

void BJsonJobView::restoreLastState()
{
    // BSettings (Widgets/Jobs/header_state) is the single source of the
    // table's layout. Older versions kept it per column through
    // BColumnConfiguration in the group "Columns": that is taken over once
    // when BSettings has nothing yet, then removed.
    const BHeaderState::Source restored = BHeaderState::restoreOrMigrate(
        horizontalHeader(), BSettings::instance().jobsHeaderState(),
        BSettings::instance().jobsHeaderScale(), QStringLiteral("Columns"));
    switch (restored) {
    case BHeaderState::Source::Saved:
        m_hasSavedLayout = true;
        sortByColumn(horizontalHeader()->sortIndicatorSection(),
                     horizontalHeader()->sortIndicatorOrder());
        break;
    case BHeaderState::Source::Legacy:
        m_hasSavedLayout = true;
        saveCurrentState();
        break;
    case BHeaderState::Source::Nothing:
        break;
    }
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
