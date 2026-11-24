// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bconfigexporter.cpp
 * @brief Implementation of Bareos configuration exporter
 */

#include "config/bconfigexporter.h"
#include "bprivatefile.h"
#include "bquote.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <functional>

// Qt's private QZipWriter (available since Qt 5.2)
#include <private/qzipwriter_p.h>

QString BConfigExporter::s_lastError;

namespace {

// A resource name becomes the file name <name>.conf in bareos-dir.d/<type>/.
// Bareos allows '/' in names, but the archive is extracted into the
// Director's root directory: a path in the name would put the file outside
// that directory, or into a subdirectory the Director does not read.
bool isUsableAsFileName(const QString &name)
{
    if (name.isEmpty())
        return false;
    for (const QChar c : name) {
        if (c == QLatin1Char('/') || c == QLatin1Char('\\') || c.unicode() < 0x20 || c.unicode() == 0x7f)
            return false;
    }
    return true;
}

// The profile values written into the archive's resource text that hold a
// control character, by directive name. BQuote escapes quotes and
// backslashes, but a control character cannot be escaped: a newline would
// end the value and the rest of the line would become a directive.
QStringList fieldsWithControlCharacters(const BConnectionProfile &p)
{
    QStringList fields;
    auto check = [&fields](const char *name, const QString &value) {
        if (BQuote::hasControlCharacter(value) && !fields.contains(QLatin1String(name)))
            fields << QLatin1String(name);
    };
    auto checkList = [&check](const char *name, const QStringList &values) {
        for (const QString &v : values)
            check(name, v);
    };
    check("Name (Director)", p.directorName);
    check("Address", p.host);
    check("Description", p.description);
    check("Profile", p.profile);
    check("TLS CA Certificate File", p.tlsCaCertFile);
    check("TLS CA Certificate Dir", p.tlsCaCertDir);
    check("TLS Certificate", p.tlsCertFile);
    check("TLS Key", p.tlsKeyFile);
    check("TLS Cipher List", p.tlsCipherList);
    check("TLS Cipher Suites", p.tlsCipherSuites);
    check("TLS DH File", p.tlsDhFile);
    check("TLS Protocol", p.tlsProtocol);
    check("TLS Certificate Revocation List", p.tlsCrlFile);
    check("Grpc Fingerprint", p.grpcCertFingerprint);
    checkList("TLS Allowed CN", p.tlsAllowedCn);
    checkList("Job ACL", p.aclJob);
    checkList("Client ACL", p.aclClient);
    checkList("Storage ACL", p.aclStorage);
    checkList("Schedule ACL", p.aclSchedule);
    checkList("Pool ACL", p.aclPool);
    checkList("FileSet ACL", p.aclFileSet);
    checkList("Catalog ACL", p.aclCatalog);
    checkList("Command ACL", p.aclCommand);
    checkList("Where ACL", p.aclWhere);
    checkList("PluginOptions ACL", p.aclPluginOptions);
    return fields;
}

} // namespace

bool BConfigExporter::exportToArchive(const BConnectionProfile &profile,
                                       const QString &outputPath,
                                       ExportMode mode)
{
    s_lastError.clear();

    // The names that become file names in the archive (see generateFiles())
    if (!isUsableAsFileName(profile.consoleName)) {
        s_lastError = QObject::tr("The console name \"%1\" cannot be used as a file name "
                                  "(empty, or with a slash, backslash or control character)")
                          .arg(profile.consoleName);
        return false;
    }
    if (mode >= ExportWithProfile && profile.hasAcls() && !profile.profile.isEmpty()
        && !isUsableAsFileName(profile.profile)) {
        s_lastError = QObject::tr("The profile name \"%1\" cannot be used as a file name "
                                  "(with a slash, backslash or control character)")
                          .arg(profile.profile);
        return false;
    }

    const QStringList badFields = fieldsWithControlCharacters(profile);
    if (!badFields.isEmpty()) {
        s_lastError = QObject::tr("These values contain a line break or another control "
                                  "character, which Bareos cannot read in a quoted value: %1")
                          .arg(badFields.join(QStringLiteral(", ")));
        return false;
    }

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

QStringList BConfigExporter::findBareosDirectories(int searchDepth)
{
    QStringList found;

    // Common locations to check first (fast path)
    QStringList commonPaths;

#ifdef Q_OS_WIN
    // Windows common paths
    commonPaths << "C:/ProgramData/Bareos"
                << "C:/Program Files/Bareos"
                << "C:/Program Files (x86)/Bareos"
                << "C:/Bareos";
#else
    // Linux/Unix common paths
    commonPaths << "/etc/bareos"
                << "/opt/bareos"
                << "/usr/local/etc/bareos"
                << "/var/lib/bareos";
#endif

    // Check common paths first
    for (const QString &path : commonPaths) {
        QDir dir(path);
        if (dir.exists("bareos-dir.d")) {
            QString fullPath = dir.absoluteFilePath("bareos-dir.d");
            if (!found.contains(fullPath)) {
                found.append(fullPath);
            }
        }
    }

    // If found in common locations, return early
    if (!found.isEmpty()) {
        return found;
    }

    // Otherwise, do a broader recursive search
    QStringList searchRoots;

#ifdef Q_OS_WIN
    // Search common Windows drives
    searchRoots << "C:/"
                << "D:/";
#else
    // Search common Linux root directories
    searchRoots << "/etc"
                << "/opt"
                << "/usr"
                << "/var";
#endif

    // Recursive search function
    std::function<void(const QDir&, int)> searchDir = [&](const QDir &dir, int depth) {
        if (depth <= 0) return;

        // Check if current directory contains bareos-dir.d
        if (dir.exists("bareos-dir.d")) {
            QString fullPath = dir.absoluteFilePath("bareos-dir.d");
            if (!found.contains(fullPath)) {
                found.append(fullPath);
            }
        }

        // Recurse into subdirectories
        QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &subdir : subdirs) {
            // Skip common large directories
            if (subdir == "Windows" || subdir == "System32" ||
                subdir == "proc" || subdir == "sys" || subdir == "dev") {
                continue;
            }

            QString subdirPath = dir.absoluteFilePath(subdir);
            QDir subdirDir(subdirPath);

            // Check if we have read permissions
            if (subdirDir.isReadable()) {
                searchDir(subdirDir, depth - 1);
            }
        }
    };

    // Perform search from each root
    for (const QString &root : searchRoots) {
        QDir rootDir(root);
        if (rootDir.exists() && rootDir.isReadable()) {
            searchDir(rootDir, searchDepth);
        }
    }

    return found;
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

    // oconsole.conf - client-side configuration incl. gRPC keys (oconsole only)
    {
        FileEntry entry;
        entry.path = QString("etc/bareos/oconsole.conf");
        entry.content = profile.toOconsoleConfig().toUtf8();
        files.append(entry);
    }

    return files;
}

bool BConfigExporter::writeZip(const QString &path, const QList<FileEntry> &files)
{
    // The archive contains console passwords: only the owner may read it
    QFile zipFile(path);
    if (!bOpenOwnerOnly(zipFile, QIODevice::WriteOnly)) {
        s_lastError = QObject::tr("Cannot create zip file: %1").arg(path);
        return false;
    }
    QZipWriter zip(&zipFile);

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
