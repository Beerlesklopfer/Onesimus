#include "bmainwindow.h"
#include "ui_bmainwindow.h"
#include "blogging.h"
#include "jobs/bjobwidget.h"
#include "jobs/bjobsstatisticswidget.h"
#include "clients/bclientswidget.h"
#include "clients/bclientsmodel.h"
#include "clients/bnewclientdialog.h"
#include "jobs/bfilesetwizard.h"
#include "jobs/bjobwizard.h"
#include "schedules/bschedulewizard.h"
#include "config/bdirectiveschema.h"
#include "director/bresourcedialog.h"
#include "director/bresourcewidgets.h"
#include "models/bresourcemodels.h"
#include "storagewidget.h"
#include "schedules/bschedulewidget.h"
// Messages widget is now inside BJobWidget
#include "config/bsettingsdialog.h"
#include "bcleanupdialog.h"
#include "bconnectionwizard.h"
#include "director/bconfigimportdialog.h"
#include "db/bdatabase.h"
#include "config/bsettings.h"
#include "bconnectionprofile.h"
#include "version.h"

#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QCryptographicHash>
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
#include <QRegularExpression>
#include <QScrollArea>
#include <QSettings>
#include <QSpinBox>
#include <QUuid>
#include <QStyle>
#include <QTabWidget>
#include <QTextEdit>
#include <QTextStream>
#include <QToolBar>
#include <QUrl>
#include <QVBoxLayout>
#include <QMenuBar>

BMainWindow::BMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BMainWindow)
    , m_director(nullptr)
    , m_database(nullptr)
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

    // Initialize database (for storing Directors, Consoles, etc.)
    m_database = new BDatabase(this);
    if (!m_database->initialize()) {
        APP_WARNING << "Database initialization failed: " << m_database->lastError();
    } else {
        APP_DEBUG << "Database initialized, version: " << m_database->currentVersion();
    }

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

    connect(m_director, &BDirector::authentificationSucceeded,  this, &BMainWindow::onAuthentificationSucceeded);

    connect(m_director, &BDirector::protocolError, this, &BMainWindow::onConnectionError);

    // ✅ Handle non-JSON command responses (for debugging)
    connect(m_director, &BDirector::textResult, this, [this](BDirector::Command cmd, const QString &response) {
        Q_UNUSED(cmd)
        Q_UNUSED(response)
#ifdef IS_DEVELOPER
        BLOG_DEBUG() << "MainWindow: Text response for cmd:" << static_cast<int>(cmd) << "Response:" << response.left(100);
#endif
        // Note: API mode confirmation and resource loading is now handled
        // by the BareosDirector state machine automatically
    });

    // ✅ Connect to state machine signals
    connect(m_director, &BDirector::connectionStateChanged, this,
            [this](BDirector::ConnectionState oldState, BDirector::ConnectionState newState) {
        Q_UNUSED(oldState)
#ifdef IS_DEVELOPER
        BLOG_DEBUG() << "MainWindow: Connection state changed:" << static_cast<int>(oldState)
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
        BLOG_DEBUG() << "MainWindow: All resources loaded - models populated via typed signals";
#endif
        // Restore cursor - loading complete
        QApplication::restoreOverrideCursor();

        // Resource dot-commands (filesets, storages, pools, levels, jobs, clients, schedule)
        // are already populated via typed signals from the state machine responses.
        // Only send non-resource commands that aren't part of the state machine.
        m_jobWidget->triggerRefresh();            // ListJobTotals / ListJobs
        m_clientWidget->triggerRefresh();          // ListClients
        m_scheduleWidget->triggerRefresh();        // DotSchedule + ShowSchedules
        onSendCommand(BDirector::Command::ShowClients, "");
        onSendCommand(BDirector::Command::ShowJobDefs, "");  // Populate jobdefs model
        onSendCommand(BDirector::Command::ListBackups, "");
        onSendCommand(BDirector::Command::Messages, "");

        statusBar()->showMessage(tr("Ready"));
    });

    // ✅ Route JSON responses to appropriate widgets based on Command enum
    connect(m_director, &BDirector::jsonResult, this, [this](BDirector::Command cmd, const QString &jsonData) {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            BLOG_WARNING() << "MainWindow: Failed to parse JSON response:" << parseError.errorString();
            return;
        }

        if (!doc.isObject()) {
            BLOG_WARNING() << "MainWindow: JSON response is not an object";
            return;
        }

        QJsonObject root = doc.object();
        QJsonObject result = root["result"].toObject();

        switch (cmd) {
        // Resource dot-commands handled by typed signals (Phase 6)
        case BDirector::Command::DotLevels:
        case BDirector::Command::DotFilesets:
        case BDirector::Command::DotStorages:
        case BDirector::Command::DotPools:
        case BDirector::Command::DotSchedule:
        case BDirector::Command::DotJobs:
        case BDirector::Command::DotClients:
        case BDirector::Command::ListClients:
            break;  // Routed via typed signals → model/widget slots

        case BDirector::Command::ListJobTotals:
            m_jobWidget->processJobTotalsResponse(jsonData);
            break;

        case BDirector::Command::ListJobs:
        case BDirector::Command::ListJobsLast: {
            m_jobWidget->processJsonResponse(jsonData);
            // Also enrich client widget with job data (for online status + total bytes)
            QJsonArray jobsArr = result["jobs"].toArray();
            if (!jobsArr.isEmpty())
                m_clientWidget->model()->enrichWithJobData(jobsArr);
            break;
        }

        case BDirector::Command::ShowClient:
        case BDirector::Command::ShowClients:
            m_clientWidget->model()->enrichWithShowClientData(jsonData);
            break;

        case BDirector::Command::ListBackups: {
            QJsonArray backupsArr = result["backups"].toArray();
            if (!backupsArr.isEmpty())
                m_clientWidget->model()->enrichWithJobData(backupsArr);
            break;
        }

        case BDirector::Command::ListVolumes:
            m_storageWidget->processJsonResponse(jsonData);
            break;

        case BDirector::Command::DotMessages:
            m_jobWidget->processMessagesResponse(jsonData);
            break;

        case BDirector::Command::StatusClient:
            // StatusClient currently disabled (Bareos JSON API limitation)
            // When re-enabled, needs command queue (Phase 5) to access client name
            break;

        case BDirector::Command::Configure:
        case BDirector::Command::Reload:
            // Handled by the wizard's own signal handler
            break;

        case BDirector::Command::ListJobId:
            // Handled by BJobWidget/BJobLogDialog/BJobDetailsDialog
            break;

        default:
#ifdef IS_DEVELOPER
            BLOG_DEBUG() << "MainWindow: Unrouted JSON response for cmd:" << static_cast<int>(cmd);
            BLOG_DEBUG() << "  Result keys:" << result.keys();
#endif
            break;
        }
    });

    // Handle command errors — mark clients as OFFLINE when "status client" fails
    connect(m_director, &BDirector::commandError, this, [this](const QString &command, const QString &error) {
        if (command.startsWith("status client=")) {
            QString clientName = command.mid(14);
            CLIENTS_DEBUG << "Client " << clientName << " unreachable → OFFLINE (" << error << ")";
            m_clientWidget->model()->setClientOnlineStatus(clientName, BClientsModel::STATUS_OFFLINE);
        }
    });

    // Connect Jobwidget signals
    connect(m_jobWidget, &BJobWidget::sendCommand, this, &BMainWindow::onSendCommand);

    QObject::connect(m_jobWidget, &BJobWidget::statusMessageChanged,
            m_statusLabel, &QLabel::setText);

    // Connect BClientsWidget signals
    connect(m_clientWidget, &BClientsWidget::sendCommand, this, &BMainWindow::onSendCommand);

    connect(m_clientWidget, &BClientsWidget::statusMessageChanged,
            m_statusLabel, &QLabel::setText);

    // Auto-refresh from client widget checkbox
    connect(m_clientWidget, &BClientsWidget::autoRefreshChanged, this, [this](bool enabled) {
        BSettings &settings = BSettings::instance();
        int interval = settings.behaviorRefreshInterval();

        if (enabled && m_director->isConnected()) {
            m_autoRefreshTimer->start(interval * 1000);
            m_statusLabel->setText(tr("Auto-refresh enabled (%1s)").arg(interval));
        } else {
            m_autoRefreshTimer->stop();
            if (!enabled && m_director->isConnected()) {
                m_statusLabel->setText(tr("Auto-refresh disabled"));
            }
        }
    });

    // Connect StorageWidget signals
    connect(m_storageWidget, &StorageWidget::sendCommand, this, &BMainWindow::onSendCommand);

    connect(m_storageWidget, &StorageWidget::statusMessageChanged,
            m_statusLabel, &QLabel::setText);

    // Connect ScheduleWidget signals
    connect(m_scheduleWidget, &BScheduleWidget::sendCommand, this, &BMainWindow::onSendCommand);

    connect(m_scheduleWidget, &BScheduleWidget::statusMessageChanged,
            m_statusLabel, &QLabel::setText);

    // ✅ Typed resource signals → direct model/widget connections (Phase 6)
    // These bypass the jsonResult routing switch entirely.
    connect(m_director, &BDirector::dotFilesetsResult,
            m_jobWidget, &BJobWidget::processDotFilesetsResponse);
    connect(m_director, &BDirector::dotStoragesResult,
            m_jobWidget, &BJobWidget::processDotStoragesResponse);
    connect(m_director, &BDirector::dotPoolsResult,
            m_jobWidget, &BJobWidget::processDotPoolsResponse);
    connect(m_director, &BDirector::dotLevelsResult,
            m_jobWidget, &BJobWidget::processDotLevelsResponse);
    connect(m_director, &BDirector::dotJobsResult,
            m_jobWidget, &BJobWidget::processDotJobsResponse);
    connect(m_director, &BDirector::dotClientsResult,
            m_jobWidget, &BJobWidget::processDotClientsResponse);
    connect(m_director, &BDirector::dotScheduleResult,
            m_scheduleWidget, &BScheduleWidget::processDotScheduleResponse);
    connect(m_director, &BDirector::showSchedulesResult,
            m_scheduleWidget, &BScheduleWidget::processShowSchedulesResponse);
    connect(m_director, &BDirector::listClientsResult,
            m_clientWidget, &BClientsWidget::processJsonResponse);
    connect(m_director, &BDirector::showJobsResult,
            m_jobWidget->jobConfigModel(), &BJobConfigModel::parseShowJobs);
    connect(m_director, &BDirector::showJobsResult,
            m_scheduleWidget, &BScheduleWidget::processShowJobsResponse);
    connect(m_director, &BDirector::showJobDefsResult,
            m_jobWidget->jobConfigModel(), &BJobConfigModel::parseShowJobDefs);

    // Pass job config model to schedule widget for job→schedule cross-referencing
    m_scheduleWidget->setJobConfigModel(m_jobWidget->jobConfigModel());
    connect(m_director, &BDirector::dotCatalogsResult,
            m_jobWidget, &BJobWidget::processDotCatalogsResponse);

    // Setup auto-refresh timer
    m_autoRefreshTimer = new QTimer(this);
    connect(m_autoRefreshTimer, &QTimer::timeout, this, &BMainWindow::onRefreshAll);

    // Connect to BSettings signals for auto-refresh
    connect(&BSettings::instance(), &BSettings::autoRefreshSettingsChanged,
            this, &BMainWindow::onAutoRefreshSettingsChanged);

    // Initialize auto-refresh from settings
    BSettings& settings = BSettings::instance();
    onAutoRefreshSettingsChanged(settings.behaviorAutoRefresh(), settings.behaviorRefreshInterval());

    onAuthentificationSucceeded(false, tr("Nicht verbunden"));
    loadAndConnectLastUsed();

    // Apply saved theme from settings
    QString savedTheme = BSettings::instance().appearanceTheme();
    applyTheme(savedTheme);
}

BMainWindow::~BMainWindow()
{
    BSettings::instance().setMainWindowActiveTab(m_tabWidget->currentIndex());
    delete ui;
}

void BMainWindow::applyTheme(const QString &themeName)
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
        BLOG_DEBUG() << "Applied theme:" << themeName << "from" << themePath;
#endif
    } else {
        BLOG_WARNING() << "Could not load theme file:" << themePath;
    }
}

void BMainWindow::setupUI()
{
    // ========================================================================
    // Main Tab Widget (Jobs, Clients, Storage, Schedules)
    // ========================================================================
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    m_jobWidget = new BJobWidget(m_director, this);
    m_tabWidget->addTab(m_jobWidget, "Jobs");

    // Client-Widget
    m_clientWidget = new BClientsWidget(m_director, this);
    m_tabWidget->addTab(m_clientWidget, "Clients");

    // Storage-Widget
    m_storageWidget = new StorageWidget(m_director, this);
    m_tabWidget->addTab(m_storageWidget, "Storage/Volumes");

    // Schedule-Widget
    m_scheduleWidget = new BScheduleWidget(this);
    m_scheduleWidget->setDirector(m_director);
    m_tabWidget->addTab(m_scheduleWidget, "Schedules");

    int savedTab = BSettings::instance().mainWindowActiveTab();
    if (savedTab >= 0 && savedTab < m_tabWidget->count()) {
        m_tabWidget->setCurrentIndex(savedTab);
    }
    connect(m_tabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        BSettings::instance().setMainWindowActiveTab(index);

        // Auto-refresh when switching to Schedules tab
        if (m_tabWidget->widget(index) == m_scheduleWidget && m_director && m_director->isConnected()) {
            m_scheduleWidget->triggerRefresh();
        }
    });

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

void BMainWindow::createActions()
{
    m_connectAction = new QAction(tr("Connect"), this);
    m_connectAction->setIcon(QIcon(":/icons/icons/connect.svg"));
    m_connectAction->setShortcut(QKeySequence("Ctrl+O"));
    connect(m_connectAction, &QAction::triggered, this, &BMainWindow::onConnectTriggered);

    m_connectLastAction = new QAction(tr("Reconnect"), this);
    m_connectLastAction->setIcon(QIcon(":/icons/icons/reconnect.svg"));
    m_connectLastAction->setShortcut(QKeySequence("Ctrl+R"));
    m_connectLastAction->setEnabled(!BSettings::instance().lastUsedProfileId().isEmpty());
    connect(m_connectLastAction, &QAction::triggered, this, &BMainWindow::onConnectLastUsed);

    m_disconnectAction = new QAction(tr("Disconnect"), this);
    m_disconnectAction->setIcon(QIcon(":/icons/icons/disconnect.svg"));
    m_disconnectAction->setEnabled(false);
    connect(m_disconnectAction, &QAction::triggered, this, &BMainWindow::onDisconnectTriggered);

    // Toggle Connection Action (for toolbar)
    m_toggleConnectionAction = new QAction(tr("Connect"), this);
    m_toggleConnectionAction->setIcon(QIcon(":/icons/icons/connect.svg"));
    m_toggleConnectionAction->setToolTip(tr("Connect to Director"));
    connect(m_toggleConnectionAction, &QAction::triggered, this, &BMainWindow::onToggleConnectionTriggered);

    // Reconnect Action (for toolbar)
    m_reconnectAction = new QAction(tr("Reconnect"), this);
    m_reconnectAction->setIcon(QIcon(":/icons/icons/reconnect.svg"));
    m_reconnectAction->setToolTip(tr("Reconnect to last used connection"));
    m_reconnectAction->setEnabled(!BSettings::instance().lastUsedProfileId().isEmpty());
    connect(m_reconnectAction, &QAction::triggered, this, &BMainWindow::onConnectLastUsed);

    m_refreshAction = new QAction(tr("Refresh"), this);
    m_refreshAction->setIcon(QIcon(":/icons/icons/refresh.svg"));
    m_refreshAction->setShortcut(QKeySequence("F5"));
    m_refreshAction->setEnabled(false);
    connect(m_refreshAction, &QAction::triggered, this, &BMainWindow::onRefreshAll);

    m_settingsAction = new QAction(tr("Settings"), this);
    m_settingsAction->setIcon(QIcon(":/icons/icons/settings.svg"));
    m_settingsAction->setShortcut(QKeySequence("Ctrl+,"));
    connect(m_settingsAction, &QAction::triggered, this, &BMainWindow::onSettingsTriggered);

    m_exitAction = new QAction(tr("Exit"), this);
    m_exitAction->setIcon(QIcon(":/icons/icons/exit.svg"));
    m_exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);

    m_aboutAction = new QAction(tr("About Onesimus"), this);
    m_aboutAction->setIcon(QIcon(":/icons/icons/onesimus.svg"));
    connect(m_aboutAction, &QAction::triggered, this, &BMainWindow::onAboutTriggered);

    m_aboutQtAction = new QAction(tr("About Qt"), this);
    m_aboutQtAction->setIcon(QIcon(":/icons/icons/info.svg"));
    connect(m_aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

    m_documentationAction = new QAction(tr("Online Documentation"), this);
    m_documentationAction->setIcon(QIcon(":/icons/icons/help.svg"));
    m_documentationAction->setShortcut(QKeySequence::HelpContents);
    connect(m_documentationAction, &QAction::triggered, this, &BMainWindow::onDocumentationTriggered);

    m_reportBugAction = new QAction(tr("Report a Bug..."), this);
    m_reportBugAction->setIcon(QIcon(":/icons/icons/info.svg"));
    connect(m_reportBugAction, &QAction::triggered, this, &BMainWindow::onReportBugTriggered);

    m_keyboardShortcutsAction = new QAction(tr("Keyboard Shortcuts"), this);
    m_keyboardShortcutsAction->setIcon(QIcon(":/icons/icons/help.svg"));
    m_keyboardShortcutsAction->setShortcut(QKeySequence("Ctrl+?"));
    connect(m_keyboardShortcutsAction, &QAction::triggered, this, &BMainWindow::onKeyboardShortcutsTriggered);

    m_myPermissionsAction = new QAction(tr("My Permissions"), this);
    m_myPermissionsAction->setIcon(QIcon(":/icons/icons/info.svg"));
    m_myPermissionsAction->setEnabled(false);
    connect(m_myPermissionsAction, &QAction::triggered, this, &BMainWindow::onMyPermissionsTriggered);

    // Edit Actions
    m_copyAction = new QAction(tr("Copy"), this);
    m_copyAction->setIcon(QIcon(":/icons/icons/copy.svg"));
    m_copyAction->setShortcut(QKeySequence::Copy);
    m_copyAction->setEnabled(false);
    connect(m_copyAction, &QAction::triggered, this, &BMainWindow::onCopyTriggered);

    m_selectAllAction = new QAction(tr("Select All"), this);
    m_selectAllAction->setIcon(QIcon(":/icons/icons/copy.svg"));
    m_selectAllAction->setShortcut(QKeySequence::SelectAll);
    m_selectAllAction->setEnabled(false);
    connect(m_selectAllAction, &QAction::triggered, this, &BMainWindow::onSelectAllTriggered);

    m_clearSelectionAction = new QAction(tr("Clear Selection"), this);
    m_clearSelectionAction->setIcon(QIcon(":/icons/icons/cancel.svg"));
    m_clearSelectionAction->setEnabled(false);
    connect(m_clearSelectionAction, &QAction::triggered, this, &BMainWindow::onClearSelectionTriggered);

    m_findAction = new QAction(tr("Find..."), this);
    m_findAction->setIcon(QIcon(":/icons/icons/search.svg"));
    m_findAction->setShortcut(QKeySequence::Find);
    m_findAction->setEnabled(false);
    connect(m_findAction, &QAction::triggered, this, &BMainWindow::onFindTriggered);

    // View Actions
    m_toggleStatisticsAction = new QAction(tr("Show Statistics"), this);
    m_toggleStatisticsAction->setCheckable(true);
    m_toggleStatisticsAction->setChecked(false);  // Hidden by default (until connected)
    m_toggleStatisticsAction->setEnabled(false);  // Only active when connected
    m_toggleStatisticsAction->setIcon(QIcon(":/icons/icons/info.svg"));
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
    m_toggleJobLogAction->setIcon(QIcon(":/icons/icons/jobs.svg"));
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
        m_toggleThemeAction->setIcon(QIcon(":/icons/icons/sun.svg"));
    } else {
        m_toggleThemeAction->setIcon(QIcon(":/icons/icons/moon.svg"));
    }

    connect(m_toggleThemeAction, &QAction::triggered, this, &BMainWindow::onToggleTheme);

    // Jobs Actions
    m_runJobAction = new QAction(tr("Run Job"), this);
    m_runJobAction->setIcon(QIcon(":/icons/icons/run.svg"));
    m_runJobAction->setEnabled(false);
    connect(m_runJobAction, &QAction::triggered, m_jobWidget, &BJobWidget::triggerRunJob);

    m_cancelJobAction = new QAction(tr("Cancel Job"), this);
    m_cancelJobAction->setIcon(QIcon(":/icons/icons/cancel.svg"));
    m_cancelJobAction->setEnabled(false);
    connect(m_cancelJobAction, &QAction::triggered, m_jobWidget, &BJobWidget::triggerCancelJob);

    m_jobDetailsAction = new QAction(tr("Show Details"), this);
    m_jobDetailsAction->setIcon(QIcon(":/icons/icons/info.svg"));
    m_jobDetailsAction->setEnabled(false);
    connect(m_jobDetailsAction, &QAction::triggered, m_jobWidget, &BJobWidget::triggerShowDetails);

    m_refreshJobsAction = new QAction(tr("Refresh Jobs"), this);
    m_refreshJobsAction->setIcon(QIcon(":/icons/icons/refresh.svg"));
    m_refreshJobsAction->setShortcut(QKeySequence("Ctrl+Shift+J"));
    m_refreshJobsAction->setEnabled(false);
    connect(m_refreshJobsAction, &QAction::triggered, m_jobWidget, &BJobWidget::triggerRefresh);

    m_exportJobsJsonAction = new QAction(tr("Export Jobs as JSON..."), this);
    m_exportJobsJsonAction->setIcon(QIcon(":/icons/icons/export.svg"));
    m_exportJobsJsonAction->setEnabled(false);
    connect(m_exportJobsJsonAction, &QAction::triggered, this, &BMainWindow::onExportSettingsTriggered);

    m_exportJobsCsvAction = new QAction(tr("Export Jobs as CSV..."), this);
    m_exportJobsCsvAction->setIcon(QIcon(":/icons/icons/export.svg"));
    m_exportJobsCsvAction->setEnabled(false);
    connect(m_exportJobsCsvAction, &QAction::triggered, this, &BMainWindow::onImportSettingsTriggered);

    // Modify Jobs submenu actions
    m_addJobAction = new QAction(tr("Add Job..."), this);
    m_addJobAction->setIcon(QIcon(":/icons/icons/add.svg"));
    m_addJobAction->setEnabled(false);
    connect(m_addJobAction, &QAction::triggered, this, [this]() {
        BJobWizard wizard(BJobWizard::JobType, m_director, this);
        QMap<QString, QStringList> refData;
        refData["Client"] = m_jobWidget->clientNames();
        refData["FileSet"] = m_jobWidget->filesetNames();
        refData["Storage"] = m_jobWidget->storageNames();
        refData["Pool"] = m_jobWidget->poolNames();
        refData["Schedule"] = QStringList();
        refData["Messages"] = QStringList();
        refData["Catalog"] = m_jobWidget->catalogNames();
        refData["JobDefs"] = m_jobWidget->jobDefsNames();
        refData["Job"] = m_jobWidget->jobNames();
        wizard.setReferenceData(refData);
        wizard.exec();
    });

    m_editJobAction = new QAction(tr("Edit Job..."), this);
    m_editJobAction->setIcon(QIcon(":/icons/icons/edit.svg"));
    m_editJobAction->setEnabled(false);
    connect(m_editJobAction, &QAction::triggered, this, [this]() {
        QStringList names = m_jobWidget->jobNames();
        if (names.isEmpty()) {
            QMessageBox::information(this, tr("Edit Job"),
                                     tr("No Jobs available."));
            return;
        }

        // Create dialog with BJobResourceWidget
        QDialog dlg(this);
        dlg.setWindowTitle(tr("Job Resources"));
        dlg.setMinimumSize(900, 600);
        dlg.resize(1000, 700);

        QVBoxLayout *layout = new QVBoxLayout(&dlg);
        BJobResourceWidget *jobWidget = new BJobResourceWidget("Job", m_director, &dlg);

        layout->addWidget(jobWidget);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, &dlg);
        connect(buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
        layout->addWidget(buttonBox);

        // Helper to populate resources + reference data
        BJobConfigModel *model = m_jobWidget->jobConfigModel();
        BJobWidget *jobW = m_jobWidget;
        auto populateWidget = [model, jobWidget, jobW]() {
            QList<BConfigResource> resources = model->jobResources();
            jobWidget->setResources(resources);

            // Build reference data from models + parsed resources
            QMap<QString, QStringList> refData;
            refData["Client"] = jobW->clientNames();
            refData["FileSet"] = jobW->filesetNames();
            refData["Storage"] = jobW->storageNames();
            refData["Pool"] = jobW->poolNames();
            refData["Schedule"] = QStringList();
            refData["Messages"] = QStringList();
            refData["Job"] = model->jobNames();
            refData["JobDefs"] = jobW->jobDefsNames();

            refData["Catalog"] = jobW->catalogNames();

            jobWidget->setReferenceData(refData);
        };

        if (model->hasConfigs()) {
            populateWidget();
        } else {
            QMetaObject::Connection *conn = new QMetaObject::Connection();
            *conn = connect(m_director, &BDirector::showJobsResult, &dlg,
                            [populateWidget, conn](const QString &) {
                populateWidget();
                QObject::disconnect(*conn);
                delete conn;
            });
            m_director->doSend(BDirector::Command::ShowJobs);
        }

        dlg.exec();
    });

    m_deleteJobAction = new QAction(tr("Delete Job..."), this);
    m_deleteJobAction->setIcon(QIcon(":/icons/icons/delete.svg"));
    m_deleteJobAction->setEnabled(false);
    connect(m_deleteJobAction, &QAction::triggered, this, [this]() {
        m_jobWidget->tableView()->deleteJob();
    });

    // Modify JobDefs submenu actions
    m_addJobDefsAction = new QAction(tr("Add JobDefs..."), this);
    m_addJobDefsAction->setIcon(QIcon(":/icons/icons/add.svg"));
    m_addJobDefsAction->setEnabled(false);
    connect(m_addJobDefsAction, &QAction::triggered, this, [this]() {
        BJobWizard wizard(BJobWizard::JobDefsType, m_director, this);
        QMap<QString, QStringList> refData;
        refData["Client"] = m_jobWidget->clientNames();
        refData["FileSet"] = m_jobWidget->filesetNames();
        refData["Storage"] = m_jobWidget->storageNames();
        refData["Pool"] = m_jobWidget->poolNames();
        refData["Schedule"] = QStringList();
        refData["Messages"] = QStringList();
        refData["Catalog"] = m_jobWidget->catalogNames();
        refData["JobDefs"] = m_jobWidget->jobDefsNames();
        refData["Job"] = m_jobWidget->jobNames();
        wizard.setReferenceData(refData);
        wizard.exec();
    });

    m_editJobDefsAction = new QAction(tr("Edit JobDefs..."), this);
    m_editJobDefsAction->setIcon(QIcon(":/icons/icons/edit.svg"));
    m_editJobDefsAction->setEnabled(false);
    connect(m_editJobDefsAction, &QAction::triggered, this, [this]() {
        // Create dialog with BJobResourceWidget for JobDefs
        QDialog dlg(this);
        dlg.setWindowTitle(tr("JobDefs Resources"));
        dlg.setMinimumSize(900, 600);
        dlg.resize(1000, 700);

        QVBoxLayout *layout = new QVBoxLayout(&dlg);
        BJobResourceWidget *jdWidget = new BJobResourceWidget("JobDefs", m_director, &dlg);

        layout->addWidget(jdWidget);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, &dlg);
        connect(buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
        layout->addWidget(buttonBox);

        // Use centralized model from BJobWidget (populated at startup)
        BJobConfigModel *model = m_jobWidget->jobConfigModel();
        BJobWidget *jobW = m_jobWidget;

        // Populate from cached data, then refresh
        auto populateWidget = [model, jdWidget, jobW]() {
            QList<BConfigResource> resources = model->jobDefsResources();
            jdWidget->setResources(resources);

            QMap<QString, QStringList> refData;
            refData["Client"] = jobW->clientNames();
            refData["FileSet"] = jobW->filesetNames();
            refData["Storage"] = jobW->storageNames();
            refData["Pool"] = jobW->poolNames();
            refData["Schedule"] = QStringList();
            refData["Messages"] = QStringList();
            refData["Job"] = jobW->jobNames();
            refData["JobDefs"] = jobW->jobDefsNames();
            refData["Catalog"] = jobW->catalogNames();

            jdWidget->setReferenceData(refData);
        };

        // If model already has data, populate immediately
        if (!model->jobDefsNames().isEmpty()) {
            populateWidget();
        }

        // Also refresh from Director (one-shot connection)
        QMetaObject::Connection *conn = new QMetaObject::Connection();
        *conn = connect(m_director, &BDirector::showJobDefsResult, &dlg,
                        [populateWidget, conn](const QString &) {
            populateWidget();
            QObject::disconnect(*conn);
            delete conn;
        });
        m_director->doSend(BDirector::Command::ShowJobDefs);

        dlg.exec();
    });

    m_deleteJobDefsAction = new QAction(tr("Delete JobDefs..."), this);
    m_deleteJobDefsAction->setIcon(QIcon(":/icons/icons/delete.svg"));
    m_deleteJobDefsAction->setEnabled(false);
    connect(m_deleteJobDefsAction, &QAction::triggered, this, [this]() {
        QMessageBox::information(this, tr("Delete JobDefs"), tr("Not implemented yet."));
    });

    // Filesets submenu actions
    m_addFileSetAction = new QAction(tr("Add FileSet..."), this);
    m_addFileSetAction->setIcon(QIcon(":/icons/icons/add.svg"));
    m_addFileSetAction->setEnabled(false);
    connect(m_addFileSetAction, &QAction::triggered, this, [this]() {
        BFileSetWizard wizard(m_director, m_jobWidget->filesetModel(), this);
        wizard.exec();
    });

    m_editFileSetAction = new QAction(tr("Edit FileSet..."), this);
    m_editFileSetAction->setIcon(QIcon(":/icons/icons/edit.svg"));
    m_editFileSetAction->setEnabled(false);
    connect(m_editFileSetAction, &QAction::triggered, this, [this]() {
        BFileSetWizard wizard(m_director, QString(), m_jobWidget->filesetModel(), this);
        wizard.exec();
    });

    m_deleteFileSetAction = new QAction(tr("Delete FileSet..."), this);
    m_deleteFileSetAction->setIcon(QIcon(":/icons/icons/delete.svg"));
    m_deleteFileSetAction->setEnabled(false);
    connect(m_deleteFileSetAction, &QAction::triggered, this, [this]() {
        BFilesetModel *model = m_jobWidget->filesetModel();
        if (!model || model->rowCount() == 0) {
            QMessageBox::information(this, tr("Delete FileSet"),
                                     tr("No FileSets available."));
            return;
        }

        QDialog dialog(this);
        dialog.setWindowTitle(tr("Delete FileSet"));
        dialog.setMinimumWidth(400);

        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *label = new QLabel(tr("Select the FileSet to delete:"), &dialog);
        layout->addWidget(label);

        QComboBox *combo = new QComboBox(&dialog);
        combo->setModel(model);
        layout->addWidget(combo);

        layout->addSpacing(10);

        QLabel *warning = new QLabel(
            tr("<span style='color: #cc0000;'><b>Warning:</b> This will remove the "
               "FileSet resource from the Director configuration. "
               "This action cannot be undone!</span>"), &dialog);
        warning->setWordWrap(true);
        layout->addWidget(warning);

        layout->addSpacing(10);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Delete"));
        buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon::fromTheme("edit-delete"));
        connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        layout->addWidget(buttonBox);

        if (dialog.exec() != QDialog::Accepted) return;

        QString name = combo->currentText();
        int confirm = QMessageBox::warning(this, tr("Confirm Delete"),
            tr("Are you sure you want to delete FileSet \"%1\"?").arg(name),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (confirm == QMessageBox::Yes) {
            onSendCommand(BDirector::Command::DeleteFileSet, name);
        }
    });

    // Clients Actions
    m_addClientAction = new QAction(tr("Add New Client..."), this);
    m_addClientAction->setIcon(QIcon(":/icons/icons/add.svg"));
    m_addClientAction->setEnabled(false);
    connect(m_addClientAction, &QAction::triggered, this, [this]() {
        BNewClientWizard wizard(m_director, this);
        wizard.exec();
    });

    m_refreshClientsAction = new QAction(tr("Refresh Clients"), this);
    m_refreshClientsAction->setIcon(QIcon(":/icons/icons/refresh.svg"));
    m_refreshClientsAction->setShortcut(QKeySequence("Ctrl+Shift+C"));
    m_refreshClientsAction->setEnabled(false);
    connect(m_refreshClientsAction, &QAction::triggered, m_clientWidget, &BClientsWidget::triggerRefresh);

    m_clientDetailsAction = new QAction(tr("Show Client Details"), this);
    m_clientDetailsAction->setIcon(QIcon(":/icons/icons/clients.svg"));
    m_clientDetailsAction->setEnabled(false);
    connect(m_clientDetailsAction, &QAction::triggered, this, [this]() {
        m_clientWidget->showSelectedClientDetails();
    });
    connect(m_clientWidget, &BClientsWidget::selectionChanged,
            m_clientDetailsAction, &QAction::setEnabled);

    // Storage Actions
    m_refreshStorageAction = new QAction(tr("Refresh Storage"), this);
    m_refreshStorageAction->setIcon(QIcon(":/icons/icons/refresh.svg"));
    m_refreshStorageAction->setShortcut(QKeySequence("Ctrl+Shift+V"));
    m_refreshStorageAction->setEnabled(false);
    connect(m_refreshStorageAction, &QAction::triggered, m_storageWidget, &StorageWidget::triggerRefresh);

    // Schedule Actions
    m_addScheduleAction = new QAction(tr("Add Schedule..."), this);
    m_addScheduleAction->setIcon(QIcon(":/icons/icons/add.svg"));
    m_addScheduleAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    m_addScheduleAction->setEnabled(false);
    connect(m_addScheduleAction, &QAction::triggered, this, [this]() {
        BDirectiveSchema::instance().loadSchemas();
        BScheduleWizard wizard(m_director, this);
        QMap<QString, QStringList> refData;
        refData["Pool"] = m_jobWidget->poolNames();
        refData["Storage"] = m_jobWidget->storageNames();
        wizard.setReferenceData(refData);
        if (wizard.exec() == QDialog::Accepted) {
            m_scheduleWidget->triggerRefresh();
        }
    });

    m_refreshSchedulesAction = new QAction(tr("Refresh Schedules"), this);
    m_refreshSchedulesAction->setIcon(QIcon(":/icons/icons/refresh.svg"));
    m_refreshSchedulesAction->setShortcut(QKeySequence("Ctrl+Shift+D"));
    m_refreshSchedulesAction->setEnabled(false);
    connect(m_refreshSchedulesAction, &QAction::triggered, m_scheduleWidget, &BScheduleWidget::triggerRefresh);

    // Tools Actions
    m_cleanupDatabaseAction = new QAction(tr("Database Cleanup..."), this);
    m_cleanupDatabaseAction->setIcon(QIcon(":/icons/icons/delete.svg"));
    m_cleanupDatabaseAction->setToolTip(tr("Clean up old backups and free disk space"));
    m_cleanupDatabaseAction->setEnabled(false);
    connect(m_cleanupDatabaseAction, &QAction::triggered, this, &BMainWindow::onCleanupDatabase);

    m_connectionWizardAction = new QAction(tr("Connection Wizard..."), this);
    m_connectionWizardAction->setIcon(QIcon(":/icons/icons/director.svg"));
    m_connectionWizardAction->setToolTip(tr("Set up a new director connection"));
    connect(m_connectionWizardAction, &QAction::triggered, this, &BMainWindow::onConnectionWizard);

    m_importConfigAction = new QAction(tr("Import Config..."), this);
    m_importConfigAction->setIcon(QIcon(":/icons/icons/import.svg"));
    m_importConfigAction->setToolTip(tr("Import Director configuration from files or ZIP archive"));
    connect(m_importConfigAction, &QAction::triggered, this, &BMainWindow::onImportConfig);
}

void BMainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("File"));
    m_fileMenu->addAction(m_connectAction);
    m_fileMenu->addAction(m_connectLastAction);
    m_fileMenu->addAction(m_disconnectAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_connectionWizardAction);
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
    m_jobsMenu->addAction(m_refreshJobsAction);
    m_jobsMenu->addSeparator();

    // Modify Jobs submenu
    m_modifyJobsSubMenu = m_jobsMenu->addMenu(QIcon(":/icons/icons/edit.svg"), tr("Modify Jobs"));
    m_modifyJobsSubMenu->addAction(m_addJobAction);
    m_modifyJobsSubMenu->addAction(m_editJobAction);
    m_modifyJobsSubMenu->addAction(m_deleteJobAction);

    // Modify JobDefs submenu
    m_jobDefsSubMenu = m_jobsMenu->addMenu(QIcon(":/icons/icons/edit.svg"), tr("Modify JobDefs"));
    m_jobDefsSubMenu->addAction(m_addJobDefsAction);
    m_jobDefsSubMenu->addAction(m_editJobDefsAction);
    m_jobDefsSubMenu->addAction(m_deleteJobDefsAction);

    // Filesets submenu
    m_filesetsSubMenu = m_jobsMenu->addMenu(QIcon(":/icons/icons/filesets.svg"), tr("Filesets"));
    m_filesetsSubMenu->addAction(m_addFileSetAction);
    m_filesetsSubMenu->addAction(m_editFileSetAction);
    m_filesetsSubMenu->addAction(m_deleteFileSetAction);

    m_jobsMenu->addSeparator();
    m_jobsMenu->addAction(m_exportJobsJsonAction);
    m_jobsMenu->addAction(m_exportJobsCsvAction);

    // Clients Menu
    m_clientsMenu = menuBar()->addMenu(tr("Clients"));
    m_clientsMenu->addAction(m_addClientAction);
    m_clientsMenu->addAction(m_clientDetailsAction);
    m_clientsMenu->addSeparator();
    m_clientsMenu->addAction(m_refreshClientsAction);

    // Storage Menu
    m_storageMenu = menuBar()->addMenu(tr("Storage"));
    m_storageMenu->addAction(m_refreshStorageAction);

    // Schedules Menu
    m_schedulesMenu = menuBar()->addMenu(tr("Schedules"));
    m_schedulesMenu->addAction(m_addScheduleAction);
    m_schedulesMenu->addSeparator();
    m_schedulesMenu->addAction(m_refreshSchedulesAction);

    // Tools Menu
    m_toolsMenu = menuBar()->addMenu(tr("Tools"));
    m_toolsMenu->addAction(m_importConfigAction);
    m_toolsMenu->addSeparator();
    m_toolsMenu->addAction(m_cleanupDatabaseAction);

    m_helpMenu = menuBar()->addMenu(tr("Help"));
    m_helpMenu->addAction(m_documentationAction);
    m_helpMenu->addAction(m_keyboardShortcutsAction);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_myPermissionsAction);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_reportBugAction);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_aboutAction);
    m_helpMenu->addAction(m_aboutQtAction);

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

void BMainWindow::createToolBar()
{
    m_mainToolBar = addToolBar(tr("Main Toolbar"));

    // Exit button first
    m_mainToolBar->addAction(m_exitAction);

    // Wizards section
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_addJobAction);
    m_mainToolBar->addAction(m_addScheduleAction);

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

void BMainWindow::onConnectTriggered()
{
    showConnectionDialog();
}

void BMainWindow::showConnectionDialog()
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
        m_director->tlsConfig()->tlsCipherList = profile.tlsCipherList;

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
                          profile.getPasswordHashHex());

        m_statusLabel->setText(tr("Connecting to %1...").arg(profile.name));

        // Show wait cursor until all resources are loaded
        QApplication::setOverrideCursor(Qt::WaitCursor);
    }
}
void BMainWindow::onDisconnectTriggered()
{
    // Restore cursor if still waiting
    while (QApplication::overrideCursor()) {
        QApplication::restoreOverrideCursor();
    }

    m_director->disconnect();
    m_statusLabel->setText("Getrennt");
}

void BMainWindow::onToggleConnectionTriggered()
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

void BMainWindow::onAboutTriggered()
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

    // Logo - Onesimus icon
    QLabel* logoLabel = new QLabel();
    logoLabel->setPixmap(QIcon(":/icons/icons/onesimus.svg").pixmap(80, 80));
    headerLayout->addWidget(logoLabel);

    // Title and version
    QVBoxLayout* titleLayout = new QVBoxLayout();
    QLabel* titleLabel = new QLabel("<h1 style='margin:0;'>Onesimus</h1>");
    titleLayout->addWidget(titleLabel);

    QLabel* versionLabel = new QLabel(QString("<p style='color:#666; margin:0;'>Version %1 (Build %2)</p>")
                                          .arg(PROJECT_VERSION)
                                          .arg(BUILD_NUMBER));
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

    // Description — etymology and purpose
    QLabel* descLabel = new QLabel(
        "<p style='font-style:italic;'>In the Letter to Philemon, the Apostle Paul sends back "
        "Onesimus &mdash; a runaway slave whose name means &ldquo;the useful one&rdquo; in Greek. "
        "Once lost, now returned with purpose: no longer useless, but indispensable.</p>"
        "<p>Backups share that story. Data slips away &mdash; through failure, accident, or time. "
        "What matters is that it comes back, intact and useful, when you need it most. "
        "Onesimus helps you manage that journey: keeping watch over your Bareos environment, "
        "so that nothing stays lost for long.</p>"
    );
    descLabel->setWordWrap(true);
    mainLayout->addWidget(descLabel);

    // Copyright and Author
    QLabel* copyrightLabel = new QLabel(
        "<p><b>Author:</b> Jörg Bernau &lt;joerg@bernau.family&gt;</p>"
        "<p><b>Copyright:</b> © 2025-2026 Jörg Bernau. All rights reserved.</p>"
        "<p><b>Website:</b> <a href='https://onesimus.io'>https://onesimus.io</a></p>"
        "<p><b>Support:</b> <a href='mailto:support@onesimus.io'>support@onesimus.io</a></p>"
        "<p><b>Source:</b> <a href='https://github.com/Beerlesklopfer/Onesimus'>github.com/Beerlesklopfer/Onesimus</a></p>"
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

    // Build Information
    QLabel* buildInfoLabel = new QLabel(
        QString("<p style='font-size:9pt;'><b>Build Date:</b> %1 %2</p>"
                "<p style='font-size:9pt;'><b>Git:</b> %3 (%4)</p>")
            .arg(BUILD_DATE)
            .arg(BUILD_TIME)
            .arg(GIT_COMMIT_HASH)
            .arg(GIT_BRANCH)
    );
    buildInfoLabel->setWordWrap(true);
    mainLayout->addWidget(buildInfoLabel);

    // Acknowledgments
    QLabel* ackLabel = new QLabel(
        "<p style='font-size:9pt;'><b>Built with:</b> Qt " + QString(qVersion()) +
        QString(", OpenSSL, %1 %2</p>").arg(COMPILER_ID).arg(COMPILER_VERSION)
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

void BMainWindow::onDocumentationTriggered()
{
    // Open online documentation in default browser
    QString docUrl = "https://github.com/Beerlesklopfer/Onesimus/wiki";
    if (!QDesktopServices::openUrl(QUrl(docUrl))) {
        QMessageBox::information(this, tr("Documentation"),
            tr("Could not open browser. Please visit:\n%1").arg(docUrl));
    }
}

void BMainWindow::onReportBugTriggered()
{
    // Open GitHub issues page in default browser
    QString issuesUrl = "https://github.com/Beerlesklopfer/Onesimus/issues";
    if (!QDesktopServices::openUrl(QUrl(issuesUrl))) {
        QMessageBox::information(this, tr("Report Bug"),
            tr("Could not open browser. Please visit:\n%1").arg(issuesUrl));
    }
}

void BMainWindow::onKeyboardShortcutsTriggered()
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

// ============================================================================
// My Permissions Dialog — queries .help all for available commands
// ============================================================================

namespace {

class BPermissionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BPermissionsDialog(BDirector *director, QWidget *parent = nullptr)
        : QDialog(parent)
        , m_director(director)
        , m_received(false)
    {
        setWindowTitle(tr("My Permissions"));
        setMinimumSize(550, 600);

        QVBoxLayout *dialogLayout = new QVBoxLayout(this);

        // Scroll area for all content
        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        QWidget *scrollContent = new QWidget();
        QVBoxLayout *mainLayout = new QVBoxLayout(scrollContent);

        // Connection info
        QGroupBox *connGroup = new QGroupBox(tr("Connection"), scrollContent);
        QFormLayout *connLayout = new QFormLayout(connGroup);
        connLayout->addRow(tr("Director:"), new QLabel(director->currentDirectorName(), this));
        connLayout->addRow(tr("Host:"), new QLabel(
            QString("%1:%2").arg(director->currentHost()).arg(director->currentPort()), this));

        auto *tlsCfg = director->tlsConfig();
        QString tlsInfo;
        if (tlsCfg && tlsCfg->tlsEnable) {
            if (tlsCfg->tlsPSKEnable)
                tlsInfo = tr("TLS-PSK");
            else
                tlsInfo = tr("TLS Certificate");
        } else {
            tlsInfo = tr("None");
        }
        connLayout->addRow(tr("Encryption:"), new QLabel(tlsInfo, this));
        mainLayout->addWidget(connGroup);

        // Key commands — cross-referenced against .help all response
        // .help all returns regular commands with "permission" field
        m_keyCommands = {
            {tr("Backup & Restore"), {"run", "restore", "estimate"}},
            {tr("Job Control"),      {"cancel", "status", "list", "llist", "messages", "rerun"}},
            {tr("Administration"),   {"delete", "purge", "prune", "update", "reload",
                                      "configure", "setdebug"}},
            {tr("Media"),            {"label", "relabel", "mount", "unmount",
                                      "release", "import", "export", "truncate"}},
        };

        // Build a flat set of tracked commands for quick lookup
        for (const auto &group : m_keyCommands) {
            for (const QString &cmd : group.second)
                m_trackedCommands.insert(cmd);
        }

        // Key commands group with labels
        m_cmdGroup = new QGroupBox(tr("Command Permissions"), this);
        m_cmdLayout = new QFormLayout(m_cmdGroup);
        for (const auto &group : m_keyCommands) {
            // Section header
            QLabel *header = new QLabel(QString("<b>%1</b>").arg(group.first), this);
            m_cmdLayout->addRow(header);
            for (const QString &cmd : group.second) {
                QLabel *label = new QLabel(tr("Checking..."), this);
                label->setStyleSheet("QLabel { color: gray; }");
                m_labels[cmd] = label;
                m_cmdLayout->addRow("  " + cmd + ":", label);
            }
        }
        mainLayout->addWidget(m_cmdGroup);

        // Restore details
        m_restoreGroup = new QGroupBox(tr("Restore Details"), scrollContent);
        QVBoxLayout *restoreLayout = new QVBoxLayout(m_restoreGroup);
        m_restoreLabel = new QLabel(tr("Loading..."), scrollContent);
        m_restoreLabel->setWordWrap(true);
        restoreLayout->addWidget(m_restoreLabel);
        m_restoreGroup->setVisible(false);
        mainLayout->addWidget(m_restoreGroup);

        // Additional available commands
        m_extraGroup = new QGroupBox(tr("Other Available Commands"), scrollContent);
        m_extraLayout = new QVBoxLayout(m_extraGroup);
        m_extraLabel = new QLabel(tr("Loading..."), scrollContent);
        m_extraLabel->setWordWrap(true);
        m_extraLayout->addWidget(m_extraLabel);
        m_extraGroup->setVisible(false);
        mainLayout->addWidget(m_extraGroup);

        // Raw response (collapsible, for debugging)
        m_rawGroup = new QGroupBox(tr("Raw Director Response"), scrollContent);
        m_rawGroup->setCheckable(true);
        m_rawGroup->setChecked(false);
        QVBoxLayout *rawLayout = new QVBoxLayout(m_rawGroup);
        m_rawEdit = new QTextEdit(scrollContent);
        m_rawEdit->setReadOnly(true);
        m_rawEdit->setFont(QFont("monospace"));
        m_rawEdit->setMaximumHeight(200);
        m_rawEdit->setVisible(false);
        rawLayout->addWidget(m_rawEdit);
        connect(m_rawGroup, &QGroupBox::toggled, m_rawEdit, &QTextEdit::setVisible);
        mainLayout->addWidget(m_rawGroup);

        mainLayout->addStretch();
        scrollArea->setWidget(scrollContent);
        dialogLayout->addWidget(scrollArea);

        // Status and close button outside scroll area
        m_statusLabel = new QLabel(tr("Querying Director..."), this);
        dialogLayout->addWidget(m_statusLabel);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::accept);
        dialogLayout->addWidget(buttonBox);

        // Connect Director signals
        connect(m_director, &BDirector::jsonResult,
                this, &BPermissionsDialog::onJsonResponse);
        connect(m_director, &BDirector::textResult,
                this, &BPermissionsDialog::onTextResponse);

        // Send single .help all command
        m_director->doSend(BDirector::Command::Custom, ".help all");
    }

    ~BPermissionsDialog() override
    {
        disconnectDirector();
    }

private slots:
    void onJsonResponse(BDirector::Command cmd, const QString &jsonData)
    {
        if (m_received) return;

        // Show ALL responses for debugging
        m_rawEdit->append(QString("[JSON cmd=%1] %2")
                              .arg(static_cast<int>(cmd))
                              .arg(jsonData.left(4000)));

        if (cmd != BDirector::Command::Custom) return;

        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject root = doc.object();
        QJsonObject result = root["result"].toObject();

        // .help all response format:
        // {"result": {"add": {"command":"add","description":"...","arguments":"...","permission":true}, ...}}
        // Each key in result is a command name, value is an object with "permission" field

        // Verify this looks like a .help response (at least some entries have "permission")
        bool isHelpResponse = false;
        for (auto it = result.begin(); it != result.end(); ++it) {
            if (it.value().isObject() && it.value().toObject().contains("permission")) {
                isHelpResponse = true;
                break;
            }
        }
        if (!isHelpResponse || result.size() < 5) return;

        m_received = true;

        // Build permission map: command name -> {permission, arguments}
        QMap<QString, bool> permissions;
        QMap<QString, QString> arguments;
        for (auto it = result.begin(); it != result.end(); ++it) {
            QJsonObject entry = it.value().toObject();
            QString cmdName = it.key().trimmed();
            permissions[cmdName] = entry["permission"].toBool(false);
            arguments[cmdName] = entry["arguments"].toString();
        }

        populateResults(permissions, arguments);
        disconnectDirector();
    }

    void onTextResponse(BDirector::Command cmd, const QString &response)
    {
        if (m_received) return;

        m_rawEdit->append(QString("[TEXT cmd=%1] %2")
                              .arg(static_cast<int>(cmd))
                              .arg(response.left(2000)));
        // Text responses ignored — we expect JSON from .help all
    }

private:
    void populateResults(const QMap<QString, bool> &permissions,
                         const QMap<QString, QString> &arguments)
    {
        // Update key command labels using actual permission field
        for (const auto &group : m_keyCommands) {
            for (const QString &cmd : group.second) {
                QLabel *label = m_labels.value(cmd);
                if (!label) continue;

                if (!permissions.contains(cmd)) {
                    label->setText(tr("N/A"));
                    label->setStyleSheet("QLabel { color: gray; }");
                } else if (permissions[cmd]) {
                    label->setText(tr("Authorized"));
                    label->setStyleSheet("QLabel { color: green; font-weight: bold; }");
                } else {
                    label->setText(tr("Not authorized"));
                    label->setStyleSheet("QLabel { color: red; font-weight: bold; }");
                }
            }
        }

        // Show restore details if available
        if (permissions.contains("restore")) {
            m_restoreGroup->setVisible(true);
            QString restoreArgs = arguments.value("restore");
            // Parse key parameters from the restore arguments string
            QStringList params;
            // Extract parameter names: look for word=<TYPE> patterns
            QRegularExpression paramRe("(\\w+)=<([^>]+)>");
            QRegularExpressionMatchIterator i = paramRe.globalMatch(restoreArgs);
            while (i.hasNext()) {
                QRegularExpressionMatch match = i.next();
                params << match.captured(1);
            }

            bool hasWhere = restoreArgs.contains("where=", Qt::CaseInsensitive);
            bool hasClient = restoreArgs.contains("client=", Qt::CaseInsensitive);
            bool hasReplace = restoreArgs.contains("replace=", Qt::CaseInsensitive);

            QString info;
            info += tr("<b>Permission:</b> %1<br>")
                        .arg(permissions["restore"]
                                 ? "<span style='color:green;'>Authorized</span>"
                                 : "<span style='color:red;'>Not authorized</span>");
            info += tr("<b>where= (restore path):</b> %1<br>")
                        .arg(hasWhere
                                 ? "<span style='color:green;'>Available</span>"
                                 : "<span style='color:red;'>Not available (WhereACL may restrict)</span>");
            info += tr("<b>client= (target client):</b> %1<br>")
                        .arg(hasClient
                                 ? "<span style='color:green;'>Available</span>"
                                 : "<span style='color:red;'>Not available</span>");
            info += tr("<b>replace= (replace policy):</b> %1<br>")
                        .arg(hasReplace
                                 ? "<span style='color:green;'>Available</span>"
                                 : "<span style='color:red;'>Not available</span>");

            if (!params.isEmpty()) {
                info += tr("<br><b>Available parameters:</b> %1").arg(params.join(", "));
            }

            m_restoreLabel->setText(info);
        }

        // Collect additional commands not in our key list
        QStringList extras;
        int authorizedCount = 0;
        for (auto it = permissions.begin(); it != permissions.end(); ++it) {
            if (it.value()) authorizedCount++;
            if (!m_trackedCommands.contains(it.key()))
                extras.append(it.key());
        }
        extras.sort();

        if (!extras.isEmpty()) {
            m_extraLabel->setText(extras.join(", "));
            m_extraGroup->setVisible(true);
        }

        m_statusLabel->setText(tr("Total: %1 commands (%2 authorized, %3 denied)")
                                   .arg(permissions.size())
                                   .arg(authorizedCount)
                                   .arg(permissions.size() - authorizedCount));
    }

    void disconnectDirector()
    {
        if (m_director) {
            disconnect(m_director, &BDirector::jsonResult,
                       this, &BPermissionsDialog::onJsonResponse);
            disconnect(m_director, &BDirector::textResult,
                       this, &BPermissionsDialog::onTextResponse);
        }
    }

    BDirector *m_director;
    bool m_received;

    // Key commands organized by category
    QList<QPair<QString, QStringList>> m_keyCommands;
    QSet<QString> m_trackedCommands;
    QMap<QString, QLabel*> m_labels;

    QGroupBox *m_cmdGroup;
    QFormLayout *m_cmdLayout;
    QGroupBox *m_restoreGroup;
    QLabel *m_restoreLabel;
    QGroupBox *m_extraGroup;
    QVBoxLayout *m_extraLayout;
    QLabel *m_extraLabel;
    QLabel *m_statusLabel;
    QGroupBox *m_rawGroup;
    QTextEdit *m_rawEdit;
};

} // anonymous namespace

void BMainWindow::onMyPermissionsTriggered()
{
    if (!m_director || !m_director->isConnected()) {
        QMessageBox::information(this, tr("My Permissions"),
                                 tr("Not connected to a Director."));
        return;
    }

    BPermissionsDialog dialog(m_director, this);
    dialog.exec();
}

void BMainWindow::onSettingsTriggered()
{
    // Get available levels from JobWidget's level model (if connected)
    QList<QPair<QString, QString>> availableLevels;
    if (m_jobWidget && m_jobWidget->levelModel()) {
        QStringList levelNames = m_jobWidget->levelModel()->levelDescriptions();
        for (const QString &name : levelNames) {
            availableLevels.append({name, name});
        }
    }

    BSettingsDialog dialog(m_director, availableLevels, this);
    if (dialog.exec() == QDialog::Accepted) {
        // Einstellungen wurden geändert
        m_statusLabel->setText("Einstellungen gespeichert");

        // Settings are automatically applied via BSettings signals
        // Auto-refresh is handled by onAutoRefreshSettingsChanged slot
    }
}

void BMainWindow::onAuthentificationSucceeded(const bool connected, const QString msg)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "####################################";
#endif
    // Update Actions
    m_connectAction->setDisabled(connected);
    m_disconnectAction->setEnabled(connected);
    m_refreshAction->setEnabled(connected);
    m_toggleStatisticsAction->setEnabled(connected);  // ✅ Statistiken nur bei Verbindung
    m_toggleStatisticsButton->setEnabled(connected);  // ✅ Toolbar-Button synchronisieren
    m_toggleJobLogAction->setEnabled(connected);  // ✅ Job Log nur bei Verbindung
    m_myPermissionsAction->setEnabled(connected);
    m_tabWidget->setEnabled(connected);

    // Update toggle connection button icon and tooltip
    if (connected) {
        m_toggleConnectionAction->setIcon(QIcon(":/icons/icons/disconnect.svg"));
        m_toggleConnectionAction->setToolTip(tr("Disconnect from Director"));
    } else {
        m_toggleConnectionAction->setIcon(QIcon(":/icons/icons/connect.svg"));
        m_toggleConnectionAction->setToolTip(tr("Connect to Director"));
    }

    // Update job menu actions
    m_refreshJobsAction->setEnabled(connected);
    m_exportJobsJsonAction->setEnabled(connected);
    m_exportJobsCsvAction->setEnabled(connected);

    // Job control actions
    m_runJobAction->setEnabled(connected);  // Run job can be used without selection (enter job name)
    m_addJobAction->setEnabled(connected);
    m_editJobAction->setEnabled(connected);
    m_deleteJobAction->setEnabled(connected);
    m_addFileSetAction->setEnabled(connected);
    m_editFileSetAction->setEnabled(connected);
    m_deleteFileSetAction->setEnabled(connected);
    m_addJobDefsAction->setEnabled(connected);
    m_editJobDefsAction->setEnabled(connected);
    m_deleteJobDefsAction->setEnabled(connected);
    m_addScheduleAction->setEnabled(connected);
    // Cancel and Details require job selection, so they stay disabled until selection changes
    if (!connected) {
        m_cancelJobAction->setEnabled(false);
        m_jobDetailsAction->setEnabled(false);
    }

    // Update client menu actions
    m_addClientAction->setEnabled(connected);
    m_refreshClientsAction->setEnabled(connected);

    // Update storage menu actions
    m_refreshStorageAction->setEnabled(connected);

    // Update schedule menu actions
    m_refreshSchedulesAction->setEnabled(connected);

    // Update tools menu actions
    m_cleanupDatabaseAction->setEnabled(connected);

    // Update widget connection states (BJobWidget handles messages widget internally)
    m_jobWidget->setConnectionState(connected);
    m_clientWidget->setConnectionState(connected);
    m_storageWidget->setConnectionState(connected);
    m_scheduleWidget->setConnectionState(connected);

    if (connected) {
        // ✅ Jetzt erst statusMessage Signal verbinden (nach erfolgreicher Auth)
        m_statusMessageConnection = connect(m_director, &BDirector::statusMessage,
                                             this, [this](const QString &msg) {
            m_statusLabel->setText(msg);
        });

        // Show "Connected to [director] version [version]" in status bar
        QString directorName = m_director->currentDirectorName();
        QString connectionInfo;
        if (!directorName.isEmpty()) {
            connectionInfo = tr("Connected to %1 version %2").arg(directorName, msg);
        } else {
            connectionInfo = tr("Connected: version %1").arg(msg);
        }
        m_connectionLabel->setText(connectionInfo);
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
            BLOG_DEBUG() << "MainWindow: Auto-refresh started (interval:" << settings.behaviorRefreshInterval() << "seconds)";
#endif
        }

        // Note: Message polling is now handled by BJobWidget::setConnectionState()

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
        BLOG_DEBUG() << "MainWindow: Auto-refresh stopped (disconnected)";
#endif

        // Note: clearData() is now handled by setConnectionState(false) above
    }
}

void BMainWindow::onConnectionError(const QString &error)
{
    // Restore cursor on error
    QApplication::restoreOverrideCursor();

    m_statusLabel->setText(tr("Error: ") + error);

    // Guard against multiple error dialogs (e.g., auth failure followed by socket disconnect)
    static bool showingErrorDialog = false;
    if (showingErrorDialog) return;
    showingErrorDialog = true;

    QMessageBox::critical(this, tr("Connection Error"), error);

    showingErrorDialog = false;
}

void BMainWindow::onRefreshAll()
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "Refresh clicked";
#endif

    if (!m_director->isConnected()) {
        return;
    }

    m_statusLabel->setText("Aktualisiere Daten...");

    // Use pagination-aware refresh for jobs (respects ListJobTotals → ListJobs flow)
    m_jobWidget->triggerRefresh();

    // Use widget's own refresh for clients (sends ListClients via signal)
    m_clientWidget->triggerRefresh();
    onSendCommand(BDirector::Command::ShowClients, "");
    onSendCommand(BDirector::Command::ListBackups, "");  // Unfiltered backup stats for clients

    // ✅ Request filter data using dot-commands
    // These provide ALL configured jobs/clients/levels/schedules, not just executed ones
    onSendCommand(BDirector::Command::DotJobs, "");
    onSendCommand(BDirector::Command::DotClients, "");
    onSendCommand(BDirector::Command::DotLevels, "");
    m_scheduleWidget->triggerRefresh();  // sends DotSchedule + ShowSchedules
    onSendCommand(BDirector::Command::DotFilesets, "");
    onSendCommand(BDirector::Command::DotStorages, "");
    onSendCommand(BDirector::Command::DotPools, "");

    // Request Director messages
    onSendCommand(BDirector::Command::Messages, "");

    // m_director->listVolumes();

    // Reset status after short delay
    QTimer::singleShot(1000, this, [this]() {
        if (m_director->isConnected()) {
            m_statusLabel->setText(tr("Ready"));
        }
    });
}

void BMainWindow::onConnectLastUsed()
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

void BMainWindow::onSendCommand(const BDirector::Command cmd, const QString &args)
{
    if (!m_director) {
        BLOG_WARNING() << "BMainWindow::onSendCommand: No director instance!";
        return;
    }

    // Thread-safe: Use queued connection to send command to Director thread
    QMetaObject::invokeMethod(m_director, "doSend",
                              Qt::QueuedConnection,
                              Q_ARG(BDirector::Command, cmd),
                              Q_ARG(QString, args));
}

void BMainWindow::onDirectorConnect(const QString &host, int port, const QString &directorName,
                                   const QString &consoleName, const QString &password)
{
    if (!m_director) {
        BLOG_WARNING() << "BMainWindow::onDirectorConnect: No director instance!";
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

void BMainWindow::onDirectorDisconnect()
{
    if (!m_director) {
        BLOG_WARNING() << "BMainWindow::onDirectorDisconnect: No director instance!";
        return;
    }

    // Thread-safe: Use queued connection to disconnect in Director thread
    QMetaObject::invokeMethod(m_director, "disconnect",
                              Qt::QueuedConnection);
}

void BMainWindow::loadAndConnectLastUsed()
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

    if (!profile.isValid()) {
#ifdef IS_DEVELOPER
        BLOG_DEBUG() << "No valid last-used profile - skipping auto-connect"
                 << "(id:" << profile.id << ", host:" << profile.host
                 << ", hasPasswordHash:" << profile.hasValidPasswordHash() << ")";
#endif
        m_connectLastAction->setEnabled(!settings.lastUsedProfileId().isEmpty());
        return;
    }

    // Configure TLS
    m_director->tlsConfig()->tlsEnable = profile.tlsEnabled;
    m_director->tlsConfig()->tlsRequire = profile.tlsEnabled;
    m_director->tlsConfig()->tlsPSKEnable = profile.tlsUsePSK;
    m_director->tlsConfig()->tlsVerifyPeer = profile.tlsVerifyPeer;
    m_director->tlsConfig()->tlsCipherList = profile.tlsCipherList;

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
    BLOG_DEBUG() << "Auto-connecting to last used profile:" << profile.name
             << "(" << profile.host << ":" << profile.port << ")";
#endif

    m_director->setTLSConfig(*m_director->tlsConfig());
    onDirectorConnect(profile.host, profile.port, profile.directorName,
                      profile.consoleName, profile.getPasswordHashHex());
    m_statusLabel->setText(tr("Auto-connecting to %1...").arg(profile.name));

    // Show wait cursor until all resources are loaded
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Update "Reconnect" button status
    m_connectLastAction->setEnabled(true);
}

BDirector *BMainWindow::director() const
{
    return m_director;
}

void BMainWindow::setDirector(BDirector *newDirector)
{
    m_director = newDirector;
}

void BMainWindow::onCopyTriggered()
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

void BMainWindow::onSelectAllTriggered()
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

void BMainWindow::onClearSelectionTriggered()
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

void BMainWindow::onFindTriggered()
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

void BMainWindow::onAutoRefreshSettingsChanged(bool enabled, int intervalSeconds)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "MainWindow: Auto-refresh settings changed:"
             << "enabled=" << enabled << "interval=" << intervalSeconds << "seconds";
#endif

    bool connected = m_director && m_director->isConnected();

    if (enabled && connected) {
        // Start or restart timer with new interval
        m_autoRefreshTimer->start(intervalSeconds * 1000);
        m_statusLabel->setText(tr("Auto-refresh enabled (%1s)").arg(intervalSeconds));
    } else {
        // Stop timer
        m_autoRefreshTimer->stop();
        if (!enabled && connected) {
            m_statusLabel->setText(tr("Auto-refresh disabled"));
        }
    }
}

void BMainWindow::onToggleTheme()
{
    // Get current theme
    QString currentTheme = BSettings::instance().appearanceTheme();

    // Toggle theme
    QString newTheme;

    if (currentTheme == "dark") {
        newTheme = "light";
        m_toggleThemeAction->setIcon(QIcon(":/icons/icons/moon.svg"));
        m_statusLabel->setText(tr("Switched to Light Theme"));
    } else {
        newTheme = "dark";
        m_toggleThemeAction->setIcon(QIcon(":/icons/icons/sun.svg"));
        m_statusLabel->setText(tr("Switched to Dark Theme"));
    }

    // Apply new theme
    applyTheme(newTheme);

    // Save new theme to settings
    BSettings::instance().setAppearanceTheme(newTheme);
}

void BMainWindow::onExportSettingsTriggered()
{
    if (!m_director->isConnected()) {
        QMessageBox::warning(this, tr("Not Connected"), tr("Please connect to the Director first."));
        return;
    }

    // Export all jobs to JSON
    m_jobWidget->tableView()->exportToJson(false);
    m_statusLabel->setText("Jobs als JSON exportiert");
}

void BMainWindow::onImportSettingsTriggered()
{
    if (!m_director->isConnected()) {
        QMessageBox::warning(this, tr("Not Connected"), tr("Please connect to the Director first."));
        return;
    }

    // Export all jobs to CSV
    m_jobWidget->tableView()->exportToCsv(false);
    m_statusLabel->setText("Jobs als CSV exportiert");
}

void BMainWindow::onCleanupDatabase()
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

void BMainWindow::onConnectionWizard()
{
    // Create wizard data struct to preserve values across page navigation
    BConnectionWizardData *wizardData = new BConnectionWizardData();

    // Pass database connection to wizard (for loading/saving Directors)
    QSqlDatabase *db = nullptr;
    if (m_database && m_database->isReady()) {
        static QSqlDatabase wizardDb = m_database->database();
        db = &wizardDb;
    }
    BConnectionWizard wizard(wizardData, db, this);

    if (wizard.exec() == QDialog::Accepted) {
        // Save the profile
        BConnectionProfile profile = wizard.profile();
        BSettings::instance().addConnectionProfile(profile);

        // Set as default if requested (use wizardData for reliable value)
        if (wizardData->setAsDefault) {
            BSettings::instance().setLastUsedProfileId(profile.id);
        }

        m_statusLabel->setText(tr("Connection profile '%1' saved").arg(profile.name));

        // Connect now if requested (use wizardData for reliable value)
        if (wizardData->connectNow) {
            // Configure TLS
            BDirector::TLSConfig tlsConfig;
            if (profile.legacyAuth) {
                tlsConfig.tlsEnable = false;
                tlsConfig.tlsRequire = false;
                tlsConfig.tlsPSKEnable = false;
            } else if (profile.tlsUsePSK) {
                tlsConfig.tlsEnable = true;
                tlsConfig.tlsRequire = true;
                tlsConfig.tlsPSKEnable = true;
                tlsConfig.tlsVerifyPeer = false;
            } else {
                tlsConfig.tlsEnable = true;
                tlsConfig.tlsRequire = true;
                tlsConfig.tlsPSKEnable = false;
                tlsConfig.tlsVerifyPeer = profile.tlsVerifyPeer;
                if (!profile.tlsCaCertFile.isEmpty())
                    tlsConfig.tlsCaCertFile = QSharedPointer<QFile>(new QFile(profile.tlsCaCertFile));
#ifdef Q_OS_WINDOWS
                if (!profile.tlsPfxFile.isEmpty())
                    tlsConfig.tlsPfxFile = QSharedPointer<QFile>(new QFile(profile.tlsPfxFile));
                if (!profile.tlsPfxPassword.isEmpty())
                    tlsConfig.tlsPfxPassword = profile.tlsPfxPassword;
#else
                if (!profile.tlsCertFile.isEmpty())
                    tlsConfig.tlsCertFile = QSharedPointer<QFile>(new QFile(profile.tlsCertFile));
                if (!profile.tlsKeyFile.isEmpty())
                    tlsConfig.tlsKeyFile = QSharedPointer<QFile>(new QFile(profile.tlsKeyFile));
#endif
            }
            m_director->setTLSConfig(tlsConfig);

            // Connect
            onDirectorConnect(profile.host, profile.port, profile.directorName,
                              profile.consoleName, profile.getPasswordHashHex());
        }
    }

    // Clean up wizard data after wizard completes (success or cancel)
    delete wizardData;
    wizardData = nullptr;
}

void BMainWindow::onImportConfig()
{
    BConfigImportDialog dialog(this);

    connect(&dialog, &BConfigImportDialog::configurationImported,
            this, [this, &dialog](const BConfigResource &director, const BConfigResource &console,
                                  const QString &host, int port) {
                QString directorName = director.name();
                QString consoleName = console.name();

                // Extract password from Console resource
                // Bareos stores it with [md5] prefix, e.g., "[md5]abc123..."
                QString password = console.simpleValue("password");
                QString passwordHash;

                APP_DEBUG << "Import: Console '" << consoleName << "' password field: "
                          << (password.isEmpty() ? "(empty)" : password.left(10) + "...");
                APP_DEBUG << "Import: Console keys: " << console.keys().join(", ");

                if (password.startsWith("[md5]")) {
                    // Already MD5 hashed, extract the hash part
                    passwordHash = password.mid(5);  // Remove "[md5]" prefix
                    APP_DEBUG << "Import: Password has [md5] prefix, hash: " << passwordHash.left(8) << "...";
                } else if (password.length() == 32 && password.contains(QRegularExpression("^[0-9a-fA-F]+$"))) {
                    // Already a raw MD5 hash (32 hex chars)
                    passwordHash = password.toLower();
                    APP_DEBUG << "Import: Password is raw MD5 hash: " << passwordHash.left(8) << "...";
                } else if (!password.isEmpty()) {
                    // Plain text password, hash it
                    QByteArray md5 = QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Md5);
                    passwordHash = QString::fromLatin1(md5.toHex());
                    APP_DEBUG << "Import: Password hashed to: " << passwordHash.left(8) << "...";
                }

                // Warn if password is empty
                if (passwordHash.isEmpty()) {
                    APP_WARNING << "Import: No password found in Console resource '" << consoleName << "'";
                    QMessageBox::warning(this, tr("Password Missing"),
                        tr("The imported Console '%1' does not contain a password.\n\n"
                           "You will need to enter the password manually in the connection settings.")
                            .arg(consoleName));
                }

                if (dialog.createConnectionProfile()) {
                    // Create connection profile from imported config
                    BConnectionProfile profile;
                    profile.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
                    profile.name = directorName;
                    profile.host = host;
                    profile.port = port;
                    profile.directorName = directorName;
                    profile.consoleName = consoleName;
                    profile.passwordHash = passwordHash;
                    profile.tlsEnabled = true;
                    profile.tlsUsePSK = true;

                    // Save the profile
                    BSettings::instance().addConnectionProfile(profile);

                    m_statusLabel->setText(tr("Created connection profile '%1' for Director '%2'")
                                               .arg(profile.name, directorName));

                    // Ask if user wants to connect now
                    QMessageBox::StandardButton reply = QMessageBox::question(
                        this, tr("Connect Now?"),
                        tr("Connection profile '%1' has been created.\n\nDo you want to connect now?")
                            .arg(profile.name),
                        QMessageBox::Yes | QMessageBox::No,
                        QMessageBox::Yes);

                    if (reply == QMessageBox::Yes) {
                        // Configure TLS and connect
                        m_director->tlsConfig()->tlsEnable = profile.tlsEnabled;
                        m_director->tlsConfig()->tlsRequire = profile.tlsEnabled;
                        m_director->tlsConfig()->tlsPSKEnable = profile.tlsUsePSK;
                        onDirectorConnect(profile.host, profile.port,
                                          profile.directorName, profile.consoleName,
                                          profile.passwordHash);
                    }
                } else {
                    m_statusLabel->setText(tr("Imported Director '%1' with Console '%2'")
                                               .arg(directorName, consoleName));
                }
            });

    dialog.exec();
}

// MOC include for Q_OBJECT classes defined in this .cpp file
#include "bmainwindow.moc"
