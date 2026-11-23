// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bschedulegridaccessible.cpp
 * @brief The schedule grid as a table for screen readers
 */

#include "schedules/bschedulegridaccessible.h"

#include "schedules/bschedulegrideditor.h"
#include "schedules/bschedulegridwidget.h"
#include "schedules/model/bschedulegridtext.h"

#include <QPointer>

namespace {

/// One child of the table: a cell, a header or the corner
class BScheduleGridAccessibleCell : public QAccessibleInterface,
                                    public QAccessibleTableCellInterface,
                                    public QAccessibleActionInterface
{
public:
    BScheduleGridAccessibleCell(BScheduleGridWidget *grid, int row, int column)
        : m_grid(grid), m_row(row), m_column(column)
    {
    }

    bool isCell() const { return m_row >= 0 && m_column >= 0; }
    bool isCorner() const { return m_row < 0 && m_column < 0; }
    bool isRowHeader() const { return m_row >= 0 && m_column < 0; }
    bool isColumnHeader() const { return m_row < 0 && m_column >= 0; }

    void *interface_cast(QAccessible::InterfaceType type) override
    {
        if (type == QAccessible::TableCellInterface && isCell())
            return static_cast<QAccessibleTableCellInterface *>(this);
        if (type == QAccessible::ActionInterface)
            return static_cast<QAccessibleActionInterface *>(this);
        return nullptr;
    }

    bool isValid() const override { return m_grid && m_grid->model(); }
    QObject *object() const override { return nullptr; }
    QWindow *window() const override
    {
        return m_grid && m_grid->window() ? m_grid->window()->windowHandle() : nullptr;
    }

    QAccessibleInterface *parent() const override
    {
        return m_grid ? QAccessible::queryAccessibleInterface(m_grid) : nullptr;
    }
    QAccessibleInterface *child(int) const override { return nullptr; }
    QAccessibleInterface *childAt(int, int) const override { return nullptr; }
    int childCount() const override { return 0; }
    int indexOfChild(const QAccessibleInterface *) const override { return -1; }

    QString text(QAccessible::Text type) const override
    {
        if (!isValid())
            return QString();
        const BScheduleGridModel *model = m_grid->model();
        if (type == QAccessible::Name) {
            if (isCell())
                return BScheduleGridText::cellName(*model, m_row, m_column);
            if (isRowHeader())
                return BScheduleGridText::dayName(model->dayOfWeekOfRow(m_row));
            if (isColumnHeader())
                return BScheduleGridWidget::tr("%1 hours").arg(m_column);
            return BScheduleGridWidget::tr("Select all");
        }
        if (type == QAccessible::Description) {
            if (isRowHeader())
                return BScheduleGridWidget::tr("Selects the whole day");
            if (isColumnHeader())
                return BScheduleGridWidget::tr("Selects this hour on every day");
            if (isCorner())
                return BScheduleGridWidget::tr("Selects all days and hours");
        }
        return QString();
    }
    void setText(QAccessible::Text, const QString &) override {}

    QRect rect() const override
    {
        if (!isValid())
            return QRect();
        const BScheduleGridGeometry &geometry = m_grid->gridGeometry();
        QRect rect;
        if (isCell())
            rect = geometry.cellRect(m_row, m_column);
        else if (isRowHeader())
            rect = geometry.rowHeaderRect(m_row);
        else if (isColumnHeader())
            rect = geometry.columnHeaderRect(m_column);
        else
            rect = geometry.cornerRect();
        return QRect(m_grid->viewport()->mapToGlobal(rect.topLeft()), rect.size());
    }

    QAccessible::Role role() const override
    {
        if (isCell())
            return QAccessible::Cell;
        if (isRowHeader())
            return QAccessible::RowHeader;
        if (isColumnHeader())
            return QAccessible::ColumnHeader;
        return QAccessible::Button;
    }

    QAccessible::State state() const override
    {
        QAccessible::State state;
        if (!isValid()) {
            state.invalid = true;
            return state;
        }
        const BGridSelection &selection = m_grid->model()->selection();
        // A cell of an editable grid can be painted and erased
        state.readOnly = !(isCell() && m_grid->isEditable());
        if (isCell()) {
            state.selectable = true;
            state.multiSelectable = true;
            state.focusable = true;
            state.selected = selection.contains(m_row, m_column);
            state.focused = m_grid->hasFocus()
                && m_grid->currentCell() == BGridPos{m_row, m_column};
            const BScheduleGridGeometry &geometry = m_grid->gridGeometry();
            if (!geometry.cellRect(m_row, m_column).intersects(geometry.scrollAreaRect()))
                state.offscreen = true;
        } else if (isRowHeader()) {
            state.selectable = true;
            state.selected = selection.isRowSelected(m_row);
        } else if (isColumnHeader()) {
            state.selectable = true;
            state.selected = selection.isColumnSelected(m_column);
        }
        return state;
    }

    // --- QAccessibleTableCellInterface (cells only) ---

    int columnExtent() const override { return 1; }
    int rowExtent() const override { return 1; }
    int columnIndex() const override { return m_column; }
    int rowIndex() const override { return m_row; }
    bool isSelected() const override
    {
        return isValid() && m_grid->model()->selection().contains(m_row, m_column);
    }
    QList<QAccessibleInterface *> columnHeaderCells() const override
    {
        QAccessibleInterface *table = parent();
        QAccessibleInterface *header = table
            ? table->child(BScheduleGridAccessible::childIndex(-1, m_column)) : nullptr;
        return header ? QList<QAccessibleInterface *>{header} : QList<QAccessibleInterface *>();
    }
    QList<QAccessibleInterface *> rowHeaderCells() const override
    {
        QAccessibleInterface *table = parent();
        QAccessibleInterface *header = table
            ? table->child(BScheduleGridAccessible::childIndex(m_row, -1)) : nullptr;
        return header ? QList<QAccessibleInterface *>{header} : QList<QAccessibleInterface *>();
    }
    QAccessibleInterface *table() const override { return parent(); }

    // --- QAccessibleActionInterface ---

    /// The names of the two actions of a cell of an editable grid
    static QString applyToolAction() { return QStringLiteral("ApplyTool"); }
    static QString eraseAction() { return QStringLiteral("Erase"); }

    QStringList actionNames() const override
    {
        QStringList names{pressAction()};
        if (isCell()) {
            names << showMenuAction() << setFocusAction();
            if (m_grid && m_grid->isEditable())
                names << applyToolAction() << eraseAction();
        }
        return names;
    }

    QString localizedActionName(const QString &actionName) const override
    {
        if (actionName == applyToolAction())
            return BScheduleGridWidget::tr("Apply the tool");
        if (actionName == eraseAction())
            return BScheduleGridWidget::tr("Erase");
        return QAccessibleActionInterface::localizedActionName(actionName);
    }

    QString localizedActionDescription(const QString &actionName) const override
    {
        if (actionName == pressAction()) {
            if (isCell())
                return BScheduleGridWidget::tr("Selects this cell");
            if (isRowHeader())
                return BScheduleGridWidget::tr("Selects the whole day");
            if (isColumnHeader())
                return BScheduleGridWidget::tr("Selects this hour on every day");
            return BScheduleGridWidget::tr("Selects all days and hours");
        }
        if (actionName == showMenuAction())
            return BScheduleGridWidget::tr("Lists the runs of this cell");
        if (actionName == applyToolAction()) {
            return BScheduleGridWidget::tr("Applies the tool in hand (%1) to this cell")
                .arg(m_grid && m_grid->editor() ? m_grid->editor()->toolText() : QString());
        }
        if (actionName == eraseAction())
            return BScheduleGridWidget::tr("Removes the runs of the scope on display from this cell");
        return QAccessibleActionInterface::localizedActionDescription(actionName);
    }

    void doAction(const QString &actionName) override
    {
        if (!isValid())
            return;
        BScheduleGridModel *model = m_grid->model();
        if (actionName == pressAction()) {
            BGridSelection selection = model->selection();
            if (isCell())
                selection.selectCell(m_row, m_column);
            else if (isRowHeader())
                selection.selectRow(m_row);
            else if (isColumnHeader())
                selection.selectColumn(m_column);
            else
                selection.selectAll();
            model->setSelection(selection);
            if (isCell())
                m_grid->setCurrentCell(BGridPos{m_row, m_column});
        } else if (isCell() && actionName == showMenuAction()) {
            m_grid->setCurrentCell(BGridPos{m_row, m_column});
            m_grid->showCellDetails(m_row, m_column);
        } else if (isCell() && actionName == setFocusAction()) {
            m_grid->setFocus();
            m_grid->setCurrentCell(BGridPos{m_row, m_column});
        } else if (isCell() && m_grid->isEditable()
                   && (actionName == applyToolAction() || actionName == eraseAction())) {
            // This cell only, whatever is selected
            BGridCellSet cell;
            cell.insert(m_row, m_column);
            if (actionName == applyToolAction())
                m_grid->editor()->applyTool(cell);
            else
                m_grid->editor()->erase(cell);
        }
    }

    QStringList keyBindingsForAction(const QString &) const override { return {}; }

private:
    QPointer<BScheduleGridWidget> m_grid;
    int m_row;
    int m_column;
};

QAccessibleInterface *gridFactory(const QString &className, QObject *object)
{
    if (className == QLatin1String("BScheduleGridWidget")) {
        if (auto *grid = qobject_cast<BScheduleGridWidget *>(object))
            return new BScheduleGridAccessible(grid);
    }
    return nullptr;
}

} // namespace

void BScheduleGridAccessible::install()
{
    static bool installed = false;
    if (installed)
        return;
    installed = true;
    QAccessible::installFactory(gridFactory);
}

BScheduleGridAccessible::BScheduleGridAccessible(BScheduleGridWidget *grid)
    : QAccessibleWidget(grid, QAccessible::Table)
{
}

BScheduleGridAccessible::~BScheduleGridAccessible()
{
    for (QAccessible::Id id : std::as_const(m_children))
        QAccessible::deleteAccessibleInterface(id);
}

BScheduleGridWidget *BScheduleGridAccessible::grid() const
{
    return qobject_cast<BScheduleGridWidget *>(object());
}

void *BScheduleGridAccessible::interface_cast(QAccessible::InterfaceType type)
{
    if (type == QAccessible::TableInterface)
        return static_cast<QAccessibleTableInterface *>(this);
    return QAccessibleWidget::interface_cast(type);
}

QString BScheduleGridAccessible::text(QAccessible::Text type) const
{
    const BScheduleGridWidget *widget = grid();
    const BScheduleGridModel *model = widget ? widget->model() : nullptr;
    if (!model)
        return QAccessibleWidget::text(type);

    if (type == QAccessible::Name) {
        return BScheduleGridWidget::tr("Schedule grid of %1, scope %2")
            .arg(model->loadedSchedule().name, BScheduleGridText::scopeLabel(model->activeScope()));
    }
    if (type == QAccessible::Description) {
        QStringList parts{BScheduleGridWidget::tr("Weekdays by hours. Arrow keys move, Shift extends "
                                                  "the selection, Enter lists the runs of a cell.")};
        if (const BScheduleGridEditor *editor = widget->editor()) {
            parts.append(BScheduleGridWidget::tr("Space applies the tool to the selected cells, "
                                                 "Delete erases them, F, D, I and V paint a level."));
            parts.append(BScheduleGridWidget::tr("Tool: %1.").arg(editor->toolText()));
            if (editor->paintMode())
                parts.append(BScheduleGridWidget::tr("Paint mode is on."));
        }
        const QList<BGridCalendarRule> &rules = model->calendarRules();
        for (int i = 0; i < rules.size(); ++i) {
            parts.append(BScheduleGridWidget::tr("Calendar rule %1: %2, %3")
                             .arg(BScheduleGridText::ruleTag(i), rules[i].text,
                                  BScheduleGridText::ruleReading(rules[i])));
        }
        return parts.join(QLatin1Char(' '));
    }
    return QAccessibleWidget::text(type);
}

int BScheduleGridAccessible::childCount() const
{
    return grid() && grid()->model() ? ChildRows * ChildColumns : 0;
}

QAccessibleInterface *BScheduleGridAccessible::child(int index) const
{
    BScheduleGridWidget *widget = grid();
    if (!widget || index < 0 || index >= ChildRows * ChildColumns)
        return nullptr;
    const auto found = m_children.constFind(index);
    if (found != m_children.constEnd()) {
        if (QAccessibleInterface *iface = QAccessible::accessibleInterface(found.value()))
            return iface;
    }
    auto *iface = new BScheduleGridAccessibleCell(widget, index / ChildColumns - 1,
                                                  index % ChildColumns - 1);
    m_children.insert(index, QAccessible::registerAccessibleInterface(iface));
    return iface;
}

int BScheduleGridAccessible::indexOfChild(const QAccessibleInterface *child) const
{
    if (!child)
        return -1;
    const QAccessible::Id id = QAccessible::uniqueId(const_cast<QAccessibleInterface *>(child));
    return m_children.key(id, -1);
}

QAccessibleInterface *BScheduleGridAccessible::childAt(int x, int y) const
{
    const BScheduleGridWidget *widget = grid();
    if (!widget)
        return nullptr;
    const QPoint pos = widget->viewport()->mapFromGlobal(QPoint(x, y));
    const BScheduleGridGeometry::Hit hit = widget->gridGeometry().hitTest(pos);
    switch (hit.part) {
    case BScheduleGridGeometry::Cell: return child(childIndex(hit.row, hit.column));
    case BScheduleGridGeometry::RowHeader: return child(childIndex(hit.row, -1));
    case BScheduleGridGeometry::ColumnHeader: return child(childIndex(-1, hit.column));
    case BScheduleGridGeometry::Corner: return child(childIndex(-1, -1));
    default: break;
    }
    return nullptr;
}

QAccessibleInterface *BScheduleGridAccessible::focusChild() const
{
    const BScheduleGridWidget *widget = grid();
    if (!widget || !widget->model() || !widget->hasFocus())
        return nullptr;
    return child(childIndex(widget->currentCell().row, widget->currentCell().column));
}

QAccessibleInterface *BScheduleGridAccessible::cellAt(int row, int column) const
{
    if (row < 0 || row >= rowCount() || column < 0 || column >= columnCount())
        return nullptr;
    return child(childIndex(row, column));
}

int BScheduleGridAccessible::selectedCellCount() const
{
    return grid() && grid()->model() ? grid()->model()->selection().count() : 0;
}

QList<QAccessibleInterface *> BScheduleGridAccessible::selectedCells() const
{
    QList<QAccessibleInterface *> cells;
    if (grid() && grid()->model()) {
        for (const BGridPos &pos : grid()->model()->selection().cells().cells())
            cells.append(child(childIndex(pos.row, pos.column)));
    }
    return cells;
}

QString BScheduleGridAccessible::columnDescription(int column) const
{
    return BScheduleGridWidget::tr("%1 hours").arg(column);
}

QString BScheduleGridAccessible::rowDescription(int row) const
{
    if (!grid() || !grid()->model() || row < 0 || row >= rowCount())
        return QString();
    return BScheduleGridText::dayName(grid()->model()->dayOfWeekOfRow(row));
}

int BScheduleGridAccessible::selectedColumnCount() const
{
    return int(selectedColumns().size());
}

int BScheduleGridAccessible::selectedRowCount() const
{
    return int(selectedRows().size());
}

int BScheduleGridAccessible::columnCount() const
{
    return BGridCellSet::Columns;
}

int BScheduleGridAccessible::rowCount() const
{
    return BGridCellSet::Rows;
}

QList<int> BScheduleGridAccessible::selectedColumns() const
{
    QList<int> columns;
    for (int column = 0; column < columnCount(); ++column) {
        if (isColumnSelected(column))
            columns.append(column);
    }
    return columns;
}

QList<int> BScheduleGridAccessible::selectedRows() const
{
    QList<int> rows;
    for (int row = 0; row < rowCount(); ++row) {
        if (isRowSelected(row))
            rows.append(row);
    }
    return rows;
}

bool BScheduleGridAccessible::isColumnSelected(int column) const
{
    return grid() && grid()->model() && grid()->model()->selection().isColumnSelected(column);
}

bool BScheduleGridAccessible::isRowSelected(int row) const
{
    return grid() && grid()->model() && grid()->model()->selection().isRowSelected(row);
}

bool BScheduleGridAccessible::selectRow(int row)
{
    if (!grid() || !grid()->model() || row < 0 || row >= rowCount())
        return false;
    BGridSelection selection = grid()->model()->selection();
    selection.selectRow(row, BGridSelection::Add);
    grid()->model()->setSelection(selection);
    return true;
}

bool BScheduleGridAccessible::selectColumn(int column)
{
    if (!grid() || !grid()->model() || column < 0 || column >= columnCount())
        return false;
    BGridSelection selection = grid()->model()->selection();
    selection.selectColumn(column, BGridSelection::Add);
    grid()->model()->setSelection(selection);
    return true;
}

bool BScheduleGridAccessible::unselectRow(int row)
{
    if (!isRowSelected(row))
        return false;
    BGridSelection selection = grid()->model()->selection();
    selection.selectRow(row, BGridSelection::Toggle);
    grid()->model()->setSelection(selection);
    return true;
}

bool BScheduleGridAccessible::unselectColumn(int column)
{
    if (!isColumnSelected(column))
        return false;
    BGridSelection selection = grid()->model()->selection();
    selection.selectColumn(column, BGridSelection::Toggle);
    grid()->model()->setSelection(selection);
    return true;
}

void BScheduleGridAccessible::modelChange(QAccessibleTableModelChangeEvent *)
{
    // The children are cells by position, not by content: nothing to drop
}
