/**
 * @file bgrpctransport.cpp
 * @brief gRPC transport to the Onesimus Director
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "director/bgrpctransport.h"
#include "blogging.h"

#include "director_service.qpb.h"
#include "director_service_client.grpc.qpb.h"

#include <QGrpcHttp2Channel>
#include <QGrpcChannelOptions>
#include <QGrpcCallOptions>
#include <QGrpcCallReply>
#include <QGrpcStatus>
#include <QGrpcBidiStream>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <QRegularExpression>
#include <QTimer>
#include <QUrl>

#include <chrono>

using namespace bareos::director;
using namespace std::chrono_literals;

namespace {

constexpr auto PreflightTimeout = 10s;
constexpr auto LoginTimeout = 15s;
constexpr auto CommandTimeout = 5min;  // restore/estimate can take a while

const QByteArray SessionHeader = QByteArrayLiteral("x-onesimus-session");

} // namespace

// ============================================================================
// Constructor / Destructor
// ============================================================================

BGrpcTransport::BGrpcTransport(QObject *parent)
    : QObject(parent)
{
}

BGrpcTransport::~BGrpcTransport()
{
    close();
}

// ============================================================================
// Helpers
// ============================================================================

QString BGrpcTransport::certificateFingerprint(const QSslCertificate &cert)
{
    return QString::fromLatin1(
        QCryptographicHash::hash(cert.toDer(), QCryptographicHash::Sha256).toHex());
}

QString BGrpcTransport::loginProof(const QString &password, const QString &nonce)
{
    // Same key derivation as TLS-PSK / BareosAuth: MD5 hex of the password
    QString key = password;
    if (key.startsWith("[md5]", Qt::CaseInsensitive)) {
        key = key.mid(5);
    }
    static const QRegularExpression hexHash("^[0-9a-fA-F]{32}$");
    if (hexHash.match(key).hasMatch()) {
        key = key.toLower();
    } else {
        key = QString::fromLatin1(
            QCryptographicHash::hash(key.toLatin1(), QCryptographicHash::Md5).toHex());
    }

    return QString::fromLatin1(
        QMessageAuthenticationCode::hash(nonce.toLatin1(), key.toLatin1(),
                                         QCryptographicHash::Sha256).toHex());
}

// ============================================================================
// Connection
// ============================================================================

void BGrpcTransport::open(const QString &host, int port, const QString &consoleName,
                          const QString &password, const QString &pinnedFingerprint)
{
    close();

    m_host = host;
    m_port = port;
    m_consoleName = consoleName;
    m_password = password;
    m_pinnedFingerprint = pinnedFingerprint.trimmed().toLower().remove(':');

    BLOG_DEBUG() << "gRPC: pre-flight TLS handshake to" << host << ":" << port;

    // Pre-flight handshake: QGrpcHttp2Channel has no hook to inspect the peer
    // certificate, so we fetch it here and compare it against the pinned one.
    const quint64 gen = m_generation;
    m_preflight = new QSslSocket(this);
    QSslConfiguration ssl = m_preflight->sslConfiguration();
    ssl.setPeerVerifyMode(QSslSocket::VerifyNone);  // self-signed; trust is the pin
    m_preflight->setSslConfiguration(ssl);

    connect(m_preflight, &QSslSocket::encrypted, this, &BGrpcTransport::preflightDone);
    connect(m_preflight, &QSslSocket::errorOccurred, this, [this, gen](QAbstractSocket::SocketError) {
        if (gen != m_generation || !m_preflight) {
            return;
        }
        fail(tr("Cannot reach gRPC endpoint %1:%2: %3")
                 .arg(m_host).arg(m_port).arg(m_preflight->errorString()));
    });
    QTimer::singleShot(PreflightTimeout, this, [this, gen]() {
        if (gen == m_generation && m_preflight) {
            fail(tr("Timeout connecting to gRPC endpoint %1:%2").arg(m_host).arg(m_port));
        }
    });

    m_preflight->connectToHostEncrypted(host, quint16(port));
}

void BGrpcTransport::preflightDone()
{
    const QSslCertificate cert = m_preflight->peerCertificate();
    m_preflight->abort();
    m_preflight->deleteLater();
    m_preflight = nullptr;

    const QString fingerprint = certificateFingerprint(cert);
    BLOG_DEBUG() << "gRPC: server certificate SHA-256" << fingerprint;

    if (cert.isNull() || fingerprint != m_pinnedFingerprint) {
        BLOG_WARNING() << "gRPC: certificate not trusted (pinned:"
                       << (m_pinnedFingerprint.isEmpty() ? QStringLiteral("none") : m_pinnedFingerprint)
                       << ")";
        const QString pinned = m_pinnedFingerprint;
        close();
        emit certificateNotTrusted(fingerprint, pinned);
        return;
    }

    // Certificate is the pinned one. The channel itself cannot verify a
    // self-signed certificate against a host name, so verification is off and
    // the pre-flight pin is the trust anchor.
    QSslConfiguration ssl = QSslConfiguration::defaultConfiguration();
    ssl.setPeerVerifyMode(QSslSocket::VerifyNone);
    ssl.setAllowedNextProtocols({QByteArrayLiteral("h2")});

    QGrpcChannelOptions options;
    options.setSslConfiguration(ssl);

    QUrl url;
    url.setScheme("https");
    url.setHost(m_host);
    url.setPort(m_port);

    m_client = std::make_unique<Director::Client>();
    m_client->attachChannel(std::make_shared<QGrpcHttp2Channel>(url, options));

    startLogin();
}

void BGrpcTransport::startLogin()
{
    ChallengeRequest request;
    request.setConsoleName(m_consoleName);

    QGrpcCallOptions options;
    options.setDeadlineTimeout(LoginTimeout);

    const quint64 gen = m_generation;
    QGrpcCallReply *reply = m_client->GetChallenge(request, options).release();
    connect(reply, &QGrpcCallReply::finished, this, [this, reply, gen](const QGrpcStatus &status) {
        reply->deleteLater();
        if (gen != m_generation) {
            return;
        }
        if (!status.isOk()) {
            fail(tr("gRPC login failed: %1").arg(status.message()));
            return;
        }
        const auto challenge = reply->read<ChallengeResponse>();
        if (!challenge || challenge->nonce().isEmpty()) {
            fail(tr("gRPC login failed: no challenge received"));
            return;
        }
        sendLogin(challenge->nonce());
    });
}

void BGrpcTransport::sendLogin(const QString &nonce)
{
    LoginRequest request;
    request.setConsoleName(m_consoleName);
    request.setNonce(nonce);
    request.setProof(loginProof(m_password, nonce));

    QGrpcCallOptions options;
    options.setDeadlineTimeout(LoginTimeout);

    const quint64 gen = m_generation;
    QGrpcCallReply *reply = m_client->Login(request, options).release();
    connect(reply, &QGrpcCallReply::finished, this, [this, reply, gen](const QGrpcStatus &status) {
        reply->deleteLater();
        if (gen != m_generation) {
            return;
        }
        if (!status.isOk()) {
            fail(tr("gRPC login failed: %1").arg(status.message()));
            return;
        }
        const auto response = reply->read<LoginResponse>();
        if (!response || !response->success() || response->sessionToken().isEmpty()) {
            fail(tr("Authentication failed: %1")
                     .arg(response ? response->errorMessage() : tr("invalid response")));
            return;
        }

        m_sessionToken = response->sessionToken();
        // The password is not needed any more
        m_password.clear();

        BLOG_DEBUG() << "gRPC: logged in to" << response->directorName()
                     << "version" << response->version();

        openPushStream();
        emit loggedIn(response->directorName(), response->version());
        sendNext();
    });
}

void BGrpcTransport::openPushStream()
{
    SessionMessage hello;
    hello.setType(SessionMessage::Type::TYPE_UNSPECIFIED);

    QGrpcCallOptions options;
    options.setMetadata(QHash<QByteArray, QByteArray>{{SessionHeader, m_sessionToken.toLatin1()}});

    const quint64 gen = m_generation;
    m_pushStream = m_client->InteractiveSession(hello, options);

    connect(m_pushStream.get(), &QGrpcBidiStream::messageReceived, this, [this, gen]() {
        if (gen != m_generation || !m_pushStream) {
            return;
        }
        const auto msg = m_pushStream->read<SessionMessage>();
        if (!msg || !msg->hasPushEvent()) {
            return;
        }
        const PushEvent &ev = msg->pushEvent();
        if (ev.hasJobStatus()) {
            const JobStatusEvent &js = ev.jobStatus();
            emit jobStatusEvent(int(js.jobId()), js.jobName(), js.status());
        } else if (ev.hasConsoleText()) {
            emit eventMessage(ev.consoleText().text());
        } else if (ev.hasFdConnected()) {
            emit eventMessage(tr("Client %1 connected").arg(ev.fdConnected().clientName()));
        } else if (ev.hasFdDisconnected()) {
            emit eventMessage(tr("Client %1 disconnected").arg(ev.fdDisconnected().clientName()));
        } else if (ev.hasFdStatus()) {
            const FdStatusEvent &fd = ev.fdStatus();
            emit eventMessage(fd.alive() ? tr("Client %1 is alive").arg(fd.clientName())
                                         : tr("Client %1 not reachable").arg(fd.clientName()));
        } else if (ev.hasSdConnected()) {
            emit eventMessage(tr("Storage %1 connected (job %2)")
                                  .arg(ev.sdConnected().storageName()).arg(ev.sdConnected().jobId()));
        } else if (ev.hasSdDisconnected()) {
            emit eventMessage(tr("Storage %1 disconnected (job %2)")
                                  .arg(ev.sdDisconnected().storageName()).arg(ev.sdDisconnected().jobId()));
        } else if (ev.hasSdStatus()) {
            const SdStatusEvent &sd = ev.sdStatus();
            emit eventMessage(sd.alive() ? tr("Storage %1 is alive").arg(sd.storageName())
                                         : tr("Storage %1 not reachable").arg(sd.storageName()));
        }
    });

    connect(m_pushStream.get(), &QGrpcBidiStream::finished, this, [this, gen](const QGrpcStatus &status) {
        if (gen != m_generation) {
            return;
        }
        BLOG_DEBUG() << "gRPC: push stream finished:" << status.message();
        if (!status.isOk() && status.code() != QtGrpc::StatusCode::Cancelled) {
            const QString reason = status.message();
            close();
            emit connectionLost(reason);
        }
    });
}

void BGrpcTransport::close()
{
    // Everything still in flight belongs to the old connection
    ++m_generation;

    if (m_preflight) {
        m_preflight->abort();
        m_preflight->deleteLater();
        m_preflight = nullptr;
    }
    // close() may run inside a reply/stream callback of this channel:
    // destroy the stream and client deferred, not synchronously
    if (m_pushStream) {
        m_pushStream->cancel();
        m_pushStream.release()->deleteLater();
    }
    if (m_client) {
        m_client.release()->deleteLater();
    }
    m_sessionToken.clear();
    m_pending.clear();
    m_inFlight = false;
}

void BGrpcTransport::fail(const QString &reason)
{
    BLOG_ERROR() << "gRPC:" << reason;
    close();
    emit failed(reason);
}

// ============================================================================
// Commands
// ============================================================================

void BGrpcTransport::execute(const QString &commandLine)
{
    m_pending.enqueue(commandLine);
    sendNext();
}

void BGrpcTransport::sendNext()
{
    if (m_inFlight || !isLoggedIn() || m_pending.isEmpty()) {
        return;
    }
    m_inFlight = true;
    const QString commandLine = m_pending.dequeue();
    const quint64 gen = m_generation;

    // The output format is requested per call (response_format), so ".api"
    // has nothing to switch on the server. Answer it locally - in queue order.
    if (commandLine.trimmed().startsWith(QLatin1String(".api"))) {
        QTimer::singleShot(0, this, [this, gen, commandLine]() {
            if (gen != m_generation) {
                return;
            }
            m_inFlight = false;
            emit commandFinished(commandLine, true,
                                 QStringLiteral(R"({"jsonrpc":"2.0","id":null,"result":{"api":2}})"),
                                 QString());
            sendNext();
        });
        return;
    }

    CommandRequest request;
    request.setCommandName(commandLine);
    request.setResponseFormat(ResponseFormatGadget::ResponseFormat::RESPONSE_FORMAT_JSON);

    QGrpcCallOptions options;
    options.setDeadlineTimeout(CommandTimeout);
    options.setMetadata(QHash<QByteArray, QByteArray>{{SessionHeader, m_sessionToken.toLatin1()}});

    QGrpcCallReply *reply = m_client->ExecuteCommand(request, options).release();
    connect(reply, &QGrpcCallReply::finished, this,
            [this, reply, gen, commandLine](const QGrpcStatus &status) {
        reply->deleteLater();
        if (gen != m_generation) {
            return;
        }
        m_inFlight = false;

        if (status.isOk()) {
            const auto response = reply->read<CommandResponse>();
            if (response) {
                emit commandFinished(commandLine, response->success(),
                                     response->textOutput(), response->errorMessage());
            } else {
                emit commandFinished(commandLine, false, QString(), tr("Invalid response"));
            }
        } else {
            emit commandFinished(commandLine, false, QString(), status.message());
            if (status.code() == QtGrpc::StatusCode::Unavailable
                || status.code() == QtGrpc::StatusCode::Unauthenticated) {
                const QString reason = status.message();
                close();
                emit connectionLost(reason);
                return;
            }
        }
        sendNext();
    });
}
