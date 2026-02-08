#include "jobs/brestorewizard.h"
#include "blogging.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QRegularExpression>

#define RESTORE_DEBUG BLOG_DEBUG()

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
    m_data.sourceFileSet = job["fileset"].toString();
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
                  << " Client: " << m_data.sourceClient;

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

    connect(m_scopeGroup, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &BRestoreSelectJobPage::onScopeChanged);
}

void BRestoreSelectJobPage::initializePage()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return;
    auto *data = wiz->wizardData();

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
    if (!m_clientsLoaded && wiz->director()) {
        connect(wiz->director(), &BDirector::jsonResult,
                this, &BRestoreSelectJobPage::onClientsResponse);
        connect(wiz->director(), &BDirector::jsonResult,
                this, &BRestoreSelectJobPage::onFileSetsResponse);

        QMetaObject::invokeMethod(wiz->director(), "doSend",
                                  Qt::QueuedConnection,
                                  Q_ARG(BDirector::Command, BDirector::Command::DotClients),
                                  Q_ARG(QString, QString()));
        QMetaObject::invokeMethod(wiz->director(), "doSend",
                                  Qt::QueuedConnection,
                                  Q_ARG(BDirector::Command, BDirector::Command::DotFilesets),
                                  Q_ARG(QString, QString()));
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
    RESTORE_DEBUG << "Loaded " << m_fileSetCombo->count() << " filesets";

    if (wiz && wiz->director()) {
        disconnect(wiz->director(), &BDirector::jsonResult,
                   this, &BRestoreSelectJobPage::onFileSetsResponse);
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
    m_listView->horizontalHeader()->setStretchLastSection(true);
    m_listView->verticalHeader()->setVisible(false);

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

    m_infoLabel->setText(tr("Browse files — Client: %1, FileSet: %2")
                             .arg(data->selectedClient)
                             .arg(data->selectedFileSet));

    // Set up proxy for tree (dirs only)
    if (!m_dirProxy) {
        m_dirProxy = new BBvfsDirFilterProxy(this);
        m_dirProxy->setSourceModel(model);

        m_treeView->setModel(m_dirProxy);
        m_treeView->setHeaderHidden(false);
        // Hide non-name columns in tree
        for (int i = 1; i < BBvfsModel::ColCount; ++i) {
            m_treeView->setColumnHidden(i, true);
        }

        m_listView->setModel(model);

        connect(m_treeView, &QTreeView::clicked,
                this, &BRestoreBrowsePage::onTreeItemClicked);
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

void BRestoreBrowsePage::onTreeItemClicked(const QModelIndex &proxyIndex)
{
    if (!proxyIndex.isValid()) return;

    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return;
    auto *model = wiz->bvfsModel();

    QModelIndex srcIndex = m_dirProxy->mapToSource(proxyIndex);

    if (model->canFetchMore(srcIndex)) {
        model->fetchMore(srcIndex);
    }
    model->loadFilesForDirectory(srcIndex);
    m_listView->setRootIndex(srcIndex);
}

void BRestoreBrowsePage::onFileListDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;
    if (!index.data(BBvfsModel::IsDirectoryRole).toBool()) return;

    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return;
    auto *model = wiz->bvfsModel();

    QModelIndex srcIndex = index.sibling(index.row(), 0);

    QModelIndex proxyIndex = m_dirProxy->mapFromSource(srcIndex);
    if (proxyIndex.isValid()) {
        m_treeView->setCurrentIndex(proxyIndex);
        m_treeView->expand(proxyIndex);
    }

    if (model->canFetchMore(srcIndex)) {
        model->fetchMore(srcIndex);
    }
    model->loadFilesForDirectory(srcIndex);
    m_listView->setRootIndex(srcIndex);
}

void BRestoreBrowsePage::onLoadingStarted()
{
    m_progressBar->setVisible(true);
}

void BRestoreBrowsePage::onLoadingFinished()
{
    m_progressBar->setVisible(false);

    // Auto-expand first root item
    QModelIndex firstProxy = m_dirProxy->index(0, 0);
    if (firstProxy.isValid()) {
        m_treeView->expand(firstProxy);
        m_treeView->setCurrentIndex(firstProxy);
        onTreeItemClicked(firstProxy);
    }

    m_listView->resizeColumnsToContents();
}

void BRestoreBrowsePage::onSelectionCountChanged(int count)
{
    if (count == 0) {
        m_selectionLabel->setText(tr("No files selected"));
    } else {
        m_selectionLabel->setText(tr("%1 file(s)/directory/directories selected").arg(count));
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

    m_sourceLabel->setText(tr("Job %1: %2 — %3 file(s), %4 directory/directories selected")
                               .arg(data->jobId)
                               .arg(data->jobName)
                               .arg(data->selectedFileIds.size())
                               .arg(data->selectedDirIds.size()));

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

    m_commandPreview = new QTextEdit(this);
    m_commandPreview->setReadOnly(true);
    m_commandPreview->setFont(QFont("monospace"));
    layout->addWidget(m_commandPreview);

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
    parts << QString(".bvfs_restore path=%1").arg(data->restoreTableName);
    parts << QString("jobid=%1").arg(data->resolvedBvfsJobIds);
    if (!fileIds.isEmpty()) {
        parts << QString("fileid=%1").arg(fileIds);
    }
    if (!dirIds.isEmpty()) {
        parts << QString("dirid=%1").arg(dirIds);
    }
    data->bvfsRestoreCommand = parts.join(" ");

    // Build restore command
    data->restoreCommand = QString("restore file=?%1 client=%2 where=%3 replace=%4 done yes")
                               .arg(data->restoreTableName)
                               .arg(data->targetClient)
                               .arg(data->restoreWhere)
                               .arg(data->replacePolicy);

    // Update UI
    m_summaryLabel->setText(tr("Restore summary:\n"
                               "• %1 file(s) selected\n"
                               "• %2 directory/directories selected\n"
                               "• Target client: %3\n"
                               "• Restore to: %4\n"
                               "• Replace policy: %5")
                                .arg(data->selectedFileIds.size())
                                .arg(data->selectedDirIds.size())
                                .arg(data->targetClient)
                                .arg(data->restoreWhere)
                                .arg(data->replacePolicy));

    QString preview;
    preview += "# Step 1: Create restore table\n";
    preview += data->bvfsRestoreCommand + "\n\n";
    preview += "# Step 2: Execute restore\n";
    preview += data->restoreCommand + "\n\n";
    preview += "# Step 3: Cleanup (automatic)\n";
    preview += QString(".bvfs_cleanup path=%1\n").arg(data->restoreTableName);

    m_commandPreview->setPlainText(preview);

    RESTORE_DEBUG << "PreviewPage: table=" << data->restoreTableName;
    RESTORE_DEBUG << "  bvfs_restore: " << data->bvfsRestoreCommand;
    RESTORE_DEBUG << "  restore: " << data->restoreCommand;
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
}

void BRestoreExecutePage::initializePage()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz) return;

    m_state = Idle;
    m_logEdit->clear();

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
        appendLog(tr("ERROR: No director connection"), true);
        m_state = Failed;
        emit completeChanged();
        return;
    }

    auto *data = wiz->wizardData();
    m_state = SendingBvfsRestore;
    setStatus(tr("Creating restore table..."));
    appendLog(tr(">>> %1").arg(data->bvfsRestoreCommand));

    wiz->director()->doSend(BDirector::Command::BvfsRestore, data->bvfsRestoreCommand);
    data->restoreTableCreated = true;
}

void BRestoreExecutePage::sendRestore()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz || !wiz->director()) return;

    auto *data = wiz->wizardData();
    m_state = SendingRestore;
    setStatus(tr("Executing restore command..."));
    appendLog(tr(">>> %1").arg(data->restoreCommand));

    wiz->director()->doSend(BDirector::Command::Restore, data->restoreCommand);
}

void BRestoreExecutePage::sendCleanup()
{
    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (!wiz || !wiz->director()) return;

    auto *data = wiz->wizardData();
    m_state = SendingCleanup;
    setStatus(tr("Cleaning up restore table..."));

    QString cleanupArgs = QString("path=%1").arg(data->restoreTableName);
    appendLog(tr(">>> .bvfs_cleanup %1").arg(cleanupArgs));

    wiz->director()->doSend(BDirector::Command::BvfsCleanup, cleanupArgs);
}

void BRestoreExecutePage::onJsonResponse(BDirector::Command cmd, const QString &jsonData)
{
    Q_UNUSED(jsonData);

    if (m_state == SendingBvfsRestore && cmd == BDirector::Command::BvfsRestore) {
        appendLog(tr("Restore table created successfully."));
        sendRestore();
        return;
    }

    if (m_state == SendingCleanup && cmd == BDirector::Command::BvfsCleanup) {
        markCompleted();
        return;
    }
}

void BRestoreExecutePage::onCommandResponse(BDirector::Command cmd, const QString &response)
{
    if (m_state == SendingRestore && cmd == BDirector::Command::Restore) {
        appendLog(response.trimmed());

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
        appendLog(tr("Restore table created (text response)."));
        sendRestore();
        return;
    }

    if (m_state == SendingCleanup && cmd == BDirector::Command::BvfsCleanup) {
        markCompleted();
        return;
    }
}

void BRestoreExecutePage::markCompleted()
{
    appendLog(tr("Cleanup complete."));

    auto *wiz = qobject_cast<BRestoreWizard*>(wizard());
    if (wiz) wiz->wizardData()->restoreTableCreated = false;

    m_state = Completed;
    m_progressBar->setRange(0, 1);
    m_progressBar->setValue(1);

    if (!m_restoreJobId.isEmpty()) {
        setStatus(tr("Restore job %1 submitted successfully!").arg(m_restoreJobId));
        m_hintLabel->setText(tr("The restore job (ID: %1) is now running on the Director.\n"
                                "You can monitor its progress in the Jobs view.\n"
                                "Refresh the job list to see the restore job status.")
                                 .arg(m_restoreJobId));
    } else {
        setStatus(tr("Restore job submitted successfully!"));
        m_hintLabel->setText(tr("The restore job is now running on the Director.\n"
                                "You can monitor its progress in the Jobs view.\n"
                                "Refresh the job list to see the restore job status."));
    }
    m_hintLabel->setVisible(true);

    emit completeChanged();

    // Disconnect
    if (wiz && wiz->director()) {
        disconnect(wiz->director(), &BDirector::jsonResult,
                   this, &BRestoreExecutePage::onJsonResponse);
        disconnect(wiz->director(), &BDirector::textResult,
                   this, &BRestoreExecutePage::onCommandResponse);
    }
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
