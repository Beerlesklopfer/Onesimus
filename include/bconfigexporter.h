/**
 * @file bconfigexporter.h
 * @brief Export Bareos configuration files to archive
 *
 * Creates tar archives (Linux) or zip archives (Windows) containing
 * the Bareos configuration files generated from a connection profile.
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 */

#ifndef BCONFIGEXPORTER_H
#define BCONFIGEXPORTER_H

#include <QString>
#include <QByteArray>
#include <QList>
#include <QPair>
#include <QFile>
#include <QDataStream>

#include "bconnectionprofile.h"

/**
 * @class BConfigExporter
 * @brief Exports connection profile to Bareos configuration archive
 *
 * Generates configuration files and packages them into a zip archive.
 *
 * Archive structure:
 *   etc/bareos/bareos-dir.d/console/<consoleName>.conf
 *   etc/bareos/bareos-dir.d/profile/<profileName>.conf (if ACLs defined)
 *   etc/bareos/bareos-dir.d/director/bareos-dir.conf (template, optional)
 *   etc/bareos/bconsole.conf (client config)
 */
class BConfigExporter
{
public:
    /**
     * @brief Export mode
     */
    enum ExportMode {
        ExportConsoleOnly,      ///< Only console config (for existing Director)
        ExportWithProfile,      ///< Console + Profile (for existing Director)
        ExportFull              ///< Console + Profile + Director template (for new installation)
    };

    /**
     * @brief Export profile to archive file
     * @param profile The connection profile to export
     * @param outputPath Path for the output archive (with or without extension)
     * @param mode Export mode
     * @return true on success, false on error
     */
    static bool exportToArchive(const BConnectionProfile &profile,
                                const QString &outputPath,
                                ExportMode mode = ExportConsoleOnly);

    /**
     * @brief Get the archive extension
     * @return ".zip"
     */
    static QString defaultExtension();

    /**
     * @brief Get last error message
     * @return Error description or empty string
     */
    static QString lastError();

private:
    /**
     * @brief Internal file entry for archive
     */
    struct FileEntry {
        QString path;       ///< Path within archive (e.g., "etc/bareos/...")
        QByteArray content; ///< File content
    };

    /**
     * @brief Generate list of files to include in archive
     */
    static QList<FileEntry> generateFiles(const BConnectionProfile &profile, ExportMode mode);

    /**
     * @brief Write zip archive
     */
    static bool writeZip(const QString &path, const QList<FileEntry> &files);

    static QString s_lastError;
};

#endif // BCONFIGEXPORTER_H
