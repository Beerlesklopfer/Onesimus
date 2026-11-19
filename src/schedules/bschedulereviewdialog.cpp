/**
 * @file bschedulereviewdialog.cpp
 * @brief The dialogs of writing a schedule: the review before, the report after
 */

#include "schedules/bschedulereviewdialog.h"
#include "buiscale.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QStyle>
#include <QTabWidget>
#include <QVBoxLayout>

namespace {

/// Colours that read on light and dark backgrounds; the word in front of a
/// line carries the meaning, the colour only helps
QColor stateColor(BScheduleReview::Row::State state, const QPalette &palette)
{
    const bool dark = palette.color(QPalette::Base).lightness() < 128;
    switch (state) {
    case BScheduleReview::Row::Changed:
        return dark ? QColor(0xf0, 0xc0, 0x60) : QColor(0x9a, 0x5b, 0x00);
    case BScheduleReview::Row::New:
        return dark ? QColor(0x7f, 0xd8, 0x8a) : QColor(0x1b, 0x6e, 0x2a);
    case BScheduleReview::Row::Removed:
        return dark ? QColor(0xff, 0x8a, 0x80) : QColor(0xa3, 0x1f, 0x16);
    case BScheduleReview::Row::Untouched:
        break;
    }
    return palette.color(QPalette::Text);
}

QListWidget *rowList(const QList<BScheduleReview::Row> &rows, QWidget *parent)
{
    auto *list = new QListWidget(parent);
    list->setSelectionMode(QAbstractItemView::NoSelection);
    list->setFocusPolicy(Qt::StrongFocus);
    BUiScale::bind(list, BUiScale::Normal, BUiScale::Monospace);
    for (const BScheduleReview::Row &row : rows) {
        auto *item = new QListWidgetItem(BScheduleReviewDialog::stateWord(row.state) + row.text, list);
        item->setData(Qt::UserRole, int(row.state));   // coloured when shown, after the theme
        item->setToolTip(row.text);
    }
    if (rows.isEmpty())
        new QListWidgetItem(BScheduleReviewDialog::tr("(no Run line)"), list);
    // An item's font is its own: given again after a change of the zoom
    BUiScale::track(list, [list]() {
        const QFont mono = BUiScale::instance().font(BUiScale::Normal, BUiScale::Monospace);
        for (int i = 0; i < list->count(); ++i) {
            QListWidgetItem *item = list->item(i);
            const QVariant state = item->data(Qt::UserRole);
            if (!state.isValid()) continue;
            QFont font = mono;
            font.setStrikeOut(state.toInt() == BScheduleReview::Row::Removed);
            font.setBold(state.toInt() == BScheduleReview::Row::Changed
                         || state.toInt() == BScheduleReview::Row::New);
            item->setFont(font);
        }
    });
    return list;
}

QPlainTextEdit *textView(const QString &text, QWidget *parent)
{
    return BResourceReviewDialog::textView(text, parent);
}

QLabel *wrapped(const QString &text, QWidget *parent)
{
    return BResourceReviewDialog::wrapped(text, parent);
}

} // namespace

// ============================================================================
// Review
// ============================================================================

QString BScheduleReviewDialog::stateWord(BScheduleReview::Row::State state)
{
    switch (state) {
    case BScheduleReview::Row::Changed: return tr("changed: ");
    case BScheduleReview::Row::New: return tr("new: ");
    case BScheduleReview::Row::Removed: return tr("removed: ");
    case BScheduleReview::Row::Untouched: break;
    }
    return QString();
}

BScheduleReviewDialog::BScheduleReviewDialog(const BScheduleReview &review, QWidget *parent)
    : QDialog(parent)
{
    auto *layout = new QVBoxLayout(this);

    QString heading;
    QString question;
    switch (review.operation) {
    case BScheduleReview::Update:
        setWindowTitle(tr("Change schedule %1").arg(review.scheduleName));
        heading = tr("Change schedule %1 on the Director?").arg(review.scheduleName);
        question = tr("The Director checked the new configuration and accepts it. Nothing is written "
                      "until you choose \"Write to the Director\".");
        break;
    case BScheduleReview::Create:
        setWindowTitle(tr("Create schedule %1").arg(review.scheduleName));
        heading = tr("Create schedule %1 on the Director?").arg(review.scheduleName);
        question = tr("The Director checked the new schedule and accepts it. Nothing is written until "
                      "you choose \"Write to the Director\".");
        break;
    case BScheduleReview::Delete:
        setWindowTitle(tr("Delete schedule %1").arg(review.scheduleName));
        heading = tr("Delete schedule %1 on the Director?").arg(review.scheduleName);
        question = tr("The Director checked that nothing refers to the schedule. Nothing is deleted "
                      "until you choose \"Delete on the Director\".");
        break;
    }
    auto *title = new QLabel(heading, this);
    title->setTextFormat(Qt::PlainText);
    BUiScale::bind(title, BUiScale::Large, BUiScale::Bold);
    layout->addWidget(title);
    if (!review.note.isEmpty())
        layout->addWidget(wrapped(review.note, this));
    layout->addWidget(wrapped(question, this));

    // --- Run lines, old and new ---
    if (review.operation != BScheduleReview::Create || !review.newRows.isEmpty()) {
        auto *runs = new QWidget(this);
        auto *runsLayout = new QHBoxLayout(runs);
        runsLayout->setContentsMargins(0, 0, 0, 0);
        if (review.operation != BScheduleReview::Create) {
            auto *oldBox = new QGroupBox(tr("Run lines now (in the file)"), runs);
            auto *oldLayout = new QVBoxLayout(oldBox);
            m_old = rowList(review.oldRows, oldBox);
            m_old->setAccessibleName(tr("Run lines now"));
            oldLayout->addWidget(m_old);
            runsLayout->addWidget(oldBox);
        }
        if (review.operation != BScheduleReview::Delete) {
            auto *newBox = new QGroupBox(tr("Run lines after writing"), runs);
            auto *newLayout = new QVBoxLayout(newBox);
            m_new = rowList(review.newRows, newBox);
            m_new->setAccessibleName(tr("Run lines after writing"));
            newLayout->addWidget(m_new);
            runsLayout->addWidget(newBox);
        }
        layout->addWidget(runs, 2);
    }

    // --- Who is affected ---
    auto *usedBox = new QGroupBox(tr("Jobs that use this schedule"), this);
    auto *usedLayout = new QVBoxLayout(usedBox);
    if (review.operation == BScheduleReview::Update && !review.jobs.isEmpty()) {
        usedLayout->addWidget(wrapped(tr("The change applies to every one of these %n job(s), not only "
                                         "to the one on display. To move a single job, copy the schedule "
                                         "and assign the copy to that job instead.", "", int(review.jobs.size())),
                                      usedBox));
    }
    m_jobs = new QListWidget(usedBox);
    m_jobs->setAccessibleName(tr("Jobs that use this schedule"));
    m_jobs->setSelectionMode(QAbstractItemView::NoSelection);
    for (const QString &job : review.jobs)
        new QListWidgetItem(job, m_jobs);
    if (review.jobs.isEmpty())
        new QListWidgetItem(tr("(no job of the Director uses it, as far as this application knows)"), m_jobs);
    m_jobs->setMaximumHeight(BUiScale::px(110));
    usedLayout->addWidget(m_jobs);
    if (!review.referencedBy.isEmpty()) {
        usedLayout->addWidget(wrapped(tr("The Director names as referencing it: %1")
                                          .arg(review.referencedBy.join(QLatin1String(", "))),
                                      usedBox));
    }
    layout->addWidget(usedBox, 1);

    // --- Resource text ---
    m_texts = new QTabWidget(this);
    if (!review.newText.isEmpty()) {
        m_edit = textView(review.newText, m_texts);
        m_edit->setReadOnly(false);
        m_edit->setTabChangesFocus(true);
        m_edit->setAccessibleName(tr("Text after writing, editable"));
        m_edit->setToolTip(tr("This text can be edited. An edited text is checked by the Director and "
                              "shown again before anything is written."));
        m_checkedText = m_edit->toPlainText();
        m_texts->addTab(m_edit, tr("Text after writing"));
    }
    if (!review.oldText.isEmpty())
        m_texts->addTab(textView(review.oldText, m_texts), tr("Text now"));
    m_texts->setMinimumHeight(BUiScale::px(120));
    layout->addWidget(m_texts, 2);
    if (m_edit) {
        m_editHint = wrapped(tr("The text was edited. \"Check with the Director\" sends it for a check "
                                "only; the review then shows it again, and only \"Write to the Director\" "
                                "there writes it."), this);
        m_editHint->setVisible(false);
        layout->addWidget(m_editHint);
    }

    QStringList notes;
    if (!review.file.isEmpty()) {
        notes.append(review.sharedFile
                         ? tr("File on the Director: %1 (it holds other resources as well; only this "
                              "resource is replaced).").arg(review.file)
                         : tr("File on the Director: %1.").arg(review.file));
    }
    if (review.operation != BScheduleReview::Delete) {
        notes.append(tr("The Director reloads its configuration after writing. Runs it has already "
                        "queued may still start at the old time."));
    }
    layout->addWidget(wrapped(notes.join(QLatin1Char(' ')), this));

    auto *buttons = new QDialogButtonBox(this);
    m_write = buttons->addButton(review.operation == BScheduleReview::Delete ? tr("&Delete on the Director")
                                                                             : tr("&Write to the Director"),
                                 QDialogButtonBox::AcceptRole);
    QPushButton *cancel = buttons->addButton(QDialogButtonBox::Cancel);
    m_writeLabel = m_write->text();
    m_write->setAutoDefault(false);
    m_write->setDefault(false);
    cancel->setDefault(true);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);

    if (m_edit)
        connect(m_edit, &QPlainTextEdit::textChanged, this, &BScheduleReviewDialog::textEdited);

    resize(BUiScale::px(820), BUiScale::px(640));
    cancel->setFocus();
}

bool BScheduleReviewDialog::isEdited() const
{
    return m_edit && m_edit->toPlainText() != m_checkedText;
}

QString BScheduleReviewDialog::editedText() const
{
    return m_edit ? m_edit->toPlainText() : QString();
}

void BScheduleReviewDialog::textEdited()
{
    const bool edited = isEdited();
    m_write->setText(edited ? tr("&Check with the Director") : m_writeLabel);
    m_editHint->setVisible(edited);
    // The lists show what was checked, not what is typed
    for (QListWidget *list : {m_old, m_new}) {
        if (list)
            list->setEnabled(!edited);
    }
}

void BScheduleReviewDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    // The theme has set the palettes of the lists by now
    for (QListWidget *list : {m_old, m_new}) {
        if (!list)
            continue;
        for (int i = 0; i < list->count(); ++i) {
            QListWidgetItem *item = list->item(i);
            const QVariant state = item->data(Qt::UserRole);
            if (state.isValid())
                item->setForeground(stateColor(BScheduleReview::Row::State(state.toInt()), list->palette()));
        }
    }
}

// ============================================================================
// Report
// ============================================================================

BResourceReport BScheduleReportDialog::toResourceReport(const BScheduleReport &report)
{
    BResourceReport out;
    out.kind = BResourceReport::Kind(int(report.kind));
    out.operation = BResourceReview::Operation(int(report.operation));
    out.type = BConfig::ResourceType::Schedule;
    out.name = report.scheduleName;
    out.title = report.title;
    out.text = report.text;
    out.directorMessage = report.directorMessage;
    out.problem = report.problem;
    out.ourText = report.ourText;
    out.currentText = report.currentText;
    out.referencedBy = report.referencedBy;
    out.followups = report.followups;
    out.reloaded = report.reloaded;
    out.validatedOnly = report.validatedOnly;
    return out;
}

BScheduleReportDialog::BScheduleReportDialog(const BScheduleReport &report, QWidget *parent)
    : BResourceReportDialog(toResourceReport(report), parent)
{
}

// ============================================================================
// BScheduleSaveDialogs
// ============================================================================

bool BScheduleSaveDialogs::confirm(const BScheduleReview &review)
{
    BScheduleReviewDialog dialog(review, m_parent);
    return dialog.exec() == QDialog::Accepted;
}

BScheduleSaveUi::Decision BScheduleSaveDialogs::decide(const BScheduleReview &review, QString *edited)
{
    BScheduleReviewDialog dialog(review, m_parent);
    if (dialog.exec() != QDialog::Accepted)
        return Cancel;
    if (!dialog.isEdited())
        return Write;
    if (edited)
        *edited = dialog.editedText();
    return Recheck;
}

bool BScheduleSaveDialogs::needsDialog(const BScheduleReport &report)
{
    switch (report.kind) {
    case BScheduleReport::Cancelled:
        return false;
    case BScheduleReport::Written:
        return !report.followups.isEmpty() || !report.reloaded;
    default:
        return true;
    }
}

BScheduleSaveUi::Answer BScheduleSaveDialogs::report(const BScheduleReport &report)
{
    if (!needsDialog(report))
        return Close;
    BScheduleReportDialog dialog(report, m_parent);
    dialog.exec();
    return dialog.reloadChosen() ? Reload : Close;
}
