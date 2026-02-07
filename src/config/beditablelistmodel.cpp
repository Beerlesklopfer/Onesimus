/**
 * @file beditablelistmodel.cpp
 * @brief Generic editable list model with undo/redo support
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */

#include "config/beditablelistmodel.h"
#include "config/bundocommands.h"

BEditableListModel::BEditableListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void BEditableListModel::setUndoStack(QUndoStack *stack)
{
    m_undoStack = stack;
}

QUndoStack *BEditableListModel::undoStack() const
{
    return m_undoStack;
}

// ============================================================================
// QAbstractListModel interface
// ============================================================================

int BEditableListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_items.count();
}

QVariant BEditableListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.count()) {
        return QVariant();
    }

    const QString &item = m_items.at(index.row());

    switch (role) {
    case Qt::DisplayRole:  // ValueRole is alias for DisplayRole
    case Qt::EditRole:
        return item;

    case ValidationRole:
        return isItemValid(index.row());

    case ValidationMessageRole:
        return validateItem(item);

    default:
        return QVariant();
    }
}

bool BEditableListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.count()) {
        return false;
    }

    if (role == Qt::EditRole) {
        QString newValue = value.toString();
        if (newValue != m_items.at(index.row())) {
            editItem(index.row(), newValue);
        }
        return true;
    }

    return false;
}

Qt::ItemFlags BEditableListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> BEditableListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[ValueRole] = "value";
    roles[ValidationRole] = "valid";
    roles[ValidationMessageRole] = "validationMessage";
    return roles;
}

// ============================================================================
// List Operations (with undo support)
// ============================================================================

void BEditableListModel::addItem(const QString &value, int row)
{
    if (m_undoStack) {
        m_undoStack->push(new BAddItemCommand(this, value, row));
    } else {
        int insertRow = (row < 0) ? m_items.count() : row;
        doAddItem(insertRow, value);
    }
}

void BEditableListModel::addItems(const QStringList &values)
{
    if (values.isEmpty()) {
        return;
    }

    // Use a macro command to group all additions
    if (m_undoStack) {
        m_undoStack->beginMacro(tr("Add %1 items").arg(values.count()));
        for (const QString &value : values) {
            m_undoStack->push(new BAddItemCommand(this, value));
        }
        m_undoStack->endMacro();
    } else {
        for (const QString &value : values) {
            doAddItem(m_items.count(), value);
        }
    }
}

void BEditableListModel::removeItem(int row)
{
    removeItems({row});
}

void BEditableListModel::removeItems(const QList<int> &rows)
{
    if (rows.isEmpty()) {
        return;
    }

    if (m_undoStack) {
        m_undoStack->push(new BRemoveItemsCommand(this, rows));
    } else {
        // Sort descending and remove
        QList<int> sortedRows = rows;
        std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());
        for (int row : sortedRows) {
            if (row >= 0 && row < m_items.count()) {
                doRemoveItem(row);
            }
        }
    }
}

void BEditableListModel::editItem(int row, const QString &newValue)
{
    if (row < 0 || row >= m_items.count()) {
        return;
    }

    if (m_items.at(row) == newValue) {
        return;
    }

    if (m_undoStack) {
        m_undoStack->push(new BEditItemCommand(this, row, newValue));
    } else {
        doEditItem(row, newValue);
    }
}

void BEditableListModel::moveItem(int fromRow, int toRow)
{
    if (fromRow < 0 || fromRow >= m_items.count() ||
        toRow < 0 || toRow >= m_items.count() ||
        fromRow == toRow) {
        return;
    }

    if (m_undoStack) {
        m_undoStack->push(new BMoveItemCommand(this, fromRow, toRow));
    } else {
        doMoveItem(fromRow, toRow);
    }
}

void BEditableListModel::clear()
{
    if (m_items.isEmpty()) {
        return;
    }

    // Create list of all rows to remove
    QList<int> allRows;
    for (int i = 0; i < m_items.count(); ++i) {
        allRows.append(i);
    }
    removeItems(allRows);
}

// ============================================================================
// Direct Access
// ============================================================================

QStringList BEditableListModel::items() const
{
    return m_items;
}

void BEditableListModel::setItems(const QStringList &items)
{
    beginResetModel();
    m_items = items;
    endResetModel();
    emit modelModified();
}

QString BEditableListModel::itemAt(int row) const
{
    if (row < 0 || row >= m_items.count()) {
        return QString();
    }
    return m_items.at(row);
}

// ============================================================================
// Validation
// ============================================================================

void BEditableListModel::setValidator(std::function<QString(const QString &)> validator)
{
    m_validator = validator;

    // Refresh validation state for all items
    if (m_items.count() > 0) {
        emit dataChanged(index(0), index(m_items.count() - 1),
                         {ValidationRole, ValidationMessageRole});
    }
}

QString BEditableListModel::validateItem(const QString &value) const
{
    if (m_validator) {
        return m_validator(value);
    }
    return QString();  // No validator = always valid
}

bool BEditableListModel::isItemValid(int row) const
{
    if (row < 0 || row >= m_items.count()) {
        return false;
    }
    return validateItem(m_items.at(row)).isEmpty();
}

// ============================================================================
// Internal Operations (called by commands)
// ============================================================================

void BEditableListModel::doAddItem(int row, const QString &value)
{
    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, value);
    endInsertRows();
    emit itemAdded(row, value);
    emit modelModified();
}

void BEditableListModel::doRemoveItem(int row)
{
    if (row < 0 || row >= m_items.count()) {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
    emit itemRemoved(row);
    emit modelModified();
}

void BEditableListModel::doEditItem(int row, const QString &value)
{
    if (row < 0 || row >= m_items.count()) {
        return;
    }

    QString oldValue = m_items.at(row);
    m_items[row] = value;
    QModelIndex idx = index(row);
    emit dataChanged(idx, idx, {Qt::DisplayRole, Qt::EditRole, ValueRole,
                                ValidationRole, ValidationMessageRole});
    emit itemEdited(row, oldValue, value);
    emit modelModified();
}

void BEditableListModel::doMoveItem(int fromRow, int toRow)
{
    if (fromRow < 0 || fromRow >= m_items.count() ||
        toRow < 0 || toRow >= m_items.count()) {
        return;
    }

    // Qt's beginMoveRows has specific semantics
    int destRow = (toRow > fromRow) ? toRow + 1 : toRow;

    if (!beginMoveRows(QModelIndex(), fromRow, fromRow, QModelIndex(), destRow)) {
        return;
    }

    m_items.move(fromRow, toRow);
    endMoveRows();
    emit modelModified();
}
