#ifndef BSCHEDULEGRIDVIEW_H
#define BSCHEDULEGRIDVIEW_H

#include <QDate>
#include <QPointer>
#include <QWidget>

#include <optional>

#include "schedules/bschedulegridwidget.h"
#include "schedules/model/bschedulegridmodel.h"

class QAbstractButton;
class QButtonGroup;
class QFrame;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSplitter;
class QStackedLayout;
class QToolButton;
class QTreeWidget;
class QTreeWidgetItem;
class BRunRuleDialog;
class BScheduleGridEditor;
class BScheduleGridLegend;
class BScheduleMiniCalendar;
class BSchedulePalette;
class BScheduleScopeDialog;

/**
 * @brief One schedule resource as a grid of weekdays and hours, to look at
 *        and to edit
 *
 * From top to bottom:
 *  - the name of the schedule, how many jobs use it, whether it is modified
 *    (with the number of changed, new and removed Run lines) and the
 *    buttons Undo, Redo and "Revert all";
 *  - a notice if a refresh from the Director brought other Run lines while
 *    there are changes: the changes are kept, and the notice offers "Keep my
 *    changes" and "Discard and reload";
 *  - the scope bar: one button per scope of the model ("Every week", "1st",
 *    "2nd-5th", "last", month-restricted ones) with the number of its Run
 *    lines; the active one is checked, one without lines is dimmed. "+" at
 *    its end adds a scope; a scope without lines is removed from the
 *    context menu of its button or with Del;
 *  - the grid (BScheduleGridWidget), the palette of brushes
 *    (BSchedulePalette) and a legend of the other marks;
 *  - a line saying what the selected cells contain, and why an edit could
 *    not be made;
 *  - side by side: "Run lines" (every line in schedule order, marked as
 *    changed, new or removed; choosing one outlines its cells and switches
 *    to its scope; lines that are not understood are listed with the reason
 *    and a warning sign), "Calendar rules" (lines by day of the month or
 *    week of the year with a plain-language reading; hidden if there are
 *    none) and a calendar of five weeks with the levels that run on each
 *    date, which follows every edit.
 *
 * Calendar rules are edited in a form (BRunRuleDialog): the buttons below
 * the Run lines add, edit and remove one; Enter or a double click on a rule
 * in either list or a double click on its mark in the grid opens the form,
 * Del in the list of rules removes one.
 *
 * Keyboard: F6 and Shift+F6 move between scope bar, grid, palette, the
 * lists and the calendar. Ctrl+Z undoes, Ctrl+Shift+Z and Ctrl+Y redo,
 * wherever in the view the keyboard is.
 *
 * The view owns its model and its editor. Nothing here writes a schedule
 * to a Director: an edit ends at the Run lines of the model. "Save..." asks
 * the schedule tab to write the changes (saveRequested(); BSchedulePersistence
 * checks, shows and confirms), "Delete..." asks it to delete the schedule
 * (deleteRequested()).
 *
 * A schedule with a Run line that is not understood is locked: the editor
 * refuses every edit, the palette, the scope and rule buttons and "Save..."
 * are disabled, and the notice says why.
 */
class BScheduleGridView : public QWidget
{
    Q_OBJECT

public:
    explicit BScheduleGridView(QWidget *parent = nullptr);

    /// Shows a schedule; scope, selection, highlights and the undo stack
    /// start afresh. Changes that were made are gone: see offerSchedule().
    void setSchedule(const BSchedule &schedule);

    /**
     * @brief Shows a schedule unless that would throw changes away
     *
     * What a refresh and a choice in the list of schedules call. Without
     * changes this is setSchedule(). With changes the schedule on display
     * stays as it is and a notice offers the two ways on: keepChanges() and
     * discardAndReload(). Other Run lines for the schedule on display that
     * were declined are not asked about again; another schedule is, every
     * time it is offered.
     *
     * @return true if the schedule is shown now
     */
    bool offerSchedule(const BSchedule &schedule);

    /// Is the notice up, waiting for the answer?
    bool hasPendingSchedule() const { return m_pending.has_value(); }
    /// "Keep my changes": the schedule that was offered is not shown
    void keepChanges();
    /// "Discard and reload": the schedule that was offered is shown
    void discardAndReload();

    /// Have the Run lines been changed?
    bool isModified() const;
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
    BScheduleGridEditor *editor() const { return m_editor; }
    BScheduleGridWidget *grid() const { return m_grid; }
    BSchedulePalette *palette() const { return m_palette; }
    BScheduleMiniCalendar *calendar() const { return m_calendar; }
    QTreeWidget *runLineList() const { return m_runLines; }
    QTreeWidget *ruleList() const { return m_rules; }
    QList<QAbstractButton *> scopeButtons() const;
    QString titleText() const;
    QString usageText() const;

    /// "Modified: 1 changed, 2 new, 1 removed Run line(s) ..."; empty if not
    QString modifiedText() const;
    /// What stands in the way of writing the schedule; empty if nothing
    QString noticeText() const;
    /// The notice about a schedule that was offered; empty if there is none
    QString pendingText() const;
    /// Why the last edit could not be made; empty after one that worked
    QString editErrorText() const;

    QAbstractButton *undoButton() const;
    QAbstractButton *redoButton() const;
    QAbstractButton *revertButton() const;
    QAbstractButton *keepChangesButton() const;
    QAbstractButton *discardButton() const;
    QAbstractButton *addScopeButton() const;
    QAbstractButton *addRuleButton() const;
    QAbstractButton *editRuleButton() const;
    QAbstractButton *removeRuleButton() const;
    QAbstractButton *saveButton() const;
    QAbstractButton *deleteButton() const;

    /// The schedule has a Run line that is not understood: nothing can be edited
    bool isLocked() const;

    /**
     * @brief Whether "Save..." and "Delete..." can be used at all
     *
     * The schedule tab sets this from the capabilities of the Director. If
     * not, the buttons stay enabled and their click is still sent: the tab
     * answers with the capability notice.
     */
    void setWritable(bool canSave, bool canDelete);

    // --- Scopes and calendar rules: what the buttons and menus do -----------
    //
    // The forms are window modal and not waited for; accepting one makes
    // the edit.

    /// Opens "Add scope"
    BScheduleScopeDialog *openScopeDialog();
    /// Removes a scope without Run lines. @return false if it has some
    bool removeScope(int index);

    /// The calendar rule that is chosen in the lists; -1 for none
    int currentRule() const;
    /// Opens the form for a calendar rule; -1 for a new one
    BRunRuleDialog *openRuleDialog(int ruleIndex);
    bool removeRule(int ruleIndex);

    /// What the line below the grid says about the selected cells
    QString selectionText() const;

    /// What choosing a line under "Run lines" does: outlines its cells,
    /// switching to its scope, or marks its calendar rule
    void selectRunLine(int runIndex);

    /// F6: the keyboard goes to the next part (or the one before)
    void focusNextPart(bool backwards = false);

signals:
    void modifiedChanged(bool modified);
    /// "Keep my changes" was the answer: the schedule of this name stays
    void changesKept(const QString &scheduleName);
    /// "Discard" was the answer: the schedule that was offered is on display
    void pendingScheduleShown(const QString &scheduleName);
    /// "Save...": write the changes of the schedule on display
    void saveRequested();
    /// "Delete...": delete the schedule on display
    void deleteRequested();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    void buildUi();
    void rebuildScopeBar();
    void rebuildLegend();
    void rebuildRunLines();
    void rebuildRules();
    void updateHeader();
    void updateModified();
    void updatePendingNotice();
    void updateRuleButtons();
    void showScopeMenu(QAbstractButton *button, const QPoint &globalPos);
    void showRuleMenu(const QPoint &globalPos);
    void announce(const QString &text);
    void updateSelectionLabel();
    void updateCalendarRange();
    void onScheduleReset();
    void onActiveScopeChanged(int index);
    void onRunLineChosen(QTreeWidgetItem *item);
    void onRuleChosen(QTreeWidgetItem *item);
    void showRuleInLists(int ruleIndex);
    QList<QWidget *> focusParts() const;

    BScheduleGridModel *m_model;
    BScheduleGridEditor *m_editor;
    bool m_hasSchedule = false;
    bool m_wasModified = false;
    std::optional<BSchedule> m_pending;     ///< Offered while there were changes
    std::optional<BSchedule> m_declined;    ///< The schedule on display as the Director reports it now: declined
    QStringList m_jobs;
    bool m_jobsKnown = false;

    QStackedLayout *m_stack = nullptr;
    QLabel *m_placeholder = nullptr;
    QLabel *m_title = nullptr;
    QLabel *m_usage = nullptr;
    QLabel *m_modified = nullptr;
    QPushButton *m_undo = nullptr;
    QPushButton *m_redo = nullptr;
    QPushButton *m_revert = nullptr;
    QPushButton *m_save = nullptr;
    QPushButton *m_delete = nullptr;
    bool m_canSave = false;
    bool m_canDelete = false;
    void updateLock();
    QLabel *m_notice = nullptr;
    QFrame *m_pendingBar = nullptr;
    QLabel *m_pendingLabel = nullptr;
    QPushButton *m_keepChanges = nullptr;
    QPushButton *m_discard = nullptr;
    QToolButton *m_addScope = nullptr;
    BSchedulePalette *m_palette = nullptr;
    QLabel *m_editError = nullptr;
    QPushButton *m_addRule = nullptr;
    QPushButton *m_editRule = nullptr;
    QPushButton *m_removeRule = nullptr;
    QPointer<BScheduleScopeDialog> m_scopeDialog;
    QPointer<BRunRuleDialog> m_ruleDialog;
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
