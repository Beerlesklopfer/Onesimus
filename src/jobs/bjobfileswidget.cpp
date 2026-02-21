#include "jobs/bjobfileswidget.h"
#include "blogging.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

BJobFilesWidget::BJobFilesWidget(const QJsonObject &job, BDirector *director, QWidget *parent)
    : QWidget(parent)
    , m_job(job)
    , m_director(director)
    , m_jobId(job["jobid"].toString().toULongLong())
    , m_clientName(job["client"].toString())
{
    setupUi();

    if (m_director) {
        BLOG_DEBUG() << "[BVFS Widget] Loading files for JobID:" << m_jobId
                 << "Client:" << m_clientName;
        m_bvfsModel->loadJob(m_jobId);
    }
}

void BJobFilesWidget::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Loading indicator
    m_loadingProgress = new QProgressBar(this);
    m_loadingProgress->setRange(0, 0);  // Indeterminate
    m_loadingProgress->setTextVisible(true);
    m_loadingProgress->setFormat("Loading files...");
    mainLayout->addWidget(m_loadingProgress);

    // Create model and directory-only proxy
    m_bvfsModel = new BBvfsModel(m_director, this);
    m_dirProxy = new BBvfsDirFilterProxy(this);
    m_dirProxy->setSourceModel(m_bvfsModel);

    connect(m_bvfsModel, &BBvfsModel::loadingStarted,
            this, &BJobFilesWidget::onLoadingStarted);
    connect(m_bvfsModel, &BBvfsModel::loadingFinished,
            this, &BJobFilesWidget::onLoadingFinished);

    // Emit filesLoaded() only once, when the command queue fully drains
    // after the initial load (root dirs + first directory's files)
    connect(m_bvfsModel, &BBvfsModel::commandQueueIdle, this, [this]() {
        if (!m_initialLoadDone) {
            m_initialLoadDone = true;
            BLOG_DEBUG() << "[BVFS Widget] Initial load complete, emitting filesLoaded()";
            emit filesLoaded();
        }
    });

    // Splitter for tree view and file list
    m_filesSplitter = new QSplitter(Qt::Horizontal, this);

    // Tree view: directories only (via proxy)
    m_fileTreeView = new QTreeView(m_filesSplitter);
    m_fileTreeView->setModel(m_dirProxy);
    m_fileTreeView->setHeaderHidden(false);
    m_fileTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Hide non-name columns in tree view
    for (int i = 1; i < BBvfsModel::ColCount; ++i) {
        m_fileTreeView->setColumnHidden(i, true);
    }

    connect(m_fileTreeView, &QTreeView::clicked,
            this, &BJobFilesWidget::onTreeItemClicked);
    connect(m_fileTreeView, &QTreeView::expanded,
            this, &BJobFilesWidget::onTreeItemExpanded);
    connect(m_fileTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &BJobFilesWidget::onTreeCurrentChanged);

    // Table view: all items via sort proxy
    m_listSortProxy = new QSortFilterProxyModel(this);
    m_listSortProxy->setSourceModel(m_bvfsModel);
    m_listSortProxy->setSortRole(BBvfsModel::SortRole);

    m_fileListView = new QTableView(m_filesSplitter);
    m_fileListView->setModel(m_listSortProxy);
    m_fileListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_fileListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_fileListView->verticalHeader()->setVisible(false);
    m_fileListView->setSortingEnabled(true);
    m_fileListView->horizontalHeader()->setSectionsClickable(true);
    m_fileListView->horizontalHeader()->setSortIndicatorShown(true);
    m_fileListView->horizontalHeader()->setStretchLastSection(false);

    // Column resize modes: Name stretches, others fit content
    auto *header = m_fileListView->horizontalHeader();
    header->setSectionResizeMode(BBvfsModel::ColName, QHeaderView::Stretch);
    header->setSectionResizeMode(BBvfsModel::ColSize, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(BBvfsModel::ColType, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(BBvfsModel::ColModified, QHeaderView::ResizeToContents);

    connect(m_fileListView, &QTableView::doubleClicked,
            this, &BJobFilesWidget::onFileListDoubleClicked);

    // Set splitter sizes (30% tree, 70% list)
    m_filesSplitter->addWidget(m_fileTreeView);
    m_filesSplitter->addWidget(m_fileListView);
    m_filesSplitter->setSizes({250, 550});

    mainLayout->addWidget(m_filesSplitter);
}

void BJobFilesWidget::showDirectoryInList(const QModelIndex &dirProxyIndex)
{
    if (!dirProxyIndex.isValid()) return;

    QModelIndex srcIndex = m_dirProxy->mapToSource(dirProxyIndex);

    if (m_bvfsModel->canFetchMore(srcIndex)) {
        m_bvfsModel->fetchMore(srcIndex);
    }
    m_bvfsModel->loadFilesForDirectory(srcIndex);

    // Map source index through sort proxy for the list view
    QModelIndex listProxyIndex = m_listSortProxy->mapFromSource(srcIndex);
    m_fileListView->setRootIndex(listProxyIndex);
}

void BJobFilesWidget::onTreeItemClicked(const QModelIndex &proxyIndex)
{
    showDirectoryInList(proxyIndex);
}

void BJobFilesWidget::onTreeItemExpanded(const QModelIndex &proxyIndex)
{
    showDirectoryInList(proxyIndex);
    m_fileTreeView->setCurrentIndex(proxyIndex);
}

void BJobFilesWidget::onTreeCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    showDirectoryInList(current);
}

void BJobFilesWidget::onFileListDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;
    if (!index.data(BBvfsModel::IsDirectoryRole).toBool()) return;

    // Map from sort proxy to source, then to dir proxy
    QModelIndex sortProxyIndex = index.sibling(index.row(), 0);
    QModelIndex srcIndex = m_listSortProxy->mapToSource(sortProxyIndex);
    QModelIndex dirProxyIndex = m_dirProxy->mapFromSource(srcIndex);

    if (dirProxyIndex.isValid()) {
        m_fileTreeView->setCurrentIndex(dirProxyIndex);
        m_fileTreeView->expand(dirProxyIndex);
        showDirectoryInList(dirProxyIndex);
    }
}

void BJobFilesWidget::onLoadingStarted()
{
    m_loadingProgress->setVisible(true);
    m_loadingProgress->setFormat("Loading files...");
}

void BJobFilesWidget::onLoadingFinished()
{
    m_loadingProgress->setVisible(false);

    // Auto-expand first root item only on initial load
    if (!m_initialExpandDone) {
        m_initialExpandDone = true;
        QModelIndex firstProxy = m_dirProxy->index(0, 0);
        if (firstProxy.isValid()) {
            m_fileTreeView->expand(firstProxy);
            m_fileTreeView->setCurrentIndex(firstProxy);
            showDirectoryInList(firstProxy);
        }
    }
}
