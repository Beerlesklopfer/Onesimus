// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bgrpcconnect.cpp
 * @brief Reaching the gRPC port of the Onesimus Director: addresses, probe,
 *        readable failures
 */

#include "director/bgrpcconnect.h"

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QLocalServer>
#include <QLocalSocket>
#include <QRandomGenerator>
#include <QHostInfo>
#include <QSslConfiguration>
#include <QSslSocket>
#include <QTimer>

#include <memory>

namespace BGrpcConnect {

namespace {

QString tr(const char *text)
{
    return QCoreApplication::translate("BGrpcConnect", text);
}

} // namespace

QList<QHostAddress> orderAddresses(const QList<QHostAddress> &addresses, Order order)
{
    QList<QHostAddress> unique;
    for (const QHostAddress &address : addresses) {
        if (!address.isNull() && !unique.contains(address))
            unique.append(address);
    }
    if (order == Order::ResolverOrder)
        return unique;

    QList<QHostAddress> ordered;
    for (const QHostAddress &address : unique) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
            ordered.append(address);
    }
    for (const QHostAddress &address : unique) {
        if (address.protocol() != QAbstractSocket::IPv4Protocol)
            ordered.append(address);
    }
    return ordered;
}

QString statusCodeName(int code)
{
    switch (code) {
    case 0: return QStringLiteral("OK");
    case 1: return QStringLiteral("CANCELLED");
    case 2: return QStringLiteral("UNKNOWN");
    case 3: return QStringLiteral("INVALID_ARGUMENT");
    case 4: return QStringLiteral("DEADLINE_EXCEEDED");
    case 5: return QStringLiteral("NOT_FOUND");
    case 6: return QStringLiteral("ALREADY_EXISTS");
    case 7: return QStringLiteral("PERMISSION_DENIED");
    case 8: return QStringLiteral("RESOURCE_EXHAUSTED");
    case 9: return QStringLiteral("FAILED_PRECONDITION");
    case 10: return QStringLiteral("ABORTED");
    case 11: return QStringLiteral("OUT_OF_RANGE");
    case 12: return QStringLiteral("UNIMPLEMENTED");
    case 13: return QStringLiteral("INTERNAL");
    case 14: return QStringLiteral("UNAVAILABLE");
    case 15: return QStringLiteral("DATA_LOSS");
    case 16: return QStringLiteral("UNAUTHENTICATED");
    default: break;
    }
    return QStringLiteral("STATUS_%1").arg(code);
}

QString failureText(int code, const QString &message, int deadlineSeconds)
{
    // grpc-message travels percent-encoded ("invalid%20session%20token");
    // QtGrpc hands it over as it came
    QString reason = message.contains(QLatin1Char('%'))
        ? QUrl::fromPercentEncoding(message.toUtf8()).trimmed()
        : message.trimmed();
    if (code == 16 && !reason.isEmpty()) {
        // The Director forgets a session after 8 hours without a request and
        // on restart; the GUI only learns that with the next call
        reason = tr("%1 - the Director no longer knows this session (it ends after "
                    "8 hours without a request or when the Director restarts). "
                    "Connect again.").arg(reason);
    }
    if (reason.isEmpty()) {
        switch (code) {
        case 1:
            reason = tr("the call was cancelled");
            break;
        case 4:
            reason = deadlineSeconds > 0
                ? tr("no connection or no reply within %1 s").arg(deadlineSeconds)
                : tr("no connection or no reply in time");
            break;
        case 7:
            reason = tr("this console is not allowed to do that");
            break;
        case 12:
            reason = tr("the Director does not offer this method (an older version?)");
            break;
        case 14:
            reason = tr("the gRPC service of the Director cannot be reached");
            break;
        case 16:
            reason = tr("not logged in, or the session has expired");
            break;
        case 2:
        case 13:
            reason = tr("the Director reported an error without a message");
            break;
        default:
            reason = tr("no reason given");
            break;
        }
    }
    return QStringLiteral("%1: %2").arg(statusCodeName(code), reason);
}

QString loginFailureText(int code, const QString &message, int deadlineSeconds,
                         const QString &servicePackage)
{
    if (code == 12) {
        QString text = QStringLiteral("%1: ").arg(statusCodeName(code))
            + tr("The Director does not offer the expected gRPC service (protocol version "
                 "mismatch: this build speaks %1; the Director may run an older or newer "
                 "build). Use BNET or update the Director or this application.")
                  .arg(servicePackage);
        if (!message.trimmed().isEmpty())
            text += QStringLiteral(" (%1)").arg(message.trimmed());
        return text;
    }
    return failureText(code, message, deadlineSeconds);
}

QUrl channelUrl(const QString &host, int port, const QHostAddress &address)
{
    QUrl url;
    url.setScheme(QStringLiteral("https"));
    if (!address.isNull() && address.scopeId().isEmpty())
        url.setHost(address.toString());
    else
        url.setHost(host);
    url.setPort(port);
    return url;
}

std::chrono::milliseconds probeTimeout(int addressCount)
{
    return addressCount <= 1 ? SingleAddressTimeout : PerAddressTimeout;
}

} // namespace BGrpcConnect

// ============================================================================
// BGrpcAddressProbe
// ============================================================================

BGrpcAddressProbe::BGrpcAddressProbe(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
{
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(m_timeout).count();
        attemptFailed(tr("no TLS connection within %1 s").arg(qMax<qint64>(1, seconds)));
    });
}

BGrpcAddressProbe::~BGrpcAddressProbe()
{
    abort();
}

void BGrpcAddressProbe::start(const QString &host, quint16 port, const QList<QHostAddress> &addresses)
{
    abort();
    m_host = host;
    m_port = port;
    m_attempts.clear();
    m_addresses.clear();
    m_next = 0;
    m_running = true;

    if (!addresses.isEmpty()) {
        tryAddresses(addresses);
        return;
    }

    // An address needs no lookup
    const QHostAddress literal(host);
    if (!literal.isNull()) {
        tryAddresses({literal});
        return;
    }

    const quint64 generation = m_generation;
    m_lookupId = QHostInfo::lookupHost(host, this, [this, generation](const QHostInfo &info) {
        if (generation != m_generation)
            return;
        m_lookupId = -1;
        if (info.error() != QHostInfo::NoError || info.addresses().isEmpty()) {
            finishFailed(tr("Cannot resolve the host name %1: %2")
                             .arg(m_host, info.errorString().isEmpty() ? tr("no address")
                                                                      : info.errorString()));
            return;
        }
        tryAddresses(info.addresses());
    });
}

void BGrpcAddressProbe::abort()
{
    ++m_generation;
    if (m_lookupId >= 0) {
        QHostInfo::abortHostLookup(m_lookupId);
        m_lookupId = -1;
    }
    m_timer->stop();
    dropSocket();
    m_running = false;
}

void BGrpcAddressProbe::tryAddresses(const QList<QHostAddress> &addresses)
{
    m_addresses = BGrpcConnect::orderAddresses(addresses, m_order);
    if (m_addresses.isEmpty()) {
        finishFailed(tr("Cannot resolve the host name %1: no address").arg(m_host));
        return;
    }
    m_timeout = m_timeoutOverride.count() > 0 ? m_timeoutOverride
                                              : BGrpcConnect::probeTimeout(int(m_addresses.size()));
    m_next = 0;
    tryNext();
}

void BGrpcAddressProbe::tryNext()
{
    dropSocket();
    if (m_next >= m_addresses.size()) {
        finishFailed(tr("Cannot reach the gRPC endpoint %1:%2 (%3)")
                         .arg(m_host).arg(m_port).arg(m_attempts.join(QLatin1String("; "))));
        return;
    }
    const QHostAddress address = m_addresses[m_next++];
    const quint64 generation = m_generation;

    auto *socket = new QSslSocket(this);
    m_socket = socket;
    QSslConfiguration ssl = socket->sslConfiguration();
    ssl.setPeerVerifyMode(QSslSocket::VerifyNone);  // self-signed; trust is the pin
    socket->setSslConfiguration(ssl);

    connect(socket, &QSslSocket::encrypted, this, [this, socket, address, generation]() {
        if (generation != m_generation || socket != m_socket)
            return;
        const QSslCertificate certificate = socket->peerCertificate();
        m_timer->stop();
        dropSocket();
        m_running = false;
        emit succeeded(address, certificate);
    });
    connect(socket, &QSslSocket::errorOccurred, this,
            [this, socket, generation](QAbstractSocket::SocketError) {
        if (generation != m_generation || socket != m_socket)
            return;
        attemptFailed(socket->errorString());
    });

    m_timer->start(m_timeout);
    // The address to connect to, the host name for the TLS server name
    const QString peerName = QHostAddress(m_host).isNull() ? m_host : QString();
    socket->connectToHostEncrypted(address.toString(), m_port, peerName);
}

void BGrpcAddressProbe::attemptFailed(const QString &reason)
{
    if (!m_running)
        return;
    m_timer->stop();
    const QHostAddress address = m_addresses.value(m_next - 1);
    m_attempts.append(QStringLiteral("%1: %2").arg(address.toString(), reason));
    // Not inside the socket's own signal: the socket is deleted later
    const quint64 generation = m_generation;
    QTimer::singleShot(0, this, [this, generation]() {
        if (generation == m_generation && m_running)
            tryNext();
    });
}

void BGrpcAddressProbe::finishFailed(const QString &reason)
{
    m_timer->stop();
    dropSocket();
    m_running = false;
    emit failed(reason);
}

void BGrpcAddressProbe::dropSocket()
{
    if (!m_socket)
        return;
    QSslSocket *socket = m_socket;
    m_socket = nullptr;
    socket->disconnect(this);
    socket->abort();
    socket->deleteLater();
}

// ============================================================================
// BGrpcPinnedRelay
// ============================================================================

namespace {

/// What the channel may send before the TLS connection is verified: the
/// HTTP/2 preface, SETTINGS and the first request headers fit easily
constexpr qint64 MaxPendingBeforeVerified = 1024 * 1024;

/// One connection of the channel and its TLS connection to the Director
struct RelayPair
{
    QPointer<QLocalSocket> local;
    QPointer<QSslSocket> tls;
    QByteArray pending;     ///< from the channel, held until verified
    bool verified = false;
    bool closed = false;

    void close()
    {
        if (closed)
            return;
        closed = true;
        if (tls) {
            tls->disconnectFromHost();
            tls->deleteLater();
        }
        if (local) {
            local->disconnectFromServer();
            local->deleteLater();
        }
    }

    void abort()
    {
        if (closed)
            return;
        closed = true;
        pending.clear();
        if (tls) {
            tls->abort();
            tls->deleteLater();
        }
        if (local) {
            local->abort();
            local->deleteLater();
        }
    }
};

} // namespace

BGrpcPinnedRelay::BGrpcPinnedRelay(QObject *parent)
    : QObject(parent)
{
}

BGrpcPinnedRelay::~BGrpcPinnedRelay()
{
    if (m_server)
        m_server->close();
}

QString BGrpcPinnedRelay::fingerprint(const QSslCertificate &certificate)
{
    if (certificate.isNull())
        return QString();
    return QString::fromLatin1(
        QCryptographicHash::hash(certificate.toDer(), QCryptographicHash::Sha256).toHex());
}

QString BGrpcPinnedRelay::normalizedFingerprint(const QString &fingerprint)
{
    QString normalized = fingerprint.trimmed().toLower();
    normalized.remove(QLatin1Char(':'));
    normalized.remove(QLatin1Char(' '));
    return normalized;
}

bool BGrpcPinnedRelay::start(const QHostAddress &address, quint16 port, const QString &serverName,
                             const QString &pinnedFingerprint)
{
    m_address = address;
    m_port = port;
    m_serverName = serverName;
    m_pinnedFingerprint = normalizedFingerprint(pinnedFingerprint);
    m_error.clear();

    if (m_pinnedFingerprint.isEmpty()) {
        m_error = tr("no pinned certificate");
        return false;
    }

    if (m_server) {
        m_server->close();
        m_server->deleteLater();
    }
    m_server = new QLocalServer(this);
    // Only this user may connect (Unix: socket file mode, Windows: pipe ACL)
    m_server->setSocketOptions(QLocalServer::UserAccessOption);
    connect(m_server, &QLocalServer::newConnection, this, &BGrpcPinnedRelay::acceptConnections);

    // Lower case letters, digits and '-' only: the name is the host part of
    // the channel URL, and QUrl lowers the case of a host
    const QString name = QStringLiteral("onesimus-grpc-%1-%2")
                             .arg(QCoreApplication::applicationPid())
                             .arg(QRandomGenerator::system()->generate64(), 16, 16, QLatin1Char('0'));
    if (!m_server->listen(name)) {
        m_error = m_server->errorString();
        return false;
    }
    return true;
}

QString BGrpcPinnedRelay::serverName() const
{
    return m_server ? m_server->serverName() : QString();
}

QUrl BGrpcPinnedRelay::channelUrl() const
{
    // QGrpcHttp2Channel connects a QLocalSocket to host() + path()
    QUrl url;
    url.setScheme(QStringLiteral("unix"));
    url.setHost(serverName());
    return url;
}

void BGrpcPinnedRelay::acceptConnections()
{
    while (m_server && m_server->hasPendingConnections()) {
        QLocalSocket *local = m_server->nextPendingConnection();
        if (!local)
            break;

        auto pair = std::make_shared<RelayPair>();
        pair->local = local;
        auto *tls = new QSslSocket(this);
        pair->tls = tls;

        // The pin is the trust anchor: verification by Qt is off, the
        // certificate of this connection is compared in encrypted() below
        QSslConfiguration ssl = tls->sslConfiguration();
        ssl.setPeerVerifyMode(QSslSocket::VerifyNone);
        ssl.setAllowedNextProtocols({QByteArrayLiteral("h2")});
        tls->setSslConfiguration(ssl);

        connect(local, &QLocalSocket::readyRead, this, [pair]() {
            if (pair->closed || !pair->local)
                return;
            const QByteArray data = pair->local->readAll();
            if (pair->verified && pair->tls) {
                pair->tls->write(data);
                return;
            }
            pair->pending.append(data);
            if (pair->pending.size() > MaxPendingBeforeVerified)
                pair->abort();
        });
        connect(local, &QLocalSocket::disconnected, this, [pair]() { pair->close(); });

        connect(tls, &QSslSocket::encrypted, this, [this, pair]() {
            if (pair->closed || !pair->tls)
                return;
            const QString presented = fingerprint(pair->tls->peerCertificate());
            if (presented.isEmpty() || presented != m_pinnedFingerprint) {
                // Not one byte of the channel (session token, commands) goes
                // to a server with another certificate
                pair->abort();
                emit pinMismatch(presented);
                return;
            }
            pair->verified = true;
            if (!pair->pending.isEmpty()) {
                pair->tls->write(pair->pending);
                pair->pending.clear();
            }
            if (pair->local && pair->tls->bytesAvailable() > 0)
                pair->local->write(pair->tls->readAll());
        });
        connect(tls, &QSslSocket::readyRead, this, [pair]() {
            if (pair->closed || !pair->verified || !pair->tls || !pair->local)
                return;
            pair->local->write(pair->tls->readAll());
        });
        connect(tls, &QSslSocket::disconnected, this, [pair]() {
            // Hand the channel what arrived with the close
            if (!pair->closed && pair->verified && pair->tls && pair->local)
                pair->local->write(pair->tls->readAll());
            pair->close();
        });
        connect(tls, &QSslSocket::errorOccurred, this, [this, pair](QAbstractSocket::SocketError error) {
            if (pair->closed)
                return;
            const QString reason = pair->tls ? pair->tls->errorString() : QString();
            if (error == QAbstractSocket::RemoteHostClosedError && pair->verified) {
                pair->close();
                return;
            }
            pair->abort();
            emit connectionFailed(reason);
        });

        // The address the probe reached, the host name for the TLS server name
        tls->connectToHostEncrypted(m_address.toString(), m_port,
                                    m_serverName.isEmpty() ? m_address.toString() : m_serverName);
    }
}
