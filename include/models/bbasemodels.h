#ifndef BBASEMODELS_H
#define BBASEMODELS_H

#include <QAbstractListModel>
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include "blogging.h"

// Debug logging prefixes for Models
#define MODEL_DEBUG BLOG_DEBUG()
#define MODEL_WARNING BLOG_WARNING()
#define MODEL_CRITICAL BLOG_ERROR()

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

#endif // BBASEMODELS_H
