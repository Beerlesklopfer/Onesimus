/**
 * @file main.cpp
 * @brief Integration test for BJobFilesWidget - Tests complete data flow
 *
 * This test verifies the complete data pipeline:
 * Director → BVFS JSON Response → Widget → Model → UI
 *
 * Two modes available:
 * 1. MockDirector mode (default): Simulates realistic BVFS responses for isolated testing
 * 2. Real Director mode: Connects to actual Bareos/Bacula Director for live testing
 *
 * Use --help for command-line options.
 */

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QSplitter>
#include <QGroupBox>
#include <QDebug>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QRegularExpression>

// Include the actual widget we're testing
#include "jobs/bjobfileswidget.h"
#include "director/bdirector.h"

/**
 * @brief Mock Director that simulates BVFS responses
 *
 * This simulates a real BDirector by emitting jsonResult signals
 * with realistic BVFS JSON data.
 */
class MockDirector : public BDirector
{
    Q_OBJECT

public:
    MockDirector(QObject *parent = nullptr)
        : BDirector(parent)
        , m_simulateDelay(true)
        , m_responseDelay(100)
    {
        qDebug() << "MockDirector: Created";
    }

    // Override doSend to intercept and simulate responses
    void doSend(Command cmd, const QString &args) override
    {
        qDebug() << "MockDirector: Received cmd=" << static_cast<int>(cmd) << "args=" << args;

        // Simulate network delay
        int delay = m_simulateDelay ? m_responseDelay : 0;

        // Route to appropriate response handler based on Command enum
        switch (cmd) {
        case Command::BvfsGetJobIds:
            QTimer::singleShot(delay, this, [this, cmd]() {
                emit jsonResult(cmd, getBvfsJobIdsResponse());
            });
            break;
        case Command::BvfsUpdate:
            QTimer::singleShot(delay, this, [this, cmd]() {
                emit jsonResult(cmd, getBvfsUpdateResponse());
            });
            break;
        case Command::BvfsLsDirs:
            QTimer::singleShot(delay, this, [this, cmd, args]() {
                emit jsonResult(cmd, getBvfsLsDirsResponse(args));
            });
            break;
        case Command::BvfsLsFiles:
            QTimer::singleShot(delay, this, [this, cmd, args]() {
                emit jsonResult(cmd, getBvfsLsFilesResponse(args));
            });
            break;
        default:
            qWarning() << "MockDirector: Unknown command:" << static_cast<int>(cmd) << args;
            break;
        }
    }

    void setSimulateDelay(bool enable) { m_simulateDelay = enable; }
    void setResponseDelay(int ms) { m_responseDelay = ms; }

private:
    QString getBvfsJobIdsResponse()
    {
        // Simulates: .bvfs_get_jobids jobid=123
        // Returns job chain (Full + Incrementals)
        QJsonObject response;
        QJsonObject result;
        QJsonArray jobids;

        // Full backup
        QJsonObject job1;
        job1["id"] = "100";
        jobids.append(job1);

        // Differential
        QJsonObject job2;
        job2["id"] = "110";
        jobids.append(job2);

        // Incremental (the one we're viewing)
        QJsonObject job3;
        job3["id"] = "123";
        jobids.append(job3);

        result["jobids"] = jobids;
        response["result"] = result;

        return QJsonDocument(response).toJson(QJsonDocument::Compact);
    }

    QString getBvfsUpdateResponse()
    {
        // Simulates: .bvfs_update jobid=100,110,123
        // Just acknowledges cache update
        QJsonObject response;
        response["result"] = QJsonObject();
        return QJsonDocument(response).toJson(QJsonDocument::Compact);
    }

    QString getBvfsLsDirsResponse(const QString &command)
    {
        // Extract pathid from command
        // Example: .bvfs_lsdirs jobid=100,110,123 pathid=1
        int pathId = 1;
        QRegularExpression re("pathid=(\\d+)");
        QRegularExpressionMatch match = re.match(command);
        if (match.hasMatch()) {
            pathId = match.captured(1).toInt();
        }

        QJsonObject response;
        QJsonArray directories;

        // Root directory (pathid=1)
        if (pathId == 1) {
            // Parent markers
            QJsonObject dot;
            dot["pathid"] = "1";
            dot["filenameid"] = "1";
            dot["fileid"] = "1";
            dot["jobid"] = "123";
            dot["lstat"] = "drwxr-xr-x 1 0 0 0 2024-01-15 10:00:00";
            dot["name"] = ".";
            dot["type"] = "dir";
            directories.append(dot);

            QJsonObject dotdot;
            dotdot["pathid"] = "0";
            dotdot["filenameid"] = "2";
            dotdot["fileid"] = "2";
            dotdot["jobid"] = "123";
            dotdot["lstat"] = "drwxr-xr-x 1 0 0 0 2024-01-15 10:00:00";
            dotdot["name"] = "..";
            dotdot["type"] = "dir";
            directories.append(dotdot);

            // /etc
            QJsonObject etc;
            etc["pathid"] = "10";
            etc["filenameid"] = "100";
            etc["fileid"] = "100";
            etc["jobid"] = "123";
            etc["lstat"] = "drwxr-xr-x 1 0 0 4096 2024-01-20 08:30:00";
            etc["name"] = "etc/";
            etc["type"] = "dir";
            directories.append(etc);

            // /home
            QJsonObject home;
            home["pathid"] = "20";
            home["filenameid"] = "200";
            home["fileid"] = "200";
            home["jobid"] = "123";
            home["lstat"] = "drwxr-xr-x 1 0 0 4096 2024-01-25 14:00:00";
            home["name"] = "home/";
            home["type"] = "dir";
            directories.append(home);

            // /var
            QJsonObject var;
            var["pathid"] = "30";
            var["filenameid"] = "300";
            var["fileid"] = "300";
            var["jobid"] = "123";
            var["lstat"] = "drwxr-xr-x 1 0 0 4096 2024-01-31 00:00:00";
            var["name"] = "var/";
            var["type"] = "dir";
            directories.append(var);
        }
        // /etc subdirectories (pathid=10)
        else if (pathId == 10) {
            // Parent markers
            QJsonObject dot;
            dot["pathid"] = "10";
            dot["name"] = ".";
            directories.append(dot);

            QJsonObject dotdot;
            dotdot["pathid"] = "1";
            dotdot["name"] = "..";
            directories.append(dotdot);

            // /etc/apache2
            QJsonObject apache;
            apache["pathid"] = "11";
            apache["filenameid"] = "110";
            apache["fileid"] = "110";
            apache["jobid"] = "123";
            apache["lstat"] = "drwxr-xr-x 1 0 0 4096 2024-01-18 12:00:00";
            apache["name"] = "apache2/";
            apache["type"] = "dir";
            directories.append(apache);

            // /etc/ssh
            QJsonObject ssh;
            ssh["pathid"] = "12";
            ssh["filenameid"] = "120";
            ssh["fileid"] = "120";
            ssh["jobid"] = "123";
            ssh["lstat"] = "drwxr-xr-x 1 0 0 4096 2024-01-10 09:30:00";
            ssh["name"] = "ssh/";
            ssh["type"] = "dir";
            directories.append(ssh);
        }
        // /home subdirectories (pathid=20)
        else if (pathId == 20) {
            QJsonObject dot;
            dot["pathid"] = "20";
            dot["name"] = ".";
            directories.append(dot);

            QJsonObject dotdot;
            dotdot["pathid"] = "1";
            dotdot["name"] = "..";
            directories.append(dotdot);

            // /home/user1
            QJsonObject user1;
            user1["pathid"] = "21";
            user1["name"] = "user1/";
            directories.append(user1);

            // /home/user2
            QJsonObject user2;
            user2["pathid"] = "22";
            user2["name"] = "user2/";
            directories.append(user2);
        }
        // No subdirectories for other paths
        else {
            QJsonObject dot;
            dot["pathid"] = QString::number(pathId);
            dot["name"] = ".";
            directories.append(dot);

            QJsonObject dotdot;
            dotdot["pathid"] = "1";
            dotdot["name"] = "..";
            directories.append(dotdot);
        }

        QJsonObject result;
        result["directories"] = directories;
        response["result"] = result;

        return QJsonDocument(response).toJson(QJsonDocument::Compact);
    }

    QString getBvfsLsFilesResponse(const QString &command)
    {
        // Extract pathid from command
        int pathId = 1;
        QRegularExpression re("pathid=(\\d+)");
        QRegularExpressionMatch match = re.match(command);
        if (match.hasMatch()) {
            pathId = match.captured(1).toInt();
        }

        QJsonObject response;
        QJsonArray files;

        // Files in root (pathid=1)
        if (pathId == 1) {
            QJsonObject file1;
            file1["fileid"] = "1001";
            file1["filenameid"] = "1001";
            file1["jobid"] = "123";
            file1["name"] = ".bashrc";
            file1["lstat"] = "frw-r--r-- 1 0 0 3771 2024-01-25 10:30:00";
            file1["type"] = "file";
            files.append(file1);

            QJsonObject file2;
            file2["fileid"] = "1002";
            file2["name"] = ".profile";
            file2["lstat"] = "frw-r--r-- 1 0 0 807 2024-01-20 08:15:00";
            files.append(file2);

            QJsonObject file3;
            file3["fileid"] = "1003";
            file3["name"] = "backup.tar.gz";
            file3["lstat"] = "frw-r--r-- 1 0 0 268435456 2024-01-31 02:00:00";
            files.append(file3);
        }
        // Files in /etc (pathid=10)
        else if (pathId == 10) {
            QJsonObject file1;
            file1["fileid"] = "2001";
            file1["name"] = "hosts";
            file1["lstat"] = "frw-r--r-- 1 0 0 240 2024-01-15 09:00:00";
            files.append(file1);

            QJsonObject file2;
            file2["fileid"] = "2002";
            file2["name"] = "passwd";
            file2["lstat"] = "frw-r--r-- 1 0 0 1802 2024-01-28 11:20:00";
            files.append(file2);

            QJsonObject file3;
            file3["fileid"] = "2003";
            file3["name"] = "group";
            file3["lstat"] = "frw-r--r-- 1 0 0 945 2024-01-28 11:20:00";
            files.append(file3);
        }
        // Files in /home (pathid=20)
        else if (pathId == 20) {
            // /home typically has no files, only directories
        }

        QJsonObject result;
        result["files"] = files;
        response["result"] = result;

        return QJsonDocument(response).toJson(QJsonDocument::Compact);
    }

private:
    bool m_simulateDelay;
    int m_responseDelay;
};

/**
 * @brief Test window that hosts the BJobFilesWidget and controls
 */
class BJobFilesWidgetTestWindow : public QMainWindow
{
    Q_OBJECT

public:
    BJobFilesWidgetTestWindow(bool useMockDirector = true, QWidget *parent = nullptr)
        : QMainWindow(parent)
        , m_useMockDirector(useMockDirector)
    {
        setWindowTitle("BJobFilesWidget Integration Test - Complete Data Pipeline");
        resize(1200, 700);

        if (m_useMockDirector) {
            setupMockDirector();
        } else {
            setupRealDirector();
        }
        setupUi();
        setupTestJob();
    }

    void setDirectorConnection(const QString &address, int port, const QString &name, const QString &password)
    {
        m_directorAddress = address;
        m_directorPort = port;
        m_directorName = name;
        m_directorPassword = password;
    }

private:
    void setupMockDirector()
    {
        m_mockDirector = new MockDirector(this);
        m_mockDirector->setSimulateDelay(true);
        m_mockDirector->setResponseDelay(100);  // 100ms delay to simulate network
        m_director = m_mockDirector;

        qDebug() << "Test: MockDirector created";
    }

    void setupRealDirector()
    {
        m_director = new BDirector(this);

        // Connect to real director if connection parameters provided
        if (!m_directorAddress.isEmpty()) {
            qDebug() << "Test: Connecting to real Director at" << m_directorAddress << ":" << m_directorPort;
            // TODO: Implement connection logic
            // m_director->connectToDirector(m_directorAddress, m_directorPort, m_directorName, m_directorPassword);
        } else {
            qDebug() << "Test: Real Director created but not connected (use --director-* options)";
        }
    }

    void setupUi()
    {
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        // Info panel
        QGroupBox *infoGroup = new QGroupBox("Test Information", this);
        QVBoxLayout *infoLayout = new QVBoxLayout(infoGroup);

        QString directorMode = m_useMockDirector ? "MockDirector (simulated)" : "Real Director";
        QString infoText = QString(
            "<b>BJobFilesWidget Integration Test</b><br>"
            "Tests complete data flow: Director → BVFS JSON → Widget → Model → UI<br>"
            "<br>"
            "<b>Director Mode:</b> %1<br>"
            "<br>"
            "<b>Test Pipeline:</b><br>"
            "1. bvfs_get_jobids → Get restore chain<br>"
            "2. bvfs_update → Update BVFS cache<br>"
            "3. bvfs_lsdirs → List directories<br>"
            "4. bvfs_lsfiles → List files<br>"
            "<br>"
            "Watch the log output below to see BVFS command flow.").arg(directorMode);

        QLabel *infoLabel = new QLabel(infoText, this);
        infoLayout->addWidget(infoLabel);

        mainLayout->addWidget(infoGroup);

        // Control panel
        QHBoxLayout *controlLayout = new QHBoxLayout();

        QPushButton *reloadBtn = new QPushButton("Reload Widget", this);
        connect(reloadBtn, &QPushButton::clicked, this, &BJobFilesWidgetTestWindow::reloadWidget);
        controlLayout->addWidget(reloadBtn);

        QPushButton *clearLogBtn = new QPushButton("Clear Log", this);
        connect(clearLogBtn, &QPushButton::clicked, this, [this]() {
            m_logOutput->clear();
        });
        controlLayout->addWidget(clearLogBtn);

        QCheckBox *delayCheckBox = new QCheckBox("Simulate Network Delay", this);
        delayCheckBox->setChecked(true);
        delayCheckBox->setEnabled(m_useMockDirector);  // Only available in mock mode
        connect(delayCheckBox, &QCheckBox::toggled, [this](bool checked) {
            if (m_mockDirector) {
                m_mockDirector->setSimulateDelay(checked);
            }
        });
        controlLayout->addWidget(delayCheckBox);

        controlLayout->addStretch();

        m_statusLabel = new QLabel("Status: Ready", this);
        controlLayout->addWidget(m_statusLabel);

        mainLayout->addLayout(controlLayout);

        // Splitter for widget and log
        QSplitter *splitter = new QSplitter(Qt::Vertical, this);

        // The actual widget being tested (will be created in setupTestJob)
        m_widgetContainer = new QWidget(this);
        m_widgetLayout = new QVBoxLayout(m_widgetContainer);
        m_widgetLayout->setContentsMargins(0, 0, 0, 0);

        splitter->addWidget(m_widgetContainer);

        // Log output
        m_logOutput = new QTextEdit(this);
        m_logOutput->setReadOnly(true);
        m_logOutput->setMaximumHeight(200);
        splitter->addWidget(m_logOutput);

        splitter->setSizes({500, 200});

        mainLayout->addWidget(splitter);

        // Redirect qDebug to log window
        qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &context, const QString &msg) {
            Q_UNUSED(context);
            QString formattedMsg;
            switch (type) {
            case QtDebugMsg:
                formattedMsg = QString("[DEBUG] %1").arg(msg);
                break;
            case QtInfoMsg:
                formattedMsg = QString("[INFO] %1").arg(msg);
                break;
            case QtWarningMsg:
                formattedMsg = QString("[WARN] %1").arg(msg);
                break;
            case QtCriticalMsg:
            case QtFatalMsg:
                formattedMsg = QString("[ERROR] %1").arg(msg);
                break;
            }

            // Find test window and append to log
            foreach (QWidget *widget, QApplication::topLevelWidgets()) {
                BJobFilesWidgetTestWindow *testWindow = qobject_cast<BJobFilesWidgetTestWindow*>(widget);
                if (testWindow && testWindow->m_logOutput) {
                    testWindow->m_logOutput->append(formattedMsg);
                }
            }

            // Also print to console
            fprintf(stderr, "%s\n", formattedMsg.toLocal8Bit().constData());
            fflush(stderr);
        });
    }

    void setupTestJob()
    {
        // Create realistic job JSON
        QJsonObject job;
        job["jobid"] = "123";
        job["name"] = "BackupClient1";
        job["client"] = "client1-fd";
        job["type"] = "B";
        job["level"] = "I";
        job["fileset"] = "FullSet";

        createWidget(job);
    }

    void createWidget(const QJsonObject &job)
    {
        // Clean up old widget if exists
        if (m_filesWidget) {
            m_widgetLayout->removeWidget(m_filesWidget);
            m_filesWidget->deleteLater();
        }

        // Create new BJobFilesWidget with director (mock or real)
        m_filesWidget = new BJobFilesWidget(job, m_director, m_widgetContainer);

        m_widgetLayout->addWidget(m_filesWidget);

        m_statusLabel->setText("Widget created - BVFS loading started...");
        qDebug() << "Test: BJobFilesWidget created and loading started";
    }

private slots:
    void reloadWidget()
    {
        QJsonObject job;
        job["jobid"] = "123";
        job["name"] = "BackupClient1";
        job["client"] = "client1-fd";
        job["type"] = "B";
        job["level"] = "I";

        createWidget(job);
        m_logOutput->append("--- Widget Reloaded ---");
    }

private:
    bool m_useMockDirector;
    BDirector *m_director = nullptr;
    MockDirector *m_mockDirector = nullptr;
    BJobFilesWidget *m_filesWidget = nullptr;
    QWidget *m_widgetContainer;
    QVBoxLayout *m_widgetLayout;
    QTextEdit *m_logOutput;
    QLabel *m_statusLabel;

    // Real Director connection parameters
    QString m_directorAddress;
    int m_directorPort = 9101;
    QString m_directorName;
    QString m_directorPassword;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug() << "===========================================";
    qDebug() << "BJobFilesWidget Integration Test Starting";
    qDebug() << "===========================================";

    // Parse command-line arguments
    bool useMockDirector = true;
    QString directorAddress;
    int directorPort = 9101;
    QString directorName;
    QString directorPassword;

    for (int i = 1; i < argc; ++i) {
        QString arg = QString::fromLocal8Bit(argv[i]);

        if (arg == "--help" || arg == "-h") {
            qInfo() << "";
            qInfo() << "Usage:" << argv[0] << "[OPTIONS]";
            qInfo() << "";
            qInfo() << "Options:";
            qInfo() << "  --mock              Use MockDirector (simulated responses) [default]";
            qInfo() << "  --real              Use real Director connection";
            qInfo() << "  --director-address ADDR   Director address (e.g., localhost)";
            qInfo() << "  --director-port PORT      Director port (default: 9101)";
            qInfo() << "  --director-name NAME      Director name";
            qInfo() << "  --director-password PASS  Director password";
            qInfo() << "  -h, --help          Show this help message";
            qInfo() << "";
            qInfo() << "Examples:";
            qInfo() << "  # Use mock director (default)";
            qInfo() << "  " << argv[0];
            qInfo() << "";
            qInfo() << "  # Connect to real director";
            qInfo() << "  " << argv[0] << "--real --director-address localhost --director-name bareos-dir --director-password secret";
            qInfo() << "";
            return 0;
        }
        else if (arg == "--mock") {
            useMockDirector = true;
        }
        else if (arg == "--real") {
            useMockDirector = false;
        }
        else if (arg == "--director-address" && i + 1 < argc) {
            directorAddress = QString::fromLocal8Bit(argv[++i]);
        }
        else if (arg == "--director-port" && i + 1 < argc) {
            directorPort = QString::fromLocal8Bit(argv[++i]).toInt();
        }
        else if (arg == "--director-name" && i + 1 < argc) {
            directorName = QString::fromLocal8Bit(argv[++i]);
        }
        else if (arg == "--director-password" && i + 1 < argc) {
            directorPassword = QString::fromLocal8Bit(argv[++i]);
        }
        else {
            qWarning() << "Unknown argument:" << arg;
            qWarning() << "Use --help for usage information";
        }
    }

    // Log configuration
    if (useMockDirector) {
        qInfo() << "Mode: MockDirector (simulated responses)";
    } else {
        qInfo() << "Mode: Real Director";
        if (!directorAddress.isEmpty()) {
            qInfo() << "  Address:" << directorAddress << ":" << directorPort;
            qInfo() << "  Name:" << directorName;
        } else {
            qWarning() << "  Warning: No connection parameters provided";
            qWarning() << "  Use --director-address, --director-name, --director-password";
        }
    }

    BJobFilesWidgetTestWindow window(useMockDirector);
    if (!useMockDirector) {
        window.setDirectorConnection(directorAddress, directorPort, directorName, directorPassword);
    }
    window.show();

    return app.exec();
}

#include "main.moc"
