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

#include "bdirector.h"
#include <QDebug>

// ============================================================================
// Constructor / Destructor
// ============================================================================

BDirector::BDirector(QObject *parent)
    : QObject(parent)
    , m_director(nullptr)
    , m_workerThread(nullptr)
{
#ifdef IS_DEVELOPER
    qDebug() << "BDirector: Creating wrapper (MainThread)";
    qDebug() << "  MainThread ID:" << QThread::currentThreadId();
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
    qDebug() << "  Director moved to worker thread";
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

    // Initialize director in worker thread (creates socket and auth)
    // Use QueuedConnection to ensure it runs in worker thread
    QObject::connect(m_workerThread, &QThread::started,
                     m_director, &DIRECTOR_CLASS::initialize);

    // Start the worker thread
    m_workerThread->start();

#ifdef IS_DEVELOPER
    qDebug() << "BDirector: Wrapper created, worker thread started";
#endif
}

BDirector::~BDirector()
{
#ifdef IS_DEVELOPER
    qDebug() << "BDirector: Destructor - stopping worker thread";
#endif

    if (m_workerThread) {
        // Stop thread gracefully
        m_workerThread->quit();
        m_workerThread->wait(3000);

        if (m_workerThread->isRunning()) {
            qWarning() << "BDirector: Worker thread still running, terminating...";
            m_workerThread->terminate();
            m_workerThread->wait();
        }

#ifdef IS_DEVELOPER
        qDebug() << "BDirector: Worker thread stopped";
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
    qDebug() << "BDirector: Destructor complete";
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

bool BDirector::hasStoredConnection() const
{
    if (m_director) {
        return m_director->hasStoredConnection();
    }
    return false;
}

// ============================================================================
// Connection Management (thread-safe forwarding)
// ============================================================================

void BDirector::connect(const QString &host, int port, const QString &directorName,
                       const QString &password)
{
    if (!m_director) {
        qCritical() << "BDirector::connect: No director instance!";
        return;
    }

#ifdef IS_DEVELOPER
    qDebug() << "BDirector::connect: Forwarding to director thread";
#endif

    // Thread-safe: Use QMetaObject::invokeMethod with Qt::QueuedConnection
    QMetaObject::invokeMethod(m_director, "connect",
                              Qt::QueuedConnection,
                              Q_ARG(QString, host),
                              Q_ARG(int, port),
                              Q_ARG(QString, directorName),
                              Q_ARG(QString, password));
}

void BDirector::disconnect()
{
    if (!m_director) {
        qCritical() << "BDirector::disconnect: No director instance!";
        return;
    }

#ifdef IS_DEVELOPER
    qDebug() << "BDirector::disconnect: Forwarding to director thread";
#endif

    // Thread-safe: Use QMetaObject::invokeMethod
    QMetaObject::invokeMethod(m_director, "disconnect",
                              Qt::QueuedConnection);
}

void BDirector::setTLSConfig(const TLSConfig &config)
{
    if (!m_director) {
        qCritical() << "BDirector::setTLSConfig: No director instance!";
        return;
    }

    // This is safe to call directly as it only sets member variables
    m_director->setTLSConfig(config);
}

void BDirector::setApiMode(ApiMode mode)
{
    if (!m_director) {
        qCritical() << "BDirector::setApiMode: No director instance!";
        return;
    }

    // Thread-safe: Use QMetaObject::invokeMethod
    QMetaObject::invokeMethod(m_director, "setApiMode",
                              Qt::QueuedConnection,
                              Q_ARG(DIRECTOR_CLASS::ApiMode, mode));
}

void BDirector::loadConnectionSettings()
{
    if (!m_director) {
        qCritical() << "BDirector::loadConnectionSettings: No director instance!";
        return;
    }

    // Thread-safe: Use QMetaObject::invokeMethod
    QMetaObject::invokeMethod(m_director, "loadConnectionSettings",
                              Qt::QueuedConnection);
}

void BDirector::saveConnectionSettings()
{
    if (!m_director) {
        qCritical() << "BDirector::saveConnectionSettings: No director instance!";
        return;
    }

    // Thread-safe: Use QMetaObject::invokeMethod
    QMetaObject::invokeMethod(m_director, "saveConnectionSettings",
                              Qt::QueuedConnection);
}

// ============================================================================
// Command Sending (thread-safe forwarding)
// ============================================================================

void BDirector::doSendCommand(const Command cmd, const QString &args)
{
    if (!m_director) {
        qCritical() << "BDirector::doSendCommand: No director instance!";
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
        qCritical() << "BDirector::sendCommand: No director instance!";
        return;
    }

    // Thread-safe: Use QMetaObject::invokeMethod
    QMetaObject::invokeMethod(m_director, "sendCommand",
                              Qt::QueuedConnection,
                              Q_ARG(QString, command));
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
