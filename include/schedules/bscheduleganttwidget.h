#ifndef BSCHEDULEGANTTWIDGET_H
#define BSCHEDULEGANTTWIDGET_H

#include <QWidget>
#include <QFont>
#include <QScrollArea>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QToolButton>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QList>
#include <QHash>
#include "models/bresourcemodels.h"
#include "schedules/model/bjobhistorystats.h"

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
 * - Bar width is the measured median duration of the job at that level
 *   (BJobHistoryStats), with a mark at the 90th percentile. A run without
 *   history is a dashed outline with a question mark: it has no length.
 * - Predicted waiting (setWaits()): a run that is expected to wait for a
 *   limit of the Director has a hatched lead-in from its scheduled time to
 *   its predicted start, and its bar begins there. Runs that only run at
 *   the same time are not marked: that is allowed by the limits.
 * - Heatmap at bottom showing how many runs are running per time slot, in
 *   one neutral colour, with a strip where runs are waiting
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
     * @brief Sets the measured job history the bars and the heat map take
     *        their durations from
     * @param stats The statistics; empty if there is no history
     * @param note  Why there is no history at all (not loaded yet, refused
     *              by the Director), for the tooltips; empty if it was
     *              loaded
     */
    void setHistoryStats(const BJobHistoryStats &stats, const QString &note = QString());

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

    /** What is predicted for one start of an entry (BScheduleWaitModel). */
    struct RunWait {
        qint64 waitSecs = 0;    ///< Predicted start minus scheduled time
        bool certain = true;    ///< false: depends on something that is not known
        /// The reason in plain words, a statement per entry: the limit and
        /// its value or the priority rule, and the runs it waits for
        QStringList lines;
    };

    /**
     * @brief Sets the predicted waits of the starts on display
     * @param waits By waitKey(); a start that is not in it does not wait
     *              and is certain
     *
     * The widget does not predict anything itself: it draws what it is
     * given. Has to be set again after setEntries().
     */
    void setWaits(const QHash<QString, RunWait> &waits);

    /// Names one entry among those of a view: schedule, Run line, hour, job
    static QString entryKey(const BScheduleEntry &entry);
    /// Names one start of an entry
    static QString waitKey(const BScheduleEntry &entry, const QDateTime &when);
    static QString waitKey(const QString &entryKey, const QDateTime &when);

    /**
     * @brief Dependency edge between two schedule entries
     * Used during drag to show affected jobs
     */
    struct DependencyEdge {
        /// ClientExclusion and StorageContention say that two entries share
        /// a client or a storage; whether one waits for the other depends
        /// on the limits and is not decided here
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

    /**
     * @brief The measures of the layout, in pixels
     *
     * All of them follow the widget's font (the desktop's font times the
     * interface zoom), so that no text is cut when the font grows.
     */
    struct Metrics {
        int rowHeight = 0;          ///< A job row or a group header
        int headerRow1Height = 0;   ///< Day names
        int headerRow2Height = 0;   ///< Hour labels
        int headerHeight = 0;       ///< Both rows
        int labelWidth = 0;         ///< The column of job names
        int heatmapHeight = 0;
        int noHistoryMarkerWidth = 0;
        int hourLabelWidth = 0;     ///< Width of the text "00:00"
    };
    Metrics metrics() const { return m_metrics; }

    /// Minutes between two hour labels of the header: as many labels as
    /// fit without touching
    int hourLabelStepMinutes() const;

signals:
    void entryClicked(const BScheduleEntry &entry);
    void entryDoubleClicked(const BScheduleEntry &entry);
    void dragStarted(const BScheduleEntry &entry);
    void dragCompleted(int entryIndex, int newHour, int newMinute,
                       const QList<BScheduleGanttWidget::DependencyEdge> &dependencies);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    // --- Layout: follows the widget's font (updateMetrics()) ---
    int m_rowHeight = 28;
    int m_headerRow1Height = 18;  // Weekday row
    int m_headerRow2Height = 18;  // Hours row
    int m_headerHeight = 36;
    int m_labelWidth = 230;       // "job · level"; longer labels are elided
    int m_heatmapHeight = 24;
    /// Width in pixels of the marker of a run without history. It is a
    /// marker, not a length: it does not change with the time zoom.
    int m_noHistoryMarkerWidth = 22;
    Metrics m_metrics;
    QFont m_smallFont;            // Hour labels, heat map label
    QFont m_smallBoldFont;        // Day names, letters in the bars
    static const int HEATMAP_SLOT_MINUTES = 15;

    /// Works out all measures from font(); after every change of it
    void updateMetrics();
    /// A measure designed for a 10 pt font, for this widget's font
    int px(int designPixels) const;
    /// Pixels per hour from the width and the time zoom
    void fitToWidth();

    // --- Drawing ---
    void drawTimeHeader(QPainter &painter);
    void drawNowMarker(QPainter &painter);
    void drawRows(QPainter &painter);
    void drawJobBar(QPainter &painter, int entryIndex, int row, int dayOffset);
    void drawHeatmap(QPainter &painter);

    // --- Layout calculation ---
    struct RowInfo {
        QString label;
        bool isGroupHeader;
        int entryIndex;  // -1 for group headers
    };
    void rebuildRows();
    /// "job · level"; the schedule name stands in for a missing job
    static QString rowLabel(const BScheduleEntry &entry);
    int totalHours() const;
    int timeToX(int hour, int minute) const;
    int xToHour(int x) const;
    int xToMinute(int x) const;
    int rowAtY(int y) const;
    /// The bar at the scheduled time, as if the run did not wait
    QRect barRect(const BScheduleEntry &entry, int row, int dayOffset) const;
    /// The bar where it is drawn: at the predicted start
    QRect runRect(const BScheduleEntry &entry, int row, int dayOffset) const;
    /// The lead-in from the scheduled time to the predicted start; empty
    /// if the run does not wait
    QRect waitRect(const BScheduleEntry &entry, int row, int dayOffset) const;
    /// What is predicted for the start of the entry on that day; nullptr
    /// if nothing was set for it
    const RunWait *waitFor(const BScheduleEntry &entry, int dayOffset) const;
    /// The start of the entry on that day; invalid if it has none
    QDateTime occurrenceOn(const BScheduleEntry &entry, int dayOffset) const;
    int contentHeight() const;
    int contentWidth() const;
    void updateMinimumSize();

    // --- Heatmap calculation ---
    void recalcHeatmap();
    int heatmapSlotCount() const;

    // --- Dates and durations ---
    /// The days on display on which the entry starts, 0 = firstDate()
    QVector<int> dayOffsets(const BScheduleEntry &entry) const;

    /**
     * @brief What was measured for the job of this entry at its level
     *
     * The level is the one the Run line names, otherwise the job's own
     * from its configuration. See BJobHistoryStats::estimate() for the
     * rule; an entry without a job has no history.
     */
    BJobHistoryStats::Estimate estimateFor(const BScheduleEntry &entry) const;

    /**
     * @brief How long a run of this entry lasts, in seconds
     *
     * The one place the views take a duration from: the measured median.
     * @return 0 if there is no history. Then the run has no known length
     *         and the caller must not draw or compute one; there is no
     *         default figure. A measured run is at least one second long.
     */
    int durationSecs(const BScheduleEntry &entry) const;

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
    BJobHistoryStats m_history;
    QString m_historyNote;      ///< Why there is no history at all; empty if loaded
    mutable QHash<QString, BJobHistoryStats::Estimate> m_estimateCache;
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
    QVector<int> m_heatmapUnknown; // runs without history that start in the slot
    QVector<int> m_heatmapWaiting; // runs that are predicted to wait in the slot

    // --- Predicted waits ---
    QHash<QString, RunWait> m_waits;    ///< By waitKey()

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
