// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

#include "schedules/model/bwaitcalibration.h"

#include <QMap>
#include <QSet>

#include <algorithm>
#include <limits>

namespace {

qint64 secs(const QDateTime &from, const QDateTime &to)
{
    return from.secsTo(to);
}

qint64 medianOf(QList<qint64> values)
{
    return BJobHistoryStats::quantiles(std::move(values)).median;
}

/// The overlap in seconds of two runs' StartTime..EndTime; <= 0: none
qint64 overlapOf(const BWaitCalibration::Run &a, const BWaitCalibration::Run &b)
{
    const QDateTime from = qMax(a.started, b.started);
    const QDateTime to = qMin(a.ended, b.ended);
    return secs(from, to);
}

} // namespace

QString BWaitCalibration::key(const QString &job, const QString &level)
{
    return job + QLatin1Char('\n') + level.trimmed().toLower();
}

void BWaitCalibration::setRuns(const QList<BJobRun> &runs, const QHash<QString, BWaitJobInfo> &jobs,
                               const QHash<QString, int> &devices)
{
    m_runs.clear();
    m_byJobLevel.clear();
    m_byJob.clear();
    m_jobs = jobs;
    m_devices = devices;

    for (const BJobRun &r : runs) {
        if (!BJobHistoryStats::terminatedSuccessfully(r.status))
            continue;
        const std::optional<qint64> wait = BJobHistoryStats::waitSecs(r);
        const std::optional<qint64> duration = BJobHistoryStats::durationSecs(r);
        if (!wait || !duration || !r.ended.isValid())
            continue;

        Run run;
        const BWaitJobInfo info = jobs.value(r.name);
        run.job = r.name;
        run.level = BJobHistoryStats::levelName(r.level);
        run.client = r.client.isEmpty() ? info.client : r.client;
        run.storage = info.storage;
        run.priority = jobs.contains(r.name) ? info.priority : 10;
        run.scheduled = r.scheduled;
        run.started = r.started;
        run.ended = r.ended;
        run.night = r.scheduled.addSecs(-12 * 3600).date();
        run.waitSecs = *wait;
        run.durationSecs = *duration;
        run.pureSecs = *duration;
        m_runs.append(run);
    }

    std::stable_sort(m_runs.begin(), m_runs.end(), [](const Run &a, const Run &b) {
        return a.scheduled < b.scheduled;
    });

    // Nights, to keep the pairs to look at small
    QMap<QDate, QList<int>> byNight;
    for (int i = 0; i < m_runs.size(); ++i)
        byNight[m_runs[i].night].append(i);

    for (const QList<int> &night : std::as_const(byNight)) {
        for (int i : night) {
            Run &r = m_runs[i];

            // Start overhead: from the last moment something could have
            // held it to its start
            QDateTime released = r.scheduled;
            for (int o : night) {
                if (o == i) continue;
                const Run &other = m_runs[o];
                const bool sameClient = !r.client.isEmpty() && other.client == r.client;
                if (!sameClient && other.priority == r.priority) continue;
                if (other.ended > r.scheduled && other.ended <= r.started && other.ended > released)
                    released = other.ended;
            }
            r.startOverheadSecs = qMax<qint64>(0, secs(released, r.started));

            // Pure duration: from the k-th latest end of another run of the
            // storage that ended while this one ran
            const int k = r.storage.isEmpty() ? 0 : devices.value(r.storage, 0);
            if (k > 0) {
                QList<QDateTime> ends;
                for (int o : night) {
                    if (o == i) continue;
                    const Run &other = m_runs[o];
                    if (other.storage == r.storage && other.ended > r.started && other.ended < r.ended)
                        ends.append(other.ended);
                }
                if (ends.size() >= k) {
                    std::sort(ends.begin(), ends.end(), std::greater<QDateTime>());
                    const QDateTime held = qMax(r.started, ends[k - 1]);
                    r.pureSecs = qMax<qint64>(0, secs(held, r.ended));
                }
            }
        }
    }

    for (int i = 0; i < m_runs.size(); ++i) {
        m_byJobLevel[key(m_runs[i].job, m_runs[i].level)].append(i);
        m_byJob[m_runs[i].job].append(i);
    }
}

QList<int> BWaitCalibration::indicesOf(const QString &job, const QString &level) const
{
    return m_byJobLevel.value(key(job, level));
}

BWaitMeasured BWaitCalibration::measuredWait(const QString &job, const QString &level) const
{
    QList<qint64> waits;
    for (int i : indicesOf(job, level))
        waits.append(m_runs[i].waitSecs);
    const BJobHistoryStats::Quantiles q = BJobHistoryStats::quantiles(waits);
    BWaitMeasured measured;
    measured.count = q.count;
    measured.medianSecs = q.median;
    measured.maxSecs = q.max;
    return measured;
}

BWaitCalibration::Overhead BWaitCalibration::startOverhead(const QString &job,
                                                           const QString &level) const
{
    auto from = [&](const QList<int> &indices, Basis basis, Overhead *out) {
        if (indices.isEmpty()) return false;
        QList<qint64> values;
        for (int i : indices) values.append(m_runs[i].startOverheadSecs);
        out->secs = medianOf(values);
        out->basis = basis;
        out->samples = int(values.size());
        return true;
    };

    Overhead overhead;
    if (from(indicesOf(job, level), Basis::JobLevel, &overhead)) return overhead;
    if (from(m_byJob.value(job), Basis::Job, &overhead)) return overhead;
    QList<int> all;
    for (int i = 0; i < m_runs.size(); ++i) all.append(i);
    if (from(all, Basis::Director, &overhead)) return overhead;
    return overhead;
}

BWaitCalibration::Pure BWaitCalibration::pureDuration(const QString &job, const QString &level) const
{
    Pure pure;
    const QList<int> indices = indicesOf(job, level);
    if (indices.isEmpty())
        return pure;
    QList<qint64> pures, raws;
    for (int i : indices) {
        pures.append(m_runs[i].pureSecs);
        raws.append(m_runs[i].durationSecs);
    }
    pure.valid = true;
    pure.samples = int(indices.size());
    pure.secs = medianOf(pures);
    pure.rawSecs = medianOf(raws);
    const QString storage = m_jobs.value(job).storage;
    pure.deviceLimit = storage.isEmpty() ? 0 : m_devices.value(storage, 0);
    return pure;
}

QString BWaitCalibration::overheadNote(const Overhead &overhead)
{
    switch (overhead.basis) {
    case Basis::JobLevel:
        return tr("median of %n measured run(s) at this level", nullptr, int(overhead.samples));
    case Basis::Job:
        return tr("median of %n measured run(s) of this job at other levels", nullptr, int(overhead.samples));
    case Basis::Director:
        return tr("this job was not measured: median of all %1 runs on this Director")
            .arg(overhead.samples);
    case Basis::Default:
        break;
    }
    return tr("nothing measured: %1 assumed").arg(BScheduleWaitModel::formatWait(overhead.secs));
}

QString BWaitCalibration::durationNote(const Pure &pure, const QString &storage)
{
    if (!pure.valid)
        return QString();
    if (pure.deviceLimit <= 0) {
        return tr("Duration %1: median of %2 measured run(s). It includes any wait for a device "
                  "inside the storage daemon (the parallel writers of %3 are not known, so it is "
                  "not taken out).")
            .arg(BScheduleWaitModel::formatWait(pure.secs)).arg(pure.samples)
            .arg(storage.isEmpty() ? tr("its storage") : storage);
    }
    if (pure.secs == pure.rawSecs) {
        return tr("Duration %1: median of %n measured run(s); they did not wait for the device.",
                  nullptr, int(pure.samples))
            .arg(BScheduleWaitModel::formatWait(pure.secs));
    }
    return tr("Duration %1 without the wait for the device (measured %2, median of %3 run(s)): "
              "counted from the end of the run that held the device before it (%4 parallel "
              "writer(s)). It may still hold up to 30 s of the storage daemon's tries.")
        .arg(BScheduleWaitModel::formatWait(pure.secs),
             BScheduleWaitModel::formatWait(pure.rawSecs))
        .arg(pure.samples).arg(pure.deviceLimit);
}

void BWaitCalibration::apply(BWaitRun *run) const
{
    const Overhead overhead = startOverhead(run->job, run->level);
    run->startOverheadSecs = overhead.secs;
    run->overheadNote = overheadNote(overhead);
    run->measuredWait = measuredWait(run->job, run->level);

    const Pure pure = pureDuration(run->job, run->level);
    if (pure.valid) {
        run->durationBasis = BWaitRun::Duration::Measured;
        run->durationSecs = qMax<qint64>(1, pure.secs);
        run->durationNote = durationNote(pure, run->storage);
    }
}

QStringList BWaitCalibration::storages() const
{
    QSet<QString> found;
    for (const Run &r : m_runs) {
        if (!r.storage.isEmpty()) found.insert(r.storage);
    }
    QStringList list(found.begin(), found.end());
    list.sort();
    return list;
}

BWaitCalibration::DeviceSuggestion BWaitCalibration::suggestDevices(const QString &storage) const
{
    DeviceSuggestion out;
    out.storage = storage;

    QMap<QDate, QList<int>> byNight;
    for (int i = 0; i < m_runs.size(); ++i) {
        if (m_runs[i].storage == storage) byNight[m_runs[i].night].append(i);
    }
    out.nights = int(byNight.size());

    // The shortest run of a job at a level when nothing else of the storage
    // ran at the same time: about what it takes when it need not wait
    QHash<QString, qint64> alone;
    for (const QList<int> &night : std::as_const(byNight)) {
        for (int i : night) {
            bool lonely = true;
            for (int o : night) {
                if (o != i && overlapOf(m_runs[i], m_runs[o]) > 0) {
                    lonely = false;
                    break;
                }
            }
            if (!lonely) continue;
            const QString k = key(m_runs[i].job, m_runs[i].level);
            const auto it = alone.constFind(k);
            if (it == alone.constEnd() || m_runs[i].durationSecs < it.value())
                alone.insert(k, m_runs[i].durationSecs);
        }
    }

    QSet<QDate> informativeNights;
    QString serialExample, parallelExample;
    for (auto it = byNight.constBegin(); it != byNight.constEnd(); ++it) {
        const QList<int> &night = it.value();
        bool contended = false;
        bool overlapped = false;
        for (int a = 0; a < night.size(); ++a) {
            for (int b = a + 1; b < night.size(); ++b) {
                const Run &x = m_runs[night[a]];
                const Run &y = m_runs[night[b]];
                // Both queued or running at the same moment
                if (qMax(x.scheduled, y.scheduled) < qMin(x.ended, y.ended))
                    contended = true;
                if (overlapOf(x, y) <= 0)
                    continue;
                overlapped = true;

                // A ends first, B later. One writer: B got the device after
                // A released it, so B runs on for at least its own length.
                // Several: B ends its own length after its start, which is
                // before the end of A.
                const Run &first = x.ended <= y.ended ? x : y;
                const Run &later = x.ended <= y.ended ? y : x;
                const auto own = alone.constFind(key(later.job, later.level));
                if (own == alone.constEnd()) continue;
                const qint64 tolerance = qMax<qint64>(PairToleranceSecs, own.value() / 5);
                // Both readings say about the same when B started near the end of A
                if (secs(later.started, first.ended) <= tolerance) continue;
                ++out.informativePairs;
                informativeNights.insert(it.key());
                const qint64 after = secs(first.ended, later.ended);
                const QString example = tr("%1 ended %2 after %3 on %4; alone it takes at least %5")
                    .arg(later.job, BScheduleWaitModel::formatWait(after), first.job,
                         it.key().toString(QStringLiteral("yyyy-MM-dd")),
                         BScheduleWaitModel::formatWait(own.value()));
                if (after >= own.value() - tolerance) {
                    ++out.serialPairs;
                    if (serialExample.isEmpty()) serialExample = example;
                } else {
                    ++out.parallelPairs;
                    if (parallelExample.isEmpty()) parallelExample = example;
                }
            }
        }
        if (contended) ++out.contendedNights;
        if (overlapped) ++out.overlapNights;
    }

    out.evidence << tr("%1 night(s) with runs on storage %2; on %3 more than one of them was "
                       "queued or running at the same time; on %4 the runs' recorded start to end "
                       "overlapped.")
                        .arg(out.nights).arg(storage).arg(out.contendedNights).arg(out.overlapNights);

    if (out.contendedNights >= SuggestMinNights && out.overlapNights == 0) {
        out.suggested = 1;
        out.evidence << tr("No two runs ever overlapped although several were queued: the "
                           "storage wrote one job at a time.");
        return out;
    }

    if (out.overlapNights > 0) {
        out.evidence << tr("Overlapping runs do not prove parallel writing: with Just In Time "
                           "Reservation a job waits for the device inside its run.");
    }
    if (out.parallelPairs > 0) {
        out.evidence << tr("%1 of %2 telling pair(s) ended sooner after each other than one "
                           "writer allows (e.g. %3): it writes more than one job at a time.")
                            .arg(out.parallelPairs).arg(out.informativePairs).arg(parallelExample);
        return out;
    }
    if (out.informativePairs >= SuggestMinPairs && informativeNights.size() >= 2) {
        out.suggested = 1;
        out.evidence << tr("In all %1 telling pair(s) on %2 night(s) the later run went on after "
                           "the other ended for at least as long as it takes alone (e.g. %3): "
                           "consistent with one writer at a time.")
                            .arg(out.informativePairs).arg(informativeNights.size())
                            .arg(serialExample);
        return out;
    }
    out.evidence << tr("Not enough telling pairs (%1; a pair tells when the later run started well "
                       "before the other ended and was also seen running alone): no suggestion.")
                        .arg(out.informativePairs);
    return out;
}

void BWaitCalibration::orderTies(QList<BWaitRun> *runs)
{
    std::stable_sort(runs->begin(), runs->end(), [](const BWaitRun &a, const BWaitRun &b) {
        if (a.scheduled != b.scheduled)
            return a.scheduled < b.scheduled;
        const qint64 never = std::numeric_limits<qint64>::max();
        const qint64 wa = a.measuredWait.isValid() ? a.measuredWait.medianSecs : never;
        const qint64 wb = b.measuredWait.isValid() ? b.measuredWait.medianSecs : never;
        return wa < wb;
    });
}

BWaitCalibration::Backtest BWaitCalibration::backtest(const BWaitLimits &limits, int nights) const
{
    Backtest out;
    QMap<QDate, QList<int>> byNight;
    for (int i = 0; i < m_runs.size(); ++i)
        byNight[m_runs[i].night].append(i);

    QList<QDate> dates = byNight.keys();
    while (dates.size() > nights)
        dates.removeFirst();

    QSet<QString> unknownDevices;
    QList<qint64> absErrors;
    for (const QDate &date : std::as_const(dates)) {
        QList<BWaitRun> replay;
        QList<int> source;
        for (int i : byNight.value(date)) {
            const Run &r = m_runs[i];
            BWaitRun run;
            run.id = QString::number(i);
            run.job = r.job;
            run.client = r.client;
            run.usesClient = !r.client.isEmpty();
            run.storage = r.storage;
            run.usesStorage = !r.storage.isEmpty();
            run.level = r.level;
            run.priority = r.priority;
            run.allowMixedPriority = false;
            run.scheduled = r.scheduled;
            apply(&run);
            if (run.usesStorage && !limits.devices.contains(run.storage))
                unknownDevices.insert(run.storage);
            replay.append(run);
        }
        orderTies(&replay);
        for (const BWaitRun &run : std::as_const(replay))
            source.append(run.id.toInt());

        const QList<BWaitResult> results = BScheduleWaitModel::predict(replay, limits);
        for (int k = 0; k < replay.size() && k < results.size(); ++k) {
            const Run &r = m_runs[source[k]];
            BacktestRow row;
            row.job = r.job;
            row.level = r.level;
            row.night = date;
            row.scheduled = r.scheduled;
            row.measuredStart = r.started;
            row.predictedStart = results[k].start;
            row.errorSecs = secs(r.started, results[k].start);
            out.rows.append(row);
            absErrors.append(qAbs(row.errorSecs));
            if (qAbs(row.errorSecs) > qAbs(out.worstErrorSecs) || out.worstJob.isEmpty()) {
                out.worstErrorSecs = row.errorSecs;
                out.worstJob = r.job;
                out.worstNight = date;
            }
        }
    }

    out.nights = int(dates.size());
    out.runs = int(out.rows.size());
    out.medianAbsErrorSecs = medianOf(absErrors);
    out.unknownDevices = QStringList(unknownDevices.begin(), unknownDevices.end());
    out.unknownDevices.sort();
    return out;
}

QString BWaitCalibration::Backtest::text() const
{
    if (runs == 0)
        return tr("Back-test: no measured night to replay.");
    QString text = tr("Back-test, last %1 night(s) (%2 runs): median error %3, worst %4 "
                      "(%5, %6; %7).")
        .arg(nights).arg(runs)
        .arg(BScheduleWaitModel::formatWait(medianAbsErrorSecs),
             BScheduleWaitModel::formatWait(qAbs(worstErrorSecs)), worstJob,
             worstNight.toString(QStringLiteral("yyyy-MM-dd")),
             worstErrorSecs > 0 ? tr("predicted too late") : tr("predicted too early"));
    if (!unknownDevices.isEmpty()) {
        text += QLatin1Char(' ') + tr("Replayed without a device limit for %1 (not set).")
                                       .arg(unknownDevices.join(QStringLiteral(", ")));
    }
    text += QLatin1Char(' ') + tr("Durations and overheads come from the same history.");
    return text;
}
