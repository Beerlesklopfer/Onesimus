#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QHeaderView>
#include <QVBoxLayout>

ClientWidget::ClientWidget(BaculaDirector *director, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientWidget)
    , m_director(director)
{
    ui->setupUi(this);
    setupUI();
    
    connect(m_director, &BaculaDirector::clientsReceived, this, &ClientWidget::onClientsReceived);
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    m_statusButton = new QPushButton("Status anzeigen", this);
    m_refreshButton = new QPushButton("Aktualisieren", this);
    
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
    connect(m_statusButton, &QPushButton::clicked, this, &ClientWidget::onStatusClicked);
    connect(m_refreshButton, &QPushButton::clicked, this, &ClientWidget::onRefreshClicked);
    connect(m_clientTable, &QTableWidget::itemSelectionChanged, this, &ClientWidget::onClientSelectionChanged);
}

void ClientWidget::onClientsReceived(const QList<BaculaDirector::ClientInfo> &clients)
{
    updateClientTable(clients);
}

void ClientWidget::updateClientTable(const QList<BaculaDirector::ClientInfo> &clients)
{
    m_clientTable->setSortingEnabled(false);
    m_clientTable->setRowCount(clients.size());
    
    for (int i = 0; i < clients.size(); ++i) {
        const BaculaDirector::ClientInfo &client = clients[i];
        
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

void ClientWidget::onStatusClicked()
{
    QList<QTableWidgetItem*> selectedItems = m_clientTable->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }
    
    int row = selectedItems.first()->row();
    QString clientName = m_clientTable->item(row, 0)->text();
    
    m_director->statusClient(clientName);
}

void ClientWidget::onRefreshClicked()
{
    if (m_director->connectionType() == BaculaDirector::RestAPI) {
        m_director->restGetClients();
    } else {
        m_director->listClients();
    }
}

void ClientWidget::onClientSelectionChanged()
{
    bool hasSelection = !m_clientTable->selectedItems().isEmpty();
    m_statusButton->setEnabled(hasSelection);
}
