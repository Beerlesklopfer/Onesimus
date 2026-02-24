/**
 * @file bjobwizard.h
 * @brief Wizard for creating new Job or JobDefs resources
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#ifndef BJOBWIZARD_H
#define BJOBWIZARD_H

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
#include <QSpinBox>
#include <QTableWidget>
#include <QDialogButtonBox>

#include "director/bdirector.h"

class BResourceForm;

// ============================================================================
// RunScript data structures and editor widgets
// ============================================================================

/**
 * @brief Data for a single RunScript block
 */
struct RunScriptEntry {
    QString command;
    QString runsWhen = "Before";        ///< Before, After, Always, AfterVSS
    bool runsOnClient = true;
    bool runsOnFailure = false;
    bool abortJobOnError = true;
    bool failJobOnError = true;
};

/**
 * @brief Dialog for editing a single RunScript entry
 */
class BRunScriptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BRunScriptDialog(QWidget *parent = nullptr);
    explicit BRunScriptDialog(const RunScriptEntry &entry, QWidget *parent = nullptr);

    RunScriptEntry entry() const;

private:
    void setupUi();
    void populateFromEntry(const RunScriptEntry &entry);

    QLineEdit *m_commandEdit = nullptr;
    QComboBox *m_runsWhenCombo = nullptr;
    QCheckBox *m_runsOnClientCheck = nullptr;
    QCheckBox *m_runsOnFailureCheck = nullptr;
    QCheckBox *m_abortOnErrorCheck = nullptr;
    QCheckBox *m_failOnErrorCheck = nullptr;
};

/**
 * @brief Reusable widget for editing multiple RunScript blocks
 */
class BRunScriptEditor : public QWidget
{
    Q_OBJECT

public:
    explicit BRunScriptEditor(QWidget *parent = nullptr);

    QList<RunScriptEntry> entries() const { return m_entries; }
    void setEntries(const QList<RunScriptEntry> &entries);
    bool hasEntries() const { return !m_entries.isEmpty(); }

signals:
    void entriesChanged();

private slots:
    void onAddScript();
    void onEditScript();
    void onRemoveScript();
    void onDoubleClicked(int row, int column);

private:
    void refreshTable();

    QTableWidget *m_table = nullptr;
    QPushButton *m_addButton = nullptr;
    QPushButton *m_editButton = nullptr;
    QPushButton *m_removeButton = nullptr;
    QList<RunScriptEntry> m_entries;
};

// ============================================================================
// Wizard page classes
// ============================================================================

class BJobWizard;

/**
 * @brief Page 1: Job name, type, JobDefs inheritance, enabled
 */
class BJobBasicsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BJobBasicsPage(QWidget *parent = nullptr);

    void initializePage() override;
    bool isComplete() const override;

    QString jobName() const;
    QString jobType() const;
    QString jobDefs() const;
    bool enabled() const;

private:
    QLineEdit *m_nameEdit = nullptr;
    QComboBox *m_typeCombo = nullptr;
    QComboBox *m_jobDefsCombo = nullptr;
    QCheckBox *m_enabledCheck = nullptr;
    QLabel *m_jobDefsHint = nullptr;
};

/**
 * @brief Page 2: Resource references + Level, Priority, Scripts
 */
class BJobResourcesPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BJobResourcesPage(QWidget *parent = nullptr);

    void initializePage() override;
    bool isComplete() const override;

    QMap<QString, QString> collectValues() const;

private:
    void setupUi();
    void populateCombos();

    // Required resources
    QComboBox *m_clientCombo = nullptr;
    QComboBox *m_filesetCombo = nullptr;
    QComboBox *m_storageCombo = nullptr;
    QComboBox *m_poolCombo = nullptr;
    QComboBox *m_messagesCombo = nullptr;

    // Job settings
    QComboBox *m_levelCombo = nullptr;
    QComboBox *m_scheduleCombo = nullptr;
    QComboBox *m_catalogCombo = nullptr;
    QSpinBox *m_prioritySpin = nullptr;
    QSpinBox *m_maxConcurrentSpin = nullptr;

    // Restore settings
    QLineEdit *m_whereEdit = nullptr;
    QComboBox *m_replaceCombo = nullptr;

    // Pool overrides
    QGroupBox *m_poolOverridesGroup = nullptr;
    QComboBox *m_fullPoolCombo = nullptr;
    QComboBox *m_diffPoolCombo = nullptr;
    QComboBox *m_incPoolCombo = nullptr;

    // Simple scripts
    QLineEdit *m_runBeforeEdit = nullptr;
    QLineEdit *m_runAfterEdit = nullptr;
    QLineEdit *m_runAfterFailedEdit = nullptr;
    QLineEdit *m_clientRunBeforeEdit = nullptr;
    QLineEdit *m_clientRunAfterEdit = nullptr;

    bool m_initialized = false;
};

/**
 * @brief Page 3: RunScript block editor
 */
class BJobScriptsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BJobScriptsPage(QWidget *parent = nullptr);

    void initializePage() override;

    QList<RunScriptEntry> runScripts() const;

private:
    BRunScriptEditor *m_editor = nullptr;
};

/**
 * @brief Page 4: Advanced directives via BResourceForm
 */
class BJobAdvancedPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BJobAdvancedPage(QWidget *parent = nullptr);

    void initializePage() override;

    QMap<QString, QString> collectValues() const;

private:
    BResourceForm *m_resourceForm = nullptr;
    bool m_initialized = false;
};

/**
 * @brief Page 5: Preview config, copy/export, execute configure add
 */
class BJobPreviewPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BJobPreviewPage(QWidget *parent = nullptr);

    void initializePage() override;
    bool validatePage() override;

private slots:
    void onCopyConfig();
    void onCopyCommand();
    void onJsonResponse(BDirector::Command cmd, const QString &jsonData);
    void onCommandResponse(BDirector::Command cmd, const QString &response);
    void onConfigureTimeout();

private:
    void generatePreview();
    void executeConfigureCommand();
    void disconnectDirectorSignals();

    QTextEdit *m_configEdit = nullptr;
    QTextEdit *m_commandEdit = nullptr;
    QLabel *m_warningLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    QProgressBar *m_progressBar = nullptr;
    QPushButton *m_copyConfigButton = nullptr;
    QPushButton *m_copyCommandButton = nullptr;
    QTimer *m_timeoutTimer = nullptr;
    bool m_executed = false;
    bool m_waitingForReload = false;
    QList<QMetaObject::Connection> m_connections;
};

// ============================================================================
// Main wizard class
// ============================================================================

/**
 * @brief Wizard for creating new Job or JobDefs resources
 *
 * Pages:
 *   1. Basics: Name, Type, JobDefs, Enabled
 *   2. Resources: Client, FileSet, Storage, Pool, Messages, Level, etc.
 *   3. Scripts: RunScript block editor
 *   4. Advanced: BResourceForm with remaining directives
 *   5. Preview: Config display, configure add execution
 */
class BJobWizard : public QWizard
{
    Q_OBJECT

public:
    enum ResourceType {
        JobType,
        JobDefsType
    };
    Q_ENUM(ResourceType)

    enum PageId {
        Page_Basics,
        Page_Resources,
        Page_Scripts,
        Page_Advanced,
        Page_Preview
    };

    explicit BJobWizard(ResourceType resourceType,
                        BDirector *director,
                        QWidget *parent = nullptr);
    ~BJobWizard() override;

    ResourceType resourceType() const { return m_resourceType; }
    BDirector *director() const { return m_director; }
    QString resourceTypeName() const { return m_resourceType == JobType ? "Job" : "JobDefs"; }

    void setReferenceData(const QMap<QString, QStringList> &referenceData);
    QMap<QString, QStringList> referenceData() const { return m_referenceData; }

    QMap<QString, QString> collectAllValues() const;
    QList<RunScriptEntry> collectRunScripts() const;
    QString generateConfigureCommand() const;
    QString generateConfigText() const;

private:
    void init();

    ResourceType m_resourceType;
    BDirector *m_director = nullptr;
    QMap<QString, QStringList> m_referenceData;
};

#endif // BJOBWIZARD_H
