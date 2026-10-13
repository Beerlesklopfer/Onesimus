#ifndef BGRPCTRANSPORT_H
#define BGRPCTRANSPORT_H

#include <QObject>
#include <QString>
#include <QQueue>
#include <QPointer>
#include <QSslCertificate>
#include <memory>

class QSslSocket;
class QGrpcBidiStream;

namespace bareos::director::Director {
class Client;
}

/**
 * @file bgrpctransport.h
 * @brief gRPC transport to the Onesimus Director (alternative to BNET)
 *
 * Only the Onesimus Director (onesimus-dird) speaks gRPC; C++ Bareos
 * Directors keep using BNET. The transport lives in the director worker
 * thread, next to the BNET socket of BareosDirector.
 *
 * Connection sequence:
 * @code
 *   pre-flight TLS handshake (QSslSocket) -> SHA-256 fingerprint
 *        │   mismatch / not yet pinned -> certificateNotTrusted()
 *        ▼
 *   QGrpcHttp2Channel (https://host:grpcPort)
 *        ▼
 *   GetChallenge(console) -> nonce
 *        ▼
 *   Login(console, nonce, HMAC-SHA256(md5hex(password), nonce)) -> token
 *        ▼
 *   loggedIn()  +  InteractiveSession (push events)
 * @endcode
 *
 * All later RPCs carry the token in the "x-onesimus-session" header.
 * Commands are executed one at a time (FIFO), so responses keep the order
 * BareosDirector's command queue relies on.
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */
class BGrpcTransport : public QObject
{
    Q_OBJECT

public:
    explicit BGrpcTransport(QObject *parent = nullptr);
    ~BGrpcTransport() override;

    /**
     * @brief Open the connection and log in
     * @param host Director host
     * @param port gRPC port
     * @param consoleName Console name ("*UserAgent*" or empty = Director password)
     * @param password Cleartext, "[md5]<hex>" or 32-char MD5 hex hash
     * @param pinnedFingerprint Trusted SHA-256 fingerprint (hex); empty = not yet trusted
     */
    void open(const QString &host, int port, const QString &consoleName,
              const QString &password, const QString &pinnedFingerprint);

    /// Close the connection (cancels pending calls and the push stream)
    void close();

    /// Queue a command line ("list jobs", ".jobs", ...) for execution
    void execute(const QString &commandLine);

    bool isLoggedIn() const { return !m_sessionToken.isEmpty(); }

    /// SHA-256 fingerprint (lowercase hex) of a certificate's DER encoding
    static QString certificateFingerprint(const QSslCertificate &cert);

    /// Login proof: hex HMAC-SHA256(key = md5 hex of password, nonce)
    static QString loginProof(const QString &password, const QString &nonce);

signals:
    /// Server certificate is not (or no longer) the pinned one; connection aborted
    void certificateNotTrusted(const QString &fingerprint, const QString &pinnedFingerprint);

    /// Login succeeded
    void loggedIn(const QString &directorName, const QString &version);

    /// Connection or login failed (connection is closed)
    void failed(const QString &reason);

    /// Result of execute(); text is the Director's JSON output
    void commandFinished(const QString &commandLine, bool success,
                         const QString &text, const QString &error);

    /// Push events from the InteractiveSession stream
    void jobStatusEvent(int jobId, const QString &jobName, const QString &status);
    void eventMessage(const QString &message);

    /// Connection lost after a successful login
    void connectionLost(const QString &reason);

private:
    void preflightDone();
    void startLogin();
    void sendLogin(const QString &nonce);
    void openPushStream();
    void sendNext();
    void fail(const QString &reason);

    QString m_host;
    int m_port = 0;
    QString m_consoleName;
    QString m_password;
    QString m_pinnedFingerprint;

    QPointer<QSslSocket> m_preflight;
    std::unique_ptr<bareos::director::Director::Client> m_client;
    std::unique_ptr<QGrpcBidiStream> m_pushStream;

    QString m_sessionToken;
    QQueue<QString> m_pending;
    bool m_inFlight = false;
    quint64 m_generation = 0;   ///< Invalidates replies of a previous connection
};

#endif // BGRPCTRANSPORT_H
