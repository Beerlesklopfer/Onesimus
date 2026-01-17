#include "baculadirector.h"
#include <QDebug>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QAuthenticator>
#include <QUrlQuery>
#include <QSslCipher>
#include <QSslError>

BaculaDirector::BaculaDirector(QObject *parent)
    : QObject(parent)
    , m_connectionType(BConsole)
    , m_socket(nullptr)
    , m_sslSocket(nullptr)
    , m_port(9101)
    , m_authenticated(false)
    , m_networkManager(nullptr)
    , m_authTimer(nullptr)
    , m_connected(false)
{
    m_socket = new QTcpSocket(this);
    m_sslSocket = new QSslSocket(this);
    m_networkManager = new QNetworkAccessManager(this);
    m_authTimer = new QTimer(this);
    m_authTimer->setInterval(3600000); // Token-Refresh alle 60 Minuten
    
    connect(m_socket, &QTcpSocket::connected, this, &BaculaDirector::onBConsoleConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &BaculaDirector::onBConsoleDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &BaculaDirector::onBConsoleReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &BaculaDirector::onBConsoleError);
    
    connect(m_sslSocket, &QSslSocket::encrypted, this, &BaculaDirector::onEncrypted);
    connect(m_sslSocket, &QSslSocket::connected, this, &BaculaDirector::onBConsoleConnected);
    connect(m_sslSocket, &QSslSocket::disconnected, this, &BaculaDirector::onBConsoleDisconnected);
    connect(m_sslSocket, &QSslSocket::readyRead, this, &BaculaDirector::onBConsoleReadyRead);
    connect(m_sslSocket, &QSslSocket::errorOccurred, this, &BaculaDirector::onBConsoleError);
    connect(m_sslSocket, QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors), 
            this, &BaculaDirector::onSslErrors);
    
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &BaculaDirector::onRestReplyFinished);
    connect(m_authTimer, &QTimer::timeout, this, &BaculaDirector::onAuthTimeout);
}

BaculaDirector::~BaculaDirector()
{
    disconnect();
}

void BaculaDirector::connectBConsole(const QString &host, int port, const QString &directorName, const QString &password, const TLSConfig &tlsConfig)
{
    m_connectionType = BConsole;
    m_host = host;
    m_port = port;
    m_directorName = directorName;
    m_password = password;
    m_tlsConfig = tlsConfig;
    m_authenticated = false;
    
    qDebug() << "Verbinde mit Bacula Director (bconsole):" << host << ":" << port;
    
    if (m_tlsConfig.enabled) {
        qDebug() << "TLS aktiviert - verwende verschlüsselte Verbindung";
        
        if (!setupTLSConnection()) {
            emit connectionError("TLS-Konfiguration fehlgeschlagen");
            return;
        }
        
        m_sslSocket->connectToHostEncrypted(host, port);
    } else {
        qDebug() << "TLS deaktiviert - verwende unverschlüsselte Verbindung";
        m_socket->connectToHost(host, port);
    }
    
    // Einstellungen werden nach erfolgreicher Verbindung in onBConsoleConnected gespeichert
}

void BaculaDirector::connectRestAPI(const QString &baseUrl, const QString &username, const QString &password)
{
    m_connectionType = RestAPI;
    m_restBaseUrl = baseUrl;
    m_restUsername = username;
    m_restPassword = password;
    
    qDebug() << "Verbinde mit Bacula REST API:" << baseUrl;
    
    // Authentifizierung über REST-API
    QJsonObject authData;
    authData["username"] = username;
    authData["password"] = password;
    
    restRequest("/api/v1/auth/login", "POST", authData);
}

void BaculaDirector::disconnect()
{
    if (m_connectionType == BConsole) {
        if (m_tlsConfig.enabled && m_sslSocket->state() == QAbstractSocket::ConnectedState) {
            sendCommand("quit");
            m_sslSocket->disconnectFromHost();
        } else if (m_socket->state() == QAbstractSocket::ConnectedState) {
            sendCommand("quit");
            m_socket->disconnectFromHost();
        }
    }
    
    m_connected = false;
    m_authenticated = false;
    m_authToken.clear();
    m_authTimer->stop();
}

bool BaculaDirector::isConnected() const
{
    return m_connected;
}

BaculaDirector::ConnectionType BaculaDirector::connectionType() const
{
    return m_connectionType;
}

// Bconsole-Verbindungsmethoden
void BaculaDirector::onBConsoleConnected()
{
    qDebug() << "Bconsole-Verbindung hergestellt";
    m_connected = true;
    saveConnectionSettings();
    emit connected();
}

void BaculaDirector::onBConsoleDisconnected()
{
    qDebug() << "Bconsole-Verbindung getrennt";
    m_connected = false;
    m_authenticated = false;
    emit disconnected();
}

void BaculaDirector::onBConsoleReadyRead()
{
    QByteArray data;
    
    if (m_tlsConfig.enabled && m_sslSocket->isEncrypted()) {
        data = m_sslSocket->readAll();
    } else {
        data = m_socket->readAll();
    }
    
    m_receiveBuffer.append(data);
    
    // Prüfe auf Authentifizierungsanfrage
    if (!m_authenticated && m_receiveBuffer.contains("cram-md5")) {
        authenticateBConsole();
        return;
    }
    
    // Verarbeite vollständige Antworten
    processBConsoleResponse(m_receiveBuffer);
    m_receiveBuffer.clear();
}

void BaculaDirector::onBConsoleError(QAbstractSocket::SocketError error)
{
    QString errorMsg = QString("Bconsole-Fehler: %1").arg(m_socket->errorString());
    qDebug() << errorMsg;
    emit connectionError(errorMsg);
}

void BaculaDirector::authenticateBConsole()
{
    // Implementierung der CRAM-MD5-Authentifizierung
    // Vereinfachte Version - in Produktion sollte echtes CRAM-MD5 verwendet werden
    QByteArray authPacket;
    authPacket.append("Hello ");
    authPacket.append(m_directorName.toUtf8());
    authPacket.append(" calling\n");
    
    if (m_tlsConfig.enabled && m_sslSocket->isEncrypted()) {
        m_sslSocket->write(authPacket);
        m_sslSocket->flush();
    } else {
        m_socket->write(authPacket);
        m_socket->flush();
    }
    
    m_authenticated = true;
}

void BaculaDirector::processBConsoleResponse(const QByteArray &data)
{
    QString response = QString::fromUtf8(data);
    emit commandResponse(response);
    
    // Parse spezifische Antworten basierend auf letztem Befehl
    if (m_lastCommand.startsWith("list jobs")) {
        // Parse Job-Liste (vereinfacht)
        QList<JobInfo> jobs;
        // Parsing-Logik hier implementieren
        emit jobsReceived(jobs);
    }
}

QByteArray BaculaDirector::prepareBConsolePacket(const QString &command)
{
    QByteArray packet;
    packet.append(command.toUtf8());
    packet.append("\n");
    return packet;
}

// Bconsole-Befehle
void BaculaDirector::sendCommand(const QString &command)
{
    if (!m_connected || m_connectionType != BConsole) {
        qWarning() << "Nicht mit Bconsole verbunden";
        return;
    }
    
    m_lastCommand = command;
    QByteArray packet = prepareBConsolePacket(command);
    
    if (m_tlsConfig.enabled && m_sslSocket->isEncrypted()) {
        m_sslSocket->write(packet);
        m_sslSocket->flush();
    } else {
        m_socket->write(packet);
        m_socket->flush();
    }
}

void BaculaDirector::listJobs(int limit)
{
    QString cmd = QString("list jobs limit=%1").arg(limit);
    sendCommand(cmd);
}

void BaculaDirector::listClients()
{
    sendCommand("list clients");
}

void BaculaDirector::listPools()
{
    sendCommand("list pools");
}

void BaculaDirector::listVolumes()
{
    sendCommand("list volumes");
}

void BaculaDirector::showJobDetails(int jobId)
{
    QString cmd = QString("list jobid=%1").arg(jobId);
    sendCommand(cmd);
}

void BaculaDirector::runJob(const QString &jobName)
{
    QString cmd = QString("run job=\"%1\" yes").arg(jobName);
    sendCommand(cmd);
}

void BaculaDirector::cancelJob(int jobId)
{
    QString cmd = QString("cancel jobid=%1").arg(jobId);
    sendCommand(cmd);
}

void BaculaDirector::restoreFiles(const QString &clientName, const QString &fileSet)
{
    QString cmd = QString("restore client=\"%1\" fileset=\"%2\" select all done yes")
                      .arg(clientName, fileSet);
    sendCommand(cmd);
}

void BaculaDirector::statusDirector()
{
    sendCommand("status director");
}

void BaculaDirector::statusStorage(const QString &storageName)
{
    QString cmd = QString("status storage=\"%1\"").arg(storageName);
    sendCommand(cmd);
}

void BaculaDirector::statusClient(const QString &clientName)
{
    QString cmd = QString("status client=\"%1\"").arg(clientName);
    sendCommand(cmd);
}

// REST-API-Methoden
void BaculaDirector::restRequest(const QString &endpoint, const QString &method, const QJsonObject &data)
{
    QUrl url(m_restBaseUrl + endpoint);
    QNetworkRequest request(url);
    
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    if (!m_authToken.isEmpty()) {
        setRestAuthHeader(request);
    }
    
    QNetworkReply *reply = nullptr;
    
    if (method == "GET") {
        reply = m_networkManager->get(request);
    } else if (method == "POST") {
        QJsonDocument doc(data);
        reply = m_networkManager->post(request, doc.toJson());
    } else if (method == "PUT") {
        QJsonDocument doc(data);
        reply = m_networkManager->put(request, doc.toJson());
    } else if (method == "DELETE") {
        reply = m_networkManager->deleteResource(request);
    }
    
    if (reply) {
        reply->setProperty("endpoint", endpoint);
        reply->setProperty("method", method);
    }
}

void BaculaDirector::setRestAuthHeader(QNetworkRequest &request)
{
    QString authHeader = QString("Bearer %1").arg(m_authToken);
    request.setRawHeader("Authorization", authHeader.toUtf8());
}

void BaculaDirector::onRestReplyFinished(QNetworkReply *reply)
{
    QString endpoint = reply->property("endpoint").toString();
    
    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = QString("REST-API-Fehler: %1").arg(reply->errorString());
        qDebug() << errorMsg;
        emit connectionError(errorMsg);
        reply->deleteLater();
        return;
    }
    
    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    
    // Handle authentication
    if (endpoint == "/api/v1/auth/login") {
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            m_authToken = obj.value("token").toString();
            if (!m_authToken.isEmpty()) {
                m_connected = true;
                m_authTimer->start();
                saveConnectionSettings();
                emit connected();
            } else {
                emit authenticationFailed();
            }
        }
    }
    // Handle jobs
    else if (endpoint.contains("/jobs")) {
        parseJobsResponse(doc);
    }
    // Handle clients
    else if (endpoint.contains("/clients")) {
        parseClientsResponse(doc);
    }
    // Handle volumes
    else if (endpoint.contains("/volumes")) {
        parseVolumesResponse(doc);
    }
    
    reply->deleteLater();
}

void BaculaDirector::parseJobsResponse(const QJsonDocument &doc)
{
    QList<JobInfo> jobs;
    
    if (doc.isArray()) {
        QJsonArray array = doc.array();
        for (const QJsonValue &value : array) {
            QJsonObject obj = value.toObject();
            JobInfo job;
            job.jobId = obj.value("jobid").toInt();
            job.name = obj.value("name").toString();
            job.type = obj.value("type").toString();
            job.level = obj.value("level").toString();
            job.clientName = obj.value("client").toString();
            job.status = obj.value("jobstatus").toString();
            job.jobBytes = obj.value("jobbytes").toVariant().toLongLong();
            job.jobFiles = obj.value("jobfiles").toVariant().toLongLong();
            jobs.append(job);
        }
    }
    
    emit jobsReceived(jobs);
}

void BaculaDirector::parseClientsResponse(const QJsonDocument &doc)
{
    QList<ClientInfo> clients;
    
    if (doc.isArray()) {
        QJsonArray array = doc.array();
        for (const QJsonValue &value : array) {
            QJsonObject obj = value.toObject();
            ClientInfo client;
            client.name = obj.value("name").toString();
            client.address = obj.value("address").toString();
            client.port = obj.value("port").toInt();
            client.os = obj.value("uname").toString();
            clients.append(client);
        }
    }
    
    emit clientsReceived(clients);
}

void BaculaDirector::parseVolumesResponse(const QJsonDocument &doc)
{
    QList<VolumeInfo> volumes;
    
    if (doc.isArray()) {
        QJsonArray array = doc.array();
        for (const QJsonValue &value : array) {
            QJsonObject obj = value.toObject();
            VolumeInfo volume;
            volume.volumeName = obj.value("volumename").toString();
            volume.poolName = obj.value("pool").toString();
            volume.mediaType = obj.value("mediatype").toString();
            volume.status = obj.value("volstatus").toString();
            volume.volumeBytes = obj.value("volbytes").toVariant().toLongLong();
            volumes.append(volume);
        }
    }
    
    emit volumesReceived(volumes);
}

void BaculaDirector::restGetJobs(const QString &filter)
{
    QString endpoint = "/api/v1/jobs";
    if (!filter.isEmpty()) {
        endpoint += "?filter=" + filter;
    }
    restRequest(endpoint, "GET");
}

void BaculaDirector::restGetClients()
{
    restRequest("/api/v1/clients", "GET");
}

void BaculaDirector::restGetPools()
{
    restRequest("/api/v1/pools", "GET");
}

void BaculaDirector::restGetVolumes()
{
    restRequest("/api/v1/volumes", "GET");
}

void BaculaDirector::restGetJobDetails(int jobId)
{
    QString endpoint = QString("/api/v1/jobs/%1").arg(jobId);
    restRequest(endpoint, "GET");
}

void BaculaDirector::restRunJob(const QString &jobName, const QJsonObject &parameters)
{
    QJsonObject data;
    data["job"] = jobName;
    if (!parameters.isEmpty()) {
        data["parameters"] = parameters;
    }
    restRequest("/api/v1/jobs/run", "POST", data);
}

void BaculaDirector::restCancelJob(int jobId)
{
    QString endpoint = QString("/api/v1/jobs/%1/cancel").arg(jobId);
    restRequest(endpoint, "POST");
}

void BaculaDirector::restGetFilesets()
{
    restRequest("/api/v1/filesets", "GET");
}

void BaculaDirector::restGetSchedules()
{
    restRequest("/api/v1/schedules", "GET");
}

void BaculaDirector::onAuthTimeout()
{
    // Token-Refresh
    if (m_connectionType == RestAPI) {
        connectRestAPI(m_restBaseUrl, m_restUsername, m_restPassword);
    }
}

bool BaculaDirector::setupTLSConnection()
{
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    
    if (!loadTLSCertificates(sslConfig)) {
        return false;
    }
    
    // SSL/TLS-Protokoll setzen
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);
    
    // Peer-Verification aktivieren
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
    
    m_sslSocket->setSslConfiguration(sslConfig);
    
    return true;
}

bool BaculaDirector::loadTLSCertificates(QSslConfiguration &sslConfig)
{
    // CA-Zertifikat laden
    if (!m_tlsConfig.caCertFile.isEmpty()) {
        QFile caCertFile(m_tlsConfig.caCertFile);
        if (!caCertFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Kann CA-Zertifikat nicht öffnen:" << m_tlsConfig.caCertFile;
            return false;
        }
        
        QList<QSslCertificate> caCerts = QSslCertificate::fromDevice(&caCertFile, QSsl::Pem);
        if (caCerts.isEmpty()) {
            qWarning() << "Keine gültigen CA-Zertifikate gefunden in:" << m_tlsConfig.caCertFile;
            caCertFile.close();
            return false;
        }
        
        sslConfig.setCaCertificates(caCerts);
        caCertFile.close();
        qDebug() << "CA-Zertifikat geladen:" << m_tlsConfig.caCertFile;
    }
    
    // Client-Zertifikat laden
    if (!m_tlsConfig.certFile.isEmpty()) {
        QFile certFile(m_tlsConfig.certFile);
        if (!certFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Kann Client-Zertifikat nicht öffnen:" << m_tlsConfig.certFile;
            return false;
        }
        
        QSslCertificate cert(&certFile, QSsl::Pem);
        if (cert.isNull()) {
            qWarning() << "Ungültiges Client-Zertifikat:" << m_tlsConfig.certFile;
            certFile.close();
            return false;
        }
        
        sslConfig.setLocalCertificate(cert);
        certFile.close();
        qDebug() << "Client-Zertifikat geladen:" << m_tlsConfig.certFile;
    }
    
    // Private Key laden
    if (!m_tlsConfig.keyFile.isEmpty()) {
        QFile keyFile(m_tlsConfig.keyFile);
        if (!keyFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Kann Private Key nicht öffnen:" << m_tlsConfig.keyFile;
            return false;
        }
        
        QByteArray keyData = keyFile.readAll();
        keyFile.close();
        
        // Versuche verschiedene Algorithmen zu erkennen
        QSslKey key;
        
        // Zuerst EC (Elliptic Curve) versuchen
        key = QSslKey(keyData, QSsl::Ec, QSsl::Pem);
        if (key.isNull()) {
            // Dann RSA versuchen
            key = QSslKey(keyData, QSsl::Rsa, QSsl::Pem);
        }
        if (key.isNull()) {
            // Dann DSA versuchen
            key = QSslKey(keyData, QSsl::Dsa, QSsl::Pem);
        }
        if (key.isNull()) {
            // DER-Format versuchen
            key = QSslKey(keyData, QSsl::Ec, QSsl::Der);
        }
        if (key.isNull()) {
            key = QSslKey(keyData, QSsl::Rsa, QSsl::Der);
        }
        
        if (key.isNull()) {
            qWarning() << "Ungültiger Private Key:" << m_tlsConfig.keyFile;
            return false;
        }
        
        sslConfig.setPrivateKey(key);
        qDebug() << "Private Key geladen:" << m_tlsConfig.keyFile 
                 << "Algorithmus:" << (key.algorithm() == QSsl::Ec ? "EC" : 
                                      key.algorithm() == QSsl::Rsa ? "RSA" : "DSA");
    }
    
    return true;
}

void BaculaDirector::onSslErrors(const QList<QSslError> &errors)
{
    QString errorMsg = "SSL-Fehler:\n";
    for (const QSslError &error : errors) {
        errorMsg += error.errorString() + "\n";
        qWarning() << "SSL-Fehler:" << error.errorString();
    }
    
    emit connectionError(errorMsg);
    
    // Optional: Ignoriere bestimmte Fehler (nur für Entwicklung!)
    // m_sslSocket->ignoreSslErrors();
}

void BaculaDirector::onEncrypted()
{
    qDebug() << "TLS-Verschlüsselung erfolgreich hergestellt";
    qDebug() << "Cipher:" << m_sslSocket->sessionCipher().name();
    qDebug() << "Protokoll:" << m_sslSocket->sessionCipher().protocolString();
}

void BaculaDirector::setTLSConfig(const TLSConfig &config)
{
    m_tlsConfig = config;
}

BaculaDirector::TLSConfig BaculaDirector::tlsConfig() const
{
    return m_tlsConfig;
}

void BaculaDirector::saveConnectionSettings()
{
    QSettings settings("Bacula", "BaculaQtUI");
    
    settings.beginGroup("Connection");
    settings.setValue("type", static_cast<int>(m_connectionType));
    
    if (m_connectionType == BConsole) {
        settings.setValue("bconsole_host", m_host);
        settings.setValue("bconsole_port", m_port);
        settings.setValue("bconsole_director", m_directorName);
        // Warnung: Passwort im Klartext gespeichert - für Produktion verschlüsseln!
        settings.setValue("bconsole_password", m_password);
        
        // TLS-Einstellungen speichern
        settings.setValue("tls_enabled", m_tlsConfig.enabled);
        settings.setValue("tls_ca_cert", m_tlsConfig.caCertFile);
        settings.setValue("tls_cert", m_tlsConfig.certFile);
        settings.setValue("tls_key", m_tlsConfig.keyFile);
    } else {
        settings.setValue("rest_baseurl", m_restBaseUrl);
        settings.setValue("rest_username", m_restUsername);
        // Warnung: Passwort im Klartext gespeichert - für Produktion verschlüsseln!
        settings.setValue("rest_password", m_restPassword);
    }
    
    settings.endGroup();
    
    qDebug() << "Verbindungseinstellungen gespeichert";
}

void BaculaDirector::loadConnectionSettings()
{
    QSettings settings("Bacula", "BaculaQtUI");
    
    settings.beginGroup("Connection");
    
    if (!settings.contains("type")) {
        qDebug() << "Keine gespeicherten Verbindungseinstellungen gefunden";
        settings.endGroup();
        return;
    }
    
    m_connectionType = static_cast<ConnectionType>(settings.value("type").toInt());
    
    if (m_connectionType == BConsole) {
        m_host = settings.value("bconsole_host", "localhost").toString();
        m_port = settings.value("bconsole_port", 9101).toInt();
        m_directorName = settings.value("bconsole_director", "bacula-dir").toString();
        m_password = settings.value("bconsole_password").toString();
        
        // TLS-Einstellungen laden
        m_tlsConfig.enabled = settings.value("tls_enabled", false).toBool();
        m_tlsConfig.caCertFile = settings.value("tls_ca_cert").toString();
        m_tlsConfig.certFile = settings.value("tls_cert").toString();
        m_tlsConfig.keyFile = settings.value("tls_key").toString();
        
        qDebug() << "Bconsole-Einstellungen geladen:" << m_host << ":" << m_port;
        if (m_tlsConfig.enabled) {
            qDebug() << "TLS aktiviert";
        }
    } else {
        m_restBaseUrl = settings.value("rest_baseurl", "http://localhost:9101").toString();
        m_restUsername = settings.value("rest_username", "admin").toString();
        m_restPassword = settings.value("rest_password").toString();
        
        qDebug() << "REST-API-Einstellungen geladen:" << m_restBaseUrl;
    }
    
    settings.endGroup();
}

bool BaculaDirector::hasStoredConnection() const
{
    QSettings settings("Bacula", "BaculaQtUI");
    return settings.contains("Connection/type");
}

BaculaDirector::JobStatus BaculaDirector::parseJobStatus(const QString &status)
{
    if (status == "C") return Created;
    if (status == "R") return Running;
    if (status == "B") return Blocked;
    if (status == "T") return Terminated;
    if (status == "W") return Waiting;
    if (status == "f") return Successful;
    if (status == "E") return Error;
    if (status == "e") return Fatal;
    if (status == "A") return Canceled;
    return Unknown;
}

QString BaculaDirector::jobStatusToString(JobStatus status)
{
    switch (status) {
        case Created: return "Erstellt";
        case Running: return "Läuft";
        case Blocked: return "Blockiert";
        case Terminated: return "Beendet";
        case Waiting: return "Wartet";
        case Successful: return "Erfolgreich";
        case Error: return "Fehler";
        case Fatal: return "Kritischer Fehler";
        case Canceled: return "Abgebrochen";
        default: return "Unbekannt";
    }
}
