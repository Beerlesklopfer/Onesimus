/**
 * @file bundocommands.cpp
 * @brief Generic undo commands for editable list models
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "config/bundocommands.h"
#include "config/beditablelistmodel.h"
#include <algorithm>

// ============================================================================
// BAddItemCommand
// ============================================================================

BAddItemCommand::BAddItemCommand(BEditableListModel *model, const QString &value,
                                 int row, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_value(value)
    , m_row(row)
{
    if (m_row < 0) {
        m_row = m_model->rowCount();
    }
    setText(QObject::tr("Add \"%1\"").arg(value.left(30)));
}

void BAddItemCommand::undo()
{
    m_model->doRemoveItem(m_row);
}

void BAddItemCommand::redo()
{
    m_model->doAddItem(m_row, m_value);
}

// ============================================================================
// BRemoveItemsCommand
// ============================================================================

BRemoveItemsCommand::BRemoveItemsCommand(BEditableListModel *model, const QList<int> &rows,
                                         QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
{
    // Sort rows descending so we remove from bottom to top
    QList<int> sortedRows = rows;
    std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());

    // Store row, value pairs for undo
    for (int row : sortedRows) {
        if (row >= 0 && row < m_model->rowCount()) {
            m_removedItems.append({row, m_model->itemAt(row)});
        }
    }

    if (m_removedItems.size() == 1) {
        setText(QObject::tr("Remove \"%1\"").arg(m_removedItems.first().second.left(30)));
    } else {
        setText(QObject::tr("Remove %1 items").arg(m_removedItems.size()));
    }
}

void BRemoveItemsCommand::undo()
{
    // Re-add items in reverse order (ascending row order)
    for (int i = m_removedItems.size() - 1; i >= 0; --i) {
        const auto &item = m_removedItems[i];
        m_model->doAddItem(item.first, item.second);
    }
}

void BRemoveItemsCommand::redo()
{
    // Remove items in descending row order
    for (const auto &item : m_removedItems) {
        m_model->doRemoveItem(item.first);
    }
}

// ============================================================================
// BEditItemCommand
// ============================================================================

BEditItemCommand::BEditItemCommand(BEditableListModel *model, int row,
                                   const QString &newValue, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_row(row)
    , m_oldValue(model->itemAt(row))
    , m_newValue(newValue)
{
    setText(QObject::tr("Edit item"));
}

void BEditItemCommand::undo()
{
    m_model->doEditItem(m_row, m_oldValue);
}

void BEditItemCommand::redo()
{
    m_model->doEditItem(m_row, m_newValue);
}

bool BEditItemCommand::mergeWith(const QUndoCommand *other)
{
    // Merge consecutive edits to the same row
    if (other->id() != id()) {
        return false;
    }

    auto *otherEdit = static_cast<const BEditItemCommand *>(other);
    if (otherEdit->m_model != m_model || otherEdit->m_row != m_row) {
        return false;
    }

    // Merge: keep original old value, use new command's new value
    m_newValue = otherEdit->m_newValue;
    return true;
}

// ============================================================================
// BMoveItemCommand
// ============================================================================

BMoveItemCommand::BMoveItemCommand(BEditableListModel *model, int fromRow, int toRow,
                                   QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_fromRow(fromRow)
    , m_toRow(toRow)
{
    setText(QObject::tr("Move item"));
}

void BMoveItemCommand::undo()
{
    m_model->doMoveItem(m_toRow, m_fromRow);
}

void BMoveItemCommand::redo()
{
    m_model->doMoveItem(m_fromRow, m_toRow);
}
