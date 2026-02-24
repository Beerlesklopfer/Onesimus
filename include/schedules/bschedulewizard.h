/**
 * @file bschedulewizard.h
 * @brief Wizard for creating new Schedule resources
 *
 * Schema-driven from schedule.json metadata sections:
 * - run_directive_syntax: level values, pool/storage formats, priority default
 * - common_schedules: predefined templates (DailyIncremental, WeeklyCycle, etc.)
 * - time_specification_reference: day/week/month/time values
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#ifndef BSCHEDULEWIZARD_H
#define BSCHEDULEWIZARD_H

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
#include <QTimeEdit>
#include <QTableWidget>
#include <QDialogButtonBox>

#include "director/bdirector.h"
#include "models/bresourcemodels.h"

class BScheduleWizard;

// ============================================================================
// BScheduleRunDialog — Editor for a single Run entry
// ============================================================================

/**
 * @brief Dialog for editing a single schedule Run directive
 *
 * All dropdown values loaded from schema (run_directive_syntax, time_specification_reference).
 */
class BScheduleRunDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BScheduleRunDialog(QWidget *parent = nullptr);
    explicit BScheduleRunDialog(const BScheduleEntry &entry, QWidget *parent = nullptr);

    BScheduleEntry entry() const;

    void setReferenceData(const QMap<QString, QStringList> &referenceData);

private:
    void setupUi();
    void populateFromSchema();
    void populateFromEntry(const BScheduleEntry &entry);
    void updatePreview();

    // Level
    QComboBox *m_levelCombo = nullptr;

    // Days
    QCheckBox *m_dayChecks[7] = {};  // sun=0..sat=6 (Bareos order)
    QPushButton *m_dailyButton = nullptr;
    QPushButton *m_weekdaysButton = nullptr;

    // Week of month
    QComboBox *m_weekCombo = nullptr;

    // Time
    QTimeEdit *m_timeEdit = nullptr;

    // Optional overrides
    QComboBox *m_poolCombo = nullptr;
    QComboBox *m_storageCombo = nullptr;
    QSpinBox *m_prioritySpin = nullptr;

    // Preview
    QLabel *m_previewLabel = nullptr;

    QMap<QString, QStringList> m_referenceData;
};

// ============================================================================
// BScheduleRunEditor — Widget with list of all Run entries
// ============================================================================

/**
 * @brief Reusable widget for editing multiple schedule Run directives
 */
class BScheduleRunEditor : public QWidget
{
    Q_OBJECT

public:
    explicit BScheduleRunEditor(QWidget *parent = nullptr);

    QList<BScheduleEntry> entries() const { return m_entries; }
    void setEntries(const QList<BScheduleEntry> &entries);
    bool hasEntries() const { return !m_entries.isEmpty(); }

    void setReferenceData(const QMap<QString, QStringList> &referenceData);

signals:
    void entriesChanged();

private slots:
    void onAddRun();
    void onEditRun();
    void onRemoveRun();
    void onDuplicateRun();
    void onDoubleClicked(int row, int column);

private:
    void refreshTable();

    QTableWidget *m_table = nullptr;
    QPushButton *m_addButton = nullptr;
    QPushButton *m_editButton = nullptr;
    QPushButton *m_removeButton = nullptr;
    QPushButton *m_duplicateButton = nullptr;
    QList<BScheduleEntry> m_entries;
    QMap<QString, QStringList> m_referenceData;
};

// ============================================================================
// Wizard pages
// ============================================================================

/**
 * @brief Page 1: Schedule name, description, enabled, template selection
 */
class BScheduleBasicsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BScheduleBasicsPage(QWidget *parent = nullptr);

    void initializePage() override;
    bool isComplete() const override;

    QString scheduleName() const;
    QString scheduleDescription() const;
    bool enabled() const;

private slots:
    void onTemplateSelected(int index);

private:
    QLineEdit *m_nameEdit = nullptr;
    QLineEdit *m_descriptionEdit = nullptr;
    QCheckBox *m_enabledCheck = nullptr;
    QComboBox *m_templateCombo = nullptr;
    QLabel *m_templateDescription = nullptr;
};

/**
 * @brief Page 2: Run entries editor
 */
class BScheduleRunsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BScheduleRunsPage(QWidget *parent = nullptr);

    void initializePage() override;
    bool isComplete() const override;

    QList<BScheduleEntry> runEntries() const;

private:
    BScheduleRunEditor *m_editor = nullptr;
    bool m_initialized = false;
};

/**
 * @brief Page 3: Preview config + configure add + execute
 */
class BSchedulePreviewPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BSchedulePreviewPage(QWidget *parent = nullptr);

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
    QLabel *m_statusLabel = nullptr;
    QProgressBar *m_progressBar = nullptr;
    QPushButton *m_copyConfigButton = nullptr;
    QPushButton *m_copyCommandButton = nullptr;
    QTimer *m_timeoutTimer = nullptr;
    bool m_executed = false;
    QList<QMetaObject::Connection> m_connections;
};

// ============================================================================
// Main wizard class
// ============================================================================

/**
 * @brief Wizard for creating new Schedule resources
 *
 * Pages:
 *   1. Basics: Name, Description, Enabled, Template selection
 *   2. Runs: Schedule Run entries editor
 *   3. Preview: Config text, configure add, execute
 */
class BScheduleWizard : public QWizard
{
    Q_OBJECT

public:
    enum PageId {
        Page_Basics,
        Page_Runs,
        Page_Preview
    };

    explicit BScheduleWizard(BDirector *director, QWidget *parent = nullptr);
    ~BScheduleWizard() override;

    BDirector *director() const { return m_director; }

    void setReferenceData(const QMap<QString, QStringList> &referenceData);
    QMap<QString, QStringList> referenceData() const { return m_referenceData; }

    QString generateConfigText() const;
    QString generateConfigureCommand() const;

    /**
     * @brief Apply a template's runs to the run editor
     */
    void applyTemplate(const QStringList &runs);

private:
    void init();

    BDirector *m_director = nullptr;
    QMap<QString, QStringList> m_referenceData;
};

#endif // BSCHEDULEWIZARD_H
