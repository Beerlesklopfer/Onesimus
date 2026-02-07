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
 *   bvfs         Explore BVFS (Backup Virtual File System)
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

#include "director/bareosdirector.h"

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

        // Handle BVFS responses
        if (m_pendingCommand.startsWith("bvfs_")) {
            processBvfsResponse(json);
            return;
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
        } else if (m_config.command == "bvfs") {
            startBvfsExplorer();
        } else if (m_config.command == "interactive") {
            startInteractiveMode();
        }
    }

    /**
     * @brief Start BVFS (Backup Virtual File System) exploration
     *
     * BVFS allows browsing backup files in the catalog.
     * Workflow:
     *   1. Get list of backup jobs to find JobIDs
     *   2. Update BVFS cache for selected job
     *   3. Get all related JobIDs for restore chain
     *   4. List directories at root (/)
     *   5. List files in a directory
     *   6. Show versions of a file
     */
    void startBvfsExplorer()
    {
        qDebug() << "\n=== BVFS Explorer ===";
        qDebug() << "Exploring Backup Virtual File System";
        qDebug() << "====================================\n";

        // Step 1: Get recent backup jobs to find a JobID
        qDebug() << "[BVFS] Step 1: Getting recent backup jobs...";
        m_bvfsState = BvfsState::GettingJobs;
        m_pendingCommand = "bvfs_getjobs";

        // Use SQL query to get recent successful backup jobs
        // Note: Need JOIN with Client table to get client name
        m_director->sendRawCommand(
            ".sql query=\"SELECT j.JobId, j.Name, c.Name AS Client, j.Level, j.JobStatus, j.StartTime "
            "FROM Job j LEFT JOIN Client c ON j.ClientId = c.ClientId "
            "WHERE j.Type='B' AND j.JobStatus IN ('T','W') "
            "ORDER BY j.JobId DESC LIMIT 10\""
        );
    }

    /**
     * @brief Process BVFS JSON response based on current state
     */
    void processBvfsResponse(const QString &json)
    {
        QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
        if (!doc.isObject()) {
            qDebug() << "[BVFS] Error: Invalid JSON response";
            finishTest(false);
            return;
        }

        QJsonObject root = doc.object();
        QJsonObject result = root["result"].toObject();

        switch (m_bvfsState) {
        case BvfsState::GettingJobs:
            handleBvfsJobsResponse(result);
            break;
        case BvfsState::UpdatingCache:
            handleBvfsCacheUpdated(result);
            break;
        case BvfsState::GettingJobIds:
            handleBvfsJobIdsResponse(result);
            break;
        case BvfsState::ListingDirs:
            handleBvfsDirsResponse(result);
            break;
        case BvfsState::ListingFiles:
            handleBvfsFilesResponse(result);
            break;
        case BvfsState::GettingVersions:
            handleBvfsVersionsResponse(result);
            break;
        default:
            break;
        }
    }

    void handleBvfsJobsResponse(const QJsonObject &result)
    {
        // Parse SQL query result
        QJsonArray queryResult = result["query"].toArray();
        if (queryResult.isEmpty()) {
            qDebug() << "[BVFS] No backup jobs found in catalog";
            finishTest(false);
            return;
        }

        qDebug() << "\n[BVFS] Recent Backup Jobs:";
        qDebug() << "  JobId | Name                | Client              | Level | Status | StartTime";
        qDebug() << "  ------|---------------------|---------------------|-------|--------|--------------------";

        int selectedJobId = 0;
        QString selectedClient;

        for (const QJsonValue &val : queryResult) {
            QJsonObject job = val.toObject();
            int jobId = job["jobid"].toString().toInt();
            QString name = job["name"].toString().left(19);
            QString client = job["client"].toString().left(19);
            QString level = job["level"].toString();
            QString status = job["jobstatus"].toString();
            QString startTime = job["starttime"].toString().left(19);

            qDebug().noquote() << QString("  %1 | %2 | %3 | %4     | %5      | %6")
                .arg(jobId, 5)
                .arg(name, -19)
                .arg(client, -19)
                .arg(level)
                .arg(status)
                .arg(startTime);

            // Select first job for exploration
            if (selectedJobId == 0) {
                selectedJobId = jobId;
                selectedClient = job["client"].toString();
            }
        }

        if (selectedJobId == 0) {
            qDebug() << "\n[BVFS] No suitable job found";
            finishTest(false);
            return;
        }

        m_bvfsJobId = selectedJobId;
        m_bvfsClient = selectedClient;

        // Step 2: Update BVFS cache for this job
        qDebug() << "\n[BVFS] Step 2: Updating BVFS cache for JobId" << m_bvfsJobId << "...";
        m_bvfsState = BvfsState::UpdatingCache;
        m_pendingCommand = "bvfs_update";
        m_director->sendRawCommand(QString(".bvfs_update jobid=%1").arg(m_bvfsJobId));
    }

    void handleBvfsCacheUpdated(const QJsonObject &result)
    {
        Q_UNUSED(result)
        qDebug() << "[BVFS] Cache updated successfully";

        // Step 3: Get all JobIDs for the restore chain
        qDebug() << "\n[BVFS] Step 3: Getting restore chain JobIDs...";
        m_bvfsState = BvfsState::GettingJobIds;
        m_pendingCommand = "bvfs_getjobids";
        m_director->sendRawCommand(QString(".bvfs_get_jobids jobid=%1 all").arg(m_bvfsJobId));
    }

    void handleBvfsJobIdsResponse(const QJsonObject &result)
    {
        // Parse jobids response - format: [{"id": "123"}, {"id": "456"}, ...]
        QJsonArray jobids = result["jobids"].toArray();
        if (jobids.isEmpty()) {
            // Fallback to original job
            m_bvfsJobIds << QString::number(m_bvfsJobId);
        } else {
            for (const QJsonValue &val : jobids) {
                // Each entry is an object with "id" field
                QJsonObject obj = val.toObject();
                QString id = obj["id"].toString();
                if (!id.isEmpty()) {
                    m_bvfsJobIds << id;
                }
            }
        }

        // Fallback if still empty
        if (m_bvfsJobIds.isEmpty()) {
            m_bvfsJobIds << QString::number(m_bvfsJobId);
        }

        qDebug() << "[BVFS] Restore chain JobIDs:" << m_bvfsJobIds.join(",");

        // Step 4: List directories at root
        qDebug() << "\n[BVFS] Step 4: Listing directories at root (/)...";
        m_bvfsState = BvfsState::ListingDirs;
        m_pendingCommand = "bvfs_lsdirs";
        m_director->sendRawCommand(
            QString(".bvfs_lsdirs jobid=%1 path=/ limit=20").arg(m_bvfsJobIds.join(","))
        );
    }

    void handleBvfsDirsResponse(const QJsonObject &result)
    {
        QJsonArray dirs = result["directories"].toArray();
        qDebug() << "\n[BVFS] Directories at /:";
        qDebug() << "  PathId | Name";
        qDebug() << "  -------|--------------------";

        QString firstDir;
        int firstPathId = 0;

        for (const QJsonValue &val : dirs) {
            QJsonObject dir = val.toObject();
            int pathId = dir["pathid"].toString().toInt();
            QString name = dir["name"].toString();

            // Skip . and ..
            if (name == "." || name == "..") continue;

            qDebug().noquote() << QString("  %1 | %2")
                .arg(pathId, 6)
                .arg(name);

            if (firstDir.isEmpty() && !name.isEmpty()) {
                firstDir = name;
                firstPathId = pathId;
            }
        }

        if (dirs.isEmpty()) {
            qDebug() << "  (no directories found)";
        }

        // Step 5: List files in first directory or root
        qDebug() << "\n[BVFS] Step 5: Listing files...";
        m_bvfsState = BvfsState::ListingFiles;
        m_pendingCommand = "bvfs_lsfiles";
        m_bvfsPathId = firstPathId > 0 ? firstPathId : 1;

        QString path = firstPathId > 0 ? QString("pathid=%1").arg(firstPathId) : "path=/";
        m_director->sendRawCommand(
            QString(".bvfs_lsfiles jobid=%1 %2 limit=20").arg(m_bvfsJobIds.join(","), path)
        );
    }

    void handleBvfsFilesResponse(const QJsonObject &result)
    {
        QJsonArray files = result["files"].toArray();
        qDebug() << "\n[BVFS] Files:";
        qDebug() << "  FileId | Name                          | Size       | MTime";
        qDebug() << "  -------|-------------------------------|------------|--------------------";

        QString firstFile;
        int firstFileId = 0;

        for (const QJsonValue &val : files) {
            QJsonObject file = val.toObject();
            int fileId = file["fileid"].toString().toInt();
            QString name = file["name"].toString().left(29);
            qint64 size = file["stat"].toObject()["st_size"].toVariant().toLongLong();
            QString mtime = file["stat"].toObject()["st_mtime"].toString().left(19);

            qDebug().noquote() << QString("  %1 | %2 | %3 | %4")
                .arg(fileId, 6)
                .arg(name, -29)
                .arg(size, 10)
                .arg(mtime);

            if (firstFile.isEmpty() && fileId > 0) {
                firstFile = file["name"].toString();
                firstFileId = fileId;
            }
        }

        if (files.isEmpty()) {
            qDebug() << "  (no files found)";
            finishBvfsExplorer();
            return;
        }

        // Step 6: Get versions of first file
        if (!firstFile.isEmpty() && m_bvfsPathId > 0) {
            qDebug() << "\n[BVFS] Step 6: Getting versions of file:" << firstFile;
            m_bvfsState = BvfsState::GettingVersions;
            m_pendingCommand = "bvfs_versions";
            m_director->sendRawCommand(
                QString(".bvfs_versions jobid=0 client=%1 pathid=%2 filename=%3")
                    .arg(m_bvfsClient)
                    .arg(m_bvfsPathId)
                    .arg(firstFile)
            );
        } else {
            finishBvfsExplorer();
        }
    }

    void handleBvfsVersionsResponse(const QJsonObject &result)
    {
        QJsonArray versions = result["versions"].toArray();
        qDebug() << "\n[BVFS] File Versions:";
        qDebug() << "  JobId | FileId | MTime               | Size";
        qDebug() << "  ------|--------|---------------------|------------";

        for (const QJsonValue &val : versions) {
            QJsonObject ver = val.toObject();
            int jobId = ver["jobid"].toString().toInt();
            int fileId = ver["fileid"].toString().toInt();
            QString mtime = ver["mtime"].toString().left(19);
            qint64 size = ver["stat"].toObject()["st_size"].toVariant().toLongLong();

            qDebug().noquote() << QString("  %1 | %2 | %3 | %4")
                .arg(jobId, 5)
                .arg(fileId, 6)
                .arg(mtime, -19)
                .arg(size, 10);
        }

        if (versions.isEmpty()) {
            qDebug() << "  (no versions found)";
        }

        finishBvfsExplorer();
    }

    void finishBvfsExplorer()
    {
        qDebug() << "\n====================================";
        qDebug() << "[BVFS] Exploration complete";
        qDebug() << "";
        qDebug() << "BVFS Commands Summary:";
        qDebug() << "  .bvfs_update jobid=<id>           - Update BVFS cache";
        qDebug() << "  .bvfs_get_jobids jobid=<id> [all] - Get restore chain";
        qDebug() << "  .bvfs_lsdirs jobid=<ids> path=/   - List directories";
        qDebug() << "  .bvfs_lsfiles jobid=<ids> path=/  - List files";
        qDebug() << "  .bvfs_versions client=<c> ...     - Get file versions";
        qDebug() << "  .bvfs_restore path=<p> ...        - Mark for restore";
        qDebug() << "  .bvfs_cleanup path=<p>            - Cleanup temp data";
        qDebug() << "  .bvfs_clear_cache yes             - Clear entire cache";
        qDebug() << "====================================";

        m_bvfsState = BvfsState::Done;
        finishTest(true);
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
        case BareosDirector::ConnectionError:  return "ConnectionError";
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

    // BVFS exploration state
    enum class BvfsState {
        Idle,
        GettingJobs,
        UpdatingCache,
        GettingJobIds,
        ListingDirs,
        ListingFiles,
        GettingVersions,
        Done
    };
    BvfsState m_bvfsState = BvfsState::Idle;
    int m_bvfsJobId = 0;
    QString m_bvfsClient;
    QStringList m_bvfsJobIds;
    int m_bvfsPathId = 0;
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
        "Test command: connect, status, jobs, clients, bvfs, interactive",
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

    QStringList validCommands = {"connect", "status", "jobs", "clients", "bvfs", "interactive"};
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
