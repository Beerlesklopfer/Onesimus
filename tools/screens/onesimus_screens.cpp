// onesimus_screens - writes the screenshots of the website.
//
// The real main window and dialogs of Onesimus, connected through the normal
// connection code (a connection profile, gRPC with a pinned certificate) to
// the simulator onesimus-sim on 127.0.0.1, are grabbed as PNG files: offscreen,
// at a fixed size, in the dark theme, in English or German. tools/screenshots.sh
// starts the simulator and runs this program once per language.
//
// Isolation: before anything else the program points HOME and the XDG
// directories (settings, profiles, database, log, caches) to a directory of
// its own, removes what an earlier run left there, and stops if Qt would still
// write anywhere else. It connects to 127.0.0.1 only.

#include "bmainwindow.h"
#include "btranslations.h"
#include "bconnectionprofile.h"
#include "bcleanupdialog.h"
#include "bconnectionwizard.h"
#include "config/bsettings.h"
#include "config/bsettingsdialog.h"
#include "clients/bclientswidget.h"
#include "clients/bclientsmodel.h"
#include "director/bdirector.h"
#include "director/baclmatrixdialog.h"
#include "jobs/bjobwidget.h"
#include "jobs/bjsonjobview.h"
#include "jobs/bjobmodels.h"
#include "jobs/bjoblogmodel.h"
#include "jobs/brestorewizard.h"
#include "schedules/bschedulewidget.h"
#include "schedules/bschedulegridview.h"
#include "schedules/bscheduleganttwidget.h"
#include "storagewidget.h"
#include "tools/b321dialog.h"

#include <QAbstractButton>
#include <QAbstractItemView>
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QCommandLineParser>
#include <QCheckBox>
#include <QDateTime>
#include <QDialog>
#include <QDir>
#include <QDockWidget>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QFont>
#include <QFontInfo>
#include <QHostAddress>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QPointer>
#include <QPushButton>
#include <QSettings>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QTabWidget>
#include <QTableWidget>
#include <QThread>
#include <QTimer>
#include <QWizard>

#include <cstdio>
#include <cstdlib>
#include <functional>

#include <version.h>

namespace {

const char *const PasswordVariable = "ONESIMUS_SCREENS_PASSWORD";

void fail(const QString &message)
{
    std::fprintf(stderr, "onesimus_screens: %s\n", qPrintable(message));
    std::exit(1);
}

void note(const QString &message)
{
    std::fprintf(stderr, "onesimus_screens: %s\n", qPrintable(message));
}

/// Runs the event loop until pred() holds or the time is up
bool waitFor(const std::function<bool()> &pred, int timeoutMs)
{
    QElapsedTimer timer;
    timer.start();
    while (!pred()) {
        if (timer.elapsed() > timeoutMs)
            return false;
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
        QThread::msleep(10);
    }
    return true;
}

/// Lets queued replies, layouts and repaints happen
void settle(int ms)
{
    waitFor([] { return false; }, ms);
}

/// The home of this run: HOME and every XDG directory below it. Removed and
/// made again, but only when it is ours (the marker file) or does not exist.
QString prepareHome(const QString &requested)
{
    const QString home = QDir::cleanPath(requested);
    if (home.isEmpty() || home == QDir::rootPath())
        fail(QStringLiteral("invalid --home"));
    QDir dir(home);
    const QString marker = home + QStringLiteral("/.onesimus-screens");
    if (dir.exists()) {
        if (!QFileInfo::exists(marker))
            fail(QStringLiteral("%1 exists and was not made by onesimus_screens: refusing to use it").arg(home));
        if (!dir.removeRecursively())
            fail(QStringLiteral("cannot clear %1").arg(home));
    }
    if (!QDir().mkpath(home))
        fail(QStringLiteral("cannot create %1").arg(home));
    QFile m(marker);
    if (!m.open(QIODevice::WriteOnly))
        fail(QStringLiteral("cannot write %1").arg(marker));
    m.write("Made by onesimus_screens; removed by its next run.\n");
    return home;
}

void isolate(const QString &home)
{
    qputenv("HOME", home.toLocal8Bit());
    qputenv("XDG_CONFIG_HOME", (home + "/.config").toLocal8Bit());
    qputenv("XDG_DATA_HOME", (home + "/.local/share").toLocal8Bit());
    qputenv("XDG_CACHE_HOME", (home + "/.cache").toLocal8Bit());
    qputenv("XDG_STATE_HOME", (home + "/.local/state").toLocal8Bit());
    qputenv("XDG_RUNTIME_DIR", (home + "/.run").toLocal8Bit());
    QDir().mkpath(home + "/.run");
    QFile::setPermissions(home + "/.run", QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner);
    // No system tray, notifications or accessibility bus of the desktop
    qunsetenv("DBUS_SESSION_BUS_ADDRESS");
    qputenv("QT_QPA_PLATFORM", "offscreen");
    qputenv("QT_SCALE_FACTOR", "1");
    qunsetenv("QT_SCREEN_SCALE_FACTORS");
    qunsetenv("QT_FONT_DPI");
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    // Mail and file dialogs, if ever opened, find nothing of the user
    qunsetenv("ONESIMUS_PASSWORD");
}

/// Stops when a path of Qt or of the settings is outside the home of this run
void checkIsolation(const QString &home)
{
    const QString prefix = home + QLatin1Char('/');
    QStringList paths;
    paths << QSettings(QStringLiteral("Bareos"), QStringLiteral("Onesimus")).fileName()
          << QSettings().fileName()
          << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
          << QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
          << QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
          << QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
          << QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QLatin1Char('/')
          << QDir::homePath() + QLatin1Char('/');
    for (const QString &p : paths) {
        if (!p.startsWith(prefix))
            fail(QStringLiteral("isolation check failed: %1 is outside %2").arg(p, home));
    }
}

} // namespace

/// Opens views and dialogs of the application and grabs them (a friend of
/// the classes whose views it opens)
class BScreensHarness
{
public:
    struct Options {
        QString outDir;
        QString lang = QStringLiteral("en");
        QSize size{1440, 900};
        QStringList only;
        QString host;
        int port = 0;
        int grpcPort = 0;
        QString console;
        QString director;
        QString password;
        QString fingerprint;
        QDate day;
    };

    explicit BScreensHarness(const Options &o) : m_o(o) {}

    int run();

private:
    bool wanted(const QString &name) const { return m_o.only.isEmpty() || m_o.only.contains(name); }
    bool save(QWidget *w, const QString &name);
    void shotMain(const QString &name, int tab, const std::function<void()> &prepare = {});
    /// A window the action opens (modal or not): prepare() gets it, then it is grabbed and closed
    template<class T>
    void shotWindow(const QString &name, const std::function<void()> &open,
                    const std::function<void(T *)> &prepare, const QSize &size);
    void connectToSimulator();
    bool loaded() const;
    void selectJob(const std::function<bool(const QJsonObject &)> &pred);
    void closeStrayMessageBoxes();

    Options m_o;
    BMainWindow *m_w = nullptr;
    QStringList m_written;
    QStringList m_failed;
    QList<QPointer<QWidget>> m_expected;
};

bool BScreensHarness::save(QWidget *w, const QString &name)
{
    // No blinking text cursor: the same picture on every run
    if (QWidget *focus = QApplication::focusWidget(); focus && qobject_cast<QLineEdit *>(focus))
        focus->clearFocus();
    settle(400);
    const QPixmap pm = w->grab();
    const QString file = QDir(m_o.outDir).filePath(name + QStringLiteral(".png"));
    if (pm.isNull() || !pm.save(file, "PNG")) {
        note(QStringLiteral("%1: could not write %2").arg(name, file));
        m_failed << name;
        return false;
    }
    note(QStringLiteral("%1: %2x%3 -> %4").arg(name).arg(pm.width()).arg(pm.height()).arg(file));
    m_written << name;
    return true;
}

void BScreensHarness::closeStrayMessageBoxes()
{
    for (QWidget *top : QApplication::topLevelWidgets()) {
        auto *box = qobject_cast<QMessageBox *>(top);
        if (!box || !box->isVisible())
            continue;
        bool expected = false;
        for (const auto &e : m_expected)
            expected = expected || e == box;
        if (expected)
            continue;
        note(QStringLiteral("closing a message box: %1 - %2").arg(box->windowTitle(), box->text().left(200)));
        box->reject();
    }
}

void BScreensHarness::connectToSimulator()
{
    // The profile a user would make with the connection wizard: gRPC to the
    // Onesimus Director with the certificate pinned
    BConnectionProfile profile = BConnectionProfile::create(QStringLiteral("Demo Director"));
    profile.description = QStringLiteral("onesimus-sim (invented data)");
    profile.host = m_o.host;
    profile.port = m_o.port;
    profile.directorName = m_o.director;
    profile.consoleName = m_o.console;
    profile.setPasswordFromCleartext(m_o.password);
    profile.transport = BConnectionProfile::Transport::Grpc;
    profile.grpcPort = m_o.grpcPort;
    profile.grpcCertFingerprint = m_o.fingerprint;
    profile.tlsEnabled = true;
    profile.tlsUsePSK = true;

    BSettings &s = BSettings::instance();
    s.addConnectionProfile(profile);
    s.setLastUsedProfileId(profile.id);
    s.setConnectionAutoConnect(true);
    s.sync();

    QMetaObject::invokeMethod(m_w, "onConnectLastUsed", Qt::QueuedConnection);
}

bool BScreensHarness::loaded() const
{
    if (!m_w->m_director->isConnected())
        return false;
    if (m_w->m_jobWidget->tableView()->jobsModel()->rowCount() == 0)
        return false;
    if (m_w->m_clientWidget->model()->rowCount() == 0)
        return false;
    return true;
}

void BScreensHarness::selectJob(const std::function<bool(const QJsonObject &)> &pred)
{
    BJsonJobView *view = m_w->m_jobWidget->tableView();
    QAbstractItemModel *shown = view->model();
    BJobsModel *jobs = view->jobsModel();
    auto *proxy = qobject_cast<QAbstractProxyModel *>(shown);
    for (int row = 0; row < shown->rowCount(); ++row) {
        QModelIndex idx = shown->index(row, 0);
        QModelIndex src = idx;
        // The view may sit on a chain of proxies
        for (auto *p = proxy; p; p = qobject_cast<QAbstractProxyModel *>(p->sourceModel())) {
            src = p->mapToSource(src);
        }
        if (src.model() != jobs)
            continue;
        if (pred(jobs->jobAt(src.row()))) {
            view->selectRow(row);
            view->scrollTo(idx, QAbstractItemView::PositionAtCenter);
            return;
        }
    }
    note(QStringLiteral("no job matched the selection"));
}

void BScreensHarness::shotMain(const QString &name, int tab, const std::function<void()> &prepare)
{
    if (!wanted(name))
        return;
    m_w->m_tabWidget->setCurrentIndex(tab);
    settle(600);
    if (prepare)
        prepare();
    settle(600);
    // Showing or hiding the statistics restores an older size later on
    m_w->resize(m_o.size);
    settle(300);
    save(m_w, name);
}

template<class T>
void BScreensHarness::shotWindow(const QString &name, const std::function<void()> &open,
                                 const std::function<void(T *)> &prepare, const QSize &size)
{
    if (!wanted(name))
        return;
    bool done = false;
    // Runs inside the event loop of a modal dialog as well
    auto shoot = [&]() {
        T *win = nullptr;
        if (!waitFor([&] {
                for (QWidget *top : QApplication::topLevelWidgets()) {
                    if (auto *t = qobject_cast<T *>(top); t && t->isVisible()) {
                        win = t;
                        return true;
                    }
                }
                return false;
            }, 10000)) {
            note(QStringLiteral("%1: the window did not open").arg(name));
            m_failed << name;
            done = true;
            return;
        }
        m_expected << win;
        if (size.isValid())
            win->resize(size);
        settle(300);
        if (prepare)
            prepare(win);
        save(win, name);
        win->close();
        if (auto *d = qobject_cast<QDialog *>(win))
            d->reject();
        done = true;
    };
    QTimer::singleShot(0, m_w, [&]() {
        QTimer::singleShot(0, m_w, shoot);
        open();
    });
    waitFor([&] { return done; }, 120000);
    settle(300);
}

int BScreensHarness::run()
{
    QDir().mkpath(m_o.outDir);

    BSettings &s = BSettings::instance();
    s.setAppearanceTheme(QStringLiteral("dark"));
    s.setAppearanceLanguage(m_o.lang);
    s.setAppearanceUiZoom(100);
    s.setStatisticsWidgetVisible(true);
    s.setMainWindowActiveTab(0);
    s.setBehaviorAutoRefresh(false);
    s.setBehaviorJobsNewestFirst(true);
    s.sync();

    BTranslations::instance()->setLanguage(BTranslations::resolve(m_o.lang));

    BMainWindow w;
    m_w = &w;
    w.resize(m_o.size);
    w.show();
    w.resize(m_o.size);

    // A message box nobody asked for (a notice, an error) must not stop the run
    QTimer strays;
    QObject::connect(&strays, &QTimer::timeout, [this]() { closeStrayMessageBoxes(); });
    strays.start(500);

    connectToSimulator();
    if (!waitFor([this] { return loaded(); }, 60000))
        fail(QStringLiteral("no data from the simulator at %1:%2 (gRPC)").arg(m_o.host).arg(m_o.grpcPort));
    note(QStringLiteral("connected, %1 jobs").arg(w.m_jobWidget->tableView()->jobsModel()->rowCount()));
    // The rest of the replies (resources, volumes, history) after the first rows
    settle(4000);

    BJobWidget *jobs = w.m_jobWidget;

    // The job list: newest on top, the filters folded away for more columns
    if (jobs->m_toggleFiltersButton->isCheckable())
        jobs->m_toggleFiltersButton->setChecked(false);
    jobs->do_toggleFilters(false);
    jobs->tableView()->sortByColumn(1, Qt::DescendingOrder);   // Job ID
    auto status = [](const QJsonObject &j) { return j.value("jobstatus").toString(); };

    // 1. overview: the job list with the statistics dock, the newest finished job selected
    shotMain(QStringLiteral("overview"), 0, [&]() {
        w.m_statisticsDock->setVisible(true);
        jobs->setLogViewVisible(true);
        selectJob([&](const QJsonObject &j) { return status(j) == QLatin1String("T"); });
        settle(2000);
    });

    // 2. job-log: a running job with its live log (the Full to tape, if it runs)
    shotMain(QStringLiteral("job-log"), 0, [&]() {
        w.m_statisticsDock->setVisible(false);
        jobs->setLogViewVisible(true);
        bool tape = false;
        selectJob([&](const QJsonObject &j) {
            tape = status(j) == QLatin1String("R") && j.value("name").toString() == QLatin1String("acme-files-offsite");
            return tape;
        });
        if (!tape)
            selectJob([&](const QJsonObject &j) { return status(j) == QLatin1String("R"); });
        settle(5000);
    });
    w.m_statisticsDock->setVisible(true);

    // 3. clients
    shotMain(QStringLiteral("clients"), 1, [&]() { settle(1500); });

    // 4. storage: pools and volumes
    shotMain(QStringLiteral("storage"), 2, [&]() {
        waitFor([&] { return w.m_storageWidget->volumeTable()->rowCount() > 0; }, 15000);
    });

    // 5.-7. schedules: the timeline, the grid editor and the stagger proposal
    BScheduleWidget *sched = w.m_scheduleWidget;
    auto openSchedules = [&]() {
        w.m_statisticsDock->setVisible(false);
        w.m_tabWidget->setCurrentIndex(3);
        waitFor([&] { return sched->m_historyState == BScheduleWidget::HistoryLoaded
                             || sched->m_historyState == BScheduleWidget::HistoryUnavailable; }, 30000);
        settle(1500);
        if (m_o.day.isValid())
            sched->setCurrentDate(m_o.day);
    };
    shotMain(QStringLiteral("schedules"), 3, [&]() {
        openSchedules();
        sched->m_viewModeCombo->setCurrentIndex(0);
        sched->m_waitButton->setChecked(true);
        sched->showLowerPage(sched->m_waitPanel, true);
    });
    shotMain(QStringLiteral("schedule-editor"), 3, [&]() {
        openSchedules();
        // A schedule with several levels
        for (int i = 0; i < sched->m_scheduleList->count(); ++i) {
            if (sched->m_scheduleList->item(i)->text().startsWith(QLatin1String("WeeklyCycle"))) {
                sched->m_scheduleList->setCurrentRow(i);
                break;
            }
        }
        sched->m_viewModeCombo->setCurrentIndex(1);
    });
    shotMain(QStringLiteral("schedule-stagger"), 3, [&]() {
        openSchedules();
        sched->m_viewModeCombo->setCurrentIndex(0);
        sched->showLowerPage(sched->m_staggerPage, true);
        waitFor([&] { return !sched->m_waitRuns.isEmpty(); }, 15000);
        settle(1000);
        sched->proposeStagger();
        settle(1000);
        if (sched->m_staggerTable->rowCount() > 0)
            sched->m_staggerTable->selectRow(0);
    });
    w.m_statisticsDock->setVisible(true);
    w.m_tabWidget->setCurrentIndex(0);
    settle(500);

    // 8. acl-matrix
    shotWindow<BAclMatrixDialog>(QStringLiteral("acl-matrix"), [&]() { w.showAclMatrix(); },
                                 [](BAclMatrixDialog *) { settle(3000); }, QSize(900, 760));

    // 9. purge: prune & purge with the analysis (a dry run of the cleanup)
    shotWindow<BCleanupDialog>(QStringLiteral("purge"), [&]() { w.onCleanupDatabase(); },
        [](BCleanupDialog *d) {
            d->m_removeOldFullBackupsCheck->setChecked(true);
            d->m_removeFailedJobsCheck->setChecked(true);
            d->m_pruneVolumesCheck->setChecked(true);
            if (d->m_purgeAvailable)
                d->m_volumeActionCombo->setCurrentIndex(1);
            settle(500);
            d->onAnalyzeClicked();
            settle(2500);
        }, QSize(1100, 900));

    // 10. restore: the wizard of a finished backup
    shotWindow<BRestoreWizard>(QStringLiteral("restore"), [&]() {
            selectJob([](const QJsonObject &j) {
                return j.value("jobstatus").toString() == QLatin1String("T")
                       && j.value("type").toString() == QLatin1String("B");
            });
            QMetaObject::invokeMethod(jobs->tableView(), "restoreFiles", Qt::DirectConnection);
        },
        [](BRestoreWizard *) { settle(4000); }, QSize(1100, 760));

    // 11. check-321
    shotWindow<B321Dialog>(QStringLiteral("check-321"), [&]() {
            for (QAction *a : w.m_toolsMenu->actions()) {
                if (a->text().contains(QLatin1String("3-2-1"))) {
                    a->trigger();
                    return;
                }
            }
            note(QStringLiteral("check-321: no menu entry"));
        },
        [](B321Dialog *) { settle(5000); }, QSize(1200, 780));

    // 12. connect: the connection wizard, on the page of the console
    // (not the server page: it searches the network with mDNS)
    {
        BConnectionWizardData data;
        data.host = m_o.host;
        data.port = m_o.port;
        data.transport = QStringLiteral("grpc");
        data.grpcPort = m_o.grpcPort;
        data.directorName = m_o.director;
        data.consoleName = m_o.console;
        data.profileName = QStringLiteral("Demo Director");
        BConnectionWizard *wizard = nullptr;
        shotWindow<BConnectionWizard>(QStringLiteral("connect"), [&]() {
                wizard = new BConnectionWizard(&data, nullptr, &w);
                wizard->setAttribute(Qt::WA_DeleteOnClose);
                wizard->setStartId(BConnectionWizard::Page_Credentials);
                wizard->show();
            },
            [&](BConnectionWizard *wz) {
                // The demo password, so that the page looks filled in (shown as dots)
                for (auto *edit : wz->currentPage()->findChildren<QLineEdit *>()) {
                    if (edit->echoMode() == QLineEdit::Password)
                        edit->setText(m_o.password);
                }
                settle(800);
            }, QSize(780, 520));
    }

    // 13. settings: the appearance page (language, theme)
    shotWindow<BSettingsDialog>(QStringLiteral("settings"), [&]() { w.onSettingsTriggered(); },
        [](BSettingsDialog *d) {
            // The category list: Connection, Appearance, Behavior, Advanced
            for (auto *list : d->findChildren<QListWidget *>()) {
                if (list->count() == 4 && list->parentWidget() == d)
                    list->setCurrentRow(1);
            }
            settle(500);
        }, QSize(960, 700));

    note(QStringLiteral("written: %1").arg(m_written.join(' ')));
    if (!m_failed.isEmpty())
        note(QStringLiteral("failed: %1").arg(m_failed.join(' ')));
    strays.stop();
    m_w = nullptr;
    return m_failed.isEmpty() ? 0 : 3;
}

int main(int argc, char *argv[])
{
    // Before Qt reads a single path: the home of this run
    QString home = QStringLiteral("/tmp/onesimus-screens/demo");
    for (int i = 1; i + 1 < argc; ++i) {
        if (qstrcmp(argv[i], "--home") == 0)
            home = QString::fromLocal8Bit(argv[i + 1]);
    }
    home = prepareHome(home);
    isolate(home);

    QApplication app(argc, argv);
    app.setApplicationName(PROJECT_NAME);
    app.setApplicationVersion(PROJECT_VERSION);
    app.setOrganizationName("bareos");
    app.setOrganizationDomain("bareos.org");
    // The font of a GNOME desktop, when it is there (offscreen has no desktop font)
    QFont font(QStringLiteral("Cantarell"));
    font.setPointSize(10);
    if (QFontInfo(font).family() == QLatin1String("Cantarell"))
        QApplication::setFont(font);
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, home + "/.config");
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, home + "/.config");
    checkIsolation(home);

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral(
        "Writes the website screenshots of Onesimus from the simulator onesimus-sim.\n"
        "The console password is read from the environment variable %1.").arg(PasswordVariable));
    parser.addHelpOption();
    QCommandLineOption outOpt("out", "Directory for the PNG files", "dir");
    QCommandLineOption langOpt("lang", "Language: en or de", "lang", "en");
    QCommandLineOption sizeOpt("size", "Size of the main window", "WxH", "1440x900");
    QCommandLineOption onlyOpt("only", "Only these shots (comma separated)", "names");
    QCommandLineOption hostOpt("host", "Simulator address (127.0.0.1 only)", "host", "127.0.0.1");
    QCommandLineOption portOpt("port", "Director (BNET) port of the profile", "port", "11100");
    QCommandLineOption grpcOpt("grpc-port", "gRPC port of the simulator", "port", "11101");
    QCommandLineOption consoleOpt("console", "Console name", "name", "admin");
    QCommandLineOption directorOpt("director", "Director name", "name", "demo-dir");
    QCommandLineOption fpOpt("fingerprint", "SHA-256 fingerprint of the simulator's certificate (hex)", "hex");
    QCommandLineOption dayOpt("day", "Day shown in the schedule timeline (yyyy-MM-dd)", "date");
    QCommandLineOption homeOpt("home", "Home of this run (removed and made again)", "dir");
    for (const auto &o : {outOpt, langOpt, sizeOpt, onlyOpt, hostOpt, portOpt, grpcOpt, consoleOpt,
                          directorOpt, fpOpt, dayOpt, homeOpt})
        parser.addOption(o);
    parser.process(app);

    BScreensHarness::Options o;
    o.outDir = parser.value(outOpt);
    if (o.outDir.isEmpty())
        fail(QStringLiteral("--out is required"));
    o.lang = parser.value(langOpt);
    if (o.lang != QLatin1String("en") && o.lang != QLatin1String("de"))
        fail(QStringLiteral("--lang must be en or de"));
    const QStringList wh = parser.value(sizeOpt).split('x');
    if (wh.size() != 2)
        fail(QStringLiteral("--size must be WxH"));
    o.size = QSize(wh[0].toInt(), wh[1].toInt());
    if (parser.isSet(onlyOpt))
        o.only = parser.value(onlyOpt).split(',', Qt::SkipEmptyParts);
    o.host = parser.value(hostOpt);
    // The simulator only: never a Director somewhere else
    if (QHostAddress(o.host) != QHostAddress(QHostAddress::LocalHost))
        fail(QStringLiteral("--host must be 127.0.0.1"));
    o.port = parser.value(portOpt).toInt();
    o.grpcPort = parser.value(grpcOpt).toInt();
    o.console = parser.value(consoleOpt);
    o.director = parser.value(directorOpt);
    o.fingerprint = parser.value(fpOpt).remove(':').toLower();
    if (o.fingerprint.size() != 64)
        fail(QStringLiteral("--fingerprint must be the 64 hex digits of the SHA-256 fingerprint"));
    o.password = qEnvironmentVariable(PasswordVariable);
    if (o.password.isEmpty())
        fail(QStringLiteral("set the console password in %1").arg(PasswordVariable));
    if (parser.isSet(dayOpt)) {
        o.day = QDate::fromString(parser.value(dayOpt), Qt::ISODate);
        if (!o.day.isValid())
            fail(QStringLiteral("--day must be yyyy-MM-dd"));
    }

    BScreensHarness harness(o);
    return harness.run();
}
