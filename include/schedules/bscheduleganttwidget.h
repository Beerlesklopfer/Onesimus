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
 * The widget shows concrete dates: one day (currentDate()) or the week from
 * Monday to Sunday that contains it. A bar is drawn for each start an entry
 * has in these dates (BScheduleEntry::occurrences); the days of the week of
 * an entry are not looked at. Whoever sets the entries works them out for
 * firstDate() and dayCount() days (BJobScheduleIndex::entriesFor()).
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
     *
     * Each entry with its starts in the dates on display. An entry without
     * a start in them has a row and no bar.
     */
    void setEntries(const QList<BScheduleEntry> &entries);

    /**
     * @brief Sets the duration statistics for bar width calculation
     */
    void setDurationStats(BJobDurationStats *stats);

    /**
     * @brief Sets the job config model for enriched tooltips (Type, FileSet)
     */
    void setJobConfigModel(BJobConfigModel *model);

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
     * @brief Sets the date to display: that day, or in week view its week
     *
     * The entries have to be set again for the new dates.
     */
    void setCurrentDate(const QDate &date);
    QDate currentDate() const { return m_currentDate; }

    /// The first date on display and the number of days from it
    QDate firstDate() const { return firstDateFor(m_viewMode, m_currentDate); }
    int dayCount() const { return dayCountFor(m_viewMode); }
    static QDate firstDateFor(ViewMode mode, const QDate &date);
    static int dayCountFor(ViewMode mode) { return mode == WeekView ? 7 : 1; }

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
        int index1 = -1;            ///< Index of entry1 in entries()
        int index2 = -1;            ///< Index of entry2 in entries()
        /// Every pair of starts that really runs at the same time, in the
        /// dates on display; never empty
        QList<BRunOverlap> overlaps;
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
    void setSnapMinutes(int minutes) { m_dragSnapMinutes = qBound(1, minutes, 24 * 60); update(); }
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
    static const int HEADER_ROW1_HEIGHT = 18;  // Weekday row
    static const int HEADER_ROW2_HEIGHT = 18;  // Hours row
    static const int HEADER_HEIGHT = HEADER_ROW1_HEIGHT + HEADER_ROW2_HEIGHT;
    static const int LABEL_WIDTH = 180;
    static const int HEATMAP_HEIGHT = 24;
    static const int GROUP_HEADER_HEIGHT = 22;
    static const int HEATMAP_SLOT_MINUTES = 15;

    // --- Drawing ---
    void drawTimeHeader(QPainter &painter);
    void drawNowMarker(QPainter &painter);
    void drawRows(QPainter &painter);
    void drawJobBar(QPainter &painter, int entryIndex, int row, int dayOffset);
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
    void updateMinimumSize();

    // --- Heatmap calculation ---
    void recalcHeatmap();
    int heatmapSlotCount() const;

    // --- Collision detection ---
    void detectCollisions();

    // --- Dates and durations ---
    /// The days on display on which the entry starts, 0 = firstDate()
    QVector<int> dayOffsets(const BScheduleEntry &entry) const;

    enum DurationUse { ForDisplay, ForCollision };
    /**
     * @brief How long a run of this entry is taken to last, in seconds
     *
     * The one place the views take a duration from: the estimate on the
     * entry if there is one, otherwise a fixed figure (for display by
     * level, for collisions one hour). Measured durations replace this.
     */
    int durationSecs(const BScheduleEntry &entry, DurationUse use) const;

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
    BJobConfigModel *m_jobConfigModel = nullptr;

    // --- View state ---
    ViewMode m_viewMode = DayView;
    GroupMode m_groupMode = GroupBySchedule;
    QDate m_currentDate = QDate::currentDate();
    int m_pixelsPerHour = 60;  // recalculated dynamically in resizeEvent
    int m_zoomMinPph = MIN_PIXELS_PER_HOUR;  // minimum px/h from zoom slider

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
    bool dragIndexValid() const { return m_dragEntryIndex >= 0 && m_dragEntryIndex < m_entries.size(); }
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
