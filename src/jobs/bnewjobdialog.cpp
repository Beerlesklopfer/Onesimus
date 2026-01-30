#include "jobs/bnewjobdialog.h"
#include "jobs/bjobwidget.h"
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
#include <QDebug>

BNewJobDialog::BNewJobDialog(BJobWidget *jobWidget, BDirector *director, QWidget *parent)
    : QDialog(parent)
    , m_jobWidget(jobWidget)
    , m_director(director)
    , m_dataLoaded(false)
{
    setWindowTitle("Neuen Job ausführen");
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
    QLabel *titleLabel = new QLabel("<h2>Backup Job ausführen</h2>");
    mainLayout->addWidget(titleLabel);

    // Tab Widget
    m_tabWidget = new QTabWidget(this);
    createBasicTab();
    createAdvancedTab();
    mainLayout->addWidget(m_tabWidget);

    // Command Preview
    QGroupBox *previewGroup = new QGroupBox("Befehlsvorschau");
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

    m_estimateButton = new QPushButton("Schätzung");
    m_estimateButton->setIcon(QIcon::fromTheme("accessories-calculator"));
    m_estimateButton->setToolTip("Geschätzte Dateien und Größe berechnen");
    connect(m_estimateButton, &QPushButton::clicked, this, &BNewJobDialog::onEstimateClicked);
    buttonLayout->addWidget(m_estimateButton);

    buttonLayout->addStretch();

    QDialogButtonBox *dialogButtons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    m_runButton = dialogButtons->button(QDialogButtonBox::Ok);
    m_runButton->setText("Job ausführen");
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
    QGroupBox *jobGroup = new QGroupBox("Job Konfiguration");
    QFormLayout *jobLayout = new QFormLayout(jobGroup);
    jobLayout->setSpacing(10);

    m_jobCombo = new QComboBox();
    m_jobCombo->setPlaceholderText("Job auswählen...");
    connect(m_jobCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BNewJobDialog::onJobChanged);
    jobLayout->addRow("Job:", m_jobCombo);

    m_clientCombo = new QComboBox();
    m_clientCombo->setPlaceholderText("Client auswählen...");
    connect(m_clientCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BNewJobDialog::onClientChanged);
    jobLayout->addRow("Client:", m_clientCombo);

    m_levelCombo = new QComboBox();
    // Levels are populated in loadConfigurationDataFromJobWidget() based on settings
    connect(m_levelCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BNewJobDialog::onLevelChanged);
    jobLayout->addRow("Level:", m_levelCombo);

    layout->addWidget(jobGroup);

    // Resources Group
    QGroupBox *resourcesGroup = new QGroupBox("Ressourcen");
    QFormLayout *resourcesLayout = new QFormLayout(resourcesGroup);
    resourcesLayout->setSpacing(10);

    m_filesetCombo = new QComboBox();
    m_filesetCombo->setPlaceholderText("FileSet auswählen...");
    resourcesLayout->addRow("FileSet:", m_filesetCombo);

    m_poolCombo = new QComboBox();
    m_poolCombo->setPlaceholderText("Pool auswählen...");
    resourcesLayout->addRow("Pool:", m_poolCombo);

    m_storageCombo = new QComboBox();
    m_storageCombo->setPlaceholderText("Storage auswählen...");
    resourcesLayout->addRow("Storage:", m_storageCombo);

    layout->addWidget(resourcesGroup);

    // Priority
    QGroupBox *optionsGroup = new QGroupBox("Optionen");
    QFormLayout *optionsLayout = new QFormLayout(optionsGroup);

    m_prioritySpin = new QSpinBox();
    m_prioritySpin->setRange(1, 100);
    m_prioritySpin->setValue(10);
    m_prioritySpin->setToolTip("Niedrigere Werte = höhere Priorität");
    optionsLayout->addRow("Priorität:", m_prioritySpin);

    layout->addWidget(optionsGroup);

    layout->addStretch();

    m_tabWidget->addTab(basicTab, "Basis");
}

void BNewJobDialog::createAdvancedTab()
{
    QWidget *advancedTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(advancedTab);
    layout->setSpacing(15);

    // Timing Group
    QGroupBox *timingGroup = new QGroupBox("Zeitplanung");
    QFormLayout *timingLayout = new QFormLayout(timingGroup);

    m_whenEdit = new QLineEdit();
    m_whenEdit->setPlaceholderText("z.B. \"2024-01-15 14:30:00\" oder leer für sofort");
    timingLayout->addRow("Wann:", m_whenEdit);

    layout->addWidget(timingGroup);

    // Bootstrap Group
    QGroupBox *bootstrapGroup = new QGroupBox("Bootstrap Optionen");
    QVBoxLayout *bootstrapLayout = new QVBoxLayout(bootstrapGroup);

    m_bootstrapCheck = new QCheckBox("Bootstrap Datei verwenden");
    bootstrapLayout->addWidget(m_bootstrapCheck);

    m_bootstrapEdit = new QLineEdit();
    m_bootstrapEdit->setEnabled(false);
    m_bootstrapEdit->setPlaceholderText("Pfad zur Bootstrap Datei");
    bootstrapLayout->addWidget(m_bootstrapEdit);

    connect(m_bootstrapCheck, &QCheckBox::toggled, m_bootstrapEdit, &QLineEdit::setEnabled);

    layout->addWidget(bootstrapGroup);

    // Replace Options
    QGroupBox *replaceGroup = new QGroupBox("Ersetzen");
    QVBoxLayout *replaceLayout = new QVBoxLayout(replaceGroup);

    m_replaceCheck = new QCheckBox("Restore-Modus: Vorhandene Dateien ersetzen");
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

    m_tabWidget->addTab(advancedTab, "Erweitert");
}

void BNewJobDialog::loadConfigurationDataFromJobWidget()
{
    if (!m_jobWidget) {
        m_statusLabel->setText("⚠ Keine JobWidget-Verbindung verfügbar");
        m_statusLabel->setStyleSheet("color: red;");
        return;
    }

    m_statusLabel->setText("Lade Konfigurationsdaten vom JobWidget...");
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
        m_statusLabel->setText(QString("✓ %1 Jobs, %2 Clients geladen")
                               .arg(m_jobNames.size())
                               .arg(m_clientNames.size()));
        m_statusLabel->setStyleSheet("color: green;");
        m_dataLoaded = true;
    } else {
        m_statusLabel->setText("⚠ Keine Jobs verfügbar - Verbindung prüfen");
        m_statusLabel->setStyleSheet("color: orange;");
    }

    buildRunCommand();
}

void BNewJobDialog::onJsonResponse(const QString &command, const QString &jsonData)
{
    if (command == ".jobs") {
        onDotJobsReceived(jsonData);
    } else if (command == ".clients") {
        onDotClientsReceived(jsonData);
    }
    // Note: .filesets, .storages, .pools are now loaded from JobWidget
}

void BNewJobDialog::onDotJobsReceived(const QString &jsonData)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "BNewJobDialog: Failed to parse .jobs response:" << error.errorString();
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

    qDebug() << "BNewJobDialog: Loaded" << m_jobNames.size() << "jobs";
    buildRunCommand();
}

void BNewJobDialog::onDotClientsReceived(const QString &jsonData)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "BNewJobDialog: Failed to parse .clients response:" << error.errorString();
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

    qDebug() << "BNewJobDialog: Loaded" << m_clientNames.size() << "clients";

    // Enable buttons when all data is loaded (filesets, storages, pools come from JobWidget)
    if (!m_jobNames.isEmpty() && !m_clientNames.isEmpty()) {
        m_dataLoaded = true;
        m_runButton->setEnabled(true);
        m_estimateButton->setEnabled(true);
        m_statusLabel->setText("✓ Konfiguration geladen");
        m_statusLabel->setStyleSheet("color: green;");
    }

    buildRunCommand();
}

void BNewJobDialog::onJobChanged(int index)
{
    Q_UNUSED(index);
    updateJobDefaults();
    buildRunCommand();
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
    // In a real implementation, we would query job defaults from director
    // For now, just rebuild the command
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

QString BNewJobDialog::getJobCommand() const
{
    return m_commandPreview->toPlainText();
}

void BNewJobDialog::onRunClicked()
{
    if (m_jobCombo->currentText().isEmpty()) {
        QMessageBox::warning(this, "Fehlende Angabe",
            "Bitte wählen Sie einen Job aus.");
        return;
    }

    QString command = getJobCommand();

    // Check if confirmation is required
    bool shouldConfirm = BSettings::instance().behaviorConfirmJobStart();

    bool proceed = true;
    if (shouldConfirm) {
        int ret = QMessageBox::question(this, "Job ausführen",
            QString("Möchten Sie folgenden Job ausführen?\n\n%1").arg(command),
            QMessageBox::Yes | QMessageBox::No);
        proceed = (ret == QMessageBox::Yes);
    }

    if (proceed) {
        if (m_director) {
            QMetaObject::invokeMethod(m_director, "doSendCommand",
                                      Qt::QueuedConnection,
                                      Q_ARG(BDirector::Command, BDirector::Command::Run),
                                      Q_ARG(QString, command));
        }
        accept();
    }
}

void BNewJobDialog::onEstimateClicked()
{
    if (m_jobCombo->currentText().isEmpty()) {
        QMessageBox::warning(this, "Fehlende Angabe",
            "Bitte wählen Sie einen Job aus.");
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

        m_statusLabel->setText("Schätzung wird berechnet...");
        m_statusLabel->setStyleSheet("color: blue;");
    }
}

void BNewJobDialog::onRefreshData()
{
    loadConfigurationDataFromJobWidget();
}
