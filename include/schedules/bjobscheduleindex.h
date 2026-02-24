/**
 * @file bjobscheduleindex.h
 * @brief Cross-reference index between Jobs and Schedules
 *
 * Builds the mapping: ScheduleName -> list of Jobs that reference it.
 * Used to enrich BScheduleEntry objects with job-level data (jobName, client, storage).
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
 * This index builds the reverse mapping so we can enrich schedule
 * entries with job-specific data for Gantt visualization and drag & drop.
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
     * @brief Enriches schedule entries with job data (jobName, client, storage)
     *
     * If a schedule is used by exactly one Job, the entry gets that job's data.
     * If shared by N jobs, creates N entries (one per job) from each original entry.
     * If no job references it, the entry is kept unchanged.
     */
    QList<BScheduleEntry> enrichEntries(const QList<BScheduleEntry> &entries) const;

    bool isValid() const { return m_valid; }

private:
    QMultiMap<QString, JobRef> m_scheduleToJobs;
    bool m_valid = false;
};

#endif // BJOBSCHEDULEINDEX_H
