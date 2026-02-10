/**
 * @file bjobwizard.cpp
 * @brief Wizard for creating new Job or JobDefs resources
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */

#include "jobs/bjobwizard.h"
#include "config/bresourceform.h"
#include "blogging.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QScrollArea>
#include <QHeaderView>
#include <QFileDialog>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>

// ============================================================================
// BRunScriptDialog
// ============================================================================

BRunScriptDialog::BRunScriptDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
}

BRunScriptDialog::BRunScriptDialog(const RunScriptEntry &entry, QWidget *parent)
    : QDialog(parent)
{
    setupUi();
    populateFromEntry(entry);
}

void BRunScriptDialog::setupUi()
{
    setWindowTitle(tr("Edit RunScript"));
    setMinimumWidth(450);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *form = new QFormLayout();

    // Command with browse button
    QHBoxLayout *cmdLayout = new QHBoxLayout();
    m_commandEdit = new QLineEdit(this);
    m_commandEdit->setPlaceholderText(tr("/path/to/script.sh"));
    QPushButton *browseBtn = new QPushButton(tr("..."), this);
    browseBtn->setFixedWidth(30);
    connect(browseBtn, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getOpenFileName(this, tr("Select Script"));
        if (!file.isEmpty()) {
            m_commandEdit->setText(file);
        }
    });
    cmdLayout->addWidget(m_commandEdit);
    cmdLayout->addWidget(browseBtn);
    form->addRow(tr("Command:"), cmdLayout);

    // RunsWhen combo
    m_runsWhenCombo = new QComboBox(this);
    m_runsWhenCombo->addItems({"Before", "After", "Always", "AfterVSS"});
    form->addRow(tr("Runs When:"), m_runsWhenCombo);

    mainLayout->addLayout(form);

    // Checkboxes
    m_runsOnClientCheck = new QCheckBox(tr("Runs On Client"), this);
    m_runsOnClientCheck->setChecked(true);
    mainLayout->addWidget(m_runsOnClientCheck);

    m_runsOnFailureCheck = new QCheckBox(tr("Runs On Failure"), this);
    mainLayout->addWidget(m_runsOnFailureCheck);

    m_abortOnErrorCheck = new QCheckBox(tr("Abort Job On Error"), this);
    m_abortOnErrorCheck->setChecked(true);
    mainLayout->addWidget(m_abortOnErrorCheck);

    m_failOnErrorCheck = new QCheckBox(tr("Fail Job On Error"), this);
    m_failOnErrorCheck->setChecked(true);
    mainLayout->addWidget(m_failOnErrorCheck);

    mainLayout->addSpacing(8);

    // Buttons
    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        if (m_commandEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, tr("Validation"), tr("Command is required."));
            return;
        }
        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttons);
}

void BRunScriptDialog::populateFromEntry(const RunScriptEntry &entry)
{
    m_commandEdit->setText(entry.command);
    m_runsWhenCombo->setCurrentText(entry.runsWhen);
    m_runsOnClientCheck->setChecked(entry.runsOnClient);
    m_runsOnFailureCheck->setChecked(entry.runsOnFailure);
    m_abortOnErrorCheck->setChecked(entry.abortJobOnError);
    m_failOnErrorCheck->setChecked(entry.failJobOnError);
}

RunScriptEntry BRunScriptDialog::entry() const
{
    RunScriptEntry e;
    e.command = m_commandEdit->text().trimmed();
    e.runsWhen = m_runsWhenCombo->currentText();
    e.runsOnClient = m_runsOnClientCheck->isChecked();
    e.runsOnFailure = m_runsOnFailureCheck->isChecked();
    e.abortJobOnError = m_abortOnErrorCheck->isChecked();
    e.failJobOnError = m_failOnErrorCheck->isChecked();
    return e;
}

// ============================================================================
// BRunScriptEditor
// ============================================================================

BRunScriptEditor::BRunScriptEditor(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Table
    m_table = new QTableWidget(0, 3, this);
    m_table->setHorizontalHeaderLabels({tr("Command"), tr("Runs When"), tr("On Client")});
    m_table->horizontalHeader()->setStretchLastSection(false);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->verticalHeader()->setVisible(false);
    connect(m_table, &QTableWidget::cellDoubleClicked, this, &BRunScriptEditor::onDoubleClicked);
    layout->addWidget(m_table);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    m_addButton = new QPushButton(tr("+ Add Script"), this);
    m_editButton = new QPushButton(tr("Edit"), this);
    m_removeButton = new QPushButton(tr("Remove"), this);
    m_editButton->setEnabled(false);
    m_removeButton->setEnabled(false);

    connect(m_addButton, &QPushButton::clicked, this, &BRunScriptEditor::onAddScript);
    connect(m_editButton, &QPushButton::clicked, this, &BRunScriptEditor::onEditScript);
    connect(m_removeButton, &QPushButton::clicked, this, &BRunScriptEditor::onRemoveScript);

    connect(m_table, &QTableWidget::currentCellChanged, this, [this](int row, int, int, int) {
        bool hasSelection = row >= 0 && row < m_entries.size();
        m_editButton->setEnabled(hasSelection);
        m_removeButton->setEnabled(hasSelection);
    });

    btnLayout->addWidget(m_addButton);
    btnLayout->addWidget(m_editButton);
    btnLayout->addWidget(m_removeButton);
    btnLayout->addStretch();
    layout->addLayout(btnLayout);
}

void BRunScriptEditor::setEntries(const QList<RunScriptEntry> &entries)
{
    m_entries = entries;
    refreshTable();
}

void BRunScriptEditor::onAddScript()
{
    BRunScriptDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        m_entries.append(dlg.entry());
        refreshTable();
        emit entriesChanged();
    }
}

void BRunScriptEditor::onEditScript()
{
    int row = m_table->currentRow();
    if (row < 0 || row >= m_entries.size()) return;

    BRunScriptDialog dlg(m_entries[row], this);
    if (dlg.exec() == QDialog::Accepted) {
        m_entries[row] = dlg.entry();
        refreshTable();
        emit entriesChanged();
    }
}

void BRunScriptEditor::onRemoveScript()
{
    int row = m_table->currentRow();
    if (row < 0 || row >= m_entries.size()) return;

    m_entries.removeAt(row);
    refreshTable();
    emit entriesChanged();
}

void BRunScriptEditor::onDoubleClicked(int row, int)
{
    if (row >= 0 && row < m_entries.size()) {
        m_table->setCurrentCell(row, 0);
        onEditScript();
    }
}

void BRunScriptEditor::refreshTable()
{
    m_table->setRowCount(m_entries.size());
    for (int i = 0; i < m_entries.size(); ++i) {
        const RunScriptEntry &e = m_entries[i];
        m_table->setItem(i, 0, new QTableWidgetItem(e.command));
        m_table->setItem(i, 1, new QTableWidgetItem(e.runsWhen));
        m_table->setItem(i, 2, new QTableWidgetItem(e.runsOnClient ? tr("Yes") : tr("No")));
    }
    m_editButton->setEnabled(false);
    m_removeButton->setEnabled(false);
}

// ============================================================================
// BJobBasicsPage
// ============================================================================

BJobBasicsPage::BJobBasicsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Basic Settings"));
    setSubTitle(tr("Configure the identity of the resource."));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QGroupBox *basicGroup = new QGroupBox(tr("Basic Settings"), this);
    QFormLayout *form = new QFormLayout(basicGroup);

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText(tr("e.g., BackupServer1"));
    form->addRow(tr("Name:"), m_nameEdit);

    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItems({"Backup", "Restore", "Verify", "Admin", "Migrate", "Copy", "Consolidate"});
    form->addRow(tr("Type:"), m_typeCombo);

    mainLayout->addWidget(basicGroup);

    // Inheritance group
    QGroupBox *inheritGroup = new QGroupBox(tr("Inheritance"), this);
    QVBoxLayout *inheritLayout = new QVBoxLayout(inheritGroup);
    QFormLayout *inheritForm = new QFormLayout();

    m_jobDefsCombo = new QComboBox(this);
    m_jobDefsCombo->setEditable(true);
    m_jobDefsCombo->addItem(QString()); // Empty = no inheritance
    inheritForm->addRow(tr("JobDefs:"), m_jobDefsCombo);

    inheritLayout->addLayout(inheritForm);

    m_jobDefsHint = new QLabel(this);
    m_jobDefsHint->setStyleSheet("color: gray; font-style: italic;");
    m_jobDefsHint->setWordWrap(true);
    m_jobDefsHint->hide();
    inheritLayout->addWidget(m_jobDefsHint);

    mainLayout->addWidget(inheritGroup);

    m_enabledCheck = new QCheckBox(tr("Enabled"), this);
    m_enabledCheck->setChecked(true);
    mainLayout->addWidget(m_enabledCheck);

    mainLayout->addStretch();

    // Validation signals
    connect(m_nameEdit, &QLineEdit::textChanged, this, &QWizardPage::completeChanged);
    connect(m_jobDefsCombo, &QComboBox::currentTextChanged, this, [this](const QString &name) {
        if (name.isEmpty()) {
            m_jobDefsHint->hide();
        } else {
            m_jobDefsHint->setText(tr("Inherits default settings from JobDefs \"%1\". "
                                       "Resource fields on the next page become optional.").arg(name));
            m_jobDefsHint->show();
        }
    });
}

void BJobBasicsPage::initializePage()
{
    auto *wiz = qobject_cast<BJobWizard*>(wizard());
    if (!wiz) return;

    // Update title based on resource type
    if (wiz->resourceType() == BJobWizard::JobDefsType) {
        setTitle(tr("Basic Settings (JobDefs)"));
    }

    // Populate JobDefs combo from reference data
    QStringList jobDefs = wiz->referenceData().value("JobDefs");
    QString current = m_jobDefsCombo->currentText();
    m_jobDefsCombo->clear();
    m_jobDefsCombo->addItem(QString()); // Empty option
    m_jobDefsCombo->addItems(jobDefs);
    if (!current.isEmpty()) {
        m_jobDefsCombo->setCurrentText(current);
    }
}

bool BJobBasicsPage::isComplete() const
{
    return !m_nameEdit->text().trimmed().isEmpty();
}

QString BJobBasicsPage::jobName() const { return m_nameEdit->text().trimmed(); }
QString BJobBasicsPage::jobType() const { return m_typeCombo->currentText(); }
QString BJobBasicsPage::jobDefs() const { return m_jobDefsCombo->currentText().trimmed(); }
bool BJobBasicsPage::enabled() const { return m_enabledCheck->isChecked(); }

// ============================================================================
// BJobResourcesPage
// ============================================================================

BJobResourcesPage::BJobResourcesPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Resource References"));
    setSubTitle(tr("Select the resources this job uses."));
    setupUi();
}

void BJobResourcesPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    QWidget *container = new QWidget(this);
    QVBoxLayout *containerLayout = new QVBoxLayout(container);

    // Required Resources group
    QGroupBox *reqGroup = new QGroupBox(tr("Required Resources"), container);
    QFormLayout *reqForm = new QFormLayout(reqGroup);

    m_clientCombo = new QComboBox(this);
    m_clientCombo->setEditable(true);
    reqForm->addRow(tr("Client:"), m_clientCombo);

    m_filesetCombo = new QComboBox(this);
    m_filesetCombo->setEditable(true);
    reqForm->addRow(tr("FileSet:"), m_filesetCombo);

    m_storageCombo = new QComboBox(this);
    m_storageCombo->setEditable(true);
    reqForm->addRow(tr("Storage:"), m_storageCombo);

    m_poolCombo = new QComboBox(this);
    m_poolCombo->setEditable(true);
    reqForm->addRow(tr("Pool:"), m_poolCombo);

    m_messagesCombo = new QComboBox(this);
    m_messagesCombo->setEditable(true);
    reqForm->addRow(tr("Messages:"), m_messagesCombo);

    containerLayout->addWidget(reqGroup);

    // Job Settings group
    QGroupBox *settingsGroup = new QGroupBox(tr("Job Settings"), container);
    QFormLayout *settingsForm = new QFormLayout(settingsGroup);

    m_levelCombo = new QComboBox(this);
    m_levelCombo->addItems({"", "Full", "Incremental", "Differential", "VirtualFull",
                            "Catalog", "InitCatalog", "VolumeToCatalog", "DiskToCatalog", "Data"});
    settingsForm->addRow(tr("Level:"), m_levelCombo);

    m_scheduleCombo = new QComboBox(this);
    m_scheduleCombo->setEditable(true);
    settingsForm->addRow(tr("Schedule:"), m_scheduleCombo);

    m_catalogCombo = new QComboBox(this);
    m_catalogCombo->setEditable(true);
    settingsForm->addRow(tr("Catalog:"), m_catalogCombo);

    m_prioritySpin = new QSpinBox(this);
    m_prioritySpin->setRange(1, 100);
    m_prioritySpin->setValue(10);
    settingsForm->addRow(tr("Priority:"), m_prioritySpin);

    m_maxConcurrentSpin = new QSpinBox(this);
    m_maxConcurrentSpin->setRange(1, 1000);
    m_maxConcurrentSpin->setValue(1);
    settingsForm->addRow(tr("Max Concurrent Jobs:"), m_maxConcurrentSpin);

    containerLayout->addWidget(settingsGroup);

    // Restore Settings group
    QGroupBox *restoreGroup = new QGroupBox(tr("Restore Settings"), container);
    QFormLayout *restoreForm = new QFormLayout(restoreGroup);

    m_whereEdit = new QLineEdit(this);
    m_whereEdit->setPlaceholderText(tr("/tmp/bareos-restores"));
    restoreForm->addRow(tr("Where:"), m_whereEdit);

    m_replaceCombo = new QComboBox(this);
    m_replaceCombo->addItems({"", "always", "ifnewer", "ifolder", "never"});
    restoreForm->addRow(tr("Replace:"), m_replaceCombo);

    containerLayout->addWidget(restoreGroup);

    // Pool Overrides group (collapsible)
    m_poolOverridesGroup = new QGroupBox(tr("Pool Overrides"), container);
    m_poolOverridesGroup->setCheckable(true);
    m_poolOverridesGroup->setChecked(false);
    QFormLayout *poolForm = new QFormLayout(m_poolOverridesGroup);

    m_fullPoolCombo = new QComboBox(this);
    m_fullPoolCombo->setEditable(true);
    poolForm->addRow(tr("Full Backup Pool:"), m_fullPoolCombo);

    m_diffPoolCombo = new QComboBox(this);
    m_diffPoolCombo->setEditable(true);
    poolForm->addRow(tr("Diff Backup Pool:"), m_diffPoolCombo);

    m_incPoolCombo = new QComboBox(this);
    m_incPoolCombo->setEditable(true);
    poolForm->addRow(tr("Inc Backup Pool:"), m_incPoolCombo);

    containerLayout->addWidget(m_poolOverridesGroup);

    // Simple Scripts group
    QGroupBox *scriptsGroup = new QGroupBox(tr("Simple Scripts"), container);
    QFormLayout *scriptsForm = new QFormLayout(scriptsGroup);

    m_runBeforeEdit = new QLineEdit(this);
    m_runBeforeEdit->setPlaceholderText(tr("/path/to/script.sh"));
    scriptsForm->addRow(tr("Run Before Job:"), m_runBeforeEdit);

    m_runAfterEdit = new QLineEdit(this);
    m_runAfterEdit->setPlaceholderText(tr("/path/to/script.sh"));
    scriptsForm->addRow(tr("Run After Job:"), m_runAfterEdit);

    m_runAfterFailedEdit = new QLineEdit(this);
    m_runAfterFailedEdit->setPlaceholderText(tr("/path/to/script.sh"));
    scriptsForm->addRow(tr("Run After Failed Job:"), m_runAfterFailedEdit);

    m_clientRunBeforeEdit = new QLineEdit(this);
    m_clientRunBeforeEdit->setPlaceholderText(tr("/path/to/client-script.sh"));
    scriptsForm->addRow(tr("Client Run Before:"), m_clientRunBeforeEdit);

    m_clientRunAfterEdit = new QLineEdit(this);
    m_clientRunAfterEdit->setPlaceholderText(tr("/path/to/client-script.sh"));
    scriptsForm->addRow(tr("Client Run After:"), m_clientRunAfterEdit);

    containerLayout->addWidget(scriptsGroup);

    containerLayout->addStretch();
    scroll->setWidget(container);
    mainLayout->addWidget(scroll);

    // Connect combos for completeness check
    auto comboChanged = [this](const QString &) { emit completeChanged(); };
    connect(m_clientCombo, &QComboBox::currentTextChanged, this, comboChanged);
    connect(m_filesetCombo, &QComboBox::currentTextChanged, this, comboChanged);
    connect(m_storageCombo, &QComboBox::currentTextChanged, this, comboChanged);
    connect(m_poolCombo, &QComboBox::currentTextChanged, this, comboChanged);
    connect(m_messagesCombo, &QComboBox::currentTextChanged, this, comboChanged);
}

void BJobResourcesPage::initializePage()
{
    if (!m_initialized) {
        populateCombos();
        m_initialized = true;
    }
}

void BJobResourcesPage::populateCombos()
{
    auto *wiz = qobject_cast<BJobWizard*>(wizard());
    if (!wiz) return;

    const QMap<QString, QStringList> &ref = wiz->referenceData();

    auto populateCombo = [](QComboBox *combo, const QStringList &items) {
        QString current = combo->currentText();
        combo->clear();
        combo->addItem(QString());
        combo->addItems(items);
        if (!current.isEmpty()) combo->setCurrentText(current);
    };

    populateCombo(m_clientCombo, ref.value("Client"));
    populateCombo(m_filesetCombo, ref.value("FileSet"));
    populateCombo(m_storageCombo, ref.value("Storage"));
    populateCombo(m_poolCombo, ref.value("Pool"));
    populateCombo(m_messagesCombo, ref.value("Messages"));
    populateCombo(m_scheduleCombo, ref.value("Schedule"));
    populateCombo(m_catalogCombo, ref.value("Catalog"));

    // Pool overrides use same Pool list
    populateCombo(m_fullPoolCombo, ref.value("Pool"));
    populateCombo(m_diffPoolCombo, ref.value("Pool"));
    populateCombo(m_incPoolCombo, ref.value("Pool"));
}

bool BJobResourcesPage::isComplete() const
{
    auto *wiz = qobject_cast<BJobWizard*>(wizard());
    if (!wiz) return false;

    auto *basicsPage = qobject_cast<BJobBasicsPage*>(wiz->page(BJobWizard::Page_Basics));
    if (!basicsPage) return false;

    // If using JobDefs, all fields become optional
    if (!basicsPage->jobDefs().isEmpty()) return true;

    // For JobDefs resource type, nothing required here
    if (wiz->resourceType() == BJobWizard::JobDefsType) return true;

    QString type = basicsPage->jobType();

    if (type == "Backup" || type == "Restore" || type == "Verify" || type == "Consolidate") {
        return !m_clientCombo->currentText().isEmpty()
            && !m_filesetCombo->currentText().isEmpty()
            && !m_storageCombo->currentText().isEmpty()
            && !m_poolCombo->currentText().isEmpty()
            && !m_messagesCombo->currentText().isEmpty();
    }
    if (type == "Admin") {
        return !m_messagesCombo->currentText().isEmpty();
    }
    if (type == "Migrate" || type == "Copy") {
        return !m_poolCombo->currentText().isEmpty()
            && !m_messagesCombo->currentText().isEmpty();
    }
    return true;
}

QMap<QString, QString> BJobResourcesPage::collectValues() const
{
    QMap<QString, QString> values;

    auto addIfSet = [&values](const QString &key, const QString &val) {
        if (!val.trimmed().isEmpty()) values[key] = val.trimmed();
    };
    auto addCombo = [&addIfSet](const QString &key, QComboBox *combo) {
        addIfSet(key, combo->currentText());
    };

    addCombo("Client", m_clientCombo);
    addCombo("FileSet", m_filesetCombo);
    addCombo("Storage", m_storageCombo);
    addCombo("Pool", m_poolCombo);
    addCombo("Messages", m_messagesCombo);
    addCombo("Level", m_levelCombo);
    addCombo("Schedule", m_scheduleCombo);
    addCombo("Catalog", m_catalogCombo);

    if (m_prioritySpin->value() != 10) {
        values["Priority"] = QString::number(m_prioritySpin->value());
    }
    if (m_maxConcurrentSpin->value() != 1) {
        values["Maximum Concurrent Jobs"] = QString::number(m_maxConcurrentSpin->value());
    }

    addIfSet("Where", m_whereEdit->text());
    addCombo("Replace", m_replaceCombo);

    // Pool overrides
    if (m_poolOverridesGroup->isChecked()) {
        addCombo("Full Backup Pool", m_fullPoolCombo);
        addCombo("Differential Backup Pool", m_diffPoolCombo);
        addCombo("Incremental Backup Pool", m_incPoolCombo);
    }

    // Simple scripts
    addIfSet("Run Before Job", m_runBeforeEdit->text());
    addIfSet("Run After Job", m_runAfterEdit->text());
    addIfSet("Run After Failed Job", m_runAfterFailedEdit->text());
    addIfSet("Client Run Before Job", m_clientRunBeforeEdit->text());
    addIfSet("Client Run After Job", m_clientRunAfterEdit->text());

    return values;
}

// ============================================================================
// BJobScriptsPage
// ============================================================================

BJobScriptsPage::BJobScriptsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("RunScript Blocks"));
    setSubTitle(tr("Define complex script execution blocks (optional). "
                   "For simple scripts, use the fields on the previous page."));

    QVBoxLayout *layout = new QVBoxLayout(this);
    m_editor = new BRunScriptEditor(this);
    layout->addWidget(m_editor);
}

void BJobScriptsPage::initializePage()
{
    // Nothing special needed
}

QList<RunScriptEntry> BJobScriptsPage::runScripts() const
{
    return m_editor->entries();
}

// ============================================================================
// BJobAdvancedPage
// ============================================================================

BJobAdvancedPage::BJobAdvancedPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Advanced Directives"));
    setSubTitle(tr("Optional settings for fine-tuning (can be skipped)."));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    // BResourceForm is created lazily in initializePage
}

void BJobAdvancedPage::initializePage()
{
    if (m_initialized) return;
    m_initialized = true;

    auto *wiz = qobject_cast<BJobWizard*>(wizard());
    if (!wiz) return;

    m_resourceForm = new BResourceForm(wiz->resourceTypeName(), this);

    // Exclude directives already on pages 1-3
    m_resourceForm->setExcludedDirectives({
        "Name", "Description", "Type", "JobDefs", "Enabled",
        "Client", "FileSet", "Storage", "Pool", "Schedule", "Messages", "Catalog",
        "Level", "Priority", "Where", "Replace",
        "Full Backup Pool", "Differential Backup Pool", "Incremental Backup Pool",
        "Maximum Concurrent Jobs",
        "Run Before Job", "Run After Job", "Run After Failed Job",
        "Client Run Before Job", "Client Run After Job",
        "Run Script"
    });

    // Set reference data for any remaining resource_reference fields
    m_resourceForm->setReferenceData(wiz->referenceData());

    // Show all directives (this IS the advanced page)
    m_resourceForm->setAdvancedVisible(true);

    layout()->addWidget(m_resourceForm);
}

QMap<QString, QString> BJobAdvancedPage::collectValues() const
{
    QMap<QString, QString> values;
    if (!m_resourceForm) return values;

    m_resourceForm->collectValues();
    BConfigResource res = m_resourceForm->resource();

    for (const QString &key : res.keys()) {
        BConfigValue val = res.value(key);
        if (val.type() == BConfigValue::Simple && !val.simpleValue().isEmpty()) {
            values[key] = val.simpleValue();
        }
    }

    return values;
}

// ============================================================================
// BJobPreviewPage
// ============================================================================

BJobPreviewPage::BJobPreviewPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Preview & Deploy"));
    setSubTitle(tr("Review the configuration and deploy to the Director."));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Config text
    QLabel *configLabel = new QLabel(tr("Configuration:"), this);
    configLabel->setStyleSheet("font-weight: bold;");
    mainLayout->addWidget(configLabel);

    m_configEdit = new QTextEdit(this);
    m_configEdit->setReadOnly(true);
    m_configEdit->setFont(QFont("Monospace", 9));
    m_configEdit->setMaximumHeight(200);
    mainLayout->addWidget(m_configEdit);

    // Copy config button
    QHBoxLayout *configBtnLayout = new QHBoxLayout();
    m_copyConfigButton = new QPushButton(tr("Copy Configuration"), this);
    connect(m_copyConfigButton, &QPushButton::clicked, this, &BJobPreviewPage::onCopyConfig);
    configBtnLayout->addWidget(m_copyConfigButton);
    configBtnLayout->addStretch();
    mainLayout->addLayout(configBtnLayout);

    // Configure command
    QLabel *cmdLabel = new QLabel(tr("Director Command:"), this);
    cmdLabel->setStyleSheet("font-weight: bold;");
    mainLayout->addWidget(cmdLabel);

    m_commandEdit = new QTextEdit(this);
    m_commandEdit->setReadOnly(true);
    m_commandEdit->setFont(QFont("Monospace", 9));
    m_commandEdit->setMaximumHeight(80);
    mainLayout->addWidget(m_commandEdit);

    // Copy command button
    QHBoxLayout *cmdBtnLayout = new QHBoxLayout();
    m_copyCommandButton = new QPushButton(tr("Copy Command"), this);
    connect(m_copyCommandButton, &QPushButton::clicked, this, &BJobPreviewPage::onCopyCommand);
    cmdBtnLayout->addWidget(m_copyCommandButton);
    cmdBtnLayout->addStretch();
    mainLayout->addLayout(cmdBtnLayout);

    // Warning label (shown if RunScript blocks present)
    m_warningLabel = new QLabel(this);
    m_warningLabel->setStyleSheet("color: #e8a838; font-style: italic;");
    m_warningLabel->setWordWrap(true);
    m_warningLabel->hide();
    mainLayout->addWidget(m_warningLabel);

    // Status area
    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    mainLayout->addWidget(m_statusLabel);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0); // Indeterminate
    m_progressBar->setVisible(false);
    mainLayout->addWidget(m_progressBar);

    mainLayout->addStretch();

    // Timeout timer
    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    m_timeoutTimer->setInterval(30000);
    connect(m_timeoutTimer, &QTimer::timeout, this, &BJobPreviewPage::onConfigureTimeout);
}

void BJobPreviewPage::initializePage()
{
    m_executed = false;
    m_waitingForReload = false;
    m_statusLabel->clear();
    m_progressBar->setVisible(false);
    generatePreview();
}

void BJobPreviewPage::generatePreview()
{
    auto *wiz = qobject_cast<BJobWizard*>(wizard());
    if (!wiz) return;

    m_configEdit->setPlainText(wiz->generateConfigText());
    m_commandEdit->setPlainText("configure " + wiz->generateConfigureCommand());

    // Show warning if RunScript blocks present
    QList<RunScriptEntry> scripts = wiz->collectRunScripts();
    if (!scripts.isEmpty()) {
        m_warningLabel->setText(
            tr("Note: RunScript blocks cannot be sent via 'configure add'. "
               "The command above creates the job without RunScript blocks. "
               "Deploy the full configuration manually using 'Copy Configuration' "
               "to /etc/bareos/bareos-dir.d/job/%1.conf and reload the Director.")
            .arg(wiz->collectAllValues().value("Name", "job")));
        m_warningLabel->show();
    } else {
        m_warningLabel->hide();
    }
}

bool BJobPreviewPage::validatePage()
{
    if (m_executed) return true;

    auto *wiz = qobject_cast<BJobWizard*>(wizard());
    if (!wiz || !wiz->director() || !wiz->director()->isConnected()) {
        // No director connection - allow closing
        return true;
    }

    int ret = QMessageBox::question(this, tr("Deploy to Director"),
        tr("Send 'configure add' to the Director?\n\n"
           "Click Yes to execute, No to close without deploying."),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (ret == QMessageBox::Cancel) return false;
    if (ret == QMessageBox::No) return true;

    executeConfigureCommand();
    return false; // Keep wizard open during execution
}

void BJobPreviewPage::executeConfigureCommand()
{
    auto *wiz = qobject_cast<BJobWizard*>(wizard());
    if (!wiz || !wiz->director()) return;

    m_progressBar->setVisible(true);
    m_statusLabel->setText(tr("Sending configure command..."));

    // Connect to Director signals
    m_connections.append(
        connect(wiz->director(), &BDirector::jsonResult,
                this, &BJobPreviewPage::onJsonResponse));
    m_connections.append(
        connect(wiz->director(), &BDirector::textResult,
                this, &BJobPreviewPage::onCommandResponse));

    m_timeoutTimer->start();

    QString cmd = wiz->generateConfigureCommand();
    BLOG_DEBUG() << "BJobWizard: Sending configure" << cmd.left(200);
    wiz->director()->doSend(BDirector::Command::Configure, cmd);
}

void BJobPreviewPage::onJsonResponse(BDirector::Command cmd, const QString &jsonData)
{
    if (cmd != BDirector::Command::Configure) return;

    m_timeoutTimer->stop();
    disconnectDirectorSignals();

    bool success = jsonData.contains("created", Qt::CaseInsensitive)
                || jsonData.contains("success", Qt::CaseInsensitive);

    if (success) {
        auto *wiz = qobject_cast<BJobWizard*>(wizard());
        if (wiz && wiz->director() && wiz->director()->isConnected()) {
            m_waitingForReload = true;
            m_statusLabel->setText(tr("Resource created. Reloading Director..."));

            m_connections.append(
                connect(wiz->director(), &BDirector::textResult,
                        this, &BJobPreviewPage::onCommandResponse));
            wiz->director()->doSend(BDirector::Command::Reload);
        } else {
            m_progressBar->setVisible(false);
            m_statusLabel->setText(tr("Resource created successfully."));
            m_statusLabel->setStyleSheet("color: green;");
            m_executed = true;
        }
    } else {
        m_progressBar->setVisible(false);
        m_statusLabel->setText(tr("Failed: %1").arg(jsonData.left(300)));
        m_statusLabel->setStyleSheet("color: red;");
    }
}

void BJobPreviewPage::onCommandResponse(BDirector::Command cmd, const QString &response)
{
    if (m_waitingForReload && cmd == BDirector::Command::Reload) {
        m_waitingForReload = false;
        disconnectDirectorSignals();
        m_progressBar->setVisible(false);
        m_statusLabel->setText(tr("Resource created and Director reloaded successfully."));
        m_statusLabel->setStyleSheet("color: green;");
        m_executed = true;
        return;
    }

    if (cmd != BDirector::Command::Configure) return;

    m_timeoutTimer->stop();
    disconnectDirectorSignals();

    bool success = response.contains("created", Qt::CaseInsensitive)
                || response.contains("success", Qt::CaseInsensitive)
                || response.contains("configure add", Qt::CaseInsensitive);

    if (success) {
        auto *wiz = qobject_cast<BJobWizard*>(wizard());
        if (wiz && wiz->director() && wiz->director()->isConnected()) {
            m_waitingForReload = true;
            m_statusLabel->setText(tr("Resource created. Reloading Director..."));

            m_connections.append(
                connect(wiz->director(), &BDirector::textResult,
                        this, &BJobPreviewPage::onCommandResponse));
            wiz->director()->doSend(BDirector::Command::Reload);
        } else {
            m_progressBar->setVisible(false);
            m_statusLabel->setText(tr("Resource created successfully."));
            m_statusLabel->setStyleSheet("color: green;");
            m_executed = true;
        }
    } else {
        m_progressBar->setVisible(false);
        m_statusLabel->setText(tr("Error: %1").arg(response.left(300)));
        m_statusLabel->setStyleSheet("color: red;");
    }
}

void BJobPreviewPage::onConfigureTimeout()
{
    disconnectDirectorSignals();
    m_progressBar->setVisible(false);
    m_statusLabel->setText(tr("Timeout waiting for Director response. "
                              "The command may still be processing."));
    m_statusLabel->setStyleSheet("color: orange;");
}

void BJobPreviewPage::onCopyConfig()
{
    QApplication::clipboard()->setText(m_configEdit->toPlainText());
    m_statusLabel->setText(tr("Configuration copied to clipboard."));
    m_statusLabel->setStyleSheet("color: gray;");
}

void BJobPreviewPage::onCopyCommand()
{
    QApplication::clipboard()->setText(m_commandEdit->toPlainText());
    m_statusLabel->setText(tr("Command copied to clipboard."));
    m_statusLabel->setStyleSheet("color: gray;");
}

void BJobPreviewPage::disconnectDirectorSignals()
{
    for (const QMetaObject::Connection &conn : m_connections) {
        QObject::disconnect(conn);
    }
    m_connections.clear();
}

// ============================================================================
// BJobWizard
// ============================================================================

BJobWizard::BJobWizard(ResourceType resourceType, BDirector *director, QWidget *parent)
    : QWizard(parent)
    , m_resourceType(resourceType)
    , m_director(director)
{
    init();
}

BJobWizard::~BJobWizard()
{
}

void BJobWizard::init()
{
    setWindowTitle(m_resourceType == JobType
                   ? tr("New Job Wizard")
                   : tr("New JobDefs Wizard"));
    setWizardStyle(QWizard::ModernStyle);
    setMinimumSize(800, 700);

    setPage(Page_Basics, new BJobBasicsPage());
    setPage(Page_Resources, new BJobResourcesPage());
    setPage(Page_Scripts, new BJobScriptsPage());
    setPage(Page_Advanced, new BJobAdvancedPage());
    setPage(Page_Preview, new BJobPreviewPage());

    setButtonText(QWizard::FinishButton, tr("Finish"));
}

void BJobWizard::setReferenceData(const QMap<QString, QStringList> &referenceData)
{
    m_referenceData = referenceData;
}

QMap<QString, QString> BJobWizard::collectAllValues() const
{
    QMap<QString, QString> values;

    // Page 1: Basics
    auto *basicsPage = qobject_cast<BJobBasicsPage*>(page(Page_Basics));
    if (basicsPage) {
        values["Name"] = basicsPage->jobName();
        values["Type"] = basicsPage->jobType();
        if (!basicsPage->jobDefs().isEmpty())
            values["JobDefs"] = basicsPage->jobDefs();
        if (!basicsPage->enabled())
            values["Enabled"] = "no";
    }

    // Page 2: Resources
    auto *resourcesPage = qobject_cast<BJobResourcesPage*>(page(Page_Resources));
    if (resourcesPage) {
        QMap<QString, QString> resValues = resourcesPage->collectValues();
        for (auto it = resValues.constBegin(); it != resValues.constEnd(); ++it) {
            if (!it.value().isEmpty())
                values[it.key()] = it.value();
        }
    }

    // Page 4: Advanced (from BResourceForm)
    auto *advancedPage = qobject_cast<BJobAdvancedPage*>(page(Page_Advanced));
    if (advancedPage) {
        QMap<QString, QString> advValues = advancedPage->collectValues();
        for (auto it = advValues.constBegin(); it != advValues.constEnd(); ++it) {
            if (!it.value().isEmpty())
                values[it.key()] = it.value();
        }
    }

    return values;
}

QList<RunScriptEntry> BJobWizard::collectRunScripts() const
{
    auto *scriptsPage = qobject_cast<BJobScriptsPage*>(page(Page_Scripts));
    return scriptsPage ? scriptsPage->runScripts() : QList<RunScriptEntry>();
}

QString BJobWizard::generateConfigureCommand() const
{
    QMap<QString, QString> values = collectAllValues();
    QString resourceName = (m_resourceType == JobType) ? "job" : "jobdefs";

    QString cmd = QString("add %1").arg(resourceName);

    for (auto it = values.constBegin(); it != values.constEnd(); ++it) {
        // Normalize key: "Full Backup Pool" -> "fullbackuppool"
        QString key = it.key().toLower().remove(' ');
        QString val = it.value();

        // Quote values containing spaces or special chars
        if (val.contains(' ') || val.contains('"') || val.contains('=')) {
            val = QString("\"%1\"").arg(val.replace("\"", "\\\""));
        }
        cmd += QString(" %1=%2").arg(key, val);
    }

    return cmd;
}

QString BJobWizard::generateConfigText() const
{
    QMap<QString, QString> values = collectAllValues();
    QList<RunScriptEntry> scripts = collectRunScripts();
    QString typeName = (m_resourceType == JobType) ? "Job" : "JobDefs";

    QString config;
    config += QString("%1 {\n").arg(typeName);

    for (auto it = values.constBegin(); it != values.constEnd(); ++it) {
        QString val = it.value();
        // Don't quote pure numbers or yes/no
        bool isNumber = false;
        val.toInt(&isNumber);
        if (!isNumber && val != "yes" && val != "no") {
            config += QString("  %1 = \"%2\"\n").arg(it.key(), val);
        } else {
            config += QString("  %1 = %2\n").arg(it.key(), val);
        }
    }

    // RunScript blocks
    for (const RunScriptEntry &s : scripts) {
        config += "  RunScript {\n";
        config += QString("    Command = \"%1\"\n").arg(s.command);
        config += QString("    RunsWhen = %1\n").arg(s.runsWhen);
        config += QString("    RunsOnClient = %1\n").arg(s.runsOnClient ? "Yes" : "No");
        if (s.runsOnFailure)
            config += "    RunsOnFailure = Yes\n";
        if (!s.abortJobOnError)
            config += "    AbortJobOnError = No\n";
        if (!s.failJobOnError)
            config += "    FailJobOnError = No\n";
        config += "  }\n";
    }

    config += "}\n";
    return config;
}
