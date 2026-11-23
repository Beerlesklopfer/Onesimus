// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bfilesetdocument_test.cpp
 * @brief FileSet wizard model: the text round trip keeps every directive,
 *        the generated text has the Bareos 25 form (no block inside Options,
 *        no empty value, only Options keywords inside Options), the
 *        templates, the "show filesets" JSON, the options form
 *
 * With ONESIMUS_FILESET_DUMP=<dir> every generated text is also written
 * there, to check it with bareos-dir -t and odir -t by hand.
 */

#include "jobs/fileset/bfilesetdocument.h"
#include "bquote.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QtTest>

namespace {

QString norm(const QString &key)
{
    QString out;
    for (const QChar c : key)
        if (!c.isSpace() && c != QLatin1Char('_'))
            out.append(c.toLower());
    return out;
}

QString value(QString v)
{
    v = v.trimmed();
    if (v.size() >= 2 && v.startsWith(QLatin1Char('"')) && v.endsWith(QLatin1Char('"')))
        v = BQuote::unescape(v.mid(1, v.size() - 2));
    const QString l = v.toLower();
    if (l == QLatin1String("true")) return QStringLiteral("yes");
    if (l == QLatin1String("false")) return QStringLiteral("no");
    return v;
}

// Line based reader for well-formed resource text (one directive per line,
// '#' comments outside quotes): every directive as "path/key=value", an
// Options block as one sorted entry of its own, so that the order of the
// Options blocks does not count
struct Flat {
    QStringList entries;
    QStringList optionsKeys;   ///< Keys found inside Options blocks
    int blocksInOptions = 0;
    int emptyValues = 0;
};

QString stripComment(const QString &line)
{
    bool inQuote = false;
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == QLatin1Char('\\') && inQuote) { ++i; continue; }
        if (line[i] == QLatin1Char('"')) inQuote = !inQuote;
        if (line[i] == QLatin1Char('#') && !inQuote) return line.left(i);
    }
    return line;
}

Flat flatten(const QString &text)
{
    Flat flat;
    QStringList path;
    QStringList options;
    int includeIndex = -1;
    for (const QString &raw : text.split(QLatin1Char('\n'))) {
        const QString line = stripComment(raw).trimmed();
        if (line.isEmpty()) continue;
        if (line.endsWith(QLatin1Char('{'))) {
            const QString key = norm(line.chopped(1));
            if (!path.isEmpty() && path.last() == QLatin1String("options"))
                ++flat.blocksInOptions;
            if (key == QLatin1String("include"))
                ++includeIndex;
            path.append(key == QLatin1String("include") ? QStringLiteral("include%1").arg(includeIndex) : key);
            if (key == QLatin1String("options"))
                options.clear();
            continue;
        }
        if (line == QLatin1String("}")) {
            if (!path.isEmpty() && path.last() == QLatin1String("options")) {
                options.sort();
                path.removeLast();
                flat.entries.append(path.join(QLatin1Char('/')) + QStringLiteral("/options{") + options.join(QLatin1Char(';')) + QLatin1Char('}'));
            } else if (!path.isEmpty()) {
                path.removeLast();
            }
            continue;
        }
        const int eq = line.indexOf(QLatin1Char('='));
        const QString key = norm(line.left(eq));
        const QString v = value(line.mid(eq + 1));
        if (v.isEmpty()) ++flat.emptyValues;
        if (!path.isEmpty() && path.last() == QLatin1String("options")) {
            options.append(key + QLatin1Char('=') + v);
            flat.optionsKeys.append(key);
        } else {
            flat.entries.append(path.join(QLatin1Char('/')) + QLatin1Char('/') + key + QLatin1Char('=') + v);
        }
    }
    flat.entries.sort();
    return flat;
}

void dump(const QString &name, const QString &text)
{
    const QString dir = qEnvironmentVariable("ONESIMUS_FILESET_DUMP");
    if (dir.isEmpty()) return;
    QDir().mkpath(dir);
    QFile f(QDir(dir).filePath(name + QStringLiteral(".conf")));
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        f.write(text.toUtf8());
}

const char *kFull =
    "FileSet {\n"
    "  Name = \"Full Set\"\n"
    "  Description = \"Say \\\"hi\\\" to C:\\\\data\"\n"
    "  Enable VSS = no\n"
    "  Ignore FileSet Changes = yes   # no new Full after a change\n"
    "  Include {\n"
    "    Options {\n"
    "      Signature = MD5\n"
    "      Compression = GZIP6\n"
    "      One FS = no\n"
    "      FS Type = ext4\n"
    "      FS Type = xfs\n"
    "      Strip Path = 2\n"
    "      Wild = \"*.keep\"\n"
    "    }\n"
    "    Options {\n"
    "      Exclude = yes\n"
    "      Regex Dir = \"^/home/[^/]+/\\\\.cache$\"\n"
    "      Wild File = \"*.tmp\"\n"
    "      Wild Dir = \"*/tmp\"\n"
    "      Wild = \"/home/x/core\"\n"
    "    }\n"
    "    Options {\n"
    "      Signature = SHA1\n"
    "      Compression = LZ4\n"
    "      Wild = \"*.iso\"\n"
    "    }\n"
    "    File = \"/home\"\n"
    "    File = \"/srv/data dir\"\n"
    "    Plugin = \"python:module_name=bareos-fd-local-fileset:filename=/etc/bareos/list\"\n"
    "    Exclude Dir Containing = \".nobackup\"\n"
    "  }\n"
    "  Include {\n"
    "    File = /etc\n"
    "  }\n"
    "  Exclude {\n"
    "    File = \"/home/x/.cache\"\n"
    "  }\n"
    "}\n";

} // namespace

class BFileSetDocumentTest : public QObject
{
    Q_OBJECT

private slots:
    void textRoundTripKeepsEveryDirective()
    {
        BFileSetDocument doc;
        QString error;
        QVERIFY2(doc.loadFromConfigText(QString::fromUtf8(kFull), &error), qPrintable(error));
        QCOMPARE(doc.name(), QStringLiteral("Full Set"));
        QCOMPARE(doc.description(), QStringLiteral("Say \"hi\" to C:\\data"));
        QVERIFY(!doc.enableVss());
        QVERIFY(doc.ignoreFileSetChanges());
        QCOMPARE(doc.includeBlockCount(), 2);

        const QString out = doc.toConfigText(&error);
        QVERIFY2(!out.isEmpty(), qPrintable(error));
        dump(QStringLiteral("roundtrip"), out);

        const Flat before = flatten(QString::fromUtf8(kFull));
        const Flat after = flatten(out);
        QCOMPARE(after.entries, before.entries);

        // And once more: the generated text is stable
        BFileSetDocument again;
        QVERIFY(again.loadFromConfigText(out, &error));
        QCOMPARE(again.toConfigText(), out);
    }

    void excludePatternsStayExcludes()
    {
        BFileSetDocument doc;
        QVERIFY(doc.loadFromConfigText(QString::fromUtf8(kFull)));
        const BFileSetDocument::IncludeBlock *block = doc.includeBlock(0);
        QCOMPARE(block->excludeWildFileModel->items(), QStringList{QStringLiteral("*.tmp")});
        QCOMPARE(block->excludePatternsModel->items(), QStringList{QStringLiteral("*/tmp")});
        QCOMPARE(block->excludeFilesModel->items(), QStringList{QStringLiteral("/home/x/core")});
        QCOMPARE(block->excludeOptionsExtra.size(), 1);
        QCOMPARE(norm(block->excludeOptionsExtra.first().key), QStringLiteral("regexdir"));
        // The include pattern of the first block is not an exclude
        QCOMPARE(block->options.value(QStringLiteral("wild")).toStringList(), QStringList{QStringLiteral("*.keep")});
        QCOMPARE(block->extraOptionsBlocks.size(), 1);
        QCOMPARE(block->plugins.size(), 1);
        QCOMPARE(block->excludeDirContaining, QStringList{QStringLiteral(".nobackup")});
    }

    void generatedTextHasTheBareosForm()
    {
        BFileSetDocument doc;
        doc.setName(QStringLiteral("New Set"));
        BFileSetDocument::IncludeBlock *block = doc.addIncludeBlock();
        block->pathsModel->setItems({QStringLiteral("/home")});
        block->excludePatternsModel->setItems({QStringLiteral("*/.cache")});
        block->excludeWildFileModel->setItems({QStringLiteral("*.tmp")});
        // What the options form sends: empty patterns, VSS, the marker file
        doc.setFormOption(block, QStringLiteral("regex"), QString());
        doc.setFormOption(block, QStringLiteral("wild"), QString());
        doc.setFormOption(block, QStringLiteral("vss"), true);
        doc.setFormOption(block, QStringLiteral("exclude_dir_containing"), QStringLiteral(".nobackup"));
        doc.setFormOption(block, QStringLiteral("strip_path"), 0);
        doc.setFormOption(block, QStringLiteral("compression"), QStringLiteral("None"));
        doc.setFormOption(block, QStringLiteral("acl"), false);

        const QString out = doc.toConfigText();
        dump(QStringLiteral("generated"), out);
        const Flat flat = flatten(out);
        QCOMPARE(flat.blocksInOptions, 0);
        QCOMPARE(flat.emptyValues, 0);
        for (const QString &key : flat.optionsKeys)
            QVERIFY2(BFileSetDocument::isOptionKeyword(key), qPrintable(key));
        QVERIFY(!out.contains(QStringLiteral("Compression")));
        QVERIFY(out.contains(QStringLiteral("ACL Support = no")));
        QVERIFY(out.contains(QStringLiteral("    Exclude Dir Containing = \".nobackup\"\n")));
        QVERIFY(out.contains(QStringLiteral("      Exclude = yes\n")));
        QVERIFY(out.contains(QStringLiteral("  Name = \"New Set\"\n")));
        QVERIFY(!out.contains(QStringLiteral("Snapshot")));
    }

    void formEditsOnlyItsOption()
    {
        BFileSetDocument doc;
        QVERIFY(doc.loadFromConfigText(QString::fromUtf8(kFull)));
        BFileSetDocument::IncludeBlock *block = doc.includeBlock(0);
        const QMap<QString, QVariant> form = BFileSetDocument::formOptions(block);
        QCOMPARE(form.value(QStringLiteral("signature")).toString(), QStringLiteral("MD5"));
        QCOMPARE(form.value(QStringLiteral("strip_path")).toInt(), 2);
        QCOMPARE(form.value(QStringLiteral("exclude_dir_containing")).toString(), QStringLiteral(".nobackup"));

        doc.setFormOption(block, QStringLiteral("signature"), QStringLiteral("SHA256"));
        const QString out = doc.toConfigText();
        QVERIFY(out.contains(QStringLiteral("Signature = SHA256")));
        QVERIFY(out.contains(QStringLiteral("FS Type = \"xfs\"")));       // untouched list kept
        QVERIFY(out.contains(QStringLiteral("Regex Dir = ")));
    }

    void controlCharacterRefused()
    {
        BFileSetDocument doc;
        doc.setName(QStringLiteral("x"));
        BFileSetDocument::IncludeBlock *block = doc.addIncludeBlock();
        block->pathsModel->setItems({QStringLiteral("/a\nFile = /etc/shadow")});
        QString error;
        QVERIFY(doc.toConfigText(&error).isEmpty());
        QVERIFY(!error.isEmpty());
        QVERIFY(!doc.validate().valid);
    }

    void unreadableTextLeavesDocument()
    {
        BFileSetDocument doc;
        doc.setName(QStringLiteral("kept"));
        QString error;
        QVERIFY(!doc.loadFromConfigText(QStringLiteral("FileSet {\n  Name = x\n  Include {\n"), &error));
        QVERIFY(!error.isEmpty());
        QVERIFY(!doc.loadFromConfigText(QStringLiteral("@/etc/bareos/x.conf\n"), &error));
        QCOMPARE(doc.name(), QStringLiteral("kept"));
    }

    void nameRule()
    {
        BFileSetDocument doc;
        BFileSetDocument::IncludeBlock *block = doc.addIncludeBlock();
        block->pathsModel->setItems({QStringLiteral("/")});
        doc.setName(QStringLiteral("Linux All"));
        QVERIFY(doc.validate().valid);
        doc.setName(QStringLiteral("bad\"name"));
        QVERIFY(!doc.validate().valid);
    }

    void templates_data()
    {
        QTest::addColumn<QString>("path");
        const QDir dir(QStringLiteral(ONESIMUS_SOURCE_DIR "/resources/templates/filesets"));
        const QStringList files = dir.entryList({QStringLiteral("*.json")}, QDir::Files, QDir::Name);
        QVERIFY(files.size() >= 20);
        for (const QString &f : files)
            QTest::newRow(qPrintable(f)) << dir.filePath(f);
    }

    void templates()
    {
        QFETCH(QString, path);
        QFile file(path);
        QVERIFY(file.open(QIODevice::ReadOnly));
        const QJsonObject tmpl = QJsonDocument::fromJson(file.readAll()).object();
        const QJsonObject fileset = tmpl.value(QStringLiteral("fileset")).toObject();
        QVERIFY2(!QJsonDocument(fileset).toJson().contains("\"Files\""), "a template uses \"Files\"");

        BFileSetDocument doc;
        doc.loadFromTemplate(fileset);
        QCOMPARE(doc.name(), fileset.value(QStringLiteral("Name")).toString());
        const BFileSetDocument::IncludeBlock *block = doc.includeBlock(0);
        QVERIFY(block);
        // Every File of the template arrives
        const QJsonValue files = fileset.value(QStringLiteral("Include")).toObject().value(QStringLiteral("File"));
        QCOMPARE(block->pathsModel->rowCount(), files.isArray() ? files.toArray().size() : (files.isString() ? 1 : 0));

        QString error;
        const QString out = doc.toConfigText(&error);
        QVERIFY2(!out.isEmpty(), qPrintable(error));
        dump(QFileInfo(path).baseName(), out);
        const Flat flat = flatten(out);
        QCOMPARE(flat.blocksInOptions, 0);
        QCOMPARE(flat.emptyValues, 0);
        for (const QString &key : flat.optionsKeys)
            QVERIFY2(BFileSetDocument::isOptionKeyword(key), qPrintable(key));
        // Accurate and Verify take option letters, not yes/no (inc_conf.cc)
        QVERIFY2(!out.contains(QStringLiteral("Accurate = yes")) && !out.contains(QStringLiteral("Verify = yes")),
                 qPrintable(out));

        BFileSetDocument again;
        QVERIFY2(again.loadFromConfigText(out, &error), qPrintable(error));
        QCOMPARE(again.toConfigText(), out);
    }

    void showFilesetsJson()
    {
        // The shape of "show filesets" in API 2 (dird_conf.cc
        // FilesetResource::PrintConfig): keys in lower case
        const QByteArray json = R"({
            "name": "LinuxAll", "description": "All",
            "include": [ {
                "options": [ { "signature": "MD5", "onefs": false, "fstype": ["ext4", "xfs"] },
                             { "exclude": true, "regexdir": ["^/proc$"], "wilddir": ["*/tmp"] } ],
                "file": ["/"], "plugin": ["python:module_name=x"], "excludedircontaining": [".nobackup"] } ],
            "exclude": [ { "file": ["/proc", "/sys"] } ]
        })";
        BFileSetDocument doc;
        doc.loadFromBareosJson(QJsonDocument::fromJson(json).object());
        const BFileSetDocument::IncludeBlock *block = doc.includeBlock(0);
        QCOMPARE(block->excludePatternsModel->items(), QStringList{QStringLiteral("*/tmp")});
        QCOMPARE(block->excludeOptionsExtra.size(), 1);   // the exclude regex stays an exclude
        QCOMPARE(block->options.value(QStringLiteral("onefs")).toBool(), false);
        QCOMPARE(block->options.value(QStringLiteral("fstype")).toStringList(),
                 (QStringList{QStringLiteral("ext4"), QStringLiteral("xfs")}));
        QCOMPARE(block->plugins.size(), 1);
        QCOMPARE(doc.excludePathModel()->items(), (QStringList{QStringLiteral("/proc"), QStringLiteral("/sys")}));
        const QString out = doc.toConfigText();
        dump(QStringLiteral("showjson"), out);
        QVERIFY(out.contains(QStringLiteral("Regex Dir = \"^/proc$\"")));
    }
};

QTEST_GUILESS_MAIN(BFileSetDocumentTest)
#include "bfilesetdocument_test.moc"
