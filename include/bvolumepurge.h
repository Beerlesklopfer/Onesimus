// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BVOLUMEPURGE_H
#define BVOLUMEPURGE_H

#include <QList>
#include <QString>
#include <QStringList>

/**
 * @brief "purge volume all" of the Onesimus Director: command line and reply
 *
 * The Onesimus Director (onesimus-dird) purges the volumes of one pool or of
 * all pools with a chosen volume status in one command:
 *
 *   purge volume all pool=<pool>|allpools [volstatus=Full,Used,...] dryrun|yes
 *
 * It refuses the command without exactly one of pool/allpools and exactly
 * one of dryrun/yes and never prompts. Plain Bareos does not know this form.
 * The functions here are pure: building the line refuses what the Director
 * would refuse, and the parser reads the API mode 2 reply of both modes and
 * the refusal.
 */
namespace BVolumePurge {

enum class Mode {
    DryRun,  ///< "dryrun": the Director only reports what it would purge
    Purge,   ///< "yes": the Director purges
};

/** What the user chose: one pool or all pools, and the volume statuses. */
struct Selection {
    bool allPools = false;
    QString pool;            ///< Pool name; empty with allPools
    QStringList volStatus;   ///< Volume statuses, each one of allowedStatuses()

    bool operator==(const Selection &other) const
    {
        return allPools == other.allPools && pool == other.pool && volStatus == other.volStatus;
    }
    bool operator!=(const Selection &other) const { return !(*this == other); }
};

/** The volume statuses the Director accepts, in the order the dialog shows them. */
QStringList allowedStatuses();

/** The statuses the Director takes when volstatus= is not given: Full, Used, Error. */
QStringList defaultStatuses();

/**
 * @brief The command line for @p selection in @p mode
 *
 * The statuses are always sent, in the order of allowedStatuses(), so that
 * the line says exactly what was chosen. Returns an empty string and sets
 * @p error (if given) when the selection is not a valid command: neither or
 * both of pool and all pools, no status, an unknown status, or a pool name
 * that cannot be written on a command line.
 */
QString buildCommand(const Selection &selection, Mode mode, QString *error = nullptr);

/** What the Director decided for one volume. */
enum class Action {
    Unknown,
    WouldPurge,  ///< dryrun: would be purged
    Purged,      ///< yes: purged
    Skipped,     ///< not purged; see reason
};

struct VolumeRow {
    QString volume;
    QString pool;
    QString volStatus;
    qint64 jobs = 0;
    qint64 files = 0;
    qint64 bytes = 0;
    Action action = Action::Unknown;
    QString actionText;      ///< The action as the Director wrote it
    QString reason;          ///< Why it was skipped; empty otherwise
};

struct Reply {
    bool ok = false;
    QString error;           ///< Without ok: the Director's refusal or why the reply is unusable
    bool modeKnown = false;
    Mode mode = Mode::DryRun;
    QString pool;            ///< Empty with allPools (null in the reply)
    bool allPools = false;
    QStringList volStatus;
    QList<VolumeRow> volumes;
    int selected = 0;
    int purged = 0;
    int skipped = 0;
};

/**
 * @brief Read the Director's reply to a purge command
 *
 * Accepts {"purge":{...}} and the same wrapped in {"jsonrpc":..,"result":{..}}.
 * An {"error":{...}} reply gives ok == false with the Director's messages
 * (data.messages.error, otherwise error.message). A reply that is not JSON
 * or has no "purge" object is not a success either; its text is kept.
 */
Reply parseReply(const QString &reply);

} // namespace BVolumePurge

#endif // BVOLUMEPURGE_H
