#ifndef BSCHEDULEGRIDWIDGET_H
#define BSCHEDULEGRIDWIDGET_H

#include <QAbstractScrollArea>
#include <QColor>
#include <QPointer>

#include <array>
#include <functional>
#include <optional>

#include "schedules/bschedulegridgeometry.h"
#include "schedules/model/bschedulegridmodel.h"

class QFrame;

/**
 * @brief The schedule grid: weekdays by hours, as the "Logon Hours" dialog
 *        of Active Directory shows them
 *
 * Draws one BScheduleGridModel and changes nothing but its selection. The
 * model is not owned.
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
 *    is counted in a "+n" badge. Enter or a click opens the list of them.
 *  - A run whose line names no time has a warning triangle.
 *  - BGridCell::expectedConcurrentRuns, where set, is a thin bar at the
 *    bottom of the cell, as long as the figure is of the largest in the grid.
 *
 * Calendar rules (lines by day of the month or week of the year) are not
 * cells. They are marked in a band below the grid, at their hours, drawn
 * without cell borders and not selectable.
 *
 * Selection (the model owns it) with mouse and keyboard as in the Logon
 * Hours dialog: click a cell, drag a rectangle, click a row header for a
 * day, a column header for an hour, the corner for all; Ctrl adds or
 * toggles, Shift extends from the anchor. Arrow keys move, Shift+arrows
 * extend, Home/End go to the first and last hour (with Ctrl to the first
 * and last cell), Page Up/Down to the first and last day, Ctrl+A selects
 * all, Enter opens the list of runs of the current cell. Nothing here edits.
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

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void currentCellChanged(int row, int column);
    /// A mark in the band of calendar rules was clicked
    void calendarRuleActivated(int ruleIndex);
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
    enum Drag { NoDrag, DragCells, DragRows, DragColumns };

    void onModelReset();
    void onCellsChanged();
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
    QColor gridLineColor() const;
    QColor selectionOutlineColor() const;
    QString dayHeaderText(int row) const;

    void notifyAccessibleFocus();
    void notifyAccessibleSelection();
    void notifyAccessibleReset();

    QPointer<BScheduleGridModel> m_model;
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

    QPointer<QFrame> m_details;
};

#endif // BSCHEDULEGRIDWIDGET_H
