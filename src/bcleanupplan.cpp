// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "bcleanupplan.h"

#include <QJsonObject>
#include <QMap>
#include <QSet>
#include <algorithm>
#include <tuple>

namespace BCleanup {

namespace {

enum class Level { None, Full, Incremental, Differential, Unknown };

struct Row {
    QString jobId;
    QString name;
    QString client;
    QString fileset;
    QString type;
    QString status;
    QString startText;
    QDateTime start;
    Level level = Level::None;
    bool hasFileCount = false;
    qint64 files = 0;
    qint64 bytes = 0;
};

QString text(const QJsonObject &job, const char *key)
{
    const QJsonValue value = job.value(QLatin1String(key));
    if (value.isString())
        return value.toString().trimmed();
    if (value.isDouble())
        return QString::number(static_cast<qint64>(value.toDouble()));
    return QString();
}

bool number(const QJsonObject &job, const char *key, qint64 *out)
{
    const QJsonValue value = job.value(QLatin1String(key));
    if (value.isDouble()) {
        *out = static_cast<qint64>(value.toDouble());
        return true;
    }
    if (value.isString()) {
        bool ok = false;
        const qint64 parsed = value.toString().trimmed().toLongLong(&ok);
        if (ok)
            *out = parsed;
        return ok;
    }
    return false;
}

QDateTime parseTime(const QString &value)
{
    if (value.isEmpty())
        return QDateTime();
    QDateTime time = QDateTime::fromString(value, Qt::ISODate);
    if (!time.isValid())
        time = QDateTime::fromString(value, QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    return time;
}

Level parseLevel(const QString &value)
{
    if (value.isEmpty())
        return Level::None;
    if (value == QLatin1String("F") || value == QLatin1String("Full"))
        return Level::Full;
    if (value == QLatin1String("I") || value == QLatin1String("Incremental"))
        return Level::Incremental;
    if (value == QLatin1String("D") || value == QLatin1String("Differential"))
        return Level::Differential;
    return Level::Unknown;
}

Row parseRow(const QJsonObject &job)
{
    Row row;
    row.jobId = text(job, "jobid");
    row.name = text(job, "name");
    row.client = text(job, "client");
    row.fileset = text(job, "fileset");
    row.type = text(job, "type");
    row.status = text(job, "jobstatus");
    row.startText = text(job, "starttime");
    row.start = parseTime(row.startText);
    row.level = parseLevel(text(job, "level"));
    row.hasFileCount = number(job, "jobfiles", &row.files);
    number(job, "jobbytes", &row.bytes);
    return row;
}

bool isBackup(const Row &row)
{
    return row.type == QLatin1String("B") || row.type == QLatin1String("Backup");
}

/// Terminated normally (T) or with warnings (W): a usable backup
bool isSuccessful(const Row &row)
{
    return row.status == QLatin1String("T") || row.status == QLatin1String("W");
}

/// Fatal error, terminated with errors or canceled: holds no usable backup
bool isUnusable(const Row &row)
{
    return row.status == QLatin1String("f") || row.status == QLatin1String("E")
        || row.status == QLatin1String("A");
}

JobRef toRef(const Row &row)
{
    JobRef ref;
    ref.jobId = row.jobId;
    ref.name = row.name;
    ref.client = row.client;
    ref.fileset = row.fileset;
    ref.startTime = row.startText;
    ref.status = row.status;
    ref.bytes = row.bytes;
    return ref;
}

bool olderThan(const Row &a, const Row &b)
{
    if (a.start != b.start)
        return a.start < b.start;
    return a.jobId.toLongLong() < b.jobId.toLongLong();
}

using JobKey = std::tuple<QString, QString>;             // client, job name
using GroupKey = std::tuple<QString, QString, QString>;  // client, job name, fileset

void selectOldFulls(const QList<Row> &rows, const QSet<QString> &deletedAnyway, Plan &plan)
{
    if (plan.options.keepFulls < 1) {
        plan.oldFullsRefusal = Refusal::KeepCountTooSmall;
        return;
    }

    // Rows that cannot even be assigned to a job: refuse as a whole
    int withoutId = 0;
    int withoutType = 0;
    int withoutOwner = 0;
    for (const Row &row : rows) {
        if (row.jobId.isEmpty())
            ++withoutId;
        else if (row.type.isEmpty())
            ++withoutType;
        else if (isBackup(row) && (row.client.isEmpty() || row.name.isEmpty()))
            ++withoutOwner;
    }
    if (withoutId > 0) {
        plan.oldFullsRefusal = Refusal::RowsWithoutJobId;
        plan.oldFullsRefusalRows = withoutId;
        return;
    }
    if (withoutType > 0) {
        plan.oldFullsRefusal = Refusal::RowsWithoutType;
        plan.oldFullsRefusalRows = withoutType;
        return;
    }
    if (withoutOwner > 0) {
        plan.oldFullsRefusal = Refusal::BackupRowsWithoutClientOrName;
        plan.oldFullsRefusalRows = withoutOwner;
        return;
    }

    // Rows of a job that lack a field: nothing of that job on that client is selected
    QMap<JobKey, SkippedJob> skipped;
    auto skip = [&](const Row &row, SkipReason reason) {
        const JobKey key(row.client, row.name);
        auto it = skipped.find(key);
        if (it == skipped.end()) {
            SkippedJob entry;
            entry.client = row.client;
            entry.name = row.name;
            entry.reason = reason;
            it = skipped.insert(key, entry);
        }
        if (it->reason == reason)
            ++it->rows;
    };

    QMap<GroupKey, QList<Row>> groups;
    for (const Row &row : rows) {
        if (!isBackup(row))
            continue;
        if (row.fileset.isEmpty())
            skip(row, SkipReason::MissingFileset);
        else if (row.status.isEmpty())
            skip(row, SkipReason::MissingStatus);
        else if (row.level == Level::None || row.level == Level::Unknown)
            skip(row, SkipReason::UnknownLevel);
        else if (!row.start.isValid())
            skip(row, SkipReason::InvalidStartTime);
        else
            groups[GroupKey(row.client, row.name, row.fileset)].append(row);
    }
    plan.skippedJobs = skipped.values();

    for (auto it = groups.begin(); it != groups.end(); ++it) {
        if (skipped.contains(JobKey(std::get<0>(it.key()), std::get<1>(it.key()))))
            continue;

        QList<Row> &group = it.value();
        std::sort(group.begin(), group.end(), olderThan);

        // Positions of the successful Fulls, oldest first
        QList<int> fulls;
        for (int i = 0; i < group.size(); ++i) {
            if (group[i].level == Level::Full && isSuccessful(group[i]))
                fulls.append(i);
        }

        // A Full that another option deletes anyway does not count as kept
        QList<int> remaining;
        for (int index : fulls) {
            if (!deletedAnyway.contains(group[index].jobId))
                remaining.append(index);
        }

        const int surplus = remaining.size() - plan.options.keepFulls;
        if (surplus <= 0)
            continue;

        FullGroup result;
        result.client = std::get<0>(it.key());
        result.name = std::get<1>(it.key());
        result.fileset = std::get<2>(it.key());
        result.fullCount = fulls.size();

        for (int n = 0; n < surplus; ++n) {
            const int position = remaining[n];

            // surplus > 0 with keepFulls >= 1: a newer Full always exists
            const int nextFull = fulls[fulls.indexOf(position) + 1];

            JobRef ref = toRef(group[position]);
            QStringList keptDependents;
            for (int i = position + 1; i < nextFull; ++i) {
                const Row &other = group[i];
                if (other.level == Level::Full || isUnusable(other))
                    continue;
                if (deletedAnyway.contains(other.jobId))
                    ref.requiredDeletions.append(other.jobId);
                else
                    keptDependents.append(other.jobId);
            }

            if (keptDependents.isEmpty()) {
                result.selected.append(ref);
                plan.oldFulls.append(ref);
                plan.oldFullsBytes += ref.bytes;
            } else {
                BlockedFull blocked;
                blocked.job = ref;
                blocked.dependentJobIds = keptDependents;
                result.blocked.append(blocked);
            }
        }

        plan.fullGroups.append(result);
    }
}

} // namespace

bool Options::operator==(const Options &other) const
{
    return removeOldFulls == other.removeOldFulls
        && keepFulls == other.keepFulls
        && removeEmptyJobs == other.removeEmptyJobs
        && removeFailedJobs == other.removeFailedJobs
        && failedAgeDays == other.failedAgeDays
        && pruneVolumes == other.pruneVolumes;
}

bool Options::anySelected() const
{
    return removeOldFulls || removeEmptyJobs || removeFailedJobs || pruneVolumes;
}

QStringList Plan::jobIdsToDelete() const
{
    QStringList ids;
    QSet<QString> seen;
    auto add = [&](const QList<JobRef> &jobs) {
        for (const JobRef &job : jobs) {
            if (!seen.contains(job.jobId)) {
                seen.insert(job.jobId);
                ids.append(job.jobId);
            }
        }
    };
    // Dependents (empty or failed Incrementals) go before the Fulls they belong to
    add(failedJobs);
    add(emptyJobs);
    add(oldFulls);
    return ids;
}

bool Plan::hasWork() const
{
    return pruneVolumes || !oldFulls.isEmpty() || !emptyJobs.isEmpty() || !failedJobs.isEmpty();
}

Plan buildPlan(const QJsonArray &jobs, const Options &options, const QDateTime &now)
{
    Plan plan;
    plan.options = options;
    plan.pruneVolumes = options.pruneVolumes;

    QList<Row> rows;
    rows.reserve(jobs.size());
    for (const QJsonValue &value : jobs)
        rows.append(parseRow(value.toObject()));
    plan.jobRows = rows.size();

    QSet<QString> deletedAnyway;

    if (options.removeEmptyJobs) {
        for (const Row &row : rows) {
            if (row.jobId.isEmpty() || row.status != QLatin1String("T"))
                continue;
            if (!row.hasFileCount) {
                // No file count is not the same as zero files
                ++plan.emptyRowsWithoutFileCount;
                continue;
            }
            if (row.files == 0) {
                plan.emptyJobs.append(toRef(row));
                deletedAnyway.insert(row.jobId);
            }
        }
    }

    if (options.removeFailedJobs && options.failedAgeDays >= 1 && now.isValid()) {
        const QDateTime cutoff = now.addDays(-options.failedAgeDays);
        for (const Row &row : rows) {
            if (row.jobId.isEmpty())
                continue;
            const bool failed = isUnusable(row) || row.status == QLatin1String("e");
            if (failed && row.start.isValid() && row.start < cutoff) {
                plan.failedJobs.append(toRef(row));
                deletedAnyway.insert(row.jobId);
            }
        }
    }

    if (options.removeOldFulls)
        selectOldFulls(rows, deletedAnyway, plan);

    return plan;
}

void Analysis::set(const Plan &plan)
{
    m_plan = plan;
    m_valid = true;
}

void Analysis::invalidate()
{
    m_plan = Plan();
    m_valid = false;
}

bool Analysis::canExecute(const Options &current) const
{
    return m_valid && m_plan.options == current && m_plan.hasWork();
}

} // namespace BCleanup
