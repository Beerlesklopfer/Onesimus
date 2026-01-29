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
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 * @version 1.0.0
 */

#include <bareosdirector.h>
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

BareosDirector::BareosDirector(QObject *parent)
    : QObject(parent)
    , m_connectionState(Disconnected)
    , m_socket(nullptr)
    , m_port(9101)
    , m_directorVersion("0.0.0")
    , m_auth(nullptr)
    , m_connected(false)
    , m_apiMode(ApiMode::Json)  // API mode enabled by default
    , m_jsonTextAccumulation(false)
    , m_lastSentSize(0)
    , m_initialized(false)
    , m_authCompleted(false)
{
    m_tlsConfig = new TLSConfig();

    // Define required resources for full initialization
    // These dot-commands will be sent after API mode is confirmed
    m_requiredResources = {
        ResourceType::Job,       // .jobs
        ResourceType::Client,    // .clients
        ResourceType::Fileset,   // .filesets
        ResourceType::Storage,   // .storages
        ResourceType::Pool,      // .pools
        ResourceType::Level,     // .levels
        ResourceType::Schedule   // .schedule
    };

    // Socket and Auth will be created in initialize() (called from worker thread)
}

void BareosDirector::initialize()
{
    qDebug() << "========================================";
    qDebug() << "BAREOSDIRECTOR INITIALIZATION";
    qDebug() << "  Thread ID:" << QThread::currentThreadId();
    qDebug() << "  Object thread affinity:" << this->thread();
    qDebug() << "========================================";

    // Create socket in this thread (worker thread)
    // Socket will inherit thread affinity from this QObject
    m_socket = new QSslSocket(this);

    // Connect socket signals
    connectSocketSignals();

    m_initialized = true;
    qDebug() << "  ✓ Socket created with correct thread affinity";
    qDebug() << "  ✓ Socket thread:" << m_socket->thread();
    qDebug() << "  ✓ BareosDirector thread:" << this->thread();
    qDebug() << "========================================";
}

BareosDirector::~BareosDirector()
{
    qDebug() << "BareosDirector: Destructor called";

    // Cleanup socket and auth
    if (m_socket) {
        if (m_socket->state() == QAbstractSocket::ConnectedState) {
            m_socket->disconnectFromHost();
            m_socket->waitForDisconnected(1000);
        }
        m_socket->deleteLater();
        m_socket = nullptr;
    }

    if (m_auth) {
        m_auth->deleteLater();
        m_auth = nullptr;
    }

    // Clean up TLS config
    delete m_tlsConfig;
    m_tlsConfig = nullptr;

    qDebug() << "BareosDirector: Destructor complete";
}

// ============================================================================
// Backend Information
// ============================================================================

QString BareosDirector::backupSystemName() const
{
    return QString(BACKUP_SYSTEM_NAME);
}

// ============================================================================
// Connection Management
// ============================================================================

void BareosDirector::connect(const QString &host, int port, const QString &directorName,
                       const QString &consoleName, const QString &password)
{
    QMutexLocker locker(&m_connectionMutex);

    if (!m_initialized || !m_socket) {
        qCritical() << "Socket not initialized! Thread not running?";
        emit protocolError("Internal error: Socket not initialized");
        return;
    }

    qDebug() << "========================================";
    qDebug() << "CONNECTING TO" << backupSystemName() << "DIRECTOR" << directorName;
    qDebug() << "========================================";
    qDebug() << "Host:" << host;
    qDebug() << "Port:" << port;
    qDebug() << "Console:" << consoleName;
    qDebug() << "Password present:" << (!password.isEmpty());
    qDebug() << "Thread:" << QThread::currentThreadId();
    qDebug() << "========================================";

    // Abort existing connection
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->abort();
        m_socket->waitForDisconnected(1000);
    }

    {
        QMutexLocker stateLocker(&m_stateMutex);
        m_connectionState = Connecting;
        m_connected = false;
    }

    m_host = host;
    m_port = port;
    m_directorName = directorName;
    m_consoleName = consoleName;
    m_password = password;

    qDebug() << "Connecting via plain TCP...";
    if (m_tlsConfig->tlsEnable) {
        qDebug() << "(TLS/PSK will be negotiated during authentication)";
    }

    // Connect via TCP - TLS comes during authentication
    m_socket->connectToHost(host, port);
}

void BareosDirector::disconnect()
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

bool BareosDirector::isConnected() const
{
    QMutexLocker locker(&m_stateMutex);
    return m_connected && m_connectionState == Ready;
}

BareosDirector::ConnectionState BareosDirector::connectionState() const
{
    QMutexLocker locker(&m_stateMutex);
    return m_connectionState;
}

// ============================================================================
// TLS Configuration
// ============================================================================

void BareosDirector::setTLSConfig(const TLSConfig &config)
{
    if (!m_tlsConfig) {
        m_tlsConfig = new TLSConfig();
    }
    *m_tlsConfig = config;
}

BareosDirector::TLSConfig *BareosDirector::tlsConfig() const
{
    if (m_tlsConfig) {
        return m_tlsConfig;
    }
    return new TLSConfig();
}

// ============================================================================
// State Machine
// ============================================================================

void BareosDirector::setState(ConnectionState newState)
{
    ConnectionState oldState;
    {
        QMutexLocker locker(&m_stateMutex);
        if (m_connectionState == newState) {
            return; // No change
        }
        oldState = m_connectionState;
        m_connectionState = newState;

        // Update m_connected flag
        m_connected = (newState == Ready);
    }

    // Debug output
    static const QMap<ConnectionState, QString> stateNames = {
        {Disconnected, "Disconnected"},
        {Connecting, "Connecting"},
        {Authenticating, "Authenticating"},
        {SettingApiMode, "SettingApiMode"},
        {LoadingResources, "LoadingResources"},
        {Ready, "Ready"},
        {ConnectionError, "ConnectionError"}
    };

    qDebug() << "STATE MACHINE:" << stateNames.value(oldState, "?")
             << "->" << stateNames.value(newState, "?");

    emit connectionStateChanged(oldState, newState);
}

// Resource name helper for debug output
static const QMap<BareosDirector::ResourceType, QString> s_resourceNames = {
    {BareosDirector::ResourceType::Catalog, "Catalog"},
    {BareosDirector::ResourceType::Client, "Client"},
    {BareosDirector::ResourceType::Console, "Console"},
    {BareosDirector::ResourceType::Director, "Director"},
    {BareosDirector::ResourceType::Fileset, "Fileset"},
    {BareosDirector::ResourceType::Job, "Job"},
    {BareosDirector::ResourceType::JobDefs, "JobDefs"},
    {BareosDirector::ResourceType::Messages, "Messages"},
    {BareosDirector::ResourceType::Pool, "Pool"},
    {BareosDirector::ResourceType::Profile, "Profile"},
    {BareosDirector::ResourceType::Schedule, "Schedule"},
    {BareosDirector::ResourceType::Storage, "Storage"},
    {BareosDirector::ResourceType::Level, "Level"}
};

static const QMap<BareosDirector::ResourceLoadState, QString> s_loadStateNames = {
    {BareosDirector::ResourceLoadState::Initial, "Initial"},
    {BareosDirector::ResourceLoadState::Waiting, "Waiting"},
    {BareosDirector::ResourceLoadState::Loaded, "Loaded"},
    {BareosDirector::ResourceLoadState::Reloading, "Reloading"},
    {BareosDirector::ResourceLoadState::Failed, "Failed"}
};

void BareosDirector::markResourceLoaded(ResourceType resourceType)
{
    ResourceLoadState oldState = m_resourceStates.value(resourceType, ResourceLoadState::Initial);
    m_resourceStates[resourceType] = ResourceLoadState::Loaded;
    m_resourceErrors.remove(resourceType);

    // Count loaded resources
    int loaded = 0;
    for (const ResourceType &type : m_requiredResources) {
        if (m_resourceStates.value(type) == ResourceLoadState::Loaded) {
            loaded++;
        }
    }
    int total = m_requiredResources.size();

    qDebug() << "RESOURCE:" << s_resourceNames.value(resourceType, "?")
             << s_loadStateNames.value(oldState) << "->" << "Loaded"
             << "(" << loaded << "/" << total << ")";

    emit resourceStateChanged(resourceType, ResourceLoadState::Loaded);
    emit resourceLoaded(resourceType);
    emit resourceLoadProgress(loaded, total);

    // Check if all resources are ready
    if (allResourcesReady()) {
        qDebug() << "STATE MACHINE: All resources loaded - transitioning to Ready";
        setState(Ready);
        emit allResourcesLoaded();
    }
}

bool BareosDirector::allResourcesReady() const
{
    // Check if all required resources have state Loaded
    for (const ResourceType &type : m_requiredResources) {
        if (m_resourceStates.value(type) != ResourceLoadState::Loaded) {
            return false;
        }
    }
    return true;
}

BareosDirector::ResourceLoadState BareosDirector::resourceState(ResourceType type) const
{
    return m_resourceStates.value(type, ResourceLoadState::Initial);
}

bool BareosDirector::isResourceLoaded(ResourceType type) const
{
    return m_resourceStates.value(type) == ResourceLoadState::Loaded;
}

bool BareosDirector::areAllResourcesLoaded() const
{
    return allResourcesReady();
}

void BareosDirector::reloadResource(ResourceType type)
{
    ResourceLoadState oldState = m_resourceStates.value(type, ResourceLoadState::Initial);
    m_resourceStates[type] = ResourceLoadState::Reloading;

    qDebug() << "RESOURCE:" << s_resourceNames.value(type, "?")
             << s_loadStateNames.value(oldState) << "->" << "Reloading";

    emit resourceStateChanged(type, ResourceLoadState::Reloading);

    // Send the appropriate dot-command
    switch (type) {
    case ResourceType::Job:
        doSendCommand(Command::DotJobs);
        break;
    case ResourceType::Client:
        doSendCommand(Command::DotClients);
        break;
    case ResourceType::Fileset:
        doSendCommand(Command::DotFilesets);
        break;
    case ResourceType::Storage:
        doSendCommand(Command::DotStorages);
        break;
    case ResourceType::Pool:
        doSendCommand(Command::DotPools);
        break;
    case ResourceType::Level:
        doSendCommand(Command::DotLevels);
        break;
    case ResourceType::Schedule:
        doSendCommand(Command::DotSchedule);
        break;
    case ResourceType::Catalog:
        doSendCommand(Command::DotCatalogs);
        break;
    default:
        qWarning() << "RESOURCE: No dot-command for" << s_resourceNames.value(type, "?");
        break;
    }
}

void BareosDirector::markResourceFailed(ResourceType resourceType, const QString &errorMessage)
{
    ResourceLoadState oldState = m_resourceStates.value(resourceType, ResourceLoadState::Initial);
    m_resourceStates[resourceType] = ResourceLoadState::Failed;
    m_resourceErrors[resourceType] = errorMessage;

    qDebug() << "RESOURCE FAILED:" << s_resourceNames.value(resourceType, "?")
             << s_loadStateNames.value(oldState) << "->" << "Failed"
             << "Error:" << errorMessage;

    emit resourceStateChanged(resourceType, ResourceLoadState::Failed);
    emit resourceLoadFailed(resourceType, errorMessage);
}

void BareosDirector::detectAndMarkResourceLoaded(const QString &jsonData)
{
    // Only process during LoadingResources or Ready state
    if (m_connectionState != LoadingResources && m_connectionState != Ready) {
        return;
    }

    // Parse JSON to detect resource type
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    // Map JSON keys to resource types
    if (result.contains("levels")) {
        markResourceLoaded(ResourceType::Level);
    }
    if (result.contains("filesets")) {
        markResourceLoaded(ResourceType::Fileset);
    }
    if (result.contains("storages")) {
        markResourceLoaded(ResourceType::Storage);
    }
    if (result.contains("pools")) {
        markResourceLoaded(ResourceType::Pool);
    }
    if (result.contains("schedules")) {
        markResourceLoaded(ResourceType::Schedule);
    }
    if (result.contains("catalogs")) {
        markResourceLoaded(ResourceType::Catalog);
    }

    // Check for .jobs response (has "enabled" or "fileset" field, not "jobstatus")
    if (result.contains("jobs")) {
        QJsonArray jobsArray = result["jobs"].toArray();
        if (!jobsArray.isEmpty()) {
            QJsonObject firstJob = jobsArray[0].toObject();
            if (firstJob.contains("enabled") || firstJob.contains("fileset")) {
                markResourceLoaded(ResourceType::Job);
            }
        }
    }

    // Check for .clients response (simple "name" field only, no "address" or "uname")
    if (result.contains("clients")) {
        QJsonArray clientsArray = result["clients"].toArray();
        if (!clientsArray.isEmpty()) {
            QJsonObject firstClient = clientsArray[0].toObject();
            // .clients has only "name", list clients has "address" and "uname"
            if (!firstClient.contains("address") && !firstClient.contains("uname")) {
                markResourceLoaded(ResourceType::Client);
            }
        }
    }
}

void BareosDirector::startResourceLoading()
{
    qDebug() << "STATE MACHINE: Starting resource loading...";
    qDebug() << "  Required resources:" << m_requiredResources.size();

    setState(LoadingResources);

    // Set all required resources to Waiting state and send dot-commands
    for (const ResourceType &type : m_requiredResources) {
        m_resourceStates[type] = ResourceLoadState::Waiting;
        emit resourceStateChanged(type, ResourceLoadState::Waiting);

        switch (type) {
        case ResourceType::Job:
            doSendCommand(Command::DotJobs);
            break;
        case ResourceType::Client:
            doSendCommand(Command::DotClients);
            break;
        case ResourceType::Fileset:
            doSendCommand(Command::DotFilesets);
            break;
        case ResourceType::Storage:
            doSendCommand(Command::DotStorages);
            break;
        case ResourceType::Pool:
            doSendCommand(Command::DotPools);
            break;
        case ResourceType::Level:
            doSendCommand(Command::DotLevels);
            break;
        case ResourceType::Schedule:
            doSendCommand(Command::DotSchedule);
            break;
        case ResourceType::Catalog:
            doSendCommand(Command::DotCatalogs);
            break;
        default:
            // Other resource types don't have corresponding dot-commands
            // Mark them as loaded immediately
            m_resourceStates[type] = ResourceLoadState::Loaded;
            break;
        }
    }
}

void BareosDirector::resetResourceFlags()
{
    m_resourceStates.clear();
    m_resourceErrors.clear();
    m_errorMessage.clear();

    // Initialize all required resources to Initial state
    for (const ResourceType &type : m_requiredResources) {
        m_resourceStates[type] = ResourceLoadState::Initial;
    }
}

// ============================================================================
// Signal Management
// ============================================================================

void BareosDirector::connectSocketSignals()
{
    // SSL/TLS signals - use DirectConnection to execute in worker thread
    m_connSocketEncrypted = QObject::connect(m_socket, &QSslSocket::encrypted,
                                    this, &BareosDirector::onEncrypted,
                                    Qt::DirectConnection);

    m_connSocketSslErrors = QObject::connect(m_socket,
                                    QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
                                    this, &BareosDirector::onSslErrors,
                                    Qt::DirectConnection);

    // Basic socket signals - use DirectConnection to execute in worker thread
    m_connSocketConnected = QObject::connect(m_socket, &QSslSocket::connected,
                                    this, &BareosDirector::onConnected,
                                    Qt::DirectConnection);

    m_connSocketDisconnected = QObject::connect(m_socket, &QSslSocket::disconnected,
                                       this, &BareosDirector::onDisconnected,
                                       Qt::DirectConnection);

    m_connSocketReadyRead = QObject::connect(m_socket, &QSslSocket::readyRead,
                                    this, &BareosDirector::onReadyRead,
                                    Qt::DirectConnection);

    m_connSocketError = QObject::connect(m_socket, &QSslSocket::errorOccurred,
                                this, &BareosDirector::onError,
                                Qt::DirectConnection);

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

void BareosDirector::disconnectAllSignals()
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

BareosDirector::ApiMode BareosDirector::apiMode() const
{
    return m_apiMode;
}

void BareosDirector::setApiMode(ApiMode mode)
{
    m_apiMode = mode;

    // ✅ Reset JSON accumulation when leaving API mode
    if (mode == ApiMode::Off) {
        m_jsonTextAccumulation = false;
        m_receiveBuffer.clear();  // Clear any pending JSON fragments
    }

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

    // Thread-safe: Use QMetaObject::invokeMethod to call doSendCommand in the correct thread
    QMetaObject::invokeMethod(this, "doSendCommand",
                              Qt::QueuedConnection,
                              Q_ARG(BareosDirector::Command, Command::ApiMode),
                              Q_ARG(QString, modeStr));
}
// ============================================================================
// Socket Event Handlers
// ============================================================================

void BareosDirector::onConnected()
{
    qDebug() << "========================================";
    qDebug() << "TCP CONNECTED";
    qDebug() << "========================================";

    startAuthentication();
}

void BareosDirector::onEncrypted()
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

void BareosDirector::onSslErrors(const QList<QSslError> &errors)
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

void BareosDirector::onDisconnected()
{
    qDebug() << "========================================";
    qDebug() << "CONNECTION CLOSED";
    qDebug() << "========================================";

    {
        QMutexLocker stateLocker(&m_stateMutex);
        m_connected = false;
        m_connectionState = Disconnected;
    }

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

void BareosDirector::onBytesWritten(qint64 bytes){

    // During authentication, the Auth class handles all data
    // This should never happen, for that the connection to this
    // slot is beed done after authentifcatuion
    if (m_connectionState == Authenticating) {
        // Auth class reads directly from socket
        return;
    }
}

void BareosDirector::onReadyRead()
{
    if (m_connectionState == Authenticating) {
        qWarning() << "⚠ readyRead during authentication - should not happen!";
        return;
    }

    // Lese neue Daten
    QByteArray newData = m_socket->readAll();
    m_receiveBuffer.append(newData);

#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
    qDebug() << "<<<< Received" << newData.size() << "bytes";
    qDebug() << "     Buffer total:" << m_receiveBuffer.size() << "bytes";
#endif

    // ✅ Handle JSON text mode (after .api 2 is activated)
    // When in API mode, responses are JSON text without binary headers
    // JSON can be fragmented across multiple packets, so we accumulate and parse
    if (m_apiMode != ApiMode::Off &&
        m_receiveBuffer.size() > 0 &&
        (m_receiveBuffer[0] == '{' || m_receiveBuffer[0] == '[' || m_jsonTextAccumulation)) {

        // Enter JSON text accumulation mode
        if (!m_jsonTextAccumulation) {
#ifdef IS_DEVELOPER
            qDebug() << "📄 Entering JSON text accumulation mode (API mode enabled)";
#endif
            m_jsonTextAccumulation = true;
        }

        // Try to parse buffer as complete JSON
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(m_receiveBuffer, &parseError);

        if (parseError.error == QJsonParseError::NoError) {
            // Successfully parsed JSON - we have at least one complete object
#ifdef IS_DEVELOPER
            qDebug() << "✓ Complete JSON parsed, size:" << m_receiveBuffer.size() << "bytes";
#endif
            // Process the JSON
            QString jsonStr = QString::fromUtf8(m_receiveBuffer).trimmed();
            processDirectorMessage(jsonStr, false);

            // Clear buffer
            m_receiveBuffer.clear();

            // ✅ STAY in JSON accumulation mode in API mode
            // Multiple JSON documents may follow without binary headers
            // Only exit JSON mode when API mode is disabled
            // m_jsonTextAccumulation remains TRUE in API mode
            return;
        } else {
            // Parse error - check if it's incomplete JSON (need more data)
            if (parseError.error == QJsonParseError::UnterminatedObject ||
                parseError.error == QJsonParseError::UnterminatedArray ||
                parseError.error == QJsonParseError::UnterminatedString ||
                m_receiveBuffer.size() < 100) {  // Very small buffer, likely incomplete
#if defined(IS_DEVELOPER) && defined(DEBUG_JSON)
                qDebug() << "⏳ Incomplete JSON, waiting for more data. Error:" << parseError.errorString()
                         << "at offset" << parseError.offset;
#endif
                // Wait for more data
                if (m_receiveBuffer.size() > 10000000) {  // 10MB limit
                    qCritical() << "⚠ JSON buffer too large without valid JSON:" << m_receiveBuffer.size();
                    qCritical() << "   Parse error:" << parseError.errorString();
                    qCritical() << "   Buffer start:" << QString::fromUtf8(m_receiveBuffer.left(200));
                    m_receiveBuffer.clear();
                    m_jsonTextAccumulation = false;
                }
                return;  // Wait for more data
            } else {
                // Real parse error - log and clear
                qCritical() << "⚠ JSON parse error:" << parseError.errorString();
                qCritical() << "   Error offset:" << parseError.offset;
                qCritical() << "   Buffer content:" << QString::fromUtf8(m_receiveBuffer.left(500));
                m_receiveBuffer.clear();
                m_jsonTextAccumulation = false;  // Exit JSON mode on real error
                return;
            }
        }
    }

    // ✅ If we reach here and JSON accumulation is active but we're not in API mode anymore,
    // reset the flag (shouldn't normally happen)
    if (m_jsonTextAccumulation && m_apiMode == ApiMode::Off) {
        qWarning() << "⚠ JSON accumulation active but API mode is off - resetting";
        m_jsonTextAccumulation = false;
    }

    // Verarbeite alle vollständigen Pakete im Buffer
    while (m_receiveBuffer.size() >= 4) {
        // Lese 4-Byte Message-Length Header (signed 32-bit big-endian)
        qint32 messageLength = 0;
        memcpy(&messageLength, m_receiveBuffer.constData(), 4);
        messageLength = qFromBigEndian(messageLength);

#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
        qDebug() << "     Message length from header:" << messageLength;
        qDebug() << "     First 20 bytes (hex):" << m_receiveBuffer.left(20).toHex(' ');
#endif

#ifdef IS_DEVELOPER
        // Warn about suspicious message lengths
        // (This should not happen now that JSON is handled properly above)
        if (messageLength > 100000 || messageLength < -100) {
            qCritical() << "⚠ SUSPICIOUS MESSAGE LENGTH!" << messageLength;
            qCritical() << "   API Mode:" << (m_apiMode != ApiMode::Off ? "ENABLED" : "DISABLED");
            qCritical() << "   JSON accumulation:" << m_jsonTextAccumulation;
            qCritical() << "   Buffer content (first 100 bytes hex):" << m_receiveBuffer.left(100).toHex(' ');
            qCritical() << "   Buffer content (as string):" << QString::fromLatin1(m_receiveBuffer.left(100));
            // Clear buffer and break to avoid infinite loop
            m_receiveBuffer.clear();
            m_jsonTextAccumulation = false;
            break;
        }
#endif

        // Negative Länge = Signal-Nachricht (z.B. Prompt)
        bool isSignal = (messageLength < 0);
        qint32 absLength = qAbs(messageLength);

        // Prüfe ob vollständiges Paket vorhanden (4 Bytes Header + Payload)
        if (m_receiveBuffer.size() < 4 + absLength) {
#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
            qDebug() << "     Waiting for more data..."
                     << "Have:" << m_receiveBuffer.size()
                     << "Need:" << (4 + absLength);
#endif
            break;  // Warte auf mehr Daten
        }

        // Extrahiere Nachricht (ohne Header)
        QByteArray message = m_receiveBuffer.mid(4, absLength);

        // ✅ KRITISCH: Signal-Pakete können verschachtelte Header enthalten!
        // Bareos sendet manchmal Signale, deren Payload weitere Paket-Header enthält.
        // In diesem Fall überspringen wir nur den Signal-Header (4 Bytes), nicht das Payload!
        if (isSignal && message.size() >= 4) {
            // Prüfe ob die ersten 4 Bytes ein Header sein könnten
            // Entweder: 00 00 xx xx (Datenpaket) oder ff ff xx xx (weiteres Signal)
            unsigned char byte0 = (unsigned char)message[0];
            unsigned char byte1 = (unsigned char)message[1];

            if ((byte0 == 0x00 && byte1 == 0x00) || (byte0 == 0xff && byte1 == 0xff)) {
#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
                qDebug() << "⚠ Signal contains embedded header(s) - removing only signal header (4 bytes)";
                qDebug() << "  Embedded header:" << message.left(4).toHex(' ');
#endif
                // Entferne NUR den Signal-Header, lasse das Payload für das nächste Paket
                m_receiveBuffer.remove(0, 4);
                continue;  // Lese das eingebettete Paket im nächsten Loop
            }
        }

        message.replace(0x01, ' ');

        // Entferne verarbeitetes Paket aus Buffer (Header + Payload)
        m_receiveBuffer.remove(0, 4 + absLength);

        // Konvertiere zu String
        QString messageStr = QString::fromUtf8(message).trimmed();

#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
        qDebug() << "✓ Complete message received:";
        qDebug() << "  Signal:" << isSignal;
        qDebug() << "  Length:" << absLength;
        if (messageStr.size() > 200) {
            qDebug() << "  Content (first 200 chars):" << messageStr.left(200);
        } else {
            qDebug() << "  Content:" << messageStr;
        }
#endif

        // Verarbeite Nachricht (ignoriere leere Signal-Prompts wie "*")
        if (!isSignal || !messageStr.isEmpty()) {
            processDirectorMessage(unbashSpaces(messageStr), isSignal);
        }
    }
}

void BareosDirector::processDirectorMessage(const QString &message, bool isSignal)
{
    // Ignoriere leere Nachrichten
    if (message.isEmpty()) {
        return;
    }

    // API-Response (JSON)?
    // First try direct match
    if (message.startsWith("{") || message.startsWith("[")) {
#ifdef IS_DEVELOPER
        qDebug() << "📄 JSON Response:";
        qDebug() << message;
#endif
        // State Machine: Detect resource type from JSON and mark as loaded
        detectAndMarkResourceLoaded(message);

        emit jsonResponse(m_lastCommand, message);
    } else {
        // Try to find JSON after trimming and removing leading garbage
        QString cleaned = message.trimmed();

        // Remove BOM if present
        if (!cleaned.isEmpty() && cleaned.at(0) == QChar(0xFEFF)) {
            cleaned = cleaned.mid(1);
        }

        // Find first JSON delimiter
        int jsonStart = -1;
        for (int i = 0; i < cleaned.length(); ++i) {
            QChar c = cleaned.at(i);
            if (c == '{' || c == '[') {
                jsonStart = i;
                break;
            }
        }

        // If we found JSON after some garbage, extract and emit as jsonResponse
        if (jsonStart > 0) {
            qDebug() << "BareosDirector: Found JSON at position" << jsonStart << "- removing leading garbage";
            QString jsonPart = cleaned.mid(jsonStart);
#ifdef IS_DEVELOPER
            qDebug() << "📄 JSON Response (cleaned):";
            qDebug() << jsonPart;
#endif
            emit jsonResponse(m_lastCommand, jsonPart);

            // State Machine: Check if .api command completed while in SettingApiMode
            if (m_connectionState == SettingApiMode && m_lastCommand.startsWith(".api")) {
                qDebug() << "STATE MACHINE: API mode confirmed (JSON response), starting resource loading";
                startResourceLoading();
            }
        } else {
            // Not JSON, emit as command response
            emit commandResponse(m_lastCommand, message);

            // State Machine: Check if .api command completed while in SettingApiMode
            if (m_connectionState == SettingApiMode && m_lastCommand.startsWith(".api")) {
                qDebug() << "STATE MACHINE: API mode confirmed, starting resource loading";
                startResourceLoading();
            }
            return;
        }
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
            case 1000:
                // 1000 OK - Command successful
#ifdef IS_DEVELOPER
                qDebug() << "✓ Command OK";
#endif
                emit statusMessage("OK");
                break;
            case 1002:
                // 1002 = Prompt received
                emit statusMessage(match.captured(3));
#ifdef IS_DEVELOPER
                qDebug() << "✓ Director ready (prompt received)";
#endif
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

void BareosDirector::onError(QAbstractSocket::SocketError error)
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

bool BareosDirector::setupTLSConnection()
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

bool BareosDirector::loadTLSCertificates(QSslConfiguration &sslConfig)
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

void BareosDirector::startAuthentication()
{
    qDebug() << "========================================";
    qDebug() << "STARTING AUTHENTICATION";
    qDebug() << "========================================";

    {
        QMutexLocker stateLocker(&m_stateMutex);
        m_connectionState = Authenticating;
    }

    {
        QMutexLocker authLocker(&m_authMutex);
        m_authCompleted = false;
    }

    // Create appropriate auth class based on AUTH_CLASS macro
    // AUTH_CLASS is defined in director.h as BaculaAuth or BareosAuth
    m_auth = new AUTH_CLASS(m_socket, this);

    // Connect authentication signals and store connections
    m_connAuthSucceeded = QObject::connect(m_auth, &AUTH_CLASS::authenticationSucceeded,
                                  this, &BareosDirector::onAuthenticationSucceeded);

    m_connAuthFailed = QObject::connect(m_auth, &AUTH_CLASS::authenticationFailed,
                               this, &BareosDirector::onAuthenticationFailed);

    m_connAuthStatus = QObject::connect(m_auth, &AUTH_CLASS::statusMessage,
                               this, &BareosDirector::onAuthStatusMessage);

    // Use PSK if password present
    bool usePSK = !m_password.isEmpty() && m_tlsConfig->tlsEnable;

    qDebug() << "PSK will be used:" << usePSK << "(password present:" << !m_password.isEmpty() << ")";

    // Start authentication
    bool authenticated = m_auth->authenticateDirector(
        m_directorName,
        m_consoleName,
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

void BareosDirector::onAuthenticationSucceeded(const QString directorVersion)
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

    {
        QMutexLocker stateLocker(&m_stateMutex);
        // DO NOT set Ready here - state machine will transition properly:
        // Authenticating -> SettingApiMode -> LoadingResources -> Ready
        m_connected = true;
    }

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

    // ✅ Signale sind bereits in connectSocketSignals() verbunden!
    // KEIN erneutes Connect nötig (würde zu doppelten Aufrufen führen)

    // ✅ Reset resource flags for new connection
    resetResourceFlags();

    // ✅ WICHTIG: Aktiviere JSON API-Modus SOFORT nach Authentifizierung
    // Dies muss VOR allen anderen Befehlen passieren!
    if (m_apiMode != ApiMode::Off) {
#ifdef IS_DEVELOPER
        qDebug() << "========================================";
        qDebug() << "ACTIVATING JSON API MODE";
        qDebug() << "  Mode:" << static_cast<int>(m_apiMode);
        qDebug() << "========================================";
#endif
        // State Machine: Transition to SettingApiMode
        setState(SettingApiMode);

        // Sende .api 2 Befehl für JSON Pretty-Print Modus
        sendCommand(".api 2");
    } else {
        // No API mode needed, go directly to LoadingResources
        startResourceLoading();
    }

    // ✅ Emit signals NACH dem API-Modus
    emit authentificationSucceeded(true, directorVersion);

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

    // ✅ Signalisiere Authentifizierung abgeschlossen
    {
        QMutexLocker authLocker(&m_authMutex);
        m_authCompleted = true;
        m_authCondition.wakeAll();
    }
}

void BareosDirector::onAuthenticationFailed(const QString &reason)
{
    qCritical() << "========================================";
    qCritical() << "✗ AUTHENTICATION FAILED";
    qCritical() << "  Reason:" << reason;
    qCritical() << "========================================";

    {
        QMutexLocker stateLocker(&m_stateMutex);
        m_connectionState = Disconnected;
        m_connected = false;
    }

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

    // ✅ Signalisiere Authentifizierung abgeschlossen (mit Fehler)
    {
        QMutexLocker authLocker(&m_authMutex);
        m_authCompleted = true;
        m_authCondition.wakeAll();
    }
}

void BareosDirector::onAuthStatusMessage(const QString &message)
{
    qDebug() << "[Auth]" << message;
    emit statusMessage(message);
}

// ============================================================================
// Command Handling
// ============================================================================

void BareosDirector::sendCommand(const QString &command)
{
#ifdef IS_DEVELOPER
    qDebug() << ">>> sendCommand() called from thread:" << QThread::currentThreadId();
    qDebug() << ">>> BareosDirector runs in thread:" << this->thread();
#endif

    // Allow commands in Ready, SettingApiMode (for .api), and LoadingResources (for dot-commands)
    bool canSend = (m_connectionState == Ready ||
                    m_connectionState == SettingApiMode ||
                    m_connectionState == LoadingResources) &&
                   m_socket->state() == QAbstractSocket::ConnectedState;

    if (!canSend) {
        qWarning() << "Cannot send command - not ready (state:" << m_connectionState << ", socket:" << m_socket->state() << ")";
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

    // ✅ KORREKT: Identisch zu BareosAuth::send()
    QByteArray packet = msg.toUtf8();
    qint32 len = packet.size();
    m_lastSentSize = len + 4;

    // Big-Endian System (SPARC, PowerPC, MIPS BE, etc.)
    #if Q_BYTE_ORDER == Q_BIG_ENDIAN
        packet.prepend(reinterpret_cast<const char*>(&len), 4);
    // Little-Endian System (x86, x86-64, ARM LE, etc.)
    #else
        qint32 lenBE = qToBigEndian(len);
        packet.prepend(reinterpret_cast<const char*>(&lenBE), 4);
    #endif

#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
    qDebug() << "  Packet size:" << packet.size() << "bytes (4 header + " << len << " payload)";
#endif

    m_lastCommand = command;

    // Thread-safe: Invoke socket write in the socket's thread
    QMetaObject::invokeMethod(this, "writeToSocket",
                              Qt::QueuedConnection,
                              Q_ARG(QByteArray, packet));
}

void BareosDirector::writeToSocket(const QByteArray &packet)
{
    if (!m_socket) {
        qCritical() << "writeToSocket: Socket is null!";
        return;
    }

    // This method runs in the worker thread where the socket lives
    qint64 written = m_socket->write(packet);

    if (written != packet.size()) {
        qCritical() << "Failed to send complete command! Written:" << written << "Expected:" << packet.size();
        emit statusMessage("Fehler beim Senden");
    } else {
#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
        qDebug() << "✓ Command sent successfully (" << written << "bytes)";
#endif
    }
}

//@deprecated
void BareosDirector::doSendCommand(Command cmd, quint64){ commandToString(cmd); }

void BareosDirector::doSendCommand(const Command cmd, const QString &args){
#ifdef IS_DEVELOPER
    qDebug() << ">>> doSendCommand() called from thread:" << QThread::currentThreadId();
    qDebug() << ">>> BareosDirector thread ID:" << this->thread()->currentThreadId();
#endif
    const QString cmdStr = commandToString(cmd, args);
    sendCommand(cmdStr);
}

// ============================================================================
// Command Helpers
// ============================================================================
const QString BareosDirector::commandToString(Command cmd, const QString &args)
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
    case Command::ListJobId:        command = QString("list joblog jobid=%1").arg(args); break;
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

    // Dot Commands (GUI/API mode commands)
    case Command::DotJobs:          command = ".jobs"; break;
    case Command::DotClients:       command = ".clients"; break;
    case Command::DotPools:         command = ".pools"; break;
    case Command::DotStorages:      command = ".storages"; break;
    case Command::DotFilesets:      command = ".filesets"; break;
    case Command::DotSchedule:      command = ".schedule"; break;
    case Command::DotCatalogs:      command = ".catalogs"; break;
    case Command::DotLevels:        command = ".levels"; break;
    case Command::DotTypes:         command = ".types"; break;
    case Command::DotMedia:         command = ".media"; break;
    case Command::DotHelp:          command = ".help"; break;

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

void BareosDirector::saveConnectionSettings()
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

void BareosDirector::loadConnectionSettings()
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

bool BareosDirector::hasStoredConnection() const
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

BareosDirector::JobStatus BareosDirector::parseJobStatus(const QString &status)
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

QString BareosDirector::jobStatusToString(JobStatus status)
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


