// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BGRPCCONNECT_H
#define BGRPCCONNECT_H

#include <QHostAddress>
#include <QList>
#include <QObject>
#include <QPointer>
#include <QSslCertificate>
#include <QString>
#include <QStringList>
#include <QUrl>

#include <chrono>

class QSslSocket;
class QTimer;

/**
 * @file bgrpcconnect.h
 * @brief Reaching the gRPC port of the Onesimus Director: addresses, probe,
 *        readable failures
 *
 * QtCore and QtNetwork only, without QtGrpc, so that it can be tested on its
 * own (bgrpcconnect_test).
 *
 * Why there is a probe. A host name can resolve to an IPv4 and an IPv6
 * address of which only one is reachable from the client. The HTTP/2
 * channel of QtGrpc connects to the host name and may sit on the address
 * that does not answer until the deadline of the call has passed; the user
 * then got "gRPC login failed: " without a reason. The transport therefore
 * resolves the name itself, tries the addresses one after the other with a
 * short TLS handshake each, and hands the channel the address that answered.
 * The handshake of the probe is also the one whose certificate is checked
 * against the pinned fingerprint, so the pin applies to exactly the address
 * the channel uses.
 */
namespace BGrpcConnect {

/// In which order the resolved addresses are tried
enum class Order {
    ResolverOrder,  ///< As the resolver returned them
    Ipv4First,      ///< IPv4 addresses first (each family in resolver order)
};

/**
 * @brief The addresses to try, in order, without duplicates
 *
 * The transport uses Ipv4First: an IPv6 address that is announced but not
 * routed is the common failure on home and office networks, while an
 * unreachable IPv4 address next to a working IPv6 one is rare. The probe
 * of an address that does not answer is short, so the other family is
 * still reached quickly in that case.
 */
QList<QHostAddress> orderAddresses(const QList<QHostAddress> &addresses, Order order);

/// The canonical gRPC name of a status code ("DEADLINE_EXCEEDED"); the
/// number for a code that is not known
QString statusCodeName(int code);

/**
 * @brief The text of a failed call: code name and reason
 * @param code             gRPC status code (QtGrpc::StatusCode as int)
 * @param message          The status message; may be empty
 * @param deadlineSeconds  Deadline of the call, named when the call timed out
 *
 * "DEADLINE_EXCEEDED: no connection within 15 s". An empty message is
 * replaced by a plain explanation of the code; a message is kept as it is.
 */
QString failureText(int code, const QString &message, int deadlineSeconds);

/**
 * @brief The text of a failed GetChallenge or Login call
 *
 * As failureText(), except for UNIMPLEMENTED: the Director answers, but not
 * the service this build calls. That happens when the gRPC package names
 * differ (@p servicePackage is the package this build calls, e.g.
 * "onesimus.director"; builds of the Onesimus Director before the rename of
 * October 2026 serve "bareos.director", and a much newer one may serve
 * another). The message says so and names the way out.
 */
QString loginFailureText(int code, const QString &message, int deadlineSeconds,
                         const QString &servicePackage);

/**
 * @brief The URL the HTTP/2 channel connects to
 *
 * The probed address, so that the channel uses the address whose
 * certificate was checked. An IPv6 address with a zone (link-local) cannot
 * be put into a URL reliably; the host name is used then, as before.
 */
QUrl channelUrl(const QString &host, int port, const QHostAddress &address);

/// How long the probe waits for one address: the whole pre-flight time if
/// there is only one, a short time each if there are several
std::chrono::milliseconds probeTimeout(int addressCount);

/// Time for the whole pre-flight with a single address
constexpr std::chrono::milliseconds SingleAddressTimeout{10000};
/// Time per address when there are several
constexpr std::chrono::milliseconds PerAddressTimeout{4000};

} // namespace BGrpcConnect

/**
 * @brief Finds the first address of a host that accepts a TLS connection on
 *        a port
 *
 * Resolves the host name (unless addresses are given or the host is an
 * address), orders the addresses (BGrpcConnect::orderAddresses()) and opens
 * a TLS connection to one after the other, each with its own timeout. The
 * handshake sends the host name as the TLS server name. The first address
 * whose handshake completes is reported with the certificate it presented;
 * the connection is closed again. Certificates are not verified here: the
 * caller compares the fingerprint with the pinned one.
 *
 * Lives in the thread it was created in; all results are signals.
 */
class BGrpcAddressProbe : public QObject
{
    Q_OBJECT

public:
    explicit BGrpcAddressProbe(QObject *parent = nullptr);
    ~BGrpcAddressProbe() override;

    /**
     * @brief Starts the probe
     * @param host      Host name or address as the user entered it
     * @param port      The gRPC port
     * @param addresses Addresses to try; empty: resolve @p host
     */
    void start(const QString &host, quint16 port, const QList<QHostAddress> &addresses = {});

    /// Stops without a result signal
    void abort();

    bool isRunning() const { return m_running; }

    /// Overrides BGrpcConnect::probeTimeout() (tests)
    void setTimeout(std::chrono::milliseconds perAddress) { m_timeoutOverride = perAddress; }
    void setOrder(BGrpcConnect::Order order) { m_order = order; }

    /// The addresses in the order they are (or were) tried
    QList<QHostAddress> addresses() const { return m_addresses; }
    /// What happened at each address that failed, "address: reason"
    QStringList attempts() const { return m_attempts; }

signals:
    void succeeded(const QHostAddress &address, const QSslCertificate &certificate);
    void failed(const QString &reason);

private:
    void tryAddresses(const QList<QHostAddress> &addresses);
    void tryNext();
    void attemptFailed(const QString &reason);
    void finishFailed(const QString &reason);
    void dropSocket();

    QString m_host;
    quint16 m_port = 0;
    BGrpcConnect::Order m_order = BGrpcConnect::Order::Ipv4First;
    std::chrono::milliseconds m_timeoutOverride{0};
    std::chrono::milliseconds m_timeout{0};
    QList<QHostAddress> m_addresses;
    int m_next = 0;
    QStringList m_attempts;
    QPointer<QSslSocket> m_socket;
    QTimer *m_timer = nullptr;
    int m_lookupId = -1;
    quint64 m_generation = 0;
    bool m_running = false;
};

#endif // BGRPCCONNECT_H
