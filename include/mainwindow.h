#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "bdirector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Forward declarations
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    BDirector *director() const;
    void setDirector(BDirector *newDirector);

private slots:
    void onConnectTriggered();
    void onDisconnectTriggered();
    void onAboutTriggered();
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
    void onDirectorConnect(const QString &host, int port, const QString &directorName, const QString &password);

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

private:
    void setupUI();
    void createActions();
    void createMenus();
    void createToolBar();
    void showConnectionDialog();
    void loadAndConnectLastUsed();
    void applyTheme(const QString &themeName);

    Ui::MainWindow *ui;
    BDirector *m_director;
    
    // Widgets
    QTabWidget *m_tabWidget;
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
    QAction *m_refreshAction;
    QAction *m_settingsAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;
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

    // Clients Actions
    QAction *m_refreshClientsAction;
    QAction *m_clientDetailsAction;

    // Storage Actions
    QAction *m_refreshStorageAction;

    // Schedule Actions
    QAction *m_refreshSchedulesAction;

    // Menus
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_viewMenu;
    QMenu *m_jobsMenu;
    QMenu *m_clientsMenu;
    QMenu *m_storageMenu;
    QMenu *m_schedulesMenu;
    QMenu *m_helpMenu;

    // Toolbar
    QToolBar *m_mainToolBar;
    QPushButton *m_toggleStatisticsButton;

    // Auto-refresh
    QTimer *m_autoRefreshTimer;

    // Window size management
    QSize m_sizeBeforeStatistics;  ///< Fenstergröße vor Einblendung der Statistiken
};

#endif // MAINWINDOW_H
