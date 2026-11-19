/**
 * @file bgrpctransport.cpp
 * @brief gRPC transport to the Onesimus Director
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "director/bgrpctransport.h"
#include "director/bgrpcconnect.h"
#include "director/bjobpushmessage.h"
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

using namespace onesimus::director;
using namespace std::chrono_literals;

namespace {

constexpr auto LoginTimeout = 15s;
constexpr auto CommandTimeout = 5min;  // restore/estimate can take a while

// The gRPC package of the generated Director client (proto/director_service.proto)
const QString ServicePackage = QStringLiteral("onesimus.director");

const QByteArray SessionHeader = QByteArrayLiteral("x-onesimus-session");
const int MaxKnownJobNames = 512;      // job names remembered for parsing job messages

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

QString BGrpcTransport::statusText(const QGrpcStatus &status, std::chrono::seconds deadline)
{
    return BGrpcConnect::failureText(int(status.code()), status.message(), int(deadline.count()));
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
    // The probe tries every address of the host name (IPv4 first) and the
    // channel then connects to the address that answered: a name with an
    // IPv6 address that is not reachable must not hang the login.
    const quint64 gen = m_generation;
    m_probe = new BGrpcAddressProbe(this);
    connect(m_probe, &BGrpcAddressProbe::succeeded, this,
            [this, gen](const QHostAddress &address, const QSslCertificate &cert) {
        if (gen == m_generation && m_probe) {
            preflightDone(address, cert);
        }
    });
    connect(m_probe, &BGrpcAddressProbe::failed, this, [this, gen](const QString &reason) {
        if (gen == m_generation && m_probe) {
            fail(reason);
        }
    });
    m_probe->start(host, quint16(port));
}

void BGrpcTransport::preflightDone(const QHostAddress &address, const QSslCertificate &cert)
{
    if (m_probe) {
        m_probe->deleteLater();
        m_probe = nullptr;
    }

    const QString fingerprint = certificateFingerprint(cert);
    BLOG_DEBUG() << "gRPC: server certificate SHA-256" << fingerprint << "at" << address.toString();

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

    m_client = std::make_unique<Director::Client>();
    m_client->attachChannel(std::make_shared<QGrpcHttp2Channel>(BGrpcConnect::channelUrl(m_host, m_port, address), options));

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
            fail(tr("gRPC login failed: %1")
                     .arg(BGrpcConnect::loginFailureText(int(status.code()), status.message(),
                                                         int(std::chrono::seconds(LoginTimeout).count()),
                                                         ServicePackage)));
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
            fail(tr("gRPC login failed: %1")
                     .arg(BGrpcConnect::loginFailureText(int(status.code()), status.message(),
                                                         int(std::chrono::seconds(LoginTimeout).count()),
                                                         ServicePackage)));
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
            if (m_jobNames.size() >= MaxKnownJobNames) {
                m_jobNames.clear();
            }
            m_jobNames.insert(js.jobId(), js.jobName());
            emit jobStatusEvent(int(js.jobId()), js.jobName(), js.status());
        } else if (ev.hasConsoleText()) {
            const QString text = ev.consoleText().text();
            // Job messages arrive as console text "JobId=<id> <name>: <text>"
            BJobPushMessage job = BJobPushMessage::parse(text);
            if (job.valid) {
                const QString knownName = m_jobNames.value(job.jobId);
                if (!knownName.isEmpty()) {
                    job = BJobPushMessage::parse(text, knownName);
                }
                emit jobMessage(job.jobId, job.jobName, job.text);
            }
            emit eventMessage(text);
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
            const QString reason = statusText(status, std::chrono::seconds(0));
            close();
            emit connectionLost(reason);
        }
    });
}

void BGrpcTransport::close()
{
    // Everything still in flight belongs to the old connection
    ++m_generation;
    m_jobNames.clear();

    if (m_probe) {
        m_probe->abort();
        m_probe->deleteLater();
        m_probe = nullptr;
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
            const QString reason = statusText(status, CommandTimeout);
            emit commandFinished(commandLine, false, QString(), reason);
            if (status.code() == QtGrpc::StatusCode::Unavailable
                || status.code() == QtGrpc::StatusCode::Unauthenticated) {
                close();
                emit connectionLost(reason);
                return;
            }
        }
        sendNext();
    });
}
