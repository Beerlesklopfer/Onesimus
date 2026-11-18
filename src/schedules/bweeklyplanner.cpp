#include "schedules/bweeklyplanner.h"
#include "jobs/blevelcolors.h"
#include "blogging.h"
#include <QPainter>
#include <QToolTip>

BWeeklyPlanner::BWeeklyPlanner(QWidget *parent)
    : QWidget(parent)
    , m_weekStart(QDate::currentDate().addDays(1 - QDate::currentDate().dayOfWeek()))
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

void BWeeklyPlanner::setWeek(const QDate &monday, const QList<BScheduleEntry> &entries)
{
    m_scheduleName = tr("All Schedules");
    if (monday.isValid()) {
        m_weekStart = monday.addDays(1 - monday.dayOfWeek());  // Qt: 1 = Monday
    }

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
            level = LEVEL_INCREMENTAL;
            break;
        case BScheduleEntry::None:
        default:
            // No level on the Run line (the job's level applies), or a level
            // without a colour here. Never shown as Incremental.
            level = entry.levelName.isEmpty() ? LEVEL_JOB : LEVEL_OTHER;
            break;
        }

        // One mark per entry in each hour of the week in which it starts.
        // A start that exists twice (end of daylight saving) is one mark.
        bool marked[DAYS][HOURS] = {};
        for (const QDateTime &when : entry.occurrences) {
            const qint64 day = m_weekStart.daysTo(when.date());
            const int hour = when.time().hour();
            if (day < 0 || day >= DAYS || hour < 0 || hour >= HOURS) continue;
            if (marked[day][hour]) continue;
            marked[day][hour] = true;

            // Append each level — side-by-side display
            m_schedule[day][hour].append(level);
        }
    }

    update();
}

void BWeeklyPlanner::clearSchedule()
{
    m_scheduleName.clear();

    for (int day = 0; day < DAYS; ++day) {
        for (int hour = 0; hour < HOURS; ++hour) {
            m_schedule[day][hour].clear();
        }
    }

    update();
}

QColor BWeeklyPlanner::getColorForLevel(BackupLevel level, bool hovered) const
{
    QString code;
    switch (level) {
        case LEVEL_FULL:         code = "F"; break;
        case LEVEL_DIFFERENTIAL: code = "D"; break;
        case LEVEL_INCREMENTAL:  code = "I"; break;
        case LEVEL_JOB:
        case LEVEL_OTHER: {
            // Neutral: the level is not known here or has no colour
            const QColor neutral = palette().color(QPalette::Mid);
            return hovered ? neutral.lighter(130) : neutral;
        }
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

    // Draw day labels: day of the week and, below it, its date in this week
    for (int day = 0; day < DAYS; ++day) {
        QRect labelRect(10, HEADER_HEIGHT + day * ch, DAY_LABEL_WIDTH - 10, ch);
        painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter,
                         m_dayNames[day] + "\n"
                         + m_weekStart.addDays(day).toString(Qt::ISODate));
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
                    case LEVEL_JOB:          levelNames << tr("level of the job"); break;
                    case LEVEL_OTHER:        levelNames << tr("other level"); break;
                    default:                 levelNames << "Unknown"; break;
                }
            }

            QString tooltip = QString("%1 %2, %3:00 (%4)")
                .arg(m_dayNames[day], m_weekStart.addDays(day).toString(Qt::ISODate))
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
