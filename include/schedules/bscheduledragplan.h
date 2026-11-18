#ifndef BSCHEDULEDRAGPLAN_H
#define BSCHEDULEDRAGPLAN_H

#include <QList>
#include <QString>
#include <QStringList>

#include "models/bresourcemodels.h"

/**
 * @brief Decides whether a dragged time change can be written as a schedule
 *
 * The Gantt view works on BScheduleEntry objects, and the Run lines of a new
 * schedule are regenerated from them. BScheduleEntry::fromRunDirective()
 * does not keep every form a Bareos Run line can have, so the regenerated
 * lines are compared here with the Director's original ones before anything
 * is offered for writing.
 *
 * Plain functions without widgets and without a Director, so that the
 * comparison can be tested on its own. This is a check, not a second parser:
 * it only has to notice that two lines do not say the same.
 */
namespace BScheduleDragPlan {

enum class Verdict {
    Exact,              ///< Only the dragged time differs from the original lines
    NeedsConfirmation,  ///< Other differences exist; show both versions and ask
    Refused             ///< Nothing may be written; see Plan::reason
};

enum class Reason {
    None,
    NoRunLines,             ///< The original Run lines of the schedule are not known
    Override,               ///< pool=, storage=, ... on a Run line
    WeekOfMonth,            ///< 1st, 2nd-5th, last, ... not kept by the regeneration
    DayRange,               ///< tue-thu, ... not kept by the regeneration
    DayOfMonth,             ///< Day-of-month numbers
    NoTime,                 ///< No readable "at HH:MM" on a Run line
    DraggedLineNotFound,    ///< No original line matches the dragged entry
    DraggedLineAmbiguous    ///< Several original lines match the dragged entry
};

struct Plan {
    Verdict verdict = Verdict::Refused;
    Reason reason = Reason::None;
    int reasonLine = -1;        ///< Index into originalRuns, -1 if not tied to a line
    QString reasonToken;        ///< The token that caused the refusal, if any

    QStringList originalRuns;   ///< As passed in
    QStringList generatedRuns;  ///< One per original line; empty when refused
    int draggedLine = -1;       ///< Index of the line whose time changes
    QList<int> differingLines;  ///< Lines that differ in more than the dragged time

    bool refused() const { return verdict == Verdict::Refused; }
};

/**
 * @brief Do two Run lines say the same?
 * @param original   Run line as the Director reported it
 * @param generated  Regenerated Run line
 * @param newHour    If >= 0: the time of @p original is replaced by
 *                   newHour:newMinute before comparing (the dragged line)
 * @param newMinute  See @p newHour
 * @return true if level, week-of-month, days, time and all other tokens agree
 *
 * Case, spacing, the order of days, `level=Full` against `Full`, a missing
 * day against `daily` and `2:05` against `02:05` are not differences. A
 * line without a readable time never equals anything.
 */
bool sameRun(const QString &original, const QString &generated,
             int newHour = -1, int newMinute = -1);

/**
 * @brief Compare the regenerated Run lines of a schedule with the original ones
 * @param originalRuns Run lines of the schedule as the Director reported them
 * @param dragged      The dragged entry with its time before the drag
 * @param newHour      Hour after the drag
 * @param newMinute    Minute after the drag
 *
 * The lines are regenerated from @p originalRuns (one per line, independent
 * of the number of jobs that use the schedule) through
 * BScheduleEntry::fromRunDirective() and toRunDirective().
 */
Plan plan(const QStringList &originalRuns, const BScheduleEntry &dragged,
          int newHour, int newMinute);

} // namespace BScheduleDragPlan

#endif // BSCHEDULEDRAGPLAN_H
