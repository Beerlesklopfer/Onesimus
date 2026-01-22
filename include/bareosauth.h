/**
 * @file bareosauth.h
 * @brief Qt-based Bareos Authentication Implementation
 *
 * This file provides a Qt-based implementation of the Bareos authentication
 * protocol, including CRAM-MD5 challenge-response authentication and TLS/PSK
 * negotiation. Bareos is a fork of Bacula with extended features.
 *
 * @author Original Bacula code by Kern Sibbald
 * @author Bareos modifications by Bareos GmbH & Co. KG
 * @author Qt port by [Your Name]
 *
 * @version 1.0.0
 * @date 2025-01-22
 *
 * @copyright Copyright (c) 2025
 * @license AGPL-3.0-or-later
 *
 * Based on Bareos's cram_md5.cc, bsock.cc and console.cc
 *
 * @par Key Differences from Bacula:
 * - TLS-PSK is established BEFORE CRAM-MD5 authentication (since Bareos 18.2)
 * - Resource names use format: R_DIRECTOR::name, R_CONSOLE::name, etc.
 * - Supports PAM authentication (optional)
 * - SSL parameter values: 0=none, 1=optional, 2=required (same as Bacula)
 *
 * @see https://www.bareos.org/
 * @see https://github.com/bareos/bareos
 * @see https://docs.bareos.org/DeveloperGuide/tls-techdoc.html
 */

#ifndef BAREOSAUTH_H
#define BAREOSAUTH_H

#include <QObject>
#include <QSslSocket>
#include <QByteArray>
#include <QString>
#include <QTimer>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QDateTime>
#include <QRandomGenerator>
#include <QSslPreSharedKeyAuthenticator>

/**
 * @defgroup BareosAuthVersion Version Information
 * @{
 */
#define BAREOSAUTH_VERSION_MAJOR 1  ///< Major version number
#define BAREOSAUTH_VERSION_MINOR 0  ///< Minor version number
#define BAREOSAUTH_VERSION_PATCH 0  ///< Patch version number

/** @brief Full version string */
#define BAREOSAUTH_VERSION_STRING "1.0.0"

/** @brief Version as integer for comparisons (1.0.0 = 10000) */
#define BAREOSAUTH_VERSION ((BAREOSAUTH_VERSION_MAJOR * 10000) + \
                            (BAREOSAUTH_VERSION_MINOR * 100) + \
                            BAREOSAUTH_VERSION_PATCH)
/** @} */

/**
 * @defgroup BareosProtocolConstants Bareos Protocol Constants
 * @{
 */

/**
 * @brief Default Console identity for anonymous console
 * @details Used when connecting as the default (root) console
 */
#define BAREOS_USERAGENT "*UserAgent*"

/**
 * @brief Authentication timeout in milliseconds
 * @details Maximum time to wait for authentication to complete (180 seconds)
 */
#define BAREOS_AUTH_TIMEOUT 180000

/**
 * @brief Bareos resource type identifiers
 * @details Used in challenge messages: R_TYPE::name
 */
#define BAREOS_R_DIRECTOR  "R_DIRECTOR"
#define BAREOS_R_CONSOLE   "R_CONSOLE"
#define BAREOS_R_CLIENT    "R_CLIENT"
#define BAREOS_R_STORAGE   "R_STORAGE"

/** @} */

/**
 * @defgroup BareosTLSConstants TLS/PSK Constants
 * @brief Constants for TLS negotiation (same values as Bacula)
 * @{
 */

/**
 * @brief TLS not available/disabled (cleartext)
 */
#define BAREOS_TLS_NONE     0

/**
 * @brief TLS available but not required (TLS-Cert possible)
 */
#define BAREOS_TLS_OK       1

/**
 * @brief TLS is required for connection (TLS-Cert required)
 */
#define BAREOS_TLS_REQUIRED 2

/** @} */

/**
 * @defgroup BareosPAMConstants PAM Authentication Constants
 * @brief Constants for PAM (Pluggable Authentication Modules) support
 * @{
 */

/**
 * @brief Record Separator for PAM messages (ASCII 0x1e)
 */
#define BAREOS_PAM_RS       "\x1e"

/**
 * @brief PAM authentication required message code
 */
#define BAREOS_PAM_REQUIRED "1001"

/**
 * @brief PAM interactive authentication request
 */
#define BAREOS_PAM_INTERACTIVE "4001"

/**
 * @brief PAM direct authentication (WebUI style)
 */
#define BAREOS_PAM_DIRECT "4002"

/**
 * @brief PAM result codes
 */
#define BAREOS_PAM_SUCCESS          0x0  ///< Authentication successful
#define BAREOS_PAM_PROMPT_ECHO_OFF  0x1  ///< Hidden input (password)
#define BAREOS_PAM_PROMPT_ECHO_ON   0x2  ///< Visible input (username)

/** @} */

/**
 * @enum BareosTLSRequirementResult
 * @brief Results of TLS requirement negotiation
 */
enum BareosTLSRequirementResult {
    BAREOS_TLS_REQ_OK,         ///< TLS requirements are compatible
    BAREOS_TLS_REQ_ERR_LOCAL,  ///< Local TLS requirements not met by remote
    BAREOS_TLS_REQ_ERR_REMOTE  ///< Remote TLS requirements not met by local
};

/**
 * @brief Converts binary data to Bareos-compatible Base64
 *
 * Bareos uses a standard Base64 encoding but without padding characters.
 * This is used for CRAM-MD5 authentication responses.
 *
 * @param data Binary data to encode
 * @return QString Base64-encoded string without padding
 *
 * @note Bareos's Base64 removes the '=' padding characters
 *
 * @par Example:
 * @code
 * QByteArray hmac = calculateHMAC(...);
 * QString encoded = bareosBase64Encode(hmac);
 * // encoded will be like "NA31sq0e02zQukZvhKKaAQ" (no padding)
 * @endcode
 *
 * @since 1.0.0
 */
static QString bareosBase64Encode(const QByteArray &data)
{
    QString base64 = QString::fromLatin1(data.toBase64());
    base64.remove('=');  // Remove padding
    return base64;
}

/**
 * @class BareosAuth
 * @brief Handles authentication with Bareos Director
 *
 * This class implements the Bareos authentication protocol for connecting
 * a console client to a Bareos Director. It supports:
 *
 * - TLS-PSK (Pre-Shared Key) encryption (established first, since Bareos 18.2)
 * - CRAM-MD5 challenge-response authentication (after TLS)
 * - Certificate-based TLS encryption
 * - PAM (Pluggable Authentication Modules) support
 * - Automatic protocol version negotiation
 *
 * @par Key Differences from Bacula:
 * Since Bareos 18.2, the authentication sequence is:
 * 1. TCP connection
 * 2. TLS-PSK handshake (using password as pre-shared key)
 * 3. Hello message
 * 4. CRAM-MD5 mutual authentication
 * 5. Ready
 *
 * The older sequence (pre-18.2) was Hello -> CRAM-MD5 -> TLS.
 *
 * @par Usage Example:
 * @code
 * QSslSocket *socket = new QSslSocket(this);
 * socket->connectToHost("director.example.com", 9101);
 *
 * BareosAuth *auth = new BareosAuth(socket, this);
 * connect(auth, &BareosAuth::authenticationSucceeded, this, &MyClass::onAuthOk);
 * connect(auth, &BareosAuth::authenticationFailed, this, &MyClass::onAuthFailed);
 *
 * auth->authenticateDirector("bareos-dir", "Console", "password", true, true, true);
 * @endcode
 *
 * @par Authentication Flow (Bareos 18.2+):
 * 1. Client connects via TCP
 * 2. TLS-PSK handshake (identity: R_CONSOLE::ConsoleName, key: MD5(password))
 * 3. Client sends: Hello ConsoleName calling
 * 4. Director sends: auth cram-md5 <challenge> ssl=N
 * 5. Client computes HMAC-MD5 and sends Base64-encoded response
 * 6. Director sends: 1000 OK auth
 * 7. Client sends: auth cram-md5 <challenge> ssl=N
 * 8. Director responds with its HMAC-MD5 response
 * 9. Client sends: 1000 OK auth
 * 10. Director sends: 1000 OK: bareos-dir Version: X.Y.Z
 *
 * @warning The socket must be connected before calling authenticateDirector()
 *
 * @see https://docs.bareos.org/DeveloperGuide/tls-techdoc.html
 * @see https://docs.bareos.org/DeveloperGuide/pam-techdoc.html
 *
 * @version 1.0.0
 * @since 1.0.0
 */
class BareosAuth : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a BareosAuth object
     *
     * @param socket Pointer to a connected QSslSocket
     * @param parent Optional parent QObject
     *
     * @pre socket must not be nullptr
     * @pre socket should be in connected state (TCP connected, not yet TLS)
     *
     * @since 1.0.0
     */
    explicit BareosAuth(QSslSocket *socket, QObject *parent = nullptr);

    /**
     * @brief Destructor
     *
     * Stops any running authentication timeout timer.
     *
     * @since 1.0.0
     */
    virtual ~BareosAuth();

    /**
     * @brief Authenticates with the Bareos Director
     *
     * This is the main entry point for authentication. It performs the complete
     * authentication handshake including TLS-PSK negotiation if enabled.
     *
     * @param directorName Name of the Director (must match Director's config)
     * @param consoleName Name of the Console resource (use "*UserAgent*" for default console)
     * @param password Password for authentication (also used as PSK key)
     * @param tlsEnable Enable TLS encryption
     * @param tlsRequire Require TLS encryption
     * @param tlsPSKEnable Enable PSK-TLS authentication (default: true for Bareos 18.2+)
     *
     * @return true if authentication started successfully, false on immediate failure
     *
     * @retval true Authentication process started
     * @retval false Immediate failure (check getErrorMessage() for details)
     *
     * @note Authentication is performed synchronously
     * @note On success, emits authenticationSucceeded() signal
     * @note On failure, emits authenticationFailed() signal
     *
     * @par TLS-PSK Identity Format:
     * For Bareos, the PSK identity format is: R_CONSOLE::ConsoleName
     * Example: R_CONSOLE::*UserAgent*
     *
     * @see getErrorMessage()
     * @see authenticationSucceeded()
     * @see authenticationFailed()
     *
     * @since 1.0.0
     */
    bool authenticateDirector(const QString &directorName,
                              const QString &consoleName,
                              const QString &password,
                              bool tlsEnable,
                              bool tlsRequire,
                              bool tlsPSKEnable = true);

    /**
     * @brief Gets the last error message
     *
     * @return QString Error message, empty if no error
     *
     * @since 1.0.0
     */
    QString getErrorMessage() const { return m_errorMessage; }

    /**
     * @brief Gets the Director's protocol version
     *
     * @return int Director version number (0 if not yet authenticated)
     *
     * @note Bareos version format in response: "Version: X.Y.Z"
     *
     * @since 1.0.0
     */
    int getDirectorVersion() const { return m_directorVersion; }

    /**
     * @brief Gets the Director's version string
     *
     * @return QString Full version string (e.g., "21.0.0")
     *
     * @since 1.0.0
     */
    QString getDirectorVersionString() const { return m_directorVersionString; }

    /**
     * @brief Gets the library version string
     *
     * @return QString Version string (e.g., "1.0.0")
     *
     * @since 1.0.0
     */
    static QString version() { return BAREOSAUTH_VERSION_STRING; }

    /**
     * @brief Gets the library version as integer
     *
     * @return int Version number for comparisons (e.g., 10000 for 1.0.0)
     *
     * @since 1.0.0
     */
    static int versionNumber() { return BAREOSAUTH_VERSION; }

signals:
    /**
     * @brief Emitted when authentication succeeds
     *
     * @param directorVersion The Director's protocol version number
     *
     * @since 1.0.0
     */
    void authenticationSucceeded(int directorVersion);

    /**
     * @brief Emitted when authentication fails
     *
     * @param reason Human-readable description of the failure
     *
     * @since 1.0.0
     */
    void authenticationFailed(const QString &reason);

    /**
     * @brief Emitted for status updates during authentication
     *
     * @param message Status message
     *
     * @since 1.0.0
     */
    void statusMessage(const QString &message);

    /**
     * @brief Emitted when PAM authentication is required
     *
     * @note Only emitted if Director requires PAM authentication
     *
     * @since 1.0.0
     */
    void pamAuthenticationRequired();

private:
    /// @name Member Variables
    /// @{
    QSslSocket *m_socket;           ///< SSL socket for communication
    QTimer *m_authTimer;            ///< Authentication timeout timer
    QString m_errorMessage;         ///< Last error message
    int m_directorVersion;          ///< Director protocol version (numeric)
    QString m_directorVersionString;///< Director version string

    QString m_directorName;         ///< Director name for authentication
    QString m_consoleName;          ///< Console name for authentication
    QString m_password;             ///< Password for CRAM-MD5 and PSK
    QByteArray m_passwordMD5;       ///< MD5 hash of password for PSK

    int m_tlsLocalNeed;             ///< Local TLS requirement level
    int m_tlsRemoteNeed;            ///< Remote TLS requirement level
    bool m_tlsPSKEnable;            ///< PSK-TLS enabled flag

    bool m_tlsStarted;              ///< TLS encryption active flag
    bool m_authSuccess;             ///< Authentication success flag
    QByteArray m_readBuffer;        ///< Buffer for incoming data
    /// @}

    /// @name TLS/PSK Helper Methods
    /// @{

    /**
     * @brief Calculates the TLS need value for negotiation
     *
     * @param tlsEnable TLS enabled flag
     * @param tlsRequire TLS required flag
     * @return int TLS need value (0, 1, or 2)
     *
     * @since 1.0.0
     */
    int calculateTLSNeed(bool tlsEnable, bool tlsRequire);

    /**
     * @brief Tests if TLS requirements can be met
     *
     * @return BareosTLSRequirementResult Result of requirement check
     *
     * @since 1.0.0
     */
    BareosTLSRequirementResult testTLSRequirement();

    /**
     * @brief Sets up TLS-PSK encryption
     *
     * Configures the socket for PSK-TLS and starts the handshake.
     * The PSK identity is: R_CONSOLE::ConsoleName
     * The PSK key is: MD5(password)
     *
     * @return true if PSK setup initiated successfully
     *
     * @since 1.0.0
     */
    bool setupPSKTLS();

    /**
     * @brief Handles PSK authentication callback
     *
     * Called by Qt when the PSK authenticator needs credentials.
     *
     * @param authenticator The PSK authenticator to fill in
     *
     * @since 1.0.0
     */
    void handlePskAuthenticator(QSslPreSharedKeyAuthenticator *authenticator);
    /// @}

    /// @name Protocol Methods
    /// @{

    /**
     * @brief Creates a Bareos protocol packet
     *
     * Bareos uses a 4-byte big-endian length prefix followed by the message.
     *
     * @param message Message to encode
     * @return QByteArray Encoded packet
     *
     * @since 1.0.0
     */
    QByteArray createBareosPacket(const QString &message);

    /**
     * @brief Sends a message using Bareos protocol
     *
     * @param message Message to send
     * @return true if message was sent successfully
     *
     * @since 1.0.0
     */
    bool sendBareosMessage(const QString &message);

    /**
     * @brief Reads a message using Bareos protocol
     *
     * @param message Output: received message
     * @param timeoutMs Timeout in milliseconds
     * @return true if message was received successfully
     *
     * @since 1.0.0
     */
    bool readBareosMessage(QString &message, int timeoutMs = 5000);

    /**
     * @brief Waits for data to be available
     *
     * @param timeoutMs Timeout in milliseconds
     * @return true if data is available
     *
     * @since 1.0.0
     */
    bool waitForResponse(int timeoutMs = 180000);

    /**
     * @brief Performs CRAM-MD5 authentication sequence
     *
     * For Bareos, this is performed AFTER TLS-PSK is established.
     *
     * @param password Password for CRAM-MD5
     * @return true if authentication succeeded
     *
     * @since 1.0.0
     */
    bool clientCramMD5Authenticate(const QString &password);
    /// @}

    /// @name CRAM-MD5 Implementation
    /// @{

    /**
     * @brief Responds to Director's CRAM-MD5 challenge
     *
     * Parses the challenge, computes HMAC-MD5, and sends response.
     *
     * @param password Password for HMAC computation
     * @return true if response accepted
     *
     * @since 1.0.0
     */
    bool cramMD5Respond(const QString &password);

    /**
     * @brief Challenges the Director with CRAM-MD5
     *
     * Sends a challenge and verifies the Director's response.
     *
     * @param password Password for HMAC computation
     * @return true if Director's response is valid
     *
     * @since 1.0.0
     */
    bool cramMD5Challenge(const QString &password);

    /**
     * @brief Generates a CRAM-MD5 challenge string
     *
     * Format: <random.timestamp@R_CONSOLE::ConsoleName>
     *
     * @return QString Challenge string
     *
     * @since 1.0.0
     */
    QString generateChallenge();

    /**
     * @brief Computes HMAC-MD5 hash
     *
     * @param key Secret key (password)
     * @param data Data to hash (challenge)
     * @return QByteArray 16-byte HMAC-MD5 result
     *
     * @since 1.0.0
     */
    QByteArray hmacMD5(const QByteArray &key, const QByteArray &data);
    /// @}

    /// @name Utility Methods
    /// @{

    /**
     * @brief Converts spaces to Bareos escape sequence
     *
     * @param str Input string
     * @return QString String with spaces escaped as 0x1
     *
     * @since 1.0.0
     */
    QString bashSpaces(const QString &str);

    /**
     * @brief Starts the authentication timeout timer
     *
     * @since 1.0.0
     */
    void startAuthTimeout();

    /**
     * @brief Stops the authentication timeout timer
     *
     * @since 1.0.0
     */
    void stopAuthTimeout();

    /**
     * @brief Parses the Director version from response
     *
     * @param response Response string containing version info
     * @return true if version was parsed successfully
     *
     * @since 1.0.0
     */
    bool parseDirectorVersion(const QString &response);
    /// @}

private slots:
    /**
     * @brief Handles authentication timeout
     *
     * @since 1.0.0
     */
    void onAuthTimeout();

    /**
     * @brief Handles TLS encryption established
     *
     * @since 1.0.0
     */
    void onEncrypted();

    /**
     * @brief Handles SSL errors during PSK handshake
     *
     * @param errors List of SSL errors
     *
     * @since 1.0.0
     */
    void onSslErrors(const QList<QSslError> &errors);

    /**
     * @brief Handles PSK authentication request
     *
     * @param authenticator PSK authenticator to fill
     *
     * @since 1.0.0
     */
    void onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator);
};

#endif // BAREOSAUTH_H
