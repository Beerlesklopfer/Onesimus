// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "bmainwindow.h"
#include "btranslations.h"
#include "config/bsettings.h"
#include "blogging.h"
#include "bcommandlineconnection.h"
#include "bheadlessrunner.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <memory>
#include <QSslSocket>
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
    QString command;
    QString scriptFile;

    // TLS options (mapped to the Director configuration by bApplyCommandLineTls())
    BCommandLineTlsOptions tls;
};

// Print a command line error and leave; nothing has been connected yet
[[noreturn]] static void commandLineError(const QString &message)
{
    fprintf(stderr, "%s: %s\n", qPrintable(QCoreApplication::applicationName()), qPrintable(message));
    ::exit(2);
}
CommandLineOptions parseCommandLine(QCoreApplication &app)
{
    QCommandLineParser parser;
    parser.setApplicationDescription(
        "Onesimus - Bareos Backup Management\n"
        "\n"
        "A modern Qt-based interface for Bareos Director.\n"
        "Can be used with GUI or in headless/batch mode.\n"
        "\n"
        "Headless mode logs in, sends the command (-e or the arguments) and the\n"
        "commands of the script (-s) one after the other and prints each reply\n"
        "(the Director's JSON) on standard output. Exit status: 0 every command\n"
        "succeeded, 1 a command failed, 2 no session with the Director."
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
        "Director password (visible in the process list; prefer --password-file "
        "or the environment variable ONESIMUS_PASSWORD)",
        "password"
        );
    parser.addOption(passwordOption);

    QCommandLineOption passwordFileOption(
        "password-file",
        "Read the Director password from the first line of this file",
        "file"
        );
    parser.addOption(passwordFileOption);

    // ========================================
    // TLS-Optionen (plattformabhängig)
    // ========================================

    QCommandLineOption tlsOption(
        "tls",
        "Connect with TLS and refuse an unencrypted session "
        "(TLS-PSK with the password, or certificates with the --tls-* options)"
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
        "Execute a console command and exit (with --headless)",
        "command"
        );
    parser.addOption(execOption);

    QCommandLineOption scriptOption(
        QStringList() << "s" << "script",
        "Execute the commands of a script file, one per line; '-' reads standard input (with --headless)",
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
    options.scriptFile  = parser.value(scriptOption);

    // Password: -P, --password-file or ONESIMUS_PASSWORD
    QString passwordError;
    options.password = bResolveCommandLinePassword(
        parser.isSet(passwordOption), parser.value(passwordOption),
        parser.value(passwordFileOption),
        qEnvironmentVariable("ONESIMUS_PASSWORD"), &passwordError);
    if (!passwordError.isEmpty()) {
        commandLineError(passwordError);
    }

    // TLS-Optionen (gemeinsam)
    options.tls.tls    = parser.isSet(tlsOption);
    options.tls.caFile = parser.value(tlsCaOption);

#ifdef Q_OS_WINDOWS
    // Windows: PFX
    options.tls.pfxFile     = parser.value(tlsPfxOption);
    options.tls.pfxPassword = parser.value(tlsPfxPasswordOption);
#else
    // Linux/Unix: PEM
    options.tls.certFile = parser.value(tlsCertOption);
    options.tls.keyFile  = parser.value(tlsKeyOption);
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
        "Copyright (C) 2025-2026 Jörg Bernau\n"
        "Licence: AGPL-3.0 (own code: AGPL-3.0-or-later; see LICENSING.md)\n"
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
        "  -P, --password <password>   Director password (visible in the process list)\n"
        "      --password-file <file>  Read the password from the first line of <file>\n"
        "                              (or set the environment variable ONESIMUS_PASSWORD)\n"
        "\n"
        "TLS/SSL Options:\n"
        "  --tls                       Connect with TLS (TLS-PSK, or certificates with the\n"
        "                              options below); an unencrypted session is refused\n"
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
        "  -e, --exec, --execute <cmd> Execute a console command and exit\n"
        "  -s, --script <file>         Execute the commands of <file>, one per line\n"
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
        "\n"
        "Headless mode prints each reply (the Director's JSON) on standard output.\n"
        "Exit status: 0 every command succeeded, 1 a command failed, 2 no session.\n");

    exit(1);
}

// True when the command line asks for headless mode. Looked at before the
// application object exists: headless mode needs no display, so it runs on
// a QCoreApplication (the parser decides again afterwards).
static bool headlessRequested(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        const QByteArray arg(argv[i]);
        if (arg == "--") {
            break;
        }
        if (arg == "-n" || arg == "--no-gui" || arg == "--headless") {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
    const bool headless = headlessRequested(argc, argv);
    std::unique_ptr<QCoreApplication> application(
        headless ? new QCoreApplication(argc, argv) : new QApplication(argc, argv));
    QCoreApplication &app = *application;

    QCoreApplication::addLibraryPath(
        QCoreApplication::applicationDirPath() + "/plugins");

    // On Windows, Qt6 defaults to the Schannel TLS backend which does NOT support
    // PSK cipher suites required by Bareos/Bacula. Force the OpenSSL backend.
#ifdef Q_OS_WIN
    if (QSslSocket::availableBackends().contains("openssl")) {
        QSslSocket::setActiveBackend("openssl");
    }
#endif

    app.setApplicationName(PROJECT_NAME);
    app.setApplicationVersion(PROJECT_VERSION);
#ifdef USE_BACULA
    app.setOrganizationName("bacula");
    app.setOrganizationDomain("bacula.org");
#else
    app.setOrganizationName("bareos");
    app.setOrganizationDomain("bareos.org");
#endif
    if (!headless) {
        // Use Onesimus application icon
        QApplication::setWindowIcon(QIcon(":/icons/icons/onesimus.svg"));
        // Matches onesimus.desktop: Wayland app_id / X11 WM_CLASS, so GNOME (Shell,
        // System Monitor) shows the desktop file's icon and name.
        QGuiApplication::setDesktopFileName(QStringLiteral("onesimus"));
    }

    // Command-Line parsen
    CommandLineOptions options = parseCommandLine(app);
    if (options.headless != headless) {
        // "-n" taken as the value of another option, or the like
        commandLineError(QStringLiteral("Give --headless (-n) as an option of its own"));
    }

    // TLS configuration of a connection made from the command line
    // (--headless, or --auto-connect with --host)
    BDirector::TLSConfig commandLineTls;
    const QString tlsError = bApplyCommandLineTls(options.tls, commandLineTls);
    if (!tlsError.isEmpty()) {
        commandLineError(tlsError);
    }
    if (options.tls.tls && options.host.isEmpty()) {
        commandLineError(QCoreApplication::translate(
            "CommandLine", "--tls needs --host. Without --host the TLS settings of the "
                           "saved connection profile are used."));
    }
    if (options.tls.tls && options.password.isEmpty()) {
        // TLS-PSK uses the password as key; certificate TLS still logs in with it
        commandLineError(QCoreApplication::translate(
            "CommandLine", "--tls needs the console password: use --password-file, the "
                           "environment variable ONESIMUS_PASSWORD or --password."));
    }

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
        if (options.host.isEmpty()) {
            commandLineError(QStringLiteral("--headless needs --host"));
        }

        // The command (-e or the positional arguments) first, then the script
        QStringList commands;
        if (!options.command.trimmed().isEmpty()) {
            commands << options.command.trimmed();
        }
        if (!options.scriptFile.isEmpty()) {
            QFile script;
            const bool opened = options.scriptFile == QLatin1String("-")
                                    ? script.open(stdin, QIODevice::ReadOnly)
                                    : (script.setFileName(options.scriptFile),
                                       script.open(QIODevice::ReadOnly));
            if (!opened) {
                commandLineError(QStringLiteral("Cannot read the script %1: %2")
                                     .arg(options.scriptFile, script.errorString()));
            }
            commands << BHeadlessRunner::scriptCommands(QString::fromUtf8(script.readAll()));
        }

        QFile out;
        QFile err;
        out.open(stdout, QIODevice::WriteOnly | QIODevice::Unbuffered);
        err.open(stderr, QIODevice::WriteOnly | QIODevice::Unbuffered);

        // Director-Verbindung ohne GUI.
        // Without --tls this is the default configuration (no TLS)
        BDirector director;
        director.setTLSConfig(commandLineTls);

        BHeadlessRunner runner(&director, commands, &out, &err);
        QObject::connect(&runner, &BHeadlessRunner::finished, &app, [&app](int exitCode) {
            app.exit(exitCode);
        });

        if (options.verbose) {
            BLOG_INFO() << "Connecting to" << options.host << ":" << options.port;
        }
        runner.start(options.host, options.port, options.director, options.console,
                     options.password);
        const int exitCode = app.exec();
        BLOG_CLOSE();
        return exitCode;
    }

    // ========================================
    // GUI-Modus (normal)
    // ========================================

    // ========================================
    // Initialize Translations
    // ========================================

    // The language of the system, unless the settings name another one.
    // Nothing is stored here: a system that changes its language takes
    // Onesimus along at the next start.
    const QString languageCode = BSettings::instance().appearanceLanguage();
    const BTranslations::Language language = BTranslations::resolve(languageCode);
    if (options.verbose) {
        BLOG_INFO() << "Language setting:" << languageCode
                    << "->" << BTranslations::languageName(language);
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

            // Verbindungs-Dialog überspringen und direkt verbinden.
            // Without --tls this is the default configuration (no TLS); with
            // --tls the connection fails rather than continue unencrypted.
            w.director()->setTLSConfig(commandLineTls);

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
