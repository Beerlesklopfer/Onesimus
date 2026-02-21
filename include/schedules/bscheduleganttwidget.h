#ifndef BSCHEDULEGANTTWIDGET_H
#define BSCHEDULEGANTTWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QComboBox>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QToolButton>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QList>
#include "models/bresourcemodels.h"

/**
 * @brief Gantt/Timeline widget for schedule visualization
 *
 * Horizontal timeline showing backup jobs as colored bars with duration.
 * Y-axis: Jobs grouped by Client/Schedule
 * X-axis: Time (24h day view or 7x24h week view)
 *
 * Features:
 * - Job bars colored by backup level (Full=Green, Diff=Orange, Inc=Blue)
 * - Bar width represents estimated duration from historical data
 * - Heatmap at bottom showing utilization per time slot
 * - Collision detection for overlapping jobs on same client/storage
 * - Zoom control (15min to 1h blocks)
 * - Now-marker (red vertical line at current time)
 * - Tooltips with job details
 */
class BScheduleGanttWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BScheduleGanttWidget(QWidget *parent = nullptr);
    ~BScheduleGanttWidget();

    enum ViewMode {
        DayView,
        WeekView
    };

    enum GroupMode {
        GroupByClient,
        GroupBySchedule
    };

    /**
     * @brief Sets the schedule entries to display
     */
    void setEntries(const QList<BScheduleEntry> &entries);

    /**
     * @brief Sets the duration statistics for bar width calculation
     */
    void setDurationStats(BJobDurationStats *stats);

    /**
     * @brief Sets the current view mode (Day/Week)
     */
    void setViewMode(ViewMode mode);
    ViewMode viewMode() const { return m_viewMode; }

    /**
     * @brief Sets how jobs are grouped on Y-axis
     */
    void setGroupMode(GroupMode mode);
    GroupMode groupMode() const { return m_groupMode; }

    /**
     * @brief Sets the day to display (0=Monday for week view, specific day for day view)
     */
    void setCurrentDay(int dayOfWeek);
    int currentDay() const { return m_currentDay; }

    /**
     * @brief Sets zoom level (pixels per hour)
     */
    void setZoomLevel(int pixelsPerHour);
    int zoomLevel() const { return m_pixelsPerHour; }

    /**
     * @brief Returns all detected collisions
     */
    struct Collision {
        enum Type { ClientCollision, StorageCollision };
        Type type;
        BScheduleEntry entry1;
        BScheduleEntry entry2;
        int startHour;
        int startMinute;
        QString description;
    };
    QList<Collision> collisions() const { return m_collisions; }

    // Zoom range constants
    static const int MIN_PIXELS_PER_HOUR = 30;
    static const int MAX_PIXELS_PER_HOUR = 240;
    static const int DEFAULT_PIXELS_PER_HOUR = 60;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void entryClicked(const BScheduleEntry &entry);
    void entryDoubleClicked(const BScheduleEntry &entry);
    void collisionsDetected(int count);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    // --- Layout constants ---
    static const int ROW_HEIGHT = 28;
    static const int HEADER_HEIGHT = 32;
    static const int LABEL_WIDTH = 180;
    static const int HEATMAP_HEIGHT = 24;
    static const int GROUP_HEADER_HEIGHT = 22;
    static const int HEATMAP_SLOT_MINUTES = 15;

    // --- Drawing ---
    void drawTimeHeader(QPainter &painter);
    void drawNowMarker(QPainter &painter);
    void drawRows(QPainter &painter);
    void drawJobBar(QPainter &painter, const BScheduleEntry &entry,
                    int row, int dayOffset);
    void drawHeatmap(QPainter &painter);
    void drawCollisionMarkers(QPainter &painter);

    // --- Layout calculation ---
    struct RowInfo {
        QString label;
        bool isGroupHeader;
        int entryIndex;  // -1 for group headers
    };
    void rebuildRows();
    int totalHours() const;
    int timeToX(int hour, int minute) const;
    int xToHour(int x) const;
    int xToMinute(int x) const;
    int rowAtY(int y) const;
    QRect barRect(const BScheduleEntry &entry, int row, int dayOffset) const;
    int contentHeight() const;
    int contentWidth() const;

    // --- Heatmap calculation ---
    void recalcHeatmap();
    int heatmapSlotCount() const;

    // --- Collision detection ---
    void detectCollisions();

    // --- Colors ---
    QColor colorForLevel(BScheduleEntry::BackupLevel level, bool hovered = false) const;

    // --- Hit testing ---
    int entryAtPos(const QPoint &pos) const;

    // --- Data ---
    QList<BScheduleEntry> m_entries;
    BJobDurationStats *m_durationStats = nullptr;

    // --- View state ---
    ViewMode m_viewMode = DayView;
    GroupMode m_groupMode = GroupBySchedule;
    int m_currentDay = 0;  // 0=Monday
    int m_pixelsPerHour = DEFAULT_PIXELS_PER_HOUR;

    // --- Layout cache ---
    QVector<RowInfo> m_rows;

    // --- Heatmap ---
    QVector<int> m_heatmapSlots;  // concurrent job count per slot

    // --- Collisions ---
    QList<Collision> m_collisions;

    // --- Hover state ---
    int m_hoverEntryIndex = -1;
    QPoint m_hoverPos;

    // --- Now-marker timer ---
    QTimer *m_nowTimer;
};

#endif // BSCHEDULEGANTTWIDGET_H
