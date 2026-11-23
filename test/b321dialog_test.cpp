/**
 * @file b321dialog_test.cpp
 * @brief Tools > 3-2-1 Verifier (B321Dialog), offscreen, without a Director
 *
 * - The worked example (the production Director's volume list with the
 *   jobs of test/fixtures/pdc-321-*.json): one row per backup job, chips
 *   copies 2/3 and media 1/2 red, off site unknown grey, verdict "Fails".
 * - Green only for a met criterion: a second example with copies on tape
 *   and the tape storage marked off site.
 * - The off-site marks per Director profile and storage, "unmarked storages
 *   are on site" and "count the original data as a copy" are kept in
 *   BSettings and change the chips at once.
 * - Refresh sends "llist jobs days=3650 reverse limit=20000",
 *   "list jobmedia" and "llist volumes", nothing else, and nothing without
 *   a connection; a job history asked for by the schedule view is ignored.
 * - Screen reader texts, buddies, tab order; the header state is saved.
 * - Pictures (dark and light) when ONESIMUS_321_SCREENSHOT_DIR is set.
 *
 * Settings are redirected to a temporary directory. TZ=UTC.
 */

#include <QtTest>

#include <QApplication>
#include <QCheckBox>
#include <QFile>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QSettings>
#include <QTableWidget>
#include <QTemporaryDir>
#include <QTextBrowser>
#include <QTreeWidget>

#include "bstatuschip.h"
#include "bstatusdelegate.h"
#include "buiscale.h"
#include "config/bsettings.h"
#include "tools/b321dialog.h"

namespace {

const QDateTime kNow(QDate(2026, 11, 19), QTime(12, 0), QTimeZone::UTC);

QString readFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return QString();
    return QString::fromUtf8(file.readAll());
}

QString fixture(const char *name)
{
    return readFile(QStringLiteral(FIXTURE_DIR "/") + QLatin1String(name));
}

QString theme(const QString &name)
{
    return readFile(QStringLiteral(THEME_DIR "/%1.qss").arg(name));
}

QJsonObject showJob(const char *type, const char *client, const char *fileset, const char *jobDefs = nullptr)
{
    QJsonObject job;
    if (type) job.insert(QStringLiteral("type"), QLatin1String(type));
    if (client) job.insert(QStringLiteral("client"), QLatin1String(client));
    if (fileset) job.insert(QStringLiteral("fileset"), QLatin1String(fileset));
    if (jobDefs) job.insert(QStringLiteral("jobdefs"), QLatin1String(jobDefs));
    return job;
}

/// "show jobs" of the worked example: five backup jobs, one of them typed by
/// its JobDefs, one restore job
B321Dialog::Context pdcContext(const QString &profile)
{
    B321Dialog::Context c;
    c.jobs.insert(QStringLiteral("Tapir"), showJob("Backup", "tapir-fd", "TapirSet"));
    c.jobs.insert(QStringLiteral("Forgejo"), showJob("Backup", "forgejo-fd", "ForgejoSet"));
    c.jobs.insert(QStringLiteral("Catalog"), showJob(nullptr, "pdc-fd", "CatalogSet", "DefaultJob"));
    c.jobs.insert(QStringLiteral("Cloud"), showJob("Backup", "pdc-fd", "CloudSet"));
    c.jobs.insert(QStringLiteral("BGLAGE"), showJob("Backup", "bglage-fd", "BglageSet"));
    c.jobs.insert(QStringLiteral("RestoreFiles"), showJob("Restore", "pdc-fd", nullptr));
    c.jobDefs.insert(QStringLiteral("DefaultJob"), showJob("Backup", nullptr, nullptr));
    c.storages = {QStringLiteral("PDC-sd")};
    c.profileId = profile;
    return c;
}

void feedPdc(B321Dialog &dialog)
{
    dialog.processJobHistory(B321Dialog::jobRequestArgs(), fixture("pdc-321-llist-jobs-api2.json"));
    dialog.processJsonResult(BDirector::Command::ListJobMedia, fixture("pdc-321-list-jobmedia-api2.json"));
    dialog.processJsonResult(BDirector::Command::LlistVolumes, fixture("pdc-llist-volumes-api2.json"));
}

// Second example: copies on tape. Srv and Web are copied whole to the tape
// pool, Mail only in part.
const char *kMixedJobs = R"({"result":{"jobs":[
 {"jobid":"1","name":"Srv","type":"B","level":"F","jobstatus":"T","priorjobid":"0","client":"srv-fd","fileset":"SrvSet","poolname":"Full","starttime":"2026-11-14 23:00:00","endtime":"2026-11-15 00:10:00","realendtime":"2026-11-15 00:10:00"},
 {"jobid":"2","name":"Srv","type":"B","level":"I","jobstatus":"T","priorjobid":"0","client":"srv-fd","fileset":"SrvSet","poolname":"Incremental","starttime":"2026-11-18 23:00:00","endtime":"2026-11-18 23:05:00","realendtime":"2026-11-18 23:05:00"},
 {"jobid":"10","name":"Srv","type":"C","level":"F","jobstatus":"T","priorjobid":"1","client":"srv-fd","fileset":"SrvSet","poolname":"TapeCopy","starttime":"2026-11-14 23:00:00","endtime":"2026-11-15 00:10:00","realendtime":"2026-11-19 06:00:00"},
 {"jobid":"11","name":"Srv","type":"C","level":"I","jobstatus":"T","priorjobid":"2","client":"srv-fd","fileset":"SrvSet","poolname":"TapeCopy","starttime":"2026-11-18 23:00:00","endtime":"2026-11-18 23:05:00","realendtime":"2026-11-19 06:10:00"},
 {"jobid":"3","name":"Web","type":"B","level":"F","jobstatus":"T","priorjobid":"0","client":"web-fd","fileset":"WebSet","poolname":"Full","starttime":"2026-11-17 22:00:00","endtime":"2026-11-17 22:30:00","realendtime":"2026-11-17 22:30:00"},
 {"jobid":"12","name":"Web","type":"C","level":"F","jobstatus":"T","priorjobid":"3","client":"web-fd","fileset":"WebSet","poolname":"TapeCopy","starttime":"2026-11-17 22:00:00","endtime":"2026-11-17 22:30:00","realendtime":"2026-11-19 06:20:00"},
 {"jobid":"4","name":"Mail","type":"B","level":"F","jobstatus":"T","priorjobid":"0","client":"mail-fd","fileset":"MailSet","poolname":"Full","starttime":"2026-11-10 21:00:00","endtime":"2026-11-10 22:00:00","realendtime":"2026-11-10 22:00:00"},
 {"jobid":"5","name":"Mail","type":"B","level":"I","jobstatus":"T","priorjobid":"0","client":"mail-fd","fileset":"MailSet","poolname":"Incremental","starttime":"2026-11-18 21:00:00","endtime":"2026-11-18 21:10:00","realendtime":"2026-11-18 21:10:00"},
 {"jobid":"13","name":"Mail","type":"C","level":"F","jobstatus":"T","priorjobid":"4","client":"mail-fd","fileset":"MailSet","poolname":"TapeCopy","starttime":"2026-11-10 21:00:00","endtime":"2026-11-10 22:00:00","realendtime":"2026-11-11 06:00:00"}
]}})";

const char *kMixedMedia = R"({"result":{"jobmedia":[
 {"jobid":"1","volumename":"File-1"},{"jobid":"2","volumename":"File-1"},
 {"jobid":"10","volumename":"Tape-1"},{"jobid":"11","volumename":"Tape-1"},
 {"jobid":"3","volumename":"File-2"},{"jobid":"12","volumename":"Tape-2"},
 {"jobid":"4","volumename":"File-3"},{"jobid":"5","volumename":"File-3"},
 {"jobid":"13","volumename":"Tape-2"}]}})";

const char *kMixedVolumes = R"({"result":{"volumes":[
 {"volumename":"File-1","pool":"Full","mediatype":"File","storage":"FileSD","volstatus":"Append","poolid":"1","locationid":"0"},
 {"volumename":"File-2","pool":"Full","mediatype":"File","storage":"FileSD","volstatus":"Full","poolid":"1","locationid":"0"},
 {"volumename":"File-3","pool":"Full","mediatype":"File","storage":"FileSD","volstatus":"Append","poolid":"1","locationid":"0"},
 {"volumename":"Tape-1","pool":"TapeCopy","mediatype":"LTO-8","storage":"TapeLib","volstatus":"Append","poolid":"4","locationid":"0"},
 {"volumename":"Tape-2","pool":"TapeCopy","mediatype":"LTO-8","storage":"TapeLib","volstatus":"Full","poolid":"4","locationid":"0"}]}})";

B321Dialog::Context mixedContext()
{
    B321Dialog::Context c;
    c.jobs.insert(QStringLiteral("Srv"), showJob("Backup", "srv-fd", "SrvSet"));
    c.jobs.insert(QStringLiteral("Web"), showJob("Backup", "web-fd", "WebSet"));
    c.jobs.insert(QStringLiteral("Mail"), showJob("Backup", "mail-fd", "MailSet"));
    c.jobs.insert(QStringLiteral("CopyToTape"), showJob("Copy", nullptr, nullptr));
    c.storages = {QStringLiteral("FileSD"), QStringLiteral("TapeLib")};
    c.profileId = QStringLiteral("mixed");
    return c;
}

void feedMixed(B321Dialog &dialog)
{
    dialog.processJobHistory(B321Dialog::jobRequestArgs(), QLatin1String(kMixedJobs));
    dialog.processJsonResult(BDirector::Command::ListJobMedia, QLatin1String(kMixedMedia));
    dialog.processJsonResult(BDirector::Command::LlistVolumes, QLatin1String(kMixedVolumes));
}

int rowOf(const QTableWidget *table, const QString &job)
{
    for (int row = 0; row < table->rowCount(); ++row) {
        if (table->item(row, B321Dialog::JobColumn)->text() == job) return row;
    }
    return -1;
}

BStatus::Tone toneAt(const QTableWidget *table, int row, int column)
{
    return BStatus::Tone(table->item(row, column)->data(BStatus::ToneRole).toInt());
}

QListWidgetItem *storageItem(QListWidget *list, const QString &storage)
{
    for (int i = 0; i < list->count(); ++i) {
        if (list->item(i)->data(Qt::UserRole).toString() == storage) return list->item(i);
    }
    return nullptr;
}

} // namespace

class B321DialogTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    void backupDefinitionsFromShowJobs();
    void pdcChips();
    void pdcDetails();
    void greenOnlyForMetCriteria();
    void countOriginalSettingPersists();
    void offSiteMarksPerProfile();
    void refreshSendsReadCommandsOnly();
    void foreignHistoryIgnoredAndFailuresShown();
    void accessibility();
    void headerStateSaved();
    void screenshots();

private:
    QTemporaryDir m_config;
};

void B321DialogTest::initTestCase()
{
    QVERIFY(m_config.isValid());
    qputenv("XDG_CONFIG_HOME", m_config.path().toUtf8());
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, m_config.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_config.path());
    // Windows keeps NativeFormat in the registry, where setPath does not reach
    QSettings::setDefaultFormat(QSettings::IniFormat);
    const QSettings probe(QSettings::defaultFormat(), QSettings::UserScope,
                         QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
    QVERIFY2(probe.fileName().startsWith(m_config.path()), qPrintable(probe.fileName()));

    QFont font(QStringLiteral("DejaVu Sans"));
    font.setPointSizeF(10);
    BUiScale::instance().setBaseFont(font);
    BUiScale::instance().setApplicationStyleSheet(theme(QStringLiteral("dark")) + BUiScale::themeRules());
}

void B321DialogTest::init()
{
    BSettings &s = BSettings::instance();
    s.setVerify321CountOriginalData(true);
    for (const QString &profile : {QStringLiteral("pdc"), QStringLiteral("other"), QStringLiteral("mixed")}) {
        for (const QString &storage : {QStringLiteral("PDC-sd"), QStringLiteral("FileSD"), QStringLiteral("TapeLib")})
            s.setVerify321OffSite(profile, storage, false);
        s.setVerify321UnmarkedOnSite(profile, false);
    }
}

void B321DialogTest::backupDefinitionsFromShowJobs()
{
    const B321Dialog::Context c = pdcContext(QStringLiteral("pdc"));
    const QList<B321Verifier::Definition> defs = B321Dialog::backupDefinitions(c.jobs, c.jobDefs);
    QStringList names;
    for (const B321Verifier::Definition &d : defs) names.append(d.name);
    // Sorted; "Catalog" is a Backup through its JobDefs; no restore job
    QCOMPARE(names, (QStringList{"BGLAGE", "Catalog", "Cloud", "Forgejo", "Tapir"}));
    QCOMPARE(defs.at(1).client, QStringLiteral("pdc-fd"));

    // A reference may come as {"name": ...}
    QMap<QString, QJsonObject> jobs;
    QJsonObject job;
    job.insert(QStringLiteral("type"), QStringLiteral("backup"));
    job.insert(QStringLiteral("client"), QJsonObject{{QStringLiteral("name"), QStringLiteral("x-fd")}});
    jobs.insert(QStringLiteral("X"), job);
    QCOMPARE(B321Dialog::backupDefinitions(jobs, {}).first().client, QStringLiteral("x-fd"));
}

void B321DialogTest::pdcChips()
{
    B321Dialog dialog([] { return pdcContext(QStringLiteral("pdc")); });
    dialog.setReferenceTime(kNow);
    feedPdc(dialog);
    QTableWidget *table = dialog.table();
    QCOMPARE(table->rowCount(), 5);
    for (int row = 0; row < table->rowCount(); ++row) {
        const QByteArray job = table->item(row, B321Dialog::JobColumn)->text().toUtf8();
        QCOMPARE(table->item(row, B321Dialog::CopiesColumn)->text(), QStringLiteral("2/3"));
        QCOMPARE(toneAt(table, row, B321Dialog::CopiesColumn), BStatus::Tone::Error);
        QCOMPARE(table->item(row, B321Dialog::CopiesColumn)->data(Qt::AccessibleTextRole).toString(),
                 QStringLiteral("Copies: 2 of 3, not met"));
        QCOMPARE(table->item(row, B321Dialog::MediaColumn)->text(), QStringLiteral("1/2"));
        QCOMPARE(toneAt(table, row, B321Dialog::MediaColumn), BStatus::Tone::Error);
        QCOMPARE(table->item(row, B321Dialog::OffSiteColumn)->text(), QStringLiteral("unknown"));
        QCOMPARE(toneAt(table, row, B321Dialog::OffSiteColumn), BStatus::Tone::Neutral);
        QCOMPARE(table->item(row, B321Dialog::OffSiteColumn)->data(BStatus::CodeRole).toString(), QStringLiteral("?"));
        QCOMPARE(table->item(row, B321Dialog::VerdictColumn)->text(), QStringLiteral("Fails"));
        QCOMPARE(toneAt(table, row, B321Dialog::VerdictColumn), BStatus::Tone::Error);
        QVERIFY2(table->item(row, B321Dialog::ReasonColumn)->text().contains(QStringLiteral("2 of 3 copies")),
                 job.constData());
        // The delegate paints a chip in each of the four columns
        for (int column : {int(B321Dialog::CopiesColumn), int(B321Dialog::MediaColumn),
                           int(B321Dialog::OffSiteColumn), int(B321Dialog::VerdictColumn)}) {
            QVERIFY(qobject_cast<BStatusDelegate *>(table->itemDelegateForColumn(column)));
            QVERIFY(BStatusDelegate::chipColors(table->model()->index(row, column)).background.isValid());
        }
    }
    QVERIFY(dialog.stateLabel()->text().contains(QStringLiteral("5 job(s) checked: 0 pass, 5 fail, 0 unknown")));
    QCOMPARE(dialog.stateLabel()->property("state").toString(), QStringLiteral("warning"));
    // The storage of the volumes is offered for marking
    QVERIFY(storageItem(dialog.offSiteList(), QStringLiteral("PDC-sd")));
    QCOMPARE(dialog.offSiteList()->count(), 1);
}

void B321DialogTest::pdcDetails()
{
    B321Dialog dialog([] { return pdcContext(QStringLiteral("pdc")); });
    dialog.setReferenceTime(kNow);
    feedPdc(dialog);
    QTableWidget *table = dialog.table();
    const int row = rowOf(table, QStringLiteral("Catalog"));
    QVERIFY(row >= 0);
    table->setCurrentCell(row, B321Dialog::JobColumn);

    QTreeWidget *tree = dialog.detailTree();
    QCOMPARE(tree->topLevelItemCount(), 1);
    QTreeWidgetItem *chain = tree->topLevelItem(0);
    QVERIFY2(chain->text(0).startsWith(QStringLiteral("Chain (the backup itself): 3 job(s)")), qPrintable(chain->text(0)));
    QVERIFY(chain->text(0).contains(QStringLiteral("File, PDC-sd, unknown")));
    QCOMPARE(chain->childCount(), 3);
    QVERIFY2(chain->child(0)->text(0).contains(QStringLiteral("Full 60")), qPrintable(chain->child(0)->text(0)));
    QVERIFY(chain->child(0)->text(0).contains(QStringLiteral("Vol-0005 (File, PDC-sd, Error)")));

    const QString text = dialog.detailText()->toPlainText();
    QVERIFY(text.contains(QStringLiteral("Catalog: Fails")));
    QVERIFY(text.contains(QStringLiteral("What to do")));
    QVERIFY(text.contains(QStringLiteral("Mark storage PDC-sd as off site")));
    QVERIFY(text.contains(QStringLiteral("Hints (not part of the verdict)")));
    QVERIFY(text.contains(QStringLiteral("which is in status Error")));
}

void B321DialogTest::greenOnlyForMetCriteria()
{
    BSettings::instance().setVerify321OffSite(QStringLiteral("mixed"), QStringLiteral("TapeLib"), true);
    B321Dialog dialog(mixedContext);
    dialog.setReferenceTime(kNow);
    feedMixed(dialog);
    QTableWidget *table = dialog.table();
    QCOMPARE(table->rowCount(), 3);     // the Copy job is no row

    const int srv = rowOf(table, QStringLiteral("Srv"));
    QCOMPARE(table->item(srv, B321Dialog::CopiesColumn)->text(), QStringLiteral("3/3"));
    QCOMPARE(table->item(srv, B321Dialog::MediaColumn)->text(), QStringLiteral("2/2"));
    QCOMPARE(table->item(srv, B321Dialog::OffSiteColumn)->text(), QStringLiteral("1/1"));
    for (int column : {int(B321Dialog::CopiesColumn), int(B321Dialog::MediaColumn),
                       int(B321Dialog::OffSiteColumn), int(B321Dialog::VerdictColumn)})
        QCOMPARE(toneAt(table, srv, column), BStatus::Tone::Success);
    QCOMPARE(table->item(srv, B321Dialog::VerdictColumn)->text(), QStringLiteral("Passes"));

    const int mail = rowOf(table, QStringLiteral("Mail"));
    QCOMPARE(table->item(mail, B321Dialog::CopiesColumn)->text(), QStringLiteral("2/3"));
    QCOMPARE(toneAt(table, mail, B321Dialog::CopiesColumn), BStatus::Tone::Error);
    QCOMPARE(toneAt(table, mail, B321Dialog::OffSiteColumn), BStatus::Tone::Neutral);
    QCOMPARE(toneAt(table, mail, B321Dialog::VerdictColumn), BStatus::Tone::Error);
    table->setCurrentCell(mail, B321Dialog::JobColumn);
    QCOMPARE(dialog.detailTree()->topLevelItemCount(), 2);
    QVERIFY(dialog.detailTree()->topLevelItem(1)->text(0).contains(QStringLiteral("partial, 1 of 2 jobs")));

    // Nothing that is not met is green
    for (int row = 0; row < table->rowCount(); ++row) {
        const B321Verifier::Result *result = nullptr;
        for (const B321Verifier::Result &r : dialog.results()) {
            if (r.job.name == table->item(row, B321Dialog::JobColumn)->text()) result = &r;
        }
        QVERIFY(result);
        QCOMPARE(toneAt(table, row, B321Dialog::CopiesColumn) == BStatus::Tone::Success,
                 result->copiesState == B321Verifier::State::Met);
        QCOMPARE(toneAt(table, row, B321Dialog::MediaColumn) == BStatus::Tone::Success,
                 result->mediaState == B321Verifier::State::Met);
        QCOMPARE(toneAt(table, row, B321Dialog::OffSiteColumn) == BStatus::Tone::Success,
                 result->offSiteState == B321Verifier::State::Met);
    }
}

void B321DialogTest::countOriginalSettingPersists()
{
    {
        B321Dialog dialog([] { return pdcContext(QStringLiteral("pdc")); });
        dialog.setReferenceTime(kNow);
        feedPdc(dialog);
        QVERIFY(dialog.countOriginalBox()->isChecked());
        dialog.countOriginalBox()->setChecked(false);
        QCOMPARE(dialog.table()->item(0, B321Dialog::CopiesColumn)->text(), QStringLiteral("1/3"));
        QVERIFY(!BSettings::instance().verify321CountOriginalData());
    }
    B321Dialog again([] { return pdcContext(QStringLiteral("pdc")); });
    QVERIFY(!again.countOriginalBox()->isChecked());
    again.countOriginalBox()->setChecked(true);
    QVERIFY(BSettings::instance().verify321CountOriginalData());
}

void B321DialogTest::offSiteMarksPerProfile()
{
    {
        B321Dialog dialog([] { return pdcContext(QStringLiteral("pdc")); });
        dialog.setReferenceTime(kNow);
        feedPdc(dialog);
        QListWidgetItem *item = storageItem(dialog.offSiteList(), QStringLiteral("PDC-sd"));
        QVERIFY(item);
        QCOMPARE(item->checkState(), Qt::Unchecked);
        item->setCheckState(Qt::Checked);
        QVERIFY(BSettings::instance().verify321OffSite(QStringLiteral("pdc"), QStringLiteral("PDC-sd")));
        QCOMPARE(item->data(Qt::AccessibleTextRole).toString(), QStringLiteral("PDC-sd, off site"));
        QTableWidget *table = dialog.table();
        for (int row = 0; row < table->rowCount(); ++row) {
            QCOMPARE(table->item(row, B321Dialog::OffSiteColumn)->text(), QStringLiteral("1/1"));
            QCOMPARE(toneAt(table, row, B321Dialog::OffSiteColumn), BStatus::Tone::Success);
            // Still fails: 2 copies, 1 media type
            QCOMPARE(table->item(row, B321Dialog::VerdictColumn)->text(), QStringLiteral("Fails"));
        }
    }
    {
        // Another Director profile has marks of its own
        B321Dialog other([] { return pdcContext(QStringLiteral("other")); });
        other.setReferenceTime(kNow);
        feedPdc(other);
        QCOMPARE(storageItem(other.offSiteList(), QStringLiteral("PDC-sd"))->checkState(), Qt::Unchecked);
        // "Unmarked storages are on site": nothing off site, a failed criterion
        other.unmarkedOnSiteBox()->setChecked(true);
        QVERIFY(BSettings::instance().verify321UnmarkedOnSite(QStringLiteral("other")));
        QCOMPARE(other.table()->item(0, B321Dialog::OffSiteColumn)->text(), QStringLiteral("0/1"));
        QCOMPARE(toneAt(other.table(), 0, B321Dialog::OffSiteColumn), BStatus::Tone::Error);
    }
    // The marks are read again by a new window of the first profile
    B321Dialog again([] { return pdcContext(QStringLiteral("pdc")); });
    again.setReferenceTime(kNow);
    feedPdc(again);
    QCOMPARE(storageItem(again.offSiteList(), QStringLiteral("PDC-sd"))->checkState(), Qt::Checked);
    QVERIFY(!again.unmarkedOnSiteBox()->isChecked());
}

void B321DialogTest::refreshSendsReadCommandsOnly()
{
    B321Dialog dialog([] { return pdcContext(QStringLiteral("pdc")); });
    QSignalSpy sent(&dialog, &B321Dialog::sendCommand);

    dialog.setConnected(false);
    dialog.refresh();
    QCOMPARE(sent.count(), 0);
    QVERIFY(dialog.stateLabel()->text().contains(QStringLiteral("Not connected")));

    dialog.setConnected(true);
    dialog.refresh();
    QCOMPARE(sent.count(), 3);
    QCOMPARE(sent.at(0).at(0).value<BDirector::Command>(), BDirector::Command::LlistJobs);
    QCOMPARE(sent.at(0).at(1).toString(), QStringLiteral("3650,20000"));
    QCOMPARE(BareosDirector::jobHistoryCommandLine(sent.at(0).at(1).toString()),
             QStringLiteral("llist jobs days=3650 reverse limit=20000"));
    QCOMPARE(sent.at(1).at(0).value<BDirector::Command>(), BDirector::Command::ListJobMedia);
    QCOMPARE(sent.at(1).at(1).toString(), QString());
    QCOMPARE(sent.at(2).at(0).value<BDirector::Command>(), BDirector::Command::LlistVolumes);
    QVERIFY(dialog.stateLabel()->text().startsWith(QStringLiteral("Loading: jobs, volumes of the jobs, volume list")));

    feedPdc(dialog);
    QVERIFY(dialog.stateLabel()->text().contains(QStringLiteral("Read only.")));
    QCOMPARE(sent.count(), 3);  // the replies send nothing further
}

void B321DialogTest::foreignHistoryIgnoredAndFailuresShown()
{
    B321Dialog dialog([] { return pdcContext(QStringLiteral("pdc")); });
    dialog.setConnected(true);
    dialog.setReferenceTime(kNow);
    // The schedule view's own history request is not this window's
    dialog.processJobHistory(QStringLiteral("60,5000"), fixture("pdc-321-llist-jobs-api2.json"));
    QCOMPARE(dialog.table()->rowCount(), 5);
    for (const B321Verifier::Result &r : dialog.results()) QVERIFY(!r.inCatalog);

    feedPdc(dialog);
    dialog.processCommandFailed(BDirector::Command::ListJobMedia, QString(), QStringLiteral("Command not authorized"));
    QVERIFY(dialog.stateLabel()->text().contains(QStringLiteral("Volumes of the jobs: Command not authorized")));
    QCOMPARE(dialog.stateLabel()->property("state").toString(), QStringLiteral("warning"));

    // A refused job list
    dialog.refresh();
    dialog.processJobHistory(B321Dialog::jobRequestArgs(),
                             QStringLiteral(R"({"error":{"message":"failed","data":{"messages":{"error":["llist: denied"]}}}})"));
    QVERIFY(dialog.stateLabel()->text().contains(QStringLiteral("Jobs: llist: denied")));
}

void B321DialogTest::accessibility()
{
    B321Dialog dialog([] { return pdcContext(QStringLiteral("pdc")); });
    dialog.setReferenceTime(kNow);
    feedPdc(dialog);
    QTableWidget *table = dialog.table();
    QVERIFY(!table->accessibleName().isEmpty());
    QVERIFY(!dialog.detailTree()->accessibleName().isEmpty());
    QVERIFY(!dialog.detailText()->accessibleName().isEmpty());
    QVERIFY(!dialog.offSiteList()->accessibleName().isEmpty());
    QVERIFY(!table->tabKeyNavigation());
    // Every list and view has a label with a mnemonic that leads to it
    for (QWidget *target : {static_cast<QWidget *>(table), static_cast<QWidget *>(dialog.detailTree()),
                            static_cast<QWidget *>(dialog.detailText()),
                            static_cast<QWidget *>(dialog.offSiteList())}) {
        bool found = false;
        for (QLabel *label : dialog.findChildren<QLabel *>()) {
            if (label->buddy() == target && label->text().contains(QLatin1Char('&'))) found = true;
        }
        QVERIFY2(found, qPrintable(target->accessibleName()));
    }
    // The row as a sentence
    const QString spoken = table->item(0, B321Dialog::JobColumn)->data(Qt::AccessibleTextRole).toString();
    QVERIFY2(spoken.contains(QStringLiteral(": Fails. 2 of 3 copies")), qPrintable(spoken));
    // Tab order: table, chain, text, then the settings
    auto nextTabStop = [](QWidget *from) {
        QWidget *w = from->nextInFocusChain();
        while (w != from && !(w->focusPolicy() & Qt::TabFocus)) w = w->nextInFocusChain();
        return w;
    };
    QCOMPARE(nextTabStop(table), static_cast<QWidget *>(dialog.detailTree()));
    QCOMPARE(nextTabStop(dialog.detailTree()), static_cast<QWidget *>(dialog.detailText()));
    QCOMPARE(nextTabStop(dialog.detailText()), static_cast<QWidget *>(dialog.countOriginalBox()));
    QCOMPARE(nextTabStop(dialog.countOriginalBox()), static_cast<QWidget *>(dialog.offSiteList()));
    QCOMPARE(nextTabStop(dialog.offSiteList()), static_cast<QWidget *>(dialog.unmarkedOnSiteBox()));
}

void B321DialogTest::headerStateSaved()
{
    BSettings::instance().setVerify321HeaderState(QByteArray());
    B321Dialog dialog([] { return pdcContext(QStringLiteral("pdc")); });
    dialog.table()->horizontalHeader()->resizeSection(B321Dialog::JobColumn, 222);
    QTRY_VERIFY_WITH_TIMEOUT(!BSettings::instance().verify321HeaderState().isEmpty(), 3000);
    QVERIFY(BSettings::instance().verify321HeaderScale() > 0);
    B321Dialog again([] { return pdcContext(QStringLiteral("pdc")); });
    QCOMPARE(again.table()->horizontalHeader()->sectionSize(B321Dialog::JobColumn), 222);
}

void B321DialogTest::screenshots()
{
    const QString dir = qEnvironmentVariable("ONESIMUS_321_SCREENSHOT_DIR");
    if (dir.isEmpty()) QSKIP("ONESIMUS_321_SCREENSHOT_DIR is not set");
    QVERIFY(QDir().mkpath(dir));
    for (const char *name : {"dark", "light"}) {
        BUiScale::instance().setApplicationStyleSheet(theme(QLatin1String(name)) + BUiScale::themeRules());
        BStatusPalette::refresh();
        auto save = [&](QWidget &window, const char *what) {
            window.show();
            QTest::qWait(50);
            const QString file = QStringLiteral("%1/v321-%2-%3.png").arg(dir, QLatin1String(what), QLatin1String(name));
            QVERIFY2(window.grab().save(file), qPrintable(file));
        };
        {
            B321Dialog dialog([] { return pdcContext(QStringLiteral("pdc")); });
            dialog.setConnected(true);
            dialog.setReferenceTime(kNow);
            feedPdc(dialog);
            dialog.table()->setCurrentCell(rowOf(dialog.table(), QStringLiteral("Catalog")), 0);
            save(dialog, "a-pdc-worked-example");
            storageItem(dialog.offSiteList(), QStringLiteral("PDC-sd"))->setCheckState(Qt::Checked);
            dialog.table()->setCurrentCell(rowOf(dialog.table(), QStringLiteral("Tapir")), 0);
            save(dialog, "b-pdc-storage-marked-off-site");
            storageItem(dialog.offSiteList(), QStringLiteral("PDC-sd"))->setCheckState(Qt::Unchecked);
        }
        {
            BSettings::instance().setVerify321OffSite(QStringLiteral("mixed"), QStringLiteral("TapeLib"), true);
            B321Dialog dialog(mixedContext);
            dialog.setConnected(true);
            dialog.setReferenceTime(kNow);
            feedMixed(dialog);
            dialog.table()->setCurrentCell(rowOf(dialog.table(), QStringLiteral("Srv")), 0);
            save(dialog, "c-copies-on-tape-passes");
            dialog.table()->setCurrentCell(rowOf(dialog.table(), QStringLiteral("Mail")), 0);
            save(dialog, "d-partial-copy");
        }
        {
            BUiScale::instance().setZoomPercent(150);
            B321Dialog dialog([] { return pdcContext(QStringLiteral("pdc")); });
            dialog.setConnected(true);
            dialog.setReferenceTime(kNow);
            feedPdc(dialog);
            dialog.table()->setCurrentCell(0, 0);
            save(dialog, "e-zoom-150");
            BUiScale::instance().resetZoom();
        }
    }
    BUiScale::instance().setApplicationStyleSheet(theme(QStringLiteral("dark")) + BUiScale::themeRules());
}

QTEST_MAIN(B321DialogTest)
#include "b321dialog_test.moc"
