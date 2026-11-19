/**
 * @file bschedulegridmodel_test.cpp
 * @brief The model behind the schedule grid
 *
 * Four groups:
 *  - from Run lines to cells, scopes and the lists beside the grid, for the
 *    schedules of the Bareos documentation (Schedule resource: WeeklyCycle,
 *    MonthlyCycle, TenMinutes, First, the modulo examples) and for the
 *    usual schedule with three scopes;
 *  - the selection, without a model;
 *  - the edits, each with the Run lines before and after written out, and
 *    fixed sequences of edits after which the cells read from the written
 *    Run lines must be the cells that were intended.
 *  - the edits of calendar rules, which are changed as a whole.
 *
 * Rows are those of a view that starts the week on Monday unless a test
 * says otherwise. Runs without a Director (registered with CTest).
 */

#include <QtTest>

#include <algorithm>

#include "schedules/model/bschedulegridmodel.h"

namespace {

// Rows of a view that starts on Monday
enum Row { Mon = 0, Tue, Wed, Thu, Fri, Sat, Sun };

BSchedule scheduleOf(const QStringList &runs)
{
    BSchedule schedule;
    schedule.name = QStringLiteral("S");
    for (const QString &run : runs)
        schedule.runs.append(BRunParser::parse(run));
    return schedule;
}

/// The lines as they would be written; not understood ones as reported
QStringList texts(const BScheduleGridModel &model)
{
    QStringList result;
    for (const BGridRunLineText &line : model.runLines()) {
        if (line.state != BGridRunLineText::Removed)
            result.append(line.text);
    }
    return result;
}

QString states(const BScheduleGridModel &model)
{
    QString result;
    for (const BGridRunLineText &line : model.runLines()) {
        switch (line.state) {
        case BGridRunLineText::Untouched: result += QLatin1Char('u'); break;
        case BGridRunLineText::Changed: result += QLatin1Char('c'); break;
        case BGridRunLineText::New: result += QLatin1Char('n'); break;
        case BGridRunLineText::Removed: result += QLatin1Char('r'); break;
        }
    }
    return result;
}

BGridBrush brush(const char *level, int minute = 0, const char *pool = nullptr)
{
    BGridBrush b;
    if (level)
        b.overrides.level = QString::fromLatin1(level);
    if (pool)
        b.overrides.pool = QString::fromLatin1(pool);
    b.minute = minute;
    return b;
}

BGridScope scopeOf(quint8 weeks, bool last = false, quint16 months = BDateTimeMask::AllMonths)
{
    BGridScope scope;
    scope.weeksOfMonth = weeks;
    scope.last = last;
    scope.months = months;
    return scope;
}

/// "Full:05" per run of a list, in its order
QString runsText(const QList<BGridRun> &runs)
{
    QStringList parts;
    for (const BGridRun &run : runs) {
        parts.append(QStringLiteral("%1:%2")
                         .arg(run.level() ? *run.level() : QStringLiteral("-"))
                         .arg(run.minute, 2, 10, QLatin1Char('0')));
    }
    return parts.join(QLatin1Char(' '));
}

int occupiedCells(const BScheduleGridModel &model)
{
    int count = 0;
    for (int row = 0; row < 7; ++row) {
        for (int column = 0; column < 24; ++column) {
            if (!model.cell(row, column).inScope.isEmpty())
                ++count;
        }
    }
    return count;
}

void select(BScheduleGridModel &model, int row1, int column1, int row2, int column2)
{
    BGridSelection selection;
    selection.selectCell(row1, column1);
    selection.extendTo(row2, column2);
    model.setSelection(selection);
}

void select(BScheduleGridModel &model, int row, int column)
{
    select(model, row, column, row, column);
}

// --- For the sequences: what the cells are meant to contain ----------------

/// One run in one cell: "months/weeks/last|weekday|hour|brush"
QString entry(const BGridScope &scope, int weekday, int hour, const BGridBrush &b)
{
    BRunSpec spec;
    spec.overrides = b.overrides;
    spec.minute = b.minute;
    return QStringLiteral("%1/%2/%3|%4|%5|%6")
        .arg(scope.months).arg(scope.weeksOfMonth).arg(scope.last ? 1 : 0)
        .arg(weekday).arg(hour, 2, 10, QLatin1Char('0'))
        .arg(BRunWriter::write(spec));
}

bool isGridSpec(const BRunSpec &spec)
{
    return spec.mask.mday == BDateTimeMask::AllMdays && spec.mask.woy == BDateTimeMask::AllWoys;
}

/// Every run of every cell of every scope, read from Run lines
QStringList entriesOf(const QStringList &runTexts)
{
    QStringList result;
    for (const QString &text : runTexts) {
        const BRunLine line = BRunParser::parse(text);
        if (!line.spec || !isGridSpec(*line.spec))
            continue;
        const BGridScope scope = BGridScope::of(line.spec->mask);
        const BGridBrush b{line.spec->overrides, line.spec->minute};
        for (int weekday = 0; weekday < 7; ++weekday) {
            for (int hour = 0; hour < 24; ++hour) {
                if ((line.spec->mask.wday & (1u << weekday)) && (line.spec->mask.hour & (1u << hour)))
                    result.append(entry(scope, weekday, hour, b));
            }
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}

QStringList notGridLines(const QStringList &runTexts)
{
    QStringList result;
    for (const QString &text : runTexts) {
        const BRunLine line = BRunParser::parse(text);
        if (!line.spec || !isGridSpec(*line.spec))
            result.append(text);
    }
    return result;
}

/// A fixed sequence of numbers (the same on every machine)
struct Sequence
{
    quint32 state;
    int next(int below)
    {
        state = state * 1664525u + 1013904223u;
        return int((state >> 8) % quint32(below));
    }
};

/// The smallest number of products a set of cells can be cut into, by trying
/// every product that covers the first open cell (for small sets only)
bool cutInto(const BGridCellSet &open, int parts)
{
    if (open.isEmpty())
        return true;
    if (parts == 0)
        return false;
    const BGridPos first = open.cells().first();
    for (int rows = 1; rows < (1 << BGridCellSet::Rows); ++rows) {
        if (!(rows & (1 << first.row)))
            continue;
        // No row outside the set
        bool possible = true;
        quint32 common = BGridCellSet::AllColumns;
        for (int row = 0; row < BGridCellSet::Rows; ++row) {
            if (rows & (1 << row)) {
                common &= open.rowBits(row);
                if (!open.rowBits(row))
                    possible = false;
            }
        }
        if (!possible || !(common & (1u << first.column)))
            continue;
        for (quint32 columns = common; columns; columns = (columns - 1) & common) {
            if (!(columns & (1u << first.column)))
                continue;
            if (cutInto(open.subtracted(BGridCellSet::product(quint8(rows), columns)), parts - 1))
                return true;
        }
    }
    return false;
}

} // namespace

class BScheduleGridModelTest : public QObject
{
    Q_OBJECT

private slots:
    // Cell sets
    void productsOfAProduct();
    void productsAfterACellIsGone();
    void productsByColumns();
    void productsNeitherByRowsNorByColumns();
    void productsAreSmallest();
    void productsOfLargeSetsAreAPartition();
    void rectsAndBounds();

    // Selection
    void selectionCell();
    void selectionRectangleInAllDirections();
    void selectionToggle();
    void selectionRowColumnAll();
    void selectionKeepsWeekdaysWhenTheWeekStartMoves();

    // Run lines to cells
    void emptyModel();
    void docWeeklyCycle();
    void docMonthlyCycleWithPools();
    void docHourlyForms();
    void lineWithSeveralHours();
    void lineWithoutTime();
    void usualThreeScopes();
    void firstDayOfWeek();
    void severalRunsInOneCell();
    void overlappingScopes();
    void addAndRemoveScope();
    void calendarRulesAndNotUnderstood();
    void brushesAndCellsOfRun();
    void loadHook();

    // Edits
    void eraseOneWeekday();
    void eraseFromTheMiddleSplits();
    void paintNewProduct();
    void paintWholeRowIsHourly();
    void paintOverAnotherBrush();
    void paintAddsBesideAnotherBrush();
    void paintWhatIsThereIsNoChange();
    void eraseWholeLine();
    void editInOneScopeLeavesTheOthers();
    void paintInAddedScopes();
    void untouchedLineIsNotRestructured();
    void generatedLinesMerge();
    void backToTheLoadedLine();
    void moveRunToAnotherMinute();
    void failedWriteBackChangesNothing();
    void calendarRulesAndNotUnderstoodSurviveEdits();
    void notUnderstoodBlocksWritingOnly();
    void restoreUndoesAndRedoes();
    void runLinesWithStates();
    void signalsOfAnEdit();
    void calendarRuleAdded();
    void calendarRuleChangedAndBack();
    void calendarRuleRemoved();
    void calendarRuleEditsRefuseWhatIsNotOne();
    void sequencesKeepTheIntendedCells_data();
    void sequencesKeepTheIntendedCells();
};

// ============================================================================
// Cell sets
// ============================================================================

void BScheduleGridModelTest::productsOfAProduct()
{
    const BGridCellSet set = BGridCellSet::product(0x3E, (1u << 21) | (1u << 22));
    BGridProduct product;
    QVERIFY(set.isProduct(&product));
    QCOMPARE(product, (BGridProduct{0x3E, (1u << 21) | (1u << 22)}));
    QCOMPARE(set.products(), QList<BGridProduct>({product}));
    QCOMPARE(set.count(), 10);

    QVERIFY(!BGridCellSet().isProduct());
    QVERIFY(BGridCellSet().products().isEmpty());
}

void BScheduleGridModelTest::productsAfterACellIsGone()
{
    // Rows 1 to 5 at 21, 22 and 23, without row 3 at 22
    const quint32 hours = (1u << 21) | (1u << 22) | (1u << 23);
    BGridCellSet set = BGridCellSet::product(0x3E, hours);
    set.remove(3, 22);
    QVERIFY(!set.isProduct());
    // The larger part first; with as many products by rows as by columns, by rows
    QCOMPARE(set.products(), QList<BGridProduct>({
        BGridProduct{0x36, hours},
        BGridProduct{0x08, (1u << 21) | (1u << 23)},
    }));
}

void BScheduleGridModelTest::productsByColumns()
{
    // Row 0: columns 1,2. Row 1: columns 1,2,3. Row 2: column 3.
    // Three different rows, but two products: rows 0,1 x 1,2 and rows 1,2 x 3
    BGridCellSet set;
    set.setRowBits(0, 0x06);
    set.setRowBits(1, 0x0E);
    set.setRowBits(2, 0x08);
    QCOMPARE(set.products(), QList<BGridProduct>({
        BGridProduct{0x03, 0x06},
        BGridProduct{0x06, 0x08},
    }));
}

void BScheduleGridModelTest::productsNeitherByRowsNorByColumns()
{
    // The set of productsByColumns() (best by columns: 2, by rows: 3) beside
    // its mirror image (best by rows: 2, by columns: 3). By rows alone or by
    // columns alone it takes five products; four are enough.
    BGridCellSet set;
    set.setRowBits(0, 0x03);
    set.setRowBits(1, 0x07);
    set.setRowBits(2, 0x04);
    set.setRowBits(3, 0x18);
    set.setRowBits(4, 0x18);
    set.setRowBits(5, 0x30);
    const QList<BGridProduct> products = set.products();
    QCOMPARE(products.size(), 4);
    BGridCellSet all;
    for (const BGridProduct &product : products) {
        const BGridCellSet part = BGridCellSet::product(product.rows, product.columns);
        QVERIFY(!all.intersects(part));
        all = all.united(part);
    }
    QCOMPARE(all, set);
    QVERIFY(!cutInto(set, 3));
}

void BScheduleGridModelTest::productsAreSmallest()
{
    // 300 fixed sets in 4 rows and 5 columns, each checked against a search
    // that tries every product
    Sequence numbers{20261119};
    for (int n = 0; n < 300; ++n) {
        BGridCellSet set;
        for (int row = 0; row < 4; ++row)
            set.setRowBits(row, quint32(numbers.next(32)));
        const QList<BGridProduct> products = set.products();

        BGridCellSet all;
        for (const BGridProduct &product : products) {
            const BGridCellSet part = BGridCellSet::product(product.rows, product.columns);
            QVERIFY2(!all.intersects(part), qPrintable(QString::number(n)));
            all = all.united(part);
        }
        QVERIFY2(all == set, qPrintable(QString::number(n)));
        if (!products.isEmpty()) {
            QVERIFY2(!cutInto(set, int(products.size()) - 1),
                     qPrintable(QStringLiteral("set %1: fewer than %2 products are possible")
                                    .arg(n).arg(products.size())));
        }
    }
}

void BScheduleGridModelTest::productsOfLargeSetsAreAPartition()
{
    Sequence numbers{4711};
    for (int n = 0; n < 200; ++n) {
        BGridCellSet set;
        QSet<quint32> distinctRows;
        for (int row = 0; row < 7; ++row) {
            // Dense, sparse and repeated rows
            quint32 bits = quint32(numbers.next(1 << 12)) | (quint32(numbers.next(1 << 12)) << 12);
            if (n % 3 == 1)
                bits &= quint32(numbers.next(1 << 12)) | (quint32(numbers.next(1 << 12)) << 12);
            if (n % 3 == 2 && row > 0 && numbers.next(2))
                bits = set.rowBits(numbers.next(row));
            set.setRowBits(row, bits);
            if (bits)
                distinctRows.insert(bits);
        }
        const QList<BGridProduct> products = set.products();
        BGridCellSet all;
        for (const BGridProduct &product : products) {
            QVERIFY(product.rows && product.columns);
            const BGridCellSet part = BGridCellSet::product(product.rows, product.columns);
            QVERIFY(!all.intersects(part));
            all = all.united(part);
        }
        QCOMPARE(all, set);
        QVERIFY(products.size() <= distinctRows.size());
    }
}

void BScheduleGridModelTest::rectsAndBounds()
{
    // An L: rows 1-3 in columns 2-3, and row 3 on to column 6
    BGridCellSet set = BGridCellSet::rect(1, 2, 3, 3).united(BGridCellSet::rect(3, 4, 3, 6));
    QCOMPARE(set.count(), 9);
    QCOMPARE(set.boundingRect(), (BGridRect{1, 2, 3, 6}));
    QCOMPARE(set.rects(), QList<BGridRect>({BGridRect{1, 2, 2, 3}, BGridRect{3, 2, 3, 6}}));

    QVERIFY(!BGridCellSet().boundingRect().isValid());
    QVERIFY(BGridCellSet().rects().isEmpty());
    QCOMPARE(BGridCellSet::all().rects(), QList<BGridRect>({BGridRect{0, 0, 6, 23}}));
}

// ============================================================================
// Selection
// ============================================================================

void BScheduleGridModelTest::selectionCell()
{
    BGridSelection selection;
    QVERIFY(selection.isEmpty());
    QVERIFY(!selection.anchor());

    selection.selectCell(2, 9);
    QCOMPARE(selection.count(), 1);
    QVERIFY(selection.contains(2, 9));
    QVERIFY(!selection.contains(2, 10));
    QCOMPARE(*selection.anchor(), (BGridPos{2, 9}));

    // Another click replaces
    selection.selectCell(4, 1);
    QCOMPARE(selection.count(), 1);
    QVERIFY(selection.contains(4, 1));

    // Outside the grid: nothing happens
    selection.selectCell(7, 0);
    selection.selectCell(0, 24);
    selection.selectCell(-1, 0);
    QCOMPARE(selection.count(), 1);
    QVERIFY(selection.contains(4, 1));
}

void BScheduleGridModelTest::selectionRectangleInAllDirections()
{
    BGridSelection selection;
    selection.selectCell(3, 10);

    selection.extendTo(5, 12);  // down and right
    QCOMPARE(selection.boundingRect(), (BGridRect{3, 10, 5, 12}));
    QCOMPARE(selection.count(), 9);

    selection.extendTo(1, 8);   // up and left: the anchor stays
    QCOMPARE(selection.boundingRect(), (BGridRect{1, 8, 3, 10}));
    QCOMPARE(selection.count(), 9);
    QVERIFY(!selection.contains(5, 12));

    selection.extendTo(1, 12);  // up and right
    QCOMPARE(selection.boundingRect(), (BGridRect{1, 10, 3, 12}));
    QCOMPARE(selection.count(), 9);

    selection.extendTo(5, 8);   // down and left
    QCOMPARE(selection.boundingRect(), (BGridRect{3, 8, 5, 10}));
    QCOMPARE(selection.count(), 9);

    selection.extendTo(3, 10);  // back to the anchor
    QCOMPARE(selection.count(), 1);
    QCOMPARE(selection.rects(), QList<BGridRect>({BGridRect{3, 10, 3, 10}}));
    QCOMPARE(*selection.anchor(), (BGridPos{3, 10}));

    // Without an anchor the cell becomes the anchor
    BGridSelection fresh;
    fresh.extendTo(2, 2);
    QCOMPARE(fresh.count(), 1);
    fresh.extendTo(3, 3);
    QCOMPARE(fresh.count(), 4);
}

void BScheduleGridModelTest::selectionToggle()
{
    BGridSelection selection;
    selection.selectCell(0, 0);
    selection.toggleCell(2, 2);
    QCOMPARE(selection.count(), 2);
    QVERIFY(selection.contains(0, 0) && selection.contains(2, 2));

    // Extending from a toggled-on cell adds the rectangle to the rest
    selection.extendTo(3, 3, true);
    QCOMPARE(selection.count(), 5);
    selection.extendTo(2, 3, true);     // and shrinks again while dragging
    QCOMPARE(selection.count(), 3);
    QVERIFY(selection.contains(0, 0) && selection.contains(2, 2) && selection.contains(2, 3));

    // Toggling a selected cell deselects it
    selection.toggleCell(0, 0);
    QCOMPARE(selection.count(), 2);
    QVERIFY(!selection.contains(0, 0));

    // Extending from a toggled-off cell takes the rectangle away
    selection.toggleCell(2, 2);
    QCOMPARE(selection.count(), 1);
    selection.extendTo(2, 3, true);
    QVERIFY(selection.isEmpty());

    // Without "keep" the rectangle replaces everything
    selection.selectCell(6, 23);
    selection.toggleCell(1, 1);
    selection.extendTo(2, 2);
    QCOMPARE(selection.count(), 4);
    QVERIFY(!selection.contains(6, 23));
}

void BScheduleGridModelTest::selectionRowColumnAll()
{
    BGridSelection selection;
    selection.selectRow(2);
    QCOMPARE(selection.count(), 24);
    QVERIFY(selection.isRowSelected(2));
    QVERIFY(!selection.isRowSelected(3));
    QVERIFY(!selection.isColumnSelected(5));

    selection.selectColumn(5, BGridSelection::Add);
    QCOMPARE(selection.count(), 30);
    QVERIFY(selection.isColumnSelected(5));

    // A whole row that is selected is deselected by the modifier
    selection.selectRow(2, BGridSelection::Toggle);
    QCOMPARE(selection.count(), 6);
    QVERIFY(!selection.contains(2, 5));
    QVERIFY(!selection.isColumnSelected(5));
    // A row that is only partly selected is selected by it
    selection.selectRow(3, BGridSelection::Toggle);
    QCOMPARE(selection.count(), 6 + 23);
    QVERIFY(selection.isRowSelected(3));

    // Dragging over row headers: from the header's anchor to the last column
    selection.selectRow(1);
    selection.extendTo(3, 23);
    QCOMPARE(selection.count(), 72);
    QVERIFY(selection.isRowSelected(1) && selection.isRowSelected(2) && selection.isRowSelected(3));

    // Dragging over column headers
    selection.selectColumn(20);
    QCOMPARE(selection.count(), 7);
    selection.extendTo(6, 22);
    QCOMPARE(selection.count(), 21);
    QCOMPARE(selection.boundingRect(), (BGridRect{0, 20, 6, 22}));

    selection.selectAll();
    QCOMPARE(selection.count(), 168);
    QCOMPARE(selection.rects(), QList<BGridRect>({BGridRect{0, 0, 6, 23}}));

    selection.clear();
    QVERIFY(selection.isEmpty());
    QVERIFY(!selection.anchor());
    QVERIFY(!selection.boundingRect().isValid());
}

void BScheduleGridModelTest::selectionKeepsWeekdaysWhenTheWeekStartMoves()
{
    BScheduleGridModel model;
    QSignalSpy changed(&model, &BScheduleGridModel::selectionChanged);

    BGridSelection selection;
    selection.selectCell(Mon, 8);
    selection.extendTo(Tue, 9);
    model.setSelection(selection);
    QCOMPARE(changed.count(), 1);
    model.setSelection(selection);      // the same again
    QCOMPARE(changed.count(), 1);

    model.setFirstDayOfWeek(Qt::Sunday);
    // Monday and Tuesday are rows 1 and 2 now
    QCOMPARE(model.selection().boundingRect(), (BGridRect{1, 8, 2, 9}));
    QCOMPARE(*model.selection().anchor(), (BGridPos{1, 8}));
    QCOMPARE(changed.count(), 2);
}

// ============================================================================
// Run lines to cells
// ============================================================================

void BScheduleGridModelTest::emptyModel()
{
    BScheduleGridModel model;
    // "Every week" is always there: a place to paint
    QCOMPARE(model.scopes().size(), 1);
    QVERIFY(model.scopes().first().scope.isEveryWeek());
    QCOMPARE(model.scopes().first().runCount, 0);
    QCOMPARE(model.activeScopeIndex(), 0);
    QCOMPARE(occupiedCells(model), 0);
    QVERIFY(model.calendarRules().isEmpty());
    QVERIFY(model.notUnderstood().isEmpty());
    QVERIFY(model.runLines().isEmpty());
    QVERIFY(!model.isModified());
    QVERIFY(model.canBeWritten());
    QVERIFY(model.cell(7, 0).isEmpty());
    QVERIFY(model.cell(0, 24).isEmpty());
}

void BScheduleGridModelTest::docWeeklyCycle()
{
    // Bareos documentation, Schedule resource
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Level=Full sun at 2:05",
        "Level=Incremental mon-sat at 2:05",
    }));

    QCOMPARE(model.scopes().size(), 1);
    QCOMPARE(model.scopes().first().runCount, 2);
    QCOMPARE(occupiedCells(model), 7);

    const BGridCell sunday = model.cell(Sun, 2);
    QCOMPARE(sunday.inScope.size(), 1);
    QCOMPARE(sunday.inScope.first().runIndex, 0);
    QCOMPARE(*sunday.inScope.first().level(), QString("Full"));
    QCOMPARE(sunday.inScope.first().minute, 5);
    QCOMPARE(sunday.inScope.first().scopeIndex, 0);
    QVERIFY(!sunday.inScope.first().withoutTime);
    QVERIFY(sunday.outOfScope.isEmpty());
    QVERIFY(!sunday.expectedConcurrentRuns);

    for (int row = Mon; row <= Sat; ++row) {
        const BGridCell cell = model.cell(row, 2);
        QCOMPARE(runsText(cell.inScope), QString("Incremental:05"));
        QCOMPARE(cell.inScope.first().runIndex, 1);
    }
    QVERIFY(model.cell(Sun, 3).isEmpty());
    QVERIFY(model.cell(Mon, 1).isEmpty());
    QVERIFY(model.canBeWritten());
    QVERIFY(!model.isModified());
}

void BScheduleGridModelTest::docMonthlyCycleWithPools()
{
    // Bareos documentation: three lines, three scopes
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Level=Full Pool=Monthly 1st sun at 2:05",
        "Level=Differential 2nd-5th sun at 2:05",
        "Level=Incremental Pool=Daily mon-sat at 2:05",
    }));

    QCOMPARE(model.scopes().size(), 3);
    QCOMPARE(model.scopes()[0], (BGridScopeInfo{BGridScope::everyWeek(), 1}));
    QCOMPARE(model.scopes()[1], (BGridScopeInfo{scopeOf(0x01), 1}));
    QCOMPARE(model.scopes()[2], (BGridScopeInfo{scopeOf(0x1E), 1}));

    // Every week: the incrementals; the Sunday runs are of other scopes
    QCOMPARE(model.activeScopeIndex(), 0);
    QCOMPARE(occupiedCells(model), 6);
    BGridCell monday = model.cell(Mon, 2);
    QCOMPARE(runsText(monday.inScope), QString("Incremental:05"));
    QCOMPARE(*monday.inScope.first().overrides.pool, QString("Daily"));
    BGridCell sunday = model.cell(Sun, 2);
    QVERIFY(sunday.inScope.isEmpty());
    QCOMPARE(runsText(sunday.outOfScope), QString("Full:05 Differential:05"));
    QCOMPARE(sunday.outOfScope[0].scopeIndex, 1);
    QCOMPARE(*sunday.outOfScope[0].overrides.pool, QString("Monthly"));
    QCOMPARE(sunday.outOfScope[1].scopeIndex, 2);
    QVERIFY(!sunday.outOfScope[1].overrides.pool);
    // "Every week" has dates in common with every scope
    QVERIFY(sunday.outOfScope[0].sharesDatesWithActiveScope);
    QVERIFY(sunday.outOfScope[1].sharesDatesWithActiveScope);

    // First week
    QSignalSpy active(&model, &BScheduleGridModel::activeScopeChanged);
    model.setActiveScope(1);
    QCOMPARE(active.count(), 1);
    QCOMPARE(model.activeScope(), scopeOf(0x01));
    QCOMPARE(occupiedCells(model), 1);
    sunday = model.cell(Sun, 2);
    QCOMPARE(runsText(sunday.inScope), QString("Full:05"));
    QCOMPARE(sunday.inScope.first().runIndex, 0);
    QCOMPARE(runsText(sunday.outOfScope), QString("Differential:05"));
    // The first week and the second to fifth never fall on the same day
    QVERIFY(!sunday.outOfScope.first().sharesDatesWithActiveScope);
    monday = model.cell(Mon, 2);
    QVERIFY(monday.inScope.isEmpty());
    QCOMPARE(runsText(monday.outOfScope), QString("Incremental:05"));
    QVERIFY(monday.outOfScope.first().sharesDatesWithActiveScope);

    // Second to fifth week
    model.setActiveScope(2);
    QCOMPARE(runsText(model.cell(Sun, 2).inScope), QString("Differential:05"));
    QCOMPARE(runsText(model.cell(Sun, 2).outOfScope), QString("Full:05"));
}

void BScheduleGridModelTest::docHourlyForms()
{
    // Bareos documentation, "TenMinutes": six lines, each every hour
    QStringList runs;
    for (int minute = 5; minute < 60; minute += 10)
        runs.append(QStringLiteral("Level=Full hourly at 0:%1").arg(minute, 2, 10, QLatin1Char('0')));
    BScheduleGridModel model;
    model.setSchedule(scheduleOf(runs));

    QCOMPARE(model.scopes().size(), 1);
    QCOMPARE(occupiedCells(model), 168);
    for (int row = 0; row < 7; ++row) {
        for (int column = 0; column < 24; ++column) {
            QCOMPARE(runsText(model.cell(row, column).inScope),
                     QString("Full:05 Full:15 Full:25 Full:35 Full:45 Full:55"));
        }
    }
    QVERIFY(!model.cell(0, 0).inScope.first().withoutTime);

    // "hourly" on some days is a band of whole rows
    model.setSchedule(scheduleOf({"Incremental sat-sun hourly at 0:30"}));
    QCOMPARE(occupiedCells(model), 48);
    for (int column = 0; column < 24; ++column) {
        QCOMPARE(runsText(model.cell(Sat, column).inScope), QString("Incremental:30"));
        QCOMPARE(runsText(model.cell(Sun, column).inScope), QString("Incremental:30"));
        QVERIFY(model.cell(Fri, column).isEmpty());
    }

    // "hourly" alone says every hour at :00 and is not a line to warn about
    model.setSchedule(scheduleOf({"Incremental hourly"}));
    QCOMPARE(occupiedCells(model), 168);
    QCOMPARE(runsText(model.cell(Wed, 13).inScope), QString("Incremental:00"));
    QVERIFY(!model.cell(Wed, 13).inScope.first().withoutTime);
}

void BScheduleGridModelTest::lineWithSeveralHours()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({"Incremental mon,wed at 8:15 at 12:15 at 16:15"}));
    QCOMPARE(occupiedCells(model), 6);
    for (const int row : {int(Mon), int(Wed)}) {
        for (const int hour : {8, 12, 16})
            QCOMPARE(runsText(model.cell(row, hour).inScope), QString("Incremental:15"));
    }
    QVERIFY(model.cell(Tue, 8).isEmpty());
    QVERIFY(model.cell(Mon, 9).isEmpty());
}

void BScheduleGridModelTest::lineWithoutTime()
{
    // No "at" and no "hourly": every hour at :00, because nothing says otherwise
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({"Full sat", "Incremental mon at 3:00"}));
    QCOMPARE(occupiedCells(model), 25);
    for (int column = 0; column < 24; ++column) {
        const BGridCell cell = model.cell(Sat, column);
        QCOMPARE(runsText(cell.inScope), QString("Full:00"));
        QVERIFY(cell.inScope.first().withoutTime);
    }
    QVERIFY(!model.cell(Mon, 3).inScope.first().withoutTime);

    // The same for a calendar rule
    model.setSchedule(scheduleOf({"Full on 1"}));
    QCOMPARE(model.calendarRules().size(), 1);
    QVERIFY(model.calendarRules().first().withoutTime);
}

void BScheduleGridModelTest::usualThreeScopes()
{
    // Full on the first Saturday, Differential on the others, Incremental
    // from Monday to Friday
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Full 1st sat at 21:00",
        "Differential 2nd-5th sat at 21:00",
        "Incremental mon-fri at 21:00",
    }));

    QCOMPARE(model.scopes().size(), 3);
    QVERIFY(model.scopes()[0].scope.isEveryWeek());
    QCOMPARE(model.scopes()[1].scope, scopeOf(0x01));
    QCOMPARE(model.scopes()[2].scope, scopeOf(0x1E));
    QCOMPARE(model.indexOfScope(scopeOf(0x1E)), 2);
    QCOMPARE(model.indexOfScope(scopeOf(0x02)), -1);

    QCOMPARE(occupiedCells(model), 5);
    QCOMPARE(runsText(model.cell(Fri, 21).inScope), QString("Incremental:00"));
    QCOMPARE(runsText(model.cell(Sat, 21).outOfScope), QString("Full:00 Differential:00"));

    model.setActiveScope(1);
    QCOMPARE(occupiedCells(model), 1);
    QCOMPARE(runsText(model.cell(Sat, 21).inScope), QString("Full:00"));
    QCOMPARE(runsText(model.cell(Sat, 21).outOfScope), QString("Differential:00"));
    QCOMPARE(runsText(model.cell(Fri, 21).outOfScope), QString("Incremental:00"));

    model.setActiveScope(2);
    QCOMPARE(occupiedCells(model), 1);
    QCOMPARE(runsText(model.cell(Sat, 21).inScope), QString("Differential:00"));

    // Out of range: nothing changes
    model.setActiveScope(3);
    QCOMPARE(model.activeScopeIndex(), 2);
}

void BScheduleGridModelTest::firstDayOfWeek()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({"Full sun at 1:00", "Incremental mon at 2:00", "Differential sat at 3:00"}));

    // Monday first (the default)
    QCOMPARE(model.firstDayOfWeek(), Qt::Monday);
    QCOMPARE(model.weekdayOfRow(0), 1);     // Bareos: 0 = Sunday
    QCOMPARE(model.weekdayOfRow(5), 6);
    QCOMPARE(model.weekdayOfRow(6), 0);
    QCOMPARE(model.rowOfWeekday(0), 6);
    QCOMPARE(model.rowOfWeekday(1), 0);
    QCOMPARE(model.dayOfWeekOfRow(0), Qt::Monday);
    QCOMPARE(model.dayOfWeekOfRow(6), Qt::Sunday);
    QCOMPARE(runsText(model.cell(0, 2).inScope), QString("Incremental:00"));
    QCOMPARE(runsText(model.cell(5, 3).inScope), QString("Differential:00"));
    QCOMPARE(runsText(model.cell(6, 1).inScope), QString("Full:00"));

    // Sunday first
    QSignalSpy changed(&model, &BScheduleGridModel::firstDayOfWeekChanged);
    model.setFirstDayOfWeek(Qt::Sunday);
    QCOMPARE(changed.count(), 1);
    QCOMPARE(model.weekdayOfRow(0), 0);
    QCOMPARE(model.weekdayOfRow(6), 6);
    QCOMPARE(model.rowOfWeekday(0), 0);
    QCOMPARE(model.rowOfWeekday(6), 6);
    QCOMPARE(model.dayOfWeekOfRow(0), Qt::Sunday);
    QCOMPARE(model.dayOfWeekOfRow(1), Qt::Monday);
    QCOMPARE(runsText(model.cell(0, 1).inScope), QString("Full:00"));
    QCOMPARE(runsText(model.cell(1, 2).inScope), QString("Incremental:00"));
    QCOMPARE(runsText(model.cell(6, 3).inScope), QString("Differential:00"));
    // By weekday of Bareos nothing moved
    QCOMPARE(runsText(model.cellAtWeekday(0, 1).inScope), QString("Full:00"));
    QCOMPARE(model.cellsOfRun(0), BGridCellSet::rect(0, 1, 0, 1));

    // An edit lands on the weekday the row shows
    select(model, 2, 4);    // Tuesday
    QVERIFY(model.applyBrush(brush("Full")).ok);
    QCOMPARE(texts(model).last(), QString("Level=Full tue at 04:00"));

    // Saturday first
    model.setFirstDayOfWeek(Qt::Saturday);
    QCOMPARE(model.weekdayOfRow(0), 6);
    QCOMPARE(model.weekdayOfRow(1), 0);
    QCOMPARE(model.rowOfWeekday(1), 2);
    QCOMPARE(runsText(model.cell(0, 3).inScope), QString("Differential:00"));
    QCOMPARE(runsText(model.cell(1, 1).inScope), QString("Full:00"));

    for (int row = 0; row < 7; ++row)
        QCOMPARE(model.rowOfWeekday(model.weekdayOfRow(row)), row);
}

void BScheduleGridModelTest::severalRunsInOneCell()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Full sat at 21:30",
        "Incremental sat at 21:00",
        "Differential sat at 21:10",
        "Level=VirtualFull Pool=Long sat at 21:10",
    }));
    const BGridCell cell = model.cell(Sat, 21);
    // By minute, then in the order of the lines
    QCOMPARE(runsText(cell.inScope),
             QString("Incremental:00 Differential:10 VirtualFull:10 Full:30"));
    QCOMPARE(cell.inScope[0].runIndex, 1);
    QCOMPARE(cell.inScope[1].runIndex, 2);
    QCOMPARE(cell.inScope[2].runIndex, 3);
    QCOMPARE(cell.inScope[3].runIndex, 0);
    QCOMPARE(*cell.inScope[2].overrides.pool, QString("Long"));
    QCOMPARE(occupiedCells(model), 1);

    // A line without a level has no level
    model.setSchedule(scheduleOf({"sat at 21:00", "Full sat at 21:00"}));
    QCOMPARE(runsText(model.cell(Sat, 21).inScope), QString("-:00 Full:00"));
    QVERIFY(!model.cell(Sat, 21).inScope.first().level());
}

void BScheduleGridModelTest::overlappingScopes()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Full 1st sat at 1:00",
        "Full 1st-2nd sat at 2:00",
        "Full jan-mar 1st sat at 3:00",
        "Full last sat at 4:00",
        "Full sat at 5:00",
        "Full 4th sat at 6:00",
        "Incremental 1st sat at 7:00",
    }));

    // Each spelling is a scope of its own; nothing is merged or cut up.
    // All months first; within that by first week, "last" at the end.
    const QList<BGridScopeInfo> expected = {
        {BGridScope::everyWeek(), 1},
        {scopeOf(0x01), 2},
        {scopeOf(0x03), 1},
        {scopeOf(0x08), 1},
        {scopeOf(0x00, true), 1},
        {scopeOf(0x01, false, 0x007), 1},
    };
    QCOMPARE(model.scopes(), expected);

    // In "1st": the two lines that say exactly "1st"
    model.setActiveScope(1);
    QCOMPARE(occupiedCells(model), 2);
    QCOMPARE(runsText(model.cell(Sat, 1).inScope), QString("Full:00"));
    QCOMPARE(runsText(model.cell(Sat, 7).inScope), QString("Incremental:00"));
    // "1st-2nd" and "jan-mar 1st" are other scopes that share dates with it
    QVERIFY(model.cell(Sat, 2).inScope.isEmpty());
    QCOMPARE(model.cell(Sat, 2).outOfScope.first().scopeIndex, 2);
    QVERIFY(model.cell(Sat, 2).outOfScope.first().sharesDatesWithActiveScope);
    QCOMPARE(model.cell(Sat, 3).outOfScope.first().scopeIndex, 5);
    QVERIFY(model.cell(Sat, 3).outOfScope.first().sharesDatesWithActiveScope);
    QVERIFY(model.cell(Sat, 5).outOfScope.first().sharesDatesWithActiveScope);
    // "last" and "4th" do not
    QVERIFY(!model.cell(Sat, 4).outOfScope.first().sharesDatesWithActiveScope);
    QVERIFY(!model.cell(Sat, 6).outOfScope.first().sharesDatesWithActiveScope);

    // "last" is days in the fourth and fifth week
    model.setActiveScope(4);
    QVERIFY(model.cell(Sat, 6).outOfScope.first().sharesDatesWithActiveScope);
    QVERIFY(!model.cell(Sat, 1).outOfScope.first().sharesDatesWithActiveScope);
    QVERIFY(!model.cell(Sat, 3).outOfScope.first().sharesDatesWithActiveScope);

    QVERIFY(scopeOf(0x01, false, 0x007).sharesDatesWith(scopeOf(0x03)));
    QVERIFY(!scopeOf(0x01, false, 0x007).sharesDatesWith(scopeOf(0x01, false, 0x008)));
    QVERIFY(scopeOf(0x00, true).sharesDatesWith(scopeOf(0x10)));
    QVERIFY(scopeOf(0x00, true).sharesDatesWith(scopeOf(0x00, true)));

    // An edit in "1st" does not touch the lines of the scopes around it
    model.setActiveScope(1);
    BGridSelection all;
    all.selectAll();
    model.setSelection(all);
    const BGridChange change = model.erase();
    QVERIFY(change.ok);
    QCOMPARE(change.removedRuns, QList<int>({0, 6}));
    QCOMPARE(texts(model), QStringList({
        "Full 1st-2nd sat at 2:00",
        "Full jan-mar 1st sat at 3:00",
        "Full last sat at 4:00",
        "Full sat at 5:00",
        "Full 4th sat at 6:00",
    }));
    // The emptied scope stays where the user is working
    QCOMPARE(model.activeScope(), scopeOf(0x01));
    QCOMPARE(model.scopes()[1], (BGridScopeInfo{scopeOf(0x01), 0}));
}

void BScheduleGridModelTest::addAndRemoveScope()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({"Incremental mon-fri at 21:00", "Full 2nd sat at 21:00"}));
    QCOMPARE(model.scopes().size(), 2);
    model.setActiveScope(1);

    QSignalSpy scopes(&model, &BScheduleGridModel::scopesChanged);
    QSignalSpy active(&model, &BScheduleGridModel::activeScopeChanged);

    // "1st" sorts in front of "2nd": the active scope keeps its value, its index moves
    QCOMPARE(model.addScope(scopeOf(0x01)), 1);
    QCOMPARE(scopes.count(), 1);
    QCOMPARE(model.scopes().size(), 3);
    QCOMPARE(model.scopes()[1], (BGridScopeInfo{scopeOf(0x01), 0}));
    QCOMPARE(model.activeScope(), scopeOf(0x02));
    QCOMPARE(model.activeScopeIndex(), 2);
    QCOMPARE(active.count(), 1);
    QCOMPARE(model.cell(Sat, 21).inScope.first().scopeIndex, 2);

    // Adding what is there: its index, no change
    QCOMPARE(model.addScope(scopeOf(0x02)), 2);
    QCOMPARE(model.addScope(BGridScope::everyWeek()), 0);
    QCOMPARE(scopes.count(), 1);

    // A scope without a month or without a week cannot be written
    QCOMPARE(model.addScope(scopeOf(0x00)), -1);
    QCOMPARE(model.addScope(scopeOf(0x01, false, 0)), -1);
    QCOMPARE(model.addScope(scopeOf(0x00, true)), 3);

    // Only an empty scope can be removed, and never "every week"
    QVERIFY(!model.removeScope(0));
    QVERIFY(!model.removeScope(2));
    QVERIFY(!model.removeScope(9));
    model.setActiveScope(3);
    QVERIFY(model.removeScope(3));
    QCOMPARE(model.activeScopeIndex(), 0);
    QVERIFY(model.removeScope(1));
    QCOMPARE(model.scopes().size(), 2);

    // A new schedule forgets added scopes
    model.addScope(scopeOf(0x10));
    model.setSchedule(scheduleOf({"Incremental mon-fri at 21:00"}));
    QCOMPARE(model.scopes().size(), 1);
}

void BScheduleGridModelTest::calendarRulesAndNotUnderstood()
{
    // Bareos documentation: "First" (on 1, on 2-31) and the modulo examples
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Level=Full on 1 at 2:05",
        "Level=Incremental on 2-31 at 2:05",
        "Level=Full 1/2 at 23:10",
        "Level=Full w01/w02 at 23:10",
        "Level=Full w03/w05 sat at 23:10",
        "Bogus sat at 5:00",
        "Incremental mon-fri at 21:00",
        "Full 1st sat 15 at 22:00",
    }));

    // Only the one weekday line is in the grid, and only its scope exists
    QCOMPARE(model.scopes().size(), 1);
    QCOMPARE(model.scopes().first().runCount, 1);
    QCOMPARE(occupiedCells(model), 5);
    for (int row = 0; row < 7; ++row) {
        for (int column = 0; column < 24; ++column) {
            const BGridCell cell = model.cell(row, column);
            QVERIFY(cell.outOfScope.isEmpty());
            QCOMPARE(cell.inScope.size(), (row <= Fri && column == 21) ? 1 : 0);
        }
    }

    const QList<BGridCalendarRule> &rules = model.calendarRules();
    QCOMPARE(rules.size(), 6);
    QCOMPARE(rules[0].runIndex, 0);
    QCOMPARE(rules[0].text, QString("Level=Full on 1 at 2:05"));
    QVERIFY(rules[0].byDayOfMonth && !rules[0].byWeekOfYear);
    QCOMPARE(rules[0].spec.mask.mday, quint32(1));
    QCOMPARE(*rules[0].spec.overrides.level, QString("Full"));
    QCOMPARE(rules[0].spec.minute, 5);
    QCOMPARE(rules[0].spec.hours(), QList<int>({2}));
    QVERIFY(!rules[0].withoutTime);
    QCOMPARE(rules[1].runIndex, 1);
    QVERIFY(rules[1].byDayOfMonth);
    QCOMPARE(rules[2].runIndex, 2);     // day modulo
    QVERIFY(rules[2].byDayOfMonth && !rules[2].byWeekOfYear);
    QCOMPARE(rules[3].runIndex, 3);     // week modulo
    QVERIFY(!rules[3].byDayOfMonth && rules[3].byWeekOfYear);
    QCOMPARE(rules[4].runIndex, 4);     // weeks of the year with a weekday: still no grid line
    QVERIFY(rules[4].byWeekOfYear);
    QCOMPARE(rules[4].spec.mask.wday, quint8(1u << 6));
    QCOMPARE(rules[5].runIndex, 7);     // a day of the month beside week and weekday
    QVERIFY(rules[5].byDayOfMonth);

    QCOMPARE(model.notUnderstood().size(), 1);
    QCOMPARE(model.notUnderstood().first().runIndex, 5);
    QCOMPARE(model.notUnderstood().first().text, QString("Bogus sat at 5:00"));
    QCOMPARE(model.notUnderstood().first().reason, BRunParser::parse("Bogus sat at 5:00").parseError);
    QVERIFY(!model.notUnderstood().first().reason.isEmpty());

    QVERIFY(model.cellsOfRun(0).isEmpty());
    QVERIFY(model.cellsOfRun(5).isEmpty());
    QCOMPARE(model.cellsOfRun(6).count(), 5);
}

void BScheduleGridModelTest::brushesAndCellsOfRun()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Full 1st sat at 21:00",
        "Incremental mon-fri at 21:00",
        "Level=Incremental tue at 9:00",
        "Level=Incremental Pool=Daily tue at 9:00",
        "Level=Incremental tue at 9:30",
        "Full on 1 at 3:33",
    }));
    // Distinct by overrides and minute, in the order of the lines; the
    // calendar rule has no brush
    QCOMPARE(model.brushes(), QList<BGridBrush>({
        brush("Full"), brush("Incremental"), brush("Incremental", 0, "Daily"), brush("Incremental", 30),
    }));

    int scope = -2;
    QCOMPARE(model.cellsOfRun(0, &scope), BGridCellSet::rect(Sat, 21, Sat, 21));
    QCOMPARE(scope, 1);
    QCOMPARE(model.cellsOfRun(1, &scope), BGridCellSet::rect(Mon, 21, Fri, 21));
    QCOMPARE(scope, 0);
    QVERIFY(model.cellsOfRun(5, &scope).isEmpty());
    QCOMPARE(scope, -1);
    QVERIFY(model.cellsOfRun(17).isEmpty());
}

void BScheduleGridModelTest::loadHook()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({"Incremental mon-fri at 21:00"}));
    QVERIFY(!model.cell(Mon, 21).expectedConcurrentRuns);

    QList<BGridPos> changed;
    connect(&model, &BScheduleGridModel::cellsChanged, this,
            [&changed](const QList<BGridPos> &cells) { changed += cells; });

    model.setExpectedLoad(1, 21, 2.5);      // weekday of Bareos: Monday
    QCOMPARE(*model.cell(Mon, 21).expectedConcurrentRuns, 2.5);
    QVERIFY(!model.cell(Tue, 21).expectedConcurrentRuns);
    QCOMPARE(changed, QList<BGridPos>({BGridPos{Mon, 21}}));

    // An edit leaves it alone
    select(model, Mon, 21);
    QVERIFY(model.erase().ok);
    QCOMPARE(*model.cell(Mon, 21).expectedConcurrentRuns, 2.5);

    changed.clear();
    model.clearExpectedLoad();
    QVERIFY(!model.cell(Mon, 21).expectedConcurrentRuns);
    QCOMPARE(changed, QList<BGridPos>({BGridPos{Mon, 21}}));
}

// ============================================================================
// Edits
// ============================================================================

void BScheduleGridModelTest::eraseOneWeekday()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Incremental mon-fri at 21:00",
    }));
    select(model, Wed, 21);
    const BGridChange change = model.erase();
    QVERIFY2(change.ok, qPrintable(change.error));
    QCOMPARE(texts(model), QStringList({
        "Level=Incremental mon,tue,thu,fri at 21:00",
    }));
    QCOMPARE(states(model), QString("c"));
    QCOMPARE(change.changedRuns, QList<int>({0}));
    QVERIFY(change.addedRuns.isEmpty() && change.removedRuns.isEmpty());
    QCOMPARE(change.before.runs.size(), 1);
    QCOMPARE(change.before.runs.first().original, QString("Incremental mon-fri at 21:00"));
    QVERIFY(!change.before.runs.first().dirty);
    QVERIFY(change.after.runs.first().dirty);
    QVERIFY(model.isModified());
    QVERIFY(model.cell(Wed, 21).isEmpty());
    QCOMPARE(occupiedCells(model), 4);
    // The changed line keeps the reported text beside the new one
    QCOMPARE(model.runLines().first().originalText, QString("Incremental mon-fri at 21:00"));
    QCOMPARE(model.schedule().runs.first().original, QString("Incremental mon-fri at 21:00"));
}

void BScheduleGridModelTest::eraseFromTheMiddleSplits()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Full sun at 3:00",
        "Incremental mon-fri at 21:00 at 22:00 at 23:00",
        "Differential sat at 3:00",
    }));
    select(model, Wed, 22);
    const BGridChange change = model.erase();
    QVERIFY2(change.ok, qPrintable(change.error));
    // What is left is not weekdays times hours any more: two lines, the
    // larger in the place of the old one, the other behind it
    QCOMPARE(texts(model), QStringList({
        "Full sun at 3:00",
        "Level=Incremental mon,tue,thu,fri at 21:00 at 22:00 at 23:00",
        "Level=Incremental wed at 21:00 at 23:00",
        "Differential sat at 3:00",
    }));
    QCOMPARE(states(model), QString("ucnu"));
    QCOMPARE(change.changedRuns, QList<int>({1}));
    QCOMPARE(change.addedRuns, QList<int>({2}));
    QVERIFY(change.removedRuns.isEmpty());
    QCOMPARE(occupiedCells(model), 16);
    QVERIFY(model.cell(Wed, 22).isEmpty());
    QCOMPARE(model.cell(Wed, 21).inScope.first().runIndex, 2);
    QCOMPARE(model.cell(Sat, 3).inScope.first().runIndex, 3);

    // A whole column out of the middle: still one line
    select(model, Mon, 22, Sun, 22);
    QVERIFY(model.erase().ok);
    QCOMPARE(texts(model), QStringList({
        "Full sun at 3:00",
        "Level=Incremental mon-fri at 21:00 at 23:00",
        "Differential sat at 3:00",
    }));
    QCOMPARE(states(model), QString("ucu"));
}

void BScheduleGridModelTest::paintNewProduct()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({}));
    select(model, Mon, 8, Wed, 9);
    const BGridChange change = model.applyBrush(brush("Full", 30));
    QVERIFY2(change.ok, qPrintable(change.error));
    QCOMPARE(texts(model), QStringList({
        "Level=Full mon-wed at 08:30 at 09:30",
    }));
    QCOMPARE(states(model), QString("n"));
    QCOMPARE(change.addedRuns, QList<int>({0}));
    QCOMPARE(occupiedCells(model), 6);
    QCOMPARE(runsText(model.cell(Tue, 9).inScope), QString("Full:30"));
    QCOMPARE(model.scopes().first().runCount, 1);

    // A brush without a level and with overrides; cells that are no product
    // become the smallest number of lines
    BGridSelection selection;
    selection.selectCell(Sat, 1);
    selection.toggleCell(Sat, 2);
    selection.toggleCell(Sun, 2);
    model.setSelection(selection);
    BGridBrush b;
    b.overrides.pool = QStringLiteral("Week End");
    b.overrides.priority = 5;
    b.minute = 0;
    QVERIFY(model.applyBrush(b).ok);
    QCOMPARE(texts(model), QStringList({
        "Level=Full mon-wed at 08:30 at 09:30",
        "Pool=\"Week End\" Priority=5 sat at 01:00 at 02:00",
        "Pool=\"Week End\" Priority=5 sun at 02:00",
    }));
    QVERIFY(!model.cell(Sat, 1).inScope.first().level());
}

void BScheduleGridModelTest::paintWholeRowIsHourly()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({}));

    BGridSelection selection;
    selection.selectRow(Sat);
    model.setSelection(selection);
    QVERIFY(model.applyBrush(brush("Incremental", 15)).ok);
    QCOMPARE(texts(model), QStringList({
        "Level=Incremental sat hourly at 0:15",
    }));

    // The whole grid with another brush replaces it
    selection.selectAll();
    model.setSelection(selection);
    QVERIFY(model.applyBrush(brush("Full")).ok);
    QCOMPARE(texts(model), QStringList({
        "Level=Full hourly at 0:00",
    }));
    QCOMPARE(states(model), QString("n"));
    QCOMPARE(occupiedCells(model), 168);
}

void BScheduleGridModelTest::paintOverAnotherBrush()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Incremental mon-fri at 21:00",
    }));
    select(model, Wed, 21, Thu, 21);
    const BGridChange change = model.applyBrush(brush("Full"));
    QVERIFY2(change.ok, qPrintable(change.error));
    // The cells end up with the painted brush only
    QCOMPARE(texts(model), QStringList({
        "Level=Incremental mon,tue,fri at 21:00",
        "Level=Full wed,thu at 21:00",
    }));
    QCOMPARE(states(model), QString("cn"));
    QCOMPARE(change.changedRuns, QList<int>({0}));
    QCOMPARE(change.addedRuns, QList<int>({1}));
    QCOMPARE(runsText(model.cell(Wed, 21).inScope), QString("Full:00"));
    QCOMPARE(runsText(model.cell(Tue, 21).inScope), QString("Incremental:00"));

    // The same level at another minute is another brush
    select(model, Wed, 21);
    QVERIFY(model.applyBrush(brush("Full", 45)).ok);
    QCOMPARE(texts(model), QStringList({
        "Level=Incremental mon,tue,fri at 21:00",
        "Level=Full thu at 21:00",
        "Level=Full wed at 21:45",
    }));

    // Painting over a whole line removes it
    select(model, Mon, 21, Fri, 21);
    QVERIFY(model.applyBrush(brush("Differential")).ok);
    QCOMPARE(texts(model), QStringList({
        "Level=Differential mon-fri at 21:00",
    }));
    QCOMPARE(states(model), QString("rn"));
}

void BScheduleGridModelTest::paintAddsBesideAnotherBrush()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Incremental mon-fri at 21:00",
    }));
    select(model, Wed, 21, Thu, 21);
    const BGridChange change = model.applyBrush(brush("Full", 30), BScheduleGridModel::AddRun);
    QVERIFY2(change.ok, qPrintable(change.error));
    QCOMPARE(texts(model), QStringList({
        "Incremental mon-fri at 21:00",
        "Level=Full wed,thu at 21:30",
    }));
    QCOMPARE(states(model), QString("un"));
    QCOMPARE(runsText(model.cell(Wed, 21).inScope), QString("Incremental:00 Full:30"));
    QCOMPARE(runsText(model.cell(Tue, 21).inScope), QString("Incremental:00"));

    // Erasing takes every run of the cell
    select(model, Wed, 21);
    QVERIFY(model.erase().ok);
    QCOMPARE(texts(model), QStringList({
        "Level=Incremental mon,tue,thu,fri at 21:00",
        "Level=Full thu at 21:30",
    }));
}

void BScheduleGridModelTest::paintWhatIsThereIsNoChange()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Incremental mon-fri at 21:00",
    }));
    QSignalSpy lines(&model, &BScheduleGridModel::runLinesChanged);

    select(model, Tue, 21, Thu, 21);
    BGridChange change = model.applyBrush(brush("Incremental"));
    QVERIFY(change.ok);
    QVERIFY(change.isNoop());
    QCOMPARE(texts(model), QStringList({"Incremental mon-fri at 21:00"}));
    QVERIFY(!model.isModified());

    // Erasing empty cells, and editing with nothing selected
    select(model, Sat, 0, Sun, 5);
    change = model.erase();
    QVERIFY(change.ok && change.isNoop());
    model.setSelection(BGridSelection());
    change = model.applyBrush(brush("Full"));
    QVERIFY(change.ok && change.isNoop());
    change = model.erase();
    QVERIFY(change.ok && change.isNoop());

    QCOMPARE(lines.count(), 0);
    QCOMPARE(states(model), QString("u"));
}

void BScheduleGridModelTest::eraseWholeLine()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Full sat at 21:00",
        "Incremental mon-fri at 21:00",
    }));
    select(model, Sat, 21);
    const BGridChange change = model.erase();
    QVERIFY2(change.ok, qPrintable(change.error));
    QCOMPARE(texts(model), QStringList({
        "Incremental mon-fri at 21:00",
    }));
    QCOMPARE(change.removedRuns, QList<int>({0}));
    QVERIFY(change.changedRuns.isEmpty() && change.addedRuns.isEmpty());
    QCOMPARE(states(model), QString("ru"));
    QCOMPARE(model.cell(Mon, 21).inScope.first().runIndex, 0);
    QVERIFY(model.isModified());

    // More than the line has: it goes just the same, and the rest with it
    BGridSelection all;
    all.selectAll();
    model.setSelection(all);
    QVERIFY(model.erase().ok);
    QVERIFY(texts(model).isEmpty());
    QCOMPARE(states(model), QString("rr"));
    QCOMPARE(model.schedule().runs.size(), 0);
}

void BScheduleGridModelTest::editInOneScopeLeavesTheOthers()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Full 1st sat at 21:00",
        "Differential   2nd-5th  saturday at 9:00pm",
        "incremental monday-friday at 21:00",
    }));
    QCOMPARE(model.scopes().size(), 3);

    // First week: the Full moves from Saturday to Friday and gets a pool
    model.setActiveScope(1);
    select(model, Sat, 21);
    QVERIFY(model.erase().ok);
    select(model, Fri, 21);
    QVERIFY(model.applyBrush(brush("Full", 0, "Monthly")).ok);
    QCOMPARE(texts(model), QStringList({
        "Differential   2nd-5th  saturday at 9:00pm",
        "incremental monday-friday at 21:00",
        "Level=Full Pool=Monthly 1st fri at 21:00",
    }));
    QCOMPARE(states(model), QString("ruun"));
    // The incremental of every week is on that Friday too; it is of another
    // scope and stays, also when the whole grid is painted
    QCOMPARE(runsText(model.cell(Fri, 21).inScope), QString("Full:00"));
    QCOMPARE(runsText(model.cell(Fri, 21).outOfScope), QString("Incremental:00"));

    BGridSelection all;
    all.selectAll();
    model.setSelection(all);
    QVERIFY(model.applyBrush(brush("Full", 5, "Monthly")).ok);
    QCOMPARE(texts(model), QStringList({
        "Differential   2nd-5th  saturday at 9:00pm",
        "incremental monday-friday at 21:00",
        "Level=Full Pool=Monthly 1st hourly at 0:05",
    }));
    QVERIFY(model.erase().ok);
    QCOMPARE(texts(model), QStringList({
        "Differential   2nd-5th  saturday at 9:00pm",
        "incremental monday-friday at 21:00",
    }));

    // The other lines are the reported ones, byte for byte, in the schedule too
    const BSchedule schedule = model.schedule();
    QCOMPARE(schedule.runs.size(), 2);
    QCOMPARE(schedule.runs[0].original, QString("Differential   2nd-5th  saturday at 9:00pm"));
    QVERIFY(!schedule.runs[0].dirty);
    QCOMPARE(BRunWriter::lineText(schedule.runs[1]), QString("incremental monday-friday at 21:00"));
}

void BScheduleGridModelTest::paintInAddedScopes()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({"Incremental mon-fri at 21:00"}));

    // The last week of the month
    model.setActiveScope(model.addScope(scopeOf(0x00, true)));
    select(model, Fri, 21);
    QVERIFY(model.applyBrush(brush("Full")).ok);
    // January to March, first week
    model.setActiveScope(model.addScope(scopeOf(0x01, false, 0x007)));
    select(model, Sat, 21);
    QVERIFY(model.applyBrush(brush("Full")).ok);
    // First and third week, and "last"
    model.setActiveScope(model.addScope(scopeOf(0x05, true)));
    select(model, Sun, 4, Sun, 5);
    QVERIFY(model.applyBrush(brush("VirtualFull", 10)).ok);

    QCOMPARE(texts(model), QStringList({
        "Incremental mon-fri at 21:00",
        "Level=Full last fri at 21:00",
        "Level=Full jan-mar 1st sat at 21:00",
        "Level=VirtualFull 1st,3rd last sun at 04:10 at 05:10",
    }));
    QCOMPARE(model.scopes().size(), 4);
    for (const BGridScopeInfo &info : model.scopes())
        QCOMPARE(info.runCount, 1);
    // The same cell in the same brush in two scopes: two lines, never one
    model.setActiveScope(0);
    select(model, Fri, 21);
    QVERIFY(model.applyBrush(brush("Full")).ok);
    QCOMPARE(texts(model), QStringList({
        "Level=Incremental mon-thu at 21:00",
        "Level=Full last fri at 21:00",
        "Level=Full jan-mar 1st sat at 21:00",
        "Level=VirtualFull 1st,3rd last sun at 04:10 at 05:10",
        "Level=Full fri at 21:00",
    }));
}

void BScheduleGridModelTest::untouchedLineIsNotRestructured()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Incremental mon-thu at 21:00",
    }));
    // Friday in the same brush would fit the line; the line is the user's
    select(model, Fri, 21);
    const BGridChange change = model.applyBrush(brush("Incremental"));
    QVERIFY2(change.ok, qPrintable(change.error));
    QCOMPARE(texts(model), QStringList({
        "Incremental mon-thu at 21:00",
        "Level=Incremental fri at 21:00",
    }));
    QCOMPARE(states(model), QString("un"));

    // The new line grows; the loaded one still does not
    select(model, Sat, 21);
    QVERIFY(model.applyBrush(brush("Incremental")).ok);
    QCOMPARE(texts(model), QStringList({
        "Incremental mon-thu at 21:00",
        "Level=Incremental fri,sat at 21:00",
    }));

    // Painting over cells the loaded line has already: only what is missing is added
    select(model, Mon, 21, Sun, 21);
    QVERIFY(model.applyBrush(brush("Incremental")).ok);
    QCOMPARE(texts(model), QStringList({
        "Incremental mon-thu at 21:00",
        "Level=Incremental sun,fri,sat at 21:00",
    }));
    for (int row = 0; row < 7; ++row)
        QCOMPARE(model.cell(row, 21).inScope.size(), 1);
}

void BScheduleGridModelTest::generatedLinesMerge()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({}));

    select(model, Mon, 21);
    QVERIFY(model.applyBrush(brush("Full")).ok);
    QCOMPARE(texts(model), QStringList({"Level=Full mon at 21:00"}));

    select(model, Tue, 21);
    BGridChange change = model.applyBrush(brush("Full"));
    QVERIFY(change.ok);
    QCOMPARE(texts(model), QStringList({"Level=Full mon,tue at 21:00"}));
    QCOMPARE(change.changedRuns, QList<int>({0}));
    QVERIFY(change.addedRuns.isEmpty());

    select(model, Wed, 21);
    QVERIFY(model.applyBrush(brush("Full")).ok);
    QCOMPARE(texts(model), QStringList({"Level=Full mon-wed at 21:00"}));

    // The same weekdays one hour later: the hours merge
    select(model, Mon, 22, Wed, 22);
    QVERIFY(model.applyBrush(brush("Full")).ok);
    QCOMPARE(texts(model), QStringList({"Level=Full mon-wed at 21:00 at 22:00"}));

    // Another brush or another scope never merges
    select(model, Thu, 21, Thu, 22);
    QVERIFY(model.applyBrush(brush("Full", 30)).ok);
    model.setActiveScope(model.addScope(scopeOf(0x01)));
    QVERIFY(model.applyBrush(brush("Full")).ok);
    QCOMPARE(texts(model), QStringList({
        "Level=Full mon-wed at 21:00 at 22:00",
        "Level=Full thu at 21:30 at 22:30",
        "Level=Full 1st thu at 21:00 at 22:00",
    }));

    // A cell that does not make a product stays a line of its own
    model.setActiveScope(0);
    select(model, Thu, 23);
    QVERIFY(model.applyBrush(brush("Full")).ok);
    QCOMPARE(texts(model), QStringList({
        "Level=Full mon-wed at 21:00 at 22:00",
        "Level=Full thu at 21:30 at 22:30",
        "Level=Full 1st thu at 21:00 at 22:00",
        "Level=Full thu at 23:00",
    }));

    // Erasing joins what fits again: without 22:00 the lines of 21:00 and
    // of Thursday 23:00 still do not fit, without Thursday 23:00 nothing is left of that line
    select(model, Mon, 22, Wed, 22);
    QVERIFY(model.erase().ok);
    QCOMPARE(texts(model), QStringList({
        "Level=Full mon-wed at 21:00",
        "Level=Full thu at 21:30 at 22:30",
        "Level=Full 1st thu at 21:00 at 22:00",
        "Level=Full thu at 23:00",
    }));
}

void BScheduleGridModelTest::backToTheLoadedLine()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Incremental monday-friday at 9:00pm",
    }));
    QSignalSpy modified(&model, &BScheduleGridModel::modifiedChanged);

    select(model, Wed, 21);
    QVERIFY(model.erase().ok);
    QCOMPARE(texts(model), QStringList({"Level=Incremental mon,tue,thu,fri at 21:00"}));
    QVERIFY(model.isModified());

    // Painting it back by hand: the line is the reported one again
    const BGridChange change = model.applyBrush(brush("Incremental"));
    QVERIFY(change.ok);
    QCOMPARE(change.changedRuns, QList<int>({0}));
    QCOMPARE(texts(model), QStringList({"Incremental monday-friday at 9:00pm"}));
    QCOMPARE(states(model), QString("u"));
    QVERIFY(!model.isModified());
    QVERIFY(!model.schedule().runs.first().dirty);
    QCOMPARE(modified.count(), 2);
    QCOMPARE(modified.at(0).at(0).toBool(), true);
    QCOMPARE(modified.at(1).at(0).toBool(), false);
}

void BScheduleGridModelTest::moveRunToAnotherMinute()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Incremental mon-fri at 21:00 at 23:00",
        "Full 1st sat at 21:00",
        "Full on 1 at 2:00",
        "Bogus at 2:00",
    }));

    BGridChange change = model.moveRun(0, 30);
    QVERIFY2(change.ok, qPrintable(change.error));
    QCOMPARE(change.changedRuns, QList<int>({0}));
    QCOMPARE(texts(model), QStringList({
        "Level=Incremental mon-fri at 21:30 at 23:30",
        "Full 1st sat at 21:00",
        "Full on 1 at 2:00",
        "Bogus at 2:00",
    }));
    QCOMPARE(runsText(model.cell(Tue, 23).inScope), QString("Incremental:30"));

    // A line of a scope that is not active
    change = model.moveRun(1, 5);
    QVERIFY(change.ok);
    QCOMPARE(texts(model)[1], QString("Level=Full 1st sat at 21:05"));

    // The same minute: no change. Back to the reported minute: the reported line.
    change = model.moveRun(1, 5);
    QVERIFY(change.ok && change.isNoop());
    change = model.moveRun(1, 0);
    QVERIFY(change.ok);
    QCOMPARE(texts(model)[1], QString("Full 1st sat at 21:00"));

    // Not a line of the grid, no such line, no such minute
    const QStringList before = texts(model);
    for (const BGridChange &bad : {model.moveRun(2, 30), model.moveRun(3, 30), model.moveRun(4, 30),
                                   model.moveRun(-1, 30), model.moveRun(0, 60), model.moveRun(0, -1)}) {
        QVERIFY(!bad.ok);
        QVERIFY(!bad.error.isEmpty());
    }
    QCOMPARE(texts(model), before);
}

void BScheduleGridModelTest::failedWriteBackChangesNothing()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Incremental mon-fri at 21:00",
        "Full 1st sat at 21:00",
    }));
    const QStringList before = texts(model);
    select(model, Wed, 21, Sat, 22);

    QSignalSpy lines(&model, &BScheduleGridModel::runLinesChanged);
    QSignalSpy cells(&model, &BScheduleGridModel::cellsChanged);
    QSignalSpy scopes(&model, &BScheduleGridModel::scopesChanged);
    QSignalSpy modified(&model, &BScheduleGridModel::modifiedChanged);

    // The writer writes "Level=full"; read back it is "Full": not what was meant
    BGridChange change = model.applyBrush(brush("full"));
    QVERIFY(!change.ok);
    QVERIFY(!change.error.isEmpty());
    QVERIFY(change.isNoop());
    QCOMPARE(change.after.runs.size(), change.before.runs.size());

    // A level Bareos does not have: the written line does not read back
    change = model.applyBrush(brush("Sometimes"));
    QVERIFY(!change.ok);
    QVERIFY(change.error.contains("Sometimes"));

    // A minute no hour has
    change = model.applyBrush(brush("Full", 60));
    QVERIFY(!change.ok);
    QVERIFY(!change.error.isEmpty());

    // An empty pool name
    BGridBrush empty = brush("Full");
    empty.overrides.pool = QString();
    change = model.applyBrush(empty);
    QVERIFY(!change.ok);

    // Each of these would first have cut Wednesday to Friday out of the
    // incremental line; nothing of that happened
    QCOMPARE(texts(model), before);
    QCOMPARE(states(model), QString("uu"));
    QVERIFY(!model.isModified());
    QCOMPARE(occupiedCells(model), 5);
    QCOMPARE(lines.count(), 0);
    QCOMPARE(cells.count(), 0);
    QCOMPARE(scopes.count(), 0);
    QCOMPARE(modified.count(), 0);
    QCOMPARE(model.schedule().runs[0].original, QString("Incremental mon-fri at 21:00"));
    QVERIFY(!model.schedule().runs[0].dirty);
}

void BScheduleGridModelTest::calendarRulesAndNotUnderstoodSurviveEdits()
{
    const QStringList loaded = {
        "Level=Full on 1 at 2:05",
        "Incremental mon-fri at 21:00",
        "Level=Full w01/w02 sat at 21:00",
        "Bogus sat at 21:00",
        "Level=Full 1/2 at 21:00",
    };
    BScheduleGridModel model;
    model.setSchedule(scheduleOf(loaded));
    QSignalSpy rules(&model, &BScheduleGridModel::calendarRulesChanged);
    QSignalSpy notUnderstood(&model, &BScheduleGridModel::notUnderstoodChanged);

    BGridSelection all;
    all.selectAll();
    model.setSelection(all);

    // Paint everything: the grid line is replaced, the others stay
    QVERIFY(model.applyBrush(brush("Differential", 7)).ok);
    QCOMPARE(texts(model), QStringList({
        "Level=Full on 1 at 2:05",
        "Level=Full w01/w02 sat at 21:00",
        "Bogus sat at 21:00",
        "Level=Full 1/2 at 21:00",
        "Level=Differential hourly at 0:07",
    }));
    QCOMPARE(states(model), QString("uruuun"));
    // Their positions moved, so the lists were announced
    QCOMPARE(rules.count(), 1);
    QCOMPARE(notUnderstood.count(), 1);
    QCOMPARE(model.calendarRules().size(), 3);
    QCOMPARE(model.calendarRules()[1].runIndex, 1);
    QCOMPARE(model.notUnderstood().first().runIndex, 2);

    // Erase everything in every scope
    for (int scope = 0; scope < model.scopes().size(); ++scope) {
        model.setActiveScope(scope);
        QVERIFY(model.erase().ok);
    }
    QCOMPARE(texts(model), QStringList({
        "Level=Full on 1 at 2:05",
        "Level=Full w01/w02 sat at 21:00",
        "Bogus sat at 21:00",
        "Level=Full 1/2 at 21:00",
    }));
    const BSchedule schedule = model.schedule();
    for (const BRunLine &line : schedule.runs) {
        QVERIFY(!line.dirty);
        QVERIFY(loaded.contains(line.original));
    }
    QVERIFY(!schedule.runs[2].understood());
    QCOMPARE(occupiedCells(model), 0);
}

void BScheduleGridModelTest::notUnderstoodBlocksWritingOnly()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Incremental mon-fri at 21:00",
        "Full 1st sat at 25:00",
    }));
    QString reason;
    QVERIFY(!model.canBeWritten(&reason));
    QVERIFY(reason.contains("Full 1st sat at 25:00"));
    QVERIFY(reason.contains(BRunParser::parse("Full 1st sat at 25:00").parseError));

    QStringList written = {"x"};
    QString error;
    QVERIFY(!model.runTexts(&written, &error));
    QVERIFY(written.isEmpty());
    QCOMPARE(error, reason);

    // Edits of the other lines still work in memory
    select(model, Fri, 21);
    QVERIFY(model.erase().ok);
    QCOMPARE(texts(model), QStringList({
        "Level=Incremental mon-thu at 21:00",
        "Full 1st sat at 25:00",
    }));
    QVERIFY(model.isModified());
    // and the state stays what it was
    QVERIFY(!model.canBeWritten());
    QVERIFY(!model.runTexts(&written));

    // Without such a line the texts are what would be written
    model.setSchedule(scheduleOf({"Incremental mon-fri at 21:00", "Full 1 at 02:05"}));
    QVERIFY(model.canBeWritten(&reason));
    QVERIFY(reason.isEmpty());
    QVERIFY(model.runTexts(&written, &error));
    QCOMPARE(written, QStringList({"Incremental mon-fri at 21:00", "Level=Full on 1 at 02:05"}));
}

void BScheduleGridModelTest::restoreUndoesAndRedoes()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Incremental mon-fri at 21:00 at 22:00",
        "Full sat at 21:00",
    }));
    const QStringList loaded = texts(model);

    select(model, Wed, 22, Sat, 22);
    const BGridChange first = model.applyBrush(brush("Differential"));
    QVERIFY(first.ok);
    const QStringList afterFirst = texts(model);
    // Two products either way; with the same number the weekdays stay together
    QCOMPARE(afterFirst, QStringList({
        "Level=Incremental mon,tue at 21:00 at 22:00",
        "Level=Incremental wed-fri at 21:00",
        "Full sat at 21:00",
        "Level=Differential wed-sat at 22:00",
    }));
    select(model, Sat, 21);
    const BGridChange second = model.erase();
    QVERIFY(second.ok);
    const QStringList afterSecond = texts(model);
    const QString statesAfterSecond = states(model);
    QCOMPARE(statesAfterSecond, QString("cnrn"));

    // Undo both
    QVERIFY(model.restore(second.before));
    QCOMPARE(texts(model), afterFirst);
    QCOMPARE(states(model), QString("cnun"));
    QVERIFY(model.restore(first.before));
    QCOMPARE(texts(model), loaded);
    QCOMPARE(states(model), QString("uu"));
    QVERIFY(!model.isModified());
    QCOMPARE(runsText(model.cell(Wed, 22).inScope), QString("Incremental:00"));

    // Redo both
    QVERIFY(model.restore(first.after));
    QVERIFY(model.restore(second.after));
    QCOMPARE(texts(model), afterSecond);
    QCOMPARE(states(model), statesAfterSecond);
    QVERIFY(model.isModified());
    QCOMPARE(runsText(model.cell(Wed, 22).inScope), QString("Differential:00"));

    // A snapshot that does not belong to this schedule
    BGridSnapshot bad = first.before;
    bad.origins[0] = 7;
    QVERIFY(!model.restore(bad));
    bad = first.before;
    bad.origins.removeLast();
    QVERIFY(!model.restore(bad));
    bad = first.before;
    bad.splitFrom[0] = 2;
    QVERIFY(!model.restore(bad));
    QCOMPARE(texts(model), afterSecond);
}

void BScheduleGridModelTest::runLinesWithStates()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({
        "Full 1 at 02:05",                  // as the Director prints it: read, but respelled
        "Incremental mon-fri at 21:00",
        "Full sat at 21:00",
        "Bogus",
        "Differential sun at 21:00",
    }));
    select(model, Sat, 21);
    QVERIFY(model.erase().ok);
    select(model, Fri, 21);
    QVERIFY(model.applyBrush(brush("Full")).ok);

    const QList<BGridRunLineText> lines = model.runLines();
    QCOMPARE(lines.size(), 6);

    QCOMPARE(lines[0].state, BGridRunLineText::Untouched);
    QVERIFY(lines[0].respelled);
    QCOMPARE(lines[0].text, QString("Level=Full on 1 at 02:05"));
    QCOMPARE(lines[0].originalText, QString("Full 1 at 02:05"));
    QCOMPARE(lines[0].runIndex, 0);
    QCOMPARE(lines[0].originalIndex, 0);

    QCOMPARE(lines[1].state, BGridRunLineText::Changed);
    QCOMPARE(lines[1].text, QString("Level=Incremental mon-thu at 21:00"));
    QCOMPARE(lines[1].originalText, QString("Incremental mon-fri at 21:00"));
    QCOMPARE(lines[1].runIndex, 1);
    QCOMPARE(lines[1].originalIndex, 1);
    QVERIFY(!lines[1].respelled);

    // The removed line stands where it stood
    QCOMPARE(lines[2].state, BGridRunLineText::Removed);
    QCOMPARE(lines[2].text, QString("Full sat at 21:00"));
    QCOMPARE(lines[2].runIndex, -1);
    QCOMPARE(lines[2].originalIndex, 2);

    QCOMPARE(lines[3].state, BGridRunLineText::Untouched);
    QVERIFY(!lines[3].understood);
    QCOMPARE(lines[3].text, QString("Bogus"));
    QVERIFY(!lines[3].error.isEmpty());
    QCOMPARE(lines[3].runIndex, 2);

    QCOMPARE(lines[4].state, BGridRunLineText::Untouched);
    QCOMPARE(lines[4].text, QString("Differential sun at 21:00"));
    QCOMPARE(lines[4].runIndex, 3);
    QCOMPARE(lines[4].originalIndex, 4);

    QCOMPARE(lines[5].state, BGridRunLineText::New);
    QCOMPARE(lines[5].text, QString("Level=Full fri at 21:00"));
    QVERIFY(lines[5].originalText.isEmpty());
    QCOMPARE(lines[5].runIndex, 4);
    QCOMPARE(lines[5].originalIndex, -1);
}

void BScheduleGridModelTest::signalsOfAnEdit()
{
    BScheduleGridModel model;
    QSignalSpy reset(&model, &BScheduleGridModel::scheduleReset);
    model.setSchedule(scheduleOf({
        "Incremental mon-fri at 21:00",
        "Full 1st sat at 21:00",
        "Full on 1 at 2:00",
    }));
    QCOMPARE(reset.count(), 1);

    QList<BGridPos> changed;
    connect(&model, &BScheduleGridModel::cellsChanged, this,
            [&changed](const QList<BGridPos> &cells) { changed += cells; });
    QSignalSpy cells(&model, &BScheduleGridModel::cellsChanged);
    QSignalSpy scopes(&model, &BScheduleGridModel::scopesChanged);
    QSignalSpy lines(&model, &BScheduleGridModel::runLinesChanged);
    QSignalSpy rules(&model, &BScheduleGridModel::calendarRulesChanged);
    QSignalSpy modified(&model, &BScheduleGridModel::modifiedChanged);
    QSignalSpy selection(&model, &BScheduleGridModel::selectionChanged);

    // One cell of a line: that cell, the lines, "modified"; nothing else
    select(model, Wed, 21);
    QCOMPARE(selection.count(), 1);
    QVERIFY(model.erase().ok);
    QCOMPARE(cells.count(), 1);
    QCOMPARE(changed, QList<BGridPos>({BGridPos{Wed, 21}}));
    QCOMPARE(lines.count(), 1);
    QCOMPARE(modified.count(), 1);
    QCOMPARE(scopes.count(), 0);
    QCOMPARE(rules.count(), 0);
    QCOMPARE(selection.count(), 1);     // an edit keeps the selection
    QVERIFY(model.selection().contains(Wed, 21));

    // A new line in front of nothing: two cells, and the scope has one line more
    changed.clear();
    select(model, Sat, 4, Sun, 4);
    QVERIFY(model.applyBrush(brush("Full")).ok);
    QCOMPARE(changed, QList<BGridPos>({BGridPos{Sat, 4}, BGridPos{Sun, 4}}));
    QCOMPARE(scopes.count(), 1);
    QCOMPARE(rules.count(), 0);
    QCOMPARE(lines.count(), 2);
    QCOMPARE(modified.count(), 1);

    // The line of the first week goes. The lines behind it move up: the
    // cells of the line painted above report another line index (they are
    // out of scope here), and so does the calendar rule.
    changed.clear();
    model.setActiveScope(1);
    select(model, Sat, 21);
    QVERIFY(model.erase().ok);
    QCOMPARE(changed, QList<BGridPos>({BGridPos{Sat, 4}, BGridPos{Sat, 21}, BGridPos{Sun, 4}}));
    QCOMPARE(model.cell(Sat, 4).outOfScope.first().runIndex, 2);
    QCOMPARE(scopes.count(), 2);
    QCOMPARE(rules.count(), 1);
    QCOMPARE(model.calendarRules().first().runIndex, 1);

    // A new schedule: one reset, the selection is gone
    model.setSchedule(scheduleOf({"Full sat at 1:00"}));
    QCOMPARE(reset.count(), 2);
    QVERIFY(model.selection().isEmpty());
    QVERIFY(!model.isModified());
    QCOMPARE(modified.count(), 2);
    QCOMPARE(model.activeScopeIndex(), 0);
}

// --- Calendar rules: edited as a whole --------------------------------------------

void BScheduleGridModelTest::calendarRuleAdded()
{
    BScheduleGridModel model;
    model.setSchedule(scheduleOf({QStringLiteral("incremental mon-fri at 9:05pm"),
                                  QStringLiteral("Level=Full on 1 at 21:00")}));
    QCOMPARE(model.calendarRules().size(), 1);

    // The 15th of January and July, in the evening and at night
    BRunSpec spec;
    spec.overrides.level = QStringLiteral("Differential");
    spec.overrides.pool = QStringLiteral("Half Year");
    spec.mask.mday = 1u << 14;
    spec.mask.month = (1u << 0) | (1u << 6);
    spec.mask.hour = (1u << 3) | (1u << 21);
    spec.minute = 30;
    QVERIFY(BScheduleGridModel::isCalendarRule(spec));

    QSignalSpy rulesChanged(&model, &BScheduleGridModel::calendarRulesChanged);
    QSignalSpy cellsChanged(&model, &BScheduleGridModel::cellsChanged);
    const BGridChange change = model.addCalendarRule(spec);
    QVERIFY2(change.ok, qPrintable(change.error));
    QCOMPARE(change.addedRuns, QList<int>({2}));
    QVERIFY(change.changedRuns.isEmpty() && change.removedRuns.isEmpty());
    QCOMPARE(rulesChanged.count(), 1);
    QCOMPARE(cellsChanged.count(), 0);      // not a line of the grid

    // At the end, the other lines byte for byte as they were reported
    QCOMPARE(texts(model), QStringList({
        QStringLiteral("incremental mon-fri at 9:05pm"),
        QStringLiteral("Level=Full on 1 at 21:00"),
        QStringLiteral("Level=Differential Pool=\"Half Year\" jan,jul on 15 at 03:30 at 21:30")}));
    QCOMPARE(states(model), QStringLiteral("uun"));
    QCOMPARE(model.calendarRules().size(), 2);
    QCOMPARE(model.calendarRules()[1].runIndex, 2);
    QVERIFY(model.calendarRules()[1].spec == spec);
    QVERIFY(model.isModified());

    // What is written reads back as the same run
    const BRunLine check = BRunParser::parse(texts(model).last());
    QVERIFY(check.spec.has_value());
    QVERIFY(*check.spec == spec);

    // Undo and redo
    QVERIFY(model.restore(change.before));
    QCOMPARE(states(model), QStringLiteral("uu"));
    QVERIFY(!model.isModified());
    QVERIFY(model.restore(change.after));
    QCOMPARE(states(model), QStringLiteral("uun"));
}

void BScheduleGridModelTest::calendarRuleChangedAndBack()
{
    BScheduleGridModel model;
    const QStringList loaded{QStringLiteral("Level=Full Pool=Monthly on 1 at 9:00pm"),
                             QStringLiteral("incremental mon-fri at 9:05pm"),
                             QStringLiteral("Level=Full w01/w02 sun at 23:10")};
    model.setSchedule(scheduleOf(loaded));
    QCOMPARE(model.calendarRules().size(), 2);
    const BRunSpec first = model.calendarRules()[0].spec;

    // Also on the 15th
    BRunSpec changed = first;
    changed.mask.mday |= 1u << 14;
    BGridChange change = model.changeCalendarRule(0, changed);
    QVERIFY2(change.ok, qPrintable(change.error));
    QCOMPARE(change.changedRuns, QList<int>({0}));
    QVERIFY(change.addedRuns.isEmpty() && change.removedRuns.isEmpty());
    QCOMPARE(texts(model), QStringList({
        QStringLiteral("Level=Full Pool=Monthly on 1,15 at 21:00"),
        loaded[1], loaded[2]}));
    QCOMPARE(states(model), QStringLiteral("cuu"));
    QCOMPARE(model.runLines()[0].originalText, loaded[0]);

    // The same spec again changes nothing
    change = model.changeCalendarRule(0, changed);
    QVERIFY(change.ok);
    QVERIFY(change.isNoop());

    // Back to what the loaded line said: the reported text again
    change = model.changeCalendarRule(0, first);
    QVERIFY2(change.ok, qPrintable(change.error));
    QCOMPARE(texts(model), loaded);
    QCOMPARE(states(model), QStringLiteral("uuu"));
    QVERIFY(!model.isModified());

    // A rule by week of the year, changed to another level and minute
    BRunSpec weeks = model.calendarRules()[1].spec;
    weeks.overrides.level = QStringLiteral("Differential");
    weeks.minute = 40;
    change = model.changeCalendarRule(2, weeks);
    QVERIFY2(change.ok, qPrintable(change.error));
    QCOMPARE(texts(model), QStringList({loaded[0], loaded[1],
        QStringLiteral("Level=Differential w01/w02 sun at 23:40")}));
    QCOMPARE(states(model), QStringLiteral("uuc"));
}

void BScheduleGridModelTest::calendarRuleRemoved()
{
    BScheduleGridModel model;
    const QStringList loaded{QStringLiteral("Level=Full Pool=Monthly on 1 at 9:00pm"),
                             QStringLiteral("incremental mon-fri at 9:05pm"),
                             QStringLiteral("sat every other week")};
    model.setSchedule(scheduleOf(loaded));
    QCOMPARE(model.notUnderstood().size(), 1);

    const BGridChange change = model.removeCalendarRule(0);
    QVERIFY2(change.ok, qPrintable(change.error));
    QCOMPARE(change.removedRuns, QList<int>({0}));
    QCOMPARE(texts(model), QStringList({loaded[1], loaded[2]}));
    QCOMPARE(states(model), QStringLiteral("ruu"));
    QVERIFY(model.calendarRules().isEmpty());
    // The line that is not understood is where it was, as it was
    QCOMPARE(model.notUnderstood().size(), 1);
    QCOMPARE(model.notUnderstood().first().text, loaded[2]);
    QCOMPARE(model.notUnderstood().first().runIndex, 1);

    QVERIFY(model.restore(change.before));
    QCOMPARE(texts(model), loaded);
    QCOMPARE(states(model), QStringLiteral("uuu"));
    QVERIFY(!model.isModified());
}

void BScheduleGridModelTest::calendarRuleEditsRefuseWhatIsNotOne()
{
    BScheduleGridModel model;
    const QStringList loaded{QStringLiteral("Level=Full on 1 at 21:00"),
                             QStringLiteral("Level=Incremental mon-fri at 21:05"),
                             QStringLiteral("sat every other week")};
    model.setSchedule(scheduleOf(loaded));
    const BRunSpec rule = model.calendarRules().first().spec;

    // A run without days of the month and weeks of the year is painted
    BRunSpec gridLine;
    gridLine.mask.wday = 1u << 1;
    gridLine.mask.hour = 1u << 3;
    QVERIFY(!BScheduleGridModel::isCalendarRule(gridLine));
    BGridChange change = model.addCalendarRule(gridLine);
    QVERIFY(!change.ok);
    QVERIFY(!change.error.isEmpty());
    change = model.changeCalendarRule(0, gridLine);
    QVERIFY(!change.ok);

    // Only calendar rules are changed and removed here: not a line of the
    // grid, not a line that is not understood, not a line that is not there
    for (int index : {1, 2, 3, -1}) {
        QVERIFY(!model.changeCalendarRule(index, rule).ok);
        QVERIFY(!model.removeCalendarRule(index).ok);
    }

    // A rule that cannot be written: no hour at all
    BRunSpec noHour = rule;
    noHour.mask.hour = 0;
    change = model.changeCalendarRule(0, noHour);
    QVERIFY(!change.ok);
    QVERIFY(!change.error.isEmpty());

    // Nothing of all that changed anything
    QCOMPARE(texts(model), loaded);
    QCOMPARE(states(model), QStringLiteral("uuu"));
    QVERIFY(!model.isModified());
}

void BScheduleGridModelTest::sequencesKeepTheIntendedCells_data()
{
    QTest::addColumn<QStringList>("loaded");
    QTest::addColumn<quint32>("seed");

    const QStringList usual = {
        "Full 1st sat at 21:00",
        "Differential 2nd-5th sat at 21:00",
        "Incremental mon-fri at 21:00",
    };
    const QStringList mixed = {
        "Level=Full Pool=Monthly 1st sun at 2:05",
        "Level=Differential 2nd-5th sun at 2:05",
        "Level=Incremental Pool=Daily mon-sat at 2:05",
        "Level=Full on 1 at 2:05",
        "Bogus sat at 5:00",
        "Level=Full w01/w02 sat at 23:10",
        "Incremental sat-sun hourly at 0:30",
        "Full jan-mar last fri at 21:00 at 22:00",
        "Incremental tue",
        "Full on 15 at 04:00",
    };
    for (quint32 seed = 1; seed <= 12; ++seed) {
        QTest::addRow("empty %u", seed) << QStringList() << seed;
        QTest::addRow("usual %u", seed) << usual << seed * 101;
        QTest::addRow("mixed %u", seed) << mixed << seed * 7919;
    }
}

void BScheduleGridModelTest::sequencesKeepTheIntendedCells()
{
    QFETCH(QStringList, loaded);
    QFETCH(quint32, seed);

    BScheduleGridModel model;
    model.setSchedule(scheduleOf(loaded));
    if (seed % 2)
        model.setFirstDayOfWeek(Qt::Sunday);

    const QList<BGridBrush> palette = {
        brush("Full"), brush("Incremental"), brush("Incremental", 30),
        brush("Full", 5, "Monthly"), brush(nullptr, 15),
    };
    const QList<BGridScope> extraScopes = {
        scopeOf(0x01), scopeOf(0x00, true), scopeOf(0x03), scopeOf(0x1F, false, 0x00F),
    };
    const QStringList fixedLines = notGridLines(loaded);

    // What every cell of every scope is meant to contain
    QStringList intended = entriesOf(loaded);
    Sequence numbers{seed};

    for (int step = 0; step < 60; ++step) {
        const QString where = QStringLiteral("step %1").arg(step);

        // A scope
        if (numbers.next(6) == 0)
            model.addScope(extraScopes[numbers.next(int(extraScopes.size()))]);
        model.setActiveScope(numbers.next(int(model.scopes().size())));
        const BGridScope scope = model.activeScope();

        // A selection
        BGridSelection selection;
        switch (numbers.next(6)) {
        case 0:
            selection.selectCell(numbers.next(7), numbers.next(24));
            break;
        case 1:
        case 2:
            selection.selectCell(numbers.next(7), numbers.next(24));
            selection.extendTo(numbers.next(7), numbers.next(24));
            break;
        case 3:
            selection.selectRow(numbers.next(7));
            if (numbers.next(2))
                selection.selectColumn(numbers.next(24), BGridSelection::Add);
            break;
        case 4:
            selection.selectCell(numbers.next(7), numbers.next(24));
            for (int i = 0; i < 6; ++i)
                selection.toggleCell(numbers.next(7), 20 + numbers.next(4));
            break;
        case 5:
            if (numbers.next(4) == 0) {
                selection.selectAll();
            } else {
                selection.selectColumn(numbers.next(24));
                selection.extendTo(6, numbers.next(24));
            }
            break;
        }
        model.setSelection(selection);

        // The selected cells as weekdays of Bareos
        QList<BGridPos> selected;
        for (const BGridPos &pos : selection.cells().cells())
            selected.append(BGridPos{model.weekdayOfRow(pos.row), pos.column});

        const auto inCell = [&](const BGridPos &pos) {
            const QString prefix = entry(scope, pos.row, pos.column, BGridBrush()).section(QLatin1Char('|'), 0, 2)
                                   + QLatin1Char('|');
            return prefix;
        };

        // An edit, on the model and on the list of what is intended
        BGridChange change;
        const int operation = numbers.next(10);
        if (operation < 4) {
            const BGridBrush b = palette[numbers.next(int(palette.size()))];
            for (const BGridPos &pos : selected) {
                const QString prefix = inCell(pos);
                const QString mine = entry(scope, pos.row, pos.column, b);
                intended.removeIf([&](const QString &e) { return e.startsWith(prefix) && e != mine; });
                if (!intended.contains(mine))
                    intended.append(mine);
            }
            change = model.applyBrush(b, BScheduleGridModel::ReplaceRuns);
        } else if (operation < 6) {
            const BGridBrush b = palette[numbers.next(int(palette.size()))];
            for (const BGridPos &pos : selected) {
                const QString mine = entry(scope, pos.row, pos.column, b);
                if (!intended.contains(mine))
                    intended.append(mine);
            }
            change = model.applyBrush(b, BScheduleGridModel::AddRun);
        } else if (operation < 9) {
            for (const BGridPos &pos : selected) {
                const QString prefix = inCell(pos);
                intended.removeIf([&](const QString &e) { return e.startsWith(prefix); });
            }
            change = model.erase();
        } else {
            const BSchedule now = model.schedule();
            if (now.runs.isEmpty())
                continue;
            const int index = numbers.next(int(now.runs.size()));
            const int minute = QList<int>({0, 5, 15, 30})[numbers.next(4)];
            const BRunLine &line = now.runs[index];
            change = model.moveRun(index, minute);
            if (!line.spec || !isGridSpec(*line.spec)) {
                QVERIFY2(!change.ok, qPrintable(where));
                change.ok = true;   // refused, as it must be; nothing is intended to change
            } else {
                BRunSpec moved = *line.spec;
                moved.minute = minute;
                const BGridScope lineScope = BGridScope::of(line.spec->mask);
                for (int weekday = 0; weekday < 7; ++weekday) {
                    for (int hour = 0; hour < 24; ++hour) {
                        if (!(line.spec->mask.wday & (1u << weekday))
                            || !(line.spec->mask.hour & (1u << hour))) {
                            continue;
                        }
                        const QString old = entry(lineScope, weekday, hour,
                                                  BGridBrush{line.spec->overrides, line.spec->minute});
                        QVERIFY2(intended.removeOne(old), qPrintable(where));
                        intended.append(entry(lineScope, weekday, hour,
                                              BGridBrush{moved.overrides, moved.minute}));
                    }
                }
            }
        }
        QVERIFY2(change.ok, qPrintable(where + QStringLiteral(": ") + change.error));

        // The cells read from the Run lines as they would be written
        const QStringList written = texts(model);
        std::sort(intended.begin(), intended.end());
        const QStringList actual = entriesOf(written);
        if (actual != intended) {
            qWarning().noquote() << where << "\n" << written.join(QLatin1Char('\n'));
            QCOMPARE(actual, intended);
        }

        // What is not of the grid is as it was loaded, in its order
        QCOMPARE(notGridLines(written), fixedLines);

        // Every line that counts as untouched is the reported text, and
        // every other line reads back as what the model holds
        const BSchedule now = model.schedule();
        for (const BGridRunLineText &line : model.runLines()) {
            if (line.state == BGridRunLineText::Untouched && !line.respelled)
                QCOMPARE(line.text, line.originalText);
            if (line.state == BGridRunLineText::Removed || !line.understood)
                continue;
            const BRunLine check = BRunParser::parse(line.text);
            QVERIFY2(check.spec.has_value(), qPrintable(line.text));
            QVERIFY2(*check.spec == *now.runs[line.runIndex].spec, qPrintable(line.text));
        }

        // The model's own cells say the same as a model that loads the result
        BScheduleGridModel reloaded;
        reloaded.setFirstDayOfWeek(model.firstDayOfWeek());
        reloaded.setSchedule(scheduleOf(written));
        const int reloadedScope = reloaded.indexOfScope(scope);
        if (reloadedScope >= 0)
            reloaded.setActiveScope(reloadedScope);
        for (int row = 0; row < 7; ++row) {
            for (int column = 0; column < 24; ++column) {
                const QString mine = runsText(model.cell(row, column).inScope);
                const QString theirs = reloadedScope >= 0
                    ? runsText(reloaded.cell(row, column).inScope) : QString();
                if (mine != theirs)
                    QFAIL(qPrintable(QStringLiteral("%1, cell %2/%3: \"%4\" but reloaded \"%5\"")
                                         .arg(where).arg(row).arg(column).arg(mine, theirs)));
            }
        }

        // Undo and redo give the same lines
        if (!change.isNoop() && numbers.next(4) == 0) {
            QVERIFY(model.restore(change.before));
            QVERIFY(model.restore(change.after));
            QCOMPARE(texts(model), written);
        }
    }
}

QTEST_APPLESS_MAIN(BScheduleGridModelTest)
#include "bschedulegridmodel_test.moc"
