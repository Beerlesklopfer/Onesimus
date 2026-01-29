#include "storagewidget.h"
#include "ui_storagewidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMessageBox>
#include <QSet>

StorageWidget::StorageWidget(BDirector *director, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StorageWidget)
    , m_director(director)
    , m_poolFilter(new QComboBox(this))
    , m_statusFilter(new QComboBox(this))
    , m_totalVolumesLabel(new QLabel("0", this))
    , m_totalSizeLabel(new QLabel("0 B", this))
    , m_usedSizeLabel(new QLabel("0 B", this))
    , m_availableSizeLabel(new QLabel("0 B", this))
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

    // Top bar with filters and refresh button
    QHBoxLayout *topLayout = new QHBoxLayout();

    // Pool filter
    topLayout->addWidget(new QLabel(tr("Pool:")));
    m_poolFilter->addItem(tr("Alle"), "all");
    m_poolFilter->setMinimumWidth(150);
    topLayout->addWidget(m_poolFilter);

    topLayout->addSpacing(20);

    // Status filter
    topLayout->addWidget(new QLabel(tr("Status:")));
    m_statusFilter->addItem(tr("Alle"), "all");
    m_statusFilter->addItem(tr("Append"), "Append");
    m_statusFilter->addItem(tr("Full"), "Full");
    m_statusFilter->addItem(tr("Error"), "Error");
    m_statusFilter->addItem(tr("Purged"), "Purged");
    m_statusFilter->setMinimumWidth(120);
    topLayout->addWidget(m_statusFilter);

    topLayout->addStretch();

    // Refresh button
    m_refreshButton = new QPushButton("Aktualisieren", this);
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshButton->setEnabled(false);
    topLayout->addWidget(m_refreshButton);

    layout->addLayout(topLayout);

    // Statistics bar
    QGroupBox *statsGroup = new QGroupBox(tr("Statistiken"));
    QHBoxLayout *statsLayout = new QHBoxLayout(statsGroup);

    QLabel *totalLabel = new QLabel(tr("Volumes:"));
    totalLabel->setStyleSheet("font-weight: bold;");
    statsLayout->addWidget(totalLabel);
    statsLayout->addWidget(m_totalVolumesLabel);

    statsLayout->addSpacing(20);

    QLabel *totalSizeLabel = new QLabel(tr("Gesamtgröße:"));
    totalSizeLabel->setStyleSheet("font-weight: bold;");
    statsLayout->addWidget(totalSizeLabel);
    statsLayout->addWidget(m_totalSizeLabel);

    statsLayout->addSpacing(20);

    QLabel *usedLabel = new QLabel(tr("Belegt:"));
    usedLabel->setStyleSheet("font-weight: bold; color: orange;");
    statsLayout->addWidget(usedLabel);
    m_usedSizeLabel->setStyleSheet("color: orange;");
    statsLayout->addWidget(m_usedSizeLabel);

    statsLayout->addSpacing(20);

    QLabel *availLabel = new QLabel(tr("Verfügbar:"));
    availLabel->setStyleSheet("font-weight: bold; color: green;");
    statsLayout->addWidget(availLabel);
    m_availableSizeLabel->setStyleSheet("color: green;");
    statsLayout->addWidget(m_availableSizeLabel);

    statsLayout->addStretch();

    layout->addWidget(statsGroup);

    // Volume table
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

    // Connect signals
    connect(m_refreshButton, &QPushButton::clicked, this, &StorageWidget::onRefreshClicked);
    connect(m_volumeTable, &QTableWidget::itemSelectionChanged, this, &StorageWidget::onVolumeSelectionChanged);
    connect(m_volumeTable, &QTableWidget::cellDoubleClicked, this, &StorageWidget::onVolumeDoubleClicked);
    connect(m_poolFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StorageWidget::onPoolFilterChanged);
    connect(m_statusFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StorageWidget::onStatusFilterChanged);
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

void StorageWidget::processJsonResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    qDebug() << "StorageWidget: Processing JSON response";
#endif

    // Re-enable refresh button
    m_refreshButton->setEnabled(m_director != nullptr);

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "StorageWidget: Failed to parse JSON:" << parseError.errorString();
        emit statusMessageChanged("Fehler beim Parsen der Volume-Daten");
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "StorageWidget: JSON response is not an object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonArray volumesArray = result["volumes"].toArray();

    // Store all volumes
    m_allVolumes = volumesArray;

    // Update pool filter combo box
    m_poolFilter->clear();
    m_poolFilter->addItem(tr("Alle"), "all");

    QSet<QString> pools;
    for (const QJsonValue &volumeVal : volumesArray) {
        QJsonObject volume = volumeVal.toObject();
        QString poolName = volume["pool"].toString().trimmed();
        if (!poolName.isEmpty()) {
            pools.insert(poolName);
        }
    }

    QStringList poolList = pools.values();
    poolList.sort(Qt::CaseInsensitive);
    for (const QString &pool : poolList) {
        m_poolFilter->addItem(pool, pool);
    }

#ifdef IS_DEVELOPER
    qDebug() << "✓ Loaded" << volumesArray.size() << "volumes from" << pools.size() << "pools";
#endif

    // Apply filters and update display
    applyFilters();

    emit statusMessageChanged(QString("%1 Volumes geladen").arg(volumesArray.size()));
}

void StorageWidget::applyFilters()
{
    QString poolFilter = m_poolFilter->currentData().toString();
    QString statusFilter = m_statusFilter->currentData().toString();

    m_filteredVolumes = QJsonArray();

    for (const QJsonValue &volumeVal : m_allVolumes) {
        QJsonObject volume = volumeVal.toObject();
        QString poolName = volume["pool"].toString();
        QString status = volume["volstatus"].toString();

        // Apply pool filter
        if (poolFilter != "all") {
            if (poolName != poolFilter) {
                continue;
            }
        }

        // Apply status filter
        if (statusFilter != "all") {
            if (status != statusFilter) {
                continue;
            }
        }

        m_filteredVolumes.append(volumeVal);
    }

    // Update table with filtered volumes
    m_volumeTable->setSortingEnabled(false);
    m_volumeTable->setRowCount(0);
    m_volumeTable->setRowCount(m_filteredVolumes.size());

    for (int i = 0; i < m_filteredVolumes.size(); ++i) {
        QJsonObject volume = m_filteredVolumes[i].toObject();

        QString volumeName = volume["volumename"].toString();
        QString poolName = volume["pool"].toString();
        QString mediaType = volume["mediatype"].toString();
        QString status = volume["volstatus"].toString();
        qint64 volumeBytes = volume["volbytes"].toVariant().toLongLong();
        qint64 maxBytes = volume["maxvolbytes"].toVariant().toLongLong();

        m_volumeTable->setItem(i, 0, new QTableWidgetItem(volumeName));
        m_volumeTable->setItem(i, 1, new QTableWidgetItem(poolName));
        m_volumeTable->setItem(i, 2, new QTableWidgetItem(mediaType));

        QTableWidgetItem *statusItem = new QTableWidgetItem(status);
        if (status == "Append") {
            statusItem->setBackground(QBrush(QColor(144, 238, 144))); // Light green
        } else if (status == "Full") {
            statusItem->setBackground(QBrush(QColor(255, 165, 0))); // Orange
        } else if (status == "Error") {
            statusItem->setBackground(QBrush(QColor(255, 182, 193))); // Light red
        } else if (status == "Purged") {
            statusItem->setBackground(QBrush(QColor(211, 211, 211))); // Light gray
        }
        m_volumeTable->setItem(i, 3, statusItem);

        m_volumeTable->setItem(i, 4, new QTableWidgetItem(formatBytes(volumeBytes)));
        m_volumeTable->setItem(i, 5, new QTableWidgetItem(formatBytes(maxBytes)));
    }

    m_volumeTable->setSortingEnabled(true);

    // Update statistics
    updateStatistics();
}

void StorageWidget::setConnectionState(bool connected)
{
    m_refreshButton->setEnabled(connected);

    if (!connected) {
        clearData();
    }
}

void StorageWidget::onRefreshClicked()
{
    m_refreshButton->setEnabled(false);
    emit statusMessageChanged("Aktualisiere Volume-Liste...");
    emit sendCommand(BDirector::Command::ListVolumes, "");
}

void StorageWidget::onVolumeSelectionChanged()
{
    // Kann für zukünftige Funktionen verwendet werden
}

void StorageWidget::onVolumeDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    if (row < 0 || row >= m_filteredVolumes.size()) {
        return;
    }

    QJsonObject volume = m_filteredVolumes[row].toObject();
    QString volumeName = volume["volumename"].toString();
    QString poolName = volume["pool"].toString();
    QString status = volume["volstatus"].toString();
    QString mediaType = volume["mediatype"].toString();
    qint64 volumeBytes = volume["volbytes"].toVariant().toLongLong();
    qint64 maxBytes = volume["maxvolbytes"].toVariant().toLongLong();

    // Show volume details dialog
    QString details = tr("<h3>Volume Details: %1</h3>").arg(volumeName);
    details += tr("<p><b>Pool:</b> %1</p>").arg(poolName);
    details += tr("<p><b>Status:</b> %1</p>").arg(status);
    details += tr("<p><b>Media Type:</b> %1</p>").arg(mediaType);
    details += tr("<p><b>Current Size:</b> %1</p>").arg(formatBytes(volumeBytes));
    details += tr("<p><b>Maximum Size:</b> %1</p>").arg(formatBytes(maxBytes));

    if (maxBytes > 0) {
        double percentage = (volumeBytes * 100.0) / maxBytes;
        details += tr("<p><b>Usage:</b> %1%</p>").arg(QString::number(percentage, 'f', 1));
    }

    QMessageBox::information(this, tr("Volume Details"), details);
}

void StorageWidget::onPoolFilterChanged(int index)
{
    Q_UNUSED(index);
    applyFilters();
}

void StorageWidget::onStatusFilterChanged(int index)
{
    Q_UNUSED(index);
    applyFilters();
}

void StorageWidget::updateStatistics()
{
    qint64 totalSize = 0;
    qint64 usedSize = 0;
    qint64 maxSize = 0;

    for (const QJsonValue &volumeVal : m_filteredVolumes) {
        QJsonObject volume = volumeVal.toObject();
        qint64 volumeBytes = volume["volbytes"].toVariant().toLongLong();
        qint64 maxBytes = volume["maxvolbytes"].toVariant().toLongLong();

        usedSize += volumeBytes;
        maxSize += maxBytes;
    }

    totalSize = maxSize;
    qint64 availableSize = maxSize - usedSize;

    m_totalVolumesLabel->setText(QString::number(m_filteredVolumes.size()));
    m_totalSizeLabel->setText(formatBytes(totalSize));
    m_usedSizeLabel->setText(formatBytes(usedSize));
    m_availableSizeLabel->setText(formatBytes(availableSize));
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

void StorageWidget::clearData()
{
#ifdef IS_DEVELOPER
    qDebug() << "StorageWidget: Clearing all data";
#endif

    // Clear volume table
    m_volumeTable->clearContents();
    m_volumeTable->setRowCount(0);

    // Clear data arrays
    m_allVolumes = QJsonArray();
    m_filteredVolumes = QJsonArray();

    // Reset filters
    m_poolFilter->setCurrentIndex(0);
    m_statusFilter->setCurrentIndex(0);

    // Reset statistics
    m_totalVolumesLabel->setText("0");
    m_totalSizeLabel->setText("0 B");
    m_usedSizeLabel->setText("0 B");
    m_availableSizeLabel->setText("0 B");
}
