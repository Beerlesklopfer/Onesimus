/**
 * @file baculaauth.cpp
 * @brief Qt-based Bacula Authentication Implementation
 *
 * This file implements the Bacula authentication protocol for Qt applications,
 * providing CRAM-MD5 challenge-response authentication and TLS/PSK support.
 *
 * @author Original Bacula code by Kern Sibbald
 * @author Qt port and modifications by [Your Name]
 *
 * @version 1.2.0
 * @date 2025-01-21
 *
 * @copyright Copyright (c) 2025
 * @license GPL-2.0-or-later
 *
 * Based on Bacula's authenticate.c and authenticatebase.cc
 *
 * @par Version History:
 * | Version | Date       | Author      | Changes                                    |
 * |---------|------------|-------------|--------------------------------------------|
 * | 1.0.0   | 2024-xx-xx | [Author]    | Initial Qt port from Bacula               |
 * | 1.1.0   | 2025-01-xx | [Author]    | Added PSK-TLS support                     |
 * | 1.2.0   | 2025-01-21 | [Author]    | Fixed CRAM-MD5 Base64, error handling     |
 *
 * @par References:
 * - Bacula Source: https://gitlab.bacula.org/bacula-community-edition/bacula-community
 * - RFC 2104: HMAC: Keyed-Hashing for Message Authentication
 * - RFC 2195: IMAP/POP AUTHorize Extension for Simple Challenge/Response
 *
 * @bug None known
 * @todo Add support for interactive authentication (BPAM)
 */

#include "baculaauth.h"

#include <QDebug>
#include <QMessageAuthenticationCode>
#include <qsslcipher.h>
#include <qsslconfiguration.h>
#include <qsslpresharedkeyauthenticator.h>

/**
 * @brief Constructs a BaculaAuth object
 *
 * Initializes all member variables to their default states and sets up
 * the authentication timeout timer.
 *
 * @param socket Pointer to a connected QSslSocket for communication
 * @param parent Optional parent QObject for memory management
 *
 * @pre socket must not be nullptr
 * @post Authentication timer is created and connected
 *
 * @since 1.0.0
 */
BaculaAuth::BaculaAuth(QSslSocket *socket, QObject *parent)
    : QObject(parent)
    , m_socket(socket)
    , m_authTimer(nullptr)
    , m_directorVersion(0)
    , m_tlsLocalNeed(BNET_TLS_NONE)
    , m_pskLocalNeed(BNET_TLS_NONE)
    , m_tlsRemoteNeed(BNET_TLS_NONE)
    , m_pskRemoteNeed(BNET_TLS_NONE)
    , m_tlsStarted(false)
    , m_checkEarlyTLS(false)
    , m_authSuccess(false)
{
    m_authTimer = new QTimer(this);
    m_authTimer->setSingleShot(true);
    connect(m_authTimer, &QTimer::timeout, this, &BaculaAuth::onAuthTimeout);
}

/**
 * @brief Destructor
 *
 * Ensures the authentication timeout timer is stopped before destruction.
 *
 * @since 1.0.0
 */
BaculaAuth::~BaculaAuth()
{
    stopAuthTimeout();
}

// ============================================================================
// Main Authentication Entry Point
// ============================================================================

/**
 * @brief Authenticates with the Bacula Director using CRAM-MD5
 *
 * This method implements the complete Bacula authentication handshake:
 *
 * @par Authentication Protocol Sequence:
 * 1. **Hello Message**: Client sends identification with TLS capabilities
 *    - Format: `Hello <name> calling <version> tlspsk=<value>`
 * 2. **TLS Negotiation** (if enabled): starttls handshake
 * 3. **CRAM-MD5 Phase 1**: Respond to Director's challenge
 *    - Receive: `auth cram-md5 <challenge> ssl=<X>`
 *    - Send: Base64-encoded HMAC-MD5 response
 * 4. **CRAM-MD5 Phase 2**: Challenge the Director
 *    - Send: `auth cram-md5 <our_challenge> ssl=<X>`
 *    - Receive: Director's Base64-encoded response
 * 5. **Verification**: Both sides verify the other's response
 *
 * @param directorName Name of the Director resource (for logging)
 * @param consoleName Name of the Console resource (sent in Hello)
 * @param password Shared secret for CRAM-MD5 authentication
 * @param tlsEnable Enable TLS encryption capability
 * @param tlsRequire Require TLS encryption (fail if not available)
 * @param tlsPSKEnable Enable PSK-TLS mode using password as pre-shared key
 *
 * @return true if authentication completed successfully
 * @return false if authentication failed (see getErrorMessage())
 *
 * @note This is a blocking call with a 180-second timeout
 * @note The consoleName must match a Console resource in bacula-dir.conf
 * @note The password must match the Password directive in the Console resource
 *
 * @warning Do not call this method if the socket is not connected
 *
 * @par Error Codes in Response:
 * - `1000 OK`: Authentication successful
 * - `1999 Authorization failed`: CRAM-MD5 verification failed
 * - `2999 No go`: Connection rejected
 *
 * @see cramMD5Respond()
 * @see cramMD5Challenge()
 * @see getErrorMessage()
 *
 * @since 1.0.0
 * @version 1.2.0 - Improved error handling and TLS negotiation
 */
bool BaculaAuth::authenticateDirector(const QString &directorName,
                                      const QString &consoleName,
                                      const QString &password,
                                      bool tlsEnable,
                                      bool tlsRequire,
                                      bool tlsPSKEnable)
{
    qDebug() << "========================================";
    qDebug() << "BACULA AUTHENTICATION START";
    qDebug() << "========================================";
    qDebug() << "Director:" << directorName;
    qDebug() << "Console:" << consoleName;
    qDebug() << "Password length:" << password.length();
    qDebug() << "TLS Enable:" << tlsEnable;
    qDebug() << "TLS Require:" << tlsRequire;
    qDebug() << "PSK Enable:" << tlsPSKEnable;

    m_password = password;
    m_authSuccess = false;
    m_errorMessage.clear();
    m_directorVersion = 0;
    m_readBuffer.clear();

    // Calculate TLS/PSK needs
    // KORREKTUR: Verwende korrekte Berechnung
    // Wenn nur PSK verwendet wird: tlspsk = 100 (PSK=1, TLS=0)
    // Wenn TLS verwendet wird: tlspsk = 1 oder 2
    // Wenn beides: tlspsk = 101 oder 102 etc.

    int tlspskLocalNeed = calculateTLSPSKNeed(tlsEnable, tlsRequire, tlsPSKEnable);

    qDebug() << "TLSPSK local need:" << tlspskLocalNeed;
    qDebug() << "  TLS local need:" << m_tlsLocalNeed;
    qDebug() << "  PSK local need:" << m_pskLocalNeed;

    // Start timeout
    startAuthTimeout();

    // =================================================================
    // STEP 1: Send Hello (MIT korrektem Format!)
    // =================================================================

    // KORREKTUR: Leerzeichen mit bashSpaces behandeln
    QString bashedName = bashSpaces(consoleName);

    // Falls der Name Leerzeichen enthält, werden diese zu 0x01
    // Alternativ könnte man *Name* verwenden
    QString hello;
    if (consoleName.contains(' ')) {
        // Mit Sternchen für Namen mit Leerzeichen
        hello = QString("Hello *%1* calling %2 tlspsk=%3\n")
                    .arg(consoleName)
                    .arg(UA_VERSION)
                    .arg(tlspskLocalNeed);
    } else {
        // Ohne Sternchen für einfache Namen
        hello = QString("Hello %1 calling %2 tlspsk=%3\n")
                    .arg(consoleName)
                    .arg(UA_VERSION)
                    .arg(tlspskLocalNeed);
    }

    qDebug() << "\n>>> Sending Bacula Hello:";
    qDebug() << "   " << hello.trimmed();

    if (!sendBaculaMessage(hello)) {
        stopAuthTimeout();
        m_errorMessage = "Failed to send Hello message";
        emit authenticationFailed(m_errorMessage);
        return false;
    }

    // =================================================================
    // STEP 2: Wait for Director response
    // =================================================================
    qDebug() << "\nWaiting for Director response...";

    if (!waitForResponse(10000)) {  // 10 Sekunden für langsame Netzwerke
        stopAuthTimeout();
        m_errorMessage = "No response from Director after Hello";
        qCritical() << m_errorMessage;
        emit authenticationFailed(m_errorMessage);
        return false;
    }

    QString response;
    if (!readBaculaMessage(response, 5000)) {
        stopAuthTimeout();
        m_errorMessage = "Failed to read Director response";
        emit authenticationFailed(m_errorMessage);
        return false;
    }

    qDebug() << "<<< Director response:" << response;

    // =================================================================
    // STEP 3: Handle "starttls" if present
    // =================================================================
    QRegularExpression startTlsRegex("starttls tlspsk=(\\d+)");
    QRegularExpressionMatch match = startTlsRegex.match(response);

    if (match.hasMatch()) {
        int tlspskRemote = match.captured(1).toInt();
        decodeRemoteTLSPSKNeed(tlspskRemote);

        qDebug() << "\n=== STARTTLS RECEIVED ===";
        qDebug() << "Remote TLSPSK:" << tlspskRemote;

        // Check TLS requirements
        TLSRequirementResult result = testTLSRequirement();
        if (result != TLS_REQ_OK) {
            stopAuthTimeout();
            m_errorMessage = (result == TLS_REQ_ERR_LOCAL)
                                 ? "Local TLS requirements not met"
                                 : "Remote TLS requirements not met";
            emit authenticationFailed(m_errorMessage);
            return false;
        }

        // Start TLS/PSK if needed
        if (m_pskLocalNeed >= BNET_TLS_OK && m_pskRemoteNeed >= BNET_TLS_OK) {
            qDebug() << "\n=== STARTING PSK-TLS ===";
            if (!startEncryption(true)) {
                stopAuthTimeout();
                emit authenticationFailed(m_errorMessage);
                return false;
            }
        } else if (m_tlsLocalNeed >= BNET_TLS_OK && m_tlsRemoteNeed >= BNET_TLS_OK) {
            qDebug() << "\n=== STARTING REGULAR TLS ===";
            if (!startEncryption(false)) {
                stopAuthTimeout();
                emit authenticationFailed(m_errorMessage);
                return false;
            }
        }

        // Nach TLS: Nächste Nachricht lesen (CRAM-MD5 Challenge)
        if (!readBaculaMessage(response, 5000)) {
            stopAuthTimeout();
            m_errorMessage = "Failed to read post-TLS response";
            emit authenticationFailed(m_errorMessage);
            return false;
        }
        qDebug() << "<<< Post-TLS response:" << response;
    }

    // =================================================================
    // STEP 4: CRAM-MD5 Authentication
    // =================================================================

    // Prüfe ob die Response bereits ein CRAM-MD5 Challenge ist
    if (response.contains("auth cram-md5")) {
        // Die Response ist bereits die Challenge - in Buffer zurücklegen
        m_readBuffer.prepend(createBaculaPacket(response));
    }

    qDebug() << "\n=== STARTING CRAM-MD5 ===";

    if (!clientCramMD5Authenticate(password)) {
        stopAuthTimeout();
        emit authenticationFailed(m_errorMessage);
        return false;
    }

    // =================================================================
    // STEP 5: Wait for "1000 OK auth"
    // =================================================================
    QString authResponse;
    if (!readBaculaMessage(authResponse, 5000)) {
        stopAuthTimeout();
        m_errorMessage = "No auth confirmation from Director";
        emit authenticationFailed(m_errorMessage);
        return false;
    }

    qDebug() << "<<< Auth response:" << authResponse;

    if (!authResponse.startsWith("1000 OK")) {
        stopAuthTimeout();
        m_errorMessage = QString("Authentication rejected: %1").arg(authResponse);
        emit authenticationFailed(m_errorMessage);
        return false;
    }

    // =================================================================
    // STEP 6: Wait for final version message
    // =================================================================
    QString finalResponse;
    if (!readBaculaMessage(finalResponse, 5000)) {
        // Manchmal ist die Version bereits in der auth-Response
        if (authResponse.contains("Version:")) {
            finalResponse = authResponse;
        } else {
            stopAuthTimeout();
            m_errorMessage = "No version info from Director";
            emit authenticationFailed(m_errorMessage);
            return false;
        }
    }

    qDebug() << "<<< Final response:" << finalResponse;

    // Parse version: "1000 OK: <version> <name> Version: X.Y.Z (date)"
    QRegularExpression versionRegex("1000 OK:?\\s*(\\d+)");
    QRegularExpressionMatch vMatch = versionRegex.match(finalResponse);
    if (vMatch.hasMatch()) {
        m_directorVersion = vMatch.captured(1).toInt();
    }

    m_authSuccess = true;
    stopAuthTimeout();

    qDebug() << "========================================";
    qDebug() << "✓ AUTHENTICATION SUCCESSFUL";
    qDebug() << "  Director Version:" << m_directorVersion;
    qDebug() << "========================================";

    emit authenticationSucceeded(m_directorVersion);
    return true;
}


// ============================================================================
// TLS/PSK Calculation and Testing
// ============================================================================

int BaculaAuth::calculateTLSPSKNeed(bool tlsEnable, bool tlsRequire, bool pskEnable)
{
    // Calculate TLS need
    if (tlsEnable) {
        m_tlsLocalNeed = tlsRequire ? BNET_TLS_REQUIRED : BNET_TLS_OK;
    } else {
        m_tlsLocalNeed = BNET_TLS_NONE;
    }

    // Calculate PSK need
    if (pskEnable) {
        m_pskLocalNeed = tlsRequire ? BNET_TLS_REQUIRED : BNET_TLS_OK;
    } else {
        m_pskLocalNeed = BNET_TLS_NONE;
    }

    // Encode: tls_need + (psk_need * 100)
    return m_tlsLocalNeed + (m_pskLocalNeed * 100);
}

void BaculaAuth::decodeRemoteTLSPSKNeed(int remoteNeed)
{
    m_tlsRemoteNeed = remoteNeed % 100;
    m_pskRemoteNeed = remoteNeed / 100;

    qDebug() << "Remote TLSPSK decoded:" << remoteNeed;
    qDebug() << "  TLS Remote Need:" << m_tlsRemoteNeed;
    qDebug() << "  PSK Remote Need:" << m_pskRemoteNeed;
}

TLSRequirementResult BaculaAuth::testTLSRequirement()
{
    // Based on TestTLSRequirement() from authenticatebase.cc

    // {OK,NONE} vs {NONE,REQUIRED} => Remote error
    if (m_tlsLocalNeed == BNET_TLS_OK && m_pskLocalNeed == BNET_TLS_NONE &&
        m_tlsRemoteNeed == BNET_TLS_NONE && m_pskRemoteNeed == BNET_TLS_REQUIRED) {
        return TLS_REQ_ERR_REMOTE;
    }

    // {OK,REQUIRED} vs {NONE,NONE} => Local error
    if (m_tlsLocalNeed == BNET_TLS_OK && m_pskLocalNeed == BNET_TLS_REQUIRED &&
        m_tlsRemoteNeed == BNET_TLS_NONE && m_pskRemoteNeed == BNET_TLS_NONE) {
        return TLS_REQ_ERR_LOCAL;
    }

    // {NONE,OK} vs {REQUIRED,NONE} => Remote error
    if (m_tlsLocalNeed == BNET_TLS_NONE && m_pskLocalNeed == BNET_TLS_OK &&
        m_tlsRemoteNeed == BNET_TLS_REQUIRED && m_pskRemoteNeed == BNET_TLS_NONE) {
        return TLS_REQ_ERR_REMOTE;
    }

    // {NONE,NONE} vs {REQUIRED or anything} => Remote error
    if (m_tlsLocalNeed == BNET_TLS_NONE && m_pskLocalNeed == BNET_TLS_NONE &&
        (m_tlsRemoteNeed == BNET_TLS_REQUIRED || m_pskRemoteNeed == BNET_TLS_REQUIRED)) {
        return TLS_REQ_ERR_REMOTE;
    }

    // {NONE,REQUIRED} vs {anything,NONE} (except REQUIRED) => Local error
    if (m_tlsLocalNeed == BNET_TLS_NONE && m_pskLocalNeed == BNET_TLS_REQUIRED &&
        m_pskRemoteNeed == BNET_TLS_NONE) {
        return TLS_REQ_ERR_LOCAL;
    }

    // {REQUIRED,OK} vs {NONE,NONE} => Local error
    if (m_tlsLocalNeed == BNET_TLS_REQUIRED && m_pskLocalNeed == BNET_TLS_OK &&
        m_tlsRemoteNeed == BNET_TLS_NONE && m_pskRemoteNeed == BNET_TLS_NONE) {
        return TLS_REQ_ERR_LOCAL;
    }

    // {REQUIRED,NONE} vs {NONE,anything} => Local error
    if (m_tlsLocalNeed == BNET_TLS_REQUIRED && m_pskLocalNeed == BNET_TLS_NONE &&
        m_tlsRemoteNeed == BNET_TLS_NONE) {
        return TLS_REQ_ERR_LOCAL;
    }

    // {REQUIRED,REQUIRED} vs {NONE,NONE} => Local error
    if (m_tlsLocalNeed == BNET_TLS_REQUIRED && m_pskLocalNeed == BNET_TLS_REQUIRED &&
        m_tlsRemoteNeed == BNET_TLS_NONE && m_pskRemoteNeed == BNET_TLS_NONE) {
        return TLS_REQ_ERR_LOCAL;
    }

    return TLS_REQ_OK;
}

// ============================================================================
// Protocol Methods
// ============================================================================

/**
 * @brief Erstellt ein Bacula-Protokoll-Paket mit 4-Byte Big-Endian Längen-Header
 * @param message Die zu sendende Nachricht (ohne Header)
 * @return Das vollständige Paket mit Header
 */
QByteArray BaculaAuth::createBaculaPacket(const QString &message)
{
    QByteArray msgData = message.toUtf8();
    qint32 length = msgData.size();

    QByteArray packet;
    packet.reserve(4 + length);

    // 4-Byte Big-Endian Längen-Header hinzufügen
    packet.append(static_cast<char>((length >> 24) & 0xFF));
    packet.append(static_cast<char>((length >> 16) & 0xFF));
    packet.append(static_cast<char>((length >> 8) & 0xFF));
    packet.append(static_cast<char>(length & 0xFF));

    // Nachricht anhängen
    packet.append(msgData);

    return packet;
}

/**
 * @brief Sendet eine Nachricht im Bacula-Paketformat
 * @param message Die zu sendende Nachricht
 * @return true bei Erfolg
 */
bool BaculaAuth::sendBaculaMessage(const QString &message)
{
    QByteArray packet = createBaculaPacket(message);

    qDebug() << ">>> Sending Bacula packet:";
    qDebug() << "    Length header:" << QString("0x%1").arg(message.toUtf8().size(), 8, 16, QChar('0'));
    qDebug() << "    Message:" << message.trimmed();
    qDebug() << "    Total packet size:" << packet.size() << "bytes";

    qint64 written = m_socket->write(packet);
    m_socket->flush();

    if (written != packet.size()) {
        m_errorMessage = QString("Failed to send packet: wrote %1 of %2 bytes")
        .arg(written).arg(packet.size());
        qCritical() << m_errorMessage;
        return false;
    }

    return true;
}

/**
  * @brief Liest eine Bacula-Protokoll-Nachricht mit 4-Byte Längen-Header
  * @param message Output: Die gelesene Nachricht (ohne Header)
  * @param timeoutMs Timeout in Millisekunden
  * @return true bei Erfolg
*/
bool BaculaAuth::readBaculaMessage(QString &message, int timeoutMs)
{
    // Wir brauchen mindestens 4 Bytes für den Header
    while (m_readBuffer.size() < 4) {
        if (!waitForResponse(timeoutMs)) {
            qDebug() << "Timeout waiting for packet header";
            return false;
        }
        m_readBuffer.append(m_socket->readAll());
    }

    // 4-Byte Big-Endian Länge lesen
    qint32 length = 0;
    length |= (static_cast<unsigned char>(m_readBuffer[0]) << 24);
    length |= (static_cast<unsigned char>(m_readBuffer[1]) << 16);
    length |= (static_cast<unsigned char>(m_readBuffer[2]) << 8);
    length |= static_cast<unsigned char>(m_readBuffer[3]);

    qDebug() << "  Packet header: length =" << length;

    // Negative Länge = Signal-Nachricht (z.B. -1 für EOD)
    if (length < 0) {
        qDebug() << "  Signal message received:" << length;
        m_readBuffer.remove(0, 4);
        message = QString("SIGNAL:%1").arg(length);
        return true;
    }

    // Länge validieren (max. 1MB als Sicherheit)
    if (length > 1024 * 1024) {
        m_errorMessage = QString("Invalid packet length: %1").arg(length);
        qCritical() << m_errorMessage;
        return false;
    }

    // Warten bis alle Daten da sind
    while (m_readBuffer.size() < 4 + length) {
        if (!waitForResponse(timeoutMs)) {
            qDebug() << "Timeout waiting for packet data";
            return false;
        }
        m_readBuffer.append(m_socket->readAll());
    }

    // Nachricht extrahieren
    message = QString::fromUtf8(m_readBuffer.mid(4, length)).trimmed();
    m_readBuffer.remove(0, 4 + length);

    qDebug() << "<<< Read Bacula message:" << message.left(100)
             << (message.length() > 100 ? "..." : "");

    return true;
}

/**
 * @brief Responds to the Director's CRAM-MD5 challenge
 *
 * This method implements the first phase of CRAM-MD5 authentication where
 * the client responds to the Director's challenge.
 *
 * @par CRAM-MD5 Response Algorithm:
 * 1. Parse the challenge string from Director's message
 * 2. Extract the challenge and SSL value
 * 3. Compute HMAC-MD5: `HMAC(password, challenge)`
 * 4. Encode result as Base64 (without padding)
 * 5. Send the encoded response
 *
 * @par Message Format:
 * - **Received**: `auth cram-md5 <timestamp.random@hostname> ssl=X`
 * - **Sent**: `<Base64-encoded-HMAC-MD5>`
 *
 * @param password The shared secret (from Console resource Password directive)
 *
 * @return true if response was sent successfully
 * @return false if challenge parsing failed or send error occurred
 *
 * @note The response uses Base64 encoding WITHOUT padding ('=' characters)
 * @note This method does NOT wait for confirmation - the Director responds
 *       after we send our challenge in cramMD5Challenge()
 * @note The remote SSL value is stored in m_tlsRemoteNeed for later use
 *
 * @warning A false return does not necessarily mean the password is wrong -
 *          password verification happens in cramMD5Challenge() when we
 *          receive the Director's response to our challenge
 *
 * @par Example Challenge:
 * @code
 * auth cram-md5 <598282218.1769008626@PDC-dir> ssl=1
 * @endcode
 *
 * @see cramMD5Challenge()
 * @see hmacMD5()
 * @see baculaBase64Encode()
 *
 * @since 1.0.0
 * @version 1.2.0 - Fixed Base64 encoding (was incorrectly using hex),
 *                  added remote TLS need storage, improved error detection
 */
bool BaculaAuth::cramMD5Respond(const QString &password)
{
    qDebug() << "--- CRAM-MD5 Respond Phase ---";

    // Read Director's challenge
    QString challenge;
    if (!readBaculaMessage(challenge, 5000)) {
        m_errorMessage = "Timeout waiting for CRAM-MD5 challenge";
        return false;
    }

    qDebug() << "<<< Received challenge:" << challenge;

    // WICHTIG: Prüfe ob dies bereits eine Fehlermeldung ist
    // (z.B. bei falscher TLS-Konfiguration oder unbekanntem Console-Namen)
    if (challenge.startsWith("1999") ||
        challenge.startsWith("2999") ||
        challenge.contains("Authorization failed") ||
        challenge.contains("No Console resource") ||
        challenge.contains("rejected")) {
        m_errorMessage = QString("Director rejected connection before CRAM-MD5: %1")
        .arg(challenge);
        qCritical() << m_errorMessage;
        return false;
    }

    // Parse challenge format: "auth cram-md5 <challenge_string> ssl=<X>"
    QRegularExpression challengeRegex("auth cram-md5 (<[^>]+>) ssl=(\\d+)");
    QRegularExpressionMatch match = challengeRegex.match(challenge);

    QString challengeStr;
    int remoteSslValue = 0;

    if (match.hasMatch()) {
        challengeStr = match.captured(1);
        remoteSslValue = match.captured(2).toInt();

        qDebug() << "  Challenge string:" << challengeStr;
        qDebug() << "  Remote SSL value:" << remoteSslValue;

        // WICHTIG: Speichere den Remote SSL-Wert
        // Der Director teilt uns hier mit, was er für TLS unterstützt/braucht
        // Dies ist relevant für die spätere TLS-Negotiation
        //
        // SSL-Werte:
        //   0 = BNET_TLS_NONE     - Kein TLS
        //   1 = BNET_TLS_OK       - TLS möglich
        //   2 = BNET_TLS_REQUIRED - TLS erforderlich
        //
        // HINWEIS: Bei neueren Bacula-Versionen (15.x) wird TLS/PSK über
        // das tlspsk= Feld im Hello verhandelt, nicht über ssl= in CRAM-MD5
        m_tlsRemoteNeed = remoteSslValue;

        qDebug() << "  Stored remote TLS need:" << m_tlsRemoteNeed;

    } else {
        m_errorMessage = QString("Invalid CRAM-MD5 challenge format: %1\n"
                                 "Expected: 'auth cram-md5 <challenge> ssl=X'")
                             .arg(challenge);
        qCritical() << m_errorMessage;
        return false;
    }

    // Compute HMAC-MD5 response
    // WICHTIG: Das Passwort ist der HMAC-Key, die Challenge ist die Message
    QByteArray hmac = hmacMD5(password.toUtf8(), challengeStr.toUtf8());

    // KRITISCHE KORREKTUR: Base64, NICHT Hex!
    // Bacula verwendet Base64-kodierte HMAC-MD5 Responses ohne Padding
    QString responseBase64 = baculaBase64Encode(hmac);

    qDebug() << ">>> Sending response:";
    qDebug() << "    Base64: " << responseBase64;
    qDebug() << "    Hex:    " << hmac.toHex();
    qDebug() << "    (Password length:" << password.length() << "chars)";

    // Sende Response MIT Bacula-Paketformat
    if (!sendBaculaMessage(responseBase64 + "\n")) {
        m_errorMessage = "Failed to send CRAM-MD5 response";
        return false;
    }

    qDebug() << "✓ CRAM-MD5 response sent successfully";

    // HINWEIS: Wir warten hier NICHT auf eine Bestätigung!
    // Der Director antwortet erst nach unserer Challenge-Phase (cramMD5Challenge)
    // mit dem Gesamtergebnis beider Authentifizierungen.
    // Wenn unsere Response falsch war, erhalten wir die Fehlermeldung
    // als Antwort auf unsere Challenge.

    return true;
}

/**
 * @brief Challenges the Director with CRAM-MD5 and verifies response
 *
 * This method implements the second phase of CRAM-MD5 authentication where
 * the client challenges the Director to prove its identity.
 *
 * @par CRAM-MD5 Challenge Algorithm:
 * 1. Generate a random challenge: `<random.timestamp@bconsole>`
 * 2. Send challenge with our TLS capabilities
 * 3. Wait for Director's response
 * 4. Verify response matches expected HMAC-MD5
 *
 * @par Message Format:
 * - **Sent**: `auth cram-md5 <random.timestamp@bconsole> ssl=X`
 * - **Expected**: `<Base64-encoded-HMAC-MD5>` (Director's response)
 * - **Error**: `1999 Authorization failed.` (if our Phase 1 response was wrong)
 *
 * @param password The shared secret for HMAC-MD5 calculation
 *
 * @return true if Director's response matches expected value
 * @return false if verification failed or error message received
 *
 * @note If the Director sends "1999 Authorization failed", it means our
 *       response in cramMD5Respond() was incorrect (wrong password)
 * @note The ssl= value reflects our local TLS requirements
 *
 * @par Error Detection:
 * This method checks for various error responses:
 * - `1999 Authorization failed.` - Our CRAM-MD5 response was wrong
 * - `2999 ...` - Connection rejected
 * - `failed`, `error` - Generic failure messages
 *
 * @par Troubleshooting:
 * If authentication fails with "1999 Authorization failed":
 * - Verify password matches Console resource in bacula-dir.conf
 * - Check for encoding issues with special characters
 * - Ensure Console name matches exactly
 *
 * @see cramMD5Respond()
 * @see hmacMD5()
 *
 * @since 1.0.0
 * @version 1.2.0 - Added error response detection, improved diagnostics,
 *                  fixed SSL value handling for PSK mode
 */
bool BaculaAuth::cramMD5Challenge(const QString &password)
{
    qDebug() << "--- CRAM-MD5 Challenge Phase ---";

    // Generate random challenge
    // Bacula verwendet: <random.timestamp@hostname>
    quint32 rand1 = QRandomGenerator::global()->generate();
    qint64 now = QDateTime::currentSecsSinceEpoch();

    // Challenge format: <random.timestamp@hostname>
    // HINWEIS: Der Hostname sollte idealerweise der Console-Name sein
    QString challenge = QString("<%1.%2@bconsole>").arg(rand1).arg(now);

    qDebug() << "Generated challenge:" << challenge;

    // KORREKTUR: Der SSL-Wert muss den TLS-Status korrekt widerspiegeln
    //
    // Bacula SSL-Werte (aus cram-md5.c):
    //   BNET_TLS_NONE     = 0  -> Kein TLS
    //   BNET_TLS_OK       = 1  -> TLS möglich aber nicht erforderlich
    //   BNET_TLS_REQUIRED = 2  -> TLS erforderlich
    //
    // WICHTIG: Wenn der Director ssl=1 gesendet hat und wir PSK verwenden wollen,
    // sollten wir auch entsprechend antworten.

    int sslValue = m_tlsLocalNeed;

    // Wenn PSK aktiv ist und TLS nicht, verwende den PSK-Wert
    // (PSK wird über das tlspsk= Feld im Hello verhandelt, nicht hier)
    // Aber der ssl-Wert sollte trotzdem korrekt sein
    if (m_tlsLocalNeed == BNET_TLS_NONE && m_pskLocalNeed > BNET_TLS_NONE) {
        // Bei reinem PSK ohne TLS: ssl=0 ist korrekt
        sslValue = BNET_TLS_NONE;
    }

    // Debug-Ausgabe
    qDebug() << "  TLS local need:" << m_tlsLocalNeed;
    qDebug() << "  PSK local need:" << m_pskLocalNeed;
    qDebug() << "  TLS remote need:" << m_tlsRemoteNeed;
    qDebug() << "  PSK remote need:" << m_pskRemoteNeed;
    qDebug() << "  SSL value to send:" << sslValue;

    // Send challenge with TLS info
    QString challengeMsg = QString("auth cram-md5 %1 ssl=%2\n")
                               .arg(challenge)
                               .arg(sslValue);

    qDebug() << ">>> Sending challenge:" << challengeMsg.trimmed();

    if (!sendBaculaMessage(challengeMsg)) {
        m_errorMessage = "Failed to send CRAM-MD5 challenge";
        return false;
    }

    // Wait for Director's response
    QString dirResponse;
    if (!readBaculaMessage(dirResponse, 5000)) {
        m_errorMessage = "Timeout waiting for Director's CRAM-MD5 response";
        return false;
    }

    qDebug() << "<<< Received Director response:" << dirResponse;

    // WICHTIG: Prüfe zuerst ob es eine Fehlermeldung ist!
    // "1999 Authorization failed." bedeutet dass UNSERE erste Response falsch war
    // Der Director sendet diese Meldung anstelle seiner HMAC-MD5 Antwort
    if (dirResponse.contains("1999") ||
        dirResponse.contains("Authorization failed") ||
        dirResponse.contains("Authorization error") ||
        dirResponse.contains("failed") ||
        dirResponse.startsWith("2999")) {

        m_errorMessage = QString("Director rejected our CRAM-MD5 response: %1\n"
                                 "Mögliche Ursachen:\n"
                                 "  1. Falsches Passwort in der Konfiguration\n"
                                 "  2. Console-Name '%2' nicht in bacula-dir.conf definiert\n"
                                 "  3. Passwort-Encoding-Problem (Sonderzeichen?)\n"
                                 "  4. MD5-Hash vs Klartext-Passwort Verwechslung")
                             .arg(dirResponse.trimmed())
                             .arg("bconsole");  // TODO: Console-Name übergeben
        qCritical() << m_errorMessage;
        return false;
    }

    // Compute expected response
    // WICHTIG: Das Passwort wird als Key verwendet, die Challenge als Message
    QByteArray expectedHmac = hmacMD5(password.toUtf8(), challenge.toUtf8());
    QString expectedBase64 = baculaBase64Encode(expectedHmac);

    qDebug() << "  Expected response (Base64):" << expectedBase64;
    qDebug() << "  Expected response (Hex):" << expectedHmac.toHex();
    qDebug() << "  Received response:" << dirResponse.trimmed();

    // Compare responses
    // Base64 ist case-sensitive, aber trimmen ist wichtig
    QString receivedTrimmed = dirResponse.trimmed();

    if (receivedTrimmed != expectedBase64) {
        // Detaillierte Fehlermeldung
        m_errorMessage = QString("Director's CRAM-MD5 response doesn't match.\n"
                                 "  Expected: %1\n"
                                 "  Received: %2\n"
                                 "  Challenge was: %3\n"
                                 "Dies bedeutet der Director verwendet ein anderes Passwort "
                                 "als wir. Überprüfen Sie die Passwort-Konfiguration auf beiden Seiten.")
                             .arg(expectedBase64)
                             .arg(receivedTrimmed)
                             .arg(challenge);
        qCritical() << m_errorMessage;
        return false;
    }

    qDebug() << "✓ Director's CRAM-MD5 response verified successfully";
    return true;
}

bool BaculaAuth::waitForResponse(int timeoutMs)
{
    if (m_socket->bytesAvailable() > 0 || !m_readBuffer.isEmpty()) {
        return true;
    }

    return m_socket->waitForReadyRead(timeoutMs);
}

bool BaculaAuth::readLine(QString &line, int timeoutMs)
{
    // Check if we have a complete line in buffer
    int newlinePos = m_readBuffer.indexOf('\n');
    if (newlinePos >= 0) {
        line = QString::fromUtf8(m_readBuffer.left(newlinePos)).trimmed();
        m_readBuffer.remove(0, newlinePos + 1);
        return true;
    }

    // Need to read more data
    if (!waitForResponse(timeoutMs)) {
        return false;
    }

    // Read available data
    m_readBuffer.append(m_socket->readAll());

    // Check again for complete line
    newlinePos = m_readBuffer.indexOf('\n');
    if (newlinePos >= 0) {
        line = QString::fromUtf8(m_readBuffer.left(newlinePos)).trimmed();
        m_readBuffer.remove(0, newlinePos + 1);
        return true;
    }

    // No complete line yet, but return what we have
    if (!m_readBuffer.isEmpty()) {
        line = QString::fromUtf8(m_readBuffer).trimmed();
        m_readBuffer.clear();
        return true;
    }

    return false;
}


bool BaculaAuth::startEncryption(bool usePSK)
{
    qDebug() << "========================================";
    qDebug() << "STARTING" << (usePSK ? "PSK-TLS" : "TLS") << "ENCRYPTION";
    qDebug() << "========================================";
    qDebug() << "Password available:" << !m_password.isEmpty();
    qDebug() << "Password length:" << m_password.length();

    QSslConfiguration sslConfig;

    if (usePSK) {
        // =====================================================
        // PSK-TLS Konfiguration
        // =====================================================

        // 1. PSK-Callback registrieren (WICHTIG: VOR startClientEncryption!)
        //    Kopie des Passworts für das Lambda erstellen
        QString password = m_password;

        QObject::connect(m_socket, &QSslSocket::preSharedKeyAuthenticationRequired,
                         this, [password](QSslPreSharedKeyAuthenticator *authenticator) {

                             qDebug() << "========================================";
                             qDebug() << "!!! PSK CALLBACK TRIGGERED !!!";
                             qDebug() << "========================================";
                             qDebug() << "  Identity hint:" << authenticator->identityHint();
                             qDebug() << "  Max identity length:" << authenticator->maximumIdentityLength();
                             qDebug() << "  Max PSK length:" << authenticator->maximumPreSharedKeyLength();
                             qDebug() << "  Password length:" << password.length();

                             if (password.isEmpty()) {
                                 qCritical() << "  ERROR: Password is empty!";
                                 return;
                             }

                             // Bacula PSK: Identity ist LEER, PSK ist das Passwort als UTF-8
                             QByteArray identity;  // Leer für Bacula!
                             QByteArray psk = password.toUtf8();

                             authenticator->setIdentity(identity);
                             authenticator->setPreSharedKey(psk);

                             qDebug() << "  ✓ PSK credentials set!";
                             qDebug() << "  Identity: (empty)";
                             qDebug() << "  PSK: [" << psk.length() << " bytes]";
                             qDebug() << "========================================";

                         }, Qt::DirectConnection);  // DirectConnection ist wichtig!

        qDebug() << "✓ PSK callback registered";

        // 2. TLS-Protokoll: PSK funktioniert mit TLS 1.0, 1.1, 1.2 (NICHT 1.3!)
        sslConfig.setProtocol(QSsl::TlsV1_2);

        // 3. Peer-Verifizierung deaktivieren (PSK braucht keine Zertifikate)
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);

        // 4. NUR PSK-Cipher verwenden (wie Bacula)
        QList<QSslCipher> pskCiphers;
        for (const QSslCipher &cipher : QSslConfiguration::supportedCiphers()) {
            // Bacula verwendet "PSK-AES256-CBC-SHA"
            if (cipher.name() == "PSK-AES256-CBC-SHA") {
                pskCiphers.append(cipher);
                qDebug() << "  Using Bacula PSK cipher:" << cipher.name();
                break;  // Nur diesen einen Cipher!
            }
        }

        if (pskCiphers.isEmpty()) {
            // Fallback: Alle PSK-Ciphers versuchen
            for (const QSslCipher &cipher : QSslConfiguration::supportedCiphers()) {
                if (cipher.name().startsWith("PSK-")) {
                    pskCiphers.append(cipher);
                    qDebug() << "  Fallback PSK cipher:" << cipher.name();
                }
            }
        }

        if (pskCiphers.isEmpty()) {
            m_errorMessage = "No PSK ciphers available on this system!";
            qCritical() << m_errorMessage;
            qCritical() << "Your OpenSSL/Qt may not support PSK ciphers.";
            return false;
        }

        sslConfig.setCiphers(pskCiphers);
        qDebug() << "PSK ciphers configured:" << pskCiphers.size();

    } else {
        // =====================================================
        // Reguläres TLS (mit Zertifikaten)
        // =====================================================

        sslConfig.setProtocol(QSsl::TlsV1_2OrLater);
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);  // Oder VerifyPeer mit Zertifikaten

        qDebug() << "Using standard TLS configuration";
    }

    // Konfiguration anwenden
    m_socket->setSslConfiguration(sslConfig);

    qDebug() << "Starting client encryption...";

    // TLS-Handshake starten
    m_socket->startClientEncryption();

    // Auf verschlüsselte Verbindung warten
    if (!m_socket->waitForEncrypted(10000)) {
        m_errorMessage = QString("%1 encryption failed: %2")
        .arg(usePSK ? "PSK-TLS" : "TLS")
            .arg(m_socket->errorString());
        qCritical() << m_errorMessage;

        // Debug-Info
        // qDebug() << "SSL Errors:";
        // for (const QSslError &error : m_socket->sslErrors() ) {
        //     qDebug() << "  -" << error.errorString();
        // }

        return false;
    }

    qDebug() << "========================================";
    qDebug() << "✓" << (usePSK ? "PSK-TLS" : "TLS") << "ENCRYPTION ESTABLISHED";
    qDebug() << "  Protocol:" << m_socket->sessionProtocol();
    qDebug() << "  Cipher:" << m_socket->sessionCipher().name();
    qDebug() << "  Encrypted:" << m_socket->isEncrypted();
    qDebug() << "========================================";

    return true;
}

bool BaculaAuth::clientEarlyTLS()
{
    // Based on ClientEarlyTLS() from authenticate.c

    m_checkEarlyTLS = true;

    QString response;
    if (!readLine(response, 5000)) {
        m_errorMessage = "Timeout waiting for Director's response after Hello";
        qCritical() << m_errorMessage;
        return false;
    }

    qDebug() << "<<< Director response:" << response;

    // Check for "starttls tlspsk=X" message
    QRegularExpression startTlsRegex("starttls tlspsk=(\\d+)");
    QRegularExpressionMatch match = startTlsRegex.match(response);

    if (match.hasMatch()) {
        int tlspskRemote = match.captured(1).toInt();
        decodeRemoteTLSPSKNeed(tlspskRemote);

        if (!handleTLS()) {
            return false;
        }

        m_checkEarlyTLS = false;  // TLS handled, next read should be CRAM-MD5

    } else if (response.contains("auth cram-md5")) {
        // Director sent CRAM-MD5 challenge directly (no TLS)
        qDebug() << "Director sent CRAM-MD5 challenge directly, no TLS negotiation";

        // Put this line back in buffer for cramMD5Respond to process
        m_readBuffer.prepend(response.toUtf8() + "\n");
        m_checkEarlyTLS = false;

    } else {
        // Unexpected response
        m_errorMessage = QString("Unexpected response from Director: %1").arg(response);
        qCritical() << m_errorMessage;
        return false;
    }

    return true;
}

bool BaculaAuth::handleTLS()
{
    qDebug() << "========================================";
    qDebug() << "HANDLE TLS/PSK";
    qDebug() << "========================================";

    // Check TLS requirements
    TLSRequirementResult result = testTLSRequirement();

    switch (result) {
    case TLS_REQ_ERR_LOCAL:
        m_errorMessage = "Local TLS requirements not met by Director";
        qCritical() << m_errorMessage;
        return false;

    case TLS_REQ_ERR_REMOTE:
        m_errorMessage = "Director's TLS requirements not met";
        qCritical() << m_errorMessage;
        return false;

    case TLS_REQ_OK:
        qDebug() << "✓ TLS requirements OK";
        break;
    }

    // Determine if we should start TLS or PSK
    if (m_tlsLocalNeed >= BNET_TLS_OK && m_tlsRemoteNeed >= BNET_TLS_OK) {
        qDebug() << "Starting regular TLS encryption";

        // QSslSocket should already be encrypted if connectToHostEncrypted was used
        if (!m_socket->isEncrypted()) {
            qDebug() << "Socket not encrypted yet, starting encryption now...";
            m_socket->startClientEncryption();

            if (!m_socket->waitForEncrypted(30000)) {
                m_errorMessage = QString("TLS encryption failed: %1").arg(m_socket->errorString());
                qCritical() << m_errorMessage;
                return false;
            }
            qDebug() << "✓ TLS encryption established";
        } else {
            qDebug() << "✓ Socket already encrypted";
        }
        m_tlsStarted = true;

    } else if (m_pskLocalNeed >= BNET_TLS_OK && m_pskRemoteNeed >= BNET_TLS_OK) {
        qDebug() << "Starting PSK (Pre-Shared Key) TLS encryption";

        // PSK-TLS requires special configuration that should have been
        // set up in BaculaDirector before calling this

        if (!m_socket->isEncrypted()) {
            qDebug() << "Starting PSK encryption...";

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
            m_socket->startClientEncryption();

            if (!m_socket->waitForEncrypted(30000)) {
                m_errorMessage = QString("PSK-TLS encryption failed: %1").arg(m_socket->errorString());
                qCritical() << m_errorMessage;
                return false;
            }
            qDebug() << "✓ PSK-TLS encryption established";
#else
            qWarning() << "⚠ PSK-TLS not supported in Qt < 5.12, using plain connection";
            qWarning() << "   Upgrade to Qt 5.12+ for proper PSK support";
#endif
        } else {
            qDebug() << "✓ Socket already encrypted (PSK)";
        }

        m_tlsStarted = true;

    } else {
        qDebug() << "Using clear text connection (no encryption)";
        m_tlsStarted = false;
    }

    qDebug() << "TLS/PSK handling complete. Encrypted:" << m_socket->isEncrypted();
    qDebug() << "========================================";
    return true;
}

// ============================================================================
// CRAM-MD5 Authentication
// ============================================================================

/**
 * @brief Performs the complete CRAM-MD5 authentication sequence
 *
 * This method orchestrates the two-phase CRAM-MD5 authentication:
 * 1. Respond to Director's challenge (prove our identity)
 * 2. Challenge the Director (verify Director's identity)
 *
 * @par Authentication Flow:
 * @code
 *   Client                          Director
 *     |                                |
 *     |   <--- Challenge + ssl=X ---   |  (Director challenges us)
 *     |   --- HMAC-MD5 Response --->   |  (We prove our identity)
 *     |   --- Our Challenge ------->   |  (We challenge Director)
 *     |   <--- HMAC-MD5 Response ---   |  (Director proves identity)
 *     |                                |   OR
 *     |   <--- "1999 Auth failed" --   |  (Our response was wrong)
 * @endcode
 *
 * @param password The shared secret for HMAC-MD5 calculation
 *
 * @return true if mutual authentication succeeded
 * @return false if either phase failed
 *
 * @note Based on ClientCramMD5AuthenticateBase() from Bacula's authenticatebase.cc
 * @note Both phases must succeed for authentication to be complete
 *
 * @see cramMD5Respond()
 * @see cramMD5Challenge()
 *
 * @since 1.0.0
 */
bool BaculaAuth::clientCramMD5Authenticate(const QString &password)
{
    // Based on ClientCramMD5AuthenticateBase() from authenticatebase.cc

    qDebug() << "========================================";
    qDebug() << "CRAM-MD5 AUTHENTICATION";
    qDebug() << "========================================";

    // Step 1: Respond to Director's challenge
    if (!cramMD5Respond(password)) {
        m_errorMessage = "CRAM-MD5 respond failed";
        qCritical() << m_errorMessage;
        return false;
    }

    // Step 2: Challenge the Director
    if (!cramMD5Challenge(password)) {
        m_errorMessage = "CRAM-MD5 challenge failed";
        qCritical() << m_errorMessage;
        return false;
    }

    qDebug() << "✓ CRAM-MD5 authentication successful";
    qDebug() << "========================================";
    return true;
}

/**
 * @brief Computes HMAC-MD5 hash
 *
 * Calculates the HMAC-MD5 (Hash-based Message Authentication Code using MD5)
 * as specified in RFC 2104.
 *
 * @param key The secret key (password in CRAM-MD5 context)
 * @param data The message to authenticate (challenge in CRAM-MD5 context)
 *
 * @return QByteArray 16-byte HMAC-MD5 digest
 *
 * @par Algorithm:
 * HMAC-MD5(K, m) = MD5((K ⊕ opad) || MD5((K ⊕ ipad) || m))
 *
 * @note Uses Qt's QMessageAuthenticationCode for the calculation
 * @note The result is binary data - use baculaBase64Encode() for transmission
 *
 * @see baculaBase64Encode()
 * @see https://tools.ietf.org/html/rfc2104
 *
 * @since 1.0.0
 */
QByteArray BaculaAuth::hmacMD5(const QByteArray &key, const QByteArray &data)
{
    return QMessageAuthenticationCode::hash(data, key, QCryptographicHash::Md5);
}

// ============================================================================
// Utility Methods
// ============================================================================

/**
 * @brief Converts spaces in a string to Bacula escape sequence
 *
 * Bacula uses 0x01 as a space replacement in certain protocol messages
 * to avoid parsing issues with space-separated fields.
 *
 * @param str Input string possibly containing spaces
 *
 * @return QString String with spaces replaced by 0x01 characters
 *
 * @par Example:
 * @code
 * QString name = "My Console";
 * QString escaped = bashSpaces(name);  // "My\x01Console"
 * @endcode
 *
 * @since 1.0.0
 */
QString BaculaAuth::bashSpaces(const QString &str)
{
    QString result = str;
    result.replace(' ', (char)0x01); // Bash spaces to 0x01
    return result;
}

/**
 * @brief Starts the authentication timeout timer
 *
 * Starts a single-shot timer that will emit authenticationFailed()
 * if authentication doesn't complete within AUTH_TIMEOUT milliseconds.
 *
 * @note Default timeout is 180 seconds (180000 ms)
 *
 * @see stopAuthTimeout()
 * @see onAuthTimeout()
 * @see AUTH_TIMEOUT
 *
 * @since 1.0.0
 */
void BaculaAuth::startAuthTimeout()
{
    m_authTimer->start(AUTH_TIMEOUT);
    qDebug() << "⏱ Authentication timeout started:" << AUTH_TIMEOUT << "ms";
}

/**
 * @brief Stops the authentication timeout timer
 *
 * Cancels the timeout timer if it is currently running.
 * Should be called when authentication completes (success or failure).
 *
 * @see startAuthTimeout()
 *
 * @since 1.0.0
 */
void BaculaAuth::stopAuthTimeout()
{
    if (m_authTimer->isActive()) {
        m_authTimer->stop();
        qDebug() << "⏱ Authentication timeout stopped";
    }
}

/**
 * @brief Handles authentication timeout
 *
 * This slot is called when the authentication timeout timer expires.
 * It sets the error message and emits the authenticationFailed() signal.
 *
 * @note This is a private slot connected to m_authTimer::timeout
 *
 * @see startAuthTimeout()
 * @see authenticationFailed()
 *
 * @since 1.0.0
 */
void BaculaAuth::onAuthTimeout()
{
    qCritical() << "========================================";
    qCritical() << "⏱ AUTHENTICATION TIMEOUT!";
    qCritical() << "========================================";
    m_errorMessage = "Authentication timeout - Director not responding";
    emit authenticationFailed(m_errorMessage);
}
