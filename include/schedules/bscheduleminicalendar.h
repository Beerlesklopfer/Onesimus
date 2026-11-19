#ifndef BSCHEDULEMINICALENDAR_H
#define BSCHEDULEMINICALENDAR_H

#include <QDate>
#include <QWidget>

#include "schedules/bschedulegridwidget.h"
#include "schedules/model/bschedulegridtext.h"

/**
 * @brief Five weeks of concrete dates with the levels that run on them
 *
 * The check beside the schedule grid that "first Saturday" came out as
 * intended: for each of 35 dates the levels of the runs that start on it,
 * worked out by BScheduleOccurrences exactly as the timeline does it
 * (BScheduleCalendar::days()). A level is a small square in the level
 * colour with the letter of the level; up to three per day, then "+n".
 *
 * Starts with the current week and moves by month: to the week with the
 * 1st of the next or of the previous month. Arrow keys move the current
 * day, Page Up and Page Down the month, Home goes back to today. The
 * current day is read to a screen reader as the name of the widget.
 */
class BScheduleMiniCalendar : public QWidget
{
    Q_OBJECT

public:
    explicit BScheduleMiniCalendar(QWidget *parent = nullptr);

    void setSchedule(const BSchedule &schedule);
    void setFirstDayOfWeek(Qt::DayOfWeek day);
    void setLevelColorSource(const BScheduleGridWidget::LevelColorSource &source);

    /// The date taken as today; the current date unless set (tests, pictures)
    void setToday(const QDate &today);
    QDate today() const;

    QDate firstDate() const { return m_first; }
    QDate lastDate() const { return m_first.addDays(BScheduleCalendar::Days - 1); }
    const QList<BScheduleCalendarDay> &days() const { return m_days; }

    QDate currentDate() const { return m_current; }
    void setCurrentDate(const QDate &date);

    /// "2026-11-16 to 2026-12-20"
    QString rangeText() const;
    /// "Saturday 2026-12-05: 21:00 Full (Run line 1)"; "... : no run"
    QString dayText(const QDate &date) const;

    QRect dayRect(const QDate &date) const;
    QDate dateAt(const QPoint &pos) const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void showNextMonth();
    void showPreviousMonth();
    void showToday();

signals:
    void rangeChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    bool event(QEvent *event) override;

private:
    void setFirstDate(const QDate &first);
    void rebuild();
    int headerHeight() const;

    BSchedule m_schedule;
    Qt::DayOfWeek m_firstDay = Qt::Monday;
    BScheduleGridWidget::LevelColorSource m_levelColors;
    QDate m_today;      ///< Invalid: the current date
    QDate m_first;
    QDate m_current;
    QList<BScheduleCalendarDay> m_days;
};

#endif // BSCHEDULEMINICALENDAR_H
