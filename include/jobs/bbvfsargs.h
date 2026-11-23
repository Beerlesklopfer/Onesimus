// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

/**
 * @file bbvfsargs.h
 * @brief The arguments of the .bvfs commands, built from checked values
 *
 * Every value in these arguments comes from the Director (job ids of
 * .bvfs_get_jobids, file ids and path ids of the listings) or is made by
 * the restore wizard (the restore table). None is quoted: the Director
 * reads job, file and directory ids as comma-separated numbers
 * (BvfsValidateJobids() and Bvfs::compute_restore_list() in
 * core/src/dird/ua_dotcmds.cc and core/src/cats/bvfs.cc) and a restore
 * table only as "b2" followed by digits (CheckTempBvfsTableName() in
 * core/src/cats/bvfs.cc). So a value of any other form is refused: each
 * builder returns an empty string, and nothing is sent.
 */
#ifndef BBVFSARGS_H
#define BBVFSARGS_H

#include <QString>
#include <QStringList>

namespace BBvfsArgs {

/// true for one or more ASCII digits
inline bool isNumber(const QString &text)
{
    if (text.isEmpty())
        return false;
    for (const QChar c : text) {
        if (c.unicode() < '0' || c.unicode() > '9')
            return false;
    }
    return true;
}

/// true for "1" or "1,2,3": numbers separated by single commas, no blanks
inline bool isIdList(const QString &text)
{
    if (text.isEmpty())
        return false;
    for (const QString &part : text.split(QLatin1Char(','))) {
        if (!isNumber(part))
            return false;
    }
    return true;
}

/// @p ids joined by commas, or an empty string if one of them is not a number
inline QString idList(const QStringList &ids)
{
    for (const QString &id : ids) {
        if (!isNumber(id))
            return QString();
    }
    return ids.join(QLatin1Char(','));
}

/// true for a restore table name the Director accepts: "b2" and digits
inline bool isRestoreTable(const QString &table)
{
    return table.size() > 2 && table.startsWith(QLatin1String("b2")) && isNumber(table.mid(2));
}

/// "jobid=<ids>" for .bvfs_get_jobids and .bvfs_update
inline QString jobIds(const QString &ids)
{
    return isIdList(ids) ? QStringLiteral("jobid=") + ids : QString();
}

/**
 * @brief "jobid=<ids> pathid=<n>|path= limit=<n> offset=<n>" for
 *        .bvfs_lsdirs and .bvfs_lsfiles
 *
 * A path id of 0 or less asks for the root by "path=" with an empty value
 * (BvfsParseArg() and Bvfs::ChDir("") in ua_dotcmds.cc).
 */
inline QString listing(const QString &ids, qint64 pathId, int offset, int limit)
{
    if (!isIdList(ids) || offset < 0 || limit <= 0)
        return QString();
    const QString where = pathId > 0 ? QStringLiteral("pathid=%1").arg(pathId)
                                     : QStringLiteral("path=");
    return QStringLiteral("jobid=%1 %2 limit=%3 offset=%4").arg(ids, where).arg(limit).arg(offset);
}

/**
 * @brief "path=<table> jobid=<ids> [fileid=<ids>] [dirid=<ids>]" for
 *        .bvfs_restore
 *
 * Empty file and directory lists are left out; both empty is refused
 * (nothing to restore).
 */
inline QString restore(const QString &table, const QString &ids,
                       const QStringList &fileIds, const QStringList &dirIds)
{
    if (!isRestoreTable(table) || !isIdList(ids) || (fileIds.isEmpty() && dirIds.isEmpty()))
        return QString();
    const QString files = idList(fileIds);
    const QString dirs = idList(dirIds);
    if ((!fileIds.isEmpty() && files.isEmpty()) || (!dirIds.isEmpty() && dirs.isEmpty()))
        return QString();
    QString args = QStringLiteral("path=%1 jobid=%2").arg(table, ids);
    if (!files.isEmpty())
        args += QStringLiteral(" fileid=") + files;
    if (!dirs.isEmpty())
        args += QStringLiteral(" dirid=") + dirs;
    return args;
}

/// "path=<table>" for .bvfs_cleanup
inline QString cleanup(const QString &table)
{
    return isRestoreTable(table) ? QStringLiteral("path=") + table : QString();
}

} // namespace BBvfsArgs

#endif // BBVFSARGS_H
