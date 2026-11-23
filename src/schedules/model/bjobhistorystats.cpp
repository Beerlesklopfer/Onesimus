// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

/**
 * @file bjobhistorystats.cpp
 * @brief Measured durations and waits of past job runs
 *
 * See the header for the rules and where they are taken from.
 */

#include "schedules/model/bjobhistorystats.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>
#include <QTimeZone>

#include <algorithm>
#include <cmath>

namespace {

const qint64 secondsPerDay = 86400;

/// The first character of a code column ("T", "F"); null for an empty value
QChar codeOf(const QJsonValue &value)
{
    const QString text = value.toString();
    return text.isEmpty() ? QChar() : text.at(0);
}

/// The Director's own words from an error reply:
/// {"error":{"message":"failed","data":{"messages":{"error":["..."]}}}}
QString errorText(const QJsonObject &error)
{
    QStringList lines;
    const QJsonObject messages = error.value(QLatin1String("data")).toObject()
                                     .value(QLatin1String("messages")).toObject();
    for (auto it = messages.constBegin(); it != messages.constEnd(); ++it) {
        const QJsonArray texts = it.value().toArray();
        for (const QJsonValue &text : texts) {
            const QString line = text.toString().trimmed();
            if (!line.isEmpty()) {
                lines.append(line);
            }
        }
    }
    if (lines.isEmpty()) {
        const QString message = error.value(QLatin1String("message")).toString().trimmed();
        if (!message.isEmpty()) {
            lines.append(message);
        }
    }
    return lines.join(QLatin1Char(' '));
}

/// Value at quantile q (0..1) of a sorted, non-empty sample
qint64 quantileOf(const QList<qint64> &sorted, double q)
{
    const double position = q * double(sorted.size() - 1);
    const qsizetype lower = qsizetype(std::floor(position));
    const qsizetype upper = qMin(lower + 1, sorted.size() - 1);
    const double fraction = position - double(lower);
    return qint64(std::llround(double(sorted.at(lower))
                               + fraction * double(sorted.at(upper) - sorted.at(lower))));
}

} // namespace

// ============================================================================
// BJobHistoryReply
// ============================================================================

QDateTime BJobHistoryReply::parseTime(const QString &text)
{
    const QString trimmed = text.trimmed();
    if (trimmed.isEmpty() || trimmed == QLatin1String("0")) {
        return QDateTime();
    }

    // "2026-11-18 02:05:00", from PostgreSQL also with fractions of a second
    const QString head = trimmed.left(19);
    const QDate date = QDate::fromString(head.left(10), QStringLiteral("yyyy-MM-dd"));
    const QTime time = QTime::fromString(head.mid(11), QStringLiteral("HH:mm:ss"));
    if (!date.isValid() || !time.isValid() || head.size() != 19) {
        return QDateTime();
    }
    // Old catalogs hold zero dates for "never"
    if (date.year() < 1971) {
        return QDateTime();
    }
    // Wall-clock time of the Director: no zone is known, none is applied
    return QDateTime(date, time, QTimeZone::UTC);
}

BJobHistoryReply BJobHistoryReply::parse(const QString &reply)
{
    BJobHistoryReply out;

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(reply.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        out.outcome = Outcome::Unreadable;
        // A Director that is not in API mode, or does not know the command,
        // answers in plain text: pass a short piece of it on
        const QString text = reply.simplified();
        out.error = text.isEmpty() ? QStringLiteral("empty reply") : text.left(200);
        return out;
    }

    const QJsonObject root = doc.object();
    if (root.contains(QLatin1String("error"))) {
        out.outcome = Outcome::Refused;
        out.error = errorText(root.value(QLatin1String("error")).toObject());
        return out;
    }

    const QJsonObject result = root.value(QLatin1String("result")).toObject();
    const QJsonValue jobs = result.value(QLatin1String("jobs"));
    if (jobs.isObject() && jobs.toObject().isEmpty()) {
        out.outcome = Outcome::Runs;
        return out;
    }
    if (!jobs.isArray()) {
        out.outcome = Outcome::Unreadable;
        out.error = QStringLiteral("the reply has no list of jobs");
        return out;
    }

    out.outcome = Outcome::Runs;
    const QJsonArray rows = jobs.toArray();
    out.rows = int(rows.size());
    out.runs.reserve(rows.size());

    for (const QJsonValue &rowValue : rows) {
        const QJsonObject row = rowValue.toObject();

        BJobRun run;
        run.name = row.value(QLatin1String("name")).toString();
        run.client = row.value(QLatin1String("client")).toString();
        if (run.name.isEmpty()) {
            continue;
        }
        // Numbers are strings in Bareos; a Director may send numbers
        const QJsonValue id = row.value(QLatin1String("jobid"));
        run.jobId = id.isString() ? id.toString().toULongLong()
                                  : quint64(id.toDouble());
        run.type = codeOf(row.value(QLatin1String("type")));
        run.level = codeOf(row.value(QLatin1String("level")));
        run.status = codeOf(row.value(QLatin1String("jobstatus")));
        run.scheduled = parseTime(row.value(QLatin1String("schedtime")).toString());
        run.started = parseTime(row.value(QLatin1String("starttime")).toString());
        run.ended = parseTime(row.value(QLatin1String("endtime")).toString());
        run.reallyEnded = parseTime(row.value(QLatin1String("realendtime")).toString());

        if (run.scheduled.isValid()) {
            ++out.rowsWithScheduledTime;
        }
        out.runs.append(run);
    }

    return out;
}

// ============================================================================
// BJobHistoryStats: single runs
// ============================================================================

bool BJobHistoryStats::terminatedSuccessfully(QChar status)
{
    // job_status.h: JS_Terminated = 'T', JS_Warnings = 'W'
    return status == QLatin1Char('T') || status == QLatin1Char('W');
}

bool BJobHistoryStats::hasForeignTimes(const BJobRun &run)
{
    return run.scheduled.isValid() && run.started.isValid() && run.started < run.scheduled;
}

std::optional<qint64> BJobHistoryStats::waitSecs(const BJobRun &run)
{
    if (!run.scheduled.isValid() || !run.started.isValid()) {
        return std::nullopt;
    }
    const qint64 wait = run.scheduled.secsTo(run.started);
    if (wait < 0) {
        return std::nullopt;
    }
    return wait;
}

std::optional<qint64> BJobHistoryStats::durationSecs(const BJobRun &run)
{
    const QDateTime end = run.ended.isValid() ? run.ended : run.reallyEnded;
    if (!run.started.isValid() || !end.isValid()) {
        return std::nullopt;
    }
    const qint64 duration = run.started.secsTo(end);
    if (duration < 0) {
        return std::nullopt;
    }
    return duration;
}

// ============================================================================
// BJobHistoryStats: helpers
// ============================================================================

BJobHistoryStats::Quantiles BJobHistoryStats::quantiles(QList<qint64> values)
{
    Quantiles result;
    if (values.isEmpty()) {
        return result;
    }
    std::sort(values.begin(), values.end());
    result.count = int(values.size());
    result.p25 = quantileOf(values, 0.25);
    result.median = quantileOf(values, 0.50);
    result.p75 = quantileOf(values, 0.75);
    result.p90 = quantileOf(values, 0.90);
    result.min = values.first();
    result.max = values.last();
    return result;
}

QString BJobHistoryStats::levelName(QChar code)
{
    // job_level.h
    switch (code.unicode()) {
    case 'F': return QStringLiteral("Full");
    case 'I': return QStringLiteral("Incremental");
    case 'D': return QStringLiteral("Differential");
    case 'f': return QStringLiteral("VirtualFull");
    case 'B': return QStringLiteral("Base");
    case 'S': return QStringLiteral("Since");
    case 'C': return QStringLiteral("Catalog");
    case 'V': return QStringLiteral("InitCatalog");
    case 'O': return QStringLiteral("VolumeToCatalog");
    case 'd': return QStringLiteral("DiskToCatalog");
    case 'A': return QStringLiteral("Data");
    case ' ':
    case 0:
        return QString();
    default:
        return QString(code);
    }
}

QString BJobHistoryStats::formatDuration(qint64 secs)
{
    if (secs < 0) {
        secs = 0;
    }
    const qint64 days = secs / secondsPerDay;
    const qint64 hours = (secs % secondsPerDay) / 3600;
    const qint64 minutes = (secs % 3600) / 60;
    const qint64 seconds = secs % 60;

    if (days > 0) {
        return QStringLiteral("%1d %2h").arg(days).arg(hours, 2, 10, QLatin1Char('0'));
    }
    if (hours > 0) {
        return QStringLiteral("%1h %2m").arg(hours).arg(minutes, 2, 10, QLatin1Char('0'));
    }
    if (minutes > 0) {
        return QStringLiteral("%1m %2s").arg(minutes).arg(seconds, 2, 10, QLatin1Char('0'));
    }
    return QStringLiteral("%1s").arg(seconds);
}

QDateTime BJobHistoryStats::wallClockNow()
{
    const QDateTime local = QDateTime::currentDateTime();
    return QDateTime(local.date(), local.time(), QTimeZone::UTC);
}

QString BJobHistoryStats::key(const QString &job, const QString &level)
{
    // Job names are case sensitive in the catalog query, level names are
    // ours. U+001F cannot be part of a resource name.
    return job + QChar(0x1F) + level.toLower();
}

// ============================================================================
// BJobHistoryStats: the statistics
// ============================================================================

void BJobHistoryStats::clear()
{
    m_groups.clear();
    m_levelsOfJob.clear();
    m_windowDays = 0;
    m_summary = Summary();
}

void BJobHistoryStats::setRuns(const QList<BJobRun> &runs, int windowDays, const QDateTime &now)
{
    clear();
    m_windowDays = windowDays;
    m_summary.rows = int(runs.size());

    const QDateTime oldest = now.addSecs(-qint64(windowDays) * secondsPerDay);

    for (const BJobRun &run : runs) {
        if (!terminatedSuccessfully(run.status)) {
            ++m_summary.notSuccessful;
            continue;
        }

        // The window is counted from the scheduled time, as the Director
        // does for "days="; without one, from the start
        const QDateTime reference = run.scheduled.isValid() ? run.scheduled : run.started;
        if (reference.isValid() && reference < oldest) {
            ++m_summary.outsideWindow;
            continue;
        }

        if (hasForeignTimes(run)) {
            ++m_summary.foreignTimes;
            continue;
        }

        const std::optional<qint64> duration = durationSecs(run);
        if (!duration) {
            ++m_summary.unusableTimes;
            continue;
        }

        MeasuredRun measured;
        measured.jobId = run.jobId;
        measured.level = levelName(run.level);
        measured.started = run.started;
        measured.durationSecs = *duration;
        measured.waitSecs = waitSecs(run);

        ++m_summary.counted;
        if (!measured.waitSecs) {
            ++m_summary.waitUnknown;
        }

        const QString groupKey = key(run.name, measured.level);
        GroupData &data = m_groups[groupKey];
        if (data.runs.isEmpty()) {
            data.job = run.name;
            data.level = measured.level;
            m_levelsOfJob[run.name].append(measured.level);
        }
        data.runs.append(measured);
    }
}

BJobHistoryStats::Group BJobHistoryStats::condense(const QString &job, const QString &level,
                                                   const QList<MeasuredRun> &runs)
{
    Group group;
    group.job = job;
    group.level = level;
    group.runs = int(runs.size());

    QList<qint64> durations;
    QList<qint64> waits;
    durations.reserve(runs.size());
    for (const MeasuredRun &run : runs) {
        durations.append(run.durationSecs);
        if (run.waitSecs) {
            waits.append(*run.waitSecs);
        } else {
            ++group.waitUnknown;
        }
        if (!group.lastRun.isValid() || run.started > group.lastRun) {
            group.lastRun = run.started;
        }
    }
    group.duration = quantiles(durations);
    group.wait = quantiles(waits);
    return group;
}

QList<BJobHistoryStats::Group> BJobHistoryStats::groups() const
{
    QList<Group> result;
    result.reserve(m_groups.size());
    for (const GroupData &data : m_groups) {
        result.append(condense(data.job, data.level, data.runs));
    }
    return result;
}

BJobHistoryStats::Estimate BJobHistoryStats::estimate(const QString &job,
                                                      const QString &level) const
{
    Estimate result;
    if (job.isEmpty() || !m_levelsOfJob.contains(job)) {
        return result;  // NoHistory
    }

    QList<MeasuredRun> runs;
    if (!level.isEmpty()) {
        const auto it = m_groups.constFind(key(job, level));
        if (it != m_groups.constEnd()) {
            runs = it->runs;
            result.basis = Basis::JobAndLevel;
        }
    }
    if (runs.isEmpty()) {
        const QStringList levels = m_levelsOfJob.value(job);
        for (const QString &other : levels) {
            runs.append(m_groups.value(key(job, other)).runs);
        }
        // Asked for the job as a whole and it only ever ran at one level:
        // that is what was measured, not an estimate
        result.basis = (level.isEmpty() && levels.size() == 1) ? Basis::JobAndLevel
                                                               : Basis::AllLevels;
    }

    const Group group = condense(job, level, runs);
    result.runs = group.runs;
    result.duration = group.duration;
    result.wait = group.wait;
    result.waitUnknown = group.waitUnknown;
    return result;
}

QList<BJobHistoryStats::MeasuredRun> BJobHistoryStats::lastRuns(const QString &job,
                                                                const QString &level,
                                                                int count) const
{
    QList<MeasuredRun> runs;
    if (level.isEmpty()) {
        const QStringList levels = m_levelsOfJob.value(job);
        for (const QString &other : levels) {
            runs.append(m_groups.value(key(job, other)).runs);
        }
    } else {
        runs = m_groups.value(key(job, level)).runs;
    }

    std::stable_sort(runs.begin(), runs.end(), [](const MeasuredRun &a, const MeasuredRun &b) {
        return a.started > b.started;
    });
    if (count >= 0 && runs.size() > count) {
        runs.resize(count);
    }
    return runs;
}
