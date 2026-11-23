// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2026 Joerg Steffens
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

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
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2025
 * @version 1.0.0
 */

#include "director/bareosdirector.h"
#include "director/bnetframe.h"
#include "director/bjsonreassembler.h"
#include "director/bdirectorreply.h"
#include "blogging.h"
#include "blogredact.h"
#include "bquote.h"

#include <QApplication>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QAuthenticator>
#include <QUrlQuery>
#include <QSslCipher>
#include <QSslError>
#include <QScopeGuard>

#ifdef HAVE_GRPC
#include "director/bgrpctransport.h"
#endif

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

    // Timeout for the LoadingResources phase - lives in this worker thread
    m_resourceLoadTimer = new QTimer(this);
    m_resourceLoadTimer->setSingleShot(true);
    m_resourceLoadTimer->setInterval(ResourceLoadTimeoutMs);
    QObject::connect(m_resourceLoadTimer, &QTimer::timeout,
                     this, &BareosDirector::onResourceLoadTimeout);

    // Timeout for the SettingApiMode phase - lives in this worker thread
    m_apiModeTimer = new QTimer(this);
    m_apiModeTimer->setSingleShot(true);
    m_apiModeTimer->setInterval(ApiModeTimeoutMs);
    QObject::connect(m_apiModeTimer, &QTimer::timeout,
                     this, &BareosDirector::onApiModeTimeout);

    // Timeout of the oldest command waiting for its reply
    m_commandTimer = new QTimer(this);
    m_commandTimer->setSingleShot(true);
    QObject::connect(m_commandTimer, &QTimer::timeout,
                     this, &BareosDirector::onCommandTimeout);

#ifdef HAVE_GRPC
    // gRPC transport (Onesimus Director) - lives in this worker thread, too
    m_grpc = new BGrpcTransport(this);
    QObject::connect(m_grpc, &BGrpcTransport::loggedIn, this, &BareosDirector::onGrpcLoggedIn);
    QObject::connect(m_grpc, &BGrpcTransport::failed, this, &BareosDirector::onGrpcFailed);
    QObject::connect(m_grpc, &BGrpcTransport::certificateNotTrusted,
                     this, &BareosDirector::onGrpcCertificateNotTrusted);
    QObject::connect(m_grpc, &BGrpcTransport::commandFinished,
                     this, &BareosDirector::onGrpcCommandFinished);
    QObject::connect(m_grpc, &BGrpcTransport::jobStatusEvent, this, &BareosDirector::onGrpcJobStatus);
    QObject::connect(m_grpc, &BGrpcTransport::eventMessage, this, &BareosDirector::statusMessage);
    QObject::connect(m_grpc, &BGrpcTransport::jobMessage, this, &BareosDirector::jobMessage);
    QObject::connect(m_grpc, &BGrpcTransport::connectionLost,
                     this, &BareosDirector::onGrpcConnectionLost);
    QObject::connect(m_grpc, &BGrpcTransport::configReply, this, &BareosDirector::configReply);
    QObject::connect(m_grpc, &BGrpcTransport::configCapabilities,
                     this, &BareosDirector::configCapabilitiesChanged);
    QObject::connect(m_grpc, &BGrpcTransport::usageChanged, this, &BareosDirector::usageChanged);
    QObject::connect(m_grpc, &BGrpcTransport::clientStatusEvent, this, &BareosDirector::clientStatusEvent);
#endif

    m_initialized = true;
    DIR_DEBUG << "  ✓ Socket created with correct thread affinity";
    DIR_DEBUG << "  ✓ Socket thread:" << m_socket->thread();
    DIR_DEBUG << "  ✓ BareosDirector thread:" << this->thread();
    DIR_DEBUG << "========================================";
}

BareosDirector::~BareosDirector()
{
    DIR_DEBUG << "Destructor called";

    // Runs in the Director's thread (BDirector deletes it when that thread
    // finishes). The socket, the auth object, the timers and the gRPC
    // transport are children and are deleted by ~QObject; before that, none
    // of them may call back into this half-destroyed object.
    if (m_socket) {
        QObject::disconnect(m_socket, nullptr, this, nullptr);
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->abort();
        }
    }
    if (m_auth) {
        QObject::disconnect(m_auth, nullptr, this, nullptr);
    }
#ifdef HAVE_GRPC
    if (m_grpc) {
        QObject::disconnect(m_grpc, nullptr, this, nullptr);
    }
#endif

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

void BareosDirector::configCall(quint64 id, const BConfig::Request &request)
{
#ifdef HAVE_GRPC
    if (m_transport == Transport::Grpc && m_grpc) {
        m_grpc->configCall(id, request);
        return;
    }
#endif
    // The console protocol has no configuration CRUD: nothing is sent
    BConfig::Reply reply;
    reply.status = BConfig::Status::NotAvailable;
    reply.rpcMessage = tr("Changing the configuration needs the gRPC interface of the Onesimus Director");
    QTimer::singleShot(0, this, [this, id, reply]() { emit configReply(id, reply); });
}

bool BareosDirector::grpcAvailable()
{
#ifdef HAVE_GRPC
    return true;
#else
    return false;
#endif
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
#ifdef HAVE_GRPC
    m_grpc->close();
#endif

    // Nothing of an earlier session may survive: a stale queue entry would
    // take the reply of the first command of the new session
    if (m_apiModeTimer) {
        m_apiModeTimer->stop();
    }
    failPendingCommands(tr("Connection to the Director was closed before the reply arrived"));
    m_jsonReply.clear();
    m_receiveBuffer.clear();
    m_consumeApiConfirmation = false;

    setState(Connecting);

    {
        QMutexLocker infoLocker(&m_infoMutex);
        m_host = host;
        m_port = port;
        m_directorName = directorName;
    }
    m_consoleName = consoleName;
    m_password = password;

    if (m_transport == Transport::Grpc) {
        connectGrpc();
        return;
    }

    DIR_DEBUG << "Connecting via plain TCP...";
    if (m_tlsConfig->tlsEnable) {
        DIR_DEBUG << "(TLS/PSK will be negotiated during authentication)";
    }

    // Connect via TCP - TLS comes during authentication
    m_socket->connectToHost(host, port);
}

void BareosDirector::disconnect()
{
    if (m_transport == Transport::Grpc) {
#ifdef HAVE_GRPC
        m_grpc->close();
#endif
        m_commandQueue.clear();
        setState(Disconnected);
        emit disconnected();
        return;
    }

    // Cleanup authentication
    if (m_auth) {
        // Disconnect auth signals
        QObject::disconnect(m_connAuthSucceeded);
        QObject::disconnect(m_connAuthFailed);
        QObject::disconnect(m_connAuthStatus);

        m_auth->deleteLater();
        m_auth = nullptr;
    }

    // Commands still waiting for a reply will not get one
    if (m_apiModeTimer) {
        m_apiModeTimer->stop();
    }
    failPendingCommands(tr("Disconnected from the Director before the reply arrived"));

    // Say goodbye, then close. "quit" is written directly, not queued:
    // nobody waits for a reply, and a queued write would only run after the
    // socket is closed. disconnectFromHost() writes what is pending first.
    // During the login the handshake owns the socket; nothing is said then.
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        if (m_connectionState == SettingApiMode || m_connectionState == LoadingResources
            || m_connectionState == Ready) {
            m_socket->write(commandPacket(commandToString(Command::Quit)));
        }
        m_socket->disconnectFromHost();
    }

    setState(Disconnected);
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
    // Never null while the object lives; for the Director's own thread (the
    // GUI thread uses the copy kept by BDirector, see BDirector::tlsConfig())
    return m_tlsConfig;
}

QString BareosDirector::currentHost() const
{
    QMutexLocker locker(&m_infoMutex);
    return m_host;
}

int BareosDirector::currentPort() const
{
    QMutexLocker locker(&m_infoMutex);
    return m_port;
}

QString BareosDirector::currentDirectorName() const
{
    QMutexLocker locker(&m_infoMutex);
    return m_directorName;
}

QString BareosDirector::tlsCipherList() const
{
    QMutexLocker locker(&m_infoMutex);
    return m_tlsCipherList;
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

    setResourceState(resourceType, ResourceLoadState::Loaded);
    m_resourceErrors.remove(resourceType);

    // Count loaded resources (only required ones, so loaded <= total)
    const int loaded = loadedResourceCount();
    const int total = m_requiredResources.size();

    DIR_DEBUG << "RESOURCE: " << s_resourceNames.value(resourceType, "?")
             << s_loadStateNames.value(oldState) << "->" << "Loaded"
             << "(" << loaded << "/" << total << ")";

    emit resourceStateChanged(resourceType, ResourceLoadState::Loaded);
    emit resourceLoaded(resourceType);
    if (m_requiredResources.contains(resourceType)) {
        emit resourceLoadProgress(loaded, total);
    }

    // Check if all resources are ready
    if (allResourcesReady()) {
        if (m_resourceLoadTimer) {
            m_resourceLoadTimer->stop();
        }
        // Only the end of the loading phase finishes initialization; a late reply
        // after a load timeout (state already Ready) must not trigger it twice
        if (m_connectionState != Ready) {
            DIR_DEBUG << "STATE MACHINE: All resources loaded - transitioning to Ready";
            setState(Ready);
            emit allResourcesLoaded();
        }
    }
}

int BareosDirector::loadedResourceCount() const
{
    int loaded = 0;
    for (const ResourceType &type : m_requiredResources) {
        if (m_resourceStates.value(type) == ResourceLoadState::Loaded) {
            loaded++;
        }
    }
    return loaded;
}

void BareosDirector::onResourceLoadTimeout()
{
    if (m_connectionState != LoadingResources) {
        return;
    }

    QStringList missing;
    for (const ResourceType &type : m_requiredResources) {
        if (m_resourceStates.value(type) != ResourceLoadState::Loaded) {
            missing << s_resourceNames.value(type, "?");
            markResourceFailed(type, tr("No response from Director within %1 seconds")
                                         .arg(ResourceLoadTimeoutMs / 1000));
        }
    }

    BLOG_WARNING() << "RESOURCE: Loading timed out after" << ResourceLoadTimeoutMs / 1000
                   << "s - missing:" << missing.join(", ")
                   << "(" << loadedResourceCount() << "/" << m_requiredResources.size() << "loaded)"
                   << "- continuing with incomplete resources";

    emit statusMessage(tr("Timeout while loading resources: %1").arg(missing.join(", ")));

    // Weiter nach Ready, damit die GUI nicht in LoadingResources hängen bleibt.
    // Fehlgeschlagene Ressourcen sind über resourceState()/resourceLoadFailed erkennbar.
    setState(Ready);
    emit allResourcesLoaded();
}

bool BareosDirector::allResourcesReady() const
{
    QMutexLocker locker(&m_infoMutex);
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
    QMutexLocker locker(&m_infoMutex);
    return m_resourceStates.value(type, ResourceLoadState::Initial);
}

bool BareosDirector::isResourceLoaded(ResourceType type) const
{
    QMutexLocker locker(&m_infoMutex);
    return m_resourceStates.value(type) == ResourceLoadState::Loaded;
}

bool BareosDirector::areAllResourcesLoaded() const
{
    return allResourcesReady();
}

void BareosDirector::reloadResource(ResourceType type)
{
    ResourceLoadState oldState = m_resourceStates.value(type, ResourceLoadState::Initial);
    setResourceState(type, ResourceLoadState::Reloading);

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
    setResourceState(resourceType, ResourceLoadState::Failed);
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

    if (m_apiModeTimer) {
        m_apiModeTimer->stop();
    }
    setState(LoadingResources);

    // Ohne Timeout bliebe die GUI für immer in LoadingResources hängen,
    // falls eine Antwort ausbleibt (siehe onResourceLoadTimeout())
    if (m_resourceLoadTimer) {
        m_resourceLoadTimer->start();
    }
    emit resourceLoadProgress(0, m_requiredResources.size());

    // Set all required resources to Waiting state and send dot-commands
    for (const ResourceType &type : m_requiredResources) {
        setResourceState(type, ResourceLoadState::Waiting);
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
            setResourceState(type, ResourceLoadState::Loaded);
            break;
        }
    }
}

void BareosDirector::resetResourceFlags()
{
    {
        QMutexLocker locker(&m_infoMutex);
        m_resourceStates.clear();
        // Initialize all required resources to Initial state
        for (const ResourceType &type : m_requiredResources) {
            m_resourceStates[type] = ResourceLoadState::Initial;
        }
    }
    m_resourceErrors.clear();
    m_errorMessage.clear();
}

void BareosDirector::setResourceState(ResourceType type, ResourceLoadState state)
{
    // Written only in the Director's thread, read from the GUI thread too
    QMutexLocker locker(&m_infoMutex);
    m_resourceStates[type] = state;
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
        m_jsonReply.clear();
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

    // Logging only. Whether an error may be ignored is decided in one place:
    // the handler the auth class installs before it starts the handshake
    // (BareosAuth::onSslErrors for TLS-PSK, onSslErrorsCertificate for
    // certificates). This slot is connected first and therefore runs first;
    // calling ignoreSslErrors() here would switch certificate verification off
    // before the auth class has looked at the errors.
}

void BareosDirector::onDisconnected()
{
    DIR_DEBUG << "========================================";
    DIR_DEBUG << "CONNECTION CLOSED";
    DIR_DEBUG << "========================================";

    setState(Disconnected);

    // Cleanup
    m_jsonReply.clear();
    m_receiveBuffer.clear();
    m_consumeApiConfirmation = false;
    if (m_apiModeTimer) {
        m_apiModeTimer->stop();
    }
    failPendingCommands(tr("Connection to the Director was lost before the reply arrived"));

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

void BareosDirector::failPendingCommands(const QString &reason)
{
    // Take the entries out first, so that the queue is already empty when
    // the failures are reported
    const QQueue<CommandEntry> dropped = m_commandQueue;
    m_commandQueue.clear();
    if (m_commandTimer) {
        m_commandTimer->stop();
    }

    for (const CommandEntry &entry : dropped) {
        if (entry.cmd == Command::Quit || entry.status == CommandEntry::TimedOut) {
            // Our own farewell, or already reported when it timed out
            continue;
        }
        emit commandFailed(entry.cmd, entry.args, reason);
    }
}

void BareosDirector::setCommandTimeout(int milliseconds)
{
    m_commandTimeoutMs = qMax(1, milliseconds);
    updateCommandTimer();
}

void BareosDirector::updateCommandTimer()
{
    if (!m_commandTimer) {
        return;
    }
    // The Director answers in order: only the oldest unanswered command is
    // timed. The API mode switch has its own timeout (onApiModeTimeout()).
    for (const CommandEntry &entry : std::as_const(m_commandQueue)) {
        if (entry.status == CommandEntry::TimedOut) {
            continue;
        }
        if (entry.status == CommandEntry::Sent && entry.cmd != Command::ApiMode) {
            if (!m_commandTimer->isActive() || m_commandTimer->interval() != m_commandTimeoutMs) {
                m_commandTimer->start(m_commandTimeoutMs);
            }
            return;
        }
        break;
    }
    m_commandTimer->stop();
}

void BareosDirector::onCommandTimeout()
{
    for (CommandEntry &entry : m_commandQueue) {
        if (entry.status == CommandEntry::TimedOut) {
            continue;
        }
        if (entry.status != CommandEntry::Sent || entry.cmd == Command::ApiMode) {
            return;
        }
        entry.status = CommandEntry::TimedOut;
        const Command cmd = entry.cmd;
        const QString args = entry.args;
        BLOG_WARNING() << "No reply to command" << static_cast<int>(cmd) << "within"
                       << m_commandTimeoutMs << "ms";
        // Time the next one before the signal, whose receivers may queue more
        m_commandTimer->start(m_commandTimeoutMs);
        emit commandFailed(cmd, args,
                           tr("The Director did not answer within %n second(s)", nullptr,
                              qMax(1, m_commandTimeoutMs / 1000)));
        return;
    }
}

void BareosDirector::finishJobHistory(CommandEntry entry, const QString &reply)
{
    // As for every other reply: text in front of the JSON is dropped
    QString payload = reply.trimmed();
    if (!payload.isEmpty() && payload.at(0) == QChar(0xFEFF)) {
        payload = payload.mid(1);
    }
    const qsizetype brace = payload.indexOf(QLatin1Char('{'));
    const qsizetype bracket = payload.indexOf(QLatin1Char('['));
    qsizetype jsonStart = brace;
    if (bracket >= 0 && (jsonStart < 0 || bracket < jsonStart)) {
        jsonStart = bracket;
    }
    if (jsonStart > 0) {
        payload = payload.mid(jsonStart);
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "Job history reply:" << payload.size() << "characters";
#endif

    // The command history keeps the outcome, not megabytes of rows. Whether
    // the Director refused the command is the consumer's to read.
    entry.status = jsonStart >= 0 ? CommandEntry::Success : CommandEntry::Failed;
    m_commandHistory.append(entry);
    if (m_commandHistory.size() > MaxHistorySize)
        m_commandHistory.removeFirst();

    emit jobHistoryResult(entry.args, payload);
}

QString BareosDirector::jobHistoryCommandLine(const QString &args)
{
    const QStringList parts = args.split(QLatin1Char(','));

    auto bounded = [](const QString &text, int fallback, int maximum) {
        bool ok = false;
        const int value = text.trimmed().toInt(&ok);
        if (!ok || value <= 0) return fallback;
        return qMin(value, maximum);
    };

    const int days = bounded(parts.value(0), DefaultJobHistoryDays, MaxJobHistoryDays);
    const int limit = bounded(parts.value(1), DefaultJobHistoryLimit, MaxJobHistoryLimit);

    return QStringLiteral("llist jobs days=%1 reverse limit=%2").arg(days).arg(limit);
}

void BareosDirector::abortConnection(const QString &reason)
{
    BLOG_ERROR() << reason;
    emit protocolError(reason);

    // abort() closes at once and ends in onDisconnected(), which resets the
    // state, the buffers and the command queue
    m_socket->abort();
    if (m_connectionState != Disconnected) {
        onDisconnected();
    }
}

void BareosDirector::onApiModeTimeout()
{
    if (m_connectionState != SettingApiMode) {
        return;
    }
    abortConnection(tr("The Director did not confirm the API mode within %1 seconds. "
                       "The connection was closed.").arg(ApiModeTimeoutMs / 1000));
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

    // Every reply comes in BNET telegrams, in every API mode (Bareos bsock,
    // odir crates/bareos-bnet). The telegrams are taken from the front of the
    // buffer by offset and the consumed bytes removed once at the end, so a
    // buffer holding many telegrams is not copied once per telegram.
    qsizetype offset = 0;
    const auto consumed = qScopeGuard([this, &offset]() {
        m_receiveBuffer.remove(0, offset);
    });

    while (m_receiveBuffer.size() - offset >= BnetFrame::HeaderSize) {
        // Reply handlers in this thread may end the session (disconnect(),
        // abortConnection()), which empties the buffer under our feet
        if (offset > m_receiveBuffer.size()) {
            offset = 0;
            return;
        }

        // Lese 4-Byte Message-Length Header (signed 32-bit big-endian)
        const qint32 messageLength = BnetFrame::announcedLength(m_receiveBuffer.constData() + offset);
        const BnetFrame::HeaderKind headerKind =
            BnetFrame::classifyHeader(messageLength, BnetFrame::MaxSessionPayload);

#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
        BLOG_DEBUG() << "     Message length from header:" << messageLength;
#endif

        // The length comes from the network: the same limit in every build.
        // Nothing is buffered for a telegram larger than the limit; the
        // stream cannot be resynchronised after it, so the connection ends.
        if (headerKind == BnetFrame::HeaderKind::TooLarge) {
            offset = 0;   // abortConnection() clears the buffer itself
            abortConnection(tr("The Director announced a message of %1 bytes (limit: %2 MiB). "
                               "The connection was closed.")
                                .arg(messageLength)
                                .arg(BnetFrame::MaxSessionPayload / (1024 * 1024)));
            return;
        }

        // Negative Länge = BNET-Signal (EOD, CMD_OK, MAIN_PROMPT, ...).
        // Ein Signal besteht NUR aus dem 4-Byte-Header, es folgt KEIN Payload
        // (Bareos bsock: Signal = nur die negative Länge auf dem Draht).
        if (headerKind == BnetFrame::HeaderKind::Signal) {
            offset += BnetFrame::HeaderSize;
            handleBnetSignal(messageLength);
            continue;
        }

        // Prüfe ob vollständiges Paket vorhanden (4 Bytes Header + Payload)
        // (computed in 64 bit; the length is already bounded here)
        if (m_receiveBuffer.size() - offset < BnetFrame::telegramSize(messageLength)) {
            break;  // Warte auf mehr Daten
        }

        // Extrahiere Nachricht (ohne Header)
        QByteArray message = m_receiveBuffer.mid(offset + BnetFrame::HeaderSize, messageLength);
        offset += BnetFrame::telegramSize(messageLength);
        message.replace(0x01, ' ');

        // A JSON reply can span many telegrams, cut anywhere, even inside a
        // UTF-8 character. Its bytes are collected unchanged and decoded once
        // the document is complete (or the reply ends with a signal, see
        // handleBnetSignal()); nothing is trimmed at telegram boundaries.
        if (!m_jsonReply.isEmpty() || BJsonReassembler::startsJson(message)) {
            m_jsonReply.append(message);
            QByteArray document;
            while (m_jsonReply.takeDocument(&document)) {
                processDirectorMessage(unbashSpaces(QString::fromUtf8(document)));
            }
            continue;
        }

        // Non-JSON text data — process immediately (one reply per telegram
        // until replies are framed by signal, see P3-1 in the backlog)
        const QString messageStr = QString::fromUtf8(message).trimmed();
#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
        BLOG_DEBUG() << "✓ Text message received:" << messageStr.left(200);
#endif
        if (!messageStr.isEmpty()) {
            processDirectorMessage(unbashSpaces(messageStr));
        }
    }
}

void BareosDirector::handleBnetSignal(qint32 signal)
{
    // Bareos BnetSignal-Werte (bsock.h)
    static const QMap<qint32, const char*> s_signalNames = {
        {-1, "EOD"}, {-2, "EOD_POLL"}, {-3, "STATUS"}, {-4, "TERMINATE"},
        {-5, "POLL"}, {-6, "HEARTBEAT"}, {-7, "HB_RESPONSE"}, {-8, "PROMPT (obsolete)"},
        {-9, "BTIME"}, {-10, "BREAK"}, {-11, "START_SELECT"}, {-12, "END_SELECT"},
        {-13, "INVALID_CMD"}, {-14, "CMD_FAILED"}, {-15, "CMD_OK"}, {-16, "CMD_BEGIN"},
        {-17, "MSGS_PENDING"}, {-18, "MAIN_PROMPT"}, {-19, "SELECT_INPUT"},
        {-20, "WARNING_MSG"}, {-21, "ERROR_MSG"}, {-22, "INFO_MSG"},
        {-23, "RUN_CMD"}, {-24, "YESNO"}, {-25, "START_RTREE"}, {-26, "END_RTREE"},
        {-27, "SUB_PROMPT"}, {-28, "TEXT_INPUT"}
    };

#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
    BLOG_DEBUG() << "✓ BNET signal received:" << signal << s_signalNames.value(signal, "?");
#endif

    // Ein Signal (außer CMD_BEGIN) schließt die laufende Antwort ab
    // (EOD, CMD_OK, CMD_FAILED, MAIN_PROMPT, ...): bis dahin gesammelte
    // JSON-Telegramme sofort verarbeiten, nicht erst beim nächsten Paket.
    if (signal != -16 /* CMD_BEGIN */ && !m_jsonReply.isEmpty()) {
        const QByteArray accumulated = m_jsonReply.takeAll();
        if (!accumulated.isEmpty()) {
            processDirectorMessage(unbashSpaces(QString::fromUtf8(accumulated)));
        }
    }

    if (signal == -4 /* TERMINATE */) {
        BLOG_WARNING() << "Director sent BNET signal TERMINATE";
    } else if (!s_signalNames.contains(signal)) {
        BLOG_WARNING() << "Unknown BNET signal:" << signal;
    }
}

void BareosDirector::processDirectorMessage(const QString &message)
{
    // Ignoriere leere Nachrichten
    if (message.isEmpty()) {
        return;
    }

    // ✅ Consume the JSON confirmation of .api 2 mode switch without dequeuing.
    // The Director sends TWO responses for .api 2: a text status message (processed first,
    // triggers startResourceLoading()) and a JSON-RPC confirmation. The JSON confirmation
    // must NOT dequeue from the command queue (the queue now contains resource commands).
    if (m_consumeApiConfirmation && message.startsWith('{')) {
        m_consumeApiConfirmation = false;
#ifdef IS_DEVELOPER
        BLOG_DEBUG() << "Consuming .api 2 JSON confirmation (not dequeuing from command queue)";
#endif
        return;
    }

    // ---- Command Queue: dequeue the matching command for this response ----
    // (BNET signals never get here, see handleBnetSignal())
    CommandEntry currentEntry;
    const bool hasQueuedCommand = dequeueCurrentCommand(currentEntry);
    if (hasQueuedCommand && currentEntry.status == CommandEntry::TimedOut) {
        // Already reported as failed; the late reply belongs to nobody
        // and must not be taken for the reply of the next command
        BLOG_WARNING() << "Dropping the late reply to command"
                       << static_cast<int>(currentEntry.cmd);
        return;
    }
    if (hasQueuedCommand) {
        m_lastCommandEnum = currentEntry.cmd;
        m_lastCommand = commandToString(currentEntry.cmd, currentEntry.args);
    }

    // The job history leaves here, through its own signal (see
    // jobHistoryResult()). It is matched like every reply: by the command
    // taken from the queue, never by what the reply looks like.
    if (hasQueuedCommand && currentEntry.cmd == Command::LlistJobs) {
        finishJobHistory(currentEntry, message);
        return;
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
        if (hasQueuedCommand && currentEntry.cmd == Command::ListJobId) {
            // The reply itself does not name its job; the queued command does
            emit jobLogResult(currentEntry.args.trimmed().section(' ', 0, 0).toULongLong(), message);
        }
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
            if (hasQueuedCommand && currentEntry.cmd == Command::ListJobId) {
                emit jobLogResult(currentEntry.args.trimmed().section(' ', 0, 0).toULongLong(), jsonPart);
            }
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

            // configure without API mode answers in plain text
            if (hasQueuedCommand && currentEntry.cmd == Command::Configure) {
                emitConfigureResult(message);
            }

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
}

void BareosDirector::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    QString errorMsg = QString("Socket error: %1").arg(m_socket->errorString());

    BLOG_ERROR() << errorMsg;
    setState(Disconnected);
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

    setState(Authenticating);

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

    // Certificate TLS (non-PSK): CA, client certificate and the names the
    // Director's certificate may carry. Windows profiles keep the client
    // certificate in a PKCS#12 file instead of PEM certificate and key.
    if (m_tlsConfig->tlsEnable && !m_tlsConfig->tlsPSKEnable) {
        QString caPath = m_tlsConfig->tlsCaCertFile ? m_tlsConfig->tlsCaCertFile->fileName() : QString();
#ifdef Q_OS_WINDOWS
        QString certPath;
        QString keyPath;
        const QString pfxPath = m_tlsConfig->tlsPfxFile ? m_tlsConfig->tlsPfxFile->fileName() : QString();
        if (!pfxPath.isEmpty())
            m_auth->setPkcs12File(pfxPath, m_tlsConfig->tlsPfxPassword);
#else
        QString certPath = m_tlsConfig->tlsCertFile ? m_tlsConfig->tlsCertFile->fileName() : QString();
        QString keyPath = m_tlsConfig->tlsKeyFile ? m_tlsConfig->tlsKeyFile->fileName() : QString();
#endif

        BLOG_DEBUG() << "Setting certificate files for TLS-Certificate mode:";
        BLOG_DEBUG() << "  CA:" << caPath;
        BLOG_DEBUG() << "  Cert:" << certPath;
        BLOG_DEBUG() << "  Key:" << keyPath;

        m_auth->setCertificateFiles(caPath, certPath, keyPath);
        m_auth->setAllowedCommonNames(m_tlsConfig->tlsAllowedCn);
    }

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

    // TLS was asked for (profile or --tls): never continue in clear text.
    // With TLS enabled the handshake comes before the Hello, so this cannot
    // happen today; the check keeps it that way.
    if (m_tlsConfig->tlsEnable && !m_socket->isEncrypted()) {
        onAuthenticationFailed(tr("TLS is enabled for this connection, but the session with the "
                                  "Director is not encrypted. The connection was closed."));
        return;
    }

    m_directorVersion = directorVersion;

    // What the Director said about itself in its greeting, for the capability
    // table. Nothing is sent for it. Emitted before authentificationSucceeded()
    // so that the description is there when the connection is announced.
    const QString banner = m_auth->directorBanner();
    emit serverIdentified(banner.isEmpty() ? directorVersion : banner);

    // Update Director name from CRAM-MD5 challenge (real name from server)
    QString remoteName = m_auth->remoteDirectorName();
    if (!remoteName.isEmpty() && remoteName != m_directorName) {
        DIR_DEBUG << "Director name updated: " << m_directorName << " -> " << remoteName;
        QMutexLocker infoLocker(&m_infoMutex);
        m_directorName = remoteName;
    }

    // Store TLS cipher list from auth for config export
    {
        QMutexLocker infoLocker(&m_infoMutex);
        m_tlsCipherList = m_auth->tlsCipherList();
    }

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

        // Without a timeout a lost confirmation would leave the GUI in
        // "Setting API mode..." for ever (see onApiModeTimeout())
        if (m_apiModeTimer) {
            m_apiModeTimer->start();
        }

        // Sende .api 2 Befehl für JSON Pretty-Print Modus
        doSend(Command::ApiMode, "2");
    } else {
        // No API mode needed, go directly to LoadingResources
        startResourceLoading();
    }

    // ✅ Emit signals NACH dem API-Modus
    emit authentificationSucceeded(true, directorVersion);

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

    setState(Disconnected);

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
    // Every configure reply (add console, add schedule, ...) is answered with
    // configureResult - also unparsable ones, otherwise the waiting dialog times out
    if (m_lastCommandEnum == Command::Configure) {
        emitConfigureResult(jsonData);
        return;
    }
    // The ACL matrix reads the reply itself, an error reply too
    if (m_lastCommandEnum == Command::DotAclList) {
        emit dotAclListResult(jsonData);
        return;
    }
    if (m_lastCommandEnum == Command::DotAclChange) {
        emit dotAclChangeResult(jsonData);
        return;
    }
    // The cleanup dialog reads the purge result itself, a refusal too
    if (m_lastCommandEnum == Command::PurgeVolumes) {
        emit volumePurgeResult(jsonData);
        return;
    }
    if (m_lastCommandEnum == Command::ShowProfiles) {
        emit showProfilesResult(jsonData);
        return;
    }

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
            if (name.size() >= 2 && name.startsWith('"') && name.endsWith('"'))
                name = BQuote::unescape(name.mid(1, name.size() - 2));   // quoted by commandToString()
            emit showConsoleResult(name, consoles.value(name).toObject());
        } else {
            // "show console" without =name returns all consoles
            for (auto it = consoles.begin(); it != consoles.end(); ++it) {
                emit showConsoleResult(it.key(), it.value().toObject());
            }
        }
        break;
    }

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

void BareosDirector::emitConfigureResult(const QString &reply)
{
    // Bareos "configure add" (API mode 2):
    //   success: {"result":{"configure":{"add":{"resource":..,"name":..,"filename":..}}}}
    //   failure: {"error":{"message":"failed","data":{"messages":{"error":["..."]}}}}
    // Text replies (no API mode / Onesimus Director): "Created resource ..."
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(reply.toUtf8(), &parseError);

    if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
        const QJsonObject root = doc.object();

        if (root.contains("error")) {
            const QJsonObject error = root["error"].toObject();
            QStringList messages;
            const QJsonArray errors = error["data"].toObject()["messages"].toObject()["error"].toArray();
            for (const QJsonValue &v : errors) {
                const QString msg = v.toString().trimmed();
                if (!msg.isEmpty()) {
                    messages << msg;
                }
            }
            if (messages.isEmpty()) {
                messages << error["message"].toString(tr("Unknown error"));
            }
            emit configureResult(false, messages.join('\n'));
            return;
        }

        const QJsonObject result = root["result"].toObject();
        const QJsonObject add = result["configure"].toObject()["add"].toObject();
        const bool ok = add.contains("name") || add.contains("filename")
                        || reply.contains("created", Qt::CaseInsensitive)
                        || reply.contains("success", Qt::CaseInsensitive);
        emit configureResult(ok, reply);
        return;
    }

    // Text reply
    const bool ok = reply.contains("created", Qt::CaseInsensitive)
                    || reply.contains("success", Qt::CaseInsensitive);
    emit configureResult(ok && !detectCommandError(reply), reply.trimmed());
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
    // gRPC (Onesimus Director): login with name + password, no BNET TLS directive
    const QString tls = m_transport == Transport::Grpc ? QString() : QStringLiteral(" tlsenable=false");
    bool nameOk = false, passwordOk = false, profileOk = false;
    const QString command = QString("add console name=%1 password=%2 profile=%3%4")
                                .arg(BQuote::quoted(name, &nameOk), BQuote::quoted(password, &passwordOk),
                                     BQuote::quoted(profile, &profileOk), tls);
    if (!nameOk || !passwordOk || !profileOk) {
        // A newline would end the command and start another one
        emit statusMessage(tr("Not sent: a value contains a control character"));
        return;
    }
    doSend(Command::Configure, command);
}

bool BareosDirector::sendCommand(const QString &command)
{
    if (m_transport == Transport::Grpc) {
        return sendCommandGrpc(command);
    }

    // Allow commands in Ready, SettingApiMode (for .api), and LoadingResources (for dot-commands)
    bool canSend = (m_connectionState == Ready ||
                    m_connectionState == SettingApiMode ||
                    m_connectionState == LoadingResources) &&
                   m_socket->state() == QAbstractSocket::ConnectedState;

    if (!canSend) {
        BLOG_WARNING() << "Cannot send command - not ready (state:" << m_connectionState << ", socket:" << m_socket->state() << ")";
        emit statusMessage(tr("Not ready"));
        return false;
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << ">>> Sending command:" << bRedactCommand(command);
#endif

    const QByteArray packet = commandPacket(command);
    m_lastSentSize = packet.size();

#if defined(IS_DEVELOPER) && defined(DEBUG_PACKETS)
    BLOG_DEBUG() << "  Packet size:" << packet.size() << "bytes (4 header + " << len << " payload)";
#endif

    m_lastCommand = command;

    // Thread-safe: Invoke socket write in the socket's thread
    QMetaObject::invokeMethod(this, "writeToSocket",
                              Qt::QueuedConnection,
                              Q_ARG(QByteArray, packet));
    return true;
}

QByteArray BareosDirector::commandPacket(const QString &command)
{
    QString msg = command;
    if (!msg.endsWith('\n')) {
        msg += '\n';
    }

    // Identical to BareosAuth::send(): signed 32-bit big-endian length
    const QByteArray payload = msg.toUtf8();
    QByteArray packet(4, '\0');
    qToBigEndian<qint32>(qint32(payload.size()), reinterpret_cast<uchar *>(packet.data()));
    packet.append(payload);
    return packet;
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

void BareosDirector::doSend(const Command cmd, const QString &args){
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << ">>> doSend() cmd=" << static_cast<int>(cmd) << " args=" << bRedactCommand(args);
#endif
    // A value that cannot be written in a command line (commandToString()),
    // or a control character that would end it early: nothing is sent
    const QString line = commandToString(cmd, args);
    if (line.isEmpty() || BQuote::hasControlCharacter(line)) {
        BLOG_WARNING() << "Command refused, cmd=" << static_cast<int>(cmd)
                       << " args=" << bRedactCommand(args);
        emit commandFailed(cmd, args,
                           tr("The command was not sent: one of its values cannot be written in a command line"));
        return;
    }
    CommandEntry entry;
    entry.cmd = cmd;
    entry.args = args;
    registerRollback(entry);

    m_commandQueue.enqueue(entry);
    flushQueue();
}

void BareosDirector::flushQueue()
{
    int i = 0;
    while (i < m_commandQueue.size()) {
        if (m_commandQueue[i].status != CommandEntry::Pending) {
            ++i;
            continue;
        }

        const CommandEntry entry = m_commandQueue[i];
        const QString cmdStr = commandToString(entry.cmd, entry.args);

        if (sendCommand(cmdStr)) {
            // Sent only now: an entry marked Sent takes the next reply
            m_commandQueue[i].status = CommandEntry::Sent;
            m_lastCommandEnum = entry.cmd;
            m_lastCommand = cmdStr;
            ++i;
        } else {
            // Refused (not connected, not ready): no reply will come, so the
            // entry must not stay in the queue and shift every later reply
            m_commandQueue.removeAt(i);
            emit commandFailed(entry.cmd, entry.args,
                               tr("The command was not sent: no ready connection to the Director"));
        }
    }
    updateCommandTimer();
}

bool BareosDirector::dequeueCurrentCommand(CommandEntry &entry)
{
    // Find and dequeue the first Sent entry (FIFO — matches Bareos serial processing)
    if (!m_commandQueue.isEmpty()
        && (m_commandQueue.head().status == CommandEntry::Sent
            || m_commandQueue.head().status == CommandEntry::TimedOut)) {
        entry = m_commandQueue.dequeue();
        if (m_commandTimer) {
            m_commandTimer->stop();   // timed afresh for the next one
        }
        updateCommandTimer();
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
    updateCommandTimer();
}

bool BareosDirector::detectCommandError(const QString &response) const
{
    // By the structure of the reply (the JSON-RPC error object, or a Bareos
    // error line in text mode), not by the word "error" somewhere in it: a
    // job log or a file name containing it is not a failed command
    return BDirectorReply::isErrorReply(response);
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
namespace {

/// One or more ASCII digits
bool isDigits(const QString &text)
{
    if (text.isEmpty())
        return false;
    for (const QChar c : text) {
        if (c.unicode() < '0' || c.unicode() > '9')
            return false;
    }
    return true;
}

/// true when @p args may follow the command word of @p cmd
bool argumentsAccepted(BareosDirector::Command cmd, const QString &args)
{
    using Command = BareosDirector::Command;
    switch (cmd) {
    // Numbers only: job ids and limits are never quoted
    case Command::ListJobs: {
        if (args.isEmpty())
            return true;
        const QStringList parts = args.split(QLatin1Char(','));
        return parts.size() <= 2 && isDigits(parts.at(0)) && (parts.size() == 1 || isDigits(parts.at(1)));
    }
    case Command::ListJobsLast:
        return args.isEmpty() || isDigits(args);
    case Command::ListJobId:
    case Command::ListFiles:
    case Command::Cancel:
    case Command::DeleteJob:
        return isDigits(args);
    case Command::Rerun:
        return isDigits(args)
            || (args.endsWith(QLatin1String(" yes")) && isDigits(args.chopped(4)));
    case Command::ApiMode:
        // "", a number or a mode word (on, off, json, json_pretty)
        for (const QChar c : args) {
            if (!(c.isDigit() || (c >= QLatin1Char('a') && c <= QLatin1Char('z')) || c == QLatin1Char('_')))
                return false;
        }
        return true;
    case Command::Trace:
        return args.isEmpty() || args == QLatin1String("on") || args == QLatin1String("off");

    // Argument strings built by the caller: values must be quoted
    case Command::Restore:
    case Command::Label:
    case Command::Relabel:
    case Command::Show:
    case Command::Estimate:
    case Command::Configure:
    case Command::Export:
    case Command::Import:
    case Command::BvfsGetJobIds:
    case Command::BvfsUpdate:
    case Command::BvfsLsDirs:
    case Command::BvfsLsFiles:
    case Command::BvfsRestore:
    case Command::BvfsCleanup:
        return BQuote::isWellFormedArguments(args);

    default:
        // Resource names are quoted by commandToString(); Custom, SqlQuery,
        // Query, DotAclChange and PurgeVolumes are whole lines (checked for
        // control characters by doSend())
        return true;
    }
}

} // namespace

QString BareosDirector::commandToString(Command cmd, const QString &args)
{
    if (!argumentsAccepted(cmd, args))
        return QString();

    QString command;
    // A resource name as the value of one keyword: always quoted, so that a
    // blank, a quote or "yes" in a name stays part of it (BQuote)
    const QString name = args.isEmpty() ? args : BQuote::quoted(args);

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
    case Command::StatusClient:     command = QString("status client=%1").arg(name); break;
    case Command::StatusStorage:    command = QString("status storage=%1").arg(name); break;
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
    case Command::ListVolumePool:   command = QString("list volumes pool=%1").arg(name); break;
    case Command::ListMedia:        command = "list media"; break;
    case Command::ListFileSets:     command = "list filesets"; break;
    case Command::ListFiles:        command = QString("list files jobid=%1").arg(args); break;
    case Command::ListBackups:      command = "list backups"; break;
    case Command::ListBackupsClient: command = QString("list backups client=%1").arg(name); break;
    case Command::ListJobTotals:    command = "list jobtotals"; break;

    // Job Control
    case Command::Run:              command = QString("run job=%1").arg(name); break;
    case Command::RunYes:           command = QString("run job=%1 yes").arg(name); break;
    case Command::Cancel:           command = QString("cancel jobid=%1").arg(args); break;
    case Command::DeleteJob:        command = QString("delete job jobid=%1 yes").arg(args); break;
    case Command::DeleteFileSet:    command = QString("delete fileset=%1 yes").arg(name); break;
    case Command::Disable:          command = QString("disable job=%1").arg(name); break;
    case Command::Enable:           command = QString("enable job=%1").arg(name); break;
    case Command::Rerun:            command = QString("rerun jobid=%1").arg(args); break;

    // Restore
    case Command::Restore:          command = QString("restore %1").arg(args); break;
    case Command::RestoreAll:       command = "restore all"; break;
    case Command::RestoreSelect:    command = "restore select"; break;

    // Volume Management
    case Command::Label:            command = QString("label %1").arg(args); break;
    case Command::Relabel:          command = QString("relabel %1").arg(args); break;
    case Command::Mount:            command = QString("mount storage=%1").arg(name); break;
    case Command::Unmount:          command = QString("unmount storage=%1").arg(name); break;
    case Command::Release:          command = QString("release storage=%1").arg(name); break;
    case Command::Update:           command = "update"; break;
    case Command::UpdateVolume:     command = QString("update volume=%1").arg(name); break;
    case Command::Purge:            command = QString("purge volume=%1").arg(name); break;
    case Command::Prune:            command = "prune"; break;
    case Command::PruneFiles:       command = "prune files"; break;
    case Command::PruneJobs:        command = "prune jobs"; break;
    case Command::PruneVolume:      command = QString("prune volume=%1").arg(name); break;

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
    case Command::DotDefaults:      command = ".defaults job=" + BQuote::quoted(args); break;
    case Command::DotConsoles:      command = ".consoles"; break;
    case Command::DotMessages:      command = ".messages"; break;
    case Command::DotAclList:       command = ".acl list"; break;
    case Command::DotAclChange:     command = args; break;
    case Command::ShowProfiles:     command = "show profiles"; break;

    // Show single resource
    case Command::ShowFileset:      command = QString("show fileset=%1").arg(name); break;
    case Command::ShowClient:       command = QString("show client=%1").arg(name); break;
    case Command::ShowConsole:      command = QString("show console=%1").arg(name); break;

    // Volume Maintenance (bulk)
    // Documented form: prune volume [=volume] [pool=<pool>] [all] [yes]
    // ("allpools" is not a prune keyword; without "all" the Director asks for a volume)
    case Command::PruneVolumeAll:   command = "prune volume all yes"; break;
    // Onesimus Director only: "purge volume all pool=<p>|allpools
    // volstatus=... dryrun|yes", built and checked by BVolumePurge
    case Command::PurgeVolumes:     command = args; break;

    // BVFS (Virtual File System for restore)
    case Command::BvfsGetJobIds:    command = QString(".bvfs_get_jobids %1").arg(args); break;
    case Command::BvfsUpdate:       command = QString(".bvfs_update %1").arg(args); break;
    case Command::BvfsLsDirs:       command = QString(".bvfs_lsdirs %1").arg(args); break;
    case Command::BvfsLsFiles:      command = QString(".bvfs_lsfiles %1").arg(args); break;
    case Command::BvfsRestore:      command = QString(".bvfs_restore %1").arg(args); break;
    case Command::BvfsCleanup:      command = QString(".bvfs_cleanup %1").arg(args); break;

    // Job history
    case Command::LlistJobs:        command = jobHistoryCommandLine(args); break;

    // Volumes with all catalog fields. Never "list nextvol": on Bareos it
    // runs the volume search with pruning (dird/ua_output.cc), which can
    // delete jobs and change volume states.
    case Command::LlistVolumes:     command = "llist volumes"; break;

    // JobId and volume name of every JobMedia record (dird/ua_output.cc:
    // "list jobmedia" without jobid= lists all, cats/sql_list.cc
    // ListJobmediaRecords; the Onesimus Director does the same)
    case Command::ListJobMedia:     command = "list jobmedia"; break;

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
    if (s == "OK") return Successful;  // Onesimus Director push events
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



// ============================================================================
// gRPC Transport (Onesimus Director)
// ============================================================================
//
// The gRPC path reuses the BNET state machine and response processing:
//   connect()  -> connectGrpc()            (Connecting)
//   login OK   -> onGrpcLoggedIn()         (Authenticating -> LoadingResources)
//   doSend()   -> flushQueue() -> sendCommand() -> sendCommandGrpc()
//   reply      -> onGrpcCommandFinished() -> processDirectorMessage()
// JSON output is requested per call (response_format), so there is no
// SettingApiMode phase.

void BareosDirector::setTransport(Transport transport, int grpcPort,
                                  const QString &grpcCertFingerprint)
{
    m_transport = transport;
    m_grpcPort = grpcPort;
    m_grpcCertFingerprint = grpcCertFingerprint;
}

void BareosDirector::connectGrpc()
{
#ifdef HAVE_GRPC
    const int grpcPort = m_grpcPort > 0 ? m_grpcPort : m_port + 1000;
    DIR_DEBUG << "Connecting via gRPC to" << m_host << ":" << grpcPort;

    m_commandQueue.clear();
    m_jsonReply.clear();
    m_receiveBuffer.clear();
    m_consumeApiConfirmation = false;

    setState(Authenticating);
    m_grpc->open(m_host, grpcPort, m_consoleName, m_password, m_grpcCertFingerprint);
#else
    onGrpcFailed(tr("This build of Onesimus has no gRPC support"));
#endif
}

bool BareosDirector::sendCommandGrpc(const QString &command)
{
#ifdef HAVE_GRPC
    const bool canSend = (m_connectionState == Ready ||
                          m_connectionState == LoadingResources) && m_grpc->isLoggedIn();
    if (!canSend) {
        BLOG_WARNING() << "Cannot send command via gRPC - not ready (state:" << m_connectionState << ")";
        emit statusMessage(tr("Not ready"));
        return false;
    }
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << ">>> Sending command (gRPC):" << bRedactCommand(command);
#endif
    m_lastCommand = command;
    m_grpc->execute(command);
    return true;
#else
    Q_UNUSED(command)
    return false;
#endif
}

void BareosDirector::onGrpcLoggedIn(const QString &directorName, const QString &version)
{
    DIR_DEBUG << "gRPC: authenticated, Director" << directorName << "version" << version;

    if (!directorName.isEmpty() && directorName != m_directorName) {
        DIR_DEBUG << "Director name updated: " << m_directorName << " -> " << directorName;
        QMutexLocker infoLocker(&m_infoMutex);
        m_directorName = directorName;
    }
    m_directorVersion = version;
    {
        QMutexLocker infoLocker(&m_infoMutex);
        m_tlsCipherList.clear();
    }

    {
        QMutexLocker stateLocker(&m_stateMutex);
        m_connected = true;
    }

    resetResourceFlags();
    startResourceLoading();

    // The version field of the login response (see onAuthenticationSucceeded())
    emit serverIdentified(version);
    emit authentificationSucceeded(true, version);

    {
        QMutexLocker authLocker(&m_authMutex);
        m_authCompleted = true;
        m_authCondition.wakeAll();
    }
}

void BareosDirector::onGrpcFailed(const QString &reason)
{
    const bool wasConnected = (m_connectionState == Ready || m_connectionState == LoadingResources);
    m_commandQueue.clear();
    setState(Disconnected);
    {
        QMutexLocker stateLocker(&m_stateMutex);
        m_connected = false;
    }

    if (wasConnected) {
        emit disconnected();
    } else {
        emit authentificationSucceeded(false, reason);
    }
    emit protocolError(reason);

    QMutexLocker authLocker(&m_authMutex);
    m_authCompleted = true;
    m_authCondition.wakeAll();
}

void BareosDirector::onGrpcCertificateNotTrusted(const QString &fingerprint,
                                                 const QString &pinnedFingerprint)
{
    const int grpcPort = m_grpcPort > 0 ? m_grpcPort : m_port + 1000;
    setState(Disconnected);
    {
        QMutexLocker stateLocker(&m_stateMutex);
        m_connected = false;
    }

    // No protocolError here: the receiver asks the user whether to trust it.
    // Certificate signal first, so receivers can ignore the auth failure that follows.
    emit grpcCertificateNotTrusted(m_host, grpcPort, fingerprint, pinnedFingerprint);
    emit authentificationSucceeded(false, tr("Server certificate not trusted"));

    QMutexLocker authLocker(&m_authMutex);
    m_authCompleted = true;
    m_authCondition.wakeAll();
}

void BareosDirector::onGrpcCommandFinished(const QString &commandLine, bool success,
                                           const QString &text, const QString &error)
{
    QString message = text.trimmed();

    if (!success) {
        // Same shape as a JSON-RPC error of the BNET API mode, so that
        // detectCommandError()/commandFailed() behave identically
        BLOG_WARNING() << "gRPC command failed:" << bRedactCommand(commandLine) << "-" << error;
        QJsonObject err{{"code", 1}, {"message", error.isEmpty() ? tr("Command failed") : error}};
        message = QString::fromUtf8(QJsonDocument(QJsonObject{
            {"jsonrpc", "2.0"}, {"id", QJsonValue::Null}, {"error", err}}).toJson(QJsonDocument::Compact));
        emit commandError(commandLine, error);
    } else if (message.isEmpty()) {
        // Every reply must dequeue its command
        message = QStringLiteral(R"({"jsonrpc":"2.0","id":null,"result":{}})");
    }

    processDirectorMessage(message);
}

void BareosDirector::onGrpcJobStatus(int jobId, const QString &jobName, const QString &status,
                                     qint64 timestamp, qint64 schedTime)
{
    // Unix times of the Director; 0 is "not known"
    auto time = [](qint64 secs) {
        return secs > 0 ? QDateTime::fromSecsSinceEpoch(secs).toLocalTime() : QDateTime();
    };
    emit jobStatusChanged(jobId, parseJobStatus(status));
    emit jobStatusEvent(quint64(jobId), jobName, status, time(timestamp), time(schedTime));
}

void BareosDirector::onGrpcConnectionLost(const QString &reason)
{
    BLOG_WARNING() << "gRPC connection lost:" << reason;
    m_commandQueue.clear();
    setState(Disconnected);
    {
        QMutexLocker stateLocker(&m_stateMutex);
        m_connected = false;
    }
    emit disconnected();
    emit protocolError(tr("Connection to Director lost: %1").arg(reason));
}
