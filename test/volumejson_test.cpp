/**
 * @file volumejson_test.cpp
 * @brief Test for BVolumeJson::normalizeVolumes (both "list volumes" reply shapes)
 *
 * Replies captured from a Bareos 25.1.2 Director in API mode 2:
 *  - "list volumes"      -> result.volumes = { "<lowercase pool>": [ ... ], ... }
 *  - "list volumes all"  -> result.volumes = [ ... ]
 *
 * Exit code 0 on success, 1 on failure.
 */

#include "models/bvolumejson.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>

static int g_failures = 0;

static void check(bool condition, const QString &what)
{
    QTextStream out(stdout);
    out << (condition ? "PASS  " : "FAIL  ") << what << Qt::endl;
    if (!condition) {
        ++g_failures;
    }
}

static QJsonValue volumesOf(const char *json)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(QByteArray(json), &error);
    if (error.error != QJsonParseError::NoError) {
        QTextStream(stdout) << "JSON parse error: " << error.errorString() << Qt::endl;
        return QJsonValue();
    }
    return doc.object()["result"].toObject()["volumes"];
}

static const char *kGroupedReply = R"({
  "jsonrpc": "2.0", "id": null,
  "result": {
    "volumes": {
      "incremental": [
        {"mediaid": "47322", "volumename": "Incremental-47322", "volstatus": "Append",
         "enabled": "1", "volbytes": "366260149", "volfiles": "0", "volretention": "2592000",
         "recycle": "1", "slot": "0", "inchanger": "0", "mediatype": "File",
         "lastwritten": "2026-07-09 21:00:03", "storage": "File", "hasencryptionkey": "0"}
      ],
      "full": [
        {"mediaid": "47318", "volumename": "Full-0001", "volstatus": "Error",
         "enabled": "1", "volbytes": "23902210671", "volfiles": "5", "volretention": "0",
         "recycle": "1", "slot": "0", "inchanger": "0", "mediatype": "File",
         "lastwritten": "2026-03-30 22:54:15", "storage": "File", "hasencryptionkey": "0"},
        {"mediaid": "47321", "volumename": "Full-47321", "volstatus": "Append",
         "enabled": "1", "volbytes": "158201165", "volfiles": "0", "volretention": "31536000",
         "recycle": "1", "slot": "0", "inchanger": "0", "mediatype": "File",
         "lastwritten": "2026-07-10 21:10:05", "storage": "File", "hasencryptionkey": "0"}
      ],
      "scratch": []
    }
  }
})";

static const char *kFlatReply = R"({
  "jsonrpc": "2.0", "id": null,
  "result": {
    "volumes": [
      {"mediaid": "47318", "volumename": "Full-0001", "volstatus": "Error",
       "enabled": "1", "volbytes": "23902210671", "mediatype": "File"},
      {"mediaid": "47322", "volumename": "Incremental-47322", "volstatus": "Append",
       "enabled": "1", "volbytes": "366260149", "mediatype": "File"}
    ]
  }
})";

static const char *kLlistReply = R"({
  "jsonrpc": "2.0", "id": null,
  "result": {
    "volumes": [
      {"mediaid": "47318", "volumename": "Full-0001", "pool": "Full", "volstatus": "Error"}
    ]
  }
})";

static QJsonObject findVolume(const QJsonArray &volumes, const QString &name)
{
    for (const QJsonValue &v : volumes) {
        if (v.toObject()["volumename"].toString() == name) {
            return v.toObject();
        }
    }
    return QJsonObject();
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const QStringList knownPools = {"Scratch", "Incremental", "Full", "Differential"};

    // Grouped form with known pools -> configured spelling
    {
        QJsonArray volumes = BVolumeJson::normalizeVolumes(volumesOf(kGroupedReply), knownPools);
        check(volumes.size() == 3, "grouped: 3 volumes flattened (empty group ignored)");
        check(findVolume(volumes, "Full-0001")["pool"].toString() == "Full",
              "grouped: pool key 'full' mapped to 'Full'");
        check(findVolume(volumes, "Incremental-47322")["pool"].toString() == "Incremental",
              "grouped: pool key 'incremental' mapped to 'Incremental'");
        check(findVolume(volumes, "Full-47321")["volbytes"].toString() == "158201165",
              "grouped: volume fields preserved");
    }

    // Grouped form without known pools -> group key
    {
        QJsonArray volumes = BVolumeJson::normalizeVolumes(volumesOf(kGroupedReply));
        check(findVolume(volumes, "Full-0001")["pool"].toString() == "full",
              "grouped: without .pools the group key is used");
    }

    // Flat form -> unchanged, no pool field
    {
        QJsonArray volumes = BVolumeJson::normalizeVolumes(volumesOf(kFlatReply), knownPools);
        check(volumes.size() == 2, "flat: 2 volumes");
        check(!findVolume(volumes, "Full-0001").contains("pool"), "flat: no pool field added");
    }

    // llist form -> pool field kept
    {
        QJsonArray volumes = BVolumeJson::normalizeVolumes(volumesOf(kLlistReply), knownPools);
        check(volumes.size() == 1 && volumes[0].toObject()["pool"].toString() == "Full",
              "llist: pool field kept");
    }

    // Empty / missing
    {
        check(BVolumeJson::normalizeVolumes(QJsonValue()).isEmpty(), "missing volumes -> empty");
        check(BVolumeJson::normalizeVolumes(QJsonObject()).isEmpty(), "empty object -> empty");
    }

    // resolvePoolName
    check(BVolumeJson::resolvePoolName("DIFFERENTIAL", knownPools) == "Differential",
          "resolvePoolName: case-insensitive match");
    check(BVolumeJson::resolvePoolName("unknown", knownPools) == "unknown",
          "resolvePoolName: unknown key unchanged");

    QTextStream(stdout) << (g_failures == 0 ? "ALL TESTS PASSED" : "TESTS FAILED")
                        << " (" << g_failures << " failures)" << Qt::endl;
    return g_failures == 0 ? 0 : 1;
}
