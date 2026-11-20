/**
 * @file baclmatrix_test.cpp
 * @brief The ACL matrix of the consoles (BAclMatrix)
 *
 * The reply follows the form the Onesimus Director (odir 7e423f6) sends for
 * ".acl list": file ACL entries with their source, and the catalog ACL, the
 * effective ACL, the state of the catalog ACL and "locked" as JSON texts in
 * extra_fields. Shortened from a reply of a test Director; the names are
 * made up. Runs without a Director (registered with CTest).
 */

#include <QtTest>

#include "director/baclmatrix.h"
#include "director/baclmatrixdialog.h"

#include <QCheckBox>
#include <QPushButton>
#include <QHeaderView>
#include <QLineEdit>
#include <QTableWidget>

namespace {

const char *const kReply = "{\"acl\":[\n"
    " {\"console\":\"admin\",\n"
    "  \"jobacl\":[{\"value\":\"*all*\",\"source\":\"Console\"}],\n"
    "  \"commandacl\":[{\"value\":\"*all*\",\"source\":\"Console\"}],\n"
    "  \"extra_fields\":{\"locked\":\"false\",\"catalog_state\":\"{\\\"state\\\":\\\"loaded\\\"}\",\n"
    "   \"catalog\":\"{\\\"jobacl\\\":[{\\\"value\\\":\\\"backup-a\\\",\\\"position\\\":1,\\\"ineffective\\\":false}]}\",\n"
    "   \"effective\":\"{\\\"jobacl\\\":{\\\"source\\\":\\\"file+catalog\\\",\\\"allowed\\\":[\\\"backup-a\\\"],\\\"configured\\\":3},\\\"commandacl\\\":{\\\"source\\\":\\\"file\\\",\\\"allowed\\\":[\\\"status\\\",\\\"delete\\\",\\\"run\\\"],\\\"configured\\\":3},\\\"whereacl\\\":{\\\"source\\\":\\\"file\\\",\\\"allowed\\\":null,\\\"configured\\\":null}}\"}},\n"
    " {\"console\":\"operator-mcp\",\"profiles\":[\"operator\"],\n"
    "  \"jobacl\":[{\"value\":\"*all*\",\"source\":\"Profile operator\"}],\n"
    "  \"commandacl\":[{\"value\":\"!delete\",\"source\":\"Profile operator\"},{\"value\":\"*all*\",\"source\":\"Profile operator\"}],\n"
    "  \"whereacl\":[{\"value\":\"/restore\",\"source\":\"Console\"}],\n"
    "  \"extra_fields\":{\"locked\":\"false\",\"catalog_state\":\"{\\\"state\\\":\\\"loaded\\\"}\",\"catalog\":\"{}\",\n"
    "   \"effective\":\"{\\\"jobacl\\\":{\\\"source\\\":\\\"file\\\",\\\"allowed\\\":[\\\"backup-a\\\",\\\"backup-b\\\",\\\"catalog\\\"],\\\"configured\\\":3},\\\"commandacl\\\":{\\\"source\\\":\\\"file\\\",\\\"allowed\\\":[\\\"status\\\",\\\"run\\\"],\\\"configured\\\":3}}\"}},\n"
    " {\"console\":\"monitor\",\n"
    "  \"commandacl\":[{\"value\":\"status\",\"source\":\"Console\"}],\n"
    "  \"extra_fields\":{\"locked\":\"false\",\n"
    "   \"effective\":\"{\\\"jobacl\\\":{\\\"source\\\":\\\"file\\\",\\\"allowed\\\":[],\\\"configured\\\":3},\\\"commandacl\\\":{\\\"source\\\":\\\"file\\\",\\\"allowed\\\":[\\\"status\\\"],\\\"configured\\\":3}}\"}},\n"
    " {\"console\":\"broken\",\"jobacl\":[{\"value\":\"*all*\",\"source\":\"Console\"}],\n"
    "  \"extra_fields\":{\"locked\":\"true\",\"catalog_state\":\"{\\\"state\\\":\\\"failed\\\",\\\"error\\\":\\\"permission denied\\\"}\"}}\n"
    "]}";

const BAclMatrix::Row *rowOf(const QList<BAclMatrix::Row> &rows, const QString &resource)
{
    for (const BAclMatrix::Row &row : rows) {
        if (row.resource == resource) return &row;
    }
    return nullptr;
}

} // namespace

class BAclMatrixTest : public QObject
{
    Q_OBJECT

private slots:
    void readsEveryConsole();
    void jobsAsTheDirectorDecided();
    void commandsWithTheirReasons();
    void knownNamesGetARow();
    void fileDecisionAsBareos();
    void errorReplies();
    void resultWrapperAndNoExtraFields();
    void dialogHasAColumnPerConsole();
    void catalogListsAfterAChange();
    void changeCommandsAndReplies();
    void dialogSendsTheChangesInOrder();
    void newConsoleAndCancel();
    void profilesProposeText();
    void newProfileIsAProposal();
};

void BAclMatrixTest::readsEveryConsole()
{
    QList<BConsoleAcl> consoles;
    QString error;
    QVERIFY2(BAclMatrix::parse(QString::fromUtf8(kReply), &consoles, &error), qPrintable(error));
    QCOMPARE(consoles.size(), 4);
    QCOMPARE(consoles[1].name, QString("operator-mcp"));
    QCOMPARE(consoles[1].profiles, QStringList{"operator"});
    QCOMPARE(consoles[0].catalog.value("jobacl").size(), 1);
    QCOMPARE(consoles[0].effective.value("jobacl").source, QString("file+catalog"));
    QVERIFY(!consoles[0].effective.value("whereacl").allowed.has_value());
    QVERIFY(consoles[3].locked);
    QCOMPARE(consoles[3].catalogState, QString("failed"));
    QCOMPARE(consoles[3].catalogError, QString("permission denied"));
    const QString summary = BAclMatrix::consoleSummary(consoles[1]);
    QVERIFY2(summary.contains("Profiles: operator") && summary.contains("Where: /restore (Console)"),
             qPrintable(summary));
}

void BAclMatrixTest::jobsAsTheDirectorDecided()
{
    QList<BConsoleAcl> consoles;
    QVERIFY(BAclMatrix::parse(QString::fromUtf8(kReply), &consoles));
    const QList<BAclMatrix::Row> rows = BAclMatrix::rows(consoles, "jobacl");
    QCOMPARE(rows.size(), 3);   // backup-a, backup-b, catalog
    QCOMPARE(rows[0].resource, QString("backup-a"));
    const BAclMatrix::Row *b = rowOf(rows, "backup-b");
    QVERIFY(b);
    // admin: the file allows everything, its catalog ACL only backup-a
    QCOMPARE(b->cells[0], BAclMatrix::Cell::NarrowedByCatalog);
    QVERIFY2(b->reasons[0].contains("catalog ACL does not"), qPrintable(b->reasons[0]));
    QCOMPARE(b->cells[1], BAclMatrix::Cell::Allowed);
    QVERIFY2(b->reasons[1].contains("*all* (Profile operator)"), qPrintable(b->reasons[1]));
    QCOMPARE(b->cells[2], BAclMatrix::Cell::NotListed);
    QVERIFY(b->reasons[2].contains("no ACL of this type"));
    QCOMPARE(b->cells[3], BAclMatrix::Cell::Locked);
    QVERIFY(b->differs);
}

void BAclMatrixTest::commandsWithTheirReasons()
{
    QList<BConsoleAcl> consoles;
    QVERIFY(BAclMatrix::parse(QString::fromUtf8(kReply), &consoles));
    const QList<BAclMatrix::Row> rows = BAclMatrix::rows(consoles, "commandacl");
    const BAclMatrix::Row *del = rowOf(rows, "delete");
    QVERIFY(del);
    QCOMPARE(del->cells[0], BAclMatrix::Cell::Allowed);
    QCOMPARE(del->cells[1], BAclMatrix::Cell::Denied);
    QVERIFY2(del->reasons[1].contains("!delete (Profile operator)"), qPrintable(del->reasons[1]));
    QCOMPARE(del->cells[2], BAclMatrix::Cell::NotListed);
    QVERIFY2(del->reasons[2].contains("No entry"), qPrintable(del->reasons[2]));
    const BAclMatrix::Row *status = rowOf(rows, "status");
    QVERIFY(status);
    QCOMPARE(status->cells[2], BAclMatrix::Cell::Allowed);
}

void BAclMatrixTest::knownNamesGetARow()
{
    QList<BConsoleAcl> consoles;
    QVERIFY(BAclMatrix::parse(QString::fromUtf8(kReply), &consoles));
    const QList<BAclMatrix::Row> rows = BAclMatrix::rows(consoles, "clientacl", {"pc-fd"});
    QCOMPARE(rows.size(), 1);
    QCOMPARE(rows[0].resource, QString("pc-fd"));
    QCOMPARE(rows[0].cells[0], BAclMatrix::Cell::NotListed);
    QVERIFY(rows[0].differs);   // the locked one
}

void BAclMatrixTest::fileDecisionAsBareos()
{
    const QList<BAclEntry> entries = {{"!backup-secret.*", "Console"}, {"backup-.*", "Profile p"},
                                      {"Catalog", "Console"}};
    const BAclEntry *by = nullptr;
    QCOMPARE(BAclMatrix::fileDecision(entries, "backup-a", &by), BAclMatrix::Cell::Allowed);
    QCOMPARE(by->source, QString("Profile p"));
    QCOMPARE(BAclMatrix::fileDecision(entries, "Backup-Secret-1", &by), BAclMatrix::Cell::Denied);
    QCOMPARE(BAclMatrix::fileDecision(entries, "catalog"), BAclMatrix::Cell::Allowed);
    QCOMPARE(BAclMatrix::fileDecision(entries, "restore"), BAclMatrix::Cell::NotListed);
    QCOMPARE(BAclMatrix::fileDecision({}, "x"), BAclMatrix::Cell::NotListed);
}

void BAclMatrixTest::errorReplies()
{
    QString error;
    QVERIFY(!BAclMatrix::parse("", nullptr, &error));
    QVERIFY(!BAclMatrix::parse(".acl: is an invalid command.", nullptr, &error));
    QVERIFY2(error.contains("invalid command"), qPrintable(error));
    QVERIFY(!BAclMatrix::parse("{\"jsonrpc\":\"2.0\",\"error\":{\"code\":1,\"message\":\"failed\"}}", nullptr, &error));
    QVERIFY2(error.contains("refused"), qPrintable(error));
    QVERIFY(!BAclMatrix::parse("{\"result\":{\"jobs\":[]}}", nullptr, &error));
}

void BAclMatrixTest::resultWrapperAndNoExtraFields()
{
    // An older Director: no effective ACL; the file entries decide
    QList<BConsoleAcl> consoles;
    QVERIFY(BAclMatrix::parse("{\"jsonrpc\":\"2.0\",\"result\":{\"acl\":[{\"console\":\"c\",\n"
    "        \"commandacl\":[{\"value\":\"!delete\",\"source\":\"Console\"},{\"value\":\"*all*\",\"source\":\"Console\"}]}]}}", &consoles));
    QCOMPARE(consoles.size(), 1);
    const QList<BAclMatrix::Row> rows = BAclMatrix::rows(consoles, "commandacl", {"delete", "run"});
    QCOMPARE(rowOf(rows, "delete")->cells[0], BAclMatrix::Cell::Denied);
    QCOMPARE(rowOf(rows, "run")->cells[0], BAclMatrix::Cell::Allowed);
}

void BAclMatrixTest::dialogHasAColumnPerConsole()
{
    BAclMatrixDialog dialog;
    int asked = 0;
    QObject::connect(&dialog, &BAclMatrixDialog::refreshRequested, [&]() { ++asked; });
    dialog.setReply(QString::fromUtf8(kReply));
    QTableWidget *table = dialog.table();
    QCOMPARE(table->columnCount(), 4);
    QCOMPARE(table->horizontalHeaderItem(1)->text(), QString("operator-mcp"));
    QVERIFY(table->horizontalHeaderItem(3)->text().startsWith("broken"));   // locked: marked
    // A console added on the Director is a column more after the refresh
    QString more = QString::fromUtf8(kReply);
    more = more.left(more.lastIndexOf(QStringLiteral("]}")))
           + QStringLiteral(",{\"console\":\"new-console\",\"jobacl\":[{\"value\":\"backup-a\",\"source\":\"Console\"}]}]}");
    dialog.setReply(more);
    QCOMPARE(table->columnCount(), 5);
    QCOMPARE(table->horizontalHeaderItem(4)->text(), QString("new-console"));

    // The rows of a type behind its title; the names are the row headers
    int deleteRow = -1;
    for (int r = 0; r < table->rowCount(); ++r) {
        if (table->verticalHeaderItem(r)->text().trimmed() == "delete") deleteRow = r;
    }
    QVERIFY(deleteRow > 0);
    QCOMPARE(table->item(deleteRow, 1)->text(), QString("\u2717"));
    QVERIFY(table->item(deleteRow, 1)->toolTip().contains("!delete (Profile operator)"));

    // Filter and only the rows that differ
    dialog.findChild<QLineEdit *>()->setText("backup-b");
    int visible = 0;
    for (int r = 0; r < table->rowCount(); ++r) visible += !table->isRowHidden(r);
    QCOMPARE(visible, 2);   // the title "Jobs" and backup-b
    dialog.findChild<QLineEdit *>()->clear();
    dialog.findChild<QCheckBox *>()->setChecked(true);
    for (int r = 0; r < table->rowCount(); ++r) {
        if (!table->isRowHidden(r) && !table->verticalHeaderItem(r)->text().startsWith("  ")) continue;
        if (!table->isRowHidden(r)) {
            QStringList cells;
            for (int c = 0; c < table->columnCount(); ++c) cells << table->item(r, c)->text();
            cells.removeDuplicates();
            QVERIFY(cells.size() > 1);
        }
    }
    if (qEnvironmentVariableIsSet("ONESIMUS_ACL_PNG")) {
        dialog.findChild<QCheckBox *>()->setChecked(false);
        dialog.resize(1000, 700);
        dialog.show();
        QTest::qWait(100);
        dialog.grab().save(qEnvironmentVariable("ONESIMUS_ACL_PNG"));
    }
    // A Director without the command
    dialog.setReply(".acl: is an invalid command.");
    QCOMPARE(table->columnCount(), 5);      // what was read stays
    QCOMPARE(asked, 0);
}

void BAclMatrixTest::catalogListsAfterAChange()
{
    // No catalog entries: deny one, the rest stays allowed
    QCOMPARE(BAclMatrix::catalogAfter({}, "backup-b", false), (QStringList{"!backup-b", "*all*"}));
    // ... and allowed again: no restriction left
    QCOMPARE(BAclMatrix::catalogAfter({"!backup-b", "*all*"}, "backup-b", true), QStringList());
    // A list: the negation goes in front (narrower: odir accepts it on the own console)
    QCOMPARE(BAclMatrix::catalogAfter({"backup-a", "backup-b"}, "backup-a", false),
             (QStringList{"!backup-a", "backup-a", "backup-b"}));
    // A positive list that does not name it: its name in front (wider)
    QCOMPARE(BAclMatrix::catalogAfter({"backup-a"}, "backup-b", true), (QStringList{"backup-b", "backup-a"}));
    // Denied already, allowed already: nothing changes
    QCOMPARE(BAclMatrix::catalogAfter({"backup-a"}, "backup-b", false), QStringList{"backup-a"});
    QCOMPARE(BAclMatrix::catalogAfter({}, "x", true), QStringList());
    // First match decides
    QVERIFY(!BAclMatrix::listAllows({"!b.*", "*all*"}, "backup"));
    QVERIFY(BAclMatrix::listAllows({"backup", "!b.*"}, "backup"));
    QVERIFY(!BAclMatrix::listAllows({}, "x"));
}

void BAclMatrixTest::changeCommandsAndReplies()
{
    QCOMPARE(BAclMatrix::changeCommand("My Console", "filesetacl", {"Full Set", "!Windows All Drives"}),
             QString(".acl set console=\"My Console\" filesetacl=\"Full Set,!Windows All Drives\""));
    QCOMPARE(BAclMatrix::changeCommand("mcp", "jobacl", {}), QString(".acl delete console=\"mcp\" jobacl"));

    QString message;
    QVERIFY(BAclMatrix::changeSucceeded(
        "{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{\"acl\":{\"action\":\"set\",\"console\":\"mcp\",\"changes\":[]}}}",
        &message));
    QVERIFY(!BAclMatrix::changeSucceeded(
        "{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":{\"code\":1,\"message\":\"failed\",\"data\":{\"result\":{},"
        "\"messages\":{\"error\":[\"Console \\\"x\\\" not found.\\n\"]}}}}", &message));
    QCOMPARE(message, QString("Console \"x\" not found."));
    QVERIFY(!BAclMatrix::changeSucceeded(".acl set: permission denied, changing the catalog ACL requires ...", &message));
    QVERIFY(!BAclMatrix::changeSucceeded("", &message));
}

void BAclMatrixTest::dialogSendsTheChangesInOrder()
{
    BAclMatrixDialog dialog;
    dialog.setReply(QString::fromUtf8(kReply));
    QTableWidget *table = dialog.table();
    auto rowOfName = [table](const QString &name, const QString &afterTitle) {
        bool inGroup = false;
        for (int r = 0; r < table->rowCount(); ++r) {
            const QString text = table->verticalHeaderItem(r)->text();
            if (!text.startsWith("  ")) inGroup = text.startsWith(afterTitle);
            else if (inGroup && text.trimmed() == name) return r;
        }
        return -1;
    };
    const int backupB = rowOfName("backup-b", "Jobs");
    const int del = rowOfName("delete", "Commands");
    QVERIFY(backupB > 0 && del > 0);

    // operator-mcp (column 1): backup-b denied; admin (column 0): backup-b allowed again
    QVERIFY(dialog.toggle(backupB, 1));
    QCOMPARE(table->item(backupB, 1)->text(), QString("⊘"));
    QVERIFY(table->item(backupB, 1)->font().underline());
    QVERIFY(dialog.toggle(backupB, 0));
    QCOMPARE(table->item(backupB, 0)->text(), QString("✓"));
    QCOMPARE(dialog.pendingCount(), 2);
    // What the file denies, and a locked console, cannot be changed here
    QVERIFY(!dialog.toggle(del, 1));
    QVERIFY(!dialog.toggle(backupB, 3));
    QVERIFY(!dialog.toggle(backupB, 2));
    // Turned twice: nothing pending for it
    QVERIFY(dialog.toggle(backupB, 1));
    QVERIFY(dialog.toggle(backupB, 1));
    QCOMPARE(dialog.pendingCount(), 2);

    QStringList sent;
    QObject::connect(&dialog, &BAclMatrixDialog::changeRequested, [&](const QString &c) { sent << c; });
    int refreshes = 0;
    QObject::connect(&dialog, &BAclMatrixDialog::refreshRequested, [&]() { ++refreshes; });
    QString summary;
    dialog.setConfirm([&](QWidget *, const QString &text) { summary = text; return true; });
    dialog.apply();
    QVERIFY2(summary.contains("now: backup-a") && summary.contains("new: backup-b, backup-a"), qPrintable(summary));
    // One at a time: the next only after the reply
    QCOMPARE(sent.size(), 1);
    QCOMPARE(sent[0], QString(".acl set console=\"admin\" jobacl=\"backup-b,backup-a\""));
    dialog.setChangeReply("{\"result\":{\"acl\":{\"action\":\"set\"}}}");
    QCOMPARE(sent.size(), 2);
    QCOMPARE(sent[1], QString(".acl set console=\"operator-mcp\" jobacl=\"!backup-b,*all*\""));
    dialog.setChangeReply("{\"result\":{\"acl\":{\"action\":\"set\"}}}");
    QCOMPARE(refreshes, 1);
    QCOMPARE(dialog.pendingCount(), 0);

    // A refusal stops the rest
    QVERIFY(dialog.toggle(backupB, 1));
    QVERIFY(dialog.toggle(rowOfName("backup-a", "Jobs"), 1));
    QCOMPARE(dialog.pendingCount(), 1);     // the same list of operator-mcp
    QVERIFY(dialog.toggle(rowOfName("status", "Commands"), 1));
    QCOMPARE(dialog.pendingCount(), 2);
    sent.clear();
    dialog.apply();
    QCOMPARE(sent.size(), 1);
    dialog.setChangeReply("{\"error\":{\"code\":1,\"message\":\"failed\",\"data\":{\"messages\":{\"error\":[\"refused, may only restrict\"]}}}}");
    QCOMPARE(sent.size(), 1);
    QCOMPARE(refreshes, 2);
    // Not confirmed: nothing is sent
    QVERIFY(dialog.toggle(backupB, 1));
    dialog.setConfirm([](QWidget *, const QString &) { return false; });
    sent.clear();
    dialog.apply();
    QVERIFY(sent.isEmpty());
    QCOMPARE(dialog.pendingCount(), 1);
}

void BAclMatrixTest::newConsoleAndCancel()
{
    BAclMatrixDialog dialog;
    dialog.setReply(QString::fromUtf8(kReply));
    QCOMPARE(dialog.knownProfiles(), QStringList{"operator"});

    QStringList offeredProfiles, offeredTaken;
    dialog.setConsoleForm([&](QWidget *, const QStringList &profiles, const QStringList &taken,
                              QString *name, QString *password, QString *profile) {
        offeredProfiles = profiles;
        offeredTaken = taken;
        *name = "viewer";
        *password = "s3cret-pass";
        *profile = "operator";
        return true;
    });
    QList<QStringList> requested;
    QObject::connect(&dialog, &BAclMatrixDialog::createConsoleRequested,
                     [&](const QString &n, const QString &p, const QString &pr) { requested << QStringList{n, p, pr}; });
    int refreshes = 0;
    QObject::connect(&dialog, &BAclMatrixDialog::refreshRequested, [&]() { ++refreshes; });

    dialog.newConsole();
    QCOMPARE(requested.size(), 1);
    QCOMPARE(requested[0], (QStringList{"viewer", "s3cret-pass", "operator"}));
    QCOMPARE(offeredTaken.size(), 4);
    // Only one at a time
    dialog.newConsole();
    QCOMPARE(requested.size(), 1);
    // Refused: said so, nothing read again
    dialog.setCreateResult(false, "Resource console viewer already exists");
    QCOMPARE(refreshes, 0);
    // Created: the ACLs are read again, the new console becomes a column
    dialog.newConsole();
    dialog.setCreateResult(true, QString());
    QCOMPARE(refreshes, 1);
    // An answer nobody asked for changes nothing
    dialog.setCreateResult(true, QString());
    QCOMPARE(refreshes, 1);

    // Cancelled form: nothing requested
    dialog.setConsoleForm([](QWidget *, const QStringList &, const QStringList &, QString *, QString *, QString *) {
        return false;
    });
    dialog.newConsole();
    QCOMPARE(requested.size(), 2);

    // Cancel closes the window (nothing pending: no question)
    dialog.show();
    QPushButton *cancel = nullptr;
    for (QPushButton *b : dialog.findChildren<QPushButton *>()) {
        if (b->text() == QLatin1String("Cancel")) cancel = b;
    }
    QVERIFY(cancel);
    cancel->click();
    QVERIFY(!dialog.isVisible());
    QCOMPARE(dialog.result(), int(QDialog::Rejected));
}

void BAclMatrixTest::profilesProposeText()
{
    const QString profilesReply = QStringLiteral(
        "{\"profiles\":{\"operator\":{\"name\":\"operator\",\"description\":\"Normal \\\"ops\\\"\","
        "\"jobacl\":[\"*all*\"],\"commandacl\":[\"!delete\",\"*all*\"],\"whereacl\":[\"*all*\"]},"
        "\"readonly\":{\"name\":\"readonly\",\"jobacl\":[\"*all*\"],\"commandacl\":[\"status\",\"list\"]}}}");
    QList<BConsoleAcl> profiles;
    QVERIFY(BAclMatrix::parseProfiles(profilesReply, &profiles));
    QCOMPARE(profiles.size(), 2);
    QCOMPARE(profiles[0].name, QString("operator"));
    QCOMPARE(BAclMatrix::fileList(profiles[0], "commandacl"), (QStringList{"!delete", "*all*"}));

    // Lists after a change: own entry removed, negation or name in front
    QCOMPARE(BAclMatrix::fileListAfter({"!delete", "*all*"}, "delete", true), QStringList{"*all*"});
    QCOMPARE(BAclMatrix::fileListAfter({"*all*"}, "run", false), (QStringList{"!run", "*all*"}));
    QCOMPARE(BAclMatrix::fileListAfter({"status", "list"}, "list", false), QStringList{"status"});
    QCOMPARE(BAclMatrix::fileListAfter({"status"}, "run", true), (QStringList{"run", "status"}));

    // The text: every directive as Bareos spells it, quoted, the changed list new
    const QString text = BAclMatrix::profileText(profiles[0], {{"commandacl", {"*all*"}}});
    QCOMPARE(text, QString("Profile {\n"
                           "  Name = \"operator\"\n"
                           "  Description = \"Normal \\\"ops\\\"\"\n"
                           "  JobAcl = \"*all*\"\n"
                           "  CommandAcl = \"*all*\"\n"
                           "  WhereAcl = \"*all*\"\n"
                           "}\n"));

    // In the dialog: profiles as columns, a change proposes the text; nothing is sent
    BAclMatrixDialog dialog;
    dialog.setReply(QString::fromUtf8(kReply));
    dialog.setProfilesReply(profilesReply);
    QStringList sent;
    QObject::connect(&dialog, &BAclMatrixDialog::changeRequested, [&](const QString &c) { sent << c; });
    QString shown;
    dialog.setTextView([&](QWidget *, const QString &t) { shown = t; });
    dialog.setView(BAclMatrixDialog::View::Profiles);
    QTableWidget *table = dialog.table();
    QCOMPARE(table->columnCount(), 2);
    QCOMPARE(table->horizontalHeaderItem(0)->text(), QString("operator"));
    QVERIFY(table->horizontalHeaderItem(0)->toolTip().contains("Used by: operator-mcp"));
    int del = -1;
    for (int r = 0; r < table->rowCount(); ++r) {
        if (table->verticalHeaderItem(r)->text().trimmed() == "delete") del = r;
    }
    QVERIFY(del > 0);
    QCOMPARE(table->item(del, 0)->text(), QString("✗"));
    QVERIFY(dialog.toggle(del, 0));
    QCOMPARE(table->item(del, 0)->text(), QString("✓"));
    QCOMPARE(dialog.pendingCount(), 1);
    dialog.apply();
    QVERIFY2(shown.contains("# /etc/bareos/bareos-dir.d/profile/operator.conf"), qPrintable(shown));
    QVERIFY2(shown.contains("CommandAcl = \"*all*\""), qPrintable(shown));
    QVERIFY(sent.isEmpty());
    // Back in the console view: its own pending changes, none
    dialog.setView(BAclMatrixDialog::View::Consoles);
    QCOMPARE(table->columnCount(), 4);
    QCOMPARE(dialog.pendingCount(), 0);
    // The Director has it now: no longer pending
    dialog.setView(BAclMatrixDialog::View::Profiles);
    QString applied = profilesReply;
    applied.replace("\"commandacl\":[\"!delete\",\"*all*\"]", "\"commandacl\":[\"*all*\"]");
    dialog.setProfilesReply(applied);
    QCOMPARE(dialog.pendingCount(), 0);
}

void BAclMatrixTest::newProfileIsAProposal()
{
    const QString profilesReply = QStringLiteral(
        "{\"profiles\":{\"operator\":{\"name\":\"operator\",\"jobacl\":[\"*all*\"],"
        "\"commandacl\":[\"!delete\",\"*all*\"]}}}");
    BAclMatrixDialog dialog;
    dialog.setReply(QString::fromUtf8(kReply));
    dialog.setProfilesReply(profilesReply);
    dialog.setView(BAclMatrixDialog::View::Profiles);
    dialog.setProfileForm([](QWidget *, const QStringList &profiles, QString *name, QString *description,
                             QString *copyOf) {
        if (!profiles.contains("operator")) return false;
        *name = "restore-only";
        *description = "Restores, nothing else";
        *copyOf = "operator";
        return true;
    });
    QString shown;
    dialog.setTextView([&](QWidget *, const QString &t) { shown = t; });

    dialog.newProfile();
    QTableWidget *table = dialog.table();
    QCOMPARE(table->columnCount(), 2);
    QCOMPARE(table->horizontalHeaderItem(1)->text(), QString("restore-only (new)"));
    QCOMPARE(dialog.pendingCount(), 1);     // the new profile itself
    // A console cannot use it before the Director has it
    QVERIFY(!dialog.knownProfiles().contains("restore-only"));

    // Copied from operator: delete denied; turned, allowed in the proposal
    int del = -1;
    for (int r = 0; r < table->rowCount(); ++r) {
        if (table->verticalHeaderItem(r)->text().trimmed() == "delete") del = r;
    }
    QVERIFY(del > 0);
    QCOMPARE(table->item(del, 1)->text(), QString("✗"));
    QVERIFY(dialog.toggle(del, 1));
    dialog.apply();
    QVERIFY2(shown.contains("# /etc/bareos/bareos-dir.d/profile/restore-only.conf\n"
                            "Profile {\n  Name = \"restore-only\"\n  Description = \"Restores, nothing else\"\n"
                            "  JobAcl = \"*all*\"\n  CommandAcl = \"*all*\"\n}\n"), qPrintable(shown));
    QVERIFY(!shown.contains("profile/operator.conf"));      // operator is unchanged

    // Refreshed before the text is in: the proposal stays
    dialog.setProfilesReply(profilesReply);
    QCOMPARE(table->columnCount(), 2);
    // The Director has it now: an ordinary column
    dialog.setProfilesReply(QStringLiteral(
        "{\"profiles\":{\"operator\":{\"name\":\"operator\",\"jobacl\":[\"*all*\"],\"commandacl\":[\"!delete\",\"*all*\"]},"
        "\"restore-only\":{\"name\":\"restore-only\",\"jobacl\":[\"*all*\"],\"commandacl\":[\"*all*\"]}}}"));
    QCOMPARE(table->columnCount(), 2);
    QCOMPARE(table->horizontalHeaderItem(1)->text(), QString("restore-only"));
    QCOMPARE(dialog.pendingCount(), 0);

    // Discard removes a proposed profile
    dialog.newProfile();    // the form refuses nothing here: a second one of a new name
    dialog.discard();
    QCOMPARE(table->columnCount(), 2);
}

QTEST_MAIN(BAclMatrixTest)
#include "baclmatrix_test.moc"
