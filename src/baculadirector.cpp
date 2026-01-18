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
    , m_backupSystem(Bacula)
    , m_socket(nullptr)
    , m_sslSocket(nullptr)
    , m_port(9101)
    , m_authenticated(false)
    , m_authChallengeSent(false)
    , m_waitingForDirectorResponse(false)
    , m_phase1Complete(false)
    , m_phase2Complete(false)
    , m_networkManager(nullptr)
    , m_authTimer(nullptr)
    , m_connected(false)
    , m_useApiMode(true)  // API-Modus standardmäßig aktivieren
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
    
    qDebug() << tr("Verbinde mit Bacula Director (%1:%2): ").arg(host).arg(port);
    
    if (m_tlsConfig.tlsEnable) {
        qDebug() << "TLS aktiviert - verwende verschlüsselte Verbindung";
        
        if (!setupTLSConnection()) {
            qDebug() << "Waiting for SSL handshake...";
            return;
        }        
        m_sslSocket->connectToHostEncrypted(host, port);

    } else {
        qDebug() << "Waiting for TCP connection...";
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
        if (m_tlsConfig.tlsEnable && m_sslSocket->state() == QAbstractSocket::ConnectedState) {
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
    qDebug() << "═══════════════════════════════════════════════════";
    qDebug() << "TCP CONNECTION ESTABLISHED";
    qDebug() << "  Host:" << m_host;
    qDebug() << "  Port:" << m_port;
    qDebug() << "═══════════════════════════════════════════════════";

    QString consName = "*UserAgent*";
    int version = 1;
    int tlspskLocalNeed = 0;
    
    QString helloMsg = QString("Hello %1 calling %2 tlspsk=%3\n")
                       .arg(consName)
                       .arg(version)
                       .arg(tlspskLocalNeed);

    if (!m_tlsConfig.tlsEnable) {
        qDebug() << ">>> SENDING HELLO TO DIRECTOR:";
        qDebug() << "    Message:" << helloMsg.trimmed();
        qDebug() << "    Length:" << helloMsg.length() << "bytes";
        qDebug() << "    HEX:" << helloMsg.toUtf8().toHex(' ');
        sendToDirector(helloMsg.toUtf8());
    }
    
    qDebug() << ">>> Waiting for Director response..." << m_socket->readAll();
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
    
    if (m_tlsConfig.tlsEnable && m_sslSocket->isEncrypted()) {
        data = m_sslSocket->readAll();
    } else {
        data = m_socket->readAll();
    }
    
    m_receiveBuffer.append(data);
    
    // Verarbeite vollständige Antworten
    // Authentifizierung passiert automatisch wenn Director seine Challenge sendet
    processBConsoleResponse(m_receiveBuffer);
    m_receiveBuffer.clear();
}

void BaculaDirector::onBConsoleError(QAbstractSocket::SocketError error)
{
    QString errorMsg = QString("Bconsole-Fehler: %1").arg(m_socket->errorString());
    qDebug() << errorMsg;
    emit connectionError(errorMsg);
}

// Authentifizierung ist event-driven - passiert automatisch in processBConsoleResponse()
// wenn der Director seine Challenge sendet

void BaculaDirector::processBConsoleResponse(const QByteArray &data)
{
    QString response = QString::fromUtf8(data).trimmed();
    
    // ===== DEBUG: Alle Rohdaten loggen =====
    qDebug() << "═══════════════════════════════════════════════════";
    qDebug() << "RAW DATA received (" << data.size() << "bytes):";
    qDebug() << "  HEX:" << data.toHex(' ');
    qDebug() << "  UTF8:" << response;
    qDebug() << "  Trimmed length:" << response.length();
    qDebug() << "  Authenticated:" << m_authenticated;
    qDebug() << "  Phase1 complete:" << m_phase1Complete;
    qDebug() << "  Phase2 complete:" << m_phase2Complete;
    qDebug() << "  Waiting for response:" << m_waitingForDirectorResponse;
    qDebug() << "═══════════════════════════════════════════════════";
    
    if (!m_authenticated) {
        // Phase 0: Optional TLS-PSK Negotiation
        // Director → "starttls tlspsk=<code>"
        if (response.startsWith("starttls tlspsk=")) {
            qDebug() << ">>> DETECTED: TLS-PSK Negotiation";
            QRegularExpression rx("starttls tlspsk=(\\d+)");
            QRegularExpressionMatch match = rx.match(response);
            if (match.hasMatch()) {
                int tlspskRemote = match.captured(1).toInt();
                qDebug() << "    TLS-PSK Remote need:" << tlspskRemote;
                qDebug() << "    TLS need:" << (tlspskRemote % 100);
                qDebug() << "    PSK need:" << (tlspskRemote / 100);
                // TODO: Handle TLS negotiation
            } else {
                qWarning() << "    ERROR: Could not parse TLS-PSK value";
            }
            return;
        }
        
        // Phase 1: Director sendet Challenge (nach Hello-Austausch)
        // Format: "auth cram-md5 <challenge> ssl=<tls_need>"
        if (response.startsWith("auth cram-md5")) {
            qDebug() << ">>> DETECTED: Director CRAM-MD5 Challenge";
            handleDirectorChallengePhase1(response);
            return;
        }
        
        // Phase 2b: Director bestätigt unsere Challenge
        // Format: "1000 OK auth"
        if (response == "1000 OK auth") {
            qDebug() << ">>> DETECTED: Director confirms authentication (1000 OK auth)";
            m_phase2Complete = true;
            qDebug() << "    Phase 1 complete:" << m_phase1Complete;
            qDebug() << "    Phase 2 complete:" << m_phase2Complete;
            
            // Wenn beide Phasen abgeschlossen sind
            if (m_phase1Complete && m_phase2Complete) {
                qDebug() << "    ✓ Bidirectional CRAM-MD5 authentication complete";
                qDebug() << "    Waiting for final Director response (1000 OK: ...)";
                // Warte auf finale "1000 OK: ..." Nachricht
            }
            return;
        }
        
        // Phase 1b: Director antwortet auf unsere Challenge
        // Dies ist eine Base64-Response (nicht "1000 OK auth")
        if (m_waitingForDirectorResponse && !response.startsWith("auth") && 
            !response.startsWith("1000") && !response.startsWith("1999") &&
            !response.startsWith("2000")) {
            qDebug() << ">>> DETECTED: Director response to our challenge (Base64)";
            qDebug() << "    Response length:" << response.length();
            qDebug() << "    First 20 chars:" << response.left(20);
            handleDirectorResponsePhase1b(response);
            return;
        }
        
        // Phase 3: Finale Director-Antwort nach erfolgreicher Authentifizierung
        // Format: "1000 OK: 10002 bacula-dir Version: 15.0.3 (...)"
        // oder:   "2000 OK Hello 10002" (FD-Modus)
        if (response.startsWith("1000 OK:") || response.startsWith("2000 OK Hello")) {
            qDebug() << ">>> DETECTED: Final Director response";
            qDebug() << "    Full response:" << response;
            
            // Parse Director-Version
            QRegularExpression versionRx("(1000|2000) OK.*?(\\d+)");
            QRegularExpressionMatch match = versionRx.match(response);
            if (match.hasMatch()) {
                int dirVersion = match.captured(2).toInt();
                qDebug() << "    Director version:" << dirVersion;
            } else {
                qDebug() << "    Could not parse Director version";
            }
            
            m_authenticated = true;
            m_connected = true;
            qDebug() << "    ✓ AUTHENTICATION SUCCESSFUL";
            qDebug() << "    Saving connection settings...";
            saveConnectionSettings();
            
            // Optional: Aktiviere API-Modus (wie BAT)
            if (m_useApiMode) {
                qDebug() << "    Activating API mode (.api 1)...";
                sendCommand(".api 1");
            }
            
            qDebug() << "    Emitting connected() signal";
            emit connected();
            return;
        }
        
        // Fehler
        if (response.contains("1999")) {
            qDebug() << ">>> DETECTED: Authentication FAILED (1999)";
            qWarning() << "    Error message:" << response;
            emit connectionError("Authentication failed: " + response);
            return;
        }
        
        // Unbekannte Nachricht während Authentifizierung
        qWarning() << ">>> UNKNOWN MESSAGE during authentication:";
        qWarning() << "    Response:" << response;
        qWarning() << "    Starts with auth:" << response.startsWith("auth");
        qWarning() << "    Starts with 1000:" << response.startsWith("1000");
        qWarning() << "    Starts with 1999:" << response.startsWith("1999");
        qWarning() << "    Starts with 2000:" << response.startsWith("2000");
        qWarning() << "    Waiting for response:" << m_waitingForDirectorResponse;
    } else {
        // Nach Authentifizierung: Normale Befehlsantworten
        qDebug() << ">>> POST-AUTH MESSAGE:";
        qDebug() << "    Response:" << response;
        emit commandResponse(response);
        
        if (m_lastCommand.startsWith("list jobs")) {
            qDebug() << "    Processing 'list jobs' response";
            QList<JobInfo> jobs;
            emit jobsReceived(jobs);
        }
    }
}

void BaculaDirector::handleDirectorChallengePhase1(const QString &challengeLine)
{
    // cram_md5_respond() Implementierung
    // Parse Challenge: "auth cram-md5[c] <challenge> ssl=<tls_need>"
    
    QRegularExpression rx("auth cram-md5c? <([^>]+)> ssl=(\\d+)");
    QRegularExpressionMatch match = rx.match(challengeLine);
    
    if (!match.hasMatch()) {
        // Alte Version ohne SSL
        QRegularExpression rxOld("auth cram-md5c? <([^>]+)>");
        match = rxOld.match(challengeLine);
        
        if (!match.hasMatch()) {
            qWarning() << "Invalid challenge format:" << challengeLine;
            emit connectionError("Invalid challenge format");
            return;
        }
    }
    
    QString challenge = match.captured(1);
    bool compatible = challengeLine.contains("cram-md5c");
    
    qDebug() << "Director challenge:" << challenge;
    qDebug() << "Compatible mode:" << compatible;
    
    // Berechne HMAC-MD5 Response auf Director-Challenge
    QByteArray hmac = calculateCramMd5Response(
        challenge.toUtf8(),
        m_password.toUtf8()
    );
    
    // Konvertiere zu Base64
    QByteArray response = hmac.toBase64();
    
    // Sende Response
    QString responseMsg = response + "\n";
    qDebug() << "Sending response to Director challenge:" << response;
    
    sendToDirector(responseMsg.toUtf8());
    
    // Jetzt müssen WIR eine Challenge senden (Phase 2)
    // Dies entspricht cram_md5_challenge() in cram_md5_respond()
    sendOurChallenge();
}

void BaculaDirector::sendOurChallenge()
{
    // cram_md5_challenge() Implementierung
    // Generiere Challenge: <random.timestamp@hostname>
    
    qint64 random1 = QRandomGenerator::global()->generate();
    qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    QString hostname = QHostInfo::localHostName();
    if (hostname.isEmpty()) {
        hostname = "onesimus-console";
    }
    
    m_ourChallenge = QString("<%1.%2@%3>").arg(random1).arg(timestamp).arg(hostname);
    
    // Sende Challenge an Director
    // Format: "auth cram-md5 <challenge> ssl=<tls_local_need>"
    int tlsLocalNeed = m_tlsConfig.tlsEnable ? 1 : 0;
    QString challengeMsg = QString("auth cram-md5 %1 ssl=%2\n")
                           .arg(m_ourChallenge)
                           .arg(tlsLocalNeed);
    
    qDebug() << "Sending our challenge to Director:" << m_ourChallenge;
    
    sendToDirector(challengeMsg.toUtf8());
    
    m_waitingForDirectorResponse = true;
}

void BaculaDirector::handleDirectorResponsePhase1b(const QString &response)
{
    // Director sendet Base64-Response auf unsere Challenge
    // Wir müssen validieren
    
    QByteArray receivedHmac = QByteArray::fromBase64(response.toUtf8());
    
    // Berechne erwartete HMAC
    QByteArray expectedHmac = calculateCramMd5Response(
        m_ourChallenge.toUtf8(),
        m_password.toUtf8()
    );
    
    bool ok = (receivedHmac == expectedHmac);
    
    if (ok) {
        qDebug() << "✓ Director response validated successfully";
        
        // Sende Bestätigung
        QString confirmMsg = "1000 OK auth\n";
        sendToDirector(confirmMsg.toUtf8());
        
        m_phase1Complete = true;
        m_waitingForDirectorResponse = false;
        
        // Wenn beide Phasen abgeschlossen sind
        if (m_phase1Complete && m_phase2Complete) {
            qDebug() << "✓ Bidirectional authentication successful!";
            m_authenticated = true;
            m_connected = true;
            saveConnectionSettings();
            emit connected();
        }
    } else {
        qWarning() << "✗ Director response validation failed!";
        qDebug() << "Expected:" << expectedHmac.toBase64();
        qDebug() << "Received:" << response;
        
        QString errorMsg = "1999 Authorization failed.\n";
        sendToDirector(errorMsg.toUtf8());
        
        emit connectionError("Director authentication failed");
    }
}

void BaculaDirector::sendToDirector(const QByteArray &data)
{
    if (m_tlsConfig.tlsEnable && m_sslSocket && m_sslSocket->isEncrypted()) {
        if(m_sslSocket->isOpen()){
            m_sslSocket->write(data);
            m_sslSocket->flush();
        } else {
            qCritical() << tr("Verbindung zu %1 nicht hergestellt").arg(m_host);
        }
    } else if (m_socket) {
        if(m_socket->isOpen()){
            m_socket->write(data);
            m_socket->flush();
        } else {
            qCritical() << tr("Verbindung zu %1 nicht hergestellt").arg(m_host);
        }
    }
}

void BaculaDirector::sendPasswordAuthentication()
{
    // Nicht mehr verwendet in modernem Bacula
    qWarning() << "sendPasswordAuthentication() called but CRAM-MD5 is always used";
}

QByteArray BaculaDirector::calculateCramMd5Response(const QByteArray &challenge, const QByteArray &password)
{
    // Bacula: hmac_md5((uint8_t *)chal, strlen(chal), (uint8_t *)password, strlen(password), hmac);
    // HMAC-MD5(challenge, password) nach RFC 2195
    
    QMessageAuthenticationCode hmac(QCryptographicHash::Md5, password);
    hmac.addData(challenge);
    
    return hmac.result();
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
    
    if (m_tlsConfig.tlsEnable && m_sslSocket->isEncrypted()) {
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
    if (m_tlsConfig.tlsVerifyPeer) {
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
    } else {
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    }

    m_sslSocket->setSslConfiguration(sslConfig);
    
    return true;
}

bool BaculaDirector::loadTLSCertificates(QSslConfiguration &sslConfig)
{
    // CA-Zertifikat laden (gemeinsam für Windows und Linux)
    if (m_tlsConfig.tlsCaCertFile->open(QIODevice::ReadOnly)) {
        
        QList<QSslCertificate> caCerts = QSslCertificate::fromDevice(m_tlsConfig.tlsCaCertFile.data(), QSsl::Pem);
        if (caCerts.isEmpty()) {
            qWarning() << "Keine gültigen CA-Zertifikate gefunden in:" << m_tlsConfig.tlsCaCertFile->fileName();
            m_tlsConfig.tlsCaCertFile->close();
            return false;
        }
        
        sslConfig.setCaCertificates(caCerts);
        m_tlsConfig.tlsCaCertFile->close();
        // qDebug() << "CA-Zertifikat geladen:" << m_tlsConfig.tlsCaCertFile->fileName();
    }
    else {
        qWarning() << "Kann CA-Zertifikat nicht öffnen:" << m_tlsConfig.tlsCaCertFile->fileName();
    }

#ifdef Q_OS_WINDOWS
    if (m_tlsConfig.tlsPfxFile->fileName().isEmpty()) {
        qWarning() << "Kein PFX-Pfad angegeben";
        return false;
    }

    if (m_tlsConfig.tlsPfxFile->open(QIODevice::ReadOnly)) {
        // QByteArray pfxData = m_tlsConfig.tlsPfxFile->readAll();

        // PKCS#12 importieren
        QSslCertificate certificate;
        QSslKey privateKey;
        QList<QSslCertificate> caCerts;

        // Qt 6.5+ verwendet QSslCertificate::importPkcs12 als static method
        bool imported = QSslCertificate::importPkcs12(
            m_tlsConfig.tlsPfxFile.data(),
            &privateKey,
            &certificate,
            &caCerts,
            m_tlsConfig.tlsPfxPassword.toUtf8()
            );

        m_tlsConfig.tlsPfxFile->close();

        if (!imported) {
            qCritical() << "Failed to import PKCS#12 certificate from" << m_tlsConfig.tlsPfxFile->fileName();
            qCritical() << "SSL Errors:" << QSslSocket::sslLibraryVersionString();
            m_socket->abort();
            return false;
        }

        if (certificate.isNull()) {
            qCritical() << "Imported private key is null";
            m_socket->abort();
            return false;
        }

        if (privateKey.isNull()) {
            qCritical() << "Imported private key is null";
            m_socket->abort();
            return false;
        }

        // Zertifikat und Key setzen
        sslConfig.setLocalCertificate(certificate);
        sslConfig.setPrivateKey(privateKey);

        // CA-Zertifikate hinzufügen (falls vorhanden)
        if (!caCerts.isEmpty()) {
            sslConfig.setCaCertificates(caCerts);
        }

        qDebug() << "✓ PFX-Zertifikat erfolgreich geladen:" << m_tlsConfig.tlsPfxFile->fileName();
        qDebug() << "  Zertifikat Subject:" << certificate.subjectDisplayName();
        qDebug() << "  Zertifikat Issuer:" << certificate.issuerDisplayName();
        qDebug() << "  Gültig von:" << certificate.effectiveDate().toString(Qt::ISODate);
        qDebug() << "  Gültig bis:" << certificate.expiryDate().toString(Qt::ISODate);
        qDebug() << "  Key-Algorithmus:" << (privateKey.algorithm() == QSsl::Rsa ? "RSA" :
                                                 privateKey.algorithm() == QSsl::Ec ? "EC" :
                                                 privateKey.algorithm() == QSsl::Dsa ? "DSA" : "Unknown");
        qDebug() << "  Key-Länge:" << privateKey.length() << "Bit";

        // Zusätzliche CA-Zertifikate aus PFX (falls vorhanden) zur Konfiguration hinzufügen
        if (!caCerts.isEmpty()) {
            QList<QSslCertificate> existingCAs = sslConfig.caCertificates();
            existingCAs.append(caCerts);
            sslConfig.setCaCertificates(existingCAs);
            qDebug() << "  Zusätzliche CA-Zertifikate aus PFX:" << caCerts.size();
        }

        return true;
    } else {
        qCritical() << "Failed to open PFX file:" << m_tlsConfig.tlsPfxFile->fileName();
        m_socket->abort();
        return false;
    }

#else
    // Client-Zertifikat laden
    if (!m_tlsConfig.tlsCertFile.fileName().isEmpty()) {
        if (!m_tlsConfig.tlsCertFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Kann Client-Zertifikat nicht öffnen:" << m_tlsConfig.tlsCertFile.fileName();
            return false;
        }

        QSslCertificate cert(&m_tlsConfig.tlsCertFile, QSsl::Pem);
        if (cert.isNull()) {
            qWarning() << "Ungültiges Client-Zertifikat:" << m_tlsConfig.tlsCertFile.fileName();
            certFile.close();
            return false;
        }

        sslConfig.setLocalCertificate(cert);
        certFile.close();
        qDebug() << "✓ Client-Zertifikat geladen:" << m_tlsConfig.tlsCertFile;
        qDebug() << "  Subject:" << cert.subjectDisplayName();
        qDebug() << "  Issuer:" << cert.issuerDisplayName();
    }

    // Private Key laden
    if (!m_tlsConfig.tlsKeyFile.fileName().isEmpty()) {
        if (!m_tlsConfig.tlsKeyFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Kann Private Key nicht öffnen:" << m_tlsConfig.tlsKeyFile.fileName();
            return false;
        }

        QByteArray keyData = m_tlsConfig.tlsKeyFile.readAll();
        m_tlsConfig.tlsKeyFile.close();

        // Versuche verschiedene Algorithmen
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
            if (key.isNull()) {
                key = QSslKey(keyData, QSsl::Rsa, QSsl::Der);
            }
        }

        if (key.isNull()) {
            qWarning() << "Ungültiger Private Key:" << m_tlsConfig.tlsKeyFile;
            return false;
        }

        sslConfig.setPrivateKey(key);
        qDebug() << "✓ Private Key geladen:" << m_tlsConfig.tlsKeyFile;
        qDebug() << "  Algorithmus:" << (key.algorithm() == QSsl::Ec ? "EC" :
                                             key.algorithm() == QSsl::Rsa ? "RSA" : "DSA");
        qDebug() << "  Länge:" << key.length() << "Bit";
    }

    return true;
#endif
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
    QString consName = "*UserAgent*";
    int version = 1;
    int tlspskLocalNeed = 0;

    QString helloMsg = QString("Hello %1 calling %2 tlspsk=%3\n")
                           .arg(consName)
                           .arg(version)
                           .arg(tlspskLocalNeed);

    qDebug() << "  TLS-Verschlüsselung erfolgreich hergestellt";
    qDebug() << "  Cipher:" << m_sslSocket->sessionCipher().name();
    qDebug() << "  Protokoll:" << m_sslSocket->sessionCipher().protocolString();
    qDebug() << "  TLS enabled:" << m_tlsConfig.tlsEnable;
    qDebug() << "  SSL-Support:" << QSslSocket::supportsSsl();
    qDebug() << "  OpenSSL-Version:" << QSslSocket::sslLibraryVersionString();
    qDebug() << "  Verfügbare Backends:" << QSslSocket::availableBackends();
    sendToDirector(helloMsg.toUtf8());

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
    QSettings settings("Bacula", "Onesimus");
    
    settings.beginGroup("Connection");
    settings.setValue("type", static_cast<int>(m_connectionType));
    
    if (m_connectionType == BConsole) {
        settings.setValue("bconsole_host", m_host);
        settings.setValue("bconsole_port", m_port);
        settings.setValue("bconsole_director", m_directorName);
        // Warnung: Passwort im Klartext gespeichert - für Produktion verschlüsseln!
        settings.setValue("bconsole_password", m_password);
        
        // TLS-Einstellungen speichern
        settings.setValue("tls_enabled", m_tlsConfig.tlsEnable);
        settings.setValue("tls_ca_cert_file", m_tlsConfig.tlsCaCertFile->fileName());

#ifdef Q_OS_WINDOWS
        settings.setValue("tls_pfx_file", m_tlsConfig.tlsPfxFile->fileName());
        settings.setValue("tls_pfx_password", m_tlsConfig.tlsPfxPassword);
#else
        settings.setValue("tls_cert_file", m_tlsConfig.tlsCertFile->fileName());
        settings.setValue("tls_key_file", m_tlsConfig.tlsKeyFile->fileName());
#endif

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
    QSettings settings("Bacula", "Onesimus");
    
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
        m_tlsConfig.tlsEnable = settings.value("tls_enabled", false).toBool();
        m_tlsConfig.tlsCaCertFile->setFileName(settings.value("tls_ca_cert_file").toString());

#ifdef Q_OS_WINDOWS
        m_tlsConfig.tlsPfxFile->setFileName(settings.value("tls_pfx_file").toString());
        m_tlsConfig.tlsPfxPassword = settings.value("tls_pfx_password").toString();
#else
        m_tlsConfig.tlsCertFile->setFileName(settings.value("tls_cert_file").toString());
        m_tlsConfig.tlsKeyFile->setFileName(settings.value("tls_key_file").toString());
#endif
        qDebug() << "Bconsole-Einstellungen geladen:" << m_host << ":" << m_port;
        if (m_tlsConfig.tlsEnable) {
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
    QSettings settings("Bacula", "Onesimus");
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

void BaculaDirector::setBackupSystem(BackupSystem system)
{
    if (m_backupSystem != system) {
        m_backupSystem = system;
        qDebug() << "Backup-System geändert zu:" << backupSystemName();
    }
}

BaculaDirector::BackupSystem BaculaDirector::backupSystem() const
{
    return m_backupSystem;
}

QString BaculaDirector::backupSystemName() const
{
    switch (m_backupSystem) {
        case Bacula:
            return "Bacula";
        case Bareos:
            return "Bareos";
        default:
            return "Unbekannt";
    }
}

