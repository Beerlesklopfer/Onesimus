// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bzipimport_test.cpp
 * @brief ZIP import: configuration files read in memory, limits on archive
 *        size, entry count, entry and total size, sizes that lie, paths
 *        leaving the archive, symbolic links; parseZipArchive() on top
 */

#include "config/bzipimport.h"
#include "config/bconfigparser.h"

#include <QFile>
#include <QTemporaryDir>
#include <QtEndian>
#include <QtTest>

#include <private/qzipwriter_p.h>

using namespace BZipImport;

namespace {

const QByteArray kDirector = "Director {\n  Name = bareos-dir\n  Password = \"secret\"\n}\n";
const QByteArray kConsole = "Console {\n  Name = admin\n  Password = \"pw\"\n}\n";

struct Entry { QString name; QByteArray data; bool symlink = false; };

QString makeZip(const QTemporaryDir &dir, const QString &name, const QList<Entry> &entries,
                QZipWriter::CompressionPolicy policy = QZipWriter::AlwaysCompress)
{
    const QString path = dir.filePath(name);
    QZipWriter zip(path);
    zip.setCompressionPolicy(policy);
    for (const Entry &e : entries) {
        if (e.symlink)
            zip.addSymLink(e.name, QString::fromUtf8(e.data));
        else
            zip.addFile(e.name, e.data);
    }
    zip.close();
    return path;
}

bool contains(const QStringList &list, const QString &part)
{
    for (const QString &s : list)
        if (s.contains(part)) return true;
    return false;
}

} // namespace

class BZipImportTest : public QObject
{
    Q_OBJECT

private slots:
    void readsConfigFilesOfTheDirectorDirectory()
    {
        QTemporaryDir dir;
        const QString zip = makeZip(dir, QStringLiteral("a.zip"), {
            {QStringLiteral("etc/bareos/bareos-dir.d/director/bareos-dir.conf"), kDirector},
            {QStringLiteral("etc/bareos/bareos-dir.d/console/admin.conf"), kConsole},
            {QStringLiteral("etc/bareos/bconsole.conf"), kConsole},   // outside bareos-dir.d: not read
            {QStringLiteral("README.txt"), "hello"},
        });
        const Result r = readConfigFiles(zip);
        QVERIFY2(r.ok, qPrintable(r.error));
        QCOMPARE(r.files.size(), 2);
        QCOMPARE(r.files.at(0).data, kDirector);
        QVERIFY(r.skipped.isEmpty());
    }

    void flatArchive()
    {
        QTemporaryDir dir;
        const QString zip = makeZip(dir, QStringLiteral("flat.zip"), {
            {QStringLiteral("bareos-dir.conf"), kDirector}, {QStringLiteral("console.conf"), kConsole}});
        const Result r = readConfigFiles(zip);
        QVERIFY(r.ok);
        QCOMPARE(r.files.size(), 2);
    }

    void pathsLeavingTheArchive_data()
    {
        QTest::addColumn<QString>("path");
        QTest::addColumn<bool>("safe");
        QTest::newRow("relative") << "etc/bareos/x.conf" << true;
        QTest::newRow("dot") << "./x.conf" << true;
        QTest::newRow("dotdot") << "../x.conf" << false;
        QTest::newRow("inner dotdot") << "a/../../x.conf" << false;
        QTest::newRow("backslash dotdot") << "a\\..\\..\\x.conf" << false;
        QTest::newRow("absolute") << "/etc/bareos/x.conf" << false;
        QTest::newRow("windows absolute") << "\\x.conf" << false;
        QTest::newRow("drive") << "C:x.conf" << false;
        QTest::newRow("dots in name") << "a..b.conf" << true;
        QTest::newRow("control") << "a\nb.conf" << false;
        QTest::newRow("empty") << "" << false;
    }

    void pathsLeavingTheArchive()
    {
        QFETCH(QString, path);
        QFETCH(bool, safe);
        QCOMPARE(isSafePath(path), safe);
    }

    void traversalEntryRefused()
    {
        QTemporaryDir dir;
        const QString zip = makeZip(dir, QStringLiteral("evil.zip"), {
            {QStringLiteral("../../evil.conf"), kDirector}, {QStringLiteral("good.conf"), kConsole}});
        const Result r = readConfigFiles(zip);
        QVERIFY(r.ok);
        QCOMPARE(r.files.size(), 1);
        QCOMPARE(r.files.first().path, QStringLiteral("good.conf"));
        QVERIFY2(contains(r.skipped, QStringLiteral("evil.conf")), qPrintable(r.skipped.join(QLatin1Char('|'))));
        // Nothing was written next to the archive
        QVERIFY(!QFile::exists(dir.filePath(QStringLiteral("../../evil.conf"))));
    }

    void symlinkRefused()
    {
        QTemporaryDir dir;
        const QString zip = makeZip(dir, QStringLiteral("link.zip"), {
            {QStringLiteral("bareos-dir.d/director/x.conf"), "/etc/shadow", true},
            {QStringLiteral("bareos-dir.d/console/admin.conf"), kConsole}});
        const Result r = readConfigFiles(zip);
        QVERIFY(r.ok);
        QCOMPARE(r.files.size(), 1);
        QVERIFY2(contains(r.skipped, QStringLiteral("symbolic link")), qPrintable(r.skipped.join(QLatin1Char('|'))));
    }

    void entrySizeLimit()
    {
        QTemporaryDir dir;
        const QString zip = makeZip(dir, QStringLiteral("big.zip"), {
            {QStringLiteral("big.conf"), QByteArray(5000, 'x')}, {QStringLiteral("small.conf"), kConsole}});
        Limits limits;
        limits.maxEntrySize = 1024;
        const Result r = readConfigFiles(zip, limits);
        QVERIFY(r.ok);
        QCOMPARE(r.files.size(), 1);
        QVERIFY(contains(r.skipped, QStringLiteral("big.conf")));
    }

    void compressedSizeLimit()
    {
        // A file that compresses badly: its compressed size counts as well
        QTemporaryDir dir;
        QByteArray noise(4000, '\0');
        quint32 x = 1;
        for (char &c : noise) { x = x * 1103515245u + 12345u; c = char(x >> 24); }
        const QString zip = makeZip(dir, QStringLiteral("noise.zip"), {{QStringLiteral("noise.conf"), noise}});
        Limits limits;
        limits.maxCompressedEntrySize = 1024;
        const Result r = readConfigFiles(zip, limits);
        QVERIFY(r.ok);
        QVERIFY(r.files.isEmpty());
        QVERIFY(contains(r.skipped, QStringLiteral("noise.conf")));
    }

    void totalSizeLimit()
    {
        QTemporaryDir dir;
        QList<Entry> entries;
        for (int i = 0; i < 5; ++i)
            entries.append({QStringLiteral("f%1.conf").arg(i), QByteArray(800, 'a' + i)});
        const QString zip = makeZip(dir, QStringLiteral("total.zip"), entries);
        Limits limits;
        limits.maxTotalSize = 2000;
        const Result r = readConfigFiles(zip, limits);
        QVERIFY(!r.ok);
        QVERIFY(r.files.isEmpty());
        QVERIFY(!r.error.isEmpty());
    }

    void entryCountLimit()
    {
        QTemporaryDir dir;
        QList<Entry> entries;
        for (int i = 0; i < 12; ++i)
            entries.append({QStringLiteral("x%1.txt").arg(i), "x"});
        const QString zip = makeZip(dir, QStringLiteral("many.zip"), entries);
        Limits limits;
        limits.maxEntries = 10;
        const Result r = readConfigFiles(zip, limits);
        QVERIFY(!r.ok);
        QVERIFY(r.error.contains(QStringLiteral("12")));
    }

    void archiveSizeLimit()
    {
        QTemporaryDir dir;
        const QString zip = makeZip(dir, QStringLiteral("a.zip"), {{QStringLiteral("a.conf"), kDirector}});
        Limits limits;
        limits.maxArchiveSize = 10;
        QVERIFY(!readConfigFiles(zip, limits).ok);
    }

    void lyingSizeRefused_data()
    {
        QTest::addColumn<bool>("compressed");
        QTest::newRow("stored") << false;
        QTest::newRow("deflated") << true;
    }

    void lyingSizeRefused()
    {
        // The central directory claims 10 bytes; the data is longer
        QFETCH(bool, compressed);
        QTemporaryDir dir;
        const QString zip = makeZip(dir, QStringLiteral("lie.zip"), {{QStringLiteral("lie.conf"), kDirector + kDirector}},
                                    compressed ? QZipWriter::AlwaysCompress : QZipWriter::NeverCompress);
        QFile f(zip);
        QVERIFY(f.open(QIODevice::ReadWrite));
        QByteArray bytes = f.readAll();
        const int cd = bytes.lastIndexOf(QByteArrayLiteral("PK\x01\x02"));
        QVERIFY(cd > 0);
        qToLittleEndian<quint32>(10, reinterpret_cast<uchar *>(bytes.data() + cd + 24));
        f.seek(0);
        f.write(bytes);
        f.close();
        const Result r = readConfigFiles(zip);
        QVERIFY(r.ok);
        QVERIFY(r.files.isEmpty());
        QVERIFY2(contains(r.skipped, QStringLiteral("lie.conf")), qPrintable(r.skipped.join(QLatin1Char('|'))));
    }

    void notAZip()
    {
        QTemporaryDir dir;
        const QString path = dir.filePath(QStringLiteral("x.zip"));
        QFile f(path);
        QVERIFY(f.open(QIODevice::WriteOnly));
        f.write("this is not a zip archive at all, just text");
        f.close();
        const Result r = readConfigFiles(path);
        QVERIFY(!r.ok);
        QVERIFY(!r.error.isEmpty());
        QVERIFY(!readConfigFiles(dir.filePath(QStringLiteral("missing.zip"))).ok);
    }

    void parserReadsTheArchive()
    {
        QTemporaryDir dir;
        const QString zip = makeZip(dir, QStringLiteral("p.zip"), {
            {QStringLiteral("bareos-dir.d/director/bareos-dir.conf"), kDirector},
            {QStringLiteral("bareos-dir.d/console/admin.conf"), kConsole},
            {QStringLiteral("bareos-dir.d/../../evil.conf"), kConsole}});
        BConfigParser parser;
        QVERIFY(parser.parseZipArchive(zip));
        QCOMPARE(parser.resourcesByType(QStringLiteral("Director")).size(), 1);
        QCOMPARE(parser.resourcesByType(QStringLiteral("Console")).size(), 1);
        QCOMPARE(parser.skippedEntries().size(), 1);

        BConfigParser none;
        QVERIFY(!none.parseZipArchive(dir.filePath(QStringLiteral("missing.zip"))));
        QVERIFY(!none.lastError().isEmpty());
    }
};

QTEST_GUILESS_MAIN(BZipImportTest)
#include "bzipimport_test.moc"
