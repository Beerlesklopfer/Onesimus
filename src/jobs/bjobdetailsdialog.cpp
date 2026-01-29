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

    qDebug() << "========================================";
    qDebug() << "BVFS: Starting file load for JobID:" << m_jobId;
    qDebug() << "========================================";

    // BVFS API correct sequence:
    // Step 1: Get all related jobids (for incremental/differential backups)
    QString getJobIdsCmd = QString(".bvfs_get_jobids jobid=%1").arg(m_jobId);
    qDebug() << "BVFS: Step 1 - Sending command:" << getJobIdsCmd;

    // Thread-safe: Use queued connection to send command to Director thread
    QMetaObject::invokeMethod(m_director, "doSendCommand",
                              Qt::QueuedConnection,
                              Q_ARG(BDirector::Command, BDirector::Command::Custom),
                              Q_ARG(QString, getJobIdsCmd));

    // Steps 2 & 3 will be triggered in onFilesDataReceived() after we receive the jobids

    // Note: Job log is no longer loaded here - it's managed by BJobWidget
}

void BJobDetailsDialog::onFilesDataReceived(const QString &command, const QString &jsonData)
{
    qDebug() << "BVFS: onFilesDataReceived called";
    qDebug() << "BVFS:   Command:" << command;
    qDebug() << "BVFS:   Data size:" << jsonData.size() << "bytes";
    qDebug() << "BVFS:   Looking for JobID:" << m_jobId;

    // Only process if this is for our job
    if (!command.contains(QString::number(m_jobId))) {
        qDebug() << "BVFS:   → Not for this job, ignoring";
        return;
    }

    // Step 1: Process bvfs_get_jobids response
    if (command.contains("bvfs_get_jobids")) {
        qDebug() << "BVFS: ✓ Step 1 - Processing bvfs_get_jobids response";
        qDebug() << "BVFS:   First 500 chars:" << jsonData.left(500);

        // Parse JSON to extract jobids
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

        if (error.error != QJsonParseError::NoError) {
            qWarning() << "BVFS: ✗ JSON parse error for jobids:" << error.errorString();
            qWarning() << "BVFS:   Offset:" << error.offset;
            qWarning() << "BVFS:   Near:" << jsonData.mid(qMax(0, error.offset - 50), 100);
            m_loadingProgress->setFormat("Error getting job IDs");
            return;
        }

        qDebug() << "BVFS:   JSON parsed successfully";
        qDebug() << "BVFS:   Root keys:" << doc.object().keys();

        // Extract jobids from response
        // Expected format: {"result": {"jobids": "12,11,10"}} or similar
        if (doc.isObject()) {
            QJsonObject root = doc.object();
            if (root.contains("result") && root["result"].isObject()) {
                QJsonObject result = root["result"].toObject();
                qDebug() << "BVFS:   Result keys:" << result.keys();

                if (result.contains("jobids")) {
                    m_bvfsJobIds = result["jobids"].toString();
                    qDebug() << "BVFS: ✓ Got BVFS JobIDs:" << m_bvfsJobIds;

                    // Step 2: Update BVFS cache with all jobids
                    QString updateCmd = QString(".bvfs_update jobid=%1").arg(m_bvfsJobIds);
                    qDebug() << "BVFS: Step 2 - Sending command:" << updateCmd;

                    QMetaObject::invokeMethod(m_director, "doSendCommand",
                                              Qt::QueuedConnection,
                                              Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                              Q_ARG(QString, updateCmd));

                    // Step 3: List directories (triggered after short delay)
                    QTimer::singleShot(300, this, [this]() {
                        QString lsdirsCmd = QString(".bvfs_lsdirs jobid=%1 path=/").arg(m_bvfsJobIds);
                        qDebug() << "BVFS: Step 3 - Sending command:" << lsdirsCmd;

                        QMetaObject::invokeMethod(m_director, "doSendCommand",
                                                  Qt::QueuedConnection,
                                                  Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                                  Q_ARG(QString, lsdirsCmd));
                    });
                } else {
                    qWarning() << "BVFS: ✗ No 'jobids' field in result";
                }
            } else {
                qWarning() << "BVFS: ✗ No 'result' object in response";
            }
        } else {
            qWarning() << "BVFS: ✗ Response is not a JSON object";
        }
        return;
    }

    // Step 3: Process bvfs_lsdirs response (directory structure)
    if (command.contains("bvfs_lsdirs")) {
        qDebug() << "BVFS: ✓ Step 3 - Processing bvfs_lsdirs response";
        qDebug() << "BVFS:   First 500 chars:" << jsonData.left(500);

        // Parse JSON
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

        if (error.error != QJsonParseError::NoError) {
            qWarning() << "BVFS: ✗ JSON parse error for directories:" << error.errorString();
            qWarning() << "BVFS:   Offset:" << error.offset;
            qWarning() << "BVFS:   Near:" << jsonData.mid(qMax(0, error.offset - 50), 100);
            m_loadingProgress->setFormat("Error parsing directory data");
            return;
        }

        qDebug() << "BVFS:   JSON parsed successfully";
        qDebug() << "BVFS:   Root keys:" << doc.object().keys();

        QJsonArray dirsArray;
        if (doc.isObject()) {
            QJsonObject root = doc.object();
            // Bvfs API response structure
            if (root.contains("result") && root["result"].isObject()) {
                QJsonObject result = root["result"].toObject();
                qDebug() << "BVFS:   Result keys:" << result.keys();

                if (result.contains("directories") && result["directories"].isArray()) {
                    dirsArray = result["directories"].toArray();
                    qDebug() << "BVFS: ✓ Found" << dirsArray.size() << "directories in result.directories";
                } else {
                    qWarning() << "BVFS: ✗ No 'directories' array in result";
                }
            } else if (root.contains("directories")) {
                dirsArray = root["directories"].toArray();
                qDebug() << "BVFS: ✓ Found" << dirsArray.size() << "directories (direct)";
            } else {
                qWarning() << "BVFS: ✗ No 'result' or 'directories' in response";
            }
        } else {
            qWarning() << "BVFS: ✗ Response is not a JSON object";
        }

        if (dirsArray.isEmpty()) {
            qWarning() << "BVFS: ✗ No directories found in response";
            qDebug() << "BVFS:   Full JSON:" << jsonData;
            m_loadingProgress->setFormat("No directories found for this job");
        } else {
            qDebug() << "BVFS: Populating file tree with" << dirsArray.size() << "directories";
            populateFileTree(dirsArray);
        }
        m_loadingProgress->setVisible(false);
        return;
    }

    // Process bvfs_lsfiles response (if needed for file details)
    if (command.contains("bvfs_lsfiles")) {
        qDebug() << "Received files data:" << jsonData.left(200);
        // TODO: Handle file listing when user clicks on a directory
        return;
    }
}


void BJobDetailsDialog::populateFileTree(const QJsonArray &dirsArray)
{
    qDebug() << "Populating file tree with" << dirsArray.size() << "directories";

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
    rootItem->setData("/", Qt::UserRole);  // Store full path
    rootItem->setIcon(QIcon::fromTheme("folder"));
    m_treeModel->appendRow(rootItem);
    pathMap["/"] = rootItem;

    // Process each directory from BVFS response
    for (const QJsonValue &dirVal : dirsArray) {
        QJsonObject dirObj = dirVal.toObject();

        // BVFS lsdirs returns: PathId, FilenameId, Name, JobId, LStat
        QString dirName = dirObj["Name"].toString();

        if (dirName.isEmpty()) {
            qDebug() << "Skipping directory with no name:" << dirObj;
            continue;
        }

        qDebug() << "Processing directory:" << dirName;

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

                // Store BVFS PathId for later file queries
                if (dirObj.contains("PathId")) {
                    item->setData(dirObj["PathId"].toInt(), Qt::UserRole + 1);
                }

                parentItem->appendRow(item);
                pathMap[currentPath] = item;
                parentItem = item;
            } else {
                parentItem = pathMap[currentPath];
            }
        }
    }

    m_fileTreeView->expandToDepth(1);
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
    qDebug() << "Tree item clicked:" << selectedPath;

    // Clear current file list
    m_fileListModel->clear();
    m_fileListModel->setHorizontalHeaderLabels({"Name", "Size", "Type", "Modified"});

    // Find all files/directories that are direct children of selected path
    QStandardItem *rootItem = item;
    int childCount = rootItem->rowCount();

    for (int i = 0; i < childCount; ++i) {
        QStandardItem *childItem = rootItem->child(i);
        QString childPath = childItem->data(Qt::UserRole).toString();
        QString childName = childItem->text();

        QList<QStandardItem*> row;
        row << new QStandardItem(childName);
        row << new QStandardItem("");  // Size (would need from file data)
        row << new QStandardItem(childItem->hasChildren() ? "Directory" : "File");
        row << new QStandardItem("");  // Modified date (would need from file data)

        m_fileListModel->appendRow(row);
    }

    qDebug() << "Populated file list with" << childCount << "items";
}
