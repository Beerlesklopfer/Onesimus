#ifndef BEDITABLELISTMODEL_H
#define BEDITABLELISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QUndoStack>
#include <functional>

/**
 * @file beditablelistmodel.h
 * @brief Generic editable list model with undo/redo support
 *
 * A reusable list model for any string list data with:
 * - Inline editing support (Qt::ItemIsEditable flag)
 * - Undo/redo via QUndoStack
 * - Optional validation function
 * - Signals for external observers
 *
 * Used for FileSet paths, Job run directives, Schedule statements, etc.
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */
class BEditableListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    /**
     * @brief Custom data roles
     */
    enum Roles {
        ValueRole = Qt::DisplayRole,          ///< Item value (same as DisplayRole)
        ValidationRole = Qt::UserRole + 1,    ///< Whether item is valid (bool)
        ValidationMessageRole                 ///< Validation error message (QString)
    };

    /**
     * @brief Construct an editable list model
     * @param parent Parent object
     */
    explicit BEditableListModel(QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~BEditableListModel() override = default;

    // =========================================================================
    // Undo Stack
    // =========================================================================

    /**
     * @brief Set shared undo stack (from parent widget/document)
     * @param stack Undo stack (not owned, must outlive model)
     */
    void setUndoStack(QUndoStack *stack);

    /**
     * @brief Get the undo stack
     * @return Pointer to undo stack, or nullptr if not set
     */
    QUndoStack *undoStack() const;

    // =========================================================================
    // QAbstractListModel interface
    // =========================================================================

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

    // =========================================================================
    // List Operations (with undo support when stack is set)
    // =========================================================================

    /**
     * @brief Add an item to the list
     * @param value Value to add
     * @param row Row to insert at (-1 for end)
     */
    void addItem(const QString &value, int row = -1);

    /**
     * @brief Add multiple items to the list
     * @param values Values to add
     */
    void addItems(const QStringList &values);

    /**
     * @brief Remove an item by row
     * @param row Row to remove
     */
    void removeItem(int row);

    /**
     * @brief Remove multiple items by rows (supports multi-select)
     * @param rows Rows to remove
     */
    void removeItems(const QList<int> &rows);

    /**
     * @brief Edit an item's value
     * @param row Row to edit
     * @param newValue New value
     */
    void editItem(int row, const QString &newValue);

    /**
     * @brief Move an item within the list
     * @param fromRow Source row
     * @param toRow Destination row
     */
    void moveItem(int fromRow, int toRow);

    /**
     * @brief Clear all items
     */
    void clear();

    // =========================================================================
    // Direct Access (for serialization)
    // =========================================================================

    /**
     * @brief Get all items
     * @return List of all item values
     */
    QStringList items() const;

    /**
     * @brief Set all items (no undo, for initialization)
     * @param items New item list
     */
    void setItems(const QStringList &items);

    /**
     * @brief Get item at row
     * @param row Row index
     * @return Item value, or empty string if invalid row
     */
    QString itemAt(int row) const;

    // =========================================================================
    // Validation
    // =========================================================================

    /**
     * @brief Set validation function
     * @param validator Function that returns error message (empty = valid)
     *
     * Example:
     * @code
     * model->setValidator([](const QString &path) -> QString {
     *     if (path.isEmpty()) return "Path cannot be empty";
     *     if (!path.startsWith("/")) return "Path must be absolute";
     *     return QString(); // Valid
     * });
     * @endcode
     */
    void setValidator(std::function<QString(const QString &)> validator);

    /**
     * @brief Validate an item value
     * @param value Value to validate
     * @return Error message, or empty string if valid
     */
    QString validateItem(const QString &value) const;

    /**
     * @brief Check if an item is valid
     * @param row Row to check
     * @return true if valid
     */
    bool isItemValid(int row) const;

signals:
    /**
     * @brief Emitted when an item is added
     * @param row Row where item was added
     * @param value Added value
     */
    void itemAdded(int row, const QString &value);

    /**
     * @brief Emitted when an item is removed
     * @param row Row that was removed
     */
    void itemRemoved(int row);

    /**
     * @brief Emitted when an item is edited
     * @param row Row that was edited
     * @param oldValue Previous value
     * @param newValue New value
     */
    void itemEdited(int row, const QString &oldValue, const QString &newValue);

    /**
     * @brief Emitted when the model is modified (any change)
     */
    void modelModified();

private:
    // Internal operations (called by commands, no undo)
    friend class BAddItemCommand;
    friend class BRemoveItemsCommand;
    friend class BEditItemCommand;
    friend class BMoveItemCommand;

    void doAddItem(int row, const QString &value);
    void doRemoveItem(int row);
    void doEditItem(int row, const QString &value);
    void doMoveItem(int fromRow, int toRow);

    QStringList m_items;
    QUndoStack *m_undoStack = nullptr;
    std::function<QString(const QString &)> m_validator;
};

#endif // BEDITABLELISTMODEL_H
