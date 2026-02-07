#ifndef BEDITABLELISTDELEGATE_H
#define BEDITABLELISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>

/**
 * @file beditablelistdelegate.h
 * @brief Delegate for inline list item editing with optional buttons
 *
 * Provides inline editing for list items with:
 * - Delete button (visible by default)
 * - Browse button (hidden by default, enable with setShowBrowseButton)
 * - Validation visual feedback (red background for invalid)
 *
 * Visual design (default - browse hidden):
 * ┌─────────────────────────────────────────────────────┬───┐
 * │ /path/to/file                                       │ × │
 * └─────────────────────────────────────────────────────┴───┘
 *
 * With browse enabled:
 * ┌─────────────────────────────────────────────────┬───┬───┐
 * │ /path/to/file                                   │ … │ × │
 * └─────────────────────────────────────────────────┴───┴───┘
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */
class BEditableListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    /**
     * @brief Browse mode for file picker
     */
    enum BrowseMode {
        BrowseNone,       ///< No browse button
        BrowseFile,       ///< Browse for file
        BrowseDirectory   ///< Browse for directory
    };
    Q_ENUM(BrowseMode)

    /**
     * @brief Construct the delegate
     * @param parent Parent object
     */
    explicit BEditableListDelegate(QObject *parent = nullptr);

    // =========================================================================
    // Configuration
    // =========================================================================

    /**
     * @brief Show or hide the browse button
     * @param show true to show (default: false - hidden)
     */
    void setShowBrowseButton(bool show);

    /**
     * @brief Check if browse button is shown
     * @return true if shown
     */
    bool showBrowseButton() const;

    /**
     * @brief Set browse mode (file or directory)
     * @param mode Browse mode
     */
    void setBrowseMode(BrowseMode mode);

    /**
     * @brief Get browse mode
     * @return Current browse mode
     */
    BrowseMode browseMode() const;

    /**
     * @brief Show or hide the delete button
     * @param show true to show (default: true - visible)
     */
    void setShowDeleteButton(bool show);

    /**
     * @brief Check if delete button is shown
     * @return true if shown
     */
    bool showDeleteButton() const;

    /**
     * @brief Set placeholder text for empty items
     * @param text Placeholder text
     */
    void setPlaceholderText(const QString &text);

    /**
     * @brief Get placeholder text
     * @return Placeholder text
     */
    QString placeholderText() const;

    // =========================================================================
    // QStyledItemDelegate interface
    // =========================================================================

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

signals:
    /**
     * @brief Emitted when delete button is clicked
     * @param index Index of item to delete
     */
    void deleteRequested(const QModelIndex &index);

    /**
     * @brief Emitted when browse button is clicked
     * @param index Index of item to browse for
     */
    void browseRequested(const QModelIndex &index);

private:
    /**
     * @brief Calculate delete button rect within cell
     * @param option Style options
     * @return Button rectangle
     */
    QRect deleteButtonRect(const QStyleOptionViewItem &option) const;

    /**
     * @brief Calculate browse button rect within cell
     * @param option Style options
     * @return Button rectangle
     */
    QRect browseButtonRect(const QStyleOptionViewItem &option) const;

    /**
     * @brief Calculate text area rect (excluding buttons)
     * @param option Style options
     * @return Text rectangle
     */
    QRect textRect(const QStyleOptionViewItem &option) const;

    /**
     * @brief Get button width
     * @return Width of action buttons
     */
    int buttonWidth() const;

    bool m_showBrowseButton = false;  ///< Browse button hidden by default
    bool m_showDeleteButton = true;   ///< Delete button visible by default
    BrowseMode m_browseMode = BrowseFile;
    QString m_placeholderText;

    static constexpr int BUTTON_SIZE = 20;  ///< Size of action buttons
    static constexpr int BUTTON_MARGIN = 2; ///< Margin around buttons
};

#endif // BEDITABLELISTDELEGATE_H
