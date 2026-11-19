/**
 * @file bschedulegridview.cpp
 * @brief One schedule resource as a grid of weekdays and hours, read-only
 */

#include "schedules/bschedulegridview.h"

#include "schedules/bscheduleminicalendar.h"
#include "schedules/model/bschedulegridtext.h"

#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>
#include <QPainter>
#include <QShortcut>
#include <QSplitter>
#include <QStackedLayout>
#include <QStyle>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace {

// Data of an item of the "Run lines" list
const int RunIndexRole = Qt::UserRole;
const int RuleIndexRole = Qt::UserRole + 1;

} // namespace

/**
 * @brief The legend below the grid
 *
 * Every brush of the schedule drawn as the grid draws it, with its text
 * ("Full at :00, pool Monthly"), then the marks that occur. The items flow
 * into as many rows as the width needs.
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
    QSize sizeHint() const override { return QSize(400, heightForWidth(qMax(400, width()))); }
    QSize minimumSizeHint() const override { return QSize(120, RowHeight); }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QList<QRect> places;
        layoutItems(width(), &places);
        QPainter painter(this);
        const QColor text = palette().color(QPalette::Text);
        for (int i = 0; i < m_items.size(); ++i) {
            const Item &item = m_items[i];
            const QRect swatch(places[i].left(), places[i].top() + (RowHeight - SwatchHeight) / 2,
                               SwatchWidth, SwatchHeight);
            switch (item.kind) {
            case Run:
                m_grid->paintRun(painter, swatch, item.run, true);
                break;
            case Ghost:
                m_grid->paintRun(painter, swatch, item.run, false);
                break;
            case Warning:
                m_grid->paintWarningMark(painter, swatch.adjusted(11, 0, 0, -6), text);
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
            painter.drawText(places[i].adjusted(SwatchWidth + 5, 0, 0, 0),
                             Qt::AlignLeft | Qt::AlignVCenter, item.text);
        }
    }

private:
    static constexpr int SwatchWidth = 36;
    static constexpr int SwatchHeight = 24;
    static constexpr int RowHeight = 28;
    static constexpr int Spacing = 18;

    /// Places the items in rows; @return the height they need
    int layoutItems(int width, QList<QRect> *places) const
    {
        const QFontMetrics metrics(font());
        int x = 0;
        int y = 0;
        for (const Item &item : m_items) {
            const int itemWidth = SwatchWidth + 5 + metrics.horizontalAdvance(item.text);
            if (x > 0 && x + itemWidth > width) {
                x = 0;
                y += RowHeight;
            }
            if (places)
                places->append(QRect(x, y, itemWidth, RowHeight));
            x += itemWidth + Spacing;
        }
        return m_items.isEmpty() ? 0 : y + RowHeight;
    }

    BScheduleGridWidget *m_grid;
    QList<Item> m_items;
};

BScheduleGridView::BScheduleGridView(QWidget *parent)
    : QWidget(parent)
    , m_model(new BScheduleGridModel(this))
{
    m_model->setFirstDayOfWeek(QLocale().firstDayOfWeek());
    buildUi();

    connect(m_model, &BScheduleGridModel::scheduleReset, this, &BScheduleGridView::onScheduleReset);
    connect(m_model, &BScheduleGridModel::scopesChanged, this, &BScheduleGridView::rebuildScopeBar);
    connect(m_model, &BScheduleGridModel::activeScopeChanged,
            this, &BScheduleGridView::onActiveScopeChanged);
    connect(m_model, &BScheduleGridModel::runLinesChanged, this, &BScheduleGridView::rebuildRunLines);
    connect(m_model, &BScheduleGridModel::runLinesChanged, this, &BScheduleGridView::rebuildLegend);
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
    QFont titleFont = m_title->font();
    titleFont.setBold(true);
    titleFont.setPointSizeF(titleFont.pointSizeF() > 0 ? titleFont.pointSizeF() + 1 : 11);
    m_title->setFont(titleFont);
    headerRow->addWidget(m_title);
    m_usage = new QLabel(top);
    m_usage->setTextFormat(Qt::PlainText);
    headerRow->addWidget(m_usage);
    headerRow->addStretch(1);
    topLayout->addLayout(headerRow);

    // What stands in the way of writing the schedule; a line of its own,
    // because it can be long
    m_notice = new QLabel(top);
    m_notice->setTextFormat(Qt::PlainText);
    m_notice->setWordWrap(true);
    m_notice->setObjectName(QStringLiteral("scheduleGridNotice"));
    topLayout->addWidget(m_notice);

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
    topLayout->addWidget(m_grid, 1);

    m_legend = new BScheduleGridLegend(m_grid, top);
    m_legend->setAccessibleName(tr("Legend"));
    topLayout->addWidget(m_legend);

    m_selectionLabel = new QLabel(top);
    m_selectionLabel->setTextFormat(Qt::PlainText);
    m_selectionLabel->setWordWrap(true);
    m_selectionLabel->setObjectName(QStringLiteral("scheduleGridSelection"));
    topLayout->addWidget(m_selectionLabel);

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
                           "to a weekday and are marked in the band below the grid."));
    rulesLayout->addWidget(m_rules);
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
}

void BScheduleGridView::setSchedule(const BSchedule &schedule)
{
    m_hasSchedule = true;
    m_model->setSchedule(schedule);
    m_stack->setCurrentIndex(1);
}

void BScheduleGridView::clear()
{
    m_hasSchedule = false;
    m_jobs.clear();
    m_jobsKnown = false;
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

// --- Model changes -------------------------------------------------------------

void BScheduleGridView::onScheduleReset()
{
    m_grid->setHighlightedRun(-1);
    m_grid->setHighlightedRule(-1);
    m_grid->setCurrentCell(BGridPos{0, 0});
    rebuildScopeBar();
    rebuildRunLines();
    rebuildRules();
    updateHeader();
    updateSelectionLabel();
    m_calendar->setSchedule(m_model->schedule());
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
                       + tr("Contains %n Run line(s) that are not understood; the application "
                            "cannot write this schedule.", "", int(m_model->notUnderstood().size())));
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
        delete button;
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
        button->setToolTip(info.runCount == 0
            ? tr("%1: no Run line in this scope").arg(label)
            : tr("%1: %n Run line(s) in this scope", "", info.runCount).arg(label));
        button->setAccessibleName(tr("Scope %1, %n Run line(s)", "", info.runCount).arg(label));
        // The theme draws these as tabs and dims a scope without lines
        button->setProperty("scopeButton", true);
        button->setProperty("scopeEmpty", info.runCount == 0);
        button->setChecked(i == m_model->activeScopeIndex());
        m_scopeGroup->addButton(button, i);
        // In front of the stretch at the end
        m_scopeLayout->insertWidget(m_scopeLayout->count() - 1, button);
    }
    if (hadFocus) {
        if (QAbstractButton *button = m_scopeGroup->checkedButton())
            button->setFocus();
    }
    rebuildLegend();
}

void BScheduleGridView::rebuildLegend()
{
    // The legend names the brushes of all scopes
    const QList<BGridScopeInfo> &scopes = m_model->scopes();
    QList<BScheduleGridLegend::Item> items;
    const QList<BGridBrush> brushes = m_model->brushes();
    for (const BGridBrush &brush : brushes) {
        BScheduleGridLegend::Item item;
        item.kind = BScheduleGridLegend::Run;
        item.run.overrides = brush.overrides;
        item.run.minute = brush.minute;
        const QString tag = m_grid->brushTag(brush);
        item.text = tag.isEmpty() ? BScheduleGridText::brushText(brush)
                                  : tr("%1 = %2").arg(tag, BScheduleGridText::brushText(brush));
        items.append(item);
    }
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
    QFont mono(QStringLiteral("monospace"));
    mono.setStyleHint(QFont::Monospace);
    mono.setPointSizeF(font().pointSizeF() > 0 ? font().pointSizeF() : 10);

    for (const BGridRunLineText &line : m_model->runLines()) {
        auto *item = new QTreeWidgetItem(m_runLines);
        item->setData(0, RunIndexRole, line.runIndex);
        item->setData(0, RuleIndexRole, -1);
        item->setText(1, line.text);
        item->setFont(1, mono);

        // What an edit did to the line (nothing edits yet)
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
    if (!outOfScope.isEmpty())
        text += QLatin1String(". ") + tr("Other scopes: %1").arg(listed(outOfScope));
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

// --- Keyboard ----------------------------------------------------------------------

QList<QWidget *> BScheduleGridView::focusParts() const
{
    QList<QWidget *> parts;
    if (QAbstractButton *scope = m_scopeGroup->checkedButton())
        parts.append(scope);
    parts.append(m_grid);
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
    // In the scope bar the keyboard may be on a button that is not checked
    if (current < 0 && focus && m_scopeBar->isAncestorOf(focus))
        current = 0;
    int next = 0;
    if (current >= 0) {
        const int count = int(parts.size());
        next = (current + (backwards ? count - 1 : 1)) % count;
    }
    parts[next]->setFocus(Qt::ShortcutFocusReason);
}
