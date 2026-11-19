#ifndef BSCHEDULEGRIDTEXT_H
#define BSCHEDULEGRIDTEXT_H

#include <QCoreApplication>
#include <QDate>
#include <QDateTime>
#include <QList>
#include <QString>

#include <optional>

#include "schedules/model/bschedulegridmodel.h"

/**
 * @file bschedulegridtext.h
 * @brief What the schedule grid says in words, and the dates of its calendar
 *
 * Everything the grid view shows as text and reads to a screen reader is
 * made here, so that it can be tested without a window: the labels of the
 * scope bar, the names of cells, the legend of brushes, the plain-language
 * reading of calendar rules. BScheduleCalendar gives the levels that run on
 * concrete dates (BScheduleOccurrences) for the mini calendar.
 * QtCore only; no widgets and no Director.
 */
class BScheduleGridText
{
    Q_DECLARE_TR_FUNCTIONS(BScheduleGridText)

public:
    static QString dayName(Qt::DayOfWeek day);          ///< "Monday"
    static QString shortDayName(Qt::DayOfWeek day);     ///< "Mon"
    static QString monthName(int month);                ///< 1 = "January"
    static QString ordinal(int number);                 ///< "1st", "22nd", "31st"
    static QString timeText(int hour, int minute);      ///< "21:05"

    // --- Scopes ------------------------------------------------------------

    /// The label of the scope bar: "Every week", "1st", "2nd-5th", "last",
    /// "jan-mar", "jan-mar 1st". Weeks and months are spelled as in a Run line.
    static QString scopeLabel(const BGridScope &scope);

    /// The same, as short as it gets, for the badge on a run of another
    /// scope: "all", "1st", "2-5", "last", "jan-mar 1st"
    static QString scopeBadge(const BGridScope &scope);

    /// How a sentence says it: "first week only", "last week only",
    /// "only 2nd-5th"; empty for "every week"
    static QString scopePhrase(const BGridScope &scope);

    // --- Levels and brushes ------------------------------------------------

    /// "Full", ...; "level of the job" for a run without a level
    static QString levelName(const std::optional<QString> &level);

    /// The letter in a cell: F, D, I, V (VirtualFull), B (Base), the first two
    /// letters of any other level, J for a run without a level
    static QString levelLetter(const std::optional<QString> &level);

    /// The code BLevelColors knows (F, I, D, V); empty for every other level
    /// and for a run without one
    static QString levelColorCode(const std::optional<QString> &level);

    /// Is anything but the level overridden?
    static bool hasOverridesBesideLevel(const BRunOverrides &overrides);

    /// "pool Monthly, storage Tape, priority 5"; the level is left out
    static QString overridesText(const BRunOverrides &overrides);

    /// "Full at :05, pool Monthly"
    static QString brushText(const BGridBrush &brush);

    /// "Full at 21:05, pool Monthly"
    static QString runText(const BGridRun &run, int hour);

    /// The tag of the n-th brush with overrides: A ... Z, AA, AB, ...
    static QString brushTag(int index);

    /**
     * @brief The tags of brushes, by position in @p brushes
     *
     * A brush that overrides something beside the level gets the next tag;
     * every other brush an empty string.
     */
    static QStringList brushTags(const QList<BGridBrush> &brushes);

    // --- Cells -------------------------------------------------------------

    /// "Saturday 21 hours"
    static QString cellPlace(Qt::DayOfWeek day, int hour);

    /**
     * @brief The name of a cell for a screen reader
     *
     * "Saturday 21 hours: Full at 21:00, pool Monthly, first week only";
     * runs of other scopes follow after "; in other scopes:"; an empty cell
     * is "Saturday 21 hours: no run".
     */
    static QString cellName(const BScheduleGridModel &model, int row, int column);

    // --- Calendar rules ----------------------------------------------------

    /// "R1", "R2", ...: the mark of a calendar rule in the band below the grid
    static QString ruleTag(int index);

    /// "on the 1st of every month at 21:00"
    static QString ruleReading(const BGridCalendarRule &rule);
};

/// One start on a date of the calendar
struct BScheduleCalendarRun
{
    QDateTime when;
    int run = -1;                   ///< Index into BSchedule::runs
    std::optional<QString> level;   ///< Empty: the job's own level
};

struct BScheduleCalendarDay
{
    QDate date;
    QList<BScheduleCalendarRun> runs;   ///< By time, then by line

    /// The distinct levels of the day in the order of their first start
    QList<std::optional<QString>> levels() const;
};

/**
 * @brief The dates of the mini calendar beside the grid
 *
 * The starts are those of BScheduleOccurrences::occurrences() from the
 * start of the first day in local time, exactly as the timeline asks for
 * them, so both show the same runs for the same date.
 */
class BScheduleCalendar
{
public:
    static constexpr int Weeks = 5;
    static constexpr int Days = Weeks * 7;

    /// One entry per date from @p first on, also for a date without a run
    static QList<BScheduleCalendarDay> days(const BSchedule &schedule, const QDate &first,
                                            int count = Days);

    /// The first day of the week @p date is in
    static QDate weekStart(const QDate &date, Qt::DayOfWeek firstDayOfWeek);

    /// Where the calendar starts after "next month": the week with the 1st of
    /// the month after the one the first week of @p first belongs to
    static QDate nextMonth(const QDate &first, Qt::DayOfWeek firstDayOfWeek);

    /// Where it starts after "previous month": the week with the 1st of that
    /// month, or of the month before if the calendar is there already
    static QDate previousMonth(const QDate &first, Qt::DayOfWeek firstDayOfWeek);
};

#endif // BSCHEDULEGRIDTEXT_H
