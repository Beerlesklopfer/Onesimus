// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bresourcereviewdialog.cpp
 * @brief The dialogs of writing a resource: the review before, the report after
 */

#include "config/bresourcereviewdialog.h"

#include "buiscale.h"
#include "config/bresourcetext.h"

#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QStyle>
#include <QTextBlock>
#include <QTextCursor>
#include <QVBoxLayout>

// ============================================================================
// Shared helpers
// ============================================================================

QPlainTextEdit *BResourceReviewDialog::textView(const QString &text, QWidget *parent)
{
    auto *view = new QPlainTextEdit(parent);
    view->setReadOnly(true);
    view->setLineWrapMode(QPlainTextEdit::NoWrap);
    BUiScale::bind(view, BUiScale::Normal, BUiScale::Monospace);
    // Line ends are shown as lines; a CRLF text stays one where it is sent
    QString shown = text;
    shown.replace(QLatin1String("\r\n"), QLatin1String("\n"));
    view->setPlainText(shown);
    return view;
}

QLabel *BResourceReviewDialog::wrapped(const QString &text, QWidget *parent)
{
    auto *label = new QLabel(text, parent);
    label->setWordWrap(true);
    label->setTextFormat(Qt::PlainText);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    return label;
}

// ============================================================================
// Review
// ============================================================================

QString BResourceReviewDialog::diffText(const QString &oldText, const QString &newText)
{
    QStringList lines;
    for (const BResourceText::DiffLine &line : BResourceText::diff(oldText, newText)) {
        switch (line.kind) {
        case BResourceText::DiffLine::Same: lines.append(QStringLiteral("  ") + line.text); break;
        case BResourceText::DiffLine::Removed: lines.append(QStringLiteral("- ") + line.text); break;
        case BResourceText::DiffLine::Added: lines.append(QStringLiteral("+ ") + line.text); break;
        }
    }
    return lines.join(QLatin1Char('\n'));
}

BResourceReviewDialog::BResourceReviewDialog(const BResourceReview &review, QWidget *parent)
    : QDialog(parent)
    , m_oldText(review.oldText)
    , m_newText(review.newText)
{
    auto *layout = new QVBoxLayout(this);
    const QString kind = BConfig::typeName(review.type);
    const QString thing = BResourcePersistence::noun(review.type);

    QString heading;
    QString question;
    switch (review.operation) {
    case BResourceReview::Update:
        setWindowTitle(tr("Change %1 %2").arg(kind, review.name));
        heading = tr("Change %1 %2 on the Director?").arg(kind, review.name);
        question = tr("The Director checked the new configuration and accepts it. Nothing is written "
                      "until you choose \"Write to the Director\".");
        break;
    case BResourceReview::Create:
        setWindowTitle(tr("Create %1 %2").arg(kind, review.name));
        heading = tr("Create %1 %2 on the Director?").arg(kind, review.name);
        question = tr("The Director checked the new %1 and accepts it. Nothing is written until you "
                      "choose \"Write to the Director\".").arg(thing);
        break;
    case BResourceReview::Delete:
        setWindowTitle(tr("Delete %1 %2").arg(kind, review.name));
        heading = tr("Delete %1 %2 on the Director?").arg(kind, review.name);
        question = tr("The Director checked that nothing refers to the %1. Nothing is deleted until you "
                      "choose \"Delete on the Director\".").arg(thing);
        break;
    }
    auto *title = new QLabel(heading, this);
    title->setTextFormat(Qt::PlainText);
    BUiScale::bind(title, BUiScale::Large, BUiScale::Bold);
    layout->addWidget(title);
    if (!review.note.isEmpty())
        layout->addWidget(wrapped(review.note, this));
    layout->addWidget(wrapped(question, this));

    if (!review.changes.isEmpty()) {
        auto *box = new QGroupBox(tr("Changed directives"), this);
        auto *boxLayout = new QVBoxLayout(box);
        auto *list = new QListWidget(box);
        list->setAccessibleName(tr("Changed directives"));
        list->setSelectionMode(QAbstractItemView::NoSelection);
        BUiScale::bind(list, BUiScale::Normal, BUiScale::Monospace);
        for (const QString &change : review.changes)
            new QListWidgetItem(change, list);
        list->setMaximumHeight(BUiScale::px(110));
        boxLayout->addWidget(list);
        layout->addWidget(box);
    }

    // --- The text ---
    auto *textBox = new QGroupBox(review.operation == BResourceReview::Update ? tr("Text before and after "
                                                                                   "(- removed, + added)")
                                  : review.operation == BResourceReview::Create ? tr("Text of the new resource")
                                                                                : tr("Text that is deleted"),
                                  this);
    auto *textLayout = new QVBoxLayout(textBox);
    if (review.operation == BResourceReview::Update) {
        m_diff = textView(diffText(review.oldText, review.newText), textBox);
    } else {
        m_diff = textView(review.operation == BResourceReview::Create ? review.newText : review.oldText, textBox);
    }
    m_diff->setAccessibleName(textBox->title());
    m_diff->setMinimumHeight(BUiScale::px(160));
    textLayout->addWidget(m_diff);
    layout->addWidget(textBox, 3);

    // --- Who is affected ---
    if (review.operation != BResourceReview::Create || !review.affected.isEmpty()) {
        auto *usedBox = new QGroupBox(review.affectedLabel.isEmpty() ? tr("Affected") : review.affectedLabel,
                                      this);
        auto *usedLayout = new QVBoxLayout(usedBox);
        if (review.operation == BResourceReview::Update && review.affected.size() > 1) {
            usedLayout->addWidget(wrapped(tr("The change applies to every one of these %n resource(s).", "",
                                             int(review.affected.size())), usedBox));
        }
        m_affected = new QListWidget(usedBox);
        m_affected->setAccessibleName(usedBox->title());
        m_affected->setSelectionMode(QAbstractItemView::NoSelection);
        for (const QString &item : review.affected)
            new QListWidgetItem(item, m_affected);
        if (review.affected.isEmpty())
            new QListWidgetItem(tr("(nothing else, as far as this application knows)"), m_affected);
        m_affected->setMaximumHeight(BUiScale::px(110));
        usedLayout->addWidget(m_affected);
        if (!review.referencedBy.isEmpty()) {
            usedLayout->addWidget(wrapped(tr("The Director names as referencing it: %1")
                                              .arg(review.referencedBy.join(QLatin1String(", "))),
                                          usedBox));
        }
        layout->addWidget(usedBox, 1);
    }

    QStringList notes;
    if (!review.file.isEmpty()) {
        notes.append(review.sharedFile
                         ? tr("File on the Director: %1 (it holds other resources as well; only this "
                              "resource is replaced).").arg(review.file)
                         : tr("File on the Director: %1.").arg(review.file));
    }
    notes.append(tr("The Director reloads its configuration after writing. Jobs that are running or "
                    "already queued keep the configuration they started with."));
    layout->addWidget(wrapped(notes.join(QLatin1Char(' ')), this));

    auto *buttons = new QDialogButtonBox(this);
    m_write = buttons->addButton(review.operation == BResourceReview::Delete ? tr("&Delete on the Director")
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

void BResourceReviewDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    // The theme has set the palette by now
    colourDiff();
}

void BResourceReviewDialog::colourDiff()
{
    if (!m_diff || m_oldText.isEmpty() || m_newText.isEmpty())
        return;
    const bool dark = m_diff->palette().color(QPalette::Base).lightness() < 128;
    const QColor added = dark ? QColor(0x7f, 0xd8, 0x8a) : QColor(0x1b, 0x6e, 0x2a);
    const QColor removed = dark ? QColor(0xff, 0x8a, 0x80) : QColor(0xa3, 0x1f, 0x16);
    QList<QTextEdit::ExtraSelection> selections;
    for (QTextBlock block = m_diff->document()->begin(); block.isValid(); block = block.next()) {
        const QString text = block.text();
        if (!text.startsWith(QLatin1String("+ ")) && !text.startsWith(QLatin1String("- ")))
            continue;
        QTextEdit::ExtraSelection selection;
        selection.cursor = QTextCursor(block);
        selection.cursor.select(QTextCursor::LineUnderCursor);
        selection.format.setForeground(text.startsWith(QLatin1Char('+')) ? added : removed);
        selection.format.setFontWeight(QFont::Bold);
        selections.append(selection);
    }
    m_diff->setExtraSelections(selections);
}

// ============================================================================
// Report
// ============================================================================

QString BResourceReportDialog::problemLine(const std::optional<BConfig::Problem> &problem)
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

QString BResourceReportDialog::problemText() const
{
    return m_problem ? m_problem->text() : QString();
}

BResourceReportDialog::BResourceReportDialog(const BResourceReport &report, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(report.title);
    auto *layout = new QVBoxLayout(this);

    auto *header = new QHBoxLayout();
    QStyle::StandardPixmap icon = QStyle::SP_MessageBoxWarning;
    if (report.kind == BResourceReport::Written)
        icon = report.followups.isEmpty() ? QStyle::SP_MessageBoxInformation : QStyle::SP_MessageBoxWarning;
    else if (report.kind == BResourceReport::Failed || report.kind == BResourceReport::Invalid)
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
    texts->addWidget(BResourceReviewDialog::wrapped(report.text, this));
    if (report.validatedOnly && report.kind != BResourceReport::Written)
        texts->addWidget(BResourceReviewDialog::wrapped(tr("This was found when the Director checked the "
                                                           "change; nothing was sent to be written."), this));
    header->addLayout(texts, 1);
    layout->addLayout(header);

    if (!report.directorMessage.isEmpty())
        layout->addWidget(BResourceReviewDialog::wrapped(tr("The Director says: %1").arg(report.directorMessage),
                                                         this));

    if (report.problem) {
        auto *box = new QGroupBox(tr("Where the configuration is invalid"), this);
        auto *boxLayout = new QVBoxLayout(box);
        m_problem = BResourceReviewDialog::wrapped(problemLine(report.problem), box);
        BUiScale::bind(m_problem, BUiScale::Normal, BUiScale::Monospace);
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
        BUiScale::bind(list, BUiScale::Normal, BUiScale::Monospace);
        for (const QString &command : report.followups)
            new QListWidgetItem(command, list);
        boxLayout->addWidget(list);
        boxLayout->addWidget(BResourceReviewDialog::wrapped(tr("They are not run by this application."), box));
        layout->addWidget(box, 1);
    }

    const bool offerReload = report.kind == BResourceReport::Conflict
                          || report.kind == BResourceReport::FileDiffers;
    if (offerReload && (!report.currentText.isEmpty() || !report.ourText.isEmpty())) {
        auto *splitter = new QSplitter(Qt::Horizontal, this);
        auto addText = [&](const QString &label, const QString &text) {
            auto *box = new QGroupBox(label, splitter);
            auto *boxLayout = new QVBoxLayout(box);
            boxLayout->addWidget(BResourceReviewDialog::textView(text, box));
            splitter->addWidget(box);
        };
        addText(tr("On the Director now"), report.currentText);
        if (!report.ourText.isEmpty())
            addText(tr("What was to be written (not written)"), report.ourText);
        layout->addWidget(splitter, 3);
    }

    auto *buttons = new QDialogButtonBox(this);
    if (offerReload) {
        m_reloadButton = buttons->addButton(tr("&Reload the %1").arg(BResourcePersistence::noun(report.type)),
                                            QDialogButtonBox::AcceptRole);
        m_reloadButton->setToolTip(tr("Reads the %1 again from the Director; your changes are offered to be "
                                      "kept or discarded").arg(BResourcePersistence::noun(report.type)));
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
// BResourceSaveDialogs
// ============================================================================

bool BResourceSaveDialogs::confirm(const BResourceReview &review)
{
    BResourceReviewDialog dialog(review, m_parent);
    return dialog.exec() == QDialog::Accepted;
}

bool BResourceSaveDialogs::needsDialog(const BResourceReport &report)
{
    switch (report.kind) {
    case BResourceReport::Cancelled:
        return false;
    case BResourceReport::Written:
        return !report.followups.isEmpty() || !report.reloaded;
    default:
        return true;
    }
}

BResourceSaveUi::Answer BResourceSaveDialogs::report(const BResourceReport &report)
{
    if (!needsDialog(report))
        return Close;
    BResourceReportDialog dialog(report, m_parent);
    dialog.exec();
    return dialog.reloadChosen() ? Reload : Close;
}
