#ifndef BSCHEDULEGRIDWIDGET_H
#define BSCHEDULEGRIDWIDGET_H

#include <QAbstractScrollArea>
#include <QColor>
#include <QPointer>

#include <array>
#include <functional>
#include <optional>

#include "schedules/bschedulegrideditor.h"
#include "schedules/bschedulegridgeometry.h"
#include "schedules/model/bschedulegridmodel.h"

class QFrame;

/**
 * @brief The schedule grid: weekdays by hours, as the "Logon Hours" dialog
 *        of Active Directory shows and edits them
 *
 * Draws one BScheduleGridModel. Without an editor it changes nothing but
 * the selection of the model; with one (setEditor()) it also paints and
 * erases, always through the editor, so that every change is a step of its
 * undo stack. Neither the model nor the editor is owned.
 *
 * What a cell shows
 *  - A run of the active scope is a stripe filled with the colour of its
 *    level and marked with the letter of the level (BScheduleGridText::
 *    levelLetter()), because colour is never the only carrier. A tick at
 *    the top edge stands at minute/60 across the stripe; the minute is also
 *    written (":05") if it is not :00 and there is room.
 *  - A run that overrides more than the level has the tag of its brush in
 *    the top right corner (A, B, ... in the order of
 *    BScheduleGridModel::brushes()); the legend of the view spells it out.
 *  - Runs of other scopes on the same weekday and hour are drawn as ghosts:
 *    hatched instead of filled, with a badge naming their scope. A ghost
 *    whose scope shares dates with the active one (both may run on the same
 *    day) is tinted with its level colour, hatched rising to the right and
 *    framed with a solid line; one that never coincides has no tint, is
 *    hatched falling to the right and framed with a dashed line.
 *  - Up to three stripes per cell, runs of the active scope first; the rest
 *    is counted in a "+n" badge. Enter or a double click opens the list of
 *    them; a plain click only selects.
 *  - A run whose line names no time has a warning triangle.
 *  - BGridCell::expectedConcurrentRuns, where set, is a thin bar at the
 *    bottom of the cell, as long as the figure is of the largest in the grid.
 *
 * Calendar rules (lines by day of the month or week of the year) are not
 * cells. They are marked in a band below the grid, at their hours, drawn
 * without cell borders; the band cannot be selected or painted. A click on
 * a mark chooses its rule (another click the next one, where several run
 * in the hour), a double click asks for the form of the rule.
 *
 * Selection (the model owns it) with mouse and keyboard as in the Logon
 * Hours dialog: click a cell, drag a rectangle, click a row header for a
 * day, a column header for an hour, the corner for all; Ctrl adds or
 * toggles, Shift extends from the anchor. Arrow keys move, Shift+arrows
 * extend, Home/End go to the first and last hour (with Ctrl to the first
 * and last cell), Page Up/Down to the first and last day, Ctrl+A selects
 * all, Enter opens the list of runs of the current cell. Clicking or
 * Shift-clicking a header always selects whole days or hours.
 *
 * Editing (with an editor)
 *  - Space applies the tool in hand (a brush or the eraser) to the selected
 *    cells; Shift+Space adds the brush to the runs the cells have instead
 *    of replacing them. Del and Backspace erase the selected cells.
 *  - F, D, I and V take the brush of that level (Full, Differential,
 *    Incremental, VirtualFull) in hand and paint the selection with it.
 *  - P switches paint mode. In paint mode a press on a cell and dragging
 *    over cells applies the tool to every cell the mouse passes, as one
 *    step, when the button is released; Esc gives the stroke up. With Ctrl
 *    or Shift the mouse selects as it does outside paint mode, and the
 *    headers always select.
 *  - Only runs of the active scope are painted and erased. Runs of other
 *    scopes in a cell stay as they are; the tooltip says so.
 *
 * A screen reader gets a table (bschedulegridaccessible.h).
 */
class BScheduleGridWidget : public QAbstractScrollArea
{
    Q_OBJECT

public:
    /// The colour of a level by its code (F, I, D, V); an invalid colour for
    /// a code without one. The application passes BLevelColors::getLevelColor.
    using LevelColorSource = std::function<QColor(const QString &code)>;

    explicit BScheduleGridWidget(QWidget *parent = nullptr);
    ~BScheduleGridWidget() override;

    void setModel(BScheduleGridModel *model);
    BScheduleGridModel *model() const { return m_model; }

    /// The editor that makes the grid editable; nullptr: only selection
    void setEditor(BScheduleGridEditor *editor);
    BScheduleGridEditor *editor() const { return m_editor; }
    bool isEditable() const { return !m_editor.isNull(); }

    /// The cells of a stroke of paint mode that is not finished yet
    const BGridCellSet &pendingStroke() const { return m_stroke; }

    void setLevelColorSource(const LevelColorSource &source);

    /**
     * @brief The fill of a level
     *
     * From @p source; without a source the default colours of the
     * application. A run without a level, and a level without a colour,
     * get the neutral colour the timeline uses (QPalette::Mid).
     */
    static QColor levelFill(const LevelColorSource &source, const QPalette &palette,
                            const std::optional<QString> &level);
    QColor levelFill(const std::optional<QString> &level) const;

    /// Black or white, whichever is readable on @p background
    static QColor inkOn(const QColor &background);

    const BScheduleGridGeometry &gridGeometry() const { return m_geometry; }

    // --- Current cell: where the keyboard is ------------------------------

    BGridPos currentCell() const { return m_current; }
    void setCurrentCell(const BGridPos &cell);

    // --- Highlights (set by the lists beside the grid) --------------------

    /// Outlines the cells of a Run line in the active scope; -1 for none
    void setHighlightedRun(int runIndex);
    int highlightedRun() const { return m_highlightedRun; }

    /// Outlines the marks of a calendar rule in the band; -1 for none
    void setHighlightedRule(int ruleIndex);
    int highlightedRule() const { return m_highlightedRule; }

    // --- Texts --------------------------------------------------------------

    /// The tooltip at a position of the viewport (rich text); empty outside
    QString toolTipAt(const QPoint &viewportPos) const;
    QString cellToolTip(int row, int column) const;

    /// The tag of a brush with overrides beside the level; empty otherwise
    QString brushTag(const BGridBrush &brush) const;

    /// The calendar rules (indices into calendarRules()) that run in an hour
    QList<int> rulesAtHour(int hour) const;

    // --- The list of runs of a cell -----------------------------------------

    /// Opens the popover for a cell. @return false for a cell without runs
    bool showCellDetails(int row, int column);
    /// The open popover, or nullptr
    QWidget *cellDetails() const;

    // --- Painting, also used by the legend ----------------------------------

    /**
     * @brief Draws one run as a stripe
     * @param inScope false: as a ghost, with @p scopeBadge
     * @param loadBar pixels at the bottom that stay free
     */
    void paintRun(QPainter &painter, const QRect &rect, const BGridRun &run, bool inScope,
                  const QString &scopeBadge = QString(), int loadBar = 0) const;
    void paintWarningMark(QPainter &painter, const QRect &rect, const QColor &ink) const;
    void paintRuleMark(QPainter &painter, const QRect &rect, const QString &text,
                       const std::optional<QString> &level, bool highlighted) const;

    /// A measure designed for a 10 pt font, for this widget's font
    int px(int designPixels) const;
    /// The size of a sample of a run in the palette and the legend: room
    /// for what paintRun() draws with this widget's font
    QSize swatchSize() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void currentCellChanged(int row, int column);
    /// A mark in the band of calendar rules was clicked
    void calendarRuleActivated(int ruleIndex);
    /// A mark in the band was double clicked: the form of the rule is wanted
    void calendarRuleEditRequested(int ruleIndex);
    /// A run was chosen in the list of runs of a cell
    void runActivated(int runIndex);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void scrollContentsBy(int dx, int dy) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    bool viewportEvent(QEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    enum Drag { NoDrag, DragCells, DragRows, DragColumns, DragPaint };

    void onModelReset();
    void onCellsChanged();
    void onCellsEdited(const QList<BGridPos> &cells);
    void onToolChanged();
    void updateCursor();
    void extendStroke(const BGridPos &from, const BGridPos &to);
    void paintStroke(QPainter &painter) const;
    void onSelectionChanged();
    void rebuildCaches();
    void updateLayout();
    void ensureCellVisible(const BGridPos &cell);
    void moveCurrent(const BGridPos &cell, Qt::KeyboardModifiers modifiers);
    template <typename Change> void changeSelection(Change change);

    void paintHeaders(QPainter &painter) const;
    void paintCell(QPainter &painter, int row, int column, double maxLoad) const;
    void paintRuleBand(QPainter &painter) const;
    void paintSelection(QPainter &painter) const;
    void paintOutline(QPainter &painter, const BGridCellSet &cells, const QPen &pen) const;

    QFont smallFont() const;
    int m_loadBarHeight = 3;    ///< Set in updateLayout()
    QColor gridLineColor() const;
    QColor selectionOutlineColor() const;
    QString dayHeaderText(int row) const;

    void notifyAccessibleFocus();
    void notifyAccessibleSelection();
    void notifyAccessibleReset();
    void notifyAccessibleCells(const QList<BGridPos> &cells);
    void notifyAccessibleTool();

    QPointer<BScheduleGridModel> m_model;
    QPointer<BScheduleGridEditor> m_editor;
    LevelColorSource m_levelColors;
    BScheduleGridGeometry m_geometry;

    BGridPos m_current;
    int m_highlightedRun = -1;
    int m_highlightedRule = -1;

    // Caches of the model, rebuilt when it changes
    QList<BGridBrush> m_brushes;
    QStringList m_brushTags;
    std::array<QList<int>, BGridCellSet::Columns> m_rulesAtHour;

    // Mouse
    Drag m_drag = NoDrag;
    bool m_dragKeep = false;    ///< Ctrl was down at the press: dragging adds
    bool m_dragMoved = false;
    bool m_pressPlain = false;  ///< A press without modifiers
    BGridPos m_pressCell;
    BGridPos m_dragCell;
    BGridCellSet m_stroke;      ///< Paint mode: the cells passed since the press

    QPointer<QFrame> m_details;
};

#endif // BSCHEDULEGRIDWIDGET_H
