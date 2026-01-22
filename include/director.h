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

#include "version.h"

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QSettings>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include <QMessageAuthenticationCode>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QDateTime>
#include <QRegularExpression>
#include <QHostInfo>

/**
 * @brief Director-Klasse für die Kommunikation mit Bacula/Bareos Director
 *
 * Diese Klasse bietet zwei Verbindungsmethoden:
 * 1. Direkte bconsole-Verbindung über TCP-Socket (Bacula/Bareos)
 * 2. REST-API-Verbindung über HTTP/HTTPS
 *
 * Unterstützt sowohl Bacula als auch Bareos (Fork von Bacula)
 *
 * Backend-Auswahl zur Compile-Zeit:
 * - USE_BACULA_ONLY: Verwendet BaculaAuth
 * - USE_BAREOS_ONLY: Verwendet BareosAuth (Standard)
 */
class Director : public QObject
{
    Q_OBJECT

public:
    enum BackupSystem {
        Bacula,     // Original Bacula
        Bareos      // Bareos (Bacula Fork)
    };

    enum ConnectionType {
        BConsole,   // Direkte bconsole-Verbindung
        RestAPI     // REST-API-Verbindung
    };

    enum ConnectionState {
        Disconnected,
        Connecting,
        Authenticating,
        Ready
    };

    enum JobStatus {
        Created,
        Running,
        Blocked,
        Terminated,
        Waiting,
        Successful,
        Error,
        Fatal,
        Canceled,
        Unknown
    };

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

    struct ClientInfo {
        QString name;
        QString address;
        int port;
        QString os;
        bool autoprune;
        int fileRetention;
        int jobRetention;
    };

    struct TLSConfig {
        bool tlsEnable;
        bool tlsRequire;
        bool tlsPSKEnable;
        bool tlsVerifyPeer;

        QSharedPointer<QFile> tlsCaCertFile;        // CA-Zertifikat (.pem)

#ifdef Q_OS_WINDOWS
        // PKCS#12-Format (bevorzugt für Windows)
        QSharedPointer<QFile> tlsPfxFile;           // Pfad zur .pfx-Datei
        QString tlsPfxPassword;                     // Passwort für .pfx (kann leer sein)
#else
        // PEM-Format (Legacy, für Linux)
        QSharedPointer<QFile> tlsCertFile;          // Client-Zertifikat (.pem)
        QSharedPointer<QFile> tlsKeyFile;           // Private Key (.pem)
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
    // Backup-System (compile-time determined)
    // ========================================================================

    /**
     * @deprecated Backend is determined at compile time.
     * This method has no effect and is kept for backward compatibility only.
     */
    [[deprecated("Backend is determined at compile time via USE_BACULA_ONLY/USE_BAREOS_ONLY")]]
    void setBackupSystem(BackupSystem system);

    /**
     * @deprecated Use backupSystemName() instead.
     * @return BackupSystem enum based on compile-time configuration
     */
    [[deprecated("Use backupSystemName() for string representation")]]
    BackupSystem backupSystem() const;

    /**
     * @brief Returns the name of the compiled backend
     * @return "Bacula" or "Bareos" depending on compile-time configuration
     */
    QString backupSystemName() const;

    // ========================================================================
    // Verbindungsverwaltung
    // ========================================================================
    void connect(const QString &host, int port, const QString &directorName,
                 const QString &password);
    void disconnect();
    bool isConnected() const;
    ConnectionState connectionState() const;

    // TLS-Konfiguration
    void setTLSConfig(const TLSConfig &config);
    TLSConfig *tlsConfig() const;

    // Verbindungseinstellungen speichern/laden
    void saveConnectionSettings();
    void loadConnectionSettings();
    bool hasStoredConnection() const;

    // Bconsole-Befehle
    void sendCommand(const QString &command);
    void listJobs(int limit = 100);
    void listClients();
    void listPools();
    void listVolumes();
    void showJobDetails(int jobId);
    void runJob(const QString &jobName);
    void cancelJob(int jobId);
    void restoreFiles(const QString &clientName, const QString &fileSet);
    void statusDirector();
    void statusStorage(const QString &storageName);
    void statusClient(const QString &clientName);

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    void commandResponse(const QString &response);
    void jobsReceived(const QList<Director::JobInfo> &jobs);
    void clientsReceived(const QList<Director::ClientInfo> &clients);
    void volumesReceived(const QList<Director::VolumeInfo> &volumes);
    void jobStatusChanged(int jobId, Director::JobStatus status);
    void authenticationRequired();
    void authenticationFailed(const QString &reason);
    void authenticationSucceeded();
    void statusMessage(const QString &message);

private slots:
    // Socket-Event-Handler
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError error);
    void onSslErrors(const QList<QSslError> &errors);
    void onEncrypted();

    // Authentifizierungs-Handler
    void onAuthenticationSucceeded(int directorVersion);
    void onAuthenticationFailed(const QString &reason);
    void onAuthStatusMessage(const QString &message);

private:
    // Verbindungsmethoden
    bool setupTLSConnection();
    bool loadTLSCertificates(QSslConfiguration &sslConfig);
    void startAuthentication();

    // Bconsole-Methoden
    void processResponse(const QByteArray &data);
    QByteArray preparePacket(const QString &command);
    QString parseResponse(const QByteArray &data);
    void sendToDirector(const QByteArray &data);

    // REST-API-Methoden
    void parseVolumesResponse(const QJsonDocument &doc);

    // Hilfsmethoden
    JobStatus parseJobStatus(const QString &status);
    QString jobStatusToString(JobStatus status);

    // ========================================================================
    // Mitgliedsvariablen
    // ========================================================================
    ConnectionState m_connectionState;

    /**
     * @deprecated Not used anymore. Backend is determined at compile time.
     * Kept for ABI compatibility only.
     */
    BackupSystem m_backupSystem;  // @deprecated

    // Bconsole-Verbindung
    QSslSocket *m_sslSocket;
    QString m_directorName;
    QString m_password;
    QString m_host;
    int m_port;
    QByteArray m_receiveBuffer;
    TLSConfig *m_tlsConfig;
    int m_directorVersion;

    // Authentifizierung - verwendet AUTH_CLASS Makro für korrekten Typ
    AUTH_CLASS *m_auth;

    // Status
    bool m_connected;
    QString m_lastCommand;
    bool m_useApiMode;  // Aktiviere .api 1 Modus (wie BAT)
};

#endif // DIRECTOR_H
