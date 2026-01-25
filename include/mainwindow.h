#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include "director.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class JobWidget;
class ClientWidget;
class StorageWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Director *director() const;
    void setDirector(Director *newDirector);

private slots:
    void onConnectTriggered();
    void onDisconnectTriggered();
    void onAboutTriggered();
    void onSettingsTriggered();
    void onConnectionChanged(bool connected);
    void onConnectionError(const QString &error);
    void onRefreshAll();
    void onConnectLastUsed();

private:
    void setupUI();
    void createActions();
    void createMenus();
    void createToolBar();
    void showConnectionDialog();
    void updateConnectionStatus(bool connected);
    void loadAndConnectLastUsed();

    Ui::MainWindow *ui;
    Director *m_director;
    
    // Widgets
    QTabWidget *m_tabWidget;
    JobWidget *m_jobWidget;
    ClientWidget *m_clientWidget;
    StorageWidget *m_storageWidget;
    
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
    
    // Menus
    QMenu *m_fileMenu;
    QMenu *m_helpMenu;
    
    // Toolbar
    QToolBar *m_mainToolBar;
};

#endif // MAINWINDOW_H
