#include "schedules/bscheduleganttwidget.h"
#include "blogging.h"
#include <QPainter>
#include <QToolTip>
#include <QDateTime>
#include <QWheelEvent>
#include <algorithm>

BScheduleGanttWidget::BScheduleGanttWidget(QWidget *parent)
    : QWidget(parent)
    , m_nowTimer(new QTimer(this))
{
    setMouseTracking(true);

    // Update now-marker every minute
    connect(m_nowTimer, &QTimer::timeout, this, QOverload<>::of(&QWidget::update));
    m_nowTimer->start(60000);
}

BScheduleGanttWidget::~BScheduleGanttWidget()
{
}

void BScheduleGanttWidget::setEntries(const QList<BScheduleEntry> &entries)
{
    m_entries = entries;

    // Enrich entries with duration estimates
    if (m_durationStats) {
        for (BScheduleEntry &e : m_entries) {
            if (e.estimatedDurationSecs <= 0 && !e.jobName.isEmpty()) {
                e.estimatedDurationSecs = m_durationStats->averageDuration(e.jobName);
            }
        }
    }

    rebuildRows();
    recalcHeatmap();
    detectCollisions();
    update();
}

void BScheduleGanttWidget::setDurationStats(BJobDurationStats *stats)
{
    m_durationStats = stats;
}

void BScheduleGanttWidget::setViewMode(ViewMode mode)
{
    if (m_viewMode == mode) return;
    m_viewMode = mode;
    recalcHeatmap();
    detectCollisions();
    update();
}

void BScheduleGanttWidget::setGroupMode(GroupMode mode)
{
    if (m_groupMode == mode) return;
    m_groupMode = mode;
    rebuildRows();
    update();
}

void BScheduleGanttWidget::setCurrentDay(int dayOfWeek)
{
    m_currentDay = qBound(0, dayOfWeek, 6);
    recalcHeatmap();
    detectCollisions();
    update();
}

void BScheduleGanttWidget::setZoomLevel(int pixelsPerHour)
{
    m_pixelsPerHour = qBound(MIN_PIXELS_PER_HOUR, pixelsPerHour, MAX_PIXELS_PER_HOUR);
    recalcHeatmap();
    update();
}

QSize BScheduleGanttWidget::sizeHint() const
{
    return QSize(LABEL_WIDTH + contentWidth() + 20,
                 contentHeight() + 20);
}

QSize BScheduleGanttWidget::minimumSizeHint() const
{
    return QSize(LABEL_WIDTH + 400, HEADER_HEIGHT + ROW_HEIGHT * 3 + HEATMAP_HEIGHT);
}

// ============================================================================
// Layout helpers
// ============================================================================

int BScheduleGanttWidget::totalHours() const
{
    return (m_viewMode == WeekView) ? 168 : 24;  // 7*24 or 24
}

int BScheduleGanttWidget::contentWidth() const
{
    return totalHours() * m_pixelsPerHour;
}

int BScheduleGanttWidget::contentHeight() const
{
    return HEADER_HEIGHT + m_rows.size() * ROW_HEIGHT + HEATMAP_HEIGHT;
}

int BScheduleGanttWidget::timeToX(int hour, int minute) const
{
    double fractionalHour = hour + minute / 60.0;
    return LABEL_WIDTH + static_cast<int>(fractionalHour * m_pixelsPerHour);
}

int BScheduleGanttWidget::xToHour(int x) const
{
    int relX = x - LABEL_WIDTH;
    if (relX < 0) return -1;
    return relX / m_pixelsPerHour;
}

int BScheduleGanttWidget::xToMinute(int x) const
{
    int relX = x - LABEL_WIDTH;
    if (relX < 0) return 0;
    int hourPixels = relX % m_pixelsPerHour;
    return (hourPixels * 60) / m_pixelsPerHour;
}

int BScheduleGanttWidget::rowAtY(int y) const
{
    int relY = y - HEADER_HEIGHT;
    if (relY < 0) return -1;
    int row = relY / ROW_HEIGHT;
    if (row >= m_rows.size()) return -1;
    return row;
}

void BScheduleGanttWidget::rebuildRows()
{
    m_rows.clear();

    if (m_groupMode == GroupBySchedule) {
        // Group entries by schedule name
        QMap<QString, QVector<int>> groups;
        for (int i = 0; i < m_entries.size(); ++i) {
            groups[m_entries[i].scheduleName].append(i);
        }

        for (auto it = groups.constBegin(); it != groups.constEnd(); ++it) {
            // Group header
            RowInfo header;
            header.label = it.key();
            header.isGroupHeader = true;
            header.entryIndex = -1;
            m_rows.append(header);

            // Entries
            for (int idx : it.value()) {
                RowInfo row;
                const BScheduleEntry &e = m_entries[idx];
                row.label = QString("  %1 %2")
                    .arg(BScheduleEntry::levelToString(e.level),
                         e.jobName.isEmpty() ? QString() : e.jobName);
                row.isGroupHeader = false;
                row.entryIndex = idx;
                m_rows.append(row);
            }
        }
    } else {
        // Group by client
        QMap<QString, QVector<int>> groups;
        for (int i = 0; i < m_entries.size(); ++i) {
            QString client = m_entries[i].client.isEmpty() ? tr("(no client)") : m_entries[i].client;
            groups[client].append(i);
        }

        for (auto it = groups.constBegin(); it != groups.constEnd(); ++it) {
            RowInfo header;
            header.label = it.key();
            header.isGroupHeader = true;
            header.entryIndex = -1;
            m_rows.append(header);

            for (int idx : it.value()) {
                RowInfo row;
                const BScheduleEntry &e = m_entries[idx];
                row.label = QString("  %1 %2")
                    .arg(BScheduleEntry::levelToString(e.level), e.scheduleName);
                row.isGroupHeader = false;
                row.entryIndex = idx;
                m_rows.append(row);
            }
        }
    }
}

QRect BScheduleGanttWidget::barRect(const BScheduleEntry &entry, int row, int dayOffset) const
{
    int startHour = dayOffset * 24 + entry.hour;
    int x = timeToX(startHour, entry.minute);
    int y = HEADER_HEIGHT + row * ROW_HEIGHT + 2;

    // Duration bar width
    int durationSecs = entry.estimatedDurationSecs;
    if (durationSecs <= 0) {
        // Default: 1 hour for Full, 30 min for Diff, 15 min for Inc
        switch (entry.level) {
        case BScheduleEntry::Full:         durationSecs = 3600; break;
        case BScheduleEntry::Differential: durationSecs = 1800; break;
        case BScheduleEntry::VirtualFull:  durationSecs = 3600; break;
        default:                           durationSecs = 900;  break;
        }
    }

    double durationHours = durationSecs / 3600.0;
    int barWidth = qMax(6, static_cast<int>(durationHours * m_pixelsPerHour));

    return QRect(x, y, barWidth, ROW_HEIGHT - 4);
}

// ============================================================================
// Painting
// ============================================================================

void BScheduleGanttWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background
    painter.fillRect(rect(), palette().window());

    // Time header
    drawTimeHeader(painter);

    // Row content
    drawRows(painter);

    // Heatmap
    drawHeatmap(painter);

    // Collision markers
    drawCollisionMarkers(painter);

    // Now marker on top
    drawNowMarker(painter);
}

void BScheduleGanttWidget::drawTimeHeader(QPainter &painter)
{
    painter.save();

    int hours = totalHours();

    // Header background
    QRect headerRect(LABEL_WIDTH, 0, contentWidth(), HEADER_HEIGHT);
    painter.fillRect(headerRect, palette().alternateBase());

    painter.setPen(palette().text().color());
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    // Determine label interval based on zoom
    int interval = 1;
    if (m_pixelsPerHour < 40) interval = 4;
    else if (m_pixelsPerHour < 60) interval = 2;

    for (int h = 0; h < hours; ++h) {
        int x = timeToX(h, 0);

        // Major tick line
        painter.setPen(QPen(palette().mid().color(), 1));
        painter.drawLine(x, HEADER_HEIGHT - 5, x, HEADER_HEIGHT);

        // Hour label
        if (h % interval == 0) {
            int displayHour = h % 24;
            QString label;
            if (m_viewMode == WeekView && h % 24 == 0) {
                static const QStringList dayAbbrev = {
                    tr("Mon"), tr("Tue"), tr("Wed"), tr("Thu"),
                    tr("Fri"), tr("Sat"), tr("Sun")
                };
                int dayIdx = h / 24;
                if (dayIdx < 7) {
                    label = dayAbbrev[dayIdx];
                }
            } else {
                label = QString("%1:00").arg(displayHour, 2, 10, QChar('0'));
            }

            painter.setPen(palette().text().color());
            QRect textRect(x - 20, 2, 40, HEADER_HEIGHT - 7);
            painter.drawText(textRect, Qt::AlignCenter, label);
        }
    }

    // Bottom border
    painter.setPen(QPen(palette().mid().color(), 1));
    painter.drawLine(LABEL_WIDTH, HEADER_HEIGHT, LABEL_WIDTH + contentWidth(), HEADER_HEIGHT);

    painter.restore();
}

void BScheduleGanttWidget::drawNowMarker(QPainter &painter)
{
    QDateTime now = QDateTime::currentDateTime();
    int currentHour = now.time().hour();
    int currentMinute = now.time().minute();

    int hourOffset = 0;
    if (m_viewMode == DayView) {
        // Only show if current day matches
        int todayDow = now.date().dayOfWeek() - 1;  // Qt: 1=Mon → 0=Mon
        if (todayDow != m_currentDay) return;
    } else {
        int todayDow = now.date().dayOfWeek() - 1;
        hourOffset = todayDow * 24;
    }

    int x = timeToX(hourOffset + currentHour, currentMinute);

    painter.save();
    painter.setPen(QPen(Qt::red, 2));
    painter.drawLine(x, HEADER_HEIGHT, x, HEADER_HEIGHT + m_rows.size() * ROW_HEIGHT);

    // Small triangle at top
    QPolygonF triangle;
    triangle << QPointF(x - 4, HEADER_HEIGHT)
             << QPointF(x + 4, HEADER_HEIGHT)
             << QPointF(x, HEADER_HEIGHT + 6);
    painter.setBrush(Qt::red);
    painter.setPen(Qt::NoPen);
    painter.drawPolygon(triangle);
    painter.restore();
}

void BScheduleGanttWidget::drawRows(QPainter &painter)
{
    painter.save();

    for (int r = 0; r < m_rows.size(); ++r) {
        const RowInfo &row = m_rows[r];
        int y = HEADER_HEIGHT + r * ROW_HEIGHT;

        // Alternating row background
        if (r % 2 == 0) {
            painter.fillRect(LABEL_WIDTH, y, contentWidth(), ROW_HEIGHT,
                             palette().alternateBase());
        }

        // Label
        QRect labelRect(4, y, LABEL_WIDTH - 8, ROW_HEIGHT);
        if (row.isGroupHeader) {
            QFont font = painter.font();
            font.setBold(true);
            painter.setFont(font);
            painter.setPen(palette().text().color());
            painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, row.label);
            font.setBold(false);
            painter.setFont(font);

            // Group header background
            painter.fillRect(LABEL_WIDTH, y, contentWidth(), ROW_HEIGHT,
                             QColor(0, 0, 0, 15));
        } else {
            painter.setPen(palette().text().color());
            painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, row.label);

            // Draw job bars for this entry
            if (row.entryIndex >= 0 && row.entryIndex < m_entries.size()) {
                const BScheduleEntry &entry = m_entries[row.entryIndex];

                if (m_viewMode == DayView) {
                    // Only show if entry runs on current day
                    if (entry.daysOfWeek.contains(m_currentDay)) {
                        drawJobBar(painter, entry, r, 0);
                    }
                } else {
                    // Week view: draw bar for each day the entry runs
                    for (int d : entry.daysOfWeek) {
                        drawJobBar(painter, entry, r, d);
                    }
                }
            }
        }

        // Row separator line
        painter.setPen(QPen(palette().mid().color(), 0.5));
        painter.drawLine(0, y + ROW_HEIGHT, LABEL_WIDTH + contentWidth(), y + ROW_HEIGHT);
    }

    // Grid: vertical lines for each hour
    painter.setPen(QPen(palette().mid().color(), 0.3));
    int hours = totalHours();
    for (int h = 0; h <= hours; ++h) {
        int x = timeToX(h, 0);
        painter.drawLine(x, HEADER_HEIGHT, x, HEADER_HEIGHT + m_rows.size() * ROW_HEIGHT);
    }

    painter.restore();
}

void BScheduleGanttWidget::drawJobBar(QPainter &painter, const BScheduleEntry &entry,
                                       int row, int dayOffset)
{
    QRect rect = barRect(entry, row, dayOffset);
    bool hovered = (row == rowAtY(m_hoverPos.y()) && rect.contains(m_hoverPos));

    QColor fillColor = colorForLevel(entry.level, hovered);

    // Main bar
    painter.setPen(Qt::NoPen);
    painter.setBrush(fillColor);
    painter.drawRoundedRect(rect, 3, 3);

    // Duration uncertainty: lighter shade for max duration
    if (m_durationStats && !entry.jobName.isEmpty()) {
        int maxDur = m_durationStats->maxDuration(entry.jobName);
        int avgDur = m_durationStats->averageDuration(entry.jobName);

        if (maxDur > avgDur && avgDur > 0) {
            double maxHours = maxDur / 3600.0;
            int maxWidth = qMax(rect.width(), static_cast<int>(maxHours * m_pixelsPerHour));
            if (maxWidth > rect.width()) {
                QRect shadowRect(rect.right(), rect.top(),
                                 maxWidth - rect.width(), rect.height());
                QColor shadowColor = fillColor;
                shadowColor.setAlpha(60);
                painter.setBrush(shadowColor);
                painter.drawRoundedRect(shadowRect, 3, 3);
            }
        }
    }

    // Collision border
    for (const Collision &c : m_collisions) {
        bool matches = false;
        if (c.entry1.scheduleName == entry.scheduleName &&
            c.entry1.level == entry.level &&
            c.entry1.hour == entry.hour) {
            matches = true;
        }
        if (c.entry2.scheduleName == entry.scheduleName &&
            c.entry2.level == entry.level &&
            c.entry2.hour == entry.hour) {
            matches = true;
        }
        if (matches) {
            painter.setPen(QPen(Qt::red, 2));
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundedRect(rect.adjusted(-1, -1, 1, 1), 3, 3);
            break;
        }
    }

    // Level letter inside bar if wide enough
    if (rect.width() > 20) {
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setPointSize(8);
        font.setBold(true);
        painter.setFont(font);
        QString levelChar = BScheduleEntry::levelToString(entry.level).left(1);
        painter.drawText(rect, Qt::AlignCenter, levelChar);
        font.setBold(false);
        painter.setFont(font);
    }
}

void BScheduleGanttWidget::drawHeatmap(QPainter &painter)
{
    if (m_heatmapSlots.isEmpty()) return;

    painter.save();

    int y = HEADER_HEIGHT + m_rows.size() * ROW_HEIGHT + 2;
    int slotCount = m_heatmapSlots.size();

    // Label
    QRect labelRect(4, y, LABEL_WIDTH - 8, HEATMAP_HEIGHT);
    painter.setPen(palette().text().color());
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);
    painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, tr("Utilization"));

    double slotWidth = static_cast<double>(contentWidth()) / slotCount;

    for (int i = 0; i < slotCount; ++i) {
        int count = m_heatmapSlots[i];
        QColor color;

        if (count == 0) {
            color = QColor(220, 220, 220, 80);  // Light gray
        } else if (count <= 1) {
            color = QColor(76, 175, 80, 150);    // Green
        } else if (count <= 3) {
            color = QColor(255, 193, 7, 180);    // Yellow/Amber
        } else {
            color = QColor(244, 67, 54, 200);    // Red
        }

        int x = LABEL_WIDTH + static_cast<int>(i * slotWidth);
        int w = static_cast<int>((i + 1) * slotWidth) - static_cast<int>(i * slotWidth);
        painter.fillRect(x, y, w, HEATMAP_HEIGHT, color);
    }

    // Border
    painter.setPen(QPen(palette().mid().color(), 1));
    painter.drawRect(LABEL_WIDTH, y, contentWidth(), HEATMAP_HEIGHT);

    painter.restore();
}

void BScheduleGanttWidget::drawCollisionMarkers(QPainter &painter)
{
    // Collision markers are drawn as part of job bars (red border)
    // This method draws the collision warning icon in the heatmap area
    if (m_collisions.isEmpty()) return;

    painter.save();
    QFont font = painter.font();
    font.setPointSize(7);
    painter.setFont(font);

    for (const Collision &c : m_collisions) {
        int hourOffset = 0;
        if (m_viewMode == WeekView) {
            // Find which day has the collision
            for (int d : c.entry1.daysOfWeek) {
                if (c.entry2.daysOfWeek.contains(d)) {
                    hourOffset = d * 24;
                    break;
                }
            }
        }

        int x = timeToX(hourOffset + c.startHour, c.startMinute);
        int y = HEADER_HEIGHT + m_rows.size() * ROW_HEIGHT + 2;

        // Warning triangle
        painter.setPen(QPen(Qt::red, 1));
        painter.setBrush(QColor(255, 0, 0, 100));
        QPolygonF triangle;
        triangle << QPointF(x - 5, y + HEATMAP_HEIGHT - 2)
                 << QPointF(x + 5, y + HEATMAP_HEIGHT - 2)
                 << QPointF(x, y + 2);
        painter.drawPolygon(triangle);
    }

    painter.restore();
}

// ============================================================================
// Heatmap calculation
// ============================================================================

int BScheduleGanttWidget::heatmapSlotCount() const
{
    return totalHours() * (60 / HEATMAP_SLOT_MINUTES);
}

void BScheduleGanttWidget::recalcHeatmap()
{
    int slotCount = heatmapSlotCount();
    m_heatmapSlots.fill(0, slotCount);

    for (const BScheduleEntry &entry : m_entries) {
        int durationSecs = entry.estimatedDurationSecs;
        if (durationSecs <= 0) {
            switch (entry.level) {
            case BScheduleEntry::Full:         durationSecs = 3600; break;
            case BScheduleEntry::Differential: durationSecs = 1800; break;
            case BScheduleEntry::VirtualFull:  durationSecs = 3600; break;
            default:                           durationSecs = 900;  break;
            }
        }
        int durationSlots = qMax(1, durationSecs / (HEATMAP_SLOT_MINUTES * 60));

        QVector<int> days;
        if (m_viewMode == DayView) {
            if (entry.daysOfWeek.contains(m_currentDay)) {
                days.append(0);  // offset 0 for day view
            }
        } else {
            days = entry.daysOfWeek;
        }

        for (int d : days) {
            int startSlot = (d * 24 * 60 + entry.hour * 60 + entry.minute) / HEATMAP_SLOT_MINUTES;
            if (m_viewMode == DayView) {
                startSlot = (entry.hour * 60 + entry.minute) / HEATMAP_SLOT_MINUTES;
            }

            for (int s = startSlot; s < startSlot + durationSlots && s < slotCount; ++s) {
                if (s >= 0) m_heatmapSlots[s]++;
            }
        }
    }
}

// ============================================================================
// Collision detection
// ============================================================================

void BScheduleGanttWidget::detectCollisions()
{
    m_collisions.clear();

    for (int i = 0; i < m_entries.size(); ++i) {
        for (int j = i + 1; j < m_entries.size(); ++j) {
            const BScheduleEntry &a = m_entries[i];
            const BScheduleEntry &b = m_entries[j];

            // Check if they share any day
            bool shareDay = false;
            for (int d : a.daysOfWeek) {
                if (b.daysOfWeek.contains(d)) {
                    shareDay = true;
                    break;
                }
            }
            if (!shareDay) continue;

            // Check time overlap
            int aDur = a.estimatedDurationSecs > 0 ? a.estimatedDurationSecs : 3600;
            int bDur = b.estimatedDurationSecs > 0 ? b.estimatedDurationSecs : 3600;

            int aStartMin = a.hour * 60 + a.minute;
            int aEndMin = aStartMin + aDur / 60;
            int bStartMin = b.hour * 60 + b.minute;
            int bEndMin = bStartMin + bDur / 60;

            bool overlaps = (aStartMin < bEndMin && bStartMin < aEndMin);
            if (!overlaps) continue;

            // Client collision
            if (!a.client.isEmpty() && a.client == b.client) {
                Collision c;
                c.type = Collision::ClientCollision;
                c.entry1 = a;
                c.entry2 = b;
                c.startHour = qMax(a.hour, b.hour);
                c.startMinute = (a.hour >= b.hour) ? a.minute : b.minute;
                c.description = tr("Client collision: %1 and %2 on %3")
                    .arg(a.scheduleName, b.scheduleName, a.client);
                m_collisions.append(c);
            }

            // Storage collision
            if (!a.storage.isEmpty() && a.storage == b.storage) {
                Collision c;
                c.type = Collision::StorageCollision;
                c.entry1 = a;
                c.entry2 = b;
                c.startHour = qMax(a.hour, b.hour);
                c.startMinute = (a.hour >= b.hour) ? a.minute : b.minute;
                c.description = tr("Storage collision: %1 and %2 on %3")
                    .arg(a.scheduleName, b.scheduleName, a.storage);
                m_collisions.append(c);
            }
        }
    }

    if (!m_collisions.isEmpty()) {
        emit collisionsDetected(m_collisions.size());
    }
}

// ============================================================================
// Colors
// ============================================================================

QColor BScheduleGanttWidget::colorForLevel(BScheduleEntry::BackupLevel level, bool hovered) const
{
    switch (level) {
    case BScheduleEntry::Full:
        return hovered ? QColor(76, 200, 100) : QColor(56, 160, 72);     // Green
    case BScheduleEntry::Differential:
        return hovered ? QColor(255, 180, 50) : QColor(230, 150, 20);    // Orange
    case BScheduleEntry::Incremental:
        return hovered ? QColor(100, 160, 255) : QColor(60, 120, 220);   // Blue
    case BScheduleEntry::VirtualFull:
        return hovered ? QColor(180, 130, 255) : QColor(140, 90, 220);   // Purple
    case BScheduleEntry::None:
    default:
        return Qt::transparent;
    }
}

// ============================================================================
// Mouse events
// ============================================================================

int BScheduleGanttWidget::entryAtPos(const QPoint &pos) const
{
    int row = rowAtY(pos.y());
    if (row < 0 || row >= m_rows.size()) return -1;
    if (m_rows[row].isGroupHeader) return -1;

    int entryIdx = m_rows[row].entryIndex;
    if (entryIdx < 0 || entryIdx >= m_entries.size()) return -1;

    const BScheduleEntry &entry = m_entries[entryIdx];

    if (m_viewMode == DayView) {
        if (entry.daysOfWeek.contains(m_currentDay)) {
            QRect br = barRect(entry, row, 0);
            if (br.contains(pos)) return entryIdx;
        }
    } else {
        for (int d : entry.daysOfWeek) {
            QRect br = barRect(entry, row, d);
            if (br.contains(pos)) return entryIdx;
        }
    }

    return -1;
}

void BScheduleGanttWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_hoverPos = event->pos();
    int idx = entryAtPos(event->pos());

    if (idx != m_hoverEntryIndex) {
        m_hoverEntryIndex = idx;
        update();
    }

    if (idx >= 0) {
        const BScheduleEntry &e = m_entries[idx];
        QString tooltip = QString("<b>%1</b><br>"
                                  "%2: %3<br>"
                                  "%4: %5:%6")
            .arg(e.scheduleName,
                 tr("Level"), BScheduleEntry::levelToString(e.level),
                 tr("Time"),
                 QString::number(e.hour).rightJustified(2, '0'),
                 QString::number(e.minute).rightJustified(2, '0'));

        if (!e.jobName.isEmpty()) {
            tooltip += QString("<br>%1: %2").arg(tr("Job"), e.jobName);
        }
        if (!e.client.isEmpty()) {
            tooltip += QString("<br>%1: %2").arg(tr("Client"), e.client);
        }
        if (e.estimatedDurationSecs > 0) {
            int h = e.estimatedDurationSecs / 3600;
            int m = (e.estimatedDurationSecs % 3600) / 60;
            tooltip += QString("<br>%1: %2h %3m").arg(tr("Duration")).arg(h).arg(m);
        }
        if (m_durationStats && !e.jobName.isEmpty()) {
            int minD = m_durationStats->minDuration(e.jobName);
            int maxD = m_durationStats->maxDuration(e.jobName);
            int samples = m_durationStats->sampleCount(e.jobName);
            if (samples > 0) {
                tooltip += QString("<br><i>%1: %2 | %3: %4 (%5 %6)</i>")
                    .arg(tr("Min"), BJobDurationStats::formatDuration(minD),
                         tr("Max"), BJobDurationStats::formatDuration(maxD))
                    .arg(samples).arg(tr("runs"));
            }
        }
        if (!e.pool.isEmpty()) {
            tooltip += QString("<br>%1: %2").arg(tr("Pool"), e.pool);
        }

        QToolTip::showText(event->globalPosition().toPoint(), tooltip, this);
    } else {
        // Check heatmap hover
        int hmY = HEADER_HEIGHT + m_rows.size() * ROW_HEIGHT + 2;
        if (event->pos().y() >= hmY && event->pos().y() <= hmY + HEATMAP_HEIGHT
            && event->pos().x() >= LABEL_WIDTH) {

            int relX = event->pos().x() - LABEL_WIDTH;
            int slotIdx = relX * heatmapSlotCount() / contentWidth();

            if (slotIdx >= 0 && slotIdx < m_heatmapSlots.size()) {
                int slotMinutes = slotIdx * HEATMAP_SLOT_MINUTES;
                int dayOffset = 0;
                if (m_viewMode == WeekView) {
                    dayOffset = slotMinutes / (24 * 60);
                    slotMinutes %= (24 * 60);
                }
                int h = slotMinutes / 60;
                int m = slotMinutes % 60;
                int count = m_heatmapSlots[slotIdx];

                QString tooltip = QString("%1:%2 - %3:%4: %5 %6")
                    .arg(h, 2, 10, QChar('0'))
                    .arg(m, 2, 10, QChar('0'))
                    .arg((slotMinutes + HEATMAP_SLOT_MINUTES) / 60, 2, 10, QChar('0'))
                    .arg((slotMinutes + HEATMAP_SLOT_MINUTES) % 60, 2, 10, QChar('0'))
                    .arg(count)
                    .arg(tr("concurrent jobs"));

                QToolTip::showText(event->globalPosition().toPoint(), tooltip, this);
            }
        }
    }

    QWidget::mouseMoveEvent(event);
}

void BScheduleGanttWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int idx = entryAtPos(event->pos());
        if (idx >= 0) {
            emit entryClicked(m_entries[idx]);
        }
    }
    QWidget::mousePressEvent(event);
}

void BScheduleGanttWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    int idx = entryAtPos(event->pos());
    if (idx >= 0) {
        emit entryDoubleClicked(m_entries[idx]);
    }
    QWidget::mouseDoubleClickEvent(event);
}

void BScheduleGanttWidget::leaveEvent(QEvent *event)
{
    if (m_hoverEntryIndex != -1) {
        m_hoverEntryIndex = -1;
        update();
    }
    QWidget::leaveEvent(event);
}

void BScheduleGanttWidget::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        int delta = event->angleDelta().y() > 0 ? 10 : -10;
        setZoomLevel(m_pixelsPerHour + delta);
        event->accept();
    } else {
        QWidget::wheelEvent(event);
    }
}
