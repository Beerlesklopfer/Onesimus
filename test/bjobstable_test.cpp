/**
 * @file bjobstable_test.cpp
 * @brief The jobs table: header state, sorting by value, no row colour by level
 *
 * - The header (order, widths, hidden columns, sort column and order) goes
 *   to BSettings (Widgets/Jobs/header_state) and comes back the same.
 * - Widths saved at one interface zoom are scaled when restored at another.
 * - The old per-column BColumnConfiguration group "Columns" is taken over
 *   once and removed: one source from then on.
 * - Bytes, files, durations, start and end times and job ids sort by value,
 *   not by their formatted text.
 * - No cell is coloured by the backup level; status and level are chips
 *   (BStatusDelegate roles), the accessible text is the full word.
 *
 * The table is set up as BJsonJobView does it (its proxy, sort role, header
 * class, BHeaderState); BJsonJobView itself is not built here, its
 * translation unit pulls in the dialogs and wizards of the job actions.
 * Widgets on the offscreen platform; settings in a temporary directory.
 */

#include <QtTest>
#include <QApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>
#include <QTableView>
#include <QTemporaryDir>

#include "bcheckableheaderview.h"
#include "bheaderstate.h"
#include "bstatuschip.h"
#include "buiscale.h"
#include "config/bsettings.h"
#include "jobs/bjobmodels.h"

namespace {

QJsonObject job(const QString &id, const QString &name, const QString &start, const QString &duration,
                const QString &level, const QString &files, const QString &bytes, const QString &status)
{
    return QJsonObject{
        {"jobid", id}, {"name", name}, {"client", "pdc-fd"}, {"starttime", start},
        {"duration", duration}, {"type", "B"}, {"level", level}, {"jobfiles", files},
        {"jobbytes", bytes}, {"jobstatus", status},
    };
}

// Mixed statuses and levels. The formatted text of every value column
// sorts differently from the value.
QJsonArray jobs()
{
    return QJsonArray{
        job("1", "BackupHome", "2026-11-10 10:00:00", "00:00:09", "I", "9", "900", "T"),
        job("2", "BackupDB", "2026-11-09 23:00:00", "100:00:00", "I", "100000", "10737418240", "E"),
        job("3", "BackupHome", "2026-11-10 09:00:00", "00:10:00", "F", "20", "2097152", "W"),
        job("4", "Catalog", "2026-11-01 08:00:00", "00:02:00", "D", "1000", "102400", "f"),
        job("5", "BackupMail", "2026-12-24 07:00:00", "26:00:00", "F", "3", "1536", "A"),
        job("6", "BackupLaptop", "2025-12-31 12:00:00", "00:00:30", "I", "7", "512000", "R"),
        job("10", "RestoreFiles", "", "", "F", "", "", "C"),
    };
}

/// The jobs table as BJsonJobView sets it up
struct JobsTable {
    BJobsModel model{nullptr};
    BJobsFilterModel filter;
    QTableView view;
    BCheckableHeaderView *header;

    JobsTable()
        : header(new BCheckableHeaderView(Qt::Horizontal, &view))
    {
        filter.setSourceModel(&model);
        filter.setSortRole(BJobsModel::SortRole);
        view.setHorizontalHeader(header);
        view.setModel(&filter);
        view.setSortingEnabled(true);
        header->setSectionsMovable(true);
        header->setMinimumSectionSize(10);
        BHeaderState::makeInteractive(header);
        view.resize(1600, 400);
        model.setJobs(jobs());
    }

    QStringList idsInViewOrder() const
    {
        QStringList ids;
        for (int row = 0; row < filter.rowCount(); ++row) {
            ids << filter.index(row, BJobsModel::COL_JOBID).data().toString();
        }
        return ids;
    }

    int lastVisibleLogical() const
    {
        for (int visual = header->count() - 1; visual >= 0; --visual) {
            const int logical = header->logicalIndex(visual);
            if (!header->isSectionHidden(logical)) return logical;
        }
        return -1;
    }
};

} // namespace

class BJobsTableTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    void sortsByValue_data();
    void sortsByValue();
    void noRowColourByLevel();
    void statusAndLevelChips();

    void headerRoundTrip();
    void headerFollowsTheZoom();
    void legacyColumnsAreMigratedOnce();
    void everyColumnIsResizable();

private:
    QTemporaryDir m_config;
};

void BJobsTableTest::initTestCase()
{
    // The settings of the test, never those of the user: before BSettings
    // and BColumnConfiguration are used for the first time
    QVERIFY(m_config.isValid());
    qputenv("XDG_CONFIG_HOME", m_config.path().toUtf8());
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, m_config.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_config.path());
    // Windows keeps NativeFormat in the registry, where setPath does not reach
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QCoreApplication::setOrganizationName(QStringLiteral("bareos"));
    QCoreApplication::setApplicationName(QStringLiteral("onesimus-jobstable-test"));
    const QSettings probe(QSettings::defaultFormat(), QSettings::UserScope,
                         QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
    QVERIFY2(probe.fileName().startsWith(m_config.path()), qPrintable(probe.fileName()));
    const QSettings legacy;
    QVERIFY2(legacy.fileName().startsWith(m_config.path()), qPrintable(legacy.fileName()));

    BUiScale::instance().setBaseFont(QFont(QStringLiteral("DejaVu Sans"), 10));
}

void BJobsTableTest::init()
{
    BUiScale::instance().resetZoom();
    BSettings::instance().setJobsHeaderState(QByteArray());
    BSettings::instance().setJobsHeaderScale(0);
    QSettings legacy;
    legacy.remove(QStringLiteral("Columns"));
}

// --- Sorting -------------------------------------------------------------------

void BJobsTableTest::sortsByValue_data()
{
    QTest::addColumn<int>("column");
    QTest::addColumn<QStringList>("ascending");

    // A missing value sorts first
    QTest::newRow("bytes") << int(BJobsModel::COL_BYTES)
                           << QStringList{"10", "1", "5", "4", "6", "3", "2"};
    QTest::newRow("files") << int(BJobsModel::COL_FILES)
                           << QStringList{"10", "5", "6", "1", "3", "4", "2"};
    QTest::newRow("duration") << int(BJobsModel::COL_DURATION)
                              << QStringList{"10", "1", "6", "4", "3", "5", "2"};
    QTest::newRow("start time") << int(BJobsModel::COL_STARTTIME)
                                << QStringList{"10", "6", "4", "2", "3", "1", "5"};
    QTest::newRow("end time") << int(BJobsModel::COL_ENDTIME)
                              << QStringList{"10", "6", "4", "3", "1", "2", "5"};
    QTest::newRow("job id") << int(BJobsModel::COL_JOBID)
                            << QStringList{"1", "2", "3", "4", "5", "6", "10"};
}

void BJobsTableTest::sortsByValue()
{
    QFETCH(int, column);
    QFETCH(QStringList, ascending);

    JobsTable table;
    table.view.sortByColumn(column, Qt::AscendingOrder);
    QCOMPARE(table.idsInViewOrder(), ascending);

    QStringList descending = ascending;
    std::reverse(descending.begin(), descending.end());
    table.view.sortByColumn(column, Qt::DescendingOrder);
    QCOMPARE(table.idsInViewOrder(), descending);

    // The text would have sorted differently: the test means something
    if (column != BJobsModel::COL_JOBID) {
        QStringList byText = table.idsInViewOrder();
        QSortFilterProxyModel textSort;
        textSort.setSourceModel(&table.model);
        textSort.sort(column, Qt::AscendingOrder);
        byText.clear();
        for (int row = 0; row < textSort.rowCount(); ++row) {
            byText << textSort.index(row, BJobsModel::COL_JOBID).data().toString();
        }
        QVERIFY2(byText != ascending, qPrintable(byText.join(',')));
    }
}

// --- Colours ---------------------------------------------------------------------

void BJobsTableTest::noRowColourByLevel()
{
    JobsTable table;
    for (int row = 0; row < table.model.rowCount(); ++row) {
        for (int column = 0; column < table.model.columnCount(); ++column) {
            const QModelIndex index = table.model.index(row, column);
            QVERIFY2(!index.data(Qt::BackgroundRole).isValid(),
                     qPrintable(QStringLiteral("row %1 column %2").arg(row).arg(column)));
            QVERIFY2(!index.data(Qt::ForegroundRole).isValid(),
                     qPrintable(QStringLiteral("row %1 column %2").arg(row).arg(column)));
        }
    }
}

void BJobsTableTest::statusAndLevelChips()
{
    JobsTable table;
    BJobsModel &model = table.model;

    struct Expected {
        int row;
        BStatus::Tone tone;
        const char *code;
        const char *text;
        const char *full;
    };
    const Expected expected[] = {
        {0, BStatus::Tone::Success, "T", "OK", "Terminated normally"},
        {1, BStatus::Tone::Error, "E", "Error", "Terminated in error"},
        {2, BStatus::Tone::Warning, "W", "Warnings", "Terminated with warnings"},
        {3, BStatus::Tone::Error, "f", "Fatal", "Fatal error"},
        {4, BStatus::Tone::Neutral, "A", "Canceled", "Canceled by user"},
        {5, BStatus::Tone::Running, "R", "Running", "Running"},
        {6, BStatus::Tone::Waiting, "C", "Created", "Created, not yet running"},
    };
    for (const Expected &e : expected) {
        const QModelIndex status = model.index(e.row, BJobsModel::COL_STATUS);
        QCOMPARE(status.data(BStatus::ToneRole).toInt(), int(e.tone));
        QCOMPARE(status.data(BStatus::CodeRole).toString(), QString::fromLatin1(e.code));
        QCOMPARE(status.data(Qt::DisplayRole).toString(), QString::fromLatin1(e.text));
        // A screen reader hears the full word
        QCOMPARE(status.data(Qt::AccessibleTextRole).toString(), QString::fromLatin1(e.full));
    }

    // The failed Incremental (row 1) is not green anywhere in its row
    const QModelIndex failedIncrStatus = model.index(1, BJobsModel::COL_STATUS);
    QVERIFY(failedIncrStatus.data(BStatus::ToneRole).toInt() != int(BStatus::Tone::Success));

    // The level is a chip of its own with the level colour of the settings
    const QModelIndex level = model.index(1, BJobsModel::COL_LEVEL);
    QCOMPARE(level.data(Qt::DisplayRole).toString(), QStringLiteral("Incr"));
    QCOMPARE(level.data(BStatus::CodeRole).toString(), QStringLiteral("I"));
    QCOMPARE(level.data(Qt::AccessibleTextRole).toString(), QStringLiteral("Incremental"));
    QCOMPARE(level.data(BStatus::ChipColorRole).value<QColor>(), BSettings::instance().levelColor("I"));

    // Filters and exports still see the Director's letters
    QCOMPARE(model.jobAt(1)["jobstatus"].toString(), QStringLiteral("E"));
    QVERIFY(model.exportToCsv(false).contains(QStringLiteral("\"I\",\"100000\",\"10737418240\",\"E\"")));
}

// --- Header state ------------------------------------------------------------------

void BJobsTableTest::headerRoundTrip()
{
    QMap<int, int> widths;
    {
        JobsTable first;
        QHeaderView *header = first.header;
        header->resizeSection(BJobsModel::COL_NAME, 231);
        header->resizeSection(BJobsModel::COL_CLIENT, 77);
        header->resizeSection(BJobsModel::COL_BYTES, 143);
        header->moveSection(header->visualIndex(BJobsModel::COL_STATUS), 2);
        header->setSectionHidden(BJobsModel::COL_TYPE, true);
        first.view.sortByColumn(BJobsModel::COL_BYTES, Qt::DescendingOrder);
        for (int i = 0; i < header->count(); ++i) {
            widths[i] = header->sectionSize(i);
        }

        // What BJsonJobView::saveCurrentState() stores
        BSettings::instance().setJobsHeaderState(header->saveState());
        BSettings::instance().setJobsHeaderScale(BHeaderState::currentScale());
    }
    QCOMPARE(BSettings::instance().jobsHeaderScale(), BUiScale::px(1000));

    JobsTable second;
    QHeaderView *header = second.header;
    QCOMPARE(BHeaderState::restoreOrMigrate(header, BSettings::instance().jobsHeaderState(),
                                            BSettings::instance().jobsHeaderScale(),
                                            QStringLiteral("Columns")),
             BHeaderState::Source::Saved);
    QCOMPARE(header->visualIndex(BJobsModel::COL_STATUS), 2);
    QVERIFY(header->isSectionHidden(BJobsModel::COL_TYPE));
    QCOMPARE(header->sortIndicatorSection(), int(BJobsModel::COL_BYTES));
    QCOMPARE(header->sortIndicatorOrder(), Qt::DescendingOrder);
    const int stretched = second.lastVisibleLogical();
    for (int i = 0; i < header->count(); ++i) {
        if (i == stretched || header->isSectionHidden(i)) continue;
        QCOMPARE(header->sectionSize(i), widths[i]);
    }
    // Restored sort applies to the rows (as BJsonJobView::restoreLastState() does)
    second.view.sortByColumn(header->sortIndicatorSection(), header->sortIndicatorOrder());
    QCOMPARE(second.idsInViewOrder().first(), QStringLiteral("2"));
}

void BJobsTableTest::headerFollowsTheZoom()
{
    {
        JobsTable first;
        first.header->resizeSection(BJobsModel::COL_NAME, 200);
        first.header->resizeSection(BJobsModel::COL_CLIENT, 90);
        BSettings::instance().setJobsHeaderState(first.header->saveState());
        BSettings::instance().setJobsHeaderScale(BHeaderState::currentScale());
    }
    const int savedScale = BSettings::instance().jobsHeaderScale();
    QCOMPARE(savedScale, 1000);

    // Ctrl++ up to 150 % later: the widths grow with the text
    BUiScale::instance().setZoomPercent(150);
    QCOMPARE(BHeaderState::currentScale(), 1500);
    JobsTable second;
    QVERIFY(BHeaderState::restore(second.header, BSettings::instance().jobsHeaderState(), savedScale));
    QCOMPARE(second.header->sectionSize(BJobsModel::COL_NAME), 300);
    QCOMPARE(second.header->sectionSize(BJobsModel::COL_CLIENT), 135);

    // And back to 100 %: the stored widths at 150 % shrink again
    BSettings::instance().setJobsHeaderState(second.header->saveState());
    BSettings::instance().setJobsHeaderScale(BHeaderState::currentScale());
    BUiScale::instance().resetZoom();
    JobsTable third;
    QVERIFY(BHeaderState::restore(third.header, BSettings::instance().jobsHeaderState(),
                                  BSettings::instance().jobsHeaderScale()));
    QCOMPARE(third.header->sectionSize(BJobsModel::COL_NAME), 200);
    QCOMPARE(third.header->sectionSize(BJobsModel::COL_CLIENT), 90);

    // A state without a scale (0) is taken as it is
    JobsTable fourth;
    QVERIFY(BHeaderState::restore(fourth.header, BSettings::instance().jobsHeaderState(), 0));
    QCOMPARE(fourth.header->sectionSize(BJobsModel::COL_NAME), 300);
}

void BJobsTableTest::legacyColumnsAreMigratedOnce()
{
    // What older versions wrote through BColumnConfiguration ("Columns",
    // widths for 10 pt)
    {
        QSettings legacy;
        for (int i = 0; i < BJobsModel::COL_COUNT; ++i) {
            legacy.setValue(QStringLiteral("Columns/Column_%1/visualIndex").arg(i), i);
            legacy.setValue(QStringLiteral("Columns/Column_%1/width").arg(i), 50 + i);
            legacy.setValue(QStringLiteral("Columns/Column_%1/visible").arg(i), true);
        }
        legacy.setValue(QStringLiteral("Columns/Column_%1/width").arg(int(BJobsModel::COL_NAME)), 240);
        legacy.setValue(QStringLiteral("Columns/Column_%1/visible").arg(int(BJobsModel::COL_CLIENT)), false);
        // Status and job id swapped
        legacy.setValue(QStringLiteral("Columns/Column_%1/visualIndex").arg(int(BJobsModel::COL_STATUS)), 1);
        legacy.setValue(QStringLiteral("Columns/Column_%1/visualIndex").arg(int(BJobsModel::COL_JOBID)),
                        int(BJobsModel::COL_STATUS));
        legacy.sync();
    }
    BUiScale::instance().setZoomPercent(200);

    JobsTable table;
    QCOMPARE(BHeaderState::restoreOrMigrate(table.header, BSettings::instance().jobsHeaderState(),
                                            BSettings::instance().jobsHeaderScale(),
                                            QStringLiteral("Columns")),
             BHeaderState::Source::Legacy);
    // Design widths, scaled to the zoom
    QCOMPARE(table.header->sectionSize(BJobsModel::COL_NAME), 480);
    QVERIFY(table.header->isSectionHidden(BJobsModel::COL_CLIENT));
    QCOMPARE(table.header->visualIndex(BJobsModel::COL_STATUS), 1);
    QCOMPARE(table.header->visualIndex(BJobsModel::COL_JOBID), int(BJobsModel::COL_STATUS));
    QCOMPARE(table.header->sectionResizeMode(BJobsModel::COL_NAME), QHeaderView::Interactive);

    // The old group is gone: one source from now on
    {
        QSettings legacy;
        QVERIFY(legacy.childGroups().isEmpty() || !legacy.childGroups().contains(QStringLiteral("Columns")));
    }
    JobsTable again;
    QCOMPARE(BHeaderState::restoreOrMigrate(again.header, QByteArray(), 0, QStringLiteral("Columns")),
             BHeaderState::Source::Nothing);
}

void BJobsTableTest::everyColumnIsResizable()
{
    JobsTable table;
    QVERIFY(BHeaderState::restore(table.header, table.header->saveState(), 0));
    for (int i = 0; i < table.header->count(); ++i) {
        QCOMPARE(table.header->sectionResizeMode(i), QHeaderView::Interactive);
    }
    QVERIFY(table.header->stretchLastSection());
    QVERIFY(table.header->sectionsMovable());
}

QTEST_MAIN(BJobsTableTest)
#include "bjobstable_test.moc"
