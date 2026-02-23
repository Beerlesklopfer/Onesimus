#include "clients/bnewclientdialog.h"
#include "blogging.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QClipboard>
#include <QApplication>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QTemporaryDir>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QFont>
#include <QFrame>
#include <QSplitter>

#include "config/bsettings.h"
#include "bconnectionprofile.h"
#include "config/bresourceform.h"

#define CLIENT_DEBUG BLOG_DEBUG()

// Helper: set a semantic state property on a label and re-polish so QSS picks it up.
// Pass an empty string to revert to the default (theme-defined) label style.
static void setLabelState(QLabel *label, const QString &state)
{
    label->setProperty("state", state.isEmpty() ? QVariant{} : QVariant{state});
    label->style()->unpolish(label);
    label->style()->polish(label);
    label->update();
}

// ============================================================================
// BNewClientWizard
// ============================================================================

BNewClientWizard::BNewClientWizard(BDirector *director, QWidget *parent)
    : QWizard(parent)
    , m_director(director)
{
    setWindowTitle(tr("Add New Client"));
    setWizardStyle(QWizard::ModernStyle);
    resize(700, 550);

    // Pre-populate director info from active connection
    if (m_director) {
        m_data.directorName = m_director->currentDirectorName();
        m_data.directorAddress = m_director->currentHost();
        m_data.directorPort = m_director->currentPort();

        // Determine TLS mode from last used profile
        BConnectionProfile profile = BSettings::instance().lastUsedProfile();
        if (profile.isValid()) {
            if (profile.tlsUsePSK) {
                m_data.tlsMode = tr("TLS-PSK");
            } else {
                m_data.tlsMode = tr("TLS Certificate");
            }
        } else {
            m_data.tlsMode = tr("Unknown");
        }
    }

    setPage(Page_Director, new BNewClientDirectorPage(this));
    setPage(Page_Settings, new BNewClientSettingsPage(this));
    setPage(Page_Preview, new BNewClientPreviewPage(this));
}

// ============================================================================
// Page 1: Director Info
// ============================================================================

BNewClientDirectorPage::BNewClientDirectorPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Director Information"));
    setSubTitle(tr("The new client will connect to this Director. "
                   "These settings are used to generate the File Daemon configuration."));

    QFormLayout *layout = new QFormLayout(this);

    m_directorNameLabel = new QLabel(this);
    m_directorNameLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addRow(tr("Director Name:"), m_directorNameLabel);

    m_directorAddressLabel = new QLabel(this);
    m_directorAddressLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addRow(tr("Director Address:"), m_directorAddressLabel);

    m_directorPortLabel = new QLabel(this);
    m_directorPortLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addRow(tr("Director Port:"), m_directorPortLabel);

    m_tlsModeLabel = new QLabel(this);
    m_tlsModeLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addRow(tr("TLS Mode:"), m_tlsModeLabel);

    m_errorLabel = new QLabel(this);
    m_errorLabel->setWordWrap(true);
    m_errorLabel->setVisible(false);
    layout->addRow(m_errorLabel);

    QLabel *hint = new QLabel(
        tr("The Director name above will be used in the File Daemon's Director resource. "
           "The client authenticates against this Director using a shared password."),
        this);
    hint->setWordWrap(true);
    hint->setObjectName("hintLabel");
    layout->addRow(hint);
}

void BNewClientDirectorPage::initializePage()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return;

    const auto &d = wiz->data();
    m_dataValid = true;
    m_errorLabel->setVisible(false);

    QStringList errors;

    if (d.directorName.isEmpty()) {
        m_directorNameLabel->setText(tr("<i>(not available)</i>"));
        errors << tr("Director name could not be retrieved from the connection.");
    } else {
        m_directorNameLabel->setText(QString("<b>%1</b>").arg(d.directorName));
    }

    if (d.directorAddress.isEmpty()) {
        m_directorAddressLabel->setText(tr("<i>(not available)</i>"));
        errors << tr("Director address could not be retrieved.");
    } else {
        m_directorAddressLabel->setText(d.directorAddress);
    }

    if (d.directorPort <= 0) {
        m_directorPortLabel->setText(tr("<i>(not available)</i>"));
        errors << tr("Director port could not be retrieved.");
    } else {
        m_directorPortLabel->setText(QString::number(d.directorPort));
    }

    m_tlsModeLabel->setText(d.tlsMode.isEmpty() ? tr("Unknown") : d.tlsMode);

    if (!errors.isEmpty()) {
        m_dataValid = false;
        m_errorLabel->setText(
            tr("<b style='color:red;'>Cannot proceed:</b><br>%1<br><br>"
               "Make sure you are connected to a Director.")
                .arg(errors.join("<br>")));
        m_errorLabel->setVisible(true);
    } else {
        // Check ACL permissions from connection profile
        BConnectionProfile profile = BSettings::instance().lastUsedProfile();
        if (profile.isValid() && !profile.aclCommand.isEmpty()) {
            bool hasConfigureAccess = false;
            for (const QString &acl : profile.aclCommand) {
                if (acl == "*all*" || acl.contains("configure", Qt::CaseInsensitive)) {
                    hasConfigureAccess = true;
                    break;
                }
            }
            if (!hasConfigureAccess) {
                m_errorLabel->setText(
                    tr("<span style='color:orange;'><b>Warning:</b> The current console may not have "
                       "permission to run 'configure add client'. "
                       "Check the Command ACL in the Console resource on the Director.</span><br>"
                       "You can still generate and export the FD configuration files."));
                m_errorLabel->setVisible(true);
            }
        }
    }

    emit completeChanged();
}

bool BNewClientDirectorPage::isComplete() const
{
    return m_dataValid;
}

// ============================================================================
// Page 2: Password & Settings
// ============================================================================

BNewClientSettingsPage::BNewClientSettingsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Client Settings"));
    setSubTitle(tr("Configure the new client's identity, password, TLS, and network settings."));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Basic settings group
    QGroupBox *basicGroup = new QGroupBox(tr("Basic Settings"), this);
    QFormLayout *basicLayout = new QFormLayout(basicGroup);

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText(tr("e.g. server1-fd"));
    basicLayout->addRow(tr("Client Name:"), m_nameEdit);

    m_addressEdit = new QLineEdit(this);
    m_addressEdit->setPlaceholderText(tr("e.g. 192.168.1.100 or server1.example.com"));
    basicLayout->addRow(tr("Address:"), m_addressEdit);

    m_portSpin = new QSpinBox(this);
    m_portSpin->setRange(1, 65535);
    m_portSpin->setValue(9102);
    basicLayout->addRow(tr("FD Port:"), m_portSpin);

    // Password row with generate button
    QHBoxLayout *pwdLayout = new QHBoxLayout;
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText(tr("Client password"));
    pwdLayout->addWidget(m_passwordEdit);
    m_generateButton = new QPushButton(tr("Generate"), this);
    pwdLayout->addWidget(m_generateButton);
    basicLayout->addRow(tr("Password:"), pwdLayout);

    m_md5Label = new QLabel(this);
    m_md5Label->setObjectName("md5HashLabel");
    m_md5Label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    basicLayout->addRow(tr("MD5 Hash:"), m_md5Label);

    mainLayout->addWidget(basicGroup);

    // TLS settings via schema-driven form (only TLS groups)
    m_resourceForm = new BResourceForm("Client", this);
    m_resourceForm->setGroupFilter({"tls", "tls-x509", "tls-pfx"});
    mainLayout->addWidget(m_resourceForm);

    // NAT/Passive mode - simple checkbox with conditional wait time
    m_passiveCheck = new QCheckBox(tr("Passive mode (client behind NAT/firewall)"), this);
    m_passiveCheck->setToolTip(
        tr("Enable if the client is behind a firewall/NAT.\n"
           "The Director and Storage Daemon will initiate connections to the client instead.\n\n"
           "Firewall ports to open on the client side:\n"
           "  - Inbound to File Daemon: TCP 9102"));
    mainLayout->addWidget(m_passiveCheck);

    m_waitContainer = new QWidget(this);
    QHBoxLayout *waitLayout = new QHBoxLayout(m_waitContainer);
    waitLayout->setContentsMargins(20, 0, 0, 0);
    QLabel *waitLabel = new QLabel(tr("Connection wait time:"), this);
    m_waitSpin = new QSpinBox(this);
    m_waitSpin->setRange(60, 86400);
    m_waitSpin->setValue(1800);
    m_waitSpin->setSuffix(tr(" sec"));
    m_waitSpin->setToolTip(tr("Time in seconds to wait for client-initiated connection"));
    waitLayout->addWidget(waitLabel);
    waitLayout->addWidget(m_waitSpin);
    waitLayout->addStretch();
    mainLayout->addWidget(m_waitContainer);
    m_waitContainer->setVisible(false);

    // Execute checkbox
    m_executeCheck = new QCheckBox(
        tr("Execute \"configure add client\" on Director"), this);
    m_executeCheck->setChecked(true);
    m_executeCheck->setToolTip(
        tr("If checked, the wizard will send the configure command to the Director\n"
           "on the Preview page to add this client automatically."));
    mainLayout->addWidget(m_executeCheck);

    mainLayout->addStretch();

    // Register fields
    registerField("clientName*", m_nameEdit);
    registerField("clientAddress*", m_addressEdit);
    registerField("clientPort", m_portSpin);
    registerField("clientPassword*", m_passwordEdit);

    connect(m_nameEdit, &QLineEdit::textChanged, this, &QWizardPage::completeChanged);
    connect(m_addressEdit, &QLineEdit::textChanged, this, &QWizardPage::completeChanged);
    connect(m_passwordEdit, &QLineEdit::textChanged, this, &BNewClientSettingsPage::onPasswordChanged);
    connect(m_passwordEdit, &QLineEdit::textChanged, this, &QWizardPage::completeChanged);
    connect(m_generateButton, &QPushButton::clicked, this, &BNewClientSettingsPage::onGeneratePassword);
    connect(m_resourceForm, &BResourceForm::valueChanged, this, &BNewClientSettingsPage::onResourceFormChanged);
    connect(m_passiveCheck, &QCheckBox::toggled, this, &BNewClientSettingsPage::onPassiveChanged);

    // Store settings in wizard data when leaving page
    connect(m_executeCheck, &QCheckBox::toggled, this, [this](bool checked) {
        auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
        if (wiz) wiz->data().executeConfigureCmd = checked;
    });
}

void BNewClientSettingsPage::initializePage()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());

    // Check ACL permissions — hide execute checkbox if no configure access
    BConnectionProfile profile = BSettings::instance().lastUsedProfile();
    if (profile.isValid() && !profile.aclCommand.isEmpty()) {
        bool hasConfigureAccess = false;
        for (const QString &acl : profile.aclCommand) {
            if (acl == "*all*" || acl.contains("configure", Qt::CaseInsensitive)) {
                hasConfigureAccess = true;
                break;
            }
        }
        if (!hasConfigureAccess) {
            m_executeCheck->setChecked(false);
            m_executeCheck->setVisible(false);
            if (wiz) wiz->data().executeConfigureCmd = false;
        }
    }

    if (!m_initialized) {
        // First initialization - set defaults based on connection profile
        if (profile.isValid() && !profile.tlsUsePSK) {
            // x509 mode - set TLS Require to true
            m_resourceForm->setDirectiveValue("TLS Require", true);
            if (wiz) wiz->data().tlsRequire = true;
        } else {
            // PSK mode - set TLS Require to false
            m_resourceForm->setDirectiveValue("TLS Require", false);
            if (wiz) wiz->data().tlsRequire = false;
        }
        m_initialized = true;
    } else {
        // Returning to page - restore values from wizard data
        restoreFormValues();
    }
}

void BNewClientSettingsPage::cleanupPage()
{
    // Save all form values before leaving the page
    collectFormValues();
}

void BNewClientSettingsPage::restoreFormValues()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return;

    const BNewClientWizardData &d = wiz->data();

    // Restore basic fields
    m_nameEdit->setText(d.clientName);
    m_addressEdit->setText(d.clientAddress);
    m_portSpin->setValue(d.fdPort);
    if (!d.password.isEmpty()) {
        m_passwordEdit->setText(d.password);
    }

    // Restore TLS settings
    m_resourceForm->setDirectiveValue("TLS Require", d.tlsRequire);
#ifdef Q_OS_WIN
    if (!d.tlsCertificateFile.isEmpty()) {
        m_resourceForm->setDirectiveValue("TLS Certificate File", d.tlsCertificateFile);
    }
#else
    if (!d.tlsCaCertificateFile.isEmpty()) {
        m_resourceForm->setDirectiveValue("TLS CA Certificate File", d.tlsCaCertificateFile);
    }
    if (!d.tlsCertificate.isEmpty()) {
        m_resourceForm->setDirectiveValue("TLS Certificate", d.tlsCertificate);
    }
    if (!d.tlsKey.isEmpty()) {
        m_resourceForm->setDirectiveValue("TLS Key", d.tlsKey);
    }
#endif

    // Restore NAT settings
    m_passiveCheck->setChecked(d.passive);
    m_waitSpin->setValue(d.connectionFromClientWait);
    m_waitContainer->setVisible(d.passive);
}

bool BNewClientSettingsPage::isComplete() const
{
    return !m_nameEdit->text().trimmed().isEmpty()
        && !m_addressEdit->text().trimmed().isEmpty()
        && !m_passwordEdit->text().trimmed().isEmpty();
}

void BNewClientSettingsPage::onGeneratePassword()
{
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString password;
    for (int i = 0; i < 32; ++i) {
        int idx = QRandomGenerator::global()->bounded(chars.length());
        password.append(chars.at(idx));
    }
    m_passwordEdit->setText(password);
}

void BNewClientSettingsPage::onPasswordChanged(const QString &text)
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return;

    QString trimmed = text.trimmed();
    if (trimmed.isEmpty()) {
        m_md5Label->setText(tr("(enter password)"));
        wiz->data().password.clear();
        wiz->data().passwordHash.clear();
        return;
    }

    // Compute MD5 hash
    QByteArray hash = QCryptographicHash::hash(trimmed.toLatin1(), QCryptographicHash::Md5);
    QString hashHex = QString::fromLatin1(hash.toHex());

    m_md5Label->setText(QString("[md5]%1").arg(hashHex));

    // Store in wizard data
    wiz->data().password = trimmed;
    wiz->data().passwordHash = hashHex;

    // Collect all form values
    collectFormValues();
}

void BNewClientSettingsPage::onResourceFormChanged()
{
    collectFormValues();
}

void BNewClientSettingsPage::onPassiveChanged(bool checked)
{
    m_waitContainer->setVisible(checked);
    collectFormValues();
}

void BNewClientSettingsPage::collectFormValues()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return;

    // Basic settings
    wiz->data().clientName = m_nameEdit->text().trimmed();
    wiz->data().clientAddress = m_addressEdit->text().trimmed();
    wiz->data().fdPort = m_portSpin->value();

    // Collect TLS settings from resource form
    wiz->data().tlsRequire = m_resourceForm->directiveValue("TLS Require").toBool();

#ifdef Q_OS_WIN
    wiz->data().tlsCertificateFile = m_resourceForm->directiveValue("TLS Certificate File").toString();
#else
    wiz->data().tlsCaCertificateFile = m_resourceForm->directiveValue("TLS CA Certificate File").toString();
    wiz->data().tlsCertificate = m_resourceForm->directiveValue("TLS Certificate").toString();
    wiz->data().tlsKey = m_resourceForm->directiveValue("TLS Key").toString();
#endif

    // Collect NAT settings from simple checkbox + spinner
    wiz->data().passive = m_passiveCheck->isChecked();
    wiz->data().connectionFromClientWait = m_waitSpin->value();
}

// ============================================================================
// Page 3: Preview & Export
// ============================================================================

BNewClientPreviewPage::BNewClientPreviewPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Preview & Export"));
    setSubTitle(tr("Review generated configuration files, validate, and export."));

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Main splitter: top = tabs with resource widgets, bottom = raw text
    m_mainSplitter = new QSplitter(Qt::Vertical, this);

    // Tab widget for structured resource views
    m_tabWidget = new QTabWidget(this);

    m_fdDirectorWidget = new BResourceWidget("Director", nullptr, this);
    m_tabWidget->addTab(m_fdDirectorWidget, tr("FD: Director"));

    m_fdClientWidget = new BResourceWidget("FileDaemon", nullptr, this);
    m_tabWidget->addTab(m_fdClientWidget, tr("FD: Client"));

    m_fdMessagesWidget = new BResourceWidget("Messages", nullptr, this);
    m_tabWidget->addTab(m_fdMessagesWidget, tr("FD: Messages"));

    m_dirClientWidget = new BResourceWidget("Client", nullptr, this);
    m_tabWidget->addTab(m_dirClientWidget, tr("Dir: Client"));

    m_mainSplitter->addWidget(m_tabWidget);

    // Raw text pane (shows deployable config for clipboard/ZIP)
    m_rawTextEdit = new QTextEdit(this);
    m_rawTextEdit->setReadOnly(true);
    m_rawTextEdit->setFont(QFont("Monospace", 9));
    m_mainSplitter->addWidget(m_rawTextEdit);

    m_mainSplitter->setSizes({400, 150});
    layout->addWidget(m_mainSplitter);

    // Validation label
    m_validationLabel = new QLabel(this);
    m_validationLabel->setWordWrap(true);
    layout->addWidget(m_validationLabel);

    // Director command preview
    m_cmdGroup = new QGroupBox(tr("Director Command"), this);
    QVBoxLayout *cmdLayout = new QVBoxLayout(m_cmdGroup);
    m_commandEdit = new QTextEdit(this);
    m_commandEdit->setReadOnly(true);
    m_commandEdit->setFont(QFont("Monospace", 9));
    m_commandEdit->setMaximumHeight(50);
    cmdLayout->addWidget(m_commandEdit);
    layout->addWidget(m_cmdGroup);

    // Button row
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    QPushButton *copyTabBtn = new QPushButton(tr("Copy Current Tab"), this);
    connect(copyTabBtn, &QPushButton::clicked, this, &BNewClientPreviewPage::onCopyCurrentTab);
    buttonLayout->addWidget(copyTabBtn);

    QPushButton *copyAllBtn = new QPushButton(tr("Copy All"), this);
    connect(copyAllBtn, &QPushButton::clicked, this, &BNewClientPreviewPage::onCopyAll);
    buttonLayout->addWidget(copyAllBtn);

    m_exportButton = new QPushButton(tr("Export ZIP..."), this);
    connect(m_exportButton, &QPushButton::clicked, this, &BNewClientPreviewPage::onExportZip);
    buttonLayout->addWidget(m_exportButton);

    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    // Progress and status
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0);
    m_progressBar->setVisible(false);
    layout->addWidget(m_progressBar);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    layout->addWidget(m_statusLabel);

    // Timeout timer for configure command
    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    m_timeoutTimer->setInterval(30000);  // 30 seconds
    connect(m_timeoutTimer, &QTimer::timeout,
            this, &BNewClientPreviewPage::onConfigureTimeout);

    // Update raw text pane when tab changes
    connect(m_tabWidget, &QTabWidget::currentChanged,
            this, [this](int) { updateRawText(); });
}

void BNewClientPreviewPage::initializePage()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return;

    // Provide director pointer to resource widgets
    BDirector *dir = wiz->director();
    m_fdDirectorWidget->setDirector(dir);
    m_fdClientWidget->setDirector(dir);
    m_fdMessagesWidget->setDirector(dir);
    m_dirClientWidget->setDirector(dir);

    // Sync data from fields (in case onPasswordChanged didn't catch latest)
    wiz->data().clientName = field("clientName").toString().trimmed();
    wiz->data().clientAddress = field("clientAddress").toString().trimmed();
    wiz->data().fdPort = field("clientPort").toInt();
    wiz->data().password = field("clientPassword").toString().trimmed();

    if (wiz->data().passwordHash.isEmpty() && !wiz->data().password.isEmpty()) {
        QByteArray hash = QCryptographicHash::hash(
            wiz->data().password.toLatin1(), QCryptographicHash::Md5);
        wiz->data().passwordHash = QString::fromLatin1(hash.toHex());
    }

    m_executed = false;
    m_waitingForReload = false;
    m_timeoutTimer->stop();
    m_statusLabel->clear();
    setLabelState(m_statusLabel, "");
    m_progressBar->setVisible(false);

    // Show/hide Director Command preview based on execute checkbox
    m_cmdGroup->setVisible(wiz->data().executeConfigureCmd);

    generateConfigs();
    parseAndDisplayResources();
    validateConfigs();
}

bool BNewClientPreviewPage::validatePage()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return true;

    // Execute configure command if requested and not yet done
    if (wiz->data().executeConfigureCmd && !m_executed) {
        executeConfigureCommand();
        return false;  // Don't close yet, wait for response
    }

    return true;
}

void BNewClientPreviewPage::generateConfigs()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return;

    auto &d = wiz->data();

    // Generate FD-side director.conf
    d.directorConf = QString(
        "Director {\n"
        "  Name = \"%1\"\n"
        "  Password = \"[md5]%2\"\n")
        .arg(d.directorName, d.passwordHash);

    // TLS settings
    d.directorConf += "  TLS Enable = yes\n";
    if (d.tlsRequire) {
        d.directorConf += "  TLS Require = yes\n";
        // Platform-specific certificate paths
#ifdef Q_OS_WIN
        if (!d.tlsCertificateFile.isEmpty()) {
            d.directorConf += QString("  TLS Certificate File = \"%1\"\n").arg(d.tlsCertificateFile);
        } else {
            d.directorConf += "  # TLS Certificate File = \"C:\\\\bareos\\\\tls\\\\client.pfx\"\n";
        }
#else
        if (!d.tlsCaCertificateFile.isEmpty()) {
            d.directorConf += QString("  TLS CA Certificate File = \"%1\"\n").arg(d.tlsCaCertificateFile);
        } else {
            d.directorConf += "  # TLS CA Certificate File = /etc/bareos/tls/ca.pem\n";
        }
        if (!d.tlsCertificate.isEmpty()) {
            d.directorConf += QString("  TLS Certificate = \"%1\"\n").arg(d.tlsCertificate);
        } else {
            d.directorConf += "  # TLS Certificate = /etc/bareos/tls/client.pem\n";
        }
        if (!d.tlsKey.isEmpty()) {
            d.directorConf += QString("  TLS Key = \"%1\"\n").arg(d.tlsKey);
        } else {
            d.directorConf += "  # TLS Key = /etc/bareos/tls/client-key.pem\n";
        }
#endif
    } else {
        d.directorConf += "  TLS Require = no\n";
    }
    d.directorConf += "}\n";

    // Generate FD-side myself.conf
    d.clientConf = QString(
        "FileDaemon {\n"
        "  Name = \"%1\"\n"
        "  FD Port = %2\n")
        .arg(d.clientName)
        .arg(d.fdPort);

    // TLS settings
    d.clientConf += "  TLS Enable = yes\n";
    if (d.tlsRequire) {
        d.clientConf += "  TLS Require = yes\n";
#ifdef Q_OS_WIN
        if (!d.tlsCertificateFile.isEmpty()) {
            d.clientConf += QString("  TLS Certificate File = \"%1\"\n").arg(d.tlsCertificateFile);
        } else {
            d.clientConf += "  # TLS Certificate File = \"C:\\\\bareos\\\\tls\\\\client.pfx\"\n";
        }
#else
        if (!d.tlsCaCertificateFile.isEmpty()) {
            d.clientConf += QString("  TLS CA Certificate File = \"%1\"\n").arg(d.tlsCaCertificateFile);
        } else {
            d.clientConf += "  # TLS CA Certificate File = /etc/bareos/tls/ca.pem\n";
        }
        if (!d.tlsCertificate.isEmpty()) {
            d.clientConf += QString("  TLS Certificate = \"%1\"\n").arg(d.tlsCertificate);
        } else {
            d.clientConf += "  # TLS Certificate = /etc/bareos/tls/client.pem\n";
        }
        if (!d.tlsKey.isEmpty()) {
            d.clientConf += QString("  TLS Key = \"%1\"\n").arg(d.tlsKey);
        } else {
            d.clientConf += "  # TLS Key = /etc/bareos/tls/client-key.pem\n";
        }
#endif
    } else {
        d.clientConf += "  TLS Require = no\n";
    }
    d.clientConf += "}\n";

    // Generate FD-side messages.conf
    d.messagesConf = QString(
        "Messages {\n"
        "  Name = \"Standard\"\n"
        "  Director = \"%1\" = all, !skipped, !restored\n"
        "}\n")
        .arg(d.directorName);

    // Generate Director-side client.conf
    d.dirClientConf = QString(
        "Client {\n"
        "  Name = \"%1\"\n"
        "  Address = \"%2\"\n"
        "  Password = \"[md5]%3\"\n"
        "  FD Port = %4\n")
        .arg(d.clientName, d.clientAddress, d.passwordHash)
        .arg(d.fdPort);

    // NAT/Passive mode settings
    if (d.passive) {
        d.dirClientConf += "  Passive = yes\n";
        d.dirClientConf += QString("  Connection From Client Wait = %1\n").arg(d.connectionFromClientWait);
    }

    d.dirClientConf += "}\n";

    // Generate configure command args (without "configure" prefix — Command::Configure adds it)
    d.configureCommand = QString(
        "add client name=%1 address=%2 password=\"[md5]%3\"")
        .arg(d.clientName, d.clientAddress, d.passwordHash);
    if (d.fdPort != 9102) {
        d.configureCommand += QString(" fdport=%1").arg(d.fdPort);
    }
    if (d.passive) {
        d.configureCommand += " passive=yes";
    }

    // Display with full "configure" prefix for user readability
    m_commandEdit->setPlainText("configure " + d.configureCommand);
}

void BNewClientPreviewPage::validateConfigs()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return;

    auto &d = wiz->data();
    d.validationErrors.clear();
    d.validationWarnings.clear();

    // Load directive schemas
    BDirectiveSchema &schema = BDirectiveSchema::instance();
    schema.loadSchemas();

    // Schema-driven subset validation: parse each config and validate present directives
    struct ConfigEntry {
        QString configText;
        QString schemaType;
        QString label;
    };

    QList<ConfigEntry> configs = {
        { d.directorConf, "Director", tr("FD Director") },
        { d.clientConf,   "Client",   tr("FD Client") },
        { d.messagesConf, "Messages", tr("FD Messages") },
        { d.dirClientConf,"Client",   tr("Dir Client") }
    };

    for (const auto &entry : configs) {
        BConfigParser parser;
        if (!parser.parseString(entry.configText, entry.label)) {
            d.validationWarnings << tr("%1: Config syntax could not be parsed.")
                                        .arg(entry.label);
            continue;
        }

        for (const BConfigResource &resource : parser.resources()) {
            for (const QString &key : resource.keys()) {
                BConfigValue val = resource.value(key);
                if (val.type() == BConfigValue::Simple) {
                    QString err = schema.validate(entry.schemaType, key, val.simpleValue());
                    if (!err.isEmpty()) {
                        d.validationErrors << tr("%1: %2").arg(entry.label, err);
                    }
                }
            }
        }
    }

    // Additional checks
    if (!d.clientName.endsWith("-fd")) {
        d.validationWarnings << tr("Client name '%1' does not end with '-fd'. "
                                   "Convention: use '<hostname>-fd'.").arg(d.clientName);
    }

    if (d.passwordHash.length() != 32) {
        d.validationErrors << tr("Password MD5 hash has invalid length (%1, expected 32).")
                                  .arg(d.passwordHash.length());
    }

    // Display validation results
    if (d.validationErrors.isEmpty() && d.validationWarnings.isEmpty()) {
        m_validationLabel->setText(tr("Validation passed."));
        setLabelState(m_validationLabel, "success");
    } else {
        QString text;
        for (const QString &e : d.validationErrors) {
            text += QString("<span style='color:red;'>ERROR: %1</span><br>").arg(e);
        }
        for (const QString &w : d.validationWarnings) {
            text += QString("<span style='color:orange;'>WARNING: %1</span><br>").arg(w);
        }
        m_validationLabel->setText(text);
    }
}

void BNewClientPreviewPage::parseAndDisplayResources()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return;

    const auto &d = wiz->data();

    // Parse FD-side Director resource
    {
        BConfigParser parser;
        if (parser.parseString(d.directorConf, "bareos-fd.d/director/" + d.directorName + ".conf")) {
            m_fdDirectorWidget->setResources(parser.resources());
        }
    }

    // Parse FD-side Client/FileDaemon resource
    {
        BConfigParser parser;
        if (parser.parseString(d.clientConf, "bareos-fd.d/client/myself.conf")) {
            m_fdClientWidget->setResources(parser.resources());
        }
    }

    // Parse FD-side Messages resource
    {
        BConfigParser parser;
        if (parser.parseString(d.messagesConf, "bareos-fd.d/messages/Standard.conf")) {
            m_fdMessagesWidget->setResources(parser.resources());
        }
    }

    // Parse Director-side Client resource
    {
        BConfigParser parser;
        if (parser.parseString(d.dirClientConf, "bareos-dir.d/client/" + d.clientName + ".conf")) {
            m_dirClientWidget->setResources(parser.resources());
        }
    }

    updateRawText();
}

void BNewClientPreviewPage::updateRawText()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return;

    const auto &d = wiz->data();
    int idx = m_tabWidget->currentIndex();

    QString rawText;
    switch (idx) {
    case 0: rawText = d.directorConf; break;
    case 1: rawText = d.clientConf; break;
    case 2: rawText = d.messagesConf; break;
    case 3: rawText = d.dirClientConf; break;
    }

    m_rawTextEdit->setPlainText(rawText);
}

void BNewClientPreviewPage::onCopyCurrentTab()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return;

    const auto &d = wiz->data();
    int idx = m_tabWidget->currentIndex();

    QString text;
    switch (idx) {
    case 0: text = d.directorConf; break;
    case 1: text = d.clientConf; break;
    case 2: text = d.messagesConf; break;
    case 3: text = d.dirClientConf; break;
    }

    if (!text.isEmpty()) {
        QApplication::clipboard()->setText(text);
        m_statusLabel->setText(tr("Copied '%1' to clipboard.").arg(m_tabWidget->tabText(idx)));
        setLabelState(m_statusLabel, "info");
    }
}

void BNewClientPreviewPage::onCopyAll()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return;

    const auto &d = wiz->data();
    QString all;
    all += "# === File Daemon (Client-side) ===\n\n";
    all += QString("# bareos-fd.d/director/%1.conf\n").arg(d.directorName);
    all += d.directorConf + "\n";
    all += QString("# bareos-fd.d/client/myself.conf\n");
    all += d.clientConf + "\n";
    all += QString("# bareos-fd.d/messages/Standard.conf\n");
    all += d.messagesConf + "\n";
    all += "# === Director-side ===\n\n";
    all += QString("# bareos-dir.d/client/%1.conf\n").arg(d.clientName);
    all += d.dirClientConf + "\n";
    all += QString("# Director command:\n# %1\n").arg(d.configureCommand);

    QApplication::clipboard()->setText(all);
    m_statusLabel->setText(tr("All configuration files copied to clipboard."));
    m_statusLabel->setStyleSheet("color: green;");
}

void BNewClientPreviewPage::onExportZip()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz) return;

    const auto &d = wiz->data();

    QString zipPath = QFileDialog::getSaveFileName(
        this, tr("Export Client Configuration"),
        QString("%1.zip").arg(d.clientName),
        tr("ZIP Archives (*.zip)"));
    if (zipPath.isEmpty()) return;

    // Create temp directory with config structure
    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        QMessageBox::warning(this, tr("Export Error"),
            tr("Failed to create temporary directory."));
        return;
    }

    QString fdPath = tempDir.path() + "/bareos-fd.d";
    QDir().mkpath(fdPath + "/director");
    QDir().mkpath(fdPath + "/client");
    QDir().mkpath(fdPath + "/messages");

    // FD-side director.conf
    QFile dirFile(fdPath + "/director/" + d.directorName + ".conf");
    if (dirFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        dirFile.write(d.directorConf.toUtf8());
        dirFile.close();
    }

    // FD-side client/myself.conf
    QFile clientFile(fdPath + "/client/myself.conf");
    if (clientFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        clientFile.write(d.clientConf.toUtf8());
        clientFile.close();
    }

    // FD-side messages.conf
    QFile msgFile(fdPath + "/messages/Standard.conf");
    if (msgFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        msgFile.write(d.messagesConf.toUtf8());
        msgFile.close();
    }

    // Director-side client config
    QString dirPath = tempDir.path() + "/bareos-dir.d";
    QDir().mkpath(dirPath + "/client");

    QFile dirClientFile(dirPath + "/client/" + d.clientName + ".conf");
    if (dirClientFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        dirClientFile.write(d.dirClientConf.toUtf8());
        dirClientFile.close();
    }

    // Write README.txt
    QFile readmeFile(tempDir.path() + "/README.txt");
    if (readmeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QString readme = QString(
            "Bareos Configuration for Client: %1\n"
            "=========================================\n\n"
            "Director: %2 (%3:%4)\n\n"
            "Contents:\n"
            "  bareos-fd.d/   - File Daemon configuration (client-side)\n"
            "  bareos-dir.d/  - Director configuration (server-side)\n\n"
            "Installation:\n"
            "  Client machine:\n"
            "    1. Copy bareos-fd.d/ to /etc/bareos/ on the client\n"
            "    2. Restart the File Daemon: systemctl restart bareos-fd\n\n"
            "  Director (alternative to 'configure add client'):\n"
            "    1. Copy bareos-dir.d/client/%1.conf to\n"
            "       /etc/bareos/bareos-dir.d/client/ on the Director\n"
            "    2. Reload the Director: systemctl reload bareos-dir\n\n"
            "Generated by Onesimus\n")
            .arg(d.clientName, d.directorName, d.directorAddress)
            .arg(d.directorPort);
        readmeFile.write(readme.toUtf8());
        readmeFile.close();
    }

    // Create ZIP using system zip command
    QProcess zipProcess;
    zipProcess.setWorkingDirectory(tempDir.path());
    QStringList zipArgs = {"-r", zipPath, "bareos-fd.d", "bareos-dir.d", "README.txt"};
    zipProcess.start("zip", zipArgs);

    if (!zipProcess.waitForFinished(10000)) {
        QMessageBox::warning(this, tr("Export Error"),
            tr("ZIP creation timed out. Is 'zip' installed?"));
        return;
    }

    if (zipProcess.exitCode() != 0) {
        QMessageBox::warning(this, tr("Export Error"),
            tr("ZIP creation failed:\n%1")
                .arg(QString::fromUtf8(zipProcess.readAllStandardError())));
        return;
    }

    m_statusLabel->setText(tr("Exported to: %1").arg(zipPath));
    m_statusLabel->setStyleSheet("color: green; font-weight: bold;");
    CLIENT_DEBUG << "Exported ZIP: " << zipPath;
}

void BNewClientPreviewPage::executeConfigureCommand()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());
    if (!wiz || !wiz->director() || !wiz->director()->isConnected()) {
        m_statusLabel->setText(tr("ERROR: No director connection available."));
        setLabelState(m_statusLabel, "error");
        return;
    }

    if (m_executed) return;
    m_executed = true;

    m_progressBar->setVisible(true);
    m_statusLabel->setText(tr("Sending configure command..."));
    setLabelState(m_statusLabel, "");

    connect(wiz->director(), &BDirector::jsonResult,
            this, &BNewClientPreviewPage::onJsonResponse);
    connect(wiz->director(), &BDirector::textResult,
            this, &BNewClientPreviewPage::onCommandResponse);

    m_timeoutTimer->start();

    CLIENT_DEBUG << "Sending: " << wiz->data().configureCommand;
    wiz->director()->doSend(BDirector::Command::Configure, wiz->data().configureCommand);
}

void BNewClientPreviewPage::onJsonResponse(BDirector::Command cmd, const QString &jsonData)
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());

    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    // Handle reload response
    if (m_waitingForReload && cmd == BDirector::Command::Reload) {
        m_waitingForReload = false;
        m_progressBar->setVisible(false);

        // Disconnect signals — we're done
        if (wiz && wiz->director()) {
            disconnect(wiz->director(), &BDirector::jsonResult,
                       this, &BNewClientPreviewPage::onJsonResponse);
            disconnect(wiz->director(), &BDirector::textResult,
                       this, &BNewClientPreviewPage::onCommandResponse);
        }

        // Check if reload succeeded: error.data.result.reload.success
        bool reloadOk = false;
        if (root.contains("error")) {
            QJsonObject data = root["error"].toObject()["data"].toObject();
            QJsonObject reloadResult = data["result"].toObject()["reload"].toObject();
            reloadOk = reloadResult["success"].toBool(false);
        } else if (result.contains("reload")) {
            reloadOk = result["reload"].toObject()["success"].toBool(false);
        }

        QString clientName = wiz ? wiz->data().clientName : "?";
        if (reloadOk) {
            CLIENT_DEBUG << "Director reload OK";
            m_statusLabel->setText(
                tr("Client '%1' added and Director reloaded successfully.\n"
                   "Deploy the FD config files to the client machine and restart bareos-fd.")
                    .arg(clientName));
            setLabelState(m_statusLabel, "success");
        } else {
            CLIENT_DEBUG << "Director reload FAILED";
            m_statusLabel->setText(
                tr("Client '%1' added to Director, but reload failed.\n"
                   "Run 'reload' manually on the Director or restart bareos-dir.\n"
                   "Deploy the FD config files to the client machine and restart bareos-fd.")
                    .arg(clientName));
            setLabelState(m_statusLabel, "warning");
        }
        return;
    }

    // Handle configure response
    if (cmd != BDirector::Command::Configure) return;

    m_timeoutTimer->stop();

    // Not a configure response — ignore
    if (!result.contains("configure") && !jsonData.contains("created", Qt::CaseInsensitive)) {
        return;
    }

    // Bareos JSON API: success is indicated by result.configure.add existing
    QJsonObject configureObj = result["configure"].toObject();
    bool success = configureObj.contains("add")
                || jsonData.contains("created", Qt::CaseInsensitive)
                || jsonData.contains("success", Qt::CaseInsensitive);

    if (success) {
        // Extract meaningful message from JSON response
        QString message;
        if (configureObj.contains("add")) {
            QJsonObject addObj = configureObj["add"].toObject();
            message = tr("Client '%1' created at: %2")
                          .arg(addObj["name"].toString(), addObj["filename"].toString());
        }

        CLIENT_DEBUG << "Configure OK: " << message;

        // Send reload to Director
        if (wiz && wiz->director() && wiz->director()->isConnected()) {
            m_waitingForReload = true;
            wiz->director()->doSend(BDirector::Command::Reload);
            CLIENT_DEBUG << "Sent reload to Director";
            m_statusLabel->setText(
                tr("Client '%1' added to Director. Reloading Director...")
                    .arg(wiz->data().clientName));
            setLabelState(m_statusLabel, "info");
        } else {
            m_progressBar->setVisible(false);
            // Disconnect signals — no reload possible
            if (wiz && wiz->director()) {
                disconnect(wiz->director(), &BDirector::jsonResult,
                           this, &BNewClientPreviewPage::onJsonResponse);
                disconnect(wiz->director(), &BDirector::textResult,
                           this, &BNewClientPreviewPage::onCommandResponse);
            }
            m_statusLabel->setText(
                tr("Client '%1' added to Director.\n"
                   "Deploy the FD config files to the client machine and restart bareos-fd.")
                    .arg(wiz ? wiz->data().clientName : "?"));
            setLabelState(m_statusLabel, "success");
        }
    } else {
        m_progressBar->setVisible(false);
        // Disconnect signals on failure
        if (wiz && wiz->director()) {
            disconnect(wiz->director(), &BDirector::jsonResult,
                       this, &BNewClientPreviewPage::onJsonResponse);
            disconnect(wiz->director(), &BDirector::textResult,
                       this, &BNewClientPreviewPage::onCommandResponse);
        }

        QString errorMsg;
        if (root.contains("error")) {
            QJsonObject errorObj = root["error"].toObject();
            errorMsg = errorObj["message"].toString();
        } else {
            errorMsg = jsonData.left(200);
        }

        CLIENT_DEBUG << "Configure FAILED: " << errorMsg;
        m_statusLabel->setText(
            tr("Failed to add client to Director: %1\n"
               "You can still export the FD config files manually.").arg(errorMsg));
        setLabelState(m_statusLabel, "error");
    }
}

void BNewClientPreviewPage::onCommandResponse(BDirector::Command cmd, const QString &response)
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());

    // Handle reload response (plain text)
    if (m_waitingForReload && cmd == BDirector::Command::Reload) {
        m_waitingForReload = false;
        m_progressBar->setVisible(false);

        if (wiz && wiz->director()) {
            disconnect(wiz->director(), &BDirector::jsonResult,
                       this, &BNewClientPreviewPage::onJsonResponse);
            disconnect(wiz->director(), &BDirector::textResult,
                       this, &BNewClientPreviewPage::onCommandResponse);
        }

        QString clientName = wiz ? wiz->data().clientName : "?";
        bool reloadOk = response.contains("success", Qt::CaseInsensitive);

        if (reloadOk) {
            CLIENT_DEBUG << "Director reload OK";
            m_statusLabel->setText(
                tr("Client '%1' added and Director reloaded successfully.\n"
                   "Deploy the FD config files to the client machine and restart bareos-fd.")
                    .arg(clientName));
            setLabelState(m_statusLabel, "success");
        } else {
            CLIENT_DEBUG << "Director reload FAILED: " << response.left(200);
            m_statusLabel->setText(
                tr("Client '%1' added to Director, but reload failed.\n"
                   "Run 'reload' manually on the Director or restart bareos-dir.\n"
                   "Deploy the FD config files to the client machine and restart bareos-fd.")
                    .arg(clientName));
            setLabelState(m_statusLabel, "warning");
        }
        return;
    }

    // Handle configure response
    if (cmd != BDirector::Command::Configure) return;

    m_timeoutTimer->stop();

    bool success = response.contains("created", Qt::CaseInsensitive)
                || response.contains("success", Qt::CaseInsensitive)
                || response.contains("configure add", Qt::CaseInsensitive);

    if (success) {
        CLIENT_DEBUG << "Configure OK: " << response.left(200);

        // Send reload to Director
        if (wiz && wiz->director() && wiz->director()->isConnected()) {
            m_waitingForReload = true;
            wiz->director()->doSend(BDirector::Command::Reload);
            CLIENT_DEBUG << "Sent reload to Director";
            m_statusLabel->setText(
                tr("Client '%1' added to Director. Reloading Director...")
                    .arg(wiz->data().clientName));
            setLabelState(m_statusLabel, "info");
        } else {
            m_progressBar->setVisible(false);
            if (wiz && wiz->director()) {
                disconnect(wiz->director(), &BDirector::jsonResult,
                           this, &BNewClientPreviewPage::onJsonResponse);
                disconnect(wiz->director(), &BDirector::textResult,
                           this, &BNewClientPreviewPage::onCommandResponse);
            }
            m_statusLabel->setText(
                tr("Client '%1' added to Director.\n"
                   "Deploy the FD config files to the client machine and restart bareos-fd.")
                    .arg(wiz ? wiz->data().clientName : "?"));
            setLabelState(m_statusLabel, "success");
        }
    } else {
        m_progressBar->setVisible(false);
        if (wiz && wiz->director()) {
            disconnect(wiz->director(), &BDirector::jsonResult,
                       this, &BNewClientPreviewPage::onJsonResponse);
            disconnect(wiz->director(), &BDirector::textResult,
                       this, &BNewClientPreviewPage::onCommandResponse);
        }
        CLIENT_DEBUG << "Configure FAILED: " << response.left(200);
        m_statusLabel->setText(
            tr("Failed to add client to Director: %1\n"
               "You can still export the FD config files manually.").arg(response.left(200)));
        setLabelState(m_statusLabel, "error");
    }
}

void BNewClientPreviewPage::onConfigureTimeout()
{
    auto *wiz = qobject_cast<BNewClientWizard*>(wizard());

    m_progressBar->setVisible(false);
    CLIENT_DEBUG << "Configure command timed out";

    QMessageBox::StandardButton reply = QMessageBox::warning(
        this,
        tr("Command Timeout"),
        tr("The 'configure add client' command did not respond within 30 seconds.\n\n"
           "The Director may be busy or the connection may have been lost.\n\n"
           "Retry the command?"),
        QMessageBox::Retry | QMessageBox::Cancel,
        QMessageBox::Retry);

    if (reply == QMessageBox::Retry) {
        // Reset state for retry
        m_executed = false;
        m_waitingForReload = false;

        // Disconnect previous signals
        if (wiz && wiz->director()) {
            disconnect(wiz->director(), &BDirector::jsonResult,
                       this, &BNewClientPreviewPage::onJsonResponse);
            disconnect(wiz->director(), &BDirector::textResult,
                       this, &BNewClientPreviewPage::onCommandResponse);
        }

        // Retry
        executeConfigureCommand();
    } else {
        // User cancelled — disconnect and show status
        if (wiz && wiz->director()) {
            disconnect(wiz->director(), &BDirector::jsonResult,
                       this, &BNewClientPreviewPage::onJsonResponse);
            disconnect(wiz->director(), &BDirector::textResult,
                       this, &BNewClientPreviewPage::onCommandResponse);
        }

        m_statusLabel->setText(
            tr("Configure command timed out.\n"
               "You can export the FD config files and configure the Director manually."));
        setLabelState(m_statusLabel, "warning");
    }
}
