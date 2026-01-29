#ifndef BDIRECTOR_H
#define BDIRECTOR_H

#include <QObject>
#include <QThread>

// ============================================================================
// Backend-Auswahl über Präprozessor
// ============================================================================
// USE_BACULA_ONLY  -> Bacula Backend (BaculaDirector)
// USE_BAREOS_ONLY  -> Bareos Backend (BareosDirector)
// Wenn keines definiert ist, wird standardmäßig Bareos verwendet
// ============================================================================

#ifdef USE_BACULA_ONLY
// TODO: Implement BaculaDirector
// #include "baculadirector.h"
// #define DIRECTOR_CLASS BaculaDirector
#error "Bacula backend not yet implemented. Please use Bareos (default) or implement BaculaDirector first."
#elif defined(USE_BAREOS_ONLY)
#include "bareosdirector.h"
#define DIRECTOR_CLASS BareosDirector
#else
// Default: Bareos
#include "bareosdirector.h"
#define DIRECTOR_CLASS BareosDirector
#define USE_BAREOS_ONLY
#endif

/**
 * @file bdirector.h
 * @brief Wrapper class for Bareos/Bacula Director communication
 *
 * This class provides a thread-safe wrapper around the actual director implementations.
 * It runs in the main thread and forwards all calls to the actual director instance
 * which runs in its own worker thread.
 *
 * The backend (Bacula or Bareos) is determined at compile time via:
 * - USE_BACULA_ONLY: Uses BaculaDirector (not yet implemented)
 * - USE_BAREOS_ONLY: Uses BareosDirector (default)
 *
 * Architecture:
 * - BDirector: Wrapper (runs in MainThread)
 * - BareosDirector/BaculaDirector: Implementation (runs in WorkerThread)
 * - Auth: Authentication (runs in WorkerThread)
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 * @version 1.0.0
 */

/**
 * @class BDirector
 * @brief Thread-safe wrapper for Director communication
 *
 * This class wraps the actual director implementation and provides thread-safe
 * access from the main thread. All method calls are forwarded to the director
 * instance running in a worker thread.
 *
 * Usage Example:
 * @code
 * BDirector *director = new BDirector(this);
 *
 * // Connect signals
 * connect(director, &BDirector::connected, this, &MyClass::onConnected);
 * connect(director, &BDirector::jsonResponse, this, &MyClass::onResponse);
 *
 * // Connect to Director
 * director->connect("192.168.1.10", 9101, "bareos-dir", "mypassword");
 *
 * // Send commands
 * director->doSendCommand(BDirector::Command::ListJobs);
 * @endcode
 */
class BDirector : public QObject
{
    Q_OBJECT

public:
    // Re-export types from the actual director implementation (BareosDirector or BaculaDirector)
    using ConnectionState = DIRECTOR_CLASS::ConnectionState;
    using ApiMode = DIRECTOR_CLASS::ApiMode;
    using Command = DIRECTOR_CLASS::Command;
    using JobStatus = DIRECTOR_CLASS::JobStatus;
    using TLSConfig = DIRECTOR_CLASS::TLSConfig;
    using VolumeInfo = DIRECTOR_CLASS::VolumeInfo;
    using JobInfo = DIRECTOR_CLASS::JobInfo;
    using ClientInfo = DIRECTOR_CLASS::ClientInfo;
    using ResourceType = DIRECTOR_CLASS::ResourceType;
    using ResourceLoadState = DIRECTOR_CLASS::ResourceLoadState;

    /**
     * @brief Constructor
     * @param parent Parent QObject
     *
     * Creates the wrapper and the underlying BareosDirector instance.
     * The director thread is started automatically.
     */
    explicit BDirector(QObject *parent = nullptr);

    /**
     * @brief Destructor
     *
     * Stops the director thread and cleans up resources.
     */
    ~BDirector();

    // ========================================================================
    // Information Methods
    // ========================================================================

    /**
     * @brief Get the backup system name
     * @return "Bareos" or "Bacula"
     */
    QString backupSystemName() const;

    /**
     * @brief Check if connected to Director
     * @return true if connected and authenticated
     */
    bool isConnected() const;

    /**
     * @brief Get current connection state
     * @return Current connection state
     */
    ConnectionState connectionState() const;

    /**
     * @brief Get current API mode
     * @return Current API mode
     */
    ApiMode apiMode() const;

    /**
     * @brief Get TLS configuration
     * @return Pointer to TLS configuration
     */
    TLSConfig *tlsConfig() const;

    /**
     * @brief Check if connection settings are stored
     * @return true if settings exist
     */
    bool hasStoredConnection() const;

    // ========================================================================
    // Resource State Machine
    // ========================================================================

    /**
     * @brief Get the loading state of a resource type
     * @param type The resource type
     * @return Current loading state
     */
    ResourceLoadState resourceState(ResourceType type) const;

    /**
     * @brief Check if a resource is loaded
     * @param type The resource type
     * @return true if state is Loaded
     */
    bool isResourceLoaded(ResourceType type) const;

    /**
     * @brief Check if all required resources are loaded
     * @return true if all required resources have state Loaded
     */
    bool areAllResourcesLoaded() const;

    /**
     * @brief Request a reload of a specific resource
     * @param type The resource type to reload
     */
    void reloadResource(ResourceType type);

    /**
     * @brief Get pointer to underlying director instance
     * @return Pointer to director instance (BareosDirector or BaculaDirector)
     * @warning Only use for signal connections, not for direct method calls!
     */
    DIRECTOR_CLASS *director() const { return m_director; }

signals:
    // ========================================================================
    // State Machine Signals
    // ========================================================================

    /**
     * @brief Emitted when connection state changes
     * @param oldState Previous state
     * @param newState New state
     */
    void connectionStateChanged(BDirector::ConnectionState oldState,
                                 BDirector::ConnectionState newState);

    /**
     * @brief Emitted when a resource load state changes
     * @param resourceType The type of resource
     * @param state The new loading state
     */
    void resourceStateChanged(BDirector::ResourceType resourceType,
                               BDirector::ResourceLoadState state);

    /**
     * @brief Emitted when a resource type has been loaded successfully
     * @param resourceType The type of resource that was loaded
     */
    void resourceLoaded(BDirector::ResourceType resourceType);

    /**
     * @brief Emitted when a resource load failed
     * @param resourceType The type of resource
     * @param errorMessage The error message
     */
    void resourceLoadFailed(BDirector::ResourceType resourceType,
                             const QString &errorMessage);

    /**
     * @brief Emitted when all required resources have been loaded
     */
    void allResourcesLoaded();

    /**
     * @brief Emitted when resource loading progress changes
     * @param loaded Number of resources loaded
     * @param total Total number of resources to load
     */
    void resourceLoadProgress(int loaded, int total);

    // ========================================================================
    // Connection Signals (forwarded from BareosDirector)
    // ========================================================================

    /**
     * @brief Emitted when connection is established
     */
    void connected();

    /**
     * @brief Emitted when disconnected
     */
    void disconnected();

    /**
     * @brief Emitted when authentication completes
     * @param success true if successful
     * @param message Director version or error message
     */
    void authentificationSucceeded(bool success, const QString &message);

    /**
     * @brief Emitted on protocol errors
     * @param error Error message
     */
    void protocolError(const QString &error);

    /**
     * @brief Emitted on status messages
     * @param status Status message
     */
    void statusMessage(const QString &status);

    /**
     * @brief Emitted when JSON response received
     * @param command The command that was sent
     * @param response JSON response
     */
    void jsonResponse(const QString &command, const QString &response);

    /**
     * @brief Emitted when text response received
     * @param command The command that was sent
     * @param response Text response
     */
    void commandResponse(const QString &command, const QString &response);

    /**
     * @brief Emitted on command errors
     * @param command The command that failed
     * @param error Error message
     */
    void commandError(const QString &command, const QString &error);

public slots:
    // ========================================================================
    // Connection Management (thread-safe)
    // ========================================================================

    /**
     * @brief Connect to Director
     * @param host Hostname or IP address
     * @param port Port number (default 9101)
     * @param directorName Director name
     * @param consoleName Console name for authentication (e.g., "admin" or "*UserAgent*")
     * @param password Console password (for CRAM-MD5 or PSK)
     *
     * Thread-safe: Forwards call to director's thread
     */
    void connect(const QString &host, int port, const QString &directorName,
                const QString &consoleName, const QString &password);

    /**
     * @brief Disconnect from Director
     *
     * Thread-safe: Forwards call to director's thread
     */
    void disconnect();

    /**
     * @brief Set TLS configuration
     * @param config TLS configuration
     */
    void setTLSConfig(const TLSConfig &config);

    /**
     * @brief Set API mode
     * @param mode API mode (Off, Json, JsonPretty)
     */
    void setApiMode(ApiMode mode);

    /**
     * @brief Load saved connection settings
     */
    void loadConnectionSettings();

    /**
     * @brief Save connection settings
     */
    void saveConnectionSettings();

    // ========================================================================
    // Command Sending (thread-safe)
    // ========================================================================

    /**
     * @brief Send command to Director
     * @param cmd Command enum
     * @param args Command arguments (optional)
     *
     * Thread-safe: Forwards call to director's thread
     */
    void doSendCommand(const Command cmd, const QString &args = QString());

    /**
     * @brief Send raw command string
     * @param command Command string
     *
     * Thread-safe: Forwards call to director's thread
     */
    void sendCommand(const QString &command);

    // ========================================================================
    // Helper Methods
    // ========================================================================

    /**
     * @brief Parse job status string
     * @param status Status string (e.g., "T", "Running")
     * @return JobStatus enum value
     */
    static JobStatus parseJobStatus(const QString &status);

    /**
     * @brief Convert job status to string
     * @param status JobStatus enum value
     * @return Status string
     */
    static QString jobStatusToString(JobStatus status);

private:
    DIRECTOR_CLASS *m_director;  ///< Underlying director instance (runs in worker thread)
    QThread *m_workerThread;     ///< Worker thread for director and auth
};

#endif // BDIRECTOR_H
