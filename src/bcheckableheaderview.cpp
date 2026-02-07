#include "bcheckableheaderview.h"
#include "btranslations.h"
#include <QApplication>
#include <QStyle>
#include <QAbstractItemModel>
#include <QContextMenuEvent>
#include <QAction>

BCheckableHeaderView::BCheckableHeaderView(Qt::Orientation orientation, QWidget *parent)
    : QHeaderView(orientation, parent)
    , m_checkboxVisible(true)  // Checkbox visible by default
    , m_triStateEnabled(true)  // Enable tri-state by default
    , m_contextMenu(new QMenu(this))
{
    setSectionsClickable(true);
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

void BCheckableHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();
    
    // Only draw checkbox for the first column (index 0) if enabled
    if (m_checkboxVisible && logicalIndex == 0) {
        QStyleOptionButton option;
        option.rect = checkBoxRect(rect);
        option.state = QStyle::State_Enabled | QStyle::State_Active;
        
        Qt::CheckState checkState = getCheckState();
        
        if (m_triStateEnabled) {
            // Tri-state mode
            if (checkState == Qt::Checked) {
                option.state |= QStyle::State_On;
            } else if (checkState == Qt::PartiallyChecked) {
                option.state |= QStyle::State_NoChange;
            } else {
                option.state |= QStyle::State_Off;
            }
        } else {
            // Binary mode
            if (checkState == Qt::Checked || checkState == Qt::PartiallyChecked) {
                option.state |= QStyle::State_On;
            } else {
                option.state |= QStyle::State_Off;
            }
        }
        
        style()->drawControl(QStyle::CE_CheckBox, &option, painter);
    }
}

void BCheckableHeaderView::mousePressEvent(QMouseEvent *event)
{
    int logicalIndex = logicalIndexAt(event->pos());
    
    // Check if clicked on checkbox column
    if (m_checkboxVisible && logicalIndex == 0) {
        QRect sectionRect = QRect(sectionViewportPosition(0), 0, sectionSize(0), height());
        QRect cbRect = checkBoxRect(sectionRect);
        
        // Check if click was on the checkbox area
        if (cbRect.contains(event->pos())) {
            emit checkboxHeaderClicked(logicalIndex);
            // Don't call base class implementation to prevent sorting
            return;
        }
    }
    
    QHeaderView::mousePressEvent(event);
}

void BCheckableHeaderView::contextMenuEvent(QContextMenuEvent *event)
{
    showContextMenu(event->globalPos());
}

QRect BCheckableHeaderView::checkBoxRect(const QRect &rect) const
{
    QStyleOptionButton option;
    QRect checkBoxRect = style()->subElementRect(QStyle::SE_CheckBoxIndicator, &option);
    
    // Center the checkbox in the header section
    int x = rect.left() + (rect.width() - checkBoxRect.width()) / 2;
    int y = rect.top() + (rect.height() - checkBoxRect.height()) / 2;
    
    return QRect(x, y, checkBoxRect.width(), checkBoxRect.height());
}

void BCheckableHeaderView::showContextMenu(const QPoint &pos)
{
    m_contextMenu->clear();
    
    // Add column visibility options
    for (int i = 0; i < model()->columnCount(); ++i) {
        QString columnName = model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
        
        QAction *action = m_contextMenu->addAction(columnName);
        action->setCheckable(true);
        action->setChecked(!isSectionHidden(i));
        action->setData(i);
        
        // First column (checkbox) should not be hideable
        if (i == 0) {
            action->setEnabled(false);
        }
        
        connect(action, &QAction::triggered, [this, i, action]() {
            toggleColumnVisibility(i);
        });
    }
    
    m_contextMenu->addSeparator();
    
    // Add "Show All" option
    QAction *showAllAction = m_contextMenu->addAction(tr("Show All Columns"));
    connect(showAllAction, &QAction::triggered, [this]() {
        for (int i = 1; i < count(); ++i) {  // Skip first column
            if (isSectionHidden(i)) {
                toggleColumnVisibility(i);
            }
        }
    });
    
    // Add "Reset" option
    QAction *resetAction = m_contextMenu->addAction(tr("Reset Columns"));
    connect(resetAction, &QAction::triggered, [this]() {
        m_hiddenColumns.clear();
        for (int i = 0; i < count(); ++i) {
            setSectionHidden(i, false);
        }
    });
    
    m_contextMenu->exec(pos);
}

void BCheckableHeaderView::setHiddenColumns(const QSet<int> &columns)
{
    m_hiddenColumns = columns;
    for (int i = 0; i < count(); ++i) {
        setSectionHidden(i, columns.contains(i));
    }
}

void BCheckableHeaderView::toggleColumnVisibility(int column)
{
    if (column == 0) {
        return;  // Don't allow hiding the checkbox column
    }
    
    bool isHidden = isSectionHidden(column);
    setSectionHidden(column, !isHidden);
    
    if (isHidden) {
        m_hiddenColumns.remove(column);
    } else {
        m_hiddenColumns.insert(column);
    }
    
    emit columnVisibilityChanged(column, isHidden);
}

Qt::CheckState BCheckableHeaderView::getCheckState() const
{
    QVariant checkState = model()->headerData(0, orientation(), Qt::CheckStateRole);
    
    if (!checkState.isValid()) {
        return Qt::Unchecked;
    }
    
    int state = checkState.toInt();
    
    // If tri-state is disabled, convert PartiallyChecked to Checked
    if (!m_triStateEnabled && state == Qt::PartiallyChecked) {
        return Qt::Checked;
    }
    
    return static_cast<Qt::CheckState>(state);
}
