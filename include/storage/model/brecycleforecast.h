#ifndef BRECYCLEFORECAST_H
#define BRECYCLEFORECAST_H

#include <QCoreApplication>
#include <QDateTime>
#include <QString>

#include <optional>

struct BVolume;

/**
 * @brief When a volume may be reused ("reusable from"), and why
 *
 * The rule of the storage plan, after Bareos 25 (core/src/dird):
 *
 * - Full/Used: T_vol = LastWritten + VolRetention. Upstream uses the same
 *   proxy for max(JobTDate) + VolRetention (next_vol.cc:345).
 * - The earlier path through the job retention: T_job = newest job on the
 *   volume + JobRetention. After it AutoPrune deletes the jobs (except the
 *   newest Full/Diff/Inc chain per client and FileSet, ua_prune.cc
 *   1075-1105); a volume without JobMedia becomes Purged at the next
 *   PruneVolumes. Used only when the caller knows the job retention.
 * - Effective: min(T_vol, T_job). It is "reusable from", never "recycled on":
 *   Bareos reuses a volume only when its pool needs one (autoprune.cc 82-200,
 *   lazy).
 * - Never: Append without Volume Use Duration (until it is full), Error,
 *   Read-Only, Disabled, Archive, Recycle = no. Automatic pruning takes Full
 *   and Used volumes only (autoprune.cc:170, ua_prune.cc:1188); Archive
 *   (Enabled = 2) is never pruned (ua_prune.cc:1179).
 * - Purged: reusable now; it holds its disk space until it is recycled or
 *   truncated.
 *
 * Every result carries a reason in one sentence. A field the volume list did
 * not deliver makes the result Unknown, never a guess.
 *
 * Only QtCore; no widget, no Director.
 *
 * @since 2.9
 */
class BRecycleForecast
{
    Q_DECLARE_TR_FUNCTIONS(BRecycleForecast)

public:
    enum class Kind {
        Date,       ///< reusable from @c when (may already have passed)
        Now,        ///< reusable now (Purged, Recycle)
        Never,      ///< not reusable on its own; reason says why
        Unknown     ///< the list lacks a field the rule needs
    };

    enum class Path {
        None,
        VolumeRetention,    ///< LastWritten + VolRetention decided
        JobRetention,       ///< newest job + JobRetention decided (earlier)
        UseDuration         ///< Append volume that becomes Used by its use duration
    };

    /// What the caller knows about the jobs on the volume (step 3 and later)
    struct JobRetention
    {
        qint64 seconds = 0;     ///< effective job retention of the volume's jobs
        QDateTime newestJob;    ///< newest JobTDate on the volume; invalid = use LastWritten
    };

    struct Result
    {
        Kind kind = Kind::Unknown;
        QDateTime when;         ///< for Kind::Date
        Path path = Path::None;
        QString reason;         ///< one sentence, English, for the table and the screen reader

        /// Date passed: due at the pool's next need of a volume
        bool isDue(const QDateTime &now) const { return kind == Kind::Date && when <= now; }
    };

    /**
     * @brief The forecast for one volume
     * @param volume        The catalog record
     * @param now           The present (a parameter, so that tests are fixed)
     * @param jobRetention  Known job retention, if any; without it only the
     *                      volume retention path is computed
     */
    static Result forecast(const BVolume &volume, const QDateTime &now,
                           const std::optional<JobRetention> &jobRetention = std::nullopt);

    /// "365 d", "12 h", "90 min", "45 s"; the largest whole unit
    static QString formatDuration(qint64 seconds);

    /// "9 Mar 2026" (English month names, independent of the locale)
    static QString formatDate(const QDateTime &when);

    /// The text of the "reusable from" column: a date, "now", "never" or "unknown"
    static QString columnText(const Result &result);
};

#endif // BRECYCLEFORECAST_H
