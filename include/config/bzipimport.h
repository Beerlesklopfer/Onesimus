// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bzipimport.h
 * @brief The configuration files of a ZIP archive, read into memory with
 *        limits; the one way every ZIP import goes (backlog P1-25)
 *
 * Nothing is extracted to disk, so a path in the archive cannot reach any
 * file; paths that would leave the archive (absolute, "..", drive letters)
 * are still refused and reported. The central directory is read here, so
 * the sizes are checked before anything is inflated: QZipReader grows its
 * buffer for as long as the data inflates, and the compressed size bounds
 * what one entry can become (deflate inflates at most about 1:1032).
 */
#ifndef BZIPIMPORT_H
#define BZIPIMPORT_H

#include <QByteArray>
#include <QList>
#include <QString>
#include <QStringList>

namespace BZipImport {

struct Limits {
    qint64 maxArchiveSize = 16 * 1024 * 1024;     ///< The ZIP file itself
    int maxEntries = 1000;                        ///< Entries in the archive, of any kind
    qint64 maxEntrySize = 1024 * 1024;            ///< One configuration file, uncompressed
    qint64 maxCompressedEntrySize = 64 * 1024;    ///< One configuration file, compressed
    qint64 maxTotalSize = 16 * 1024 * 1024;       ///< All configuration files, uncompressed
};

struct File {
    QString path;       ///< Path in the archive
    QByteArray data;
};

struct Result {
    bool ok = false;
    QString error;          ///< Why nothing was read (ok false)
    QList<File> files;      ///< The *.conf files (of bareos-dir.d/bacula-dir.d if the archive has one)
    QStringList skipped;    ///< "path: reason" for every entry that was refused
};

/// true when @p path stays inside the archive (relative, no "..", no drive)
bool isSafePath(const QString &path);

/// Read the configuration files of @p zipPath within @p limits
Result readConfigFiles(const QString &zipPath, const Limits &limits = Limits());

} // namespace BZipImport

#endif // BZIPIMPORT_H
