#ifndef BACULADIRECTOR_H
#define BACULADIRECTOR_H

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
 * @brief BaculaDirector-Klasse für die Kommunikation mit Bacula/Bareos Director
 * 
 * Diese Klasse bietet zwei Verbindungsmethoden:
 * 1. Direkte bconsole-Verbindung über TCP-Socket (Bacula/Bareos)
 * 2. REST-API-Verbindung über HTTP/HTTPS
 * 
 * Unterstützt sowohl Bacula als auch Bareos (Fork von Bacula)
 */
class BaculaDirector : public QObject
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
        bool tlsVerifyPeer;

        QSharedPointer<QFile> tlsCaCertFile;        // CA-Zertifikat (.pem)

#ifdef Q_OS_WINDOWS
        // PKCS#12-Format (bevorzugt für Windows)
        QSharedPointer<QFile> tlsPfxFile;           // Pfad zur .pfx-Datei
        QString tlsPfxPassword;                       // Passwort für .pfx (kann leer sein)
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
            tlsCaCertFile(new QFile("")),
            tlsPfxFile(new QFile("")),
            tlsPfxPassword("") {}
#else
        TLSConfig() :
            tlsEnable(false),
            tlsRequire(false),
            tlsVerifyPeer(false),
            tlsCaCertFile(new QFile("")),
            tlsCertFile(new QFile("")),
            tlsKeyFile(new QFile("")){}
#endif
        // // Move constructor and assignment (automatically generated)
        // TLSConfig(TLSConfig&&) = delete;
        // TLSConfig& operator=(TLSConfig&&) = delete;

        // // Delete copy constructor and assignment
        // TLSConfig(const TLSConfig&) = delete;
        // TLSConfig& operator=(const TLSConfig&) = delete;
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

    explicit BaculaDirector(QObject *parent = nullptr);
    ~BaculaDirector();

    // Backup-System (Bacula/Bareos)
    void setBackupSystem(BackupSystem system);
    BackupSystem backupSystem() const;
    QString backupSystemName() const;

    // Verbindungsverwaltung
    void connectBConsole(const QString &host, int port, const QString &directorName, const QString &password, const TLSConfig &tlsConfig = TLSConfig());
    void connectRestAPI(const QString &baseUrl, const QString &username, const QString &password);
    void disconnect();
    bool isConnected() const;
    ConnectionType connectionType() const;
    
    // TLS-Konfiguration
    void setTLSConfig(const TLSConfig &config);
    TLSConfig tlsConfig() const;
    
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

    // REST-API-Methoden
    void restGetJobs(const QString &filter = "");
    void restGetClients();
    void restGetPools();
    void restGetVolumes();
    void restGetJobDetails(int jobId);
    void restRunJob(const QString &jobName, const QJsonObject &parameters = QJsonObject());
    void restCancelJob(int jobId);
    void restGetFilesets();
    void restGetSchedules();

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    void commandResponse(const QString &response);
    void jobsReceived(const QList<BaculaDirector::JobInfo> &jobs);
    void clientsReceived(const QList<BaculaDirector::ClientInfo> &clients);
    void volumesReceived(const QList<BaculaDirector::VolumeInfo> &volumes);
    void jobStatusChanged(int jobId, BaculaDirector::JobStatus status);
    void authenticationRequired();
    void authenticationFailed();

private slots:
    void onBConsoleConnected();
    void onBConsoleDisconnected();
    void onBConsoleReadyRead();
    void onBConsoleError(QAbstractSocket::SocketError error);
    void onRestReplyFinished(QNetworkReply *reply);
    void onAuthTimeout();
    void onSslErrors(const QList<QSslError> &errors);
    void onEncrypted();

private:
    // Bconsole-Methoden
    void processBConsoleResponse(const QByteArray &data);
    QByteArray prepareBConsolePacket(const QString &command);
    QString parseBConsoleResponse(const QByteArray &data);
    bool setupTLSConnection();
    bool loadTLSCertificates(QSslConfiguration &sslConfig);
    
    // CRAM-MD5 Authentifizierung (bidirektional)
    void handleDirectorChallengePhase1(const QString &challengeLine);
    void sendOurChallenge();
    void handleDirectorResponsePhase1b(const QString &response);
    void sendToDirector(const QByteArray &data);
    void sendPasswordAuthentication();
    QByteArray calculateCramMd5Response(const QByteArray &challenge, const QByteArray &password);
    
    // REST-API-Methoden
    void restRequest(const QString &endpoint, const QString &method = "GET", const QJsonObject &data = QJsonObject());
    void setRestAuthHeader(QNetworkRequest &request);
    void parseJobsResponse(const QJsonDocument &doc);
    void parseClientsResponse(const QJsonDocument &doc);
    void parseVolumesResponse(const QJsonDocument &doc);
    
    // Hilfsmethoden
    JobStatus parseJobStatus(const QString &status);
    QString jobStatusToString(JobStatus status);

    // Mitgliedsvariablen
    ConnectionType m_connectionType;
    BackupSystem m_backupSystem;
    
    // Bconsole-Verbindung
    QTcpSocket *m_socket;
    QSslSocket *m_sslSocket;
    QString m_directorName;
    QString m_password;
    QString m_host;
    int m_port;
    bool m_authenticated;
    bool m_authChallengeSent;  // DEPRECATED
    QByteArray m_receiveBuffer;
    TLSConfig m_tlsConfig;
    
    // Bidirektionale CRAM-MD5 Authentifizierung (Client-Seite)
    QString m_ourChallenge;
    bool m_waitingForDirectorResponse;
    bool m_phase1Complete;  // Director hat unsere Response akzeptiert
    bool m_phase2Complete;  // Wir haben Director-Response validiert
    
    // REST-API-Verbindung
    QNetworkAccessManager *m_networkManager;
    QString m_restBaseUrl;
    QString m_restUsername;
    QString m_restPassword;
    QString m_authToken;
    QTimer *m_authTimer;
    
    // Status
    bool m_connected;
    QString m_lastCommand;
    bool m_useApiMode;  // Aktiviere .api 1 Modus (wie BAT)
};

#endif // BACULADIRECTOR_H
