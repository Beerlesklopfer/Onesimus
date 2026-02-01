/**
 * @file bconnectionwizard.cpp
 * @brief Director Configuration Wizard (Q&A Style)
 */

#include "bconnectionwizard.h"
#include "bareosdirector.h"
#include "bcertificategenerator.h"
#include "bprofilesettingsdialog.h"
#include "bconfigexporter.h"
#include "bpfxconverter.h"
#include "bsettings.h"
#include "bdatabase.h"
#include "bdirectormodel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QFile>
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
    setWindowIcon(QIcon::fromTheme("network-server"));

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

        qDebug() << "[ConnectionWizard] Using wizardData - savePassword:" << m_wizardData->savePassword;

        if (m_wizardData->savePassword) {
            // MANDATORY: Transform cleartext to MD5 hash
            p.setPasswordFromCleartext(m_wizardData->password);
            qDebug() << "[ConnectionWizard] Password saved to profile (as MD5 hash)";
        } else {
            p.password.clear();
            p.passwordHash.clear();
            qDebug() << "[ConnectionWizard] Password cleared from profile";
        }

        QString auth = m_wizardData->authMethod;
        if (auth == "legacy") {
            p.legacyAuth = true;
            p.tlsEnabled = false;
            p.tlsUsePSK = false;
        } else if (auth == "psk") {
            p.legacyAuth = false;
            p.tlsEnabled = true;
            p.tlsUsePSK = true;
        } else {
            p.legacyAuth = false;
            p.tlsEnabled = true;
            p.tlsUsePSK = false;
            p.tlsCaCertFile = m_wizardData->tlsCaCertFile;
            p.tlsCertFile = m_wizardData->tlsCertFile;
            p.tlsKeyFile = m_wizardData->tlsKeyFile;
            p.tlsVerifyPeer = true;
        }
    } else {
        // Fallback to field values (for backward compatibility)
        p.name = field("profileName").toString();
        p.host = field("host").toString();
        p.port = field("port").toInt();
        p.directorName = field("directorName").toString();
        p.consoleName = field("consoleName").toString();

        bool savePasswordChecked = field("savePassword").toBool();
        qDebug() << "[ConnectionWizard] Using fields - savePassword:" << savePasswordChecked;

        if (savePasswordChecked) {
            // MANDATORY: Transform cleartext to MD5 hash
            p.setPasswordFromCleartext(field("password").toString());
        } else {
            p.password.clear();
            p.passwordHash.clear();
        }

        QString auth = field("authMethod").toString();
        if (auth == "legacy") {
            p.legacyAuth = true;
            p.tlsEnabled = false;
            p.tlsUsePSK = false;
        } else if (auth == "psk") {
            p.legacyAuth = false;
            p.tlsEnabled = true;
            p.tlsUsePSK = true;
        } else {
            p.legacyAuth = false;
            p.tlsEnabled = true;
            p.tlsUsePSK = false;
            p.tlsCaCertFile = field("caCert").toString();
            p.tlsCertFile = field("clientCert").toString();
            p.tlsKeyFile = field("clientKey").toString();
            p.tlsVerifyPeer = true;
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
        qWarning() << "[ConnectionWizard] Cannot save: database not open or no wizard data";
        return -1;
    }

    // Create Director model
    BDirectorModel model(nullptr, db);
    if (!model.initialize()) {
        qWarning() << "[ConnectionWizard] Failed to initialize BDirectorModel";
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
        qWarning() << "[ConnectionWizard] Failed to create Director record";
        return -1;
    }

    // Find the row for the new Director
    int row = model.findDirectorRow(dirId);
    if (row < 0) {
        qWarning() << "[ConnectionWizard] Director created but row not found";
        return -1;
    }

    // Update TLS settings based on auth method
    QString auth = m_wizardData->authMethod;
    if (auth == "legacy") {
        // Legacy: no TLS
        model.setData(model.index(row, BDirectorModel::TlsEnable), false);
    } else if (auth == "psk") {
        // TLS-PSK: TLS enabled, no certificates
        model.setData(model.index(row, BDirectorModel::TlsEnable), true);
        model.setData(model.index(row, BDirectorModel::TlsRequire), true);
    } else {
        // Certificate mode: TLS with certificates
        model.setData(model.index(row, BDirectorModel::TlsEnable), true);
        model.setData(model.index(row, BDirectorModel::TlsRequire), true);
        model.setData(model.index(row, BDirectorModel::TlsVerifyPeer), true);
        model.setData(model.index(row, BDirectorModel::TlsCaCertificateFile), m_wizardData->tlsCaCertFile);
        model.setData(model.index(row, BDirectorModel::TlsCertificate), m_wizardData->tlsCertFile);
        model.setData(model.index(row, BDirectorModel::TlsKey), m_wizardData->tlsKeyFile);
    }

    // Submit all changes
    if (!model.submitAll()) {
        qWarning() << "[ConnectionWizard] Failed to save Director TLS settings:" << model.lastError().text();
        return -1;
    }

    qDebug() << "[ConnectionWizard] Director saved to database with ID:" << dirId;
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
}

void TemplateSelectionPage::initializePage()
{
    loadTemplates();
}

void TemplateSelectionPage::loadTemplates()
{
    m_templateCombo->clear();
    m_templateDetailsLabel->setVisible(false);

    // Get database from wizard
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (!wiz || !wiz->database() || !wiz->database()->isOpen()) {
        m_templateCombo->addItem(tr("(No database connection)"), -1);
        return;
    }

    // Load Directors from database
    BDirectorModel *model = new BDirectorModel(this, *wiz->database());
    if (!model->initialize()) {
        m_templateCombo->addItem(tr("(Failed to load templates)"), -1);
        delete model;
        return;
    }

    model->setFilterActiveOnly(true);

    if (model->rowCount() == 0) {
        m_templateCombo->addItem(tr("(No templates available)"), -1);
        delete model;
        return;
    }

    // Add each Director as a template option
    for (int row = 0; row < model->rowCount(); ++row) {
        int dirId = model->directorId(row);
        QString name = model->directorName(row);
        QString address = model->data(model->index(row, BDirectorModel::Address)).toString();
        int port = model->data(model->index(row, BDirectorModel::Port)).toInt();

        QString displayText = QString("%1 (%2:%3)").arg(name, address).arg(port);
        m_templateCombo->addItem(displayText, dirId);
    }

    delete model;
}

void TemplateSelectionPage::loadTemplateDetails(int directorId)
{
    if (directorId < 0) {
        m_templateDetailsLabel->setVisible(false);
        return;
    }

    // Get database from wizard
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (!wiz || !wiz->database() || !wiz->database()->isOpen()) {
        m_templateDetailsLabel->setVisible(false);
        return;
    }

    // Load Director details from database
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

    // Load template data into wizard data struct for pre-filling fields
    if (wiz->wizardData()) {
        wiz->wizardData()->host = model->data(model->index(row, BDirectorModel::Address)).toString();
        wiz->wizardData()->port = model->data(model->index(row, BDirectorModel::Port)).toInt();
        wiz->wizardData()->directorName = model->directorName(row);
        wiz->wizardData()->password = model->data(model->index(row, BDirectorModel::PasswordHash)).toString();

        // Determine auth method from TLS settings
        if (!tlsEnable) {
            wiz->wizardData()->authMethod = "legacy";
        } else {
            QString tlsCert = model->data(model->index(row, BDirectorModel::TlsCertificate)).toString();
            wiz->wizardData()->authMethod = tlsCert.isEmpty() ? "psk" : "cert";

            if (wiz->wizardData()->authMethod == "cert") {
                wiz->wizardData()->tlsCaCertFile = model->data(model->index(row, BDirectorModel::TlsCaCertificateFile)).toString();
                wiz->wizardData()->tlsCertFile = tlsCert;
                wiz->wizardData()->tlsKeyFile = model->data(model->index(row, BDirectorModel::TlsKey)).toString();
            }
        }
    }

    delete model;
}

int TemplateSelectionPage::nextId() const
{
    if (m_importZipRadio->isChecked()) {
        // Skip to final page after ZIP import validation
        return BConnectionWizard::Page_ProfileName;
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
        int dirId = m_templateCombo->currentData().toInt();
        return dirId >= 0;
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

    int dirId = m_templateCombo->itemData(index).toInt();
    loadTemplateDetails(dirId);

    // TODO: Load template data into wizard data struct
    // BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    // if (wiz && wiz->wizardData() && dirId >= 0) {
    //     // Load from database and populate wizardData
    // }

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
// ServerPage
// ============================================================================

ServerPage::ServerPage(QWidget *parent)
    : QAPage(tr("Where is your Bareos Director?"), parent)
    , m_checkInProgress(false)
    , m_checkCompleted(false)
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

    m_layout->addLayout(form);

    // Check button and status
    auto *checkLayout = new QHBoxLayout();
    m_checkButton = new QPushButton(tr("Check Server"), this);
    m_checkButton->setToolTip(tr("Check if the server is reachable and detect supported authentication methods"));
    checkLayout->addWidget(m_checkButton);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0);
    m_progressBar->setMaximumWidth(100);
    m_progressBar->setVisible(false);
    checkLayout->addWidget(m_progressBar);

    checkLayout->addStretch();
    m_layout->addLayout(checkLayout);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet("color: gray; font-style: italic;");
    m_layout->addWidget(m_statusLabel);

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

    connect(m_hostEdit, &QLineEdit::textChanged, this, &ServerPage::completeChanged);
    connect(m_hostEdit, &QLineEdit::textChanged, this, [this]() {
        m_checkCompleted = false;
        m_statusLabel->clear();
    });
    connect(m_portSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this]() {
        m_checkCompleted = false;
        m_statusLabel->clear();
    });
    connect(m_checkButton, &QPushButton::clicked, this, &ServerPage::onCheckClicked);
}

bool ServerPage::isComplete() const
{
    return !m_hostEdit->text().trimmed().isEmpty();
}

bool ServerPage::validatePage()
{
    // If check not done yet, do it now
    if (!m_checkCompleted && !m_checkInProgress) {
        checkCapabilities();
        return false;  // Will proceed when check completes
    }

    // Save values to wizard data struct
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        wiz->wizardData()->host = m_hostEdit->text().trimmed();
        wiz->wizardData()->port = m_portSpin->value();
    }

    return m_checkCompleted;
}

void ServerPage::onCheckClicked()
{
    if (!m_checkInProgress) {
        checkCapabilities();
    }
}

void ServerPage::checkCapabilities()
{
    if (m_checkInProgress) return;

    QString host = m_hostEdit->text().trimmed();
    int port = m_portSpin->value();

    if (host.isEmpty()) {
        m_statusLabel->setText(tr("Please enter a hostname."));
        m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
        return;
    }

    m_checkInProgress = true;
    m_checkButton->setEnabled(false);
    m_progressBar->setVisible(true);
    m_statusLabel->setText(tr("Checking server %1:%2...").arg(host).arg(port));
    m_statusLabel->setStyleSheet("color: gray; font-style: italic;");

    // Simple TCP connection check first
    auto *socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, [this, socket]() {
        socket->disconnectFromHost();
        socket->deleteLater();
        onCheckComplete(true);
    });

    connect(socket, &QTcpSocket::errorOccurred, this, [this, socket](QAbstractSocket::SocketError error) {
        Q_UNUSED(error)
        QString errMsg = socket->errorString();
        socket->deleteLater();
        m_statusLabel->setText(tr("Server unreachable: %1").arg(errMsg));
        m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
        m_checkInProgress = false;
        m_checkButton->setEnabled(true);
        m_progressBar->setVisible(false);
    });

    // Timeout after 10 seconds
    QTimer::singleShot(10000, socket, [this, socket]() {
        if (m_checkInProgress && socket->state() != QAbstractSocket::ConnectedState) {
            socket->abort();
            socket->deleteLater();
            m_statusLabel->setText(tr("Connection timeout"));
            m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
            m_checkInProgress = false;
            m_checkButton->setEnabled(true);
            m_progressBar->setVisible(false);
        }
    });

    socket->connectToHost(host, port);
}

void ServerPage::onCheckComplete(bool reachable)
{
    m_checkInProgress = false;
    m_checkButton->setEnabled(true);
    m_progressBar->setVisible(false);

    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (!wiz) return;

    BConnectionWizard::ServerCapabilities caps;
    caps.checked = true;
    caps.reachable = reachable;

    if (reachable) {
        // Server is reachable - assume modern Bareos with PSK support
        // The actual auth test will happen on the Test page
        caps.supportsPSK = true;
        caps.supportsCert = true;
        caps.supportsLegacy = true;  // We'll let the user choose but warn them

        m_statusLabel->setText(tr("Server reachable. Click Next to configure authentication."));
        m_statusLabel->setStyleSheet("color: #008000; font-style: italic;");
        m_checkCompleted = true;
    } else {
        caps.lastError = tr("Server not reachable");
        m_statusLabel->setText(tr("Server not reachable on this port."));
        m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
        m_checkCompleted = false;
    }

    wiz->setCapabilities(caps);
    emit completeChanged();

    // Auto-advance if validation was pending
    if (m_checkCompleted) {
        wizard()->next();
    }
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

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    form->addRow(tr("Password:"), m_passwordEdit);

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
        qDebug() << "[CredentialsPage] Saving to wizardData - savePassword:" << m_saveCheck->isChecked();
    }
    return true;
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

    m_legacyRadio = new QRadioButton(tr("Legacy (no encryption)"), this);
    m_group->addButton(m_legacyRadio, 2);
    m_layout->addWidget(m_legacyRadio);

    // Capability info label (shown after server check)
    m_capabilityLabel = new QLabel(this);
    m_capabilityLabel->setWordWrap(true);
    m_capabilityLabel->setStyleSheet("color: #666; font-size: 11px; margin-top: 10px;");
    m_capabilityLabel->setVisible(false);
    m_layout->addWidget(m_capabilityLabel);

    setHint(tr("TLS-PSK is the default for Bareos 18.2+."));
    m_layout->addStretch();

    registerField("authMethod", this, "authMethod");
    connect(m_group, &QButtonGroup::idClicked, this, &AuthMethodPage::onSelectionChanged);
}

void AuthMethodPage::initializePage()
{
    // Get capabilities from wizard
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());

    // Update UI based on detected capabilities
    updateCapabilityHints();

    // Restore radio button selection from stored field value
    QString auth = field("authMethod").toString();
    if (auth == "legacy" && m_legacyRadio->isEnabled()) {
        m_legacyRadio->setChecked(true);
    } else if (auth == "cert") {
        m_certRadio->setChecked(true);
    } else {
        m_pskRadio->setChecked(true);
    }
    onSelectionChanged();  // Update hint text
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
        m_legacyRadio->setEnabled(true);
        return;
    }

    // Show capability info
    m_capabilityLabel->setVisible(true);

    if (caps.reachable) {
        // If server supports PSK, disable legacy for security
        if (caps.supportsPSK) {
            m_legacyRadio->setEnabled(false);
            m_legacyRadio->setText(tr("Legacy (no encryption) - disabled, use TLS"));
            m_capabilityLabel->setText(tr("Server supports TLS encryption. "
                                          "Legacy mode is disabled for security reasons."));
            m_capabilityLabel->setStyleSheet("color: #008000; font-size: 11px; margin-top: 10px;");

            // If legacy was selected, switch to PSK
            if (m_legacyRadio->isChecked()) {
                m_pskRadio->setChecked(true);
            }
        } else {
            // Server doesn't support PSK (older Bareos/Bacula?)
            m_legacyRadio->setEnabled(true);
            m_legacyRadio->setText(tr("Legacy (no encryption)"));
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
    } else if (m_certRadio->isChecked()) {
        setHint(tr("You'll need CA, client certificate, and private key."));
        m_hintLabel->setStyleSheet("color: gray; font-style: italic;");
    } else {
        setHint(tr("Warning: Data will be sent without encryption!"));
        m_hintLabel->setStyleSheet("color: #c00000; font-style: italic; font-weight: bold;");
    }
    setField("authMethod", authMethod());

    // Save to wizard data struct
    BConnectionWizard *wiz = qobject_cast<BConnectionWizard*>(wizard());
    if (wiz && wiz->wizardData()) {
        wiz->wizardData()->authMethod = authMethod();
    }
}

QString AuthMethodPage::authMethod() const
{
    if (m_legacyRadio && m_legacyRadio->isChecked()) return "legacy";
    if (m_certRadio && m_certRadio->isChecked()) return "cert";
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
#ifndef Q_OS_WINDOWS
        wiz->wizardData()->tlsCertFile = cert;
        wiz->wizardData()->tlsKeyFile = key;
#endif
    }

    return true;
}

void TLSPage::browseCaCert()
{
    QString f = QFileDialog::getOpenFileName(this, tr("CA Certificate"), "/etc/bareos/ssl", tr("*.pem *.crt"));
    if (!f.isEmpty()) m_caCertEdit->setText(f);
}

void TLSPage::browseClientCert()
{
#ifdef Q_OS_WINDOWS
    QString f = QFileDialog::getOpenFileName(this, tr("Select PFX Certificate"),
        QString(), tr("PFX Files (*.pfx);;All Files (*)"));
#else
    QString f = QFileDialog::getOpenFileName(this, tr("Client Certificate"), "/etc/bareos/ssl", tr("*.pem *.crt"));
#endif
    if (!f.isEmpty()) m_clientCertEdit->setText(f);
}

void TLSPage::browseClientKey()
{
    QString f = QFileDialog::getOpenFileName(this, tr("Private Key"), "/etc/bareos/ssl", tr("*.pem *.key"));
    if (!f.isEmpty()) m_clientKeyEdit->setText(f);
}

void TLSPage::generateCertificates()
{
    BCertificateGenerator dialog(this);
    if (dialog.exec() == QDialog::Accepted && dialog.isSuccessful()) {
        m_caCertEdit->setText(dialog.caCertPath());
#ifndef Q_OS_WINDOWS
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

    m_copyConsoleButton = new QPushButton(tr("Copy to Clipboard"), consoleWidget);
    m_copyConsoleButton->setIcon(QIcon::fromTheme("edit-copy"));
    connect(m_copyConsoleButton, &QPushButton::clicked, this, &ConfigPreviewPage::onCopyConsoleConfig);
    consoleLayout->addWidget(m_copyConsoleButton);

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

    m_copyDirectorButton = new QPushButton(tr("Copy to Clipboard"), directorWidget);
    m_copyDirectorButton->setIcon(QIcon::fromTheme("edit-copy"));
    connect(m_copyDirectorButton, &QPushButton::clicked, this, &ConfigPreviewPage::onCopyDirectorConfig);
    directorLayout->addWidget(m_copyDirectorButton);

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

    if (authMethod == "legacy") {
        consoleConfig += QString("  TLS Enable = no\n");
    } else if (authMethod == "psk") {
        consoleConfig += QString("  TLS Enable = yes\n");
        consoleConfig += QString("  TLS Require = no\n");
        consoleConfig += QString("  TLS Verify Peer = no\n");
    } else {
        // Certificate mode
        consoleConfig += QString("  TLS Enable = yes\n");
        consoleConfig += QString("  TLS Require = yes\n");
        consoleConfig += QString("  TLS Verify Peer = yes\n");
        QString caCert = field("caCert").toString();
        QString clientCert = field("clientCert").toString();
        QString clientKey = field("clientKey").toString();
        if (!caCert.isEmpty())
            consoleConfig += QString("  TLS CA Certificate File = \"%1\"\n").arg(caCert);
        if (!clientCert.isEmpty())
            consoleConfig += QString("  TLS Certificate = \"%1\"\n").arg(clientCert);
        if (!clientKey.isEmpty())
            consoleConfig += QString("  TLS Key = \"%1\"\n").arg(clientKey);
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
    consoleConfig += QString("}\n");

    m_consoleConfigEdit->setPlainText(consoleConfig);

    // Generate Director TLS hints
    QString directorConfig;
    directorConfig += QString("# Add/modify these TLS settings in your Director resource:\n\n");

    if (authMethod == "legacy") {
        directorConfig += QString("Director {\n");
        directorConfig += QString("  # ... other settings ...\n");
        directorConfig += QString("  TLS Enable = no\n");
        directorConfig += QString("}\n");
    } else if (authMethod == "psk") {
        directorConfig += QString("Director {\n");
        directorConfig += QString("  # ... other settings ...\n");
        directorConfig += QString("  TLS Enable = yes\n");
        directorConfig += QString("  TLS Require = no      # Allow both TLS and non-TLS\n");
        directorConfig += QString("  # TLS-PSK uses the Console password for encryption\n");
        directorConfig += QString("}\n");
    } else {
        directorConfig += QString("Director {\n");
        directorConfig += QString("  # ... other settings ...\n");
        directorConfig += QString("  TLS Enable = yes\n");
        directorConfig += QString("  TLS Require = yes\n");
        directorConfig += QString("  TLS CA Certificate File = \"/etc/bareos/ssl/ca.pem\"\n");
        directorConfig += QString("  TLS Certificate = \"/etc/bareos/ssl/bareos-dir.pem\"\n");
        directorConfig += QString("  TLS Key = \"/etc/bareos/ssl/bareos-dir-key.pem\"\n");
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
int TestPage::nextId() const { return BConnectionWizard::Page_ConsoleSetup; }

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

    if (auth == "legacy") {
        tls.tlsEnable = false;
        tls.tlsRequire = false;
        tls.tlsPSKEnable = false;
        appendLog(tr("Mode: Legacy"));
    } else if (auth == "psk") {
        tls.tlsEnable = true;
        tls.tlsRequire = true;
        tls.tlsPSKEnable = true;
        tls.tlsVerifyPeer = false;
        appendLog(tr("Mode: TLS-PSK"));
    } else {
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

    // Option 1: Modify existing console
    m_modifyExistingRadio = new QRadioButton(tr("Modify existing console"), this);
    m_modifyExistingRadio->setChecked(true);
    m_group->addButton(m_modifyExistingRadio, 0);
    m_layout->addWidget(m_modifyExistingRadio);

    auto *selectLayout = new QHBoxLayout();
    selectLayout->setContentsMargins(25, 0, 0, 0);
    m_consoleCombo = new QComboBox(this);
    m_consoleCombo->setMinimumWidth(200);
    selectLayout->addWidget(m_consoleCombo);
    m_refreshButton = new QPushButton(tr("Refresh"), this);
    selectLayout->addWidget(m_refreshButton);
    selectLayout->addStretch();
    m_layout->addLayout(selectLayout);

    // Console details display (shown when selecting existing console)
    m_consoleDetails = new QTextEdit(this);
    m_consoleDetails->setReadOnly(true);
    m_consoleDetails->setMaximumHeight(100);
    m_consoleDetails->setPlaceholderText(tr("Select a console to view its configuration..."));
    m_consoleDetails->setStyleSheet("font-family: monospace; font-size: 9pt;");
    auto *detailsLayout = new QHBoxLayout();
    detailsLayout->setContentsMargins(25, 0, 0, 0);
    detailsLayout->addWidget(m_consoleDetails);
    m_layout->addLayout(detailsLayout);

    // Option 2: Create new console
    m_createNewRadio = new QRadioButton(tr("Create new console"), this);
    m_group->addButton(m_createNewRadio, 1);
    m_layout->addWidget(m_createNewRadio);

    auto *createForm = new QFormLayout();
    createForm->setContentsMargins(25, 0, 0, 0);
    m_newNameEdit = new QLineEdit(this);
    m_newNameEdit->setText("onesimus-client");
    m_newNameEdit->setEnabled(false);
    createForm->addRow(tr("Name:"), m_newNameEdit);

    auto *pwdLayout = new QHBoxLayout();
    m_newPasswordEdit = new QLineEdit(this);
    m_newPasswordEdit->setEnabled(false);
    pwdLayout->addWidget(m_newPasswordEdit);
    m_generatePasswordButton = new QPushButton(tr("Generate"), this);
    m_generatePasswordButton->setEnabled(false);
    pwdLayout->addWidget(m_generatePasswordButton);
    createForm->addRow(tr("Password:"), pwdLayout);
    m_layout->addLayout(createForm);

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

void ConsoleSetupPage::loadConsoleDetails(const QString &name)
{
    if (name.isEmpty()) return;

    m_consoleDetails->setPlainText(tr("Loading details for %1...").arg(name));

    // Create a new director connection to fetch console details
    auto *detailDirector = new BareosDirector(this);
    detailDirector->initialize();

    connect(detailDirector, &BareosDirector::jsonResponse, this, [this, detailDirector, name](const QString &cmd, const QString &json) {
        Q_UNUSED(cmd)

        QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
        if (!doc.isObject()) {
            m_consoleDetails->setPlainText(tr("Failed to parse response"));
            detailDirector->disconnect();
            detailDirector->deleteLater();
            return;
        }

        QJsonObject root = doc.object();
        QJsonObject result = root["result"].toObject();

        // Look for console details in the result
        if (result.contains("consoles")) {
            QJsonArray consoles = result["consoles"].toArray();
            QString details;
            for (const QJsonValue &v : consoles) {
                QJsonObject console = v.toObject();
                if (console["name"].toString() == name) {
                    // Format the console details
                    details += tr("Name: %1\n").arg(console["name"].toString());
                    if (console.contains("description"))
                        details += tr("Description: %1\n").arg(console["description"].toString());
                    if (console.contains("tlsenable"))
                        details += tr("TLS Enabled: %1\n").arg(console["tlsenable"].toBool() ? "Yes" : "No");
                    if (console.contains("tlspskenable"))
                        details += tr("TLS-PSK: %1\n").arg(console["tlspskenable"].toBool() ? "Yes" : "No");
                    if (console.contains("profile"))
                        details += tr("Profile: %1\n").arg(console["profile"].toString());
                    if (console.contains("jobacl"))
                        details += tr("Job ACL: %1\n").arg(console["jobacl"].toVariant().toStringList().join(", "));
                    break;
                }
            }
            if (!details.isEmpty()) {
                m_consoleDetails->setPlainText(details);
            } else {
                m_consoleDetails->setPlainText(tr("No details found for %1").arg(name));
            }
        }

        detailDirector->disconnect();
        detailDirector->deleteLater();
    });

    connect(detailDirector, &BareosDirector::allResourcesLoaded, detailDirector, [detailDirector, name]() {
        detailDirector->sendRawCommand(QString("show console=%1").arg(name));
    });

    connect(detailDirector, &BareosDirector::protocolError, this, [this, detailDirector](const QString &err) {
        m_consoleDetails->setPlainText(tr("Error: %1").arg(err));
        detailDirector->disconnect();
        detailDirector->deleteLater();
    });

    // Configure TLS same as main director
    BareosDirector::TLSConfig tls;
    QString auth = field("authMethod").toString();
    if (auth == "legacy") {
        tls.tlsEnable = false;
        tls.tlsRequire = false;
        tls.tlsPSKEnable = false;
    } else if (auth == "psk") {
        tls.tlsEnable = true;
        tls.tlsRequire = true;
        tls.tlsPSKEnable = true;
        tls.tlsVerifyPeer = false;
    } else {
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
    }
    detailDirector->setTLSConfig(tls);

    QString host = field("host").toString();
    int port = field("port").toInt();
    QString dir = field("directorName").toString();
    QString con = field("consoleName").toString();
    QString pwd = field("password").toString();

    detailDirector->connect(host, port, dir, con, pwd);
}

void ConsoleSetupPage::loadConsoles()
{
    m_statusLabel->setText(tr("Loading consoles..."));
    m_consoleCombo->clear();

    if (m_director) {
        m_director->disconnect();
        m_director->deleteLater();
    }

    m_director = new BareosDirector(this);
    m_director->initialize();

    connect(m_director, &BareosDirector::jsonResponse, this, &ConsoleSetupPage::onJsonResponse);
    connect(m_director, &BareosDirector::allResourcesLoaded, this, [this]() {
        // Send "show consoles" command once connected
        m_director->sendRawCommand("show consoles");
    });
    connect(m_director, &BareosDirector::protocolError, this, [this](const QString &err) {
        m_statusLabel->setText(tr("Error: %1").arg(err));
        m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
    });

    // Configure TLS same as test page
    BareosDirector::TLSConfig tls;
    QString auth = field("authMethod").toString();
    if (auth == "legacy") {
        tls.tlsEnable = false;
        tls.tlsRequire = false;
        tls.tlsPSKEnable = false;
    } else if (auth == "psk") {
        tls.tlsEnable = true;
        tls.tlsRequire = true;
        tls.tlsPSKEnable = true;
        tls.tlsVerifyPeer = false;
    } else {
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
    }
    m_director->setTLSConfig(tls);

    QString host = field("host").toString();
    int port = field("port").toInt();
    QString dir = field("directorName").toString();
    QString con = field("consoleName").toString();
    QString pwd = field("password").toString();

    m_director->connect(host, port, dir, con, pwd);
}

void ConsoleSetupPage::onJsonResponse(const QString &cmd, const QString &json)
{
    Q_UNUSED(cmd)

    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isObject()) return;

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    // Look for "consoles" array in the result
    if (result.contains("consoles")) {
        QJsonArray consoles = result["consoles"].toArray();
        m_consoleCombo->clear();
        for (const QJsonValue &v : consoles) {
            QJsonObject console = v.toObject();
            QString name = console["name"].toString();
            if (!name.isEmpty()) {
                m_consoleCombo->addItem(name);
            }
        }
        m_consolesLoaded = true;
        m_statusLabel->setText(tr("Found %1 console(s)").arg(m_consoleCombo->count()));
        m_statusLabel->setStyleSheet("color: gray; font-style: italic;");

        // Select current console if it exists
        QString current = field("consoleName").toString();
        int idx = m_consoleCombo->findText(current);
        if (idx >= 0) m_consoleCombo->setCurrentIndex(idx);

        emit completeChanged();
    }

    if (m_director) {
        m_director->disconnect();
    }
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
    // Connect and send configure add console command
    if (m_director) {
        m_director->disconnect();
        m_director->deleteLater();
    }

    m_director = new BareosDirector(this);
    m_director->initialize();

    QString newName = m_newNameEdit->text().trimmed();
    QString newPassword = m_newPasswordEdit->text();

    connect(m_director, &BareosDirector::allResourcesLoaded, this, [this, newName, newPassword]() {
        // Send configure add console command
        QString cmd = QString("configure add console name=%1 password=\"%2\" profile=operator tlsenable=false")
                          .arg(newName, newPassword);
        m_director->sendRawCommand(cmd);
        m_statusLabel->setText(tr("Creating console '%1'...").arg(newName));
    });

    connect(m_director, &BareosDirector::jsonResponse, this, [this, newName](const QString &, const QString &json) {
        // Check if creation was successful
        if (json.contains("created") || json.contains("Created")) {
            m_statusLabel->setText(tr("Console '%1' created successfully!").arg(newName));
            m_statusLabel->setStyleSheet("color: #008000; font-style: italic;");
        } else if (json.contains("error") || json.contains("Error")) {
            m_statusLabel->setText(tr("Failed to create console"));
            m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
        }
        if (m_director) m_director->disconnect();
    });

    connect(m_director, &BareosDirector::protocolError, this, [this](const QString &err) {
        m_statusLabel->setText(tr("Error: %1").arg(err));
        m_statusLabel->setStyleSheet("color: #c00000; font-style: italic;");
    });

    // Configure TLS
    BareosDirector::TLSConfig tls;
    QString auth = field("authMethod").toString();
    if (auth == "legacy") {
        tls.tlsEnable = false;
        tls.tlsRequire = false;
        tls.tlsPSKEnable = false;
    } else if (auth == "psk") {
        tls.tlsEnable = true;
        tls.tlsRequire = true;
        tls.tlsPSKEnable = true;
        tls.tlsVerifyPeer = false;
    } else {
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
    }
    m_director->setTLSConfig(tls);

    QString host = field("host").toString();
    int port = field("port").toInt();
    QString dir = field("directorName").toString();
    QString con = field("consoleName").toString();
    QString pwd = field("password").toString();

    m_director->connect(host, port, dir, con, pwd);
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
    QString host = wiz && wiz->wizardData() ? wiz->wizardData()->host : field("host").toString();
    if (m_edit->text().isEmpty() && !host.isEmpty()) m_edit->setText(host);
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
