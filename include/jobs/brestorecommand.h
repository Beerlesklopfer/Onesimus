// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BRESTORECOMMAND_H
#define BRESTORECOMMAND_H

#include <QString>

/**
 * @brief Builds the `restore` command of the restore wizard
 *
 * Plain functions without widgets, so the command can be tested without a
 * Director. The wizard shows and sends exactly the arguments returned by
 * build(). Keywords and their meaning follow the Bareos 25 sources
 * (core/src/dird/ua_restore.cc): `client=` is the client the files were
 * backed up from (GetClientName(), checked against the catalog),
 * `restoreclient=` the client that receives them (GetRestoreClientName()),
 * `restorejob=` the Restore Job resource (get_restore_job(); without it the
 * Director asks which one when more than one exists). Values are parsed by
 * core/src/lib/scan.cc next_arg(): double quotes group, a backslash escapes
 * the next character.
 */
namespace BRestoreCommand {

struct Params {
    QString table;          ///< Restore table made by .bvfs_restore path=<table>
    QString backupClient;   ///< client=: where the files were backed up from
    QString restoreClient;  ///< restoreclient=: where they are restored to; empty = backupClient
    QString restoreJob;     ///< restorejob=: empty leaves the choice to the Director
    QString where;          ///< where=: prefix on the restore client
    QString replace;        ///< always, ifnewer, ifolder or never
};

enum class Error {
    None,
    InvalidTable,       ///< Empty or not letters, digits and underscores
    MissingClient,
    MissingWhere,
    InvalidReplace,
    ControlCharacter    ///< A value contains a newline or another control character
};

struct Result {
    QString arguments;      ///< Everything after "restore "; empty when error != None
    Error error = Error::None;
    QString field;          ///< Keyword of the offending value (e.g. "where")

    bool ok() const { return error == Error::None; }
};

/**
 * @brief Build `file=?<table> client=… restoreclient=… [restorejob=…] where=… replace=… yes`
 *
 * Client names, the restore job and the path are always quoted.
 */
Result build(const Params &params);

} // namespace BRestoreCommand

#endif // BRESTORECOMMAND_H
