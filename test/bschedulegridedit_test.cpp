/**
 * @file bschedulegridedit_test.cpp
 * @brief Editing in the schedule grid
 *
 * What step "edit" adds to the grid view: the palette with the brush in
 * hand, painting and erasing with the keyboard and, in paint mode, with the
 * mouse, the level keys, undo and redo back to the Run lines exactly as
 * they were reported, the states of the "Run lines" list, adding and
 * removing scopes, the form for calendar rules, a schedule with a line that
 * is not understood, a refresh that brings other Run lines while there are
 * changes, whole days and hours from the headers, and what a screen reader
 * is told about edits.
 *
 * Nothing here talks to a Director; an edit ends at the Run lines of the
 * model. Needs a QApplication but no display: CTest runs it on the
 * offscreen platform. With ONESIMUS_GRID_SCREENSHOT_DIR set to a directory
 * the last test saves pictures of the editing states there (prefix "s6-").
 *
 * Rows are those of a view that starts the week on Monday.
 */

#include <QtTest>

#include <QAbstractButton>
#include <QAccessible>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QTreeWidget>
#include <QUndoStack>

#include "schedules/brunruledialog.h"
#include "schedules/bschedulegridaccessible.h"
#include "schedules/bschedulegriddialogs.h"
#include "schedules/bschedulegrideditor.h"
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

/// The usual three scopes, in spellings the writer would not choose: an
/// old-style level without "Level=", a time with "pm", two blanks
const QStringList &typicalRuns()
{
    static const QStringList runs{
        QStringLiteral("Level=Full Pool=Monthly 1st sat at 21:00"),
        QStringLiteral("differential 2nd-5th sat at 9:00pm"),
        QStringLiteral("Level=Incremental  mon-fri at 21:05"),
    };
    return runs;
}

BSchedule typicalSchedule()
{
    return scheduleOf(QStringLiteral("MonthlyCycle"), typicalRuns());
}

/// Two calendar rules, a line that is not understood, one without a time
const QStringList &mixedRuns()
{
    static const QStringList runs{
        QStringLiteral("Level=Full Pool=Monthly on 1 at 21:00"),
        QStringLiteral("Level=Incremental mon-fri at 21:00"),
        QStringLiteral("Level=Differential every other sat at 21:00"),
        QStringLiteral("Level=Full w01/w02 sun at 23:10"),
        QStringLiteral("sat"),
    };
    return runs;
}

BSchedule mixedSchedule()
{
    return scheduleOf(QStringLiteral("FirstOfMonth"), mixedRuns());
}

/// The lines as they would be written; not understood ones as reported
QStringList texts(const BScheduleGridModel *model)
{
    QStringList result;
    for (const BGridRunLineText &line : model->runLines()) {
        if (line.state != BGridRunLineText::Removed)
            result.append(line.text);
    }
    return result;
}

/// One letter per row of runLines(): untouched, changed, new, removed
QString states(const BScheduleGridModel *model)
{
    QString result;
    for (const BGridRunLineText &line : model->runLines()) {
        switch (line.state) {
        case BGridRunLineText::Untouched: result += QLatin1Char('u'); break;
        case BGridRunLineText::Changed: result += QLatin1Char('c'); break;
        case BGridRunLineText::New: result += QLatin1Char('n'); break;
        case BGridRunLineText::Removed: result += QLatin1Char('r'); break;
        }
    }
    return result;
}

/// The first two columns of the "Run lines" list, row by row
QStringList listRows(QTreeWidget *list)
{
    QStringList rows;
    for (int i = 0; i < list->topLevelItemCount(); ++i)
        rows.append(list->topLevelItem(i)->text(0) + QStringLiteral(" | ") + list->topLevelItem(i)->text(1));
    return rows;
}

BGridBrush brushOf(const char *level, int minute = 0, const char *pool = nullptr)
{
    BGridBrush brush;
    if (level)
        brush.overrides.level = QString::fromLatin1(level);
    if (pool)
        brush.overrides.pool = QString::fromLatin1(pool);
    brush.minute = minute;
    return brush;
}

/// A view in a window of its own, shown, laid out and with the keyboard in the grid
struct Shown
{
    explicit Shown(const BSchedule &schedule, const QSize &size = QSize(1280, 860))
    {
        view.setAttribute(Qt::WA_DontShowOnScreen, false);
        view.model()->setFirstDayOfWeek(Qt::Monday);
        view.setToday(QDate(2026, 11, 19));
        view.resize(size);
        view.setSchedule(schedule);
        view.setUsage({QStringLiteral("backup-client1"), QStringLiteral("backup-client2")}, true);
        view.show();
        ok = QTest::qWaitForWindowExposed(&view);
        view.activateWindow();
        view.grid()->setFocus();
        QCoreApplication::processEvents();
    }

    BScheduleGridWidget *grid() { return view.grid(); }
    BScheduleGridModel *model() { return view.model(); }
    BScheduleGridEditor *editor() { return view.editor(); }
    BSchedulePalette *palette() { return view.palette(); }
    QUndoStack *undo() { return view.editor()->undoStack(); }
    QWidget *viewport() { return view.grid()->viewport(); }
    QPoint cell(int row, int column) { return view.grid()->gridGeometry().cellRect(row, column).center(); }
    QPoint rowHeader(int row) { return view.grid()->gridGeometry().rowHeaderRect(row).center(); }
    QPoint columnHeader(int column) { return view.grid()->gridGeometry().columnHeaderRect(column).center(); }

    void click(const QPoint &pos, Qt::KeyboardModifiers modifiers = Qt::NoModifier)
    {
        QTest::mouseClick(viewport(), Qt::LeftButton, modifiers, pos);
    }

    void move(const QPoint &pos, Qt::KeyboardModifiers modifiers = Qt::NoModifier)
    {
        QMouseEvent event(QEvent::MouseMove, QPointF(pos), QPointF(viewport()->mapToGlobal(pos)),
                          Qt::NoButton, Qt::LeftButton, modifiers);
        QApplication::sendEvent(viewport(), &event);
    }

    /// Press, move with the button down, release
    void drag(const QPoint &from, const QPoint &to, Qt::KeyboardModifiers modifiers = Qt::NoModifier)
    {
        QTest::mousePress(viewport(), Qt::LeftButton, modifiers, from);
        const int steps = 4;
        for (int i = 1; i <= steps; ++i)
            move(from + (to - from) * i / steps, modifiers);
        QTest::mouseRelease(viewport(), Qt::LeftButton, modifiers, to);
    }

    void key(int key, Qt::KeyboardModifiers modifiers = Qt::NoModifier)
    {
        QTest::keyClick(view.grid(), Qt::Key(key), modifiers);
    }

    /// Selects a rectangle of cells with the mouse
    void select(int row1, int column1, int row2, int column2)
    {
        if (row1 == row2 && column1 == column2)
            click(cell(row1, column1));
        else
            drag(cell(row1, column1), cell(row2, column2));
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

/// What a widget told assistive technology
struct AccessibleNote
{
    QAccessible::Event type;
    QString name;       ///< Of the interface the event is about
    QString message;    ///< Of an announcement
};
QList<AccessibleNote> accessibleNotes;
QObject *accessibleSource = nullptr;

void noteAccessibleEvent(QAccessibleEvent *event)
{
    if (event->object() != accessibleSource)
        return;
    AccessibleNote note{event->type(), QString(), QString()};
    if (QAccessibleInterface *iface = event->accessibleInterface())
        note.name = iface->text(QAccessible::Name);
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    if (event->type() == QAccessible::Announcement)
        note.message = static_cast<QAccessibleAnnouncementEvent *>(event)->message();
#endif
    accessibleNotes.append(note);
}

bool noted(QAccessible::Event type, const QString &name = QString())
{
    for (const AccessibleNote &note : std::as_const(accessibleNotes)) {
        if (note.type == type && (name.isEmpty() || note.name == name))
            return true;
    }
    return false;
}

int notedCount(QAccessible::Event type)
{
    int count = 0;
    for (const AccessibleNote &note : std::as_const(accessibleNotes))
        count += note.type == type;
    return count;
}

} // namespace

class BScheduleGridEditTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanup();

    // --- The editor alone ---
    void editorPaintsErasesAndUndoes();
    void editorBrushesAndLevels();

    // --- The palette ---
    void paletteOffersTheBrushes();
    void paletteWithTheKeyboard();
    void newBrushForm();
    void emptyScheduleHasTheUsualLevels();

    // --- Painting and erasing ---
    void plainClickOnlySelects();
    void paintAndEraseWithTheKeyboard();
    void undoRedoBackToTheReportedBytes();
    void levelKeys();
    void paintModeWithTheMouse();
    void paintModeLeavesSelectingToModifiersAndHeaders();
    void keepOtherRuns();
    void paintingOverOtherScopes();
    void gridWithoutEditorIsReadOnly();

    // --- Run lines, scopes, calendar rules ---
    void runLinesListShowsTheStates();
    void headersSelectWholeDaysAndHours();
    void addScope();
    void removeScope();
    void scopeForm();
    void contextMenus();
    void calendarRuleFormShowsARule();
    void calendarRuleChanged();
    void calendarRuleAddedAndRemoved();
    void calendarRuleFormRefusesWhatIsNotARule();
    void calendarRuleFormKeyboard();

    // --- What must not be lost ---
    void scheduleWithALineNotUnderstood();
    void refreshWhileModified();
    void anotherScheduleWhileModified();

    // --- Screen readers and the keyboard round ---
    void accessibleEdits();
    void accessibleTool();
    void f6ReachesThePalette();

    // --- Pictures ---
    void screenshots_data();
    void screenshots();
};

void BScheduleGridEditTest::initTestCase()
{
    // The lines the tests rely on are understood as they stand
    for (const QString &run : typicalRuns()) {
        const BRunLine line = BRunParser::parse(run);
        QVERIFY2(line.understood(), qPrintable(run + QStringLiteral(": ") + line.parseError));
        QVERIFY2(!line.needsRewrite, qPrintable(run));
    }
}

void BScheduleGridEditTest::cleanup()
{
    qApp->setStyleSheet(QString());
    QAccessible::installUpdateHandler(nullptr);
    accessibleSource = nullptr;
    accessibleNotes.clear();
}

// --- The editor alone -------------------------------------------------------------

void BScheduleGridEditTest::editorPaintsErasesAndUndoes()
{
    BScheduleGridModel model;
    model.setFirstDayOfWeek(Qt::Monday);
    BScheduleGridEditor editor(&model);
    model.setSchedule(typicalSchedule());
    QSignalSpy edited(&editor, &BScheduleGridEditor::edited);
    QSignalSpy failed(&editor, &BScheduleGridEditor::editFailed);

    // The brush in hand is the first of the schedule
    QCOMPARE(editor.tool(), BScheduleGridEditor::Brush);
    QCOMPARE(editor.toolText(), QStringLiteral("Full at :00, pool Monthly"));

    // Nothing selected: nothing to apply to
    QVERIFY(!editor.applyToolToSelection());
    QCOMPARE(failed.count(), 1);
    QCOMPARE(editor.lastError(), QStringLiteral("No cell is selected."));
    QVERIFY(!editor.eraseSelection());
    QCOMPARE(editor.undoStack()->count(), 0);

    // One step per edit
    QVERIFY(editor.paint(BGridCellSet::rect(Mon, 3, Fri, 3), brushOf("Incremental", 5)));
    QCOMPARE(edited.count(), 1);
    QCOMPARE(edited.last().first().toString(), QStringLiteral("Paint Incremental at :05 on 5 cell(s)"));
    QCOMPARE(editor.undoStack()->count(), 1);
    QCOMPARE(editor.undoStack()->undoText(), QStringLiteral("Paint Incremental at :05 on 5 cell(s)"));
    QCOMPARE(states(&model), QStringLiteral("uuun"));
    QVERIFY(editor.lastError().isEmpty());

    // What is there already is no step
    QVERIFY(editor.paint(BGridCellSet::rect(Mon, 3, Wed, 3), brushOf("Incremental", 5)));
    QCOMPARE(editor.undoStack()->count(), 1);

    QVERIFY(editor.erase(BGridCellSet::rect(Wed, 0, Wed, 23)));
    QCOMPARE(editor.undoStack()->count(), 2);
    QCOMPARE(editor.undoStack()->undoText(), QStringLiteral("Erase 24 cell(s)"));
    QVERIFY(model.cell(Wed, 21).inScope.isEmpty());

    // Undo, redo, and everything back at once
    editor.undo();
    QCOMPARE(model.cell(Wed, 21).inScope.size(), 1);
    QCOMPARE(edited.last().first().toString(), QStringLiteral("Undone: Erase 24 cell(s)"));
    editor.redo();
    QVERIFY(model.cell(Wed, 21).inScope.isEmpty());
    QCOMPARE(edited.last().first().toString(), QStringLiteral("Redone: Erase 24 cell(s)"));
    editor.revertAll();
    QVERIFY(!model.isModified());
    QCOMPARE(texts(&model), typicalRuns());
    QVERIFY(editor.undoStack()->canRedo());

    // The step names the scope it was made in, and undo shows that scope
    model.setActiveScope(1);
    QVERIFY(editor.paint(BGridCellSet::rect(Sun, 2, Sun, 2), brushOf("Full")));
    QCOMPARE(editor.undoStack()->undoText(),
             QStringLiteral("Paint Full at :00 on 1 cell(s) of the scope 1st"));
    QCOMPARE(texts(&model).last(), QStringLiteral("Level=Full 1st sun at 02:00"));
    model.setActiveScope(0);
    editor.undo();
    QCOMPARE(model.activeScopeIndex(), 1);
    QVERIFY(!model.isModified());

    // A new schedule starts with an empty stack
    model.setSchedule(mixedSchedule());
    QCOMPARE(editor.undoStack()->count(), 0);
    QCOMPARE(editor.toolText(), QStringLiteral("Incremental at :00"));
}

void BScheduleGridEditTest::editorBrushesAndLevels()
{
    BScheduleGridModel model;
    model.setFirstDayOfWeek(Qt::Monday);
    BScheduleGridEditor editor(&model);
    model.setSchedule(typicalSchedule());
    QSignalSpy toolChanged(&editor, &BScheduleGridEditor::toolChanged);
    QSignalSpy brushesChanged(&editor, &BScheduleGridEditor::brushesChanged);

    QCOMPARE(editor.brushes(), model.brushes());
    QCOMPARE(editor.brushes().size(), 3);
    QCOMPARE(editor.activeBrushIndex(), 0);

    editor.setActiveBrush(2);
    QCOMPARE(toolChanged.count(), 1);
    QCOMPARE(editor.activeBrush()->minute, 5);
    editor.setActiveBrush(2);
    QCOMPARE(toolChanged.count(), 1);

    // The eraser; the brush that was in hand is remembered
    editor.setEraser();
    QCOMPARE(editor.tool(), BScheduleGridEditor::Eraser);
    QVERIFY(!editor.activeBrush());
    QCOMPARE(editor.activeBrushIndex(), 2);
    QCOMPARE(editor.toolText(), QStringLiteral("Eraser"));

    // A new brush is offered at the end and keeps its place
    const BGridBrush tape = brushOf("VirtualFull", 30, "Tape");
    editor.setActiveBrush(tape);
    QCOMPARE(editor.brushes().size(), 4);
    QCOMPARE(editor.activeBrushIndex(), 3);
    QCOMPARE(brushesChanged.count(), 1);
    QCOMPARE(editor.addBrush(tape), 3);
    QCOMPARE(brushesChanged.count(), 1);

    // The level keys: the level alone, at the minute of the brush in hand
    QCOMPARE(BScheduleGridEditor::levelOfKey(Qt::Key_F), QStringLiteral("Full"));
    QCOMPARE(BScheduleGridEditor::levelOfKey(Qt::Key_D), QStringLiteral("Differential"));
    QCOMPARE(BScheduleGridEditor::levelOfKey(Qt::Key_I), QStringLiteral("Incremental"));
    QCOMPARE(BScheduleGridEditor::levelOfKey(Qt::Key_V), QStringLiteral("VirtualFull"));
    QVERIFY(BScheduleGridEditor::levelOfKey(Qt::Key_X).isEmpty());
    QCOMPARE(editor.levelBrush(QStringLiteral("Full")), brushOf("Full", 30));
    editor.setActiveBrush(1);
    QCOMPARE(editor.levelBrush(QStringLiteral("Differential")), editor.brushes()[1]);

    // A brush whose last cell is erased stays on offer
    BGridSelection all;
    all.selectAll();
    model.setSelection(all);
    QVERIFY(editor.eraseSelection());
    QCOMPARE(model.brushes().size(), 2);    // the other scopes
    QCOMPARE(editor.brushes().size(), 4);
    QCOMPARE(editor.brushes()[2], brushOf("Incremental", 5));
}

// --- The palette --------------------------------------------------------------------

void BScheduleGridEditTest::paletteOffersTheBrushes()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    BSchedulePalette *palette = s.palette();

    QList<QAbstractButton *> buttons = palette->brushButtons();
    QCOMPARE(buttons.size(), 3);
    QCOMPARE(buttons[0]->text(), QStringLiteral("A = Full at :00, pool Monthly"));
    QCOMPARE(buttons[1]->text(), QStringLiteral("Differential at :00"));
    QCOMPARE(buttons[2]->text(), QStringLiteral("Incremental at :05"));
    for (QAbstractButton *button : buttons) {
        QVERIFY(!button->icon().isNull());
        QVERIFY(button->isVisibleTo(&s.view));
    }

    // The brush in hand is checked and named in words
    QVERIFY(buttons[0]->isChecked());
    QCOMPARE(palette->activeButton(), buttons[0]);
    QVERIFY2(palette->activeText().startsWith(
                 QStringLiteral("In hand: the brush A = Full at :00, pool Monthly.")),
             qPrintable(palette->activeText()));

    // A click takes another brush in hand
    QTest::mouseClick(buttons[2], Qt::LeftButton);
    QCOMPARE(s.editor()->activeBrushIndex(), 2);
    QVERIFY(buttons[2]->isChecked());
    QVERIFY(!buttons[0]->isChecked());
    QVERIFY(palette->activeText().startsWith(QStringLiteral("In hand: the brush Incremental at :05.")));

    // The eraser
    QTest::mouseClick(palette->eraserButton(), Qt::LeftButton);
    QCOMPARE(s.editor()->tool(), BScheduleGridEditor::Eraser);
    QVERIFY(palette->eraserButton()->isChecked());
    QVERIFY(!buttons[2]->isChecked());
    QVERIFY(palette->activeText().startsWith(QStringLiteral("In hand: the eraser.")));

    // The palette follows the editor
    s.editor()->setActiveBrush(1);
    QVERIFY(palette->brushButtons()[1]->isChecked());
    QVERIFY(!palette->eraserButton()->isChecked());

    // The switches
    QVERIFY(!palette->paintModeButton()->isChecked());
    QTest::mouseClick(palette->paintModeButton(), Qt::LeftButton);
    QVERIFY(s.editor()->paintMode());
    QVERIFY(palette->activeText().contains(QStringLiteral("Paint mode")));
    s.editor()->setPaintMode(false);
    QVERIFY(!palette->paintModeButton()->isChecked());
    QTest::mouseClick(palette->keepRunsButton(), Qt::LeftButton);
    QVERIFY(s.editor()->keepOtherRuns());
    QVERIFY(palette->activeText().contains(QStringLiteral("Other runs of a painted cell stay.")));

    // A brush that comes up by painting gets a button; the tags stay
    QVERIFY(s.editor()->paint(BGridCellSet::rect(Sun, 4, Sun, 4), brushOf("Full", 15, "Yearly")));
    buttons = palette->brushButtons();
    QCOMPARE(buttons.size(), 4);
    QCOMPARE(buttons[0]->text(), QStringLiteral("A = Full at :00, pool Monthly"));
    QCOMPARE(buttons[3]->text(), QStringLiteral("B = Full at :15, pool Yearly"));
    QCOMPARE(s.grid()->brushTag(brushOf("Full", 15, "Yearly")), QStringLiteral("B"));
    QVERIFY(buttons[1]->isChecked());
}

void BScheduleGridEditTest::paletteWithTheKeyboard()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    BSchedulePalette *palette = s.palette();
    const QList<QAbstractButton *> buttons = palette->brushButtons();

    // Every button takes the keyboard, and Space presses it
    for (QAbstractButton *button : buttons)
        QCOMPARE(button->focusPolicy(), Qt::StrongFocus);
    QCOMPARE(palette->eraserButton()->focusPolicy(), Qt::StrongFocus);
    QCOMPARE(palette->newBrushButton()->focusPolicy(), Qt::StrongFocus);
    QCOMPARE(palette->paintModeButton()->focusPolicy(), Qt::StrongFocus);

    buttons[1]->setFocus();
    QTRY_VERIFY(buttons[1]->hasFocus());
    QTest::keyClick(buttons[1], Qt::Key_Space);
    QCOMPARE(s.editor()->activeBrushIndex(), 1);

    palette->eraserButton()->setFocus();
    QTest::keyClick(palette->eraserButton(), Qt::Key_Space);
    QCOMPARE(s.editor()->tool(), BScheduleGridEditor::Eraser);

    palette->paintModeButton()->setFocus();
    QTest::keyClick(palette->paintModeButton(), Qt::Key_Space);
    QVERIFY(s.editor()->paintMode());
    QTest::keyClick(palette->paintModeButton(), Qt::Key_Space);
    QVERIFY(!s.editor()->paintMode());

    // In the grid, P switches paint mode
    s.grid()->setFocus();
    s.key(Qt::Key_P);
    QVERIFY(s.editor()->paintMode());
    QVERIFY(palette->paintModeButton()->isChecked());
    s.key(Qt::Key_P);
    QVERIFY(!s.editor()->paintMode());
}

void BScheduleGridEditTest::newBrushForm()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    BSchedulePalette *palette = s.palette();
    s.editor()->setActiveBrush(2);

    // The form starts from the brush in hand
    QTest::mouseClick(palette->newBrushButton(), Qt::LeftButton);
    BScheduleBrushDialog *dialog = palette->openBrushDialog();
    QVERIFY(dialog);
    QVERIFY(dialog->isVisible());
    QCOMPARE(dialog->brush(), brushOf("Incremental", 5));
    QVERIFY(dialog->isValid());

    // Cancelled: nothing changes
    dialog->reject();
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    QCOMPARE(s.editor()->brushes().size(), 3);
    QCOMPARE(s.editor()->activeBrushIndex(), 2);

    // Level, minute and overrides
    dialog = palette->openBrushDialog();
    QVERIFY(dialog);
    BRunOverridesEditor *overrides = dialog->overridesEditor();
    overrides->levelCombo()->setCurrentIndex(overrides->levelCombo()->findData(QStringLiteral("VirtualFull")));
    dialog->minuteBox()->setValue(30);
    overrides->poolEdit()->setText(QStringLiteral("Long Term"));
    overrides->storageEdit()->setText(QStringLiteral("Tape"));
    BGridBrush wanted = brushOf("VirtualFull", 30, "Long Term");
    wanted.overrides.storage = QStringLiteral("Tape");
    QCOMPARE(dialog->brush(), wanted);
    QString error;
    QVERIFY2(dialog->isValid(&error), qPrintable(error));
    QVERIFY(dialog->buttons()->button(QDialogButtonBox::Ok)->isEnabled());
    dialog->accept();

    // Offered, in hand, with the next tag
    QCOMPARE(s.editor()->brushes().size(), 4);
    QCOMPARE(s.editor()->activeBrush().value(), wanted);
    const QList<QAbstractButton *> buttons = palette->brushButtons();
    QCOMPARE(buttons.size(), 4);
    QCOMPARE(buttons[3]->text(), QStringLiteral("B = VirtualFull at :30, pool Long Term, storage Tape"));
    QVERIFY(buttons[3]->isChecked());
    // Not a change of the schedule yet
    QVERIFY(!s.view.isModified());
    QCOMPARE(s.undo()->count(), 0);

    // Painted, it is written with its overrides in front, the name quoted
    s.select(Sun, 2, Sun, 2);
    s.key(Qt::Key_Space);
    QCOMPARE(texts(s.model()).last(),
             QStringLiteral("Level=VirtualFull Pool=\"Long Term\" Storage=Tape sun at 02:30"));

    // All the overrides the model has go through the form
    BRunOverrides all;
    all.level = QStringLiteral("Full");
    all.pool = QStringLiteral("P");
    all.fullPool = QStringLiteral("FP");
    all.incrementalPool = QStringLiteral("IP");
    all.differentialPool = QStringLiteral("DP");
    all.nextPool = QStringLiteral("NP");
    all.storage = QStringLiteral("S");
    all.messages = QStringLiteral("M");
    all.maxRunSchedTime = QStringLiteral("8 hours");
    all.priority = 7;
    all.spoolData = true;
    all.accurate = false;
    BRunOverridesEditor form;
    form.setOverrides(all);
    QVERIFY(form.overrides() == all);
    form.setOverrides(BRunOverrides());
    QVERIFY(form.overrides().isEmpty());
}

void BScheduleGridEditTest::emptyScheduleHasTheUsualLevels()
{
    Shown s(scheduleOf(QStringLiteral("New"), {}));
    QVERIFY(s.ok);
    const QList<QAbstractButton *> buttons = s.palette()->brushButtons();
    QCOMPARE(buttons.size(), 3);
    QCOMPARE(buttons[0]->text(), QStringLiteral("Full at :00"));
    QCOMPARE(buttons[1]->text(), QStringLiteral("Differential at :00"));
    QCOMPARE(buttons[2]->text(), QStringLiteral("Incremental at :00"));
    QVERIFY(buttons[0]->isChecked());

    // A schedule is painted from nothing
    s.select(Sat, 21, Sat, 21);
    s.key(Qt::Key_Space);
    s.editor()->setActiveBrush(2);
    s.select(Mon, 21, Fri, 21);
    s.key(Qt::Key_Space);
    QCOMPARE(texts(s.model()), QStringList({QStringLiteral("Level=Full sat at 21:00"),
                                            QStringLiteral("Level=Incremental mon-fri at 21:00")}));
    QCOMPARE(states(s.model()), QStringLiteral("nn"));
    QStringList written;
    QVERIFY(s.model()->runTexts(&written));
    QCOMPARE(written, texts(s.model()));
}

// --- Painting and erasing -----------------------------------------------------------

void BScheduleGridEditTest::plainClickOnlySelects()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);

    // A cell with several runs: a click selects and opens nothing
    s.click(s.cell(Sat, 21));
    QVERIFY(s.model()->selection().contains(Sat, 21));
    QCOMPARE(s.model()->selection().count(), 1);
    QVERIFY(!s.grid()->cellDetails());
    QVERIFY(!s.view.isModified());

    // Enter and a double click list the runs
    s.key(Qt::Key_Return);
    QVERIFY(s.grid()->cellDetails());
    s.grid()->cellDetails()->close();
    QCoreApplication::processEvents();
    QTest::mouseDClick(s.viewport(), Qt::LeftButton, Qt::NoModifier, s.cell(Sat, 21));
    QVERIFY(s.grid()->cellDetails());
    s.grid()->cellDetails()->close();
    QVERIFY(!s.view.isModified());
}

void BScheduleGridEditTest::paintAndEraseWithTheKeyboard()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    QVERIFY(!s.view.isModified());
    QVERIFY(s.view.modifiedText().isEmpty());
    QSignalSpy modified(&s.view, &BScheduleGridView::modifiedChanged);

    // Select with the keyboard: Monday to Friday at three
    s.click(s.cell(Mon, 3));
    for (int i = 0; i < 4; ++i)
        s.key(Qt::Key_Down, Qt::ShiftModifier);
    QCOMPARE(s.model()->selection().count(), 5);

    // Space applies the brush in hand
    s.key(Qt::Key_Space);
    QCOMPARE(texts(s.model()), QStringList({typicalRuns()[0], typicalRuns()[1], typicalRuns()[2],
                                            QStringLiteral("Level=Full Pool=Monthly mon-fri at 03:00")}));
    QCOMPARE(states(s.model()), QStringLiteral("uuun"));
    QCOMPARE(s.model()->cell(Wed, 3).inScope.size(), 1);
    QVERIFY(s.view.isModified());
    QCOMPARE(modified.count(), 1);
    QCOMPARE(modified.last().first().toBool(), true);
    QCOMPARE(s.view.modifiedText(),
             QStringLiteral("Modified: 1 new Run line(s), not written to the Director."));
    QCOMPARE(s.undo()->count(), 1);
    // The selection is still there for the next key
    QCOMPARE(s.model()->selection().count(), 5);

    // Space again changes nothing and leaves no step
    s.key(Qt::Key_Space);
    QCOMPARE(s.undo()->count(), 1);

    // Del erases a part: the line shrinks
    s.click(s.cell(Wed, 3));
    s.key(Qt::Key_Delete);
    QCOMPARE(texts(s.model()).last(), QStringLiteral("Level=Full Pool=Monthly mon,tue,thu,fri at 03:00"));
    QVERIFY(s.model()->cell(Wed, 3).inScope.isEmpty());
    QCOMPARE(s.undo()->count(), 2);

    // Backspace does the same; with all of it erased the schedule is as loaded
    s.click(s.rowHeader(Mon));
    s.click(s.rowHeader(Fri), Qt::ShiftModifier);
    s.click(s.columnHeader(3));
    s.key(Qt::Key_Backspace);
    QCOMPARE(texts(s.model()), typicalRuns());
    QCOMPARE(states(s.model()), QStringLiteral("uuu"));
    QVERIFY(!s.view.isModified());
    QVERIFY(s.view.modifiedText().isEmpty());
    QCOMPARE(modified.last().first().toBool(), false);

    // The eraser in hand: Space erases
    s.editor()->setEraser();
    s.click(s.cell(Wed, 21));
    s.key(Qt::Key_Space);
    QVERIFY(s.model()->cell(Wed, 21).inScope.isEmpty());
    QCOMPARE(texts(s.model())[2], QStringLiteral("Level=Incremental mon,tue,thu,fri at 21:05"));
    QCOMPARE(states(s.model()), QStringLiteral("uuc"));
    QCOMPARE(s.view.modifiedText(),
             QStringLiteral("Modified: 1 changed Run line(s), not written to the Director."));

    // Nothing selected: the view says why nothing happened
    s.model()->setSelection(BGridSelection());
    QVERIFY(s.view.editErrorText().isEmpty());
    s.key(Qt::Key_Space);
    QVERIFY2(s.view.editErrorText().contains(QStringLiteral("No cell is selected.")),
             qPrintable(s.view.editErrorText()));
    // ... and the next edit that works takes the remark away
    s.click(s.cell(Thu, 21));
    s.key(Qt::Key_Delete);
    QVERIFY(s.view.editErrorText().isEmpty());
}

void BScheduleGridEditTest::undoRedoBackToTheReportedBytes()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    const QStringList loaded = typicalRuns();

    // 1: a hole in the working days; 2: a new line; 3: the Saturdays of the
    // other weeks go
    s.click(s.cell(Wed, 21));
    s.key(Qt::Key_Delete);
    s.select(Mon, 3, Fri, 3);
    s.key(Qt::Key_Space);
    s.model()->setActiveScope(2);
    s.click(s.cell(Sat, 21));
    s.key(Qt::Key_Delete);
    QCOMPARE(s.undo()->count(), 3);
    const QStringList edited = texts(s.model());
    QCOMPARE(edited, QStringList({loaded[0],
                                  QStringLiteral("Level=Incremental mon,tue,thu,fri at 21:05"),
                                  QStringLiteral("Level=Full Pool=Monthly mon-fri at 03:00")}));
    QCOMPARE(states(s.model()), QStringLiteral("urcn"));
    // The line nothing touched is the reported text throughout
    QCOMPARE(s.model()->runLines()[0].text, loaded[0]);

    // Ctrl+Z, step by step
    s.model()->setActiveScope(0);
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(states(s.model()), QStringLiteral("uucn"));
    QCOMPARE(texts(s.model())[1], loaded[1]);       // "differential ... 9:00pm" as reported
    // Undo shows the scope the step was made in
    QCOMPARE(BScheduleGridText::scopeLabel(s.model()->activeScope()), QStringLiteral("2nd-5th"));
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(states(s.model()), QStringLiteral("uuc"));
    QCOMPARE(BScheduleGridText::scopeLabel(s.model()->activeScope()), QStringLiteral("Every week"));
    s.key(Qt::Key_Z, Qt::ControlModifier);

    // Everything undone: the Run lines byte for byte as they were reported
    QCOMPARE(texts(s.model()), loaded);
    QCOMPARE(states(s.model()), QStringLiteral("uuu"));
    QVERIFY(!s.view.isModified());
    QStringList written;
    QVERIFY(s.model()->runTexts(&written));
    QCOMPARE(written, loaded);
    for (int i = 0; i < loaded.size(); ++i) {
        QCOMPARE(s.model()->schedule().runs[i].original, loaded[i]);
        QVERIFY(!s.model()->schedule().runs[i].dirty);
    }
    // One more Ctrl+Z does nothing
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(texts(s.model()), loaded);

    // Redo: Ctrl+Shift+Z and Ctrl+Y
    s.key(Qt::Key_Z, Qt::ControlModifier | Qt::ShiftModifier);
    QCOMPARE(states(s.model()), QStringLiteral("uuc"));
    s.key(Qt::Key_Y, Qt::ControlModifier);
    QCOMPARE(states(s.model()), QStringLiteral("uucn"));
    s.key(Qt::Key_Y, Qt::ControlModifier);
    QCOMPARE(texts(s.model()), edited);
    QCOMPARE(states(s.model()), QStringLiteral("urcn"));
    QVERIFY(!s.undo()->canRedo());

    // The keys also work from the lists beside the grid
    s.view.runLineList()->setFocus();
    QTRY_VERIFY(s.view.runLineList()->hasFocus());
    QTest::keyClick(s.view.runLineList(), Qt::Key_Z, Qt::ControlModifier);
    QTRY_COMPARE(states(s.model()), QStringLiteral("uucn"));

    // The buttons do the same and say what they will do
    QVERIFY(s.view.undoButton()->isEnabled());
    QVERIFY2(s.view.undoButton()->toolTip().startsWith(QStringLiteral("Undo: Paint Full at :00, pool Monthly on 5 cell(s)")),
             qPrintable(s.view.undoButton()->toolTip()));
    QVERIFY(s.view.redoButton()->toolTip().startsWith(QStringLiteral("Redo: Erase 1 cell(s) of the scope 2nd-5th")));
    QTest::mouseClick(s.view.redoButton(), Qt::LeftButton);
    QCOMPARE(states(s.model()), QStringLiteral("urcn"));
    QVERIFY(!s.view.redoButton()->isEnabled());
    QTest::mouseClick(s.view.undoButton(), Qt::LeftButton);
    QCOMPARE(states(s.model()), QStringLiteral("uucn"));

    // "Revert all" undoes every step; they can be redone
    QTest::mouseClick(s.view.revertButton(), Qt::LeftButton);
    QCOMPARE(texts(s.model()), loaded);
    QVERIFY(!s.view.isModified());
    QVERIFY(!s.view.undoButton()->isEnabled());
    QVERIFY(!s.view.revertButton()->isEnabled());
    QVERIFY(s.view.redoButton()->isEnabled());

    // A hole that is painted shut again with the brush of the line is the
    // reported line again, not a rewritten one
    s.model()->setActiveScope(0);
    s.click(s.cell(Wed, 21));
    s.key(Qt::Key_Delete);
    QCOMPARE(states(s.model()), QStringLiteral("uuc"));
    s.editor()->setActiveBrush(2);
    s.key(Qt::Key_Space);
    QCOMPARE(texts(s.model()), loaded);
    QCOMPARE(states(s.model()), QStringLiteral("uuu"));
    QVERIFY(!s.view.isModified());
}

void BScheduleGridEditTest::levelKeys()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    s.click(s.cell(Sun, 4));

    // D: the Differential brush of the schedule, taken in hand and painted
    s.key(Qt::Key_D);
    QCOMPARE(s.editor()->activeBrushIndex(), 1);
    QCOMPARE(texts(s.model()).last(), QStringLiteral("Level=Differential sun at 04:00"));
    QCOMPARE(s.editor()->brushes().size(), 3);

    // I: there is no Incremental at :00 yet; it is offered from now on
    s.key(Qt::Key_I);
    QCOMPARE(s.editor()->brushes().size(), 4);
    QCOMPARE(s.editor()->activeBrush().value(), brushOf("Incremental", 0));
    QCOMPARE(texts(s.model()).last(), QStringLiteral("Level=Incremental sun at 04:00"));
    QCOMPARE(s.model()->cell(Sun, 4).inScope.size(), 1);
    QCOMPARE(s.palette()->brushButtons().size(), 4);
    QVERIFY(s.palette()->brushButtons()[3]->isChecked());

    s.key(Qt::Key_V);
    QCOMPARE(texts(s.model()).last(), QStringLiteral("Level=VirtualFull sun at 04:00"));
    // F: the level alone, not the Full of the schedule with its pool
    s.key(Qt::Key_F);
    QCOMPARE(texts(s.model()).last(), QStringLiteral("Level=Full sun at 04:00"));
    QCOMPARE(s.editor()->activeBrush().value(), brushOf("Full", 0));
    QCOMPARE(s.undo()->count(), 4);

    // The minute is that of the brush in hand
    s.editor()->setActiveBrush(2);      // Incremental at :05
    s.click(s.cell(Sun, 6));
    s.key(Qt::Key_F);
    QCOMPARE(texts(s.model()).last(), QStringLiteral("Level=Full sun at 06:05"));

    // Without a selection the key only takes the brush in hand
    s.model()->setSelection(BGridSelection());
    const QStringList before = texts(s.model());
    const int steps = s.undo()->count();
    s.key(Qt::Key_D);
    QCOMPARE(s.editor()->activeBrush().value(), brushOf("Differential", 5));
    QCOMPARE(texts(s.model()), before);
    QCOMPARE(s.undo()->count(), steps);
    QVERIFY(s.view.editErrorText().isEmpty());

    // With Ctrl the letters are not level keys
    s.click(s.cell(Mon, 10));
    s.key(Qt::Key_D, Qt::ControlModifier);
    s.key(Qt::Key_F, Qt::ControlModifier);
    QVERIFY(s.model()->cell(Mon, 10).inScope.isEmpty());
}

void BScheduleGridEditTest::paintModeWithTheMouse()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    s.click(s.cell(Tue, 1));
    const BGridSelection selection = s.model()->selection();
    QTest::mouseClick(s.palette()->brushButtons()[2], Qt::LeftButton);     // Incremental at :05
    QTest::mouseClick(s.palette()->paintModeButton(), Qt::LeftButton);
    QVERIFY(s.editor()->paintMode());
    QCOMPARE(s.viewport()->cursor().shape(), Qt::CrossCursor);

    // A stroke: nothing changes until the button is released
    QTest::mousePress(s.viewport(), Qt::LeftButton, Qt::NoModifier, s.cell(Sat, 8));
    QCOMPARE(s.grid()->pendingStroke().count(), 1);
    s.move(s.cell(Sat, 9));
    // The mouse is faster than its events: the cells between are painted too
    s.move(s.cell(Sat, 11));
    QCOMPARE(s.grid()->pendingStroke(), BGridCellSet::rect(Sat, 8, Sat, 11));
    QCOMPARE(texts(s.model()), typicalRuns());
    QCOMPARE(s.undo()->count(), 0);
    QTest::mouseRelease(s.viewport(), Qt::LeftButton, Qt::NoModifier, s.cell(Sat, 11));
    QVERIFY(s.grid()->pendingStroke().isEmpty());

    // One line, one step
    QCOMPARE(texts(s.model()).last(),
             QStringLiteral("Level=Incremental sat at 08:05 at 09:05 at 10:05 at 11:05"));
    QCOMPARE(s.undo()->count(), 1);
    QCOMPARE(s.undo()->undoText(), QStringLiteral("Paint Incremental at :05 on 4 cell(s)"));
    // Painting is not selecting
    QCOMPARE(s.model()->selection(), selection);

    // A press without dragging paints one cell
    s.click(s.cell(Sun, 8));
    QCOMPARE(s.model()->cell(Sun, 8).inScope.size(), 1);
    QCOMPARE(s.undo()->count(), 2);

    // A stroke round a corner is not a rectangle: it becomes the lines it needs
    s.drag(s.cell(Mon, 12), s.cell(Mon, 14));
    QTest::mousePress(s.viewport(), Qt::LeftButton, Qt::NoModifier, s.cell(Tue, 14));
    s.move(s.cell(Wed, 14));
    QTest::mouseRelease(s.viewport(), Qt::LeftButton, Qt::NoModifier, s.cell(Wed, 14));
    for (const BGridPos &pos : {BGridPos{Mon, 12}, BGridPos{Mon, 13}, BGridPos{Mon, 14},
                                BGridPos{Tue, 14}, BGridPos{Wed, 14}}) {
        QCOMPARE(s.model()->cell(pos.row, pos.column).inScope.size(), 1);
    }
    QVERIFY(s.model()->cell(Tue, 13).inScope.isEmpty());

    // Esc gives a stroke up
    const QStringList before = texts(s.model());
    const int steps = s.undo()->count();
    QTest::mousePress(s.viewport(), Qt::LeftButton, Qt::NoModifier, s.cell(Thu, 2));
    s.move(s.cell(Thu, 5));
    QCOMPARE(s.grid()->pendingStroke().count(), 4);
    s.key(Qt::Key_Escape);
    QVERIFY(s.grid()->pendingStroke().isEmpty());
    QTest::mouseRelease(s.viewport(), Qt::LeftButton, Qt::NoModifier, s.cell(Thu, 5));
    QCOMPARE(texts(s.model()), before);
    QCOMPARE(s.undo()->count(), steps);

    // The eraser in paint mode
    QTest::mouseClick(s.palette()->eraserButton(), Qt::LeftButton);
    s.drag(s.cell(Sat, 8), s.cell(Sat, 9));
    QVERIFY(s.model()->cell(Sat, 8).inScope.isEmpty());
    QVERIFY(s.model()->cell(Sat, 9).inScope.isEmpty());
    QCOMPARE(s.model()->cell(Sat, 10).inScope.size(), 1);
    QCOMPARE(s.undo()->undoText(), QStringLiteral("Erase 2 cell(s)"));

    // One undo takes back one stroke
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(s.model()->cell(Sat, 8).inScope.size(), 1);

    // Paint mode off: the mouse selects again and changes nothing
    QTest::mouseClick(s.palette()->paintModeButton(), Qt::LeftButton);
    QVERIFY(!s.editor()->paintMode());
    QVERIFY(s.viewport()->cursor().shape() != Qt::CrossCursor);
    const QStringList after = texts(s.model());
    s.drag(s.cell(Mon, 0), s.cell(Tue, 1));
    QCOMPARE(s.model()->selection().count(), 4);
    QCOMPARE(texts(s.model()), after);
}

void BScheduleGridEditTest::paintModeLeavesSelectingToModifiersAndHeaders()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    s.editor()->setPaintMode(true);

    // With Ctrl and with Shift the mouse selects as it always does
    s.click(s.cell(Mon, 2), Qt::ControlModifier);
    s.click(s.cell(Wed, 4), Qt::ShiftModifier);
    QCOMPARE(s.model()->selection().count(), 9);
    QVERIFY(!s.view.isModified());
    s.drag(s.cell(Sun, 0), s.cell(Sun, 1), Qt::ControlModifier);
    QCOMPARE(s.model()->selection().count(), 11);
    QVERIFY(!s.view.isModified());

    // The headers select whole days and hours, also in paint mode
    s.click(s.rowHeader(Thu));
    QCOMPARE(s.model()->selection().count(), 24);
    QVERIFY(s.model()->selection().isRowSelected(Thu));
    s.click(s.columnHeader(7));
    QVERIFY(s.model()->selection().isColumnSelected(7));
    s.click(s.grid()->gridGeometry().cornerRect().center());
    QCOMPARE(s.model()->selection().count(), 7 * 24);
    QVERIFY(!s.view.isModified());
    QCOMPARE(s.undo()->count(), 0);

    // The band of calendar rules is never painted
    Shown mixed(mixedSchedule());
    QVERIFY(mixed.ok);
    mixed.editor()->setPaintMode(true);
    mixed.click(mixed.grid()->gridGeometry().ruleCellRect(21).center());
    QVERIFY(!mixed.view.isModified());
    QCOMPARE(mixed.grid()->highlightedRule(), 0);
}

void BScheduleGridEditTest::keepOtherRuns()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    s.click(s.cell(Mon, 21));
    QCOMPARE(s.model()->cell(Mon, 21).inScope.size(), 1);

    // Shift+Space adds the brush beside the run that is there
    s.editor()->setActiveBrush(1);      // Differential at :00
    s.key(Qt::Key_Space, Qt::ShiftModifier);
    QCOMPARE(s.model()->cell(Mon, 21).inScope.size(), 2);
    QCOMPARE(texts(s.model()), QStringList({typicalRuns()[0], typicalRuns()[1], typicalRuns()[2],
                                            QStringLiteral("Level=Differential mon at 21:00")}));
    QCOMPARE(s.undo()->undoText(), QStringLiteral("Add Differential at :00 to 1 cell(s)"));

    // Plain Space leaves the brush alone in the cell, as the Logon Hours dialog does
    s.key(Qt::Key_Space);
    QCOMPARE(s.model()->cell(Mon, 21).inScope.size(), 1);
    QCOMPARE(texts(s.model())[2], QStringLiteral("Level=Incremental tue-fri at 21:05"));

    // With the switch on, plain Space adds
    s.editor()->setKeepOtherRuns(true);
    s.editor()->setActiveBrush(2);      // Incremental at :05
    s.key(Qt::Key_Space);
    QCOMPARE(s.model()->cell(Mon, 21).inScope.size(), 2);
    QCOMPARE(s.model()->cell(Mon, 21).inScope[0].minute, 0);
    QCOMPARE(s.model()->cell(Mon, 21).inScope[1].minute, 5);
}

void BScheduleGridEditTest::paintingOverOtherScopes()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);

    // In "Every week" the Saturday evening has only runs of other scopes
    QVERIFY(s.model()->cell(Sat, 21).inScope.isEmpty());
    QCOMPARE(s.model()->cell(Sat, 21).outOfScope.size(), 2);
    s.click(s.cell(Sat, 21));

    // The tooltip and the selection line say that they stay
    const QString tip = s.grid()->toolTipAt(s.cell(Sat, 21));
    QVERIFY2(tip.contains(QStringLiteral("Painting or erasing here changes only the scope "
                                         "&quot;Every week&quot;; the runs of other scopes stay.")),
             qPrintable(tip));
    QVERIFY2(s.view.selectionText().endsWith(
                 QStringLiteral("They stay when this scope is painted or erased.")),
             qPrintable(s.view.selectionText()));
    // A cell without runs of other scopes says nothing of the kind
    QVERIFY(!s.grid()->toolTipAt(s.cell(Mon, 21)).contains(QStringLiteral("other scopes stay")));

    // Painting is allowed, and they do stay
    s.editor()->setActiveBrush(2);
    s.key(Qt::Key_Space);
    QCOMPARE(texts(s.model()), QStringList({typicalRuns()[0], typicalRuns()[1], typicalRuns()[2],
                                            QStringLiteral("Level=Incremental sat at 21:05")}));
    QCOMPARE(s.model()->cell(Sat, 21).inScope.size(), 1);
    QCOMPARE(s.model()->cell(Sat, 21).outOfScope.size(), 2);

    // Erasing takes only the run of this scope
    s.key(Qt::Key_Delete);
    QCOMPARE(texts(s.model()), typicalRuns());
    // Erasing a cell that only has runs of other scopes changes nothing
    const int steps = s.undo()->count();
    s.key(Qt::Key_Delete);
    QCOMPARE(texts(s.model()), typicalRuns());
    QCOMPARE(s.undo()->count(), steps);
}

void BScheduleGridEditTest::gridWithoutEditorIsReadOnly()
{
    BScheduleGridModel model;
    model.setFirstDayOfWeek(Qt::Monday);
    model.setSchedule(typicalSchedule());
    BScheduleGridWidget grid;
    grid.setModel(&model);
    grid.resize(1100, 400);
    grid.show();
    QVERIFY(QTest::qWaitForWindowExposed(&grid));
    QVERIFY(!grid.isEditable());

    BGridSelection all;
    all.selectAll();
    model.setSelection(all);
    for (int key : {Qt::Key_Space, Qt::Key_Delete, Qt::Key_Backspace, Qt::Key_F, Qt::Key_D,
                    Qt::Key_I, Qt::Key_V, Qt::Key_P}) {
        QTest::keyClick(&grid, Qt::Key(key));
    }
    QVERIFY(!model.isModified());
    QCOMPARE(texts(&model), typicalRuns());

    // A screen reader hears a table that cannot be changed
    QAccessibleInterface *iface = QAccessible::queryAccessibleInterface(&grid);
    QVERIFY(iface);
    QAccessibleInterface *cell = iface->tableInterface()->cellAt(Mon, 21);
    QVERIFY(cell->state().readOnly);
    QVERIFY(!cell->actionInterface()->actionNames().contains(QStringLiteral("Erase")));
}

// --- Run lines, scopes, calendar rules ------------------------------------------------

void BScheduleGridEditTest::runLinesListShowsTheStates()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    QTreeWidget *list = s.view.runLineList();
    const QStringList loadedRows{QStringLiteral("1 | ") + typicalRuns()[0],
                                 QStringLiteral("2 | ") + typicalRuns()[1],
                                 QStringLiteral("3 | ") + typicalRuns()[2]};
    QCOMPARE(listRows(list), loadedRows);

    // A changed line, a new line and a removed one
    s.click(s.cell(Wed, 21));
    s.key(Qt::Key_Delete);
    s.select(Mon, 3, Fri, 3);
    s.key(Qt::Key_Space);
    s.model()->setActiveScope(2);
    s.click(s.cell(Sat, 21));
    s.key(Qt::Key_Delete);

    const QStringList editedRows{
        QStringLiteral("1 | ") + typicalRuns()[0],
        QStringLiteral("removed | ") + typicalRuns()[1],
        QStringLiteral("2 changed | Level=Incremental mon,tue,thu,fri at 21:05"),
        QStringLiteral("3 new | Level=Full Pool=Monthly mon-fri at 03:00")};
    QCOMPARE(listRows(list), editedRows);
    // The removed line is struck through, the others are not
    QVERIFY(list->topLevelItem(1)->font(1).strikeOut());
    QVERIFY(!list->topLevelItem(0)->font(1).strikeOut());
    QVERIFY(!list->topLevelItem(2)->font(1).strikeOut());
    // Where the lines are shown
    QCOMPARE(list->topLevelItem(0)->text(2), QStringLiteral("scope 1st"));
    QCOMPARE(list->topLevelItem(1)->text(2), QString());
    QCOMPARE(list->topLevelItem(3)->text(2), QStringLiteral("scope Every week"));
    QCOMPARE(s.view.modifiedText(),
             QStringLiteral("Modified: 1 changed, 1 new, 1 removed Run line(s), not written to the Director."));

    // The scope bar counts with it; the emptied scope stays, dimmed
    const QList<QAbstractButton *> scopes = s.view.scopeButtons();
    QCOMPARE(scopes.size(), 3);
    QCOMPARE(scopes[0]->text(), QStringLiteral("Every week (2)"));
    QCOMPARE(scopes[2]->text(), QStringLiteral("2nd-5th (0)"));
    QVERIFY(scopes[2]->property("scopeEmpty").toBool());

    // Choosing the new line outlines its cells
    list->setCurrentItem(list->topLevelItem(3));
    QCOMPARE(s.grid()->highlightedRun(), 2);
    QCOMPARE(s.model()->activeScopeIndex(), 0);

    // Undo, one step: the removed line is back as it was reported
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(listRows(list), QStringList({loadedRows[0], loadedRows[1],
        QStringLiteral("3 changed | Level=Incremental mon,tue,thu,fri at 21:05"),
        QStringLiteral("4 new | Level=Full Pool=Monthly mon-fri at 03:00")}));
    QVERIFY(!list->topLevelItem(1)->font(1).strikeOut());
    // An outline of before the step is gone: the numbers mean other lines now
    QCOMPARE(s.grid()->highlightedRun(), -1);

    // All undone: the list as it was loaded
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(listRows(list), QStringList({loadedRows[0], loadedRows[1],
        QStringLiteral("3 changed | Level=Incremental mon,tue,thu,fri at 21:05")}));
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(listRows(list), loadedRows);
    QVERIFY(s.view.modifiedText().isEmpty());
    QCOMPARE(s.view.scopeButtons()[2]->text(), QStringLiteral("2nd-5th (1)"));

    // All redone: the same states again
    for (int i = 0; i < 3; ++i)
        s.key(Qt::Key_Y, Qt::ControlModifier);
    QCOMPARE(listRows(list), editedRows);
    QVERIFY(list->topLevelItem(1)->font(1).strikeOut());

    // The calendar follows: painted Monday at three, there is a Full on Mondays now
    bool mondayFull = false;
    const QList<BScheduleCalendarDay> days = BScheduleCalendar::days(s.model()->schedule(),
                                                                     QDate(2026, 11, 23), 1);
    for (const BScheduleCalendarRun &run : days.first().runs)
        mondayFull = mondayFull || (run.level && *run.level == QLatin1String("Full"));
    QVERIFY(mondayFull);
}

void BScheduleGridEditTest::headersSelectWholeDaysAndHours()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    const BGridSelection &selection = s.model()->selection();

    // The anchor was set by a cell: Shift on a day header still takes whole days
    s.click(s.cell(Tue, 5));
    s.click(s.rowHeader(Thu), Qt::ShiftModifier);
    QCOMPARE(selection.count(), 3 * 24);
    QVERIFY(selection.isRowSelected(Tue) && selection.isRowSelected(Wed) && selection.isRowSelected(Thu));
    // ... and again, to another day
    s.click(s.rowHeader(Mon), Qt::ShiftModifier);
    QCOMPARE(selection.count(), 2 * 24);
    QVERIFY(selection.isRowSelected(Mon) && selection.isRowSelected(Tue));

    // The same for hours
    s.click(s.cell(Wed, 8));
    s.click(s.columnHeader(11), Qt::ShiftModifier);
    QCOMPARE(selection.count(), 4 * 7);
    for (int column = 8; column <= 11; ++column)
        QVERIFY(selection.isColumnSelected(column));

    // Ctrl+Shift adds whole days to what is selected
    s.click(s.cell(Sun, 0));
    s.click(s.cell(Fri, 3), Qt::ControlModifier);
    s.click(s.rowHeader(Sat), Qt::ControlModifier | Qt::ShiftModifier);
    QVERIFY(selection.isRowSelected(Fri) && selection.isRowSelected(Sat));
    QVERIFY(selection.contains(Sun, 0));
    QCOMPARE(selection.count(), 2 * 24 + 1);

    // What a header selects is what the keys edit: a whole day is "hourly"
    s.click(s.rowHeader(Sun));
    s.editor()->setActiveBrush(2);
    s.key(Qt::Key_Space);
    QCOMPARE(texts(s.model()).last(), QStringLiteral("Level=Incremental sun hourly at 0:05"));
}

void BScheduleGridEditTest::addScope()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    s.select(Mon, 3, Tue, 4);
    const BGridSelection selection = s.model()->selection();

    // "+" at the end of the scope bar opens the form
    QAbstractButton *plus = s.view.addScopeButton();
    QVERIFY(plus->isVisibleTo(&s.view));
    QCOMPARE(plus->text(), QStringLiteral("+"));
    QCOMPARE(plus->accessibleName(), QStringLiteral("Add scope"));
    QCOMPARE(plus->focusPolicy(), Qt::StrongFocus);
    QVERIFY(plus->x() > s.view.scopeButtons().last()->x());
    plus->setFocus();
    QTest::keyClick(plus, Qt::Key_Space);       // the keyboard does what a click does
    BScheduleScopeDialog *dialog = s.view.openScopeDialog();
    QVERIFY(dialog);
    QVERIFY(dialog->isVisible());

    // The third week
    dialog->weekBoxes()[0]->setChecked(false);
    dialog->weekBoxes()[2]->setChecked(true);
    QCOMPARE(dialog->noteText(), QStringLiteral("New scope: 3rd"));
    dialog->accept();

    // Added, shown, not a change of the schedule, one undo step
    // (in the order of the bar: by first week, so behind "2nd-5th")
    QList<QAbstractButton *> buttons = s.view.scopeButtons();
    QCOMPARE(buttons.size(), 4);
    QCOMPARE(buttons[3]->text(), QStringLiteral("3rd (0)"));
    QVERIFY(buttons[3]->isChecked());
    QVERIFY(buttons[3]->property("scopeEmpty").toBool());
    QCOMPARE(BScheduleGridText::scopeLabel(s.model()->activeScope()), QStringLiteral("3rd"));
    QVERIFY(!s.view.isModified());
    QCOMPARE(s.undo()->count(), 1);
    QCOMPARE(s.undo()->undoText(), QStringLiteral("Add the scope 3rd"));

    // The selection survives the switch, and paints into the new scope
    QCOMPARE(s.model()->selection(), selection);
    s.editor()->setActiveBrush(1);
    s.grid()->setFocus();
    s.key(Qt::Key_Space);
    QCOMPARE(texts(s.model()).last(), QStringLiteral("Level=Differential 3rd mon,tue at 03:00 at 04:00"));
    QCOMPARE(s.view.scopeButtons()[3]->text(), QStringLiteral("3rd (1)"));
    // Every other line is as it was reported
    QCOMPARE(texts(s.model()).mid(0, 3), typicalRuns());

    // A switch by the bar keeps the selection too
    QTest::mouseClick(s.view.scopeButtons()[0], Qt::LeftButton);
    QCOMPARE(s.model()->activeScopeIndex(), 0);
    QCOMPARE(s.model()->selection(), selection);

    // Undo: first the paint, shown in its scope, then the scope itself
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(BScheduleGridText::scopeLabel(s.model()->activeScope()), QStringLiteral("3rd"));
    QCOMPARE(texts(s.model()), typicalRuns());
    QCOMPARE(s.view.scopeButtons().size(), 4);
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(s.view.scopeButtons().size(), 3);
    QCOMPARE(BScheduleGridText::scopeLabel(s.model()->activeScope()), QStringLiteral("Every week"));
    QVERIFY(s.view.scopeButtons()[0]->isChecked());

    // Redo both: the scope and its line are back
    s.key(Qt::Key_Y, Qt::ControlModifier);
    s.key(Qt::Key_Y, Qt::ControlModifier);
    QCOMPARE(s.view.scopeButtons().size(), 4);
    QCOMPARE(s.view.scopeButtons()[3]->text(), QStringLiteral("3rd (1)"));
    QCOMPARE(texts(s.model()).last(), QStringLiteral("Level=Differential 3rd mon,tue at 03:00 at 04:00"));

    // A scope that exists is shown, not added again
    const int steps = s.undo()->count();
    dialog = s.view.openScopeDialog();
    QVERIFY(dialog);
    dialog->weekBoxes()[0]->setChecked(true);       // "1st" alone
    QCOMPARE(dialog->noteText(), QStringLiteral("The scope \"1st\" exists already; it will be shown."));
    dialog->accept();
    QCOMPARE(s.view.scopeButtons().size(), 4);
    QCOMPARE(BScheduleGridText::scopeLabel(s.model()->activeScope()), QStringLiteral("1st"));
    QCOMPARE(s.undo()->count(), steps);
}

void BScheduleGridEditTest::removeScope()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);

    // A scope with Run lines is not removed, and the view says why
    QVERIFY(!s.view.removeScope(1));
    QVERIFY2(s.view.editErrorText().contains(QStringLiteral("The scope 1st still has Run lines.")),
             qPrintable(s.view.editErrorText()));
    QCOMPARE(s.view.scopeButtons().size(), 3);
    // "Every week" is always there
    QVERIFY(!s.view.removeScope(0));
    QCOMPARE(s.undo()->count(), 0);

    // Emptied, it can go
    s.model()->setActiveScope(1);
    s.click(s.cell(Sat, 21));
    s.key(Qt::Key_Delete);
    QCOMPARE(s.view.scopeButtons()[1]->text(), QStringLiteral("1st (0)"));
    QVERIFY(s.view.editErrorText().isEmpty());

    // Del on the button of the scope removes it
    QAbstractButton *first = s.view.scopeButtons()[1];
    first->setFocus();
    QTRY_VERIFY(first->hasFocus());
    QTest::keyClick(first, Qt::Key_Delete);
    QCOMPARE(s.view.scopeButtons().size(), 2);
    QCOMPARE(s.view.scopeButtons()[1]->text(), QStringLiteral("2nd-5th (1)"));
    // It was on display: "Every week" is now
    QCOMPARE(s.model()->activeScopeIndex(), 0);
    QCOMPARE(s.undo()->undoText(), QStringLiteral("Remove the scope 1st"));
    QCOMPARE(states(s.model()), QStringLiteral("ruu"));

    // Del on a scope with lines only explains
    QAbstractButton *others = s.view.scopeButtons()[1];
    others->setFocus();
    QTest::keyClick(others, Qt::Key_Delete);
    QCOMPARE(s.view.scopeButtons().size(), 2);
    QVERIFY(s.view.editErrorText().contains(QStringLiteral("still has Run lines")));

    // Undo brings the scope back, shown as it was, then its line
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(s.view.scopeButtons().size(), 3);
    QCOMPARE(s.view.scopeButtons()[1]->text(), QStringLiteral("1st (0)"));
    QCOMPARE(BScheduleGridText::scopeLabel(s.model()->activeScope()), QStringLiteral("1st"));
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(texts(s.model()), typicalRuns());
    QCOMPARE(states(s.model()), QStringLiteral("uuu"));
    QCOMPARE(s.view.scopeButtons()[1]->text(), QStringLiteral("1st (1)"));

    // Redo removes both again
    s.key(Qt::Key_Y, Qt::ControlModifier);
    s.key(Qt::Key_Y, Qt::ControlModifier);
    QCOMPARE(s.view.scopeButtons().size(), 2);
    QCOMPARE(states(s.model()), QStringLiteral("ruu"));
}

void BScheduleGridEditTest::scopeForm()
{
    BScheduleGridModel model;
    model.setSchedule(typicalSchedule());
    BScheduleScopeDialog dialog(model.scopes());
    QPushButton *ok = dialog.buttons()->button(QDialogButtonBox::Ok);

    // It starts with the first week of every month, which exists here
    BGridScope first;
    first.weeksOfMonth = 1;
    QCOMPARE(dialog.scope(), first);
    QVERIFY(ok->isEnabled());
    QVERIFY(dialog.noteText().contains(QStringLiteral("exists already")));

    // No week and not "last": nothing to add
    dialog.weekBoxes()[0]->setChecked(false);
    QVERIFY(!ok->isEnabled());
    QCOMPARE(dialog.noteText(), QStringLiteral("Choose at least one week of the month, or \"last\"."));
    dialog.lastBox()->setChecked(true);
    QVERIFY(ok->isEnabled());
    QCOMPARE(dialog.noteText(), QStringLiteral("New scope: last"));
    QVERIFY(dialog.scope().last);
    QCOMPARE(int(dialog.scope().weeksOfMonth), 0);

    // Months
    for (int month = 3; month < 12; ++month)
        dialog.monthBoxes()[month]->setChecked(false);
    dialog.lastBox()->setChecked(false);
    dialog.weekBoxes()[0]->setChecked(true);
    dialog.weekBoxes()[1]->setChecked(true);
    QCOMPARE(dialog.noteText(), QStringLiteral("New scope: jan-mar 1st,2nd"));
    QCOMPARE(int(dialog.scope().months), 0x7);
    for (int month = 0; month < 3; ++month)
        dialog.monthBoxes()[month]->setChecked(false);
    QVERIFY(!ok->isEnabled());
    QCOMPARE(dialog.noteText(), QStringLiteral("Choose at least one month."));

    // A scope is put in as it is
    BGridScope given;
    given.weeksOfMonth = 0x1e;
    given.months = 0x800;
    dialog.setScope(given);
    QCOMPARE(dialog.scope(), given);
    QVERIFY(ok->isEnabled());
}

void BScheduleGridEditTest::contextMenus()
{
    // Chooses the entry of that text from the menu that opens next
    QStringList offered;
    auto choose = [&offered](const QString &text) {
        QTimer::singleShot(50, qApp, [&offered, text]() {
            auto *menu = qobject_cast<QMenu *>(QApplication::activePopupWidget());
            if (!menu)
                return;
            offered.clear();
            QAction *wanted = nullptr;
            for (QAction *action : menu->actions()) {
                if (action->isSeparator())
                    continue;
                offered.append(action->text() + (action->isEnabled() ? QString() : QStringLiteral(" (off)")));
                if (action->text() == text && action->isEnabled())
                    wanted = action;
            }
            if (wanted) {
                menu->setActiveAction(wanted);
                QTest::keyClick(menu, Qt::Key_Return);
            } else {
                menu->close();
            }
        });
    };

    Shown s(typicalSchedule());
    QVERIFY(s.ok);

    // A scope with Run lines: "Remove" is there, but off
    choose(QStringLiteral("Remove the scope \"1st\""));
    emit s.view.scopeButtons()[1]->customContextMenuRequested(QPoint(4, 4));
    QCOMPARE(offered, QStringList({QStringLiteral("Remove the scope \"1st\" (off)"),
                                   QStringLiteral("Add scope...")}));
    QCOMPARE(s.view.scopeButtons().size(), 3);

    // Emptied, the menu of its button removes it
    s.model()->setActiveScope(1);
    s.click(s.cell(Sat, 21));
    s.key(Qt::Key_Delete);
    choose(QStringLiteral("Remove the scope \"1st\""));
    emit s.view.scopeButtons()[1]->customContextMenuRequested(QPoint(4, 4));
    QCOMPARE(offered.first(), QStringLiteral("Remove the scope \"1st\""));
    QCOMPARE(s.view.scopeButtons().size(), 2);
    QCOMPARE(s.undo()->undoText(), QStringLiteral("Remove the scope 1st"));

    // "Add scope..." from the same menu opens the form
    choose(QStringLiteral("Add scope..."));
    emit s.view.scopeButtons()[0]->customContextMenuRequested(QPoint(4, 4));
    QCOMPARE(offered.first(), QStringLiteral("Remove the scope \"Every week\" (off)"));
    BScheduleScopeDialog *scope = s.view.window()->findChild<BScheduleScopeDialog *>();
    QVERIFY(scope);
    QVERIFY(scope->isVisible());
    scope->reject();

    // The list of calendar rules: edit, remove, add
    Shown mixed(mixedSchedule());
    QVERIFY(mixed.ok);
    QTreeWidget *rules = mixed.view.ruleList();
    rules->setCurrentItem(rules->topLevelItem(1));
    choose(QStringLiteral("Remove calendar rule"));
    emit rules->customContextMenuRequested(QPoint(10, 10));
    QCOMPARE(offered, QStringList({QStringLiteral("Edit calendar rule..."),
                                   QStringLiteral("Remove calendar rule"),
                                   QStringLiteral("Add calendar rule...")}));
    QCOMPARE(states(mixed.model()), QStringLiteral("uuuru"));
    QCOMPARE(rules->topLevelItemCount(), 1);

    // Nothing chosen: only "Add" is on
    choose(QStringLiteral("Add calendar rule..."));
    emit rules->customContextMenuRequested(QPoint(10, 10));
    QCOMPARE(offered, QStringList({QStringLiteral("Edit calendar rule... (off)"),
                                   QStringLiteral("Remove calendar rule (off)"),
                                   QStringLiteral("Add calendar rule...")}));
    BRunRuleDialog *rule = mixed.view.window()->findChild<BRunRuleDialog *>();
    QVERIFY(rule);
    QCOMPARE(rule->windowTitle(), QStringLiteral("New calendar rule"));
    rule->reject();
}

void BScheduleGridEditTest::calendarRuleFormShowsARule()
{
    BRunRuleDialog dialog;
    QPushButton *ok = dialog.buttons()->button(QDialogButtonBox::Ok);

    // A new rule: on the 1st at 21:00, and that can be accepted
    QVERIFY(dialog.isValid());
    QVERIFY(ok->isEnabled());
    QCOMPARE(dialog.runText(), QStringLiteral("Run = on 1 at 21:00"));
    QCOMPARE(dialog.readingText(), QStringLiteral("level of the job on the 1st of every month at 21:00"));

    // Every part of a Run line has its place in the form, and what goes in comes out
    const QStringList lines{
        QStringLiteral("Level=Full Pool=Monthly on 1 at 21:00"),
        QStringLiteral("Level=Full w01/w02 sun at 23:10"),
        QStringLiteral("Level=Differential Storage=Tape Priority=5 jan,jul 2nd-5th last mon-fri on 2-31 at 03:30 at 15:30"),
        QStringLiteral("Level=Incremental SpoolData=yes on 1/2 hourly at 0:15"),
        QStringLiteral("w05,w07 on 15 at 06:00"),
    };
    for (const QString &text : lines) {
        const BRunLine line = BRunParser::parse(text);
        QVERIFY2(line.understood(), qPrintable(text + QStringLiteral(": ") + line.parseError));
        dialog.setSpec(*line.spec);
        QVERIFY2(dialog.spec() == *line.spec, qPrintable(text));
        QVERIFY2(dialog.isValid(), qPrintable(text));
        QCOMPARE(dialog.runText(), QStringLiteral("Run = ") + text);
    }

    // The first of them, part by part
    dialog.setSpec(*BRunParser::parse(lines[0]).spec);
    QCOMPARE(dialog.overridesEditor()->levelCombo()->currentText(), QStringLiteral("Full"));
    QCOMPARE(dialog.overridesEditor()->poolEdit()->text(), QStringLiteral("Monthly"));
    QVERIFY(dialog.daysBox()->isChecked());
    QVERIFY(!dialog.weeksBox()->isChecked());
    QCOMPARE(dialog.dayButtons().size(), 31);
    QCOMPARE(dialog.weekOfYearButtons().size(), 54);
    QCOMPARE(dialog.hourButtons().size(), 24);
    for (int day = 0; day < 31; ++day)
        QCOMPARE(dialog.dayButtons()[day]->isChecked(), day == 0);
    for (int hour = 0; hour < 24; ++hour)
        QCOMPARE(dialog.hourButtons()[hour]->isChecked(), hour == 21);
    QVERIFY(!dialog.everyHourBox()->isChecked());
    QCOMPARE(dialog.minuteBox()->value(), 0);
    for (QCheckBox *box : dialog.weekdayBoxes() + dialog.weekOfMonthBoxes() + dialog.monthBoxes())
        QVERIFY(box->isChecked());
    QVERIFY(!dialog.lastBox()->isChecked());
    QCOMPARE(dialog.readingText(), QStringLiteral("Full on the 1st of every month at 21:00"));
    QCOMPARE(dialog.dayButtons()[0]->accessibleName(), QStringLiteral("Day 1"));
    QCOMPARE(dialog.weekOfYearButtons()[3]->accessibleName(), QStringLiteral("Week 3"));
    QCOMPARE(dialog.hourButtons()[21]->accessibleName(), QStringLiteral("21 hours"));

    // The second: every other week of the year, Sundays
    dialog.setSpec(*BRunParser::parse(lines[1]).spec);
    QVERIFY(!dialog.daysBox()->isChecked());
    QVERIFY(dialog.weeksBox()->isChecked());
    for (int week = 0; week < 54; ++week)
        QCOMPARE(dialog.weekOfYearButtons()[week]->isChecked(), week % 2 == 1);
    for (int weekday = 0; weekday < 7; ++weekday)
        QCOMPARE(dialog.weekdayBoxes()[weekday]->isChecked(), weekday == 0);   // Sunday is 0 for Bareos
    QCOMPARE(dialog.minuteBox()->value(), 10);

    // "hourly": every hour, the single hours are not asked
    dialog.setSpec(*BRunParser::parse(lines[3]).spec);
    QVERIFY(dialog.everyHourBox()->isChecked());
    QVERIFY(!dialog.hourButtons()[5]->isEnabled());
    dialog.everyHourBox()->setChecked(false);
    QVERIFY(dialog.hourButtons()[5]->isEnabled());
    // No hour chosen: that cannot be written
    QString error;
    QVERIFY(!dialog.isValid(&error));
    QVERIFY(!error.isEmpty());
    QVERIFY(!ok->isEnabled());
    dialog.hourButtons()[5]->setChecked(true);
    QVERIFY(ok->isEnabled());
    QCOMPARE(dialog.runText(), QStringLiteral("Run = Level=Incremental SpoolData=yes on 1/2 at 05:15"));
}

void BScheduleGridEditTest::calendarRuleChanged()
{
    Shown s(mixedSchedule());
    QVERIFY(s.ok);
    QTreeWidget *rules = s.view.ruleList();
    QTreeWidget *lines = s.view.runLineList();

    // Nothing chosen: only "Add" is offered
    QVERIFY(s.view.addRuleButton()->isEnabled());
    QVERIFY(!s.view.editRuleButton()->isEnabled());
    QVERIFY(!s.view.removeRuleButton()->isEnabled());

    // A click on the mark in the band chooses the rule ...
    s.click(s.grid()->gridGeometry().ruleCellRect(21).center());
    QCOMPARE(s.view.currentRule(), 0);
    QCOMPARE(rules->currentItem(), rules->topLevelItem(0));
    QVERIFY(s.view.editRuleButton()->isEnabled());
    QVERIFY(s.view.removeRuleButton()->isEnabled());
    QVERIFY(!s.view.findChild<BRunRuleDialog *>());

    // ... a double click opens its form
    QTest::mouseDClick(s.viewport(), Qt::LeftButton, Qt::NoModifier,
                       s.grid()->gridGeometry().ruleCellRect(21).center());
    BRunRuleDialog *dialog = s.view.openRuleDialog(0);     // the one that is open
    QVERIFY(dialog);
    QVERIFY(dialog->isVisible());
    QCOMPARE(dialog->windowTitle(), QStringLiteral("Calendar rule R1"));
    QCOMPARE(dialog->runText(), QStringLiteral("Run = ") + mixedRuns()[0]);

    // Cancelled: nothing changes
    dialog->reject();
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    QVERIFY(!s.view.isModified());
    QCOMPARE(s.undo()->count(), 0);

    // Enter in the list of rules opens it as well
    rules->setFocus();
    rules->setCurrentItem(rules->topLevelItem(0));
    QTest::keyClick(rules, Qt::Key_Return);
    dialog = s.view.findChild<BRunRuleDialog *>();
    if (!dialog)
        dialog = s.view.window()->findChild<BRunRuleDialog *>();
    QVERIFY(dialog);
    QCOMPARE(dialog->windowTitle(), QStringLiteral("Calendar rule R1"));

    // Also on the 15th, a quarter past
    dialog->dayButtons()[14]->setChecked(true);
    dialog->minuteBox()->setValue(15);
    QCOMPARE(dialog->runText(), QStringLiteral("Run = Level=Full Pool=Monthly on 1,15 at 21:15"));
    dialog->accept();

    // One line changed, every other one as it was reported
    QStringList expected = mixedRuns();
    expected[0] = QStringLiteral("Level=Full Pool=Monthly on 1,15 at 21:15");
    QCOMPARE(texts(s.model()), expected);
    QCOMPARE(states(s.model()), QStringLiteral("cuuuu"));
    QCOMPARE(s.undo()->count(), 1);
    QCOMPARE(s.undo()->undoText(), QStringLiteral("Change the calendar rule R1"));
    QCOMPARE(rules->topLevelItem(0)->text(1),
             QStringLiteral("Full on the 1st and 15th of every month at 21:15"));
    QCOMPARE(rules->topLevelItem(0)->text(2), expected[0]);
    QCOMPARE(lines->topLevelItem(0)->text(0), QStringLiteral("1 changed"));
    // The rule stays chosen
    QCOMPARE(s.view.currentRule(), 0);

    // The button, for the other rule; Enter on its line under "Run lines" too
    lines->setCurrentItem(lines->topLevelItem(3));
    QCOMPARE(s.view.currentRule(), 1);
    QTest::mouseClick(s.view.editRuleButton(), Qt::LeftButton);
    dialog = s.view.openRuleDialog(1);
    QVERIFY(dialog);
    QCOMPARE(dialog->windowTitle(), QStringLiteral("Calendar rule R2"));
    QCOMPARE(dialog->runText(), QStringLiteral("Run = ") + mixedRuns()[3]);
    // Accepted without a change: no step
    dialog->accept();
    QCOMPARE(s.undo()->count(), 1);
    QCOMPARE(states(s.model()), QStringLiteral("cuuuu"));

    // Undo: the reported text, byte for byte
    s.grid()->setFocus();
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(texts(s.model()), mixedRuns());
    QCOMPARE(states(s.model()), QStringLiteral("uuuuu"));
    QVERIFY(!s.view.isModified());
    QCOMPARE(rules->topLevelItem(0)->text(2), mixedRuns()[0]);
    s.key(Qt::Key_Y, Qt::ControlModifier);
    QCOMPARE(texts(s.model()), expected);

    // Changed back by hand in the form: the reported line again
    dialog = s.view.openRuleDialog(0);
    QVERIFY(dialog);
    dialog->dayButtons()[14]->setChecked(false);
    dialog->minuteBox()->setValue(0);
    dialog->accept();
    QCOMPARE(texts(s.model()), mixedRuns());
    QCOMPARE(states(s.model()), QStringLiteral("uuuuu"));
}

void BScheduleGridEditTest::calendarRuleAddedAndRemoved()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    QTreeWidget *rules = s.view.ruleList();
    // No rule yet: no list of rules and no band, but the way to add one
    QVERIFY(!rules->isVisibleTo(&s.view));
    QVERIFY(!s.grid()->gridGeometry().hasRuleBand());
    QVERIFY(s.view.addRuleButton()->isVisibleTo(&s.view));

    QTest::mouseClick(s.view.addRuleButton(), Qt::LeftButton);
    BRunRuleDialog *dialog = s.view.openRuleDialog(-1);
    QVERIFY(dialog);
    QCOMPARE(dialog->windowTitle(), QStringLiteral("New calendar rule"));
    // Full to the yearly pool on the first of January, at night
    BRunOverridesEditor *overrides = dialog->overridesEditor();
    overrides->levelCombo()->setCurrentIndex(overrides->levelCombo()->findData(QStringLiteral("Full")));
    overrides->poolEdit()->setText(QStringLiteral("Yearly"));
    for (int month = 1; month < 12; ++month)
        dialog->monthBoxes()[month]->setChecked(false);
    dialog->hourButtons()[21]->setChecked(false);
    dialog->hourButtons()[2]->setChecked(true);
    QCOMPARE(dialog->runText(), QStringLiteral("Run = Level=Full Pool=Yearly jan on 1 at 02:00"));
    QCOMPARE(dialog->readingText(), QStringLiteral("Full on the 1st of January at 02:00"));
    dialog->accept();

    // A new line at the end; the lines of the grid as they were reported
    QCOMPARE(texts(s.model()), QStringList({typicalRuns()[0], typicalRuns()[1], typicalRuns()[2],
                                            QStringLiteral("Level=Full Pool=Yearly jan on 1 at 02:00")}));
    QCOMPARE(states(s.model()), QStringLiteral("uuun"));
    QCOMPARE(s.undo()->undoText(), QStringLiteral("Add a calendar rule"));
    QVERIFY(rules->isVisibleTo(&s.view));
    QCOMPARE(rules->topLevelItemCount(), 1);
    QCOMPARE(rules->topLevelItem(0)->text(0), QStringLiteral("R1"));
    QCOMPARE(s.view.runLineList()->topLevelItem(3)->text(0), QStringLiteral("4 new"));
    QCOMPARE(s.view.runLineList()->topLevelItem(3)->text(2), QStringLiteral("calendar rule R1"));
    // It is marked in the band and chosen
    QCoreApplication::processEvents();
    QVERIFY(s.grid()->gridGeometry().hasRuleBand());
    QCOMPARE(s.grid()->rulesAtHour(2), QList<int>({0}));
    QCOMPARE(s.view.currentRule(), 0);
    // A calendar rule is not a cell: the grid did not change
    QVERIFY(s.model()->cell(Mon, 2).isEmpty());

    // Del in the list of rules removes it
    rules->setFocus();
    rules->setCurrentItem(rules->topLevelItem(0));
    QTest::keyClick(rules, Qt::Key_Delete);
    QCOMPARE(texts(s.model()), typicalRuns());
    QVERIFY(!s.view.isModified());
    QVERIFY(!rules->isVisibleTo(&s.view));
    QCOMPARE(s.undo()->undoText(), QStringLiteral("Remove the calendar rule R1"));

    // A rule the Director reported: removed with the button, back with undo
    Shown mixed(mixedSchedule());
    QVERIFY(mixed.ok);
    mixed.view.ruleList()->setCurrentItem(mixed.view.ruleList()->topLevelItem(1));
    QCOMPARE(mixed.view.currentRule(), 1);
    QTest::mouseClick(mixed.view.removeRuleButton(), Qt::LeftButton);
    QCOMPARE(states(mixed.model()), QStringLiteral("uuuru"));
    QCOMPARE(mixed.view.ruleList()->topLevelItemCount(), 1);
    QCOMPARE(listRows(mixed.view.runLineList())[3], QStringLiteral("removed | ") + mixedRuns()[3]);
    QVERIFY(!mixed.view.removeRuleButton()->isEnabled());   // nothing is chosen now
    mixed.key(Qt::Key_Z, Qt::ControlModifier);
    QCOMPARE(texts(mixed.model()), mixedRuns());
    QCOMPARE(states(mixed.model()), QStringLiteral("uuuuu"));
    QCOMPARE(mixed.view.ruleList()->topLevelItemCount(), 2);
}

void BScheduleGridEditTest::calendarRuleFormRefusesWhatIsNotARule()
{
    BRunRuleDialog dialog;
    QPushButton *ok = dialog.buttons()->button(QDialogButtonBox::Ok);
    QString error;

    // Without days of the month and weeks of the year it is a line of the grid
    dialog.daysBox()->setChecked(false);
    QVERIFY(!dialog.isValid(&error));
    QVERIFY2(error.contains(QStringLiteral("belongs to the grid")), qPrintable(error));
    QVERIFY(!ok->isEnabled());

    // Days asked for, none chosen
    dialog.daysBox()->setChecked(true);
    for (QAbstractButton *button : dialog.dayButtons())
        button->setChecked(false);
    QVERIFY(!dialog.isValid(&error));
    QVERIFY(!error.isEmpty());
    QVERIFY(!ok->isEnabled());
    dialog.dayButtons()[30]->setChecked(true);
    QVERIFY(dialog.isValid());
    QVERIFY(ok->isEnabled());
    QCOMPARE(dialog.runText(), QStringLiteral("Run = on 31 at 21:00"));

    // No weekday, no week of the month, no month: nothing would ever run
    dialog.weekdayBoxes()[3]->setChecked(true);
    for (QCheckBox *box : dialog.weekdayBoxes())
        box->setChecked(false);
    QVERIFY(!dialog.isValid());
    QVERIFY(!ok->isEnabled());
    dialog.weekdayBoxes()[1]->setChecked(true);
    QVERIFY(ok->isEnabled());
    QCOMPARE(dialog.runText(), QStringLiteral("Run = mon on 31 at 21:00"));

    // The view does not take a rule the form could not accept either
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    BRunSpec gridLine;
    gridLine.mask.wday = 1u << 1;
    gridLine.mask.hour = 1u << 3;
    QVERIFY(!s.editor()->addCalendarRule(gridLine));
    QVERIFY(s.view.editErrorText().contains(QStringLiteral("not a calendar rule")));
    QVERIFY(!s.view.isModified());
    // Nothing chosen: nothing to edit or remove
    QVERIFY(!s.view.removeRule(-1));
    QVERIFY(!s.view.openRuleDialog(5));
}

void BScheduleGridEditTest::calendarRuleFormKeyboard()
{
    BRunRuleDialog dialog;
    dialog.show();
    QVERIFY(QTest::qWaitForWindowExposed(&dialog));
    dialog.activateWindow();
    const QList<QAbstractButton *> days = dialog.dayButtons();

    // Tab reaches a block of buttons once; the arrows move inside it
    QCOMPARE(days[0]->focusPolicy(), Qt::StrongFocus);
    QCOMPARE(days[1]->focusPolicy(), Qt::ClickFocus);
    days[0]->setFocus();
    QTRY_VERIFY(days[0]->hasFocus());
    QTest::keyClick(days[0], Qt::Key_Right);
    QVERIFY(days[1]->hasFocus());
    QTest::keyClick(days[1], Qt::Key_Down);
    QVERIFY(days[9]->hasFocus());       // eight in a row
    QTest::keyClick(days[9], Qt::Key_Left);
    QVERIFY(days[8]->hasFocus());
    QTest::keyClick(days[8], Qt::Key_Up);
    QVERIFY(days[0]->hasFocus());
    QTest::keyClick(days[0], Qt::Key_Up);       // nothing above
    QVERIFY(days[0]->hasFocus());
    QTest::keyClick(days[0], Qt::Key_End);
    QVERIFY(days[30]->hasFocus());
    QTest::keyClick(days[30], Qt::Key_Home);
    QVERIFY(days[0]->hasFocus());

    // Space toggles the day the keyboard is on
    QVERIFY(days[0]->isChecked());
    QTest::keyClick(days[0], Qt::Key_Right);
    QTest::keyClick(days[1], Qt::Key_Space);
    QVERIFY(days[1]->isChecked());
    QCOMPARE(dialog.runText(), QStringLiteral("Run = on 1,2 at 21:00"));

    // The weekdays stand in the order of the week of the view
    dialog.setFirstDayOfWeek(Qt::Sunday);
    QCoreApplication::processEvents();
    QVERIFY(dialog.weekdayBoxes()[0]->x() < dialog.weekdayBoxes()[1]->x());
    dialog.setFirstDayOfWeek(Qt::Monday);
    QCoreApplication::processEvents();
    QVERIFY(dialog.weekdayBoxes()[1]->x() < dialog.weekdayBoxes()[0]->x());
}

// --- What must not be lost ----------------------------------------------------------

void BScheduleGridEditTest::scheduleWithALineNotUnderstood()
{
    Shown s(mixedSchedule());
    QVERIFY(s.ok);
    const QString bad = mixedRuns()[2];

    // It cannot be written, and the view says so before anything is edited
    QString reason;
    QVERIFY(!s.model()->canBeWritten(&reason));
    QVERIFY(reason.contains(bad));
    QVERIFY2(s.view.noticeText().contains(QStringLiteral("cannot write this schedule")),
             qPrintable(s.view.noticeText()));
    QCOMPARE(s.model()->notUnderstood().size(), 1);
    QCOMPARE(s.model()->notUnderstood().first().text, bad);

    // The other lines can be edited in memory, as the model defines
    s.click(s.cell(Sun, 5));
    s.key(Qt::Key_Space);
    QVERIFY(s.view.isModified());
    QStringList expected = mixedRuns();
    expected.append(QStringLiteral("Level=Incremental sun at 05:00"));
    QCOMPARE(texts(s.model()), expected);
    QCOMPARE(states(s.model()), QStringLiteral("uuuuun"));

    // ... but it stays locked: nothing is handed out to write
    QStringList written{QStringLiteral("left over")};
    QString error;
    QVERIFY(!s.model()->runTexts(&written, &error));
    QVERIFY(written.isEmpty());
    QVERIFY(error.contains(bad));
    QVERIFY(!s.model()->canBeWritten());
    QVERIFY(s.view.noticeText().contains(QStringLiteral("cannot write this schedule")));
    QVERIFY(!s.view.modifiedText().isEmpty());

    // The line itself is never touched, whatever is edited: everything erased ...
    s.key(Qt::Key_A, Qt::ControlModifier);
    s.key(Qt::Key_Delete);
    QCOMPARE(texts(s.model()), QStringList({mixedRuns()[0], bad, mixedRuns()[3]}));
    QCOMPARE(s.model()->notUnderstood().first().text, bad);
    const BSchedule now = s.model()->schedule();
    QCOMPARE(now.runs[1].original, bad);
    QVERIFY(!now.runs[1].dirty);
    QVERIFY(!now.runs[1].understood());
    // ... the calendar rules changed and removed ...
    QVERIFY(s.view.removeRule(0));
    QCOMPARE(s.model()->notUnderstood().first().text, bad);
    QCOMPARE(s.model()->notUnderstood().first().runIndex, 0);
    // ... and no edit of the model reaches it
    BRunSpec rule;
    rule.mask.mday = 1u;
    rule.mask.hour = 1u;
    QVERIFY(!s.model()->changeCalendarRule(0, rule).ok);
    QVERIFY(!s.model()->removeCalendarRule(0).ok);
    QVERIFY(!s.model()->moveRun(0, 30).ok);
    QVERIFY(!s.model()->canBeWritten());

    // In the list it keeps its warning sign and its text
    QTreeWidget *lines = s.view.runLineList();
    QTreeWidgetItem *row = nullptr;
    for (int i = 0; i < lines->topLevelItemCount(); ++i) {
        if (lines->topLevelItem(i)->text(1) == bad)
            row = lines->topLevelItem(i);
    }
    QVERIFY(row);
    QVERIFY(!row->icon(0).isNull());
    QVERIFY(row->text(2).startsWith(QStringLiteral("not understood: ")));

    // Everything undone: all five lines as they were reported
    QTest::mouseClick(s.view.revertButton(), Qt::LeftButton);
    QCOMPARE(texts(s.model()), mixedRuns());
    QCOMPARE(states(s.model()), QStringLiteral("uuuuu"));
    QVERIFY(!s.view.isModified());
    QVERIFY(!s.model()->canBeWritten());

    // A schedule without such a line has no such notice
    Shown typical(typicalSchedule());
    QVERIFY(typical.ok);
    QVERIFY(typical.view.noticeText().isEmpty());
    QVERIFY(typical.model()->canBeWritten());
}

void BScheduleGridEditTest::refreshWhileModified()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    QSignalSpy kept(&s.view, &BScheduleGridView::changesKept);
    QSignalSpy shown(&s.view, &BScheduleGridView::pendingScheduleShown);

    // Without changes a refresh with other Run lines just shows them
    QStringList other = typicalRuns();
    other[2] = QStringLiteral("Level=Incremental mon-sat at 22:05");
    QVERIFY(s.view.offerSchedule(scheduleOf(QStringLiteral("MonthlyCycle"), other)));
    QVERIFY(!s.view.hasPendingSchedule());
    QCOMPARE(texts(s.model()), other);
    QVERIFY(s.view.offerSchedule(typicalSchedule()));
    QCOMPARE(texts(s.model()), typicalRuns());

    // Two edits
    s.select(Mon, 3, Fri, 3);
    s.key(Qt::Key_Space);
    s.click(s.cell(Wed, 21));
    s.key(Qt::Key_Delete);
    const QStringList edited = texts(s.model());
    const BGridSelection selection = s.model()->selection();
    QCOMPARE(s.undo()->count(), 2);
    QVERIFY(!s.view.keepChangesButton()->isVisibleTo(&s.view));

    // A refresh that reports what the changes started from asks nothing
    QVERIFY(s.view.offerSchedule(typicalSchedule()));
    QVERIFY(!s.view.hasPendingSchedule());
    QCOMPARE(texts(s.model()), edited);

    // A refresh with other Run lines does not replace the changes: they
    // stay, and a notice offers the two ways on
    QVERIFY(!s.view.offerSchedule(scheduleOf(QStringLiteral("MonthlyCycle"), other)));
    QVERIFY(s.view.hasPendingSchedule());
    QCOMPARE(texts(s.model()), edited);
    QCOMPARE(s.undo()->count(), 2);
    QCOMPARE(s.model()->selection(), selection);
    QVERIFY(s.view.isModified());
    QVERIFY2(s.view.pendingText().startsWith(
                 QStringLiteral("The Director now reports other Run lines for this schedule")),
             qPrintable(s.view.pendingText()));
    QVERIFY(s.view.keepChangesButton()->isVisibleTo(&s.view));
    QVERIFY(s.view.discardButton()->isVisibleTo(&s.view));
    QCOMPARE(s.view.keepChangesButton()->text().remove(QLatin1Char('&')), QStringLiteral("Keep my changes"));
    QCOMPARE(s.view.discardButton()->text().remove(QLatin1Char('&')), QStringLiteral("Discard and reload"));
    // Editing goes on while the question is open
    s.key(Qt::Key_Z, Qt::ControlModifier);
    s.key(Qt::Key_Y, Qt::ControlModifier);
    QCOMPARE(texts(s.model()), edited);
    QVERIFY(s.view.hasPendingSchedule());
    // The same refresh again is the same question, not a second one
    QVERIFY(!s.view.offerSchedule(scheduleOf(QStringLiteral("MonthlyCycle"), other)));

    // "Keep my changes"
    QTest::mouseClick(s.view.keepChangesButton(), Qt::LeftButton);
    QVERIFY(!s.view.hasPendingSchedule());
    QVERIFY(!s.view.keepChangesButton()->isVisibleTo(&s.view));
    QVERIFY(s.view.pendingText().isEmpty());
    QCOMPARE(texts(s.model()), edited);
    QCOMPARE(s.undo()->count(), 2);
    QCOMPARE(kept.count(), 1);
    QCOMPARE(kept.last().first().toString(), QStringLiteral("MonthlyCycle"));
    QVERIFY2(s.view.modifiedText().endsWith(QStringLiteral("The Director reports other Run lines since.")),
             qPrintable(s.view.modifiedText()));
    // The lines the changes did not touch are still the bytes that were loaded
    QCOMPARE(texts(s.model())[0], typicalRuns()[0]);
    QCOMPARE(texts(s.model())[1], typicalRuns()[1]);

    // The next refresh brings the same again: answered already
    QVERIFY(!s.view.offerSchedule(scheduleOf(QStringLiteral("MonthlyCycle"), other)));
    QVERIFY(!s.view.hasPendingSchedule());
    QCOMPARE(texts(s.model()), edited);

    // Yet other Run lines are a new question
    QStringList third = other;
    third.append(QStringLiteral("Level=Full sun at 01:00"));
    QVERIFY(!s.view.offerSchedule(scheduleOf(QStringLiteral("MonthlyCycle"), third)));
    QVERIFY(s.view.hasPendingSchedule());
    // ... which a refresh with the lines the changes started from takes back
    QVERIFY(s.view.offerSchedule(typicalSchedule()));
    QVERIFY(!s.view.hasPendingSchedule());
    QCOMPARE(texts(s.model()), edited);

    // "Discard and reload"
    QVERIFY(!s.view.offerSchedule(scheduleOf(QStringLiteral("MonthlyCycle"), third)));
    QTest::mouseClick(s.view.discardButton(), Qt::LeftButton);
    QVERIFY(!s.view.hasPendingSchedule());
    QVERIFY(!s.view.discardButton()->isVisibleTo(&s.view));
    QCOMPARE(texts(s.model()), third);
    QCOMPARE(states(s.model()), QStringLiteral("uuuu"));
    QVERIFY(!s.view.isModified());
    QVERIFY(s.view.modifiedText().isEmpty());
    QCOMPARE(s.undo()->count(), 0);
    QCOMPARE(kept.count(), 1);
    QCOMPARE(shown.count(), 1);
    QCOMPARE(shown.last().first().toString(), QStringLiteral("MonthlyCycle"));

    // setSchedule() itself asks nothing: it is the caller's decision
    s.click(s.cell(Sun, 9));
    s.key(Qt::Key_Space);
    QVERIFY(s.view.isModified());
    s.view.setSchedule(typicalSchedule());
    QVERIFY(!s.view.isModified());
    QCOMPARE(texts(s.model()), typicalRuns());
}

void BScheduleGridEditTest::anotherScheduleWhileModified()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    QSignalSpy kept(&s.view, &BScheduleGridView::changesKept);
    s.click(s.cell(Sun, 9));
    s.key(Qt::Key_Space);
    const QStringList edited = texts(s.model());

    // Another schedule is chosen in the list: the changes are not thrown away
    QVERIFY(!s.view.offerSchedule(mixedSchedule()));
    QVERIFY(s.view.hasPendingSchedule());
    QCOMPARE(s.view.titleText(), QStringLiteral("Schedule MonthlyCycle"));
    QCOMPARE(texts(s.model()), edited);
    QCOMPARE(s.view.pendingText(),
             QStringLiteral("The schedule MonthlyCycle has changes that are not written. "
                            "Showing FirstOfMonth discards them."));
    QCOMPARE(s.view.discardButton()->text().remove(QLatin1Char('&')),
             QStringLiteral("Discard and show FirstOfMonth"));

    // Kept: the list is told which schedule stays
    s.view.keepChanges();
    QCOMPARE(kept.count(), 1);
    QCOMPARE(kept.last().first().toString(), QStringLiteral("MonthlyCycle"));
    QCOMPARE(texts(s.model()), edited);
    QVERIFY(!s.view.modifiedText().contains(QStringLiteral("other Run lines since")));
    QVERIFY(!s.view.hasPendingSchedule());

    // The same schedule chosen once more is asked about once more: a choice
    // in the list is made by hand, unlike a refresh
    QVERIFY(!s.view.offerSchedule(mixedSchedule()));
    QVERIFY(s.view.hasPendingSchedule());
    s.view.keepChanges();

    // Chosen again and discarded this time
    QVERIFY(!s.view.offerSchedule(scheduleOf(QStringLiteral("TenMinutes"),
                                             {QStringLiteral("Level=Incremental hourly at 0:05")})));
    s.view.discardAndReload();
    QCOMPARE(s.view.titleText(), QStringLiteral("Schedule TenMinutes"));
    QVERIFY(!s.view.isModified());
    QVERIFY(!s.view.hasPendingSchedule());
    QCOMPARE(s.undo()->count(), 0);

    // A form that is open belongs to the schedule that was on display
    BRunRuleDialog *dialog = s.view.openRuleDialog(-1);
    QVERIFY(dialog);
    QPointer<BRunRuleDialog> guard(dialog);
    s.view.setSchedule(typicalSchedule());
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    QVERIFY(guard.isNull() || !guard->isVisible());
    QVERIFY(!s.view.isModified());
}

// --- Screen readers and the keyboard round ---------------------------------------------

void BScheduleGridEditTest::accessibleEdits()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    BScheduleGridWidget *grid = s.grid();
    QTRY_VERIFY(grid->hasFocus());
    QAccessibleInterface *iface = QAccessible::queryAccessibleInterface(grid);
    QVERIFY(iface);
    QAccessibleTableInterface *table = iface->tableInterface();
    QVERIFY(table);

    // A cell of the editable grid is not read-only and has the two edit actions
    QAccessibleInterface *cell = table->cellAt(Mon, 3);
    QVERIFY(cell);
    QVERIFY(!cell->state().readOnly);
    QAccessibleActionInterface *actions = cell->actionInterface();
    QVERIFY(actions);
    QVERIFY(actions->actionNames().contains(QStringLiteral("ApplyTool")));
    QVERIFY(actions->actionNames().contains(QStringLiteral("Erase")));
    QCOMPARE(actions->localizedActionName(QStringLiteral("ApplyTool")), QStringLiteral("Apply the tool"));
    QCOMPARE(actions->localizedActionDescription(QStringLiteral("ApplyTool")),
             QStringLiteral("Applies the tool in hand (Full at :00, pool Monthly) to this cell"));
    // Headers stay what they were
    QVERIFY(iface->child(BScheduleGridAccessible::childIndex(Mon, -1))->state().readOnly);

    // As if a screen reader were listening
    s.select(Mon, 3, Tue, 3);
    accessibleSource = grid;
    accessibleNotes.clear();
    QAccessible::installUpdateHandler(noteAccessibleEvent);

    // Painting: the changed part of the table, and every changed cell by its new name
    QCOMPARE(cell->text(QAccessible::Name), QStringLiteral("Monday 3 hours: no run"));
    s.key(Qt::Key_Space);
    QVERIFY(noted(QAccessible::TableModelChanged));
    QVERIFY(noted(QAccessible::NameChanged, QStringLiteral("Monday 3 hours: Full at 03:00, pool Monthly")));
    QVERIFY(noted(QAccessible::NameChanged, QStringLiteral("Tuesday 3 hours: Full at 03:00, pool Monthly")));
    QCOMPARE(notedCount(QAccessible::NameChanged), 2);
    QCOMPARE(cell->text(QAccessible::Name), QStringLiteral("Monday 3 hours: Full at 03:00, pool Monthly"));
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    // ... and what was done, in words
    bool announced = false;
    for (const AccessibleNote &note : std::as_const(accessibleNotes))
        announced = announced || note.message == QLatin1String("Paint Full at :00, pool Monthly on 2 cell(s)");
    QVERIFY(announced);
#endif

    // Erasing one of them: that cell only
    accessibleNotes.clear();
    s.click(s.cell(Tue, 3));
    s.key(Qt::Key_Delete);
    QVERIFY(noted(QAccessible::NameChanged, QStringLiteral("Tuesday 3 hours: no run")));
    QCOMPARE(notedCount(QAccessible::NameChanged), 1);

    // Undo and redo are edits like any other
    accessibleNotes.clear();
    s.key(Qt::Key_Z, Qt::ControlModifier);
    QVERIFY(noted(QAccessible::NameChanged, QStringLiteral("Tuesday 3 hours: Full at 03:00, pool Monthly")));
    QCOMPARE(notedCount(QAccessible::NameChanged), 1);

    // An edit that fails is said too
    accessibleNotes.clear();
    s.model()->setSelection(BGridSelection());
    s.key(Qt::Key_Space);
    QCOMPARE(notedCount(QAccessible::NameChanged), 0);
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    announced = false;
    for (const AccessibleNote &note : std::as_const(accessibleNotes))
        announced = announced || note.message == QLatin1String("No cell is selected.");
    QVERIFY(announced);
#endif

    // The two actions edit the cell they belong to, whatever is selected
    accessibleNotes.clear();
    QAccessibleInterface *other = table->cellAt(Thu, 7);
    other->actionInterface()->doAction(QStringLiteral("ApplyTool"));
    QCOMPARE(s.model()->cell(Thu, 7).inScope.size(), 1);
    QVERIFY(noted(QAccessible::NameChanged, QStringLiteral("Thursday 7 hours: Full at 07:00, pool Monthly")));
    other->actionInterface()->doAction(QStringLiteral("Erase"));
    QVERIFY(s.model()->cell(Thu, 7).inScope.isEmpty());
    QCOMPARE(other->text(QAccessible::Name), QStringLiteral("Thursday 7 hours: no run"));

    // A calendar rule is not a cell: no cell is named, the description of the table changes
    accessibleNotes.clear();
    BRunSpec rule;
    rule.mask.mday = 1u;
    rule.mask.hour = 1u << 21;
    QVERIFY(s.editor()->addCalendarRule(rule));
    QCOMPARE(notedCount(QAccessible::NameChanged), 0);
    QVERIFY(iface->text(QAccessible::Description).contains(QStringLiteral("Calendar rule R1: on 1 at 21:00")));
}

void BScheduleGridEditTest::accessibleTool()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    BScheduleGridWidget *grid = s.grid();
    QAccessibleInterface *iface = QAccessible::queryAccessibleInterface(grid);
    QVERIFY(iface);

    // The table says how it is edited and which tool is in hand
    QString description = iface->text(QAccessible::Description);
    QVERIFY2(description.contains(QStringLiteral("Space applies the tool to the selected cells, "
                                                 "Delete erases them, F, D, I and V paint a level.")),
             qPrintable(description));
    QVERIFY(description.contains(QStringLiteral("Tool: Full at :00, pool Monthly.")));

    // The buttons of the palette: a name each, and the one in hand is checked
    const QList<QAbstractButton *> buttons = s.palette()->brushButtons();
    QAccessibleInterface *first = QAccessible::queryAccessibleInterface(buttons[0]);
    QAccessibleInterface *third = QAccessible::queryAccessibleInterface(buttons[2]);
    QAccessibleInterface *eraser = QAccessible::queryAccessibleInterface(s.palette()->eraserButton());
    QVERIFY(first && third && eraser);
    QCOMPARE(first->text(QAccessible::Name), QStringLiteral("Brush A = Full at :00, pool Monthly"));
    QCOMPARE(third->text(QAccessible::Name), QStringLiteral("Brush Incremental at :05"));
    QCOMPARE(eraser->text(QAccessible::Name), QStringLiteral("Eraser"));
    QVERIFY(first->state().checkable && first->state().checked);
    QVERIFY(third->state().checkable && !third->state().checked);
    QCOMPARE(QAccessible::queryAccessibleInterface(s.palette())->text(QAccessible::Name),
             QStringLiteral("Brushes"));

    // Another tool: the buttons say so, and the grid announces it
    accessibleSource = grid;
    accessibleNotes.clear();
    QAccessible::installUpdateHandler(noteAccessibleEvent);
    QTest::mouseClick(buttons[2], Qt::LeftButton);
    QVERIFY(third->state().checked);
    QVERIFY(!first->state().checked);
    QVERIFY(noted(QAccessible::DescriptionChanged));
    QVERIFY(iface->text(QAccessible::Description).contains(QStringLiteral("Tool: Incremental at :05.")));
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QVERIFY(noted(QAccessible::Announcement));
    QCOMPARE(accessibleNotes.last().message, QStringLiteral("Tool: Incremental at :05"));
#endif

    accessibleNotes.clear();
    s.editor()->setEraser();
    QVERIFY(eraser->state().checked);
    QVERIFY(noted(QAccessible::DescriptionChanged));
    QVERIFY(iface->text(QAccessible::Description).contains(QStringLiteral("Tool: Eraser.")));

    // The level keys announce the brush they take in hand
    accessibleNotes.clear();
    s.grid()->setFocus();
    s.model()->setSelection(BGridSelection());
    s.key(Qt::Key_D);
    QVERIFY(noted(QAccessible::DescriptionChanged));
    QVERIFY(iface->text(QAccessible::Description).contains(QStringLiteral("Tool: Differential at :05.")));

    s.editor()->setPaintMode(true);
    QVERIFY(iface->text(QAccessible::Description).contains(QStringLiteral("Paint mode is on.")));
}

void BScheduleGridEditTest::f6ReachesThePalette()
{
    Shown s(typicalSchedule());
    QVERIFY(s.ok);
    QTRY_VERIFY(s.grid()->hasFocus());

    // From the grid to the tool in hand, and on to the Run lines
    s.view.focusNextPart();
    QVERIFY(s.palette()->activeButton()->hasFocus());
    QCOMPARE(s.palette()->activeButton(), s.palette()->brushButtons()[0]);
    s.view.focusNextPart();
    QVERIFY(s.view.runLineList()->hasFocus());
    s.view.focusNextPart(true);
    QVERIFY(s.palette()->activeButton()->hasFocus());

    // From a button of the palette that is not the tool in hand
    s.palette()->paintModeButton()->setFocus();
    QTRY_VERIFY(s.palette()->paintModeButton()->hasFocus());
    s.view.focusNextPart(true);
    QVERIFY(s.grid()->hasFocus());

    // With the eraser in hand the round stops at the eraser
    s.editor()->setEraser();
    QTest::keyClick(s.grid(), Qt::Key_F6);
    QTRY_VERIFY(s.palette()->eraserButton()->hasFocus());
}

// --- Pictures ---------------------------------------------------------------------------

void BScheduleGridEditTest::screenshots_data()
{
    QTest::addColumn<QString>("theme");
    QTest::newRow("dark") << "dark";
    QTest::newRow("light") << "light";
}

void BScheduleGridEditTest::screenshots()
{
    QFETCH(QString, theme);
    QVERIFY2(loadTheme(theme), "theme file not found");
    const QString shots = qEnvironmentVariable("ONESIMUS_GRID_SCREENSHOT_DIR");
    auto save = [&](QWidget *widget, const QString &name) {
        QCoreApplication::processEvents();
        const QPixmap picture = widget->grab();
        QVERIFY(!picture.isNull());
        if (shots.isEmpty())
            return;
        QDir().mkpath(shots);
        const QString path = QStringLiteral("%1/s6-%2-%3.png").arg(shots, name, theme);
        QVERIFY2(picture.save(path), qPrintable(path));
    };

    // --- The palette with the brush in hand; a selection before and after painting ---
    {
        Shown s(typicalSchedule());
        QVERIFY(s.ok);
        s.editor()->setActiveBrush(0);
        save(&s.view, QStringLiteral("a-palette-active-brush"));

        s.select(Mon, 2, Fri, 4);
        save(&s.view, QStringLiteral("b-selection-before-painting"));
        const QImage before = s.viewport()->grab().toImage();
        s.key(Qt::Key_Space);
        QCoreApplication::processEvents();
        const QImage after = s.viewport()->grab().toImage();
        QVERIFY(before != after);
        save(&s.view, QStringLiteral("c-selection-after-painting"));

        // A stroke of paint mode that is not released yet
        s.model()->setSelection(BGridSelection());
        s.editor()->setActiveBrush(2);
        s.editor()->setPaintMode(true);
        QTest::mousePress(s.viewport(), Qt::LeftButton, Qt::NoModifier, s.cell(Sun, 8));
        s.move(s.cell(Sun, 14));
        const QImage stroke = s.viewport()->grab().toImage();
        QVERIFY(stroke != after);
        save(&s.view, QStringLiteral("d-paint-mode-stroke"));
        QTest::mouseRelease(s.viewport(), Qt::LeftButton, Qt::NoModifier, s.cell(Sun, 14));
        s.editor()->setPaintMode(false);

        // Changed, new and removed Run lines
        s.model()->setActiveScope(0);
        s.click(s.cell(Wed, 21));
        s.key(Qt::Key_Delete);
        s.model()->setActiveScope(2);
        s.click(s.cell(Sat, 21));
        s.key(Qt::Key_Delete);
        s.model()->setActiveScope(0);
        s.model()->setSelection(BGridSelection());
        QCOMPARE(states(s.model()), QStringLiteral("urcnn"));
        save(&s.view, QStringLiteral("e-modified-run-lines"));

        // The refresh that brought other Run lines
        QStringList other = typicalRuns();
        other[2] = QStringLiteral("Level=Incremental mon-sat at 22:05");
        QVERIFY(!s.view.offerSchedule(scheduleOf(QStringLiteral("MonthlyCycle"), other)));
        save(&s.view, QStringLiteral("f-refresh-notice"));
        s.view.keepChanges();

        // The forms
        BScheduleBrushDialog *brush = s.palette()->openBrushDialog();
        QVERIFY(brush);
        brush->overridesEditor()->levelCombo()->setCurrentIndex(
            brush->overridesEditor()->levelCombo()->findData(QStringLiteral("VirtualFull")));
        brush->overridesEditor()->poolEdit()->setText(QStringLiteral("Long Term"));
        brush->minuteBox()->setValue(30);
        QVERIFY(QTest::qWaitForWindowExposed(brush));
        save(brush, QStringLiteral("g-new-brush-form"));
        brush->reject();

        BScheduleScopeDialog *scope = s.view.openScopeDialog();
        QVERIFY(scope);
        scope->weekBoxes()[0]->setChecked(false);
        scope->weekBoxes()[2]->setChecked(true);
        QVERIFY(QTest::qWaitForWindowExposed(scope));
        save(scope, QStringLiteral("h-add-scope-form"));
        scope->accept();
        save(&s.view, QStringLiteral("i-scope-added"));
        QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    }

    // --- Calendar rules, and a schedule that cannot be written ---
    {
        Shown s(mixedSchedule());
        QVERIFY(s.ok);
        s.click(s.grid()->gridGeometry().ruleCellRect(21).center());
        save(&s.view, QStringLiteral("j-locked-schedule"));

        BRunRuleDialog *rule = s.view.openRuleDialog(0);
        QVERIFY(rule);
        rule->dayButtons()[14]->setChecked(true);
        QVERIFY(QTest::qWaitForWindowExposed(rule));
        save(rule, QStringLiteral("k-calendar-rule-form"));
        rule->accept();

        s.click(s.cell(Sun, 5));
        s.key(Qt::Key_Space);
        save(&s.view, QStringLiteral("l-locked-schedule-edited"));

        BRunRuleDialog *bad = s.view.openRuleDialog(-1);
        QVERIFY(bad);
        bad->daysBox()->setChecked(false);
        QVERIFY(QTest::qWaitForWindowExposed(bad));
        save(bad, QStringLiteral("m-calendar-rule-form-refused"));
        bad->reject();
        QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    }

    // --- A window too small for everything ---
    {
        Shown s(typicalSchedule(), QSize(700, 640));
        QVERIFY(s.ok);
        s.editor()->setEraser();
        save(&s.view, QStringLiteral("n-small-window-eraser"));
    }
}

QTEST_MAIN(BScheduleGridEditTest)
#include "bschedulegridedit_test.moc"
