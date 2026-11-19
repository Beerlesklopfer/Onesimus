/**
 * @file bvolumelist_test.cpp
 * @brief Test for BVolumeList: the volumes of "list volumes" / "llist volumes"
 *
 * - The real API mode 2 reply of "list volumes" of the production Director
 *   (PDC, 19.11.2026), stripped of the console lines:
 *   test/fixtures/pdc-list-volumes-api2.json. It is the short listing: no
 *   pool field, no maxvolbytes, no voluseduration, no firstwritten.
 * - The real API mode 2 reply of "llist volumes" of the same Director, the
 *   same day: test/fixtures/pdc-llist-volumes-api2.json. Grouped by pool like
 *   the short list (empty pools included), with a "pool" field per volume
 *   and the columns of list_volumes_select_long_0; no actiononpurge.
 * - "llist volumes" with made-up values for the cases the real reply lacks
 *   (NULL and zero dates, empty strings, a use duration).
 * - Flat form, typed gRPC form, missing and null fields.
 *
 * Runs with TZ=UTC (set by CTest), so that catalog times are fixed.
 */

#include <QtTest>

#include <QFile>
#include <QJsonDocument>

#include "storage/model/bvolumelist.h"

#ifndef STORAGE_FIXTURE_DIR
#error "STORAGE_FIXTURE_DIR must point to test/fixtures"
#endif

namespace {

const QStringList kPdcPools = {"Tapir", "Scratch", "Incremental", "Full", "Forgejo",
                               "Differential", "Default", "CLOUD", "BGLAGE"};

// "llist volumes", grouped by pool like ua_output.cc ListMedia writes it.
// One group per kind of case: an Error volume with all long columns, an
// Append volume without use duration, one with a use duration and NULL
// columns, and an empty pool.
const char *kLlistGrouped = R"({
  "jsonrpc": "2.0", "id": null,
  "result": {
    "volumes": {
      "cloud": [
        {"mediaid": "17", "volumename": "CLOUD-0017", "slot": "0", "poolid": "3", "pool": "CLOUD",
         "mediatype": "File", "firstwritten": "2026-03-02 04:16:20", "lastwritten": "2026-03-09 01:39:04",
         "labeldate": "2026-03-02 04:16:20", "voljobs": "9", "volfiles": "11", "volblocks": "794183",
         "volmounts": "3", "volbytes": "51233279615", "volerrors": "1", "volwrites": "794190",
         "volcapacitybytes": "0", "volstatus": "Error", "enabled": "1", "recycle": "1",
         "volretention": "31536000", "voluseduration": "0", "maxvoljobs": "0", "maxvolfiles": "0",
         "maxvolbytes": "53687091200", "inchanger": "0", "endfile": "11", "endblock": "3053472",
         "labeltype": "0", "storageid": "1", "deviceid": "0", "locationid": "0", "recyclecount": "2",
         "initialwrite": "", "scratchpoolid": "0", "scratchpool": "", "recyclepoolid": "0",
         "recyclepool": "", "hasencryptionkey": "0", "comment": "", "storage": "PDC-sd"}
      ],
      "bglage": [
        {"mediaid": "12", "volumename": "BGLAGE-0012", "poolid": "5", "pool": "BGLAGE",
         "mediatype": "File", "firstwritten": "2026-11-04 23:23:01", "lastwritten": "2026-11-18 23:06:16",
         "labeldate": "2026-11-04 23:23:01", "voljobs": "14", "volbytes": "49341890717",
         "volerrors": "0", "volstatus": "Append", "enabled": "1", "recycle": "1",
         "volretention": "2592000", "voluseduration": "0", "maxvoljobs": "0",
         "maxvolbytes": "53687091200", "recyclecount": "0", "storage": "PDC-sd"}
      ],
      "tapir": [
        {"mediaid": "21", "volumename": "Tapir-0021", "poolid": "7", "pool": "Tapir",
         "mediatype": "File", "firstwritten": null, "lastwritten": "0000-00-00 00:00:00",
         "labeldate": "", "voljobs": "", "volbytes": "14009488", "volstatus": "Append",
         "enabled": "1", "recycle": "0", "volretention": "15552000", "voluseduration": "604800",
         "maxvolbytes": "0", "storage": "PDC-sd"}
      ],
      "scratch": []
    }
  }
})";

const char *kFlatShort = R"({"result": {"volumes": [
  {"mediaid": "1", "volumename": "Vol-0001", "volstatus": "Error", "enabled": "1",
   "volbytes": "3127776477", "volretention": "31536000", "recycle": "1",
   "lastwritten": "2026-01-24 02:00:06", "storage": "PDC-sd"}
]}})";

// The typed form of the Onesimus Director (protobuf JSON leaves out zeros)
const char *kTypedShort = R"({"result": {"pools": {
  "full": {},
  "default": {"volumes": [
    {"mediaid": 1, "volumename": "Vol-0001", "mediatype": "File", "volbytes": 3127776477,
     "volstatus": 6, "enabled": 1, "recycle": true, "volretention": 31536000, "storage": "PDC-sd"}]}
}}})";

const char *kTypedLong = R"({"result": {"pools": {
  "cloud": {"volumes": [
    {"mediaid": 24, "volumename": "CLOUD-0024", "poolid": 3, "pool": "CLOUD", "volbytes": 53686358248,
     "volstatus": 2, "enabled": 1, "recycle": true, "volretention": 31536000, "voljobs": 12,
     "maxvolbytes": 53687091200, "lastwritten": "2026-11-02 01:06:57",
     "firstwritten": "2026-10-20T22:00:00", "storage": "PDC-sd"}]}
}}})";

QByteArray readFixture(const QString &name)
{
    QFile file(QStringLiteral(STORAGE_FIXTURE_DIR "/") + name);
    if (!file.open(QIODevice::ReadOnly)) {
        return QByteArray();
    }
    return file.readAll();
}

const BVolume *find(const BVolumeList &list, const QString &name)
{
    for (const BVolume &v : list.volumes()) {
        if (v.name == name) {
            return &v;
        }
    }
    return nullptr;
}

} // namespace

class BVolumeListTest : public QObject
{
    Q_OBJECT

private slots:
    void pdcFixtureCounts();
    void pdcFixtureFieldsAndUnknowns();
    void pdcFixturePools();
    void pdcLlistFixture();
    void llistGroupedAllFields();
    void llistMissingAndNullFields();
    void flatForm();
    void typedShortDefaults();
    void typedLongDefaults();
    void brokenReplies();
    void statusLetters();
    void dateParsing();
};

void BVolumeListTest::pdcFixtureCounts()
{
    const QByteArray json = readFixture(QStringLiteral("pdc-list-volumes-api2.json"));
    QVERIFY2(!json.isEmpty(), "fixture missing");
    QString error;
    const BVolumeList list = BVolumeList::fromJson(json, kPdcPools, &error);
    QVERIFY2(error.isEmpty(), qPrintable(error));

    // Counted in the file: 23 volumes, 12 Full, 5 Append, 4 Error, 2 Purged
    QCOMPARE(list.size(), 23);
    QCOMPARE(list.countWithStatus(QStringLiteral("Full")), 12);
    QCOMPARE(list.countWithStatus(QStringLiteral("Append")), 5);
    QCOMPARE(list.countWithStatus(QStringLiteral("Error")), 4);
    QCOMPARE(list.countWithStatus(QStringLiteral("Purged")), 2);
    QCOMPARE(list.countWithStatus(QStringLiteral("Used")), 0);
    QVERIFY(!list.hasLongForm());

    QStringList errors;
    for (const BVolume &v : list.volumes()) {
        if (v.status == QLatin1String("Error")) {
            errors << v.name;
        }
    }
    errors.sort();
    QCOMPARE(errors, (QStringList{"CLOUD-0004", "CLOUD-0017", "Vol-0001", "Vol-0005"}));
}

void BVolumeListTest::pdcFixtureFieldsAndUnknowns()
{
    const BVolumeList list =
        BVolumeList::fromJson(readFixture(QStringLiteral("pdc-list-volumes-api2.json")), kPdcPools);
    const BVolume *v = find(list, QStringLiteral("CLOUD-0017"));
    QVERIFY(v);
    QCOMPARE(v->pool, QStringLiteral("CLOUD"));       // group key "cloud" -> .pools spelling
    QCOMPARE(v->status, QStringLiteral("Error"));
    QCOMPARE(v->volBytes.value_or(-1), 51233279615LL);
    QCOMPARE(v->volRetention.value_or(-1), 31536000LL);
    QCOMPARE(v->recycle.value_or(false), true);
    QCOMPARE(v->enabled.value_or(-1), 1);
    QCOMPARE(v->storage, QStringLiteral("PDC-sd"));
    QCOMPARE(v->mediaId.value_or(-1), 17LL);
    QCOMPARE(v->lastWritten, QDateTime(QDate(2026, 3, 9), QTime(1, 39, 4)));

    // Not in the short listing: unknown, never 0
    QVERIFY(!v->maxVolBytes.has_value());
    QVERIFY(!v->volJobs.has_value());
    QVERIFY(!v->volUseDuration.has_value());
    QVERIFY(!v->volErrors.has_value());
    QVERIFY(!v->actionOnPurge.has_value());
    QVERIFY(!v->firstWritten.isValid());
    QVERIFY(!v->longForm);

    const BVolume *purged = find(list, QStringLiteral("BGLAGE-0019"));
    QVERIFY(purged);
    QCOMPARE(purged->pool, QStringLiteral("BGLAGE"));
    QCOMPARE(purged->status, QStringLiteral("Purged"));
    QCOMPARE(purged->volBytes.value_or(-1), 53686042829LL);
}

void BVolumeListTest::pdcFixturePools()
{
    const QByteArray json = readFixture(QStringLiteral("pdc-list-volumes-api2.json"));
    // With .pools: nine pools, the four empty ones included
    const BVolumeList list = BVolumeList::fromJson(json, kPdcPools);
    QCOMPARE(list.pools(), (QStringList{"BGLAGE", "CLOUD", "Default", "Differential", "Forgejo",
                                        "Full", "Incremental", "Scratch", "Tapir"}));
    QCOMPARE(list.countInPool(QStringLiteral("CLOUD")), 12);
    QCOMPARE(list.countInPool(QStringLiteral("BGLAGE")), 6);
    QCOMPARE(list.countInPool(QStringLiteral("Default")), 3);
    QCOMPARE(list.countInPool(QStringLiteral("Scratch")), 0);

    // Without .pools the empty groups of the reply still name the pools
    const BVolumeList bare = BVolumeList::fromJson(json);
    QCOMPARE(bare.pools().size(), 9);
    QVERIFY(bare.pools().contains(QStringLiteral("scratch")));
}

void BVolumeListTest::pdcLlistFixture()
{
    const QByteArray json = readFixture(QStringLiteral("pdc-llist-volumes-api2.json"));
    QVERIFY2(!json.isEmpty(), "fixture missing");

    // The reply is grouped, not flat
    const QJsonObject result = QJsonDocument::fromJson(json).object().value("result").toObject();
    QVERIFY(result.value("volumes").isObject());

    QString error;
    const BVolumeList list = BVolumeList::fromJson(json, kPdcPools, &error);
    QVERIFY2(error.isEmpty(), qPrintable(error));
    QCOMPARE(list.size(), 23);
    QCOMPARE(list.countWithStatus(QStringLiteral("Full")), 12);
    QCOMPARE(list.countWithStatus(QStringLiteral("Append")), 5);
    QCOMPARE(list.countWithStatus(QStringLiteral("Error")), 4);
    QCOMPARE(list.countWithStatus(QStringLiteral("Purged")), 2);
    QVERIFY(list.hasLongForm());

    // All nine pools, the four empty groups included, also without .pools
    const QStringList all = {"BGLAGE", "CLOUD", "Default", "Differential", "Forgejo",
                             "Full", "Incremental", "Scratch", "Tapir"};
    QCOMPARE(list.pools(), all);
    QCOMPARE(BVolumeList::fromJson(json).pools().size(), 9);
    QCOMPARE(list.countInPool(QStringLiteral("CLOUD")), 12);
    QCOMPARE(list.countInPool(QStringLiteral("BGLAGE")), 6);
    QCOMPARE(list.countInPool(QStringLiteral("Scratch")), 0);

    for (const BVolume &v : list.volumes()) {
        QVERIFY2(v.longForm, qPrintable(v.name));
        QVERIFY2(!v.pool.isEmpty() && all.contains(v.pool), qPrintable(v.name + " " + v.pool));
        QVERIFY2(v.volJobs.has_value(), qPrintable(v.name));
        QCOMPARE(v.maxVolBytes.value_or(-1), 53687091200LL);     // 50G on every pool
        QCOMPARE(v.maxVolJobs.value_or(-1), 0LL);
        QCOMPARE(v.volUseDuration.value_or(-1), 0LL);            // no use duration on PDC
        QVERIFY2(v.firstWritten.isValid(), qPrintable(v.name));
        QVERIFY2(v.labelDate.isValid(), qPrintable(v.name));
        QVERIFY(v.recycle.value_or(false));
        QCOMPARE(v.enabled.value_or(-1), 1);
        QVERIFY(!v.actionOnPurge.has_value());                   // not a column of llist
        QCOMPARE(v.storage, QStringLiteral("PDC-sd"));
    }

    const BVolume *v = find(list, QStringLiteral("CLOUD-0017"));
    QVERIFY(v);
    QCOMPARE(v->status, QStringLiteral("Error"));
    QCOMPARE(v->volJobs.value_or(-1), 16LL);
    QCOMPARE(v->volBytes.value_or(-1), 51233279615LL);
    QCOMPARE(v->firstWritten, QDateTime(QDate(2026, 3, 1), QTime(23, 7, 27)));
    QCOMPARE(v->lastWritten, QDateTime(QDate(2026, 3, 9), QTime(1, 39, 4)));
    QVERIFY(v->recyclePool.isEmpty());

    const BVolume *empty = find(list, QStringLiteral("BGLAGE-0013"));
    QVERIFY(empty);
    QCOMPARE(empty->volJobs.value_or(-1), 0LL);                  // known 0, not unknown
    QCOMPARE(find(list, QStringLiteral("Vol-0005"))->volJobs.value_or(-1), 130LL);

    // Same volumes, same statuses as the short list of the same day
    const BVolumeList shortList =
        BVolumeList::fromJson(readFixture(QStringLiteral("pdc-list-volumes-api2.json")), kPdcPools);
    for (const BVolume &s : shortList.volumes()) {
        const BVolume *l = find(list, s.name);
        QVERIFY2(l, qPrintable(s.name));
        QCOMPARE(l->status, s.status);
        QCOMPARE(l->pool, s.pool);
    }
}

void BVolumeListTest::llistGroupedAllFields()
{
    QString error;
    const BVolumeList list = BVolumeList::fromJson(kLlistGrouped, kPdcPools, &error);
    QVERIFY(error.isEmpty());
    QCOMPARE(list.size(), 3);
    QVERIFY(list.hasLongForm());
    QVERIFY(list.pools().contains(QStringLiteral("Scratch")));

    const BVolume *v = find(list, QStringLiteral("CLOUD-0017"));
    QVERIFY(v);
    QVERIFY(v->longForm);
    QCOMPARE(v->pool, QStringLiteral("CLOUD"));
    QCOMPARE(v->maxVolBytes.value_or(-1), 53687091200LL);   // 50G
    QCOMPARE(v->volJobs.value_or(-1), 9LL);
    QCOMPARE(v->maxVolJobs.value_or(-1), 0LL);
    QCOMPARE(v->volErrors.value_or(-1), 1LL);
    QCOMPARE(v->volUseDuration.value_or(-1), 0LL);
    QCOMPARE(v->recycleCount.value_or(-1), 2LL);
    QCOMPARE(v->firstWritten, QDateTime(QDate(2026, 3, 2), QTime(4, 16, 20)));
    QCOMPARE(v->labelDate, QDateTime(QDate(2026, 3, 2), QTime(4, 16, 20)));
    QVERIFY(v->recyclePool.isEmpty());
    // Bareos 25 llist has no ActionOnPurge column
    QVERIFY(!v->actionOnPurge.has_value());
}

void BVolumeListTest::llistMissingAndNullFields()
{
    const BVolumeList list = BVolumeList::fromJson(kLlistGrouped, kPdcPools);
    const BVolume *v = find(list, QStringLiteral("Tapir-0021"));
    QVERIFY(v);
    QVERIFY(!v->firstWritten.isValid());         // null
    QVERIFY(!v->lastWritten.isValid());          // zero date
    QVERIFY(!v->labelDate.isValid());            // empty string
    QVERIFY(!v->volJobs.has_value());            // empty string: unknown
    QVERIFY(!v->maxVolJobs.has_value());         // key missing: unknown
    QCOMPARE(v->maxVolBytes.value_or(-1), 0LL);  // 0 = no limit, known
    QCOMPARE(v->recycle.value_or(true), false);
    QCOMPARE(v->volUseDuration.value_or(-1), 604800LL);
}

void BVolumeListTest::flatForm()
{
    const BVolumeList list = BVolumeList::fromJson(kFlatShort, kPdcPools);
    QCOMPARE(list.size(), 1);
    const BVolume &v = list.volumes().first();
    QCOMPARE(v.name, QStringLiteral("Vol-0001"));
    QVERIFY(v.pool.isEmpty());                    // the flat short list has no pool
    QVERIFY(!v.volFiles.has_value());             // key missing in a string reply: unknown
    QCOMPARE(list.pools().size(), 9);             // the configured pools all the same
}

void BVolumeListTest::typedShortDefaults()
{
    const BVolumeList list = BVolumeList::fromJson(kTypedShort, kPdcPools);
    QCOMPARE(list.size(), 1);
    const BVolume &v = list.volumes().first();
    QCOMPARE(v.pool, QStringLiteral("Default"));
    QCOMPARE(v.status, QStringLiteral("Error"));  // 6
    QCOMPARE(v.volFiles.value_or(-1), 0LL);       // short column left out by protobuf: 0
    QVERIFY(!v.maxVolBytes.has_value());          // long column, short record: unknown
    QVERIFY(!v.longForm);
    QVERIFY(list.pools().contains(QStringLiteral("Full")));
}

void BVolumeListTest::typedLongDefaults()
{
    const BVolumeList list = BVolumeList::fromJson(kTypedLong, kPdcPools);
    QCOMPARE(list.size(), 1);
    const BVolume &v = list.volumes().first();
    QVERIFY(v.longForm);
    QCOMPARE(v.status, QStringLiteral("Full"));
    QCOMPARE(v.volJobs.value_or(-1), 12LL);
    QCOMPARE(v.maxVolJobs.value_or(-1), 0LL);     // left out by protobuf: 0
    QCOMPARE(v.volUseDuration.value_or(-1), 0LL);
    QCOMPARE(v.maxVolBytes.value_or(-1), 53687091200LL);
    QCOMPARE(v.firstWritten, QDateTime(QDate(2026, 10, 20), QTime(22, 0)));
}

void BVolumeListTest::brokenReplies()
{
    QString error;
    QVERIFY(BVolumeList::fromJson("not json", {}, &error).isEmpty());
    QVERIFY(!error.isEmpty());
    QVERIFY(BVolumeList::fromJson("[1,2]", {}, &error).isEmpty());
    QVERIFY(!error.isEmpty());
    const BVolumeList none = BVolumeList::fromJson(R"({"result": {"jobs": []}})", kPdcPools, &error);
    QVERIFY(none.isEmpty());
    QVERIFY(!error.isEmpty());
    QCOMPARE(none.pools().size(), 9);
    // A volume without anything is kept, with everything unknown
    const BVolume empty = BVolumeList::volumeFromJson(QJsonObject());
    QVERIFY(empty.status.isEmpty());
    QVERIFY(!empty.volBytes.has_value());
    QVERIFY(!empty.recycle.has_value());
    QVERIFY(!empty.enabled.has_value());
}

void BVolumeListTest::statusLetters()
{
    const QStringList statuses = BVolumeList::allStatuses();
    QCOMPARE(statuses.size(), 12);
    QSet<QString> letters;
    for (const QString &status : statuses) {
        const QString letter = BVolumeList::statusLetter(status);
        QCOMPARE(letter.size(), 1);
        QVERIFY2(letter != QLatin1String("?"), qPrintable(status));
        letters.insert(letter);
    }
    QCOMPARE(letters.size(), 12);                 // one letter per status
    QCOMPARE(BVolumeList::statusLetter(QStringLiteral("Append")), QStringLiteral("A"));
    QCOMPARE(BVolumeList::statusLetter(QStringLiteral("Error")), QStringLiteral("E"));
    QCOMPARE(BVolumeList::statusLetter(QStringLiteral("read-only")), QStringLiteral("O"));
    QCOMPARE(BVolumeList::statusLetter(QStringLiteral("Whatever")), QStringLiteral("?"));
}

void BVolumeListTest::dateParsing()
{
    QCOMPARE(BVolumeList::parseDateTime(QJsonValue(QStringLiteral("2026-03-09 01:39:04"))),
             QDateTime(QDate(2026, 3, 9), QTime(1, 39, 4)));
    QCOMPARE(BVolumeList::parseDateTime(QJsonValue(QStringLiteral("2026-03-09T01:39:04"))),
             QDateTime(QDate(2026, 3, 9), QTime(1, 39, 4)));
    QCOMPARE(BVolumeList::parseDateTime(QJsonValue(1773020344.0)),
             QDateTime::fromSecsSinceEpoch(1773020344));
    QVERIFY(!BVolumeList::parseDateTime(QJsonValue(QStringLiteral(""))).isValid());
    QVERIFY(!BVolumeList::parseDateTime(QJsonValue(QJsonValue::Null)).isValid());
    QVERIFY(!BVolumeList::parseDateTime(QJsonValue(QStringLiteral("0000-00-00 00:00:00"))).isValid());
    QVERIFY(!BVolumeList::parseDateTime(QJsonValue(0.0)).isValid());
    QVERIFY(!BVolumeList::parseDateTime(QJsonValue(QStringLiteral("yesterday"))).isValid());
}

QTEST_GUILESS_MAIN(BVolumeListTest)
#include "bvolumelist_test.moc"
