#include "schedules/bweeklyplanner.h"
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
    setMinimumSize(sizeHint());
}

BWeeklyPlanner::~BWeeklyPlanner()
{
}

QSize BWeeklyPlanner::sizeHint() const
{
    return QSize(
        DAY_LABEL_WIDTH + (HOURS * CELL_WIDTH) + 20,
        HEADER_HEIGHT + (DAYS * CELL_HEIGHT) + LEGEND_HEIGHT + 20
    );
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
    switch (level) {
        case LEVEL_FULL:
            return hovered ? QColor(60, 180, 75, 200) : QColor(40, 140, 55);  // Green
        case LEVEL_DIFFERENTIAL:
            return hovered ? QColor(255, 165, 0, 200) : QColor(220, 130, 0);  // Orange
        case LEVEL_INCREMENTAL:
            return hovered ? QColor(100, 150, 255) : QColor(50, 100, 200);  // Blue
        case LEVEL_NONE:
        default:
            return Qt::transparent;
    }
}

void BWeeklyPlanner::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawGrid(painter);
    drawSchedule(painter);
    drawLegend(painter);
}

void BWeeklyPlanner::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(Qt::gray, 1));

    // Draw day labels
    for (int day = 0; day < DAYS; ++day) {
        QRect labelRect(10, HEADER_HEIGHT + day * CELL_HEIGHT, DAY_LABEL_WIDTH - 10, CELL_HEIGHT);
        painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, m_dayNames[day]);
    }

    // Draw hour labels
    painter.save();
    painter.setPen(QPen(Qt::darkGray, 1));
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    for (int hour = 0; hour < HOURS; hour += 2) {  // Show every 2 hours
        int x = DAY_LABEL_WIDTH + hour * CELL_WIDTH;
        QRect headerRect(x, 10, CELL_WIDTH * 2, HEADER_HEIGHT - 10);
        painter.drawText(headerRect, Qt::AlignCenter, QString("%1").arg(hour, 2, 10, QChar('0')));
    }
    painter.restore();

    // Draw grid lines
    painter.setPen(QPen(Qt::lightGray, 1));

    // Horizontal lines
    for (int day = 0; day <= DAYS; ++day) {
        int y = HEADER_HEIGHT + day * CELL_HEIGHT;
        painter.drawLine(DAY_LABEL_WIDTH, y, DAY_LABEL_WIDTH + HOURS * CELL_WIDTH, y);
    }

    // Vertical lines
    for (int hour = 0; hour <= HOURS; ++hour) {
        int x = DAY_LABEL_WIDTH + hour * CELL_WIDTH;
        painter.drawLine(x, HEADER_HEIGHT, x, HEADER_HEIGHT + DAYS * CELL_HEIGHT);
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

void BWeeklyPlanner::drawLegend(QPainter &painter)
{
    int legendY = HEADER_HEIGHT + DAYS * CELL_HEIGHT + 10;
    int x = DAY_LABEL_WIDTH;
    int boxSize = 16;
    int spacing = 20;

    struct LegendItem {
        BackupLevel level;
        QString label;
    };

    QVector<LegendItem> items = {
        { LEVEL_FULL, tr("Full") },
        { LEVEL_DIFFERENTIAL, tr("Differential") },
        { LEVEL_INCREMENTAL, tr("Incremental") }
    };

    painter.save();
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);
    QFontMetrics fm(font);

    for (const auto &item : items) {
        QRect colorBox(x, legendY + (LEGEND_HEIGHT - boxSize) / 2 - 5, boxSize, boxSize);
        painter.fillRect(colorBox, getColorForLevel(item.level));
        painter.setPen(Qt::gray);
        painter.drawRect(colorBox);

        int textX = x + boxSize + 4;
        int textWidth = fm.horizontalAdvance(item.label);
        QRect textRect(textX, legendY - 5, textWidth, LEGEND_HEIGHT);
        painter.setPen(Qt::black);
        painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, item.label);

        x = textX + textWidth + spacing;
    }

    painter.restore();
}

QRect BWeeklyPlanner::getCellRect(int day, int hour) const
{
    int x = DAY_LABEL_WIDTH + hour * CELL_WIDTH;
    int y = HEADER_HEIGHT + day * CELL_HEIGHT;
    return QRect(x + 1, y + 1, CELL_WIDTH - 2, CELL_HEIGHT - 2);
}

bool BWeeklyPlanner::getDayHourFromPos(const QPoint &pos, int &day, int &hour) const
{
    if (pos.x() < DAY_LABEL_WIDTH || pos.y() < HEADER_HEIGHT) {
        return false;
    }

    hour = (pos.x() - DAY_LABEL_WIDTH) / CELL_WIDTH;
    day = (pos.y() - HEADER_HEIGHT) / CELL_HEIGHT;

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
