// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BSORTTABLEITEM_H
#define BSORTTABLEITEM_H

#include <QTableWidgetItem>

/**
 * @brief QTableWidget item that sorts by a key instead of its text
 *
 * QTableWidget sorts with the items' operator<, which compares the display
 * text. This item shows formatted text ("1.50 GB") and compares the raw
 * value stored under SortRole.
 */
class BSortTableItem : public QTableWidgetItem
{
public:
    static constexpr int SortRole = Qt::UserRole + 1;

    BSortTableItem(const QString &text, qint64 sortKey)
        : QTableWidgetItem(text)
    {
        setData(SortRole, sortKey);
    }

    bool operator<(const QTableWidgetItem &other) const override
    {
        const QVariant mine = data(SortRole);
        const QVariant theirs = other.data(SortRole);
        if (mine.isValid() && theirs.isValid()) {
            return mine.toLongLong() < theirs.toLongLong();
        }
        return QTableWidgetItem::operator<(other);
    }
};

#endif // BSORTTABLEITEM_H
