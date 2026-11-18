#ifndef BSCHEDULEDRAGPLAN_H
#define BSCHEDULEDRAGPLAN_H

#include <QList>
#include <QString>
#include <QStringList>

#include "models/bresourcemodels.h"

/**
 * @brief Decides whether a dragged time change can be written as a schedule
 *
 * The Run lines of the new schedule are the Director's own lines, passed
 * through verbatim. Only the dragged line is generated: its parsed run
 * (BRunSpec) with the new time, written by BRunWriter and checked by
 * reading it back. Lines are compared as parsed runs, never as text.
 *
 * Plain functions without widgets and without a Director, so that the
 * decision can be tested on its own.
 */
namespace BScheduleDragPlan {

enum class Verdict {
    Exact,              ///< Every line is the original one; the dragged line differs in its time only
    NeedsConfirmation,  ///< Some untouched lines had to be respelled; show both versions and ask
    Refused             ///< Nothing may be written; see Plan::reason
};

enum class Reason {
    None,
    NoRunLines,             ///< The original Run lines of the schedule are not known
    NotUnderstood,          ///< A Run line that the parser does not understand
    SeveralHours,           ///< The dragged line runs at several hours and has one minute for all
    BadTime,                ///< The new time is not a time of day
    DraggedLineNotFound,    ///< No original line matches the dragged entry
    DraggedLineAmbiguous,   ///< Several original lines match the dragged entry and its position is not known
    WriteCheckFailed        ///< A generated line did not read back as intended
};

struct Plan {
    Verdict verdict = Verdict::Refused;
    Reason reason = Reason::None;
    int reasonLine = -1;        ///< Index into originalRuns, -1 if not tied to a line
    QString reasonText;         ///< The parser's or writer's message, if any

    QStringList originalRuns;   ///< As passed in
    QStringList generatedRuns;  ///< One per original line; empty when refused
    int draggedLine = -1;       ///< Index of the line whose time changes
    QList<int> differingLines;  ///< Untouched lines whose text is not the original's

    bool refused() const { return verdict == Verdict::Refused; }
};

/**
 * @brief Do two Run lines say the same?
 * @param original   Run line as the Director reported it
 * @param generated  Another Run line
 * @param newHour    If >= 0: the time of @p original is replaced by
 *                   newHour:newMinute before comparing (the dragged line)
 * @param newMinute  See @p newHour
 * @return true if both lines are understood and their overrides, masks and
 *         minutes agree. A line that is not understood equals nothing.
 */
bool sameRun(const QString &original, const QString &generated,
             int newHour = -1, int newMinute = -1);

/**
 * @brief The Run lines of a schedule after a drag
 * @param originalRuns Run lines of the schedule as the Director reported them
 * @param dragged      The dragged entry with its time before the drag. Its
 *                     runIndex, if set, names the line; otherwise the line
 *                     is found by what it says.
 * @param newHour      Hour after the drag
 * @param newMinute    Minute after the drag
 *
 * One line per original line, independent of the number of jobs that use
 * the schedule. Refused if any line is not understood: such a schedule
 * cannot be written by the application.
 */
Plan plan(const QStringList &originalRuns, const BScheduleEntry &dragged,
          int newHour, int newMinute);

} // namespace BScheduleDragPlan

#endif // BSCHEDULEDRAGPLAN_H
