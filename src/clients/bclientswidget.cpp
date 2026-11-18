#include "clients/bclientswidget.h"
#include "clients/bclientsmodel.h"
#include "clients/bclientdetailsdialog.h"
#include "bcheckableheaderview.h"
#include "config/bcolumnconfiguration.h"
#include "blogging.h"
#include "config/bsettings.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QMenu>
#include <QTimer>
#include <QVBoxLayout>

BClientsWidget::BClientsWidget(BDirector *director, QWidget *parent)
    : QWidget(parent)
    , m_tableView(new QTableView(this))
    , m_model(new BClientsModel(director, this))
    , m_headerView(new BCheckableHeaderView(Qt::Horizontal, this))
    , m_columnConfig(new BColumnConfiguration(this))
    , m_autoSaveTimer(new QTimer(this))
    , m_clientFilter(new QComboBox(this))
    , m_statusFilter(new QComboBox(this))
    , m_autoRefreshCheck(new QCheckBox(tr("Auto"), this))
    , m_refreshButton(new QPushButton(tr("Refresh"), this))
    , m_totalClientsLabel(new QLabel("0", this))
    , m_recentClientsLabel(new QLabel("0", this))
    , m_staleClientsLabel(new QLabel("0", this))
    , m_neverClientsLabel(new QLabel("0", this))
    , m_unknownClientsLabel(new QLabel("0", this))
    , m_director(director)
    , m_filterDebounceTimer(new QTimer(this))
{
    setupUI();
    setupConnections();
}

void BClientsWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Top bar with filters and refresh button
    QHBoxLayout *topLayout = new QHBoxLayout();

    // Client name filter
    topLayout->addWidget(new QLabel(tr("Client:")));
    m_clientFilter->setEditable(true);
    m_clientFilter->setInsertPolicy(QComboBox::NoInsert);
    m_clientFilter->setPlaceholderText(tr("Filter by client..."));
    m_clientFilter->setMinimumWidth(200);
    topLayout->addWidget(m_clientFilter);

    topLayout->addSpacing(20);

    // Filter by the age of the last backup (not a reachability status)
    topLayout->addWidget(new QLabel(tr("Last backup:")));
    m_statusFilter->addItem(tr("All"), "all");
    m_statusFilter->addItem(tr("Recent"), "recent");
    m_statusFilter->addItem(tr("Stale"), "stale");
    m_statusFilter->addItem(tr("Never"), "never");
    m_statusFilter->addItem(tr("Unknown"), "unknown");
    m_statusFilter->setMinimumWidth(120);
    topLayout->addWidget(m_statusFilter);

    topLayout->addStretch();

    // Auto-refresh checkbox
    m_autoRefreshCheck->setToolTip(tr("Enable automatic refresh"));
    m_autoRefreshCheck->setEnabled(false);
    m_autoRefreshCheck->setChecked(BSettings::instance().behaviorAutoRefresh());
    topLayout->addWidget(m_autoRefreshCheck);

    // Refresh button
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshButton->setEnabled(false);
    topLayout->addWidget(m_refreshButton);

    mainLayout->addLayout(topLayout);

    // Statistics bar
    QGroupBox *statsGroup = new QGroupBox(tr("Statistics"));
    QHBoxLayout *statsLayout = new QHBoxLayout(statsGroup);

    QLabel *totalLabel = new QLabel(tr("Total:"));
    totalLabel->setObjectName("clientStatsBoldLabel");
    statsLayout->addWidget(totalLabel);
    statsLayout->addWidget(m_totalClientsLabel);

    statsLayout->addSpacing(20);

    // One counter per state of the last backup. "Never" and "Unknown" are
    // counted on their own: no information is not the same as an old backup.
    // Colours come from the themes' QLabel[state=...] selectors.
    const auto addCounter = [statsLayout](const QString &caption, QLabel *valueLabel,
                                          const char *state, const QString &toolTip) {
        QLabel *captionLabel = new QLabel(caption);
        captionLabel->setProperty("state", state);
        captionLabel->setToolTip(toolTip);
        valueLabel->setProperty("state", state);
        valueLabel->setToolTip(toolTip);
        statsLayout->addWidget(captionLabel);
        statsLayout->addWidget(valueLabel);
    };

    addCounter(tr("Recent:"), m_recentClientsLabel, "success",
               tr("Clients whose last backup or contact is younger than the limit in the settings"));
    statsLayout->addSpacing(20);
    addCounter(tr("Stale:"), m_staleClientsLabel, "warning",
               tr("Clients whose last backup or contact is older than the limit in the settings"));
    statsLayout->addSpacing(20);
    addCounter(tr("Never:"), m_neverClientsLabel, "muted",
               tr("Clients without any known backup or contact"));
    statsLayout->addSpacing(20);
    addCounter(tr("Unknown:"), m_unknownClientsLabel, "muted",
               tr("Clients whose last backup time cannot be read"));

    statsLayout->addStretch();

    mainLayout->addWidget(statsGroup);

    // Table view with custom header
    m_model->setStaleThresholdHours(BSettings::instance().behaviorClientStaleHours());
    m_tableView->setHorizontalHeader(m_headerView);
    m_tableView->setModel(m_model);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setSortingEnabled(true);
    m_tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_tableView->setMouseTracking(true);
    m_tableView->verticalHeader()->setVisible(false);

    // Configure headers (column visibility context menu, no checkbox)
    m_headerView->setCheckboxVisible(false);
    m_headerView->setStretchLastSection(false);
    m_headerView->setSectionsMovable(true);
    m_headerView->setDragEnabled(true);
    m_headerView->setDragDropMode(QAbstractItemView::InternalMove);

    // Column resize modes
    m_headerView->setSectionResizeMode(BClientsModel::COL_NAME, QHeaderView::Interactive);
    m_headerView->setSectionResizeMode(BClientsModel::COL_ADDRESS, QHeaderView::Stretch);
    m_headerView->setSectionResizeMode(BClientsModel::COL_PORT, QHeaderView::Fixed);
    m_headerView->setSectionResizeMode(BClientsModel::COL_STATUS, QHeaderView::Fixed);
    m_headerView->setSectionResizeMode(BClientsModel::COL_OS, QHeaderView::Interactive);
    m_headerView->setSectionResizeMode(BClientsModel::COL_VERSION, QHeaderView::Interactive);
    m_headerView->setSectionResizeMode(BClientsModel::COL_LAST_CONN, QHeaderView::Interactive);
    m_headerView->setSectionResizeMode(BClientsModel::COL_JOB_COUNT, QHeaderView::Fixed);
    m_headerView->setSectionResizeMode(BClientsModel::COL_TOTAL_BYTES, QHeaderView::Fixed);

    // Set column widths (~20% for Name, Address stretches)
    m_tableView->setColumnWidth(BClientsModel::COL_NAME, 180);
    m_tableView->setColumnWidth(BClientsModel::COL_PORT, 50);
    m_tableView->setColumnWidth(BClientsModel::COL_STATUS, 110);
    m_tableView->setColumnWidth(BClientsModel::COL_OS, 120);
    m_tableView->setColumnWidth(BClientsModel::COL_VERSION, 80);
    m_tableView->setColumnWidth(BClientsModel::COL_LAST_CONN, 160);
    m_tableView->setColumnWidth(BClientsModel::COL_JOB_COUNT, 50);
    m_tableView->setColumnWidth(BClientsModel::COL_TOTAL_BYTES, 80);

    // Auto-save column config on resize/move (debounced)
    m_autoSaveTimer->setSingleShot(true);
    m_autoSaveTimer->setInterval(500);
    connect(m_autoSaveTimer, &QTimer::timeout, this, [this]() {
        m_columnConfig->saveHeaderState(m_headerView, "ClientsWidget/Columns");
    });
    connect(m_headerView, &QHeaderView::sectionResized,
            this, [this]() { m_autoSaveTimer->start(); });
    connect(m_headerView, &QHeaderView::sectionMoved,
            this, [this]() { m_autoSaveTimer->start(); });
    connect(m_headerView, &BCheckableHeaderView::columnVisibilityChanged,
            this, [this]() { m_autoSaveTimer->start(); });

    // Restore saved column configuration
    m_columnConfig->restoreHeaderState(m_headerView, "ClientsWidget/Columns");

    mainLayout->addWidget(m_tableView);

    // Debounce timer for filters
    m_filterDebounceTimer->setSingleShot(true);
    m_filterDebounceTimer->setInterval(300);
}

void BClientsWidget::setupConnections()
{
    // Auto-refresh checkbox
    connect(m_autoRefreshCheck, &QCheckBox::toggled, this, [this](bool checked) {
        BSettings::instance().setBehaviorAutoRefresh(checked);

        // Disable refresh button when auto-refresh is active
        m_refreshButton->setEnabled(!checked);

        emit autoRefreshChanged(checked);
    });

    // Sync checkbox when settings change (e.g., from Settings dialog)
    connect(&BSettings::instance(), &BSettings::autoRefreshSettingsChanged,
            this, [this](bool enabled, int /*interval*/) {
        if (m_autoRefreshCheck->isChecked() != enabled) {
            m_autoRefreshCheck->blockSignals(true);
            m_autoRefreshCheck->setChecked(enabled);
            m_autoRefreshCheck->blockSignals(false);
            m_refreshButton->setEnabled(!enabled);
        }
    });

    // The limit for "stale" is a setting
    connect(&BSettings::instance(), &BSettings::behaviorSettingsChanged,
            this, [this]() {
        m_model->setStaleThresholdHours(BSettings::instance().behaviorClientStaleHours());
    });

    // Refresh button
    connect(m_refreshButton, &QPushButton::clicked,
            this, &BClientsWidget::onRefreshClicked);

    // Filter changes
    connect(m_clientFilter, &QComboBox::currentTextChanged,
            this, &BClientsWidget::onClientFilterChanged);
    connect(m_statusFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BClientsWidget::onStatusFilterChanged);

    // Debounced filter application
    connect(m_filterDebounceTimer, &QTimer::timeout,
            this, &BClientsWidget::applyFilters);

    // Double-click on client
    connect(m_tableView, &QTableView::doubleClicked,
            this, &BClientsWidget::onClientDoubleClicked);

    // Context menu
    connect(m_tableView, &QTableView::customContextMenuRequested,
            this, &BClientsWidget::onContextMenu);

    // Model changes
    connect(m_model, &BClientsModel::dataChanged,
            this, &BClientsWidget::updateStatistics);

    // Re-apply filters when model enrichment data changes
    connect(m_model, &BClientsModel::allClientsChanged,
            this, &BClientsWidget::applyFilters);

    // Forward model signals to widget signals
    connect(m_model, &BClientsModel::sendCommand,
            this, &BClientsWidget::sendCommand);
    connect(m_model, &BClientsModel::statusMessageChanged,
            this, &BClientsWidget::statusMessageChanged);

    // Selection changed - emit signal when client selection changes
    connect(m_tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, [this](const QItemSelection &, const QItemSelection &) {
                emit selectionChanged(hasSelection());
            });
}

void BClientsWidget::setConnectionState(bool connected)
{
    m_autoRefreshCheck->setEnabled(connected);
    bool autoRefreshActive = connected && m_autoRefreshCheck->isChecked();
    m_refreshButton->setEnabled(connected && !autoRefreshActive);

    // When connecting with auto-refresh already enabled, start the timer
    if (connected && autoRefreshActive) {
        emit autoRefreshChanged(true);
    }

    if (!connected) {
        clearData();
        refresh();
    }
}

void BClientsWidget::refresh()
{
    // Disable refresh button during update
    m_refreshButton->setEnabled(false);

    // Delegate to model (checks connection, sends command)
    m_model->refresh();
}

void BClientsWidget::onRefreshClicked()
{
    refresh();
}

void BClientsWidget::onClientFilterChanged(const QString &text)
{
    Q_UNUSED(text);
    // Restart debounce timer
    m_filterDebounceTimer->start();
}

void BClientsWidget::onStatusFilterChanged(int index)
{
    Q_UNUSED(index);
    // Apply filter immediately (no debounce for combo box selection)
    applyFilters();
}

void BClientsWidget::onClientDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    QJsonObject client = m_model->enrichedClient(index.row());
    if (!client.isEmpty()) {
        BClientDetailsDialog dialog(m_director, m_model, index.row(), this);
        dialog.exec();

        emit clientDoubleClicked(client);
    }
}

bool BClientsWidget::hasSelection() const
{
    return m_tableView->selectionModel()->hasSelection();
}

bool BClientsWidget::showSelectedClientDetails()
{
    QModelIndexList selected = m_tableView->selectionModel()->selectedRows();
    if (selected.isEmpty())
        return false;

    // Show details for the first selected client
    QModelIndex index = selected.first();
    if (!index.isValid())
        return false;

    QJsonObject client = m_model->enrichedClient(index.row());
    if (!client.isEmpty()) {
        BClientDetailsDialog dialog(m_director, m_model, index.row(), this);
        dialog.exec();
        return true;
    }
    return false;
}

void BClientsWidget::processJsonResponse(const QString &jsonData)
{
    // Re-enable refresh button
    m_refreshButton->setEnabled(true);

    // Parse JSON
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BClientsWidget: JSON parse error:" << error.errorString();
        emit statusMessageChanged(tr("Error parsing client data"));
        return;
    }

    QJsonArray clientsArray;

    // Extract clients array from various possible JSON structures
    if (doc.isArray()) {
        clientsArray = doc.array();
    } else if (doc.isObject()) {
        QJsonObject root = doc.object();
        if (root.contains("result") && root["result"].isObject()) {
            QJsonObject result = root["result"].toObject();
            if (result.contains("clients") && result["clients"].isArray()) {
                clientsArray = result["clients"].toArray();
            }
        } else if (root.contains("clients") && root["clients"].isArray()) {
            clientsArray = root["clients"].toArray();
        }
    }

    BLOG_DEBUG() << "BClientsWidget: Found" << clientsArray.size() << "clients";

    // Update client filter combo box first (before model signal triggers applyFilters)
    m_clientFilter->clear();
    m_clientFilter->addItem("");  // Empty option for "show all"

    QStringList clientNames;
    for (const QJsonValue &clientVal : clientsArray) {
        QJsonObject client = clientVal.toObject();
        QString name = client["name"].toString().trimmed();
        if (!name.isEmpty()) {
            clientNames.append(name);
        }
    }
    clientNames.sort(Qt::CaseInsensitive);
    m_clientFilter->addItems(clientNames);

    // Store in model (triggers cached enrichments + allClientsChanged → applyFilters)
    m_model->setAllClients(clientsArray);

    emit statusMessageChanged(tr("%1 client(s) loaded").arg(clientsArray.size()));
}

void BClientsWidget::applyFilters()
{
    QString clientFilter = m_clientFilter->currentText().trimmed();
    QString statusFilter = m_statusFilter->currentData().toString();

    QJsonArray filteredClients;

    for (const QJsonValue &clientVal : m_model->allClients()) {
        QJsonObject client = clientVal.toObject();
        QString clientName = client["name"].toString();

        // Apply client name filter
        if (!clientFilter.isEmpty()) {
            if (!clientName.contains(clientFilter, Qt::CaseInsensitive)) {
                continue;
            }
        }

        // Apply the filter on the age of the last backup
        if (statusFilter != "all") {
            const BClientStatus::Activity activity = m_model->clientActivity(client).activity;

            if (statusFilter == "recent" && activity != BClientStatus::Activity::Recent) {
                continue;
            }
            if (statusFilter == "stale" && activity != BClientStatus::Activity::Stale) {
                continue;
            }
            if (statusFilter == "never" && activity != BClientStatus::Activity::Never) {
                continue;
            }
            if (statusFilter == "unknown" && activity != BClientStatus::Activity::Unknown) {
                continue;
            }
        }

        filteredClients.append(clientVal);
    }

    BLOG_DEBUG() << "BClientsWidget: Filtered to" << filteredClients.size() << "clients";

    // Update model
    m_model->setClients(filteredClients);

    // Update statistics
    updateStatistics();
}

void BClientsWidget::updateStatistics()
{
    int totalClients = m_model->rowCount();
    int recentClients = 0;
    int staleClients = 0;
    int neverClients = 0;
    int unknownClients = 0;

    for (int row = 0; row < totalClients; ++row) {
        switch (m_model->clientActivity(m_model->clientAt(row)).activity) {
        case BClientStatus::Activity::Recent:
            recentClients++;
            break;
        case BClientStatus::Activity::Stale:
            staleClients++;
            break;
        case BClientStatus::Activity::Never:
            neverClients++;
            break;
        case BClientStatus::Activity::Unknown:
            unknownClients++;
            break;
        }
    }

    m_totalClientsLabel->setText(QString::number(totalClients));
    m_recentClientsLabel->setText(QString::number(recentClients));
    m_staleClientsLabel->setText(QString::number(staleClients));
    m_neverClientsLabel->setText(QString::number(neverClients));
    m_unknownClientsLabel->setText(QString::number(unknownClients));
}

void BClientsWidget::clearData()
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BClientsWidget: Clearing all data";
#endif

    // Clear model (also clears enrichment caches)
    m_model->clear();

    // Clear filter combo boxes
    m_clientFilter->clear();
    m_statusFilter->setCurrentIndex(0);  // Reset to "All"

    // Reset statistics
    m_totalClientsLabel->setText("0");
    m_recentClientsLabel->setText("0");
    m_staleClientsLabel->setText("0");
    m_neverClientsLabel->setText("0");
    m_unknownClientsLabel->setText("0");
}

void BClientsWidget::onContextMenu(const QPoint &pos)
{
    QModelIndex index = m_tableView->indexAt(pos);
    if (!index.isValid()) return;

    QMenu menu(this);
    QAction *detailsAction = menu.addAction(tr("Details / Export..."));
    connect(detailsAction, &QAction::triggered, this, [this, index]() {
        BClientDetailsDialog dialog(m_director, m_model, index.row(), this);
        dialog.showSettingsTab();
        dialog.exec();
    });
    menu.exec(m_tableView->viewport()->mapToGlobal(pos));
}

