/**
 * @file baculaauth.h
 * @brief Qt-based Bacula Authentication Implementation
 *
 * This file provides a Qt-based implementation of the Bacula authentication
 * protocol, including CRAM-MD5 challenge-response authentication and TLS/PSK
 * negotiation.
 *
 * @author Original Bacula code by Kern Sibbald
 * @author Qt port and modifications by [Joerg Bernau <Joerg@bernau.family>]
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
 * - 1.0.0 (2024-xx-xx): Initial Qt port
 * - 1.1.0 (2025-01-xx): Added PSK-TLS support
 * - 1.2.0 (2025-01-21): Fixed CRAM-MD5 Base64 encoding, improved error handling
 *
 * @see https://www.bacula.org/
 * @see https://gitlab.bacula.org/bacula-community-edition/bacula-community
 */

#ifndef BACULAAUTH_H
#define BACULAAUTH_H

#include <QObject>
#include <QSslSocket>
#include <QByteArray>
#include <QString>
#include <QTimer>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QDateTime>
#include <QRandomGenerator>

/**
 * @defgroup BaculaAuthVersion Version Information
 * @{
 */
#define BACULAAUTH_VERSION_MAJOR 1  ///< Major version number
#define BACULAAUTH_VERSION_MINOR 2  ///< Minor version number
#define BACULAAUTH_VERSION_PATCH 0  ///< Patch version number

/** @brief Full version string */
#define BACULAAUTH_VERSION_STRING "1.2.0"

/** @brief Version as integer for comparisons (1.2.0 = 10200) */
#define BACULAAUTH_VERSION ((BACULAAUTH_VERSION_MAJOR * 10000) + \
                            (BACULAAUTH_VERSION_MINOR * 100) + \
                            BACULAAUTH_VERSION_PATCH)
/** @} */

/**
 * @defgroup BaculaProtocolConstants Bacula Protocol Constants
 * @{
 */

/**
 * @brief User Agent Version for plugin authentication
 * @details This version number is sent in the Hello message to identify
 *          the client's protocol capabilities.
 */
#define UA_VERSION 200  // UA_VERSION_PLUGINAUTH

/**
 * @brief Authentication timeout in milliseconds
 * @details Maximum time to wait for authentication to complete (180 seconds)
 */
#define AUTH_TIMEOUT 180000

/** @} */

/**
 * @defgroup TLSConstants TLS/PSK Constants
 * @brief Constants for TLS negotiation
 * @{
 */

/**
 * @brief TLS not available/disabled
 */
#define BNET_TLS_NONE     0

/**
 * @brief TLS available but not required
 */
#define BNET_TLS_OK       1

/**
 * @brief TLS is required for connection
 */
#define BNET_TLS_REQUIRED 2

/** @} */

/**
 * @defgroup AuthInteractive Interactive Authentication Protocol
 * @brief Constants for interactive authentication (BPAM)
 * @{
 */
#define UA_AUTH_INTERACTIVE           "auth interactive"  ///< Interactive auth marker
#define UA_AUTH_INTERACTIVE_PLAIN     'P'  ///< Plain text input
#define UA_AUTH_INTERACTIVE_HIDDEN    'H'  ///< Hidden (password) input
#define UA_AUTH_INTERACTIVE_MESSAGE   'M'  ///< Message to display
#define UA_AUTH_INTERACTIVE_FINISH    'F'  ///< Authentication finished
#define UA_AUTH_INTERACTIVE_RESPONSE  'R'  ///< Response from user
/** @} */

/**
 * @enum TLSRequirementResult
 * @brief Results of TLS requirement negotiation
 */
enum TLSRequirementResult {
    TLS_REQ_OK,         ///< TLS requirements are compatible
    TLS_REQ_ERR_LOCAL,  ///< Local TLS requirements not met by remote
    TLS_REQ_ERR_REMOTE  ///< Remote TLS requirements not met by local
};

/**
 * @brief Converts binary data to Bacula-compatible Base64
 *
 * Bacula uses a standard Base64 encoding but without padding characters.
 * This is used for CRAM-MD5 authentication responses.
 *
 * @param data Binary data to encode
 * @return QString Base64-encoded string without padding
 *
 * @note Bacula's Base64 removes the '=' padding characters
 *
 * @par Example:
 * @code
 * QByteArray hmac = calculateHMAC(...);
 * QString encoded = baculaBase64Encode(hmac);
 * // encoded will be like "NA31sq0e02zQukZvhKKaAQ" (no padding)
 * @endcode
 *
 * @since 1.0.0
 */
static QString baculaBase64Encode(const QByteArray &data)
{
    QString base64 = QString::fromLatin1(data.toBase64());
    base64.remove('=');  // Remove padding
    return base64;
}

/**
 * @class BaculaAuth
 * @brief Handles authentication with Bacula Director
 *
 * This class implements the Bacula authentication protocol for connecting
 * a console client to a Bacula Director. It supports:
 *
 * - CRAM-MD5 challenge-response authentication
 * - TLS (Transport Layer Security) encryption
 * - PSK (Pre-Shared Key) TLS authentication
 * - Automatic protocol version negotiation
 *
 * @par Usage Example:
 * @code
 * QSslSocket *socket = new QSslSocket(this);
 * socket->connectToHost("director.example.com", 9101);
 *
 * BaculaAuth *auth = new BaculaAuth(socket, this);
 * connect(auth, &BaculaAuth::authenticationSucceeded, this, &MyClass::onAuthOk);
 * connect(auth, &BaculaAuth::authenticationFailed, this, &MyClass::onAuthFailed);
 *
 * auth->authenticateDirector("Director-dir", "Console", "password", false, false);
 * @endcode
 *
 * @par Authentication Flow:
 * 1. Client sends Hello message with version and TLS capabilities
 * 2. Director responds with CRAM-MD5 challenge
 * 3. Client computes HMAC-MD5 and sends Base64-encoded response
 * 4. Client sends its own CRAM-MD5 challenge to Director
 * 5. Director responds with its HMAC-MD5 response
 * 6. If both verify, authentication succeeds
 *
 * @warning The socket must be connected before calling authenticateDirector()
 *
 * @see https://www.bacula.org/15.0.x-manuals/en/main/Bacula_TLS_Communications_E.html
 *
 * @version 1.2.0
 * @since 1.0.0
 */
class BaculaAuth : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a BaculaAuth object
     *
     * @param socket Pointer to a connected QSslSocket
     * @param parent Optional parent QObject
     *
     * @pre socket must not be nullptr
     * @pre socket should be in connected state
     *
     * @since 1.0.0
     */
    explicit BaculaAuth(QSslSocket *socket, QObject *parent = nullptr);

    /**
     * @brief Destructor
     *
     * Stops any running authentication timeout timer.
     *
     * @since 1.0.0
     */
    virtual ~BaculaAuth();

    /**
     * @brief Authenticates with the Bacula Director
     *
     * This is the main entry point for authentication. It performs the complete
     * authentication handshake including TLS negotiation if enabled.
     *
     * @param directorName Name of the Director (must match Director's config)
     * @param consoleName Name of the Console resource (must match Director's config)
     * @param password Password for authentication
     * @param tlsEnable Enable TLS encryption
     * @param tlsRequire Require TLS encryption
     * @param tlsPSKEnable Enable PSK-TLS authentication (default: false)
     *
     * @return true if authentication succeeded, false otherwise
     *
     * @retval true Authentication successful
     * @retval false Authentication failed (check getErrorMessage() for details)
     *
     * @note This method blocks until authentication completes or times out
     * @note On success, emits authenticationSucceeded() signal
     * @note On failure, emits authenticationFailed() signal
     *
     * @par Error Handling:
     * If authentication fails, call getErrorMessage() to get detailed
     * information about the failure reason.
     *
     * @see getErrorMessage()
     * @see authenticationSucceeded()
     * @see authenticationFailed()
     *
     * @since 1.0.0
     * @version 1.2.0 - Added improved error handling
     */
    bool authenticateDirector(const QString &directorName,
                              const QString &consoleName,
                              const QString &password,
                              bool tlsEnable,
                              bool tlsRequire,
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
     * @since 1.0.0
     */
    int getDirectorVersion() const { return m_directorVersion; }

    /**
     * @brief Gets the real Director name extracted from the CRAM-MD5 challenge
     *
     * The Director sends its actual name in the CRAM-MD5 challenge string
     * (e.g., `<random.timestamp@PDC-dir>`). This name may differ from
     * the name stored in the connection profile.
     *
     * @return QString Real Director name, empty if not yet authenticated
     *
     * @since 2.9
     */
    QString remoteDirectorName() const { return m_remoteDirectorName; }

    /**
     * @brief Gets the TLS cipher list used for the connection
     * @return QString Colon-separated cipher list
     * @since 2.9
     */
    QString tlsCipherList() const { return m_tlsCipherList; }

    /**
     * @brief Gets the library version string
     *
     * @return QString Version string (e.g., "1.2.0")
     *
     * @since 1.2.0
     */
    static QString version() { return BACULAAUTH_VERSION_STRING; }

    /**
     * @brief Gets the library version as integer
     *
     * @return int Version number for comparisons (e.g., 10200 for 1.2.0)
     *
     * @since 1.2.0
     */
    static int versionNumber() { return BACULAAUTH_VERSION; }

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

private:
    /// @name Member Variables
    /// @{
    QSslSocket *m_socket;        ///< SSL socket for communication
    QTimer *m_authTimer;         ///< Authentication timeout timer
    QString m_errorMessage;      ///< Last error message
    int m_directorVersion;       ///< Director protocol version

    int m_tlsLocalNeed;          ///< Local TLS requirement level
    int m_pskLocalNeed;          ///< Local PSK requirement level
    int m_tlsRemoteNeed;         ///< Remote TLS requirement level
    int m_pskRemoteNeed;         ///< Remote PSK requirement level

    bool m_tlsStarted;           ///< TLS encryption active flag
    bool m_checkEarlyTLS;        ///< Early TLS check flag
    bool m_authSuccess;          ///< Authentication success flag
    QString m_password;          ///< Stored password for PSK
    QByteArray m_readBuffer;     ///< Buffer for incoming data
    QString m_remoteDirectorName; ///< Real Director name from CRAM-MD5 challenge
    QString m_tlsCipherList;      ///< Colon-separated PSK cipher list from auth
    /// @}

    /// @name TLS/PSK Helper Methods
    /// @{

    /**
     * @brief Calculates the combined TLS/PSK need value
     * @since 1.0.0
     */
    int calculateTLSPSKNeed(bool tlsEnable, bool tlsRequire, bool pskEnable);

    /**
     * @brief Decodes the remote TLS/PSK need value
     * @since 1.0.0
     */
    void decodeRemoteTLSPSKNeed(int remoteNeed);

    /**
     * @brief Tests if TLS requirements can be met
     * @since 1.0.0
     */
    TLSRequirementResult testTLSRequirement();
    /// @}

    /// @name Protocol Methods
    /// @{

    /**
     * @brief Creates a Bacula protocol packet
     * @since 1.0.0
     */
    QByteArray createBaculaPacket(const QString &message);

    /**
     * @brief Sends a message using Bacula protocol
     * @since 1.0.0
     */
    bool sendBaculaMessage(const QString &message);

    /**
     * @brief Reads a message using Bacula protocol
     * @since 1.0.0
     */
    bool readBaculaMessage(QString &message, int timeoutMs = 5000);

    /**
     * @brief Waits for data to be available
     * @since 1.0.0
     */
    bool waitForResponse(int timeoutMs = 180000);

    /**
     * @brief Reads a single line from the socket
     * @since 1.0.0
     */
    bool readLine(QString &line, int timeoutMs = 5000);

    /**
     * @brief Handles early TLS negotiation
     * @since 1.1.0
     */
    bool clientEarlyTLS();

    /**
     * @brief Handles TLS setup after negotiation
     * @since 1.1.0
     */
    bool handleTLS();

    /**
     * @brief Performs CRAM-MD5 authentication sequence
     * @since 1.0.0
     */
    bool clientCramMD5Authenticate(const QString &password);
    /// @}

    /// @name CRAM-MD5 Implementation
    /// @{

    /**
     * @brief Responds to Director's CRAM-MD5 challenge
     * @since 1.0.0
     * @version 1.2.0 - Fixed Base64 encoding, added TLS need storage
     */
    bool cramMD5Respond(const QString &password);

    /**
     * @brief Challenges the Director with CRAM-MD5
     * @since 1.0.0
     * @version 1.2.0 - Added error detection, improved diagnostics
     */
    bool cramMD5Challenge(const QString &password);

    /**
     * @brief Computes HMAC-MD5 hash
     * @since 1.0.0
     */
    QByteArray hmacMD5(const QByteArray &key, const QByteArray &data);
    /// @}

    /// @name Utility Methods
    /// @{

    /**
     * @brief Converts spaces to Bacula escape sequence
     * @since 1.0.0
     */
    QString bashSpaces(const QString &str);

    /**
     * @brief Starts the authentication timeout timer
     * @since 1.0.0
     */
    void startAuthTimeout();

    /**
     * @brief Stops the authentication timeout timer
     * @since 1.0.0
     */
    void stopAuthTimeout();

    /**
     * @brief Starts TLS or PSK-TLS encryption
     * @since 1.1.0
     */
    bool startEncryption(bool usePSK);
    /// @}

private slots:
    /**
     * @brief Handles authentication timeout
     * @since 1.0.0
     */
    void onAuthTimeout();
};

#endif // BACULAAUTH_H
