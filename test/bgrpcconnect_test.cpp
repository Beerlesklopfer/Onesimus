// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

// bgrpcconnect_test - reaching the gRPC port: order of the resolved
// addresses, the TLS probe per address against local sockets, and the texts
// of failed calls. No Director, no network beyond the loopback interface.

#include "director/bgrpcconnect.h"

#include <QSignalSpy>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QSslKey>
#include <QSslServer>
#include <QSslSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtTest>

namespace {

// A throw-away self-signed certificate for CN=localhost (test only)
const char CertPem[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBfzCCASWgAwIBAgIUJeFqWI4gkEwx92FgZ+ElIr/i23cwCgYIKoZIzj0EAwIw\n"
    "FDESMBAGA1UEAwwJbG9jYWxob3N0MCAXDTI2MTExOTAzNDQwMFoYDzIxMjYxMDI2\n"
    "MDM0NDAwWjAUMRIwEAYDVQQDDAlsb2NhbGhvc3QwWTATBgcqhkjOPQIBBggqhkjO\n"
    "PQMBBwNCAASWa3eKVGTaj9mimdnQ/Ui1CPVfzYe37qpnkFbP8TTeQGnWNgvhSqvI\n"
    "DiEHmSguSxN0tZwomhz0fta0m8sh5M6Ro1MwUTAdBgNVHQ4EFgQUGL5IHQDda5Kj\n"
    "OCHpZkIduoA3OWMwHwYDVR0jBBgwFoAUGL5IHQDda5KjOCHpZkIduoA3OWMwDwYD\n"
    "VR0TAQH/BAUwAwEB/zAKBggqhkjOPQQDAgNIADBFAiB/DG2CID4qo2eUBnbANEiX\n"
    "XOWbvTwqNSTCrQIvPg3ZtwIhAIOpKhyIiw3+D0o1H70poeh8mES46DGKMQcKmoLc\n"
    "58nj\n"
    "-----END CERTIFICATE-----\n";
const char KeyPem[] =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgueF1yiax5B48j738\n"
    "akr9S7Hl5prtM/9VPK8yiK+LLP2hRANCAASWa3eKVGTaj9mimdnQ/Ui1CPVfzYe3\n"
    "7qpnkFbP8TTeQGnWNgvhSqvIDiEHmSguSxN0tZwomhz0fta0m8sh5M6R\n"
    "-----END PRIVATE KEY-----\n";

QSslCertificate testCertificate()
{
    return QSslCertificate(QByteArray(CertPem), QSsl::Pem);
}

/// A TLS server on 127.0.0.1 with the test certificate
class TlsServer
{
public:
    TlsServer()
    {
        QSslConfiguration config = QSslConfiguration::defaultConfiguration();
        config.setLocalCertificate(testCertificate());
        config.setPrivateKey(QSslKey(QByteArray(KeyPem), QSsl::Ec, QSsl::Pem));
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        server.setSslConfiguration(config);
        server.listen(QHostAddress(QStringLiteral("127.0.0.1")), 0);
    }
    QSslServer server;
};

} // namespace

class BGrpcConnectTest : public QObject
{
    Q_OBJECT

private slots:
    void ordersIpv4First()
    {
        const QHostAddress v6a(QStringLiteral("2001:db8::1"));
        const QHostAddress v4a(QStringLiteral("192.0.2.10"));
        const QHostAddress v6b(QStringLiteral("2001:db8::2"));
        const QHostAddress v4b(QStringLiteral("192.0.2.11"));
        const QList<QHostAddress> resolved{v6a, v4a, v6b, v6a, v4b};

        const QList<QHostAddress> ipv4First =
            BGrpcConnect::orderAddresses(resolved, BGrpcConnect::Order::Ipv4First);
        QCOMPARE(ipv4First, (QList<QHostAddress>{v4a, v4b, v6a, v6b}));

        const QList<QHostAddress> asResolved =
            BGrpcConnect::orderAddresses(resolved, BGrpcConnect::Order::ResolverOrder);
        QCOMPARE(asResolved, (QList<QHostAddress>{v6a, v4a, v6b, v4b}));

        QVERIFY(BGrpcConnect::orderAddresses({QHostAddress()}, BGrpcConnect::Order::Ipv4First).isEmpty());
    }

    void statusTextsCarryTheCode()
    {
        QCOMPARE(BGrpcConnect::statusCodeName(4), QStringLiteral("DEADLINE_EXCEEDED"));
        QCOMPARE(BGrpcConnect::statusCodeName(12), QStringLiteral("UNIMPLEMENTED"));
        QCOMPARE(BGrpcConnect::statusCodeName(99), QStringLiteral("STATUS_99"));

        // An empty message gets a plain explanation, with the deadline
        const QString timeout = BGrpcConnect::failureText(4, QString(), 15);
        QVERIFY2(timeout.startsWith(QStringLiteral("DEADLINE_EXCEEDED: ")), qPrintable(timeout));
        QVERIFY2(timeout.contains(QStringLiteral("15 s")), qPrintable(timeout));

        const QString unavailable = BGrpcConnect::failureText(14, QStringLiteral("  "), 15);
        QVERIFY2(unavailable.startsWith(QStringLiteral("UNAVAILABLE: ")), qPrintable(unavailable));
        QVERIFY(unavailable.size() > QStringLiteral("UNAVAILABLE: ").size());

        // A message is kept as it is, behind the code
        QCOMPARE(BGrpcConnect::failureText(7, QStringLiteral("command \"configure\" not allowed"), 15),
                 QStringLiteral("PERMISSION_DENIED: command \"configure\" not allowed"));

        // grpc-message arrives percent-encoded; an expired session is explained
        const QString expired = BGrpcConnect::failureText(16, QStringLiteral("invalid%20session%20token"), 15);
        QVERIFY2(expired.startsWith(QStringLiteral("UNAUTHENTICATED: invalid session token - ")),
                 qPrintable(expired));
        QVERIFY2(expired.contains(QStringLiteral("8 hours")), qPrintable(expired));
        QCOMPARE(BGrpcConnect::failureText(13, QStringLiteral("disk%20full"), 15),
                 QStringLiteral("INTERNAL: disk full"));

        // Every code says something
        for (int code = 1; code <= 16; ++code) {
            const QString text = BGrpcConnect::failureText(code, QString(), 15);
            QVERIFY2(text.startsWith(BGrpcConnect::statusCodeName(code) + QStringLiteral(": ")),
                     qPrintable(text));
            QVERIFY2(text.size() > BGrpcConnect::statusCodeName(code).size() + 4, qPrintable(text));
        }
    }

    void unimplementedLoginIsAProtocolMismatch()
    {
        // What the maintainer saw as "gRPC login failed: " with nothing
        // behind it: the Director serves another package of the service
        const QString text = BGrpcConnect::loginFailureText(12, QString(), 15,
                                                            QStringLiteral("onesimus.director"));
        QVERIFY2(text.startsWith(QStringLiteral("UNIMPLEMENTED: ")), qPrintable(text));
        QVERIFY2(text.contains(QStringLiteral("protocol version mismatch")), qPrintable(text));
        QVERIFY2(text.contains(QStringLiteral("onesimus.director")), qPrintable(text));
        QVERIFY2(text.contains(QStringLiteral("BNET")), qPrintable(text));

        // The Director's own words are kept
        QVERIFY(BGrpcConnect::loginFailureText(12, QStringLiteral("unknown service"), 15,
                                               QStringLiteral("onesimus.director"))
                    .contains(QStringLiteral("(unknown service)")));

        // Every other code as failureText()
        QCOMPARE(BGrpcConnect::loginFailureText(4, QString(), 15, QStringLiteral("x")),
                 BGrpcConnect::failureText(4, QString(), 15));
    }

    void channelUrlUsesTheProbedAddress()
    {
        QCOMPARE(BGrpcConnect::channelUrl(QStringLiteral("dir.example"), 9201,
                                          QHostAddress(QStringLiteral("192.0.2.10"))).toString(),
                 QStringLiteral("https://192.0.2.10:9201"));
        QCOMPARE(BGrpcConnect::channelUrl(QStringLiteral("dir.example"), 9201,
                                          QHostAddress(QStringLiteral("2001:db8::1"))).toString(),
                 QStringLiteral("https://[2001:db8::1]:9201"));
        QCOMPARE(BGrpcConnect::channelUrl(QStringLiteral("dir.example"), 9201, QHostAddress()).toString(),
                 QStringLiteral("https://dir.example:9201"));
        QHostAddress zoned(QStringLiteral("fe80::1"));
        zoned.setScopeId(QStringLiteral("eth0"));
        QCOMPARE(BGrpcConnect::channelUrl(QStringLiteral("dir.example"), 9201, zoned).host(),
                 QStringLiteral("dir.example"));
    }

    void probeTimeouts()
    {
        QCOMPARE(BGrpcConnect::probeTimeout(1), BGrpcConnect::SingleAddressTimeout);
        QCOMPARE(BGrpcConnect::probeTimeout(2), BGrpcConnect::PerAddressTimeout);
        QVERIFY(BGrpcConnect::PerAddressTimeout < BGrpcConnect::SingleAddressTimeout);
    }

    void probeSkipsDeadAndSilentAddresses()
    {
        TlsServer tls;
        QVERIFY(tls.server.isListening());
        const quint16 port = tls.server.serverPort();

        // An address that accepts TCP but never answers the handshake: the
        // unroutable IPv6 address of the maintainer, in small
        QTcpServer silent;
        QVERIFY(silent.listen(QHostAddress(QStringLiteral("127.0.0.3")), port));

        BGrpcAddressProbe probe;
        probe.setOrder(BGrpcConnect::Order::ResolverOrder);
        probe.setTimeout(std::chrono::milliseconds(600));
        QSignalSpy ok(&probe, &BGrpcAddressProbe::succeeded);
        QSignalSpy failed(&probe, &BGrpcAddressProbe::failed);

        // 127.0.0.2: nothing listens (refused); 127.0.0.3: silent
        probe.start(QStringLiteral("dir.test"), port,
                    {QHostAddress(QStringLiteral("127.0.0.2")), QHostAddress(QStringLiteral("127.0.0.3")),
                     QHostAddress(QStringLiteral("127.0.0.1"))});
        QTRY_COMPARE_WITH_TIMEOUT(ok.size() + failed.size(), 1, 10000);
        QCOMPARE(ok.size(), 1);
        QCOMPARE(ok.first().at(0).value<QHostAddress>(), QHostAddress(QStringLiteral("127.0.0.1")));
        QCOMPARE(ok.first().at(1).value<QSslCertificate>(), testCertificate());
        QCOMPARE(probe.attempts().size(), 2);
        QVERIFY2(probe.attempts().at(0).startsWith(QStringLiteral("127.0.0.2: ")), qPrintable(probe.attempts().at(0)));
        QVERIFY2(probe.attempts().at(1).startsWith(QStringLiteral("127.0.0.3: no TLS connection within")),
                 qPrintable(probe.attempts().at(1)));
        QVERIFY(!probe.isRunning());
    }

    void probeOrderIsIpv4FirstByDefault()
    {
        TlsServer tls;
        BGrpcAddressProbe probe;
        probe.setTimeout(std::chrono::milliseconds(600));
        QSignalSpy ok(&probe, &BGrpcAddressProbe::succeeded);
        // ::1 is not served by the IPv4 server; IPv4 first means it is never tried
        probe.start(QStringLiteral("dir.test"), tls.server.serverPort(),
                    {QHostAddress(QStringLiteral("::1")), QHostAddress(QStringLiteral("127.0.0.1"))});
        QTRY_COMPARE_WITH_TIMEOUT(ok.size(), 1, 5000);
        QCOMPARE(probe.addresses().first(), QHostAddress(QStringLiteral("127.0.0.1")));
        QVERIFY(probe.attempts().isEmpty());
    }

    void probeResolvesTheHostName()
    {
        TlsServer tls;
        BGrpcAddressProbe probe;
        probe.setTimeout(std::chrono::milliseconds(800));
        QSignalSpy ok(&probe, &BGrpcAddressProbe::succeeded);
        QSignalSpy failed(&probe, &BGrpcAddressProbe::failed);
        probe.start(QStringLiteral("localhost"), tls.server.serverPort());
        QTRY_COMPARE_WITH_TIMEOUT(ok.size() + failed.size(), 1, 10000);
        QVERIFY2(ok.size() == 1, failed.isEmpty() ? "" : qPrintable(failed.first().at(0).toString()));
        QCOMPARE(ok.first().at(0).value<QHostAddress>(), QHostAddress(QStringLiteral("127.0.0.1")));
    }

    void probeReportsEveryAddressWhenAllFail()
    {
        // A port nobody listens on
        QTcpServer closed;
        QVERIFY(closed.listen(QHostAddress(QStringLiteral("127.0.0.1")), 0));
        const quint16 port = closed.serverPort();
        closed.close();

        BGrpcAddressProbe probe;
        probe.setTimeout(std::chrono::milliseconds(600));
        QSignalSpy failed(&probe, &BGrpcAddressProbe::failed);
        probe.start(QStringLiteral("dir.test"), port,
                    {QHostAddress(QStringLiteral("127.0.0.1")), QHostAddress(QStringLiteral("127.0.0.2"))});
        QTRY_COMPARE_WITH_TIMEOUT(failed.size(), 1, 5000);
        const QString reason = failed.first().at(0).toString();
        QVERIFY2(reason.contains(QStringLiteral("dir.test:%1").arg(port)), qPrintable(reason));
        QVERIFY2(reason.contains(QStringLiteral("127.0.0.1: ")), qPrintable(reason));
        QVERIFY2(reason.contains(QStringLiteral("127.0.0.2: ")), qPrintable(reason));
    }

    void probeAbortSendsNothing()
    {
        TlsServer tls;
        BGrpcAddressProbe probe;
        QSignalSpy ok(&probe, &BGrpcAddressProbe::succeeded);
        QSignalSpy failed(&probe, &BGrpcAddressProbe::failed);
        probe.start(QStringLiteral("127.0.0.1"), tls.server.serverPort());
        probe.abort();
        QTest::qWait(300);
        QCOMPARE(ok.size() + failed.size(), 0);
    }
};

QTEST_GUILESS_MAIN(BGrpcConnectTest)
#include "bgrpcconnect_test.moc"
