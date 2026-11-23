// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bareosauth_security_test.cpp
 * @brief BareosAuth against fake BNET Directors on the loopback interface
 *
 * No real Director. A small server (QTcpServer, or QSslServer for
 * certificate TLS) plays the Director's side of the Bareos console
 * handshake (Bareos 25 cram_md5.cc, CramMd5Handshake::DoHandshake() with
 * initiated_by_remote = true: challenge first, then answer the console's
 * challenge) and can misbehave in the ways an impostor would.
 *
 * - P0-6: a server that does not know the password, answers with a wrong
 *   HMAC, skips the mutual half or reflects the console's challenge is
 *   refused; the correct Director is accepted; the TLS policy is checked;
 *   only PSK-* cipher names are kept for TLS-PSK.
 * - P0-11: certificate TLS with verification needs a readable CA file and a
 *   certificate for the host name, or a common name in TLS Allowed CN.
 */

#include <QtTest>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSslServer>
#include <QSslSocket>
#include <QSslKey>
#include <QSslConfiguration>
#include <QTemporaryDir>
#include <QMessageAuthenticationCode>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QtEndian>

#include "director/bareosauth.h"
#include "btlstestcerts.h"

namespace {

const QString Password = QStringLiteral("console-secret");
const QString ConsoleName = QStringLiteral("admin");

QByteArray keyOf(const QString &password)
{
    return QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Md5).toHex();
}

/// Bareos BinToBase64() (lib/base64.cc), both variants
QByteArray bareosBase64(const QByteArray &bin, bool compatible)
{
    static const char digits[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    QByteArray out;
    quint32 reg = 0;
    int rem = 0;
    for (int i = 0; i < bin.size();) {
        if (rem < 6) {
            reg <<= 8;
            if (compatible)
                reg |= quint8(bin[i++]);
            else
                reg |= quint32(qint32(qint8(bin[i++])));
            rem += 8;
        }
        const quint32 save = reg;
        reg >>= (rem - 6);
        out.append(digits[reg & 0x3F]);
        reg = save;
        rem -= 6;
    }
    if (rem) {
        const quint32 mask = (1u << rem) - 1;
        // Bareos quirk: only the compatible variant shifts the last digit
        out.append(digits[compatible ? (reg & mask) << (6 - rem) : (reg & mask)]);
    }
    return out;
}

QByteArray hmacAnswer(const QByteArray &challenge, const QString &password, bool compatible)
{
    return bareosBase64(QMessageAuthenticationCode::hash(challenge, keyOf(password),
                                                         QCryptographicHash::Md5),
                        compatible);
}

QByteArray frame(const QByteArray &payload)
{
    QByteArray out(4, '\0');
    qToBigEndian<qint32>(qint32(payload.size()), reinterpret_cast<uchar *>(out.data()));
    return out + payload;
}

/**
 * @brief The Director's side of the console handshake, honest or not
 */
class FakeBnetDirector : public QObject
{
    Q_OBJECT

public:
    enum class Behaviour {
        Honest,             ///< knows the password, completes both halves
        WrongHmac,          ///< accepts the console, answers its challenge wrongly
        BannerInsteadOfOk,  ///< greeting right after the console's HMAC (no mutual half)
        OkInsteadOfHmac,    ///< "1000 OK" with greeting instead of the HMAC
        RejectConsole,      ///< "1999 Authorization failed." for the console's HMAC
        ReflectChallenge,   ///< challenge carrying the console's own name
    };

    FakeBnetDirector(Behaviour behaviour, const QString &directorPassword, QObject *parent = nullptr)
        : QObject(parent), m_behaviour(behaviour), m_password(directorPassword)
    {
    }

    /// Clear text (legacy)
    bool listenPlain()
    {
        auto *server = new QTcpServer(this);
        connect(server, &QTcpServer::pendingConnectionAvailable, this, [this, server]() {
            attach(server->nextPendingConnection());
        });
        m_server = server;
        return server->listen(QHostAddress::LocalHost);
    }

    /// Certificate TLS from the first byte (Bareos 18.2+ immediate TLS)
    bool listenTls(const char *certPem, const char *keyPem)
    {
        auto *server = new QSslServer(this);
        QSslConfiguration ssl = QSslConfiguration::defaultConfiguration();
        ssl.setLocalCertificate(QSslCertificate(QByteArray(certPem), QSsl::Pem));
        ssl.setPrivateKey(QSslKey(QByteArray(keyPem), QSsl::Ec, QSsl::Pem));
        ssl.setPeerVerifyMode(QSslSocket::VerifyNone);
        server->setSslConfiguration(ssl);
        connect(server, &QSslServer::pendingConnectionAvailable, this, [this, server]() {
            attach(server->nextPendingConnection());
        });
        m_server = server;
        return server->listen(QHostAddress::LocalHost);
    }

    quint16 port() const { return m_server->serverPort(); }
    void setTlsPolicy(int ssl) { m_tlsPolicy = ssl; }

    QList<QByteArray> received;     ///< every message from the console
    bool consoleRefusedUs = false;  ///< the console sent "1999 ..."
    bool greetingSent = false;

private:
    void attach(QTcpSocket *socket)
    {
        if (!socket)
            return;
        m_socket = socket;
        connect(socket, &QTcpSocket::readyRead, this, &FakeBnetDirector::onReadyRead);
        // Data may have arrived with the handshake already
        if (socket->bytesAvailable() > 0)
            QMetaObject::invokeMethod(this, &FakeBnetDirector::onReadyRead, Qt::QueuedConnection);
    }

    /// FAKE_BNET_DEBUG=1 shows the conversation
    static bool debug() { return qEnvironmentVariableIsSet("FAKE_BNET_DEBUG"); }

    void send(const QByteArray &payload)
    {
        if (debug())
            qDebug() << "fake Director sends" << payload;
        m_socket->write(frame(payload));
    }

    void onReadyRead()
    {
        m_buffer.append(m_socket->readAll());
        while (m_buffer.size() >= 4) {
            const qint32 len = qFromBigEndian<qint32>(reinterpret_cast<const uchar *>(m_buffer.constData()));
            if (len < 0 || m_buffer.size() < 4 + len)
                return;
            const QByteArray message = m_buffer.mid(4, len);
            m_buffer.remove(0, 4 + len);
            received.append(message);
            if (debug())
                qDebug() << "fake Director receives" << message;
            handle(message);
        }
    }

    void handle(const QByteArray &message)
    {
        switch (m_step++) {
        case 0: { // Hello -> our challenge
            const QByteArray name = m_behaviour == Behaviour::ReflectChallenge
                                        ? ConsoleName.toLatin1()
                                        : QByteArray("R_DIRECTOR::test-dir");
            m_challenge = "<1804289383.1763884800@" + name + ">";
            send("auth cram-md5 " + m_challenge + " ssl=" + QByteArray::number(m_tlsPolicy) + "\n");
            break;
        }
        case 1: { // console's HMAC
            const QByteArray answer = message.trimmed();
            const bool ok = answer == hmacAnswer(m_challenge, m_password, false)
                            || answer == hmacAnswer(m_challenge, m_password, true);
            if (m_behaviour == Behaviour::RejectConsole || (m_behaviour == Behaviour::Honest && !ok)) {
                send("1999 Authorization failed.\n");
            } else if (m_behaviour == Behaviour::BannerInsteadOfOk) {
                send("1000 OK: bareos-dir Version: 25.1.2 (13 November 2026)\n");
                greetingSent = true;
            } else {
                send("1000 OK auth\n");
            }
            break;
        }
        case 2: { // console's challenge -> our HMAC
            static const QRegularExpression rx(QStringLiteral("auth cram-md5c? (<[^>]+>) ssl=(\\d+)"));
            const auto match = rx.match(QString::fromLatin1(message));
            const QByteArray challenge = match.captured(1).toLatin1();
            if (m_behaviour == Behaviour::OkInsteadOfHmac) {
                send("1000 OK: bareos-dir Version: 25.1.2 (13 November 2026)\n");
                greetingSent = true;
            } else if (m_behaviour == Behaviour::WrongHmac) {
                QByteArray wrong = hmacAnswer(challenge, QStringLiteral("guessed"), false);
                wrong.append('\0');
                send(wrong);
            } else {
                QByteArray answer = hmacAnswer(challenge, m_password, false);
                answer.append('\0');   // Bareos sends the terminating NUL
                send(answer);
            }
            break;
        }
        case 3: // console's verdict on our HMAC
            if (message.startsWith("1999")) {
                consoleRefusedUs = true;
            } else if (message.startsWith("1000 OK auth")) {
                send("1000\x1eOK: test-dir Version: 25.1.2 (13 November 2026)");
                send("1002\x1eYou are logged in as: admin");
                greetingSent = true;
            }
            break;
        default:
            break;
        }
    }

    Behaviour m_behaviour;
    QString m_password;
    int m_tlsPolicy = 0;
    QTcpServer *m_server = nullptr;
    QPointer<QTcpSocket> m_socket;
    QByteArray m_buffer;
    QByteArray m_challenge;
    int m_step = 0;
};

/// The console side: one BareosAuth on a QSslSocket and its outcome
struct Outcome
{
    bool started = false;
    bool succeeded = false;
    bool failed = false;
    int failures = 0;
    QString error;
};

struct ConsoleOptions
{
    bool tlsEnable = false;
    bool tlsRequire = false;
    bool verifyPeer = false;
    QString caFile;
    QStringList allowedCn;
    QString host = QStringLiteral("127.0.0.1");
};

Outcome runConsole(quint16 port, const ConsoleOptions &options, const QString &password = Password)
{
    Outcome outcome;
    QSslSocket socket;
    BareosAuth auth(&socket);
    QObject::connect(&auth, &BareosAuth::authenticationSucceeded, [&](const QString &) {
        outcome.succeeded = true;
    });
    QObject::connect(&auth, &BareosAuth::authenticationFailed, [&](const QString &reason) {
        outcome.failed = true;
        ++outcome.failures;
        outcome.error = reason;
    });

    socket.connectToHost(options.host, port);
    if (!socket.waitForConnected(5000)) {
        outcome.error = socket.errorString();
        return outcome;
    }
    if (options.tlsEnable) {
        auth.setCertificateFiles(options.caFile, QString(), QString());
        auth.setAllowedCommonNames(options.allowedCn);
    }
    outcome.started = auth.authenticateDirector(QStringLiteral("test-dir"), ConsoleName, password,
                                                options.tlsEnable, options.tlsRequire,
                                                options.verifyPeer, false);
    if (!outcome.started) {
        outcome.failed = true;
        outcome.error = auth.getErrorMessage();
        return outcome;
    }
    QDeadlineTimer deadline(10000);
    while (!outcome.succeeded && !outcome.failed && !deadline.hasExpired())
        QTest::qWait(10);
    if (!outcome.succeeded && !outcome.failed)
        outcome.error = QStringLiteral("no outcome within 10 s");
    // Late signals must not turn a refusal into a success
    QTest::qWait(100);
    return outcome;
}

} // namespace

class BareosAuthSecurityTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    // P0-6
    void honestDirectorIsAccepted();
    void directorWithoutPasswordIsRefused();
    void wrongDirectorHmacIsRefused();
    void greetingWithoutMutualHalfIsRefused();
    void okInsteadOfHmacIsRefused();
    void rejectedPasswordSaysSo();
    void reflectedChallengeIsRefused();
    void directorRequiringTlsInClearTextIsRefused();
    void localTlsRequirementNeverFallsBackToClearText();
    void pskCipherListKeepsOnlyPskSuites();

    // P0-11
    void certificateForHostIsAccepted();
    void certificateForOtherHostIsRefused();
    void verificationWithoutCaFileIsRefused();
    void unreadableCaFileIsRefused();
    void certificateFromOtherCaIsRefused();
    void allowedCommonNameReplacesHostCheck();
    void commonNameNotInAllowedListIsRefused();

    // Optional, not in CI
    void simulatorHandshake_data();
    void simulatorHandshake();

private:
    QString writeFile(const QString &name, const QByteArray &content);
    QTemporaryDir m_dir;
    QString m_caFile;
};

void BareosAuthSecurityTest::initTestCase()
{
    QVERIFY(m_dir.isValid());
    m_caFile = writeFile(QStringLiteral("ca.pem"), BTlsTestCerts::CaCert);
}

QString BareosAuthSecurityTest::writeFile(const QString &name, const QByteArray &content)
{
    const QString path = m_dir.filePath(name);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
        return QString();
    file.write(content);
    return path;
}

// ---------------------------------------------------------------------------
// P0-6: the Director has to prove that it knows the password
// ---------------------------------------------------------------------------

void BareosAuthSecurityTest::honestDirectorIsAccepted()
{
    FakeBnetDirector director(FakeBnetDirector::Behaviour::Honest, Password);
    QVERIFY(director.listenPlain());

    const Outcome outcome = runConsole(director.port(), ConsoleOptions());
    QVERIFY2(outcome.succeeded, qPrintable(outcome.error));
    QVERIFY(!outcome.failed);
    QVERIFY(director.greetingSent);
    QVERIFY(!director.consoleRefusedUs);
}

void BareosAuthSecurityTest::directorWithoutPasswordIsRefused()
{
    // The impostor accepts any HMAC and answers the console's challenge
    // with a password it guessed
    FakeBnetDirector director(FakeBnetDirector::Behaviour::WrongHmac, QStringLiteral("unknown"));
    QVERIFY(director.listenPlain());

    const Outcome outcome = runConsole(director.port(), ConsoleOptions());
    QVERIFY(!outcome.succeeded);
    QVERIFY(outcome.failed);
    QCOMPARE(outcome.failures, 1);
    QVERIFY2(outcome.error.contains(QLatin1String("could not prove that it knows the console password")),
             qPrintable(outcome.error));
    QTRY_VERIFY(director.consoleRefusedUs);   // "1999 Authorization failed." as in Bareos
}

void BareosAuthSecurityTest::wrongDirectorHmacIsRefused()
{
    // Knows the password for the first half, answers the second wrongly
    FakeBnetDirector director(FakeBnetDirector::Behaviour::WrongHmac, Password);
    QVERIFY(director.listenPlain());

    const Outcome outcome = runConsole(director.port(), ConsoleOptions());
    QVERIFY(!outcome.succeeded);
    QVERIFY(outcome.error.contains(QLatin1String("CRAM-MD5 challenge is wrong")));
}

void BareosAuthSecurityTest::greetingWithoutMutualHalfIsRefused()
{
    // Before P0-6 any message with "bareos-dir" or "Version" was success
    FakeBnetDirector director(FakeBnetDirector::Behaviour::BannerInsteadOfOk, QStringLiteral("unknown"));
    QVERIFY(director.listenPlain());

    const Outcome outcome = runConsole(director.port(), ConsoleOptions());
    QVERIFY(!outcome.succeeded);
    QVERIFY(outcome.failed);
    QVERIFY2(outcome.error.contains(QLatin1String("did not complete the CRAM-MD5 handshake")),
             qPrintable(outcome.error));
}

void BareosAuthSecurityTest::okInsteadOfHmacIsRefused()
{
    // Before P0-6 "1000 OK" in place of the HMAC skipped the mutual half
    FakeBnetDirector director(FakeBnetDirector::Behaviour::OkInsteadOfHmac, QStringLiteral("unknown"));
    QVERIFY(director.listenPlain());

    const Outcome outcome = runConsole(director.port(), ConsoleOptions());
    QVERIFY(!outcome.succeeded);
    QVERIFY(outcome.error.contains(QLatin1String("could not prove")));
}

void BareosAuthSecurityTest::rejectedPasswordSaysSo()
{
    FakeBnetDirector director(FakeBnetDirector::Behaviour::Honest, Password);
    QVERIFY(director.listenPlain());

    const Outcome outcome = runConsole(director.port(), ConsoleOptions(), QStringLiteral("wrong"));
    QVERIFY(!outcome.succeeded);
    QVERIFY2(outcome.error.contains(QLatin1String("rejected the password")), qPrintable(outcome.error));
}

void BareosAuthSecurityTest::reflectedChallengeIsRefused()
{
    FakeBnetDirector director(FakeBnetDirector::Behaviour::ReflectChallenge, Password);
    QVERIFY(director.listenPlain());

    const Outcome outcome = runConsole(director.port(), ConsoleOptions());
    QVERIFY(!outcome.succeeded);
    QVERIFY(outcome.error.contains(QLatin1String("reflection")));
    // Our HMAC never left: only the Hello was sent
    QCOMPARE(director.received.size(), 1);
}

void BareosAuthSecurityTest::directorRequiringTlsInClearTextIsRefused()
{
    FakeBnetDirector director(FakeBnetDirector::Behaviour::Honest, Password);
    director.setTlsPolicy(2);
    QVERIFY(director.listenPlain());

    const Outcome outcome = runConsole(director.port(), ConsoleOptions());
    QVERIFY(!outcome.succeeded);
    QVERIFY2(outcome.error.contains(QLatin1String("The Director requires TLS")), qPrintable(outcome.error));
    QCOMPARE(director.received.size(), 1);   // no HMAC in clear text
}

void BareosAuthSecurityTest::localTlsRequirementNeverFallsBackToClearText()
{
    FakeBnetDirector director(FakeBnetDirector::Behaviour::Honest, Password);
    QVERIFY(director.listenPlain());

    ConsoleOptions options;
    options.tlsRequire = true;   // but neither PSK nor certificate TLS
    const Outcome outcome = runConsole(director.port(), options);
    QVERIFY(!outcome.started);
    QVERIFY(outcome.error.contains(QLatin1String("TLS is required")));
    QTest::qWait(100);
    QVERIFY(director.received.isEmpty());   // not even the Hello
}

void BareosAuthSecurityTest::pskCipherListKeepsOnlyPskSuites()
{
    QStringList dropped;
    const QStringList kept = BareosAuth::pskOnlyCipherNames(
        QStringLiteral("PSK-AES256-GCM-SHA384: ECDHE-RSA-AES256-GCM-SHA384:AES128-SHA:PSK-AES128-CBC-SHA::"),
        &dropped);
    QCOMPARE(kept, (QStringList{QStringLiteral("PSK-AES256-GCM-SHA384"), QStringLiteral("PSK-AES128-CBC-SHA")}));
    QCOMPARE(dropped, (QStringList{QStringLiteral("ECDHE-RSA-AES256-GCM-SHA384"), QStringLiteral("AES128-SHA")}));
    QVERIFY(BareosAuth::pskOnlyCipherNames(QStringLiteral("ECDHE-PSK-AES128-CBC-SHA256")).isEmpty());
}

// ---------------------------------------------------------------------------
// P0-11: certificate TLS checks CA and name
// ---------------------------------------------------------------------------

void BareosAuthSecurityTest::certificateForHostIsAccepted()
{
    FakeBnetDirector director(FakeBnetDirector::Behaviour::Honest, Password);
    QVERIFY(director.listenTls(BTlsTestCerts::LocalCert, BTlsTestCerts::LocalKey));

    ConsoleOptions options;
    options.tlsEnable = options.tlsRequire = options.verifyPeer = true;
    options.caFile = m_caFile;
    const Outcome outcome = runConsole(director.port(), options);
    QVERIFY2(outcome.succeeded, qPrintable(outcome.error));
}

void BareosAuthSecurityTest::certificateForOtherHostIsRefused()
{
    // Signed by the trusted CA, but for other.example: before P0-11 the
    // host name mismatch was ignored
    FakeBnetDirector director(FakeBnetDirector::Behaviour::Honest, Password);
    QVERIFY(director.listenTls(BTlsTestCerts::OtherCert, BTlsTestCerts::OtherKey));

    ConsoleOptions options;
    options.tlsEnable = options.tlsRequire = options.verifyPeer = true;
    options.caFile = m_caFile;
    const Outcome outcome = runConsole(director.port(), options);
    QVERIFY(!outcome.succeeded);
    QVERIFY2(outcome.error.contains(QLatin1String("not issued for the host name \"127.0.0.1\"")),
             qPrintable(outcome.error));
    QVERIFY(outcome.error.contains(QLatin1String("other.example")));
    QVERIFY(director.received.isEmpty());   // not even the Hello
}

void BareosAuthSecurityTest::verificationWithoutCaFileIsRefused()
{
    FakeBnetDirector director(FakeBnetDirector::Behaviour::Honest, Password);
    QVERIFY(director.listenTls(BTlsTestCerts::LocalCert, BTlsTestCerts::LocalKey));

    ConsoleOptions options;
    options.tlsEnable = options.tlsRequire = options.verifyPeer = true;
    const Outcome outcome = runConsole(director.port(), options);
    QVERIFY(!outcome.started);
    QVERIFY(outcome.error.contains(QLatin1String("no CA certificate file is set")));
}

void BareosAuthSecurityTest::unreadableCaFileIsRefused()
{
    FakeBnetDirector director(FakeBnetDirector::Behaviour::Honest, Password);
    QVERIFY(director.listenTls(BTlsTestCerts::LocalCert, BTlsTestCerts::LocalKey));

    ConsoleOptions options;
    options.tlsEnable = options.tlsRequire = options.verifyPeer = true;
    options.caFile = writeFile(QStringLiteral("garbage.pem"), "not a certificate\n");
    Outcome outcome = runConsole(director.port(), options);
    QVERIFY(!outcome.started);
    QVERIFY(outcome.error.contains(QLatin1String("cannot be read or contains no certificate")));

    options.caFile = m_dir.filePath(QStringLiteral("missing.pem"));
    outcome = runConsole(director.port(), options);
    QVERIFY(!outcome.started);
    QVERIFY(outcome.error.contains(QLatin1String("missing.pem")));
}

void BareosAuthSecurityTest::certificateFromOtherCaIsRefused()
{
    // Self-signed, not from the CA of the profile; the system store is not used
    FakeBnetDirector director(FakeBnetDirector::Behaviour::Honest, Password);
    QVERIFY(director.listenTls(BTlsTestCerts::PinACert, BTlsTestCerts::PinAKey));

    ConsoleOptions options;
    options.tlsEnable = options.tlsRequire = options.verifyPeer = true;
    options.caFile = m_caFile;
    const Outcome outcome = runConsole(director.port(), options);
    QVERIFY(!outcome.succeeded);
    QVERIFY2(outcome.error.contains(QLatin1String("was not accepted")), qPrintable(outcome.error));
}

void BareosAuthSecurityTest::allowedCommonNameReplacesHostCheck()
{
    // Bareos style: the certificate names the daemon, not the host
    FakeBnetDirector director(FakeBnetDirector::Behaviour::Honest, Password);
    QVERIFY(director.listenTls(BTlsTestCerts::DirNameCert, BTlsTestCerts::DirNameKey));

    ConsoleOptions options;
    options.tlsEnable = options.tlsRequire = options.verifyPeer = true;
    options.caFile = m_caFile;
    Outcome outcome = runConsole(director.port(), options);
    QVERIFY(!outcome.succeeded);   // without the list: host name check
    QVERIFY(outcome.error.contains(QLatin1String("bareos-dir")));

    FakeBnetDirector second(FakeBnetDirector::Behaviour::Honest, Password);
    QVERIFY(second.listenTls(BTlsTestCerts::DirNameCert, BTlsTestCerts::DirNameKey));
    options.allowedCn = {QStringLiteral("bareos-dir")};
    outcome = runConsole(second.port(), options);
    QVERIFY2(outcome.succeeded, qPrintable(outcome.error));
}

void BareosAuthSecurityTest::commonNameNotInAllowedListIsRefused()
{
    // Host name matches, but the list decides (Bareos BnetTlsClient())
    FakeBnetDirector director(FakeBnetDirector::Behaviour::Honest, Password);
    QVERIFY(director.listenTls(BTlsTestCerts::LocalCert, BTlsTestCerts::LocalKey));

    ConsoleOptions options;
    options.tlsEnable = options.tlsRequire = options.verifyPeer = true;
    options.caFile = m_caFile;
    options.allowedCn = {QStringLiteral("bareos-dir")};
    const Outcome outcome = runConsole(director.port(), options);
    QVERIFY(!outcome.succeeded);
    QVERIFY2(outcome.error.contains(QLatin1String("not in the TLS Allowed CN list")), qPrintable(outcome.error));
    QVERIFY(director.received.isEmpty());
}

void BareosAuthSecurityTest::simulatorHandshake_data()
{
    QTest::addColumn<bool>("psk");
    QTest::newRow("clear text") << false;
    QTest::newRow("TLS-PSK") << true;
}

void BareosAuthSecurityTest::simulatorHandshake()
{
    // A running onesimus-sim or a test Director, e.g.
    // ONESIMUS_TEST_BNET=127.0.0.1:21001:admin:demo
    const QStringList target = qEnvironmentVariable("ONESIMUS_TEST_BNET").split(QLatin1Char(':'));
    if (target.size() != 4)
        QSKIP("ONESIMUS_TEST_BNET=host:port:console:password not set");
    QFETCH(bool, psk);

    for (const QString &password : {target[3], QStringLiteral("wrong-password")}) {
        QSslSocket socket;
        BareosAuth auth(&socket);
        bool succeeded = false;
        QString error;
        QObject::connect(&auth, &BareosAuth::authenticationSucceeded, [&](const QString &) { succeeded = true; });
        QObject::connect(&auth, &BareosAuth::authenticationFailed, [&](const QString &e) { error = e; });
        socket.connectToHost(target[0], quint16(target[1].toInt()));
        QVERIFY(socket.waitForConnected(5000));
        QVERIFY(auth.authenticateDirector(QStringLiteral("demo-dir"), target[2], password, psk, psk, false, psk));
        QTRY_VERIFY_WITH_TIMEOUT(succeeded || !error.isEmpty(), 10000);
        if (password == target[3]) {
            QVERIFY2(succeeded, qPrintable(error));
            QCOMPARE(socket.isEncrypted(), psk);
            qInfo() << "greeting:" << auth.directorBanner();
        } else {
            QVERIFY(!succeeded);
            qInfo() << "wrong password:" << error;
        }
    }
}

QTEST_MAIN(BareosAuthSecurityTest)
#include "bareosauth_security_test.moc"
