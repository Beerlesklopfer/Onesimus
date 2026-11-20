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
#include <QSet>

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
        // Only an explicit "no" disables: the default is enabled
        const bool enabled = BWaitLimits::boolValue(directive("enabled")) != std::optional<bool>(false);

        JobRef ref;
        ref.jobName = jobName;
        ref.client = client;
        ref.storage = storage;
        ref.pool = pool;
        ref.fileset = fileset;
        ref.priority = priority;
        ref.enabled = enabled;

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

QList<BJobScheduleIndex::JobRef> BJobScheduleIndex::runningJobsForSchedule(const QString &scheduleName) const
{
    QList<JobRef> jobs;
    if (!m_valid) return jobs;
    for (const JobRef &ref : m_scheduleToJobs.values(scheduleName)) {
        if (ref.enabled) jobs.append(ref);
    }
    return jobs;
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

        const QList<JobRef> jobs = runningJobsForSchedule(schedule.name);
        if (jobs.isEmpty()) {
            // The jobs are not known yet, or no enabled job uses this
            // schedule: then nothing starts at these times
            for (BScheduleEntry run : runs) {
                run.unused = m_valid;
                result.append(run);
            }
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

BJobScheduleIndex::WeekLoad BJobScheduleIndex::startsPerWeekdayHour(const QList<BScheduleEntry> &entries,
                                                                    const QDate &first, int weeks)
{
    WeekLoad load{};
    if (weeks <= 0 || !first.isValid()) return load;
    const QDate end = first.addDays(7 * weeks);
    for (const BScheduleEntry &entry : entries) {
        if (entry.unused || entry.jobName.isEmpty()) continue;
        // A start that exists twice on a day (end of daylight saving) is one run
        QSet<QDate> dates;
        for (const QDateTime &when : entry.occurrences) {
            const QDate date = when.date();
            if (date < first || date >= end || dates.contains(date)) continue;
            dates.insert(date);
            load[size_t(date.dayOfWeek() % 7)][size_t(when.time().hour())] += 1.0;
        }
    }
    for (auto &day : load)
        for (double &hour : day)
            hour /= weeks;
    return load;
}
