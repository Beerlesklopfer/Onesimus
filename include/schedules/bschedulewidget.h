#ifndef BSCHEDULEWIDGET_H
#define BSCHEDULEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QSplitter>
#include <QStackedWidget>
#include <QPushButton>
#include <QToolButton>
#include <QComboBox>
#include <QLabel>
#include <QScrollArea>
#include <QGroupBox>
#include <QTableWidget>
#include <QSlider>
#include "director/bdirector.h"
#include "schedules/bweeklyplanner.h"
#include "schedules/bscheduleganttwidget.h"
#include "schedules/bjobscheduleindex.h"
#include "models/bresourcemodels.h"

/**
 * @brief Widget for displaying and managing backup schedules
 *
 * Provides two views:
 * - Gantt/Timeline view: dual stacked timelines (FD/Client + SD/Storage)
 * - Grid view: compact 7x24 weekly planner (legacy)
 *
 * Both show concrete dates: the day chosen in the toolbar or the week
 * (Monday to Sunday) it is in. What runs on them is worked out from the Run
 * lines of the schedules (BScheduleOccurrences).
 *
 * Left panel: schedule list. Right panel: switchable visualization.
 */
class BScheduleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BScheduleWidget(QWidget *parent = nullptr);
    ~BScheduleWidget();

    void setDirector(BDirector *director) { m_director = director; }
    void triggerRefresh() { onRefreshClicked(); }
    void clearData();

    BScheduleModel *scheduleModel() { return m_scheduleModel; }
    BJobDurationStats *durationStats() { return m_durationStats; }
    BJobScheduleIndex *jobScheduleIndex() { return m_jobScheduleIndex; }

    void setJobConfigModel(BJobConfigModel *model);

public slots:
    void processDotScheduleResponse(const QString &jsonData);
    void processShowSchedulesResponse(const QString &jsonData);
    void processListJobsResponse(const QString &jsonData);
    void processShowJobsResponse(const QString &jsonData);
    void setConnectionState(bool connected);

signals:
    void sendCommand(const BDirector::Command cmd, const QString &args);
    void statusMessageChanged(const QString &message);
    void editJobRequested(const QString &jobName);

private slots:
    void onRefreshClicked();
    void onScheduleSelectionChanged();
    void onScheduleDoubleClicked(QListWidgetItem *item);
    void onViewModeChanged(int index);
    void onDayViewModeChanged(int index);
    void onDayNavigationPrev();
    void onDayNavigationNext();
    void onDayNavigationToday();
    void onCollisionsDetected(int count);
    void onEntryClicked(const BScheduleEntry &entry);
    void onFdDragCompleted(int entryIndex, int newHour, int newMinute,
                           const QList<BScheduleGanttWidget::DependencyEdge> &dependencies);
    void onSdDragCompleted(int entryIndex, int newHour, int newMinute,
                           const QList<BScheduleGanttWidget::DependencyEdge> &dependencies);
    void onSnapChanged(int index);
    void onScheduleCheckChanged(QListWidgetItem *item);
    void onZoomSliderChanged(int value);
    void onJobSelectionChanged();
    void onJobListDoubleClicked(int row, int column);

private:
    void setupUI();
    void updateFilteredViews();
    void setupStatsPanel();
    void updateStatsPanel(const BScheduleEntry &entry);
    void updateGanttEntries();
    void setCurrentDate(const QDate &date);
    void updateDateLabel();
    bool showsWeek() const;     ///< The grid, or the timeline in week view
    /// The schedules checked in the list
    QList<BSchedule> checkedSchedules() const;
    void updateRunProblems();
    void updateJobList();  // Aggregates jobs from all checked schedules
    void handleDragCompleted(BScheduleGanttWidget *source, int entryIndex,
                             int newHour, int newMinute,
                             const QList<BScheduleGanttWidget::DependencyEdge> &dependencies);
    void resizeBothGanttWidgets();
    void updateSnapComboForViewMode(BScheduleGanttWidget::ViewMode mode);

    // --- Models ---
    BScheduleModel *m_scheduleModel;
    BJobDurationStats *m_durationStats;
    BJobScheduleIndex *m_jobScheduleIndex;
    BJobConfigModel *m_jobConfigModel = nullptr;  // not owned

    // --- UI ---
    QSplitter *m_splitter;
    QSplitter *m_leftSplitter;            // Vertical: schedules top, jobs bottom
    QListWidget *m_scheduleList;
    QLabel *m_jobListLabel;
    QTableWidget *m_jobList;

    // Right panel — dual Gantt timelines
    QStackedWidget *m_viewStack;
    QSplitter *m_ganttSplitter;           // Vertical: FD on top, SD on bottom
    BScheduleGanttWidget *m_fdGanttWidget; // FD/Client timeline
    QScrollArea *m_fdScrollArea;
    BScheduleGanttWidget *m_sdGanttWidget; // SD/Storage timeline
    QScrollArea *m_sdScrollArea;
    BWeeklyPlanner *m_weeklyPlanner;
    QScrollArea *m_gridScrollArea;
    QLabel *m_outsideWeekLabel;   ///< Run lines without a run in the week of the grid

    // Toolbar
    QPushButton *m_refreshButton;
    QComboBox *m_viewModeCombo;       // Gantt / Grid
    QComboBox *m_dayViewCombo;        // Day / Week
    QToolButton *m_prevDayButton;
    QToolButton *m_nextDayButton;
    QToolButton *m_todayButton;
    QLabel *m_dayLabel;               // The date or the week on display
    QComboBox *m_snapCombo;            // 15 min / 30 min / 1 hour
    QLabel *m_zoomLabel;
    QSlider *m_zoomSlider;             // Zoom in week mode (px/hour)
    QLabel *m_collisionLabel;
    QLabel *m_runProblemLabel;     ///< Run lines that were not understood

    // Stats panel
    QGroupBox *m_statsPanel;
    QLabel *m_statsTitle;
    // Job config section
    QLabel *m_statsJobType;
    QLabel *m_statsClient;
    QLabel *m_statsFileSet;
    QLabel *m_statsStorage;
    QLabel *m_statsPriority;
    // Duration stats
    QLabel *m_statsMin;
    QLabel *m_statsAvg;
    QLabel *m_statsMax;
    QLabel *m_statsSamples;
    QLabel *m_statsTrend;
    QTableWidget *m_statsRunsTable;

    BDirector *m_director;
    QDate m_currentDate;              // The day on display; in week views its week
};

#endif // BSCHEDULEWIDGET_H
