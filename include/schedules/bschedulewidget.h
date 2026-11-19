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
#include <QPointer>
#include <QThread>
#include <QTimer>
#include "director/bdirector.h"
#include "schedules/bschedulegridview.h"
#include "schedules/bscheduleganttwidget.h"
#include "schedules/bjobscheduleindex.h"
#include "models/bresourcemodels.h"
#include "schedules/model/bjobhistorystats.h"

/**
 * @brief Widget for displaying and managing backup schedules
 *
 * Provides three views:
 * - Gantt/Timeline view: dual stacked timelines (FD/Client + SD/Storage)
 * - Grid view: one schedule resource as weekdays by hours (BScheduleGridView)
 * - Measured waits: a table of what was measured per job and level
 *
 * The timeline shows concrete dates: the day chosen in the toolbar or the
 * week (Monday to Sunday) it is in. What runs on them is worked out from
 * the Run lines of the checked schedules (BScheduleOccurrences).
 *
 * The grid is not about a date and not about the checked schedules: it
 * shows the schedule of the current row of the list, one at a time,
 * because that is the resource a later step writes. Without a current row
 * it takes the first schedule. The controls of the toolbar that belong to
 * the timeline (day or week, date navigation, snap, zoom) are hidden while
 * the grid is shown; its calendar of five weeks has its own navigation.
 *
 * Job history. How long jobs run and wait is measured from the catalog
 * (BJobHistoryStats). The request (BDirector::Command::LlistJobs, bounded
 * by a window in days and a row limit) is not part of connecting or of
 * triggerRefresh(). It is sent once per connection, shortly after this
 * widget is on screen with a connection, again when the window setting
 * has changed, and when the user asks for it. Until a reply is there, and
 * if the Director refuses, the views work without history: runs have no
 * duration and waits are unknown.
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
    const BJobHistoryStats &historyStats() const { return m_historyStats; }
    BJobScheduleIndex *jobScheduleIndex() { return m_jobScheduleIndex; }

    void setJobConfigModel(BJobConfigModel *model);

public slots:
    void processDotScheduleResponse(const QString &jsonData);
    void processShowSchedulesResponse(const QString &jsonData);
    void processShowJobsResponse(const QString &jsonData);
    void setConnectionState(bool connected);

    /**
     * @brief Reply to the job history request (BDirector::jobHistoryResult)
     *
     * Accepted only while this widget waits for a reply to a request with
     * exactly these arguments; anything else is dropped. The reply is read
     * off the GUI thread.
     */
    void processJobHistoryReply(const QString &args, const QString &reply);

    /// A command got no reply (BDirector::commandFailed); only the job
    /// history request of this widget is looked at
    void onCommandFailed(BDirector::Command cmd, const QString &args, const QString &errorMsg);

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
    void onCollisionsDetected(int certain, int uncertain);
    void onHistoryRefreshClicked();
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

protected:
    void showEvent(QShowEvent *event) override;

private:
    void setupUI();
    void setupWaitsPage();
    /**
     * @brief Shows the schedule of the current row of the list in the grid
     * @param namesOnly The model was filled from the list of names, which
     *                  may come without Run lines: a schedule on display is
     *                  not replaced by it
     */
    void updateGridView(bool namesOnly = false);

    // --- Job history ---
    enum HistoryState {
        HistoryNotRequested,    ///< Nothing asked on this connection yet
        HistoryLoading,         ///< Request sent, no reply yet
        HistoryParsing,         ///< Reply is being read
        HistoryLoaded,          ///< Statistics are from the Director
        HistoryUnavailable,     ///< Refused, unreadable or not sent
    };
    /// Starts the timer for a request if one is due and the widget is on
    /// screen with a connection; cheap, may be called often
    void scheduleHistoryRequest();
    void requestHistoryIfDue();
    void requestHistory();
    void applyHistoryReply(const BJobHistoryReply &reply, quint64 generation);
    void setHistoryUnavailable(const QString &reason);
    void resetHistory();
    void updateHistoryViews();
    void updateHistoryInfo();
    void updateWaitsTable();
    /// Why the views have no history at all; empty once it is loaded
    QString historyNote() const;
    /// The level a run of this entry has: the Run line's, else the job's own
    QString historyLevel(const BScheduleEntry &entry) const;
    void setupStatsPanel();
    void updateStatsPanel(const BScheduleEntry &entry);
    void updateGanttEntries();
    void setCurrentDate(const QDate &date);
    void updateDateLabel();
    bool showsWeek() const;     ///< The timeline is in week view
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
    BJobScheduleIndex *m_jobScheduleIndex;

    // --- Job history (cached for the connection) ---
    BJobHistoryStats m_historyStats;
    HistoryState m_historyState = HistoryNotRequested;
    QString m_historyArgs;              ///< Arguments of the request in flight or answered
    int m_historyDays = 0;              ///< Window of that request
    int m_historyLimit = 0;             ///< Row limit of that request
    int m_historyRows = 0;              ///< Rows in the reply
    int m_historyRowsWithSchedTime = 0; ///< Rows with a scheduled time
    bool m_historyOldestKept = false;   ///< Limit reached and the Director kept the oldest rows
    QDateTime m_historyFetchedAt;
    QString m_historyError;             ///< HistoryUnavailable: why
    bool m_historyNoticeShown = false;  ///< The status line said so on this connection
    quint64 m_historyGeneration = 0;    ///< Bumped on reset: older replies are dropped
    QPointer<QThread> m_historyParser;
    QTimer *m_historyTimer = nullptr;
    bool m_connected = false;
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
    BScheduleGridView *m_gridView;        ///< The schedule of the current row of the list
    QString m_gridScheduleName;           ///< Its name: the list is rebuilt on every refresh

    // Measured waits (view stack index 2)
    QWidget *m_waitsPage = nullptr;
    QLabel *m_waitsInfoLabel = nullptr;   ///< Window, time of the request, what was left out
    QPushButton *m_waitsRefreshButton = nullptr;
    QTableWidget *m_waitsTable = nullptr;

    // Toolbar
    QPushButton *m_refreshButton;
    QComboBox *m_viewModeCombo;       // Gantt / Grid / Measured waits
    QComboBox *m_dayViewCombo;        // Day / Week
    QToolButton *m_prevDayButton;
    QToolButton *m_nextDayButton;
    QToolButton *m_todayButton;
    QLabel *m_dayLabel;               // The date or the week on display
    QLabel *m_snapLabel = nullptr;
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
    // Measured history of the job at the entry's level
    QLabel *m_statsBasis = nullptr;   ///< Says so if the figures are an estimate or missing
    QLabel *m_statsMedian;
    QLabel *m_statsSpread;        ///< 25th to 75th percentile
    QLabel *m_statsP90;
    QLabel *m_statsSamples;
    QLabel *m_statsWaitMedian;
    QLabel *m_statsWaitP90;
    QTableWidget *m_statsRunsTable;

    BDirector *m_director;
    QDate m_currentDate;              // The day on display; in week views its week
};

#endif // BSCHEDULEWIDGET_H
