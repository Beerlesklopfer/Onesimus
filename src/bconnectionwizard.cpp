/**
 * @file bconnectionwizard.cpp
 * @brief Director Configuration Wizard (Q&A Style)
 */

#include "bconnectionwizard.h"
#include "blogging.h"
#include "director/bareosdirector.h"
#include "bcertificategenerator.h"
#include "config/bprofilesettingsdialog.h"
#include "config/bconfigexporter.h"
#include "bpfxconverter.h"
#include "config/bsettings.h"
#include "db/bdatabase.h"
#include "db/bdirectormodel.h"
#include "config/bconfigparser.h"

#include <private/qzipreader_p.h>
#include <private/qzipwriter_p.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QComboBox>
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTcpSocket>
#include <QClipboard>
#include <QApplication>
#include <QTabWidget>
#include <QDirIterator>

// ============================================================================
// BConnectionWizard
// ============================================================================

BConnectionWizard::BConnectionWizard(BConnectionWizardData *wizardData,
                                     QSqlDatabase *db,
                                     QWidget *parent)
    : QWizard(parent)
    , m_wizardData(wizardData)
    , m_database(db)
{
    setWindowTitle(tr("Connection Setup"));
    setWizardStyle(QWizard::ModernStyle);
    setMinimumSize(500, 400);
    setWindowIcon(QIcon(":/icons/icons/onesimus.svg"));

    // Custom button layout: Connect/Finish on left, Cancel on right
    QList<QWizard::WizardButton> buttonLayout;
    buttonLayout << QWizard::BackButton
                 << QWizard::NextButton
                 << QWizard::FinishButton
                 << QWizard::Stretch
                 << QWizard::CancelButton;
    setButtonLayout(buttonLayout);

    setPage(Page_Welcome, new WelcomePage(this));
    setPage(Page_TemplateSelection, new TemplateSelectionPage(this));
    setPage(Page_Server, new ServerPage(this));
    setPage(Page_Credentials, new CredentialsPage(this));
    setPage(Page_AuthMethod, new AuthMethodPage(this));
    setPage(Page_TLS, new TLSPage(this));
    setPage(Page_ConfigPreview, new ConfigPreviewPage(this));
    setPage(Page_Test, new TestPage(this));
    setPage(Page_ConsoleSetup, new ConsoleSetupPage(this));
    setPage(Page_ProfileName, new ProfileNamePage(this));
    setPage(Page_ImportConsoleSelection, new ImportConsoleSelectionPage(this));

    m_profile = BConnectionProfile::create(tr("New Connection"));
}

BConnectionWizard::~BConnectionWizard() = default;

BConnectionProfile BConnectionWizard::profile() const
{
    BConnectionProfile p = m_profile;

    // Use wizard data struct if available (preserves values across page navigation)
    if (m_wizardData) {
        p.name = m_wizardData->profileName;
        p.host = m_wizardData->host;
        p.port = m_wizardData->port;
        p.directorName = m_wizardData->directorName;
        p.consoleName = m_wizardData->consoleName;

        BLOG_DEBUG() << "[ConnectionWizard] Using wizardData - savePassword:" << m_wizardData->savePassword;

        if (m_wizardData->savePassword) {
            // MANDATORY: Transform cleartext to MD5 hash
            p.setPasswordFromCleartext(m_wizardData->password);
            BLOG_DEBUG() << "[ConnectionWizard] Password saved to profile (as MD5 hash)";
        } else {
            p.passwordHash.clear();
            BLOG_DEBUG() << "[ConnectionWizard] Password cleared from profile";
        }

        QString auth = m_wizardData->authMethod;
        if (auth == "x509") {
            p.legacyAuth = false;
            p.tlsEnabled = true;
            p.tlsUsePSK = false;
            p.tlsCaCertFile = m_wizardData->tlsCaCertFile;
            p.tlsCertFile = m_wizardData->tlsCertFile;
            p.tlsKeyFile = m_wizardData->tlsKeyFile;
            p.tlsVerifyPeer = true;
        } else {
            // Default to TLS-PSK
            p.legacyAuth = false;
            p.tlsEnabled = true;
            p.tlsUsePSK = true;
            p.tlsCipherList = m_wizardData->tlsCipherList;
        }
    } else {
        // Fallback to field values (for backward compatibility)
        p.name = field("profileName").toString();
        p.host = field("host").toString();
        p.port = field("port").toInt();
        p.directorName = field("directorName").toString();
        p.consoleName = field("consoleName").toString();

        bool savePasswordChecked = field("savePassword").toBool();
        BLOG_DEBUG() << "[ConnectionWizard] Using fields - savePassword:" << savePasswordChecked;

        if (savePasswordChecked) {
            // MANDATORY: Transform cleartext to MD5 hash
            p.setPasswordFromCleartext(field("password").toString());
        } else {
            p.passwordHash.clear();
        }

        QString auth = field("authMethod").toString();
        if (auth == "x509") {
            p.legacyAuth = false;
            p.tlsEnabled = true;
            p.tlsUsePSK = false;
            p.tlsCaCertFile = field("caCert").toString();
            p.tlsCertFile = field("clientCert").toString();
            p.tlsKeyFile = field("clientKey").toString();
            p.tlsVerifyPeer = true;
        } else {
            // Default to TLS-PSK
            p.legacyAuth = false;
            p.tlsEnabled = true;
            p.tlsUsePSK = true;
            p.tlsCipherList = field("tlsCipherList").toString();
        }
    }

    return p;
}

void BConnectionWizard::setProfile(const BConnectionProfile &profile)
{
    m_profile = profile;
}

int BConnectionWizard::saveToDatabase(QSqlDatabase &db)
{
    if (!db.isOpen() || !m_wizardData) {
        BLOG_WARNING() << "[ConnectionWizard] Cannot save: database not open or no wizard data";
        return -1;
    }

    // Create Director model
    BDirectorModel model(nullptr, db);
    if (!model.initialize()) {
        BLOG_WARNING() << "[ConnectionWizard] Failed to initialize BDirectorModel";
        return -1;
    }

    // Create Director record with basic connection info
    int dirId = model.createDirector(
        m_wizardData->directorName,
        m_wizardData->host,
        m_wizardData->port,
        m_wizardData->password,  // Already MD5 hash from wizard
        "bareos"  // Default to Bareos
    );

    if (dirId < 0) {
        BLOG_WARNING() << "[ConnectionWizard] Failed to create Director record";
        return -1;
    }

    // Find the row for the new Director
    int row = model.findDirectorRow(dirId);
    if (row < 0) {
        BLOG_WARNING() << "[ConnectionWizard] Director created but row not found";
        return -1;
    }

    // Update TLS settings based on auth method
    QString auth = m_wizardData->authMethod;
    if (auth == "x509") {
        // Certificate mode: TLS with certificates
        model.setData(model.index(row, BDirectorModel::TlsEnable), true);
        model.setData(model.index(row, BDirectorModel::TlsRequire), true);
        model.setData(model.index(row, BDirectorModel::TlsVerifyPeer), true);
        model.setData(model.index(row, BDirectorModel::TlsCaCertificateFile), m_wizardData->tlsCaCertFile);
        model.setData(model.index(row, BDirectorModel::TlsCertificate), m_wizardData->tlsCertFile);
        model.setData(model.index(row, BDirectorModel::TlsKey), m_wizardData->tlsKeyFile);
    } else {
        // Default: TLS-PSK - TLS enabled, no certificates
        model.setData(model.index(row, BDirectorModel::TlsEnable), true);
        model.setData(model.index(row, BDirectorModel::TlsRequire), true);
    }

    // Submit all changes
    if (!model.submitAll()) {
        BLOG_WARNING() << "[ConnectionWizard] Failed to save Director TLS settings:" << model.lastError().text();
        return -1;
    }

    BLOG_DEBUG() << "[ConnectionWizard] Director saved to database with ID:" << dirId;
    return dirId;
}

// ============================================================================
// QAPage
// ============================================================================

QAPage::QAPage(const QString &question, QWidget *parent)
    : QWizardPage(parent)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(15);

    m_questionLabel = new QLabel(question, this);
    m_questionLabel->setWordWrap(true);
    QFont font = m_questionLabel->font();
    font.setPointSize(font.pointSize() + 2);
    m_questionLabel->setFont(font);
    m_layout->addWidget(m_questionLabel);

    m_hintLabel = new QLabel(this);
    m_hintLabel->setWordWrap(true);
    m_hintLabel->setStyleSheet("color: gray; font-style: italic;");
    m_hintLabel->setVisible(false);
    m_layout->addWidget(m_hintLabel);
}

void QAPage::setHint(const QString &hint)
{
    m_hintLabel->setText(hint);
    m_hintLabel->setVisible(!hint.isEmpty());
}

// ============================================================================
// WelcomePage
// ============================================================================

WelcomePage::WelcomePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Welcome"));

    auto *layout = new QVBoxLayout(this);
    auto *label = new QLabel(this);
    label->setWordWrap(true);
    QFont font = label->font();
    font.setPointSize(font.pointSize() + 2);
    label->setFont(font);
    label->setText(tr(
        "Let's set up a connection to your Bareos Director.\n\n"
        "You'll need:\n"
        "  - The hostname or IP of your Director\n"
        "  - Your console name and password\n\n"
        "Click Next to begin."
    ));
    layout->addWidget(label);
    layout->addStretch();
}

// ============================================================================
// TemplateSelectionPage
// ============================================================================

TemplateSelectionPage::TemplateSelectionPage(QWidget *parent)
    : QAPage(tr("How would you like to create the connection?"), parent)
{
    setHint(tr("You can start from scratch, use a template from an existing connection, or import from a ZIP file."));

    m_group = new QButtonGroup(this);

    m_newConnectionRadio = new QRadioButton(tr("Create new connection from scratch"), this);
    m_fromTemplateRadio = new QRadioButton(tr("Use template from existing connection"), this);
    m_importZipRadio = new QRadioButton(tr("Import from ZIP file"), this);

    m_group->addButton(m_newConnectionRadio, 0);
    m_group->addButton(m_fromTemplateRadio, 1);
    m_group->addButton(m_importZipRadio, 2);

    m_newConnectionRadio->setChecked(true);

    m_layout->addWidget(m_newConnectionRadio);
    m_layout->addSpacing(10);

    // Template selection group
    m_layout->addWidget(m_fromTemplateRadio);

    auto *templateLayout = new QHBoxLayout();
    templateLayout->addSpacing(30);  // Indent

    m_templateCombo = new QComboBox(this);
    m_templateCombo->setEnabled(false);
    m_templateCombo->setMinimumWidth(300);
    templateLayout->addWidget(m_templateCombo, 1);

    m_refreshButton = new QPushButton(tr("Refresh"), this);
    m_refreshButton->setEnabled(false);
    m_refreshButton->setMaximumWidth(80);
    templateLayout->addWidget(m_refreshButton);

    m_layout->addLayout(templateLayout);

    m_templateDetailsLabel = new QLabel(this);
    m_templateDetailsLabel->setWordWrap(true);
    m_templateDetailsLabel->setStyleSheet("color: gray; font-style: italic; margin-left: 30px;");
    m_templateDetailsLabel->setVisible(false);
    m_layout->addWidget(m_templateDetailsLabel);

    m_layout->addSpacing(10);

    // ZIP import group
    m_layout->addWidget(m_importZipRadio);

    auto *zipLayout = new QHBoxLayout();
    zipLayout->addSpacing(30);  // Indent

    m_zipFileEdit = new QLineEdit(this);
    m_zipFileEdit->setEnabled(false);
    m_zipFileEdit->setPlaceholderText(tr("Select ZIP file..."));
    zipLayout->addWidget(m_zipFileEdit, 1);

    m_browseZipButton = new QPushButton(tr("Browse..."), this);
    m_browseZipButton->setEnabled(false);
    m_browseZipButton->setMaximumWidth(80);
    zipLayout->addWidget(m_browseZipButton);

    m_layout->addLayout(zipLayout);

    m_layout->addStretch();

    // Connect signals
    connect(m_group, &QButtonGroup::idClicked,
            this, &TemplateSelectionPage::onSelectionChanged);
    connect(m_templateCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TemplateSelectionPage::onTemplateSelected);
    connect(m_refreshButton, &QPushButton::clicked,
            this, &TemplateSelectionPage::onRefreshClicked);
    connect(m_browseZipButton, &QPushButton::clicked,
            this, &TemplateSelectionPage::onBrowseZipClicked);

    // Register fields for access from other pages
    registerField("useTemplate", m_fromTemplateRadio);
    registerField("importFromZip", m_importZipRadio);
    registerField("importZipPath", m_zipFileEdit);
}

void TemplateSelectionPage::initializePage()
{
    loadTemplates();
}

void TemplateSelectionPage::loadTemplates()
{
    m_templateCombo->clear();
    m_templateDetailsLabel->setVisible(false);

    int count = 0;

    // First, load connection profiles from BSettings (primary source)
    QList<BConnectionProfile> profiles = BSettings::instance().connectionProfiles();
    for (const BConnectionProfile &profile : profiles) {
        if (profile.isValid()) {
            QString displayText = QString("%1 (%2:%3)")
                .arg(profile.name, profile.host).arg(profile.port);
            // Use negative IDs prefixed with 'p' concept: store profile ID as string in user data
            m_templateCombo->addItem(displayText, "profile:" + profile.id);
            count++;
        }
    }

    // Optionally, also load Directors from database (legacy/future support)
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->database() && wiz->database()->isOpen()) {
        BDirectorModel *model = new BDirectorModel(this, *wiz->database());
        if (model->initialize()) {
            model->setFilterActiveOnly(true);

            for (int row = 0; row < model->rowCount(); ++row) {
                int dirId = model->directorId(row);
                QString name = model->directorName(row);
                QString address = model->data(model->index(row, BDirectorModel::Address)).toString();
                int port = model->data(model->index(row, BDirectorModel::Port)).toInt();

                QString displayText = QString("%1 (%2:%3) [DB]").arg(name, address).arg(port);
                m_templateCombo->addItem(displayText, "db:" + QString::number(dirId));
                count++;
            }
        }
        delete model;
    }

    // Hide template option entirely when no saved connections exist
    bool hasTemplates = (count > 0);
    m_fromTemplateRadio->setVisible(hasTemplates);
    m_templateCombo->setVisible(hasTemplates);
    m_refreshButton->setVisible(hasTemplates);

    // If template was selected but no templates available, switch to new connection
    if (!hasTemplates && m_fromTemplateRadio->isChecked()) {
        m_newConnectionRadio->setChecked(true);
        onSelectionChanged();
    }
}

void TemplateSelectionPage::loadTemplateDetails(int index)
{
    Q_UNUSED(index)  // We use currentData() instead

    QVariant data = m_templateCombo->currentData();
    if (!data.isValid() || data.isNull()) {
        m_templateDetailsLabel->setVisible(false);
        return;
    }

    QString dataStr = data.toString();
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());

    if (dataStr.startsWith("profile:")) {
        // Load from BSettings profile
        QString profileId = dataStr.mid(8);  // Remove "profile:" prefix
        BConnectionProfile profile = BSettings::instance().connectionProfile(profileId);

        if (!profile.isValid()) {
            m_templateDetailsLabel->setVisible(false);
            return;
        }

        // Show details
        QString details;
        if (!profile.description.isEmpty()) {
            details += profile.description + "\n";
        }
        details += tr("Director: %1").arg(profile.directorName);
        details += " | " + tr("Console: %1").arg(profile.consoleName);
        details += "\n" + tr("TLS: %1").arg(profile.tlsEnabled ? (profile.tlsUsePSK ? tr("PSK") : tr("Certificate")) : tr("Disabled"));

        m_templateDetailsLabel->setText(details);
        m_templateDetailsLabel->setVisible(true);

        // Pre-fill wizard data
        if (wiz && wiz->wizardData()) {
            wiz->wizardData()->host = profile.host;
            wiz->wizardData()->port = profile.port;
            wiz->wizardData()->directorName = profile.directorName;
            wiz->wizardData()->consoleName = profile.consoleName;
            wiz->wizardData()->password = profile.passwordHash;  // Already hashed

            if (profile.tlsUsePSK || !profile.tlsEnabled) {
                // Default to PSK (legacy profiles without TLS are upgraded to PSK)
                wiz->wizardData()->authMethod = "psk";
            } else {
                wiz->wizardData()->authMethod = "x509";
                wiz->wizardData()->tlsCaCertFile = profile.tlsCaCertFile;
                wiz->wizardData()->tlsCertFile = profile.tlsCertFile;
                wiz->wizardData()->tlsKeyFile = profile.tlsKeyFile;
            }
        }
    }
    else if (dataStr.startsWith("db:")) {
        // Load from database
        int directorId = dataStr.mid(3).toInt();  // Remove "db:" prefix

        if (!wiz || !wiz->database() || !wiz->database()->isOpen()) {
            m_templateDetailsLabel->setVisible(false);
            return;
        }

        BDirectorModel *model = new BDirectorModel(this, *wiz->database());
        if (!model->initialize()) {
            m_templateDetailsLabel->setVisible(false);
            delete model;
            return;
        }

        int row = model->findDirectorRow(directorId);
        if (row < 0) {
            m_templateDetailsLabel->setVisible(false);
            delete model;
            return;
        }

        // Get details
        QString desc = model->data(model->index(row, BDirectorModel::Description)).toString();
        bool tlsEnable = model->data(model->index(row, BDirectorModel::TlsEnable)).toBool();
        QString backupSystem = model->data(model->index(row, BDirectorModel::BackupSystem)).toString();

        QString details;
        if (!desc.isEmpty()) {
            details += desc + "\n";
        }
        details += tr("TLS: %1").arg(tlsEnable ? tr("Enabled") : tr("Disabled"));
        details += " | " + tr("System: %1").arg(backupSystem);

        m_templateDetailsLabel->setText(details);
        m_templateDetailsLabel->setVisible(true);

        // Pre-fill wizard data
        if (wiz->wizardData()) {
            wiz->wizardData()->host = model->data(model->index(row, BDirectorModel::Address)).toString();
            wiz->wizardData()->port = model->data(model->index(row, BDirectorModel::Port)).toInt();
            wiz->wizardData()->directorName = model->directorName(row);
            wiz->wizardData()->password = model->data(model->index(row, BDirectorModel::PasswordHash)).toString();

            QString tlsCert = model->data(model->index(row, BDirectorModel::TlsCertificate)).toString();
            if (!tlsCert.isEmpty() && tlsEnable) {
                wiz->wizardData()->authMethod = "x509";
                wiz->wizardData()->tlsCaCertFile = model->data(model->index(row, BDirectorModel::TlsCaCertificateFile)).toString();
                wiz->wizardData()->tlsCertFile = tlsCert;
                wiz->wizardData()->tlsKeyFile = model->data(model->index(row, BDirectorModel::TlsKey)).toString();
            } else {
                // Default to PSK (legacy profiles without TLS are upgraded to PSK)
                wiz->wizardData()->authMethod = "psk";
            }
        }

        delete model;
    }
    else {
        m_templateDetailsLabel->setVisible(false);
    }
}

int TemplateSelectionPage::nextId() const
{
    if (m_importZipRadio->isChecked()) {
        // Go to console selection page for imported config
        return BConnectionWizard::Page_ImportConsoleSelection;
    }

    // Normal flow: go to Server page
    return BConnectionWizard::Page_Server;
}

bool TemplateSelectionPage::isComplete() const
{
    if (m_newConnectionRadio->isChecked()) {
        return true;
    }

    if (m_fromTemplateRadio->isChecked()) {
        QVariant data = m_templateCombo->currentData();
        if (!data.isValid() || data.isNull()) {
            return false;
        }
        QString dataStr = data.toString();
        return dataStr.startsWith("profile:") || dataStr.startsWith("db:");
    }

    if (m_importZipRadio->isChecked()) {
        return !m_zipFileEdit->text().isEmpty() && QFile::exists(m_zipFileEdit->text());
    }

    return false;
}

void TemplateSelectionPage::onSelectionChanged()
{
    bool useTemplate = m_fromTemplateRadio->isChecked();
    bool importZip = m_importZipRadio->isChecked();

    m_templateCombo->setEnabled(useTemplate);
    m_refreshButton->setEnabled(useTemplate);

    m_zipFileEdit->setEnabled(importZip);
    m_browseZipButton->setEnabled(importZip);

    if (!useTemplate) {
        m_templateDetailsLabel->setVisible(false);
    } else {
        onTemplateSelected(m_templateCombo->currentIndex());
    }

    emit completeChanged();
}

void TemplateSelectionPage::onTemplateSelected(int index)
{
    if (!m_fromTemplateRadio->isChecked()) {
        return;
    }

    // loadTemplateDetails now handles both profile and db sources
    // and pre-fills wizard data automatically
    loadTemplateDetails(index);

    emit completeChanged();
}

void TemplateSelectionPage::onRefreshClicked()
{
    loadTemplates();
}

void TemplateSelectionPage::onBrowseZipClicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Select Director Configuration ZIP"),
        QString(),
        tr("ZIP Files (*.zip);;All Files (*)")
    );

    if (!fileName.isEmpty()) {
        m_zipFileEdit->setText(fileName);
        emit completeChanged();
    }
}

// ============================================================================
// ImportConsoleSelectionPage
// ============================================================================

ImportConsoleSelectionPage::ImportConsoleSelectionPage(QWidget *parent)
    : QAPage(tr("Select Director and Console from imported configuration"), parent)
    , m_parser(new BConfigParser(this))
    , m_tempDir(nullptr)
    , m_parsed(false)
{
    setHint(tr("The configuration will be parsed from your ZIP file. "
               "Select which Director and Console to use for the connection."));

    auto *form = new QFormLayout();

    m_directorCombo = new QComboBox(this);
    form->addRow(tr("Director:"), m_directorCombo);

    m_consoleCombo = new QComboBox(this);
    form->addRow(tr("Console:"), m_consoleCombo);

    m_layout->addLayout(form);

    // Progress bar for parsing
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0);  // Indeterminate
    m_progressBar->setVisible(false);
    m_layout->addWidget(m_progressBar);

    // Status label
    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet("color: gray; font-style: italic;");
    m_layout->addWidget(m_statusLabel);

    // Details area (read-only) showing selected console config
    m_detailsEdit = new QTextEdit(this);
    m_detailsEdit->setReadOnly(true);
    m_detailsEdit->setMaximumHeight(200);
    m_detailsEdit->setFont(QFont("monospace", 9));
    m_detailsEdit->setPlaceholderText(tr("Select a console to view its details..."));
    m_layout->addWidget(m_detailsEdit);

    m_layout->addStretch();

    connect(m_directorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) {
                updateConsoleDetails();
                emit completeChanged();
            });
    connect(m_consoleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) {
                updateConsoleDetails();
                emit completeChanged();
            });
}

ImportConsoleSelectionPage::~ImportConsoleSelectionPage()
{
    cleanupTempDir();
}

void ImportConsoleSelectionPage::initializePage()
{
    m_parsed = false;
    m_directorCombo->clear();
    m_consoleCombo->clear();
    m_detailsEdit->clear();
    m_parser->clear();

    parseConfigSource();
}

void ImportConsoleSelectionPage::cleanupPage()
{
    // Nothing special needed when navigating back
}

void ImportConsoleSelectionPage::parseConfigSource()
{
    QString zipPath = field("importZipPath").toString();
    if (zipPath.isEmpty()) {
        m_statusLabel->setText(tr("No ZIP file specified."));
        m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
        return;
    }

    m_progressBar->setVisible(true);
    m_statusLabel->setText(tr("Extracting and parsing ZIP archive..."));
    m_statusLabel->setStyleSheet("color: gray; font-style: italic;");

    // Clean up any previous temp directory
    cleanupTempDir();
    m_tempDir = new QTemporaryDir();

    if (!m_tempDir->isValid()) {
        m_statusLabel->setText(tr("Failed to create temporary directory."));
        m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
        m_progressBar->setVisible(false);
        return;
    }

    // Extract ZIP using QZipReader (Qt6 private API)
    QZipReader zip(zipPath);
    if (!zip.isReadable()) {
        m_statusLabel->setText(tr("Cannot read ZIP file: %1").arg(zipPath));
        m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
        m_progressBar->setVisible(false);
        return;
    }

    if (!zip.extractAll(m_tempDir->path())) {
        m_statusLabel->setText(tr("Failed to extract ZIP archive."));
        m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
        m_progressBar->setVisible(false);
        return;
    }

    zip.close();

    // Scan extracted content for config sources
    // 1. Look for directory-based configs (bareos-dir.d/ or bacula-dir.d/)
    QDirIterator dirIt(m_tempDir->path(),
                       QStringList() << "bareos-dir.d" << "bacula-dir.d",
                       QDir::Dirs, QDirIterator::Subdirectories);

    bool parsed = false;
    if (dirIt.hasNext()) {
        QString configDir = dirIt.next();
        BLOG_DEBUG() << "[ImportConsole] Found config directory:" << configDir;
        parsed = m_parser->parseDirectory(configDir);
    }

    // 2. If no directory found, look for flat config files (*.conf)
    if (!parsed || m_parser->resources().isEmpty()) {
        QDirIterator confIt(m_tempDir->path(),
                            QStringList() << "*.conf",
                            QDir::Files, QDirIterator::Subdirectories);
        while (confIt.hasNext()) {
            QString confFile = confIt.next();
            BLOG_DEBUG() << "[ImportConsole] Parsing config file:" << confFile;
            m_parser->parseFile(confFile);
            parsed = true;
        }
    }

    m_progressBar->setVisible(false);

    if (!parsed || m_parser->resources().isEmpty()) {
        m_statusLabel->setText(tr("No configuration resources found in the archive."));
        m_statusLabel->setStyleSheet("color: #c08000; font-style: italic;");
        return;
    }

    m_parsed = true;
    populateCombos();
}

void ImportConsoleSelectionPage::populateCombos()
{
    QList<BConfigResource> directors = m_parser->resourcesByType("Director");
    QList<BConfigResource> consoles = m_parser->resourcesByType("Console");

    m_directorCombo->clear();
    for (int i = 0; i < directors.size(); ++i) {
        m_directorCombo->addItem(directors.at(i).name(), i);
    }

    m_consoleCombo->clear();
    for (int i = 0; i < consoles.size(); ++i) {
        m_consoleCombo->addItem(consoles.at(i).name(), i);
    }

    int totalResources = m_parser->resources().size();
    m_statusLabel->setText(tr("Parsed %1 resources: %2 Director(s), %3 Console(s)")
                               .arg(totalResources)
                               .arg(directors.size())
                               .arg(consoles.size()));
    m_statusLabel->setStyleSheet("color: #008000; font-style: italic;");

    if (directors.isEmpty() || consoles.isEmpty()) {
        m_statusLabel->setText(tr("Warning: Need at least one Director and one Console resource."));
        m_statusLabel->setStyleSheet("color: #c08000; font-style: italic;");
    }

    updateConsoleDetails();
    emit completeChanged();
}

void ImportConsoleSelectionPage::updateConsoleDetails()
{
    QList<BConfigResource> directors = m_parser->resourcesByType("Director");
    QList<BConfigResource> consoles = m_parser->resourcesByType("Console");

    int directorIdx = m_directorCombo->currentData().toInt();
    int consoleIdx = m_consoleCombo->currentData().toInt();

    QString details;

    // Show Director info
    if (directorIdx >= 0 && directorIdx < directors.size()) {
        const BConfigResource &dir = directors.at(directorIdx);
        details += tr("=== Director: %1 ===\n").arg(dir.name());
        QString address = dir.simpleValue("address", "");
        if (address.isEmpty()) {
            BConfigValue dirAddrs = dir.value("diraddresses");
            if (dirAddrs.type() == BConfigValue::Block) {
                address = tr("(multiple addresses)");
            }
        }
        details += tr("  Address: %1\n").arg(address.isEmpty() ? tr("(not set)") : address);
        details += tr("  Port: %1\n").arg(dir.simpleValue("dirport", "9101"));
        details += tr("  TLS Enable: %1\n").arg(dir.simpleValue("tls enable", "no"));
        details += "\n";
    }

    // Show Console info
    if (consoleIdx >= 0 && consoleIdx < consoles.size()) {
        const BConfigResource &con = consoles.at(consoleIdx);
        details += tr("=== Console: %1 ===\n").arg(con.name());

        // Password (masked)
        QString pwd = con.simpleValue("password", "");
        if (!pwd.isEmpty()) {
            if (pwd.startsWith("[md5]")) {
                details += tr("  Password: [md5]***\n");
            } else {
                details += tr("  Password: ***\n");
            }
        } else {
            details += tr("  Password: (not set)\n");
        }

        // TLS settings
        details += tr("  TLS Enable: %1\n").arg(con.simpleValue("tls enable", "no"));
        details += tr("  TLS Require: %1\n").arg(con.simpleValue("tls require", "no"));

        // Profile reference
        QString profile = con.simpleValue("profile", "");
        if (!profile.isEmpty()) {
            details += tr("  Profile: %1\n").arg(profile);
        }

        // ACLs
        QStringList aclKeys = {"commandacl", "jobacl", "clientacl", "storageacl",
                               "poolacl", "filesetacl", "catalogacl", "scheduleacl",
                               "whereacl", "pluginoptionsacl"};
        QStringList aclLabels = {"Command ACL", "Job ACL", "Client ACL", "Storage ACL",
                                 "Pool ACL", "FileSet ACL", "Catalog ACL", "Schedule ACL",
                                 "Where ACL", "PluginOptions ACL"};

        bool hasAnyAcl = false;
        for (int i = 0; i < aclKeys.size(); ++i) {
            QStringList values = con.listValue(aclKeys.at(i));
            if (!values.isEmpty()) {
                if (!hasAnyAcl) {
                    details += tr("\n  --- ACLs ---\n");
                    hasAnyAcl = true;
                }
                details += tr("  %1: %2\n").arg(aclLabels.at(i), values.join(", "));
            }
        }

        if (!hasAnyAcl) {
            details += tr("\n  (No ACLs defined on this console)\n");
        }
    }

    m_detailsEdit->setPlainText(details);
}

void ImportConsoleSelectionPage::prefillWizardData()
{
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (!wiz || !wiz->wizardData()) return;

    BConnectionWizardData *data = wiz->wizardData();

    QList<BConfigResource> directors = m_parser->resourcesByType("Director");
    QList<BConfigResource> consoles = m_parser->resourcesByType("Console");
    int directorIdx = m_directorCombo->currentData().toInt();
    int consoleIdx = m_consoleCombo->currentData().toInt();

    // Pre-fill from Director resource
    if (directorIdx >= 0 && directorIdx < directors.size()) {
        const BConfigResource &dir = directors.at(directorIdx);
        data->directorName = dir.name();

        // Extract address
        QString address = dir.simpleValue("address", "");
        if (address.isEmpty()) {
            BConfigValue dirAddrs = dir.value("diraddresses");
            if (dirAddrs.type() == BConfigValue::Block) {
                QMap<QString, BConfigValue> block = dirAddrs.blockValue();
                for (auto it = block.constBegin(); it != block.constEnd(); ++it) {
                    if (it.value().type() == BConfigValue::Block) {
                        QMap<QString, BConfigValue> inner = it.value().blockValue();
                        if (inner.contains("addr")) {
                            address = inner["addr"].simpleValue();
                            break;
                        }
                    }
                }
            }
        }
        if (!address.isEmpty()) {
            data->host = address;
        }

        // Port
        QString portStr = dir.simpleValue("dirport", "9101");
        bool ok;
        int port = portStr.toInt(&ok);
        if (ok && port > 0 && port <= 65535) {
            data->port = port;
        }
    }

    // Pre-fill from Console resource
    if (consoleIdx >= 0 && consoleIdx < consoles.size()) {
        const BConfigResource &con = consoles.at(consoleIdx);
        data->consoleName = con.name();

        // Password - strip [md5] prefix if present
        QString password = con.simpleValue("password", "");
        if (password.startsWith("[md5]")) {
            password = password.mid(5);  // Raw MD5 hash
        }
        data->password = password;

        // TLS settings
        QString tlsEnable = con.simpleValue("tls enable", "no");
        if (tlsEnable.toLower() == "yes") {
            QString caCert = con.simpleValue("tls ca certificate file", "");
            QString cert = con.simpleValue("tls certificate", "");
            QString key = con.simpleValue("tls key", "");

            if (!caCert.isEmpty() || !cert.isEmpty()) {
                data->authMethod = "x509";
                data->tlsCaCertFile = caCert;
                data->tlsCertFile = cert;
                data->tlsKeyFile = key;
            } else {
                data->authMethod = "psk";
            }
        } else {
            // Default to PSK (legacy profiles without TLS are upgraded to PSK)
            data->authMethod = "psk";
        }

        // Populate ACLs on the wizard's profile
        BConnectionProfile profile = wiz->profile();
        profile.aclCommand = con.listValue("commandacl");
        profile.aclJob = con.listValue("jobacl");
        profile.aclClient = con.listValue("clientacl");
        profile.aclStorage = con.listValue("storageacl");
        profile.aclPool = con.listValue("poolacl");
        profile.aclFileSet = con.listValue("filesetacl");
        profile.aclCatalog = con.listValue("catalogacl");
        profile.aclSchedule = con.listValue("scheduleacl");
        profile.aclWhere = con.listValue("whereacl");
        profile.aclPluginOptions = con.listValue("pluginoptionsacl");

        // Profile reference
        QString profileRef = con.simpleValue("profile", "");
        if (!profileRef.isEmpty()) {
            profile.profile = profileRef;
        }

        // TLS certificate paths
        profile.tlsCaCertFile = con.simpleValue("tls ca certificate file", "");
        profile.tlsCertFile = con.simpleValue("tls certificate", "");
        profile.tlsKeyFile = con.simpleValue("tls key", "");

        wiz->setProfile(profile);
    }
}

bool ImportConsoleSelectionPage::isComplete() const
{
    if (!m_parsed) return false;

    QList<BConfigResource> directors = m_parser->resourcesByType("Director");
    QList<BConfigResource> consoles = m_parser->resourcesByType("Console");
    int dirIdx = m_directorCombo->currentData().toInt();
    int conIdx = m_consoleCombo->currentData().toInt();

    return (dirIdx >= 0 && dirIdx < directors.size() &&
            conIdx >= 0 && conIdx < consoles.size());
}

bool ImportConsoleSelectionPage::validatePage()
{
    prefillWizardData();

    // Validate completeness
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        BConnectionWizardData *data = wiz->wizardData();

        QStringList missing;
        if (data->host.isEmpty()) missing << tr("Host/Address");
        if (data->directorName.isEmpty()) missing << tr("Director Name");
        if (data->consoleName.isEmpty()) missing << tr("Console Name");
        if (data->password.isEmpty()) missing << tr("Password");

        if (!missing.isEmpty()) {
            QMessageBox::warning(this, tr("Incomplete Configuration"),
                tr("The imported configuration is missing the following required fields:\n\n"
                   "  %1\n\n"
                   "You can fill them in on the following pages.")
                    .arg(missing.join("\n  ")));
        }
    }

    return true;
}

int ImportConsoleSelectionPage::nextId() const
{
    return BConnectionWizard::Page_Server;
}

void ImportConsoleSelectionPage::cleanupTempDir()
{
    if (m_tempDir) {
        delete m_tempDir;
        m_tempDir = nullptr;
    }
}

// ============================================================================
// ServerPage
// ============================================================================

ServerPage::ServerPage(QWidget *parent)
    : QAPage(tr("Where is your Bareos Director?"), parent)
{
    auto *form = new QFormLayout();

    m_hostEdit = new QLineEdit(this);
    m_hostEdit->setPlaceholderText(tr("e.g., bareos.example.com or 192.168.1.100"));
    form->addRow(tr("Host:"), m_hostEdit);

    m_portSpin = new QSpinBox(this);
    m_portSpin->setRange(1, 65535);
    m_portSpin->setValue(9101);
    m_portSpin->setMaximumWidth(100);
    form->addRow(tr("Port:"), m_portSpin);

    m_platformCombo = new QComboBox(this);
    m_platformCombo->addItem(tr("Linux/Unix"), "linux");
    m_platformCombo->addItem(tr("Windows"), "windows");
    m_platformCombo->addItem(tr("FreeBSD"), "freebsd");
    m_platformCombo->addItem(tr("macOS"), "darwin");
    m_platformCombo->setToolTip(tr("Select the operating system of the Bareos Director server.\n"
                                   "This affects certificate paths in exported configurations."));
    form->addRow(tr("Server OS:"), m_platformCombo);

    m_layout->addLayout(form);

    // Config file path hint
    auto *configLabel = new QLabel(this);
    configLabel->setWordWrap(true);
    configLabel->setStyleSheet("color: #666; font-size: 11px;");
    configLabel->setText(tr("Director config: /etc/bareos/bareos-dir.d/director/bareos-dir.conf"));
    m_layout->addWidget(configLabel);

    setHint(tr("The default port is 9101."));
    m_layout->addStretch();

    registerField("host", m_hostEdit);
    registerField("port", m_portSpin);
    registerField("serverPlatform", m_platformCombo, "currentData", "currentIndexChanged");

    connect(m_hostEdit, &QLineEdit::textChanged, this, &ServerPage::completeChanged);
}

void ServerPage::initializePage()
{
    // Pre-fill fields from wizardData if available (e.g., from template selection)
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        BConnectionWizardData *data = wiz->wizardData();
        if (!data->host.isEmpty()) {
            m_hostEdit->setText(data->host);
        }
        if (data->port > 0) {
            m_portSpin->setValue(data->port);
        }
        // Set server platform
        int platformIdx = m_platformCombo->findData(data->serverPlatform);
        if (platformIdx >= 0) {
            m_platformCombo->setCurrentIndex(platformIdx);
        }
    }
}

bool ServerPage::isComplete() const
{
    return !m_hostEdit->text().trimmed().isEmpty();
}

bool ServerPage::validatePage()
{
    // Save values to wizard data struct
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        wiz->wizardData()->host = m_hostEdit->text().trimmed();
        wiz->wizardData()->port = m_portSpin->value();
        wiz->wizardData()->serverPlatform = m_platformCombo->currentData().toString();
    }

    return true;
}

// ============================================================================
// CredentialsPage
// ============================================================================

CredentialsPage::CredentialsPage(QWidget *parent)
    : QAPage(tr("Enter your credentials"), parent)
{
    auto *form = new QFormLayout();

    m_directorEdit = new QLineEdit(this);
    m_directorEdit->setText("bareos-dir");
    form->addRow(tr("Director:"), m_directorEdit);

    m_consoleEdit = new QLineEdit(this);
    m_consoleEdit->setText("onesimus");
    form->addRow(tr("Console:"), m_consoleEdit);

    // Password field with generate button
    auto *passwordRow = new QHBoxLayout();
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    passwordRow->addWidget(m_passwordEdit);

    m_generateBtn = new QPushButton(tr("Generate"), this);
    m_generateBtn->setToolTip(tr("Generate a secure random password"));
    m_generateBtn->setMaximumWidth(80);
    connect(m_generateBtn, &QPushButton::clicked, this, &CredentialsPage::onGeneratePassword);
    passwordRow->addWidget(m_generateBtn);

    form->addRow(tr("Password:"), passwordRow);

    // MD5 Hash preview (read-only, updates in real-time)
    m_md5Label = new QLabel(this);
    m_md5Label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_md5Label->setStyleSheet("QLabel { color: #666; font-family: monospace; font-size: 11px; }");
    m_md5Label->setText(tr("(password MD5 hash will appear here)"));
    form->addRow(tr("MD5 Hash:"), m_md5Label);

    // Update MD5 hash in real-time as user types
    connect(m_passwordEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (text.isEmpty()) {
            m_md5Label->setText(tr("(password MD5 hash will appear here)"));
            m_md5Label->setStyleSheet("QLabel { color: #666; font-family: monospace; font-size: 11px; }");
        } else if (text.startsWith("[md5]")) {
            // Already an MD5 hash with prefix - extract and display
            QString hash = text.mid(5);  // Remove [md5] prefix
            m_md5Label->setText(hash);
            m_md5Label->setStyleSheet("QLabel { color: #080; font-family: monospace; font-size: 11px; font-weight: bold; }");
        } else if (text.length() == 32) {
            // Check if it's a raw 32-char hex MD5 hash
            bool isValidHex = true;
            for (const QChar &c : text) {
                if (!c.isDigit() && (c.toLower() < 'a' || c.toLower() > 'f')) {
                    isValidHex = false;
                    break;
                }
            }
            if (isValidHex) {
                m_md5Label->setText(text.toLower());
                m_md5Label->setStyleSheet("QLabel { color: #080; font-family: monospace; font-size: 11px; font-weight: bold; }");
            } else {
                QByteArray md5 = QCryptographicHash::hash(text.toLatin1(), QCryptographicHash::Md5);
                m_md5Label->setText(QString::fromLatin1(md5.toHex()));
                m_md5Label->setStyleSheet("QLabel { color: #000; font-family: monospace; font-size: 11px; font-weight: bold; }");
            }
        } else {
            QByteArray md5 = QCryptographicHash::hash(text.toLatin1(), QCryptographicHash::Md5);
            m_md5Label->setText(QString::fromLatin1(md5.toHex()));
            m_md5Label->setStyleSheet("QLabel { color: #000; font-family: monospace; font-size: 11px; font-weight: bold; }");
        }
    });

    m_layout->addLayout(form);

    m_saveCheck = new QCheckBox(tr("Remember password"), this);
    m_saveCheck->setChecked(true);
    m_layout->addWidget(m_saveCheck);

    // Config file path hints
    auto *configLabel = new QLabel(this);
    configLabel->setWordWrap(true);
    configLabel->setStyleSheet("color: #666; font-size: 11px;");
    configLabel->setText(tr(
        "Director config: /etc/bareos/bareos-dir.d/director/bareos-dir.conf\n"
        "Console config: /etc/bareos/bareos-dir.d/console/<name>.conf"
    ));
    m_layout->addWidget(configLabel);

    setHint(tr("These must match your Bareos Director configuration."));
    m_layout->addStretch();

    registerField("directorName", m_directorEdit);
    registerField("consoleName", m_consoleEdit);
    registerField("password", m_passwordEdit);
    registerField("savePassword", m_saveCheck);

    connect(m_directorEdit, &QLineEdit::textChanged, this, &CredentialsPage::completeChanged);
    connect(m_consoleEdit, &QLineEdit::textChanged, this, &CredentialsPage::completeChanged);
    connect(m_passwordEdit, &QLineEdit::textChanged, this, &CredentialsPage::completeChanged);
}

void CredentialsPage::initializePage()
{
    // Pre-fill fields from wizardData if available (e.g., from template selection)
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        BConnectionWizardData *data = wiz->wizardData();
        if (!data->directorName.isEmpty()) {
            m_directorEdit->setText(data->directorName);
        }
        if (!data->consoleName.isEmpty()) {
            m_consoleEdit->setText(data->consoleName);
        }
        if (!data->password.isEmpty()) {
            m_passwordEdit->setText(data->password);
        }
        m_saveCheck->setChecked(data->savePassword);
    }
}

bool CredentialsPage::isComplete() const
{
    return !m_directorEdit->text().trimmed().isEmpty() &&
           !m_consoleEdit->text().trimmed().isEmpty() &&
           !m_passwordEdit->text().isEmpty();
}

bool CredentialsPage::validatePage()
{
    // Save values to wizard data struct
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        wiz->wizardData()->directorName = m_directorEdit->text().trimmed();
        wiz->wizardData()->consoleName = m_consoleEdit->text().trimmed();
        wiz->wizardData()->password = m_passwordEdit->text();
        wiz->wizardData()->savePassword = m_saveCheck->isChecked();
        BLOG_DEBUG() << "[CredentialsPage] Saving to wizardData - savePassword:" << m_saveCheck->isChecked();
    }
    return true;
}

void CredentialsPage::onGeneratePassword()
{
    // Generate a random 24-character password
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%";
    QString clearPassword;
    for (int i = 0; i < 24; ++i) {
        int idx = QRandomGenerator::global()->bounded(chars.length());
        clearPassword.append(chars.at(idx));
    }

    // Convert to MD5 hash with [md5] prefix (Bareos format)
    QByteArray md5 = QCryptographicHash::hash(clearPassword.toLatin1(), QCryptographicHash::Md5);
    QString md5Password = QString("[md5]%1").arg(QString::fromLatin1(md5.toHex()));

    m_passwordEdit->setText(md5Password);
    emit completeChanged();
}

// ============================================================================
// AuthMethodPage
// ============================================================================

AuthMethodPage::AuthMethodPage(QWidget *parent)
    : QAPage(tr("How should we authenticate?"), parent)
{
    m_group = new QButtonGroup(this);

    m_pskRadio = new QRadioButton(tr("TLS-PSK (recommended)"), this);
    m_pskRadio->setChecked(true);
    m_group->addButton(m_pskRadio, 0);
    m_layout->addWidget(m_pskRadio);

    m_certRadio = new QRadioButton(tr("TLS with certificates"), this);
    m_group->addButton(m_certRadio, 1);
    m_layout->addWidget(m_certRadio);

    // Capability info label (shown after server check)
    m_capabilityLabel = new QLabel(this);
    m_capabilityLabel->setWordWrap(true);
    m_capabilityLabel->setStyleSheet("color: #666; font-size: 11px; margin-top: 10px;");
    m_capabilityLabel->setVisible(false);
    m_layout->addWidget(m_capabilityLabel);

    // PSK Cipher List (optional, shown only for PSK mode)
    m_cipherWidget = new QWidget(this);
    QVBoxLayout *cipherLayout = new QVBoxLayout(m_cipherWidget);
    cipherLayout->setContentsMargins(0, 10, 0, 0);

    QLabel *cipherLabel = new QLabel(tr("TLS Cipher List (optional):"), m_cipherWidget);
    cipherLabel->setStyleSheet("font-size: 11px; color: #666;");
    cipherLayout->addWidget(cipherLabel);

    QHBoxLayout *cipherInputLayout = new QHBoxLayout();
    m_cipherListEdit = new QLineEdit(m_cipherWidget);
    m_cipherListEdit->setPlaceholderText(tr("e.g., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256"));
    m_cipherListEdit->setToolTip(tr("Colon-separated list of TLS-PSK ciphers.\n"
                                     "Leave empty for auto-detection.\n"
                                     "Click Preset to use recommended ciphers."));
    cipherInputLayout->addWidget(m_cipherListEdit);

    QPushButton *presetBtn = new QPushButton(tr("Preset"), m_cipherWidget);
    presetBtn->setToolTip(tr("Fill with recommended PSK ciphers"));
    connect(presetBtn, &QPushButton::clicked, this, [this]() {
        m_cipherListEdit->setText("PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256:PSK-AES256-CBC-SHA:PSK-AES128-CBC-SHA");
    });
    cipherInputLayout->addWidget(presetBtn);
    cipherLayout->addLayout(cipherInputLayout);

    m_layout->addWidget(m_cipherWidget);

    setHint(tr("TLS-PSK is the default for Bareos 18.2+."));
    m_layout->addStretch();

    registerField("authMethod", this, "authMethod");
    registerField("tlsCipherList", m_cipherListEdit);
    connect(m_group, &QButtonGroup::idClicked, this, &AuthMethodPage::onSelectionChanged);
}

void AuthMethodPage::initializePage()
{
    // Get capabilities from wizard
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());

    // Update UI based on detected capabilities
    updateCapabilityHints();

    // Restore radio button selection from wizardData or field value
    QString auth;
    if (wiz && wiz->wizardData() && !wiz->wizardData()->authMethod.isEmpty()) {
        auth = wiz->wizardData()->authMethod;
    } else {
        auth = field("authMethod").toString();
    }

    if (auth == "x509") {
        m_certRadio->setChecked(true);
    } else {
        m_pskRadio->setChecked(true);
    }

    // Restore cipher list from wizard data
    if (wiz && wiz->wizardData() && !wiz->wizardData()->tlsCipherList.isEmpty()) {
        m_cipherListEdit->setText(wiz->wizardData()->tlsCipherList);
    }

    onSelectionChanged();  // Update hint text and cipher widget visibility
}

void AuthMethodPage::updateCapabilityHints()
{
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (!wiz) return;

    auto caps = wiz->capabilities();

    if (!caps.checked) {
        // No capability check performed yet
        m_capabilityLabel->setVisible(false);
        m_pskRadio->setEnabled(true);
        m_certRadio->setEnabled(true);
        return;
    }

    // Show capability info
    m_capabilityLabel->setVisible(true);

    if (caps.reachable) {
        if (caps.supportsPSK) {
            m_capabilityLabel->setText(tr("Server supports TLS encryption."));
            m_capabilityLabel->setStyleSheet("color: #008000; font-size: 11px; margin-top: 10px;");
        } else {
            // Server doesn't support PSK (older Bareos/Bacula?)
            m_pskRadio->setEnabled(caps.supportsPSK);

            if (!caps.supportsPSK) {
                m_pskRadio->setText(tr("TLS-PSK - not available on this server"));
                m_capabilityLabel->setText(tr("This server doesn't appear to support TLS-PSK. "
                                              "Consider upgrading to Bareos 18.2 or later."));
                m_capabilityLabel->setStyleSheet("color: #c08000; font-size: 11px; margin-top: 10px;");
            }
        }
    } else {
        // Server not reachable
        m_capabilityLabel->setText(tr("Server check failed: %1").arg(caps.lastError));
        m_capabilityLabel->setStyleSheet("color: #c00000; font-size: 11px; margin-top: 10px;");
    }
}

int AuthMethodPage::nextId() const
{
    if (m_certRadio->isChecked()) {
        return BConnectionWizard::Page_TLS;
    }
    return BConnectionWizard::Page_ConfigPreview;
}

void AuthMethodPage::onSelectionChanged()
{
    if (m_pskRadio->isChecked()) {
        setHint(tr("TLS-PSK uses your password to encrypt the connection."));
        m_hintLabel->setStyleSheet("color: gray; font-style: italic;");
        m_cipherWidget->setVisible(true);  // Show cipher list for PSK mode
    } else if (m_certRadio->isChecked()) {
        setHint(tr("You'll need CA, client certificate, and private key."));
        m_hintLabel->setStyleSheet("color: gray; font-style: italic;");
        m_cipherWidget->setVisible(false);  // Hide cipher list for cert mode
    }
    setField("authMethod", authMethod());

    // Save to wizard data struct
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        wiz->wizardData()->authMethod = authMethod();
    }
}

bool AuthMethodPage::validatePage()
{
    // Save cipher list to wizard data
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        wiz->wizardData()->tlsCipherList = m_cipherListEdit->text().trimmed();
    }
    return true;
}

QString AuthMethodPage::authMethod() const
{
    if (m_certRadio && m_certRadio->isChecked()) return "x509";
    return "psk";
}

// ============================================================================
// TLSPage
// ============================================================================

TLSPage::TLSPage(QWidget *parent)
    : QAPage(tr("Where are your TLS certificates?"), parent)
{
    auto *form = new QFormLayout();

    auto *caRow = new QHBoxLayout();
    m_caCertEdit = new QLineEdit(this);
    m_caCertEdit->setPlaceholderText(tr("/etc/bareos/ssl/ca.pem"));
    m_caBrowseButton = new QPushButton(tr("..."), this);
    m_caBrowseButton->setMaximumWidth(40);
    caRow->addWidget(m_caCertEdit);
    caRow->addWidget(m_caBrowseButton);
    form->addRow(tr("CA Cert:"), caRow);

#ifdef Q_OS_WINDOWS
    // Windows: PFX file + password
    auto *pfxRow = new QHBoxLayout();
    m_clientCertEdit = new QLineEdit(this);
    m_clientCertEdit->setPlaceholderText(tr("Path to PFX certificate file"));
    m_clientCertBrowseButton = new QPushButton(tr("..."), this);
    m_clientCertBrowseButton->setMaximumWidth(40);
    pfxRow->addWidget(m_clientCertEdit);
    pfxRow->addWidget(m_clientCertBrowseButton);
    form->addRow(tr("PFX File:"), pfxRow);

    m_clientKeyEdit = new QLineEdit(this);
    m_clientKeyEdit->setEchoMode(QLineEdit::Password);
    m_clientKeyEdit->setPlaceholderText(tr("PFX file password (if encrypted)"));
    form->addRow(tr("PFX Password:"), m_clientKeyEdit);

    registerField("clientPfx", m_clientCertEdit);
    registerField("pfxPassword", m_clientKeyEdit);
    // Also register standard field names for ConfigPreviewPage compatibility
    registerField("clientCert", m_clientCertEdit);
    registerField("clientKey", m_clientKeyEdit);
#else
    // Linux: Separate certificate and key files
    auto *certRow = new QHBoxLayout();
    m_clientCertEdit = new QLineEdit(this);
    m_clientCertEdit->setPlaceholderText(tr("/etc/bareos/ssl/client.pem"));
    m_clientCertBrowseButton = new QPushButton(tr("..."), this);
    m_clientCertBrowseButton->setMaximumWidth(40);
    certRow->addWidget(m_clientCertEdit);
    certRow->addWidget(m_clientCertBrowseButton);
    form->addRow(tr("Client Cert:"), certRow);

    auto *keyRow = new QHBoxLayout();
    m_clientKeyEdit = new QLineEdit(this);
    m_clientKeyEdit->setPlaceholderText(tr("/etc/bareos/ssl/client.key"));
    m_clientKeyBrowseButton = new QPushButton(tr("..."), this);
    m_clientKeyBrowseButton->setMaximumWidth(40);
    keyRow->addWidget(m_clientKeyEdit);
    keyRow->addWidget(m_clientKeyBrowseButton);
    form->addRow(tr("Private Key:"), keyRow);

    registerField("clientCert", m_clientCertEdit);
    registerField("clientKey", m_clientKeyEdit);
#endif

    m_layout->addLayout(form);

#ifdef Q_OS_WINDOWS
    // Convert to PFX button (Windows only)
    QPushButton *convertPfxButton = new QPushButton(tr("Convert PEM/DER to PFX..."), this);
    convertPfxButton->setToolTip(tr("Convert separate PEM/DER certificate and key files into a PFX file"));
    connect(convertPfxButton, &QPushButton::clicked, this, &TLSPage::convertToPFX);
    m_layout->addWidget(convertPfxButton);
#endif

    m_generateButton = new QPushButton(tr("Generate Certificates..."), this);
    m_layout->addWidget(m_generateButton);

    setHint(tr("No certificates? Click Generate to create them."));
    m_layout->addStretch();

    registerField("caCert", m_caCertEdit);

    connect(m_caBrowseButton, &QPushButton::clicked, this, &TLSPage::browseCaCert);
    connect(m_clientCertBrowseButton, &QPushButton::clicked, this, &TLSPage::browseClientCert);
#ifndef Q_OS_WINDOWS
    connect(m_clientKeyBrowseButton, &QPushButton::clicked, this, &TLSPage::browseClientKey);
#endif
    connect(m_generateButton, &QPushButton::clicked, this, &TLSPage::generateCertificates);
}

void TLSPage::initializePage()
{
    // Pre-fill fields from wizardData if available (e.g., from template selection)
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        BConnectionWizardData *data = wiz->wizardData();
        if (!data->tlsCaCertFile.isEmpty()) {
            m_caCertEdit->setText(data->tlsCaCertFile);
        }
        if (!data->tlsCertFile.isEmpty()) {
            m_clientCertEdit->setText(data->tlsCertFile);
        }
        if (!data->tlsKeyFile.isEmpty()) {
            m_clientKeyEdit->setText(data->tlsKeyFile);
        }
    }
}

bool TLSPage::validatePage()
{
    QString ca = m_caCertEdit->text().trimmed();

    if (ca.isEmpty()) {
        QMessageBox::warning(this, tr("Missing"), tr("Please provide a CA certificate file."));
        return false;
    }
    if (!QFile::exists(ca)) {
        QMessageBox::warning(this, tr("Not Found"), tr("CA certificate file not found."));
        return false;
    }

#ifdef Q_OS_WINDOWS
    QString pfx = m_clientCertEdit->text().trimmed();
    if (pfx.isEmpty()) {
        QMessageBox::warning(this, tr("Missing"), tr("Please provide a PFX certificate file."));
        return false;
    }
    if (!QFile::exists(pfx)) {
        QMessageBox::warning(this, tr("Not Found"), tr("PFX certificate file not found."));
        return false;
    }
#else
    QString cert = m_clientCertEdit->text().trimmed();
    QString key = m_clientKeyEdit->text().trimmed();

    if (cert.isEmpty() || key.isEmpty()) {
        QMessageBox::warning(this, tr("Missing"), tr("Please provide all certificate files."));
        return false;
    }
    if (!QFile::exists(cert) || !QFile::exists(key)) {
        QMessageBox::warning(this, tr("Not Found"), tr("One or more certificate files not found."));
        return false;
    }
#endif

    // Save to wizard data struct
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        wiz->wizardData()->tlsCaCertFile = ca;
#ifdef Q_OS_WINDOWS
        // On Windows, PFX contains cert+key - store in tlsCertFile
        wiz->wizardData()->tlsCertFile = pfx;
        wiz->wizardData()->tlsKeyFile = m_clientKeyEdit->text().trimmed();
#else
        wiz->wizardData()->tlsCertFile = cert;
        wiz->wizardData()->tlsKeyFile = key;
#endif
    }

    return true;
}

void TLSPage::browseCaCert()
{
    // Use current path from line edit, or default to bareos-certs in user's home
    QString startPath = m_caCertEdit->text().trimmed();
    if (startPath.isEmpty() || !QFileInfo(startPath).dir().exists()) {
        startPath = QDir::homePath() + "/bareos-certs";
        if (!QDir(startPath).exists())
            startPath = QDir::homePath();
    } else {
        startPath = QFileInfo(startPath).absolutePath();
    }

    QString f = QFileDialog::getOpenFileName(this, tr("CA Certificate"), startPath,
        tr("Certificates (*.pem *.crt *.cer);;All Files (*)"));
    if (!f.isEmpty()) m_caCertEdit->setText(f);
}

void TLSPage::browseClientCert()
{
    // Use current path from line edit, or default to bareos-certs in user's home
    QString startPath = m_clientCertEdit->text().trimmed();
    if (startPath.isEmpty() || !QFileInfo(startPath).dir().exists()) {
        startPath = QDir::homePath() + "/bareos-certs";
        if (!QDir(startPath).exists())
            startPath = QDir::homePath();
    } else {
        startPath = QFileInfo(startPath).absolutePath();
    }

#ifdef Q_OS_WINDOWS
    QString f = QFileDialog::getOpenFileName(this, tr("Select PFX Certificate"), startPath,
        tr("PFX Files (*.pfx);;Certificates (*.pem *.crt);;All Files (*)"));
#else
    QString f = QFileDialog::getOpenFileName(this, tr("Client Certificate"), startPath,
        tr("Certificates (*.pem *.crt *.cer);;All Files (*)"));
#endif
    if (!f.isEmpty()) m_clientCertEdit->setText(f);
}

void TLSPage::browseClientKey()
{
    // Use current path from line edit, or default to bareos-certs in user's home
    QString startPath = m_clientKeyEdit->text().trimmed();
    if (startPath.isEmpty() || !QFileInfo(startPath).dir().exists()) {
        startPath = QDir::homePath() + "/bareos-certs";
        if (!QDir(startPath).exists())
            startPath = QDir::homePath();
    } else {
        startPath = QFileInfo(startPath).absolutePath();
    }

    QString f = QFileDialog::getOpenFileName(this, tr("Private Key"), startPath,
        tr("Private Keys (*.pem *.key);;All Files (*)"));
    if (!f.isEmpty()) m_clientKeyEdit->setText(f);
}

void TLSPage::generateCertificates()
{
    BCertificateGenerator dialog(this);
    if (dialog.exec() == QDialog::Accepted && dialog.isSuccessful()) {
        m_caCertEdit->setText(dialog.caCertPath());
#ifdef Q_OS_WINDOWS
        // On Windows, use PFX file for client certificate
        if (!dialog.pfxPath().isEmpty()) {
            m_clientCertEdit->setText(dialog.pfxPath());
        } else {
            m_clientCertEdit->setText(dialog.clientCertPath());
        }
        // Key is embedded in PFX, but also set the path for reference
        m_clientKeyEdit->setText(dialog.clientKeyPath());
#else
        m_clientCertEdit->setText(dialog.clientCertPath());
        m_clientKeyEdit->setText(dialog.clientKeyPath());
#endif
    }
}

void TLSPage::convertToPFX()
{
    BPFXConverter converter(this);
    if (converter.exec() == QDialog::Accepted) {
        QString pfxPath = converter.getPFXFilePath();
        if (!pfxPath.isEmpty()) {
            m_clientCertEdit->setText(pfxPath);
            QMessageBox::information(this, tr("PFX File Created"),
                tr("PFX file has been created successfully.\n\n"
                   "The certificate path has been updated."));
        }
    }
}

// ============================================================================
// ConfigPreviewPage
// ============================================================================

ConfigPreviewPage::ConfigPreviewPage(QWidget *parent)
    : QAPage(tr("Server Configuration Preview"), parent)
{
    m_questionLabel->setText(tr("Copy this configuration to your Bareos Director"));

    auto *tabs = new QTabWidget(this);

    // Console config tab
    auto *consoleWidget = new QWidget();
    auto *consoleLayout = new QVBoxLayout(consoleWidget);
    consoleLayout->setContentsMargins(5, 5, 5, 5);

    auto *consoleLabel = new QLabel(tr("Console resource for the Director:"), consoleWidget);
    consoleLabel->setStyleSheet("font-weight: bold;");
    consoleLayout->addWidget(consoleLabel);

    auto *consolePath = new QLabel(tr("Path: /etc/bareos/bareos-dir.d/console/&lt;name&gt;.conf"), consoleWidget);
    consolePath->setStyleSheet("color: #666; font-size: 10px;");
    consoleLayout->addWidget(consolePath);

    m_consoleConfigEdit = new QTextEdit(consoleWidget);
    m_consoleConfigEdit->setReadOnly(true);
    m_consoleConfigEdit->setFont(QFont("monospace", 9));
    m_consoleConfigEdit->setMinimumHeight(150);
    consoleLayout->addWidget(m_consoleConfigEdit);

    auto *consoleButtonLayout = new QHBoxLayout();
    m_copyConsoleButton = new QPushButton(tr("Copy to Clipboard"), consoleWidget);
    m_copyConsoleButton->setIcon(QIcon::fromTheme("edit-copy"));
    connect(m_copyConsoleButton, &QPushButton::clicked, this, &ConfigPreviewPage::onCopyConsoleConfig);
    consoleButtonLayout->addWidget(m_copyConsoleButton);

    m_saveConsoleButton = new QPushButton(tr("Save .conf"), consoleWidget);
    m_saveConsoleButton->setIcon(QIcon::fromTheme("document-save"));
    connect(m_saveConsoleButton, &QPushButton::clicked, this, &ConfigPreviewPage::onSaveConsoleConf);
    consoleButtonLayout->addWidget(m_saveConsoleButton);

    m_saveConsoleZipButton = new QPushButton(tr("Save ZIP"), consoleWidget);
    m_saveConsoleZipButton->setIcon(QIcon::fromTheme("package-x-generic"));
    connect(m_saveConsoleZipButton, &QPushButton::clicked, this, &ConfigPreviewPage::onSaveConsoleZip);
    consoleButtonLayout->addWidget(m_saveConsoleZipButton);

    consoleButtonLayout->addStretch();
    consoleLayout->addLayout(consoleButtonLayout);

    tabs->addTab(consoleWidget, tr("Console Config"));

    // Director hint tab (optional settings for Director resource)
    auto *directorWidget = new QWidget();
    auto *directorLayout = new QVBoxLayout(directorWidget);
    directorLayout->setContentsMargins(5, 5, 5, 5);

    auto *directorLabel = new QLabel(tr("Director resource TLS settings (if needed):"), directorWidget);
    directorLabel->setStyleSheet("font-weight: bold;");
    directorLayout->addWidget(directorLabel);

    auto *directorPath = new QLabel(tr("Path: /etc/bareos/bareos-dir.d/director/bareos-dir.conf"), directorWidget);
    directorPath->setStyleSheet("color: #666; font-size: 10px;");
    directorLayout->addWidget(directorPath);

    m_directorConfigEdit = new QTextEdit(directorWidget);
    m_directorConfigEdit->setReadOnly(true);
    m_directorConfigEdit->setFont(QFont("monospace", 9));
    m_directorConfigEdit->setMinimumHeight(150);
    directorLayout->addWidget(m_directorConfigEdit);

    auto *directorButtonLayout = new QHBoxLayout();
    m_copyDirectorButton = new QPushButton(tr("Copy to Clipboard"), directorWidget);
    m_copyDirectorButton->setIcon(QIcon::fromTheme("edit-copy"));
    connect(m_copyDirectorButton, &QPushButton::clicked, this, &ConfigPreviewPage::onCopyDirectorConfig);
    directorButtonLayout->addWidget(m_copyDirectorButton);

    m_saveDirectorButton = new QPushButton(tr("Save .conf"), directorWidget);
    m_saveDirectorButton->setIcon(QIcon::fromTheme("document-save"));
    connect(m_saveDirectorButton, &QPushButton::clicked, this, &ConfigPreviewPage::onSaveDirectorConf);
    directorButtonLayout->addWidget(m_saveDirectorButton);

    m_saveDirectorZipButton = new QPushButton(tr("Save ZIP"), directorWidget);
    m_saveDirectorZipButton->setIcon(QIcon::fromTheme("package-x-generic"));
    connect(m_saveDirectorZipButton, &QPushButton::clicked, this, &ConfigPreviewPage::onSaveDirectorZip);
    directorButtonLayout->addWidget(m_saveDirectorZipButton);

    directorButtonLayout->addStretch();
    directorLayout->addLayout(directorButtonLayout);

    tabs->addTab(directorWidget, tr("Director TLS"));

    m_layout->addWidget(tabs);

    setHint(tr("After copying, paste this into your Bareos Director config and restart bareos-dir."));
}

void ConfigPreviewPage::initializePage()
{
    updateConfigs();
}

int ConfigPreviewPage::nextId() const
{
    return BConnectionWizard::Page_Test;
}

void ConfigPreviewPage::updateConfigs()
{
    // Build a temporary profile with current wizard values
    QString consoleName = field("consoleName").toString();
    QString password = field("password").toString();
    QString authMethod = field("authMethod").toString();

    // Generate Console config
    QString consoleConfig;
    consoleConfig += QString("Console {\n");
    consoleConfig += QString("  Name = \"%1\"\n").arg(consoleName);
    consoleConfig += QString("  Password = \"%1\"\n").arg(password);

    if (authMethod == "x509") {
        // Certificate mode
        consoleConfig += QString("  TLS Enable = yes\n");
        consoleConfig += QString("  TLS Require = yes\n");
        consoleConfig += QString("  TLS Verify Peer = yes\n");
        QString caCert = field("caCert").toString();
        QString clientCert = field("clientCert").toString();
        QString clientKey = field("clientKey").toString();
#ifdef Q_OS_WINDOWS
        // Windows uses PFX files - also check clientPfx field
        if (clientCert.isEmpty())
            clientCert = field("clientPfx").toString();
#endif
        if (!caCert.isEmpty())
            consoleConfig += QString("  TLS CA Certificate File = \"%1\"\n").arg(caCert);
        if (!clientCert.isEmpty()) {
#ifdef Q_OS_WINDOWS
            // Windows: PFX contains both certificate and key
            if (clientCert.endsWith(".pfx", Qt::CaseInsensitive)) {
                consoleConfig += QString("  TLS Certificate = \"%1\"\n").arg(clientCert);
                // PFX password is handled separately by the application
            } else {
                consoleConfig += QString("  TLS Certificate = \"%1\"\n").arg(clientCert);
                if (!clientKey.isEmpty())
                    consoleConfig += QString("  TLS Key = \"%1\"\n").arg(clientKey);
            }
#else
            consoleConfig += QString("  TLS Certificate = \"%1\"\n").arg(clientCert);
            if (!clientKey.isEmpty())
                consoleConfig += QString("  TLS Key = \"%1\"\n").arg(clientKey);
#endif
        }
    } else {
        // TLS-PSK mode (default)
        consoleConfig += QString("  TLS Enable = yes\n");
        consoleConfig += QString("  TLS Require = no\n");
        consoleConfig += QString("  TLS Verify Peer = no\n");
        // Add cipher list if specified
        QString cipherList = field("tlsCipherList").toString();
        if (!cipherList.isEmpty()) {
            consoleConfig += QString("  TLS Cipher List = \"%1\"\n").arg(cipherList);
        }
    }

    consoleConfig += QString("\n  # ACLs - adjust as needed\n");
    consoleConfig += QString("  CommandACL = *all*\n");
    consoleConfig += QString("  ClientAcl = *all*\n");
    consoleConfig += QString("  JobAcl = *all*\n");
    consoleConfig += QString("  StorageAcl = *all*\n");
    consoleConfig += QString("  ScheduleAcl = *all*\n");
    consoleConfig += QString("  PoolAcl = *all*\n");
    consoleConfig += QString("  FileSetAcl = *all*\n");
    consoleConfig += QString("  CatalogAcl = *all*\n");
    consoleConfig += QString("  WhereAcl = *all*\n");
    consoleConfig += QString("}\n");

    m_consoleConfigEdit->setPlainText(consoleConfig);

    // Generate Director TLS hints
    QString directorConfig;
    directorConfig += QString("# Add/modify these TLS settings in your Director resource:\n\n");

    if (authMethod == "x509") {
        directorConfig += QString("Director {\n");
        directorConfig += QString("  # ... other settings ...\n");
        directorConfig += QString("  TLS Enable = yes\n");
        directorConfig += QString("  TLS Require = yes\n");
        directorConfig += QString("  TLS CA Certificate File = \"/etc/bareos/ssl/ca.pem\"\n");
        directorConfig += QString("  TLS Certificate = \"/etc/bareos/ssl/bareos-dir.pem\"\n");
        directorConfig += QString("  TLS Key = \"/etc/bareos/ssl/bareos-dir-key.pem\"\n");
        directorConfig += QString("}\n");
    } else {
        // TLS-PSK mode (default)
        directorConfig += QString("Director {\n");
        directorConfig += QString("  # ... other settings ...\n");
        directorConfig += QString("  TLS Enable = yes\n");
        directorConfig += QString("  TLS Require = no      # Allow both TLS and non-TLS\n");
        // Add cipher list if specified
        QString cipherList = field("tlsCipherList").toString();
        if (!cipherList.isEmpty()) {
            directorConfig += QString("  TLS Cipher List = \"%1\"\n").arg(cipherList);
        }
        directorConfig += QString("  # TLS-PSK uses the Console password for encryption\n");
        directorConfig += QString("}\n");
    }

    directorConfig += QString("\n# After changes, restart the Director:\n");
    directorConfig += QString("# sudo systemctl restart bareos-dir\n");

    m_directorConfigEdit->setPlainText(directorConfig);
}

void ConfigPreviewPage::onCopyConsoleConfig()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_consoleConfigEdit->toPlainText());

    m_copyConsoleButton->setText(tr("Copied!"));
    QTimer::singleShot(2000, this, [this]() {
        m_copyConsoleButton->setText(tr("Copy to Clipboard"));
    });
}

void ConfigPreviewPage::onCopyDirectorConfig()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_directorConfigEdit->toPlainText());

    m_copyDirectorButton->setText(tr("Copied!"));
    QTimer::singleShot(2000, this, [this]() {
        m_copyDirectorButton->setText(tr("Copy to Clipboard"));
    });
}

void ConfigPreviewPage::onSaveConsoleConf()
{
    QString consoleName = field("consoleName").toString();
    QString defaultName = QString("%1.conf").arg(consoleName.isEmpty() ? "console" : consoleName);

    QString filePath = QFileDialog::getSaveFileName(
        this, tr("Save Console Configuration"), defaultName,
        tr("Configuration Files (*.conf);;All Files (*)"));
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Save Failed"),
                             tr("Could not write to %1").arg(filePath));
        return;
    }

    file.write(m_consoleConfigEdit->toPlainText().toUtf8());
    file.close();

    m_saveConsoleButton->setText(tr("Saved!"));
    QTimer::singleShot(2000, this, [this]() {
        m_saveConsoleButton->setText(tr("Save .conf"));
    });
}

void ConfigPreviewPage::onSaveConsoleZip()
{
    QString consoleName = field("consoleName").toString();
    QString defaultName = QString("console-%1.zip").arg(consoleName.isEmpty() ? "config" : consoleName);

    QString filePath = QFileDialog::getSaveFileName(
        this, tr("Save Console Configuration as ZIP"), defaultName,
        tr("ZIP Archives (*.zip);;All Files (*)"));
    if (filePath.isEmpty()) return;

    if (!filePath.endsWith(".zip", Qt::CaseInsensitive))
        filePath += ".zip";

    QZipWriter zip(filePath);
    if (zip.status() != QZipWriter::NoError) {
        QMessageBox::warning(this, tr("Save Failed"),
                             tr("Could not create ZIP file: %1").arg(filePath));
        return;
    }

    // Create Bareos directory structure
    zip.addDirectory("etc");
    zip.addDirectory("etc/bareos");
    zip.addDirectory("etc/bareos/bareos-dir.d");
    zip.addDirectory("etc/bareos/bareos-dir.d/console");

    // Add console config
    QString confName = QString("%1.conf").arg(consoleName.isEmpty() ? "onesimus" : consoleName);
    QString confPath = QString("etc/bareos/bareos-dir.d/console/%1").arg(confName);
    zip.addFile(confPath, m_consoleConfigEdit->toPlainText().toUtf8());

    zip.close();

    m_saveConsoleZipButton->setText(tr("Saved!"));
    QTimer::singleShot(2000, this, [this]() {
        m_saveConsoleZipButton->setText(tr("Save ZIP"));
    });
}

void ConfigPreviewPage::onSaveDirectorConf()
{
    QString defaultName = "bareos-dir-tls.conf";

    QString filePath = QFileDialog::getSaveFileName(
        this, tr("Save Director TLS Configuration"), defaultName,
        tr("Configuration Files (*.conf);;All Files (*)"));
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Save Failed"),
                             tr("Could not write to %1").arg(filePath));
        return;
    }

    file.write(m_directorConfigEdit->toPlainText().toUtf8());
    file.close();

    m_saveDirectorButton->setText(tr("Saved!"));
    QTimer::singleShot(2000, this, [this]() {
        m_saveDirectorButton->setText(tr("Save .conf"));
    });
}

void ConfigPreviewPage::onSaveDirectorZip()
{
    QString defaultName = "director-tls-config.zip";

    QString filePath = QFileDialog::getSaveFileName(
        this, tr("Save Director TLS Configuration as ZIP"), defaultName,
        tr("ZIP Archives (*.zip);;All Files (*)"));
    if (filePath.isEmpty()) return;

    if (!filePath.endsWith(".zip", Qt::CaseInsensitive))
        filePath += ".zip";

    QZipWriter zip(filePath);
    if (zip.status() != QZipWriter::NoError) {
        QMessageBox::warning(this, tr("Save Failed"),
                             tr("Could not create ZIP file: %1").arg(filePath));
        return;
    }

    // Create Bareos directory structure
    zip.addDirectory("etc");
    zip.addDirectory("etc/bareos");
    zip.addDirectory("etc/bareos/bareos-dir.d");
    zip.addDirectory("etc/bareos/bareos-dir.d/director");

    // Add director TLS hints config
    QString confPath = "etc/bareos/bareos-dir.d/director/tls-hints.conf";
    zip.addFile(confPath, m_directorConfigEdit->toPlainText().toUtf8());

    zip.close();

    m_saveDirectorZipButton->setText(tr("Saved!"));
    QTimer::singleShot(2000, this, [this]() {
        m_saveDirectorZipButton->setText(tr("Save ZIP"));
    });
}

// ============================================================================
// TestPage
// ============================================================================

TestPage::TestPage(QWidget *parent)
    : QWizardPage(parent), m_testDirector(nullptr), m_testSucceeded(false), m_testRunning(false)
{
    setTitle(tr("Test Connection"));

    auto *layout = new QVBoxLayout(this);

    auto *label = new QLabel(tr("Let's test your connection."), this);
    QFont f = label->font();
    f.setPointSize(f.pointSize() + 2);
    label->setFont(f);
    layout->addWidget(label);

    m_statusLabel = new QLabel(tr("Click the button to test."), this);
    m_statusLabel->setStyleSheet("color: gray;");
    layout->addWidget(m_statusLabel);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0);
    m_progressBar->setVisible(false);
    layout->addWidget(m_progressBar);

    m_testButton = new QPushButton(tr("Test Connection"), this);
    m_testButton->setMinimumHeight(40);
    layout->addWidget(m_testButton);

    m_logEdit = new QTextEdit(this);
    m_logEdit->setReadOnly(true);
    m_logEdit->setMaximumHeight(120);
    m_logEdit->setVisible(false);
    layout->addWidget(m_logEdit);

    layout->addStretch();

    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, &QTimer::timeout, this, &TestPage::onTimeout);
    connect(m_testButton, &QPushButton::clicked, this, &TestPage::onTestClicked);
}

TestPage::~TestPage() { stopTest(); }

void TestPage::initializePage()
{
    m_testSucceeded = false;
    m_testRunning = false;
    m_logEdit->clear();
    m_logEdit->setVisible(false);
    m_progressBar->setVisible(false);
    m_statusLabel->setText(tr("Click the button to test."));
    m_statusLabel->setStyleSheet("color: gray;");
    m_testButton->setEnabled(true);
    m_testButton->setText(tr("Test Connection"));
}

void TestPage::cleanupPage() { stopTest(); }
bool TestPage::isComplete() const { return m_testSucceeded; }
int TestPage::nextId() const
{
    // Always go to ProfileName after successful test
    // Console selection already done in CredentialsPage or ImportConsoleSelectionPage
    return BConnectionWizard::Page_ProfileName;
}

void TestPage::startTest()
{
    m_testRunning = true;
    m_testSucceeded = false;
    m_logEdit->clear();
    m_logEdit->setVisible(true);
    m_progressBar->setVisible(true);
    m_testButton->setEnabled(false);
    m_statusLabel->setText(tr("Connecting..."));
    m_statusLabel->setStyleSheet("color: black;");

    appendLog(tr("Starting test..."));

    if (m_testDirector) delete m_testDirector;
    m_testDirector = new BareosDirector(this);
    m_testDirector->initialize();

    connect(m_testDirector, &BareosDirector::connectionStateChanged, this, &TestPage::onStateChanged);
    connect(m_testDirector, &BareosDirector::authentificationSucceeded, this, &TestPage::onAuthResult);
    connect(m_testDirector, &BareosDirector::protocolError, this, &TestPage::onProtocolError);
    connect(m_testDirector, &BareosDirector::allResourcesLoaded, this, &TestPage::onResourcesLoaded);

    BareosDirector::TLSConfig tls;
    QString auth = field("authMethod").toString();

    if (auth == "x509") {
        tls.tlsEnable = true;
        tls.tlsRequire = true;
        tls.tlsPSKEnable = false;
        tls.tlsVerifyPeer = true;
        QString ca = field("caCert").toString();
        if (!ca.isEmpty()) tls.tlsCaCertFile = QSharedPointer<QFile>(new QFile(ca));
#ifdef Q_OS_WINDOWS
        QString pfx = field("clientPfx").toString();
        QString pfxPass = field("pfxPassword").toString();
        if (!pfx.isEmpty()) tls.tlsPfxFile = QSharedPointer<QFile>(new QFile(pfx));
        if (!pfxPass.isEmpty()) tls.tlsPfxPassword = pfxPass;
#else
        QString cert = field("clientCert").toString();
        QString key = field("clientKey").toString();
        if (!cert.isEmpty()) tls.tlsCertFile = QSharedPointer<QFile>(new QFile(cert));
        if (!key.isEmpty()) tls.tlsKeyFile = QSharedPointer<QFile>(new QFile(key));
#endif
        appendLog(tr("Mode: TLS-Cert"));
    } else {
        // TLS-PSK mode (default)
        tls.tlsEnable = true;
        tls.tlsRequire = true;
        tls.tlsPSKEnable = true;
        tls.tlsVerifyPeer = false;

        // Apply cipher list if specified
        QString cipherList = field("tlsCipherList").toString();
        if (!cipherList.isEmpty()) {
            tls.tlsCipherList = cipherList;
            appendLog(tr("Mode: TLS-PSK with cipher list: %1").arg(cipherList));
        } else {
            appendLog(tr("Mode: TLS-PSK"));
        }
    }

    m_testDirector->setTLSConfig(tls);

    QString host = field("host").toString();
    int port = field("port").toInt();
    QString dir = field("directorName").toString();
    QString con = field("consoleName").toString();
    QString pwd = field("password").toString();

    appendLog(tr("Connecting to %1:%2...").arg(host).arg(port));
    m_testDirector->connect(host, port, dir, con, pwd);
    m_timeoutTimer->start(30000);
}

void TestPage::stopTest()
{
    m_timeoutTimer->stop();
    if (m_testDirector) {
        m_testDirector->disconnect();
        m_testDirector->deleteLater();
        m_testDirector = nullptr;
    }
    m_testRunning = false;
    m_progressBar->setVisible(false);
    m_testButton->setEnabled(true);
    m_testButton->setText(tr("Test Connection"));
}

void TestPage::appendLog(const QString &msg, bool err)
{
    // Use palette-aware colors for dark/light theme compatibility
    QString color;
    if (err) {
        color = "#dc3545";  // Bootstrap red - visible in both themes
    } else {
        // Use the window text color from the palette
        QColor textColor = palette().color(QPalette::WindowText);
        color = textColor.name();
    }
    m_logEdit->append(QString("<span style='color:%1;'>%2</span>").arg(color, msg));
}

void TestPage::onTestClicked()
{
    if (m_testRunning) { stopTest(); appendLog(tr("Cancelled."), true); }
    else { startTest(); }
}

void TestPage::onStateChanged(int, int newState)
{
    QString s;
    switch (static_cast<BareosDirector::ConnectionState>(newState)) {
    case BareosDirector::Connecting: s = tr("Connecting"); break;
    case BareosDirector::Authenticating: s = tr("Authenticating"); break;
    case BareosDirector::SettingApiMode: s = tr("Setting API"); break;
    case BareosDirector::LoadingResources: s = tr("Loading"); break;
    case BareosDirector::Ready: s = tr("Ready"); break;
    default: return;
    }
    appendLog(s);
}

void TestPage::onAuthResult(bool ok, const QString &msg)
{
    if (ok) appendLog(tr("Auth OK"));
    else { appendLog(tr("Auth failed: %1").arg(msg), true); m_statusLabel->setText(tr("Failed")); m_statusLabel->setStyleSheet("color:#c00000;font-weight:bold;"); stopTest(); }
}

void TestPage::onProtocolError(const QString &e)
{
    appendLog(tr("Error: %1").arg(e), true);
    m_statusLabel->setText(tr("Failed"));
    m_statusLabel->setStyleSheet("color:#c00000;font-weight:bold;");
    stopTest();
}

void TestPage::onResourcesLoaded()
{
    m_timeoutTimer->stop();
    m_testSucceeded = true;
    m_testRunning = false;
    appendLog(tr("Success!"));
    m_statusLabel->setText(tr("Connection successful!"));
    m_statusLabel->setStyleSheet("color:#008000;font-weight:bold;");
    m_progressBar->setVisible(false);
    m_testButton->setText(tr("Test Again"));
    m_testButton->setEnabled(true);
    if (m_testDirector) m_testDirector->disconnect();
    emit completeChanged();
}

void TestPage::onTimeout()
{
    appendLog(tr("Timeout"), true);
    m_statusLabel->setText(tr("Timed out"));
    m_statusLabel->setStyleSheet("color:#c00000;font-weight:bold;");
    stopTest();
}

// ============================================================================
// ConsoleSetupPage
// ============================================================================

ConsoleSetupPage::ConsoleSetupPage(QWidget *parent)
    : QAPage(tr("Console configuration"), parent)
    , m_director(nullptr)
    , m_consolesLoaded(false)
{
    m_group = new QButtonGroup(this);
    m_layout->setSpacing(10);

    // Option 1: Modify existing console
    m_modifyExistingRadio = new QRadioButton(tr("Modify existing console"), this);
    m_modifyExistingRadio->setChecked(true);
    m_group->addButton(m_modifyExistingRadio, 0);
    m_layout->addWidget(m_modifyExistingRadio);

    // Console selection row
    auto *selectLayout = new QHBoxLayout();
    selectLayout->setContentsMargins(25, 5, 0, 5);
    selectLayout->setSpacing(10);
    m_consoleCombo = new QComboBox(this);
    m_consoleCombo->setMinimumWidth(250);
    m_consoleCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    selectLayout->addWidget(m_consoleCombo, 1);
    m_refreshButton = new QPushButton(tr("Refresh"), this);
    m_refreshButton->setMinimumWidth(80);
    selectLayout->addWidget(m_refreshButton);
    m_layout->addLayout(selectLayout);

    // Console details display (shown when selecting existing console)
    m_consoleDetails = new QTextEdit(this);
    m_consoleDetails->setReadOnly(true);
    m_consoleDetails->setMinimumHeight(80);
    m_consoleDetails->setMaximumHeight(120);
    m_consoleDetails->setPlaceholderText(tr("Select a console to view its configuration..."));
    m_consoleDetails->setStyleSheet("font-family: monospace; font-size: 9pt;");
    auto *detailsLayout = new QHBoxLayout();
    detailsLayout->setContentsMargins(25, 0, 0, 10);
    detailsLayout->addWidget(m_consoleDetails);
    m_layout->addLayout(detailsLayout);

    // Option 2: Create new console
    m_createNewRadio = new QRadioButton(tr("Create new console"), this);
    m_group->addButton(m_createNewRadio, 1);
    m_layout->addWidget(m_createNewRadio);

    auto *createForm = new QFormLayout();
    createForm->setContentsMargins(25, 5, 0, 5);
    createForm->setSpacing(8);
    createForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    m_newNameEdit = new QLineEdit(this);
    m_newNameEdit->setText("onesimus-client");
    m_newNameEdit->setEnabled(false);
    createForm->addRow(tr("Name:"), m_newNameEdit);

    auto *pwdLayout = new QHBoxLayout();
    pwdLayout->setSpacing(10);
    m_newPasswordEdit = new QLineEdit(this);
    m_newPasswordEdit->setEnabled(false);
    pwdLayout->addWidget(m_newPasswordEdit, 1);
    m_generatePasswordButton = new QPushButton(tr("Generate"), this);
    m_generatePasswordButton->setEnabled(false);
    m_generatePasswordButton->setMinimumWidth(80);
    pwdLayout->addWidget(m_generatePasswordButton);
    createForm->addRow(tr("Password:"), pwdLayout);
    m_layout->addLayout(createForm);

    m_layout->addSpacing(10);

    // Security warning about console ACLs
    auto *securityWarning = new QLabel(this);
    securityWarning->setWordWrap(true);
    securityWarning->setText(tr("Security notice: Consoles with overly wide ACLs (e.g. *all*) "
                                "grant full access to all backup operations, including deletion. "
                                "Ask your backup administrator to create a Console resource "
                                "with restricted ACLs tailored to your needs."));
    securityWarning->setStyleSheet("color: #c08000; font-size: 10px; padding: 8px; "
                                   "border: 1px solid #c08000; border-radius: 4px; "
                                   "background-color: rgba(192, 128, 0, 30);");
    m_layout->addWidget(securityWarning);

    m_layout->addSpacing(5);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("color: gray; font-style: italic;");
    m_layout->addWidget(m_statusLabel);

    setHint(tr("Modify an existing console or create a new one on the director."));
    m_layout->addStretch();

    connect(m_group, &QButtonGroup::idClicked, this, &ConsoleSetupPage::onSelectionChanged);
    connect(m_refreshButton, &QPushButton::clicked, this, &ConsoleSetupPage::onRefreshClicked);
    connect(m_generatePasswordButton, &QPushButton::clicked, this, &ConsoleSetupPage::onGeneratePasswordClicked);
    connect(m_consoleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ConsoleSetupPage::onConsoleSelected);
}

void ConsoleSetupPage::initializePage()
{
    m_consolesLoaded = false;
    m_consoleCombo->clear();
    m_consoleDetails->clear();
    m_statusLabel->clear();

    // Set up initial visibility based on selection
    bool modifyMode = m_modifyExistingRadio->isChecked();
    m_consoleDetails->setVisible(modifyMode);

    // Auto-load consoles when page is shown
    loadConsoles();
}

bool ConsoleSetupPage::validatePage()
{
    if (m_modifyExistingRadio->isChecked()) {
        if (m_consoleCombo->currentText().isEmpty()) {
            QMessageBox::warning(this, tr("No Console"), tr("Please select a console."));
            return false;
        }
        // Update wizard fields with selected console
        wizard()->setField("consoleName", m_consoleCombo->currentText());
        // Note: password remains the one entered, user must know it
    } else if (m_createNewRadio->isChecked()) {
        if (m_newNameEdit->text().trimmed().isEmpty() || m_newPasswordEdit->text().isEmpty()) {
            QMessageBox::warning(this, tr("Missing"), tr("Please enter a name and password for the new console."));
            return false;
        }
        // Create the console on the director
        createConsole();
        // Update wizard fields with new console
        wizard()->setField("consoleName", m_newNameEdit->text().trimmed());
        wizard()->setField("password", m_newPasswordEdit->text());
    }
    return true;
}

bool ConsoleSetupPage::isComplete() const
{
    if (m_modifyExistingRadio->isChecked()) return !m_consoleCombo->currentText().isEmpty();
    if (m_createNewRadio->isChecked()) return !m_newNameEdit->text().trimmed().isEmpty() && !m_newPasswordEdit->text().isEmpty();
    return false;
}

void ConsoleSetupPage::onSelectionChanged()
{
    bool modifyMode = m_modifyExistingRadio->isChecked();
    bool createMode = m_createNewRadio->isChecked();

    m_consoleCombo->setEnabled(modifyMode);
    m_refreshButton->setEnabled(modifyMode);
    m_consoleDetails->setVisible(modifyMode);
    m_newNameEdit->setEnabled(createMode);
    m_newPasswordEdit->setEnabled(createMode);
    m_generatePasswordButton->setEnabled(createMode);

    emit completeChanged();
}

void ConsoleSetupPage::onRefreshClicked()
{
    loadConsoles();
}

void ConsoleSetupPage::onGeneratePasswordClicked()
{
    generatePassword();
}

void ConsoleSetupPage::onConsoleSelected(int index)
{
    if (index < 0 || m_consoleCombo->currentText().isEmpty()) {
        m_consoleDetails->clear();
        return;
    }

    // Fetch details for the selected console
    loadConsoleDetails(m_consoleCombo->currentText());
    emit completeChanged();
}

BareosDirector::TLSConfig ConsoleSetupPage::buildTLSConfig() const
{
    BareosDirector::TLSConfig tls;
    QString auth = field("authMethod").toString();
    if (auth == "x509") {
        tls.tlsEnable = true;
        tls.tlsRequire = true;
        tls.tlsPSKEnable = false;
        tls.tlsVerifyPeer = true;
        QString ca = field("caCert").toString();
        if (!ca.isEmpty()) tls.tlsCaCertFile = QSharedPointer<QFile>(new QFile(ca));
#ifdef Q_OS_WINDOWS
        QString pfx = field("clientPfx").toString();
        QString pfxPass = field("pfxPassword").toString();
        if (!pfx.isEmpty()) tls.tlsPfxFile = QSharedPointer<QFile>(new QFile(pfx));
        if (!pfxPass.isEmpty()) tls.tlsPfxPassword = pfxPass;
#else
        QString cert = field("clientCert").toString();
        QString key = field("clientKey").toString();
        if (!cert.isEmpty()) tls.tlsCertFile = QSharedPointer<QFile>(new QFile(cert));
        if (!key.isEmpty()) tls.tlsKeyFile = QSharedPointer<QFile>(new QFile(key));
#endif
    } else {
        // TLS-PSK mode (default)
        tls.tlsEnable = true;
        tls.tlsRequire = true;
        tls.tlsPSKEnable = true;
        tls.tlsVerifyPeer = false;

        // Apply cipher list if specified
        QString cipherList = field("tlsCipherList").toString();
        if (!cipherList.isEmpty()) {
            tls.tlsCipherList = cipherList;
        }
    }
    return tls;
}

BareosDirector *ConsoleSetupPage::createConnectedDirector()
{
    auto *dir = new BareosDirector(this);
    dir->initialize();
    dir->setTLSConfig(buildTLSConfig());

    QObject::connect(dir, &BareosDirector::protocolError, this, [this](const QString &err) {
        m_statusLabel->setText(tr("Error: %1").arg(err));
        m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
    });

    QString host = field("host").toString();
    int port = field("port").toInt();
    QString dirName = field("directorName").toString();
    QString con = field("consoleName").toString();
    QString pwd = field("password").toString();
    dir->connect(host, port, dirName, con, pwd);

    return dir;
}

void ConsoleSetupPage::loadConsoleDetails(const QString &name)
{
    if (name.isEmpty()) return;

    m_consoleDetails->setPlainText(tr("Loading details for %1...").arg(name));

    // Reuse existing connection if ready (signal is already connected in loadConsoles)
    if (m_director && m_director->connectionState() == BareosDirector::Ready) {
        m_director->queryShowConsole(name);
        return;
    }

    // No connection available yet - details will load when console is selected after consoles load
    m_consoleDetails->setPlainText(tr("Waiting for connection..."));
}

void ConsoleSetupPage::loadConsoles()
{
    m_consoleCombo->clear();
    m_statusLabel->setStyleSheet("");

    // Reuse existing connection if already in Ready state
    if (m_director && m_director->connectionState() == BareosDirector::Ready) {
        m_statusLabel->setText(tr("Loading consoles..."));
        m_director->queryConsoles();
        return;
    }

    // Otherwise, create a new connection
    m_statusLabel->setText(tr("Connecting to Director..."));

    if (m_director) {
        m_director->disconnect();
        m_director->deleteLater();
        m_director = nullptr;
    }

    m_director = createConnectedDirector();

    // Connect to consolesResult to receive the list
    QObject::connect(m_director, &BareosDirector::consolesResult,
                     this, &ConsoleSetupPage::onConsolesResult);

    // Connect to showConsoleResult for console details queries
    QObject::connect(m_director, &BareosDirector::showConsoleResult,
                     this, [this](const QString &, const QJsonObject &console) {
        onShowConsoleResult(console.value("name").toString(), console);
    });

    // Handle authentication result
    QObject::connect(m_director, &BareosDirector::authentificationSucceeded,
                     this, [this](bool success, const QString &msg) {
        if (success) {
            m_statusLabel->setText(tr("Loading resources..."));
        } else {
            m_statusLabel->setText(tr("Authentication failed: %1").arg(msg));
            m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
        }
    });

    // Query consoles after all resources are loaded (Ready state)
    // This ensures m_lastCommand won't be overwritten by resource loading commands
    QObject::connect(m_director, &BareosDirector::allResourcesLoaded,
                     this, [this]() {
        m_statusLabel->setText(tr("Loading consoles..."));
        m_director->queryConsoles();
    });

    // Handle connection errors
    QObject::connect(m_director, &BareosDirector::connectionStateChanged,
                     this, [this](BareosDirector::ConnectionState, BareosDirector::ConnectionState newState) {
        if (newState == BareosDirector::Disconnected) {
            if (m_consoleCombo->count() == 0 && !m_consolesLoaded) {
                m_statusLabel->setText(tr("Connection lost"));
                m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
            }
        }
    });

    // Handle protocol errors
    QObject::connect(m_director, &BareosDirector::protocolError,
                     this, [this](const QString &err) {
        m_statusLabel->setText(tr("Error: %1").arg(err));
        m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
    });
}

void ConsoleSetupPage::onConsolesResult(const QJsonArray &consoles)
{
    m_consoleCombo->clear();
    bool hasUserAgent = false;

    for (const QJsonValue &v : consoles) {
        QString name = v.toObject()["name"].toString();
        if (name == "*UserAgent*") {
            hasUserAgent = true;
            continue;
        }
        if (!name.isEmpty()) {
            m_consoleCombo->addItem(name);
        }
    }

    m_consolesLoaded = true;

    if (m_consoleCombo->count() == 0 && hasUserAgent) {
        m_statusLabel->setText(tr("No named consoles found. Ask your backup administrator "
                                  "to create a personalized Console resource on the Director."));
        m_statusLabel->setStyleSheet("color: #c08000; font-style: italic;");
    } else {
        m_statusLabel->setText(tr("Found %1 console(s)").arg(m_consoleCombo->count()));
        m_statusLabel->setStyleSheet("color: gray; font-style: italic;");
    }

    // Select current console if it exists
    QString current = field("consoleName").toString();
    int idx = m_consoleCombo->findText(current);
    if (idx >= 0) m_consoleCombo->setCurrentIndex(idx);

    emit completeChanged();

    // Keep connection open for console detail queries (don't disconnect)
}

void ConsoleSetupPage::onShowConsoleResult(const QString &name, const QJsonObject &console)
{
    if (console.isEmpty()) {
        m_consoleDetails->setPlainText(tr("No details found for %1").arg(name));
        return;
    }

    QString details;
    details += tr("Name: %1\n").arg(console["name"].toString());
    if (console.contains("description"))
        details += tr("Description: %1\n").arg(console["description"].toString());
    if (console.contains("profile"))
        details += tr("Profile: %1\n").arg(console["profile"].toString());
    if (console.contains("tlsenable"))
        details += tr("TLS Enabled: %1\n").arg(console["tlsenable"].toBool() ? "Yes" : "No");
    if (console.contains("tlsrequire"))
        details += tr("TLS Required: %1\n").arg(console["tlsrequire"].toBool() ? "Yes" : "No");
    if (console.contains("tlsverifypeer"))
        details += tr("TLS Verify Peer: %1\n").arg(console["tlsverifypeer"].toBool() ? "Yes" : "No");
    if (console.contains("jobacl"))
        details += tr("Job ACL: %1\n").arg(console["jobacl"].toVariant().toStringList().join(", "));
    if (console.contains("clientacl"))
        details += tr("Client ACL: %1\n").arg(console["clientacl"].toVariant().toStringList().join(", "));
    if (console.contains("storageacl"))
        details += tr("Storage ACL: %1\n").arg(console["storageacl"].toVariant().toStringList().join(", "));
    if (console.contains("commandacl"))
        details += tr("Command ACL: %1\n").arg(console["commandacl"].toVariant().toStringList().join(", "));
    if (console.contains("catalogacl"))
        details += tr("Catalog ACL: %1\n").arg(console["catalogacl"].toVariant().toStringList().join(", "));

    // Check for overly wide ACLs
    bool hasWideAcl = false;
    for (const QString &aclKey : {"jobacl", "clientacl", "storageacl", "commandacl"}) {
        if (console.contains(aclKey)) {
            QStringList acls = console[aclKey].toVariant().toStringList();
            if (acls.contains("*all*")) {
                hasWideAcl = true;
                break;
            }
        }
    }
    if (hasWideAcl) {
        details += tr("\n--- WARNING ---\n"
                      "This console has full access (*all*). Consider restricting ACLs "
                      "to prevent accidental deletion of backups or other critical operations.\n");
    }

    m_consoleDetails->setPlainText(details);
}

void ConsoleSetupPage::generatePassword()
{
    // Generate a random 24-character password
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%";
    QString password;
    for (int i = 0; i < 24; ++i) {
        int idx = QRandomGenerator::global()->bounded(chars.length());
        password.append(chars.at(idx));
    }
    m_newPasswordEdit->setText(password);
    emit completeChanged();
}

void ConsoleSetupPage::createConsole()
{
    if (m_director) {
        m_director->disconnect();
        m_director->deleteLater();
    }

    m_director = createConnectedDirector();

    QString newName = m_newNameEdit->text().trimmed();
    QString newPassword = m_newPasswordEdit->text();

    QObject::connect(m_director, &BareosDirector::allResourcesLoaded, this, [this, newName, newPassword]() {
        m_director->queryConfigureAddConsole(newName, newPassword);
        m_statusLabel->setText(tr("Creating console '%1'...").arg(newName));
    });

    QObject::connect(m_director, &BareosDirector::configureResult,
                     this, &ConsoleSetupPage::onConfigureResult);
}

void ConsoleSetupPage::onConfigureResult(bool success, const QString &message)
{
    Q_UNUSED(message)
    if (success) {
        m_statusLabel->setText(tr("Console created successfully!"));
        m_statusLabel->setStyleSheet("color: #008000; font-style: italic;");
    } else {
        m_statusLabel->setText(tr("Failed to create console"));
        m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
    }
    if (m_director) m_director->disconnect();
}

// ============================================================================
// ProfileNamePage
// ============================================================================

ProfileNamePage::ProfileNamePage(QWidget *parent)
    : QAPage(tr("Name this connection"), parent)
{
    m_edit = new QLineEdit(this);
    m_edit->setPlaceholderText(tr("e.g., Production Server"));
    m_layout->addWidget(m_edit);

    m_defaultCheck = new QCheckBox(tr("Set as default"), this);
    m_defaultCheck->setChecked(true);
    m_layout->addWidget(m_defaultCheck);

    m_connectCheck = new QCheckBox(tr("Connect now"), this);
    m_connectCheck->setChecked(true);
    m_layout->addWidget(m_connectCheck);

    m_layout->addSpacing(20);

    // Buttons in horizontal layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Advanced Settings button
    m_advancedButton = new QPushButton(tr("Advanced Settings..."), this);
    m_advancedButton->setToolTip(tr("Configure TLS options, ACLs, and other advanced settings"));
    connect(m_advancedButton, &QPushButton::clicked, this, &ProfileNamePage::onAdvancedSettingsClicked);
    buttonLayout->addWidget(m_advancedButton);

    // Export Config button
    m_exportButton = new QPushButton(tr("Export Config..."), this);
    m_exportButton->setToolTip(tr("Export Bareos configuration files as zip archive"));
    connect(m_exportButton, &QPushButton::clicked, this, &ProfileNamePage::onExportConfigClicked);
    buttonLayout->addWidget(m_exportButton);

    buttonLayout->addStretch();
    m_layout->addLayout(buttonLayout);

    setHint(tr("This name helps identify the connection."));
    m_layout->addStretch();

    registerField("profileName*", m_edit);
    registerField("setDefault", m_defaultCheck);
    registerField("connectNow", m_connectCheck);
}

void ProfileNamePage::initializePage()
{
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        BConnectionWizardData *data = wiz->wizardData();
        // Use profile name from template if available, otherwise use host as default
        if (!data->profileName.isEmpty()) {
            m_edit->setText(data->profileName);
        } else if (m_edit->text().isEmpty() && !data->host.isEmpty()) {
            m_edit->setText(data->host);
        }
        m_defaultCheck->setChecked(data->setAsDefault);
        m_connectCheck->setChecked(data->connectNow);
    } else {
        QString host = field("host").toString();
        if (m_edit->text().isEmpty() && !host.isEmpty()) m_edit->setText(host);
    }
}

bool ProfileNamePage::validatePage()
{
    // Save to wizard data struct
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        wiz->wizardData()->profileName = m_edit->text().trimmed();
        wiz->wizardData()->setAsDefault = m_defaultCheck->isChecked();
        wiz->wizardData()->connectNow = m_connectCheck->isChecked();
    }
    return true;
}

int ProfileNamePage::nextId() const
{
    // This is the final page - end the wizard
    return -1;
}

void ProfileNamePage::onAdvancedSettingsClicked()
{
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (!wiz) return;

    // Get current profile from wizard
    BConnectionProfile profile = wiz->profile();

    // Open the advanced settings dialog
    BProfileSettingsDialog dialog(profile, this);

    if (dialog.exec() == QDialog::Accepted) {
        // Update the wizard's profile with the advanced settings
        wiz->setProfile(dialog.profile());
    }
}

void ProfileNamePage::onExportConfigClicked()
{
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (!wiz) return;

    // Get current profile from wizard
    BConnectionProfile profile = wiz->profile();

    // Ask for export mode
    QStringList options;
    options << tr("Console only (for existing Director)");
    options << tr("Console + Profile (with ACLs)");
    options << tr("Full configuration (new installation)");

    bool ok;
    QString selected = QInputDialog::getItem(this,
                                              tr("Export Configuration"),
                                              tr("Select export mode:"),
                                              options, 0, false, &ok);
    if (!ok) return;

    BConfigExporter::ExportMode mode = BConfigExporter::ExportConsoleOnly;
    if (selected == options[1]) {
        mode = BConfigExporter::ExportWithProfile;
    } else if (selected == options[2]) {
        mode = BConfigExporter::ExportFull;
    }

    // Ask for file location
    QString defaultName = QString("bareos-config-%1.zip").arg(profile.consoleName);
    QString filename = QFileDialog::getSaveFileName(this,
                                                     tr("Export Configuration"),
                                                     defaultName,
                                                     tr("Zip Archives (*.zip)"));
    if (filename.isEmpty()) return;

    // Export
    if (BConfigExporter::exportToArchive(profile, filename, mode)) {
        QMessageBox::information(this, tr("Export Successful"),
                                 tr("Configuration exported to:\n%1\n\n"
                                    "Extract to your Bareos server's root directory and "
                                    "restart the Director.").arg(filename));
    } else {
        QMessageBox::warning(this, tr("Export Failed"),
                             tr("Failed to export configuration:\n%1")
                             .arg(BConfigExporter::lastError()));
    }
}
