// The Job Definitions tab with a fake Director (BFakeConfigClient): the save
// flow (read, check, review with diff and affected jobs, write) and every
// status, the raw text changed only where edited, delete refused while
// referenced, create and copy, and read-only with nothing sent on BNET and
// without the ACLs. No Director, no network.

#include <QComboBox>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSettings>
#include <QSignalSpy>
#include <QTableView>
#include <QTableWidget>
#include <QTemporaryDir>
#include <QtTest>

#include "bfakeconfigclient.h"
#include "jobs/bjobdefinitionswidget.h"

namespace {

using Kind = BJobDefinitions::Kind;

// The texts as they stand in their files on the Director; CRLF on purpose
const QString kNightly = QStringLiteral(
    "Job {\r\n"
    "  Name = \"nightly\"\r\n"
    "  # backs up the domain controller\r\n"
    "  JobDefs = \"DefaultJob\"\r\n"
    "  Client = pdc-fd   # since 2024\r\n"
    "  Unknown Directive Of A Newer Director = yes\r\n"
    "  Run Script {\r\n"
    "    Command = \"/bin/true\"\r\n"
    "    Runs When = Before\r\n"
    "  }\r\n"
    "}\r\n");
const QString kDefaultJob = QStringLiteral(
    "JobDefs {\n"
    "  Name = \"DefaultJob\"\n"
    "  Type = Backup\n"
    "  Level = Incremental\n"
    "  FileSet = \"LinuxAll\"\n"
    "  Schedule = \"WeeklyCycle\"\n"
    "  Pool = Incremental\n"
    "  Priority = 10\n"
    "}\n");
const QString kSpare = QStringLiteral("Job {\n  Name = \"spare\"\n  JobDefs = \"DefaultJob\"\n}\n");

QMap<QString, QJsonObject> objects(const char *json)
{
    QMap<QString, QJsonObject> out;
    const QJsonObject root = QJsonDocument::fromJson(QByteArray(json)).object();
    for (auto it = root.constBegin(); it != root.constEnd(); ++it)
        out.insert(it.key(), it.value().toObject());
    return out;
}

const char *kJobsJson = R"({
    "nightly": {"name": "nightly", "jobdefs": "DefaultJob", "client": "pdc-fd"},
    "spare":   {"name": "spare", "jobdefs": "DefaultJob"}
})";
const char *kJobDefsJson = R"({
    "DefaultJob": {"name": "DefaultJob", "type": "Backup", "level": "Incremental", "fileset": "LinuxAll",
                   "schedule": "WeeklyCycle", "pool": "Incremental", "priority": 10}
})";

/// The dialogs of BResourcePersistence, answered by the test
class FakeUi : public BResourceSaveUi
{
public:
    explicit FakeUi(BFakeConfigClient *client) : m_client(client) {}

    bool confirmAnswer = true;
    Answer reportAnswer = Close;
    QList<BResourceReview> reviews;
    QList<BResourceReport> reports;
    int writesAtConfirm = -1;

    bool confirm(const BResourceReview &review) override
    {
        reviews.append(review);
        writesAtConfirm = int(m_client->writes().size());
        return confirmAnswer;
    }
    Answer report(const BResourceReport &report) override
    {
        reports.append(report);
        return reportAnswer;
    }

private:
    BFakeConfigClient *m_client;
};

/// The tab with a fake Director that knows the three resources
struct Tab
{
    BFakeConfigClient client;
    FakeUi ui{&client};
    BJobDefinitionsWidget widget;
    QList<BResourceReport> finished;
    int gateCalls = 0;

    explicit Tab(bool crud = true)
    {
        client.caps = BConfig::Capabilities();
        if (crud) {
            client.allow(BConfig::ResourceType::Job);
            client.allow(BConfig::ResourceType::JobDefs);
        }
        client.files.insert(QStringLiteral("nightly"), kNightly);
        client.files.insert(QStringLiteral("DefaultJob"), kDefaultJob);
        client.files.insert(QStringLiteral("spare"), kSpare);
        client.refs.insert(QStringLiteral("DefaultJob"),
                           {BConfig::ResourceRef{BConfig::ResourceType::Job, QStringLiteral("nightly")},
                            BConfig::ResourceRef{BConfig::ResourceType::Job, QStringLiteral("spare")}});
        widget.setConfigClient(&client);
        widget.setSaveUi(&ui);
        widget.setDiscardUi([](const QString &) { return true; });
        widget.setNameSource([]() {
            QMap<QString, QStringList> names;
            names.insert(QStringLiteral("Client"), {QStringLiteral("pdc-fd"), QStringLiteral("nas-fd")});
            names.insert(QStringLiteral("Pool"), {QStringLiteral("Incremental"), QStringLiteral("Full")});
            return names;
        });
        // As the capability notice would: available where the session lists it
        widget.setGate([this](QWidget *, BCapabilities::Function function) {
            ++gateCalls;
            const BCapabilities::Entry *entry = BCapabilities::entry(function);
            if (entry->bareos)
                return true;
            return client.capabilities().supports(entry->liveType, entry->liveOperation);
        });
        QObject::connect(widget.persistence(), &BResourcePersistence::finished,
                         [this](const BResourceReport &report) { finished.append(report); });
        widget.setResources(objects(kJobsJson), objects(kJobDefsJson));
    }

    bool open(Kind kind, const QString &name)
    {
        if (!widget.selectResource(kind, name))
            return false;
        if (!client.capabilities().supports(kind == Kind::Job ? BConfig::ResourceType::Job
                                                              : BConfig::ResourceType::JobDefs,
                                            BConfig::Operation::ReadRaw))
            return true;
        return QTest::qWaitFor([this]() { return !widget.isLoadingText(); });
    }

    bool waitFinished(int count = 1)
    {
        return QTest::qWaitFor([this, count]() { return finished.size() >= count; });
    }

    QList<BConfig::Request> requestsOf(BConfig::Request::Kind kind) const
    {
        QList<BConfig::Request> out;
        for (const BConfig::Request &request : client.requests) {
            if (request.kind == kind)
                out.append(request);
        }
        return out;
    }
};

} // namespace

class BJobDefinitionsWidgetTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void tableShowsInheritedValues();
    void saveWritesOnlyTheEditedDirectives();
    void removingADirectiveInherits();
    void jobDefsReviewNamesTheJobs();
    void cancelledReviewWritesNothing();
    void conflictIsNeverOverwritten();
    void statusesAreReported_data();
    void statusesAreReported();
    void deleteRefusedWhileReferenced();
    void deleteAfterConfirm();
    void createUsesCreateResource();
    void copyIsCreatedDisabled();
    void textEditingSendsTheText();
    void bnetSendsNothing();
    void withoutTheAclReadOnly();

private:
    QTemporaryDir m_config;
};

void BJobDefinitionsWidgetTest::initTestCase()
{
    // The settings of the test, not those of the user: before BSettings is
    // used for the first time
    QVERIFY(m_config.isValid());
    // The icons are in the application's resources, not in the test
    QLoggingCategory::setFilterRules(QStringLiteral("qt.svg.warning=false"));
    qputenv("XDG_CONFIG_HOME", m_config.path().toUtf8());
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, m_config.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_config.path());
    // Windows keeps NativeFormat in the registry, where setPath does not reach
    QSettings::setDefaultFormat(QSettings::IniFormat);
    const QSettings probe(QSettings::defaultFormat(), QSettings::UserScope,
                         QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
    QVERIFY2(probe.fileName().startsWith(m_config.path()), qPrintable(probe.fileName()));
}

void BJobDefinitionsWidgetTest::tableShowsInheritedValues()
{
    Tab tab;
    BJobDefinitionsTableModel *model = tab.widget.tableModel();
    QCOMPARE(model->rowCount(), 3);
    const int row = model->rowOf(Kind::Job, QStringLiteral("nightly"));
    QVERIFY(row >= 0);
    const QModelIndex client = model->index(row, BJobDefinitionsTableModel::ColClient);
    const QModelIndex pool = model->index(row, BJobDefinitionsTableModel::ColPool);
    QCOMPARE(client.data().toString(), QStringLiteral("pdc-fd"));
    QVERIFY(!client.data(Qt::FontRole).value<QFont>().italic());
    QCOMPARE(pool.data().toString(), QStringLiteral("Incremental"));
    QVERIFY(pool.data(Qt::FontRole).value<QFont>().italic());
    QVERIFY(pool.data(Qt::ToolTipRole).toString().contains(QStringLiteral("DefaultJob")));
    QCOMPARE(model->index(row, BJobDefinitionsTableModel::ColEnabled).data().toString(), QStringLiteral("Enabled"));
    QCOMPARE(model->index(row, BJobDefinitionsTableModel::ColPriority).data(Qt::UserRole).toInt(), 10);

    // The detail: read the raw text, then "set here" follows the text
    QVERIFY(tab.open(Kind::Job, QStringLiteral("nightly")));
    QVERIFY(tab.widget.isEditable());
    QCOMPARE(tab.widget.fieldValue(QStringLiteral("client")), QStringLiteral("pdc-fd"));
    QCOMPARE(tab.widget.fieldValue(QStringLiteral("pool")), QString());     // inherited, empty here
    auto *poolCombo = qobject_cast<QComboBox *>(tab.widget.fieldEditor(QStringLiteral("pool")));
    QVERIFY(poolCombo);
    QCOMPARE(poolCombo->lineEdit()->placeholderText(), QStringLiteral("Incremental"));
    QVERIFY(poolCombo->findText(QStringLiteral("Full")) >= 0);           // pick list of the loaded names
    QVERIFY(tab.widget.textView()->toPlainText().contains(QStringLiteral("Unknown Directive")));
    QCOMPARE(tab.requestsOf(BConfig::Request::Read).size(), 1);
}

void BJobDefinitionsWidgetTest::saveWritesOnlyTheEditedDirectives()
{
    Tab tab;
    QSignalSpy changed(&tab.widget, &BJobDefinitionsWidget::definitionsChanged);
    QVERIFY(tab.open(Kind::Job, QStringLiteral("nightly")));
    QVERIFY(!tab.widget.isModified());
    QVERIFY(tab.widget.setFieldValue(QStringLiteral("client"), QStringLiteral("nas-fd")));
    QVERIFY(tab.widget.setFieldValue(QStringLiteral("priority"), QStringLiteral("7")));
    QVERIFY(tab.widget.isModified());
    QVERIFY(tab.widget.saveButton()->isEnabled());
    tab.widget.save();
    QVERIFY(tab.waitFinished());

    const QList<BConfig::Request> updates = tab.requestsOf(BConfig::Request::Update);
    QCOMPARE(updates.size(), 2);
    QVERIFY(updates[0].validateOnly);           // checked first
    QVERIFY(!updates[1].validateOnly);
    QCOMPARE(tab.ui.writesAtConfirm, 0);        // the review came before any write
    QCOMPARE(updates[0].expectedSha256, BConfig::sha256Hex(kNightly));
    QCOMPARE(updates[1].text, updates[0].text);

    QString expected = kNightly;
    expected.replace(QStringLiteral("  Client = pdc-fd   # since 2024\r\n"),
                     QStringLiteral("  Client = nas-fd   # since 2024\r\n"));
    expected.replace(QStringLiteral("  }\r\n}\r\n"), QStringLiteral("  }\r\n  Priority = 7\r\n}\r\n"));
    QCOMPARE(updates[1].text.toUtf8(), expected.toUtf8());
    QCOMPARE(tab.client.files.value(QStringLiteral("nightly")), expected);

    QCOMPARE(tab.ui.reviews.size(), 1);
    const BResourceReview &review = tab.ui.reviews.first();
    QCOMPARE(review.type, BConfig::ResourceType::Job);
    QCOMPARE(review.oldText, kNightly);
    QCOMPARE(review.newText, expected);
    QCOMPARE(review.changes.size(), 2);
    QVERIFY(review.changes.first().contains(QStringLiteral("nas-fd")));
    QCOMPARE(review.affected, QStringList({QStringLiteral("Job nightly")}));

    QCOMPARE(tab.finished.last().kind, BResourceReport::Written);
    QCOMPARE(changed.size(), 1);                // definitions and schedules are read again
    // The form follows the new text
    QTRY_COMPARE(tab.widget.fieldValue(QStringLiteral("priority")), QStringLiteral("7"));
    QTRY_VERIFY(!tab.widget.isModified());
}

void BJobDefinitionsWidgetTest::removingADirectiveInherits()
{
    Tab tab;
    QVERIFY(tab.open(Kind::Job, QStringLiteral("nightly")));
    QVERIFY(tab.widget.setFieldValue(QStringLiteral("client"), QString()));
    tab.widget.save();
    QVERIFY(tab.waitFinished());
    const QString written = tab.client.files.value(QStringLiteral("nightly"));
    QVERIFY(!written.contains(QStringLiteral("Client")));
    QString expected = kNightly;
    expected.remove(QStringLiteral("  Client = pdc-fd   # since 2024\r\n"));
    QCOMPARE(written, expected);
    QVERIFY(tab.ui.reviews.first().changes.first().contains(QStringLiteral("inherited")));
}

void BJobDefinitionsWidgetTest::jobDefsReviewNamesTheJobs()
{
    Tab tab;
    QVERIFY(tab.open(Kind::JobDefs, QStringLiteral("DefaultJob")));
    QVERIFY(tab.widget.setFieldValue(QStringLiteral("pool"), QStringLiteral("Full")));
    tab.widget.save();
    QVERIFY(tab.waitFinished());
    QCOMPARE(tab.ui.reviews.size(), 1);
    const BResourceReview &review = tab.ui.reviews.first();
    QCOMPARE(review.type, BConfig::ResourceType::JobDefs);
    QCOMPARE(review.affected, QStringList({QStringLiteral("Job nightly"), QStringLiteral("Job spare")}));
    QVERIFY(review.note.contains(QStringLiteral("2")));
    QVERIFY(tab.client.files.value(QStringLiteral("DefaultJob")).contains(QStringLiteral("  Pool = Full\n")));
}

void BJobDefinitionsWidgetTest::cancelledReviewWritesNothing()
{
    Tab tab;
    tab.ui.confirmAnswer = false;
    QVERIFY(tab.open(Kind::Job, QStringLiteral("nightly")));
    tab.widget.setFieldValue(QStringLiteral("client"), QStringLiteral("nas-fd"));
    tab.widget.save();
    QVERIFY(tab.waitFinished());
    QCOMPARE(tab.finished.last().kind, BResourceReport::Cancelled);
    QVERIFY(tab.client.writes().isEmpty());
    QCOMPARE(tab.client.files.value(QStringLiteral("nightly")), kNightly);
    QVERIFY(tab.widget.isModified());           // the edit is still there
}

void BJobDefinitionsWidgetTest::conflictIsNeverOverwritten()
{
    Tab tab;
    tab.ui.reportAnswer = BResourceSaveUi::Reload;
    QVERIFY(tab.open(Kind::Job, QStringLiteral("nightly")));
    tab.widget.setFieldValue(QStringLiteral("client"), QStringLiteral("nas-fd"));
    // Someone else changes the job on the Director in between
    QString theirs = kNightly;
    theirs.replace(QStringLiteral("# backs up"), QStringLiteral("# still backs up"));
    tab.client.files.insert(QStringLiteral("nightly"), theirs);
    tab.widget.save();
    QVERIFY(tab.waitFinished());
    const BResourceReport report = tab.finished.last();
    QCOMPARE(report.kind, BResourceReport::Conflict);
    QVERIFY(report.validatedOnly);
    QCOMPARE(report.currentText, theirs);
    QVERIFY(tab.client.writes().isEmpty());
    QVERIFY(tab.ui.reviews.isEmpty());
    QCOMPARE(tab.client.files.value(QStringLiteral("nightly")), theirs);

    // Reload: the newer text is read, the edit stays and goes into it
    QTRY_COMPARE(tab.requestsOf(BConfig::Request::Read).size(), 2);
    QTRY_VERIFY(tab.widget.textView()->toPlainText().contains(QStringLiteral("still backs up")));
    QCOMPARE(tab.widget.fieldValue(QStringLiteral("client")), QStringLiteral("nas-fd"));
    tab.ui.reportAnswer = BResourceSaveUi::Close;
    tab.widget.save();
    QVERIFY(tab.waitFinished(2));
    QCOMPARE(tab.finished.last().kind, BResourceReport::Written);
    const QString written = tab.client.files.value(QStringLiteral("nightly"));
    QVERIFY(written.contains(QStringLiteral("still backs up")));
    QVERIFY(written.contains(QStringLiteral("Client = nas-fd")));
}

void BJobDefinitionsWidgetTest::statusesAreReported_data()
{
    QTest::addColumn<int>("status");
    QTest::addColumn<bool>("onWrite");
    QTest::addColumn<int>("kind");
    QTest::addColumn<QString>("words");

    QTest::newRow("invalid") << int(BConfig::Status::Invalid) << false << int(BResourceReport::Invalid)
                             << QStringLiteral("rejected");
    QTest::newRow("denied") << int(BConfig::Status::Denied) << false << int(BResourceReport::Denied)
                            << QStringLiteral("Job ACL");
    QTest::newRow("not found") << int(BConfig::Status::NotFound) << false << int(BResourceReport::NotFound)
                               << QStringLiteral("no job of this name");
    QTest::newRow("exists") << int(BConfig::Status::Exists) << false << int(BResourceReport::Exists)
                            << QStringLiteral("already has");
    QTest::newRow("reload failed") << int(BConfig::Status::ReloadFailed) << true
                                   << int(BResourceReport::ReloadFailed) << QStringLiteral("put the old file back");
    QTest::newRow("permission denied") << int(BConfig::Status::RpcError) << false << int(BResourceReport::Failed)
                                       << QStringLiteral("command ACL \"configure\"");
    QTest::newRow("not available") << int(BConfig::Status::NotAvailable) << false
                                   << int(BResourceReport::NotAvailable) << QStringLiteral("Nothing was sent");
    QTest::newRow("no status") << int(BConfig::Status::Unspecified) << true << int(BResourceReport::Failed)
                               << QStringLiteral("no status");
}

void BJobDefinitionsWidgetTest::statusesAreReported()
{
    QFETCH(int, status);
    QFETCH(bool, onWrite);
    QFETCH(int, kind);
    QFETCH(QString, words);

    Tab tab;
    tab.client.intercept = [&](const BConfig::Request &request) -> std::optional<BConfig::Reply> {
        if (request.kind != BConfig::Request::Update || request.validateOnly == onWrite)
            return std::nullopt;
        BConfig::Reply reply = BFakeConfigClient::withText(BConfig::Status(status), request.name, QString());
        if (status == int(BConfig::Status::RpcError)) {
            reply.rpcCode = 7;
            reply.rpcMessage = QStringLiteral("PERMISSION_DENIED: command \"configure\" not allowed");
        }
        if (status == int(BConfig::Status::Invalid)) {
            reply.problem = BConfig::Problem{QStringLiteral("/etc/bareos/bareos-dir.d/job/nightly.conf"), 5, 12,
                                             QStringLiteral("Could not find config resource \"nas-fd\"")};
        }
        return reply;
    };
    QVERIFY(tab.open(Kind::Job, QStringLiteral("nightly")));
    tab.widget.setFieldValue(QStringLiteral("client"), QStringLiteral("nas-fd"));
    tab.widget.save();
    QVERIFY(tab.waitFinished());
    const BResourceReport report = tab.finished.last();
    QCOMPARE(int(report.kind), kind);
    QVERIFY2(report.text.contains(words), qPrintable(report.text));
    QCOMPARE(tab.ui.reports.size(), 1);                     // the user is told
    QCOMPARE(report.validatedOnly, !onWrite);
    QCOMPARE(tab.ui.reviews.size(), onWrite ? 1 : 0);       // a failed check shows no review
    if (status == int(BConfig::Status::Invalid))
        QCOMPARE(report.problem->line, 5);
    // Only an answer to the write itself follows a write
    QCOMPARE(tab.client.writes().size(), onWrite ? 1 : 0);
    QVERIFY(tab.widget.isModified());                       // nothing lost
}

void BJobDefinitionsWidgetTest::deleteRefusedWhileReferenced()
{
    Tab tab;
    QVERIFY(tab.open(Kind::JobDefs, QStringLiteral("DefaultJob")));
    QVERIFY(tab.widget.deleteButton()->isEnabled());
    tab.widget.deleteCurrent();
    QVERIFY(tab.waitFinished());
    const BResourceReport report = tab.finished.last();
    QCOMPARE(report.kind, BResourceReport::Referenced);
    QCOMPARE(report.referencedBy, QStringList({QStringLiteral("Job nightly"), QStringLiteral("Job spare")}));
    QVERIFY(report.validatedOnly);
    QVERIFY(tab.ui.reviews.isEmpty());
    QVERIFY(tab.client.writes().isEmpty());
    QVERIFY(tab.client.files.contains(QStringLiteral("DefaultJob")));
    // The request carried the SHA-256 of the text of now
    const QList<BConfig::Request> deletes = tab.requestsOf(BConfig::Request::Delete);
    QCOMPARE(deletes.size(), 1);
    QCOMPARE(deletes.first().expectedSha256, BConfig::sha256Hex(kDefaultJob));
}

void BJobDefinitionsWidgetTest::deleteAfterConfirm()
{
    Tab tab;
    QSignalSpy changed(&tab.widget, &BJobDefinitionsWidget::definitionsChanged);
    QVERIFY(tab.open(Kind::Job, QStringLiteral("spare")));
    tab.widget.deleteCurrent();
    QVERIFY(tab.waitFinished());
    QCOMPARE(tab.finished.last().kind, BResourceReport::Written);
    QCOMPARE(tab.ui.reviews.size(), 1);
    QCOMPARE(tab.ui.reviews.first().operation, BResourceReview::Delete);
    QCOMPARE(tab.ui.reviews.first().oldText, kSpare);
    QCOMPARE(tab.ui.writesAtConfirm, 0);
    QVERIFY(!tab.client.files.contains(QStringLiteral("spare")));
    QCOMPARE(changed.size(), 1);
    QVERIFY(!tab.widget.hasSelection());
}

void BJobDefinitionsWidgetTest::createUsesCreateResource()
{
    Tab tab;
    QSignalSpy wizard(&tab.widget, &BJobDefinitionsWidget::wizardRequested);
    tab.widget.setNewResourceUi([](Kind, const QString &copyOf) -> std::optional<BJobDefinitionsWidget::NewResource> {
        if (!copyOf.isEmpty())
            return std::nullopt;
        BJobDefinitionsWidget::NewResource resource;
        resource.name = QStringLiteral("mail");
        resource.directives = {BResourceText::Change::set(QStringLiteral("JobDefs"), QStringLiteral("DefaultJob")),
                               BResourceText::Change::set(QStringLiteral("Client"), QStringLiteral("nas-fd")),
                               BResourceText::Change::set(QStringLiteral("Enabled"), QStringLiteral("no"))};
        return resource;
    });
    tab.widget.newResource(Kind::Job);
    QVERIFY(tab.waitFinished());
    QCOMPARE(wizard.size(), 0);
    const QList<BConfig::Request> creates = tab.requestsOf(BConfig::Request::Create);
    QCOMPARE(creates.size(), 2);
    QVERIFY(creates[0].validateOnly);
    QCOMPARE(creates[0].type, BConfig::ResourceType::Job);
    QCOMPARE(creates[1].text, QStringLiteral("Job {\n  Name = \"mail\"\n  JobDefs = DefaultJob\n  Client = nas-fd\n"
                                             "  Enabled = no\n}\n"));
    QCOMPARE(tab.finished.last().kind, BResourceReport::Written);
    QVERIFY(tab.client.files.contains(QStringLiteral("mail")));

    // A name that is taken: EXISTS at the check, nothing written
    tab.widget.newResource(Kind::Job);
    QVERIFY(tab.waitFinished(2));
    QCOMPARE(tab.finished.last().kind, BResourceReport::Exists);
    QCOMPARE(tab.client.writes().size(), 1);
}

void BJobDefinitionsWidgetTest::copyIsCreatedDisabled()
{
    Tab tab;
    tab.widget.setNewResourceUi([](Kind, const QString &copyOf) -> std::optional<BJobDefinitionsWidget::NewResource> {
        BJobDefinitionsWidget::NewResource resource;
        resource.name = copyOf + QStringLiteral("-copy");
        resource.directives = {BResourceText::Change::set(QStringLiteral("Enabled"), QStringLiteral("no"))};
        return resource;
    });
    QVERIFY(tab.open(Kind::Job, QStringLiteral("nightly")));
    QVERIFY(tab.widget.copyButton()->isEnabled());
    tab.widget.copyCurrent();
    QVERIFY(tab.waitFinished());
    QCOMPARE(tab.finished.last().kind, BResourceReport::Written);
    QString expected = kNightly;
    expected.replace(QStringLiteral("  Name = \"nightly\"\r\n"), QStringLiteral("  Name = \"nightly-copy\"\r\n"));
    expected.replace(QStringLiteral("  }\r\n}\r\n"), QStringLiteral("  }\r\n  Enabled = no\r\n}\r\n"));
    QCOMPARE(tab.client.files.value(QStringLiteral("nightly-copy")).toUtf8(), expected.toUtf8());
    QCOMPARE(tab.client.files.value(QStringLiteral("nightly")), kNightly);    // the original untouched
}

void BJobDefinitionsWidgetTest::textEditingSendsTheText()
{
    Tab tab;
    QVERIFY(tab.open(Kind::Job, QStringLiteral("nightly")));
    tab.widget.setTextEditing(true);
    QVERIFY(!tab.widget.fieldEditor(QStringLiteral("client"))->isEnabled());   // the form rests
    QString edited = tab.widget.textView()->toPlainText();
    edited.replace(QStringLiteral("    Runs When = Before\n"), QStringLiteral("    Runs When = After\n"));
    tab.widget.setEditedText(edited);
    QVERIFY(tab.widget.isModified());
    tab.widget.save();
    QVERIFY(tab.waitFinished());
    QString expected = kNightly;
    expected.replace(QStringLiteral("Runs When = Before"), QStringLiteral("Runs When = After"));
    QCOMPARE(tab.client.files.value(QStringLiteral("nightly")).toUtf8(), expected.toUtf8());   // CRLF kept
}

void BJobDefinitionsWidgetTest::bnetSendsNothing()
{
    // BNET (or plain Bareos): no capabilities are known; the tab reads what
    // "show" printed and sends nothing to the configuration methods
    Tab tab(false);
    QSignalSpy wizard(&tab.widget, &BJobDefinitionsWidget::wizardRequested);
    QSignalSpy status(&tab.widget, &BJobDefinitionsWidget::statusMessageChanged);
    QVERIFY(tab.open(Kind::Job, QStringLiteral("nightly")));
    QVERIFY(!tab.widget.isEditable());
    QVERIFY(tab.widget.readOnlyReason().contains(QStringLiteral("gRPC")));
    QVERIFY(!tab.widget.fieldEditor(QStringLiteral("client"))->isEnabled());
    QCOMPARE(tab.widget.fieldValue(QStringLiteral("client")), QStringLiteral("pdc-fd"));   // from "show"
    QVERIFY(tab.widget.directivesView()->rowCount() > 0);
    QVERIFY(!tab.widget.copyButton()->isEnabled());

    tab.widget.save();
    tab.widget.deleteCurrent();
    tab.widget.copyCurrent();
    tab.widget.newResource(Kind::JobDefs);
    QTest::qWait(20);
    QVERIFY(tab.client.requests.isEmpty());
    QVERIFY(tab.finished.isEmpty());
    QVERIFY(tab.ui.reviews.isEmpty());
    QVERIFY(tab.gateCalls >= 2);                // delete and create asked the gate (the notice)
    QCOMPARE(wizard.size(), 1);                 // a new JobDefs goes through the wizard
    QVERIFY(status.size() >= 2);
}

void BJobDefinitionsWidgetTest::withoutTheAclReadOnly()
{
    // gRPC, but the console may only read Job resources and nothing of JobDefs
    Tab tab(false);
    tab.client.allow(BConfig::ResourceType::Job, {int(BConfig::Operation::ReadRaw)});
    QVERIFY(tab.open(Kind::Job, QStringLiteral("nightly")));
    QCOMPARE(tab.requestsOf(BConfig::Request::Read).size(), 1);
    QVERIFY(!tab.widget.isEditable());
    QVERIFY(tab.widget.readOnlyReason().contains(QStringLiteral("ACL")));
    QVERIFY(tab.widget.textView()->toPlainText().contains(QStringLiteral("Unknown Directive")));
    tab.widget.deleteCurrent();
    tab.widget.setFieldValue(QStringLiteral("client"), QStringLiteral("nas-fd"));
    tab.widget.save();
    QTest::qWait(20);
    QCOMPARE(tab.client.requests.size(), 1);    // only the read
    QVERIFY(tab.client.writes().isEmpty());

    // No JobDefs rights: not even read
    QVERIFY(tab.open(Kind::JobDefs, QStringLiteral("DefaultJob")));
    QCOMPARE(tab.client.requests.size(), 1);
}

QTEST_MAIN(BJobDefinitionsWidgetTest)
#include "bjobdefinitionswidget_test.moc"
