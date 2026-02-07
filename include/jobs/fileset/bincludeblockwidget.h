#ifndef BINCLUDEBLOCKWIDGET_H
#define BINCLUDEBLOCKWIDGET_H

#include <QWidget>
#include <QUndoStack>

class BEditableListWidget;
class BIncludeOptionsForm;
class BFileSetDocument;
class QGroupBox;
class QTabWidget;

/**
 * @file bincludeblockwidget.h
 * @brief Widget for editing a single FileSet Include block
 *
 * Provides UI for:
 * - File paths list (using BEditableListWidget)
 * - Include Options (signature, compression, etc.)
 * - Nested Exclude within Options (files and patterns)
 *
 * Visual layout:
 * ┌─────────────────────────────────────────────────────────┐
 * │ Include Block #1                                    [-] │
 * ├─────────────────────────────────────────────────────────┤
 * │ [Files] [Options] [Exclude]                             │
 * │ ┌─────────────────────────────────────────────────────┐ │
 * │ │ /home/user                                      [×] │ │
 * │ │ /var/data                                       [×] │ │
 * │ │                                                     │ │
 * │ │                           [+ Add Path]              │ │
 * │ └─────────────────────────────────────────────────────┘ │
 * └─────────────────────────────────────────────────────────┘
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */
class BIncludeBlockWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct Include block widget
     * @param document Parent FileSet document
     * @param blockIndex Index of this block in the document
     * @param parent Parent widget
     */
    explicit BIncludeBlockWidget(BFileSetDocument *document, int blockIndex,
                                 QWidget *parent = nullptr);

    /**
     * @brief Get block index
     * @return Index in parent document
     */
    int blockIndex() const { return m_blockIndex; }

    /**
     * @brief Set block index (when blocks are reordered)
     * @param index New index
     */
    void setBlockIndex(int index);

    /**
     * @brief Refresh widget from document data
     */
    void refresh();

signals:
    /**
     * @brief Emitted when user requests to remove this block
     * @param blockIndex Index of block to remove
     */
    void removeRequested(int blockIndex);

private slots:
    void onRemoveClicked();
    void onOptionChanged();

private:
    void setupUi();
    void createFilesTab();
    void createOptionsTab();
    void createExcludeTab();
    void connectSignals();
    void loadOptionsFromBlock();
    void saveOptionsToBlock();

    BFileSetDocument *m_document;
    int m_blockIndex;

    QTabWidget *m_tabWidget;

    // Files tab
    BEditableListWidget *m_pathsWidget = nullptr;

    // Options tab - schema-driven form
    BIncludeOptionsForm *m_optionsForm = nullptr;

    // Exclude tab (within Options)
    QWidget *m_excludeWidget = nullptr;
    BEditableListWidget *m_excludeFilesWidget = nullptr;
    BEditableListWidget *m_excludePatternsWidget = nullptr;   // WildDir patterns
    BEditableListWidget *m_excludeWildFileWidget = nullptr;   // WildFile patterns
};

#endif // BINCLUDEBLOCKWIDGET_H
