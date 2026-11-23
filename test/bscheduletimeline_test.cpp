// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bscheduletimeline_test.cpp
 * @brief The timeline of the schedule tab as a whole (BScheduleWidget)
 *
 * - A bar that is dragged and dropped over a connection without the
 *   configuration methods (BNET, plain Bareos) writes nothing: no command
 *   leaves the widget, the capability gate is asked, the status line says
 *   that the schedule was not changed, and the bar is back at its time.
 * - Over gRPC with the configuration methods (a fake Director here) a drop
 *   updates the same schedule: read, check, review, write; never a new
 *   schedule, never "configure add". "Save..." of the grid writes the grid's
 *   changes the same way; "Delete..." of a schedule a job uses deletes
 *   nothing. A schedule with a Run line that is not understood is locked.
 * - Schedules that no enabled job uses are left out of the prediction, the
 *   counter and the list, and are marked for the timeline.
 * - The counter says whether its list is open.
 * - Waits of seconds are shown with seconds and sorted by their length.
 *
 * The "show" replies are written out from the Bareos sources (API mode 2,
 * see bschedulewaitmodel_test.cpp); nothing was captured from a Director.
 * Runs offscreen, without a Director; the settings are redirected to a
 * temporary directory before BSettings is used.
 */

#include <QtTest>
#include <QScopeGuard>
#include <QSemaphore>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QWizard>
#include <QMessageBox>
#include <QListWidget>
#include <QLineEdit>

#include <QCheckBox>
#include <QComboBox>
#include <QScrollArea>
#include <QHeaderView>
#include <QDateEdit>
#include <QSplitter>
#include <QToolButton>
#include <QGroupBox>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QSignalSpy>
#include <QTableWidget>
#include <QCheckBox>
#include <QTimeEdit>
#include <QTemporaryDir>
#include <QUndoStack>

#include "buiscale.h"
#include "config/bsettings.h"
#include "models/bresourcemodels.h"
#include "schedules/bscheduleganttwidget.h"
#include "schedules/bschedulegrideditor.h"
#include "schedules/bschedulegridview.h"
#include "schedules/bschedulelegend.h"
#include "schedules/bschedulereviewdialog.h"
#include "schedules/bschedulewidget.h"
#include "schedules/bstaggerconstraintsdialog.h"
#include "schedules/bstaggerplan.h"
#include "schedules/model/bscheduleresource.h"

#include "bfakeconfigclient.h"
#include "bfakesaveui.h"

namespace {

const char *const kStorages =
    R"({"result":{"storages":{"File":{"name":"File","address":"sd","maximumconcurrentjobs":10}}}})";
const char *const kDirector =
    R"({"result":{"directors":{"test-dir":{"name":"test-dir","maximumconcurrentjobs":20}}}})";

/// The schedule tab with its job model, shown, in the timeline's day view
struct Tab
{
    BJobConfigModel jobs;
    BScheduleWidget widget;

    Tab()
    {
        widget.resize(1400, 900);
        widget.setJobConfigModel(&jobs);
        widget.show();
        // View: timeline; day view
        const QList<QComboBox *> combos = widget.findChildren<QComboBox *>();
        for (QComboBox *combo : combos) {
            if (combo->findText(QStringLiteral("Timeline")) >= 0)
                combo->setCurrentIndex(combo->findText(QStringLiteral("Timeline")));
            if (combo->findText(QStringLiteral("Week")) >= 0)
                combo->setCurrentIndex(combo->findText(QStringLiteral("Day")));
        }
        QCoreApplication::processEvents();
    }

    void feed(const QString &schedules, const QString &showJobs, const QString &jobDefs,
              const QString &clients)
    {
        widget.processShowSchedulesResponse(schedules);
        jobs.parseShowJobs(showJobs);
        jobs.parseShowJobDefs(jobDefs);
        widget.processShowJobsResponse(QString());
        widget.processShowStoragesResponse(QString::fromLatin1(kStorages));
        widget.processShowDirectorResponse(QString::fromLatin1(kDirector));
        widget.processShowClientsResponse(clients);
        QCoreApplication::processEvents();
        settle();
    }

    /// The wait prediction runs in a worker thread: until its result is in
    /// and the views have been laid out for it (the counter elides its text
    /// to the width it had before; the new width comes with the layout)
    void settle()
    {
        QTRY_VERIFY_WITH_TIMEOUT(widget.waitPredictionIdle(), 10000);
        QCoreApplication::sendPostedEvents();
        QCoreApplication::processEvents();
    }

    /// The timeline of the clients (the upper one)
    BScheduleGanttWidget *timeline() const
    {
        return widget.findChildren<BScheduleGanttWidget *>().value(0);
    }

    QPushButton *counter() const
    {
        const QList<QPushButton *> buttons = widget.findChildren<QPushButton *>();
        for (QPushButton *button : buttons) {
            if (button->property("disclosure").toBool()) return button;
        }
        return nullptr;
    }

    /// The tab of the predicted waits below the timeline
    QGroupBox *waitPanel() const
    {
        return widget.findChild<QGroupBox *>(QStringLiteral("predictedWaits"));
    }

    QTableWidget *waitTable() const { return waitPanel()->findChild<QTableWidget *>(); }

    QStringList column(int column) const
    {
        QStringList cells;
        const QTableWidget *table = waitTable();
        for (int row = 0; row < table->rowCount(); ++row)
            cells << (table->item(row, column) ? table->item(row, column)->text() : QString());
        return cells;
    }

    QString panelText() const
    {
        QStringList lines;
        const QList<QLabel *> labels = waitPanel()->findChildren<QLabel *>();
        for (const QLabel *label : labels)
            lines << label->text();
        return lines.join(QLatin1Char('\n'));
    }
};

/// Where the bar of a run at this time of the day begins, in day view
QPoint barStart(const BScheduleGanttWidget *timeline, int row, int hour, int minute)
{
    const BScheduleGanttWidget::Metrics m = timeline->metrics();
    const int x = m.labelWidth + static_cast<int>((hour + minute / 60.0) * timeline->zoomLevel());
    return QPoint(x + 3, m.headerHeight + row * m.rowHeight + m.rowHeight / 2);
}

/// Press on a bar, drag it by so many pixels, drop it
void dragBar(BScheduleGanttWidget *timeline, const QPoint &from, int dx)
{
    QTest::mousePress(timeline, Qt::LeftButton, Qt::NoModifier, from);
    QTest::mouseMove(timeline, from + QPoint(dx / 2, 0));
    QTest::mouseMove(timeline, from + QPoint(dx, 0));
    QTest::mouseRelease(timeline, Qt::LeftButton, Qt::NoModifier, from + QPoint(dx, 0));
    QCoreApplication::processEvents();
}

const char *const kWeeklyCycle = R"({"result":{"schedules":{
    "WeeklyCycle":{"name":"WeeklyCycle","run":["Level=Incremental daily at 08:00"]}}}})";
const char *const kOneJob = R"({"result":{"jobs":{
    "BackupEleLa":{"name":"BackupEleLa","type":"Backup","schedule":"WeeklyCycle",
                   "client":"elela-fd","storage":["File"]}}}})";
const char *const kNoJobDefs = R"({"result":{"jobdefs":{}}})";
const char *const kOneClient = R"({"result":{"clients":{"elela-fd":{"name":"elela-fd","address":"x"}}}})";

} // namespace

class BScheduleTimelineTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    // --- A dropped bar writes nothing ---
    void timelineReportsTheDropAndSnapsBack();
    void dropSendsNoCommandAndAsksTheGate();
    void dropWritesNothingEvenIfTheGateAllows();
    void dropWithoutGateSaysSoAsWell();

    // --- Writing over gRPC (a fake Director) ---
    void dropOnGrpcUpdatesTheSameSchedule();
    void dropOnBnetSendsNothingEvenWithAClient();
    void dropOnALockedScheduleIsRefused();
    void gridSaveWritesAndStartsAfresh();
    void gridSaveWritesTheDescription();
    void clickOnTheListsAnswers();
    void cancelIsLeftInEveryDialog();
    void deleteOfAScheduleInUseDeletesNothing();

    // --- Copy, rename, move of one job ---
    void scheduleNamesAndRenamedText();
    void copyCreatesTheSameRunLines();
    void copyRefusesBadNamesAndNeedsCreate();
    void renameMovesJobsAndDeletesTheOld();
    void renameRefusals();
    void dropOfASharedScheduleMovesOneJob();
    void dropOfASharedScheduleForAllOrCancelled();
    void moveOfOneJobStopsAtTheFirstRefusal();

    // --- Schedules no job uses ---
    void unusedSchedulesAreLeftOut();

    // --- The counter and its list ---
    void counterSaysWhetherTheListIsOpen();
    void themedToolbarCutsNoButtonAndWeekHasCalendar();
    void handleBelowTimelineMovesUpAndIsKept();
    void handleBesideWaitsIsKept();
    void waitsOfSecondsInTheList();
    void staggerIsWrittenAfterOneConfirmation();
    void staggerNotConfirmedWritesNothing();
    void staggerFollowsTheStoredConstraints();
    void staggerConstraintsDialogKeepsTheOthers();

    // --- The review ---
    void reviewTextCanBeEdited();

    // --- The legend ---
    void legendExplainsEveryMark();

    // --- Measured against planned ---
    void measuredRunsBesideThePlannedBars();
    void gridLoadCountsTheStartsOfAllSchedules();
    void columnWidthsOfTheTablesAreKept();

    // --- Live overlay (push events over gRPC) ---
    void liveOverlayFollowsTheEvents();
    void liveOverlayIsThrottled();
    void liveOverlayOffOnBnet();
    void liveOverlaySeededFromTheHistory();
    void liveJobFindsTheRowOfItsRun();

    // --- The wait prediction in its worker thread ---
    void waitPredictionDropsResultsOfOlderInputs();
    void waitsWaitForTheHistoryLoadedAfterTheConnect();

private:
    QTemporaryDir m_config;
};

void BScheduleTimelineTest::initTestCase()
{
    // The settings of the test, not those of the user: before BSettings is
    // used for the first time
    QVERIFY(m_config.isValid());
    qputenv("XDG_CONFIG_HOME", m_config.path().toUtf8());
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, m_config.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_config.path());
    // Windows keeps NativeFormat in the registry, where setPath does not reach
    QSettings::setDefaultFormat(QSettings::IniFormat);
    const QSettings probe(QSettings::defaultFormat(), QSettings::UserScope,
                         QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
    QVERIFY2(probe.fileName().startsWith(m_config.path()), qPrintable(probe.fileName()));
}

// ============================================================================
// A dropped bar writes nothing
// ============================================================================

void BScheduleTimelineTest::timelineReportsTheDropAndSnapsBack()
{
    Tab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    BScheduleGanttWidget *timeline = tab.timeline();
    QVERIFY(timeline);
    QCOMPARE(timeline->entries().size(), 1);
    QCOMPARE(timeline->entries()[0].jobName, QString("BackupEleLa"));

    QSignalSpy dropped(timeline, &BScheduleGanttWidget::dragDropped);
    QSignalSpy started(timeline, &BScheduleGanttWidget::dragStarted);

    // Row 0 is the group of the client, row 1 the run. Three hours later.
    const QPoint from = barStart(timeline, 1, 8, 0);
    dragBar(timeline, from, 3 * timeline->zoomLevel());

    QCOMPARE(started.count(), 1);
    QCOMPARE(dropped.count(), 1);
    QCOMPARE(dropped[0][0].toInt(), 0);
    QCOMPARE(dropped[0][1].toInt(), 11);
    // The entry is what it was: the widget changes nothing
    QCOMPARE(timeline->entries()[0].hour, 8);
    QCOMPARE(timeline->entries()[0].minute, 0);
    QCOMPARE(timeline->entries()[0].source.original, QString("Level=Incremental daily at 08:00"));

    // A click, and a drag that ends at the time it began, are not a drop
    QTest::mouseClick(timeline, Qt::LeftButton, Qt::NoModifier, from);
    QTest::mousePress(timeline, Qt::LeftButton, Qt::NoModifier, from);
    QTest::mouseMove(timeline, from + QPoint(200, 0));
    QTest::mouseMove(timeline, from);
    QTest::mouseRelease(timeline, Qt::LeftButton, Qt::NoModifier, from);
    QCOMPARE(dropped.count(), 1);

    // Escape ends a drag without a drop
    QTest::mousePress(timeline, Qt::LeftButton, Qt::NoModifier, from);
    QTest::mouseMove(timeline, from + QPoint(200, 0));
    QTest::keyClick(timeline, Qt::Key_Escape);
    QTest::mouseRelease(timeline, Qt::LeftButton, Qt::NoModifier, from + QPoint(200, 0));
    QCOMPARE(dropped.count(), 1);
}

void BScheduleTimelineTest::dropSendsNoCommandAndAsksTheGate()
{
    Tab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    BScheduleGanttWidget *timeline = tab.timeline();

    // The gate of the main window shows the capability notice
    // (BCapabilityNotice::check()) and answers "not available"
    int asked = 0;
    QWidget *askedFor = nullptr;
    tab.widget.setScheduleChangeGate([&](QWidget *parent) {
        ++asked;
        askedFor = parent;
        return false;
    });

    QSignalSpy commands(&tab.widget, &BScheduleWidget::sendCommand);
    QSignalSpy status(&tab.widget, &BScheduleWidget::statusMessageChanged);
    QVERIFY(commands.isValid());

    dragBar(timeline, barStart(timeline, 1, 8, 0), 3 * timeline->zoomLevel());
    QTest::qWait(50);   // nothing is sent later either

    // Not "configure add", not anything
    QCOMPARE(commands.count(), 0);
    QCOMPARE(asked, 1);
    QCOMPARE(askedFor, &tab.widget);
    QCOMPARE(status.count(), 1);
    const QString message = status[0][0].toString();
    QVERIFY2(message.contains("Schedule WeeklyCycle was not changed"), qPrintable(message));
    QVERIFY2(message.contains("gRPC interface of the Onesimus Director (onesimus-dird)"),
             qPrintable(message));
    QVERIFY2(message.contains("not available on this connection"), qPrintable(message));
    QVERIFY2(!message.contains("configure"), qPrintable(message));
    QVERIFY2(message.contains("BackupEleLa stays at 08:00 (dropped at 11:00)"), qPrintable(message));
    QVERIFY2(!message.contains("-modified"), qPrintable(message));

    // The schedule and its run are what they were
    QCOMPARE(tab.widget.scheduleModel()->scheduleNames(), QStringList({"WeeklyCycle"}));
    QCOMPARE(tab.widget.scheduleModel()->runDirectives("WeeklyCycle"),
             QStringList({"Level=Incremental daily at 08:00"}));
    QCOMPARE(timeline->entries()[0].hour, 8);

    // The lower timeline (storages) is the same: asked again, said again
    BScheduleGanttWidget *storages = tab.widget.findChildren<BScheduleGanttWidget *>().value(1);
    QVERIFY(storages && storages != timeline);
    dragBar(storages, barStart(storages, 1, 8, 0), 2 * storages->zoomLevel());
    QCOMPARE(commands.count(), 0);
    QCOMPARE(asked, 2);
    QCOMPARE(status.count(), 2);
}

void BScheduleTimelineTest::dropWritesNothingEvenIfTheGateAllows()
{
    // A gate that says yes, but no configuration methods (no client, as on
    // BNET): nothing is sent and the user is told
    Tab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    int asked = 0;
    tab.widget.setScheduleChangeGate([&](QWidget *) { ++asked; return true; });
    QSignalSpy commands(&tab.widget, &BScheduleWidget::sendCommand);
    QSignalSpy status(&tab.widget, &BScheduleWidget::statusMessageChanged);

    dragBar(tab.timeline(), barStart(tab.timeline(), 1, 8, 0), 3 * tab.timeline()->zoomLevel());
    QTest::qWait(50);

    QCOMPARE(asked, 1);
    QCOMPARE(commands.count(), 0);
    QCOMPARE(status.count(), 1);
    QVERIFY(status[0][0].toString().contains("was not changed"));
}

void BScheduleTimelineTest::dropWithoutGateSaysSoAsWell()
{
    Tab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    QSignalSpy commands(&tab.widget, &BScheduleWidget::sendCommand);
    QSignalSpy status(&tab.widget, &BScheduleWidget::statusMessageChanged);

    dragBar(tab.timeline(), barStart(tab.timeline(), 1, 8, 0), 3 * tab.timeline()->zoomLevel());

    QCOMPARE(commands.count(), 0);
    QCOMPARE(status.count(), 1);
    QVERIFY(status[0][0].toString().contains("was not changed"));
}

// ============================================================================
// Writing over gRPC (a fake Director)
// ============================================================================

namespace {

const QString kWeeklyCycleText = QStringLiteral(
    "Schedule {\n"
    "  Name = \"WeeklyCycle\"\n"
    "  Run = Level=Incremental daily at 08:00   # mornings\n"
    "}\n");

/// The tab with a fake Director behind the configuration methods
struct GrpcTab : Tab
{
    BFakeConfigClient client;
    BFakeSaveUi ui{&client};
    QSignalSpy finished{widget.persistence(), &BSchedulePersistence::finished};

    GrpcTab()
    {
        client.files.insert(QStringLiteral("WeeklyCycle"), kWeeklyCycleText);
        widget.setConfigClient(&client);
        widget.setSaveUi(&ui);
    }
    ~GrpcTab() { widget.setConfigClient(nullptr); widget.setSaveUi(nullptr); }

    bool wait() { return !finished.isEmpty() || finished.wait(3000); }
    BScheduleReport report() const { return finished.last().at(0).value<BScheduleReport>(); }
};

/// Three jobs on one client that takes one job at a time, measured at 35 s
/// and started 2 s after their time on four days: two of them wait
void feedThreeWaitingJobs(Tab &tab)
{
    const QStringList names = {"JobA", "JobB", "JobC"};
    QString history = R"({"jsonrpc":"2.0","id":null,"result":{"jobs":[)";
    int id = 100;
    for (int day = 1; day <= 4; ++day) {
        for (const QString &name : names) {
            const QDateTime sched(QDate::currentDate().addDays(-day), QTime(8, 0));
            const QDateTime start = sched.addSecs(2);
            const QString f = QStringLiteral("yyyy-MM-dd HH:mm:ss");
            history += QString(R"(%1{"jobid":"%2","name":"%3","type":"B","level":"I","jobstatus":"T",)"
                               R"("schedtime":"%4","starttime":"%5","endtime":"%6","realendtime":"%6"})")
                           .arg(id == 100 ? "" : ",").arg(id).arg(name)
                           .arg(sched.toString(f), start.toString(f), start.addSecs(35).toString(f));
            ++id;
        }
    }
    history += "]}}";
    QObject::connect(&tab.widget, &BScheduleWidget::sendCommand, &tab.widget,
                     [&tab, history](BDirector::Command cmd, const QString &args) {
        if (cmd == BDirector::Command::LlistJobs) {
            QTimer::singleShot(0, &tab.widget, [&tab, args, history]() {
                tab.widget.processJobHistoryReply(args, history);
            });
        }
    });
    tab.widget.setConnectionState(true);
    QString showJobs = R"({"result":{"jobs":{)";
    for (int i = 0; i < names.size(); ++i) {
        showJobs += QString(R"(%1"%2":{"name":"%2","type":"Backup","schedule":"WeeklyCycle",)"
                            R"("client":"elela-fd","storage":["File"],"maximumconcurrentjobs":1})")
                        .arg(i ? "," : "", names[i]);
    }
    showJobs += "}}}";
    tab.feed(kWeeklyCycle, showJobs, kNoJobDefs, kOneClient);
}

/// The stagger tab, proposed: its Apply button
QPushButton *proposeStagger(Tab &tab)
{
    QElapsedTimer waited;
    waited.start();
    while (!tab.counter()->accessibleName().contains("2 run(s) wait") && waited.elapsed() < 15000)
        QTest::qWait(20);
    QWidget *stagger = tab.widget.findChild<QWidget *>(QStringLiteral("stagger"));
    if (!stagger) return nullptr;
    QPushButton *apply = nullptr;
    for (QPushButton *b : stagger->findChildren<QPushButton *>()) {
        if (b->text() == QLatin1String("Propose later start times")) b->click();
        if (b->text() == QLatin1String("Apply...")) apply = b;
    }
    return apply;
}

/// Proposes again and returns the rows of the stagger table: job, later
/// by, note
QList<QStringList> staggerRows(Tab &tab)
{
    QWidget *stagger = tab.widget.findChild<QWidget *>(QStringLiteral("stagger"));
    QList<QStringList> rows;
    if (!stagger) return rows;
    for (QPushButton *b : stagger->findChildren<QPushButton *>()) {
        if (b->text() == QLatin1String("Propose later start times")) b->click();
    }
    QTableWidget *table = stagger->findChild<QTableWidget *>();
    for (int row = 0; table && row < table->rowCount(); ++row)
        rows.append({table->item(row, 0)->text(), table->item(row, 4)->text(), table->item(row, 7)->text()});
    return rows;
}

int movedRows(const QList<QStringList> &rows)
{
    int n = 0;
    for (const QStringList &row : rows) {
        if (!row.at(1).isEmpty()) ++n;
    }
    return n;
}

} // namespace

void BScheduleTimelineTest::dropOnGrpcUpdatesTheSameSchedule()
{
    GrpcTab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    int asked = 0;
    tab.widget.setScheduleChangeGate([&](QWidget *) { ++asked; return true; });
    QSignalSpy commands(&tab.widget, &BScheduleWidget::sendCommand);

    dragBar(tab.timeline(), barStart(tab.timeline(), 1, 8, 0), 3 * tab.timeline()->zoomLevel());
    QVERIFY(tab.wait());
    QCOMPARE(asked, 1);
    QCOMPARE(tab.report().kind, BScheduleReport::Written);

    // Read, check, write - of WeeklyCycle; nothing created, no command
    QCOMPARE(tab.client.requests.size(), 3);
    QCOMPARE(tab.client.requests[0].kind, BConfig::Request::Read);
    QCOMPARE(tab.client.requests[1].kind, BConfig::Request::Update);
    QVERIFY(tab.client.requests[1].validateOnly);
    QCOMPARE(tab.client.writes().size(), 1);
    const BConfig::Request write = tab.client.writes().first();
    QCOMPARE(write.kind, BConfig::Request::Update);
    QCOMPARE(write.name, QStringLiteral("WeeklyCycle"));
    QCOMPARE(write.expectedSha256, BConfig::sha256Hex(kWeeklyCycleText));
    for (const BConfig::Request &request : std::as_const(tab.client.requests))
        QVERIFY(request.kind != BConfig::Request::Create);
    QCOMPARE(commands.count(), 0);

    // The new time, the comment kept
    QStringList values;
    QVERIFY(BScheduleResource::runValues(write.text, &values));
    QCOMPARE(values.size(), 1);
    QVERIFY2(values.first().contains(QStringLiteral("at 11:00")), qPrintable(values.first()));
    QVERIFY(write.text.contains(QStringLiteral("   # mornings\n")));

    // The review named the job and the move
    QCOMPARE(tab.ui.reviews.size(), 1);
    QCOMPARE(tab.ui.reviews.first().jobs, QStringList({QStringLiteral("BackupEleLa")}));
    QVERIFY(tab.ui.reviews.first().note.contains(QStringLiteral("from 08:00 to 11:00")));
}

void BScheduleTimelineTest::dropOnBnetSendsNothingEvenWithAClient()
{
    // The gate answers as BCapabilityNotice::check() does on BNET: no
    GrpcTab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    tab.widget.setScheduleChangeGate([](QWidget *) { return false; });
    QSignalSpy commands(&tab.widget, &BScheduleWidget::sendCommand);
    QSignalSpy status(&tab.widget, &BScheduleWidget::statusMessageChanged);

    dragBar(tab.timeline(), barStart(tab.timeline(), 1, 8, 0), 3 * tab.timeline()->zoomLevel());
    QTest::qWait(50);
    QVERIFY(tab.client.requests.isEmpty());
    QCOMPARE(commands.count(), 0);
    QCOMPARE(status.count(), 1);
    QVERIFY(status[0][0].toString().contains(QStringLiteral("was not changed")));
    QCOMPARE(tab.client.files.value(QStringLiteral("WeeklyCycle")), kWeeklyCycleText);
}

void BScheduleTimelineTest::dropOnALockedScheduleIsRefused()
{
    GrpcTab tab;
    const char *locked = R"({"result":{"schedules":{
        "WeeklyCycle":{"name":"WeeklyCycle","run":["Level=Incremental daily at 08:00",
                                                    "Level=Full every other sat at 21:00"]}}}})";
    tab.feed(locked, kOneJob, kNoJobDefs, kOneClient);
    tab.widget.setScheduleChangeGate([](QWidget *) { return true; });
    QSignalSpy status(&tab.widget, &BScheduleWidget::statusMessageChanged);

    dragBar(tab.timeline(), barStart(tab.timeline(), 1, 8, 0), 3 * tab.timeline()->zoomLevel());
    QTest::qWait(50);
    QVERIFY(tab.client.requests.isEmpty());
    QVERIFY(!status.isEmpty());
    QVERIFY2(status.last()[0].toString().contains(QStringLiteral("locked")), qPrintable(status.last()[0].toString()));
}

void BScheduleTimelineTest::gridSaveWritesAndStartsAfresh()
{
    GrpcTab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    int asked = 0;
    tab.widget.setScheduleSaveGate([&](QWidget *) { ++asked; return true; });
    BScheduleGridView *grid = tab.widget.gridView();
    QVERIFY(grid->hasSchedule());
    QVERIFY(!grid->saveButton()->isEnabled());

    // An edit: the third Run line... here the only one, moved to :30
    QVERIFY(grid->editor()->paint(BGridCellSet::rect(6, 5, 6, 5), [] {
        BGridBrush brush;
        brush.overrides.level = QStringLiteral("Full");
        return brush;
    }()));
    QVERIFY(grid->isModified());
    QVERIFY(grid->saveButton()->isEnabled());
    // Editing began: the raw text was read for its SHA-256
    QTRY_COMPARE(tab.widget.persistence()->preparedText(QStringLiteral("WeeklyCycle")), kWeeklyCycleText);

    QTest::mouseClick(grid->saveButton(), Qt::LeftButton);
    QVERIFY(tab.wait());
    QCOMPARE(asked, 1);
    QCOMPARE(tab.report().kind, BScheduleReport::Written);
    const BConfig::Request write = tab.client.writes().value(0);
    QCOMPARE(write.name, QStringLiteral("WeeklyCycle"));
    QStringList values;
    QVERIFY(BScheduleResource::runValues(write.text, &values));
    QCOMPARE(values.first(), QStringLiteral("Level=Incremental daily at 08:00"));   // untouched, as in the file
    QCOMPARE(values.size(), 2);

    // The schedule is read again: it is what the grid starts from now
    const QString reloaded = QStringLiteral(R"({"result":{"schedules":{"WeeklyCycle":{"name":"WeeklyCycle","run":["%1","%2"]}}}})")
                                 .arg(values[0], values[1]);
    tab.widget.processShowSchedulesResponse(reloaded);
    QVERIFY(!grid->isModified());
    QVERIFY(!grid->hasPendingSchedule());
    QCOMPARE(grid->model()->loadedSchedule().runs.size(), 2);
    QCOMPARE(grid->editor()->undoStack()->count(), 0);
}

void BScheduleTimelineTest::cancelIsLeftInEveryDialog()
{
    // The application-wide filter comes with the style sheet
    BUiScale::instance().setApplicationStyleSheet(qApp->styleSheet());
    // A button box of any style: Cancel leftmost, OK to its right
    QDialog dialog;
    auto *layout = new QVBoxLayout(&dialog);
    auto *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(box);
    dialog.resize(400, 100);
    dialog.show();
    QVERIFY(QTest::qWaitForWindowExposed(&dialog));
    QTRY_VERIFY(box->button(QDialogButtonBox::Cancel)->geometry().x()
                < box->button(QDialogButtonBox::Ok)->geometry().x());
    QVERIFY(box->button(QDialogButtonBox::Cancel)->geometry().x() < 40);
    // A second show keeps it
    dialog.hide();
    dialog.show();
    QTest::qWait(20);
    QVERIFY(box->button(QDialogButtonBox::Cancel)->geometry().x()
            < box->button(QDialogButtonBox::Ok)->geometry().x());

    // A message box too
    QMessageBox message(QMessageBox::Question, "t", "q", QMessageBox::Discard | QMessageBox::Cancel);
    message.show();
    QVERIFY(QTest::qWaitForWindowExposed(&message));
    QTRY_VERIFY(message.button(QMessageBox::Cancel)->mapTo(&message, QPoint()).x()
                < message.button(QMessageBox::Discard)->mapTo(&message, QPoint()).x());
    message.hide();

    // A wizard: Cancel first in its button layout
    QWizard wizard;
    wizard.addPage(new QWizardPage);
    wizard.show();
    QVERIFY(QTest::qWaitForWindowExposed(&wizard));
    QTRY_VERIFY(wizard.button(QWizard::CancelButton)->mapTo(&wizard, QPoint()).x()
                < wizard.button(QWizard::FinishButton)->mapTo(&wizard, QPoint()).x());
}

void BScheduleTimelineTest::clickOnTheListsAnswers()
{
    Tab tab;
    const char *two = R"({"result":{"schedules":{
        "WeeklyCycle":{"name":"WeeklyCycle","run":["Level=Incremental daily at 08:00"]},
        "Nightly":{"name":"Nightly","run":["Level=Full daily at 22:00"]}}}})";
    const char *jobs = R"({"result":{"jobs":{
        "BackupEleLa":{"name":"BackupEleLa","type":"Backup","schedule":"WeeklyCycle","client":"elela-fd","storage":["File"]},
        "BackupMail":{"name":"BackupMail","type":"Backup","schedule":"Nightly","client":"elela-fd","storage":["File"]}}}})";
    tab.feed(two, jobs, kNoJobDefs, kOneClient);
    tab.widget.resize(1200, 800);
    tab.widget.show();
    QVERIFY(QTest::qWaitForWindowExposed(&tab.widget));
    auto *schedules = tab.widget.findChild<QListWidget *>(QStringLiteral("scheduleList"));
    QVERIFY(schedules);
    auto *jobList = tab.widget.findChild<QTableWidget *>(QStringLiteral("scheduleJobList"));
    QVERIFY(jobList);

    // A schedule clicked: its jobs, and its rows highlighted in the timeline
    QListWidgetItem *nightly = schedules->findItems("Nightly", Qt::MatchExactly).value(0);
    QVERIFY(nightly);
    QTest::mouseClick(schedules->viewport(), Qt::LeftButton, Qt::NoModifier,
                      schedules->visualItemRect(nightly).center());
    QCOMPARE(jobList->rowCount(), 1);
    QCOMPARE(jobList->item(0, 0)->text(), QString("BackupMail"));
    QCOMPARE(tab.timeline()->highlightedSchedule(), QString("Nightly"));
    QVERIFY(tab.timeline()->highlightedRowTop() > 0);
    // A second click: the jobs of every checked schedule again
    QTest::mouseClick(schedules->viewport(), Qt::LeftButton, Qt::NoModifier,
                      schedules->visualItemRect(nightly).center());
    QCOMPARE(jobList->rowCount(), 2);
    QVERIFY(tab.timeline()->highlightedSchedule().isEmpty());
    QTest::mouseClick(schedules->viewport(), Qt::LeftButton, Qt::NoModifier,
                      schedules->visualItemRect(nightly).center());
    QCOMPARE(jobList->rowCount(), 1);

    // A job clicked: its measured history opens below the timeline, and
    // again after the area was closed, with the same row still selected
    QTest::mouseClick(jobList->viewport(), Qt::LeftButton, Qt::NoModifier,
                      jobList->visualItemRect(jobList->item(0, 0)).center());
    QTabWidget *lower = qobject_cast<QTabWidget *>(tab.waitPanel()->parentWidget()->parentWidget());
    QVERIFY(lower);
    QVERIFY(lower->isVisible() || !lower->isHidden());
    QCOMPARE(lower->currentIndex(), 1);
    lower->setVisible(false);
    QTest::mouseClick(jobList->viewport(), Qt::LeftButton, Qt::NoModifier,
                      jobList->visualItemRect(jobList->item(0, 0)).center());
    QVERIFY(!lower->isHidden());
}

void BScheduleTimelineTest::gridSaveWritesTheDescription()
{
    GrpcTab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    tab.widget.setScheduleSaveGate([](QWidget *) { return true; });
    BScheduleGridView *grid = tab.widget.gridView();
    auto *description = grid->findChild<QLineEdit *>(QStringLiteral("scheduleDescription"));
    QVERIFY(description);
    QVERIFY(description->text().isEmpty());

    // Only the description: the schedule is modified and can be saved
    QTest::keyClicks(description, QStringLiteral("Nightly \"core\" backups"));
    QVERIFY(grid->isModified());
    QVERIFY(grid->descriptionChanged());
    QVERIFY(grid->saveButton()->isEnabled());
    QTRY_COMPARE(tab.widget.persistence()->preparedText(QStringLiteral("WeeklyCycle")), kWeeklyCycleText);

    QTest::mouseClick(grid->saveButton(), Qt::LeftButton);
    QVERIFY(tab.wait());
    QCOMPARE(tab.report().kind, BScheduleReport::Written);
    const QString written = tab.client.writes().value(0).text;
    QVERIFY2(written.contains(QStringLiteral("Description = \"Nightly \\\"core\\\" backups\"")), qPrintable(written));
    // The Run line as it was in the file, with its comment
    QVERIFY2(written.contains(QStringLiteral("Run = Level=Incremental daily at 08:00   # mornings")),
             qPrintable(written));

    // Read again with the description: nothing is modified any more
    tab.widget.processShowSchedulesResponse(QStringLiteral(
        R"({"result":{"schedules":{"WeeklyCycle":{"name":"WeeklyCycle","description":"Nightly \"core\" backups",)"
        R"("run":["Level=Incremental daily at 08:00"]}}}})"));
    QCOMPARE(description->text(), QStringLiteral("Nightly \"core\" backups"));
    QVERIFY(!grid->isModified());

    // Revert all brings the reported description back
    QTest::keyClicks(description, QStringLiteral("!"));
    QVERIFY(grid->isModified());
    QVERIFY(grid->revertButton()->isEnabled());
    QTest::mouseClick(grid->revertButton(), Qt::LeftButton);
    QCOMPARE(description->text(), QStringLiteral("Nightly \"core\" backups"));
    QVERIFY(!grid->isModified());
}

void BScheduleTimelineTest::deleteOfAScheduleInUseDeletesNothing()
{
    GrpcTab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    tab.client.refs.insert(QStringLiteral("WeeklyCycle"),
                           {BConfig::ResourceRef{BConfig::ResourceType::Job, QStringLiteral("BackupEleLa")}});
    tab.widget.setScheduleDeleteGate([](QWidget *) { return true; });

    QTest::mouseClick(tab.widget.gridView()->deleteButton(), Qt::LeftButton);
    QVERIFY(tab.wait());
    QCOMPARE(tab.report().kind, BScheduleReport::Referenced);
    QCOMPARE(tab.report().referencedBy, QStringList({QStringLiteral("Job BackupEleLa")}));
    QVERIFY(tab.client.writes().isEmpty());
    QVERIFY(tab.ui.reviews.isEmpty());
    QVERIFY(tab.client.files.contains(QStringLiteral("WeeklyCycle")));

    // Without the capability (BNET): nothing is asked of the Director
    tab.client.requests.clear();
    tab.widget.setScheduleDeleteGate([](QWidget *) { return false; });
    QTest::mouseClick(tab.widget.gridView()->deleteButton(), Qt::LeftButton);
    QTest::qWait(50);
    QVERIFY(tab.client.requests.isEmpty());
}

// ============================================================================
// Copy, rename, move of one job
// ============================================================================

namespace {

/// Two jobs on WeeklyCycle, on the same client
const char *const kTwoJobs = "{\"result\":{\"jobs\":{"
    "\"BackupA\":{\"name\":\"BackupA\",\"type\":\"Backup\",\"schedule\":\"WeeklyCycle\","
    "\"client\":\"elela-fd\",\"storage\":[\"File\"]},"
    "\"BackupB\":{\"name\":\"BackupB\",\"type\":\"Backup\",\"schedule\":\"WeeklyCycle\","
    "\"client\":\"elela-fd\",\"storage\":[\"File\"]}}}}";

/// The raw texts of the two jobs on the fake Director
void addTwoJobTexts(GrpcTab &tab)
{
    tab.client.allow(BConfig::ResourceType::Job);
    tab.client.files.insert("BackupA", "Job {\n  Name = \"BackupA\"\n  Schedule = \"WeeklyCycle\"   # nightly\n}\n");
    tab.client.files.insert("BackupB", "Job {\n  Name = BackupB\n  schedule = WeeklyCycle\n  Level = Full\n}\n");
}

/// The last line of the status
QString lastStatus(const QSignalSpy &status)
{
    return status.isEmpty() ? QString() : status.last().at(0).toString();
}

} // namespace

void BScheduleTimelineTest::scheduleNamesAndRenamedText()
{
    QVERIFY(BScheduleResource::nameProblem("Weekly-2.copy:1_a", {"WeeklyCycle"}).isEmpty());
    QVERIFY(!BScheduleResource::nameProblem("", {}).isEmpty());
    QVERIFY(!BScheduleResource::nameProblem("Weekly Cycle", {}).isEmpty());
    QVERIFY(!BScheduleResource::nameProblem("Weekly\"x", {}).isEmpty());
    QVERIFY(!BScheduleResource::nameProblem(QString(128, QLatin1Char('a')), {}).isEmpty());
    QVERIFY(BScheduleResource::nameProblem("weeklycycle", {"WeeklyCycle"}).contains("WeeklyCycle"));

    // Only the value of Name changes; comments, Run lines and line ends stay
    QCOMPARE(BScheduleResource::withName(kWeeklyCycleText, "Nightly"),
             QString(kWeeklyCycleText).replace("\"WeeklyCycle\"", "\"Nightly\""));
    const QString crlf = "Schedule {\r\n  name=Old  # was\r\n  Run = daily at 01:00\r\n}\r\n";
    QCOMPARE(BScheduleResource::withName(crlf, "New"),
             QString("Schedule {\r\n  name=\"New\"  # was\r\n  Run = daily at 01:00\r\n}\r\n"));
    QCOMPARE(BScheduleResource::withName("Schedule {\n  Run = daily at 01:00\n}\n", "New"), QString());
}

void BScheduleTimelineTest::copyCreatesTheSameRunLines()
{
    GrpcTab tab;
    tab.feed("{\"result\":{\"schedules\":{\"WeeklyCycle\":{\"name\":\"WeeklyCycle\",\"description\":\"Mornings\","
             "\"run\":[\"Level=Incremental daily at 08:00\",\"Level=Full sun at 23:05\"]}}}}",
             kOneJob, kNoJobDefs, kOneClient);
    BScheduleGridView *grid = tab.widget.gridView();
    QVERIFY(grid->copyButton()->isEnabled());

    QString suggested;
    QString takenProblem;
    tab.widget.setScheduleNamePrompt([&](QWidget *, const QString &, const QString &, const QString &suggestion,
                                         const std::function<QString(const QString &)> &problem) {
        suggested = suggestion;
        takenProblem = problem("WeeklyCycle");
        return QStringLiteral("Weekly2");
    });
    QTest::mouseClick(grid->copyButton(), Qt::LeftButton);
    QVERIFY(tab.wait());
    QCOMPARE(suggested, QString("WeeklyCycle-copy"));
    QVERIFY(!takenProblem.isEmpty());
    QCOMPARE(tab.report().kind, BScheduleReport::Written);

    // Checked first, shown, then created: the same Run lines and Description
    QCOMPARE(tab.ui.reviews.size(), 1);
    QCOMPARE(tab.ui.reviews.first().operation, BScheduleReview::Create);
    QCOMPARE(tab.client.writes().size(), 1);
    const BConfig::Request write = tab.client.writes().first();
    QCOMPARE(write.kind, BConfig::Request::Create);
    QCOMPARE(write.type, BConfig::ResourceType::Schedule);
    QCOMPARE(write.text, BScheduleResource::createText("Weekly2", "Mornings",
                                                       {"Level=Incremental daily at 08:00", "Level=Full sun at 23:05"}));
    QVERIFY(tab.client.files.contains("Weekly2"));
    // The original is untouched
    QCOMPARE(tab.client.files.value("WeeklyCycle"), kWeeklyCycleText);
}

void BScheduleTimelineTest::copyRefusesBadNamesAndNeedsCreate()
{
    GrpcTab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    BScheduleGridView *grid = tab.widget.gridView();
    QSignalSpy status(&tab.widget, &BScheduleWidget::statusMessageChanged);
    QString answer;
    tab.widget.setScheduleNamePrompt([&](QWidget *, const QString &, const QString &, const QString &,
                                         const std::function<QString(const QString &)> &) { return answer; });

    // Cancelled, a blank in the name, a name that is taken: nothing is sent
    for (const QString &name : {QString(), QString("Weekly Copy"), QString("weeklycycle")}) {
        answer = name;
        QTest::mouseClick(grid->copyButton(), Qt::LeftButton);
        QTest::qWait(20);
        QVERIFY2(tab.client.requests.isEmpty(), qPrintable(name));
    }
    QVERIFY2(lastStatus(status).contains("already a schedule named WeeklyCycle"), qPrintable(lastStatus(status)));

    // The Director refuses the name (taken by another resource): reported, nothing created
    tab.client.files.insert("Taken", "Job {\n  Name = Taken\n}\n");
    answer = "Taken";
    QTest::mouseClick(grid->copyButton(), Qt::LeftButton);
    QVERIFY(tab.wait());
    QCOMPARE(tab.report().kind, BScheduleReport::Exists);
    QVERIFY(tab.client.writes().isEmpty());

    // Without CreateRaw for schedules the button is disabled and says why
    BConfig::Capabilities caps = tab.client.caps;
    caps.operations[int(BConfig::ResourceType::Schedule)].remove(int(BConfig::Operation::CreateRaw));
    tab.client.setCapabilities(caps);
    QVERIFY(!grid->copyButton()->isEnabled());
    QVERIFY2(grid->copyButton()->toolTip().contains("not available on this connection"),
             qPrintable(grid->copyButton()->toolTip()));
    // Rename needs the jobs as well
    QVERIFY(!grid->renameButton()->isEnabled());
    QVERIFY2(grid->renameButton()->toolTip().contains("not available on this connection"),
             qPrintable(grid->renameButton()->toolTip()));
}

void BScheduleTimelineTest::renameMovesJobsAndDeletesTheOld()
{
    GrpcTab tab;
    addTwoJobTexts(tab);
    tab.feed(kWeeklyCycle, kTwoJobs, kNoJobDefs, kOneClient);
    BScheduleGridView *grid = tab.widget.gridView();
    QVERIFY(grid->renameButton()->isEnabled());
    tab.widget.setScheduleNamePrompt([](QWidget *, const QString &, const QString &, const QString &,
                                        const std::function<QString(const QString &)> &) {
        return QStringLiteral("Nightly");
    });
    int asked = 0;
    QString summary;
    tab.widget.setChangeConfirm([&](QWidget *, const QString &, const QString &text) {
        ++asked;
        summary = text;
        // Nothing is written before the confirmation
        return tab.client.writes().isEmpty();
    });
    QSignalSpy status(&tab.widget, &BScheduleWidget::statusMessageChanged);

    QTest::mouseClick(grid->renameButton(), Qt::LeftButton);
    QTRY_COMPARE_WITH_TIMEOUT(tab.client.writes().size(), 4, 3000);
    QTRY_VERIFY_WITH_TIMEOUT(lastStatus(status).contains("is now named Nightly"), 3000);
    QCOMPARE(asked, 1);
    QVERIFY2(summary.contains("BackupA") && summary.contains("BackupB")
             && summary.contains("Then schedule WeeklyCycle is deleted"), qPrintable(summary));

    const QList<BConfig::Request> writes = tab.client.writes();
    // The copy: the file's text under the new name, comment kept
    QCOMPARE(writes[0].kind, BConfig::Request::Create);
    QCOMPARE(writes[0].text, QString(kWeeklyCycleText).replace("\"WeeklyCycle\"", "\"Nightly\""));
    // The jobs, with the SHA-256 of the text read; only the Schedule line changes
    QCOMPARE(writes[1].kind, BConfig::Request::Update);
    QCOMPARE(writes[2].kind, BConfig::Request::Update);
    QCOMPARE(tab.client.files.value("BackupA"),
             QString("Job {\n  Name = \"BackupA\"\n  Schedule = \"Nightly\"\n}\n"));
    QCOMPARE(tab.client.files.value("BackupB"),
             QString("Job {\n  Name = BackupB\n  Schedule = \"Nightly\"\n  Level = Full\n}\n"));
    for (int i = 1; i <= 2; ++i)
        QVERIFY(!writes[i].expectedSha256.isEmpty());
    // Then the old schedule goes, with the SHA-256 of its text
    QCOMPARE(writes[3].kind, BConfig::Request::Delete);
    QCOMPARE(writes[3].name, QString("WeeklyCycle"));
    QCOMPARE(writes[3].expectedSha256, BConfig::sha256Hex(kWeeklyCycleText));
    QVERIFY(!tab.client.files.contains("WeeklyCycle"));
    QVERIFY(tab.client.files.contains("Nightly"));
}

void BScheduleTimelineTest::renameRefusals()
{
    GrpcTab tab;
    addTwoJobTexts(tab);
    tab.feed(kWeeklyCycle, kTwoJobs, kNoJobDefs, kOneClient);
    BScheduleGridView *grid = tab.widget.gridView();
    tab.widget.setScheduleNamePrompt([](QWidget *, const QString &, const QString &, const QString &,
                                        const std::function<QString(const QString &)> &) {
        return QStringLiteral("Nightly");
    });
    bool confirm = false;
    tab.widget.setChangeConfirm([&](QWidget *, const QString &, const QString &) { return confirm; });
    QSignalSpy status(&tab.widget, &BScheduleWidget::statusMessageChanged);

    // Not confirmed: read, but nothing written
    QTest::mouseClick(grid->renameButton(), Qt::LeftButton);
    QTRY_VERIFY_WITH_TIMEOUT(lastStatus(status).contains("was not confirmed"), 3000);
    QVERIFY(tab.client.writes().isEmpty());

    // A job that takes its Schedule from its JobDefs: refused before anything is written
    confirm = true;
    const QString jobB = tab.client.files.value("BackupB");
    tab.client.files.insert("BackupB", "Job {\n  Name = BackupB\n  JobDefs = \"D\"\n}\n");
    QTest::mouseClick(grid->renameButton(), Qt::LeftButton);
    QTRY_VERIFY_WITH_TIMEOUT(lastStatus(status).contains("takes its Schedule from its JobDefs"), 3000);
    QVERIFY(tab.client.writes().isEmpty());
    tab.client.files.insert("BackupB", jobB);

    // The file says other Run lines than loaded: refused, refresh first
    const QString weekly = tab.client.files.value("WeeklyCycle");
    tab.client.files.insert("WeeklyCycle", QString(weekly).replace("08:00", "09:00"));
    QTest::mouseClick(grid->renameButton(), Qt::LeftButton);
    QTRY_VERIFY_WITH_TIMEOUT(lastStatus(status).contains("does not say what was loaded"), 3000);
    QVERIFY(tab.client.writes().isEmpty());
    tab.client.files.insert("WeeklyCycle", weekly);

    // The Director refuses the delete (a JobDefs still names the schedule):
    // the steps before are written, the status line says which
    tab.client.refs.insert("WeeklyCycle", {BConfig::ResourceRef{BConfig::ResourceType::JobDefs, "D"}});
    QTest::mouseClick(grid->renameButton(), Qt::LeftButton);
    QTRY_VERIFY_WITH_TIMEOUT(lastStatus(status).contains("failed"), 3000);
    const QString message = lastStatus(status);
    QVERIFY2(message.contains("Writing deletion of schedule WeeklyCycle failed: it is still used by JobDefs D"),
             qPrintable(message));
    QVERIFY2(message.contains("Written before: schedule Nightly, job BackupA, job BackupB"), qPrintable(message));
    QVERIFY(tab.client.files.contains("WeeklyCycle"));

    // Changes in the grid that are not written: no rename
    GrpcTab other;
    addTwoJobTexts(other);
    other.feed(kWeeklyCycle, kTwoJobs, kNoJobDefs, kOneClient);
    BScheduleGridView *otherGrid = other.widget.gridView();
    QVERIFY(otherGrid->renameButton()->isEnabled());
    QTest::keyClicks(otherGrid->findChild<QLineEdit *>(QStringLiteral("scheduleDescription")), QStringLiteral("x"));
    QVERIFY(otherGrid->isModified());
    QVERIFY(!otherGrid->renameButton()->isEnabled());
    QVERIFY(otherGrid->copyButton()->isEnabled());

    // Without the jobs' methods: disabled, with the reason
    GrpcTab bare;
    bare.feed(kWeeklyCycle, kTwoJobs, kNoJobDefs, kOneClient);
    QVERIFY(!bare.widget.gridView()->renameButton()->isEnabled());
    QVERIFY(bare.widget.gridView()->renameButton()->toolTip().contains("read and change jobs"));
}

void BScheduleTimelineTest::dropOfASharedScheduleMovesOneJob()
{
    GrpcTab tab;
    addTwoJobTexts(tab);
    tab.feed(kWeeklyCycle, kTwoJobs, kNoJobDefs, kOneClient);
    tab.widget.setScheduleChangeGate([](QWidget *) { return true; });
    QStringList askedJobs;
    QString askedJob, askedCopy, askedTime;
    tab.widget.setDragScopeQuestion([&](QWidget *, const QString &, const QStringList &jobs, const QString &job,
                                        const QString &copyName, const QString &newTime) {
        askedJobs = jobs;
        askedJob = job;
        askedCopy = copyName;
        askedTime = newTime;
        return BScheduleWidget::DragScope::OneJob;
    });
    QString summary;
    tab.widget.setChangeConfirm([&](QWidget *, const QString &, const QString &text) {
        summary = text;
        return true;
    });
    QSignalSpy dropped(tab.timeline(), &BScheduleGanttWidget::dragDropped);
    QSignalSpy status(&tab.widget, &BScheduleWidget::statusMessageChanged);

    dragBar(tab.timeline(), barStart(tab.timeline(), 1, 8, 0), 3 * tab.timeline()->zoomLevel());
    QCOMPARE(dropped.count(), 1);
    const QString job = tab.timeline()->entries().value(dropped[0][0].toInt()).jobName;
    QVERIFY(job == "BackupA" || job == "BackupB");
    QTRY_COMPARE_WITH_TIMEOUT(tab.client.writes().size(), 2, 3000);
    QTRY_VERIFY_WITH_TIMEOUT(lastStatus(status).contains("on its own schedule"), 3000);

    QCOMPARE(askedJobs.size(), 2);
    QCOMPARE(askedJob, job);
    QCOMPARE(askedCopy, QString("WeeklyCycle-1100"));
    QCOMPARE(askedTime, QString("11:00"));
    QVERIFY2(summary.contains("WeeklyCycle-1100") && summary.contains(job), qPrintable(summary));

    // A copy with the dragged line at its new time, then the job set to it
    const QList<BConfig::Request> writes = tab.client.writes();
    QCOMPARE(writes[0].kind, BConfig::Request::Create);
    QStringList values;
    QVERIFY(BScheduleResource::runValues(writes[0].text, &values));
    QCOMPARE(values.size(), 1);
    QVERIFY2(values.first().contains("at 11:00"), qPrintable(values.first()));
    QVERIFY(writes[0].text.contains("Name = \"WeeklyCycle-1100\""));
    QCOMPARE(writes[1].kind, BConfig::Request::Update);
    QCOMPARE(writes[1].type, BConfig::ResourceType::Job);
    QCOMPARE(writes[1].name, job);
    QCOMPARE(BStaggerPlan::scheduleValue(tab.client.files.value(job)), QString("WeeklyCycle-1100"));
    // The schedule itself and the other job stay
    QCOMPARE(tab.client.files.value("WeeklyCycle"), kWeeklyCycleText);
    const QString otherJob = job == "BackupA" ? "BackupB" : "BackupA";
    QCOMPARE(BStaggerPlan::scheduleValue(tab.client.files.value(otherJob)), QString("WeeklyCycle"));
    QVERIFY(tab.ui.reviews.isEmpty());
}

void BScheduleTimelineTest::dropOfASharedScheduleForAllOrCancelled()
{
    GrpcTab tab;
    addTwoJobTexts(tab);
    tab.feed(kWeeklyCycle, kTwoJobs, kNoJobDefs, kOneClient);
    tab.widget.setScheduleChangeGate([](QWidget *) { return true; });
    int asked = 0;
    BScheduleWidget::DragScope answer = BScheduleWidget::DragScope::Cancel;
    tab.widget.setDragScopeQuestion([&](QWidget *, const QString &, const QStringList &, const QString &,
                                        const QString &, const QString &) {
        ++asked;
        return answer;
    });
    QSignalSpy status(&tab.widget, &BScheduleWidget::statusMessageChanged);

    // Cancelled: nothing is sent
    dragBar(tab.timeline(), barStart(tab.timeline(), 1, 8, 0), 3 * tab.timeline()->zoomLevel());
    QTest::qWait(50);
    QCOMPARE(asked, 1);
    QVERIFY(tab.client.requests.isEmpty());
    QVERIFY2(lastStatus(status).contains("was not changed"), qPrintable(lastStatus(status)));

    // For all jobs: the schedule itself is updated, as before, with both jobs in the review
    answer = BScheduleWidget::DragScope::AllJobs;
    dragBar(tab.timeline(), barStart(tab.timeline(), 1, 8, 0), 3 * tab.timeline()->zoomLevel());
    QVERIFY(tab.wait());
    QCOMPARE(asked, 2);
    QCOMPARE(tab.report().kind, BScheduleReport::Written);
    QCOMPARE(tab.client.writes().size(), 1);
    QCOMPARE(tab.client.writes().first().kind, BConfig::Request::Update);
    QCOMPARE(tab.client.writes().first().name, QString("WeeklyCycle"));
    QStringList reviewed = tab.ui.reviews.first().jobs;
    reviewed.sort();
    QCOMPARE(reviewed, (QStringList{"BackupA", "BackupB"}));

    // Without the methods for jobs, nobody is asked: the schedule changes for all
    GrpcTab bare;
    bare.feed(kWeeklyCycle, kTwoJobs, kNoJobDefs, kOneClient);
    bare.widget.setScheduleChangeGate([](QWidget *) { return true; });
    int bareAsked = 0;
    bare.widget.setDragScopeQuestion([&](QWidget *, const QString &, const QStringList &, const QString &,
                                         const QString &, const QString &) {
        ++bareAsked;
        return BScheduleWidget::DragScope::Cancel;
    });
    dragBar(bare.timeline(), barStart(bare.timeline(), 1, 8, 0), 3 * bare.timeline()->zoomLevel());
    QVERIFY(bare.wait());
    QCOMPARE(bareAsked, 0);
    QCOMPARE(bare.report().kind, BScheduleReport::Written);
}

void BScheduleTimelineTest::moveOfOneJobStopsAtTheFirstRefusal()
{
    GrpcTab tab;
    addTwoJobTexts(tab);
    tab.feed(kWeeklyCycle, kTwoJobs, kNoJobDefs, kOneClient);
    tab.widget.setScheduleChangeGate([](QWidget *) { return true; });
    tab.widget.setDragScopeQuestion([](QWidget *, const QString &, const QStringList &, const QString &,
                                       const QString &, const QString &) {
        return BScheduleWidget::DragScope::OneJob;
    });
    int confirmed = 0;
    tab.widget.setChangeConfirm([&](QWidget *, const QString &, const QString &) { return ++confirmed > 1; });
    QSignalSpy status(&tab.widget, &BScheduleWidget::statusMessageChanged);

    // Not confirmed: nothing written
    dragBar(tab.timeline(), barStart(tab.timeline(), 1, 8, 0), 3 * tab.timeline()->zoomLevel());
    QTRY_VERIFY_WITH_TIMEOUT(lastStatus(status).contains("was not confirmed"), 3000);
    QVERIFY(tab.client.writes().isEmpty());

    // The copy is refused: the job is not changed
    tab.client.intercept = [](const BConfig::Request &r) -> std::optional<BConfig::Reply> {
        if (r.kind == BConfig::Request::Create) {
            BConfig::Reply reply;
            reply.status = BConfig::Status::Invalid;
            reply.message = "refused";
            return reply;
        }
        return std::nullopt;
    };
    dragBar(tab.timeline(), barStart(tab.timeline(), 1, 8, 0), 3 * tab.timeline()->zoomLevel());
    QTRY_VERIFY_WITH_TIMEOUT(lastStatus(status).contains("failed"), 3000);
    QCOMPARE(tab.client.writes().size(), 1);
    QVERIFY2(lastStatus(status).contains("Written before: nothing"), qPrintable(lastStatus(status)));
    QCOMPARE(BStaggerPlan::scheduleValue(tab.client.files.value("BackupA")), QString("WeeklyCycle"));
    QCOMPARE(BStaggerPlan::scheduleValue(tab.client.files.value("BackupB")), QString("WeeklyCycle"));
}

// ============================================================================
// Schedules no job uses
// ============================================================================

void BScheduleTimelineTest::unusedSchedulesAreLeftOut()
{
    // "Nightly" has one enabled job. Nothing runs "Orphan"; the only job
    // of "Retired" is disabled. Both are scheduled for the time of the
    // real run, on the client's side nothing is known of them.
    Tab tab;
    tab.feed(R"({"result":{"schedules":{
            "Nightly":{"name":"Nightly","run":["Level=Incremental daily at 08:00"]},
            "Orphan":{"name":"Orphan","run":["Level=Full daily at 08:00"]},
            "Retired":{"name":"Retired","run":["Level=Full daily at 08:00","Level=Incremental daily at 14:00"]}}}})",
             R"({"result":{"jobs":{
            "BackupEleLa":{"name":"BackupEleLa","type":"Backup","schedule":"Nightly",
                           "client":"elela-fd","storage":["File"]},
            "OldBackup":{"name":"OldBackup","type":"Backup","schedule":"Retired",
                         "client":"elela-fd","storage":["File"],"enabled":false}}}})",
             kNoJobDefs, kOneClient);

    // The timeline gets them, marked, without a job
    const QList<BScheduleEntry> entries = tab.timeline()->entries();
    QCOMPARE(entries.size(), 4);
    int unused = 0;
    for (const BScheduleEntry &entry : entries) {
        if (entry.jobName.isEmpty()) {
            QVERIFY2(entry.unused, qPrintable(entry.scheduleName));
            ++unused;
        } else {
            QCOMPARE(entry.jobName, QString("BackupEleLa"));
            QVERIFY(!entry.unused);
        }
    }
    QCOMPARE(unused, 3);

    // One run was replayed and is on display; nothing waits, nothing is
    // uncertain, and the list has no row for a schedule that starts nothing
    QCOMPARE(tab.counter()->text().mid(3), QString("No waiting predicted"));
    const QString panel = tab.panelText();
    QVERIFY2(panel.contains("1 run(s) on display, replayed with the 2 run(s) of all enabled "
                            "schedules from the day before on."), qPrintable(panel));
    QVERIFY2(!panel.contains("storage of job"), qPrintable(panel));
    QCOMPARE(tab.waitTable()->rowCount(), 1);
    QCOMPARE(tab.waitTable()->item(0, 0)->text(), QString("No run on display is predicted to wait."));

    // Such a row cannot be dragged: nothing is reported, nothing is asked
    int asked = 0;
    tab.widget.setScheduleChangeGate([&](QWidget *) { ++asked; return false; });
    QSignalSpy dropped(tab.timeline(), &BScheduleGanttWidget::dragDropped);
    // Rows: client group, its run, the group "Not used by any job", three rows
    for (int row = 3; row <= 5; ++row) {
        const QPoint centre = barStart(tab.timeline(), row, 8, 0) - QPoint(3, 0);
        dragBar(tab.timeline(), centre, 3 * tab.timeline()->zoomLevel());
        const QPoint afternoon = barStart(tab.timeline(), row, 14, 0) - QPoint(3, 0);
        dragBar(tab.timeline(), afternoon, 3 * tab.timeline()->zoomLevel());
    }
    QCOMPARE(dropped.count(), 0);
    QCOMPARE(asked, 0);

    // It is painted: a smoke test of the rows of both timelines
    QVERIFY(!tab.widget.grab().isNull());
}

// ============================================================================
// The counter and its list
// ============================================================================

void BScheduleTimelineTest::handleBelowTimelineMovesUpAndIsKept()
{
    BSettings::instance().setSchedulesLowerHeight(0);
    {
        Tab tab;
        tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
        // The hidden grid does not count: the page fits a laptop's height
        QVERIFY2(tab.widget.minimumSizeHint().height() < 450,
                 qPrintable(QString::number(tab.widget.minimumSizeHint().height())));

        tab.counter()->setChecked(true);
        QTest::qWait(50);
        auto *split = qobject_cast<QSplitter *>(tab.waitPanel()->parentWidget()->parentWidget()
                                                    ->parentWidget());
        QVERIFY(split);
        // Moved up as far as it goes: the timeline keeps a few rows only
        const int total = split->sizes().value(0) + split->sizes().value(1);
        split->setSizes({0, total});
        emit split->splitterMoved(0, 1);
        QVERIFY2(split->sizes().value(0) < total / 3,
                 qPrintable(QStringLiteral("%1 of %2").arg(split->sizes().value(0)).arg(total)));
        QVERIFY(BSettings::instance().schedulesLowerHeight() > total / 2);
    }
    // Another session: the tabs open where the handle was left
    const int kept = BSettings::instance().schedulesLowerHeight();
    Tab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    tab.counter()->setChecked(false);
    tab.counter()->setChecked(true);
    QTest::qWait(50);
    auto *split = qobject_cast<QSplitter *>(tab.waitPanel()->parentWidget()->parentWidget()
                                                ->parentWidget());
    QVERIFY(split);
    // (the timeline keeps its few rows, which may cost some pixels)
    QVERIFY2(qAbs(split->sizes().value(1) - kept) < 20,
             qPrintable(QStringLiteral("%1, kept %2").arg(split->sizes().value(1)).arg(kept)));
    BSettings::instance().setSchedulesLowerHeight(0);
}

void BScheduleTimelineTest::handleBesideWaitsIsKept()
{
    BSettings::instance().setSchedulesWaitSplit(0);
    {
        Tab tab;
        tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
        tab.widget.resize(1400, 900);
        tab.counter()->setChecked(true);
        QTest::qWait(50);
        auto *split = tab.waitPanel()->findChild<QSplitter *>(QStringLiteral("waitSplitter"));
        QVERIFY(split);
        const int total = split->sizes().value(0) + split->sizes().value(1);
        // The table narrower than at first: a quarter of the width
        split->setSizes({total / 4, total - total / 4});
        emit split->splitterMoved(total / 4, 1);
        const int share = BSettings::instance().schedulesWaitSplit();
        QVERIFY2(share > 150 && share < 350, qPrintable(QString::number(share)));
    }
    // Another session: the table and the notes share the width as left
    const int kept = BSettings::instance().schedulesWaitSplit();
    Tab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    tab.widget.resize(1400, 900);
    tab.counter()->setChecked(true);
    QTest::qWait(50);
    auto *split = tab.waitPanel()->findChild<QSplitter *>(QStringLiteral("waitSplitter"));
    QVERIFY(split);
    const int total = split->sizes().value(0) + split->sizes().value(1);
    QVERIFY(total > 0);
    const int share = split->sizes().value(0) * 1000 / total;
    QVERIFY2(qAbs(share - kept) < 30, qPrintable(QStringLiteral("%1, kept %2").arg(share).arg(kept)));
    BSettings::instance().setSchedulesWaitSplit(0);
}

void BScheduleTimelineTest::staggerIsWrittenAfterOneConfirmation()
{
    GrpcTab tab;
    tab.client.allow(BConfig::ResourceType::Job);
    // One job names its schedule, the others inherit it (no directive)
    tab.client.files.insert("JobA", "Job {\n  Name = \"JobA\"\n  Schedule = \"WeeklyCycle\"\n}\n");
    tab.client.files.insert("JobB", "Job {\n  Name = \"JobB\"\n  JobDefs = \"D\"\n}\n");
    tab.client.files.insert("JobC", "Job {\n  Name = \"JobC\"\n  Schedule = WeeklyCycle\n}\n");
    feedThreeWaitingJobs(tab);
    QString summary;
    int asked = 0;
    tab.widget.setStaggerConfirm([&](QWidget *, const QString &text) { ++asked; summary = text; return true; });
    QSignalSpy commands(&tab.widget, &BScheduleWidget::sendCommand);

    QPushButton *apply = proposeStagger(tab);
    QVERIFY(apply);
    QVERIFY(apply->isEnabled());
    apply->click();
    QTRY_COMPARE_WITH_TIMEOUT(tab.client.writes().size(), 4, 3000);
    QCOMPARE(asked, 1);
    QVERIFY2(summary.contains("New schedule WeeklyCycle-0815"), qPrintable(summary));
    QVERIFY2(summary.contains("New schedule WeeklyCycle-0830"), qPrintable(summary));

    // The two schedules first, then the two jobs that start later
    const QList<BConfig::Request> writes = tab.client.writes();
    QCOMPARE(writes[0].kind, BConfig::Request::Create);
    QCOMPARE(writes[1].kind, BConfig::Request::Create);
    QCOMPARE(writes[0].type, BConfig::ResourceType::Schedule);
    QCOMPARE(writes[2].kind, BConfig::Request::Update);
    QCOMPARE(writes[3].kind, BConfig::Request::Update);
    QCOMPARE(writes[2].type, BConfig::ResourceType::Job);
    QStringList created;
    for (int i = 0; i < 2; ++i) {
        const QString text = writes[i].text;
        QVERIFY2(text.contains("Run = ") && (text.contains("at 08:15") || text.contains("at 08:30")),
                 qPrintable(text));
        created << text.section('"', 1, 1);
    }
    created.sort();
    QCOMPARE(created, (QStringList{"WeeklyCycle-0815", "WeeklyCycle-0830"}));
    for (int i = 2; i < 4; ++i) {
        QVERIFY2(writes[i].text.contains("Schedule = \"WeeklyCycle-08"), qPrintable(writes[i].text));
        QCOMPARE(writes[i].text.count("Schedule ="), 1);
        // Sent with the SHA-256 of the text read: a change in between is a conflict
        QVERIFY(!writes[i].expectedSha256.isEmpty());
    }
    // Afterwards the jobs and schedules are read again
    QTRY_VERIFY_WITH_TIMEOUT([&]() {
        for (const QList<QVariant> &c : commands)
            if (c.at(0).value<BDirector::Command>() == BDirector::Command::ShowJobs) return true;
        return false;
    }(), 3000);
}

void BScheduleTimelineTest::staggerNotConfirmedWritesNothing()
{
    GrpcTab tab;
    tab.client.allow(BConfig::ResourceType::Job);
    for (const char *job : {"JobA", "JobB", "JobC"})
        tab.client.files.insert(job, QString("Job {\n  Name = \"%1\"\n}\n").arg(job));
    feedThreeWaitingJobs(tab);
    int asked = 0;
    tab.widget.setStaggerConfirm([&](QWidget *, const QString &) { ++asked; return false; });
    QPushButton *apply = proposeStagger(tab);
    QVERIFY(apply);
    apply->click();
    QTRY_COMPARE_WITH_TIMEOUT(asked, 1, 3000);
    QTest::qWait(50);
    QVERIFY(tab.client.writes().isEmpty());
    QVERIFY(apply->isEnabled());    // the proposal is still there

    // A refused write stops the rest: the first schedule is taken
    tab.widget.setStaggerConfirm([](QWidget *, const QString &) { return true; });
        int creates = 0;
    tab.client.intercept = [&](const BConfig::Request &r) -> std::optional<BConfig::Reply> {
        if (r.kind == BConfig::Request::Create && ++creates == 1) {
            BConfig::Reply reply;
            reply.status = BConfig::Status::Invalid;
            reply.message = "refused";
            return reply;
        }
        return std::nullopt;
    };
    QSignalSpy status(&tab.widget, &BScheduleWidget::statusMessageChanged);
    apply->click();
    QTRY_VERIFY_WITH_TIMEOUT(!status.isEmpty() && status.last().at(0).toString().contains("failed"), 3000);
    QCOMPARE(tab.client.writes().size(), 1);    // the refused create, nothing after it
    QVERIFY2(status.last().at(0).toString().contains("Written before: nothing"),
             qPrintable(status.last().at(0).toString()));
}

void BScheduleTimelineTest::staggerFollowsTheStoredConstraints()
{
    BSettings &settings = BSettings::instance();
    BSettings::StaggerConstraint fixed;
    fixed.fixed = true;
    BSettings::StaggerConstraint latest;
    latest.latestStart = QTime(8, 15);
    settings.setSchedulesStaggerConstraints("p1", {{"JobA", fixed}, {"JobB", fixed}, {"JobC", fixed}});

    GrpcTab tab;
    tab.widget.setDirectorProfileId("p1");
    feedThreeWaitingJobs(tab);
    QPushButton *apply = proposeStagger(tab);
    QVERIFY(apply);

    // All fixed: nothing moves, and the table says why
    QList<QStringList> rows = staggerRows(tab);
    QVERIFY(!rows.isEmpty());
    QCOMPARE(movedRows(rows), 0);
    QVERIFY(!apply->isEnabled());
    for (const QStringList &row : rows)
        QVERIFY2(row.at(2) == QLatin1String("Fixed by constraint"), qPrintable(row.join(" | ")));

    // Not after 08:15: only one of the two that wait finds a place
    settings.setSchedulesStaggerConstraints("p1", {{"JobA", latest}, {"JobB", latest}, {"JobC", latest}});
    rows = staggerRows(tab);
    QCOMPARE(movedRows(rows), 1);
    bool noted = false;
    for (const QStringList &row : rows) {
        if (!row.at(1).isEmpty()) QCOMPARE(row.at(1), QString("15 min"));
        noted = noted || row.at(2).contains("latest start 08:15");
    }
    QVERIFY2(noted, qPrintable(rows.value(0).join(" | ")));

    // Another Director has its own (none): both move
    tab.widget.setDirectorProfileId("p2");
    rows = staggerRows(tab);
    QCOMPARE(movedRows(rows), 2);
    for (const QStringList &row : rows) QVERIFY(!row.at(2).contains("constraint"));

    settings.setSchedulesStaggerConstraints("p1", {});
    QVERIFY(settings.schedulesStaggerConstraints("p1").isEmpty());
}

void BScheduleTimelineTest::staggerConstraintsDialogKeepsTheOthers()
{
    BSettings::StaggerConstraint fixed;
    fixed.fixed = true;
    BStaggerConstraintsDialog dialog({"A", "B", "C"}, {{"C", fixed}, {"Other", fixed}});
    QCheckBox *fixedA = dialog.findChild<QCheckBox *>("fixed:A");
    QCheckBox *fixedC = dialog.findChild<QCheckBox *>("fixed:C");
    QCheckBox *latestOnB = dialog.findChild<QCheckBox *>("latestOn:B");
    QTimeEdit *latestB = dialog.findChild<QTimeEdit *>("latest:B");
    QVERIFY(fixedA && fixedC && latestOnB && latestB);
    QVERIFY(fixedC->isChecked());
    QVERIFY(!latestB->isEnabled());
    fixedA->setChecked(true);
    latestOnB->setChecked(true);
    QVERIFY(latestB->isEnabled());
    latestB->setTime(QTime(23, 30));
    fixedC->setChecked(false);

    const QHash<QString, BSettings::StaggerConstraint> out = dialog.constraints();
    QCOMPARE(out.size(), 3);
    QVERIFY(out.value("A").fixed);
    QCOMPARE(out.value("B").latestStart, QTime(23, 30));
    QVERIFY(!out.value("B").fixed);
    QVERIFY(!out.contains("C"));        // cleared
    QVERIFY(out.value("Other").fixed);  // not listed: kept

    // Stored and read back per profile
    BSettings::instance().setSchedulesStaggerConstraints("dialog", out);
    const QHash<QString, BSettings::StaggerConstraint> back =
        BSettings::instance().schedulesStaggerConstraints("dialog");
    QCOMPARE(back.size(), 3);
    QCOMPARE(back.value("B").latestStart, QTime(23, 30));
    QVERIFY(back.value("A").fixed);
    QVERIFY(BSettings::instance().schedulesStaggerConstraints("elsewhere").isEmpty());
    BSettings::instance().setSchedulesStaggerConstraints("dialog", {});
}

void BScheduleTimelineTest::themedToolbarCutsNoButtonAndWeekHasCalendar()
{
    // A large font: the texts are wider than the themes' minimum of 80 px
    const int zoom = BUiScale::instance().zoomPercent();
    BUiScale::instance().setZoomPercent(200);
    Tab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    // The rule of the themes that cut the buttons: a minimum width of 80 px
    BUiScale::instance().setApplicationStyleSheet(
        QStringLiteral("QPushButton { padding: 5px 15px; min-width: 80px; }"));
    for (QComboBox *combo : tab.widget.findChildren<QComboBox *>()) {
        if (combo->findText(QStringLiteral("Week")) >= 0)
            combo->setCurrentIndex(combo->findText(QStringLiteral("Week")));
    }
    tab.widget.resize(900, 900);
    QCoreApplication::processEvents();

    for (const QString &name : {QStringLiteral("Legend"), QStringLiteral("Refresh")}) {
        QPushButton *button = nullptr;
        for (QPushButton *b : tab.widget.findChildren<QPushButton *>())
            if (b->text() == name) button = b;
        QVERIFY(button);
        QVERIFY2(button->width() >= button->sizeHint().width(),
                 qPrintable(QStringLiteral("%1: %2 px of %3").arg(name).arg(button->width())
                                .arg(button->sizeHint().width())));
    }
    // The week has the calendar as well
    QDateEdit *picker = tab.widget.findChild<QDateEdit *>();
    QVERIFY(picker);
    QVERIFY(picker->isVisible());
    QVERIFY(picker->calendarPopup());

    BUiScale::instance().setApplicationStyleSheet(QString());
    BUiScale::instance().setZoomPercent(zoom);
}

void BScheduleTimelineTest::counterSaysWhetherTheListIsOpen()
{
    Tab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    QPushButton *counter = tab.counter();
    QVERIFY(counter);
    QVERIFY(counter->isCheckable());
    QVERIFY(counter->focusPolicy() & Qt::TabFocus);    // the keyboard reaches it

    counter->setChecked(false);
    QVERIFY(counter->text().startsWith(QChar(0x25B6)));     // points to the right: closed
    QVERIFY(counter->text().endsWith("No waiting predicted"));
    QVERIFY2(counter->accessibleName().endsWith("The list is closed."),
             qPrintable(counter->accessibleName()));
    QVERIFY(!counter->accessibleName().contains(QChar(0x25B6)));
    QVERIFY(!tab.waitPanel()->isVisible());

    // Space on the focused button opens the list
    counter->setFocus();
    QTest::keyClick(counter, Qt::Key_Space);
    QVERIFY(counter->isChecked());
    QVERIFY(counter->text().startsWith(QChar(0x25BC)));     // points down: open
    QVERIFY2(counter->accessibleName().endsWith("The list is open."),
             qPrintable(counter->accessibleName()));
    QVERIFY(tab.waitPanel()->isVisible());
    // With room, the whole text; without, it is cut with "…" (the buttons
    // beside it are not) and the accessible name still has all of it.
    // "With room" is made explicit: fonts differ in width between systems
    tab.widget.resize(1600, 900);
    QCoreApplication::processEvents();
    QVERIFY2(!counter->text().contains(QChar(0x2026)), qPrintable(counter->text()));
    QWidget *legend = nullptr;
    for (QPushButton *b : tab.widget.findChildren<QPushButton *>())
        if (b->text() == QLatin1String("Legend")) legend = b;
    QVERIFY(legend);
    tab.widget.resize(500, 900);
    QApplication::processEvents();
    QVERIFY2(counter->text().endsWith(QChar(0x2026)), qPrintable(counter->text()));
    QVERIFY(legend->width() >= legend->sizeHint().width());
    QVERIFY(counter->accessibleName().contains(QLatin1String("No waiting predicted")));
    tab.widget.resize(1400, 900);
    QApplication::processEvents();
    QVERIFY(!counter->text().contains(QChar(0x2026)));

    QTest::keyClick(counter, Qt::Key_Space);
    QVERIFY(!counter->isChecked());
    QVERIFY(counter->text().startsWith(QChar(0x25B6)));
    QVERIFY(!tab.waitPanel()->isVisible());
}

void BScheduleTimelineTest::waitsOfSecondsInTheList()
{
    // Three jobs on one client that takes one job at a time, each measured
    // at 35 seconds and starting 2 s after its scheduled time. The 2 s are
    // the start overhead (connections, Client Run Before Job): the run holds
    // its client meanwhile. The first starts at 08:00:02 and is not held;
    // the second gets the client at 08:00:37 and starts 08:00:39; the third
    // gets it at 08:01:14 and starts 08:01:16.
    Tab tab;
    const QStringList names = {"JobA", "JobB", "JobC"};

    QString history = R"({"jsonrpc":"2.0","id":null,"result":{"jobs":[)";
    int id = 100;
    for (int day = 1; day <= 4; ++day) {
        for (const QString &name : names) {
            const QDateTime sched(QDate::currentDate().addDays(-day), QTime(8, 0));
            const QDateTime start = sched.addSecs(2);
            const QString f = QStringLiteral("yyyy-MM-dd HH:mm:ss");
            history += QString(R"(%1{"jobid":"%2","name":"%3","type":"B","level":"I","jobstatus":"T",)"
                               R"("schedtime":"%4","starttime":"%5","endtime":"%6","realendtime":"%6"})")
                           .arg(id == 100 ? "" : ",").arg(id).arg(name)
                           .arg(sched.toString(f), start.toString(f), start.addSecs(35).toString(f));
            ++id;
        }
    }
    history += "]}}";

    // The history is asked for a moment after the tab is on screen with a
    // connection; this is the Director's answer
    QObject::connect(&tab.widget, &BScheduleWidget::sendCommand, &tab.widget,
                     [&](BDirector::Command cmd, const QString &args) {
        if (cmd == BDirector::Command::LlistJobs) {
            QTimer::singleShot(0, &tab.widget, [&tab, args, history]() {
                tab.widget.processJobHistoryReply(args, history);
            });
        }
    });
    tab.widget.setConnectionState(true);

    QString showJobs = R"({"result":{"jobs":{)";
    for (int i = 0; i < names.size(); ++i) {
        showJobs += QString(R"(%1"%2":{"name":"%2","type":"Backup","schedule":"WeeklyCycle",)"
                            R"("client":"elela-fd","storage":["File"],"maximumconcurrentjobs":1})")
                        .arg(i ? "," : "", names[i]);
    }
    showJobs += "}}}";
    tab.feed(kWeeklyCycle, showJobs, kNoJobDefs, kOneClient);

    QPushButton *counter = tab.counter();
    counter->setChecked(true);
    // The whole text: the shown one is cut when the toolbar has no room
    QTRY_VERIFY2_WITH_TIMEOUT(counter->accessibleName().contains("2 run(s) wait, longest 1:16 min"),
                              qPrintable(counter->accessibleName()), 15000);
    if (qEnvironmentVariableIsSet("ONESIMUS_WAITS_PNG")) {
        QCoreApplication::processEvents();
        tab.widget.grab().save(qEnvironmentVariable("ONESIMUS_WAITS_PNG"));
    }

    // A run clicked: its history opens as a second tab beside the waits,
    // not below them
    QTabWidget *lower = tab.waitPanel()->parentWidget()
        ? qobject_cast<QTabWidget *>(tab.waitPanel()->parentWidget()->parentWidget()) : nullptr;
    QVERIFY(lower);
    BScheduleEntry clicked;
    clicked.jobName = QStringLiteral("JobA");
    emit tab.timeline()->entryClicked(clicked);
    QCoreApplication::processEvents();
    QCOMPARE(lower->count(), 3);
    QCOMPARE(lower->currentIndex(), 1);
    QVERIFY(!tab.waitPanel()->isVisible());     // behind the history's tab
    QVERIFY(!tab.counter()->isChecked());       // the list of waits is not on display
    if (qEnvironmentVariableIsSet("ONESIMUS_WAITS_PNG")) {
        tab.widget.grab().save(qEnvironmentVariable("ONESIMUS_WAITS_PNG")
                                   .replace(QLatin1String(".png"), QLatin1String("-history.png")));
    }
    // Staggered on the quarter hour: the two that wait start at 08:15 and
    // 08:30 and wait no more; the first, which does not wait, stays
    {
        QWidget *stagger = lower->findChild<QWidget *>(QStringLiteral("stagger"));
        QVERIFY(stagger);
        lower->setCurrentWidget(stagger);
        QPushButton *propose = nullptr;
        for (QPushButton *b : stagger->findChildren<QPushButton *>()) {
            if (b->text() == QLatin1String("Propose later start times")) propose = b;
        }
        QVERIFY(propose);
        propose->click();
        auto *table = stagger->findChild<QTableWidget *>();
        QVERIFY(table);
        QCOMPARE(table->rowCount(), 2);
        QMap<QString, QString> later;
        for (int row = 0; row < table->rowCount(); ++row)
            later.insert(table->item(row, 0)->text(), table->item(row, 4)->text());
        // (which of them starts first at 08:00 is the order the history shows)
        QStringList moves = later.values();
        moves.sort();
        QCOMPARE(moves, (QStringList{"15 min", "30 min"}));
        QCOMPARE(table->item(0, 6)->text(), QString("none"));
        bool summarised = false;
        for (QLabel *l : stagger->findChildren<QLabel *>()) {
            if (l->text().contains("2 job(s) start later")) summarised = true;
        }
        QVERIFY(summarised);
        if (qEnvironmentVariableIsSet("ONESIMUS_WAITS_PNG")) {
            tab.widget.grab().save(qEnvironmentVariable("ONESIMUS_WAITS_PNG")
                                       .replace(QLatin1String(".png"), QLatin1String("-stagger.png")));
        }
    }
    // The counter brings the waits to the front
    tab.counter()->click();
    QCoreApplication::processEvents();
    QCOMPARE(lower->currentIndex(), 0);
    QVERIFY(tab.counter()->isChecked());
    QVERIFY(tab.waitPanel()->isVisible());

    // The ✕ beside the tabs closes the area; both open again
    auto *close = qobject_cast<QToolButton *>(lower->cornerWidget(Qt::TopRightCorner));
    QVERIFY(close);
    close->click();
    QCoreApplication::processEvents();
    QVERIFY(!lower->isVisible());
    QVERIFY(!tab.counter()->isChecked());
    emit tab.timeline()->entryClicked(clicked);
    QTest::qWait(50);
    QVERIFY(lower->isVisible());
    QCOMPARE(lower->currentIndex(), 1);
    QVERIFY2(lower->height() > 100, qPrintable(QString::number(lower->height())));
    close->click();
    tab.counter()->click();
    QTest::qWait(50);
    QVERIFY(lower->isVisible());
    QCOMPARE(lower->currentIndex(), 0);
    QVERIFY(tab.waitPanel()->isVisible());
    QVERIFY2(tab.waitPanel()->height() > 100, qPrintable(QString::number(tab.waitPanel()->height())));

    // Columns: job, level, scheduled, predicted start, wait, measured,
    // certain, reason. Longest wait first: by its length, not by its text
    // ("39 s" > "1:16 min")
    QCOMPARE(tab.column(4), QStringList({"1:16 min", "39 s"}));
    QCOMPARE(tab.column(2), QStringList({"08:00", "08:00"}));
    QCOMPARE(tab.column(3), QStringList({"08:01:16", "08:00:39"}));
    // The history says 2 s: the runs it was measured on ran side by side,
    // which a client limit of 1 does not allow. The third is flagged.
    QCOMPARE(tab.column(5), QStringList({"2 s / 2 s", "2 s / 2 s"}));
    QCOMPARE(tab.column(6), QStringList({"yes", "yes"}));
    QVERIFY(tab.column(7).first().startsWith("Prediction too high"));

    // Ascending as well
    tab.waitTable()->sortByColumn(4, Qt::AscendingOrder);
    QCOMPARE(tab.column(4), QStringList({"39 s", "1:16 min"}));
    tab.waitTable()->sortByColumn(3, Qt::DescendingOrder);
    QCOMPARE(tab.column(3), QStringList({"08:01:16", "08:00:39"}));

    // The details of a row say the same, with seconds
    tab.waitTable()->sortByColumn(4, Qt::AscendingOrder);
    const QString tip = tab.waitTable()->item(0, 0)->toolTip();
    QVERIFY2(tip.contains("Predicted start: 08:00:39, 39 s after the scheduled time (08:00)"),
             qPrintable(tip));
    QVERIFY2(tip.contains("Start overhead 2 s"), qPrintable(tip));
    QVERIFY2(tip.contains("Measured wait at this level: median 2 s, max 2 s (4 run(s))"),
             qPrintable(tip));

    // The device of storage File: unknown until the user sets it, and the
    // history (runs side by side every day) gives no suggestion of 1
    QSpinBox *writers = tab.waitPanel()->findChild<QSpinBox *>("deviceWriters:File");
    QVERIFY(writers);
    QCOMPARE(writers->value(), 0);
    QCOMPARE(writers->text(), QString("unknown"));
    QPushButton *use = tab.waitPanel()->findChild<QPushButton *>("deviceUse:File");
    QVERIFY(use);
    QVERIFY(!use->isEnabled());
    QVERIFY(tab.widget.backtestText().startsWith("Back-test, last 4 night(s) (12 runs)"));

    // Set by the user: kept per Director profile and storage
    writers->setValue(1);
    QCOMPARE(BSettings::instance().schedulesDeviceWriters(QString(), "File"), 1);
    QTRY_VERIFY(!tab.widget.backtestText().contains("without a device limit"));
}

void BScheduleTimelineTest::reviewTextCanBeEdited()
{
    BScheduleReview review;
    review.scheduleName = QStringLiteral("WeeklyCycle");
    review.oldText = QStringLiteral("Schedule {\r\n  Name = WeeklyCycle\r\n  Run = mon-sat at 23:05\r\n}\r\n");
    review.newText = QStringLiteral("Schedule {\r\n  Name = WeeklyCycle\r\n  Run = mon-sat at 22:00\r\n}\r\n");
    BScheduleReviewDialog dialog(review);
    QPlainTextEdit *edit = dialog.textEdit();
    QVERIFY(edit);
    QVERIFY(!edit->isReadOnly());
    const QString writeLabel = dialog.writeButton()->text();
    QVERIFY(!dialog.isEdited());

    // 22:00 -> 22:05: the button no longer writes, it checks
    edit->setPlainText(edit->toPlainText().replace(QStringLiteral("22:00"), QStringLiteral("22:05")));
    QVERIFY(dialog.isEdited());
    QVERIFY(dialog.editedText().contains(QStringLiteral("22:05")));
    QVERIFY(dialog.writeButton()->text() != writeLabel);
    QVERIFY(!dialog.writeButton()->isDefault());

    // Back to the checked text: writing again; an empty text is an edit too
    edit->setPlainText(edit->toPlainText().replace(QStringLiteral("22:05"), QStringLiteral("22:00")));
    QVERIFY(!dialog.isEdited());
    QCOMPARE(dialog.writeButton()->text(), writeLabel);
    edit->clear();
    QVERIFY(dialog.isEdited());

    // A delete has no text to edit
    BScheduleReview gone = review;
    gone.operation = BScheduleReview::Delete;
    gone.newText.clear();
    BScheduleReviewDialog deleteDialog(gone);
    QVERIFY(!deleteDialog.textEdit());
    QVERIFY(!deleteDialog.isEdited());
}

void BScheduleTimelineTest::legendExplainsEveryMark()
{
    // Every mark the timeline draws has a picture and a sentence
    BScheduleLegend legend;
    const QList<BScheduleLegend::Mark> marks = BScheduleLegend::marks();
    QCOMPARE(marks.size(), 17);
    for (BScheduleLegend::Mark mark : marks) {
        QVERIFY(!BScheduleLegend::text(mark).isEmpty());
        const QImage image = legend.picture(mark).toImage();
        QVERIFY(!image.isNull());
        // Something is drawn on the background
        const QRgb background = image.pixel(0, 0);
        bool drawn = false;
        for (int y = 0; y < image.height() && !drawn; ++y)
            for (int x = 0; x < image.width() && !drawn; ++x)
                drawn = image.pixel(x, y) != background;
        QVERIFY2(drawn, qPrintable(BScheduleLegend::text(mark)));
    }
    QCOMPARE(legend.findChildren<QLabel *>().size(), 1 + 2 * marks.size());
    // For a look at it: ONESIMUS_LEGEND_PNG=/path/legend.png
    const QString png = qEnvironmentVariable("ONESIMUS_LEGEND_PNG");
    if (!png.isEmpty()) {
        legend.adjustSize();
        legend.grab().save(png);
    }
}

// ============================================================================
// Measured against planned
// ============================================================================

namespace {

/// One row of "llist jobs" of BackupEleLa (API mode 2: every value a string)
QString historyRow(int id, const QDateTime &sched, const QDateTime &start, const QDateTime &end,
                   const QString &status, const QString &level)
{
    const QString f = QStringLiteral("yyyy-MM-dd HH:mm:ss");
    return QString(R"({"jobid":"%1","name":"BackupEleLa","type":"B","level":"%2","jobstatus":"%3",)"
                   R"("schedtime":"%4","starttime":"%5","endtime":"%6","realendtime":"%6"})")
        .arg(id).arg(level, status, sched.toString(f), start.toString(f), end.toString(f));
}

/// Number of pixels that differ between two pictures inside @p rect
int pixelsDiffering(const QImage &a, const QImage &b, const QRect &rect)
{
    int count = 0;
    for (int y = rect.top(); y <= rect.bottom(); ++y)
        for (int x = rect.left(); x <= rect.right(); ++x)
            if (a.pixel(x, y) != b.pixel(x, y)) ++count;
    return count;
}

} // namespace

void BScheduleTimelineTest::measuredRunsBesideThePlannedBars()
{
    BSettings::instance().setSchedulesShowMeasuredRuns(true);
    Tab tab;
    const QDate today = QDate::currentDate();
    const QDate yesterday = today.addDays(-1);
    auto at = [](const QDate &date, int hour, int minute) { return QDateTime(date, QTime(hour, minute)); };

    // Yesterday: scheduled 08:00, waited 5 min, ran 30 min. A run started
    // by hand at 13:00 belongs to no start. The day before: an error.
    QString history = R"({"jsonrpc":"2.0","id":null,"result":{"jobs":[)";
    history += historyRow(1, at(yesterday, 8, 0), at(yesterday, 8, 5), at(yesterday, 8, 35), "T", "I") + ",";
    history += historyRow(2, at(yesterday, 13, 0), at(yesterday, 13, 0), at(yesterday, 13, 10), "T", "F") + ",";
    history += historyRow(3, at(yesterday.addDays(-1), 8, 0), at(yesterday.addDays(-1), 8, 0),
                          at(yesterday.addDays(-1), 8, 20), "E", "F");
    history += "]}}";
    QObject::connect(&tab.widget, &BScheduleWidget::sendCommand, &tab.widget,
                     [&tab, history](BDirector::Command cmd, const QString &args) {
        if (cmd == BDirector::Command::LlistJobs) {
            QTimer::singleShot(0, &tab.widget, [&tab, args, history]() {
                tab.widget.processJobHistoryReply(args, history);
            });
        }
    });
    tab.widget.setConnectionState(true);
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);

    QDateEdit *picker = tab.widget.findChild<QDateEdit *>();
    QVERIFY(picker);
    picker->setDate(yesterday);
    BScheduleGanttWidget *timeline = tab.timeline();
    QTRY_COMPARE_WITH_TIMEOUT(timeline->measuredRuns().size(), 1, 15000);
    QCOMPARE(timeline->measuredRuns()[0].run.jobId, quint64(1));
    // The storages' timeline has it as well
    QCOMPARE(tab.widget.findChildren<BScheduleGanttWidget *>().value(1)->measuredRuns().size(), 1);
    QCoreApplication::processEvents();

    // The wait from the scheduled time to the start, then the run, to the
    // minute of the catalog's times
    const int pph = timeline->zoomLevel();
    const int x8 = barStart(timeline, 1, 8, 0).x() - 3;
    const QRect wait = timeline->measuredWaitRect(0);
    const QRect run = timeline->measuredRunRect(0);
    QVERIFY2(qAbs(wait.left() - x8) <= 1, qPrintable(QStringLiteral("%1 %2").arg(wait.left()).arg(x8)));
    QVERIFY(qAbs(wait.width() - pph * 5 / 60) <= 1);
    QVERIFY(qAbs(run.left() - (x8 + pph * 5 / 60)) <= 1);
    QVERIFY(qAbs(run.width() - pph / 2) <= 1);
    // In the run's row, below its planned bar
    const BScheduleGanttWidget::Metrics m = timeline->metrics();
    const int rowTop = m.headerHeight + m.rowHeight;
    QVERIFY(run.top() > rowTop + m.rowHeight / 2);
    QVERIFY(run.bottom() < rowTop + m.rowHeight);
    QCOMPARE(timeline->measuredRunAt(run.center()), 0);
    QCOMPARE(timeline->measuredRunAt(wait.center()), 0);
    QCOMPARE(timeline->measuredRunAt(barStart(timeline, 1, 8, 0)), -1);   // the planned bar

    const QString tip = timeline->measuredToolTip(0);
    const QString day = yesterday.toString(Qt::ISODate);
    for (const QString &part : {QStringLiteral("Scheduled: %1 08:00:00").arg(day),
                                QStringLiteral("Started: %1 08:05:00").arg(day),
                                QStringLiteral("Ended: %1 08:35:00").arg(day),
                                QStringLiteral("Waited: 5m 00s"), QStringLiteral("Level: Incremental"),
                                QStringLiteral("Status: terminated normally")}) {
        QVERIFY2(tip.contains(part), qPrintable(part + QStringLiteral(" in ") + tip));
    }

    // Drawn, and gone when switched off (kept in the settings). The
    // timeline as wide as a screen: here the scroll area is narrow.
    auto *area = qobject_cast<QScrollArea *>(timeline->parentWidget()->parentWidget());
    QVERIFY(area);
    area->setWidgetResizable(false);
    timeline->resize(1600, timeline->height());
    timeline->grab();       // delivers the resize of the hidden tab: the time scale follows
    const QRect drawn = timeline->measuredRunRect(0);
    QVERIFY(QRect(QPoint(0, 0), timeline->size()).contains(drawn));
    const QImage shown = timeline->grab().toImage();
    QCheckBox *check = tab.widget.findChild<QCheckBox *>(QStringLiteral("showMeasuredRuns"));
    QVERIFY(check);
    QVERIFY(check->isChecked());
    check->setChecked(false);
    QVERIFY(!BSettings::instance().schedulesShowMeasuredRuns());
    QVERIFY(timeline->measuredRunRect(0).isEmpty());
    QCOMPARE(timeline->measuredRunAt(drawn.center()), -1);
    const QImage hidden = timeline->grab().toImage();
    if (qEnvironmentVariableIsSet("ONESIMUS_MEASURED_PNG")) {
        shown.save(qEnvironmentVariable("ONESIMUS_MEASURED_PNG"));
        hidden.save(qEnvironmentVariable("ONESIMUS_MEASURED_PNG").replace(".png", "-hidden.png"));
    }
    QVERIFY2(pixelsDiffering(shown, hidden, drawn) > drawn.width(),
             qPrintable(QString::number(pixelsDiffering(shown, hidden, drawn))));
    check->setChecked(true);
    QVERIFY(BSettings::instance().schedulesShowMeasuredRuns());
    area->setWidgetResizable(true);

    // The day before: the run that failed; the level differs from the plan
    picker->setDate(yesterday.addDays(-1));
    QCOMPARE(timeline->measuredRuns().size(), 1);
    QCOMPARE(timeline->measuredRuns()[0].run.status, QChar('E'));
    QVERIFY(timeline->measuredToolTip(0).contains(QStringLiteral("Status: terminated in error")));
    QVERIFY(timeline->measuredToolTip(0).contains(QStringLiteral("Level: Full (planned: Incremental)")));

    // Tomorrow has not run
    picker->setDate(today.addDays(1));
    QVERIFY(timeline->measuredRuns().isEmpty());
}

void BScheduleTimelineTest::gridLoadCountsTheStartsOfAllSchedules()
{
    Tab tab;
    // Without jobs nobody knows what starts: no load
    tab.widget.processShowSchedulesResponse(QString::fromLatin1(kWeeklyCycle));
    QCoreApplication::processEvents();
    BScheduleGridModel *model = tab.widget.gridView()->model();
    QVERIFY(!model->cell(0, 8).expectedConcurrentRuns.has_value());

    // Two jobs on WeeklyCycle (daily at 08:00); a disabled schedule and an
    // unused one start nothing
    const char *schedules = R"({"result":{"schedules":{
        "WeeklyCycle":{"name":"WeeklyCycle","run":["Level=Incremental daily at 08:00"]},
        "Retired":{"name":"Retired","enabled":false,"run":["Level=Full daily at 08:00"]},
        "Spare":{"name":"Spare","run":["Level=Full daily at 09:00"]}}}})";
    const char *jobs = R"({"result":{"jobs":{
        "A":{"name":"A","type":"Backup","schedule":"WeeklyCycle","client":"elela-fd","storage":["File"]},
        "B":{"name":"B","type":"Backup","schedule":"WeeklyCycle","client":"elela-fd","storage":["File"]},
        "C":{"name":"C","type":"Backup","schedule":"Retired","client":"elela-fd","storage":["File"]}}}})";
    tab.feed(schedules, jobs, kNoJobDefs, kOneClient);

    for (int row = 0; row < 7; ++row) {
        QVERIFY(model->cell(row, 8).expectedConcurrentRuns.has_value());
        QCOMPARE(*model->cell(row, 8).expectedConcurrentRuns, 2.0);
        QCOMPARE(*model->cell(row, 9).expectedConcurrentRuns, 0.0);
    }
    // The same whichever schedule the grid shows
    const QString tip = tab.widget.gridView()->grid()->cellToolTip(0, 8);
    QVERIFY2(tip.contains(QStringLiteral("2 run(s) a week")), qPrintable(tip));
    QVERIFY(tab.widget.gridView()->grid()->cellToolTip(0, 9).contains(QStringLiteral("no run")));
}

void BScheduleTimelineTest::columnWidthsOfTheTablesAreKept()
{
    for (const QString &key : {QStringLiteral("waits"), QStringLiteral("stagger")}) {
        BSettings::instance().setSchedulesTableHeaderState(key, QByteArray());
        BSettings::instance().setSchedulesTableHeaderScale(key, 0);
    }
    auto staggerTable = [](Tab &tab) {
        QWidget *page = tab.widget.findChild<QWidget *>(QStringLiteral("stagger"));
        return page ? page->findChild<QTableWidget *>() : nullptr;
    };
    {
        Tab tab;
        tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
        tab.counter()->setChecked(true);
        QCoreApplication::processEvents();
        // Fitting the columns to their contents is not the user's choice
        QVERIFY(BSettings::instance().schedulesTableHeaderState(QStringLiteral("waits")).isEmpty());

        tab.waitTable()->horizontalHeader()->resizeSection(0, 222);
        QTableWidget *stagger = staggerTable(tab);
        QVERIFY(stagger);
        stagger->horizontalHeader()->resizeSection(1, 177);
        QVERIFY(!BSettings::instance().schedulesTableHeaderState(QStringLiteral("waits")).isEmpty());
        QVERIFY(!BSettings::instance().schedulesTableHeaderState(QStringLiteral("stagger")).isEmpty());

        // A refresh keeps them
        tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
        QCOMPARE(tab.waitTable()->columnWidth(0), 222);
    }
    // Another session: as the user left them
    Tab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    QCOMPARE(tab.waitTable()->columnWidth(0), 222);
    QTableWidget *stagger = staggerTable(tab);
    QVERIFY(stagger);
    QCOMPARE(stagger->columnWidth(1), 177);
    for (const QString &key : {QStringLiteral("waits"), QStringLiteral("stagger")})
        BSettings::instance().setSchedulesTableHeaderState(key, QByteArray());
}

// ============================================================================
// Live overlay (push events over gRPC)
// ============================================================================

void BScheduleTimelineTest::liveOverlayFollowsTheEvents()
{
    Tab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    tab.widget.setLiveOverlayEnabled(true);
    BScheduleGanttWidget *timeline = tab.timeline();

    // Queued: a waiting job in the row of its run
    tab.widget.onJobStatusEvent(42, "BackupEleLa", "Queued");
    tab.widget.flushLiveOverlay();
    QCOMPARE(timeline->liveJobs().size(), 1);
    QCOMPARE(tab.widget.sdTimeline()->liveJobs().size(), 1);
    QCOMPARE(timeline->liveJobs()[0].phase, BLiveJobOverlay::Phase::Waiting);

    // Ten minutes on, the wait is a band up to now (not checked in the
    // last minutes of a day: "now" would be tomorrow)
    const QDateTime later = QDateTime::currentDateTime().addSecs(600);
    const bool sameDay = later.date() == QDate::currentDate();
    QList<BScheduleGanttWidget::LiveBand> bands = timeline->liveBands(later);
    if (sameDay) {
        QCOMPARE(bands.size(), 1);
        QCOMPARE(bands[0].row, 1);
        QVERIFY(!bands[0].wait.isEmpty());
        QVERIFY(bands[0].run.isEmpty());
    }

    // It starts: a running job, its band ends at now
    tab.widget.onJobMessage(42, "BackupEleLa",
                            "Start Backup JobId 42, Job=BackupEleLa.2026-11-20_08.00.00_01\n");
    tab.widget.flushLiveOverlay();
    QCOMPARE(timeline->liveJobs()[0].phase, BLiveJobOverlay::Phase::Running);
    bands = timeline->liveBands(later);
    if (sameDay) {
        QCOMPARE(bands.size(), 1);
        QVERIFY(!bands[0].run.isEmpty());
        const BScheduleGanttWidget::Metrics m = timeline->metrics();
        const int nowX = m.labelWidth + static_cast<int>(
            QDate::currentDate().startOfDay().secsTo(later) / 3600.0 * timeline->zoomLevel());
        QVERIFY2(qAbs(bands[0].run.right() + 1 - nowX) <= 1,
                 qPrintable(QString("%1 vs %2").arg(bands[0].run.right()).arg(nowX)));
    }
    // For a look at it: ONESIMUS_LIVE_PNG=/path/live.png (a second job
    // waits since an hour ago)
    const QString png = qEnvironmentVariable("ONESIMUS_LIVE_PNG");
    if (!png.isEmpty()) {
        BLiveJobOverlay::Job waiting;
        waiting.jobId = 44;
        waiting.job = QStringLiteral("BackupEleLa");
        waiting.queued = QDateTime::currentDateTime().addSecs(-3600);
        QList<BLiveJobOverlay::Job> shown = timeline->liveJobs();
        shown[0].queued = QDateTime::currentDateTime().addSecs(-3 * 3600);
        shown[0].started = QDateTime::currentDateTime().addSecs(-2 * 3600);
        // A timeline of its own, wide enough to see the bands
        BScheduleGanttWidget wide;
        wide.setEntries(timeline->entries());
        wide.setLiveJobs(shown + QList<BLiveJobOverlay::Job>{waiting});
        wide.resize(1200, 160);
        wide.grab().save(png);
    }

    // It ends: cleared
    tab.widget.onJobStatusEvent(42, "BackupEleLa", "Success");
    tab.widget.flushLiveOverlay();
    QVERIFY(timeline->liveJobs().isEmpty());
    QVERIFY(timeline->liveBands(later).isEmpty());

    // A job no run of the view belongs to (started by hand) has no row
    tab.widget.onJobStatusEvent(43, "RestoreFiles", "Queued");
    tab.widget.flushLiveOverlay();
    QCOMPARE(timeline->liveJobs().size(), 1);
    QVERIFY(timeline->liveBands(later).isEmpty());

    // The connection goes: nothing is left
    tab.widget.setConnectionState(false);
    QVERIFY(tab.widget.liveOverlay().isEmpty());
    QVERIFY(timeline->liveJobs().isEmpty());
}

void BScheduleTimelineTest::liveOverlayIsThrottled()
{
    Tab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    tab.widget.setLiveOverlayEnabled(true);
    BScheduleGanttWidget *timeline = tab.timeline();

    // A burst of events reaches the timeline once, after the throttle
    for (int id = 1; id <= 50; ++id)
        tab.widget.onJobStatusEvent(quint64(id), "BackupEleLa", "Queued");
    QCOMPARE(tab.widget.liveOverlay().jobs().size(), 50);
    QVERIFY(timeline->liveJobs().isEmpty());
    QTRY_COMPARE_WITH_TIMEOUT(timeline->liveJobs().size(), 50, 5 * BScheduleWidget::LiveRepaintMs);
}

void BScheduleTimelineTest::liveOverlayOffOnBnet()
{
    Tab tab;
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);
    QVERIFY(!tab.widget.liveOverlayEnabled());
    tab.widget.onJobStatusEvent(42, "BackupEleLa", "Queued");
    tab.widget.onJobMessage(42, "BackupEleLa", "Start Backup JobId 42, Job=x\n");
    QTest::qWait(2 * BScheduleWidget::LiveRepaintMs);
    QVERIFY(tab.widget.liveOverlay().isEmpty());
    QVERIFY(tab.timeline()->liveJobs().isEmpty());

    // Turned off: what was there goes
    tab.widget.setLiveOverlayEnabled(true);
    tab.widget.onJobStatusEvent(42, "BackupEleLa", "Queued");
    tab.widget.flushLiveOverlay();
    QCOMPARE(tab.timeline()->liveJobs().size(), 1);
    tab.widget.setLiveOverlayEnabled(false);
    QVERIFY(tab.timeline()->liveJobs().isEmpty());
}

void BScheduleTimelineTest::liveOverlaySeededFromTheHistory()
{
    // A job that was running when the connection was made, and one that
    // ended before the history's reply arrived
    Tab tab;
    tab.widget.setLiveOverlayEnabled(true);
    const QString f = QStringLiteral("yyyy-MM-dd HH:mm:ss");
    const QDateTime sched(QDate::currentDate(), QTime(8, 0));
    QString history = QStringLiteral("{'jsonrpc':'2.0','id':null,'result':{'jobs':["
        "{'jobid':'7','name':'BackupEleLa','type':'B','level':'I','jobstatus':'R',"
        "'schedtime':'%1','starttime':'%2','endtime':'','realendtime':''},"
        "{'jobid':'8','name':'BackupEleLa','type':'B','level':'I','jobstatus':'R',"
        "'schedtime':'%1','starttime':'%2','endtime':'','realendtime':''}]}}")
        .arg(sched.toString(f), sched.addSecs(120).toString(f));
    history.replace(QLatin1Char('\''), QLatin1Char('"'));
    QObject::connect(&tab.widget, &BScheduleWidget::sendCommand, &tab.widget,
                     [&tab, history](BDirector::Command cmd, const QString &args) {
        if (cmd == BDirector::Command::LlistJobs) {
            QTimer::singleShot(0, &tab.widget, [&tab, args, history]() {
                // Job 8 ended while the reply was on its way
                tab.widget.onJobStatusEvent(8, "BackupEleLa", "Success");
                tab.widget.processJobHistoryReply(args, history);
            });
        }
    });
    tab.widget.setConnectionState(true);
    tab.feed(kWeeklyCycle, kOneJob, kNoJobDefs, kOneClient);

    QTRY_VERIFY_WITH_TIMEOUT(tab.widget.liveOverlay().job(7) != nullptr, 10000);
    QVERIFY(!tab.widget.liveOverlay().job(8));
    const BLiveJobOverlay::Job *job = tab.widget.liveOverlay().job(7);
    QCOMPARE(job->phase, BLiveJobOverlay::Phase::Running);
    QCOMPARE(job->queued, sched);
    QCOMPARE(job->started, sched.addSecs(120));
    QVERIFY(job->seeded);
    QTRY_COMPARE_WITH_TIMEOUT(tab.timeline()->liveJobs().size(), 1, 5 * BScheduleWidget::LiveRepaintMs);
}

void BScheduleTimelineTest::liveJobFindsTheRowOfItsRun()
{
    const QDate today(2026, 11, 20);
    auto entry = [&](const QString &job, int hour, bool unused = false) {
        BScheduleEntry e;
        e.scheduleName = QStringLiteral("S");
        e.jobName = job;
        e.hour = hour;
        e.unused = unused;
        e.occurrences = {QDateTime(today, QTime(hour, 0))};
        return e;
    };
    const QList<BScheduleEntry> entries = {
        entry("Other", 21),
        entry("Nightly", 2),
        entry("Nightly", 21),
        entry("Nightly", 23),
        entry("Nightly", 21, true),
    };
    BLiveJobOverlay::Job job;
    job.job = QStringLiteral("Nightly");
    // Queued at 21:02: the run of 21:00
    job.queued = QDateTime(today, QTime(21, 2));
    QCOMPARE(BScheduleGanttWidget::liveEntryIndex(entries, job), 2);
    // Queued two minutes early: the run of 23:00
    job.queued = QDateTime(today, QTime(22, 58));
    QCOMPARE(BScheduleGanttWidget::liveEntryIndex(entries, job), 3);
    // Before every run of the day: the first one
    job.queued = QDateTime(today, QTime(1, 0));
    QCOMPARE(BScheduleGanttWidget::liveEntryIndex(entries, job), 1);
    // No run of the job
    job.job = QStringLiteral("Unknown");
    QCOMPARE(BScheduleGanttWidget::liveEntryIndex(entries, job), -1);
}

// ============================================================================
// The wait prediction in its worker thread
// ============================================================================

namespace {

const char *const kTwoClientJobs = R"({"result":{"jobs":{
    "JobA":{"name":"JobA","type":"Backup","schedule":"WeeklyCycle","client":"a-fd","storage":["File"],
            "maximumconcurrentjobs":1},
    "JobB":{"name":"JobB","type":"Backup","schedule":"WeeklyCycle","client":"b-fd","storage":["File"],
            "maximumconcurrentjobs":1}}}})";
const char *const kTwoClients = R"({"result":{"clients":{
    "a-fd":{"name":"a-fd","address":"a","maximumconcurrentjobs":10},
    "b-fd":{"name":"b-fd","address":"b","maximumconcurrentjobs":10}}}})";
const char *const kDirectorOfOne =
    R"({"result":{"directors":{"test-dir":{"name":"test-dir","maximumconcurrentjobs":1}}}})";

} // namespace

void BScheduleTimelineTest::waitPredictionDropsResultsOfOlderInputs()
{
    // Two jobs at 08:00 on two clients; without a history their length is
    // not known. With 20 slots of the Director neither waits for the
    // other; with one slot the second may wait as long as the first runs.
    Tab tab;
    tab.feed(kWeeklyCycle, kTwoClientJobs, kNoJobDefs, kTwoClients);
    const QString withTwenty = tab.counter()->accessibleName() + tab.panelText();

    // The worker is held at its start while newer inputs come in: its
    // result belongs to the old inputs and must not be taken over
    QSemaphore go;
    tab.widget.setWaitWorkerGate([&go]() { go.acquire(); });
    auto release = qScopeGuard([&go]() { go.release(100); });  // never leave a worker held
    const int started = tab.widget.waitWorkersStarted();
    const int dropped = tab.widget.waitResultsDropped();

    tab.widget.processShowDirectorResponse(QString::fromLatin1(kDirector));
    QTRY_COMPARE(tab.widget.waitWorkersStarted(), started + 1);     // held at the gate
    tab.widget.processShowDirectorResponse(QString::fromLatin1(kDirectorOfOne));
    QCoreApplication::processEvents();
    // One worker at a time: the newer inputs wait for the held one
    QCOMPARE(tab.widget.waitWorkersStarted(), started + 1);
    QVERIFY(!tab.widget.waitPredictionIdle());

    go.release(2);
    QTRY_VERIFY_WITH_TIMEOUT(tab.widget.waitPredictionIdle(), 10000);
    QCOMPARE(tab.widget.waitWorkersStarted(), started + 2);
    QCOMPARE(tab.widget.waitResultsDropped(), dropped + 1);
    const QString threaded = tab.counter()->accessibleName() + tab.panelText();
    QVERIFY2(threaded.contains("Director = 1"), qPrintable(threaded));
    QVERIFY(threaded != withTwenty);

    // The worker's answer is the one worked out in this thread
    tab.widget.setWaitWorkerGate({});
    tab.widget.flushWaitPrediction();
    QCOMPARE(tab.counter()->accessibleName() + tab.panelText(), threaded);
}

void BScheduleTimelineTest::waitsWaitForTheHistoryLoadedAfterTheConnect()
{
    // The history is asked for after the connect even while the tab is
    // not on screen, and until it is in the waits are not predicted
    Tab tab;
    tab.widget.hide();
    QStringList asked;
    QObject::connect(&tab.widget, &BScheduleWidget::sendCommand, &tab.widget,
                     [&](BDirector::Command cmd, const QString &args) {
        if (cmd == BDirector::Command::LlistJobs) asked << args;
    });
    tab.widget.setConnectionState(true);

    // On a connect the JobDefs come first: no prediction of their own
    const int started = tab.widget.waitWorkersStarted();
    tab.jobs.parseShowJobDefs(QString::fromLatin1(kNoJobDefs));
    tab.widget.processShowJobDefsResponse(QString());
    QCoreApplication::processEvents();
    QVERIFY(tab.widget.waitPredictionIdle());
    QCOMPARE(tab.widget.waitWorkersStarted(), started);

    tab.feed(kWeeklyCycle, kTwoClientJobs, kNoJobDefs, kTwoClients);
    QVERIFY2(tab.counter()->accessibleName().contains("Waiting not predicted yet"),
             qPrintable(tab.counter()->accessibleName()));
    QVERIFY2(tab.panelText().contains("Not predicted yet: the job history is being loaded."),
             qPrintable(tab.panelText()));

    QTRY_COMPARE_WITH_TIMEOUT(asked.size(), 1, 5000);
    tab.widget.processJobHistoryReply(asked.first(),
                                      R"({"jsonrpc":"2.0","id":null,"result":{"jobs":[]}})");
    QTRY_VERIFY_WITH_TIMEOUT(!tab.counter()->accessibleName().contains("not predicted"), 10000);
    tab.settle();
    QVERIFY2(tab.panelText().contains("run(s) on display"), qPrintable(tab.panelText()));
}

QTEST_MAIN(BScheduleTimelineTest)
#include "bscheduletimeline_test.moc"
