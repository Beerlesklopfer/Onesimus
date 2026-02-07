#include "bsettingsdialog.h"
#include "ui_bsettingsdialog.h"
#include "bsettings.h"
#include "btranslations.h"
#include "bconnectionprofile.h"
#include "bpfxconverter.h"
#include "blogging.h"

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
    resize(900, 600);
    setModal(true);

    setupUI();
    loadSettings();
    // applyModernStyle();
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

    m_resetButton = new QPushButton(tr("Reset"), this);
    m_resetButton->setObjectName("resetButton");
    m_cancelButton = new QPushButton(tr("Cancel"), this);
    m_cancelButton->setObjectName("cancelButton");
    m_applyButton = new QPushButton(tr("Apply"), this);
    m_applyButton->setObjectName("applyButton");
    m_applyButton->setDefault(true);

    buttonLayout->addWidget(m_resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
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
    m_categoryList->setFixedWidth(220);
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
    m_profileList->setMinimumHeight(120);
    m_profileList->setMaximumHeight(180);
    m_profileList->setSelectionMode(QAbstractItemView::SingleSelection);
    profilesLayout->addWidget(m_profileList);

    // Profile buttons
    QHBoxLayout *profileButtonLayout = new QHBoxLayout();
    m_addProfileButton = new QPushButton(tr("+ Add"));
    m_addProfileButton->setToolTip(tr("Add a new connection profile"));
    m_editProfileButton = new QPushButton(tr("Edit"));
    m_editProfileButton->setEnabled(false);
    m_duplicateProfileButton = new QPushButton(tr("Duplicate"));
    m_duplicateProfileButton->setEnabled(false);
    m_deleteProfileButton = new QPushButton(tr("Delete"));
    m_deleteProfileButton->setObjectName("dangerButton");
    m_deleteProfileButton->setEnabled(false);

    profileButtonLayout->addWidget(m_addProfileButton);
    profileButtonLayout->addWidget(m_editProfileButton);
    profileButtonLayout->addWidget(m_duplicateProfileButton);
    profileButtonLayout->addStretch();
    profileButtonLayout->addWidget(m_deleteProfileButton);
    profilesLayout->addLayout(profileButtonLayout);

    layout->addWidget(profilesGroup);

    // Connect profile list signals
    connect(m_profileList, &QListWidget::currentRowChanged, this, &BSettingsDialog::onProfileSelectionChanged);
    connect(m_profileList, &QListWidget::itemDoubleClicked, this, &BSettingsDialog::onEditProfile);
    connect(m_addProfileButton, &QPushButton::clicked, this, &BSettingsDialog::onAddProfile);
    connect(m_editProfileButton, &QPushButton::clicked, this, &BSettingsDialog::onEditProfile);
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
    m_profileNameEdit->setPlaceholderText(tr("e.g. Production Server, Test Environment"));
    nameLayout->addRow(tr("Profile Name:"), m_profileNameEdit);

    detailsLayout->addWidget(nameGroup);

    // Director Connection Settings
    QGroupBox *bconsoleGroup = new QGroupBox(tr("Director Connection"));
    bconsoleGroup->setObjectName("settingsGroup");
    QFormLayout *bconsoleLayout = new QFormLayout(bconsoleGroup);
    bconsoleLayout->setSpacing(12);

    m_hostEdit = new QLineEdit();
    m_hostEdit->setPlaceholderText(tr("e.g. 192.168.1.100 or bareos-dir.local"));
    bconsoleLayout->addRow(tr("Host:"), m_hostEdit);

    m_portSpin = new QSpinBox();
    m_portSpin->setRange(1, 65535);
    m_portSpin->setValue(9101);
    bconsoleLayout->addRow(tr("Port:"), m_portSpin);

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
    m_md5Label->setStyleSheet("QLabel { color: #666; font-family: monospace; font-size: 11px; }");
    m_md5Label->setText(tr("(password MD5 hash will appear here)"));
    bconsoleLayout->addRow(tr("MD5 Hash:"), m_md5Label);

    // Update MD5 hash in real-time as user types
    connect(m_passwordEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (text.isEmpty()) {
            m_md5Label->setText(tr("(password MD5 hash will appear here)"));
            m_md5Label->setStyleSheet("QLabel { color: #666; font-family: monospace; font-size: 11px; }");
        } else if (text.startsWith("[md5]")) {
            // Already an MD5 hash with prefix - extract and display
            QString hash = text.mid(5);
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

    QHBoxLayout *timeoutLayout = new QHBoxLayout();
    QLabel *timeoutLabel = new QLabel(tr("Connection timeout:"));
    m_connectionTimeoutSpin = new QSpinBox();
    m_connectionTimeoutSpin->setRange(5, 120);
    m_connectionTimeoutSpin->setValue(30);
    m_connectionTimeoutSpin->setSuffix(tr(" seconds"));
    timeoutLayout->addWidget(timeoutLabel);
    timeoutLayout->addWidget(m_connectionTimeoutSpin);
    timeoutLayout->addStretch();
    optionsLayout->addLayout(timeoutLayout);

    detailsLayout->addWidget(optionsGroup);

    detailsLayout->addStretch();

    // Save profile button - slim, full width at bottom
    QPushButton *saveProfileBtn = new QPushButton(tr("Save Profile"));
    saveProfileBtn->setObjectName("applyButton");
    saveProfileBtn->setToolTip(tr("Save changes to this connection profile"));
    connect(saveProfileBtn, &QPushButton::clicked, this, &BSettingsDialog::saveCurrentProfile);
    detailsLayout->addWidget(saveProfileBtn);

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
    layout->addWidget(titleLabel);

    // Theme
    QGroupBox *themeGroup = new QGroupBox(tr("Color Scheme"));
    themeGroup->setObjectName("settingsGroup");
    QFormLayout *themeLayout = new QFormLayout(themeGroup);

    m_themeCombo = new QComboBox();
    m_themeCombo->addItem(tr("🌙 Dark (Industrial)"), "dark");
    m_themeCombo->addItem(tr("☀️ Light"), "light");
    m_themeCombo->addItem(tr("🖥️ System"), "system");
    themeLayout->addRow(tr("Theme:"), m_themeCombo);

    layout->addWidget(themeGroup);

    // Font
    QGroupBox *fontGroup = new QGroupBox(tr("Font Size"));
    fontGroup->setObjectName("settingsGroup");
    QFormLayout *fontLayout = new QFormLayout(fontGroup);

    m_fontSizeSpin = new QSpinBox();
    m_fontSizeSpin->setRange(8, 16);
    m_fontSizeSpin->setValue(10);
    m_fontSizeSpin->setSuffix(" pt");
    fontLayout->addRow(tr("Base Font Size:"), m_fontSizeSpin);

    layout->addWidget(fontGroup);

    // UI Options
    QGroupBox *uiGroup = new QGroupBox(tr("User Interface"));
    uiGroup->setObjectName("settingsGroup");
    QVBoxLayout *uiLayout = new QVBoxLayout(uiGroup);

    m_animationsCheck = new QCheckBox(tr("Enable animations"));
    m_animationsCheck->setChecked(true);
    uiLayout->addWidget(m_animationsCheck);

    m_compactModeCheck = new QCheckBox(tr("Compact mode"));
    uiLayout->addWidget(m_compactModeCheck);

    layout->addWidget(uiGroup);

    // Language Selection
    QGroupBox *languageGroup = new QGroupBox(tr("Language"));
    languageGroup->setObjectName("settingsGroup");
    QFormLayout *languageLayout = new QFormLayout(languageGroup);

    m_languageCombo = new QComboBox();

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
        tr("Language changes require an application restart."));
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: #888; font-size: 9pt; font-style: italic;");
    languageLayout->addRow("", infoLabel);

    layout->addWidget(languageGroup);

    // Status Bar Colors
    QGroupBox *statusBarColorsGroup = new QGroupBox(tr("Status Bar Colors"));
    statusBarColorsGroup->setObjectName("settingsGroup");
    QFormLayout *statusBarColorsLayout = new QFormLayout(statusBarColorsGroup);
    statusBarColorsLayout->setSpacing(12);

    // Connected Color
    m_colorButtonConnected = new QPushButton();
    m_colorButtonConnected->setMinimumSize(80, 30);
    m_colorButtonConnected->setCursor(Qt::PointingHandCursor);
    m_colorButtonConnected->setToolTip(tr("Color shown when connected to Director"));
    connect(m_colorButtonConnected, &QPushButton::clicked, this, &BSettingsDialog::onChooseColorConnected);
    statusBarColorsLayout->addRow(tr("Connected:"), m_colorButtonConnected);

    // Disconnected Color
    m_colorButtonDisconnected = new QPushButton();
    m_colorButtonDisconnected->setMinimumSize(80, 30);
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
    m_colorButtonFull->setMinimumSize(80, 30);
    m_colorButtonFull->setCursor(Qt::PointingHandCursor);
    connect(m_colorButtonFull, &QPushButton::clicked, this, &BSettingsDialog::onChooseColorFull);
    levelColorsLayout->addRow("Full (F):", m_colorButtonFull);

    // Incremental Backup Color
    m_colorButtonIncremental = new QPushButton();
    m_colorButtonIncremental->setMinimumSize(80, 30);
    m_colorButtonIncremental->setCursor(Qt::PointingHandCursor);
    connect(m_colorButtonIncremental, &QPushButton::clicked, this, &BSettingsDialog::onChooseColorIncremental);
    levelColorsLayout->addRow("Incremental (I):", m_colorButtonIncremental);

    // Differential Backup Color
    m_colorButtonDifferential = new QPushButton();
    m_colorButtonDifferential->setMinimumSize(80, 30);
    m_colorButtonDifferential->setCursor(Qt::PointingHandCursor);
    connect(m_colorButtonDifferential, &QPushButton::clicked, this, &BSettingsDialog::onChooseColorDifferential);
    levelColorsLayout->addRow("Differential (D):", m_colorButtonDifferential);

    // Virtual Full Backup Color
    m_colorButtonVirtualFull = new QPushButton();
    m_colorButtonVirtualFull->setMinimumSize(80, 30);
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
    layout->addWidget(titleLabel);

    // Confirmations
    QGroupBox *confirmGroup = new QGroupBox(tr("Confirmations"));
    confirmGroup->setObjectName("settingsGroup");
    QVBoxLayout *confirmLayout = new QVBoxLayout(confirmGroup);

    m_confirmJobCancelCheck = new QCheckBox(tr("Confirm job cancellation"));
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

    layout->addWidget(displayGroup);

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

void BSettingsDialog::applyModernStyle()
{
    setStyleSheet(R"(
        /* Main Dialog */
        BSettingsDialog {
            background-color: #1a1d23;
            color: #e4e6eb;
        }

        /* Sidebar */
        QListWidget#categoryList {
            background-color: #0f1115;
            border: none;
            border-right: 2px solid #2d3139;
            outline: none;
            padding: 20px 0;
        }

        QListWidget#categoryList::item {
            padding: 16px 20px;
            margin: 2px 10px;
            border-radius: 6px;
            color: #8b92a0;
            font-size: 13px;
            font-weight: 500;
        }

        QListWidget#categoryList::item:selected {
            background-color: #2d5a8f;
            color: #ffffff;
        }

        QListWidget#categoryList::item:hover {
            background-color: #252932;
            color: #e4e6eb;
        }

        /* Page Titles */
        QLabel#pageTitle {
            font-size: 24px;
            font-weight: 600;
            color: #ffffff;
            padding-bottom: 10px;
            border-bottom: 2px solid #2d5a8f;
        }

        /* Group Boxes */
        QGroupBox#settingsGroup {
            font-size: 13px;
            font-weight: 600;
            color: #c4c9d4;
            border: 1px solid #2d3139;
            border-radius: 8px;
            margin-top: 16px;
            padding-top: 24px;
            background-color: #171a1f;
        }

        QGroupBox#settingsGroup::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 4px 12px;
            margin-left: 8px;
            background-color: #2d5a8f;
            border-radius: 4px;
            color: #ffffff;
        }

        /* Input Fields */
        QLineEdit, QSpinBox {
            background-color: #0f1115;
            border: 1px solid #2d3139;
            border-radius: 6px;
            padding: 10px 12px;
            color: #e4e6eb;
            font-size: 12px;
        }

        QLineEdit:focus, QSpinBox:focus {
            border-color: #2d5a8f;
            background-color: #1a1d23;
        }

        QLineEdit::placeholder {
            color: #5a6270;
        }

        /* ComboBox */
        QComboBox {
            background-color: #0f1115;
            border: 1px solid #2d3139;
            border-radius: 6px;
            padding: 10px 12px;
            color: #e4e6eb;
            min-width: 200px;
        }

        QComboBox:hover {
            border-color: #2d5a8f;
        }

        QComboBox::drop-down {
            border: none;
            width: 30px;
        }

        QComboBox QAbstractItemView {
            background-color: #1a1d23;
            border: 1px solid #2d3139;
            selection-background-color: #2d5a8f;
            color: #e4e6eb;
        }

        /* Checkboxes */
        QCheckBox {
            color: #e4e6eb;
            spacing: 8px;
            font-size: 12px;
        }

        QCheckBox#prominentCheckbox {
            font-size: 14px;
            font-weight: 600;
            color: #ffffff;
        }

        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #2d3139;
            border-radius: 4px;
            background-color: #0f1115;
        }

        QCheckBox::indicator:hover {
            border-color: #2d5a8f;
        }

        QCheckBox::indicator:checked {
            background-color: #2d5a8f;
            border-color: #2d5a8f;
            image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTIiIGhlaWdodD0iMTIiIHZpZXdCb3g9IjAgMCAxMiAxMiIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTkuNSAzLjVMNC41IDguNUwyLjUgNi41IiBzdHJva2U9IndoaXRlIiBzdHJva2Utd2lkdGg9IjIiIHN0cm9rZS1saW5lY2FwPSJyb3VuZCIgc3Ryb2tlLWxpbmVqb2luPSJyb3VuZCIvPgo8L3N2Zz4K);
        }

        /* Buttons */
        QPushButton {
            background-color: #252932;
            color: #e4e6eb;
            border: 1px solid #2d3139;
            border-radius: 6px;
            padding: 10px 20px;
            font-size: 12px;
            font-weight: 500;
        }

        QPushButton:hover {
            background-color: #2d3139;
            border-color: #3a4048;
        }

        QPushButton:pressed {
            background-color: #1a1d23;
        }

        QPushButton#applyButton {
            background-color: #2d5a8f;
            color: #ffffff;
            border: none;
            padding: 12px 32px;
        }

        QPushButton#applyButton:hover {
            background-color: #3a6ba5;
        }

        QPushButton#cancelButton {
            background-color: transparent;
            border: 1px solid #2d3139;
        }

        QPushButton#resetButton {
            background-color: transparent;
            color: #8b92a0;
        }

        QPushButton#browseButton {
            padding: 8px 16px;
        }

        QPushButton#dangerButton {
            background-color: #8f2d2d;
            color: #ffffff;
            border: none;
        }

        QPushButton#dangerButton:hover {
            background-color: #a53a3a;
        }

        /* Info/Warning Labels */
        QLabel#infoLabel {
            background-color: #1a2d3a;
            border-left: 3px solid #2d5a8f;
            padding: 12px;
            border-radius: 4px;
            color: #c4d9e8;
        }

        QLabel#warningLabel {
            background-color: #3a2d1a;
            border-left: 3px solid #8f6a2d;
            padding: 12px;
            border-radius: 4px;
            color: #e8d9c4;
        }
    )");
}

void BSettingsDialog::loadSettings()
{
    BSettings& settings = BSettings::instance();

    // ========================================================================
    // Connection Profiles
    // ========================================================================

    // Migrate old settings if needed
    settings.migrateOldConnectionSettings();

    // Load profiles into list
    m_profileList->clear();
    QList<BConnectionProfile> profiles = settings.connectionProfiles();
    QString lastUsedId = settings.lastUsedProfileId();

    for (const BConnectionProfile &profile : profiles) {
        QListWidgetItem *item = new QListWidgetItem(profile.displayName());
        item->setData(Qt::UserRole, profile.id);
        m_profileList->addItem(item);

        // Select the last used profile
        if (profile.id == lastUsedId) {
            m_profileList->setCurrentItem(item);
        }
    }

    // If no profile was selected, select the first one
    if (m_profileList->currentRow() < 0 && m_profileList->count() > 0) {
        m_profileList->setCurrentRow(0);
    }

    // Global connection options (not per-profile)
    m_savePasswordCheck->setChecked(settings.connectionSavePassword());
    m_autoConnectCheck->setChecked(settings.connectionAutoConnect());
    m_connectionTimeoutSpin->setValue(settings.connectionTimeout());

    // Appearance
    QString theme = settings.appearanceTheme();
    int themeIndex = m_themeCombo->findData(theme);
    if (themeIndex >= 0) m_themeCombo->setCurrentIndex(themeIndex);
    m_fontSizeSpin->setValue(settings.appearanceFontSize());
    m_animationsCheck->setChecked(settings.appearanceAnimations());
    m_compactModeCheck->setChecked(settings.appearanceCompactMode());

    // Language
    QString currentLangCode = settings.appearanceLanguage();
    int langIndex = m_languageCombo->findData(currentLangCode);
    if (langIndex >= 0) {
        m_languageCombo->setCurrentIndex(langIndex);
    }

    // Level Colors
    QColor colorFull = settings.levelColor("F");
    if (colorFull.isValid()) {
        QString styleSheet = QString("background-color: %1; border: 1px solid #888;").arg(colorFull.name());
        m_colorButtonFull->setStyleSheet(styleSheet);
    }

    QColor colorIncremental = settings.levelColor("I");
    if (colorIncremental.isValid()) {
        QString styleSheet = QString("background-color: %1; border: 1px solid #888;").arg(colorIncremental.name());
        m_colorButtonIncremental->setStyleSheet(styleSheet);
    }

    QColor colorDifferential = settings.levelColor("D");
    if (colorDifferential.isValid()) {
        QString styleSheet = QString("background-color: %1; border: 1px solid #888;").arg(colorDifferential.name());
        m_colorButtonDifferential->setStyleSheet(styleSheet);
    }

    QColor colorVirtualFull = settings.levelColor("V");
    if (colorVirtualFull.isValid()) {
        QString styleSheet = QString("background-color: %1; border: 1px solid #888;").arg(colorVirtualFull.name());
        m_colorButtonVirtualFull->setStyleSheet(styleSheet);
    }

    // Status Bar Colors
    QColor colorConnected = settings.statusBarConnectedColor();
    if (colorConnected.isValid()) {
        QString styleSheet = QString("background-color: %1; border: 1px solid #888;").arg(colorConnected.name());
        m_colorButtonConnected->setStyleSheet(styleSheet);
    }

    QColor colorDisconnected = settings.statusBarDisconnectedColor();
    if (colorDisconnected.isValid()) {
        QString styleSheet = QString("background-color: %1; border: 1px solid #888;").arg(colorDisconnected.name());
        m_colorButtonDisconnected->setStyleSheet(styleSheet);
    }

    // Behavior
    m_confirmJobCancelCheck->setChecked(settings.behaviorConfirmJobCancel());
    m_confirmJobStartCheck->setChecked(settings.behaviorConfirmJobStart());
    m_autoRefreshCheck->setChecked(settings.behaviorAutoRefresh());
    m_refreshIntervalSpin->setValue(settings.behaviorRefreshInterval());
    m_maxJobsDisplaySpin->setValue(settings.behaviorMaxJobsDisplay());
    m_jobsNewestFirstCheck->setChecked(settings.behaviorJobsNewestFirst());

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

    // Save the currently edited profile (if any)
    if (!m_currentProfileId.isEmpty()) {
        BConnectionProfile profile = settings.connectionProfile(m_currentProfileId);
        if (profile.isValid()) {
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

            profile.legacyAuth = false;  // Legacy mode is disabled
            profile.tlsEnabled = true;    // TLS always enabled
            profile.tlsUsePSK = !m_useCertificatesCheck->isChecked();  // PSK unless certificates checked

#ifndef Q_OS_WINDOWS
            profile.tlsCaCertFile = m_caCertEdit->text();
            profile.tlsCertFile = m_clientCertEdit->text();
            profile.tlsKeyFile = m_clientKeyEdit->text();
#else
            profile.tlsPfxFile = m_clientCertEdit->text();
#endif
            profile.tlsVerifyPeer = m_verifyPeerCheck->isChecked();
            profile.tlsCipherList = m_cipherListEdit->text().trimmed();

            settings.updateConnectionProfile(profile);
        }
    }

    // Save global connection options
    settings.setConnectionSavePassword(m_savePasswordCheck->isChecked());
    settings.setConnectionAutoConnect(m_autoConnectCheck->isChecked());
    settings.setConnectionTimeout(m_connectionTimeoutSpin->value());

    // Appearance
    settings.setAppearanceTheme(m_themeCombo->currentData().toString());
    settings.setAppearanceFontSize(m_fontSizeSpin->value());
    settings.setAppearanceAnimations(m_animationsCheck->isChecked());
    settings.setAppearanceCompactMode(m_compactModeCheck->isChecked());

    // Language
    QString selectedLangCode = m_languageCombo->currentData().toString();
    settings.setAppearanceLanguage(selectedLangCode);

    // Update language immediately (though full effect requires restart)
    BTranslations::Language lang = BTranslations::languageFromCode(selectedLangCode);
    BTranslations::instance()->setLanguage(lang);

    // Behavior
    settings.setBehaviorConfirmJobCancel(m_confirmJobCancelCheck->isChecked());
    settings.setBehaviorConfirmJobStart(m_confirmJobStartCheck->isChecked());
    settings.setBehaviorAutoRefresh(m_autoRefreshCheck->isChecked());
    settings.setBehaviorRefreshInterval(m_refreshIntervalSpin->value());
    settings.setBehaviorMaxJobsDisplay(m_maxJobsDisplaySpin->value());
    settings.setBehaviorJobsNewestFirst(m_jobsNewestFirstCheck->isChecked());

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

void BSettingsDialog::onResetToDefaultsClicked()
{
    int ret = QMessageBox::question(this, tr("Reset"),
        tr("Do you really want to reset all settings to default values?\n"
        "This action cannot be undone."),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        BSettings::instance().resetToDefaults();
        loadSettings();
        QMessageBox::information(this, tr("Reset"),
            tr("All settings have been reset to default values."));
    }
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
    QString file = QFileDialog::getSaveFileName(this, tr("Export Settings"),
        "bacula-settings.json", "JSON (*.json)");

    if (!file.isEmpty()) {
        QSettings settings;
        QJsonObject json;

        foreach (QString key, settings.allKeys()) {
            json[key] = settings.value(key).toString();
        }

        QJsonDocument doc(json);
        QFile outFile(file);
        if (outFile.open(QIODevice::WriteOnly)) {
            outFile.write(doc.toJson());
            outFile.close();
            QMessageBox::information(this, tr("Export"), tr("Settings exported successfully."));
        }
    }
}

void BSettingsDialog::onImportSettings()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Import Settings"),
        QString(), "JSON (*.json)");

    if (!file.isEmpty()) {
        QFile inFile(file);
        if (inFile.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(inFile.readAll());
            QJsonObject json = doc.object();

            QSettings settings;
            for (auto it = json.begin(); it != json.end(); ++it) {
                settings.setValue(it.key(), it.value().toString());
            }

            loadSettings();
            QMessageBox::information(this, tr("Import"), tr("Settings imported successfully."));
        }
    }
}

void BSettingsDialog::onClearStoredConnections()
{
    int profileCount = BSettings::instance().connectionProfiles().count();
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
        BSettings& settings = BSettings::instance();

        // Clear all profiles
        settings.setConnectionProfiles(QList<BConnectionProfile>());
        settings.setLastUsedProfileId(QString());
        settings.sync();

        // Clear the list
        m_profileList->clear();
        m_currentProfileId.clear();
        m_profileDetailsWidget->setVisible(false);

        QMessageBox::information(this, tr("Deleted"),
            tr("All connection profiles have been deleted."));
    }
}

void BSettingsDialog::onChooseColorFull()
{
    QColor currentColor = BSettings::instance().levelColor("F");
    QColor newColor = QColorDialog::getColor(currentColor, this, tr("Choose Color for Full Backup"));

    if (newColor.isValid() && newColor != currentColor) {
        BSettings::instance().setLevelColor("F", newColor);

        // Update button color
        QString styleSheet = QString("background-color: %1; border: 1px solid #888;").arg(newColor.name());
        m_colorButtonFull->setStyleSheet(styleSheet);
    }
}

void BSettingsDialog::onChooseColorIncremental()
{
    QColor currentColor = BSettings::instance().levelColor("I");
    QColor newColor = QColorDialog::getColor(currentColor, this, tr("Choose Color for Incremental Backup"));

    if (newColor.isValid() && newColor != currentColor) {
        BSettings::instance().setLevelColor("I", newColor);

        // Update button color
        QString styleSheet = QString("background-color: %1; border: 1px solid #888;").arg(newColor.name());
        m_colorButtonIncremental->setStyleSheet(styleSheet);
    }
}

void BSettingsDialog::onChooseColorDifferential()
{
    QColor currentColor = BSettings::instance().levelColor("D");
    QColor newColor = QColorDialog::getColor(currentColor, this, tr("Choose Color for Differential Backup"));

    if (newColor.isValid() && newColor != currentColor) {
        BSettings::instance().setLevelColor("D", newColor);

        // Update button color
        QString styleSheet = QString("background-color: %1; border: 1px solid #888;").arg(newColor.name());
        m_colorButtonDifferential->setStyleSheet(styleSheet);
    }
}

void BSettingsDialog::onChooseColorVirtualFull()
{
    QColor currentColor = BSettings::instance().levelColor("V");
    QColor newColor = QColorDialog::getColor(currentColor, this, tr("Choose Color for Virtual Full Backup"));

    if (newColor.isValid() && newColor != currentColor) {
        BSettings::instance().setLevelColor("V", newColor);

        // Update button color
        QString styleSheet = QString("background-color: %1; border: 1px solid #888;").arg(newColor.name());
        m_colorButtonVirtualFull->setStyleSheet(styleSheet);
    }
}

void BSettingsDialog::onChooseColorConnected()
{
    QColor currentColor = BSettings::instance().statusBarConnectedColor();
    QColor newColor = QColorDialog::getColor(currentColor, this, tr("Choose Color for Connected Status"));

    if (newColor.isValid() && newColor != currentColor) {
        BSettings::instance().setStatusBarConnectedColor(newColor);

        // Update button color
        QString styleSheet = QString("background-color: %1; border: 1px solid #888;").arg(newColor.name());
        m_colorButtonConnected->setStyleSheet(styleSheet);
    }
}

void BSettingsDialog::onChooseColorDisconnected()
{
    QColor currentColor = BSettings::instance().statusBarDisconnectedColor();
    QColor newColor = QColorDialog::getColor(currentColor, this, tr("Choose Color for Disconnected Status"));

    if (newColor.isValid() && newColor != currentColor) {
        BSettings::instance().setStatusBarDisconnectedColor(newColor);

        // Update button color
        QString styleSheet = QString("background-color: %1; border: 1px solid #888;").arg(newColor.name());
        m_colorButtonDisconnected->setStyleSheet(styleSheet);
    }
}

// ============================================================================
// Connection Profile Management
// ============================================================================

void BSettingsDialog::onProfileSelectionChanged()
{
    int currentRow = m_profileList->currentRow();
    bool hasSelection = (currentRow >= 0);

    // Enable/disable buttons based on selection
    m_editProfileButton->setEnabled(hasSelection);
    m_duplicateProfileButton->setEnabled(hasSelection);
    m_deleteProfileButton->setEnabled(hasSelection);

    // Show/hide details widget
    m_profileDetailsWidget->setVisible(hasSelection);

    if (hasSelection) {
        // Load the selected profile into the form
        QListWidgetItem *item = m_profileList->currentItem();
        if (item) {
            QString profileId = item->data(Qt::UserRole).toString();
            BConnectionProfile profile = BSettings::instance().connectionProfile(profileId);

            if (profile.isValid()) {
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
                    m_md5Label->setStyleSheet("QLabel { color: #000; font-family: monospace; font-size: 11px; font-weight: bold; }");
                } else {
                    m_passwordEdit->setPlaceholderText(tr("Enter password"));
                    m_passwordEdit->clear();
                    m_md5Label->setText(tr("(password MD5 hash will appear here)"));
                    m_md5Label->setStyleSheet("QLabel { color: #666; font-family: monospace; font-size: 11px; }");
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
            }
        }
    } else {
        m_currentProfileId.clear();
    }
}

void BSettingsDialog::onAddProfile()
{
    // Create a new profile with defaults
    BConnectionProfile profile = BConnectionProfile::create(tr("New Connection"));

    // Add to settings
    BSettings::instance().addConnectionProfile(profile);

    // Add to list
    QListWidgetItem *item = new QListWidgetItem(profile.displayName());
    item->setData(Qt::UserRole, profile.id);
    m_profileList->addItem(item);

    // Select the new profile
    m_profileList->setCurrentItem(item);

    // Focus the name edit for immediate renaming
    m_profileNameEdit->setFocus();
    m_profileNameEdit->selectAll();
}

void BSettingsDialog::onEditProfile()
{
    // Just ensure the profile is selected and details are visible
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
           "This action cannot be undone.").arg(profileName),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // Remove from settings
        BSettings::instance().removeConnectionProfile(profileId);

        // Remove from list
        delete m_profileList->takeItem(currentRow);

        // Clear current profile ID
        if (m_currentProfileId == profileId) {
            m_currentProfileId.clear();
        }
    }
}

void BSettingsDialog::onDuplicateProfile()
{
    int currentRow = m_profileList->currentRow();
    if (currentRow < 0) return;

    QListWidgetItem *item = m_profileList->currentItem();
    if (!item) return;

    QString profileId = item->data(Qt::UserRole).toString();
    BConnectionProfile original = BSettings::instance().connectionProfile(profileId);

    if (!original.isValid()) return;

    // Create a copy with a new ID
    BConnectionProfile copy = original;
    copy.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    copy.name = tr("%1 (Copy)").arg(original.name);

    // Add to settings
    BSettings::instance().addConnectionProfile(copy);

    // Add to list
    QListWidgetItem *newItem = new QListWidgetItem(copy.displayName());
    newItem->setData(Qt::UserRole, copy.id);
    m_profileList->addItem(newItem);

    // Select the new profile
    m_profileList->setCurrentItem(newItem);
}

void BSettingsDialog::saveCurrentProfile()
{
    if (m_currentProfileId.isEmpty()) return;

    BConnectionProfile profile = BSettings::instance().connectionProfile(m_currentProfileId);
    if (!profile.isValid()) return;

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

    // Save to settings
    BSettings::instance().updateConnectionProfile(profile);

    // Update list item text
    QListWidgetItem *item = m_profileList->currentItem();
    if (item) {
        item->setText(profile.displayName());
    }

    // Show confirmation
    QMessageBox::information(this, tr("Profile Saved"),
        tr("Connection profile \"%1\" has been saved.").arg(profile.name));
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
