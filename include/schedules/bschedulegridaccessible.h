// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BSCHEDULEGRIDACCESSIBLE_H
#define BSCHEDULEGRIDACCESSIBLE_H

#include <QAccessibleWidget>
#include <QHash>

class BScheduleGridWidget;

/**
 * @brief The schedule grid as a table for screen readers
 *
 * A table of seven rows (the weekdays, in the order of the view) and 24
 * columns (the hours). Its children are, row by row, the corner, the 24
 * column headers, and for every weekday its row header and its 24 cells:
 * child index = (row + 1) * 25 + (column + 1), with row -1 for the column
 * headers and column -1 for the row headers.
 *
 * A cell is named by BScheduleGridText::cellName() ("Saturday 21 hours:
 * Full at 21:00, pool Monthly, first week only"), says whether it is
 * selected and focused, knows its row and column header, and offers two
 * actions: press (select it, as a click does) and show menu (the list of
 * its runs). In a grid with an editor a cell is not read-only and has two
 * more actions: apply the tool in hand to it, and erase it. Headers are
 * named after the day or the hour and select it when pressed; the corner
 * selects all.
 *
 * After an edit the grid reports the changed part of the table and the new
 * name of every changed cell; the tool in hand is part of the description
 * of the table.
 *
 * The calendar rules are not cells; they are read as the description of
 * the table.
 */
class BScheduleGridAccessible : public QAccessibleWidget, public QAccessibleTableInterface
{
public:
    /// Registers the factory; safe to call more than once
    static void install();

    static constexpr int ChildColumns = 25;     ///< Row header plus 24 hours
    static constexpr int ChildRows = 8;         ///< Column headers plus 7 days

    /// The child index of a cell; -1 as row or column gives a header, both
    /// the corner
    static int childIndex(int row, int column) { return (row + 1) * ChildColumns + column + 1; }

    explicit BScheduleGridAccessible(BScheduleGridWidget *grid);

    void *interface_cast(QAccessible::InterfaceType type) override;

    QString text(QAccessible::Text type) const override;
    int childCount() const override;
    QAccessibleInterface *child(int index) const override;
    int indexOfChild(const QAccessibleInterface *child) const override;
    QAccessibleInterface *childAt(int x, int y) const override;
    QAccessibleInterface *focusChild() const override;

    // QAccessibleTableInterface
    QAccessibleInterface *caption() const override { return nullptr; }
    QAccessibleInterface *summary() const override { return nullptr; }
    QAccessibleInterface *cellAt(int row, int column) const override;
    int selectedCellCount() const override;
    QList<QAccessibleInterface *> selectedCells() const override;
    QString columnDescription(int column) const override;
    QString rowDescription(int row) const override;
    int selectedColumnCount() const override;
    int selectedRowCount() const override;
    int columnCount() const override;
    int rowCount() const override;
    QList<int> selectedColumns() const override;
    QList<int> selectedRows() const override;
    bool isColumnSelected(int column) const override;
    bool isRowSelected(int row) const override;
    bool selectRow(int row) override;
    bool selectColumn(int column) override;
    bool unselectRow(int row) override;
    bool unselectColumn(int column) override;
    void modelChange(QAccessibleTableModelChangeEvent *event) override;

protected:
    ~BScheduleGridAccessible() override;

private:
    BScheduleGridWidget *grid() const;

    /// The interfaces of the children made so far, by child index
    mutable QHash<int, QAccessible::Id> m_children;
};

#endif // BSCHEDULEGRIDACCESSIBLE_H
