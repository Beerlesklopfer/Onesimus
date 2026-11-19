/**
 * @file bschedulegridwidget_test.cpp
 * @brief The schedule grid on screen
 *
 * What the widgets add to the model: selection with mouse and keyboard as
 * in the Logon Hours dialog, the list of runs of a cell, tooltips, the
 * scope bar, the lists beside the grid, the keyboard round with F6, the
 * table for screen readers, and a rendering smoke test that paints a few
 * schedules in both themes and checks that cells differ where they should.
 * Editing is tested in bschedulegridedit_test.cpp.
 *
 * Needs a QApplication but no display: CTest runs it on the offscreen
 * platform. With ONESIMUS_GRID_SCREENSHOT_DIR set to a directory the
 * rendering test also saves what it painted as PNG files there.
 *
 * Rows are those of a view that starts the week on Monday.
 */

#include <QtTest>

#include <QAbstractButton>
#include <QAccessible>
#include <QApplication>
#include <QTreeWidget>

#include "schedules/bschedulegridaccessible.h"
#include "schedules/bschedulegridview.h"
#include "schedules/bschedulegridwidget.h"
#include "schedules/bscheduleminicalendar.h"
#include "schedules/bschedulepalette.h"
#include "schedules/model/bschedulegridtext.h"

namespace {

enum Row { Mon = 0, Tue, Wed, Thu, Fri, Sat, Sun };

BSchedule scheduleOf(const QString &name, const QStringList &runs)
{
    BSchedule schedule;
    schedule.name = name;
    for (const QString &run : runs)
        schedule.runs.append(BRunParser::parse(run));
    return schedule;
}

/// Full on the first Saturday (to its own pool), Differential on the
/// others, Incremental on working days
BSchedule typicalSchedule()
{
    return scheduleOf(QStringLiteral("MonthlyCycle"), {
        QStringLiteral("Level=Full Pool=Monthly 1st sat at 21:00"),
        QStringLiteral("Level=Differential 2nd-5th sat at 21:00"),
        QStringLiteral("Level=Incremental mon-fri at 21:05"),
    });
}

BSchedule hourlySchedule()
{
    return scheduleOf(QStringLiteral("TenMinutes"), {
        QStringLiteral("Level=Incremental hourly at 0:05"),
        QStringLiteral("Level=Incremental hourly at 0:25"),
        QStringLiteral("Level=Incremental hourly at 0:45"),
        QStringLiteral("Level=Full sun at 3:15"),
    });
}

/// A calendar rule, a line that is not understood, one without a time
BSchedule mixedSchedule()
{
    return scheduleOf(QStringLiteral("FirstOfMonth"), {
        QStringLiteral("Level=Full Pool=Monthly on 1 at 21:00"),
        QStringLiteral("Level=Incremental mon-fri at 21:00"),
        QStringLiteral("Level=Differential every other sat at 21:00"),
        QStringLiteral("Level=Full w01/w02 sun at 23:10"),
        QStringLiteral("sat"),
    });
}

/// A view in a window of its own, shown and laid out
struct Shown
{
    explicit Shown(const BSchedule &schedule, const QSize &size = QSize(1280, 820))
    {
        view.setAttribute(Qt::WA_DontShowOnScreen, false);
        view.model()->setFirstDayOfWeek(Qt::Monday);
        view.setToday(QDate(2026, 11, 19));
        view.resize(size);
        view.setSchedule(schedule);
        view.setUsage({QStringLiteral("backup-client1"), QStringLiteral("backup-client2")}, true);
        view.show();
        ok = QTest::qWaitForWindowExposed(&view);
        QCoreApplication::processEvents();
    }

    BScheduleGridWidget *grid() { return view.grid(); }
    BScheduleGridModel *model() { return view.model(); }
    QWidget *viewport() { return view.grid()->viewport(); }
    QPoint cell(int row, int column) { return view.grid()->gridGeometry().cellRect(row, column).center(); }
    QPoint rowHeader(int row) { return view.grid()->gridGeometry().rowHeaderRect(row).center(); }
    QPoint columnHeader(int column) { return view.grid()->gridGeometry().columnHeaderRect(column).center(); }

    void click(const QPoint &pos, Qt::KeyboardModifiers modifiers = Qt::NoModifier)
    {
        QTest::mouseClick(viewport(), Qt::LeftButton, modifiers, pos);
    }

    void doubleClick(const QPoint &pos)
    {
        QTest::mouseDClick(viewport(), Qt::LeftButton, Qt::NoModifier, pos);
    }

    /// Press, move with the button down, release
    void drag(const QPoint &from, const QPoint &to, Qt::KeyboardModifiers modifiers = Qt::NoModifier)
    {
        QTest::mousePress(viewport(), Qt::LeftButton, modifiers, from);
        const int steps = 4;
        for (int i = 1; i <= steps; ++i) {
            const QPoint pos = from + (to - from) * i / steps;
            QMouseEvent move(QEvent::MouseMove, QPointF(pos), QPointF(viewport()->mapToGlobal(pos)),
                             Qt::NoButton, Qt::LeftButton, modifiers);
            QApplication::sendEvent(viewport(), &move);
        }
        QTest::mouseRelease(viewport(), Qt::LeftButton, modifiers, to);
    }

    void closeDetails()
    {
        if (QWidget *details = view.grid()->cellDetails())
            details->close();
        QCoreApplication::processEvents();
    }

    BScheduleGridView view;
    bool ok = false;
};

bool loadTheme(const QString &name)
{
    QFile file(QStringLiteral(ONESIMUS_THEME_DIR "/%1.qss").arg(name));
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return false;
    qApp->setStyleSheet(QString::fromUtf8(file.readAll()));
    return true;
}

/// What the grid told assistive technology: type and name of the interface
struct AccessibleNote
{
    QAccessible::Event type;
    QString name;
};
QList<AccessibleNote> accessibleNotes;
QObject *accessibleSource = nullptr;

void noteAccessibleEvent(QAccessibleEvent *event)
{
    if (event->object() != accessibleSource)
        return;
    QAccessibleInterface *iface = event->accessibleInterface();
    accessibleNotes.append(AccessibleNote{event->type(),
                                          iface ? iface->text(QAccessible::Name) : QString()});
}

bool noted(QAccessible::Event type, const QString &name = QString())
{
    for (const AccessibleNote &note : std::as_const(accessibleNotes)) {
        if (note.type == type && (name.isEmpty() || note.name == name))
            return true;
    }
    return false;
}

/// The mean colour of a part of an image
QColor meanColor(const QImage &image, const QRect &rect)
{
    const QRect area = rect.intersected(image.rect());
    qint64 red = 0;
    qint64 green = 0;
    qint64 blue = 0;
    qint64 count = 0;
    for (int y = area.top(); y <= area.bottom(); ++y) {
        for (int x = area.left(); x <= area.right(); ++x) {
            const QRgb pixel = image.pixel(x, y);
            red += qRed(pixel);
            green += qGreen(pixel);
            blue += qBlue(pixel);
            ++count;
        }
    }
    if (count == 0)
        return QColor();
    return QColor(int(red / count), int(green / count), int(blue / count));
}

int distance(const QColor &a, const QColor &b)
{
    return qAbs(a.red() - b.red()) + qAbs(a.green() - b.green()) + qAbs(a.blue() - b.blue());
}

/// How many pixels of two images differ within a rectangle
int differingPixels(const QImage &a, const QImage &b, const QRect &rect)
{
    int count = 0;
    const QRect area = rect.intersected(a.rect()).intersected(b.rect());
    for (int y = area.top(); y <= area.bottom(); ++y) {
        for (int x = area.left(); x <= area.right(); ++x) {
            if (a.pixel(x, y) != b.pixel(x, y))
                ++count;
        }
    }
    return count;
}

} // namespace

class BScheduleGridWidgetTest : public QObject
{
    Q_OBJECT

private slots:
    void cleanup();

    // --- Selection ---
    void mouseSelectsCells();
    void mouseSelectsDaysHoursAndAll();
    void keyboardMovesAndSelects();

    // --- Runs of a cell ---
    void listOfRunsOfACell();
    void toolTips();

    // --- The view around the grid ---
    void header();
    void scopeBar();
    void runLinesAndCalendarRules();
    void selectionLine();
    void miniCalendar();
    void f6GoesRound();
    void noSchedule();

    // --- Screen readers ---
    void accessibleTable();
    void accessibleEvents();

    // --- Rendering ---
    void rendering_data();
    void rendering();
    void renderingOfTheLoadBar();
};

void BScheduleGridWidgetTest::cleanup()
{
    qApp->setStyleSheet(QString());
}

// --- Selection -----------------------------------------------------------------

void BScheduleGridWidgetTest::mouseSelectsCells()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    const BGridSelection &selection = s.model()->selection();
    QVERIFY(selection.isEmpty());

    // A click: this cell only
    s.click(s.cell(Mon, 3));
    QCOMPARE(selection.count(), 1);
    QVERIFY(selection.contains(Mon, 3));
    QVERIFY(s.grid()->currentCell() == (BGridPos{Mon, 3}));
    s.click(s.cell(Tue, 5));
    QCOMPARE(selection.count(), 1);
    QVERIFY(selection.contains(Tue, 5));

    // Ctrl adds, and takes away again
    s.click(s.cell(Thu, 9), Qt::ControlModifier);
    QCOMPARE(selection.count(), 2);
    QVERIFY(selection.contains(Tue, 5) && selection.contains(Thu, 9));
    s.click(s.cell(Tue, 5), Qt::ControlModifier);
    QCOMPARE(selection.count(), 1);
    QVERIFY(selection.contains(Thu, 9));

    // Shift extends from the anchor to a rectangle
    s.click(s.cell(Tue, 5));
    s.click(s.cell(Wed, 7), Qt::ShiftModifier);
    QCOMPARE(selection.count(), 6);
    QVERIFY(selection.cells() == BGridCellSet::rect(Tue, 5, Wed, 7));
    // ... and the rectangle can shrink again
    s.click(s.cell(Tue, 6), Qt::ShiftModifier);
    QVERIFY(selection.cells() == BGridCellSet::rect(Tue, 5, Tue, 6));

    // Dragging spans a rectangle
    s.drag(s.cell(Mon, 0), s.cell(Wed, 3));
    QVERIFY(selection.cells() == BGridCellSet::rect(Mon, 0, Wed, 3));
    QVERIFY(s.grid()->currentCell() == (BGridPos{Wed, 3}));

    // Dragging with Ctrl adds a second rectangle
    s.drag(s.cell(Fri, 10), s.cell(Sat, 11), Qt::ControlModifier);
    QVERIFY(selection.cells()
            == BGridCellSet::rect(Mon, 0, Wed, 3).united(BGridCellSet::rect(Fri, 10, Sat, 11)));

    // Dragging beyond the grid stays on the last cells
    s.drag(s.cell(Sat, 22), s.cell(Sun, 23) + QPoint(400, 400));
    QVERIFY(selection.cells() == BGridCellSet::rect(Sat, 22, Sun, 23));
    QVERIFY(!s.grid()->cellDetails());
}

void BScheduleGridWidgetTest::mouseSelectsDaysHoursAndAll()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    const BGridSelection &selection = s.model()->selection();

    // A row header: the day
    s.click(s.rowHeader(Wed));
    QCOMPARE(selection.count(), 24);
    QVERIFY(selection.isRowSelected(Wed));

    // Ctrl adds a day and takes it away again
    s.click(s.rowHeader(Fri), Qt::ControlModifier);
    QCOMPARE(selection.count(), 48);
    QVERIFY(selection.isRowSelected(Wed) && selection.isRowSelected(Fri));
    s.click(s.rowHeader(Wed), Qt::ControlModifier);
    QCOMPARE(selection.count(), 24);
    QVERIFY(selection.isRowSelected(Fri));

    // Shift extends over the days between
    s.click(s.rowHeader(Mon));
    s.click(s.rowHeader(Wed), Qt::ShiftModifier);
    QCOMPARE(selection.count(), 72);
    QVERIFY(selection.isRowSelected(Mon) && selection.isRowSelected(Tue) && selection.isRowSelected(Wed));

    // Dragging over row headers
    s.drag(s.rowHeader(Thu), s.rowHeader(Sat));
    QCOMPARE(selection.count(), 72);
    QVERIFY(selection.isRowSelected(Thu) && selection.isRowSelected(Fri) && selection.isRowSelected(Sat));

    // A column header: the hour on every day
    s.click(s.columnHeader(21));
    QCOMPARE(selection.count(), 7);
    QVERIFY(selection.isColumnSelected(21));
    s.click(s.columnHeader(3), Qt::ControlModifier);
    QCOMPARE(selection.count(), 14);
    s.drag(s.columnHeader(8), s.columnHeader(17));
    QCOMPARE(selection.count(), 70);
    QVERIFY(selection.isColumnSelected(8) && selection.isColumnSelected(17));
    QVERIFY(!selection.isColumnSelected(7) && !selection.isColumnSelected(18));

    // The corner: everything
    s.click(s.grid()->gridGeometry().cornerRect().center());
    QCOMPARE(selection.count(), 7 * 24);
}

void BScheduleGridWidgetTest::keyboardMovesAndSelects()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    BScheduleGridWidget *grid = s.grid();
    const BGridSelection &selection = s.model()->selection();
    grid->setFocus();
    s.click(s.cell(Wed, 5));

    // Arrows move, and the cell they reach is the selection
    QTest::keyClick(grid, Qt::Key_Right);
    QVERIFY(grid->currentCell() == (BGridPos{Wed, 6}));
    QCOMPARE(selection.count(), 1);
    QVERIFY(selection.contains(Wed, 6));
    QTest::keyClick(grid, Qt::Key_Up);
    QTest::keyClick(grid, Qt::Key_Left);
    QVERIFY(grid->currentCell() == (BGridPos{Tue, 5}));
    QVERIFY(selection.contains(Tue, 5));

    // Shift+arrows extend from where the keyboard was
    QTest::keyClick(grid, Qt::Key_Right, Qt::ShiftModifier);
    QTest::keyClick(grid, Qt::Key_Right, Qt::ShiftModifier);
    QTest::keyClick(grid, Qt::Key_Down, Qt::ShiftModifier);
    QVERIFY(selection.cells() == BGridCellSet::rect(Tue, 5, Wed, 7));
    QVERIFY(grid->currentCell() == (BGridPos{Wed, 7}));
    QTest::keyClick(grid, Qt::Key_Left, Qt::ShiftModifier);
    QVERIFY(selection.cells() == BGridCellSet::rect(Tue, 5, Wed, 6));

    // Home and End: first and last hour of the day
    QTest::keyClick(grid, Qt::Key_Home);
    QVERIFY(grid->currentCell() == (BGridPos{Wed, 0}));
    QCOMPARE(selection.count(), 1);
    QTest::keyClick(grid, Qt::Key_End, Qt::ShiftModifier);
    QVERIFY(selection.isRowSelected(Wed));
    QCOMPARE(selection.count(), 24);

    // With Ctrl: first and last cell
    QTest::keyClick(grid, Qt::Key_Home, Qt::ControlModifier);
    QVERIFY(grid->currentCell() == (BGridPos{Mon, 0}));
    QTest::keyClick(grid, Qt::Key_End, Qt::ControlModifier);
    QVERIFY(grid->currentCell() == (BGridPos{Sun, 23}));
    QVERIFY(selection.contains(Sun, 23));

    // The edges hold
    QTest::keyClick(grid, Qt::Key_Right);
    QTest::keyClick(grid, Qt::Key_Down);
    QVERIFY(grid->currentCell() == (BGridPos{Sun, 23}));
    QTest::keyClick(grid, Qt::Key_PageUp);
    QVERIFY(grid->currentCell() == (BGridPos{Mon, 23}));
    QTest::keyClick(grid, Qt::Key_PageDown, Qt::ShiftModifier);
    QVERIFY(selection.isColumnSelected(23));

    // Ctrl+A
    QTest::keyClick(grid, Qt::Key_A, Qt::ControlModifier);
    QCOMPARE(selection.count(), 7 * 24);

    // None of these keys edits: the schedule is as it was. Space, Del and
    // the level keys do (bschedulegridedit_test.cpp).
    QVERIFY(!s.model()->isModified());
    QCOMPARE(selection.count(), 7 * 24);
}

// --- Runs of a cell --------------------------------------------------------------

void BScheduleGridWidgetTest::listOfRunsOfACell()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    BScheduleGridWidget *grid = s.grid();

    // A click on a cell with one run only selects
    s.click(s.cell(Mon, 21));
    QVERIFY(!grid->cellDetails());

    // Enter lists it
    grid->setFocus();
    QTest::keyClick(grid, Qt::Key_Return);
    QVERIFY(grid->cellDetails());
    auto *tree = grid->cellDetails()->findChild<QTreeWidget *>(QStringLiteral("scheduleGridCellRuns"));
    QVERIFY(tree);
    QCOMPARE(tree->topLevelItemCount(), 1);
    QCOMPARE(tree->topLevelItem(0)->text(0), QStringLiteral("21:05"));
    QCOMPARE(tree->topLevelItem(0)->text(1), QStringLiteral("Incremental"));
    QCOMPARE(tree->topLevelItem(0)->text(3), QStringLiteral("Every week"));
    QCOMPARE(tree->topLevelItem(0)->text(4), QStringLiteral("3: Level=Incremental mon-fri at 21:05"));
    s.closeDetails();
    QVERIFY(!grid->cellDetails());

    // Enter on an empty cell: nothing to list
    s.click(s.cell(Mon, 3));
    QTest::keyClick(grid, Qt::Key_Return);
    QVERIFY(!grid->cellDetails());

    // A plain click on a cell with several runs only selects it ...
    s.click(s.cell(Sat, 21));
    QVERIFY(!grid->cellDetails());
    QVERIFY(s.model()->selection().contains(Sat, 21));
    // ... a double click lists them; here two of other scopes
    s.doubleClick(s.cell(Sat, 21));
    QVERIFY(grid->cellDetails());
    tree = grid->cellDetails()->findChild<QTreeWidget *>(QStringLiteral("scheduleGridCellRuns"));
    QVERIFY(tree);
    QCOMPARE(tree->topLevelItemCount(), 2);
    QCOMPARE(tree->topLevelItem(0)->text(0), QStringLiteral("21:00"));
    QCOMPARE(tree->topLevelItem(0)->text(1), QStringLiteral("Full"));
    QCOMPARE(tree->topLevelItem(0)->text(2), QStringLiteral("[A] pool Monthly"));
    QCOMPARE(tree->topLevelItem(0)->text(3), QStringLiteral("1st (other scope, may run on the same day)"));
    QCOMPARE(tree->topLevelItem(0)->text(4),
             QStringLiteral("1: Level=Full Pool=Monthly 1st sat at 21:00"));
    QCOMPARE(tree->topLevelItem(1)->text(1), QStringLiteral("Differential"));
    QCOMPARE(tree->topLevelItem(1)->text(2), QString());

    // Choosing a run there shows its Run line: the scope of the line and its cells
    emit tree->itemActivated(tree->topLevelItem(0), 0);
    s.closeDetails();
    QCOMPARE(BScheduleGridText::scopeLabel(s.model()->activeScope()), QStringLiteral("1st"));
    QCOMPARE(grid->highlightedRun(), 0);
    QVERIFY(s.view.runLineList()->currentItem());
    QCOMPARE(s.view.runLineList()->currentItem()->text(1),
             QStringLiteral("Level=Full Pool=Monthly 1st sat at 21:00"));

    // In "1st" the Differential never coincides, the Incremental may
    s.doubleClick(s.cell(Sat, 21));
    QVERIFY(grid->cellDetails());
    tree = grid->cellDetails()->findChild<QTreeWidget *>(QStringLiteral("scheduleGridCellRuns"));
    QCOMPARE(tree->topLevelItemCount(), 2);
    QCOMPARE(tree->topLevelItem(0)->text(3), QStringLiteral("1st"));
    QCOMPARE(tree->topLevelItem(1)->text(3),
             QStringLiteral("2nd-5th (other scope, never on the same day)"));
    s.closeDetails();

    // Dragging does not open the list, also not if it ends where it began
    s.drag(s.cell(Sat, 21), s.cell(Sat, 22));
    QVERIFY(!grid->cellDetails());
    // ... and a click with a modifier does not either
    s.click(s.cell(Sat, 21), Qt::ControlModifier);
    QVERIFY(!grid->cellDetails());
}

void BScheduleGridWidgetTest::toolTips()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    BScheduleGridWidget *grid = s.grid();

    QString tip = grid->toolTipAt(s.cell(Mon, 21));
    QVERIFY2(tip.contains(QStringLiteral("Monday, 21:00 to 21:59")), qPrintable(tip));
    QVERIFY2(tip.contains(QStringLiteral("Incremental at 21:05")), qPrintable(tip));
    QVERIFY2(tip.contains(QStringLiteral("Run line 3: Level=Incremental mon-fri at 21:05")), qPrintable(tip));

    tip = grid->toolTipAt(s.cell(Mon, 3));
    QVERIFY2(tip.contains(QStringLiteral("No run in the scope")), qPrintable(tip));

    // Runs of other scopes say whether they can meet this one
    tip = grid->toolTipAt(s.cell(Sat, 21));
    QVERIFY2(tip.contains(QStringLiteral("[A] Full at 21:00, pool Monthly")), qPrintable(tip));
    QVERIFY2(tip.contains(QStringLiteral("1st: may run on the same day as this scope")), qPrintable(tip));
    s.model()->setActiveScope(1);
    tip = grid->toolTipAt(s.cell(Sat, 21));
    QVERIFY2(tip.contains(QStringLiteral("2nd-5th: never on the same day as this scope")), qPrintable(tip));

    // Headers and corner
    QVERIFY(grid->toolTipAt(s.rowHeader(Sat)).startsWith(QStringLiteral("Saturday:")));
    QVERIFY(grid->toolTipAt(s.columnHeader(21)).startsWith(QStringLiteral("21:00 to 21:59:")));
    QVERIFY(grid->toolTipAt(grid->gridGeometry().cornerRect().center()).contains(QStringLiteral("Ctrl+A")));
    QVERIFY(grid->toolTipAt(QPoint(-5, -5)).isEmpty());

    // The band of calendar rules
    Shown mixed(mixedSchedule());
    QVERIFY(mixed.ok);
    const BScheduleGridGeometry &geometry = mixed.grid()->gridGeometry();
    QVERIFY(geometry.hasRuleBand());
    tip = mixed.grid()->toolTipAt(geometry.ruleCellRect(21).center());
    QVERIFY2(tip.contains(QStringLiteral("R1")), qPrintable(tip));
    QVERIFY2(tip.contains(QStringLiteral("on the 1st of every month at 21:00")), qPrintable(tip));
    QVERIFY(mixed.grid()->toolTipAt(geometry.ruleCellRect(5).center()).isEmpty());
    QVERIFY(mixed.grid()->toolTipAt(geometry.ruleHeaderRect().center()).contains(
        QStringLiteral("cannot be selected")));
    // The line without a time warns
    tip = mixed.grid()->toolTipAt(mixed.cell(Sat, 4));
    QVERIFY2(tip.contains(QStringLiteral("the Run line names no time")), qPrintable(tip));
}

// --- The view around the grid ------------------------------------------------------

void BScheduleGridWidgetTest::header()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    QCOMPARE(s.view.titleText(), QStringLiteral("Schedule MonthlyCycle"));
    QCOMPARE(s.view.usageText(), QStringLiteral("Used by 2 job(s)"));
    s.view.setUsage({}, true);
    QCOMPARE(s.view.usageText(), QStringLiteral("Not used by any job"));
    s.view.setUsage({}, false);
    QCOMPARE(s.view.usageText(), QStringLiteral("(jobs not loaded yet)"));
}

void BScheduleGridWidgetTest::scopeBar()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);

    QList<QAbstractButton *> buttons = s.view.scopeButtons();
    QCOMPARE(buttons.size(), 3);
    QCOMPARE(buttons[0]->text(), QStringLiteral("Every week (1)"));
    QCOMPARE(buttons[1]->text(), QStringLiteral("1st (1)"));
    QCOMPARE(buttons[2]->text(), QStringLiteral("2nd-5th (1)"));
    QVERIFY(buttons[0]->isChecked());
    QVERIFY(!buttons[1]->isChecked());
    QVERIFY(!buttons[0]->property("scopeEmpty").toBool());

    // In "Every week" the Saturday shows only runs of other scopes
    QVERIFY(s.model()->cell(Sat, 21).inScope.isEmpty());
    QCOMPARE(s.model()->cell(Sat, 21).outOfScope.size(), 2);

    // A click changes the scope of the model
    QTest::mouseClick(buttons[1], Qt::LeftButton);
    QCOMPARE(s.model()->activeScopeIndex(), 1);
    QVERIFY(buttons[1]->isChecked());
    QVERIFY(!buttons[0]->isChecked());
    QCOMPARE(s.model()->cell(Sat, 21).inScope.size(), 1);

    // A change of the model checks the button
    s.model()->setActiveScope(2);
    QVERIFY(s.view.scopeButtons()[2]->isChecked());

    // A scope without lines is shown, dimmed by the theme
    BGridScope third;
    third.weeksOfMonth = 1 << 2;
    const int added = s.model()->addScope(third);
    QVERIFY(added >= 0);
    buttons = s.view.scopeButtons();
    QCOMPARE(buttons.size(), 4);
    QCOMPARE(buttons[added]->text(), QStringLiteral("3rd (0)"));
    QVERIFY(buttons[added]->property("scopeEmpty").toBool());
    QVERIFY(buttons[2]->isChecked());
}

void BScheduleGridWidgetTest::runLinesAndCalendarRules()
{
    Shown s(mixedSchedule());
    QVERIFY(s.ok);
    QTreeWidget *lines = s.view.runLineList();
    QTreeWidget *rules = s.view.ruleList();

    // Every line, in schedule order
    QCOMPARE(lines->topLevelItemCount(), 5);
    QCOMPARE(lines->topLevelItem(0)->text(0), QStringLiteral("1"));
    QCOMPARE(lines->topLevelItem(0)->text(1), QStringLiteral("Level=Full Pool=Monthly on 1 at 21:00"));
    QCOMPARE(lines->topLevelItem(0)->text(2), QStringLiteral("calendar rule R1"));
    QCOMPARE(lines->topLevelItem(1)->text(2), QStringLiteral("scope Every week"));
    QCOMPARE(lines->topLevelItem(3)->text(2), QStringLiteral("calendar rule R2"));
    QCOMPARE(lines->topLevelItem(4)->text(1), QStringLiteral("sat"));

    // The line that is not understood: the Director's text, the reason, a sign
    QTreeWidgetItem *bad = lines->topLevelItem(2);
    QCOMPARE(bad->text(1), QStringLiteral("Level=Differential every other sat at 21:00"));
    QVERIFY2(bad->text(2).startsWith(QStringLiteral("not understood: ")), qPrintable(bad->text(2)));
    QCOMPARE(bad->text(2), QStringLiteral("not understood: ") + s.model()->notUnderstood().first().reason);
    QVERIFY(!bad->icon(0).isNull());
    QVERIFY(lines->topLevelItem(1)->icon(0).isNull());
    QVERIFY(bad->toolTip(1).contains(QStringLiteral("cannot be written")));

    // The calendar rules with their reading
    QVERIFY(rules->isVisibleTo(&s.view));
    QCOMPARE(rules->topLevelItemCount(), 2);
    QCOMPARE(rules->topLevelItem(0)->text(0), QStringLiteral("R1"));
    QCOMPARE(rules->topLevelItem(0)->text(1), QStringLiteral("Full on the 1st of every month at 21:00"));
    QCOMPARE(rules->topLevelItem(0)->text(2), QStringLiteral("Level=Full Pool=Monthly on 1 at 21:00"));
    QCOMPARE(rules->topLevelItem(1)->text(1),
             QStringLiteral("Full in every 2nd week of the year (starting with week 1), on Sunday, at 23:10"));
    QCOMPARE(s.grid()->rulesAtHour(21), QList<int>({0}));
    QCOMPARE(s.grid()->rulesAtHour(23), QList<int>({1}));
    QVERIFY(s.grid()->rulesAtHour(5).isEmpty());

    // Choosing a grid line outlines its cells
    lines->setCurrentItem(lines->topLevelItem(1));
    QCOMPARE(s.grid()->highlightedRun(), 1);
    QCOMPARE(s.grid()->highlightedRule(), -1);
    QVERIFY(!rules->currentItem());

    // Choosing a calendar rule marks it in the band and in its list
    lines->setCurrentItem(lines->topLevelItem(3));
    QCOMPARE(s.grid()->highlightedRun(), -1);
    QCOMPARE(s.grid()->highlightedRule(), 1);
    QCOMPARE(rules->currentItem(), rules->topLevelItem(1));

    // ... and the other way round, also from a click on the band
    rules->setCurrentItem(rules->topLevelItem(0));
    QCOMPARE(s.grid()->highlightedRule(), 0);
    QCOMPARE(lines->currentItem(), lines->topLevelItem(0));
    s.click(s.grid()->gridGeometry().ruleCellRect(23).center());
    QCOMPARE(s.grid()->highlightedRule(), 1);
    QCOMPARE(lines->currentItem(), lines->topLevelItem(3));
    // The band is not a cell: nothing was selected
    QVERIFY(s.model()->selection().isEmpty());

    // A line that is not understood has nothing to show
    lines->setCurrentItem(bad);
    QCOMPARE(s.grid()->highlightedRun(), -1);
    QCOMPARE(s.grid()->highlightedRule(), -1);

    // A line of another scope switches to that scope
    Shown typical(typicalSchedule());
    QVERIFY(typical.ok);
    QVERIFY(!typical.view.ruleList()->isVisibleTo(&typical.view));
    QCOMPARE(typical.model()->activeScopeIndex(), 0);
    QTreeWidget *typicalLines = typical.view.runLineList();
    QCOMPARE(typicalLines->topLevelItem(1)->text(2), QStringLiteral("scope 2nd-5th"));
    typicalLines->setCurrentItem(typicalLines->topLevelItem(1));
    QCOMPARE(BScheduleGridText::scopeLabel(typical.model()->activeScope()), QStringLiteral("2nd-5th"));
    QCOMPARE(typical.grid()->highlightedRun(), 1);
    QVERIFY(typical.view.scopeButtons()[2]->isChecked());
    // Esc in the grid takes the outline away
    typical.grid()->setFocus();
    QTest::keyClick(typical.grid(), Qt::Key_Escape);
    QCOMPARE(typical.grid()->highlightedRun(), -1);
}

void BScheduleGridWidgetTest::selectionLine()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    QVERIFY(s.view.selectionText().startsWith(QStringLiteral("Nothing selected.")));

    s.click(s.columnHeader(21));
    QCOMPARE(s.view.selectionText(),
             QStringLiteral("7 cell(s) selected: Incremental at :05 in 5 cell(s). Other scopes: "
                            "Full at :00, pool Monthly (1st) in 1 cell(s); "
                            "Differential at :00 (2nd-5th) in 1 cell(s). "
                            "They stay when this scope is painted or erased."));
    s.click(s.cell(Mon, 3));
    QCOMPARE(s.view.selectionText(), QStringLiteral("1 cell(s) selected: no run in this scope."));

    // The line follows the scope
    s.click(s.cell(Sat, 21), Qt::ControlModifier);
    s.model()->setActiveScope(1);
    QCOMPARE(s.view.selectionText(),
             QStringLiteral("2 cell(s) selected: Full at :00, pool Monthly in 1 cell(s). Other scopes: "
                            "Differential at :00 (2nd-5th) in 1 cell(s). "
                            "They stay when this scope is painted or erased."));
}

void BScheduleGridWidgetTest::miniCalendar()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    BScheduleMiniCalendar *calendar = s.view.calendar();

    // Five weeks from the week of "today" (Thursday 19 November 2026)
    QCOMPARE(calendar->firstDate(), QDate(2026, 11, 16));
    QCOMPARE(calendar->lastDate(), QDate(2026, 12, 20));
    QCOMPARE(calendar->days().size(), 35);
    QCOMPARE(calendar->rangeText(), QStringLiteral("2026-11-16 to 2026-12-20"));
    QCOMPARE(calendar->currentDate(), QDate(2026, 11, 19));

    // The first Saturday of December is the Full
    QCOMPARE(calendar->dayText(QDate(2026, 12, 5)),
             QStringLiteral("Saturday 2026-12-05: 21:00 Full (Run line 1)"));
    QCOMPARE(calendar->dayText(QDate(2026, 11, 28)),
             QStringLiteral("Saturday 2026-11-28: 21:00 Differential (Run line 2)"));
    QCOMPARE(calendar->dayText(QDate(2026, 11, 29)), QStringLiteral("Sunday 2026-11-29: no run"));

    // Positions and dates, there and back
    for (const BScheduleCalendarDay &day : calendar->days())
        QCOMPARE(calendar->dateAt(calendar->dayRect(day.date).center()), day.date);
    QTest::mouseClick(calendar, Qt::LeftButton, Qt::NoModifier,
                      calendar->dayRect(QDate(2026, 12, 5)).center());
    QCOMPARE(calendar->currentDate(), QDate(2026, 12, 5));
    QVERIFY(calendar->accessibleName().contains(QStringLiteral("2026-12-05: 21:00 Full")));

    // By month, and back to today
    QTest::keyClick(calendar, Qt::Key_PageDown);
    QCOMPARE(calendar->firstDate(), QDate(2026, 11, 30));
    QTest::keyClick(calendar, Qt::Key_PageUp);
    QCOMPARE(calendar->firstDate(), QDate(2026, 10, 26));
    QTest::keyClick(calendar, Qt::Key_Home);
    QCOMPARE(calendar->firstDate(), QDate(2026, 11, 16));
    QCOMPARE(calendar->currentDate(), QDate(2026, 11, 19));
    QTest::keyClick(calendar, Qt::Key_Down);
    QTest::keyClick(calendar, Qt::Key_Right);
    QCOMPARE(calendar->currentDate(), QDate(2026, 11, 27));

    // Another schedule, other dates
    s.view.setSchedule(mixedSchedule());
    QVERIFY(calendar->dayText(QDate(2026, 12, 1)).contains(QStringLiteral("21:00 Full (Run line 1)")));
}

void BScheduleGridWidgetTest::f6GoesRound()
{
    Shown s(mixedSchedule());
    QVERIFY(s.ok);
    s.view.activateWindow();
    s.grid()->setFocus();
    QTRY_VERIFY(s.grid()->hasFocus());

    // Grid, palette, Run lines, calendar rules, calendar, scope bar, grid
    s.view.focusNextPart();
    QVERIFY(s.view.palette()->activeButton()->hasFocus());
    s.view.focusNextPart();
    QVERIFY(s.view.runLineList()->hasFocus());
    s.view.focusNextPart();
    QVERIFY(s.view.ruleList()->hasFocus());
    s.view.focusNextPart();
    QVERIFY(s.view.calendar()->hasFocus());
    s.view.focusNextPart();
    QVERIFY(s.view.scopeButtons().first()->hasFocus());
    s.view.focusNextPart();
    QVERIFY(s.grid()->hasFocus());

    // Backwards
    s.view.focusNextPart(true);
    QVERIFY(s.view.scopeButtons().first()->hasFocus());
    s.view.focusNextPart(true);
    QVERIFY(s.view.calendar()->hasFocus());

    // The key itself
    QTest::keyClick(s.view.calendar(), Qt::Key_F6);
    QTRY_VERIFY(s.view.scopeButtons().first()->hasFocus());
    QTest::keyClick(s.view.scopeButtons().first(), Qt::Key_F6);
    QTRY_VERIFY(s.grid()->hasFocus());
    QTest::keyClick(s.grid(), Qt::Key_F6, Qt::ShiftModifier);
    QTRY_VERIFY(s.view.scopeButtons().first()->hasFocus());

    // Without calendar rules their list is left out
    Shown typical(typicalSchedule());
    QVERIFY(typical.ok);
    typical.view.activateWindow();
    typical.view.runLineList()->setFocus();
    QTRY_VERIFY(typical.view.runLineList()->hasFocus());
    typical.view.focusNextPart();
    QVERIFY(typical.view.calendar()->hasFocus());
}

void BScheduleGridWidgetTest::noSchedule()
{
    BScheduleGridView view;
    QVERIFY(!view.hasSchedule());
    view.resize(900, 600);
    view.show();
    QVERIFY(QTest::qWaitForWindowExposed(&view));
    // Nothing chosen: the grid is not on display
    QVERIFY(!view.grid()->isVisible());

    view.setSchedule(typicalSchedule());
    QVERIFY(view.hasSchedule());
    QVERIFY(view.grid()->isVisible());

    view.clear();
    QVERIFY(!view.hasSchedule());
    QVERIFY(!view.grid()->isVisible());
    QCOMPARE(view.runLineList()->topLevelItemCount(), 0);
}

// --- Screen readers ------------------------------------------------------------------

void BScheduleGridWidgetTest::accessibleTable()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    BScheduleGridWidget *grid = s.grid();
    s.model()->setActiveScope(1);   // "1st"

    QAccessibleInterface *iface = QAccessible::queryAccessibleInterface(grid);
    QVERIFY(iface);
    QCOMPARE(iface->role(), QAccessible::Table);
    QCOMPARE(iface->text(QAccessible::Name), QStringLiteral("Schedule grid of MonthlyCycle, scope 1st"));
    QAccessibleTableInterface *table = iface->tableInterface();
    QVERIFY(table);
    QCOMPARE(table->rowCount(), 7);
    QCOMPARE(table->columnCount(), 24);
    QCOMPARE(table->rowDescription(Sat), QStringLiteral("Saturday"));
    QCOMPARE(table->columnDescription(21), QStringLiteral("21 hours"));
    QCOMPARE(iface->childCount(), 8 * 25);

    // A cell: its name is the sentence of the plan
    QAccessibleInterface *cell = table->cellAt(Sat, 21);
    QVERIFY(cell);
    QCOMPARE(cell->role(), QAccessible::Cell);
    QCOMPARE(cell->text(QAccessible::Name),
             QStringLiteral("Saturday 21 hours: Full at 21:00, pool Monthly, first week only; "
                            "in other scopes: Differential at 21:00, only 2nd-5th"));
    QCOMPARE(cell->parent(), iface);
    QCOMPARE(iface->indexOfChild(cell), BScheduleGridAccessible::childIndex(Sat, 21));
    QCOMPARE(iface->child(BScheduleGridAccessible::childIndex(Sat, 21)), cell);
    QCOMPARE(table->cellAt(Mon, 3)->text(QAccessible::Name), QStringLiteral("Monday 3 hours: no run"));
    QVERIFY(!table->cellAt(7, 0));
    QVERIFY(!table->cellAt(0, 24));

    // Its place in the table and its headers
    QAccessibleTableCellInterface *tableCell = cell->tableCellInterface();
    QVERIFY(tableCell);
    QCOMPARE(tableCell->rowIndex(), int(Sat));
    QCOMPARE(tableCell->columnIndex(), 21);
    QCOMPARE(tableCell->table(), iface);
    QCOMPARE(tableCell->rowHeaderCells().size(), 1);
    QCOMPARE(tableCell->rowHeaderCells().first()->role(), QAccessible::RowHeader);
    QCOMPARE(tableCell->rowHeaderCells().first()->text(QAccessible::Name), QStringLiteral("Saturday"));
    QCOMPARE(tableCell->columnHeaderCells().size(), 1);
    QCOMPARE(tableCell->columnHeaderCells().first()->role(), QAccessible::ColumnHeader);
    QCOMPARE(tableCell->columnHeaderCells().first()->text(QAccessible::Name), QStringLiteral("21 hours"));
    QAccessibleInterface *corner = iface->child(BScheduleGridAccessible::childIndex(-1, -1));
    QCOMPARE(corner->text(QAccessible::Name), QStringLiteral("Select all"));

    // Where it is
    const QRect onScreen(grid->viewport()->mapToGlobal(grid->gridGeometry().cellRect(Sat, 21).topLeft()),
                         grid->gridGeometry().cellRect(Sat, 21).size());
    QCOMPARE(cell->rect(), onScreen);
    QCOMPARE(iface->childAt(onScreen.center().x(), onScreen.center().y()), cell);

    // Selection and focus
    QVERIFY(!cell->state().selected);
    QVERIFY(!tableCell->isSelected());
    QCOMPARE(table->selectedCellCount(), 0);
    s.view.activateWindow();
    grid->setFocus();
    s.click(s.cell(Sat, 21), Qt::ControlModifier);   // with Ctrl: no list of runs
    QVERIFY(cell->state().selected);
    QVERIFY(tableCell->isSelected());
    // The view edits: a cell can be painted, a header cannot
    QVERIFY(cell->state().selectable && cell->state().focusable && !cell->state().readOnly);
    QCOMPARE(table->selectedCellCount(), 1);
    QCOMPARE(table->selectedCells(), QList<QAccessibleInterface *>({cell}));
    QTRY_VERIFY(grid->hasFocus());
    QVERIFY(cell->state().focused);
    QCOMPARE(iface->focusChild(), cell);
    QVERIFY(!table->cellAt(Mon, 3)->state().focused);

    // Rows and columns
    s.click(s.rowHeader(Wed));
    QVERIFY(table->isRowSelected(Wed));
    QCOMPARE(table->selectedRows(), QList<int>({Wed}));
    QCOMPARE(table->selectedRowCount(), 1);
    QVERIFY(iface->child(BScheduleGridAccessible::childIndex(Wed, -1))->state().selected);
    QVERIFY(table->selectRow(Fri));
    QCOMPARE(table->selectedRows(), QList<int>({Wed, Fri}));
    QVERIFY(table->unselectRow(Wed));
    QCOMPARE(table->selectedRows(), QList<int>({Fri}));
    QVERIFY(table->selectColumn(3));
    QVERIFY(table->isColumnSelected(3));
    QCOMPARE(table->selectedColumnCount(), 1);

    // Actions: a header selects, a cell selects and lists its runs
    QAccessibleActionInterface *headerAction =
        iface->child(BScheduleGridAccessible::childIndex(-1, 21))->actionInterface();
    QVERIFY(headerAction);
    headerAction->doAction(QAccessibleActionInterface::pressAction());
    QCOMPARE(s.model()->selection().count(), 7);
    QVERIFY(s.model()->selection().isColumnSelected(21));
    QAccessibleActionInterface *cellAction = table->cellAt(Mon, 21)->actionInterface();
    QVERIFY(cellAction);
    QVERIFY(cellAction->actionNames().contains(QAccessibleActionInterface::showMenuAction()));
    cellAction->doAction(QAccessibleActionInterface::pressAction());
    QCOMPARE(s.model()->selection().count(), 1);
    QVERIFY(grid->currentCell() == (BGridPos{Mon, 21}));
    cellAction->doAction(QAccessibleActionInterface::showMenuAction());
    QVERIFY(grid->cellDetails());
    s.closeDetails();

    // The name follows the scope
    s.model()->setActiveScope(0);
    QCOMPARE(iface->text(QAccessible::Name),
             QStringLiteral("Schedule grid of MonthlyCycle, scope Every week"));
    QCOMPARE(table->cellAt(Mon, 21)->text(QAccessible::Name),
             QStringLiteral("Monday 21 hours: Incremental at 21:05"));

    // Calendar rules are not cells: they are read with the table
    Shown mixed(mixedSchedule());
    QVERIFY(mixed.ok);
    QAccessibleInterface *mixedIface = QAccessible::queryAccessibleInterface(mixed.grid());
    QVERIFY(mixedIface);
    const QString description = mixedIface->text(QAccessible::Description);
    QVERIFY2(description.contains(QStringLiteral("Calendar rule R1: Level=Full Pool=Monthly on 1 at 21:00, "
                                                 "on the 1st of every month at 21:00")),
             qPrintable(description));
}

void BScheduleGridWidgetTest::accessibleEvents()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    BScheduleGridWidget *grid = s.grid();
    s.view.activateWindow();
    grid->setFocus();
    QTRY_VERIFY(grid->hasFocus());
    s.click(s.cell(Wed, 5));

    // As if a screen reader were listening
    accessibleSource = grid;
    accessibleNotes.clear();
    QAccessible::installUpdateHandler(noteAccessibleEvent);

    // Moving the keyboard: the focus goes to the cell, named in full
    QTest::keyClick(grid, Qt::Key_Right);
    QVERIFY(noted(QAccessible::Focus, QStringLiteral("Wednesday 6 hours: no run")));
    QVERIFY(noted(QAccessible::SelectionWithin));
    accessibleNotes.clear();
    grid->setCurrentCell(BGridPos{Mon, 21});
    QVERIFY(noted(QAccessible::Focus, QStringLiteral("Monday 21 hours: Incremental at 21:05")));

    // Another scope: the table has other content
    accessibleNotes.clear();
    s.model()->setActiveScope(1);
    QVERIFY(noted(QAccessible::TableModelChanged));
    grid->setCurrentCell(BGridPos{Sat, 21});
    QVERIFY(noted(QAccessible::Focus,
                  QStringLiteral("Saturday 21 hours: Full at 21:00, pool Monthly, first week only; "
                                 "in other scopes: Differential at 21:00, only 2nd-5th")));

    // Selecting without moving
    accessibleNotes.clear();
    s.click(s.rowHeader(Tue));
    QVERIFY(noted(QAccessible::SelectionWithin));

    QAccessible::installUpdateHandler(nullptr);
    accessibleSource = nullptr;
    accessibleNotes.clear();
}

// --- Rendering -----------------------------------------------------------------------

void BScheduleGridWidgetTest::rendering_data()
{
    QTest::addColumn<QString>("theme");
    QTest::newRow("dark") << "dark";
    QTest::newRow("light") << "light";
}

void BScheduleGridWidgetTest::rendering()
{
    QFETCH(QString, theme);
    QVERIFY2(loadTheme(theme), "theme file not found");
    const QString shots = qEnvironmentVariable("ONESIMUS_GRID_SCREENSHOT_DIR");
    auto save = [&](QWidget *widget, const QString &name) {
        if (shots.isEmpty())
            return;
        QDir().mkpath(shots);
        const QString path = QStringLiteral("%1/%2-%3.png").arg(shots, name, theme);
        QVERIFY2(widget->grab().save(path), qPrintable(path));
    };
    auto paint = [](Shown &s) {
        QCoreApplication::processEvents();
        return s.viewport()->grab().toImage().convertToFormat(QImage::Format_RGB32);
    };
    auto inside = [](Shown &s, int row, int column) {
        return s.grid()->gridGeometry().cellRect(row, column).adjusted(2, 2, -3, -3);
    };

    // --- (a) The usual schedule ---
    {
        Shown s(typicalSchedule());
        QVERIFY(s.ok);
        const QColor base = s.grid()->palette().color(QPalette::Base);
        // The themes arrive in the palette the grid paints from
        QVERIFY(theme == QLatin1String("dark") ? base.lightness() < 80 : base.lightness() > 200);

        QImage image = paint(s);
        const QColor empty = meanColor(image, inside(s, Mon, 3));
        const QColor incremental = meanColor(image, inside(s, Mon, 21));
        const QColor ghosts = meanColor(image, inside(s, Sat, 21));
        // An empty cell is the base colour of the theme
        QVERIFY2(distance(empty, base) < 12, qPrintable(empty.name()));
        // A run of the scope is filled with its level colour
        const QColor fill = s.grid()->levelFill(std::optional<QString>(QStringLiteral("Incremental")));
        QVERIFY2(distance(incremental, empty) > 60, qPrintable(incremental.name()));
        QVERIFY2(distance(incremental, fill) < 60, qPrintable(incremental.name()));
        // Runs of other scopes are neither empty nor filled
        QVERIFY2(distance(ghosts, empty) > 15, qPrintable(ghosts.name()));
        QVERIFY2(distance(ghosts, incremental) > 30, qPrintable(ghosts.name()));
        // Working days look alike, the weekend does not
        QCOMPARE(meanColor(image, inside(s, Tue, 21)), incremental);
        QCOMPARE(meanColor(image, inside(s, Sun, 21)), empty);
        // The letter and the tick are in the cell: it is not one colour
        int inked = 0;
        const QRect cellRect = inside(s, Mon, 21);
        for (int y = cellRect.top(); y <= cellRect.bottom(); ++y) {
            for (int x = cellRect.left(); x <= cellRect.right(); ++x) {
                if (distance(image.pixelColor(x, y), fill) > 200)
                    ++inked;
            }
        }
        QVERIFY2(inked > 15, qPrintable(QString::number(inked)));
        save(&s.view, QStringLiteral("a-typical-every-week"));

        // Selection shows: the same empty cell, selected and not
        s.click(s.cell(Mon, 3));
        s.drag(s.cell(Mon, 20), s.cell(Fri, 22));
        image = paint(s);
        QVERIFY(distance(meanColor(image, inside(s, Tue, 20)), empty) > 15);
        QCOMPARE(meanColor(image, inside(s, Sun, 20)), empty);
        QVERIFY(distance(meanColor(image, inside(s, Mon, 21)), incremental) > 10);
        save(&s.view, QStringLiteral("a-typical-selection"));

        // The scope "1st": the Saturday is filled now, the working days are ghosts
        s.model()->setSelection(BGridSelection());
        s.view.selectRunLine(0);
        QCOMPARE(s.model()->activeScopeIndex(), 1);
        const QImage first = paint(s);
        const QColor full = s.grid()->levelFill(std::optional<QString>(QStringLiteral("Full")));
        // One stripe of two is the Full
        const QRect saturday = inside(s, Sat, 21);
        const QRect leftHalf(saturday.left(), saturday.bottom() - 8, saturday.width() / 2 - 4, 6);
        QVERIFY2(distance(meanColor(first, leftHalf), full) < 60,
                 qPrintable(meanColor(first, leftHalf).name()));
        QVERIFY(distance(meanColor(first, inside(s, Mon, 21)), incremental) > 30);
        QVERIFY(differingPixels(first, image, inside(s, Sat, 21)) > 50);
        save(&s.view, QStringLiteral("a-typical-first-week"));

        // The two kinds of ghosts differ: in "1st" the Differential never
        // coincides (right stripe of Saturday), the Incremental may (Monday)
        s.model()->setActiveScope(2);       // "2nd-5th"
        const QImage others = paint(s);
        QVERIFY(differingPixels(first, others, inside(s, Sat, 21)) > 50);
        save(&s.view, QStringLiteral("a-typical-other-weeks"));

        // The list of runs of the Saturday
        s.model()->setActiveScope(0);
        s.grid()->showCellDetails(Sat, 21);
        QVERIFY(s.grid()->cellDetails());
        save(s.grid()->cellDetails(), QStringLiteral("a-typical-runs-of-a-cell"));
        s.closeDetails();
    }

    // --- (b) An hourly schedule ---
    {
        Shown s(hourlySchedule());
        QVERIFY(s.ok);
        const QImage image = paint(s);
        const QColor base = s.grid()->palette().color(QPalette::Base);
        // Every cell of every day has runs
        for (int row = 0; row < 7; ++row) {
            for (int column = 0; column < 24; ++column) {
                QVERIFY2(distance(meanColor(image, inside(s, row, column)), base) > 60,
                         qPrintable(QStringLiteral("%1/%2").arg(row).arg(column)));
            }
        }
        // Sunday at three has a fourth run: it does not look like its neighbour
        const QImage three = image.copy(inside(s, Sun, 3));
        const QImage four = image.copy(inside(s, Sun, 4));
        QCOMPARE(three.size(), four.size());
        QVERIFY(three != four);
        QCOMPARE(image.copy(inside(s, Sat, 3)), image.copy(inside(s, Sat, 4)));
        save(&s.view, QStringLiteral("b-hourly"));
    }

    // --- (c) A calendar rule, a line not understood, a line without a time ---
    {
        Shown s(mixedSchedule());
        QVERIFY(s.ok);
        const BScheduleGridGeometry &geometry = s.grid()->gridGeometry();
        QVERIFY(geometry.hasRuleBand());
        QImage image = paint(s);
        // The band has a mark at the hours of the rules and nowhere else
        const QImage mark = image.copy(geometry.ruleCellRect(21).adjusted(3, 3, -3, -3));
        const QImage none = image.copy(geometry.ruleCellRect(5).adjusted(3, 3, -3, -3));
        const QImage noneToo = image.copy(geometry.ruleCellRect(6).adjusted(3, 3, -3, -3));
        QVERIFY(mark != none);
        QCOMPARE(none, noneToo);
        QVERIFY(image.copy(geometry.ruleCellRect(23).adjusted(3, 3, -3, -3)) != none);
        // The line without a time fills the Saturday, with its warning sign
        const QColor base = s.grid()->palette().color(QPalette::Base);
        for (int column = 0; column < 24; ++column)
            QVERIFY(distance(meanColor(image, inside(s, Sat, column)), base) > 40);
        save(&s.view, QStringLiteral("c-calendar-rule-and-problem"));

        // A chosen rule is outlined in the band
        s.view.selectRunLine(0);
        const QImage chosen = paint(s);
        QVERIFY(differingPixels(chosen, image, geometry.ruleCellRect(21)) > 5);
        QCOMPARE(differingPixels(chosen, image, geometry.ruleCellRect(23)), 0);
        save(&s.view, QStringLiteral("c-calendar-rule-chosen"));
    }

    // --- A window too small for the cells: they scroll under the headers ---
    {
        Shown s(typicalSchedule(), QSize(620, 560));
        QVERIFY(s.ok);
        const BScheduleGridGeometry &geometry = s.grid()->gridGeometry();
        QCOMPARE(geometry.cellWidth(), int(BScheduleGridGeometry::MinCellWidth));
        QVERIFY(geometry.scrollRange().width() > 0);
        // The keyboard brings the cell into view
        s.grid()->setCurrentCell(BGridPos{Mon, 21});
        QCoreApplication::processEvents();
        QVERIFY(geometry.scrollAreaRect().contains(geometry.cellRect(Mon, 21)));
        const QImage image = paint(s);
        const QColor fill = s.grid()->levelFill(std::optional<QString>(QStringLiteral("Incremental")));
        QVERIFY(distance(meanColor(image, inside(s, Mon, 21)), fill) < 60);
        // The row header is still at the left edge
        QCOMPARE(geometry.rowHeaderRect(Mon).left(), 0);
        save(&s.view, QStringLiteral("d-small-window"));
    }
}

void BScheduleGridWidgetTest::renderingOfTheLoadBar()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    auto paint = [&]() {
        QCoreApplication::processEvents();
        return s.viewport()->grab().toImage().convertToFormat(QImage::Format_RGB32);
    };
    const BScheduleGridGeometry &geometry = s.grid()->gridGeometry();

    // Nothing sets the load yet: nothing is drawn for it
    const QImage before = paint();
    QVERIFY(!s.model()->cell(Mon, 3).expectedConcurrentRuns.has_value());

    // Set by hand: Monday (weekday 1 of Bareos) at three and at 21 hours
    s.model()->setExpectedLoad(1, 3, 1.0);
    s.model()->setExpectedLoad(1, 21, 2.0);
    const QImage after = paint();

    // A bar at the bottom of these two cells ...
    const QRect emptyCell = geometry.cellRect(Mon, 3).adjusted(0, 0, -1, -1);
    const QRect bar(emptyCell.left(), emptyCell.bottom() - 2, emptyCell.width(), 3);
    const QRect above(emptyCell.left(), emptyCell.top(), emptyCell.width(), emptyCell.height() - 3);
    QVERIFY(differingPixels(before, after, bar) > emptyCell.width());
    QCOMPARE(differingPixels(before, after, above), 0);
    QVERIFY(differingPixels(before, after, geometry.cellRect(Mon, 21)) > 0);
    // ... and nowhere else
    QCOMPARE(differingPixels(before, after, geometry.cellRect(Tue, 3)), 0);
    QCOMPARE(differingPixels(before, after, geometry.cellRect(Tue, 21)), 0);

    // The bar is as long as the load is of the largest: half and full
    auto filled = [&](const QImage &image, int column) {
        const QRect cell = geometry.cellRect(Mon, column).adjusted(0, 0, -1, -1);
        const QColor track = image.pixelColor(cell.right() - 1, cell.bottom() - 1);
        const QColor start = image.pixelColor(cell.left() + 1, cell.bottom() - 1);
        int count = 0;
        for (int x = cell.left(); x <= cell.right(); ++x) {
            if (image.pixelColor(x, cell.bottom() - 1) == start)
                ++count;
        }
        return track == start ? cell.width() : count;
    };
    const int width = geometry.cellWidth() - 1;
    QVERIFY(qAbs(filled(after, 3) - width / 2) <= 2);
    QCOMPARE(filled(after, 21), width);

    // The tooltip names the figure
    QVERIFY(s.grid()->toolTipAt(s.cell(Mon, 3)).contains(QStringLiteral("Expected load: 1.0 runs")));

    // Cleared: as before
    s.model()->clearExpectedLoad();
    QCOMPARE(paint(), before);
}

QTEST_MAIN(BScheduleGridWidgetTest)
#include "bschedulegridwidget_test.moc"
