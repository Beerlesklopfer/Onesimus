#ifndef BSCHEDULEGRIDGEOMETRY_H
#define BSCHEDULEGRIDGEOMETRY_H

#include <QPoint>
#include <QRect>
#include <QSize>

#include "schedules/model/bschedulegridmodel.h"

/**
 * @file bschedulegridgeometry.h
 * @brief Where the parts of the schedule grid are, in viewport coordinates
 *
 * The layout of BScheduleGridWidget without a widget, so that the mapping
 * between positions and cells can be tested without a window:
 *
 *     corner     | column headers 0 ... 23        (stay at the top)
 *     -----------+----------------------------
 *     row header | cells, 7 rows by 24 columns    (scroll)
 *     ...        |
 *                  (gap)
 *     rule header| rule band, one field per hour  (only with calendar rules)
 *
 * The row headers stay at the left edge and the column headers at the top
 * while the cells scroll under them. The cells take the space there is,
 * never less than the minimum size; the height is limited so that the grid
 * does not grow into a wall of colour on a large window.
 * QtCore only.
 */
class BScheduleGridGeometry
{
public:
    enum Part {
        Outside,
        Corner,         ///< Selects all
        RowHeader,      ///< A weekday; row is set
        ColumnHeader,   ///< An hour; column is set
        Cell,           ///< row and column are set
        RuleHeader,     ///< The label of the band of calendar rules
        RuleCell        ///< An hour of that band; column is set. Not a cell.
    };

    struct Hit
    {
        Part part = Outside;
        int row = -1;
        int column = -1;

        bool operator==(const Hit &other) const
        {
            return part == other.part && row == other.row && column == other.column;
        }
        bool operator!=(const Hit &other) const { return !(*this == other); }
    };

    static constexpr int Rows = BGridCellSet::Rows;
    static constexpr int Columns = BGridCellSet::Columns;
    static constexpr int MinCellWidth = 30;
    static constexpr int MinCellHeight = 34;
    static constexpr int MaxCellHeight = 64;
    static constexpr int RuleBandGap = 6;

    void setViewportSize(const QSize &size) { m_viewport = size; }
    QSize viewportSize() const { return m_viewport; }

    /// The width of the row headers and the height of the column headers
    void setHeaderSize(int rowHeaderWidth, int columnHeaderHeight);
    int rowHeaderWidth() const { return m_headerWidth; }
    int columnHeaderHeight() const { return m_headerHeight; }

    /// The height of the band of calendar rules; 0 for no band
    void setRuleBandHeight(int height) { m_ruleBandHeight = qMax(0, height); }
    bool hasRuleBand() const { return m_ruleBandHeight > 0; }

    /// How far the cells are scrolled
    void setScrollOffset(const QPoint &offset) { m_offset = offset; }
    QPoint scrollOffset() const { return m_offset; }

    int cellWidth() const;
    int cellHeight() const;

    /// Headers, cells and band, not scrolled
    QSize contentSize() const;
    /// The smallest content there is: every cell at its minimum size
    QSize minimumContentSize() const;
    /// How far the cells can be scrolled; 0 where everything fits
    QSize scrollRange() const;

    QRect cornerRect() const;
    QRect rowHeaderRect(int row) const;
    QRect columnHeaderRect(int column) const;
    QRect cellRect(int row, int column) const;
    QRect ruleHeaderRect() const;
    QRect ruleCellRect(int column) const;

    /// The part of the viewport the cells and the band are drawn in
    QRect scrollAreaRect() const;
    /// All cells, as scrolled; may reach outside the viewport
    QRect cellsRect() const;

    /// What is at a position of the viewport
    Hit hitTest(const QPoint &pos) const;

    /// The cell at or nearest to a position, for dragging beyond the cells
    BGridPos nearestCell(const QPoint &pos) const;

private:
    QSize m_viewport;
    int m_headerWidth = 90;
    int m_headerHeight = 24;
    int m_ruleBandHeight = 0;
    QPoint m_offset;
};

#endif // BSCHEDULEGRIDGEOMETRY_H
