/**
 * @file bareosauth.cpp
 * @brief Implementation of Qt-based Bareos Authentication
 *
 * @see bareosauth.h for class documentation
 */

#include "director/bareosauth.h"
#include "blogging.h"

#include <QtEndian>
#include <QDataStream>
#include <QSslConfiguration>
#include <QSslCipher>
#include <QSslKey>
#include <QFile>
#include <QMessageAuthenticationCode>

// ============================================================================
// Constructor / Destructor
// ============================================================================

BareosAuth::BareosAuth(QSslSocket *socket, QObject *parent)
    : QObject(parent), m_socket(socket), m_authTimer(nullptr), m_directorVersion(0), m_tlsLocalNeed(BAREOS_TLS_NONE), m_tlsRemoteNeed(BAREOS_TLS_NONE), m_tlsPSKEnable(true) // Default for Bareos 18.2+
      ,
    m_tlsStarted(false), m_authSuccess(false), m_authState(BAuthState::AUTH_IDLE)
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
        QString passwordToUse = password;

        // Check for [md5] prefix (Bareos config file format)
        // Format: [md5]01234abcdef... where the hash follows the prefix
        if (passwordToUse.startsWith("[md5]", Qt::CaseInsensitive)) {
            passwordToUse = passwordToUse.mid(5);  // Remove "[md5]" prefix
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "Detected [md5] prefix, stripped to:" << passwordToUse;
#endif
        }

        // Check if password is already a 32-char hex MD5 hash
        // (e.g., from BConnectionProfile::getPasswordHashHex())
        bool isValidHexHash = (passwordToUse.length() == 32);
        if (isValidHexHash) {
            for (const QChar &c : passwordToUse) {
                if (!c.isDigit() && (c.toLower() < 'a' || c.toLower() > 'f')) {
                    isValidHexHash = false;
                    break;
                }
            }
        }

        if (isValidHexHash) {
            // Password is already MD5 hash in hex format - convert to binary
            m_password = QByteArray::fromHex(passwordToUse.toLatin1());
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "Password is already MD5 hash (hex), using directly";
#endif
        } else {
            // Password is cleartext - hash it with MD5
            m_password = QCryptographicHash::hash(passwordToUse.toLatin1(), QCryptographicHash::Md5);
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "Password is cleartext, hashing with MD5";
#endif
        }
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

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "========================================";
    AUTH_DEBUG << "BAREOS AUTHENTICATION START";
    AUTH_DEBUG << "========================================";
    AUTH_DEBUG << "  Director           : " << directorName;
    AUTH_DEBUG << "  Console            : " << consoleName;
    AUTH_DEBUG << "  TLS Enable         : " << tlsEnable;
    AUTH_DEBUG << "  TLS Require        : " << tlsRequire;
    AUTH_DEBUG << "  TLS Require        : " << tlsVerifyPeer;
    AUTH_DEBUG << "  TLS-PSK Enable     : " << tlsPSKEnable;
    AUTH_DEBUG << "  BareosAuth Version : " << version();
#endif

    // Calculate TLS needs
    m_tlsLocalNeed = calculateTLSNeed(tlsEnable, tlsRequire);
#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  Local TLS need:" << m_tlsLocalNeed;
#endif

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
    emit statusMessage("Starting Bareos authentication...");

    m_authState = BAuthState::AUTH_IDLE;

    // PSK Auth (Bareos 18.2+)
    if (tlsPSKEnable && !password.isEmpty())
    {
        emit statusMessage("Setting up TLS-PSK...");

        // Prepare Hello for TLS-PSK mode (Bareos 18.2+)
        // Format: "Hello <consoleName> calling version <version>\n"
        // See: bareos/core/src/lib/bsock.cc line 346
        m_writeBuffer = QByteArray("Hello ");
        m_writeBuffer.append(bashSpaces(m_consoleName.toLatin1()));
        m_writeBuffer.append(" calling version ");
        m_writeBuffer.append(BAREOS_VERSION_STR);
        m_writeBuffer.append('\n');

        if (!setupPSKTLS())
        {
            // If PSK setup fails, authentication will fail in the slot
            return false;
        }

        // ✅ Hello wird in onEncrypted() nach TLS-Handshake gesendet
        // m_writeBuffer wurde bereits vorbereitet (Zeile 102-106)
        emit statusMessage("Starting TLS-PSK handshake...");

        return true;
    }
    // Certificate-based TLS (Bareos <= 18.2.1 oder explizit konfiguriert)
    else if (tlsEnable && !password.isEmpty())
    {
        emit statusMessage("Setting up certificate-based TLS...");

        // Prepare Hello for Certificate TLS mode (Bareos 18.2+)
        // Format: "Hello <consoleName> calling version <version>\n"
        m_writeBuffer = QByteArray("Hello ");
        m_writeBuffer.append(bashSpaces(m_consoleName.toLatin1()));
        m_writeBuffer.append(" calling version ");
        m_writeBuffer.append(BAREOS_VERSION_STR);
        m_writeBuffer.append('\n');

        if (!setupCertificateTLS())
        {
            return false;
        }

        // ✅ Hello wird in onEncrypted() nach TLS-Handshake gesendet
        emit statusMessage("Starting TLS handshake...");

        return true;
    }
    // Legacy mode: Kein TLS
    else if (!password.isEmpty())
    {
        // Legacy mode: Hello + CRAM-MD5 (no TLS)
        emit statusMessage("Starting legacy authentication (no TLS)...");

        // Prepare Hello for Legacy mode (pre-18.2 format)
        // Format: "Hello <consoleName> calling\n"
        // See: bareos/python-bareos/bareos/bsock/protocolmessages.py line 50
        m_writeBuffer = QByteArray("Hello ");
        m_writeBuffer.append(bashSpaces(m_consoleName.toLatin1()));
        m_writeBuffer.append(" calling\n");

#ifdef IS_DEVELOPER
        AUTH_DEBUG << "Legacy Hello message:" << m_writeBuffer;
#endif
        emit statusMessage(QString("Sending Hello (legacy): %1").arg(QString::fromLatin1(m_writeBuffer)));

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

        // BLOG_DEBUG() << "<<< Director banner:" << response;

        // // Parse version from banner
        // if (!parseDirectorVersion(response))
        // {
        //     AUTH_WARNING << "Could not parse Director version from response";
        // }

        // // Success!
        // stopAuthTimeout();
        // m_authSuccess = true;

        // BLOG_DEBUG() << "\n\n";
        // BLOG_DEBUG() << "BAREOS AUTHENTICATION SUCCESSFUL";
        // BLOG_DEBUG() << "  Director Version:" << m_directorVersionString;
        // BLOG_DEBUG() << "========================================";

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
// TLS Setup Methods
// ============================================================================

bool BareosAuth::setupCertificateTLS()
{
#ifdef IS_DEVELOPER
    AUTH_DEBUG << "Setting up certificate-based TLS...";
#endif

    // Connect encrypted signal for continuing after TLS
    m_encryptedConn = QObject::connect(m_socket, &QSslSocket::encrypted,
            this, &BareosAuth::onEncrypted);

    // Connect SSL error handler
    m_sslErrorsConn = QObject::connect(m_socket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
            this, &BareosAuth::onSslErrorsCertificate);

    // Connect socket error handler for TLS handshake failures
    m_socketErrorConn = QObject::connect(m_socket, &QSslSocket::errorOccurred,
            this, &BareosAuth::onSocketError);

    // Configure SSL for certificate-based authentication
    QSslConfiguration sslConfig = m_socket->sslConfiguration();

    // Set protocol to TLS 1.2 or later
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);

    // ✅ Peer verification basierend auf Konfiguration
    if (m_tlsVerifyPeer)
    {
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Peer verification: ENABLED (Certificate mode)";
#endif
    }
    else
    {
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Peer verification: DISABLED (Certificate mode without verification)";
#endif
    }

    // Load CA certificate
    if (!m_tlsCaFile.isEmpty()) {
        QList<QSslCertificate> caCerts = QSslCertificate::fromPath(m_tlsCaFile);
        if (caCerts.isEmpty()) {
            AUTH_WARNING << "No CA certificates found in:" << m_tlsCaFile;
        } else {
            sslConfig.setCaCertificates(caCerts);
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "  Loaded CA certificate:" << m_tlsCaFile;
#endif
        }
    }

    // Load client certificate
    if (!m_tlsCertFile.isEmpty()) {
        QList<QSslCertificate> localCerts = QSslCertificate::fromPath(m_tlsCertFile);
        if (localCerts.isEmpty()) {
            AUTH_WARNING << "No client certificate found in:" << m_tlsCertFile;
        } else {
            sslConfig.setLocalCertificateChain(localCerts);
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "  Loaded client certificate:" << m_tlsCertFile;
#endif
        }
    }

    // Load private key
    if (!m_tlsKeyFile.isEmpty()) {
        QFile keyFile(m_tlsKeyFile);
        if (keyFile.open(QIODevice::ReadOnly)) {
            // Try RSA first, then EC
            QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);
            if (key.isNull()) {
                keyFile.seek(0);
                key = QSslKey(&keyFile, QSsl::Ec, QSsl::Pem, QSsl::PrivateKey);
            }
            keyFile.close();

            if (key.isNull()) {
                AUTH_WARNING << "Failed to load private key from:" << m_tlsKeyFile;
            } else {
                sslConfig.setPrivateKey(key);
#ifdef IS_DEVELOPER
                AUTH_DEBUG << "  Loaded private key:" << m_tlsKeyFile;
#endif
            }
        } else {
            AUTH_WARNING << "Failed to open key file:" << m_tlsKeyFile;
        }
    }

    m_socket->setSslConfiguration(sslConfig);

    // Start TLS handshake
#ifdef IS_DEVELOPER
    AUTH_DEBUG << "Starting TLS handshake (certificate mode)...";
#endif
    m_socket->startClientEncryption();

    return true;
}

bool BareosAuth::setupPSKTLS()
{
#ifdef IS_DEVELOPER
    AUTH_DEBUG << "Setting up TLS-PSK...";
#endif

    // Connect PSK signal
    m_pskConn = QObject::connect(m_socket, &QSslSocket::preSharedKeyAuthenticationRequired,
            this, &BareosAuth::onPreSharedKeyAuthenticationRequired);

    // Connect encrypted signal for continuing after TLS
    m_encryptedConn = QObject::connect(m_socket, &QSslSocket::encrypted,
            this, &BareosAuth::onEncrypted);

    // Connect SSL error handler
    m_sslErrorsConn = QObject::connect(m_socket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
            this, &BareosAuth::onSslErrors);

    // Connect socket error handler for TLS handshake failures
    m_socketErrorConn = QObject::connect(m_socket, &QSslSocket::errorOccurred,
            this, &BareosAuth::onSocketError);

    // Configure SSL for PSK
    QSslConfiguration sslConfig = m_socket->sslConfiguration();

    // Force TLSv1.2 to ensure preSharedKeyAuthenticationRequired signal works
    // Qt's PSK callback may not work reliably with TLSv1.3
    sslConfig.setProtocol(QSsl::TlsV1_2);

    // ✅ RICHTIG: Für PSK keine Peer-Verifikation, für Zertifikate je nach Konfiguration
    if (m_tlsPSKEnable)
    {
        // PSK braucht keine Zertifikat-Verifikation
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Peer verification: DISABLED (PSK mode)";
#endif
    }
    else if (m_tlsVerifyPeer)
    {
        // Zertifikat-basierte TLS mit Verifikation
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Peer verification: ENABLED (Certificate mode)";
#endif
    }
    else
    {
        // Zertifikat-basierte TLS ohne Verifikation
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Peer verification: DISABLED (Certificate mode without verification)";
#endif
    }

    // Force TLSv1.2 for PSK (Qt PSK callback doesn't work with TLSv1.3)
    sslConfig.setProtocol(QSsl::TlsV1_2);

    // Set explicit PSK ciphers to ensure PSK callback is triggered
    // Only plain PSK-* ciphers work with Bareos pure PSK mode
    // (RSA-PSK, DHE-PSK, ECDHE-PSK require additional key exchange)
    QList<QSslCipher> pskCiphers;
    QList<QSslCipher> availableCiphers = QSslConfiguration::supportedCiphers();

    // Check if we have a configured cipher list from the profile
    if (!m_configuredCipherList.isEmpty())
    {
        AUTH_DEBUG << "Using configured cipher list:" << m_configuredCipherList;
        QStringList configuredNames = m_configuredCipherList.split(':', Qt::SkipEmptyParts);

        for (const QString &cipherName : configuredNames)
        {
            QString trimmedName = cipherName.trimmed();
            for (const QSslCipher &cipher : availableCiphers)
            {
                if (cipher.name() == trimmedName)
                {
                    pskCiphers.append(cipher);
#ifdef IS_DEVELOPER
                    AUTH_DEBUG << "  Using configured cipher:" << trimmedName;
#endif
                    break;
                }
            }
        }

        if (pskCiphers.isEmpty())
        {
            AUTH_WARNING << "WARNING: None of the configured ciphers are available!";
            AUTH_WARNING << "Configured:" << m_configuredCipherList;
            AUTH_WARNING << "Falling back to auto-detection...";
            // Clear to trigger auto-detection below
        }
        else
        {
            AUTH_DEBUG << "Using" << pskCiphers.count() << "configured PSK ciphers";
            sslConfig.setCiphers(pskCiphers);
        }
    }

    // Auto-detect PSK ciphers if not configured or configured list was empty
    if (pskCiphers.isEmpty())
    {
        for (const QSslCipher &cipher : availableCiphers)
        {
            QString name = cipher.name();
            // Only plain PSK-* ciphers (not RSA-PSK, DHE-PSK, ECDHE-PSK)
            if (name.startsWith("PSK-") &&
                cipher.protocol() == QSsl::TlsV1_2)
            {
                pskCiphers.append(cipher);
#ifdef IS_DEVELOPER
                AUTH_DEBUG << "  Adding PSK cipher:" << name << "(" << cipher.protocolString() << ")";
#endif
            }
        }

        if (pskCiphers.isEmpty())
        {
            AUTH_WARNING << "========================================";
            AUTH_WARNING << "WARNING: No PSK ciphers found automatically!";
            AUTH_WARNING << "Trying to add common Bareos PSK ciphers manually...";
            AUTH_WARNING << "Available cipher count:" << availableCiphers.count();

            // Try to find common Bareos-compatible PSK cipher suites
            // Bareos typically uses: AES256-GCM-SHA384, AES128-GCM-SHA256
            QStringList preferredPskCipherNames = {
                "PSK-AES256-GCM-SHA384",
                "PSK-AES128-GCM-SHA256",
                "PSK-AES256-CBC-SHA384",
                "PSK-AES128-CBC-SHA256",
                "PSK-AES256-CBC-SHA",
                "PSK-AES128-CBC-SHA"
            };

            for (const QString &cipherName : preferredPskCipherNames)
            {
                for (const QSslCipher &cipher : availableCiphers)
                {
                    if (cipher.name() == cipherName)
                    {
                        pskCiphers.append(cipher);
                        AUTH_WARNING << "  Found:" << cipherName;
                        break;
                    }
                }
            }

            if (pskCiphers.isEmpty())
            {
                AUTH_WARNING << "FATAL: Still no PSK ciphers available!";
                AUTH_WARNING << "OpenSSL may not be compiled with PSK support";
                AUTH_WARNING << "========================================";
                // Don't set empty cipher list - let it fail with proper error
            }
            else
            {
                AUTH_WARNING << "Using" << pskCiphers.count() << "manually configured PSK ciphers";
                AUTH_WARNING << "========================================";
                sslConfig.setCiphers(pskCiphers);
            }
        }
        else
        {
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "Setting" << pskCiphers.count() << "PSK cipher suites";
#endif
            sslConfig.setCiphers(pskCiphers);
        }
    }

    // Store cipher list for config export
    QStringList cipherNames;
    for (const QSslCipher &c : pskCiphers)
        cipherNames.append(c.name());
    m_tlsCipherList = cipherNames.join(':');

    m_socket->setSslConfiguration(sslConfig);

    // Start TLS handshake
#ifdef IS_DEVELOPER
    AUTH_DEBUG << "Starting TLS-PSK handshake...";
#endif
    m_socket->startClientEncryption();

    return true;
}

void BareosAuth::onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator)
{
#ifdef IS_DEVELOPER
    AUTH_DEBUG << "========================================";
    AUTH_DEBUG << "PSK AUTHENTICATION REQUIRED";
    AUTH_DEBUG << "========================================";
#endif

    handlePskAuthenticator(authenticator);
}

void BareosAuth::handlePskAuthenticator(QSslPreSharedKeyAuthenticator *authenticator)
{
    // Bareos PSK identity format: R_CONSOLE<RS>ConsoleName
    // where <RS> is ASCII Record Separator (0x1e)
    // See: bareos/core/src/lib/tls_openssl_private.cc - psk_server_cb()
    QByteArray identity;
    identity.append(BAREOS_R_CONSOLE);
    identity.append('\x1e');  // ASCII Record Separator
    identity.append(m_consoleName.toLatin1());

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  Identity hint from server:" << authenticator->identityHint();
    AUTH_DEBUG << "  Setting identity:" << identity;
    AUTH_DEBUG << "  Identity (hex):" << identity.toHex();
    AUTH_DEBUG << "  PSK key (hex):" << m_password.toHex();
#endif

    // Bareos expects the PSK key as hex representation of MD5(password)
    // Not the raw MD5 bytes, but the 32-character hex string
    QByteArray pskKey = m_password.toHex();

    authenticator->setIdentity(identity);
    authenticator->setPreSharedKey(pskKey);

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "PSK credentials set";
#endif
}

void BareosAuth::onEncrypted()
{
#ifdef IS_DEVELOPER
    AUTH_DEBUG << "========================================";
    AUTH_DEBUG << "TLS-PSK HANDSHAKE SUCCESSFUL";
    AUTH_DEBUG << "========================================";
    AUTH_DEBUG << "  Encrypted: true";
    AUTH_DEBUG << "  Protocol:" << m_socket->sessionProtocol();
    AUTH_DEBUG << "  Cipher:" << m_socket->sessionCipher().name();
    AUTH_DEBUG << "========================================";
#endif

    m_tlsStarted = true;

    // ✅ Nach PSK-TLS: Jetzt Hello senden und CRAM-MD5 starten
    emit statusMessage("TLS established, sending Hello packet...");

    // m_writeBuffer wurde bereits in authenticateDirector() vorbereitet
    // Format: "Hello <consoleName> calling <directorName> version <version>"

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "Sending Hello packet:" << m_writeBuffer;
#endif

    if (send() != BnetStatus::Ok)
    {
        m_errorMessage = "Failed to send Hello message after TLS handshake";
        emit authenticationFailed(m_errorMessage);
        return;
    }

    // ✅ State-Machine starten: Warten auf Director Challenge
    m_authState = BAuthState::WAIT_FOR_CHALLENGE;

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  ✓ Hello sent, waiting for Director challenge...";
    AUTH_DEBUG << "  State changed -> WAIT_FOR_CHALLENGE";
#endif

    emit statusMessage("Waiting for Director challenge...");
}

void BareosAuth::onSslErrors(const QList<QSslError> &errors)
{
    AUTH_WARNING << "SSL Errors during PSK handshake:";
    for (const QSslError &error : errors)
    {
        AUTH_WARNING << "  -" << error.errorString();
    }

    // For PSK, we expect some "errors" like no peer certificate
    // This is normal for PSK authentication
    if (m_tlsPSKEnable)
    {
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "Ignoring SSL errors for PSK mode (this is normal)";
#endif
        m_socket->ignoreSslErrors();
    }
}

void BareosAuth::onSslErrorsCertificate(const QList<QSslError> &errors)
{
    AUTH_WARNING << "SSL Errors during certificate-based TLS handshake:";
    for (const QSslError &error : errors)
    {
        AUTH_WARNING << "  -" << error.errorString();
    }

    // Bei Zertifikat-basierter TLS: Nur ignorieren wenn VerifyPeer deaktiviert
    if (!m_tlsVerifyPeer)
    {
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "Ignoring SSL errors (peer verification disabled)";
#endif
        m_socket->ignoreSslErrors();
        return;
    }

    // ✅ Bareos verwendet Daemon-Namen in Zertifikaten, nicht Hostnamen
    // Daher ignorieren wir HostNameMismatch, prüfen aber andere Fehler
    QList<QSslError> criticalErrors;
    QList<QSslError> ignorableErrors;

    for (const QSslError &error : errors)
    {
        if (error.error() == QSslError::HostNameMismatch)
        {
            // Hostname-Mismatch ist bei Bareos normal (Zertifikat hat Daemon-Namen)
            ignorableErrors.append(error);
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "  Ignoring HostNameMismatch (Bareos uses daemon names in certificates)";
#endif
        }
        else
        {
            // Andere Fehler sind kritisch
            criticalErrors.append(error);
        }
    }

    // Ignorierbare Fehler dem Socket mitteilen
    if (!ignorableErrors.isEmpty())
    {
        m_socket->ignoreSslErrors(ignorableErrors);
    }

    // Bei kritischen Fehlern: Authentifizierung abbrechen
    if (!criticalErrors.isEmpty())
    {
        m_errorMessage = QString("SSL/TLS Error: %1").arg(criticalErrors.first().errorString());
        emit authenticationFailed(m_errorMessage);
    }
}

bool BareosAuth::cramMD5Response(const QByteArray challenge)
{
#ifdef IS_DEVELOPER
    AUTH_DEBUG << "Processing Director challenge:" << challenge;
#endif

    QRegularExpression challengeRx(
        R"(auth\s+cram-md5(c?)\s+(<[^>]+>)\s+ssl=(\d+))",
        QRegularExpression::CaseInsensitiveOption);

    auto match = challengeRx.match(challenge);

    if (!match.hasMatch())
    {
        // ✅ Prüfe ob der Director eine Fehlermeldung gesendet hat
        QString response = QString::fromLatin1(challenge).trimmed();

        if (response.startsWith("1999") || response.contains("denied", Qt::CaseInsensitive) ||
            response.contains("failed", Qt::CaseInsensitive) || response.contains("rejected", Qt::CaseInsensitive))
        {
            // Director hat eine Fehlermeldung gesendet - zeige sie dem Benutzer
            m_errorMessage = tr("Director rejected connection: %1").arg(response);

            // Zusätzliche Hinweise basierend auf der Fehlermeldung
            if (response.contains("TLS", Qt::CaseInsensitive) ||
                response.contains("configuration mismatch", Qt::CaseInsensitive) ||
                response.contains("ssl", Qt::CaseInsensitive))
            {
                m_errorMessage += tr("\n\nHint: The Director requires TLS encryption. "
                                     "Please use PSK or Certificate authentication instead of Legacy mode.");
            }
        }
        else if (response.isEmpty())
        {
            m_errorMessage = tr("Director sent empty response.\n\n"
                                "This usually means the Director requires TLS encryption "
                                "but Legacy (unencrypted) mode was used.\n"
                                "Please try PSK or Certificate authentication.");
        }
        else
        {
            // Unbekanntes Format
            m_errorMessage = tr("Unexpected response from Director:\n%1").arg(response);
        }

        emit authenticationFailed(m_errorMessage);
        AUTH_CRITICAL << m_errorMessage;
        return false;
    }

    const QByteArray directorChallenge = match.captured(2).toLatin1();
    m_tlsRemoteNeed = match.captured(3).toInt();

    // ✅ Speichere für später
    m_isCompatible = !match.captured(1).isEmpty();
    m_directorChallenge = match.captured(2);

    // Extract real Director name from challenge: <random.timestamp@R_DIRECTOR::DirectorName>
    QString challengeContent = m_directorChallenge;
    int atIdx = challengeContent.indexOf('@');
    if (atIdx >= 0) {
        QString afterAt = challengeContent.mid(atIdx + 1);
        if (afterAt.endsWith('>'))
            afterAt.chop(1);
        // Bareos format: "R_DIRECTOR::name" — strip the prefix
        if (afterAt.startsWith("R_DIRECTOR::"))
            afterAt = afterAt.mid(12);
        m_remoteDirectorName = afterAt;
    }

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  Director challenge:" << directorChallenge;
    AUTH_DEBUG << "  Remote Director name:" << m_remoteDirectorName;
    AUTH_DEBUG << "  Compatible mode:" << m_isCompatible;
    AUTH_DEBUG << "  Remote TLS need:" << m_tlsRemoteNeed;
    AUTH_DEBUG << "  Password (hex):" << m_password.toHex();
#endif

    // Berechne HMAC
    const QByteArray hmac = hmac_md5(directorChallenge, m_password.toHex());

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  HMAC (raw hex):" << hmac.toHex();
#endif

    // ✅ Verwende den gleichen Modus wie der Director (cram-md5 vs cram-md5c)
    m_writeBuffer = base64Encode(hmac, m_isCompatible);

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  HMAC (base64):" << m_writeBuffer;
#endif

    // ✅ Sende NUR die HMAC-Response
    if (send() != BnetStatus::Ok)
    {
        m_errorMessage = "Failed to send HMAC response";
        emit authenticationFailed(m_errorMessage);
        AUTH_CRITICAL << m_errorMessage;
        return false;
    }

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  ✓ HMAC response sent";
#endif

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

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  Sending client challenge:" << m_clientChallenge;
#endif

    // Nachricht vorbereiten (mit Newline am Ende!)
    // ✅ Verwende den gleichen Modus wie der Director (cram-md5 vs cram-md5c)
    if (m_isCompatible) {
        m_writeBuffer = QByteArray("auth cram-md5c ");
    } else {
        m_writeBuffer = QByteArray("auth cram-md5 ");
    }
    m_writeBuffer.append(m_clientChallenge);
    m_writeBuffer.append(" ssl=");
    m_writeBuffer.append(QByteArray::number(m_tlsLocalNeed));
    m_writeBuffer.append('\n');

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  Full message:" << m_writeBuffer;
#endif

    if (send() != BnetStatus::Ok)
    {
        m_errorMessage = "Failed to send CRAM-MD5 challenge";
        return false;
    }

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  ✓ Client challenge sent";
    AUTH_DEBUG << "  Socket state:" << m_socket->state();
    AUTH_DEBUG << "  Socket encrypted:" << m_socket->isEncrypted();
    AUTH_DEBUG << "  Bytes to write:" << m_socket->bytesToWrite();
#endif

    emit statusMessage(QString("Client challenge sent. Socket: %1, BytesToWrite: %2")
                       .arg(m_socket->state())
                       .arg(m_socket->bytesToWrite()));

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
        AUTH_CRITICAL << "Cannot send - socket not connected";
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

    // Note: Don't use flush() here - it blocks the async state machine!
    // The data will be sent when the event loop processes the socket.

    if (len+4 != m_writeBuffer.size()) {
        AUTH_CRITICAL << "Failed to send complete command! Written:" << m_lastSentSize << "Expected:" << m_writeBuffer.size();
        return BnetStatus::Error;
    } else {
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "✓ Command sent successfully (" << m_lastSentSize << "bytes)";
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
    //     AUTH_WARNING << "Remote TLS level" << m_tlsRemoteNeed
    //                << "does not meet local requirement" << m_tlsLocalNeed;
    //     return BAREOS_TLS_REQ_ERR_LOCAL;
    // }

    // // Check if we can meet remote's requirements
    // if (m_tlsRemoteNeed > m_tlsLocalNeed &&
    //     m_tlsLocalNeed != BAREOS_TLS_OK &&
    //     m_tlsRemoteNeed != BAREOS_TLS_OK)
    // {
    //     AUTH_WARNING << "Local TLS level" << m_tlsLocalNeed
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
    AUTH_DEBUG << "Verifying Director's response...";
    AUTH_DEBUG << "  Raw response:" << response;
#endif

    // Response ist base64-encoded HMAC
    QByteArray trimmedResponse = response.trimmed();

    // Entferne Null-Terminator falls vorhanden
    if (trimmedResponse.endsWith('\0')) {
        trimmedResponse.chop(1);
    }

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  Trimmed response:" << trimmedResponse;
#endif

    // ✅ Berechne erwarteten HMAC basierend auf UNSERER Challenge
    QByteArray expectedHMAC = hmac_md5(m_clientChallenge, m_password.toHex());

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  Our challenge was:" << m_clientChallenge;
    AUTH_DEBUG << "  Expected HMAC (hex):" << expectedHMAC.toHex();
#endif

    // ✅ Bareos probiert BEIDE Base64-Varianten (siehe cram_md5.cc Zeile 131-142)
    // Erste Variante: mit m_isCompatible
    QByteArray expectedBase64 = base64Encode(expectedHMAC, m_isCompatible);

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  Expected (compatible=" << m_isCompatible << "):" << expectedBase64;
    AUTH_DEBUG << "  Received:" << trimmedResponse;
#endif

    if (trimmedResponse == expectedBase64) {
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  ✓ Director HMAC verified successfully (compatible mode)";
#endif
        return true;
    }

    // Zweite Variante: mit !m_isCompatible (Fallback wie Bareos)
    QByteArray expectedBase64Alt = base64Encode(expectedHMAC, !m_isCompatible);

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  Expected (compatible=" << !m_isCompatible << "):" << expectedBase64Alt;
#endif

    if (trimmedResponse == expectedBase64Alt) {
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  ✓ Director HMAC verified successfully (alternate mode)";
#endif
        return true;
    }

    AUTH_WARNING << "  ✗ Director HMAC verification failed";
    AUTH_WARNING << "    Received:" << trimmedResponse;
    AUTH_WARNING << "    Expected (compat):" << expectedBase64;
    AUTH_WARNING << "    Expected (non-compat):" << expectedBase64Alt;
    return false;
}

bool BareosAuth::hasCompleteMessage()
{
    if (m_readBuffer.size() < 4) {
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  hasCompleteMessage: buffer too small:" << m_readBuffer.size() << "bytes";
#endif
        return false;
    }

    qint32 msgLen = qFromBigEndian<qint32>(
        reinterpret_cast<const uchar*>(m_readBuffer.constData())
        );

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  hasCompleteMessage: msgLen=" << msgLen << ", buffer=" << m_readBuffer.size();
    AUTH_DEBUG << "  hasCompleteMessage: header bytes (hex):" << m_readBuffer.left(4).toHex();
#endif

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
    AUTH_DEBUG << "onReadyRead=>BAuthState:" << static_cast<int>(m_authState);
#endif

    QByteArray newData = m_socket->readAll();
    m_readBuffer.append(newData);

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  Received:" << newData.size() << "bytes";
    AUTH_DEBUG << "  Buffer total:" << m_readBuffer.size() << "bytes";
    AUTH_DEBUG << "  Data:" << QString(m_readBuffer).left(200);
#endif

    if (!hasCompleteMessage()) {
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Waiting for more data...";
#endif
        return;
    }

    QByteArray message = extractMessage();

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "  Complete message:" << message;
#endif

    switch (m_authState)
    {
    case BAuthState::WAIT_FOR_CHALLENGE:
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Processing Director challenge...";
#endif

        if (cramMD5Response(message)) {
            m_authState = BAuthState::COMPUTING_RESPONSE;
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "  State changed -> COMPUTING_RESPONSE";
#endif
        } else {
            m_authState = BAuthState::AUTH_FAILED;
            emit authenticationFailed(m_errorMessage);
        }
        break;

    case BAuthState::WAIT_FOR_OK_AUTH:
    {
        // Wir haben unsere HMAC-Response gesendet und warten auf "1000 OK auth"
        // (siehe Bareos cram_md5.cc Zeile 240: "1000 OK auth\n")
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Waiting for '1000 OK auth' from Director";
        AUTH_DEBUG << "  Received:" << message;
#endif

        QString msgStr = QString::fromLatin1(message).trimmed();

        if (msgStr.startsWith("1000 OK auth")) {
            // Director hat unsere HMAC-Response akzeptiert
            // Jetzt senden wir unsere eigene Challenge (bidirektionale Auth)
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "  ✓ Director accepted our HMAC";
            AUTH_DEBUG << "  Sending our challenge now (bidirectional auth)...";
#endif

            if (cramMD5Challenge()) {
                m_authState = BAuthState::WAIT_FOR_DIRECTOR_HMAC;
#ifdef IS_DEVELOPER
                AUTH_DEBUG << "  State changed -> WAIT_FOR_DIRECTOR_HMAC";
#endif
            } else {
                m_authState = BAuthState::AUTH_FAILED;
                emit authenticationFailed(m_errorMessage);
            }
        } else if (msgStr.contains("bareos-dir") || msgStr.contains("Version")) {
            // Director sendet direkt das finale Banner (unidirektionale Auth)
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "  ✓ Director sent final banner (unidirectional auth)";
#endif
            m_authSuccess = true;
            m_authState = BAuthState::AUTH_SUCCESS;
            stopAuthTimeout();
            disconnectSignals();
            emit statusMessage("Connected");
            emit authenticationSucceeded(msgStr);
        } else if (msgStr.startsWith("1999")) {
            // Authentifizierung fehlgeschlagen
            m_errorMessage = QString("Director rejected authentication: %1").arg(msgStr);
            m_authState = BAuthState::AUTH_FAILED;
            emit authenticationFailed(m_errorMessage);
        } else {
#ifdef IS_DEVELOPER
            AUTH_WARNING << "  Unexpected response in WAIT_FOR_OK_AUTH:" << message;
#endif
            // Versuche trotzdem weiterzumachen
            m_errorMessage = QString("Unexpected response: %1").arg(msgStr);
        }
        break;
    }

    case BAuthState::WAIT_FOR_DIRECTOR_HMAC:
    {
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Received response after sending our challenge";
        AUTH_DEBUG << "  Response:" << message;
#endif

        // Prüfe ob der Director "1000 OK" sendet (keine bidirektionale Auth)
        // oder einen HMAC (bidirektionale Auth)
        QString msgStr = QString::fromLatin1(message).trimmed();

        if (msgStr.startsWith("1000 OK")) {
            // Director macht KEINE bidirektionale Authentifizierung
            // Er hat unsere Challenge ignoriert und direkt "1000 OK" gesendet
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "  Director skipped bidirectional auth, sent OK directly";
#endif

            // Prüfe ob es das finale Banner ist
            if (msgStr.contains("bareos-dir") || msgStr.contains("Version")) {
                // Das ist bereits das finale Banner!
                m_authSuccess = true;
                m_authState = BAuthState::AUTH_SUCCESS;
                stopAuthTimeout();
                emit authenticationSucceeded(msgStr);
#ifdef IS_DEVELOPER
                AUTH_DEBUG << "  ✓ Authentication complete (unidirectional)";
#endif
            } else {
                // Es ist "1000 OK auth", warte auf das finale Banner
                m_authState = BAuthState::WAIT_FOR_FINAL_OK;
#ifdef IS_DEVELOPER
                AUTH_DEBUG << "  State changed -> WAIT_FOR_FINAL_OK";
#endif
            }
            break;
        }

        // Director sendet HMAC - bidirektionale Authentifizierung
        // Verifiziere Director's HMAC
        if (!verifyDirectorResponse(message)) {
            AUTH_WARNING << "  Director HMAC verification failed";
            // Bei Fehlern trotzdem fortfahren (einige Directors haben Bugs)
            // Die Verbindung wird später scheitern wenn wirklich falsch
        } else {
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "  ✓ Director HMAC verified successfully";
#endif
        }

        // Sende "1000 OK auth" an Director
        m_writeBuffer = QByteArray("1000 OK auth\n");

        if (send() != BnetStatus::Ok) {
            m_errorMessage = "Failed to send '1000 OK auth'";
            m_authState = BAuthState::AUTH_FAILED;
            emit authenticationFailed(m_errorMessage);
            return;
        }

#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  ✓ Sent '1000 OK auth' to Director";
#endif

        // Warte auf Director's finale "1000 OK: bareos-dir Version: ..."
        m_authState = BAuthState::WAIT_FOR_FINAL_OK;
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  State changed -> WAIT_FOR_FINAL_OK";
#endif
        break;
    }

    case BAuthState::WAIT_FOR_FINAL_OK:
    {
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Processing final Director banner...";
#endif
            message = unbashSpaces(message);

        // ✅ Jetzt erwarten wir "1000 OK: bareos-dir Version: ..."
        if (message.contains("1000 OK:")) {
#ifdef IS_DEVELOPER
            AUTH_DEBUG << "  ✓ Received final OK from Director!";
#endif

            parseDirectorVersion(message);

            m_authState = BAuthState::AUTH_SUCCESS;
            m_authSuccess = true;
            stopAuthTimeout();

            disconnectSignals();
            emit statusMessage("Connected");
            emit authenticationSucceeded(m_directorVersionString);
        } else {
#ifdef IS_DEVELOPER
            AUTH_WARNING << "  Unexpected final response:" << message;
#endif
            // m_errorMessage = QString("Expected '1000 OK: bareos-dir Version...', got: %1").arg(QString(message));
            // m_authState = BAuthState::AUTH_FAILED;
            // emit authenticationFailed(m_errorMessage);
        }
        break;
    }
    case BAuthState::AUTH_SUCCESS:
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Already authenticated, ignoring data";
#endif
        break;

    case BAuthState::AUTH_FAILED:
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Auth already failed, ignoring data";
#endif
        break;

    default:
#ifdef IS_DEVELOPER
        AUTH_WARNING << "  Unexpected state:" << static_cast<int>(m_authState);
#endif
        break;
    }
}

void BareosAuth::onBytesWritten(qint64 bytesWritten)
{

#ifdef IS_DEVELOPER
    AUTH_DEBUG << "onBytesWritten=>BAuthState:" << static_cast<int>(m_authState);
    AUTH_DEBUG << "  Bytes written:" << bytesWritten;
#endif

    // ✅ Vergleiche mit gespeicherter Größe
    if (bytesWritten != m_lastSentSize) {
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  Partial write:" << bytesWritten << "/" << m_lastSentSize;
#endif
        return;
    }

    switch (m_authState)
    {
    case BAuthState::AUTH_IDLE:
        // Hello wurde komplett gesendet
        m_authState = BAuthState::WAIT_FOR_CHALLENGE;
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  ✓ Hello sent completely";
        AUTH_DEBUG << "  State changed -> WAIT_FOR_CHALLENGE";
#endif
        break;

    case BAuthState::WAIT_FOR_CHALLENGE:
        // Das sollte nicht passieren - Hello wurde schon gesendet
#ifdef IS_DEVELOPER
        AUTH_WARNING << "  WARNING: Unexpected write in WAIT_FOR_CHALLENGE state";
#endif
        break;

    case BAuthState::COMPUTING_RESPONSE:
        // HMAC-Response wurde gesendet, jetzt auf "1000 OK auth" vom Director warten
        // (siehe Bareos cram_md5.cc Zeile 233-240: bs_->recv() nach send())
        m_authState = BAuthState::WAIT_FOR_OK_AUTH;
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  ✓ HMAC response sent, waiting for '1000 OK auth' from Director...";
        AUTH_DEBUG << "  State changed -> WAIT_FOR_OK_AUTH";
#endif
        break;

    case BAuthState::WAIT_FOR_DIRECTOR_HMAC:
        // Challenge wurde gesendet, warte auf Director's Response
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  ✓ Client challenge sent, waiting for Director response...";
        AUTH_DEBUG << "    Socket state:" << m_socket->state();
        AUTH_DEBUG << "    Bytes available:" << m_socket->bytesAvailable();
        AUTH_DEBUG << "    ReadBuffer size:" << m_readBuffer.size();
#endif
        break;

    case BAuthState::WAIT_FOR_FINAL_OK:
        // "1000 OK auth" wurde gesendet, warte auf finale Director-Nachricht
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  ✓ OK response sent, waiting for Director's final message...";
#endif
        break;

    default:
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "  No action for state:" << static_cast<int>(m_authState);
#endif
        break;
    }

    m_writeBuffer.clear();
}

void BareosAuth::onAuthTimeout()
{
    AUTH_CRITICAL << "========================================";
    AUTH_CRITICAL << "AUTHENTICATION TIMEOUT";
    AUTH_CRITICAL << "========================================";

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
        AUTH_DEBUG << "Parsed Director version:" << m_directorVersionString
                 << "(" << m_directorVersion << ")";
#endif
        return true;
    }

    // Wenn keine Version gefunden, prüfe auf einfaches "1000 OK"
    if (normalized.contains("1000 OK", Qt::CaseInsensitive))
    {
#ifdef IS_DEVELOPER
        AUTH_DEBUG << "Got OK response but could not parse version";
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
        disconnect(m_socketErrorConn);
}

void BareosAuth::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)

    QString errorString = m_socket->errorString();
    AUTH_CRITICAL << "Socket error during TLS handshake:" << errorString;

    // Check for common PSK-related errors
    if (errorString.contains("bad record mac", Qt::CaseInsensitive) ||
        errorString.contains("decrypt_error", Qt::CaseInsensitive) ||
        errorString.contains("illegal_parameter", Qt::CaseInsensitive))
    {
        m_errorMessage = tr("TLS-PSK authentication failed: %1\n\n"
                            "This usually means:\n"
                            "• Wrong password\n"
                            "• Wrong console name\n"
                            "• Console not configured on Director\n"
                            "• TLS-PSK not enabled on Director").arg(errorString);
    }
    else if (errorString.contains("handshake_failure", Qt::CaseInsensitive))
    {
        m_errorMessage = tr("TLS handshake failed: %1\n\n"
                            "Possible causes:\n"
                            "• No compatible cipher suites\n"
                            "• TLS version mismatch\n"
                            "• Director doesn't support PSK").arg(errorString);
    }
    else
    {
        m_errorMessage = tr("Socket error: %1").arg(errorString);
    }

    m_authState = BAuthState::AUTH_FAILED;
    stopAuthTimeout();
    emit authenticationFailed(m_errorMessage);
}
