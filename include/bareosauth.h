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
 * @author Qt port by Jörg Bernau
 *
 * @version 1.0.0
 * @date 2025-01-22
 *
 * @copyright Copyright (c) 2025
 * @license AGPL-3.0-or-later
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

#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/md5.h>

constexpr int PAD_LEN = 64; // HMAC block size
constexpr int SIG_LEN = 16; // MD5 output size

static uint8_t constexpr base64_digits[64]
    = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
       'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
       'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
       'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
       '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

#define MAXHOSTNAMELEN 50
#define MD5ENCODEDLEN  16

#if defined(HAVE_GCC)
#  define IGNORE_DEPRECATED_ON      \
_Pragma("GCC diagnostic push"); \
    _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"");
#  define IGNORE_DEPRECATED_OFF _Pragma("GCC diagnostic pop")
#elif defined(HAVE_CLANG)
#  define IGNORE_DEPRECATED_ON        \
_Pragma("clang diagnostic push"); \
    _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"");
#  define IGNORE_DEPRECATED_OFF _Pragma("clang diagnostic pop")
#elif defined(HAVE_MSVC)
#  define IGNORE_DEPRECATED_ON  \
_Pragma("warning( push )"); \
    _Pragma("warning( disable : 4996 )")
#  define IGNORE_DEPRECATED_OFF _Pragma("warning( pop )")
#else
#  define IGNORE_DEPRECATED_ON
#  define IGNORE_DEPRECATED_OFF
#endif

/**
 * @defgroup BareosAuthVersion Version Information
 * @{
 */
#define BAREOSAUTH_VERSION_MAJOR 1 ///< Major version number
#define BAREOSAUTH_VERSION_MINOR 0 ///< Minor version number
#define BAREOSAUTH_VERSION_PATCH 0 ///< Patch version number

#ifndef BAREOS_VERSION_STR
#define BAREOS_VERSION_STR "25.0.0"
#endif

/** @brief Full version string */
#define BAREOSAUTH_VERSION_STRING "1.0.0"

/** @brief Version as integer for comparisons (1.0.0 = 10000) */
#define BAREOSAUTH_VERSION ((BAREOSAUTH_VERSION_MAJOR * 10000) + \
                            (BAREOSAUTH_VERSION_MINOR * 100) +   \
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
#define BAREOS_R_DIRECTOR "R_DIRECTOR"
#define BAREOS_R_CONSOLE "R_CONSOLE"
#define BAREOS_R_CLIENT "R_CLIENT"
#define BAREOS_R_STORAGE "R_STORAGE"

/** @} */

/**
 * @defgroup BareosTLSConstants TLS/PSK Constants
 * @brief Constants for TLS negotiation (same values as Bacula)
 * @{
 */

/**
 * @brief TLS not available/disabled (cleartext)
 */
#define BAREOS_TLS_NONE 0

/**
 * @brief TLS available but not required (TLS-Cert possible)
 */
#define BAREOS_TLS_OK 1

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
#define BAREOS_PAM_RS "\x1e"

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
#define BAREOS_PAM_SUCCESS 0x0         ///< Authentication successful
#define BAREOS_PAM_PROMPT_ECHO_OFF 0x1 ///< Hidden input (password)
#define BAREOS_PAM_PROMPT_ECHO_ON 0x2  ///< Visible input (username)

/** @} */

/**
 * @enum BareosTLSRequirementResult
 * @brief Results of TLS requirement negotiation
 */
enum BareosTLSRequirementResult
{
    BAREOS_TLS_REQ_OK,        ///< TLS requirements are compatible
    BAREOS_TLS_REQ_ERR_LOCAL, ///< Local TLS requirements not met by remote
    BAREOS_TLS_REQ_ERR_REMOTE ///< Remote TLS requirements not met by local
};

/**
 * @brief Zustände der Bareos-Authentifizierung (CRAM-MD5 Handshake).
 *
 * Diese Enum beschreibt alle möglichen Zustände während
 * der bidirektionalen Authentifizierung zwischen Client und Director.
 *
 * Ablauf:
 *   AUTH_IDLE → WAIT_FOR_CHALLENGE → COMPUTING_RESPONSE
 *   → WAIT_FOR_DIRECTOR_HMAC → WAIT_FOR_FINAL_OK → AUTH_SUCCESS
 *
 * @since 1.0.0
 */
enum class BAuthState
{
    // Normale Zustände im Auth-Flow
    AUTH_IDLE,                  /**< Startzustand, noch kein Hello gesendet */
    WAIT_FOR_CHALLENGE,         /**< Hello gesendet, warte auf Director-Challenge */
    COMPUTING_RESPONSE,         /**< Director-Challenge empfangen, berechne HMAC-Response */
    WAIT_FOR_DIRECTOR_HMAC,     /**< Unsere Challenge gesendet, warte auf Director's HMAC-Antwort */
    WAIT_FOR_FINAL_OK,          /**< "1000 OK auth" gesendet, warte auf Director's finale Bestätigung */
    AUTH_SUCCESS,               /**< Authentifizierung erfolgreich abgeschlossen */

    // Fehlerzustände
    AUTH_FAILED,                /**< Authentifizierung fehlgeschlagen (allgemein) */
    AUTH_ERROR_FORMAT,          /**< Protokoll-/Formatfehler */
    AUTH_ERROR_HASH,            /**< HMAC-Verifikation fehlgeschlagen */
    AUTH_ERROR_REPLAY           /**< Replay-Angriff erkannt */
};

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
 *
 * @par Authentication Flow (Bareos 18.2+):
 * 1. Client connects via TCP
 * 2. Optional: TLS-PSK handshake or SSL/TLS establishment
 *
 * 3. Console → Director: "Hello <consolename> calling version <version>"
 *    onBytesWritten: IDLE → HELLO_SENT
 *
 * 4. Director → Console: "auth cram-md5 <DIR_CHALLENGE> ssl=0"
 *    onReadyRead: Process Director's challenge
 *
 * 5. Console → Director: "base64(HMAC(DIR_CHALLENGE, password))"
 *    onBytesWritten: HELLO_SENT → DIRECTOR_CHALLENGE_RECEIVED
 *                    Triggers cramMD5Challenge()
 *
 * 6. Console → Director: "auth cram-md5 <CON_CHALLENGE> ssl=0"
 *    onBytesWritten: DIRECTOR_CHALLENGE_RECEIVED → CLIENT_CHALLENGE_SENT
 *
 * 7. Director → Console: "base64(HMAC(CON_CHALLENGE, password))" or "1000 OK auth"
 *    onReadyRead: Verify Director's HMAC (if mutual auth)
 *
 * 8. Console → Director: "1000 OK auth\n"
 *    onBytesWritten: CLIENT_CHALLENGE_SENT → DIRECTOR_RESPONSE_RECEIVED
 *
 * 9. Director → Console: "1000 OK: bareos-dir Version: <version> (<date>)"
 *    onReadyRead: Parse version → AUTHENTICATED
 *
 * 10. Director → Console: "1002 <info message>" (optional)
 *     onReadyRead: Ignored in AUTHENTICATED state
 *
 * @note If Director does not require mutual authentication, step 7 returns
 *       "1000 OK auth" instead of HMAC, and step 8 is skipped.
 *
 * @warning The socket must be connected before calling authenticateDirector()
 *
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
                              bool tlsVerifyPeer = false,
                              bool tlsPSKEnable = false);

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

    /**
     * @brief Sets the certificate files for TLS-Certificate mode
     *
     * @param caCertFile Path to CA certificate file (.pem)
     * @param clientCertFile Path to client certificate file (.pem)
     * @param clientKeyFile Path to client private key file (.pem/.key)
     *
     * @since 1.0.0
     */
    void setCertificateFiles(const QString &caCertFile,
                             const QString &clientCertFile,
                             const QString &clientKeyFile)
    {
        m_tlsCaFile = caCertFile;
        m_tlsCertFile = clientCertFile;
        m_tlsKeyFile = clientKeyFile;
    }

    enum class BnetStatus
    {
        Ok,         // Normale Daten
        Empty,      // Leere Nachricht (pktsiz == 0)
        Signal,     // Protokoll-Signal (pktsiz < 0)
        Terminated, // TERMINATE / Verbindung beendet
        Eof,        // Peer hat Verbindung geschlossen
        Error       // Socket- oder Protokollfehler
    };
    Q_ENUM(BnetStatus)

public slots:

    /**
     * @brief Copies data to m_writeBuffer and sends them via @ref send()
     *
     * Each message is prefixed by a 4-byte signed big-endian length.
     *
     * @return BnetStatus describing the result
     *
     * @since 1.0.0
     * @version 1.0.0
     */
    void send(const QString &data);

signals:
    /**
     * @brief Emitted when authentication succeeds
     *
     * @param directorVersion The Director's protocol version number
     *
     * @since 1.0.0
     */
    void authenticationSucceeded(const QString &directorVersion);

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

    /// @name Connections als Member speichern
    /// @{
    QMetaObject::Connection m_readyReadConn;
    QMetaObject::Connection m_bytesWrittenConn;
    QMetaObject::Connection m_pskConn;
    QMetaObject::Connection m_encryptedConn;
    QMetaObject::Connection m_sslErrorsConn;
    /// @}

    /// @name Private Member Variables
    /// @{
    QSslSocket *m_socket;            ///< SSL socket for communication
    QByteArray m_readBuffer;         ///< buffer for readings from socket
    QByteArray m_writeBuffer;        ///< buffer for writings to socket
    QTimer *m_authTimer;             ///< Authentication timeout timer
    QString m_errorMessage;          ///< Last error message
    int m_directorVersion;       ///< Director protocol version (numeric)
    QString m_directorVersionString; ///< Director version string

    /**
     * @brief Compatibility mode flag for base64 encoding
     *
     * Determines whether to use compatible or standard base64 encoding.
     * Extracted from Director's challenge message ('c' suffix).
     */
    bool m_isCompatible;

    /**
     * @brief Stores the client challenge sent during mutual authentication
     *
     * Format: "<random_number>.<timestamp>@<hostname>"
     * Used by verifyDirectorResponse() to validate Director's HMAC
     */
    QByteArray m_clientChallenge;

    /**
     * @brief Stores the Director's challenge received during authentication
     *
     * Format: "<random_number>.<timestamp>@R_DIRECTOR::<director_name>"
     * Used to send our HMAC response after Director verification
     */
    QString m_directorChallenge;

    qint64 m_lastSentSize;

    QString m_directorName; ///< Director name for authentication
    QString m_consoleName;  ///< Console name for authentication
    QByteArray m_password;  ///< Password for CRAM-MD5 and PSK

    BAuthState m_authState; ///< Aktueller Zustand der Authentifizierung
    int m_tlsLocalNeed;     ///< Local TLS requirement level
    int m_tlsRemoteNeed;    ///< Remote TLS requirement level

    bool m_tlsLocalEnable;  ///< Local TLS enabled flag
    bool m_tlsLocalRequire; ///< Local TLS requirement flag
    bool m_tlsVerifyPeer;   ///< TLS peer verifivation enabled flag
    bool m_tlsPSKEnable;    ///< PSK-TLS enabled flag

    bool m_tlsStarted;       ///< TLS encryption active flag
    bool m_authSuccess;      ///< Authentication success flag

    // Certificate paths for TLS-Certificate mode
    QString m_tlsCaFile;     ///< Path to CA certificate file
    QString m_tlsCertFile;   ///< Path to client certificate file
    QString m_tlsKeyFile;    ///< Path to client private key file
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
     * @brief Sets up certificate-based TLS encryption
     *
     * Configures the socket for certificate-based TLS and starts the handshake.
     * Used for traditional TLS authentication with X.509 certificates.
     *
     * @return true if TLS setup initiated successfully
     *
     * @since 1.0.0
     */
    bool setupCertificateTLS();

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
     * @brief Sends a formated m_writeBuffer using the Bareos protocol
     *
     * Each message is prefixed by a 4-byte signed big-endian length.
     *
     * @param[in] data The payload to send (QByteArray)
     * @return BnetStatus describing the result
     *
     * @since 1.0.0
     * @version 1.0.0
     */
    BnetStatus send();

    /**
     * @brief Receives a message to m_readBuffer using Bareos protocol
     *
     * @return BnetStatus describing the result
     */
    // BnetStatus read();

    /// @}

    /// @name CRAM-MD5 Implementation
    /// @{

    /**
     * @brief Sends a CRAM-MD5 challenge to the Director for mutual authentication
     *
     * Creates a challenge string in the format "<random>.<timestamp>@<hostname>",
     * stores it in m_clientChallenge, and sends it to the Director. The Director
     * must respond with HMAC(challenge, password) which is verified by verifyDirectorResponse().
     *
     * @return true if challenge was sent successfully, false on error
     *
     * @note Only called if Director requests mutual authentication
     * @note Challenge is stored in m_clientChallenge for later verification
     * @see verifyDirectorResponse()
     *
     * @since 1.0.0
     * @version 1.0.0
     */
    bool cramMD5Challenge();

    /**
 * @brief Processes the Director's CRAM-MD5 challenge and sends HMAC response
 *
 * Parses the Director's challenge message in the format:
 * "auth cram-md5[c] <challenge> ssl=N"
 * where [c] indicates compatibility mode (base64 encoding variant).
 *
 * Extracts the challenge string, calculates HMAC-MD5 using the password,
 * encodes it as base64, and sends it back to the Director. Also extracts
 * the Director's TLS requirement level from the ssl parameter.
 *
 * @param challenge Raw challenge message from Director
 * @return true if challenge was processed and response sent successfully, false on error
 *
 * @note Sets m_tlsRemoteNeed based on the ssl parameter
 * @note Uses bashSpaces() to handle special characters in challenge
 * @see hmac_md5()
 * @see base64Encode()
 *
 * @since 1.0.0
 * @version 1.0.0
 */
    bool cramMD5Response(const QByteArray challenge);

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
     * @brief Replaces spaces with special character (0x01)
     *
     * @param str Input string with spaces
     * @return String with spaces replaced by 0x01
     *
     * @see unbashSpaces()
     *
     * @since 1.0.0
     * @version 1.0.0
     */
    QByteArray bashSpaces(const QByteArray &str);

    /**
     * @brief Converts special character (0x01) back to spaces
     *
     * Bareos uses 0x01 to encode spaces in certain fields. This function
     * reverses the encoding performed by bashSpaces().
     *
     * @param str Input string with 0x01 characters
     * @return String with spaces restored
     *
     * @see bashSpaces()
     *
     * @since 1.0.0
     * @version 1.0.0
     */
    QByteArray unbashSpaces(const QByteArray &str);

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

    /**
     * @brief Computes HMAC-MD5 digest of given data using the specified key.
     *
     * This implementation uses OpenSSL's `HMAC()` function with `EVP_md5()`,
     * ensuring byte-compatibility with the traditional MD5_CTX approach.
     *
     * The output is exactly 16 bytes (128 bits), suitable for Bareos protocol HMAC.
     *
     * @param text The input data to hash.
     * @param key The secret key used for HMAC.
     * @return QByteArray The resulting 16-byte HMAC-MD5 digest.
     *
     * @since 1.0.0
     * @version 1.0.0
     */
    const QByteArray hmac_md5(const QByteArray &challenge, const QByteArray &key_str);

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
    const QByteArray base64Encode(const QByteArray &data, bool isCompatible = false);

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
     * QByteArray hmac= base64Decode(hmac);
     * @endcode
     *
     * @since 1.0.0
     */
    const QByteArray base64Decode(const QByteArray &data, bool isCompatible = false);

    /**
     * @brief Verifies the Director's HMAC response during mutual authentication
     *
     * Decodes the Director's base64-encoded HMAC and compares it against the
     * expected HMAC calculated from the client challenge and shared password.
     * This is the counterpart to cramMD5Response() for mutual authentication.
     *
     * @param response Raw response from Director containing the base64-encoded HMAC
     * @return true if Director's HMAC is valid, false otherwise
     *
     * @note Requires m_clientChallenge to be set by cramMD5Challenge()
     * @see cramMD5Response()
     * @see cramMD5Challenge()
     *
     * @since 1.0.0
     * @version 1.0.0
     */
    bool verifyDirectorResponse(const QByteArray &response);

    /**
     * @brief Checks if m_readBuffer contains a complete Bareos message
     *
     * Bareos messages have a 4-byte big-endian length header followed by
     * the payload. This method verifies both header and payload are complete.
     *
     * @return true if a complete message is available in m_readBuffer
     *
     * @since 1.0.0
     * @version 1.0.0
     */
    bool hasCompleteMessage();

    /**
     * @brief Extracts one complete message from m_readBuffer
     *
     * Reads the 4-byte length header, extracts the corresponding payload,
     * and removes both from m_readBuffer. This allows processing multiple
     * messages that arrive in a single read operation.
     *
     * @return The extracted message payload (without length header), or empty if incomplete
     *
     * @note m_readBuffer is modified - processed data is removed
     * @see hasCompleteMessage()
     *
     * @since 1.0.0
     * @version 1.0.0
     */
    QByteArray extractMessage();

private slots:
    /**
     * @brief Handles authentication timeout
     *
     * @since 1.0.0
     */
    void onAuthTimeout();

    /**
     * @brief Handles all readings from director
     *
     * @since 1.0.0
     */
    void onReadyRead();

    /**
     * @brief Handles all readings from director
     *
     * @since 1.0.0
     */
    void onBytesWritten(qint64 bytes);

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
     * @brief Handles SSL errors during certificate-based TLS handshake
     *
     * @param errors List of SSL errors
     *
     * @since 1.0.0
     */
    void onSslErrorsCertificate(const QList<QSslError> &errors);

    /**
     * @brief Handles PSK authentication request
     *
     * @param authenticator PSK authenticator to fill
     *
     * @since 1.0.0
     */
    void onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator);

    /**
     * @brief Disconnects all signal connections to the socket
     *
     * Called after successful authentication to return socket control
     * to the Director class. Disconnects readyRead, bytesWritten,
     * encrypted, preSharedKeyAuthenticationRequired, and sslErrors signals.
     *
     * @note Must be called before Director connects its own signal handlers
     *
     * @since 1.0.0
     * @version 1.0.0
     */
    void disconnectSignals();
};

#endif // BAREOSAUTH_H
