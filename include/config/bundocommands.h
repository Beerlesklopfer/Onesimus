#ifndef BUNDOCOMMANDS_H
#define BUNDOCOMMANDS_H

#include <QUndoCommand>
#include <QString>
#include <QList>
#include <QPair>

class BEditableListModel;

/**
 * @file bundocommands.h
 * @brief Generic undo commands for editable list models
 *
 * Provides QUndoCommand implementations for common list operations:
 * - Add item
 * - Remove items (supports multi-select)
 * - Edit item (supports merge for continuous typing)
 * - Move item
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

/**
 * @brief Command IDs for merging consecutive edits
 */
enum BUndoCommandId {
    AddItemCmdId = 1000,
    RemoveItemsCmdId,
    EditItemCmdId,
    MoveItemCmdId
};

/**
 * @brief Add an item to an editable list model
 */
class BAddItemCommand : public QUndoCommand
{
public:
    /**
     * @brief Construct add item command
     * @param model Target model
     * @param value Value to add
     * @param row Row to insert at (-1 for end)
     * @param parent Parent undo command
     */
    BAddItemCommand(BEditableListModel *model, const QString &value,
                    int row = -1, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    BEditableListModel *m_model;
    QString m_value;
    int m_row;
};

/**
 * @brief Remove items from an editable list model (supports multi-select)
 */
class BRemoveItemsCommand : public QUndoCommand
{
public:
    /**
     * @brief Construct remove items command
     * @param model Target model
     * @param rows Rows to remove (will be sorted descending internally)
     * @param parent Parent undo command
     */
    BRemoveItemsCommand(BEditableListModel *model, const QList<int> &rows,
                        QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    BEditableListModel *m_model;
    QList<QPair<int, QString>> m_removedItems;  // row, value pairs (sorted descending)
};

/**
 * @brief Edit an item in the list model (supports merge for continuous typing)
 */
class BEditItemCommand : public QUndoCommand
{
public:
    /**
     * @brief Construct edit item command
     * @param model Target model
     * @param row Row to edit
     * @param newValue New value
     * @param parent Parent undo command
     */
    BEditItemCommand(BEditableListModel *model, int row,
                     const QString &newValue, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;
    int id() const override { return EditItemCmdId; }
    bool mergeWith(const QUndoCommand *other) override;

private:
    BEditableListModel *m_model;
    int m_row;
    QString m_oldValue;
    QString m_newValue;
};

/**
 * @brief Move an item within the list model
 */
class BMoveItemCommand : public QUndoCommand
{
public:
    /**
     * @brief Construct move item command
     * @param model Target model
     * @param fromRow Source row
     * @param toRow Destination row
     * @param parent Parent undo command
     */
    BMoveItemCommand(BEditableListModel *model, int fromRow, int toRow,
                     QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    BEditableListModel *m_model;
    int m_fromRow;
    int m_toRow;
};

#endif // BUNDOCOMMANDS_H
