// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bzipimport.cpp
 * @brief Configuration files of a ZIP archive with limits (see bzipimport.h)
 */

#include "config/bzipimport.h"
#include "config/bconfigparser.h"

#include <QCoreApplication>
#include <QFile>
#include <QSet>
#include <QtEndian>

#include <private/qzipreader_p.h>

namespace BZipImport {

namespace {

class Text
{
    Q_DECLARE_TR_FUNCTIONS(BZipImport)
};

quint16 u16(const QByteArray &d, qint64 at)
{
    return qFromLittleEndian<quint16>(reinterpret_cast<const uchar *>(d.constData() + at));
}

quint32 u32(const QByteArray &d, qint64 at)
{
    return qFromLittleEndian<quint32>(reinterpret_cast<const uchar *>(d.constData() + at));
}

struct CentralEntry {
    QString name;
    quint16 method = 0;
    quint16 flags = 0;
    quint32 compressedSize = 0;
    quint32 size = 0;
    bool symlink = false;
};

// The central directory; false with @p error if the archive is not one
// this reader takes
bool centralDirectory(const QByteArray &d, QList<CentralEntry> *entries, int maxEntries, QString *error)
{
    const qint64 n = d.size();
    if (n < 22) {
        *error = Text::tr("The file is not a ZIP archive.");
        return false;
    }
    // End of central directory: 22 bytes plus a comment of up to 64 KiB
    qint64 eocd = -1;
    for (qint64 i = n - 22; i >= 0 && i >= n - 22 - 65535; --i) {
        if (u32(d, i) == 0x06054b50) {
            eocd = i;
            break;
        }
    }
    if (eocd < 0) {
        *error = Text::tr("The file is not a ZIP archive.");
        return false;
    }
    const quint16 count = u16(d, eocd + 10);
    const quint32 cdSize = u32(d, eocd + 12);
    const quint32 cdOffset = u32(d, eocd + 16);
    if (count == 0xffff || cdSize == 0xffffffff || cdOffset == 0xffffffff) {
        *error = Text::tr("ZIP64 archives are not read.");
        return false;
    }
    if (count > maxEntries) {
        *error = Text::tr("The archive has %1 entries; at most %2 are read.").arg(count).arg(maxEntries);
        return false;
    }
    if (qint64(cdOffset) + cdSize > eocd) {
        *error = Text::tr("The ZIP archive is damaged.");
        return false;
    }
    qint64 p = cdOffset;
    for (int i = 0; i < count; ++i) {
        if (p + 46 > eocd || u32(d, p) != 0x02014b50) {
            *error = Text::tr("The ZIP archive is damaged.");
            return false;
        }
        CentralEntry e;
        const quint16 madeBy = u16(d, p + 4);
        e.flags = u16(d, p + 8);
        e.method = u16(d, p + 10);
        e.compressedSize = u32(d, p + 20);
        e.size = u32(d, p + 24);
        const quint16 nameLen = u16(d, p + 28);
        const quint16 extraLen = u16(d, p + 30);
        const quint16 commentLen = u16(d, p + 32);
        const quint32 external = u32(d, p + 38);
        if (p + 46 + nameLen > eocd) {
            *error = Text::tr("The ZIP archive is damaged.");
            return false;
        }
        const QByteArray rawName = d.mid(p + 46, nameLen);
        e.name = (e.flags & 0x0800) ? QString::fromUtf8(rawName) : QString::fromLatin1(rawName);
        // Unix: the file type in the upper half of the external attributes
        e.symlink = (madeBy >> 8) == 3 && ((external >> 16) & 0170000) == 0120000;
        entries->append(e);
        p += 46 + nameLen + extraLen + commentLen;
    }
    return true;
}

} // namespace

bool isSafePath(const QString &path)
{
    if (path.isEmpty())
        return false;
    QString p = path;
    p.replace(QLatin1Char('\\'), QLatin1Char('/'));
    if (p.startsWith(QLatin1Char('/')))
        return false;
    if (p.size() >= 2 && p[1] == QLatin1Char(':'))
        return false;   // C:...
    for (const QString &part : p.split(QLatin1Char('/'))) {
        if (part == QLatin1String(".."))
            return false;
    }
    for (const QChar c : p) {
        if (c.unicode() < 0x20)
            return false;
    }
    return true;
}

Result readConfigFiles(const QString &zipPath, const Limits &limits)
{
    Result result;
    QFile file(zipPath);
    if (!file.open(QIODevice::ReadOnly)) {
        result.error = Text::tr("Cannot open %1: %2").arg(zipPath, file.errorString());
        return result;
    }
    if (file.size() > limits.maxArchiveSize) {
        result.error = Text::tr("The archive is larger than %1 MiB.").arg(limits.maxArchiveSize / (1024 * 1024));
        return result;
    }
    const QByteArray bytes = file.readAll();
    file.close();

    QList<CentralEntry> entries;
    if (!centralDirectory(bytes, &entries, limits.maxEntries, &result.error))
        return result;

    QZipReader zip(zipPath);
    if (!zip.isReadable()) {
        result.error = Text::tr("Cannot read the ZIP archive %1.").arg(zipPath);
        return result;
    }
    QSet<QString> known;
    for (const QZipReader::FileInfo &info : zip.fileInfoList())
        known.insert(info.filePath);

    QList<CentralEntry> wanted;
    for (const CentralEntry &e : std::as_const(entries)) {
        if (e.name.endsWith(QLatin1Char('/')))
            continue;   // a directory
        if (!e.name.endsWith(QLatin1String(".conf"), Qt::CaseInsensitive))
            continue;   // not a configuration file; not read
        QString why;
        if (!isSafePath(e.name))
            why = Text::tr("the path leaves the archive");
        else if (e.symlink)
            why = Text::tr("a symbolic link");
        else if (e.flags & 0x0001)
            why = Text::tr("encrypted");
        else if (e.method != 0 && e.method != 8)
            why = Text::tr("compression method %1 is not read").arg(e.method);
        else if (e.method == 0 && e.compressedSize != e.size)
            why = Text::tr("the size does not match the archive directory");   // QZipReader would cut it
        else if (e.size > limits.maxEntrySize || e.compressedSize > limits.maxCompressedEntrySize)
            why = Text::tr("larger than %1 KiB").arg(limits.maxEntrySize / 1024);
        else if (!known.contains(e.name))
            why = Text::tr("the name cannot be read");
        if (!why.isEmpty()) {
            result.skipped.append(QStringLiteral("%1: %2").arg(e.name, why));
            continue;
        }
        wanted.append(e);
    }

    // A Director configuration directory in the archive: only its files
    auto inConfigDir = [](const QString &path) {
        const QStringList parts = path.split(QLatin1Char('/'));
        return parts.contains(QStringLiteral("bareos-dir.d")) || parts.contains(QStringLiteral("bacula-dir.d"));
    };
    bool hasConfigDir = false;
    for (const CentralEntry &e : std::as_const(wanted))
        hasConfigDir = hasConfigDir || inConfigDir(e.name);

    qint64 total = 0;
    for (const CentralEntry &e : std::as_const(wanted)) {
        if (hasConfigDir && !inConfigDir(e.name))
            continue;
        if (total + e.size > limits.maxTotalSize) {
            result.error = Text::tr("The configuration files are larger than %1 MiB together.")
                               .arg(limits.maxTotalSize / (1024 * 1024));
            result.files.clear();
            return result;
        }
        const QByteArray data = zip.fileData(e.name);
        // The size in the directory can lie; the data decides
        if (data.size() > limits.maxEntrySize || data.size() != qint64(e.size)) {
            result.skipped.append(QStringLiteral("%1: %2").arg(e.name, Text::tr("the size does not match the archive directory")));
            continue;
        }
        total += data.size();
        result.files.append(File{e.name, data});
    }
    result.ok = true;
    return result;
}

} // namespace BZipImport

bool BConfigParser::parseZipArchive(const QString &zipPath, const QString &basePath)
{
    m_skippedEntries.clear();
    const BZipImport::Result read = BZipImport::readConfigFiles(zipPath);
    m_skippedEntries = read.skipped;
    if (!read.ok) {
        m_lastError = read.error;
        emit parsingError(m_lastError);
        return false;
    }
    QString base = basePath;
    if (!base.isEmpty() && !base.endsWith(QLatin1Char('/')))
        base += QLatin1Char('/');
    const int before = m_resources.size();
    for (const BZipImport::File &f : read.files) {
        if (!base.isEmpty() && !f.path.startsWith(base))
            continue;
        parseContent(QString::fromUtf8(f.data), zipPath + QStringLiteral(":") + f.path);
    }
    emit parsingComplete(m_resources.size());
    return m_resources.size() > before;
}
