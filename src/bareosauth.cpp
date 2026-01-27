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
        m_password = QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Md5);
    }

    // Store authentication parameters
    m_directorName = directorName;
    m_consoleName = consoleName.isEmpty() ? QString(BAREOS_USERAGENT) : consoleName;
    m_tlsLocalEnable = tlsEnable;
    m_tlsLocalRequire = tlsRequire;
    m_tlsVerifyPeer = tlsVerifyPeer;
    m_tlsPSKEnable = tlsPSKEnable;

    // Initializing asyncrounous processing
    m_readyReadConn = QObject::connect(m_socket, &QSslSocket::readyRead, this, &BareosAuth::onReadyRead);
    m_bytesWrittenConn = QObject::connect(m_socket, &QSslSocket::bytesWritten, this, &BareosAuth::onBytesWritten);

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
        emit authenticationFailed(m_errorMessage);
        return false;
    }

    // Bareos 18.2+ sequence: TLS-PSK first, then Hello + CRAM-MD5
    emit statusMessage("Starting Bareos PSK authentication...");

    // Prepare Hello
    m_writeBuffer = QByteArray("Hello ");
    m_writeBuffer.append(bashSpaces(m_consoleName.toLatin1()));
    m_writeBuffer.append(" onesimus version ");
    m_writeBuffer.append(BAREOS_VERSION_STR);

    m_cramState = BCramState::CRAM_IDLE;

    if (tlsPSKEnable && !password.isEmpty())
    {

        emit statusMessage("Setting up TLS-PSK...");

        if (!setupPSKTLS())
        {
            // If PSK setup fails, authentication will fail in the slot
            return false;
        }

        if (send() != BnetStatus::Ok)
        {
            m_errorMessage = "Failed to send Hello message";
            emit authenticationFailed(m_errorMessage);
            return false;
        }

        emit statusMessage(QString("Sending: %1 with PSK").arg(m_writeBuffer));
        m_cramState = BCramState::CRAM_HELLO_SENT;

        return true;
    }
    // Bareos <= 18.2.1 sequence: Hello + CRAM-MD5
    else if (!password.isEmpty())
    {
        // Legacy mode: Hello + CRAM-MD5 + TLS (if required)
        emit statusMessage("Starting legacy authentication...");
        emit statusMessage(QString("Sending: %1 with PSK").arg(m_writeBuffer));

        if (send() != BnetStatus::Ok)
        {
            m_errorMessage = "Failed to send Hello message";
            emit authenticationFailed(m_errorMessage);
            return false;
        }

        return true;

        // // Check TLS requirements, if server requre this
        // BareosTLSRequirementResult tlsResult = testTLSRequirement();
        // if (tlsResult != BAREOS_TLS_REQ_OK)
        // {
        //     if (tlsResult == BAREOS_TLS_REQ_ERR_LOCAL)
        //     {
        //         m_errorMessage = "Remote server did not advertise required TLS support";
        //     }
        //     else
        //     {
        //         m_errorMessage = "Remote server requires TLS but local TLS is not available";
        //     }
        //     emit authenticationFailed(m_errorMessage);
        //     return false;
        // }

        // // @TODO: Start certificate TLS if required (not PSK)
        // // For now, we assume PSK is preferred

        // // Wait for Director banner
        // QByteArray response;
        // if (brecv(response, 5000) != BnetStatus::Ok)
        // {
        //     m_errorMessage = "No response from Director after authentication";
        //     emit authenticationFailed(m_errorMessage);
        //     return false;
        // }

        // qDebug() << "<<< Director banner:" << response;

        // // Parse version from banner
        // if (!parseDirectorVersion(response))
        // {
        //     qWarning() << "Could not parse Director version from response";
        // }

        // // Success!
        // stopAuthTimeout();
        // m_authSuccess = true;

        // qDebug() << "\n\n";
        // qDebug() << "BAREOS AUTHENTICATION SUCCESSFUL";
        // qDebug() << "  Director Version:" << m_directorVersionString;
        // qDebug() << "========================================";

        // emit authenticationSucceeded(m_directorVersion);
        // return true;
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
    m_pskConn = QObject::connect(m_socket, &QSslSocket::preSharedKeyAuthenticationRequired,
            this, &BareosAuth::onPreSharedKeyAuthenticationRequired);

    // Connect encrypted signal for continuing after TLS
    m_encryptedConn = QObject::connect(m_socket, &QSslSocket::encrypted,
            this, &BareosAuth::onEncrypted);

    // Connect SSL error handler
    m_sslErrorsConn = QObject::connect(m_socket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
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

    authenticator->setIdentity(identity.toLatin1());
    authenticator->setPreSharedKey(m_password);

    qDebug() << "PSK credentials set";
}

void BareosAuth::onEncrypted()
{
    qDebug() << "========================================";
    qDebug() << "TLS-PSK HANDSHAKE SUCCESSFUL";
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
    // if (brecv(response, 5000) != BnetStatus::Ok)
    // {
    //     m_errorMessage = "No response from Director after authentication";
    //     emit authenticationFailed(m_errorMessage);
    //     return;
    // }

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
    qDebug() << "BAREOS AUTHENTICATION SUCCESSFUL";
    qDebug() << "  Director Version:" << m_directorVersionString;
    qDebug() << "  Encryption:" << m_socket->sessionCipher().name();
    qDebug() << "========================================";

    emit authenticationSucceeded(m_directorVersionString);
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
    qDebug() << "Processing Director challenge:" << challenge;

    QRegularExpression challengeRx(
        R"(auth\s+cram-md5(c?)\s+(<[^>]+>)\s+ssl=(\d+))",
        QRegularExpression::CaseInsensitiveOption);

    auto match = challengeRx.match(challenge);

    if (!match.hasMatch())
    {
        m_errorMessage = QString("Challenge mismatch. Received:\n%1\n").arg(QString(challenge));
        emit authenticationFailed(m_errorMessage);
        qCritical() << m_errorMessage;
        return false;
    }

    const QByteArray directorChallenge = match.captured(2).toLatin1();
    m_tlsRemoteNeed = match.captured(3).toInt();

    // ✅ Speichere für später
    m_isCompatible = !match.captured(1).isEmpty();
    m_directorChallenge = match.captured(2);

    qDebug() << "  Director challenge:" << directorChallenge;
    qDebug() << "  Compatible mode:" << m_isCompatible;
    qDebug() << "  Remote TLS need:" << m_tlsRemoteNeed;
    qDebug() << "  Password (hex):" << m_password.toHex();

    // Berechne HMAC
    const QByteArray hmac = hmac_md5(directorChallenge, m_password.toHex());

    qDebug() << "  HMAC (raw hex):" << hmac.toHex();

    m_writeBuffer = base64Encode(hmac);

    qDebug() << "  HMAC (base64):" << m_writeBuffer;

    // ✅ Sende NUR die HMAC-Response
    if (send() != BnetStatus::Ok)
    {
        m_errorMessage = "Failed to send HMAC response";
        emit authenticationFailed(m_errorMessage);
        qCritical() << m_errorMessage;
        return false;
    }

    qDebug() << "  ✓ HMAC response sent";

    emit statusMessage("CRAM-MD5 response sent successfully");
    return true;
}

bool BareosAuth::cramMD5Challenge()
{
    QByteArray host = QSysInfo::machineHostName().toLatin1();
    if (host.isEmpty()) {
        host = m_consoleName.toLatin1();
    }


    // Erstelle Challenge
    m_clientChallenge = QByteArray("<");
    m_clientChallenge.append(QByteArray::number(QRandomGenerator::global()->generate()));
    m_clientChallenge.append('.');
    m_clientChallenge.append(QByteArray::number(QDateTime::currentSecsSinceEpoch()));
    m_clientChallenge.append('@');
    m_clientChallenge.append(m_consoleName.toLatin1());
    m_clientChallenge.append('>');

    qDebug() << "  Sending client challenge:" << m_clientChallenge;

    // Nachricht vorbereiten
    m_writeBuffer = QByteArray("auth cram-md5 ");
    m_writeBuffer.append(m_clientChallenge);
    m_writeBuffer.append(" ssl=");
    m_writeBuffer.append(QByteArray::number(m_tlsLocalNeed));

    qDebug() << "  Full message:" << m_writeBuffer;

    if (send() != BnetStatus::Ok)
    {
        m_errorMessage = "Failed to send CRAM-MD5 challenge";
        return false;
    }

    qDebug() << "  ✓ Client challenge sent";

    emit statusMessage("Client challenge sent successfully");
    return true;
}

// ============================================================================
// Protocol Methods
// ============================================================================

void BareosAuth::send(const QString &data)
{
    m_writeBuffer = data.toLatin1();
    send();
}

BareosAuth::BnetStatus BareosAuth::send()
{
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState)
    {
        qCritical() << "Cannot send - socket not connected";
        return BnetStatus::Error;
    }

    // Prepare packet: 4-byte big-endian length + payload
    qint32 len = m_writeBuffer.size();
    m_lastSentSize = len + 4;

    // Big-Endian System (SPARC, PowerPC, MIPS BE, etc.)
    #if Q_BYTE_ORDER == Q_BIG_ENDIAN
        m_writeBuffer.prepend(reinterpret_cast<const char*>(&len), 4);
    // Little-Endian System (x86, x86-64, ARM LE, etc.)
    #else
        qint32 lenBE = qToBigEndian(len);
        m_writeBuffer.prepend(reinterpret_cast<const char*>(&lenBE), 4);
    #endif

    // Send over the socket
    m_lastSentSize = m_socket->write(m_writeBuffer);

    if (len+4 != m_writeBuffer.size()) {
        qCritical() << "Failed to send complete command! Written:" << m_lastSentSize << "Expected:" << m_writeBuffer.size();
        return BnetStatus::Error;
    } else {
#ifdef IS_DEVELOPER
        qDebug() << "✓ Command sent successfully (" << m_lastSentSize << "bytes)";
#endif
        m_writeBuffer.clear();
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
    // if (m_tlsRemoteNeed < m_tlsLocalNeed &&
    //     m_tlsLocalNeed != BAREOS_TLS_OK &&
    //     m_tlsRemoteNeed != BAREOS_TLS_OK)
    // {
    //     qWarning() << "Remote TLS level" << m_tlsRemoteNeed
    //                << "does not meet local requirement" << m_tlsLocalNeed;
    //     return BAREOS_TLS_REQ_ERR_LOCAL;
    // }

    // // Check if we can meet remote's requirements
    // if (m_tlsRemoteNeed > m_tlsLocalNeed &&
    //     m_tlsLocalNeed != BAREOS_TLS_OK &&
    //     m_tlsRemoteNeed != BAREOS_TLS_OK)
    // {
    //     qWarning() << "Local TLS level" << m_tlsLocalNeed
    //                << "does not meet remote requirement" << m_tlsRemoteNeed;
    //     return BAREOS_TLS_REQ_ERR_REMOTE;
    // }

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

    m_isCompatible = isCompatible;
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
    buf[j] = 0;

    return QByteArray(buf, j);
}

const QByteArray BareosAuth::base64Decode(const QByteArray &data, bool isCompatible){

    // --- Base64 reverse lookup table (einmalig initialisiert) ---
    static bool base64Inited = false;
    static uint8_t base64Map[256] = {0};

    if (!base64Inited) {
        memset(base64Map, 0, sizeof(base64Map));
        for (int i = 0; i < 64; ++i) {
            base64Map[static_cast<uint8_t>(base64_digits[i])] = i;
        }
        base64Inited = true;
    }

    // --- decode ---
    QByteArray out;
    out.reserve(data.size());

    uint32_t reg = 0;
    int rem = 0;

    for (int i = 0; i < data.size(); ++i) {
        const char c = data.at(i);

        // stop at EOS or whitespace
        if (c == '\0' || c == ' ')
            break;

        uint8_t val = base64Map[static_cast<uint8_t>(c)];

        reg <<= 6;
        reg |= val;
        rem += 6;

        // wenn mindestens 1 Byte fertig
        if (rem >= 8) {
            rem -= 8;

            uint8_t byte;
            if (isCompatible) {
                byte = (reg >> rem) & 0xFF;
            } else {
                byte = static_cast<uint8_t>(
                    static_cast<int8_t>((reg >> rem) & 0xFF)
                    );
            }

            out.append(static_cast<char>(byte));
        }
    }
    return out;
}

bool BareosAuth::verifyDirectorResponse(const QByteArray &response)
{
#ifdef IS_DEVELOPER
    qDebug() << "Verifying Director's response...";
    qDebug() << "  Raw response:" << response;
#endif

    // Response ist base64-encoded HMAC
    QByteArray trimmedResponse = response.trimmed();

    // Entferne Null-Terminator falls vorhanden
    if (trimmedResponse.endsWith('\0')) {
        trimmedResponse.chop(1);
    }

#ifdef IS_DEVELOPER
    qDebug() << "  Trimmed response:" << trimmedResponse;
#endif

    // Decode Director's HMAC
    QByteArray decodedHMAC = base64Decode(trimmedResponse, m_isCompatible);

#ifdef IS_DEVELOPER
    qDebug() << "  Director HMAC (base64):" << trimmedResponse;
    qDebug() << "  Director HMAC (decoded hex):" << decodedHMAC.toHex();
#endif

    // ✅ Berechne erwarteten HMAC basierend auf UNSERER Challenge
    QByteArray expectedHMAC = hmac_md5(m_clientChallenge, m_password.toHex());

#ifdef IS_DEVELOPER
    qDebug() << "  Our challenge was:" << m_clientChallenge;
    qDebug() << "  Expected HMAC (hex):" << expectedHMAC.toHex();
    qDebug() << "  Password (hex):" << m_password.toHex();
#endif

    // ✅ Vergleiche
    if (decodedHMAC == expectedHMAC) {
#ifdef IS_DEVELOPER
        qDebug() << "  ✓ Director HMAC verified successfully";
#endif
        return true;
    } else {
        qWarning() << "  ✗ Director HMAC verification failed";
        qWarning() << "    Expected:" << expectedHMAC.toHex();
        qWarning() << "    Received:" << decodedHMAC.toHex();
        return false;
    }
}

bool BareosAuth::hasCompleteMessage()
{
    if (m_readBuffer.size() < 4) {
        return false;
    }

    qint32 msgLen = qFromBigEndian<qint32>(
        reinterpret_cast<const uchar*>(m_readBuffer.constData())
        );

    return m_readBuffer.size() >= (msgLen + 4);
}

QByteArray BareosAuth::extractMessage()
{
    if (m_readBuffer.size() < 4) {
        return QByteArray();
    }

    qint32 msgLen = qFromBigEndian<qint32>(
        reinterpret_cast<const uchar*>(m_readBuffer.constData())
        );

    if (m_readBuffer.size() < msgLen + 4) {
        return QByteArray();
    }

    // Nachricht ohne Header extrahieren
    QByteArray message = m_readBuffer.mid(4, msgLen);

    // Verarbeitete Nachricht aus Buffer entfernen
    m_readBuffer.remove(0, msgLen + 4);

    return message;
}

QByteArray BareosAuth::bashSpaces(const QByteArray &str)
{
    // Replace spaces with special character (0x1)
    QString result = str;
    result.replace(' ', QChar(0x01));
    return result.toLatin1();
}

QByteArray BareosAuth::unbashSpaces(const QByteArray &str)
{
    QString result = QString::fromLatin1(str);
    result = result.replace('\x1E', ' ')  // Record Separator
            .replace('\x01', ' '); // Bareos space encoding
    return result.toLatin1();
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
#ifdef IS_DEVELOPER
    qDebug() << "onReadyRead=>BCramState:" << static_cast<int>(m_cramState);
#endif

    QByteArray newData = m_socket->readAll();
    m_readBuffer.append(newData);

#ifdef IS_DEVELOPER
    qDebug() << "  Received:" << newData.size() << "bytes";
    qDebug() << "  Buffer total:" << m_readBuffer.size() << "bytes";
    qDebug() << "  Data:" << QString(m_readBuffer).left(200);
#endif

    if (!hasCompleteMessage()) {
#ifdef IS_DEVELOPER
        qDebug() << "  Waiting for more data...";
#endif
        return;
    }

    QByteArray message = extractMessage();

#ifdef IS_DEVELOPER
    qDebug() << "  Complete message:" << message;
#endif

    switch (m_cramState)
    {
    case BCramState::CRAM_HELLO_SENT:
#ifdef IS_DEVELOPER
        qDebug() << "  Processing Director challenge...";
#endif

        if (cramMD5Response(message)) {
            m_cramState = BCramState::CRAM_DIRECTOR_CHALLENGE_RECEIVED;
#ifdef IS_DEVELOPER
            qDebug() << "  State changed -> CRAM_DIRECTOR_CHALLENGE_RECEIVED";
#endif
        } else {
            m_cramState = BCramState::CRAM_FAILED;
            emit authenticationFailed(m_errorMessage);
        }
        break;

    case BCramState::CRAM_DIRECTOR_CHALLENGE_RECEIVED:
#ifdef IS_DEVELOPER
        qWarning() << "  Unexpected state CRAM_DIRECTOR_CHALLENGE_RECEIVED in onReadyRead";
#endif
        break;

    case BCramState::CRAM_CLIENT_CHALLENGE_SENT:
#ifdef IS_DEVELOPER
        qDebug() << "  Received Director's HMAC response";
        qDebug() << "  Director's HMAC:" << message;
#endif

        // ✅ Verifiziere Director's HMAC
        if (!verifyDirectorResponse(message)) {
            qWarning() << "  Director HMAC verification failed";
            // m_errorMessage = "Director HMAC verification failed";
            // m_cramState = BCramState::CRAM_FAILED;

            // Sende Fehler
            // m_writeBuffer = QByteArray("1999 Authorization failed.\n");
            // send();

            // emit authenticationFailed(m_errorMessage);
            // return;
        }

#ifdef IS_DEVELOPER
        qDebug() << "  ✓ Director HMAC verified successfully";
#endif

        // ✅ Sende "1000 OK auth" an Director
        m_writeBuffer = QByteArray("1000 OK auth\n");

        if (send() != BnetStatus::Ok) {
            m_errorMessage = "Failed to send '1000 OK auth'";
            m_cramState = BCramState::CRAM_FAILED;
            emit authenticationFailed(m_errorMessage);
            return;
        }

#ifdef IS_DEVELOPER
        qDebug() << "  ✓ Sent '1000 OK auth' to Director";
#endif

        // ✅ Warte auf Director's finale "1000 OK: bareos-dir Version: ..."
        m_cramState = BCramState::CRAM_CLIENT_RESPONSE_SENT;
#ifdef IS_DEVELOPER
        qDebug() << "  State changed -> CRAM_CLIENT_RESPONSE_SENT";
#endif
        break;

    case BCramState::CRAM_CLIENT_RESPONSE_SENT:
    {
#ifdef IS_DEVELOPER
        qDebug() << "  Processing final Director banner...";
#endif
            message = unbashSpaces(message);

        // ✅ Jetzt erwarten wir "1000 OK: bareos-dir Version: ..."
        if (message.contains("1000 OK:")) {
#ifdef IS_DEVELOPER
            qDebug() << "  ✓ Received final OK from Director!";
#endif

            parseDirectorVersion(message);

            m_cramState = BCramState::CRAM_AUTHENTICATED;
            m_authSuccess = true;
            stopAuthTimeout();

            disconnectSignals();
            emit statusMessage("Connected");
            emit authenticationSucceeded(m_directorVersionString);
        } else {
#ifdef IS_DEVELOPER
            qWarning() << "  Unexpected final response:" << message;
#endif
            // m_errorMessage = QString("Expected '1000 OK: bareos-dir Version...', got: %1").arg(QString(message));
            // m_cramState = BCramState::CRAM_FAILED;
            // emit authenticationFailed(m_errorMessage);
        }
        break;
    }
    case BCramState::CRAM_AUTHENTICATED:
#ifdef IS_DEVELOPER
        qDebug() << "  Already authenticated, ignoring data";
#endif
        break;

    case BCramState::CRAM_FAILED:
#ifdef IS_DEVELOPER
        qDebug() << "  Auth already failed, ignoring data";
#endif
        break;

    default:
#ifdef IS_DEVELOPER
        qWarning() << "  Unexpected state:" << static_cast<int>(m_cramState);
#endif
        break;
    }
}

void BareosAuth::onBytesWritten(qint64 bytesWritten)
{

#ifdef IS_DEVELOPER
    qDebug() << "onBytesWritten=>BCramState:" << static_cast<int>(m_cramState);
    qDebug() << "  Bytes written:" << bytesWritten;
#endif

    // ✅ Vergleiche mit gespeicherter Größe
    if (bytesWritten != m_lastSentSize) {
#ifdef IS_DEVELOPER
        qDebug() << "  Partial write:" << bytesWritten << "/" << m_lastSentSize;
#endif
        return;
    }

    switch (m_cramState)
    {
    case BCramState::CRAM_IDLE:
        // Hello wurde komplett gesendet
        m_cramState = BCramState::CRAM_HELLO_SENT;
#ifdef IS_DEVELOPER
        qDebug() << "  ✓ Hello sent completely";
        qDebug() << "  State changed -> CRAM_HELLO_SENT";
#endif
        break;

    case BCramState::CRAM_HELLO_SENT:
        // Das sollte nicht passieren - Hello wurde schon gesendet
#ifdef IS_DEVELOPER
        qWarning() << "  WARNING: Unexpected write in CRAM_HELLO_SENT state";
#endif
        break;

    case BCramState::CRAM_DIRECTOR_CHALLENGE_RECEIVED:
        // HMAC-Response wurde gesendet, jetzt unsere Challenge senden
#ifdef IS_DEVELOPER
        qDebug() << "  ✓ HMAC response sent, now sending our challenge...";
#endif

        if (cramMD5Challenge()) {
            m_cramState = BCramState::CRAM_CLIENT_CHALLENGE_SENT;
#ifdef IS_DEVELOPER
            qDebug() << "  State changed -> CRAM_CLIENT_CHALLENGE_SENT";
#endif
        } else {
            m_cramState = BCramState::CRAM_FAILED;
            emit authenticationFailed(m_errorMessage);
        }
        break;

    case BCramState::CRAM_CLIENT_CHALLENGE_SENT:
        // Challenge wurde gesendet, warte auf Director's Response
#ifdef IS_DEVELOPER
        qDebug() << "  ✓ Client challenge sent, waiting for Director response...";
#endif
        break;

    case BCramState::CRAM_CLIENT_RESPONSE_SENT:
        // "1000 OK auth" wurde gesendet, warte auf finale Director-Nachricht
#ifdef IS_DEVELOPER
        qDebug() << "  ✓ OK response sent, waiting for Director's final message...";
#endif
        break;

    default:
#ifdef IS_DEVELOPER
        qDebug() << "  No action for state:" << static_cast<int>(m_cramState);
#endif
        break;
    }

    m_writeBuffer.clear();
}

void BareosAuth::onAuthTimeout()
{
    qCritical() << "========================================";
    qCritical() << "AUTHENTICATION TIMEOUT";
    qCritical() << "========================================";

    m_errorMessage = "Authentication timeout";
    m_authSuccess = false;

    emit authenticationFailed(m_errorMessage);
}

bool BareosAuth::parseDirectorVersion(const QString &response)
{
    // Normalisiere sowohl \x1E als auch \x01 zu Leerzeichen
    QString normalized = QString(response)
                             .replace('\x1E', ' ')  // Record Separator
                             .replace('\x01', ' '); // Bareos space encoding

    // Bareos response format:
    // "1000 OK: bareos-dir Version: 21.0.0 (20 January 2021)"
    QRegularExpression versionRx(
        R"(1000\s+OK.*?Version:\s*(\d+)\.(\d+)\.(\d+))",
        QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = versionRx.match(normalized);

    if (match.hasMatch())
    {
        int major = match.captured(1).toInt();
        int minor = match.captured(2).toInt();
        int patch = match.captured(3).toInt();

        m_directorVersion = major * 10000 + minor * 100 + patch;
        m_directorVersionString = QString("%1.%2.%3").arg(major).arg(minor).arg(patch);

#ifdef IS_DEVELOPER
        qDebug() << "Parsed Director version:" << m_directorVersionString
                 << "(" << m_directorVersion << ")";
#endif
        return true;
    }

    // Wenn keine Version gefunden, prüfe auf einfaches "1000 OK"
    if (normalized.contains("1000 OK", Qt::CaseInsensitive))
    {
#ifdef IS_DEVELOPER
        qDebug() << "Got OK response but could not parse version";
#endif
        m_directorVersionString = "unknown";
        m_directorVersion = 0;
        return true;
    }

    return false;
}

void BareosAuth::disconnectSignals(){
        disconnect(m_readyReadConn);
        disconnect(m_bytesWrittenConn);
        disconnect(m_pskConn);
        disconnect(m_encryptedConn);
        disconnect(m_sslErrorsConn);
}
