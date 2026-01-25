#ifndef DIRECTOR_H
#define DIRECTOR_H

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

/**
 * @file director.h
 * @brief Qt-based Bacula/Bareos Director Communication Interface
 *
 * This class provides a high-level interface for communicating with
 * Bacula or Bareos Director daemons via bconsole protocol.
 *
 * @author Your Name
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
 * director->sendCommand(Director::DirectorCommand::ListJobs);
 * director->sendCommand(Director::DirectorCommand::StatusDirector);
 * director->sendCommand(Director::DirectorCommand::ListJobsLast, "50");
 * @endcode
 *
 * @since 1.0.0
 * @version 1.0.0
 */
class Director : public QObject
{
    Q_OBJECT

public:
    /**
     * @enum ConnectionState
     * @brief Current connection state
     */
    enum ConnectionState {
        Disconnected,    ///< Not connected
        Connecting,      ///< TCP connection in progress
        Authenticating,  ///< CRAM-MD5 authentication in progress
        Ready            ///< Connected and authenticated, ready for commands
    };

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
     * @enum DirectorCommand
     * @brief Bareos/Bacula Director commands
     *
     * Standard bconsole commands that can be sent to the Director.
     * Commands are grouped by functionality.
     */
    enum class DirectorCommand {
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
        ListJobId,          ///< list jobid=<id>
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

        // Custom
        Custom              ///< Custom command string
    };

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
        int jobId;
        QString name;
        QString type;
        QString level;
        QString clientName;
        QString status;
        QDateTime startTime;
        QDateTime endTime;
        qint64 jobBytes;
        qint64 jobFiles;
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

    explicit Director(QObject *parent = nullptr);
    ~Director();

    // ========================================================================
    // Backend Information
    // ========================================================================

    QString backupSystemName() const;

    // ========================================================================
    // Connection Management
    // ========================================================================

    void connect(const QString &host, int port, const QString &directorName,
                 const QString &password);
    void disconnect();
    bool isConnected() const;
    ConnectionState connectionState() const;

    // ========================================================================
    // TLS Configuration
    // ========================================================================

    void setTLSConfig(const TLSConfig &config);
    TLSConfig *tlsConfig() const;

    // ========================================================================
    // Connection Settings Persistence
    // ========================================================================

    void saveConnectionSettings();
    void loadConnectionSettings();
    bool hasStoredConnection() const;

    // ========================================================================
    // Command Sending
    // ========================================================================

    /**
     * @brief Sends a raw command string to the Director
     *
     * @param command Command string (without newline)
     *
     * @since 1.0.0
     */
    void sendCommand(const QString &command);

    /**
     * @brief Sends a raw command string to the Director
     *
     * @param command Command string (without newline)
     *
     * @since 1.0.0
     */
    void sendCommand(DirectorCommand cmd, quint64);

    /**
     * @brief Sends a command using DirectorCommand enum
     *
     * @param cmd Command enum value
     * @param args Optional command arguments
     *
     * @since 1.0.0
     */
    void sendCommand(DirectorCommand cmd, const QString &args = QString());

    // ========================================================================
    // API Mode
    // ========================================================================

    void setApiMode(ApiMode mode);
    ApiMode apiMode() const;

signals:
    void connected(const QString &directorVersion);
    void disconnected();
    void connectionError(const QString &error);
    void commandResponse(const QString &response);
    void jobsReceived(const QList<Director::JobInfo> &jobs);
    void clientsReceived(const QList<Director::ClientInfo> &clients);
    void volumesReceived(const QList<Director::VolumeInfo> &volumes);
    void jobStatusChanged(int jobId, Director::JobStatus status);
    void authenticationRequired();
    void authenticationFailed(const QString &reason);
    void authenticationSucceeded(const QString &directorVersion);
    void statusMessage(const QString &message);

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
    QString directorCommandToString(DirectorCommand cmd, const QString &args = QString());
    bool setupTLSConnection();
    bool loadTLSCertificates(QSslConfiguration &sslConfig);
    void startAuthentication();
    void processResponse(const QByteArray &data);
    QString parseResponse(const QByteArray &data);
    void sendToDirector(const QByteArray &data);
    JobStatus parseJobStatus(const QString &status);
    QString jobStatusToString(JobStatus status);
    void connectSocketSignals();
    void disconnectAllSignals();

    // ========================================================================
    // Member Variables
    // ========================================================================

    ConnectionState m_connectionState;
    QSslSocket *m_sslSocket;
    QString m_directorName;
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
};

#endif // DIRECTOR_H
