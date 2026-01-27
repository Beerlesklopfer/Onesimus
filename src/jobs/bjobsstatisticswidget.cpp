#include "jobs/bjobsstatisticswidget.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

BJobsStatisticsWidget::BJobsStatisticsWidget(QWidget *parent)
    : QWidget(parent)
    , m_model(nullptr)
{
    setupUi();
}

void BJobsStatisticsWidget::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Summary group
    QGroupBox *summaryGroup = new QGroupBox("Job Summary", this);
    QFormLayout *summaryLayout = new QFormLayout(summaryGroup);
    
    m_totalJobsLabel = new QLabel("0", this);
    m_totalJobsLabel->setStyleSheet("font-weight: bold; font-size: 14pt;");
    summaryLayout->addRow("Total Jobs:", m_totalJobsLabel);
    
    m_selectedLabel = new QLabel("0", this);
    summaryLayout->addRow("Selected:", m_selectedLabel);
    
    mainLayout->addWidget(summaryGroup);
    
    // Status breakdown group
    QGroupBox *statusGroup = new QGroupBox("Status Breakdown", this);
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);
    
    // Successful jobs
    QHBoxLayout *successLayout = new QHBoxLayout();
    m_successLabel = new QLabel("0", this);
    m_successLabel->setMinimumWidth(50);
    successLayout->addWidget(new QLabel("Successful:", this));
    successLayout->addWidget(m_successLabel);
    statusLayout->addLayout(successLayout);
    
    m_successBar = new QProgressBar(this);
    m_successBar->setStyleSheet("QProgressBar::chunk { background-color: #4CAF50; }");
    m_successBar->setTextVisible(true);
    m_successBar->setFormat("%p%");
    statusLayout->addWidget(m_successBar);
    
    // Warning jobs
    QHBoxLayout *warningLayout = new QHBoxLayout();
    m_warningLabel = new QLabel("0", this);
    m_warningLabel->setMinimumWidth(50);
    warningLayout->addWidget(new QLabel("Warnings:", this));
    warningLayout->addWidget(m_warningLabel);
    statusLayout->addLayout(warningLayout);
    
    m_warningBar = new QProgressBar(this);
    m_warningBar->setStyleSheet("QProgressBar::chunk { background-color: #FFC107; }");
    m_warningBar->setTextVisible(true);
    m_warningBar->setFormat("%p%");
    statusLayout->addWidget(m_warningBar);
    
    // Failed jobs
    QHBoxLayout *failedLayout = new QHBoxLayout();
    m_failedLabel = new QLabel("0", this);
    m_failedLabel->setMinimumWidth(50);
    failedLayout->addWidget(new QLabel("Failed:", this));
    failedLayout->addWidget(m_failedLabel);
    statusLayout->addLayout(failedLayout);
    
    m_failedBar = new QProgressBar(this);
    m_failedBar->setStyleSheet("QProgressBar::chunk { background-color: #F44336; }");
    m_failedBar->setTextVisible(true);
    m_failedBar->setFormat("%p%");
    statusLayout->addWidget(m_failedBar);
    
    mainLayout->addWidget(statusGroup);
    
    // Data summary group
    QGroupBox *dataGroup = new QGroupBox("Data Summary", this);
    QFormLayout *dataLayout = new QFormLayout(dataGroup);
    
    m_totalFilesLabel = new QLabel("0", this);
    dataLayout->addRow("Total Files:", m_totalFilesLabel);
    
    m_totalBytesLabel = new QLabel("0 B", this);
    dataLayout->addRow("Total Size:", m_totalBytesLabel);
    
    m_dateRangeLabel = new QLabel("N/A", this);
    dataLayout->addRow("Date Range:", m_dateRangeLabel);
    
    mainLayout->addWidget(dataGroup);
    
    // Refresh button
    m_refreshButton = new QPushButton("Refresh Statistics", this);
    connect(m_refreshButton, &QPushButton::clicked,
            this, &BJobsStatisticsWidget::onRefreshClicked);
    mainLayout->addWidget(m_refreshButton);
    
    mainLayout->addStretch();
}

void BJobsStatisticsWidget::setModel(BJobsModel *model)
{
    if (m_model) {
        disconnect(m_model, nullptr, this, nullptr);
    }
    
    m_model = model;
    
    if (m_model) {
        connect(m_model, &QAbstractItemModel::modelReset,
                this, &BJobsStatisticsWidget::onModelDataChanged);
        
        connect(m_model, &BJobsModel::jobsAppended,
                this, &BJobsStatisticsWidget::onModelDataChanged);
        
        connect(m_model, &BJobsModel::selectionChanged,
                this, &BJobsStatisticsWidget::onModelDataChanged);
    }
    
    updateStatistics();
}

void BJobsStatisticsWidget::refresh()
{
    updateStatistics();
}

void BJobsStatisticsWidget::onRefreshClicked()
{
    updateStatistics();
}

void BJobsStatisticsWidget::onModelDataChanged()
{
    updateStatistics();
}

void BJobsStatisticsWidget::updateStatistics()
{
    if (!m_model) {
        return;
    }
    
    BJobsModel::Statistics stats = m_model->calculateStatistics();
    
    // Update summary
    m_totalJobsLabel->setText(QString::number(stats.totalJobs));
    m_selectedLabel->setText(QString::number(stats.selectedCount));
    
    // Update status breakdown
    m_successLabel->setText(QString::number(stats.successfulJobs));
    m_warningLabel->setText(QString::number(stats.warningJobs));
    m_failedLabel->setText(QString::number(stats.failedJobs));
    
    // Update progress bars
    if (stats.totalJobs > 0) {
        int successPercent = (stats.successfulJobs * 100) / stats.totalJobs;
        int warningPercent = (stats.warningJobs * 100) / stats.totalJobs;
        int failedPercent = (stats.failedJobs * 100) / stats.totalJobs;
        
        m_successBar->setMaximum(100);
        m_successBar->setValue(successPercent);
        
        m_warningBar->setMaximum(100);
        m_warningBar->setValue(warningPercent);
        
        m_failedBar->setMaximum(100);
        m_failedBar->setValue(failedPercent);
    } else {
        m_successBar->setValue(0);
        m_warningBar->setValue(0);
        m_failedBar->setValue(0);
    }
    
    // Update data summary
    m_totalFilesLabel->setText(QString::number(stats.totalFiles));
    m_totalBytesLabel->setText(formatBytes(stats.totalBytes));
    
    // Update date range
    if (stats.earliestJob.isValid() && stats.latestJob.isValid()) {
        QString dateRange = QString("%1 to %2")
            .arg(stats.earliestJob.toString("yyyy-MM-dd"))
            .arg(stats.latestJob.toString("yyyy-MM-dd"));
        m_dateRangeLabel->setText(dateRange);
    } else {
        m_dateRangeLabel->setText("N/A");
    }
}

QString BJobsStatisticsWidget::formatBytes(qint64 bytes) const
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
