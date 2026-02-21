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

#include "director/bareosdirector.h"
#include "blogging.h"
#include "version.h"

#include <QApplication>
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
        ResourceType::Schedule,  // .schedule
        ResourceType::Catalog    // .catalogs
    };

    // Socket and Auth will be created in initialize() (called from worker thread)
}

void BareosDirector::initialize()
{
    DIR_DEBUG << "========================================";
    DIR_DEBUG << "INITIALIZATION";
    DIR_DEBUG << "  Thread ID:" << QThread::currentThreadId();
    DIR_DEBUG << "  Object thread affinity:" << this->thread();
    DIR_DEBUG << "========================================";

    // Create socket in this thread (worker thread)
    // Socket will inherit thread affinity from this QObject
    m_socket = new QSslSocket(this);

    // Connect socket signals
    connectSocketSignals();

    m_initialized = true;
    DIR_DEBUG << "  ✓ Socket created with correct thread affinity";
    DIR_DEBUG << "  ✓ Socket thread:" << m_socket->thread();
    DIR_DEBUG << "  ✓ BareosDirector thread:" << this->thread();
    DIR_DEBUG << "========================================";
}

BareosDirector::~BareosDirector()
{
    DIR_DEBUG << "Destructor called";

    // Cleanup socket
    if (m_socket) {
        // abort() immediately closes without waiting - cleaner for destructor
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->abort();
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

    DIR_DEBUG << "Destructor complete";
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
        DIR_CRITICAL << "Socket not initialized! Thread not running?";
        emit protocolError("Internal error: Socket not initialized");
        return;
    }

    DIR_DEBUG << "========================================";
    DIR_DEBUG << "CONNECTING TO " << backupSystemName() << " DIRECTOR " << directorName;
    DIR_DEBUG << "========================================";
    DIR_DEBUG << "Host: " << host;
    DIR_DEBUG << "Port: " << port;
    DIR_DEBUG << "Console: " << consoleName;
    DIR_DEBUG << "Password present: " << (!password.isEmpty());
    DIR_DEBUG << "Thread: " << QThread::currentThreadId();
    DIR_DEBUG << "========================================";

    // Abort existing connection
    // Note: abort() immediately closes the socket synchronously (no need for waitForDisconnected)
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->abort();
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

    DIR_DEBUG << "Connecting via plain TCP...";
    if (m_tlsConfig->tlsEnable) {
        DIR_DEBUG << "(TLS/PSK will be negotiated during authentication)";
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
        doSend(Command::Quit);
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

    DIR_DEBUG << "STATE MACHINE: " << stateNames.value(oldState, "?")
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

    // Skip if already loaded - prevents duplicate state transitions
    if (oldState == ResourceLoadState::Loaded) {
        return;
    }

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

    DIR_DEBUG << "RESOURCE: " << s_resourceNames.value(resourceType, "?")
             << s_loadStateNames.value(oldState) << "->" << "Loaded"
             << "(" << loaded << "/" << total << ")";

    emit resourceStateChanged(resourceType, ResourceLoadState::Loaded);
    emit resourceLoaded(resourceType);
    emit resourceLoadProgress(loaded, total);

    // Check if all resources are ready
    if (allResourcesReady()) {
        DIR_DEBUG << "STATE MACHINE: All resources loaded - transitioning to Ready";
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

    DIR_DEBUG << "RESOURCE: " << s_resourceNames.value(type, "?")
             << s_loadStateNames.value(oldState) << "->" << "Reloading";

    emit resourceStateChanged(type, ResourceLoadState::Reloading);

    // Send the appropriate dot-command
    switch (type) {
    case ResourceType::Job:
        doSend(Command::DotJobs);
        break;
    case ResourceType::Client:
        doSend(Command::DotClients);
        break;
    case ResourceType::Fileset:
        doSend(Command::DotFilesets);
        break;
    case ResourceType::Storage:
        doSend(Command::DotStorages);
        break;
    case ResourceType::Pool:
        doSend(Command::DotPools);
        break;
    case ResourceType::Level:
        doSend(Command::DotLevels);
        break;
    case ResourceType::Schedule:
        doSend(Command::DotSchedule);
        break;
    case ResourceType::Catalog:
        doSend(Command::DotCatalogs);
        break;
    default:
        BLOG_WARNING() << "RESOURCE: No dot-command for" << s_resourceNames.value(type, "?");
        break;
    }
}

void BareosDirector::markResourceFailed(ResourceType resourceType, const QString &errorMessage)
{
    ResourceLoadState oldState = m_resourceStates.value(resourceType, ResourceLoadState::Initial);
    m_resourceStates[resourceType] = ResourceLoadState::Failed;
    m_resourceErrors[resourceType] = errorMessage;

    BLOG_DEBUG() << "RESOURCE FAILED:" << s_resourceNames.value(resourceType, "?")
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

    // Primary: enum-based detection (fast, unambiguous)
    switch (m_lastCommandEnum) {
    case Command::DotLevels:    markResourceLoaded(ResourceType::Level);    return;
    case Command::DotFilesets:  markResourceLoaded(ResourceType::Fileset);  return;
    case Command::DotStorages:  markResourceLoaded(ResourceType::Storage);  return;
    case Command::DotPools:     markResourceLoaded(ResourceType::Pool);     return;
    case Command::DotSchedule:  markResourceLoaded(ResourceType::Schedule); return;
    case Command::DotCatalogs:  markResourceLoaded(ResourceType::Catalog);  return;
    case Command::DotJobs:      markResourceLoaded(ResourceType::Job);      return;
    case Command::DotClients:   markResourceLoaded(ResourceType::Client);   return;
    default:
        break;  // Fall through to JSON key detection for untracked commands
    }

    // Fallback: JSON key detection (for commands sent via Custom)
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    if (result.contains("levels"))    markResourceLoaded(ResourceType::Level);
    if (result.contains("filesets"))  markResourceLoaded(ResourceType::Fileset);
    if (result.contains("storages")) markResourceLoaded(ResourceType::Storage);
    if (result.contains("pools"))    markResourceLoaded(ResourceType::Pool);
    if (result.contains("schedules"))markResourceLoaded(ResourceType::Schedule);
    if (result.contains("catalogs")) markResourceLoaded(ResourceType::Catalog);

    if (result.contains("jobs")) {
        QJsonArray jobsArray = result["jobs"].toArray();
        if (!jobsArray.isEmpty()) {
            QJsonObject firstJob = jobsArray[0].toObject();
            if (firstJob.contains("enabled") || firstJob.contains("fileset")) {
                markResourceLoaded(ResourceType::Job);
            }
        }
    }

    if (result.contains("clients")) {
        QJsonArray clientsArray = result["clients"].toArray();
        if (!clientsArray.isEmpty()) {
            QJsonObject firstClient = clientsArray[0].toObject();
            if (!firstClient.contains("address") && !firstClient.contains("uname")
                && !firstClient.contains("clientid")) {
                markResourceLoaded(ResourceType::Client);
            }
        }
    }
}

void BareosDirector::startResourceLoading()
{
    DIR_DEBUG << "STATE MACHINE: Starting resource loading...";
    DIR_DEBUG << "  Required resources: " << m_requiredResources.size();

    setState(LoadingResources);

    // Set all required resources to Waiting state and send dot-commands
    for (const ResourceType &type : m_requiredResources) {
        m_resourceStates[type] = ResourceLoadState::Waiting;
        emit resourceStateChanged(type, ResourceLoadState::Waiting);

        switch (type) {
        case ResourceType::Job:
            doSend(Command::DotJobs);
            break;
        case ResourceType::Client:
            doSend(Command::DotClients);
            break;
        case ResourceType::Fileset:
            doSend(Command::DotFilesets);
            break;
        case ResourceType::Storage:
            doSend(Command::DotStorages);
            break;
        case ResourceType::Pool:
            doSend(Command::DotPools);
            break;
        case ResourceType::Level:
            doSend(Command::DotLevels);
            break;
        case ResourceType::Schedule:
            doSend(Command::DotSchedule);
            break;
        case ResourceType::Catalog:
            doSend(Command::DotCatalogs);
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
                BLOG_DEBUG() << "Socket state changed to:" << state;
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
                BLOG_ERROR() << "Socket error:" << errorMsg;
                // Note: onError() slot already emits protocolError for this signal
            });

    QObject::connect(m_socket, &QSslSocket::modeChanged, this,
            [this](QSslSocket::SslMode mode) {
                BLOG_DEBUG() << "========================================";
                BLOG_DEBUG() << "SSL MODE CHANGED";
                BLOG_DEBUG() << "  Mode:" << (mode == QSslSocket::UnencryptedMode ?
                                              "Unencrypted" : "SslClientMode/SslServerMode");
                BLOG_DEBUG() << "========================================";
            });

    QObject::connect(m_socket, &QSslSocket::encryptedBytesWritten, this,
            [this](qint64 written) {
                BLOG_DEBUG() << "Encrypted bytes written:" << written;
            });

    QObject::connect(m_socket, &QSslSocket::peerVerifyError, this,
            [this](const QSslError &error) {
                BLOG_DEBUG() << "⚠ Peer verify error:" << error.errorString();
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
        m_binaryJsonAccumulator.clear();
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
    BLOG_DEBUG() << "Setting API mode to:" << modeStr;
#endif

    // Thread-safe: Use QMetaObject::invokeMethod to call doSend in the correct thread
    QMetaObject::invokeMethod(this, "doSend",
                              Qt::QueuedConnection,
                              Q_ARG(BareosDirector::Command, Command::ApiMode),
                              Q_ARG(QString, modeStr));
}
// ============================================================================
// Socket Event Handlers
// ============================================================================

void BareosDirector::onConnected()
{
    DIR_DEBUG << "========================================";
    DIR_DEBUG << "TCP CONNECTED";
    DIR_DEBUG << "========================================";

    startAuthentication();
}

void BareosDirector::onEncrypted()
{
    DIR_DEBUG << "========================================";
    DIR_DEBUG << "✓ PSK-TLS HANDSHAKE SUCCESSFUL";
    DIR_DEBUG << "========================================";
    DIR_DEBUG << "  Encrypted: true";
    DIR_DEBUG << "  Protocol: " << m_socket->sessionProtocol();
    DIR_DEBUG << "  Cipher: " << m_socket->sessionCipher().name();

    if (!m_socket->peerCertificate().isNull()) {
        BLOG_DEBUG() << "  Peer Certificate:";
        BLOG_DEBUG() << "    Subject:" << m_socket->peerCertificate().subjectInfo(QSslCertificate::CommonName);
    }
    BLOG_DEBUG() << "========================================";

    // IMPORTANT: Do NOT call startAuthentication() here!
    // For PSK-TLS, authentication continues in BareosAuth::onEncrypted()
    // For non-PSK, startAuthentication() is called from onConnected()
}

void BareosDirector::onSslErrors(const QList<QSslError> &errors)
{
    BLOG_WARNING() << "========================================";
    BLOG_WARNING() << "SSL ERRORS OCCURRED (" << errors.size() << "errors)";
    BLOG_WARNING() << "========================================";

    for (const QSslError &error : errors) {
        BLOG_WARNING() << "  Error:" << error.errorString();
        BLOG_WARNING() << "  Error Type:" << error.error();
        if (!error.certificate().isNull()) {
            BLOG_WARNING() << "  Certificate:";
            BLOG_WARNING() << "    Subject:" << error.certificate().subjectInfo(QSslCertificate::CommonName);
            BLOG_WARNING() << "    Issuer:" << error.certificate().issuerInfo(QSslCertificate::CommonName);
            BLOG_WARNING() << "    Valid from:" << error.certificate().effectiveDate();
            BLOG_WARNING() << "    Valid until:" << error.certificate().expiryDate();
        }
    }

    BLOG_WARNING() << "========================================";

    if (!m_password.isEmpty()) {
        BLOG_WARNING() << "⚠ PSK-Mode: Ignoring SSL errors (this is normal for PSK)";
        m_socket->ignoreSslErrors();
    } else if (!m_tlsConfig->tlsVerifyPeer) {
        BLOG_WARNING() << "⚠ Peer verification disabled - ignoring SSL errors";
        m_socket->ignoreSslErrors();
    } else {
        BLOG_ERROR() << "✗ SSL errors with peer verification enabled - connection will fail";
        m_connectionState = Disconnected;
        QString errorSummary = QString("%1 SSL error(s): %2")
                                   .arg(errors.size())
                                   .arg(errors.first().errorString());
        emit protocolError("SSL error: " + errorSummary);
    }
}

void BareosDirector::onDisconnected()
{
    DIR_DEBUG << "========================================";
    DIR_DEBUG << "CONNECTION CLOSED";
    DIR_DEBUG << "========================================";

    {
        QMutexLocker stateLocker(&m_stateMutex);
        m_connected = false;
        m_connectionState = Disconnected;
    }

    // Cleanup
    m_binaryJsonAccumulator.clear();
    m_receiveBuffer.clear();
    m_jsonTextAccumulation = false;
    m_consumeApiConfirmation = false;

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
        BLOG_WARNING() << "⚠ readyRead during authentication - should not happen!";
        return;
    }

    // Lese neue Daten
    QByteArray newData = m_socket->readAll();
    m_receiveBuffer.append(newData);

#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
    BLOG_DEBUG() << "<<<< Received" << newData.size() << "bytes";
    BLOG_DEBUG() << "     Buffer total:" << m_receiveBuffer.size() << "bytes";
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
            BLOG_DEBUG() << "📄 Entering JSON text accumulation mode (API mode enabled)";
#endif
            m_jsonTextAccumulation = true;
        }

        // Try to parse buffer as complete JSON
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(m_receiveBuffer, &parseError);

        if (parseError.error == QJsonParseError::NoError) {
            // Successfully parsed JSON - we have at least one complete object
#ifdef IS_DEVELOPER
            BLOG_DEBUG() << "✓ Complete JSON parsed, size:" << m_receiveBuffer.size() << "bytes";
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
                BLOG_DEBUG() << "⏳ Incomplete JSON, waiting for more data. Error:" << parseError.errorString()
                         << "at offset" << parseError.offset;
#endif
                // Wait for more data
                if (m_receiveBuffer.size() > 10000000) {  // 10MB limit
                    BLOG_ERROR() << "⚠ JSON buffer too large without valid JSON:" << m_receiveBuffer.size();
                    BLOG_ERROR() << "   Parse error:" << parseError.errorString();
                    BLOG_ERROR() << "   Buffer start:" << QString::fromUtf8(m_receiveBuffer.left(200));
                    m_receiveBuffer.clear();
                    m_jsonTextAccumulation = false;
                }
                return;  // Wait for more data
            } else {
                // Real parse error - log and clear
                BLOG_ERROR() << "⚠ JSON parse error:" << parseError.errorString();
                BLOG_ERROR() << "   Error offset:" << parseError.offset;
                BLOG_ERROR() << "   Buffer content:" << QString::fromUtf8(m_receiveBuffer.left(500));
                m_receiveBuffer.clear();
                m_jsonTextAccumulation = false;  // Exit JSON mode on real error
                return;
            }
        }
    }

    // ✅ If we reach here and JSON accumulation is active but we're not in API mode anymore,
    // reset the flag (shouldn't normally happen)
    if (m_jsonTextAccumulation && m_apiMode == ApiMode::Off) {
        BLOG_WARNING() << "⚠ JSON accumulation active but API mode is off - resetting";
        m_jsonTextAccumulation = false;
    }

    // Verarbeite alle vollständigen Pakete im Buffer
    while (m_receiveBuffer.size() >= 4) {
        // Lese 4-Byte Message-Length Header (signed 32-bit big-endian)
        qint32 messageLength = 0;
        memcpy(&messageLength, m_receiveBuffer.constData(), 4);
        messageLength = qFromBigEndian(messageLength);

#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
        BLOG_DEBUG() << "     Message length from header:" << messageLength;
        BLOG_DEBUG() << "     First 20 bytes (hex):" << m_receiveBuffer.left(20).toHex(' ');
#endif

#ifdef IS_DEVELOPER
        // Warn about suspicious message lengths
        // (This should not happen now that JSON is handled properly above)
        if (messageLength > 100000 || messageLength < -100) {
            BLOG_ERROR() << "⚠ SUSPICIOUS MESSAGE LENGTH!" << messageLength;
            BLOG_ERROR() << "   API Mode:" << (m_apiMode != ApiMode::Off ? "ENABLED" : "DISABLED");
            BLOG_ERROR() << "   JSON accumulation:" << m_jsonTextAccumulation;
            BLOG_ERROR() << "   Buffer content (first 100 bytes hex):" << m_receiveBuffer.left(100).toHex(' ');
            BLOG_ERROR() << "   Buffer content (as string):" << QString::fromLatin1(m_receiveBuffer.left(100));
            // Clear buffer and break to avoid infinite loop
            m_receiveBuffer.clear();
            m_jsonTextAccumulation = false;
            m_binaryJsonAccumulator.clear();
            break;
        }
#endif

        // Negative Länge = Signal-Nachricht (z.B. Prompt)
        bool isSignal = (messageLength < 0);
        qint32 absLength = qAbs(messageLength);

        // Prüfe ob vollständiges Paket vorhanden (4 Bytes Header + Payload)
        if (m_receiveBuffer.size() < 4 + absLength) {
#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
            BLOG_DEBUG() << "     Waiting for more data..."
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
            // - Data packets start with 0x00 (positive length in big-endian)
            // - Signal packets start with 0xff (negative length in big-endian, two's complement)
            unsigned char byte0 = (unsigned char)message[0];

            if (byte0 == 0x00 || byte0 == 0xff) {
                DIR_DEBUG << "Signal contains embedded header - removing only signal header (4 bytes)";
                DIR_DEBUG << "  Embedded header: " << message.left(4).toHex(' ');
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
        BLOG_DEBUG() << "✓ Complete message received:";
        BLOG_DEBUG() << "  Signal:" << isSignal;
        BLOG_DEBUG() << "  Length:" << absLength;
        if (messageStr.size() > 200) {
            BLOG_DEBUG() << "  Content (first 200 chars):" << messageStr.left(200);
        } else {
            BLOG_DEBUG() << "  Content:" << messageStr;
        }
#endif

        // ✅ Multi-telegram JSON accumulation for binary protocol
        // A single JSON response from the Director can span multiple binary telegrams.
        // We accumulate data telegrams and process the complete JSON when:
        //   (a) a signal packet arrives (end-of-response delimiter), or
        //   (b) the accumulated data is already valid JSON (single-telegram response)
        if (isSignal) {
            // Signal marks end of current response
            if (!m_binaryJsonAccumulator.isEmpty()) {
                processDirectorMessage(unbashSpaces(m_binaryJsonAccumulator), false);
                m_binaryJsonAccumulator.clear();
            }
            // Process signal itself if non-empty
            if (!messageStr.isEmpty()) {
                processDirectorMessage(unbashSpaces(messageStr), true);
            }
        } else if (messageStr.startsWith('{') || messageStr.startsWith('[')
                   || !m_binaryJsonAccumulator.isEmpty()) {
            // JSON data telegram — accumulate (may span multiple telegrams)
            m_binaryJsonAccumulator += messageStr;

            // Check if JSON is already complete (avoids waiting for signal)
            QJsonParseError parseErr;
            QJsonDocument::fromJson(m_binaryJsonAccumulator.toUtf8(), &parseErr);
            if (parseErr.error == QJsonParseError::NoError) {
                // Complete JSON — process immediately
                processDirectorMessage(unbashSpaces(m_binaryJsonAccumulator), false);
                m_binaryJsonAccumulator.clear();
            }
            // Otherwise wait for more telegrams or signal
        } else {
            // Non-JSON text data — process immediately
            if (!messageStr.isEmpty()) {
                processDirectorMessage(unbashSpaces(messageStr), false);
            }
        }
    }
}

void BareosDirector::processDirectorMessage(const QString &message, bool isSignal)
{
    // Ignoriere leere Nachrichten
    if (message.isEmpty()) {
        return;
    }

    // ✅ Consume the JSON confirmation of .api 2 mode switch without dequeuing.
    // The Director sends TWO responses for .api 2: a text status message (processed first,
    // triggers startResourceLoading()) and a JSON-RPC confirmation. The JSON confirmation
    // must NOT dequeue from the command queue (the queue now contains resource commands).
    if (m_consumeApiConfirmation && !isSignal && message.startsWith('{')) {
        m_consumeApiConfirmation = false;
#ifdef IS_DEVELOPER
        BLOG_DEBUG() << "Consuming .api 2 JSON confirmation (not dequeuing from command queue)";
#endif
        return;
    }

    // ---- Command Queue: dequeue the matching command for this response ----
    // Signal messages (negative-length packets) are not command responses
    CommandEntry currentEntry;
    bool hasQueuedCommand = false;
    if (!isSignal) {
        hasQueuedCommand = dequeueCurrentCommand(currentEntry);
        if (hasQueuedCommand) {
            m_lastCommandEnum = currentEntry.cmd;
            m_lastCommand = commandToString(currentEntry.cmd, currentEntry.args);
        }
    }

    // API-Response (JSON)?
    // First try direct match
    if (message.startsWith("{") || message.startsWith("[")) {
#ifdef IS_DEVELOPER
        BLOG_DEBUG() << "📄 JSON Response (cmd=" << static_cast<int>(m_lastCommandEnum) << "):";
        BLOG_DEBUG() << message;
#endif
        // State Machine: Detect resource type from JSON and mark as loaded
        detectAndMarkResourceLoaded(message);

        emit jsonResult(m_lastCommandEnum, message);
        routeTypedResponse(message);

        // Track command completion
        if (hasQueuedCommand) {
            currentEntry.response = message;
            currentEntry.status = detectCommandError(message)
                                      ? CommandEntry::Failed : CommandEntry::Success;
            m_commandHistory.append(currentEntry);
            if (m_commandHistory.size() > MaxHistorySize)
                m_commandHistory.removeFirst();
            if (currentEntry.status == CommandEntry::Failed)
                emit commandFailed(currentEntry.cmd, currentEntry.args, message);
        }

        // State Machine: Check if .api command completed while in SettingApiMode
        if (m_connectionState == SettingApiMode && m_lastCommandEnum == Command::ApiMode) {
            DIR_DEBUG << "STATE MACHINE: API mode confirmed (clean JSON), starting resource loading";
            startResourceLoading();
        }
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

        // If we found JSON after some garbage, extract and emit
        if (jsonStart > 0) {
            QString jsonPart = cleaned.mid(jsonStart);
#ifdef IS_DEVELOPER
            BLOG_DEBUG() << "📄 JSON Response (cleaned, cmd=" << static_cast<int>(m_lastCommandEnum) << "):";
            BLOG_DEBUG() << jsonPart;
#endif
            emit jsonResult(m_lastCommandEnum, jsonPart);
            routeTypedResponse(jsonPart);

            // Track command completion
            if (hasQueuedCommand) {
                currentEntry.response = jsonPart;
                currentEntry.status = detectCommandError(jsonPart)
                                          ? CommandEntry::Failed : CommandEntry::Success;
                m_commandHistory.append(currentEntry);
                if (m_commandHistory.size() > MaxHistorySize)
                    m_commandHistory.removeFirst();
                if (currentEntry.status == CommandEntry::Failed)
                    emit commandFailed(currentEntry.cmd, currentEntry.args, jsonPart);
            }

            // State Machine: Check if .api command completed while in SettingApiMode
            if (m_connectionState == SettingApiMode && m_lastCommandEnum == Command::ApiMode) {
                DIR_DEBUG << "STATE MACHINE: API mode confirmed (JSON response), starting resource loading";
                startResourceLoading();
            }
        } else {
            // Not JSON, emit as text response
            emit textResult(m_lastCommandEnum, message);

            // Track command completion
            if (hasQueuedCommand) {
                currentEntry.response = message;
                currentEntry.status = detectCommandError(message)
                                          ? CommandEntry::Failed : CommandEntry::Success;
                m_commandHistory.append(currentEntry);
                if (m_commandHistory.size() > MaxHistorySize)
                    m_commandHistory.removeFirst();
                if (currentEntry.status == CommandEntry::Failed)
                    emit commandFailed(currentEntry.cmd, currentEntry.args, message);
            }

            // State Machine: Check if .api command completed while in SettingApiMode
            if (m_connectionState == SettingApiMode && m_lastCommandEnum == Command::ApiMode) {
                DIR_DEBUG << "STATE MACHINE: API mode confirmed, starting resource loading";
                // The Director sends TWO responses for .api 2: this text status and a JSON
                // confirmation that follows. Set flag to consume the JSON without dequeuing.
                m_consumeApiConfirmation = true;
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
            BLOG_WARNING() << "⚠ Director error:" << message;
            emit commandError(m_lastCommand, message);
            return;
        } else {
#ifdef IS_DEVELOPER
            BLOG_DEBUG() << "ℹ Director status:" << message;
#endif

            switch (statusCode) {
            case 1000:
                // 1000 OK - Command successful
#ifdef IS_DEVELOPER
                BLOG_DEBUG() << "✓ Command OK";
#endif
                emit statusMessage("OK");
                break;
            case 1002:
                // 1002 = Prompt received
                emit statusMessage(match.captured(3));
#ifdef IS_DEVELOPER
                BLOG_DEBUG() << "✓ Director ready (prompt received)";
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
    BLOG_DEBUG() << "📝 Text response:" << message;
#endif
    // emit commandResponse(m_lastCommand, message);
}

void BareosDirector::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    QString errorMsg = QString("Socket error: %1").arg(m_socket->errorString());

    BLOG_ERROR() << errorMsg;
    m_connectionState = Disconnected;
    emit protocolError(errorMsg);
}

// ============================================================================
// Authentication
// ============================================================================

void BareosDirector::startAuthentication()
{
    BLOG_DEBUG() << "========================================";
    BLOG_DEBUG() << "STARTING AUTHENTICATION";
    BLOG_DEBUG() << "========================================";

    {
        QMutexLocker stateLocker(&m_stateMutex);
        m_connectionState = Authenticating;
    }

    {
        QMutexLocker authLocker(&m_authMutex);
        m_authCompleted = false;
    }

    // IMPORTANT: Disconnect our readyRead handler during authentication
    // BareosAuth has its own readyRead handler - having both connected
    // causes signal conflicts where BareosDirector steals data meant for BareosAuth
    QObject::disconnect(m_connSocketReadyRead);
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "  Disconnected BareosDirector::onReadyRead during authentication";
#endif

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

    // Use PSK if password present and PSK is enabled
    bool usePSK = !m_password.isEmpty() && m_tlsConfig->tlsEnable && m_tlsConfig->tlsPSKEnable;

    BLOG_DEBUG() << "PSK will be used:" << usePSK << "(password present:" << !m_password.isEmpty()
             << ", tlsPSKEnable:" << m_tlsConfig->tlsPSKEnable << ")";

    // Set certificate files for TLS-Certificate mode (non-PSK)
#ifndef Q_OS_WINDOWS
    if (m_tlsConfig->tlsEnable && !m_tlsConfig->tlsPSKEnable) {
        QString caPath = m_tlsConfig->tlsCaCertFile ? m_tlsConfig->tlsCaCertFile->fileName() : QString();
        QString certPath = m_tlsConfig->tlsCertFile ? m_tlsConfig->tlsCertFile->fileName() : QString();
        QString keyPath = m_tlsConfig->tlsKeyFile ? m_tlsConfig->tlsKeyFile->fileName() : QString();

        BLOG_DEBUG() << "Setting certificate files for TLS-Certificate mode:";
        BLOG_DEBUG() << "  CA:" << caPath;
        BLOG_DEBUG() << "  Cert:" << certPath;
        BLOG_DEBUG() << "  Key:" << keyPath;

        m_auth->setCertificateFiles(caPath, certPath, keyPath);
    }
#endif

    // Set cipher list if configured
    if (!m_tlsConfig->tlsCipherList.isEmpty()) {
        BLOG_DEBUG() << "Setting configured cipher list:" << m_tlsConfig->tlsCipherList;
        m_auth->setCipherList(m_tlsConfig->tlsCipherList);
    }

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
        BLOG_ERROR() << "Failed to start authentication:" << m_auth->getErrorMessage();
        onAuthenticationFailed(m_auth->getErrorMessage());
    }
}

void BareosDirector::onAuthenticationSucceeded(const QString directorVersion)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "========================================";
    BLOG_DEBUG() << "✓ AUTHENTICATION SUCCESSFUL";
    BLOG_DEBUG() << "  Encryption:" << (m_socket->sessionCipher().name().isEmpty() ? "None" : m_socket->sessionCipher().name());
    BLOG_DEBUG() << "  Director Version:" << directorVersion;
    BLOG_DEBUG() << "========================================";
#endif

    // ✅ Disconnect auth signals - authentication complete
    QObject::disconnect(m_connAuthSucceeded);
    QObject::disconnect(m_connAuthFailed);
    QObject::disconnect(m_connAuthStatus);

    m_directorVersion = directorVersion;

    // Update Director name from CRAM-MD5 challenge (real name from server)
    QString remoteName = m_auth->remoteDirectorName();
    if (!remoteName.isEmpty() && remoteName != m_directorName) {
        DIR_DEBUG << "Director name updated: " << m_directorName << " -> " << remoteName;
        m_directorName = remoteName;
    }

    // Store TLS cipher list from auth for config export
    m_tlsCipherList = m_auth->tlsCipherList();

    {
        QMutexLocker stateLocker(&m_stateMutex);
        // DO NOT set Ready here - state machine will transition properly:
        // Authenticating -> SettingApiMode -> LoadingResources -> Ready
        m_connected = true;
    }

    // Cleanup auth object
    m_auth->deleteLater();
    m_auth = nullptr;

    // IMPORTANT: Reconnect our readyRead handler now that authentication is complete
    // This must happen BEFORE sending any commands (like .api 2)
    m_connSocketReadyRead = QObject::connect(m_socket, &QSslSocket::readyRead,
                                    this, &BareosDirector::onReadyRead,
                                    Qt::DirectConnection);
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "  Reconnected BareosDirector::onReadyRead after authentication";
#endif

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
        BLOG_DEBUG() << "  Parsed version:" << major << "." << minor << "." << patch;

        // Enable compression if Director supports it
        //@TODO !!!
        if (major >= 1) {
            BLOG_DEBUG() << "  Director supports compression";
        }
#endif
    }

    // ✅ Signale sind bereits in connectSocketSignals() verbunden!
    // KEIN erneutes Connect nötig (würde zu doppelten Aufrufen führen)

    // ✅ Reset resource flags for new connection
    resetResourceFlags();

    // ✅ WICHTIG: Aktiviere JSON API-Modus SOFORT nach Authentifizierung
    // Dies muss VOR allen anderen Befehlen passieren!
    if (m_apiMode != ApiMode::Off) {
#ifdef IS_DEVELOPER
        BLOG_DEBUG() << "========================================";
        BLOG_DEBUG() << "ACTIVATING JSON API MODE";
        BLOG_DEBUG() << "  Mode:" << static_cast<int>(m_apiMode);
        BLOG_DEBUG() << "========================================";
#endif
        // State Machine: Transition to SettingApiMode
        setState(SettingApiMode);

        // Sende .api 2 Befehl für JSON Pretty-Print Modus
        doSend(Command::ApiMode, "2");
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
            doSend(Command::DotMessages);  // Keep connection alive
        }
#ifdef IS_DEVELOPER
        BLOG_DEBUG() << ".";
#endif
    });
    // keepAliveTimer->start(3000);

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "✓ Connection established and ready";
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
    BLOG_ERROR() << "========================================";
    BLOG_ERROR() << "✗ AUTHENTICATION FAILED";
    BLOG_ERROR() << "  Reason:" << reason;
    BLOG_ERROR() << "========================================";

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

    // Reconnect our readyRead handler for consistency (even though we're disconnecting)
    m_connSocketReadyRead = QObject::connect(m_socket, &QSslSocket::readyRead,
                                    this, &BareosDirector::onReadyRead,
                                    Qt::DirectConnection);

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
    BLOG_DEBUG() << "[Auth]" << message;
    emit statusMessage(message);
}

// ============================================================================
// Query Convenience Methods
// ============================================================================

void BareosDirector::routeTypedResponse(const QString &jsonData)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    if (!doc.isObject()) return;

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    switch (m_lastCommandEnum) {
    case Command::DotConsoles:
        emit consolesResult(result["consoles"].toArray());
        break;
    case Command::ShowConsole: {
        QJsonObject consoles = result["consoles"].toObject();
        // Extract the name from m_lastCommand (after "show console=")
        if (m_lastCommand.contains("=")) {
            QString name = m_lastCommand.mid(m_lastCommand.indexOf('=') + 1);
            emit showConsoleResult(name, consoles.value(name).toObject());
        } else {
            // "show console" without =name returns all consoles
            for (auto it = consoles.begin(); it != consoles.end(); ++it) {
                emit showConsoleResult(it.key(), it.value().toObject());
            }
        }
        break;
    }
    case Command::Configure:
        if (m_lastCommand.contains("add console")) {
            bool ok = jsonData.contains("created", Qt::CaseInsensitive);
            emit configureResult(ok, jsonData);
        }
        break;

    // Typed resource signals — connect directly to model parse slots
    case Command::DotFilesets:  emit dotFilesetsResult(jsonData); break;
    case Command::DotJobs:      emit dotJobsResult(jsonData); break;
    case Command::DotClients:   emit dotClientsResult(jsonData); break;
    case Command::DotStorages:  emit dotStoragesResult(jsonData); break;
    case Command::DotPools:     emit dotPoolsResult(jsonData); break;
    case Command::DotLevels:    emit dotLevelsResult(jsonData); break;
    case Command::DotCatalogs:  emit dotCatalogsResult(jsonData); break;
    case Command::DotSchedule:  emit dotScheduleResult(jsonData); break;
    case Command::ListClients:  emit listClientsResult(jsonData); break;
    case Command::ShowJobs:      emit showJobsResult(jsonData); break;
    case Command::ShowJobDefs:   emit showJobDefsResult(jsonData); break;
    case Command::ShowSchedules: emit showSchedulesResult(jsonData); break;

    default:
        break;
    }
}

void BareosDirector::queryConsoles()
{
    doSend(Command::DotConsoles);
}

void BareosDirector::queryShowConsole(const QString &name)
{
    doSend(Command::ShowConsole, name);
}

void BareosDirector::queryConfigureAddConsole(const QString &name, const QString &password,
                                               const QString &profile)
{
    doSend(Command::Configure,
           QString("add console name=%1 password=\"%2\" profile=%3 tlsenable=false")
               .arg(name, password, profile));
}

void BareosDirector::sendCommand(const QString &command)
{
    // Allow commands in Ready, SettingApiMode (for .api), and LoadingResources (for dot-commands)
    bool canSend = (m_connectionState == Ready ||
                    m_connectionState == SettingApiMode ||
                    m_connectionState == LoadingResources) &&
                   m_socket->state() == QAbstractSocket::ConnectedState;

    if (!canSend) {
        BLOG_WARNING() << "Cannot send command - not ready (state:" << m_connectionState << ", socket:" << m_socket->state() << ")";
        emit statusMessage("Nicht bereit");
        return;
    }

    QString msg = command;
    if (!msg.endsWith('\n')) {
        msg += '\n';
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << ">>> Sending command:" << command;
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
    BLOG_DEBUG() << "  Packet size:" << packet.size() << "bytes (4 header + " << len << " payload)";
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
        BLOG_ERROR() << "writeToSocket: Socket is null!";
        return;
    }

    // This method runs in the worker thread where the socket lives
    qint64 written = m_socket->write(packet);

    if (written != packet.size()) {
        BLOG_ERROR() << "Failed to send complete command! Written:" << written << "Expected:" << packet.size();
        emit statusMessage(tr("Error sending command"));
    } else {
#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
        BLOG_DEBUG() << "✓ Command sent successfully (" << written << "bytes)";
#endif
    }
}

//@deprecated
void BareosDirector::doSend(Command cmd, quint64){ commandToString(cmd); }

void BareosDirector::doSend(const Command cmd, const QString &args){
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << ">>> doSend() cmd=" << static_cast<int>(cmd) << " args=" << args;
#endif
    CommandEntry entry;
    entry.cmd = cmd;
    entry.args = args;
    registerRollback(entry);

    m_commandQueue.enqueue(entry);
    flushQueue();
}

void BareosDirector::flushQueue()
{
    for (int i = 0; i < m_commandQueue.size(); ++i) {
        CommandEntry &entry = m_commandQueue[i];
        if (entry.status == CommandEntry::Pending) {
            entry.status = CommandEntry::Sent;
            m_lastCommandEnum = entry.cmd;
            const QString cmdStr = commandToString(entry.cmd, entry.args);
            m_lastCommand = cmdStr;
            sendCommand(cmdStr);
        }
    }
}

bool BareosDirector::dequeueCurrentCommand(CommandEntry &entry)
{
    // Find and dequeue the first Sent entry (FIFO — matches Bareos serial processing)
    if (!m_commandQueue.isEmpty() && m_commandQueue.head().status == CommandEntry::Sent) {
        entry = m_commandQueue.dequeue();
        return true;
    }
    return false;
}

void BareosDirector::registerRollback(CommandEntry &entry)
{
    switch (entry.cmd) {
    case Command::Configure:
        // configure add fileset/client/console — rollback by deleting
        // Rollback args will need to be extracted from the response
        // (the Director returns the resource name in the configure response)
        break;
    case Command::BvfsRestore:
        entry.rollbackCmd = Command::BvfsCleanup;
        break;
    default:
        break;
    }
}

bool BareosDirector::detectCommandError(const QString &response) const
{
    // Bareos error patterns in JSON responses
    if (response.contains("\"error\"", Qt::CaseInsensitive)) return true;
    if (response.contains("ERR=", Qt::CaseSensitive)) return true;
    if (response.contains("Error:", Qt::CaseInsensitive)) return true;
    // JSON API error field
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    if (doc.isObject()) {
        QJsonObject root = doc.object();
        if (root.contains("error")) return true;
        QJsonObject result = root["result"].toObject();
        if (result.contains("error")) return true;
    }
    return false;
}

bool BareosDirector::rollbackLast()
{
    for (int i = m_commandHistory.size() - 1; i >= 0; --i) {
        const CommandEntry &entry = m_commandHistory[i];
        if (entry.status == CommandEntry::Success
            && entry.rollbackCmd != Command::Custom) {
            Command rollCmd = entry.rollbackCmd;
            QString rollArgs = entry.rollbackArgs.isEmpty() ? entry.args : entry.rollbackArgs;
            m_commandHistory.removeAt(i);
            doSend(rollCmd, rollArgs);
            emit rollbackCompleted(entry.cmd);
            return true;
        }
    }
    return false;
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
            command = ".api json compact=yes";  // Default: JSON
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
    case Command::ListJobs: {
        // Args format: "limit" or "limit,offset"
        if (args.isEmpty()) {
            command = "list jobs limit=100";
        } else if (args.contains(',')) {
            QStringList parts = args.split(',');
            QString limit = parts.at(0);
            QString offset = parts.size() > 1 ? parts.at(1) : "0";
            command = QString("list jobs limit=%1 offset=%2").arg(limit, offset);
        } else {
            command = QString("list jobs limit=%1").arg(args);
        }
        break;
    }
    case Command::ListJobsLast:     command = QString("list jobs last=%1").arg(args.isEmpty() ? "100" : args); break;
    case Command::ListJobId:        command = QString("list joblog jobid=%1").arg(args); break;
    case Command::ListClients:      command = "llist clients"; break;
    case Command::ListPools:        command = "list pools"; break;
    case Command::ListVolumes:      command = "list volumes"; break;
    case Command::ListVolumePool:   command = QString("list volumes pool=%1").arg(args); break;
    case Command::ListMedia:        command = "list media"; break;
    case Command::ListFileSets:     command = "list filesets"; break;
    case Command::ListFiles:        command = QString("list files jobid=%1").arg(args); break;
    case Command::ListNextVolume:   command = QString("list nextvol job=%1").arg(args); break;
    case Command::ListBackups:      command = "list backups"; break;
    case Command::ListBackupsClient: command = QString("list backups client=%1").arg(args); break;
    case Command::ListJobTotals:    command = "list jobtotals"; break;

    // Job Control
    case Command::Run:              command = QString("run job=%1").arg(args); break;
    case Command::RunYes:           command = QString("run job=%1 yes").arg(args); break;
    case Command::Cancel:           command = QString("cancel jobid=%1").arg(args); break;
    case Command::DeleteJob:        command = QString("delete job jobid=%1 yes").arg(args); break;
    case Command::DeleteFileSet:    command = QString("delete fileset=\"%1\" yes").arg(args); break;
    case Command::Disable:          command = QString("disable job=%1").arg(args); break;
    case Command::Enable:           command = QString("enable job=%1").arg(args); break;
    case Command::Rerun:            command = QString("rerun jobid=%1").arg(args); break;

    // Restore
    case Command::Restore:          command = QString("restore %1").arg(args); break;
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
    case Command::ShowJobDefs:      command = "show jobdefs"; break;
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
    case Command::DotDefaults:      command = ".defaults job=\"" + args + "\""; break;
    case Command::DotConsoles:      command = ".consoles"; break;
    case Command::DotMessages:      command = ".messages"; break;

    // Show single resource
    case Command::ShowFileset:      command = QString("show fileset=%1").arg(args); break;
    case Command::ShowClient:       command = QString("show client=%1").arg(args); break;
    case Command::ShowConsole:      command = QString("show console=%1").arg(args); break;

    // Volume Maintenance (bulk)
    case Command::PruneVolumeAll:   command = "prune volume allpools yes"; break;
    case Command::PurgeVolumeAll:   command = "purge volume allpools yes"; break;

    // BVFS (Virtual File System for restore)
    case Command::BvfsGetJobIds:    command = QString(".bvfs_get_jobids %1").arg(args); break;
    case Command::BvfsUpdate:       command = QString(".bvfs_update %1").arg(args); break;
    case Command::BvfsLsDirs:       command = QString(".bvfs_lsdirs %1").arg(args); break;
    case Command::BvfsLsFiles:      command = QString(".bvfs_lsfiles %1").arg(args); break;
    case Command::BvfsRestore:      command = QString(".bvfs_restore %1").arg(args); break;
    case Command::BvfsCleanup:      command = QString(".bvfs_cleanup %1").arg(args); break;

    // Custom
    case Command::Custom:           command = args; break;

    default:
        BLOG_WARNING() << "Unknown Command:" << static_cast<int>(cmd);
        command = args;
        break;
    }

    return command;
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


