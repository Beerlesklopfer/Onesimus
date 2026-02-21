#ifndef BSCHEDULEWIDGET_H
#define BSCHEDULEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QSplitter>
#include <QStackedWidget>
#include <QPushButton>
#include <QToolButton>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QScrollArea>
#include <QGroupBox>
#include <QTableWidget>
#include "director/bdirector.h"
#include "schedules/bweeklyplanner.h"
#include "schedules/bscheduleganttwidget.h"
#include "models/bresourcemodels.h"

/**
 * @brief Widget for displaying and managing backup schedules
 *
 * Provides two views:
 * - Gantt/Timeline view: horizontal bars with duration, heatmap, collisions
 * - Grid view: compact 7x24 weekly planner (legacy)
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

public slots:
    void processDotScheduleResponse(const QString &jsonData);
    void processListJobsResponse(const QString &jsonData);
    void setConnectionState(bool connected);

signals:
    void sendCommand(const BDirector::Command cmd, const QString &args);
    void statusMessageChanged(const QString &message);

private slots:
    void onRefreshClicked();
    void onScheduleSelectionChanged();
    void onScheduleDoubleClicked(QListWidgetItem *item);
    void onViewModeChanged(int index);
    void onDayViewModeChanged(int index);
    void onDayNavigationPrev();
    void onDayNavigationNext();
    void onZoomChanged(int value);
    void onGroupModeChanged(int index);
    void onCollisionsDetected(int count);
    void onEntryClicked(const BScheduleEntry &entry);

private:
    void setupUI();
    void setupStatsPanel();
    void updateStatsPanel(const BScheduleEntry &entry);
    void updateGanttEntries();

    // --- Models ---
    BScheduleModel *m_scheduleModel;
    BJobDurationStats *m_durationStats;

    // --- UI ---
    QSplitter *m_splitter;
    QListWidget *m_scheduleList;

    // Right panel
    QStackedWidget *m_viewStack;
    BScheduleGanttWidget *m_ganttWidget;
    QScrollArea *m_ganttScrollArea;
    BWeeklyPlanner *m_weeklyPlanner;

    // Toolbar
    QPushButton *m_refreshButton;
    QComboBox *m_viewModeCombo;       // Gantt / Grid
    QComboBox *m_dayViewCombo;        // Day / Week
    QToolButton *m_prevDayButton;
    QToolButton *m_nextDayButton;
    QLabel *m_dayLabel;
    QSlider *m_zoomSlider;
    QComboBox *m_groupModeCombo;      // By Schedule / By Client
    QLabel *m_collisionLabel;

    // Stats panel
    QGroupBox *m_statsPanel;
    QLabel *m_statsTitle;
    QLabel *m_statsMin;
    QLabel *m_statsAvg;
    QLabel *m_statsMax;
    QLabel *m_statsSamples;
    QLabel *m_statsTrend;
    QTableWidget *m_statsRunsTable;

    BDirector *m_director;
    int m_currentDay;                 // 0=Mon..6=Sun
};

#endif // BSCHEDULEWIDGET_H
