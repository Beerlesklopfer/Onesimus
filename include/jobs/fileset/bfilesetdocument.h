// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BFILESETDOCUMENT_H
#define BFILESETDOCUMENT_H

#include <QObject>
#include <QUndoStack>
#include <QJsonObject>
#include <QList>
#include "config/beditablelistmodel.h"

/**
 * @file bfilesetdocument.h
 * @brief Data model of the FileSet wizard, with undo/redo for the lists
 *
 * Coordinates BEditableListModel instances for the nested structure of a
 * FileSet: Include blocks (files, the options of the files, the patterns of
 * an exclusion Options block), the global Exclude block, and everything the
 * wizard does not edit, kept verbatim so that loading and writing a FileSet
 * loses no directive (backlog P1-23).
 *
 * What it writes follows the grammar of Bareos 25 (dird/inc_conf.cc,
 * dird/dird_conf.cc fs_items): resource directives Name, Description,
 * Ignore FileSet Changes, Enable VSS; an Include block with Options blocks,
 * File, Plugin and Exclude Dir Containing; patterns to exclude in an
 * Options block with "Exclude = yes" (there is no Exclude block inside
 * Options); a global Exclude block with File.
 *
 * This is a pure data model - no Director reference.
 */
class BFileSetDocument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(bool modified READ isModified NOTIFY modifiedChanged)

public:
    /// A directive as it stands in the text: key as written, value without
    /// quotes and escapes, and whether it was quoted
    struct Directive {
        QString key;
        QString value;
        bool quoted = false;
    };
    using DirectiveList = QList<Directive>;

    /**
     * @brief One Include block
     *
     * The first Options block without "Exclude = yes" is @c options (keys are
     * normalised Bareos keywords, see optionKey()); the first one with it
     * holds the three exclude lists and @c excludeOptionsExtra. Further Options
     * blocks, other Include directives and unknown nested blocks are kept
     * verbatim.
     */
    struct IncludeBlock {
        int id = 0;                                    ///< Unique ID for this block
        BEditableListModel *pathsModel = nullptr;      ///< File = entries
        QMap<QString, QVariant> options;               ///< The options of the files
        BEditableListModel *excludeFilesModel = nullptr;     ///< Wild = in the exclusion Options block
        BEditableListModel *excludePatternsModel = nullptr;  ///< WildDir = in the exclusion Options block
        BEditableListModel *excludeWildFileModel = nullptr;  ///< WildFile = in the exclusion Options block
        DirectiveList excludeOptionsExtra;             ///< Regex... and other directives of that block
        QList<DirectiveList> extraOptionsBlocks;       ///< Further Options blocks, verbatim
        QStringList plugins;                           ///< Plugin = entries
        QStringList excludeDirContaining;              ///< Exclude Dir Containing = entries
        DirectiveList extraDirectives;                 ///< Other Include directives, verbatim
        QStringList extraBlocks;                       ///< Unknown nested blocks, as text
    };

    explicit BFileSetDocument(QObject *parent = nullptr);
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

    /// Resource directives the wizard does not edit, verbatim
    DirectiveList extraDirectives() const { return m_extraDirectives; }

    // =========================================================================
    // Include Block Management
    // =========================================================================

    int includeBlockCount() const;
    IncludeBlock *includeBlock(int index);
    const IncludeBlock *includeBlock(int index) const;

    /**
     * @brief Add a new Include block with the default options
     *        (Signature, Compression, One FS, ACL and XAttr support)
     */
    IncludeBlock *addIncludeBlock();
    void removeIncludeBlock(int index);

    // =========================================================================
    // Global Exclude
    // =========================================================================

    BEditableListModel *excludePathModel() const;

    // =========================================================================
    // Options of the files, and the options form
    // =========================================================================

    /**
     * @brief The normalised key of an Options directive: lower case, no
     *        blanks or underscores, aliases resolved ("ACL Support", "acl"
     *        -> "aclsupport", "strip_path" -> "strippath")
     */
    static QString optionKey(const QString &key);

    /// The directive name written for a normalised key ("aclsupport" -> "ACL Support")
    static QString optionDirective(const QString &key);

    /// true for the keywords Bareos 25 reads inside an Options block
    static bool isOptionKeyword(const QString &key);

    /**
     * @brief Apply one value of the options form to @p block
     *
     * Only this key changes. An empty text or list, compression "None" and
     * Strip Path 0 remove the directive; "exclude_dir_containing" goes to the
     * Include block (it is not an Options directive); "vss" is the FileSet's
     * Enable VSS and is not written into Options.
     */
    void setFormOption(IncludeBlock *block, const QString &formKey, const QVariant &value);

    /// The options of @p block under the keys of the options form
    static QMap<QString, QVariant> formOptions(const IncludeBlock *block);

    // =========================================================================
    // Undo/Redo
    // =========================================================================

    QUndoStack *undoStack() const;
    bool isModified() const;
    void markClean();

    // =========================================================================
    // Serialization (pure data - no Director calls)
    // =========================================================================

    /**
     * @brief Load from the text of a FileSet resource (ReadResource)
     *
     * Every directive is kept: what the wizard edits goes to its fields, the
     * rest verbatim. @return false with @p error set if the text is not one
     * FileSet resource the reader understands (@include, unbalanced braces);
     * the document is then unchanged.
     */
    bool loadFromConfigText(const QString &text, QString *error = nullptr);

    /**
     * @brief Load from a wizard template ("fileset" object of a template)
     *
     * Reads "File" (and the older "Files") wherever the templates use it.
     */
    void loadFromTemplate(const QJsonObject &fileset);

    /**
     * @brief Load from Bareos Director JSON response (show filesets format)
     *
     * "show filesets" does not report Enable VSS and Ignore FileSet Changes
     * (dird_conf.cc FilesetResource::PrintConfig); they keep their defaults.
     */
    void loadFromBareosJson(const QJsonObject &fsObj);

    /**
     * @brief Generate Bareos config text
     * @return FileSet configuration string, or an empty string with @p error
     *         set if a value contains a control character
     */
    QString toConfigText(QString *error = nullptr) const;

    // =========================================================================
    // Validation
    // =========================================================================

    struct ValidationResult {
        bool valid;
        QStringList errors;
        QStringList warnings;
    };

    ValidationResult validate() const;

    /// Clear all data and reset to empty state
    void clear();

signals:
    void nameChanged(const QString &name);
    void descriptionChanged(const QString &desc);
    void modifiedChanged(bool modified);
    void includeBlockAdded(int index);
    void includeBlockRemoved(int index);
    void documentChanged();

private:
    IncludeBlock *newIncludeBlock(bool withDefaults);
    void connectBlockSignals(IncludeBlock *block);
    void onAnyModelChanged();
    void initDefaultOptions(IncludeBlock *block);
    void deleteBlock(IncludeBlock *block);

    QString m_name;
    QString m_description;
    bool m_enableVss = true;
    bool m_ignoreFileSetChanges = false;
    DirectiveList m_extraDirectives;           ///< Unknown resource directives
    DirectiveList m_excludeExtra;              ///< Global Exclude: other directives
    QStringList m_extraBlocks;                 ///< Unknown nested resource blocks, as text

    QList<IncludeBlock*> m_includeBlocks;
    QUndoStack *m_undoStack = nullptr;
    BEditableListModel *m_excludePathModel = nullptr;

    int m_nextBlockId = 1;
};

#endif // BFILESETDOCUMENT_H
