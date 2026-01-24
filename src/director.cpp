#include <director.h>

/**
 * @file director.cpp
 * @brief Implementation of Director class for Bacula/Bareos communication
 *
 * The backend (Bacula or Bareos) is determined at compile time via:
 * - USE_BACULA_ONLY: Uses BaculaAuth
 * - USE_BAREOS_ONLY: Uses BareosAuth (default)
 *
 * @note m_backupSystem is deprecated and only kept for backward compatibility.
 *       Use the compile-time constant BACKUP_SYSTEM_NAME instead.
 */

#include <QApplication>
#include <QDebug>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QAuthenticator>
#include <QUrlQuery>
#include <QSslCipher>
#include <QSslError>

// ============================================================================
// Compile-time backend identification
// ============================================================================
#ifdef USE_BACULA_ONLY
static const char* BACKUP_SYSTEM_NAME = "Bacula";
#else
static const char* BACKUP_SYSTEM_NAME = "Bareos";
#endif

Director::Director(QObject *parent)
    : QObject(parent)
    , m_connectionState(Disconnected)
    , m_backupSystem(Bacula)  // @deprecated - use BACKUP_SYSTEM_NAME instead
    , m_sslSocket(nullptr)
    , m_port(9101)
    , m_directorVersion(0)
    , m_auth(nullptr)
    , m_connected(false)
    , m_useApiMode(true)  // API-Modus standardmäßig aktivieren
{
    m_tlsConfig = new TLSConfig();
    m_sslSocket = new QSslSocket(this);

    // Nur SSL Socket-Verbindungen (funktioniert auch für plain TCP)
    QObject::connect(m_sslSocket, &QSslSocket::encrypted, this, &Director::onEncrypted);
    QObject::connect(m_sslSocket, &QSslSocket::connected, this, &Director::onConnected);
    QObject::connect(m_sslSocket, &QSslSocket::disconnected, this, &Director::onDisconnected);
    // QObject::connect(m_sslSocket, &QSslSocket::readyRead, this, &Director::onReadyRead);
    QObject::connect(m_sslSocket, &QSslSocket::errorOccurred, this, &Director::onError);
    QObject::connect(m_sslSocket, QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
                     this, &Director::onSslErrors);

    // Im Konstruktor - behält die Socket-State-Änderungen im Auge
    QObject::connect(m_sslSocket, &QSslSocket::stateChanged, this,
                     [this](QAbstractSocket::SocketState state) {
                         qDebug() << "Socket state changed to:" << state;
                     });

    // Error-Handler mit aussagekräftigen Meldungen
    QObject::connect(m_sslSocket, &QAbstractSocket::errorOccurred, this,
                     [this](QAbstractSocket::SocketError socketError) {
                         QString errorMsg;
                         switch(socketError) {
                         case QAbstractSocket::ConnectionRefusedError:
                             errorMsg = "Verbindung verweigert - ist der Director gestartet?";
                             break;
                         case QAbstractSocket::HostNotFoundError:
                             errorMsg = "Host nicht gefunden - überprüfen Sie den Hostnamen";
                             break;
                         case QAbstractSocket::SocketTimeoutError:
                             errorMsg = "Verbindungs-Timeout - Server antwortet nicht";
                             break;
                         case QAbstractSocket::NetworkError:
                             errorMsg = "Netzwerkfehler - überprüfen Sie Ihre Netzwerkverbindung";
                             break;
                         default:
                             errorMsg = m_sslSocket->errorString();
                         }
                         qCritical() << "Socket-Fehler:" << errorMsg;
                         emit connectionError(errorMsg);
                     });
    // SSL Mode geändert
    QObject::connect(m_sslSocket, &QSslSocket::modeChanged, this,
                     [this](QSslSocket::SslMode mode) {
                         qDebug() << "========================================";
                         qDebug() << "SSL MODE CHANGED";
                         qDebug() << "  Mode:" << (mode == QSslSocket::UnencryptedMode ? "Unencrypted" : "SslClientMode/SslServerMode");
                         qDebug() << "========================================";
                     });

    // Verschlüsselung gestartet
    QObject::connect(m_sslSocket, &QSslSocket::encryptedBytesWritten, this,
                     [this](qint64 written) {
                         qDebug() << "Encrypted bytes written:" << written;
                     });

    // Peer verifiziert (bei PSK oft übersprungen)
    QObject::connect(m_sslSocket, &QSslSocket::peerVerifyError, this,
                     [this](const QSslError &error) {
                         qDebug() << "⚠ Peer verify error:" << error.errorString();
                     });

    // State changes
    QObject::connect(m_sslSocket, &QSslSocket::stateChanged, this,
                     [this](QAbstractSocket::SocketState state) {
                         qDebug() << "Socket state changed to:" << state;
                     });
}

Director::~Director()
{
    disconnect();
    // Clean up
    delete m_tlsConfig;
    m_tlsConfig = nullptr;

    delete m_sslSocket;
    m_sslSocket = nullptr;
}

/**
 * @deprecated Use compile-time backend selection instead.
 * This method is kept for backward compatibility only.
 */
void Director::setBackupSystem(BackupSystem system)
{
    Q_UNUSED(system);
    qWarning() << "setBackupSystem() is deprecated. Backend is determined at compile time:"
               << BACKUP_SYSTEM_NAME;
}

/**
 * @deprecated Use compile-time backend selection instead.
 */
Director::BackupSystem Director::backupSystem() const
{
#ifdef USE_BACULA_ONLY
    return Bacula;
#else
    return Bareos;
#endif
}

/**
 * @brief Returns the name of the compiled backend
 * @return "Bacula" or "Bareos" depending on compile-time configuration
 */
QString Director::backupSystemName() const
{
    return QString(BACKUP_SYSTEM_NAME);
}

void Director::connect(const QString &host, int port, const QString &directorName,
                       const QString &password)
{
    qDebug() << "========================================";
    qDebug() << "CONNECTING TO" << backupSystemName() << "DIRECTOR" << directorName;
    qDebug() << "========================================";
    qDebug() << "Host:" << host;
    qDebug() << "Port:" << port;
    qDebug() << "Password present:" << (!password.isEmpty());
    qDebug() << "========================================";

    if (m_sslSocket->state() != QAbstractSocket::UnconnectedState) {
        m_sslSocket->abort();
        m_sslSocket->waitForDisconnected(1000);
    }

    m_connectionState = Connecting;
    m_host = host;
    m_port = port;
    m_directorName = directorName;
    m_password = password;


    qDebug() << "Connecting via plain TCP...";
    if (m_tlsConfig->tlsEnable) {
    qDebug() << "(TLS/PSK will be negotiated after 'starttls' response)";
    }

    // Nur TCP verbinden - TLS kommt NACH dem "starttls" vom Director
    m_sslSocket->connectToHost(host, port);
}

void Director::disconnect()
{
    // Cleanup authentication
    if (m_auth) {
        m_auth->deleteLater();
        m_auth = nullptr;
    }

    // Disconnect socket
    if (m_sslSocket->state() == QAbstractSocket::ConnectedState) {
        sendCommand("quit");
        m_sslSocket->disconnectFromHost();
    }

    m_connected = false;
    m_connectionState = Disconnected;
}

bool Director::isConnected() const
{
    return m_connected && m_connectionState == Ready;
}

Director::ConnectionState Director::connectionState() const
{
    return m_connectionState;
}

void Director::setTLSConfig(const TLSConfig &config)
{
    if (!m_tlsConfig) {
        m_tlsConfig = new TLSConfig();
    }
    *m_tlsConfig = config;
}

Director::TLSConfig *Director::tlsConfig() const
{
    if (m_tlsConfig) {
        return m_tlsConfig;
    }
    return new TLSConfig();
}

// ============================================================================
// Socket Event Handlers
// ============================================================================

void Director::onConnected()
{
    qDebug() << "========================================";
    qDebug() << "TCP CONNECTED";
    qDebug() << "========================================";

    startAuthentication();
}

void Director::onEncrypted()
{
    qDebug() << "========================================";
    qDebug() << "✓ PSK-TLS HANDSHAKE SUCCESSFUL";
    qDebug() << "========================================";
    qDebug() << "  Encrypted: true";
    qDebug() << "  Protocol:" << m_sslSocket->sessionProtocol();
    qDebug() << "  Cipher:" << m_sslSocket->sessionCipher().name();

    if (!m_sslSocket->peerCertificate().isNull()) {
        qDebug() << "  Peer Certificate:";
        qDebug() << "    Subject:" << m_sslSocket->peerCertificate().subjectInfo(QSslCertificate::CommonName);
    }
    qDebug() << "========================================";

    // WICHTIG: Nicht startAuthentication() aufrufen!
    // Bei PSK-TLS wird die Authentifizierung von BareosAuth::onEncrypted() fortgesetzt
    // Bei Nicht-PSK wird startAuthentication() von onConnected() aufgerufen
}

void Director::onSslErrors(const QList<QSslError> &errors)
{
    qWarning() << "========================================";
    qWarning() << "SSL ERRORS OCCURRED (" << errors.size() << "errors)";
    qWarning() << "========================================";

    for (const QSslError &error : errors) {
        qWarning() << "  Error:" << error.errorString();
        qWarning() << "  Error Type:" << error.error();
        if (!error.certificate().isNull()) {
            qWarning() << "  Certificate:";
            qWarning() << "    Subject:" << error.certificate().subjectInfo(QSslCertificate::CommonName);
            qWarning() << "    Issuer:" << error.certificate().issuerInfo(QSslCertificate::CommonName);
            qWarning() << "    Valid from:" << error.certificate().effectiveDate();
            qWarning() << "    Valid until:" << error.certificate().expiryDate();
        }
    }

    qWarning() << "========================================";

    if (!m_password.isEmpty()) {
        qWarning() << "⚠ PSK-Mode: Ignoring SSL errors (this is normal for PSK)";
        m_sslSocket->ignoreSslErrors();
    } else if (!m_tlsConfig->tlsVerifyPeer) {
        qWarning() << "⚠ Peer verification disabled - ignoring SSL errors";
        m_sslSocket->ignoreSslErrors();
    } else {
        qCritical() << "✗ SSL errors with peer verification enabled - connection will fail";
        m_connectionState = Disconnected;
        QString errorSummary = QString("%1 SSL error(s): %2")
                                   .arg(errors.size())
                                   .arg(errors.first().errorString());
        emit connectionError("SSL-Fehler: " + errorSummary);
    }
}

void Director::onDisconnected()
{
    qDebug() << "========================================";
    qDebug() << "CONNECTION CLOSED";
    qDebug() << "========================================";

    m_connected = false;
    m_connectionState = Disconnected;

    // Cleanup
    if (m_auth) {
        m_auth->deleteLater();
        m_auth = nullptr;
    }

    emit disconnected();
}

void Director::onReadyRead()
{
    // Während der Authentifizierung handhabt die Auth-Klasse alle Daten
    if (m_connectionState == Authenticating) {
        // Auth-Klasse liest direkt vom Socket
        return;
    }

    // Nach Authentifizierung: Normale Kommando-Antworten verarbeiten
    QByteArray data = m_sslSocket->readAll();

    m_receiveBuffer.append(data);

    // Verarbeite vollständige Antworten
    m_receiveBuffer.clear();
}

void Director::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    QString errorMsg = QString("Socket-Fehler: %1").arg(m_sslSocket->errorString());

    qCritical() << errorMsg;
    m_connectionState = Disconnected;
    emit connectionError(errorMsg);
}

// ============================================================================
// TLS Setup
// ============================================================================

bool Director::setupTLSConnection()
{
    qDebug() << "========================================";
    qDebug() << "TLS CONFIGURATION";
    qDebug() << "========================================";

    QSslConfiguration sslConfig = m_sslSocket->sslConfiguration();

    // Load certificates
    if (!loadTLSCertificates(sslConfig)) {
        qCritical() << "Failed to load TLS certificates";
        return false;
    }

    // Set peer verification mode
    if (m_tlsConfig->tlsVerifyPeer) {
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
        qDebug() << "Peer verification: ENABLED";
    } else {
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        qDebug() << "Peer verification: DISABLED";
    }

    // Set TLS version
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);
    qDebug() << "TLS Protocol: TLS 1.2 or later";

    // Apply configuration
    m_sslSocket->setSslConfiguration(sslConfig);

    qDebug() << "✓ TLS configuration complete";
    qDebug() << "========================================";

    return true;
}

bool Director::loadTLSCertificates(QSslConfiguration &sslConfig)
{
    // Load CA certificate
    if (m_tlsConfig->tlsCaCertFile && !m_tlsConfig->tlsCaCertFile->fileName().isEmpty()) {
        if (!m_tlsConfig->tlsCaCertFile->open(QIODevice::ReadOnly)) {
            qCritical() << "Failed to open CA certificate:" << m_tlsConfig->tlsCaCertFile->fileName();
            return false;
        }

        QList<QSslCertificate> caCerts = QSslCertificate::fromDevice(m_tlsConfig->tlsCaCertFile.data(), QSsl::Pem);
        m_tlsConfig->tlsCaCertFile->close();

        if (caCerts.isEmpty()) {
            qCritical() << "No CA certificates found in:" << m_tlsConfig->tlsCaCertFile->fileName();
            return false;
        }

        sslConfig.setCaCertificates(caCerts);
    }

#ifdef Q_OS_WINDOWS
    // Windows: Load PKCS#12 file
    if (m_tlsConfig->tlsPfxFile && !m_tlsConfig->tlsPfxFile->fileName().isEmpty()) {
        if (!m_tlsConfig->tlsPfxFile->open(QIODevice::ReadOnly)) {
            qCritical() << "Failed to open PFX file:" << m_tlsConfig->tlsPfxFile->fileName();
            return false;
        }

        QSslCertificate certificate;
        QSslKey privateKey;
        QList<QSslCertificate> caCerts;

        bool imported = QSslCertificate::importPkcs12(
            m_tlsConfig->tlsPfxFile.data(),
            &privateKey,
            &certificate,
            &caCerts,
            m_tlsConfig->tlsPfxPassword.toUtf8()
            );

        m_tlsConfig->tlsPfxFile->close();

        if (!imported) {
            qCritical() << "Failed to import PKCS#12 certificate";
            return false;
        }

        if (certificate.isNull() || privateKey.isNull()) {
            qCritical() << "Certificate or private key is null";
            return false;
        }

        sslConfig.setLocalCertificate(certificate);
        sslConfig.setPrivateKey(privateKey);

        if (!caCerts.isEmpty()) {
            sslConfig.setCaCertificates(caCerts);
        }
    }
#else
    // Linux: Load separate PEM files
    if (m_tlsConfig->tlsCertFile && !m_tlsConfig->tlsCertFile->fileName().isEmpty()) {
        if (!m_tlsConfig->tlsCertFile->open(QIODevice::ReadOnly)) {
            qCritical() << "Failed to open certificate file:" << m_tlsConfig->tlsCertFile->fileName();
            return false;
        }

        QSslCertificate certificate(m_tlsConfig->tlsCertFile.data(), QSsl::Pem);
        m_tlsConfig->tlsCertFile->close();

        if (certificate.isNull()) {
            qCritical() << "Failed to load certificate";
            return false;
        }

        sslConfig.setLocalCertificate(certificate);
        qDebug() << "✓ Certificate loaded:" << m_tlsConfig->tlsCertFile->fileName();
    }

    if (m_tlsConfig->tlsKeyFile && !m_tlsConfig->tlsKeyFile->fileName().isEmpty()) {
        if (!m_tlsConfig->tlsKeyFile->open(QIODevice::ReadOnly)) {
            qCritical() << "Failed to open key file:" << m_tlsConfig->tlsKeyFile->fileName();
            return false;
        }

        QSslKey privateKey(m_tlsConfig->tlsKeyFile.data(), QSsl::Rsa, QSsl::Pem);
        m_tlsConfig->tlsKeyFile->close();

        if (privateKey.isNull()) {
            qCritical() << "Failed to load private key";
            return false;
        }

        sslConfig.setPrivateKey(privateKey);
        qDebug() << "✓ Private key loaded:" << m_tlsConfig->tlsKeyFile->fileName();
    }
#endif

    return true;
}

// ============================================================================
// Authentication
// ============================================================================

void Director::startAuthentication()
{
    qDebug() << "========================================";
    qDebug() << "STARTING AUTHENTICATION";
    qDebug() << "========================================";

    m_connectionState = Authenticating;

    // Erstelle die passende Auth-Klasse basierend auf AUTH_CLASS Makro
    // AUTH_CLASS wird in director.h definiert als BaculaAuth oder BareosAuth
    m_auth = new AUTH_CLASS(m_sslSocket, this);

    // Connect authentication signals - verwende AUTH_CLASS für korrekten Typ
    QObject::connect(m_auth, &AUTH_CLASS::authenticationSucceeded,
                     this, &Director::onAuthenticationSucceeded);
    QObject::connect(m_auth, &AUTH_CLASS::authenticationFailed,
                     this, &Director::onAuthenticationFailed);
    QObject::connect(m_auth, &AUTH_CLASS::statusMessage,
                     this, &Director::onAuthStatusMessage);
    
    // PSK wenn Passwort vorhanden
    bool usePSK = !m_password.isEmpty() && m_tlsConfig->tlsEnable;  

    qDebug() << "PSK will be used:" << usePSK << "(password present:" << !m_password.isEmpty() << ")";

    // Start authentication
    bool authentificated = m_auth->authenticateDirector(
        m_directorName,
        PROJECT_NAME,
        m_password,
        m_tlsConfig->tlsEnable,
        m_tlsConfig->tlsRequire,
        m_tlsConfig->tlsVerifyPeer,
        usePSK
        );

    if (!authentificated) {
        qCritical() << "Failed to start authentication:" << m_auth->getErrorMessage();
        onAuthenticationFailed(m_auth->getErrorMessage());
    }
}

void Director::onAuthenticationSucceeded(int directorVersion)
{
    qDebug() << "========================================";
    qDebug() << "✓ AUTHENTICATION SUCCESSFUL";
    qDebug() << "  Director Version:" << directorVersion;
    qDebug() << "========================================";

    m_directorVersion = directorVersion;
    m_connectionState = Ready;
    m_connected = true;

    // Cleanup auth object
    m_auth->deleteLater();
    m_auth = nullptr;

    // Enable compression if Director supports it
    if (m_directorVersion >= 1) {
        qDebug() << "Director supports compression";
    }

    // Save connection settings
    saveConnectionSettings();

    // Optional: Activate API mode (like BAT)
    if (m_useApiMode) {
        qDebug() << "Activating API mode (.api 1)...";
        sendCommand(".api 1");
    }

    emit authenticationSucceeded();
    emit connected();
}

void Director::onAuthenticationFailed(const QString &reason)
{
    qCritical() << "========================================";
    qCritical() << "✗ AUTHENTICATION FAILED";
    qCritical() << "  Reason:" << reason;
    qCritical() << "========================================";

    m_connectionState = Disconnected;
    m_connected = false;

    // Cleanup
    if (m_auth) {
        m_auth->deleteLater();
        m_auth = nullptr;
    }

    // Disconnect socket
    m_sslSocket->disconnectFromHost();

    emit authenticationFailed(reason);
    emit connectionError(reason);
}

void Director::onAuthStatusMessage(const QString &message)
{
    qDebug() << "[Auth]" << message;
    emit statusMessage(message);
}

// ============================================================================
// Command Handling
// ============================================================================

void Director::sendCommand(const QString &command)
{
    if (m_connectionState != Ready) {
        qWarning() << "Cannot send command - not ready (state:" << m_connectionState << ")";
        return;
    }

    qDebug() << ">>> Sending command:" << command;

    QString msg = command + "\n";
    QByteArray data = msg.toUtf8();

    qint64 written = m_sslSocket->write(data);
    m_sslSocket->flush();

    if (written != data.size()) {
        qCritical() << "Failed to send complete command! Written:" << written << "Expected:" << data.size();
    } else {
        qDebug() << "✓ Command sent successfully";
    }

    m_lastCommand = command;
}

void Director::sendToDirector(const QByteArray &data)
{
    m_sslSocket->write(data);
    m_sslSocket->flush();
}

// ============================================================================
// Bconsole Commands
// ============================================================================

void Director::listJobs(int limit)
{
    QString cmd = QString("list jobs last=%1").arg(limit);
    sendCommand(cmd);
}

void Director::listClients()
{
    sendCommand("list clients");
}

void Director::listPools()
{
    sendCommand("list pools");
}

void Director::listVolumes()
{
    sendCommand("list volumes");
}

void Director::showJobDetails(int jobId)
{
    QString cmd = QString("list jobid=%1").arg(jobId);
    sendCommand(cmd);
}

void Director::runJob(const QString &jobName)
{
    QString cmd = QString("run job=\"%1\" yes").arg(jobName);
    sendCommand(cmd);
}

void Director::cancelJob(int jobId)
{
    QString cmd = QString("cancel jobid=%1").arg(jobId);
    sendCommand(cmd);
}

void Director::restoreFiles(const QString &clientName, const QString &fileSet)
{
    QString cmd = QString("restore client=\"%1\" fileset=\"%2\"").arg(clientName, fileSet);
    sendCommand(cmd);
}

void Director::statusDirector()
{
    sendCommand("status director");
}

void Director::statusStorage(const QString &storageName)
{
    QString cmd = QString("status storage=\"%1\"").arg(storageName);
    sendCommand(cmd);
}

void Director::statusClient(const QString &clientName)
{
    QString cmd = QString("status client=\"%1\"").arg(clientName);
    sendCommand(cmd);
}

// ============================================================================
// Settings Management
// ============================================================================

void Director::saveConnectionSettings()
{
    QSettings settings("Bacula", QCoreApplication::applicationName());
    settings.beginGroup("BaculaConnection");
    settings.setValue("host", m_host);
    settings.setValue("port", m_port);
    settings.setValue("directorName", m_directorName);
    settings.setValue("backupSystem", static_cast<int>(m_backupSystem));
    settings.setValue("tlsEnable", m_tlsConfig->tlsEnable);
    settings.setValue("tlsRequire", m_tlsConfig->tlsRequire);
    settings.setValue("tlsVerifyPeer", m_tlsConfig->tlsVerifyPeer);

    // Don't save passwords or certificate paths for security
    settings.endGroup();

    qDebug() << "Connection settings saved";
}

void Director::loadConnectionSettings()
{
    QSettings settings("Bacula", QCoreApplication::applicationName());
    settings.beginGroup("BaculaConnection");
    m_host = settings.value("host").toString();
    m_port = settings.value("port", 9101).toInt();
    m_directorName = settings.value("directorName").toString();
    m_backupSystem = static_cast<BackupSystem>(settings.value("backupSystem", Bacula).toInt());
    m_tlsConfig->tlsEnable = settings.value("tlsEnable", false).toBool();
    m_tlsConfig->tlsRequire = settings.value("tlsRequire", false).toBool();
    m_tlsConfig->tlsVerifyPeer = settings.value("tlsVerifyPeer", false).toBool();
    settings.endGroup();

    qDebug() << "Connection settings loaded";
}

bool Director::hasStoredConnection() const
{
    QSettings settings("Bacula", QCoreApplication::applicationName());
    settings.beginGroup("BaculaConnection");
    bool hasConnection = settings.contains("host") && settings.contains("port");
    settings.endGroup();
    return hasConnection;
}

// ============================================================================
// Helper Methods
// ============================================================================

Director::JobStatus Director::parseJobStatus(const QString &status)
{
    QString s = status.toUpper();

    if (s == "C" || s == "CREATED") return Created;
    if (s == "R" || s == "RUNNING") return Running;
    if (s == "B" || s == "BLOCKED") return Blocked;
    if (s == "T" || s == "TERMINATED") return Terminated;
    if (s == "W" || s == "WAITING") return Waiting;
    if (s == "S" || s == "SUCCESSFUL") return Successful;
    if (s == "E" || s == "ERROR") return Error;
    if (s == "F" || s == "FATAL") return Fatal;
    if (s == "A" || s == "CANCELED") return Canceled;

    return Unknown;
}

QString Director::jobStatusToString(JobStatus status)
{
    switch (status) {
    case Created: return "Created";
    case Running: return "Running";
    case Blocked: return "Blocked";
    case Terminated: return "Terminated";
    case Waiting: return "Waiting";
    case Successful: return "Successful";
    case Error: return "Error";
    case Fatal: return "Fatal";
    case Canceled: return "Canceled";
    default: return "Unknown";
    }
}
