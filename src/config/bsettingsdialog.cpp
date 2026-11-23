// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "config/bsettingsdialog.h"
#include "buiscale.h"
#include "blabelstate.h"
#include "ui_bsettingsdialog.h"
#include "config/bsettings.h"
#include "btranslations.h"
#include "bconnectionprofile.h"
#include "bpfxconverter.h"
#include "blogging.h"
#include "director/bareosdirector.h"

#include <QFormLayout>
#include <QSet>
#include <algorithm>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QColorDialog>
#include <QStandardItemModel>

BSettingsDialog::BSettingsDialog(BDirector *director,
                               const QList<QPair<QString, QString>> &availableLevels,
                               QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BSettingsDialog)
    , m_director(director)
    , m_categoryList(nullptr)
    , m_contentStack(nullptr)
    , m_availableLevels(availableLevels)
    , m_levelsLayout(nullptr)
{
    ui->setupUi(this);

    // Use default levels if none provided from Director
    if (m_availableLevels.isEmpty()) {
        m_availableLevels = {
            {"Full", "Full"},
            {"Incremental", "Incremental"},
            {"Differential", "Differential"},
            {"VirtualFull", "VirtualFull"}
        };
    } else {
        // Remove duplicates and sort
        QSet<QString> seen;
        QList<QPair<QString, QString>> uniqueLevels;
        for (const auto &level : m_availableLevels) {
            if (!seen.contains(level.first)) {
                seen.insert(level.first);
                uniqueLevels.append(level);
            }
        }
        // Sort alphabetically by name
        std::sort(uniqueLevels.begin(), uniqueLevels.end(),
                  [](const QPair<QString, QString> &a, const QPair<QString, QString> &b) {
                      return a.first < b.first;
                  });
        m_availableLevels = uniqueLevels;
    }

    setWindowTitle(tr("Settings"));
    resize(BUiScale::px(900), BUiScale::px(600));
    setModal(true);

    setupUI();
    loadSettings();
}

BSettingsDialog::~BSettingsDialog()
{
    delete ui;
}

void BSettingsDialog::setupUI()
{
    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create sidebar
    createSidebar();

    // Content area
    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 30, 30, 20);
    contentLayout->setSpacing(20);

    // Content Stack
    m_contentStack = new QStackedWidget(this);
    createContentPages();
    contentLayout->addWidget(m_contentStack, 1);

    // Button bar
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    m_resetButton = new QPushButton(tr("Reset..."), this);
    m_resetButton->setToolTip(tr("Reset the settings to their defaults at once. "
                                 "The connection profiles are kept unless you choose otherwise."));
    m_resetButton->setObjectName("resetButton");
    m_cancelButton = new QPushButton(tr("Cancel"), this);
    m_cancelButton->setObjectName("cancelButton");
    // Saves everything, the profiles included; nothing is written before
    m_applyButton = new QPushButton(tr("Save"), this);
    m_applyButton->setObjectName("applyButton");
    m_applyButton->setDefault(true);

    // Cancel on the left, as in every dialog
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_applyButton);

    contentLayout->addLayout(buttonLayout);

    mainLayout->addWidget(m_categoryList);
    mainLayout->addWidget(contentWidget, 1);

    // Connect signals
    connect(m_categoryList, &QListWidget::currentRowChanged, this, &BSettingsDialog::onCategoryChanged);
    connect(m_applyButton, &QPushButton::clicked, this, &BSettingsDialog::onApplyClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &BSettingsDialog::onCancelClicked);
    connect(m_resetButton, &QPushButton::clicked, this, &BSettingsDialog::onResetToDefaultsClicked);

    // Select first category
    m_categoryList->setCurrentRow(0);
}

void BSettingsDialog::createSidebar()
{
    m_categoryList = new QListWidget(this);
    m_categoryList->setObjectName("categoryList");
    BUiScale::track(m_categoryList, [this]() { m_categoryList->setFixedWidth(BUiScale::px(220)); });
    m_categoryList->setSpacing(2);
    m_categoryList->setFrameShape(QFrame::NoFrame);

    // Add categories
    QListWidgetItem *connectionItem = new QListWidgetItem(tr("🔌 Connection"));
    connectionItem->setData(Qt::UserRole, "connection");
    m_categoryList->addItem(connectionItem);

    QListWidgetItem *appearanceItem = new QListWidgetItem(tr("🎨 Appearance"));
    appearanceItem->setData(Qt::UserRole, "appearance");
    m_categoryList->addItem(appearanceItem);

    QListWidgetItem *behaviorItem = new QListWidgetItem(tr("⚙️ Behavior"));
    behaviorItem->setData(Qt::UserRole, "behavior");
    m_categoryList->addItem(behaviorItem);

    QListWidgetItem *advancedItem = new QListWidgetItem(tr("🔧 Advanced"));
    advancedItem->setData(Qt::UserRole, "advanced");
    m_categoryList->addItem(advancedItem);
}

void BSettingsDialog::createContentPages()
{
    createConnectionPage();
    createAppearancePage();
    createBehaviorPage();
    createAdvancedPage();
}

void BSettingsDialog::createConnectionPage()
{
    m_connectionPage = new QWidget();

    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea(m_connectionPage);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Content widget inside scroll area
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);
    layout->setSpacing(20);

    // Title
    QLabel *titleLabel = new QLabel(tr("Connection Manager"));
    titleLabel->setObjectName("pageTitle");
    BUiScale::bind(titleLabel, BUiScale::Title, BUiScale::Bold);
    layout->addWidget(titleLabel);

    // ========================================================================
    // Connection Profiles List
    // ========================================================================
    QGroupBox *profilesGroup = new QGroupBox(tr("Saved Connections"));
    profilesGroup->setObjectName("settingsGroup");
    QVBoxLayout *profilesLayout = new QVBoxLayout(profilesGroup);
    profilesLayout->setSpacing(12);

    // Profile list
    m_profileList = new QListWidget();
    m_profileList->setObjectName("profileList");
    BUiScale::track(m_profileList, [this]() { m_profileList->setMinimumHeight(BUiScale::px(120)); });
    BUiScale::track(m_profileList, [this]() { m_profileList->setMaximumHeight(BUiScale::px(180)); });
    m_profileList->setSelectionMode(QAbstractItemView::SingleSelection);
    profilesLayout->addWidget(m_profileList);

    // Profile buttons
    QHBoxLayout *profileButtonLayout = new QHBoxLayout();
    m_addProfileButton = new QPushButton(tr("+ Add"));
    m_addProfileButton->setObjectName("addProfileButton");
    m_addProfileButton->setToolTip(tr("Add a new connection profile"));
    m_duplicateProfileButton = new QPushButton(tr("Duplicate"));
    m_duplicateProfileButton->setEnabled(false);
    m_deleteProfileButton = new QPushButton(tr("Delete"));
    m_deleteProfileButton->setObjectName("dangerButton");
    m_deleteProfileButton->setEnabled(false);

    profileButtonLayout->addWidget(m_addProfileButton);
    profileButtonLayout->addWidget(m_duplicateProfileButton);
    profileButtonLayout->addStretch();
    profileButtonLayout->addWidget(m_deleteProfileButton);
    profilesLayout->addLayout(profileButtonLayout);

    layout->addWidget(profilesGroup);

    // Connect profile list signals
    connect(m_profileList, &QListWidget::currentRowChanged, this, &BSettingsDialog::onProfileSelectionChanged);
    connect(m_profileList, &QListWidget::itemDoubleClicked, this, &BSettingsDialog::onEditProfile);
    connect(m_addProfileButton, &QPushButton::clicked, this, &BSettingsDialog::onAddProfile);
    connect(m_duplicateProfileButton, &QPushButton::clicked, this, &BSettingsDialog::onDuplicateProfile);
    connect(m_deleteProfileButton, &QPushButton::clicked, this, &BSettingsDialog::onDeleteProfile);

    // ========================================================================
    // Profile Details (shown when a profile is selected)
    // ========================================================================
    m_profileDetailsWidget = new QWidget();
    QVBoxLayout *detailsLayout = new QVBoxLayout(m_profileDetailsWidget);
    detailsLayout->setContentsMargins(0, 0, 0, 0);
    detailsLayout->setSpacing(20);

    // Profile Name
    QGroupBox *nameGroup = new QGroupBox(tr("Profile"));
    nameGroup->setObjectName("settingsGroup");
    QFormLayout *nameLayout = new QFormLayout(nameGroup);
    nameLayout->setSpacing(12);

    m_profileNameEdit = new QLineEdit();
    m_profileNameEdit->setObjectName("profileNameEdit");
    m_profileNameEdit->setPlaceholderText(tr("e.g. Production Server, Test Environment"));
    nameLayout->addRow(tr("Profile Name:"), m_profileNameEdit);

    detailsLayout->addWidget(nameGroup);

    // Director Connection Settings
    QGroupBox *bconsoleGroup = new QGroupBox(tr("Director Connection"));
    bconsoleGroup->setObjectName("settingsGroup");
    QFormLayout *bconsoleLayout = new QFormLayout(bconsoleGroup);
    bconsoleLayout->setSpacing(12);

    m_hostEdit = new QLineEdit();
    m_hostEdit->setObjectName("hostEdit");
    m_hostEdit->setPlaceholderText(tr("e.g. 192.168.1.100 or bareos-dir.local"));
    bconsoleLayout->addRow(tr("Host:"), m_hostEdit);

    m_portSpin = new QSpinBox();
    m_portSpin->setRange(1, 65535);
    m_portSpin->setValue(9101);
    bconsoleLayout->addRow(tr("Port:"), m_portSpin);

    // Protocol (gRPC only with the Onesimus Director)
    m_transportCombo = new QComboBox();
    m_transportCombo->addItem(tr("BNET (Bareos console protocol)"), "bnet");
    m_transportCombo->addItem(tr("gRPC (Onesimus Director)"), "grpc");
    if (!BareosDirector::grpcAvailable()) {
        // Build without gRPC: keep the entry visible but not selectable
        if (auto *model = qobject_cast<QStandardItemModel *>(m_transportCombo->model())) {
            model->item(1)->setEnabled(false);
        }
        m_transportCombo->setToolTip(tr("This build of Onesimus has no gRPC support."));
    }
    bconsoleLayout->addRow(tr("Protocol:"), m_transportCombo);

    m_grpcPortSpin = new QSpinBox();
    m_grpcPortSpin->setRange(1, 65535);
    m_grpcPortSpin->setValue(10101);
    m_grpcPortSpin->setToolTip(tr("gRPC port of the Director (default: BNET port + 1000)"));
    bconsoleLayout->addRow(tr("gRPC port:"), m_grpcPortSpin);

    // Pinned server certificate (trusted on first use)
    QHBoxLayout *grpcCertLayout = new QHBoxLayout();
    m_grpcFingerprintLabel = new QLabel();
    m_grpcFingerprintLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    grpcCertLayout->addWidget(m_grpcFingerprintLabel, 1);
    m_forgetGrpcCertButton = new QPushButton(tr("Forget certificate"));
    m_forgetGrpcCertButton->setToolTip(tr("Remove the pinned certificate. The next connection "
                                          "asks again whether the server certificate is trusted."));
    grpcCertLayout->addWidget(m_forgetGrpcCertButton);
    bconsoleLayout->addRow(tr("Server certificate:"), grpcCertLayout);

    connect(m_transportCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BSettingsDialog::onTransportChanged);
    connect(m_forgetGrpcCertButton, &QPushButton::clicked,
            this, &BSettingsDialog::onForgetGrpcCertificate);

    m_serverPlatformCombo = new QComboBox();
    m_serverPlatformCombo->addItem(tr("Linux/Unix"), "linux");
    m_serverPlatformCombo->addItem(tr("Windows"), "windows");
    m_serverPlatformCombo->addItem(tr("FreeBSD"), "freebsd");
    m_serverPlatformCombo->addItem(tr("macOS"), "darwin");
    m_serverPlatformCombo->setToolTip(tr("Server operating system (affects TLS paths in exported configs)"));
    bconsoleLayout->addRow(tr("Server OS:"), m_serverPlatformCombo);

    m_directorEdit = new QLineEdit();
    m_directorEdit->setPlaceholderText("bareos-dir");
    bconsoleLayout->addRow(tr("Director Name:"), m_directorEdit);

    m_consoleEdit = new QLineEdit();
    m_consoleEdit->setPlaceholderText("onesimus");
    m_consoleEdit->setToolTip(tr("Console name for authentication (e.g., 'admin' or 'onesimus')"));
    bconsoleLayout->addRow(tr("Console Name:"), m_consoleEdit);

    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("••••••••");
    bconsoleLayout->addRow(tr("Password:"), m_passwordEdit);

    // MD5 Hash preview (read-only, updates in real-time)
    m_md5Label = new QLabel();
    m_md5Label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    BUiScale::bind(m_md5Label, BUiScale::Small, BUiScale::Monospace);
    bSetLabelState(m_md5Label, "muted");
    m_md5Label->setText(tr("(password MD5 hash will appear here)"));
    bconsoleLayout->addRow(tr("MD5 Hash:"), m_md5Label);

    // Update MD5 hash in real-time as user types
    connect(m_passwordEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (text.isEmpty()) {
            m_md5Label->setText(tr("(password MD5 hash will appear here)"));
            bSetLabelState(m_md5Label, "muted");
        } else if (text.startsWith("[md5]")) {
            // Already an MD5 hash with prefix - extract and display
            QString hash = text.mid(5);
            m_md5Label->setText(hash);
            bSetLabelState(m_md5Label, "success");
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
                bSetLabelState(m_md5Label, "success");
            } else {
                QByteArray md5 = QCryptographicHash::hash(text.toLatin1(), QCryptographicHash::Md5);
                m_md5Label->setText(QString::fromLatin1(md5.toHex()));
                bSetLabelState(m_md5Label, "");  // theme text colour
            }
        } else {
            QByteArray md5 = QCryptographicHash::hash(text.toLatin1(), QCryptographicHash::Md5);
            m_md5Label->setText(QString::fromLatin1(md5.toHex()));
            bSetLabelState(m_md5Label, "");  // theme text colour
        }
    });

    detailsLayout->addWidget(bconsoleGroup);

    // TLS Warning Label (shown when Legacy is selected)
    QLabel *tlsWarningLabel = new QLabel(
        tr("⚠️ TLS encrypts communication with the Director.\n"
        "TLS is strongly recommended for production environments!"));
    tlsWarningLabel->setObjectName("warningLabel");
    tlsWarningLabel->setWordWrap(true);
    tlsWarningLabel->setVisible(false);
    detailsLayout->addWidget(tlsWarningLabel);

    // Authentication & Encryption Settings
    m_tlsGroupBox = new QGroupBox(tr("Authentication & Encryption"));
    m_tlsGroupBox->setObjectName("settingsGroup");
    QVBoxLayout *tlsLayout = new QVBoxLayout(m_tlsGroupBox);
    tlsLayout->setSpacing(12);

    // Authentication Method
    QLabel *authMethodLabel = new QLabel(tr("Authentication Method:"));
    authMethodLabel->setStyleSheet("font-weight: bold;");
    tlsLayout->addWidget(authMethodLabel);

    // Default is PSK mode (checkbox unchecked)
    // Checkbox enables certificate mode when checked
    m_useCertificatesCheck = new QCheckBox(tr("Use x509 certificates (instead of PSK)"));
    m_useCertificatesCheck->setChecked(false);  // PSK is default
    m_useCertificatesCheck->setToolTip(tr("By default, TLS uses Pre-Shared Key (PSK) authentication.\n"
                                           "Enable this to use X.509 certificate-based authentication instead.\n"
                                           "Certificate mode requires CA certificate and client certificate/key files."));
    tlsLayout->addWidget(m_useCertificatesCheck);

    // Warning label is hidden by default (legacy mode is disabled)
    Q_UNUSED(tlsWarningLabel);
    tlsWarningLabel->setVisible(false);

    // Certificate Settings (only for Certificate mode)
    m_certWidget = new QWidget();
    QFormLayout *certLayout = new QFormLayout(m_certWidget);
    certLayout->setSpacing(12);

#ifndef Q_OS_WINDOWS
    // Linux: Separate PEM files
    QHBoxLayout *caLayout = new QHBoxLayout();
    m_caCertEdit = new QLineEdit();
    m_caCertEdit->setPlaceholderText(tr("Path to CA certificate (.pem)"));
    QPushButton *caBrowse = new QPushButton(tr("Browse..."));
    caBrowse->setObjectName("browseButton");
    connect(caBrowse, &QPushButton::clicked, this, &BSettingsDialog::onBrowseCACert);
    caLayout->addWidget(m_caCertEdit);
    caLayout->addWidget(caBrowse);
    certLayout->addRow(tr("CA Certificate:"), caLayout);

    QHBoxLayout *clientCertLayout = new QHBoxLayout();
    m_clientCertEdit = new QLineEdit();
    m_clientCertEdit->setPlaceholderText(tr("Path to client certificate (.pem)"));
    QPushButton *clientCertBrowse = new QPushButton(tr("Browse..."));
    clientCertBrowse->setObjectName("browseButton");
    connect(clientCertBrowse, &QPushButton::clicked, this, &BSettingsDialog::onBrowseClientCert);
    clientCertLayout->addWidget(m_clientCertEdit);
    clientCertLayout->addWidget(clientCertBrowse);
    certLayout->addRow(tr("Client Certificate:"), clientCertLayout);

    QHBoxLayout *keyLayout = new QHBoxLayout();
    m_clientKeyEdit = new QLineEdit();
    m_clientKeyEdit->setPlaceholderText(tr("Path to private key (.pem, .key)"));
    QPushButton *keyBrowse = new QPushButton(tr("Browse..."));
    keyBrowse->setObjectName("browseButton");
    connect(keyBrowse, &QPushButton::clicked, this, &BSettingsDialog::onBrowseClientKey);
    keyLayout->addWidget(m_clientKeyEdit);
    keyLayout->addWidget(keyBrowse);
    certLayout->addRow(tr("Private Key:"), keyLayout);
#else
    // Windows: PFX file
    QHBoxLayout *clientCertLayout = new QHBoxLayout();
    m_clientCertEdit = new QLineEdit();
    m_clientCertEdit->setPlaceholderText(tr("Path to client certificate (.pfx)"));
    QPushButton *clientCertBrowse = new QPushButton(tr("Browse..."));
    clientCertBrowse->setObjectName("browseButton");
    connect(clientCertBrowse, &QPushButton::clicked, this, &BSettingsDialog::onBrowseClientCert);
    clientCertLayout->addWidget(m_clientCertEdit);
    clientCertLayout->addWidget(clientCertBrowse);
    certLayout->addRow(tr("PFX Certificate:"), clientCertLayout);

    // Convert to PFX button
    QPushButton *convertPfxButton = new QPushButton(tr("Convert PEM/DER to PFX..."));
    convertPfxButton->setToolTip(tr("Convert separate PEM/DER certificate and key files into a PFX file"));
    connect(convertPfxButton, &QPushButton::clicked, this, &BSettingsDialog::onConvertToPFX);
    certLayout->addRow("", convertPfxButton);
#endif

    m_verifyPeerCheck = new QCheckBox(tr("Verify server certificate (recommended)"));
    m_verifyPeerCheck->setChecked(true);
    certLayout->addRow("", m_verifyPeerCheck);

    m_certWidget->setEnabled(false);
    tlsLayout->addWidget(m_certWidget);
    connect(m_useCertificatesCheck, &QCheckBox::toggled, m_certWidget, &QWidget::setEnabled);

    // Cipher List (for advanced PSK configuration)
    QLabel *cipherLabel = new QLabel(tr("TLS Cipher List (optional):"));
    cipherLabel->setStyleSheet("margin-top: 8px;");
    tlsLayout->addWidget(cipherLabel);

    QHBoxLayout *cipherLayout = new QHBoxLayout();
    m_cipherListEdit = new QLineEdit();
    m_cipherListEdit->setPlaceholderText(tr("e.g., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256"));
    m_cipherListEdit->setToolTip(tr("Colon-separated list of TLS-PSK ciphers.\n"
                                     "Leave empty for auto-detection.\n"
                                     "Example: PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256"));
    cipherLayout->addWidget(m_cipherListEdit);

    QPushButton *presetCipherBtn = new QPushButton(tr("Preset"));
    presetCipherBtn->setToolTip(tr("Fill with recommended PSK ciphers"));
    connect(presetCipherBtn, &QPushButton::clicked, this, [this]() {
        m_cipherListEdit->setText("PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256:PSK-AES256-CBC-SHA:PSK-AES128-CBC-SHA");
    });
    cipherLayout->addWidget(presetCipherBtn);
    tlsLayout->addLayout(cipherLayout);

    detailsLayout->addWidget(m_tlsGroupBox);

    // gRPC: shown instead of the BNET TLS options
    m_grpcInfoLabel = new QLabel(
        tr("gRPC is always encrypted with TLS. The server certificate is confirmed on "
           "first use and pinned; the Director authenticates with console name and "
           "password. TLS-PSK and certificate settings do not apply."));
    m_grpcInfoLabel->setObjectName("infoLabel");
    m_grpcInfoLabel->setWordWrap(true);
    m_grpcInfoLabel->setVisible(false);
    detailsLayout->addWidget(m_grpcInfoLabel);
    updateGrpcFingerprintDisplay();
    onTransportChanged();

    // Connection Options
    QGroupBox *optionsGroup = new QGroupBox(tr("Options"));
    optionsGroup->setObjectName("settingsGroup");
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);
    optionsLayout->setSpacing(12);

    m_savePasswordCheck = new QCheckBox(tr("Save password in profile"));
    m_savePasswordCheck->setChecked(true);
    optionsLayout->addWidget(m_savePasswordCheck);

    m_autoConnectCheck = new QCheckBox(tr("Auto-connect on startup"));
    optionsLayout->addWidget(m_autoConnectCheck);


    detailsLayout->addWidget(optionsGroup);

    detailsLayout->addStretch();

    // Initially hide details until a profile is selected
    m_profileDetailsWidget->setVisible(false);
    layout->addWidget(m_profileDetailsWidget);

    // ========================================================================
    // Import/Export Buttons
    // ========================================================================
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *exportBtn = new QPushButton(tr("Export All Profiles"));
    QPushButton *importBtn = new QPushButton(tr("Import Profiles"));

    connect(exportBtn, &QPushButton::clicked, this, &BSettingsDialog::onExportSettings);
    connect(importBtn, &QPushButton::clicked, this, &BSettingsDialog::onImportSettings);

    buttonLayout->addWidget(exportBtn);
    buttonLayout->addWidget(importBtn);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    layout->addStretch();

    // Set content widget to scroll area
    scrollArea->setWidget(contentWidget);

    // Add scroll area to page
    QVBoxLayout *pageLayout = new QVBoxLayout(m_connectionPage);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(scrollArea);

    m_contentStack->addWidget(m_connectionPage);
}

void BSettingsDialog::createAppearancePage()
{
    m_appearancePage = new QWidget();

    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea(m_appearancePage);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Content widget inside scroll area
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);
    layout->setSpacing(20);

    // Title
    QLabel *titleLabel = new QLabel(tr("Appearance"));
    titleLabel->setObjectName("pageTitle");
    BUiScale::bind(titleLabel, BUiScale::Title, BUiScale::Bold);
    layout->addWidget(titleLabel);

    // Interface zoom: the text size is the desktop's font times this factor
    QGroupBox *fontGroup = new QGroupBox(tr("Text Size"));
    fontGroup->setObjectName("settingsGroup");
    QFormLayout *fontLayout = new QFormLayout(fontGroup);

    m_uiZoomSpin = new QSpinBox();
    m_uiZoomSpin->setRange(BUiScale::MinZoom, BUiScale::MaxZoom);
    m_uiZoomSpin->setSingleStep(BUiScale::ZoomStep);
    m_uiZoomSpin->setValue(BUiScale::DefaultZoom);
    m_uiZoomSpin->setSuffix(" %");
    m_uiZoomSpin->setToolTip(tr("The text follows the font of your desktop. "
                                "This factor enlarges or reduces it (Ctrl++, Ctrl+-, Ctrl+0)."));
    fontLayout->addRow(tr("Interface zoom:"), m_uiZoomSpin);

    layout->addWidget(fontGroup);

    // Colour theme
    QGroupBox *themeGroup = new QGroupBox(tr("Theme"));
    themeGroup->setObjectName("settingsGroup");
    QFormLayout *themeLayout = new QFormLayout(themeGroup);
    m_themeCombo = new QComboBox();
    m_themeCombo->setObjectName("themeCombo");
    m_themeCombo->addItem(tr("System"), QStringLiteral("system"));
    m_themeCombo->addItem(tr("Light"), QStringLiteral("light"));
    m_themeCombo->addItem(tr("Dark"), QStringLiteral("dark"));
    m_themeCombo->setToolTip(tr("\"System\" follows the light or dark colour scheme of the desktop."));
    themeLayout->addRow(tr("Colour theme:"), m_themeCombo);
    layout->addWidget(themeGroup);

    // Language Selection
    QGroupBox *languageGroup = new QGroupBox(tr("Language"));
    languageGroup->setObjectName("settingsGroup");
    QFormLayout *languageLayout = new QFormLayout(languageGroup);

    m_languageCombo = new QComboBox();

    // The language of the system first: the default
    m_languageCombo->addItem(
        tr("System (%1)").arg(BTranslations::languageName(BTranslations::detectSystemLanguage())),
        BTranslations::systemCode());

    // Populate with available languages using flags
    QList<BTranslations::Language> languages = BTranslations::availableLanguages();
    for (BTranslations::Language lang : languages) {
        QString flag = BTranslations::languageFlag(lang);
        QString displayName = BTranslations::languageName(lang);
        QString code = BTranslations::languageCode(lang);
        m_languageCombo->addItem(flag + " " + displayName, code);
    }

    languageLayout->addRow(tr("Application Language:"), m_languageCombo);

    QLabel *infoLabel = new QLabel(
        tr("Language changes require an application restart. \"System\" follows the language of the operating system."));
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: #888; font-style: italic;");
    BUiScale::bind(infoLabel, BUiScale::Small);
    languageLayout->addRow("", infoLabel);

    layout->addWidget(languageGroup);

    // Status Bar Colors
    QGroupBox *statusBarColorsGroup = new QGroupBox(tr("Status Bar Colors"));
    statusBarColorsGroup->setObjectName("settingsGroup");
    QFormLayout *statusBarColorsLayout = new QFormLayout(statusBarColorsGroup);
    statusBarColorsLayout->setSpacing(12);

    // Connected Color
    m_colorButtonConnected = new QPushButton();
    BUiScale::track(m_colorButtonConnected, [this]() { m_colorButtonConnected->setMinimumSize(BUiScale::px(80), BUiScale::px(30)); });
    m_colorButtonConnected->setCursor(Qt::PointingHandCursor);
    m_colorButtonConnected->setToolTip(tr("Color shown when connected to Director"));
    connect(m_colorButtonConnected, &QPushButton::clicked, this, &BSettingsDialog::onChooseColorConnected);
    statusBarColorsLayout->addRow(tr("Connected:"), m_colorButtonConnected);

    // Disconnected Color
    m_colorButtonDisconnected = new QPushButton();
    BUiScale::track(m_colorButtonDisconnected, [this]() { m_colorButtonDisconnected->setMinimumSize(BUiScale::px(80), BUiScale::px(30)); });
    m_colorButtonDisconnected->setCursor(Qt::PointingHandCursor);
    m_colorButtonDisconnected->setToolTip(tr("Color shown when not connected"));
    connect(m_colorButtonDisconnected, &QPushButton::clicked, this, &BSettingsDialog::onChooseColorDisconnected);
    statusBarColorsLayout->addRow(tr("Disconnected:"), m_colorButtonDisconnected);

    layout->addWidget(statusBarColorsGroup);

    // Backup Level Colors
    QGroupBox *levelColorsGroup = new QGroupBox(tr("Backup Level Colors"));
    levelColorsGroup->setObjectName("settingsGroup");
    QFormLayout *levelColorsLayout = new QFormLayout(levelColorsGroup);
    levelColorsLayout->setSpacing(12);

    // Full Backup Color
    m_colorButtonFull = new QPushButton();
    BUiScale::track(m_colorButtonFull, [this]() { m_colorButtonFull->setMinimumSize(BUiScale::px(80), BUiScale::px(30)); });
    m_colorButtonFull->setCursor(Qt::PointingHandCursor);
    connect(m_colorButtonFull, &QPushButton::clicked, this, &BSettingsDialog::onChooseColorFull);
    levelColorsLayout->addRow("Full (F):", m_colorButtonFull);

    // Incremental Backup Color
    m_colorButtonIncremental = new QPushButton();
    BUiScale::track(m_colorButtonIncremental, [this]() { m_colorButtonIncremental->setMinimumSize(BUiScale::px(80), BUiScale::px(30)); });
    m_colorButtonIncremental->setCursor(Qt::PointingHandCursor);
    connect(m_colorButtonIncremental, &QPushButton::clicked, this, &BSettingsDialog::onChooseColorIncremental);
    levelColorsLayout->addRow("Incremental (I):", m_colorButtonIncremental);

    // Differential Backup Color
    m_colorButtonDifferential = new QPushButton();
    BUiScale::track(m_colorButtonDifferential, [this]() { m_colorButtonDifferential->setMinimumSize(BUiScale::px(80), BUiScale::px(30)); });
    m_colorButtonDifferential->setCursor(Qt::PointingHandCursor);
    connect(m_colorButtonDifferential, &QPushButton::clicked, this, &BSettingsDialog::onChooseColorDifferential);
    levelColorsLayout->addRow("Differential (D):", m_colorButtonDifferential);

    // Virtual Full Backup Color
    m_colorButtonVirtualFull = new QPushButton();
    BUiScale::track(m_colorButtonVirtualFull, [this]() { m_colorButtonVirtualFull->setMinimumSize(BUiScale::px(80), BUiScale::px(30)); });
    m_colorButtonVirtualFull->setCursor(Qt::PointingHandCursor);
    connect(m_colorButtonVirtualFull, &QPushButton::clicked, this, &BSettingsDialog::onChooseColorVirtualFull);
    levelColorsLayout->addRow("Virtual Full (V):", m_colorButtonVirtualFull);

    layout->addWidget(levelColorsGroup);

    layout->addStretch();

    // Set content widget to scroll area
    scrollArea->setWidget(contentWidget);

    // Add scroll area to page
    QVBoxLayout *pageLayout = new QVBoxLayout(m_appearancePage);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(scrollArea);

    m_contentStack->addWidget(m_appearancePage);
}

void BSettingsDialog::createBehaviorPage()
{
    m_behaviorPage = new QWidget();

    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea(m_behaviorPage);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Content widget inside scroll area
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);
    layout->setSpacing(20);

    // Title
    QLabel *titleLabel = new QLabel(tr("Behavior"));
    titleLabel->setObjectName("pageTitle");
    BUiScale::bind(titleLabel, BUiScale::Title, BUiScale::Bold);
    layout->addWidget(titleLabel);

    // Confirmations
    QGroupBox *confirmGroup = new QGroupBox(tr("Confirmations"));
    confirmGroup->setObjectName("settingsGroup");
    QVBoxLayout *confirmLayout = new QVBoxLayout(confirmGroup);

    m_confirmJobCancelCheck = new QCheckBox(tr("Confirm job cancellation"));
    m_confirmJobCancelCheck->setObjectName("confirmJobCancelCheck");
    m_confirmJobCancelCheck->setChecked(true);
    confirmLayout->addWidget(m_confirmJobCancelCheck);

    m_confirmJobStartCheck = new QCheckBox(tr("Confirm job start"));
    m_confirmJobStartCheck->setChecked(true);
    confirmLayout->addWidget(m_confirmJobStartCheck);

    layout->addWidget(confirmGroup);

    // Auto-Refresh
    QGroupBox *refreshGroup = new QGroupBox(tr("Auto-Refresh"));
    refreshGroup->setObjectName("settingsGroup");
    QVBoxLayout *refreshLayout = new QVBoxLayout(refreshGroup);

    m_autoRefreshCheck = new QCheckBox(tr("Auto-refresh"));
    refreshLayout->addWidget(m_autoRefreshCheck);

    QHBoxLayout *intervalLayout = new QHBoxLayout();
    QLabel *intervalLabel = new QLabel(tr("Interval:"));
    m_refreshIntervalSpin = new QSpinBox();
    m_refreshIntervalSpin->setRange(10, 300);
    m_refreshIntervalSpin->setValue(30);
    m_refreshIntervalSpin->setSuffix(tr(" seconds"));
    intervalLayout->addWidget(intervalLabel);
    intervalLayout->addWidget(m_refreshIntervalSpin);
    intervalLayout->addStretch();
    refreshLayout->addLayout(intervalLayout);

    layout->addWidget(refreshGroup);

    // Display
    QGroupBox *displayGroup = new QGroupBox(tr("Display"));
    displayGroup->setObjectName("settingsGroup");
    QFormLayout *displayLayout = new QFormLayout(displayGroup);

    m_maxJobsDisplaySpin = new QSpinBox();
    m_maxJobsDisplaySpin->setRange(10, 1000);
    m_maxJobsDisplaySpin->setValue(100);
    m_maxJobsDisplaySpin->setSuffix(" Jobs");
    displayLayout->addRow(tr("Maximum Jobs:"), m_maxJobsDisplaySpin);

    m_jobsNewestFirstCheck = new QCheckBox(tr("Show newest jobs first"));
    m_jobsNewestFirstCheck->setChecked(true);
    m_jobsNewestFirstCheck->setToolTip(
        tr("When enabled, the job list shows the most recent jobs first.\n"
           "When disabled, the oldest jobs are shown first."));
    displayLayout->addRow("", m_jobsNewestFirstCheck);

    m_clientStaleHoursSpin = new QSpinBox();
    m_clientStaleHoursSpin->setRange(1, 8760);
    m_clientStaleHoursSpin->setValue(24);
    m_clientStaleHoursSpin->setSuffix(tr(" hours"));
    m_clientStaleHoursSpin->setToolTip(
        tr("The clients table marks a client as stale when its last backup or contact\n"
           "is at least this old. The value is taken from the job history; it is not\n"
           "a live connection check."));
    displayLayout->addRow(tr("Client backup is stale after:"), m_clientStaleHoursSpin);

    layout->addWidget(displayGroup);

    // Schedules
    QGroupBox *schedulesGroup = new QGroupBox(tr("Schedules"));
    schedulesGroup->setObjectName("settingsGroup");
    QFormLayout *schedulesLayout = new QFormLayout(schedulesGroup);

    m_scheduleHistoryDaysSpin = new QSpinBox();
    m_scheduleHistoryDaysSpin->setRange(1, 365);
    m_scheduleHistoryDaysSpin->setValue(60);
    m_scheduleHistoryDaysSpin->setSuffix(tr(" days"));
    m_scheduleHistoryDaysSpin->setToolTip(
        tr("The schedule view measures how long jobs run and how long they wait\n"
           "from the jobs of the last days in the catalog. A longer time gives more\n"
           "runs per job and a larger request to the Director. The history is only\n"
           "requested while the schedule view is open."));
    schedulesLayout->addRow(tr("Measure job history over the last:"), m_scheduleHistoryDaysSpin);

    layout->addWidget(schedulesGroup);

    // Visible Backup Levels (dynamic from Director)
    QGroupBox *levelsGroup = new QGroupBox(tr("Visible Backup Levels"));
    levelsGroup->setObjectName("settingsGroup");
    m_levelsLayout = new QVBoxLayout(levelsGroup);

    QLabel *levelsInfoLabel = new QLabel(
        tr("Select which backup levels appear in the Run Backup dialog:"));
    levelsInfoLabel->setWordWrap(true);
    m_levelsLayout->addWidget(levelsInfoLabel);

    // Tooltips for common levels
    QMap<QString, QString> levelTooltips = {
        {"Full", tr("Complete backup of all files")},
        {"Incremental", tr("Backup files changed since last backup")},
        {"Differential", tr("Backup files changed since last Full backup")},
        {"VirtualFull", tr("Consolidate incremental backups into a synthetic full")},
        {"Data", tr("Data-level backup (rarely used)")}
    };

    // Create checkboxes dynamically from available levels
    for (const auto &level : m_availableLevels) {
        QString levelName = level.first;
        QCheckBox *checkbox = new QCheckBox(levelName);

        // Set tooltip if available
        if (levelTooltips.contains(levelName)) {
            checkbox->setToolTip(levelTooltips[levelName]);
        }

        m_levelCheckboxes[levelName] = checkbox;
        m_levelsLayout->addWidget(checkbox);
    }

    layout->addWidget(levelsGroup);

    layout->addStretch();

    // Set content widget to scroll area
    scrollArea->setWidget(contentWidget);

    // Add scroll area to page
    QVBoxLayout *pageLayout = new QVBoxLayout(m_behaviorPage);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(scrollArea);

    m_contentStack->addWidget(m_behaviorPage);
}

void BSettingsDialog::createAdvancedPage()
{
    m_advancedPage = new QWidget();

    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea(m_advancedPage);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Content widget inside scroll area
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);
    layout->setSpacing(20);

    // Title
    QLabel *titleLabel = new QLabel(tr("Advanced Settings"));
    titleLabel->setObjectName("pageTitle");
    BUiScale::bind(titleLabel, BUiScale::Title, BUiScale::Bold);
    layout->addWidget(titleLabel);

    // Logging (only visible when file logging is compiled in)
#ifdef ONESIMUS_FILE_LOGGING
    QGroupBox *logGroup = new QGroupBox(tr("File Logging"));
    logGroup->setObjectName("settingsGroup");
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);

    m_debugLoggingCheck = new QCheckBox(tr("Enable file logging"));
    m_debugLoggingCheck->setToolTip(tr("Write debug information to a log file"));
    logLayout->addWidget(m_debugLoggingCheck);

    QHBoxLayout *logFileLayout = new QHBoxLayout();
    QLabel *logFileLabel = new QLabel(tr("Log File:"));
    m_logFileEdit = new QLineEdit();
    m_logFileEdit->setPlaceholderText(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/onesimus.log");
    m_logFileEdit->setReadOnly(true);  // Path is fixed, just for display
    logFileLayout->addWidget(logFileLabel);
    logFileLayout->addWidget(m_logFileEdit, 1);
    logLayout->addLayout(logFileLayout);

    QHBoxLayout *maxSizeLayout = new QHBoxLayout();
    QLabel *maxSizeLabel = new QLabel(tr("Max. Log Size:"));
    m_maxLogSizeSpin = new QSpinBox();
    m_maxLogSizeSpin->setRange(1, 100);
    m_maxLogSizeSpin->setValue(10);
    m_maxLogSizeSpin->setSuffix(" MB");
    maxSizeLayout->addWidget(maxSizeLabel);
    maxSizeLayout->addWidget(m_maxLogSizeSpin);
    maxSizeLayout->addStretch();
    logLayout->addLayout(maxSizeLayout);

    // Connect checkbox to BFileLogger
    connect(m_debugLoggingCheck, &QCheckBox::toggled, this, [](bool enabled) {
        BLOG_SET_ENABLED(enabled);
    });

    layout->addWidget(logGroup);
#else
    // Hide logging UI when file logging is not compiled in
    m_debugLoggingCheck = nullptr;
    m_logFileEdit = nullptr;
    m_maxLogSizeSpin = nullptr;
#endif

    // Miscellaneous Options
    QGroupBox *miscGroup = new QGroupBox(tr("Miscellaneous"));
    miscGroup->setObjectName("settingsGroup");
    QVBoxLayout *miscLayout = new QVBoxLayout(miscGroup);

    // Applied by the main window's tooltip filter at once after saving
    m_enableTooltipsCheck = new QCheckBox(tr("Show tooltips"));
    m_enableTooltipsCheck->setChecked(true);
    miscLayout->addWidget(m_enableTooltipsCheck);

    layout->addWidget(miscGroup);

    // Warning
    QLabel *warningLabel = new QLabel(
        tr("⚠️ These settings are for advanced users.\n"
        "Changes may affect application stability."));
    warningLabel->setObjectName("warningLabel");
    warningLabel->setWordWrap(true);
    layout->addWidget(warningLabel);

    layout->addStretch();

    // Set content widget to scroll area
    scrollArea->setWidget(contentWidget);

    // Add scroll area to page
    QVBoxLayout *pageLayout = new QVBoxLayout(m_advancedPage);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(scrollArea);

    m_contentStack->addWidget(m_advancedPage);
}

void BSettingsDialog::loadSettings()
{
    BSettings& settings = BSettings::instance();

    // ========================================================================
    // Connection Profiles
    // ========================================================================

    // Migrate old settings if needed
    settings.migrateOldConnectionSettings();

    // Working copy of the profiles: written by saveSettings() only
    m_currentProfileId.clear();
    m_profiles = settings.connectionProfiles();
    m_lastUsedProfileId = settings.lastUsedProfileId();
    {
        QSignalBlocker blocker(m_profileList);
        m_profileList->clear();
    }
    for (const BConnectionProfile &profile : std::as_const(m_profiles))
        addProfileToList(profile, profile.id == m_lastUsedProfileId);

    // If no profile was selected, select the first one
    if (m_profileList->currentRow() < 0 && m_profileList->count() > 0) {
        m_profileList->setCurrentRow(0);
    }

    // Global connection options (not per-profile)
    m_savePasswordCheck->setChecked(settings.connectionSavePassword());
    m_autoConnectCheck->setChecked(settings.connectionAutoConnect());

    // Appearance
    m_uiZoomSpin->setValue(settings.appearanceUiZoom());

    // Theme
    const int themeIndex = m_themeCombo->findData(settings.appearanceTheme());
    m_themeCombo->setCurrentIndex(themeIndex >= 0 ? themeIndex : m_themeCombo->findData("dark"));

    // Language
    QString currentLangCode = settings.appearanceLanguage();
    int langIndex = m_languageCombo->findData(currentLangCode);
    m_languageCombo->setCurrentIndex(langIndex >= 0 ? langIndex : 0);

    // Level and status bar colours: working copies, shown on the buttons
    m_levelColors.clear();
    for (const QString &level : {QStringLiteral("F"), QStringLiteral("I"),
                                 QStringLiteral("D"), QStringLiteral("V")})
        m_levelColors.insert(level, settings.levelColor(level));
    showColor(m_colorButtonFull, m_levelColors.value("F"));
    showColor(m_colorButtonIncremental, m_levelColors.value("I"));
    showColor(m_colorButtonDifferential, m_levelColors.value("D"));
    showColor(m_colorButtonVirtualFull, m_levelColors.value("V"));
    m_connectedColor = settings.statusBarConnectedColor();
    m_disconnectedColor = settings.statusBarDisconnectedColor();
    showColor(m_colorButtonConnected, m_connectedColor);
    showColor(m_colorButtonDisconnected, m_disconnectedColor);

    // Behavior
    m_confirmJobCancelCheck->setChecked(settings.behaviorConfirmJobCancel());
    m_confirmJobStartCheck->setChecked(settings.behaviorConfirmJobStart());
    m_autoRefreshCheck->setChecked(settings.behaviorAutoRefresh());
    m_refreshIntervalSpin->setValue(settings.behaviorRefreshInterval());
    m_maxJobsDisplaySpin->setValue(settings.behaviorMaxJobsDisplay());
    m_jobsNewestFirstCheck->setChecked(settings.behaviorJobsNewestFirst());
    m_clientStaleHoursSpin->setValue(settings.behaviorClientStaleHours());
    m_scheduleHistoryDaysSpin->setValue(settings.schedulesHistoryDays());

    // Visible Backup Levels (dynamic)
    QStringList visibleLevels = settings.visibleLevels();
    for (auto it = m_levelCheckboxes.constBegin(); it != m_levelCheckboxes.constEnd(); ++it) {
        it.value()->setChecked(visibleLevels.contains(it.key()));
    }

    // Advanced
#ifdef ONESIMUS_FILE_LOGGING
    if (m_debugLoggingCheck) {
        m_debugLoggingCheck->setChecked(settings.advancedDebugLogging());
        // Apply the setting to BFileLogger
        BLOG_SET_ENABLED(settings.advancedDebugLogging());
    }
    if (m_logFileEdit) {
        QString logPath = settings.advancedLogFile();
        if (logPath.isEmpty()) {
            logPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/onesimus.log";
        }
        m_logFileEdit->setText(logPath);
    }
    if (m_maxLogSizeSpin) {
        m_maxLogSizeSpin->setValue(settings.advancedMaxLogSize());
    }
#endif
    m_enableTooltipsCheck->setChecked(settings.advancedEnableTooltips());
}

void BSettingsDialog::saveSettings()
{
    BSettings& settings = BSettings::instance();

    // ========================================================================
    // Connection Profiles
    // ========================================================================

    // The form of the current profile into the working copy, then all of
    // them at once: additions, deletions and edits made in the dialog
    saveCurrentProfileSilent();
    settings.setConnectionProfiles(m_profiles);
    if (profileIndex(m_lastUsedProfileId) < 0)
        m_lastUsedProfileId.clear();
    if (settings.lastUsedProfileId() != m_lastUsedProfileId)
        settings.setLastUsedProfileId(m_lastUsedProfileId);

    // Save global connection options
    settings.setConnectionSavePassword(m_savePasswordCheck->isChecked());
    settings.setConnectionAutoConnect(m_autoConnectCheck->isChecked());

    // Appearance
    // Applied at once; the main window stores the value
    settings.setAppearanceUiZoom(m_uiZoomSpin->value());
    BUiScale::instance().setZoomPercent(m_uiZoomSpin->value());
    for (auto it = m_levelColors.constBegin(); it != m_levelColors.constEnd(); ++it) {
        if (it.value().isValid() && it.value() != settings.levelColor(it.key()))
            settings.setLevelColor(it.key(), it.value());
    }
    if (m_connectedColor.isValid() && m_connectedColor != settings.statusBarConnectedColor())
        settings.setStatusBarConnectedColor(m_connectedColor);
    if (m_disconnectedColor.isValid() && m_disconnectedColor != settings.statusBarDisconnectedColor())
        settings.setStatusBarDisconnectedColor(m_disconnectedColor);

    // Theme: the main window applies it on appearanceSettingsChanged()
    const QString theme = m_themeCombo->currentData().toString();
    if (theme != settings.appearanceTheme())
        settings.setAppearanceTheme(theme);

    // Language
    QString selectedLangCode = m_languageCombo->currentData().toString();
    settings.setAppearanceLanguage(selectedLangCode);

    // Takes effect at the next start: switching the translator and the
    // locale now would mix two languages in the open windows

    // Behavior
    settings.setBehaviorConfirmJobCancel(m_confirmJobCancelCheck->isChecked());
    settings.setBehaviorConfirmJobStart(m_confirmJobStartCheck->isChecked());
    settings.setBehaviorAutoRefresh(m_autoRefreshCheck->isChecked());
    settings.setBehaviorRefreshInterval(m_refreshIntervalSpin->value());
    settings.setBehaviorMaxJobsDisplay(m_maxJobsDisplaySpin->value());
    settings.setBehaviorJobsNewestFirst(m_jobsNewestFirstCheck->isChecked());
    settings.setBehaviorClientStaleHours(m_clientStaleHoursSpin->value());
    settings.setSchedulesHistoryDays(m_scheduleHistoryDaysSpin->value());

    // Visible Backup Levels (dynamic)
    QStringList visibleLevels;
    for (auto it = m_levelCheckboxes.constBegin(); it != m_levelCheckboxes.constEnd(); ++it) {
        if (it.value()->isChecked()) {
            visibleLevels << it.key();
        }
    }
    settings.setVisibleLevels(visibleLevels);

    // Advanced
#ifdef ONESIMUS_FILE_LOGGING
    if (m_debugLoggingCheck) {
        settings.setAdvancedDebugLogging(m_debugLoggingCheck->isChecked());
        BLOG_SET_ENABLED(m_debugLoggingCheck->isChecked());
    }
    if (m_logFileEdit) {
        settings.setAdvancedLogFile(m_logFileEdit->text());
    }
    if (m_maxLogSizeSpin) {
        settings.setAdvancedMaxLogSize(m_maxLogSizeSpin->value());
        BLOG_SET_MAX_SIZE_MB(m_maxLogSizeSpin->value());
    }
#endif
    settings.setAdvancedEnableTooltips(m_enableTooltipsCheck->isChecked());

    settings.sync();

    emit settingsChanged();
}

void BSettingsDialog::onCategoryChanged(int index)
{
    m_contentStack->setCurrentIndex(index);
}

void BSettingsDialog::onApplyClicked()
{
    saveSettings();
    QMessageBox::information(this, tr("Settings"),
        tr("Settings have been saved.\n"
        "Some changes require an application restart."));
    accept();
}

void BSettingsDialog::onCancelClicked()
{
    reject();
}

void BSettingsDialog::reject()
{
    // The file logging checkbox switches the logger at once: back to the
    // stored state. Everything else was only held by the dialog.
#ifdef ONESIMUS_FILE_LOGGING
    if (m_debugLoggingCheck)
        BLOG_SET_ENABLED(BSettings::instance().advancedDebugLogging());
#endif
    QDialog::reject();
}

void BSettingsDialog::onResetToDefaultsClicked()
{
    QMessageBox box(QMessageBox::Question, tr("Reset Settings"),
                    tr("Reset all settings to their default values now?\n\n"
                       "This takes effect at once, also if you then press Cancel, "
                       "and cannot be undone."),
                    QMessageBox::NoButton, this);
    QCheckBox *profilesCheck = new QCheckBox(tr("Also delete all connection profiles"), &box);
    box.setCheckBox(profilesCheck);
    QPushButton *resetButton = box.addButton(tr("Reset"), QMessageBox::DestructiveRole);
    box.addButton(QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Cancel);
    box.exec();
    if (box.clickedButton() != resetButton)
        return;

    const bool includeProfiles = profilesCheck->isChecked();
    resetToDefaults(includeProfiles);
    QMessageBox::information(this, tr("Reset Settings"),
        includeProfiles ? tr("All settings and connection profiles have been reset.")
                        : tr("All settings have been reset to their default values. "
                             "The connection profiles were kept."));
}

void BSettingsDialog::resetToDefaults(bool includeProfiles)
{
    BSettings::instance().resetToDefaults(includeProfiles);
    loadSettings();
}

void BSettingsDialog::onBrowseCACert()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select CA Certificate"),
        QString(), tr("Certificates (*.pem *.crt *.cert);;All Files (*)"));
    if (!file.isEmpty()) {
        m_caCertEdit->setText(file);
    }
}

void BSettingsDialog::onBrowseClientCert()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select Client Certificate"),
#ifdef Q_OS_WINDOWS
    QString(), tr("Certificates (*.pfx *.pem *.crt *.cert *.der);;PFX Files (*.pfx);;PEM Files (*.pem);;DER Files (*.der);;All Files (*)"));
#else
    QString(), tr("Certificates (*.pem *.crt *.cert);;All Files (*)"));
#endif
    if (!file.isEmpty()) {
        m_clientCertEdit->setText(file);
    }
}

void BSettingsDialog::onBrowseClientKey()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select Private Key"),
        QString(), tr("Keys (*.pem *.key);;All Files (*)"));
    if (!file.isEmpty()) {
        m_clientKeyEdit->setText(file);
    }
}

void BSettingsDialog::onExportSettings()
{
    saveCurrentProfileSilent();
    if (m_profiles.isEmpty()) {
        QMessageBox::information(this, tr("Export Profiles"),
            tr("There are no connection profiles to export."));
        return;
    }

    const QString file = QFileDialog::getSaveFileName(this, tr("Export Profiles"),
        QStringLiteral("onesimus-profiles.json"), tr("JSON (*.json)"));
    if (file.isEmpty())
        return;

    const auto answer = QMessageBox::question(this, tr("Export Profiles"),
        tr("Include the stored passwords in the file?\n\n"
           "The file then holds the password hashes, and anyone who has it can log "
           "in to the Directors with them. Without them, the passwords must be "
           "entered again after the import."),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    QString error;
    if (!exportProfiles(file, answer == QMessageBox::Yes, &error)) {
        QMessageBox::warning(this, tr("Export Profiles"), error);
        return;
    }
    QMessageBox::information(this, tr("Export Profiles"),
        tr("%n connection profile(s) exported.", nullptr, int(m_profiles.size())));
}

bool BSettingsDialog::exportProfiles(const QString &path, bool includeSecrets, QString *error)
{
    saveCurrentProfileSilent();
    QFile out(path);
    if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        if (error)
            *error = tr("Cannot write %1: %2").arg(path, out.errorString());
        return false;
    }
    if (includeSecrets) {
        // Secrets: readable by the owner only
        out.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }
    const QByteArray data = BSettings::profilesExportJson(m_profiles, includeSecrets);
    if (out.write(data) != data.size()) {
        if (error)
            *error = tr("Cannot write %1: %2").arg(path, out.errorString());
        return false;
    }
    return true;
}

void BSettingsDialog::onImportSettings()
{
    const QString file = QFileDialog::getOpenFileName(this, tr("Import Profiles"),
        QString(), tr("JSON (*.json)"));
    if (file.isEmpty())
        return;

    QString error;
    const int count = importProfiles(file, &error);
    if (count == 0) {
        QMessageBox::warning(this, tr("Import Profiles"), error);
        return;
    }
    QMessageBox::information(this, tr("Import Profiles"),
        tr("%n connection profile(s) imported. They are stored when you save the settings.",
           nullptr, count));
}

int BSettingsDialog::importProfiles(const QString &path, QString *error)
{
    QFile in(path);
    if (!in.open(QIODevice::ReadOnly)) {
        if (error)
            *error = tr("Cannot read %1: %2").arg(path, in.errorString());
        return 0;
    }
    const QList<BConnectionProfile> imported = BSettings::profilesFromExportJson(in.readAll(), error);
    if (imported.isEmpty())
        return 0;

    saveCurrentProfileSilent();
    const int count = BSettings::mergeImportedProfiles(m_profiles, imported);

    // The list again from the working copy, the selection kept
    const QString selected = m_currentProfileId;
    m_currentProfileId.clear();
    {
        QSignalBlocker blocker(m_profileList);
        m_profileList->clear();
    }
    for (const BConnectionProfile &profile : std::as_const(m_profiles))
        addProfileToList(profile, false);
    const int row = profileRow(selected.isEmpty() ? imported.first().id : selected);
    m_profileList->setCurrentRow(row >= 0 ? row : 0);
    onProfileSelectionChanged();
    return count;
}

void BSettingsDialog::onClearStoredConnections()
{
    int profileCount = int(m_profiles.count());
    if (profileCount == 0) {
        QMessageBox::information(this, tr("No Profiles"),
            tr("There are no connection profiles to delete."));
        return;
    }

    int ret = QMessageBox::warning(this, tr("Clear All Profiles"),
        tr("Do you really want to delete all %1 connection profile(s)?\n\n"
           "This includes all passwords and certificate paths.\n"
           "This action cannot be undone.").arg(profileCount),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // Working copy: stored when the settings are saved
        m_profiles.clear();
        m_lastUsedProfileId.clear();

        // Clear the list
        {
            QSignalBlocker blocker(m_profileList);
            m_profileList->clear();
        }
        m_currentProfileId.clear();
        m_profileDetailsWidget->setVisible(false);

        QMessageBox::information(this, tr("Deleted"),
            tr("All connection profiles have been removed. They are deleted when you save the settings."));
    }
}

void BSettingsDialog::showColor(QPushButton *button, const QColor &color)
{
    button->setStyleSheet(color.isValid()
        ? QString("background-color: %1; border: 1px solid #888;").arg(color.name())
        : QString());
}

void BSettingsDialog::chooseLevelColor(const QString &level, QPushButton *button, const QString &title)
{
    const QColor current = m_levelColors.value(level);
    const QColor chosen = QColorDialog::getColor(current, this, title);
    if (chosen.isValid() && chosen != current) {
        // Stored with the other settings when they are saved
        m_levelColors.insert(level, chosen);
        showColor(button, chosen);
    }
}

void BSettingsDialog::onChooseColorFull()
{
    chooseLevelColor("F", m_colorButtonFull, tr("Choose Color for Full Backup"));
}

void BSettingsDialog::onChooseColorIncremental()
{
    chooseLevelColor("I", m_colorButtonIncremental, tr("Choose Color for Incremental Backup"));
}

void BSettingsDialog::onChooseColorDifferential()
{
    chooseLevelColor("D", m_colorButtonDifferential, tr("Choose Color for Differential Backup"));
}

void BSettingsDialog::onChooseColorVirtualFull()
{
    chooseLevelColor("V", m_colorButtonVirtualFull, tr("Choose Color for Virtual Full Backup"));
}

void BSettingsDialog::onChooseColorConnected()
{
    const QColor chosen = QColorDialog::getColor(m_connectedColor, this,
                                                 tr("Choose Color for Connected Status"));
    if (chosen.isValid() && chosen != m_connectedColor) {
        m_connectedColor = chosen;
        showColor(m_colorButtonConnected, chosen);
    }
}

void BSettingsDialog::onChooseColorDisconnected()
{
    const QColor chosen = QColorDialog::getColor(m_disconnectedColor, this,
                                                 tr("Choose Color for Disconnected Status"));
    if (chosen.isValid() && chosen != m_disconnectedColor) {
        m_disconnectedColor = chosen;
        showColor(m_colorButtonDisconnected, chosen);
    }
}

// ============================================================================
// Connection Profile Management
// ============================================================================

void BSettingsDialog::onProfileSelectionChanged()
{
    // Auto-save the previously edited profile before switching
    if (!m_currentProfileId.isEmpty()) {
        saveCurrentProfileSilent();
    }

    int currentRow = m_profileList->currentRow();
    bool hasSelection = (currentRow >= 0);

    // Enable/disable buttons based on selection
    m_duplicateProfileButton->setEnabled(hasSelection);
    m_deleteProfileButton->setEnabled(hasSelection);

    // Show/hide details widget
    m_profileDetailsWidget->setVisible(hasSelection);

    if (hasSelection) {
        // Load the selected profile into the form
        QListWidgetItem *item = m_profileList->currentItem();
        if (item) {
            QString profileId = item->data(Qt::UserRole).toString();
            const int index = profileIndex(profileId);
            const BConnectionProfile profile = index >= 0 ? m_profiles.at(index) : BConnectionProfile();

            if (!profile.id.isEmpty()) {
                m_currentProfileId = profileId;

                // Block signals to prevent triggering saves
                QSignalBlocker blocker1(m_profileNameEdit);
                QSignalBlocker blocker2(m_hostEdit);
                QSignalBlocker blocker3(m_portSpin);
                QSignalBlocker blocker4(m_directorEdit);
                QSignalBlocker blocker5(m_consoleEdit);
                QSignalBlocker blocker6(m_passwordEdit);
                QSignalBlocker blocker7(m_useCertificatesCheck);
                QSignalBlocker blocker10(m_serverPlatformCombo);
                QSignalBlocker blocker11(m_transportCombo);

                m_profileNameEdit->setText(profile.name);
                m_hostEdit->setText(profile.host);
                m_portSpin->setValue(profile.port);
                int platformIdx = m_serverPlatformCombo->findData(profile.serverPlatform);
                if (platformIdx >= 0) m_serverPlatformCombo->setCurrentIndex(platformIdx);
                m_directorEdit->setText(profile.directorName);
                m_consoleEdit->setText(profile.consoleName);

                // Password: Show placeholder and display stored MD5 hash
                if (profile.hasValidPasswordHash()) {
                    m_passwordEdit->setPlaceholderText(tr("••••••••  (password saved)"));
                    m_passwordEdit->clear();
                    // Show the stored MD5 hash
                    m_md5Label->setText(profile.passwordHash);
                    bSetLabelState(m_md5Label, "");  // theme text colour
                } else {
                    m_passwordEdit->setPlaceholderText(tr("Enter password"));
                    m_passwordEdit->clear();
                    m_md5Label->setText(tr("(password MD5 hash will appear here)"));
                    bSetLabelState(m_md5Label, "muted");
                }

                // Auth method - checkbox checked = use certificates (not PSK)
                m_useCertificatesCheck->setChecked(!profile.tlsUsePSK);

                // TLS certificates
#ifndef Q_OS_WINDOWS
                m_caCertEdit->setText(profile.tlsCaCertFile);
                m_clientCertEdit->setText(profile.tlsCertFile);
                m_clientKeyEdit->setText(profile.tlsKeyFile);
#else
                m_clientCertEdit->setText(profile.tlsPfxFile);
#endif
                m_verifyPeerCheck->setChecked(profile.tlsVerifyPeer);

                // Load cipher list
                m_cipherListEdit->setText(profile.tlsCipherList);

                // Manually enable/disable certWidget since signals were blocked
                m_certWidget->setEnabled(m_useCertificatesCheck->isChecked());

                // Protocol
                m_transportCombo->setCurrentIndex(m_transportCombo->findData(
                    BConnectionProfile::transportToString(profile.transport)));
                m_grpcPortSpin->setValue(profile.grpcPort > 0 ? profile.grpcPort : 10101);
                m_grpcCertFingerprint = profile.grpcCertFingerprint;
                updateGrpcFingerprintDisplay();
                onTransportChanged();
            }
        }
    } else {
        m_currentProfileId.clear();
    }
}

int BSettingsDialog::profileRow(const QString &profileId) const
{
    for (int row = 0; row < m_profileList->count(); ++row) {
        if (m_profileList->item(row)->data(Qt::UserRole).toString() == profileId)
            return row;
    }
    return -1;
}

int BSettingsDialog::profileIndex(const QString &profileId) const
{
    for (int i = 0; i < m_profiles.size(); ++i) {
        if (m_profiles.at(i).id == profileId)
            return i;
    }
    return -1;
}

void BSettingsDialog::addProfileToList(const BConnectionProfile &profile, bool select)
{
    QListWidgetItem *item = new QListWidgetItem(profile.displayName());
    item->setData(Qt::UserRole, profile.id);
    m_profileList->addItem(item);
    if (select)
        m_profileList->setCurrentItem(item);
}

void BSettingsDialog::onAddProfile()
{
    // Working copy: stored when the settings are saved
    BConnectionProfile profile = BConnectionProfile::create(tr("New Connection"));
    m_profiles.append(profile);
    addProfileToList(profile, true);

    // Focus the name edit for immediate renaming
    m_profileNameEdit->setFocus();
    m_profileNameEdit->selectAll();
}

void BSettingsDialog::onEditProfile()
{
    // Double click: the details of the selected profile are shown already
    if (m_profileList->currentRow() >= 0) {
        m_profileDetailsWidget->setVisible(true);
        m_profileNameEdit->setFocus();
    }
}

void BSettingsDialog::onDeleteProfile()
{
    int currentRow = m_profileList->currentRow();
    if (currentRow < 0) return;

    QListWidgetItem *item = m_profileList->currentItem();
    if (!item) return;

    QString profileId = item->data(Qt::UserRole).toString();
    QString profileName = item->text();

    int ret = QMessageBox::question(this, tr("Delete Profile"),
        tr("Do you really want to delete the connection profile \"%1\"?\n\n"
           "It is deleted when you save the settings.").arg(profileName),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // Forget the form first: the row that is selected next must not
        // receive the deleted profile's fields
        if (m_currentProfileId == profileId)
            m_currentProfileId.clear();
        const int index = profileIndex(profileId);
        if (index >= 0)
            m_profiles.removeAt(index);
        delete m_profileList->takeItem(currentRow);
    }
}

void BSettingsDialog::onDuplicateProfile()
{
    QListWidgetItem *item = m_profileList->currentItem();
    if (!item) return;

    // The form first, so the copy has the edits not saved yet
    saveCurrentProfileSilent();
    const int index = profileIndex(item->data(Qt::UserRole).toString());
    if (index < 0) return;

    // Create a copy with a new ID
    BConnectionProfile copy = m_profiles.at(index);
    copy.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    copy.name = tr("%1 (Copy)").arg(m_profiles.at(index).name);
    m_profiles.append(copy);
    addProfileToList(copy, true);
}

void BSettingsDialog::saveCurrentProfileSilent()
{
    if (m_currentProfileId.isEmpty()) return;

    const int index = profileIndex(m_currentProfileId);
    if (index < 0) return;
    BConnectionProfile profile = m_profiles.at(index);
    // Update from form fields
    profile.name = m_profileNameEdit->text().trimmed();
    if (profile.name.isEmpty()) {
        profile.name = tr("Unnamed Connection");
    }
    profile.host = m_hostEdit->text().trimmed();
    profile.port = m_portSpin->value();
    profile.serverPlatform = m_serverPlatformCombo->currentData().toString();
    profile.directorName = m_directorEdit->text().trimmed();
    profile.consoleName = m_consoleEdit->text().trimmed();

    // Password handling: Only update if user entered new password
    if (m_savePasswordCheck->isChecked()) {
        QString newPassword = m_passwordEdit->text();
        if (!newPassword.isEmpty()) {
            // User entered a new password - hash it
            profile.setPasswordFromCleartext(newPassword);
        }
        // else: keep existing passwordHash (user didn't change it)
    } else {
        // Don't save password
        profile.passwordHash.clear();
    }

    // Auth method - legacy mode is disabled, TLS always enabled
    profile.legacyAuth = false;
    profile.tlsEnabled = true;
    profile.tlsUsePSK = !m_useCertificatesCheck->isChecked();  // PSK unless certificates checked

    // TLS certificates
#ifndef Q_OS_WINDOWS
    profile.tlsCaCertFile = m_caCertEdit->text();
    profile.tlsCertFile = m_clientCertEdit->text();
    profile.tlsKeyFile = m_clientKeyEdit->text();
#else
    profile.tlsPfxFile = m_clientCertEdit->text();
#endif
    profile.tlsVerifyPeer = m_verifyPeerCheck->isChecked();
    profile.tlsCipherList = m_cipherListEdit->text().trimmed();

    // Protocol
    profile.transport = BConnectionProfile::transportFromString(
        m_transportCombo->currentData().toString());
    profile.grpcPort = m_grpcPortSpin->value();
    profile.grpcCertFingerprint = m_grpcCertFingerprint;

    // Into the working copy; the row of this profile gets its name (the
    // current row may already be the next profile when the selection moves)
    m_profiles[index] = profile;
    const int row = profileRow(profile.id);
    if (row >= 0)
        m_profileList->item(row)->setText(profile.displayName());
}

void BSettingsDialog::onTransportChanged()
{
    const bool grpc = m_transportCombo->currentData().toString() == "grpc";
    m_grpcPortSpin->setEnabled(grpc);
    m_forgetGrpcCertButton->setEnabled(grpc && !m_grpcCertFingerprint.isEmpty());

    // gRPC: BNET TLS/PSK options do not apply
    m_tlsGroupBox->setVisible(!grpc);
    m_grpcInfoLabel->setVisible(grpc);
}

void BSettingsDialog::onForgetGrpcCertificate()
{
    m_grpcCertFingerprint.clear();
    updateGrpcFingerprintDisplay();
    onTransportChanged();
}

void BSettingsDialog::updateGrpcFingerprintDisplay()
{
    if (m_grpcCertFingerprint.isEmpty()) {
        m_grpcFingerprintLabel->setText(tr("(not pinned - confirmed on next connection)"));
        m_grpcFingerprintLabel->setToolTip(QString());
    } else {
        m_grpcFingerprintLabel->setText(QString("<tt>%1</tt>").arg(
            BConnectionProfile::formatFingerprint(m_grpcCertFingerprint, true)));
        m_grpcFingerprintLabel->setToolTip(tr("SHA-256: %1").arg(
            BConnectionProfile::formatFingerprint(m_grpcCertFingerprint)));
    }
}

void BSettingsDialog::onConvertToPFX()
{
    BPFXConverter converter(this);
    if (converter.exec() == QDialog::Accepted) {
        // User successfully converted and created a PFX file
        QString pfxPath = converter.getPFXFilePath();
        if (!pfxPath.isEmpty()) {
            // Update the PFX file path in the current profile
            m_clientCertEdit->setText(pfxPath);

            QMessageBox::information(this, tr("PFX File Created"),
                tr("PFX file has been created successfully.\n\n"
                   "The certificate path has been updated in the current profile."));
        }
    }
}
