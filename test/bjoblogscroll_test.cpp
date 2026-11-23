// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bjoblogscroll_test.cpp
 * @brief The job log view does not flip when log data arrives (issue #10)
 *
 * The real path: BDirector's jobLogResult/jobMessage signals (emitted here,
 * no connection) -> the view's BJobLogSource -> BJobLogModel -> BJobLogView.
 * Every change of the vertical scroll bar's value is recorded, within the
 * call that delivers the data and across the event loop, and so is the
 * value at every paint of the viewport (what the user sees). Checked:
 *
 * (a) following the tail: the view stays at the bottom; no value or paint
 *     goes back (to the top and down again);
 * (b) scrolled up: the value does not change at all;
 * (c) a large first load (with the multi-line termination summary and lines
 *     wider than the view) is painted at the bottom only, then incremental
 *     loads and pushed lines follow the tail;
 * (d) a job switch: the new job's first load is painted at the bottom only;
 *     a late reply for the previous job changes nothing.
 *
 * The same for the three places with a job log: the pane of the Jobs tab
 * (a filter proxy in between), the job log dialog and the "Logs" tab of the
 * job details dialog. A model reset on appends (the cause named in the
 * issue) makes these checks fail: the first frame shows the top.
 * Offscreen, no Director, no network.
 */

#include <QtTest>
#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QScrollBar>
#include <QSettings>
#include <QTabWidget>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QVBoxLayout>

#include "config/bsettings.h"
#include "director/bdirector.h"
#include "jobs/bjobdetailsdialog.h"
#include "jobs/bjobfileswidget.h"
#include "jobs/bjoblogdialog.h"
#include "jobs/bjoblogmodel.h"
#include "jobs/bjoblogview.h"
#include "jobs/bjobwidget.h"

namespace {

/// Termination summary as `list joblog` returns it: one record, many lines
QString summaryText(quint64 jobId)
{
    QString text = QStringLiteral("bareos-dir JobId %1: Bareos bareos-dir 23.0.3 (11Jun24):\n").arg(jobId);
    const char *lines[] = {
        "  Build OS:               Debian GNU/Linux 12 (bookworm)",
        "  Backup Level:           Incremental, since=2026-11-16 21:00:03",
        "  Client:                 \"pdc-fd\" 23.0.3 (11Jun24) Debian GNU/Linux 12 (bookworm),debian",
        "  FileSet:                \"LinuxAll\" 2026-07-09 21:00:00",
        "  Pool:                   \"Incremental\" (From Job IncPool override)",
        "  Start time:             17-Nov-2026 21:00:03",
        "  End time:               17-Nov-2026 21:00:41",
        "  FD Files Written:       1,204",
        "  FD Bytes Written:       918,331,201 (918.3 MB)",
        "  Volume name(s):         Incremental-0007",
        "  Non-fatal FD errors:    0",
        "  SD Errors:              0",
        "  FD termination status:  OK",
        "  SD termination status:  OK",
        "  Termination:            Backup OK",
    };
    for (int round = 0; round < 3; ++round)   // about 45 lines, taller than the view
        for (const char *line : lines)
            text += QLatin1String(line) + QLatin1Char('\n');
    return text;
}

/// `list joblog` reply with the records [first, last] (LogIds first..last)
QString joblogReply(quint64 jobId, int first, int last, bool withSummary = false)
{
    QJsonArray array;
    for (int i = first; i <= last; ++i) {
        QString text = QStringLiteral("pdc-dir JobId %1: line %2 of the log").arg(jobId).arg(i);
        if (i % 7 == 0)   // wider than the view: a horizontal scroll bar appears
            text += QStringLiteral(" - Pruning files of client pdc-fd, ") + QString(160, QLatin1Char('x'));
        array.append(QJsonObject{{"time", "2026-11-17 21:00:03"}, {"logtext", text},
                                 {"logid", QString::number(jobId * 100000 + i)}});
    }
    if (withSummary) {
        array.append(QJsonObject{{"time", "2026-11-17 21:00:41"}, {"logtext", summaryText(jobId)},
                                 {"logid", QString::number(jobId * 100000 + last + 1)}});
    }
    return QString::fromUtf8(QJsonDocument(QJsonObject{
        {"jsonrpc", "2.0"}, {"id", QJsonValue::Null},
        {"result", QJsonObject{{"joblog", array}}}}).toJson(QJsonDocument::Compact));
}

/// Records the scroll bar's values: every change, and the value at every paint
class ScrollRecorder : public QObject
{
public:
    struct Paint { int value; int maximum; int rows; };

    explicit ScrollRecorder(QAbstractItemView *view)
        : m_view(view)
    {
        connect(view->verticalScrollBar(), &QScrollBar::valueChanged, this,
                [this](int v) { changes.append(v); });
        view->viewport()->installEventFilter(this);
    }

    void clear() { changes.clear(); paints.clear(); }

    QList<int> changes;
    QList<Paint> paints;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (watched == m_view->viewport() && event->type() == QEvent::Paint) {
            const QScrollBar *bar = m_view->verticalScrollBar();
            paints.append({bar->value(), bar->maximum(), m_view->model() ? m_view->model()->rowCount() : 0});
        }
        return false;
    }

private:
    QAbstractItemView *m_view;
};

/// Let the view lay out and paint (several event loop turns)
void settle()
{
    for (int i = 0; i < 6; ++i) {
        QCoreApplication::processEvents();
        QTest::qWait(10);
    }
}

QString describe(const ScrollRecorder &r)
{
    QStringList p;
    for (const auto &paint : r.paints)
        p << QStringLiteral("%1/%2(%3 rows)").arg(paint.value).arg(paint.maximum).arg(paint.rows);
    QStringList c;
    for (int v : r.changes)
        c << QString::number(v);
    return QStringLiteral("values [%1] paints [%2]").arg(c.join(QLatin1Char(' ')), p.join(QLatin1Char(' ')));
}

} // namespace

class BJobLogScrollTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void followTailStaysAtBottom_data();
    void followTailStaysAtBottom();
    void scrolledUpStaysPut_data();
    void scrolledUpStaysPut();
    void largeFirstLoadPaintedAtBottom();
    void jobSwitchPaintedAtBottom();
    void jobsTabPaneFollowsTail();
    void jobLogDialogOpensAtBottom();
    void jobDetailsDialogOpensAtBottom();

private:
    struct Fixture
    {
        BDirector director;
        QWidget window;
        BJobLogView *view = nullptr;
        BJobLogModel *model = nullptr;
        std::unique_ptr<ScrollRecorder> recorder;

        explicit Fixture(bool push)
        {
            if (push)
                director.setTransport(BDirector::Transport::Grpc, 9101);
            auto *layout = new QVBoxLayout(&window);
            view = new BJobLogView(&window);
            model = new BJobLogModel(&window);
            view->setModel(model);
            view->attach(&director, model);
            layout->addWidget(view);
            window.resize(500, 300);
            window.show();
            recorder = std::make_unique<ScrollRecorder>(view);
        }
        QScrollBar *bar() const { return view->verticalScrollBar(); }
    };

    /// Asserts: no value change and no paint went below @p floor; ends at the bottom
    static void verifyFollowedTail(const ScrollRecorder &r, QScrollBar *bar, int floor, const char *step);

    QTemporaryDir m_config;
};

void BJobLogScrollTest::initTestCase()
{
    QVERIFY(m_config.isValid());
    QStandardPaths::setTestModeEnabled(true);
    qputenv("XDG_CONFIG_HOME", m_config.path().toUtf8());
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, m_config.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_config.path());
    QSettings::setDefaultFormat(QSettings::IniFormat);
}

void BJobLogScrollTest::verifyFollowedTail(const ScrollRecorder &r, QScrollBar *bar, int floor, const char *step)
{
    for (int i = 1; i < r.changes.size(); ++i)
        QVERIFY2(r.changes.at(i) >= r.changes.at(i - 1),
                 qPrintable(QStringLiteral("%1: value went back: %2").arg(QLatin1String(step), describe(r))));
    for (int v : r.changes)
        QVERIFY2(v >= floor, qPrintable(QStringLiteral("%1: value went back to %2 (was %3): %4")
                                            .arg(QLatin1String(step)).arg(v).arg(floor).arg(describe(r))));
    for (const auto &p : r.paints) {
        QVERIFY2(p.value >= floor, qPrintable(QStringLiteral("%1: painted at %2 (was %3): %4")
                                                  .arg(QLatin1String(step)).arg(p.value).arg(floor).arg(describe(r))));
        // Every frame shows the tail
        QVERIFY2(p.value == p.maximum, qPrintable(QStringLiteral("%1: painted above the bottom: %2")
                                                      .arg(QLatin1String(step), describe(r))));
    }
    QVERIFY2(bar->value() == bar->maximum(),
             qPrintable(QStringLiteral("%1: not at the bottom: %2/%3")
                            .arg(QLatin1String(step)).arg(bar->value()).arg(bar->maximum())));
}

void BJobLogScrollTest::followTailStaysAtBottom_data()
{
    QTest::addColumn<bool>("push");
    QTest::newRow("polling (BNET)") << false;
    QTest::newRow("push (gRPC)") << true;
}

// (a) Following the tail of a running job
void BJobLogScrollTest::followTailStaysAtBottom()
{
    QFETCH(bool, push);
    Fixture f(push);
    QVERIFY(QTest::qWaitForWindowExposed(&f.window));
    const quint64 job = 41;
    f.view->showJob(job, true);
    emit f.director.jobLogResult(job, joblogReply(job, 1, 80));
    settle();
    QVERIFY(f.bar()->maximum() > 0);
    QCOMPARE(f.bar()->value(), f.bar()->maximum());

    int last = 80;
    for (int round = 0; round < 10; ++round) {
        const int floor = f.bar()->value();
        f.recorder->clear();
        if (push && round % 2) {
            emit f.director.jobMessage(job, QStringLiteral("backup-pdc"),
                                       QStringLiteral("pdc-sd JobId 41: pushed line %1").arg(round));
        } else {
            // The catalog grows by a few records (polling, or the final load)
            emit f.director.jobLogResult(job, joblogReply(job, 1, last + 5));
            last += 5;
        }
        // Within the call that delivered the data
        verifyFollowedTail(*f.recorder, f.bar(), floor, "same call");
        // Across the event loop (layout, paint)
        settle();
        verifyFollowedTail(*f.recorder, f.bar(), floor, "after the event loop");
        QVERIFY(!f.recorder->paints.isEmpty() || f.recorder->changes.isEmpty());
    }
}

void BJobLogScrollTest::scrolledUpStaysPut_data()
{
    followTailStaysAtBottom_data();
}

// (b) The user scrolled up: appends do not move the view
void BJobLogScrollTest::scrolledUpStaysPut()
{
    QFETCH(bool, push);
    Fixture f(push);
    QVERIFY(QTest::qWaitForWindowExposed(&f.window));
    const quint64 job = 42;
    f.view->showJob(job, true);
    emit f.director.jobLogResult(job, joblogReply(job, 1, 120));
    settle();
    const int where = f.bar()->maximum() / 3;
    f.bar()->setValue(where);
    settle();

    int last = 120;
    for (int round = 0; round < 8; ++round) {
        f.recorder->clear();
        if (push && round % 2) {
            emit f.director.jobMessage(job, QStringLiteral("backup-pdc"),
                                       QStringLiteral("pdc-sd JobId 42: pushed line %1").arg(round));
        } else {
            emit f.director.jobLogResult(job, joblogReply(job, 1, last + 10, round == 7));
            last += 10;
        }
        settle();
        QVERIFY2(f.recorder->changes.isEmpty(), qPrintable(describe(*f.recorder)));
        for (const auto &p : f.recorder->paints)
            QVERIFY2(p.value == where, qPrintable(describe(*f.recorder)));
        QCOMPARE(f.bar()->value(), where);
        QVERIFY(f.bar()->maximum() > where);
    }
}

// (c) A finished job with a long log: the first load is painted at the
// bottom only, with the whole summary reachable; later loads follow the tail
void BJobLogScrollTest::largeFirstLoadPaintedAtBottom()
{
    Fixture f(false);
    QVERIFY(QTest::qWaitForWindowExposed(&f.window));
    const quint64 job = 43;
    f.view->showJob(job, true);
    settle();
    f.recorder->clear();

    emit f.director.jobLogResult(job, joblogReply(job, 1, 2000));
    verifyFollowedTail(*f.recorder, f.bar(), 0, "first load, same call");
    settle();
    QVERIFY(!f.recorder->paints.isEmpty());
    for (const auto &p : f.recorder->paints) {
        if (p.rows > 0)
            QVERIFY2(p.value == p.maximum && p.maximum > 0, qPrintable(describe(*f.recorder)));
    }
    QCOMPARE(f.bar()->value(), f.bar()->maximum());

    // Then the summary arrives: still at the bottom, its last line visible
    const int floor = f.bar()->value();
    f.recorder->clear();
    emit f.director.jobLogResult(job, joblogReply(job, 1, 2000, true));
    settle();
    verifyFollowedTail(*f.recorder, f.bar(), floor, "summary");
    const QModelIndex lastRow = f.view->model()->index(f.view->model()->rowCount() - 1, 0);
    QVERIFY(lastRow.data().toString().contains(QLatin1String("Termination:")));
    const QRect lastRect = f.view->visualRect(lastRow);
    QVERIFY2(lastRect.top() >= 0 && lastRect.bottom() <= f.view->viewport()->rect().bottom(),
             qPrintable(QStringLiteral("last line at %1,%2 %3x%4, viewport %5x%6")
                            .arg(lastRect.x()).arg(lastRect.y()).arg(lastRect.width()).arg(lastRect.height())
                            .arg(f.view->viewport()->width()).arg(f.view->viewport()->height())));
}

// (d) Switching to another job
void BJobLogScrollTest::jobSwitchPaintedAtBottom()
{
    Fixture f(false);
    QVERIFY(QTest::qWaitForWindowExposed(&f.window));
    f.view->showJob(44, false);
    emit f.director.jobLogResult(44, joblogReply(44, 1, 300, true));
    settle();
    f.bar()->setValue(f.bar()->maximum() / 2);   // the user looks at the middle
    settle();

    f.recorder->clear();
    f.view->showJob(45, false);
    settle();
    // The empty model of the new job: no rows painted at any position
    for (const auto &p : f.recorder->paints)
        QVERIFY2(p.rows == 0, qPrintable(describe(*f.recorder)));

    f.recorder->clear();
    emit f.director.jobLogResult(44, joblogReply(44, 1, 310, true));   // late, previous job
    settle();
    QCOMPARE(f.view->model()->rowCount(), 0);
    QVERIFY(f.recorder->changes.isEmpty());

    emit f.director.jobLogResult(45, joblogReply(45, 1, 500, true));
    settle();
    QVERIFY(!f.recorder->paints.isEmpty());
    for (const auto &p : f.recorder->paints) {
        if (p.rows > 0)
            QVERIFY2(p.value == p.maximum && p.maximum > 0, qPrintable(describe(*f.recorder)));
    }
    QCOMPARE(f.bar()->value(), f.bar()->maximum());
}

// The pane of the Jobs tab: the same view, with a search proxy in between
void BJobLogScrollTest::jobsTabPaneFollowsTail()
{
    BDirector director;
    BJobWidget widget(&director);
    widget.resize(1000, 700);
    widget.show();
    QVERIFY(QTest::qWaitForWindowExposed(&widget));
    auto *view = widget.findChild<BJobLogView *>();
    QVERIFY(view);
    QVERIFY(view->model() != widget.logModel());   // the proxy
    view->setMinimumHeight(200);
    ScrollRecorder recorder(view);
    QScrollBar *bar = view->verticalScrollBar();

    const quint64 job = 46;
    view->showJob(job, true);
    emit director.jobLogResult(job, joblogReply(job, 1, 200));
    settle();
    QVERIFY(bar->maximum() > 0);
    for (const auto &p : recorder.paints) {
        if (p.rows > 0)
            QVERIFY2(p.value == p.maximum, qPrintable(describe(recorder)));
    }

    // Following the tail
    for (int round = 0; round < 5; ++round) {
        const int floor = bar->value();
        recorder.clear();
        emit director.jobLogResult(job, joblogReply(job, 1, 200 + 5 * (round + 1)));
        settle();
        verifyFollowedTail(recorder, bar, floor, "pane");
    }

    // Scrolled up
    const int where = bar->maximum() / 2;
    bar->setValue(where);
    settle();
    recorder.clear();
    emit director.jobLogResult(job, joblogReply(job, 1, 260, true));
    settle();
    QVERIFY2(recorder.changes.isEmpty(), qPrintable(describe(recorder)));
    QCOMPARE(bar->value(), where);
}

// The job log dialog: opens on the bottom of a long log, nothing above painted
void BJobLogScrollTest::jobLogDialogOpensAtBottom()
{
    BDirector director;
    const QJsonObject job{{"jobid", "47"}, {"name", "backup-pdc"}, {"jobstatus", "T"},
                          {"starttime", "2026-11-17 21:00:03"}};
    BJobLogDialog dialog(job, &director);
    auto *view = dialog.findChild<BJobLogView *>();
    QVERIFY(view);
    ScrollRecorder recorder(view);
    dialog.resize(700, 400);
    dialog.show();
    QVERIFY(QTest::qWaitForWindowExposed(&dialog));
    settle();
    QCOMPARE(view->jobId(), quint64(47));

    emit director.jobLogResult(47, joblogReply(47, 1, 400, true));
    settle();
    QScrollBar *bar = view->verticalScrollBar();
    QVERIFY(bar->maximum() > 0);
    QVERIFY(!recorder.paints.isEmpty());
    for (const auto &p : recorder.paints) {
        if (p.rows > 0)
            QVERIFY2(p.value == p.maximum, qPrintable(describe(recorder)));
    }
    for (int i = 1; i < recorder.changes.size(); ++i)
        QVERIFY2(recorder.changes.at(i) >= recorder.changes.at(i - 1), qPrintable(describe(recorder)));
    QCOMPARE(bar->value(), bar->maximum());
}

// The job details dialog: its "Logs" tab is the same view
void BJobLogScrollTest::jobDetailsDialogOpensAtBottom()
{
    BDirector director;
    const QJsonObject job{{"jobid", "48"}, {"name", "backup-pdc"}, {"jobstatus", "T"},
                          {"type", "B"}, {"level", "F"}, {"client", "pdc-fd"},
                          {"starttime", "2026-11-17 21:00:03"}};
    BJobDetailsDialog dialog(job, nullptr, &director);
    auto *view = dialog.findChild<BJobLogView *>();
    auto *files = dialog.findChild<BJobFilesWidget *>();
    auto *tabs = dialog.findChild<QTabWidget *>();
    QVERIFY(view && files && tabs);
    ScrollRecorder recorder(view);
    dialog.resize(800, 500);
    dialog.show();
    QVERIFY(QTest::qWaitForWindowExposed(&dialog));
    for (int i = 0; i < tabs->count(); ++i) {
        if (tabs->widget(i)->isAncestorOf(view))
            tabs->setCurrentIndex(i);
    }
    emit files->filesLoaded();   // the dialog asks for the log after the file list
    settle();
    QCOMPARE(view->jobId(), quint64(48));

    emit director.jobLogResult(48, joblogReply(48, 1, 400, true));
    settle();
    QScrollBar *bar = view->verticalScrollBar();
    QVERIFY(bar->maximum() > 0);
    QVERIFY(!recorder.paints.isEmpty());
    for (const auto &p : recorder.paints) {
        if (p.rows > 0)
            QVERIFY2(p.value == p.maximum, qPrintable(describe(recorder)));
    }
    QCOMPARE(bar->value(), bar->maximum());
}

QTEST_MAIN(BJobLogScrollTest)
#include "bjoblogscroll_test.moc"
