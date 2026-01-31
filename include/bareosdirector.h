#ifndef BAREOSDIRECTOR_H
#define BAREOSDIRECTOR_H

// ============================================================================
// Backend-Auswahl über Präprozessor
// ============================================================================
// USE_BACULA_ONLY  -> Bacula Backend (BaculaAuth)
// USE_BAREOS_ONLY  -> Bareos Backend (BareosAuth)
// Wenn keines definiert ist, wird standardmäßig Bareos verwendet
// ============================================================================

#ifdef USE_BACULA_ONLY
#include "baculaauth.h"
#define AUTH_CLASS BaculaAuth
#elif defined(USE_BAREOS_ONLY)
#include "bareosauth.h"
#define AUTH_CLASS BareosAuth
#else
// Default: Bareos
#include "bareosauth.h"
#define AUTH_CLASS BareosAuth
#define USE_BAREOS_ONLY
#endif

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QTimer>
#include <QSettings>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include <QDateTime>
#include <QRegularExpression>
#include <QMetaObject>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QSet>
#include <QMap>

// Debug logging prefixes
#define DIR_DEBUG qDebug().nospace() << "[Dir] "
#define DIR_WARNING qWarning().nospace() << "[Dir] "
#define DIR_CRITICAL qCritical().nospace() << "[Dir] "

/**
 * @file director.h
 * @brief Qt-based Bacula/Bareos Director Communication Interface
 *
 * This class provides a high-level interface for communicating with
 * Bacula or Bareos Director daemons via bconsole protocol.
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 * @version 1.0.0
 */

/**
 * @class Director
 * @brief Main interface for Bacula/Bareos Director communication
 *
 * The Director class manages connections, authentication, and command
 * execution with Bacula/Bareos Director daemons. It supports both
 * plain TCP and TLS-encrypted connections.
 *
 * ## Features
 * - Automatic backend selection (Bacula/Bareos) at compile time
 * - TLS/SSL support with certificate and PSK authentication
 * - CRAM-MD5 authentication
 * - Asynchronous command execution
 * - Type-safe command enum
 * - Connection settings persistence
 *
 * ## Backend Selection
 * The backend is selected at compile time via preprocessor defines:
 * - `USE_BACULA_ONLY`: Uses BaculaAuth backend
 * - `USE_BAREOS_ONLY`: Uses BareosAuth backend (default)
 *
 * ## Usage Example
 * @code
 * Director *director = new Director(this);
 *
 * // Configure TLS
 * Director::TLSConfig tlsConfig;
 * tlsConfig.tlsEnable = true;
 * tlsConfig.tlsPSKEnable = true;
 * director->setTLSConfig(tlsConfig);
 *
 * // Connect signals
 * connect(director, &Director::connected, this, &MyClass::onConnected);
 * connect(director, &Director::commandResponse, this, &MyClass::onResponse);
 *
 * // Connect to Director
 * director->connect("192.168.1.10", 9101, "bareos-dir", "mypassword");
 *
 * // Send commands using enum
 * director->doSendCommand(Director::Command::ListJobs);
 * director->doSendCommand(Director::Command::StatusDirector);
 * director->doSendCommand(Director::Command::ListJobsLast, "50");
 * @endcode
 *
 * @since 1.0.0
 * @version 1.0.0
 */
class BareosDirector : public QObject
{
    Q_OBJECT

public:
    /**
     * @enum ConnectionState
     * @brief Current connection state with full initialization phases
     *
     * State Machine:
     * @code
     *   Disconnected
     *        │
     *        ▼ (connect() called)
     *   Connecting
     *        │
     *        ▼ (socket connected)
     *   Authenticating
     *        │
     *        ▼ (CRAM-MD5 success)
     *   SettingApiMode
     *        │
     *        ▼ (.api 2 confirmed)
     *   LoadingResources
     *        │
     *        ▼ (all resources loaded)
     *   Ready
     *        │
     *        ▼ (error or disconnect)
     *   Disconnected / Error
     * @endcode
     */
    enum ConnectionState {
        Disconnected,       ///< Not connected
        Connecting,         ///< TCP connection in progress
        Authenticating,     ///< CRAM-MD5 authentication in progress
        SettingApiMode,     ///< .api 2 command sent, waiting for confirmation
        LoadingResources,   ///< Loading all resource data (dot-commands)
        Ready,              ///< Fully initialized, ready for user commands
        ConnectionError     ///< Connection error state
    };
    Q_ENUM(ConnectionState)

    /**
     * @enum ResourceType
     * @brief Bareos resource types that can be loaded via dot-commands
     *
     * These correspond to the Bareos Director resource configuration types.
     * @see https://docs.bareos.org/Configuration/Director.html
     */
    enum class ResourceType {
        Catalog,    ///< .catalogs - Database catalogs
        Client,     ///< .clients - Backup clients (File Daemons)
        Console,    ///< .consoles - Console configurations (not typically loaded)
        Director,   ///< .directors - Director configurations (not typically loaded)
        Fileset,    ///< .filesets - File set definitions
        Job,        ///< .jobs - Job definitions
        JobDefs,    ///< .jobdefs - Job default definitions (not a dot-command)
        Messages,   ///< .messages - Message configurations (not typically loaded)
        Pool,       ///< .pools - Storage pools
        Profile,    ///< .profiles - ACL profiles (not typically loaded)
        Schedule,   ///< .schedule - Backup schedules
        Storage,    ///< .storages - Storage daemons
        Level       ///< .levels - Backup levels (F, I, D, etc.)
    };
    Q_ENUM(ResourceType)

    /**
     * @enum ResourceLoadState
     * @brief Loading state for each resource type
     */
    enum class ResourceLoadState {
        Initial,    ///< Not yet requested
        Waiting,    ///< Request sent, waiting for response
        Loaded,     ///< Successfully loaded
        Reloading,  ///< Reload requested, waiting for response
        Failed      ///< Loading failed
    };
    Q_ENUM(ResourceLoadState)

    /**
     * @enum ApiMode
     * @brief API output modes for Director responses
     *
     * Controls the format of Director responses. Different modes provide
     * different levels of structure and machine-readability.
     *
     * @see https://docs.bareos.org/DeveloperGuide/api.html
     */
    enum ApiMode {
        Off = 0,         ///< No API mode (human-readable text)
        Json = 1,        ///< Compact JSON output
        JsonPretty = 2   ///< Pretty-printed JSON output
    };

    /**
     * @enum Command
     * @brief Bareos/Bacula Director commands
     *
     * Standard bconsole commands that can be sent to the Director.
     * Commands are grouped by functionality.
     */
    enum class Command {
        // Connection & Session
        ApiMode,            ///< .api [mode] - Set API output mode
        Quit,               ///< quit - Close connection
        Exit,               ///< exit - Alias for quit

        // Status Commands
        StatusDirector,     ///< status director
        StatusClient,       ///< status client=<name>
        StatusStorage,      ///< status storage=<name>
        StatusScheduler,    ///< status scheduler
        StatusRunning,      ///< status running
        StatusSubscriptions,///< status subscriptions (Bareos only)

        // List Commands
        ListJobs,           ///< list jobs
        ListJobsLast,       ///< list jobs last=<n>
        ListJobId,          ///< list joblog jobid=<id>
        ListClients,        ///< list clients
        ListPools,          ///< list pools
        ListVolumes,        ///< list volumes
        ListVolumePool,     ///< list volumes pool=<name>
        ListMedia,          ///< list media
        ListFileSets,       ///< list filesets
        ListFiles,          ///< list files jobid=<id>
        ListNextVolume,     ///< list nextvol job=<name>
        ListBackups,        ///< list backups
        ListBackupsClient,  ///< list backups client=<name>
        ListJobTotals,      ///< list jobtotals - Get total job count for pagination

        // Job Control
        Run,                ///< run job=<name>
        RunYes,             ///< run job=<name> yes
        Cancel,             ///< cancel jobid=<id>
        Delete,             ///< delete job jobid=<id>
        Disable,            ///< disable job=<name>
        Enable,             ///< enable job=<name>
        Rerun,              ///< rerun jobid=<id>

        // Restore
        Restore,            ///< restore
        RestoreAll,         ///< restore all
        RestoreSelect,      ///< restore select

        // Volume Management
        Label,              ///< label
        Relabel,            ///< relabel
        Mount,              ///< mount storage=<name>
        Unmount,            ///< unmount storage=<name>
        Release,            ///< release storage=<name>
        Update,             ///< update
        UpdateVolume,       ///< update volume=<name>
        Purge,              ///< purge volume=<name>
        Prune,              ///< prune
        PruneFiles,         ///< prune files
        PruneJobs,          ///< prune jobs
        PruneVolume,        ///< prune volume

        // Console Commands
        Show,               ///< show
        ShowJobs,           ///< show jobs
        ShowClients,        ///< show clients
        ShowFilesets,       ///< show filesets
        ShowSchedules,      ///< show schedules
        ShowPools,          ///< show pools
        ShowStorages,       ///< show storages
        ShowCatalogs,       ///< show catalogs
        ShowMessages,       ///< show messages
        ShowAll,            ///< show all

        // Messages
        Messages,           ///< messages

        // Catalog
        SqlQuery,           ///< sqlquery
        Query,              ///< query

        // Testing & Debugging
        Estimate,           ///< estimate
        Time,               ///< time
        Trace,              ///< trace on|off
        Version,            ///< version
        Memory,             ///< memory

        // Configuration
        Reload,             ///< reload
        Configure,          ///< configure (Bareos only)
        Export,             ///< export (Bareos only)
        Import,             ///< import (Bareos only)

        // Help
        Help,               ///< help

        // Dot Commands (GUI/API mode commands)
        DotJobs,            ///< .jobs - List all configured job resources
        DotClients,         ///< .clients - List all client resources
        DotPools,           ///< .pools - List all pool resources
        DotStorages,        ///< .storages - List all storage resources
        DotFilesets,        ///< .filesets - List all fileset resources
        DotSchedule,        ///< .schedule - List all schedule resources
        DotCatalogs,        ///< .catalogs - List all catalog resources
        DotLevels,          ///< .levels - List all backup levels
        DotTypes,           ///< .types - List all job types
        DotMedia,           ///< .media - List all media/volumes
        DotHelp,            ///< .help - List all dot commands
        DotDefaults,        ///< .defaults job=<name> - Get job default values

        // Custom
        Custom              ///< Custom command string
    };
    Q_ENUM(Command)

    /**
     * @enum JobStatus
     * @brief Backup job status codes
     */
    enum JobStatus {
        Created,      ///< Job created but not started
        Running,      ///< Job is currently running
        Blocked,      ///< Job is blocked (waiting for resource)
        Terminated,   ///< Job terminated (may be success or error)
        Waiting,      ///< Job waiting to start
        Successful,   ///< Job completed successfully
        Error,        ///< Job completed with errors
        Fatal,        ///< Job failed fatally
        Canceled,     ///< Job was canceled
        Unknown       ///< Unknown status
    };

    /**
     * @struct JobInfo
     * @brief Information about a backup job
     */
    struct JobInfo {
        quint64 jobId;          ///< Job ID
        QString name;           ///< Job name
        QString type;           ///< Job type (B=Backup, R=Restore, etc.)
        QString level;          ///< Backup level (F/I/D)
        QString clientName;     ///< Client name
        QString status;         ///< Job status (C/R/T/W/f/E/e/A)
        QDateTime startTime;    ///< Start time
        QString duration;       ///< Duration string
        qint64 jobBytes;        ///< Bytes backed up
        qint64 jobFiles;        ///< Files backed up
    };

    /**
     * @struct ClientInfo
     * @brief Information about a backup client
     */
    struct ClientInfo {
        QString name;
        QString address;
        int port;
        QString os;
        bool autoprune;
        int fileRetention;
        int jobRetention;
    };

    /**
     * @struct TLSConfig
     * @brief TLS/SSL configuration
     */
    struct TLSConfig {
        bool tlsEnable;
        bool tlsRequire;
        bool tlsPSKEnable;
        bool tlsVerifyPeer;

        QSharedPointer<QFile> tlsCaCertFile;

#ifdef Q_OS_WINDOWS
        QSharedPointer<QFile> tlsPfxFile;
        QString tlsPfxPassword;
#else
        QSharedPointer<QFile> tlsCertFile;
        QSharedPointer<QFile> tlsKeyFile;
#endif

#ifdef Q_OS_WINDOWS
        TLSConfig() :
            tlsEnable(false),
            tlsRequire(false),
            tlsVerifyPeer(false),
            tlsPSKEnable(false),
            tlsCaCertFile(new QFile("")),
            tlsPfxFile(new QFile("")),
            tlsPfxPassword("") {}
#else
        TLSConfig() :
            tlsEnable(false),
            tlsRequire(false),
            tlsVerifyPeer(false),
            tlsPSKEnable(false),
            tlsCaCertFile(new QFile("")),
            tlsCertFile(new QFile("")),
            tlsKeyFile(new QFile("")){}
#endif
    };

    /**
     * @struct VolumeInfo
     * @brief Information about a storage volume
     */
    struct VolumeInfo {
        QString volumeName;
        QString poolName;
        QString mediaType;
        QString status;
        qint64 volumeBytes;
        qint64 maxVolumeBytes;
        int volRetention;
    };

    explicit BareosDirector(QObject *parent = nullptr);
    ~BareosDirector();

    // ========================================================================
    // Backend Information
    // ========================================================================

    QString backupSystemName() const;

    // ========================================================================
    // Connection Management
    // ========================================================================

    bool isConnected() const;
    ConnectionState connectionState() const;

    /**
     * @brief Get current host name
     * @return Host name or empty string if not connected
     */
    QString currentHost() const { return m_host; }

    /**
     * @brief Get current port
     * @return Port number or 0 if not connected
     */
    int currentPort() const { return m_port; }

    /**
     * @brief Get current director name
     * @return Director name or empty string if not connected
     */
    QString currentDirectorName() const { return m_directorName; }

    // ========================================================================
    // TLS Configuration
    // ========================================================================

    void setTLSConfig(const TLSConfig &config);
    TLSConfig *tlsConfig() const;

    // ========================================================================
    // Connection Settings Persistence
    // ========================================================================

    Q_INVOKABLE void saveConnectionSettings();
    Q_INVOKABLE void loadConnectionSettings();
    Q_INVOKABLE bool hasStoredConnection() const;

    // ========================================================================
    // Command Sending
    // ========================================================================


    // ========================================================================
    // API Mode
    // ========================================================================

    void setApiMode(ApiMode mode);
    ApiMode apiMode() const;

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

signals:
    // ========================================================================
    // State Machine Signals
    // ========================================================================

    /**
     * @brief Emitted when connection state changes
     * @param oldState Previous state
     * @param newState New state
     */
    void connectionStateChanged(BareosDirector::ConnectionState oldState,
                                 BareosDirector::ConnectionState newState);

    /**
     * @brief Emitted when a resource load state changes
     * @param resourceType The type of resource
     * @param state The new loading state
     */
    void resourceStateChanged(BareosDirector::ResourceType resourceType,
                               BareosDirector::ResourceLoadState state);

    /**
     * @brief Emitted when a resource type has been loaded successfully
     * @param resourceType The type of resource that was loaded
     */
    void resourceLoaded(BareosDirector::ResourceType resourceType);

    /**
     * @brief Emitted when a resource load failed
     * @param resourceType The type of resource
     * @param errorMessage The error message
     */
    void resourceLoadFailed(BareosDirector::ResourceType resourceType,
                             const QString &errorMessage);

    /**
     * @brief Emitted when all required resources have been loaded
     *
     * This signal indicates that the Director is fully initialized
     * and ready for user commands. The state will be Ready.
     */
    void allResourcesLoaded();

    /**
     * @brief Emitted when resource loading progress changes
     * @param loaded Number of resources loaded
     * @param total Total number of resources to load
     */
    void resourceLoadProgress(int loaded, int total);

    // ========================================================================
    // Connection Signals
    // ========================================================================

    void authentificationSucceeded(const bool result, const QString &msg);
    void protocolError(const QString &msg);

    void disconnected();

    /**
     * @brief Emitted when a JSON response is received from the Director
     * @param command The command that was sent (e.g., "list jobs")
     * @param jsonData The JSON response data
     */
    void jsonResponse(const QString &command, const QString &jsonData);

    /**
     * @brief Emitted when a text response is received from the Director
     * @param command The command that was sent
     * @param response The text response
     */
    void commandResponse(const QString &command, const QString &response);

    // void jobsReceived(const QList<BareosDirector::JobInfo> &jobs);
    // void clientsReceived(const QList<BareosDirector::ClientInfo> &clients);
    // void volumesReceived(const QList<BareosDirector::VolumeInfo> &volumes);
    void jobStatusChanged(int jobId, BareosDirector::JobStatus status);
    void authenticationRequired();
    void statusMessage(const QString &message);
    void commandError(const QString &command, const QString &error);

public slots:

    /**
     * @brief Thread-safe connection to Director
     * @param host Hostname or IP address
     * @param port Port number
     * @param directorName Director name
     * @param consoleName Console name for authentication (e.g., "admin" or "*UserAgent*")
     * @param password Console password
     * @since 1.0.0
     */
    void connect(const QString &host, int port, const QString &directorName,
                 const QString &consoleName, const QString &password);

    /**
     * @brief Thread-safe disconnection from Director
     * @since 1.0.0
     */
    void disconnect();

    /**
     * @brief Sends a raw command string to the Director
     *
     * @param command Command string (without newline)
     * @deprecated
     *
     * @since 1.0.0
     */
    void doSendCommand(Command cmd, quint64);

    /**
     * @brief Internal slot for thread-safe socket writing
     * @param packet Raw packet data to write to socket
     * @since 1.0.0
     */
    void writeToSocket(const QByteArray &packet);

    /**
     * @brief Sends a command using Command enum
     *
     * @param cmd Command enum value
     * @param args Optional command arguments
     *
     * @since 1.0.0
     */
    void doSendCommand(const BareosDirector::Command cmd, const QString &args = QString());

    /**
     * @brief Parse job status string to JobStatus enum
     * @param status Status string (e.g., "T", "Running")
     * @return JobStatus enum value
     * @since 1.0.0
     */
    static JobStatus parseJobStatus(const QString &status);

    /**
     * @brief Convert JobStatus enum to string
     * @param status JobStatus enum value
     * @return Status string
     * @since 1.0.0
     */
    static QString jobStatusToString(JobStatus status);

    /**
     * @brief Sends a raw command string to the Director
     *
     * Use this for commands not covered by the Command enum,
     * such as "show consoles" or "configure add console".
     *
     * @param command Command string (without newline)
     * @since 1.0.0
     */
    void sendRawCommand(const QString &command);

public slots:
    /**
     * @brief Initialize director (creates socket and auth in current thread)
     * @note Must be called from worker thread
     */
    void initialize();

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onBytesWritten(qint64 bytes);
    void onError(QAbstractSocket::SocketError error);
    void onSslErrors(const QList<QSslError> &errors);
    void onEncrypted();
    void onAuthenticationSucceeded(const QString directorVersion);
    void onAuthenticationFailed(const QString &reason);
    void onAuthStatusMessage(const QString &message);

private:
    // ========================================================================
    // State Machine Methods
    // ========================================================================

    /**
     * @brief Sets the connection state and emits signal
     * @param newState The new connection state
     */
    void setState(ConnectionState newState);

    /**
     * @brief Marks a resource as loaded and checks if all resources are ready
     * @param resourceType The resource type that was loaded
     */
    void markResourceLoaded(ResourceType resourceType);

    /**
     * @brief Marks a resource as failed
     * @param resourceType The resource type that failed
     * @param errorMessage The error message
     */
    void markResourceFailed(ResourceType resourceType, const QString &errorMessage);

    /**
     * @brief Detects resource type from JSON response and marks it as loaded
     * @param jsonData The JSON response data
     *
     * Parses the JSON to identify which resource type it represents
     * based on the keys in the result object (levels, filesets, etc.)
     */
    void detectAndMarkResourceLoaded(const QString &jsonData);

    /**
     * @brief Checks if all required resources have been loaded
     * @return true if all resources are loaded
     */
    bool allResourcesReady() const;

    /**
     * @brief Starts loading all required resources (dot-commands)
     *
     * Called automatically after API mode is confirmed.
     * Sends: .jobs, .clients, .filesets, .storages, .pools, .levels, .schedule
     */
    void startResourceLoading();

    /**
     * @brief Resets all resource loading flags
     */
    void resetResourceFlags();

    // ========================================================================
    // Protocol Methods
    // ========================================================================

    const QString commandToString(Command cmd, const QString &args = QString());
    void startAuthentication();
    void processResponse(const QByteArray &data);
    QString parseResponse(const QByteArray &data);
    void connectSocketSignals();
    void disconnectAllSignals();
    void processDirectorMessage(const QString &message, bool isSignal);

    /**
     * @brief Sends a raw command string to the Director
     *
     * @param command Command string (without newline)
     * @deprecated
     *
     * @since 1.0.0
     */
    void sendCommand(const QString &command);

    inline QString unbashSpaces(const QString &str)
    {
        QString result = str;
        result = result.replace('\x1E', ' ')
                    .replace(QChar(0x001E), "") // Record Separator
                    .replace('\x01', ' ');    // Bareos space encoding
        return result;
    }


    // ========================================================================
    // Member Variables
    // ========================================================================

    ConnectionState m_connectionState;
    QSslSocket *m_socket;
    QString m_directorName;
    QString m_consoleName;
    QString m_password;
    QString m_host;
    int m_port;
    QByteArray m_receiveBuffer;
    QByteArray m_readBuffer;
    QByteArray m_writeBuffer;
    TLSConfig *m_tlsConfig;
    QString m_directorVersion;
    AUTH_CLASS *m_auth;
    bool m_connected;
    QString m_lastCommand;
    ApiMode m_apiMode;
    bool m_jsonTextAccumulation;  ///< True when accumulating fragmented JSON text

    quint64 m_lastSentSize;

    // Signal connections
    QMetaObject::Connection m_connSocketConnected;
    QMetaObject::Connection m_connSocketDisconnected;
    QMetaObject::Connection m_connSocketReadyRead;
    QMetaObject::Connection m_connSocketError;
    QMetaObject::Connection m_connSocketSslErrors;
    QMetaObject::Connection m_connSocketEncrypted;
    QMetaObject::Connection m_connAuthSucceeded;
    QMetaObject::Connection m_connAuthFailed;
    QMetaObject::Connection m_connAuthStatus;
    QMetaObject::Connection m_connReadyRead;
    QMetaObject::Connection m_connBytesWritten;

    // Thread-Safety
    mutable QMutex m_connectionMutex;  ///< Schützt Socket-Operationen
    mutable QMutex m_stateMutex;       ///< Schützt m_connectionState, m_connected
    mutable QMutex m_authMutex;        ///< Mutex für Authentifizierung-Wait
    QWaitCondition m_authCondition;    ///< Wait Condition für Authentifizierung
    bool m_initialized;                ///< Socket initialisiert?
    bool m_authCompleted;              ///< Authentifizierung abgeschlossen (Erfolg oder Fehler)

    // ========================================================================
    // Resource Loading State Machine
    // ========================================================================

    /**
     * @brief Set of required resources to load during initialization
     *
     * Default resources: Job, Client, Fileset, Storage, Pool, Level, Schedule
     * Optional resources: Catalog, Messages, Profile, Console, Director, JobDefs
     */
    QSet<ResourceType> m_requiredResources;

    /**
     * @brief Map of resource type to its current loading state
     */
    QMap<ResourceType, ResourceLoadState> m_resourceStates;

    /**
     * @brief Error message if state is Error
     */
    QString m_errorMessage;

    /**
     * @brief Error messages for individual resource loads
     */
    QMap<ResourceType, QString> m_resourceErrors;
};

#endif // BAREOSDIRECTOR_H
