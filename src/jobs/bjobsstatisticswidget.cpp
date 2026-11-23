// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "jobs/bjobsstatisticswidget.h"
#include "buiscale.h"
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
    QGroupBox *summaryGroup = new QGroupBox(tr("Job Summary"), this);
    QFormLayout *summaryLayout = new QFormLayout(summaryGroup);
    
    m_totalJobsLabel = new QLabel("0", this);
    m_totalJobsLabel->setStyleSheet("font-weight: bold;");
    BUiScale::bind(m_totalJobsLabel, BUiScale::Title);
    summaryLayout->addRow(tr("Total Jobs:"), m_totalJobsLabel);
    
    m_selectedLabel = new QLabel("0", this);
    summaryLayout->addRow(tr("Selected:"), m_selectedLabel);
    
    mainLayout->addWidget(summaryGroup);
    
    // Status breakdown group
    QGroupBox *statusGroup = new QGroupBox(tr("Status Breakdown"), this);
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);
    
    // Successful jobs
    QHBoxLayout *successLayout = new QHBoxLayout();
    m_successLabel = new QLabel("0", this);
    BUiScale::track(m_successLabel, [this]() { m_successLabel->setMinimumWidth(BUiScale::px(50)); });
    successLayout->addWidget(new QLabel(tr("Successful:"), this));
    successLayout->addWidget(m_successLabel);
    statusLayout->addLayout(successLayout);
    
    m_successBar = new QProgressBar(this);
    // Chunk colour from the theme: QProgressBar[state="success"] (status tokens)
    m_successBar->setProperty("state", QStringLiteral("success"));
    m_successBar->setTextVisible(true);
    m_successBar->setFormat("%p%");
    statusLayout->addWidget(m_successBar);
    
    // Warning jobs
    QHBoxLayout *warningLayout = new QHBoxLayout();
    m_warningLabel = new QLabel("0", this);
    BUiScale::track(m_warningLabel, [this]() { m_warningLabel->setMinimumWidth(BUiScale::px(50)); });
    warningLayout->addWidget(new QLabel(tr("Warnings:"), this));
    warningLayout->addWidget(m_warningLabel);
    statusLayout->addLayout(warningLayout);
    
    m_warningBar = new QProgressBar(this);
    // Chunk colour from the theme: QProgressBar[state="warning"] (status tokens)
    m_warningBar->setProperty("state", QStringLiteral("warning"));
    m_warningBar->setTextVisible(true);
    m_warningBar->setFormat("%p%");
    statusLayout->addWidget(m_warningBar);
    
    // Failed jobs
    QHBoxLayout *failedLayout = new QHBoxLayout();
    m_failedLabel = new QLabel("0", this);
    BUiScale::track(m_failedLabel, [this]() { m_failedLabel->setMinimumWidth(BUiScale::px(50)); });
    failedLayout->addWidget(new QLabel(tr("Failed:"), this));
    failedLayout->addWidget(m_failedLabel);
    statusLayout->addLayout(failedLayout);
    
    m_failedBar = new QProgressBar(this);
    // Chunk colour from the theme: QProgressBar[state="error"] (status tokens)
    m_failedBar->setProperty("state", QStringLiteral("error"));
    m_failedBar->setTextVisible(true);
    m_failedBar->setFormat("%p%");
    statusLayout->addWidget(m_failedBar);
    
    mainLayout->addWidget(statusGroup);
    
    // Data summary group
    QGroupBox *dataGroup = new QGroupBox(tr("Data Summary"), this);
    QFormLayout *dataLayout = new QFormLayout(dataGroup);
    
    m_totalFilesLabel = new QLabel("0", this);
    dataLayout->addRow(tr("Total Files:"), m_totalFilesLabel);
    
    m_totalBytesLabel = new QLabel("0 B", this);
    dataLayout->addRow(tr("Total Size:"), m_totalBytesLabel);
    
    m_dateRangeLabel = new QLabel(tr("N/A"), this);
    dataLayout->addRow(tr("Date Range:"), m_dateRangeLabel);
    
    mainLayout->addWidget(dataGroup);

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
        QString dateRange = tr("%1 to %2")
            .arg(stats.earliestJob.toString("yyyy-MM-dd"))
            .arg(stats.latestJob.toString("yyyy-MM-dd"));
        m_dateRangeLabel->setText(dateRange);
    } else {
        m_dateRangeLabel->setText(tr("N/A"));
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
