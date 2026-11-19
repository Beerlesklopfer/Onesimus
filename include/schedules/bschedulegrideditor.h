#ifndef BSCHEDULEGRIDEDITOR_H
#define BSCHEDULEGRIDEDITOR_H

#include <QList>
#include <QObject>
#include <QString>

#include <optional>

#include "schedules/model/bschedulegridmodel.h"

class QUndoStack;

/**
 * @brief Editing a schedule in the grid: the tool in hand and the undo stack
 *
 * Sits between the widgets and BScheduleGridModel. The grid, the palette
 * and the forms call the edits here; each one that changes something is one
 * step of the undo stack. Nothing is written to a Director: an edit ends at
 * the Run lines of the model, which says whether it is modified.
 *
 * The tool is what Space and painting apply: one of the brushes or the
 * eraser. The brushes on offer are those of the schedule as it was loaded
 * (for a schedule without a line of the grid: Full, Differential and
 * Incremental at :00) and every brush that came up since: one made in the
 * "New brush" form, one a level key asked for, one an undo brought back.
 * They keep their place, so the tag of a brush (A, B, ...) does not change
 * while a schedule is edited.
 *
 * Painting replaces the other runs of the active scope in a cell, as the
 * Logon Hours dialog does, unless keepOtherRuns() is set: then the brush is
 * added to what the cell has, which is how a cell gets two runs in one
 * hour. Runs of other scopes, calendar rules and lines that are not
 * understood are never changed by painting or erasing.
 *
 * Undo and redo also show the scope the step was made in. Adding and
 * removing a scope are steps of their own.
 *
 * A schedule with a Run line that is not understood is locked (decision of
 * the maintainer, 19.11.2026): every edit fails with a message, so that
 * nothing of such a schedule is changed and nothing of it is written.
 */
class BScheduleGridEditor : public QObject
{
    Q_OBJECT

public:
    enum Tool { Brush, Eraser };

    explicit BScheduleGridEditor(BScheduleGridModel *model, QObject *parent = nullptr);

    BScheduleGridModel *model() const { return m_model; }
    QUndoStack *undoStack() const { return m_undo; }

    // --- The tool ----------------------------------------------------------

    const QList<BGridBrush> &brushes() const { return m_brushes; }

    /// Offers a brush. @return its index (also if it was there already)
    int addBrush(const BGridBrush &brush);

    Tool tool() const { return m_tool; }
    /// The index of the brush in hand; also kept while the eraser is in hand
    int activeBrushIndex() const { return m_activeBrush; }
    /// The brush in hand; empty while the eraser is, or without any brush
    std::optional<BGridBrush> activeBrush() const;

    void setActiveBrush(int index);
    /// Takes this brush in hand, offering it first if it is new
    void setActiveBrush(const BGridBrush &brush);
    void setEraser();

    /// "Full at :05, pool Monthly" or "Eraser"
    QString toolText() const;

    /**
     * @brief The brush a level key (F, D, I, V) stands for
     *
     * The level alone, at the minute of the brush in hand: the offered
     * brush that says exactly that, otherwise a new one.
     */
    BGridBrush levelBrush(const QString &level) const;

    /// The level of a key: F Full, D Differential, I Incremental,
    /// V VirtualFull; empty for every other key
    static QString levelOfKey(int key);

    // --- How painting works ------------------------------------------------

    /// Dragging over cells applies the tool instead of selecting
    bool paintMode() const { return m_paintMode; }
    void setPaintMode(bool on);

    /// Painting adds the brush to the runs a cell has instead of replacing them
    bool keepOtherRuns() const { return m_keepOtherRuns; }
    void setKeepOtherRuns(bool on);

    // --- Edits: each one that changes something is one undo step ------------
    //
    // Cells are in rows of the view and belong to the active scope. An edit
    // that changes nothing (the cells have the brush already) returns true
    // and leaves no step. false: nothing was changed, see lastError().

    bool applyTool(const BGridCellSet &cells);
    bool applyToolToSelection();
    bool paint(const BGridCellSet &cells, const BGridBrush &brush);
    /// As paint(), but the other runs of the cells stay whatever keepOtherRuns() says
    bool paintBeside(const BGridCellSet &cells, const BGridBrush &brush);
    bool erase(const BGridCellSet &cells);
    bool eraseSelection();

    /// A level key: takes levelBrush() in hand and paints the selection with it
    bool paintLevel(const QString &level);

    /// Adds a scope and shows it. @return its index, -1 if it is not valid.
    /// A scope that exists already is only shown.
    int addScope(const BGridScope &scope);
    /// Removes a scope without Run lines that is not "every week"
    bool removeScope(int index);

    bool addCalendarRule(const BRunSpec &spec);
    /// @param ruleIndex an index into BScheduleGridModel::calendarRules()
    bool changeCalendarRule(int ruleIndex, const BRunSpec &spec);
    bool removeCalendarRule(int ruleIndex);

    /// The schedule has a Run line that is not understood: no edit is made
    bool isLocked() const;

    /// Why the last edit failed; empty after one that worked
    QString lastError() const { return m_lastError; }

    void undo();
    void redo();
    /// Undoes every step. They can be redone.
    void revertAll();

signals:
    void brushesChanged();
    /// Another tool is in hand
    void toolChanged();
    void paintModeChanged(bool on);
    void keepOtherRunsChanged(bool on);

    /// An edit, an undo or a redo changed the schedule; @p what names it
    void edited(const QString &what);
    void editFailed(const QString &message);

private:
    bool pushChange(const BGridChange &change, const QString &text, bool inActiveScope);
    /// isLocked(): says so (editFailed) and returns true
    bool refuseLocked();
    void onScheduleReset();
    void collectBrushes();
    QString cellCountText(const BGridCellSet &cells) const;

    BScheduleGridModel *m_model;
    QUndoStack *m_undo;

    QList<BGridBrush> m_brushes;
    Tool m_tool = Brush;
    int m_activeBrush = -1;
    bool m_paintMode = false;
    bool m_keepOtherRuns = false;
    QString m_lastError;
};

#endif // BSCHEDULEGRIDEDITOR_H
