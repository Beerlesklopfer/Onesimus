#include "jobs/bjobfileswidget.h"
#include "bsettings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QFontMetrics>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QIcon>
#include <QStandardItem>
#include <QTimer>
#include <QMessageBox>
#include <QDialog>
#include <QGroupBox>
#include <QFormLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QDateTime>
#include <functional>

BJobFilesWidget::BJobFilesWidget(const QJsonObject &job, BDirector *director, QWidget *parent)
    : QWidget(parent)
    , m_job(job)
    , m_director(director)
    , m_jobId(job["jobid"].toString().toULongLong())
    , m_currentPath("/")
    , m_currentPathId(0)
    , m_clientName(job["client"].toString())
    , m_bvfsState(BvfsState::Idle)
{
    setupUi();

    // Connect to Director signals
    if (m_director) {
        connect(m_director, &BDirector::jsonResponse,
                this, &BJobFilesWidget::onFilesDataReceived);

        // Load files from Director
        loadFilesFromDirector();
    }
}

void BJobFilesWidget::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Toolbar with options
    QHBoxLayout *toolbarLayout = new QHBoxLayout();

    // Checkbox: Show files from all related jobs (full restore chain)
    m_allJobsCheckBox = new QCheckBox(tr("Show files from all related jobs (full restore chain)"), this);
    m_allJobsCheckBox->setToolTip(tr("When checked, shows files from all incremental/differential backups.\n"
                                      "When unchecked, shows only files from this specific job."));
    // Load from settings (default: false = show only current job)
    m_allJobsCheckBox->setChecked(BSettings::instance().bvfsShowAllRelatedJobs());
    connect(m_allJobsCheckBox, &QCheckBox::toggled,
            this, &BJobFilesWidget::onAllJobsToggled);
    toolbarLayout->addWidget(m_allJobsCheckBox);
    toolbarLayout->addStretch();

    // Restore button
    m_restoreButton = new QPushButton(tr("Restore Selected..."), this);
    m_restoreButton->setIcon(QIcon::fromTheme("edit-undo"));
    m_restoreButton->setToolTip(tr("Restore selected files and directories"));
    m_restoreButton->setEnabled(false);  // Disabled until files are selected
    m_restoreButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    connect(m_restoreButton, &QPushButton::clicked,
            this, &BJobFilesWidget::onRestoreClicked);
    toolbarLayout->addWidget(m_restoreButton);

    mainLayout->addLayout(toolbarLayout);

    // Loading indicator
    m_loadingProgress = new QProgressBar(this);
    m_loadingProgress->setRange(0, 0);  // Indeterminate
    m_loadingProgress->setTextVisible(true);
    m_loadingProgress->setFormat("Loading files...");
    mainLayout->addWidget(m_loadingProgress);

    // Splitter for tree view and file list
    m_filesSplitter = new QSplitter(Qt::Horizontal, this);

    // Tree view for directory structure with checkboxes for selection
    m_fileTreeView = new QTreeView(m_filesSplitter);
    m_treeModel = new QStandardItemModel(this);
    m_treeModel->setHorizontalHeaderLabels({"Directory Structure"});
    m_fileTreeView->setModel(m_treeModel);
    m_fileTreeView->setHeaderHidden(false);
    m_fileTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_fileTreeView, &QTreeView::clicked,
            this, &BJobFilesWidget::onTreeItemClicked);
    connect(m_fileTreeView, &QTreeView::expanded,
            this, &BJobFilesWidget::onTreeItemExpanded);
    connect(m_treeModel, &QStandardItemModel::itemChanged,
            this, &BJobFilesWidget::onTreeItemCheckChanged);

    // Table view for file list with checkboxes for selection
    m_fileListView = new QTableView(m_filesSplitter);
    m_fileListModel = new QStandardItemModel(this);
    m_fileListModel->setHorizontalHeaderLabels({"", "Name", "Size", "Type", "Modified"});
    m_fileListView->setModel(m_fileListModel);
    m_fileListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_fileListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_fileListView->setColumnWidth(0, 30);  // Checkbox column
    m_fileListView->horizontalHeader()->setStretchLastSection(true);
    m_fileListView->verticalHeader()->setVisible(false);
    connect(m_fileListModel, &QStandardItemModel::itemChanged,
            this, &BJobFilesWidget::onSelectionChanged);
    connect(m_fileListView, &QTableView::doubleClicked,
            this, &BJobFilesWidget::onFileListDoubleClicked);

    // Set splitter sizes (30% tree, 70% list)
    m_filesSplitter->addWidget(m_fileTreeView);
    m_filesSplitter->addWidget(m_fileListView);
    m_filesSplitter->setSizes({250, 550});

    mainLayout->addWidget(m_filesSplitter);
}

int BJobFilesWidget::selectedItemCount() const
{
    return countSelectedItems();
}

bool BJobFilesWidget::hasSelection() const
{
    return countSelectedItems() > 0;
}

QString BJobFilesWidget::formatBytes(qint64 bytes) const
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

void BJobFilesWidget::loadFilesFromDirector()
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
        m_bvfsState = BvfsState::GettingJobIds;
        QString getJobIdsCmd = QString(".bvfs_get_jobids jobid=%1 all").arg(m_jobId);
        qDebug() << "BVFS: Step 1 - Sending command:" << getJobIdsCmd;

        QMetaObject::invokeMethod(m_director, "doSendCommand",
                                  Qt::QueuedConnection,
                                  Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                  Q_ARG(QString, getJobIdsCmd));
    } else {
        m_bvfsJobIds = QString::number(m_jobId);
        qDebug() << "BVFS: Using single job ID:" << m_bvfsJobIds;

        m_bvfsState = BvfsState::UpdatingCache;
        QString updateCmd = QString(".bvfs_update jobid=%1").arg(m_bvfsJobIds);
        qDebug() << "BVFS: Step 2 - Sending command:" << updateCmd;

        QMetaObject::invokeMethod(m_director, "doSendCommand",
                                  Qt::QueuedConnection,
                                  Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                  Q_ARG(QString, updateCmd));

        QTimer::singleShot(300, this, [this]() {
            m_bvfsState = BvfsState::ListingDirs;
            QString lsdirsCmd = QString(".bvfs_lsdirs jobid=%1 pathid=1").arg(m_bvfsJobIds);
            qDebug() << "BVFS: Step 3 - Sending command:" << lsdirsCmd;

            QMetaObject::invokeMethod(m_director, "doSendCommand",
                                      Qt::QueuedConnection,
                                      Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                      Q_ARG(QString, lsdirsCmd));
        });
    }
}

void BJobFilesWidget::onFilesDataReceived(const QString &command, const QString &jsonData)
{
    qDebug() << "BVFS: onFilesDataReceived called";
    qDebug() << "BVFS:   Command:" << command;
    qDebug() << "BVFS:   Data size:" << jsonData.size() << "bytes";
    qDebug() << "BVFS:   State:" << static_cast<int>(m_bvfsState);

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

        if (jobIdList.isEmpty()) {
            jobIdList << QString::number(m_jobId);
        }

        m_bvfsJobIds = jobIdList.join(",");
        qDebug() << "BVFS: ✓ Got BVFS JobIDs:" << m_bvfsJobIds;

        m_bvfsState = BvfsState::UpdatingCache;
        QString updateCmd = QString(".bvfs_update jobid=%1").arg(m_bvfsJobIds);
        qDebug() << "BVFS: Step 2 - Sending command:" << updateCmd;

        QMetaObject::invokeMethod(m_director, "doSendCommand",
                                  Qt::QueuedConnection,
                                  Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                  Q_ARG(QString, updateCmd));

        QTimer::singleShot(300, this, [this]() {
            m_bvfsState = BvfsState::ListingDirs;
            QString lsdirsCmd = QString(".bvfs_lsdirs jobid=%1 pathid=1").arg(m_bvfsJobIds);
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

        if (dirsArray.isEmpty() && root.contains("result")) {
            QJsonValue resultVal = root["result"];
            if (resultVal.isArray()) {
                dirsArray = resultVal.toArray();
            }
        }

        qDebug() << "BVFS: Found" << dirsArray.size() << "directories";

        bool onlyDotDirs = true;
        int parentPathId = -1;
        for (const QJsonValue &dirVal : dirsArray) {
            QJsonObject dirObj = dirVal.toObject();
            QString dirName = dirObj["name"].toString();
            if (dirName != "." && dirName != "..") {
                onlyDotDirs = false;
                break;
            }
            if (dirName == "..") {
                parentPathId = dirObj["pathid"].toVariant().toInt();
            }
        }

        if (onlyDotDirs && parentPathId > 0) {
            qDebug() << "BVFS: Only . and .. found, navigating to parent pathid:" << parentPathId;
            QString lsdirsCmd = QString(".bvfs_lsdirs jobid=%1 pathid=%2")
                                    .arg(m_bvfsJobIds)
                                    .arg(parentPathId);
            QMetaObject::invokeMethod(m_director, "doSendCommand",
                                      Qt::QueuedConnection,
                                      Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                      Q_ARG(QString, lsdirsCmd));
            return;
        }

        if (dirsArray.isEmpty()) {
            m_loadingProgress->setFormat("No directories found for this job");
        } else {
            populateFileTree(dirsArray);
        }

        m_loadingProgress->setVisible(false);
        m_bvfsState = BvfsState::Idle;
        return;
    }

    // Process bvfs_lsdirs response for subdirectory expansion
    if (command.contains("bvfs_lsdirs") && m_bvfsState == BvfsState::ListingSubDirs) {
        qDebug() << "BVFS: Processing subdirectory expansion response for:" << m_pendingExpandPath;

        QJsonArray dirsArray = result["directories"].toArray();
        if (dirsArray.isEmpty() && root.contains("result")) {
            QJsonValue resultVal = root["result"];
            if (resultVal.isArray()) {
                dirsArray = resultVal.toArray();
            }
        }

        if (m_pendingExpandItem) {
            for (const QJsonValue &dirVal : dirsArray) {
                QJsonObject dirObj = dirVal.toObject();
                QString dirName = dirObj["name"].toString();
                QString fullPath = dirObj["fullpath"].toString();
                int pathId = dirObj["pathid"].toVariant().toInt();

                if (dirName == "." || dirName == ".." || dirName.isEmpty()) {
                    continue;
                }

                if (dirName.endsWith('/')) {
                    dirName.chop(1);
                }

                if (fullPath.isEmpty()) {
                    fullPath = m_pendingExpandPath + dirName;
                }

                QStandardItem *item = new QStandardItem(dirName);
                item->setData(fullPath, Qt::UserRole);
                item->setData(pathId, Qt::UserRole + 1);
                item->setIcon(QIcon::fromTheme("folder"));
                item->setCheckable(true);
                item->setCheckState(Qt::Unchecked);

                QStandardItem *placeholder = new QStandardItem("Loading...");
                placeholder->setData("__placeholder__", Qt::UserRole);
                item->appendRow(placeholder);

                m_pendingExpandItem->appendRow(item);
            }
        }

        m_pendingExpandItem = nullptr;
        m_pendingExpandPath.clear();
        m_bvfsState = BvfsState::Idle;
        return;
    }

    // Process bvfs_lsfiles response
    if (command.contains("bvfs_lsfiles") && m_bvfsState == BvfsState::ListingFiles) {
        qDebug() << "BVFS: Processing bvfs_lsfiles response";

        QJsonArray filesArray = result["files"].toArray();
        qDebug() << "BVFS: Found" << filesArray.size() << "files";

        populateFileList(filesArray);
        m_bvfsState = BvfsState::Idle;
        return;
    }

    // Process bvfs_restore response
    if (command.contains("bvfs_restore") && m_bvfsState == BvfsState::CreatingRestoreTable) {
        qDebug() << "BVFS: Restore table created";

        QString tableName = result["table"].toString();
        if (tableName.isEmpty()) {
            tableName = m_restoreTableName;
        }

        m_bvfsState = BvfsState::ExecutingRestore;

        QString restoreCmd = QString("restore file=?%1 client=%2")
                                 .arg(tableName)
                                 .arg(m_restoreClient);

        if (!m_restoreWhere.isEmpty()) {
            restoreCmd += QString(" where=\"%1\"").arg(m_restoreWhere);
        }

        restoreCmd += QString(" replace=%1 yes").arg(m_restoreReplace);

        qDebug() << "BVFS: Executing restore command:" << restoreCmd;

        QMetaObject::invokeMethod(m_director, "sendCommand",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, restoreCmd));

        m_bvfsState = BvfsState::Idle;
        return;
    }
}

void BJobFilesWidget::populateFileTree(const QJsonArray &dirsArray)
{
    qDebug() << "BVFS: Populating file tree with" << dirsArray.size() << "directories";

    m_treeModel->clear();
    m_treeModel->setHorizontalHeaderLabels({"Directory Structure"});

    if (dirsArray.isEmpty()) {
        QStandardItem *rootItem = new QStandardItem("No directories found");
        m_treeModel->appendRow(rootItem);
        return;
    }

    QMap<QString, QStandardItem*> rootItems;
    bool hasWindowsPaths = false;
    bool hasUnixPaths = false;

    for (const QJsonValue &dirVal : dirsArray) {
        QJsonObject dirObj = dirVal.toObject();
        QString dirName = dirObj["name"].toString();
        QString fullPath = dirObj["fullpath"].toString();

        if (dirName == "." || dirName == ".." || dirName.isEmpty()) {
            continue;
        }

        QString checkPath = fullPath.isEmpty() ? dirName : fullPath;
        if (checkPath.length() >= 2 && checkPath[1] == ':') {
            hasWindowsPaths = true;
        } else if (checkPath.startsWith('/')) {
            hasUnixPaths = true;
        }
    }

    if (!hasWindowsPaths) {
        QStandardItem *rootItem = new QStandardItem("/");
        rootItem->setData("/", Qt::UserRole);
        rootItem->setData(0, Qt::UserRole + 1);
        rootItem->setIcon(QIcon::fromTheme("folder"));
        rootItem->setCheckable(true);
        rootItem->setCheckState(Qt::Unchecked);
        m_treeModel->appendRow(rootItem);
        rootItems["/"] = rootItem;
    }

    for (const QJsonValue &dirVal : dirsArray) {
        QJsonObject dirObj = dirVal.toObject();

        QString dirName = dirObj["name"].toString();
        QString fullPath = dirObj["fullpath"].toString();
        int pathId = dirObj["pathid"].toVariant().toInt();

        if (dirName == "." || dirName == ".." || dirName.isEmpty()) {
            continue;
        }

        QString displayName = dirName;
        if (displayName.endsWith('/')) {
            displayName.chop(1);
        }

        QString rootKey;
        QStandardItem *parentItem = nullptr;

        if (displayName.length() == 2 && displayName[1] == ':') {
            rootKey = displayName;
            fullPath = displayName + "/";

            if (!rootItems.contains(rootKey)) {
                QStandardItem *driveItem = new QStandardItem(rootKey);
                driveItem->setData(fullPath, Qt::UserRole);
                driveItem->setData(pathId, Qt::UserRole + 1);
                driveItem->setIcon(QIcon::fromTheme("drive-harddisk"));
                driveItem->setCheckable(true);
                driveItem->setCheckState(Qt::Unchecked);

                QStandardItem *placeholder = new QStandardItem("Loading...");
                placeholder->setData("__placeholder__", Qt::UserRole);
                driveItem->appendRow(placeholder);

                m_treeModel->appendRow(driveItem);
                rootItems[rootKey] = driveItem;
            }
            continue;
        }

        if (fullPath.length() >= 2 && fullPath[1] == ':') {
            rootKey = fullPath.left(2);
        } else if (fullPath.isEmpty() && displayName.length() >= 2 && displayName[1] == ':') {
            rootKey = displayName.left(2);
            fullPath = displayName;
            if (!fullPath.endsWith('/')) {
                fullPath += '/';
            }
        } else if (fullPath.isEmpty()) {
            fullPath = "/" + displayName;
            rootKey = "/";
        } else if (fullPath.startsWith('/')) {
            rootKey = "/";
        } else {
            rootKey = "/";
            if (!fullPath.startsWith('/')) {
                fullPath = "/" + fullPath;
            }
        }

        if (rootKey != "/" && !rootItems.contains(rootKey)) {
            QStandardItem *driveItem = new QStandardItem(rootKey);
            driveItem->setData(rootKey + "/", Qt::UserRole);
            driveItem->setData(0, Qt::UserRole + 1);
            driveItem->setIcon(QIcon::fromTheme("drive-harddisk"));
            driveItem->setCheckable(true);
            driveItem->setCheckState(Qt::Unchecked);
            m_treeModel->appendRow(driveItem);
            rootItems[rootKey] = driveItem;
        }

        parentItem = rootItems.value(rootKey);
        if (!parentItem) {
            continue;
        }

        QStandardItem *item = new QStandardItem(displayName);
        item->setData(fullPath, Qt::UserRole);
        item->setData(pathId, Qt::UserRole + 1);
        item->setIcon(QIcon::fromTheme("folder"));
        item->setCheckable(true);
        item->setCheckState(Qt::Unchecked);

        QStandardItem *placeholder = new QStandardItem("Loading...");
        placeholder->setData("__placeholder__", Qt::UserRole);
        item->appendRow(placeholder);

        parentItem->appendRow(item);
    }

    QModelIndex rootIndex = m_treeModel->index(0, 0);
    if (rootIndex.isValid()) {
        m_fileTreeView->expand(rootIndex);
        m_fileTreeView->setCurrentIndex(rootIndex);
        onTreeItemClicked(rootIndex);
    }
}

void BJobFilesWidget::onTreeItemClicked(const QModelIndex &index)
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

    m_currentPath = selectedPath;
    m_currentPathId = pathId;

    loadFilesForDirectory(pathId, selectedPath);
}

void BJobFilesWidget::onTreeItemExpanded(const QModelIndex &index)
{
    if (!index.isValid() || !m_director) {
        return;
    }

    QStandardItem *item = m_treeModel->itemFromIndex(index);
    if (!item) {
        return;
    }

    if (item->rowCount() == 1) {
        QStandardItem *firstChild = item->child(0);
        if (firstChild && firstChild->data(Qt::UserRole).toString() == "__placeholder__") {
            item->removeRow(0);

            QString path = item->data(Qt::UserRole).toString();
            int pathId = item->data(Qt::UserRole + 1).toInt();

            m_pendingExpandPath = path.endsWith('/') ? path : path + '/';
            m_pendingExpandItem = item;
            m_bvfsState = BvfsState::ListingSubDirs;

            QString lsdirsCmd = QString(".bvfs_lsdirs jobid=%1 pathid=%2")
                                    .arg(m_bvfsJobIds)
                                    .arg(pathId);

            QMetaObject::invokeMethod(m_director, "doSendCommand",
                                      Qt::QueuedConnection,
                                      Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                      Q_ARG(QString, lsdirsCmd));
        }
    }
}

void BJobFilesWidget::onAllJobsToggled(bool checked)
{
    qDebug() << "BVFS: All jobs toggle changed to:" << checked;

    BSettings::instance().setBvfsShowAllRelatedJobs(checked);

    m_treeModel->clear();
    m_treeModel->setHorizontalHeaderLabels({"Directory Structure"});
    m_fileListModel->clear();
    m_fileListModel->setHorizontalHeaderLabels({"", "Name", "Size", "Type", "Modified"});

    m_loadingProgress->setVisible(true);
    m_loadingProgress->setFormat("Loading files...");

    loadFilesFromDirector();
}

void BJobFilesWidget::loadFilesForDirectory(int pathId, const QString &path)
{
    if (!m_director) {
        return;
    }

    QString jobIds;
    if (m_allJobsCheckBox->isChecked()) {
        jobIds = m_bvfsJobIds;
    } else {
        jobIds = QString::number(m_jobId);
    }

    m_fileListModel->clear();
    m_fileListModel->setHorizontalHeaderLabels({"", "Name", "Size", "Type", "Modified"});

    QModelIndex currentIndex = m_fileTreeView->currentIndex();
    if (currentIndex.isValid()) {
        QStandardItem *item = m_treeModel->itemFromIndex(currentIndex);
        if (item) {
            int childCount = item->rowCount();
            for (int i = 0; i < childCount; ++i) {
                QStandardItem *childItem = item->child(i);
                if (!childItem) {
                    continue;
                }

                if (childItem->data(Qt::UserRole).toString() == "__placeholder__") {
                    continue;
                }

                QString childName = childItem->text();
                int childPathId = childItem->data(Qt::UserRole + 1).toInt();

                QList<QStandardItem*> row;
                QStandardItem *checkItem = new QStandardItem();
                checkItem->setCheckable(true);
                checkItem->setCheckState(Qt::Unchecked);
                // Store pathId for directories (UserRole+2 to distinguish from files)
                checkItem->setData(childPathId, Qt::UserRole + 2);
                row << checkItem;

                QStandardItem *nameItem = new QStandardItem(childName);
                nameItem->setIcon(QIcon::fromTheme("folder"));
                row << nameItem;
                row << new QStandardItem("-");
                row << new QStandardItem(tr("Directory"));
                row << new QStandardItem("-");

                m_fileListModel->appendRow(row);
            }
        }
    }

    m_bvfsState = BvfsState::ListingFiles;

    QString lsfilesCmd = QString(".bvfs_lsfiles jobid=%1 pathid=%2")
                             .arg(jobIds)
                             .arg(pathId);

    QMetaObject::invokeMethod(m_director, "doSendCommand",
                              Qt::QueuedConnection,
                              Q_ARG(BDirector::Command, BDirector::Command::Custom),
                              Q_ARG(QString, lsfilesCmd));
}

void BJobFilesWidget::populateFileList(const QJsonArray &filesArray)
{
    for (const QJsonValue &fileVal : filesArray) {
        QJsonObject fileObj = fileVal.toObject();

        QString fileName = fileObj["name"].toString();
        if (fileName.isEmpty() || fileName == "." || fileName == "..") {
            continue;
        }

        QJsonObject stat = fileObj["stat"].toObject();
        qint64 fileSize = stat["size"].toVariant().toLongLong();
        qint64 mtime = stat["mtime"].toVariant().toLongLong();

        QString sizeStr = formatBytes(fileSize);

        QString mtimeStr;
        if (mtime > 0) {
            QDateTime dateTime = QDateTime::fromSecsSinceEpoch(mtime);
            mtimeStr = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        }

        QString fileType = tr("File");
        int mode = stat["mode"].toInt();
        if (mode > 0) {
            if ((mode & 0170000) == 0040000) {
                fileType = tr("Directory");
            } else if ((mode & 0170000) == 0120000) {
                fileType = tr("Symlink");
            }
        }

        QList<QStandardItem*> row;

        QStandardItem *checkItem = new QStandardItem();
        checkItem->setCheckable(true);
        checkItem->setCheckState(Qt::Unchecked);
        checkItem->setData(fileObj["fileid"].toString(), Qt::UserRole);
        checkItem->setData(fileObj["jobid"].toString(), Qt::UserRole + 1);
        row << checkItem;

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

        m_fileListModel->appendRow(row);
    }

    m_fileListView->resizeColumnsToContents();
    updateRestoreButtonState();
}

QStringList BJobFilesWidget::getSelectedFileIds() const
{
    QStringList fileIds;

    for (int row = 0; row < m_fileListModel->rowCount(); ++row) {
        QStandardItem *checkItem = m_fileListModel->item(row, 0);
        if (checkItem && checkItem->checkState() == Qt::Checked) {
            QString fileId = checkItem->data(Qt::UserRole).toString();
            if (!fileId.isEmpty()) {
                fileIds << fileId;
            }
        }
    }

    return fileIds;
}

QStringList BJobFilesWidget::getSelectedDirIds() const
{
    QStringList dirIds;

    // Collect from tree view
    std::function<void(QStandardItem*)> collectDirs = [&](QStandardItem *item) {
        if (!item) return;

        if (item->isCheckable() && item->checkState() == Qt::Checked) {
            int pathId = item->data(Qt::UserRole + 1).toInt();
            if (pathId > 0) {
                dirIds << QString::number(pathId);
            }
        }

        for (int i = 0; i < item->rowCount(); ++i) {
            collectDirs(item->child(i));
        }
    };

    for (int i = 0; i < m_treeModel->rowCount(); ++i) {
        collectDirs(m_treeModel->item(i));
    }

    // Also collect directories selected in the file list (stored in UserRole+2)
    for (int row = 0; row < m_fileListModel->rowCount(); ++row) {
        QStandardItem *checkItem = m_fileListModel->item(row, 0);
        if (checkItem && checkItem->checkState() == Qt::Checked) {
            int pathId = checkItem->data(Qt::UserRole + 2).toInt();
            if (pathId > 0) {
                // Avoid duplicates (directory might be checked in both tree and list)
                QString pathIdStr = QString::number(pathId);
                if (!dirIds.contains(pathIdStr)) {
                    dirIds << pathIdStr;
                }
            }
        }
    }

    return dirIds;
}

int BJobFilesWidget::countSelectedItems() const
{
    return getSelectedFileIds().count() + getSelectedDirIds().count();
}

void BJobFilesWidget::updateRestoreButtonState()
{
    int count = countSelectedItems();
    m_restoreButton->setEnabled(count > 0);

    if (count > 0) {
        m_restoreButton->setText(tr("Restore Selected (%1)...").arg(count));
    } else {
        m_restoreButton->setText(tr("Restore Selected..."));
    }

    // Adjust minimum width based on current text
    QFontMetrics fm(m_restoreButton->font());
    int textWidth = fm.horizontalAdvance(m_restoreButton->text());
    int iconWidth = m_restoreButton->iconSize().width();
    int padding = 30;  // Account for button padding and margins
    m_restoreButton->setMinimumWidth(textWidth + iconWidth + padding);

    emit selectionChanged(count);
}

void BJobFilesWidget::onTreeItemCheckChanged(QStandardItem *item)
{
    if (!item || !item->isCheckable()) {
        updateRestoreButtonState();
        return;
    }

    // Get the current tree selection
    QModelIndex currentIndex = m_fileTreeView->currentIndex();
    if (!currentIndex.isValid()) {
        updateRestoreButtonState();
        return;
    }

    QStandardItem *currentItem = m_treeModel->itemFromIndex(currentIndex);

    // If the changed item is the currently selected folder, update all file list items
    if (item == currentItem) {
        Qt::CheckState newState = item->checkState();

        // Block signals to avoid recursive updates
        m_fileListModel->blockSignals(true);

        for (int row = 0; row < m_fileListModel->rowCount(); ++row) {
            QStandardItem *checkItem = m_fileListModel->item(row, 0);
            if (checkItem && checkItem->isCheckable()) {
                checkItem->setCheckState(newState);
            }
        }

        m_fileListModel->blockSignals(false);
    }

    updateRestoreButtonState();
}

void BJobFilesWidget::onSelectionChanged()
{
    updateRestoreButtonState();
}

void BJobFilesWidget::onFileListDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    QModelIndex typeIndex = m_fileListModel->index(index.row(), 3);
    QString type = m_fileListModel->data(typeIndex).toString();

    if (type != tr("Directory")) {
        return;
    }

    QModelIndex nameIndex = m_fileListModel->index(index.row(), 1);
    QString dirName = m_fileListModel->data(nameIndex).toString();

    if (dirName == "Loading..." || dirName.isEmpty()) {
        return;
    }

    QModelIndex currentTreeIndex = m_fileTreeView->currentIndex();
    if (!currentTreeIndex.isValid()) {
        return;
    }

    QStandardItem *currentItem = m_treeModel->itemFromIndex(currentTreeIndex);
    if (!currentItem) {
        return;
    }

    for (int i = 0; i < currentItem->rowCount(); ++i) {
        QStandardItem *child = currentItem->child(i);
        if (!child) {
            continue;
        }

        if (child->data(Qt::UserRole).toString() == "__placeholder__") {
            continue;
        }

        if (child->text() == dirName) {
            QModelIndex childIndex = m_treeModel->indexFromItem(child);
            if (!childIndex.isValid()) {
                return;
            }

            m_fileTreeView->expand(currentTreeIndex);
            m_fileTreeView->setCurrentIndex(childIndex);
            m_fileTreeView->expand(childIndex);

            int pathId = child->data(Qt::UserRole + 1).toInt();
            QString path = child->data(Qt::UserRole).toString();

            if (pathId > 0 && !path.isEmpty()) {
                loadFilesForDirectory(pathId, path);
            }
            break;
        }
    }
}

void BJobFilesWidget::onRestoreClicked()
{
    QStringList fileIds = getSelectedFileIds();
    QStringList dirIds = getSelectedDirIds();

    if (fileIds.isEmpty() && dirIds.isEmpty()) {
        QMessageBox::information(this, tr("No Selection"),
                                 tr("Please select at least one file or directory to restore."));
        return;
    }

    QDialog restoreDialog(this);
    restoreDialog.setWindowTitle(tr("Restore Configuration"));
    restoreDialog.resize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(&restoreDialog);

    QGroupBox *summaryGroup = new QGroupBox(tr("Selection Summary"), &restoreDialog);
    QFormLayout *summaryLayout = new QFormLayout(summaryGroup);
    summaryLayout->addRow(tr("Files selected:"), new QLabel(QString::number(fileIds.count())));
    summaryLayout->addRow(tr("Directories selected:"), new QLabel(QString::number(dirIds.count())));
    summaryLayout->addRow(tr("Source job:"), new QLabel(QString("%1 (ID: %2)")
        .arg(m_job["name"].toString())
        .arg(m_jobId)));
    layout->addWidget(summaryGroup);

    QGroupBox *optionsGroup = new QGroupBox(tr("Restore Options"), &restoreDialog);
    QFormLayout *optionsLayout = new QFormLayout(optionsGroup);

    QComboBox *clientCombo = new QComboBox();
    clientCombo->addItem(m_clientName);
    clientCombo->setToolTip(tr("Client where files will be restored"));
    optionsLayout->addRow(tr("Target Client:"), clientCombo);

    QComboBox *whereCombo = new QComboBox();
    whereCombo->setEditable(true);
    whereCombo->addItem("");
    whereCombo->addItem("/tmp/bareos-restores");
    whereCombo->addItem("/var/tmp/restore");
    whereCombo->setToolTip(tr("Leave empty to restore to original location,\n"
                               "or specify a prefix path for restored files"));
    optionsLayout->addRow(tr("Restore Where:"), whereCombo);

    QComboBox *replaceCombo = new QComboBox();
    replaceCombo->addItem(tr("Always"), "always");
    replaceCombo->addItem(tr("Never"), "never");
    replaceCombo->addItem(tr("If Newer"), "ifnewer");
    replaceCombo->addItem(tr("If Older"), "ifolder");
    replaceCombo->setCurrentIndex(0);
    replaceCombo->setToolTip(tr("How to handle existing files at destination"));
    optionsLayout->addRow(tr("Replace:"), replaceCombo);

    layout->addWidget(optionsGroup);

    QGroupBox *previewGroup = new QGroupBox(tr("Command Preview"), &restoreDialog);
    QVBoxLayout *previewLayout = new QVBoxLayout(previewGroup);
    QTextEdit *previewText = new QTextEdit();
    previewText->setReadOnly(true);
    previewText->setMaximumHeight(100);
    previewLayout->addWidget(previewText);
    layout->addWidget(previewGroup);

    auto updatePreview = [&]() {
        QString preview;
        preview += QString("# Step 1: Create BVFS restore table\n");
        preview += QString(".bvfs_restore path=b%1").arg(m_jobId);
        if (!fileIds.isEmpty()) {
            preview += QString(" fileid=%1").arg(fileIds.join(","));
        }
        if (!dirIds.isEmpty()) {
            preview += QString(" dirid=%1").arg(dirIds.join(","));
        }
        preview += QString(" jobid=%1\n\n").arg(m_bvfsJobIds.isEmpty() ?
                                                 QString::number(m_jobId) : m_bvfsJobIds);
        preview += QString("# Step 2: Run restore\n");
        preview += QString("restore file=?b%1 client=%2")
                       .arg(m_jobId)
                       .arg(clientCombo->currentText());
        QString where = whereCombo->currentText();
        if (!where.isEmpty()) {
            preview += QString(" where=\"%1\"").arg(where);
        }
        preview += QString(" replace=%1 yes").arg(replaceCombo->currentData().toString());
        previewText->setText(preview);
    };

    connect(clientCombo, &QComboBox::currentTextChanged, this, updatePreview);
    connect(whereCombo, &QComboBox::currentTextChanged, this, updatePreview);
    connect(replaceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, updatePreview);
    updatePreview();

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Start Restore"));
    connect(buttonBox, &QDialogButtonBox::accepted, &restoreDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &restoreDialog, &QDialog::reject);
    layout->addWidget(buttonBox);

    if (restoreDialog.exec() != QDialog::Accepted) {
        return;
    }

    m_restoreTableName = QString("b%1").arg(m_jobId);
    QString bvfsRestoreCmd = QString(".bvfs_restore path=%1").arg(m_restoreTableName);

    if (!fileIds.isEmpty()) {
        bvfsRestoreCmd += QString(" fileid=%1").arg(fileIds.join(","));
    }
    if (!dirIds.isEmpty()) {
        bvfsRestoreCmd += QString(" dirid=%1").arg(dirIds.join(","));
    }
    bvfsRestoreCmd += QString(" jobid=%1").arg(m_bvfsJobIds.isEmpty() ?
                                                QString::number(m_jobId) : m_bvfsJobIds);

    m_bvfsState = BvfsState::CreatingRestoreTable;

    m_restoreClient = clientCombo->currentText();
    m_restoreWhere = whereCombo->currentText();
    m_restoreReplace = replaceCombo->currentData().toString();

    QMetaObject::invokeMethod(m_director, "doSendCommand",
                              Qt::QueuedConnection,
                              Q_ARG(BDirector::Command, BDirector::Command::Custom),
                              Q_ARG(QString, bvfsRestoreCmd));

    emit restoreRequested();

    QMessageBox::information(this, tr("Restore Started"),
        tr("Restore job has been initiated.\n\n"
           "Check the Jobs view for the restore job status."));
}
