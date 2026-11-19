/**
 * @file buiscale_test.cpp
 * @brief Font sizes and the interface zoom
 *
 * BUiScale: the roles relative to the base font, the clamping of the zoom,
 * pixel measures, style sheet sizes, and that the application, a bound
 * widget and a tracked measure follow a change without a restart.
 * BSettings: the zoom is stored and read back, a value out of range in the
 * file is brought into range. The schedule timeline: its rows, header and
 * label column at 100 % and at 200 %.
 *
 * Needs a QApplication but no display: CTest runs it on the offscreen
 * platform. The base font is set here, so that nothing depends on the font
 * of the machine. The settings go to a temporary directory.
 */

#include <QtTest>
#include <QApplication>
#include <QLabel>
#include <QSettings>
#include <QSignalSpy>
#include <QTemporaryDir>

#include "buiscale.h"
#include "config/bsettings.h"
#include "schedules/bscheduleganttwidget.h"

namespace {

QFont testFont()
{
    return QFont(QStringLiteral("DejaVu Sans"), 10);
}

BScheduleEntry entry(const QString &job, int hour, int minute, const QDate &date)
{
    BScheduleEntry e;
    e.scheduleName = QStringLiteral("Nightly");
    e.jobName = job;
    e.client = QStringLiteral("client-fd");
    e.level = BScheduleEntry::Incremental;
    e.levelName = QStringLiteral("Incremental");
    e.hour = hour;
    e.minute = minute;
    e.occurrences.append(QDateTime(date, QTime(hour, minute)));
    return e;
}

} // namespace

class BUiScaleTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    // --- The helper ---
    void zoomIsClamped_data();
    void zoomIsClamped();
    void rolesAreRelativeToTheBase();
    void scaledWorksOnAnyFont();
    void monospaceRole();
    void pixelsFollowTheFont();
    void styleSheetSizes();
    void zoomSteps();
    void applicationFollowsTheZoom();
    void boundWidgetAndTrackedMeasureFollow();

    // --- Persistence ---
    void zoomIsStored();
    void storedValueOutOfRange();

    // --- The schedule timeline ---
    void timelineGeometry_data();
    void timelineGeometry();
    void timelineFollowsAChange();
    void dayViewDrawsWhoWaitsForWhom();

private:
    QTemporaryDir m_config;
};

void BUiScaleTest::initTestCase()
{
    // The settings of the test, not those of the user: before BSettings is
    // used for the first time
    QVERIFY(m_config.isValid());
    qputenv("XDG_CONFIG_HOME", m_config.path().toUtf8());
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, m_config.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_config.path());
    const QSettings probe(QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
    QVERIFY2(probe.fileName().startsWith(m_config.path()), qPrintable(probe.fileName()));

    BUiScale::instance().setBaseFont(testFont());
}

void BUiScaleTest::init()
{
    BUiScale::instance().resetZoom();
}

// --- The helper ------------------------------------------------------------------

void BUiScaleTest::zoomIsClamped_data()
{
    QTest::addColumn<int>("asked");
    QTest::addColumn<int>("result");
    QTest::newRow("below the range") << 10 << 70;
    QTest::newRow("negative") << -50 << 70;
    QTest::newRow("lower limit") << 70 << 70;
    QTest::newRow("default") << 100 << 100;
    QTest::newRow("between steps, down") << 104 << 100;
    QTest::newRow("between steps, up") << 105 << 110;
    QTest::newRow("a step") << 150 << 150;
    QTest::newRow("upper limit") << 250 << 250;
    QTest::newRow("above the range") << 251 << 250;
    QTest::newRow("far above") << 4000 << 250;
}

void BUiScaleTest::zoomIsClamped()
{
    QFETCH(int, asked);
    QFETCH(int, result);
    QCOMPARE(BUiScale::clampZoom(asked), result);
    BUiScale::instance().setZoomPercent(asked);
    QCOMPARE(BUiScale::instance().zoomPercent(), result);
}

void BUiScaleTest::rolesAreRelativeToTheBase()
{
    BUiScale &scale = BUiScale::instance();
    QCOMPARE(scale.baseFont().pointSizeF(), 10.0);
    QCOMPARE(scale.font(BUiScale::Tiny).pointSizeF(), 8.0);
    QCOMPARE(scale.font(BUiScale::Small).pointSizeF(), 9.0);
    QCOMPARE(scale.font(BUiScale::Normal).pointSizeF(), 10.0);
    QCOMPARE(scale.font(BUiScale::Large).pointSizeF(), 12.0);
    QCOMPARE(scale.font(BUiScale::Title).pointSizeF(), 14.0);
    QCOMPARE(scale.font(BUiScale::Headline).pointSizeF(), 18.0);
    QCOMPARE(scale.font(BUiScale::Normal).families(), testFont().families());

    // The same proportions at another zoom: nothing is absolute
    scale.setZoomPercent(200);
    QCOMPARE(scale.baseFont().pointSizeF(), 10.0);
    QCOMPARE(scale.applicationFont().pointSizeF(), 20.0);
    QCOMPARE(scale.font(BUiScale::Tiny).pointSizeF(), 16.0);
    QCOMPARE(scale.font(BUiScale::Small).pointSizeF(), 18.0);
    QCOMPARE(scale.font(BUiScale::Headline).pointSizeF(), 36.0);

    // And with another base font
    scale.setZoomPercent(100);
    scale.setBaseFont(QFont(QStringLiteral("DejaVu Sans"), 12));
    QCOMPARE(scale.font(BUiScale::Small).pointSizeF(), 10.8);
    QCOMPARE(scale.font(BUiScale::Large).pointSizeF(), 14.4);
    scale.setBaseFont(testFont());

    // Styles
    QVERIFY(scale.font(BUiScale::Normal, BUiScale::Bold).bold());
    QVERIFY(scale.font(BUiScale::Normal, BUiScale::Italic).italic());
    QVERIFY(!scale.font(BUiScale::Normal).bold());
}

void BUiScaleTest::scaledWorksOnAnyFont()
{
    QFont points(QStringLiteral("DejaVu Sans"), 15);
    QCOMPARE(BUiScale::scaled(points, BUiScale::Tiny).pointSizeF(), 12.0);
    QCOMPARE(BUiScale::scaled(points, BUiScale::Normal).pointSizeF(), 15.0);

    // A font with a size in pixels stays one
    QFont pixels(QStringLiteral("DejaVu Sans"));
    pixels.setPixelSize(20);
    QCOMPARE(BUiScale::scaled(pixels, BUiScale::Tiny).pixelSize(), 16);
    QCOMPARE(BUiScale::scaled(pixels, BUiScale::Large).pixelSize(), 24);
    QCOMPARE(BUiScale::scaleOf(pixels), 1.5);
}

void BUiScaleTest::monospaceRole()
{
    const QFont mono = BUiScale::instance().font(BUiScale::Small, BUiScale::Monospace);
    QCOMPARE(mono.pointSizeF(), 9.0);
    QVERIFY(mono.fixedPitch());
    QCOMPARE(mono.styleHint(), QFont::Monospace);
    QVERIFY(mono.families() != testFont().families());
}

void BUiScaleTest::pixelsFollowTheFont()
{
    // 10 pt at 96 dpi is the design: the measure itself
    QCOMPARE(BUiScale::scaleOf(testFont()), 1.0);
    QCOMPARE(BUiScale::px(28), 28);
    QCOMPARE(BUiScale::px(28, QFont(QStringLiteral("DejaVu Sans"), 20)), 56);
    QCOMPARE(BUiScale::px(28, QFont(QStringLiteral("DejaVu Sans"), 11)), 31);
    // Never nothing
    QCOMPARE(BUiScale::px(1, QFont(QStringLiteral("DejaVu Sans"), 4)), 1);
    QCOMPARE(BUiScale::px(0), 0);

    BUiScale::instance().setZoomPercent(150);
    QCOMPARE(BUiScale::px(28), 42);
    QCOMPARE(BUiScale::px(16), 24);
}

void BUiScaleTest::styleSheetSizes()
{
    QCOMPARE(BUiScale::cssFontSize(BUiScale::Small), QStringLiteral("font-size:9.0pt"));
    QCOMPARE(BUiScale::resolveCss(QStringLiteral("QLabel { font-size: @headline; } "
                                                 "QCheckBox { font-size: @small; }")),
             QStringLiteral("QLabel { font-size: 18.0pt; } QCheckBox { font-size: 9.0pt; }"));
    BUiScale::instance().setZoomPercent(200);
    QCOMPARE(BUiScale::cssFontSize(BUiScale::Small), QStringLiteral("font-size:18.0pt"));
    QCOMPARE(BUiScale::resolveCss(QStringLiteral("a { font-size: @normal; }")),
             QStringLiteral("a { font-size: 20.0pt; }"));
    // Nothing to fill in
    QCOMPARE(BUiScale::resolveCss(QStringLiteral("a { color: red; }")),
             QStringLiteral("a { color: red; }"));
}

void BUiScaleTest::zoomSteps()
{
    BUiScale &scale = BUiScale::instance();
    QSignalSpy zoomSpy(&scale, &BUiScale::zoomChanged);

    scale.zoomIn();
    QCOMPARE(scale.zoomPercent(), 110);
    scale.zoomOut();
    scale.zoomOut();
    QCOMPARE(scale.zoomPercent(), 90);
    scale.resetZoom();
    QCOMPARE(scale.zoomPercent(), 100);
    QCOMPARE(zoomSpy.count(), 4);
    QCOMPARE(zoomSpy.last().at(0).toInt(), 100);

    // At the limits nothing changes and nothing is announced
    scale.setZoomPercent(BUiScale::MaxZoom);
    zoomSpy.clear();
    scale.zoomIn();
    QCOMPARE(scale.zoomPercent(), 250);
    QCOMPARE(zoomSpy.count(), 0);
    scale.setZoomPercent(BUiScale::MinZoom);
    zoomSpy.clear();
    scale.zoomOut();
    QCOMPARE(scale.zoomPercent(), 70);
    QCOMPARE(zoomSpy.count(), 0);
}

void BUiScaleTest::applicationFollowsTheZoom()
{
    QLabel plain(QStringLiteral("text"));
    plain.show();
    QCOMPARE(QApplication::font().pointSizeF(), 10.0);
    QCOMPARE(plain.font().pointSizeF(), 10.0);
    const int height = plain.sizeHint().height();

    BUiScale::instance().setZoomPercent(200);
    QCOMPARE(QApplication::font().pointSizeF(), 20.0);
    // A widget that is already open has the new font
    QCOMPARE(plain.font().pointSizeF(), 20.0);
    QVERIFY(plain.sizeHint().height() > height * 3 / 2);

    BUiScale::instance().setZoomPercent(70);
    QCOMPARE(plain.font().pointSizeF(), 7.0);
}

void BUiScaleTest::boundWidgetAndTrackedMeasureFollow()
{
    QLabel title(QStringLiteral("title"));
    BUiScale::bind(&title, BUiScale::Large, BUiScale::Bold);
    QLabel mono(QStringLiteral("mono"));
    BUiScale::bind(&mono, BUiScale::Small, BUiScale::Monospace);
    QCOMPARE(title.font().pointSizeF(), 12.0);
    QVERIFY(title.font().bold());
    QCOMPARE(mono.font().pointSizeF(), 9.0);

    int tracked = 0;
    int calls = 0;
    {
        QObject context;
        BUiScale::track(&context, [&]() { tracked = BUiScale::px(100); ++calls; });
        QCOMPARE(tracked, 100);     // at once
        QCOMPARE(calls, 1);

        BUiScale::instance().setZoomPercent(150);
        QCOMPARE(title.font().pointSizeF(), 18.0);
        QVERIFY(title.font().bold());
        QCOMPARE(mono.font().pointSizeF(), 13.5);
        QVERIFY(mono.font().fixedPitch());
        QCOMPARE(tracked, 150);
        QCOMPARE(calls, 2);
    }

    // The context is gone: no more calls
    BUiScale::instance().setZoomPercent(100);
    QCOMPARE(calls, 2);
    QCOMPARE(title.font().pointSizeF(), 12.0);
}

// --- Persistence -------------------------------------------------------------------

void BUiScaleTest::zoomIsStored()
{
    BSettings &settings = BSettings::instance();
    // Nothing stored: the default
    QCOMPARE(settings.appearanceUiZoom(), 100);

    settings.setAppearanceUiZoom(130);
    settings.sync();
    QCOMPARE(settings.appearanceUiZoom(), 130);

    // It is in the file, where the next start reads it
    {
        const QSettings file(QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
        QVERIFY2(file.fileName().startsWith(m_config.path()), qPrintable(file.fileName()));
        QCOMPARE(file.value(QStringLiteral("Appearance/ui_zoom")).toInt(), 130);
    }

    // What the main window does at start-up
    BUiScale::instance().setZoomPercent(settings.appearanceUiZoom());
    QCOMPARE(BUiScale::instance().zoomPercent(), 130);
    QCOMPARE(QApplication::font().pointSizeF(), 13.0);

    // A value that is not a step is stored as one
    settings.setAppearanceUiZoom(133);
    QCOMPARE(settings.appearanceUiZoom(), 130);
    settings.setAppearanceUiZoom(100);
}

void BUiScaleTest::storedValueOutOfRange()
{
    BSettings &settings = BSettings::instance();
    settings.sync();
    {
        // Written by hand or by another version
        QSettings file(QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
        file.setValue(QStringLiteral("Appearance/ui_zoom"), 999);
        file.sync();
    }
    settings.sync();
    QCOMPARE(settings.appearanceUiZoom(), 250);
    {
        QSettings file(QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
        file.setValue(QStringLiteral("Appearance/ui_zoom"), QStringLiteral("nonsense"));
        file.sync();
    }
    settings.sync();
    QCOMPARE(settings.appearanceUiZoom(), 70);
    settings.setAppearanceUiZoom(100);
    settings.sync();
}

// --- The schedule timeline ---------------------------------------------------------

void BUiScaleTest::timelineGeometry_data()
{
    QTest::addColumn<int>("zoom");
    QTest::addColumn<bool>("week");
    QTest::newRow("100 %, day") << 100 << false;
    QTest::newRow("200 %, day") << 200 << false;
    QTest::newRow("100 %, week") << 100 << true;
    QTest::newRow("200 %, week") << 200 << true;
    QTest::newRow("250 %, day") << 250 << false;
    QTest::newRow("70 %, week") << 70 << true;
}

void BUiScaleTest::timelineGeometry()
{
    QFETCH(int, zoom);
    QFETCH(bool, week);
    BUiScale::instance().setZoomPercent(zoom);

    const QDate date(2026, 11, 19);
    BScheduleGanttWidget gantt;
    gantt.setCurrentDate(date);
    gantt.setViewMode(week ? BScheduleGanttWidget::WeekView : BScheduleGanttWidget::DayView);
    gantt.setEntries({entry(QStringLiteral("BackupMail"), 21, 5, date),
                      entry(QStringLiteral("BackupWeb"), 21, 5, date),
                      entry(QStringLiteral("BackupCatalog"), 23, 10, date)});
    gantt.resize(1400, 600);
    gantt.show();
    QVERIFY(QTest::qWaitForWindowExposed(&gantt));

    QCOMPARE(gantt.font().pointSizeF(), 10.0 * zoom / 100);
    const BScheduleGanttWidget::Metrics m = gantt.metrics();
    const QFontMetrics fm(gantt.font());
    const QFontMetrics smallFm(BUiScale::scaled(gantt.font(), BUiScale::Tiny));
    const QFontMetrics smallBoldFm(BUiScale::scaled(gantt.font(), BUiScale::Tiny, BUiScale::Bold));

    // No row is lower than its text
    QVERIFY2(m.rowHeight >= fm.height(), qPrintable(QString::number(m.rowHeight)));
    QVERIFY(m.headerRow1Height >= smallBoldFm.height());
    QVERIFY(m.headerRow2Height >= smallFm.height());
    QCOMPARE(m.headerHeight, m.headerRow1Height + m.headerRow2Height);
    QVERIFY(m.heatmapHeight >= smallFm.height());
    QVERIFY(m.noHistoryMarkerWidth >= smallBoldFm.horizontalAdvance(QLatin1Char('?')));
    QCOMPARE(m.hourLabelWidth, smallFm.horizontalAdvance(QStringLiteral("00:00")));

    // The measures of the design, times the zoom
    QCOMPARE(m.labelWidth, qRound(230 * zoom / 100.0));
    QVERIFY(m.rowHeight >= qRound(28 * zoom / 100.0));

    // The hour labels of the header do not touch
    const int step = gantt.hourLabelStepMinutes();
    QVERIFY(step >= 60);
    QVERIFY2(step * gantt.zoomLevel() / 60 > m.hourLabelWidth,
             qPrintable(QStringLiteral("step %1 min at %2 px/h, label %3 px")
                            .arg(step).arg(gantt.zoomLevel()).arg(m.hourLabelWidth)));

    // The widget asks for the room its rows need: header, three job rows
    // and their group header, heat map
    QVERIFY(gantt.minimumHeight() >= m.headerHeight + 4 * m.rowHeight + m.heatmapHeight);
    QVERIFY(gantt.minimumSizeHint().height() >= m.headerHeight + 3 * m.rowHeight);

    // And it paints
    QVERIFY(!gantt.grab().isNull());
}

void BUiScaleTest::dayViewDrawsWhoWaitsForWhom()
{
    BUiScale::instance().setZoomPercent(100);
    const QDate date(2026, 11, 19);
    const BScheduleEntry mail = entry(QStringLiteral("BackupMail"), 21, 5, date);
    const BScheduleEntry web = entry(QStringLiteral("BackupWeb"), 21, 5, date);
    const QDateTime when(date, QTime(21, 5));

    // Web waits an hour for Mail: the client allows one job
    BScheduleGanttWidget::RunWait waits;
    waits.waitSecs = 3600;
    BScheduleGanttWidget::RunWait::Link link;
    link.blockerKey = BScheduleGanttWidget::waitKey(mail, when);
    link.freedAt = when.addSecs(3600);
    link.label = QStringLiteral("Client client-fd (1)");
    waits.links.append(link);
    BScheduleGanttWidget::RunWait plain = waits;
    plain.links.clear();

    auto render = [&](BScheduleGanttWidget::ViewMode mode, const BScheduleGanttWidget::RunWait &wait) {
        BScheduleGanttWidget gantt;
        gantt.setCurrentDate(date);
        gantt.setViewMode(mode);
        gantt.setEntries({mail, web});
        gantt.setWaits({{BScheduleGanttWidget::waitKey(web, when), wait}});
        gantt.resize(1400, 300);
        return gantt.grab().toImage();
    };

    // The day view draws the arrow, the week view does not
    const QImage dayWith = render(BScheduleGanttWidget::DayView, waits);
    QVERIFY(dayWith != render(BScheduleGanttWidget::DayView, plain));
    QCOMPARE(render(BScheduleGanttWidget::WeekView, waits), render(BScheduleGanttWidget::WeekView, plain));

    // For a look at it: ONESIMUS_LINKS_PNG=/path/links.png
    const QString png = qEnvironmentVariable("ONESIMUS_LINKS_PNG");
    if (!png.isEmpty())
        dayWith.save(png);
}

void BUiScaleTest::timelineFollowsAChange()
{
    const QDate date(2026, 11, 19);
    BScheduleGanttWidget gantt;
    gantt.setCurrentDate(date);
    gantt.setEntries({entry(QStringLiteral("BackupMail"), 21, 5, date)});
    gantt.resize(1400, 600);
    gantt.show();
    QVERIFY(QTest::qWaitForWindowExposed(&gantt));
    const BScheduleGanttWidget::Metrics before = gantt.metrics();
    const int minimumHeight = gantt.minimumHeight();

    // The widget is open: it follows without being built again
    BUiScale::instance().setZoomPercent(200);
    QCoreApplication::processEvents();
    const BScheduleGanttWidget::Metrics after = gantt.metrics();
    QCOMPARE(after.labelWidth, 2 * before.labelWidth);
    QVERIFY(after.rowHeight >= 2 * before.rowHeight - 2);
    QVERIFY(after.headerHeight > before.headerHeight * 3 / 2);
    QVERIFY(after.hourLabelWidth > before.hourLabelWidth * 3 / 2);
    QVERIFY(gantt.minimumHeight() > minimumHeight);

    BUiScale::instance().setZoomPercent(100);
    QCoreApplication::processEvents();
    QCOMPARE(gantt.metrics().rowHeight, before.rowHeight);
    QCOMPARE(gantt.metrics().labelWidth, before.labelWidth);
    QCOMPARE(gantt.minimumHeight(), minimumHeight);
}

QTEST_MAIN(BUiScaleTest)
#include "buiscale_test.moc"
