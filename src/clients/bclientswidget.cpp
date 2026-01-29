#include "clients/bclientswidget.h"
#include "clients/bclientsmodel.h"
#include "clients/bclientdetailsdialog.h"
#include <QComboBox>
#include <QDebug>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QTimer>
#include <QVBoxLayout>

BClientsWidget::BClientsWidget(QWidget *parent)
    : QWidget(parent)
    , m_tableView(new QTableView(this))
    , m_model(new BClientsModel(this))
    , m_clientFilter(new QComboBox(this))
    , m_statusFilter(new QComboBox(this))
    , m_refreshButton(new QPushButton(tr("Aktualisieren"), this))
    , m_totalClientsLabel(new QLabel("0", this))
    , m_onlineClientsLabel(new QLabel("0", this))
    , m_offlineClientsLabel(new QLabel("0", this))
    , m_director(nullptr)
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
    m_clientFilter->setPlaceholderText(tr("Nach Client filtern..."));
    m_clientFilter->setMinimumWidth(200);
    topLayout->addWidget(m_clientFilter);

    topLayout->addSpacing(20);

    // Status filter
    topLayout->addWidget(new QLabel(tr("Status:")));
    m_statusFilter->addItem(tr("Alle"), "all");
    m_statusFilter->addItem(tr("Online"), "online");
    m_statusFilter->addItem(tr("Offline"), "offline");
    m_statusFilter->setMinimumWidth(120);
    topLayout->addWidget(m_statusFilter);

    topLayout->addStretch();

    // Refresh button
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshButton->setEnabled(false);
    topLayout->addWidget(m_refreshButton);

    mainLayout->addLayout(topLayout);

    // Statistics bar
    QGroupBox *statsGroup = new QGroupBox(tr("Statistiken"));
    QHBoxLayout *statsLayout = new QHBoxLayout(statsGroup);

    QLabel *totalLabel = new QLabel(tr("Gesamt:"));
    totalLabel->setStyleSheet("font-weight: bold;");
    statsLayout->addWidget(totalLabel);
    statsLayout->addWidget(m_totalClientsLabel);

    statsLayout->addSpacing(20);

    QLabel *onlineLabel = new QLabel(tr("Online:"));
    onlineLabel->setStyleSheet("font-weight: bold; color: green;");
    statsLayout->addWidget(onlineLabel);
    m_onlineClientsLabel->setStyleSheet("color: green;");
    statsLayout->addWidget(m_onlineClientsLabel);

    statsLayout->addSpacing(20);

    QLabel *offlineLabel = new QLabel(tr("Offline:"));
    offlineLabel->setStyleSheet("font-weight: bold; color: gray;");
    statsLayout->addWidget(offlineLabel);
    m_offlineClientsLabel->setStyleSheet("color: gray;");
    statsLayout->addWidget(m_offlineClientsLabel);

    statsLayout->addStretch();

    mainLayout->addWidget(statsGroup);

    // Table view
    m_tableView->setModel(m_model);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setSortingEnabled(true);
    m_tableView->verticalHeader()->setVisible(false);

    // Configure column resize modes
    QHeaderView *header = m_tableView->horizontalHeader();
    header->setSectionResizeMode(BClientsModel::COL_NAME, QHeaderView::Stretch);
    header->setSectionResizeMode(BClientsModel::COL_ADDRESS, QHeaderView::Interactive);
    header->setSectionResizeMode(BClientsModel::COL_STATUS, QHeaderView::Fixed);
    header->setSectionResizeMode(BClientsModel::COL_OS, QHeaderView::Interactive);
    header->setSectionResizeMode(BClientsModel::COL_VERSION, QHeaderView::Interactive);
    header->setSectionResizeMode(BClientsModel::COL_LAST_CONN, QHeaderView::Interactive);
    header->setSectionResizeMode(BClientsModel::COL_JOB_COUNT, QHeaderView::Fixed);
    header->setSectionResizeMode(BClientsModel::COL_TOTAL_BYTES, QHeaderView::Fixed);

    // Set column widths
    m_tableView->setColumnWidth(BClientsModel::COL_STATUS, 100);
    m_tableView->setColumnWidth(BClientsModel::COL_ADDRESS, 150);
    m_tableView->setColumnWidth(BClientsModel::COL_OS, 150);
    m_tableView->setColumnWidth(BClientsModel::COL_VERSION, 100);
    m_tableView->setColumnWidth(BClientsModel::COL_LAST_CONN, 150);
    m_tableView->setColumnWidth(BClientsModel::COL_JOB_COUNT, 80);
    m_tableView->setColumnWidth(BClientsModel::COL_TOTAL_BYTES, 100);

    mainLayout->addWidget(m_tableView);

    // Debounce timer for filters
    m_filterDebounceTimer->setSingleShot(true);
    m_filterDebounceTimer->setInterval(300);
}

void BClientsWidget::setupConnections()
{
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

    // Model changes
    connect(m_model, &BClientsModel::dataChanged,
            this, &BClientsWidget::updateStatistics);
}

void BClientsWidget::setDirector(BDirector *director)
{
    m_director = director;

    if (m_director) {
        m_refreshButton->setEnabled(true);
    } else {
        m_refreshButton->setEnabled(false);
    }
}

void BClientsWidget::refresh()
{
    if (!m_director) {
        qWarning() << "BClientsWidget: No Director connection available";
        emit statusMessageChanged(tr("Keine Verbindung zum Director"));
        return;
    }

    qDebug() << "BClientsWidget: Requesting client list";

    // Disable refresh button during update
    m_refreshButton->setEnabled(false);

    emit statusMessageChanged(tr("Lade Clients..."));

    // Request client list from Director via signal
    emit sendCommand(BDirector::Command::ListClients, "");
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

    QJsonObject client = m_model->clientAt(index.row());
    if (!client.isEmpty()) {
        // Show client details dialog
        BClientDetailsDialog dialog(client, m_director, this);
        dialog.exec();

        emit clientDoubleClicked(client);
    }
}

void BClientsWidget::onClientsDataReceived(const QString &command, const QString &jsonData)
{
    // This method is no longer used - MainWindow routes JSON responses directly
    Q_UNUSED(command);
    Q_UNUSED(jsonData);
}

void BClientsWidget::processJsonResponse(const QString &jsonData)
{
    // Re-enable refresh button
    m_refreshButton->setEnabled(m_director != nullptr);

    // Parse JSON
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "BClientsWidget: JSON parse error:" << error.errorString();
        emit statusMessageChanged(tr("Fehler beim Parsen der Client-Daten"));
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

    qDebug() << "BClientsWidget: Found" << clientsArray.size() << "clients";

    // Store all clients for filtering
    m_allClients = clientsArray;

    // Update client filter combo box
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

    // Apply current filters
    applyFilters();

    emit statusMessageChanged(tr("%1 Client(s) geladen").arg(clientsArray.size()));
}

void BClientsWidget::applyFilters()
{
    QString clientFilter = m_clientFilter->currentText().trimmed();
    QString statusFilter = m_statusFilter->currentData().toString();

    QJsonArray filteredClients;

    for (const QJsonValue &clientVal : m_allClients) {
        QJsonObject client = clientVal.toObject();
        QString clientName = client["name"].toString();

        // Apply client name filter
        if (!clientFilter.isEmpty()) {
            if (!clientName.contains(clientFilter, Qt::CaseInsensitive)) {
                continue;
            }
        }

        // Apply status filter
        if (statusFilter != "all") {
            BClientsModel::Status status = m_model->getClientStatus(client);

            if (statusFilter == "online" && status != BClientsModel::STATUS_ONLINE) {
                continue;
            }
            if (statusFilter == "offline" && status != BClientsModel::STATUS_OFFLINE) {
                continue;
            }
        }

        filteredClients.append(clientVal);
    }

    qDebug() << "BClientsWidget: Filtered to" << filteredClients.size() << "clients";

    // Update model
    m_model->setClients(filteredClients);

    // Update statistics
    updateStatistics();
}

void BClientsWidget::updateStatistics()
{
    int totalClients = m_model->rowCount();
    int onlineClients = 0;
    int offlineClients = 0;

    for (int row = 0; row < totalClients; ++row) {
        QJsonObject client = m_model->clientAt(row);
        BClientsModel::Status status = m_model->getClientStatus(client);

        if (status == BClientsModel::STATUS_ONLINE) {
            onlineClients++;
        } else {
            offlineClients++;
        }
    }

    m_totalClientsLabel->setText(QString::number(totalClients));
    m_onlineClientsLabel->setText(QString::number(onlineClients));
    m_offlineClientsLabel->setText(QString::number(offlineClients));
}

void BClientsWidget::clearData()
{
#ifdef IS_DEVELOPER
    qDebug() << "BClientsWidget: Clearing all data";
#endif

    // Clear model
    m_model->clear();

    // Clear filter combo boxes
    m_clientFilter->clear();
    m_statusFilter->setCurrentIndex(0);  // Reset to "Alle"

    // Reset statistics
    m_totalClientsLabel->setText("0");
    m_onlineClientsLabel->setText("0");
    m_offlineClientsLabel->setText("0");
}
