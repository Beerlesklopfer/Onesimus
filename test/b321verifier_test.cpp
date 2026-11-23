// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file b321verifier_test.cpp
 * @brief The 3-2-1 rule per backup job (B321Verifier), without widgets and
 *        without a Director
 *
 * - The worked example: the real volume list of the production Director
 *   (test/fixtures/pdc-llist-volumes-api2.json: one storage PDC-sd, every
 *   volume MediaType File, no locations) with the jobs and JobMedia records
 *   of test/fixtures/pdc-321-*.json (synthesized on those volumes, no Copy
 *   jobs). Every job: copies 2/3 (1/3 with the original data not counted),
 *   media 1/2, off site unknown, verdict fails, with suggestions.
 * - The chain: newest successful Full, newest Differential after it,
 *   Incrementals after the newer of both; failed jobs left out.
 * - Copies: a whole chain copied, a partial copy, copies of copies, failed
 *   copies, a Migrate (moves, adds no copy).
 * - Media types: the same type on two storages is one; a chain spanning two
 *   types without a copy counts once.
 * - Off site: the user's marking, a location, "unmarked are on site",
 *   unknown; a marked storage without a copy on it does not count.
 * - The setting "count the original data as a copy".
 * - Reading "llist jobs" and "list jobmedia" (strings, numbers, errors).
 *
 * TZ=UTC (set by CTest).
 */

#include <QtTest>

#include <QFile>

#include "storage/model/bvolumelist.h"
#include "tools/b321verifier.h"

namespace {

using V = B321Verifier;

const QDateTime kNow(QDate(2026, 11, 19), QTime(12, 0), QTimeZone::UTC);

QByteArray readFixture(const char *name)
{
    QFile file(QStringLiteral(FIXTURE_DIR "/") + QLatin1String(name));
    if (!file.open(QIODevice::ReadOnly)) return QByteArray();
    return file.readAll();
}

V::Job job(quint64 id, const char *name, char type, char level, const char *start,
           quint64 prior = 0, const char *pool = "Full", char status = 'T')
{
    V::Job j;
    j.jobId = id;
    j.name = QLatin1String(name);
    j.type = QLatin1Char(type);
    j.level = QLatin1Char(level);
    j.status = QLatin1Char(status);
    j.priorJobId = prior;
    j.client = QStringLiteral("srv-fd");
    j.fileset = QStringLiteral("SrvSet");
    j.pool = QLatin1String(pool);
    j.started = V::parseTime(QLatin1String(start));
    j.ended = j.started.addSecs(600);
    j.reallyEnded = j.ended;
    return j;
}

/// A copy made at @p made (Bareos gives it the start time of the original)
V::Job copyJob(quint64 id, const V::Job &original, const char *pool, const char *made,
               char status = 'T', char type = 'C')
{
    V::Job j = original;
    j.jobId = id;
    j.type = QLatin1Char(type);
    j.status = QLatin1Char(status);
    j.priorJobId = original.jobId;
    j.pool = QLatin1String(pool);
    j.reallyEnded = V::parseTime(QLatin1String(made));
    return j;
}

BVolume volume(const char *name, const char *mediaType, const char *storage,
               const char *status = "Append", qint64 location = 0)
{
    BVolume v;
    v.name = QLatin1String(name);
    v.mediaType = QLatin1String(mediaType);
    v.storage = QLatin1String(storage);
    v.status = QLatin1String(status);
    v.locationId = location;
    return v;
}

/// A server with a Full (1) and an Incremental (2) on File volumes of FileSD
V::Input baseInput()
{
    V::Input in;
    in.definitionsKnown = true;
    V::Definition d;
    d.name = QStringLiteral("Srv");
    in.definitions = {d};
    in.jobsKnown = true;
    in.jobs = {job(1, "Srv", 'B', 'F', "2026-11-01 23:00:00"),
               job(2, "Srv", 'B', 'I', "2026-11-02 23:00:00", 0, "Incremental")};
    in.jobMediaKnown = true;
    in.jobMedia = {{1, {QStringLiteral("File-1")}}, {2, {QStringLiteral("File-2")}}};
    in.volumesKnown = true;
    in.volumes = {volume("File-1", "File", "FileSD"), volume("File-2", "File", "FileSD"),
                  volume("Tape-1", "LTO-8", "Tape"), volume("Tape-2", "LTO-8", "Tape"),
                  volume("File2-1", "File", "FileSD2"), volume("File2-2", "File", "FileSD2")};
    in.now = kNow;
    return in;
}

/// Copies of both chain jobs into pool @p pool on volume @p volumeName
void addCopies(V::Input &in, quint64 firstId, const char *pool, const char *volumeName)
{
    const V::Job full = in.jobs.at(0);
    const V::Job inc = in.jobs.at(1);
    in.jobs.append(copyJob(firstId, full, pool, "2026-11-03 10:00:00"));
    in.jobs.append(copyJob(firstId + 1, inc, pool, "2026-11-03 10:10:00"));
    in.jobMedia.insert(firstId, {QLatin1String(volumeName)});
    in.jobMedia.insert(firstId + 1, {QLatin1String(volumeName)});
}

QList<quint64> chainIds(const V::Result &r)
{
    QList<quint64> ids;
    for (const V::Job &j : r.chain) ids.append(j.jobId);
    return ids;
}

bool anyContains(const QStringList &lines, const QString &part)
{
    for (const QString &line : lines) {
        if (line.contains(part)) return true;
    }
    return false;
}

V::Input pdcInput(const char *volumeFixture = "pdc-llist-volumes-api2.json")
{
    V::Input in;
    QString error;
    in.jobs = V::parseJobs(readFixture("pdc-321-llist-jobs-api2.json"), &error);
    in.jobsKnown = error.isEmpty();
    in.historyDays = 3650;
    in.jobMedia = V::parseJobMedia(readFixture("pdc-321-list-jobmedia-api2.json"), &error);
    in.jobMediaKnown = error.isEmpty();
    const BVolumeList volumes = BVolumeList::fromJson(readFixture(volumeFixture));
    in.volumes = volumes.volumes();
    in.volumesKnown = !in.volumes.isEmpty();
    in.definitionsKnown = true;
    for (const char *name : {"BGLAGE", "Catalog", "Cloud", "Forgejo", "Tapir"}) {
        V::Definition d;
        d.name = QLatin1String(name);
        in.definitions.append(d);
    }
    in.now = kNow;
    return in;
}

} // namespace

class B321VerifierTest : public QObject
{
    Q_OBJECT

private slots:
    void pdcFixtureVerdicts();
    void pdcSettingOff();
    void pdcShortVolumeList();
    void pdcChains();
    void pdcHints();
    void definitionsFromCatalog();

    void wholeChainCopied();
    void partialCopy();
    void copyOfCopyAndFailedCopy();
    void migrateMovesAndAddsNoCopy();

    void sameMediaTypeOnTwoStorages();
    void threeBackupsWithoutOriginal();

    void offSiteByMarking();
    void offSiteByLocation();
    void offSiteUnmarkedOnSite();
    void offSiteMarkWithoutCopyDoesNotCount();
    void offSitePartlyOnSite();

    void jobMediaNotLoaded();
    void differentialAndIncrementals();
    void copyOfEarlierChain();

    void parseJobsShapes();
    void parseJobMediaShapes();
};

void B321VerifierTest::pdcFixtureVerdicts()
{
    const V::Input in = pdcInput();
    QVERIFY(in.jobsKnown);
    QVERIFY(in.jobMediaKnown);
    QCOMPARE(in.volumes.size(), 23);

    const QList<V::Result> results = V::verify(in);
    QCOMPARE(results.size(), 5);
    for (const V::Result &r : results) {
        const QByteArray name = r.job.name.toUtf8();
        QVERIFY2(r.inCatalog, name.constData());
        QCOMPARE(r.copies, 2);                      // original data + 1 backup
        QCOMPARE(r.copiesState, V::State::Failed);
        QCOMPARE(r.mediaTypes, 1);
        QCOMPARE(r.mediaTypeNames, QStringList{QStringLiteral("File")});
        QCOMPARE(r.mediaState, V::State::Failed);
        QCOMPARE(r.offSiteCopies, 0);
        QCOMPARE(r.offSiteState, V::State::Unknown);
        QCOMPARE(r.verdict, V::Verdict::Fails);
        QCOMPARE(r.sets.size(), 1);                 // no Copy jobs
        QCOMPARE(r.sets.first().storages, QStringList{QStringLiteral("PDC-sd")});
        QVERIFY2(anyContains(r.reasons, QStringLiteral("No Copy job has copied the chain")), name.constData());
        QVERIFY2(anyContains(r.suggestions, QStringLiteral("Add a Copy job to a second storage with a "
                                                           "different media type than File")),
                 name.constData());
        QVERIFY2(anyContains(r.suggestions, QStringLiteral("Mark storage PDC-sd as off site")), name.constData());
        QVERIFY2(r.summary.contains(QStringLiteral("2 of 3 copies")), qPrintable(r.summary));
        QVERIFY2(r.summary.contains(QStringLiteral("off site unknown")), qPrintable(r.summary));
    }
}

void B321VerifierTest::pdcSettingOff()
{
    V::Input in = pdcInput();
    in.countOriginalData = false;
    const QList<V::Result> results = V::verify(in);
    QCOMPARE(results.size(), 5);
    for (const V::Result &r : results) {
        QCOMPARE(r.copies, 1);
        QCOMPARE(r.mediaTypes, 1);
        QCOMPARE(r.offSiteState, V::State::Unknown);
        QCOMPARE(r.verdict, V::Verdict::Fails);
        QVERIFY(anyContains(r.suggestions, QStringLiteral("(needed: 2 more)")));
    }
}

void B321VerifierTest::pdcShortVolumeList()
{
    // "list volumes" lacks the LocationId: off site stays unknown, the media type is there
    const V::Input in = pdcInput("pdc-list-volumes-api2.json");
    QCOMPARE(in.volumes.size(), 23);
    for (const V::Result &r : V::verify(in)) {
        QCOMPARE(r.copies, 2);
        QCOMPARE(r.mediaTypes, 1);
        QCOMPARE(r.offSiteState, V::State::Unknown);
        QCOMPARE(r.verdict, V::Verdict::Fails);
    }
}

void B321VerifierTest::pdcChains()
{
    QHash<QString, V::Result> byName;
    for (const V::Result &r : V::verify(pdcInput())) byName.insert(r.job.name, r);

    // Failed Incremental 156 left out
    QCOMPARE(chainIds(byName.value(QStringLiteral("Tapir"))), (QList<quint64>{101, 140, 151, 160}));
    // A Full with warnings ('W') is restorable
    QCOMPARE(chainIds(byName.value(QStringLiteral("Forgejo"))), (QList<quint64>{150, 158}));
    QCOMPARE(chainIds(byName.value(QStringLiteral("Catalog"))), (QList<quint64>{60, 120, 159}));
    QCOMPARE(chainIds(byName.value(QStringLiteral("Cloud"))), (QList<quint64>{122, 157}));
    QCOMPARE(chainIds(byName.value(QStringLiteral("BGLAGE"))), (QList<quint64>{135, 145, 155}));
    // A Full over three volumes: every volume is placed
    QCOMPARE(byName.value(QStringLiteral("BGLAGE")).sets.first().placements.size(), 3);
    QCOMPARE(byName.value(QStringLiteral("Tapir")).job.client, QStringLiteral("tapir-fd"));
}

void B321VerifierTest::pdcHints()
{
    QHash<QString, V::Result> byName;
    for (const V::Result &r : V::verify(pdcInput())) byName.insert(r.job.name, r);

    // The Full of Catalog lies on Vol-0005, which is in Error
    const V::Result catalog = byName.value(QStringLiteral("Catalog"));
    QVERIFY(anyContains(catalog.hints, QStringLiteral("is on volume Vol-0005, which is in status Error")));
    QVERIFY(anyContains(catalog.suggestions, QStringLiteral("The chain's Full is on volume Vol-0005")));
    // Fulls on 22 Jan and 8 Mar (45 days), the newest 256 days old: overdue
    QVERIFY(anyContains(catalog.hints, QStringLiteral("256 day(s) old")));
    QVERIFY(anyContains(catalog.hints, QStringLiteral("overdue")));
    // Forgejo: Fulls 14 and 7 days apart (median 10.5 -> 11 days), the newest 2 days old
    const V::Result forgejo = byName.value(QStringLiteral("Forgejo"));
    QVERIFY(anyContains(forgejo.hints, QStringLiteral("2 day(s) old")));
    QVERIFY(!anyContains(forgejo.hints, QStringLiteral("overdue")));
    // Tapir: the only Full
    QVERIFY(anyContains(byName.value(QStringLiteral("Tapir")).hints, QStringLiteral("only successful Full")));
    QVERIFY(byName.value(QStringLiteral("Tapir")).sets.first().placements.first().offSite == V::OffSite::Unknown);
}

void B321VerifierTest::definitionsFromCatalog()
{
    V::Input in = pdcInput();
    in.definitions.clear();
    in.definitionsKnown = false;
    QStringList names;
    for (const V::Result &r : V::verify(in)) names.append(r.job.name);
    // Backup job names of the catalog, sorted; no restore job
    QCOMPARE(names, (QStringList{"BGLAGE", "Catalog", "Cloud", "Forgejo", "Tapir"}));

    // A Job resource that never ran: no chain, fails
    in = baseInput();
    V::Definition never;
    never.name = QStringLiteral("Never");
    in.definitions = {never};
    const V::Result r = V::verify(in).first();
    QVERIFY(!r.inCatalog);
    QCOMPARE(r.copies, 1);  // the original data only
    QCOMPARE(r.verdict, V::Verdict::Fails);
    QVERIFY(anyContains(r.suggestions, QStringLiteral("Run a Full backup of Never")));
}

void B321VerifierTest::wholeChainCopied()
{
    V::Input in = baseInput();
    addCopies(in, 10, "TapeCopy", "Tape-1");
    const V::Result r = V::verify(in).first();
    QCOMPARE(r.sets.size(), 2);
    QVERIFY(r.sets.at(1).complete);
    QCOMPARE(r.sets.at(1).pool, QStringLiteral("TapeCopy"));
    QCOMPARE(r.sets.at(1).jobIds, (QList<quint64>{10, 11}));
    QCOMPARE(r.copies, 3);
    QCOMPARE(r.copiesState, V::State::Met);
    QCOMPARE(r.mediaTypes, 2);
    QCOMPARE(r.mediaState, V::State::Met);
    QCOMPARE(r.offSiteState, V::State::Unknown);
    QCOMPARE(r.verdict, V::Verdict::Unknown);
    QVERIFY(anyContains(r.suggestions, QStringLiteral("Mark storage Tape as off site")));
    QVERIFY(anyContains(r.suggestions, QStringLiteral("Mark storage FileSD as off site")));
}

void B321VerifierTest::partialCopy()
{
    V::Input in = baseInput();
    in.jobs.append(copyJob(10, in.jobs.at(0), "TapeCopy", "2026-11-03 10:00:00"));
    in.jobMedia.insert(10, {QStringLiteral("Tape-1")});
    const V::Result r = V::verify(in).first();
    QCOMPARE(r.sets.size(), 2);
    QVERIFY(!r.sets.at(1).complete);
    QCOMPARE(r.sets.at(1).missing, QList<quint64>{2});
    QCOMPARE(r.partialCopySets().size(), 1);
    QCOMPARE(r.copies, 2);
    // The partial copy's medium does not count
    QCOMPARE(r.mediaTypes, 1);
    QCOMPARE(r.verdict, V::Verdict::Fails);
    QVERIFY(anyContains(r.reasons, QStringLiteral("Pool TapeCopy holds copies of 1 of the 2 jobs")));
    QVERIFY(anyContains(r.suggestions, QStringLiteral("Pool TapeCopy lacks copies of the jobs 2")));
    QVERIFY(r.summary.contains(QStringLiteral("partial copies: 1")));
}

void B321VerifierTest::copyOfCopyAndFailedCopy()
{
    V::Input in = baseInput();
    addCopies(in, 10, "TapeCopy", "Tape-1");
    // Copies of the copies into a second pool count for the original
    in.jobs.append(copyJob(20, in.jobs.at(2), "Remote", "2026-11-04 10:00:00"));
    in.jobs.append(copyJob(21, in.jobs.at(3), "Remote", "2026-11-04 10:05:00"));
    in.jobMedia.insert(20, {QStringLiteral("File2-1")});
    in.jobMedia.insert(21, {QStringLiteral("File2-2")});
    // A failed copy into a third pool does not count
    in.jobs.append(copyJob(30, in.jobs.at(0), "Broken", "2026-11-05 10:00:00", 'E'));
    in.jobs.append(copyJob(31, in.jobs.at(1), "Broken", "2026-11-05 10:00:00", 'f'));
    // The control jobs of a Copy run hold no data
    V::Job control = job(40, "CopyToTape", 'c', ' ', "2026-11-03 10:00:00", 10);
    in.jobs.append(control);

    const V::Result r = V::verify(in).first();
    QCOMPARE(r.completeCopySets(), 2);
    QCOMPARE(r.partialCopySets().size(), 0);
    QCOMPARE(r.copies, 4);
    QCOMPARE(r.copiesState, V::State::Met);
}

void B321VerifierTest::migrateMovesAndAddsNoCopy()
{
    V::Input in = baseInput();
    // Full 1 migrated to tape: the old record becomes 'M', the new one is
    // a 'B' with PriorJobId 1 and the start time of the old one
    in.jobs[0].type = QLatin1Char('M');
    V::Job moved = copyJob(50, in.jobs.at(0), "TapePool", "2026-11-05 10:00:00", 'T', 'B');
    in.jobs.append(moved);
    V::Job control = job(51, "MigrateToTape", 'g', ' ', "2026-11-05 09:00:00", 50);
    in.jobs.append(control);
    in.jobMedia.insert(50, {QStringLiteral("Tape-1")});

    const V::Result r = V::verify(in).first();
    QCOMPARE(chainIds(r), (QList<quint64>{50, 2}));
    QVERIFY(r.migratedFrom.contains(50));
    QCOMPARE(r.migratedFrom.value(50).jobId, quint64(1));
    QCOMPARE(r.copies, 2);                  // a Migrate adds no copy
    QCOMPARE(r.completeCopySets(), 0);
    // The chain spans LTO-8 and File: one copy on two media counts once
    QCOMPARE(r.mediaTypeNames.size(), 2);
    QCOMPARE(r.mediaTypes, 1);
    QCOMPARE(r.mediaState, V::State::Failed);
    QVERIFY(anyContains(r.reasons, QStringLiteral("a Migrate moves the data and adds no copy")));
    QVERIFY(anyContains(r.reasons, QStringLiteral("A copy that spans several media types counts once")));
}

void B321VerifierTest::sameMediaTypeOnTwoStorages()
{
    V::Input in = baseInput();
    in.jobs.append(copyJob(10, in.jobs.at(0), "Copy2", "2026-11-03 10:00:00"));
    in.jobs.append(copyJob(11, in.jobs.at(1), "Copy2", "2026-11-03 10:00:00"));
    in.jobMedia.insert(10, {QStringLiteral("File2-1")});
    in.jobMedia.insert(11, {QStringLiteral("File2-2")});
    const V::Result r = V::verify(in).first();
    QCOMPARE(r.copies, 3);
    QCOMPARE(r.copiesState, V::State::Met);
    QCOMPARE(r.mediaTypes, 1);
    QCOMPARE(r.mediaState, V::State::Failed);
    QVERIFY(anyContains(r.reasons, QStringLiteral("the same media type on several storages counts as one type")));
    QVERIFY(anyContains(r.suggestions, QStringLiteral("Write one of the copies to a storage with another media type than File")));
}

void B321VerifierTest::threeBackupsWithoutOriginal()
{
    V::Input in = baseInput();
    in.countOriginalData = false;
    addCopies(in, 10, "TapeCopy", "Tape-1");
    V::Result r = V::verify(in).first();
    QCOMPARE(r.copies, 2);
    QCOMPARE(r.copiesState, V::State::Failed);
    QVERIFY(anyContains(r.suggestions, QStringLiteral("(needed: 1 more)")));
    addCopies(in, 20, "Remote", "File2-1");
    r = V::verify(in).first();
    QCOMPARE(r.copies, 3);
    QCOMPARE(r.copiesState, V::State::Met);
}

void B321VerifierTest::offSiteByMarking()
{
    V::Input in = baseInput();
    addCopies(in, 10, "TapeCopy", "Tape-1");
    in.offSiteStorages = {QStringLiteral("Tape")};
    const V::Result r = V::verify(in).first();
    QCOMPARE(r.sets.at(1).offSite, V::OffSite::Yes);
    QCOMPARE(r.sets.at(1).placements.first().source, V::OffSiteSource::Marking);
    QCOMPARE(r.offSiteCopies, 1);
    QCOMPARE(r.offSiteState, V::State::Met);
    QCOMPARE(r.verdict, V::Verdict::Passes);
    QVERIFY(anyContains(r.reasons, QStringLiteral("storage Tape marked off site")));
}

void B321VerifierTest::offSiteByLocation()
{
    V::Input in = baseInput();
    addCopies(in, 10, "TapeCopy", "Tape-1");
    in.volumes[2].locationId = 3;   // Tape-1 has a Location in the catalog
    const V::Result r = V::verify(in).first();
    QCOMPARE(r.sets.at(1).placements.first().source, V::OffSiteSource::Location);
    QCOMPARE(r.offSiteState, V::State::Met);
    QCOMPARE(r.verdict, V::Verdict::Passes);
    QVERIFY(anyContains(r.reasons, QStringLiteral("volume Tape-1 has location 3")));
}

void B321VerifierTest::offSiteUnmarkedOnSite()
{
    V::Input in = baseInput();
    addCopies(in, 10, "TapeCopy", "Tape-1");
    in.unmarkedAreOnSite = true;
    const V::Result r = V::verify(in).first();
    QCOMPARE(r.offSiteCopies, 0);
    QCOMPARE(r.offSiteState, V::State::Failed);
    QCOMPARE(r.verdict, V::Verdict::Fails);
    QVERIFY(anyContains(r.suggestions, QStringLiteral("Copy the chain to a storage at another site")));
}

void B321VerifierTest::offSiteMarkWithoutCopyDoesNotCount()
{
    V::Input in = baseInput();
    in.offSiteStorages = {QStringLiteral("Tape")};  // nothing of the chain is there
    const V::Result r = V::verify(in).first();
    QCOMPARE(r.offSiteCopies, 0);
    QCOMPARE(r.offSiteState, V::State::Unknown);
    in.unmarkedAreOnSite = true;
    QCOMPARE(V::verify(in).first().offSiteState, V::State::Failed);
}

void B321VerifierTest::offSitePartlyOnSite()
{
    // A copy with one volume off site and one on site is not off site
    V::Input in = baseInput();
    in.jobs.append(copyJob(10, in.jobs.at(0), "Mixed", "2026-11-03 10:00:00"));
    in.jobs.append(copyJob(11, in.jobs.at(1), "Mixed", "2026-11-03 10:00:00"));
    in.jobMedia.insert(10, {QStringLiteral("Tape-1")});
    in.jobMedia.insert(11, {QStringLiteral("File2-2")});
    in.offSiteStorages = {QStringLiteral("Tape")};
    in.unmarkedAreOnSite = true;
    const V::Result r = V::verify(in).first();
    QCOMPARE(r.sets.at(1).offSite, V::OffSite::No);
    QCOMPARE(r.offSiteState, V::State::Failed);
}

void B321VerifierTest::jobMediaNotLoaded()
{
    V::Input in = baseInput();
    addCopies(in, 10, "TapeCopy", "Tape-1");
    in.jobMediaKnown = false;
    in.jobMedia.clear();
    const V::Result r = V::verify(in).first();
    QCOMPARE(r.copies, 3);
    QCOMPARE(r.copiesState, V::State::Met);
    QCOMPARE(r.mediaState, V::State::Unknown);
    QCOMPARE(r.offSiteState, V::State::Unknown);
    QCOMPARE(r.verdict, V::Verdict::Unknown);
    QVERIFY(anyContains(r.reasons, QStringLiteral("JobMedia) were not loaded")));

    // A volume missing from the volume list makes its media type unknown
    in = baseInput();
    in.volumes.removeFirst();
    QCOMPARE(V::verify(in).first().mediaState, V::State::Unknown);
    in.volumes.clear();
    QCOMPARE(V::verify(in).first().mediaState, V::State::Unknown);
}

void B321VerifierTest::differentialAndIncrementals()
{
    V::Input in = baseInput();
    in.jobs = {job(1, "Srv", 'B', 'F', "2026-11-01 23:00:00"),
               job(2, "Srv", 'B', 'I', "2026-11-02 23:00:00"),
               job(3, "Srv", 'B', 'D', "2026-11-03 23:00:00"),
               job(4, "Srv", 'B', 'I', "2026-11-04 23:00:00"),
               job(5, "Srv", 'B', 'D', "2026-11-05 23:00:00"),
               job(6, "Srv", 'B', 'I', "2026-11-06 23:00:00"),
               job(7, "Srv", 'B', 'I', "2026-11-07 23:00:00", 0, "Full", 'A'),
               job(8, "Srv", 'B', 'F', "2026-11-08 23:00:00", 0, "Full", 'E'),
               // Another job name with the same client and FileSet: the restore takes it
               job(9, "SrvManual", 'B', 'I', "2026-11-09 10:00:00"),
               // An older Full
               job(0x10, "Srv", 'B', 'F', "2026-10-01 23:00:00")};
    const V::Result r = V::verify(in).first();
    QCOMPARE(chainIds(r), (QList<quint64>{1, 5, 6, 9}));
}

void B321VerifierTest::copyOfEarlierChain()
{
    V::Input in = baseInput();
    addCopies(in, 10, "TapeCopy", "Tape-1");
    // A new Full after the copies: the copies belong to the old chain
    in.jobs.append(job(3, "Srv", 'B', 'F', "2026-11-10 23:00:00"));
    in.jobMedia.insert(3, {QStringLiteral("File-1")});
    const V::Result r = V::verify(in).first();
    QCOMPARE(chainIds(r), QList<quint64>{3});
    QCOMPARE(r.completeCopySets(), 0);
    QCOMPARE(r.copies, 2);
    QVERIFY(anyContains(r.hints, QStringLiteral("newest copy of this job (job 11 in pool TapeCopy")));
    QVERIFY(anyContains(r.hints, QStringLiteral("belongs to an earlier chain")));
    // Fulls on 1 Nov and 10 Nov, the newest 9 days old: not overdue
    QVERIFY(anyContains(r.hints, QStringLiteral("9 day(s) old")));
    QVERIFY(!anyContains(r.hints, QStringLiteral("overdue")));
}

void B321VerifierTest::parseJobsShapes()
{
    // API mode 2: strings
    QString error;
    QList<V::Job> jobs = V::parseJobs(readFixture("pdc-321-llist-jobs-api2.json"), &error);
    QVERIFY(error.isEmpty());
    QCOMPARE(jobs.size(), 21);
    const V::Job &first = jobs.first();
    QCOMPARE(first.jobId, quint64(161));
    QCOMPARE(first.type, QLatin1Char('R'));

    // Typed: numbers
    jobs = V::parseJobs(R"({"result":{"jobs":[{"jobid":7,"name":"A","type":"C","level":"F",
        "jobstatus":"T","priorjobid":5,"client":"c","fileset":"f","poolname":"P",
        "starttime":"2026-11-01 10:00:00","endtime":"2026-11-01 11:00:00",
        "realendtime":"2026-11-02 09:00:00.123","jobbytes":1234}]}})", &error);
    QVERIFY(error.isEmpty());
    QCOMPARE(jobs.size(), 1);
    QCOMPARE(jobs.first().priorJobId, quint64(5));
    QCOMPARE(jobs.first().pool, QStringLiteral("P"));
    QCOMPARE(jobs.first().reallyEnded, QDateTime(QDate(2026, 11, 2), QTime(9, 0), QTimeZone::UTC));
    QCOMPARE(jobs.first().bytes, std::optional<qint64>(1234));

    // Nothing selected: an empty object
    jobs = V::parseJobs(R"({"result":{"jobs":{}}})", &error);
    QVERIFY(error.isEmpty());
    QVERIFY(jobs.isEmpty());

    // Refused
    jobs = V::parseJobs(R"({"error":{"message":"failed","data":{"messages":{"error":["llist: is an invalid command"]}}}})",
                        &error);
    QVERIFY(jobs.isEmpty());
    QCOMPARE(error, QStringLiteral("llist: is an invalid command"));

    // Plain text
    V::parseJobs("You have no messages.", &error);
    QCOMPARE(error, QStringLiteral("You have no messages."));
}

void B321VerifierTest::parseJobMediaShapes()
{
    QString error;
    QHash<quint64, QStringList> media = V::parseJobMedia(readFixture("pdc-321-list-jobmedia-api2.json"), &error);
    QVERIFY(error.isEmpty());
    QCOMPARE(media.value(135), (QStringList{"BGLAGE-0010", "BGLAGE-0011", "BGLAGE-0012"}));
    QVERIFY(!media.contains(156));  // the failed job wrote nothing

    // llist form and typed numbers; a volume twice is kept once
    media = V::parseJobMedia(R"({"result":{"jobmedia":[
        {"jobmediaid":"1","jobid":"3","mediaid":"9","volumename":"A"},
        {"jobmediaid":"2","jobid":3,"mediaid":9,"volumename":"A"},
        {"jobmediaid":"3","jobid":"3","mediaid":"10","volumename":"B"}]}})", &error);
    QVERIFY(error.isEmpty());
    QCOMPARE(media.value(3), (QStringList{"A", "B"}));

    media = V::parseJobMedia(R"({"result":{}})", &error);
    QVERIFY(!error.isEmpty());
}

QTEST_GUILESS_MAIN(B321VerifierTest)
#include "b321verifier_test.moc"
