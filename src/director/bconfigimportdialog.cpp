/**
 * @file bconfigimportdialog.cpp
 * @brief Implementation of tabbed config import dialog
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 */

#include "director/bconfigimportdialog.h"
#include "config/bdirectiveschema.h"
#include <QApplication>
#include <QClipboard>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QSettings>

namespace {
    const QString SETTINGS_KEY_LAST_IMPORT_DIR = QStringLiteral("configImport/lastDirectory");
}

BConfigImportDialog::BConfigImportDialog(QWidget *parent)
    : QDialog(parent)
    , m_parser(new BConfigParser(this))
    , m_tempDir(nullptr)
{
    setWindowTitle(tr("Import Director Configuration"));
    setMinimumSize(900, 700);

    // Load directive schemas
    BDirectiveSchema::instance().loadSchemas();

    setupUI();

    connect(m_parser, &BConfigParser::resourceParsed,
            this, &BConfigImportDialog::onResourceParsed);
    connect(m_parser, &BConfigParser::parsingComplete,
            this, &BConfigImportDialog::onParsingComplete);
    connect(m_parser, &BConfigParser::parsingError,
            this, &BConfigImportDialog::onParsingError);
}

BConfigImportDialog::~BConfigImportDialog()
{
    cleanupTempDir();
}

void BConfigImportDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Source selection group
    QGroupBox *sourceGroup = new QGroupBox(tr("Configuration Source"), this);
    QHBoxLayout *sourceLayout = new QHBoxLayout(sourceGroup);

    m_pathEdit = new QLineEdit(this);
    m_pathEdit->setPlaceholderText(tr("Select directory or ZIP file..."));
    m_pathEdit->setReadOnly(true);
    sourceLayout->addWidget(m_pathEdit, 1);

    m_browseDirectoryButton = new QPushButton(tr("Directory..."), this);
    connect(m_browseDirectoryButton, &QPushButton::clicked,
            this, &BConfigImportDialog::onBrowseDirectory);
    sourceLayout->addWidget(m_browseDirectoryButton);

    m_browseZipButton = new QPushButton(tr("ZIP File..."), this);
    connect(m_browseZipButton, &QPushButton::clicked,
            this, &BConfigImportDialog::onBrowseZipFile);
    sourceLayout->addWidget(m_browseZipButton);

    m_parseButton = new QPushButton(tr("Parse"), this);
    m_parseButton->setEnabled(false);
    connect(m_parseButton, &QPushButton::clicked,
            this, &BConfigImportDialog::onParse);
    sourceLayout->addWidget(m_parseButton);

    mainLayout->addWidget(sourceGroup);

    // Progress bar
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    mainLayout->addWidget(m_progressBar);

    // Status label
    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("color: gray; font-style: italic;");
    mainLayout->addWidget(m_statusLabel);

    // Tab widget
    setupTabs();
    mainLayout->addWidget(m_tabWidget, 1);

    // Preview group (connection settings are now in Director tab)
    m_previewGroup = new QGroupBox(tr("Connection Preview"), this);
    m_previewGroup->setEnabled(false);
    QGridLayout *previewLayout = new QGridLayout(m_previewGroup);

    previewLayout->addWidget(new QLabel(tr("Director:")), 0, 0);
    m_previewDirectorLabel = new QLabel("-", this);
    m_previewDirectorLabel->setStyleSheet("font-weight: bold;");
    previewLayout->addWidget(m_previewDirectorLabel, 0, 1);

    previewLayout->addWidget(new QLabel(tr("Address:")), 1, 0);
    m_previewAddressLabel = new QLabel("-", this);
    previewLayout->addWidget(m_previewAddressLabel, 1, 1);

    previewLayout->addWidget(new QLabel(tr("Port:")), 2, 0);
    m_previewPortLabel = new QLabel("-", this);
    previewLayout->addWidget(m_previewPortLabel, 2, 1);

    previewLayout->addWidget(new QLabel(tr("Console:")), 3, 0);
    m_previewConsoleLabel = new QLabel("-", this);
    previewLayout->addWidget(m_previewConsoleLabel, 3, 1);

    previewLayout->addWidget(new QLabel(tr("TLS:")), 4, 0);
    m_previewTlsLabel = new QLabel("-", this);
    previewLayout->addWidget(m_previewTlsLabel, 4, 1);

    mainLayout->addWidget(m_previewGroup);

    // Validation feedback label
    m_validationLabel = new QLabel(this);
    m_validationLabel->setWordWrap(true);
    m_validationLabel->setVisible(false);
    mainLayout->addWidget(m_validationLabel);

    // Button box
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_importButton = new QPushButton(tr("Import"), this);
    m_importButton->setEnabled(false);
    m_importButton->setDefault(true);
    connect(m_importButton, &QPushButton::clicked,
            this, &BConfigImportDialog::onAccept);
    buttonLayout->addWidget(m_importButton);

    m_cancelButton = new QPushButton(tr("Cancel"), this);
    connect(m_cancelButton, &QPushButton::clicked,
            this, &QDialog::reject);
    buttonLayout->addWidget(m_cancelButton);

    mainLayout->addLayout(buttonLayout);
}

void BConfigImportDialog::setupTabs()
{
    m_tabWidget = new QTabWidget(this);

    // Director tab with connection settings
    QWidget *directorTab = new QWidget(this);
    QVBoxLayout *directorTabLayout = new QVBoxLayout(directorTab);
    directorTabLayout->setContentsMargins(0, 0, 0, 0);

    m_directorWidget = new BDirectorResourceWidget(nullptr, this);
    connect(m_directorWidget, &BResourceWidget::resourceSelected,
            this, &BConfigImportDialog::onDirectorSelected);
    directorTabLayout->addWidget(m_directorWidget, 1);

    // Connection settings group (inside Director tab)
    m_connectionGroup = new QGroupBox(tr("Connection Settings"), this);
    QGridLayout *connLayout = new QGridLayout(m_connectionGroup);

    connLayout->addWidget(new QLabel(tr("Director:")), 0, 0);
    m_directorCombo = new QComboBox(this);
    connect(m_directorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) {
                int idx = m_directorCombo->currentData().toInt();
                if (idx >= 0) {
                    QList<BConfigResource> directors = m_parser->resourcesByType("Director");
                    if (idx < directors.size()) {
                        onDirectorSelected(directors.at(idx));
                    }
                }
            });
    connLayout->addWidget(m_directorCombo, 0, 1);

    connLayout->addWidget(new QLabel(tr("Console:")), 1, 0);
    m_consoleCombo = new QComboBox(this);
    connect(m_consoleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) {
                int idx = m_consoleCombo->currentData().toInt();
                if (idx >= 0) {
                    QList<BConfigResource> consoles = m_parser->resourcesByType("Console");
                    if (idx < consoles.size()) {
                        onConsoleSelected(consoles.at(idx));
                    }
                }
            });
    connLayout->addWidget(m_consoleCombo, 1, 1);

    // Address and port input (for remote connection)
    connLayout->addWidget(new QLabel(tr("Address:")), 2, 0);
    m_addressEdit = new QLineEdit(this);
    m_addressEdit->setPlaceholderText(tr("hostname or IP address"));
    m_addressEdit->setToolTip(tr("Enter the hostname or IP address of the Director to connect to"));
    connect(m_addressEdit, &QLineEdit::textChanged, this, &BConfigImportDialog::validateSelection);
    connLayout->addWidget(m_addressEdit, 2, 1);

    connLayout->addWidget(new QLabel(tr("Port:")), 3, 0);
    m_portSpinBox = new QSpinBox(this);
    m_portSpinBox->setRange(1, 65535);
    m_portSpinBox->setValue(9101);
    m_portSpinBox->setToolTip(tr("Port number (default: 9101)"));
    connLayout->addWidget(m_portSpinBox, 3, 1);

    m_createProfileCheck = new QCheckBox(tr("Create connection profile"), this);
    m_createProfileCheck->setChecked(true);
    connLayout->addWidget(m_createProfileCheck, 4, 0, 1, 2);

    // DEBUG: Copy button to copy parsed resource info to clipboard
    QPushButton *copyDebugButton = new QPushButton(tr("Copy Debug Info"), this);
    copyDebugButton->setToolTip(tr("Copy parsed resource details to clipboard for debugging"));
    connect(copyDebugButton, &QPushButton::clicked, this, [this]() {
        QString debugInfo;
        debugInfo += "=== PARSED RESOURCES DEBUG INFO ===\n\n";

        // Directors
        QList<BConfigResource> directors = m_parser->resourcesByType("Director");
        debugInfo += QString("Directors: %1\n").arg(directors.size());
        for (const BConfigResource &dir : directors) {
            debugInfo += QString("  - %1\n").arg(dir.name());
            debugInfo += QString("    Keys: %1\n").arg(dir.keys().join(", "));
            for (const QString &key : dir.keys()) {
                debugInfo += QString("    %1 = %2\n").arg(key, dir.simpleValue(key, "(complex)"));
            }
        }

        // Consoles
        QList<BConfigResource> consoles = m_parser->resourcesByType("Console");
        debugInfo += QString("\nConsoles: %1\n").arg(consoles.size());
        for (const BConfigResource &con : consoles) {
            debugInfo += QString("  - %1\n").arg(con.name());
            debugInfo += QString("    Keys: %1\n").arg(con.keys().join(", "));
            for (const QString &key : con.keys()) {
                QString val = con.simpleValue(key, "(complex)");
                // Mask password partially
                if (key.toLower() == "password" && val.length() > 10) {
                    val = val.left(10) + "..." + QString(" (len=%1)").arg(val.length());
                }
                debugInfo += QString("    %1 = %2\n").arg(key, val);
            }
        }

        QApplication::clipboard()->setText(debugInfo);
        QMessageBox::information(this, tr("Debug Info Copied"),
            tr("Debug information has been copied to clipboard.\n\nPaste it somewhere to view."));
    });
    connLayout->addWidget(copyDebugButton, 5, 0, 1, 2);

    directorTabLayout->addWidget(m_connectionGroup);
    m_tabWidget->addTab(directorTab, tr("Director"));

    m_consoleWidget = new BConsoleResourceWidget(nullptr, this);
    connect(m_consoleWidget, &BResourceWidget::resourceSelected,
            this, &BConfigImportDialog::onConsoleSelected);
    m_tabWidget->addTab(m_consoleWidget, tr("Console"));

    m_clientWidget = new BClientResourceWidget(nullptr, this);
    m_tabWidget->addTab(m_clientWidget, tr("Clients"));

    m_jobWidget = new BJobResourceWidget("Job", nullptr, this);
    m_tabWidget->addTab(m_jobWidget, tr("Jobs"));

    m_storageWidget = new BStorageResourceWidget(nullptr, this);
    m_tabWidget->addTab(m_storageWidget, tr("Storage"));

    m_fileSetWidget = new BFileSetResourceWidget(nullptr, this);
    m_tabWidget->addTab(m_fileSetWidget, tr("FileSets"));

    m_poolWidget = new BPoolResourceWidget(nullptr, this);
    m_tabWidget->addTab(m_poolWidget, tr("Pools"));

    m_scheduleWidget = new BScheduleResourceWidget(nullptr, this);
    m_tabWidget->addTab(m_scheduleWidget, tr("Schedules"));

    m_messagesWidget = new BMessagesResourceWidget(nullptr, this);
    m_tabWidget->addTab(m_messagesWidget, tr("Messages"));

    m_catalogWidget = new BCatalogResourceWidget(nullptr, this);
    m_tabWidget->addTab(m_catalogWidget, tr("Catalogs"));
}

void BConfigImportDialog::onBrowseDirectory()
{
    // Remember last used directory
    QSettings settings;
    QString lastDir = settings.value(SETTINGS_KEY_LAST_IMPORT_DIR, QDir::homePath()).toString();

    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Select Director Configuration Directory"),
        lastDir,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        m_pathEdit->setText(dir);
        m_parseButton->setEnabled(true);
        m_statusLabel->setText(tr("Ready to parse directory"));
        // Save selected directory for next time
        settings.setValue(SETTINGS_KEY_LAST_IMPORT_DIR, dir);
    }
}

void BConfigImportDialog::onBrowseZipFile()
{
    // Remember last used directory
    QSettings settings;
    QString lastDir = settings.value(SETTINGS_KEY_LAST_IMPORT_DIR, QDir::homePath()).toString();

    QString file = QFileDialog::getOpenFileName(
        this,
        tr("Select Configuration ZIP Archive"),
        lastDir,
        tr("ZIP Archives (*.zip);;All Files (*)"));

    if (!file.isEmpty()) {
        m_pathEdit->setText(file);
        m_parseButton->setEnabled(true);
        m_statusLabel->setText(tr("Ready to parse ZIP archive"));
        // Save parent directory for next time
        QFileInfo fileInfo(file);
        settings.setValue(SETTINGS_KEY_LAST_IMPORT_DIR, fileInfo.absolutePath());
    }
}

void BConfigImportDialog::onParse()
{
    QString path = m_pathEdit->text();
    if (path.isEmpty()) return;

    m_parser->clear();
    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 0); // Indeterminate
    m_statusLabel->setText(tr("Parsing..."));
    m_parseButton->setEnabled(false);

    QFileInfo fi(path);

    if (fi.isDir()) {
        // Parse directory directly
        m_parser->parseDirectory(path);
    } else if (fi.suffix().toLower() == "zip") {
        // Extract ZIP to temp directory first
        cleanupTempDir();
        m_tempDir = new QTemporaryDir();

        if (!m_tempDir->isValid()) {
            onParsingError(tr("Failed to create temporary directory"));
            return;
        }

        m_statusLabel->setText(tr("Extracting ZIP archive..."));

        // Use unzip command
        QProcess unzip;
        unzip.setWorkingDirectory(m_tempDir->path());
        unzip.start("unzip", {"-q", path});

        if (!unzip.waitForFinished(60000)) {
            onParsingError(tr("Failed to extract ZIP archive (timeout)"));
            return;
        }

        if (unzip.exitCode() != 0) {
            onParsingError(tr("Failed to extract ZIP archive: %1")
                               .arg(QString::fromUtf8(unzip.readAllStandardError())));
            return;
        }

        // Find the bareos-dir.d directory
        QDirIterator it(m_tempDir->path(), QStringList() << "bareos-dir.d" << "bacula-dir.d",
                        QDir::Dirs, QDirIterator::Subdirectories);

        QString configDir;
        while (it.hasNext()) {
            configDir = it.next();
            break;
        }

        if (configDir.isEmpty()) {
            // Try to find any .conf files
            configDir = m_tempDir->path();
        }

        m_statusLabel->setText(tr("Parsing extracted files..."));
        m_parser->parseDirectory(configDir);
    } else {
        // Try to parse as single file
        m_parser->parseFile(path);
    }
}

void BConfigImportDialog::onResourceParsed(const QString &type, const QString &name)
{
    m_statusLabel->setText(tr("Parsed %1: %2").arg(type, name));
}

void BConfigImportDialog::onParsingComplete(int resourceCount)
{
    m_progressBar->setVisible(false);
    m_parseButton->setEnabled(true);
    m_statusLabel->setText(tr("Parsing complete: %1 resources found").arg(resourceCount));

    populateTabs();

    // Enable connection selection if we have Directors and Consoles
    QList<BConfigResource> directors = m_parser->resourcesByType("Director");
    QList<BConfigResource> consoles = m_parser->resourcesByType("Console");

    m_connectionGroup->setEnabled(!directors.isEmpty() && !consoles.isEmpty());
    m_previewGroup->setEnabled(!directors.isEmpty() && !consoles.isEmpty());

    // Populate combos
    m_directorCombo->clear();
    for (int i = 0; i < directors.size(); ++i) {
        m_directorCombo->addItem(directors.at(i).name(), i);
    }

    m_consoleCombo->clear();
    for (int i = 0; i < consoles.size(); ++i) {
        m_consoleCombo->addItem(consoles.at(i).name(), i);
    }

    validateSelection();
    updateValidationDisplay();
}

void BConfigImportDialog::onParsingError(const QString &error)
{
    m_progressBar->setVisible(false);
    m_parseButton->setEnabled(true);
    m_statusLabel->setText(tr("Error: %1").arg(error));
    m_statusLabel->setStyleSheet("color: red;");

    QMessageBox::warning(this, tr("Parsing Error"), error);
}

void BConfigImportDialog::populateTabs()
{
    m_directorWidget->setResources(m_parser->resourcesByType("Director"));
    m_consoleWidget->setResources(m_parser->resourcesByType("Console"));
    m_clientWidget->setResources(m_parser->resourcesByType("Client"));
    m_jobWidget->setResources(m_parser->resourcesByType("Job"));
    m_storageWidget->setResources(m_parser->resourcesByType("Storage"));
    m_fileSetWidget->setResources(m_parser->resourcesByType("FileSet"));
    m_poolWidget->setResources(m_parser->resourcesByType("Pool"));
    m_scheduleWidget->setResources(m_parser->resourcesByType("Schedule"));
    m_messagesWidget->setResources(m_parser->resourcesByType("Messages"));
    m_catalogWidget->setResources(m_parser->resourcesByType("Catalog"));

    // Update tab labels with counts
    auto updateTabLabel = [this](int index, const QString &name, int count) {
        m_tabWidget->setTabText(index, QString("%1 (%2)").arg(name).arg(count));
    };

    updateTabLabel(0, tr("Director"), m_directorWidget->resourceCount());
    updateTabLabel(1, tr("Console"), m_consoleWidget->resourceCount());
    updateTabLabel(2, tr("Clients"), m_clientWidget->resourceCount());
    updateTabLabel(3, tr("Jobs"), m_jobWidget->resourceCount());
    updateTabLabel(4, tr("Storage"), m_storageWidget->resourceCount());
    updateTabLabel(5, tr("FileSets"), m_fileSetWidget->resourceCount());
    updateTabLabel(6, tr("Pools"), m_poolWidget->resourceCount());
    updateTabLabel(7, tr("Schedules"), m_scheduleWidget->resourceCount());
    updateTabLabel(8, tr("Messages"), m_messagesWidget->resourceCount());
    updateTabLabel(9, tr("Catalogs"), m_catalogWidget->resourceCount());
}

void BConfigImportDialog::onDirectorSelected(const BConfigResource &resource)
{
    Q_UNUSED(resource)
    updateConnectionPreview();
    validateSelection();
}

void BConfigImportDialog::onConsoleSelected(const BConfigResource &resource)
{
    Q_UNUSED(resource)
    updateConnectionPreview();
    validateSelection();
}

void BConfigImportDialog::updateConnectionPreview()
{
    BConfigResource director = selectedDirector();
    BConfigResource console = selectedConsole();

    if (!director.type().isEmpty()) {
        m_previewDirectorLabel->setText(director.name());

        QString address = director.simpleValue("address", "-");
        if (address == "-") {
            BConfigValue dirAddrs = director.value("diraddresses");
            if (dirAddrs.type() == BConfigValue::Block) {
                address = tr("(multiple addresses)");
            }
        }
        m_previewAddressLabel->setText(address);
        m_previewPortLabel->setText(director.simpleValue("dirport", "9101"));

        QString tls = director.simpleValue("tls enable", "no");
        m_previewTlsLabel->setText(tls.toLower() == "yes" ? tr("Enabled") : tr("Disabled"));
    } else {
        m_previewDirectorLabel->setText("-");
        m_previewAddressLabel->setText("-");
        m_previewPortLabel->setText("-");
        m_previewTlsLabel->setText("-");
    }

    if (!console.type().isEmpty()) {
        m_previewConsoleLabel->setText(console.name());
    } else {
        m_previewConsoleLabel->setText("-");
    }
}

void BConfigImportDialog::validateSelection()
{
    BConfigResource director = selectedDirector();
    BConfigResource console = selectedConsole();

    bool valid = !director.type().isEmpty() &&
                 !console.type().isEmpty() &&
                 !m_addressEdit->text().trimmed().isEmpty();
    m_importButton->setEnabled(valid);
}

BConfigResource BConfigImportDialog::selectedDirector() const
{
    int idx = m_directorCombo->currentData().toInt();
    QList<BConfigResource> directors = m_parser->resourcesByType("Director");
    if (idx >= 0 && idx < directors.size()) {
        return directors.at(idx);
    }
    return BConfigResource();
}

BConfigResource BConfigImportDialog::selectedConsole() const
{
    int idx = m_consoleCombo->currentData().toInt();
    QList<BConfigResource> consoles = m_parser->resourcesByType("Console");
    if (idx >= 0 && idx < consoles.size()) {
        return consoles.at(idx);
    }
    return BConfigResource();
}

bool BConfigImportDialog::createConnectionProfile() const
{
    return m_createProfileCheck->isChecked();
}

void BConfigImportDialog::onAccept()
{
    BConfigResource director = selectedDirector();
    BConfigResource console = selectedConsole();
    QString address = m_addressEdit->text().trimmed();
    int port = m_portSpinBox->value();

    if (director.type().isEmpty() || console.type().isEmpty()) {
        QMessageBox::warning(this, tr("Invalid Selection"),
                             tr("Please select both a Director and a Console."));
        return;
    }

    if (address.isEmpty()) {
        QMessageBox::warning(this, tr("Invalid Address"),
                             tr("Please enter the hostname or IP address of the Director."));
        return;
    }

    emit configurationImported(director, console, address, port);
    accept();
}

void BConfigImportDialog::cleanupTempDir()
{
    if (m_tempDir) {
        delete m_tempDir;
        m_tempDir = nullptr;
    }
}

QStringList BConfigImportDialog::validateResources() const
{
    QStringList warnings;
    BDirectiveSchema &schema = BDirectiveSchema::instance();

    // Get all resource types we want to validate
    QStringList resourceTypes = {"Director", "Console", "Client", "Job", "Storage",
                                  "FileSet", "Pool", "Schedule", "Messages", "Catalog"};

    for (const QString &type : resourceTypes) {
        QList<BConfigResource> resources = m_parser->resourcesByType(type);
        QMap<QString, BDirective> directives = schema.directives(type);

        for (const BConfigResource &resource : resources) {
            // Check for missing required fields
            for (auto it = directives.constBegin(); it != directives.constEnd(); ++it) {
                const BDirective &directive = it.value();
                if (directive.required && !resource.hasKey(it.key()) && !resource.hasKey(directive.name)) {
                    // Check if Name is the required field and resource has a name
                    if (directive.name.compare("Name", Qt::CaseInsensitive) == 0 && !resource.name().isEmpty()) {
                        continue;  // Name is set via resource name, not a directive
                    }
                    warnings.append(tr("%1 '%2': Missing required field '%3'")
                                        .arg(type, resource.name(), directive.name));
                }
            }
        }
    }

    return warnings;
}

void BConfigImportDialog::updateValidationDisplay()
{
    QStringList warnings = validateResources();

    if (warnings.isEmpty()) {
        m_validationLabel->setVisible(false);
    } else {
        // Limit display to first 10 warnings
        QString displayText;
        int displayCount = qMin(warnings.size(), 10);
        if (warnings.size() > 10) {
            displayText = tr("<b>Validation Warnings (%1 total, showing first 10):</b><br>").arg(warnings.size());
        } else {
            displayText = tr("<b>Validation Warnings:</b><br>");
        }
        for (int i = 0; i < displayCount; ++i) {
            displayText += QString("• %1<br>").arg(warnings.at(i));
        }

        m_validationLabel->setText(displayText);
        m_validationLabel->setStyleSheet("QLabel { color: #b36b00; background-color: #fff3cd; "
                                          "padding: 8px; border: 1px solid #ffc107; border-radius: 4px; }");
        m_validationLabel->setVisible(true);
    }
}
