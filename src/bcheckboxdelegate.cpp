#include "jobs/bcheckboxdelegate.h"
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>

BCheckBoxDelegate::BCheckBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void BCheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    
    // Get checkbox state from model
    QVariant checkState = index.data(Qt::CheckStateRole);
    
    if (checkState.isValid()) {
        // Create checkbox style option
        QStyleOptionButton checkboxOption;
        checkboxOption.state = QStyle::State_Enabled;
        
        if (checkState.toInt() == Qt::Checked) {
            checkboxOption.state |= QStyle::State_On;
        } else {
            checkboxOption.state |= QStyle::State_Off;
        }
        
        // Center the checkbox
        int checkboxWidth = QApplication::style()->pixelMetric(QStyle::PM_IndicatorWidth);
        int checkboxHeight = QApplication::style()->pixelMetric(QStyle::PM_IndicatorHeight);
        
        checkboxOption.rect = QRect(
            option.rect.x() + (option.rect.width() - checkboxWidth) / 2,
            option.rect.y() + (option.rect.height() - checkboxHeight) / 2,
            checkboxWidth,
            checkboxHeight
        );
        
        // Draw background
        QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);
        
        // Draw checkbox
        style->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkboxOption, painter);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool BCheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        
        if (mouseEvent->button() == Qt::LeftButton) {
            // Toggle checkbox state
            QVariant currentState = index.data(Qt::CheckStateRole);
            if (currentState.isValid()) {
                Qt::CheckState newState = (currentState.toInt() == Qt::Checked) 
                                          ? Qt::Unchecked 
                                          : Qt::Checked;
                return model->setData(index, newState, Qt::CheckStateRole);
            }
        }
    }
    
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
