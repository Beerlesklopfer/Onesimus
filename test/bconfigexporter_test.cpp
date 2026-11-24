// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bconfigexporter_test.cpp
 * @brief BConfigExporter: the archive the connection wizard exports for a
 *        Director (files per mode, file names, owner-only permissions,
 *        errors) read back with QZipReader and with the import path
 *        (BZipImport, BConfigParser)
 *
 * The user extracts the archive into the Director's root directory, so its
 * paths and the resource text in it must be what Bareos reads.
 *
 * Runs without a Director and without network traffic (registered with CTest).
 */

#include <QtTest>
#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>

#include <private/qzipreader_p.h>

#include "bconnectionprofile.h"
#include "config/bconfigexporter.h"
#include "config/bconfigparser.h"
#include "config/bzipimport.h"

class BConfigExporterTest : public QObject
{
    Q_OBJECT

private slots:
    void consoleOnlyArchive();
    void consoleOnlyLeavesProfileOut();
    void withProfileNeedsAcls();
    void profileFileNamedAfterProfile();
    void fullArchiveHasDirectorTemplate();
    void directoriesBeforeTheirFiles();
    void extensionAddedOnce_data();
    void extensionAddedOnce();
    void archiveIsOwnerOnly();
    void overwritesExistingArchive();
    void unwritablePathFails();
    void lastErrorClearedBySuccess();
    void grpcKeysOnlyInOconsole();
    void roundTripThroughImport();
    void windowsServerPaths();
    void defaultExtensionIsZip();
    void namesThatAreNoFileNameRefused_data();
    void namesThatAreNoFileNameRefused();
    void profileNameOnlyCheckedWhenWritten();
    void quotesAndBackslashesReadBackUnchanged();
    void quoteInValueAddsNoDirective();
    void controlCharacterRefused_data();
    void controlCharacterRefused();
};

namespace {

BConnectionProfile sampleProfile()
{
    BConnectionProfile p = BConnectionProfile::create(QStringLiteral("Test"));
    p.host = QStringLiteral("backup.example.org");
    p.port = 9101;
    p.directorName = QStringLiteral("bareos-dir");
    p.consoleName = QStringLiteral("admin-console");
    p.setPasswordFromCleartext(QStringLiteral("secret"));
    return p;
}

/// Entry path -> content of every file in @p zipPath; directories in @p dirs
QMap<QString, QByteArray> readZip(const QString &zipPath, QStringList *dirs = nullptr,
                                  QStringList *order = nullptr)
{
    QMap<QString, QByteArray> files;
    QZipReader zip(zipPath);
    if (!zip.isReadable())
        return files;
    for (const QZipReader::FileInfo &info : zip.fileInfoList()) {
        if (order)
            order->append(info.filePath);
        if (info.isDir) {
            if (dirs)
                dirs->append(info.filePath);
            continue;
        }
        files.insert(info.filePath, zip.fileData(info.filePath));
    }
    return files;
}

const QString kConsole = QStringLiteral("etc/bareos/bareos-dir.d/console/admin-console.conf");
const QString kBconsole = QStringLiteral("etc/bareos/bconsole.conf");
const QString kOconsole = QStringLiteral("etc/bareos/oconsole.conf");
const QString kDirector = QStringLiteral("etc/bareos/bareos-dir.d/director/bareos-dir.conf.template");

} // namespace

void BConfigExporterTest::consoleOnlyArchive()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const BConnectionProfile p = sampleProfile();
    const QString out = dir.filePath("export.zip");

    QVERIFY2(BConfigExporter::exportToArchive(p, out), qPrintable(BConfigExporter::lastError()));
    QVERIFY(BConfigExporter::lastError().isEmpty());

    const QMap<QString, QByteArray> files = readZip(out);
    QCOMPARE(files.keys(), (QStringList{kConsole, kBconsole, kOconsole}));
    QCOMPARE(files.value(kConsole), p.toConsoleConfig().toUtf8());
    QCOMPARE(files.value(kBconsole), p.toBconsoleConfig().toUtf8());
    QCOMPARE(files.value(kOconsole), p.toOconsoleConfig().toUtf8());
    // The password only as its MD5 hash, never in clear text
    QVERIFY(files.value(kConsole).contains("Password = \"[md5]" + p.passwordHash.toUtf8() + "\""));
    for (const QByteArray &content : files)
        QVERIFY(!content.contains("secret"));
}

void BConfigExporterTest::consoleOnlyLeavesProfileOut()
{
    QTemporaryDir dir;
    BConnectionProfile p = sampleProfile();
    p.aclJob = QStringList{QStringLiteral("Backup.*")};
    const QString out = dir.filePath("export.zip");
    QVERIFY(BConfigExporter::exportToArchive(p, out, BConfigExporter::ExportConsoleOnly));

    const QMap<QString, QByteArray> files = readZip(out);
    QCOMPARE(files.size(), 3);
    // Without a Profile the ACLs go into the Console resource itself
    QVERIFY(files.value(kConsole).contains("Job ACL = \"Backup.*\""));
}

void BConfigExporterTest::withProfileNeedsAcls()
{
    QTemporaryDir dir;
    BConnectionProfile p = sampleProfile();
    const QString noAcls = dir.filePath("noacls.zip");
    QVERIFY(BConfigExporter::exportToArchive(p, noAcls, BConfigExporter::ExportWithProfile));
    QCOMPARE(readZip(noAcls).size(), 3);   // no ACLs, no profile file

    p.aclCommand = QStringList{QStringLiteral("status"), QStringLiteral("list")};
    const QString withAcls = dir.filePath("acls.zip");
    QVERIFY(BConfigExporter::exportToArchive(p, withAcls, BConfigExporter::ExportWithProfile));
    const QMap<QString, QByteArray> files = readZip(withAcls);
    const QString profileFile =
        QStringLiteral("etc/bareos/bareos-dir.d/profile/admin-console-profile.conf");
    QCOMPARE(files.size(), 4);
    QVERIFY(files.contains(profileFile));
    QCOMPARE(files.value(profileFile), p.toProfileConfig().toUtf8());
    QVERIFY(files.value(profileFile).contains("Name = \"admin-console-profile\""));
    QVERIFY(files.value(profileFile).contains("Command ACL = \"status\""));
    QVERIFY(files.value(profileFile).contains("Command ACL = \"list\""));
    QVERIFY(!files.contains(kDirector));
}

void BConfigExporterTest::profileFileNamedAfterProfile()
{
    QTemporaryDir dir;
    BConnectionProfile p = sampleProfile();
    p.profile = QStringLiteral("operators");
    p.aclPool = QStringList{QStringLiteral("Full")};
    const QString out = dir.filePath("export.zip");
    QVERIFY(BConfigExporter::exportToArchive(p, out, BConfigExporter::ExportWithProfile));

    const QMap<QString, QByteArray> files = readZip(out);
    const QString profileFile = QStringLiteral("etc/bareos/bareos-dir.d/profile/operators.conf");
    QVERIFY(files.contains(profileFile));
    // The Console refers to the Profile and does not repeat its ACLs
    QVERIFY(files.value(kConsole).contains("Profile = \"operators\""));
    QVERIFY(!files.value(kConsole).contains("Pool ACL"));
    QVERIFY(files.value(profileFile).contains("Pool ACL = \"Full\""));
}

void BConfigExporterTest::fullArchiveHasDirectorTemplate()
{
    QTemporaryDir dir;
    BConnectionProfile p = sampleProfile();
    p.heartbeatInterval = 60;
    p.aclClient = QStringList{QStringLiteral("client1-fd")};
    const QString out = dir.filePath("export.zip");
    QVERIFY(BConfigExporter::exportToArchive(p, out, BConfigExporter::ExportFull));

    const QMap<QString, QByteArray> files = readZip(out);
    QCOMPARE(files.size(), 5);   // console, profile, director, bconsole, oconsole
    QVERIFY(files.contains(kDirector));
    QCOMPARE(files.value(kDirector), p.toDirectorConfig().toUtf8());
    QVERIFY(files.value(kDirector).contains("Heartbeat Interval = 60"));
    QVERIFY(files.contains(QStringLiteral("etc/bareos/bareos-dir.d/profile/admin-console-profile.conf")));

    // Full without ACLs: the director template, still no profile file
    p.aclClient.clear();
    const QString noAcls = dir.filePath("full-noacls.zip");
    QVERIFY(BConfigExporter::exportToArchive(p, noAcls, BConfigExporter::ExportFull));
    QCOMPARE(readZip(noAcls).size(), 4);
}

void BConfigExporterTest::directoriesBeforeTheirFiles()
{
    QTemporaryDir dir;
    BConnectionProfile p = sampleProfile();
    p.aclJob = QStringList{QStringLiteral("*all*")};
    const QString out = dir.filePath("export.zip");
    QVERIFY(BConfigExporter::exportToArchive(p, out, BConfigExporter::ExportFull));

    QStringList dirs;
    QStringList order;
    const QMap<QString, QByteArray> files = readZip(out, &dirs, &order);
    for (const QString &d : QStringList{"etc", "etc/bareos", "etc/bareos/bareos-dir.d",
                                        "etc/bareos/bareos-dir.d/console",
                                        "etc/bareos/bareos-dir.d/profile",
                                        "etc/bareos/bareos-dir.d/director"})
        QVERIFY2(dirs.contains(d), qPrintable(d));
    QCOMPARE(dirs.size(), 6);

    // Every entry comes after the directory that holds it
    for (int i = 0; i < order.size(); ++i) {
        const QString parent = QFileInfo(order.at(i)).path();
        if (parent == QLatin1String("."))
            continue;
        QVERIFY2(order.indexOf(parent) >= 0 && order.indexOf(parent) < i, qPrintable(order.at(i)));
    }
    // Paths are relative: nothing to extract outside the target directory
    for (const QString &entry : order)
        QVERIFY2(BZipImport::isSafePath(entry), qPrintable(entry));
}

void BConfigExporterTest::extensionAddedOnce_data()
{
    QTest::addColumn<QString>("given");
    QTest::addColumn<QString>("written");
    QTest::newRow("no extension") << "export" << "export.zip";
    QTest::newRow("zip") << "export.zip" << "export.zip";
    QTest::newRow("ZIP") << "export.ZIP" << "export.ZIP";
    QTest::newRow("other extension") << "export.tar" << "export.tar.zip";
    QTest::newRow("dots in name") << "bareos-config-a.b" << "bareos-config-a.b.zip";
}

void BConfigExporterTest::extensionAddedOnce()
{
    QFETCH(QString, given);
    QFETCH(QString, written);
    QTemporaryDir dir;
    QVERIFY(BConfigExporter::exportToArchive(sampleProfile(), dir.filePath(given)));
    QVERIFY(QFileInfo::exists(dir.filePath(written)));
    QCOMPARE(QDir(dir.path()).entryList(QDir::Files), QStringList{written});
}

void BConfigExporterTest::archiveIsOwnerOnly()
{
#ifdef Q_OS_WIN
    QSKIP("Permission bits do not protect a file on Windows (its directory's ACL does)");
#else
    QTemporaryDir dir;
    const QString out = dir.filePath("export.zip");
    QVERIFY(BConfigExporter::exportToArchive(sampleProfile(), out));
    const QFileDevice::Permissions perms = QFileInfo(out).permissions();
    QVERIFY(perms & QFileDevice::ReadOwner);
    QVERIFY(perms & QFileDevice::WriteOwner);
    QVERIFY(!(perms & (QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ExeGroup
                       | QFileDevice::ReadOther | QFileDevice::WriteOther | QFileDevice::ExeOther)));
#endif
}

void BConfigExporterTest::overwritesExistingArchive()
{
    QTemporaryDir dir;
    const QString out = dir.filePath("export.zip");
    {
        QFile old(out);
        QVERIFY(old.open(QIODevice::WriteOnly));
        old.write(QByteArray(100000, 'x'));   // longer than the archive
        old.close();
        QVERIFY(old.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner
                                   | QFileDevice::ReadGroup | QFileDevice::ReadOther));
    }
    QVERIFY(BConfigExporter::exportToArchive(sampleProfile(), out));
    QVERIFY(QFileInfo(out).size() < 100000);   // truncated, not written over the old bytes
    QCOMPARE(readZip(out).size(), 3);
#ifndef Q_OS_WIN
    QVERIFY(!(QFileInfo(out).permissions() & (QFileDevice::ReadGroup | QFileDevice::ReadOther)));
#endif
}

void BConfigExporterTest::unwritablePathFails()
{
    QTemporaryDir dir;
    const QString out = dir.filePath("missing/subdir/export.zip");
    QVERIFY(!BConfigExporter::exportToArchive(sampleProfile(), out));
    QVERIFY(!BConfigExporter::lastError().isEmpty());
    QVERIFY(BConfigExporter::lastError().contains(out));
    QVERIFY(!QFileInfo::exists(out));

    // A directory where the file should be
    QVERIFY(QDir(dir.path()).mkdir("taken.zip"));
    QVERIFY(!BConfigExporter::exportToArchive(sampleProfile(), dir.filePath("taken.zip")));
    QVERIFY(!BConfigExporter::lastError().isEmpty());
}

void BConfigExporterTest::lastErrorClearedBySuccess()
{
    QTemporaryDir dir;
    QVERIFY(!BConfigExporter::exportToArchive(sampleProfile(), dir.filePath("no/such/dir/x.zip")));
    QVERIFY(!BConfigExporter::lastError().isEmpty());
    QVERIFY(BConfigExporter::exportToArchive(sampleProfile(), dir.filePath("ok.zip")));
    QVERIFY(BConfigExporter::lastError().isEmpty());
}

void BConfigExporterTest::grpcKeysOnlyInOconsole()
{
    QTemporaryDir dir;
    BConnectionProfile p = sampleProfile();
    p.transport = BConnectionProfile::Transport::Grpc;
    p.grpcPort = 10101;
    p.grpcCertFingerprint = QStringLiteral("ABCDEF0123");
    const QString out = dir.filePath("export.zip");
    QVERIFY(BConfigExporter::exportToArchive(p, out));

    const QMap<QString, QByteArray> files = readZip(out);
    // C++ bconsole refuses unknown directives
    QVERIFY(!files.value(kBconsole).contains("Grpc"));
    const QByteArray oconsole = files.value(kOconsole);
    QVERIFY(oconsole.contains("Grpc = yes"));
    QVERIFY(oconsole.contains("Grpc Port = 10101"));
    QVERIFY(oconsole.contains("Grpc Fingerprint = abcdef0123"));
    // In the Director resource, not in the Console resource after it
    QVERIFY(oconsole.indexOf("Grpc Port") < oconsole.indexOf("Console {"));
}

void BConfigExporterTest::roundTripThroughImport()
{
    QTemporaryDir dir;
    BConnectionProfile p = sampleProfile();
    p.description = QStringLiteral("Operators of the night shift");
    p.profile = QStringLiteral("night");
    p.aclJob = QStringList{QStringLiteral("Backup-.*"), QStringLiteral("Restore")};
    p.aclWhere = QStringList{QStringLiteral("/tmp/bareos-restores")};
    const QString out = dir.filePath("export.zip");
    QVERIFY(BConfigExporter::exportToArchive(p, out, BConfigExporter::ExportFull));

    // The import path of the application reads the Director part back
    const BZipImport::Result read = BZipImport::readConfigFiles(out);
    QVERIFY2(read.ok, qPrintable(read.error));
    QVERIFY2(read.skipped.isEmpty(), qPrintable(read.skipped.join('\n')));

    BConfigParser parser;
    QVERIFY2(parser.parseZipArchive(out), qPrintable(parser.lastError()));
    const BConfigResource console = parser.resource(QStringLiteral("Console"), p.consoleName);
    QCOMPARE(console.name(), p.consoleName);
    QCOMPARE(console.simpleValue(QStringLiteral("Password")), "[md5]" + p.passwordHash);
    QCOMPARE(console.simpleValue(QStringLiteral("Profile")), QStringLiteral("night"));
    QCOMPARE(console.simpleValue(QStringLiteral("Description")), p.description);

    const BConfigResource profile = parser.resource(QStringLiteral("Profile"), QStringLiteral("night"));
    QCOMPARE(profile.name(), QStringLiteral("night"));
    QCOMPARE(profile.listValue(QStringLiteral("Job ACL")), p.aclJob);
    QCOMPARE(profile.listValue(QStringLiteral("Where ACL")), p.aclWhere);

    // bconsole.conf is outside bareos-dir.d: parsed on its own
    const QMap<QString, QByteArray> files = readZip(out);
    BConfigParser client;
    QVERIFY2(client.parseString(QString::fromUtf8(files.value(kBconsole))), qPrintable(client.lastError()));
    const BConfigResource director = client.resource(QStringLiteral("Director"), p.directorName);
    QCOMPARE(director.simpleValue(QStringLiteral("Address")), p.host);
    QCOMPARE(director.simpleValue(QStringLiteral("DIRport")), QStringLiteral("9101"));
    QCOMPARE(client.resource(QStringLiteral("Console"), p.consoleName).name(), p.consoleName);
}

void BConfigExporterTest::windowsServerPaths()
{
    QTemporaryDir dir;
    BConnectionProfile p = sampleProfile();
    p.serverPlatform = QStringLiteral("windows");
    p.tlsUsePSK = false;
    const QString out = dir.filePath("export.zip");
    QVERIFY(BConfigExporter::exportToArchive(p, out, BConfigExporter::ExportFull));

    const QMap<QString, QByteArray> files = readZip(out);
    BConfigParser parser;
    QVERIFY2(parser.parseString(QString::fromUtf8(files.value(kDirector))), qPrintable(parser.lastError()));
    // Bareos takes a backslash in a quoted string as an escape: the file
    // must hold doubled backslashes for the Director to read single ones
    QCOMPARE(parser.resource(QStringLiteral("Director"), p.directorName).simpleValue(QStringLiteral("QueryFile")),
             QStringLiteral("C:\\Program Files\\Bareos\\scripts\\query.sql"));
}

void BConfigExporterTest::defaultExtensionIsZip()
{
    QCOMPARE(BConfigExporter::defaultExtension(), QStringLiteral(".zip"));
}

void BConfigExporterTest::namesThatAreNoFileNameRefused_data()
{
    QTest::addColumn<QString>("consoleName");
    QTest::addColumn<QString>("profileName");
    // The archive is extracted into the Director's root directory: a name
    // with a path in it would write a file outside bareos-dir.d/console
    // (or into a subdirectory the Director does not read)
    QTest::newRow("console leaves the tree") << "../../../../root/.ssh/authorized_keys" << "";
    QTest::newRow("console in a subdirectory") << "team/admin" << "";
    QTest::newRow("console with backslash") << "..\\..\\evil" << "";
    QTest::newRow("console empty") << "" << "";
    QTest::newRow("console with newline") << "admin\nother" << "";
    QTest::newRow("profile leaves the tree") << "admin" << "../../../etc/cron.d/job";
    QTest::newRow("profile with backslash") << "admin" << "ops\\night";
}

void BConfigExporterTest::namesThatAreNoFileNameRefused()
{
    QFETCH(QString, consoleName);
    QFETCH(QString, profileName);
    QTemporaryDir dir;
    BConnectionProfile p = sampleProfile();
    p.consoleName = consoleName;
    p.profile = profileName;
    p.aclJob = QStringList{QStringLiteral("*all*")};
    const QString out = dir.filePath("export.zip");

    QVERIFY(!BConfigExporter::exportToArchive(p, out, BConfigExporter::ExportWithProfile));
    QVERIFY(!BConfigExporter::lastError().isEmpty());
    QVERIFY(!QFileInfo::exists(out));   // nothing half written
}

void BConfigExporterTest::profileNameOnlyCheckedWhenWritten()
{
    // Console only: the Profile is a name in the Console resource, no file
    QTemporaryDir dir;
    BConnectionProfile p = sampleProfile();
    p.profile = QStringLiteral("ops/night");
    p.aclJob = QStringList{QStringLiteral("*all*")};
    QVERIFY2(BConfigExporter::exportToArchive(p, dir.filePath("a.zip"), BConfigExporter::ExportConsoleOnly),
             qPrintable(BConfigExporter::lastError()));
    // Bareos names may contain dots, colons, blanks and dashes
    p.consoleName = QStringLiteral("admin.console: 2-b_c");
    p.profile.clear();
    QVERIFY2(BConfigExporter::exportToArchive(p, dir.filePath("b.zip"), BConfigExporter::ExportWithProfile),
             qPrintable(BConfigExporter::lastError()));
    QVERIFY(readZip(dir.filePath("b.zip")).contains(
        QStringLiteral("etc/bareos/bareos-dir.d/console/admin.console: 2-b_c.conf")));
}

void BConfigExporterTest::quotesAndBackslashesReadBackUnchanged()
{
    // Bareos reads a backslash in a quoted string as an escape
    // (lex_quoted_string in core/src/lib/lex.cc): an ACL regex or a Windows
    // path must reach the Director as the user typed it
    QTemporaryDir dir;
    BConnectionProfile p = sampleProfile();
    p.description = QStringLiteral("Say \"hi\" \\ bye");
    p.aclJob = QStringList{QStringLiteral("Backup\\.daily"), QStringLiteral("a\\\\b")};
    p.aclWhere = QStringList{QStringLiteral("C:\\restore")};
    p.tlsUsePSK = false;
    p.tlsCaCertFile = QStringLiteral("C:\\certs\\ca.pem");
    p.tlsAllowedCn = QStringList{QStringLiteral("cn \"quoted\"")};
    const QString out = dir.filePath("export.zip");
    QVERIFY2(BConfigExporter::exportToArchive(p, out, BConfigExporter::ExportFull),
             qPrintable(BConfigExporter::lastError()));

    const QMap<QString, QByteArray> files = readZip(out);
    BConfigParser parser;
    QVERIFY2(parser.parseString(QString::fromUtf8(files.value(kConsole))), qPrintable(parser.lastError()));
    const BConfigResource console = parser.resource(QStringLiteral("Console"), p.consoleName);
    QCOMPARE(console.simpleValue(QStringLiteral("Description")), p.description);
    QCOMPARE(console.listValue(QStringLiteral("Job ACL")), p.aclJob);
    QCOMPARE(console.listValue(QStringLiteral("Where ACL")), p.aclWhere);
    QCOMPARE(console.simpleValue(QStringLiteral("TLS CA Certificate File")), p.tlsCaCertFile);
    QCOMPARE(console.listValue(QStringLiteral("TLS Allowed CN")), p.tlsAllowedCn);

    const QString profileFile = QStringLiteral("etc/bareos/bareos-dir.d/profile/admin-console-profile.conf");
    BConfigParser profileParser;
    QVERIFY2(profileParser.parseString(QString::fromUtf8(files.value(profileFile))),
             qPrintable(profileParser.lastError()));
    const BConfigResource profile =
        profileParser.resource(QStringLiteral("Profile"), QStringLiteral("admin-console-profile"));
    QCOMPARE(profile.simpleValue(QStringLiteral("Description")), p.description);
    QCOMPARE(profile.listValue(QStringLiteral("Job ACL")), p.aclJob);

    BConfigParser client;
    QVERIFY2(client.parseString(QString::fromUtf8(files.value(kBconsole))), qPrintable(client.lastError()));
    QCOMPARE(client.resource(QStringLiteral("Director"), p.directorName)
                 .simpleValue(QStringLiteral("TLS CA Certificate File")),
             p.tlsCaCertFile);
}

void BConfigExporterTest::quoteInValueAddsNoDirective()
{
    QTemporaryDir dir;
    BConnectionProfile p = sampleProfile();
    p.description = QStringLiteral("x\" Command ACL = \"*all*");
    const QString out = dir.filePath("export.zip");
    QVERIFY2(BConfigExporter::exportToArchive(p, out), qPrintable(BConfigExporter::lastError()));

    BConfigParser parser;
    QVERIFY2(parser.parseString(QString::fromUtf8(readZip(out).value(kConsole))), qPrintable(parser.lastError()));
    const BConfigResource console = parser.resource(QStringLiteral("Console"), p.consoleName);
    QVERIFY(!console.hasKey(QStringLiteral("Command ACL")));
    QCOMPARE(console.simpleValue(QStringLiteral("Description")), p.description);
}

void BConfigExporterTest::controlCharacterRefused_data()
{
    // A control character cannot be escaped in Bareos resource text
    // (BQuote); a newline would end the value and start a directive
    QTest::addColumn<int>("field");
    QTest::newRow("description") << 0;
    QTest::newRow("host") << 1;
    QTest::newRow("director name") << 2;
    QTest::newRow("acl entry") << 3;
    QTest::newRow("tls file") << 4;
    QTest::newRow("allowed cn") << 5;
}

void BConfigExporterTest::controlCharacterRefused()
{
    QFETCH(int, field);
    const QString bad = QStringLiteral("x\"\n  Command ACL = \"*all*");
    BConnectionProfile p = sampleProfile();
    p.tlsUsePSK = false;
    switch (field) {
    case 0: p.description = bad; break;
    case 1: p.host = bad; break;
    case 2: p.directorName = bad; break;
    case 3: p.aclJob = QStringList{QStringLiteral("ok"), bad}; break;
    case 4: p.tlsKeyFile = bad; break;
    case 5: p.tlsAllowedCn = QStringList{bad}; break;
    }
    QTemporaryDir dir;
    const QString out = dir.filePath("export.zip");
    QVERIFY(!BConfigExporter::exportToArchive(p, out, BConfigExporter::ExportFull));
    QVERIFY(!BConfigExporter::lastError().isEmpty());
    QVERIFY(!QFileInfo::exists(out));
}

QTEST_GUILESS_MAIN(BConfigExporterTest)
#include "bconfigexporter_test.moc"
