#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "bsettings.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

SettingsDialog::SettingsDialog(BDirector *director, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , m_director(director)
    , m_categoryList(nullptr)
    , m_contentStack(nullptr)
{
    ui->setupUi(this);
    
    setWindowTitle("Einstellungen");
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
    // Haupt-Layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Sidebar erstellen
    createSidebar();
    
    // Content-Bereich
    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 30, 30, 20);
    contentLayout->setSpacing(20);
    
    // Content Stack
    m_contentStack = new QStackedWidget(this);
    createContentPages();
    contentLayout->addWidget(m_contentStack, 1);
    
    // Button-Leiste
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    
    m_resetButton = new QPushButton("Zurücksetzen", this);
    m_resetButton->setObjectName("resetButton");
    m_cancelButton = new QPushButton("Abbrechen", this);
    m_cancelButton->setObjectName("cancelButton");
    m_applyButton = new QPushButton("Übernehmen", this);
    m_applyButton->setObjectName("applyButton");
    m_applyButton->setDefault(true);
    
    buttonLayout->addWidget(m_resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_applyButton);
    
    contentLayout->addLayout(buttonLayout);
    
    mainLayout->addWidget(m_categoryList);
    mainLayout->addWidget(contentWidget, 1);
    
    // Signals verbinden
    connect(m_categoryList, &QListWidget::currentRowChanged, this, &SettingsDialog::onCategoryChanged);
    connect(m_applyButton, &QPushButton::clicked, this, &SettingsDialog::onApplyClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &SettingsDialog::onCancelClicked);
    connect(m_resetButton, &QPushButton::clicked, this, &SettingsDialog::onResetToDefaultsClicked);
    
    // Erste Kategorie auswählen
    m_categoryList->setCurrentRow(0);
}

void SettingsDialog::createSidebar()
{
    m_categoryList = new QListWidget(this);
    m_categoryList->setObjectName("categoryList");
    m_categoryList->setFixedWidth(220);
    m_categoryList->setSpacing(2);
    m_categoryList->setFrameShape(QFrame::NoFrame);
    
    // Kategorien hinzufügen
    QListWidgetItem *connectionItem = new QListWidgetItem("🔌 Verbindung");
    connectionItem->setData(Qt::UserRole, "connection");
    m_categoryList->addItem(connectionItem);

    QListWidgetItem *appearanceItem = new QListWidgetItem("🎨 Erscheinungsbild");
    appearanceItem->setData(Qt::UserRole, "appearance");
    m_categoryList->addItem(appearanceItem);

    QListWidgetItem *behaviorItem = new QListWidgetItem("⚙️ Verhalten");
    behaviorItem->setData(Qt::UserRole, "behavior");
    m_categoryList->addItem(behaviorItem);

    QListWidgetItem *advancedItem = new QListWidgetItem("🔧 Erweitert");
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

    // Titel
    QLabel *titleLabel = new QLabel("Verbindungseinstellungen");
    titleLabel->setObjectName("pageTitle");
    layout->addWidget(titleLabel);
    
    // Backup-System Auswahl
    QGroupBox *systemGroup = new QGroupBox("Backup-System");
    systemGroup->setObjectName("settingsGroup");
    QVBoxLayout *systemLayout = new QVBoxLayout(systemGroup);
    systemLayout->setSpacing(12);
    
    QLabel *systemInfoLabel = new QLabel(
        "ℹ️ Bareos ist ein Fork von Bacula mit zusätzlichen Features.\n"
        "Beide Systeme verwenden kompatible Protokolle.");
    systemInfoLabel->setObjectName("infoLabel");
    systemInfoLabel->setWordWrap(true);
    systemLayout->addWidget(systemInfoLabel);
    
    layout->addWidget(systemGroup);
    
    // Bconsole-Einstellungen
    QGroupBox *bconsoleGroup = new QGroupBox("Director-Verbindung (Bconsole)");
    bconsoleGroup->setObjectName("settingsGroup");
    QFormLayout *bconsoleLayout = new QFormLayout(bconsoleGroup);
    bconsoleLayout->setSpacing(12);
    
    m_hostEdit = new QLineEdit();
    m_hostEdit->setPlaceholderText("z.B. 192.168.1.100 oder bacula-dir.local");
    bconsoleLayout->addRow("Host:", m_hostEdit);
    
    m_portSpin = new QSpinBox();
    m_portSpin->setRange(1, 65535);
    m_portSpin->setValue(9101);
    bconsoleLayout->addRow("Port:", m_portSpin);
    
    m_directorEdit = new QLineEdit();
    m_directorEdit->setPlaceholderText("bacula-dir");
    bconsoleLayout->addRow("Director Name:", m_directorEdit);
    
    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("••••••••");
    bconsoleLayout->addRow("Passwort:", m_passwordEdit);
    
    layout->addWidget(bconsoleGroup);

    // TLS Info-Label (angezeigt wenn TLS deaktiviert ist)
    QLabel *tlsWarningLabel = new QLabel(
        "⚠️ TLS verschlüsselt die Kommunikation mit dem Director.\n"
        "Für Produktionsumgebungen wird TLS dringend empfohlen!");
    tlsWarningLabel->setObjectName("warningLabel");
    tlsWarningLabel->setWordWrap(true);
    layout->addWidget(tlsWarningLabel);

    // TLS/SSL-Einstellungen (checkable GroupBox)
    m_tlsGroupBox = new QGroupBox("TLS/SSL-Verschlüsselung");
    m_tlsGroupBox->setObjectName("settingsGroup");
    m_tlsGroupBox->setCheckable(true);
    m_tlsGroupBox->setChecked(true);  // Initial: TLS aktiviert
    QVBoxLayout *tlsLayout = new QVBoxLayout(m_tlsGroupBox);
    tlsLayout->setSpacing(12);

    // Verbinde Signal um Warnung anzuzeigen/verstecken
    tlsWarningLabel->setVisible(!m_tlsGroupBox->isChecked());
    connect(m_tlsGroupBox, &QGroupBox::toggled, [tlsWarningLabel](bool checked) {
        tlsWarningLabel->setVisible(!checked);
    });

    // Authentifizierungsmethode
    QLabel *authMethodLabel = new QLabel("Authentifizierungsmethode:");
    authMethodLabel->setStyleSheet("font-weight: bold;");
    tlsLayout->addWidget(authMethodLabel);

    m_tlsPSKRadio = new QRadioButton("PSK (Pre-Shared Key) - Standard für Bareos 18.2+");
    m_tlsPSKRadio->setChecked(true);
    tlsLayout->addWidget(m_tlsPSKRadio);

    m_tlsCertificateRadio = new QRadioButton("Zertifikat-basierte TLS-Authentifizierung");
    tlsLayout->addWidget(m_tlsCertificateRadio);

    // Zertifikat-Einstellungen (nur für Certificate-Modus)
    QWidget *certWidget = new QWidget();
    QFormLayout *certLayout = new QFormLayout(certWidget);
    certLayout->setSpacing(12);

#ifndef Q_OS_WINDOWS
    // Linux: Separate PEM-Dateien
    // CA Certificate
    QHBoxLayout *caLayout = new QHBoxLayout();
    m_caCertEdit = new QLineEdit();
    m_caCertEdit->setPlaceholderText("Pfad zum CA-Zertifikat (.pem)");
    QPushButton *caBrowse = new QPushButton("Durchsuchen...");
    caBrowse->setObjectName("browseButton");
    connect(caBrowse, &QPushButton::clicked, this, &SettingsDialog::onBrowseCACert);
    caLayout->addWidget(m_caCertEdit);
    caLayout->addWidget(caBrowse);
    certLayout->addRow("CA Certificate:", caLayout);

    // Client Certificate
    QHBoxLayout *clientCertLayout = new QHBoxLayout();
    m_clientCertEdit = new QLineEdit();
    m_clientCertEdit->setPlaceholderText("Pfad zum Client-Zertifikat (.pem)");
    QPushButton *clientCertBrowse = new QPushButton("Durchsuchen...");
    clientCertBrowse->setObjectName("browseButton");
    connect(clientCertBrowse, &QPushButton::clicked, this, &SettingsDialog::onBrowseClientCert);
    clientCertLayout->addWidget(m_clientCertEdit);
    clientCertLayout->addWidget(clientCertBrowse);
    certLayout->addRow("Client Certificate:", clientCertLayout);

    // Private Key
    QHBoxLayout *keyLayout = new QHBoxLayout();
    m_clientKeyEdit = new QLineEdit();
    m_clientKeyEdit->setPlaceholderText("Pfad zum Private Key (.pem, .key)");
    QPushButton *keyBrowse = new QPushButton("Durchsuchen...");
    keyBrowse->setObjectName("browseButton");
    connect(keyBrowse, &QPushButton::clicked, this, &SettingsDialog::onBrowseClientKey);
    keyLayout->addWidget(m_clientKeyEdit);
    keyLayout->addWidget(keyBrowse);
    certLayout->addRow("Private Key:", keyLayout);
#else
    // Windows: PFX-Datei
    QHBoxLayout *clientCertLayout = new QHBoxLayout();
    m_clientCertEdit = new QLineEdit();
    m_clientCertEdit->setPlaceholderText("Pfad zum Client-Zertifikat (.pfx)");
    QPushButton *clientCertBrowse = new QPushButton("Durchsuchen...");
    clientCertBrowse->setObjectName("browseButton");
    connect(clientCertBrowse, &QPushButton::clicked, this, &SettingsDialog::onBrowseClientCert);
    clientCertLayout->addWidget(m_clientCertEdit);
    clientCertLayout->addWidget(clientCertBrowse);
    certLayout->addRow("PFX Certificate:", clientCertLayout);
#endif

    // Peer Verification
    m_verifyPeerCheck = new QCheckBox("Server-Zertifikat validieren (empfohlen)");
    m_verifyPeerCheck->setChecked(true);
    certLayout->addRow("", m_verifyPeerCheck);

    certWidget->setEnabled(false);  // Standardmäßig deaktiviert (PSK ist ausgewählt)
    tlsLayout->addWidget(certWidget);

    // Certificate-Felder nur aktivieren wenn Certificate-Radio ausgewählt
    connect(m_tlsCertificateRadio, &QRadioButton::toggled, certWidget, &QWidget::setEnabled);

    layout->addWidget(m_tlsGroupBox);

    // Verbindungsoptionen
    QGroupBox *optionsGroup = new QGroupBox("Verbindungsoptionen");
    optionsGroup->setObjectName("settingsGroup");
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);
    optionsLayout->setSpacing(12);

    m_savePasswordCheck = new QCheckBox("Passwort speichern");
    m_savePasswordCheck->setChecked(true);
    optionsLayout->addWidget(m_savePasswordCheck);

    m_autoConnectCheck = new QCheckBox("Automatisch beim Start verbinden");
    optionsLayout->addWidget(m_autoConnectCheck);

    QHBoxLayout *timeoutLayout = new QHBoxLayout();
    QLabel *timeoutLabel = new QLabel("Verbindungs-Timeout:");
    m_connectionTimeoutSpin = new QSpinBox();
    m_connectionTimeoutSpin->setRange(5, 120);
    m_connectionTimeoutSpin->setValue(30);
    m_connectionTimeoutSpin->setSuffix(" Sekunden");
    timeoutLayout->addWidget(timeoutLabel);
    timeoutLayout->addWidget(m_connectionTimeoutSpin);
    timeoutLayout->addStretch();
    optionsLayout->addLayout(timeoutLayout);

    layout->addWidget(optionsGroup);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *exportBtn = new QPushButton("Einstellungen exportieren");
    QPushButton *importBtn = new QPushButton("Einstellungen importieren");
    QPushButton *clearBtn = new QPushButton("Gespeicherte Verbindungen löschen");
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

    // Titel
    QLabel *titleLabel = new QLabel("Erscheinungsbild");
    titleLabel->setObjectName("pageTitle");
    layout->addWidget(titleLabel);
    
    // Theme
    QGroupBox *themeGroup = new QGroupBox("Farbschema");
    themeGroup->setObjectName("settingsGroup");
    QFormLayout *themeLayout = new QFormLayout(themeGroup);
    
    m_themeCombo = new QComboBox();
    m_themeCombo->addItem("🌙 Dunkel (Industrial)", "dark");
    m_themeCombo->addItem("☀️ Hell", "light");
    m_themeCombo->addItem("🖥️ System", "system");
    themeLayout->addRow("Theme:", m_themeCombo);
    
    layout->addWidget(themeGroup);
    
    // Schrift
    QGroupBox *fontGroup = new QGroupBox("Schriftgröße");
    fontGroup->setObjectName("settingsGroup");
    QFormLayout *fontLayout = new QFormLayout(fontGroup);
    
    m_fontSizeSpin = new QSpinBox();
    m_fontSizeSpin->setRange(8, 16);
    m_fontSizeSpin->setValue(10);
    m_fontSizeSpin->setSuffix(" pt");
    fontLayout->addRow("Basis-Schriftgröße:", m_fontSizeSpin);
    
    layout->addWidget(fontGroup);
    
    // UI-Optionen
    QGroupBox *uiGroup = new QGroupBox("Benutzeroberfläche");
    uiGroup->setObjectName("settingsGroup");
    QVBoxLayout *uiLayout = new QVBoxLayout(uiGroup);
    
    m_animationsCheck = new QCheckBox("Animationen aktivieren");
    m_animationsCheck->setChecked(true);
    uiLayout->addWidget(m_animationsCheck);
    
    m_compactModeCheck = new QCheckBox("Kompakter Modus");
    uiLayout->addWidget(m_compactModeCheck);

    layout->addWidget(uiGroup);

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

    // Titel
    QLabel *titleLabel = new QLabel("Verhalten");
    titleLabel->setObjectName("pageTitle");
    layout->addWidget(titleLabel);
    
    // Bestätigungen
    QGroupBox *confirmGroup = new QGroupBox("Bestätigungen");
    confirmGroup->setObjectName("settingsGroup");
    QVBoxLayout *confirmLayout = new QVBoxLayout(confirmGroup);
    
    m_confirmJobCancelCheck = new QCheckBox("Job-Abbruch bestätigen");
    m_confirmJobCancelCheck->setChecked(true);
    confirmLayout->addWidget(m_confirmJobCancelCheck);
    
    layout->addWidget(confirmGroup);
    
    // Auto-Refresh
    QGroupBox *refreshGroup = new QGroupBox("Automatische Aktualisierung");
    refreshGroup->setObjectName("settingsGroup");
    QVBoxLayout *refreshLayout = new QVBoxLayout(refreshGroup);
    
    m_autoRefreshCheck = new QCheckBox("Automatisch aktualisieren");
    refreshLayout->addWidget(m_autoRefreshCheck);
    
    QHBoxLayout *intervalLayout = new QHBoxLayout();
    QLabel *intervalLabel = new QLabel("Intervall:");
    m_refreshIntervalSpin = new QSpinBox();
    m_refreshIntervalSpin->setRange(10, 300);
    m_refreshIntervalSpin->setValue(30);
    m_refreshIntervalSpin->setSuffix(" Sekunden");
    intervalLayout->addWidget(intervalLabel);
    intervalLayout->addWidget(m_refreshIntervalSpin);
    intervalLayout->addStretch();
    refreshLayout->addLayout(intervalLayout);
    
    layout->addWidget(refreshGroup);
    
    // Anzeige
    QGroupBox *displayGroup = new QGroupBox("Anzeige");
    displayGroup->setObjectName("settingsGroup");
    QFormLayout *displayLayout = new QFormLayout(displayGroup);
    
    m_maxJobsDisplaySpin = new QSpinBox();
    m_maxJobsDisplaySpin->setRange(10, 1000);
    m_maxJobsDisplaySpin->setValue(100);
    m_maxJobsDisplaySpin->setSuffix(" Jobs");
    displayLayout->addRow("Maximale Jobs:", m_maxJobsDisplaySpin);

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

    // Titel
    QLabel *titleLabel = new QLabel("Erweiterte Einstellungen");
    titleLabel->setObjectName("pageTitle");
    layout->addWidget(titleLabel);
    
    // Logging
    QGroupBox *logGroup = new QGroupBox("Protokollierung");
    logGroup->setObjectName("settingsGroup");
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);
    
    m_debugLoggingCheck = new QCheckBox("Debug-Logging aktivieren");
    logLayout->addWidget(m_debugLoggingCheck);
    
    QHBoxLayout *logFileLayout = new QHBoxLayout();
    QLabel *logFileLabel = new QLabel("Log-Datei:");
    m_logFileEdit = new QLineEdit();
    m_logFileEdit->setPlaceholderText("bacula-qt-ui.log");
    QPushButton *logBrowse = new QPushButton("Durchsuchen...");
    logFileLayout->addWidget(logFileLabel);
    logFileLayout->addWidget(m_logFileEdit, 1);
    logFileLayout->addWidget(logBrowse);
    logLayout->addLayout(logFileLayout);
    
    QHBoxLayout *maxSizeLayout = new QHBoxLayout();
    QLabel *maxSizeLabel = new QLabel("Max. Log-Größe:");
    m_maxLogSizeSpin = new QSpinBox();
    m_maxLogSizeSpin->setRange(1, 100);
    m_maxLogSizeSpin->setValue(10);
    m_maxLogSizeSpin->setSuffix(" MB");
    maxSizeLayout->addWidget(maxSizeLabel);
    maxSizeLayout->addWidget(m_maxLogSizeSpin);
    maxSizeLayout->addStretch();
    logLayout->addLayout(maxSizeLayout);
    
    layout->addWidget(logGroup);
    
    // Weitere Optionen
    QGroupBox *miscGroup = new QGroupBox("Sonstiges");
    miscGroup->setObjectName("settingsGroup");
    QVBoxLayout *miscLayout = new QVBoxLayout(miscGroup);
    
    m_enableTooltipsCheck = new QCheckBox("Tooltips anzeigen");
    m_enableTooltipsCheck->setChecked(true);
    miscLayout->addWidget(m_enableTooltipsCheck);
    
    layout->addWidget(miscGroup);
    
    // Warnung
    QLabel *warningLabel = new QLabel(
        "⚠️ Diese Einstellungen sind für fortgeschrittene Benutzer.\n"
        "Änderungen können die Stabilität der Anwendung beeinflussen.");
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
        /* Haupt-Dialog */
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

    // Verbindung
    m_hostEdit->setText(settings.connectionHost());
    m_portSpin->setValue(settings.connectionPort());
    m_directorEdit->setText(settings.connectionDirector());
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

    // Behavior
    m_confirmJobCancelCheck->setChecked(settings.behaviorConfirmJobCancel());
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

    // Verbindung
    settings.setConnectionHost(m_hostEdit->text());
    settings.setConnectionPort(m_portSpin->value());
    settings.setConnectionDirector(m_directorEdit->text());
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

    // Behavior
    settings.setBehaviorConfirmJobCancel(m_confirmJobCancelCheck->isChecked());
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
    QMessageBox::information(this, "Einstellungen", 
        "Die Einstellungen wurden gespeichert.\n"
        "Einige Änderungen erfordern einen Neustart der Anwendung.");
    accept();
}

void SettingsDialog::onCancelClicked()
{
    reject();
}

void SettingsDialog::onResetToDefaultsClicked()
{
    int ret = QMessageBox::question(this, "Zurücksetzen",
        "Möchten Sie wirklich alle Einstellungen auf die Standardwerte zurücksetzen?\n"
        "Diese Aktion kann nicht rückgängig gemacht werden.",
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        BSettings::instance().resetToDefaults();
        loadSettings();
        QMessageBox::information(this, "Zurückgesetzt",
            "Alle Einstellungen wurden auf die Standardwerte zurückgesetzt.");
    }
}

void SettingsDialog::onBrowseCACert()
{
    QString file = QFileDialog::getOpenFileName(this, "CA-Zertifikat wählen",
        QString(), "Zertifikate (*.pem *.crt *.cert);;Alle Dateien (*)");
    if (!file.isEmpty()) {
        m_caCertEdit->setText(file);
    }
}

void SettingsDialog::onBrowseClientCert()
{
    QString file = QFileDialog::getOpenFileName(this, "Client-Zertifikat wählen",
#ifdef Q_OS_WINDOWS
    QString(), "Zertifikate (*.pfx);;Alle Dateien (*)");
#else
    QString(), "Zertifikate (*.pem *.crt *.cert);;Alle Dateien (*)");
#endif
    if (!file.isEmpty()) {
        m_clientCertEdit->setText(file);
    }
}

void SettingsDialog::onBrowseClientKey()
{
    QString file = QFileDialog::getOpenFileName(this, "Private Key wählen",
        QString(), "Keys (*.pem *.key);;Alle Dateien (*)");
    if (!file.isEmpty()) {
        m_clientKeyEdit->setText(file);
    }
}

void SettingsDialog::onExportSettings()
{
    QString file = QFileDialog::getSaveFileName(this, "Einstellungen exportieren",
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
            QMessageBox::information(this, "Export", "Einstellungen erfolgreich exportiert.");
        }
    }
}

void SettingsDialog::onImportSettings()
{
    QString file = QFileDialog::getOpenFileName(this, "Einstellungen importieren",
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
            QMessageBox::information(this, "Import", "Einstellungen erfolgreich importiert.");
        }
    }
}

void SettingsDialog::onClearStoredConnections()
{
    int ret = QMessageBox::warning(this, "Verbindungen löschen",
        "Möchten Sie wirklich alle gespeicherten Verbindungsinformationen löschen?\n"
        "Dies beinhaltet Passwörter und Zertifikatspfade.",
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        BSettings& settings = BSettings::instance();

        // Clear all connection settings
        settings.setConnectionHost("");
        settings.setConnectionPort(9101);
        settings.setConnectionDirector("bareos-dir");
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
        QMessageBox::information(this, "Gelöscht",
            "Alle gespeicherten Verbindungsinformationen wurden gelöscht.");
    }
}
