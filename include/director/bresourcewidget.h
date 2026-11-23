// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BRESOURCEWIDGET_H
#define BRESOURCEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHeaderView>
#include "config/bconfigparser.h"
#include "director/bdirector.h"

class BResourcePersistence;
class BResourceSaveDialogs;
struct BResourceReport;

/**
 * @file bresourcewidget.h
 * @brief Base widget for displaying parsed Bareos/Bacula resources
 *
 * This widget provides a common interface for displaying resources
 * parsed from configuration files. Each resource type can subclass
 * this to provide type-specific display and validation.
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2025
 */
class BResourceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BResourceWidget(const QString &resourceType, BDirector *director = nullptr, QWidget *parent = nullptr);
    virtual ~BResourceWidget();

    /**
     * @brief Set the resources to display
     */
    virtual void setResources(const QList<BConfigResource> &resources);

    /**
     * @brief Set the director connection (optional, enables director-dependent features)
     */
    void setDirector(BDirector *director) { m_director = director; }

    /**
     * @brief Get the director connection
     */
    BDirector *director() const { return m_director; }

    /**
     * @brief Get the resource type this widget handles
     */
    QString resourceType() const { return m_resourceType; }

    /**
     * @brief Set a custom base name for export filenames (e.g. "JOERG-fd")
     */
    void setExportName(const QString &name) { m_exportName = name; }

    /**
     * @brief Set additional resources included only in exports (conf/ZIP)
     *
     * When set, exports use these resources instead of the displayed ones.
     * The displayed resources (m_resources) are unaffected.
     */
    void setExportResources(const QList<BConfigResource> &resources) { m_exportResources = resources; }

    /**
     * @brief Get the currently selected resource
     */
    BConfigResource selectedResource() const;

    /**
     * @brief Get number of resources
     */
    int resourceCount() const { return m_resources.size(); }

    /**
     * @brief Set reference data for populating combo boxes in edit dialogs
     * @param referenceData Map of reference type (e.g., "Client", "Pool") to list of names
     */
    void setReferenceData(const QMap<QString, QStringList> &referenceData) { m_referenceData = referenceData; }

    /**
     * @brief Select and scroll to the resource with the given name
     */
    void selectResource(const QString &name);

    /**
     * @brief Export all resources to a .conf file
     */
    void exportToConf();

    /**
     * @brief Export all resources to a ZIP archive (Bareos directory layout)
     */
    void exportToZip();

signals:
    /**
     * @brief Emitted when a resource is selected
     */
    void resourceSelected(const BConfigResource &resource);

    /**
     * @brief Emitted when selection is cleared
     */
    void selectionCleared();

    /**
     * @brief Emitted when a resource is modified via the edit dialog
     */
    void resourceModified(const BConfigResource &resource);

protected slots:
    virtual void onItemSelectionChanged();
    virtual void onEditResource();

protected:
    /**
     * @brief Populate the tree with resources - override for custom columns
     */
    virtual void populateTree();

    /**
     * @brief Update the details view for the selected resource
     */
    virtual void updateDetails(const BConfigResource &resource);

    /**
     * @brief Format a resource value for display
     */
    QString formatValue(const BConfigValue &value, int indent = 0) const;

    /**
     * @brief Get icon for this resource type
     */
    virtual QIcon resourceIcon() const;

    /**
     * @brief Writes an edited resource to the Director (Job, JobDefs)
     *
     * Over gRPC to an Onesimus Director with the configuration methods: the
     * changed directives are written into the resource text as it stands in
     * its file, checked by the Director, shown for confirmation, written.
     * Over BNET, or where the session does not offer it, the capability
     * notice says so and nothing is written or changed in the list.
     * @return false if there is nothing to write to (no write path for
     *         the type, or no Director); nothing is changed then
     */
    bool writeEdited(const BConfigResource &before, const BConfigResource &after,
                     const QStringList &formDirectives);
    void onWriteFinished(const BResourceReport &report);
    /// true for the types whose changes are written (Job, JobDefs)
    bool hasWritePath() const;

    /**
     * @brief Update the count label
     */
    void updateCountLabel();

    /**
     * @brief Serialize a resource to Bareos config syntax
     */
    QString resourceToConf(const BConfigResource &resource) const;

    /**
     * @brief Show raw config file syntax in the details pane
     */
    void updateConfigPreview(const BConfigResource &resource);

    // UI components
    QSplitter *m_splitter;
    QTreeWidget *m_treeWidget;
    QTextEdit *m_detailsEdit;
    QLabel *m_countLabel;
    QPushButton *m_previewToggle;
    QPushButton *m_editButton;
    QPushButton *m_exportButton;
    QPushButton *m_exportZipButton;

    // Data
    BDirector *m_director = nullptr;
    QString m_resourceType;
    QString m_exportName;
    QList<BConfigResource> m_resources;
    QList<BConfigResource> m_exportResources;  ///< If set, used by export instead of m_resources
    QMap<QString, QStringList> m_referenceData; ///< Reference data for edit dialog combos

    // Write path (writeEdited)
    BResourcePersistence *m_persistence = nullptr;
    BResourceSaveDialogs *m_saveDialogs = nullptr;
    BConfigResource m_pendingEdit;              ///< What the list shows once the write went through
};

#endif // BRESOURCEWIDGET_H
