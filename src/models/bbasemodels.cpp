#include "models/bbasemodels.h"
#include "blogging.h"
#include <QJsonParseError>

// ============================================================================
// BListModel Implementation
// ============================================================================

BListModel::BListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int BListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

QVariant BListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        QJsonObject item = m_items[index.row()].toObject();
        return getDisplayText(item);
    }

    return QVariant();
}

void BListModel::setData(const QJsonArray &jsonArray)
{
    beginResetModel();
    m_items = jsonArray;
    endResetModel();
    emit dataUpdated();
}

void BListModel::setData(const QJsonDocument &jsonDoc, const QString &arrayKey)
{
    if (!jsonDoc.isObject()) {
        BLOG_WARNING() << "BListModel: JSON document is not an object";
        clear();
        return;
    }

    QJsonObject root = jsonDoc.object();
    QJsonObject result = root.value("result").toObject();
    QJsonArray dataArray = result.value(arrayKey).toArray();

    if (dataArray.isEmpty()) {
        BLOG_WARNING() << "BListModel: No data found for key" << arrayKey;
        clear();
        return;
    }

    setData(dataArray);
}

QJsonObject BListModel::itemAt(int row) const
{
    if (row >= 0 && row < m_items.size()) {
        return m_items[row].toObject();
    }
    return QJsonObject();
}

void BListModel::clear()
{
    beginResetModel();
    m_items = QJsonArray();
    endResetModel();
    emit dataUpdated();
}

QString BListModel::getDisplayText(const QJsonObject &item) const
{
    // Default: return "name" field if it exists
    if (item.contains("name")) {
        return item["name"].toString();
    }

    // Fallback: return first string value
    for (const QString &key : item.keys()) {
        if (item[key].isString()) {
            return item[key].toString();
        }
    }

    return QString();
}


// ============================================================================
// BTableModel Implementation
// ============================================================================

BTableModel::BTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int BTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

int BTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_columns.size();
}

QVariant BTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size() || index.column() >= m_columns.size())
        return QVariant();

    QJsonObject item = m_items[index.row()].toObject();

    if (role == Qt::DisplayRole) {
        return formatCellData(item, index.column());
    }

    if (role == Qt::TextAlignmentRole) {
        return m_columns[index.column()].alignment;
    }

    return QVariant();
}

QVariant BTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    if (section >= 0 && section < m_columns.size()) {
        return m_columns[section].headerText;
    }

    return QVariant();
}

void BTableModel::setData(const QJsonArray &jsonArray)
{
    beginResetModel();
    m_items = jsonArray;
    endResetModel();
    emit dataUpdated();
}

void BTableModel::setData(const QJsonDocument &jsonDoc, const QString &arrayKey)
{
    if (!jsonDoc.isObject()) {
        BLOG_WARNING() << "BTableModel: JSON document is not an object";
        clear();
        return;
    }

    QJsonObject root = jsonDoc.object();
    QJsonObject result = root.value("result").toObject();
    QJsonArray dataArray = result.value(arrayKey).toArray();

    if (dataArray.isEmpty()) {
        BLOG_WARNING() << "BTableModel: No data found for key" << arrayKey;
        clear();
        return;
    }

    setData(dataArray);
}

QJsonObject BTableModel::itemAt(int row) const
{
    if (row >= 0 && row < m_items.size()) {
        return m_items[row].toObject();
    }
    return QJsonObject();
}

void BTableModel::clear()
{
    beginResetModel();
    m_items = QJsonArray();
    endResetModel();
    emit dataUpdated();
}

void BTableModel::setColumns(const QVector<ColumnConfig> &columns)
{
    beginResetModel();
    m_columns = columns;
    endResetModel();
}

QString BTableModel::formatCellData(const QJsonObject &item, int column) const
{
    if (column < 0 || column >= m_columns.size())
        return QString();

    const QString &jsonKey = m_columns[column].jsonKey;
    if (item.contains(jsonKey)) {
        QJsonValue value = item[jsonKey];
        if (value.isString()) {
            return value.toString();
        } else if (value.isDouble()) {
            return QString::number(value.toDouble());
        } else if (value.isBool()) {
            return value.toBool() ? tr("Yes") : tr("No");
        }
    }

    return QString();
}
