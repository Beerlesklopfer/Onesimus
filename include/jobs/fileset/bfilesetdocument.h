#ifndef BFILESETDOCUMENT_H
#define BFILESETDOCUMENT_H

#include <QObject>
#include <QUndoStack>
#include <QJsonObject>
#include <QList>
#include "config/beditablelistmodel.h"

class BConfigResource;

/**
 * @file bfilesetdocument.h
 * @brief Pure data model for FileSet editing with undo/redo support
 *
 * Coordinates multiple BEditableListModel instances for FileSet's nested structure:
 * - Basic properties (Name, Description, Enable VSS, etc.)
 * - Include blocks (each with paths, options, and nested exclude)
 * - Global Exclude block
 *
 * This is a PURE DATA MODEL - no Director reference. All Director communication
 * (validate, import, export) happens in the wizard/dialog layer.
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */
class BFileSetDocument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(bool modified READ isModified NOTIFY modifiedChanged)

public:
    /**
     * @brief Represents a single Include block with paths and options
     */
    struct IncludeBlock {
        int id;                                    ///< Unique ID for this block
        BEditableListModel *pathsModel;            ///< File paths in this Include
        QMap<QString, QVariant> options;           ///< Include Options (signature, compression, etc.)
        BEditableListModel *excludeFilesModel;     ///< Exclude files within Options
        BEditableListModel *excludePatternsModel;  ///< WildDir patterns in Options
        BEditableListModel *excludeWildFileModel;  ///< WildFile patterns in Options

        IncludeBlock() : id(0), pathsModel(nullptr),
                         excludeFilesModel(nullptr), excludePatternsModel(nullptr),
                         excludeWildFileModel(nullptr) {}
    };

    /**
     * @brief Construct FileSet document
     * @param parent Parent object
     */
    explicit BFileSetDocument(QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~BFileSetDocument() override;

    // =========================================================================
    // Basic Properties
    // =========================================================================

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &desc);

    bool enableVss() const;
    void setEnableVss(bool enable);

    bool ignoreFileSetChanges() const;
    void setIgnoreFileSetChanges(bool ignore);

    bool enableSnapshot() const;
    void setEnableSnapshot(bool enable);

    // =========================================================================
    // Include Block Management
    // =========================================================================

    /**
     * @brief Get number of Include blocks
     * @return Number of Include blocks
     */
    int includeBlockCount() const;

    /**
     * @brief Get Include block at index
     * @param index Block index
     * @return Pointer to Include block, or nullptr if invalid
     */
    IncludeBlock *includeBlock(int index);
    const IncludeBlock *includeBlock(int index) const;

    /**
     * @brief Add a new Include block
     * @return Pointer to the new block
     */
    IncludeBlock *addIncludeBlock();

    /**
     * @brief Remove an Include block
     * @param index Block index to remove
     */
    void removeIncludeBlock(int index);

    // =========================================================================
    // Global Exclude (separate from Include Options)
    // =========================================================================

    /**
     * @brief Get global Exclude paths model
     * @return Pointer to exclude paths model
     */
    BEditableListModel *excludePathModel() const;

    // =========================================================================
    // Undo/Redo
    // =========================================================================

    /**
     * @brief Get the undo stack
     * @return Pointer to undo stack
     */
    QUndoStack *undoStack() const;

    /**
     * @brief Check if document has unsaved changes
     * @return true if modified
     */
    bool isModified() const;

    /**
     * @brief Mark document as clean (no unsaved changes)
     */
    void markClean();

    // =========================================================================
    // Serialization (pure data - no Director calls)
    // =========================================================================

    /**
     * @brief Load from parsed BConfigResource
     * @param resource Parsed FileSet resource
     */
    void loadFromResource(const BConfigResource &resource);

    /**
     * @brief Load from JSON preset/template
     * @param preset JSON object with FileSet template data
     */
    void loadFromJson(const QJsonObject &preset);

    /**
     * @brief Load from Bareos Director JSON response (show filesets format)
     *
     * Handles the structured JSON returned by "show filesets" in .api 2 mode:
     * { "name": "...", "include": [{ "options": [...], "file": [...] }], "exclude": [...] }
     *
     * @param fsObj JSON object for a single fileset from the Director response
     */
    void loadFromBareosJson(const QJsonObject &fsObj);

    /**
     * @brief Convert to BConfigResource
     * @return FileSet as BConfigResource
     */
    // BConfigResource toResource() const;

    /**
     * @brief Generate Bareos config text
     * @return FileSet configuration string
     */
    QString toConfigText() const;

    /**
     * @brief Generate Bareos configure add command
     * @return Single-line command for bconsole
     */
    QString toConfigureCommand() const;

    /**
     * @brief Convert to JSON (for template export)
     * @return JSON object with FileSet data
     */
    QJsonObject toJson() const;

    // =========================================================================
    // Validation
    // =========================================================================

    /**
     * @brief Validation result structure
     */
    struct ValidationResult {
        bool valid;
        QStringList errors;
        QStringList warnings;
    };

    /**
     * @brief Validate the FileSet configuration
     * @return Validation result with errors and warnings
     */
    ValidationResult validate() const;

    /**
     * @brief Clear all data and reset to empty state
     */
    void clear();

signals:
    void nameChanged(const QString &name);
    void descriptionChanged(const QString &desc);
    void modifiedChanged(bool modified);
    void includeBlockAdded(int index);
    void includeBlockRemoved(int index);
    void documentChanged();

private:
    void connectBlockSignals(IncludeBlock *block);
    void onAnyModelChanged();
    QString formatOption(const QString &key, const QVariant &value,
                         const QString &indent = QString()) const;
    void initDefaultOptions(IncludeBlock *block);

    QString m_name;
    QString m_description;
    bool m_enableVss = true;
    bool m_ignoreFileSetChanges = false;
    bool m_enableSnapshot = false;

    QList<IncludeBlock*> m_includeBlocks;
    BEditableListModel *m_excludePathModel = nullptr;
    QUndoStack *m_undoStack = nullptr;

    int m_nextBlockId = 1;
};

#endif // BFILESETDOCUMENT_H
