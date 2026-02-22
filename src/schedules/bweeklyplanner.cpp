#include "schedules/bweeklyplanner.h"
#include "jobs/blevelcolors.h"
#include "blogging.h"
#include <QPainter>
#include <QToolTip>
#include <QJsonArray>
#include <QRegularExpression>

BWeeklyPlanner::BWeeklyPlanner(QWidget *parent)
    : QWidget(parent)
    , m_hoverDay(-1)
    , m_hoverHour(-1)
{
    // QVector<BackupLevel> arrays are empty by default — nothing to initialize

    m_dayNames << tr("Monday") << tr("Tuesday") << tr("Wednesday") << tr("Thursday")
               << tr("Friday") << tr("Saturday") << tr("Sunday");

    setMouseTracking(true);
    setMinimumSize(DAY_LABEL_WIDTH + HOURS * MIN_CELL_WIDTH + 20,
                   HEADER_HEIGHT + DAYS * MIN_CELL_HEIGHT + 20);
}

BWeeklyPlanner::~BWeeklyPlanner()
{
}

QSize BWeeklyPlanner::sizeHint() const
{
    return QSize(
        DAY_LABEL_WIDTH + (HOURS * MIN_CELL_WIDTH) + 20,
        HEADER_HEIGHT + (DAYS * MIN_CELL_HEIGHT) + 20
    );
}

int BWeeklyPlanner::cellWidth() const
{
    int available = width() - DAY_LABEL_WIDTH - 10;
    return qMax(MIN_CELL_WIDTH, available / HOURS);
}

int BWeeklyPlanner::cellHeight() const
{
    int available = height() - HEADER_HEIGHT - 10;
    return qMax(MIN_CELL_HEIGHT, available / DAYS);
}

void BWeeklyPlanner::setSchedule(const QJsonObject &schedule)
{
    m_scheduleData = schedule;
    m_scheduleName = schedule["name"].toString();

    // Clear existing schedule
    for (int day = 0; day < DAYS; ++day) {
        for (int hour = 0; hour < HOURS; ++hour) {
            m_schedule[day][hour].clear();
        }
    }

    // Parse schedule runs (Bareos API may use "run" or "Run")
    QJsonArray runs = schedule["run"].toArray();
    if (runs.isEmpty()) {
        runs = schedule["Run"].toArray();
    }
    for (const QJsonValue &runVal : runs) {
        if (runVal.isString()) {
            parseScheduleRun(runVal.toString());
        }
    }

    update();
}

void BWeeklyPlanner::setAllSchedules(const QJsonArray &schedules)
{
    m_scheduleData = QJsonObject();
    m_scheduleName = tr("All Schedules");

    // Clear existing
    for (int day = 0; day < DAYS; ++day) {
        for (int hour = 0; hour < HOURS; ++hour) {
            m_schedule[day][hour].clear();
        }
    }

    // Parse all schedules
    for (const QJsonValue &scheduleVal : schedules) {
        if (!scheduleVal.isObject()) continue;
        QJsonObject schedule = scheduleVal.toObject();

        QJsonArray runs = schedule["run"].toArray();
        if (runs.isEmpty()) {
            runs = schedule["Run"].toArray();
        }
        for (const QJsonValue &runVal : runs) {
            if (runVal.isString()) {
                parseScheduleRun(runVal.toString());
            }
        }
    }

    update();
}

void BWeeklyPlanner::setEntries(const QList<BScheduleEntry> &entries)
{
    m_scheduleData = QJsonObject();
    m_scheduleName = tr("All Schedules");

    // Clear existing
    for (int day = 0; day < DAYS; ++day) {
        for (int hour = 0; hour < HOURS; ++hour) {
            m_schedule[day][hour].clear();
        }
    }

    // Map BScheduleEntry::BackupLevel to our BackupLevel enum
    for (const BScheduleEntry &entry : entries) {
        BackupLevel level;
        switch (entry.level) {
        case BScheduleEntry::Full:
        case BScheduleEntry::VirtualFull:
            level = LEVEL_FULL;
            break;
        case BScheduleEntry::Differential:
            level = LEVEL_DIFFERENTIAL;
            break;
        case BScheduleEntry::Incremental:
        default:
            level = LEVEL_INCREMENTAL;
            break;
        }

        int hour = entry.hour;
        if (hour < 0 || hour >= HOURS) continue;

        for (int day : entry.daysOfWeek) {
            if (day >= 0 && day < DAYS) {
                // Append each level — side-by-side display
                m_schedule[day][hour].append(level);
            }
        }
    }

    update();
}

void BWeeklyPlanner::clearSchedule()
{
    m_scheduleData = QJsonObject();
    m_scheduleName.clear();

    for (int day = 0; day < DAYS; ++day) {
        for (int hour = 0; hour < HOURS; ++hour) {
            m_schedule[day][hour].clear();
        }
    }

    update();
}

void BWeeklyPlanner::parseScheduleRun(const QString &scheduleRun)
{
    // Bareos schedule syntax examples:
    // "Full 1st sun at 23:05"
    // "Level=Differential 2nd-5th sun at 23:05"
    // "Level=Incremental mon-sat at 23:05"

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "Parsing schedule run:" << scheduleRun;
#endif

    // Extract backup level
    BackupLevel level = extractLevel(scheduleRun);

    // Extract time
    int hour, minute;
    if (!extractTime(scheduleRun, hour, minute)) {
        return;
    }

    // Extract days
    QVector<int> days = extractDays(scheduleRun);

    // Mark schedule cells — append for side-by-side display
    for (int day : days) {
        if (day >= 0 && day < DAYS && hour >= 0 && hour < HOURS) {
            m_schedule[day][hour].append(level);
        }
    }
}

BWeeklyPlanner::BackupLevel BWeeklyPlanner::extractLevel(const QString &scheduleRun) const
{
    QString lower = scheduleRun.toLower();

    if (lower.contains("level=differential") || lower.contains("differential")) {
        return LEVEL_DIFFERENTIAL;
    } else if (lower.contains("level=incremental") || lower.contains("incremental")) {
        return LEVEL_INCREMENTAL;
    } else if (lower.contains("full")) {
        return LEVEL_FULL;
    }

    // Default to incremental if no level specified
    return LEVEL_INCREMENTAL;
}

bool BWeeklyPlanner::extractTime(const QString &scheduleRun, int &hour, int &minute) const
{
    // Extract time (format: "at HH:MM" or "at H:MM")
    QRegularExpression timeRx("at\\s+(\\d{1,2}):(\\d{2})");
    QRegularExpressionMatch match = timeRx.match(scheduleRun);

    if (match.hasMatch()) {
        hour = match.captured(1).toInt();
        minute = match.captured(2).toInt();

        if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60) {
            return true;
        }
    }

    return false;
}

QVector<int> BWeeklyPlanner::extractDays(const QString &scheduleRun) const
{
    QString lower = scheduleRun.toLower();
    QVector<int> days;

    // Handle "daily" - all days
    if (lower.contains("daily")) {
        for (int i = 0; i < 7; ++i) {
            days.append(i);
        }
        return days;
    }

    // Handle "hourly" - all days (hourly means every hour of every day)
    if (lower.contains("hourly")) {
        for (int i = 0; i < 7; ++i) {
            days.append(i);
        }
        return days;
    }

    // Handle day ranges first (before individual days to avoid conflicts)
    if (lower.contains("mon-sat")) {
        for (int i = 0; i < 6; ++i) {
            days.append(i);
        }
        return days;
    }

    if (lower.contains("mon-fri")) {
        for (int i = 0; i < 5; ++i) {
            days.append(i);
        }
        return days;
    }

    // Handle week specifications (1st, 2nd-5th, last, etc.) with specific day
    // For now, we'll mark all occurrences of that day in the month
    // A more sophisticated parser would need calendar logic

    // Check for specific weekday names
    if (lower.contains("mon")) days.append(0);
    if (lower.contains("tue")) days.append(1);
    if (lower.contains("wed")) days.append(2);
    if (lower.contains("thu")) days.append(3);
    if (lower.contains("fri")) days.append(4);
    if (lower.contains("sat")) days.append(5);
    if (lower.contains("sun")) days.append(6);

    return days;
}

QColor BWeeklyPlanner::getColorForLevel(BackupLevel level, bool hovered) const
{
    QString code;
    switch (level) {
        case LEVEL_FULL:         code = "F"; break;
        case LEVEL_DIFFERENTIAL: code = "D"; break;
        case LEVEL_INCREMENTAL:  code = "I"; break;
        case LEVEL_NONE:
        default:
            return Qt::transparent;
    }

    QColor base = BLevelColors::getLevelColor(code);
    if (hovered)
        return base.lighter(130);
    return base;
}

void BWeeklyPlanner::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawGrid(painter);
    drawSchedule(painter);
}

void BWeeklyPlanner::drawGrid(QPainter &painter)
{
    int cw = cellWidth();
    int ch = cellHeight();

    painter.setPen(QPen(Qt::gray, 1));

    // Draw day labels
    for (int day = 0; day < DAYS; ++day) {
        QRect labelRect(10, HEADER_HEIGHT + day * ch, DAY_LABEL_WIDTH - 10, ch);
        painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, m_dayNames[day]);
    }

    // Draw hour labels
    painter.save();
    painter.setPen(QPen(Qt::darkGray, 1));
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    // Determine label interval based on cell width
    int labelInterval = 1;
    if (cw < 25) labelInterval = 4;
    else if (cw < 35) labelInterval = 2;

    for (int hour = 0; hour < HOURS; hour += labelInterval) {
        int x = DAY_LABEL_WIDTH + hour * cw;
        QRect headerRect(x, 10, cw * labelInterval, HEADER_HEIGHT - 10);
        painter.drawText(headerRect, Qt::AlignCenter, QString("%1").arg(hour, 2, 10, QChar('0')));
    }
    painter.restore();

    // Draw grid lines
    painter.setPen(QPen(Qt::lightGray, 1));

    // Horizontal lines
    for (int day = 0; day <= DAYS; ++day) {
        int y = HEADER_HEIGHT + day * ch;
        painter.drawLine(DAY_LABEL_WIDTH, y, DAY_LABEL_WIDTH + HOURS * cw, y);
    }

    // Vertical lines
    for (int hour = 0; hour <= HOURS; ++hour) {
        int x = DAY_LABEL_WIDTH + hour * cw;
        painter.drawLine(x, HEADER_HEIGHT, x, HEADER_HEIGHT + DAYS * ch);
    }
}

void BWeeklyPlanner::drawSchedule(QPainter &painter)
{
    for (int day = 0; day < DAYS; ++day) {
        for (int hour = 0; hour < HOURS; ++hour) {
            const QVector<BackupLevel> &levels = m_schedule[day][hour];
            if (levels.isEmpty()) continue;

            QRect cellRect = getCellRect(day, hour);
            bool isHovered = (day == m_hoverDay && hour == m_hoverHour);

            if (levels.size() == 1) {
                // Single level — fill entire cell
                painter.fillRect(cellRect, getColorForLevel(levels[0], isHovered));
            } else {
                // Multiple levels — split cell horizontally side-by-side
                int count = levels.size();
                int sliceWidth = cellRect.width() / count;
                int remainder = cellRect.width() % count;

                int x = cellRect.x();
                for (int i = 0; i < count; ++i) {
                    int w = sliceWidth + (i < remainder ? 1 : 0);
                    QRect slice(x, cellRect.y(), w, cellRect.height());
                    painter.fillRect(slice, getColorForLevel(levels[i], isHovered));
                    x += w;
                }
            }
        }
    }
}

QRect BWeeklyPlanner::getCellRect(int day, int hour) const
{
    int cw = cellWidth();
    int ch = cellHeight();
    int x = DAY_LABEL_WIDTH + hour * cw;
    int y = HEADER_HEIGHT + day * ch;
    return QRect(x + 1, y + 1, cw - 2, ch - 2);
}

bool BWeeklyPlanner::getDayHourFromPos(const QPoint &pos, int &day, int &hour) const
{
    int cw = cellWidth();
    int ch = cellHeight();

    if (pos.x() < DAY_LABEL_WIDTH || pos.y() < HEADER_HEIGHT) {
        return false;
    }

    hour = (pos.x() - DAY_LABEL_WIDTH) / cw;
    day = (pos.y() - HEADER_HEIGHT) / ch;

    if (hour >= 0 && hour < HOURS && day >= 0 && day < DAYS) {
        return true;
    }

    return false;
}

void BWeeklyPlanner::mouseMoveEvent(QMouseEvent *event)
{
    int day, hour;
    if (getDayHourFromPos(event->pos(), day, hour)) {
        const QVector<BackupLevel> &levels = m_schedule[day][hour];
        if (!levels.isEmpty()) {
            m_hoverDay = day;
            m_hoverHour = hour;

            // Build tooltip showing all levels
            QStringList levelNames;
            for (BackupLevel level : levels) {
                switch (level) {
                    case LEVEL_FULL:         levelNames << "Full"; break;
                    case LEVEL_DIFFERENTIAL: levelNames << "Differential"; break;
                    case LEVEL_INCREMENTAL:  levelNames << "Incremental"; break;
                    default:                 levelNames << "Unknown"; break;
                }
            }

            QString tooltip = QString("%1, %2:00 (%3)")
                .arg(m_dayNames[day])
                .arg(hour, 2, 10, QChar('0'))
                .arg(levelNames.join(" + "));
            QToolTip::showText(event->globalPosition().toPoint(), tooltip, this);

            update();
            return;
        }
    }

    // No cell hovered
    if (m_hoverDay != -1 || m_hoverHour != -1) {
        m_hoverDay = -1;
        m_hoverHour = -1;
        update();
    }

    QWidget::mouseMoveEvent(event);
}

void BWeeklyPlanner::leaveEvent(QEvent *event)
{
    if (m_hoverDay != -1 || m_hoverHour != -1) {
        m_hoverDay = -1;
        m_hoverHour = -1;
        update();
    }

    QWidget::leaveEvent(event);
}
