#ifndef BWEEKLYPLANNER_H
#define BWEEKLYPLANNER_H

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <QJsonObject>

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
 */
class BWeeklyPlanner : public QWidget
{
    Q_OBJECT

public:
    explicit BWeeklyPlanner(QWidget *parent = nullptr);
    ~BWeeklyPlanner();

    /**
     * @brief Sets the schedule data to display
     * @param schedule JSON object containing schedule information
     */
    void setSchedule(const QJsonObject &schedule);

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
        LEVEL_INCREMENTAL = 3
    };

    /**
     * @brief Parses Bareos schedule syntax and marks cells
     * @param scheduleRun Schedule run specification string
     */
    void parseScheduleRun(const QString &scheduleRun);

    /**
     * @brief Extracts backup level from schedule string
     */
    BackupLevel extractLevel(const QString &scheduleRun) const;

    /**
     * @brief Extracts time (hour and minute) from schedule string
     */
    bool extractTime(const QString &scheduleRun, int &hour, int &minute) const;

    /**
     * @brief Extracts days from schedule string
     */
    QVector<int> extractDays(const QString &scheduleRun) const;

    /**
     * @brief Draws the grid with day labels and hour markers
     */
    void drawGrid(QPainter &painter);

    /**
     * @brief Draws the scheduled time blocks
     */
    void drawSchedule(QPainter &painter);

    /**
     * @brief Gets color for backup level
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

    static const int CELL_WIDTH = 30;
    static const int CELL_HEIGHT = 30;
    static const int HEADER_HEIGHT = 40;
    static const int DAY_LABEL_WIDTH = 100;
    static const int DAYS = 7;
    static const int HOURS = 24;

    // Schedule data: [day][hour] = level
    BackupLevel m_schedule[DAYS][HOURS];

    QJsonObject m_scheduleData;
    QString m_scheduleName;

    // Hover state
    int m_hoverDay;
    int m_hoverHour;

    // Day names
    QStringList m_dayNames;
};

#endif // BWEEKLYPLANNER_H
