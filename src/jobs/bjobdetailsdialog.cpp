#include "jobs/bjobdetailsdialog.h"
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

BJobDetailsDialog::BJobDetailsDialog(const QJsonObject &job, BDirector *director, QWidget *parent)
    : QDialog(parent)
    , m_job(job)
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
        connect(m_director, &BDirector::commandResponse,
                this, &BJobDetailsDialog::onJobLogReceived);

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

    // Job Log List View (MVC pattern)
    m_logListView = new QListView(logWidget);
    m_logModel = new QStandardItemModel(this);
    m_logListView->setModel(m_logModel);
    m_logListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_logListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_logListView->setFont(QFont("Courier", 9));
    m_logListView->setAlternatingRowColors(true);

    // Add initial loading message
    QStandardItem *loadingItem = new QStandardItem("Loading job log...");
    loadingItem->setForeground(QBrush(Qt::gray));
    m_logModel->appendRow(loadingItem);

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
        qWarning() << "No Director connection available";
        m_loadingProgress->setFormat("No connection to Director");
        return;
    }

    qDebug() << "Loading files for JobID:" << m_jobId;

    // Use Bvfs API for file listing (works with JSON API mode)
    // Step 1: Update bvfs cache for this job
    m_director->doSendCommand(BDirector::Command::Custom, QString(".bvfs_update jobid=%1").arg(m_jobId));

    // Step 2: List files using bvfs (will be triggered after cache update)
    QTimer::singleShot(500, this, [this]() {
        m_director->doSendCommand(BDirector::Command::Custom, QString(".bvfs_lsfiles jobid=%1 path=/").arg(m_jobId));
    });

    // Request job log
    m_director->doSendCommand(BDirector::Command::ListJobId, QString::number(m_jobId));
}

void BJobDetailsDialog::onFilesDataReceived(const QString &command, const QString &jsonData)
{
    // Only process if this is for our job
    if (!command.contains(QString::number(m_jobId))) {
        return;
    }

    if (command.contains("bvfs_lsfiles") || command.contains("list files")) {
        qDebug() << "Received files data:" << jsonData.left(200);

        // Parse JSON
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

        if (error.error != QJsonParseError::NoError) {
            qWarning() << "JSON parse error:" << error.errorString();
            m_loadingProgress->setFormat("Error parsing file data");
            return;
        }

        QJsonArray filesArray;
        if (doc.isArray()) {
            filesArray = doc.array();
        } else if (doc.isObject()) {
            QJsonObject root = doc.object();
            // Bvfs API response structure
            if (root.contains("result") && root["result"].isObject()) {
                QJsonObject result = root["result"].toObject();
                if (result.contains("files") && result["files"].isArray()) {
                    filesArray = result["files"].toArray();
                    qDebug() << "Found" << filesArray.size() << "files in Bvfs response";
                }
            } else if (root.contains("files")) {
                filesArray = root["files"].toArray();
            }
        }

        if (filesArray.isEmpty()) {
            qWarning() << "No files found in response";
            qDebug() << "Full JSON:" << jsonData;
            m_loadingProgress->setFormat("No files found for this job");
        } else {
            populateFileTree(filesArray);
        }
        m_loadingProgress->setVisible(false);
    }
}

void BJobDetailsDialog::onJobLogReceived(const QString &command, const QString &response)
{
    // Check for "list joblog jobid=..." command
    if (command.contains("list joblog") && command.contains(QString::number(m_jobId))) {
        qDebug() << "Received job log data for job" << m_jobId;
        qDebug() << "Log size:" << response.size() << "bytes";

        // Clear existing log entries
        m_logModel->clear();

        if (response.isEmpty()) {
            QStandardItem *item = new QStandardItem(tr("No log data available for this job."));
            item->setForeground(QBrush(Qt::gray));
            m_logModel->appendRow(item);
            return;
        }

        // Try to parse as JSON first
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8(), &parseError);

        if (parseError.error == QJsonParseError::NoError) {
            // Successfully parsed as JSON - extract log entries
            QStringList logLines;

            if (doc.isObject()) {
                QJsonObject root = doc.object();

                // Try to find log data in various possible structures
                // Structure 1: { "result": { "joblog": [...] } }
                if (root.contains("result") && root["result"].isObject()) {
                    QJsonObject result = root["result"].toObject();
                    if (result.contains("joblog") && result["joblog"].isArray()) {
                        QJsonArray logArray = result["joblog"].toArray();
                        for (const QJsonValue &val : logArray) {
                            logLines.append(val.toString());
                        }
                    }
                }
                // Structure 2: { "joblog": [...] }
                else if (root.contains("joblog") && root["joblog"].isArray()) {
                    QJsonArray logArray = root["joblog"].toArray();
                    for (const QJsonValue &val : logArray) {
                        logLines.append(val.toString());
                    }
                }
                // Structure 3: { "log": [...] }
                else if (root.contains("log") && root["log"].isArray()) {
                    QJsonArray logArray = root["log"].toArray();
                    for (const QJsonValue &val : logArray) {
                        logLines.append(val.toString());
                    }
                }
            }
            // Structure 4: Direct array of log lines
            else if (doc.isArray()) {
                QJsonArray logArray = doc.array();
                for (const QJsonValue &val : logArray) {
                    logLines.append(val.toString());
                }
            }

            if (!logLines.isEmpty()) {
                // Populate model with log entries
                for (const QString &line : logLines) {
                    QStandardItem *item = new QStandardItem(line);
                    item->setFont(QFont("Courier", 9));
                    m_logModel->appendRow(item);
                }
            } else {
                QStandardItem *item = new QStandardItem(tr("No log entries found in JSON response."));
                item->setForeground(QBrush(Qt::gray));
                m_logModel->appendRow(item);
            }
        } else {
            // Not JSON, display as plain text - split by lines
            QStringList lines = response.split('\n');
            for (const QString &line : lines) {
                QStandardItem *item = new QStandardItem(line);
                item->setFont(QFont("Courier", 9));
                m_logModel->appendRow(item);
            }
        }
    }
}

void BJobDetailsDialog::populateFileTree(const QJsonArray &filesArray)
{
    qDebug() << "Populating file tree with" << filesArray.size() << "files";

    m_treeModel->clear();
    m_treeModel->setHorizontalHeaderLabels({"Directory Structure"});

    if (filesArray.isEmpty()) {
        QStandardItem *rootItem = new QStandardItem("No files found");
        m_treeModel->appendRow(rootItem);
        return;
    }

    // Build directory tree structure
    QMap<QString, QStandardItem*> pathMap;
    QStandardItem *rootItem = new QStandardItem("/");
    rootItem->setData("/", Qt::UserRole);  // Store full path
    m_treeModel->appendRow(rootItem);
    pathMap["/"] = rootItem;

    // Process each file and build tree
    for (const QJsonValue &fileVal : filesArray) {
        QJsonObject fileObj = fileVal.toObject();

        // Bvfs API uses "name" field, not "filename"
        QString filename = fileObj["name"].toString();

        // If "name" is not available, try "filename" for compatibility
        if (filename.isEmpty()) {
            filename = fileObj["filename"].toString();
        }

        if (filename.isEmpty()) {
            qDebug() << "Skipping file with no name:" << fileObj;
            continue;
        }

        qDebug() << "Processing file:" << filename;

        // Split path into components
        QStringList pathParts = filename.split('/', Qt::SkipEmptyParts);

        QString currentPath = "";
        QStandardItem *parentItem = rootItem;

        // Build directory structure
        for (int i = 0; i < pathParts.size(); ++i) {
            bool isLastPart = (i == pathParts.size() - 1);
            QString part = pathParts[i];
            currentPath += "/" + part;

            if (!pathMap.contains(currentPath)) {
                QStandardItem *item = new QStandardItem(part);
                item->setData(currentPath, Qt::UserRole);  // Store full path

                if (isLastPart) {
                    // This is a file
                    item->setIcon(QIcon::fromTheme("text-x-generic"));

                    // Add file size if available from stat
                    if (fileObj.contains("stat") && fileObj["stat"].isObject()) {
                        QJsonObject stat = fileObj["stat"].toObject();
                        qint64 size = stat["size"].toInteger();
                        item->setData(size, Qt::UserRole + 1);  // Store size
                    }
                } else {
                    // This is a directory
                    item->setIcon(QIcon::fromTheme("folder"));
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
