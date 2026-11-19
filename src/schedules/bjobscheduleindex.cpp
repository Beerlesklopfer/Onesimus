/**
 * @file bjobscheduleindex.cpp
 * @brief Implementation of Job-Schedule cross-reference index
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "schedules/bjobscheduleindex.h"
#include "models/bresourcemodels.h"
#include "schedules/model/bschedulewaitmodel.h"
#include "blogging.h"
#include <QJsonObject>
#include <QJsonValue>

BJobScheduleIndex::BJobScheduleIndex(QObject *parent)
    : QObject(parent)
{
}

void BJobScheduleIndex::rebuild(const BJobConfigModel *jobConfigModel)
{
    m_scheduleToJobs.clear();
    m_valid = false;

    if (!jobConfigModel || !jobConfigModel->hasConfigs()) {
        BLOG_DEBUG() << "BJobScheduleIndex: No job configs available";
        return;
    }

    const QMap<QString, QJsonObject> &configs = jobConfigModel->jobConfigsRaw();
    const QMap<QString, QJsonObject> &jobDefs = jobConfigModel->jobDefsConfigsRaw();

    for (auto it = configs.constBegin(); it != configs.constEnd(); ++it) {
        const QString &jobName = it.key();

        // A directive of the job, or of the JobDefs it names: without
        // "verbose" the Director does not print in a Job what the Job
        // inherits (BWaitLimits). A reference is {"name": ...} or a string.
        auto directive = [&](const char *key) {
            return BWaitLimits::jobDirective(configs, jobDefs, jobName, QLatin1String(key));
        };

        const QString scheduleName = BWaitLimits::resourceName(directive("schedule"));
        if (scheduleName.isEmpty()) continue;

        const QString client = BWaitLimits::resourceName(directive("client"));
        const QString storage = BWaitLimits::resourceName(directive("storage"));
        const QString pool = BWaitLimits::resourceName(directive("pool"));
        const QString fileset = BWaitLimits::resourceName(directive("fileset"));
        const QJsonValue priorityValue = directive("priority");
        int priority = priorityValue.isString() ? priorityValue.toString().toInt()
                                                : priorityValue.toInt(10);
        if (priority < 1) priority = 10;

        JobRef ref;
        ref.jobName = jobName;
        ref.client = client;
        ref.storage = storage;
        ref.pool = pool;
        ref.fileset = fileset;
        ref.priority = priority;

        m_scheduleToJobs.insert(scheduleName, ref);
    }

    m_valid = true;
    BLOG_DEBUG() << "BJobScheduleIndex: Built index for"
                 << m_scheduleToJobs.uniqueKeys().size() << "schedules,"
                 << m_scheduleToJobs.size() << "job references";
}

QList<BJobScheduleIndex::JobRef> BJobScheduleIndex::jobsForSchedule(const QString &scheduleName) const
{
    return m_scheduleToJobs.values(scheduleName);
}

int BJobScheduleIndex::sharedCount(const QString &scheduleName) const
{
    return m_scheduleToJobs.count(scheduleName);
}

QList<BScheduleEntry> BJobScheduleIndex::entriesFor(const QList<BSchedule> &schedules,
                                                    const QDateTime &from,
                                                    const QDateTime &to) const
{
    QList<BScheduleEntry> result;

    for (const BSchedule &schedule : schedules) {
        // What runs when in these dates, once per schedule
        const QList<BScheduleEntry> runs = BScheduleEntry::fromOccurrences(
            schedule, BScheduleOccurrences::occurrences(schedule, from, to));
        if (runs.isEmpty())
            continue;

        const QList<JobRef> jobs = m_valid ? m_scheduleToJobs.values(schedule.name)
                                           : QList<JobRef>();
        if (jobs.isEmpty()) {
            // No job uses this schedule, or the jobs are not known yet
            result.append(runs);
            continue;
        }

        // Each job that uses the schedule runs each of its runs. What the
        // Run line overrides (pool, storage, priority) wins over the job.
        for (const JobRef &ref : jobs) {
            for (const BScheduleEntry &run : runs) {
                BScheduleEntry entry = run;
                entry.jobName = ref.jobName;
                entry.client = ref.client;
                if (entry.storage.isEmpty()) entry.storage = ref.storage;
                if (entry.pool.isEmpty()) entry.pool = ref.pool;
                if (entry.priority == 10 && ref.priority != 10) entry.priority = ref.priority;
                result.append(entry);
            }
        }
    }

    return result;
}
