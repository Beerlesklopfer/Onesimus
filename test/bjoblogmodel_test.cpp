/**
 * @file bjoblogmodel_test.cpp
 * @brief Job log model, its sources and the push message parser
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>

#include "director/bjobpushmessage.h"
#include "jobs/bjoblogmodel.h"
#include "jobs/bjoblogsource.h"

using BJobLog::Entry;
using BJobLog::LineKind;

namespace {

/// Termination summary as `list joblog` returns it: one record, many lines
const char *SummaryText =
    "bareos-dir JobId 13: Bareos bareos-dir 23.0.3 (11Jun24):\n"
    "  Build OS:               Debian GNU/Linux 12 (bookworm)\n"
    "  JobId:                  13\n"
    "  Job:                    backup-pdc.2026-11-17_21.00.00_04\n"
    "  Backup Level:           Incremental, since=2026-11-16 21:00:03\n"
    "  Client:                 \"pdc-fd\" 23.0.3 (11Jun24) Debian GNU/Linux 12 (bookworm),debian\n"
    "  FileSet:                \"LinuxAll\" 2026-07-09 21:00:00\n"
    "  Pool:                   \"Incremental\" (From Job IncPool override)\n"
    "  Catalog:                \"MyCatalog\" (From Client resource)\n"
    "  Storage:                \"File\" (From Job resource)\n"
    "  Scheduled time:         17-Nov-2026 21:00:00\n"
    "  Start time:             17-Nov-2026 21:00:03\n"
    "  End time:               17-Nov-2026 21:00:41\n"
    "  Elapsed time:           38 secs\n"
    "  Priority:               10\n"
    "  Allow Mixed Priority:   no\n"
    "  FD Files Written:       1,204\n"
    "  SD Files Written:       1,204\n"
    "  FD Bytes Written:       918,331,201 (918.3 MB)\n"
    "  SD Bytes Written:       918,512,880 (918.5 MB)\n"
    "  Rate:                   24166.6 KB/s\n"
    "  Software Compression:   None\n"
    "  VSS:                    no\n"
    "  Encryption:             no\n"
    "  Accurate:               no\n"
    "  Volume name(s):         Incremental-0007\n"
    "  Volume Session Id:      4\n"
    "  Volume Session Time:    1763400000\n"
    "  Last Volume Bytes:      2,918,512,880 (2.918 GB)\n"
    "  Non-fatal FD errors:    0\n"
    "  SD Errors:              0\n"
    "  FD termination status:  OK\n"
    "  SD termination status:  OK\n"
    "  Bareos binary info:     Onesimus build: Support: https://onesimus.io\n"
    "  Job triggered by:       Scheduler\n"
    "  Termination:            Backup OK\n"
    "\n";
const int SummaryLines = 36;

Entry entry(const QString &text, const QString &time = QStringLiteral("2026-11-17 21:00:03"),
            qint64 logId = 0)
{
    Entry e;
    e.logId = logId;
    e.time = time;
    e.text = text;
    return e;
}

/// Reply in the shape of the BNET `.api 2` / gRPC JSON output
QString joblogReply(const QList<Entry> &entries, bool withLogId = false)
{
    QJsonArray array;
    for (const Entry &e : entries) {
        QJsonObject record{{"time", e.time}, {"logtext", e.text}};
        if (withLogId) {
            record.insert("logid", QString::number(e.logId));
        }
        array.append(record);
    }
    const QJsonObject root{{"jsonrpc", "2.0"}, {"id", QJsonValue::Null},
                           {"result", QJsonObject{{"joblog", array}}}};
    return QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
}

QList<Entry> startEntries()
{
    return {
        entry("bareos-dir JobId 13: Start Backup JobId 13, Job=backup-pdc.2026-11-17_21.00.00_04\n"),
        entry("bareos-dir JobId 13: Connected Storage daemon at pdc:9103, encryption: TLS_CHACHA20_POLY1305_SHA256 TLSv1.3\n"),
        entry("bareos-dir JobId 13: Using Device \"FileStorage\" to write.\n"),
    };
}

QStringList displayLines(const BJobLogModel &model)
{
    return model.logLines();
}

bool hasBackground(const BJobLogModel &model, int row)
{
    return model.data(model.index(row), Qt::BackgroundRole).isValid();
}

int rowOf(const BJobLogModel &model, const QString &needle)
{
    const QStringList lines = model.logLines();
    for (int i = 0; i < lines.size(); ++i) {
        if (lines.at(i).contains(needle)) {
            return i;
        }
    }
    return -1;
}

} // namespace

class BJobLogModelTest : public QObject
{
    Q_OBJECT

private slots:
    // --- line splitting ---------------------------------------------------
    void splitsMultiLineSummaryIntoRows();
    void parsesReplyShapes();

    // --- merging ----------------------------------------------------------
    void mergesOverlappingAndRepeatedLoads();
    void mergesByLogIdWhenDelivered();
    void catalogLoadReplacesPushedRows();
    void recentlyPushedRowsSurviveUnlessInCatalog();
    void ignoresOtherJob();

    // --- colouring --------------------------------------------------------
    void summaryOfSuccessfulJobIsNotFlagged();
    void errorLinesAreFlagged();
    void backgroundAlwaysComesWithForeground();

    // --- helpers ----------------------------------------------------------
    void followTailDecision();
    void jobStatusClassification();
    void parsesPushPrefix();

    // --- sources ----------------------------------------------------------
    void pollingFetchesFinishedJobOnce();
    void pollingRepeatsUntilJobEnds();
    void pollingStopsOnTerminationSummary();
    void sourceIgnoresReplyForOtherJob();
    void pushAppendsThenReloadsAtJobEnd();
};

// ---------------------------------------------------------------------------
// Line splitting
// ---------------------------------------------------------------------------

void BJobLogModelTest::splitsMultiLineSummaryIntoRows()
{
    const Entry summary = entry(QString::fromLatin1(SummaryText));
    const QStringList lines = BJobLog::entryLines(summary);

    QCOMPARE(lines.size(), SummaryLines);
    QCOMPARE(lines.first(),
             QStringLiteral("[2026-11-17 21:00:03] bareos-dir JobId 13: Bareos bareos-dir 23.0.3 (11Jun24):"));
    // Continuation lines are indented to the text column of the first line
    const QString indent(QStringLiteral("[2026-11-17 21:00:03] ").size(), QLatin1Char(' '));
    for (int i = 1; i < lines.size(); ++i) {
        QVERIFY2(lines.at(i).startsWith(indent + QStringLiteral("  ")), qPrintable(lines.at(i)));
        QVERIFY(!lines.at(i).contains(QLatin1Char('\n')));
    }
    QVERIFY(lines.last().endsWith(QStringLiteral("Termination:            Backup OK")));

    // In the model: one row per line, no filler rows
    BJobLogModel model;
    model.setJob(13);
    QVERIFY(model.appendCatalogEntries(13, startEntries() + QList<Entry>{summary}));
    QCOMPARE(model.rowCount(), 3 + SummaryLines);
    QVERIFY(!displayLines(model).last().trimmed().isEmpty());
    QVERIFY(model.hasTermination());

    // Records without text produce no row
    QVERIFY(BJobLog::entryLines(entry(QStringLiteral("\n\n"))).isEmpty());
    // Without a time there is no prefix
    QCOMPARE(BJobLog::entryLines(entry(QStringLiteral("a\nb"), QString())),
             QStringList({QStringLiteral("a"), QStringLiteral("    b")}));
}

void BJobLogModelTest::parsesReplyShapes()
{
    const QList<Entry> entries = startEntries();

    // {"result":{"joblog":[..]}}
    BJobLog::ParsedReply parsed = BJobLog::parseCatalogReply(joblogReply(entries));
    QVERIFY(!parsed.isError);
    QCOMPARE(parsed.entries.size(), 3);
    QCOMPARE(parsed.entries.at(0).time, QStringLiteral("2026-11-17 21:00:03"));
    QCOMPARE(parsed.entries.at(0).text, entries.at(0).text);
    QCOMPARE(parsed.entries.at(0).logId, qint64(0));   // the reply carries no logid

    // {"joblog":[..]}, {"log":[..]}, top-level array, strings as records
    QCOMPARE(BJobLog::parseCatalogReply(R"({"joblog":[{"time":"t","logtext":"x"}]})").entries.size(), 1);
    QCOMPARE(BJobLog::parseCatalogReply(R"({"log":[{"time":"t","logtext":"x"},{"logtext":"y"}]})").entries.size(), 2);
    QCOMPARE(BJobLog::parseCatalogReply(R"(["one","two","three"])").entries.size(), 3);

    // logid as string (API mode 2) or number
    parsed = BJobLog::parseCatalogReply(
        R"({"result":{"joblog":[{"logid":"41","time":"t","logtext":"x"},{"logid":42,"time":"t","logtext":"y"}]}})");
    QCOMPARE(parsed.entries.at(0).logId, qint64(41));
    QCOMPARE(parsed.entries.at(1).logId, qint64(42));

    // Empty result and error object
    parsed = BJobLog::parseCatalogReply(R"({"jsonrpc":"2.0","id":null,"result":{}})");
    QVERIFY(!parsed.isError);
    QVERIFY(parsed.entries.isEmpty());
    parsed = BJobLog::parseCatalogReply(R"({"jsonrpc":"2.0","id":null,"error":{"code":1,"message":"No Job found"}})");
    QVERIFY(parsed.isError);
    QCOMPARE(parsed.error, QStringLiteral("No Job found"));
}

// ---------------------------------------------------------------------------
// Merging
// ---------------------------------------------------------------------------

void BJobLogModelTest::mergesOverlappingAndRepeatedLoads()
{
    BJobLogModel model;
    model.setJob(13);

    QSignalSpy resets(&model, &QAbstractItemModel::modelReset);
    QSignalSpy inserts(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy removes(&model, &QAbstractItemModel::rowsRemoved);

    QList<Entry> entries = startEntries();
    QVERIFY(model.appendCatalogEntries(13, entries));
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(inserts.count(), 1);
    const QStringList firstLoad = displayLines(model);

    // The same load again: nothing happens
    QVERIFY(model.appendCatalogEntries(13, entries));
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(inserts.count(), 1);

    // An overlapping load (the job went on): only the new records are appended
    entries.append(entry("bareos-sd JobId 13: Wrote label to prelabeled Volume \"Incremental-0007\"\n",
                         "2026-11-17 21:00:05"));
    entries.append(entry("", "2026-11-17 21:00:06"));   // empty record: counted, no row
    entries.append(entry(QString::fromLatin1(SummaryText), "2026-11-17 21:00:41"));
    QVERIFY(model.appendCatalogEntries(13, entries));
    QCOMPARE(model.catalogEntryCount(), 6);
    QCOMPARE(model.rowCount(), 3 + 1 + SummaryLines);
    QCOMPARE(inserts.count(), 2);
    QCOMPARE(inserts.last().at(1).toInt(), 3);   // appended behind the rows held
    QCOMPARE(displayLines(model).mid(0, 3), firstLoad);

    // Repeated and shorter (stale) loads change nothing
    QVERIFY(model.appendCatalogEntries(13, entries));
    QVERIFY(model.appendCatalogEntries(13, startEntries()));
    QCOMPARE(model.rowCount(), 3 + 1 + SummaryLines);
    QCOMPARE(inserts.count(), 2);

    QCOMPARE(resets.count(), 0);
    QCOMPARE(removes.count(), 0);

    // setJob() is the reset
    model.setJob(13);
    QCOMPARE(resets.count(), 1);
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(model.catalogEntryCount(), 0);
}

void BJobLogModelTest::mergesByLogIdWhenDelivered()
{
    BJobLogModel model;
    model.setJob(5);

    QVERIFY(model.appendCatalogEntries(5, {entry("one", "t", 101), entry("two", "t", 102),
                                           entry("three", "t", 103)}));
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(model.lastLogId(), qint64(103));

    // Overlap by id, independent of the position in the reply (e.g. a paged load)
    QVERIFY(model.appendCatalogEntries(5, {entry("three", "t", 103), entry("four", "t", 104)}));
    QCOMPARE(model.rowCount(), 4);
    QCOMPARE(model.lastLogId(), qint64(104));
    QCOMPARE(model.data(model.index(3), BJobLogModel::LogIdRole).toLongLong(), qint64(104));

    // Late records with an older id are not inserted (append-only)
    QVERIFY(model.appendCatalogEntries(5, {entry("late", "t", 100), entry("five", "t", 105)}));
    QCOMPARE(displayLines(model), QStringList({"[t] one", "[t] two", "[t] three", "[t] four", "[t] five"}));

    // Through the JSON path
    model.setJob(5);
    const BJobLog::ParsedReply parsed = BJobLog::parseCatalogReply(
        joblogReply({entry("one", "t", 7), entry("two", "t", 8)}, true));
    QVERIFY(model.appendCatalogEntries(5, parsed.entries));
    QCOMPARE(model.lastLogId(), qint64(8));
}

void BJobLogModelTest::catalogLoadReplacesPushedRows()
{
    BJobLogModel model;
    model.setJob(13);
    QList<Entry> entries = startEntries();
    QVERIFY(model.appendCatalogEntries(13, entries));

    // Pushed lines have neither time nor logid and form the tail
    QVERIFY(model.appendPushText(13, "Wrote label to prelabeled Volume \"Incremental-0007\"\n") >= 0);
    QVERIFY(model.appendPushText(13, "Elapsed time=00:00:38, Transfer rate=24.16 M Bytes/second") >= 0);
    QVERIFY(model.appendPushText(13, "first line\n  second line\n") >= 0);
    QCOMPARE(model.rowCount(), 3 + 4);
    QCOMPARE(model.pushRowCount(), 4);
    QVERIFY(model.data(model.index(3), BJobLogModel::PushRole).toBool());
    QCOMPARE(model.data(model.index(3)).toString(),
             QStringLiteral("Wrote label to prelabeled Volume \"Incremental-0007\""));

    // A load without anything new leaves the pushed rows alone
    QVERIFY(model.appendCatalogEntries(13, entries));
    QCOMPARE(model.pushRowCount(), 4);

    QSignalSpy resets(&model, &QAbstractItemModel::modelReset);
    QSignalSpy changes(&model, &QAbstractItemModel::dataChanged);
    QSignalSpy inserts(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy removes(&model, &QAbstractItemModel::rowsRemoved);
    const QStringList head = displayLines(model).mid(0, 3);

    // The catalog load is authoritative: 2 new records replace the 4 pushed rows
    entries.append(entry("bareos-sd JobId 13: Wrote label to prelabeled Volume \"Incremental-0007\"\n",
                         "2026-11-17 21:00:05"));
    entries.append(entry("bareos-sd JobId 13: Elapsed time=00:00:38, Transfer rate=24.16 M Bytes/second\n",
                         "2026-11-17 21:00:41"));
    QVERIFY(model.appendCatalogEntries(13, entries));

    QCOMPARE(model.rowCount(), 5);
    QCOMPARE(model.pushRowCount(), 0);
    QCOMPARE(displayLines(model).mid(0, 3), head);
    QCOMPARE(displayLines(model).at(3),
             QStringLiteral("[2026-11-17 21:00:05] bareos-sd JobId 13: Wrote label to prelabeled Volume \"Incremental-0007\""));
    QVERIFY(!model.data(model.index(3), BJobLogModel::PushRole).toBool());

    // Replaced in place: the tail is rewritten and trimmed, the model is not reset
    QCOMPARE(resets.count(), 0);
    QCOMPARE(changes.count(), 1);
    QCOMPARE(changes.first().at(0).toModelIndex().row(), 3);
    QCOMPARE(changes.first().at(1).toModelIndex().row(), 4);
    QCOMPARE(inserts.count(), 0);
    QCOMPARE(removes.count(), 1);
    QCOMPARE(removes.first().at(1).toInt(), 5);
    QCOMPARE(removes.first().at(2).toInt(), 6);

    // More catalog rows than pushed rows: overwrite, then insert the rest
    QVERIFY(model.appendPushText(13, "pushed") >= 0);
    entries.append(entry(QString::fromLatin1(SummaryText), "2026-11-17 21:00:41"));
    QVERIFY(model.appendCatalogEntries(13, entries));
    QCOMPARE(model.rowCount(), 5 + SummaryLines);
    QCOMPARE(model.pushRowCount(), 0);
    QCOMPARE(resets.count(), 0);
    QCOMPARE(inserts.count(), 2);   // the pushed row, then the rest of the summary
}

void BJobLogModelTest::recentlyPushedRowsSurviveUnlessInCatalog()
{
    BJobLogModel model;
    model.setJob(13);
    QList<Entry> entries = startEntries();
    QVERIFY(model.appendCatalogEntries(13, entries));

    const int old = model.appendPushText(13, "old message");
    const int inCatalog = model.appendPushText(13, "Wrote label to prelabeled Volume \"Incremental-0007\"");
    const int tooNew = model.appendPushText(13, "pushed after the query ran");
    QCOMPARE(old, 0);
    QCOMPARE(inCatalog, 1);
    QCOMPARE(tooNew, 2);

    entries.append(entry("bareos-sd JobId 13: Wrote label to prelabeled Volume \"Incremental-0007\"\n"));
    QVERIFY(model.appendCatalogEntries(13, entries, inCatalog));

    // "old" is replaced, the second one is part of the load, the third one is kept
    QCOMPARE(model.rowCount(), 5);
    QCOMPARE(model.pushRowCount(), 1);
    QCOMPARE(displayLines(model).last(), QStringLiteral("pushed after the query ran"));
    QVERIFY(model.data(model.index(4), BJobLogModel::PushRole).toBool());

    QVERIFY(model.recentCatalogContains("Wrote label to prelabeled Volume \"Incremental-0007\""));
    QVERIFY(!model.recentCatalogContains("pushed after the query ran"));

    // The next load brings it from the catalog
    entries.append(entry("bareos-dir JobId 13: pushed after the query ran\n"));
    QVERIFY(model.appendCatalogEntries(13, entries));
    QCOMPARE(model.rowCount(), 5);
    QCOMPARE(model.pushRowCount(), 0);
}

void BJobLogModelTest::ignoresOtherJob()
{
    BJobLogModel model;
    model.setJob(7);
    QSignalSpy inserts(&model, &QAbstractItemModel::rowsInserted);

    QVERIFY(!model.appendCatalogEntries(8, startEntries()));
    QCOMPARE(model.appendPushText(8, "text"), -1);
    QVERIFY(!model.appendCatalogEntries(0, startEntries()));
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(inserts.count(), 0);

    QVERIFY(model.appendCatalogEntries(7, startEntries()));
    QCOMPARE(model.rowCount(), 3);

    // A snapshot (history) is detached from any job
    model.setLogLines({"a", "b"});
    QCOMPARE(model.jobId(), quint64(0));
    QVERIFY(!model.appendCatalogEntries(7, startEntries()));
    QCOMPARE(model.logLines(), QStringList({"a", "b"}));
}

// ---------------------------------------------------------------------------
// Colouring
// ---------------------------------------------------------------------------

void BJobLogModelTest::summaryOfSuccessfulJobIsNotFlagged()
{
    BJobLogModel model;
    model.setJob(13);
    QVERIFY(model.appendCatalogEntries(13, startEntries()
                                               + QList<Entry>{entry(QString::fromLatin1(SummaryText))}));

    const int termination = rowOf(model, "Termination:");
    QVERIFY(termination > 0);
    for (int row = 0; row < model.rowCount(); ++row) {
        if (row == termination) {
            continue;
        }
        QVERIFY2(!hasBackground(model, row), qPrintable(model.data(model.index(row)).toString()));
    }
    QVERIFY(rowOf(model, "Non-fatal FD errors:") > 0);
    QVERIFY(rowOf(model, "SD Errors:") > 0);

    // "Termination: Backup OK" keeps its own (green) mark
    QCOMPARE(model.data(model.index(termination), Qt::BackgroundRole).value<QBrush>().color(),
             QColor(200, 255, 200));

    // The same through a snapshot (log history)
    BJobLogModel snapshot;
    snapshot.setLogLines(model.logLines());
    QVERIFY(!hasBackground(snapshot, rowOf(snapshot, "Non-fatal FD errors:")));
    QVERIFY(!hasBackground(snapshot, rowOf(snapshot, "SD Errors:")));
    QVERIFY(hasBackground(snapshot, termination));
}

void BJobLogModelTest::errorLinesAreFlagged()
{
    QCOMPARE(BJobLog::classifyLine("[t] bareos-dir JobId 14: Fatal error: Bareos cannot connect to pdc-fd", false),
             LineKind::Fatal);
    QCOMPARE(BJobLog::classifyLine("[t] bareos-dir JobId 14: Error: Bareos bareos-dir 23.0.3 (11Jun24):", false),
             LineKind::Error);
    QCOMPARE(BJobLog::classifyLine("[t] pdc-fd JobId 14: Warning: Could not stat \"/proc/1\": ERR=No such file", false),
             LineKind::Warning);
    QCOMPARE(BJobLog::classifyLine("[t] bareos-sd JobId 14: Job write elapsed time failed", false),
             LineKind::Error);
    QCOMPARE(BJobLog::classifyLine("[t] bareos-dir JobId 14: Non-fatal error while pruning", false),
             LineKind::Error);

    // Whole words only
    QCOMPARE(BJobLog::classifyLine("[t] bareos-dir JobId 14: Using errorlog /var/log/terrors", false),
             LineKind::Normal);
    QCOMPARE(BJobLog::classifyLine("[t] bareos-dir JobId 14: Forewarned is forearmed", false),
             LineKind::Normal);
    QCOMPARE(BJobLog::classifyLine("[t] bareos-dir JobId 14: Start Backup JobId 14", false),
             LineKind::Normal);

    // Summary lines: only the termination line is rated
    QCOMPARE(BJobLog::classifyLine("  Non-fatal FD errors:    3", true), LineKind::Normal);
    QCOMPARE(BJobLog::classifyLine("  SD Errors:              0", true), LineKind::Normal);
    QCOMPARE(BJobLog::classifyLine("  FD termination status:  Error", true), LineKind::Normal);
    QCOMPARE(BJobLog::classifyLine("  Termination:            Backup OK", true), LineKind::Success);
    QCOMPARE(BJobLog::classifyLine("  Termination:            Backup OK -- with warnings", true),
             LineKind::Warning);
    QCOMPARE(BJobLog::classifyLine("  Termination:            *** Backup Error ***", true), LineKind::Error);
    QCOMPARE(BJobLog::classifyLine("  Termination:            Backup Canceled", true), LineKind::Warning);

    // A failed job in the model: the error line and the termination line are marked
    QString failedSummary = QString::fromLatin1(SummaryText);
    failedSummary.replace("Non-fatal FD errors:    0", "Non-fatal FD errors:    2");
    failedSummary.replace("Termination:            Backup OK", "Termination:            *** Backup Error ***");

    BJobLogModel model;
    model.setJob(14);
    QVERIFY(model.appendCatalogEntries(14, {
        entry("bareos-dir JobId 14: Start Backup JobId 14\n"),
        entry("bareos-dir JobId 14: Fatal error: Network error with FD during Backup: ERR=Connection reset by peer\n"),
        entry(failedSummary),
    }));
    QVERIFY(!hasBackground(model, 0));
    QCOMPARE(model.data(model.index(1), Qt::BackgroundRole).value<QBrush>().color(), QColor(255, 200, 200));
    QVERIFY(!hasBackground(model, rowOf(model, "Non-fatal FD errors:")));
    QVERIFY(hasBackground(model, rowOf(model, "Termination:")));
}

void BJobLogModelTest::backgroundAlwaysComesWithForeground()
{
    BJobLogModel model;
    model.setJob(14);
    QVERIFY(model.appendCatalogEntries(14, {
        entry("bareos-dir JobId 14: Start Backup JobId 14\n"),
        entry("bareos-dir JobId 14: Fatal error: no connection\n"),
        entry("bareos-dir JobId 14: Error: something\n"),
        entry("bareos-dir JobId 14: Warning: something\n"),
        entry(QString::fromLatin1(SummaryText)),
    }));
    model.appendPushText(14, "Error: pushed line");

    int marked = 0;
    for (int row = 0; row < model.rowCount(); ++row) {
        const QVariant background = model.data(model.index(row), Qt::BackgroundRole);
        const QVariant foreground = model.data(model.index(row), Qt::ForegroundRole);
        QCOMPARE(foreground.isValid(), background.isValid());
        if (background.isValid()) {
            ++marked;
            // Readable: clearly different luminance
            const QColor bg = background.value<QBrush>().color();
            const QColor fg = foreground.value<QBrush>().color();
            QVERIFY(qAbs(bg.lightness() - fg.lightness()) > 100);
        }
    }
    QCOMPARE(marked, 5);   // fatal, error, warning, termination, pushed error
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

void BJobLogModelTest::followTailDecision()
{
    QVERIFY(BJobLog::isAtBottom(0, 0, 4));        // empty view: first load ends at the bottom
    QVERIFY(BJobLog::isAtBottom(500, 500, 4));
    QVERIFY(BJobLog::isAtBottom(497, 500, 4));    // within tolerance
    QVERIFY(!BJobLog::isAtBottom(495, 500, 4));   // the user scrolled up
    QVERIFY(!BJobLog::isAtBottom(0, 500, 4));
    QVERIFY(BJobLog::isAtBottom(500, 500, 0));
    QVERIFY(!BJobLog::isAtBottom(499, 500, 0));
}

void BJobLogModelTest::jobStatusClassification()
{
    // Catalog JobStatus codes
    for (const char *running : {"R", "C", "F", "S", "M", "m", "s", "j", "c", "d", "t", "p", "a", "i", "q"}) {
        QVERIFY2(BJobLog::isRunningJobStatus(running), running);
    }
    for (const char *ended : {"T", "W", "E", "e", "f", "A", "D", "I", ""}) {
        QVERIFY2(!BJobLog::isRunningJobStatus(ended), ended);
    }

    // Status texts of the pushed JobStatusEvent
    for (const char *terminal : {"Success", "Error", "OK", "Canceled", "T", "f"}) {
        QVERIFY2(BJobLog::isTerminalStatusEvent(terminal), terminal);
    }
    for (const char *other : {"Queued", "Running", "Starting", "Finishing", "WaitingMedia", "R", ""}) {
        QVERIFY2(!BJobLog::isTerminalStatusEvent(other), other);
    }
}

void BJobLogModelTest::parsesPushPrefix()
{
    BJobPushMessage msg = BJobPushMessage::parse("JobId=13 backup-pdc: Start Backup JobId 13");
    QVERIFY(msg.valid);
    QCOMPARE(msg.jobId, quint64(13));
    QCOMPARE(msg.jobName, QStringLiteral("backup-pdc"));
    QCOMPARE(msg.text, QStringLiteral("Start Backup JobId 13"));

    // Job name with spaces
    msg = BJobPushMessage::parse("JobId=4711 Nightly Backup PDC: Using Device \"FileStorage\" to write.");
    QVERIFY(msg.valid);
    QCOMPARE(msg.jobId, quint64(4711));
    QCOMPARE(msg.jobName, QStringLiteral("Nightly Backup PDC"));
    QCOMPARE(msg.text, QStringLiteral("Using Device \"FileStorage\" to write."));

    // Job name with a colon; the text contains ": " as well
    msg = BJobPushMessage::parse("JobId=5 pdc:home: Elapsed time: 38 secs");
    QVERIFY(msg.valid);
    QCOMPARE(msg.jobName, QStringLiteral("pdc:home"));
    QCOMPARE(msg.text, QStringLiteral("Elapsed time: 38 secs"));

    // A name containing ": " is only split exactly when the name is known
    const QString odd = QStringLiteral("JobId=6 site: pdc home: Start Backup JobId 6");
    msg = BJobPushMessage::parse(odd);
    QVERIFY(msg.valid);
    QCOMPARE(msg.jobName, QStringLiteral("site"));
    msg = BJobPushMessage::parse(odd, QStringLiteral("site: pdc home"));
    QVERIFY(msg.valid);
    QCOMPARE(msg.jobName, QStringLiteral("site: pdc home"));
    QCOMPARE(msg.text, QStringLiteral("Start Backup JobId 6"));
    // A known name that does not fit falls back to the first ": "
    msg = BJobPushMessage::parse(odd, QStringLiteral("other"));
    QCOMPARE(msg.jobName, QStringLiteral("site"));

    // Multi-line text (the termination summary is one message)
    msg = BJobPushMessage::parse(QStringLiteral("JobId=13 backup-pdc: ") + QString::fromLatin1(SummaryText));
    QVERIFY(msg.valid);
    QCOMPARE(msg.text, QString::fromLatin1(SummaryText));

    // Empty text
    msg = BJobPushMessage::parse("JobId=13 backup-pdc: ");
    QVERIFY(msg.valid);
    QCOMPARE(msg.jobName, QStringLiteral("backup-pdc"));
    QVERIFY(msg.text.isEmpty());

    // Not a job message
    QVERIFY(!BJobPushMessage::parse("Client pdc-fd connected").valid);
    QVERIFY(!BJobPushMessage::parse("JobId= backup: x").valid);
    QVERIFY(!BJobPushMessage::parse("JobId=abc backup: x").valid);
    QVERIFY(!BJobPushMessage::parse("JobId=13").valid);
    QVERIFY(!BJobPushMessage::parse("JobId=13 no separator").valid);
    QVERIFY(!BJobPushMessage::parse("").valid);
}

// ---------------------------------------------------------------------------
// Sources
// ---------------------------------------------------------------------------

void BJobLogModelTest::pollingFetchesFinishedJobOnce()
{
    BJobLogModel model;
    BPollingJobLogSource source(&model);
    source.setPollInterval(5);
    QSignalSpy fetches(&source, &BJobLogSource::fetchRequested);
    QSignalSpy finished(&source, &BJobLogSource::finished);
    QSignalSpy updates(&source, &BJobLogSource::updated);

    source.start(13, false);
    QCOMPARE(model.jobId(), quint64(13));
    QCOMPARE(fetches.count(), 1);
    QCOMPARE(fetches.first().at(0).toULongLong(), quint64(13));
    QVERIFY(!source.isLive());

    source.catalogReply(13, joblogReply(startEntries()));
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(updates.count(), 1);
    QCOMPARE(finished.count(), 1);

    QTest::qWait(40);
    QCOMPARE(fetches.count(), 1);   // no polling for a finished job

    // A late duplicate reply (another view asked, too) is merged without effect
    source.catalogReply(13, joblogReply(startEntries()));
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(finished.count(), 1);

    // An error reply is reported, not shown as log
    QSignalSpy failures(&source, &BJobLogSource::failed);
    source.start(99, false);
    source.catalogReply(99, R"({"jsonrpc":"2.0","id":null,"error":{"code":1,"message":"No Job found"}})");
    QCOMPARE(failures.count(), 1);
    QCOMPARE(model.rowCount(), 0);
}

void BJobLogModelTest::pollingRepeatsUntilJobEnds()
{
    BJobLogModel model;
    BPollingJobLogSource source(&model);
    source.setPollInterval(5);
    QSignalSpy fetches(&source, &BJobLogSource::fetchRequested);
    QSignalSpy finished(&source, &BJobLogSource::finished);
    QSignalSpy resets(&model, &QAbstractItemModel::modelReset);

    QList<Entry> entries = startEntries();
    source.start(13, true);
    QVERIFY(source.isLive());
    QCOMPARE(resets.count(), 1);   // setJob()
    QCOMPARE(fetches.count(), 1);
    source.catalogReply(13, joblogReply(entries));
    QCOMPARE(model.rowCount(), 3);

    // The timer asks again; the log grows without a reset
    QTRY_VERIFY(fetches.count() >= 2);
    entries.append(entry("bareos-sd JobId 13: Wrote label to prelabeled Volume \"Incremental-0007\"\n"));
    source.catalogReply(13, joblogReply(entries));
    QCOMPARE(model.rowCount(), 4);
    QTRY_VERIFY(fetches.count() >= 3);
    source.catalogReply(13, joblogReply(entries));
    QCOMPARE(model.rowCount(), 4);
    QCOMPARE(finished.count(), 0);

    // Hidden view: stop() ends the polling, start() for the same job resumes without a reset
    source.stop();
    const int whileStopped = fetches.count();
    QTest::qWait(40);
    QCOMPARE(fetches.count(), whileStopped);
    source.catalogReply(13, joblogReply(entries + QList<Entry>{entry("ignored while stopped")}));
    QCOMPARE(model.rowCount(), 4);

    source.start(13, true);
    QCOMPARE(resets.count(), 1);
    QCOMPARE(fetches.count(), whileStopped + 1);
    source.catalogReply(13, joblogReply(entries));

    // The owner reports the end of the job: one final fetch, then silence
    const int beforeEnd = fetches.count();
    source.jobEnded(13);
    QCOMPARE(fetches.count(), beforeEnd + 1);
    entries.append(entry(QString::fromLatin1(SummaryText)));
    source.catalogReply(13, joblogReply(entries));
    QCOMPARE(finished.count(), 1);
    QVERIFY(!source.isLive());
    QCOMPARE(model.rowCount(), 4 + SummaryLines);

    QTest::qWait(40);
    QCOMPARE(fetches.count(), beforeEnd + 1);
    QCOMPARE(resets.count(), 1);
}

void BJobLogModelTest::pollingStopsOnTerminationSummary()
{
    // Nobody reports the end of the job (BNET, dialog): the summary in the log does
    BJobLogModel model;
    BPollingJobLogSource source(&model);
    source.setPollInterval(5);
    QSignalSpy fetches(&source, &BJobLogSource::fetchRequested);
    QSignalSpy finished(&source, &BJobLogSource::finished);

    QList<Entry> entries = startEntries();
    source.start(13, true);
    source.catalogReply(13, joblogReply(entries));
    QTRY_VERIFY(fetches.count() >= 2);

    entries.append(entry(QString::fromLatin1(SummaryText)));
    source.catalogReply(13, joblogReply(entries));
    QCOMPARE(finished.count(), 0);

    // One more fetch after the summary, its reply ends the live phase
    const int afterSummary = fetches.count();
    QTRY_COMPARE(fetches.count(), afterSummary + 1);
    entries.append(entry("bareos-dir JobId 13: End auto prune.\n"));
    source.catalogReply(13, joblogReply(entries));
    QCOMPARE(finished.count(), 1);
    QCOMPARE(model.rowCount(), 3 + SummaryLines + 1);

    QTest::qWait(40);
    QCOMPARE(fetches.count(), afterSummary + 1);
}

void BJobLogModelTest::sourceIgnoresReplyForOtherJob()
{
    // Review finding M1: arrowing through jobs A, B, C must not show A's log under C
    BJobLogModel model;
    BPollingJobLogSource source(&model);
    QSignalSpy fetches(&source, &BJobLogSource::fetchRequested);
    QSignalSpy updates(&source, &BJobLogSource::updated);

    source.start(1, false);
    source.start(2, false);
    source.start(3, false);
    QCOMPARE(fetches.count(), 3);
    QCOMPARE(model.jobId(), quint64(3));

    // The replies arrive in request order
    source.catalogReply(1, joblogReply({entry("log of job 1")}));
    source.catalogReply(2, joblogReply({entry("log of job 2"), entry("more of job 2")}));
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(updates.count(), 0);

    source.catalogReply(3, joblogReply({entry("log of job 3")}));
    QCOMPARE(model.rowCount(), 1);
    QVERIFY(model.data(model.index(0)).toString().endsWith("log of job 3"));

    // Pushed messages and status events of other jobs are ignored, too
    BPushJobLogSource push(&model);
    QSignalSpy pushFetches(&push, &BJobLogSource::fetchRequested);
    push.start(3, true);
    push.catalogReply(3, joblogReply({entry("log of job 3")}));
    push.jobMessage(4, "message of job 4");
    push.jobStatusEvent(4, "Success");
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(pushFetches.count(), 1);
}

void BJobLogModelTest::pushAppendsThenReloadsAtJobEnd()
{
    BJobLogModel model;
    BPushJobLogSource source(&model);
    QSignalSpy fetches(&source, &BJobLogSource::fetchRequested);
    QSignalSpy finished(&source, &BJobLogSource::finished);
    QSignalSpy resets(&model, &QAbstractItemModel::modelReset);

    // Initial catalog load
    QList<Entry> entries = startEntries();
    source.start(13, true);
    QCOMPARE(fetches.count(), 1);
    QCOMPARE(resets.count(), 1);
    source.catalogReply(13, joblogReply(entries));
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(finished.count(), 0);
    QVERIFY(source.isLive());

    // A message that the load already contained is not shown twice
    source.jobMessage(13, "Using Device \"FileStorage\" to write.");
    QCOMPARE(model.rowCount(), 3);

    // Pushed lines are appended, no further catalog load while the job runs
    source.jobMessage(13, "Wrote label to prelabeled Volume \"Incremental-0007\"");
    source.jobMessage(13, "Elapsed time=00:00:38, Transfer rate=24.16 M Bytes/second");
    source.jobStatusEvent(13, "Running");
    QCOMPARE(model.rowCount(), 5);
    QCOMPARE(model.pushRowCount(), 2);
    QCOMPARE(fetches.count(), 1);

    // Terminal status: one catalog load, which replaces the pushed rows
    source.jobStatusEvent(13, "Success");
    QCOMPARE(fetches.count(), 2);
    entries.append(entry("bareos-sd JobId 13: Wrote label to prelabeled Volume \"Incremental-0007\"\n"));
    entries.append(entry("bareos-sd JobId 13: Elapsed time=00:00:38, Transfer rate=24.16 M Bytes/second\n"));
    entries.append(entry(QString::fromLatin1(SummaryText)));
    source.catalogReply(13, joblogReply(entries));

    QCOMPARE(finished.count(), 1);
    QVERIFY(!source.isLive());
    QCOMPARE(model.pushRowCount(), 0);
    QCOMPARE(model.rowCount(), 5 + SummaryLines);
    QVERIFY(model.data(model.index(3)).toString().startsWith("[2026-11-17 21:00:03] bareos-sd JobId 13: Wrote label"));
    QCOMPARE(resets.count(), 1);   // only setJob() reset the model

    // A second terminal event does not fetch again
    source.jobStatusEvent(13, "Success");
    QCOMPARE(fetches.count(), 2);
}

QTEST_GUILESS_MAIN(BJobLogModelTest)
#include "bjoblogmodel_test.moc"
