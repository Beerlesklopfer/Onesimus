/**
 * @file bjobscheduleindex.cpp
 * @brief Implementation of Job-Schedule cross-reference index
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "schedules/bjobscheduleindex.h"
#include "models/bresourcemodels.h"
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

    for (auto it = configs.constBegin(); it != configs.constEnd(); ++it) {
        const QString &jobName = it.key();
        const QJsonObject &jobObj = it.value();

        // Extract schedule name — Bareos JSON uses {"name": "ScheduleName"} for references
        QString scheduleName;
        QJsonValue schedVal = jobObj["schedule"];
        if (schedVal.isObject()) {
            scheduleName = schedVal.toObject()["name"].toString();
        } else if (schedVal.isString()) {
            scheduleName = schedVal.toString();
        }

        if (scheduleName.isEmpty()) continue;

        // Extract client
        QString client;
        QJsonValue clientVal = jobObj["client"];
        if (clientVal.isObject()) {
            client = clientVal.toObject()["name"].toString();
        } else if (clientVal.isString()) {
            client = clientVal.toString();
        }

        // Extract storage
        QString storage;
        QJsonValue storageVal = jobObj["storage"];
        if (storageVal.isObject()) {
            storage = storageVal.toObject()["name"].toString();
        } else if (storageVal.isString()) {
            storage = storageVal.toString();
        }

        // Extract pool
        QString pool;
        QJsonValue poolVal = jobObj["pool"];
        if (poolVal.isObject()) {
            pool = poolVal.toObject()["name"].toString();
        } else if (poolVal.isString()) {
            pool = poolVal.toString();
        }

        // Extract fileset
        QString fileset;
        QJsonValue filesetVal = jobObj["fileset"];
        if (filesetVal.isObject()) {
            fileset = filesetVal.toObject()["name"].toString();
        } else if (filesetVal.isString()) {
            fileset = filesetVal.toString();
        }

        // Extract priority
        int priority = jobObj["priority"].toInt(10);

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
