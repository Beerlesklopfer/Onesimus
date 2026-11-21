#ifndef BGRPCTRANSPORT_H
#define BGRPCTRANSPORT_H

#include <QObject>
#include <QString>
#include <QQueue>
#include <QHash>
#include <QPointer>
#include <QSslCertificate>
#include <QHostAddress>
#include <chrono>
#include <memory>

#include "director/bconfigtypes.h"

class QGrpcBidiStream;
class QGrpcCallReply;
class QGrpcStatus;
class BGrpcAddressProbe;

namespace BJobHistoryProto {
struct Request;
}

namespace onesimus::director::Director {
class Client;
}
namespace onesimus::director::api::DirectorApi {
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
 *   resolve host, TLS probe per address, IPv4 first (BGrpcAddressProbe)
 *        │   first address that completes the handshake -> SHA-256 fingerprint
 *        │   mismatch / not yet pinned -> certificateNotTrusted()
 *        ▼
 *   QGrpcHttp2Channel (https://<that address>:grpcPort)
 *        ▼
 *   GetChallenge(console) -> nonce
 *        ▼
 *   Login(console, nonce, HMAC-SHA256(md5hex(password), nonce)) -> token
 *        ▼
 *   loggedIn()  +  InteractiveSession (push events)
 * @endcode
 *
 * All later RPCs carry the token in the "x-onesimus-session" header, also
 * those of the service DirectorApi (proto/director_api.proto), which runs on
 * the same channel. Right after the login GetConfigCapabilities is asked
 * once (configCapabilities()); configCall() carries the configuration CRUD
 * methods. odir checks the session token of every DirectorApi call the same
 * way as for Director (grpc/auth.rs, AuthState::identity()).
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

    /**
     * @brief Runs a configuration request (DirectorApi CRUD)
     *
     * Answered by configReply() with the same id, also when not logged in
     * (Status::NotAvailable) or on a gRPC error (Status::RpcError with the
     * code). Independent of the command queue.
     */
    void configCall(quint64 id, const BConfig::Request &request);

    /// SHA-256 fingerprint (lowercase hex) of a certificate's DER encoding
    static QString certificateFingerprint(const QSslCertificate &cert);

    /// "CODE: reason" of a failed call (BGrpcConnect::failureText())
    static QString statusText(const QGrpcStatus &status, std::chrono::seconds deadline);

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
    /// @param timestamp When the Director posted it (unix time); @param schedTime the job's
    /// scheduled start (unix time, 0: not known; odir since proto 3302131)
    void jobStatusEvent(int jobId, const QString &jobName, const QString &status,
                        qint64 timestamp, qint64 schedTime);
    void eventMessage(const QString &message);
    /// Job message, parsed from a ConsoleTextEvent "JobId=<id> <name>: <text>"
    /// (also emitted as eventMessage). Push is lossy: no time, no message type.
    void jobMessage(quint64 jobId, const QString &jobName, const QString &text);

    /// Reply to configCall()
    void configReply(quint64 id, const BConfig::Reply &reply);

    /// What GetConfigCapabilities reported after the login
    void configCapabilities(const BConfig::Capabilities &capabilities);

    /// Connection lost after a successful login
    void connectionLost(const QString &reason);

private:
    void preflightDone(const QHostAddress &address, const QSslCertificate &cert);
    void startLogin();
    void sendLogin(const QString &nonce);
    void openPushStream();
    void sendNext();
    void fail(const QString &reason);
    void fetchConfigCapabilities();
    /// The job history by the typed ListJobs (see BJobHistoryProto)
    void sendTypedHistory(const QString &commandLine, const BJobHistoryProto::Request &history,
                          quint64 gen);
    void finishConfigCall(quint64 id, QGrpcCallReply *reply, BConfig::Request::Kind kind);

    QString m_host;
    int m_port = 0;
    QString m_consoleName;
    QString m_password;
    QString m_pinnedFingerprint;

    QPointer<BGrpcAddressProbe> m_probe;
    std::unique_ptr<onesimus::director::Director::Client> m_client;
    std::unique_ptr<onesimus::director::api::DirectorApi::Client> m_api;
    std::unique_ptr<QGrpcBidiStream> m_pushStream;

    QString m_sessionToken;
    QQueue<QString> m_pending;
    bool m_inFlight = false;
    bool m_typedHistory = true; ///< false: this Director's ListJobs lacks `reverse` or is unknown
    bool m_untypedOnce = false; ///< The next line goes by the command line (typed call failed)
    quint64 m_generation = 0;   ///< Invalidates replies of a previous connection
    QHash<quint64, QString> m_jobNames;  ///< Job names announced by JobStatusEvent (exact message parsing)
};

#endif // BGRPCTRANSPORT_H
