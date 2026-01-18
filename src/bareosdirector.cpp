#include "bareosdirector.h"

#include <QSslConfiguration>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrlQuery>

BareosDirector::BareosDirector(QObject *parent)
    : QObject(parent)
    , m_connectionType(BConsole)
    , m_tcpSocket(nullptr)
    , m_sslSocket(nullptr)
    , m_authenticated(false)
    , m_networkManager(nullptr)
    , m_connected(false)
{
}

BareosDirector::~BareosDirector()
{
    disconnect();
}

bool BareosDirector::connectBConsole(const QString &host, int port,
                                     const QString &directorName,
                                     const QString &password,
                                     const TLSConfig &tlsConfig)
{
    if (m_connected) {
        disconnect();
    }
    
    m_connectionType = BConsole;
    m_directorName = directorName;
    m_password = password;
    m_tlsConfig = tlsConfig;
    m_authenticated = false;
    m_responseBuffer.clear();
    
    if (m_tlsConfig.enabled) {
        // TLS-verschlüsselte Verbindung
        if (!m_sslSocket) {
            m_sslSocket = new QSslSocket(this);
            connect(m_sslSocket, &QSslSocket::connected, this, &BareosDirector::onBConsoleConnected);
            connect(m_sslSocket, &QSslSocket::readyRead, this, &BareosDirector::onBConsoleReadyRead);
            connect(m_sslSocket, QOverload<QAbstractSocket::SocketError>::of(&QSslSocket::errorOccurred),
                    this, &BareosDirector::onBConsoleError);
            connect(m_sslSocket, &QSslSocket::disconnected, this, &BareosDirector::onBConsoleDisconnected);
            connect(m_sslSocket, QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
                    this, &BareosDirector::onSslErrors);
            connect(m_sslSocket, &QSslSocket::encrypted, this, &BareosDirector::onEncrypted);
        }
        
        setupTLSConnection();
        m_sslSocket->connectToHostEncrypted(host, port);
        
    } else {
        // Unverschlüsselte Verbindung
        if (!m_tcpSocket) {
            m_tcpSocket = new QTcpSocket(this);
            connect(m_tcpSocket, &QTcpSocket::connected, this, &BareosDirector::onBConsoleConnected);
            connect(m_tcpSocket, &QTcpSocket::readyRead, this, &BareosDirector::onBConsoleReadyRead);
            connect(m_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
                    this, &BareosDirector::onBConsoleError);
            connect(m_tcpSocket, &QTcpSocket::disconnected, this, &BareosDirector::onBConsoleDisconnected);
        }
        
        m_tcpSocket->connectToHost(host, port);
    }
    
    return true;
}

bool BareosDirector::connectRestApi(const RestApiConfig &config)
{
    if (m_connected) {
        disconnect();
    }
    
    m_connectionType = RestAPI;
    m_restApiConfig = config;
    
    if (!m_networkManager) {
        m_networkManager = new QNetworkAccessManager(this);
        connect(m_networkManager, &QNetworkAccessManager::finished,
                this, &BareosDirector::onRestApiFinished);
        connect(m_networkManager, &QNetworkAccessManager::authenticationRequired,
                this, &BareosDirector::onRestApiAuthRequired);
    }
    
    // REST API Login
    QUrl url(m_restApiConfig.baseUrl + "/api/v1/auth/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QJsonObject loginData;
    loginData["username"] = m_restApiConfig.username;
    loginData["password"] = m_restApiConfig.password;
    
    QJsonDocument doc(loginData);
    m_networkManager->post(request, doc.toJson());
    
    return true;
}

void BareosDirector::disconnect()
{
    if (m_sslSocket) {
        m_sslSocket->disconnectFromHost();
        m_sslSocket->deleteLater();
        m_sslSocket = nullptr;
    }
    
    if (m_tcpSocket) {
        m_tcpSocket->disconnectFromHost();
        m_tcpSocket->deleteLater();
        m_tcpSocket = nullptr;
    }
    
    m_connected = false;
    m_authenticated = false;
    m_restApiToken.clear();
}

bool BareosDirector::isConnected() const
{
    return m_connected;
}

void BareosDirector::sendCommand(const QString &command)
{
    if (!m_connected || !m_authenticated) {
        emit commandResponse("Error: Not connected or authenticated");
        return;
    }
    
    QString cmd = command.trimmed() + "\n";
    
    if (m_tlsConfig.enabled && m_sslSocket) {
        m_sslSocket->write(cmd.toUtf8());
    } else if (m_tcpSocket) {
        m_tcpSocket->write(cmd.toUtf8());
    }
}

void BareosDirector::sendRestRequest(const QString &endpoint, const QString &method,
                                     const QJsonObject &data)
{
    if (m_restApiToken.isEmpty()) {
        emit restApiError("Not authenticated to REST API");
        return;
    }
    
    QUrl url(m_restApiConfig.baseUrl + endpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(m_restApiToken).toUtf8());
    
    if (method == "GET") {
        m_networkManager->get(request);
    } else if (method == "POST") {
        QJsonDocument doc(data);
        m_networkManager->post(request, doc.toJson());
    } else if (method == "PUT") {
        QJsonDocument doc(data);
        m_networkManager->put(request, doc.toJson());
    } else if (method == "DELETE") {
        m_networkManager->deleteResource(request);
    }
}

void BareosDirector::setTLSConfig(const TLSConfig &config)
{
    m_tlsConfig = config;
}

void BareosDirector::getJobs()
{
    sendRestRequest("/api/v1/jobs", "GET");
}

void BareosDirector::getClients()
{
    sendRestRequest("/api/v1/clients", "GET");
}

void BareosDirector::getStorages()
{
    sendRestRequest("/api/v1/storages", "GET");
}

void BareosDirector::getVolumes()
{
    sendRestRequest("/api/v1/volumes", "GET");
}

void BareosDirector::getJobStatus(int jobId)
{
    sendRestRequest(QString("/api/v1/jobs/%1").arg(jobId), "GET");
}

void BareosDirector::runJob(const QString &jobName)
{
    QJsonObject data;
    data["job"] = jobName;
    sendRestRequest("/api/v1/jobs/run", "POST", data);
}

void BareosDirector::cancelJob(int jobId)
{
    sendRestRequest(QString("/api/v1/jobs/%1/cancel").arg(jobId), "POST");
}

void BareosDirector::onBConsoleConnected()
{
    qDebug() << "Bareos Director: Connected to bconsole";
    // Warte auf Begrüßungsnachricht
}

void BareosDirector::onBConsoleReadyRead()
{
    QByteArray data;
    
    if (m_tlsConfig.enabled && m_sslSocket) {
        data = m_sslSocket->readAll();
    } else if (m_tcpSocket) {
        data = m_tcpSocket->readAll();
    }
    
    m_responseBuffer.append(QString::fromUtf8(data));
    
    if (!m_authenticated) {
        // Warte auf Authentifizierungsaufforderung
        if (m_responseBuffer.contains("Enter a period to cancel")) {
            authenticateBConsole(m_directorName, m_password);
        } else if (m_responseBuffer.contains("*")) {
            // Authentifizierung erfolgreich
            m_authenticated = true;
            m_connected = true;
            
            // Extrahiere Bareos-Version
            m_bareosVersion = extractBareosVersion(m_responseBuffer);
            
            m_responseBuffer.clear();
            emit connected();
        }
    } else {
        // Normale Kommando-Antwort
        if (m_responseBuffer.endsWith("*")) {
            QString response = m_responseBuffer;
            m_responseBuffer.clear();
            emit commandResponse(response);
        }
    }
}

void BareosDirector::onBConsoleError(QAbstractSocket::SocketError error)
{
    QString errorStr;
    
    if (m_tlsConfig.enabled && m_sslSocket) {
        errorStr = m_sslSocket->errorString();
    } else if (m_tcpSocket) {
        errorStr = m_tcpSocket->errorString();
    }
    
    m_lastError = errorStr;
    emit connectionError(errorStr);
}

void BareosDirector::onBConsoleDisconnected()
{
    m_connected = false;
    m_authenticated = false;
    emit disconnected();
}

void BareosDirector::onSslErrors(const QList<QSslError> &errors)
{
    QString errorStr = "SSL Errors:\n";
    for (const QSslError &error : errors) {
        errorStr += "- " + error.errorString() + "\n";
    }
    
    m_lastError = errorStr;
    qWarning() << errorStr;
    
    // Optional: Ignoriere Fehler wenn verifyPeer = false
    if (!m_tlsConfig.verifyPeer && m_sslSocket) {
        m_sslSocket->ignoreSslErrors();
    } else {
        emit connectionError(errorStr);
    }
}

void BareosDirector::onEncrypted()
{
    qDebug() << "Bareos Director: TLS connection established";
    qDebug() << "Cipher:" << m_sslSocket->sessionCipher().name();
    qDebug() << "Protocol:" << m_sslSocket->sessionCipher().protocolString();
}

void BareosDirector::onRestApiFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        
        // Prüfe ob Login-Response
        if (reply->url().path().endsWith("/auth/login")) {
            QJsonObject obj = doc.object();
            if (obj.contains("token")) {
                m_restApiToken = obj["token"].toString();
                m_connected = true;
                emit connected();
            }
        } else {
            emit restApiResponse(doc);
        }
    } else {
        QString error = reply->errorString();
        m_lastError = error;
        emit restApiError(error);
    }
    
    reply->deleteLater();
}

void BareosDirector::onRestApiError(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)
}

void BareosDirector::onRestApiAuthRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    Q_UNUSED(reply)
    authenticator->setUser(m_restApiConfig.username);
    authenticator->setPassword(m_restApiConfig.password);
}

void BareosDirector::setupTLSConnection()
{
    if (!m_sslSocket) {
        return;
    }
    
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);
    
    if (m_tlsConfig.verifyPeer) {
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
    } else {
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    }
    
    if (loadTLSCertificates()) {
        m_sslSocket->setSslConfiguration(sslConfig);
    }
}

bool BareosDirector::loadTLSCertificates()
{
    if (!m_sslSocket) {
        return false;
    }
    
    bool success = true;
    
    // CA-Zertifikat laden
    if (!m_tlsConfig.caCertFile.isEmpty()) {
        QFile caFile(m_tlsConfig.caCertFile);
        if (caFile.open(QIODevice::ReadOnly)) {
            QSslCertificate caCert(&caFile, QSsl::Pem);
            if (!caCert.isNull()) {
                m_sslSocket->addCaCertificate(caCert);
                qDebug() << "Bareos Director: CA certificate loaded";
            }
            caFile.close();
        } else {
            qWarning() << "Bareos Director: Failed to load CA certificate:" << m_tlsConfig.caCertFile;
            success = false;
        }
    }
    
    // Client-Zertifikat laden
    if (!m_tlsConfig.certFile.isEmpty()) {
        QFile certFile(m_tlsConfig.certFile);
        if (certFile.open(QIODevice::ReadOnly)) {
            QSslCertificate clientCert(&certFile, QSsl::Pem);
            if (!clientCert.isNull()) {
                m_sslSocket->setLocalCertificate(clientCert);
                qDebug() << "Bareos Director: Client certificate loaded";
            }
            certFile.close();
        } else {
            qWarning() << "Bareos Director: Failed to load client certificate:" << m_tlsConfig.certFile;
            success = false;
        }
    }
    
    // Private Key laden
    if (!m_tlsConfig.keyFile.isEmpty()) {
        QFile keyFile(m_tlsConfig.keyFile);
        if (keyFile.open(QIODevice::ReadOnly)) {
            QByteArray keyData = keyFile.readAll();
            keyFile.close();
            
            // Versuche verschiedene Formate
            QSslKey key;
            
            // EC Key
            key = QSslKey(keyData, QSsl::Ec, QSsl::Pem, QSsl::PrivateKey);
            if (!key.isNull()) {
                m_sslSocket->setPrivateKey(key);
                qDebug() << "Bareos Director: Private key loaded (EC)";
                return success;
            }
            
            // RSA Key
            key = QSslKey(keyData, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);
            if (!key.isNull()) {
                m_sslSocket->setPrivateKey(key);
                qDebug() << "Bareos Director: Private key loaded (RSA)";
                return success;
            }
            
            qWarning() << "Bareos Director: Failed to load private key:" << m_tlsConfig.keyFile;
            success = false;
        }
    }
    
    return success;
}

void BareosDirector::authenticateBConsole(const QString &directorName, const QString &password)
{
    QString authCmd = QString("%1\n%2\n").arg(directorName, password);
    
    if (m_tlsConfig.enabled && m_sslSocket) {
        m_sslSocket->write(authCmd.toUtf8());
    } else if (m_tcpSocket) {
        m_tcpSocket->write(authCmd.toUtf8());
    }
}

QString BareosDirector::extractBareosVersion(const QString &response)
{
    // Extrahiere Version aus Begrüßungstext
    // Beispiel: "Bareos version 21.1.0"
    QRegularExpression re(R"(Bareos version ([\d.]+))");
    QRegularExpressionMatch match = re.match(response);
    
    if (match.hasMatch()) {
        return match.captured(1);
    }
    
    return "Unknown";
}
