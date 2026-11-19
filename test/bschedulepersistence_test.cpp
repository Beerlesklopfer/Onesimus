/**
 * @file bschedulepersistence_test.cpp
 * @brief Writing schedules: the resource text, the flow over the
 *        configuration methods, and the bytes of the requests
 *
 * - BScheduleResource: a change is written into the raw text of the
 *   resource; untouched lines stay byte for byte, CRLF stays CRLF, comments
 *   and quoting stay; what cannot be done safely is refused.
 * - BSchedulePersistence with a fake Director (bfakeconfigclient.h): every
 *   write is checked first (validate_only), shown and confirmed; every
 *   status of the Director is reported; a conflict is never overwritten; a
 *   referenced schedule is not deleted; a locked schedule is not written.
 * - With gRPC built in: the requests as the bytes of their proto messages,
 *   and every status of a ResourceResponse read back.
 *
 * Nothing is sent to a Director.
 */

#include <QtTest>

#include <QSignalSpy>

#include "bfakeconfigclient.h"
#include "bfakesaveui.h"
#include "schedules/bscheduledragplan.h"
#include "schedules/bschedulepersistence.h"
#include "schedules/model/bschedulegridmodel.h"
#include "schedules/model/bscheduleresource.h"

#ifdef HAVE_GRPC
#include "director/bconfigproto.h"
#endif

namespace {

// The schedule as it stands in its file on the Director: CRLF, comments,
// a quoted value, an old-style level, a comment behind a Run line
const QString kRaw = QStringLiteral(
    "Schedule {\r\n"
    "  Name = \"WeeklyCycle\"\r\n"
    "  Description = \"Full on the first Saturday\"   # why\r\n"
    "  Run = Full 1st sat at 21:00\r\n"
    "  # the rest of the month\r\n"
    "  Run = Differential 2nd-5th sat at 21:00   # keep this\r\n"
    "  Run = \"Incremental mon-fri at 21:00\"\r\n"
    "}\r\n");

// What "show schedules" reported: the first line respelled, as Bareos does
const QStringList kShown = {
    QStringLiteral("Level=Full 1st sat at 21:00"),
    QStringLiteral("Differential 2nd-5th sat at 21:00"),
    QStringLiteral("Incremental mon-fri at 21:00"),
};

BSchedule scheduleOf(const QString &name, const QStringList &runs)
{
    BSchedule schedule;
    schedule.name = name;
    for (const QString &run : runs)
        schedule.runs.append(BRunParser::parse(run));
    return schedule;
}

BSchedule loaded()
{
    return scheduleOf(QStringLiteral("WeeklyCycle"), kShown);
}

QStringList linesOf(const QString &text)
{
    QStringList lines;
    int start = 0;
    while (start < text.size()) {
        const int end = text.indexOf(QLatin1Char('\n'), start);
        if (end < 0) {
            lines.append(text.mid(start));
            break;
        }
        lines.append(text.mid(start, end - start + 1));
        start = end + 1;
    }
    return lines;
}

/// A fake Director, the dialogs and the flow
struct Flow
{
    BFakeConfigClient client;
    BFakeSaveUi ui{&client};
    BSchedulePersistence persistence{&client, &ui};
    QSignalSpy finished{&persistence, &BSchedulePersistence::finished};
    QSignalSpy reload{&persistence, &BSchedulePersistence::reloadRequested};

    Flow() { client.files.insert(QStringLiteral("WeeklyCycle"), kRaw); }

    bool wait()
    {
        if (finished.isEmpty() && !finished.wait(3000))
            return false;
        QCoreApplication::processEvents();
        return true;
    }
    BScheduleReport report() const { return finished.last().at(0).value<BScheduleReport>(); }

    /// Every request that changes the configuration came after a confirm
    /// and after the same request with validate_only
    bool writesWereConfirmedAndChecked() const
    {
        int confirmed = 0;
        for (const QString &entry : ui.log)
            confirmed += entry.startsWith(QStringLiteral("confirm@"));
        const QList<BConfig::Request> writes = client.writes();
        if (writes.size() > confirmed)
            return false;
        for (const BConfig::Request &write : writes) {
            bool checked = false;
            for (const BConfig::Request &request : client.requests) {
                if (&request == &write || (request.writes() && request.kind == write.kind))
                    break;
                checked = checked
                    || (request.validateOnly && request.kind == write.kind && request.name == write.name
                        && request.text == write.text && request.expectedSha256 == write.expectedSha256);
            }
            if (!checked)
                return false;
        }
        return true;
    }
};

/// The grid model of the loaded schedule, the third line moved to :30
QList<BScheduleResource::Line> movedThirdLine(BScheduleGridModel &model)
{
    model.setFirstDayOfWeek(Qt::Monday);
    model.setSchedule(loaded());
    const BGridChange change = model.moveRun(2, 30);
    if (!change.ok)
        return {};
    QList<BScheduleResource::Line> lines;
    if (!BSchedulePersistence::linesFromModel(model, &lines))
        return {};
    return lines;
}

} // namespace

class BSchedulePersistenceTest : public QObject
{
    Q_OBJECT

private slots:
    // --- The resource text ---
    void parsesTheRunLines();
    void rewriteChangesOnlyTheTouchedLine();
    void rewriteRemovesAndAdds();
    void crlfStaysCrlf();
    void lfStaysLf();
    void refusesWhatIsNotSafe();
    void fileMustSayWhatWasLoaded();
    void createText();

    // --- The flow ---
    void gridEditIsCheckedConfirmedAndWritten();
    void declinedReviewWritesNothing();
    void conflictOnCheckIsNotOverwritten();
    void conflictOnWriteIsNeverRetried();
    void conflictWithTheStateWhenEditingBegan();
    void invalidNamesLineAndColumn();
    void statusesAreReported_data();
    void statusesAreReported();
    void writtenWithFollowups();
    void fileThatDiffersIsNotSent();
    void lockedScheduleIsNotWritten();
    void dragWritesOnlyTheDraggedLine();
    void deleteRefusedWhileReferenced();
    void deleteAfterConfirm();
    void createIsCheckedFirst();
    void nothingWithoutAClient();

#ifdef HAVE_GRPC
    // --- On the wire ---
    void updateRequestBytes();
    void everyRequestKindRoundTrips();
    void everyStatusReadBack_data();
    void everyStatusReadBack();
#endif
};

// ============================================================================
// The resource text
// ============================================================================

void BSchedulePersistenceTest::parsesTheRunLines()
{
    const BScheduleResource::Parsed parsed = BScheduleResource::parse(kRaw);
    QVERIFY2(parsed.ok, qPrintable(parsed.error));
    QCOMPARE(parsed.name, QStringLiteral("WeeklyCycle"));
    QCOMPARE(parsed.lineEnd, QStringLiteral("\r\n"));
    QCOMPARE(parsed.runs.size(), 3);
    QCOMPARE(parsed.runs[0].value, QStringLiteral("Full 1st sat at 21:00"));
    QCOMPARE(parsed.runs[1].value, QStringLiteral("Differential 2nd-5th sat at 21:00"));
    QCOMPARE(parsed.runs[2].value, QStringLiteral("Incremental mon-fri at 21:00"));
    QVERIFY(parsed.runs[2].quoted);
    QCOMPARE(parsed.closingLine, 7);
    // Lines and their ends together are the text
    QCOMPARE(parsed.lines.join(QString()), kRaw);
}

void BSchedulePersistenceTest::rewriteChangesOnlyTheTouchedLine()
{
    QList<BScheduleResource::Line> lines = {
        {0, true, QString()},
        {1, false, QStringLiteral("Level=Differential 2nd-5th sat at 22:00")},
        {2, true, QString()},
    };
    QString out;
    QString error;
    QVERIFY2(BScheduleResource::rewrite(kRaw, lines, &out, &error), qPrintable(error));
    const QStringList before = linesOf(kRaw);
    const QStringList after = linesOf(out);
    QCOMPARE(after.size(), before.size());
    for (int i = 0; i < before.size(); ++i) {
        if (i == 5) {
            // Indentation, "Run =", the comment and the line end stay
            QCOMPARE(after[i], QStringLiteral("  Run = Level=Differential 2nd-5th sat at 22:00   # keep this\r\n"));
        } else {
            QCOMPARE(after[i], before[i]);   // byte for byte
        }
    }

    // A quoted value stays quoted
    lines = {{0, true, QString()}, {1, true, QString()},
             {2, false, QStringLiteral("Level=Incremental mon-fri at 21:30")}};
    QVERIFY(BScheduleResource::rewrite(kRaw, lines, &out, &error));
    QCOMPARE(linesOf(out)[6], QStringLiteral("  Run = \"Level=Incremental mon-fri at 21:30\"\r\n"));

    // Nothing changed: the same text
    lines = {{0, true, QString()}, {1, true, QString()}, {2, true, QString()}};
    QVERIFY(BScheduleResource::rewrite(kRaw, lines, &out, &error));
    QCOMPARE(out, kRaw);
}

void BSchedulePersistenceTest::rewriteRemovesAndAdds()
{
    // The second line goes, a new one follows the third, one leads
    const QList<BScheduleResource::Line> lines = {
        {-1, false, QStringLiteral("Level=Full on 1 at 03:00")},
        {0, true, QString()},
        {2, true, QString()},
        {-1, false, QStringLiteral("Level=Full sun at 05:00")},
    };
    QString out;
    QString error;
    QVERIFY2(BScheduleResource::rewrite(kRaw, lines, &out, &error), qPrintable(error));
    QCOMPARE(out, QStringLiteral(
        "Schedule {\r\n"
        "  Name = \"WeeklyCycle\"\r\n"
        "  Description = \"Full on the first Saturday\"   # why\r\n"
        "  Run = Level=Full on 1 at 03:00\r\n"
        "  Run = Full 1st sat at 21:00\r\n"
        "  # the rest of the month\r\n"
        "  Run = \"Incremental mon-fri at 21:00\"\r\n"
        "  Run = \"Level=Full sun at 05:00\"\r\n"
        "}\r\n"));

    // Every Run line removed, one new: where the first one was
    QVERIFY(BScheduleResource::rewrite(kRaw, {{-1, false, QStringLiteral("Level=Full sat at 01:00")}}, &out, &error));
    QStringList values;
    QVERIFY(BScheduleResource::runValues(out, &values));
    QCOMPARE(values, QStringList({QStringLiteral("Level=Full sat at 01:00")}));
    QVERIFY(out.contains(QStringLiteral("  # the rest of the month\r\n")));

    // A resource without any Run line gets them before the closing brace
    const QString empty = QStringLiteral("Schedule {\n  Name = Empty\n}\n");
    QVERIFY(BScheduleResource::rewrite(empty, {{-1, false, QStringLiteral("Level=Full sun at 05:00")}}, &out, &error));
    QCOMPARE(out, QStringLiteral("Schedule {\n  Name = Empty\n  Run = Level=Full sun at 05:00\n}\n"));
}

void BSchedulePersistenceTest::crlfStaysCrlf()
{
    QString out;
    QVERIFY(BScheduleResource::rewrite(kRaw, {{0, false, QStringLiteral("Level=Full 1st sat at 20:00")},
                                              {1, true, QString()}, {2, true, QString()},
                                              {-1, false, QStringLiteral("Level=Full sun at 05:00")}},
                                       &out));
    // Every line end is CRLF; none is a bare LF
    QCOMPARE(out.count(QStringLiteral("\r\n")), out.count(QLatin1Char('\n')));
    QCOMPARE(out.count(QStringLiteral("\r\n")), kRaw.count(QStringLiteral("\r\n")) + 1);
}

void BSchedulePersistenceTest::lfStaysLf()
{
    QString raw = kRaw;
    raw.replace(QStringLiteral("\r\n"), QStringLiteral("\n"));
    QString out;
    QVERIFY(BScheduleResource::rewrite(raw, {{0, true, QString()}, {1, true, QString()},
                                             {2, false, QStringLiteral("Level=Incremental mon-fri at 23:00")},
                                             {-1, false, QStringLiteral("Level=Full sun at 05:00")}},
                                       &out));
    QVERIFY(!out.contains(QLatin1Char('\r')));
}

void BSchedulePersistenceTest::refusesWhatIsNotSafe()
{
    const QList<BScheduleResource::Line> change = {{0, false, QStringLiteral("Level=Full sun at 05:00")}};
    QString out = QStringLiteral("untouched");
    QString error;
    const QStringList unsafe = {
        // Another directive on the line of a Run directive
        QStringLiteral("Schedule {\n  Name = A; Run = Full sun at 01:00\n}\n"),
        QStringLiteral("Schedule {\n  Run = Full sun at 01:00; Enabled = no\n}\n"),
        // Directives on the line of the brace
        QStringLiteral("Schedule { Name = A\n  Run = Full sun at 01:00\n}\n"),
        QStringLiteral("Schedule {\n  Run = Full sun at 01:00 }\n"),
        // Not a schedule, not one resource, not closed
        QStringLiteral("Job {\n  Name = A\n}\n"),
        QStringLiteral("Schedule {\n  Run = Full sun at 01:00\n}\nSchedule {\n}\n"),
        QStringLiteral("Schedule {\n  Run = Full sun at 01:00\n"),
        // A quoted value that does not end, or with something behind it
        QStringLiteral("Schedule {\n  Run = \"Full sun at 01:00\n}\n"),
        QStringLiteral("Schedule {\n  Run = \"Full sun at 01:00\" x\n}\n"),
    };
    for (const QString &text : unsafe) {
        QVERIFY2(!BScheduleResource::rewrite(text, change, &out, &error), qPrintable(text));
        QVERIFY(!error.isEmpty());
        QCOMPARE(out, QStringLiteral("untouched"));
    }
    // Order changed, an origin twice, a line break in a value
    QVERIFY(!BScheduleResource::rewrite(kRaw, {{1, true, QString()}, {0, true, QString()}}, &out, &error));
    QVERIFY(!BScheduleResource::rewrite(kRaw, {{0, true, QString()}, {0, true, QString()}}, &out, &error));
    QVERIFY(!BScheduleResource::rewrite(kRaw, {{0, false, QStringLiteral("Full\nsun at 01:00")}}, &out, &error));
    QVERIFY(!BScheduleResource::rewrite(kRaw, {{7, true, QString()}}, &out, &error));
    QCOMPARE(out, QStringLiteral("untouched"));
    // A "#" inside quotes is not a comment
    const BScheduleResource::Parsed quoted =
        BScheduleResource::parse(QStringLiteral("Schedule {\n  Description = \"a # b\"\n  Run = Full sun at 01:00 # c\n}\n"));
    QVERIFY2(quoted.ok, qPrintable(quoted.error));
    QCOMPARE(quoted.runs.first().value, QStringLiteral("Full sun at 01:00"));
}

void BSchedulePersistenceTest::fileMustSayWhatWasLoaded()
{
    const BScheduleResource::Parsed parsed = BScheduleResource::parse(kRaw);
    QString error;
    // Respelled by the Director, same meaning: it matches
    QVERIFY2(BScheduleResource::matchesLoaded(parsed, loaded().runs, &error), qPrintable(error));
    // Another time
    BSchedule other = scheduleOf(QStringLiteral("WeeklyCycle"),
                                 {kShown[0], QStringLiteral("Differential 2nd-5th sat at 22:00"), kShown[2]});
    QVERIFY(!BScheduleResource::matchesLoaded(parsed, other.runs, &error));
    QVERIFY(error.contains(QStringLiteral("Run line 2")));
    // Another number of lines
    other.runs.removeLast();
    QVERIFY(!BScheduleResource::matchesLoaded(parsed, other.runs, &error));
}

void BSchedulePersistenceTest::createText()
{
    const QString text = BScheduleResource::createText(QStringLiteral("Nightly"), QStringLiteral("a \"b\""),
                                                       {QStringLiteral("Level=Full sun at 02:00")});
    QCOMPARE(text, QStringLiteral("Schedule {\n  Name = \"Nightly\"\n  Description = \"a \\\"b\\\"\"\n"
                                  "  Run = Level=Full sun at 02:00\n}\n"));
    QStringList values;
    QVERIFY(BScheduleResource::runValues(text, &values));
    QCOMPARE(values, QStringList({QStringLiteral("Level=Full sun at 02:00")}));
}

// ============================================================================
// The flow
// ============================================================================

void BSchedulePersistenceTest::gridEditIsCheckedConfirmedAndWritten()
{
    Flow f;
    BScheduleGridModel model;
    const QList<BScheduleResource::Line> lines = movedThirdLine(model);
    QCOMPARE(lines.size(), 3);
    QVERIFY(lines[0].keep && lines[1].keep && !lines[2].keep);

    QVERIFY(f.persistence.saveRuns(model.loadedSchedule(), lines, {QStringLiteral("BackupMail")},
                                   QStringLiteral("grid")));
    QVERIFY(f.persistence.isBusy());
    QVERIFY(f.wait());
    QCOMPARE(f.report().kind, BScheduleReport::Written);
    QVERIFY(f.report().reloaded);

    // Read, check, write: in this order, with the SHA-256 of the read text
    QCOMPARE(f.client.requests.size(), 3);
    const BConfig::Request &read = f.client.requests[0];
    const BConfig::Request &check = f.client.requests[1];
    const BConfig::Request &write = f.client.requests[2];
    QCOMPARE(read.kind, BConfig::Request::Read);
    QCOMPARE(read.name, QStringLiteral("WeeklyCycle"));
    QCOMPARE(check.kind, BConfig::Request::Update);
    QVERIFY(check.validateOnly);
    QCOMPARE(write.kind, BConfig::Request::Update);
    QVERIFY(!write.validateOnly);
    QCOMPARE(check.expectedSha256, BConfig::sha256Hex(kRaw));
    QCOMPARE(write.expectedSha256, check.expectedSha256);
    QCOMPARE(write.text, check.text);
    QCOMPARE(write.name, QStringLiteral("WeeklyCycle"));     // the same schedule, never a new one
    QVERIFY(f.writesWereConfirmedAndChecked());
    QCOMPARE(f.ui.log.first(), QStringLiteral("confirm@2 writes=0"));

    // The text: every line as in the file but the moved one; CRLF kept
    const QStringList before = linesOf(kRaw);
    const QStringList after = linesOf(write.text);
    QCOMPARE(after.size(), before.size());
    for (int i = 0; i < before.size(); ++i) {
        if (i == 6)
            QCOMPARE(after[i], QStringLiteral("  Run = \"Level=Incremental mon-fri at 21:30\"\r\n"));
        else
            QCOMPARE(after[i], before[i]);
    }
    // The respelled first line stays as the file has it, not as shown
    QVERIFY(write.text.contains(QStringLiteral("  Run = Full 1st sat at 21:00\r\n")));

    // The review showed both versions and the jobs
    QCOMPARE(f.ui.reviews.size(), 1);
    const BScheduleReview &review = f.ui.reviews.first();
    QCOMPARE(review.operation, BScheduleReview::Update);
    QCOMPARE(review.jobs, QStringList({QStringLiteral("BackupMail")}));
    QCOMPARE(review.oldRows.size(), 3);
    QCOMPARE(review.oldRows[2].state, BScheduleReview::Row::Changed);
    QCOMPARE(review.oldRows[0].state, BScheduleReview::Row::Untouched);
    QCOMPARE(review.newRows[2].text, QStringLiteral("Level=Incremental mon-fri at 21:30"));
    QCOMPARE(review.newRows[2].state, BScheduleReview::Row::Changed);
    QCOMPARE(review.oldText, kRaw);
    QCOMPARE(review.newText, write.text);

    // Written: read again; the Director has the new text
    QCOMPARE(f.reload.size(), 1);
    QCOMPARE(f.client.files.value(QStringLiteral("WeeklyCycle")), write.text);
    QVERIFY(!f.persistence.isBusy());
}

void BSchedulePersistenceTest::declinedReviewWritesNothing()
{
    Flow f;
    f.ui.confirmAnswer = false;
    BScheduleGridModel model;
    QVERIFY(f.persistence.saveRuns(model.loadedSchedule(), movedThirdLine(model), {}));
    QVERIFY(f.wait());
    QCOMPARE(f.report().kind, BScheduleReport::Cancelled);
    QCOMPARE(f.ui.reviews.size(), 1);
    QVERIFY(f.client.writes().isEmpty());
    QCOMPARE(f.client.files.value(QStringLiteral("WeeklyCycle")), kRaw);
    QVERIFY(f.ui.reports.isEmpty());   // no dialog for "you said no"
    QCOMPARE(f.reload.size(), 0);
}

void BSchedulePersistenceTest::conflictOnCheckIsNotOverwritten()
{
    Flow f;
    f.ui.reportAnswer = BScheduleSaveUi::Reload;
    // Someone changes the file between the read and the check
    const QString theirs = QString(kRaw).replace(QStringLiteral("# why"), QStringLiteral("# changed"));
    f.client.intercept = [&](const BConfig::Request &request) -> std::optional<BConfig::Reply> {
        if (request.kind == BConfig::Request::Update)
            f.client.files.insert(QStringLiteral("WeeklyCycle"), theirs);
        return std::nullopt;
    };
    BScheduleGridModel model;
    QVERIFY(f.persistence.saveRuns(model.loadedSchedule(), movedThirdLine(model), {}));
    QVERIFY(f.wait());
    f.client.intercept = nullptr;

    const BScheduleReport report = f.report();
    QCOMPARE(report.kind, BScheduleReport::Conflict);
    QVERIFY(report.validatedOnly);
    QCOMPARE(report.currentText, theirs);   // both texts are there to compare
    QVERIFY(report.ourText.contains(QStringLiteral("21:30")));
    QVERIFY(f.ui.reviews.isEmpty());        // not even asked
    QVERIFY(f.client.writes().isEmpty());
    QCOMPARE(f.client.files.value(QStringLiteral("WeeklyCycle")), theirs);
    // "Reload" was chosen
    QCOMPARE(f.reload.size(), 1);
    QCOMPARE(f.reload.first().at(0).toString(), QStringLiteral("WeeklyCycle"));
}

void BSchedulePersistenceTest::conflictOnWriteIsNeverRetried()
{
    Flow f;
    f.client.intercept = [](const BConfig::Request &request) -> std::optional<BConfig::Reply> {
        if (request.kind == BConfig::Request::Update && !request.validateOnly) {
            return BFakeConfigClient::withText(BConfig::Status::Conflict, request.name,
                                               QStringLiteral("Schedule {\n  Name = WeeklyCycle\n}\n"));
        }
        return std::nullopt;
    };
    BScheduleGridModel model;
    QVERIFY(f.persistence.saveRuns(model.loadedSchedule(), movedThirdLine(model), {}));
    QVERIFY(f.wait());
    QTest::qWait(50);
    QCOMPARE(f.report().kind, BScheduleReport::Conflict);
    QVERIFY(!f.report().validatedOnly);
    // Read, check, write: no second write with the new SHA-256
    QCOMPARE(f.client.requests.size(), 3);
    QCOMPARE(f.client.writes().size(), 1);
    QCOMPARE(f.reload.size(), 0);   // "Close" was the answer
}

void BSchedulePersistenceTest::conflictWithTheStateWhenEditingBegan()
{
    Flow f;
    // Editing begins: the text and its SHA-256 are read
    f.persistence.prepare(QStringLiteral("WeeklyCycle"));
    QTRY_COMPARE(f.persistence.preparedText(QStringLiteral("WeeklyCycle")), kRaw);
    // The file changes on the Director (a comment: the Run lines still match)
    f.client.files.insert(QStringLiteral("WeeklyCycle"),
                          QString(kRaw).replace(QStringLiteral("# keep this"), QStringLiteral("# edited")));
    f.client.requests.clear();

    BScheduleGridModel model;
    QVERIFY(f.persistence.saveRuns(model.loadedSchedule(), movedThirdLine(model), {}));
    QVERIFY(f.wait());
    // No second read: the check carries the SHA-256 of when editing began
    QCOMPARE(f.client.requests.first().kind, BConfig::Request::Update);
    QCOMPARE(f.client.requests.first().expectedSha256, BConfig::sha256Hex(kRaw));
    QCOMPARE(f.report().kind, BScheduleReport::Conflict);
    QVERIFY(f.client.writes().isEmpty());
}

void BSchedulePersistenceTest::invalidNamesLineAndColumn()
{
    Flow f;
    f.client.intercept = [](const BConfig::Request &request) -> std::optional<BConfig::Reply> {
        if (request.kind != BConfig::Request::Update)
            return std::nullopt;
        BConfig::Reply reply = BFakeConfigClient::withText(BConfig::Status::Invalid, request.name, QString());
        reply.message = QStringLiteral("configuration is invalid");
        reply.problem = BConfig::Problem{QStringLiteral("/etc/bareos/bareos-dir.d/schedule/WeeklyCycle.conf"), 7, 11,
                                         QStringLiteral("unknown keyword \"Nonsense\"")};
        reply.validatedOnly = true;
        return reply;
    };
    BScheduleGridModel model;
    QVERIFY(f.persistence.saveRuns(model.loadedSchedule(), movedThirdLine(model), {}));
    QVERIFY(f.wait());
    const BScheduleReport report = f.report();
    QCOMPARE(report.kind, BScheduleReport::Invalid);
    QVERIFY(report.problem.has_value());
    QCOMPARE(report.problem->line, 7);
    QCOMPARE(report.problem->column, 11);
    QCOMPARE(report.directorMessage, QStringLiteral("configuration is invalid"));
    QVERIFY(f.client.writes().isEmpty());
    QVERIFY(f.ui.reviews.isEmpty());
}

void BSchedulePersistenceTest::statusesAreReported_data()
{
    QTest::addColumn<int>("status");
    QTest::addColumn<bool>("onWrite");
    QTest::addColumn<int>("kind");
    QTest::addColumn<QString>("words");

    QTest::newRow("denied") << int(BConfig::Status::Denied) << false << int(BScheduleReport::Denied)
                            << QStringLiteral("Schedule ACL");
    QTest::newRow("not found") << int(BConfig::Status::NotFound) << false << int(BScheduleReport::NotFound)
                               << QStringLiteral("no schedule of this name");
    QTest::newRow("reload failed") << int(BConfig::Status::ReloadFailed) << true
                                   << int(BScheduleReport::ReloadFailed) << QStringLiteral("put the old file back");
    QTest::newRow("permission denied") << int(BConfig::Status::RpcError) << false << int(BScheduleReport::Failed)
                                       << QStringLiteral("command ACL \"configure\"");
    QTest::newRow("not available") << int(BConfig::Status::NotAvailable) << false
                                   << int(BScheduleReport::NotAvailable) << QStringLiteral("gRPC");
    QTest::newRow("no status") << int(BConfig::Status::Unspecified) << true << int(BScheduleReport::Failed)
                               << QStringLiteral("no status");
}

void BSchedulePersistenceTest::statusesAreReported()
{
    QFETCH(int, status);
    QFETCH(bool, onWrite);
    QFETCH(int, kind);
    QFETCH(QString, words);

    Flow f;
    f.client.intercept = [&](const BConfig::Request &request) -> std::optional<BConfig::Reply> {
        if (request.kind != BConfig::Request::Update || request.validateOnly == onWrite)
            return std::nullopt;
        BConfig::Reply reply = BFakeConfigClient::withText(BConfig::Status(status), request.name, QString());
        if (status == int(BConfig::Status::RpcError)) {
            reply.rpcCode = 7;
            reply.rpcMessage = QStringLiteral("PERMISSION_DENIED: command \"configure\" not allowed");
        }
        if (status == int(BConfig::Status::NotAvailable))
            reply.rpcMessage = QStringLiteral("Changing the configuration needs the gRPC interface of the Onesimus Director");
        return reply;
    };
    BScheduleGridModel model;
    QVERIFY(f.persistence.saveRuns(model.loadedSchedule(), movedThirdLine(model), {}));
    QVERIFY(f.wait());
    const BScheduleReport report = f.report();
    QCOMPARE(int(report.kind), kind);
    QVERIFY2(report.text.contains(words), qPrintable(report.text));
    QCOMPARE(f.ui.reports.size(), 1);   // the user is told
    QCOMPARE(f.reload.size(), 0);
    QVERIFY(f.writesWereConfirmedAndChecked());
    // Only where the write itself was answered has anything been sent to write
    QCOMPARE(f.client.writes().size(), onWrite ? 1 : 0);
}

void BSchedulePersistenceTest::writtenWithFollowups()
{
    Flow f;
    f.client.followups = {QStringLiteral("update volume pool=Full allfrompool")};
    BScheduleGridModel model;
    QVERIFY(f.persistence.saveRuns(model.loadedSchedule(), movedThirdLine(model), {}));
    QVERIFY(f.wait());
    QCOMPARE(f.report().kind, BScheduleReport::Written);
    QCOMPARE(f.report().followups, f.client.followups);
    QVERIFY(f.report().text.contains(QStringLiteral("follow-up")));
    QCOMPARE(f.ui.reports.size(), 1);
}

void BSchedulePersistenceTest::fileThatDiffersIsNotSent()
{
    Flow f;
    f.ui.reportAnswer = BScheduleSaveUi::Reload;
    f.client.files.insert(QStringLiteral("WeeklyCycle"),
                          QString(kRaw).replace(QStringLiteral("2nd-5th sat at 21:00"),
                                                QStringLiteral("2nd-5th sat at 23:00")));
    BScheduleGridModel model;
    QVERIFY(f.persistence.saveRuns(model.loadedSchedule(), movedThirdLine(model), {}));
    QVERIFY(f.wait());
    QCOMPARE(f.report().kind, BScheduleReport::FileDiffers);
    QVERIFY(f.report().text.contains(QStringLiteral("Run line 2")));
    QVERIFY(f.report().currentText.contains(QStringLiteral("23:00")));
    // Only the read
    QCOMPARE(f.client.requests.size(), 1);
    QCOMPARE(f.reload.size(), 1);
}

void BSchedulePersistenceTest::lockedScheduleIsNotWritten()
{
    Flow f;
    const BSchedule locked = scheduleOf(QStringLiteral("WeeklyCycle"),
                                        {kShown[0], QStringLiteral("Level=Differential every other sat at 21:00"),
                                         kShown[2]});
    QVERIFY(!locked.runs[1].understood());
    QVERIFY(!f.persistence.saveRuns(locked, BSchedulePersistence::linesFromDrag(locked, 0,
                                                                                QStringLiteral("Level=Full 1st sat at 20:00")),
                                    {}));
    QCOMPARE(f.report().kind, BScheduleReport::Locked);
    QVERIFY(f.client.requests.isEmpty());
    // The grid model hands out nothing for it either
    BScheduleGridModel model;
    model.setSchedule(locked);
    QList<BScheduleResource::Line> lines;
    QString error;
    QVERIFY(!BSchedulePersistence::linesFromModel(model, &lines, &error));
    QVERIFY(!error.isEmpty());
}

void BSchedulePersistenceTest::dragWritesOnlyTheDraggedLine()
{
    Flow f;
    const BSchedule schedule = loaded();
    BScheduleEntry entry;
    entry.scheduleName = schedule.name;
    entry.jobName = QStringLiteral("BackupMail");
    entry.source = schedule.runs[2];
    entry.runIndex = 2;
    entry.hour = 21;
    entry.minute = 0;
    QStringList originals;
    for (const BRunLine &run : schedule.runs)
        originals.append(run.original);
    const BScheduleDragPlan::Plan plan = BScheduleDragPlan::plan(originals, entry, 22, 15);
    QVERIFY(!plan.refused());
    QCOMPARE(plan.draggedLine, 2);

    QVERIFY(f.persistence.saveRuns(schedule, BSchedulePersistence::linesFromDrag(schedule, 2,
                                                                                 plan.generatedRuns[2]),
                                   {QStringLiteral("BackupMail")}, QStringLiteral("drag")));
    QVERIFY(f.wait());
    QCOMPARE(f.report().kind, BScheduleReport::Written);
    const QList<BConfig::Request> writes = f.client.writes();
    QCOMPARE(writes.size(), 1);
    QCOMPARE(writes[0].kind, BConfig::Request::Update);   // an update of this schedule
    QCOMPARE(writes[0].name, QStringLiteral("WeeklyCycle"));
    QStringList values;
    QVERIFY(BScheduleResource::runValues(writes[0].text, &values));
    QCOMPARE(values, QStringList({QStringLiteral("Full 1st sat at 21:00"),
                                  QStringLiteral("Differential 2nd-5th sat at 21:00"),
                                  QStringLiteral("Level=Incremental mon-fri at 22:15")}));
    QCOMPARE(f.ui.reviews.first().note, QStringLiteral("drag"));
}

void BSchedulePersistenceTest::deleteRefusedWhileReferenced()
{
    Flow f;
    f.client.refs.insert(QStringLiteral("WeeklyCycle"),
                         {BConfig::ResourceRef{BConfig::ResourceType::Job, QStringLiteral("forgejo")},
                          BConfig::ResourceRef{BConfig::ResourceType::Job, QStringLiteral("BackupMail")}});
    QVERIFY(f.persistence.deleteSchedule(QStringLiteral("WeeklyCycle"), {QStringLiteral("BackupMail")}));
    QVERIFY(f.wait());
    const BScheduleReport report = f.report();
    QCOMPARE(report.kind, BScheduleReport::Referenced);
    QCOMPARE(report.referencedBy, QStringList({QStringLiteral("Job forgejo"), QStringLiteral("Job BackupMail")}));
    QVERIFY(report.validatedOnly);
    // Read and the check; nothing deleted, not even asked
    QCOMPARE(f.client.requests.size(), 2);
    QCOMPARE(f.client.requests[1].kind, BConfig::Request::Delete);
    QVERIFY(f.client.requests[1].validateOnly);
    QCOMPARE(f.client.requests[1].expectedSha256, BConfig::sha256Hex(kRaw));
    QVERIFY(f.client.writes().isEmpty());
    QVERIFY(f.ui.reviews.isEmpty());
    QVERIFY(f.client.files.contains(QStringLiteral("WeeklyCycle")));
}

void BSchedulePersistenceTest::deleteAfterConfirm()
{
    Flow f;
    QVERIFY(f.persistence.deleteSchedule(QStringLiteral("WeeklyCycle"), {}));
    QVERIFY(f.wait());
    QCOMPARE(f.report().kind, BScheduleReport::Written);
    QCOMPARE(f.report().operation, BScheduleReview::Delete);
    QCOMPARE(f.ui.reviews.size(), 1);
    QCOMPARE(f.ui.reviews.first().operation, BScheduleReview::Delete);
    QCOMPARE(f.ui.reviews.first().oldRows.size(), 3);
    QVERIFY(f.writesWereConfirmedAndChecked());
    QCOMPARE(f.client.writes().size(), 1);
    QCOMPARE(f.client.writes().first().expectedSha256, BConfig::sha256Hex(kRaw));
    QVERIFY(!f.client.files.contains(QStringLiteral("WeeklyCycle")));
}

void BSchedulePersistenceTest::createIsCheckedFirst()
{
    Flow f;
    const QString text = BScheduleResource::createText(QStringLiteral("Nightly"), QString(),
                                                       {QStringLiteral("Level=Full sun at 02:00")});
    QVERIFY(f.persistence.createSchedule(QStringLiteral("Nightly"), text));
    QVERIFY(f.wait());
    QCOMPARE(f.report().kind, BScheduleReport::Written);
    QCOMPARE(f.client.requests.size(), 2);
    QVERIFY(f.client.requests[0].validateOnly);
    QCOMPARE(f.client.requests[1].text, text);
    QVERIFY(f.writesWereConfirmedAndChecked());
    QCOMPARE(f.client.files.value(QStringLiteral("Nightly")), text);

    // The name taken: EXISTS, from the check, nothing written
    Flow g;
    const QString again = BScheduleResource::createText(QStringLiteral("WeeklyCycle"), QString(),
                                                        {QStringLiteral("Level=Full sun at 02:00")});
    QVERIFY(g.persistence.createSchedule(QStringLiteral("WeeklyCycle"), again));
    QVERIFY(g.wait());
    QCOMPARE(g.report().kind, BScheduleReport::Exists);
    QVERIFY(g.client.writes().isEmpty());
    QCOMPARE(g.client.files.value(QStringLiteral("WeeklyCycle")), kRaw);
}

void BSchedulePersistenceTest::nothingWithoutAClient()
{
    BSchedulePersistence persistence(nullptr, nullptr);
    QSignalSpy finished(&persistence, &BSchedulePersistence::finished);
    BScheduleGridModel model;
    model.setSchedule(loaded());
    QVERIFY(!persistence.saveRuns(loaded(), BSchedulePersistence::linesFromDrag(loaded(), 0, kShown[0]), {}));
    QCOMPARE(finished.size(), 1);
    QCOMPARE(finished.first().at(0).value<BScheduleReport>().kind, BScheduleReport::NotAvailable);
    QVERIFY(!persistence.deleteSchedule(QStringLiteral("WeeklyCycle"), {}));
    QVERIFY(!persistence.createSchedule(QStringLiteral("X"), QStringLiteral("Schedule {\n}\n")));
}

// ============================================================================
// On the wire
// ============================================================================

#ifdef HAVE_GRPC

void BSchedulePersistenceTest::updateRequestBytes()
{
    // From the grid edit to the bytes of UpdateResourceRequest
    Flow f;
    BScheduleGridModel model;
    QVERIFY(f.persistence.saveRuns(model.loadedSchedule(), movedThirdLine(model), {}));
    QVERIFY(f.wait());
    const BConfig::Request write = f.client.writes().value(0);
    QCOMPARE(write.kind, BConfig::Request::Update);

    const QByteArray bytes = BConfigProto::serializeRequest(write);
    QVERIFY(!bytes.isEmpty());
    // The text is in the message as its exact UTF-8 bytes, CRLF and all
    const QByteArray text = write.text.toUtf8();
    QVERIFY(bytes.contains(text));
    QVERIFY(text.contains("\r\n  Run = Full 1st sat at 21:00\r\n"));
    QVERIFY(bytes.contains(write.expectedSha256.toLatin1()));

    BConfig::Request back;
    QVERIFY(BConfigProto::parseRequest(BConfig::Request::Update, bytes, &back));
    QCOMPARE(back.name, QStringLiteral("WeeklyCycle"));
    QCOMPARE(back.type, BConfig::ResourceType::Schedule);
    QCOMPARE(back.expectedSha256, BConfig::sha256Hex(kRaw));
    QCOMPARE(back.text.toUtf8(), text);
    QVERIFY(!back.validateOnly);

    // The check before it differs only in validate_only
    const BConfig::Request check = f.client.requests[1];
    BConfig::Request checkBack;
    QVERIFY(BConfigProto::parseRequest(BConfig::Request::Update, BConfigProto::serializeRequest(check), &checkBack));
    QVERIFY(checkBack.validateOnly);
    QCOMPARE(checkBack.text, back.text);
}

void BSchedulePersistenceTest::everyRequestKindRoundTrips()
{
    const QString text = QStringLiteral("Schedule {\r\n  Name = \"Wöchentlich\"\r\n  Run = Full sun at 02:00\r\n}\r\n");
    const QList<BConfig::Request> requests = {
        BConfig::Request::read(BConfig::ResourceType::Schedule, QStringLiteral("Wöchentlich")),
        BConfig::Request::create(BConfig::ResourceType::Schedule, text, true),
        BConfig::Request::update(BConfig::ResourceType::JobDefs, QStringLiteral("DefaultJob"), QStringLiteral("ab12"), text, false),
        BConfig::Request::remove(BConfig::ResourceType::Job, QStringLiteral("forgejo"), QStringLiteral("cd34"), true),
    };
    for (const BConfig::Request &request : requests) {
        BConfig::Request back;
        QVERIFY(BConfigProto::parseRequest(request.kind, BConfigProto::serializeRequest(request), &back));
        QCOMPARE(back.kind, request.kind);
        QCOMPARE(back.type, request.type);
        QCOMPARE(back.name, request.name);
        QCOMPARE(back.text.toUtf8(), request.text.toUtf8());
        QCOMPARE(back.expectedSha256, request.expectedSha256);
        QCOMPARE(back.validateOnly, request.validateOnly);
    }
    BConfig::Request list;
    list.kind = BConfig::Request::List;
    list.type = BConfig::ResourceType::Schedule;
    BConfig::Request back;
    QVERIFY(BConfigProto::parseRequest(BConfig::Request::List, BConfigProto::serializeRequest(list), &back));
    QCOMPARE(back.type, BConfig::ResourceType::Schedule);
}

void BSchedulePersistenceTest::everyStatusReadBack_data()
{
    QTest::addColumn<int>("status");
    for (int status = int(BConfig::Status::Ok); status <= int(BConfig::Status::ReloadFailed); ++status)
        QTest::newRow(qPrintable(BConfig::statusName(BConfig::Status(status)))) << status;
}

void BSchedulePersistenceTest::everyStatusReadBack()
{
    QFETCH(int, status);
    BConfig::Reply reply = BFakeConfigClient::withText(BConfig::Status(status), QStringLiteral("WeeklyCycle"), kRaw);
    reply.message = QStringLiteral("a message");
    reply.sharedFile = true;
    reply.referencedBy = {BConfig::ResourceRef{BConfig::ResourceType::Job, QStringLiteral("forgejo")}};
    reply.problem = BConfig::Problem{QStringLiteral("f.conf"), 3, 11, QStringLiteral("bad")};
    reply.reloaded = true;
    reply.validatedOnly = true;
    reply.oldSha256 = QStringLiteral("00");
    reply.followups = {QStringLiteral("update volume pool=Full allfrompool")};

    BConfig::Reply back;
    QVERIFY(BConfigProto::parseResourceResponse(BConfigProto::serializeResourceResponse(reply), &back));
    QCOMPARE(int(back.status), status);
    QCOMPARE(back.message, reply.message);
    QCOMPARE(back.resource.name, QStringLiteral("WeeklyCycle"));
    QCOMPARE(back.resource.type, BConfig::ResourceType::Schedule);
    QCOMPARE(back.text, kRaw);
    QCOMPARE(back.sha256, BConfig::sha256Hex(kRaw));
    QVERIFY(back.sharedFile);
    QCOMPARE(back.referencedBy.size(), 1);
    QCOMPARE(back.referencedBy.first().type, BConfig::ResourceType::Job);
    QVERIFY(back.problem.has_value());
    QCOMPARE(back.problem->line, 3);
    QCOMPARE(back.problem->column, 11);
    QVERIFY(back.reloaded);
    QVERIFY(back.validatedOnly);
    QCOMPARE(back.oldSha256, QStringLiteral("00"));
    QCOMPARE(back.followups, reply.followups);
}

#endif // HAVE_GRPC

QTEST_GUILESS_MAIN(BSchedulePersistenceTest)
#include "bschedulepersistence_test.moc"
