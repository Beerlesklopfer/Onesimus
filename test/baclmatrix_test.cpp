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

QTEST_MAIN(BAclMatrixTest)
#include "baclmatrix_test.moc"
