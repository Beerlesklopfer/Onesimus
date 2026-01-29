#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jobs/bjobwidget.h"
#include "jobs/bjobsstatisticswidget.h"
#include "clients/bclientswidget.h"
#include "storagewidget.h"
#include "schedules/bschedulewidget.h"
#include "settingsdialog.h"
#include "bsettings.h"

#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include <QSpinBox>
#include <QTabWidget>
#include <QTextStream>
#include <QToolBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_director(nullptr)
    , m_statusLabel(nullptr)
    , m_connectionLabel(nullptr)
    , m_autoRefreshTimer(nullptr)
{
    ui->setupUi(this);
    
#if USE_BACULA
    setWindowTitle("Onesimus - Bacula Backup Management");
#elif defined(USE_BAREOS)
    setWindowTitle("Onesimus - Bareos Backup Management");
#endif
    resize(1200, 800);

    m_director = new BDirector(this);

    setupUI();
    createActions();
    createMenus();
    createToolBar();

    // ✅ Synchronisiere DockWidget-Sichtbarkeit mit Action und speichere in Settings
    connect(m_statisticsDock, &QDockWidget::visibilityChanged, this, [this](bool visible) {
        m_toggleStatisticsAction->setChecked(visible);
        // Save statistics widget visibility state
        BSettings::instance().setStatisticsWidgetVisible(visible);
    });

    connect(m_director, &BDirector::disconnected, this, [this]() {
        onAuthentificationSucceeded(false, "");
    });

    // ✅ Verbinde statusMessage Signal
    connect(m_director, &BDirector::statusMessage, this, [this](const QString &msg) {
        m_statusLabel->setText(msg);
    });

    connect(m_director, &BDirector::authentificationSucceeded,  this, &MainWindow::onAuthentificationSucceeded);

    connect(m_director, &BDirector::protocolError, this, &MainWindow::onConnectionError);

    // ✅ Route JSON responses to appropriate widgets based on command
    connect(m_director, &BDirector::jsonResponse, this, [this](const QString &command, const QString &jsonData) {
#ifdef DEBUG_JSON
        qDebug() << "========================================";
        qDebug() << "MainWindow: JSON RESPONSE FOR COMMAND:" << command;
        qDebug() << "  Data size:" << jsonData.size() << "bytes";
        qDebug() << "========================================";
#endif

        // Route to appropriate widget based on command
        if (command.contains("list jobs") || command.contains("list jobid")) {
#ifdef IS_DEVELOPER
            qDebug() << "→ Routing jobs data to JobWidget";
#endif
            m_jobWidget->processJsonResponse(jsonData);
        } else if (command.contains("list joblog")) {
#ifdef IS_DEVELOPER
            qDebug() << "→ Job log response (handled by job details dialog)";
#endif
            // Job log responses are handled directly by BJobDetailsDialog
            // No routing needed here
        } else if (command.contains("list clients")) {
#ifdef IS_DEVELOPER
            qDebug() << "→ Routing clients data to ClientWidget";
#endif
            m_clientWidget->processJsonResponse(jsonData);
        } else if (command.contains("list volumes") || command.contains("list media")) {
#ifdef IS_DEVELOPER
            qDebug() << "→ Routing volumes data to StorageWidget";
#endif
            m_storageWidget->processJsonResponse(jsonData);
        } else if (command == ".jobs") {
#ifdef IS_DEVELOPER
            qDebug() << "→ Routing .jobs dot-command response to JobWidget";
#endif
            m_jobWidget->processDotJobsResponse(jsonData);
        } else if (command == ".clients") {
#ifdef IS_DEVELOPER
            qDebug() << "→ Routing .clients dot-command response to JobWidget";
#endif
            m_jobWidget->processDotClientsResponse(jsonData);
        } else if (command == ".levels") {
#ifdef IS_DEVELOPER
            qDebug() << "→ Routing .levels dot-command response to JobWidget";
#endif
            m_jobWidget->processDotLevelsResponse(jsonData);
        } else if (command == ".schedule") {
#ifdef IS_DEVELOPER
            qDebug() << "→ Routing .schedule dot-command response to ScheduleWidget";
#endif
            m_scheduleWidget->processDotScheduleResponse(jsonData);
        } else {
#ifdef IS_DEVELOPER
            qDebug() << "⚠ Unhandled command response:" << command;
#endif
        }
    });

    // Connect Jobwidget signals
    connect(m_jobWidget, &BJobWidget::sendCommand, this, &MainWindow::onSendCommand);

    QObject::connect(m_jobWidget, &BJobWidget::statusMessageChanged,
            m_statusLabel, &QLabel::setText);

    // Connect ClientWidget signals
    connect(m_clientWidget, &BClientsWidget::sendCommand, this, &MainWindow::onSendCommand);

    connect(m_clientWidget, &BClientsWidget::statusMessageChanged,
            m_statusLabel, &QLabel::setText);

    // Connect StorageWidget signals
    connect(m_storageWidget, &StorageWidget::sendCommand, this, &MainWindow::onSendCommand);

    connect(m_storageWidget, &StorageWidget::statusMessageChanged,
            m_statusLabel, &QLabel::setText);

    // Connect ScheduleWidget signals
    connect(m_scheduleWidget, &BScheduleWidget::sendCommand, this, &MainWindow::onSendCommand);

    connect(m_scheduleWidget, &BScheduleWidget::statusMessageChanged,
            m_statusLabel, &QLabel::setText);

    // Setup auto-refresh timer
    m_autoRefreshTimer = new QTimer(this);
    connect(m_autoRefreshTimer, &QTimer::timeout, this, &MainWindow::onRefreshAll);

    // Connect to BSettings signals for auto-refresh
    connect(&BSettings::instance(), &BSettings::autoRefreshSettingsChanged,
            this, &MainWindow::onAutoRefreshSettingsChanged);

    // Initialize auto-refresh from settings
    BSettings& settings = BSettings::instance();
    onAutoRefreshSettingsChanged(settings.behaviorAutoRefresh(), settings.behaviorRefreshInterval());

    onAuthentificationSucceeded(false, tr("Nicht verbunden"));
    loadAndConnectLastUsed();
}

MainWindow::~MainWindow()
{
    m_director->saveConnectionSettings();
    delete ui;
}

void MainWindow::setupUI()
{
    // Zentrales Widget mit Tabs
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    m_jobWidget = new BJobWidget(this);
    m_jobWidget->setDirector(m_director);
    m_tabWidget->addTab(m_jobWidget, "Jobs");

    // Client-Widget
    m_clientWidget = new BClientsWidget(this);
    m_clientWidget->setDirector(m_director);
    m_tabWidget->addTab(m_clientWidget, "Clients");
    
    // Storage-Widget
    m_storageWidget = new StorageWidget(m_director, this);
    m_tabWidget->addTab(m_storageWidget, "Storage/Volumes");

    // Schedule-Widget
    m_scheduleWidget = new BScheduleWidget(this);
    m_scheduleWidget->setDirector(m_director);
    m_tabWidget->addTab(m_scheduleWidget, "Schedules");

    m_tabWidget->setEnabled(false);

    // ========================================================================
    // Statistics Dock Widget (rechte Seite)
    // ========================================================================
    m_statisticsWidget = new BJobsStatisticsWidget(this);
    m_statisticsWidget->setModel(m_jobWidget->tableView()->jobsModel());

    m_statisticsDock = new QDockWidget(tr("Statistiken"), this);
    m_statisticsDock->setWidget(m_statisticsWidget);
    m_statisticsDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);

    // ✅ Deaktiviere Features für feste Breite
    m_statisticsDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);

    addDockWidget(Qt::RightDockWidgetArea, m_statisticsDock);

    // Setze feste Breite für das DockWidget
    m_statisticsDock->setMinimumWidth(300);
    m_statisticsDock->setMaximumWidth(300);

    // Lade gespeicherten Sichtbarkeitszustand (oder standardmäßig versteckt)
    bool statsVisible = BSettings::instance().statisticsWidgetVisible();
    m_statisticsDock->setVisible(statsVisible);
    m_toggleStatisticsAction->setChecked(statsVisible);

    // Statusleiste
    m_statusLabel = new QLabel("Bereit", this);
    statusBar()->addWidget(m_statusLabel);

    m_connectionLabel = new QLabel("Nicht verbunden", this);
    m_connectionLabel->setStyleSheet("color: red; font-weight: bold;");
    statusBar()->addPermanentWidget(m_connectionLabel);
}

void MainWindow::createActions()
{
    m_connectAction = new QAction("Verbinden", this);
    m_connectAction->setIcon(QIcon::fromTheme("network-connect"));
    m_connectAction->setShortcut(QKeySequence("Ctrl+O"));
    connect(m_connectAction, &QAction::triggered, this, &MainWindow::onConnectTriggered);
    
    m_connectLastAction = new QAction("Reconnect", this);
    m_connectLastAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_connectLastAction->setShortcut(QKeySequence("Ctrl+R"));
    m_connectLastAction->setEnabled(m_director->hasStoredConnection());
    connect(m_connectLastAction, &QAction::triggered, this, &MainWindow::onConnectLastUsed);
    
    m_disconnectAction = new QAction("Trennen", this);
    m_disconnectAction->setIcon(QIcon::fromTheme("network-disconnect"));
    m_disconnectAction->setEnabled(false);
    connect(m_disconnectAction, &QAction::triggered, this, &MainWindow::onDisconnectTriggered);
    
    m_refreshAction = new QAction("Aktualisieren", this);
    m_refreshAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshAction->setShortcut(QKeySequence("F5"));
    m_refreshAction->setEnabled(false);
    connect(m_refreshAction, &QAction::triggered, this, &MainWindow::onRefreshAll);

    m_settingsAction = new QAction("Einstellungen", this);
    m_settingsAction->setIcon(QIcon::fromTheme("preferences-system"));
    m_settingsAction->setShortcut(QKeySequence("Ctrl+,"));
    connect(m_settingsAction, &QAction::triggered, this, &MainWindow::onSettingsTriggered);
    
    m_exitAction = new QAction("Beenden", this);
    m_exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    
    m_aboutAction = new QAction("Über", this);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAboutTriggered);

    // Bearbeiten Actions
    m_copyAction = new QAction("Kopieren", this);
    m_copyAction->setIcon(QIcon::fromTheme("edit-copy"));
    m_copyAction->setShortcut(QKeySequence::Copy);
    m_copyAction->setEnabled(false);
    connect(m_copyAction, &QAction::triggered, this, &MainWindow::onCopyTriggered);

    m_selectAllAction = new QAction("Alles auswählen", this);
    m_selectAllAction->setIcon(QIcon::fromTheme("edit-select-all"));
    m_selectAllAction->setShortcut(QKeySequence::SelectAll);
    m_selectAllAction->setEnabled(false);
    connect(m_selectAllAction, &QAction::triggered, this, &MainWindow::onSelectAllTriggered);

    m_clearSelectionAction = new QAction("Auswahl aufheben", this);
    m_clearSelectionAction->setIcon(QIcon::fromTheme("edit-clear"));
    m_clearSelectionAction->setEnabled(false);
    connect(m_clearSelectionAction, &QAction::triggered, this, &MainWindow::onClearSelectionTriggered);

    m_findAction = new QAction("Suchen...", this);
    m_findAction->setIcon(QIcon::fromTheme("edit-find"));
    m_findAction->setShortcut(QKeySequence::Find);
    m_findAction->setEnabled(false);
    connect(m_findAction, &QAction::triggered, this, &MainWindow::onFindTriggered);

    // Ansicht Actions
    m_toggleStatisticsAction = new QAction("Statistiken anzeigen", this);
    m_toggleStatisticsAction->setCheckable(true);
    m_toggleStatisticsAction->setChecked(false);  // Standardmäßig versteckt (bis Verbindung)
    m_toggleStatisticsAction->setEnabled(false);  // Nur bei Verbindung aktiv
    m_toggleStatisticsAction->setIcon(QIcon::fromTheme("view-statistics"));
    m_toggleStatisticsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(m_toggleStatisticsAction, &QAction::toggled, this, [this](bool checked) {
        m_statisticsDock->setVisible(checked);
    });

    // Jobs Actions
    m_runJobAction = new QAction("Job ausführen", this);
    m_runJobAction->setIcon(QIcon::fromTheme("media-playback-start"));
    m_runJobAction->setEnabled(false);
    connect(m_runJobAction, &QAction::triggered, m_jobWidget, &BJobWidget::triggerRunJob);

    m_cancelJobAction = new QAction("Job abbrechen", this);
    m_cancelJobAction->setIcon(QIcon::fromTheme("process-stop"));
    m_cancelJobAction->setEnabled(false);
    connect(m_cancelJobAction, &QAction::triggered, m_jobWidget, &BJobWidget::triggerCancelJob);

    m_jobDetailsAction = new QAction("Details anzeigen", this);
    m_jobDetailsAction->setIcon(QIcon::fromTheme("document-properties"));
    m_jobDetailsAction->setEnabled(false);
    connect(m_jobDetailsAction, &QAction::triggered, m_jobWidget, &BJobWidget::triggerShowDetails);

    m_refreshJobsAction = new QAction("Jobs aktualisieren", this);
    m_refreshJobsAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshJobsAction->setShortcut(QKeySequence("Ctrl+Shift+J"));
    m_refreshJobsAction->setEnabled(false);
    connect(m_refreshJobsAction, &QAction::triggered, m_jobWidget, &BJobWidget::triggerRefresh);

    m_exportJobsJsonAction = new QAction("Jobs als JSON exportieren...", this);
    m_exportJobsJsonAction->setIcon(QIcon::fromTheme("document-save"));
    m_exportJobsJsonAction->setEnabled(false);
    connect(m_exportJobsJsonAction, &QAction::triggered, this, &MainWindow::onExportSettingsTriggered);

    m_exportJobsCsvAction = new QAction("Jobs als CSV exportieren...", this);
    m_exportJobsCsvAction->setIcon(QIcon::fromTheme("text-csv"));
    m_exportJobsCsvAction->setEnabled(false);
    connect(m_exportJobsCsvAction, &QAction::triggered, this, &MainWindow::onImportSettingsTriggered);

    // Clients Actions
    m_refreshClientsAction = new QAction("Clients aktualisieren", this);
    m_refreshClientsAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshClientsAction->setShortcut(QKeySequence("Ctrl+Shift+C"));
    m_refreshClientsAction->setEnabled(false);
    connect(m_refreshClientsAction, &QAction::triggered, m_clientWidget, &BClientsWidget::triggerRefresh);

    m_clientDetailsAction = new QAction("Client-Details anzeigen", this);
    m_clientDetailsAction->setIcon(QIcon::fromTheme("document-properties"));
    m_clientDetailsAction->setEnabled(false);
    // TODO: Connect to client details dialog when implemented

    // Storage Actions
    m_refreshStorageAction = new QAction("Storage aktualisieren", this);
    m_refreshStorageAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshStorageAction->setShortcut(QKeySequence("Ctrl+Shift+V"));
    m_refreshStorageAction->setEnabled(false);
    connect(m_refreshStorageAction, &QAction::triggered, m_storageWidget, &StorageWidget::triggerRefresh);

    // Schedule Actions
    m_refreshSchedulesAction = new QAction("Schedules aktualisieren", this);
    m_refreshSchedulesAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshSchedulesAction->setShortcut(QKeySequence("Ctrl+Shift+D"));
    m_refreshSchedulesAction->setEnabled(false);
    connect(m_refreshSchedulesAction, &QAction::triggered, m_scheduleWidget, &BScheduleWidget::triggerRefresh);
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu("Datei");
    m_fileMenu->addAction(m_connectAction);
    m_fileMenu->addAction(m_connectLastAction);
    m_fileMenu->addAction(m_disconnectAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_refreshAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_settingsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    // Bearbeiten Menü
    m_editMenu = menuBar()->addMenu("Bearbeiten");
    m_editMenu->addAction(m_copyAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_selectAllAction);
    m_editMenu->addAction(m_clearSelectionAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_findAction);

    // Ansicht Menü
    m_viewMenu = menuBar()->addMenu("Ansicht");
    m_viewMenu->addAction(m_toggleStatisticsAction);

    // Jobs Menü
    m_jobsMenu = menuBar()->addMenu("Jobs");
    m_jobsMenu->addAction(m_runJobAction);
    m_jobsMenu->addAction(m_cancelJobAction);
    m_jobsMenu->addAction(m_jobDetailsAction);
    m_jobsMenu->addSeparator();
    m_jobsMenu->addAction(m_refreshJobsAction);
    m_jobsMenu->addSeparator();
    m_jobsMenu->addAction(m_exportJobsJsonAction);
    m_jobsMenu->addAction(m_exportJobsCsvAction);

    // Clients Menü
    m_clientsMenu = menuBar()->addMenu("Clients");
    m_clientsMenu->addAction(m_clientDetailsAction);
    m_clientsMenu->addSeparator();
    m_clientsMenu->addAction(m_refreshClientsAction);

    // Storage Menü
    m_storageMenu = menuBar()->addMenu("Storage");
    m_storageMenu->addAction(m_refreshStorageAction);

    // Schedules Menü
    m_schedulesMenu = menuBar()->addMenu("Schedules");
    m_schedulesMenu->addAction(m_refreshSchedulesAction);

    m_helpMenu = menuBar()->addMenu("Hilfe");
    m_helpMenu->addAction(m_aboutAction);
}

void MainWindow::createToolBar()
{
    m_mainToolBar = addToolBar("Haupt-Toolbar");
    m_mainToolBar->addAction(m_connectAction);
    m_mainToolBar->addAction(m_connectLastAction);
    m_mainToolBar->addAction(m_disconnectAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_refreshAction);

    // Spacer um den Toggle-Button rechts zu positionieren
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_mainToolBar->addWidget(spacer);

    // Toggle-Button für Statistiken
    m_toggleStatisticsButton = new QPushButton("Statistiken ▼", this);
    m_toggleStatisticsButton->setCheckable(false);
    m_toggleStatisticsButton->setEnabled(false);  // Nur bei Verbindung aktiv
    m_toggleStatisticsButton->setToolTip(tr("Statistiken ein-/ausblenden"));

    connect(m_toggleStatisticsButton, &QPushButton::clicked, this, [this]() {
        bool isVisible = m_statisticsDock->isVisible();
        m_statisticsDock->setVisible(!isVisible);
        m_toggleStatisticsButton->setText(isVisible ? "Statistiken ▶" : "Statistiken ▼");

        // Synchronisiere mit der Menu-Action
        if (m_toggleStatisticsAction) {
            m_toggleStatisticsAction->setChecked(!isVisible);
        }
    });

    m_mainToolBar->addWidget(m_toggleStatisticsButton);
}

void MainWindow::onConnectTriggered()
{
    showConnectionDialog();
}

void MainWindow::showConnectionDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Bareos Director Verbindung");
    dialog.resize(550, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setSpacing(15);

    // Lade gespeicherte Einstellungen
    m_director->loadConnectionSettings();

    BSettings& settings = BSettings::instance();

    // Bconsole-Verbindungsfelder
    QGroupBox *connectionGroup = new QGroupBox("Director-Verbindung", &dialog);
    QFormLayout *connectionLayout = new QFormLayout(connectionGroup);
    connectionLayout->setSpacing(12);

    QLineEdit *hostEdit = new QLineEdit(settings.connectionHost(), &dialog);
    hostEdit->setPlaceholderText("z.B. 192.168.1.100 oder bareos-dir.local");

    QSpinBox *portSpin = new QSpinBox(&dialog);
    portSpin->setRange(1, 65535);
    portSpin->setValue(settings.connectionPort());

    QLineEdit *directorEdit = new QLineEdit(settings.connectionDirector(), &dialog);
    directorEdit->setPlaceholderText("bareos-dir");

    QLineEdit *passwordEdit = new QLineEdit(&dialog);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setText(settings.connectionPassword());
    passwordEdit->setPlaceholderText("••••••••");

    connectionLayout->addRow("Host:", hostEdit);
    connectionLayout->addRow("Port:", portSpin);
    connectionLayout->addRow("Director Name:", directorEdit);
    connectionLayout->addRow("Passwort:", passwordEdit);

    mainLayout->addWidget(connectionGroup);

    // TLS Warning Label (angezeigt wenn TLS deaktiviert ist)
    QLabel *tlsWarningLabel = new QLabel(
        "⚠️ TLS verschlüsselt die Kommunikation mit dem Director.\n"
        "Für Produktionsumgebungen wird TLS dringend empfohlen!", &dialog);
    tlsWarningLabel->setStyleSheet(
        "QLabel { background-color: #3a2d1a; border-left: 3px solid #8f6a2d; "
        "padding: 12px; border-radius: 4px; color: #e8d9c4; }");
    tlsWarningLabel->setWordWrap(true);
    mainLayout->addWidget(tlsWarningLabel);

    // TLS/SSL-Einstellungen (checkable GroupBox)
    QGroupBox *tlsGroupBox = new QGroupBox("TLS/SSL-Verschlüsselung", &dialog);
    tlsGroupBox->setCheckable(true);
    tlsGroupBox->setChecked(settings.tlsEnabled());
    QVBoxLayout *tlsLayout = new QVBoxLayout(tlsGroupBox);
    tlsLayout->setSpacing(12);

    // Authentifizierungsmethode
    QLabel *authMethodLabel = new QLabel("Authentifizierungsmethode:");
    authMethodLabel->setStyleSheet("font-weight: bold;");
    tlsLayout->addWidget(authMethodLabel);

    QRadioButton *tlsPSKRadio = new QRadioButton("PSK (Pre-Shared Key) - Standard für Bareos 18.2+", &dialog);
    bool usePSK = settings.tlsUsePSK();
    tlsPSKRadio->setChecked(usePSK);
    tlsLayout->addWidget(tlsPSKRadio);

    QRadioButton *tlsCertificateRadio = new QRadioButton("Zertifikat-basierte TLS-Authentifizierung", &dialog);
    tlsCertificateRadio->setChecked(!usePSK);
    tlsLayout->addWidget(tlsCertificateRadio);

    // Zertifikat-Einstellungen (nur für Certificate-Modus)
    QWidget *certWidget = new QWidget(&dialog);
    QFormLayout *certLayout = new QFormLayout(certWidget);
    certLayout->setSpacing(12);

#ifndef Q_OS_WINDOWS
    // Linux: Separate PEM-Dateien
    QLineEdit *caCertEdit = new QLineEdit(settings.tlsCaCertFile(), &dialog);
    caCertEdit->setPlaceholderText("Pfad zum CA-Zertifikat (.pem)");
    QPushButton *caCertBrowse = new QPushButton("Durchsuchen...", &dialog);
    QHBoxLayout *caCertLayout = new QHBoxLayout();
    caCertLayout->addWidget(caCertEdit);
    caCertLayout->addWidget(caCertBrowse);
    certLayout->addRow("CA Certificate:", caCertLayout);

    connect(caCertBrowse, &QPushButton::clicked, [&, caCertEdit]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "CA-Zertifikat wählen",
                                                    QString(), "Zertifikate (*.pem *.crt *.cert);;Alle Dateien (*)");
        if (!file.isEmpty()) caCertEdit->setText(file);
    });

    QLineEdit *certEdit = new QLineEdit(settings.tlsCertFile(), &dialog);
    certEdit->setPlaceholderText("Pfad zum Client-Zertifikat (.pem)");
    QPushButton *certBrowse = new QPushButton("Durchsuchen...", &dialog);
    QHBoxLayout *certEditLayout = new QHBoxLayout();
    certEditLayout->addWidget(certEdit);
    certEditLayout->addWidget(certBrowse);
    certLayout->addRow("Client Certificate:", certEditLayout);

    connect(certBrowse, &QPushButton::clicked, [&, certEdit]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "Client-Zertifikat wählen",
                                                    QString(), "Zertifikate (*.pem *.crt *.cert);;Alle Dateien (*)");
        if (!file.isEmpty()) certEdit->setText(file);
    });

    QLineEdit *keyEdit = new QLineEdit(settings.tlsKeyFile(), &dialog);
    keyEdit->setPlaceholderText("Pfad zum Private Key (.pem, .key)");
    QPushButton *keyBrowse = new QPushButton("Durchsuchen...", &dialog);
    QHBoxLayout *keyEditLayout = new QHBoxLayout();
    keyEditLayout->addWidget(keyEdit);
    keyEditLayout->addWidget(keyBrowse);
    certLayout->addRow("Private Key:", keyEditLayout);

    connect(keyBrowse, &QPushButton::clicked, [&, keyEdit]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "Private Key wählen",
                                                    QString(), "Keys (*.pem *.key);;Alle Dateien (*)");
        if (!file.isEmpty()) keyEdit->setText(file);
    });
#else
    // Windows: PFX-Datei
    QLineEdit *pfxFileEdit = new QLineEdit(settings.tlsPfxFile(), &dialog);
    pfxFileEdit->setPlaceholderText("Pfad zum Client-Zertifikat (.pfx)");
    QPushButton *pfxBrowse = new QPushButton("Durchsuchen...", &dialog);
    QHBoxLayout *pfxLayout = new QHBoxLayout();
    pfxLayout->addWidget(pfxFileEdit);
    pfxLayout->addWidget(pfxBrowse);
    certLayout->addRow("PFX Certificate:", pfxLayout);

    connect(pfxBrowse, &QPushButton::clicked, [&, pfxFileEdit]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "PFX-Zertifikat wählen",
                                                    QString(), "PKCS#12 (*.pfx *.p12);;Alle Dateien (*)");
        if (!file.isEmpty()) pfxFileEdit->setText(file);
    });
#endif

    // Peer Verification
    QCheckBox *verifyPeerCheck = new QCheckBox("Server-Zertifikat validieren (empfohlen)", &dialog);
    verifyPeerCheck->setChecked(settings.tlsVerifyPeer());
    certLayout->addRow("", verifyPeerCheck);

    certWidget->setEnabled(!usePSK);  // Deaktiviert wenn PSK ausgewählt
    tlsLayout->addWidget(certWidget);

    // Certificate-Felder nur aktivieren wenn Certificate-Radio ausgewählt
    connect(tlsCertificateRadio, &QRadioButton::toggled, certWidget, &QWidget::setEnabled);

    mainLayout->addWidget(tlsGroupBox);

    // Verbinde Signal um Warnung anzuzeigen/verstecken
    tlsWarningLabel->setVisible(!tlsGroupBox->isChecked());
    connect(tlsGroupBox, &QGroupBox::toggled, [tlsWarningLabel](bool checked) {
        tlsWarningLabel->setVisible(!checked);
    });

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    mainLayout->addWidget(buttonBox);

    if (dialog.exec() == QDialog::Accepted) {
        // Speichere Einstellungen
        settings.setConnectionHost(hostEdit->text());
        settings.setConnectionPort(portSpin->value());
        settings.setConnectionDirector(directorEdit->text());
        settings.setConnectionPassword(passwordEdit->text());
        settings.setTlsEnabled(tlsGroupBox->isChecked());
        settings.setTlsUsePSK(tlsPSKRadio->isChecked());

#ifndef Q_OS_WINDOWS
        settings.setTlsCaCertFile(caCertEdit->text());
        settings.setTlsCertFile(certEdit->text());
        settings.setTlsKeyFile(keyEdit->text());
#else
        settings.setTlsPfxFile(pfxFileEdit->text());
#endif
        settings.setTlsVerifyPeer(verifyPeerCheck->isChecked());
        settings.sync();

        // TLS-Konfiguration setzen
        m_director->tlsConfig()->tlsEnable = tlsGroupBox->isChecked();
        m_director->tlsConfig()->tlsPSKEnable = tlsPSKRadio->isChecked();
        m_director->tlsConfig()->tlsVerifyPeer = verifyPeerCheck->isChecked();

#ifndef Q_OS_WINDOWS
        if (!caCertEdit->text().isEmpty()) {
            m_director->tlsConfig()->tlsCaCertFile->setFileName(caCertEdit->text());
        }
        if (!certEdit->text().isEmpty()) {
            m_director->tlsConfig()->tlsCertFile->setFileName(certEdit->text());
        }
        if (!keyEdit->text().isEmpty()) {
            m_director->tlsConfig()->tlsKeyFile->setFileName(keyEdit->text());
        }
#else
        if (!pfxFileEdit->text().isEmpty()) {
            m_director->tlsConfig()->tlsPfxFile->setFileName(pfxFileEdit->text());
        }
#endif

        // Verbindung herstellen - Thread-safe via helper slot
        onDirectorConnect(hostEdit->text(),
                          portSpin->value(),
                          directorEdit->text(),
                          passwordEdit->text());

        m_statusLabel->setText("Verbindung wird hergestellt...");
    }
}
void MainWindow::onDisconnectTriggered()
{
    m_director->disconnect();
    m_statusLabel->setText("Getrennt");
}

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

void MainWindow::onAboutTriggered()
{
    // Dialog erstellen
    QDialog aboutDialog(this);
    aboutDialog.setWindowTitle("Über Onesimus");
    aboutDialog.setMinimumSize(400, 300);

    // Hauptlayout
    QVBoxLayout* mainLayout = new QVBoxLayout(&aboutDialog);

    // Überschrift
    QLabel* titleLabel = new QLabel("<h2>Onesimus v1.0</h2>");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Horizontaler Container für Bild + Text
    QHBoxLayout* contentLayout = new QHBoxLayout();

    // Logo
    QLabel* logoLabel = new QLabel();
#if USE_BACULA
    logoLabel->setPixmap(QPixmap(":/images/logo_bacula.png").scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
#elif defined(USE_BAREOS)
    logoLabel->setPixmap(QPixmap(":/images/logo_bareos.png").scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
#endif
    contentLayout->addWidget(logoLabel);

    // Text neben dem Bild
    QLabel* textLabel = new QLabel();
#if USE_BACULA
    textLabel->setText("<p>Eine moderne Qt-Oberfläche für Bacula Backup</p>");
#elif defined(USE_BAREOS)
    textLabel->setText("<p>Eine moderne Qt-Oberfläche für Bareos Backup</p>");
#endif
    textLabel->setWordWrap(true);
    contentLayout->addWidget(textLabel);

    mainLayout->addLayout(contentLayout);

    // Unterstützte Features
    QLabel* featuresLabel = new QLabel(
        "<p>Unterstützt:</p>"
        "<ul>"
        "<li>Console TCP-Verbindungen über SSL/TLS</li>"
        "<li>Job-Verwaltung</li>"
        "<li>Client-Verwaltung</li>"
        "<li>Storage/Volume-Verwaltung</li>"
        "</ul>"
        "<p>© 2026</p>"
    );
    featuresLabel->setWordWrap(true);
    mainLayout->addWidget(featuresLabel);

    // Horizontaler Container für Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    // Schließen-Button
    QPushButton* closeButton = new QPushButton("Schließen");
    QObject::connect(closeButton, &QPushButton::clicked, &aboutDialog, &QDialog::accept);
    buttonLayout->addWidget(closeButton);

    // Über Qt Button mit Qt-Logo
    QPushButton* aboutQtButton = new QPushButton("Über Qt");

    // Qt-Logo aus Ressourcen (oder Standard Qt Icon verwenden)
    QPixmap qtLogo(":/icons/qt_logo.png"); // Füge das Qt-Logo in deine Ressourcen hinzu
    aboutQtButton->setIcon(QIcon(qtLogo));
    aboutQtButton->setIconSize(QSize(24, 24));

    QObject::connect(aboutQtButton, &QPushButton::clicked, this, &QApplication::aboutQt);
    buttonLayout->addWidget(aboutQtButton);

    // Buttons zum Hauptlayout hinzufügen, zentriert
    mainLayout->addLayout(buttonLayout);
    
    // Dialog anzeigen (modal)
    aboutDialog.exec();
}

void MainWindow::onSettingsTriggered()
{
    SettingsDialog dialog(m_director, this);
    if (dialog.exec() == QDialog::Accepted) {
        // Einstellungen wurden geändert
        m_statusLabel->setText("Einstellungen gespeichert");

        // Settings are automatically applied via BSettings signals
        // Auto-refresh is handled by onAutoRefreshSettingsChanged slot
    }
}

void MainWindow::onAuthentificationSucceeded(const bool connected, const QString msg)
{
#ifdef IS_DEVELOPER
    qDebug() << "####################################";
#endif
    // Update Actions
    m_connectAction->setDisabled(connected);
    m_disconnectAction->setEnabled(connected);
    m_refreshAction->setEnabled(connected);
    m_toggleStatisticsAction->setEnabled(connected);  // ✅ Statistiken nur bei Verbindung
    m_toggleStatisticsButton->setEnabled(connected);  // ✅ Toolbar-Button synchronisieren
    m_tabWidget->setEnabled(connected);

    // Update job menu actions
    m_refreshJobsAction->setEnabled(connected);
    m_exportJobsJsonAction->setEnabled(connected);
    m_exportJobsCsvAction->setEnabled(connected);
    // Job control actions (run, cancel, details) werden durch Job-Widget Selection gesteuert

    // Update client menu actions
    m_refreshClientsAction->setEnabled(connected);

    // Update storage menu actions
    m_refreshStorageAction->setEnabled(connected);

    // Update schedule menu actions
    m_refreshSchedulesAction->setEnabled(connected);

    // Update widget connection states
    m_jobWidget->setConnectionState(connected);
    m_storageWidget->setConnectionState(connected);
    m_scheduleWidget->setConnectionState(connected);

    if (connected) {
        // Zeige Version an wenn vorhanden
        QString statusText = QString("Verbunden BAREOS (v%1)").arg(msg);
        m_connectionLabel->setText(statusText);  // ✅ Text setzen!
        m_connectionLabel->setStyleSheet("color: green; font-weight: bold;");
        m_statusLabel->setText("Verbunden - Lade Daten...");

        // ✅ Stelle gespeicherten Statistics-DockWidget-Zustand wieder her
        bool statsVisible = BSettings::instance().statisticsWidgetVisible();
        m_statisticsDock->setVisible(statsVisible);
        m_toggleStatisticsAction->setChecked(statsVisible);
        m_toggleStatisticsButton->setText(statsVisible ? "Statistiken ▼" : "Statistiken ▶");

        // Enable edit menu actions
        m_copyAction->setEnabled(true);
        m_selectAllAction->setEnabled(true);
        m_clearSelectionAction->setEnabled(true);
        m_findAction->setEnabled(true);

        // Start auto-refresh if enabled in settings
        BSettings& settings = BSettings::instance();
        if (settings.behaviorAutoRefresh()) {
            m_autoRefreshTimer->start(settings.behaviorRefreshInterval() * 1000);
#ifdef IS_DEVELOPER
            qDebug() << "MainWindow: Auto-refresh started (interval:" << settings.behaviorRefreshInterval() << "seconds)";
#endif
        }

        // Refresh mit Verzögerung, damit API-Modus aktiviert wird
        // 300ms sollte ausreichen für API-Modus Aktivierung + erste Signale
        QTimer::singleShot(300, this, [this]() {
            onRefreshAll();
        });
    } else {
        m_connectionLabel->setText(msg);
        m_connectionLabel->setStyleSheet("color: red; font-weight: bold;");
        m_statusLabel->setText("");

        // ✅ Verstecke Statistics DockWidget bei Trennung
        m_statisticsDock->setVisible(false);
        m_toggleStatisticsAction->setChecked(false);
        m_toggleStatisticsButton->setText("Statistiken ▶");  // ✅ Button-Text aktualisieren

        // Disable edit menu actions
        m_copyAction->setEnabled(false);
        m_selectAllAction->setEnabled(false);
        m_clearSelectionAction->setEnabled(false);
        m_findAction->setEnabled(false);

        // Stop auto-refresh when disconnected
        m_autoRefreshTimer->stop();
#ifdef IS_DEVELOPER
        qDebug() << "MainWindow: Auto-refresh stopped (disconnected)";
#endif

        // Clear all widget data on disconnect
        m_jobWidget->clearData();
        m_clientWidget->clearData();
        m_storageWidget->clearData();
        m_scheduleWidget->clearData();
    }
}

void MainWindow::onConnectionError(const QString &error)
{
    QMessageBox::critical(this, "Verbindungsfehler", error);
    m_statusLabel->setText("Fehler: " + error);
}

void MainWindow::onRefreshAll()
{
#ifdef IS_DEVELOPER
    qDebug() << "Refresh clicked";
#endif

    if (!m_director->isConnected()) {
        return;
    }

    m_statusLabel->setText("Aktualisiere Daten...");

    // Thread-safe: Use helper slots to send commands
    onSendCommand(BDirector::Command::ListJobs, "");
    onSendCommand(BDirector::Command::ListClients, "");

    // ✅ Request filter data using dot-commands
    // These provide ALL configured jobs/clients/levels/schedules, not just executed ones
    onSendCommand(BDirector::Command::DotJobs, "");
    onSendCommand(BDirector::Command::DotClients, "");
    onSendCommand(BDirector::Command::DotLevels, "");
    onSendCommand(BDirector::Command::DotSchedule, "");

    // m_director->listVolumes();

    // ✅ Status nach kurzer Zeit zurücksetzen
    QTimer::singleShot(1000, this, [this]() {
        if (m_director->isConnected()) {
            m_statusLabel->setText("Bereit");
        }
    });
}

void MainWindow::onConnectLastUsed()
{
    if (!m_director->hasStoredConnection()) {
        QMessageBox::information(this, "Keine Verbindung gespeichert",
            "Es wurde keine vorherige Verbindung gefunden. Bitte verwenden Sie 'Verbinden' um eine neue Verbindung herzustellen.");
        return;
    }
    
    m_statusLabel->setText("Verbinde mit letzter Konfiguration...");
    loadAndConnectLastUsed();
}

void MainWindow::onSendCommand(const BDirector::Command cmd, const QString &args)
{
    if (!m_director) {
        qWarning() << "MainWindow::onSendCommand: No director instance!";
        return;
    }

    // Thread-safe: Use queued connection to send command to Director thread
    QMetaObject::invokeMethod(m_director, "doSendCommand",
                              Qt::QueuedConnection,
                              Q_ARG(BDirector::Command, cmd),
                              Q_ARG(QString, args));
}

void MainWindow::onDirectorConnect(const QString &host, int port, const QString &directorName, const QString &password)
{
    if (!m_director) {
        qWarning() << "MainWindow::onDirectorConnect: No director instance!";
        return;
    }

    // Thread-safe: Use queued connection to connect in Director thread
    QMetaObject::invokeMethod(m_director, "connect",
                              Qt::QueuedConnection,
                              Q_ARG(QString, host),
                              Q_ARG(int, port),
                              Q_ARG(QString, directorName),
                              Q_ARG(QString, password));
}

void MainWindow::onDirectorDisconnect()
{
    if (!m_director) {
        qWarning() << "MainWindow::onDirectorDisconnect: No director instance!";
        return;
    }

    // Thread-safe: Use queued connection to disconnect in Director thread
    QMetaObject::invokeMethod(m_director, "disconnect",
                              Qt::QueuedConnection);
}

void MainWindow::loadAndConnectLastUsed()
{
    BSettings& settings = BSettings::instance();

    if (!settings.hasStoredConnection()) {
        return;
    }

    // Lade gespeicherte Verbindungsdaten
    QString host = settings.connectionHost();
    int port = settings.connectionPort();
    QString director = settings.connectionDirector();
    QString password = settings.connectionPassword();

    // Prüfe ob Passwort vorhanden ist
    if (password.isEmpty() || host.isEmpty() || director.isEmpty()) {
#ifdef IS_DEVELOPER
        qDebug() << "Keine vollständigen Verbindungsdaten gespeichert - Automatische Verbindung übersprungen";
#endif
        m_connectLastAction->setEnabled(true);
        return;
    }

    // Lade TLS-Konfiguration
    m_director->tlsConfig()->tlsEnable = settings.tlsEnabled();
    m_director->tlsConfig()->tlsRequire = settings.tlsEnabled();
    m_director->tlsConfig()->tlsPSKEnable = settings.tlsUsePSK();
    m_director->tlsConfig()->tlsVerifyPeer = settings.tlsVerifyPeer();

    QString caCertFile = settings.tlsCaCertFile();
    if (!caCertFile.isEmpty()) {
        m_director->tlsConfig()->tlsCaCertFile->setFileName(caCertFile);
    }

#ifdef Q_OS_WINDOWS
    QString pfxFile = settings.tlsPfxFile();
    if (!pfxFile.isEmpty()) {
        m_director->tlsConfig()->tlsPfxFile->setFileName(pfxFile);
    }
#else
    QString certFile = settings.tlsCertFile();
    QString keyFile = settings.tlsKeyFile();
    if (!certFile.isEmpty()) {
        m_director->tlsConfig()->tlsCertFile->setFileName(certFile);
    }
    if (!keyFile.isEmpty()) {
        m_director->tlsConfig()->tlsKeyFile->setFileName(keyFile);
    }
#endif

    // Stelle Verbindung her - Thread-safe via helper slot
#ifdef IS_DEVELOPER
    qDebug() << "Automatische Wiederherstellung der letzten Verbindung:" << host << ":" << port;
#endif
    m_director->setTLSConfig(*m_director->tlsConfig());
    onDirectorConnect(host, port, director, password);
    m_statusLabel->setText(QString("Verbindung wird automatisch hergestellt zu %1...").arg(host));

    // Aktualisiere "Reconnect" Button Status
    m_connectLastAction->setEnabled(true);
}

BDirector *MainWindow::director() const
{
    return m_director;
}

void MainWindow::setDirector(BDirector *newDirector)
{
    m_director = newDirector;
}

void MainWindow::onCopyTriggered()
{
    // Get the currently active tab
    QWidget *currentWidget = m_tabWidget->currentWidget();

    if (currentWidget == m_jobWidget) {
        // Copy selected job data
        BJsonJobView *tableView = m_jobWidget->tableView();
        if (tableView && tableView->selectionModel()->hasSelection()) {
            QModelIndexList selectedIndexes = tableView->selectionModel()->selectedRows();
            QString copyText;

            for (const QModelIndex &index : selectedIndexes) {
                for (int col = 0; col < tableView->model()->columnCount(); ++col) {
                    QModelIndex cellIndex = tableView->model()->index(index.row(), col);
                    copyText += tableView->model()->data(cellIndex).toString();
                    if (col < tableView->model()->columnCount() - 1) {
                        copyText += "\t";
                    }
                }
                copyText += "\n";
            }

            QApplication::clipboard()->setText(copyText);
            m_statusLabel->setText(tr("%1 Zeile(n) kopiert").arg(selectedIndexes.count()));
        }
    } else if (currentWidget == m_clientWidget) {
        // Copy selected client data
        if (m_clientWidget->model()) {
            // TODO: Implement client data copy
            m_statusLabel->setText(tr("Client-Daten kopieren noch nicht implementiert"));
        }
    }
}

void MainWindow::onSelectAllTriggered()
{
    // Get the currently active tab
    QWidget *currentWidget = m_tabWidget->currentWidget();

    if (currentWidget == m_jobWidget) {
        // Select all jobs
        m_jobWidget->tableView()->jobsModel()->selectAll();
        m_statusLabel->setText(tr("Alle Jobs ausgewählt"));
    } else if (currentWidget == m_clientWidget) {
        // TODO: Implement select all for clients
        m_statusLabel->setText(tr("Alle Clients auswählen noch nicht implementiert"));
    }
}

void MainWindow::onClearSelectionTriggered()
{
    // Get the currently active tab
    QWidget *currentWidget = m_tabWidget->currentWidget();

    if (currentWidget == m_jobWidget) {
        // Clear job selection
        m_jobWidget->tableView()->jobsModel()->clearSelection();
        m_statusLabel->setText(tr("Auswahl aufgehoben"));
    } else if (currentWidget == m_clientWidget) {
        // TODO: Implement clear selection for clients
        m_statusLabel->setText(tr("Auswahl für Clients aufheben noch nicht implementiert"));
    }
}

void MainWindow::onFindTriggered()
{
    // Get the currently active tab
    QWidget *currentWidget = m_tabWidget->currentWidget();

    if (currentWidget == m_jobWidget) {
        // Info: User kann Filter mit dem Toggle-Button in der Job-Ansicht nutzen
        QMessageBox::information(this, tr("Suchen"),
            tr("Verwenden Sie die Filter-Optionen in der Job-Ansicht,\n"
               "um nach bestimmten Jobs zu suchen.\n\n"
               "Verfügbare Filter:\n"
               "• Job-Name\n"
               "• Client-Name\n"
               "• Status (Erfolg, Warnung, Fehler)\n"
               "• Level (Full, Incremental, Differential)\n"
               "• Datums-Bereich"));
    } else if (currentWidget == m_clientWidget) {
        QMessageBox::information(this, tr("Suchen"),
            tr("Verwenden Sie die Filter-ComboBox in der Client-Ansicht,\n"
               "um nach bestimmten Clients zu suchen."));
    }
}

void MainWindow::onAutoRefreshSettingsChanged(bool enabled, int intervalSeconds)
{
#ifdef IS_DEVELOPER
    qDebug() << "MainWindow: Auto-refresh settings changed:"
             << "enabled=" << enabled << "interval=" << intervalSeconds << "seconds";
#endif

    if (enabled && m_director->isConnected()) {
        // Start or restart timer with new interval
        m_autoRefreshTimer->start(intervalSeconds * 1000);
        m_statusLabel->setText(QString("Auto-Refresh aktiviert (%1s)").arg(intervalSeconds));
    } else {
        // Stop timer
        m_autoRefreshTimer->stop();
        if (!enabled && m_director->isConnected()) {
            m_statusLabel->setText("Auto-Refresh deaktiviert");
        }
    }
}

void MainWindow::onExportSettingsTriggered()
{
    if (!m_director->isConnected()) {
        QMessageBox::warning(this, "Nicht verbunden", "Bitte stellen Sie zuerst eine Verbindung zum Director her.");
        return;
    }

    // Export all jobs to JSON
    m_jobWidget->tableView()->exportToJson(false);
    m_statusLabel->setText("Jobs als JSON exportiert");
}

void MainWindow::onImportSettingsTriggered()
{
    if (!m_director->isConnected()) {
        QMessageBox::warning(this, "Nicht verbunden", "Bitte stellen Sie zuerst eine Verbindung zum Director her.");
        return;
    }

    // Export all jobs to CSV
    m_jobWidget->tableView()->exportToCsv(false);
    m_statusLabel->setText("Jobs als CSV exportiert");
}
