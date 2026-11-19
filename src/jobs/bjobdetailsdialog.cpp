#include "jobs/bjobdetailsdialog.h"
#include "buiscale.h"
#include "bstatusdelegate.h"
#include "jobs/bjobwidget.h"
#include "jobs/bjobfileswidget.h"
#include "blogging.h"
#include <QGroupBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFont>
#include <QLocale>
#include <QDateTime>


BJobDetailsDialog::BJobDetailsDialog(const QJsonObject &job, BJobWidget *jobWidget, BDirector *director, QWidget *parent)
    : QDialog(parent)
    , m_job(job)
    , m_jobWidget(jobWidget)
    , m_director(director)
    , m_jobId(job["jobid"].toString().toULongLong())
    , m_clientName(job["client"].toString())
{
    setupUi(job);

    // Create descriptive title with Job-Name, ID and Client
    QString jobName = job["name"].toString();
    QString jobId = job["jobid"].toString();
    QString client = job["client"].toString();

    setWindowTitle(QString("Job Details: %1 (ID: %2) - Client: %3")
                       .arg(jobName)
                       .arg(jobId)
                       .arg(client));

    resize(BUiScale::px(900), BUiScale::px(600));

    // Query full job details (list jobs doesn't include fileset, schedule, etc.)
    queryJobDetails();
}

void BJobDetailsDialog::setupUi(const QJsonObject &job)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create tab widget
    m_tabWidget = new QTabWidget(this);

    // Setup Files Tab
    setupFilesTab();

    // Setup Status Tab
    setupStatusTab(job);

    // Setup Log Tab
    setupLogTab();

    mainLayout->addWidget(m_tabWidget);

    // Button box
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    mainLayout->addWidget(buttonBox);
}

void BJobDetailsDialog::setupFilesTab()
{
    // Use the reusable BJobFilesWidget for the Files tab
    m_filesWidget = new BJobFilesWidget(m_job, m_director, this);
    m_tabWidget->addTab(m_filesWidget, tr("Files"));
}

void BJobDetailsDialog::setupStatusTab(const QJsonObject &job)
{
    m_statusWidget = new QWidget();
    QVBoxLayout *statusLayout = new QVBoxLayout(m_statusWidget);

    // Job Information Group
    QGroupBox *jobInfoGroup = new QGroupBox(tr("Job Information"), m_statusWidget);
    QFormLayout *jobInfoLayout = new QFormLayout(jobInfoGroup);

    jobInfoLayout->addRow(tr("Job ID:"), new QLabel(job["jobid"].toString()));
    jobInfoLayout->addRow(tr("Job Name:"), new QLabel(job["name"].toString()));
    jobInfoLayout->addRow(tr("Client:"), new QLabel(job["client"].toString()));

    // Backup Type and Level
    QString type = job["type"].toString();
    QString typeDesc = (type == "B") ? tr("Backup") : type;
    jobInfoLayout->addRow(tr("Type:"), new QLabel(typeDesc));

    QString level = job["level"].toString();
    QString levelDesc;
    if (level == "F") levelDesc = tr("Full");
    else if (level == "I") levelDesc = tr("Incremental");
    else if (level == "D") levelDesc = tr("Differential");
    else levelDesc = level;
    jobInfoLayout->addRow(tr("Level:"), new QLabel(levelDesc));

    // FileSet (may be empty from "list jobs" — filled by llist jobid query)
    QString fileset = job["fileset"].toString();
    m_fileSetLabel = new QLabel(fileset.isEmpty() ? tr("...") : fileset);
    jobInfoLayout->addRow(tr("FileSet:"), m_fileSetLabel);

    // Scheduler (may also be empty from "list jobs")
    QString schedname = job["schedname"].toString();
    m_scheduleLabel = new QLabel(schedname.isEmpty() ? QString() : schedname);
    if (!schedname.isEmpty()) {
        jobInfoLayout->addRow(tr("Schedule:"), m_scheduleLabel);
    }

    m_jobInfoLayout = jobInfoLayout;

    statusLayout->addWidget(jobInfoGroup);

    // Timing Information Group
    QGroupBox *timingGroup = new QGroupBox(tr("Timing"), m_statusWidget);
    QFormLayout *timingLayout = new QFormLayout(timingGroup);

    QString startTimeStr = job["starttime"].toString();
    QDateTime startTime = QDateTime::fromString(startTimeStr, Qt::ISODate);
    if (!startTime.isValid())
        startTime = QDateTime::fromString(startTimeStr, "yyyy-MM-dd HH:mm:ss");
    timingLayout->addRow(tr("Start Time:"),
                         new QLabel(startTime.isValid()
                                        ? QLocale().toString(startTime, QLocale::ShortFormat)
                                        : startTimeStr));
    timingLayout->addRow(tr("Duration:"), new QLabel(job["duration"].toString()));

    statusLayout->addWidget(timingGroup);

    // Statistics Group
    QGroupBox *statsGroup = new QGroupBox(tr("Statistics"), m_statusWidget);
    QFormLayout *statsLayout = new QFormLayout(statsGroup);

    QString filesCount = job["jobfiles"].toString();
    statsLayout->addRow(tr("Files Backed Up:"), new QLabel(filesCount));

    qint64 bytes = job["jobbytes"].toString().toLongLong();
    QString bytesFormatted = formatBytes(bytes);
    QString bytesExact = QString("%1 bytes").arg(job["jobbytes"].toString());
    QLabel *bytesLabel = new QLabel(bytesFormatted);
    bytesLabel->setToolTip(bytesExact);
    statsLayout->addRow(tr("Data Size:"), bytesLabel);

    statusLayout->addWidget(statsGroup);

    // Status Group
    QGroupBox *statusGroup = new QGroupBox(tr("Status"), m_statusWidget);
    QFormLayout *statusGroupLayout = new QFormLayout(statusGroup);

    QString status = job["jobstatus"].toString();
    QString statusDesc = formatStatus(status);

    // The status chip of the job table: letter, word, semantic colour from
    // the theme's tokens (green only for 'T')
    QLabel *statusLabel = new QLabel(statusDesc);
    statusLabel->setStyleSheet(statusStyleSheet(status));
    statusLabel->setAccessibleName(BStatus::jobStatusText(status));
    statusGroupLayout->addRow(tr("Job Status:"), statusLabel);

    statusLayout->addWidget(statusGroup);

    statusLayout->addStretch();

    m_tabWidget->addTab(m_statusWidget, tr("Status"));
}

void BJobDetailsDialog::setupLogTab()
{
    QWidget *logWidget = new QWidget();
    QVBoxLayout *logLayout = new QVBoxLayout(logWidget);

    // Shared job log view with a local model for this dialog's job:
    // per-pixel scrolling, follow-tail, live source
    m_logListView = new BJobLogView(logWidget);
    m_logModel = new BJobLogModel(this);
    m_logListView->setModel(m_logModel);
    m_logListView->attach(m_director, m_logModel);
    m_logListView->setPlaceholderText(tr("Loading job log..."));

    logLayout->addWidget(m_logListView);

    m_tabWidget->addTab(logWidget, tr("Logs"));

    if (m_director) {
        // Wait for BVFS loading to finish before requesting log (avoids m_lastCommand race).
        // The view fetches once its tab is visible and keeps a running job's log up to date.
        connect(m_filesWidget, &BJobFilesWidget::filesLoaded, this, [this]() {
            if (m_director) {
                BLOG_DEBUG() << "BJobDetailsDialog: BVFS done, requesting log for Job ID" << m_jobId;
                m_logListView->showJob(m_jobId,
                                       BJobLog::isRunningJobStatus(m_job["jobstatus"].toString()));
            }
        });
    } else {
        m_logListView->setPlaceholderText(tr("No connection to the Director."));
    }
}

void BJobDetailsDialog::queryJobDetails()
{
    if (!m_director) return;

    connect(m_director, &BDirector::jsonResult,
            this, &BJobDetailsDialog::onJobDetailReceived);
    connect(m_director, &BDirector::textResult,
            this, &BJobDetailsDialog::onJobDetailReceived);

    m_director->doSend(BDirector::Command::Custom,
                       QString("llist jobid=%1").arg(m_jobId));
}

void BJobDetailsDialog::onJobDetailReceived(BDirector::Command cmd, const QString &jsonData)
{
    if (cmd != BDirector::Command::Custom) return;

    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    QJsonArray jobs = result["jobs"].toArray();
    if (jobs.isEmpty()) return;

    QJsonObject job = jobs[0].toObject();

    // Update FileSet
    QString fileset = job["fileset"].toString();
    if (!fileset.isEmpty() && m_fileSetLabel) {
        m_fileSetLabel->setText(fileset);
    }

    // Update Schedule if it was empty before
    QString schedname = job["schedname"].toString();
    if (!schedname.isEmpty() && m_scheduleLabel) {
        if (m_scheduleLabel->text().isEmpty() && m_jobInfoLayout) {
            m_jobInfoLayout->addRow(tr("Schedule:"), m_scheduleLabel);
        }
        m_scheduleLabel->setText(schedname);
    }

    // Disconnect after handling
    if (m_director) {
        disconnect(m_director, &BDirector::jsonResult,
                   this, &BJobDetailsDialog::onJobDetailReceived);
        disconnect(m_director, &BDirector::textResult,
                   this, &BJobDetailsDialog::onJobDetailReceived);
    }
}

QString BJobDetailsDialog::formatBytes(qint64 bytes) const
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    const qint64 TB = GB * 1024;

    if (bytes >= TB) {
        return QString("%1 TB").arg(bytes / (double)TB, 0, 'f', 2);
    } else if (bytes >= GB) {
        return QString("%1 GB").arg(bytes / (double)GB, 0, 'f', 2);
    } else if (bytes >= MB) {
        return QString("%1 MB").arg(bytes / (double)MB, 0, 'f', 2);
    } else if (bytes >= KB) {
        return QString("%1 KB").arg(bytes / (double)KB, 0, 'f', 2);
    } else {
        return QString("%1 B").arg(bytes);
    }
}

QString BJobDetailsDialog::formatStatus(const QString &status) const
{
    const BStatus::Chip chip = BStatus::jobStatus(status);
    return QStringLiteral("%1  %2").arg(chip.code, chip.fullText);
}

QString BJobDetailsDialog::statusStyleSheet(const QString &status) const
{
    BStatusPalette::instance();  // the theme's tokens
    const BStatus::Tone tone = BStatus::jobStatus(status).tone;
    return QStringLiteral("QLabel { background-color: %1; color: %2; border: 1px solid %3; "
                          "padding: %4px %5px; border-radius: %6px; font-weight: bold; }")
        .arg(BStatus::color(tone, BStatus::Part::Background).name(),
             BStatus::color(tone, BStatus::Part::Foreground).name(),
             BStatus::color(tone, BStatus::Part::Accent).name())
        .arg(BUiScale::px(4))
        .arg(BUiScale::px(8))
        .arg(BUiScale::px(4));
}
