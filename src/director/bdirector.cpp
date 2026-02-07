/**
 * @file bdirector.cpp
 * @brief Implementation of BDirector wrapper class
 *
 * This wrapper runs in the MainThread and forwards all calls thread-safely
 * to the director instance (BareosDirector or BaculaDirector) which runs
 * in its own worker thread.
 *
 * The backend is selected at compile time via preprocessor defines:
 * - USE_BACULA_ONLY: Uses BaculaDirector (not yet implemented)
 * - USE_BAREOS_ONLY: Uses BareosDirector (default)
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 * @version 1.0.0
 */

#include "director/bdirector.h"
#include "blogging.h"

// ============================================================================
// Constructor / Destructor
// ============================================================================

BDirector::BDirector(QObject *parent)
    : QObject(parent)
    , m_director(nullptr)
    , m_workerThread(nullptr)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BDirector: Creating wrapper (MainThread)";
    BLOG_DEBUG() << "  MainThread ID:" << QThread::currentThreadId();
#endif

    // Create worker thread
    m_workerThread = new QThread(this);
    m_workerThread->setObjectName("BareosWorkerThread");

    // Create director instance (BareosDirector or BaculaDirector based on compile-time selection)
    m_director = new DIRECTOR_CLASS();

    // Move director to worker thread BEFORE connecting signals
    // This ensures the director lives in the worker thread
    m_director->moveToThread(m_workerThread);

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "  Director moved to worker thread";
#endif

    // Forward all signals from director to BDirector wrapper
    // Qt automatically handles thread-safety for signal forwarding (signals are queued connections)

    QObject::connect(m_director, &DIRECTOR_CLASS::disconnected,
                     this, &BDirector::disconnected);

    QObject::connect(m_director, &DIRECTOR_CLASS::authentificationSucceeded,
                     this, &BDirector::authentificationSucceeded);

    // Emit connected() when authentication succeeds
    QObject::connect(m_director, &DIRECTOR_CLASS::authentificationSucceeded,
                     this, [this](bool success, const QString &) {
                         if (success) {
                             emit connected();
                         }
                     });

    QObject::connect(m_director, &DIRECTOR_CLASS::protocolError,
                     this, &BDirector::protocolError);

    QObject::connect(m_director, &DIRECTOR_CLASS::statusMessage,
                     this, &BDirector::statusMessage);

    QObject::connect(m_director, &DIRECTOR_CLASS::jsonResponse,
                     this, &BDirector::jsonResponse);

    QObject::connect(m_director, &DIRECTOR_CLASS::commandResponse,
                     this, &BDirector::commandResponse);

    QObject::connect(m_director, &DIRECTOR_CLASS::commandError,
                     this, &BDirector::commandError);

    // Forward state machine signals
    QObject::connect(m_director, &DIRECTOR_CLASS::connectionStateChanged,
                     this, [this](DIRECTOR_CLASS::ConnectionState oldState,
                                   DIRECTOR_CLASS::ConnectionState newState) {
                         emit connectionStateChanged(oldState, newState);
                     });

    QObject::connect(m_director, &DIRECTOR_CLASS::resourceStateChanged,
                     this, [this](DIRECTOR_CLASS::ResourceType type,
                                   DIRECTOR_CLASS::ResourceLoadState state) {
                         emit resourceStateChanged(type, state);
                     });

    QObject::connect(m_director, &DIRECTOR_CLASS::resourceLoaded,
                     this, [this](DIRECTOR_CLASS::ResourceType type) {
                         emit resourceLoaded(type);
                     });

    QObject::connect(m_director, &DIRECTOR_CLASS::resourceLoadFailed,
                     this, [this](DIRECTOR_CLASS::ResourceType type, const QString &error) {
                         emit resourceLoadFailed(type, error);
                     });

    QObject::connect(m_director, &DIRECTOR_CLASS::allResourcesLoaded,
                     this, &BDirector::allResourcesLoaded);

    QObject::connect(m_director, &DIRECTOR_CLASS::resourceLoadProgress,
                     this, &BDirector::resourceLoadProgress);

    // Typed query result signals
    QObject::connect(m_director, &DIRECTOR_CLASS::consolesResult,
                     this, &BDirector::consolesResult);
    QObject::connect(m_director, &DIRECTOR_CLASS::showConsoleResult,
                     this, &BDirector::showConsoleResult);
    QObject::connect(m_director, &DIRECTOR_CLASS::configureResult,
                     this, &BDirector::configureResult);

    // Initialize director in worker thread (creates socket and auth)
    // Use QueuedConnection to ensure it runs in worker thread
    QObject::connect(m_workerThread, &QThread::started,
                     m_director, &DIRECTOR_CLASS::initialize);

    // Start the worker thread
    m_workerThread->start();

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BDirector: Wrapper created, worker thread started";
#endif
}

BDirector::~BDirector()
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BDirector: Destructor - stopping worker thread";
#endif

    if (m_workerThread) {
        // Stop thread gracefully
        m_workerThread->quit();
        m_workerThread->wait(3000);

        if (m_workerThread->isRunning()) {
            BLOG_WARNING() << "BDirector: Worker thread still running, terminating...";
            m_workerThread->terminate();
            m_workerThread->wait();
        }

#ifdef IS_DEVELOPER
        BLOG_DEBUG() << "BDirector: Worker thread stopped";
#endif
    }

    // Director will be deleted automatically when thread is deleted
    // because it's parented to the thread via moveToThread
    if (m_director) {
        m_director->deleteLater();
        m_director = nullptr;
    }

    // Thread is parented to this, so it will be deleted automatically
    // but we can delete it explicitly for clarity
    if (m_workerThread) {
        delete m_workerThread;
        m_workerThread = nullptr;
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BDirector: Destructor complete";
#endif
}

// ============================================================================
// Information Methods
// ============================================================================

QString BDirector::backupSystemName() const
{
    if (m_director) {
        return m_director->backupSystemName();
    }
    return QString();
}

bool BDirector::isConnected() const
{
    if (m_director) {
        return m_director->isConnected();
    }
    return false;
}

BDirector::ConnectionState BDirector::connectionState() const
{
    if (m_director) {
        return m_director->connectionState();
    }
    return DIRECTOR_CLASS::Disconnected;
}

QString BDirector::currentHost() const
{
    if (m_director) {
        return m_director->currentHost();
    }
    return QString();
}

int BDirector::currentPort() const
{
    if (m_director) {
        return m_director->currentPort();
    }
    return 0;
}

QString BDirector::currentDirectorName() const
{
    if (m_director) {
        return m_director->currentDirectorName();
    }
    return QString();
}

QString BDirector::tlsCipherList() const
{
    if (m_director) {
        return m_director->tlsCipherList();
    }
    return QString();
}

BDirector::ApiMode BDirector::apiMode() const
{
    if (m_director) {
        return m_director->apiMode();
    }
    return DIRECTOR_CLASS::ApiMode::Off;
}

BDirector::TLSConfig *BDirector::tlsConfig() const
{
    if (m_director) {
        return m_director->tlsConfig();
    }
    return nullptr;
}

// ============================================================================
// Connection Management (thread-safe forwarding)
// ============================================================================

void BDirector::connect(const QString &host, int port, const QString &directorName,
                       const QString &consoleName, const QString &password)
{
    if (!m_director) {
        BLOG_ERROR() << "BDirector::connect: No director instance!";
        return;
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BDirector::connect: Forwarding to director thread";
#endif

    // Thread-safe: Use QMetaObject::invokeMethod with Qt::QueuedConnection
    QMetaObject::invokeMethod(m_director, "connect",
                              Qt::QueuedConnection,
                              Q_ARG(QString, host),
                              Q_ARG(int, port),
                              Q_ARG(QString, directorName),
                              Q_ARG(QString, consoleName),
                              Q_ARG(QString, password));
}

void BDirector::disconnect()
{
    if (!m_director) {
        BLOG_ERROR() << "BDirector::disconnect: No director instance!";
        return;
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BDirector::disconnect: Forwarding to director thread";
#endif

    // Thread-safe: Use QMetaObject::invokeMethod
    QMetaObject::invokeMethod(m_director, "disconnect",
                              Qt::QueuedConnection);
}

void BDirector::setTLSConfig(const TLSConfig &config)
{
    if (!m_director) {
        BLOG_ERROR() << "BDirector::setTLSConfig: No director instance!";
        return;
    }

    // This is safe to call directly as it only sets member variables
    m_director->setTLSConfig(config);
}

void BDirector::setApiMode(ApiMode mode)
{
    if (!m_director) {
        BLOG_ERROR() << "BDirector::setApiMode: No director instance!";
        return;
    }

    // Thread-safe: Use QMetaObject::invokeMethod
    QMetaObject::invokeMethod(m_director, "setApiMode",
                              Qt::QueuedConnection,
                              Q_ARG(DIRECTOR_CLASS::ApiMode, mode));
}

// ============================================================================
// Command Sending (thread-safe forwarding)
// ============================================================================

void BDirector::doSendCommand(const Command cmd, const QString &args)
{
    if (!m_director) {
        BLOG_ERROR() << "BDirector::doSendCommand: No director instance!";
        return;
    }

    // Thread-safe: Use QMetaObject::invokeMethod
    QMetaObject::invokeMethod(m_director, "doSendCommand",
                              Qt::QueuedConnection,
                              Q_ARG(DIRECTOR_CLASS::Command, cmd),
                              Q_ARG(QString, args));
}

void BDirector::sendCommand(const QString &command)
{
    if (!m_director) {
        BLOG_ERROR() << "BDirector::sendCommand: No director instance!";
        return;
    }

    // Thread-safe: Use QMetaObject::invokeMethod with public slot doSendCommand
    QMetaObject::invokeMethod(m_director, "doSendCommand",
                              Qt::QueuedConnection,
                              Q_ARG(BareosDirector::Command, DIRECTOR_CLASS::Command::Custom),
                              Q_ARG(QString, command));
}

// ============================================================================
// Query Convenience Methods (thread-safe)
// ============================================================================

void BDirector::queryConsoles()
{
    if (!m_director) return;
    QMetaObject::invokeMethod(m_director, "queryConsoles", Qt::QueuedConnection);
}

void BDirector::queryShowConsole(const QString &name)
{
    if (!m_director) return;
    QMetaObject::invokeMethod(m_director, "queryShowConsole", Qt::QueuedConnection,
                              Q_ARG(QString, name));
}

void BDirector::queryConfigureAddConsole(const QString &name, const QString &password,
                                          const QString &profile)
{
    if (!m_director) return;
    QMetaObject::invokeMethod(m_director, "queryConfigureAddConsole", Qt::QueuedConnection,
                              Q_ARG(QString, name), Q_ARG(QString, password),
                              Q_ARG(QString, profile));
}

// ============================================================================
// Helper Methods (static - can be called directly)
// ============================================================================

BDirector::JobStatus BDirector::parseJobStatus(const QString &status)
{
    return DIRECTOR_CLASS::parseJobStatus(status);
}

QString BDirector::jobStatusToString(JobStatus status)
{
    return DIRECTOR_CLASS::jobStatusToString(status);
}

// ============================================================================
// Resource State Machine (thread-safe forwarding)
// ============================================================================

BDirector::ResourceLoadState BDirector::resourceState(ResourceType type) const
{
    if (m_director) {
        return m_director->resourceState(type);
    }
    return DIRECTOR_CLASS::ResourceLoadState::Initial;
}

bool BDirector::isResourceLoaded(ResourceType type) const
{
    if (m_director) {
        return m_director->isResourceLoaded(type);
    }
    return false;
}

bool BDirector::areAllResourcesLoaded() const
{
    if (m_director) {
        return m_director->areAllResourcesLoaded();
    }
    return false;
}

void BDirector::reloadResource(ResourceType type)
{
    if (!m_director) {
        BLOG_ERROR() << "BDirector::reloadResource: No director instance!";
        return;
    }

    // Thread-safe: Use QMetaObject::invokeMethod
    QMetaObject::invokeMethod(m_director, "reloadResource",
                              Qt::QueuedConnection,
                              Q_ARG(DIRECTOR_CLASS::ResourceType, type));
}
