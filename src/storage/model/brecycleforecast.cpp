// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

#include "storage/model/brecycleforecast.h"

#include "bbyteformat.h"
#include "storage/model/bvolumelist.h"

#include <QLocale>

namespace {

BRecycleForecast::Result make(BRecycleForecast::Kind kind, const QString &reason,
                              const QDateTime &when = QDateTime(),
                              BRecycleForecast::Path path = BRecycleForecast::Path::None)
{
    BRecycleForecast::Result r;
    r.kind = kind;
    r.reason = reason;
    r.when = when;
    r.path = path;
    return r;
}

bool is(const QString &status, const char *name)
{
    return status.compare(QLatin1String(name), Qt::CaseInsensitive) == 0;
}

} // namespace

QString BRecycleForecast::formatDuration(qint64 seconds)
{
    if (seconds <= 0) {
        return QStringLiteral("0 s");
    }
    if (seconds % 86400 == 0) {
        return QStringLiteral("%1 d").arg(seconds / 86400);
    }
    if (seconds >= 86400) {
        // Not whole days: one decimal is enough for a retention
        return QStringLiteral("%1 d").arg(QString::number(seconds / 86400.0, 'f', 1));
    }
    if (seconds % 3600 == 0) {
        return QStringLiteral("%1 h").arg(seconds / 3600);
    }
    if (seconds % 60 == 0) {
        return QStringLiteral("%1 min").arg(seconds / 60);
    }
    return QStringLiteral("%1 s").arg(seconds);
}

QString BRecycleForecast::formatDate(const QDateTime &when)
{
    if (!when.isValid()) {
        return QString();
    }
    return QLocale::c().toString(when.date(), QStringLiteral("d MMM yyyy"));
}

QString BRecycleForecast::columnText(const Result &result)
{
    switch (result.kind) {
    case Kind::Date: return formatDate(result.when);
    case Kind::Now: return tr("now");
    case Kind::Never: return tr("never");
    case Kind::Unknown: break;
    }
    return tr("unknown");
}

BRecycleForecast::Result BRecycleForecast::forecast(const BVolume &volume, const QDateTime &now,
                                                    const std::optional<JobRetention> &jobRetention)
{
    const QString &status = volume.status;
    if (status.isEmpty()) {
        return make(Kind::Unknown, tr("The Director did not report the status of this volume."));
    }

    // Archive (status, or Enabled = 2) is never pruned (ua_prune.cc:1179)
    if (is(status, "Archive") || volume.enabled == 2) {
        return make(Kind::Never, tr("Archived: Bareos never prunes or recycles an archived volume."));
    }
    if (is(status, "Disabled") || volume.enabled == 0) {
        return make(Kind::Never, tr("Disabled: Bareos neither writes to nor recycles a disabled volume."));
    }
    if (is(status, "Error")) {
        return make(Kind::Never,
                    tr("Error: Bareos never prunes a volume in Error, so it keeps its space "
                       "until its status is changed. The job records on it still expire."));
    }
    if (is(status, "Read-Only")) {
        return make(Kind::Never, tr("Read-Only: Bareos does not prune or recycle a read-only volume."));
    }
    if (is(status, "Cleaning")) {
        return make(Kind::Never, tr("Cleaning tape: never used for data."));
    }
    // Recycle = no: Bareos never takes it for reuse, not even when Purged
    if (volume.recycle.has_value() && !*volume.recycle
        && (is(status, "Full") || is(status, "Used") || is(status, "Append") || is(status, "Purged"))) {
        return make(Kind::Never, tr("Recycle = no: the volume is never reused; "
                                    "its data stays until it is relabeled or deleted."));
    }
    if (is(status, "Purged")) {
        QString reason = tr("Purged: no job needs it any more, reusable now.");
        if (volume.volBytes.has_value() && *volume.volBytes > 64512) {
            // Truncate takes Purged volumes above 64512 bytes (ua_cmds.cc:1974-2010)
            reason += QLatin1Char(' ')
                + tr("It still holds %1 on disk until it is recycled or truncated.")
                      .arg(BByteFormat::format(*volume.volBytes));
        }
        return make(Kind::Now, reason, now);
    }
    if (is(status, "Recycle")) {
        return make(Kind::Now, tr("Marked for recycling: reused at the next need of its pool."), now);
    }
    if (is(status, "Busy")) {
        return make(Kind::Unknown, tr("Busy: in use by a job right now."));
    }
    if (is(status, "Unlabeled")) {
        return make(Kind::Unknown, tr("Unlabeled: not yet written."));
    }

    if (is(status, "Append")) {
        if (!volume.volUseDuration.has_value()) {
            return make(Kind::Unknown,
                        tr("Append: the short volume list does not report the Volume Use Duration, "
                           "so it is not known whether this volume stays open until it is full."));
        }
        if (*volume.volUseDuration <= 0) {
            QString reason = tr("Append without Volume Use Duration: it stays open until it is full");
            if (volume.maxVolBytes.has_value() && *volume.maxVolBytes > 0) {
                reason += QLatin1Char(' ') + tr("(%1)").arg(BByteFormat::format(*volume.maxVolBytes));
            }
            if (volume.maxVolJobs.has_value() && *volume.maxVolJobs > 0) {
                reason += QLatin1Char(' ') + tr("or holds %1 jobs").arg(*volume.maxVolJobs);
            }
            reason += tr("; nothing on it is freed before that.");
            return make(Kind::Never, reason);
        }
        // It becomes Used at FirstWritten + VolUseDuration (next_vol.cc:229-288)
        if (!volume.firstWritten.isValid() || !volume.volRetention.has_value()) {
            return make(Kind::Unknown,
                        tr("Append with a use duration of %1, but the first write or the retention "
                           "is not reported.").arg(formatDuration(*volume.volUseDuration)));
        }
        const QDateTime usedAt = volume.firstWritten.addSecs(*volume.volUseDuration);
        // Already past: Bareos marks it Used at its next look; jobs stopped by then
        const QDateTime lastWrite = usedAt <= now && volume.lastWritten.isValid()
            ? volume.lastWritten : usedAt;
        const QDateTime when = lastWrite.addSecs(*volume.volRetention);
        return make(Kind::Date,
                    tr("Append until %1 (use duration %2), then %3 of volume retention.")
                        .arg(formatDate(usedAt), formatDuration(*volume.volUseDuration),
                             formatDuration(*volume.volRetention)),
                    when, Path::UseDuration);
    }

    if (is(status, "Full") || is(status, "Used")) {
        if (!volume.lastWritten.isValid() || !volume.volRetention.has_value()) {
            return make(Kind::Unknown, tr("%1: the last write or the volume retention is not reported.")
                                           .arg(status));
        }
        const QDateTime tVol = volume.lastWritten.addSecs(*volume.volRetention);
        Result result = make(Kind::Date,
                             tr("%1: last written %2, volume retention %3.")
                                 .arg(status, formatDate(volume.lastWritten),
                                      formatDuration(*volume.volRetention)),
                             tVol, Path::VolumeRetention);
        if (jobRetention.has_value()) {
            const QDateTime newest = jobRetention->newestJob.isValid()
                ? jobRetention->newestJob : volume.lastWritten;
            const QDateTime tJob = newest.addSecs(jobRetention->seconds);
            if (tJob < tVol) {
                result.when = tJob;
                result.path = Path::JobRetention;
                result.reason = tr("%1: the job retention of %2 ends before the volume retention "
                                   "of %3; the jobs are pruned first and the volume becomes Purged.")
                                    .arg(status, formatDuration(jobRetention->seconds),
                                         formatDuration(*volume.volRetention));
            }
        }
        if (result.when <= now) {
            result.reason += QLatin1Char(' ')
                + tr("The date has passed: it is pruned when its pool next needs a volume.");
        }
        return result;
    }

    return make(Kind::Unknown, tr("Status \"%1\" is not known to this version.").arg(status));
}
