#ifndef BSCHEDULEGRIDVIEW_H
#define BSCHEDULEGRIDVIEW_H

#include <QDate>
#include <QWidget>

#include "schedules/bschedulegridwidget.h"
#include "schedules/model/bschedulegridmodel.h"

class QAbstractButton;
class QButtonGroup;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QSplitter;
class QStackedLayout;
class QToolButton;
class QTreeWidget;
class QTreeWidgetItem;
class BScheduleGridLegend;
class BScheduleMiniCalendar;

/**
 * @brief One schedule resource as a grid of weekdays and hours, read-only
 *
 * From top to bottom:
 *  - the name of the schedule and how many jobs use it;
 *  - the scope bar: one button per scope of the model ("Every week", "1st",
 *    "2nd-5th", "last", month-restricted ones) with the number of its Run
 *    lines; the active one is checked, one without lines is dimmed;
 *  - the grid (BScheduleGridWidget) and its legend, which spells out the
 *    brushes with their tags and the marks;
 *  - a line saying what the selected cells contain;
 *  - side by side: "Run lines" (every line in schedule order; choosing one
 *    outlines its cells and switches to its scope; lines that are not
 *    understood are listed with the reason and a warning sign), "Calendar
 *    rules" (lines by day of the month or week of the year with a
 *    plain-language reading; hidden if there are none) and a calendar of
 *    five weeks with the levels that run on each date.
 *
 * F6 and Shift+F6 move the keyboard between scope bar, grid, the lists and
 * the calendar.
 *
 * The view owns its model. Nothing here edits or writes a schedule.
 */
class BScheduleGridView : public QWidget
{
    Q_OBJECT

public:
    explicit BScheduleGridView(QWidget *parent = nullptr);

    /// Shows a schedule; scope, selection and highlights start afresh
    void setSchedule(const BSchedule &schedule);
    /// Shows the note that no schedule is chosen
    void clear();
    bool hasSchedule() const { return m_hasSchedule; }

    /**
     * @brief Who uses the schedule
     * @param jobs  Names of the jobs that reference it
     * @param known false: the jobs of the Director are not loaded yet
     */
    void setUsage(const QStringList &jobs, bool known);

    void setLevelColorSource(const BScheduleGridWidget::LevelColorSource &source);

    /// The date the calendar takes as today (tests, pictures)
    void setToday(const QDate &today);

    BScheduleGridModel *model() const { return m_model; }
    BScheduleGridWidget *grid() const { return m_grid; }
    BScheduleMiniCalendar *calendar() const { return m_calendar; }
    QTreeWidget *runLineList() const { return m_runLines; }
    QTreeWidget *ruleList() const { return m_rules; }
    QList<QAbstractButton *> scopeButtons() const;
    QString titleText() const;
    QString usageText() const;

    /// What the line below the grid says about the selected cells
    QString selectionText() const;

    /// What choosing a line under "Run lines" does: outlines its cells,
    /// switching to its scope, or marks its calendar rule
    void selectRunLine(int runIndex);

    /// F6: the keyboard goes to the next part (or the one before)
    void focusNextPart(bool backwards = false);

private:
    void buildUi();
    void rebuildScopeBar();
    void rebuildLegend();
    void rebuildRunLines();
    void rebuildRules();
    void updateHeader();
    void updateSelectionLabel();
    void updateCalendarRange();
    void onScheduleReset();
    void onActiveScopeChanged(int index);
    void onRunLineChosen(QTreeWidgetItem *item);
    void onRuleChosen(QTreeWidgetItem *item);
    void showRuleInLists(int ruleIndex);
    QList<QWidget *> focusParts() const;

    BScheduleGridModel *m_model;
    bool m_hasSchedule = false;
    QStringList m_jobs;
    bool m_jobsKnown = false;

    QStackedLayout *m_stack = nullptr;
    QLabel *m_placeholder = nullptr;
    QLabel *m_title = nullptr;
    QLabel *m_usage = nullptr;
    QLabel *m_notice = nullptr;
    QWidget *m_scopeBar = nullptr;
    QHBoxLayout *m_scopeLayout = nullptr;
    QButtonGroup *m_scopeGroup = nullptr;
    BScheduleGridWidget *m_grid = nullptr;
    BScheduleGridLegend *m_legend = nullptr;
    QLabel *m_selectionLabel = nullptr;
    QSplitter *m_mainSplitter = nullptr;
    QSplitter *m_listSplitter = nullptr;
    QGroupBox *m_runLinesBox = nullptr;
    QTreeWidget *m_runLines = nullptr;
    QGroupBox *m_rulesBox = nullptr;
    QTreeWidget *m_rules = nullptr;
    QGroupBox *m_calendarBox = nullptr;
    BScheduleMiniCalendar *m_calendar = nullptr;
    QLabel *m_calendarRange = nullptr;
};

#endif // BSCHEDULEGRIDVIEW_H
