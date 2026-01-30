/**
 * @file bconnectionwizard.cpp
 * @brief Director Configuration Wizard (Q&A Style)
 */

#include "bconnectionwizard.h"
#include "bareosdirector.h"
#include "bcertificategenerator.h"
#include "bsettings.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

// ============================================================================
// BConnectionWizard
// ============================================================================

BConnectionWizard::BConnectionWizard(QWidget *parent)
    : QWizard(parent)
{
    setWindowTitle(tr("Connection Setup"));
    setWizardStyle(QWizard::ModernStyle);
    setMinimumSize(500, 400);
    setWindowIcon(QIcon::fromTheme("network-server"));

    setPage(Page_Welcome, new WelcomePage(this));
    setPage(Page_Server, new ServerPage(this));
    setPage(Page_Credentials, new CredentialsPage(this));
    setPage(Page_AuthMethod, new AuthMethodPage(this));
    setPage(Page_TLS, new TLSPage(this));
    setPage(Page_Test, new TestPage(this));
    setPage(Page_ProfileName, new ProfileNamePage(this));

    m_profile = BConnectionProfile::create(tr("New Connection"));
}

BConnectionWizard::~BConnectionWizard() = default;

BConnectionProfile BConnectionWizard::profile() const
{
    BConnectionProfile p = m_profile;

    p.name = field("profileName").toString();
    p.host = field("host").toString();
    p.port = field("port").toInt();
    p.directorName = field("directorName").toString();
    p.consoleName = field("consoleName").toString();

    if (field("savePassword").toBool()) {
        p.password = field("password").toString();
    } else {
        p.password.clear();
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

    return p;
}

void BConnectionWizard::setProfile(const BConnectionProfile &profile)
{
    m_profile = profile;
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

    m_layout->addLayout(form);
    setHint(tr("The default port is 9101."));
    m_layout->addStretch();

    registerField("host*", m_hostEdit);
    registerField("port", m_portSpin);
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

    m_layout->addLayout(form);

    m_saveCheck = new QCheckBox(tr("Remember password"), this);
    m_saveCheck->setChecked(true);
    m_layout->addWidget(m_saveCheck);

    setHint(tr("These are defined in your Bareos console configuration."));
    m_layout->addStretch();

    registerField("directorName*", m_directorEdit);
    registerField("consoleName*", m_consoleEdit);
    registerField("password*", m_passwordEdit);
    registerField("savePassword", m_saveCheck);
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

    setHint(tr("TLS-PSK is the default for Bareos 18.2+."));
    m_layout->addStretch();

    registerField("authMethod", this, "authMethod");
    connect(m_group, &QButtonGroup::idClicked, this, &AuthMethodPage::onSelectionChanged);
}

int AuthMethodPage::nextId() const
{
    if (m_certRadio->isChecked()) {
        return BConnectionWizard::Page_TLS;
    }
    return BConnectionWizard::Page_Test;
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

    m_layout->addLayout(form);

    m_generateButton = new QPushButton(tr("Generate Certificates..."), this);
    m_layout->addWidget(m_generateButton);

    setHint(tr("No certificates? Click Generate to create them."));
    m_layout->addStretch();

    registerField("caCert", m_caCertEdit);
    registerField("clientCert", m_clientCertEdit);
    registerField("clientKey", m_clientKeyEdit);

    connect(m_caBrowseButton, &QPushButton::clicked, this, &TLSPage::browseCaCert);
    connect(m_clientCertBrowseButton, &QPushButton::clicked, this, &TLSPage::browseClientCert);
    connect(m_clientKeyBrowseButton, &QPushButton::clicked, this, &TLSPage::browseClientKey);
    connect(m_generateButton, &QPushButton::clicked, this, &TLSPage::generateCertificates);
}

bool TLSPage::validatePage()
{
    QString ca = m_caCertEdit->text().trimmed();
    QString cert = m_clientCertEdit->text().trimmed();
    QString key = m_clientKeyEdit->text().trimmed();

    if (ca.isEmpty() || cert.isEmpty() || key.isEmpty()) {
        QMessageBox::warning(this, tr("Missing"), tr("Please provide all certificate files."));
        return false;
    }
    if (!QFile::exists(ca) || !QFile::exists(cert) || !QFile::exists(key)) {
        QMessageBox::warning(this, tr("Not Found"), tr("One or more certificate files not found."));
        return false;
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
    QString f = QFileDialog::getOpenFileName(this, tr("Client Certificate"), "/etc/bareos/ssl", tr("*.pem *.crt"));
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
        m_clientCertEdit->setText(dialog.clientCertPath());
        m_clientKeyEdit->setText(dialog.clientKeyPath());
    }
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
int TestPage::nextId() const { return BConnectionWizard::Page_ProfileName; }

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
        QString cert = field("clientCert").toString();
        QString key = field("clientKey").toString();
        if (!ca.isEmpty()) tls.tlsCaCertFile = QSharedPointer<QFile>(new QFile(ca));
        if (!cert.isEmpty()) tls.tlsCertFile = QSharedPointer<QFile>(new QFile(cert));
        if (!key.isEmpty()) tls.tlsKeyFile = QSharedPointer<QFile>(new QFile(key));
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
    m_logEdit->append(QString("<span style='color:%1;'>%2</span>").arg(err ? "#c00000" : "#333", msg));
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

    setHint(tr("This name helps identify the connection."));
    m_layout->addStretch();

    registerField("profileName*", m_edit);
    registerField("setDefault", m_defaultCheck);
    registerField("connectNow", m_connectCheck);
}

void ProfileNamePage::initializePage()
{
    QString host = field("host").toString();
    if (m_edit->text().isEmpty() && !host.isEmpty()) m_edit->setText(host);
}
