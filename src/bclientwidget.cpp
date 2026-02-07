#include "bclientwidget.h"
#include "ui_bclientwidget.h"
#include <QHeaderView>
#include <QVBoxLayout>

BClientWidget::BClientWidget(BDirector *director, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BClientWidget)
    , m_director(director)
{
    ui->setupUi(this);
    setupUI();

    // connect(m_director, &BDirector::clientsReceived, this, &BClientWidget::onClientsReceived);
}

BClientWidget::~BClientWidget()
{
    delete ui;
}

void BClientWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();

    m_statusButton = new QPushButton(tr("Show state"), this);
    m_refreshButton = new QPushButton(tr("Refresh"), this);

    m_statusButton->setIcon(QIcon::fromTheme("dialog-information"));
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    m_statusButton->setEnabled(false);

    toolbarLayout->addWidget(m_statusButton);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(m_refreshButton);

    layout->addLayout(toolbarLayout);

    // Client-Tabelle
    m_clientTable = new QTableWidget(this);
    m_clientTable->setColumnCount(5);
    m_clientTable->setHorizontalHeaderLabels({
        "Name", "Adresse", "Port", "OS", "Auto Prune"
    });

    m_clientTable->horizontalHeader()->setStretchLastSection(true);
    m_clientTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_clientTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_clientTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_clientTable->setAlternatingRowColors(true);
    m_clientTable->setSortingEnabled(true);

    layout->addWidget(m_clientTable);

    // Verbinde Buttons
    connect(m_statusButton, &QPushButton::clicked, this, &BClientWidget::onStatusClicked);
    connect(m_refreshButton, &QPushButton::clicked, this, &BClientWidget::onRefreshClicked);
    connect(m_clientTable, &QTableWidget::itemSelectionChanged, this, &BClientWidget::onClientSelectionChanged);
}

void BClientWidget::onClientsReceived(const QList<BDirector::ClientInfo> &clients)
{
    updateClientTable(clients);
}

void BClientWidget::updateClientTable(const QList<BDirector::ClientInfo> &clients)
{
    m_clientTable->setSortingEnabled(false);
    m_clientTable->setRowCount(clients.size());

    for (int i = 0; i < clients.size(); ++i) {
        const BDirector::ClientInfo &client = clients[i];

        m_clientTable->setItem(i, 0, new QTableWidgetItem(client.name));
        m_clientTable->setItem(i, 1, new QTableWidgetItem(client.address));
        m_clientTable->setItem(i, 2, new QTableWidgetItem(QString::number(client.port)));
        m_clientTable->setItem(i, 3, new QTableWidgetItem(client.os));
        m_clientTable->setItem(i, 4, new QTableWidgetItem(
            client.autoprune ? "Ja" : "Nein"
        ));
    }

    m_clientTable->setSortingEnabled(true);
}

void BClientWidget::onStatusClicked()
{
    QList<QTableWidgetItem*> selectedItems = m_clientTable->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }

    int row = selectedItems.first()->row();
    QString clientName = m_clientTable->item(row, 0)->text();

    emit sendCommand(BDirector::Command::StatusClient, "");
}

void BClientWidget::onRefreshClicked()
{
    emit sendCommand(BDirector::Command::ListClients, "100");
}

void BClientWidget::onClientSelectionChanged()
{
    bool hasSelection = !m_clientTable->selectedItems().isEmpty();
    m_statusButton->setEnabled(hasSelection);
}
