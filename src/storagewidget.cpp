#include "storagewidget.h"
#include "ui_storagewidget.h"
#include <QHeaderView>
#include <QVBoxLayout>

StorageWidget::StorageWidget(BDirector *director, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StorageWidget)
    , m_director(director)
{
    ui->setupUi(this);
    setupUI();
    
    // connect(m_director, &BDirector::volumesReceived, this, &StorageWidget::onVolumesReceived);
}

StorageWidget::~StorageWidget()
{
    delete ui;
}

void StorageWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    m_refreshButton = new QPushButton("Aktualisieren", this);
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(m_refreshButton);
    
    layout->addLayout(toolbarLayout);
    
    // Volume-Tabelle
    m_volumeTable = new QTableWidget(this);
    m_volumeTable->setColumnCount(6);
    m_volumeTable->setHorizontalHeaderLabels({
        "Volume Name", "Pool", "Media Typ", "Status", "Bytes", "Max Bytes"
    });
    
    m_volumeTable->horizontalHeader()->setStretchLastSection(true);
    m_volumeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_volumeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_volumeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_volumeTable->setAlternatingRowColors(true);
    m_volumeTable->setSortingEnabled(true);
    
    layout->addWidget(m_volumeTable);
    
    // Verbinde Buttons
    connect(m_refreshButton, &QPushButton::clicked, this, &StorageWidget::onRefreshClicked);
    connect(m_volumeTable, &QTableWidget::itemSelectionChanged, this, &StorageWidget::onVolumeSelectionChanged);
}

void StorageWidget::onVolumesReceived(const QList<BDirector::VolumeInfo> &volumes)
{
    updateVolumeTable(volumes);
}

void StorageWidget::updateVolumeTable(const QList<BDirector::VolumeInfo> &volumes)
{
    m_volumeTable->setSortingEnabled(false);
    m_volumeTable->setRowCount(volumes.size());
    
    for (int i = 0; i < volumes.size(); ++i) {
        const BDirector::VolumeInfo &volume = volumes[i];
        
        m_volumeTable->setItem(i, 0, new QTableWidgetItem(volume.volumeName));
        m_volumeTable->setItem(i, 1, new QTableWidgetItem(volume.poolName));
        m_volumeTable->setItem(i, 2, new QTableWidgetItem(volume.mediaType));
        
        QTableWidgetItem *statusItem = new QTableWidgetItem(volume.status);
        if (volume.status == "Append") {
            statusItem->setBackground(QBrush(QColor(144, 238, 144))); // Hellgrün
        } else if (volume.status == "Full") {
            statusItem->setBackground(QBrush(QColor(255, 165, 0))); // Orange
        } else if (volume.status == "Error") {
            statusItem->setBackground(QBrush(QColor(255, 182, 193))); // Hellrot
        }
        m_volumeTable->setItem(i, 3, statusItem);
        
        m_volumeTable->setItem(i, 4, new QTableWidgetItem(formatBytes(volume.volumeBytes)));
        m_volumeTable->setItem(i, 5, new QTableWidgetItem(formatBytes(volume.maxVolumeBytes)));
    }
    
    m_volumeTable->setSortingEnabled(true);
}

void StorageWidget::onRefreshClicked()
{
    emit sendCommand(BDirector::Command::ListVolumes, "");
}

void StorageWidget::onVolumeSelectionChanged()
{
    // Kann für zukünftige Funktionen verwendet werden
}

QString StorageWidget::formatBytes(qint64 bytes)
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    const qint64 TB = GB * 1024;
    
    if (bytes >= TB) {
        return QString::number(bytes / (double)TB, 'f', 2) + " TB";
    } else if (bytes >= GB) {
        return QString::number(bytes / (double)GB, 'f', 2) + " GB";
    } else if (bytes >= MB) {
        return QString::number(bytes / (double)MB, 'f', 2) + " MB";
    } else if (bytes >= KB) {
        return QString::number(bytes / (double)KB, 'f', 2) + " KB";
    } else {
        return QString::number(bytes) + " B";
    }
}
