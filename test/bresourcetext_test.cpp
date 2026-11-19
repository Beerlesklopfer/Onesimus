// The raw text of one resource: reading its directives, writing a change of
// single directives into it with every other line byte for byte, refusing
// what cannot be done safely, and the line diff of the review.

#include <QtTest>

#include "config/bresourcetext.h"

namespace {

// A Job as it may stand in its file: comments, an unknown directive, a
// nested block, quoting of both kinds, a directive with a comment
const QString kJob = QStringLiteral(
    "Job {\n"
    "  Name = \"nightly\"\n"
    "  # what is backed up\n"
    "  JobDefs = \"DefaultJob\"\n"
    "  Client = pdc-fd          # the domain controller\n"
    "  FileSet=\"LinuxAll\"\n"
    "  Some Future Directive = 42\n"
    "  Run Script {\n"
    "    Command = \"/usr/local/bin/pre.sh\"\n"
    "    Runs When = Before\n"
    "  }\n"
    "  Priority = 10\n"
    "}\n");

QStringList linesOf(const QString &text)
{
    QStringList lines = text.split(QLatin1Char('\n'));
    if (!lines.isEmpty() && lines.last().isEmpty())
        lines.removeLast();
    return lines;
}

} // namespace

class BResourceTextTest : public QObject
{
    Q_OBJECT

private slots:
    void readsDirectives();
    void changesOnlyTheEditedLine();
    void keepsCrlf();
    void addsAndRemovesDirectives();
    void quotesWhereNeeded();
    void refusesWhatIsNotSafe();
    void refusesTextsThatAreNotOneResource();
    void changeThatChangesNothing();
    void createsText();
    void diffsLines();
};

void BResourceTextTest::readsDirectives()
{
    const BResourceText::Parsed parsed = BResourceText::parse(kJob);
    QVERIFY2(parsed.ok, qPrintable(parsed.error));
    QCOMPARE(parsed.typeWord, QStringLiteral("Job"));
    QCOMPARE(parsed.name, QStringLiteral("nightly"));
    QCOMPARE(parsed.value(QStringLiteral("JobDefs")), QStringLiteral("DefaultJob"));
    QCOMPARE(parsed.value(QStringLiteral("client")), QStringLiteral("pdc-fd"));
    QCOMPARE(parsed.value(QStringLiteral("File Set")), QStringLiteral("LinuxAll"));
    QCOMPARE(parsed.value(QStringLiteral("somefuturedirective")), QStringLiteral("42"));
    QCOMPARE(parsed.value(QStringLiteral("Priority")), QStringLiteral("10"));
    // The nested block is not read as directives of the job
    QVERIFY(!parsed.has(QStringLiteral("Command")));
    QVERIFY(parsed.opaqueKeys.contains(QStringLiteral("runscript")));
    QCOMPARE(parsed.closingLine, 12);
}

void BResourceTextTest::changesOnlyTheEditedLine()
{
    QString out;
    QString error;
    QVERIFY2(BResourceText::edit(kJob, {BResourceText::Change::set(QStringLiteral("Client"), QStringLiteral("nas-fd"))},
                                 &out, &error), qPrintable(error));
    const QStringList before = linesOf(kJob);
    const QStringList after = linesOf(out);
    QCOMPARE(after.size(), before.size());
    for (int i = 0; i < before.size(); ++i) {
        if (i == 4)
            continue;
        QCOMPARE(after[i], before[i]);
    }
    // The comment, the spacing and the missing quotes of the line stay
    QCOMPARE(after[4], QStringLiteral("  Client = nas-fd          # the domain controller"));

    // A quoted value stays quoted, "=" without spaces stays so
    QVERIFY(BResourceText::edit(kJob, {BResourceText::Change::set(QStringLiteral("FileSet"), QStringLiteral("Windows"))},
                                &out, &error));
    QCOMPARE(linesOf(out)[5], QStringLiteral("  FileSet=\"Windows\""));
    QCOMPARE(out.size(), kJob.size() + 7 - 8);
}

void BResourceTextTest::keepsCrlf()
{
    QString crlf = kJob;
    crlf.replace(QLatin1String("\n"), QLatin1String("\r\n"));
    QString out;
    QString error;
    QVERIFY2(BResourceText::edit(crlf, {BResourceText::Change::set(QStringLiteral("Priority"), QStringLiteral("12")),
                                        BResourceText::Change::set(QStringLiteral("Pool"), QStringLiteral("Full"))},
                                 &out, &error), qPrintable(error));
    QCOMPARE(out.count(QLatin1String("\r\n")), crlf.count(QLatin1String("\r\n")) + 1);
    QCOMPARE(out.count(QLatin1Char('\n')), out.count(QLatin1String("\r\n")));   // no bare LF
    QVERIFY(out.contains(QStringLiteral("  Priority = 12\r\n  Pool = Full\r\n}\r\n")));
    // Everything else byte for byte
    QString expected = crlf;
    expected.replace(QStringLiteral("  Priority = 10\r\n"), QStringLiteral("  Priority = 12\r\n  Pool = Full\r\n"));
    QCOMPARE(out.toUtf8(), expected.toUtf8());

    // A text whose last line has no line end keeps that, too
    QString noEnd = crlf;
    noEnd.chop(2);
    QVERIFY(BResourceText::edit(noEnd, {BResourceText::Change::set(QStringLiteral("Priority"), QStringLiteral("5"))},
                                &out, &error));
    QVERIFY(out.endsWith(QStringLiteral("  Priority = 5\r\n}")));
}

void BResourceTextTest::addsAndRemovesDirectives()
{
    QString out;
    QString error;
    QVERIFY2(BResourceText::edit(kJob, {BResourceText::Change::unset(QStringLiteral("Client")),
                                        BResourceText::Change::set(QStringLiteral("Schedule"), QStringLiteral("WeeklyCycle")),
                                        BResourceText::Change::unset(QStringLiteral("Storage"))},   // not set: no-op
                                 &out, &error), qPrintable(error));
    QVERIFY(!out.contains(QStringLiteral("Client")));
    QVERIFY(!out.contains(QStringLiteral("domain controller")));     // the comment of the line goes with it
    QVERIFY(out.endsWith(QStringLiteral("  Priority = 10\n  Schedule = WeeklyCycle\n}\n")));
    QVERIFY(out.contains(QStringLiteral("  Run Script {\n    Command = \"/usr/local/bin/pre.sh\"\n")));
    QVERIFY(out.contains(QStringLiteral("  # what is backed up\n")));
    QVERIFY(out.contains(QStringLiteral("  Some Future Directive = 42\n")));

    // Indentation of the directives there, here a tab
    const QString tabbed = QStringLiteral("JobDefs {\n\tName = Base\n\tLevel = Full\n}\n");
    QVERIFY(BResourceText::edit(tabbed, {BResourceText::Change::set(QStringLiteral("Messages"), QStringLiteral("Standard"))},
                                &out, &error));
    QCOMPARE(out, QStringLiteral("JobDefs {\n\tName = Base\n\tLevel = Full\n\tMessages = Standard\n}\n"));

    // A renamed copy: Name may change when it is asked for
    QVERIFY(BResourceText::edit(kJob, {BResourceText::Change::set(QStringLiteral("Name"), QStringLiteral("nightly-copy"))},
                                &out, &error));
    QCOMPARE(BResourceText::parse(out).name, QStringLiteral("nightly-copy"));
}

void BResourceTextTest::quotesWhereNeeded()
{
    QString out;
    QString error;
    QVERIFY(BResourceText::edit(kJob, {BResourceText::Change::set(QStringLiteral("Client"), QStringLiteral("a client"))},
                                &out, &error));
    QVERIFY(out.contains(QStringLiteral("  Client = \"a client\"          # the domain controller\n")));
    QVERIFY(BResourceText::edit(kJob, {BResourceText::Change::set(QStringLiteral("Description"),
                                                                  QStringLiteral("say \"hi\" \\ there"))},
                                &out, &error));
    QVERIFY(out.contains(QStringLiteral("  Description = \"say \\\"hi\\\" \\\\ there\"\n")));
    QCOMPARE(BResourceText::parse(out).value(QStringLiteral("Description")), QStringLiteral("say \"hi\" \\ there"));
}

void BResourceTextTest::refusesWhatIsNotSafe()
{
    QString out = QStringLiteral("unchanged");
    QString error;

    // Twice the same directive
    const QString twice = QStringLiteral("Job {\n  Name = a\n  Storage = File\n  Storage = Tape\n}\n");
    QVERIFY(!BResourceText::edit(twice, {BResourceText::Change::set(QStringLiteral("Storage"), QStringLiteral("X"))},
                                 &out, &error));
    QVERIFY(error.contains(QStringLiteral("more than once")));
    QCOMPARE(out, QStringLiteral("unchanged"));

    // Two directives on a line
    const QString shared = QStringLiteral("Job {\n  Name = a\n  Client = c; Pool = p\n}\n");
    QVERIFY(!BResourceText::edit(shared, {BResourceText::Change::set(QStringLiteral("Pool"), QStringLiteral("X"))},
                                 &out, &error));
    QVERIFY(!BResourceText::edit(shared, {BResourceText::Change::set(QStringLiteral("Client"), QStringLiteral("X"))},
                                 &out, &error));
    // ... another directive of that resource can be changed
    QVERIFY(BResourceText::edit(shared, {BResourceText::Change::set(QStringLiteral("Level"), QStringLiteral("Full"))},
                                &out, &error));
    QVERIFY(out.contains(QStringLiteral("  Client = c; Pool = p\n")));

    // A list
    const QString list = QStringLiteral("Job {\n  Name = a\n  Storage = \"File\", \"Tape\"\n}\n");
    QVERIFY(!BResourceText::edit(list, {BResourceText::Change::set(QStringLiteral("Storage"), QStringLiteral("X"))},
                                 &out, &error));

    // A directive of a nested block is not one of the resource
    QVERIFY(!BResourceText::edit(kJob, {BResourceText::Change::set(QStringLiteral("Run Script"), QStringLiteral("x"))},
                                 &out, &error));

    // Line breaks in a value, empty values
    QVERIFY(!BResourceText::edit(kJob, {BResourceText::Change::set(QStringLiteral("Client"), QStringLiteral("a\nb"))},
                                 &out, &error));
    QVERIFY(!BResourceText::edit(kJob, {BResourceText::Change::set(QStringLiteral("Client"), QString())},
                                 &out, &error));
}

void BResourceTextTest::refusesTextsThatAreNotOneResource()
{
    QVERIFY(!BResourceText::parse(QStringLiteral("Name = a\n")).ok);
    QVERIFY(!BResourceText::parse(QStringLiteral("Job {\n  Name = a\n")).ok);
    QVERIFY(!BResourceText::parse(QStringLiteral("Job {\n  Name = a\n}\nJob {\n}\n")).ok);
    QVERIFY(!BResourceText::parse(QStringLiteral("Job { Name = a\n}\n")).ok);
    QVERIFY(!BResourceText::parse(QStringLiteral("Job {\n  Name = a }\n")).ok);
    // A comment after the closing brace is part of the resource
    QVERIFY(BResourceText::parse(QStringLiteral("Job {\n  Name = a\n} # end\n")).ok);
    QVERIFY(BResourceText::parse(QStringLiteral("# above\nJob {\n  Name = a\n}\n\n")).ok);
}

void BResourceTextTest::changeThatChangesNothing()
{
    QString out;
    QVERIFY(BResourceText::edit(kJob, {BResourceText::Change::set(QStringLiteral("Client"), QStringLiteral("pdc-fd"))},
                                &out));
    QCOMPARE(out, kJob);
    QVERIFY(BResourceText::edit(kJob, {}, &out));
    QCOMPARE(out, kJob);
}

void BResourceTextTest::createsText()
{
    const QString text = BResourceText::createText(
        QStringLiteral("Job"), {BResourceText::Change::set(QStringLiteral("Name"), QStringLiteral("new")),
                                BResourceText::Change::set(QStringLiteral("JobDefs"), QStringLiteral("DefaultJob")),
                                BResourceText::Change::set(QStringLiteral("Client"), QStringLiteral("my client")),
                                BResourceText::Change::set(QStringLiteral("Enabled"), QStringLiteral("no"))},
        QStringLiteral("\r\n"));
    QCOMPARE(text, QStringLiteral("Job {\r\n  Name = \"new\"\r\n  JobDefs = DefaultJob\r\n  Client = \"my client\"\r\n"
                                  "  Enabled = no\r\n}\r\n"));
    const BResourceText::Parsed parsed = BResourceText::parse(text);
    QVERIFY(parsed.ok);
    QCOMPARE(parsed.name, QStringLiteral("new"));
}

void BResourceTextTest::diffsLines()
{
    QString out;
    QVERIFY(BResourceText::edit(kJob, {BResourceText::Change::set(QStringLiteral("Client"), QStringLiteral("nas-fd")),
                                       BResourceText::Change::set(QStringLiteral("Pool"), QStringLiteral("Full"))},
                                &out));
    const QList<BResourceText::DiffLine> diff = BResourceText::diff(kJob, out);
    int removed = 0;
    int added = 0;
    int same = 0;
    for (const BResourceText::DiffLine &line : diff) {
        if (line.kind == BResourceText::DiffLine::Removed) {
            ++removed;
            QVERIFY(line.text.contains(QStringLiteral("pdc-fd")));
        } else if (line.kind == BResourceText::DiffLine::Added) {
            ++added;
            QVERIFY(line.text.contains(QStringLiteral("nas-fd")) || line.text.contains(QStringLiteral("Pool")));
        } else {
            ++same;
        }
    }
    QCOMPARE(removed, 1);
    QCOMPARE(added, 2);
    QCOMPARE(same, int(linesOf(kJob).size()) - 1);
}

QTEST_APPLESS_MAIN(BResourceTextTest)
#include "bresourcetext_test.moc"
