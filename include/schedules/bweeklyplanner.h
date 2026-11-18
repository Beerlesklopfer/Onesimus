#ifndef BWEEKLYPLANNER_H
#define BWEEKLYPLANNER_H

#include <QWidget>
#include <QDate>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include "models/bresourcemodels.h"

/**
 * @brief Widget displaying a weekly schedule planner
 * @since 2.7
 *
 * Visual representation of schedule times across a week.
 * Similar to Windows logon hours configuration grid.
 *
 * Features:
 * - 7 days (rows) x 24 hours (columns)
 * - Visual representation of scheduled backup times
 * - Hover effects and tooltips
 *
 * The grid shows one concrete week, Monday to Sunday: a cell is filled if a
 * run starts in that hour of that date. A run restricted to a week of the
 * month, a day of the month, a month or a week of the year is therefore
 * shown only in a week in which it runs.
 */
class BWeeklyPlanner : public QWidget
{
    Q_OBJECT

public:
    explicit BWeeklyPlanner(QWidget *parent = nullptr);
    ~BWeeklyPlanner();

    /**
     * @brief Sets the week to display and what starts in it
     * @param monday  First day of the week; any other day of the week
     *                selects the week it is in
     * @param entries Entries with their starts in that week
     *                (BScheduleEntry::fromOccurrences()); starts outside
     *                the week are not shown
     */
    void setWeek(const QDate &monday, const QList<BScheduleEntry> &entries);

    /// The Monday of the week on display
    QDate weekStart() const { return m_weekStart; }

    /**
     * @brief Clears the schedule display
     */
    void clearSchedule();

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    enum BackupLevel {
        LEVEL_NONE = 0,
        LEVEL_FULL = 1,
        LEVEL_DIFFERENTIAL = 2,
        LEVEL_INCREMENTAL = 3,
        LEVEL_JOB = 4,      ///< The Run line names no level: the job's level applies
        LEVEL_OTHER = 5     ///< A level without a colour here (Base, Catalog, ...)
    };

    /**
     * @brief Draws the grid with day labels and hour markers
     */
    void drawGrid(QPainter &painter);

    /**
     * @brief Draws the scheduled time blocks
     */
    void drawSchedule(QPainter &painter);

    /**
     * @brief Gets color for backup level (from settings)
     */
    QColor getColorForLevel(BackupLevel level, bool hovered = false) const;

    /**
     * @brief Gets cell position for given day and hour
     */
    QRect getCellRect(int day, int hour) const;

    /**
     * @brief Gets day and hour from mouse position
     */
    bool getDayHourFromPos(const QPoint &pos, int &day, int &hour) const;

    static constexpr int MIN_CELL_WIDTH = 30;
    static constexpr int MIN_CELL_HEIGHT = 30;
    static constexpr int HEADER_HEIGHT = 40;
    static constexpr int DAY_LABEL_WIDTH = 100;
    static constexpr int DAYS = 7;
    static constexpr int HOURS = 24;

    // Dynamic cell size (computed from widget size)
    int cellWidth() const;
    int cellHeight() const;

    // Schedule data: [day][hour] = list of levels (for side-by-side display)
    QVector<BackupLevel> m_schedule[DAYS][HOURS];

    QString m_scheduleName;

    // Monday of the week on display
    QDate m_weekStart;

    // Hover state
    int m_hoverDay;
    int m_hoverHour;

    // Day names
    QStringList m_dayNames;
};

#endif // BWEEKLYPLANNER_H
