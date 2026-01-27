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

#include <bdirector.h>
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

BDirector::BDirector(QObject *parent)
    : QObject(parent)
    , m_connectionState(Disconnected)
    , m_socket(nullptr)
    , m_port(9101)
    , m_directorVersion("0.0.0")
    , m_auth(nullptr)
    , m_connected(false)
    , m_apiMode(ApiMode::Json)  // API mode enabled by default
    , m_lastSentSize(0)
{
    m_tlsConfig = new TLSConfig();
    m_socket = new QSslSocket(this);
    // Connect socket signals and store connections
    connectSocketSignals();
}

BDirector::~BDirector()
{
    disconnect();

    // Disconnect all signals
    disconnectAllSignals();

    // Clean up
    delete m_tlsConfig;
    m_tlsConfig = nullptr;

    delete m_socket;
    m_socket = nullptr;
}

// ============================================================================
// Backend Information
// ============================================================================

QString BDirector::backupSystemName() const
{
    return QString(BACKUP_SYSTEM_NAME);
}

// ============================================================================
// Connection Management
// ============================================================================

void BDirector::connect(const QString &host, int port, const QString &directorName,
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
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->abort();
        m_socket->waitForDisconnected(1000);
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
    m_socket->connectToHost(host, port);
}

void BDirector::disconnect()
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
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        sendCommand("quit");
        m_socket->disconnectFromHost();
    }

    m_connected = false;
    m_connectionState = Disconnected;
}

bool BDirector::isConnected() const
{
    return m_connected && m_connectionState == Ready;
}

BDirector::ConnectionState BDirector::connectionState() const
{
    return m_connectionState;
}

// ============================================================================
// TLS Configuration
// ============================================================================

void BDirector::setTLSConfig(const TLSConfig &config)
{
    if (!m_tlsConfig) {
        m_tlsConfig = new TLSConfig();
    }
    *m_tlsConfig = config;
}

BDirector::TLSConfig *BDirector::tlsConfig() const
{
    if (m_tlsConfig) {
        return m_tlsConfig;
    }
    return new TLSConfig();
}

// ============================================================================
// Signal Management
// ============================================================================

void BDirector::connectSocketSignals()
{
    // SSL/TLS signals
    m_connSocketEncrypted = QObject::connect(m_socket, &QSslSocket::encrypted,
                                    this, &BDirector::onEncrypted);

    m_connSocketSslErrors = QObject::connect(m_socket,
                                    QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
                                    this, &BDirector::onSslErrors);

    // Basic socket signals
    m_connSocketConnected = QObject::connect(m_socket, &QSslSocket::connected,
                                    this, &BDirector::onConnected);

    m_connSocketDisconnected = QObject::connect(m_socket, &QSslSocket::disconnected,
                                       this, &BDirector::onDisconnected);

    m_connSocketReadyRead = QObject::connect(m_socket, &QSslSocket::readyRead,
                                    this, &BDirector::onReadyRead);

    m_connSocketError = QObject::connect(m_socket, &QSslSocket::errorOccurred,
                                this, &BDirector::onError);

    // Debug signals
#ifdef IS_DEVELOPER
    QObject::connect(m_socket, &QSslSocket::stateChanged, this,
            [this](QAbstractSocket::SocketState state) {
                qDebug() << "Socket state changed to:" << state;
            });

    QObject::connect(m_socket, &QAbstractSocket::errorOccurred, this,
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
                    errorMsg = m_socket->errorString();
                }
                qCritical() << "Socket error:" << errorMsg;
                emit protocolError(errorMsg);
            });

    QObject::connect(m_socket, &QSslSocket::modeChanged, this,
            [this](QSslSocket::SslMode mode) {
                qDebug() << "========================================";
                qDebug() << "SSL MODE CHANGED";
                qDebug() << "  Mode:" << (mode == QSslSocket::UnencryptedMode ?
                                              "Unencrypted" : "SslClientMode/SslServerMode");
                qDebug() << "========================================";
            });

    QObject::connect(m_socket, &QSslSocket::encryptedBytesWritten, this,
            [this](qint64 written) {
                qDebug() << "Encrypted bytes written:" << written;
            });

    QObject::connect(m_socket, &QSslSocket::peerVerifyError, this,
            [this](const QSslError &error) {
                qDebug() << "⚠ Peer verify error:" << error.errorString();
            });
#endif
}

void BDirector::disconnectAllSignals()
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

BDirector::ApiMode BDirector::apiMode() const
{
    return m_apiMode;
}

void BDirector::setApiMode(ApiMode mode)
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

    doSendCommand(Command::ApiMode, modeStr);
}
// ============================================================================
// Socket Event Handlers
// ============================================================================

void BDirector::onConnected()
{
    qDebug() << "========================================";
    qDebug() << "TCP CONNECTED";
    qDebug() << "========================================";

    startAuthentication();
}

void BDirector::onEncrypted()
{
    qDebug() << "========================================";
    qDebug() << "✓ PSK-TLS HANDSHAKE SUCCESSFUL";
    qDebug() << "========================================";
    qDebug() << "  Encrypted: true";
    qDebug() << "  Protocol:" << m_socket->sessionProtocol();
    qDebug() << "  Cipher:" << m_socket->sessionCipher().name();

    if (!m_socket->peerCertificate().isNull()) {
        qDebug() << "  Peer Certificate:";
        qDebug() << "    Subject:" << m_socket->peerCertificate().subjectInfo(QSslCertificate::CommonName);
    }
    qDebug() << "========================================";

    // IMPORTANT: Do NOT call startAuthentication() here!
    // For PSK-TLS, authentication continues in BareosAuth::onEncrypted()
    // For non-PSK, startAuthentication() is called from onConnected()
}

void BDirector::onSslErrors(const QList<QSslError> &errors)
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
        m_socket->ignoreSslErrors();
    } else if (!m_tlsConfig->tlsVerifyPeer) {
        qWarning() << "⚠ Peer verification disabled - ignoring SSL errors";
        m_socket->ignoreSslErrors();
    } else {
        qCritical() << "✗ SSL errors with peer verification enabled - connection will fail";
        m_connectionState = Disconnected;
        QString errorSummary = QString("%1 SSL error(s): %2")
                                   .arg(errors.size())
                                   .arg(errors.first().errorString());
        emit protocolError("SSL error: " + errorSummary);
    }
}

void BDirector::onDisconnected()
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

void BDirector::onBytesWritten(qint64 bytes){

    // During authentication, the Auth class handles all data
    // This should never happen, for that the connection to this
    // slot is beed done after authentifcatuion
    if (m_connectionState == Authenticating) {
        // Auth class reads directly from socket
        return;
    }
}

void BDirector::onReadyRead()
{
    if (m_connectionState == Authenticating) {
        qWarning() << "⚠ readyRead during authentication - should not happen!";
        return;
    }

    // Lese neue Daten
    QByteArray newData = m_socket->readAll();
    m_receiveBuffer.append(newData);

#ifdef IS_DEVELOPER
    qDebug() << "<<<< Received" << newData.size() << "bytes";
    qDebug() << "     Buffer total:" << m_receiveBuffer.size() << "bytes";
#endif

    // Verarbeite alle vollständigen Pakete im Buffer
    while (m_receiveBuffer.size() >= 4) {
        // Lese 4-Byte Message-Length Header (signed 32-bit big-endian)
        qint32 messageLength = 0;
        memcpy(&messageLength, m_receiveBuffer.constData(), 4);
        messageLength = qFromBigEndian(messageLength);

#ifdef IS_DEVELOPER
        qDebug() << "     Message length from header:" << messageLength;
#endif

        // Negative Länge = Signal-Nachricht (z.B. Prompt)
        bool isSignal = (messageLength < 0);
        qint32 absLength = qAbs(messageLength);

        // Prüfe ob vollständiges Paket vorhanden (4 Bytes Header + Payload)
        if (m_receiveBuffer.size() < 4 + absLength) {
#ifdef IS_DEVELOPER
            qDebug() << "     Waiting for more data..."
                     << "Have:" << m_receiveBuffer.size()
                     << "Need:" << (4 + absLength);
#endif
            break;  // Warte auf mehr Daten
        }

        // Extrahiere Nachricht (ohne Header)
        QByteArray message = m_receiveBuffer.mid(4, absLength);
        message.replace(0x01, ' ');

        // Entferne verarbeitetes Paket aus Buffer
        m_receiveBuffer.remove(0, 4 + absLength);

        // Konvertiere zu String
        QString messageStr = QString::fromUtf8(message).trimmed();

#ifdef IS_DEVELOPER
        qDebug() << "✓ Complete message received:";
        qDebug() << "  Signal:" << isSignal;
        qDebug() << "  Length:" << absLength;
        qDebug() << "  Content:" << messageStr;
#endif

        // Verarbeite Nachricht
        processDirectorMessage(unbashSpaces(messageStr), isSignal);
    }
}

void BDirector::processDirectorMessage(const QString &message, bool isSignal)
{
    // Ignoriere leere Nachrichten
    if (message.isEmpty()) {
        return;
    }

    // API-Response (JSON)?
    if (message.startsWith("{") || message.startsWith("[")) {
#ifdef IS_DEVELOPER
        qDebug() << "📄 JSON Response:";
        qDebug() << message;
#endif
        emit jsonResponse(m_lastCommand, message);
    } else {
        emit commandResponse(m_lastCommand, message);
        return;
    }

    // Status-Nachricht (1000, 1002, etc.)?
    QRegularExpression statusRe("^(\\d+)(\\n+)");
    QRegularExpressionMatch match = statusRe.match(message);
    if (match.hasMatch()) {
        int statusCode = match.captured(1).toInt();

        // 1000 = OK
        // 1002 = Info message
        // 2xxx = Error

        if (statusCode >= 2000) {
            qWarning() << "⚠ Director error:" << message;
            emit commandError(m_lastCommand, message);
            return;
        } else {
#ifdef IS_DEVELOPER
            qDebug() << "ℹ Director status:" << message;
#endif

            switch (statusCode) {
            case 1002:
                emit statusMessage(match.captured(3));
#ifdef IS_DEVELOPER
                qDebug() << "✓ Director ready (prompt received)";
#endif
                // Optional: Activate API mode (like BAT)
                if (m_apiMode != ApiMode::Off) {
                    m_apiMode = ApiMode::JsonPretty;
                    doSendCommand(Command::ApiMode, m_apiMode);
#ifdef IS_DEVELOPER
                    qDebug() << "Activating API mode " << m_apiMode;
#endif
                    //// emit directorReady();
                }
                break;
            default:
                emit statusMessage(message);
            }
        }
    }

    // Prompt "*" oder andere Text-Nachrichten
    if (isSignal || message == "*") {
        return;
    }

    // Normale Text-Antwort
#ifdef IS_DEVELOPER
    qDebug() << "📝 Text response:" << message;
#endif
    // emit commandResponse(m_lastCommand, message);
}

void BDirector::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    QString errorMsg = QString("Socket error: %1").arg(m_socket->errorString());

    qCritical() << errorMsg;
    m_connectionState = Disconnected;
    emit protocolError(errorMsg);
}

// ============================================================================
// TLS Setup
// ============================================================================

bool BDirector::setupTLSConnection()
{
    qDebug() << "========================================";
    qDebug() << "TLS CONFIGURATION";
    qDebug() << "========================================";

    QSslConfiguration sslConfig = m_socket->sslConfiguration();

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
    m_socket->setSslConfiguration(sslConfig);

    qDebug() << "✓ TLS configuration complete";
    qDebug() << "========================================";

    return true;
}

bool BDirector::loadTLSCertificates(QSslConfiguration &sslConfig)
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

void BDirector::startAuthentication()
{
    qDebug() << "========================================";
    qDebug() << "STARTING AUTHENTICATION";
    qDebug() << "========================================";

    m_connectionState = Authenticating;

    // Create appropriate auth class based on AUTH_CLASS macro
    // AUTH_CLASS is defined in director.h as BaculaAuth or BareosAuth
    m_auth = new AUTH_CLASS(m_socket, this);

    // Connect authentication signals and store connections
    m_connAuthSucceeded = QObject::connect(m_auth, &AUTH_CLASS::authenticationSucceeded,
                                  this, &BDirector::onAuthenticationSucceeded);

    m_connAuthFailed = QObject::connect(m_auth, &AUTH_CLASS::authenticationFailed,
                               this, &BDirector::onAuthenticationFailed);

    m_connAuthStatus = QObject::connect(m_auth, &AUTH_CLASS::statusMessage,
                               this, &BDirector::onAuthStatusMessage);

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

void BDirector::onAuthenticationSucceeded(const QString directorVersion)
{
#ifdef IS_DEVELOPER
    qDebug() << "========================================";
    qDebug() << "✓ AUTHENTICATION SUCCESSFUL";
    qDebug() << "  Encryption:" << (m_socket->sessionCipher().name().isEmpty() ? "None" : m_socket->sessionCipher().name());
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
        //@TODO !!!
        if (major >= 1) {
            qDebug() << "  Director supports compression";
        }
#endif
    }

    // Save connection settings
    saveConnectionSettings();

    // Initializing asyncrounous processing
    m_connReadyRead = QObject::connect(m_socket, &QSslSocket::readyRead, this, &BDirector::onReadyRead);
    m_connBytesWritten = QObject::connect(m_socket, &QSslSocket::bytesWritten, this, &BDirector::onBytesWritten);

    // ✅ Emit signals NACH dem API-Modus
    emit authentificationSucceeded(true, directorVersion);

    m_connectionState = Ready;

    // Verwende QTimer für Keep-Alive
    QTimer *keepAliveTimer = new QTimer(this);
    QObject::connect(keepAliveTimer, &QTimer::timeout, [this]() {
        if (m_socket->isOpen()) {
            sendCommand(".message\n");  // Keep connection alive
        }
#ifdef IS_DEVELOPER
        qDebug() << ".";
#endif
    });
    // keepAliveTimer->start(3000);

#ifdef IS_DEVELOPER
    qDebug() << "✓ Connection established and ready";
#endif
}

void BDirector::onAuthenticationFailed(const QString &reason)
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
    m_socket->disconnectFromHost();

    emit authentificationSucceeded(false, reason);
    emit protocolError(reason);
}

void BDirector::onAuthStatusMessage(const QString &message)
{
    qDebug() << "[Auth]" << message;
    emit statusMessage(message);
}

// ============================================================================
// Command Handling
// ============================================================================

void BDirector::sendCommand(const QString &command)
{
    if (m_connectionState != Ready && m_socket->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "Cannot send command - not ready (state:" << m_connectionState << ")";
        emit statusMessage("Nicht bereit");
        return;
    }

    QString msg = command;
    if (!msg.endsWith('\n')) {
        msg += '\n';
    }

#ifdef IS_DEVELOPER
    qDebug() << ">>> Sending command:" << command;
#endif

    // Prepare packet: 4-byte big-endian length + payload
    qint32 len = m_writeBuffer.size();
    m_lastSentSize = len + 4;

    // Big-Endian System (SPARC, PowerPC, MIPS BE, etc.)
    #if Q_BYTE_ORDER == Q_BIG_ENDIAN
        m_writeBuffer.prepend(reinterpret_cast<const char*>(&len), 4);
    // Little-Endian System (x86, x86-64, ARM LE, etc.)
    #else
        qint32 lenBE = qToBigEndian(len);
        m_writeBuffer.prepend(reinterpret_cast<const char*>(&lenBE), 4);
    #endif

    QByteArray data = msg.toUtf8();

    m_lastSentSize = m_socket->write(data);
    // m_socket->flush();

    if (m_lastSentSize!= data.size()) {
        qCritical() << "Failed to send complete command! Written:" << m_lastSentSize << "Expected:" << data.size();
        emit statusMessage("Fehler beim Senden");
    } else {
#ifdef IS_DEVELOPER
        qDebug() << "✓ Command sent successfully (" << m_lastSentSize << "bytes)";
#endif
        m_writeBuffer.clear();
        emit statusMessage(QString("Befehl '%1' gesendet").arg(command));
    }

    m_lastCommand = command;
}

//@deprecated
void BDirector::doSendCommand(Command cmd, quint64){ commandToString(cmd); }

void BDirector::doSendCommand(const Command cmd, const QString &args){ 
    const QString cmdStr = commandToString(cmd, args); 
    sendCommand(cmdStr);
}

// ============================================================================
// Command Helpers
// ============================================================================
const QString BDirector::commandToString(Command cmd, const QString &args)
{
    QString command;

    switch (cmd) {
    case Command::ApiMode:
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
    case Command::Quit:             command = "quit"; break;
    case Command::Exit:             command = "exit"; break;

    // Status Commands
    case Command::StatusDirector:   command = "status director"; break;
    case Command::StatusClient:     command = QString("status client=%1").arg(args); break;
    case Command::StatusStorage:    command = QString("status storage=%1").arg(args); break;
    case Command::StatusScheduler:  command = "status scheduler"; break;
    case Command::StatusRunning:    command = "status running"; break;
    case Command::StatusSubscriptions: command = "status subscriptions"; break;

    // List Commands
    case Command::ListJobs:         command = "list jobs"; break;
    case Command::ListJobsLast:     command = QString("list jobs last=%1").arg(args.isEmpty() ? "100" : args); break;
    case Command::ListJobId:        command = QString("list jobid=%1").arg(args); break;
    case Command::ListClients:      command = "list clients"; break;
    case Command::ListPools:        command = "list pools"; break;
    case Command::ListVolumes:      command = "list volumes"; break;
    case Command::ListVolumePool:   command = QString("list volumes pool=%1").arg(args); break;
    case Command::ListMedia:        command = "list media"; break;
    case Command::ListFileSets:     command = "list filesets"; break;
    case Command::ListFiles:        command = QString("list files jobid=%1").arg(args); break;
    case Command::ListNextVolume:   command = QString("list nextvol job=%1").arg(args); break;
    case Command::ListBackups:      command = "list backups"; break;
    case Command::ListBackupsClient: command = QString("list backups client=%1").arg(args); break;

    // Job Control
    case Command::Run:              command = QString("run job=%1").arg(args); break;
    case Command::RunYes:           command = QString("run job=%1 yes").arg(args); break;
    case Command::Cancel:           command = QString("cancel jobid=%1").arg(args); break;
    case Command::Delete:           command = QString("delete job jobid=%1").arg(args); break;
    case Command::Disable:          command = QString("disable job=%1").arg(args); break;
    case Command::Enable:           command = QString("enable job=%1").arg(args); break;
    case Command::Rerun:            command = QString("rerun jobid=%1").arg(args); break;

    // Restore
    case Command::Restore:          command = "restore"; break;
    case Command::RestoreAll:       command = "restore all"; break;
    case Command::RestoreSelect:    command = "restore select"; break;

    // Volume Management
    case Command::Label:            command = QString("label %1").arg(args); break;
    case Command::Relabel:          command = QString("relabel %1").arg(args); break;
    case Command::Mount:            command = QString("mount storage=%1").arg(args); break;
    case Command::Unmount:          command = QString("unmount storage=%1").arg(args); break;
    case Command::Release:          command = QString("release storage=%1").arg(args); break;
    case Command::Update:           command = "update"; break;
    case Command::UpdateVolume:     command = QString("update volume=%1").arg(args); break;
    case Command::Purge:            command = QString("purge volume=%1").arg(args); break;
    case Command::Prune:            command = "prune"; break;
    case Command::PruneFiles:       command = "prune files"; break;
    case Command::PruneJobs:        command = "prune jobs"; break;
    case Command::PruneVolume:      command = QString("prune volume=%1").arg(args); break;

    // Console Commands
    case Command::Show:             command = QString("show %1").arg(args); break;
    case Command::ShowJobs:         command = "show jobs"; break;
    case Command::ShowClients:      command = "show clients"; break;
    case Command::ShowFilesets:     command = "show filesets"; break;
    case Command::ShowSchedules:    command = "show schedules"; break;
    case Command::ShowPools:        command = "show pools"; break;
    case Command::ShowStorages:     command = "show storages"; break;
    case Command::ShowCatalogs:     command = "show catalogs"; break;
    case Command::ShowMessages:     command = "show messages"; break;
    case Command::ShowAll:          command = "show all"; break;

    // Messages
    case Command::Messages:         command = "messages"; break;

    // Catalog
    case Command::SqlQuery:         command = QString("sqlquery %1").arg(args); break;
    case Command::Query:            command = QString("query %1").arg(args); break;

    // Testing & Debugging
    case Command::Estimate:         command = QString("estimate %1").arg(args); break;
    case Command::Time:             command = "time"; break;
    case Command::Trace:            command = QString("trace %1").arg(args.isEmpty() ? "on" : args); break;
    case Command::Version:          command = "version"; break;
    case Command::Memory:           command = "memory"; break;

    // Configuration
    case Command::Reload:           command = "reload"; break;
    case Command::Configure:        command = QString("configure %1").arg(args); break;
    case Command::Export:           command = QString("export %1").arg(args); break;
    case Command::Import:           command = QString("import %1").arg(args); break;

    // Help
    case Command::Help:             command = "help"; break;

    // Custom
    case Command::Custom:           command = args; break;

    default:
        qWarning() << "Unknown Command:" << static_cast<int>(cmd);
        command = args;
        break;
    }

    return command;
}


// ============================================================================
// Settings Management
// ============================================================================

void BDirector::saveConnectionSettings()
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

void BDirector::loadConnectionSettings()
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

bool BDirector::hasStoredConnection() const
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

BDirector::JobStatus BDirector::parseJobStatus(const QString &status)
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

QString BDirector::jobStatusToString(JobStatus status)
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


