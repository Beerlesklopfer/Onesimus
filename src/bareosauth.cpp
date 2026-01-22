/**
 * @file bareosauth.cpp
 * @brief Implementation of Qt-based Bareos Authentication
 *
 * @see bareosauth.h for class documentation
 */

#include "bareosauth.h"

#include <QDebug>
#include <QDataStream>
#include <QSslConfiguration>
#include <QSslCipher>
#include <QMessageAuthenticationCode>

// ============================================================================
// Constructor / Destructor
// ============================================================================

BareosAuth::BareosAuth(QSslSocket *socket, QObject *parent)
    : QObject(parent)
    , m_socket(socket)
    , m_authTimer(nullptr)
    , m_directorVersion(0)
    , m_tlsLocalNeed(BAREOS_TLS_NONE)
    , m_tlsRemoteNeed(BAREOS_TLS_NONE)
    , m_tlsPSKEnable(true)  // Default for Bareos 18.2+
    , m_tlsStarted(false)
    , m_authSuccess(false)
{
    Q_ASSERT(socket != nullptr);

    // Create timeout timer
    m_authTimer = new QTimer(this);
    m_authTimer->setSingleShot(true);
    connect(m_authTimer, &QTimer::timeout, this, &BareosAuth::onAuthTimeout);
}

BareosAuth::~BareosAuth()
{
    stopAuthTimeout();
}

// ============================================================================
// Main Authentication Entry Point
// ============================================================================

bool BareosAuth::authenticateDirector(const QString &directorName,
                                       const QString &consoleName,
                                       const QString &password,
                                       bool tlsEnable,
                                       bool tlsRequire,
                                       bool tlsPSKEnable)
{
    qDebug() << "========================================";
    qDebug() << "BAREOS AUTHENTICATION START";
    qDebug() << "========================================";
    qDebug() << "  Director:" << directorName;
    qDebug() << "  Console:" << consoleName;
    qDebug() << "  TLS Enable:" << tlsEnable;
    qDebug() << "  TLS Require:" << tlsRequire;
    qDebug() << "  TLS-PSK Enable:" << tlsPSKEnable;
    qDebug() << "  BareosAuth Version:" << version();
    qDebug() << "========================================";

    // Store authentication parameters
    m_directorName = directorName;
    m_consoleName = consoleName.isEmpty() ? QString(BAREOS_USERAGENT) : consoleName;
    m_password = password;
    m_tlsPSKEnable = tlsPSKEnable;

    // Calculate password MD5 for PSK
    if (!password.isEmpty()) {
        m_passwordMD5 = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);
        qDebug() << "  Password MD5 calculated for PSK";
    }

    // Calculate TLS needs
    m_tlsLocalNeed = calculateTLSNeed(tlsEnable, tlsRequire);
    qDebug() << "  Local TLS need:" << m_tlsLocalNeed;

    // Start authentication timeout
    startAuthTimeout();

    // Verify socket state
    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        m_errorMessage = "Socket not connected";
        qCritical() << "ERROR:" << m_errorMessage;
        emit authenticationFailed(m_errorMessage);
        return false;
    }

    emit statusMessage("Starting Bareos authentication...");

    // Bareos 18.2+ sequence: TLS-PSK first, then Hello + CRAM-MD5
    if (tlsPSKEnable && !password.isEmpty()) {
        qDebug() << "Using TLS-PSK authentication (Bareos 18.2+ style)";
        emit statusMessage("Setting up TLS-PSK...");

        if (!setupPSKTLS()) {
            // If PSK setup fails, authentication will fail in the slot
            return false;
        }

        // PSK handshake is asynchronous - authentication continues in onEncrypted()
        return true;
    }
    else {
        // Legacy mode: Hello + CRAM-MD5 + TLS (if required)
        qDebug() << "Using legacy authentication (pre-18.2 style)";
        emit statusMessage("Starting legacy authentication...");

        // Send Hello
        QString hello = QString("Hello %1 calling").arg(bashSpaces(m_consoleName));
        qDebug() << ">>> Sending:" << hello;

        if (!sendBareosMessage(hello)) {
            m_errorMessage = "Failed to send Hello message";
            emit authenticationFailed(m_errorMessage);
            return false;
        }

        // Perform CRAM-MD5 authentication
        if (!clientCramMD5Authenticate(m_password)) {
            // Error already set
            emit authenticationFailed(m_errorMessage);
            return false;
        }

        // Check TLS requirements
        BareosTLSRequirementResult tlsResult = testTLSRequirement();
        if (tlsResult != BAREOS_TLS_REQ_OK) {
            if (tlsResult == BAREOS_TLS_REQ_ERR_LOCAL) {
                m_errorMessage = "Remote server did not advertise required TLS support";
            } else {
                m_errorMessage = "Remote server requires TLS but local TLS is not available";
            }
            emit authenticationFailed(m_errorMessage);
            return false;
        }

        // TODO: Start certificate TLS if required (not PSK)
        // For now, we assume PSK is preferred

        // Wait for Director banner
        QString response;
        if (!readBareosMessage(response, 5000)) {
            m_errorMessage = "No response from Director after authentication";
            emit authenticationFailed(m_errorMessage);
            return false;
        }

        qDebug() << "<<< Director banner:" << response;

        // Parse version from banner
        if (!parseDirectorVersion(response)) {
            qWarning() << "Could not parse Director version from response";
        }

        // Success!
        stopAuthTimeout();
        m_authSuccess = true;

        qDebug() << "========================================";
        qDebug() << "✓ BAREOS AUTHENTICATION SUCCESSFUL";
        qDebug() << "  Director Version:" << m_directorVersionString;
        qDebug() << "========================================";

        emit authenticationSucceeded(m_directorVersion);
        return true;
    }
}

// ============================================================================
// TLS-PSK Setup
// ============================================================================

bool BareosAuth::setupPSKTLS()
{
    qDebug() << "Setting up TLS-PSK...";

    // Connect PSK signal
    connect(m_socket, &QSslSocket::preSharedKeyAuthenticationRequired,
            this, &BareosAuth::onPreSharedKeyAuthenticationRequired);

    // Connect encrypted signal for continuing after TLS
    connect(m_socket, &QSslSocket::encrypted,
            this, &BareosAuth::onEncrypted);

    // Connect SSL error handler
    connect(m_socket, QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
            this, &BareosAuth::onSslErrors);

    // Configure SSL for PSK
    QSslConfiguration sslConfig = m_socket->sslConfiguration();

    // Set protocol to TLS 1.2 or later
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);

    // For PSK, we don't verify peer certificates
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);

    // Set PSK ciphers - Bareos prefers these
    // Note: Qt may not support all of these depending on OpenSSL version
    QList<QSslCipher> pskCiphers;
    QList<QSslCipher> availableCiphers = QSslConfiguration::supportedCiphers();

    for (const QSslCipher &cipher : availableCiphers) {
        QString name = cipher.name();
        // Select PSK ciphers
        if (name.contains("PSK", Qt::CaseInsensitive)) {
            pskCiphers.append(cipher);
            qDebug() << "  Adding PSK cipher:" << name;
        }
    }

    if (pskCiphers.isEmpty()) {
        qWarning() << "WARNING: No PSK ciphers available!";
        qWarning() << "Make sure OpenSSL was compiled with PSK support";
        // Continue anyway - Qt might still handle it
    } else {
        sslConfig.setCiphers(pskCiphers);
    }

    m_socket->setSslConfiguration(sslConfig);

    // Start TLS handshake
    qDebug() << "Starting TLS-PSK handshake...";
    m_socket->startClientEncryption();

    return true;
}

void BareosAuth::onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator)
{
    qDebug() << "========================================";
    qDebug() << "PSK AUTHENTICATION REQUIRED";
    qDebug() << "========================================";

    handlePskAuthenticator(authenticator);
}

void BareosAuth::handlePskAuthenticator(QSslPreSharedKeyAuthenticator *authenticator)
{
    // Bareos PSK identity format: R_CONSOLE::ConsoleName
    QString identity = QString("%1::%2").arg(BAREOS_R_CONSOLE, m_consoleName);

    qDebug() << "  Identity hint from server:" << authenticator->identityHint();
    qDebug() << "  Setting identity:" << identity;
    qDebug() << "  PSK key length:" << m_passwordMD5.length() << "bytes";

    authenticator->setIdentity(identity.toUtf8());
    authenticator->setPreSharedKey(m_passwordMD5);

    qDebug() << "PSK credentials set";
}

void BareosAuth::onEncrypted()
{
    qDebug() << "========================================";
    qDebug() << "✓ TLS-PSK HANDSHAKE SUCCESSFUL";
    qDebug() << "========================================";
    qDebug() << "  Encrypted: true";
    qDebug() << "  Protocol:" << m_socket->sessionProtocol();
    qDebug() << "  Cipher:" << m_socket->sessionCipher().name();
    qDebug() << "========================================";

    m_tlsStarted = true;

    emit statusMessage("TLS-PSK established, sending Hello...");

    // Now send Hello message
    QString hello = QString("Hello %1 calling").arg(bashSpaces(m_consoleName));
    qDebug() << ">>> Sending:" << hello;

    if (!sendBareosMessage(hello)) {
        m_errorMessage = "Failed to send Hello message after TLS";
        emit authenticationFailed(m_errorMessage);
        return;
    }

    // Perform CRAM-MD5 authentication over TLS
    emit statusMessage("Performing CRAM-MD5 authentication...");

    if (!clientCramMD5Authenticate(m_password)) {
        emit authenticationFailed(m_errorMessage);
        return;
    }

    // Wait for Director banner
    QString response;
    if (!readBareosMessage(response, 5000)) {
        m_errorMessage = "No response from Director after authentication";
        emit authenticationFailed(m_errorMessage);
        return;
    }

    qDebug() << "<<< Director banner:" << response;

    // Parse version
    if (!parseDirectorVersion(response)) {
        qWarning() << "Could not parse Director version";
    }

    // Success!
    stopAuthTimeout();
    m_authSuccess = true;

    qDebug() << "========================================";
    qDebug() << "✓ BAREOS AUTHENTICATION SUCCESSFUL";
    qDebug() << "  Director Version:" << m_directorVersionString;
    qDebug() << "  Encryption:" << m_socket->sessionCipher().name();
    qDebug() << "========================================";

    emit authenticationSucceeded(m_directorVersion);
}

void BareosAuth::onSslErrors(const QList<QSslError> &errors)
{
    qWarning() << "SSL Errors during PSK handshake:";
    for (const QSslError &error : errors) {
        qWarning() << "  -" << error.errorString();
    }

    // For PSK, we expect some "errors" like no peer certificate
    // This is normal for PSK authentication
    if (m_tlsPSKEnable) {
        qDebug() << "Ignoring SSL errors for PSK mode (this is normal)";
        m_socket->ignoreSslErrors();
    }
}

// ============================================================================
// CRAM-MD5 Authentication
// ============================================================================

bool BareosAuth::clientCramMD5Authenticate(const QString &password)
{
    qDebug() << "Starting CRAM-MD5 authentication...";

    // Step 1: Respond to Director's challenge
    emit statusMessage("Responding to Director challenge...");

    if (!cramMD5Respond(password)) {
        return false;
    }

    // Step 2: Challenge the Director
    emit statusMessage("Challenging Director...");

    if (!cramMD5Challenge(password)) {
        return false;
    }

    qDebug() << "✓ CRAM-MD5 mutual authentication successful";
    return true;
}

bool BareosAuth::cramMD5Respond(const QString &password)
{
    // Read challenge from Director
    // Format: auth cram-md5 <random.timestamp@R_DIRECTOR::name> ssl=N
    QString challenge;
    if (!readBareosMessage(challenge, 10000)) {
        m_errorMessage = "Failed to receive CRAM-MD5 challenge from Director";
        return false;
    }

    qDebug() << "<<< Challenge:" << challenge;

    // Parse challenge
    // Bareos format: "auth cram-md5 <challenge> ssl=N" or "auth cram-md5c <challenge> ssl=N"
    QRegularExpression challengeRx(
        R"(auth cram-md5c?\s+<([^>]+)>\s+ssl=(\d+))",
        QRegularExpression::CaseInsensitiveOption
    );

    QRegularExpressionMatch match = challengeRx.match(challenge);
    if (!match.hasMatch()) {
        m_errorMessage = QString("Invalid challenge format: %1").arg(challenge);
        qCritical() << m_errorMessage;
        return false;
    }

    QString challengeString = match.captured(1);
    m_tlsRemoteNeed = match.captured(2).toInt();

    qDebug() << "  Challenge string:" << challengeString;
    qDebug() << "  Remote TLS need:" << m_tlsRemoteNeed;

    // Check if 'c' suffix present (compatibility mode - MD5 password)
    bool compatMode = challenge.contains("cram-md5c", Qt::CaseInsensitive);
    qDebug() << "  Compatibility mode (MD5 password):" << compatMode;

    // Compute HMAC-MD5 response
    QByteArray key;
    if (compatMode) {
        // Use MD5 of password
        key = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex();
    } else {
        key = password.toUtf8();
    }

    QByteArray hmac = hmacMD5(key, challengeString.toUtf8());
    QString response = bareosBase64Encode(hmac);

    qDebug() << ">>> Sending response:" << response;

    if (!sendBareosMessage(response)) {
        m_errorMessage = "Failed to send CRAM-MD5 response";
        return false;
    }

    // Wait for acknowledgment
    QString ack;
    if (!readBareosMessage(ack, 5000)) {
        m_errorMessage = "No acknowledgment after CRAM-MD5 response";
        return false;
    }

    qDebug() << "<<< Acknowledgment:" << ack;

    // Check for success
    if (!ack.contains("1000 OK", Qt::CaseInsensitive)) {
        m_errorMessage = QString("CRAM-MD5 response rejected: %1").arg(ack);
        qCritical() << m_errorMessage;
        return false;
    }

    qDebug() << "✓ Our CRAM-MD5 response accepted";
    return true;
}

bool BareosAuth::cramMD5Challenge(const QString &password)
{
    // Generate our challenge
    QString challenge = generateChallenge();
    QString challengeMsg = QString("auth cram-md5 <%1> ssl=%2")
                               .arg(challenge)
                               .arg(m_tlsLocalNeed);

    qDebug() << ">>> Sending challenge:" << challengeMsg;

    if (!sendBareosMessage(challengeMsg)) {
        m_errorMessage = "Failed to send CRAM-MD5 challenge";
        return false;
    }

    // Read Director's response
    QString response;
    if (!readBareosMessage(response, 5000)) {
        m_errorMessage = "Failed to receive Director's CRAM-MD5 response";
        return false;
    }

    qDebug() << "<<< Director response:" << response;

    // Verify Director's response
    QByteArray expectedHmac = hmacMD5(password.toUtf8(), challenge.toUtf8());
    QString expectedResponse = bareosBase64Encode(expectedHmac);

    // Also try with MD5 password (compatibility)
    QByteArray md5Password = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex();
    QByteArray expectedHmacCompat = hmacMD5(md5Password, challenge.toUtf8());
    QString expectedResponseCompat = bareosBase64Encode(expectedHmacCompat);

    QString trimmedResponse = response.trimmed();

    bool valid = (trimmedResponse == expectedResponse || trimmedResponse == expectedResponseCompat);

    if (!valid) {
        qWarning() << "CRAM-MD5 response mismatch!";
        qWarning() << "  Received:" << trimmedResponse;
        qWarning() << "  Expected (plain):" << expectedResponse;
        qWarning() << "  Expected (MD5):" << expectedResponseCompat;

        m_errorMessage = "Director's CRAM-MD5 response is invalid";
        return false;
    }

    qDebug() << "✓ Director's CRAM-MD5 response verified";

    // Send acknowledgment
    QString ack = "1000 OK auth";
    qDebug() << ">>> Sending:" << ack;

    if (!sendBareosMessage(ack)) {
        m_errorMessage = "Failed to send authentication acknowledgment";
        return false;
    }

    return true;
}

QString BareosAuth::generateChallenge()
{
    // Format: <random.timestamp@R_CONSOLE::ConsoleName>
    quint32 random = QRandomGenerator::global()->generate();
    qint64 timestamp = QDateTime::currentSecsSinceEpoch();

    QString challenge = QString("%1.%2@%3::%4")
                            .arg(random)
                            .arg(timestamp)
                            .arg(BAREOS_R_CONSOLE)
                            .arg(m_consoleName);

    return challenge;
}

// ============================================================================
// HMAC-MD5 Implementation
// ============================================================================

QByteArray BareosAuth::hmacMD5(const QByteArray &key, const QByteArray &data)
{
    // Use Qt's built-in HMAC implementation
    return QMessageAuthenticationCode::hash(data, key, QCryptographicHash::Md5);
}

// ============================================================================
// Protocol Methods
// ============================================================================

QByteArray BareosAuth::createBareosPacket(const QString &message)
{
    QByteArray packet;
    QByteArray msgData = message.toUtf8();

    // Bareos packet format: 4-byte signed big-endian length + data
    qint32 len = msgData.length();

    // Write length in big-endian
    packet.append((len >> 24) & 0xFF);
    packet.append((len >> 16) & 0xFF);
    packet.append((len >> 8) & 0xFF);
    packet.append(len & 0xFF);

    // Append message
    packet.append(msgData);

    return packet;
}

bool BareosAuth::sendBareosMessage(const QString &message)
{
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState) {
        qCritical() << "Cannot send - socket not connected";
        return false;
    }

    QByteArray packet = createBareosPacket(message);

    qint64 written = m_socket->write(packet);
    m_socket->flush();

    if (written != packet.size()) {
        qCritical() << "Failed to send complete packet. Written:" << written
                   << "Expected:" << packet.size();
        return false;
    }

    return true;
}

bool BareosAuth::readBareosMessage(QString &message, int timeoutMs)
{
    // Wait for data
    if (!waitForResponse(timeoutMs)) {
        return false;
    }

    // Read length (4 bytes, big-endian signed)
    while (m_socket->bytesAvailable() < 4) {
        if (!m_socket->waitForReadyRead(timeoutMs)) {
            qCritical() << "Timeout waiting for message length";
            return false;
        }
    }

    QByteArray lenData = m_socket->read(4);
    qint32 len = ((qint32)(quint8)lenData[0] << 24) |
                 ((qint32)(quint8)lenData[1] << 16) |
                 ((qint32)(quint8)lenData[2] << 8) |
                 ((qint32)(quint8)lenData[3]);

    // Negative length indicates signal (e.g., end of data)
    if (len < 0) {
        qDebug() << "Received signal packet:" << len;
        // Handle signals
        if (len == -1) {
            // End of data - try to read next message
            return readBareosMessage(message, timeoutMs);
        }
        message = QString("SIGNAL:%1").arg(len);
        return true;
    }

    if (len == 0) {
        message = "";
        return true;
    }

    if (len > 1000000) {  // Sanity check: 1MB max
        qCritical() << "Message length too large:" << len;
        return false;
    }

    // Read message data
    QByteArray msgData;
    while (msgData.size() < len) {
        if (!m_socket->waitForReadyRead(timeoutMs)) {
            qCritical() << "Timeout waiting for message data";
            return false;
        }
        msgData.append(m_socket->read(len - msgData.size()));
    }

    message = QString::fromUtf8(msgData);
    return true;
}

bool BareosAuth::waitForResponse(int timeoutMs)
{
    if (m_socket->bytesAvailable() > 0) {
        return true;
    }

    return m_socket->waitForReadyRead(timeoutMs);
}

// ============================================================================
// TLS Helper Methods
// ============================================================================

int BareosAuth::calculateTLSNeed(bool tlsEnable, bool tlsRequire)
{
    if (tlsRequire) {
        return BAREOS_TLS_REQUIRED;
    } else if (tlsEnable) {
        return BAREOS_TLS_OK;
    } else {
        return BAREOS_TLS_NONE;
    }
}

BareosTLSRequirementResult BareosAuth::testTLSRequirement()
{
    // Check if remote can meet our requirements
    if (m_tlsRemoteNeed < m_tlsLocalNeed &&
        m_tlsLocalNeed != BAREOS_TLS_OK &&
        m_tlsRemoteNeed != BAREOS_TLS_OK) {
        qWarning() << "Remote TLS level" << m_tlsRemoteNeed
                  << "does not meet local requirement" << m_tlsLocalNeed;
        return BAREOS_TLS_REQ_ERR_LOCAL;
    }

    // Check if we can meet remote's requirements
    if (m_tlsRemoteNeed > m_tlsLocalNeed &&
        m_tlsLocalNeed != BAREOS_TLS_OK &&
        m_tlsRemoteNeed != BAREOS_TLS_OK) {
        qWarning() << "Local TLS level" << m_tlsLocalNeed
                  << "does not meet remote requirement" << m_tlsRemoteNeed;
        return BAREOS_TLS_REQ_ERR_REMOTE;
    }

    return BAREOS_TLS_REQ_OK;
}

// ============================================================================
// Utility Methods
// ============================================================================

QString BareosAuth::bashSpaces(const QString &str)
{
    // Replace spaces with special character (0x1)
    QString result = str;
    result.replace(' ', QChar(0x01));
    return result;
}

void BareosAuth::startAuthTimeout()
{
    if (m_authTimer) {
        m_authTimer->start(BAREOS_AUTH_TIMEOUT);
    }
}

void BareosAuth::stopAuthTimeout()
{
    if (m_authTimer) {
        m_authTimer->stop();
    }
}

void BareosAuth::onAuthTimeout()
{
    qCritical() << "========================================";
    qCritical() << "✗ AUTHENTICATION TIMEOUT";
    qCritical() << "========================================";

    m_errorMessage = "Authentication timeout";
    m_authSuccess = false;

    emit authenticationFailed(m_errorMessage);
}

bool BareosAuth::parseDirectorVersion(const QString &response)
{
    // Bareos response format:
    // "1000 OK: bareos-dir Version: 21.0.0 (20 January 2021)"
    // or with encryption info:
    // "1000 OK auth Encryption: PSK-AES128-GCM-SHA256 TLSv1.2"
    // followed by:
    // "1000 OK: bareos-dir Version: 21.0.0 (20 January 2021)"

    QRegularExpression versionRx(
        R"(1000 OK[:\s]+(?:.*?)?Version:\s*(\d+)\.(\d+)\.(\d+))",
        QRegularExpression::CaseInsensitiveOption
    );

    QRegularExpressionMatch match = versionRx.match(response);
    if (match.hasMatch()) {
        int major = match.captured(1).toInt();
        int minor = match.captured(2).toInt();
        int patch = match.captured(3).toInt();

        m_directorVersion = major * 10000 + minor * 100 + patch;
        m_directorVersionString = QString("%1.%2.%3").arg(major).arg(minor).arg(patch);

        qDebug() << "Parsed Director version:" << m_directorVersionString
                << "(" << m_directorVersion << ")";
        return true;
    }

    // Try simpler pattern
    QRegularExpression simpleRx(R"(Version:\s*([\d.]+))");
    match = simpleRx.match(response);
    if (match.hasMatch()) {
        m_directorVersionString = match.captured(1);
        // Parse as single number for comparison
        QStringList parts = m_directorVersionString.split('.');
        if (parts.size() >= 2) {
            m_directorVersion = parts[0].toInt() * 10000 + parts[1].toInt() * 100;
            if (parts.size() >= 3) {
                m_directorVersion += parts[2].toInt();
            }
        }
        return true;
    }

    // If no version found, check if we at least got OK
    if (response.contains("1000 OK", Qt::CaseInsensitive)) {
        qDebug() << "Got OK response but could not parse version";
        m_directorVersionString = "unknown";
        m_directorVersion = 0;
        return true;
    }

    return false;
}
