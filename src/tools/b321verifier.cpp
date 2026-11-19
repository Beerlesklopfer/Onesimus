#include "tools/b321verifier.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLocale>
#include <QMap>
#include <QMultiHash>
#include <QTimeZone>

#include <algorithm>

namespace {

using Job = B321Verifier::Job;

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
            if (!line.isEmpty()) lines.append(line);
        }
    }
    if (lines.isEmpty()) {
        const QString message = error.value(QLatin1String("message")).toString().trimmed();
        if (!message.isEmpty()) lines.append(message);
    }
    return lines.join(QLatin1Char(' '));
}

/// The "result" object of a reply, or an error text
bool resultOf(const QByteArray &json, QJsonObject *result, QString *error)
{
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(json, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        if (error) {
            const QString text = QString::fromUtf8(json).simplified();
            *error = text.isEmpty() ? QStringLiteral("empty reply") : text.left(200);
        }
        return false;
    }
    const QJsonObject root = doc.object();
    if (root.contains(QLatin1String("error"))) {
        if (error) *error = errorText(root.value(QLatin1String("error")).toObject());
        return false;
    }
    *result = root.value(QLatin1String("result")).toObject();
    return true;
}

QString textOf(const QJsonValue &value)
{
    if (value.isString()) return value.toString().trimmed();
    if (value.isDouble()) return QString::number(qint64(value.toDouble()));
    return QString();
}

quint64 idOf(const QJsonValue &value)
{
    if (value.isDouble()) return quint64(qMax(0.0, value.toDouble()));
    return value.toString().trimmed().toULongLong();
}

QChar codeOf(const QJsonValue &value)
{
    const QString text = textOf(value);
    return text.isEmpty() ? QChar() : text.at(0);
}

/// Order of jobs in time: start time, then JobId
bool earlier(const Job &a, const Job &b)
{
    if (a.started.isValid() && b.started.isValid() && a.started != b.started)
        return a.started < b.started;
    return a.jobId < b.jobId;
}

QString dateText(const QDateTime &when)
{
    return when.isValid() ? QLocale::c().toString(when, QStringLiteral("yyyy-MM-dd HH:mm"))
                          : B321Verifier::tr("unknown time");
}

/// "Full 123 (2026-11-01 23:05)"
QString jobText(const Job &job)
{
    return B321Verifier::tr("%1 %2 (%3)")
        .arg(B321Verifier::levelName(job.level)).arg(job.jobId).arg(dateText(job.started));
}

QString joinedIds(const QList<quint64> &ids)
{
    QStringList parts;
    for (quint64 id : ids) parts.append(QString::number(id));
    return parts.join(QStringLiteral(", "));
}

QString poolLabel(const QString &pool)
{
    return pool.isEmpty() ? B321Verifier::tr("(pool not reported)") : pool;
}

struct Index
{
    QHash<quint64, const Job *> byId;
    QMultiHash<quint64, const Job *> copiesOf;  ///< original JobId -> successful 'C' jobs
    QHash<QString, const BVolume *> volumes;
};

/// The job a copy was made of, through copies of copies
quint64 originalOf(const Job &copy, const Index &index)
{
    quint64 id = copy.priorJobId;
    for (int depth = 0; depth < 16 && id != 0; ++depth) {
        const Job *prior = index.byId.value(id, nullptr);
        if (!prior || prior->type != QLatin1Char('C')) return id;
        id = prior->priorJobId;
    }
    return id;
}

B321Verifier::Placement placementOf(const QString &volumeName, const Index &index,
                                    const B321Verifier::Input &input)
{
    B321Verifier::Placement p;
    p.volume = volumeName;
    if (const BVolume *volume = index.volumes.value(volumeName, nullptr)) {
        p.known = true;
        p.mediaType = volume->mediaType;
        p.storage = volume->storage;
        p.status = volume->status;
        p.locationId = volume->locationId;
    }
    using O = B321Verifier::OffSite;
    using S = B321Verifier::OffSiteSource;
    if (!p.storage.isEmpty() && input.offSiteStorages.contains(p.storage)) {
        p.offSite = O::Yes;
        p.source = S::Marking;
    } else if (p.locationId && *p.locationId > 0) {
        p.offSite = O::Yes;
        p.source = S::Location;
    } else if (input.unmarkedAreOnSite && !p.storage.isEmpty()) {
        p.offSite = O::No;
        p.source = S::UnmarkedOnSite;
    }
    return p;
}

/// Volumes, media types, storages and off-site state of the jobs of a set
void place(B321Verifier::CopySet &set, const Index &index, const B321Verifier::Input &input)
{
    QSet<QString> seen;
    for (quint64 id : std::as_const(set.jobIds)) {
        const QStringList names = input.jobMediaKnown ? input.jobMedia.value(id) : QStringList();
        if (names.isEmpty()) {
            set.jobsWithoutVolume.append(id);
            continue;
        }
        for (const QString &name : names) {
            if (seen.contains(name)) continue;
            seen.insert(name);
            set.placements.append(placementOf(name, index, input));
        }
    }
    bool anyNo = false;
    bool allYes = set.jobsWithoutVolume.isEmpty() && !set.placements.isEmpty();
    for (const B321Verifier::Placement &p : std::as_const(set.placements)) {
        if (!p.mediaType.isEmpty() && !set.mediaTypes.contains(p.mediaType))
            set.mediaTypes.append(p.mediaType);
        if (!p.storage.isEmpty() && !set.storages.contains(p.storage))
            set.storages.append(p.storage);
        anyNo = anyNo || p.offSite == B321Verifier::OffSite::No;
        allYes = allYes && p.offSite == B321Verifier::OffSite::Yes;
    }
    set.offSite = anyNo ? B321Verifier::OffSite::No
                        : (allYes ? B321Verifier::OffSite::Yes : B321Verifier::OffSite::Unknown);
}

bool setHasUnknownMedia(const B321Verifier::CopySet &set)
{
    if (!set.jobsWithoutVolume.isEmpty()) return true;
    for (const B321Verifier::Placement &p : set.placements) {
        if (!p.known || p.mediaType.isEmpty()) return true;
    }
    return false;
}

/// The largest number of the type sets that share no media type
int largestDisjointGroup(const QList<QStringList> &typeSets)
{
    const qsizetype n = typeSets.size();
    if (n == 0) return 0;
    if (n > 16) {
        // Greedy beyond what an exhaustive search should try
        QSet<QString> used;
        int count = 0;
        for (const QStringList &types : typeSets) {
            bool available = true;
            for (const QString &type : types) available = available && !used.contains(type);
            if (!available) continue;
            for (const QString &type : types) used.insert(type);
            ++count;
        }
        return count;
    }
    int best = 0;
    for (quint32 mask = 1; mask < (quint32(1) << n); ++mask) {
        QSet<QString> used;
        int count = 0;
        bool disjoint = true;
        for (qsizetype i = 0; i < n && disjoint; ++i) {
            if (!(mask & (quint32(1) << i))) continue;
            for (const QString &type : typeSets.at(i)) {
                if (used.contains(type)) {
                    disjoint = false;
                    break;
                }
                used.insert(type);
            }
            ++count;
        }
        if (disjoint) best = qMax(best, count);
    }
    return best;
}

qint64 medianOf(QList<qint64> values)
{
    std::sort(values.begin(), values.end());
    const qsizetype n = values.size();
    return n % 2 ? values.at(n / 2) : (values.at(n / 2 - 1) + values.at(n / 2)) / 2;
}

B321Verifier::Result verifyJob(const B321Verifier::Definition &definition,
                               const B321Verifier::Input &input, const Index &index)
{
    using R = B321Verifier::Result;
    using State = B321Verifier::State;
    using OffSite = B321Verifier::OffSite;

    R r;
    r.job = definition;

    // --- The chain the restore would use ---------------------------------
    const Job *full = nullptr;
    QList<const Job *> fulls;
    for (const Job &job : input.jobs) {
        if (job.type != QLatin1Char('B') || !job.isSuccessful() || job.level != QLatin1Char('F')
            || job.name != definition.name)
            continue;
        fulls.append(&job);
        if (!full || earlier(*full, job)) full = &job;
    }

    if (full) {
        r.inCatalog = true;
        if (r.job.client.isEmpty()) r.job.client = full->client;
        if (r.job.fileset.isEmpty()) r.job.fileset = full->fileset;

        const bool byScope = !full->client.isEmpty() && !full->fileset.isEmpty();
        auto sameScope = [&](const Job &job) {
            return byScope ? (job.client == full->client && job.fileset == full->fileset)
                           : job.name == full->name;
        };
        const Job *diff = nullptr;
        for (const Job &job : input.jobs) {
            if (job.type != QLatin1Char('B') || !job.isSuccessful() || job.level != QLatin1Char('D')
                || !sameScope(job) || !earlier(*full, job))
                continue;
            if (!diff || earlier(*diff, job)) diff = &job;
        }
        const Job *base = diff ? diff : full;
        QList<Job> incs;
        for (const Job &job : input.jobs) {
            if (job.type != QLatin1Char('B') || !job.isSuccessful() || job.level != QLatin1Char('I')
                || !sameScope(job) || !earlier(*base, job))
                continue;
            incs.append(job);
        }
        std::sort(incs.begin(), incs.end(), earlier);
        r.chain.append(*full);
        if (diff) r.chain.append(*diff);
        r.chain.append(incs);

        for (const Job &member : std::as_const(r.chain)) {
            if (member.priorJobId == 0) continue;
            const Job *prior = index.byId.value(member.priorJobId, nullptr);
            if (prior && prior->type == QLatin1Char('M')) r.migratedFrom.insert(member.jobId, *prior);
        }
    }

    QList<quint64> chainIds;
    for (const Job &member : std::as_const(r.chain)) chainIds.append(member.jobId);

    // --- The chain itself, then the copies by pool ------------------------
    if (!r.chain.isEmpty()) {
        B321Verifier::CopySet original;
        original.original = true;
        original.jobIds = chainIds;
        original.covered = chainIds;
        original.complete = true;
        for (const Job &member : std::as_const(r.chain)) {
            const QDateTime end = member.ended.isValid() ? member.ended : member.started;
            if (end.isValid() && (!original.written.isValid() || end > original.written))
                original.written = end;
        }
        place(original, index, input);
        r.sets.append(original);

        // pool -> chain member -> the newest copy of it in that pool
        QMap<QString, QHash<quint64, const Job *>> byPool;
        for (quint64 member : std::as_const(chainIds)) {
            const QList<const Job *> copies = index.copiesOf.values(member);
            for (const Job *copy : copies) {
                const Job *&kept = byPool[copy->pool][member];
                if (!kept || kept->reallyEnded < copy->reallyEnded
                    || (kept->reallyEnded == copy->reallyEnded && kept->jobId < copy->jobId))
                    kept = copy;
            }
        }
        for (auto it = byPool.constBegin(); it != byPool.constEnd(); ++it) {
            B321Verifier::CopySet set;
            set.pool = it.key();
            for (quint64 member : std::as_const(chainIds)) {
                const Job *copy = it.value().value(member, nullptr);
                if (!copy) {
                    set.missing.append(member);
                    continue;
                }
                set.covered.append(member);
                set.jobIds.append(copy->jobId);
                const QDateTime made = copy->reallyEnded.isValid() ? copy->reallyEnded : copy->ended;
                if (made.isValid() && (!set.written.isValid() || made > set.written))
                    set.written = made;
            }
            set.complete = set.missing.isEmpty();
            place(set, index, input);
            r.sets.append(set);
        }
    }

    // --- Copies -------------------------------------------------------------
    const int completeCopies = r.completeCopySets();
    r.copies = (input.countOriginalData ? 1 : 0) + (r.chain.isEmpty() ? 0 : 1) + completeCopies;
    r.copiesState = r.copies >= R::copiesRequired ? State::Met : State::Failed;

    // The chain and its complete copies are what counts
    QList<const B321Verifier::CopySet *> counted;
    for (const B321Verifier::CopySet &set : std::as_const(r.sets)) {
        if (set.complete) counted.append(&set);
    }

    // --- Media types --------------------------------------------------------
    // A copy on several media types does not make the data independent of
    // one of them: the count is the largest number of copies whose media
    // types do not overlap (copies with an unknown medium left out)
    bool unknownMedia = false;
    QStringList allStorages;
    QList<QStringList> knownTypeSets;
    for (const B321Verifier::CopySet *set : std::as_const(counted)) {
        for (const QString &type : set->mediaTypes) {
            if (!r.mediaTypeNames.contains(type)) r.mediaTypeNames.append(type);
        }
        for (const QString &storage : set->storages) {
            if (!allStorages.contains(storage)) allStorages.append(storage);
        }
        const bool unknown = setHasUnknownMedia(*set);
        unknownMedia = unknownMedia || unknown;
        if (!unknown && !set->mediaTypes.isEmpty()) knownTypeSets.append(set->mediaTypes);
    }
    r.mediaTypes = largestDisjointGroup(knownTypeSets);
    r.mediaState = r.mediaTypes >= R::mediaTypesRequired
        ? State::Met
        : ((unknownMedia && !r.chain.isEmpty()) ? State::Unknown : State::Failed);

    // --- Off site -----------------------------------------------------------
    bool unknownOffSite = false;
    for (const B321Verifier::CopySet *set : std::as_const(counted)) {
        if (set->offSite == OffSite::Yes) ++r.offSiteCopies;
        unknownOffSite = unknownOffSite || set->offSite == OffSite::Unknown;
    }
    r.offSiteState = r.offSiteCopies >= R::offSiteRequired
        ? State::Met
        : (unknownOffSite ? State::Unknown : State::Failed);

    // --- Verdict ------------------------------------------------------------
    const State states[] = {r.copiesState, r.mediaState, r.offSiteState};
    bool anyFailed = false;
    bool anyUnknown = false;
    for (State state : states) {
        anyFailed = anyFailed || state == State::Failed;
        anyUnknown = anyUnknown || state == State::Unknown;
    }
    r.verdict = anyFailed ? B321Verifier::Verdict::Fails
                          : (anyUnknown ? B321Verifier::Verdict::Unknown : B321Verifier::Verdict::Passes);

    // --- Words --------------------------------------------------------------
    const QList<const B321Verifier::CopySet *> partial = r.partialCopySets();

    if (r.chain.isEmpty()) {
        r.reasons.append(input.historyDays > 0
            ? B321Verifier::tr("No successful Full backup of %1 in the job history of the last %n day(s): "
                               "there is no chain to restore.", nullptr, input.historyDays).arg(definition.name)
            : B321Verifier::tr("No successful Full backup of %1 in the job history: "
                               "there is no chain to restore.").arg(definition.name));
        r.suggestions.append(B321Verifier::tr("Run a Full backup of %1 and check that it terminates "
                                              "normally.").arg(definition.name));
        r.summary = B321Verifier::tr("No successful Full backup in the job history");
        if (input.countOriginalData) {
            r.reasons.append(B321Verifier::tr("Only the original data on the client counts: 1 of 3 copies."));
        }
        return r;
    }

    // Copies
    {
        int diffs = 0;
        int incs = 0;
        for (const Job &member : std::as_const(r.chain)) {
            if (member.level == QLatin1Char('D')) ++diffs;
            if (member.level == QLatin1Char('I')) ++incs;
        }
        QString chainText = B321Verifier::tr("The chain a restore would use today is %1").arg(jobText(r.chain.first()));
        if (diffs) chainText += B321Verifier::tr(", a Differential");
        if (incs) chainText += B321Verifier::tr(" and %n Incremental(s) after it", nullptr, incs);
        chainText += B321Verifier::tr("; it counts as 1 copy.");
        r.reasons.append(chainText);
        if (input.countOriginalData) {
            r.reasons.append(B321Verifier::tr("The original data on the client counts as 1 copy "
                                              "(setting \"Count the original data as a copy\")."));
        } else {
            r.reasons.append(B321Verifier::tr("The original data on the client is not counted "
                                              "(setting \"Count the original data as a copy\" is off)."));
        }
        if (!r.migratedFrom.isEmpty()) {
            QStringList moved;
            for (auto it = r.migratedFrom.constBegin(); it != r.migratedFrom.constEnd(); ++it)
                moved.append(B321Verifier::tr("job %1 from job %2").arg(it.key()).arg(it.value().jobId));
            std::sort(moved.begin(), moved.end());
            r.reasons.append(B321Verifier::tr("Migrated chain jobs (%1): a Migrate moves the data and "
                                              "adds no copy; the old job is not restored any more.")
                                 .arg(moved.join(QStringLiteral(", "))));
        }
        for (const B321Verifier::CopySet &set : std::as_const(r.sets)) {
            if (set.original || !set.complete) continue;
            r.reasons.append(B321Verifier::tr("Pool %1 holds a copy of every job of the chain; it counts "
                                              "as 1 copy.").arg(poolLabel(set.pool)));
        }
        for (const B321Verifier::CopySet *set : partial) {
            r.reasons.append(B321Verifier::tr("Pool %1 holds copies of %2 of the %3 jobs of the chain "
                                              "(missing: %4); a partial copy does not count.")
                                 .arg(poolLabel(set->pool)).arg(set->covered.size())
                                 .arg(chainIds.size()).arg(joinedIds(set->missing)));
        }
        if (completeCopies == 0 && partial.isEmpty()) {
            r.reasons.append(B321Verifier::tr("No Copy job has copied the chain."));
        }
        r.reasons.append(B321Verifier::tr("Copies: %1 of 3.").arg(r.copies));
    }

    // Media
    if (r.mediaTypeNames.isEmpty()) {
        r.reasons.append(input.jobMediaKnown
            ? B321Verifier::tr("The volumes of the chain are not known, so neither are their media types.")
            : B321Verifier::tr("The volumes of the jobs (JobMedia) were not loaded: media types unknown."));
    } else {
        r.reasons.append(B321Verifier::tr("Media types of the chain and its complete copies: %1.")
                             .arg(r.mediaTypeNames.join(QStringLiteral(", "))));
        if (r.mediaTypeNames.size() > r.mediaTypes) {
            r.reasons.append(B321Verifier::tr("A copy that spans several media types counts once; copies on "
                                              "media types of their own: %1.").arg(r.mediaTypes));
        }
        if (allStorages.size() > r.mediaTypeNames.size()) {
            r.reasons.append(B321Verifier::tr("They lie on the storages %1; the same media type on "
                                              "several storages counts as one type.")
                                 .arg(allStorages.join(QStringLiteral(", "))));
        }
        if (unknownMedia) {
            r.reasons.append(B321Verifier::tr("Some volumes of these jobs are not in the volume list "
                                              "or have no media type."));
        }
    }

    // Off site
    QStringList unmarkedStorages;
    for (const B321Verifier::CopySet *set : std::as_const(counted)) {
        for (const B321Verifier::Placement &p : set->placements) {
            if (p.offSite == OffSite::Unknown && !p.storage.isEmpty()
                && !unmarkedStorages.contains(p.storage))
                unmarkedStorages.append(p.storage);
        }
    }
    switch (r.offSiteState) {
    case State::Met: {
        QStringList where;
        for (const B321Verifier::CopySet *set : std::as_const(counted)) {
            if (set->offSite != OffSite::Yes) continue;
            QStringList sources;
            for (const B321Verifier::Placement &p : set->placements) {
                const QString source = p.source == B321Verifier::OffSiteSource::Marking
                    ? B321Verifier::tr("storage %1 marked off site").arg(p.storage)
                    : B321Verifier::tr("volume %1 has location %2 in the catalog")
                          .arg(p.volume).arg(p.locationId.value_or(0));
                if (!sources.contains(source)) sources.append(source);
            }
            where.append(B321Verifier::tr("%1 (%2)")
                             .arg(set->original ? B321Verifier::tr("the chain")
                                                : B321Verifier::tr("the copy in pool %1").arg(poolLabel(set->pool)),
                                  sources.join(QStringLiteral("; "))));
        }
        r.reasons.append(B321Verifier::tr("Off site: %1.").arg(where.join(QStringLiteral(", "))));
        break;
    }
    case State::Unknown:
        r.reasons.append(B321Verifier::tr("Off site is unknown: no storage of the chain or its copies is "
                                          "marked off site and no volume has a location in the catalog. "
                                          "Bareos itself does not record whether a storage is off site."));
        break;
    case State::Failed:
        r.reasons.append(B321Verifier::tr("Nothing is off site: every copy has a volume on a storage "
                                          "that is on site."));
        break;
    }

    // Suggestions
    const QString mediaText = r.mediaTypeNames.isEmpty() ? B321Verifier::tr("the present one")
                                                        : r.mediaTypeNames.join(QStringLiteral(", "));
    for (const B321Verifier::CopySet *set : partial) {
        r.suggestions.append(B321Verifier::tr("Pool %1 lacks copies of the jobs %2: run its Copy job "
                                              "again (Selection Type = PoolUncopiedJobs copies what is "
                                              "missing) so that the whole chain is copied.")
                                 .arg(poolLabel(set->pool), joinedIds(set->missing)));
    }
    if (r.copiesState == State::Failed) {
        const int needed = R::copiesRequired - r.copies;
        if (r.mediaState != State::Met) {
            r.suggestions.append(B321Verifier::tr("Add a Copy job to a second storage with a different "
                                                  "media type than %1 (for example tape, a removable disk "
                                                  "or cloud storage with a Media Type of its own) "
                                                  "(needed: %2 more).").arg(mediaText).arg(needed));
        } else {
            r.suggestions.append(B321Verifier::tr("Add a Copy job to a second storage (needed: %1 more).")
                                     .arg(needed));
        }
    } else if (r.mediaState == State::Failed) {
        r.suggestions.append(B321Verifier::tr("Write one of the copies to a storage with another media "
                                              "type than %1.").arg(mediaText));
    }
    if (r.mediaState == State::Unknown) {
        r.suggestions.append(B321Verifier::tr("Refresh: the volumes of the jobs and the volume list are "
                                              "needed to tell the media types."));
    }
    if (r.offSiteState == State::Unknown) {
        for (const QString &storage : std::as_const(unmarkedStorages)) {
            r.suggestions.append(B321Verifier::tr("Mark storage %1 as off site if it is at another site; "
                                                  "if it is not, tick \"Unmarked storages are on site\".")
                                     .arg(storage));
        }
        if (unmarkedStorages.isEmpty()) {
            r.suggestions.append(B321Verifier::tr("Refresh: the storages of the volumes are needed to tell "
                                                  "where the copies are."));
        }
    } else if (r.offSiteState == State::Failed) {
        r.suggestions.append(B321Verifier::tr("Copy the chain to a storage at another site and mark that "
                                              "storage as off site (or take a volume off site and record "
                                              "its location)."));
    }

    // --- Hints ---------------------------------------------------------------
    // Age of the newest Full against the usual interval of this job's Fulls
    {
        const Job &newest = r.chain.first();
        const QDateTime end = newest.ended.isValid() ? newest.ended : newest.started;
        if (end.isValid() && input.now.isValid()) {
            const qint64 age = end.daysTo(input.now);
            std::sort(fulls.begin(), fulls.end(), [](const Job *a, const Job *b) { return earlier(*a, *b); });
            QList<qint64> intervals;
            for (qsizetype i = 1; i < fulls.size(); ++i) {
                if (fulls.at(i - 1)->started.isValid() && fulls.at(i)->started.isValid())
                    intervals.append(fulls.at(i - 1)->started.secsTo(fulls.at(i)->started));
            }
            if (!intervals.isEmpty()) {
                const qint64 usualDays = qMax<qint64>(1, (medianOf(intervals) + 43200) / 86400);
                QString hint = B321Verifier::tr("The newest Full (job %1) is %n day(s) old", nullptr, int(age))
                                   .arg(newest.jobId);
                hint += B321Verifier::tr("; this job's Fulls ran every %n day(s) (median of the loaded "
                                         "history).", nullptr, int(usualDays));
                if (age * 2 > usualDays * 3 && age > usualDays + 1)
                    hint += B321Verifier::tr(" It is overdue.");
                r.hints.append(hint);
            } else {
                r.hints.append(B321Verifier::tr("The newest Full (job %1) is %n day(s) old; it is the only "
                                                "successful Full in the loaded history.", nullptr, int(age))
                                   .arg(newest.jobId));
            }
        }
    }

    // Age of the newest copy of this job; copies of an earlier chain
    {
        const Job *newestCopy = nullptr;
        quint64 newestOriginal = 0;
        for (const Job &job : input.jobs) {
            if (job.type != QLatin1Char('C') || !job.isSuccessful()) continue;
            const quint64 originalId = originalOf(job, index);
            const Job *originalJob = index.byId.value(originalId, nullptr);
            const bool ours = originalJob ? originalJob->name == definition.name : job.name == definition.name;
            if (!ours) continue;
            if (!newestCopy || newestCopy->reallyEnded < job.reallyEnded
                || (newestCopy->reallyEnded == job.reallyEnded && newestCopy->jobId < job.jobId)) {
                newestCopy = &job;
                newestOriginal = originalId;
            }
        }
        if (newestCopy) {
            const QDateTime made = newestCopy->reallyEnded.isValid() ? newestCopy->reallyEnded
                                                                    : newestCopy->ended;
            if (made.isValid() && input.now.isValid()) {
                r.hints.append(B321Verifier::tr("The newest copy of this job (job %1 in pool %2, a copy of "
                                                "job %3) was made %n day(s) ago.", nullptr,
                                                int(made.daysTo(input.now)))
                                   .arg(newestCopy->jobId).arg(poolLabel(newestCopy->pool)).arg(newestOriginal));
            }
            bool poolComplete = false;
            for (const B321Verifier::CopySet &set : std::as_const(r.sets)) {
                poolComplete = poolComplete || (!set.original && set.complete && set.pool == newestCopy->pool);
            }
            if (!chainIds.contains(newestOriginal) && !poolComplete) {
                r.hints.append(B321Verifier::tr("That copy belongs to an earlier chain: the current chain "
                                                "(from job %1) has no complete copy in pool %2.")
                                   .arg(chainIds.first()).arg(poolLabel(newestCopy->pool)));
            }
        }
    }

    // Volumes in a state that endangers chain parts
    {
        QHash<quint64, const Job *> chainById;
        for (const Job &member : std::as_const(r.chain)) chainById.insert(member.jobId, &member);
        QStringList seen;
        for (const B321Verifier::CopySet &set : std::as_const(r.sets)) {
            for (qsizetype i = 0; i < set.jobIds.size(); ++i) {
                const quint64 id = set.jobIds.at(i);
                const quint64 memberId = set.original ? id : set.covered.value(i);
                const Job *member = chainById.value(memberId, nullptr);
                if (!input.jobMediaKnown) continue;
                for (const QString &name : input.jobMedia.value(id)) {
                    const BVolume *volume = index.volumes.value(name, nullptr);
                    if (!volume) continue;
                    const QString &status = volume->status;
                    if (status != QLatin1String("Error") && status != QLatin1String("Purged")
                        && status != QLatin1String("Recycle"))
                        continue;
                    const QString key = QString::number(id) + QLatin1Char('/') + name;
                    if (seen.contains(key)) continue;
                    seen.append(key);
                    const QString what = member
                        ? (set.original ? B321Verifier::tr("The chain's %1 (job %2)")
                                              .arg(B321Verifier::levelName(member->level)).arg(id)
                                        : B321Verifier::tr("The copy (job %1, pool %2) of the chain's %3")
                                              .arg(id).arg(poolLabel(set.pool), B321Verifier::levelName(member->level)))
                        : B321Verifier::tr("Job %1").arg(id);
                    if (status == QLatin1String("Error")) {
                        r.hints.append(B321Verifier::tr("%1 is on volume %2, which is in status Error.")
                                           .arg(what, name));
                        if (set.original && member && member->level == QLatin1Char('F')) {
                            r.suggestions.append(B321Verifier::tr("The chain's Full is on volume %1, which is "
                                                                  "in Error: run a new Full backup and check "
                                                                  "the volume.").arg(name));
                        }
                    } else {
                        r.hints.append(B321Verifier::tr("%1 is on volume %2, which is %3: Bareos may "
                                                        "overwrite it.").arg(what, name, status));
                    }
                }
            }
        }
    }

    if (input.jobsTruncated) {
        r.hints.append(B321Verifier::tr("The job list hit its row limit: older jobs and copies may be "
                                        "missing from this check."));
    }

    // Summary for the table
    QStringList parts;
    parts.append(B321Verifier::tr("%1 of 3 copies").arg(r.copies));
    if (!partial.isEmpty()) parts.append(B321Verifier::tr("partial copies: %1").arg(partial.size()));
    if (r.mediaTypeNames.isEmpty())
        parts.append(B321Verifier::tr("media types unknown"));
    else
        parts.append(B321Verifier::tr("media: %1 of 2 (%2)").arg(r.mediaTypes)
                         .arg(r.mediaTypeNames.join(QStringLiteral(", "))));
    switch (r.offSiteState) {
    case State::Met: parts.append(B321Verifier::tr("off site: %1 of 1").arg(r.offSiteCopies)); break;
    case State::Unknown: parts.append(B321Verifier::tr("off site unknown")); break;
    case State::Failed: parts.append(B321Verifier::tr("nothing off site")); break;
    }
    r.summary = parts.join(QStringLiteral(", "));
    return r;
}

} // namespace

int B321Verifier::Result::completeCopySets() const
{
    int count = 0;
    for (const CopySet &set : sets) {
        if (!set.original && set.complete) ++count;
    }
    return count;
}

QList<const B321Verifier::CopySet *> B321Verifier::Result::partialCopySets() const
{
    QList<const CopySet *> partial;
    for (const CopySet &set : sets) {
        if (!set.original && !set.complete) partial.append(&set);
    }
    return partial;
}

QList<B321Verifier::Result> B321Verifier::verify(const Input &input)
{
    Index index;
    for (const Job &job : input.jobs) index.byId.insert(job.jobId, &job);
    for (const Job &job : input.jobs) {
        if (job.type == QLatin1Char('C') && job.isSuccessful() && job.priorJobId != 0)
            index.copiesOf.insert(originalOf(job, index), &job);
    }
    for (const BVolume &volume : input.volumes) {
        if (!volume.name.isEmpty()) index.volumes.insert(volume.name, &volume);
    }

    QList<Definition> definitions = input.definitions;
    if (!input.definitionsKnown) {
        QStringList names;
        for (const Job &job : input.jobs) {
            if (job.type == QLatin1Char('B') && !job.name.isEmpty() && !names.contains(job.name))
                names.append(job.name);
        }
        std::sort(names.begin(), names.end(), [](const QString &a, const QString &b) {
            return a.compare(b, Qt::CaseInsensitive) < 0;
        });
        for (const QString &name : std::as_const(names)) {
            Definition definition;
            definition.name = name;
            definitions.append(definition);
        }
    }

    QList<Result> results;
    results.reserve(definitions.size());
    for (const Definition &definition : std::as_const(definitions))
        results.append(verifyJob(definition, input, index));
    return results;
}

QList<B321Verifier::Job> B321Verifier::parseJobs(const QByteArray &json, QString *error)
{
    QList<Job> jobs;
    QJsonObject result;
    if (!resultOf(json, &result, error)) return jobs;
    const QJsonValue rows = result.value(QLatin1String("jobs"));
    if (rows.isObject() && rows.toObject().isEmpty()) {
        if (error) error->clear();
        return jobs;    // Bareos answers so for an empty selection
    }
    if (!rows.isArray()) {
        if (error) *error = QStringLiteral("the reply has no list of jobs");
        return jobs;
    }
    const QJsonArray array = rows.toArray();
    jobs.reserve(array.size());
    for (const QJsonValue &value : array) {
        const QJsonObject row = value.toObject();
        Job job;
        job.jobId = idOf(row.value(QLatin1String("jobid")));
        if (job.jobId == 0) continue;
        job.name = textOf(row.value(QLatin1String("name")));
        job.type = codeOf(row.value(QLatin1String("type")));
        job.level = codeOf(row.value(QLatin1String("level")));
        job.status = codeOf(row.value(QLatin1String("jobstatus")));
        job.priorJobId = idOf(row.value(QLatin1String("priorjobid")));
        job.client = textOf(row.value(QLatin1String("client")));
        job.fileset = textOf(row.value(QLatin1String("fileset")));
        job.pool = textOf(row.value(QLatin1String("poolname")));
        job.started = parseTime(textOf(row.value(QLatin1String("starttime"))));
        job.ended = parseTime(textOf(row.value(QLatin1String("endtime"))));
        job.reallyEnded = parseTime(textOf(row.value(QLatin1String("realendtime"))));
        const QString bytes = textOf(row.value(QLatin1String("jobbytes")));
        bool ok = false;
        const qint64 n = bytes.toLongLong(&ok);
        if (ok) job.bytes = n;
        jobs.append(job);
    }
    if (error) error->clear();
    return jobs;
}

QHash<quint64, QStringList> B321Verifier::parseJobMedia(const QByteArray &json, QString *error)
{
    QHash<quint64, QStringList> media;
    QJsonObject result;
    if (!resultOf(json, &result, error)) return media;
    const QJsonValue rows = result.value(QLatin1String("jobmedia"));
    if (rows.isObject() && rows.toObject().isEmpty()) {
        if (error) error->clear();
        return media;
    }
    if (!rows.isArray()) {
        if (error) *error = QStringLiteral("the reply has no list of job media");
        return media;
    }
    const QJsonArray array = rows.toArray();
    for (const QJsonValue &value : array) {
        const QJsonObject row = value.toObject();
        const quint64 id = idOf(row.value(QLatin1String("jobid")));
        const QString volume = textOf(row.value(QLatin1String("volumename")));
        if (id == 0 || volume.isEmpty()) continue;
        QStringList &names = media[id];
        if (!names.contains(volume)) names.append(volume);
    }
    if (error) error->clear();
    return media;
}

QDateTime B321Verifier::parseTime(const QString &text)
{
    const QString trimmed = text.trimmed();
    if (trimmed.isEmpty() || trimmed == QLatin1String("0")) return QDateTime();
    // "2026-11-18 02:05:00", from PostgreSQL also with fractions of a second
    const QString head = trimmed.left(19);
    const QDate date = QDate::fromString(head.left(10), QStringLiteral("yyyy-MM-dd"));
    const QTime time = QTime::fromString(head.mid(11), QStringLiteral("HH:mm:ss"));
    if (!date.isValid() || !time.isValid() || head.size() != 19 || date.year() < 1971)
        return QDateTime();
    return QDateTime(date, time, QTimeZone::UTC);
}

QDateTime B321Verifier::wallClockNow()
{
    const QDateTime local = QDateTime::currentDateTime();
    return QDateTime(local.date(), local.time(), QTimeZone::UTC);
}

QString B321Verifier::stateText(State state)
{
    switch (state) {
    case State::Met: return tr("met");
    case State::Failed: return tr("not met");
    case State::Unknown: break;
    }
    return tr("unknown");
}

QString B321Verifier::verdictText(Verdict verdict)
{
    switch (verdict) {
    case Verdict::Passes: return tr("Passes");
    case Verdict::Fails: return tr("Fails");
    case Verdict::Unknown: break;
    }
    return tr("Unknown");
}

QString B321Verifier::levelName(QChar level)
{
    switch (level.unicode()) {
    case 'F': return tr("Full");
    case 'D': return tr("Differential");
    case 'I': return tr("Incremental");
    case 'V': return tr("Virtual Full");
    default: break;
    }
    return level.isNull() ? tr("Job") : QString(level);
}

QString B321Verifier::offSiteText(OffSite offSite)
{
    switch (offSite) {
    case OffSite::Yes: return tr("off site");
    case OffSite::No: return tr("on site");
    case OffSite::Unknown: break;
    }
    return tr("unknown");
}
