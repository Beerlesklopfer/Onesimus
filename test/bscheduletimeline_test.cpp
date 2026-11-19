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

#include <QComboBox>
#include <QDateEdit>
#include <QGroupBox>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QSignalSpy>
#include <QTableWidget>
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

    QGroupBox *waitPanel() const
    {
        const QList<QGroupBox *> boxes = widget.findChildren<QGroupBox *>();
        for (QGroupBox *box : boxes) {
            if (box->title() == QLatin1String("Predicted waits")) return box;
        }
        return nullptr;
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
    void deleteOfAScheduleInUseDeletesNothing();

    // --- Schedules no job uses ---
    void unusedSchedulesAreLeftOut();

    // --- The counter and its list ---
    void counterSaysWhetherTheListIsOpen();
    void themedToolbarCutsNoButtonAndWeekHasCalendar();
    void waitsOfSecondsInTheList();

    // --- The review ---
    void reviewTextCanBeEdited();

    // --- The legend ---
    void legendExplainsEveryMark();

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
    const QSettings probe(QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
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
    // beside it are not) and the accessible name still has all of it
    QVERIFY(!counter->text().contains(QChar(0x2026)));
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
    QCOMPARE(marks.size(), 15);
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

QTEST_MAIN(BScheduleTimelineTest)
#include "bscheduletimeline_test.moc"
