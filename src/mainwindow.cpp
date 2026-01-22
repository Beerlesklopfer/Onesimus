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
    
    m_director = new Director(this);

    setupUI();
    createActions();
    createMenus();
    createToolBar();
    
    // Verbinde Signals
    connect(m_director, &Director::connected, this, [this]() {
        onConnectionChanged(true);
    });
    connect(m_director, &Director::disconnected, this, [this]() {
        onConnectionChanged(false);
    });
    connect(m_director, &Director::connectionError, this, &MainWindow::onConnectionError);
    
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
    dialog.setWindowTitle("Bareos Director Verbindung");
    dialog.resize(500, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

    // Lade gespeicherte Einstellungen
    m_director->loadConnectionSettings();

    QSettings settings("Bacula", QCoreApplication::applicationName());
    settings.beginGroup("Connection");

    // Bconsole-Verbindungsfelder
    QGroupBox *connectionGroup = new QGroupBox("Bareos Director", &dialog);
    QFormLayout *connectionLayout = new QFormLayout(connectionGroup);

    QLineEdit *hostEdit = new QLineEdit(settings.value("host", "localhost").toString(), &dialog);
    QSpinBox *portSpin = new QSpinBox(&dialog);
    portSpin->setRange(1, 65535);
    portSpin->setValue(settings.value("port", 9101).toInt());
    QLineEdit *directorEdit = new QLineEdit(settings.value("director", "bareos-dir").toString(), &dialog);
    QLineEdit *passwordEdit = new QLineEdit(&dialog);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setText(settings.value("password").toString());

    connectionLayout->addRow("Host:", hostEdit);
    connectionLayout->addRow("Port:", portSpin);
    connectionLayout->addRow("Director Name:", directorEdit);
    connectionLayout->addRow("Passwort:", passwordEdit);

    mainLayout->addWidget(connectionGroup);

    // ####### TLS-Konfiguration #######
    QGroupBox *tlsGroup = new QGroupBox("TLS Konfiguration", &dialog);
    QFormLayout *tlsLayout = new QFormLayout(tlsGroup);

    // TLS Enable - Bareos 18.2+ verwendet standardmäßig TLS-PSK
    QCheckBox *tlsEnableCheckBox = new QCheckBox("TLS aktivieren", &dialog);
    tlsEnableCheckBox->setChecked(settings.value("tls_enabled", true).toBool());
    tlsLayout->addRow("", tlsEnableCheckBox);

    // TLS Require
    QCheckBox *tlsRequireCheckBox = new QCheckBox("TLS erzwingen", &dialog);
    tlsRequireCheckBox->setChecked(settings.value("tls_require", true).toBool());
    tlsLayout->addRow("", tlsRequireCheckBox);

    // TLS-PSK - Standard für Bareos 18.2+
    QCheckBox *tlsPSKEnableCheckBox = new QCheckBox("TLS-PSK verwenden (empfohlen für Bareos 18.2+)", &dialog);
    tlsPSKEnableCheckBox->setChecked(settings.value("tls_psk_enabled", true).toBool());
    tlsLayout->addRow("", tlsPSKEnableCheckBox);

    // Verify Peer
    QCheckBox *tlsVerifyPeerCheckBox = new QCheckBox("Gegenstelle verifizieren", &dialog);
    tlsVerifyPeerCheckBox->setChecked(settings.value("tls_verify_peer", false).toBool());
    tlsLayout->addRow("", tlsVerifyPeerCheckBox);

    // CA Certificate
    QLineEdit *caCertEdit = new QLineEdit(settings.value("tls_ca_cert_file").toString(), &dialog);
    QPushButton *caCertBrowse = new QPushButton("...", &dialog);
    caCertBrowse->setMaximumWidth(30);
    QHBoxLayout *caCertLayout = new QHBoxLayout();
    caCertLayout->addWidget(caCertEdit);
    caCertLayout->addWidget(caCertBrowse);
    tlsLayout->addRow("CA Zertifikat:", caCertLayout);

    connect(caCertBrowse, &QPushButton::clicked, [&]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "CA-Zertifikat wählen",
                                                    QString(), "Zertifikate (*.pem *.crt *.cert);;Alle Dateien (*)");
        if (!file.isEmpty()) caCertEdit->setText(file);
    });

#ifdef Q_OS_WINDOWS
    // Windows: PFX-Format
    QLineEdit *pfxFileEdit = new QLineEdit(settings.value("tls_pfx_file").toString(), &dialog);
    QPushButton *pfxBrowse = new QPushButton("...", &dialog);
    pfxBrowse->setMaximumWidth(30);
    QHBoxLayout *pfxLayout = new QHBoxLayout();
    pfxLayout->addWidget(pfxFileEdit);
    pfxLayout->addWidget(pfxBrowse);
    tlsLayout->addRow("PFX Zertifikat:", pfxLayout);

    QLineEdit *pfxPasswordEdit = new QLineEdit(settings.value("tls_pfx_password").toString(), &dialog);
    pfxPasswordEdit->setEchoMode(QLineEdit::Password);
    tlsLayout->addRow("PFX Passwort:", pfxPasswordEdit);

    connect(pfxBrowse, &QPushButton::clicked, [&]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "PFX-Zertifikat wählen",
                                                    QString(), "PKCS#12 (*.pfx *.p12);;Alle Dateien (*)");
        if (!file.isEmpty()) pfxFileEdit->setText(file);
    });

    // TLS-Felder aktivieren/deaktivieren
    auto updateTlsFields = [=]() {
        bool enabled = tlsEnableCheckBox->isChecked();
        bool isPSK = tlsPSKEnableCheckBox->isChecked();
        // Bei PSK werden keine Zertifikatdateien benötigt
        bool needsCerts = enabled && !isPSK;

        tlsRequireCheckBox->setEnabled(enabled);
        tlsPSKEnableCheckBox->setEnabled(enabled);
        tlsVerifyPeerCheckBox->setEnabled(enabled && !isPSK);
        caCertEdit->setEnabled(needsCerts);
        caCertBrowse->setEnabled(needsCerts);
        pfxFileEdit->setEnabled(needsCerts);
        pfxBrowse->setEnabled(needsCerts);
        pfxPasswordEdit->setEnabled(needsCerts);
    };
#else
    // Linux: PEM-Format
    QLineEdit *certEdit = new QLineEdit(settings.value("tls_cert_file").toString(), &dialog);
    QPushButton *certBrowse = new QPushButton("...", &dialog);
    certBrowse->setMaximumWidth(30);
    QHBoxLayout *certLayout = new QHBoxLayout();
    certLayout->addWidget(certEdit);
    certLayout->addWidget(certBrowse);
    tlsLayout->addRow("Zertifikat:", certLayout);

    QLineEdit *keyEdit = new QLineEdit(settings.value("tls_key_file").toString(), &dialog);
    QPushButton *keyBrowse = new QPushButton("...", &dialog);
    keyBrowse->setMaximumWidth(30);
    QHBoxLayout *keyLayout = new QHBoxLayout();
    keyLayout->addWidget(keyEdit);
    keyLayout->addWidget(keyBrowse);
    tlsLayout->addRow("Private Key:", keyLayout);

    connect(certBrowse, &QPushButton::clicked, [&]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "Zertifikat wählen",
                                                    QString(), "Zertifikate (*.pem *.crt *.cert);;Alle Dateien (*)");
        if (!file.isEmpty()) certEdit->setText(file);
    });

    connect(keyBrowse, &QPushButton::clicked, [&]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "Private Key wählen",
                                                    QString(), "Keys (*.pem *.key);;Alle Dateien (*)");
        if (!file.isEmpty()) keyEdit->setText(file);
    });

    // TLS-Felder aktivieren/deaktivieren
    auto updateTlsFields = [=]() {
        bool enabled = tlsEnableCheckBox->isChecked();
        bool isPSK = tlsPSKEnableCheckBox->isChecked();
        // Bei PSK werden keine Zertifikatdateien benötigt
        bool needsCerts = enabled && !isPSK;

        tlsRequireCheckBox->setEnabled(enabled);
        tlsPSKEnableCheckBox->setEnabled(enabled);
        tlsVerifyPeerCheckBox->setEnabled(enabled && !isPSK);
        caCertEdit->setEnabled(needsCerts);
        caCertBrowse->setEnabled(needsCerts);
        certEdit->setEnabled(needsCerts);
        certBrowse->setEnabled(needsCerts);
        keyEdit->setEnabled(needsCerts);
        keyBrowse->setEnabled(needsCerts);
    };
#endif

    updateTlsFields();
    connect(tlsEnableCheckBox, &QCheckBox::toggled, updateTlsFields);
    connect(tlsPSKEnableCheckBox, &QCheckBox::toggled, updateTlsFields);

    mainLayout->addWidget(tlsGroup);

    // Hinweis für Bareos
    QLabel *hintLabel = new QLabel(
        "<i>Hinweis: Bareos 18.2+ verwendet standardmäßig TLS-PSK.<br>"
        "Bei TLS-PSK wird das Director-Passwort als Pre-Shared Key verwendet.</i>", &dialog);
    hintLabel->setWordWrap(true);
    mainLayout->addWidget(hintLabel);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    mainLayout->addWidget(buttonBox);

    if (dialog.exec() == QDialog::Accepted) {
        // Speichere Einstellungen
        settings.setValue("host", hostEdit->text());
        settings.setValue("port", portSpin->value());
        settings.setValue("director", directorEdit->text());
        settings.setValue("password", passwordEdit->text());
        settings.setValue("tls_enabled", tlsEnableCheckBox->isChecked());
        settings.setValue("tls_require", tlsRequireCheckBox->isChecked());
        settings.setValue("tls_psk_enabled", tlsPSKEnableCheckBox->isChecked());
        settings.setValue("tls_verify_peer", tlsVerifyPeerCheckBox->isChecked());
        settings.setValue("tls_ca_cert_file", caCertEdit->text());

#ifdef Q_OS_WINDOWS
        settings.setValue("tls_pfx_file", pfxFileEdit->text());
        settings.setValue("tls_pfx_password", pfxPasswordEdit->text());
#else
        settings.setValue("tls_cert_file", certEdit->text());
        settings.setValue("tls_key_file", keyEdit->text());
#endif
        settings.endGroup();

        // TLS-Konfiguration setzen
        m_director->tlsConfig()->tlsEnable = tlsEnableCheckBox->isChecked();
        m_director->tlsConfig()->tlsRequire = tlsRequireCheckBox->isChecked();
        m_director->tlsConfig()->tlsPSKEnable = tlsPSKEnableCheckBox->isChecked();
        m_director->tlsConfig()->tlsVerifyPeer = tlsVerifyPeerCheckBox->isChecked();

        if (!caCertEdit->text().isEmpty()) {
            m_director->tlsConfig()->tlsCaCertFile->setFileName(caCertEdit->text());
        }

#ifdef Q_OS_WINDOWS
        if (!pfxFileEdit->text().isEmpty()) {
            m_director->tlsConfig()->tlsPfxFile->setFileName(pfxFileEdit->text());
            m_director->tlsConfig()->tlsPfxPassword = pfxPasswordEdit->text();
        }
#else
        if (!certEdit->text().isEmpty()) {
            m_director->tlsConfig()->tlsCertFile->setFileName(certEdit->text());
        }
        if (!keyEdit->text().isEmpty()) {
            m_director->tlsConfig()->tlsKeyFile->setFileName(keyEdit->text());
        }
#endif

        // Verbindung herstellen
        m_director->connect(
            hostEdit->text(),
            portSpin->value(),
            directorEdit->text(),
            passwordEdit->text()
            );

        m_statusLabel->setText("Verbindung wird hergestellt...");
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
        m_connectionLabel->setText("Verbunden (" ")");
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
    
    m_director->listJobs();
    m_director->listClients();
    m_director->listVolumes();
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
   Director::ConnectionType type = static_cast<Director::ConnectionType>(
        settings.value("Connection/type", 0).toInt()
    );
    
    if (type == Director::BConsole) {
        QString host = settings.value("Connection/bconsole_host", "localhost").toString();
        int port = settings.value("Connection/bconsole_port", 9101).toInt();
        QString director = settings.value("Connection/bconsole_director", "bacula-dir").toString();
        QString password = settings.value("Connection/bconsole_password").toString();
        
        Director::TLSConfig tlsConfig;
        m_director->tlsConfig()->tlsEnable = settings.value("Connection/tls_enabled", false).toBool();
        m_director->tlsConfig()->tlsCaCertFile->setFileName(settings.value("Connection/tls_ca_cert_file").toString());

#ifdef Q_OS_WINDOWS
        m_director->tlsConfig()->tlsPfxFile->setFileName(settings.value("Connection/tls_pfx_file").toString());
        m_director->tlsConfig()->tlsPfxPassword =settings.value("Connection/tls_pfx_password").toString();
#else
        m_director->tlsConfig()->tlsCertFile->setFileName(settings.value("Connection/tls_cert_file").toString());
        m_director->tlsConfig()->tlsKeyFile->setFileName(settings.value("Connection/tls_key_file").toString());
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

Director *MainWindow::director() const
{
    return m_director;
}

void MainWindow::setDirector(Director *newDirector)
{
    m_director = newDirector;
}
