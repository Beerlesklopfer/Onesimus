#include "storagewidget.h"
#include "buiscale.h"
#include "bbyteformat.h"
#include "models/bvolumejson.h"
#include "bsorttableitem.h"
#include "ui_storagewidget.h"
#include "blogging.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QSet>
#include <QSignalBlocker>

// Role of the name item (column 0): index of the row's volume in m_filteredVolumes.
// Sorting moves the rows, so a row number says nothing about the volume.
static constexpr int VolumeIndexRole = Qt::UserRole;

StorageWidget::StorageWidget(BDirector *director, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StorageWidget)
    , m_director(director)
    , m_poolFilter(new QComboBox(this))
    , m_statusFilter(new QComboBox(this))
    , m_totalVolumesLabel(new QLabel("0", this))
    , m_usedSizeLabel(new QLabel("0 B", this))
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
    m_poolFilter->addItem(tr("All"), "all");
    BUiScale::track(m_poolFilter, [this]() { m_poolFilter->setMinimumWidth(BUiScale::px(150)); });
    topLayout->addWidget(m_poolFilter);

    topLayout->addSpacing(20);

    // Status filter
    topLayout->addWidget(new QLabel(tr("Status:")));
    m_statusFilter->addItem(tr("All"), "all");
    m_statusFilter->addItem(tr("Append"), "Append");
    m_statusFilter->addItem(tr("Full"), "Full");
    m_statusFilter->addItem(tr("Error"), "Error");
    m_statusFilter->addItem(tr("Purged"), "Purged");
    BUiScale::track(m_statusFilter, [this]() { m_statusFilter->setMinimumWidth(BUiScale::px(120)); });
    topLayout->addWidget(m_statusFilter);

    topLayout->addStretch();

    // Refresh button
    m_refreshButton = new QPushButton(tr("Refresh"), this);
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshButton->setEnabled(false);
    topLayout->addWidget(m_refreshButton);

    layout->addLayout(topLayout);

    // Statistics bar
    // Only what the volume list really says: the number of volumes and the
    // bytes written to them. A total or free capacity cannot be derived from
    // it (the limit of a File storage is its disk, which the list does not
    // know), so none is shown.
    QGroupBox *statsGroup = new QGroupBox(tr("Statistics"));
    QHBoxLayout *statsLayout = new QHBoxLayout(statsGroup);

    QLabel *totalLabel = new QLabel(tr("Volumes:"));
    totalLabel->setStyleSheet("font-weight: bold;");
    statsLayout->addWidget(totalLabel);
    statsLayout->addWidget(m_totalVolumesLabel);

    statsLayout->addSpacing(20);

    QLabel *usedLabel = new QLabel(tr("Used:"));
    usedLabel->setStyleSheet("font-weight: bold; color: orange;");
    statsLayout->addWidget(usedLabel);
    m_usedSizeLabel->setStyleSheet("color: orange;");
    statsLayout->addWidget(m_usedSizeLabel);

    statsLayout->addStretch();

    layout->addWidget(statsGroup);

    // Volume table
    m_volumeTable = new QTableWidget(this);
    m_volumeTable->setColumnCount(5);
    m_volumeTable->setHorizontalHeaderLabels({
        tr("Volume Name"), tr("Pool"), tr("Media Type"), tr("Status"), tr("Bytes")
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
        
        m_volumeTable->setItem(i, 4, new QTableWidgetItem(BByteFormat::format(volume.volumeBytes)));
    }
    
    m_volumeTable->setSortingEnabled(true);
}

void StorageWidget::processJsonResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "StorageWidget: Processing JSON response";
#endif

    // Re-enable refresh button
    m_refreshButton->setEnabled(m_director != nullptr);

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "StorageWidget: Failed to parse JSON:" << parseError.errorString();
        emit statusMessageChanged(tr("Error parsing volume data"));
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "StorageWidget: JSON response is not an object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    // "list volumes" returns volumes grouped by pool ({"<pool>": [...]}),
    // "list volumes pool=X|all" a flat array, and the Onesimus Director over
    // gRPC its typed form ({"pools": {"<pool>": {"volumes": [...]}}})
    bool recognized = false;
    QJsonArray volumesArray = BVolumeJson::volumesFromResult(result, m_knownPools, &recognized);
    if (!recognized) {
        BLOG_WARNING() << "StorageWidget: volume reply in an unknown shape, keys:"
                       << result.keys();
    }

    // Store all volumes
    m_allVolumes = volumesArray;

    // Update pool filter combo box. The list is reloaded with every refresh:
    // keep the selected pool, and do not rebuild the table once per combo
    // box change while the box is refilled.
    const QString selectedPool = m_poolFilter->currentData().toString();
    const QSignalBlocker poolFilterBlocker(m_poolFilter);
    m_poolFilter->clear();
    m_poolFilter->addItem(tr("All"), "all");

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
    // A pool that no longer exists falls back to "all"
    m_poolFilter->setCurrentIndex(qMax(0, m_poolFilter->findData(selectedPool)));

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "✓ Loaded" << volumesArray.size() << "volumes from" << pools.size() << "pools";
#endif

    // Apply filters and update display
    applyFilters();

    // Only a refresh the user asked for on this tab reports to the status bar;
    // the load on connect and the automatic refresh stay quiet
    if (m_refreshRequested) {
        m_refreshRequested = false;
        emit statusMessageChanged(tr("%n volume(s) loaded", nullptr, int(volumesArray.size())));
    }
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

    // The table is rebuilt from scratch (a reload never adds to old rows);
    // the selected volume is found again by its name afterwards
    QString selectedVolume;
    if (const QTableWidgetItem *current = m_volumeTable->item(m_volumeTable->currentRow(), 0)) {
        selectedVolume = current->text();
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

        QTableWidgetItem *nameItem = new QTableWidgetItem(volumeName);
        nameItem->setData(VolumeIndexRole, i);
        m_volumeTable->setItem(i, 0, nameItem);
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
        if (statusItem->background().style() != Qt::NoBrush) {
            // All four backgrounds are light: the theme's text colour would
            // be unreadable on them in the dark theme
            statusItem->setForeground(QBrush(Qt::black));
        }
        m_volumeTable->setItem(i, 3, statusItem);

        // Sizes sort by their byte count, not by the formatted text
        m_volumeTable->setItem(i, 4, new BSortTableItem(BByteFormat::format(volumeBytes), volumeBytes));
    }

    m_volumeTable->setSortingEnabled(true);

    // Sorting has moved the rows: look the volume up by name
    if (!selectedVolume.isEmpty()) {
        for (int row = 0; row < m_volumeTable->rowCount(); ++row) {
            const QTableWidgetItem *nameItem = m_volumeTable->item(row, 0);
            if (nameItem && nameItem->text() == selectedVolume) {
                m_volumeTable->setCurrentCell(row, 0);
                break;
            }
        }
    }

    // Update statistics
    updateStatistics();
}

void StorageWidget::setKnownPools(const QStringList &pools)
{
    m_knownPools = pools;
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
    m_refreshRequested = true;
    emit statusMessageChanged(tr("Refreshing the volume list..."));
    emit sendCommand(BDirector::Command::ListVolumes, "");
}

void StorageWidget::onVolumeSelectionChanged()
{
    // Kann für zukünftige Funktionen verwendet werden
}

void StorageWidget::onVolumeDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    // The row is a position in the sorted table; the name item knows its volume
    const QTableWidgetItem *nameItem = m_volumeTable->item(row, 0);
    if (!nameItem) {
        return;
    }
    bool ok = false;
    const int volumeIndex = nameItem->data(VolumeIndexRole).toInt(&ok);
    if (!ok || volumeIndex < 0 || volumeIndex >= m_filteredVolumes.size()) {
        return;
    }

    QJsonObject volume = m_filteredVolumes[volumeIndex].toObject();
    QString volumeName = volume["volumename"].toString();
    QString poolName = volume["pool"].toString();
    QString status = volume["volstatus"].toString();
    QString mediaType = volume["mediatype"].toString();
    qint64 volumeBytes = volume["volbytes"].toVariant().toLongLong();

    // Show volume details dialog
    QString details = tr("<h3>Volume Details: %1</h3>").arg(volumeName);
    details += tr("<p><b>Pool:</b> %1</p>").arg(poolName);
    details += tr("<p><b>Status:</b> %1</p>").arg(status);
    details += tr("<p><b>Media Type:</b> %1</p>").arg(mediaType);
    details += tr("<p><b>Current Size:</b> %1</p>").arg(BByteFormat::format(volumeBytes));

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
    qint64 usedSize = 0;
    for (const QJsonValue &volumeVal : m_filteredVolumes) {
        usedSize += volumeVal.toObject()["volbytes"].toVariant().toLongLong();
    }

    m_totalVolumesLabel->setText(QString::number(m_filteredVolumes.size()));
    m_usedSizeLabel->setText(BByteFormat::format(usedSize));
}

void StorageWidget::clearData()
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "StorageWidget: Clearing all data";
#endif

    // Clear volume table
    m_volumeTable->clearContents();
    m_volumeTable->setRowCount(0);

    m_refreshRequested = false;

    // Clear data arrays
    m_allVolumes = QJsonArray();
    m_filteredVolumes = QJsonArray();

    // Reset filters
    m_poolFilter->setCurrentIndex(0);
    m_statusFilter->setCurrentIndex(0);

    // Reset statistics
    m_totalVolumesLabel->setText("0");
    m_usedSizeLabel->setText("0 B");
}
