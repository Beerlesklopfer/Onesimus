/**
 * @file beditablelistwidget.cpp
 * @brief Complete editable list widget with add/remove, context menu, undo/redo
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "config/beditablelistwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QFileDialog>
#include <QInputDialog>

BEditableListWidget::BEditableListWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    createContextMenu();
}

BEditableListWidget::~BEditableListWidget()
{
    // Only delete model if we own it
    // If it's an external model (setModel was called), m_ownsModel is false
    // and the model will be deleted by its actual owner
    if (m_model && m_ownsModel) {
        // Model is owned by us as child QObject, Qt will delete it
        // but we set m_model to nullptr for clarity
        m_model = nullptr;
    }
}

void BEditableListWidget::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(4);

    // Title row (for compact mode - title with inline buttons)
    m_titleRow = new QWidget(this);
    QHBoxLayout *titleLayout = new QHBoxLayout(m_titleRow);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(4);

    m_titleLabel = new QLabel(this);
    titleLayout->addWidget(m_titleLabel);
    titleLayout->addStretch();

    m_titleRow->setVisible(false);  // Hidden by default
    mainLayout->addWidget(m_titleRow);

    // List view
    m_listView = new QListView(this);
    m_listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_listView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_listView->setEditTriggers(QAbstractItemView::DoubleClicked |
                                QAbstractItemView::EditKeyPressed);

    // Model
    m_model = new BEditableListModel(this);
    m_listView->setModel(m_model);

    // Delegate
    m_delegate = new BEditableListDelegate(this);
    m_listView->setItemDelegate(m_delegate);

    mainLayout->addWidget(m_listView, 1);

    // Button row - with top margin to prevent overlap with list items
    m_buttonRow = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(m_buttonRow);
    buttonLayout->setContentsMargins(0, 4, 0, 0);

    m_addButton = new QPushButton(tr("Add..."), this);
    m_removeButton = new QPushButton(tr("Remove"), this);
    m_removeButton->setEnabled(false);

    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_removeButton);
    buttonLayout->addStretch();

    mainLayout->addWidget(m_buttonRow);

    // Connections
    connect(m_addButton, &QPushButton::clicked, this, &BEditableListWidget::onAddClicked);
    connect(m_removeButton, &QPushButton::clicked, this, &BEditableListWidget::onRemoveClicked);
    connect(m_listView, &QListView::customContextMenuRequested, this, &BEditableListWidget::onContextMenu);
    connect(m_listView, &QListView::doubleClicked, this, &BEditableListWidget::onDoubleClicked);

    connect(m_listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &BEditableListWidget::updateRemoveButtonState);

    // Delegate signals
    connect(m_delegate, &BEditableListDelegate::deleteRequested,
            this, &BEditableListWidget::onDeleteRequested);
    connect(m_delegate, &BEditableListDelegate::browseRequested,
            this, &BEditableListWidget::onBrowseRequested);

    // Forward model signals
    connectModelSignals();
}

void BEditableListWidget::createContextMenu()
{
    m_contextMenu = new QMenu(this);

    m_actionAdd = m_contextMenu->addAction(tr("Add..."));
    connect(m_actionAdd, &QAction::triggered, this, &BEditableListWidget::onAddClicked);

    m_contextMenu->addSeparator();

    m_actionEdit = m_contextMenu->addAction(tr("Edit"));
    connect(m_actionEdit, &QAction::triggered, this, [this]() {
        QModelIndex current = m_listView->currentIndex();
        if (current.isValid()) {
            m_listView->edit(current);
        }
    });

    m_actionRemove = m_contextMenu->addAction(tr("Remove"));
    m_actionRemove->setShortcut(QKeySequence::Delete);
    connect(m_actionRemove, &QAction::triggered, this, &BEditableListWidget::onRemoveClicked);
}

void BEditableListWidget::updateRemoveButtonState()
{
    bool hasSelection = !m_listView->selectionModel()->selectedIndexes().isEmpty();
    m_removeButton->setEnabled(hasSelection);
}

// ============================================================================
// Model Access
// ============================================================================

BEditableListModel *BEditableListWidget::model() const
{
    return m_model;
}

void BEditableListWidget::setModel(BEditableListModel *model)
{
    if (m_model == model) {
        return;
    }

    // Disconnect old model signals
    disconnectModelSignals();

    // Delete old model if we own it
    if (m_model && m_ownsModel) {
        delete m_model;
    }

    // Set new model
    m_model = model;
    m_ownsModel = false;  // External model - we don't own it

    // Set on list view
    m_listView->setModel(m_model);

    // Reconnect selection changed signal
    connect(m_listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &BEditableListWidget::updateRemoveButtonState);

    // Connect new model signals
    connectModelSignals();

    // Set undo stack if we have one
    if (m_undoStack && m_model) {
        m_model->setUndoStack(m_undoStack);
    }
}

QListView *BEditableListWidget::listView() const
{
    return m_listView;
}

void BEditableListWidget::connectModelSignals()
{
    if (!m_model) return;

    connect(m_model, &BEditableListModel::itemAdded, this, &BEditableListWidget::itemAdded);
    connect(m_model, &BEditableListModel::itemRemoved, this, &BEditableListWidget::itemRemoved);
    connect(m_model, &BEditableListModel::itemEdited, this, &BEditableListWidget::itemEdited);
    connect(m_model, &BEditableListModel::modelModified, this, &BEditableListWidget::modified);
}

void BEditableListWidget::disconnectModelSignals()
{
    if (!m_model) return;

    disconnect(m_model, &BEditableListModel::itemAdded, this, &BEditableListWidget::itemAdded);
    disconnect(m_model, &BEditableListModel::itemRemoved, this, &BEditableListWidget::itemRemoved);
    disconnect(m_model, &BEditableListModel::itemEdited, this, &BEditableListWidget::itemEdited);
    disconnect(m_model, &BEditableListModel::modelModified, this, &BEditableListWidget::modified);
}

// ============================================================================
// Configuration
// ============================================================================

void BEditableListWidget::setUndoStack(QUndoStack *stack)
{
    m_undoStack = stack;
    m_model->setUndoStack(stack);
}

QUndoStack *BEditableListWidget::undoStack() const
{
    return m_undoStack;
}

void BEditableListWidget::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
    // In compact mode, show title row; otherwise show just the label
    if (m_compactMode) {
        m_titleRow->setVisible(!title.isEmpty());
    } else {
        m_titleLabel->setVisible(!title.isEmpty());
    }
}

QString BEditableListWidget::title() const
{
    return m_titleLabel->text();
}

void BEditableListWidget::setCompactMode(bool compact)
{
    if (m_compactMode == compact) return;

    m_compactMode = compact;

    if (compact) {
        // Move buttons to title row
        QHBoxLayout *titleLayout = qobject_cast<QHBoxLayout*>(m_titleRow->layout());
        if (titleLayout) {
            // Create small icon buttons for compact mode
            QPushButton *addBtn = new QPushButton("+", this);
            addBtn->setFixedSize(24, 24);
            addBtn->setToolTip(tr("Add item"));
            connect(addBtn, &QPushButton::clicked, this, &BEditableListWidget::onAddClicked);

            QPushButton *removeBtn = new QPushButton("-", this);
            removeBtn->setFixedSize(24, 24);
            removeBtn->setToolTip(tr("Remove selected"));
            removeBtn->setEnabled(false);
            connect(removeBtn, &QPushButton::clicked, this, &BEditableListWidget::onRemoveClicked);

            // Connect selection change to update remove button
            connect(m_listView->selectionModel(), &QItemSelectionModel::selectionChanged,
                    this, [removeBtn, this]() {
                removeBtn->setEnabled(!m_listView->selectionModel()->selectedIndexes().isEmpty());
            });

            titleLayout->addWidget(addBtn);
            titleLayout->addWidget(removeBtn);
        }

        // Hide normal button row
        m_buttonRow->setVisible(false);

        // Show title row if we have a title
        m_titleRow->setVisible(!m_titleLabel->text().isEmpty());
        m_titleLabel->setVisible(false);  // Label is in title row now
    } else {
        // Show normal buttons, hide title row
        m_buttonRow->setVisible(true);
        m_titleRow->setVisible(false);
        m_titleLabel->setVisible(!m_titleLabel->text().isEmpty());
    }
}

void BEditableListWidget::setAddButtonText(const QString &text)
{
    m_addButton->setText(text);
}

void BEditableListWidget::setPlaceholderText(const QString &text)
{
    m_placeholderText = text;
    m_delegate->setPlaceholderText(text);
}

QString BEditableListWidget::placeholderText() const
{
    return m_placeholderText;
}

void BEditableListWidget::setBrowseMode(BEditableListDelegate::BrowseMode mode)
{
    m_delegate->setShowBrowseButton(mode != BEditableListDelegate::BrowseNone);
    m_delegate->setBrowseMode(mode);
}

BEditableListDelegate::BrowseMode BEditableListWidget::browseMode() const
{
    return m_delegate->browseMode();
}

void BEditableListWidget::setValidator(std::function<QString(const QString &)> validator)
{
    m_model->setValidator(validator);
}

void BEditableListWidget::setShowDeleteButton(bool show)
{
    m_delegate->setShowDeleteButton(show);
}

// ============================================================================
// Selection
// ============================================================================

QList<int> BEditableListWidget::selectedRows() const
{
    QList<int> rows;
    const QModelIndexList indexes = m_listView->selectionModel()->selectedIndexes();
    for (const QModelIndex &index : indexes) {
        if (!rows.contains(index.row())) {
            rows.append(index.row());
        }
    }
    return rows;
}

void BEditableListWidget::clearSelection()
{
    m_listView->clearSelection();
}

void BEditableListWidget::selectAll()
{
    m_listView->selectAll();
}

// ============================================================================
// Convenience Methods
// ============================================================================

void BEditableListWidget::addItem(const QString &value)
{
    m_model->addItem(value);
}

QStringList BEditableListWidget::items() const
{
    return m_model->items();
}

void BEditableListWidget::setItems(const QStringList &items)
{
    m_model->setItems(items);
}

void BEditableListWidget::clear()
{
    m_model->clear();
}

// ============================================================================
// Event Handlers
// ============================================================================

void BEditableListWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        if (m_listView->hasFocus()) {
            onRemoveClicked();
            event->accept();
            return;
        }
    }
    QWidget::keyPressEvent(event);
}

void BEditableListWidget::onAddClicked()
{
    QString value;

    // Use browse dialog if browse mode is set
    BEditableListDelegate::BrowseMode mode = m_delegate->browseMode();
    if (m_delegate->showBrowseButton() && mode != BEditableListDelegate::BrowseNone) {
        if (mode == BEditableListDelegate::BrowseDirectory) {
            value = QFileDialog::getExistingDirectory(this, tr("Select Directory"));
        } else {
            value = QFileDialog::getOpenFileName(this, tr("Select File"));
        }
    } else {
        // Use text input dialog
        bool ok;
        value = QInputDialog::getText(this, tr("Add Item"),
                                      m_placeholderText.isEmpty() ? tr("Enter value:") : m_placeholderText,
                                      QLineEdit::Normal, QString(), &ok);
        if (!ok) {
            return;
        }
    }

    if (!value.isEmpty()) {
        m_model->addItem(value);
    }
}

void BEditableListWidget::onRemoveClicked()
{
    QList<int> rows = selectedRows();
    if (!rows.isEmpty()) {
        m_model->removeItems(rows);
    }
}

void BEditableListWidget::onContextMenu(const QPoint &pos)
{
    QModelIndex index = m_listView->indexAt(pos);

    // Update action states
    m_actionEdit->setEnabled(index.isValid());
    m_actionRemove->setEnabled(!selectedRows().isEmpty());

    m_contextMenu->exec(m_listView->viewport()->mapToGlobal(pos));
}

void BEditableListWidget::onDeleteRequested(const QModelIndex &index)
{
    if (index.isValid()) {
        m_model->removeItem(index.row());
    }
}

void BEditableListWidget::onBrowseRequested(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    QString currentValue = index.data(Qt::DisplayRole).toString();
    QString newValue;

    BEditableListDelegate::BrowseMode mode = m_delegate->browseMode();
    if (mode == BEditableListDelegate::BrowseDirectory) {
        newValue = QFileDialog::getExistingDirectory(this, tr("Select Directory"), currentValue);
    } else {
        newValue = QFileDialog::getOpenFileName(this, tr("Select File"), currentValue);
    }

    if (!newValue.isEmpty() && newValue != currentValue) {
        m_model->editItem(index.row(), newValue);
    }
}

void BEditableListWidget::onDoubleClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        m_listView->edit(index);
    }
}
