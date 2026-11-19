#include "storagewidget.h"
#include "bstatusdelegate.h"
#include "buiscale.h"
#include "bbyteformat.h"
#include "storage/model/brecycleforecast.h"
#include "bsorttableitem.h"
#include "ui_storagewidget.h"
#include "blogging.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLocale>
#include <QMessageBox>
#include <QSignalBlocker>

#include <limits>

// Role of the name item (column 0): index of the row's volume in m_volumes.
// Sorting moves the rows, so a row number says nothing about the volume.
static constexpr int VolumeIndexRole = Qt::UserRole;

namespace {

QString unknownText()
{
    return StorageWidget::tr("unknown");
}

QString dateTimeText(const QDateTime &when)
{
    return when.isValid() ? QLocale::c().toString(when, QStringLiteral("yyyy-MM-dd HH:mm"))
                          : unknownText();
}

// Sort key of the "reusable from" column: now first, then by date; unknown
// and never last
qint64 reusableSortKey(const BRecycleForecast::Result &result)
{
    switch (result.kind) {
    case BRecycleForecast::Kind::Now: return 0;
    case BRecycleForecast::Kind::Date: return result.when.toSecsSinceEpoch();
    case BRecycleForecast::Kind::Unknown: return std::numeric_limits<qint64>::max() - 1;
    case BRecycleForecast::Kind::Never: break;
    }
    return std::numeric_limits<qint64>::max();
}

QString reusableText(const BRecycleForecast::Result &result, const QDateTime &now)
{
    QString text = BRecycleForecast::columnText(result);
    if (result.isDue(now)) {
        text += QLatin1Char(' ') + StorageWidget::tr("(due)");
    }
    return text;
}

} // namespace

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

    QLabel *poolLabel = new QLabel(tr("&Pool:"), this);
    poolLabel->setBuddy(m_poolFilter);
    topLayout->addWidget(poolLabel);
    m_poolFilter->setAccessibleName(tr("Pool filter"));
    BUiScale::track(m_poolFilter, [this]() { m_poolFilter->setMinimumWidth(BUiScale::px(150)); });
    topLayout->addWidget(m_poolFilter);

    topLayout->addSpacing(BUiScale::px(20));

    QLabel *statusLabel = new QLabel(tr("&Status:"), this);
    statusLabel->setBuddy(m_statusFilter);
    topLayout->addWidget(statusLabel);
    m_statusFilter->setAccessibleName(tr("Status filter"));
    BUiScale::track(m_statusFilter, [this]() { m_statusFilter->setMinimumWidth(BUiScale::px(150)); });
    topLayout->addWidget(m_statusFilter);

    topLayout->addStretch();

    m_refreshButton = new QPushButton(tr("Refresh"), this);
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshButton->setEnabled(false);
    topLayout->addWidget(m_refreshButton);

    layout->addLayout(topLayout);

    // Only what the volume list really says: the number of volumes and the
    // bytes written to them. A total or free capacity cannot be derived from
    // it (the limit of a File storage is its disk, which the list does not
    // know), so none is shown.
    QGroupBox *statsGroup = new QGroupBox(tr("Statistics"));
    QHBoxLayout *statsLayout = new QHBoxLayout(statsGroup);

    QLabel *totalLabel = new QLabel(tr("Volumes:"));
    totalLabel->setStyleSheet("font-weight: bold;");
    totalLabel->setBuddy(m_totalVolumesLabel);
    statsLayout->addWidget(totalLabel);
    statsLayout->addWidget(m_totalVolumesLabel);

    statsLayout->addSpacing(BUiScale::px(20));

    QLabel *usedLabel = new QLabel(tr("Used:"));
    usedLabel->setStyleSheet("font-weight: bold;");
    usedLabel->setBuddy(m_usedSizeLabel);
    statsLayout->addWidget(usedLabel);
    statsLayout->addWidget(m_usedSizeLabel);

    statsLayout->addStretch();

    layout->addWidget(statsGroup);

    // Shown when the Director sent the short list
    m_noticeLabel = new QLabel(this);
    m_noticeLabel->setWordWrap(true);
    m_noticeLabel->setVisible(false);
    layout->addWidget(m_noticeLabel);

    // Volume table
    m_volumeTable = new QTableWidget(this);
    m_volumeTable->setColumnCount(ColumnCount);
    m_volumeTable->setHorizontalHeaderLabels({
        tr("Volume"), tr("Pool"), tr("Status"), tr("Used"), tr("Limit"), tr("Jobs"),
        tr("Last Written"), tr("Retention"), tr("Reusable From"), tr("Reason")
    });
    m_volumeTable->horizontalHeaderItem(LimitColumn)->setToolTip(
        tr("Maximum Volume Bytes of the volume; \"none\" = no limit"));
    m_volumeTable->horizontalHeaderItem(ReusableColumn)->setToolTip(
        tr("From when Bareos may reuse the volume. It does so only when its pool "
           "needs a volume, so this is the earliest date, not the day of recycling."));
    m_volumeTable->setAccessibleName(tr("Volumes"));
    m_volumeTable->setAccessibleDescription(
        tr("One row per volume. Enter opens the details of the selected volume."));

    m_volumeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_volumeTable->horizontalHeader()->setSectionResizeMode(ReasonColumn, QHeaderView::Stretch);
    m_volumeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_volumeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_volumeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_volumeTable->setAlternatingRowColors(true);
    m_volumeTable->setWordWrap(false);
    m_volumeTable->setTextElideMode(Qt::ElideRight);
    // Tab leaves the table; the arrows move inside it
    m_volumeTable->setTabKeyNavigation(false);
    m_volumeTable->setSortingEnabled(true);
    m_volumeTable->setItemDelegateForColumn(StatusColumn, new BStatusDelegate(m_volumeTable));
    m_volumeTable->installEventFilter(this);

    layout->addWidget(m_volumeTable, 1);

    fillFilters();

    connect(m_refreshButton, &QPushButton::clicked, this, &StorageWidget::onRefreshClicked);
    connect(m_volumeTable, &QTableWidget::cellDoubleClicked, this,
            [this](int row, int) { onVolumeActivated(row); });
    connect(m_poolFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StorageWidget::applyFilters);
    connect(m_statusFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StorageWidget::applyFilters);
}

bool StorageWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_volumeTable && event->type() == QEvent::KeyPress) {
        const auto *key = static_cast<QKeyEvent *>(event);
        if ((key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter)
            && key->modifiers() == Qt::NoModifier && m_volumeTable->currentRow() >= 0) {
            onVolumeActivated(m_volumeTable->currentRow());
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

QDateTime StorageWidget::referenceTime() const
{
    return m_referenceTime.isValid() ? m_referenceTime : QDateTime::currentDateTime();
}

void StorageWidget::fillFilters()
{
    // Both lists are rebuilt with every reply: keep the selection, and do not
    // rebuild the table once per change while the boxes are refilled
    const QString selectedPool = m_poolFilter->currentData().toString();
    const QString selectedStatus = m_statusFilter->currentData().toString();
    const QSignalBlocker poolBlocker(m_poolFilter);
    const QSignalBlocker statusBlocker(m_statusFilter);

    m_poolFilter->clear();
    m_poolFilter->addItem(tr("All pools (%1)").arg(m_volumes.size()), QStringLiteral("all"));
    const QStringList pools = m_volumes.pools();
    for (const QString &pool : pools) {
        m_poolFilter->addItem(tr("%1 (%2)").arg(pool).arg(m_volumes.countInPool(pool)), pool);
    }
    // A pool that no longer exists falls back to "all"
    m_poolFilter->setCurrentIndex(qMax(0, m_poolFilter->findData(selectedPool)));

    m_statusFilter->clear();
    m_statusFilter->addItem(tr("All statuses (%1)").arg(m_volumes.size()), QStringLiteral("all"));
    const QStringList statuses = BVolumeList::allStatuses();
    for (const QString &status : statuses) {
        m_statusFilter->addItem(tr("%1 %2 (%3)")
                                    .arg(BVolumeList::statusLetter(status), status)
                                    .arg(m_volumes.countWithStatus(status)),
                                status);
    }
    m_statusFilter->setCurrentIndex(qMax(0, m_statusFilter->findData(selectedStatus)));
}

void StorageWidget::processJsonResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "StorageWidget: Processing JSON response";
#endif

    m_refreshButton->setEnabled(m_director != nullptr);

    // "llist volumes" and "list volumes" are grouped by pool
    // ({"<pool>": [...]}), "... pool=X|all" flat, and the Onesimus Director
    // over gRPC answers in its typed form ({"pools": {"<pool>": {"volumes": [...]}}})
    QString error;
    const BVolumeList volumes = BVolumeList::fromJson(jsonData.toUtf8(), m_knownPools, &error);
    if (!error.isEmpty() && volumes.isEmpty()) {
        BLOG_WARNING() << "StorageWidget: volume reply not usable:" << error;
        emit statusMessageChanged(tr("Error parsing volume data"));
        return;
    }
    m_volumes = volumes;

    if (!m_volumes.isEmpty() && !m_volumes.hasLongForm()) {
        m_noticeLabel->setText(
            tr("The Director sent the short volume list: limits, job counts and use durations "
               "are unknown, and so is when an Append volume will be closed."));
        m_noticeLabel->setVisible(true);
    } else {
        m_noticeLabel->setVisible(false);
    }

    fillFilters();
    applyFilters();

    // Only a refresh the user asked for on this tab reports to the status bar;
    // the load on connect and the automatic refresh stay quiet
    if (m_refreshRequested) {
        m_refreshRequested = false;
        emit statusMessageChanged(tr("%n volume(s) loaded", nullptr, m_volumes.size()));
    }
}

void StorageWidget::applyFilters()
{
    const QString poolFilter = m_poolFilter->currentData().toString();
    const QString statusFilter = m_statusFilter->currentData().toString();
    const QDateTime now = referenceTime();

    m_filtered.clear();
    const QList<BVolume> &all = m_volumes.volumes();
    for (int i = 0; i < all.size(); ++i) {
        const BVolume &volume = all.at(i);
        if (poolFilter != QLatin1String("all")
            && volume.pool.compare(poolFilter, Qt::CaseInsensitive) != 0) {
            continue;
        }
        if (statusFilter != QLatin1String("all") && volume.status != statusFilter) {
            continue;
        }
        m_filtered.append(i);
    }

    // The table is rebuilt from scratch (a reload never adds to old rows);
    // the selected volume is found again by its name afterwards
    QString selectedVolume;
    if (const QTableWidgetItem *current = m_volumeTable->item(m_volumeTable->currentRow(), VolumeColumn)) {
        selectedVolume = current->text();
    }

    m_volumeTable->setSortingEnabled(false);
    m_volumeTable->setRowCount(0);
    m_volumeTable->setRowCount(int(m_filtered.size()));
    // Job counts come only with the long list
    m_volumeTable->setColumnHidden(JobsColumn, !m_volumes.hasLongForm());

    for (int row = 0; row < m_filtered.size(); ++row) {
        const int index = m_filtered.at(row);
        const BVolume &v = all.at(index);
        const BRecycleForecast::Result forecast = BRecycleForecast::forecast(v, now);

        // Volume; the item a screen reader announces for the row
        auto *nameItem = new QTableWidgetItem(v.name);
        nameItem->setData(VolumeIndexRole, index);
        const QString usedText = v.volBytes ? BByteFormat::format(*v.volBytes) : unknownText();
        nameItem->setData(Qt::AccessibleTextRole,
                          tr("%1, %2, %3, last written %4, reusable from %5")
                              .arg(v.name, v.status.isEmpty() ? unknownText() : v.status, usedText,
                                   v.lastWritten.isValid() ? BRecycleForecast::formatDate(v.lastWritten)
                                                           : unknownText(),
                                   reusableText(forecast, now)));
        m_volumeTable->setItem(row, VolumeColumn, nameItem);

        m_volumeTable->setItem(row, PoolColumn,
                               new QTableWidgetItem(v.pool.isEmpty() ? unknownText() : v.pool));

        // A chip with letter, word and the colour of the state
        // (BStatusDelegate): colour is never the only cue
        auto *statusItem = new QTableWidgetItem(v.status.isEmpty() ? unknownText() : v.status);
        statusItem->setData(BStatus::CodeRole, BVolumeList::statusLetter(v.status));
        statusItem->setData(BStatus::ToneRole, int(BStatus::volumeTone(v.status)));
        statusItem->setData(Qt::AccessibleTextRole, v.status.isEmpty() ? unknownText() : v.status);
        if (v.status == QLatin1String("Error")) {
            QFont font = statusItem->font();
            font.setBold(true);
            statusItem->setFont(font);
        }
        m_volumeTable->setItem(row, StatusColumn, statusItem);

        m_volumeTable->setItem(row, UsedColumn, new BSortTableItem(usedText, v.volBytes.value_or(-1)));

        QString limitText = unknownText();
        qint64 limitKey = -1;
        if (v.maxVolBytes) {
            limitText = *v.maxVolBytes > 0 ? BByteFormat::format(*v.maxVolBytes) : tr("none");
            limitKey = *v.maxVolBytes > 0 ? *v.maxVolBytes : std::numeric_limits<qint64>::max();
        }
        m_volumeTable->setItem(row, LimitColumn, new BSortTableItem(limitText, limitKey));

        QString jobsText = unknownText();
        if (v.volJobs) {
            jobsText = v.maxVolJobs && *v.maxVolJobs > 0
                ? tr("%1 of %2").arg(*v.volJobs).arg(*v.maxVolJobs)
                : QString::number(*v.volJobs);
        }
        m_volumeTable->setItem(row, JobsColumn, new BSortTableItem(jobsText, v.volJobs.value_or(-1)));

        m_volumeTable->setItem(row, LastWrittenColumn,
                               new BSortTableItem(dateTimeText(v.lastWritten),
                                                  v.lastWritten.isValid() ? v.lastWritten.toSecsSinceEpoch() : -1));

        m_volumeTable->setItem(row, RetentionColumn,
                               new BSortTableItem(v.volRetention ? BRecycleForecast::formatDuration(*v.volRetention)
                                                                 : unknownText(),
                                                  v.volRetention.value_or(-1)));

        auto *reusableItem = new BSortTableItem(reusableText(forecast, now), reusableSortKey(forecast));
        reusableItem->setToolTip(forecast.reason);
        m_volumeTable->setItem(row, ReusableColumn, reusableItem);

        auto *reasonItem = new QTableWidgetItem(forecast.reason);
        reasonItem->setToolTip(forecast.reason);
        m_volumeTable->setItem(row, ReasonColumn, reasonItem);
    }

    m_volumeTable->setSortingEnabled(true);

    // Sorting has moved the rows: look the volume up by name
    if (!selectedVolume.isEmpty()) {
        for (int row = 0; row < m_volumeTable->rowCount(); ++row) {
            const QTableWidgetItem *nameItem = m_volumeTable->item(row, VolumeColumn);
            if (nameItem && nameItem->text() == selectedVolume) {
                m_volumeTable->setCurrentCell(row, VolumeColumn);
                break;
            }
        }
    }

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
    emit sendCommand(BDirector::Command::LlistVolumes, "");
}

const BVolume *StorageWidget::volumeAtRow(int row) const
{
    // The row is a position in the sorted table; the name item knows its volume
    const QTableWidgetItem *nameItem = m_volumeTable->item(row, VolumeColumn);
    if (!nameItem) {
        return nullptr;
    }
    bool ok = false;
    const int index = nameItem->data(VolumeIndexRole).toInt(&ok);
    if (!ok || index < 0 || index >= m_volumes.size()) {
        return nullptr;
    }
    return &m_volumes.volumes().at(index);
}

void StorageWidget::onVolumeActivated(int row)
{
    const BVolume *volume = volumeAtRow(row);
    if (!volume) {
        return;
    }
    const BVolume &v = *volume;
    const BRecycleForecast::Result forecast = BRecycleForecast::forecast(v, referenceTime());

    const QString unknown = tr("unknown (not in this list)");
    const auto bytes = [&](const std::optional<qint64> &value) {
        return value ? BByteFormat::format(*value) : unknown;
    };
    const auto number = [&](const std::optional<qint64> &value) {
        return value ? QString::number(*value) : unknown;
    };
    const auto date = [&](const QDateTime &value) {
        return value.isValid() ? dateTimeText(value) : unknown;
    };
    const auto duration = [&](const std::optional<qint64> &value) {
        return value ? BRecycleForecast::formatDuration(*value) : unknown;
    };

    QList<QPair<QString, QString>> rows = {
        {tr("Pool"), v.pool.isEmpty() ? unknown : v.pool},
        {tr("Status"), v.status.isEmpty() ? unknown : v.status},
        {tr("Media type"), v.mediaType.isEmpty() ? unknown : v.mediaType},
        {tr("Storage"), v.storage.isEmpty() ? unknown : v.storage},
        {tr("Used"), bytes(v.volBytes)},
        {tr("Limit"), v.maxVolBytes && *v.maxVolBytes == 0 ? tr("none") : bytes(v.maxVolBytes)},
        {tr("Jobs"), number(v.volJobs)},
        {tr("Maximum jobs"), v.maxVolJobs && *v.maxVolJobs == 0 ? tr("none") : number(v.maxVolJobs)},
        {tr("Errors"), number(v.volErrors)},
        {tr("First written"), date(v.firstWritten)},
        {tr("Last written"), date(v.lastWritten)},
        {tr("Labeled"), date(v.labelDate)},
        {tr("Volume retention"), duration(v.volRetention)},
        {tr("Use duration"), v.volUseDuration && *v.volUseDuration == 0 ? tr("none") : duration(v.volUseDuration)},
        {tr("Recycle"), v.recycle ? (*v.recycle ? tr("yes") : tr("no")) : unknown},
        {tr("Recycle pool"), v.longForm ? (v.recyclePool.isEmpty() ? tr("none") : v.recyclePool) : unknown},
        {tr("Times recycled"), number(v.recycleCount)},
        {tr("Reusable from"), BRecycleForecast::columnText(forecast)},
    };

    QString details = QStringLiteral("<h3>%1</h3><table>").arg(v.name.toHtmlEscaped());
    for (const auto &entry : std::as_const(rows)) {
        details += QStringLiteral("<tr><td><b>%1</b>&nbsp;&nbsp;</td><td>%2</td></tr>")
                       .arg(entry.first.toHtmlEscaped(), entry.second.toHtmlEscaped());
    }
    details += QStringLiteral("</table><p>%1</p>").arg(forecast.reason.toHtmlEscaped());

    QMessageBox::information(this, tr("Volume Details"), details);
}

void StorageWidget::updateStatistics()
{
    qint64 usedSize = 0;
    for (const int index : std::as_const(m_filtered)) {
        usedSize += m_volumes.volumes().at(index).volBytes.value_or(0);
    }

    m_totalVolumesLabel->setText(QString::number(m_filtered.size()));
    m_usedSizeLabel->setText(BByteFormat::format(usedSize));
}

void StorageWidget::clearData()
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "StorageWidget: Clearing all data";
#endif

    m_volumeTable->clearContents();
    m_volumeTable->setRowCount(0);

    m_refreshRequested = false;

    m_volumes = BVolumeList();
    m_filtered.clear();
    m_noticeLabel->setVisible(false);

    fillFilters();
    m_poolFilter->setCurrentIndex(0);
    m_statusFilter->setCurrentIndex(0);

    m_totalVolumesLabel->setText("0");
    m_usedSizeLabel->setText("0 B");
}
