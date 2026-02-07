#ifndef BSCHEDULEWIDGET_H
#define BSCHEDULEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QSplitter>
#include <QPushButton>
#include "director/bdirector.h"
#include "schedules/bweeklyplanner.h"

/**
 * @brief Widget for displaying and managing backup schedules
 * @since 2.7
 *
 * Displays a list of schedules on the left and a weekly planner view on the right.
 * Similar to Windows logon hours configuration.
 *
 * @note BClientJobsModel (in clients/bclientdetailsdialog.h) can be used to
 *       calculate average job durations per client, which is useful for
 *       estimating scheduled job time windows in the planner.
 */
class BScheduleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BScheduleWidget(QWidget *parent = nullptr);
    ~BScheduleWidget();

    /**
     * @brief Sets the director for schedule operations
     * @param director Pointer to BDirector
     */
    void setDirector(BDirector *director) { m_director = director; }

    /**
     * @brief Triggers refresh action
     */
    void triggerRefresh() { onRefreshClicked(); }

    /**
     * @brief Clears all data from the widget
     */
    void clearData();

public slots:
    /**
     * @brief Processes .schedule dot-command response
     * @param jsonData JSON response from .schedule command
     */
    void processDotScheduleResponse(const QString &jsonData);

    /**
     * @brief Updates UI based on connection state
     * @param connected True if connected to Director
     */
    void setConnectionState(bool connected);

signals:
    void sendCommand(const BDirector::Command cmd, const QString &args);
    void statusMessageChanged(const QString &message);

private slots:
    void onRefreshClicked();
    void onScheduleSelectionChanged();
    void onScheduleDoubleClicked(QListWidgetItem *item);

private:
    void setupUI();

    QSplitter *m_splitter;
    QListWidget *m_scheduleList;
    BWeeklyPlanner *m_weeklyPlanner;
    QPushButton *m_refreshButton;
    BDirector *m_director;
};

#endif // BSCHEDULEWIDGET_H
