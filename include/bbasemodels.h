#ifndef BBASEMODELS_H
#define BBASEMODELS_H

#include <QAbstractListModel>
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

// ============================================================================
// BListModel - Base class for list models with JSON support
// ============================================================================

/**
 * @brief Base class for list models that work with JSON data
 * @version 1.0
 * @since 2026-01-29
 *
 * Provides common functionality for list models:
 * - Loading data from JSON arrays
 * - Automatic data clearing
 * - Item access by index
 * - Data update notifications
 */
class BListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BListModel(QObject *parent = nullptr);
    virtual ~BListModel() = default;

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Sets the data from a JSON array
     * @param jsonArray Array containing item objects
     */
    virtual void setData(const QJsonArray &jsonArray);

    /**
     * @brief Sets the data from a JSON document
     * @param jsonDoc Document containing result structure
     * @param arrayKey Key in result object (e.g., "filesets", "storages")
     */
    virtual void setData(const QJsonDocument &jsonDoc, const QString &arrayKey);

    /**
     * @brief Returns the item at the given row
     * @param row Row index
     * @return QJsonObject containing item data
     */
    QJsonObject itemAt(int row) const;

    /**
     * @brief Returns all items as JSON array
     * @return QJsonArray containing all items
     */
    QJsonArray allItems() const { return m_items; }

    /**
     * @brief Clears all data from the model
     */
    virtual void clear();

    /**
     * @brief Returns whether the model is empty
     * @return True if no items
     */
    bool isEmpty() const { return m_items.isEmpty(); }

signals:
    /**
     * @brief Emitted when data has been updated
     */
    void dataUpdated();

protected:
    /**
     * @brief Returns the display text for an item (override in subclasses)
     * @param item The item object
     * @return Display text
     */
    virtual QString getDisplayText(const QJsonObject &item) const;

    QJsonArray m_items;  ///< Array of item objects
};


// ============================================================================
// BTableModel - Base class for table models with JSON support
// ============================================================================

/**
 * @brief Base class for table models that work with JSON data
 * @version 1.0
 * @since 2026-01-29
 *
 * Provides common functionality for table models:
 * - Loading data from JSON arrays
 * - Column configuration
 * - Item access by row
 * - Data update notifications
 */
class BTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    /**
     * @brief Column configuration structure
     */
    struct ColumnConfig {
        QString jsonKey;      ///< JSON key for this column
        QString headerText;   ///< Header display text
        int alignment;        ///< Qt::Alignment flags

        ColumnConfig(const QString &key = QString(),
                    const QString &header = QString(),
                    int align = Qt::AlignLeft | Qt::AlignVCenter)
            : jsonKey(key), headerText(header), alignment(align) {}
    };

    explicit BTableModel(QObject *parent = nullptr);
    virtual ~BTableModel() = default;

    // QAbstractTableModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /**
     * @brief Sets the data from a JSON array
     * @param jsonArray Array containing item objects
     */
    virtual void setData(const QJsonArray &jsonArray);

    /**
     * @brief Sets the data from a JSON document
     * @param jsonDoc Document containing result structure
     * @param arrayKey Key in result object (e.g., "filesets", "storages")
     */
    virtual void setData(const QJsonDocument &jsonDoc, const QString &arrayKey);

    /**
     * @brief Returns the item at the given row
     * @param row Row index
     * @return QJsonObject containing item data
     */
    QJsonObject itemAt(int row) const;

    /**
     * @brief Returns all items as JSON array
     * @return QJsonArray containing all items
     */
    QJsonArray allItems() const { return m_items; }

    /**
     * @brief Clears all data from the model
     */
    virtual void clear();

    /**
     * @brief Returns whether the model is empty
     * @return True if no items
     */
    bool isEmpty() const { return m_items.isEmpty(); }

    /**
     * @brief Sets the column configuration
     * @param columns Vector of column configurations
     */
    void setColumns(const QVector<ColumnConfig> &columns);

signals:
    /**
     * @brief Emitted when data has been updated
     */
    void dataUpdated();

protected:
    /**
     * @brief Formats cell data for display (override in subclasses)
     * @param item The item object
     * @param column Column index
     * @return Formatted display text
     */
    virtual QString formatCellData(const QJsonObject &item, int column) const;

    QJsonArray m_items;                     ///< Array of item objects
    QVector<ColumnConfig> m_columns;        ///< Column configurations
};


// ============================================================================
// BTreeModel - Base class for tree models with JSON support
// ============================================================================

/**
 * @brief Base class for tree models that work with JSON data
 * @version 1.0
 * @since 2026-01-29
 *
 * Provides common functionality for tree models:
 * - Loading hierarchical data from JSON
 * - Parent-child relationships
 * - Recursive tree building
 */
class BTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit BTreeModel(QObject *parent = nullptr);
    virtual ~BTreeModel();

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /**
     * @brief Sets the data from a JSON array
     * @param jsonArray Array containing hierarchical data
     */
    virtual void setData(const QJsonArray &jsonArray);

    /**
     * @brief Sets the data from a JSON document
     * @param jsonDoc Document containing result structure
     * @param arrayKey Key in result object
     */
    virtual void setData(const QJsonDocument &jsonDoc, const QString &arrayKey);

    /**
     * @brief Clears all data from the model
     */
    virtual void clear();

signals:
    /**
     * @brief Emitted when data has been updated
     */
    void dataUpdated();

protected:
    /**
     * @brief Tree node structure
     */
    struct TreeNode {
        QJsonObject data;                   ///< Node data
        TreeNode *parent;                   ///< Parent node
        QVector<TreeNode*> children;        ///< Child nodes

        TreeNode(const QJsonObject &obj = QJsonObject(), TreeNode *p = nullptr)
            : data(obj), parent(p) {}

        ~TreeNode() {
            qDeleteAll(children);
        }
    };

    /**
     * @brief Returns the display text for a node (override in subclasses)
     * @param node The tree node
     * @return Display text
     */
    virtual QString getNodeText(const TreeNode *node) const;

    /**
     * @brief Builds the tree structure from JSON (override in subclasses)
     * @param jsonArray Source data
     */
    virtual void buildTree(const QJsonArray &jsonArray);

    TreeNode *m_rootNode;  ///< Root node of the tree
};

#endif // BBASEMODELS_H
