#ifndef BAREOSDIRECTOR_H
#define BAREOSDIRECTOR_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QSslSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>

/**
 * @brief BareosDirector - Kommunikation mit Bareos Director
 * 
 * Unterstützt:
 * - Bareos Console (bconsole) via TCP/TLS
 * - Bareos REST API (JSON)
 * - TLS/SSL Verschlüsselung
 * 
 * Bareos ist ein Fork von Bacula mit erweiterten Features
 */
class BareosDirector : public QObject
{
    Q_OBJECT

public:
    explicit BareosDirector(QObject *parent = nullptr);
    ~BareosDirector();
    
    // Verbindungstypen
    enum ConnectionType {
        BConsole,    // bconsole TCP/TLS (kompatibel mit Bacula)
        RestAPI      // Bareos REST API (JSON)
    };
    
    // TLS-Konfiguration
    struct TLSConfig {
        bool enabled = false;
        QString caCertFile;
        QString certFile;
        QString keyFile;
        bool verifyPeer = true;
    };
    
    // REST API Konfiguration
    struct RestApiConfig {
        QString baseUrl;      // z.B. https://bareos:9101
        QString username;
        QString password;
        bool useTLS = true;
    };
    
    // Verbindungsmethoden
    bool connectBConsole(const QString &host, int port, const QString &directorName,
                         const QString &password, const TLSConfig &tlsConfig = TLSConfig());
    
    bool connectRestApi(const RestApiConfig &config);
    
    void disconnect();
    bool isConnected() const;
    
    // Befehle senden
    void sendCommand(const QString &command);
    void sendRestRequest(const QString &endpoint, const QString &method = "GET",
                        const QJsonObject &data = QJsonObject());
    
    // Getters
    ConnectionType connectionType() const { return m_connectionType; }
    QString lastError() const { return m_lastError; }
    
    // TLS-Konfiguration
    void setTLSConfig(const TLSConfig &config);
    TLSConfig tlsConfig() const { return m_tlsConfig; }
    
    // REST API-spezifische Methoden
    void getJobs();
    void getClients();
    void getStorages();
    void getVolumes();
    void getJobStatus(int jobId);
    void runJob(const QString &jobName);
    void cancelJob(int jobId);

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    void commandResponse(const QString &response);
    void restApiResponse(const QJsonDocument &response);
    void restApiError(const QString &error);

private slots:
    void onBConsoleConnected();
    void onBConsoleReadyRead();
    void onBConsoleError(QAbstractSocket::SocketError error);
    void onBConsoleDisconnected();
    
    void onSslErrors(const QList<QSslError> &errors);
    void onEncrypted();
    
    void onRestApiFinished(QNetworkReply *reply);
    void onRestApiError(QNetworkReply::NetworkError error);
    void onRestApiAuthRequired(QNetworkReply *reply, QAuthenticator *authenticator);

private:
    void setupTLSConnection();
    bool loadTLSCertificates();
    void authenticateBConsole(const QString &directorName, const QString &password);
    QString extractBareosVersion(const QString &response);
    
    // Verbindungstyp
    ConnectionType m_connectionType;
    
    // bconsole TCP/TLS
    QTcpSocket *m_tcpSocket;
    QSslSocket *m_sslSocket;
    QString m_directorName;
    QString m_password;
    bool m_authenticated;
    QString m_responseBuffer;
    
    // TLS-Konfiguration
    TLSConfig m_tlsConfig;
    
    // REST API
    QNetworkAccessManager *m_networkManager;
    RestApiConfig m_restApiConfig;
    QString m_restApiToken;
    
    // Status
    bool m_connected;
    QString m_lastError;
    QString m_bareosVersion;
};

#endif // BAREOSDIRECTOR_H
