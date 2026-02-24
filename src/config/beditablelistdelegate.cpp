/**
 * @file beditablelistdelegate.cpp
 * @brief Delegate for inline list item editing with optional buttons
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "config/beditablelistdelegate.h"
#include "config/beditablelistmodel.h"
#include <QPainter>
#include <QApplication>
#include <QLineEdit>
#include <QMouseEvent>
#include <QFileDialog>

BEditableListDelegate::BEditableListDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

// ============================================================================
// Configuration
// ============================================================================

void BEditableListDelegate::setShowBrowseButton(bool show)
{
    m_showBrowseButton = show;
}

bool BEditableListDelegate::showBrowseButton() const
{
    return m_showBrowseButton;
}

void BEditableListDelegate::setBrowseMode(BrowseMode mode)
{
    m_browseMode = mode;
}

BEditableListDelegate::BrowseMode BEditableListDelegate::browseMode() const
{
    return m_browseMode;
}

void BEditableListDelegate::setShowDeleteButton(bool show)
{
    m_showDeleteButton = show;
}

bool BEditableListDelegate::showDeleteButton() const
{
    return m_showDeleteButton;
}

void BEditableListDelegate::setPlaceholderText(const QString &text)
{
    m_placeholderText = text;
}

QString BEditableListDelegate::placeholderText() const
{
    return m_placeholderText;
}

// ============================================================================
// Geometry helpers
// ============================================================================

int BEditableListDelegate::buttonWidth() const
{
    return BUTTON_SIZE + BUTTON_MARGIN * 2;
}

QRect BEditableListDelegate::deleteButtonRect(const QStyleOptionViewItem &option) const
{
    if (!m_showDeleteButton) {
        return QRect();
    }

    int x = option.rect.right() - buttonWidth();
    int y = option.rect.top() + (option.rect.height() - BUTTON_SIZE) / 2;
    return QRect(x + BUTTON_MARGIN, y, BUTTON_SIZE, BUTTON_SIZE);
}

QRect BEditableListDelegate::browseButtonRect(const QStyleOptionViewItem &option) const
{
    if (!m_showBrowseButton) {
        return QRect();
    }

    int offset = m_showDeleteButton ? buttonWidth() : 0;
    int x = option.rect.right() - buttonWidth() - offset;
    int y = option.rect.top() + (option.rect.height() - BUTTON_SIZE) / 2;
    return QRect(x + BUTTON_MARGIN, y, BUTTON_SIZE, BUTTON_SIZE);
}

QRect BEditableListDelegate::textRect(const QStyleOptionViewItem &option) const
{
    int rightMargin = 0;
    if (m_showDeleteButton) rightMargin += buttonWidth();
    if (m_showBrowseButton) rightMargin += buttonWidth();

    QRect rect = option.rect;
    rect.setRight(rect.right() - rightMargin);
    rect.adjust(4, 0, -4, 0);  // Add some padding
    return rect;
}

// ============================================================================
// QStyledItemDelegate interface
// ============================================================================

void BEditableListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    painter->save();

    // Draw selection/hover background
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    // Check validation state
    bool isValid = index.data(BEditableListModel::ValidationRole).toBool();
    if (!isValid) {
        // Invalid item - light red background
        QColor invalidColor(255, 220, 220);
        if (opt.state & QStyle::State_Selected) {
            invalidColor = QColor(255, 180, 180);
        }
        painter->fillRect(opt.rect, invalidColor);
    }

    // Draw standard background (selection, hover)
    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter);

    // Draw text
    QString text = index.data(Qt::DisplayRole).toString();
    QRect textArea = textRect(opt);

    if (text.isEmpty() && !m_placeholderText.isEmpty()) {
        // Draw placeholder in gray
        painter->setPen(QColor(150, 150, 150));
        painter->drawText(textArea, Qt::AlignLeft | Qt::AlignVCenter, m_placeholderText);
    } else {
        painter->setPen(opt.palette.color(QPalette::Text));
        painter->drawText(textArea, Qt::AlignLeft | Qt::AlignVCenter, text);
    }

    // Get viewport widget safely for hover detection
    const QWidget *viewportWidget = opt.widget;

    // Draw browse button if enabled
    if (m_showBrowseButton) {
        QRect browseRect = browseButtonRect(opt);

        // Draw button background on hover
        if (viewportWidget) {
            QPoint mousePos = QCursor::pos();
            QPoint viewPos = viewportWidget->mapFromGlobal(mousePos);
            bool hovered = browseRect.contains(viewPos);
            if (hovered) {
                painter->fillRect(browseRect, QColor(200, 200, 200, 100));
            }
        }

        // Draw "..." text
        painter->setPen(QColor(80, 80, 80));
        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
        painter->drawText(browseRect, Qt::AlignCenter, "…");
    }

    // Draw delete button if enabled
    if (m_showDeleteButton) {
        QRect deleteRect = deleteButtonRect(opt);

        // Draw button background on hover
        if (viewportWidget) {
            QPoint mousePos = QCursor::pos();
            QPoint viewPos = viewportWidget->mapFromGlobal(mousePos);
            bool hovered = deleteRect.contains(viewPos);
            if (hovered) {
                painter->fillRect(deleteRect, QColor(255, 100, 100, 100));
            }
        }

        // Draw "×" text
        painter->setPen(QColor(150, 50, 50));
        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
        painter->drawText(deleteRect, Qt::AlignCenter, "×");
    }

    painter->restore();
}

QSize BEditableListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);

    // Ensure minimum height for buttons
    int minHeight = BUTTON_SIZE + BUTTON_MARGIN * 2 + 4;
    if (size.height() < minHeight) {
        size.setHeight(minHeight);
    }

    return size;
}

QWidget *BEditableListDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QLineEdit *editor = new QLineEdit(parent);
    editor->setFrame(false);

    if (!m_placeholderText.isEmpty()) {
        editor->setPlaceholderText(m_placeholderText);
    }

    return editor;
}

void BEditableListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
    if (lineEdit) {
        lineEdit->setText(index.data(Qt::EditRole).toString());
        lineEdit->selectAll();
    }
}

void BEditableListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
    if (lineEdit) {
        model->setData(index, lineEdit->text(), Qt::EditRole);
    }
}

void BEditableListDelegate::updateEditorGeometry(QWidget *editor,
                                                 const QStyleOptionViewItem &option,
                                                 const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(textRect(option));
}

bool BEditableListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint pos = mouseEvent->pos();

        // Check delete button click
        if (m_showDeleteButton) {
            QRect deleteRect = deleteButtonRect(option);
            if (deleteRect.contains(pos)) {
                emit deleteRequested(index);
                return true;
            }
        }

        // Check browse button click
        if (m_showBrowseButton) {
            QRect browseRect = browseButtonRect(option);
            if (browseRect.contains(pos)) {
                emit browseRequested(index);
                return true;
            }
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
