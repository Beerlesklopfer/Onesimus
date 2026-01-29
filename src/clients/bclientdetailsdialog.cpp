#include "clients/bclientdetailsdialog.h"
#include <QDateTime>
#include <QDebug>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLabel>
#include <QLocale>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>
#include <QVBoxLayout>

BClientDetailsDialog::BClientDetailsDialog(const QJsonObject &client, BDirector *director, QWidget *parent)
    : QDialog(parent)
    , m_client(client)
    , m_director(director)
    , m_clientName(client["name"].toString())
{
    setWindowTitle(tr("Client Details - %1").arg(m_clientName));
    resize(800, 600);

    setupUI();
    loadClientJobs();
}

void BClientDetailsDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_tabWidget = new QTabWidget(this);
    setupInfoTab();
    setupJobsTab();
    setupStatisticsTab();

    mainLayout->addWidget(m_tabWidget);

    // Button box
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    QPushButton *closeButton = new QPushButton(tr("Schließen"), this);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);
}

void BClientDetailsDialog::setupInfoTab()
{
    QWidget *infoTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(infoTab);

    // General Information Group
    QGroupBox *generalGroup = new QGroupBox(tr("Allgemeine Informationen"));
    QFormLayout *generalForm = new QFormLayout(generalGroup);

    m_nameLabel = new QLabel(m_client["name"].toString());
    m_addressLabel = new QLabel(m_client["address"].toString());
    m_portLabel = new QLabel(QString::number(m_client["port"].toInt()));
    m_versionLabel = new QLabel(m_client["version"].toString());
    m_osLabel = new QLabel(m_client["uname"].toString());

    generalForm->addRow(tr("Name:"), m_nameLabel);
    generalForm->addRow(tr("Adresse:"), m_addressLabel);
    generalForm->addRow(tr("Port:"), m_portLabel);
    generalForm->addRow(tr("Version:"), m_versionLabel);
    generalForm->addRow(tr("Betriebssystem:"), m_osLabel);

    layout->addWidget(generalGroup);

    // Connection Information Group
    QGroupBox *connGroup = new QGroupBox(tr("Verbindungsinformationen"));
    QFormLayout *connForm = new QFormLayout(connGroup);

    // Status
    QString statusText;
    QString lastConn = m_client["lastconnection"].toString();
    if (lastConn.isEmpty() || lastConn == "0") {
        statusText = tr("Offline (Nie verbunden)");
        m_lastConnLabel = new QLabel(tr("Nie"));
    } else {
        QDateTime lastConnTime = QDateTime::fromString(lastConn, Qt::ISODate);
        if (!lastConnTime.isValid())
            lastConnTime = QDateTime::fromString(lastConn, "yyyy-MM-dd HH:mm:ss");

        if (lastConnTime.isValid()) {
            qint64 secsSinceLastConn = lastConnTime.secsTo(QDateTime::currentDateTime());
            if (secsSinceLastConn > 86400) {
                statusText = tr("Offline (seit %1 Tagen)").arg(secsSinceLastConn / 86400);
            } else {
                statusText = tr("Online");
            }
            m_lastConnLabel = new QLabel(QLocale().toString(lastConnTime, QLocale::ShortFormat));
        } else {
            statusText = tr("Unbekannt");
            m_lastConnLabel = new QLabel(lastConn);
        }
    }

    m_statusLabel = new QLabel(statusText);
    if (statusText.contains("Online")) {
        m_statusLabel->setStyleSheet("color: green; font-weight: bold;");
    } else {
        m_statusLabel->setStyleSheet("color: gray;");
    }

    connForm->addRow(tr("Status:"), m_statusLabel);
    connForm->addRow(tr("Letzte Verbindung:"), m_lastConnLabel);

    layout->addWidget(connGroup);

    // Retention Information Group
    QGroupBox *retentionGroup = new QGroupBox(tr("Aufbewahrungsrichtlinien"));
    QFormLayout *retentionForm = new QFormLayout(retentionGroup);

    m_autoprune = new QLabel(m_client["autoprune"].toBool() ? tr("Ja") : tr("Nein"));
    m_fileRetention = new QLabel(m_client["fileretention"].toString());
    m_jobRetention = new QLabel(m_client["jobretention"].toString());

    retentionForm->addRow(tr("Auto-Bereinigung:"), m_autoprune);
    retentionForm->addRow(tr("Datei-Aufbewahrung:"), m_fileRetention);
    retentionForm->addRow(tr("Job-Aufbewahrung:"), m_jobRetention);

    layout->addWidget(retentionGroup);

    layout->addStretch();

    m_tabWidget->addTab(infoTab, tr("Informationen"));
}

void BClientDetailsDialog::setupJobsTab()
{
    QWidget *jobsTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(jobsTab);

    // Toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    QLabel *infoLabel = new QLabel(tr("Letzte Jobs für diesen Client:"));
    toolbarLayout->addWidget(infoLabel);
    toolbarLayout->addStretch();

    m_refreshJobsButton = new QPushButton(tr("Aktualisieren"));
    connect(m_refreshJobsButton, &QPushButton::clicked, this, &BClientDetailsDialog::loadClientJobs);
    toolbarLayout->addWidget(m_refreshJobsButton);

    layout->addLayout(toolbarLayout);

    // Jobs table
    m_jobsTable = new QTableWidget();
    m_jobsTable->setColumnCount(7);
    m_jobsTable->setHorizontalHeaderLabels({
        tr("JobID"), tr("Name"), tr("Start"), tr("Ende"),
        tr("Level"), tr("Files"), tr("Status")
    });
    m_jobsTable->horizontalHeader()->setStretchLastSection(false);
    m_jobsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_jobsTable->setAlternatingRowColors(true);
    m_jobsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_jobsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_jobsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(m_jobsTable);

    m_tabWidget->addTab(jobsTab, tr("Jobs"));
}

void BClientDetailsDialog::setupStatisticsTab()
{
    QWidget *statsTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(statsTab);

    QGroupBox *statsGroup = new QGroupBox(tr("Statistiken"));
    QFormLayout *statsForm = new QFormLayout(statsGroup);

    m_totalJobsLabel = new QLabel(QString::number(m_client["jobcount"].toInt()));
    m_successfulJobsLabel = new QLabel("0");  // Will be calculated from jobs
    m_failedJobsLabel = new QLabel("0");      // Will be calculated from jobs
    m_totalBytesLabel = new QLabel(QString::number(m_client["totalbytes"].toString().toLongLong()));
    m_totalFilesLabel = new QLabel("0");      // Will be calculated from jobs

    statsForm->addRow(tr("Gesamtzahl Jobs:"), m_totalJobsLabel);
    statsForm->addRow(tr("Erfolgreiche Jobs:"), m_successfulJobsLabel);
    statsForm->addRow(tr("Fehlgeschlagene Jobs:"), m_failedJobsLabel);
    statsForm->addRow(tr("Gesamt-Bytes:"), m_totalBytesLabel);
    statsForm->addRow(tr("Gesamt-Dateien:"), m_totalFilesLabel);

    layout->addWidget(statsGroup);
    layout->addStretch();

    m_tabWidget->addTab(statsTab, tr("Statistiken"));
}

void BClientDetailsDialog::loadClientJobs()
{
    if (!m_director) {
        qDebug() << "BClientDetailsDialog: No director connection";
        return;
    }

    m_refreshJobsButton->setEnabled(false);
    m_jobsTable->setRowCount(0);

    // Connect to receive response
    connect(m_director, &BDirector::jsonResponse,
            this, &BClientDetailsDialog::onJobsReceived,
            Qt::UniqueConnection);

    // Request jobs for this client - Thread-safe via queued connection
    QString args = QString("client=\"%1\"").arg(m_clientName);
    QMetaObject::invokeMethod(m_director, "doSendCommand",
                              Qt::QueuedConnection,
                              Q_ARG(BDirector::Command, BDirector::Command::ListJobs),
                              Q_ARG(QString, args));
}

void BClientDetailsDialog::onJobsReceived(const QString &command, const QString &response)
{
    if (!command.contains("list jobs")) {
        return;
    }

    m_refreshJobsButton->setEnabled(true);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "BClientDetailsDialog: JSON parse error:" << error.errorString();
        return;
    }

    QJsonArray jobsArray;

    // Extract jobs array from JSON
    if (doc.isArray()) {
        jobsArray = doc.array();
    } else if (doc.isObject()) {
        QJsonObject root = doc.object();
        if (root.contains("result") && root["result"].isObject()) {
            QJsonObject result = root["result"].toObject();
            if (result.contains("jobs") && result["jobs"].isArray()) {
                jobsArray = result["jobs"].toArray();
            }
        } else if (root.contains("jobs") && root["jobs"].isArray()) {
            jobsArray = root["jobs"].toArray();
        }
    }

    qDebug() << "BClientDetailsDialog: Found" << jobsArray.size() << "jobs";

    m_jobsTable->setRowCount(jobsArray.size());

    int successCount = 0;
    int failCount = 0;
    qint64 totalFiles = 0;

    for (int i = 0; i < jobsArray.size(); ++i) {
        QJsonObject job = jobsArray[i].toObject();

        m_jobsTable->setItem(i, 0, new QTableWidgetItem(QString::number(job["jobid"].toInt())));
        m_jobsTable->setItem(i, 1, new QTableWidgetItem(job["name"].toString()));
        m_jobsTable->setItem(i, 2, new QTableWidgetItem(job["starttime"].toString()));
        m_jobsTable->setItem(i, 3, new QTableWidgetItem(job["endtime"].toString()));
        m_jobsTable->setItem(i, 4, new QTableWidgetItem(job["level"].toString()));
        m_jobsTable->setItem(i, 5, new QTableWidgetItem(QString::number(job["jobfiles"].toInt())));
        m_jobsTable->setItem(i, 6, new QTableWidgetItem(job["jobstatus"].toString()));

        // Calculate statistics
        QString status = job["jobstatus"].toString();
        if (status == "T") {
            successCount++;
        } else if (status == "f" || status == "E") {
            failCount++;
        }

        totalFiles += job["jobfiles"].toInt();
    }

    // Update statistics
    m_successfulJobsLabel->setText(QString::number(successCount));
    m_failedJobsLabel->setText(QString::number(failCount));
    m_totalFilesLabel->setText(QString::number(totalFiles));
}
