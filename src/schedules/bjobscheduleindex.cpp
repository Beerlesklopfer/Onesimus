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

QList<BScheduleEntry> BJobScheduleIndex::enrichEntries(const QList<BScheduleEntry> &entries) const
{
    if (!m_valid) return entries;

    QList<BScheduleEntry> result;
    result.reserve(entries.size());

    for (const BScheduleEntry &entry : entries) {
        QList<JobRef> jobs = m_scheduleToJobs.values(entry.scheduleName);

        if (jobs.isEmpty()) {
            // No job references this schedule — keep as-is (orphan)
            result.append(entry);
        } else if (jobs.size() == 1) {
            // Exactly one job — enrich in place
            BScheduleEntry enriched = entry;
            const JobRef &ref = jobs.first();
            if (enriched.jobName.isEmpty()) enriched.jobName = ref.jobName;
            if (enriched.client.isEmpty()) enriched.client = ref.client;
            if (enriched.storage.isEmpty()) enriched.storage = ref.storage;
            if (enriched.pool.isEmpty()) enriched.pool = ref.pool;
            if (enriched.priority == 10 && ref.priority != 10) enriched.priority = ref.priority;
            result.append(enriched);
        } else {
            // Multiple jobs share this schedule — clone entry for each job
            for (const JobRef &ref : jobs) {
                BScheduleEntry clone = entry;
                clone.jobName = ref.jobName;
                if (clone.client.isEmpty()) clone.client = ref.client;
                if (clone.storage.isEmpty()) clone.storage = ref.storage;
                if (clone.pool.isEmpty()) clone.pool = ref.pool;
                if (clone.priority == 10 && ref.priority != 10) clone.priority = ref.priority;
                result.append(clone);
            }
        }
    }

    return result;
}
