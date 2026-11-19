/**
 * @file bscheduleminicalendar.cpp
 * @brief Five weeks of concrete dates with the levels that run on them
 */

#include "schedules/bscheduleminicalendar.h"

#include <QHelpEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>

namespace {

const int MaxChips = 3;

QColor mix(const QColor &a, const QColor &b, double share)
{
    return QColor(qRound(a.red() + (b.red() - a.red()) * share),
                  qRound(a.green() + (b.green() - a.green()) * share),
                  qRound(a.blue() + (b.blue() - a.blue()) * share));
}

} // namespace

BScheduleMiniCalendar::BScheduleMiniCalendar(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    showToday();
}

void BScheduleMiniCalendar::setSchedule(const BSchedule &schedule)
{
    m_schedule = schedule;
    rebuild();
}

void BScheduleMiniCalendar::setFirstDayOfWeek(Qt::DayOfWeek day)
{
    if (day == m_firstDay)
        return;
    // The week of today stays the week of today; any other stays the week
    // of the month it was moved to
    const bool atToday = m_first == BScheduleCalendar::weekStart(today(), m_firstDay);
    m_firstDay = day;
    if (atToday)
        showToday();
    else
        setFirstDate(BScheduleCalendar::weekStart(m_first.addDays(6), m_firstDay));
}

void BScheduleMiniCalendar::setLevelColorSource(const BScheduleGridWidget::LevelColorSource &source)
{
    m_levelColors = source;
    update();
}

void BScheduleMiniCalendar::setToday(const QDate &today)
{
    m_today = today;
    showToday();
}

QDate BScheduleMiniCalendar::today() const
{
    return m_today.isValid() ? m_today : QDate::currentDate();
}

void BScheduleMiniCalendar::setFirstDate(const QDate &first)
{
    m_first = first;
    if (!m_current.isValid() || m_current < m_first || m_current > lastDate())
        m_current = today() >= m_first && today() <= lastDate() ? today() : m_first;
    rebuild();
    emit rangeChanged();
}

void BScheduleMiniCalendar::rebuild()
{
    m_days = BScheduleCalendar::days(m_schedule, m_first);
    setAccessibleName(tr("Calendar of runs, %1").arg(dayText(m_current)));
    update();
}

void BScheduleMiniCalendar::showToday()
{
    m_current = today();
    setFirstDate(BScheduleCalendar::weekStart(today(), m_firstDay));
}

void BScheduleMiniCalendar::showNextMonth()
{
    setFirstDate(BScheduleCalendar::nextMonth(m_first, m_firstDay));
}

void BScheduleMiniCalendar::showPreviousMonth()
{
    setFirstDate(BScheduleCalendar::previousMonth(m_first, m_firstDay));
}

void BScheduleMiniCalendar::setCurrentDate(const QDate &date)
{
    if (!date.isValid() || date < m_first || date > lastDate() || date == m_current)
        return;
    m_current = date;
    setAccessibleName(tr("Calendar of runs, %1").arg(dayText(m_current)));
    update();
}

QString BScheduleMiniCalendar::rangeText() const
{
    return tr("%1 to %2").arg(m_first.toString(Qt::ISODate), lastDate().toString(Qt::ISODate));
}

QString BScheduleMiniCalendar::dayText(const QDate &date) const
{
    const QString head = QStringLiteral("%1 %2").arg(
        BScheduleGridText::dayName(Qt::DayOfWeek(date.dayOfWeek())), date.toString(Qt::ISODate));
    const qint64 index = m_first.daysTo(date);
    if (index < 0 || index >= m_days.size() || m_days[int(index)].runs.isEmpty())
        return tr("%1: no run").arg(head);
    QStringList runs;
    for (const BScheduleCalendarRun &run : m_days[int(index)].runs) {
        runs.append(tr("%1 %2 (Run line %3)")
                        .arg(run.when.time().toString(QStringLiteral("HH:mm")),
                             BScheduleGridText::levelName(run.level))
                        .arg(run.run + 1));
    }
    return QStringLiteral("%1: %2").arg(head, runs.join(QLatin1String(", ")));
}

int BScheduleMiniCalendar::headerHeight() const
{
    return fontMetrics().height() + 6;
}

QRect BScheduleMiniCalendar::dayRect(const QDate &date) const
{
    const qint64 index = m_first.daysTo(date);
    if (index < 0 || index >= BScheduleCalendar::Days)
        return QRect();
    const int cellWidth = qMax(1, width() / 7);
    const int cellHeight = qMax(1, (height() - headerHeight()) / BScheduleCalendar::Weeks);
    return QRect(int(index % 7) * cellWidth, headerHeight() + int(index / 7) * cellHeight,
                 cellWidth, cellHeight);
}

QDate BScheduleMiniCalendar::dateAt(const QPoint &pos) const
{
    const int cellWidth = qMax(1, width() / 7);
    const int cellHeight = qMax(1, (height() - headerHeight()) / BScheduleCalendar::Weeks);
    if (pos.x() < 0 || pos.y() < headerHeight())
        return QDate();
    const int column = pos.x() / cellWidth;
    const int row = (pos.y() - headerHeight()) / cellHeight;
    if (column >= 7 || row >= BScheduleCalendar::Weeks)
        return QDate();
    return m_first.addDays(row * 7 + column);
}

QSize BScheduleMiniCalendar::sizeHint() const
{
    return QSize(7 * 52, headerHeight() + BScheduleCalendar::Weeks * 40);
}

QSize BScheduleMiniCalendar::minimumSizeHint() const
{
    return QSize(7 * 34, headerHeight() + BScheduleCalendar::Weeks * 30);
}

void BScheduleMiniCalendar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    const QColor base = palette().color(QPalette::Base);
    const QColor alternate = palette().color(QPalette::AlternateBase);
    const QColor text = palette().color(QPalette::Text);
    const QColor line = mix(base, text, 0.28);
    painter.fillRect(rect(), palette().color(QPalette::Window));

    QFont small = font();
    if (small.pointSizeF() > 0)
        small.setPointSizeF(qMax(6.5, small.pointSizeF() * 0.8));
    QFont smallBold = small;
    smallBold.setBold(true);
    const QFontMetrics smallMetrics(small);

    // The days of the week
    const int cellWidth = qMax(1, width() / 7);
    painter.setFont(small);
    painter.setPen(text);
    for (int column = 0; column < 7; ++column) {
        const QDate date = m_first.addDays(column);
        painter.drawText(QRect(column * cellWidth, 0, cellWidth, headerHeight()), Qt::AlignCenter,
                         BScheduleGridText::shortDayName(Qt::DayOfWeek(date.dayOfWeek())));
    }

    const QDate now = today();
    for (int i = 0; i < m_days.size(); ++i) {
        const BScheduleCalendarDay &day = m_days[i];
        const QRect cell = dayRect(day.date);
        const QRect inner = cell.adjusted(0, 0, -1, -1);

        // Months alternate, so that the 1st is found at a glance
        painter.fillRect(inner, day.date.month() % 2 ? base : alternate);
        painter.setPen(line);
        painter.drawRect(inner);

        // The day; with the month on the 1st and on the first date shown
        QString label = QString::number(day.date.day());
        const bool named = day.date.day() == 1 || i == 0;
        if (named) {
            label = QStringLiteral("%1 %2").arg(day.date.day())
                        .arg(BScheduleGridText::monthName(day.date.month()).left(3));
        }
        painter.setFont(named || day.date == now ? smallBold : small);
        painter.setPen(day.date < now ? mix(text, base, 0.35) : text);
        painter.drawText(inner.adjusted(3, 1, -2, 0), Qt::AlignLeft | Qt::AlignTop, label);

        // The levels of the day
        const QList<std::optional<QString>> levels = day.levels();
        // As many squares as fit; the last place says how many more there are
        const int chip = qBound(12, inner.height() - smallMetrics.height() - 5, 16);
        const int room = qMax(1, qMin(MaxChips, (inner.width() - 4) / (chip + 2)));
        const bool more = levels.size() > room;
        const int shown = more ? room - 1 : int(levels.size());
        int x = inner.left() + 3;
        const int y = inner.bottom() - chip - 2;
        painter.setFont(smallBold);
        for (int k = 0; k < shown; ++k) {
            const QColor fill = BScheduleGridWidget::levelFill(m_levelColors, palette(), levels[k]);
            const QRect square(x, y, chip, chip);
            painter.fillRect(square, fill);
            painter.setPen(mix(fill, text, 0.5));
            painter.drawRect(square.adjusted(0, 0, -1, -1));
            painter.setPen(BScheduleGridWidget::inkOn(fill));
            painter.drawText(square, Qt::AlignCenter, BScheduleGridText::levelLetter(levels[k]).left(1));
            x += chip + 2;
        }
        if (more) {
            painter.setPen(text);
            painter.drawText(QRect(x, y, inner.right() - x, chip), Qt::AlignLeft | Qt::AlignVCenter,
                             QStringLiteral("+%1").arg(levels.size() - shown));
        }

        if (day.date == now) {
            painter.setPen(QPen(text, 2));
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(inner.adjusted(1, 1, -1, -1));
        }
        if (hasFocus() && day.date == m_current) {
            painter.setPen(QPen(text, 1, Qt::DotLine));
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(inner.adjusted(3, 3, -3, -3));
        }
    }
}

void BScheduleMiniCalendar::keyPressEvent(QKeyEvent *event)
{
    QDate target = m_current;
    switch (event->key()) {
    case Qt::Key_Left: target = m_current.addDays(-1); break;
    case Qt::Key_Right: target = m_current.addDays(1); break;
    case Qt::Key_Up: target = m_current.addDays(-7); break;
    case Qt::Key_Down: target = m_current.addDays(7); break;
    case Qt::Key_PageUp: showPreviousMonth(); return;
    case Qt::Key_PageDown: showNextMonth(); return;
    case Qt::Key_Home: showToday(); return;
    default:
        QWidget::keyPressEvent(event);
        return;
    }
    setCurrentDate(target);
}

void BScheduleMiniCalendar::mousePressEvent(QMouseEvent *event)
{
    const QDate date = dateAt(event->position().toPoint());
    if (date.isValid())
        setCurrentDate(date);
    QWidget::mousePressEvent(event);
}

bool BScheduleMiniCalendar::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        auto *help = static_cast<QHelpEvent *>(event);
        const QDate date = dateAt(help->pos());
        if (date.isValid()) {
            QToolTip::showText(help->globalPos(), dayText(date), this, dayRect(date));
        } else {
            QToolTip::hideText();
            event->ignore();
        }
        return true;
    }
    if (event->type() == QEvent::FocusIn || event->type() == QEvent::FocusOut)
        update();
    return QWidget::event(event);
}
