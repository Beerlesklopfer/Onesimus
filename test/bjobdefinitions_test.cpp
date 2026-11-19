// Job and JobDefs: every directive resolved over the JobDefs chain, the raw
// text as the judge of what is set here, and who uses a JobDefs.

#include <QJsonArray>
#include <QJsonDocument>
#include <QtTest>

#include "jobs/bjobdefinitions.h"

namespace {

using Kind = BJobDefinitions::Kind;

QMap<QString, QJsonObject> objects(const char *json)
{
    QMap<QString, QJsonObject> out;
    const QJsonObject root = QJsonDocument::fromJson(QByteArray(json)).object();
    for (auto it = root.constBegin(); it != root.constEnd(); ++it)
        out.insert(it.key(), it.value().toObject());
    return out;
}

// As "show jobs" / "show jobdefs" print them: lower-case keys, references
// as strings or as objects with a name, a job prints only what it sets
const char *kJobs = R"({
    "nightly": {"name": "nightly", "jobdefs": "DefaultJob", "client": "pdc-fd"},
    "weekly":  {"name": "weekly", "jobdefs": {"name": "Archive"}, "priority": 5, "enabled": false},
    "alone":   {"name": "alone", "type": "Admin", "schedule": "Daily"},
    "lost":    {"name": "lost", "jobdefs": "Missing"}
})";
const char *kJobDefs = R"({
    "DefaultJob": {"name": "DefaultJob", "jobdefs": "Base", "fileset": "LinuxAll", "schedule": "WeeklyCycle",
                   "client": "default-fd"},
    "Base":       {"name": "Base", "type": "Backup", "level": "Incremental", "pool": "Incremental",
                   "storage": ["File", "Tape"], "messages": {"name": "Standard"}, "priority": 10},
    "Archive":    {"name": "Archive", "jobdefs": "Base", "pool": "Archive", "maxruntime": "1 day"},
    "LoopA":      {"name": "LoopA", "jobdefs": "LoopB"},
    "LoopB":      {"name": "LoopB", "jobdefs": "LoopA", "level": "Full"}
})";

BJobDefinitions definitions()
{
    BJobDefinitions defs;
    defs.setResources(objects(kJobs), objects(kJobDefs));
    return defs;
}

} // namespace

class BJobDefinitionsTest : public QObject
{
    Q_OBJECT

private slots:
    void resolvesOverTheChain();
    void hereBeatsInherited();
    void notSetAnywhere();
    void circlesAndMissingJobDefs();
    void listsEveryDirective();
    void rawTextDecidesWhatIsSetHere();
    void usersOfAJobDefs();
    void displayTexts();
};

void BJobDefinitionsTest::resolvesOverTheChain()
{
    const BJobDefinitions defs = definitions();
    QCOMPARE(defs.chain(Kind::Job, QStringLiteral("nightly")),
             QStringList({QStringLiteral("DefaultJob"), QStringLiteral("Base")}));

    // From the nearest JobDefs
    BJobDefinitions::Value fileset = defs.value(Kind::Job, QStringLiteral("nightly"), QStringLiteral("FileSet"));
    QVERIFY(fileset.set);
    QVERIFY(!fileset.here);
    QCOMPARE(fileset.from, QStringLiteral("DefaultJob"));
    QCOMPARE(fileset.text, QStringLiteral("LinuxAll"));
    QCOMPARE(fileset.label, QStringLiteral("FileSet"));

    // Two steps up
    const BJobDefinitions::Value pool = defs.value(Kind::Job, QStringLiteral("nightly"), QStringLiteral("pool"));
    QCOMPARE(pool.from, QStringLiteral("Base"));
    QCOMPARE(pool.text, QStringLiteral("Incremental"));

    // A JobDefs inherits as well
    const BJobDefinitions::Value level = defs.value(Kind::JobDefs, QStringLiteral("Archive"), QStringLiteral("level"));
    QCOMPARE(level.from, QStringLiteral("Base"));
    // ... and the nearer one wins
    QCOMPARE(defs.value(Kind::Job, QStringLiteral("weekly"), QStringLiteral("pool")).from, QStringLiteral("Archive"));
}

void BJobDefinitionsTest::hereBeatsInherited()
{
    const BJobDefinitions defs = definitions();
    const BJobDefinitions::Value client = defs.value(Kind::Job, QStringLiteral("nightly"), QStringLiteral("client"));
    QVERIFY(client.here);
    QCOMPARE(client.text, QStringLiteral("pdc-fd"));       // not default-fd of DefaultJob
    QVERIFY(client.from.isEmpty());

    const BJobDefinitions::Value priority = defs.value(Kind::Job, QStringLiteral("weekly"), QStringLiteral("priority"));
    QVERIFY(priority.here);
    QCOMPARE(priority.text, QStringLiteral("5"));
    QCOMPARE(defs.value(Kind::Job, QStringLiteral("weekly"), QStringLiteral("enabled")).text, QStringLiteral("no"));

    // JobDefs is never inherited: it names the chain
    QVERIFY(!defs.value(Kind::Job, QStringLiteral("alone"), QStringLiteral("jobdefs")).set);
    QCOMPARE(defs.value(Kind::Job, QStringLiteral("weekly"), QStringLiteral("jobdefs")).text, QStringLiteral("Archive"));
}

void BJobDefinitionsTest::notSetAnywhere()
{
    const BJobDefinitions defs = definitions();
    const BJobDefinitions::Value pool = defs.value(Kind::Job, QStringLiteral("alone"), QStringLiteral("pool"));
    QVERIFY(!pool.set);
    QVERIFY(!pool.uncertain);
    QVERIFY(pool.text.isEmpty());
}

void BJobDefinitionsTest::circlesAndMissingJobDefs()
{
    const BJobDefinitions defs = definitions();
    // A circle is followed once round, never forever
    QCOMPARE(defs.chain(Kind::JobDefs, QStringLiteral("LoopA")), QStringList({QStringLiteral("LoopB")}));
    QCOMPARE(defs.value(Kind::JobDefs, QStringLiteral("LoopA"), QStringLiteral("level")).from, QStringLiteral("LoopB"));
    QVERIFY(!defs.value(Kind::JobDefs, QStringLiteral("LoopA"), QStringLiteral("pool")).set);

    // A JobDefs that was not reported: not known, not "default"
    QCOMPARE(defs.chain(Kind::Job, QStringLiteral("lost")), QStringList({QStringLiteral("Missing")}));
    const BJobDefinitions::Value pool = defs.value(Kind::Job, QStringLiteral("lost"), QStringLiteral("pool"));
    QVERIFY(!pool.set);
    QVERIFY(pool.uncertain);
}

void BJobDefinitionsTest::listsEveryDirective()
{
    const BJobDefinitions defs = definitions();
    const QList<BJobDefinitions::Value> values = defs.values(Kind::Job, QStringLiteral("weekly"));
    QStringList keys;
    for (const BJobDefinitions::Value &value : values)
        keys.append(value.key);
    // The common directives first, in their order, then the others
    QCOMPARE(keys.mid(0, BJobDefinitions::commonKeys().size()), BJobDefinitions::commonKeys());
    QVERIFY(keys.contains(QStringLiteral("maxruntime")));     // from Archive
    QVERIFY(!keys.contains(QStringLiteral("name")));
    for (const BJobDefinitions::Value &value : values) {
        if (value.key == QLatin1String("maxruntime")) {
            QCOMPARE(value.label, QStringLiteral("Max Run Time"));
            QCOMPARE(value.from, QStringLiteral("Archive"));
        }
    }
}

void BJobDefinitionsTest::rawTextDecidesWhatIsSetHere()
{
    // "show" printed the client as if set in the job, but the text does not
    // have it: then it comes from the chain. The text sets Pool, which show
    // did not print, and an unknown directive.
    BJobDefinitions defs;
    QMap<QString, QJsonObject> jobs = objects(kJobs);
    QJsonObject nightly = jobs.value(QStringLiteral("nightly"));
    nightly.insert(QStringLiteral("level"), QStringLiteral("Incremental"));   // printed, inherited in truth
    jobs.insert(QStringLiteral("nightly"), nightly);
    defs.setResources(jobs, objects(kJobDefs));

    const BResourceText::Parsed text = BResourceText::parse(QStringLiteral(
        "Job {\n  Name = nightly\n  JobDefs = DefaultJob\n  Client = pdc-fd\n  Pool = \"Daily\"\n"
        "  Future Thing = on\n  Run Script {\n    Command = x\n  }\n}\n"));
    QVERIFY(text.ok);
    QHash<QString, BJobDefinitions::Value> byKey;
    for (const BJobDefinitions::Value &value : defs.resolveWithText(Kind::Job, QStringLiteral("nightly"), text))
        byKey.insert(value.key, value);

    QVERIFY(byKey.value(QStringLiteral("pool")).here);
    QCOMPARE(byKey.value(QStringLiteral("pool")).text, QStringLiteral("Daily"));
    QVERIFY(!byKey.value(QStringLiteral("level")).here);
    QCOMPARE(byKey.value(QStringLiteral("level")).from, QStringLiteral("Base"));
    QVERIFY(byKey.value(QStringLiteral("futurething")).here);
    QCOMPARE(byKey.value(QStringLiteral("futurething")).label, QStringLiteral("Future Thing"));
    QVERIFY(byKey.value(QStringLiteral("runscript")).here);
    QCOMPARE(byKey.value(QStringLiteral("fileset")).from, QStringLiteral("DefaultJob"));
}

void BJobDefinitionsTest::usersOfAJobDefs()
{
    const BJobDefinitions defs = definitions();
    // Directly and through another JobDefs
    QCOMPARE(defs.jobsUsing(QStringLiteral("Base")),
             QStringList({QStringLiteral("nightly"), QStringLiteral("weekly")}));
    QCOMPARE(defs.usersOf(QStringLiteral("Base")),
             QStringList({QStringLiteral("Job nightly"), QStringLiteral("Job weekly"),
                          QStringLiteral("JobDefs Archive"), QStringLiteral("JobDefs DefaultJob")}));
    QCOMPARE(defs.usersOf(QStringLiteral("Archive")), QStringList({QStringLiteral("Job weekly")}));
    QVERIFY(defs.usersOf(QStringLiteral("Nobody")).isEmpty());
}

void BJobDefinitionsTest::displayTexts()
{
    QCOMPARE(BJobDefinitions::displayText(QJsonValue(true)), QStringLiteral("yes"));
    QCOMPARE(BJobDefinitions::displayText(QJsonValue(12)), QStringLiteral("12"));
    QJsonObject ref;
    ref.insert(QStringLiteral("name"), QStringLiteral("Standard"));
    QCOMPARE(BJobDefinitions::displayText(ref), QStringLiteral("Standard"));
    QJsonArray list{QStringLiteral("File"), QStringLiteral("Tape")};
    QCOMPARE(BJobDefinitions::displayText(list), QStringLiteral("File, Tape"));
    QCOMPARE(BJobDefinitions::label(QStringLiteral("fullbackuppool")), QStringLiteral("Full Backup Pool"));
    QCOMPARE(BJobDefinitions::label(QStringLiteral("somethingnew")), QStringLiteral("somethingnew"));
}

QTEST_APPLESS_MAIN(BJobDefinitionsTest)
#include "bjobdefinitions_test.moc"
