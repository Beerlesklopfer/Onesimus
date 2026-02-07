/**
 * @file bprofilesettingsdialog.cpp
 * @brief Implementation of the advanced profile settings dialog
 */

#include "bprofilesettingsdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>

BProfileSettingsDialog::BProfileSettingsDialog(const BConnectionProfile &profile, QWidget *parent)
    : QDialog(parent)
    , m_profile(profile)
{
    setWindowTitle(tr("Advanced Profile Settings - %1").arg(profile.name));
    setMinimumSize(600, 500);
    setupUi();
    loadProfile();
}

BConnectionProfile BProfileSettingsDialog::profile() const
{
    return m_profile;
}

void BProfileSettingsDialog::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Tab widget
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(createGeneralTab(), tr("General"));
    m_tabWidget->addTab(createTlsTab(), tr("TLS"));
    m_tabWidget->addTab(createAclTab(), tr("ACL"));
    mainLayout->addWidget(m_tabWidget);

    // Button box
    m_buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults,
        this);
    m_resetButton = m_buttonBox->button(QDialogButtonBox::RestoreDefaults);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &BProfileSettingsDialog::onAccept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_resetButton, &QPushButton::clicked, this, &BProfileSettingsDialog::onResetDefaults);

    mainLayout->addWidget(m_buttonBox);
}

QWidget* BProfileSettingsDialog::createGeneralTab()
{
    QWidget *widget = new QWidget(this);
    QFormLayout *layout = new QFormLayout(widget);

    // Description
    m_descriptionEdit = new QLineEdit(widget);
    m_descriptionEdit->setPlaceholderText(tr("Optional description for this connection"));
    layout->addRow(tr("Description:"), m_descriptionEdit);

    // Heartbeat interval
    m_heartbeatSpin = new QSpinBox(widget);
    m_heartbeatSpin->setRange(0, 3600);
    m_heartbeatSpin->setSuffix(tr(" seconds"));
    m_heartbeatSpin->setSpecialValueText(tr("Disabled"));
    m_heartbeatSpin->setToolTip(tr("Keepalive interval (0 = disabled)"));
    layout->addRow(tr("Heartbeat Interval:"), m_heartbeatSpin);

    // Director Profile reference
    QWidget *profileWidget = new QWidget(widget);
    QHBoxLayout *profileLayout = new QHBoxLayout(profileWidget);
    profileLayout->setContentsMargins(0, 0, 0, 0);
    m_profileEdit = new QLineEdit(profileWidget);
    m_profileEdit->setPlaceholderText(tr("e.g., operator or readonly"));
    m_profileEdit->setToolTip(tr("Reference to a Profile resource on the Director.\n"
                                  "The Profile defines ACLs for this console."));
    profileLayout->addWidget(m_profileEdit);
    layout->addRow(tr("Director Profile:"), profileWidget);

    // Info label
    QLabel *infoLabel = new QLabel(
        tr("<b>Note:</b> The Director Profile is a server-side resource that defines "
           "permissions. If set, the ACLs in this profile are ignored in favor of "
           "the server-side Profile."),
        widget);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: #666; font-size: 11px;");
    layout->addRow(infoLabel);

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    return widget;
}

QWidget* BProfileSettingsDialog::createTlsTab()
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(widget);

    // TLS Options Group
    QGroupBox *optionsGroup = new QGroupBox(tr("TLS Options"), widget);
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);

    m_tlsRequireCheck = new QCheckBox(tr("TLS Required"), optionsGroup);
    m_tlsRequireCheck->setToolTip(tr("Require TLS for all connections (recommended)"));
    optionsLayout->addWidget(m_tlsRequireCheck);

    m_tlsAuthenticateCheck = new QCheckBox(tr("TLS Authenticate Only"), optionsGroup);
    m_tlsAuthenticateCheck->setToolTip(tr("Use TLS only for authentication, not for data encryption"));
    optionsLayout->addWidget(m_tlsAuthenticateCheck);

    mainLayout->addWidget(optionsGroup);

    // Cipher Configuration Group
    QGroupBox *cipherGroup = new QGroupBox(tr("Cipher Configuration"), widget);
    QFormLayout *cipherLayout = new QFormLayout(cipherGroup);

    m_tlsCipherListEdit = new QLineEdit(cipherGroup);
    m_tlsCipherListEdit->setPlaceholderText(tr("TLSv1.2 ciphers (colon-separated)"));
    m_tlsCipherListEdit->setToolTip(tr("OpenSSL cipher list for TLSv1.2 connections\n"
                                        "Example: HIGH:!aNULL:!MD5"));
    cipherLayout->addRow(tr("Cipher List (TLSv1.2):"), m_tlsCipherListEdit);

    m_tlsCipherSuitesEdit = new QLineEdit(cipherGroup);
    m_tlsCipherSuitesEdit->setPlaceholderText(tr("TLSv1.3 cipher suites (colon-separated)"));
    m_tlsCipherSuitesEdit->setToolTip(tr("OpenSSL cipher suites for TLSv1.3 connections\n"
                                          "Example: TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256"));
    cipherLayout->addRow(tr("Cipher Suites (TLSv1.3):"), m_tlsCipherSuitesEdit);

    m_tlsProtocolEdit = new QLineEdit(cipherGroup);
    m_tlsProtocolEdit->setPlaceholderText(tr("e.g., -ALL,TLSv1.2,TLSv1.3"));
    m_tlsProtocolEdit->setToolTip(tr("OpenSSL protocol string"));
    cipherLayout->addRow(tr("Protocol:"), m_tlsProtocolEdit);

    mainLayout->addWidget(cipherGroup);

    // Certificate Files Group
    QGroupBox *certGroup = new QGroupBox(tr("Certificate Files"), widget);
    QGridLayout *certLayout = new QGridLayout(certGroup);

    // DH File
    certLayout->addWidget(new QLabel(tr("DH Parameters:"), certGroup), 0, 0);
    m_tlsDhFileEdit = new QLineEdit(certGroup);
    m_tlsDhFileEdit->setToolTip(tr("Path to Diffie-Hellman parameters file"));
    certLayout->addWidget(m_tlsDhFileEdit, 0, 1);
    m_browseDhFileButton = new QPushButton(tr("..."), certGroup);
    m_browseDhFileButton->setMaximumWidth(30);
    connect(m_browseDhFileButton, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getOpenFileName(this, tr("Select DH Parameters File"),
                                                     QString(), tr("PEM Files (*.pem);;All Files (*)"));
        if (!file.isEmpty()) {
            m_tlsDhFileEdit->setText(file);
        }
    });
    certLayout->addWidget(m_browseDhFileButton, 0, 2);

    // CRL File
    certLayout->addWidget(new QLabel(tr("CRL File:"), certGroup), 1, 0);
    m_tlsCrlFileEdit = new QLineEdit(certGroup);
    m_tlsCrlFileEdit->setToolTip(tr("Path to Certificate Revocation List file"));
    certLayout->addWidget(m_tlsCrlFileEdit, 1, 1);
    m_browseCrlFileButton = new QPushButton(tr("..."), certGroup);
    m_browseCrlFileButton->setMaximumWidth(30);
    connect(m_browseCrlFileButton, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getOpenFileName(this, tr("Select CRL File"),
                                                     QString(), tr("CRL Files (*.crl *.pem);;All Files (*)"));
        if (!file.isEmpty()) {
            m_tlsCrlFileEdit->setText(file);
        }
    });
    certLayout->addWidget(m_browseCrlFileButton, 1, 2);

    mainLayout->addWidget(certGroup);

    // Allowed CNs Group
    QGroupBox *cnGroup = new QGroupBox(tr("Allowed Certificate Common Names"), widget);
    QVBoxLayout *cnLayout = new QVBoxLayout(cnGroup);

    m_tlsAllowedCnEdit = new QTextEdit(cnGroup);
    m_tlsAllowedCnEdit->setPlaceholderText(tr("One Common Name per line"));
    m_tlsAllowedCnEdit->setToolTip(tr("List of allowed certificate Common Names.\n"
                                       "Leave empty to allow all valid certificates."));
    m_tlsAllowedCnEdit->setMaximumHeight(80);
    cnLayout->addWidget(m_tlsAllowedCnEdit);

    mainLayout->addWidget(cnGroup);

    mainLayout->addStretch();

    return widget;
}

QWidget* BProfileSettingsDialog::createAclTab()
{
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *widget = new QWidget(scrollArea);
    QVBoxLayout *mainLayout = new QVBoxLayout(widget);

    // Info label
    QLabel *infoLabel = new QLabel(
        tr("<b>Access Control Lists (ACLs)</b><br>"
           "Define which resources this console can access. One entry per line. "
           "Use <code>*all*</code> to allow all resources of that type. "
           "Regex patterns are supported for Job ACL."),
        widget);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("margin-bottom: 10px;");
    mainLayout->addWidget(infoLabel);

    // Grid layout for ACL fields (2 columns)
    QGridLayout *aclLayout = new QGridLayout();
    aclLayout->setSpacing(10);

    auto createAclGroup = [widget](const QString &title, const QString &tooltip) -> QTextEdit* {
        QTextEdit *edit = new QTextEdit(widget);
        edit->setPlaceholderText(title);
        edit->setToolTip(tooltip);
        edit->setMaximumHeight(60);
        return edit;
    };

    // Row 0: Job ACL, Client ACL
    aclLayout->addWidget(new QLabel(tr("Job ACL:"), widget), 0, 0);
    m_aclJobEdit = createAclGroup(tr("Allowed jobs"), tr("List of allowed job names (regex supported)"));
    aclLayout->addWidget(m_aclJobEdit, 1, 0);

    aclLayout->addWidget(new QLabel(tr("Client ACL:"), widget), 0, 1);
    m_aclClientEdit = createAclGroup(tr("Allowed clients"), tr("List of allowed client names"));
    aclLayout->addWidget(m_aclClientEdit, 1, 1);

    // Row 2: Storage ACL, Pool ACL
    aclLayout->addWidget(new QLabel(tr("Storage ACL:"), widget), 2, 0);
    m_aclStorageEdit = createAclGroup(tr("Allowed storages"), tr("List of allowed storage names"));
    aclLayout->addWidget(m_aclStorageEdit, 3, 0);

    aclLayout->addWidget(new QLabel(tr("Pool ACL:"), widget), 2, 1);
    m_aclPoolEdit = createAclGroup(tr("Allowed pools"), tr("List of allowed pool names"));
    aclLayout->addWidget(m_aclPoolEdit, 3, 1);

    // Row 4: Schedule ACL, FileSet ACL
    aclLayout->addWidget(new QLabel(tr("Schedule ACL:"), widget), 4, 0);
    m_aclScheduleEdit = createAclGroup(tr("Allowed schedules"), tr("List of allowed schedule names"));
    aclLayout->addWidget(m_aclScheduleEdit, 5, 0);

    aclLayout->addWidget(new QLabel(tr("FileSet ACL:"), widget), 4, 1);
    m_aclFileSetEdit = createAclGroup(tr("Allowed filesets"), tr("List of allowed fileset names"));
    aclLayout->addWidget(m_aclFileSetEdit, 5, 1);

    // Row 6: Catalog ACL, Command ACL
    aclLayout->addWidget(new QLabel(tr("Catalog ACL:"), widget), 6, 0);
    m_aclCatalogEdit = createAclGroup(tr("Allowed catalogs"), tr("List of allowed catalog names"));
    aclLayout->addWidget(m_aclCatalogEdit, 7, 0);

    aclLayout->addWidget(new QLabel(tr("Command ACL:"), widget), 6, 1);
    m_aclCommandEdit = createAclGroup(tr("Allowed commands"), tr("List of allowed commands"));
    aclLayout->addWidget(m_aclCommandEdit, 7, 1);

    // Row 8: Where ACL, Plugin Options ACL
    aclLayout->addWidget(new QLabel(tr("Where ACL:"), widget), 8, 0);
    m_aclWhereEdit = createAclGroup(tr("Allowed restore locations"), tr("List of allowed restore locations"));
    aclLayout->addWidget(m_aclWhereEdit, 9, 0);

    aclLayout->addWidget(new QLabel(tr("Plugin Options ACL:"), widget), 8, 1);
    m_aclPluginOptionsEdit = createAclGroup(tr("Allowed plugin options"), tr("List of allowed plugin options"));
    aclLayout->addWidget(m_aclPluginOptionsEdit, 9, 1);

    mainLayout->addLayout(aclLayout);
    mainLayout->addStretch();

    scrollArea->setWidget(widget);
    return scrollArea;
}

void BProfileSettingsDialog::loadProfile()
{
    // General tab
    m_descriptionEdit->setText(m_profile.description);
    m_heartbeatSpin->setValue(m_profile.heartbeatInterval);
    m_profileEdit->setText(m_profile.profile);

    // TLS tab
    m_tlsRequireCheck->setChecked(m_profile.tlsRequire);
    m_tlsAuthenticateCheck->setChecked(m_profile.tlsAuthenticate);
    m_tlsCipherListEdit->setText(m_profile.tlsCipherList);
    m_tlsCipherSuitesEdit->setText(m_profile.tlsCipherSuites);
    m_tlsProtocolEdit->setText(m_profile.tlsProtocol);
    m_tlsDhFileEdit->setText(m_profile.tlsDhFile);
    m_tlsCrlFileEdit->setText(m_profile.tlsCrlFile);
    m_tlsAllowedCnEdit->setPlainText(m_profile.tlsAllowedCn.join("\n"));

    // ACL tab
    m_aclJobEdit->setPlainText(m_profile.aclJob.join("\n"));
    m_aclClientEdit->setPlainText(m_profile.aclClient.join("\n"));
    m_aclStorageEdit->setPlainText(m_profile.aclStorage.join("\n"));
    m_aclScheduleEdit->setPlainText(m_profile.aclSchedule.join("\n"));
    m_aclPoolEdit->setPlainText(m_profile.aclPool.join("\n"));
    m_aclFileSetEdit->setPlainText(m_profile.aclFileSet.join("\n"));
    m_aclCatalogEdit->setPlainText(m_profile.aclCatalog.join("\n"));
    m_aclCommandEdit->setPlainText(m_profile.aclCommand.join("\n"));
    m_aclWhereEdit->setPlainText(m_profile.aclWhere.join("\n"));
    m_aclPluginOptionsEdit->setPlainText(m_profile.aclPluginOptions.join("\n"));
}

void BProfileSettingsDialog::saveProfile()
{
    // General tab
    m_profile.description = m_descriptionEdit->text().trimmed();
    m_profile.heartbeatInterval = m_heartbeatSpin->value();
    m_profile.profile = m_profileEdit->text().trimmed();

    // TLS tab
    m_profile.tlsRequire = m_tlsRequireCheck->isChecked();
    m_profile.tlsAuthenticate = m_tlsAuthenticateCheck->isChecked();
    m_profile.tlsCipherList = m_tlsCipherListEdit->text().trimmed();
    m_profile.tlsCipherSuites = m_tlsCipherSuitesEdit->text().trimmed();
    m_profile.tlsProtocol = m_tlsProtocolEdit->text().trimmed();
    m_profile.tlsDhFile = m_tlsDhFileEdit->text().trimmed();
    m_profile.tlsCrlFile = m_tlsCrlFileEdit->text().trimmed();

    // Parse allowed CNs (one per line)
    QString cnText = m_tlsAllowedCnEdit->toPlainText().trimmed();
    m_profile.tlsAllowedCn = cnText.isEmpty() ? QStringList() :
                             cnText.split('\n', Qt::SkipEmptyParts);

    // ACL tab - helper lambda to parse text to string list
    auto parseAcl = [](QTextEdit *edit) -> QStringList {
        QString text = edit->toPlainText().trimmed();
        if (text.isEmpty()) return QStringList();
        return text.split('\n', Qt::SkipEmptyParts);
    };

    m_profile.aclJob = parseAcl(m_aclJobEdit);
    m_profile.aclClient = parseAcl(m_aclClientEdit);
    m_profile.aclStorage = parseAcl(m_aclStorageEdit);
    m_profile.aclSchedule = parseAcl(m_aclScheduleEdit);
    m_profile.aclPool = parseAcl(m_aclPoolEdit);
    m_profile.aclFileSet = parseAcl(m_aclFileSetEdit);
    m_profile.aclCatalog = parseAcl(m_aclCatalogEdit);
    m_profile.aclCommand = parseAcl(m_aclCommandEdit);
    m_profile.aclWhere = parseAcl(m_aclWhereEdit);
    m_profile.aclPluginOptions = parseAcl(m_aclPluginOptionsEdit);
}

void BProfileSettingsDialog::onAccept()
{
    saveProfile();
    accept();
}

void BProfileSettingsDialog::onResetDefaults()
{
    int result = QMessageBox::question(this, tr("Reset to Defaults"),
        tr("Are you sure you want to reset all advanced settings to their default values?"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (result == QMessageBox::Yes) {
        // Reset to defaults
        m_descriptionEdit->clear();
        m_heartbeatSpin->setValue(0);
        m_profileEdit->clear();

        m_tlsRequireCheck->setChecked(true);
        m_tlsAuthenticateCheck->setChecked(false);
        m_tlsCipherListEdit->clear();
        m_tlsCipherSuitesEdit->clear();
        m_tlsProtocolEdit->clear();
        m_tlsDhFileEdit->clear();
        m_tlsCrlFileEdit->clear();
        m_tlsAllowedCnEdit->clear();

        m_aclJobEdit->clear();
        m_aclClientEdit->clear();
        m_aclStorageEdit->clear();
        m_aclScheduleEdit->clear();
        m_aclPoolEdit->clear();
        m_aclFileSetEdit->clear();
        m_aclCatalogEdit->clear();
        m_aclCommandEdit->clear();
        m_aclWhereEdit->clear();
        m_aclPluginOptionsEdit->clear();
    }
}
