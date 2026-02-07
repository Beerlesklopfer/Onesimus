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

    // Table view: all items (files + dirs) via source model with setRootIndex
    m_fileListView = new QTableView(m_filesSplitter);
    m_fileListView->setModel(m_bvfsModel);
    m_fileListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_fileListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_fileListView->horizontalHeader()->setStretchLastSection(true);
    m_fileListView->verticalHeader()->setVisible(false);

    connect(m_fileListView, &QTableView::doubleClicked,
            this, &BJobFilesWidget::onFileListDoubleClicked);

    // Set splitter sizes (30% tree, 70% list)
    m_filesSplitter->addWidget(m_fileTreeView);
    m_filesSplitter->addWidget(m_fileListView);
    m_filesSplitter->setSizes({250, 550});

    mainLayout->addWidget(m_filesSplitter);
}

void BJobFilesWidget::onTreeItemClicked(const QModelIndex &proxyIndex)
{
    if (!proxyIndex.isValid()) return;

    QModelIndex srcIndex = m_dirProxy->mapToSource(proxyIndex);

    BLOG_DEBUG() << "[BVFS Widget] Tree item clicked:"
             << srcIndex.data(BBvfsModel::FullPathRole).toString();

    // Ensure subdirectories are loaded (tree expand triggers fetchMore,
    // but clicking without expanding does not)
    if (m_bvfsModel->canFetchMore(srcIndex)) {
        m_bvfsModel->fetchMore(srcIndex);
    }

    // Load files for this directory
    m_bvfsModel->loadFilesForDirectory(srcIndex);

    // Show this directory's children in the list view
    m_fileListView->setRootIndex(srcIndex);
}

void BJobFilesWidget::onFileListDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    // Check if it's a directory
    if (!index.data(BBvfsModel::IsDirectoryRole).toBool()) {
        return;
    }

    QString dirName = index.sibling(index.row(), BBvfsModel::ColName).data().toString();
    BLOG_DEBUG() << "[BVFS Widget] Double-clicked directory:" << dirName;

    // List view uses source model directly, so index is already a source index.
    QModelIndex srcIndex = index.sibling(index.row(), 0);

    // Map to proxy for tree view selection
    QModelIndex proxyIndex = m_dirProxy->mapFromSource(srcIndex);
    if (proxyIndex.isValid()) {
        m_fileTreeView->setCurrentIndex(proxyIndex);
        m_fileTreeView->expand(proxyIndex);
    }

    // Ensure subdirectories are loaded
    if (m_bvfsModel->canFetchMore(srcIndex)) {
        m_bvfsModel->fetchMore(srcIndex);
    }

    // Load files and update list view
    m_bvfsModel->loadFilesForDirectory(srcIndex);
    m_fileListView->setRootIndex(srcIndex);
}

void BJobFilesWidget::onLoadingStarted()
{
    m_loadingProgress->setVisible(true);
    m_loadingProgress->setFormat("Loading files...");
}

void BJobFilesWidget::onLoadingFinished()
{
    m_loadingProgress->setVisible(false);

    // Auto-expand and select first root item in tree
    QModelIndex firstProxy = m_dirProxy->index(0, 0);
    if (firstProxy.isValid()) {
        m_fileTreeView->expand(firstProxy);
        m_fileTreeView->setCurrentIndex(firstProxy);
        onTreeItemClicked(firstProxy);
    }

    m_fileListView->resizeColumnsToContents();
}
