#include "jobs/bjobdetailsdialog.h"
#include <QGroupBox>

BJobDetailsDialog::BJobDetailsDialog(const QJsonObject &job, QWidget *parent)
    : QDialog(parent)
{
    setupUi(job);
    setWindowTitle(QString("Job Details - %1").arg(job["name"].toString()));
    resize(500, 400);
}

void BJobDetailsDialog::setupUi(const QJsonObject &job)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Job Information Group
    QGroupBox *jobInfoGroup = new QGroupBox("Job Information", this);
    QFormLayout *jobInfoLayout = new QFormLayout(jobInfoGroup);
    
    jobInfoLayout->addRow("Job ID:", new QLabel(job["jobid"].toString()));
    jobInfoLayout->addRow("Job Name:", new QLabel(job["name"].toString()));
    jobInfoLayout->addRow("Client:", new QLabel(job["client"].toString()));
    
    // Backup Type and Level
    QString type = job["type"].toString();
    QString typeDesc = (type == "B") ? "Backup" : type;
    jobInfoLayout->addRow("Type:", new QLabel(typeDesc));
    
    QString level = job["level"].toString();
    QString levelDesc;
    if (level == "F") levelDesc = "Full";
    else if (level == "I") levelDesc = "Incremental";
    else if (level == "D") levelDesc = "Differential";
    else levelDesc = level;
    jobInfoLayout->addRow("Level:", new QLabel(levelDesc));
    
    mainLayout->addWidget(jobInfoGroup);
    
    // Timing Information Group
    QGroupBox *timingGroup = new QGroupBox("Timing", this);
    QFormLayout *timingLayout = new QFormLayout(timingGroup);
    
    timingLayout->addRow("Start Time:", new QLabel(job["starttime"].toString()));
    timingLayout->addRow("Duration:", new QLabel(job["duration"].toString()));
    
    mainLayout->addWidget(timingGroup);
    
    // Statistics Group
    QGroupBox *statsGroup = new QGroupBox("Statistics", this);
    QFormLayout *statsLayout = new QFormLayout(statsGroup);
    
    QString filesCount = job["jobfiles"].toString();
    statsLayout->addRow("Files Backed Up:", new QLabel(filesCount));
    
    qint64 bytes = job["jobbytes"].toString().toLongLong();
    QString bytesFormatted = formatBytes(bytes);
    QString bytesExact = QString("%1 bytes").arg(job["jobbytes"].toString());
    QLabel *bytesLabel = new QLabel(bytesFormatted);
    bytesLabel->setToolTip(bytesExact);
    statsLayout->addRow("Data Size:", bytesLabel);
    
    mainLayout->addWidget(statsGroup);
    
    // Status Group
    QGroupBox *statusGroup = new QGroupBox("Status", this);
    QFormLayout *statusLayout = new QFormLayout(statusGroup);
    
    QString status = job["jobstatus"].toString();
    QString statusDesc = formatStatus(status);
    QString statusColor = getStatusColor(status);
    
    QLabel *statusLabel = new QLabel(statusDesc);
    statusLabel->setStyleSheet(QString("QLabel { "
                                       "background-color: %1; "
                                       "padding: 5px; "
                                       "border-radius: 3px; "
                                       "font-weight: bold; "
                                       "}").arg(statusColor));
    statusLayout->addRow("Job Status:", statusLabel);
    
    mainLayout->addWidget(statusGroup);
    
    // Add stretch to push button box to bottom
    mainLayout->addStretch();
    
    // Button box
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    mainLayout->addWidget(buttonBox);
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
    if (status == "T") return "Terminated normally ✓";
    if (status == "W") return "Terminated with warnings ⚠";
    if (status == "f") return "Failed ✗";
    if (status == "E") return "Terminated in Error ✗";
    if (status == "e") return "Non-fatal error ⚠";
    if (status == "A") return "Canceled by user";
    if (status == "R") return "Running...";
    if (status == "C") return "Created";
    
    return QString("Status: %1").arg(status);
}

QString BJobDetailsDialog::getStatusColor(const QString &status) const
{
    if (status == "T") return "#c8ffc8";  // Light green
    if (status == "W") return "#ffffc8";  // Light yellow
    if (status == "f" || status == "E") return "#ffc8c8";  // Light red
    
    return "#e0e0e0";  // Light gray
}
