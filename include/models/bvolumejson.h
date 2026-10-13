#ifndef BVOLUMEJSON_H
#define BVOLUMEJSON_H

#include <QJsonArray>
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
 * @brief Maps a (lowercased) pool key back to the configured pool name
 * @return Matching entry of @p knownPools, or @p key if none matches
 */
QString resolvePoolName(const QString &key, const QStringList &knownPools);

} // namespace BVolumeJson

#endif // BVOLUMEJSON_H
