#include "bbasemodels.h"
#include <QJsonParseError>
#include <QDebug>

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
        qWarning() << "BListModel: JSON document is not an object";
        clear();
        return;
    }

    QJsonObject root = jsonDoc.object();
    QJsonObject result = root.value("result").toObject();
    QJsonArray dataArray = result.value(arrayKey).toArray();

    if (dataArray.isEmpty()) {
        qWarning() << "BListModel: No data found for key" << arrayKey;
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
        qWarning() << "BTableModel: JSON document is not an object";
        clear();
        return;
    }

    QJsonObject root = jsonDoc.object();
    QJsonObject result = root.value("result").toObject();
    QJsonArray dataArray = result.value(arrayKey).toArray();

    if (dataArray.isEmpty()) {
        qWarning() << "BTableModel: No data found for key" << arrayKey;
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


// ============================================================================
// BTreeModel Implementation
// ============================================================================

BTreeModel::BTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_rootNode(new TreeNode())
{
}

BTreeModel::~BTreeModel()
{
    delete m_rootNode;
}

QModelIndex BTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode *parentNode;

    if (!parent.isValid())
        parentNode = m_rootNode;
    else
        parentNode = static_cast<TreeNode*>(parent.internalPointer());

    if (row >= 0 && row < parentNode->children.size()) {
        TreeNode *childNode = parentNode->children[row];
        return createIndex(row, column, childNode);
    }

    return QModelIndex();
}

QModelIndex BTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    TreeNode *childNode = static_cast<TreeNode*>(child.internalPointer());
    TreeNode *parentNode = childNode->parent;

    if (parentNode == m_rootNode || !parentNode)
        return QModelIndex();

    // Find row of parent in grandparent
    TreeNode *grandparentNode = parentNode->parent;
    if (!grandparentNode)
        return QModelIndex();

    int row = grandparentNode->children.indexOf(parentNode);
    return createIndex(row, 0, parentNode);
}

int BTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeNode *parentNode;

    if (!parent.isValid())
        parentNode = m_rootNode;
    else
        parentNode = static_cast<TreeNode*>(parent.internalPointer());

    return parentNode->children.size();
}

int BTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;  // Default: single column tree
}

QVariant BTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        TreeNode *node = static_cast<TreeNode*>(index.internalPointer());
        return getNodeText(node);
    }

    return QVariant();
}

QVariant BTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return tr("Name");
    }

    return QVariant();
}

void BTreeModel::setData(const QJsonArray &jsonArray)
{
    beginResetModel();

    // Clear existing tree
    qDeleteAll(m_rootNode->children);
    m_rootNode->children.clear();

    // Build new tree
    buildTree(jsonArray);

    endResetModel();
    emit dataUpdated();
}

void BTreeModel::setData(const QJsonDocument &jsonDoc, const QString &arrayKey)
{
    if (!jsonDoc.isObject()) {
        qWarning() << "BTreeModel: JSON document is not an object";
        clear();
        return;
    }

    QJsonObject root = jsonDoc.object();
    QJsonObject result = root.value("result").toObject();
    QJsonArray dataArray = result.value(arrayKey).toArray();

    if (dataArray.isEmpty()) {
        qWarning() << "BTreeModel: No data found for key" << arrayKey;
        clear();
        return;
    }

    setData(dataArray);
}

void BTreeModel::clear()
{
    beginResetModel();
    qDeleteAll(m_rootNode->children);
    m_rootNode->children.clear();
    endResetModel();
    emit dataUpdated();
}

QString BTreeModel::getNodeText(const TreeNode *node) const
{
    if (!node)
        return QString();

    // Default: return "name" field if it exists
    if (node->data.contains("name")) {
        return node->data["name"].toString();
    }

    // Fallback: return first string value
    for (const QString &key : node->data.keys()) {
        if (node->data[key].isString()) {
            return node->data[key].toString();
        }
    }

    return QString();
}

void BTreeModel::buildTree(const QJsonArray &jsonArray)
{
    // Default implementation: flat list (override in subclasses for hierarchy)
    for (const QJsonValue &value : jsonArray) {
        if (value.isObject()) {
            TreeNode *node = new TreeNode(value.toObject(), m_rootNode);
            m_rootNode->children.append(node);
        }
    }
}
