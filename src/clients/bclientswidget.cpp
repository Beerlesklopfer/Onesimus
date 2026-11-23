// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "clients/bclientswidget.h"
#include <QLineEdit>
#include "buiscale.h"
#include "clients/bclientsmodel.h"
#include "clients/bclientdetailsdialog.h"
#include "bcheckableheaderview.h"
#include "bheaderstate.h"
#include "bcapabilitynotice.h"
#include "bstatusdelegate.h"
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
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QMenu>
#include <QTimer>
#include <QVBoxLayout>

BClientsWidget::BClientsWidget(BDirector *director, QWidget *parent)
    : QWidget(parent)
    , m_tableView(new QTableView(this))
    , m_model(new BClientsModel(director, this))
    , m_sortModel(new QSortFilterProxyModel(this))
    , m_headerView(new BCheckableHeaderView(Qt::Horizontal, this))
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
    BUiScale::track(m_clientFilter, [this]() { m_clientFilter->setMinimumWidth(BUiScale::px(200)); });
    topLayout->addWidget(m_clientFilter);

    topLayout->addSpacing(20);

    // Filter by the age of the last backup (not a reachability status)
    topLayout->addWidget(new QLabel(tr("Last backup:")));
    m_statusFilter->addItem(tr("All"), "all");
    m_statusFilter->addItem(tr("Recent"), "recent");
    m_statusFilter->addItem(tr("Stale"), "stale");
    m_statusFilter->addItem(tr("Never"), "never");
    m_statusFilter->addItem(tr("Unknown"), "unknown");
    BUiScale::track(m_statusFilter, [this]() { m_statusFilter->setMinimumWidth(BUiScale::px(120)); });
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

    addCounter(tr("Recent:"), m_recentClientsLabel, "active",
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
    // The view shows a sorting proxy: numbers, ages and times sort by value
    // (the model's sort role), names without regard to case
    m_model->setStaleThresholdHours(BSettings::instance().behaviorClientStaleHours());
    m_sortModel->setSourceModel(m_model);
    m_sortModel->setSortRole(BClientsModel::SortRole);
    m_sortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_tableView->setHorizontalHeader(m_headerView);
    m_tableView->setModel(m_sortModel);
    // The last backup's age as a chip: sign, age, colour of the state
    m_tableView->setItemDelegateForColumn(BClientsModel::COL_STATUS, new BStatusDelegate(m_tableView));
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setSortingEnabled(true);
    m_tableView->sortByColumn(BClientsModel::COL_NAME, Qt::AscendingOrder);
    m_tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_tableView->setMouseTracking(true);
    m_tableView->verticalHeader()->setVisible(false);

    // Configure headers (column visibility context menu, no checkbox)
    m_headerView->setCheckboxVisible(false);
    m_headerView->setSectionsMovable(true);
    m_headerView->setDragEnabled(true);
    m_headerView->setDragDropMode(QAbstractItemView::InternalMove);

    // Every column can be resized by the user; the last visible column takes
    // the remaining width, so no gap is left on the right
    m_headerView->setSectionResizeMode(QHeaderView::Interactive);
    m_headerView->setStretchLastSection(true);
    m_headerView->setMinimumSectionSize(BUiScale::px(40));

    // Default column widths (used until the user changes them)
    m_tableView->setColumnWidth(BClientsModel::COL_ADDRESS, BUiScale::px(200));
    m_tableView->setColumnWidth(BClientsModel::COL_NAME, BUiScale::px(180));
    m_tableView->setColumnWidth(BClientsModel::COL_PORT, BUiScale::px(50));
    m_tableView->setColumnWidth(BClientsModel::COL_STATUS, BUiScale::px(110));
    m_tableView->setColumnWidth(BClientsModel::COL_OS, BUiScale::px(120));
    m_tableView->setColumnWidth(BClientsModel::COL_VERSION, BUiScale::px(80));
    m_tableView->setColumnWidth(BClientsModel::COL_LAST_CONN, BUiScale::px(160));
    m_tableView->setColumnWidth(BClientsModel::COL_JOB_COUNT, BUiScale::px(50));
    m_tableView->setColumnWidth(BClientsModel::COL_TOTAL_BYTES, BUiScale::px(80));

    // Auto-save column config on resize/move (debounced)
    m_autoSaveTimer->setSingleShot(true);
    m_autoSaveTimer->setInterval(500);
    connect(m_autoSaveTimer, &QTimer::timeout, this, [this]() {
        // Order, widths, hidden columns and the sort column, in BSettings
        BSettings::instance().setClientsHeaderState(m_headerView->saveState());
        BSettings::instance().setClientsHeaderScale(BHeaderState::currentScale());
    });
    connect(m_headerView, &QHeaderView::sectionResized,
            this, [this]() { m_autoSaveTimer->start(); });
    connect(m_headerView, &QHeaderView::sectionMoved,
            this, [this]() { m_autoSaveTimer->start(); });
    connect(m_headerView, &BCheckableHeaderView::columnVisibilityChanged,
            this, [this]() { m_autoSaveTimer->start(); });
    connect(m_headerView, &QHeaderView::sortIndicatorChanged,
            this, [this]() { m_autoSaveTimer->start(); });

    // Restore the saved header. Widths saved at another interface zoom are
    // scaled to the current one. Older versions kept order, widths and
    // visibility through BColumnConfiguration; that is taken over once if
    // nothing is in BSettings yet, and removed.
    const BHeaderState::Source restored = BHeaderState::restoreOrMigrate(
        m_headerView, BSettings::instance().clientsHeaderState(),
        BSettings::instance().clientsHeaderScale(), QStringLiteral("ClientsWidget/Columns"));
    if (restored == BHeaderState::Source::Saved) {
        m_tableView->sortByColumn(m_headerView->sortIndicatorSection(),
                                  m_headerView->sortIndicatorOrder());
    } else if (restored == BHeaderState::Source::Legacy) {
        m_autoSaveTimer->start();
    }

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

int BClientsWidget::sourceRow(const QModelIndex &viewIndex) const
{
    const QModelIndex sourceIndex = m_sortModel->mapToSource(viewIndex);
    return sourceIndex.isValid() ? sourceIndex.row() : -1;
}

void BClientsWidget::onClientDoubleClicked(const QModelIndex &index)
{
    const int row = sourceRow(index);
    if (row < 0)
        return;

    QJsonObject client = m_model->enrichedClient(row);
    if (!client.isEmpty()) {
        BClientDetailsDialog dialog(m_director, m_model, row, this);
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
    const int row = sourceRow(selected.first());
    if (row < 0)
        return false;

    QJsonObject client = m_model->enrichedClient(row);
    if (!client.isEmpty()) {
        BClientDetailsDialog dialog(m_director, m_model, row, this);
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

    emit statusMessageChanged(tr("%n client(s) loaded", nullptr, int(clientsArray.size())));
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
    const int row = sourceRow(m_tableView->indexAt(pos));
    if (row < 0) return;

    QMenu menu(this);
    QAction *detailsAction = menu.addAction(tr("Details / Export..."));
    connect(detailsAction, &QAction::triggered, this, [this, row]() {
        BClientDetailsDialog dialog(m_director, m_model, row, this);
        dialog.showSettingsTab();
        dialog.exec();
    });

    // "Check now" (issue #9): one probe of this client, on request only
    const QString name = m_model->clientAt(row).value("name").toString();
    const QString probing = m_model->probingClient();
    QAction *checkAction = menu.addAction(probing.isEmpty()
                                              ? tr("Check Now")
                                              : tr("Check Now (checking %1...)").arg(probing));
    checkAction->setToolTip(tr("Ask the Director whether it can reach this client now"));
    checkAction->setEnabled(!name.isEmpty() && probing.isEmpty() && m_director
                            && m_director->isConnected());
    connect(checkAction, &QAction::triggered, this, [this, name]() {
        checkClientNow(name);
    });
    menu.exec(m_tableView->viewport()->mapToGlobal(pos));
}

void BClientsWidget::focusFilter()
{
    m_clientFilter->setFocus(Qt::ShortcutFocusReason);
    if (m_clientFilter->lineEdit())
        m_clientFilter->lineEdit()->selectAll();
}

bool BClientsWidget::checkClientNow(const QString &clientName)
{
    // On a Director or connection that cannot carry the probe, say so and send nothing
    if (!BCapabilityNotice::check(this, m_director, BCapabilities::Function::ProbeClient,
                                  BCapabilities::Request::Explicit))
        return false;
    return m_model->checkClientNow(clientName);
}
