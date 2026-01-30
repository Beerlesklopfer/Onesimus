/**
 * @file director_test.cpp
 * @brief Test program for BareosDirector class and state machine
 *
 * Tests connection, authentication, commands, and state transitions.
 * Monitors loading of all Bareos resource types during initialization.
 *
 * State Machine Flow:
 *   Disconnected -> Connecting -> Authenticating -> SettingApiMode
 *   -> LoadingResources -> Ready
 *
 * Resources loaded during initialization:
 *   - Catalog    (.catalogs)   - Database catalogs
 *   - Client     (.clients)    - Backup clients (File Daemons)
 *   - Fileset    (.filesets)   - File set definitions
 *   - Job        (.jobs)       - Job definitions
 *   - Pool       (.pools)      - Storage pools
 *   - Schedule   (.schedule)   - Backup schedules
 *   - Storage    (.storages)   - Storage daemons
 *   - Level      (.levels)     - Backup levels (F, I, D)
 *
 * Usage: ./director_test [options] [command]
 *
 * Commands:
 *   connect      Connect and authenticate (default)
 *   status       Connect, auth, then send 'status director'
 *   jobs         Connect, auth, then send '.jobs'
 *   clients      Connect, auth, then send '.clients'
 *   interactive  Connect, auth, then enter interactive mode
 *
 * Options:
 *   --host <host>         Director host (default: localhost)
 *   -p, --port <port>     Director port (default: 9101)
 *   -d, --director <name> Director name (default: bareos-dir)
 *   -c, --console <name>  Console name (default: onesimus)
 *   -P, --password <pwd>  Password (required)
 *   --legacy              Use legacy mode (no TLS)
 *   --psk                 Use TLS-PSK mode
 *   --verbose             Verbose output
 *   -h, --help            Show help
 *
 * @see BareosDirector
 * @see BareosDirector::ResourceType
 * @see BareosDirector::ConnectionState
 */

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QTimer>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "bareosdirector.h"

/**
 * @class DirectorTester
 * @brief Test harness for BareosDirector state machine
 */
class DirectorTester : public QObject
{
    Q_OBJECT

public:
    /**
     * @struct Config
     * @brief Test configuration
     */
    struct Config {
        QString host = "localhost";
        int port = 9101;
        QString directorName = "bareos-dir";
        QString consoleName = "onesimus";
        QString password;
        bool legacy = true;
        bool psk = false;
        bool verbose = false;
        QString command = "connect";
    };

    /**
     * @brief Construct DirectorTester
     * @param config Test configuration
     * @param parent Parent QObject
     */
    explicit DirectorTester(const Config &config, QObject *parent = nullptr)
        : QObject(parent)
        , m_config(config)
    {
        m_director = new BareosDirector(this);
        m_director->initialize();

        // Connect signals
        connect(m_director, &BareosDirector::connectionStateChanged,
                this, &DirectorTester::onStateChanged);
        connect(m_director, &BareosDirector::disconnected,
                this, &DirectorTester::onDisconnected);
        connect(m_director, &BareosDirector::protocolError,
                this, &DirectorTester::onProtocolError);
        connect(m_director, &BareosDirector::authentificationSucceeded,
                this, &DirectorTester::onAuthResult);
        connect(m_director, &BareosDirector::jsonResponse,
                this, &DirectorTester::onJsonResponse);
        connect(m_director, &BareosDirector::commandResponse,
                this, &DirectorTester::onCommandResponse);
        connect(m_director, &BareosDirector::commandError,
                this, &DirectorTester::onCommandError);
        connect(m_director, &BareosDirector::resourceLoaded,
                this, &DirectorTester::onResourceLoaded);
        connect(m_director, &BareosDirector::allResourcesLoaded,
                this, &DirectorTester::onAllResourcesLoaded);

        // Configure TLS
        BareosDirector::TLSConfig tlsConfig;
        tlsConfig.tlsEnable = !m_config.legacy;
        tlsConfig.tlsRequire = false;
        tlsConfig.tlsVerifyPeer = false;
        tlsConfig.tlsPSKEnable = m_config.psk;
        m_director->setTLSConfig(tlsConfig);
    }

    ~DirectorTester() override = default;

    /**
     * @brief Start the test
     */
    void start()
    {
        printHeader();
        m_director->connect(m_config.host, m_config.port,
                           m_config.directorName, m_config.consoleName,
                           m_config.password);
    }

private slots:
    void onStateChanged(BareosDirector::ConnectionState oldState,
                        BareosDirector::ConnectionState newState)
    {
        if (m_config.verbose) {
            qDebug().noquote() << QString("  [STATE] %1 -> %2")
                .arg(stateToString(oldState), stateToString(newState));
        }

        // Track state for final summary
        if (newState == BareosDirector::Ready && !m_reachedReady) {
            m_reachedReady = true;
            QTimer::singleShot(100, this, &DirectorTester::executeCommand);
        }
    }

    void onAuthResult(bool success, const QString &message)
    {
        m_authSuccess = success;
        m_authMessage = message;

        if (!success) {
            printResult(false, "Authentication failed: " + message);
            qApp->exit(1);
        }
    }

    void onDisconnected()
    {
        if (!m_testComplete) {
            printResult(false, "Disconnected unexpectedly");
            qApp->exit(1);
        }
    }

    void onProtocolError(const QString &error)
    {
        printResult(false, "Protocol error: " + error);
        qApp->exit(1);
    }

    void onResourceLoaded(BareosDirector::ResourceType type)
    {
        m_resourcesLoaded++;
        if (m_config.verbose) {
            qDebug().noquote() << QString("  [RESOURCE] %1 loaded (%2/7)")
                .arg(resourceToString(type))
                .arg(m_resourcesLoaded);
        }
    }

    void onAllResourcesLoaded()
    {
        m_allResourcesLoaded = true;
    }

    void onJsonResponse(const QString &command, const QString &json)
    {
        if (m_config.verbose) {
            qDebug().noquote() << QString("  [JSON] %1 (%2 bytes)")
                .arg(command).arg(json.size());
        }

        if (m_pendingCommand == "jobs" || m_pendingCommand == "clients") {
            parseAndShowSummary(json);
            finishTest(true);
        } else if (m_pendingCommand == "status") {
            // In API mode 2, status director also returns JSON
            qDebug().noquote() << "\n--- Status Director (JSON) ---";
            QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
            if (doc.isObject()) {
                QJsonObject root = doc.object();
                QJsonObject result = root["result"].toObject();
                for (const QString &key : result.keys()) {
                    qDebug().noquote() << QString("  %1: %2")
                        .arg(key, result[key].toVariant().toString().left(100));
                }
            }
            finishTest(true);
        }
    }

    void onCommandResponse(const QString &command, const QString &response)
    {
        Q_UNUSED(command)
        // In API mode 2, most commands return JSON, not text
        // This is mainly for legacy/non-API mode responses
        if (m_config.verbose) {
            qDebug().noquote() << QString("  [TEXT] %1 (%2 bytes)")
                .arg(command).arg(response.size());
        }
    }

    void onCommandError(const QString &command, const QString &error)
    {
        Q_UNUSED(command)
        printResult(false, "Command error: " + error);
        qApp->exit(1);
    }

    void executeCommand()
    {
        if (m_config.command == "connect") {
            finishTest(true);
        } else if (m_config.command == "status") {
            m_pendingCommand = "status";
            m_director->doSendCommand(BareosDirector::Command::StatusDirector, "");
        } else if (m_config.command == "jobs") {
            m_pendingCommand = "jobs";
            m_director->doSendCommand(BareosDirector::Command::ListJobs, "");
        } else if (m_config.command == "clients") {
            m_pendingCommand = "clients";
            m_director->doSendCommand(BareosDirector::Command::ListClients, "");
        } else if (m_config.command == "interactive") {
            startInteractiveMode();
        }
    }

    void startInteractiveMode()
    {
        qDebug() << "\n=== Interactive Mode ===";
        qDebug() << "Commands: state, quit, help";
        m_interactive = true;
        promptCommand();
    }

    void promptCommand()
    {
        QTextStream in(stdin);
        QTextStream out(stdout);

        out << "bareos> " << Qt::flush;
        QString line = in.readLine().trimmed();

        if (line.isEmpty()) {
            QTimer::singleShot(50, this, &DirectorTester::promptCommand);
            return;
        }

        if (line == "quit" || line == "exit") {
            finishTest(true);
            return;
        }

        if (line == "state") {
            qDebug().noquote() << "State:" << stateToString(m_director->connectionState());
        } else if (line == "help") {
            qDebug() << "  state  - Show connection state";
            qDebug() << "  quit   - Exit";
        } else {
            qDebug() << "Unknown command. Type 'help' for options.";
        }

        QTimer::singleShot(50, this, &DirectorTester::promptCommand);
    }

private:
    void printHeader()
    {
        qDebug().noquote() << "========================================";
        qDebug().noquote() << "BareosDirector Test";
        qDebug().noquote() << "========================================";
        qDebug().noquote() << QString("Host:     %1:%2").arg(m_config.host).arg(m_config.port);
        qDebug().noquote() << QString("Director: %1").arg(m_config.directorName);
        qDebug().noquote() << QString("Console:  %1").arg(m_config.consoleName);
        qDebug().noquote() << QString("Mode:     %1").arg(m_config.legacy ? "Legacy" : (m_config.psk ? "PSK" : "TLS"));
        qDebug().noquote() << QString("Command:  %1").arg(m_config.command);
        qDebug().noquote() << "========================================";
    }

    void printResult(bool success, const QString &message = QString())
    {
        qDebug() << "";
        qDebug().noquote() << "========================================";
        if (success) {
            qDebug().noquote() << "RESULT: PASSED";
            qDebug().noquote() << QString("  Auth:      OK (%1)").arg(m_authMessage);
            qDebug().noquote() << QString("  Resources: %1 loaded").arg(m_resourcesLoaded);
            qDebug().noquote() << QString("  State:     %1").arg(stateToString(m_director->connectionState()));
        } else {
            qDebug().noquote() << "RESULT: FAILED";
            if (!message.isEmpty()) {
                qDebug().noquote() << QString("  Error: %1").arg(message);
            }
        }
        qDebug().noquote() << "========================================";
    }

    void finishTest(bool success)
    {
        m_testComplete = true;
        printResult(success);
        QTimer::singleShot(100, qApp, &QCoreApplication::quit);
    }

    void parseAndShowSummary(const QString &json)
    {
        QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
        if (!doc.isObject()) return;

        QJsonObject root = doc.object();
        QJsonObject result = root["result"].toObject();

        qDebug() << "";
        for (const QString &key : result.keys()) {
            if (result[key].isArray()) {
                qDebug().noquote() << QString("  %1: %2 items")
                    .arg(key).arg(result[key].toArray().size());
            }
        }
    }

    static QString stateToString(BareosDirector::ConnectionState state)
    {
        switch (state) {
        case BareosDirector::Disconnected:     return "Disconnected";
        case BareosDirector::Connecting:       return "Connecting";
        case BareosDirector::Authenticating:   return "Authenticating";
        case BareosDirector::SettingApiMode:   return "SettingApiMode";
        case BareosDirector::LoadingResources: return "LoadingResources";
        case BareosDirector::Ready:            return "Ready";
        case BareosDirector::Error:            return "Error";
        default:                               return "Unknown";
        }
    }

    static QString resourceToString(BareosDirector::ResourceType type)
    {
        switch (type) {
        case BareosDirector::ResourceType::Catalog:   return "Catalog";
        case BareosDirector::ResourceType::Client:    return "Client";
        case BareosDirector::ResourceType::Console:   return "Console";
        case BareosDirector::ResourceType::Director:  return "Director";
        case BareosDirector::ResourceType::Fileset:   return "Fileset";
        case BareosDirector::ResourceType::Job:       return "Job";
        case BareosDirector::ResourceType::JobDefs:   return "JobDefs";
        case BareosDirector::ResourceType::Messages:  return "Messages";
        case BareosDirector::ResourceType::Pool:      return "Pool";
        case BareosDirector::ResourceType::Profile:   return "Profile";
        case BareosDirector::ResourceType::Schedule:  return "Schedule";
        case BareosDirector::ResourceType::Storage:   return "Storage";
        case BareosDirector::ResourceType::Level:     return "Level";
        default:                                      return "Unknown";
        }
    }

    Config m_config;
    BareosDirector *m_director = nullptr;
    QString m_pendingCommand;
    QString m_authMessage;
    int m_resourcesLoaded = 0;
    bool m_authSuccess = false;
    bool m_allResourcesLoaded = false;
    bool m_reachedReady = false;
    bool m_testComplete = false;
    bool m_interactive = false;
};

#include "director_test.moc"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("director_test");
    app.setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("BareosDirector state machine test");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption({"host", "Director host", "host", "localhost"});
    parser.addOption({{"p", "port"}, "Director port", "port", "9101"});
    parser.addOption({{"d", "director"}, "Director name", "name", "bareos-dir"});
    parser.addOption({{"c", "console"}, "Console name", "name", "onesimus"});
    parser.addOption({{"P", "password"}, "Password", "password"});
    parser.addOption({"legacy", "Legacy mode (no TLS)"});
    parser.addOption({"psk", "TLS-PSK mode"});
    parser.addOption({"verbose", "Verbose output"});

    parser.addPositionalArgument("command",
        "Test command: connect, status, jobs, clients, interactive",
        "[command]");

    parser.process(app);

    DirectorTester::Config config;
    config.host = parser.value("host");
    config.port = parser.value("port").toInt();
    config.directorName = parser.value("director");
    config.consoleName = parser.value("console");
    config.password = parser.value("password");
    config.legacy = parser.isSet("legacy") || !parser.isSet("psk");
    config.psk = parser.isSet("psk");
    config.verbose = parser.isSet("verbose");

    QStringList args = parser.positionalArguments();
    if (!args.isEmpty()) {
        config.command = args.first().toLower();
    }

    if (config.password.isEmpty()) {
        qCritical() << "Error: Password required (-P/--password)";
        return 1;
    }

    QStringList validCommands = {"connect", "status", "jobs", "clients", "interactive"};
    if (!validCommands.contains(config.command)) {
        qCritical() << "Error: Invalid command:" << config.command;
        qCritical() << "Valid:" << validCommands.join(", ");
        return 1;
    }

    DirectorTester tester(config);
    tester.start();

    QTimer::singleShot(30000, &app, []() {
        qCritical() << "\nTimeout after 30 seconds";
        qApp->exit(2);
    });

    return app.exec();
}
