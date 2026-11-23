// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BCHECKBOXDELEGATE_H
#define BCHECKBOXDELEGATE_H

#include <QStyledItemDelegate>

/**
 * @brief Custom delegate for rendering checkboxes in the selection column
 */
class BCheckBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit BCheckBoxDelegate(QObject *parent = nullptr);
    
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;
};

#endif // BCHECKBOXDELEGATE_H
