#ifndef BVOLUMELIST_H
#define BVOLUMELIST_H

#include <QDateTime>
#include <QJsonObject>
#include <QList>
#include <QString>
#include <QStringList>

#include <optional>

/**
 * @brief One volume (catalog Media record) as the Director lists it
 *
 * Every field the reply may lack is optional, or an invalid QDateTime, or an
 * empty string: a field the Director did not send is unknown, never 0. The
 * short "list volumes" of Bareos 25 has 14 columns
 * (list_volumes_select_0 in core/src/cats/postgresql_queries.inc), the long
 * "llist volumes" 44 (list_volumes_select_long_0). Neither carries the
 * volume's ActionOnPurge.
 *
 * @since 2.9
 */
struct BVolume
{
    QString name;                       ///< volumename
    QString pool;                       ///< pool (llist), else the group key mapped to the configured name
    QString status;                     ///< volstatus in the Bareos spelling ("Append", "Read-Only", ...)
    QString mediaType;                  ///< mediatype
    QString storage;                    ///< storage
    QString recyclePool;                ///< recyclepool (llist; empty = none or unknown)
    QString scratchPool;                ///< scratchpool (llist)

    std::optional<qint64> mediaId;
    std::optional<qint64> volBytes;     ///< bytes written
    std::optional<qint64> maxVolBytes;  ///< limit; 0 = no limit (llist only)
    std::optional<qint64> volJobs;      ///< jobs written (llist only)
    std::optional<qint64> maxVolJobs;   ///< 0 = no limit (llist only)
    std::optional<qint64> volFiles;
    std::optional<qint64> maxVolFiles;  ///< llist only
    std::optional<qint64> volErrors;    ///< llist only
    std::optional<qint64> recycleCount; ///< llist only
    std::optional<qint64> volRetention; ///< seconds
    std::optional<qint64> volUseDuration; ///< seconds; 0 = none (llist only)
    std::optional<bool> recycle;        ///< Recycle = yes
    std::optional<int> enabled;         ///< 0 disabled, 1 enabled, 2 archived
    std::optional<int> actionOnPurge;   ///< only if a reply ever carries it (Bareos 25 does not)
    std::optional<qint64> locationId;   ///< Media.LocationId; 0 = no location (llist only)

    QDateTime firstWritten;             ///< llist only; invalid = unknown or never written
    QDateTime lastWritten;
    QDateTime labelDate;                ///< llist only

    /// The record came with the fields of the long listing ("llist volumes")
    bool longForm = false;
};

/**
 * @brief The volumes of a "list volumes" or "llist volumes" reply
 *
 * Reads all reply shapes that BVolumeJson knows: API mode 2 grouped by pool
 * ({"volumes": {"<lowercase pool>": [...]}}, the form of both "list volumes"
 * and "llist volumes" without arguments, Bareos 25 dird/ua_output.cc
 * ListMedia), the flat array ("... pool=X", "... all") and the typed form of
 * the Onesimus Director over gRPC ({"pools": {"<pool>": {"volumes": [...]}}},
 * numeric volstatus). Values may be strings (API mode 2 sends every column
 * as a string) or JSON numbers and booleans (typed form).
 *
 * The typed form is protobuf JSON, which leaves out fields that hold their
 * default value. There a missing field of the columns the listing has is 0
 * or false, not unknown: for the short columns always, for the long ones
 * when the record shows it is a long listing (it carries poolid, labeldate,
 * voljobs or volmounts).
 *
 * Only QtCore; no widget, no Director.
 *
 * @since 2.9
 */
class BVolumeList
{
public:
    BVolumeList() = default;

    /**
     * @brief Reads the "result" object of a reply
     * @param result      reply["result"]
     * @param knownPools  Configured pool names (".pools"): restore the spelling
     *                    of the lowercased group keys and list empty pools
     * @param recognized  Set to false if the result has none of the shapes
     */
    static BVolumeList fromResult(const QJsonObject &result,
                                  const QStringList &knownPools = QStringList(),
                                  bool *recognized = nullptr);

    /**
     * @brief Reads a whole JSON reply ({"result": {...}})
     * @param error  Receives a message if the text is not a JSON object
     */
    static BVolumeList fromJson(const QByteArray &json,
                                const QStringList &knownPools = QStringList(),
                                QString *error = nullptr);

    /// One volume object of a reply
    static BVolume volumeFromJson(const QJsonObject &object);

    const QList<BVolume> &volumes() const { return m_volumes; }
    int size() const { return int(m_volumes.size()); }
    bool isEmpty() const { return m_volumes.isEmpty(); }

    /**
     * @brief All pools: the configured ones, the groups of the reply and
     *        those that hold volumes
     *
     * Sorted case-insensitively. A pool without volumes is listed (it is a
     * pool all the same; the grouped reply has an empty group for it); a
     * group key that matches no configured name keeps its own spelling.
     */
    QStringList pools() const;

    /// Number of volumes in @p pool
    int countInPool(const QString &pool) const;

    /// Number of volumes with @p status
    int countWithStatus(const QString &status) const;

    /// At least one volume came with the fields of the long listing
    bool hasLongForm() const;

    /**
     * @brief The statuses of the Bareos catalog, in the order of a volume's life
     *
     * Append, Full, Used, Purged, Recycle, Error, Read-Only, Disabled,
     * Archive, Busy, Cleaning, Unlabeled.
     */
    static QStringList allStatuses();

    /**
     * @brief One letter per status, so that colour is never the only cue
     *
     * A Append, F Full, U Used, P Purged, R Recycle, E Error, O Read-Only,
     * D Disabled, V Archive, B Busy, C Cleaning, L Unlabeled; "?" otherwise.
     */
    static QString statusLetter(const QString &status);

    /**
     * @brief A catalog date ("2026-03-09 01:39:04", ISO 8601, or seconds since
     *        1970) in local time; invalid for an empty, null or zero date
     */
    static QDateTime parseDateTime(const QJsonValue &value);

private:
    QList<BVolume> m_volumes;
    QStringList m_knownPools;
    QStringList m_replyPools;   ///< group keys of a grouped reply, empty groups included
};

#endif // BVOLUMELIST_H
