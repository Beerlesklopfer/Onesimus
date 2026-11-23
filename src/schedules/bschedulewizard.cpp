// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bschedulewizard.cpp
 * @brief Wizard for creating new Schedule resources
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "schedules/bschedulewizard.h"
#include "schedules/bschedulegridview.h"
#include "schedules/bschedulepersistence.h"
#include "schedules/model/bschedulegridmodel.h"
#include "schedules/model/bscheduleresource.h"
#include "schedules/model/brunspec.h"
#include "jobs/blevelcolors.h"
#include "buiscale.h"
#include "blogging.h"
#include "blogredact.h"

#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QRegularExpression>
#include <QTimer>
#include <QVBoxLayout>

namespace {

/// The wizard a page belongs to (QWizardPage::wizard() is protected)
BScheduleWizard *scheduleWizard(const QWizardPage *page)
{
    for (QObject *o = page->parent(); o; o = o->parent()) {
        if (auto *wizard = qobject_cast<BScheduleWizard *>(o))
            return wizard;
    }
    return nullptr;
}

QPlainTextEdit *readOnlyText(QWidget *parent)
{
    auto *edit = new QPlainTextEdit(parent);
    edit->setReadOnly(true);
    edit->setLineWrapMode(QPlainTextEdit::NoWrap);
    BUiScale::bind(edit, BUiScale::Normal, BUiScale::Monospace);
    return edit;
}

} // namespace

// ============================================================================
// BScheduleNamePage
// ============================================================================

BScheduleNamePage::BScheduleNamePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("New Schedule"));
    setSubTitle(tr("Name the schedule. On the next page you paint its runs on a grid of weekdays and hours."));

    auto *layout = new QVBoxLayout(this);
    auto *form = new QFormLayout();

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setObjectName(QStringLiteral("scheduleWizardName"));
    m_nameEdit->setPlaceholderText(tr("e.g., WeeklyCycle"));
    m_nameEdit->setMaxLength(127);
    form->addRow(tr("&Name:"), m_nameEdit);

    m_descriptionEdit = new QLineEdit(this);
    m_descriptionEdit->setObjectName(QStringLiteral("scheduleWizardDescription"));
    m_descriptionEdit->setPlaceholderText(tr("e.g., Full Sunday, incremental daily"));
    form->addRow(tr("&Description:"), m_descriptionEdit);

    m_enabledCheck = new QCheckBox(tr("&Enabled"), this);
    m_enabledCheck->setToolTip(tr("A schedule that is not enabled starts no job."));
    m_enabledCheck->setChecked(true);
    form->addRow(QString(), m_enabledCheck);

    layout->addLayout(form);

    // Why the name cannot be taken; empty while it can
    m_problemLabel = new QLabel(this);
    m_problemLabel->setObjectName(QStringLiteral("scheduleWizardNameProblem"));
    m_problemLabel->setWordWrap(true);
    m_problemLabel->setStyleSheet(QStringLiteral("color: red;"));
    layout->addWidget(m_problemLabel);
    layout->addStretch();

    connect(m_nameEdit, &QLineEdit::textChanged, this, [this]() {
        updateProblem();
        emit completeChanged();
    });
}

QString BScheduleNamePage::nameProblem() const
{
    const BScheduleWizard *wiz = scheduleWizard(this);
    return BScheduleWizard::nameProblem(scheduleName(), wiz ? wiz->existingScheduleNames() : QStringList());
}

void BScheduleNamePage::updateProblem()
{
    // An empty field is not an error yet: the Next button says enough
    m_problemLabel->setText(scheduleName().isEmpty() ? QString() : nameProblem());
}

bool BScheduleNamePage::isComplete() const
{
    return nameProblem().isEmpty();
}

QString BScheduleNamePage::scheduleName() const { return m_nameEdit->text().trimmed(); }
QString BScheduleNamePage::scheduleDescription() const { return m_descriptionEdit->text().trimmed(); }
bool BScheduleNamePage::enabled() const { return m_enabledCheck->isChecked(); }

// ============================================================================
// BScheduleGridPage
// ============================================================================

BScheduleGridPage::BScheduleGridPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Runs"));
    setSubTitle(tr("Paint the runs on the grid: choose a level in the palette and drag over the hours. "
                   "Calendar rules (days of the month, weeks of the year) are added below the grid."));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    m_view = new BScheduleGridView(this);
    m_view->setNewScheduleMode();
    m_view->setLevelColorSource(&BLevelColors::getLevelColor);
    layout->addWidget(m_view, 1);

    m_hint = new QLabel(this);
    m_hint->setObjectName(QStringLiteral("scheduleWizardGridHint"));
    m_hint->setWordWrap(true);
    layout->addWidget(m_hint);

    connect(m_view->model(), &BScheduleGridModel::runLinesChanged, this, [this]() {
        updateHint();
        emit completeChanged();
    });
    connect(m_view->model(), &BScheduleGridModel::scheduleReset, this, [this]() {
        updateHint();
        emit completeChanged();
    });
}

void BScheduleGridPage::initializePage()
{
    // The grid starts empty once; going back to the name and on again keeps
    // what was painted. The name is not shown here, so it need not follow.
    if (!m_started) {
        BSchedule schedule;
        if (const BScheduleWizard *wiz = scheduleWizard(this))
            schedule.name = wiz->scheduleName();
        m_view->setSchedule(schedule);
        m_started = true;
    }
    updateHint();
}

bool BScheduleGridPage::isComplete() const
{
    const BScheduleWizard *wiz = scheduleWizard(this);
    return wiz && wiz->runTexts(nullptr);
}

void BScheduleGridPage::updateHint()
{
    const BScheduleWizard *wiz = scheduleWizard(this);
    QString error;
    if (!wiz || wiz->runTexts(nullptr, &error)) {
        m_hint->clear();
        m_hint->setVisible(false);
        return;
    }
    m_hint->setText(error);
    m_hint->setVisible(true);
}

// ============================================================================
// BScheduleReviewPage
// ============================================================================

BScheduleReviewPage::BScheduleReviewPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Review"));
    setSubTitle(tr("These are the Run lines and the resource that will be written. Finish creates the schedule."));
    setCommitPage(false);

    auto *layout = new QVBoxLayout(this);

    auto *runsLabel = new QLabel(tr("Run lines:"), this);
    runsLabel->setStyleSheet(QStringLiteral("font-weight: bold;"));
    layout->addWidget(runsLabel);
    m_runsEdit = readOnlyText(this);
    m_runsEdit->setObjectName(QStringLiteral("scheduleWizardRuns"));
    layout->addWidget(m_runsEdit, 1);

    auto *configLabel = new QLabel(tr("Resource:"), this);
    configLabel->setStyleSheet(QStringLiteral("font-weight: bold;"));
    layout->addWidget(configLabel);
    m_configEdit = readOnlyText(this);
    m_configEdit->setObjectName(QStringLiteral("scheduleWizardConfig"));
    layout->addWidget(m_configEdit, 1);

    // Without CreateResource: the "configure" command that is sent
    m_commandLabel = new QLabel(tr("Director command:"), this);
    m_commandLabel->setStyleSheet(QStringLiteral("font-weight: bold;"));
    layout->addWidget(m_commandLabel);
    m_commandEdit = readOnlyText(this);
    m_commandEdit->setObjectName(QStringLiteral("scheduleWizardCommand"));
    m_commandEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    BUiScale::track(m_commandEdit, [this]() { m_commandEdit->setMaximumHeight(BUiScale::px(80)); });
    layout->addWidget(m_commandEdit);

    auto *buttons = new QHBoxLayout();
    m_copyButton = new QPushButton(tr("Copy Resource"), this);
    m_copyButton->setToolTip(tr("Copies the resource text to the clipboard"));
    connect(m_copyButton, &QPushButton::clicked, this, [this]() {
        QApplication::clipboard()->setText(m_configEdit->toPlainText());
        setStatus(tr("Resource copied to clipboard."));
    });
    buttons->addWidget(m_copyButton);
    buttons->addStretch();
    layout->addLayout(buttons);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName(QStringLiteral("scheduleWizardStatus"));
    m_statusLabel->setWordWrap(true);
    layout->addWidget(m_statusLabel);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0);
    m_progressBar->setVisible(false);
    layout->addWidget(m_progressBar);

    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    m_timeoutTimer->setInterval(30000);
    connect(m_timeoutTimer, &QTimer::timeout, this, &BScheduleReviewPage::onConfigureTimeout);
}

void BScheduleReviewPage::initializePage()
{
    endWaiting();
    m_executed = false;
    setStatus(QString());

    BScheduleWizard *wiz = scheduleWizard(this);
    if (!wiz)
        return;

    QStringList runs;
    QString error;
    const bool writable = wiz->runTexts(&runs, &error);
    QStringList shown;
    for (const QString &run : std::as_const(runs))
        shown.append(QStringLiteral("Run = ") + run);
    m_runsEdit->setPlainText(shown.join(QLatin1Char('\n')));
    m_configEdit->setPlainText(writable ? wiz->generateConfigText() : QString());

    const bool viaCreate = wiz->persistence() != nullptr;
    m_commandLabel->setVisible(!viaCreate);
    m_commandEdit->setVisible(!viaCreate);
    m_commandEdit->setPlainText(writable && !viaCreate
                                    ? QStringLiteral("configure ") + wiz->generateConfigureCommand()
                                    : QString());
    m_copyButton->setEnabled(writable);
    if (!writable)
        setStatus(tr("This schedule cannot be written. %1").arg(error), QStringLiteral("color: red;"));
}

QString BScheduleReviewPage::runLinesText() const { return m_runsEdit->toPlainText(); }
QString BScheduleReviewPage::configText() const { return m_configEdit->toPlainText(); }
QString BScheduleReviewPage::commandText() const { return m_commandEdit->toPlainText(); }
QString BScheduleReviewPage::statusText() const { return m_statusLabel->text(); }

void BScheduleReviewPage::setStatus(const QString &text, const QString &style)
{
    m_statusLabel->setStyleSheet(style);
    m_statusLabel->setText(text);
}

bool BScheduleReviewPage::validatePage()
{
    if (m_waiting) return false;    // the answer to what was sent decides
    if (m_executed) return true;

    BScheduleWizard *wiz = scheduleWizard(this);
    QString runError;
    if (!wiz || !wiz->runTexts(nullptr, &runError)) {
        QMessageBox::warning(this, tr("Schedule cannot be written"),
            tr("%1\n\nGo back and paint at least one run, or cancel the wizard.").arg(runError));
        return false;
    }

    if (wiz->persistence()) {
        // CreateResource: the Director checks the text, the review asks
        executeCreateResource();
        return false;
    }

    if (!wiz->director() || !wiz->director()->isConnected())
        return true;

    int ret = QMessageBox::question(this, tr("Deploy to Director"),
        tr("Send 'configure add' to the Director?\n\n"
           "Click Yes to execute, No to close without deploying."),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (ret == QMessageBox::Cancel) return false;
    if (ret == QMessageBox::No) return true;

    executeConfigureCommand();
    return false;
}

void BScheduleReviewPage::executeConfigureCommand()
{
    BScheduleWizard *wiz = scheduleWizard(this);
    if (!wiz || !wiz->director() || m_waiting)
        return;

    const QString cmd = wiz->generateConfigureCommand();
    if (cmd.isEmpty())
        return;     // a Run line cannot be written; validatePage() said why

    disconnectDirectorSignals();
    m_waiting = true;
    m_progressBar->setVisible(true);
    setStatus(tr("Sending configure command..."));

    // configureResult() is the Director layer's verdict on the reply, in
    // API mode (JSON) and in text mode alike
    m_connections.append(connect(wiz->director(), &BDirector::configureResult,
                                 this, &BScheduleReviewPage::onConfigureResult));
    m_connections.append(connect(wiz->director(), &BDirector::disconnected,
                                 this, &BScheduleReviewPage::onDirectorDisconnected));
    m_timeoutTimer->start();

    BLOG_DEBUG() << "BScheduleWizard: Sending configure" << bRedactCommand(cmd).left(200);
    wiz->director()->doSend(BDirector::Command::Configure, cmd);
}

void BScheduleReviewPage::executeCreateResource()
{
    BScheduleWizard *wiz = scheduleWizard(this);
    if (!wiz || !wiz->persistence() || m_waiting)
        return;

    BSchedulePersistence *persistence = wiz->persistence();
    if (persistence->isBusy()) {
        setStatus(tr("Another change of a schedule is being written. Try again when it is done."),
                  QStringLiteral("color: orange;"));
        return;
    }

    disconnectDirectorSignals();
    setStatus(tr("The Director checks the schedule..."));
    m_connections.append(connect(persistence, &BSchedulePersistence::finished, this,
                                 [this](const BScheduleReport &report) {
        endWaiting();
        if (report.kind == BScheduleReport::Written) {
            // The flow has told the user already (its report); the wizard
            // has nothing more to do
            m_executed = true;
            setStatus(tr("Schedule created."), QStringLiteral("color: green; font-weight: bold;"));
            if (QWizard *wiz = wizard())
                wiz->accept();
        } else if (report.kind == BScheduleReport::Cancelled) {
            setStatus(tr("Nothing was written."));
        } else {
            setStatus(tr("%1: %2").arg(report.title, report.text).left(600), QStringLiteral("color: red;"));
        }
    }));
    m_waiting = true;
    m_progressBar->setVisible(true);
    if (!persistence->createSchedule(wiz->scheduleName(), wiz->generateConfigText())) {
        // Refused at once; the report (if any) came through finished()
        if (m_waiting)
            endWaiting();
    }
}

void BScheduleReviewPage::endWaiting()
{
    m_timeoutTimer->stop();
    disconnectDirectorSignals();
    m_progressBar->setVisible(false);
    m_waiting = false;
}

void BScheduleReviewPage::onConfigureResult(bool success, const QString &message)
{
    if (!m_waiting) return;
    endWaiting();

    if (success) {
        m_executed = true;
        setStatus(tr("Schedule created successfully! Click Finish to close."),
                  QStringLiteral("color: green; font-weight: bold;"));
    } else {
        setStatus(tr("Error: %1").arg(message.left(500)), QStringLiteral("color: red;"));
    }
}

void BScheduleReviewPage::onDirectorDisconnected()
{
    if (!m_waiting) return;
    endWaiting();
    setStatus(tr("The connection to the Director was lost before it answered. "
                 "Reconnect and check with 'show schedules' whether the schedule exists."),
              QStringLiteral("color: red;"));
}

void BScheduleReviewPage::onConfigureTimeout()
{
    endWaiting();
    setStatus(tr("Timeout waiting for Director response. "
                 "The schedule may have been created — check with 'show schedules'."),
              QStringLiteral("color: orange;"));
    m_executed = true;
}

void BScheduleReviewPage::disconnectDirectorSignals()
{
    for (const auto &conn : std::as_const(m_connections))
        QObject::disconnect(conn);
    m_connections.clear();
}

// ============================================================================
// BScheduleWizard
// ============================================================================

BScheduleWizard::BScheduleWizard(BDirector *director, QWidget *parent)
    : QWizard(parent)
    , m_director(director)
{
    init();
}

BScheduleWizard::~BScheduleWizard()
{
}

void BScheduleWizard::init()
{
    setWindowTitle(tr("Add Schedule"));
    // The grid needs room: seven rows of 24 hours, the palette and the lists
    BUiScale::track(this, [this]() { setMinimumSize(BUiScale::px(900), BUiScale::px(700)); });

    setPage(Page_Name, new BScheduleNamePage(this));
    setPage(Page_Grid, new BScheduleGridPage(this));
    setPage(Page_Review, new BScheduleReviewPage(this));
    setStartId(Page_Name);
}

BScheduleNamePage *BScheduleWizard::namePage() const
{
    return qobject_cast<BScheduleNamePage *>(page(Page_Name));
}

BScheduleGridPage *BScheduleWizard::gridPage() const
{
    return qobject_cast<BScheduleGridPage *>(page(Page_Grid));
}

BScheduleReviewPage *BScheduleWizard::reviewPage() const
{
    return qobject_cast<BScheduleReviewPage *>(page(Page_Review));
}

BScheduleGridView *BScheduleWizard::gridView() const
{
    BScheduleGridPage *grid = gridPage();
    return grid ? grid->gridView() : nullptr;
}

QString BScheduleWizard::scheduleName() const
{
    BScheduleNamePage *name = namePage();
    return name ? name->scheduleName() : QString();
}

QString BScheduleWizard::scheduleDescription() const
{
    BScheduleNamePage *name = namePage();
    return name ? name->scheduleDescription() : QString();
}

bool BScheduleWizard::scheduleEnabled() const
{
    BScheduleNamePage *name = namePage();
    return name ? name->enabled() : true;
}

QString BScheduleWizard::nameProblem(const QString &name, const QStringList &existing)
{
    if (name.isEmpty())
        return tr("The schedule needs a name.");
    if (name.size() > 127)
        return tr("The name is too long: at most 127 characters.");
    // What Bareos accepts in a resource name
    static const QRegularExpression allowed(QStringLiteral("^[A-Za-z0-9_.:\\- ]+$"));
    if (!allowed.match(name).hasMatch())
        return tr("The name may contain letters, digits, blanks and _ . : - only.");
    if (existing.contains(name))
        return tr("A schedule named %1 exists already.").arg(name);
    return QString();
}

bool BScheduleWizard::runTexts(QStringList *texts, QString *error) const
{
    if (texts)
        texts->clear();
    const BScheduleGridView *view = gridView();
    if (!view || !view->hasSchedule()) {
        if (error)
            *error = tr("The schedule has no runs yet.");
        return false;
    }
    // Every line of a new schedule is new: each is written by BRunWriter
    // and read back (the same lines the tab's "Save..." would write)
    QList<BScheduleResource::Line> lines;
    if (!BSchedulePersistence::linesFromModel(*view->model(), &lines, error))
        return false;
    if (lines.isEmpty()) {
        if (error)
            *error = tr("The schedule has no runs yet. Paint at least one run on the grid.");
        return false;
    }
    if (texts) {
        for (const BScheduleResource::Line &line : std::as_const(lines))
            texts->append(line.text);
    }
    return true;
}

QString BScheduleWizard::generateConfigText() const
{
    QStringList runs;
    if (!runTexts(&runs))
        return QString();
    return BScheduleResource::createText(scheduleName(), scheduleDescription(), scheduleEnabled(), runs);
}

QString BScheduleWizard::generateConfigureCommand() const
{
    // No command at all if one Run line cannot be written
    QStringList runs;
    if (!runTexts(&runs))
        return QString();

    QStringList parts;
    parts.append(QStringLiteral("add schedule"));
    parts.append(QStringLiteral("name=\"%1\"").arg(scheduleName()));
    if (!scheduleDescription().isEmpty())
        parts.append(QStringLiteral("description=\"%1\"").arg(scheduleDescription()));
    if (!scheduleEnabled())
        parts.append(QStringLiteral("enabled=no"));
    // Run lines may contain quoted names (Pool="Full Pool")
    for (const QString &run : std::as_const(runs))
        parts.append(QStringLiteral("run=") + BRunWriter::quotedForCommand(run));
    return parts.join(QLatin1Char(' '));
}
