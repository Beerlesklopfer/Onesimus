// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file storagewidget_test.cpp
 * @brief The Storage/Volumes tab (view B) on the real reply of the production
 *        Director, offscreen, without a Director
 *
 * - The table has the columns of view B, no "Max Bytes" and no capacity
 *   figures; sizes in IEC units.
 * - Pool filter from .pools (empty pools included) and a status filter with
 *   every Bareos status.
 * - Refresh asks for "llist volumes" (Command::LlistVolumes) and nothing else;
 *   the tab sends no other command.
 * - Enter on a row opens the details; Tab leaves the table.
 *
 * Settings are redirected by XDG_CONFIG_HOME (set by CTest). TZ=UTC.
 */

#include <QtTest>

#include <QApplication>
#include <QComboBox>
#include <QFile>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidget>
#include <QTimer>

#include "storagewidget.h"
#include "bstatuschip.h"

namespace {

const QStringList kPdcPools = {"Tapir", "Scratch", "Incremental", "Full", "Forgejo",
                               "Differential", "Default", "CLOUD", "BGLAGE"};

const QDateTime kNow(QDate(2026, 11, 19), QTime(12, 0));

QString fixture()
{
    QFile file(QStringLiteral(STORAGE_FIXTURE_DIR "/pdc-list-volumes-api2.json"));
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }
    return QString::fromUtf8(file.readAll());
}

// "llist volumes" with the long columns (values made up after PDC; the
// real reply is test/fixtures/pdc-llist-volumes-api2.json)
const char *kLlist = R"({"result": {"volumes": {
  "cloud": [
    {"mediaid": "17", "volumename": "CLOUD-0017", "poolid": "3", "pool": "CLOUD", "mediatype": "File",
     "firstwritten": "2026-03-02 04:16:20", "lastwritten": "2026-03-09 01:39:04",
     "labeldate": "2026-03-02 04:16:20", "voljobs": "9", "volbytes": "51233279615", "volerrors": "1",
     "volstatus": "Error", "enabled": "1", "recycle": "1", "volretention": "31536000",
     "voluseduration": "0", "maxvoljobs": "0", "maxvolbytes": "53687091200", "recyclecount": "0",
     "recyclepool": "", "storage": "PDC-sd"},
    {"mediaid": "24", "volumename": "CLOUD-0024", "poolid": "3", "pool": "CLOUD", "mediatype": "File",
     "firstwritten": "2026-10-20 22:00:00", "lastwritten": "2026-11-02 01:06:57",
     "labeldate": "2026-10-20 22:00:00", "voljobs": "12", "volbytes": "53686358248", "volerrors": "0",
     "volstatus": "Full", "enabled": "1", "recycle": "1", "volretention": "31536000",
     "voluseduration": "0", "maxvoljobs": "20", "maxvolbytes": "53687091200", "recyclecount": "1",
     "recyclepool": "", "storage": "PDC-sd"}
  ],
  "scratch": []
}}})";

int columnOf(QTableWidget *table, const QString &header)
{
    for (int c = 0; c < table->columnCount(); ++c) {
        if (table->horizontalHeaderItem(c)->text() == header) {
            return c;
        }
    }
    return -1;
}

QString cell(QTableWidget *table, const QString &volume, int column)
{
    for (int r = 0; r < table->rowCount(); ++r) {
        if (table->item(r, StorageWidget::VolumeColumn)->text() == volume) {
            return table->item(r, column)->text();
        }
    }
    return QString();
}

// The status chip: letter (CodeRole), word (text) and tone
QString chip(QTableWidget *table, const QString &volume)
{
    for (int r = 0; r < table->rowCount(); ++r) {
        if (table->item(r, StorageWidget::VolumeColumn)->text() == volume) {
            const QTableWidgetItem *item = table->item(r, StorageWidget::StatusColumn);
            return QStringLiteral("%1 %2 %3").arg(item->data(BStatus::CodeRole).toString(), item->text(),
                                                 BStatus::toneName(static_cast<BStatus::Tone>(
                                                     item->data(BStatus::ToneRole).toInt())));
        }
    }
    return QString();
}

} // namespace

class StorageWidgetTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void columnsAndNoFalseNumbers();
    void pdcRows();
    void poolFilterListsEmptyPools();
    void statusFilter();
    void shortListNotice();
    void longListShowsJobsAndLimit();
    void pdcLlistReply();
    void refreshAsksForLlistOnly();
    void keyboard();
    void clearOnDisconnect();

private:
    StorageWidget *makeWidget();
    QList<BDirector::Command> m_sent;
    QStringList m_sentArgs;
};

void StorageWidgetTest::initTestCase()
{
    // The expected texts use '.', the separator of the C locale
    QLocale::setDefault(QLocale::c());
    QVERIFY2(!fixture().isEmpty(), "fixture missing");
}

StorageWidget *StorageWidgetTest::makeWidget()
{
    auto *w = new StorageWidget(nullptr);
    w->setReferenceTime(kNow);
    w->setKnownPools(kPdcPools);
    connect(w, &StorageWidget::sendCommand, this, [this](BDirector::Command cmd, const QString &args) {
        m_sent.append(cmd);
        m_sentArgs.append(args);
    });
    w->resize(1400, 800);
    return w;
}

void StorageWidgetTest::columnsAndNoFalseNumbers()
{
    QScopedPointer<StorageWidget> w(makeWidget());
    w->processJsonResponse(fixture());
    QTableWidget *table = w->volumeTable();
    QStringList headers;
    for (int c = 0; c < table->columnCount(); ++c) {
        headers << table->horizontalHeaderItem(c)->text();
    }
    QCOMPARE(headers, (QStringList{"Volume", "Pool", "Status", "Used", "Limit", "Jobs",
                                   "Last Written", "Retention", "Reusable From", "Reason"}));
    QVERIFY(!headers.contains(QStringLiteral("Max Bytes")));

    // No label of the widget speaks of a total or available capacity
    const QList<QLabel *> labels = w->findChildren<QLabel *>();
    for (const QLabel *label : labels) {
        QVERIFY2(!label->text().contains(QLatin1String("Available"), Qt::CaseInsensitive)
                     && !label->text().contains(QLatin1String("Verfügbar"))
                     && !label->text().contains(QLatin1String("Gesamt"))
                     && !label->text().contains(QLatin1String("Usage")),
                 qPrintable(label->text()));
    }
}

void StorageWidgetTest::pdcRows()
{
    QScopedPointer<StorageWidget> w(makeWidget());
    w->processJsonResponse(fixture());
    QTableWidget *table = w->volumeTable();
    QCOMPARE(table->rowCount(), 23);

    QCOMPARE(cell(table, "CLOUD-0017", StorageWidget::PoolColumn), QStringLiteral("CLOUD"));
    QCOMPARE(chip(table, "CLOUD-0017"), QStringLiteral("E Error error"));
    QCOMPARE(cell(table, "CLOUD-0017", StorageWidget::UsedColumn), QStringLiteral("47.7 GiB"));
    QCOMPARE(cell(table, "CLOUD-0017", StorageWidget::LimitColumn), QStringLiteral("unknown"));
    QCOMPARE(cell(table, "CLOUD-0017", StorageWidget::ReusableColumn), QStringLiteral("never"));
    QVERIFY(cell(table, "CLOUD-0017", StorageWidget::ReasonColumn).startsWith(QLatin1String("Error:")));
    QCOMPARE(cell(table, "CLOUD-0017", StorageWidget::RetentionColumn), QStringLiteral("365 d"));
    QCOMPARE(cell(table, "CLOUD-0017", StorageWidget::LastWrittenColumn),
             QStringLiteral("2026-03-09 01:39"));

    QCOMPARE(chip(table, "BGLAGE-0019"), QStringLiteral("P Purged waiting"));
    QCOMPARE(cell(table, "BGLAGE-0019", StorageWidget::UsedColumn), QStringLiteral("50 GiB"));
    QCOMPARE(cell(table, "BGLAGE-0019", StorageWidget::ReusableColumn), QStringLiteral("now"));
    QCOMPARE(cell(table, "BGLAGE-0020", StorageWidget::ReusableColumn), QStringLiteral("28 Nov 2026"));
    QCOMPARE(cell(table, "Tapir-0021", StorageWidget::ReusableColumn), QStringLiteral("unknown"));

    // The screen reader hears the row in one sentence
    for (int r = 0; r < table->rowCount(); ++r) {
        if (table->item(r, 0)->text() == QLatin1String("CLOUD-0017")) {
            const QString spoken = table->item(r, 0)->data(Qt::AccessibleTextRole).toString();
            QCOMPARE(spoken, QStringLiteral("CLOUD-0017, Error, 47.7 GiB, last written 9 Mar 2026, "
                                            "reusable from never"));
        }
    }

    // Sorting by size sorts by bytes, not by text
    table->sortItems(StorageWidget::UsedColumn, Qt::AscendingOrder);
    QCOMPARE(table->item(0, 0)->text(), QStringLiteral("BGLAGE-0013"));   // 207 B
}

void StorageWidgetTest::poolFilterListsEmptyPools()
{
    QScopedPointer<StorageWidget> w(makeWidget());
    w->processJsonResponse(fixture());
    auto *pool = w->findChildren<QComboBox *>().at(0);
    QCOMPARE(pool->accessibleName(), QStringLiteral("Pool filter"));
    QCOMPARE(pool->count(), 10);
    QCOMPARE(pool->itemText(0), QStringLiteral("All pools (23)"));
    QVERIFY(pool->findText(QStringLiteral("Scratch (0)")) > 0);
    QVERIFY(pool->findText(QStringLiteral("CLOUD (12)")) > 0);

    pool->setCurrentIndex(pool->findData(QStringLiteral("BGLAGE")));
    QCOMPARE(w->volumeTable()->rowCount(), 6);
    pool->setCurrentIndex(pool->findData(QStringLiteral("Scratch")));
    QCOMPARE(w->volumeTable()->rowCount(), 0);

    // The selection survives a refresh
    pool->setCurrentIndex(pool->findData(QStringLiteral("Default")));
    w->processJsonResponse(fixture());
    QCOMPARE(pool->currentData().toString(), QStringLiteral("Default"));
    QCOMPARE(w->volumeTable()->rowCount(), 3);
}

void StorageWidgetTest::statusFilter()
{
    QScopedPointer<StorageWidget> w(makeWidget());
    w->processJsonResponse(fixture());
    auto *status = w->findChildren<QComboBox *>().at(1);
    QCOMPARE(status->accessibleName(), QStringLiteral("Status filter"));
    QCOMPARE(status->count(), 13);
    QVERIFY(status->findText(QStringLiteral("U Used (0)")) > 0);
    QVERIFY(status->findText(QStringLiteral("O Read-Only (0)")) > 0);

    status->setCurrentIndex(status->findData(QStringLiteral("Error")));
    QCOMPARE(status->currentText(), QStringLiteral("E Error (4)"));
    QCOMPARE(w->volumeTable()->rowCount(), 4);
    status->setCurrentIndex(status->findData(QStringLiteral("Full")));
    QCOMPARE(w->volumeTable()->rowCount(), 12);
    status->setCurrentIndex(status->findData(QStringLiteral("Append")));
    QCOMPARE(w->volumeTable()->rowCount(), 5);

    // Both filters together
    auto *pool = w->findChildren<QComboBox *>().at(0);
    pool->setCurrentIndex(pool->findData(QStringLiteral("CLOUD")));
    QCOMPARE(w->volumeTable()->rowCount(), 1);
}

void StorageWidgetTest::shortListNotice()
{
    QScopedPointer<StorageWidget> w(makeWidget());
    w->show();
    w->processJsonResponse(fixture());
    bool noticeShown = false;
    for (const QLabel *label : w->findChildren<QLabel *>()) {
        noticeShown = noticeShown
            || (label->isVisible() && label->text().contains(QLatin1String("short volume list")));
    }
    QVERIFY(noticeShown);
    QVERIFY(w->volumeTable()->isColumnHidden(StorageWidget::JobsColumn));
}

void StorageWidgetTest::longListShowsJobsAndLimit()
{
    QScopedPointer<StorageWidget> w(makeWidget());
    w->show();
    w->processJsonResponse(QString::fromUtf8(kLlist));
    QTableWidget *table = w->volumeTable();
    QCOMPARE(table->rowCount(), 2);
    QVERIFY(!table->isColumnHidden(StorageWidget::JobsColumn));
    QCOMPARE(cell(table, "CLOUD-0017", StorageWidget::LimitColumn), QStringLiteral("50 GiB"));
    QCOMPARE(cell(table, "CLOUD-0017", StorageWidget::JobsColumn), QStringLiteral("9"));
    QCOMPARE(cell(table, "CLOUD-0024", StorageWidget::JobsColumn), QStringLiteral("12 of 20"));
    QCOMPARE(cell(table, "CLOUD-0024", StorageWidget::ReusableColumn), QStringLiteral("2 Nov 2027"));
    for (const QLabel *label : w->findChildren<QLabel *>()) {
        QVERIFY(!(label->isVisible() && label->text().contains(QLatin1String("short volume list"))));
    }
    QCOMPARE(columnOf(table, QStringLiteral("Jobs")), int(StorageWidget::JobsColumn));
}

void StorageWidgetTest::pdcLlistReply()
{
    // The real "llist volumes" reply of PDC (grouped by pool)
    QFile file(QStringLiteral(STORAGE_FIXTURE_DIR "/pdc-llist-volumes-api2.json"));
    QVERIFY(file.open(QIODevice::ReadOnly));
    QScopedPointer<StorageWidget> w(makeWidget());
    w->show();
    w->processJsonResponse(QString::fromUtf8(file.readAll()));
    QTableWidget *table = w->volumeTable();
    QCOMPARE(table->rowCount(), 23);
    QVERIFY(!table->isColumnHidden(StorageWidget::JobsColumn));
    for (const QLabel *label : w->findChildren<QLabel *>()) {
        QVERIFY(!(label->isVisible() && label->text().contains(QLatin1String("short volume list"))));
    }
    QCOMPARE(cell(table, "CLOUD-0017", StorageWidget::JobsColumn), QStringLiteral("16"));
    QCOMPARE(cell(table, "CLOUD-0017", StorageWidget::LimitColumn), QStringLiteral("50 GiB"));
    QCOMPARE(cell(table, "BGLAGE-0013", StorageWidget::JobsColumn), QStringLiteral("0"));
    QCOMPARE(cell(table, "Tapir-0021", StorageWidget::ReusableColumn), QStringLiteral("never"));
    QVERIFY(cell(table, "Tapir-0021", StorageWidget::ReasonColumn)
                .contains(QLatin1String("until it is full (50 GiB)")));
    auto *pool = w->findChildren<QComboBox *>().at(0);
    QCOMPARE(pool->count(), 10);
    QVERIFY(pool->findText(QStringLiteral("Incremental (0)")) > 0);
}

void StorageWidgetTest::refreshAsksForLlistOnly()
{
    m_sent.clear();
    m_sentArgs.clear();
    QScopedPointer<StorageWidget> w(makeWidget());
    w->processJsonResponse(fixture());
    // Filters, sorting and details send nothing
    auto *status = w->findChildren<QComboBox *>().at(1);
    status->setCurrentIndex(2);
    w->volumeTable()->sortItems(StorageWidget::ReusableColumn);
    QVERIFY(m_sent.isEmpty());

    w->setConnectionState(true);
    w->triggerRefresh();
    QCOMPARE(m_sent.size(), 1);
    QCOMPARE(m_sent.first(), BDirector::Command::LlistVolumes);
    QVERIFY(m_sentArgs.first().isEmpty());
}

void StorageWidgetTest::keyboard()
{
    QScopedPointer<StorageWidget> w(makeWidget());
    w->show();
    w->processJsonResponse(fixture());
    QTableWidget *table = w->volumeTable();
    QVERIFY(!table->tabKeyNavigation());
    table->setFocus();
    table->setCurrentCell(0, 0);
    const QString volume = table->item(0, 0)->text();

    QString dialogText;
    QTimer::singleShot(0, this, [&dialogText]() {
        if (auto *box = qobject_cast<QMessageBox *>(QApplication::activeModalWidget())) {
            dialogText = box->text();
            box->accept();
        }
    });
    QTest::keyClick(table, Qt::Key_Return);
    QVERIFY2(dialogText.contains(volume), qPrintable(dialogText));
    QVERIFY(dialogText.contains(QLatin1String("Reusable from")));
}

void StorageWidgetTest::clearOnDisconnect()
{
    QScopedPointer<StorageWidget> w(makeWidget());
    w->processJsonResponse(fixture());
    w->setConnectionState(false);
    QCOMPARE(w->volumeTable()->rowCount(), 0);
    auto *pool = w->findChildren<QComboBox *>().at(0);
    QCOMPARE(pool->itemText(0), QStringLiteral("All pools (0)"));
}

QTEST_MAIN(StorageWidgetTest)
#include "storagewidget_test.moc"
