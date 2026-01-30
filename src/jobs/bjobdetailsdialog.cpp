#include "jobs/bjobdetailsdialog.h"
#include "jobs/bjobwidget.h"
#include <QGroupBox>
#include <QHeaderView>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QIcon>
#include <QStandardItem>
#include <QTimer>

BJobDetailsDialog::BJobDetailsDialog(const QJsonObject &job, BJobWidget *jobWidget, BDirector *director, QWidget *parent)
    : QDialog(parent)
    , m_job(job)
    , m_jobWidget(jobWidget)
    , m_director(director)
    , m_jobId(job["jobid"].toString().toULongLong())
    , m_currentPath("/")
    , m_currentPathId(0)
    , m_clientName(job["client"].toString())
    , m_bvfsState(BvfsState::Idle)
{
    setupUi(job);

    // Erstelle aussagekräftigen Titel mit Job-Name, ID und Client
    QString jobName = job["name"].toString();
    QString jobId = job["jobid"].toString();
    QString client = job["client"].toString();

    setWindowTitle(QString("Auftrags-Details: %1 (ID: %2) - Client: %3")
                       .arg(jobName)
                       .arg(jobId)
                       .arg(client));

    resize(900, 600);

    // Connect to Director signals
    if (m_director) {
        connect(m_director, &BDirector::jsonResponse,
                this, &BJobDetailsDialog::onFilesDataReceived);
        // Note: Job log is now handled by BJobWidget, no need to load it here

        // Load files from Director
        loadFilesFromDirector();
    }
}

void BJobDetailsDialog::setupUi(const QJsonObject &job)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create tab widget
    m_tabWidget = new QTabWidget(this);

    // Setup Files Tab
    setupFilesTab();

    // Setup Status Tab
    setupStatusTab(job);

    // Setup Log Tab
    setupLogTab();

    mainLayout->addWidget(m_tabWidget);

    // Button box
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    mainLayout->addWidget(buttonBox);
}

void BJobDetailsDialog::setupFilesTab()
{
    QWidget *filesTab = new QWidget();
    QVBoxLayout *filesLayout = new QVBoxLayout(filesTab);

    // Toolbar with options
    QHBoxLayout *toolbarLayout = new QHBoxLayout();

    // Checkbox: Show files from all related jobs (full restore chain)
    m_allJobsCheckBox = new QCheckBox(tr("Show files from all related jobs (full restore chain)"), filesTab);
    m_allJobsCheckBox->setToolTip(tr("When checked, shows files from all incremental/differential backups.\n"
                                      "When unchecked, shows only files from this specific job."));
    m_allJobsCheckBox->setChecked(true);  // Default: show all jobs for complete view
    connect(m_allJobsCheckBox, &QCheckBox::toggled,
            this, &BJobDetailsDialog::onAllJobsToggled);
    toolbarLayout->addWidget(m_allJobsCheckBox);
    toolbarLayout->addStretch();

    filesLayout->addLayout(toolbarLayout);

    // Loading indicator
    m_loadingProgress = new QProgressBar(filesTab);
    m_loadingProgress->setRange(0, 0);  // Indeterminate
    m_loadingProgress->setTextVisible(true);
    m_loadingProgress->setFormat("Loading files...");
    filesLayout->addWidget(m_loadingProgress);

    // Splitter for tree view and file list
    m_filesSplitter = new QSplitter(Qt::Horizontal, filesTab);

    // Tree view for directory structure
    m_fileTreeView = new QTreeView(m_filesSplitter);
    m_treeModel = new QStandardItemModel(this);
    m_treeModel->setHorizontalHeaderLabels({"Directory Structure"});
    m_fileTreeView->setModel(m_treeModel);
    m_fileTreeView->setHeaderHidden(false);
    m_fileTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_fileTreeView, &QTreeView::clicked,
            this, &BJobDetailsDialog::onTreeItemClicked);

    // Table view for file list
    m_fileListView = new QTableView(m_filesSplitter);
    m_fileListModel = new QStandardItemModel(this);
    m_fileListModel->setHorizontalHeaderLabels({"Name", "Size", "Type", "Modified"});
    m_fileListView->setModel(m_fileListModel);
    m_fileListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_fileListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_fileListView->horizontalHeader()->setStretchLastSection(true);
    m_fileListView->verticalHeader()->setVisible(false);

    // Set splitter sizes (30% tree, 70% list)
    m_filesSplitter->addWidget(m_fileTreeView);
    m_filesSplitter->addWidget(m_fileListView);
    m_filesSplitter->setSizes({250, 550});

    filesLayout->addWidget(m_filesSplitter);

    m_tabWidget->addTab(filesTab, "Files");
}

void BJobDetailsDialog::setupStatusTab(const QJsonObject &job)
{
    m_statusWidget = new QWidget();
    QVBoxLayout *statusLayout = new QVBoxLayout(m_statusWidget);

    // Job Information Group
    QGroupBox *jobInfoGroup = new QGroupBox("Job Information", m_statusWidget);
    QFormLayout *jobInfoLayout = new QFormLayout(jobInfoGroup);

    jobInfoLayout->addRow("Job ID:", new QLabel(job["jobid"].toString()));
    jobInfoLayout->addRow("Job Name:", new QLabel(job["name"].toString()));
    jobInfoLayout->addRow("Client:", new QLabel(job["client"].toString()));

    // Backup Type and Level
    QString type = job["type"].toString();
    QString typeDesc = (type == "B") ? "Backup" : type;
    jobInfoLayout->addRow("Type:", new QLabel(typeDesc));

    QString level = job["level"].toString();
    QString levelDesc;
    if (level == "F") levelDesc = "Full";
    else if (level == "I") levelDesc = "Incremental";
    else if (level == "D") levelDesc = "Differential";
    else levelDesc = level;
    jobInfoLayout->addRow("Level:", new QLabel(levelDesc));

    // FileSet
    QString fileset = job["fileset"].toString();
    if (!fileset.isEmpty()) {
        jobInfoLayout->addRow("FileSet:", new QLabel(fileset));
    }

    // Scheduler
    QString schedname = job["schedname"].toString();
    if (!schedname.isEmpty()) {
        jobInfoLayout->addRow("Schedule:", new QLabel(schedname));
    }

    statusLayout->addWidget(jobInfoGroup);

    // Timing Information Group
    QGroupBox *timingGroup = new QGroupBox("Timing", m_statusWidget);
    QFormLayout *timingLayout = new QFormLayout(timingGroup);

    timingLayout->addRow("Start Time:", new QLabel(job["starttime"].toString()));
    timingLayout->addRow("Duration:", new QLabel(job["duration"].toString()));

    statusLayout->addWidget(timingGroup);

    // Statistics Group
    QGroupBox *statsGroup = new QGroupBox("Statistics", m_statusWidget);
    QFormLayout *statsLayout = new QFormLayout(statsGroup);

    QString filesCount = job["jobfiles"].toString();
    statsLayout->addRow("Files Backed Up:", new QLabel(filesCount));

    qint64 bytes = job["jobbytes"].toString().toLongLong();
    QString bytesFormatted = formatBytes(bytes);
    QString bytesExact = QString("%1 bytes").arg(job["jobbytes"].toString());
    QLabel *bytesLabel = new QLabel(bytesFormatted);
    bytesLabel->setToolTip(bytesExact);
    statsLayout->addRow("Data Size:", bytesLabel);

    statusLayout->addWidget(statsGroup);

    // Status Group
    QGroupBox *statusGroup = new QGroupBox("Status", m_statusWidget);
    QFormLayout *statusGroupLayout = new QFormLayout(statusGroup);

    QString status = job["jobstatus"].toString();
    QString statusDesc = formatStatus(status);
    QString statusColor = getStatusColor(status);

    QLabel *statusLabel = new QLabel(statusDesc);
    statusLabel->setStyleSheet(QString("QLabel { "
                                       "background-color: %1; "
                                       "padding: 5px; "
                                       "border-radius: 3px; "
                                       "font-weight: bold; "
                                       "}").arg(statusColor));
    statusGroupLayout->addRow("Job Status:", statusLabel);

    statusLayout->addWidget(statusGroup);

    statusLayout->addStretch();  // Push content to the top

    m_tabWidget->addTab(m_statusWidget, "Status");
}

void BJobDetailsDialog::setupLogTab()
{
    QWidget *logWidget = new QWidget();
    QVBoxLayout *logLayout = new QVBoxLayout(logWidget);

    // Job Log List View - uses shared model from BJobWidget
    m_logListView = new QListView(logWidget);

    // Use the shared log model from BJobWidget (data already loaded)
    if (m_jobWidget && m_jobWidget->logModel()) {
        m_logListView->setModel(m_jobWidget->logModel());
    }

    m_logListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_logListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_logListView->setAlternatingRowColors(true);

    logLayout->addWidget(m_logListView);

    m_tabWidget->addTab(logWidget, "Logs");
}

QString BJobDetailsDialog::formatBytes(qint64 bytes) const
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    const qint64 TB = GB * 1024;
    
    if (bytes >= TB) {
        return QString("%1 TB").arg(bytes / (double)TB, 0, 'f', 2);
    } else if (bytes >= GB) {
        return QString("%1 GB").arg(bytes / (double)GB, 0, 'f', 2);
    } else if (bytes >= MB) {
        return QString("%1 MB").arg(bytes / (double)MB, 0, 'f', 2);
    } else if (bytes >= KB) {
        return QString("%1 KB").arg(bytes / (double)KB, 0, 'f', 2);
    } else {
        return QString("%1 B").arg(bytes);
    }
}

QString BJobDetailsDialog::formatStatus(const QString &status) const
{
    if (status == "T") return "Terminated normally ✓";
    if (status == "W") return "Terminated with warnings ⚠";
    if (status == "f") return "Failed ✗";
    if (status == "E") return "Terminated in Error ✗";
    if (status == "e") return "Non-fatal error ⚠";
    if (status == "A") return "Canceled by user";
    if (status == "R") return "Running...";
    if (status == "C") return "Created";
    
    return QString("Status: %1").arg(status);
}

QString BJobDetailsDialog::getStatusColor(const QString &status) const
{
    if (status == "T") return "#c8ffc8";  // Light green
    if (status == "W") return "#ffffc8";  // Light yellow
    if (status == "f" || status == "E") return "#ffc8c8";  // Light red

    return "#e0e0e0";  // Light gray
}

void BJobDetailsDialog::loadFilesFromDirector()
{
    if (!m_director) {
        qWarning() << "BVFS: No Director connection available";
        m_loadingProgress->setFormat("No connection to Director");
        return;
    }

    bool useAllJobs = m_allJobsCheckBox->isChecked();

    qDebug() << "========================================";
    qDebug() << "BVFS: Starting file load for JobID:" << m_jobId;
    qDebug() << "BVFS: Client:" << m_clientName;
    qDebug() << "BVFS: Mode:" << (useAllJobs ? "All related jobs" : "Current job only");
    qDebug() << "========================================";

    if (useAllJobs) {
        // Mode: Show all related jobs (full restore chain)
        // Step 1: Get all related jobids (for incremental/differential backups)
        m_bvfsState = BvfsState::GettingJobIds;
        QString getJobIdsCmd = QString(".bvfs_get_jobids jobid=%1 all").arg(m_jobId);
        qDebug() << "BVFS: Step 1 - Sending command:" << getJobIdsCmd;

        QMetaObject::invokeMethod(m_director, "doSendCommand",
                                  Qt::QueuedConnection,
                                  Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                  Q_ARG(QString, getJobIdsCmd));
        // Steps 2 & 3 will be triggered in onFilesDataReceived()
    } else {
        // Mode: Show only current job files
        // Skip bvfs_get_jobids and use only this job's ID
        m_bvfsJobIds = QString::number(m_jobId);
        qDebug() << "BVFS: Using single job ID:" << m_bvfsJobIds;

        // Step 2: Update BVFS cache
        m_bvfsState = BvfsState::UpdatingCache;
        QString updateCmd = QString(".bvfs_update jobid=%1").arg(m_bvfsJobIds);
        qDebug() << "BVFS: Step 2 - Sending command:" << updateCmd;

        QMetaObject::invokeMethod(m_director, "doSendCommand",
                                  Qt::QueuedConnection,
                                  Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                  Q_ARG(QString, updateCmd));

        // Step 3: List directories (triggered after short delay)
        QTimer::singleShot(300, this, [this]() {
            m_bvfsState = BvfsState::ListingDirs;
            QString lsdirsCmd = QString(".bvfs_lsdirs jobid=%1 path=/").arg(m_bvfsJobIds);
            qDebug() << "BVFS: Step 3 - Sending command:" << lsdirsCmd;

            QMetaObject::invokeMethod(m_director, "doSendCommand",
                                      Qt::QueuedConnection,
                                      Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                      Q_ARG(QString, lsdirsCmd));
        });
    }
}

void BJobDetailsDialog::onFilesDataReceived(const QString &command, const QString &jsonData)
{
    qDebug() << "BVFS: onFilesDataReceived called";
    qDebug() << "BVFS:   Command:" << command;
    qDebug() << "BVFS:   Data size:" << jsonData.size() << "bytes";
    qDebug() << "BVFS:   State:" << static_cast<int>(m_bvfsState);

    // Parse JSON
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "BVFS: JSON parse error:" << error.errorString();
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "BVFS: Response is not a JSON object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    // Step 1: Process bvfs_get_jobids response
    if (command.contains("bvfs_get_jobids") && m_bvfsState == BvfsState::GettingJobIds) {
        qDebug() << "BVFS: ✓ Step 1 - Processing bvfs_get_jobids response";

        // Bareos returns: {"result": {"jobids": [{"id": "123"}, {"id": "456"}, ...]}}
        QJsonArray jobidsArray = result["jobids"].toArray();
        QStringList jobIdList;

        if (!jobidsArray.isEmpty()) {
            for (const QJsonValue &val : jobidsArray) {
                QJsonObject obj = val.toObject();
                QString id = obj["id"].toString();
                if (!id.isEmpty()) {
                    jobIdList << id;
                }
            }
        }

        // Fallback to original job ID if no results
        if (jobIdList.isEmpty()) {
            jobIdList << QString::number(m_jobId);
        }

        m_bvfsJobIds = jobIdList.join(",");
        qDebug() << "BVFS: ✓ Got BVFS JobIDs:" << m_bvfsJobIds;

        // Step 2: Update BVFS cache with all jobids
        m_bvfsState = BvfsState::UpdatingCache;
        QString updateCmd = QString(".bvfs_update jobid=%1").arg(m_bvfsJobIds);
        qDebug() << "BVFS: Step 2 - Sending command:" << updateCmd;

        QMetaObject::invokeMethod(m_director, "doSendCommand",
                                  Qt::QueuedConnection,
                                  Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                  Q_ARG(QString, updateCmd));

        // Step 3: List directories (triggered after short delay)
        QTimer::singleShot(300, this, [this]() {
            m_bvfsState = BvfsState::ListingDirs;
            QString lsdirsCmd = QString(".bvfs_lsdirs jobid=%1 path=/").arg(m_bvfsJobIds);
            qDebug() << "BVFS: Step 3 - Sending command:" << lsdirsCmd;

            QMetaObject::invokeMethod(m_director, "doSendCommand",
                                      Qt::QueuedConnection,
                                      Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                      Q_ARG(QString, lsdirsCmd));
        });
        return;
    }

    // Step 3: Process bvfs_lsdirs response (directory structure)
    if (command.contains("bvfs_lsdirs") && m_bvfsState == BvfsState::ListingDirs) {
        qDebug() << "BVFS: ✓ Step 3 - Processing bvfs_lsdirs response";

        QJsonArray dirsArray = result["directories"].toArray();
        qDebug() << "BVFS: Found" << dirsArray.size() << "directories";

        if (dirsArray.isEmpty()) {
            m_loadingProgress->setFormat("No directories found for this job");
            qDebug() << "BVFS: No directories - this job may not have file entries in catalog";
        } else {
            populateFileTree(dirsArray);
        }

        m_loadingProgress->setVisible(false);
        m_bvfsState = BvfsState::Idle;
        return;
    }

    // Process bvfs_lsfiles response (file listing for selected directory)
    if (command.contains("bvfs_lsfiles") && m_bvfsState == BvfsState::ListingFiles) {
        qDebug() << "BVFS: Processing bvfs_lsfiles response";

        QJsonArray filesArray = result["files"].toArray();
        qDebug() << "BVFS: Found" << filesArray.size() << "files";

        populateFileList(filesArray);
        m_bvfsState = BvfsState::Idle;
        return;
    }
}


void BJobDetailsDialog::populateFileTree(const QJsonArray &dirsArray)
{
    qDebug() << "BVFS: Populating file tree with" << dirsArray.size() << "directories";

    m_treeModel->clear();
    m_treeModel->setHorizontalHeaderLabels({"Directory Structure"});

    if (dirsArray.isEmpty()) {
        QStandardItem *rootItem = new QStandardItem("No directories found");
        m_treeModel->appendRow(rootItem);
        return;
    }

    // Build directory tree structure from BVFS lsdirs response
    QMap<QString, QStandardItem*> pathMap;
    QStandardItem *rootItem = new QStandardItem("/");
    rootItem->setData("/", Qt::UserRole);      // Store full path
    rootItem->setData(0, Qt::UserRole + 1);    // PathId 0 for root
    rootItem->setIcon(QIcon::fromTheme("folder"));
    m_treeModel->appendRow(rootItem);
    pathMap["/"] = rootItem;

    // Process each directory from BVFS response
    // BVFS lsdirs returns (lowercase): pathid, fileid, name, jobid, lstat, fileindex
    for (const QJsonValue &dirVal : dirsArray) {
        QJsonObject dirObj = dirVal.toObject();

        // Note: Bareos returns lowercase field names
        QString dirName = dirObj["name"].toString();
        int pathId = dirObj["pathid"].toString().toInt();

        // Skip . and .. directories
        if (dirName == "." || dirName == "..") {
            continue;
        }

        if (dirName.isEmpty()) {
            qDebug() << "BVFS: Skipping directory with no name:" << dirObj;
            continue;
        }

        qDebug() << "BVFS: Processing directory:" << dirName << "PathId:" << pathId;

        // dirName is typically the full path like "/home/user/documents/"
        // Remove trailing slash if present
        if (dirName.endsWith('/') && dirName != "/") {
            dirName.chop(1);
        }

        // Split path into components
        QStringList pathParts = dirName.split('/', Qt::SkipEmptyParts);

        QString currentPath = "";
        QStandardItem *parentItem = rootItem;

        // Build directory structure
        for (int i = 0; i < pathParts.size(); ++i) {
            QString part = pathParts[i];
            currentPath += "/" + part;

            if (!pathMap.contains(currentPath)) {
                QStandardItem *item = new QStandardItem(part);
                item->setData(currentPath, Qt::UserRole);  // Store full path
                item->setIcon(QIcon::fromTheme("folder"));

                // Store BVFS PathId for later file queries (only for the leaf item)
                if (i == pathParts.size() - 1) {
                    item->setData(pathId, Qt::UserRole + 1);
                    qDebug() << "BVFS: Set PathId" << pathId << "for" << currentPath;
                }

                parentItem->appendRow(item);
                pathMap[currentPath] = item;
                parentItem = item;
            } else {
                parentItem = pathMap[currentPath];
                // Update PathId if this is the leaf and we have a valid pathId
                if (i == pathParts.size() - 1 && pathId > 0) {
                    parentItem->setData(pathId, Qt::UserRole + 1);
                }
            }
        }
    }

    m_fileTreeView->expandToDepth(1);

    // Auto-select root to load initial file list
    QModelIndex rootIndex = m_treeModel->index(0, 0);
    if (rootIndex.isValid()) {
        m_fileTreeView->setCurrentIndex(rootIndex);
        onTreeItemClicked(rootIndex);
    }
}

void BJobDetailsDialog::onTreeItemClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    QStandardItem *item = m_treeModel->itemFromIndex(index);
    if (!item) {
        return;
    }

    QString selectedPath = item->data(Qt::UserRole).toString();
    int pathId = item->data(Qt::UserRole + 1).toInt();
    qDebug() << "Tree item clicked:" << selectedPath << "PathId:" << pathId;

    m_currentPath = selectedPath;
    m_currentPathId = pathId;

    // Load files for this directory via BVFS API
    loadFilesForDirectory(pathId, selectedPath);
}

void BJobDetailsDialog::onAllJobsToggled(bool checked)
{
    qDebug() << "BVFS: All jobs toggle changed to:" << checked;

    // Clear current data and reload
    m_treeModel->clear();
    m_treeModel->setHorizontalHeaderLabels({"Directory Structure"});
    m_fileListModel->clear();
    m_fileListModel->setHorizontalHeaderLabels({"Name", "Size", "Type", "Modified"});

    m_loadingProgress->setVisible(true);
    m_loadingProgress->setFormat("Loading files...");

    // Reload with new job selection
    loadFilesFromDirector();
}

void BJobDetailsDialog::loadFilesForDirectory(int pathId, const QString &path)
{
    if (!m_director) {
        qWarning() << "BVFS: No Director connection for file loading";
        return;
    }

    // Determine which job IDs to use based on checkbox state
    QString jobIds;
    if (m_allJobsCheckBox->isChecked()) {
        // Use all related jobs (full restore chain)
        jobIds = m_bvfsJobIds;
    } else {
        // Use only the current job
        jobIds = QString::number(m_jobId);
    }

    qDebug() << "BVFS: Loading files for path:" << path << "PathId:" << pathId << "JobIds:" << jobIds;

    // Clear current file list
    m_fileListModel->clear();
    m_fileListModel->setHorizontalHeaderLabels({"Name", "Size", "Type", "Modified"});

    // First add subdirectories from tree model
    QModelIndex currentIndex = m_fileTreeView->currentIndex();
    if (currentIndex.isValid()) {
        QStandardItem *item = m_treeModel->itemFromIndex(currentIndex);
        if (item) {
            int childCount = item->rowCount();
            for (int i = 0; i < childCount; ++i) {
                QStandardItem *childItem = item->child(i);
                QString childName = childItem->text();

                QList<QStandardItem*> row;
                QStandardItem *nameItem = new QStandardItem(childName);
                nameItem->setIcon(QIcon::fromTheme("folder"));
                row << nameItem;
                row << new QStandardItem("-");  // Size
                row << new QStandardItem(tr("Directory"));
                row << new QStandardItem("-");  // Modified

                m_fileListModel->appendRow(row);
            }
        }
    }

    // Now load actual files via BVFS
    m_bvfsState = BvfsState::ListingFiles;

    // Use path-based query for files in this directory
    QString lsfilesCmd = QString(".bvfs_lsfiles jobid=%1 path=%2")
                             .arg(jobIds)
                             .arg(path);
    qDebug() << "BVFS: Sending command:" << lsfilesCmd;

    QMetaObject::invokeMethod(m_director, "doSendCommand",
                              Qt::QueuedConnection,
                              Q_ARG(BDirector::Command, BDirector::Command::Custom),
                              Q_ARG(QString, lsfilesCmd));
}

void BJobDetailsDialog::populateFileList(const QJsonArray &filesArray)
{
    qDebug() << "BVFS: Populating file list with" << filesArray.size() << "files";

    // Files are appended after directories (which were added in loadFilesForDirectory)
    for (const QJsonValue &fileVal : filesArray) {
        QJsonObject fileObj = fileVal.toObject();

        // BVFS lsfiles returns (lowercase): fileid, pathid, name, jobid, lstat, fileindex, stat
        QString fileName = fileObj["name"].toString();
        if (fileName.isEmpty() || fileName == "." || fileName == "..") {
            continue;
        }

        // Parse lstat for file size and mtime
        // lstat format is base64 encoded stat structure, but Bareos also provides "stat" object
        QJsonObject stat = fileObj["stat"].toObject();
        qint64 fileSize = stat["size"].toVariant().toLongLong();
        qint64 mtime = stat["mtime"].toVariant().toLongLong();

        // Format file size
        QString sizeStr = formatBytes(fileSize);

        // Format modification time
        QString mtimeStr;
        if (mtime > 0) {
            QDateTime dateTime = QDateTime::fromSecsSinceEpoch(mtime);
            mtimeStr = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        }

        // Determine file type from stat mode
        QString fileType = tr("File");
        int mode = stat["mode"].toInt();
        if (mode > 0) {
            // S_IFDIR = 0040000 (directory)
            // S_IFREG = 0100000 (regular file)
            // S_IFLNK = 0120000 (symlink)
            if ((mode & 0170000) == 0040000) {
                fileType = tr("Directory");
            } else if ((mode & 0170000) == 0120000) {
                fileType = tr("Symlink");
            }
        }

        QList<QStandardItem*> row;
        QStandardItem *nameItem = new QStandardItem(fileName);
        if (fileType == tr("Directory")) {
            nameItem->setIcon(QIcon::fromTheme("folder"));
        } else if (fileType == tr("Symlink")) {
            nameItem->setIcon(QIcon::fromTheme("emblem-symbolic-link"));
        } else {
            nameItem->setIcon(QIcon::fromTheme("text-x-generic"));
        }
        row << nameItem;
        row << new QStandardItem(sizeStr);
        row << new QStandardItem(fileType);
        row << new QStandardItem(mtimeStr);

        // Store file metadata in first item for potential restore selection
        nameItem->setData(fileObj["fileid"].toString(), Qt::UserRole);
        nameItem->setData(fileObj["jobid"].toString(), Qt::UserRole + 1);

        m_fileListModel->appendRow(row);
    }

    // Resize columns to content
    m_fileListView->resizeColumnsToContents();
}
