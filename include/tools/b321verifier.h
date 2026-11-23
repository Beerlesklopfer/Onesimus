// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

#ifndef B321VERIFIER_H
#define B321VERIFIER_H

#include <QByteArray>
#include <QChar>
#include <QCoreApplication>
#include <QDateTime>
#include <QHash>
#include <QList>
#include <QSet>
#include <QString>
#include <QStringList>

#include <optional>

#include "storage/model/bvolumelist.h"

/**
 * @brief Does a backup job follow the 3-2-1 rule? Without widgets, without a Director
 *
 * The rule: at least 3 copies of the data, on at least 2 different media
 * types, at least 1 copy off site. It is judged for the data a restore of
 * the job would use today, from what the catalog says.
 *
 * Where the facts come from (Bareos, branch bareos-25):
 * - core/src/include/job_types.h: 'B' backup, 'C' copy of a job (the job
 *   record that holds the copied data), 'c' copy control job, 'g' migration
 *   control job, 'M' a backup that was migrated away.
 * - core/src/dird/migrate.cc (around line 1660): a Copy or Migrate run
 *   creates a new job record with PriorJobId = the copied or migrated job
 *   and the start and end time of that job; RealEndTime is the time of the
 *   copy. After a Copy the new record gets Type 'C'. After a Migrate the new
 *   record stays 'B' and the old one becomes 'M'. The control job ('c', 'g')
 *   stores the new JobId in its own PriorJobId and holds no data.
 * - core/src/cats/sql_update.cc UpgradeCopies: when the original is purged,
 *   its oldest copy becomes Type 'B'.
 * - core/src/dird/ua_restore.cc and cats (uar_last_full, uar_dif, uar_inc):
 *   the restore of the most recent backup takes the newest successful Full
 *   (JobStatus 'T' or 'W') of the client and FileSet, the newest
 *   Differential after it and every Incremental after the newer of the two.
 *
 * The rules implemented here:
 *
 * Chain. For the Job resource N: the newest successful ('T' or 'W') Full of
 * Type 'B' named N. Its client and FileSet select the rest of the chain, as
 * the restore does: the newest successful Differential after the Full and
 * every successful Incremental after the newer of Full and Differential, of
 * any job name with that client and FileSet. Without client or FileSet in
 * the reply, the job name selects instead. A 'B' job with a PriorJobId is
 * the result of a Migrate (or an upgraded copy) and is a normal chain member.
 *
 * Copies. The chain counts as 1. With countOriginalData (default on, the
 * rule as usually stated: production data + 2 backups) the data on the
 * client counts as one more. Copies are the successful jobs of Type 'C'
 * whose PriorJobId leads (through other 'C' jobs) to a chain member. They
 * are grouped by their pool, the pool a Copy job writes to: a pool that
 * holds a copy of every chain member is one more copy; a pool that holds
 * copies of some members only is reported as a partial copy and does not
 * count. A Migrate moves data and adds no copy: the moved job is in the
 * chain, the 'M' record it came from is not restored any more.
 *
 * Media types. The MediaType of the volumes (JobMedia) that hold the chain
 * and each complete copy. The count is the largest number of these copies
 * whose media types do not overlap: a chain on File and a copy on LTO make
 * 2; a chain that spans File and LTO (after a Migrate, say) without a copy
 * makes 1, as losing either medium loses the chain. The same MediaType on
 * two storages is one type. The data on the client is not a medium of this
 * count. Met with 2 or more; unknown while a volume or its MediaType is not
 * known and fewer than 2 are known.
 *
 * Off site. Bareos has no off-site flag. A volume is off site if its
 * storage is marked off site by the user (1), else if the catalog has a
 * Location for it (LocationId > 0, 2); it is on site if the user said that
 * unmarked storages are on site; otherwise unknown (3). Addresses are never
 * used. A copy (the chain or a complete copy) is off site if all its
 * volumes are; it is on site if one of them is on site. Met if at least one
 * is off site; unknown if none is but one is unknown.
 *
 * Verdict. Fails if a criterion fails, else unknown if one is unknown,
 * else passes.
 *
 * Hints, shown but not part of the verdict: age of the newest Full against
 * the usual interval of the job's Fulls, age of the newest copy, copies that
 * belong to an earlier chain, chain volumes in Error, Purged or Recycle.
 *
 * Only QtCore.
 */
class B321Verifier
{
    Q_DECLARE_TR_FUNCTIONS(B321Verifier)

public:
    /// One job record of the catalog ("llist jobs")
    struct Job
    {
        quint64 jobId = 0;
        QString name;               ///< Job.Name
        QChar type;                 ///< 'B', 'C', 'c', 'g', 'M', ...
        QChar level;                ///< 'F', 'D', 'I', ...
        QChar status;               ///< 'T', 'W', 'E', ...
        quint64 priorJobId = 0;
        QString client;
        QString fileset;
        QString pool;               ///< Pool.Name
        QDateTime started;          ///< wall-clock time of the Director
        QDateTime ended;
        QDateTime reallyEnded;      ///< for a copy: when the copy was made
        std::optional<qint64> bytes;

        bool isSuccessful() const { return status == QLatin1Char('T') || status == QLatin1Char('W'); }
    };

    /// A Job resource of type Backup
    struct Definition
    {
        QString name;
        QString client;             ///< from the configuration; may be empty
        QString fileset;
    };

    struct Input
    {
        /// The Job resources of type Backup. Empty with definitionsKnown
        /// false: every name of a backup job in the catalog is checked.
        QList<Definition> definitions;
        bool definitionsKnown = false;

        QList<Job> jobs;
        bool jobsKnown = false;
        bool jobsTruncated = false; ///< the reply hit its row limit
        int historyDays = 0;        ///< window of the job list; 0 = not said

        QHash<quint64, QStringList> jobMedia;   ///< JobId -> volume names
        bool jobMediaKnown = false;

        QList<BVolume> volumes;
        bool volumesKnown = false;

        QSet<QString> offSiteStorages;  ///< storages the user marked off site
        bool unmarkedAreOnSite = false; ///< the user said: the others are on site

        bool countOriginalData = true;
        QDateTime now;              ///< wall-clock time, as the job times
    };

    enum class State { Met, Failed, Unknown };
    enum class Verdict { Passes, Fails, Unknown };
    enum class OffSite { Yes, No, Unknown };
    enum class OffSiteSource {
        None,           ///< nothing known
        Marking,        ///< storage marked off site by the user
        Location,       ///< the catalog has a Location for the volume
        UnmarkedOnSite  ///< the user said unmarked storages are on site
    };

    /// Where one volume of a copy lies
    struct Placement
    {
        QString volume;
        bool known = false;         ///< the volume is in the volume list
        QString mediaType;
        QString storage;
        QString status;
        std::optional<qint64> locationId;
        OffSite offSite = OffSite::Unknown;
        OffSiteSource source = OffSiteSource::None;
    };

    /// The chain itself or the copies of it in one pool
    struct CopySet
    {
        bool original = false;      ///< the chain itself
        QString pool;               ///< pool of the copy jobs (copies)
        QList<quint64> jobIds;      ///< jobs that hold this copy, in chain order
        QList<quint64> covered;     ///< chain members this copy covers
        QList<quint64> missing;     ///< chain members without a copy here
        bool complete = false;
        QList<Placement> placements;    ///< distinct volumes
        QList<quint64> jobsWithoutVolume;   ///< jobs with no JobMedia record
        QStringList mediaTypes;     ///< distinct, known ones
        QStringList storages;       ///< distinct, known ones
        OffSite offSite = OffSite::Unknown;
        QDateTime written;          ///< newest copy time (copies) or chain end
    };

    struct Result
    {
        Definition job;
        bool inCatalog = false;     ///< a successful Full was found
        QList<Job> chain;           ///< Full, Differential, Incrementals, by time
        QHash<quint64, Job> migratedFrom;   ///< chain JobId -> the 'M' job it came from
        QList<CopySet> sets;        ///< [0] the chain itself, then copies by pool

        int copies = 0;
        static constexpr int copiesRequired = 3;
        State copiesState = State::Failed;

        int mediaTypes = 0;         ///< copies on media types of their own
        static constexpr int mediaTypesRequired = 2;
        QStringList mediaTypeNames; ///< every media type of the counted copies
        State mediaState = State::Unknown;

        int offSiteCopies = 0;
        static constexpr int offSiteRequired = 1;
        State offSiteState = State::Unknown;

        Verdict verdict = Verdict::Unknown;
        QString summary;            ///< one line for the table
        QStringList reasons;        ///< plain sentences
        QStringList suggestions;    ///< what to do
        QStringList hints;          ///< freshness and integrity, not in the verdict

        int completeCopySets() const;
        QList<const CopySet *> partialCopySets() const;
    };

    /// Every job of the input
    static QList<Result> verify(const Input &input);

    /**
     * @brief The rows of "llist jobs" (API mode 2 or typed JSON)
     * @param error Receives the Director's message or a parse error
     * @return the jobs; empty with an error if the reply is not usable
     */
    static QList<Job> parseJobs(const QByteArray &json, QString *error = nullptr);

    /// The rows of "list jobmedia" or "llist jobmedia": JobId -> volume names
    static QHash<quint64, QStringList> parseJobMedia(const QByteArray &json,
                                                     QString *error = nullptr);

    /// A catalog time "yyyy-MM-dd HH:mm:ss" as wall-clock time (UTC spec)
    static QDateTime parseTime(const QString &text);

    /// The present as wall-clock time, comparable with parseTime()
    static QDateTime wallClockNow();

    static QString stateText(State state);
    static QString verdictText(Verdict verdict);
    static QString levelName(QChar level);
    static QString offSiteText(OffSite offSite);
};

#endif // B321VERIFIER_H
