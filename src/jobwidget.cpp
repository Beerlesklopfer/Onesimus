#include "jobwidget.h"
#include "ui_jobwidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>

JobWidget::JobWidget(Director *director, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JobWidget)
    , m_director(director)
{
    ui->setupUi(this);
    setupUI();
    
    connect(m_director, &Director::jobsReceived, this, &JobWidget::onJobsReceived);
}

JobWidget::~JobWidget()
{
    delete ui;
}

void JobWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Toolbar mit Buttons
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    m_runJobButton = new QPushButton("Job ausführen", this);
    m_cancelJobButton = new QPushButton("Job abbrechen", this);
    m_detailsButton = new QPushButton("Details anzeigen", this);
    m_refreshButton = new QPushButton("Aktualisieren", this);
    
    m_runJobButton->setIcon(QIcon::fromTheme("media-playback-start"));
    m_cancelJobButton->setIcon(QIcon::fromTheme("process-stop"));
    m_detailsButton->setIcon(QIcon::fromTheme("document-properties"));
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    
    toolbarLayout->addWidget(m_runJobButton);
    toolbarLayout->addWidget(m_cancelJobButton);
    toolbarLayout->addWidget(m_detailsButton);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(m_refreshButton);
    
    layout->addLayout(toolbarLayout);
    
    // Job-Tabelle
    m_jobTable = new QTableWidget(this);
    m_jobTable->setColumnCount(9);
    m_jobTable->setHorizontalHeaderLabels({
        "Job ID", "Name", "Typ", "Level", "Client", 
        "Status", "Startzeit", "Bytes", "Dateien"
    });
    
    m_jobTable->horizontalHeader()->setStretchLastSection(false);
    m_jobTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_jobTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_jobTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_jobTable->setAlternatingRowColors(true);
    m_jobTable->setSortingEnabled(true);
    
    layout->addWidget(m_jobTable);
    
    // Verbinde Buttons
    connect(m_runJobButton, &QPushButton::clicked, this, &JobWidget::onRunJobClicked);
    connect(m_cancelJobButton, &QPushButton::clicked, this, &JobWidget::onCancelJobClicked);
    connect(m_detailsButton, &QPushButton::clicked, this, &JobWidget::onShowDetailsClicked);
    connect(m_refreshButton, &QPushButton::clicked, this, &JobWidget::onRefreshClicked);
    connect(m_jobTable, &QTableWidget::itemSelectionChanged, this, &JobWidget::onJobSelectionChanged);
    
    // Initial deaktivierte Buttons
    m_cancelJobButton->setEnabled(false);
    m_detailsButton->setEnabled(false);
}

void JobWidget::onJobsReceived(const QList<Director::JobInfo> &jobs)
{
    updateJobTable(jobs);
}

void JobWidget::updateJobTable(const QList<Director::JobInfo> &jobs)
{
    m_jobTable->setSortingEnabled(false);
    m_jobTable->setRowCount(jobs.size());
    
    for (int i = 0; i < jobs.size(); ++i) {
        const Director::JobInfo &job = jobs[i];
        
        m_jobTable->setItem(i, 0, new QTableWidgetItem(QString::number(job.jobId)));
        m_jobTable->setItem(i, 1, new QTableWidgetItem(job.name));
        m_jobTable->setItem(i, 2, new QTableWidgetItem(job.type));
        m_jobTable->setItem(i, 3, new QTableWidgetItem(job.level));
        m_jobTable->setItem(i, 4, new QTableWidgetItem(job.clientName));
        
        QTableWidgetItem *statusItem = new QTableWidgetItem(formatJobStatus(job.status));
        if (job.status == "R") {
            statusItem->setBackground(QBrush(QColor(173, 216, 230))); // Hellblau für laufende Jobs
        } else if (job.status == "T" || job.status == "f") {
            statusItem->setBackground(QBrush(QColor(144, 238, 144))); // Hellgrün für erfolgreiche Jobs
        } else if (job.status == "E" || job.status == "e") {
            statusItem->setBackground(QBrush(QColor(255, 182, 193))); // Hellrot für Fehler
        }
        m_jobTable->setItem(i, 5, statusItem);
        
        m_jobTable->setItem(i, 6, new QTableWidgetItem(
            job.startTime.isValid() ? job.startTime.toString("dd.MM.yyyy HH:mm") : "-"
        ));
        m_jobTable->setItem(i, 7, new QTableWidgetItem(formatBytes(job.jobBytes)));
        m_jobTable->setItem(i, 8, new QTableWidgetItem(QString::number(job.jobFiles)));
    }
    
    m_jobTable->setSortingEnabled(true);
    m_jobTable->sortByColumn(0, Qt::DescendingOrder);
}

void JobWidget::onRunJobClicked()
{
    bool ok;
    QString jobName = QInputDialog::getText(this, "Job ausführen",
        "Job-Name:", QLineEdit::Normal, "", &ok);
    
    if (ok && !jobName.isEmpty()) {
        m_director->sendCommand(Director::DirectorCommand::Run, jobName);

        QMessageBox::information(this, "Job gestartet", 
            QString("Job '%1' wurde gestartet.").arg(jobName));
    }
}

void JobWidget::onCancelJobClicked()
{
    QList<QTableWidgetItem*> selectedItems = m_jobTable->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }
    
    int row = selectedItems.first()->row();
    quint64 jobId = m_jobTable->item(row, 0)->text().toInt();
    QString jobName = m_jobTable->item(row, 1)->text();
    
    int ret = QMessageBox::question(this, "Job abbrechen",
        QString("Möchten Sie Job '%1' (ID: %2) wirklich abbrechen?")
            .arg(jobName).arg(jobId),
        QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        m_director->sendCommand(Director::DirectorCommand::Cancel, jobId);
        QMessageBox::information(this, "Job abgebrochen", 
            QString("Job %1 wurde abgebrochen.").arg(jobId));
    }
}

void JobWidget::onShowDetailsClicked()
{
    QList<QTableWidgetItem*> selectedItems = m_jobTable->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }
    
    quint64 row = selectedItems.first()->row();
    int jobId = m_jobTable->item(row, 0)->text().toInt();
    //// m_director->sendCommand(Director::DirectorCommand::ListJobDetails, 100);
}

void JobWidget::onRefreshClicked()
{
    m_director->sendCommand(Director::DirectorCommand::ListJobs, "100");
}

void JobWidget::onJobSelectionChanged()
{
    bool hasSelection = !m_jobTable->selectedItems().isEmpty();
    m_cancelJobButton->setEnabled(hasSelection);
    m_detailsButton->setEnabled(hasSelection);
}

QString JobWidget::formatBytes(qint64 bytes)
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

QString JobWidget::formatJobStatus(const QString &status)
{
    if (status == "C") return "Erstellt";
    if (status == "R") return "Läuft";
    if (status == "B") return "Blockiert";
    if (status == "T") return "Beendet";
    if (status == "W") return "Wartet";
    if (status == "f") return "Erfolgreich";
    if (status == "E") return "Fehler";
    if (status == "e") return "Kritischer Fehler";
    if (status == "A") return "Abgebrochen";
    return status;
}
