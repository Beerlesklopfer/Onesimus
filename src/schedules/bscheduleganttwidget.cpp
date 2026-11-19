#include "schedules/bscheduleganttwidget.h"
#include "jobs/blevelcolors.h"
#include "buiscale.h"
#include "blogging.h"
#include <QPainter>
#include <QStyleOption>
#include <QToolTip>
#include <QDateTime>
#include <QWheelEvent>
#include <QResizeEvent>
#include <algorithm>

BScheduleGanttWidget::BScheduleGanttWidget(QWidget *parent)
    : QWidget(parent)
    , m_nowTimer(new QTimer(this))
{
    // Allow QSS to control background-color and other palette roles
    setAttribute(Qt::WA_StyledBackground, true);
    setMouseTracking(true);
    updateMetrics();

    // Update now-marker every minute
    connect(m_nowTimer, &QTimer::timeout, this, QOverload<>::of(&QWidget::update));
    m_nowTimer->start(60000);
}

BScheduleGanttWidget::~BScheduleGanttWidget()
{
}

void BScheduleGanttWidget::setEntries(const QList<BScheduleEntry> &entries)
{
    // A refresh during a drag invalidates the stored entry index: even if it
    // is still in range it may now refer to a different entry.
    if (m_dragState != NoDrag) {
        cancelDrag();
    }

    m_entries = entries;

    // The job configuration may have changed with the entries, and with it
    // the level a job runs at when its Run line names none
    m_estimateCache.clear();

    // What was predicted belongs to the entries shown so far
    m_waits.clear();

    rebuildRows();
    recalcHeatmap();
    update();
}

void BScheduleGanttWidget::setWaits(const QHash<QString, RunWait> &waits)
{
    m_waits = waits;

    // The bars moved under a drag
    if (m_dragState != NoDrag) {
        cancelDrag();
    }

    recalcHeatmap();
    update();
}

QString BScheduleGanttWidget::entryKey(const BScheduleEntry &entry)
{
    const QChar sep(0x1F);
    return entry.scheduleName + sep + QString::number(entry.runIndex) + sep
           + QString::number(entry.hour) + sep + entry.jobName;
}

QString BScheduleGanttWidget::waitKey(const BScheduleEntry &entry, const QDateTime &when)
{
    return waitKey(entryKey(entry), when);
}

QString BScheduleGanttWidget::waitKey(const QString &entryKey, const QDateTime &when)
{
    return entryKey + QChar(0x1F) + QString::number(when.toSecsSinceEpoch());
}

QDateTime BScheduleGanttWidget::occurrenceOn(const BScheduleEntry &entry, int dayOffset) const
{
    const QDate date = firstDate().addDays(dayOffset);
    for (const QDateTime &when : entry.occurrences) {
        if (when.date() == date) return when;
    }
    return QDateTime();
}

const BScheduleGanttWidget::RunWait *BScheduleGanttWidget::waitFor(const BScheduleEntry &entry,
                                                                   int dayOffset) const
{
    if (m_waits.isEmpty()) return nullptr;
    const QDateTime when = occurrenceOn(entry, dayOffset);
    if (!when.isValid()) return nullptr;
    const auto it = m_waits.constFind(waitKey(entry, when));
    return it == m_waits.constEnd() ? nullptr : &it.value();
}

void BScheduleGanttWidget::setHistoryStats(const BJobHistoryStats &stats, const QString &note)
{
    m_history = stats;
    m_historyNote = note;
    m_estimateCache.clear();

    // Bar lengths changed under a drag
    if (m_dragState != NoDrag) {
        cancelDrag();
    }

    recalcHeatmap();
    update();
}

void BScheduleGanttWidget::setJobConfigModel(BJobConfigModel *model)
{
    m_jobConfigModel = model;
}

void BScheduleGanttWidget::setViewMode(ViewMode mode)
{
    if (m_viewMode == mode) return;
    m_viewMode = mode;
    updateMinimumSize();
    recalcHeatmap();
    update();
}

void BScheduleGanttWidget::setGroupMode(GroupMode mode)
{
    if (m_groupMode == mode) return;
    m_groupMode = mode;
    rebuildRows();
    update();
}

void BScheduleGanttWidget::setCurrentDate(const QDate &date)
{
    if (!date.isValid() || date == m_currentDate) return;

    // The bars of the entries belong to the dates shown so far
    if (m_dragState != NoDrag) {
        cancelDrag();
    }

    m_currentDate = date;
    recalcHeatmap();
    update();
}

QDate BScheduleGanttWidget::firstDateFor(ViewMode mode, const QDate &date)
{
    // Week view: Monday to Sunday (Qt: 1 = Monday)
    return mode == WeekView ? date.addDays(1 - date.dayOfWeek()) : date;
}

QVector<int> BScheduleGanttWidget::dayOffsets(const BScheduleEntry &entry) const
{
    QVector<int> offsets;
    const QDate first = firstDate();
    const int days = dayCount();
    for (const QDateTime &when : entry.occurrences) {
        const int d = static_cast<int>(first.daysTo(when.date()));
        // A start that exists twice on a day (end of daylight saving) is one bar
        if (d >= 0 && d < days && !offsets.contains(d)) {
            offsets.append(d);
        }
    }
    return offsets;
}

BJobHistoryStats::Estimate BScheduleGanttWidget::estimateFor(const BScheduleEntry &entry) const
{
    // A schedule no job uses has no runs to measure
    if (entry.jobName.isEmpty()) return BJobHistoryStats::Estimate();

    // The level of the Run line; without one, the job's own
    QString level = entry.levelLabel();
    if (level.isEmpty() && m_jobConfigModel) {
        level = m_jobConfigModel->jobConfigsRaw().value(entry.jobName)
                    .value(QLatin1String("level")).toString();
    }

    const QString cacheKey = entry.jobName + QChar(0x1F) + level;
    const auto cached = m_estimateCache.constFind(cacheKey);
    if (cached != m_estimateCache.constEnd()) return cached.value();

    const BJobHistoryStats::Estimate estimate = m_history.estimate(entry.jobName, level);
    m_estimateCache.insert(cacheKey, estimate);
    return estimate;
}

int BScheduleGanttWidget::durationSecs(const BScheduleEntry &entry) const
{
    const BJobHistoryStats::Estimate estimate = estimateFor(entry);
    if (!estimate.hasHistory()) return 0;

    // 0 is "no history": a run that was measured at under a second is one
    return static_cast<int>(qBound<qint64>(1, estimate.duration.median, 366LL * 86400));
}

void BScheduleGanttWidget::setZoomLevel(int pixelsPerHour)
{
    m_zoomMinPph = qMax(MIN_PIXELS_PER_HOUR, pixelsPerHour);
    m_pixelsPerHour = m_zoomMinPph;
    updateMinimumSize();
    recalcHeatmap();
    update();
}

void BScheduleGanttWidget::updateMetrics()
{
    // Everything is a multiple of the widget's font: the font of the
    // desktop times the interface zoom. A measure given in pixels is the
    // one for a 10 pt font (BUiScale::px()); where text has to fit, the
    // height of that text is the lower limit.
    const QFont base = font();
    m_smallFont = BUiScale::scaled(base, BUiScale::Tiny);
    m_smallBoldFont = BUiScale::scaled(base, BUiScale::Tiny, BUiScale::Bold);
    const QFontMetrics fm(base);
    const QFontMetrics smallFm(m_smallFont);
    const QFontMetrics smallBoldFm(m_smallBoldFont);

    m_metrics.rowHeight = qMax(px(28), fm.height() + px(8));
    m_metrics.headerRow1Height = qMax(px(18), smallBoldFm.height() + px(4));
    m_metrics.headerRow2Height = qMax(px(18), smallFm.height() + px(6));
    m_metrics.headerHeight = m_metrics.headerRow1Height + m_metrics.headerRow2Height;
    m_metrics.labelWidth = px(230);
    m_metrics.heatmapHeight = qMax(px(24), smallFm.height() + px(6));
    m_metrics.noHistoryMarkerWidth = qMax(px(22),
        smallBoldFm.horizontalAdvance(QLatin1Char('?')) + px(12));
    m_metrics.hourLabelWidth = smallFm.horizontalAdvance(QStringLiteral("00:00"));
    // Odd, so that the hourglass and the diamond have a middle pixel
    m_metrics.waitMarkWidth = px(9) | 1;
    m_metrics.unusedMarkWidth = px(9) | 1;

    m_rowHeight = m_metrics.rowHeight;
    m_headerRow1Height = m_metrics.headerRow1Height;
    m_headerRow2Height = m_metrics.headerRow2Height;
    m_headerHeight = m_metrics.headerHeight;
    m_labelWidth = m_metrics.labelWidth;
    m_heatmapHeight = m_metrics.heatmapHeight;
    m_noHistoryMarkerWidth = m_metrics.noHistoryMarkerWidth;
    m_waitMarkWidth = m_metrics.waitMarkWidth;
}

int BScheduleGanttWidget::px(int designPixels) const
{
    return BUiScale::px(designPixels, font());
}

int BScheduleGanttWidget::hourLabelStepMinutes() const
{
    // In day view a label every hour, in week view not closer than the
    // snap. Fewer if the labels would touch: the next step that leaves a
    // gap between two of them.
    const int wanted = (m_viewMode == DayView) ? 60 : qMax(60, m_dragSnapMinutes);
    const int needed = m_metrics.hourLabelWidth + px(6);
    static const int steps[] = {60, 120, 180, 240, 360, 720, 1440};
    for (int step : steps) {
        if (step < wanted) continue;
        if (step * m_pixelsPerHour / 60 >= needed) return step;
    }
    return 1440;
}

void BScheduleGanttWidget::fitToWidth()
{
    // Stretch-to-fit: compute pixels-per-hour from available width
    int availableWidth = width() - m_labelWidth - 20;
    if (availableWidth < 100) availableWidth = 100;

    int hours = totalHours();
    int fitPph = availableWidth / hours;

    // Respect zoom minimum (slider-driven in week mode, default in day mode)
    m_pixelsPerHour = qMax(m_zoomMinPph, fitPph);

    recalcHeatmap();
}

void BScheduleGanttWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    fitToWidth();
}

void BScheduleGanttWidget::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::FontChange || event->type() == QEvent::StyleChange) {
        // Another font or zoom: rows, header and label column follow
        updateMetrics();
        updateMinimumSize();
        fitToWidth();
        updateGeometry();
        update();
    }
}

QSize BScheduleGanttWidget::sizeHint() const
{
    return QSize(m_labelWidth + contentWidth() + 20, contentHeight() + 20);
}

QSize BScheduleGanttWidget::minimumSizeHint() const
{
    return QSize(px(300), m_headerHeight + m_rowHeight * 3 + m_heatmapHeight);
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
    return m_headerHeight + m_rows.size() * m_rowHeight + m_heatmapHeight;
}

void BScheduleGanttWidget::updateMinimumSize()
{
    // Height: content-driven so vertical scrollbar appears
    setMinimumHeight(contentHeight() + 20);

    // Width: in week mode, enforce minimum from zoom so horizontal scrollbar appears
    if (m_viewMode == WeekView) {
        int minWidth = m_labelWidth + totalHours() * m_zoomMinPph + 20;
        setMinimumWidth(minWidth);
    } else {
        setMinimumWidth(0);  // day mode: stretch to fit
    }
}

int BScheduleGanttWidget::timeToX(int hour, int minute) const
{
    double fractionalHour = hour + minute / 60.0;
    return m_labelWidth + static_cast<int>(fractionalHour * m_pixelsPerHour);
}

int BScheduleGanttWidget::xToHour(int x) const
{
    int relX = x - m_labelWidth;
    if (relX < 0) return -1;
    return relX / m_pixelsPerHour;
}

int BScheduleGanttWidget::xToMinute(int x) const
{
    int relX = x - m_labelWidth;
    if (relX < 0) return 0;
    int hourPixels = relX % m_pixelsPerHour;
    return (hourPixels * 60) / m_pixelsPerHour;
}

int BScheduleGanttWidget::rowAtY(int y) const
{
    int relY = y - m_headerHeight;
    if (relY < 0) return -1;
    int row = relY / m_rowHeight;
    if (row >= m_rows.size()) return -1;
    return row;
}

void BScheduleGanttWidget::rebuildRows()
{
    m_rows.clear();

    // The entries by the name of their group. A schedule that no job uses
    // has no client and no storage to be grouped by, and no runs: its
    // entries are a group of their own, the last one.
    QMap<QString, QVector<int>> groups;
    QVector<int> unused;
    for (int i = 0; i < m_entries.size(); ++i) {
        const BScheduleEntry &entry = m_entries[i];
        if (entry.unused) {
            unused.append(i);
            continue;
        }
        QString group;
        switch (m_groupMode) {
        case GroupBySchedule:
            group = entry.scheduleName;
            break;
        case GroupByClient:
            group = entry.client.isEmpty() ? tr("(no client)") : entry.client;
            break;
        case GroupByStorage:
            group = entry.storage.isEmpty() ? tr("(no storage)") : entry.storage;
            break;
        }
        groups[group].append(i);
    }

    auto addGroup = [this](const QString &label, const QVector<int> &entries, bool isUnused) {
        RowInfo header;
        header.label = label;
        header.isGroupHeader = true;
        header.entryIndex = -1;
        header.unused = isUnused;
        m_rows.append(header);

        for (int idx : entries) {
            RowInfo row;
            row.label = rowLabel(m_entries[idx]);
            row.isGroupHeader = false;
            row.entryIndex = idx;
            row.unused = isUnused;
            m_rows.append(row);
        }
    };

    for (auto it = groups.constBegin(); it != groups.constEnd(); ++it)
        addGroup(it.key(), it.value(), false);
    if (!unused.isEmpty())
        addGroup(tr("Not used by any job"), unused, true);

    updateMinimumSize();
}

QString BScheduleGanttWidget::rowLabel(const BScheduleEntry &entry)
{
    // The job first: rows of one schedule differ in nothing else. An entry
    // without a job (no job uses the schedule) is named by its schedule.
    const QString who = entry.jobName.isEmpty() ? entry.scheduleName : entry.jobName;
    const QString level = entry.levelLabel();
    return level.isEmpty() ? QStringLiteral("  %1").arg(who)
                           : QStringLiteral("  %1 \u00B7 %2").arg(who, level);
}

QRect BScheduleGanttWidget::barRect(const BScheduleEntry &entry, int row, int dayOffset) const
{
    int startHour = dayOffset * 24 + entry.hour;
    int x = timeToX(startHour, entry.minute);
    const int gap = px(2);
    int y = m_headerHeight + row * m_rowHeight + gap;

    // A schedule no job uses has no run: the place of its diamond, centred
    // on the time
    if (entry.unused) {
        const int mark = m_metrics.unusedMarkWidth;
        return QRect(x - mark / 2, y, mark, m_rowHeight - 2 * gap);
    }

    // Bar width: the measured median. Without history there is no length,
    // only a marker of fixed size at the start.
    const int secs = durationSecs(entry);
    int barWidth = m_noHistoryMarkerWidth;
    if (secs > 0) {
        barWidth = qMax(px(6), static_cast<int>(secs / 3600.0 * m_pixelsPerHour));
    }

    return QRect(x, y, barWidth, m_rowHeight - 2 * gap);
}

QRect BScheduleGanttWidget::waitRect(const BScheduleEntry &entry, int row, int dayOffset) const
{
    const RunWait *wait = waitFor(entry, dayOffset);
    if (!wait || wait->waitSecs <= 0) return QRect();

    // As long as the wait and no longer: a wait of seconds has no length
    // at this scale. That the run waits is said by the hourglass.
    const int width = static_cast<int>(wait->waitSecs / 3600.0 * m_pixelsPerHour);
    if (width < 1) return QRect();
    const QRect bar = barRect(entry, row, dayOffset);
    return QRect(bar.left() + waitMarkShift(entry, dayOffset), bar.top(), width, bar.height());
}

int BScheduleGanttWidget::waitMarkShift(const BScheduleEntry &entry, int dayOffset) const
{
    const RunWait *wait = waitFor(entry, dayOffset);
    if (!wait || wait->waitSecs <= 0) return 0;
    // The hourglass stands in front of the scheduled time. A run at the
    // very beginning of the dates on display has no room there: the mark
    // takes the place of the start and everything else moves behind it.
    const int x = timeToX(dayOffset * 24 + entry.hour, entry.minute);
    return (x - m_waitMarkWidth - 1 < m_labelWidth) ? m_waitMarkWidth + 1 : 0;
}

QRect BScheduleGanttWidget::waitMarkRect(const BScheduleEntry &entry, int row, int dayOffset) const
{
    const RunWait *wait = waitFor(entry, dayOffset);
    if (!wait || wait->waitSecs <= 0) return QRect();

    const QRect bar = barRect(entry, row, dayOffset);
    const int left = waitMarkShift(entry, dayOffset) > 0 ? bar.left()
                                                         : bar.left() - m_waitMarkWidth - 1;
    return QRect(left, bar.top(), m_waitMarkWidth, bar.height());
}

QRect BScheduleGanttWidget::runRect(const BScheduleEntry &entry, int row, int dayOffset) const
{
    const QRect bar = barRect(entry, row, dayOffset);
    const QRect lead = waitRect(entry, row, dayOffset);
    return bar.translated(waitMarkShift(entry, dayOffset) + lead.width(), 0);
}

bool BScheduleGanttWidget::startContains(const BScheduleEntry &entry, int row, int dayOffset,
                                         const QPoint &pos) const
{
    return runRect(entry, row, dayOffset).contains(pos)
        || waitRect(entry, row, dayOffset).contains(pos)
        || waitMarkRect(entry, row, dayOffset).contains(pos);
}

// ============================================================================
// Painting
// ============================================================================

void BScheduleGanttWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background — drawn via style engine so QSS background-color is honored
    QStyleOption opt;
    opt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    // Time header
    drawTimeHeader(painter);

    // Row content
    drawRows(painter);

    if (m_rows.isEmpty()) {
        painter.setPen(palette().text().color());
        painter.drawText(QRect(m_labelWidth, m_headerHeight, contentWidth(), m_rowHeight * 2),
                         Qt::AlignCenter,
                         m_viewMode == WeekView ? tr("No runs in this week")
                                                : tr("No runs on this date"));
    }

    // Heatmap
    drawHeatmap(painter);

    // Drag overlay (ghost bar and preview)
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
    QRect headerBg(m_labelWidth, 0, width() - m_labelWidth, m_headerHeight);
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

    font = m_smallBoldFont;
    painter.setFont(font);
    const QFontMetrics dayFm(font);

    // Derive palette-based colors for the header
    QColor hlBase = palette().highlight().color();
    QColor hlTintEven = hlBase; hlTintEven.setAlpha(40);
    QColor hlTintOdd  = hlBase; hlTintOdd.setAlpha(20);
    QColor hlTintDay  = hlBase; hlTintDay.setAlpha(30);
    QColor hlSep      = hlBase; hlSep.setAlpha(100);

    const QDate first = firstDate();

    if (m_viewMode == WeekView) {
        // Week: one label per day spanning 24 hours
        for (int d = 0; d < 7; ++d) {
            int x1 = timeToX(d * 24, 0);
            int x2 = timeToX((d + 1) * 24, 0);
            int dayWidth = x2 - x1;

            // Alternating day background in row 1
            painter.fillRect(x1, 0, dayWidth, m_headerRow1Height,
                             (d % 2 == 0) ? hlTintEven : hlTintOdd);

            // Day name and date — shorter forms if too narrow
            const QDate date = first.addDays(d);
            QString label;
            const QString longLabel = QString("%1, %2").arg(dayNames[d], date.toString(Qt::ISODate));
            const QString shortLabel = QString("%1 %2").arg(dayAbbrev[d], date.toString("MM-dd"));
            const int room = dayWidth - px(8);
            if (dayFm.horizontalAdvance(longLabel) <= room)
                label = longLabel;
            else if (dayFm.horizontalAdvance(shortLabel) <= room)
                label = shortLabel;
            else
                label = dayAbbrev[d];
            painter.setPen(palette().text().color());
            QRect textRect(x1, 0, dayWidth, m_headerRow1Height);
            painter.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter, label);

            // Vertical separator between days
            painter.setPen(QPen(hlSep, 1));
            painter.drawLine(x1, 0, x1, m_headerRow1Height);
        }
    } else {
        // Day mode: day name and date centered
        int dayIdx = first.dayOfWeek() - 1;  // Qt: 1=Mon → 0=Mon
        painter.fillRect(m_labelWidth, 0, width() - m_labelWidth, m_headerRow1Height, hlTintDay);
        painter.setPen(palette().text().color());
        QRect textRect(m_labelWidth, 0, contentWidth(), m_headerRow1Height);
        painter.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter,
                         QString("%1, %2").arg(dayNames[dayIdx], first.toString(Qt::ISODate)));
    }

    // Separator line between row 1 and row 2
    painter.setPen(QPen(palette().mid().color(), 1));
    painter.drawLine(m_labelWidth, m_headerRow1Height, width(), m_headerRow1Height);

    // === Row 2: Hour labels ===
    font = m_smallFont;
    painter.setFont(font);

    int labelIntervalMin = hourLabelStepMinutes();
    int totalMinutes = hours * 60;
    const int tick = px(4);
    const int labelBox = m_metrics.hourLabelWidth + px(6);

    for (int m = 0; m < totalMinutes; m += labelIntervalMin) {
        int h = m / 60;
        int min = m % 60;
        int x = timeToX(h, min);

        // Tick line at bottom of row 2
        painter.setPen(QPen(palette().mid().color(), 1));
        painter.drawLine(x, m_headerHeight - tick, x, m_headerHeight);

        // Hour label
        int displayHour = h % 24;
        QString label = QString("%1:%2")
            .arg(displayHour, 2, 10, QChar('0'))
            .arg(min, 2, 10, QChar('0'));

        painter.setPen(palette().text().color());
        QRect textRect(x - labelBox / 2, m_headerRow1Height, labelBox, m_headerRow2Height - tick);
        painter.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter | Qt::TextDontClip, label);
    }

    // Bottom border
    painter.setPen(QPen(palette().mid().color(), 1));
    painter.drawLine(m_labelWidth, m_headerHeight, width(), m_headerHeight);

    painter.restore();
}

void BScheduleGanttWidget::drawNowMarker(QPainter &painter)
{
    QDateTime now = QDateTime::currentDateTime();
    int currentHour = now.time().hour();
    int currentMinute = now.time().minute();

    // Only if today is one of the dates on display
    const int todayOffset = static_cast<int>(firstDate().daysTo(now.date()));
    if (todayOffset < 0 || todayOffset >= dayCount()) return;
    int hourOffset = todayOffset * 24;

    int x = timeToX(hourOffset + currentHour, currentMinute);

    painter.save();
    painter.setPen(QPen(Qt::red, 2));
    painter.drawLine(x, m_headerHeight, x, m_headerHeight + m_rows.size() * m_rowHeight);

    // Small triangle at top
    QPolygonF triangle;
    triangle << QPointF(x - px(4), m_headerHeight)
             << QPointF(x + px(4), m_headerHeight)
             << QPointF(x, m_headerHeight + px(6));
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
        int y = m_headerHeight + r * m_rowHeight;

        // Alternating row background
        if (r % 2 == 0) {
            painter.fillRect(m_labelWidth, y, contentWidth(), m_rowHeight,
                             palette().alternateBase());
        }

        // Label. What no job uses is written in muted italics: these rows
        // are not runs.
        QRect labelRect(px(4), y, m_labelWidth - 2 * px(4), m_rowHeight);
        const QFont rowFont = painter.font();
        QColor labelColor = palette().text().color();
        if (row.unused) {
            QFont font = rowFont;
            font.setItalic(true);
            painter.setFont(font);
            labelColor = mutedColor();
        }
        if (row.isGroupHeader) {
            QFont font = painter.font();
            font.setBold(true);
            painter.setFont(font);
            painter.setPen(labelColor);
            painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter,
                             painter.fontMetrics().elidedText(row.label, Qt::ElideRight,
                                                              labelRect.width()));

            // Group header background — palette-derived tint, works in dark and light
            QColor groupTint = palette().highlight().color();
            groupTint.setAlpha(20);
            painter.fillRect(m_labelWidth, y, contentWidth(), m_rowHeight, groupTint);
        } else {
            painter.setPen(labelColor);
            painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter,
                             painter.fontMetrics().elidedText(row.label, Qt::ElideRight,
                                                              labelRect.width()));

            // One bar for each date on display on which the entry starts
            if (row.entryIndex >= 0 && row.entryIndex < m_entries.size()) {
                for (int d : dayOffsets(m_entries[row.entryIndex])) {
                    if (row.unused)
                        drawUnusedMark(painter, row.entryIndex, r, d);
                    else
                        drawJobBar(painter, row.entryIndex, r, d);
                }
            }
        }
        painter.setFont(rowFont);

        // Row separator line
        painter.setPen(QPen(palette().mid().color(), 0.5));
        painter.drawLine(0, y + m_rowHeight, m_labelWidth + contentWidth(), y + m_rowHeight);
    }

    // Grid: vertical lines at snap interval
    int snapMin = qMax(1, m_dragSnapMinutes);
    int totalMin = totalHours() * 60;
    int rowsBottom = m_headerHeight + m_rows.size() * m_rowHeight;
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
        painter.drawLine(x, m_headerHeight, x, rowsBottom);
    }

    painter.restore();
}

void BScheduleGanttWidget::drawJobBar(QPainter &painter, int entryIndex,
                                       int row, int dayOffset)
{
    const BScheduleEntry &entry = m_entries[entryIndex];
    // The bar begins where the run is predicted to start; before it, from
    // the scheduled time on, is the lead-in of its wait
    QRect rect = runRect(entry, row, dayOffset);
    const QRect lead = waitRect(entry, row, dayOffset);
    const QRect waitMark = waitMarkRect(entry, row, dayOffset);
    const RunWait *wait = waitFor(entry, dayOffset);
    bool hovered = (row == rowAtY(m_hoverPos.y())
                    && startContains(entry, row, dayOffset, m_hoverPos));

    QColor fillColor = colorForLevel(entry.level, hovered);
    const BJobHistoryStats::Estimate estimate = estimateFor(entry);

    // Nothing is drawn outside the dates on display: a run that waits past
    // their end has its lead-in cut there
    painter.save();
    const int contentRight = m_labelWidth + contentWidth();
    painter.setClipRect(QRect(m_labelWidth, m_headerHeight, contentWidth() + 1,
                              m_rows.size() * m_rowHeight));

    if (!waitMark.isEmpty()) {
        // Waiting: an hourglass of fixed size in front of the scheduled
        // time. It is a mark, not a length, and is there at every zoom,
        // also where the wait itself is shorter than a pixel. Filled for a
        // wait that is certain, an outline for one that is not.
        const QColor ink = palette().text().color();
        const QRectF glass = QRectF(waitMark).adjusted(0.5, px(4) + 0.5, -0.5, -px(4) - 0.5);
        QPolygonF hourglass;
        hourglass << glass.topLeft() << glass.topRight() << glass.center()
                  << glass.bottomRight() << glass.bottomLeft() << glass.center();
        QColor fill = ink;
        fill.setAlpha(hovered ? 255 : 215);
        painter.setPen(QPen(ink, 1));
        painter.setBrush(wait->certain ? QBrush(fill) : QBrush(Qt::NoBrush));
        painter.drawPolygon(hourglass);
        painter.setBrush(Qt::NoBrush);
        // The scheduled time: a tick of full height in the level's colour
        const int tickX = waitMark.right() + 1;
        painter.setPen(QPen(fillColor, 2));
        painter.drawLine(tickX, waitMark.top(), tickX, waitMark.bottom());
    }
    if (!lead.isEmpty()) {
        // As long as the wait: a hatched strip, thinner than the bar, from
        // the scheduled time to the predicted start. An uncertain wait has
        // a dashed outline and a question mark; hatching and outline carry
        // the meaning, not a colour.
        const QColor ink = palette().text().color();
        QColor hatch = ink;
        hatch.setAlpha(hovered ? 230 : 170);
        const QRect strip = lead.adjusted(0, px(6), 0, -px(6));
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(hatch, Qt::BDiagPattern));
        painter.drawRect(strip);
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(ink, 1, wait->certain ? Qt::SolidLine : Qt::DashLine));
        painter.drawRect(strip.adjusted(0, 0, -1, -1));

        const int markWidth = QFontMetrics(BUiScale::scaled(font(), BUiScale::Normal, BUiScale::Bold))
                                  .horizontalAdvance(QStringLiteral("\u00BB")) + px(4);
        if (rect.left() >= contentRight && lead.left() < contentRight - markWidth) {
            // The start is after the dates on display
            QFont markFont = font();
            markFont.setBold(true);
            painter.setFont(markFont);
            const QRect markRect(contentRight - markWidth - 1, lead.top(), markWidth, lead.height());
            painter.fillRect(markRect.adjusted(0, px(4), 0, -px(4)), palette().base());
            painter.setPen(ink);
            painter.drawText(markRect, Qt::AlignCenter, QStringLiteral("\u00BB"));
        }
    }
    if (wait && !wait->certain) {
        // Not certain: a question mark in front, also where no wait is
        // predicted (the run may have to wait)
        QFont markFont = font();
        markFont.setBold(true);
        painter.setFont(markFont);
        painter.setPen(palette().text().color());
        const int markX = waitMark.isEmpty() ? rect.left() : waitMark.left();
        const int questionWidth = painter.fontMetrics().horizontalAdvance(QLatin1Char('?')) + px(2);
        painter.drawText(QRect(markX - questionWidth - 1, rect.top(), questionWidth, rect.height()),
                         Qt::AlignCenter, QStringLiteral("?"));
    }

    if (!estimate.hasHistory()) {
        // No history: nothing was measured, so there is no bar. A dashed
        // outline of fixed size marks the start.
        painter.setPen(QPen(fillColor, 1.5, Qt::DashLine));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(rect.adjusted(0, 0, -1, -1), 3, 3);
    } else if (estimate.isMeasured()) {
        // Main bar: the measured median
        painter.setPen(Qt::NoPen);
        painter.setBrush(fillColor);
        painter.drawRoundedRect(rect, 3, 3);
    } else {
        // Estimate from the job's runs at other levels: paler, dashed
        QColor pale = fillColor;
        pale.setAlpha(110);
        painter.setPen(QPen(fillColor, 1.5, Qt::DashLine));
        painter.setBrush(pale);
        painter.drawRoundedRect(rect.adjusted(0, 0, -1, -1), 3, 3);
    }

    // 90th percentile: a lighter extension of the bar with a mark at its end
    if (estimate.hasHistory() && estimate.duration.p90 > estimate.duration.median) {
        const int p90Width = static_cast<int>(estimate.duration.p90 / 3600.0 * m_pixelsPerHour);
        if (p90Width > rect.width()) {
            QRect tailRect(rect.right(), rect.top(),
                           p90Width - rect.width(), rect.height());
            QColor tailColor = fillColor;
            tailColor.setAlpha(60);
            painter.setPen(Qt::NoPen);
            painter.setBrush(tailColor);
            painter.drawRect(tailRect);
            painter.setPen(QPen(fillColor, 2));
            painter.drawLine(tailRect.right(), rect.top(), tailRect.right(), rect.bottom());
        }
    }

    // Inside: a question mark without history, otherwise the level letter
    // if the bar is wide enough ("~" in front for an estimate)
    painter.setFont(m_smallBoldFont);
    if (!estimate.hasHistory()) {
        painter.setPen(palette().text().color());
        painter.drawText(rect, Qt::AlignCenter, QStringLiteral("?"));
    } else {
        QString levelChar = entry.levelLabel().left(1);
        if (!estimate.isMeasured()) levelChar.prepend(QLatin1Char('~'));
        // Only if the bar has room for it
        if (rect.width() >= painter.fontMetrics().horizontalAdvance(levelChar) + px(10)) {
            painter.setPen(estimate.isMeasured() ? QColor(Qt::white) : palette().text().color());
            painter.drawText(rect, Qt::AlignCenter, levelChar);
        }
    }
    painter.setFont(m_smallFont);

    painter.restore();      // the clip
}

QColor BScheduleGanttWidget::mutedColor() const
{
    // Between the text and the background: readable, and clearly not the
    // colour of what is active
    const QColor text = palette().text().color();
    const QColor base = palette().base().color();
    return QColor((text.red() * 3 + base.red() * 2) / 5,
                  (text.green() * 3 + base.green() * 2) / 5,
                  (text.blue() * 3 + base.blue() * 2) / 5);
}

void BScheduleGanttWidget::drawUnusedMark(QPainter &painter, int entryIndex, int row, int dayOffset)
{
    // Where a run would start if a job used the schedule: an open diamond
    // in the muted colour. No bar, no level colour, no question mark:
    // nothing runs here.
    const BScheduleEntry &entry = m_entries[entryIndex];
    const QRect rect = barRect(entry, row, dayOffset);
    const bool hovered = row == rowAtY(m_hoverPos.y()) && rect.contains(m_hoverPos);

    painter.save();
    painter.setClipRect(QRect(m_labelWidth, m_headerHeight, contentWidth() + 1,
                              m_rows.size() * m_rowHeight));
    const qreal half = (rect.width() - 1) / 2.0;
    const QPointF centre(rect.left() + half + 0.5, rect.center().y() + 0.5);
    QPolygonF diamond;
    diamond << QPointF(centre.x(), centre.y() - half) << QPointF(centre.x() + half, centre.y())
            << QPointF(centre.x(), centre.y() + half) << QPointF(centre.x() - half, centre.y());
    painter.setPen(QPen(hovered ? palette().text().color() : mutedColor(), 1.2));
    painter.setBrush(Qt::NoBrush);
    painter.drawPolygon(diamond);
    painter.restore();
}

void BScheduleGanttWidget::drawHeatmap(QPainter &painter)
{
    if (m_heatmapSlots.isEmpty()) return;

    painter.save();

    int y = m_headerHeight + m_rows.size() * m_rowHeight + px(2);
    int slotCount = m_heatmapSlots.size();

    // Label
    QRect labelRect(px(4), y, m_labelWidth - 2 * px(4), m_heatmapHeight);
    painter.setPen(palette().text().color());
    painter.setFont(m_smallFont);
    painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, tr("Utilization"));
    const int unknownStrip = px(4);
    const int waitingStrip = px(7);

    double slotWidth = static_cast<double>(contentWidth()) / slotCount;

    // One hue, stronger with every run that is running: several runs at
    // the same time are what the limits allow, not a fault
    const QColor running = palette().highlight().color();
    const QColor ink = palette().text().color();

    for (int i = 0; i < slotCount; ++i) {
        int count = m_heatmapSlots[i];
        QColor color;

        if (count == 0) {
            color = palette().mid().color();
            color.setAlpha(80);
        } else {
            color = running;
            color.setAlpha(qMin(235, 70 + 33 * (count - 1)));
        }

        int x = m_labelWidth + static_cast<int>(i * slotWidth);
        int w = static_cast<int>((i + 1) * slotWidth) - static_cast<int>(i * slotWidth);
        painter.fillRect(x, y, w, m_heatmapHeight, color);

        // A run without history starts here: it is not in the count of any
        // slot, because nobody knows how many slots it covers
        if (m_heatmapUnknown.value(i) > 0) {
            painter.fillRect(x, y, qMax(2, w), unknownStrip,
                             QBrush(palette().text().color(), Qt::BDiagPattern));
        }

        // Runs are predicted to wait in this slot: the hatching of the
        // lead-ins, as a strip along the bottom
        if (m_heatmapWaiting.value(i) > 0) {
            const QRect strip(x, y + m_heatmapHeight - waitingStrip, qMax(2, w), waitingStrip);
            painter.fillRect(strip, palette().base());
            painter.fillRect(strip, QBrush(ink, Qt::BDiagPattern));
        }
    }

    // Border
    painter.setPen(QPen(palette().mid().color(), 1));
    painter.drawRect(m_labelWidth, y, contentWidth(), m_heatmapHeight);

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
    m_heatmapUnknown.fill(0, slotCount);
    m_heatmapWaiting.fill(0, slotCount);

    const QDate first = firstDate();
    const int days = dayCount();

    for (const BScheduleEntry &entry : m_entries) {
        // A schedule no job uses starts nothing
        if (entry.unused) continue;

        // Measured median; 0 = no history, no length
        const int secs = durationSecs(entry);
        const int slotSecs = HEATMAP_SLOT_MINUTES * 60;
        const int durationSlots = qMax(1, (secs + slotSecs - 1) / slotSecs);

        // Every start in the dates on display
        for (const QDateTime &when : entry.occurrences) {
            const int d = static_cast<int>(first.daysTo(when.date()));
            if (d < 0 || d >= days) continue;

            const int scheduledMinute = d * 24 * 60 + when.time().hour() * 60
                                        + when.time().minute();

            // The run is counted from its predicted start; until then it
            // is waiting
            int waitMinutes = 0;
            const auto waitIt = m_waits.constFind(waitKey(entry, when));
            if (waitIt != m_waits.constEnd() && waitIt->waitSecs > 0) {
                waitMinutes = static_cast<int>((waitIt->waitSecs + 59) / 60);
                const int firstWaiting = scheduledMinute / HEATMAP_SLOT_MINUTES;
                const int lastWaiting = (scheduledMinute + waitMinutes - 1) / HEATMAP_SLOT_MINUTES;
                for (int s = qMax(0, firstWaiting); s <= lastWaiting && s < slotCount; ++s) {
                    m_heatmapWaiting[s]++;
                }
            }

            int startSlot = (scheduledMinute + waitMinutes) / HEATMAP_SLOT_MINUTES;

            if (secs <= 0) {
                // Marked at its start, counted nowhere
                if (startSlot >= 0 && startSlot < slotCount) m_heatmapUnknown[startSlot]++;
                continue;
            }

            for (int s = startSlot; s < startSlot + durationSlots && s < slotCount; ++s) {
                if (s >= 0) m_heatmapSlots[s]++;
            }
        }
    }
}

// ============================================================================
// Colors
// ============================================================================

QColor BScheduleGanttWidget::colorForLevel(BScheduleEntry::BackupLevel level, bool hovered) const
{
    QString code;
    switch (level) {
    case BScheduleEntry::Full:         code = "F"; break;
    case BScheduleEntry::Differential: code = "D"; break;
    case BScheduleEntry::Incremental:  code = "I"; break;
    case BScheduleEntry::VirtualFull:  code = "V"; break;
    case BScheduleEntry::None:
    default: {
        // The Run line names no level (the job's level applies) or one
        // without a colour: neutral, so that the bar stays visible
        const QColor neutral = palette().color(QPalette::Mid);
        return hovered ? neutral.lighter(130) : neutral;
    }
    }

    QColor base = BLevelColors::getLevelColor(code);
    if (hovered)
        return base.lighter(130);
    return base;
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

    // The bar where it is drawn, the lead-in of its wait and its hourglass
    for (int d : dayOffsets(entry)) {
        if (startContains(entry, row, d, pos))
            return entryIdx;
    }

    return -1;
}

void BScheduleGanttWidget::mouseMoveEvent(QMouseEvent *event)
{
    // --- Drag state transitions ---
    // The entry list can change between press and move; never index with a
    // stale drag index.
    if (m_dragState != NoDrag && !dragIndexValid()) {
        cancelDrag();
    }

    if (m_dragState == DragPending) {
        if ((event->pos() - m_dragStartPos).manhattanLength() > 5) {
            m_dragState = Dragging;
            setCursor(Qt::ClosedHandCursor);
            setMouseTracking(true);
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
        const int snapMin = qMax(1, m_dragSnapMinutes);
        rawMinutes = (rawMinutes / snapMin) * snapMin;
        m_dragNewHour = rawMinutes / 60;
        m_dragNewMinute = rawMinutes % 60;

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

    if (idx >= 0 && m_entries[idx].unused) {
        // Not a run: say so, and nothing about durations or waits
        const BScheduleEntry &e = m_entries[idx];
        QString tooltip = QString("<b>%1</b><br>%2").arg(
            e.scheduleName.toHtmlEscaped(),
            tr("Not used by any enabled job: nothing starts at this time."));
        tooltip += QString("<br>%1: %2:%3").arg(tr("Time"))
                       .arg(e.hour, 2, 10, QChar('0')).arg(e.minute, 2, 10, QChar('0'));
        if (!e.levelLabel().isEmpty())
            tooltip += QString("<br>%1: %2").arg(tr("Level"), e.levelLabel());
        if (!e.source.original.isEmpty())
            tooltip += QString("<br>%1: %2").arg(tr("Run"), e.source.original.toHtmlEscaped());
        QToolTip::showText(event->globalPosition().toPoint(), tooltip, this);
    } else if (idx >= 0) {
        const BScheduleEntry &e = m_entries[idx];
        QString tooltip = QString("<b>%1</b><br>"
                                  "%2: %3<br>"
                                  "%4: %5:%6")
            .arg((e.jobName.isEmpty() ? e.scheduleName
                                      : QString("%1 (%2)").arg(e.jobName, e.scheduleName))
                     .toHtmlEscaped(),
                 tr("Level"),
                 e.levelLabel().isEmpty() ? tr("(level of the job)") : e.levelLabel(),
                 tr("Time"),
                 QString::number(e.hour).rightJustified(2, '0'),
                 QString::number(e.minute).rightJustified(2, '0'));

        // The Run line itself: the bars show when it runs in the dates on
        // display, the line says when it runs at all
        if (!e.source.original.isEmpty()) {
            tooltip += QString("<br>%1: %2").arg(tr("Run"), e.source.original.toHtmlEscaped());
        }

        if (!e.jobName.isEmpty()) {
            tooltip += QString("<br>%1: %2").arg(tr("Schedule"), e.scheduleName.toHtmlEscaped());
        }
        if (!e.client.isEmpty()) {
            tooltip += QString("<br>%1: %2").arg(tr("Client"), e.client);
        }
        // What is predicted for the start under the pointer: whether it
        // waits, for which limit or rule, and for which runs
        if (const RunWait *wait = waitFor(e, dayOffsetAtPos(event->pos(), idx))) {
            if (wait->lines.isEmpty()) {
                tooltip += QString("<br>%1: %2").arg(tr("Predicted wait"), tr("none"));
            } else {
                tooltip += QString("<br><b>%1</b>").arg(
                    wait->waitSecs > 0 ? tr("Predicted wait") : tr("Predicted wait: none"));
                for (const QString &line : wait->lines) {
                    tooltip += QString("<br>") + line.toHtmlEscaped();
                }
            }
        }
        // What was measured, or that nothing was
        const BJobHistoryStats::Estimate estimate = estimateFor(e);
        if (!estimate.hasHistory()) {
            QString why;
            if (e.jobName.isEmpty()) {
                why = tr("the jobs of the Director are not loaded: it is not known which job "
                         "runs this schedule");
            } else if (!m_historyNote.isEmpty()) {
                why = m_historyNote;
            } else {
                why = tr("no successful run of this job in the last %1 days")
                          .arg(m_history.windowDays());
            }
            tooltip += QString("<br><b>%1</b> (%2)<br><i>%3</i>")
                .arg(tr("No history"), why.toHtmlEscaped(),
                     tr("Duration unknown: the outline marks the start only."));
        } else {
            tooltip += QString("<br>%1: %2 (%3: %4, %5 %6)")
                .arg(tr("Median duration"),
                     BJobHistoryStats::formatDuration(estimate.duration.median),
                     tr("90%"),
                     BJobHistoryStats::formatDuration(estimate.duration.p90))
                .arg(estimate.runs).arg(tr("runs"));
            if (estimate.wait.isValid()) {
                tooltip += QString("<br>%1: %2 (%3: %4)")
                    .arg(tr("Median wait"),
                         BJobHistoryStats::formatDuration(estimate.wait.median),
                         tr("90%"),
                         BJobHistoryStats::formatDuration(estimate.wait.p90));
            } else {
                tooltip += QString("<br>%1: %2").arg(tr("Wait"), tr("unknown"));
            }
            if (!estimate.isMeasured()) {
                tooltip += QString("<br><i>%1</i>")
                    .arg(tr("Estimate: no run at this level in the last %1 days, "
                            "figures from all levels of the job.")
                             .arg(m_history.windowDays()));
            }
        }
        if (!e.pool.isEmpty()) {
            tooltip += QString("<br>%1: %2").arg(tr("Pool"), e.pool);
        }
        if (!e.storage.isEmpty()) {
            tooltip += QString("<br>%1: %2").arg(tr("Storage"), e.storage);
        }
        if (m_jobConfigModel && !e.jobName.isEmpty()) {
            const auto &raw = m_jobConfigModel->jobConfigsRaw();
            if (raw.contains(e.jobName)) {
                const QJsonObject &cfg = raw[e.jobName];
                QString type = cfg["type"].toString();
                QString fileset = cfg["fileset"].toString();
                if (!type.isEmpty())
                    tooltip += QString("<br>%1: %2").arg(tr("Type"), type);
                if (!fileset.isEmpty())
                    tooltip += QString("<br>%1: %2").arg(tr("FileSet"), fileset);
            }
        }
        if (e.priority != 10) {
            tooltip += QString("<br>%1: %2").arg(tr("Priority")).arg(e.priority);
        }

        QToolTip::showText(event->globalPosition().toPoint(), tooltip, this);
    } else {
        // Check heatmap hover
        int hmY = m_headerHeight + m_rows.size() * m_rowHeight + px(2);
        if (event->pos().y() >= hmY && event->pos().y() <= hmY + m_heatmapHeight
            && event->pos().x() >= m_labelWidth) {

            int relX = event->pos().x() - m_labelWidth;
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
                    .arg(tr("job(s) running"));

                const int waiting = m_heatmapWaiting.value(slotIdx);
                if (waiting > 0) {
                    tooltip += QString("; ") + tr("%1 run(s) predicted to wait").arg(waiting);
                }

                const int unknown = m_heatmapUnknown.value(slotIdx);
                if (unknown > 0) {
                    tooltip += QString("; ") + tr("%1 run(s) without history start here "
                                                  "(not counted: duration unknown)").arg(unknown);
                }

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

        // Dropping changes nothing here: the bar goes back to its time
        // first, then whoever listens is told where it was dropped. Only
        // if the time is another one and the entry still exists.
        const int index = m_dragEntryIndex;
        const bool moved = dragIndexValid()
            && (m_dragNewHour != m_dragOriginalHour || m_dragNewMinute != m_dragOriginalMinute);
        const int newHour = m_dragNewHour;
        const int newMinute = m_dragNewMinute;

        cancelDrag();
        if (moved) emit dragDropped(index, newHour, newMinute);
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
    for (int d : dayOffsets(entry)) {
        if (startContains(entry, row, d, pos))
            return d;
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

    // --- 3. The time: in the preview bar if it has room, otherwise beside
    // it (a run of minutes is a narrow bar) ---
    QFont font = m_smallBoldFont;
    painter.setFont(font);
    QString timeLabel = QString("%1:%2")
        .arg(m_dragNewHour, 2, 10, QChar('0'))
        .arg(m_dragNewMinute, 2, 10, QChar('0'));
    const int labelWidth = painter.fontMetrics().horizontalAdvance(timeLabel) + px(8);
    if (previewRect.width() >= labelWidth) {
        painter.setPen(Qt::white);
        painter.drawText(previewRect, Qt::AlignCenter, timeLabel);
    } else {
        // To the right, or to the left where the dates on display end
        QRect labelRect(previewRect.right() + px(3), previewRect.top(), labelWidth,
                        previewRect.height());
        if (labelRect.right() > m_labelWidth + contentWidth())
            labelRect.moveRight(previewRect.left() - px(3));
        painter.fillRect(labelRect.adjusted(0, px(2), 0, -px(2)), palette().base());
        painter.setPen(palette().text().color());
        painter.drawText(labelRect, Qt::AlignCenter, timeLabel);
    }
    font.setBold(false);
    painter.setFont(font);

    // --- 4. Snap line: vertical dotted line at snap position ---
    int snapX = timeToX(m_dragDayOffset * 24 + m_dragNewHour, m_dragNewMinute);
    QColor snapColor = palette().mid().color(); snapColor.setAlpha(150);
    QPen snapPen(snapColor, 1, Qt::DotLine);
    painter.setPen(snapPen);
    painter.drawLine(snapX, m_headerHeight, snapX, m_headerHeight + m_rows.size() * m_rowHeight);

    painter.restore();
}
