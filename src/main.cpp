#include "bmainwindow.h"
#include "btranslations.h"
#include "bsettings.h"
#include "blogging.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>
#include <QStandardPaths>
#include <QDir>
#include <version.h>

void usage();

struct CommandLineOptions {
    bool headless = false;
    bool autoConnect = false;
    bool verbose = false;
    QString host;
    int port = 9101;
    QString director;
    QString console;
    QString password;
    QString configFile;
    QString command;

    // TLS options
    bool tlsEnabled = false;
    QString tlsCaFile;

#ifdef Q_OS_WINDOWS
    QString tlsPfxFile;
    QString tlsPfxPassword;
#else
    QString tlsCertFile;
    QString tlsKeyFile;
#endif
};
CommandLineOptions parseCommandLine(QApplication &app)
{
    QCommandLineParser parser;
    parser.setApplicationDescription(
        "Onesimus - Bareos Backup Management\n"
        "\n"
        "A modern Qt-based interface for Bareos Director.\n"
        "Can be used with GUI or in headless/batch mode."
        );

    // Standard-Optionen
    parser.addHelpOption();

    // Version mit -V (statt -v, damit -v für --verbose frei bleibt)
    QCommandLineOption versionOption(
        QStringList() << "V" << "version",
        "Displays version information."
        );
    parser.addOption(versionOption);

    // ========================================
    // GUI-Optionen
    // ========================================

    QCommandLineOption headlessOption(
        QStringList() << "n" << "no-gui" << "headless",
        "Run without GUI (batch mode)"
        );
    parser.addOption(headlessOption);

    QCommandLineOption autoConnectOption(
        QStringList() << "a" << "auto-connect",
        "Connect automatically on startup"
        );
    parser.addOption(autoConnectOption);

    QCommandLineOption verboseOption(
        QStringList() << "v" << "verbose",
        "Verbose output (also: ONESIMUS_VERBOSE=1)"
        );
    parser.addOption(verboseOption);

    // ========================================
    // Verbindungs-Optionen
    // ========================================

    QCommandLineOption hostOption(
        QStringList() << "H" << "host",
        "Bacula Director hostname or IP",
        "host"
        );
    parser.addOption(hostOption);

    QCommandLineOption portOption(
        QStringList() << "p" << "port",
        "Director port",
        "port",
        "9101"
        );
    parser.addOption(portOption);

    QCommandLineOption directorOption(
        QStringList() << "D" << "director",
        "Director name",
        "director",
        "bareos-dir"
        );
    parser.addOption(directorOption);

    QCommandLineOption consoleOption(
        QStringList() << "C" << "console",
        "Console name for authentication",
        "console",
        "onesimus"
        );
    parser.addOption(consoleOption);

    QCommandLineOption passwordOption(
        QStringList() << "P" << "password",
        "Director password",
        "password"
        );
    parser.addOption(passwordOption);

    // ========================================
    // Config-Optionen
    // ========================================

    QCommandLineOption configOption(
        QStringList() << "c" << "config",
        "Configuration file",
        "file"
        );
    parser.addOption(configOption);

    // ========================================
    // TLS-Optionen (plattformabhängig)
    // ========================================

    QCommandLineOption tlsOption(
        "tls",
        "Enable TLS/SSL"
        );
    parser.addOption(tlsOption);

    QCommandLineOption tlsCaOption(
        "tls-ca",
        "TLS CA certificate file",
        "ca-file"
        );
    parser.addOption(tlsCaOption);

#ifdef Q_OS_WINDOWS
    // Windows: PKCS#12 (.pfx) Format
    QCommandLineOption tlsPfxOption(
        "tls-pfx",
        "TLS PFX/PKCS#12 certificate file (contains certificate and private key)",
        "pfx-file"
        );
    parser.addOption(tlsPfxOption);

    QCommandLineOption tlsPfxPasswordOption(
        "tls-pfx-password",
        "Password for PFX/PKCS#12 file",
        "password"
        );
    parser.addOption(tlsPfxPasswordOption);
#else
    // Linux/Unix: Separate PEM files
    QCommandLineOption tlsCertOption(
        "tls-cert",
        "TLS certificate file (PEM format)",
        "cert-file"
        );
    parser.addOption(tlsCertOption);

    QCommandLineOption tlsKeyOption(
        "tls-key",
        "TLS private key file (PEM format)",
        "key-file"
        );
    parser.addOption(tlsKeyOption);
#endif

    // ========================================
    // Befehls-Optionen
    // ========================================

    QCommandLineOption execOption(
        QStringList() << "e" << "exec" << "execute",
        "Execute bconsole command and exit",
        "command"
        );
    parser.addOption(execOption);

    QCommandLineOption scriptOption(
        QStringList() << "s" << "script",
        "Execute commands from script file",
        "script-file"
        );
    parser.addOption(scriptOption);

    // ========================================
    // Positionale Argumente
    // ========================================

    parser.addPositionalArgument(
        "command",
        "Bconsole command to execute",
        "[command]"
        );

    // Parse
    parser.process(app);

    // -V / --version manuell behandeln
    if (parser.isSet(versionOption)) {
        printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
               qPrintable(QCoreApplication::applicationVersion()));
        ::exit(0);
    }

    // ========================================
    // Ergebnisse extrahieren
    // ========================================

    CommandLineOptions options;
    options.headless    = parser.isSet(headlessOption);
    options.autoConnect = parser.isSet(autoConnectOption);
    options.verbose     = parser.isSet(verboseOption);
    options.host        = parser.value(hostOption);
    options.port        = parser.value(portOption).toInt();
    options.director    = parser.value(directorOption);
    options.console     = parser.value(consoleOption);
    options.password    = parser.value(passwordOption);
    options.configFile  = parser.value(configOption);

    // TLS-Optionen (gemeinsam)
    options.tlsEnabled  = parser.isSet(tlsOption);
    options.tlsCaFile   = parser.value(tlsCaOption);

#ifdef Q_OS_WINDOWS
    // Windows: PFX
    options.tlsPfxFile     = parser.value(tlsPfxOption);
    options.tlsPfxPassword = parser.value(tlsPfxPasswordOption);
#else
    // Linux/Unix: PEM
    options.tlsCertFile = parser.value(tlsCertOption);
    options.tlsKeyFile  = parser.value(tlsKeyOption);
#endif

    // Befehl
    if (parser.isSet(execOption)) {
        options.command = parser.value(execOption);
    } else {
        QStringList args = parser.positionalArguments();
        if (!args.isEmpty()) {
            options.command = args.join(" ");
        }
    }

    return options;
}

void usage()
{
    qInfo() << QString(
        "Onesimus - Bareos Backup Management\n"
        "Version: " PROJECT_VERSION "\n"
        "Copyright (C) 2025\n"
        "\n"
        "Usage: onesimus [options] [command]\n"
        "\n"
        "GUI Options:\n"
        "  -n, --no-gui, --headless    Run without GUI (batch mode)\n"
        "  -a, --auto-connect          Connect automatically on startup\n"
        "  -v, --verbose               Verbose output\n"
        "\n"
        "Connection Options:\n"
        "  -H, --host <host>           Bareos Director hostname or IP\n"
        "  -p, --port <port>           Director port (default: 9101)\n"
        "  -D, --director <name>       Director name (default: bareos-dir)\n"
        "  -C, --console <name>        Console name (default: onesimus)\n"
        "  -P, --password <password>   Director password\n"
        "\n"
        "Configuration:\n"
        "  -c, --config <file>         Configuration file\n"
        "\n"
        "TLS/SSL Options:\n"
        "  --tls                       Enable TLS/SSL\n"
        "  --tls-ca <file>             TLS CA certificate file\n"
#ifdef Q_OS_WINDOWS
        "  --tls-pfx <file>            TLS PFX/PKCS#12 file (certificate + key)\n"
        "  --tls-pfx-password <pwd>    Password for PFX file\n"
#else
        "  --tls-cert <file>           TLS certificate file (PEM format)\n"
        "  --tls-key <file>            TLS private key file (PEM format)\n"
#endif
        "\n"
        "Command Execution:\n"
        "  -e, --exec, --execute <cmd> Execute bconsole command and exit\n"
        "  -s, --script <file>         Execute commands from script file\n"
        "  [command]                   Bconsole command to execute\n"
        "\n"
        "Other Options:\n"
        "  -h, --help                  Display this help and exit\n"
        "  --version                   Display version information and exit\n"
        "\n"
        "Examples:\n"
        "  GUI mode:\n"
        "    onesimus\n"
        "    onesimus --auto-connect\n"
        "\n"
        "  Headless mode with command:\n"
        "    onesimus --headless -H localhost -D bacula-dir -P secret -e \"status director\"\n"
        "\n"
#ifdef Q_OS_WINDOWS
        "  With TLS (Windows):\n"
        "    onesimus --tls --tls-ca \"C:\\certs\\ca.pem\" --tls-pfx \"C:\\certs\\client.pfx\" --tls-pfx-password \"mypass\"\n"
#else
        "  With TLS (Linux / OS X):\n"
        "    onesimus --tls --tls-ca /etc/bacula/ca.pem --tls-cert /etc/bacula/cert.pem --tls-key /etc/bacula/key.pem\n"
#endif
        "\n"
        "  Execute script:\n"
        "    onesimus --headless -H localhost -P secret -s backup-script.txt\n"
        "\n");

    exit(1);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::addLibraryPath(
        QCoreApplication::applicationDirPath() + "/plugins");

    app.setApplicationName(PROJECT_NAME);
    app.setApplicationVersion(PROJECT_VERSION);
#ifdef USE_BACULA
    app.setOrganizationName("bacula");
    app.setOrganizationDomain("bacula.org");
#else
    app.setOrganizationName("bareos");
    app.setOrganizationDomain("bareos.org");
#endif
    // Use Onesimus application icon
    app.setWindowIcon(QIcon(":/icons/icons/onesimus.svg"));

    // Command-Line parsen
    CommandLineOptions options = parseCommandLine(app);

    // Initialize file logging (if enabled via compile flag ONESIMUS_FILE_LOGGING)
    QString logPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(logPath);
    BLOG_INIT(logPath + "/onesimus.log", 10);

    // Enable logging via --verbose flag or ONESIMUS_VERBOSE=1 environment variable
    if (options.verbose || qEnvironmentVariableIsSet("ONESIMUS_VERBOSE")) {
        BLOG_SET_ENABLED(true);
    }

    if (options.verbose) {
        BLOG_INFO() << "Starting" << PROJECT_NAME << "v" PROJECT_VERSION;
        BLOG_INFO() << "Build:" << BUILD_DATE << BUILD_TIME;
    }

    // ========================================
    // Headless-Modus (kein GUI)
    // ========================================

    if (options.headless) {
        if (options.verbose) {
            BLOG_INFO() << "Running in headless mode";
        }

        // Director-Verbindung ohne GUI
        BDirector director;

        // Verbinde
        if (!options.host.isEmpty()) {
            if (options.verbose) {
                BLOG_INFO() << "Connecting to" << options.host << ":" << options.port;
            }

            director.connect(
                options.host,
                options.port,
                options.director,
                options.console,
                options.password
                );

            // Warte auf Verbindung
            QEventLoop loop;
            // QObject::connect(&director, &BDirector::connected, &loop, &QEventLoop::quit);
            QObject::connect(&director, &BDirector::protocolError, &loop, &QEventLoop::quit);
            QTimer::singleShot(10000, &loop, &QEventLoop::quit); // Timeout
            loop.exec();

            if (director.isConnected()) {
                // Führe Befehl aus
                if (!options.command.isEmpty()) {
                    // director.doSendCommand(options.command);

                    // Warte auf Antwort
                    QObject::connect(&director, &BDirector::commandResponse,
                                     [](const QString &response) {
                                         BLOG_DEBUG() << response;
                                     });

                    QTimer::singleShot(5000, &app, &QCoreApplication::quit);
                    return app.exec();
                }
            } else {
                BLOG_ERROR() << "Connection failed";
                BLOG_CLOSE();
                return 1;
            }
        }

        return 0;
    }

    // ========================================
    // GUI-Modus (normal)
    // ========================================

    // ========================================
    // Initialize Translations
    // ========================================

    BTranslations::Language language;
    QString languageCode;

    // Check if this is the first start (no language preference saved)
    if (!BSettings::instance().hasLanguagePreference()) {
        // First start: detect and save system language
        language = BTranslations::detectSystemLanguage();
        languageCode = BTranslations::languageCode(language);
        BSettings::instance().setAppearanceLanguage(languageCode);

        if (options.verbose) {
            BLOG_INFO() << "First start: Detected system language:" << languageCode
                        << "(" << BTranslations::languageName(language) << ")";
        }
    } else {
        // Load saved language preference
        languageCode = BSettings::instance().appearanceLanguage();
        language = BTranslations::languageFromCode(languageCode);

        if (options.verbose) {
            BLOG_INFO() << "Loading saved language preference:" << languageCode
                        << "(" << BTranslations::languageName(language) << ")";
        }
    }

    // Initialize translation system BEFORE creating MainWindow
    BTranslations::instance()->setLanguage(language);

    BMainWindow w;

    // Auto-Connect
    if (options.autoConnect) {
        if (!options.host.isEmpty()) {
            if (options.verbose) {
                BLOG_INFO() << "Auto-connecting to" << options.host;
            }

            // Verbindungs-Dialog überspringen und direkt verbinden
            if (options.tlsEnabled){
                if(!options.tlsCaFile.isEmpty())
                    w.director()->tlsConfig()->tlsCaCertFile->setFileName(options.tlsCaFile);
#ifdef Q_OS_WINDOWS
                if(!options.tlsPfxFile.isEmpty())
                    w.director()->tlsConfig()->tlsCaCertFile->setFileName(options.tlsPfxFile);
                if(!options.tlsPfxPassword.isEmpty())
                    w.director()->tlsConfig()->tlsCaCertFile->setFileName(options.tlsPfxPassword);
#else
                if(!options.tlsCertFile.isEmpty())
                    w.director()->tlsConfig()->tlsCaCertFile->setFileName(options.tlsCertFile);
                if(!options.tlsKeyFile.isEmpty())
                    w.director()->tlsConfig()->tlsCaCertFile->setFileName(options.tlsKeyFile);
#endif
            }
            // TLS konfigurieren...

            QMetaObject::invokeMethod(&w, [&w, &options]() {
                w.director()->connect(
                    options.host,
                    options.port,
                    options.director,
                    options.console,
                    options.password
                    );
            }, Qt::QueuedConnection);
        } else {
            // Lade letzte Verbindung
            QMetaObject::invokeMethod(&w, "onConnectLastUsed", Qt::QueuedConnection);
        }
    }

    w.show();
    return app.exec();
}
