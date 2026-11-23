// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bgrpcpin_test.cpp
 * @brief The certificate pin of the gRPC transport applies to the connection
 *        the channel really uses (P0-7)
 *
 * No Director: a TLS server on the loopback interface that presents a
 * chosen certificate per connection.
 *
 * - BGrpcPinnedRelay forwards both ways when the certificate matches the pin,
 *   and closes the connection without forwarding a byte when it does not.
 * - BGrpcTransport end to end: the server shows the pinned certificate to the
 *   pre-flight probe and another one to the next connection (an attacker
 *   who lets the probe through). Before P0-7 the channel ran with
 *   verification off on that second connection and sent the login to it.
 */

#include <QtTest>
#include <QTcpServer>
#include <QSslSocket>
#include <QSslKey>
#include <QSslConfiguration>
#include <QLocalSocket>

#include "director/bgrpcconnect.h"
#include "director/bgrpctransport.h"
#include "btlstestcerts.h"

namespace {

QSslCertificate certificate(const char *pem)
{
    return QSslCertificate(QByteArray(pem), QSsl::Pem);
}

/**
 * @brief TLS server; connection n presents certificates[min(n, last)]
 */
class SwitchingTlsServer : public QTcpServer
{
    Q_OBJECT

public:
    struct Identity
    {
        const char *cert;
        const char *key;
    };

    explicit SwitchingTlsServer(QList<Identity> identities, QObject *parent = nullptr)
        : QTcpServer(parent), m_identities(std::move(identities))
    {
    }

    int connections = 0;
    QByteArray receivedPlain;   ///< everything decrypted on any connection
    QList<QPointer<QSslSocket>> sockets;

protected:
    void incomingConnection(qintptr descriptor) override
    {
        const Identity identity = m_identities.value(qMin(connections, int(m_identities.size()) - 1));
        ++connections;
        auto *socket = new QSslSocket(this);
        if (!socket->setSocketDescriptor(descriptor)) {
            delete socket;
            return;
        }
        QSslConfiguration ssl = QSslConfiguration::defaultConfiguration();
        ssl.setLocalCertificate(certificate(identity.cert));
        ssl.setPrivateKey(QSslKey(QByteArray(identity.key), QSsl::Ec, QSsl::Pem));
        ssl.setPeerVerifyMode(QSslSocket::VerifyNone);
        ssl.setAllowedNextProtocols({QByteArrayLiteral("h2")});
        socket->setSslConfiguration(ssl);
        connect(socket, &QSslSocket::readyRead, this, [this, socket]() {
            receivedPlain.append(socket->readAll());
        });
        sockets.append(socket);
        socket->startServerEncryption();
    }

private:
    QList<Identity> m_identities;
};

const SwitchingTlsServer::Identity PinA{BTlsTestCerts::PinACert, BTlsTestCerts::PinAKey};
const SwitchingTlsServer::Identity PinB{BTlsTestCerts::PinBCert, BTlsTestCerts::PinBKey};

} // namespace

class BGrpcPinTest : public QObject
{
    Q_OBJECT

private slots:
    void fingerprintIsSha256OfDer();
    void relayForwardsWhenCertificateMatchesPin();
    void relayRefusesOtherCertificate();
    void relayNeedsAPin();
    void transportRefusesCertificateSwappedAfterProbe();
    void simulatorLoginThroughRelay();
};

void BGrpcPinTest::fingerprintIsSha256OfDer()
{
    const QSslCertificate a = certificate(BTlsTestCerts::PinACert);
    QCOMPARE(BGrpcPinnedRelay::fingerprint(a),
             QString::fromLatin1(QCryptographicHash::hash(a.toDer(), QCryptographicHash::Sha256).toHex()));
    QCOMPARE(BGrpcPinnedRelay::fingerprint(a), BGrpcTransport::certificateFingerprint(a));
    QCOMPARE(BGrpcPinnedRelay::normalizedFingerprint(QStringLiteral(" AB:cd:EF ")), QStringLiteral("abcdef"));
}

void BGrpcPinTest::relayForwardsWhenCertificateMatchesPin()
{
    SwitchingTlsServer server({PinA});
    QVERIFY(server.listen(QHostAddress::LocalHost));

    BGrpcPinnedRelay relay;
    QSignalSpy mismatch(&relay, &BGrpcPinnedRelay::pinMismatch);
    QVERIFY2(relay.start(QHostAddress::LocalHost, server.serverPort(), QString(),
                         BGrpcPinnedRelay::fingerprint(certificate(BTlsTestCerts::PinACert))),
             qPrintable(relay.errorString()));
    QCOMPARE(relay.channelUrl().scheme(), QStringLiteral("unix"));
    QCOMPARE(relay.channelUrl().host(), relay.serverName());

    QLocalSocket channel;
    channel.connectToServer(relay.channelUrl().host());
    QVERIFY(channel.waitForConnected(5000));
    channel.write("PRI * HTTP/2.0");   // sent before TLS is up: held, then forwarded
    QTRY_COMPARE(server.receivedPlain, QByteArray("PRI * HTTP/2.0"));

    QTRY_VERIFY(!server.sockets.isEmpty() && server.sockets.first());
    server.sockets.first()->write("SETTINGS");
    QByteArray back;
    QTRY_VERIFY((back += channel.readAll()) == "SETTINGS");
    QCOMPARE(mismatch.count(), 0);
}

void BGrpcPinTest::relayRefusesOtherCertificate()
{
    SwitchingTlsServer server({PinB});
    QVERIFY(server.listen(QHostAddress::LocalHost));

    BGrpcPinnedRelay relay;
    QSignalSpy mismatch(&relay, &BGrpcPinnedRelay::pinMismatch);
    QVERIFY(relay.start(QHostAddress::LocalHost, server.serverPort(), QString(),
                        BGrpcPinnedRelay::fingerprint(certificate(BTlsTestCerts::PinACert))));

    QLocalSocket channel;
    QSignalSpy closed(&channel, &QLocalSocket::disconnected);
    channel.connectToServer(relay.serverName());
    QVERIFY(channel.waitForConnected(5000));
    channel.write("x-onesimus-session: secret-token");

    QTRY_COMPARE(mismatch.count(), 1);
    QCOMPARE(mismatch.first().first().toString(),
             BGrpcPinnedRelay::fingerprint(certificate(BTlsTestCerts::PinBCert)));
    QTRY_VERIFY(closed.count() > 0 || channel.state() == QLocalSocket::UnconnectedState);
    QTest::qWait(200);
    QVERIFY(server.receivedPlain.isEmpty());   // not one byte reached the server
}

void BGrpcPinTest::relayNeedsAPin()
{
    BGrpcPinnedRelay relay;
    QVERIFY(!relay.start(QHostAddress::LocalHost, 1, QString(), QString()));
    QVERIFY(!relay.errorString().isEmpty());
}

void BGrpcPinTest::transportRefusesCertificateSwappedAfterProbe()
{
    // The probe (connection 1) sees the pinned certificate, the channel's
    // connection (2 and later) another one
    SwitchingTlsServer server({PinA, PinB});
    QVERIFY(server.listen(QHostAddress::LocalHost));

    BGrpcTransport transport;
    QSignalSpy failed(&transport, &BGrpcTransport::failed);
    QSignalSpy notTrusted(&transport, &BGrpcTransport::certificateNotTrusted);
    QSignalSpy loggedIn(&transport, &BGrpcTransport::loggedIn);

    transport.open(QStringLiteral("127.0.0.1"), server.serverPort(), QStringLiteral("admin"),
                   QStringLiteral("secret"),
                   BGrpcPinnedRelay::fingerprint(certificate(BTlsTestCerts::PinACert)));

    QTRY_VERIFY_WITH_TIMEOUT(failed.count() > 0, 15000);
    const QString reason = failed.first().first().toString();
    QVERIFY2(reason.contains(QLatin1String("does not match the pinned certificate")), qPrintable(reason));
    QVERIFY(reason.contains(BGrpcPinnedRelay::fingerprint(certificate(BTlsTestCerts::PinBCert))));
    QCOMPARE(notTrusted.count(), 0);   // the probe was fine
    QCOMPARE(loggedIn.count(), 0);
    QVERIFY(server.connections >= 2);
    QTest::qWait(200);
    QVERIFY2(server.receivedPlain.isEmpty(), server.receivedPlain.left(64).constData());   // no HTTP/2, no login
}

void BGrpcPinTest::simulatorLoginThroughRelay()
{
    // Optional, not in CI: a running onesimus-sim (or odir on a test
    // machine), e.g. ONESIMUS_TEST_GRPC=127.0.0.1:21101:admin:demo:<sha256>
    const QStringList target = qEnvironmentVariable("ONESIMUS_TEST_GRPC").split(QLatin1Char(':'));
    if (target.size() != 5)
        QSKIP("ONESIMUS_TEST_GRPC=host:port:console:password:fingerprint not set");

    BGrpcTransport transport;
    QSignalSpy loggedIn(&transport, &BGrpcTransport::loggedIn);
    QSignalSpy failed(&transport, &BGrpcTransport::failed);
    QSignalSpy finished(&transport, &BGrpcTransport::commandFinished);
    transport.open(target[0], target[1].toInt(), target[2], target[3], target[4]);
    QTRY_VERIFY_WITH_TIMEOUT(loggedIn.count() > 0 || failed.count() > 0, 15000);
    QVERIFY2(loggedIn.count() == 1, failed.isEmpty() ? "" : qPrintable(failed.first().first().toString()));

    transport.execute(QStringLiteral("version"));
    QTRY_VERIFY_WITH_TIMEOUT(finished.count() > 0, 15000);
    QVERIFY(finished.first().at(1).toBool());
    qInfo() << "logged in to" << loggedIn.first().first().toString() << loggedIn.first().at(1).toString();
}

QTEST_GUILESS_MAIN(BGrpcPinTest)
#include "bgrpcpin_test.moc"
