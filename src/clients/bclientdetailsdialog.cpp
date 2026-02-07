#include "clients/bclientdetailsdialog.h"
#include "clients/bclientsmodel.h"
#include "blogging.h"
#include <QDateTime>
#include <QFont>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLabel>
#include <QLocale>
#include <QPushButton>
#include <QRegularExpression>
#include <QTableView>
#include <QTabWidget>
#include <QTimeZone>
#include "director/bresourcewidget.h"
#include <QRadioButton>
#include <QVBoxLayout>
#include "bsettings.h"
#include "bconnectionprofile.h"

// ============================================================================
// BClientJobsModel
// ============================================================================

BClientJobsModel::BClientJobsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int BClientJobsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_jobs.size();
}

int BClientJobsModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : COL_COUNT;
}

QVariant BClientJobsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_jobs.size())
        return QVariant();

    const QJsonObject job = m_jobs[index.row()].toObject();

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case COL_JOBID:
            return job["jobid"].toVariant().toInt();
        case COL_NAME:
            return job["name"].toString();
        case COL_START: {
            QString startTime = job["starttime"].toString();
            QDateTime dt = QDateTime::fromString(startTime, Qt::ISODate);
            if (!dt.isValid())
                dt = QDateTime::fromString(startTime, "yyyy-MM-dd HH:mm:ss");
            return dt.isValid() ? QLocale().toString(dt, QLocale::ShortFormat) : startTime;
        }
        case COL_DURATION: {
            QString dur = job["duration"].toString();
            // Already HH:MM:SS from Bareos API 2
            if (dur.contains(':'))
                return dur;
            // Fallback: raw seconds → HH:MM:SS
            qint64 secs = dur.toLongLong();
            return QString("%1:%2:%3")
                .arg(secs / 3600, 2, 10, QChar('0'))
                .arg((secs % 3600) / 60, 2, 10, QChar('0'))
                .arg(secs % 60, 2, 10, QChar('0'));
        }
        case COL_LEVEL: {
            QString level = job["level"].toString();
            if (level == "F") return tr("Full");
            if (level == "I") return tr("Incremental");
            if (level == "D") return tr("Differential");
            return level;
        }
        case COL_FILES:
            return job["jobfiles"].toVariant().toLongLong();
        case COL_BYTES:
            return formatBytes(job["jobbytes"].toVariant().toLongLong());
        case COL_STATUS: {
            QString status = job["jobstatus"].toString();
            if (status == "T") return tr("OK");
            if (status == "R") return tr("Running");
            if (status == "B") return tr("Blocked");
            if (status == "C") return tr("Created");
            if (status == "F") return tr("Failed");
            if (status == "E") return tr("Error");
            if (status == "A") return tr("Canceled");
            if (status == "W") return tr("Warning");
            return status;
        }
        }
    } else if (role == Qt::ForegroundRole && index.column() == COL_STATUS) {
        QString status = job["jobstatus"].toString();
        if (status == "T") return QColor(0, 128, 0);
        if (status == "f" || status == "E") return QColor(Qt::red);
        if (status == "W") return QColor(200, 150, 0);
    } else if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case COL_JOBID:
        case COL_FILES:
            return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);
        case COL_BYTES:
        case COL_DURATION:
            return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);
        }
    }

    return QVariant();
}

QVariant BClientJobsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
    case COL_JOBID:    return tr("Job ID");
    case COL_NAME:     return tr("Name");
    case COL_START:    return tr("Start");
    case COL_DURATION: return tr("Duration");
    case COL_LEVEL:    return tr("Level");
    case COL_FILES:    return tr("Files");
    case COL_BYTES:    return tr("Bytes");
    case COL_STATUS:   return tr("Status");
    }
    return QVariant();
}

void BClientJobsModel::setJobs(const QJsonArray &jobs)
{
    beginResetModel();
    m_jobs = jobs;

    // Calculate statistics
    m_stats = Statistics();
    m_stats.totalJobs = jobs.size();

    for (const QJsonValue &val : jobs) {
        QJsonObject job = val.toObject();
        QString status = job["jobstatus"].toString();

        if (status == "T")
            m_stats.successfulJobs++;
        else if (status == "f" || status == "E")
            m_stats.failedJobs++;

        m_stats.totalFiles += job["jobfiles"].toVariant().toLongLong();
        m_stats.totalBytes += job["jobbytes"].toVariant().toLongLong();
    }

    endResetModel();
}

QString BClientJobsModel::formatBytes(qint64 bytes) const
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    const qint64 TB = GB * 1024;

    if (bytes >= TB)
        return QString("%1 TB").arg(bytes / (double)TB, 0, 'f', 2);
    if (bytes >= GB)
        return QString("%1 GB").arg(bytes / (double)GB, 0, 'f', 2);
    if (bytes >= MB)
        return QString("%1 MB").arg(bytes / (double)MB, 0, 'f', 2);
    if (bytes >= KB)
        return QString("%1 KB").arg(bytes / (double)KB, 0, 'f', 2);
    return QString("%1 B").arg(bytes);
}

BClientDetailsDialog::BClientDetailsDialog(BDirector *director, BClientsModel *model, int row, QWidget *parent)
    : QDialog(parent)
    , m_client(model->enrichedClient(row))
    , m_director(director)
    , m_model(model)
    , m_clientName(m_client["name"].toString())
    , m_row(row)
{
    setWindowTitle(tr("Client Details - %1").arg(m_clientName));
    resize(800, 600);

    setupUI();
    loadClientJobs();
}

void BClientDetailsDialog::showSettingsTab()
{
    m_tabWidget->setCurrentWidget(m_settingsTab);
}

void BClientDetailsDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_tabWidget = new QTabWidget(this);
    setupInfoTab();
    setupSettingsTab();
    setupJobsTab();
    setupStatisticsTab();

    mainLayout->addWidget(m_tabWidget);

    // Button box
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    QPushButton *closeButton = new QPushButton(tr("Close"), this);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);
}

// Helper: extract Bareos FD version from uname string
static QString extractVersion(const QString &uname)
{
    QRegularExpression versionRx(R"((\d+\.\d+(?:\.\d+)*))");
    QRegularExpressionMatch match = versionRx.match(uname);
    if (match.hasMatch())
        return match.captured(1);
    int spaceIdx = uname.indexOf(' ');
    return (spaceIdx > 0) ? uname.mid(spaceIdx + 1).trimmed() : QString();
}

// Helper: extract OS from uname string (same logic as BClientsModel::COL_OS)
static QString extractOS(const QString &uname)
{
    QString trimmed = uname.trimmed();
    bool isLinux = trimmed.contains("linux", Qt::CaseInsensitive);

    QRegularExpression distroRx(
        R"(,(debian|ubuntu|centos|redhat|red\s?hat|suse|fedora|arch|gentoo|alpine|rocky|alma)[- ]?([\d.]*),)",
        QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch distroMatch = distroRx.match(trimmed);
    if (distroMatch.hasMatch()) {
        QString distro = distroMatch.captured(1);
        distro[0] = distro[0].toUpper();
        QString ver = distroMatch.captured(2);
        QString label = ver.isEmpty() ? distro : QString("%1 %2").arg(distro, ver);
        return isLinux ? QString("Linux %1").arg(label) : label;
    }

    QRegularExpression debRx(R"(\+deb(\d+))", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch debMatch = debRx.match(trimmed);
    if (debMatch.hasMatch())
        return QString("Linux Debian %1").arg(debMatch.captured(1));

    for (const QString &os : {"Windows", "FreeBSD", "Darwin", "macOS",
                               "Solaris", "AIX", "HP-UX"}) {
        if (trimmed.contains(os, Qt::CaseInsensitive))
            return os;
    }

    if (isLinux)
        return QStringLiteral("Linux");

    return trimmed.section(' ', 0, 0);
}

// Helper: format bytes to human-readable string
static QString formatBytes(qint64 bytes)
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    const qint64 TB = GB * 1024;

    if (bytes >= TB)
        return QString("%1 TB").arg(bytes / (double)TB, 0, 'f', 2);
    if (bytes >= GB)
        return QString("%1 GB").arg(bytes / (double)GB, 0, 'f', 2);
    if (bytes >= MB)
        return QString("%1 MB").arg(bytes / (double)MB, 0, 'f', 2);
    if (bytes >= KB)
        return QString("%1 KB").arg(bytes / (double)KB, 0, 'f', 2);
    return QString("%1 B").arg(bytes);
}

// Helper: format Bareos retention time (seconds) to human-readable string
static QString formatRetention(const QJsonValue &val)
{
    // Bareos returns retention as seconds (integer or string)
    qint64 secs = val.isString() ? val.toString().toLongLong()
                                 : static_cast<qint64>(val.toDouble());
    if (secs <= 0)
        return QObject::tr("Not set");

    qint64 days = secs / 86400;
    if (days >= 365 && days % 365 == 0)
        return QObject::tr("%1 year(s)").arg(days / 365);
    if (days > 0)
        return QObject::tr("%1 day(s)").arg(days);

    qint64 hours = secs / 3600;
    if (hours > 0)
        return QObject::tr("%1 hour(s)").arg(hours);

    return QObject::tr("%1 second(s)").arg(secs);
}

void BClientDetailsDialog::setupInfoTab()
{
    QWidget *infoTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(infoTab);

    // General Information Group
    QGroupBox *generalGroup = new QGroupBox(tr("General Information"));
    QFormLayout *generalForm = new QFormLayout(generalGroup);

    QString uname = m_client["uname"].toString();

    m_nameLabel = new QLabel(m_client["name"].toString());
    m_addressLabel = new QLabel(m_client["address"].toString().isEmpty()
                                ? tr("N/A") : m_client["address"].toString());
    int port = m_client["port"].toInt(0);
    m_portLabel = new QLabel(port > 0 ? QString::number(port) : tr("N/A"));
    m_versionLabel = new QLabel(extractVersion(uname));
    m_osLabel = new QLabel(extractOS(uname));
    m_osLabel->setToolTip(uname);  // Full uname as tooltip

    generalForm->addRow(tr("Name:"), m_nameLabel);
    generalForm->addRow(tr("Address:"), m_addressLabel);
    generalForm->addRow(tr("Port:"), m_portLabel);
    generalForm->addRow(tr("Version:"), m_versionLabel);
    generalForm->addRow(tr("Operating System:"), m_osLabel);

    layout->addWidget(generalGroup);

    // Connection Information Group
    QGroupBox *connGroup = new QGroupBox(tr("Connection Information"));
    QFormLayout *connForm = new QFormLayout(connGroup);

    // Status - use UTC comparison (same as BClientsModel::getClientStatus)
    QString statusText;
    QString lastConn = m_client["lastconnection"].toString();
    if (lastConn.isEmpty() || lastConn == "0") {
        statusText = tr("Offline (Never connected)");
        m_lastConnLabel = new QLabel(tr("Never"));
    } else {
        QDateTime lastConnTime = QDateTime::fromString(lastConn, Qt::ISODate);
        if (!lastConnTime.isValid())
            lastConnTime = QDateTime::fromString(lastConn, "yyyy-MM-dd HH:mm:ss");

        if (lastConnTime.isValid()) {
            lastConnTime.setTimeZone(QTimeZone::utc());
            QDateTime nowUtc = QDateTime::currentDateTimeUtc();
            qint64 secsSinceLastConn = lastConnTime.secsTo(nowUtc);
            if (secsSinceLastConn >= 0 && secsSinceLastConn < 86400) {
                statusText = tr("Online");
            } else {
                qint64 days = secsSinceLastConn / 86400;
                statusText = tr("Offline (since %n day(s))", "", days);
            }
            m_lastConnLabel = new QLabel(QLocale().toString(lastConnTime.toLocalTime(), QLocale::ShortFormat));
        } else {
            statusText = tr("Unknown");
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
    connForm->addRow(tr("Last Connection:"), m_lastConnLabel);

    layout->addWidget(connGroup);

    // Retention Information Group
    QGroupBox *retentionGroup = new QGroupBox(tr("Retention Policies"));
    QFormLayout *retentionForm = new QFormLayout(retentionGroup);

    bool autoprune = m_client["autoprune"].toBool();
    m_autoprune = new QLabel(m_client.contains("autoprune")
                             ? (autoprune ? tr("Yes") : tr("No"))
                             : tr("N/A"));
    m_fileRetention = new QLabel(formatRetention(m_client["fileretention"]));
    m_jobRetention = new QLabel(formatRetention(m_client["jobretention"]));

    retentionForm->addRow(tr("Auto Prune:"), m_autoprune);
    retentionForm->addRow(tr("File Retention:"), m_fileRetention);
    retentionForm->addRow(tr("Job Retention:"), m_jobRetention);

    layout->addWidget(retentionGroup);

    layout->addStretch();

    m_tabWidget->addTab(infoTab, tr("Information"));
}

void BClientDetailsDialog::setupSettingsTab()
{
    m_settingsTab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(m_settingsTab);
    layout->setContentsMargins(4, 4, 4, 4);

    // Auto-detect initial TLS mode from Director data

    // Resource widget
    m_settingsWidget = new BResourceWidget("Client", m_director, this);
    m_settingsWidget->setExportName(m_clientName);
    layout->addWidget(m_settingsWidget);

    // Initial load
    onTlsModeChanged();

    m_tabWidget->addTab(m_settingsTab, tr("Settings"));
}

void BClientDetailsDialog::onTlsModeChanged()
{
    // Detect TLS mode from connection profile or default to PSK
    BClientsModel::TlsMode mode = BClientsModel::TLS_PSK;
    BConnectionProfile profile = BSettings::instance().lastUsedProfile();
    if (profile.isValid() && !profile.tlsUsePSK) {
        mode = BClientsModel::TLS_X509;
    }

    QList<BConfigResource> allRes = m_model->clientResources(m_row, mode);

    // Display only the Director-side Client resource
    QList<BConfigResource> dirClientRes;
    for (const auto &res : allRes) {
        if (res.type() == "Client")
            dirClientRes.append(res);
    }
    m_settingsWidget->setResources(dirClientRes);

    // Export includes all resources (FD + Director side)
    m_settingsWidget->setExportResources(allRes);
}

void BClientDetailsDialog::setupJobsTab()
{
    QWidget *jobsTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(jobsTab);

    // Toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    QLabel *infoLabel = new QLabel(tr("Recent jobs for this client:"));
    toolbarLayout->addWidget(infoLabel);
    toolbarLayout->addStretch();

    m_refreshJobsButton = new QPushButton(tr("Refresh"));
    connect(m_refreshJobsButton, &QPushButton::clicked, this, &BClientDetailsDialog::loadClientJobs);
    toolbarLayout->addWidget(m_refreshJobsButton);

    layout->addLayout(toolbarLayout);

    // Jobs model + table view
    m_jobsModel = new BClientJobsModel(this);

    m_jobsTable = new QTableView();
    m_jobsTable->setModel(m_jobsModel);
    m_jobsTable->setAlternatingRowColors(true);
    m_jobsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_jobsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_jobsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_jobsTable->setSortingEnabled(true);
    m_jobsTable->verticalHeader()->hide();

    // Column sizing
    QHeaderView *header = m_jobsTable->horizontalHeader();
    header->setSectionResizeMode(BClientJobsModel::COL_NAME, QHeaderView::Stretch);
    header->setSectionResizeMode(BClientJobsModel::COL_JOBID, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(BClientJobsModel::COL_START, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(BClientJobsModel::COL_DURATION, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(BClientJobsModel::COL_LEVEL, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(BClientJobsModel::COL_FILES, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(BClientJobsModel::COL_BYTES, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(BClientJobsModel::COL_STATUS, QHeaderView::ResizeToContents);

    layout->addWidget(m_jobsTable);

    m_tabWidget->addTab(jobsTab, tr("Jobs"));
}

void BClientDetailsDialog::setupStatisticsTab()
{
    QWidget *statsTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(statsTab);

    QGroupBox *statsGroup = new QGroupBox(tr("Statistics"));
    QFormLayout *statsForm = new QFormLayout(statsGroup);

    int jobCount = m_client["jobcount"].toInt();
    qint64 totalBytes = m_client["totalbytes"].isString()
                        ? m_client["totalbytes"].toString().toLongLong()
                        : static_cast<qint64>(m_client["totalbytes"].toDouble());

    m_totalJobsLabel = new QLabel(QString::number(jobCount));
    m_successfulJobsLabel = new QLabel(tr("Loading..."));
    m_failedJobsLabel = new QLabel(tr("Loading..."));
    m_totalBytesLabel = new QLabel(totalBytes > 0 ? formatBytes(totalBytes) : "0 B");
    m_totalFilesLabel = new QLabel(tr("Loading..."));

    statsForm->addRow(tr("Total Jobs:"), m_totalJobsLabel);
    statsForm->addRow(tr("Successful Jobs:"), m_successfulJobsLabel);
    statsForm->addRow(tr("Failed Jobs:"), m_failedJobsLabel);
    statsForm->addRow(tr("Total Data:"), m_totalBytesLabel);
    statsForm->addRow(tr("Total Files:"), m_totalFilesLabel);

    layout->addWidget(statsGroup);
    layout->addStretch();

    m_tabWidget->addTab(statsTab, tr("Statistics"));
}

void BClientDetailsDialog::loadClientJobs()
{
    if (!m_director) {
        BLOG_DEBUG() << "BClientDetailsDialog: No director connection";
        m_successfulJobsLabel->setText("N/A");
        m_failedJobsLabel->setText("N/A");
        m_totalFilesLabel->setText("N/A");
        return;
    }

    m_refreshJobsButton->setEnabled(false);

    // Connect to receive response
    connect(m_director, &BDirector::jsonResponse,
            this, &BClientDetailsDialog::onJobsReceived,
            Qt::UniqueConnection);

    // Send "list jobs client=<name>" via Custom command
    QString cmd = QString("list jobs client=%1").arg(m_clientName);
    QMetaObject::invokeMethod(m_director, "doSendCommand",
                              Qt::QueuedConnection,
                              Q_ARG(BDirector::Command, BDirector::Command::Custom),
                              Q_ARG(QString, cmd));
}

void BClientDetailsDialog::onJobsReceived(const QString &command, const QString &response)
{
    if (!command.contains("list jobs"))
        return;

    m_refreshJobsButton->setEnabled(true);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BClientDetailsDialog: JSON parse error:" << error.errorString();
        return;
    }

    QJsonArray jobsArray;

    // Extract jobs array: result.jobs[] or top-level jobs[] or direct array
    if (doc.isArray()) {
        jobsArray = doc.array();
    } else if (doc.isObject()) {
        QJsonObject root = doc.object();
        if (root.contains("result") && root["result"].isObject()) {
            QJsonObject result = root["result"].toObject();
            if (result.contains("jobs") && result["jobs"].isArray())
                jobsArray = result["jobs"].toArray();
        } else if (root.contains("jobs") && root["jobs"].isArray()) {
            jobsArray = root["jobs"].toArray();
        }
    }

    BLOG_DEBUG() << "BClientDetailsDialog: Found" << jobsArray.size() << "jobs";

    // Populate model (calculates statistics internally)
    m_jobsModel->setJobs(jobsArray);

    // Update statistics tab from model
    BClientJobsModel::Statistics stats = m_jobsModel->statistics();
    m_totalJobsLabel->setText(QString::number(stats.totalJobs));
    m_successfulJobsLabel->setText(QString::number(stats.successfulJobs));
    m_failedJobsLabel->setText(QString::number(stats.failedJobs));
    m_totalFilesLabel->setText(QLocale().toString(stats.totalFiles));
    if (stats.totalBytes > 0)
        m_totalBytesLabel->setText(formatBytes(stats.totalBytes));
}
