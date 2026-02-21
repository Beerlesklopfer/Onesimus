#include "jobs/brestorewizard.h"
#include "blogging.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QRegularExpression>

#define RESTORE_DEBUG BLOG_DEBUG()

static const int COMMAND_TIMEOUT_MS = 60000;  // 60 seconds per command

/**
 * @brief Extract error message from Bareos JSON-RPC error response
 *
 * Bareos JSON-RPC errors have this structure:
 * {"error": {"code": 1, "message": "failed", "data": {"messages": {"error": ["actual error"]}}}}
 */
static QString extractJsonError(const QJsonObject &root)
{
    QJsonObject error = root["error"].toObject();
    if (error.isEmpty()) return QString();

    // Try data.messages.error[] (Bareos JSON-RPC format)
    QJsonObject data = error["data"].toObject();
    QJsonObject messages = data["messages"].toObject();
    QJsonArray errors = messages["error"].toArray();
    if (!errors.isEmpty()) {
        QStringList parts;
        for (const QJsonValue &v : errors) {
            parts << v.toString();
        }
        return parts.join("; ");
    }

    // Fallback to error.message
    QString msg = error["message"].toString();
    if (!msg.isEmpty()) return msg;

    return QString();
}

// ============================================================================
// BRestoreWizard
// ============================================================================

BRestoreWizard::BRestoreWizard(const QJsonObject &job, BDirector *director,
                               QWidget *parent)
    : QWizard(parent)
    , m_director(director)
{
    // Populate source data from context menu selection
    m_data.jobId = job["jobid"].toString().toULongLong();
    m_data.jobName = job["name"].toString();
    m_data.sourceClient = job["client"].toString();
    // Try common Bareos JSON key names for fileset
    m_data.sourceFileSet = job["fileset"].toString();
    if (m_data.sourceFileSet.isEmpty())
        m_data.sourceFileSet = job["filesetname"].toString();
    if (m_data.sourceFileSet.isEmpty())
        m_data.sourceFileSet = job["FileSet"].toString();
    m_data.selectedClient = m_data.sourceClient;
    m_data.selectedFileSet = m_data.sourceFileSet;
    m_data.targetClient = m_data.sourceClient;

    // Preset beforeDate from job's end time (starttime + duration)
    QDateTime jobStart = QDateTime::fromString(job["starttime"].toString(),
                                                "yyyy-MM-dd HH:mm:ss");
    if (jobStart.isValid()) {
        // Parse duration "HH:MM:SS" and add to start time
        QString durStr = job["duration"].toString();
        QStringList durParts = durStr.split(':');
        if (durParts.size() == 3) {
            int secs = durParts[0].toInt() * 3600
                     + durParts[1].toInt() * 60
                     + durParts[2].toInt();
            m_data.beforeDate = jobStart.addSecs(secs);
        } else {
            m_data.beforeDate = jobStart;
        }
    } else {
        m_data.beforeDate = QDateTime::currentDateTime();
    }

    RESTORE_DEBUG << "Wizard created for Job " << m_data.jobId
                  << " (" << m_data.jobName << ")"
                  << " Client: " << m_data.sourceClient
                  << " FileSet: " << m_data.sourceFileSet;

    if (m_data.sourceFileSet.isEmpty()) {
        RESTORE_DEBUG << "WARNING: No fileset in job data. Available keys:"
                      << job.keys().join(", ");
    }

    // Create BVFS model with checkable mode
    m_bvfsModel = new BBvfsModel(m_director, this);
    m_bvfsModel->setCheckable(true);

    // Set up wizard pages
    setPage(Page_SelectJob, new BRestoreSelectJobPage(this));
    setPage(Page_Browse, new BRestoreBrowsePage(this));
    setPage(Page_Options, new BRestoreOptionsPage(this));
    setPage(Page_Preview, new BRestorePreviewPage(this));
    setPage(Page_Execute, new BRestoreExecutePage(this));

    setWindowTitle(tr("Restore Wizard — Job %1: %2")
                       .arg(m_data.jobId)
                       .arg(m_data.jobName));
    resize(900, 650);

    setWizardStyle(QWizard::ModernStyle);
}

BRestoreWizard::~BRestoreWizard()
{
    RESTORE_DEBUG << "Wizard destroyed";
}

void BRestoreWizard::reject()
{
    // Send cleanup if a restore table was created
    if (m_data.restoreTableCreated && m_director && !m_data.restoreTableName.isEmpty()) {
        RESTORE_DEBUG << "Sending cleanup on cancel: .bvfs_cleanup path="
                      << m_data.restoreTableName;
        m_director->doSend(BDirector::Command::BvfsCleanup,
                           QString("path=%1").arg(m_data.restoreTableName));
    }
    QWizard::reject();
}

// ============================================================================
// Page 1: BRestoreSelectJobPage
// ============================================================================

BRestoreSelectJobPage::BRestoreSelectJobPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Restore Conditions"));
    setSubTitle(tr("Configure the restore scope before browsing files."));

    QFormLayout *layout = new QFormLayout(this);

    // Source job info
    m_sourceLabel = new QLabel(this);
    m_sourceLabel->setWordWrap(true);
    layout->addRow(tr("Source Job:"), m_sourceLabel);

    // Client combo
    m_clientCombo = new QComboBox(this);
    layout->addRow(tr("Client:"), m_clientCombo);

    // FileSet combo
    m_fileSetCombo = new QComboBox(this);
    layout->addRow(tr("FileSet:"), m_fileSetCombo);

    // Restore scope
    m_scopeGroup = new QButtonGroup(this);
    m_allRelatedRadio = new QRadioButton(tr("All related backups (full restore chain)"), this);
    m_singleJobRadio = new QRadioButton(this);  // Text set in initializePage
    m_scopeGroup->addButton(m_allRelatedRadio, 0);
    m_scopeGroup->addButton(m_singleJobRadio, 1);
    m_allRelatedRadio->setChecked(true);

    QVBoxLayout *scopeLayout = new QVBoxLayout;
    scopeLayout->addWidget(m_allRelatedRadio);
    scopeLayout->addWidget(m_singleJobRadio);
    layout->addRow(tr("Restore Scope:"), scopeLayout);

    // Before date
    m_beforeDateEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    m_beforeDateEdit->setCalendarPopup(true);
    m_beforeDateEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    layout->addRow(tr("Before Date:"), m_beforeDateEdit);

    // Load status warning
    m_errorLabel = new QLabel(this);
    m_errorLabel->setWordWrap(true);
    m_errorLabel->setStyleSheet("QLabel { color: orange; font-weight: bold; }");
    m_errorLabel->setVisible(false);
    layout->addRow(QString(), m_errorLabel);

    m_loadTimeoutTimer = new QTimer(this);
    m_loadTimeoutTimer->setSingleShot(true);
    m_loadTimeoutTimer->setInterval(10000);  // 10 seconds
    connect(m_loadTimeoutTimer, &QTimer::timeout,
            this, &BRestoreSelectJobPage::onLoadTimeout);

    connect(m_scopeGroup, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &BRestoreSelectJobPage::onScopeChanged);
}

void BRestoreSelectJobPage::initializePage()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return;
    auto *data = wiz->wizardData();

    int step = wiz->pageIds().indexOf(wiz->currentId()) + 1;
    int total = wiz->pageIds().size();
    setSubTitle(tr("Step %1 of %2 — Configure the restore scope before browsing files.")
                    .arg(step).arg(total));

    m_sourceLabel->setText(tr("Job %1: %2 (Client: %3, FileSet: %4)")
                               .arg(data->jobId)
                               .arg(data->jobName)
                               .arg(data->sourceClient)
                               .arg(data->sourceFileSet));

    m_singleJobRadio->setText(tr("Only this specific job (ID: %1)").arg(data->jobId));

    // Restore previous selections
    m_allRelatedRadio->setChecked(data->allRelatedJobs);
    m_singleJobRadio->setChecked(!data->allRelatedJobs);
    m_beforeDateEdit->setDateTime(data->beforeDate);
    onScopeChanged();

    // Load clients and filesets from Director
    m_errorLabel->setVisible(false);
    if (wiz->director()) {
        if (!m_clientsLoaded || !m_fileSetsLoaded) {
            m_loadTimeoutTimer->start();
        }
        if (!m_clientsLoaded) {
            connect(wiz->director(), &BDirector::jsonResult,
                    this, &BRestoreSelectJobPage::onClientsResponse);
            QMetaObject::invokeMethod(wiz->director(), "doSend",
                                      Qt::QueuedConnection,
                                      Q_ARG(BDirector::Command, BDirector::Command::DotClients),
                                      Q_ARG(QString, QString()));
        }
        if (!m_fileSetsLoaded) {
            connect(wiz->director(), &BDirector::jsonResult,
                    this, &BRestoreSelectJobPage::onFileSetsResponse);
            QMetaObject::invokeMethod(wiz->director(), "doSend",
                                      Qt::QueuedConnection,
                                      Q_ARG(BDirector::Command, BDirector::Command::DotFilesets),
                                      Q_ARG(QString, QString()));
        }
        // Query full job details to get fileset (list jobs doesn't include it)
        if (!m_jobDetailQueried && data->sourceFileSet.isEmpty()) {
            m_jobDetailQueried = true;
            connect(wiz->director(), &BDirector::jsonResult,
                    this, &BRestoreSelectJobPage::onJobDetailResponse);
            connect(wiz->director(), &BDirector::textResult,
                    this, &BRestoreSelectJobPage::onJobDetailResponse);
            RESTORE_DEBUG << "Querying job detail: llist jobid=" << data->jobId;
            QMetaObject::invokeMethod(wiz->director(), "doSend",
                                      Qt::QueuedConnection,
                                      Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                      Q_ARG(QString, QString("llist jobid=%1").arg(data->jobId)));
        }
    }
}

void BRestoreSelectJobPage::onLoadTimeout()
{
    QStringList missing;
    if (!m_clientsLoaded) missing << tr("Clients");
    if (!m_fileSetsLoaded) missing << tr("FileSets");

    if (!missing.isEmpty()) {
        m_errorLabel->setText(tr("Warning: Failed to load %1 from Director within timeout. "
                                 "The Director may be busy or the connection may be slow.")
                                  .arg(missing.join(tr(" and "))));
        m_errorLabel->setVisible(true);
    }
}

void BRestoreSelectJobPage::onScopeChanged()
{
    m_beforeDateEdit->setEnabled(m_allRelatedRadio->isChecked());
}

void BRestoreSelectJobPage::onClientsResponse(BDirector::Command cmd, const QString &jsonData)
{
    if (cmd != BDirector::Command::DotClients) return;
    if (m_clientsLoaded) return;

    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject result = doc.object()["result"].toObject();
    if (!result.contains("clients")) return;

    QJsonArray clients = result["clients"].toArray();
    if (clients.isEmpty()) return;

    m_clientsLoaded = true;
    m_clientCombo->clear();

    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    auto *data = wiz ? wiz->wizardData() : nullptr;
    QString preselect = data ? data->sourceClient : QString();
    int selectIdx = 0;

    // Store client names in wizard data for OptionsPage to reuse
    QStringList clientNames;
    for (int i = 0; i < clients.size(); ++i) {
        QString name = clients[i].toObject()["name"].toString();
        if (!name.isEmpty()) {
            m_clientCombo->addItem(name);
            clientNames.append(name);
            if (name == preselect) selectIdx = m_clientCombo->count() - 1;
        }
    }

    if (data) data->clientNames = clientNames;
    m_clientCombo->setCurrentIndex(selectIdx);
    RESTORE_DEBUG << "Loaded " << m_clientCombo->count() << " clients";

    // Disconnect after first successful load
    if (wiz && wiz->director()) {
        disconnect(wiz->director(), &BDirector::jsonResult,
                   this, &BRestoreSelectJobPage::onClientsResponse);
    }
    if (m_clientsLoaded && m_fileSetsLoaded) {
        m_loadTimeoutTimer->stop();
        m_errorLabel->setVisible(false);
    }
}

void BRestoreSelectJobPage::onFileSetsResponse(BDirector::Command cmd, const QString &jsonData)
{
    if (cmd != BDirector::Command::DotFilesets) return;
    if (m_fileSetsLoaded) return;

    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject result = doc.object()["result"].toObject();
    if (!result.contains("filesets")) return;

    QJsonArray filesets = result["filesets"].toArray();
    if (filesets.isEmpty()) return;

    m_fileSetsLoaded = true;
    m_fileSetCombo->clear();

    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    QString preselect = wiz ? wiz->wizardData()->sourceFileSet : QString();
    int selectIdx = 0;

    for (int i = 0; i < filesets.size(); ++i) {
        QString name = filesets[i].toObject()["name"].toString();
        if (!name.isEmpty()) {
            m_fileSetCombo->addItem(name);
            if (name == preselect) selectIdx = m_fileSetCombo->count() - 1;
        }
    }

    m_fileSetCombo->setCurrentIndex(selectIdx);
    RESTORE_DEBUG << "Loaded " << m_fileSetCombo->count() << " filesets"
                  << " preselect=" << preselect << " selectIdx=" << selectIdx;

    if (wiz && wiz->director()) {
        disconnect(wiz->director(), &BDirector::jsonResult,
                   this, &BRestoreSelectJobPage::onFileSetsResponse);
    }
    if (m_clientsLoaded && m_fileSetsLoaded) {
        m_loadTimeoutTimer->stop();
        m_errorLabel->setVisible(false);
    }
}

void BRestoreSelectJobPage::onJobDetailResponse(BDirector::Command cmd, const QString &jsonData)
{
    if (cmd != BDirector::Command::Custom) return;

    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());

    RESTORE_DEBUG << "onJobDetailResponse: " << jsonData.left(300);

    // Parse llist jobid response to extract fileset
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    // llist jobs returns a "jobs" array
    QJsonArray jobs = result["jobs"].toArray();
    if (jobs.isEmpty()) {
        RESTORE_DEBUG << "llist jobid: no jobs array in response";
        // Might be a text response or different JSON structure — disconnect and bail
        if (wiz && wiz->director()) {
            disconnect(wiz->director(), &BDirector::jsonResult,
                       this, &BRestoreSelectJobPage::onJobDetailResponse);
            disconnect(wiz->director(), &BDirector::textResult,
                       this, &BRestoreSelectJobPage::onJobDetailResponse);
        }
        return;
    }

    QJsonObject job = jobs[0].toObject();
    QString fileset = job["fileset"].toString();
    if (fileset.isEmpty())
        fileset = job["filesetname"].toString();
    if (fileset.isEmpty())
        fileset = job["FileSet"].toString();

    if (fileset.isEmpty()) {
        RESTORE_DEBUG << "llist jobid response has no fileset. Keys: "
                      << job.keys().join(", ");
    } else {
        RESTORE_DEBUG << "Got fileset from llist jobid: " << fileset;

        // Store in wizard data
        if (wiz) {
            auto *data = wiz->wizardData();
            data->sourceFileSet = fileset;
            data->selectedFileSet = fileset;

            // Update source label
            m_sourceLabel->setText(tr("Job %1: %2 (Client: %3, FileSet: %4)")
                                       .arg(data->jobId)
                                       .arg(data->jobName)
                                       .arg(data->sourceClient)
                                       .arg(data->sourceFileSet));
        }

        // Preselect in combo if filesets are already loaded
        preselectFileSet();
    }

    // Disconnect after handling
    if (wiz && wiz->director()) {
        disconnect(wiz->director(), &BDirector::jsonResult,
                   this, &BRestoreSelectJobPage::onJobDetailResponse);
        disconnect(wiz->director(), &BDirector::textResult,
                   this, &BRestoreSelectJobPage::onJobDetailResponse);
    }
}

void BRestoreSelectJobPage::preselectFileSet()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return;

    QString fileset = wiz->wizardData()->sourceFileSet;
    if (fileset.isEmpty() || m_fileSetCombo->count() == 0) return;

    int idx = m_fileSetCombo->findText(fileset);
    if (idx >= 0) {
        m_fileSetCombo->setCurrentIndex(idx);
        RESTORE_DEBUG << "FileSet preselected: " << fileset << " at index " << idx;
    } else {
        RESTORE_DEBUG << "FileSet not found in combo: " << fileset;
    }
}

bool BRestoreSelectJobPage::validatePage()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return false;
    auto *data = wiz->wizardData();

    data->selectedClient = m_clientCombo->currentText();
    data->selectedFileSet = m_fileSetCombo->currentText();
    data->allRelatedJobs = m_allRelatedRadio->isChecked();
    data->beforeDate = m_beforeDateEdit->dateTime();

    RESTORE_DEBUG << "SelectJobPage validated: client=" << data->selectedClient
                  << " fileset=" << data->selectedFileSet
                  << " allRelated=" << data->allRelatedJobs
                  << " before=" << data->beforeDate.toString(Qt::ISODate);

    return true;
}

// ============================================================================
// Page 2: BRestoreBrowsePage
// ============================================================================

BRestoreBrowsePage::BRestoreBrowsePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Select Files"));
    setSubTitle(tr("Browse and select files/directories to restore."));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Info label
    m_infoLabel = new QLabel(this);
    mainLayout->addWidget(m_infoLabel);

    // Progress bar
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0);
    m_progressBar->setTextVisible(true);
    m_progressBar->setFormat(tr("Loading files..."));
    m_progressBar->setVisible(false);
    mainLayout->addWidget(m_progressBar);

    // Splitter: tree (left) + list (right)
    m_splitter = new QSplitter(Qt::Horizontal, this);

    m_treeView = new QTreeView(m_splitter);
    m_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_listView = new QTableView(m_splitter);
    m_listView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listView->verticalHeader()->setVisible(false);
    m_listView->setSortingEnabled(true);
    m_listView->horizontalHeader()->setSectionsClickable(true);
    m_listView->horizontalHeader()->setSortIndicatorShown(true);
    m_listView->horizontalHeader()->setStretchLastSection(false);
    m_listView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    m_splitter->addWidget(m_treeView);
    m_splitter->addWidget(m_listView);
    m_splitter->setSizes({250, 550});
    mainLayout->addWidget(m_splitter);

    // Selection count label
    m_selectionLabel = new QLabel(tr("No files selected"), this);
    mainLayout->addWidget(m_selectionLabel);
}

void BRestoreBrowsePage::initializePage()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return;
    auto *data = wiz->wizardData();
    auto *model = wiz->bvfsModel();

    int step = wiz->pageIds().indexOf(wiz->currentId()) + 1;
    int total = wiz->pageIds().size();
    setSubTitle(tr("Step %1 of %2 — Browse and select files/directories to restore.")
                    .arg(step).arg(total));

    m_infoLabel->setText(tr("Browse files — Client: %1, FileSet: %2")
                             .arg(data->selectedClient)
                             .arg(data->selectedFileSet));

    // Set up proxy for tree (dirs only) and sort proxy for list
    if (!m_dirProxy) {
        m_dirProxy = new BBvfsDirFilterProxy(this);
        m_dirProxy->setSourceModel(model);

        m_treeView->setModel(m_dirProxy);
        m_treeView->setHeaderHidden(false);
        // Hide non-name columns in tree
        for (int i = 1; i < BBvfsModel::ColCount; ++i) {
            m_treeView->setColumnHidden(i, true);
        }

        // Sort proxy for the file list
        m_listSortProxy = new QSortFilterProxyModel(this);
        m_listSortProxy->setSourceModel(model);
        m_listSortProxy->setSortRole(BBvfsModel::SortRole);
        m_listView->setModel(m_listSortProxy);

        // Set column resize modes: Name stretches, others fit content
        auto *header = m_listView->horizontalHeader();
        header->setSectionResizeMode(BBvfsModel::ColName, QHeaderView::Stretch);
        header->setSectionResizeMode(BBvfsModel::ColSize, QHeaderView::ResizeToContents);
        header->setSectionResizeMode(BBvfsModel::ColType, QHeaderView::ResizeToContents);
        header->setSectionResizeMode(BBvfsModel::ColModified, QHeaderView::ResizeToContents);

        connect(m_treeView, &QTreeView::clicked,
                this, &BRestoreBrowsePage::onTreeItemClicked);
        connect(m_treeView, &QTreeView::expanded,
                this, &BRestoreBrowsePage::onTreeItemExpanded);
        connect(m_treeView->selectionModel(), &QItemSelectionModel::currentChanged,
                this, &BRestoreBrowsePage::onTreeCurrentChanged);
        connect(m_listView, &QTableView::doubleClicked,
                this, &BRestoreBrowsePage::onFileListDoubleClicked);
        connect(model, &BBvfsModel::loadingStarted,
                this, &BRestoreBrowsePage::onLoadingStarted);
        connect(model, &BBvfsModel::loadingFinished,
                this, &BRestoreBrowsePage::onLoadingFinished);
        connect(model, &BBvfsModel::selectionCountChanged,
                this, &BRestoreBrowsePage::onSelectionCountChanged);
    }

    // Load BVFS data if not already loaded or if settings changed
    bool settingsChanged = m_loaded && (
        m_loadedClient != data->selectedClient ||
        m_loadedFileSet != data->selectedFileSet ||
        m_loadedAllRelated != data->allRelatedJobs);

    if (!m_loaded || settingsChanged) {
        model->resetModel();
        model->loadJob(data->jobId, data->allRelatedJobs);
        m_loaded = true;
        m_initialExpandDone = false;
        m_loadedClient = data->selectedClient;
        m_loadedFileSet = data->selectedFileSet;
        m_loadedAllRelated = data->allRelatedJobs;
    }
}

bool BRestoreBrowsePage::isComplete() const
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    return wiz && wiz->bvfsModel()->selectedCount() > 0;
}

bool BRestoreBrowsePage::validatePage()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return false;
    auto *data = wiz->wizardData();
    auto *model = wiz->bvfsModel();

    data->selectedFileIds = model->selectedFileIds();
    data->selectedDirIds = model->selectedDirIds();
    data->resolvedBvfsJobIds = model->bvfsJobIds();

    RESTORE_DEBUG << "BrowsePage validated: " << data->selectedFileIds.size()
                  << " files, " << data->selectedDirIds.size() << " dirs selected"
                  << " bvfsJobIds=" << data->resolvedBvfsJobIds;

    return true;
}

void BRestoreBrowsePage::showDirectoryInList(const QModelIndex &dirProxyIndex)
{
    if (!dirProxyIndex.isValid()) return;

    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return;
    auto *model = wiz->bvfsModel();

    QModelIndex srcIndex = m_dirProxy->mapToSource(dirProxyIndex);

    if (model->canFetchMore(srcIndex)) {
        model->fetchMore(srcIndex);
    }
    model->loadFilesForDirectory(srcIndex);

    // Map source index through sort proxy for the list view
    QModelIndex listProxyIndex = m_listSortProxy->mapFromSource(srcIndex);
    m_listView->setRootIndex(listProxyIndex);
}

void BRestoreBrowsePage::onTreeItemClicked(const QModelIndex &proxyIndex)
{
    showDirectoryInList(proxyIndex);
}

void BRestoreBrowsePage::onTreeItemExpanded(const QModelIndex &proxyIndex)
{
    // When expanding via arrow click, also update the list view
    showDirectoryInList(proxyIndex);
    m_treeView->setCurrentIndex(proxyIndex);
}

void BRestoreBrowsePage::onTreeCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    showDirectoryInList(current);
}

void BRestoreBrowsePage::onFileListDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;
    if (!index.data(BBvfsModel::IsDirectoryRole).toBool()) return;

    // Map from sort proxy to source, then to dir proxy
    QModelIndex sortProxyIndex = index.sibling(index.row(), 0);
    QModelIndex srcIndex = m_listSortProxy->mapToSource(sortProxyIndex);
    QModelIndex dirProxyIndex = m_dirProxy->mapFromSource(srcIndex);

    if (dirProxyIndex.isValid()) {
        m_treeView->setCurrentIndex(dirProxyIndex);
        m_treeView->expand(dirProxyIndex);
        showDirectoryInList(dirProxyIndex);
    }
}

void BRestoreBrowsePage::onLoadingStarted()
{
    m_progressBar->setVisible(true);
}

void BRestoreBrowsePage::onLoadingFinished()
{
    m_progressBar->setVisible(false);

    // Auto-expand first root item only on initial load
    if (!m_initialExpandDone) {
        m_initialExpandDone = true;
        QModelIndex firstProxy = m_dirProxy->index(0, 0);
        if (firstProxy.isValid()) {
            m_treeView->expand(firstProxy);
            m_treeView->setCurrentIndex(firstProxy);
            showDirectoryInList(firstProxy);
        }
    }
}

void BRestoreBrowsePage::onSelectionCountChanged(int count)
{
    if (count == 0) {
        m_selectionLabel->setText(tr("No files selected"));
    } else {
        auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
        if (wiz) {
            auto *model = wiz->bvfsModel();
            int fileCount = model->selectedFileIds().size();
            int dirCount = model->selectedDirIds().size();
            QStringList parts;
            if (dirCount > 0)
                parts << tr("%n directory(ies)", "", dirCount);
            if (fileCount > 0)
                parts << tr("%n file(s)", "", fileCount);
            m_selectionLabel->setText(tr("Selected: %1").arg(parts.join(", ")));
        } else {
            m_selectionLabel->setText(tr("%1 item(s) selected").arg(count));
        }
    }
    emit completeChanged();
}

// ============================================================================
// Page 3: BRestoreOptionsPage
// ============================================================================

BRestoreOptionsPage::BRestoreOptionsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Restore Options"));
    setSubTitle(tr("Configure where and how to restore the selected files."));

    QFormLayout *layout = new QFormLayout(this);

    m_sourceLabel = new QLabel(this);
    m_sourceLabel->setWordWrap(true);
    layout->addRow(tr("Source:"), m_sourceLabel);

    m_targetClientCombo = new QComboBox(this);
    layout->addRow(tr("Target Client:"), m_targetClientCombo);

    m_restoreWhereEdit = new QLineEdit("/tmp/bareos-restores", this);
    layout->addRow(tr("Restore Where:"), m_restoreWhereEdit);

    m_replacePolicyCombo = new QComboBox(this);
    m_replacePolicyCombo->addItem(tr("Always"), "always");
    m_replacePolicyCombo->addItem(tr("Never"), "never");
    m_replacePolicyCombo->addItem(tr("If Newer"), "ifnewer");
    m_replacePolicyCombo->addItem(tr("If Older"), "ifolder");
    layout->addRow(tr("Replace Policy:"), m_replacePolicyCombo);

    m_replacePolicyHelpLabel = new QLabel(this);
    m_replacePolicyHelpLabel->setWordWrap(true);
    m_replacePolicyHelpLabel->setStyleSheet("QLabel { color: gray; font-style: italic; }");
    layout->addRow(QString(), m_replacePolicyHelpLabel);
    updateReplacePolicyHelp(0);

    connect(m_replacePolicyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BRestoreOptionsPage::updateReplacePolicyHelp);
    connect(m_targetClientCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BRestoreOptionsPage::completeChanged);
    connect(m_restoreWhereEdit, &QLineEdit::textChanged,
            this, &BRestoreOptionsPage::completeChanged);
}

void BRestoreOptionsPage::initializePage()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return;
    auto *data = wiz->wizardData();

    int step = wiz->pageIds().indexOf(wiz->currentId()) + 1;
    int total = wiz->pageIds().size();
    setSubTitle(tr("Step %1 of %2 — Configure where and how to restore the selected files.")
                    .arg(step).arg(total));

    QStringList selParts;
    if (data->selectedDirIds.size() > 0)
        selParts << tr("%n directory(ies)", "", data->selectedDirIds.size());
    if (data->selectedFileIds.size() > 0)
        selParts << tr("%n file(s)", "", data->selectedFileIds.size());
    if (selParts.isEmpty())
        selParts << tr("nothing");

    // Compute estimated restore size
    auto *model = wiz->bvfsModel();
    qint64 estimatedSize = model->computeSelectedSize();
    QString sizeStr = estimatedSize > 0
        ? tr("~%1").arg(model->formatBytes(estimatedSize))
        : tr("unknown");

    m_sourceLabel->setText(tr("Job %1: %2 — %3 selected (estimated size: %4)")
                               .arg(data->jobId)
                               .arg(data->jobName)
                               .arg(selParts.join(", "))
                               .arg(sizeStr));

    // Populate target client combo from cached client list (loaded by SelectJobPage)
    m_targetClientCombo->clear();
    QString preselect = data->targetClient.isEmpty() ? data->sourceClient : data->targetClient;
    int selectIdx = 0;

    if (!data->clientNames.isEmpty()) {
        for (int i = 0; i < data->clientNames.size(); ++i) {
            m_targetClientCombo->addItem(data->clientNames.at(i));
            if (data->clientNames.at(i) == preselect) selectIdx = i;
        }
    } else {
        // Fallback: at least add source client if list wasn't loaded
        m_targetClientCombo->addItem(data->sourceClient);
    }
    m_targetClientCombo->setCurrentIndex(selectIdx);

    // Restore previous values
    m_restoreWhereEdit->setText(data->restoreWhere);

    int policyIdx = m_replacePolicyCombo->findData(data->replacePolicy);
    if (policyIdx >= 0) m_replacePolicyCombo->setCurrentIndex(policyIdx);
}

void BRestoreOptionsPage::updateReplacePolicyHelp(int index)
{
    switch (index) {
    case 0:
        m_replacePolicyHelpLabel->setText(
            tr("Always overwrite existing files on the target, regardless of timestamps."));
        break;
    case 1:
        m_replacePolicyHelpLabel->setText(
            tr("Never overwrite. Skip files that already exist on the target."));
        break;
    case 2:
        m_replacePolicyHelpLabel->setText(
            tr("Only overwrite if the backed-up file is newer than the existing file."));
        break;
    case 3:
        m_replacePolicyHelpLabel->setText(
            tr("Only overwrite if the backed-up file is older than the existing file."));
        break;
    default:
        m_replacePolicyHelpLabel->clear();
        break;
    }
}

bool BRestoreOptionsPage::isComplete() const
{
    return !m_targetClientCombo->currentText().isEmpty()
        && !m_restoreWhereEdit->text().trimmed().isEmpty();
}

bool BRestoreOptionsPage::validatePage()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return false;
    auto *data = wiz->wizardData();

    data->targetClient = m_targetClientCombo->currentText();
    data->restoreWhere = m_restoreWhereEdit->text().trimmed();
    data->replacePolicy = m_replacePolicyCombo->currentData().toString();

    RESTORE_DEBUG << "OptionsPage validated: target=" << data->targetClient
                  << " where=" << data->restoreWhere
                  << " replace=" << data->replacePolicy;

    return true;
}

// ============================================================================
// Page 4: BRestorePreviewPage
// ============================================================================

BRestorePreviewPage::BRestorePreviewPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Command Preview"));
    setSubTitle(tr("Review the commands that will be sent to the Director."));
    setCommitPage(true);

    QVBoxLayout *layout = new QVBoxLayout(this);

    m_summaryLabel = new QLabel(this);
    m_summaryLabel->setWordWrap(true);
    layout->addWidget(m_summaryLabel);

    // Permission checks group
    QGroupBox *permGroup = new QGroupBox(tr("Permission Checks"), this);
    QFormLayout *permLayout = new QFormLayout(permGroup);
    m_authRestoreLabel = new QLabel(this);
    m_authClientLabel = new QLabel(this);
    m_authWhereLabel = new QLabel(this);
    m_authStatusLabel = new QLabel(this);
    m_authStatusLabel->setStyleSheet("QLabel { color: gray; font-style: italic; }");
    permLayout->addRow(tr("Restore command:"), m_authRestoreLabel);
    permLayout->addRow(tr("Target client:"), m_authClientLabel);
    permLayout->addRow(tr("Where path:"), m_authWhereLabel);
    permLayout->addRow(QString(), m_authStatusLabel);
    layout->addWidget(permGroup);

    // Timeout timer for permission checks
    m_authTimeoutTimer = new QTimer(this);
    m_authTimeoutTimer->setSingleShot(true);
    connect(m_authTimeoutTimer, &QTimer::timeout,
            this, &BRestorePreviewPage::onAuthCheckTimeout);

    // Command preview — only visible in debug builds
    m_commandPreview = new QTextEdit(this);
    m_commandPreview->setReadOnly(true);
    m_commandPreview->setFont(QFont("monospace"));
#ifdef QT_DEBUG
    layout->addWidget(m_commandPreview);
#else
    m_commandPreview->setVisible(false);
    layout->addStretch();
#endif

    QLabel *infoLabel = new QLabel(
        tr("Click \"Commit\" to execute these commands on the Director."), this);
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);
}

void BRestorePreviewPage::initializePage()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return;
    auto *data = wiz->wizardData();

    int step = wiz->pageIds().indexOf(wiz->currentId()) + 1;
    int total = wiz->pageIds().size();
    setSubTitle(tr("Step %1 of %2 — Review the commands that will be sent to the Director.")
                    .arg(step).arg(total));

    // Generate unique restore table name
    int rnd = QRandomGenerator::global()->bounded(100000, 999999);
    data->restoreTableName = QString("b2%1").arg(rnd);

    // Build file ID and dir ID lists
    QString fileIds = data->selectedFileIds.join(",");
    QString dirIds;
    QStringList dirIdStrings;
    for (int id : data->selectedDirIds) {
        dirIdStrings.append(QString::number(id));
    }
    dirIds = dirIdStrings.join(",");

    // Build .bvfs_restore command
    QStringList parts;
    parts << QString("path=%1").arg(data->restoreTableName);
    parts << QString("jobid=%1").arg(data->resolvedBvfsJobIds);
    if (!fileIds.isEmpty()) {
        parts << QString("fileid=%1").arg(fileIds);
    }
    if (!dirIds.isEmpty()) {
        parts << QString("dirid=%1").arg(dirIds);
    }
    data->bvfsRestoreCommand = parts.join(" ");

    // Build restore command
    // Other useful options:
    // restorejob=
    // restoreclient=
    data->restoreCommand = QString("file=?%1 client=%2 where=%3 replace=%4 yes")
                               .arg(data->restoreTableName)
                               .arg(data->targetClient)
                               .arg(data->restoreWhere)
                               .arg(data->replacePolicy);

    // Update UI — build selection summary
    QStringList selectionParts;
    int nFiles = data->selectedFileIds.size();
    int nDirs = data->selectedDirIds.size();
    if (nDirs > 0 && nFiles > 0) {
        selectionParts << tr("%n directory(ies) (including all contents)", "", nDirs);
        selectionParts << tr("%n individual file(s)", "", nFiles);
    } else if (nDirs > 0) {
        selectionParts << tr("%n directory(ies) (including all contents)", "", nDirs);
    } else if (nFiles > 0) {
        selectionParts << tr("%n file(s)", "", nFiles);
    } else {
        selectionParts << tr("No files or directories selected");
    }

    // Compute estimated restore size
    auto *model = wiz->bvfsModel();
    qint64 estimatedSize = model->computeSelectedSize();
    QString sizeStr = estimatedSize > 0
        ? tr("~%1").arg(model->formatBytes(estimatedSize))
        : tr("unknown");

    m_summaryLabel->setText(tr("Restore summary:\n"
                               "• Selection: %1\n"
                               "• Estimated size: %2\n"
                               "• Target client: %3\n"
                               "• Restore to: %4\n"
                               "• Replace policy: %5")
                                .arg(selectionParts.join(", "))
                                .arg(sizeStr)
                                .arg(data->targetClient)
                                .arg(data->restoreWhere)
                                .arg(data->replacePolicy));

    QString preview;
    preview += "# Step 1: Create restore table\n";
    preview += ".bvfs_restore " + data->bvfsRestoreCommand + "\n\n";
    preview += "# Step 2: Execute restore\n";
    preview += "restore " + data->restoreCommand + "\n\n";
    preview += "# Step 3: Cleanup (automatic)\n";
    preview += QString(".bvfs_cleanup path=%1\n").arg(data->restoreTableName);

    m_commandPreview->setPlainText(preview);

    RESTORE_DEBUG << "PreviewPage: table=" << data->restoreTableName;
    RESTORE_DEBUG << "  bvfs_restore: " << data->bvfsRestoreCommand;
    RESTORE_DEBUG << "  restore: " << data->restoreCommand;

    // Start permission checks using .help all (single command, reliable)
    m_authRestoreLabel->setText(tr("Checking..."));
    m_authClientLabel->setText(tr("Checking..."));
    m_authWhereLabel->setText(tr("Checking..."));
    m_authStatusLabel->setText(tr("Querying Director for permissions..."));
    m_authDone = false;

    if (wiz->director()) {
        connect(wiz->director(), &BDirector::jsonResult,
                this, &BRestorePreviewPage::onHelpAllResponse);
        connect(wiz->director(), &BDirector::textResult,
                this, &BRestorePreviewPage::onHelpAllResponse);
        wiz->director()->doSend(BDirector::Command::Custom, ".help all");
        m_authTimeoutTimer->start(8000);  // 8 second timeout
    } else {
        // No director — skip checks
        m_authStatusLabel->setText(tr("No Director connection"));
        finishAuthChecks();
    }
}

bool BRestorePreviewPage::isComplete() const
{
    // Block the Commit button until auth checks finish.
    // This prevents a race condition where auth responses arrive
    // after the Execute page has already sent its own commands,
    // corrupting m_lastCommand tracking.
    return m_authDone;
}

void BRestorePreviewPage::cleanupPage()
{
    m_authTimeoutTimer->stop();
    disconnectAuth();
}

void BRestorePreviewPage::onHelpAllResponse(BDirector::Command cmd, const QString &jsonData)
{
    if (cmd != BDirector::Command::Custom) return;
    if (m_authDone) return;

    // Parse .help all response: {"result": {"restore": {"permission": true, ...}, ...}}
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    // Verify this is a .help all response (entries have "permission" field)
    bool isHelpResponse = false;
    for (auto it = result.begin(); it != result.end(); ++it) {
        if (it.value().isObject() && it.value().toObject().contains("permission")) {
            isHelpResponse = true;
            break;
        }
    }
    if (!isHelpResponse || result.size() < 5) return;

    m_authTimeoutTimer->stop();

    // Check "restore" command permission
    bool restoreAuthorized = false;
    if (result.contains("restore")) {
        restoreAuthorized = result["restore"].toObject()["permission"].toBool(false);
    }
    setAuthLabel(m_authRestoreLabel, restoreAuthorized);

    // Parse restore command arguments for client= and where= availability
    QString restoreArgs;
    if (result.contains("restore")) {
        restoreArgs = result["restore"].toObject()["arguments"].toString();
    }

    // Check client= parameter
    bool clientAuthorized = restoreAuthorized && restoreArgs.contains("client=");
    setAuthLabel(m_authClientLabel, clientAuthorized,
                 clientAuthorized ? QString() : tr("restore command lacks client= parameter"));

    // Check where= parameter — if available, it's still subject to WhereACL
    bool whereAvailable = restoreAuthorized && restoreArgs.contains("where=");
    if (whereAvailable) {
        m_authWhereLabel->setText(tr("Available (subject to WhereACL)"));
        m_authWhereLabel->setStyleSheet("QLabel { color: green; font-weight: bold; }");
    } else if (restoreAuthorized) {
        m_authWhereLabel->setText(tr("Not available"));
        m_authWhereLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    } else {
        m_authWhereLabel->setText(tr("N/A (restore not authorized)"));
        m_authWhereLabel->setStyleSheet("QLabel { color: gray; font-weight: bold; }");
    }

    m_authStatusLabel->setText(tr("Permission check completed."));
    m_authStatusLabel->setStyleSheet("QLabel { color: green; }");

    RESTORE_DEBUG << "Permission check via .help all: restore=" << restoreAuthorized
                  << " client=" << clientAuthorized << " where=" << whereAvailable;

    finishAuthChecks();
}

void BRestorePreviewPage::onAuthCheckTimeout()
{
    if (m_authDone) return;

    RESTORE_DEBUG << "Permission check timed out";

    // Set labels to unknown state — don't block the user
    QString timeoutStyle = QStringLiteral("QLabel { color: orange; font-weight: bold; }");
    m_authRestoreLabel->setText(tr("Unknown (timeout)"));
    m_authRestoreLabel->setStyleSheet(timeoutStyle);
    m_authClientLabel->setText(tr("Unknown (timeout)"));
    m_authClientLabel->setStyleSheet(timeoutStyle);
    m_authWhereLabel->setText(tr("Unknown (timeout)"));
    m_authWhereLabel->setStyleSheet(timeoutStyle);
    m_authStatusLabel->setText(tr("Permission check timed out — you can still proceed."));
    m_authStatusLabel->setStyleSheet("QLabel { color: orange; }");

    finishAuthChecks();
}

void BRestorePreviewPage::finishAuthChecks()
{
    m_authDone = true;
    m_authTimeoutTimer->stop();
    disconnectAuth();
    emit completeChanged();  // Enable the Commit button
}

void BRestorePreviewPage::disconnectAuth()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (wiz && wiz->director()) {
        disconnect(wiz->director(), &BDirector::jsonResult,
                   this, &BRestorePreviewPage::onHelpAllResponse);
        disconnect(wiz->director(), &BDirector::textResult,
                   this, &BRestorePreviewPage::onHelpAllResponse);
    }
}

void BRestorePreviewPage::setAuthLabel(QLabel *label, bool authorized, const QString &detail)
{
    if (authorized) {
        label->setText(tr("Authorized"));
        label->setStyleSheet("QLabel { color: green; font-weight: bold; }");
    } else {
        QString text = tr("Not authorized");
        if (!detail.isEmpty()) text += " — " + detail;
        label->setText(text);
        label->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    }
}

// ============================================================================
// Page 5: BRestoreExecutePage
// ============================================================================

BRestoreExecutePage::BRestoreExecutePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Execute Restore"));
    setSubTitle(tr("Sending commands to the Director..."));

    QVBoxLayout *layout = new QVBoxLayout(this);

    m_statusLabel = new QLabel(tr("Preparing..."), this);
    layout->addWidget(m_statusLabel);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0);  // Indeterminate
    layout->addWidget(m_progressBar);

    m_logEdit = new QTextEdit(this);
    m_logEdit->setReadOnly(true);
    m_logEdit->setFont(QFont("monospace"));
    layout->addWidget(m_logEdit);

    m_hintLabel = new QLabel(this);
    m_hintLabel->setWordWrap(true);
    m_hintLabel->setVisible(false);
    layout->addWidget(m_hintLabel);

    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    m_timeoutTimer->setInterval(COMMAND_TIMEOUT_MS);
    connect(m_timeoutTimer, &QTimer::timeout, this, &BRestoreExecutePage::onTimeout);
}

void BRestoreExecutePage::initializePage()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return;

    int step = wiz->pageIds().indexOf(wiz->currentId()) + 1;
    int total = wiz->pageIds().size();
    setSubTitle(tr("Step %1 of %2 — Sending commands to the Director...")
                    .arg(step).arg(total));

    m_state = Idle;
    m_logEdit->clear();
    m_restoreJobId.clear();
    m_hintLabel->setVisible(false);
    m_progressBar->setRange(0, 3);  // 3 steps: bvfs_restore, restore, bvfs_cleanup
    m_progressBar->setValue(0);
    m_progressBar->setFormat(tr("Step %v of %m"));

    // Reset BVFS model to free command queue
    wiz->bvfsModel()->resetModel();

    // Connect to Director responses
    if (wiz->director()) {
        connect(wiz->director(), &BDirector::jsonResult,
                this, &BRestoreExecutePage::onJsonResponse);
        connect(wiz->director(), &BDirector::textResult,
                this, &BRestoreExecutePage::onCommandResponse);
    }

    // Start execution
    sendBvfsRestore();
}

bool BRestoreExecutePage::isComplete() const
{
    return m_state == Completed || m_state == Failed;
}

void BRestoreExecutePage::sendBvfsRestore()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz || !wiz->director()) {
        markFailed(tr("No director connection"));
        return;
    }

    auto *data = wiz->wizardData();
    m_state = SendingBvfsRestore;
    m_progressBar->setValue(0);
    setStatus(tr("Creating restore table..."));
    appendLog(tr(">>> .bvfs_restore %1").arg(data->bvfsRestoreCommand));

    m_timeoutTimer->start();
    wiz->director()->doSend(BDirector::Command::BvfsRestore, data->bvfsRestoreCommand);
    data->restoreTableCreated = true;
}

void BRestoreExecutePage::sendRestore()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz || !wiz->director()) {
        markFailed(tr("No director connection"));
        return;
    }

    auto *data = wiz->wizardData();
    m_state = SendingRestore;
    m_progressBar->setValue(1);
    setStatus(tr("Executing restore command..."));
    appendLog(tr(">>> restore %1").arg(data->restoreCommand));

    m_timeoutTimer->start();
    wiz->director()->doSend(BDirector::Command::Restore, data->restoreCommand);
}

void BRestoreExecutePage::sendCleanup()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz || !wiz->director()) return;

    auto *data = wiz->wizardData();
    m_state = SendingCleanup;
    m_progressBar->setValue(2);
    setStatus(tr("Cleaning up restore table..."));

    QString cleanupArgs = QString("path=%1").arg(data->restoreTableName);
    appendLog(tr(">>> .bvfs_cleanup %1").arg(cleanupArgs));

    m_timeoutTimer->start();
    wiz->director()->doSend(BDirector::Command::BvfsCleanup, cleanupArgs);
}

void BRestoreExecutePage::onJsonResponse(BDirector::Command cmd, const QString &jsonData)
{
    if (m_state == SendingBvfsRestore && cmd == BDirector::Command::BvfsRestore) {
        m_timeoutTimer->stop();

        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject root = doc.object();

        QString errorMsg = extractJsonError(root);
        if (!errorMsg.isEmpty()) {
            markFailed(tr(".bvfs_restore failed: %1").arg(errorMsg));
            return;
        }

        appendLog(tr("Restore table created successfully."));
        sendRestore();
        return;
    }

    if (m_state == SendingRestore && cmd == BDirector::Command::Restore) {
        m_timeoutTimer->stop();

        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject root = doc.object();
        QJsonObject result = root["result"].toObject();

        RESTORE_DEBUG << "Restore JSON response: " << jsonData.left(500);

        QString errorMsg = extractJsonError(root);
        if (!errorMsg.isEmpty()) {
            appendLog(tr("Restore command failed: %1").arg(errorMsg), true);
            // Detect WhereACL restriction and give specific hint
            if (errorMsg.contains("where", Qt::CaseInsensitive) &&
                errorMsg.contains("not authorized", Qt::CaseInsensitive)) {
                appendLog(tr("Hint: The Bareos Director restricts the \"where\" path "
                             "via WhereACL. Ask your administrator to add the path \"%1\" "
                             "to the WhereACL of your console configuration, or use a "
                             "permitted restore path.")
                              .arg(qobject_cast<BRestoreWizard*>(wizard())->wizardData()->restoreWhere),
                          true);
            }
            sendCleanup();
            return;
        }

        // Try to extract Job ID from JSON response
        QString jobId = result["jobid"].toString();
        if (jobId.isEmpty()) jobId = root["jobid"].toString();
        if (jobId.isEmpty()) {
            // Search in run object: {"result":{"run":{"jobid":"123"}}}
            QJsonObject run = result["run"].toObject();
            if (!run.isEmpty()) jobId = run["jobid"].toString();
        }

        if (!jobId.isEmpty()) {
            m_restoreJobId = jobId;
            RESTORE_DEBUG << "Restore JobId (from JSON): " << m_restoreJobId;
            appendLog(tr("Restore job submitted successfully (Job ID: %1).").arg(jobId));
        } else {
            appendLog(tr("Restore command accepted."));
        }

        sendCleanup();
        return;
    }

    if (m_state == SendingCleanup && cmd == BDirector::Command::BvfsCleanup) {
        m_timeoutTimer->stop();
        markCompleted();
        return;
    }
}

void BRestoreExecutePage::onCommandResponse(BDirector::Command cmd, const QString &response)
{
    if (m_state == SendingRestore && cmd == BDirector::Command::Restore) {
        m_timeoutTimer->stop();
        appendLog(response.trimmed());

        // Check for error indicators in the text response
        if (response.contains("error", Qt::CaseInsensitive) ||
            response.contains("ERR=", Qt::CaseSensitive) ||
            response.contains("failed", Qt::CaseInsensitive)) {
            appendLog(tr("Restore command failed."), true);
            // Detect WhereACL restriction
            if (response.contains("where", Qt::CaseInsensitive) &&
                response.contains("not authorized", Qt::CaseInsensitive)) {
                auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
                appendLog(tr("Hint: The Bareos Director restricts the \"where\" path "
                             "via WhereACL. Ask your administrator to add the path \"%1\" "
                             "to the WhereACL of your console configuration, or use a "
                             "permitted restore path.")
                              .arg(wiz ? wiz->wizardData()->restoreWhere : QString()),
                          true);
            }
            // Still clean up the restore table even on failure
            sendCleanup();
            return;
        }

        if (response.contains("Job queued", Qt::CaseInsensitive) ||
            response.contains("OK", Qt::CaseInsensitive)) {
            appendLog(tr("Restore job submitted successfully."));

            // Extract Job ID from response (e.g. "Job queued. JobId=123")
            QRegularExpression re("JobId=(\\d+)", QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch match = re.match(response);
            if (match.hasMatch()) {
                m_restoreJobId = match.captured(1);
                RESTORE_DEBUG << "Restore JobId: " << m_restoreJobId;
            }
        } else {
            appendLog(tr("WARNING: Unexpected restore response."), true);
        }

        sendCleanup();
        return;
    }

    // bvfs_restore or bvfs_cleanup might also come as text
    if (m_state == SendingBvfsRestore && cmd == BDirector::Command::BvfsRestore) {
        m_timeoutTimer->stop();

        // Check for error in text response
        if (response.contains("error", Qt::CaseInsensitive) ||
            response.contains("ERR=", Qt::CaseSensitive)) {
            markFailed(tr(".bvfs_restore failed: %1").arg(response.trimmed()));
            return;
        }

        appendLog(tr("Restore table created (text response)."));
        sendRestore();
        return;
    }

    if (m_state == SendingCleanup && cmd == BDirector::Command::BvfsCleanup) {
        m_timeoutTimer->stop();
        markCompleted();
        return;
    }
}

void BRestoreExecutePage::markCompleted()
{
    m_timeoutTimer->stop();
    appendLog(tr("Cleanup complete."));

    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (wiz) {
        wiz->wizardData()->restoreTableCreated = false;
        wiz->wizardData()->restoreSucceeded = true;
    }

    m_state = Completed;
    m_progressBar->setValue(3);
    m_progressBar->setFormat(tr("Complete"));

    if (!m_restoreJobId.isEmpty()) {
        setStatus(tr("Restore job %1 submitted successfully!").arg(m_restoreJobId));
        m_hintLabel->setText(tr("The restore job (ID: %1) is now running on the Director.\n"
                                "You can monitor its progress in the Jobs view.\n"
                                "Refresh the job list to see the restore job status.")
                                 .arg(m_restoreJobId));
    } else {
        setStatus(tr("Restore completed."));
        m_hintLabel->setText(tr("The restore commands have been executed.\n"
                                "Check the log above for details."));
    }
    m_hintLabel->setVisible(true);

    emit completeChanged();
    disconnectDirector();
}

void BRestoreExecutePage::markFailed(const QString &reason)
{
    m_timeoutTimer->stop();
    appendLog(tr("ERROR: %1").arg(reason), true);

    m_state = Failed;
    m_progressBar->setFormat(tr("Failed"));
    setStatus(tr("Restore failed"));

    m_hintLabel->setText(reason);
    m_hintLabel->setVisible(true);

    emit completeChanged();
    disconnectDirector();
}

void BRestoreExecutePage::disconnectDirector()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (wiz && wiz->director()) {
        disconnect(wiz->director(), &BDirector::jsonResult,
                   this, &BRestoreExecutePage::onJsonResponse);
        disconnect(wiz->director(), &BDirector::textResult,
                   this, &BRestoreExecutePage::onCommandResponse);
    }
}

void BRestoreExecutePage::onTimeout()
{
    static const QMap<ExecutionState, QString> stateNames = {
        {SendingBvfsRestore, ".bvfs_restore"},
        {SendingRestore, "restore"},
        {SendingCleanup, ".bvfs_cleanup"}
    };

    QString cmdName = stateNames.value(m_state, tr("command"));
    markFailed(tr("Timeout waiting for %1 response (no response within %2 seconds)")
                   .arg(cmdName)
                   .arg(COMMAND_TIMEOUT_MS / 1000));
}

void BRestoreExecutePage::appendLog(const QString &msg, bool isError)
{
    if (isError) {
        m_logEdit->append(QString("<span style='color:red'>%1</span>").arg(msg.toHtmlEscaped()));
    } else {
        m_logEdit->append(msg);
    }
    RESTORE_DEBUG << (isError ? "ERROR: " : "") << msg;
}

void BRestoreExecutePage::setStatus(const QString &msg)
{
    m_statusLabel->setText(msg);
}
