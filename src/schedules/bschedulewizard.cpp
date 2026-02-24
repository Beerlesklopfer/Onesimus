/**
 * @file bschedulewizard.cpp
 * @brief Wizard for creating new Schedule resources
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "schedules/bschedulewizard.h"
#include "config/bdirectiveschema.h"
#include "blogging.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QScrollArea>
#include <QHeaderView>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>

// ============================================================================
// BScheduleRunDialog
// ============================================================================

BScheduleRunDialog::BScheduleRunDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
    populateFromSchema();
}

BScheduleRunDialog::BScheduleRunDialog(const BScheduleEntry &entry, QWidget *parent)
    : QDialog(parent)
{
    setupUi();
    populateFromSchema();
    populateFromEntry(entry);
}

void BScheduleRunDialog::setupUi()
{
    setWindowTitle(tr("Edit Run Directive"));
    setMinimumWidth(500);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Level
    QFormLayout *form = new QFormLayout();
    m_levelCombo = new QComboBox(this);
    form->addRow(tr("Level:"), m_levelCombo);

    // Time
    m_timeEdit = new QTimeEdit(QTime(23, 5), this);
    m_timeEdit->setDisplayFormat("HH:mm");
    form->addRow(tr("Time:"), m_timeEdit);

    // Week of month
    m_weekCombo = new QComboBox(this);
    m_weekCombo->addItem(tr("(every week)"), QString());
    form->addRow(tr("Week of Month:"), m_weekCombo);

    mainLayout->addLayout(form);

    // Days of week
    QGroupBox *daysGroup = new QGroupBox(tr("Days of Week"), this);
    QVBoxLayout *daysLayout = new QVBoxLayout(daysGroup);

    QHBoxLayout *dayChecksLayout = new QHBoxLayout();
    // Bareos order: sun=0, mon=1, ..., sat=6
    static const QStringList dayLabels = {
        tr("Sun"), tr("Mon"), tr("Tue"), tr("Wed"),
        tr("Thu"), tr("Fri"), tr("Sat")
    };
    for (int i = 0; i < 7; ++i) {
        m_dayChecks[i] = new QCheckBox(dayLabels[i], this);
        connect(m_dayChecks[i], &QCheckBox::toggled, this, &BScheduleRunDialog::updatePreview);
        dayChecksLayout->addWidget(m_dayChecks[i]);
    }
    daysLayout->addLayout(dayChecksLayout);

    // Quick-select buttons
    QHBoxLayout *quickLayout = new QHBoxLayout();
    m_dailyButton = new QPushButton(tr("Daily"), this);
    connect(m_dailyButton, &QPushButton::clicked, this, [this]() {
        for (int i = 0; i < 7; ++i) m_dayChecks[i]->setChecked(true);
    });
    m_weekdaysButton = new QPushButton(tr("Mon-Fri"), this);
    connect(m_weekdaysButton, &QPushButton::clicked, this, [this]() {
        for (int i = 0; i < 7; ++i)
            m_dayChecks[i]->setChecked(i >= 1 && i <= 5);
    });
    QPushButton *clearButton = new QPushButton(tr("Clear"), this);
    connect(clearButton, &QPushButton::clicked, this, [this]() {
        for (int i = 0; i < 7; ++i) m_dayChecks[i]->setChecked(false);
    });
    quickLayout->addWidget(m_dailyButton);
    quickLayout->addWidget(m_weekdaysButton);
    quickLayout->addWidget(clearButton);
    quickLayout->addStretch();
    daysLayout->addLayout(quickLayout);

    mainLayout->addWidget(daysGroup);

    // Optional overrides
    QGroupBox *overridesGroup = new QGroupBox(tr("Optional Overrides"), this);
    QFormLayout *overForm = new QFormLayout(overridesGroup);

    m_poolCombo = new QComboBox(this);
    m_poolCombo->setEditable(true);
    m_poolCombo->addItem(QString());  // empty = no override
    overForm->addRow(tr("Pool:"), m_poolCombo);

    m_storageCombo = new QComboBox(this);
    m_storageCombo->setEditable(true);
    m_storageCombo->addItem(QString());
    overForm->addRow(tr("Storage:"), m_storageCombo);

    m_prioritySpin = new QSpinBox(this);
    m_prioritySpin->setRange(1, 99);
    m_prioritySpin->setValue(10);
    m_prioritySpin->setSpecialValueText(tr("(default)"));
    overForm->addRow(tr("Priority:"), m_prioritySpin);

    mainLayout->addWidget(overridesGroup);

    // Preview
    m_previewLabel = new QLabel(this);
    m_previewLabel->setStyleSheet(
        "background-color: #f5f5f5; padding: 6px; border: 1px solid #ccc; "
        "font-family: monospace; font-size: 10pt;");
    m_previewLabel->setWordWrap(true);
    mainLayout->addWidget(new QLabel(tr("Preview:"), this));
    mainLayout->addWidget(m_previewLabel);

    // Connect signals for live preview
    connect(m_levelCombo, &QComboBox::currentTextChanged, this, &BScheduleRunDialog::updatePreview);
    connect(m_timeEdit, &QTimeEdit::timeChanged, this, &BScheduleRunDialog::updatePreview);
    connect(m_weekCombo, &QComboBox::currentTextChanged, this, &BScheduleRunDialog::updatePreview);
    connect(m_poolCombo, &QComboBox::currentTextChanged, this, &BScheduleRunDialog::updatePreview);
    connect(m_storageCombo, &QComboBox::currentTextChanged, this, &BScheduleRunDialog::updatePreview);
    connect(m_prioritySpin, &QSpinBox::valueChanged, this, &BScheduleRunDialog::updatePreview);

    // Buttons
    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        // At least one day must be selected
        bool anyDay = false;
        for (int i = 0; i < 7; ++i) {
            if (m_dayChecks[i]->isChecked()) { anyDay = true; break; }
        }
        if (!anyDay) {
            QMessageBox::warning(this, tr("Validation"),
                tr("Please select at least one day of the week."));
            return;
        }
        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttons);
}

void BScheduleRunDialog::populateFromSchema()
{
    BDirectiveSchema &schema = BDirectiveSchema::instance();

    // Level values from schema
    QStringList levels = schema.scheduleLevelValues();
    if (levels.isEmpty()) {
        levels = {"Full", "Differential", "Incremental", "VirtualFull"};
    }
    m_levelCombo->addItems(levels);
    m_levelCombo->setCurrentText("Incremental");

    // Week of month values from schema
    QStringList weeks = schema.weekOfMonthValues();
    for (const QString &w : weeks) {
        m_weekCombo->addItem(w, w);
    }

    // Default: all days selected (daily)
    for (int i = 0; i < 7; ++i) {
        m_dayChecks[i]->setChecked(true);
    }

    updatePreview();
}

void BScheduleRunDialog::setReferenceData(const QMap<QString, QStringList> &referenceData)
{
    m_referenceData = referenceData;

    // Populate pool combo
    QStringList pools = referenceData.value("Pool");
    QString currentPool = m_poolCombo->currentText();
    m_poolCombo->clear();
    m_poolCombo->addItem(QString());
    m_poolCombo->addItems(pools);
    if (!currentPool.isEmpty()) m_poolCombo->setCurrentText(currentPool);

    // Populate storage combo
    QStringList storages = referenceData.value("Storage");
    QString currentStorage = m_storageCombo->currentText();
    m_storageCombo->clear();
    m_storageCombo->addItem(QString());
    m_storageCombo->addItems(storages);
    if (!currentStorage.isEmpty()) m_storageCombo->setCurrentText(currentStorage);
}

void BScheduleRunDialog::populateFromEntry(const BScheduleEntry &entry)
{
    m_levelCombo->setCurrentText(BScheduleEntry::levelToString(entry.level));
    m_timeEdit->setTime(QTime(entry.hour, entry.minute));

    if (!entry.weekSpec.isEmpty()) {
        m_weekCombo->setCurrentText(entry.weekSpec);
    } else {
        m_weekCombo->setCurrentIndex(0);  // (every week)
    }

    // Days: BScheduleEntry uses 0=Mon..6=Sun, Bareos uses sun=0..sat=6
    // Convert: Mon=0 → bareos index 1, Sun=6 → bareos index 0
    for (int i = 0; i < 7; ++i) m_dayChecks[i]->setChecked(false);
    for (int d : entry.daysOfWeek) {
        // d is 0=Mon..6=Sun in BScheduleEntry
        // Bareos dialog order: 0=Sun, 1=Mon, ..., 6=Sat
        int bareosIdx = (d + 1) % 7;
        if (bareosIdx >= 0 && bareosIdx < 7) {
            m_dayChecks[bareosIdx]->setChecked(true);
        }
    }

    if (!entry.pool.isEmpty()) m_poolCombo->setCurrentText(entry.pool);
    if (!entry.storage.isEmpty()) m_storageCombo->setCurrentText(entry.storage);
    if (entry.priority != 10) m_prioritySpin->setValue(entry.priority);

    updatePreview();
}

BScheduleEntry BScheduleRunDialog::entry() const
{
    BScheduleEntry e;
    e.level = BScheduleEntry::levelFromString(m_levelCombo->currentText());
    e.hour = m_timeEdit->time().hour();
    e.minute = m_timeEdit->time().minute();

    // Week spec
    QString week = m_weekCombo->currentData().toString();
    e.weekSpec = week;

    // Days: dialog uses bareos order (0=Sun..6=Sat), convert to BScheduleEntry (0=Mon..6=Sun)
    for (int i = 0; i < 7; ++i) {
        if (m_dayChecks[i]->isChecked()) {
            // Bareos: 0=Sun, 1=Mon, ..., 6=Sat
            // BScheduleEntry: 0=Mon, ..., 5=Sat, 6=Sun
            int entryIdx = (i == 0) ? 6 : (i - 1);
            e.daysOfWeek.append(entryIdx);
        }
    }

    e.pool = m_poolCombo->currentText().trimmed();
    e.storage = m_storageCombo->currentText().trimmed();
    e.priority = m_prioritySpin->value();

    return e;
}

void BScheduleRunDialog::updatePreview()
{
    BScheduleEntry e = entry();
    m_previewLabel->setText(QString("Run = %1").arg(e.toRunDirective()));
}

// ============================================================================
// BScheduleRunEditor
// ============================================================================

BScheduleRunEditor::BScheduleRunEditor(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Table
    m_table = new QTableWidget(0, 4, this);
    m_table->setHorizontalHeaderLabels({tr("Level"), tr("Days"), tr("Time"), tr("Pool")});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->verticalHeader()->setVisible(false);
    m_table->setAlternatingRowColors(true);
    connect(m_table, &QTableWidget::cellDoubleClicked, this, &BScheduleRunEditor::onDoubleClicked);
    layout->addWidget(m_table);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    m_addButton = new QPushButton(tr("+ Add Run"), this);
    m_editButton = new QPushButton(tr("Edit"), this);
    m_removeButton = new QPushButton(tr("Remove"), this);
    m_duplicateButton = new QPushButton(tr("Duplicate"), this);
    m_editButton->setEnabled(false);
    m_removeButton->setEnabled(false);
    m_duplicateButton->setEnabled(false);

    connect(m_addButton, &QPushButton::clicked, this, &BScheduleRunEditor::onAddRun);
    connect(m_editButton, &QPushButton::clicked, this, &BScheduleRunEditor::onEditRun);
    connect(m_removeButton, &QPushButton::clicked, this, &BScheduleRunEditor::onRemoveRun);
    connect(m_duplicateButton, &QPushButton::clicked, this, &BScheduleRunEditor::onDuplicateRun);

    connect(m_table, &QTableWidget::currentCellChanged, this, [this](int row, int, int, int) {
        bool hasSelection = row >= 0 && row < m_entries.size();
        m_editButton->setEnabled(hasSelection);
        m_removeButton->setEnabled(hasSelection);
        m_duplicateButton->setEnabled(hasSelection);
    });

    btnLayout->addWidget(m_addButton);
    btnLayout->addWidget(m_editButton);
    btnLayout->addWidget(m_duplicateButton);
    btnLayout->addWidget(m_removeButton);
    btnLayout->addStretch();
    layout->addLayout(btnLayout);
}

void BScheduleRunEditor::setEntries(const QList<BScheduleEntry> &entries)
{
    m_entries = entries;
    refreshTable();
    emit entriesChanged();
}

void BScheduleRunEditor::setReferenceData(const QMap<QString, QStringList> &referenceData)
{
    m_referenceData = referenceData;
}

void BScheduleRunEditor::onAddRun()
{
    BScheduleRunDialog dlg(this);
    dlg.setReferenceData(m_referenceData);
    if (dlg.exec() == QDialog::Accepted) {
        m_entries.append(dlg.entry());
        refreshTable();
        emit entriesChanged();
    }
}

void BScheduleRunEditor::onEditRun()
{
    int row = m_table->currentRow();
    if (row < 0 || row >= m_entries.size()) return;

    BScheduleRunDialog dlg(m_entries[row], this);
    dlg.setReferenceData(m_referenceData);
    if (dlg.exec() == QDialog::Accepted) {
        m_entries[row] = dlg.entry();
        refreshTable();
        emit entriesChanged();
    }
}

void BScheduleRunEditor::onRemoveRun()
{
    int row = m_table->currentRow();
    if (row < 0 || row >= m_entries.size()) return;

    m_entries.removeAt(row);
    refreshTable();
    emit entriesChanged();
}

void BScheduleRunEditor::onDuplicateRun()
{
    int row = m_table->currentRow();
    if (row < 0 || row >= m_entries.size()) return;

    m_entries.append(m_entries[row]);
    refreshTable();
    emit entriesChanged();
}

void BScheduleRunEditor::onDoubleClicked(int row, int)
{
    if (row >= 0 && row < m_entries.size()) {
        m_table->setCurrentCell(row, 0);
        onEditRun();
    }
}

void BScheduleRunEditor::refreshTable()
{
    m_table->setRowCount(m_entries.size());
    static const QStringList dayAbbrev = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    for (int i = 0; i < m_entries.size(); ++i) {
        const BScheduleEntry &e = m_entries[i];

        // Level
        m_table->setItem(i, 0, new QTableWidgetItem(BScheduleEntry::levelToString(e.level)));

        // Days
        QStringList days;
        if (e.daysOfWeek.size() == 7) {
            days.append(tr("daily"));
        } else {
            for (int d : e.daysOfWeek) {
                if (d >= 0 && d < 7) days.append(dayAbbrev[d]);
            }
        }
        if (!e.weekSpec.isEmpty()) {
            days.prepend(e.weekSpec);
        }
        m_table->setItem(i, 1, new QTableWidgetItem(days.join(" ")));

        // Time
        m_table->setItem(i, 2, new QTableWidgetItem(
            QString("%1:%2").arg(e.hour, 2, 10, QChar('0')).arg(e.minute, 2, 10, QChar('0'))));

        // Pool
        m_table->setItem(i, 3, new QTableWidgetItem(e.pool));
    }

    m_editButton->setEnabled(false);
    m_removeButton->setEnabled(false);
    m_duplicateButton->setEnabled(false);
}


// ============================================================================
// BScheduleBasicsPage
// ============================================================================

BScheduleBasicsPage::BScheduleBasicsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Schedule Basics"));
    setSubTitle(tr("Configure the schedule name and select an optional template."));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Basic settings
    QGroupBox *basicGroup = new QGroupBox(tr("Basic Settings"), this);
    QFormLayout *form = new QFormLayout(basicGroup);

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText(tr("e.g., WeeklyCycle"));
    form->addRow(tr("Name:"), m_nameEdit);

    m_descriptionEdit = new QLineEdit(this);
    m_descriptionEdit->setPlaceholderText(tr("e.g., Full Sunday, incremental daily"));
    form->addRow(tr("Description:"), m_descriptionEdit);

    m_enabledCheck = new QCheckBox(tr("Enabled"), this);
    m_enabledCheck->setChecked(true);
    form->addRow(QString(), m_enabledCheck);

    mainLayout->addWidget(basicGroup);

    // Template selection
    QGroupBox *templateGroup = new QGroupBox(tr("Template"), this);
    QVBoxLayout *templateLayout = new QVBoxLayout(templateGroup);

    m_templateCombo = new QComboBox(this);
    m_templateCombo->addItem(tr("(Empty — build from scratch)"), QString());
    templateLayout->addWidget(m_templateCombo);

    m_templateDescription = new QLabel(this);
    m_templateDescription->setStyleSheet("color: gray; font-style: italic;");
    m_templateDescription->setWordWrap(true);
    m_templateDescription->setText(tr("Select a template to pre-fill the schedule with common run directives."));
    templateLayout->addWidget(m_templateDescription);

    mainLayout->addWidget(templateGroup);
    mainLayout->addStretch();

    // Validation signal
    connect(m_nameEdit, &QLineEdit::textChanged, this, &QWizardPage::completeChanged);
    connect(m_templateCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleBasicsPage::onTemplateSelected);
}

void BScheduleBasicsPage::initializePage()
{
    // Load templates from schema
    BDirectiveSchema &schema = BDirectiveSchema::instance();
    QJsonObject templates = schema.commonSchedules();

    // Only add templates if not already populated
    if (m_templateCombo->count() <= 1) {
        for (auto it = templates.begin(); it != templates.end(); ++it) {
            QJsonObject tmpl = it.value().toObject();
            QString name = tmpl["name"].toString();
            QString desc = tmpl["description"].toString();
            m_templateCombo->addItem(name, it.key());
            m_templateCombo->setItemData(m_templateCombo->count() - 1,
                                         desc, Qt::ToolTipRole);
        }
    }
}

bool BScheduleBasicsPage::isComplete() const
{
    return !m_nameEdit->text().trimmed().isEmpty();
}

QString BScheduleBasicsPage::scheduleName() const { return m_nameEdit->text().trimmed(); }
QString BScheduleBasicsPage::scheduleDescription() const { return m_descriptionEdit->text().trimmed(); }
bool BScheduleBasicsPage::enabled() const { return m_enabledCheck->isChecked(); }

void BScheduleBasicsPage::onTemplateSelected(int index)
{
    QString key = m_templateCombo->itemData(index).toString();

    if (key.isEmpty()) {
        m_templateDescription->setText(
            tr("Select a template to pre-fill the schedule with common run directives."));
        return;
    }

    BDirectiveSchema &schema = BDirectiveSchema::instance();
    QJsonObject templates = schema.commonSchedules();
    QJsonObject tmpl = templates[key].toObject();

    QString desc = tmpl["description"].toString();
    QString name = tmpl["name"].toString();
    QJsonArray runsArr = tmpl["runs"].toArray();

    m_templateDescription->setText(
        tr("<b>%1</b>: %2<br>%3 run directive(s)")
        .arg(name, desc).arg(runsArr.size()));

    // Auto-fill name if empty
    if (m_nameEdit->text().trimmed().isEmpty()) {
        m_nameEdit->setText(name);
    }

    // Apply template runs to wizard
    auto *wiz = qobject_cast<BScheduleWizard*>(wizard());
    if (wiz) {
        QStringList runs;
        for (const QJsonValue &v : runsArr) {
            runs.append(v.toString());
        }
        wiz->applyTemplate(runs);
    }
}


// ============================================================================
// BScheduleRunsPage
// ============================================================================

BScheduleRunsPage::BScheduleRunsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Run Directives"));
    setSubTitle(tr("Define when this schedule runs backups. Add one or more Run entries."));

    QVBoxLayout *layout = new QVBoxLayout(this);
    m_editor = new BScheduleRunEditor(this);
    layout->addWidget(m_editor);

    connect(m_editor, &BScheduleRunEditor::entriesChanged,
            this, &QWizardPage::completeChanged);
}

void BScheduleRunsPage::initializePage()
{
    auto *wiz = qobject_cast<BScheduleWizard*>(wizard());
    if (wiz) {
        m_editor->setReferenceData(wiz->referenceData());
    }
    m_initialized = true;
}

bool BScheduleRunsPage::isComplete() const
{
    return m_editor->hasEntries();
}

QList<BScheduleEntry> BScheduleRunsPage::runEntries() const
{
    return m_editor->entries();
}


// ============================================================================
// BSchedulePreviewPage
// ============================================================================

BSchedulePreviewPage::BSchedulePreviewPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Preview & Deploy"));
    setSubTitle(tr("Review the schedule configuration and deploy to the Director."));

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

    QHBoxLayout *configBtnLayout = new QHBoxLayout();
    m_copyConfigButton = new QPushButton(tr("Copy Configuration"), this);
    connect(m_copyConfigButton, &QPushButton::clicked, this, &BSchedulePreviewPage::onCopyConfig);
    configBtnLayout->addWidget(m_copyConfigButton);
    configBtnLayout->addStretch();
    mainLayout->addLayout(configBtnLayout);

    // Console command
    QLabel *cmdLabel = new QLabel(tr("Director Command:"), this);
    cmdLabel->setStyleSheet("font-weight: bold;");
    mainLayout->addWidget(cmdLabel);

    m_commandEdit = new QTextEdit(this);
    m_commandEdit->setReadOnly(true);
    m_commandEdit->setFont(QFont("Monospace", 9));
    m_commandEdit->setMaximumHeight(80);
    mainLayout->addWidget(m_commandEdit);

    QHBoxLayout *cmdBtnLayout = new QHBoxLayout();
    m_copyCommandButton = new QPushButton(tr("Copy Command"), this);
    connect(m_copyCommandButton, &QPushButton::clicked, this, &BSchedulePreviewPage::onCopyCommand);
    cmdBtnLayout->addWidget(m_copyCommandButton);
    cmdBtnLayout->addStretch();
    mainLayout->addLayout(cmdBtnLayout);

    // Status
    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    mainLayout->addWidget(m_statusLabel);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0);
    m_progressBar->setVisible(false);
    mainLayout->addWidget(m_progressBar);

    mainLayout->addStretch();

    // Timeout
    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    m_timeoutTimer->setInterval(30000);
    connect(m_timeoutTimer, &QTimer::timeout, this, &BSchedulePreviewPage::onConfigureTimeout);
}

void BSchedulePreviewPage::initializePage()
{
    m_executed = false;
    m_statusLabel->clear();
    m_progressBar->setVisible(false);
    generatePreview();
}

void BSchedulePreviewPage::generatePreview()
{
    auto *wiz = qobject_cast<BScheduleWizard*>(wizard());
    if (!wiz) return;

    m_configEdit->setPlainText(wiz->generateConfigText());
    m_commandEdit->setPlainText("configure " + wiz->generateConfigureCommand());
}

bool BSchedulePreviewPage::validatePage()
{
    if (m_executed) return true;

    auto *wiz = qobject_cast<BScheduleWizard*>(wizard());
    if (!wiz || !wiz->director() || !wiz->director()->isConnected()) {
        return true;
    }

    int ret = QMessageBox::question(this, tr("Deploy to Director"),
        tr("Send 'configure add' to the Director?\n\n"
           "Click Yes to execute, No to close without deploying."),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (ret == QMessageBox::Cancel) return false;
    if (ret == QMessageBox::No) return true;

    executeConfigureCommand();
    return false;
}

void BSchedulePreviewPage::executeConfigureCommand()
{
    auto *wiz = qobject_cast<BScheduleWizard*>(wizard());
    if (!wiz || !wiz->director()) return;

    m_progressBar->setVisible(true);
    m_statusLabel->setText(tr("Sending configure command..."));

    m_connections.append(
        connect(wiz->director(), &BDirector::jsonResult,
                this, &BSchedulePreviewPage::onJsonResponse));
    m_connections.append(
        connect(wiz->director(), &BDirector::textResult,
                this, &BSchedulePreviewPage::onCommandResponse));

    m_timeoutTimer->start();

    QString cmd = wiz->generateConfigureCommand();
    BLOG_DEBUG() << "BScheduleWizard: Sending configure" << cmd.left(200);
    wiz->director()->doSend(BDirector::Command::Configure, cmd);
}

void BSchedulePreviewPage::onJsonResponse(BDirector::Command cmd, const QString &jsonData)
{
    if (cmd != BDirector::Command::Configure) return;

    m_timeoutTimer->stop();
    disconnectDirectorSignals();

    bool success = jsonData.contains("created", Qt::CaseInsensitive)
                || jsonData.contains("success", Qt::CaseInsensitive);

    m_progressBar->setVisible(false);

    if (success) {
        m_executed = true;
        m_statusLabel->setStyleSheet("color: green; font-weight: bold;");
        m_statusLabel->setText(tr("Schedule created successfully! Click Finish to close."));
    } else {
        m_statusLabel->setStyleSheet("color: red;");
        m_statusLabel->setText(tr("Error: %1").arg(jsonData.left(500)));
    }
}

void BSchedulePreviewPage::onCommandResponse(BDirector::Command cmd, const QString &response)
{
    if (cmd != BDirector::Command::Configure) return;

    m_timeoutTimer->stop();
    disconnectDirectorSignals();
    m_progressBar->setVisible(false);

    bool success = response.contains("created", Qt::CaseInsensitive)
                || response.contains("success", Qt::CaseInsensitive);

    if (success) {
        m_executed = true;
        m_statusLabel->setStyleSheet("color: green; font-weight: bold;");
        m_statusLabel->setText(tr("Schedule created successfully! Click Finish to close."));
    } else {
        m_statusLabel->setStyleSheet("color: red;");
        m_statusLabel->setText(tr("Response: %1").arg(response.left(500)));
    }
}

void BSchedulePreviewPage::onConfigureTimeout()
{
    disconnectDirectorSignals();
    m_progressBar->setVisible(false);
    m_statusLabel->setStyleSheet("color: orange;");
    m_statusLabel->setText(tr("Timeout waiting for Director response. "
                              "The schedule may have been created — check with 'show schedules'."));
    m_executed = true;
}

void BSchedulePreviewPage::disconnectDirectorSignals()
{
    for (const auto &conn : m_connections) {
        QObject::disconnect(conn);
    }
    m_connections.clear();
}

void BSchedulePreviewPage::onCopyConfig()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_configEdit->toPlainText());
    m_statusLabel->setText(tr("Configuration copied to clipboard."));
}

void BSchedulePreviewPage::onCopyCommand()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_commandEdit->toPlainText());
    m_statusLabel->setText(tr("Command copied to clipboard."));
}


// ============================================================================
// BScheduleWizard
// ============================================================================

BScheduleWizard::BScheduleWizard(BDirector *director, QWidget *parent)
    : QWizard(parent)
    , m_director(director)
{
    init();
}

BScheduleWizard::~BScheduleWizard()
{
}

void BScheduleWizard::init()
{
    setWindowTitle(tr("Add Schedule"));
    setMinimumSize(600, 500);

    setPage(Page_Basics, new BScheduleBasicsPage(this));
    setPage(Page_Runs, new BScheduleRunsPage(this));
    setPage(Page_Preview, new BSchedulePreviewPage(this));

    setStartId(Page_Basics);
}

void BScheduleWizard::setReferenceData(const QMap<QString, QStringList> &referenceData)
{
    m_referenceData = referenceData;
}

void BScheduleWizard::applyTemplate(const QStringList &runs)
{
    QList<BScheduleEntry> entries;
    for (const QString &run : runs) {
        entries.append(BScheduleEntry::fromRunDirective(run));
    }

    // Apply to runs page editor
    auto *runsPage = qobject_cast<BScheduleRunsPage*>(page(Page_Runs));
    if (runsPage) {
        // Access the editor through the page's public method
        // We set entries via a direct method call
        auto *editor = runsPage->findChild<BScheduleRunEditor*>();
        if (editor) {
            editor->setEntries(entries);
        }
    }
}

QString BScheduleWizard::generateConfigText() const
{
    auto *basicsPage = qobject_cast<BScheduleBasicsPage*>(page(Page_Basics));
    auto *runsPage = qobject_cast<BScheduleRunsPage*>(page(Page_Runs));
    if (!basicsPage || !runsPage) return QString();

    QString config;
    config += "Schedule {\n";
    config += QString("  Name = \"%1\"\n").arg(basicsPage->scheduleName());

    if (!basicsPage->scheduleDescription().isEmpty()) {
        config += QString("  Description = \"%1\"\n").arg(basicsPage->scheduleDescription());
    }

    if (!basicsPage->enabled()) {
        config += "  Enabled = no\n";
    }

    for (const BScheduleEntry &entry : runsPage->runEntries()) {
        config += QString("  Run = %1\n").arg(entry.toRunDirective());
    }

    config += "}\n";
    return config;
}

QString BScheduleWizard::generateConfigureCommand() const
{
    auto *basicsPage = qobject_cast<BScheduleBasicsPage*>(page(Page_Basics));
    auto *runsPage = qobject_cast<BScheduleRunsPage*>(page(Page_Runs));
    if (!basicsPage || !runsPage) return QString();

    QStringList parts;
    parts.append("add schedule");
    parts.append(QString("name=\"%1\"").arg(basicsPage->scheduleName()));

    if (!basicsPage->scheduleDescription().isEmpty()) {
        parts.append(QString("description=\"%1\"").arg(basicsPage->scheduleDescription()));
    }

    if (!basicsPage->enabled()) {
        parts.append("enabled=no");
    }

    for (const BScheduleEntry &entry : runsPage->runEntries()) {
        parts.append(QString("run=\"%1\"").arg(entry.toRunDirective()));
    }

    return parts.join(" ");
}
