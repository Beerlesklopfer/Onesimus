// Tests for "purge volume all" of the Onesimus Director: the command line,
// the reply parser and the panel of the cleanup dialog. No Director is
// involved; the panel's sender and confirmation are test functions.

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QtTest>

#include "bvolumepurge.h"
#include "bvolumepurgepanel.h"

using namespace BVolumePurge;

namespace {

const char *DryRunReply =
    "{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{\"purge\":{\"mode\":\"dryrun\",\"pool\":null,"
    "\"allpools\":true,\"volstatus\":[\"Full\",\"Used\",\"Error\"],\"volumes\":["
    "{\"volume\":\"Full-0001\",\"pool\":\"Full\",\"volstatus\":\"Full\",\"jobs\":3,\"files\":1200,"
    "\"bytes\":1073741824,\"action\":\"would_purge\",\"reason\":null},"
    "{\"volume\":\"Inc-0007\",\"pool\":\"Incremental\",\"volstatus\":\"Used\",\"jobs\":\"2\","
    "\"files\":\"40\",\"bytes\":\"2048\",\"action\":\"skipped\",\"reason\":\"volume is in use\"}"
    "],\"summary\":{\"selected\":2,\"purged\":1,\"skipped\":1}}}}";

const char *YesReply =
    "{\"purge\":{\"mode\":\"yes\",\"pool\":\"Full\",\"allpools\":false,\"volstatus\":[\"Full\"],"
    "\"volumes\":[{\"volume\":\"Full-0001\",\"pool\":\"Full\",\"volstatus\":\"Full\",\"jobs\":3,"
    "\"files\":1200,\"bytes\":1073741824,\"action\":\"purged\",\"reason\":null}],"
    "\"summary\":{\"selected\":1,\"purged\":1,\"skipped\":0}}}";

const char *RefusalReply =
    "{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":{\"code\":1,\"message\":\"failed\",\"data\":"
    "{\"messages\":{\"error\":[\"Permission denied: need full pool permission.\"]}}}}";

const char *EmptyDryRunReply =
    "{\"purge\":{\"mode\":\"dryrun\",\"pool\":null,\"allpools\":true,\"volstatus\":[\"Full\"],"
    "\"volumes\":[],\"summary\":{\"selected\":0,\"purged\":0,\"skipped\":0}}}";

Selection allPools(QStringList statuses = defaultStatuses())
{
    Selection selection;
    selection.allPools = true;
    selection.volStatus = statuses;
    return selection;
}

Selection onePool(const QString &pool, QStringList statuses = defaultStatuses())
{
    Selection selection;
    selection.pool = pool;
    selection.volStatus = statuses;
    return selection;
}

} // namespace

class BVolumePurgeTest : public QObject
{
    Q_OBJECT

private slots:
    void commandLines();
    void invalidSelectionsAreRefused_data();
    void invalidSelectionsAreRefused();
    void parseDryRun();
    void parseYes();
    void parseRefusal();
    void parseUnusable();

    void purgeOnlyAfterPreview();
    void changedSelectionInvalidatesPreview();
    void declinedConfirmationSendsNothing();
    void refusalKeepsPurgeDisabled();
    void emptyPreviewCannotBePurged();
    void wrongModeIsNotAPreview();
    void failureEndsTheWait();
};

void BVolumePurgeTest::commandLines()
{
    QCOMPARE(buildCommand(allPools(), Mode::DryRun),
             QStringLiteral("purge volume all allpools volstatus=Full,Used,Error dryrun"));
    QCOMPARE(buildCommand(allPools(), Mode::Purge),
             QStringLiteral("purge volume all allpools volstatus=Full,Used,Error yes"));
    QCOMPARE(buildCommand(onePool(QStringLiteral("Full")), Mode::Purge),
             QStringLiteral("purge volume all pool=Full volstatus=Full,Used,Error yes"));
    // A name with a space is quoted
    QCOMPARE(buildCommand(onePool(QStringLiteral("Off Site")), Mode::DryRun),
             QStringLiteral("purge volume all pool=\"Off Site\" volstatus=Full,Used,Error dryrun"));
    // The statuses go out in the fixed order, whatever order they were chosen in
    QCOMPARE(buildCommand(onePool(QStringLiteral("P"),
                                  {QStringLiteral("Read-Only"), QStringLiteral("Append"),
                                   QStringLiteral("Full")}),
                          Mode::DryRun),
             QStringLiteral("purge volume all pool=P volstatus=Full,Append,Read-Only dryrun"));

    QString error = QStringLiteral("stale");
    QVERIFY(!buildCommand(allPools(), Mode::DryRun, &error).isEmpty());
    QVERIFY(error.isEmpty());

    QCOMPARE(allowedStatuses().size(), 7);
    QCOMPARE(defaultStatuses(),
             (QStringList{QStringLiteral("Full"), QStringLiteral("Used"), QStringLiteral("Error")}));
}

void BVolumePurgeTest::invalidSelectionsAreRefused_data()
{
    QTest::addColumn<bool>("all");
    QTest::addColumn<QString>("pool");
    QTest::addColumn<QStringList>("statuses");

    QTest::newRow("neither pool nor all pools") << false << QString() << defaultStatuses();
    QTest::newRow("blank pool") << false << QStringLiteral("   ") << defaultStatuses();
    QTest::newRow("pool and all pools") << true << QStringLiteral("Full") << defaultStatuses();
    QTest::newRow("no status") << true << QString() << QStringList();
    QTest::newRow("unknown status") << true << QString() << QStringList{QStringLiteral("Recycle")};
    QTest::newRow("lower-case status") << true << QString() << QStringList{QStringLiteral("full")};
    QTest::newRow("quote in pool") << false << QStringLiteral("a\"b") << defaultStatuses();
    QTest::newRow("newline in pool") << false << QStringLiteral("a\nyes") << defaultStatuses();
    QTest::newRow("backslash in pool") << false << QStringLiteral("a\\b") << defaultStatuses();
}

void BVolumePurgeTest::invalidSelectionsAreRefused()
{
    QFETCH(bool, all);
    QFETCH(QString, pool);
    QFETCH(QStringList, statuses);

    Selection selection;
    selection.allPools = all;
    selection.pool = pool;
    selection.volStatus = statuses;

    for (Mode mode : {Mode::DryRun, Mode::Purge}) {
        QString error;
        QVERIFY(buildCommand(selection, mode, &error).isEmpty());
        QVERIFY(!error.isEmpty());
    }
}

void BVolumePurgeTest::parseDryRun()
{
    const Reply reply = parseReply(QString::fromLatin1(DryRunReply));
    QVERIFY(reply.ok);
    QVERIFY(reply.error.isEmpty());
    QVERIFY(reply.modeKnown);
    QCOMPARE(reply.mode, Mode::DryRun);
    QVERIFY(reply.allPools);
    QVERIFY(reply.pool.isEmpty());
    QCOMPARE(reply.volStatus, defaultStatuses());
    QCOMPARE(reply.selected, 2);
    QCOMPARE(reply.purged, 1);
    QCOMPARE(reply.skipped, 1);
    QCOMPARE(reply.volumes.size(), 2);

    const VolumeRow &first = reply.volumes.at(0);
    QCOMPARE(first.volume, QStringLiteral("Full-0001"));
    QCOMPARE(first.pool, QStringLiteral("Full"));
    QCOMPARE(first.volStatus, QStringLiteral("Full"));
    QCOMPARE(first.jobs, qint64(3));
    QCOMPARE(first.files, qint64(1200));
    QCOMPARE(first.bytes, qint64(1073741824));
    QCOMPARE(first.action, Action::WouldPurge);
    QVERIFY(first.reason.isEmpty());

    // Counts as strings are read too
    const VolumeRow &second = reply.volumes.at(1);
    QCOMPARE(second.jobs, qint64(2));
    QCOMPARE(second.files, qint64(40));
    QCOMPARE(second.bytes, qint64(2048));
    QCOMPARE(second.action, Action::Skipped);
    QCOMPARE(second.reason, QStringLiteral("volume is in use"));
}

void BVolumePurgeTest::parseYes()
{
    const Reply reply = parseReply(QString::fromLatin1(YesReply));
    QVERIFY(reply.ok);
    QCOMPARE(reply.mode, Mode::Purge);
    QVERIFY(!reply.allPools);
    QCOMPARE(reply.pool, QStringLiteral("Full"));
    QCOMPARE(reply.volumes.size(), 1);
    QCOMPARE(reply.volumes.at(0).action, Action::Purged);
    QCOMPARE(reply.selected, 1);
    QCOMPARE(reply.purged, 1);
    QCOMPARE(reply.skipped, 0);
}

void BVolumePurgeTest::parseRefusal()
{
    const Reply reply = parseReply(QString::fromLatin1(RefusalReply));
    QVERIFY(!reply.ok);
    QCOMPARE(reply.error, QStringLiteral("Permission denied: need full pool permission."));
    QVERIFY(reply.volumes.isEmpty());

    // Without messages the error's own message is taken
    const Reply plain = parseReply(QStringLiteral(
        "{\"error\":{\"code\":1,\"message\":\"Command failed\"}}"));
    QVERIFY(!plain.ok);
    QCOMPARE(plain.error, QStringLiteral("Command failed"));
}

void BVolumePurgeTest::parseUnusable()
{
    // Plain text (a Director without the form) is not a success
    const Reply text = parseReply(QStringLiteral("Invalid keyword: all"));
    QVERIFY(!text.ok);
    QVERIFY(text.error.contains(QStringLiteral("Invalid keyword: all")));

    QVERIFY(!parseReply(QString()).ok);
    QVERIFY(!parseReply(QStringLiteral("{\"result\":{}}")).ok);
    // No mode: not taken as a preview
    QVERIFY(!parseReply(QStringLiteral("{\"purge\":{\"volumes\":[]}}")).ok);
}

// --- The panel ---------------------------------------------------------------

namespace {

struct PanelFixture {
    BVolumePurgePanel panel;
    QStringList sent;
    QStringList questions;
    bool answer = true;

    PanelFixture()
    {
        panel.setSender([this](const QString &line) { sent.append(line); });
        panel.setConfirm([this](const QString &, const QString &text) {
            questions.append(text);
            return answer;
        });
        panel.setPoolNames({QStringLiteral("Full"), QStringLiteral("Incremental")});
    }

    QPushButton *previewButton() { return panel.findChild<QPushButton *>(QStringLiteral("purgePreviewButton")); }
    QPushButton *purgeButton() { return panel.findChild<QPushButton *>(QStringLiteral("purgeButton")); }
    QComboBox *poolCombo() { return panel.findChild<QComboBox *>(QStringLiteral("purgePoolCombo")); }
    QCheckBox *status(const QString &name) { return panel.findChild<QCheckBox *>(QStringLiteral("purgeStatus_") + name); }
    QTableWidget *table() { return panel.findChild<QTableWidget *>(QStringLiteral("purgeTable")); }
    QString summary() { return panel.findChild<QLabel *>(QStringLiteral("purgeSummary"))->text(); }

    void preview()
    {
        previewButton()->click();
        panel.handleReply(QString::fromLatin1(DryRunReply));
    }
};

} // namespace

void BVolumePurgeTest::purgeOnlyAfterPreview()
{
    PanelFixture f;
    QVERIFY(f.previewButton() && f.purgeButton() && f.poolCombo() && f.table());

    // Defaults: all pools, Full, Used and Error
    QVERIFY(f.status(QStringLiteral("Full"))->isChecked());
    QVERIFY(f.status(QStringLiteral("Used"))->isChecked());
    QVERIFY(f.status(QStringLiteral("Error"))->isChecked());
    QVERIFY(!f.status(QStringLiteral("Append"))->isChecked());
    QCOMPARE(f.poolCombo()->count(), 3);

    QVERIFY(!f.purgeButton()->isEnabled());
    // A click on the disabled button does nothing
    f.purgeButton()->click();
    QVERIFY(f.sent.isEmpty());
    QVERIFY(f.questions.isEmpty());

    f.previewButton()->click();
    QCOMPARE(f.sent, QStringList{QStringLiteral("purge volume all allpools volstatus=Full,Used,Error dryrun")});
    QVERIFY(f.panel.isBusy());
    QVERIFY(!f.purgeButton()->isEnabled());
    QVERIFY(!f.previewButton()->isEnabled());

    f.panel.handleReply(QString::fromLatin1(DryRunReply));
    QVERIFY(!f.panel.isBusy());
    QVERIFY(f.panel.hasValidPreview());
    QVERIFY(f.purgeButton()->isEnabled());
    QCOMPARE(f.table()->rowCount(), 2);
    QCOMPARE(f.table()->item(0, 0)->text(), QStringLiteral("Full-0001"));
    QCOMPARE(f.table()->item(0, 5)->text(), QStringLiteral("1 GiB"));
    QCOMPARE(f.table()->item(0, 6)->text(), QStringLiteral("Would purge"));
    QCOMPARE(f.table()->item(1, 6)->text(), QStringLiteral("Skipped: volume is in use"));
    QVERIFY(f.summary().contains(QStringLiteral("2 volumes selected")));

    f.purgeButton()->click();
    QCOMPARE(f.questions.size(), 1);
    // The question says plainly that retention periods do not count
    QVERIFY(f.questions.first().contains(QStringLiteral("ignores the retention periods")));
    QVERIFY(f.questions.first().contains(QStringLiteral("cannot be undone")));
    QCOMPARE(f.sent.size(), 2);
    QCOMPARE(f.sent.last(), QStringLiteral("purge volume all allpools volstatus=Full,Used,Error yes"));
    QVERIFY(f.panel.isBusy());

    QSignalSpy finished(&f.panel, &BVolumePurgePanel::purgeFinished);
    f.panel.handleReply(QString::fromLatin1(YesReply));
    QCOMPARE(finished.size(), 1);
    QCOMPARE(f.table()->rowCount(), 1);
    QCOMPARE(f.table()->item(0, 6)->text(), QStringLiteral("Purged"));
    QVERIFY(f.summary().contains(QStringLiteral("1 purged")));

    // The preview is used up: no second purge without a new preview
    QVERIFY(!f.panel.hasValidPreview());
    QVERIFY(!f.purgeButton()->isEnabled());

    // A reply nobody waits for changes nothing
    f.panel.handleReply(QString::fromLatin1(DryRunReply));
    QVERIFY(!f.purgeButton()->isEnabled());
}

void BVolumePurgeTest::changedSelectionInvalidatesPreview()
{
    PanelFixture f;
    f.preview();
    QVERIFY(f.purgeButton()->isEnabled());

    f.status(QStringLiteral("Append"))->setChecked(true);
    QVERIFY(!f.panel.hasValidPreview());
    QVERIFY(!f.purgeButton()->isEnabled());
    QCOMPARE(f.table()->rowCount(), 0);

    // Back to the old selection: still a new preview is needed
    f.status(QStringLiteral("Append"))->setChecked(false);
    QVERIFY(!f.purgeButton()->isEnabled());

    f.preview();
    QVERIFY(f.purgeButton()->isEnabled());
    f.poolCombo()->setCurrentIndex(1);
    QVERIFY(!f.purgeButton()->isEnabled());

    // The next preview is for the chosen pool
    f.previewButton()->click();
    QCOMPARE(f.sent.last(), QStringLiteral("purge volume all pool=Full volstatus=Full,Used,Error dryrun"));

    // A new pool list that keeps the chosen pool keeps the selection
    f.panel.handleReply(QString::fromLatin1(DryRunReply));
    QVERIFY(f.purgeButton()->isEnabled());
    f.panel.setPoolNames({QStringLiteral("Incremental"), QStringLiteral("Full")});
    QVERIFY(f.purgeButton()->isEnabled());
    // ... one without it falls back to all pools and invalidates the preview
    f.panel.setPoolNames({QStringLiteral("Incremental")});
    QVERIFY(f.panel.currentSelection().allPools);
    QVERIFY(!f.purgeButton()->isEnabled());

    // No status: the preview is refused before anything is sent
    const int sentBefore = f.sent.size();
    for (const QString &status : allowedStatuses()) {
        f.status(status)->setChecked(false);
    }
    f.previewButton()->click();
    QCOMPARE(f.sent.size(), sentBefore);
    QVERIFY(!f.panel.isBusy());
}

void BVolumePurgeTest::declinedConfirmationSendsNothing()
{
    PanelFixture f;
    f.answer = false;
    f.preview();
    f.purgeButton()->click();
    QCOMPARE(f.questions.size(), 1);
    QCOMPARE(f.sent.size(), 1);  // only the dryrun
    QVERIFY(!f.panel.isBusy());
    // The preview stays and can still be executed
    QVERIFY(f.purgeButton()->isEnabled());
}

void BVolumePurgeTest::refusalKeepsPurgeDisabled()
{
    PanelFixture f;
    f.previewButton()->click();
    f.panel.handleReply(QString::fromLatin1(RefusalReply));
    QVERIFY(!f.panel.isBusy());
    QVERIFY(!f.panel.hasValidPreview());
    QVERIFY(!f.purgeButton()->isEnabled());
    QVERIFY(f.summary().contains(QStringLiteral("Permission denied: need full pool permission.")));
    QVERIFY(f.previewButton()->isEnabled());
}

void BVolumePurgeTest::emptyPreviewCannotBePurged()
{
    PanelFixture f;
    f.previewButton()->click();
    f.panel.handleReply(QString::fromLatin1(EmptyDryRunReply));
    QVERIFY(f.panel.hasValidPreview());
    QVERIFY(!f.purgeButton()->isEnabled());
}

void BVolumePurgeTest::wrongModeIsNotAPreview()
{
    PanelFixture f;
    f.previewButton()->click();
    f.panel.handleReply(QString::fromLatin1(YesReply));
    QVERIFY(!f.panel.hasValidPreview());
    QVERIFY(!f.purgeButton()->isEnabled());
}

void BVolumePurgeTest::failureEndsTheWait()
{
    PanelFixture f;
    f.panel.setReplyTimeout(10);
    f.previewButton()->click();
    QVERIFY(f.panel.isBusy());
    QTRY_VERIFY(!f.panel.isBusy());
    QVERIFY(!f.purgeButton()->isEnabled());

    // A lost connection during the purge: the result is unknown
    f.panel.setReplyTimeout(60000);
    f.preview();
    f.purgeButton()->click();
    QVERIFY(f.panel.isBusy());
    f.panel.handleFailure(QStringLiteral("the connection to the Director was lost"));
    QVERIFY(!f.panel.isBusy());
    QVERIFY(f.summary().contains(QStringLiteral("unknown")));
    QVERIFY(!f.purgeButton()->isEnabled());
}

QTEST_MAIN(BVolumePurgeTest)
#include "bvolumepurge_test.moc"
