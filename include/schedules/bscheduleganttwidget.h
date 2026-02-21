#ifndef BSCHEDULEGANTTWIDGET_H
#define BSCHEDULEGANTTWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QComboBox>
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
        GroupBySchedule,
        GroupByStorage
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

    /**
     * @brief Dependency edge between two schedule entries
     * Used during drag to show affected jobs
     */
    struct DependencyEdge {
        enum Type { LevelChain, ClientExclusion, StorageContention };
        Type type;
        int sourceEntryIndex;
        int targetEntryIndex;
        QString description;
    };

    /**
     * @brief Returns the current entries list
     */
    const QList<BScheduleEntry>& entries() const { return m_entries; }

    /**
     * @brief Sets the snap granularity for drag operations
     */
    void setSnapMinutes(int minutes) { m_dragSnapMinutes = minutes; }
    int snapMinutes() const { return m_dragSnapMinutes; }

    // Minimum pixels per hour (prevents labels from overlapping)
    static constexpr int MIN_PIXELS_PER_HOUR = 4;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void entryClicked(const BScheduleEntry &entry);
    void entryDoubleClicked(const BScheduleEntry &entry);
    void collisionsDetected(int count);
    void dragStarted(const BScheduleEntry &entry);
    void dragCompleted(int entryIndex, int newHour, int newMinute,
                       const QList<BScheduleGanttWidget::DependencyEdge> &dependencies);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
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
    int dayOffsetAtPos(const QPoint &pos, int entryIdx) const;
    int rowForEntry(int entryIndex) const;

    // --- Drag & Drop ---
    void drawDragOverlay(QPainter &painter);
    void drawDependencyHighlights(QPainter &painter);
    void computeDependencies(int draggedIndex);
    void updateDragDependencies();
    void cancelDrag();

    // --- Data ---
    QList<BScheduleEntry> m_entries;
    BJobDurationStats *m_durationStats = nullptr;

    // --- View state ---
    ViewMode m_viewMode = DayView;
    GroupMode m_groupMode = GroupBySchedule;
    int m_currentDay = 0;  // 0=Monday
    int m_pixelsPerHour = 60;  // recalculated dynamically in resizeEvent

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

    // --- Drag state ---
    enum DragState { NoDrag, DragPending, Dragging };
    DragState m_dragState = NoDrag;
    int m_dragEntryIndex = -1;
    int m_dragDayOffset = 0;
    QPoint m_dragStartPos;
    int m_dragOriginalHour = 0;
    int m_dragOriginalMinute = 0;
    int m_dragNewHour = 0;
    int m_dragNewMinute = 0;
    int m_dragSnapMinutes = 15;
    QList<DependencyEdge> m_dragDependencies;
};

#endif // BSCHEDULEGANTTWIDGET_H
