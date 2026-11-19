/**
 * @file bschedulegridmodel.cpp
 * @brief The model behind the schedule grid (see the header for the terms
 *        and the edit rules)
 */

#include "schedules/model/bschedulegridmodel.h"

#include <algorithm>
#include <tuple>

// ============================================================================
// BGridCellSet
// ============================================================================

namespace {

int bitCount(quint32 bits)
{
    int count = 0;
    for (; bits; bits &= bits - 1)
        ++count;
    return count;
}

int lowestBit(quint32 bits)
{
    for (int i = 0; i < 32; ++i) {
        if (bits & (1u << i))
            return i;
    }
    return 32;
}

/// Rank over GF(2): no partition into products can have fewer parts
int rankOf(QList<quint32> vectors)
{
    int rank = 0;
    for (int bit = 0; bit < BGridCellSet::Columns; ++bit) {
        const quint32 mask = 1u << bit;
        int pivot = -1;
        for (int i = rank; i < vectors.size(); ++i) {
            if (vectors[i] & mask) {
                pivot = i;
                break;
            }
        }
        if (pivot < 0)
            continue;
        vectors.swapItemsAt(rank, pivot);
        for (int i = 0; i < vectors.size(); ++i) {
            if (i != rank && (vectors[i] & mask))
                vectors[i] ^= vectors[rank];
        }
        ++rank;
    }
    return rank;
}

/**
 * The search for the smallest partition into products.
 *
 * Rows with the same columns are one unit. A partition into k products is
 * then a family of k sets of units such that the units of every column are
 * a union of members of the family that do not overlap: the member is the
 * rows of a product, and the columns that use it are its columns.
 */
struct FamilySearch
{
    QList<quint8> patterns;     ///< Per distinct column: the units that have it
    QList<quint8> family;
    int budget = 0;

    /// Is @p pattern a union of members that do not overlap? @p used
    /// receives their indices.
    bool expressible(quint8 pattern, QList<int> *used = nullptr) const
    {
        if (!pattern)
            return true;
        const quint8 low = quint8(pattern & (~pattern + 1));
        for (int i = 0; i < family.size(); ++i) {
            const quint8 member = family[i];
            if ((member & low) && !(member & ~pattern)) {
                if (expressible(quint8(pattern & ~member), used)) {
                    if (used)
                        used->append(i);
                    return true;
                }
            }
        }
        return false;
    }

    /// Can the family be completed with at most @p size members?
    bool complete(int size)
    {
        if (--budget < 0)
            return false;
        quint8 open = 0;
        for (const quint8 pattern : patterns) {
            if (!expressible(pattern)) {
                open = pattern;
                break;
            }
        }
        if (!open)
            return true;
        if (family.size() >= size)
            return false;
        // The partition of this column has a member that is not in the
        // family yet, and it is a part of the column
        for (quint8 member = open; member; member = quint8((member - 1) & open)) {
            if (family.contains(member))
                continue;
            family.append(member);
            if (complete(size))
                return true;
            family.removeLast();
            if (budget < 0)
                return false;
        }
        return false;
    }
};

const int searchBudget = 20000;

} // namespace

BGridCellSet BGridCellSet::product(quint8 rows, quint32 columns)
{
    BGridCellSet set;
    for (int row = 0; row < Rows; ++row) {
        if (rows & (1u << row))
            set.m_rows[size_t(row)] = columns & AllColumns;
    }
    return set;
}

BGridCellSet BGridCellSet::rect(int row1, int column1, int row2, int column2)
{
    const int top = qBound(0, qMin(row1, row2), Rows - 1);
    const int bottom = qBound(0, qMax(row1, row2), Rows - 1);
    const int left = qBound(0, qMin(column1, column2), Columns - 1);
    const int right = qBound(0, qMax(column1, column2), Columns - 1);
    quint32 columns = 0;
    for (int column = left; column <= right; ++column)
        columns |= (1u << column);
    BGridCellSet set;
    for (int row = top; row <= bottom; ++row)
        set.m_rows[size_t(row)] = columns;
    return set;
}

bool BGridCellSet::contains(int row, int column) const
{
    if (row < 0 || row >= Rows || column < 0 || column >= Columns)
        return false;
    return m_rows[size_t(row)] & (1u << column);
}

void BGridCellSet::insert(int row, int column)
{
    if (row < 0 || row >= Rows || column < 0 || column >= Columns)
        return;
    m_rows[size_t(row)] |= (1u << column);
}

void BGridCellSet::remove(int row, int column)
{
    if (row < 0 || row >= Rows || column < 0 || column >= Columns)
        return;
    m_rows[size_t(row)] &= ~(1u << column);
}

quint32 BGridCellSet::rowBits(int row) const
{
    return (row < 0 || row >= Rows) ? 0 : m_rows[size_t(row)];
}

void BGridCellSet::setRowBits(int row, quint32 columns)
{
    if (row >= 0 && row < Rows)
        m_rows[size_t(row)] = columns & AllColumns;
}

bool BGridCellSet::isEmpty() const
{
    for (const quint32 bits : m_rows) {
        if (bits)
            return false;
    }
    return true;
}

int BGridCellSet::count() const
{
    int count = 0;
    for (const quint32 bits : m_rows)
        count += bitCount(bits);
    return count;
}

QList<BGridPos> BGridCellSet::cells() const
{
    QList<BGridPos> result;
    for (int row = 0; row < Rows; ++row) {
        for (int column = 0; column < Columns; ++column) {
            if (m_rows[size_t(row)] & (1u << column))
                result.append(BGridPos{row, column});
        }
    }
    return result;
}

BGridCellSet BGridCellSet::united(const BGridCellSet &other) const
{
    BGridCellSet result;
    for (size_t row = 0; row < Rows; ++row)
        result.m_rows[row] = m_rows[row] | other.m_rows[row];
    return result;
}

BGridCellSet BGridCellSet::intersected(const BGridCellSet &other) const
{
    BGridCellSet result;
    for (size_t row = 0; row < Rows; ++row)
        result.m_rows[row] = m_rows[row] & other.m_rows[row];
    return result;
}

BGridCellSet BGridCellSet::subtracted(const BGridCellSet &other) const
{
    BGridCellSet result;
    for (size_t row = 0; row < Rows; ++row)
        result.m_rows[row] = m_rows[row] & ~other.m_rows[row];
    return result;
}

BGridCellSet BGridCellSet::toggled(const BGridCellSet &other) const
{
    BGridCellSet result;
    for (size_t row = 0; row < Rows; ++row)
        result.m_rows[row] = m_rows[row] ^ other.m_rows[row];
    return result;
}

bool BGridCellSet::intersects(const BGridCellSet &other) const
{
    for (size_t row = 0; row < Rows; ++row) {
        if (m_rows[row] & other.m_rows[row])
            return true;
    }
    return false;
}

bool BGridCellSet::isProduct(BGridProduct *product) const
{
    quint8 rows = 0;
    quint32 columns = 0;
    for (int row = 0; row < Rows; ++row) {
        const quint32 bits = m_rows[size_t(row)];
        if (!bits)
            continue;
        if (columns && bits != columns)
            return false;
        columns = bits;
        rows |= quint8(1u << row);
    }
    if (!rows)
        return false;
    if (product)
        *product = BGridProduct{rows, columns};
    return true;
}

QList<BGridProduct> BGridCellSet::products() const
{
    // Units: rows with the same columns
    QList<quint8> unitRows;
    QList<quint32> unitColumns;
    for (int row = 0; row < Rows; ++row) {
        const quint32 bits = m_rows[size_t(row)];
        if (!bits)
            continue;
        const int unit = unitColumns.indexOf(bits);
        if (unit >= 0) {
            unitRows[unit] |= quint8(1u << row);
        } else {
            unitRows.append(quint8(1u << row));
            unitColumns.append(bits);
        }
    }
    if (unitRows.isEmpty())
        return {};

    // Distinct columns: which units have them
    QList<quint8> patterns;
    QList<quint32> patternColumns;
    for (int column = 0; column < Columns; ++column) {
        quint8 pattern = 0;
        for (int unit = 0; unit < unitColumns.size(); ++unit) {
            if (unitColumns[unit] & (1u << column))
                pattern |= quint8(1u << unit);
        }
        if (!pattern)
            continue;
        const int known = patterns.indexOf(pattern);
        if (known >= 0) {
            patternColumns[known] |= (1u << column);
        } else {
            patterns.append(pattern);
            patternColumns.append(1u << column);
        }
    }

    // Two partitions are always at hand: one product per unit of rows, or
    // one per distinct column. With the same number the one by rows wins.
    FamilySearch search;
    search.patterns = patterns;
    if (patterns.size() < unitRows.size()) {
        search.family = patterns;
    } else {
        for (int unit = 0; unit < unitRows.size(); ++unit)
            search.family.append(quint8(1u << unit));
    }

    // Fewer are possible only below that and not below the rank
    const int upper = int(search.family.size());
    const int lower = rankOf(unitColumns);
    if (lower < upper) {
        FamilySearch better;
        better.patterns = patterns;
        better.budget = searchBudget;
        for (int size = lower; size < upper && better.budget >= 0; ++size) {
            better.family.clear();
            if (better.complete(size)) {
                search.family = better.family;
                break;
            }
        }
    }

    // A member of the family is the rows of a product; its columns are the
    // columns whose partition uses it
    QList<quint32> memberColumns(search.family.size(), quint32(0));
    for (int i = 0; i < patterns.size(); ++i) {
        QList<int> used;
        search.expressible(patterns[i], &used);
        for (const int member : used)
            memberColumns[member] |= patternColumns[i];
    }

    QList<BGridProduct> result;
    for (int member = 0; member < search.family.size(); ++member) {
        if (!memberColumns[member])
            continue;
        BGridProduct product;
        product.columns = memberColumns[member];
        for (int unit = 0; unit < unitRows.size(); ++unit) {
            if (search.family[member] & (1u << unit))
                product.rows |= unitRows[unit];
        }
        result.append(product);
    }

    std::sort(result.begin(), result.end(), [](const BGridProduct &a, const BGridProduct &b) {
        const int sizeA = bitCount(a.rows) * bitCount(a.columns);
        const int sizeB = bitCount(b.rows) * bitCount(b.columns);
        return std::make_tuple(-sizeA, lowestBit(a.rows), a.columns, a.rows)
             < std::make_tuple(-sizeB, lowestBit(b.rows), b.columns, b.rows);
    });
    return result;
}

BGridRect BGridCellSet::boundingRect() const
{
    BGridRect rect;
    bool any = false;
    for (int row = 0; row < Rows; ++row) {
        const quint32 bits = m_rows[size_t(row)];
        if (!bits)
            continue;
        int left = lowestBit(bits);
        int right = left;
        for (int column = left; column < Columns; ++column) {
            if (bits & (1u << column))
                right = column;
        }
        if (!any) {
            rect = BGridRect{row, left, row, right};
            any = true;
        } else {
            rect.bottom = row;
            rect.left = qMin(rect.left, left);
            rect.right = qMax(rect.right, right);
        }
    }
    return rect;
}

QList<BGridRect> BGridCellSet::rects() const
{
    QList<BGridRect> result;
    for (int row = 0; row < Rows; ++row) {
        const quint32 bits = m_rows[size_t(row)];
        int column = 0;
        while (column < Columns) {
            if (!(bits & (1u << column))) {
                ++column;
                continue;
            }
            int right = column;
            while (right + 1 < Columns && (bits & (1u << (right + 1))))
                ++right;
            // The same stretch in the row above: one taller rectangle
            bool grown = false;
            for (BGridRect &rect : result) {
                if (rect.bottom == row - 1 && rect.left == column && rect.right == right) {
                    rect.bottom = row;
                    grown = true;
                    break;
                }
            }
            if (!grown)
                result.append(BGridRect{row, column, row, right});
            column = right + 1;
        }
    }
    std::sort(result.begin(), result.end(), [](const BGridRect &a, const BGridRect &b) {
        return std::make_tuple(a.top, a.left) < std::make_tuple(b.top, b.left);
    });
    return result;
}

// ============================================================================
// BGridSelection
// ============================================================================

bool BGridSelection::valid(int row, int column)
{
    return row >= 0 && row < BGridCellSet::Rows && column >= 0 && column < BGridCellSet::Columns;
}

void BGridSelection::apply(const BGridCellSet &cells, Mode mode)
{
    m_base = (mode == Replace) ? BGridCellSet() : m_cells;
    m_subtract = false;
    switch (mode) {
    case Replace:
        m_cells = cells;
        break;
    case Add:
        m_cells = m_cells.united(cells);
        break;
    case Toggle:
        if (m_cells.intersected(cells) == cells) {
            m_cells = m_cells.subtracted(cells);
            m_subtract = true;
        } else {
            m_cells = m_cells.united(cells);
        }
        break;
    }
}

void BGridSelection::clear()
{
    *this = BGridSelection();
}

void BGridSelection::selectCell(int row, int column)
{
    if (!valid(row, column))
        return;
    apply(BGridCellSet::rect(row, column, row, column), Replace);
    m_anchor = BGridPos{row, column};
}

void BGridSelection::toggleCell(int row, int column)
{
    if (!valid(row, column))
        return;
    apply(BGridCellSet::rect(row, column, row, column), Toggle);
    m_anchor = BGridPos{row, column};
}

void BGridSelection::extendTo(int row, int column, bool keep)
{
    if (!valid(row, column))
        return;
    if (!m_anchor) {
        apply(BGridCellSet::rect(row, column, row, column), keep ? Add : Replace);
        m_anchor = BGridPos{row, column};
        return;
    }
    const BGridCellSet rect = BGridCellSet::rect(m_anchor->row, m_anchor->column, row, column);
    if (!keep)
        m_cells = rect;
    else if (m_subtract)
        m_cells = m_base.subtracted(rect);
    else
        m_cells = m_base.united(rect);
}

void BGridSelection::selectRow(int row, Mode mode)
{
    if (!valid(row, 0))
        return;
    apply(BGridCellSet::product(quint8(1u << row), BGridCellSet::AllColumns), mode);
    m_anchor = BGridPos{row, 0};
}

void BGridSelection::selectColumn(int column, Mode mode)
{
    if (!valid(0, column))
        return;
    apply(BGridCellSet::product(BGridCellSet::AllRows, 1u << column), mode);
    m_anchor = BGridPos{0, column};
}

void BGridSelection::selectAll()
{
    apply(BGridCellSet::all(), Replace);
    m_anchor = BGridPos{0, 0};
}

bool BGridSelection::isRowSelected(int row) const
{
    return valid(row, 0) && m_cells.rowBits(row) == BGridCellSet::AllColumns;
}

bool BGridSelection::isColumnSelected(int column) const
{
    if (!valid(0, column))
        return false;
    for (int row = 0; row < BGridCellSet::Rows; ++row) {
        if (!m_cells.contains(row, column))
            return false;
    }
    return true;
}

BGridSelection BGridSelection::withRowsMapped(const std::array<int, 7> &rowOfRow) const
{
    BGridSelection result;
    for (int row = 0; row < BGridCellSet::Rows; ++row) {
        result.m_cells.setRowBits(rowOfRow[size_t(row)], m_cells.rowBits(row));
        result.m_base.setRowBits(rowOfRow[size_t(row)], m_base.rowBits(row));
    }
    result.m_subtract = m_subtract;
    if (m_anchor)
        result.m_anchor = BGridPos{rowOfRow[size_t(m_anchor->row)], m_anchor->column};
    return result;
}

// ============================================================================
// Scopes and the small value types
// ============================================================================

BGridScope BGridScope::of(const BDateTimeMask &mask)
{
    BGridScope scope;
    scope.months = mask.month & BDateTimeMask::AllMonths;
    scope.weeksOfMonth = mask.wom & BDateTimeMask::AllWoms;
    scope.last = mask.last7DaysOfMonth;
    return scope;
}

bool BGridScope::isEveryWeek() const
{
    return *this == BGridScope();
}

bool BGridScope::isValid() const
{
    return months != 0 && !(months & ~BDateTimeMask::AllMonths)
        && !(weeksOfMonth & ~BDateTimeMask::AllWoms)
        && (weeksOfMonth != 0 || last);
}

bool BGridScope::sharesDatesWith(const BGridScope &other) const
{
    if (!(months & other.months))
        return false;
    // The last seven days of a month are days 22 to 31: its 4th and 5th week
    const quint8 lastWeeks = 0x18;
    return (weeksOfMonth & other.weeksOfMonth)
        || (last && other.last)
        || (last && (other.weeksOfMonth & lastWeeks))
        || (other.last && (weeksOfMonth & lastWeeks));
}

bool BGridRun::operator==(const BGridRun &other) const
{
    return runIndex == other.runIndex
        && overrides == other.overrides
        && minute == other.minute
        && scopeIndex == other.scopeIndex
        && withoutTime == other.withoutTime
        && sharesDatesWithActiveScope == other.sharesDatesWithActiveScope;
}

bool BGridCalendarRule::operator==(const BGridCalendarRule &other) const
{
    return runIndex == other.runIndex
        && spec == other.spec
        && text == other.text
        && byDayOfMonth == other.byDayOfMonth
        && byWeekOfYear == other.byWeekOfYear
        && withoutTime == other.withoutTime;
}

namespace {

/// The order of the scope bar
auto scopeKey(const BGridScope &scope)
{
    const bool allMonths = scope.months == BDateTimeMask::AllMonths;
    const bool allWeeks = scope.weeksOfMonth == BDateTimeMask::AllWoms && !scope.last;
    return std::make_tuple(allMonths ? 0 : 1,
                           lowestBit(scope.months),
                           scope.months,
                           allWeeks ? 0 : 1,
                           scope.weeksOfMonth ? lowestBit(scope.weeksOfMonth) : 5,
                           scope.weeksOfMonth,
                           scope.last);
}

BGridBrush brushOf(const BRunSpec &spec)
{
    return BGridBrush{spec.overrides, spec.minute};
}

} // namespace

// ============================================================================
// BScheduleGridModel
// ============================================================================

BScheduleGridModel::BScheduleGridModel(QObject *parent)
    : QObject(parent)
{
    rebuildDerived();
}

bool BScheduleGridModel::isGridLine(const BRunLine &line)
{
    if (!line.spec)
        return false;
    const BDateTimeMask &mask = line.spec->mask;
    return (mask.mday & BDateTimeMask::AllMdays) == BDateTimeMask::AllMdays
        && (mask.woy & BDateTimeMask::AllWoys) == BDateTimeMask::AllWoys;
}

bool BScheduleGridModel::isGenerated(const Item &item)
{
    return item.line.dirty || item.line.original.isEmpty();
}

BGridCellSet BScheduleGridModel::cellsOf(const BRunSpec &spec)
{
    return BGridCellSet::product(spec.mask.wday & BDateTimeMask::AllWdays,
                                 spec.mask.hour & BDateTimeMask::AllHours);
}

// --- The schedule -----------------------------------------------------------

void BScheduleGridModel::setSchedule(const BSchedule &schedule)
{
    m_loaded = schedule;
    QList<Item> items;
    items.reserve(schedule.runs.size());
    for (int i = 0; i < schedule.runs.size(); ++i) {
        Item item;
        item.line = schedule.runs[i];
        item.origin = i;
        if (item.line.spec)
            item.text = BRunWriter::lineText(item.line);
        items.append(item);
    }
    m_knownScopes.clear();
    m_activeScopeValue = BGridScope::everyWeek();
    installItems(items, true);
}

BSchedule BScheduleGridModel::schedule() const
{
    BSchedule schedule = m_loaded;
    schedule.runs.clear();
    for (const Item &item : m_items)
        schedule.runs.append(item.line);
    return schedule;
}

bool BScheduleGridModel::isModified() const
{
    if (m_items.size() != m_loaded.runs.size())
        return true;
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].origin != i || m_items[i].line.dirty != m_loaded.runs[i].dirty)
            return true;
    }
    return false;
}

bool BScheduleGridModel::canBeWritten(QString *reason) const
{
    if (reason)
        reason->clear();
    for (int i = 0; i < m_items.size(); ++i) {
        const Item &item = m_items[i];
        if (!item.line.spec) {
            if (reason) {
                *reason = tr("Run line %1 (\"%2\") is not understood: %3")
                              .arg(i + 1).arg(item.line.original, item.line.parseError);
            }
            return false;
        }
        if (item.text.isEmpty()) {
            if (reason) {
                QString error;
                BRunWriter::lineText(item.line, &error);
                *reason = tr("Run line %1: %2").arg(i + 1).arg(error);
            }
            return false;
        }
    }
    return true;
}

// --- Rows and weekdays ------------------------------------------------------

void BScheduleGridModel::setFirstDayOfWeek(Qt::DayOfWeek day)
{
    if (day < Qt::Monday || day > Qt::Sunday || day == m_firstDay)
        return;
    // The selection stays on the weekdays it is on
    std::array<int, 7> weekdayOfOldRow{};
    for (int row = 0; row < 7; ++row)
        weekdayOfOldRow[size_t(row)] = weekdayOfRow(row);
    m_firstDay = day;
    std::array<int, 7> newRowOfOldRow{};
    for (int row = 0; row < 7; ++row)
        newRowOfOldRow[size_t(row)] = rowOfWeekday(weekdayOfOldRow[size_t(row)]);
    m_selection = m_selection.withRowsMapped(newRowOfOldRow);
    emit firstDayOfWeekChanged();
    emit selectionChanged();
}

int BScheduleGridModel::weekdayOfRow(int row) const
{
    // Qt counts Monday = 1 ... Sunday = 7, Bareos Sunday = 0 ... Saturday = 6
    return (int(m_firstDay) % 7 + row % 7 + 7) % 7;
}

int BScheduleGridModel::rowOfWeekday(int weekday) const
{
    return ((weekday - int(m_firstDay) % 7) % 7 + 7) % 7;
}

Qt::DayOfWeek BScheduleGridModel::dayOfWeekOfRow(int row) const
{
    const int weekday = weekdayOfRow(row);
    return weekday == 0 ? Qt::Sunday : Qt::DayOfWeek(weekday);
}

BGridCellSet BScheduleGridModel::toWeekdays(const BGridCellSet &rows) const
{
    BGridCellSet result;
    for (int row = 0; row < 7; ++row)
        result.setRowBits(weekdayOfRow(row), rows.rowBits(row));
    return result;
}

BGridCellSet BScheduleGridModel::toRows(const BGridCellSet &weekdays) const
{
    BGridCellSet result;
    for (int weekday = 0; weekday < 7; ++weekday)
        result.setRowBits(rowOfWeekday(weekday), weekdays.rowBits(weekday));
    return result;
}

// --- Scopes -----------------------------------------------------------------

BGridScope BScheduleGridModel::activeScope() const
{
    return m_activeScopeValue;
}

int BScheduleGridModel::indexOfScope(const BGridScope &scope) const
{
    for (int i = 0; i < m_scopes.size(); ++i) {
        if (m_scopes[i].scope == scope)
            return i;
    }
    return -1;
}

void BScheduleGridModel::setActiveScope(int index)
{
    if (index < 0 || index >= m_scopes.size() || index == m_activeScope)
        return;
    m_activeScopeValue = m_scopes[index].scope;
    m_activeScope = index;
    m_cells = computeCells();
    emit activeScopeChanged(index);
}

int BScheduleGridModel::addScope(const BGridScope &scope)
{
    if (!scope.isValid())
        return -1;
    if (indexOfScope(scope) < 0) {
        const int activeBefore = m_activeScope;
        m_knownScopes.append(scope);
        rebuildScopes();
        // Scope indices are part of what the cells report
        m_cells = computeCells();
        emit scopesChanged();
        if (m_activeScope != activeBefore)
            emit activeScopeChanged(m_activeScope);
    }
    return indexOfScope(scope);
}

bool BScheduleGridModel::removeScope(int index)
{
    if (index < 0 || index >= m_scopes.size())
        return false;
    const BGridScopeInfo info = m_scopes[index];
    if (info.runCount > 0 || info.scope.isEveryWeek())
        return false;
    const int activeBefore = m_activeScope;
    const bool wasActive = info.scope == m_activeScopeValue;
    m_knownScopes.removeAll(info.scope);
    if (wasActive)
        m_activeScopeValue = BGridScope::everyWeek();
    rebuildScopes();
    m_cells = computeCells();
    emit scopesChanged();
    if (wasActive || m_activeScope != activeBefore)
        emit activeScopeChanged(m_activeScope);
    return true;
}

void BScheduleGridModel::rebuildScopes()
{
    if (!m_knownScopes.contains(BGridScope::everyWeek()))
        m_knownScopes.append(BGridScope::everyWeek());
    for (const Item &item : std::as_const(m_items)) {
        if (!isGridLine(item.line))
            continue;
        const BGridScope scope = BGridScope::of(item.line.spec->mask);
        if (!m_knownScopes.contains(scope))
            m_knownScopes.append(scope);
    }
    std::sort(m_knownScopes.begin(), m_knownScopes.end(),
              [](const BGridScope &a, const BGridScope &b) { return scopeKey(a) < scopeKey(b); });

    m_scopes.clear();
    for (const BGridScope &scope : std::as_const(m_knownScopes))
        m_scopes.append(BGridScopeInfo{scope, 0});
    for (const Item &item : std::as_const(m_items)) {
        if (isGridLine(item.line))
            ++m_scopes[indexOfScope(BGridScope::of(item.line.spec->mask))].runCount;
    }

    if (!m_knownScopes.contains(m_activeScopeValue))
        m_activeScopeValue = BGridScope::everyWeek();
    m_activeScope = indexOfScope(m_activeScopeValue);
}

// --- Cells ------------------------------------------------------------------

BScheduleGridModel::Cells BScheduleGridModel::computeCells() const
{
    Cells cells;
    for (int i = 0; i < m_items.size(); ++i) {
        const BRunLine &line = m_items[i].line;
        if (!isGridLine(line))
            continue;
        const BRunSpec &spec = *line.spec;
        const BGridScope scope = BGridScope::of(spec.mask);
        const bool inScope = scope == m_activeScopeValue;

        BGridRun run;
        run.runIndex = i;
        run.overrides = spec.overrides;
        run.minute = spec.minute;
        run.scopeIndex = indexOfScope(scope);
        run.withoutTime = !spec.haveHour;
        run.sharesDatesWithActiveScope = scope.sharesDatesWith(m_activeScopeValue);

        for (size_t weekday = 0; weekday < 7; ++weekday) {
            if (!(spec.mask.wday & (1u << weekday)))
                continue;
            for (size_t hour = 0; hour < 24; ++hour) {
                if (!(spec.mask.hour & (1u << hour)))
                    continue;
                BGridCell &cell = cells[weekday][hour];
                (inScope ? cell.inScope : cell.outOfScope).append(run);
            }
        }
    }
    // By minute; lines were added in their order, which a stable sort keeps
    const auto byMinute = [](const BGridRun &a, const BGridRun &b) { return a.minute < b.minute; };
    for (auto &row : cells) {
        for (BGridCell &cell : row) {
            std::stable_sort(cell.inScope.begin(), cell.inScope.end(), byMinute);
            std::stable_sort(cell.outOfScope.begin(), cell.outOfScope.end(), byMinute);
        }
    }
    return cells;
}

BGridCell BScheduleGridModel::cellAtWeekday(int weekday, int hour) const
{
    if (weekday < 0 || weekday >= 7 || hour < 0 || hour >= 24)
        return BGridCell();
    BGridCell cell = m_cells[size_t(weekday)][size_t(hour)];
    cell.expectedConcurrentRuns = m_load[size_t(weekday)][size_t(hour)];
    return cell;
}

BGridCell BScheduleGridModel::cell(int row, int column) const
{
    if (row < 0 || row >= 7)
        return BGridCell();
    return cellAtWeekday(weekdayOfRow(row), column);
}

QList<BGridBrush> BScheduleGridModel::brushes() const
{
    QList<BGridBrush> result;
    for (const Item &item : m_items) {
        if (!isGridLine(item.line))
            continue;
        const BGridBrush brush = brushOf(*item.line.spec);
        if (!result.contains(brush))
            result.append(brush);
    }
    return result;
}

BGridCellSet BScheduleGridModel::cellsOfRun(int runIndex, int *scopeIndex) const
{
    if (scopeIndex)
        *scopeIndex = -1;
    if (runIndex < 0 || runIndex >= m_items.size() || !isGridLine(m_items[runIndex].line))
        return BGridCellSet();
    const BRunSpec &spec = *m_items[runIndex].line.spec;
    if (scopeIndex)
        *scopeIndex = indexOfScope(BGridScope::of(spec.mask));
    return toRows(cellsOf(spec));
}

void BScheduleGridModel::setExpectedLoad(int weekday, int hour, std::optional<double> concurrentRuns)
{
    if (weekday < 0 || weekday >= 7 || hour < 0 || hour >= 24)
        return;
    if (m_load[size_t(weekday)][size_t(hour)] == concurrentRuns)
        return;
    m_load[size_t(weekday)][size_t(hour)] = concurrentRuns;
    emit cellsChanged({BGridPos{rowOfWeekday(weekday), hour}});
}

void BScheduleGridModel::clearExpectedLoad()
{
    QList<BGridPos> changed;
    for (int weekday = 0; weekday < 7; ++weekday) {
        for (int hour = 0; hour < 24; ++hour) {
            if (m_load[size_t(weekday)][size_t(hour)]) {
                m_load[size_t(weekday)][size_t(hour)].reset();
                changed.append(BGridPos{rowOfWeekday(weekday), hour});
            }
        }
    }
    if (!changed.isEmpty())
        emit cellsChanged(changed);
}

// --- Selection --------------------------------------------------------------

void BScheduleGridModel::setSelection(const BGridSelection &selection)
{
    if (selection == m_selection)
        return;
    m_selection = selection;
    emit selectionChanged();
}

// --- Edits ------------------------------------------------------------------

BGridChange BScheduleGridModel::failed(const QString &error) const
{
    BGridChange change;
    change.ok = false;
    change.error = error;
    change.before = snapshot();
    change.after = change.before;
    return change;
}

BGridChange BScheduleGridModel::applyBrush(const BGridBrush &brush, PaintMode mode)
{
    return paint(toWeekdays(m_selection.cells()), brush, mode);
}

BGridChange BScheduleGridModel::applyBrush(const BGridCellSet &cells, const BGridBrush &brush,
                                           PaintMode mode)
{
    return paint(toWeekdays(cells), brush, mode);
}

BGridChange BScheduleGridModel::erase()
{
    return eraseCells(toWeekdays(m_selection.cells()));
}

BGridChange BScheduleGridModel::erase(const BGridCellSet &cells)
{
    return eraseCells(toWeekdays(cells));
}

int BScheduleGridModel::removeCells(QList<Item> &items, int index, const BGridCellSet &cells)
{
    const BGridCellSet had = cellsOf(*items[index].line.spec);
    const BGridCellSet left = had.subtracted(cells);
    if (left == had)
        return 0;
    if (left.isEmpty()) {
        items.removeAt(index);
        return -1;
    }

    const QList<BGridProduct> products = left.products();
    Item &first = items[index];
    first.line.spec->mask.wday = products[0].rows;
    first.line.spec->mask.hour = products[0].columns;
    first.line.dirty = true;
    first.touched = true;
    first.text.clear();

    // Further products are new lines directly behind it
    for (int i = 1; i < products.size(); ++i) {
        Item piece;
        piece.line.spec = *items[index].line.spec;
        piece.line.spec->mask.wday = products[i].rows;
        piece.line.spec->mask.hour = products[i].columns;
        piece.line.dirty = true;
        piece.touched = true;
        piece.splitFrom = items[index].origin >= 0 ? items[index].origin : items[index].splitFrom;
        items.insert(index + i, piece);
    }
    return int(products.size()) - 1;
}

void BScheduleGridModel::mergeGenerated(QList<Item> &items, const BGridScope &scope)
{
    const auto candidate = [&scope](const Item &item) {
        return isGridLine(item.line) && isGenerated(item)
            && BGridScope::of(item.line.spec->mask) == scope;
    };

    bool merged = true;
    while (merged) {
        merged = false;
        for (int i = 0; i < items.size() && !merged; ++i) {
            if (!candidate(items[i]))
                continue;
            for (int j = i + 1; j < items.size(); ++j) {
                if (!candidate(items[j]) || (!items[i].touched && !items[j].touched))
                    continue;
                if (brushOf(*items[i].line.spec) != brushOf(*items[j].line.spec))
                    continue;
                const BGridCellSet a = cellsOf(*items[i].line.spec);
                const BGridCellSet b = cellsOf(*items[j].line.spec);
                BGridProduct product;
                // Overlapping lines run twice in the shared cells; one line cannot say that
                if (a.intersects(b) || !a.united(b).isProduct(&product))
                    continue;

                Item &keep = items[i];
                // The merged line stays a changed line if either was one
                if (keep.origin < 0 && items[j].origin >= 0) {
                    keep.origin = items[j].origin;
                    keep.splitFrom = -1;
                    keep.before = items[j].before;
                    keep.line.original = items[j].line.original;
                }
                keep.line.spec->mask.wday = product.rows;
                keep.line.spec->mask.hour = product.columns;
                keep.line.dirty = true;
                keep.touched = true;
                keep.text.clear();
                items.removeAt(j);
                merged = true;
                break;
            }
        }
    }
}

BGridChange BScheduleGridModel::paint(const BGridCellSet &weekdayCells, const BGridBrush &brush,
                                      PaintMode mode)
{
    const BGridScope scope = m_activeScopeValue;
    QList<Item> items = m_items;
    for (int i = 0; i < items.size(); ++i) {
        items[i].before = i;
        items[i].touched = false;
    }

    const auto inScope = [&scope](const Item &item) {
        return isGridLine(item.line) && BGridScope::of(item.line.spec->mask) == scope;
    };

    // The other brushes leave the painted cells
    if (mode == ReplaceRuns) {
        for (int i = 0; i < items.size(); ++i) {
            if (!inScope(items[i]) || brushOf(*items[i].line.spec) == brush)
                continue;
            i += removeCells(items, i, weekdayCells);
        }
    }

    // Cells that have the brush already are left as they are
    BGridCellSet missing = weekdayCells;
    for (const Item &item : std::as_const(items)) {
        if (inScope(item) && brushOf(*item.line.spec) == brush)
            missing = missing.subtracted(cellsOf(*item.line.spec));
    }

    const QList<BGridProduct> products = missing.products();
    for (const BGridProduct &product : products) {
        BRunSpec spec;
        spec.overrides = brush.overrides;
        spec.minute = brush.minute;
        spec.mask.wday = product.rows;
        spec.mask.hour = product.columns;
        spec.mask.month = scope.months;
        spec.mask.wom = scope.weeksOfMonth;
        spec.mask.last7DaysOfMonth = scope.last;

        Item item;
        item.line.spec = spec;
        item.line.dirty = true;
        item.touched = true;
        items.append(item);
    }

    mergeGenerated(items, scope);
    return commit(items);
}

BGridChange BScheduleGridModel::eraseCells(const BGridCellSet &weekdayCells)
{
    const BGridScope scope = m_activeScopeValue;
    QList<Item> items = m_items;
    for (int i = 0; i < items.size(); ++i) {
        items[i].before = i;
        items[i].touched = false;
    }
    for (int i = 0; i < items.size(); ++i) {
        if (!isGridLine(items[i].line) || BGridScope::of(items[i].line.spec->mask) != scope)
            continue;
        i += removeCells(items, i, weekdayCells);
    }
    mergeGenerated(items, scope);
    return commit(items);
}

BGridChange BScheduleGridModel::moveRun(int runIndex, int minute)
{
    if (runIndex < 0 || runIndex >= m_items.size())
        return failed(tr("There is no Run line %1.").arg(runIndex + 1));
    if (!isGridLine(m_items[runIndex].line)) {
        return failed(tr("Run line %1 is not a line of the grid and is not changed here.")
                          .arg(runIndex + 1));
    }

    QList<Item> items = m_items;
    for (int i = 0; i < items.size(); ++i) {
        items[i].before = i;
        items[i].touched = false;
    }
    Item &item = items[runIndex];
    if (item.line.spec->minute != minute) {
        item.line.spec->minute = minute;
        item.line.dirty = true;
        item.touched = true;
        item.text.clear();
    }
    return commit(items);
}

bool BScheduleGridModel::isCalendarRule(const BRunSpec &spec)
{
    return (spec.mask.mday & BDateTimeMask::AllMdays) != BDateTimeMask::AllMdays
        || (spec.mask.woy & BDateTimeMask::AllWoys) != BDateTimeMask::AllWoys;
}

BGridChange BScheduleGridModel::addCalendarRule(const BRunSpec &spec)
{
    if (!isCalendarRule(spec)) {
        return failed(tr("A run without days of the month and without weeks of the year "
                         "is not a calendar rule; it is painted in the grid."));
    }
    QList<Item> items = m_items;
    for (int i = 0; i < items.size(); ++i) {
        items[i].before = i;
        items[i].touched = false;
    }
    Item item;
    item.line.spec = spec;
    item.line.dirty = true;
    item.touched = true;
    items.append(item);
    return commit(items);
}

BGridChange BScheduleGridModel::changeCalendarRule(int runIndex, const BRunSpec &spec)
{
    if (runIndex < 0 || runIndex >= m_items.size())
        return failed(tr("There is no Run line %1.").arg(runIndex + 1));
    if (!m_items[runIndex].line.spec || isGridLine(m_items[runIndex].line))
        return failed(tr("Run line %1 is not a calendar rule.").arg(runIndex + 1));
    if (!isCalendarRule(spec)) {
        return failed(tr("A run without days of the month and without weeks of the year "
                         "is not a calendar rule; it is painted in the grid."));
    }
    QList<Item> items = m_items;
    for (int i = 0; i < items.size(); ++i) {
        items[i].before = i;
        items[i].touched = false;
    }
    Item &item = items[runIndex];
    if (*item.line.spec != spec) {
        item.line.spec = spec;
        item.line.dirty = true;
        item.touched = true;
        item.text.clear();
    }
    return commit(items);
}

BGridChange BScheduleGridModel::removeCalendarRule(int runIndex)
{
    if (runIndex < 0 || runIndex >= m_items.size())
        return failed(tr("There is no Run line %1.").arg(runIndex + 1));
    if (!m_items[runIndex].line.spec || isGridLine(m_items[runIndex].line))
        return failed(tr("Run line %1 is not a calendar rule.").arg(runIndex + 1));
    QList<Item> items = m_items;
    for (int i = 0; i < items.size(); ++i) {
        items[i].before = i;
        items[i].touched = false;
    }
    items.removeAt(runIndex);
    return commit(items);
}

BGridChange BScheduleGridModel::commit(QList<Item> items)
{
    // Every line the edit created or changed: back to the reported text if
    // it says the same again, otherwise written, read back and compared
    for (int i = 0; i < items.size(); ++i) {
        Item &item = items[i];
        if (!item.touched)
            continue;
        if (item.origin >= 0) {
            const BRunLine &loaded = m_loaded.runs[item.origin];
            if (loaded.spec && !loaded.dirty && !loaded.needsRewrite
                && *loaded.spec == *item.line.spec) {
                item.line = loaded;
                item.text = loaded.original;
                continue;
            }
        }
        QString error;
        const QString text = BRunWriter::lineText(item.line, &error);
        if (text.isEmpty())
            return failed(error);
        // The spec as the parser reads the text: the same run (lineText()
        // compared it), with the have* notes of the text that is written
        const BRunLine check = BRunParser::parse(text);
        if (!check.spec || *check.spec != *item.line.spec)
            return failed(tr("The generated Run line \"%1\" does not say what was intended.").arg(text));
        item.line.spec = check.spec;
        item.line.needsRewrite = false;
        item.line.parseError.clear();
        item.text = text;
    }

    BGridChange change;
    change.ok = true;
    change.before = snapshot();

    QList<bool> kept(m_items.size(), false);
    for (int i = 0; i < items.size(); ++i) {
        const Item &item = items[i];
        if (item.before < 0) {
            change.addedRuns.append(i);
            continue;
        }
        kept[item.before] = true;
        const BRunLine &old = m_items[item.before].line;
        if (item.touched && (old.dirty != item.line.dirty || *old.spec != *item.line.spec))
            change.changedRuns.append(i);
    }
    for (int i = 0; i < kept.size(); ++i) {
        if (!kept[i])
            change.removedRuns.append(i);
    }

    if (change.isNoop()) {
        change.after = change.before;
        return change;
    }

    for (Item &item : items) {
        item.before = -1;
        item.touched = false;
    }
    installItems(items, false);
    change.after = snapshot();
    return change;
}

BGridSnapshot BScheduleGridModel::snapshot() const
{
    BGridSnapshot result;
    result.runs.reserve(m_items.size());
    result.origins.reserve(m_items.size());
    result.splitFrom.reserve(m_items.size());
    for (const Item &item : m_items) {
        result.runs.append(item.line);
        result.origins.append(item.origin);
        result.splitFrom.append(item.splitFrom);
    }
    return result;
}

bool BScheduleGridModel::restore(const BGridSnapshot &snapshot)
{
    if (snapshot.runs.size() != snapshot.origins.size()
        || snapshot.runs.size() != snapshot.splitFrom.size()) {
        return false;
    }
    QList<Item> items;
    items.reserve(snapshot.runs.size());
    for (int i = 0; i < snapshot.runs.size(); ++i) {
        Item item;
        item.line = snapshot.runs[i];
        item.origin = snapshot.origins[i];
        item.splitFrom = snapshot.splitFrom[i];
        if (item.origin < -1 || item.origin >= m_loaded.runs.size()
            || item.splitFrom < -1 || item.splitFrom >= m_loaded.runs.size()) {
            return false;
        }
        if (item.line.spec) {
            item.text = BRunWriter::lineText(item.line);
            if (item.text.isEmpty())
                return false;
        }
        items.append(item);
    }
    installItems(items, false);
    return true;
}

void BScheduleGridModel::rebuildDerived()
{
    rebuildScopes();
    m_cells = computeCells();

    m_calendarRules.clear();
    m_notUnderstood.clear();
    for (int i = 0; i < m_items.size(); ++i) {
        const Item &item = m_items[i];
        if (!item.line.spec) {
            m_notUnderstood.append(BGridNotUnderstood{i, item.line.original, item.line.parseError});
        } else if (!isGridLine(item.line)) {
            const BRunSpec &spec = *item.line.spec;
            BGridCalendarRule rule;
            rule.runIndex = i;
            rule.spec = spec;
            rule.text = item.text;
            rule.byDayOfMonth = (spec.mask.mday & BDateTimeMask::AllMdays) != BDateTimeMask::AllMdays;
            rule.byWeekOfYear = (spec.mask.woy & BDateTimeMask::AllWoys) != BDateTimeMask::AllWoys;
            rule.withoutTime = !spec.haveHour;
            m_calendarRules.append(rule);
        }
    }
}

void BScheduleGridModel::installItems(const QList<Item> &items, bool reset)
{
    const Cells oldCells = m_cells;
    const QList<BGridScopeInfo> oldScopes = m_scopes;
    const int oldActive = m_activeScope;
    const QList<BGridCalendarRule> oldRules = m_calendarRules;
    const QList<BGridNotUnderstood> oldNotUnderstood = m_notUnderstood;
    const bool oldModified = isModified();

    m_items = items;
    rebuildDerived();

    if (reset) {
        const bool hadSelection = m_selection != BGridSelection();
        m_selection.clear();
        emit scheduleReset();
        if (hadSelection)
            emit selectionChanged();
        if (isModified() != oldModified)
            emit modifiedChanged(isModified());
        return;
    }

    QList<BGridPos> changed;
    for (int row = 0; row < 7; ++row) {
        const size_t weekday = size_t(weekdayOfRow(row));
        for (size_t hour = 0; hour < 24; ++hour) {
            if (oldCells[weekday][hour].inScope != m_cells[weekday][hour].inScope
                || oldCells[weekday][hour].outOfScope != m_cells[weekday][hour].outOfScope) {
                changed.append(BGridPos{row, int(hour)});
            }
        }
    }

    if (m_scopes != oldScopes)
        emit scopesChanged();
    if (m_activeScope != oldActive)
        emit activeScopeChanged(m_activeScope);
    if (!changed.isEmpty())
        emit cellsChanged(changed);
    if (m_calendarRules != oldRules)
        emit calendarRulesChanged();
    if (m_notUnderstood != oldNotUnderstood)
        emit notUnderstoodChanged();
    emit runLinesChanged();
    if (isModified() != oldModified)
        emit modifiedChanged(isModified());
}

// --- Run lines as text ------------------------------------------------------

QList<BGridRunLineText> BScheduleGridModel::runLines() const
{
    QList<bool> alive(m_loaded.runs.size(), false);
    for (const Item &item : m_items) {
        if (item.origin >= 0)
            alive[item.origin] = true;
    }

    // Where a line belongs among the loaded ones: its own place, or for a
    // line cut out of a loaded line the place of that line; -1 for a line
    // that was painted
    const auto placeOf = [](const Item &item) {
        return item.origin >= 0 ? item.origin : item.splitFrom;
    };
    // Per position: the highest place from there on
    QList<int> placeBehind(m_items.size() + 1, -1);
    for (int i = int(m_items.size()) - 1; i >= 0; --i)
        placeBehind[i] = qMax(placeBehind[i + 1], placeOf(m_items[i]));

    QList<BGridRunLineText> result;
    int nextLoaded = 0;
    // A removed line stands where it stood: in front of the next loaded line
    // that is still there (behind the lines cut out of itself), or, if none
    // follows, in front of the painted lines at the end
    const auto removedUpTo = [&](int end) {
        for (; nextLoaded < end; ++nextLoaded) {
            if (alive[nextLoaded])
                continue;
            const BRunLine &line = m_loaded.runs[nextLoaded];
            BGridRunLineText row;
            row.state = BGridRunLineText::Removed;
            row.text = line.original;
            row.originalText = line.original;
            row.originalIndex = nextLoaded;
            row.understood = line.understood();
            row.error = line.parseError;
            result.append(row);
        }
    };

    for (int i = 0; i < m_items.size(); ++i) {
        const Item &item = m_items[i];
        if (item.origin >= nextLoaded) {
            removedUpTo(item.origin);
            nextLoaded = item.origin + 1;
        } else if (item.origin < 0 && item.splitFrom >= nextLoaded) {
            // What is left of a loaded line that is gone itself
            removedUpTo(item.splitFrom + 1);
        } else if (placeOf(item) < 0 && placeBehind[i] < nextLoaded) {
            removedUpTo(int(m_loaded.runs.size()));
        }
        BGridRunLineText row;
        row.runIndex = i;
        row.originalIndex = item.origin;
        row.understood = item.line.understood();
        row.error = item.line.parseError;
        row.text = row.understood ? item.text : item.line.original;
        if (item.origin < 0) {
            row.state = BGridRunLineText::New;
        } else {
            const BRunLine &loaded = m_loaded.runs[item.origin];
            row.originalText = loaded.original;
            row.state = (item.line.dirty != loaded.dirty) ? BGridRunLineText::Changed
                                                          : BGridRunLineText::Untouched;
            row.respelled = row.state == BGridRunLineText::Untouched && item.line.needsRewrite;
        }
        result.append(row);
    }
    removedUpTo(int(m_loaded.runs.size()));
    return result;
}

bool BScheduleGridModel::runTexts(QStringList *texts, QString *error) const
{
    if (texts)
        texts->clear();
    if (!canBeWritten(error))
        return false;
    if (texts) {
        for (const Item &item : m_items)
            texts->append(item.text);
    }
    return true;
}
