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
 *
 * @author Your Name
 * @date 2025
 * @version 1.0.0
 */

#include <director.h>
#include "version.h"

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

// ============================================================================
// Constructor / Destructor
// ============================================================================

Director::Director(QObject *parent)
    : QObject(parent)
    , m_connectionState(Disconnected)
    , m_sslSocket(nullptr)
    , m_port(9101)
    , m_directorVersion("0.0.0")
    , m_auth(nullptr)
    , m_connected(false)
    , m_apiMode(Off)  // API mode enabled by default
{
    m_tlsConfig = new TLSConfig();
    m_sslSocket = new QSslSocket(this);

    // Connect socket signals and store connections
    connectSocketSignals();
}

Director::~Director()
{
    disconnect();

    // Disconnect all signals
    disconnectAllSignals();

    // Clean up
    delete m_tlsConfig;
    m_tlsConfig = nullptr;

    delete m_sslSocket;
    m_sslSocket = nullptr;
}

// ============================================================================
// Backend Information
// ============================================================================

QString Director::backupSystemName() const
{
    return QString(BACKUP_SYSTEM_NAME);
}

// ============================================================================
// Connection Management
// ============================================================================

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

    // Abort existing connection
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
        qDebug() << "(TLS/PSK will be negotiated during authentication)";
    }

    // Connect via TCP - TLS comes during authentication
    m_sslSocket->connectToHost(host, port);
}

void Director::disconnect()
{
    // Cleanup authentication
    if (m_auth) {
        // Disconnect auth signals
        QObject::disconnect(m_connAuthSucceeded);
        QObject::disconnect(m_connAuthFailed);
        QObject::disconnect(m_connAuthStatus);

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

// ============================================================================
// TLS Configuration
// ============================================================================

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
// Signal Management
// ============================================================================

void Director::connectSocketSignals()
{
    // SSL/TLS signals
    m_connSocketEncrypted = QObject::connect(m_sslSocket, &QSslSocket::encrypted,
                                    this, &Director::onEncrypted);

    m_connSocketSslErrors = QObject::connect(m_sslSocket,
                                    QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
                                    this, &Director::onSslErrors);

    // Basic socket signals
    m_connSocketConnected = QObject::connect(m_sslSocket, &QSslSocket::connected,
                                    this, &Director::onConnected);

    m_connSocketDisconnected = QObject::connect(m_sslSocket, &QSslSocket::disconnected,
                                       this, &Director::onDisconnected);

    m_connSocketReadyRead = QObject::connect(m_sslSocket, &QSslSocket::readyRead,
                                    this, &Director::onReadyRead);

    m_connSocketError = QObject::connect(m_sslSocket, &QSslSocket::errorOccurred,
                                this, &Director::onError);

    // Debug signals
#ifdef IS_DEVELOPER
    QObject::connect(m_sslSocket, &QSslSocket::stateChanged, this,
            [this](QAbstractSocket::SocketState state) {
                qDebug() << "Socket state changed to:" << state;
            });

    QObject::connect(m_sslSocket, &QAbstractSocket::errorOccurred, this,
            [this](QAbstractSocket::SocketError socketError) {
                QString errorMsg;
                switch(socketError) {
                case QAbstractSocket::ConnectionRefusedError:
                    errorMsg = "Connection refused - is Director running?";
                    break;
                case QAbstractSocket::HostNotFoundError:
                    errorMsg = "Host not found - check hostname";
                    break;
                case QAbstractSocket::SocketTimeoutError:
                    errorMsg = "Connection timeout - server not responding";
                    break;
                case QAbstractSocket::NetworkError:
                    errorMsg = "Network error - check network connection";
                    break;
                default:
                    errorMsg = m_sslSocket->errorString();
                }
                qCritical() << "Socket error:" << errorMsg;
                emit connectionError(errorMsg);
            });

    QObject::connect(m_sslSocket, &QSslSocket::modeChanged, this,
            [this](QSslSocket::SslMode mode) {
                qDebug() << "========================================";
                qDebug() << "SSL MODE CHANGED";
                qDebug() << "  Mode:" << (mode == QSslSocket::UnencryptedMode ?
                                              "Unencrypted" : "SslClientMode/SslServerMode");
                qDebug() << "========================================";
            });

    QObject::connect(m_sslSocket, &QSslSocket::encryptedBytesWritten, this,
            [this](qint64 written) {
                qDebug() << "Encrypted bytes written:" << written;
            });

    QObject::connect(m_sslSocket, &QSslSocket::peerVerifyError, this,
            [this](const QSslError &error) {
                qDebug() << "⚠ Peer verify error:" << error.errorString();
            });
#endif
}

void Director::disconnectAllSignals()
{
    // Disconnect socket signals
    QObject::disconnect(m_connSocketConnected);
    QObject::disconnect(m_connSocketDisconnected);
    QObject::disconnect(m_connSocketReadyRead);
    QObject::disconnect(m_connSocketError);
    QObject::disconnect(m_connSocketSslErrors);
    QObject::disconnect(m_connSocketEncrypted);

    // Disconnect auth signals (if still connected)
    QObject::disconnect(m_connAuthSucceeded);
    QObject::disconnect(m_connAuthFailed);
    QObject::disconnect(m_connAuthStatus);
}

Director::ApiMode Director::apiMode() const
{
    return m_apiMode;
}

void Director::setApiMode(ApiMode mode)
{
    m_apiMode = mode;

    QString modeStr;
    switch (mode) {
    case ApiMode::Off:
        modeStr = "0";
        break;
    case ApiMode::Json:
        modeStr = "1";
        break;
    case ApiMode::JsonPretty:
        modeStr = "2";
        break;
    }

#ifdef IS_DEVELOPER
    qDebug() << "Setting API mode to:" << modeStr;
#endif

    sendCommand(DirectorCommand::ApiMode, modeStr);
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

    // IMPORTANT: Do NOT call startAuthentication() here!
    // For PSK-TLS, authentication continues in BareosAuth::onEncrypted()
    // For non-PSK, startAuthentication() is called from onConnected()
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
        emit connectionError("SSL error: " + errorSummary);
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
        // Disconnect auth signals
        QObject::disconnect(m_connAuthSucceeded);
        QObject::disconnect(m_connAuthFailed);
        QObject::disconnect(m_connAuthStatus);

        m_auth->deleteLater();
        m_auth = nullptr;
    }

    emit disconnected();
}

void Director::onBytesWritten(qint64 bytes){

    // During authentication, the Auth class handles all data
    // This should never happen, for that the connection to this
    // slot is beed done after authentifcatuion
    if (m_connectionState == Authenticating) {
        // Auth class reads directly from socket
        return;
    }
}

void Director::onReadyRead()
{

    // During authentication, the Auth class handles all data
    // This should never happen, for that the connection to this
    // slot is beed done after authentifcatuion
    if (m_connectionState == Authenticating) {
        // Auth class reads directly from socket
        return;
    }

    // After authentication: Process normal command responses
    m_receiveBuffer.append(m_sslSocket->readAll());

#ifdef IS_DEVELOPER
    qDebug() << "<<<< Getting data:" << m_receiveBuffer;
#endif
    // Process complete responses
    // TODO: Implement response parsing

    m_receiveBuffer.clear();
}

void Director::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    QString errorMsg = QString("Socket error: %1").arg(m_sslSocket->errorString());

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

    // Create appropriate auth class based on AUTH_CLASS macro
    // AUTH_CLASS is defined in director.h as BaculaAuth or BareosAuth
    m_auth = new AUTH_CLASS(m_sslSocket, this);

    // Connect authentication signals and store connections
    m_connAuthSucceeded = QObject::connect(m_auth, &AUTH_CLASS::authenticationSucceeded,
                                  this, &Director::onAuthenticationSucceeded);

    m_connAuthFailed = QObject::connect(m_auth, &AUTH_CLASS::authenticationFailed,
                               this, &Director::onAuthenticationFailed);

    m_connAuthStatus = QObject::connect(m_auth, &AUTH_CLASS::statusMessage,
                               this, &Director::onAuthStatusMessage);

    // Use PSK if password present
    bool usePSK = !m_password.isEmpty() && m_tlsConfig->tlsEnable;

    qDebug() << "PSK will be used:" << usePSK << "(password present:" << !m_password.isEmpty() << ")";

    // Start authentication
    bool authenticated = m_auth->authenticateDirector(
        m_directorName,
        PROJECT_NAME,
        m_password,
        m_tlsConfig->tlsEnable,
        m_tlsConfig->tlsRequire,
        m_tlsConfig->tlsVerifyPeer,
        usePSK
        );

    if (!authenticated) {
        qCritical() << "Failed to start authentication:" << m_auth->getErrorMessage();
        onAuthenticationFailed(m_auth->getErrorMessage());
    }
}

void Director::onAuthenticationSucceeded(const QString directorVersion)
{
#ifdef IS_DEVELOPER
    qDebug() << "========================================";
    qDebug() << "✓ AUTHENTICATION SUCCESSFUL";
    qDebug() << "  Encryption:" << m_sslSocket->sessionCipher().name();
    qDebug() << "  Director Version:" << directorVersion;
    qDebug() << "========================================";
#endif

    // ✅ Disconnect auth signals - authentication complete
    QObject::disconnect(m_connAuthSucceeded);
    QObject::disconnect(m_connAuthFailed);
    QObject::disconnect(m_connAuthStatus);

    m_directorVersion = directorVersion;
    m_connectionState = Ready;
    m_connected = true;

    // Cleanup auth object
    m_auth->deleteLater();
    m_auth = nullptr;

    // Parse version
    QRegularExpression versionRx(
        R"((\d+)\.(\d+)\.(\d+))",
        QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = versionRx.match(directorVersion);

    if (match.hasMatch())
    {
        int major = match.captured(1).toInt();
        int minor = match.captured(2).toInt();
        int patch = match.captured(3).toInt();

#ifdef IS_DEVELOPER
        qDebug() << "  Parsed version:" << major << "." << minor << "." << patch;

        // Enable compression if Director supports it
        if (major >= 1) {
            qDebug() << "  Director supports compression";
        }
#endif
    }

    // Save connection settings
    saveConnectionSettings();

    // Optional: Activate API mode (like BAT)
    if (m_apiMode) {
#ifdef IS_DEVELOPER
        qDebug() << "Activating API mode (.api 1)...";
#endif
        // sendCommand(".api 1");
    }

    // Initializing asyncrounous processing
    m_connReadyRead = QObject::connect(m_sslSocket, &QSslSocket::readyRead, this, &Director::onReadyRead);
    m_connBytesWritten = QObject::connect(m_sslSocket, &QSslSocket::bytesWritten, this, &Director::onBytesWritten);

    // ✅ Emit signals NACH dem API-Modus
    emit authenticationSucceeded(directorVersion);
    emit connected(directorVersion);
    m_connectionState = Ready;

#ifdef IS_DEVELOPER
    qDebug() << "✓ Connection established and ready";
#endif
}

void Director::onAuthenticationFailed(const QString &reason)
{
    qCritical() << "========================================";
    qCritical() << "✗ AUTHENTICATION FAILED";
    qCritical() << "  Reason:" << reason;
    qCritical() << "========================================";

    m_connectionState = Disconnected;
    m_connected = false;

    // ✅ Disconnect auth signals
    QObject::disconnect(m_connAuthSucceeded);
    QObject::disconnect(m_connAuthFailed);
    QObject::disconnect(m_connAuthStatus);

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

void Director::sendCommand(DirectorCommand cmd, quint64){}

void Director::sendCommand(DirectorCommand cmd, const QString &command){}

void Director::sendCommand(const QString &command)
{
    if (m_connectionState != Ready && m_sslSocket->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "Cannot send command - not ready (state:" << m_connectionState << ")";
        emit statusMessage("Nicht bereit");
        return;
    }

#ifdef IS_DEVELOPER
    qDebug() << ">>> Sending command:" << command;
#endif

    // ✅ Befehle werden als einfacher Text mit Newline gesendet
    // KEIN 4-Byte-Header nach der Authentifizierung!
    QString msg = command;

    // ✅ Füge Newline hinzu, falls nicht vorhanden
    if (!msg.endsWith('\n')) {
        msg += '\n';
    }

    QByteArray data = msg.toUtf8();

    qint64 written = m_sslSocket->write(data);
    m_sslSocket->flush();

    if (written != data.size()) {
        qCritical() << "Failed to send complete command! Written:" << written << "Expected:" << data.size();
        emit statusMessage("Fehler beim Senden");
    } else {
#ifdef IS_DEVELOPER
        qDebug() << "✓ Command sent successfully (" << written << "bytes)";
#endif
        emit statusMessage("Befehl gesendet: " + command);
    }

    m_lastCommand = command;
}

void Director::sendToDirector(const QByteArray &data)
{
    m_sslSocket->write(data);
    m_sslSocket->flush();
}

// ============================================================================
// Command Helpers
// ============================================================================

QString Director::directorCommandToString(DirectorCommand cmd, const QString &args)
{
    QString command;

    switch (cmd) {
    case DirectorCommand::ApiMode:
        // ✅ Intelligente API-Modus-Verarbeitung
        if (args.isEmpty()) {
            command = ".api 1";  // Default: JSON
        } else {
            bool ok;
            int mode = args.toInt(&ok);
            if (ok) {
                // Numerischer Modus
                command = QString(".api %1").arg(mode);
            } else {
                // String-Modus (on, off, json, json_pretty)
                command = QString(".api %1").arg(args);
            }
        }
        break;

    // Connection & Session
    case DirectorCommand::Quit:             command = "quit"; break;
    case DirectorCommand::Exit:             command = "exit"; break;

    // Status Commands
    case DirectorCommand::StatusDirector:   command = "status director"; break;
    case DirectorCommand::StatusClient:     command = QString("status client=%1").arg(args); break;
    case DirectorCommand::StatusStorage:    command = QString("status storage=%1").arg(args); break;
    case DirectorCommand::StatusScheduler:  command = "status scheduler"; break;
    case DirectorCommand::StatusRunning:    command = "status running"; break;
    case DirectorCommand::StatusSubscriptions: command = "status subscriptions"; break;

    // List Commands
    case DirectorCommand::ListJobs:         command = "list jobs"; break;
    case DirectorCommand::ListJobsLast:     command = QString("list jobs last=%1").arg(args.isEmpty() ? "100" : args); break;
    case DirectorCommand::ListJobId:        command = QString("list jobid=%1").arg(args); break;
    case DirectorCommand::ListClients:      command = "list clients"; break;
    case DirectorCommand::ListPools:        command = "list pools"; break;
    case DirectorCommand::ListVolumes:      command = "list volumes"; break;
    case DirectorCommand::ListVolumePool:   command = QString("list volumes pool=%1").arg(args); break;
    case DirectorCommand::ListMedia:        command = "list media"; break;
    case DirectorCommand::ListFileSets:     command = "list filesets"; break;
    case DirectorCommand::ListFiles:        command = QString("list files jobid=%1").arg(args); break;
    case DirectorCommand::ListNextVolume:   command = QString("list nextvol job=%1").arg(args); break;
    case DirectorCommand::ListBackups:      command = "list backups"; break;
    case DirectorCommand::ListBackupsClient: command = QString("list backups client=%1").arg(args); break;

    // Job Control
    case DirectorCommand::Run:              command = QString("run job=%1").arg(args); break;
    case DirectorCommand::RunYes:           command = QString("run job=%1 yes").arg(args); break;
    case DirectorCommand::Cancel:           command = QString("cancel jobid=%1").arg(args); break;
    case DirectorCommand::Delete:           command = QString("delete job jobid=%1").arg(args); break;
    case DirectorCommand::Disable:          command = QString("disable job=%1").arg(args); break;
    case DirectorCommand::Enable:           command = QString("enable job=%1").arg(args); break;
    case DirectorCommand::Rerun:            command = QString("rerun jobid=%1").arg(args); break;

    // Restore
    case DirectorCommand::Restore:          command = "restore"; break;
    case DirectorCommand::RestoreAll:       command = "restore all"; break;
    case DirectorCommand::RestoreSelect:    command = "restore select"; break;

    // Volume Management
    case DirectorCommand::Label:            command = QString("label %1").arg(args); break;
    case DirectorCommand::Relabel:          command = QString("relabel %1").arg(args); break;
    case DirectorCommand::Mount:            command = QString("mount storage=%1").arg(args); break;
    case DirectorCommand::Unmount:          command = QString("unmount storage=%1").arg(args); break;
    case DirectorCommand::Release:          command = QString("release storage=%1").arg(args); break;
    case DirectorCommand::Update:           command = "update"; break;
    case DirectorCommand::UpdateVolume:     command = QString("update volume=%1").arg(args); break;
    case DirectorCommand::Purge:            command = QString("purge volume=%1").arg(args); break;
    case DirectorCommand::Prune:            command = "prune"; break;
    case DirectorCommand::PruneFiles:       command = "prune files"; break;
    case DirectorCommand::PruneJobs:        command = "prune jobs"; break;
    case DirectorCommand::PruneVolume:      command = QString("prune volume=%1").arg(args); break;

    // Console Commands
    case DirectorCommand::Show:             command = QString("show %1").arg(args); break;
    case DirectorCommand::ShowJobs:         command = "show jobs"; break;
    case DirectorCommand::ShowClients:      command = "show clients"; break;
    case DirectorCommand::ShowFilesets:     command = "show filesets"; break;
    case DirectorCommand::ShowSchedules:    command = "show schedules"; break;
    case DirectorCommand::ShowPools:        command = "show pools"; break;
    case DirectorCommand::ShowStorages:     command = "show storages"; break;
    case DirectorCommand::ShowCatalogs:     command = "show catalogs"; break;
    case DirectorCommand::ShowMessages:     command = "show messages"; break;
    case DirectorCommand::ShowAll:          command = "show all"; break;

    // Messages
    case DirectorCommand::Messages:         command = "messages"; break;

    // Catalog
    case DirectorCommand::SqlQuery:         command = QString("sqlquery %1").arg(args); break;
    case DirectorCommand::Query:            command = QString("query %1").arg(args); break;

    // Testing & Debugging
    case DirectorCommand::Estimate:         command = QString("estimate %1").arg(args); break;
    case DirectorCommand::Time:             command = "time"; break;
    case DirectorCommand::Trace:            command = QString("trace %1").arg(args.isEmpty() ? "on" : args); break;
    case DirectorCommand::Version:          command = "version"; break;
    case DirectorCommand::Memory:           command = "memory"; break;

    // Configuration
    case DirectorCommand::Reload:           command = "reload"; break;
    case DirectorCommand::Configure:        command = QString("configure %1").arg(args); break;
    case DirectorCommand::Export:           command = QString("export %1").arg(args); break;
    case DirectorCommand::Import:           command = QString("import %1").arg(args); break;

    // Help
    case DirectorCommand::Help:             command = "help"; break;

    // Custom
    case DirectorCommand::Custom:           command = args; break;

    default:
        qWarning() << "Unknown DirectorCommand:" << static_cast<int>(cmd);
        command = args;
        break;
    }

    return command;
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
