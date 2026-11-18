/**
 * @file bjobscheduleindex.h
 * @brief Cross-reference index between Jobs and Schedules
 *
 * Builds the mapping: ScheduleName -> list of Jobs that reference it.
 * Used to say which jobs run a schedule's Run lines on concrete dates.
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#ifndef BJOBSCHEDULEINDEX_H
#define BJOBSCHEDULEINDEX_H

#include <QObject>
#include <QMap>
#include <QMultiMap>
#include <QStringList>
#include "models/bresourcemodels.h"

class BJobConfigModel;

/**
 * @brief Cross-references Job configs with Schedule entries
 *
 * Jobs reference schedules via Job { Schedule = "ScheduleName" }.
 * This index builds the reverse mapping, so that the views can show each
 * run once per job that runs it.
 */
class BJobScheduleIndex : public QObject
{
    Q_OBJECT

public:
    explicit BJobScheduleIndex(QObject *parent = nullptr);

    /**
     * @brief A single job that references a schedule
     */
    struct JobRef {
        QString jobName;
        QString client;
        QString storage;
        QString pool;
        QString fileset;
        int priority = 10;
    };

    /**
     * @brief Rebuild the index from job configs
     * @param jobConfigModel The model containing parsed "show jobs" data
     */
    void rebuild(const BJobConfigModel *jobConfigModel);

    /**
     * @brief Returns all jobs that use a given schedule
     */
    QList<JobRef> jobsForSchedule(const QString &scheduleName) const;

    /**
     * @brief Returns how many jobs share a given schedule
     */
    int sharedCount(const QString &scheduleName) const;

    /**
     * @brief What the Gantt view shows for the time from @p from to @p to
     *
     * The starts of each schedule are worked out once for these dates
     * (BScheduleOccurrences). Each Run line and hour that has a start in
     * them gives one entry per job that uses the schedule, with the job's
     * name, client, storage, pool and priority where the Run line does not
     * override them, or one entry without a job if no job uses the schedule
     * or the jobs are not known yet. A line that does not run in these
     * dates gives nothing.
     *
     * Every entry carries its Run line (BScheduleEntry::source) and the
     * line's position in its schedule (BScheduleEntry::runIndex).
     */
    QList<BScheduleEntry> entriesFor(const QList<BSchedule> &schedules,
                                     const QDateTime &from, const QDateTime &to) const;

    bool isValid() const { return m_valid; }

private:
    QMultiMap<QString, JobRef> m_scheduleToJobs;
    bool m_valid = false;
};

#endif // BJOBSCHEDULEINDEX_H
