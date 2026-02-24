#ifndef BEDITABLELISTWIDGET_H
#define BEDITABLELISTWIDGET_H

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QUndoStack>
#include <QLabel>
#include <functional>

#include "config/beditablelistmodel.h"
#include "config/beditablelistdelegate.h"

/**
 * @file beditablelistwidget.h
 * @brief Complete editable list widget with add/remove, context menu, undo/redo
 *
 * A reusable widget combining:
 * - QListView with BEditableListModel
 * - BEditableListDelegate for inline editing
 * - Add/Remove buttons
 * - Context menu (Add, Edit, Remove)
 * - Delete key handling
 * - Multi-selection support
 * - Optional browse button per item
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */
class BEditableListWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct an editable list widget
     * @param parent Parent widget
     */
    explicit BEditableListWidget(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~BEditableListWidget() override;

    // =========================================================================
    // Model Access
    // =========================================================================

    /**
     * @brief Get the model
     * @return Pointer to the model
     */
    BEditableListModel *model() const;

    /**
     * @brief Set an external model (replaces internal model)
     *
     * Use this to share a model with BFileSetDocument or other components.
     * The widget does NOT take ownership of the external model.
     *
     * @param model External model to use (not owned)
     */
    void setModel(BEditableListModel *model);

    /**
     * @brief Get the list view
     * @return Pointer to the list view
     */
    QListView *listView() const;

    // =========================================================================
    // Configuration
    // =========================================================================

    /**
     * @brief Set shared undo stack
     * @param stack Undo stack (not owned)
     */
    void setUndoStack(QUndoStack *stack);

    /**
     * @brief Get the undo stack
     * @return Pointer to undo stack
     */
    QUndoStack *undoStack() const;

    /**
     * @brief Set widget title (shown as label above list)
     * @param title Title text (empty to hide)
     */
    void setTitle(const QString &title);

    /**
     * @brief Get widget title
     * @return Title text
     */
    QString title() const;

    /**
     * @brief Set add button text
     * @param text Button text
     */
    void setAddButtonText(const QString &text);

    /**
     * @brief Set placeholder text for empty items
     * @param text Placeholder text
     */
    void setPlaceholderText(const QString &text);

    /**
     * @brief Get placeholder text
     * @return Placeholder text
     */
    QString placeholderText() const;

    /**
     * @brief Enable browse button on each item
     * @param mode Browse mode (File, Directory, or None)
     */
    void setBrowseMode(BEditableListDelegate::BrowseMode mode);

    /**
     * @brief Get browse mode
     * @return Current browse mode
     */
    BEditableListDelegate::BrowseMode browseMode() const;

    /**
     * @brief Set validation function
     * @param validator Function that returns error message (empty = valid)
     */
    void setValidator(std::function<QString(const QString &)> validator);

    /**
     * @brief Show or hide the delete button on items
     * @param show true to show
     */
    void setShowDeleteButton(bool show);

    /**
     * @brief Enable compact mode (buttons in title bar, no separate button row)
     * @param compact true to enable compact mode
     */
    void setCompactMode(bool compact);

    // =========================================================================
    // Selection
    // =========================================================================

    /**
     * @brief Get selected row indices
     * @return List of selected rows
     */
    QList<int> selectedRows() const;

    /**
     * @brief Clear selection
     */
    void clearSelection();

    /**
     * @brief Select all items
     */
    void selectAll();

    // =========================================================================
    // Convenience Methods
    // =========================================================================

    /**
     * @brief Add an item to the list
     * @param value Value to add
     */
    void addItem(const QString &value);

    /**
     * @brief Get all items
     * @return List of all values
     */
    QStringList items() const;

    /**
     * @brief Set all items (replaces existing)
     * @param items New item list
     */
    void setItems(const QStringList &items);

    /**
     * @brief Clear all items
     */
    void clear();

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
     * @brief Emitted when the list is modified (any change)
     */
    void modified();

protected:
    /**
     * @brief Handle key press events
     * @param event Key event
     */
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onContextMenu(const QPoint &pos);
    void onDeleteRequested(const QModelIndex &index);
    void onBrowseRequested(const QModelIndex &index);
    void onDoubleClicked(const QModelIndex &index);

private:
    void setupUi();
    void createContextMenu();
    void updateRemoveButtonState();

    QLabel *m_titleLabel = nullptr;
    QListView *m_listView = nullptr;
    BEditableListModel *m_model = nullptr;
    BEditableListDelegate *m_delegate = nullptr;

    QPushButton *m_addButton = nullptr;
    QPushButton *m_removeButton = nullptr;

    QMenu *m_contextMenu = nullptr;
    QAction *m_actionAdd = nullptr;
    QAction *m_actionRemove = nullptr;
    QAction *m_actionEdit = nullptr;

    QUndoStack *m_undoStack = nullptr;
    QString m_placeholderText;
    bool m_ownsModel = true;  ///< true if we own and should delete the model
    bool m_compactMode = false;
    QWidget *m_buttonRow = nullptr;  ///< Container for add/remove buttons
    QWidget *m_titleRow = nullptr;   ///< Container for title + compact buttons

    void connectModelSignals();
    void disconnectModelSignals();
};

#endif // BEDITABLELISTWIDGET_H
