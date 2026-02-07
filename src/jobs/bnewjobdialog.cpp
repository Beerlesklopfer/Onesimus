#include "jobs/bnewjobdialog.h"
#include "jobs/bjobwidget.h"
#include "blogging.h"
#include "bsettings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

BNewJobDialog::BNewJobDialog(BJobWidget *jobWidget, BDirector *director, QWidget *parent)
    : QDialog(parent)
    , m_jobWidget(jobWidget)
    , m_director(director)
    , m_dataLoaded(false)
{
    setWindowTitle(tr("Run New Job"));
    resize(700, 600);
    setModal(true);

    setupUI();

    if (m_jobWidget) {
        loadConfigurationDataFromJobWidget();
    }
}

BNewJobDialog::~BNewJobDialog()
{
    // Disconnect signal to prevent receiving responses after dialog is closed
    if (m_director) {
        disconnect(m_director, &BDirector::jsonResponse,
                   this, &BNewJobDialog::onJsonResponse);
    }
}

void BNewJobDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    // Title
    QLabel *titleLabel = new QLabel("<h2>" + tr("Run Backup Job") + "</h2>");
    mainLayout->addWidget(titleLabel);

    // Tab Widget
    m_tabWidget = new QTabWidget(this);
    createBasicTab();
    createAdvancedTab();
    mainLayout->addWidget(m_tabWidget);

    // Command Preview
    QGroupBox *previewGroup = new QGroupBox(tr("Command Preview"));
    QVBoxLayout *previewLayout = new QVBoxLayout(previewGroup);

    m_commandPreview = new QTextEdit();
    m_commandPreview->setReadOnly(true);
    m_commandPreview->setMaximumHeight(80);
    m_commandPreview->setStyleSheet("background-color: #2b2b2b; color: #00ff00; font-family: monospace;");
    previewLayout->addWidget(m_commandPreview);

    mainLayout->addWidget(previewGroup);

    // Status Label
    m_statusLabel = new QLabel();
    m_statusLabel->setWordWrap(true);
    mainLayout->addWidget(m_statusLabel);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    m_estimateButton = new QPushButton(tr("Estimate"));
    m_estimateButton->setIcon(QIcon::fromTheme("accessories-calculator"));
    m_estimateButton->setToolTip(tr("Calculate estimated files and size"));
    connect(m_estimateButton, &QPushButton::clicked, this, &BNewJobDialog::onEstimateClicked);
    buttonLayout->addWidget(m_estimateButton);

    buttonLayout->addStretch();

    QDialogButtonBox *dialogButtons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    m_runButton = dialogButtons->button(QDialogButtonBox::Ok);
    m_runButton->setText(tr("Run Job"));
    m_runButton->setIcon(QIcon::fromTheme("media-playback-start"));

    connect(dialogButtons, &QDialogButtonBox::accepted, this, &BNewJobDialog::onRunClicked);
    connect(dialogButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    buttonLayout->addWidget(dialogButtons);

    mainLayout->addLayout(buttonLayout);

    // Initially disable buttons until data is loaded
    m_runButton->setEnabled(false);
    m_estimateButton->setEnabled(false);
}

void BNewJobDialog::createBasicTab()
{
    QWidget *basicTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(basicTab);
    layout->setSpacing(15);

    // Job Configuration Group
    QGroupBox *jobGroup = new QGroupBox(tr("Job Configuration"));
    QFormLayout *jobLayout = new QFormLayout(jobGroup);
    jobLayout->setSpacing(10);

    m_jobCombo = new QComboBox();
    m_jobCombo->setPlaceholderText(tr("Select job..."));
    connect(m_jobCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BNewJobDialog::onJobChanged);
    jobLayout->addRow(tr("Job:"), m_jobCombo);

    m_clientCombo = new QComboBox();
    m_clientCombo->setPlaceholderText(tr("Select client..."));
    connect(m_clientCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BNewJobDialog::onClientChanged);
    jobLayout->addRow(tr("Client:"), m_clientCombo);

    m_levelCombo = new QComboBox();
    // Levels are populated in loadConfigurationDataFromJobWidget() based on settings
    connect(m_levelCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BNewJobDialog::onLevelChanged);
    jobLayout->addRow(tr("Level:"), m_levelCombo);

    layout->addWidget(jobGroup);

    // Resources Group
    QGroupBox *resourcesGroup = new QGroupBox(tr("Resources"));
    QFormLayout *resourcesLayout = new QFormLayout(resourcesGroup);
    resourcesLayout->setSpacing(10);

    m_filesetCombo = new QComboBox();
    m_filesetCombo->setPlaceholderText(tr("Select FileSet..."));
    resourcesLayout->addRow(tr("FileSet:"), m_filesetCombo);

    m_poolCombo = new QComboBox();
    m_poolCombo->setPlaceholderText(tr("Select Pool..."));
    resourcesLayout->addRow(tr("Pool:"), m_poolCombo);

    m_storageCombo = new QComboBox();
    m_storageCombo->setPlaceholderText(tr("Select Storage..."));
    resourcesLayout->addRow(tr("Storage:"), m_storageCombo);

    layout->addWidget(resourcesGroup);

    // Priority
    QGroupBox *optionsGroup = new QGroupBox(tr("Options"));
    QFormLayout *optionsLayout = new QFormLayout(optionsGroup);

    m_prioritySpin = new QSpinBox();
    m_prioritySpin->setRange(1, 100);
    m_prioritySpin->setValue(10);
    m_prioritySpin->setToolTip(tr("Lower values = higher priority"));
    optionsLayout->addRow(tr("Priority:"), m_prioritySpin);

    layout->addWidget(optionsGroup);

    layout->addStretch();

    m_tabWidget->addTab(basicTab, tr("Basic"));
}

void BNewJobDialog::createAdvancedTab()
{
    QWidget *advancedTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(advancedTab);
    layout->setSpacing(15);

    // Timing Group
    QGroupBox *timingGroup = new QGroupBox(tr("Scheduling"));
    QFormLayout *timingLayout = new QFormLayout(timingGroup);

    m_whenEdit = new QLineEdit();
    m_whenEdit->setPlaceholderText(tr("e.g. \"2024-01-15 14:30:00\" or empty for immediate"));
    timingLayout->addRow(tr("When:"), m_whenEdit);

    layout->addWidget(timingGroup);

    // Bootstrap Group
    QGroupBox *bootstrapGroup = new QGroupBox(tr("Bootstrap Options"));
    QVBoxLayout *bootstrapLayout = new QVBoxLayout(bootstrapGroup);

    m_bootstrapCheck = new QCheckBox(tr("Use Bootstrap file"));
    bootstrapLayout->addWidget(m_bootstrapCheck);

    m_bootstrapEdit = new QLineEdit();
    m_bootstrapEdit->setEnabled(false);
    m_bootstrapEdit->setPlaceholderText(tr("Path to Bootstrap file"));
    bootstrapLayout->addWidget(m_bootstrapEdit);

    connect(m_bootstrapCheck, &QCheckBox::toggled, m_bootstrapEdit, &QLineEdit::setEnabled);

    layout->addWidget(bootstrapGroup);

    // Replace Options
    QGroupBox *replaceGroup = new QGroupBox(tr("Replace"));
    QVBoxLayout *replaceLayout = new QVBoxLayout(replaceGroup);

    m_replaceCheck = new QCheckBox(tr("Restore mode: Replace existing files"));
    replaceLayout->addWidget(m_replaceCheck);

    m_replaceCombo = new QComboBox();
    m_replaceCombo->addItem("Always", "always");
    m_replaceCombo->addItem("Never", "never");
    m_replaceCombo->addItem("If Newer", "ifnewer");
    m_replaceCombo->addItem("If Older", "ifolder");
    m_replaceCombo->setEnabled(false);
    replaceLayout->addWidget(m_replaceCombo);

    connect(m_replaceCheck, &QCheckBox::toggled, m_replaceCombo, &QComboBox::setEnabled);

    layout->addWidget(replaceGroup);

    layout->addStretch();

    m_tabWidget->addTab(advancedTab, tr("Advanced"));
}

void BNewJobDialog::loadConfigurationDataFromJobWidget()
{
    if (!m_jobWidget) {
        m_statusLabel->setText(tr("⚠ No JobWidget connection available"));
        m_statusLabel->setStyleSheet("color: red;");
        return;
    }

    m_statusLabel->setText(tr("Loading configuration data from JobWidget..."));
    m_statusLabel->setStyleSheet("color: blue;");

    // Get all data directly from JobWidget (already loaded during connection)
    m_jobNames = m_jobWidget->jobNames();
    m_clientNames = m_jobWidget->clientNames();
    m_filesetNames = m_jobWidget->filesetNames();
    m_storageNames = m_jobWidget->storageNames();
    m_poolNames = m_jobWidget->poolNames();

    // Populate combo boxes with data from JobWidget
    m_jobCombo->clear();
    m_jobCombo->addItems(m_jobNames);

    m_clientCombo->clear();
    m_clientCombo->addItems(m_clientNames);

    m_filesetCombo->clear();
    m_filesetCombo->addItems(m_filesetNames);

    m_storageCombo->clear();
    m_storageCombo->addItems(m_storageNames);

    m_poolCombo->clear();
    m_poolCombo->addItems(m_poolNames);

    // Populate level combo based on visible levels from settings
    m_levelCombo->clear();
    QStringList visibleLevels = BSettings::instance().visibleLevels();
    for (const QString &level : visibleLevels) {
        m_levelCombo->addItem(level, level);
    }
    if (m_levelCombo->count() > 0) {
        m_levelCombo->setCurrentIndex(0);
    }

    // Update status based on loaded data
    if (!m_jobNames.isEmpty()) {
        m_statusLabel->setText(tr("✓ %1 jobs, %2 clients loaded")
                               .arg(m_jobNames.size())
                               .arg(m_clientNames.size()));
        m_statusLabel->setStyleSheet("color: green;");
        m_dataLoaded = true;
    } else {
        m_statusLabel->setText(tr("⚠ No jobs available - check connection"));
        m_statusLabel->setStyleSheet("color: orange;");
        m_dataLoaded = false;
    }

    buildRunCommand();
    updateButtonState();  // Enable/disable buttons based on job selection
}

void BNewJobDialog::onJsonResponse(const QString &command, const QString &jsonData)
{
    if (command == ".jobs") {
        onDotJobsReceived(jsonData);
    } else if (command == ".clients") {
        onDotClientsReceived(jsonData);
    } else if (command.startsWith(".defaults")) {
        onDotDefaultsReceived(jsonData);
    }
    // Note: .filesets, .storages, .pools are now loaded from JobWidget
}

void BNewJobDialog::onDotJobsReceived(const QString &jsonData)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BNewJobDialog: Failed to parse .jobs response:" << error.errorString();
        return;
    }

    m_jobNames.clear();
    m_jobCombo->clear();

    if (doc.isObject()) {
        QJsonObject root = doc.object();
        QJsonArray jobsArray = root["result"].toObject()["jobs"].toArray();

        for (const QJsonValue &val : jobsArray) {
            if (val.isObject()) {
                QString name = val.toObject()["name"].toString();
                if (!name.isEmpty()) {
                    m_jobNames.append(name);
                    m_jobCombo->addItem(name);
                }
            }
        }
    }

    BLOG_DEBUG() << "BNewJobDialog: Loaded" << m_jobNames.size() << "jobs";
    buildRunCommand();
}

void BNewJobDialog::onDotClientsReceived(const QString &jsonData)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BNewJobDialog: Failed to parse .clients response:" << error.errorString();
        return;
    }

    m_clientNames.clear();
    m_clientCombo->clear();

    if (doc.isObject()) {
        QJsonObject root = doc.object();
        QJsonArray clientsArray = root["result"].toObject()["clients"].toArray();

        for (const QJsonValue &val : clientsArray) {
            if (val.isObject()) {
                QString name = val.toObject()["name"].toString();
                if (!name.isEmpty()) {
                    m_clientNames.append(name);
                    m_clientCombo->addItem(name);
                }
            }
        }
    }

    BLOG_DEBUG() << "BNewJobDialog: Loaded" << m_clientNames.size() << "clients";

    // Mark data as loaded when all required data is available
    if (!m_jobNames.isEmpty() && !m_clientNames.isEmpty()) {
        m_dataLoaded = true;
        m_statusLabel->setText(tr("✓ Configuration loaded"));
        m_statusLabel->setStyleSheet("color: green;");
    }

    buildRunCommand();
    updateButtonState();  // Enable/disable buttons based on job selection
}

void BNewJobDialog::onJobChanged(int index)
{
    Q_UNUSED(index);
    updateJobDefaults();
    buildRunCommand();
    updateButtonState();
}

void BNewJobDialog::onClientChanged(int index)
{
    Q_UNUSED(index);
    buildRunCommand();
}

void BNewJobDialog::onLevelChanged(int index)
{
    Q_UNUSED(index);
    buildRunCommand();
}

void BNewJobDialog::updateJobDefaults()
{
    QString jobName = m_jobCombo->currentText();
    if (jobName.isEmpty()) {
        buildRunCommand();
        return;
    }

    // Request job defaults from Director using .defaults command
    if (m_director) {
        BLOG_DEBUG() << "BNewJobDialog: Requesting defaults for job:" << jobName;
        m_statusLabel->setText(tr("Loading job defaults..."));
        m_statusLabel->setStyleSheet("color: blue;");

        // Connect to receive the response
        connect(m_director, &BDirector::jsonResponse,
                this, &BNewJobDialog::onJsonResponse,
                Qt::UniqueConnection);

        // Send .defaults job=<name> command
        QMetaObject::invokeMethod(m_director, "doSendCommand",
                                  Qt::QueuedConnection,
                                  Q_ARG(BDirector::Command, BDirector::Command::DotDefaults),
                                  Q_ARG(QString, jobName));
    }

    buildRunCommand();
}

void BNewJobDialog::onDotDefaultsReceived(const QString &jsonData)
{
    BLOG_DEBUG() << "BNewJobDialog: Processing .defaults response";
    BLOG_DEBUG() << "  Data:" << jsonData.left(500);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BNewJobDialog: Failed to parse .defaults response:" << error.errorString();
        m_statusLabel->setText(tr("Failed to load job defaults"));
        m_statusLabel->setStyleSheet("color: orange;");
        return;
    }

    if (!doc.isObject()) {
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    // defaults can be an object or an array depending on Bareos version
    QJsonObject defaults;
    if (result["defaults"].isObject()) {
        defaults = result["defaults"].toObject();
    } else if (result["defaults"].isArray()) {
        QJsonArray defaultsArray = result["defaults"].toArray();
        if (!defaultsArray.isEmpty()) {
            defaults = defaultsArray.first().toObject();
        }
    }

    if (defaults.isEmpty()) {
        BLOG_DEBUG() << "BNewJobDialog: No defaults in response";
        return;
    }

    BLOG_DEBUG() << "BNewJobDialog: Defaults:" << defaults;

    // Pre-select FileSet from defaults
    QString fileset = defaults["fileset"].toString();
    if (!fileset.isEmpty()) {
        int index = m_filesetCombo->findText(fileset);
        BLOG_DEBUG() << "  FileSet:" << fileset << "index:" << index << "count:" << m_filesetCombo->count();
        if (index >= 0) {
            m_filesetCombo->setCurrentIndex(index);
        } else {
            BLOG_DEBUG() << "    Available filesets:";
            for (int i = 0; i < m_filesetCombo->count(); ++i) {
                BLOG_DEBUG() << "      " << i << ":" << m_filesetCombo->itemText(i);
            }
        }
    }

    // Pre-select Pool from defaults
    QString pool = defaults["pool"].toString();
    if (!pool.isEmpty()) {
        int index = m_poolCombo->findText(pool);
        BLOG_DEBUG() << "  Pool:" << pool << "index:" << index << "count:" << m_poolCombo->count();
        if (index >= 0) {
            m_poolCombo->setCurrentIndex(index);
        } else {
            BLOG_DEBUG() << "    Available pools:";
            for (int i = 0; i < m_poolCombo->count(); ++i) {
                BLOG_DEBUG() << "      " << i << ":" << m_poolCombo->itemText(i);
            }
        }
    }

    // Pre-select Storage from defaults
    QString storage = defaults["storage"].toString();
    if (!storage.isEmpty()) {
        int index = m_storageCombo->findText(storage);
        BLOG_DEBUG() << "  Storage:" << storage << "index:" << index << "count:" << m_storageCombo->count();
        if (index >= 0) {
            m_storageCombo->setCurrentIndex(index);
        } else {
            BLOG_DEBUG() << "    Available storages:";
            for (int i = 0; i < m_storageCombo->count(); ++i) {
                BLOG_DEBUG() << "      " << i << ":" << m_storageCombo->itemText(i);
            }
        }
    }

    // Pre-select Client from defaults
    QString client = defaults["client"].toString();
    if (!client.isEmpty()) {
        int index = m_clientCombo->findText(client);
        BLOG_DEBUG() << "  Client:" << client << "index:" << index << "count:" << m_clientCombo->count();
        if (index >= 0) {
            m_clientCombo->setCurrentIndex(index);
        } else {
            BLOG_DEBUG() << "    Available clients:";
            for (int i = 0; i < m_clientCombo->count(); ++i) {
                BLOG_DEBUG() << "      " << i << ":" << m_clientCombo->itemText(i);
            }
        }
    }

    // Pre-select Level from defaults (if available)
    QString level = defaults["level"].toString();
    if (!level.isEmpty()) {
        // Try to find by text first (e.g., "Incremental", "Full")
        int index = m_levelCombo->findText(level);
        BLOG_DEBUG() << "  Level:" << level << "findText index:" << index;
        if (index < 0) {
            // Try to find by data (level code like "F", "I", "D")
            index = m_levelCombo->findData(level);
            BLOG_DEBUG() << "  Level:" << level << "findData index:" << index;
        }
        if (index >= 0) {
            m_levelCombo->setCurrentIndex(index);
        } else {
            BLOG_DEBUG() << "    Available levels:";
            for (int i = 0; i < m_levelCombo->count(); ++i) {
                BLOG_DEBUG() << "      " << i << ": text=" << m_levelCombo->itemText(i) << "data=" << m_levelCombo->itemData(i);
            }
        }
    }

    m_statusLabel->setText(tr("✓ Job defaults loaded"));
    m_statusLabel->setStyleSheet("color: green;");

    buildRunCommand();
}

void BNewJobDialog::buildRunCommand()
{
    QString command = "run";

    if (!m_jobCombo->currentText().isEmpty()) {
        command += " job=\"" + m_jobCombo->currentText() + "\"";
    }

    if (!m_clientCombo->currentText().isEmpty()) {
        command += " client=\"" + m_clientCombo->currentText() + "\"";
    }

    if (!m_filesetCombo->currentText().isEmpty()) {
        command += " fileset=\"" + m_filesetCombo->currentText() + "\"";
    }

    if (!m_poolCombo->currentText().isEmpty()) {
        command += " pool=\"" + m_poolCombo->currentText() + "\"";
    }

    if (!m_storageCombo->currentText().isEmpty()) {
        command += " storage=\"" + m_storageCombo->currentText() + "\"";
    }

    command += " level=" + m_levelCombo->currentData().toString();

    if (m_prioritySpin->value() != 10) {
        command += " priority=" + QString::number(m_prioritySpin->value());
    }

    if (!m_whenEdit->text().isEmpty()) {
        command += " when=\"" + m_whenEdit->text() + "\"";
    }

    if (m_bootstrapCheck->isChecked() && !m_bootstrapEdit->text().isEmpty()) {
        command += " bootstrap=\"" + m_bootstrapEdit->text() + "\"";
    }

    command += " yes";  // Auto-confirm

    m_commandPreview->setPlainText(command);
}

void BNewJobDialog::updateButtonState()
{
    // Minimum requirement: a job must be selected
    bool canRun = !m_jobCombo->currentText().isEmpty() && m_dataLoaded;

    m_runButton->setEnabled(canRun);
    m_estimateButton->setEnabled(canRun);
}

QString BNewJobDialog::getJobCommand() const
{
    return m_commandPreview->toPlainText();
}

void BNewJobDialog::onRunClicked()
{
    if (m_jobCombo->currentText().isEmpty()) {
        QMessageBox::warning(this, tr("Missing Input"),
            tr("Please select a job."));
        return;
    }

    QString command = getJobCommand();

    // Check if confirmation is required
    bool shouldConfirm = BSettings::instance().behaviorConfirmJobStart();

    bool proceed = true;
    if (shouldConfirm) {
        int ret = QMessageBox::question(this, tr("Run Job"),
            tr("Do you want to run the following job?\n\n%1").arg(command),
            QMessageBox::Yes | QMessageBox::No);
        proceed = (ret == QMessageBox::Yes);
    }

    if (proceed) {
        if (m_director) {
            // Use Custom command since we build the full "run ..." command ourselves
            QMetaObject::invokeMethod(m_director, "doSendCommand",
                                      Qt::QueuedConnection,
                                      Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                      Q_ARG(QString, command));
        }
        accept();
    }
}

void BNewJobDialog::onEstimateClicked()
{
    if (m_jobCombo->currentText().isEmpty()) {
        QMessageBox::warning(this, tr("Missing Input"),
            tr("Please select a job."));
        return;
    }

    QString command = "estimate";
    command += " job=\"" + m_jobCombo->currentText() + "\"";

    if (!m_clientCombo->currentText().isEmpty()) {
        command += " client=\"" + m_clientCombo->currentText() + "\"";
    }

    if (!m_filesetCombo->currentText().isEmpty()) {
        command += " fileset=\"" + m_filesetCombo->currentText() + "\"";
    }

    command += " level=" + m_levelCombo->currentData().toString();

    if (m_director) {
        QMetaObject::invokeMethod(m_director, "doSendCommand",
                                  Qt::QueuedConnection,
                                  Q_ARG(BDirector::Command, BDirector::Command::Custom),
                                  Q_ARG(QString, command));

        m_statusLabel->setText(tr("Calculating estimate..."));
        m_statusLabel->setStyleSheet("color: blue;");
    }
}

void BNewJobDialog::onRefreshData()
{
    loadConfigurationDataFromJobWidget();
}
