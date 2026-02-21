#include "schedules/bscheduledragresultdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QClipboard>
#include <QDialogButtonBox>

BScheduleDragResultDialog::BScheduleDragResultDialog(QWidget *parent)
    : QDialog(parent)
    , m_timeoutTimer(new QTimer(this))
{
    setWindowTitle(tr("Schedule Change"));
    setMinimumWidth(550);
    m_timeoutTimer->setSingleShot(true);
    m_timeoutTimer->setInterval(30000);
    connect(m_timeoutTimer, &QTimer::timeout, this, &BScheduleDragResultDialog::onTimeout);

    buildUI();
}

void BScheduleDragResultDialog::buildUI()
{
    auto *layout = new QVBoxLayout(this);

    // --- Summary ---
    m_summaryLabel = new QLabel(this);
    m_summaryLabel->setWordWrap(true);
    m_summaryLabel->setTextFormat(Qt::RichText);
    layout->addWidget(m_summaryLabel);

    // --- Warning (shared schedule) ---
    m_warningLabel = new QLabel(this);
    m_warningLabel->setWordWrap(true);
    m_warningLabel->setStyleSheet(
        "color: #b35900; font-style: italic; padding: 6px; "
        "background-color: #fff3e0; border: 1px solid #ffcc80; border-radius: 3px;");
    m_warningLabel->setVisible(false);
    layout->addWidget(m_warningLabel);

    // --- Dependencies ---
    m_dependencyLabel = new QLabel(this);
    m_dependencyLabel->setWordWrap(true);
    m_dependencyLabel->setTextFormat(Qt::RichText);
    m_dependencyLabel->setVisible(false);
    layout->addWidget(m_dependencyLabel);

    layout->addSpacing(8);

    // --- Config text block ---
    auto *configLabel = new QLabel(tr("Schedule Configuration:"), this);
    configLabel->setStyleSheet("font-weight: bold;");
    layout->addWidget(configLabel);

    m_configEdit = new QTextEdit(this);
    m_configEdit->setReadOnly(true);
    m_configEdit->setFont(QFont("Monospace", 9));
    m_configEdit->setMaximumHeight(200);
    layout->addWidget(m_configEdit);

    auto *configBtnLayout = new QHBoxLayout();
    m_copyConfigButton = new QPushButton(tr("Copy Configuration"), this);
    connect(m_copyConfigButton, &QPushButton::clicked, this, &BScheduleDragResultDialog::onCopyConfig);
    configBtnLayout->addWidget(m_copyConfigButton);
    configBtnLayout->addStretch();
    layout->addLayout(configBtnLayout);

    // --- Command block ---
    auto *cmdLabel = new QLabel(tr("Director Command:"), this);
    cmdLabel->setStyleSheet("font-weight: bold;");
    layout->addWidget(cmdLabel);

    m_commandEdit = new QTextEdit(this);
    m_commandEdit->setReadOnly(true);
    m_commandEdit->setFont(QFont("Monospace", 9));
    m_commandEdit->setMaximumHeight(80);
    layout->addWidget(m_commandEdit);

    auto *cmdBtnLayout = new QHBoxLayout();
    m_copyCommandButton = new QPushButton(tr("Copy Command"), this);
    connect(m_copyCommandButton, &QPushButton::clicked, this, &BScheduleDragResultDialog::onCopyCommand);
    cmdBtnLayout->addWidget(m_copyCommandButton);
    cmdBtnLayout->addStretch();
    layout->addLayout(cmdBtnLayout);

    // --- Status / Progress ---
    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    layout->addWidget(m_statusLabel);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0);
    m_progressBar->setVisible(false);
    layout->addWidget(m_progressBar);

    // --- Buttons ---
    layout->addSpacing(8);
    auto *buttonLayout = new QHBoxLayout();
    auto *noteLabel = new QLabel(
        tr("* Apply creates a new schedule via \"configure add\""), this);
    noteLabel->setStyleSheet("color: grey; font-size: 9pt;");
    buttonLayout->addWidget(noteLabel);
    buttonLayout->addStretch();

    m_applyButton = new QPushButton(tr("Apply"), this);
    m_cancelButton = new QPushButton(tr("Cancel"), this);
    buttonLayout->addWidget(m_applyButton);
    buttonLayout->addWidget(m_cancelButton);
    layout->addLayout(buttonLayout);

    connect(m_applyButton, &QPushButton::clicked, this, &BScheduleDragResultDialog::onApply);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void BScheduleDragResultDialog::setDragResult(
    const BScheduleEntry &entry,
    int newHour, int newMinute,
    const QList<BScheduleEntry> &allEntries,
    const QList<BScheduleGanttWidget::DependencyEdge> &deps,
    const BJobScheduleIndex *index)
{
    m_entry = entry;
    m_newHour = newHour;
    m_newMinute = newMinute;
    m_allEntries = allEntries;
    m_dependencies = deps;
    m_index = index;

    generatePreview();
}

void BScheduleDragResultDialog::generatePreview()
{
    // --- Summary ---
    int oldMinutes = m_entry.hour * 60 + m_entry.minute;
    int newMinutes = m_newHour * 60 + m_newMinute;
    int deltaMin = newMinutes - oldMinutes;
    QString deltaStr;
    if (deltaMin >= 0) {
        deltaStr = QString("+%1h %2m").arg(deltaMin / 60).arg(qAbs(deltaMin) % 60);
    } else {
        deltaStr = QString("-%1h %2m").arg(qAbs(deltaMin) / 60).arg(qAbs(deltaMin) % 60);
    }

    QString jobLabel = m_entry.jobName.isEmpty() ? m_entry.scheduleName : m_entry.jobName;
    QString summary = QString(
        "<b>%1: %2</b><br>"
        "<table>"
        "<tr><td>%3:</td><td>%4:%5 &rarr; %6:%7 (%8)</td></tr>"
        "<tr><td>Schedule:</td><td>%9</td></tr>")
        .arg(tr("Schedule Change"), jobLabel,
             tr("Time"),
             QString::number(m_entry.hour).rightJustified(2, '0'),
             QString::number(m_entry.minute).rightJustified(2, '0'),
             QString::number(m_newHour).rightJustified(2, '0'),
             QString::number(m_newMinute).rightJustified(2, '0'),
             deltaStr)
        .arg(m_entry.scheduleName);

    if (!m_entry.jobName.isEmpty()) {
        summary += QString("<tr><td>Job:</td><td>%1</td></tr>").arg(m_entry.jobName);
    }
    if (!m_entry.client.isEmpty()) {
        summary += QString("<tr><td>Client:</td><td>%1</td></tr>").arg(m_entry.client);
    }
    summary += "</table>";
    m_summaryLabel->setText(summary);

    // --- Shared schedule warning ---
    if (m_index && !m_entry.scheduleName.isEmpty()) {
        int shared = m_index->sharedCount(m_entry.scheduleName);
        if (shared > 1) {
            QList<BJobScheduleIndex::JobRef> jobs = m_index->jobsForSchedule(m_entry.scheduleName);
            QStringList jobNames;
            for (const auto &ref : jobs) {
                if (ref.jobName == m_entry.jobName) {
                    jobNames.append(ref.jobName + tr(" (this job)"));
                } else {
                    jobNames.append(ref.jobName + tr(" (affected!)"));
                }
            }
            m_warningLabel->setText(
                tr("Warning: This schedule is used by %1 jobs:\n%2")
                    .arg(shared)
                    .arg(jobNames.join(", ")));
            m_warningLabel->setVisible(true);
        } else {
            m_warningLabel->setVisible(false);
        }
    }

    // --- Dependencies ---
    if (!m_dependencies.isEmpty()) {
        QString depHtml = "<b>" + tr("Dependencies:") + "</b><ul>";
        for (const auto &dep : m_dependencies) {
            QString icon;
            QString color;
            switch (dep.type) {
            case BScheduleGanttWidget::DependencyEdge::LevelChain:
                icon = "&#9889;";  // lightning
                color = "#e67e00";
                break;
            case BScheduleGanttWidget::DependencyEdge::ClientExclusion:
                icon = "&#9888;";  // warning
                color = "#cc0000";
                break;
            case BScheduleGanttWidget::DependencyEdge::StorageContention:
                icon = "&#128280;";  // lock
                color = "#7b1fa2";
                break;
            }
            depHtml += QString("<li style='color:%1'>%2 %3</li>")
                .arg(color, icon, dep.description);
        }
        depHtml += "</ul>";
        m_dependencyLabel->setText(depHtml);
        m_dependencyLabel->setVisible(true);
    } else {
        m_dependencyLabel->setVisible(false);
    }

    // --- Config + Command ---
    m_configEdit->setPlainText(buildConfigText());
    m_commandEdit->setPlainText(buildConfigureCommand());
}

QString BScheduleDragResultDialog::buildConfigText() const
{
    // Build the full Schedule { ... } block with the modified Run directive
    QString config;
    QString newName = m_entry.scheduleName + "-modified";
    config += QString("Schedule {\n");
    config += QString("  Name = \"%1\"\n").arg(newName);

    // Collect all Run directives for this schedule
    for (const BScheduleEntry &e : m_allEntries) {
        if (e.scheduleName != m_entry.scheduleName) continue;

        BScheduleEntry modified = e;
        // If this is the dragged entry, apply the new time
        if (e.level == m_entry.level && e.hour == m_entry.hour
            && e.minute == m_entry.minute && e.daysOfWeek == m_entry.daysOfWeek) {
            modified.hour = m_newHour;
            modified.minute = m_newMinute;
            config += QString("  Run = %1  # was %2:%3\n")
                .arg(modified.toRunDirective(),
                     QString::number(m_entry.hour).rightJustified(2, '0'),
                     QString::number(m_entry.minute).rightJustified(2, '0'));
        } else {
            config += QString("  Run = %1\n").arg(e.toRunDirective());
        }
    }

    config += "}\n";
    return config;
}

QString BScheduleDragResultDialog::buildConfigureCommand() const
{
    QString newName = m_entry.scheduleName + "-modified";
    QStringList parts;
    parts.append(QString("configure add schedule name=\"%1\"").arg(newName));

    for (const BScheduleEntry &e : m_allEntries) {
        if (e.scheduleName != m_entry.scheduleName) continue;

        BScheduleEntry modified = e;
        if (e.level == m_entry.level && e.hour == m_entry.hour
            && e.minute == m_entry.minute && e.daysOfWeek == m_entry.daysOfWeek) {
            modified.hour = m_newHour;
            modified.minute = m_newMinute;
        }
        parts.append(QString("run=\"%1\"").arg(modified.toRunDirective()));
    }

    return parts.join(" ");
}

void BScheduleDragResultDialog::onCopyConfig()
{
    QApplication::clipboard()->setText(m_configEdit->toPlainText());
    m_statusLabel->setText(tr("Configuration copied to clipboard."));
}

void BScheduleDragResultDialog::onCopyCommand()
{
    QApplication::clipboard()->setText(m_commandEdit->toPlainText());
    m_statusLabel->setText(tr("Command copied to clipboard."));
}

void BScheduleDragResultDialog::onApply()
{
    if (!m_director) {
        m_statusLabel->setStyleSheet("color: red;");
        m_statusLabel->setText(tr("No director connection available."));
        return;
    }

    m_applyButton->setEnabled(false);
    m_progressBar->setVisible(true);
    m_statusLabel->clear();
    m_statusLabel->setStyleSheet("");

    // Connect to director response
    disconnectDirectorSignals();
    m_connections.append(
        connect(m_director, &BDirector::configureResult,
                this, &BScheduleDragResultDialog::onDirectorResponse));

    m_timeoutTimer->start();

    QString cmd = buildConfigureCommand();
    // Strip the leading "configure " since sendCommand adds the command type
    QString args = cmd.mid(QString("configure ").length());
    emit sendCommand(BDirector::Command::Configure, args);
}

void BScheduleDragResultDialog::onDirectorResponse(bool success, const QString &message)
{
    m_timeoutTimer->stop();
    disconnectDirectorSignals();
    m_progressBar->setVisible(false);

    if (success) {
        m_statusLabel->setStyleSheet("color: green; font-weight: bold;");
        m_statusLabel->setText(tr("Schedule created successfully."));
        m_applyButton->setEnabled(false);
    } else {
        m_statusLabel->setStyleSheet("color: red;");
        m_statusLabel->setText(tr("Error: %1").arg(message.left(300)));
        m_applyButton->setEnabled(true);
    }
}

void BScheduleDragResultDialog::onTimeout()
{
    disconnectDirectorSignals();
    m_progressBar->setVisible(false);
    m_statusLabel->setStyleSheet("color: orange;");
    m_statusLabel->setText(tr("Timeout: No response from Director after 30 seconds."));
    m_applyButton->setEnabled(true);
}

void BScheduleDragResultDialog::disconnectDirectorSignals()
{
    for (const auto &conn : m_connections) {
        disconnect(conn);
    }
    m_connections.clear();
}
