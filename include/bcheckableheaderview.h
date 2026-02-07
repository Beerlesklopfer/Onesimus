#ifndef BCHECKABLEHEADERVIEW_H
#define BCHECKABLEHEADERVIEW_H

#include <QHeaderView>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionButton>
#include <QMenu>
#include <QSet>

/**
 * @brief Custom header view with clickable checkbox and column visibility
 * @version 2.0
 * @since 2026-01-26
 * 
 * Features:
 * - Tri-state checkbox (checked/unchecked/partial)
 * - Right-click context menu
 * - Show/hide columns
 */
class BCheckableHeaderView : public QHeaderView
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a checkable header view
     * @param orientation Header orientation
     * @param parent Parent widget
     * @since 1.0
     */
    explicit BCheckableHeaderView(Qt::Orientation orientation, QWidget *parent = nullptr);
    
    /**
     * @brief Sets whether the checkbox in column 0 is visible
     * @param visible True to show checkbox (default), false to hide it
     * @since 2.9
     */
    void setCheckboxVisible(bool visible) { m_checkboxVisible = visible; }

    /**
     * @brief Sets whether tri-state mode is enabled
     * @param enabled True to enable tri-state (partial selection)
     * @since 2.0
     */
    void setTriStateEnabled(bool enabled) { m_triStateEnabled = enabled; }
    
    /**
     * @brief Returns whether tri-state is enabled
     * @return True if tri-state mode is enabled
     * @since 2.0
     */
    bool isTriStateEnabled() const { return m_triStateEnabled; }
    
    /**
     * @brief Sets hidden columns
     * @param columns Set of column indices to hide
     * @since 2.0
     */
    void setHiddenColumns(const QSet<int> &columns);
    
    /**
     * @brief Gets hidden columns
     * @return Set of hidden column indices
     * @since 2.0
     */
    QSet<int> hiddenColumns() const { return m_hiddenColumns; }
    
    /**
     * @brief Toggles column visibility
     * @param column Column index
     * @since 2.0
     */
    void toggleColumnVisibility(int column);

signals:
    /**
     * @brief Emitted when the checkbox column header is clicked
     * @param column Column index (should be 0)
     * @since 1.0
     */
    void checkboxHeaderClicked(int column);
    
    /**
     * @brief Emitted when column visibility changes
     * @param column Column index
     * @param visible New visibility state
     * @since 2.0
     */
    void columnVisibilityChanged(int column, bool visible);

protected:
    /**
     * @brief Handles paint events for the header
     * @param painter Painter
     * @param rect Section rectangle
     * @param logicalIndex Logical section index
     * @since 1.0
     */
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
    
    /**
     * @brief Handles mouse press events
     * @param event Mouse event
     * @since 1.0
     */
    void mousePressEvent(QMouseEvent *event) override;
    
    /**
     * @brief Handles context menu events
     * @param event Context menu event
     * @since 2.0
     */
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    /**
     * @brief Gets the checkbox rect for a section
     * @param rect Section rectangle
     * @return Rectangle for checkbox
     * @since 1.0
     */
    QRect checkBoxRect(const QRect &rect) const;
    
    /**
     * @brief Creates and shows the context menu
     * @param pos Global position
     * @since 2.0
     */
    void showContextMenu(const QPoint &pos);
    
    /**
     * @brief Gets the tri-state checkbox state
     * @return Qt::CheckState (Checked/Unchecked/PartiallyChecked)
     * @since 2.0
     */
    Qt::CheckState getCheckState() const;

private:
    bool m_checkboxVisible;         ///< Whether checkbox is drawn in column 0
    bool m_triStateEnabled;         ///< Whether tri-state mode is enabled
    QSet<int> m_hiddenColumns;      ///< Set of hidden column indices
    QMenu *m_contextMenu;           ///< Context menu for column visibility
};

#endif // BCHECKABLEHEADERVIEW_H
