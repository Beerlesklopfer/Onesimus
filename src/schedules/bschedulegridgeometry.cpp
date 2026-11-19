/**
 * @file bschedulegridgeometry.cpp
 * @brief Where the parts of the schedule grid are
 */

#include "schedules/bschedulegridgeometry.h"

void BScheduleGridGeometry::setHeaderSize(int rowHeaderWidth, int columnHeaderHeight)
{
    m_headerWidth = qMax(0, rowHeaderWidth);
    m_headerHeight = qMax(0, columnHeaderHeight);
}

int BScheduleGridGeometry::cellWidth() const
{
    return qMax(int(MinCellWidth), (m_viewport.width() - m_headerWidth) / Columns);
}

int BScheduleGridGeometry::cellHeight() const
{
    const int band = hasRuleBand() ? RuleBandGap + m_ruleBandHeight : 0;
    const int available = (m_viewport.height() - m_headerHeight - band) / Rows;
    return qBound(int(MinCellHeight), available, int(MaxCellHeight));
}

QSize BScheduleGridGeometry::contentSize() const
{
    const int band = hasRuleBand() ? RuleBandGap + m_ruleBandHeight : 0;
    return QSize(m_headerWidth + Columns * cellWidth(),
                 m_headerHeight + Rows * cellHeight() + band);
}

QSize BScheduleGridGeometry::minimumContentSize() const
{
    const int band = hasRuleBand() ? RuleBandGap + m_ruleBandHeight : 0;
    return QSize(m_headerWidth + Columns * MinCellWidth,
                 m_headerHeight + Rows * MinCellHeight + band);
}

QSize BScheduleGridGeometry::scrollRange() const
{
    const QSize content = contentSize();
    return QSize(qMax(0, content.width() - m_viewport.width()),
                 qMax(0, content.height() - m_viewport.height()));
}

QRect BScheduleGridGeometry::cornerRect() const
{
    return QRect(0, 0, m_headerWidth, m_headerHeight);
}

QRect BScheduleGridGeometry::rowHeaderRect(int row) const
{
    return QRect(0, m_headerHeight + row * cellHeight() - m_offset.y(),
                 m_headerWidth, cellHeight());
}

QRect BScheduleGridGeometry::columnHeaderRect(int column) const
{
    return QRect(m_headerWidth + column * cellWidth() - m_offset.x(), 0,
                 cellWidth(), m_headerHeight);
}

QRect BScheduleGridGeometry::cellRect(int row, int column) const
{
    return QRect(m_headerWidth + column * cellWidth() - m_offset.x(),
                 m_headerHeight + row * cellHeight() - m_offset.y(),
                 cellWidth(), cellHeight());
}

QRect BScheduleGridGeometry::ruleHeaderRect() const
{
    if (!hasRuleBand())
        return QRect();
    return QRect(0, m_headerHeight + Rows * cellHeight() + RuleBandGap - m_offset.y(),
                 m_headerWidth, m_ruleBandHeight);
}

QRect BScheduleGridGeometry::ruleCellRect(int column) const
{
    if (!hasRuleBand())
        return QRect();
    return QRect(m_headerWidth + column * cellWidth() - m_offset.x(),
                 m_headerHeight + Rows * cellHeight() + RuleBandGap - m_offset.y(),
                 cellWidth(), m_ruleBandHeight);
}

QRect BScheduleGridGeometry::scrollAreaRect() const
{
    return QRect(m_headerWidth, m_headerHeight,
                 qMax(0, m_viewport.width() - m_headerWidth),
                 qMax(0, m_viewport.height() - m_headerHeight));
}

QRect BScheduleGridGeometry::cellsRect() const
{
    return QRect(m_headerWidth - m_offset.x(), m_headerHeight - m_offset.y(),
                 Columns * cellWidth(), Rows * cellHeight());
}

BScheduleGridGeometry::Hit BScheduleGridGeometry::hitTest(const QPoint &pos) const
{
    Hit hit;
    if (pos.x() < 0 || pos.y() < 0 || pos.x() >= m_viewport.width() || pos.y() >= m_viewport.height())
        return hit;

    const bool inHeaderColumn = pos.x() < m_headerWidth;
    const bool inHeaderRow = pos.y() < m_headerHeight;
    if (inHeaderColumn && inHeaderRow) {
        hit.part = Corner;
        return hit;
    }

    const int x = pos.x() - m_headerWidth + m_offset.x();
    const int y = pos.y() - m_headerHeight + m_offset.y();
    const int column = x / cellWidth();
    const int row = y / cellHeight();
    const int bandTop = Rows * cellHeight() + RuleBandGap;
    const bool inBand = hasRuleBand() && y >= bandTop && y < bandTop + m_ruleBandHeight;

    if (inHeaderRow) {
        if (column < Columns) {
            hit.part = ColumnHeader;
            hit.column = column;
        }
        return hit;
    }
    if (inHeaderColumn) {
        if (row < Rows) {
            hit.part = RowHeader;
            hit.row = row;
        } else if (inBand) {
            hit.part = RuleHeader;
        }
        return hit;
    }
    if (column >= Columns)
        return hit;
    if (row < Rows) {
        hit.part = Cell;
        hit.row = row;
        hit.column = column;
    } else if (inBand) {
        hit.part = RuleCell;
        hit.column = column;
    }
    return hit;
}

BGridPos BScheduleGridGeometry::nearestCell(const QPoint &pos) const
{
    const int x = pos.x() - m_headerWidth + m_offset.x();
    const int y = pos.y() - m_headerHeight + m_offset.y();
    BGridPos cell;
    cell.column = qBound(0, x < 0 ? 0 : x / cellWidth(), Columns - 1);
    cell.row = qBound(0, y < 0 ? 0 : y / cellHeight(), Rows - 1);
    return cell;
}
