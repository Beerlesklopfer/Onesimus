#ifndef BSCHEDULEWIDGET_H
#define BSCHEDULEWIDGET_H

#include <QWidget>
#include <QDateEdit>
#include <QTabWidget>
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
#include <functional>
#include "director/bdirector.h"
#include "schedules/bschedulegridview.h"
#include "schedules/bscheduleganttwidget.h"
#include "schedules/bjobscheduleindex.h"
#include "models/bresourcemodels.h"
#include "schedules/model/bjobhistorystats.h"
#include "schedules/model/bschedulewaitmodel.h"
#include "schedules/model/bwaitcalibration.h"
#include "schedules/model/bwaitstagger.h"
#include "schedules/bschedulepersistence.h"

class BScheduleSaveDialogs;

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
 * the timeline (day or week, date navigation, snap, zoom, the counter of
 * predicted waits) are hidden while the grid is shown; its calendar of five
 * weeks has its own navigation.
 *
 * Predicted waits. A run waits only when a limit of the Director is
 * exceeded. For the dates on display the queue of the Director is replayed
 * (BScheduleWaitModel) with the runs of all enabled schedules and jobs, not
 * only of the checked ones, and with the runs of the day before, which may
 * still be running. The timeline draws the wait of each run, the counter in
 * the toolbar sums up the runs on display and opens the list of them below
 * the timeline (an arrow in its text and its filling say whether the list
 * is open). Schedules that no enabled job uses start nothing: they are not
 * replayed, not counted and not listed, and the timeline shows them as a
 * last group "Not used by any job". The limits come from the Director: Maximum Concurrent Jobs
 * of the jobs from "show jobs" and "show jobdefs", of the clients from
 * "show clients", of the storages from "show storages" and of the Director
 * from "show directors" (the last two are asked for with every refresh of
 * the schedules; nothing is written). A limit the Director did not report
 * is not replaced by a default: the runs it matters for are "uncertain".
 *
 * Measured against predicted. The job history is the ground truth
 * (BWaitCalibration): the list shows next to each predicted wait the
 * median and maximum wait the history measured for the job at that level,
 * lists runs the history shows waiting even where nothing is predicted, and
 * flags a prediction far from the measurement. The history also gives the
 * start overhead of every job (connections, Client Run Before Job) and,
 * once the parallel writers of a storage are known, the durations without
 * the wait for the device. The parallel writers of a storage's devices are
 * not reported by the Director: the panel has a field per storage (unknown
 * by default, kept in BSettings per Director profile and storage), with
 * what the history suggests and why; nothing is set without the user. A
 * last line replays the last nights of the history with the model
 * (BWaitCalibration::backtest()) and says how far it was off.
 *
 * Writing (step 7 of the schedule plan). A schedule is changed, deleted and
 * created through the configuration methods of the Onesimus Director over
 * gRPC (BConfigClient, set with setConfigClient()), never with a console
 * command that changes an existing schedule. Every write goes through
 * BSchedulePersistence: the Director checks it first (validate_only), the
 * review shows old and new Run lines and every job that uses the schedule,
 * and only "Write to the Director" sends it.
 *
 *  - The grid: "Save..." writes the changes of the schedule on display
 *    (UpdateResource with the SHA-256 read when editing began). After the
 *    Director wrote it, the schedule is read again and becomes what the
 *    grid starts from; the undo history ends there.
 *  - Dragging a bar of the timeline changes the dragged Run line of the
 *    existing schedule (an update of that schedule, never a new one), on
 *    the same way. Where the connection cannot do that (BNET, plain Bareos,
 *    a session whose capabilities do not list it) nothing is sent: the gate
 *    of setScheduleChangeGate() shows the capability notice and the status
 *    line says that nothing was changed.
 *  - "Delete..." deletes the schedule on display (DeleteResource); a
 *    schedule the Director still finds referenced is not deleted, and the
 *    references are shown (gate: setScheduleDeleteGate()).
 *  - A schedule with a Run line that is not understood is locked: the grid
 *    cannot edit it and a drag of one of its bars is refused.
 *
 * The grid keeps changes that are not written. A refresh or a choice in the
 * list never replaces them: the schedule is offered to the view
 * (BScheduleGridView::offerSchedule()), which asks whether to keep or to
 * discard the changes.
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

    /**
     * @brief Asked when the user tries to change an existing schedule
     *
     * Returns whether the connected Director can do that
     * (BCapabilities::Function::ModifySchedule) and tells the user if it
     * cannot (BCapabilityNotice::check()). The main window sets it. Without
     * a gate nothing can be changed.
     */
    using ScheduleChangeGate = std::function<bool(QWidget *parent)>;
    void setScheduleChangeGate(ScheduleChangeGate gate) { m_scheduleChangeGate = std::move(gate); }

    /// As setScheduleChangeGate(), for "Save..." of the grid (an explicit
    /// request: the notice is shown every time)
    void setScheduleSaveGate(ScheduleChangeGate gate) { m_scheduleSaveGate = std::move(gate); }

    /// As setScheduleChangeGate(), for deleting a schedule
    /// (BCapabilities::Function::DeleteSchedule)
    void setScheduleDeleteGate(ScheduleChangeGate gate) { m_scheduleDeleteGate = std::move(gate); }

    /**
     * @brief The configuration methods of the Director
     *
     * Without a client nothing can be written (the gates then say why).
     * The widget does not own it.
     */
    void setConfigClient(BConfigClient *client);

    /// The flow that writes schedules (also used by the wizard)
    BSchedulePersistence *persistence() const { return m_persistence; }

    /// The dialogs of that flow; tests put their own answers in
    void setSaveUi(BScheduleSaveUi *ui);

    /// The grid on display (tests)
    BScheduleGridView *gridView() const { return m_gridView; }

    /// Names for the pick lists of pools and storages (.pools, .storages)
    void setResourceNames(const QStringList &pools, const QStringList &storages);
    void clearData();

    BScheduleModel *scheduleModel() { return m_scheduleModel; }
    const BJobHistoryStats &historyStats() const { return m_historyStats; }
    BJobScheduleIndex *jobScheduleIndex() { return m_jobScheduleIndex; }

    void setJobConfigModel(BJobConfigModel *model);

    /**
     * @brief The Director profile of the connection
     *
     * Settings that belong to one Director, such as the parallel writers of
     * a storage's devices for the wait prediction, are kept under it. Empty:
     * "default".
     */
    void setDirectorProfileId(const QString &profileId);

    /// The calibration of the wait prediction from the job history (tests)
    const BWaitCalibration &waitCalibration() const { return m_calibration; }
    /// The back-test line of the wait panel (tests)
    QString backtestText() const;

    /// Nights the back-test of the wait prediction replays
    static constexpr int BacktestNights = 7;

public slots:
    void processDotScheduleResponse(const QString &jsonData);
    void processShowSchedulesResponse(const QString &jsonData);
    void processShowJobsResponse(const QString &jsonData);
    /// Replies the limits of the wait prediction are read from
    void processShowStoragesResponse(const QString &jsonData);
    void processShowDirectorResponse(const QString &jsonData);
    void processShowClientsResponse(const QString &jsonData);
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
    /// history request and the limit requests of this widget are looked at
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
    void onDayNavigationToday();
    void onWaitRowChanged();
    void onDeviceWritersChanged();
    void onHistoryRefreshClicked();
    void onEntryClicked(const BScheduleEntry &entry);
    /// A bar of one of the timelines was dropped at another time
    void onDragDropped(int entryIndex, int newHour, int newMinute);
    void onSnapChanged(int index);
    void onScheduleCheckChanged(QListWidgetItem *item);
    void onZoomSliderChanged(int value);
    void onJobSelectionChanged();
    void onJobListDoubleClicked(int row, int column);
    void onGridSaveRequested();
    void onGridDeleteRequested();
    void onPersistenceFinished(const BScheduleReport &report);
    void updateWritable();

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
    void setupWaitPanel();
    void setupStaggerPanel();
    /// Later start times for the jobs that wait (BWaitStagger), for the
    /// dates on display
    void proposeStagger();
    /// The proposal no longer fits the prediction: it is cleared
    void clearStagger(const QString &why);
    /// Per job: how far its Run lines can move without crossing midnight;
    /// 0 for a job whose lines run at several hours or are not understood
    QHash<QString, int> staggerBounds(QHash<QString, QString> *schedules) const;
    /**
     * @brief Replays the Director's queue for the dates on display and
     *        hands the result to the timelines, the counter and the list
     *
     * Called whenever the entries on display, the job history, the jobs or
     * a limit have changed.
     */
    void updateWaitPrediction();
    void updateWaitViews();
    void updateStatsPanel(const BScheduleEntry &entry);
    void updateGanttEntries();
    void setCurrentDate(const QDate &date);
    void updateDateLabel();
    /// Day view: the calendar; week view: previous, the week, next
    void updateDateControls();
    /// Gives an open panel below the views (wait list, history) at least
    /// the height it needs; the views keep the rest
    void fitLowerPanels();
    /// Shows or hides a tab below the views (predicted waits, measured
    /// history); the tabs go when none is open. @p bringToFront: make it
    /// the current tab.
    void showLowerPage(QWidget *page, bool on, bool bringToFront = true);
    /// The counter checked while the waits' tab is on display, unchecked
    /// otherwise; without opening or closing anything
    void syncWaitButton();
    /// A section of the waits' right column with its heading
    void setSectionVisible(QWidget *section, const QString &name, bool on);
    bool showsWeek() const;     ///< The timeline is in week view
    /// The schedules checked in the list
    QList<BSchedule> checkedSchedules() const;
    void updateRunProblems();
    void updateJobList();  // Aggregates jobs from all checked schedules
    /**
     * @brief Writes a new time of a Run line of an existing schedule
     * @return false: the connection cannot write (the caller says so in the
     *         status line); true: the update of that schedule was started
     *         or refused with its own message
     *
     * Updates the schedule the entry belongs to (UpdateResource) on the way
     * of BSchedulePersistence: the Run lines of BScheduleDragPlan::plan(),
     * only the dragged one written anew. Never "configure add": that would
     * create another schedule and leave this one as it is.
     */
    bool writeScheduleChange(const BScheduleEntry &entry, int newHour, int newMinute);

    /// The jobs that use a schedule, for the review
    QStringList jobsUsing(const QString &scheduleName) const;
    /// The text of the counter with the arrow that says whether its list is open
    void updateWaitButtonText();
    /// The parallel writers of the storages of the runs, as the user set them
    QHash<QString, int> deviceWriters() const;
    /// Client, storage and priority of every configured job
    QHash<QString, BWaitJobInfo> waitJobInfo() const;
    /// The rows of storage devices in the wait panel: one per storage
    void updateDevicePanel(const QStringList &storages);
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

    // --- Predicted waits ---
    BWaitLimits m_limits;               ///< What the Director reported; reset with the connection
    bool m_directorLimitAsked = false;  ///< "show directors" is in flight
    QList<BWaitRun> m_waitRuns;         ///< What was replayed, with the day before
    QList<BWaitResult> m_waitResults;   ///< One per run
    QList<int> m_waitShown;             ///< Runs of the entries and dates on display
    QList<BJobRun> m_historyRuns;       ///< The runs of the history reply, for the calibration
    BWaitCalibration m_calibration;     ///< From m_historyRuns, the jobs and the device writers
    BWaitCalibration::Backtest m_backtest;
    QString m_profileId;                ///< Director profile of the connection
    QString m_waitSummary;              ///< The counter's text without its arrow

    // --- UI ---
    QSplitter *m_splitter;
    QSplitter *m_leftSplitter;            // Vertical: schedules top, jobs bottom
    QSplitter *m_rightSplitter = nullptr; // Vertical: views top, the tabs below
    QTabWidget *m_lowerTabs = nullptr;    // Predicted waits, measured history
    QWidget *m_statsPage = nullptr;       // The history's tab: m_statsPanel, scrolling
    bool m_syncingWaitButton = false;     // syncWaitButton() sets the counter
    QListWidget *m_scheduleList;
    QLabel *m_jobListLabel;
    QTableWidget *m_jobList;

    // Right panel — dual Gantt timelines
    QStackedWidget *m_viewStack;
    QTabWidget *m_ganttTabs;              // Tabs: clients (FD), storages (SD)
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
    QToolButton *m_todayButton;
    QDateEdit *m_dayPicker = nullptr;   ///< The date, with a calendar (week view: a day of it)
    QLabel *m_dayName = nullptr;        ///< Day view: its weekday, in the language of the texts
    QLabel *m_snapLabel = nullptr;
    QComboBox *m_snapCombo;            // 15 min / 30 min / 1 hour
    QLabel *m_zoomLabel;
    QSlider *m_zoomSlider;             // Zoom in week mode (px/hour)
    QPushButton *m_waitButton;     ///< Counter of predicted waits; opens their list
    QLabel *m_runProblemLabel;     ///< Run lines that were not understood

    // Predicted waits of the dates on display (below the timeline)
    QGroupBox *m_waitPanel = nullptr;
    QLabel *m_waitInfoLabel = nullptr;    ///< The limits the prediction used, and what is not known
    QLabel *m_waitSharedLabel = nullptr;  ///< Reasons several rows share, stated once: "[1] ..."
    QTableWidget *m_waitTable = nullptr;
    QLabel *m_waitDetails = nullptr;      ///< The reason of the current row in full
    QWidget *m_devicePanel = nullptr;     ///< Parallel writers per storage, with suggestions
    QStringList m_deviceStorages;         ///< The storages that have a row there
    QLabel *m_backtestLabel = nullptr;    ///< The prediction replayed over past nights

    // Later start times for the jobs that wait (a tab below the timeline)
    QWidget *m_staggerPage = nullptr;
    QLabel *m_staggerSummary = nullptr;
    QTableWidget *m_staggerTable = nullptr;
    QPushButton *m_staggerButton = nullptr;
    BWaitStagger::Proposal m_staggerProposal;

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
    ScheduleChangeGate m_scheduleChangeGate;
    ScheduleChangeGate m_scheduleDeleteGate;
    ScheduleChangeGate m_scheduleSaveGate;
    QPointer<BConfigClient> m_configClient;
    BSchedulePersistence *m_persistence = nullptr;
    BScheduleSaveDialogs *m_saveDialogs = nullptr;
    QString m_gridAwaitingReload;     ///< Written from the grid: the next reply is the new baseline
    QString m_gridSaving;             ///< The grid's schedule whose save is under way
    QDate m_currentDate;              // The day on display; in week views its week
};

#endif // BSCHEDULEWIDGET_H
