/**
 * @file bareosauth_test.cpp
 * @brief Test program for BareosAuth class with all authentication modes
 *
 * Usage: ./bareosauth_test [options]
 *
 * Options:
 *   --host <host>            Director host (default: localhost)
 *   -p, --port <port>        Director port (default: 9101)
 *   -d, --director <name>    Director name (default: bareos-dir)
 *   -c, --console <name>     Console name (default: onesimus)
 *   -P, --password <pwd>     Password (required)
 *   -m, --mode <mode>        Auth mode: legacy, psk, cert (default: legacy)
 *   --ca <file>              CA certificate file (for cert mode)
 *   --cert <file>            Client certificate file (for cert mode)
 *   --key <file>             Client key file (for cert mode)
 *   --verbose                Verbose output
 *   -h, --help               Show this help
 *
 * Examples:
 *   ./bareosauth_test -m legacy -c onesimus-legacy -P mypassword
 *   ./bareosauth_test -m psk -c onesimus-psk -P mypassword
 *   ./bareosauth_test -m cert -c onesimus-cert -P mypassword --ca /etc/bareos/ssl/bareos-ca.pem
 */

#include <QCoreApplication>
#include <QSslSocket>
#include <QCommandLineParser>
#include <QDebug>
#include <QTimer>
#include <QFile>

#include "bareosauth.h"

class AuthTester : public QObject
{
    Q_OBJECT

public:
    struct Config {
        QString host = "localhost";
        int port = 9101;
        QString directorName = "bareos-dir";
        QString consoleName = "onesimus";
        QString password;
        QString mode = "legacy";
        QString caFile;
        QString certFile;
        QString keyFile;
        bool verbose = false;
    };

    AuthTester(const Config &config, QObject *parent = nullptr)
        : QObject(parent)
        , m_config(config)
    {
        m_socket = new QSslSocket(this);

        connect(m_socket, &QSslSocket::connected, this, &AuthTester::onConnected);
        connect(m_socket, &QSslSocket::disconnected, this, &AuthTester::onDisconnected);
        connect(m_socket, &QSslSocket::errorOccurred, this, &AuthTester::onError);
    }

    void start()
    {
        qDebug() << "========================================";
        qDebug() << "BAREOSAUTH CLASS TEST";
        qDebug() << "========================================";
        qDebug() << "Host:" << m_config.host;
        qDebug() << "Port:" << m_config.port;
        qDebug() << "Director:" << m_config.directorName;
        qDebug() << "Console:" << m_config.consoleName;
        qDebug() << "Mode:" << m_config.mode;
        if (m_config.mode == "cert") {
            qDebug() << "CA File:" << m_config.caFile;
            qDebug() << "Cert File:" << m_config.certFile;
            qDebug() << "Key File:" << m_config.keyFile;
        }
        qDebug() << "========================================";

        qDebug() << "\n>>> Connecting to" << m_config.host << ":" << m_config.port;
        m_socket->connectToHost(m_config.host, m_config.port);
    }

private slots:
    void onConnected()
    {
        qDebug() << "\n>>> TCP Connected!";
        qDebug() << ">>> Starting authentication with BareosAuth class...";

        m_auth = new BareosAuth(m_socket, this);

        connect(m_auth, &BareosAuth::authenticationSucceeded, this, [this]() {
            qDebug() << ">>> AUTH SUCCESS!";
            qDebug() << ">>> Director Version:" << m_auth->getDirectorVersionString();
            qApp->exit(0);
        });

        connect(m_auth, &BareosAuth::authenticationFailed, this, [this](const QString &error) {
            qDebug() << "!!! AUTH FAILED:" << error;
            qApp->exit(1);
        });

        if (m_config.verbose) {
            connect(m_auth, &BareosAuth::statusMessage, this, [](const QString &msg) {
                qDebug() << ">>> Status:" << msg;
            });
        }

        // Determine TLS settings based on mode
        bool tlsEnable = false;
        bool tlsRequire = false;
        bool tlsVerifyPeer = false;
        bool tlsPSKEnable = false;

        if (m_config.mode == "legacy") {
            tlsEnable = false;
            tlsRequire = false;
            tlsPSKEnable = false;
        } else if (m_config.mode == "psk") {
            tlsEnable = true;
            tlsRequire = true;
            tlsPSKEnable = true;
        } else if (m_config.mode == "cert") {
            tlsEnable = true;
            tlsRequire = true;
            tlsVerifyPeer = !m_config.caFile.isEmpty();
            tlsPSKEnable = false;

            // Load certificates
            m_auth->setCertificateFiles(m_config.caFile, m_config.certFile, m_config.keyFile);
        }

        if (m_config.verbose) {
            qDebug() << ">>> Calling authenticateDirector()";
            qDebug() << ">>>   tlsEnable:" << tlsEnable;
            qDebug() << ">>>   tlsRequire:" << tlsRequire;
            qDebug() << ">>>   tlsVerifyPeer:" << tlsVerifyPeer;
            qDebug() << ">>>   tlsPSKEnable:" << tlsPSKEnable;
        }

        bool started = m_auth->authenticateDirector(
            m_config.directorName,
            m_config.consoleName,
            m_config.password,
            tlsEnable,
            tlsRequire,
            tlsVerifyPeer,
            tlsPSKEnable
        );

        if (!started) {
            qDebug() << "!!! authenticateDirector() returned false:" << m_auth->getErrorMessage();
            qApp->exit(1);
        }
    }

    void onDisconnected()
    {
        qDebug() << "\n!!! Disconnected";
        qApp->exit(1);
    }

    void onError(QAbstractSocket::SocketError error)
    {
        qDebug() << "\n!!! Socket error:" << error << "-" << m_socket->errorString();
        qApp->exit(1);
    }

private:
    Config m_config;
    QSslSocket *m_socket;
    BareosAuth *m_auth = nullptr;
};

#include "bareosauth_test.moc"

void printUsage(const QString &appName)
{
    qDebug() << "Usage:" << appName << "[options]";
    qDebug() << "";
    qDebug() << "Options:";
    qDebug() << "  --host <host>            Director host (default: localhost)";
    qDebug() << "  -p, --port <port>        Director port (default: 9101)";
    qDebug() << "  -d, --director <name>    Director name (default: bareos-dir)";
    qDebug() << "  -c, --console <name>     Console name (default: onesimus)";
    qDebug() << "  -P, --password <pwd>     Password (required)";
    qDebug() << "  -m, --mode <mode>        Auth mode: legacy, psk, cert (default: legacy)";
    qDebug() << "  --ca <file>              CA certificate file (for cert mode)";
    qDebug() << "  --cert <file>            Client certificate file (for cert mode)";
    qDebug() << "  --key <file>             Client key file (for cert mode)";
    qDebug() << "  --verbose                Verbose output";
    qDebug() << "  -h, --help               Show this help";
    qDebug() << "";
    qDebug() << "Examples:";
    qDebug() << "  " << appName << " -m legacy -c onesimus -P mypassword";
    qDebug() << "  " << appName << " -m psk -c onesimus-psk -P mypassword";
    qDebug() << "  " << appName << " -m cert -c onesimus-cert -P mypassword --ca /etc/bareos/ssl/bareos-ca.pem";
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("bareosauth_test");
    app.setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("BareosAuth class test program");

    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption({"host", "Director host", "host", "localhost"});
    parser.addOption({{"p", "port"}, "Director port", "port", "9101"});
    parser.addOption({{"d", "director"}, "Director name", "name", "bareos-dir"});
    parser.addOption({{"c", "console"}, "Console name", "name", "onesimus"});
    parser.addOption({{"P", "password"}, "Password", "password"});
    parser.addOption({{"m", "mode"}, "Auth mode: legacy, psk, cert", "mode", "legacy"});
    parser.addOption({"ca", "CA certificate file", "file"});
    parser.addOption({"cert", "Client certificate file", "file"});
    parser.addOption({"key", "Client key file", "file"});
    parser.addOption({"verbose", "Verbose output"});

    parser.process(app);

    AuthTester::Config config;
    config.host = parser.value("host");
    config.port = parser.value("port").toInt();
    config.directorName = parser.value("director");
    config.consoleName = parser.value("console");
    config.password = parser.value("password");
    config.mode = parser.value("mode").toLower();
    config.caFile = parser.value("ca");
    config.certFile = parser.value("cert");
    config.keyFile = parser.value("key");
    config.verbose = parser.isSet("verbose");

    // Debug output
    if (config.verbose) {
        qDebug() << "Config:";
        qDebug() << "  host:" << config.host;
        qDebug() << "  port:" << config.port;
        qDebug() << "  director:" << config.directorName;
        qDebug() << "  console:" << config.consoleName;
        qDebug() << "  mode:" << config.mode;
    }

    // Validate
    if (config.password.isEmpty()) {
        qDebug() << "Error: Password is required (-P/--password)";
        printUsage(argv[0]);
        return 1;
    }

    if (config.mode != "legacy" && config.mode != "psk" && config.mode != "cert") {
        qDebug() << "Error: Invalid mode. Use: legacy, psk, or cert";
        return 1;
    }

    if (config.mode == "cert" && config.caFile.isEmpty()) {
        qDebug() << "Warning: Certificate mode without CA file - peer verification may fail";
    }

    AuthTester tester(config);
    tester.start();

    // Timeout after 30 seconds
    QTimer::singleShot(30000, &app, []() {
        qDebug() << "!!! Timeout - authentication took too long";
        qApp->exit(2);
    });

    return app.exec();
}
