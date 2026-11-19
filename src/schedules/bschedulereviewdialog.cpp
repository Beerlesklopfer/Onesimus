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

QListWidget *rowList(const QList<BScheduleReview::Row> &rows, const QFont &mono, QWidget *parent)
{
    auto *list = new QListWidget(parent);
    list->setSelectionMode(QAbstractItemView::NoSelection);
    list->setFocusPolicy(Qt::StrongFocus);
    for (const BScheduleReview::Row &row : rows) {
        auto *item = new QListWidgetItem(BScheduleReviewDialog::stateWord(row.state) + row.text, list);
        QFont font = mono;
        font.setStrikeOut(row.state == BScheduleReview::Row::Removed);
        font.setBold(row.state == BScheduleReview::Row::Changed || row.state == BScheduleReview::Row::New);
        item->setFont(font);
        item->setData(Qt::UserRole, int(row.state));   // coloured when shown, after the theme
        item->setToolTip(row.text);
    }
    if (rows.isEmpty())
        new QListWidgetItem(BScheduleReviewDialog::tr("(no Run line)"), list);
    return list;
}

QPlainTextEdit *textView(const QString &text, const QFont &mono, QWidget *parent)
{
    auto *view = new QPlainTextEdit(parent);
    view->setReadOnly(true);
    view->setLineWrapMode(QPlainTextEdit::NoWrap);
    view->setFont(mono);
    // Line ends are shown as they are; a CRLF text stays one
    QString shown = text;
    shown.replace(QLatin1String("\r\n"), QLatin1String("\n"));
    view->setPlainText(shown);
    return view;
}

QLabel *wrapped(const QString &text, QWidget *parent)
{
    auto *label = new QLabel(text, parent);
    label->setWordWrap(true);
    label->setTextFormat(Qt::PlainText);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    return label;
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
    const QFont mono = BUiScale::scaled(font(), BUiScale::Normal, BUiScale::Monospace);
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
            m_old = rowList(review.oldRows, mono, oldBox);
            m_old->setAccessibleName(tr("Run lines now"));
            oldLayout->addWidget(m_old);
            runsLayout->addWidget(oldBox);
        }
        if (review.operation != BScheduleReview::Delete) {
            auto *newBox = new QGroupBox(tr("Run lines after writing"), runs);
            auto *newLayout = new QVBoxLayout(newBox);
            m_new = rowList(review.newRows, mono, newBox);
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
    if (!review.newText.isEmpty())
        m_texts->addTab(textView(review.newText, mono, m_texts), tr("Text after writing"));
    if (!review.oldText.isEmpty())
        m_texts->addTab(textView(review.oldText, mono, m_texts), tr("Text now"));
    m_texts->setMinimumHeight(BUiScale::px(120));
    layout->addWidget(m_texts, 2);

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
    m_write->setAutoDefault(false);
    m_write->setDefault(false);
    cancel->setDefault(true);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);

    resize(BUiScale::px(820), BUiScale::px(640));
    cancel->setFocus();
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

QString BScheduleReportDialog::problemLine(const std::optional<BConfig::Problem> &problem)
{
    if (!problem)
        return QString();
    QStringList where;
    if (!problem->file.isEmpty())
        where.append(tr("file %1").arg(problem->file));
    if (problem->line > 0)
        where.append(tr("line %1").arg(problem->line));
    if (problem->column > 0)
        where.append(tr("column %1").arg(problem->column));
    const QString place = where.join(QLatin1String(", "));
    if (place.isEmpty())
        return problem->text;
    return tr("%1: %2").arg(place, problem->text);
}

QString BScheduleReportDialog::problemText() const
{
    return m_problem ? m_problem->text() : QString();
}

BScheduleReportDialog::BScheduleReportDialog(const BScheduleReport &report, QWidget *parent)
    : QDialog(parent)
    , m_report(report)
{
    const QFont mono = BUiScale::scaled(font(), BUiScale::Normal, BUiScale::Monospace);
    setWindowTitle(report.title);
    auto *layout = new QVBoxLayout(this);

    auto *header = new QHBoxLayout();
    QStyle::StandardPixmap icon = QStyle::SP_MessageBoxWarning;
    if (report.kind == BScheduleReport::Written)
        icon = report.followups.isEmpty() ? QStyle::SP_MessageBoxInformation : QStyle::SP_MessageBoxWarning;
    else if (report.kind == BScheduleReport::Failed || report.kind == BScheduleReport::Invalid)
        icon = QStyle::SP_MessageBoxCritical;
    auto *iconLabel = new QLabel(this);
    const int iconSize = BUiScale::px(32);
    iconLabel->setPixmap(style()->standardIcon(icon).pixmap(iconSize, iconSize));
    iconLabel->setAlignment(Qt::AlignTop);
    header->addWidget(iconLabel);
    auto *texts = new QVBoxLayout();
    auto *title = new QLabel(report.title, this);
    title->setTextFormat(Qt::PlainText);
    BUiScale::bind(title, BUiScale::Large, BUiScale::Bold);
    texts->addWidget(title);
    texts->addWidget(wrapped(report.text, this));
    if (report.validatedOnly && report.kind != BScheduleReport::Written)
        texts->addWidget(wrapped(tr("This was found when the Director checked the change; nothing was "
                                    "sent to be written."), this));
    header->addLayout(texts, 1);
    layout->addLayout(header);

    if (!report.directorMessage.isEmpty())
        layout->addWidget(wrapped(tr("The Director says: %1").arg(report.directorMessage), this));

    if (report.problem) {
        auto *box = new QGroupBox(tr("Where the configuration is invalid"), this);
        auto *boxLayout = new QVBoxLayout(box);
        m_problem = wrapped(problemLine(report.problem), box);
        m_problem->setFont(mono);
        boxLayout->addWidget(m_problem);
        layout->addWidget(box);
    }

    if (!report.referencedBy.isEmpty()) {
        auto *box = new QGroupBox(tr("Still referenced by"), this);
        auto *boxLayout = new QVBoxLayout(box);
        auto *list = new QListWidget(box);
        list->setAccessibleName(tr("Still referenced by"));
        for (const QString &ref : report.referencedBy)
            new QListWidgetItem(ref, list);
        boxLayout->addWidget(list);
        layout->addWidget(box, 1);
    }

    if (!report.followups.isEmpty()) {
        auto *box = new QGroupBox(tr("Follow-up commands the Director names"), this);
        auto *boxLayout = new QVBoxLayout(box);
        auto *list = new QListWidget(box);
        list->setFont(mono);
        for (const QString &command : report.followups)
            new QListWidgetItem(command, list);
        boxLayout->addWidget(list);
        boxLayout->addWidget(wrapped(tr("They are not run by this application."), box));
        layout->addWidget(box, 1);
    }

    const bool offerReload = report.kind == BScheduleReport::Conflict
                          || report.kind == BScheduleReport::FileDiffers;
    if ((report.kind == BScheduleReport::Conflict || report.kind == BScheduleReport::FileDiffers)
        && (!report.currentText.isEmpty() || !report.ourText.isEmpty())) {
        auto *splitter = new QSplitter(Qt::Horizontal, this);
        auto addText = [&](const QString &label, const QString &text) {
            auto *box = new QGroupBox(label, splitter);
            auto *boxLayout = new QVBoxLayout(box);
            boxLayout->addWidget(textView(text, mono, box));
            splitter->addWidget(box);
        };
        addText(tr("On the Director now"), report.currentText);
        if (!report.ourText.isEmpty())
            addText(tr("What was to be written (not written)"), report.ourText);
        layout->addWidget(splitter, 3);
    }

    auto *buttons = new QDialogButtonBox(this);
    if (offerReload) {
        m_reloadButton = buttons->addButton(tr("&Reload the schedule"), QDialogButtonBox::AcceptRole);
        m_reloadButton->setToolTip(tr("Reads the schedule again from the Director; your changes are "
                                      "offered to be kept or discarded"));
        connect(m_reloadButton, &QPushButton::clicked, this, [this]() {
            m_reload = true;
            accept();
        });
    }
    QPushButton *close = buttons->addButton(QDialogButtonBox::Close);
    connect(close, &QPushButton::clicked, this, &QDialog::reject);
    close->setDefault(true);
    layout->addWidget(buttons);

    resize(BUiScale::px(offerReload ? 900 : 560), BUiScale::px(offerReload ? 560 : 300));
}

// ============================================================================
// BScheduleSaveDialogs
// ============================================================================

bool BScheduleSaveDialogs::confirm(const BScheduleReview &review)
{
    BScheduleReviewDialog dialog(review, m_parent);
    return dialog.exec() == QDialog::Accepted;
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
