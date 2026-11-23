// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BSCHEDULEGRIDMODEL_H
#define BSCHEDULEGRIDMODEL_H

#include <QList>
#include <QObject>
#include <QString>

#include <array>
#include <optional>

#include "schedules/model/brunspec.h"

/**
 * @file bschedulegridmodel.h
 * @brief The model behind the schedule grid: weekdays by hours, edited like
 *        the "Logon Hours" dialog of Active Directory
 *
 * Pure logic on top of the Run line model (brunspec.h). QtCore only; no
 * widgets and no Director. A widget draws what this model reports and calls
 * its operations; nothing here knows how a cell looks.
 *
 * The terms, as they are used everywhere in this file:
 *
 * Grid line
 *     An understood Run line that is not restricted by day of the month or
 *     by week of the year (both masks have all bits). Only grid lines appear
 *     in cells and only grid lines are changed by grid edits.
 *
 * Calendar rule
 *     An understood Run line restricted by day of the month (a list, a range
 *     or a modulo) or by week of the year. It is not weekday based and is
 *     listed apart (calendarRules()); a grid edit never changes it.
 *
 * Not understood
 *     A line without a BRunSpec. Listed apart with its text and the reason
 *     (notUnderstood()); never changed. While there is one, edits of other
 *     lines still work in memory, but the schedule cannot be written
 *     (canBeWritten()).
 *
 * Scope
 *     What a grid of weekdays and hours cannot show: the months, the weeks
 *     of the month and "last" of a grid line, exactly as the three masks of
 *     that line (BGridScope). A line without such a restriction has the
 *     scope "every week". Two lines are in the same scope if and only if
 *     these three values are equal.
 *
 *     Scopes are never merged and never split. "1st" and "1st-2nd" are two
 *     scopes that share dates; so are "jan-mar 1st" and "1st". A line
 *     belongs to the one scope its masks spell and to no other, and an edit
 *     in one scope never rewrites a line of another, however they overlap.
 *     The reason: every other choice (cutting scopes into disjoint pieces,
 *     or showing "1st-2nd" inside "1st") would make the model rewrite lines
 *     the user did not touch, or show one line in two places with an edit
 *     in either changing both.
 *
 * Cell
 *     One weekday and one hour. For the active scope a cell lists
 *      - in scope: the grid lines of the active scope whose masks contain
 *        that weekday and hour;
 *      - out of scope: the grid lines of every other scope whose masks
 *        contain them, each with its scope and with a flag saying whether
 *        that scope shares dates with the active one (then both run on the
 *        same day at least sometimes). A widget draws these ghosted.
 *     Both lists are ordered by minute, then by position of the line in the
 *     schedule. Several runs in one cell are normal.
 *
 * Brush
 *     The overrides of a run (the level among them, or no level: the job's
 *     own level applies) and its minute. Two runs have the same brush if
 *     their BRunOverrides compare equal (every field, values as written,
 *     case sensitive) and their minutes are equal.
 *
 * Weekdays are kept as Bareos keeps them (0 = Sunday ... 6 = Saturday).
 * Rows are what a view shows: row 0 is the first day of the week the model
 * was given. Functions say which of the two they take.
 */

/// A cell position in rows and columns (column = hour)
struct BGridPos
{
    int row = 0;
    int column = 0;

    bool operator==(const BGridPos &other) const { return row == other.row && column == other.column; }
    bool operator!=(const BGridPos &other) const { return !(*this == other); }
};

/// An inclusive rectangle of cells
struct BGridRect
{
    int top = 0;
    int left = 0;
    int bottom = -1;
    int right = -1;

    bool isValid() const { return top <= bottom && left <= right; }
    bool operator==(const BGridRect &other) const
    {
        return top == other.top && left == other.left
            && bottom == other.bottom && right == other.right;
    }
    bool operator!=(const BGridRect &other) const { return !(*this == other); }
};

/// A set of rows times a set of columns: what one Run line can say
struct BGridProduct
{
    quint8 rows = 0;        ///< 7 bits
    quint32 columns = 0;    ///< 24 bits

    bool operator==(const BGridProduct &other) const
    {
        return rows == other.rows && columns == other.columns;
    }
    bool operator!=(const BGridProduct &other) const { return !(*this == other); }
};

/**
 * @brief A set of cells of the 7 by 24 grid
 *
 * A plain value. What a row means (a weekday of Bareos or a row of a view)
 * is up to the user of the set.
 */
class BGridCellSet
{
public:
    static constexpr int Rows = 7;
    static constexpr int Columns = 24;
    static constexpr quint32 AllColumns = (1u << Columns) - 1;
    static constexpr quint8 AllRows = (1u << Rows) - 1;

    BGridCellSet() = default;

    static BGridCellSet product(quint8 rows, quint32 columns);
    static BGridCellSet rect(int row1, int column1, int row2, int column2);
    static BGridCellSet all() { return product(AllRows, AllColumns); }

    bool contains(int row, int column) const;
    void insert(int row, int column);
    void remove(int row, int column);

    /// The columns of one row as bits; 0 for a row outside the grid
    quint32 rowBits(int row) const;
    void setRowBits(int row, quint32 columns);

    bool isEmpty() const;
    int count() const;
    QList<BGridPos> cells() const;      ///< By row, then by column

    BGridCellSet united(const BGridCellSet &other) const;
    BGridCellSet intersected(const BGridCellSet &other) const;
    BGridCellSet subtracted(const BGridCellSet &other) const;
    BGridCellSet toggled(const BGridCellSet &other) const;
    bool intersects(const BGridCellSet &other) const;

    /// Is the set a set of rows times a set of columns? The empty set is not.
    bool isProduct(BGridProduct *product = nullptr) const;

    /**
     * @brief The set as the smallest number of products that do not overlap
     *
     * Rows with the same columns are one unit. The number is proven smallest
     * whenever the cheap bounds meet (the usual case) or the exact search
     * finishes within its fixed budget; otherwise the result is the best
     * found, never more than seven products and never more than the number
     * of distinct rows or of distinct columns.
     *
     * Ordered by size (largest first), then by lowest row, then by columns.
     */
    QList<BGridProduct> products() const;

    /// The smallest rectangle around all cells; invalid for the empty set
    BGridRect boundingRect() const;

    /// The set cut into rectangles of neighbouring cells, for drawing an
    /// outline: by top row, then by left column
    QList<BGridRect> rects() const;

    bool operator==(const BGridCellSet &other) const { return m_rows == other.m_rows; }
    bool operator!=(const BGridCellSet &other) const { return !(*this == other); }

private:
    std::array<quint32, Rows> m_rows{};
};

/**
 * @brief The selection of the grid, independent of any view
 *
 * The operations of the Logon Hours dialog. Rows are rows of the view.
 *
 * An anchor is set by selectCell(), toggleCell(), selectRow(),
 * selectColumn() and selectAll(); extendTo() spans the rectangle from the
 * anchor to a cell. Dragging calls extendTo() again and again: every call
 * starts from the selection as it was when the anchor was set, so the
 * rectangle can shrink again. A row header sets the anchor to the first
 * column of its row and a column header to the first row of its column, so
 * dragging over headers is extendTo(row, 23) or extendTo(6, column).
 */
class BGridSelection
{
public:
    enum Mode {
        Replace,    ///< The selection becomes exactly this
        Add,        ///< Added to what is selected (Ctrl)
        Toggle      ///< Deselected if all of it is selected, otherwise added
    };

    void clear();

    /// A click: this cell only
    void selectCell(int row, int column);

    /// A click with the modifier: this cell changes, the rest stays
    void toggleCell(int row, int column);

    /**
     * @brief The rectangle from the anchor to this cell
     * @param keep false: the selection becomes the rectangle (Shift);
     *             true: the rectangle is added to what was selected when
     *             the anchor was set (Ctrl+Shift, or dragging after a
     *             click with the modifier). If setting the anchor
     *             deselected (a toggle of a selected cell, row or column),
     *             the rectangle is taken away instead.
     *
     * Without an anchor the cell becomes the anchor.
     */
    void extendTo(int row, int column, bool keep = false);

    void selectRow(int row, Mode mode = Replace);
    void selectColumn(int column, Mode mode = Replace);
    void selectAll();

    bool contains(int row, int column) const { return m_cells.contains(row, column); }
    bool isEmpty() const { return m_cells.isEmpty(); }
    int count() const { return m_cells.count(); }
    const BGridCellSet &cells() const { return m_cells; }

    bool isRowSelected(int row) const;          ///< All 24 cells of the row
    bool isColumnSelected(int column) const;    ///< All 7 cells of the column

    std::optional<BGridPos> anchor() const { return m_anchor; }
    BGridRect boundingRect() const { return m_cells.boundingRect(); }
    QList<BGridRect> rects() const { return m_cells.rects(); }

    /// The same cells and anchor with the rows mapped: @p rowOfRow[old] = new
    BGridSelection withRowsMapped(const std::array<int, 7> &rowOfRow) const;

    bool operator==(const BGridSelection &other) const
    {
        return m_cells == other.m_cells && m_anchor == other.m_anchor
            && m_base == other.m_base && m_subtract == other.m_subtract;
    }
    bool operator!=(const BGridSelection &other) const { return !(*this == other); }

private:
    void apply(const BGridCellSet &cells, Mode mode);
    static bool valid(int row, int column);

    BGridCellSet m_cells;
    BGridCellSet m_base;    ///< The selection just before the anchor was set
    bool m_subtract = false; ///< The anchor deselected: extending with keep deselects too
    std::optional<BGridPos> m_anchor;
};

/// What the grid cannot show of a grid line: see "Scope" at the top
struct BGridScope
{
    quint16 months = BDateTimeMask::AllMonths;  ///< Bit 0 = January
    quint8 weeksOfMonth = BDateTimeMask::AllWoms;   ///< Bit 0 = "1st"
    bool last = false;                          ///< "last": the last seven days of a month

    static BGridScope everyWeek() { return BGridScope(); }

    /// The scope a grid line spells
    static BGridScope of(const BDateTimeMask &mask);

    bool isEveryWeek() const;

    /// Can it be written: at least one month, and a week of the month or "last"
    bool isValid() const;

    /**
     * @brief Is there a date that is in both scopes?
     *
     * Meant for "may run on the same day": a common month, and a common
     * week of the month, or "last" on one side and "4th", "5th" or "last"
     * on the other (the last seven days of a month are in its fourth and
     * fifth week). Lengths of single months are not looked at.
     */
    bool sharesDatesWith(const BGridScope &other) const;

    bool operator==(const BGridScope &other) const
    {
        return months == other.months && weeksOfMonth == other.weeksOfMonth && last == other.last;
    }
    bool operator!=(const BGridScope &other) const { return !(*this == other); }
};

/// One entry of the scope bar
struct BGridScopeInfo
{
    BGridScope scope;
    int runCount = 0;   ///< Grid lines in this scope; 0 for a scope that was added or emptied

    bool operator==(const BGridScopeInfo &other) const
    {
        return scope == other.scope && runCount == other.runCount;
    }
    bool operator!=(const BGridScopeInfo &other) const { return !(*this == other); }
};

/// What is painted: see "Brush" at the top
struct BGridBrush
{
    BRunOverrides overrides;    ///< overrides.level empty: the job's own level
    int minute = 0;

    bool operator==(const BGridBrush &other) const
    {
        return overrides == other.overrides && minute == other.minute;
    }
    bool operator!=(const BGridBrush &other) const { return !(*this == other); }
};

/// One run in one cell
struct BGridRun
{
    int runIndex = -1;              ///< Index of its Run line in the schedule
    BRunOverrides overrides;        ///< All overrides of the line, the level among them
    int minute = 0;
    int scopeIndex = -1;            ///< Index into scopes()

    /// The line names neither a time nor "hourly": it runs every hour at :00
    /// because nothing says otherwise. A widget should warn.
    bool withoutTime = false;

    /// For a run out of scope: its scope shares dates with the active one
    bool sharesDatesWithActiveScope = true;

    const std::optional<QString> &level() const { return overrides.level; }
    BGridBrush brush() const { return BGridBrush{overrides, minute}; }

    bool operator==(const BGridRun &other) const;
    bool operator!=(const BGridRun &other) const { return !(*this == other); }
};

struct BGridCell
{
    QList<BGridRun> inScope;        ///< Runs of the active scope
    QList<BGridRun> outOfScope;     ///< Runs of the other scopes on the same weekday and hour

    /**
     * The load of this hour of the weekday, whatever setExpectedLoad() was
     * given, empty by default. The schedule tab gives how many runs of all
     * enabled schedules start in it, on average per week
     * (BJobScheduleIndex::startsPerWeekdayHour()).
     */
    std::optional<double> expectedConcurrentRuns;

    bool isEmpty() const { return inScope.isEmpty() && outOfScope.isEmpty(); }
};

/// An understood line that is not weekday based
struct BGridCalendarRule
{
    int runIndex = -1;
    BRunSpec spec;              ///< Everything a form needs to show and edit the line
    QString text;               ///< As it would be written now
    bool byDayOfMonth = false;  ///< A list, range or modulo of days of the month
    bool byWeekOfYear = false;  ///< A list, range or modulo of weeks of the year
    bool withoutTime = false;   ///< As BGridRun::withoutTime

    bool operator==(const BGridCalendarRule &other) const;
    bool operator!=(const BGridCalendarRule &other) const { return !(*this == other); }
};

struct BGridNotUnderstood
{
    int runIndex = -1;
    QString text;       ///< Verbatim
    QString reason;     ///< The parser's

    bool operator==(const BGridNotUnderstood &other) const
    {
        return runIndex == other.runIndex && text == other.text && reason == other.reason;
    }
    bool operator!=(const BGridNotUnderstood &other) const { return !(*this == other); }
};

/// One row of the "Run lines" panel and of the review before saving
struct BGridRunLineText
{
    enum State {
        Untouched,  ///< Written back as it was reported
        Changed,    ///< A reported line an edit has changed
        New,        ///< Created by an edit
        Removed     ///< A reported line that is gone
    };

    State state = Untouched;
    QString text;           ///< What would be written; for Removed and for a line not understood the reported text
    QString originalText;   ///< As reported; empty for New
    int runIndex = -1;      ///< In the schedule as it is now; -1 for Removed
    int originalIndex = -1; ///< In the schedule as it was loaded; -1 for New
    bool understood = true;
    QString error;          ///< Why it is not understood

    /// Untouched, but in a spelling Bareos does not read back
    /// (BRunLine::needsRewrite): text differs from originalText
    bool respelled = false;
};

/// The Run lines of the model at one moment; what an undo step restores
struct BGridSnapshot
{
    QList<BRunLine> runs;
    QList<int> origins;     ///< Per run: its index in the loaded schedule, -1 for a new line
    QList<int> splitFrom;   ///< Per run: for a new line cut out of a loaded line that line's index, else -1
};

/// What an edit did
struct BGridChange
{
    bool ok = false;        ///< false: nothing was changed, see error
    QString error;

    BGridSnapshot before;
    BGridSnapshot after;

    QList<int> removedRuns; ///< Indices into before.runs
    QList<int> changedRuns; ///< Indices into after.runs: lines that existed and say something else now
    QList<int> addedRuns;   ///< Indices into after.runs

    bool isNoop() const
    {
        return removedRuns.isEmpty() && changedRuns.isEmpty() && addedRuns.isEmpty();
    }
};

/**
 * @brief The grid of one schedule resource
 *
 * Edit rules (applyBrush(), erase(), moveRun()):
 *  - An edit works in the active scope and changes only the grid lines it
 *    hits. Every other line keeps its text: lines of other scopes, calendar
 *    rules and lines not understood are never changed, and a line of the
 *    active scope that is not hit is not changed either.
 *  - A line that loses cells is rewritten to mean exactly the cells left.
 *    If they are no longer weekdays times hours, the line is split into the
 *    smallest number of lines that are (BGridCellSet::products()): the
 *    largest stays in the place of the line, the others follow it.
 *    A line that loses all its cells is removed.
 *  - Painted cells that do not yet have the brush become new lines at the
 *    end of the schedule, again the smallest number.
 *  - Merging: after an edit two lines of the active scope with the same
 *    brush become one if they do not overlap, together are weekdays times
 *    hours, at least one of them was created or changed by this edit and
 *    neither is a line as it was loaded. A line the user did not touch is
 *    never restructured, also not to take up a neighbouring cell.
 *  - A changed line that says again what the loaded line said goes back to
 *    its reported text.
 *  - Every line an edit creates or changes is written by BRunWriter, read
 *    back and compared (BRunWriter::lineText()). If one fails, the whole
 *    edit fails and the model is as before.
 *
 * Every edit returns a BGridChange with the Run lines before and after;
 * restore() puts either back, which is all an undo command needs.
 */
class BScheduleGridModel : public QObject
{
    Q_OBJECT

public:
    /// What painting does with the other runs of the active scope in a cell
    enum PaintMode {
        ReplaceRuns,    ///< The cell ends up with the brush only, as in the Logon Hours dialog
        AddRun          ///< The brush is added; other runs stay
    };

    explicit BScheduleGridModel(QObject *parent = nullptr);

    // --- The schedule ------------------------------------------------------

    /// Takes a schedule as it was loaded. Clears selection and added scopes.
    void setSchedule(const BSchedule &schedule);

    /// The schedule as it is now, with all edits
    BSchedule schedule() const;

    /// The schedule as it was given to setSchedule()
    const BSchedule &loadedSchedule() const { return m_loaded; }

    bool isModified() const;

    /// false while a line is not understood; the reason names the first one
    bool canBeWritten(QString *reason = nullptr) const;

    // --- Rows and weekdays -------------------------------------------------

    /// The day of the week of row 0. A view passes the locale's.
    void setFirstDayOfWeek(Qt::DayOfWeek day);
    Qt::DayOfWeek firstDayOfWeek() const { return m_firstDay; }

    static constexpr int rowCount() { return BGridCellSet::Rows; }
    static constexpr int columnCount() { return BGridCellSet::Columns; }

    /// The weekday of Bareos (0 = Sunday ... 6 = Saturday) a row shows
    int weekdayOfRow(int row) const;
    int rowOfWeekday(int weekday) const;
    Qt::DayOfWeek dayOfWeekOfRow(int row) const;

    // --- Scopes ------------------------------------------------------------

    /**
     * @brief The scope bar
     *
     * "Every week" is always there and always first, so that a schedule
     * without lines has a place to paint. Then the scopes of the grid
     * lines: those for all months before those for some months (by first
     * month), within that by first week of the month, "last" alone at the
     * end. A scope that was added, or whose last line was erased, stays
     * with runCount 0 until setSchedule() or removeScope().
     */
    const QList<BGridScopeInfo> &scopes() const { return m_scopes; }
    int activeScopeIndex() const { return m_activeScope; }
    BGridScope activeScope() const;
    void setActiveScope(int index);
    int indexOfScope(const BGridScope &scope) const;

    /// Adds a scope that does not occur yet. @return its index (also if it
    /// was there already), -1 if the scope is not valid
    int addScope(const BGridScope &scope);

    /// Removes a scope without lines that is not "every week". If it was
    /// active, "every week" becomes active.
    bool removeScope(int index);

    // --- Cells of the active scope -----------------------------------------

    BGridCell cell(int row, int column) const;
    BGridCell cellAtWeekday(int weekday, int hour) const;

    /// The distinct brushes of the grid lines of all scopes, in the order of
    /// their first line: what a palette offers
    QList<BGridBrush> brushes() const;

    /**
     * @brief The cells of one grid line, for highlighting it
     * @param scopeIndex receives the index of its scope
     * @return the cells in rows of the view; empty for a line that is not a
     *         grid line
     */
    BGridCellSet cellsOfRun(int runIndex, int *scopeIndex = nullptr) const;

    /// Sets the load figure of BGridCell::expectedConcurrentRuns
    void setExpectedLoad(int weekday, int hour, std::optional<double> concurrentRuns);
    void clearExpectedLoad();

    // --- What does not fit the grid ----------------------------------------

    const QList<BGridCalendarRule> &calendarRules() const { return m_calendarRules; }
    const QList<BGridNotUnderstood> &notUnderstood() const { return m_notUnderstood; }

    // --- Selection ---------------------------------------------------------

    const BGridSelection &selection() const { return m_selection; }
    void setSelection(const BGridSelection &selection);

    // --- Edits -------------------------------------------------------------

    /// Paints the selected cells of the active scope
    BGridChange applyBrush(const BGridBrush &brush, PaintMode mode = ReplaceRuns);
    /// Paints the given cells (rows of the view) of the active scope
    BGridChange applyBrush(const BGridCellSet &cells, const BGridBrush &brush,
                           PaintMode mode = ReplaceRuns);

    /// Removes every run of the active scope from the selected cells
    BGridChange erase();
    BGridChange erase(const BGridCellSet &cells);

    /**
     * @brief Moves a grid line to another minute
     *
     * The unit is the Run line: it has one minute for all its cells. The
     * line may be of any scope. To move the runs of some cells only, paint
     * them with a brush of the other minute.
     */
    BGridChange moveRun(int runIndex, int minute);

    // --- Edits of calendar rules -------------------------------------------
    //
    // A calendar rule is edited as a whole, by a form. The three operations
    // follow the edit rules above: every other line keeps its text, the
    // line that is created or changed is written, read back and compared,
    // and a changed rule that says again what the loaded line said goes
    // back to its reported text.

    /// Is the run restricted by day of the month or by week of the year,
    /// which makes it a calendar rule and not a line of the grid?
    static bool isCalendarRule(const BRunSpec &spec);

    /// Adds a calendar rule at the end of the schedule. Fails for a spec
    /// that is not a calendar rule: such a run is painted in the grid.
    BGridChange addCalendarRule(const BRunSpec &spec);

    /// Replaces the calendar rule at @p runIndex (an index of a Run line,
    /// BGridCalendarRule::runIndex). The new spec has to be a calendar rule.
    BGridChange changeCalendarRule(int runIndex, const BRunSpec &spec);

    /// Removes the calendar rule at @p runIndex
    BGridChange removeCalendarRule(int runIndex);

    BGridSnapshot snapshot() const;

    /// Puts back the Run lines of BGridChange::before or ::after
    bool restore(const BGridSnapshot &snapshot);

    // --- Run lines as text -------------------------------------------------

    /// The lines as they are now. A removed line stands in front of the next
    /// loaded line that is still there; if none follows, in front of the
    /// painted lines at the end.
    QList<BGridRunLineText> runLines() const;

    /// Only what would be written, in order; false with @p error if the
    /// schedule cannot be written
    bool runTexts(QStringList *texts, QString *error = nullptr) const;

signals:
    /// Everything may be different: a schedule was set
    void scheduleReset();

    /// Cells of the active scope whose content changed, in rows of the view
    void cellsChanged(const QList<BGridPos> &cells);

    /// The scope bar changed: scopes or their run counts
    void scopesChanged();
    /// Another scope is active, or the active one has another index. All
    /// cells are to be drawn again.
    void activeScopeChanged(int index);

    void calendarRulesChanged();
    void notUnderstoodChanged();

    /// What runLines() returns changed
    void runLinesChanged();
    void modifiedChanged(bool modified);
    void selectionChanged();

    /// The rows show other weekdays: everything is to be drawn again
    void firstDayOfWeekChanged();

private:
    struct Item {
        BRunLine line;
        int origin = -1;        ///< Index in m_loaded.runs, -1 for a new line
        int splitFrom = -1;     ///< For a new line cut out of a loaded line: that line's index
        QString text;           ///< What would be written; empty if not understood
        // Only during an edit:
        int before = -1;        ///< Index before the edit, -1 for a line the edit created
        bool touched = false;   ///< Created or changed by the edit
    };
    using Cells = std::array<std::array<BGridCell, BGridCellSet::Columns>, BGridCellSet::Rows>;

    static bool isGridLine(const BRunLine &line);
    static bool isGenerated(const Item &item);
    static BGridCellSet cellsOf(const BRunSpec &spec);

    BGridCellSet toWeekdays(const BGridCellSet &rows) const;
    BGridCellSet toRows(const BGridCellSet &weekdays) const;

    BGridChange failed(const QString &error) const;
    BGridChange paint(const BGridCellSet &weekdayCells, const BGridBrush &brush, PaintMode mode);
    BGridChange eraseCells(const BGridCellSet &weekdayCells);

    /// Takes cells away from the line at @p index; splits or removes it.
    /// @return how many items the list grew by (-1: the line is gone)
    static int removeCells(QList<Item> &items, int index, const BGridCellSet &cells);
    static void mergeGenerated(QList<Item> &items, const BGridScope &scope);
    BGridChange commit(QList<Item> items);

    void installItems(const QList<Item> &items, bool reset);
    void rebuildDerived();
    Cells computeCells() const;
    void rebuildScopes();

    BSchedule m_loaded;
    QList<Item> m_items;

    Qt::DayOfWeek m_firstDay = Qt::Monday;

    QList<BGridScope> m_knownScopes;    ///< Every scope seen since setSchedule(), incl. added ones
    QList<BGridScopeInfo> m_scopes;
    BGridScope m_activeScopeValue;
    int m_activeScope = 0;

    Cells m_cells;      ///< By weekday of Bareos, then hour
    std::array<std::array<std::optional<double>, BGridCellSet::Columns>, BGridCellSet::Rows> m_load;
    QList<BGridCalendarRule> m_calendarRules;
    QList<BGridNotUnderstood> m_notUnderstood;

    BGridSelection m_selection;
};

#endif // BSCHEDULEGRIDMODEL_H
