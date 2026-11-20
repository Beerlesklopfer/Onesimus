/**
 * @file bschedulegridview.cpp
 * @brief One schedule resource as a grid of weekdays and hours, to look at
 *        and to edit
 */

#include "schedules/bschedulegridview.h"
#include "buiscale.h"

#include "schedules/brunruledialog.h"
#include "schedules/bschedulegriddialogs.h"
#include "schedules/bschedulegrideditor.h"
#include "schedules/bscheduleminicalendar.h"
#include "schedules/bschedulepalette.h"
#include "schedules/model/bschedulegridtext.h"

#include <QAccessible>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMenu>
#include <QPainter>
#include <QPushButton>
#include <QShortcut>
#include <QSplitter>
#include <QStackedLayout>
#include <QStyle>
#include <QToolButton>
#include <QTreeWidget>
#include <QUndoStack>
#include <QVBoxLayout>

namespace {

// Data of an item of the "Run lines" list
const int RunIndexRole = Qt::UserRole;
const int RuleIndexRole = Qt::UserRole + 1;

/// Do two schedules say the same, line for line as the Director reported them?
bool sameAsReported(const BSchedule &a, const BSchedule &b)
{
    if (a.name != b.name || a.description != b.description || a.enabled != b.enabled
        || a.runs.size() != b.runs.size()) {
        return false;
    }
    for (int i = 0; i < a.runs.size(); ++i) {
        if (a.runs[i].original != b.runs[i].original)
            return false;
    }
    return true;
}

} // namespace

/**
 * @brief The legend below the palette
 *
 * The marks of the grid that are not brushes, each drawn as the grid draws
 * it: runs of other scopes, the warning sign, the mark of a calendar rule.
 * Only those that occur. The brushes themselves are in the palette. The
 * items flow into as many rows as the width needs.
 */
class BScheduleGridLegend : public QWidget
{
public:
    enum Kind { Run, Ghost, Warning, Rule };

    struct Item
    {
        Kind kind = Run;
        BGridRun run;       ///< Run and Ghost: what the swatch shows
        std::optional<QString> level;   ///< Rule: the level of the mark
        QString mark;       ///< Rule: the text of the mark
        QString text;
    };

    explicit BScheduleGridLegend(BScheduleGridWidget *grid, QWidget *parent = nullptr)
        : QWidget(parent), m_grid(grid)
    {
        QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        policy.setHeightForWidth(true);
        setSizePolicy(policy);
    }

    void setItems(const QList<Item> &items)
    {
        m_items = items;
        QStringList texts;
        for (const Item &item : items)
            texts.append(item.text);
        setAccessibleDescription(texts.join(QLatin1String("; ")));
        updateGeometry();
        update();
    }

    const QList<Item> &items() const { return m_items; }

    bool hasHeightForWidth() const override { return true; }
    int heightForWidth(int width) const override
    {
        return layoutItems(width, nullptr);
    }
    QSize sizeHint() const override
    {
        const int wide = m_grid->px(400);
        return QSize(wide, heightForWidth(qMax(wide, width())));
    }
    QSize minimumSizeHint() const override
    {
        return QSize(m_grid->px(120), m_items.isEmpty() ? 0 : rowHeight());
    }

protected:
    void changeEvent(QEvent *event) override
    {
        QWidget::changeEvent(event);
        if (event->type() == QEvent::FontChange || event->type() == QEvent::StyleChange) {
            updateGeometry();
            update();
        }
    }

    void paintEvent(QPaintEvent *) override
    {
        QList<QRect> places;
        layoutItems(width(), &places);
        QPainter painter(this);
        const QColor text = palette().color(QPalette::Text);
        const QSize swatchSize = m_grid->swatchSize();
        const int gap = m_grid->px(5);
        for (int i = 0; i < m_items.size(); ++i) {
            const Item &item = m_items[i];
            const QRect swatch(places[i].left(),
                               places[i].top() + (places[i].height() - swatchSize.height()) / 2,
                               swatchSize.width(), swatchSize.height());
            switch (item.kind) {
            case Run:
                m_grid->paintRun(painter, swatch, item.run, true);
                break;
            case Ghost:
                m_grid->paintRun(painter, swatch, item.run, false);
                break;
            case Warning:
                m_grid->paintWarningMark(painter, swatch.adjusted(m_grid->px(11), 0, 0, -m_grid->px(6)), text);
                break;
            case Rule:
                m_grid->paintRuleMark(painter, swatch, item.mark, item.level, false);
                break;
            }
            if (item.kind == Run) {
                painter.setPen(palette().color(QPalette::Mid));
                painter.setBrush(Qt::NoBrush);
                painter.drawRect(swatch.adjusted(0, 0, -1, -1));
            }
            painter.setFont(font());
            painter.setPen(text);
            painter.drawText(places[i].adjusted(swatchSize.width() + gap, 0, 0, 0),
                             Qt::AlignLeft | Qt::AlignVCenter, item.text);
        }
    }

private:
    /// A row holds a swatch and a line of text, whichever is higher
    int rowHeight() const
    {
        return qMax(m_grid->swatchSize().height(), QFontMetrics(font()).height()) + m_grid->px(4);
    }

    /// Places the items in rows; @return the height they need
    int layoutItems(int width, QList<QRect> *places) const
    {
        const QFontMetrics metrics(font());
        const int swatchWidth = m_grid->swatchSize().width();
        const int gap = m_grid->px(5);
        const int spacing = m_grid->px(18);
        const int row = rowHeight();
        int x = 0;
        int y = 0;
        for (const Item &item : m_items) {
            const int itemWidth = swatchWidth + gap + metrics.horizontalAdvance(item.text);
            if (x > 0 && x + itemWidth > width) {
                x = 0;
                y += row;
            }
            if (places)
                places->append(QRect(x, y, itemWidth, row));
            x += itemWidth + spacing;
        }
        return m_items.isEmpty() ? 0 : y + row;
    }

    BScheduleGridWidget *m_grid;
    QList<Item> m_items;
};

BScheduleGridView::BScheduleGridView(QWidget *parent)
    : QWidget(parent)
    , m_model(new BScheduleGridModel(this))
    , m_editor(new BScheduleGridEditor(m_model, this))
{
    m_model->setFirstDayOfWeek(QLocale().firstDayOfWeek());
    buildUi();

    connect(m_model, &BScheduleGridModel::scheduleReset, this, &BScheduleGridView::onScheduleReset);
    connect(m_model, &BScheduleGridModel::scopesChanged, this, &BScheduleGridView::rebuildScopeBar);
    connect(m_model, &BScheduleGridModel::activeScopeChanged,
            this, &BScheduleGridView::onActiveScopeChanged);
    connect(m_model, &BScheduleGridModel::runLinesChanged, this, [this]() {
        // What was outlined belongs to the lines as they were
        m_grid->setHighlightedRun(-1);
        m_grid->setHighlightedRule(-1);
        rebuildRunLines();
        rebuildLegend();
        updateModified();
        m_calendar->setSchedule(m_model->schedule());
    });
    connect(m_model, &BScheduleGridModel::firstDayOfWeekChanged, this, [this]() {
        m_calendar->setFirstDayOfWeek(m_model->firstDayOfWeek());
    });
    connect(m_model, &BScheduleGridModel::calendarRulesChanged, this, &BScheduleGridView::rebuildRules);
    connect(m_model, &BScheduleGridModel::selectionChanged,
            this, &BScheduleGridView::updateSelectionLabel);
    connect(m_model, &BScheduleGridModel::cellsChanged, this, &BScheduleGridView::updateSelectionLabel);

    // F6 goes round the parts of the view
    auto *next = new QShortcut(QKeySequence(Qt::Key_F6), this);
    next->setContext(Qt::WidgetWithChildrenShortcut);
    connect(next, &QShortcut::activated, this, [this]() { focusNextPart(false); });
    auto *previous = new QShortcut(QKeySequence(Qt::SHIFT | Qt::Key_F6), this);
    previous->setContext(Qt::WidgetWithChildrenShortcut);
    connect(previous, &QShortcut::activated, this, [this]() { focusNextPart(true); });

    // Undo and redo, wherever in the view the keyboard is
    auto *undo = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z), this);
    undo->setContext(Qt::WidgetWithChildrenShortcut);
    connect(undo, &QShortcut::activated, m_editor, &BScheduleGridEditor::undo);
    for (const QKeySequence &keys : {QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Z),
                                     QKeySequence(Qt::CTRL | Qt::Key_Y)}) {
        auto *redo = new QShortcut(keys, this);
        redo->setContext(Qt::WidgetWithChildrenShortcut);
        connect(redo, &QShortcut::activated, m_editor, &BScheduleGridEditor::redo);
    }

    QUndoStack *stack = m_editor->undoStack();
    auto updateUndo = [this, stack]() {
        m_undo->setEnabled(stack->canUndo());
        m_redo->setEnabled(stack->canRedo());
        m_revert->setEnabled(stack->canUndo() || descriptionChanged());
        m_save->setEnabled(isModified() && !isLocked());
        m_undo->setToolTip(stack->canUndo() ? tr("Undo: %1 (Ctrl+Z)").arg(stack->undoText())
                                            : tr("Nothing to undo (Ctrl+Z)"));
        m_redo->setToolTip(stack->canRedo() ? tr("Redo: %1 (Ctrl+Shift+Z or Ctrl+Y)").arg(stack->redoText())
                                            : tr("Nothing to redo (Ctrl+Shift+Z or Ctrl+Y)"));
    };
    connect(stack, &QUndoStack::indexChanged, this, updateUndo);
    connect(stack, &QUndoStack::canUndoChanged, this, updateUndo);
    connect(stack, &QUndoStack::canRedoChanged, this, updateUndo);
    updateUndo();
    connect(m_undo, &QPushButton::clicked, m_editor, &BScheduleGridEditor::undo);
    connect(m_redo, &QPushButton::clicked, m_editor, &BScheduleGridEditor::redo);
    connect(m_revert, &QPushButton::clicked, m_editor, &BScheduleGridEditor::revertAll);
    connect(m_revert, &QPushButton::clicked, this, [this]() {
        m_description->setText(m_model->loadedSchedule().description);
        updateModified();
    });

    connect(m_editor, &BScheduleGridEditor::edited, this, [this](const QString &what) {
        m_editError->clear();
        m_editError->setVisible(false);
        announce(what);
    });
    connect(m_editor, &BScheduleGridEditor::editFailed, this, [this](const QString &message) {
        // U+26A0, the warning sign
        m_editError->setText(QStringLiteral("\u26A0 ") + message);
        m_editError->setVisible(true);
        announce(message);
    });

    clear();
}

void BScheduleGridView::buildUi()
{
    m_stack = new QStackedLayout(this);
    m_stack->setContentsMargins(0, 0, 0, 0);

    // Page 0: nothing chosen
    m_placeholder = new QLabel(tr("No schedule chosen. Select a schedule in the list on the left "
                                  "to see it as a grid of weekdays and hours."), this);
    m_placeholder->setAlignment(Qt::AlignCenter);
    m_placeholder->setWordWrap(true);
    m_stack->addWidget(m_placeholder);

    // Page 1: the schedule
    auto *content = new QWidget(this);
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    m_stack->addWidget(content);

    m_mainSplitter = new QSplitter(Qt::Vertical, content);
    m_mainSplitter->setChildrenCollapsible(false);
    contentLayout->addWidget(m_mainSplitter);

    auto *top = new QWidget(m_mainSplitter);
    auto *topLayout = new QVBoxLayout(top);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(4);

    // --- Name and use ---
    auto *headerRow = new QHBoxLayout();
    m_title = new QLabel(top);
    m_title->setTextFormat(Qt::PlainText);
    BUiScale::bind(m_title, BUiScale::Large, BUiScale::Bold);
    // Name and use may be cut in a narrow window (both have tool tips);
    // the buttons may not
    BUiScale::track(m_title, [this]() { m_title->setMinimumWidth(BUiScale::px(60)); });
    headerRow->addWidget(m_title);
    m_usage = new QLabel(top);
    m_usage->setTextFormat(Qt::PlainText);
    BUiScale::track(m_usage, [this]() { m_usage->setMinimumWidth(BUiScale::px(40)); });
    headerRow->addWidget(m_usage);
    headerRow->addStretch(1);
    // Whether the Run lines differ from what the Director reported
    m_modified = new QLabel(top);
    m_modified->setTextFormat(Qt::PlainText);
    m_modified->setObjectName(QStringLiteral("scheduleGridModified"));
    headerRow->addWidget(m_modified);
    m_undo = new QPushButton(top);
    m_undo->setText(tr("Undo"));
    m_undo->setAccessibleName(tr("Undo"));
    m_redo = new QPushButton(top);
    m_redo->setText(tr("Redo"));
    m_redo->setAccessibleName(tr("Redo"));
    m_revert = new QPushButton(top);
    m_revert->setText(tr("Revert all"));
    m_revert->setToolTip(tr("Undoes every change of this schedule. Redo brings them back."));
    m_save = new QPushButton(top);
    m_save->setText(tr("&Save..."));
    m_save->setObjectName(QStringLiteral("scheduleGridSave"));
    m_save->setToolTip(tr("Writes the changed Run lines to the Director. The Director checks them first, "
                          "and you see old and new lines and the jobs that use the schedule before "
                          "anything is written."));
    m_save->setEnabled(false);
    m_delete = new QPushButton(top);
    m_delete->setText(tr("Delete..."));
    m_delete->setToolTip(tr("Deletes this schedule on the Director, after a check that no job uses it "
                            "and your confirmation"));
    headerRow->addWidget(m_undo);
    headerRow->addWidget(m_redo);
    headerRow->addWidget(m_revert);
    // Copy and rename: disabled where the connection cannot do them (the
    // tab says why in the tooltip, setCopyRenameAvailable()); placed on the
    // description row below
    m_copy = new QPushButton(top);
    m_copy->setText(tr("Cop&y..."));
    m_copy->setObjectName(QStringLiteral("scheduleGridCopy"));
    m_copy->setEnabled(false);
    m_rename = new QPushButton(top);
    m_rename->setText(tr("Re&name..."));
    m_rename->setObjectName(QStringLiteral("scheduleGridRename"));
    m_rename->setEnabled(false);
    headerRow->addWidget(m_save);
    headerRow->addWidget(m_delete);
    topLayout->addLayout(headerRow);

    // --- Description (written with the Run lines) ---
    auto *descriptionRow = new QHBoxLayout();
    auto *descriptionLabel = new QLabel(tr("Descr&iption:"), top);
    descriptionRow->addWidget(descriptionLabel);
    m_description = new QLineEdit(top);
    m_description->setObjectName(QStringLiteral("scheduleDescription"));
    m_description->setPlaceholderText(tr("No description"));
    m_description->setToolTip(tr("The Description of the schedule. Saved with the Run lines; empty "
                                 "removes it from the file."));
    m_description->setClearButtonEnabled(true);
    descriptionLabel->setBuddy(m_description);
    descriptionRow->addWidget(m_description, 1);
    // On this row, not the header: there the buttons would make the view
    // wider than a small window, the description field can give way
    descriptionRow->addWidget(m_copy);
    descriptionRow->addWidget(m_rename);
    topLayout->addLayout(descriptionRow);
    connect(m_description, &QLineEdit::textEdited, this, [this]() {
        updateModified();
        m_revert->setEnabled(m_editor->undoStack()->canUndo() || descriptionChanged());
    });
    connect(m_save, &QPushButton::clicked, this, &BScheduleGridView::saveRequested);
    connect(m_delete, &QPushButton::clicked, this, &BScheduleGridView::deleteRequested);
    connect(m_copy, &QPushButton::clicked, this, &BScheduleGridView::copyRequested);
    connect(m_rename, &QPushButton::clicked, this, &BScheduleGridView::renameRequested);

    // What stands in the way of writing the schedule; a line of its own,
    // because it can be long
    m_notice = new QLabel(top);
    m_notice->setTextFormat(Qt::PlainText);
    m_notice->setWordWrap(true);
    m_notice->setObjectName(QStringLiteral("scheduleGridNotice"));
    topLayout->addWidget(m_notice);

    // A schedule that was offered while there were changes: they are kept,
    // and the two ways on are offered here
    m_pendingBar = new QFrame(top);
    m_pendingBar->setObjectName(QStringLiteral("scheduleGridPending"));
    m_pendingBar->setFrameShape(QFrame::StyledPanel);
    auto *pendingLayout = new QHBoxLayout(m_pendingBar);
    pendingLayout->setContentsMargins(8, 4, 8, 4);
    m_pendingLabel = new QLabel(m_pendingBar);
    m_pendingLabel->setTextFormat(Qt::PlainText);
    m_pendingLabel->setWordWrap(true);
    pendingLayout->addWidget(m_pendingLabel, 1);
    m_keepChanges = new QPushButton(tr("&Keep my changes"), m_pendingBar);
    m_discard = new QPushButton(tr("&Discard and reload"), m_pendingBar);
    pendingLayout->addWidget(m_keepChanges);
    pendingLayout->addWidget(m_discard);
    connect(m_keepChanges, &QPushButton::clicked, this, &BScheduleGridView::keepChanges);
    connect(m_discard, &QPushButton::clicked, this, &BScheduleGridView::discardAndReload);
    m_pendingBar->setVisible(false);
    topLayout->addWidget(m_pendingBar);

    // --- Scope bar ---
    m_scopeBar = new QWidget(top);
    m_scopeBar->setAccessibleName(tr("Scopes"));
    m_scopeLayout = new QHBoxLayout(m_scopeBar);
    m_scopeLayout->setContentsMargins(0, 0, 0, 0);
    m_scopeLayout->setSpacing(4);
    auto *scopeLabel = new QLabel(tr("Scope:"), m_scopeBar);
    scopeLabel->setToolTip(tr("The weeks of the month and the months a Run line is restricted to. "
                              "The grid shows the runs of the chosen scope; runs of the other "
                              "scopes are hatched."));
    m_scopeLayout->addWidget(scopeLabel);
    m_addScope = new QToolButton(m_scopeBar);
    m_addScope->setText(QStringLiteral("+"));
    m_addScope->setObjectName(QStringLiteral("scheduleGridAddScope"));
    m_addScope->setAccessibleName(tr("Add scope"));
    m_addScope->setToolTip(tr("Add scope: weeks of the month, and months, to paint a grid of "
                              "their own for"));
    m_addScope->setFocusPolicy(Qt::StrongFocus);
    m_scopeLayout->addWidget(m_addScope);
    connect(m_addScope, &QToolButton::clicked, this, [this]() { openScopeDialog(); });
    m_scopeLayout->addStretch(1);
    m_scopeGroup = new QButtonGroup(this);
    m_scopeGroup->setExclusive(true);
    connect(m_scopeGroup, &QButtonGroup::idClicked, this, [this](int id) {
        m_grid->setHighlightedRun(-1);
        m_model->setActiveScope(id);
    });
    topLayout->addWidget(m_scopeBar);

    // --- Grid, legend, selection ---
    m_grid = new BScheduleGridWidget(top);
    m_grid->setModel(m_model);
    m_grid->setEditor(m_editor);
    topLayout->addWidget(m_grid, 1);

    // After the grid has its editor: the palette draws with the grid's tags
    m_palette = new BSchedulePalette(m_editor, m_grid, top);
    topLayout->addWidget(m_palette);

    m_legend = new BScheduleGridLegend(m_grid, top);
    m_legend->setAccessibleName(tr("Legend"));
    topLayout->addWidget(m_legend);

    m_selectionLabel = new QLabel(top);
    m_selectionLabel->setTextFormat(Qt::PlainText);
    m_selectionLabel->setWordWrap(true);
    m_selectionLabel->setObjectName(QStringLiteral("scheduleGridSelection"));
    topLayout->addWidget(m_selectionLabel);

    // Why an edit could not be made
    m_editError = new QLabel(top);
    m_editError->setTextFormat(Qt::PlainText);
    m_editError->setWordWrap(true);
    m_editError->setObjectName(QStringLiteral("scheduleGridNotice"));
    m_editError->setVisible(false);
    topLayout->addWidget(m_editError);

    // --- Lists and calendar ---
    m_listSplitter = new QSplitter(Qt::Horizontal, m_mainSplitter);
    m_listSplitter->setChildrenCollapsible(false);

    m_runLinesBox = new QGroupBox(tr("Run lines"), m_listSplitter);
    auto *runLinesLayout = new QVBoxLayout(m_runLinesBox);
    runLinesLayout->setContentsMargins(4, 4, 4, 4);
    m_runLines = new QTreeWidget(m_runLinesBox);
    m_runLines->setAccessibleName(tr("Run lines"));
    m_runLines->setRootIsDecorated(false);
    m_runLines->setUniformRowHeights(true);
    m_runLines->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_runLines->setSelectionMode(QAbstractItemView::SingleSelection);
    m_runLines->setHeaderLabels({tr("No."), tr("Run line"), tr("Shown in")});
    m_runLines->setToolTip(tr("The Run lines of the schedule in their order. Choosing a line "
                              "outlines its cells in the grid and switches to its scope."));
    runLinesLayout->addWidget(m_runLines);
    connect(m_runLines, &QTreeWidget::currentItemChanged, this,
            [this](QTreeWidgetItem *item) { onRunLineChosen(item); });
    connect(m_runLines, &QTreeWidget::itemClicked, this,
            [this](QTreeWidgetItem *item) { onRunLineChosen(item); });
    // Enter or a double click on the line of a calendar rule: its form
    connect(m_runLines, &QTreeWidget::itemActivated, this, [this](QTreeWidgetItem *item) {
        const int rule = item ? item->data(0, RuleIndexRole).toInt() : -1;
        if (rule >= 0)
            openRuleDialog(rule);
    });

    // Calendar rules are not painted: they have a form
    auto *ruleButtons = new QHBoxLayout();
    m_addRule = new QPushButton(tr("&Add rule..."), m_runLinesBox);
    m_addRule->setAccessibleName(tr("Add calendar rule"));
    m_addRule->setToolTip(tr("Adds a calendar rule: a Run line by day of the month or by week "
                             "of the year, such as \"on 1 at 21:00\""));
    m_editRule = new QPushButton(tr("&Edit rule..."), m_runLinesBox);
    m_editRule->setAccessibleName(tr("Edit calendar rule"));
    m_editRule->setToolTip(tr("Opens the form of the calendar rule that is chosen in the list"));
    m_removeRule = new QPushButton(tr("&Remove rule"), m_runLinesBox);
    m_removeRule->setAccessibleName(tr("Remove calendar rule"));
    m_removeRule->setToolTip(tr("Removes the calendar rule that is chosen in the list"));
    ruleButtons->addWidget(m_addRule);
    ruleButtons->addWidget(m_editRule);
    ruleButtons->addWidget(m_removeRule);
    ruleButtons->addStretch(1);
    runLinesLayout->addLayout(ruleButtons);
    connect(m_addRule, &QPushButton::clicked, this, [this]() { openRuleDialog(-1); });
    connect(m_editRule, &QPushButton::clicked, this, [this]() { openRuleDialog(currentRule()); });
    connect(m_removeRule, &QPushButton::clicked, this, [this]() { removeRule(currentRule()); });

    m_rulesBox = new QGroupBox(tr("Calendar rules"), m_listSplitter);
    auto *rulesLayout = new QVBoxLayout(m_rulesBox);
    rulesLayout->setContentsMargins(4, 4, 4, 4);
    m_rules = new QTreeWidget(m_rulesBox);
    m_rules->setAccessibleName(tr("Calendar rules"));
    m_rules->setRootIsDecorated(false);
    m_rules->setUniformRowHeights(true);
    m_rules->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_rules->setSelectionMode(QAbstractItemView::SingleSelection);
    m_rules->setHeaderLabels({tr("Mark"), tr("Runs"), tr("Run line")});
    m_rules->setToolTip(tr("Run lines by day of the month or week of the year. They do not belong "
                           "to a weekday and are marked in the band below the grid. Enter or a "
                           "double click opens the form of a rule, Del removes it."));
    rulesLayout->addWidget(m_rules);
    connect(m_rules, &QTreeWidget::itemActivated, this, [this](QTreeWidgetItem *item) {
        if (item)
            openRuleDialog(item->data(0, RuleIndexRole).toInt());
    });
    m_rules->installEventFilter(this);
    m_rules->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_rules, &QTreeWidget::customContextMenuRequested, this, [this](const QPoint &pos) {
        showRuleMenu(m_rules->viewport()->mapToGlobal(pos));
    });
    connect(m_rules, &QTreeWidget::currentItemChanged, this,
            [this](QTreeWidgetItem *item) { onRuleChosen(item); });
    connect(m_rules, &QTreeWidget::itemClicked, this,
            [this](QTreeWidgetItem *item) { onRuleChosen(item); });

    m_calendarBox = new QGroupBox(tr("Next five weeks"), m_listSplitter);
    auto *calendarLayout = new QVBoxLayout(m_calendarBox);
    calendarLayout->setContentsMargins(4, 4, 4, 4);
    calendarLayout->setSpacing(4);
    m_calendar = new BScheduleMiniCalendar(m_calendarBox);
    m_calendar->setFirstDayOfWeek(m_model->firstDayOfWeek());
    m_calendar->setToolTip(tr("The levels that run on each date"));

    auto *navigation = new QHBoxLayout();
    auto *previous = new QToolButton(m_calendarBox);
    previous->setText(QStringLiteral("\u25C0"));
    previous->setToolTip(tr("Previous month"));
    previous->setAccessibleName(tr("Previous month"));
    auto *next = new QToolButton(m_calendarBox);
    next->setText(QStringLiteral("\u25B6"));
    next->setToolTip(tr("Next month"));
    next->setAccessibleName(tr("Next month"));
    auto *today = new QToolButton(m_calendarBox);
    today->setText(tr("Today"));
    today->setToolTip(tr("Back to the current week"));
    m_calendarRange = new QLabel(m_calendarBox);
    m_calendarRange->setTextFormat(Qt::PlainText);
    m_calendarRange->setAlignment(Qt::AlignCenter);
    navigation->addWidget(previous);
    navigation->addWidget(m_calendarRange, 1);
    navigation->addWidget(next);
    navigation->addWidget(today);
    calendarLayout->addLayout(navigation);
    calendarLayout->addWidget(m_calendar, 1);
    connect(previous, &QToolButton::clicked, m_calendar, &BScheduleMiniCalendar::showPreviousMonth);
    connect(next, &QToolButton::clicked, m_calendar, &BScheduleMiniCalendar::showNextMonth);
    connect(today, &QToolButton::clicked, m_calendar, &BScheduleMiniCalendar::showToday);
    connect(m_calendar, &BScheduleMiniCalendar::rangeChanged,
            this, &BScheduleGridView::updateCalendarRange);
    updateCalendarRange();

    m_mainSplitter->addWidget(top);
    m_mainSplitter->addWidget(m_listSplitter);
    // The grid takes the room there is; the lists keep the height they ask
    // for, and the calendar the width
    m_mainSplitter->setStretchFactor(0, 1);
    m_mainSplitter->setStretchFactor(1, 0);
    m_listSplitter->setStretchFactor(0, 5);
    m_listSplitter->setStretchFactor(1, 4);
    m_listSplitter->setStretchFactor(2, 0);

    connect(m_grid, &BScheduleGridWidget::runActivated, this, &BScheduleGridView::selectRunLine);
    connect(m_grid, &BScheduleGridWidget::calendarRuleActivated,
            this, &BScheduleGridView::showRuleInLists);
    connect(m_grid, &BScheduleGridWidget::calendarRuleEditRequested, this, [this](int rule) {
        showRuleInLists(rule);
        openRuleDialog(rule);
    });
}

void BScheduleGridView::setSchedule(const BSchedule &schedule)
{
    m_hasSchedule = true;
    m_pending.reset();
    m_declined.reset();
    m_model->setSchedule(schedule);
    m_stack->setCurrentIndex(1);
}

bool BScheduleGridView::offerSchedule(const BSchedule &schedule)
{
    if (!m_hasSchedule || !isModified()) {
        setSchedule(schedule);
        return true;
    }
    // There are changes. Nothing new is nothing to ask about ...
    if (sameAsReported(schedule, m_model->loadedSchedule())) {
        if (m_pending) {
            m_pending.reset();
            updatePendingNotice();
        }
        return true;
    }
    // ... and what was declined, or is being asked about, is not asked again
    if (m_declined && sameAsReported(schedule, *m_declined))
        return false;
    if (m_pending && sameAsReported(schedule, *m_pending))
        return false;
    m_pending = schedule;
    updatePendingNotice();
    announce(pendingText());
    return false;
}

void BScheduleGridView::keepChanges()
{
    if (!m_pending)
        return;
    // A refresh that brings the same again is not asked about again. The
    // choice of another schedule is: it is made by hand every time.
    if (m_pending->name == m_model->loadedSchedule().name)
        m_declined = m_pending;
    m_pending.reset();
    updatePendingNotice();
    updateModified();
    emit changesKept(m_model->loadedSchedule().name);
}

void BScheduleGridView::discardAndReload()
{
    if (!m_pending)
        return;
    const BSchedule schedule = *m_pending;
    setSchedule(schedule);
    emit pendingScheduleShown(schedule.name);
}

bool BScheduleGridView::isModified() const
{
    return m_hasSchedule && (m_model->isModified() || descriptionChanged());
}

QString BScheduleGridView::description() const
{
    return m_description ? m_description->text().trimmed() : QString();
}

bool BScheduleGridView::descriptionChanged() const
{
    return m_hasSchedule && m_description && description() != m_model->loadedSchedule().description;
}

void BScheduleGridView::clear()
{
    m_hasSchedule = false;
    m_delete->setEnabled(false);
    m_copy->setEnabled(false);
    m_rename->setEnabled(false);
    m_jobs.clear();
    m_jobsKnown = false;
    m_pending.reset();
    m_declined.reset();
    m_model->setSchedule(BSchedule());
    m_stack->setCurrentIndex(0);
}

void BScheduleGridView::setUsage(const QStringList &jobs, bool known)
{
    m_jobs = jobs;
    m_jobsKnown = known;
    updateHeader();
}

void BScheduleGridView::setLevelColorSource(const BScheduleGridWidget::LevelColorSource &source)
{
    m_grid->setLevelColorSource(source);
    m_calendar->setLevelColorSource(source);
    m_palette->refresh();
    m_legend->update();
}

void BScheduleGridView::setToday(const QDate &today)
{
    m_calendar->setToday(today);
}

QList<QAbstractButton *> BScheduleGridView::scopeButtons() const
{
    return m_scopeGroup->buttons();
}

QString BScheduleGridView::titleText() const
{
    return m_title->text();
}

QString BScheduleGridView::usageText() const
{
    return m_usage->text();
}

QString BScheduleGridView::modifiedText() const
{
    if (!isModified())
        return QString();
    if (!m_model->isModified()) {
        QString text = tr("Modified: the description, not written to the Director.");
        if (m_declined && m_declined->name == m_model->loadedSchedule().name)
            text += QLatin1Char(' ') + tr("The Director reports other Run lines since.");
        return text;
    }
    int changed = 0;
    int added = 0;
    int removed = 0;
    for (const BGridRunLineText &line : m_model->runLines()) {
        changed += line.state == BGridRunLineText::Changed;
        added += line.state == BGridRunLineText::New;
        removed += line.state == BGridRunLineText::Removed;
    }
    QStringList parts;
    if (changed)
        parts.append(tr("%n changed", "", changed));
    if (added)
        parts.append(tr("%n new", "", added));
    if (removed)
        parts.append(tr("%n removed", "", removed));
    QString text = descriptionChanged()
        ? tr("Modified: %1 Run line(s) and the description, not written to the Director.")
              .arg(parts.join(QLatin1String(", ")))
        : tr("Modified: %1 Run line(s), not written to the Director.")
              .arg(parts.join(QLatin1String(", ")));
    if (m_declined && m_declined->name == m_model->loadedSchedule().name)
        text += QLatin1Char(' ') + tr("The Director reports other Run lines since.");
    return text;
}

QString BScheduleGridView::noticeText() const
{
    return m_notice->isHidden() ? QString() : m_notice->text();
}

QString BScheduleGridView::pendingText() const
{
    return m_pending ? m_pendingLabel->text() : QString();
}

QString BScheduleGridView::editErrorText() const
{
    return m_editError->text();
}

QAbstractButton *BScheduleGridView::undoButton() const { return m_undo; }
QAbstractButton *BScheduleGridView::redoButton() const { return m_redo; }
QAbstractButton *BScheduleGridView::revertButton() const { return m_revert; }
QAbstractButton *BScheduleGridView::keepChangesButton() const { return m_keepChanges; }
QAbstractButton *BScheduleGridView::discardButton() const { return m_discard; }
QAbstractButton *BScheduleGridView::addScopeButton() const { return m_addScope; }
QAbstractButton *BScheduleGridView::addRuleButton() const { return m_addRule; }
QAbstractButton *BScheduleGridView::editRuleButton() const { return m_editRule; }
QAbstractButton *BScheduleGridView::removeRuleButton() const { return m_removeRule; }
QAbstractButton *BScheduleGridView::saveButton() const { return m_save; }
QAbstractButton *BScheduleGridView::deleteButton() const { return m_delete; }
QAbstractButton *BScheduleGridView::copyButton() const { return m_copy; }
QAbstractButton *BScheduleGridView::renameButton() const { return m_rename; }

void BScheduleGridView::setCopyRenameAvailable(bool canCopy, const QString &copyWhyNot, bool canRename,
                                               const QString &renameWhyNot)
{
    m_canCopy = canCopy;
    m_canRename = canRename;
    m_copyWhyNot = copyWhyNot;
    m_renameWhyNot = renameWhyNot;
    updateCopyRename();
}

void BScheduleGridView::updateCopyRename()
{
    if (!m_copy)
        return;
    m_copy->setEnabled(m_hasSchedule && m_canCopy);
    m_copy->setToolTip(m_canCopy
        ? tr("Creates a new schedule with the Run lines and the Description of this one, as the "
             "Director reported them (changes not written are not copied); you choose the name and "
             "see the new resource before anything is written")
        : m_copyWhyNot);
    const bool modified = isModified();
    m_rename->setEnabled(m_hasSchedule && m_canRename && !modified);
    m_rename->setToolTip(!m_canRename
        ? m_renameWhyNot
        : modified ? tr("Save or revert the changes of this schedule first: renaming replaces it")
                   : tr("Gives this schedule another name: a copy under the new name, every job that "
                        "names it changed to the copy, then the old one deleted; all shown once for "
                        "confirmation first"));
}

bool BScheduleGridView::isLocked() const
{
    return m_hasSchedule && m_editor->isLocked();
}

void BScheduleGridView::setWritable(bool canSave, bool canDelete)
{
    m_canSave = canSave;
    m_canDelete = canDelete;
    m_save->setToolTip(canSave
        ? tr("Writes the changed Run lines to the Director. The Director checks them first, and you "
             "see old and new lines and the jobs that use the schedule before anything is written.")
        : tr("This connection cannot change a schedule; the click says why."));
    m_delete->setToolTip(canDelete
        ? tr("Deletes this schedule on the Director, after a check that no job uses it and your "
             "confirmation")
        : tr("This connection cannot delete a schedule; the click says why."));
}

void BScheduleGridView::updateLock()
{
    // Locked: nothing that edits is offered
    const bool editable = m_hasSchedule && !isLocked();
    m_palette->setEnabled(editable);
    m_addScope->setEnabled(editable);
    m_description->setReadOnly(!editable);
    m_scopeBar->setProperty("locked", !editable);
    m_save->setEnabled(isModified() && editable);
    m_delete->setEnabled(m_hasSchedule);
    updateCopyRename();
    updateRuleButtons();
}

// --- Model changes -------------------------------------------------------------

void BScheduleGridView::onScheduleReset()
{
    // A form that is open belongs to the schedule that was on display
    if (m_scopeDialog)
        m_scopeDialog->reject();
    if (m_ruleDialog)
        m_ruleDialog->reject();
    m_editError->clear();
    m_editError->setVisible(false);
    m_grid->setHighlightedRun(-1);
    m_grid->setHighlightedRule(-1);
    m_grid->setCurrentCell(BGridPos{0, 0});
    rebuildScopeBar();
    rebuildRunLines();
    rebuildRules();
    // The description as the Director reports it; an edit of it is gone
    // with the Run lines
    m_description->setText(m_model->loadedSchedule().description);
    updateHeader();
    updateModified();
    updatePendingNotice();
    updateSelectionLabel();
    m_calendar->setSchedule(m_model->schedule());
    updateLock();
}

void BScheduleGridView::updateModified()
{
    const bool modified = isModified();
    m_modified->setText(modifiedText());
    m_modified->setVisible(modified);
    m_save->setEnabled(modified && !isLocked());
    updateCopyRename();
    if (modified != m_wasModified) {
        m_wasModified = modified;
        emit modifiedChanged(modified);
    }
}

void BScheduleGridView::updatePendingNotice()
{
    if (!m_pending) {
        m_pendingBar->setVisible(false);
        return;
    }
    const QString shown = m_model->loadedSchedule().name;
    if (m_pending->name == shown) {
        m_pendingLabel->setText(tr("The Director now reports other Run lines for this schedule than "
                                   "the ones your changes started from. Your changes are kept and "
                                   "nothing was replaced."));
        m_discard->setText(tr("&Discard and reload"));
    } else {
        m_pendingLabel->setText(tr("The schedule %1 has changes that are not written. Showing %2 "
                                   "discards them.").arg(shown, m_pending->name));
        m_discard->setText(tr("&Discard and show %1").arg(m_pending->name));
    }
    m_pendingBar->setVisible(true);
}

void BScheduleGridView::announce(const QString &text)
{
#if QT_CONFIG(accessibility) && QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    if (text.isEmpty())
        return;
    QAccessibleAnnouncementEvent event(m_grid, text);
    QAccessible::updateAccessibility(&event);
#else
    Q_UNUSED(text);
#endif
}

void BScheduleGridView::onActiveScopeChanged(int index)
{
    if (QAbstractButton *button = m_scopeGroup->button(index))
        button->setChecked(true);
    updateSelectionLabel();
}

void BScheduleGridView::updateHeader()
{
    const BSchedule &schedule = m_model->loadedSchedule();
    m_title->setText(tr("Schedule %1").arg(schedule.name));
    m_title->setToolTip(schedule.description);

    QString usage;
    if (!m_jobsKnown)
        usage = tr("(jobs not loaded yet)");
    else if (m_jobs.isEmpty())
        usage = tr("Not used by any job");
    else
        usage = tr("Used by %n job(s)", "", int(m_jobs.size()));
    m_usage->setText(usage);
    m_usage->setToolTip(m_jobs.isEmpty() ? QString() : m_jobs.join(QLatin1Char('\n')));

    QStringList notices;
    if (!schedule.enabled)
        notices.append(tr("Disabled: the Director does not run it."));
    QString reason;
    if (!m_model->canBeWritten(&reason)) {
        // U+26A0, the warning sign
        notices.append(QStringLiteral("\u26A0 ")
                       + tr("Contains %n Run line(s) that are not understood. The schedule is locked: "
                            "it cannot be edited or written here. Change it in its configuration file "
                            "on the Director.", "", int(m_model->notUnderstood().size())));
    }
    m_notice->setText(notices.join(QLatin1Char(' ')));
    m_notice->setToolTip(reason);
    m_notice->setVisible(!notices.isEmpty());
}

void BScheduleGridView::rebuildScopeBar()
{
    const QList<QAbstractButton *> old = m_scopeGroup->buttons();
    const bool hadFocus = m_scopeBar->isAncestorOf(focusWidget()) && focusWidget()->hasFocus();
    for (QAbstractButton *button : old) {
        m_scopeGroup->removeButton(button);
        m_scopeLayout->removeWidget(button);
        // Not at once: the change may come from the menu or a key of the button
        button->hide();
        button->deleteLater();
    }

    const QList<BGridScopeInfo> &scopes = m_model->scopes();
    for (int i = 0; i < scopes.size(); ++i) {
        const BGridScopeInfo &info = scopes[i];
        auto *button = new QToolButton(m_scopeBar);
        button->setCheckable(true);
        button->setAutoRaise(false);
        button->setFocusPolicy(Qt::StrongFocus);
        const QString label = BScheduleGridText::scopeLabel(info.scope);
        button->setText(tr("%1 (%2)").arg(label).arg(info.runCount));
        const bool removable = info.runCount == 0 && !info.scope.isEveryWeek();
        button->setToolTip(info.runCount == 0
            ? (removable ? tr("%1: no Run line in this scope. Del or the context menu removes it.").arg(label)
                         : tr("%1: no Run line in this scope").arg(label))
            : tr("%1: %n Run line(s) in this scope", "", info.runCount).arg(label));
        button->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(button, &QToolButton::customContextMenuRequested, this, [this, button](const QPoint &pos) {
            showScopeMenu(button, button->mapToGlobal(pos));
        });
        button->installEventFilter(this);
        button->setAccessibleName(tr("Scope %1, %n Run line(s)", "", info.runCount).arg(label));
        // The theme draws these as tabs and dims a scope without lines
        button->setProperty("scopeButton", true);
        button->setProperty("scopeEmpty", info.runCount == 0);
        button->setChecked(i == m_model->activeScopeIndex());
        m_scopeGroup->addButton(button, i);
        // In front of "+" and the stretch at the end
        m_scopeLayout->insertWidget(m_scopeLayout->count() - 2, button);
    }
    // The keyboard goes along the bar and then to "+"
    QWidget *before = nullptr;
    for (int i = 0; i < scopes.size(); ++i) {
        QWidget *button = m_scopeGroup->button(i);
        if (before)
            setTabOrder(before, button);
        before = button;
    }
    if (before)
        setTabOrder(before, m_addScope);
    if (hadFocus) {
        if (QAbstractButton *button = m_scopeGroup->checkedButton())
            button->setFocus();
    }
    rebuildLegend();
}

void BScheduleGridView::rebuildLegend()
{
    // The brushes are in the palette; here are the other marks that occur
    const QList<BGridScopeInfo> &scopes = m_model->scopes();
    QList<BScheduleGridLegend::Item> items;
    const QList<BGridBrush> brushes = m_model->brushes();
    if (scopes.size() > 1 && !brushes.isEmpty()) {
        BScheduleGridLegend::Item shared;
        shared.kind = BScheduleGridLegend::Ghost;
        shared.run.overrides.level = brushes.first().overrides.level;
        shared.run.sharesDatesWithActiveScope = true;
        shared.text = tr("other scope, may run on the same day");
        items.append(shared);
        BScheduleGridLegend::Item never = shared;
        never.run.sharesDatesWithActiveScope = false;
        never.text = tr("other scope, never on the same day");
        items.append(never);
    }
    bool withoutTime = false;
    for (int row = 0; row < BGridCellSet::Rows && !withoutTime; ++row) {
        for (int column = 0; column < BGridCellSet::Columns && !withoutTime; ++column) {
            const BGridCell cell = m_model->cell(row, column);
            for (const BGridRun &run : cell.inScope + cell.outOfScope)
                withoutTime = withoutTime || run.withoutTime;
        }
    }
    if (withoutTime) {
        BScheduleGridLegend::Item item;
        item.kind = BScheduleGridLegend::Warning;
        item.text = tr("no time in the Run line: runs every hour at :00");
        items.append(item);
    }
    if (!m_model->calendarRules().isEmpty()) {
        BScheduleGridLegend::Item item;
        item.kind = BScheduleGridLegend::Rule;
        item.mark = BScheduleGridText::ruleTag(0);
        item.level = m_model->calendarRules().first().spec.overrides.level;
        item.text = tr("calendar rule, not a cell");
        items.append(item);
    }
    m_legend->setItems(items);
}

void BScheduleGridView::rebuildRunLines()
{
    const QSignalBlocker blocker(m_runLines);
    m_runLines->clear();

    const QList<BGridScopeInfo> &scopes = m_model->scopes();
    const QList<BGridCalendarRule> &rules = m_model->calendarRules();
    const QIcon warning = QIcon::fromTheme(QStringLiteral("dialog-warning"),
                                           style()->standardIcon(QStyle::SP_MessageBoxWarning));
    const QFont mono = BUiScale::scaled(font(), BUiScale::Normal, BUiScale::Monospace);

    for (const BGridRunLineText &line : m_model->runLines()) {
        auto *item = new QTreeWidgetItem(m_runLines);
        item->setData(0, RunIndexRole, line.runIndex);
        item->setData(0, RuleIndexRole, -1);
        item->setText(1, line.text);
        item->setFont(1, mono);

        // What the edits did to the line
        QString number = line.runIndex >= 0 ? QString::number(line.runIndex + 1) : QString();
        switch (line.state) {
        case BGridRunLineText::Untouched:
            break;
        case BGridRunLineText::Changed:
            number += QLatin1Char(' ') + tr("changed");
            break;
        case BGridRunLineText::New:
            number += QLatin1Char(' ') + tr("new");
            break;
        case BGridRunLineText::Removed: {
            number = tr("removed");
            QFont struck = mono;
            struck.setStrikeOut(true);
            item->setFont(1, struck);
            break;
        }
        }
        item->setText(0, number);

        QString tip = line.text;
        if (line.respelled) {
            tip += QLatin1Char('\n')
                   + tr("Reported by the Director as \"%1\", a spelling it does not read back.")
                         .arg(line.originalText);
        }

        if (!line.understood) {
            // Marked as the schedule list marks a schedule with such a line
            item->setIcon(0, warning);
            item->setText(2, tr("not understood: %1").arg(line.error));
            tip += QLatin1Char('\n') + tr("Not understood and not shown in the grid: %1").arg(line.error)
                   + QLatin1Char('\n') + tr("This schedule cannot be written by the application.");
        } else if (line.runIndex >= 0) {
            int rule = -1;
            for (int i = 0; i < rules.size(); ++i) {
                if (rules[i].runIndex == line.runIndex)
                    rule = i;
            }
            if (rule >= 0) {
                item->setData(0, RuleIndexRole, rule);
                item->setText(2, tr("calendar rule %1").arg(BScheduleGridText::ruleTag(rule)));
            } else {
                int scope = -1;
                m_model->cellsOfRun(line.runIndex, &scope);
                if (scope >= 0) {
                    item->setText(2, tr("scope %1")
                                         .arg(BScheduleGridText::scopeLabel(scopes.value(scope).scope)));
                }
            }
        }
        for (int column = 0; column < 3; ++column)
            item->setToolTip(column, tip);
    }
    for (int column = 0; column < 3; ++column)
        m_runLines->resizeColumnToContents(column);
    updateRuleButtons();
}

void BScheduleGridView::rebuildRules()
{
    const QSignalBlocker blocker(m_rules);
    m_rules->clear();
    const QList<BGridCalendarRule> &rules = m_model->calendarRules();
    for (int i = 0; i < rules.size(); ++i) {
        auto *item = new QTreeWidgetItem(m_rules);
        item->setData(0, RuleIndexRole, i);
        item->setText(0, BScheduleGridText::ruleTag(i));
        const QString reading = tr("%1 %2").arg(
            BScheduleGridText::levelName(rules[i].spec.overrides.level),
            BScheduleGridText::ruleReading(rules[i]));
        item->setText(1, reading);
        item->setText(2, rules[i].text);
        const QString overrides = BScheduleGridText::overridesText(rules[i].spec.overrides);
        QString tip = reading + QLatin1Char('\n') + rules[i].text;
        if (!overrides.isEmpty())
            tip += QLatin1Char('\n') + overrides;
        for (int column = 0; column < 3; ++column)
            item->setToolTip(column, tip);
    }
    for (int column = 0; column < 3; ++column)
        m_rules->resizeColumnToContents(column);
    m_rulesBox->setVisible(!rules.isEmpty());
    updateRuleButtons();
}

void BScheduleGridView::updateCalendarRange()
{
    m_calendarRange->setText(m_calendar->rangeText());
}

// --- Selection -------------------------------------------------------------------

QString BScheduleGridView::selectionText() const
{
    const BGridSelection &selection = m_model->selection();
    if (selection.isEmpty()) {
        return tr("Nothing selected. Click a cell or drag over cells; click a day or an hour "
                  "to select all of it.");
    }

    // How many of the selected cells contain each kind of run
    struct Kind
    {
        QString text;
        int cells = 0;
    };
    QList<Kind> inScope;
    QList<Kind> outOfScope;
    auto count = [](QList<Kind> &kinds, const QString &text) {
        for (Kind &kind : kinds) {
            if (kind.text == text) {
                ++kind.cells;
                return;
            }
        }
        kinds.append(Kind{text, 1});
    };
    const QList<BGridScopeInfo> &scopes = m_model->scopes();
    for (const BGridPos &pos : selection.cells().cells()) {
        const BGridCell cell = m_model->cell(pos.row, pos.column);
        for (const BGridRun &run : cell.inScope)
            count(inScope, BScheduleGridText::brushText(run.brush()));
        for (const BGridRun &run : cell.outOfScope) {
            count(outOfScope, tr("%1 (%2)").arg(BScheduleGridText::brushText(run.brush()),
                                                BScheduleGridText::scopeLabel(
                                                    scopes.value(run.scopeIndex).scope)));
        }
    }
    auto listed = [this](const QList<Kind> &kinds) {
        QStringList parts;
        for (const Kind &kind : kinds)
            parts.append(tr("%1 in %n cell(s)", "", kind.cells).arg(kind.text));
        return parts.join(QLatin1String("; "));
    };

    QString text = tr("%n cell(s) selected:", "", selection.count()) + QLatin1Char(' ');
    text += inScope.isEmpty() ? tr("no run in this scope") : listed(inScope);
    if (!outOfScope.isEmpty()) {
        // Painting is allowed where another scope has a run: that run stays
        text += QLatin1String(". ") + tr("Other scopes: %1").arg(listed(outOfScope))
                + QLatin1String(". ") + tr("They stay when this scope is painted or erased");
    }
    return text + QLatin1Char('.');
}

void BScheduleGridView::updateSelectionLabel()
{
    m_selectionLabel->setText(selectionText());
}

// --- Lists -----------------------------------------------------------------------

void BScheduleGridView::onRunLineChosen(QTreeWidgetItem *item)
{
    if (!item)
        return;
    selectRunLine(item->data(0, RunIndexRole).toInt());
}

void BScheduleGridView::selectRunLine(int runIndex)
{
    // The row of the line, in case the request did not come from the list
    QTreeWidgetItem *row = nullptr;
    for (int i = 0; i < m_runLines->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = m_runLines->topLevelItem(i);
        if (runIndex >= 0 && item->data(0, RunIndexRole).toInt() == runIndex)
            row = item;
    }
    if (row && m_runLines->currentItem() != row) {
        const QSignalBlocker blocker(m_runLines);
        m_runLines->setCurrentItem(row);
    }

    const int rule = row ? row->data(0, RuleIndexRole).toInt() : -1;
    {
        const QSignalBlocker blocker(m_rules);
        m_rules->setCurrentItem(rule >= 0 ? m_rules->topLevelItem(rule) : nullptr);
    }
    m_grid->setHighlightedRule(rule);
    updateRuleButtons();

    int scope = -1;
    if (runIndex >= 0 && rule < 0)
        m_model->cellsOfRun(runIndex, &scope);
    if (scope >= 0) {
        // A line of another scope: that scope is shown
        m_model->setActiveScope(scope);
        m_grid->setHighlightedRun(runIndex);
    } else {
        m_grid->setHighlightedRun(-1);
    }
}

void BScheduleGridView::onRuleChosen(QTreeWidgetItem *item)
{
    if (!item)
        return;
    showRuleInLists(item->data(0, RuleIndexRole).toInt());
}

void BScheduleGridView::showRuleInLists(int ruleIndex)
{
    const QList<BGridCalendarRule> &rules = m_model->calendarRules();
    if (ruleIndex < 0 || ruleIndex >= rules.size())
        return;
    selectRunLine(rules[ruleIndex].runIndex);
}

// --- Scopes and calendar rules ---------------------------------------------------------

BScheduleScopeDialog *BScheduleGridView::openScopeDialog()
{
    if (!m_hasSchedule)
        return nullptr;
    // One that was closed is on its way out
    if (m_scopeDialog && m_scopeDialog->isVisible()) {
        m_scopeDialog->raise();
        m_scopeDialog->activateWindow();
        return m_scopeDialog;
    }
    auto *dialog = new BScheduleScopeDialog(m_model->scopes(), window());
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowModality(Qt::WindowModal);
    connect(dialog, &QDialog::accepted, this, [this, dialog]() {
        if (m_editor->addScope(dialog->scope()) >= 0)
            m_grid->setFocus();
    });
    m_scopeDialog = dialog;
    dialog->open();
    return dialog;
}

bool BScheduleGridView::removeScope(int index)
{
    return m_editor->removeScope(index);
}

void BScheduleGridView::showScopeMenu(QAbstractButton *button, const QPoint &globalPos)
{
    const int index = m_scopeGroup->id(button);
    const QList<BGridScopeInfo> &scopes = m_model->scopes();
    if (index < 0 || index >= scopes.size())
        return;
    const BGridScopeInfo info = scopes[index];
    QMenu menu(this);
    QAction *remove = menu.addAction(tr("Remove the scope \"%1\"").arg(BScheduleGridText::scopeLabel(info.scope)));
    // Only a scope without Run lines; "Every week" is always there
    remove->setEnabled(info.runCount == 0 && !info.scope.isEveryWeek());
    QAction *add = menu.addAction(tr("Add scope..."));
    const QAction *chosen = menu.exec(globalPos);
    if (chosen == remove)
        removeScope(index);
    else if (chosen == add)
        openScopeDialog();
}

int BScheduleGridView::currentRule() const
{
    const int rule = m_grid->highlightedRule();
    return rule >= 0 && rule < m_model->calendarRules().size() ? rule : -1;
}

void BScheduleGridView::updateRuleButtons()
{
    const bool chosen = currentRule() >= 0;
    const bool editable = m_hasSchedule && !isLocked();
    m_addRule->setEnabled(editable);
    m_editRule->setEnabled(chosen && editable);
    m_removeRule->setEnabled(chosen && editable);
}

BRunRuleDialog *BScheduleGridView::openRuleDialog(int ruleIndex)
{
    const QList<BGridCalendarRule> &rules = m_model->calendarRules();
    if (!m_hasSchedule || ruleIndex >= rules.size())
        return nullptr;
    // One that was closed is on its way out
    if (m_ruleDialog && m_ruleDialog->isVisible()) {
        m_ruleDialog->raise();
        m_ruleDialog->activateWindow();
        return m_ruleDialog;
    }
    auto *dialog = new BRunRuleDialog(window());
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setFirstDayOfWeek(m_model->firstDayOfWeek());
    if (ruleIndex >= 0) {
        dialog->setWindowTitle(tr("Calendar rule %1").arg(BScheduleGridText::ruleTag(ruleIndex)));
        dialog->setSpec(rules[ruleIndex].spec);
    } else {
        dialog->setWindowTitle(tr("New calendar rule"));
    }
    connect(dialog, &QDialog::accepted, this, [this, dialog, ruleIndex]() {
        // The form is window modal and closes with a new schedule, so the
        // rule is still the one it was opened for
        if (ruleIndex >= 0) {
            if (m_editor->changeCalendarRule(ruleIndex, dialog->spec()))
                showRuleInLists(ruleIndex);
        } else if (m_editor->addCalendarRule(dialog->spec())) {
            showRuleInLists(int(m_model->calendarRules().size()) - 1);
        }
    });
    m_ruleDialog = dialog;
    dialog->open();
    return dialog;
}

bool BScheduleGridView::removeRule(int ruleIndex)
{
    if (ruleIndex < 0 || ruleIndex >= m_model->calendarRules().size())
        return false;
    return m_editor->removeCalendarRule(ruleIndex);
}

void BScheduleGridView::showRuleMenu(const QPoint &globalPos)
{
    const int rule = currentRule();
    QMenu menu(this);
    QAction *edit = menu.addAction(tr("Edit calendar rule..."));
    QAction *remove = menu.addAction(tr("Remove calendar rule"));
    edit->setEnabled(rule >= 0);
    remove->setEnabled(rule >= 0);
    menu.addSeparator();
    QAction *add = menu.addAction(tr("Add calendar rule..."));
    const QAction *chosen = menu.exec(globalPos);
    if (chosen == edit)
        openRuleDialog(rule);
    else if (chosen == remove)
        removeRule(rule);
    else if (chosen == add)
        openRuleDialog(-1);
}

void BScheduleGridView::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    // The Run lines are set in a fixed-pitch font made from this font:
    // the same again at the new size, a removed line stays struck out
    if (event->type() == QEvent::FontChange && m_runLines) {
        const QFont mono = BUiScale::scaled(font(), BUiScale::Normal, BUiScale::Monospace);
        for (int i = 0; i < m_runLines->topLevelItemCount(); ++i) {
            QTreeWidgetItem *item = m_runLines->topLevelItem(i);
            QFont font = mono;
            font.setStrikeOut(item->font(1).strikeOut());
            item->setFont(1, font);
        }
    }
}

bool BScheduleGridView::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        const int key = static_cast<QKeyEvent *>(event)->key();
        if (key == Qt::Key_Delete || key == Qt::Key_Backspace) {
            // Del on a scope without Run lines removes it; in the list of
            // calendar rules it removes the rule
            if (watched == m_rules) {
                removeRule(currentRule());
                return true;
            }
            if (auto *button = qobject_cast<QAbstractButton *>(watched)) {
                const int index = m_scopeGroup->id(button);
                if (index >= 0) {
                    removeScope(index);
                    return true;
                }
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

// --- Keyboard ----------------------------------------------------------------------

QList<QWidget *> BScheduleGridView::focusParts() const
{
    QList<QWidget *> parts;
    if (QAbstractButton *scope = m_scopeGroup->checkedButton())
        parts.append(scope);
    parts.append(m_grid);
    if (QAbstractButton *tool = m_palette->activeButton(); tool && m_palette->isEnabled())
        parts.append(tool);
    parts.append(m_runLines);
    if (m_rulesBox->isVisibleTo(this))
        parts.append(m_rules);
    parts.append(m_calendar);
    return parts;
}

void BScheduleGridView::focusNextPart(bool backwards)
{
    if (!m_hasSchedule)
        return;
    const QList<QWidget *> parts = focusParts();
    const QWidget *focus = focusWidget();
    int current = -1;
    for (int i = 0; i < parts.size(); ++i) {
        if (focus && (parts[i] == focus || parts[i]->isAncestorOf(focus)))
            current = i;
    }
    // In the scope bar and in the palette the keyboard may be on a button
    // that is not checked
    if (current < 0 && focus && m_scopeBar->isAncestorOf(focus))
        current = 0;
    if (current < 0 && focus && m_palette->isAncestorOf(focus))
        current = int(parts.indexOf(m_palette->activeButton()));
    int next = 0;
    if (current >= 0) {
        const int count = int(parts.size());
        next = (current + (backwards ? count - 1 : 1)) % count;
    }
    parts[next]->setFocus(Qt::ShortcutFocusReason);
}
