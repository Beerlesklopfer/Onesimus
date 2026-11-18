#include "schedules/bscheduledragresultdialog.h"
#include "blabelstate.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QClipboard>
#include <QDialogButtonBox>
#include <QHeaderView>

using BScheduleDragPlan::Reason;
using BScheduleDragPlan::Verdict;

BScheduleDragResultDialog::BScheduleDragResultDialog(QWidget *parent)
    : QDialog(parent)
    , m_timeoutTimer(new QTimer(this))
{
    setWindowTitle(tr("Create Schedule Copy"));
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

    // --- What Apply does (or why nothing can be applied) ---
    m_effectLabel = new QLabel(this);
    m_effectLabel->setWordWrap(true);
    m_effectLabel->setTextFormat(Qt::PlainText);
    layout->addWidget(m_effectLabel);

    // --- Jobs that use the schedule ---
    m_warningLabel = new QLabel(this);
    m_warningLabel->setWordWrap(true);
    m_warningLabel->setTextFormat(Qt::PlainText);
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

    // --- Original and generated Run lines side by side ---
    m_compareLabel = new QLabel(this);
    m_compareLabel->setWordWrap(true);
    m_compareLabel->setTextFormat(Qt::PlainText);
    m_compareLabel->setVisible(false);
    layout->addWidget(m_compareLabel);

    m_compareTable = new QTableWidget(this);
    m_compareTable->setColumnCount(3);
    m_compareTable->setHorizontalHeaderLabels(
        {tr("Original Run line"), tr("Generated Run line"), tr("Difference")});
    m_compareTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_compareTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_compareTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_compareTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_compareTable->setSelectionMode(QAbstractItemView::NoSelection);
    m_compareTable->setFont(QFont("Monospace", 9));
    m_compareTable->setMaximumHeight(160);
    m_compareTable->setVisible(false);
    layout->addWidget(m_compareTable);

    m_confirmCheck = new QCheckBox(
        tr("I have compared the lines. Create the schedule with the generated Run lines."), this);
    m_confirmCheck->setVisible(false);
    connect(m_confirmCheck, &QCheckBox::toggled, this, &BScheduleDragResultDialog::updateApplyEnabled);
    layout->addWidget(m_confirmCheck);

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
    m_statusLabel->setTextFormat(Qt::PlainText);
    layout->addWidget(m_statusLabel);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0);
    m_progressBar->setVisible(false);
    layout->addWidget(m_progressBar);

    // --- Buttons ---
    layout->addSpacing(8);
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_applyButton = new QPushButton(tr("Create Schedule"), this);
    m_cancelButton = new QPushButton(tr("Close"), this);
    buttonLayout->addWidget(m_applyButton);
    buttonLayout->addWidget(m_cancelButton);
    layout->addLayout(buttonLayout);

    connect(m_applyButton, &QPushButton::clicked, this, &BScheduleDragResultDialog::onApply);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void BScheduleDragResultDialog::setDragResult(
    const BScheduleEntry &entry,
    int newHour, int newMinute,
    const QStringList &originalRuns,
    const QList<BScheduleGanttWidget::DependencyEdge> &deps,
    const BJobScheduleIndex *index)
{
    m_entry = entry;
    m_newHour = newHour;
    m_newMinute = newMinute;
    m_dependencies = deps;
    m_index = index;
    m_applied = false;
    m_busy = false;

    // Compare what would be written with what the Director has
    m_plan = BScheduleDragPlan::plan(originalRuns, entry, newHour, newMinute);

    generatePreview();
}

QString BScheduleDragResultDialog::newScheduleName() const
{
    return m_entry.scheduleName + "-modified";
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
        "<tr><td>%9:</td>")
        .arg(tr("Dragged"), jobLabel.toHtmlEscaped(),
             tr("Time"),
             QString::number(m_entry.hour).rightJustified(2, '0'),
             QString::number(m_entry.minute).rightJustified(2, '0'),
             QString::number(m_newHour).rightJustified(2, '0'),
             QString::number(m_newMinute).rightJustified(2, '0'),
             deltaStr)
        .arg(tr("Schedule"));
    summary += QString("<td>%1</td></tr>").arg(m_entry.scheduleName.toHtmlEscaped());

    if (!m_entry.jobName.isEmpty()) {
        summary += QString("<tr><td>%1:</td><td>%2</td></tr>")
            .arg(tr("Job"), m_entry.jobName.toHtmlEscaped());
    }
    if (!m_entry.client.isEmpty()) {
        summary += QString("<tr><td>%1:</td><td>%2</td></tr>")
            .arg(tr("Client"), m_entry.client.toHtmlEscaped());
    }
    summary += "</table>";
    m_summaryLabel->setText(summary);

    // --- Jobs that use the schedule: none of them is touched ---
    m_warningLabel->setVisible(false);
    if (m_index && !m_entry.scheduleName.isEmpty()) {
        const QList<BJobScheduleIndex::JobRef> jobs = m_index->jobsForSchedule(m_entry.scheduleName);
        if (!jobs.isEmpty()) {
            QStringList jobNames;
            for (const auto &ref : jobs) {
                jobNames.append(ref.jobName);
            }
            m_warningLabel->setText(
                tr("Jobs using schedule \"%1\": %2.\n"
                   "They all keep using \"%1\" with its current times.")
                    .arg(m_entry.scheduleName, jobNames.join(", ")));
            m_warningLabel->setVisible(true);
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

    showPlan();
}

void BScheduleDragResultDialog::showPlan()
{
    m_confirmCheck->setChecked(false);
    m_statusLabel->clear();
    bSetLabelState(m_statusLabel, "");

    const bool refused = m_plan.refused();
    const bool differs = (m_plan.verdict == Verdict::NeedsConfirmation);

    // --- What Apply does ---
    if (refused) {
        bSetLabelState(m_effectLabel, "error");
        m_effectLabel->setText(
            tr("No schedule can be created from this drag.\n%1\n"
               "Nothing was changed. To move the time, edit the Run line in the "
               "configuration of schedule \"%2\".")
                .arg(refusalText(), m_entry.scheduleName));
    } else {
        bSetLabelState(m_effectLabel, "");
        m_effectLabel->setText(
            tr("\"Create Schedule\" adds a new schedule named \"%1\" with the Run lines "
               "shown below. Schedule \"%2\" is not changed and no job is moved to the "
               "new schedule: every job keeps its current schedule and start time until "
               "you set Schedule = \"%1\" in the job's configuration yourself.")
                .arg(newScheduleName(), m_entry.scheduleName));
    }

    // --- Side by side, if more than the dragged time differs ---
    m_compareLabel->setVisible(differs);
    m_compareTable->setVisible(differs);
    m_confirmCheck->setVisible(differs);
    m_compareTable->setRowCount(0);
    if (differs) {
        m_compareLabel->setText(
            tr("The Run lines of the new schedule are generated, not copied. %n line(s) "
               "would differ from the original in more than the dragged time. Compare them "
               "before you create the schedule:", "", m_plan.differingLines.size()));

        m_compareTable->setRowCount(m_plan.originalRuns.size());
        for (int i = 0; i < m_plan.originalRuns.size(); ++i) {
            QString remark;
            if (m_plan.differingLines.contains(i)) {
                remark = tr("differs");
            } else if (i == m_plan.draggedLine) {
                remark = tr("time only");
            }
            auto *original = new QTableWidgetItem(m_plan.originalRuns[i]);
            auto *generated = new QTableWidgetItem(m_plan.generatedRuns.value(i));
            auto *remarkItem = new QTableWidgetItem(remark);
            if (m_plan.differingLines.contains(i)) {
                // Marked by weight, not by colour, so that it reads in both themes
                QFont bold = m_compareTable->font();
                bold.setBold(true);
                original->setFont(bold);
                generated->setFont(bold);
                remarkItem->setFont(bold);
            }
            m_compareTable->setItem(i, 0, original);
            m_compareTable->setItem(i, 1, generated);
            m_compareTable->setItem(i, 2, remarkItem);
        }
    }

    // --- Config + Command ---
    m_configEdit->setPlainText(buildConfigText());
    m_commandEdit->setPlainText(buildConfigureCommand());
    m_copyCommandButton->setEnabled(!refused);

    updateApplyEnabled();
}

QString BScheduleDragResultDialog::refusalText() const
{
    const int line = m_plan.reasonLine;
    const QString number = QString::number(line + 1);
    const QString text = m_plan.originalRuns.value(line);
    const QString token = m_plan.reasonToken;

    switch (m_plan.reason) {
    case Reason::NoRunLines:
        return tr("The Run lines of schedule \"%1\" are not known, so the result "
                  "cannot be checked against them.").arg(m_entry.scheduleName);
    case Reason::Override:
        return tr("Run line %1 (\"%2\") contains the override \"%3\". Lines with overrides "
                  "such as pool= or storage= cannot be generated without changing them.")
            .arg(number, text, token);
    case Reason::WeekOfMonth:
        return tr("Run line %1 (\"%2\") contains the week-of-month form \"%3\", which the "
                  "generated line would not keep.").arg(number, text, token);
    case Reason::DayRange:
        return tr("Run line %1 (\"%2\") contains the day range \"%3\", which the "
                  "generated line would not keep.").arg(number, text, token);
    case Reason::DayOfMonth:
        return tr("Run line %1 (\"%2\") contains the day of month \"%3\", which the "
                  "generated line would not keep.").arg(number, text, token);
    case Reason::NoTime:
        return tr("Run line %1 (\"%2\") has no time of the form \"at HH:MM\", so it cannot "
                  "be compared with a generated line.").arg(number, text);
    case Reason::DraggedLineNotFound:
        return tr("None of the Run lines of schedule \"%1\" matches the dragged entry.")
            .arg(m_entry.scheduleName);
    case Reason::DraggedLineAmbiguous:
        return tr("More than one Run line of schedule \"%1\" matches the dragged entry "
                  "(one of them is line %2: \"%3\"), so it cannot be determined which "
                  "line to change.").arg(m_entry.scheduleName, number, text);
    case Reason::None:
        break;
    }
    return QString();
}

void BScheduleDragResultDialog::updateApplyEnabled()
{
    bool enabled = !m_plan.refused() && !m_applied && !m_busy;
    if (m_plan.verdict == Verdict::NeedsConfirmation && !m_confirmCheck->isChecked()) {
        enabled = false;
    }
    m_applyButton->setEnabled(enabled);
}

void BScheduleDragResultDialog::setStatus(const char *state, const QString &text)
{
    bSetLabelState(m_statusLabel, state);
    m_statusLabel->setText(text);
}

QString BScheduleDragResultDialog::buildConfigText() const
{
    QString config;

    if (m_plan.refused()) {
        // Nothing is generated; show what the Director has, unchanged
        config += tr("# Run lines of schedule \"%1\" as they are (nothing is generated):")
                      .arg(m_entry.scheduleName) + "\n";
        for (const QString &run : m_plan.originalRuns) {
            config += QString("#   Run = %1\n").arg(run);
        }
        return config;
    }

    // The full Schedule { ... } block of the new schedule
    config += QString("Schedule {\n");
    config += QString("  Name = \"%1\"\n").arg(newScheduleName());

    for (int i = 0; i < m_plan.generatedRuns.size(); ++i) {
        if (i == m_plan.draggedLine) {
            config += QString("  Run = %1  # was %2:%3\n")
                .arg(m_plan.generatedRuns[i],
                     QString::number(m_entry.hour).rightJustified(2, '0'),
                     QString::number(m_entry.minute).rightJustified(2, '0'));
        } else {
            config += QString("  Run = %1\n").arg(m_plan.generatedRuns[i]);
        }
    }

    config += "}\n";
    return config;
}

QString BScheduleDragResultDialog::buildConfigureCommand() const
{
    if (m_plan.refused()) {
        return QString();
    }

    QStringList parts;
    parts.append(QString("configure add schedule name=\"%1\"").arg(newScheduleName()));

    for (const QString &run : m_plan.generatedRuns) {
        parts.append(QString("run=\"%1\"").arg(run));
    }

    return parts.join(" ");
}

void BScheduleDragResultDialog::onCopyConfig()
{
    QApplication::clipboard()->setText(m_configEdit->toPlainText());
    setStatus("", tr("Configuration copied to clipboard."));
}

void BScheduleDragResultDialog::onCopyCommand()
{
    QApplication::clipboard()->setText(m_commandEdit->toPlainText());
    setStatus("", tr("Command copied to clipboard."));
}

void BScheduleDragResultDialog::onApply()
{
    // The button is disabled in these cases; never send on a stale click
    if (m_plan.refused() || m_applied || m_busy) {
        return;
    }
    if (m_plan.verdict == Verdict::NeedsConfirmation && !m_confirmCheck->isChecked()) {
        return;
    }

    if (!m_director) {
        setStatus("error", tr("No director connection available."));
        return;
    }

    const QString cmd = buildConfigureCommand();
    if (cmd.isEmpty()) {
        return;
    }

    m_busy = true;
    updateApplyEnabled();
    m_confirmCheck->setEnabled(false);
    m_progressBar->setVisible(true);
    setStatus("", QString());

    // Connect to director response
    disconnectDirectorSignals();
    m_connections.append(
        connect(m_director, &BDirector::configureResult,
                this, &BScheduleDragResultDialog::onDirectorResponse));

    m_timeoutTimer->start();

    // Strip the leading "configure " since sendCommand adds the command type
    QString args = cmd.mid(QString("configure ").length());
    emit sendCommand(BDirector::Command::Configure, args);
}

void BScheduleDragResultDialog::onDirectorResponse(bool success, const QString &message)
{
    m_timeoutTimer->stop();
    disconnectDirectorSignals();
    m_progressBar->setVisible(false);
    m_busy = false;

    if (success) {
        m_applied = true;
        setStatus("success",
                  tr("Schedule \"%1\" was created. No job uses it yet: every job still runs "
                     "on its previous schedule at the previous time. To use the new time, set "
                     "Schedule = \"%1\" in the job's configuration.")
                      .arg(newScheduleName()));
    } else {
        m_confirmCheck->setEnabled(true);
        setStatus("error", tr("No schedule was created. Error: %1").arg(message.left(300)));
    }
    updateApplyEnabled();
}

void BScheduleDragResultDialog::onTimeout()
{
    disconnectDirectorSignals();
    m_progressBar->setVisible(false);
    m_busy = false;
    m_confirmCheck->setEnabled(true);
    setStatus("warning",
              tr("Timeout: No response from Director after 30 seconds. It is not known "
                 "whether schedule \"%1\" was created.").arg(newScheduleName()));
    updateApplyEnabled();
}

void BScheduleDragResultDialog::disconnectDirectorSignals()
{
    for (const auto &conn : m_connections) {
        disconnect(conn);
    }
    m_connections.clear();
}
