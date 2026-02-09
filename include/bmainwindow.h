#ifndef BMAINWINDOW_H
#define BMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "blogging.h"
#include "director/bdirector.h"

// Debug logging prefixes for App/BMainWindow
#define APP_DEBUG BLOG_DEBUG()
#define APP_WARNING BLOG_WARNING()
#define APP_CRITICAL BLOG_ERROR()

QT_BEGIN_NAMESPACE
namespace Ui { class BMainWindow; }
QT_END_NAMESPACE

// Forward declarations
class BDatabase;
class BJobWidget;
class BClientsWidget;
class StorageWidget;
class BScheduleWidget;
class BJobsStatisticsWidget;
class QTabWidget;
class QLabel;
class QDockWidget;
class QMenu;
class QToolBar;
class QPushButton;
class QCheckBox;

class BMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    BMainWindow(QWidget *parent = nullptr);
    ~BMainWindow();

    BDirector *director() const;
    void setDirector(BDirector *newDirector);

private slots:
    void onConnectTriggered();
    void onDisconnectTriggered();
    void onToggleConnectionTriggered();
    void onAboutTriggered();
    void onDocumentationTriggered();
    void onReportBugTriggered();
    void onKeyboardShortcutsTriggered();
    void onSettingsTriggered();
    void onExportSettingsTriggered();
    void onImportSettingsTriggered();
    void onAuthentificationSucceeded(bool connected, const QString msg);
    void onConnectionError(const QString &error);
    void onRefreshAll();
    void onConnectLastUsed();

    /**
     * @brief Thread-safe helper to send commands to Director
     * @param cmd Command enum
     * @param args Command arguments
     *
     * This slot ensures all Director commands are executed in the Director's thread
     * using Qt's queued connection mechanism. All widgets should use this slot
     * instead of calling m_director directly.
     */
    void onSendCommand(const BDirector::Command cmd, const QString &args);

    /**
     * @brief Thread-safe helper to connect to Director
     * @param host Hostname or IP
     * @param port Port number
     * @param directorName Director name
     * @param password Director password
     */
    void onDirectorConnect(const QString &host, int port, const QString &directorName,
                           const QString &consoleName, const QString &password);

    /**
     * @brief Thread-safe helper to disconnect from Director
     */
    void onDirectorDisconnect();

    // Edit menu slots
    void onCopyTriggered();
    void onSelectAllTriggered();
    void onClearSelectionTriggered();
    void onFindTriggered();

    // View menu slots
    void onToggleTheme();

    // Settings slots
    void onAutoRefreshSettingsChanged(bool enabled, int intervalSeconds);

    // Tools slots
    void onCleanupDatabase();
    void onConnectionWizard();
    void onImportConfig();

private:
    void setupUI();
    void createActions();
    void createMenus();
    void createToolBar();
    void showConnectionDialog();
    void loadAndConnectLastUsed();
    void applyTheme(const QString &themeName);

    Ui::BMainWindow *ui;
    BDirector *m_director;
    BDatabase *m_database;  ///< SQLite database for storing Directors, Consoles, etc.
    QMetaObject::Connection m_statusMessageConnection;  ///< Connection for statusMessage signal (only after auth)

    // Widgets
    QTabWidget *m_tabWidget;                ///< Main tab widget (Jobs, Clients, Storage, Schedules)
    BJobWidget *m_jobWidget;
    BClientsWidget *m_clientWidget;
    StorageWidget *m_storageWidget;
    BScheduleWidget *m_scheduleWidget;

    // Dock Widgets
    QDockWidget *m_statisticsDock;
    BJobsStatisticsWidget *m_statisticsWidget;

    // Status
    QLabel *m_statusLabel;
    QLabel *m_connectionLabel;
    QString m_directorVersion;  ///< Speichere Director-Version

    // Actions
    QAction *m_connectAction;
    QAction *m_connectLastAction;
    QAction *m_disconnectAction;
    QAction *m_toggleConnectionAction;  // Toggle between connect/disconnect
    QAction *m_reconnectAction;         // Reconnect to last used connection
    QAction *m_refreshAction;
    QAction *m_settingsAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;
    QAction *m_aboutQtAction;
    QAction *m_documentationAction;
    QAction *m_reportBugAction;
    QAction *m_keyboardShortcutsAction;
    QAction *m_toggleStatisticsAction;
    QAction *m_toggleThemeAction;

    // Edit Actions
    QAction *m_copyAction;
    QAction *m_selectAllAction;
    QAction *m_clearSelectionAction;
    QAction *m_findAction;

    // Jobs Actions
    QAction *m_runJobAction;
    QAction *m_cancelJobAction;
    QAction *m_jobDetailsAction;
    QAction *m_refreshJobsAction;
    QAction *m_toggleJobLogAction;
    QAction *m_exportJobsJsonAction;
    QAction *m_exportJobsCsvAction;
    QAction *m_addJobAction;
    QAction *m_editJobAction;
    QAction *m_deleteJobAction;
    QAction *m_addFileSetAction;
    QAction *m_editFileSetAction;
    QAction *m_deleteFileSetAction;

    // Clients Actions
    QAction *m_addClientAction;
    QAction *m_refreshClientsAction;
    QAction *m_clientDetailsAction;

    // Storage Actions
    QAction *m_refreshStorageAction;

    // Schedule Actions
    QAction *m_refreshSchedulesAction;

    // Tools Actions
    QAction *m_cleanupDatabaseAction;
    QAction *m_connectionWizardAction;
    QAction *m_importConfigAction;

    // Menus
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_viewMenu;
    QMenu *m_jobsMenu;
    QMenu *m_modifyJobsSubMenu;
    QMenu *m_filesetsSubMenu;
    QMenu *m_clientsMenu;
    QMenu *m_storageMenu;
    QMenu *m_schedulesMenu;
    QMenu *m_toolsMenu;
    QMenu *m_helpMenu;

    // Toolbar
    QToolBar *m_mainToolBar;
    QPushButton *m_toggleStatisticsButton;

    // Auto-refresh
    QTimer *m_autoRefreshTimer;

    // Initial data load tracking
    // Note: Initial refresh is now handled by BareosDirector state machine (allResourcesLoaded signal)

    // Window size management
    QSize m_sizeBeforeStatistics;  ///< Fenstergröße vor Einblendung der Statistiken
};

#endif // BMAINWINDOW_H
