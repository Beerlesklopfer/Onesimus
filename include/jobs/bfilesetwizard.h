/**
 * @file bfilesetwizard.h
 * @brief Unified wizard for creating and editing FileSet resources
 *
 * Replaces the separate BNewFileSetWizard and BFileSetDialog with a single
 * unified wizard that supports both New and Edit modes.
 *
 * Uses BFileSetDocument as the data model and BIncludeBlockWidget for
 * editing Include blocks with full undo/redo support.
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */
#ifndef BFILESETWIZARD_H
#define BFILESETWIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QProgressBar>
#include <QTimer>
#include <QJsonObject>
#include <QStackedWidget>
#include <QScrollArea>
#include <QVBoxLayout>

#include "director/bdirector.h"
class BFileSetDocument;
class BFilesetModel;
class BIncludeBlockWidget;
class BResourceWidget;
class BEditableListWidget;

/**
 * @brief Unified wizard for creating and editing FileSet resources
 *
 * Supports two modes:
 * - NewMode: Template selection → Settings → Preview
 * - EditMode: Settings → Preview (skip template selection)
 *
 * Uses BFileSetDocument as the data model for full undo/redo support.
 *
 * @since 2.12
 */
class BFileSetWizard : public QWizard
{
    Q_OBJECT

public:
    /**
     * @brief Wizard operating mode
     */
    enum Mode {
        NewMode,    ///< Creating a new FileSet
        EditMode    ///< Editing an existing FileSet
    };
    Q_ENUM(Mode)

    /**
     * @brief Page identifiers
     */
    enum PageId {
        Page_Template,      ///< Template selection (NewMode only)
        Page_Settings,      ///< Basic FileSet settings (name, description, options)
        Page_Include,       ///< Include blocks editing
        Page_Exclude,       ///< Global exclude editing
        Page_Preview        ///< Preview and execute
    };

    /**
     * @brief Construct wizard for creating a new FileSet
     * @param director Director connection for validation and execution
     * @param filesetModel Already-populated fileset model (optional, for combo population)
     * @param parent Parent widget
     */
    explicit BFileSetWizard(BDirector *director, BFilesetModel *filesetModel = nullptr,
                            QWidget *parent = nullptr);

    /**
     * @brief Construct wizard for editing an existing FileSet
     * @param director Director connection
     * @param filesetName Name of the FileSet to edit
     * @param filesetModel Already-populated fileset model (optional, for combo population)
     * @param parent Parent widget
     */
    explicit BFileSetWizard(BDirector *director, const QString &filesetName,
                            BFilesetModel *filesetModel = nullptr,
                            QWidget *parent = nullptr);

    ~BFileSetWizard() override;

    /**
     * @brief Get the wizard mode
     */
    Mode mode() const { return m_mode; }

    /**
     * @brief Get the Director connection
     */
    BDirector *director() const { return m_director; }

    /**
     * @brief Get the fileset model (for combo population)
     */
    BFilesetModel *filesetModel() const { return m_filesetModel; }

    /**
     * @brief Get the FileSet document (data model)
     */
    BFileSetDocument *document() const { return m_document; }

    /**
     * @brief Get whether to execute configure command
     */
    bool executeOnFinish() const { return m_executeOnFinish; }

    /**
     * @brief Set whether to execute configure command
     */
    void setExecuteOnFinish(bool execute) { m_executeOnFinish = execute; }

    /**
     * @brief Load FileSet templates from resources
     * @return Map of template name to template object
     */
    static QJsonObject loadTemplates();

private:
    void init();

    Mode m_mode = NewMode;
    BDirector *m_director = nullptr;
    BFilesetModel *m_filesetModel = nullptr;
    BFileSetDocument *m_document = nullptr;
    QString m_originalName;  // For edit mode
    bool m_executeOnFinish = true;

    static QJsonObject s_templates;
};

// ============================================================================
// Page 1: Template Selection (NewMode only)
// ============================================================================

/**
 * @brief Template selection page for new FileSets
 */
class BFileSetTemplatePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BFileSetTemplatePage(QWidget *parent = nullptr);

    void initializePage() override;
    bool isComplete() const override;

private slots:
    void onTemplateChanged(int index);
    void onMixTemplates();

private:
    void updatePreview();
    void loadTemplateIntoDocument(const QString &templateName);

    QComboBox *m_templateCombo = nullptr;
    QPushButton *m_mixButton = nullptr;
    QLabel *m_descriptionLabel = nullptr;
    QLabel *m_platformLabel = nullptr;
    QLabel *m_includePreview = nullptr;
    QLabel *m_excludePreview = nullptr;
    QLabel *m_mixedLabel = nullptr;
    QStringList m_mixedTemplates;
};

// ============================================================================
// Page 2: Basic FileSet Settings
// ============================================================================

/**
 * @brief Basic settings page (name, description, options)
 */
class BFileSetSettingsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BFileSetSettingsPage(QWidget *parent = nullptr);

    void initializePage() override;
    void cleanupPage() override;
    bool validatePage() override;
    bool isComplete() const override;

private slots:
    void onFileSetSelected(int index);
    void onFileSetsLoaded(const QStringList &names);

private:
    void setupUi();
    void collectFormValues();
    void fetchFileSets();
    void requestShowFilesets();
    void loadFileSetFromModel(const QString &filesetName);

    // FileSet selection (Edit mode only)
    QWidget *m_filesetSelectWidget = nullptr;
    QComboBox *m_filesetCombo = nullptr;
    QLabel *m_loadingLabel = nullptr;

    // Basic settings
    QLineEdit *m_nameEdit = nullptr;
    QLineEdit *m_descriptionEdit = nullptr;

    // Global FileSet options
    QCheckBox *m_enableVssCheck = nullptr;
    QCheckBox *m_ignoreChangesCheck = nullptr;
    QCheckBox *m_enableSnapshotCheck = nullptr;

    bool m_initialized = false;
};

// ============================================================================
// Page 3: Include Blocks
// ============================================================================

/**
 * @brief Include blocks editing page with scrollable list
 */
class BFileSetIncludePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BFileSetIncludePage(QWidget *parent = nullptr);

    void initializePage() override;
    void cleanupPage() override;

private slots:
    void onAddIncludeBlock();
    void onRemoveIncludeBlock(int index);
    void rebuildIncludeBlockWidgets();

private:
    void setupUi();

    QScrollArea *m_scrollArea = nullptr;
    QWidget *m_container = nullptr;
    QVBoxLayout *m_blocksLayout = nullptr;
    QList<BIncludeBlockWidget*> m_includeWidgets;
    QPushButton *m_addButton = nullptr;

    bool m_initialized = false;
};

// ============================================================================
// Page 4: Global Exclude
// ============================================================================

/**
 * @brief Global exclude paths editing page
 */
class BFileSetExcludePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BFileSetExcludePage(QWidget *parent = nullptr);

    void initializePage() override;

private:
    void setupUi();

    BEditableListWidget *m_excludeWidget = nullptr;
    bool m_initialized = false;
};

// ============================================================================
// Page 5: Preview & Execute
// ============================================================================

/**
 * @brief Preview page with config generation and Director execution
 */
class BFileSetPreviewPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BFileSetPreviewPage(QWidget *parent = nullptr);

    void initializePage() override;
    bool validatePage() override;

private slots:
    void onCopyConfig();
    void onExportZip();
    void onJsonResponse(BDirector::Command cmd, const QString &jsonData);
    void onCommandResponse(BDirector::Command cmd, const QString &response);
    void onConfigureTimeout();

private:
    void generateConfig();
    void executeConfigureCommand();

    BResourceWidget *m_filesetWidget = nullptr;
    QTextEdit *m_commandEdit = nullptr;
    QLabel *m_validationLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    QProgressBar *m_progressBar = nullptr;
    QPushButton *m_copyButton = nullptr;
    QTimer *m_timeoutTimer = nullptr;
    bool m_executed = false;
};

#endif // BFILESETWIZARD_H
