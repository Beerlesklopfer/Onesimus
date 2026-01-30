#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jobs/bjobwidget.h"
#include "jobs/bjobsstatisticswidget.h"
#include "clients/bclientswidget.h"
#include "storagewidget.h"
#include "schedules/bschedulewidget.h"
#include "settingsdialog.h"
#include "bcleanupdialog.h"
#include "bsettings.h"
#include "bconnectionprofile.h"
#include "version.h"

#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QDateTime>
#include <QDesktopServices>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
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
#include <QStyle>
#include <QTabWidget>
#include <QTextEdit>
#include <QTextStream>
#include <QToolBar>
#include <QUrl>
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

    // Restore statistics action checked state after action is created
    bool statsVisible = BSettings::instance().statisticsWidgetVisible();
    m_toggleStatisticsAction->setChecked(statsVisible);

    // ✅ Synchronisiere DockWidget-Sichtbarkeit mit Action und speichere in Settings
    connect(m_statisticsDock, &QDockWidget::visibilityChanged, this, [this](bool visible) {
        m_toggleStatisticsAction->setChecked(visible);
        // Save statistics widget visibility state
        BSettings::instance().setStatisticsWidgetVisible(visible);

        if (!visible) {
            // Restore previous window size when hiding statistics
            if (!m_sizeBeforeStatistics.isEmpty()) {
                QTimer::singleShot(100, this, [this]() {
                    resize(m_sizeBeforeStatistics);
                });
            }
        }
    });

    connect(m_director, &BDirector::disconnected, this, [this]() {
        onAuthentificationSucceeded(false, "");
    });

    // ✅ statusMessage Signal wird erst nach erfolgreicher Auth verbunden
    // (siehe onAuthentificationSucceeded), um Auth-Fehler nicht in der Statuszeile anzuzeigen

    connect(m_director, &BDirector::authentificationSucceeded,  this, &MainWindow::onAuthentificationSucceeded);

    connect(m_director, &BDirector::protocolError, this, &MainWindow::onConnectionError);

    // ✅ Handle non-JSON command responses (for debugging)
    connect(m_director, &BDirector::commandResponse, this, [this](const QString &command, const QString &response) {
        Q_UNUSED(command)
        Q_UNUSED(response)
#ifdef IS_DEVELOPER
        qDebug() << "MainWindow: Command response for:" << command << "Response:" << response.left(100);
#endif
        // Note: API mode confirmation and resource loading is now handled
        // by the BareosDirector state machine automatically
    });

    // ✅ Connect to state machine signals
    connect(m_director, &BDirector::connectionStateChanged, this,
            [this](BDirector::ConnectionState oldState, BDirector::ConnectionState newState) {
        Q_UNUSED(oldState)
#ifdef IS_DEVELOPER
        qDebug() << "MainWindow: Connection state changed:" << static_cast<int>(oldState)
                 << "->" << static_cast<int>(newState);
#endif
        // Update status bar based on state
        switch (newState) {
        case BDirector::ConnectionState::Connecting:
            statusBar()->showMessage(tr("Connecting..."));
            break;
        case BDirector::ConnectionState::Authenticating:
            statusBar()->showMessage(tr("Authenticating..."));
            break;
        case BDirector::ConnectionState::SettingApiMode:
            statusBar()->showMessage(tr("Setting API mode..."));
            break;
        case BDirector::ConnectionState::LoadingResources:
            statusBar()->showMessage(tr("Loading resources..."));
            break;
        case BDirector::ConnectionState::Ready:
            statusBar()->showMessage(tr("Connected and ready"), 5000);
            break;
        case BDirector::ConnectionState::ConnectionError:
            statusBar()->showMessage(tr("Connection error"));
            break;
        case BDirector::ConnectionState::Disconnected:
            statusBar()->showMessage(tr("Disconnected"));
            break;
        }
    });

    connect(m_director, &BDirector::resourceLoadProgress, this,
            [this](int loaded, int total) {
        statusBar()->showMessage(tr("Loading resources... %1/%2").arg(loaded).arg(total));
    });

    connect(m_director, &BDirector::allResourcesLoaded, this, [this]() {
#ifdef IS_DEVELOPER
        qDebug() << "MainWindow: All resources loaded - refreshing views";
#endif
        // Restore cursor - loading complete
        QApplication::restoreOverrideCursor();

        // Trigger UI refresh now that all resources are available
        onRefreshAll();
    });

    // ✅ Route JSON responses to appropriate widgets based on JSON content
    // Note: We route based on JSON structure, not command name, because multiple
    // commands can be sent asynchronously and m_lastCommand may be overwritten
    connect(m_director, &BDirector::jsonResponse, this, [this](const QString &command, const QString &jsonData) {
        // Parse JSON to determine response type
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "MainWindow: Failed to parse JSON response:" << parseError.errorString();
            return;
        }

        if (!doc.isObject()) {
            qWarning() << "MainWindow: JSON response is not an object";
            return;
        }

        QJsonObject root = doc.object();
        QJsonObject result = root["result"].toObject();

        // Route based on JSON content (keys in result object)
        bool routed = false;

        // Check for dot-command responses (have specific array keys)
        if (result.contains("levels")) {
            qWarning() << "→ Routing levels data to JobWidget (detected by JSON content)";
            m_jobWidget->processDotLevelsResponse(jsonData);
            routed = true;
        }
        if (result.contains("filesets")) {
            qWarning() << "→ Routing filesets data to JobWidget (detected by JSON content)";
            m_jobWidget->processDotFilesetsResponse(jsonData);
            routed = true;
        }
        if (result.contains("storages")) {
            qWarning() << "→ Routing storages data to JobWidget (detected by JSON content)";
            m_jobWidget->processDotStoragesResponse(jsonData);
            routed = true;
        }
        if (result.contains("pools")) {
            qWarning() << "→ Routing pools data to JobWidget (detected by JSON content)";
            m_jobWidget->processDotPoolsResponse(jsonData);
            routed = true;
        }
        if (result.contains("schedules")) {
            qWarning() << "→ Routing schedules data to ScheduleWidget (detected by JSON content)";
            m_scheduleWidget->processDotScheduleResponse(jsonData);
            routed = true;
        }

        // Check for .jobs response (array of job configurations with "name" and "enabled" keys)
        if (result.contains("jobs")) {
            QJsonArray jobsArray = result["jobs"].toArray();
            if (!jobsArray.isEmpty()) {
                QJsonObject firstJob = jobsArray[0].toObject();
                // .jobs has "enabled" field, list jobs has "jobstatus"
                if (firstJob.contains("enabled") || firstJob.contains("fileset")) {
                    qWarning() << "→ Routing .jobs data to JobWidget (detected by JSON content)";
                    m_jobWidget->processDotJobsResponse(jsonData);
                    routed = true;
                } else if (firstJob.contains("jobstatus") || firstJob.contains("jobid")) {
                    qWarning() << "→ Routing list jobs data to JobWidget (detected by JSON content)";
                    m_jobWidget->processJsonResponse(jsonData);
                    routed = true;
                }
            }
        }

        // Check for .clients response
        if (result.contains("clients")) {
            QJsonArray clientsArray = result["clients"].toArray();
            if (!clientsArray.isEmpty()) {
                QJsonObject firstClient = clientsArray[0].toObject();
                // .clients has simple "name" field, list clients has more fields
                if (firstClient.contains("address") || firstClient.contains("uname")) {
                    qWarning() << "→ Routing list clients data to ClientWidget (detected by JSON content)";
                    m_clientWidget->processJsonResponse(jsonData);
                    routed = true;
                } else {
                    qWarning() << "→ Routing .clients data to JobWidget (detected by JSON content)";
                    m_jobWidget->processDotClientsResponse(jsonData);
                    routed = true;
                }
            }
        }

        // Check for volumes/media response
        if (result.contains("volumes") || result.contains("media")) {
            qWarning() << "→ Routing volumes data to StorageWidget (detected by JSON content)";
            m_storageWidget->processJsonResponse(jsonData);
            routed = true;
        }

        // Fallback to command-based routing if content-based didn't match
        if (!routed) {
            if (command.contains("list jobs") || command.contains("list jobid")) {
                qWarning() << "→ Routing jobs data to JobWidget (by command)";
                m_jobWidget->processJsonResponse(jsonData);
            } else if (command.contains("list clients")) {
                qWarning() << "→ Routing clients data to ClientWidget (by command)";
                m_clientWidget->processJsonResponse(jsonData);
            } else if (command.contains("list volumes") || command.contains("list media")) {
                qWarning() << "→ Routing volumes data to StorageWidget (by command)";
                m_storageWidget->processJsonResponse(jsonData);
            } else if (command.contains("list joblog")) {
                // Job log responses are handled directly by BJobDetailsDialog
                qWarning() << "→ Job log response (handled by job details dialog)";
            } else {
                qWarning() << "⚠ Unhandled JSON response - command:" << command;
                qWarning() << "  Result keys:" << result.keys();
            }
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

    // Apply saved theme from settings
    QString savedTheme = BSettings::instance().appearanceTheme();
    applyTheme(savedTheme);
}

MainWindow::~MainWindow()
{
    m_director->saveConnectionSettings();
    delete ui;
}

void MainWindow::applyTheme(const QString &themeName)
{
    QString themePath;

    if (themeName == "light") {
        themePath = ":/themes/themes/light.qss";
    } else {
        // Default to dark theme
        themePath = ":/themes/themes/dark.qss";
    }

    // Load and apply stylesheet
    QFile themeFile(themePath);
    if (themeFile.open(QFile::ReadOnly | QFile::Text)) {
        QString stylesheet = QLatin1String(themeFile.readAll());
        qApp->setStyleSheet(stylesheet);
        themeFile.close();

#ifdef IS_DEVELOPER
        qDebug() << "Applied theme:" << themeName << "from" << themePath;
#endif
    } else {
        qWarning() << "Could not load theme file:" << themePath;
    }
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
    // Note: Action checked state will be set after createActions() in constructor

    // Status bar
    m_statusLabel = new QLabel(tr("Ready"), this);
    m_statusLabel->setObjectName("statusLabel");
    statusBar()->addWidget(m_statusLabel);

    m_connectionLabel = new QLabel(tr("Not connected"), this);
    m_connectionLabel->setObjectName("connectionLabel");
    m_connectionLabel->setProperty("connected", false);
    QColor disconnectedColor = BSettings::instance().statusBarDisconnectedColor();
    m_connectionLabel->setStyleSheet(QString("color: %1; font-weight: bold;").arg(disconnectedColor.name()));
    statusBar()->addPermanentWidget(m_connectionLabel);
}

void MainWindow::createActions()
{
    m_connectAction = new QAction(tr("Connect"), this);
    m_connectAction->setIcon(QIcon::fromTheme("network-connect"));
    m_connectAction->setShortcut(QKeySequence("Ctrl+O"));
    connect(m_connectAction, &QAction::triggered, this, &MainWindow::onConnectTriggered);

    m_connectLastAction = new QAction(tr("Reconnect"), this);
    m_connectLastAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_connectLastAction->setShortcut(QKeySequence("Ctrl+R"));
    m_connectLastAction->setEnabled(m_director->hasStoredConnection());
    connect(m_connectLastAction, &QAction::triggered, this, &MainWindow::onConnectLastUsed);

    m_disconnectAction = new QAction(tr("Disconnect"), this);
    m_disconnectAction->setIcon(QIcon::fromTheme("network-disconnect"));
    m_disconnectAction->setEnabled(false);
    connect(m_disconnectAction, &QAction::triggered, this, &MainWindow::onDisconnectTriggered);

    // Toggle Connection Action (for toolbar)
    m_toggleConnectionAction = new QAction(tr("Connect"), this);
    m_toggleConnectionAction->setIcon(QIcon(":/icons/icons/connect.png"));
    m_toggleConnectionAction->setToolTip(tr("Connect to Director"));
    connect(m_toggleConnectionAction, &QAction::triggered, this, &MainWindow::onToggleConnectionTriggered);

    // Reconnect Action (for toolbar)
    m_reconnectAction = new QAction(tr("Reconnect"), this);
    m_reconnectAction->setIcon(QIcon(":/icons/icons/reconnect.png"));
    m_reconnectAction->setToolTip(tr("Reconnect to last used connection"));
    m_reconnectAction->setEnabled(m_director->hasStoredConnection());
    connect(m_reconnectAction, &QAction::triggered, this, &MainWindow::onConnectLastUsed);

    m_refreshAction = new QAction(tr("Refresh"), this);
    m_refreshAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshAction->setShortcut(QKeySequence("F5"));
    m_refreshAction->setEnabled(false);
    connect(m_refreshAction, &QAction::triggered, this, &MainWindow::onRefreshAll);

    m_settingsAction = new QAction(tr("Settings"), this);
    m_settingsAction->setIcon(QIcon::fromTheme("preferences-system"));
    m_settingsAction->setShortcut(QKeySequence("Ctrl+,"));
    connect(m_settingsAction, &QAction::triggered, this, &MainWindow::onSettingsTriggered);

    m_exitAction = new QAction(tr("Exit"), this);
    m_exitAction->setIcon(QIcon(":/icons/icons/exit.png"));
    m_exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);

    m_aboutAction = new QAction(tr("About Onesimus"), this);
    m_aboutAction->setIcon(QIcon::fromTheme("help-about"));
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAboutTriggered);

    m_documentationAction = new QAction(tr("Online Documentation"), this);
    m_documentationAction->setIcon(QIcon::fromTheme("help-contents"));
    m_documentationAction->setShortcut(QKeySequence::HelpContents);
    connect(m_documentationAction, &QAction::triggered, this, &MainWindow::onDocumentationTriggered);

    m_reportBugAction = new QAction(tr("Report a Bug..."), this);
    m_reportBugAction->setIcon(QIcon::fromTheme("tools-report-bug"));
    connect(m_reportBugAction, &QAction::triggered, this, &MainWindow::onReportBugTriggered);

    m_keyboardShortcutsAction = new QAction(tr("Keyboard Shortcuts"), this);
    m_keyboardShortcutsAction->setIcon(QIcon::fromTheme("preferences-desktop-keyboard"));
    m_keyboardShortcutsAction->setShortcut(QKeySequence("Ctrl+?"));
    connect(m_keyboardShortcutsAction, &QAction::triggered, this, &MainWindow::onKeyboardShortcutsTriggered);

    // Edit Actions
    m_copyAction = new QAction(tr("Copy"), this);
    m_copyAction->setIcon(QIcon::fromTheme("edit-copy"));
    m_copyAction->setShortcut(QKeySequence::Copy);
    m_copyAction->setEnabled(false);
    connect(m_copyAction, &QAction::triggered, this, &MainWindow::onCopyTriggered);

    m_selectAllAction = new QAction(tr("Select All"), this);
    m_selectAllAction->setIcon(QIcon::fromTheme("edit-select-all"));
    m_selectAllAction->setShortcut(QKeySequence::SelectAll);
    m_selectAllAction->setEnabled(false);
    connect(m_selectAllAction, &QAction::triggered, this, &MainWindow::onSelectAllTriggered);

    m_clearSelectionAction = new QAction(tr("Clear Selection"), this);
    m_clearSelectionAction->setIcon(QIcon::fromTheme("edit-clear"));
    m_clearSelectionAction->setEnabled(false);
    connect(m_clearSelectionAction, &QAction::triggered, this, &MainWindow::onClearSelectionTriggered);

    m_findAction = new QAction(tr("Find..."), this);
    m_findAction->setIcon(QIcon::fromTheme("edit-find"));
    m_findAction->setShortcut(QKeySequence::Find);
    m_findAction->setEnabled(false);
    connect(m_findAction, &QAction::triggered, this, &MainWindow::onFindTriggered);

    // View Actions
    m_toggleStatisticsAction = new QAction(tr("Show Statistics"), this);
    m_toggleStatisticsAction->setCheckable(true);
    m_toggleStatisticsAction->setChecked(false);  // Hidden by default (until connected)
    m_toggleStatisticsAction->setEnabled(false);  // Only active when connected
    m_toggleStatisticsAction->setIcon(QIcon::fromTheme("view-statistics"));
    m_toggleStatisticsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(m_toggleStatisticsAction, &QAction::toggled, this, [this](bool checked) {
        if (checked) {
            // Save current window size before showing statistics
            m_sizeBeforeStatistics = size();
        }
        m_statisticsDock->setVisible(checked);
    });

    m_toggleJobLogAction = new QAction(tr("Show Job Log"), this);
    m_toggleJobLogAction->setCheckable(true);
    m_toggleJobLogAction->setChecked(true);  // Visible by default
    m_toggleJobLogAction->setEnabled(false);  // Only active when connected
    m_toggleJobLogAction->setIcon(QIcon::fromTheme("view-list-details"));
    m_toggleJobLogAction->setShortcut(QKeySequence("Ctrl+Shift+L"));
    connect(m_toggleJobLogAction, &QAction::toggled, this, [this](bool checked) {
        if (m_jobWidget) {
            m_jobWidget->setLogViewVisible(checked);
        }
    });

    // Theme Toggle Action
    m_toggleThemeAction = new QAction(this);
    m_toggleThemeAction->setToolTip(tr("Toggle Dark/Light Theme"));
    m_toggleThemeAction->setShortcut(QKeySequence("Ctrl+Shift+T"));

    // Set initial icon based on current theme
    QString currentTheme = BSettings::instance().appearanceTheme();
    if (currentTheme == "dark") {
        m_toggleThemeAction->setIcon(QIcon(":/icons/icons/sun.png"));
    } else {
        m_toggleThemeAction->setIcon(QIcon(":/icons/icons/moon.png"));
    }

    connect(m_toggleThemeAction, &QAction::triggered, this, &MainWindow::onToggleTheme);

    // Jobs Actions
    m_runJobAction = new QAction(tr("Run Job"), this);
    m_runJobAction->setIcon(QIcon::fromTheme("media-playback-start"));
    m_runJobAction->setEnabled(false);
    connect(m_runJobAction, &QAction::triggered, m_jobWidget, &BJobWidget::triggerRunJob);

    m_cancelJobAction = new QAction(tr("Cancel Job"), this);
    m_cancelJobAction->setIcon(QIcon::fromTheme("process-stop"));
    m_cancelJobAction->setEnabled(false);
    connect(m_cancelJobAction, &QAction::triggered, m_jobWidget, &BJobWidget::triggerCancelJob);

    m_jobDetailsAction = new QAction(tr("Show Details"), this);
    m_jobDetailsAction->setIcon(QIcon::fromTheme("document-properties"));
    m_jobDetailsAction->setEnabled(false);
    connect(m_jobDetailsAction, &QAction::triggered, m_jobWidget, &BJobWidget::triggerShowDetails);

    m_refreshJobsAction = new QAction(tr("Refresh Jobs"), this);
    m_refreshJobsAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshJobsAction->setShortcut(QKeySequence("Ctrl+Shift+J"));
    m_refreshJobsAction->setEnabled(false);
    connect(m_refreshJobsAction, &QAction::triggered, m_jobWidget, &BJobWidget::triggerRefresh);

    m_exportJobsJsonAction = new QAction(tr("Export Jobs as JSON..."), this);
    m_exportJobsJsonAction->setIcon(QIcon::fromTheme("document-save"));
    m_exportJobsJsonAction->setEnabled(false);
    connect(m_exportJobsJsonAction, &QAction::triggered, this, &MainWindow::onExportSettingsTriggered);

    m_exportJobsCsvAction = new QAction(tr("Export Jobs as CSV..."), this);
    m_exportJobsCsvAction->setIcon(QIcon::fromTheme("text-csv"));
    m_exportJobsCsvAction->setEnabled(false);
    connect(m_exportJobsCsvAction, &QAction::triggered, this, &MainWindow::onImportSettingsTriggered);

    // Clients Actions
    m_refreshClientsAction = new QAction(tr("Refresh Clients"), this);
    m_refreshClientsAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshClientsAction->setShortcut(QKeySequence("Ctrl+Shift+C"));
    m_refreshClientsAction->setEnabled(false);
    connect(m_refreshClientsAction, &QAction::triggered, m_clientWidget, &BClientsWidget::triggerRefresh);

    m_clientDetailsAction = new QAction(tr("Show Client Details"), this);
    m_clientDetailsAction->setIcon(QIcon::fromTheme("document-properties"));
    m_clientDetailsAction->setEnabled(false);
    // TODO: Connect to client details dialog when implemented

    // Storage Actions
    m_refreshStorageAction = new QAction(tr("Refresh Storage"), this);
    m_refreshStorageAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshStorageAction->setShortcut(QKeySequence("Ctrl+Shift+V"));
    m_refreshStorageAction->setEnabled(false);
    connect(m_refreshStorageAction, &QAction::triggered, m_storageWidget, &StorageWidget::triggerRefresh);

    // Schedule Actions
    m_refreshSchedulesAction = new QAction(tr("Refresh Schedules"), this);
    m_refreshSchedulesAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshSchedulesAction->setShortcut(QKeySequence("Ctrl+Shift+D"));
    m_refreshSchedulesAction->setEnabled(false);
    connect(m_refreshSchedulesAction, &QAction::triggered, m_scheduleWidget, &BScheduleWidget::triggerRefresh);

    // Tools Actions
    m_cleanupDatabaseAction = new QAction(tr("Database Cleanup..."), this);
    m_cleanupDatabaseAction->setIcon(QIcon::fromTheme("edit-clear"));
    m_cleanupDatabaseAction->setToolTip(tr("Clean up old backups and free disk space"));
    m_cleanupDatabaseAction->setEnabled(false);
    connect(m_cleanupDatabaseAction, &QAction::triggered, this, &MainWindow::onCleanupDatabase);
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("File"));
    m_fileMenu->addAction(m_connectAction);
    m_fileMenu->addAction(m_connectLastAction);
    m_fileMenu->addAction(m_disconnectAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_refreshAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_settingsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    // Edit Menu
    m_editMenu = menuBar()->addMenu(tr("Edit"));
    m_editMenu->addAction(m_copyAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_selectAllAction);
    m_editMenu->addAction(m_clearSelectionAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_findAction);

    // View Menu
    m_viewMenu = menuBar()->addMenu(tr("View"));
    m_viewMenu->addAction(m_toggleStatisticsAction);
    m_viewMenu->addAction(m_toggleJobLogAction);

    // Jobs Menu
    m_jobsMenu = menuBar()->addMenu(tr("Jobs"));
    m_jobsMenu->addAction(m_runJobAction);
    m_jobsMenu->addAction(m_cancelJobAction);
    m_jobsMenu->addAction(m_jobDetailsAction);
    m_jobsMenu->addSeparator();
    m_jobsMenu->addAction(m_refreshJobsAction);
    m_jobsMenu->addSeparator();
    m_jobsMenu->addAction(m_exportJobsJsonAction);
    m_jobsMenu->addAction(m_exportJobsCsvAction);

    // Clients Menu
    m_clientsMenu = menuBar()->addMenu(tr("Clients"));
    m_clientsMenu->addAction(m_clientDetailsAction);
    m_clientsMenu->addSeparator();
    m_clientsMenu->addAction(m_refreshClientsAction);

    // Storage Menu
    m_storageMenu = menuBar()->addMenu(tr("Storage"));
    m_storageMenu->addAction(m_refreshStorageAction);

    // Schedules Menu
    m_schedulesMenu = menuBar()->addMenu(tr("Schedules"));
    m_schedulesMenu->addAction(m_refreshSchedulesAction);

    // Tools Menu
    m_toolsMenu = menuBar()->addMenu(tr("Tools"));
    m_toolsMenu->addAction(m_cleanupDatabaseAction);

    m_helpMenu = menuBar()->addMenu(tr("Help"));
    m_helpMenu->addAction(m_documentationAction);
    m_helpMenu->addAction(m_keyboardShortcutsAction);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_reportBugAction);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_aboutAction);

    // Add theme toggle button to the right side of menu bar
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    menuBar()->setCornerWidget(spacer, Qt::TopLeftCorner);

    QToolBar *themeToolbar = new QToolBar();
    themeToolbar->setStyleSheet("QToolBar { border: none; background: transparent; }");
    themeToolbar->setIconSize(QSize(16, 16));  // Half size (16x16 instead of 32x32)
    themeToolbar->addAction(m_toggleThemeAction);
    menuBar()->setCornerWidget(themeToolbar, Qt::TopRightCorner);
}

void MainWindow::createToolBar()
{
    m_mainToolBar = addToolBar(tr("Main Toolbar"));

    // Exit button first
    m_mainToolBar->addAction(m_exitAction);

    // Spacer to push connection buttons to the right
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_mainToolBar->addWidget(spacer);

    // Connection control buttons on the right
    m_mainToolBar->addAction(m_toggleConnectionAction);
    m_mainToolBar->addAction(m_reconnectAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_refreshAction);
    m_mainToolBar->addSeparator();

    // Toggle button for statistics
    m_toggleStatisticsButton = new QPushButton(tr("Statistics ▼"), this);
    m_toggleStatisticsButton->setCheckable(false);
    m_toggleStatisticsButton->setEnabled(false);  // Only active when connected
    m_toggleStatisticsButton->setToolTip(tr("Show/Hide Statistics"));

    connect(m_toggleStatisticsButton, &QPushButton::clicked, this, [this]() {
        bool isVisible = m_statisticsDock->isVisible();

        if (!isVisible) {
            // Save current window size before showing statistics
            m_sizeBeforeStatistics = size();
        }

        m_statisticsDock->setVisible(!isVisible);
        m_toggleStatisticsButton->setText(isVisible ? tr("Statistics ▶") : tr("Statistics ▼"));

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
    BSettings& settings = BSettings::instance();

    // Migrate old settings if needed
    settings.migrateOldConnectionSettings();

    // Get available profiles
    QList<BConnectionProfile> profiles = settings.connectionProfiles();

    // If no profiles exist, open settings dialog to create one
    if (profiles.isEmpty()) {
        QMessageBox::information(this, tr("No Connection Profiles"),
            tr("No connection profiles found.\n\n"
               "Please create a connection profile in Settings first."));
        onSettingsTriggered();
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("Connect to Director"));
    dialog.resize(450, 250);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setSpacing(20);

    // Title
    QLabel *titleLabel = new QLabel(tr("Select Connection Profile"));
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    // Profile selection
    QGroupBox *profileGroup = new QGroupBox(tr("Connection Profile"), &dialog);
    QVBoxLayout *profileLayout = new QVBoxLayout(profileGroup);
    profileLayout->setSpacing(12);

    QComboBox *profileCombo = new QComboBox(&dialog);
    QString lastUsedId = settings.lastUsedProfileId();
    int selectedIndex = 0;

    for (int i = 0; i < profiles.size(); ++i) {
        const BConnectionProfile &profile = profiles[i];
        profileCombo->addItem(profile.displayName(), profile.id);
        if (profile.id == lastUsedId) {
            selectedIndex = i;
        }
    }
    profileCombo->setCurrentIndex(selectedIndex);
    profileLayout->addWidget(profileCombo);

    // Profile info label
    QLabel *infoLabel = new QLabel(&dialog);
    infoLabel->setStyleSheet("color: #888; font-style: italic;");
    infoLabel->setWordWrap(true);
    profileLayout->addWidget(infoLabel);

    // Update info when profile changes
    auto updateInfo = [&profiles, profileCombo, infoLabel]() {
        QString profileId = profileCombo->currentData().toString();
        for (const BConnectionProfile &p : profiles) {
            if (p.id == profileId) {
                QString authMethod = p.legacyAuth ? "Legacy (unencrypted)" :
                                     (p.tlsUsePSK ? "TLS-PSK" : "TLS Certificate");
                infoLabel->setText(QString("%1:%2 - %3")
                    .arg(p.host).arg(p.port).arg(authMethod));
                break;
            }
        }
    };
    connect(profileCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            &dialog, updateInfo);
    updateInfo();

    mainLayout->addWidget(profileGroup);

    // Manage profiles link
    QHBoxLayout *linkLayout = new QHBoxLayout();
    QPushButton *manageBtn = new QPushButton(tr("Manage Profiles..."), &dialog);
    manageBtn->setFlat(true);
    manageBtn->setStyleSheet("color: #4a90d9; text-decoration: underline;");
    manageBtn->setCursor(Qt::PointingHandCursor);
    connect(manageBtn, &QPushButton::clicked, [this, &dialog]() {
        dialog.reject();
        onSettingsTriggered();
    });
    linkLayout->addStretch();
    linkLayout->addWidget(manageBtn);
    mainLayout->addLayout(linkLayout);

    mainLayout->addStretch();

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
    QPushButton *connectBtn = buttonBox->addButton(tr("Connect"), QDialogButtonBox::AcceptRole);
    connectBtn->setDefault(true);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    mainLayout->addWidget(buttonBox);

    if (dialog.exec() == QDialog::Accepted) {
        QString profileId = profileCombo->currentData().toString();
        BConnectionProfile profile = settings.connectionProfile(profileId);

        if (!profile.isValid()) {
            QMessageBox::critical(this, tr("Error"),
                tr("Selected profile is invalid."));
            return;
        }

        // Save as last used
        settings.setLastUsedProfileId(profileId);
        settings.sync();

        // Configure TLS
        m_director->tlsConfig()->tlsEnable = profile.tlsEnabled;
        m_director->tlsConfig()->tlsPSKEnable = profile.tlsUsePSK;
        m_director->tlsConfig()->tlsVerifyPeer = profile.tlsVerifyPeer;

#ifndef Q_OS_WINDOWS
        if (!profile.tlsCaCertFile.isEmpty()) {
            m_director->tlsConfig()->tlsCaCertFile->setFileName(profile.tlsCaCertFile);
        }
        if (!profile.tlsCertFile.isEmpty()) {
            m_director->tlsConfig()->tlsCertFile->setFileName(profile.tlsCertFile);
        }
        if (!profile.tlsKeyFile.isEmpty()) {
            m_director->tlsConfig()->tlsKeyFile->setFileName(profile.tlsKeyFile);
        }
#else
        if (!profile.tlsPfxFile.isEmpty()) {
            m_director->tlsConfig()->tlsPfxFile->setFileName(profile.tlsPfxFile);
        }
#endif

        // Connect using profile data
        onDirectorConnect(profile.host,
                          profile.port,
                          profile.directorName,
                          profile.consoleName,
                          profile.password);

        m_statusLabel->setText(tr("Connecting to %1...").arg(profile.name));

        // Show wait cursor until all resources are loaded
        QApplication::setOverrideCursor(Qt::WaitCursor);
    }
}
void MainWindow::onDisconnectTriggered()
{
    // Restore cursor if still waiting
    while (QApplication::overrideCursor()) {
        QApplication::restoreOverrideCursor();
    }

    m_director->disconnect();
    m_statusLabel->setText("Getrennt");
}

void MainWindow::onToggleConnectionTriggered()
{
    if (m_director->isConnected()) {
        // Currently connected, so disconnect
        onDisconnectTriggered();
    } else {
        // Currently disconnected, so show connection dialog
        onConnectTriggered();
    }
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
    aboutDialog.setWindowTitle(tr("About Onesimus"));
    aboutDialog.setMinimumSize(500, 450);

    // Hauptlayout
    QVBoxLayout* mainLayout = new QVBoxLayout(&aboutDialog);
    mainLayout->setSpacing(15);

    // Horizontaler Container für Logo + Title
    QHBoxLayout* headerLayout = new QHBoxLayout();

    // Logo
    QLabel* logoLabel = new QLabel();
#if USE_BACULA
    logoLabel->setPixmap(QPixmap(":/images/logo_bacula.png").scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
#elif defined(USE_BAREOS)
    logoLabel->setPixmap(QPixmap(":/images/logo_bareos.png").scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
#endif
    headerLayout->addWidget(logoLabel);

    // Title and version
    QVBoxLayout* titleLayout = new QVBoxLayout();
    QLabel* titleLabel = new QLabel("<h1 style='margin:0;'>Onesimus</h1>");
    titleLayout->addWidget(titleLabel);

    QLabel* versionLabel = new QLabel(QString("<p style='color:#666; margin:0;'>Version %1</p>").arg(PROJECT_VERSION));
    titleLayout->addWidget(versionLabel);

#if USE_BACULA
    QLabel* subtitleLabel = new QLabel("<p style='margin:0;'>Modern Qt6 GUI for Bacula Backup</p>");
#elif defined(USE_BAREOS)
    QLabel* subtitleLabel = new QLabel("<p style='margin:0;'>Modern Qt6 GUI for Bareos Backup</p>");
#endif
    titleLayout->addWidget(subtitleLabel);
    titleLayout->addStretch();

    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout);

    // Separator
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);

    // Description
    QLabel* descLabel = new QLabel(
        "<p>Onesimus is a modern, user-friendly graphical interface for managing "
        "backup systems. It provides real-time job monitoring, client management, "
        "and storage administration through a clean, intuitive interface.</p>"
    );
    descLabel->setWordWrap(true);
    mainLayout->addWidget(descLabel);

    // Copyright and Author
    QLabel* copyrightLabel = new QLabel(
        "<p><b>Author:</b> Jörg Bernau &lt;joerg@bernau.family&gt;</p>"
        "<p><b>Copyright:</b> © 2026 Jörg Bernau. All rights reserved.</p>"
        "<p><b>Website:</b> <a href='https://github.com/Beerlesklopfer/Onesimus'>github.com/Beerlesklopfer/Onesimus</a></p>"
    );
    copyrightLabel->setWordWrap(true);
    copyrightLabel->setOpenExternalLinks(true);
    mainLayout->addWidget(copyrightLabel);

    // License
    QLabel* licenseLabel = new QLabel(
        "<p><b>License:</b> MIT License</p>"
        "<p style='font-size:9pt; color:#666;'>"
        "Permission is hereby granted, free of charge, to any person obtaining a copy "
        "of this software and associated documentation files, to deal in the Software "
        "without restriction, including without limitation the rights to use, copy, modify, "
        "merge, publish, distribute, sublicense, and/or sell copies of the Software.</p>"
        "<p style='font-size:9pt; color:#666;'>"
        "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED.</p>"
    );
    licenseLabel->setWordWrap(true);
    mainLayout->addWidget(licenseLabel);

    // Acknowledgments
    QLabel* ackLabel = new QLabel(
        "<p style='font-size:9pt;'><b>Built with:</b> Qt " + QString(qVersion()) + ", OpenSSL</p>"
    );
    ackLabel->setWordWrap(true);
    mainLayout->addWidget(ackLabel);

    mainLayout->addStretch();

    // Close Button
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    QPushButton* closeButton = new QPushButton(tr("Close"));
    closeButton->setDefault(true);
    QObject::connect(closeButton, &QPushButton::clicked, &aboutDialog, &QDialog::accept);
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);

    // Dialog anzeigen (modal)
    aboutDialog.exec();
}

void MainWindow::onDocumentationTriggered()
{
    // Open online documentation in default browser
    QString docUrl = "https://github.com/Beerlesklopfer/Onesimus/wiki";
    if (!QDesktopServices::openUrl(QUrl(docUrl))) {
        QMessageBox::information(this, tr("Documentation"),
            tr("Could not open browser. Please visit:\n%1").arg(docUrl));
    }
}

void MainWindow::onReportBugTriggered()
{
    // Open GitHub issues page in default browser
    QString issuesUrl = "https://github.com/Beerlesklopfer/Onesimus/issues";
    if (!QDesktopServices::openUrl(QUrl(issuesUrl))) {
        QMessageBox::information(this, tr("Report Bug"),
            tr("Could not open browser. Please visit:\n%1").arg(issuesUrl));
    }
}

void MainWindow::onKeyboardShortcutsTriggered()
{
    // Create dialog showing keyboard shortcuts
    QDialog shortcutsDialog(this);
    shortcutsDialog.setWindowTitle(tr("Keyboard Shortcuts"));
    shortcutsDialog.setMinimumSize(600, 500);

    QVBoxLayout *mainLayout = new QVBoxLayout(&shortcutsDialog);

    // Title
    QLabel *titleLabel = new QLabel("<h2>" + tr("Keyboard Shortcuts") + "</h2>");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Create tab widget for different categories
    QTabWidget *tabWidget = new QTabWidget();

    // File shortcuts
    QWidget *fileTab = new QWidget();
    QVBoxLayout *fileLayout = new QVBoxLayout(fileTab);
    QTextEdit *fileText = new QTextEdit();
    fileText->setReadOnly(true);
    fileText->setHtml(
        "<table width='100%' cellpadding='5'>"
        "<tr><td width='50%'><b>" + tr("Connect") + "</b></td><td>Ctrl+O</td></tr>"
        "<tr><td><b>" + tr("Reconnect") + "</b></td><td>Ctrl+R</td></tr>"
        "<tr><td><b>" + tr("Refresh") + "</b></td><td>F5</td></tr>"
        "<tr><td><b>" + tr("Settings") + "</b></td><td>Ctrl+,</td></tr>"
        "<tr><td><b>" + tr("Exit") + "</b></td><td>Ctrl+Q</td></tr>"
        "</table>"
    );
    fileLayout->addWidget(fileText);
    tabWidget->addTab(fileTab, tr("File"));

    // Edit shortcuts
    QWidget *editTab = new QWidget();
    QVBoxLayout *editLayout = new QVBoxLayout(editTab);
    QTextEdit *editText = new QTextEdit();
    editText->setReadOnly(true);
    editText->setHtml(
        "<table width='100%' cellpadding='5'>"
        "<tr><td width='50%'><b>" + tr("Copy") + "</b></td><td>Ctrl+C</td></tr>"
        "<tr><td><b>" + tr("Select All") + "</b></td><td>Ctrl+A</td></tr>"
        "<tr><td><b>" + tr("Find") + "</b></td><td>Ctrl+F</td></tr>"
        "</table>"
    );
    editLayout->addWidget(editText);
    tabWidget->addTab(editTab, tr("Edit"));

    // View shortcuts
    QWidget *viewTab = new QWidget();
    QVBoxLayout *viewLayout = new QVBoxLayout(viewTab);
    QTextEdit *viewText = new QTextEdit();
    viewText->setReadOnly(true);
    viewText->setHtml(
        "<table width='100%' cellpadding='5'>"
        "<tr><td width='50%'><b>" + tr("Show Statistics") + "</b></td><td>Ctrl+Shift+S</td></tr>"
        "<tr><td><b>" + tr("Show Job Log") + "</b></td><td>Ctrl+Shift+L</td></tr>"
        "<tr><td><b>" + tr("Toggle Theme") + "</b></td><td>Ctrl+Shift+T</td></tr>"
        "</table>"
    );
    viewLayout->addWidget(viewText);
    tabWidget->addTab(viewTab, tr("View"));

    // Jobs shortcuts
    QWidget *jobsTab = new QWidget();
    QVBoxLayout *jobsLayout = new QVBoxLayout(jobsTab);
    QTextEdit *jobsText = new QTextEdit();
    jobsText->setReadOnly(true);
    jobsText->setHtml(
        "<table width='100%' cellpadding='5'>"
        "<tr><td width='50%'><b>" + tr("Refresh Jobs") + "</b></td><td>Ctrl+Shift+J</td></tr>"
        "<tr><td><b>" + tr("Refresh Clients") + "</b></td><td>Ctrl+Shift+C</td></tr>"
        "<tr><td><b>" + tr("Refresh Storage") + "</b></td><td>Ctrl+Shift+V</td></tr>"
        "<tr><td><b>" + tr("Refresh Schedules") + "</b></td><td>Ctrl+Shift+D</td></tr>"
        "</table>"
    );
    jobsLayout->addWidget(jobsText);
    tabWidget->addTab(jobsTab, tr("Data"));

    // Help shortcuts
    QWidget *helpTab = new QWidget();
    QVBoxLayout *helpLayout = new QVBoxLayout(helpTab);
    QTextEdit *helpText = new QTextEdit();
    helpText->setReadOnly(true);
    helpText->setHtml(
        "<table width='100%' cellpadding='5'>"
        "<tr><td width='50%'><b>" + tr("Help Contents") + "</b></td><td>F1</td></tr>"
        "<tr><td><b>" + tr("Keyboard Shortcuts") + "</b></td><td>Ctrl+?</td></tr>"
        "</table>"
    );
    helpLayout->addWidget(helpText);
    tabWidget->addTab(helpTab, tr("Help"));

    mainLayout->addWidget(tabWidget);

    // Close button
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, &shortcutsDialog, &QDialog::accept);
    mainLayout->addWidget(buttonBox);

    shortcutsDialog.exec();
}

void MainWindow::onSettingsTriggered()
{
    // Get available levels from JobWidget's level model (if connected)
    QList<QPair<QString, QString>> availableLevels;
    if (m_jobWidget && m_jobWidget->levelModel()) {
        QStringList levelNames = m_jobWidget->levelModel()->levelDescriptions();
        for (const QString &name : levelNames) {
            availableLevels.append({name, name});
        }
    }

    SettingsDialog dialog(m_director, availableLevels, this);
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
    m_toggleJobLogAction->setEnabled(connected);  // ✅ Job Log nur bei Verbindung
    m_tabWidget->setEnabled(connected);

    // Update toggle connection button icon and tooltip
    if (connected) {
        m_toggleConnectionAction->setIcon(QIcon(":/icons/icons/disconnect.png"));
        m_toggleConnectionAction->setToolTip(tr("Disconnect from Director"));
    } else {
        m_toggleConnectionAction->setIcon(QIcon(":/icons/icons/connect.png"));
        m_toggleConnectionAction->setToolTip(tr("Connect to Director"));
    }

    // Update job menu actions
    m_refreshJobsAction->setEnabled(connected);
    m_exportJobsJsonAction->setEnabled(connected);
    m_exportJobsCsvAction->setEnabled(connected);

    // Job control actions
    m_runJobAction->setEnabled(connected);  // Run job can be used without selection (enter job name)
    // Cancel and Details require job selection, so they stay disabled until selection changes
    if (!connected) {
        m_cancelJobAction->setEnabled(false);
        m_jobDetailsAction->setEnabled(false);
    }

    // Update client menu actions
    m_refreshClientsAction->setEnabled(connected);

    // Update storage menu actions
    m_refreshStorageAction->setEnabled(connected);

    // Update schedule menu actions
    m_refreshSchedulesAction->setEnabled(connected);

    // Update tools menu actions
    m_cleanupDatabaseAction->setEnabled(connected);

    // Update widget connection states
    m_jobWidget->setConnectionState(connected);
    m_storageWidget->setConnectionState(connected);
    m_scheduleWidget->setConnectionState(connected);

    if (connected) {
        // ✅ Jetzt erst statusMessage Signal verbinden (nach erfolgreicher Auth)
        m_statusMessageConnection = connect(m_director, &BDirector::statusMessage,
                                             this, [this](const QString &msg) {
            m_statusLabel->setText(msg);
        });

        // Show version only with connected color from settings
        m_connectionLabel->setText(msg);
        m_connectionLabel->setProperty("connected", true);
        QColor connectedColor = BSettings::instance().statusBarConnectedColor();
        m_connectionLabel->setStyleSheet(QString("color: %1; font-weight: bold;").arg(connectedColor.name()));
        m_statusLabel->setText(tr("Connected - Loading data..."));

        // Restore saved Statistics DockWidget state
        bool statsVisible = BSettings::instance().statisticsWidgetVisible();
        m_statisticsDock->setVisible(statsVisible);
        m_toggleStatisticsAction->setChecked(statsVisible);
        m_toggleStatisticsButton->setText(statsVisible ? tr("Statistics ▼") : tr("Statistics ▶"));

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

        // Note: Resource loading and initial refresh is now handled by the state machine.
        // The allResourcesLoaded() signal will trigger onRefreshAll() when ready.
    } else {
        // ✅ statusMessage Signal trennen bei Disconnect
        QObject::disconnect(m_statusMessageConnection);

        // m_connectionLabel zeigt nur Verbindungsstatus, keine Fehlermeldungen
        // (Fehlermeldungen werden über MessageBox angezeigt)
        m_connectionLabel->setText(tr("Not connected"));
        m_connectionLabel->setProperty("connected", false);
        QColor disconnectedColor = BSettings::instance().statusBarDisconnectedColor();
        m_connectionLabel->setStyleSheet(QString("color: %1; font-weight: bold;").arg(disconnectedColor.name()));
        m_statusLabel->setText("");

        // Hide Statistics DockWidget on disconnect
        m_statisticsDock->setVisible(false);
        m_toggleStatisticsAction->setChecked(false);
        m_toggleStatisticsButton->setText(tr("Statistics ▶"));

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
    // Restore cursor on error
    QApplication::restoreOverrideCursor();

    QMessageBox::critical(this, tr("Connection Error"), error);
    m_statusLabel->setText(tr("Error: ") + error);
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
    onSendCommand(BDirector::Command::DotFilesets, "");
    onSendCommand(BDirector::Command::DotStorages, "");
    onSendCommand(BDirector::Command::DotPools, "");

    // m_director->listVolumes();

    // Reset status after short delay
    QTimer::singleShot(1000, this, [this]() {
        if (m_director->isConnected()) {
            m_statusLabel->setText(tr("Ready"));
        }
    });
}

void MainWindow::onConnectLastUsed()
{
    BConnectionProfile profile = BSettings::instance().lastUsedProfile();
    if (!profile.isValid()) {
        QMessageBox::information(this, tr("No Saved Connection"),
            tr("No previous connection found. Please use 'Connect' to establish a new connection."));
        return;
    }

    m_statusLabel->setText(tr("Connecting to %1...").arg(profile.name));
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

void MainWindow::onDirectorConnect(const QString &host, int port, const QString &directorName,
                                   const QString &consoleName, const QString &password)
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
                              Q_ARG(QString, consoleName),
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

    // Migrate old settings if needed
    settings.migrateOldConnectionSettings();

    // Check if auto-connect is enabled
    if (!settings.connectionAutoConnect()) {
        m_connectLastAction->setEnabled(!settings.lastUsedProfileId().isEmpty());
        return;
    }

    // Get last used profile
    BConnectionProfile profile = settings.lastUsedProfile();

    if (!profile.isValid() || profile.host.isEmpty() || profile.password.isEmpty()) {
#ifdef IS_DEVELOPER
        qDebug() << "No valid last-used profile - skipping auto-connect";
#endif
        m_connectLastAction->setEnabled(!settings.lastUsedProfileId().isEmpty());
        return;
    }

    // Configure TLS
    m_director->tlsConfig()->tlsEnable = profile.tlsEnabled;
    m_director->tlsConfig()->tlsRequire = profile.tlsEnabled;
    m_director->tlsConfig()->tlsPSKEnable = profile.tlsUsePSK;
    m_director->tlsConfig()->tlsVerifyPeer = profile.tlsVerifyPeer;

#ifndef Q_OS_WINDOWS
    if (!profile.tlsCaCertFile.isEmpty()) {
        m_director->tlsConfig()->tlsCaCertFile->setFileName(profile.tlsCaCertFile);
    }
    if (!profile.tlsCertFile.isEmpty()) {
        m_director->tlsConfig()->tlsCertFile->setFileName(profile.tlsCertFile);
    }
    if (!profile.tlsKeyFile.isEmpty()) {
        m_director->tlsConfig()->tlsKeyFile->setFileName(profile.tlsKeyFile);
    }
#else
    if (!profile.tlsPfxFile.isEmpty()) {
        m_director->tlsConfig()->tlsPfxFile->setFileName(profile.tlsPfxFile);
    }
#endif

#ifdef IS_DEVELOPER
    qDebug() << "Auto-connecting to last used profile:" << profile.name
             << "(" << profile.host << ":" << profile.port << ")";
#endif

    m_director->setTLSConfig(*m_director->tlsConfig());
    onDirectorConnect(profile.host, profile.port, profile.directorName,
                      profile.consoleName, profile.password);
    m_statusLabel->setText(tr("Auto-connecting to %1...").arg(profile.name));

    // Show wait cursor until all resources are loaded
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Update "Reconnect" button status
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
        // Info: User can use filters via the toggle button in the Job view
        QMessageBox::information(this, tr("Search"),
            tr("Use the filter options in the Job view\n"
               "to search for specific jobs.\n\n"
               "Available filters:\n"
               "• Job Name\n"
               "• Client Name\n"
               "• Status (Success, Warning, Failed)\n"
               "• Level (Full, Incremental, Differential)\n"
               "• Date Range"));
    } else if (currentWidget == m_clientWidget) {
        QMessageBox::information(this, tr("Search"),
            tr("Use the filter combo box in the Client view\n"
               "to search for specific clients."));
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

void MainWindow::onToggleTheme()
{
    // Get current theme
    QString currentTheme = BSettings::instance().appearanceTheme();

    // Toggle theme
    QString newTheme;

    if (currentTheme == "dark") {
        newTheme = "light";
        m_toggleThemeAction->setIcon(QIcon(":/icons/icons/moon.png"));
        m_statusLabel->setText(tr("Switched to Light Theme"));
    } else {
        newTheme = "dark";
        m_toggleThemeAction->setIcon(QIcon(":/icons/icons/sun.png"));
        m_statusLabel->setText(tr("Switched to Dark Theme"));
    }

    // Apply new theme
    applyTheme(newTheme);

    // Save new theme to settings
    BSettings::instance().setAppearanceTheme(newTheme);
}

void MainWindow::onExportSettingsTriggered()
{
    if (!m_director->isConnected()) {
        QMessageBox::warning(this, tr("Not Connected"), tr("Please connect to the Director first."));
        return;
    }

    // Export all jobs to JSON
    m_jobWidget->tableView()->exportToJson(false);
    m_statusLabel->setText("Jobs als JSON exportiert");
}

void MainWindow::onImportSettingsTriggered()
{
    if (!m_director->isConnected()) {
        QMessageBox::warning(this, tr("Not Connected"), tr("Please connect to the Director first."));
        return;
    }

    // Export all jobs to CSV
    m_jobWidget->tableView()->exportToCsv(false);
    m_statusLabel->setText("Jobs als CSV exportiert");
}

void MainWindow::onCleanupDatabase()
{
    if (!m_director->isConnected()) {
        QMessageBox::warning(this, tr("Not Connected"),
                           tr("Please connect to Bareos Director first."));
        return;
    }

    // Ensure job data is loaded before opening dialog
    BJobsModel *jobsModel = m_jobWidget->tableView()->jobsModel();
    if (jobsModel->allJobs().isEmpty()) {
        // Trigger refresh and wait a moment for data to load
        m_jobWidget->triggerRefresh();
        m_statusLabel->setText(tr("Loading job data..."));
    }

    BCleanupDialog *dialog = new BCleanupDialog(m_director, jobsModel, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Remove deleted jobs directly from model (faster than full refresh)
    connect(dialog, &BCleanupDialog::cleanupCompleted, jobsModel, &BJobsModel::removeJobsByIds);

    dialog->exec();
}
