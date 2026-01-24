/**
 * @file bareosauth.cpp
 * @brief Implementation of Qt-based Bareos Authentication
 *
 * @see bareosauth.h for class documentation
 */

#include "bareosauth.h"

#include <QtEndian>
#include <QDebug>
#include <QDataStream>
#include <QSslConfiguration>
#include <QSslCipher>
#include <QMessageAuthenticationCode>

// ============================================================================
// Constructor / Destructor
// ============================================================================

BareosAuth::BareosAuth(QSslSocket *socket, QObject *parent)
    : QObject(parent), m_socket(socket), m_authTimer(nullptr), m_directorVersion(0), m_tlsLocalNeed(BAREOS_TLS_NONE), m_tlsRemoteNeed(BAREOS_TLS_NONE), m_tlsPSKEnable(true) // Default for Bareos 18.2+
      ,
    m_tlsStarted(false), m_authSuccess(false), m_cramState(BCramState::CRAM_IDLE)
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
                                      bool tlsVerifyPeer,
                                      bool tlsPSKEnable)
{
    if (password.isEmpty() && !tlsPSKEnable)
    {
        return false;
    }
    else
    {
        m_password = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);
    }

    // Store authentication parameters
    m_directorName = directorName;
    m_consoleName = consoleName.isEmpty() ? QString(BAREOS_USERAGENT) : consoleName;
    m_tlsLocalEnable = tlsEnable;
    m_tlsLocalRequire = tlsRequire;
    m_tlsVerifyPeer = tlsVerifyPeer;
    m_tlsPSKEnable = tlsPSKEnable;

    // Initializing asyncrounous processing
    QObject::connect(m_socket, &QSslSocket::readyRead, this, &BareosAuth::onReadyRead);
    QObject::connect(m_socket, &QSslSocket::bytesWritten, this, &BareosAuth::onBytesWritten);

    qDebug() << "========================================";
    qDebug() << "BAREOS AUTHENTICATION START";
    qDebug() << "========================================";
    qDebug() << "  Director           : " << directorName;
    qDebug() << "  Console            : " << consoleName;
    qDebug() << "  TLS Enable         : " << tlsEnable;
    qDebug() << "  TLS Require        : " << tlsRequire;
    qDebug() << "  TLS Require        : " << tlsVerifyPeer;
    qDebug() << "  TLS-PSK Enable     : " << tlsPSKEnable;
    qDebug() << "  BareosAuth Version : " << version();

    // Calculate TLS needs
    m_tlsLocalNeed = calculateTLSNeed(tlsEnable, tlsRequire);
    qDebug() << "  Local TLS need:" << m_tlsLocalNeed;

    // Start authentication timeout
    m_authTimer->start(BAREOS_AUTH_TIMEOUT);

    // Verify socket state
    if (m_socket->state() != QAbstractSocket::ConnectedState)
    {
        m_errorMessage = "Socket not connected";
        qCritical() << "ERROR:" << m_errorMessage;
        emit authenticationFailed(m_errorMessage);
        return false;
    }

    // Bareos 18.2+ sequence: TLS-PSK first, then Hello + CRAM-MD5
    if (tlsPSKEnable && !password.isEmpty())
    {

        emit statusMessage("Starting Bareos PSK authentication...");

        // Send Hello
        const QString hello = QString("Hello %1 calling version %2\n")
                                  .arg(bashSpaces(m_consoleName))
                                  .arg(BAREOS_VERSION_STR);
        qDebug() << ">>> Sending:" << hello;

        emit statusMessage(QString("Sending hello message: [%1]").arg(hello));

        if (bfsend(hello.toUtf8()) != BnetStatus::Ok)
        {
            m_errorMessage = "Failed to send Hello message";
            emit authenticationFailed(m_errorMessage);
            return false;
        }

        m_cramState = BCramState::CRAM_HELLO_SENT;

        emit statusMessage("Setting up TLS-PSK...");

        if (!setupPSKTLS())
        {
            // If PSK setup fails, authentication will fail in the slot
            return false;
        }

        // PSK handshake is asynchronous - authentication continues in onEncrypted()
        return true;
    }
    // Bareos <= 18.2.1 sequence: Hello + CRAM-MD5
    else if (!password.isEmpty())
    {
        // Legacy mode: Hello + CRAM-MD5 + TLS (if required)
        emit statusMessage("Starting legacy authentication...");

        // Send Hello
        const QString hello = QString("Hello %1 calling version 18.0.0\n")
                                  .arg(bashSpaces(m_consoleName));

        emit statusMessage(QString("Sending hello message: [%1]").arg(hello));

        if (bfsend(hello.toUtf8()) != BnetStatus::Ok)
        {
            m_errorMessage = "Failed to send Hello message";
            qDebug() << m_errorMessage;
            emit authenticationFailed(m_errorMessage);
            return false;
        }

        m_cramState = BCramState::CRAM_HELLO_SENT;


        // Check TLS requirements, if server requre this
        BareosTLSRequirementResult tlsResult = testTLSRequirement();
        if (tlsResult != BAREOS_TLS_REQ_OK)
        {
            if (tlsResult == BAREOS_TLS_REQ_ERR_LOCAL)
            {
                m_errorMessage = "Remote server did not advertise required TLS support";
            }
            else
            {
                m_errorMessage = "Remote server requires TLS but local TLS is not available";
            }
            emit authenticationFailed(m_errorMessage);
            return false;
        }

        // @TODO: Start certificate TLS if required (not PSK)
        // For now, we assume PSK is preferred

        // Wait for Director banner
        QByteArray response;
        if (brecv(response, 5000) != BnetStatus::Ok)
        {
            m_errorMessage = "No response from Director after authentication";
            emit authenticationFailed(m_errorMessage);
            return false;
        }

        qDebug() << "<<< Director banner:" << response;

        // Parse version from banner
        if (!parseDirectorVersion(response))
        {
            qWarning() << "Could not parse Director version from response";
        }

        // Success!
        stopAuthTimeout();
        m_authSuccess = true;

        qDebug() << "\n\n";
        qDebug() << "✓ BAREOS AUTHENTICATION SUCCESSFUL";
        qDebug() << "  Director Version:" << m_directorVersionString;
        qDebug() << "========================================";

        emit authenticationSucceeded(m_directorVersion);
        return true;
    }
    else
    {
        return false;
    }

    return false;
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
    connect(m_socket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
            this, &BareosAuth::onSslErrors);

    // Configure SSL for PSK
    QSslConfiguration sslConfig = m_socket->sslConfiguration();

    // Set protocol to TLS 1.2 or later
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);

    // For PSK, we don't verify peer certificates
    if (m_tlsVerifyPeer || m_tlsPSKEnable)
    {
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    }
    else
    {
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
    }

    // Set PSK ciphers - Bareos prefers these
    // Note: Qt may not support all of these depending on OpenSSL version
    QList<QSslCipher> pskCiphers;
    QList<QSslCipher> availableCiphers = QSslConfiguration::supportedCiphers();

    for (const QSslCipher &cipher : availableCiphers)
    {
        QString name = cipher.name();
        // Select PSK ciphers
        if (name.contains("PSK", Qt::CaseInsensitive))
        {
            pskCiphers.append(cipher);
            qDebug() << "  Adding PSK cipher:" << name;
        }
    }

    if (pskCiphers.isEmpty())
    {
        qWarning() << "WARNING: No PSK ciphers available!";
        qWarning() << "Make sure OpenSSL was compiled with PSK support";
        // Continue anyway - Qt might still handle it
    }
    else
    {
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
    qDebug() << "  PSK key length:" << m_password.length() << "bytes";

    authenticator->setIdentity(identity.toUtf8());
    authenticator->setPreSharedKey(m_password);

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

    // Perform CRAM-MD5 authentication over TLS
    emit statusMessage("Performing CRAM-MD5 authentication...");

    // Wait for Director banner
    QByteArray response;
    if (brecv(response, 5000) != BnetStatus::Ok)
    {
        m_errorMessage = "No response from Director after authentication";
        emit authenticationFailed(m_errorMessage);
        return;
    }

    qDebug() << "<<< Director banner:" << response;

    // Parse version
    if (!parseDirectorVersion(response))
    {
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
    for (const QSslError &error : errors)
    {
        qWarning() << "  -" << error.errorString();
    }

    // For PSK, we expect some "errors" like no peer certificate
    // This is normal for PSK authentication
    if (m_tlsPSKEnable)
    {
        qDebug() << "Ignoring SSL errors for PSK mode (this is normal)";
        m_socket->ignoreSslErrors();
    }
}

bool BareosAuth::cramMD5Response(const QByteArray challenge)
{
    qDebug() << challenge;
    // Generate our challenge
    const QString ownQualifiedName = QString("<%1::%2>")
                                    .arg(BAREOS_R_DIRECTOR)
                                    .arg(bashSpaces(m_directorName));;

    // Extract the challenge string
    // Bareos format: "auth cram-md5 <challenge> ssl=N" or "auth cram-md5c <challenge> ssl=N"
    QRegularExpression challengeRx(
        R"(auth cram-md5(c?)\s+<([^>]+)>\s+ssl=(\d+))",
        QRegularExpression::CaseInsensitiveOption);

    auto match = challengeRx.match(challenge);

    if (!match.hasMatch())
    {
        m_errorMessage = QString("Invalid challenge format: [%1]").arg(challenge);
        bfsend("1999 Authorization failed.\n");
        qCritical() << m_errorMessage;
        return false;
    }

    // Check if 'c' suffix present (compatibility mode - MD5 password)
    const bool isCompatible = !match.captured(1).isEmpty();
    const QByteArray challengechallengeQualifiedName = match.captured(2).toUtf8();
    m_tlsRemoteNeed = match.captured(3).toInt();

    QRegularExpression cmpRx("@([^>]+)>");
    auto cmpMatch = cmpRx.match(challengechallengeQualifiedName);

    if (cmpMatch.hasMatch()) {
        if( ownQualifiedName == cmpMatch.captured(1)){
            emit statusMessage("Performing CRAM-MD5 Dierector Auth");
        } else {
            emit authenticationFailed(QString("Challenges mismatch:\n Own:[%1]\nRemote: [%2]").arg(ownQualifiedName).arg(cmpMatch.captured(1)) );
            return false;
        }
    }

    qDebug() << '<'+challengechallengeQualifiedName +'>' << isCompatible;
    const QByteArray hmac = hmac_md5('<'+challengechallengeQualifiedName +'>', m_password);
    // const QByteArray hmac = hmac_md5(challengechallengeQualifiedName, m_password);

    qDebug() << hmac.toHex();
    QByteArray msg = base64Encode(hmac, isCompatible);
    msg.append('\n');
    qDebug() << msg;

    if (bfsend(msg) != BnetStatus::Ok)
    {
        m_errorMessage = "Failed to send CRAM-MD5 challenge";
        emit authenticationFailed(m_errorMessage);
        qDebug() << m_errorMessage;
        return false;
    }

    return true;
}

bool BareosAuth::cramMD5Challenge()
{
    // Generate our challenge
    const QString chal = QString("<%1.%2@%3::%4>")
                             .arg(QRandomGenerator::global()->generate())
                             .arg(QDateTime::currentSecsSinceEpoch())
                             .arg(BAREOS_R_CONSOLE)
                             .arg(bashSpaces(m_consoleName));

    // Format: <random.timestamp@consoleName> ssl=nnn qualified-name=R_CONSOLE::DIR-name>
    QString challengeMsg;
    bool isCompatiblelMode=false;
    challengeMsg = QString("auth cram-md5%1%2 ssl=%6\n")
                           .arg(isCompatiblelMode ? 'c' : ' ')
                           .arg(chal)
                           .arg(m_tlsLocalNeed);

    qDebug() << ">>> Sending challenge:  " << challengeMsg;

    if (bfsend(challengeMsg.toUtf8()) != BnetStatus::Ok)
    {
        m_errorMessage = "Failed to send CRAM-MD5 challenge";
        return false;
    }

    QByteArray hmac = hmac_md5(chal.toUtf8(), m_password);
    QByteArray challenge;

    if (brecv(challenge) != BnetStatus::Ok)
    {
        m_errorMessage = QString("Failed to read CRAM-MD5 challenge %1").arg(challenge);
        return false;
    }

    qDebug() << "<<< Received challenge: " << challenge;
    /*
  bool ok = bstrcmp(bs_->msg, host.c_str());
  if (ok) {
    Dmsg1(debuglevel_, "Authenticate OK %s\n", host.c_str());
  } else {
    BinToBase64(host.c_str(), MAXHOSTNAMELEN, (char*)hmac, 16, false);
    ok = bstrcmp(bs_->msg, host.c_str());
    if (!ok) {
      Dmsg2(debuglevel_, "Authenticate NOT OK: wanted %s, got %s\n",
            host.c_str(), bs_->msg);
    }
  }
  if (ok) {
    result = HandshakeResult::SUCCESS;
    bs_->fsend("1000 OK auth\n");
  } else {
    result = HandshakeResult::WRONG_HASH;
    bs_->fsend(T_("1999 Authorization failed.\n"));
    Bmicrosleep(bs_->sleep_time_after_authentication_error, 0);
  }
  return ok;
    */

    return false;
}

// ============================================================================
// Protocol Methods
// ============================================================================

BareosAuth::BnetStatus BareosAuth::bsend(const QByteArray &data)
{
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState)
    {
        qCritical() << "Cannot send - socket not connected";
        return BnetStatus::Error;
    }

    qint32 bytesWritten =  m_socket->write(data+"\n");
    m_socket->flush();

    qDebug() << bytesWritten << data;
    // if (bytesWritten == -1) {
    //     // Error writing
    //     return BnetStatus::Error;
    // }
    return BnetStatus::Ok;
}

BareosAuth::BnetStatus BareosAuth::bfsend(const QByteArray &data)
{
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState)
    {
        qCritical() << "Cannot send - socket not connected";
        return BnetStatus::Error;
    }

    // Prepare packet: 4-byte big-endian length + payload
    m_writeBuffer.clear();
    qint32 len = data.size();

    m_writeBuffer.append(static_cast<char>((len >> 24) & 0xFF));
    m_writeBuffer.append(static_cast<char>((len >> 16) & 0xFF));
    m_writeBuffer.append(static_cast<char>((len >> 8) & 0xFF));
    m_writeBuffer.append(static_cast<char>(len & 0xFF));

    m_writeBuffer.append(data); // Append payload

    // Send over the socket
    qint64 written = m_socket->write(m_writeBuffer);
    m_socket->flush();

    // if (written != m_writeBuffer.size())
    // {
    //     qCritical() << "Failed to send complete packet. Written:" << written
    //                 << "Expected:" << m_writeBuffer.size();
    //     return BnetStatus::Error;
    // }

    return BnetStatus::Ok;
}

BareosAuth::BnetStatus BareosAuth::brecv(QByteArray &data,
                                         int timeoutMs,
                                         int32_t maxPacketSize)
{
    data.clear();

    if (!m_socket)
    {
        return BnetStatus::Error;
    }

    // ---- Read header (int32, big endian) ----
    if (!m_socket->waitForReadyRead(timeoutMs))
    {
        return (m_socket->state() == QAbstractSocket::UnconnectedState)
                   ? BnetStatus::Eof
                   : BnetStatus::Error;
    }

    QByteArray header = m_socket->read(sizeof(int32_t));
    if (header.size() != sizeof(int32_t))
    {
        return BnetStatus::Error;
    }

    int32_t pktsiz = 0;
    std::memcpy(&pktsiz, header.constData(), sizeof(int32_t));
    pktsiz = qFromBigEndian(pktsiz);

    // ---- Empty message ----
    if (pktsiz == 0)
    {
        return BnetStatus::Empty;
    }

    // ---- Signal / terminate / invalid ----
    if (pktsiz < 0 || pktsiz > maxPacketSize)
    {
        if (pktsiz > maxPacketSize)
        {
            m_socket->disconnectFromHost();
            return BnetStatus::Terminated;
        }
        return BnetStatus::Signal;
    }

    // ---- Read payload ----
    data.resize(pktsiz);

    int32_t totalRead = 0;
    while (totalRead < pktsiz)
    {
        if (!m_socket->waitForReadyRead(timeoutMs))
        {
            return BnetStatus::Error;
        }

        const qint64 n = m_socket->read(
            data.data() + totalRead,
            pktsiz - totalRead);

        if (n <= 0)
        {
            return BnetStatus::Error;
        }

        totalRead += static_cast<int32_t>(n);
    }

    return BnetStatus::Ok;
}

// ============================================================================
// TLS Helper Methods
// ============================================================================

int BareosAuth::calculateTLSNeed(bool tlsEnable, bool tlsRequire)
{
    if (tlsRequire)
    {
        return BAREOS_TLS_REQUIRED;
    }
    else if (tlsEnable)
    {
        return BAREOS_TLS_OK;
    }
    else
    {
        return BAREOS_TLS_NONE;
    }
}

BareosTLSRequirementResult BareosAuth::testTLSRequirement()
{
    // Check if remote can meet our requirements
    if (m_tlsRemoteNeed < m_tlsLocalNeed &&
        m_tlsLocalNeed != BAREOS_TLS_OK &&
        m_tlsRemoteNeed != BAREOS_TLS_OK)
    {
        qWarning() << "Remote TLS level" << m_tlsRemoteNeed
                   << "does not meet local requirement" << m_tlsLocalNeed;
        return BAREOS_TLS_REQ_ERR_LOCAL;
    }

    // Check if we can meet remote's requirements
    if (m_tlsRemoteNeed > m_tlsLocalNeed &&
        m_tlsLocalNeed != BAREOS_TLS_OK &&
        m_tlsRemoteNeed != BAREOS_TLS_OK)
    {
        qWarning() << "Local TLS level" << m_tlsLocalNeed
                   << "does not meet remote requirement" << m_tlsRemoteNeed;
        return BAREOS_TLS_REQ_ERR_REMOTE;
    }

    return BAREOS_TLS_REQ_OK;
}

// ============================================================================
// Utility Methods
// ============================================================================

const QByteArray BareosAuth::hmac_md5(const QByteArray &text, const QByteArray &key)
{
    const int PAD_LEN = 64;
    const int SIG_LEN = 16;
    uint8_t hmac[SIG_LEN];
    uint8_t k_ipad[PAD_LEN], k_opad[PAD_LEN];
    uint8_t keyHash[SIG_LEN];

    const uint8_t *keyBytes = reinterpret_cast<const uint8_t*>(key.constData());
    int keyLen = key.size();

    // Schlüssel kürzen oder hashen, falls > PAD_LEN
    if (keyLen > PAD_LEN) {
        MD5(keyBytes, keyLen, keyHash);
        keyBytes = keyHash;
        keyLen = SIG_LEN;
    }

    memset(k_ipad, 0, PAD_LEN);
    memset(k_opad, 0, PAD_LEN);
    memcpy(k_ipad, keyBytes, keyLen);
    memcpy(k_opad, keyBytes, keyLen);

    for (int i = 0; i < PAD_LEN; ++i) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }

    // Inner MD5
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, k_ipad, PAD_LEN);
    MD5_Update(&ctx, reinterpret_cast<const uint8_t*>(text.constData()), text.size());
    MD5_Final(hmac, &ctx);

    // Outer MD5
    MD5_Init(&ctx);
    MD5_Update(&ctx, k_opad, PAD_LEN);
    MD5_Update(&ctx, hmac, SIG_LEN);
    MD5_Final(hmac, &ctx);

    return QByteArray(reinterpret_cast<const char*>(hmac), SIG_LEN);
}

const QByteArray BareosAuth::base64Encode(const QByteArray &data, bool isCompatible)
{

    char* buf = static_cast<char*>(malloc(MAXHOSTNAMELEN * sizeof(char)));
    int buflen = MAXHOSTNAMELEN;
    const char* bin = data.constData();
    int binlen = MD5ENCODEDLEN;

    uint32_t reg, save, mask;
    int rem, i;
    int j = 0;

    reg = 0;
    rem = 0;
    buflen--; /* allow for storing EOS */
    for (i = 0; i < binlen;) {
        if (rem < 6) {
            reg <<= 8;
            if (isCompatible) {
                reg |= (uint8_t)bin[i++];
            } else {
                reg |= (int8_t)bin[i++];
            }
            rem += 8;
        }
        save = reg;
        reg >>= (rem - 6);
        if (j < buflen) { buf[j++] = base64_digits[reg & 0x3F]; }
        reg = save;
        rem -= 6;
    }
    if (rem && j < buflen) {
        mask = (1 << rem) - 1;
        if (isCompatible) {
            buf[j++] = base64_digits[(reg & mask) << (6 - rem)];
        } else {
            buf[j++] = base64_digits[reg & mask];
        }
    }
    // buf[j] = 0;

    return QByteArray(buf, j);
}

QString BareosAuth::bashSpaces(const QString &str)
{
    // Replace spaces with special character (0x1)
    QString result = str;
    result.replace(' ', QChar(0x01));
    return result;
}

void BareosAuth::stopAuthTimeout()
{
    if (m_authTimer)
    {
        m_authTimer->stop();
    }
}

void BareosAuth::onReadyRead()
{
    m_readBuffer.append(m_socket->readAll());

    // --- State Machine Debugging ---
    switch (m_cramState)
    {
    case BCramState::CRAM_HELLO_SENT:        
        if (!cramMD5Response(m_readBuffer)){
            qDebug() << "cramMD5Response failed!";
        } else {
            m_cramState = BCramState::CRAM_WAITING_DIRECTOR_CHALLENGE;
            qDebug() << "[onReadyRead] State changed -> CRAM_WAITING_DIRECTOR_CHALLENGE";
        }
        break;

    case BCramState::CRAM_DIRECTOR_CHALLENGE_RECEIVED:
        // Hole MD5 vom Socket und Vergleiche mit egenem cram-md5
        // Bei Erfolg setze State CRAM_CLIENT_RESPONSE_SENT;
        m_cramState = BCramState::CRAM_CLIENT_RESPONSE_SENT;
        qDebug() << "[onReadyRead] tate changed -> CRAM_CLIENT_RESPONSE_SENT";
        break;

    case BCramState::CRAM_SENDING_CLIENT_CHALLENGE:
        /* code */
        break;

    case BCramState::CRAM_DIRECTOR_RESPONSE_RECIVED:
        /* code */
        break;

    case BCramState::CRAM_AUTHENTICATED:
        m_cramState = BCramState::CRAM_IDLE;
        qDebug() << "[onReadyRead] State changed -> CRAM_IDLE";

        // Disconnect events
        // QObject::connect(m_sslSocket, &QSslSocket::readyRead, this, &Director::onReadyRead);

        QObject::disconnect(m_socket, &QSslSocket::readyRead, this, &BareosAuth::onReadyRead);
        QObject::disconnect(m_socket, &QSslSocket::bytesWritten, this, &BareosAuth::onBytesWritten);
        break;

    case BCramState::CRAM_FAILED:
        /* code */
        break;

    default:
        break;
    }

    qDebug() << "onReadyRead=>BCramState: " << static_cast<int>(m_cramState);
}


void BareosAuth::onBytesWritten(qint64 bytesWritten)
{

    if (bytesWritten != m_readBuffer.size()){
        return;
    }

    switch (m_cramState)
    {
        // Hello has been sent
        case BCramState::CRAM_WAITING_DIRECTOR_CHALLENGE:
            m_cramState = BCramState::CRAM_DIRECTOR_CHALLENGE_RECEIVED;
            qDebug() << "[onBytesWritten] State changed -> CRAM_DIRECTOR_CHALLENGE_RECEIVED";

        case BCramState::CRAM_CLIENT_RESPONSE_SENT:
            m_cramState = BCramState::CRAM_SENDING_CLIENT_CHALLENGE;
            qDebug() << "[onBytesWritten] State changed -> CRAM_SENDING_CLIENT_CHALLENGE";
            break;

        case BCramState::CRAM_CLIENT_CHALLENGE_SENT:
            qDebug() << "[onBytesWritten] State changed -> ???";
            break;


        default:
            break;
    }

        qDebug() << "onBytesWritten=>BCramState: " << static_cast<int>(m_cramState);
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
        QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = versionRx.match(response);
    if (match.hasMatch())
    {
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
    if (match.hasMatch())
    {
        m_directorVersionString = match.captured(1);
        // Parse as single number for comparison
        QStringList parts = m_directorVersionString.split('.');
        if (parts.size() >= 2)
        {
            m_directorVersion = parts[0].toInt() * 10000 + parts[1].toInt() * 100;
            if (parts.size() >= 3)
            {
                m_directorVersion += parts[2].toInt();
            }
        }
        return true;
    }

    // If no version found, check if we at least got OK
    if (response.contains("1000 OK", Qt::CaseInsensitive))
    {
        qDebug() << "Got OK response but could not parse version";
        m_directorVersionString = "unknown";
        m_directorVersion = 0;
        return true;
    }

    return false;
}
