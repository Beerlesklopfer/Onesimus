#include "schedules/bscheduleganttwidget.h"
#include "blogging.h"
#include <QPainter>
#include <QToolTip>
#include <QDateTime>
#include <QWheelEvent>
#include <QResizeEvent>
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
    updateMinimumSize();
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
    m_zoomMinPph = qMax(MIN_PIXELS_PER_HOUR, pixelsPerHour);
    m_pixelsPerHour = m_zoomMinPph;
    updateMinimumSize();
    recalcHeatmap();
    update();
}

void BScheduleGanttWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Stretch-to-fit: compute pixels-per-hour from available width
    int availableWidth = width() - LABEL_WIDTH - 20;
    if (availableWidth < 100) availableWidth = 100;

    int hours = totalHours();
    int fitPph = availableWidth / hours;

    // Respect zoom minimum (slider-driven in week mode, default in day mode)
    m_pixelsPerHour = qMax(m_zoomMinPph, fitPph);

    recalcHeatmap();
}

QSize BScheduleGanttWidget::sizeHint() const
{
    return QSize(LABEL_WIDTH + contentWidth() + 20, contentHeight() + 20);
}

QSize BScheduleGanttWidget::minimumSizeHint() const
{
    return QSize(300, HEADER_HEIGHT + ROW_HEIGHT * 3 + HEATMAP_HEIGHT);
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

void BScheduleGanttWidget::updateMinimumSize()
{
    // Height: content-driven so vertical scrollbar appears
    setMinimumHeight(contentHeight() + 20);

    // Width: in week mode, enforce minimum from zoom so horizontal scrollbar appears
    if (m_viewMode == WeekView) {
        int minWidth = LABEL_WIDTH + totalHours() * m_zoomMinPph + 20;
        setMinimumWidth(minWidth);
    } else {
        setMinimumWidth(0);  // day mode: stretch to fit
    }
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
    } else if (m_groupMode == GroupByClient) {
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
    } else {
        // GroupByStorage
        QMap<QString, QVector<int>> groups;
        for (int i = 0; i < m_entries.size(); ++i) {
            QString storage = m_entries[i].storage.isEmpty() ? tr("(no storage)") : m_entries[i].storage;
            groups[storage].append(i);
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

    updateMinimumSize();
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

    // Drag overlay (ghost bar, dependency highlights)
    if (m_dragState == Dragging) {
        drawDragOverlay(painter);
    }

    // Now marker on top
    drawNowMarker(painter);
}

void BScheduleGanttWidget::drawTimeHeader(QPainter &painter)
{
    painter.save();

    int hours = totalHours();

    // Header background — full widget width to clear old paint after view switch
    QRect headerBg(LABEL_WIDTH, 0, width() - LABEL_WIDTH, HEADER_HEIGHT);
    painter.fillRect(headerBg, palette().alternateBase());

    QFont font = painter.font();

    // === Row 1: Weekday names ===
    static const QStringList dayNames = {
        tr("Monday"), tr("Tuesday"), tr("Wednesday"), tr("Thursday"),
        tr("Friday"), tr("Saturday"), tr("Sunday")
    };
    static const QStringList dayAbbrev = {
        tr("Mon"), tr("Tue"), tr("Wed"), tr("Thu"),
        tr("Fri"), tr("Sat"), tr("Sun")
    };

    font.setPointSize(8);
    font.setBold(true);
    painter.setFont(font);

    if (m_viewMode == WeekView) {
        // Week: one label per day spanning 24 hours
        for (int d = 0; d < 7; ++d) {
            int x1 = timeToX(d * 24, 0);
            int x2 = timeToX((d + 1) * 24, 0);
            int dayWidth = x2 - x1;

            // Alternating day background in row 1
            if (d % 2 == 0) {
                painter.fillRect(x1, 0, dayWidth, HEADER_ROW1_HEIGHT,
                                 QColor(70, 130, 180, 40));  // steel blue tint
            } else {
                painter.fillRect(x1, 0, dayWidth, HEADER_ROW1_HEIGHT,
                                 QColor(70, 130, 180, 20));
            }

            // Day name — use abbreviation if too narrow
            QString label = (dayWidth > 80) ? dayNames[d] : dayAbbrev[d];
            painter.setPen(QColor(30, 80, 140));  // dark blue
            QRect textRect(x1, 0, dayWidth, HEADER_ROW1_HEIGHT);
            painter.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter, label);

            // Vertical separator between days
            painter.setPen(QPen(QColor(70, 130, 180, 100), 1));
            painter.drawLine(x1, 0, x1, HEADER_ROW1_HEIGHT);
        }
    } else {
        // Day mode: single day name centered
        int dayIdx = qBound(0, m_currentDay, 6);
        painter.fillRect(LABEL_WIDTH, 0, width() - LABEL_WIDTH, HEADER_ROW1_HEIGHT,
                         QColor(70, 130, 180, 30));
        painter.setPen(QColor(30, 80, 140));
        QRect textRect(LABEL_WIDTH, 0, contentWidth(), HEADER_ROW1_HEIGHT);
        painter.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter, dayNames[dayIdx]);
    }

    // Separator line between row 1 and row 2
    painter.setPen(QPen(palette().mid().color(), 1));
    painter.drawLine(LABEL_WIDTH, HEADER_ROW1_HEIGHT, width(), HEADER_ROW1_HEIGHT);

    // === Row 2: Hour labels ===
    font.setBold(false);
    font.setPointSize(7);
    painter.setFont(font);

    int labelIntervalMin = (m_viewMode == DayView) ? 60 : qMax(60, m_dragSnapMinutes);
    int totalMinutes = hours * 60;

    for (int m = 0; m < totalMinutes; m += labelIntervalMin) {
        int h = m / 60;
        int min = m % 60;
        int x = timeToX(h, min);

        // Tick line at bottom of row 2
        painter.setPen(QPen(palette().mid().color(), 1));
        painter.drawLine(x, HEADER_HEIGHT - 4, x, HEADER_HEIGHT);

        // Hour label
        int displayHour = h % 24;
        QString label = QString("%1:%2")
            .arg(displayHour, 2, 10, QChar('0'))
            .arg(min, 2, 10, QChar('0'));

        painter.setPen(palette().text().color());
        QRect textRect(x - 20, HEADER_ROW1_HEIGHT, 40, HEADER_ROW2_HEIGHT);
        painter.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter, label);
    }

    // Bottom border
    painter.setPen(QPen(palette().mid().color(), 1));
    painter.drawLine(LABEL_WIDTH, HEADER_HEIGHT, width(), HEADER_HEIGHT);

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

    // Grid: vertical lines at snap interval
    int snapMin = qMax(1, m_dragSnapMinutes);
    int totalMin = totalHours() * 60;
    int rowsBottom = HEADER_HEIGHT + m_rows.size() * ROW_HEIGHT;
    for (int m = 0; m <= totalMin; m += snapMin) {
        int h = m / 60;
        int min = m % 60;
        int x = timeToX(h, min);
        // Stronger line at day boundaries in week view, medium at hour boundaries
        if (m_viewMode == WeekView && m % (24 * 60) == 0)
            painter.setPen(QPen(palette().mid().color(), 1.0));
        else if (min == 0)
            painter.setPen(QPen(palette().mid().color(), 0.5));
        else
            painter.setPen(QPen(palette().mid().color(), 0.2));
        painter.drawLine(x, HEADER_HEIGHT, x, rowsBottom);
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
    // --- Drag state transitions ---
    if (m_dragState == DragPending) {
        if ((event->pos() - m_dragStartPos).manhattanLength() > 5) {
            m_dragState = Dragging;
            setCursor(Qt::ClosedHandCursor);
            setMouseTracking(true);
            computeDependencies(m_dragEntryIndex);
            emit dragStarted(m_entries[m_dragEntryIndex]);
        }
    }

    if (m_dragState == Dragging) {
        // Calculate snapped time from X position
        int x = event->pos().x();
        int rawMinutes = xToHour(x) * 60 + xToMinute(x);

        // Account for day offset in week view
        if (m_viewMode == WeekView && m_dragDayOffset > 0) {
            rawMinutes -= m_dragDayOffset * 24 * 60;
        }

        // Clamp to 0:00-23:59
        rawMinutes = qBound(0, rawMinutes, 23 * 60 + 59);

        // Snap to grid
        rawMinutes = (rawMinutes / m_dragSnapMinutes) * m_dragSnapMinutes;
        m_dragNewHour = rawMinutes / 60;
        m_dragNewMinute = rawMinutes % 60;

        updateDragDependencies();
        update();
        event->accept();
        return;
    }

    // --- Normal hover/tooltip handling ---
    m_hoverPos = event->pos();
    int idx = entryAtPos(event->pos());

    if (idx != m_hoverEntryIndex) {
        m_hoverEntryIndex = idx;
        // Show open hand cursor for draggable entries
        if (idx >= 0 && !m_entries[idx].jobName.isEmpty()) {
            setCursor(Qt::OpenHandCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }
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
            QString levelStr = BScheduleEntry::levelToString(e.level);
            int minD = m_durationStats->minDuration(e.jobName, levelStr);
            int maxD = m_durationStats->maxDuration(e.jobName, levelStr);
            int samples = m_durationStats->sampleCount(e.jobName, levelStr);
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
            const BScheduleEntry &e = m_entries[idx];
            emit entryClicked(e);

            // Start potential drag (only if entry has a job name)
            if (!e.jobName.isEmpty()) {
                m_dragState = DragPending;
                m_dragEntryIndex = idx;
                m_dragStartPos = event->pos();
                m_dragDayOffset = dayOffsetAtPos(event->pos(), idx);
                m_dragOriginalHour = e.hour;
                m_dragOriginalMinute = e.minute;
            }
        }
    }
    QWidget::mousePressEvent(event);
}

void BScheduleGanttWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dragState == Dragging && event->button() == Qt::LeftButton) {
        setCursor(Qt::ArrowCursor);

        // Only emit if time actually changed
        if (m_dragNewHour != m_dragOriginalHour || m_dragNewMinute != m_dragOriginalMinute) {
            emit dragCompleted(m_dragEntryIndex, m_dragNewHour, m_dragNewMinute,
                               m_dragDependencies);
        }

        cancelDrag();
    } else if (m_dragState == DragPending && event->button() == Qt::LeftButton) {
        m_dragState = NoDrag;
        m_dragEntryIndex = -1;
    }

    QWidget::mouseReleaseEvent(event);
}

void BScheduleGanttWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_dragState != NoDrag) {
        cancelDrag();
        return;
    }

    int idx = entryAtPos(event->pos());
    if (idx >= 0) {
        emit entryDoubleClicked(m_entries[idx]);
    }
    QWidget::mouseDoubleClickEvent(event);
}

void BScheduleGanttWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && m_dragState == Dragging) {
        cancelDrag();
        event->accept();
        return;
    }
    QWidget::keyPressEvent(event);
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
    // Zoom is automatic (stretch-to-fit) — pass wheel events to parent for scrolling
    QWidget::wheelEvent(event);
}

// ============================================================================
// Drag helpers
// ============================================================================

void BScheduleGanttWidget::cancelDrag()
{
    m_dragState = NoDrag;
    m_dragEntryIndex = -1;
    m_dragDependencies.clear();
    setCursor(Qt::ArrowCursor);
    update();
}

int BScheduleGanttWidget::dayOffsetAtPos(const QPoint &pos, int entryIdx) const
{
    if (m_viewMode == DayView) return 0;
    if (entryIdx < 0 || entryIdx >= m_entries.size()) return 0;

    int row = rowForEntry(entryIdx);
    if (row < 0) return 0;

    const BScheduleEntry &entry = m_entries[entryIdx];
    for (int d : entry.daysOfWeek) {
        QRect br = barRect(entry, row, d);
        if (br.contains(pos)) return d;
    }
    return 0;
}

int BScheduleGanttWidget::rowForEntry(int entryIndex) const
{
    for (int r = 0; r < m_rows.size(); ++r) {
        if (!m_rows[r].isGroupHeader && m_rows[r].entryIndex == entryIndex) {
            return r;
        }
    }
    return -1;
}

// ============================================================================
// Dependency calculation
// ============================================================================

void BScheduleGanttWidget::computeDependencies(int draggedIndex)
{
    m_dragDependencies.clear();
    if (draggedIndex < 0 || draggedIndex >= m_entries.size()) return;

    const BScheduleEntry &dragged = m_entries[draggedIndex];

    for (int i = 0; i < m_entries.size(); ++i) {
        if (i == draggedIndex) continue;
        const BScheduleEntry &other = m_entries[i];

        // Check if they share any day
        bool shareDay = false;
        for (int d : dragged.daysOfWeek) {
            if (other.daysOfWeek.contains(d)) {
                shareDay = true;
                break;
            }
        }
        if (!shareDay) continue;

        // 1. Level chain: same client, same schedule → Full/Inc/Diff relationship
        if (!dragged.client.isEmpty() && dragged.client == other.client
            && dragged.scheduleName == other.scheduleName) {
            bool isChain = false;
            QString desc;

            if (dragged.level == BScheduleEntry::Full &&
                (other.level == BScheduleEntry::Incremental || other.level == BScheduleEntry::Differential)) {
                isChain = true;
                desc = tr("Level chain: %1 (%2) depends on this Full")
                    .arg(other.jobName.isEmpty() ? other.scheduleName : other.jobName,
                         BScheduleEntry::levelToString(other.level));
            } else if ((dragged.level == BScheduleEntry::Incremental || dragged.level == BScheduleEntry::Differential)
                       && other.level == BScheduleEntry::Full) {
                isChain = true;
                desc = tr("Level chain: this %1 depends on Full %2")
                    .arg(BScheduleEntry::levelToString(dragged.level),
                         other.jobName.isEmpty() ? other.scheduleName : other.jobName);
            }

            if (isChain) {
                DependencyEdge edge;
                edge.type = DependencyEdge::LevelChain;
                edge.sourceEntryIndex = draggedIndex;
                edge.targetEntryIndex = i;
                edge.description = desc;
                m_dragDependencies.append(edge);
                continue;  // don't duplicate as client exclusion
            }
        }

        // 2. Client exclusion: same client, different job
        if (!dragged.client.isEmpty() && dragged.client == other.client) {
            DependencyEdge edge;
            edge.type = DependencyEdge::ClientExclusion;
            edge.sourceEntryIndex = draggedIndex;
            edge.targetEntryIndex = i;
            edge.description = tr("Client exclusion: %1 (%2) on same client %3")
                .arg(other.jobName.isEmpty() ? other.scheduleName : other.jobName,
                     BScheduleEntry::levelToString(other.level),
                     other.client);
            m_dragDependencies.append(edge);
            continue;  // don't duplicate as storage contention
        }

        // 3. Storage contention: same storage
        if (!dragged.storage.isEmpty() && dragged.storage == other.storage) {
            DependencyEdge edge;
            edge.type = DependencyEdge::StorageContention;
            edge.sourceEntryIndex = draggedIndex;
            edge.targetEntryIndex = i;
            edge.description = tr("Storage contention: %1 on same storage %2")
                .arg(other.jobName.isEmpty() ? other.scheduleName : other.jobName,
                     other.storage);
            m_dragDependencies.append(edge);
        }
    }
}

void BScheduleGanttWidget::updateDragDependencies()
{
    // Recompute which dependencies actually overlap with the new drag position
    // The dependency list itself stays the same (computed at drag start),
    // but we mark which ones create actual time conflicts at the new position

    // Dependencies are already computed in computeDependencies().
    // The visual rendering in drawDependencyHighlights() checks overlap
    // dynamically using the current m_dragNewHour/m_dragNewMinute.
}

// ============================================================================
// Drag visual overlay
// ============================================================================

void BScheduleGanttWidget::drawDragOverlay(QPainter &painter)
{
    if (m_dragEntryIndex < 0 || m_dragEntryIndex >= m_entries.size()) return;

    int row = rowForEntry(m_dragEntryIndex);
    if (row < 0) return;

    const BScheduleEntry &entry = m_entries[m_dragEntryIndex];
    painter.save();

    // --- 1. Ghost bar at original position (semi-transparent, dashed) ---
    QRect originalRect = barRect(entry, row, m_dragDayOffset);
    QColor ghostColor = colorForLevel(entry.level);
    ghostColor.setAlpha(80);

    QPen dashPen(ghostColor.darker(120), 1.5, Qt::DashLine);
    painter.setPen(dashPen);
    painter.setBrush(ghostColor);
    painter.drawRoundedRect(originalRect, 3, 3);

    // --- 2. Preview bar at new snapped position ---
    // Build a temporary entry with the new time
    BScheduleEntry previewEntry = entry;
    previewEntry.hour = m_dragNewHour;
    previewEntry.minute = m_dragNewMinute;
    QRect previewRect = barRect(previewEntry, row, m_dragDayOffset);

    QColor previewColor = colorForLevel(entry.level);
    painter.setPen(QPen(previewColor.darker(140), 2));
    painter.setBrush(previewColor);
    painter.drawRoundedRect(previewRect, 3, 3);

    // --- 3. Time label centered in preview bar ---
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(8);
    font.setBold(true);
    painter.setFont(font);
    QString timeLabel = QString("%1:%2")
        .arg(m_dragNewHour, 2, 10, QChar('0'))
        .arg(m_dragNewMinute, 2, 10, QChar('0'));
    painter.drawText(previewRect, Qt::AlignCenter, timeLabel);
    font.setBold(false);
    painter.setFont(font);

    // --- 4. Snap line: vertical dotted line at snap position ---
    int snapX = timeToX(m_dragDayOffset * 24 + m_dragNewHour, m_dragNewMinute);
    QPen snapPen(QColor(100, 100, 100, 120), 1, Qt::DotLine);
    painter.setPen(snapPen);
    painter.drawLine(snapX, HEADER_HEIGHT, snapX, HEADER_HEIGHT + m_rows.size() * ROW_HEIGHT);

    // --- 5. Dependency highlights ---
    drawDependencyHighlights(painter);

    painter.restore();
}

void BScheduleGanttWidget::drawDependencyHighlights(QPainter &painter)
{
    if (m_dragDependencies.isEmpty()) return;
    if (m_dragEntryIndex < 0 || m_dragEntryIndex >= m_entries.size()) return;

    const BScheduleEntry &dragged = m_entries[m_dragEntryIndex];

    // Drag entry duration for overlap check
    int dragDur = dragged.estimatedDurationSecs;
    if (dragDur <= 0) {
        switch (dragged.level) {
        case BScheduleEntry::Full:         dragDur = 3600; break;
        case BScheduleEntry::Differential: dragDur = 1800; break;
        case BScheduleEntry::VirtualFull:  dragDur = 3600; break;
        default:                           dragDur = 900;  break;
        }
    }
    int dragStartMin = m_dragNewHour * 60 + m_dragNewMinute;
    int dragEndMin = dragStartMin + dragDur / 60;

    for (const DependencyEdge &dep : m_dragDependencies) {
        int targetIdx = dep.targetEntryIndex;
        if (targetIdx < 0 || targetIdx >= m_entries.size()) continue;

        const BScheduleEntry &target = m_entries[targetIdx];
        int targetRow = rowForEntry(targetIdx);
        if (targetRow < 0) continue;

        // Check actual time overlap with new position
        int targetDur = target.estimatedDurationSecs;
        if (targetDur <= 0) {
            switch (target.level) {
            case BScheduleEntry::Full:         targetDur = 3600; break;
            case BScheduleEntry::Differential: targetDur = 1800; break;
            case BScheduleEntry::VirtualFull:  targetDur = 3600; break;
            default:                           targetDur = 900;  break;
            }
        }
        int targetStartMin = target.hour * 60 + target.minute;
        int targetEndMin = targetStartMin + targetDur / 60;

        bool overlaps = (dragStartMin < targetEndMin && targetStartMin < dragEndMin);

        // Choose color based on dependency type
        QColor depColor;
        switch (dep.type) {
        case DependencyEdge::LevelChain:
            depColor = QColor(255, 165, 0);   // Orange
            break;
        case DependencyEdge::ClientExclusion:
            depColor = QColor(220, 40, 40);    // Red
            break;
        case DependencyEdge::StorageContention:
            depColor = QColor(160, 40, 200);   // Violet
            break;
        }

        // Find target bar rect (check all days the target runs on)
        QVector<int> targetDays;
        if (m_viewMode == DayView) {
            if (target.daysOfWeek.contains(m_currentDay)) {
                targetDays.append(0);
            }
        } else {
            // In week view, highlight the day that matches the drag day
            for (int d : target.daysOfWeek) {
                if (m_entries[m_dragEntryIndex].daysOfWeek.contains(d)) {
                    targetDays.append(d);
                }
            }
        }

        for (int d : targetDays) {
            QRect targetRect = barRect(target, targetRow, d);

            // Draw colored dashed border around affected entry
            QPen borderPen(depColor, 2, overlaps ? Qt::SolidLine : Qt::DashLine);
            painter.setPen(borderPen);
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundedRect(targetRect.adjusted(-2, -2, 2, 2), 4, 4);

            // If overlapping, draw a semi-transparent fill
            if (overlaps) {
                QColor fillColor = depColor;
                fillColor.setAlpha(40);
                painter.setBrush(fillColor);
                painter.setPen(Qt::NoPen);
                painter.drawRoundedRect(targetRect, 3, 3);
            }

            // Draw connector line from drag preview to target bar
            int dragRow = rowForEntry(m_dragEntryIndex);
            if (dragRow >= 0) {
                BScheduleEntry previewEntry = dragged;
                previewEntry.hour = m_dragNewHour;
                previewEntry.minute = m_dragNewMinute;
                QRect previewRect = barRect(previewEntry, dragRow, m_dragDayOffset);

                QPoint from = previewRect.center();
                QPoint to = targetRect.center();

                // Only draw if not the same row
                if (dragRow != targetRow) {
                    QPen connPen(depColor, 1, Qt::DotLine);
                    connPen.setColor(QColor(depColor.red(), depColor.green(), depColor.blue(), 150));
                    painter.setPen(connPen);
                    painter.drawLine(from, to);
                }
            }
        }
    }
}
