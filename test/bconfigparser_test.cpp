// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

/**
 * @file bconfigparser_test.cpp
 * @brief The configuration parser read as Bareos 25 reads it (P1-25)
 *
 * Directive names without blanks and case, ';' as end of a directive,
 * quoted strings, lists, repeated blocks, @include (followed by
 * parseFile() only, never a command), and malicious input: deep nesting,
 * include loops, huge values.
 *
 * The expected values of valuesAsBareosReadsThem() and quotedStrings() are
 * what bareos-dir 25.1.3 printed for the same text with "-t --xc" (export
 * of the parsed configuration), run on 2026-11-23.
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QElapsedTimer>

#include "config/bconfigparser.h"

namespace {

void writeFile(const QString &path, const QByteArray &text)
{
    QDir().mkpath(QFileInfo(path).absolutePath());
    QFile f(path);
    QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Truncate));
    f.write(text);
}

QString nested(int depth)
{
    // A resource body (level 1) and depth - 1 blocks inside it
    QString text = QStringLiteral("FileSet {\n  Name = n\n");
    for (int i = 1; i < depth; ++i)
        text += QStringLiteral("b = {\n");
    for (int i = 1; i < depth; ++i)
        text += QStringLiteral("}\n");
    return text + QStringLiteral("}\n");
}

} // namespace

class BConfigParserTest : public QObject
{
    Q_OBJECT

private slots:
    void keysWithoutBlanksAndCase();
    void valuesAsBareosReadsThem();
    void quotedStrings();
    void semicolonEndsADirective();
    void valueMustStartOnItsLine();
    void numbersWithUnits();
    void repeatedBlocksAreLists();
    void resourceWithoutNameIsReported();
    void syntaxErrorsNameTheLine();
    void nestingDepthLimit();
    void deepNestingFailsFast();
    void includeInTextIsReported();
    void includeFollowedByParseFile();
    void includeNeverRunsACommand();
    void missingIncludeIsAnError();
    void includeLoopIsAnError();
    void includeDepthLimit();
    void hugeValue();
    void tooMuchText();
    void utf16IsRefused();
};

void BConfigParserTest::keysWithoutBlanksAndCase()
{
    BConfigParser parser;
    QVERIFY(parser.parseString(QStringLiteral("Console {\n"
                                              "  Name = admin\n"
                                              "  TLS Enable = yes\n"
                                              "  Command ACL = status\n"
                                              "  CommandACL = run\n"
                                              "  commandacl = .api\n"
                                              "}\n"
                                              "Director {\n  Name = dir\n  DIRport = 9101\n  FD Port = 1\n}\n")));
    const BConfigResource con = parser.resource("Console", "admin");
    QCOMPARE(con.simpleValue("tls enable"), QStringLiteral("yes"));
    QCOMPARE(con.simpleValue("TLSEnable"), QStringLiteral("yes"));
    // One directive, three spellings: one list (StoreAlistStr() appends)
    QCOMPARE(con.listValue("commandacl"), QStringList({"status", "run", ".api"}));
    QCOMPARE(con.listValue("Command ACL"), QStringList({"status", "run", ".api"}));
    const BConfigResource dir = parser.resource("Director", "dir");
    QCOMPARE(dir.simpleValue("dirport"), QStringLiteral("9101"));
    QCOMPARE(dir.simpleValue("Dir Port"), QStringLiteral("9101"));
    QCOMPARE(dir.simpleValue("fdport"), QStringLiteral("1"));
    QVERIFY(dir.hasKey("FD Port"));
}

void BConfigParserTest::valuesAsBareosReadsThem()
{
    // bareos-dir --xc: Name "mytest-con", CommandAcl "status", "listx", ".api", "run"
    BConfigParser parser;
    QVERIFY(parser.parseString(QStringLiteral("Console {\n"
                                              "  Name = my test-con sole\n"
                                              "  Password = \"secret\"\n"
                                              "  Command ACL = status, \"listx\", .api; JobACL = *all*\n"
                                              "  Command   ACL = run\n"
                                              "}\n"
                                              "Console { Name = third; Password = y\n"
                                              "  Description = a-b c\n"
                                              "}\n")));
    QCOMPARE(parser.resources().size(), 2);
    const BConfigResource con = parser.resources().at(0);
    QCOMPARE(con.name(), QStringLiteral("mytest-con"));
    QCOMPARE(con.listValue("commandacl"), QStringList({"status", "listx", ".api", "run"}));
    QCOMPARE(con.simpleValue("jobacl"), QStringLiteral("*all*"));
    QCOMPARE(con.simpleValue("password"), QStringLiteral("secret"));
    // bareos-dir --xc: Description "a-b"
    QCOMPARE(parser.resources().at(1).simpleValue("description"), QStringLiteral("a-b"));
}

void BConfigParserTest::quotedStrings()
{
    // bareos-dir --xc: "anb"c\dcont" and "firstnext line": a backslash takes
    // the next character as it is, quoted strings in a row are one
    BConfigParser parser;
    QVERIFY(parser.parseString(QStringLiteral("Console {\n"
                                              "  Name = a\n"
                                              "  Description = \"a\\nb\\\"c\\\\d\" \"cont\"\n"
                                              "}\n"
                                              "Console { Name = second; Password = x\n"
                                              "  Description = \"first\"\n"
                                              "     \"next line\"\n"
                                              "  Address = \"has # and ; and } inside\"\n"
                                              "}\n")));
    QCOMPARE(parser.resource("Console", "a").simpleValue("description"), QStringLiteral("anb\"c\\dcont"));
    const BConfigResource second = parser.resource("Console", "second");
    QCOMPARE(second.simpleValue("description"), QStringLiteral("firstnext line"));
    QCOMPARE(second.simpleValue("address"), QStringLiteral("has # and ; and } inside"));
    QCOMPARE(second.simpleValue("password"), QStringLiteral("x"));
}

void BConfigParserTest::semicolonEndsADirective()
{
    // Before: parsing ended after the first ';' at resource level
    BConfigParser parser;
    QVERIFY(parser.parseString(QStringLiteral("Director { Name = d; Address = h; Password = p; }\n"
                                              "Console { Name = c; Password = q }\n")));
    QCOMPARE(parser.resources().size(), 2);
    const BConfigResource dir = parser.resource("Director", "d");
    QCOMPARE(dir.simpleValue("address"), QStringLiteral("h"));
    QCOMPARE(dir.simpleValue("password"), QStringLiteral("p"));
    QCOMPARE(parser.resource("Console", "c").simpleValue("password"), QStringLiteral("q"));
}

void BConfigParserTest::valueMustStartOnItsLine()
{
    // bareos-dir: "expected a name, got BCT_EOL"
    BConfigParser parser;
    QVERIFY(!parser.parseString(QStringLiteral("Console {\n  Name =\n    \"x\"\n}\n"), "t.conf"));
    QVERIFY(parser.lastError().startsWith(QStringLiteral("t.conf:")));
    // A block may start on the next line (StoreAddresses() skips EOL)
    BConfigParser blocks;
    QVERIFY(blocks.parseString(QStringLiteral("Director {\n  Name = d\n  DirAddresses =\n  {\n"
                                              "    ipv4 = { addr = 1.2.3.4 }\n  }\n}\n")));
    QCOMPARE(blocks.resource("Director", "d").value("diraddresses").type(), BConfigValue::Block);
}

void BConfigParserTest::numbersWithUnits()
{
    // StoreTime()/StoreSize() join the parts after a number; other values
    // end at the first blank (ScanToEol())
    BConfigParser parser;
    QVERIFY(parser.parseString(QStringLiteral("Pool {\n  Name = p\n  Volume Retention = 30 days\n"
                                              "  Maximum Volume Bytes = 10 G # comment\n"
                                              "  Label Format = Full-\n}\n")));
    const BConfigResource pool = parser.resource("Pool", "p");
    QCOMPARE(pool.simpleValue("volumeretention"), QStringLiteral("30days"));
    QCOMPARE(pool.simpleValue("maximumvolumebytes"), QStringLiteral("10G"));
    QCOMPARE(pool.simpleValue("labelformat"), QStringLiteral("Full-"));
}

void BConfigParserTest::repeatedBlocksAreLists()
{
    BConfigParser parser;
    QVERIFY(parser.parseString(QStringLiteral(
        "Director {\n  Name = d\n"
        "  DirAddresses = {\n"
        "    ipv4 = { addr = 10.0.0.1; port = 9101; }\n"
        "    ipv4 = { addr = 10.0.0.2; port = 9102; }\n"
        "    ip = { addr = ::1 }\n"
        "  }\n}\n"
        "FileSet {\n  Name = fs\n"
        "  Include {\n"
        "    Options { Signature = XXH128 }\n"
        "    Options { Wild Dir = \"*/tmp\"; Exclude = yes }\n"
        "    File = /etc\n    File = \"/home/a b\"\n"
        "  }\n"
        "  Include { File = /srv }\n"
        "}\n")));
    const QMap<QString, BConfigValue> addrs = parser.resource("Director", "d").value("diraddresses").blockValue();
    QCOMPARE(addrs.value("ipv4").type(), BConfigValue::BlockList);
    const auto ipv4 = addrs.value("ipv4").blockListValue();
    QCOMPARE(ipv4.size(), 2);
    QCOMPARE(ipv4.at(0).value("addr").simpleValue(), QStringLiteral("10.0.0.1"));
    QCOMPARE(ipv4.at(1).value("port").simpleValue(), QStringLiteral("9102"));
    QCOMPARE(addrs.value("ip").blockValue().value("addr").simpleValue(), QStringLiteral("::1"));

    const BConfigValue include = parser.resource("FileSet", "fs").value("include");
    QCOMPARE(include.type(), BConfigValue::BlockList);
    QCOMPARE(include.blockListValue().size(), 2);
    const QMap<QString, BConfigValue> first = include.blockListValue().at(0);
    QCOMPARE(first.value("options").type(), BConfigValue::BlockList);
    QCOMPARE(first.value("options").blockListValue().at(1).value("wilddir").simpleValue(), QStringLiteral("*/tmp"));
    QCOMPARE(first.value("file").listValue(), QStringList({"/etc", "/home/a b"}));
}

void BConfigParserTest::resourceWithoutNameIsReported()
{
    BConfigParser parser;
    QVERIFY(parser.parseString(QStringLiteral("Console {\n  Password = x\n}\nConsole { Name = c }\n"), "t.conf"));
    QCOMPARE(parser.resources().size(), 1);
    QCOMPARE(parser.skippedEntries().size(), 1);
    QVERIFY(parser.skippedEntries().first().startsWith(QStringLiteral("t.conf:1:")));
}

void BConfigParserTest::syntaxErrorsNameTheLine()
{
    const struct { const char *text; const char *line; } cases[] = {
        {"Console {\n  Name = \"open\n}\n", ""},          // no closing quote: at the end
        {"Console {\n  Name = c\n", "t.conf:3"},           // no closing brace
        {"Console\n  Name = c\n}\n", "t.conf:2"},          // no opening brace
        {"Console {\n  Name c\n}\n", "t.conf:2"},          // no '='
        {"\"Console\" { Name = c }\n", "t.conf:1"},        // type is not an identifier
        {"Console {\n  Name = c\n  = x\n}\n", "t.conf:3"}, // '=' without a name
    };
    for (const auto &c : cases) {
        BConfigParser parser;
        QSignalSpy errors(&parser, &BConfigParser::parsingError);
        QVERIFY2(!parser.parseString(QString::fromUtf8(c.text), "t.conf"), c.text);
        QCOMPARE(errors.count(), 1);
        QVERIFY2(parser.lastError().startsWith(QStringLiteral("t.conf:")), qPrintable(parser.lastError()));
        if (*c.line)
            QVERIFY2(parser.lastError().startsWith(QString::fromUtf8(c.line) + ':'), qPrintable(parser.lastError()));
    }
}

void BConfigParserTest::nestingDepthLimit()
{
    BConfigParser ok;
    QVERIFY(ok.parseString(nested(BConfigParser::MaxNestingDepth)));
    QCOMPARE(ok.resources().size(), 1);

    BConfigParser tooDeep;
    QVERIFY(!tooDeep.parseString(nested(BConfigParser::MaxNestingDepth + 1)));
    QVERIFY(tooDeep.lastError().contains(QStringLiteral("32")));
    QVERIFY(tooDeep.resources().isEmpty());
}

void BConfigParserTest::deepNestingFailsFast()
{
    // 200000 opening braces: refused at level 33, no deep recursion
    QString text = QStringLiteral("FileSet {\n");
    text += QStringLiteral("b={").repeated(200000);
    QElapsedTimer timer;
    timer.start();
    BConfigParser parser;
    QVERIFY(!parser.parseString(text));
    QVERIFY(timer.elapsed() < 5000);
}

void BConfigParserTest::includeInTextIsReported()
{
    // Before: an @include ended parsing (with "true")
    BConfigParser parser;
    QVERIFY(parser.parseString(QStringLiteral("@/etc/bareos/bareos-dir.d/*/*.conf\n"
                                              "Director {\n  Name = d\n  @\"/etc/bareos/a b.inc\"\n"
                                              "  Password = p\n}\n"),
                               "bareos-dir.conf"));
    QCOMPARE(parser.resources().size(), 1);
    QCOMPARE(parser.resource("Director", "d").simpleValue("password"), QStringLiteral("p"));
    const QStringList skipped = parser.skippedEntries();
    QCOMPARE(skipped.size(), 2);
    QVERIFY(skipped.at(0).startsWith(QStringLiteral("bareos-dir.conf:1: @/etc/bareos/bareos-dir.d/*/*.conf")));
    QVERIFY(skipped.at(1).startsWith(QStringLiteral("bareos-dir.conf:4: @/etc/bareos/a b.inc")));
}

void BConfigParserTest::includeFollowedByParseFile()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString d = dir.path();
    writeFile(d + "/bareos-dir.d/console/a.conf", "Console { Name = a; Password = x }\n");
    writeFile(d + "/bareos-dir.d/director/d.conf",
              QStringLiteral("Director {\n  Name = d\n  @%1/inc/pw.inc\n}\n").arg(d).toUtf8());
    writeFile(d + "/inc/pw.inc", "Password = \"from include\"\n");
    writeFile(d + "/a b/c.conf", "Console { Name = c; Password = y }\n");
    writeFile(d + "/bareos-dir.conf",
              QStringLiteral("@%1/bareos-dir.d/*/*.conf\n@\"%1/a b/c.conf\"\n@%1/none/*.conf\n"
                             "@relative.conf\n").arg(d).toUtf8());

    BConfigParser parser;
    QVERIFY2(parser.parseFile(d + "/bareos-dir.conf"), qPrintable(parser.lastError()));
    QCOMPARE(parser.resources().size(), 3);
    QCOMPARE(parser.resource("Director", "d").simpleValue("password"), QStringLiteral("from include"));
    QVERIFY(!parser.resource("Console", "a").name().isEmpty());
    QVERIFY(!parser.resource("Console", "c").name().isEmpty());
    // A wildcard without a match is no error; a relative path is reported
    QCOMPARE(parser.skippedEntries().size(), 1);
    QVERIFY(parser.skippedEntries().first().contains(QStringLiteral("@relative.conf")));

    // parseDirectory() reads every *.conf on its own and follows nothing
    BConfigParser tree;
    QVERIFY(tree.parseDirectory(d + "/bareos-dir.d"));
    QCOMPARE(tree.resourcesByType("Console").size(), 1);
    QVERIFY(tree.resource("Director", "d").simpleValue("password").isEmpty());
    QCOMPARE(tree.skippedEntries().size(), 1);
}

void BConfigParserTest::includeNeverRunsACommand()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString marker = dir.filePath("ran");
    writeFile(dir.filePath("main.conf"),
              QStringLiteral("@\"|touch %1\"\nConsole { Name = c; Password = p }\n").arg(marker).toUtf8());
    BConfigParser parser;
    QVERIFY2(parser.parseFile(dir.filePath("main.conf")), qPrintable(parser.lastError()));
    QCOMPARE(parser.resources().size(), 1);
    QVERIFY(!QFile::exists(marker));
    QCOMPARE(parser.skippedEntries().size(), 1);
    QVERIFY(parser.skippedEntries().first().contains(QStringLiteral("|touch")));
}

void BConfigParserTest::missingIncludeIsAnError()
{
    // lex_open_file(): "Cannot open included config file"
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    writeFile(dir.filePath("main.conf"),
              QStringLiteral("@%1\nConsole { Name = c }\n").arg(dir.filePath("missing.conf")).toUtf8());
    BConfigParser parser;
    QVERIFY(!parser.parseFile(dir.filePath("main.conf")));
    QVERIFY(parser.lastError().contains(QStringLiteral("missing.conf")));
}

void BConfigParserTest::includeLoopIsAnError()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString a = dir.filePath("a.conf"), b = dir.filePath("b.conf"), self = dir.filePath("self.conf");
    writeFile(a, QStringLiteral("Console { Name = a }\n@%1\n").arg(b).toUtf8());
    writeFile(b, QStringLiteral("Console { Name = b }\n@%1\n").arg(a).toUtf8());
    writeFile(self, QStringLiteral("@%1\n").arg(self).toUtf8());

    QElapsedTimer timer;
    timer.start();
    BConfigParser parser;
    QVERIFY(!parser.parseFile(a));
    QVERIFY2(parser.lastError().contains(QStringLiteral("includes itself")), qPrintable(parser.lastError()));
    BConfigParser selfParser;
    QVERIFY(!selfParser.parseFile(self));
    QVERIFY(selfParser.lastError().contains(QStringLiteral("includes itself")));
    // The same file twice in a row is no loop
    const QString twice = dir.filePath("twice.conf"), part = dir.filePath("part.inc");
    writeFile(part, "Password = p\n");
    writeFile(twice, QStringLiteral("Console {\n Name = t\n @%1\n @%1\n}\n").arg(part).toUtf8());
    BConfigParser twiceParser;
    QVERIFY2(twiceParser.parseFile(twice), qPrintable(twiceParser.lastError()));
    QCOMPARE(twiceParser.resource("Console", "t").listValue("password"), QStringList({"p", "p"}));
    QVERIFY(timer.elapsed() < 5000);
}

void BConfigParserTest::includeDepthLimit()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const int files = BConfigParser::MaxIncludeDepth + 5;
    for (int i = 0; i < files; ++i) {
        const QByteArray next = i + 1 < files ? QStringLiteral("@%1\n").arg(dir.filePath(QString("f%1.conf").arg(i + 1))).toUtf8()
                                              : QByteArray("Console { Name = last }\n");
        writeFile(dir.filePath(QString("f%1.conf").arg(i)), next);
    }
    BConfigParser parser;
    QVERIFY(!parser.parseFile(dir.filePath("f0.conf")));
    QVERIFY2(parser.lastError().contains(QStringLiteral("deeper than 32")), qPrintable(parser.lastError()));

    // Exactly MaxIncludeDepth levels are read
    writeFile(dir.filePath(QString("f%1.conf").arg(BConfigParser::MaxIncludeDepth)), "Console { Name = last }\n");
    BConfigParser deepest;
    QVERIFY2(deepest.parseFile(dir.filePath("f0.conf")), qPrintable(deepest.lastError()));
    QCOMPARE(deepest.resources().size(), 1);
}

void BConfigParserTest::hugeValue()
{
    // 8 MiB in one quoted value, 1 MiB unquoted: read whole, in linear time
    const QString big(8 * 1024 * 1024, QLatin1Char('x'));
    const QString word(1024 * 1024, QLatin1Char('y'));
    QElapsedTimer timer;
    timer.start();
    BConfigParser parser;
    QVERIFY(parser.parseString(QStringLiteral("Console {\n  Name = c\n  Description = \"%1\"\n  Address = %2\n}\n")
                                   .arg(big, word)));
    QCOMPARE(parser.resource("Console", "c").simpleValue("description").size(), big.size());
    QCOMPARE(parser.resource("Console", "c").simpleValue("address").size(), word.size());
    QVERIFY(timer.elapsed() < 20000);
}

void BConfigParserTest::tooMuchText()
{
    QString text = QStringLiteral("Console { Name = c; Description = \"");
    text += QString(BConfigParser::MaxTextSize, QLatin1Char('x'));
    text += QStringLiteral("\" }\n");
    BConfigParser parser;
    QVERIFY(!parser.parseString(text, "big.conf"));
    QVERIFY(parser.lastError().startsWith(QStringLiteral("big.conf: ")));
    QVERIFY(parser.resources().isEmpty());

    // Through includes: the text of all files counts
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QByteArray half(BConfigParser::MaxTextSize / 2 + 16, '#');
    writeFile(dir.filePath("h1.inc"), half);
    writeFile(dir.filePath("h2.inc"), half);
    writeFile(dir.filePath("main.conf"),
              QStringLiteral("@%1\n@%2\n").arg(dir.filePath("h1.inc"), dir.filePath("h2.inc")).toUtf8());
    BConfigParser includes;
    QVERIFY(!includes.parseFile(dir.filePath("main.conf")));
    QVERIFY2(includes.lastError().contains(QStringLiteral("MiB")), qPrintable(includes.lastError()));
}

void BConfigParserTest::utf16IsRefused()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    writeFile(dir.filePath("u16.conf"), QByteArray("\xFF\xFE", 2) + QByteArray("C\0o\0", 4));
    BConfigParser parser;
    QVERIFY(!parser.parseFile(dir.filePath("u16.conf")));
    QVERIFY(parser.lastError().contains(QStringLiteral("UTF-16")));
    // A UTF-8 byte order mark is accepted
    writeFile(dir.filePath("u8.conf"), QByteArray("\xEF\xBB\xBF") + "Console { Name = c }\n");
    BConfigParser bom;
    QVERIFY2(bom.parseFile(dir.filePath("u8.conf")), qPrintable(bom.lastError()));
    QCOMPARE(bom.resources().size(), 1);
}

QTEST_GUILESS_MAIN(BConfigParserTest)
#include "bconfigparser_test.moc"
