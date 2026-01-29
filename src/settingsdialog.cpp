#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "bsettings.h"
#include "btranslations.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QColorDialog>

SettingsDialog::SettingsDialog(BDirector *director, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , m_director(director)
    , m_categoryList(nullptr)
    , m_contentStack(nullptr)
{
    ui->setupUi(this);

    setWindowTitle(tr("Settings"));
    resize(900, 600);
    setModal(true);

    setupUI();
    loadSettings();
    // applyModernStyle();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setupUI()
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
    connect(m_categoryList, &QListWidget::currentRowChanged, this, &SettingsDialog::onCategoryChanged);
    connect(m_applyButton, &QPushButton::clicked, this, &SettingsDialog::onApplyClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &SettingsDialog::onCancelClicked);
    connect(m_resetButton, &QPushButton::clicked, this, &SettingsDialog::onResetToDefaultsClicked);

    // Select first category
    m_categoryList->setCurrentRow(0);
}

void SettingsDialog::createSidebar()
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

void SettingsDialog::createContentPages()
{
    createConnectionPage();
    createAppearancePage();
    createBehaviorPage();
    createAdvancedPage();
}

void SettingsDialog::createConnectionPage()
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
    QLabel *titleLabel = new QLabel(tr("Connection Settings"));
    titleLabel->setObjectName("pageTitle");
    layout->addWidget(titleLabel);

    // Backup System Selection
    QGroupBox *systemGroup = new QGroupBox(tr("Backup System"));
    systemGroup->setObjectName("settingsGroup");
    QVBoxLayout *systemLayout = new QVBoxLayout(systemGroup);
    systemLayout->setSpacing(12);

    QLabel *systemInfoLabel = new QLabel(
        tr("ℹ️ Bareos is a fork of Bacula with additional features.\n"
        "Both systems use compatible protocols."));
    systemInfoLabel->setObjectName("infoLabel");
    systemInfoLabel->setWordWrap(true);
    systemLayout->addWidget(systemInfoLabel);

    layout->addWidget(systemGroup);

    // Bconsole Settings
    QGroupBox *bconsoleGroup = new QGroupBox(tr("Director Connection (Bconsole)"));
    bconsoleGroup->setObjectName("settingsGroup");
    QFormLayout *bconsoleLayout = new QFormLayout(bconsoleGroup);
    bconsoleLayout->setSpacing(12);

    m_hostEdit = new QLineEdit();
    m_hostEdit->setPlaceholderText(tr("e.g. 192.168.1.100 or bacula-dir.local"));
    bconsoleLayout->addRow(tr("Host:"), m_hostEdit);

    m_portSpin = new QSpinBox();
    m_portSpin->setRange(1, 65535);
    m_portSpin->setValue(9101);
    bconsoleLayout->addRow(tr("Port:"), m_portSpin);

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

    layout->addWidget(bconsoleGroup);

    // TLS Info Label (shown when TLS is disabled)
    QLabel *tlsWarningLabel = new QLabel(
        tr("⚠️ TLS encrypts communication with the Director.\n"
        "TLS is strongly recommended for production environments!"));
    tlsWarningLabel->setObjectName("warningLabel");
    tlsWarningLabel->setWordWrap(true);
    layout->addWidget(tlsWarningLabel);

    // TLS/SSL Settings (checkable GroupBox)
    m_tlsGroupBox = new QGroupBox(tr("TLS/SSL Encryption"));
    m_tlsGroupBox->setObjectName("settingsGroup");
    m_tlsGroupBox->setCheckable(true);
    m_tlsGroupBox->setChecked(true);  // Initial: TLS enabled
    QVBoxLayout *tlsLayout = new QVBoxLayout(m_tlsGroupBox);
    tlsLayout->setSpacing(12);

    // Connect signal to show/hide warning
    tlsWarningLabel->setVisible(!m_tlsGroupBox->isChecked());
    connect(m_tlsGroupBox, &QGroupBox::toggled, [tlsWarningLabel](bool checked) {
        tlsWarningLabel->setVisible(!checked);
    });

    // Authentication Method
    QLabel *authMethodLabel = new QLabel(tr("Authentication Method:"));
    authMethodLabel->setStyleSheet("font-weight: bold;");
    tlsLayout->addWidget(authMethodLabel);

    m_tlsPSKRadio = new QRadioButton(tr("PSK (Pre-Shared Key) - Standard for Bareos 18.2+"));
    m_tlsPSKRadio->setChecked(true);
    tlsLayout->addWidget(m_tlsPSKRadio);

    m_tlsCertificateRadio = new QRadioButton(tr("Certificate-based TLS Authentication"));
    tlsLayout->addWidget(m_tlsCertificateRadio);

    // Certificate Settings (only for Certificate mode)
    QWidget *certWidget = new QWidget();
    QFormLayout *certLayout = new QFormLayout(certWidget);
    certLayout->setSpacing(12);

#ifndef Q_OS_WINDOWS
    // Linux: Separate PEM files
    // CA Certificate
    QHBoxLayout *caLayout = new QHBoxLayout();
    m_caCertEdit = new QLineEdit();
    m_caCertEdit->setPlaceholderText(tr("Path to CA certificate (.pem)"));
    QPushButton *caBrowse = new QPushButton(tr("Browse..."));
    caBrowse->setObjectName("browseButton");
    connect(caBrowse, &QPushButton::clicked, this, &SettingsDialog::onBrowseCACert);
    caLayout->addWidget(m_caCertEdit);
    caLayout->addWidget(caBrowse);
    certLayout->addRow(tr("CA Certificate:"), caLayout);

    // Client Certificate
    QHBoxLayout *clientCertLayout = new QHBoxLayout();
    m_clientCertEdit = new QLineEdit();
    m_clientCertEdit->setPlaceholderText(tr("Path to client certificate (.pem)"));
    QPushButton *clientCertBrowse = new QPushButton(tr("Browse..."));
    clientCertBrowse->setObjectName("browseButton");
    connect(clientCertBrowse, &QPushButton::clicked, this, &SettingsDialog::onBrowseClientCert);
    clientCertLayout->addWidget(m_clientCertEdit);
    clientCertLayout->addWidget(clientCertBrowse);
    certLayout->addRow(tr("Client Certificate:"), clientCertLayout);

    // Private Key
    QHBoxLayout *keyLayout = new QHBoxLayout();
    m_clientKeyEdit = new QLineEdit();
    m_clientKeyEdit->setPlaceholderText(tr("Path to private key (.pem, .key)"));
    QPushButton *keyBrowse = new QPushButton(tr("Browse..."));
    keyBrowse->setObjectName("browseButton");
    connect(keyBrowse, &QPushButton::clicked, this, &SettingsDialog::onBrowseClientKey);
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
    connect(clientCertBrowse, &QPushButton::clicked, this, &SettingsDialog::onBrowseClientCert);
    clientCertLayout->addWidget(m_clientCertEdit);
    clientCertLayout->addWidget(clientCertBrowse);
    certLayout->addRow(tr("PFX Certificate:"), clientCertLayout);
#endif

    // Peer Verification
    m_verifyPeerCheck = new QCheckBox(tr("Verify server certificate (recommended)"));
    m_verifyPeerCheck->setChecked(true);
    certLayout->addRow("", m_verifyPeerCheck);

    certWidget->setEnabled(false);  // Disabled by default (PSK is selected)
    tlsLayout->addWidget(certWidget);

    // Only enable certificate fields when Certificate radio is selected
    connect(m_tlsCertificateRadio, &QRadioButton::toggled, certWidget, &QWidget::setEnabled);

    layout->addWidget(m_tlsGroupBox);

    // Connection Options
    QGroupBox *optionsGroup = new QGroupBox(tr("Connection Options"));
    optionsGroup->setObjectName("settingsGroup");
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);
    optionsLayout->setSpacing(12);

    m_savePasswordCheck = new QCheckBox(tr("Save password"));
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

    layout->addWidget(optionsGroup);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *exportBtn = new QPushButton(tr("Export Settings"));
    QPushButton *importBtn = new QPushButton(tr("Import Settings"));
    QPushButton *clearBtn = new QPushButton(tr("Clear Stored Connections"));
    clearBtn->setObjectName("dangerButton");

    connect(exportBtn, &QPushButton::clicked, this, &SettingsDialog::onExportSettings);
    connect(importBtn, &QPushButton::clicked, this, &SettingsDialog::onImportSettings);
    connect(clearBtn, &QPushButton::clicked, this, &SettingsDialog::onClearStoredConnections);

    buttonLayout->addWidget(exportBtn);
    buttonLayout->addWidget(importBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(clearBtn);
    optionsLayout->addLayout(buttonLayout);

    layout->addStretch();

    // Set content widget to scroll area
    scrollArea->setWidget(contentWidget);

    // Add scroll area to page
    QVBoxLayout *pageLayout = new QVBoxLayout(m_connectionPage);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(scrollArea);

    m_contentStack->addWidget(m_connectionPage);
}

void SettingsDialog::createAppearancePage()
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

    // Backup Level Colors
    QGroupBox *levelColorsGroup = new QGroupBox(tr("Backup Level Colors"));
    levelColorsGroup->setObjectName("settingsGroup");
    QFormLayout *levelColorsLayout = new QFormLayout(levelColorsGroup);
    levelColorsLayout->setSpacing(12);

    // Full Backup Color
    m_colorButtonFull = new QPushButton();
    m_colorButtonFull->setMinimumSize(80, 30);
    m_colorButtonFull->setCursor(Qt::PointingHandCursor);
    connect(m_colorButtonFull, &QPushButton::clicked, this, &SettingsDialog::onChooseColorFull);
    levelColorsLayout->addRow("Full (F):", m_colorButtonFull);

    // Incremental Backup Color
    m_colorButtonIncremental = new QPushButton();
    m_colorButtonIncremental->setMinimumSize(80, 30);
    m_colorButtonIncremental->setCursor(Qt::PointingHandCursor);
    connect(m_colorButtonIncremental, &QPushButton::clicked, this, &SettingsDialog::onChooseColorIncremental);
    levelColorsLayout->addRow("Incremental (I):", m_colorButtonIncremental);

    // Differential Backup Color
    m_colorButtonDifferential = new QPushButton();
    m_colorButtonDifferential->setMinimumSize(80, 30);
    m_colorButtonDifferential->setCursor(Qt::PointingHandCursor);
    connect(m_colorButtonDifferential, &QPushButton::clicked, this, &SettingsDialog::onChooseColorDifferential);
    levelColorsLayout->addRow("Differential (D):", m_colorButtonDifferential);

    // Virtual Full Backup Color
    m_colorButtonVirtualFull = new QPushButton();
    m_colorButtonVirtualFull->setMinimumSize(80, 30);
    m_colorButtonVirtualFull->setCursor(Qt::PointingHandCursor);
    connect(m_colorButtonVirtualFull, &QPushButton::clicked, this, &SettingsDialog::onChooseColorVirtualFull);
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

void SettingsDialog::createBehaviorPage()
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

    layout->addWidget(displayGroup);

    layout->addStretch();

    // Set content widget to scroll area
    scrollArea->setWidget(contentWidget);

    // Add scroll area to page
    QVBoxLayout *pageLayout = new QVBoxLayout(m_behaviorPage);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(scrollArea);

    m_contentStack->addWidget(m_behaviorPage);
}

void SettingsDialog::createAdvancedPage()
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

    // Logging
    QGroupBox *logGroup = new QGroupBox(tr("Logging"));
    logGroup->setObjectName("settingsGroup");
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);

    m_debugLoggingCheck = new QCheckBox(tr("Enable debug logging"));
    logLayout->addWidget(m_debugLoggingCheck);

    QHBoxLayout *logFileLayout = new QHBoxLayout();
    QLabel *logFileLabel = new QLabel(tr("Log File:"));
    m_logFileEdit = new QLineEdit();
    m_logFileEdit->setPlaceholderText("bacula-qt-ui.log");
    QPushButton *logBrowse = new QPushButton(tr("Browse..."));
    logFileLayout->addWidget(logFileLabel);
    logFileLayout->addWidget(m_logFileEdit, 1);
    logFileLayout->addWidget(logBrowse);
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

    layout->addWidget(logGroup);

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

void SettingsDialog::applyModernStyle()
{
    setStyleSheet(R"(
        /* Main Dialog */
        SettingsDialog {
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

void SettingsDialog::loadSettings()
{
    BSettings& settings = BSettings::instance();

    // Connection
    m_hostEdit->setText(settings.connectionHost());
    m_portSpin->setValue(settings.connectionPort());
    m_directorEdit->setText(settings.connectionDirector());
    m_consoleEdit->setText(settings.connectionConsole());
    m_passwordEdit->setText(settings.connectionPassword());
    m_savePasswordCheck->setChecked(settings.connectionSavePassword());
    m_autoConnectCheck->setChecked(settings.connectionAutoConnect());
    m_connectionTimeoutSpin->setValue(settings.connectionTimeout());

    // TLS
    m_tlsGroupBox->setChecked(settings.tlsEnabled());
    bool usePSK = settings.tlsUsePSK();
    if (usePSK) {
        m_tlsPSKRadio->setChecked(true);
    } else {
        m_tlsCertificateRadio->setChecked(true);
    }

#ifndef Q_OS_WINDOWS
    m_caCertEdit->setText(settings.tlsCaCertFile());
    m_clientCertEdit->setText(settings.tlsCertFile());
    m_clientKeyEdit->setText(settings.tlsKeyFile());
#else
    m_clientCertEdit->setText(settings.tlsPfxFile());
#endif

    m_verifyPeerCheck->setChecked(settings.tlsVerifyPeer());

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

    // Behavior
    m_confirmJobCancelCheck->setChecked(settings.behaviorConfirmJobCancel());
    m_confirmJobStartCheck->setChecked(settings.behaviorConfirmJobStart());
    m_autoRefreshCheck->setChecked(settings.behaviorAutoRefresh());
    m_refreshIntervalSpin->setValue(settings.behaviorRefreshInterval());
    m_maxJobsDisplaySpin->setValue(settings.behaviorMaxJobsDisplay());

    // Advanced
    m_debugLoggingCheck->setChecked(settings.advancedDebugLogging());
    m_logFileEdit->setText(settings.advancedLogFile());
    m_maxLogSizeSpin->setValue(settings.advancedMaxLogSize());
    m_enableTooltipsCheck->setChecked(settings.advancedEnableTooltips());
}

void SettingsDialog::saveSettings()
{
    BSettings& settings = BSettings::instance();

    // Connection
    settings.setConnectionHost(m_hostEdit->text());
    settings.setConnectionPort(m_portSpin->value());
    settings.setConnectionDirector(m_directorEdit->text());
    settings.setConnectionConsole(m_consoleEdit->text());
    settings.setConnectionSavePassword(m_savePasswordCheck->isChecked());
    settings.setConnectionPassword(m_passwordEdit->text());  // Will only save if savePassword is true
    settings.setConnectionAutoConnect(m_autoConnectCheck->isChecked());
    settings.setConnectionTimeout(m_connectionTimeoutSpin->value());

    // TLS
    settings.setTlsEnabled(m_tlsGroupBox->isChecked());
    settings.setTlsUsePSK(m_tlsPSKRadio->isChecked());
#ifndef Q_OS_WINDOWS
    settings.setTlsCaCertFile(m_caCertEdit->text());
    settings.setTlsCertFile(m_clientCertEdit->text());
    settings.setTlsKeyFile(m_clientKeyEdit->text());
#else
    settings.setTlsPfxFile(m_clientCertEdit->text());
#endif
    settings.setTlsVerifyPeer(m_verifyPeerCheck->isChecked());

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

    // Advanced
    settings.setAdvancedDebugLogging(m_debugLoggingCheck->isChecked());
    settings.setAdvancedLogFile(m_logFileEdit->text());
    settings.setAdvancedMaxLogSize(m_maxLogSizeSpin->value());
    settings.setAdvancedEnableTooltips(m_enableTooltipsCheck->isChecked());

    settings.sync();

    emit settingsChanged();
}

void SettingsDialog::onCategoryChanged(int index)
{
    m_contentStack->setCurrentIndex(index);
}

void SettingsDialog::onApplyClicked()
{
    saveSettings();
    QMessageBox::information(this, tr("Settings"),
        tr("Settings have been saved.\n"
        "Some changes require an application restart."));
    accept();
}

void SettingsDialog::onCancelClicked()
{
    reject();
}

void SettingsDialog::onResetToDefaultsClicked()
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

void SettingsDialog::onBrowseCACert()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select CA Certificate"),
        QString(), tr("Certificates (*.pem *.crt *.cert);;All Files (*)"));
    if (!file.isEmpty()) {
        m_caCertEdit->setText(file);
    }
}

void SettingsDialog::onBrowseClientCert()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select Client Certificate"),
#ifdef Q_OS_WINDOWS
    QString(), tr("Certificates (*.pfx);;All Files (*)"));
#else
    QString(), tr("Certificates (*.pem *.crt *.cert);;All Files (*)"));
#endif
    if (!file.isEmpty()) {
        m_clientCertEdit->setText(file);
    }
}

void SettingsDialog::onBrowseClientKey()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select Private Key"),
        QString(), tr("Keys (*.pem *.key);;All Files (*)"));
    if (!file.isEmpty()) {
        m_clientKeyEdit->setText(file);
    }
}

void SettingsDialog::onExportSettings()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Export Settings"),
        "bacula-settings.json", "JSON (*.json)");

    if (!file.isEmpty()) {
        QSettings settings("Bacula", "Onesimus");
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

void SettingsDialog::onImportSettings()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Import Settings"),
        QString(), "JSON (*.json)");

    if (!file.isEmpty()) {
        QFile inFile(file);
        if (inFile.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(inFile.readAll());
            QJsonObject json = doc.object();

            QSettings settings("Bacula", "Onesimus");
            for (auto it = json.begin(); it != json.end(); ++it) {
                settings.setValue(it.key(), it.value().toString());
            }

            loadSettings();
            QMessageBox::information(this, tr("Import"), tr("Settings imported successfully."));
        }
    }
}

void SettingsDialog::onClearStoredConnections()
{
    int ret = QMessageBox::warning(this, tr("Clear Connections"),
        tr("Do you really want to delete all stored connection information?\n"
        "This includes passwords and certificate paths."),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        BSettings& settings = BSettings::instance();

        // Clear all connection settings
        settings.setConnectionHost("");
        settings.setConnectionPort(9101);
        settings.setConnectionDirector("bareos-dir");
        settings.setConnectionConsole("onesimus");
        settings.setConnectionPassword("");
        settings.setConnectionSavePassword(true);
        settings.setConnectionAutoConnect(false);
        settings.setConnectionTimeout(30);
        settings.setTlsEnabled(true);
        settings.setTlsUsePSK(true);
        settings.setTlsCaCertFile("");
        settings.setTlsCertFile("");
        settings.setTlsKeyFile("");
        settings.setTlsPfxFile("");
        settings.setTlsVerifyPeer(true);
        settings.sync();

        loadSettings();
        QMessageBox::information(this, tr("Deleted"),
            tr("All stored connection information has been deleted."));
    }
}

void SettingsDialog::onChooseColorFull()
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

void SettingsDialog::onChooseColorIncremental()
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

void SettingsDialog::onChooseColorDifferential()
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

void SettingsDialog::onChooseColorVirtualFull()
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
