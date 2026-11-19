#ifndef BVOLUMEJSON_H
#define BVOLUMEJSON_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QStringList>

/**
 * @brief Helpers for the JSON reply of "list volumes"
 *
 * The Director returns two different shapes in API mode 2:
 *  - "list volumes" (no pool): volumes grouped by pool,
 *    result.volumes = { "<lowercase pool name>": [ {...}, ... ], ... }
 *  - "list volumes pool=X" / "list volumes all" / "llist volumes":
 *    flat array result.volumes = [ {...}, ... ]
 *
 * Only "llist volumes" carries a "pool" field per volume.
 *
 * The Onesimus Director answers DirectorApi.ExecuteCommand (gRPC) with its
 * typed form instead: result.pools = { "<pool>": { "volumes": [ ... ] }, ... },
 * where volstatus is the number of the VolumeStatus enum (1 Append, 2 Full,
 * 6 Error, ...). volumesFromResult() accepts all three shapes.
 *
 * @since 2.8
 */
namespace BVolumeJson {

/**
 * @brief Converts both reply shapes into one flat array of volume objects
 *
 * For the grouped form every volume gets a "pool" field taken from the group
 * key. The key is the lowercased pool name; if @p knownPools (e.g. from
 * ".pools") contains a case-insensitive match, the configured spelling is used.
 * Volumes that already carry a non-empty "pool" field are left unchanged.
 *
 * @param volumes     The value of result["volumes"]
 * @param knownPools  Configured pool names (optional)
 * @return Flat array of volume objects
 */
QJsonArray normalizeVolumes(const QJsonValue &volumes,
                            const QStringList &knownPools = QStringList());

/**
 * @brief Flat array of volume objects from the "result" object of any of the
 *        three reply shapes (grouped, flat, typed gRPC)
 *
 * A numeric volstatus is replaced by its name ("Append", "Full", ...), so the
 * rest of the application sees the API-mode-2 spelling.
 *
 * @param result      The value of reply["result"]
 * @param knownPools  Configured pool names (optional)
 * @param recognized  Set to false if the result carries none of the shapes
 */
QJsonArray volumesFromResult(const QJsonObject &result,
                             const QStringList &knownPools = QStringList(),
                             bool *recognized = nullptr);

/**
 * @brief Name of a numeric VolumeStatus (onesimus-dird proto), e.g. 6 -> "Error"
 * @return The Bareos spelling, or an empty string for an unknown number
 */
QString volumeStatusName(int status);

/**
 * @brief Maps a (lowercased) pool key back to the configured pool name
 * @return Matching entry of @p knownPools, or @p key if none matches
 */
QString resolvePoolName(const QString &key, const QStringList &knownPools);

} // namespace BVolumeJson

#endif // BVOLUMEJSON_H
