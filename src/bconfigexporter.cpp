/**
 * @file bconfigexporter.cpp
 * @brief Implementation of Bareos configuration exporter
 */

#include "bconfigexporter.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>

// Qt's private QZipWriter (available since Qt 5.2)
#include <private/qzipwriter_p.h>

QString BConfigExporter::s_lastError;

bool BConfigExporter::exportToArchive(const BConnectionProfile &profile,
                                       const QString &outputPath,
                                       ExportMode mode)
{
    s_lastError.clear();

    // Generate files
    QList<FileEntry> files = generateFiles(profile, mode);

    if (files.isEmpty()) {
        s_lastError = QObject::tr("No files to export");
        return false;
    }

    // Determine output path with extension
    QString path = outputPath;
    QString ext = defaultExtension();
    if (!path.endsWith(ext, Qt::CaseInsensitive)) {
        path += ext;
    }

    return writeZip(path, files);
}

QString BConfigExporter::defaultExtension()
{
    return ".zip";
}

QString BConfigExporter::lastError()
{
    return s_lastError;
}

QList<BConfigExporter::FileEntry> BConfigExporter::generateFiles(const BConnectionProfile &profile,
                                                                   ExportMode mode)
{
    QList<FileEntry> files;

    // Console configuration - always included
    {
        FileEntry entry;
        entry.path = QString("etc/bareos/bareos-dir.d/console/%1.conf").arg(profile.consoleName);
        entry.content = profile.toConsoleConfig().toUtf8();
        files.append(entry);
    }

    // Profile configuration - if ACLs are defined
    if (mode >= ExportWithProfile && profile.hasAcls()) {
        FileEntry entry;
        QString profileName = profile.profile.isEmpty() ?
                              profile.consoleName + "-profile" : profile.profile;
        entry.path = QString("etc/bareos/bareos-dir.d/profile/%1.conf").arg(profileName);
        entry.content = profile.toProfileConfig().toUtf8();
        files.append(entry);
    }

    // Director configuration template - for new installations
    if (mode == ExportFull) {
        FileEntry entry;
        entry.path = QString("etc/bareos/bareos-dir.d/director/bareos-dir.conf.template");
        entry.content = profile.toDirectorConfig().toUtf8();
        files.append(entry);
    }

    // bconsole.conf - client-side configuration
    {
        FileEntry entry;
        entry.path = QString("etc/bareos/bconsole.conf");
        entry.content = profile.toBconsoleConfig().toUtf8();
        files.append(entry);
    }

    return files;
}

bool BConfigExporter::writeZip(const QString &path, const QList<FileEntry> &files)
{
    QZipWriter zip(path);
    if (zip.status() != QZipWriter::NoError) {
        s_lastError = QObject::tr("Cannot create zip file: %1").arg(path);
        return false;
    }

    // Collect directories to create
    QSet<QString> directories;
    for (const FileEntry &entry : files) {
        QString dir = QFileInfo(entry.path).path();
        // Add all parent directories
        while (!dir.isEmpty() && dir != ".") {
            directories.insert(dir);
            dir = QFileInfo(dir).path();
        }
    }

    // Create directories (sorted to ensure parents come before children)
    QStringList sortedDirs = directories.values();
    sortedDirs.sort();
    for (const QString &dir : sortedDirs) {
        zip.addDirectory(dir);
    }

    // Add files
    for (const FileEntry &entry : files) {
        zip.addFile(entry.path, entry.content);
    }

    zip.close();

    if (zip.status() != QZipWriter::NoError) {
        s_lastError = QObject::tr("Error writing zip file");
        return false;
    }

    return true;
}
