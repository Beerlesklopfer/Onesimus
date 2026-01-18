#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jobwidget.h"
#include "clientwidget.h"
#include "storagewidget.h"
#include "settingsdialog.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QSettings>
#include <QCheckBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_director(nullptr)
    , m_statusLabel(nullptr)
    , m_connectionLabel(nullptr)
{
    ui->setupUi(this);
    
    setWindowTitle("Onesimus - Bacula Backup Management");
    resize(1200, 800);
    
    m_director = new BaculaDirector(this);

    setupUI();
    createActions();
    createMenus();
    createToolBar();
    
    // Verbinde Signals
    connect(m_director, &BaculaDirector::connected, this, [this]() {
        onConnectionChanged(true);
    });
    connect(m_director, &BaculaDirector::disconnected, this, [this]() {
        onConnectionChanged(false);
    });
    connect(m_director, &BaculaDirector::connectionError, this, &MainWindow::onConnectionError);
    
    updateConnectionStatus(false);
    
    // Automatisch mit letzter Verbindung verbinden, falls vorhanden
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
    
    // Job-Widget
    m_jobWidget = new JobWidget(m_director, this);
    m_tabWidget->addTab(m_jobWidget, "Jobs");
    
    // Client-Widget
    m_clientWidget = new ClientWidget(m_director, this);
    m_tabWidget->addTab(m_clientWidget, "Clients");
    
    // Storage-Widget
    m_storageWidget = new StorageWidget(m_director, this);
    m_tabWidget->addTab(m_storageWidget, "Storage/Volumes");

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
    
    m_connectLastAction = new QAction("Letzte Verbindung", this);
    m_connectLastAction->setIcon(QIcon::fromTheme("document-open-recent"));
    m_connectLastAction->setShortcut(QKeySequence("Ctrl+L"));
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
}

void MainWindow::onConnectTriggered()
{
    showConnectionDialog();
}

void MainWindow::showConnectionDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Bacula Director Verbindung");
    dialog.resize(450, 450);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    
    // Lade gespeicherte Einstellungen
    m_director->loadConnectionSettings();
    
#ifdef USE_BAREOS
    // Verbindungstyp
    QGroupBox *typeGroup = new QGroupBox("Verbindungstyp", &dialog);
    QVBoxLayout *typeLayout = new QVBoxLayout(typeGroup);
    
    QRadioButton *bconsoleRadio = new QRadioButton("Bconsole (TCP)", typeGroup);
    QRadioButton *restRadio = new QRadioButton("REST API (HTTP/HTTPS)", typeGroup);
    
    // Setze gespeicherten Verbindungstyp als Standard
    if (m_director->hasStoredConnection()) {
        if (m_director->connectionType() == BaculaDirector::BConsole) {
            bconsoleRadio->setChecked(true);
        } else {
            restRadio->setChecked(true);
        }
    } else {
        bconsoleRadio->setChecked(true);
    }
    
    typeLayout->addWidget(bconsoleRadio);
    typeLayout->addWidget(restRadio);
    mainLayout->addWidget(typeGroup);    
#endif

    // Bconsole-Felder mit gespeicherten Werten
    QGroupBox *bconsoleGroup = new QGroupBox("Bconsole-Verbindung", &dialog);
    QFormLayout *bconsoleLayout = new QFormLayout(bconsoleGroup);
    
    QSettings settings("Bacula", "Onesimus");
    settings.beginGroup("Connection");
    QLineEdit *hostEdit = new QLineEdit(settings.value("bconsole_host", "localhost").toString(), &dialog);
    QSpinBox *portSpin = new QSpinBox(&dialog);
    portSpin->setRange(1, 65535);
    portSpin->setValue(settings.value("/bconsole_port", 9101).toInt());
    QLineEdit *directorEdit = new QLineEdit(settings.value("bconsole_director", "bacula-dir").toString(), &dialog);
    QLineEdit *passwordEdit = new QLineEdit(&dialog);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setText(settings.value("bconsole_password").toString());
    
    bconsoleLayout->addRow("Host:", hostEdit);
    bconsoleLayout->addRow("Port:", portSpin);
    bconsoleLayout->addRow("Director Name:", directorEdit);
    bconsoleLayout->addRow("Passwort:", passwordEdit);
    
    // TLS-Konfiguration
    QCheckBox *tlsCheckBox = new QCheckBox("TLS/SSL verwenden", &dialog);
    tlsCheckBox->setChecked(settings.value("tls_enabled", false).toBool());
    bconsoleLayout->addRow("", tlsCheckBox);

    QLineEdit *caCertEdit = new QLineEdit(settings.value("tls_ca_cert_file").toString(), &dialog);
    QPushButton *caCertBrowse = new QPushButton("...", &dialog);
    caCertBrowse->setMaximumWidth(30);
    QHBoxLayout *caCertLayout = new QHBoxLayout();
    caCertLayout->addWidget(caCertEdit);
    caCertLayout->addWidget(caCertBrowse);
    bconsoleLayout->addRow("TLS CA Certificate:", caCertLayout);

    connect(caCertBrowse, &QPushButton::clicked, [&]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "CA-Zertifikat wählen",
                                                    QString(), "Zertifikate (*.pem *.crt *.cert);;Alle Dateien (*)");
        if (!file.isEmpty()) caCertEdit->setText(file);
    });

#ifdef Q_OS_WINDOWS

    QLineEdit *pfxFileEdit = new QLineEdit(settings.value("tls_pfx_file").toString(), &dialog);
    QLineEdit *pfxPasswordEdit = new QLineEdit(settings.value("tls_pfx_password").toString(), &dialog);
    pfxPasswordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    QPushButton *certBrowse = new QPushButton("...", &dialog);
    certBrowse->setMaximumWidth(30);
    QGridLayout *pfxLayout = new QGridLayout();
    pfxLayout->addWidget(new QLabel("PFX Certificate:"), 0, 0);
    pfxLayout->addWidget(pfxFileEdit, 0, 1);
    pfxLayout->addWidget(certBrowse, 0, 2);
    pfxLayout->addWidget(new QLabel("PFX Password:"), 1, 0);
    pfxLayout->addWidget(pfxPasswordEdit, 1, 1, 1, 2);
    bconsoleLayout->addRow(pfxLayout);

    // TLS-Felder initial aktivieren/deaktivieren
    auto updateTlsFields = [=]() {
        bool enabled = tlsCheckBox->isChecked();
        caCertEdit->setEnabled(enabled);
        caCertBrowse->setEnabled(enabled);
        pfxFileEdit->setEnabled(enabled);
        pfxPasswordEdit->setEnabled(enabled);
        certBrowse->setEnabled(enabled);
    };
    updateTlsFields();
    connect(tlsCheckBox, &QCheckBox::toggled, updateTlsFields);

    connect(certBrowse, &QPushButton::clicked, [&]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "Client-Zertifikat wählen",
                                                    QString(), "Zertifikate (*.pfx);;Alle Dateien (*)");
        if (!file.isEmpty()) pfxFileEdit->setText(file);
    });
#else
    
    QLineEdit *certEdit = new QLineEdit(settings.value("Connection/tls_cert_file").toString(), &dialog);
    QPushButton *certBrowse = new QPushButton("...", &dialog);
    certBrowse->setMaximumWidth(30);
    QHBoxLayout *certLayout = new QHBoxLayout();
    certLayout->addWidget(certEdit);
    certLayout->addWidget(certBrowse);
    bconsoleLayout->addRow("TLS Certificate:", certLayout);
    
    QLineEdit *keyEdit = new QLineEdit(settings.value("Connection/tls_key_file").toString(), &dialog);
    QPushButton *keyBrowse = new QPushButton("...", &dialog);
    keyBrowse->setMaximumWidth(30);
    QHBoxLayout *keyLayout = new QHBoxLayout();
    keyLayout->addWidget(keyEdit);
    keyLayout->addWidget(keyBrowse);
    bconsoleLayout->addRow("TLS Key:", keyLayout);
    
    // TLS-Felder initial aktivieren/deaktivieren
    auto updateTlsFields = [=]() {
        bool enabled = tlsCheckBox->isChecked();
        caCertEdit->setEnabled(enabled);
        caCertBrowse->setEnabled(enabled);
        certEdit->setEnabled(enabled);
        certBrowse->setEnabled(enabled);
        keyEdit->setEnabled(enabled);
        keyBrowse->setEnabled(enabled);
    };
    updateTlsFields();
    connect(tlsCheckBox, &QCheckBox::toggled, updateTlsFields);

    // Datei-Browser für Zertifikate
    
    connect(certBrowse, &QPushButton::clicked, [&]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "Client-Zertifikat wählen", 
            QString(), "Zertifikate (*.pem *.crt *.cert);;Alle Dateien (*)");
        if (!file.isEmpty()) certEdit->setText(file);
    });
    
    connect(keyBrowse, &QPushButton::clicked, [&]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "Private Key wählen", 
            QString(), "Keys (*.pem *.key);;Alle Dateien (*)");
        if (!file.isEmpty()) keyEdit->setText(file);
    });
    
#endif
    mainLayout->addWidget(bconsoleGroup);
    
#ifdef USE_BAREOS
    // REST-API-Felder mit gespeicherten Werten
    QGroupBox *restGroup = new QGroupBox("REST-API-Verbindung", &dialog);
    QFormLayout *restLayout = new QFormLayout(restGroup);
    
    QLineEdit *urlEdit = new QLineEdit(settings.value("Connection/rest_baseurl", "http://localhost:9101").toString(), &dialog);
    QLineEdit *usernameEdit = new QLineEdit(settings.value("Connection/rest_username", "admin").toString(), &dialog);
    QLineEdit *restPasswordEdit = new QLineEdit(&dialog);
    restPasswordEdit->setEchoMode(QLineEdit::Password);
    restPasswordEdit->setText(settings.value("Connection/rest_password").toString());
    
    restLayout->addRow("Base URL:", urlEdit);
    restLayout->addRow("Benutzername:", usernameEdit);
    restLayout->addRow("Passwort:", restPasswordEdit);
    mainLayout->addWidget(restGroup);
    // Toggle zwischen Bconsole und REST basierend auf gespeichertem Typ
    if (bconsoleRadio->isChecked()) {
        bconsoleGroup->setVisible(true);
        restGroup->setVisible(false);
    } else {
        bconsoleGroup->setVisible(false);
        restGroup->setVisible(true);
    }

    connect(restRadio, &QRadioButton::toggled, restGroup, &QGroupBox::setVisible);
    connect(bconsoleRadio, &QRadioButton::toggled, bconsoleGroup, &QGroupBox::setVisible);
#endif

    
    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    mainLayout->addWidget(buttonBox);

    if (dialog.exec() == QDialog::Accepted) {

        settings.setValue("tls_enabled", tlsCheckBox->isChecked());
        settings.setValue("bconsole_host", hostEdit->text());
        settings.setValue("bconsole_port", portSpin->value());
        settings.setValue("bconsole_director", directorEdit->text());

        if (tlsCheckBox->isChecked()) {
            BaculaDirector::TLSConfig tlsConfig;
            tlsConfig.tlsEnable = tlsCheckBox->isChecked();

             if(!caCertEdit->text().isEmpty())
            {
                tlsConfig.tlsCaCertFile->setFileName(caCertEdit->text());
                settings.setValue("tls_ca_cert_file",caCertEdit->text());
            }

#ifdef Q_OS_WINDOWS
        if(!pfxFileEdit->text().isEmpty()){
            tlsConfig.tlsPfxFile->setFileName(pfxFileEdit->text());
            settings.setValue("tls_pfx_file",pfxFileEdit->text());
        }

        if(!pfxPasswordEdit->text().isEmpty()){
            tlsConfig.tlsPfxPassword = pfxPasswordEdit->text();
            settings.setValue("tls_pfx_file",pfxPasswordEdit->text());
        }
#else
            tlsConfig.tlsCertFile->setFileName(certEdit->text());
            tlsConfig.tlsKeyFile = "";
#endif

#ifdef USE_BAREOS

#ifdef Q_OS_WINDOWS
#else
            tlsConfig.caCertFile = caCertEdit->text();
            tlsConfig.keyFile = keyEdit->text();
#endif //Q_OS_WINDOWS
            m_director->connectBConsole(
                hostEdit->text(),
                portSpin->value(),
                directorEdit->text(),
                passwordEdit->text(),
                tlsConfig
            );
        }
        else {
            m_director->connectRestAPI(
                urlEdit->text(),
                usernameEdit->text(),
                restPasswordEdit->text()
            );
        }
#else


    m_director->connectBConsole(
        hostEdit->text(),
        portSpin->value(),
        directorEdit->text(),
        passwordEdit->text(),
        tlsConfig
        );

#endif
        settings.endGroup();
        m_statusLabel->setText("Verbindung wird hergestellt...");
    }
}
}

void MainWindow::onDisconnectTriggered()
{
    m_director->disconnect();
    m_statusLabel->setText("Getrennt");
}

void MainWindow::onAboutTriggered()
{
    QMessageBox::about(this, "Über Onesimus",
        "<h3>Onesimus v1.0</h3>"
        "<p>Eine moderne Qt-Oberfläche für Bacula Backup</p>"
        "<p>Unterstützt:</p>"
        "<ul>"
        "<li>Bconsole TCP-Verbindung</li>"
        "<li>REST-API-Verbindung</li>"
        "<li>Job-Verwaltung</li>"
        "<li>Client-Verwaltung</li>"
        "<li>Storage/Volume-Verwaltung</li>"
        "</ul>"
        "<p>© 2026</p>");
}

void MainWindow::onSettingsTriggered()
{
    SettingsDialog dialog(m_director, this);
    if (dialog.exec() == QDialog::Accepted) {
        // Einstellungen wurden geändert
        m_statusLabel->setText("Einstellungen gespeichert");
        
        // Optional: Einstellungen neu laden
        // z.B. Auto-Refresh aktivieren/deaktivieren
    }
}

void MainWindow::onConnectionChanged(bool connected)
{
    updateConnectionStatus(connected);
    
    if (connected) {
        m_statusLabel->setText("Verbunden");
        onRefreshAll();
    } else {
        m_statusLabel->setText("Nicht verbunden");
    }
}

void MainWindow::onConnectionError(const QString &error)
{
    QMessageBox::critical(this, "Verbindungsfehler", error);
    m_statusLabel->setText("Fehler: " + error);
}

void MainWindow::updateConnectionStatus(bool connected)
{
    m_connectAction->setEnabled(!connected);
    m_disconnectAction->setEnabled(connected);
    m_refreshAction->setEnabled(connected);
    
    if (connected) {
        QString connType = (m_director->connectionType() == BaculaDirector::BConsole) 
                          ? "Bconsole" : "REST-API";
        m_connectionLabel->setText("Verbunden (" + connType + ")");
        m_connectionLabel->setStyleSheet("color: green; font-weight: bold;");
    } else {
        m_connectionLabel->setText("Nicht verbunden");
        m_connectionLabel->setStyleSheet("color: red; font-weight: bold;");
    }
}

void MainWindow::onRefreshAll()
{
    if (!m_director->isConnected()) {
        return;
    }
    
    m_statusLabel->setText("Aktualisiere Daten...");
    
    if (m_director->connectionType() == BaculaDirector::RestAPI) {
        m_director->restGetJobs();
        m_director->restGetClients();
        m_director->restGetVolumes();
    } else {
        m_director->listJobs();
        m_director->listClients();
        m_director->listVolumes();
    }
    
    m_statusLabel->setText("Aktualisierung abgeschlossen");
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

void MainWindow::loadAndConnectLastUsed()
{
    if (!m_director->hasStoredConnection()) {
        return;
    }
    
    m_director->loadConnectionSettings();
    
  /*   QSettings settings("Bacula", "Onesimus");
   BaculaDirector::ConnectionType type = static_cast<BaculaDirector::ConnectionType>(
        settings.value("Connection/type", 0).toInt()
    );
    
    if (type == BaculaDirector::BConsole) {
        QString host = settings.value("Connection/bconsole_host", "localhost").toString();
        int port = settings.value("Connection/bconsole_port", 9101).toInt();
        QString director = settings.value("Connection/bconsole_director", "bacula-dir").toString();
        QString password = settings.value("Connection/bconsole_password").toString();
        
        BaculaDirector::TLSConfig tlsConfig;
        tlsConfig.tlsEnable = settings.value("Connection/tls_enabled", false).toBool();
        tlsConfig.tlsCaCertFile->setFileName(settings.value("Connection/tls_ca_cert_file").toString());

#ifdef Q_OS_WINDOWS
        tlsConfig.tlsPfxFile->setFileName(settings.value("Connection/tls_pfx_file").toString());
        tlsConfig.tlsPfxPassword =settings.value("Connection/tls_pfx_password").toString();
#else
        tlsConfig.tlsCertFile->setFileName(settings.value("Connection/tls_cert_file").toString());
        tlsConfig.tlsKeyFile->setFileName(settings.value("Connection/tls_key_file").toString());
#endif
        if (!password.isEmpty()) {
            m_director->connectBConsole(host, port, director, password, tlsConfig);
            m_statusLabel->setText("Verbinde mit Bconsole...");
        }
    } else {
        QString baseUrl = settings.value("Connection/rest_baseurl", "http://localhost:9101").toString();
        QString username = settings.value("Connection/rest_username", "admin").toString();
        QString password = settings.value("Connection/rest_password").toString();
        
        if (!password.isEmpty()) {
            m_director->connectRestAPI(baseUrl, username, password);
            m_statusLabel->setText("Verbinde mit REST-API...");
        }
    }*/
    
    // Aktualisiere "Letzte Verbindung" Button Status
    m_connectLastAction->setEnabled(true);
}
