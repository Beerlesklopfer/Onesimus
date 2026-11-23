// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bschedulewizard_test.cpp
 * @brief The wizard for new schedules: name page, grid, review
 *
 * - A name, a run painted on the grid, Finish: the schedule is created
 *   through BSchedulePersistence (CreateResource, checked first, reviewed),
 *   with the text of BScheduleResource::createText(); nothing else is sent.
 * - Names that cannot be taken are refused on the first page.
 * - An empty grid cannot be finished.
 * - Without CreateResource (plain Bareos, BNET) the text and the
 *   "configure add schedule" command carry name, description, Enabled and
 *   the painted runs.
 *
 * Offscreen, against the fake Director of bfakeconfigclient.h.
 */

#include <QtTest>
#include <QAbstractButton>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTemporaryDir>

#include "config/bsettings.h"
#include "schedules/bschedulegrideditor.h"
#include "schedules/bschedulegridview.h"
#include "schedules/bschedulepersistence.h"
#include "schedules/bschedulewizard.h"
#include "schedules/model/bschedulegridmodel.h"
#include "schedules/model/bscheduleresource.h"

#include "bfakeconfigclient.h"
#include "bfakesaveui.h"

namespace {

BGridBrush levelBrush(const QString &level)
{
    BGridBrush brush;
    brush.overrides.level = level;
    return brush;
}

/// The wizard with a fake Director behind CreateResource
struct Fixture
{
    BFakeConfigClient client;
    BFakeSaveUi ui{&client};
    BSchedulePersistence persistence{&client, &ui};
    BScheduleWizard wizard{nullptr};
    QSignalSpy finished{&persistence, &BSchedulePersistence::finished};

    explicit Fixture(bool viaCreate = true)
    {
        client.files.insert(QStringLiteral("WeeklyCycle"),
                            QStringLiteral("Schedule {\n  Name = \"WeeklyCycle\"\n  Run = Level=Full sun at 02:00\n}\n"));
        if (viaCreate)
            wizard.setPersistence(&persistence);
        wizard.setExistingScheduleNames({QStringLiteral("WeeklyCycle")});
        wizard.show();
    }

    bool wait() { return !finished.isEmpty() || finished.wait(3000); }
    BScheduleReport report() const { return finished.last().at(0).value<BScheduleReport>(); }
    QAbstractButton *next() const { return wizard.button(QWizard::NextButton); }
    QAbstractButton *finish() const { return wizard.button(QWizard::FinishButton); }

    void fillName(const QString &name, const QString &description = QString())
    {
        wizard.namePage()->nameEdit()->setText(name);
        wizard.namePage()->descriptionEdit()->setText(description);
    }
    bool paint(int row, int column, const QString &level)
    {
        return wizard.gridView()->editor()->paint(BGridCellSet::rect(row, column, row, column), levelBrush(level));
    }
};

} // namespace

class BScheduleWizardTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void createsThroughCreateResource();
    void cancelledReviewWritesNothing();
    void nameIsValidated();
    void emptyGridIsRefused();
    void withoutCreateResourceTheCommandCarriesTheRuns();

private:
    QTemporaryDir m_settingsDir;
};

void BScheduleWizardTest::initTestCase()
{
    // The settings never go to those of the user
    QVERIFY(m_settingsDir.isValid());
    qputenv("XDG_CONFIG_HOME", m_settingsDir.path().toUtf8());
    QStandardPaths::setTestModeEnabled(true);
}

void BScheduleWizardTest::createsThroughCreateResource()
{
    Fixture f;
    QCOMPARE(f.wizard.currentId(), int(BScheduleWizard::Page_Name));
    QVERIFY(!f.next()->isEnabled());
    f.fillName(QStringLiteral("Nightly"), QStringLiteral("Night runs"));
    QVERIFY(f.next()->isEnabled());
    f.wizard.next();
    QCOMPARE(f.wizard.currentId(), int(BScheduleWizard::Page_Grid));

    // The grid starts empty
    QVERIFY(f.wizard.gridView()->hasSchedule());
    QVERIFY(f.wizard.gridView()->model()->snapshot().runs.isEmpty());
    QVERIFY(f.paint(6, 5, QStringLiteral("Full")));
    QVERIFY(f.paint(2, 22, QStringLiteral("Incremental")));
    QVERIFY(f.next()->isEnabled());
    f.wizard.next();
    QCOMPARE(f.wizard.currentId(), int(BScheduleWizard::Page_Review));

    QStringList runs;
    QVERIFY(f.wizard.runTexts(&runs));
    QCOMPARE(runs.size(), 2);
    const QString expected = BScheduleResource::createText(QStringLiteral("Nightly"), QStringLiteral("Night runs"), runs);
    QCOMPARE(f.wizard.reviewPage()->configText(), expected);
    for (const QString &run : std::as_const(runs))
        QVERIFY(f.wizard.reviewPage()->runLinesText().contains(QStringLiteral("Run = ") + run));
    QVERIFY(f.client.requests.isEmpty());

    QTest::mouseClick(f.finish(), Qt::LeftButton);
    QVERIFY(f.wait());
    QCOMPARE(f.report().kind, BScheduleReport::Written);

    // Checked, reviewed, written: one Create with the text of the review
    QCOMPARE(f.client.requests.size(), 2);
    QCOMPARE(f.client.requests[0].kind, BConfig::Request::Create);
    QVERIFY(f.client.requests[0].validateOnly);
    QCOMPARE(f.client.writes().size(), 1);
    const BConfig::Request write = f.client.writes().first();
    QCOMPARE(write.kind, BConfig::Request::Create);
    QCOMPARE(write.text, expected);
    QVERIFY(f.ui.log.first().startsWith(QStringLiteral("confirm@1 writes=0")));
    QCOMPARE(f.ui.reviews.size(), 1);
    QCOMPARE(f.ui.reviews.first().operation, BScheduleReview::Create);
    QCOMPARE(f.ui.reviews.first().newRows.size(), 2);

    QStringList values;
    QVERIFY(BScheduleResource::runValues(write.text, &values));
    QCOMPARE(values, runs);
    QVERIFY2(values.join(' ').contains(QStringLiteral("Level=Full")), qPrintable(values.join(" | ")));
    QVERIFY2(values.join(' ').contains(QStringLiteral("at 05:00")), qPrintable(values.join(" | ")));
    QVERIFY2(values.join(' ').contains(QStringLiteral("at 22:00")), qPrintable(values.join(" | ")));
    QCOMPARE(f.client.files.value(QStringLiteral("Nightly")), expected);

    // Written: the wizard is done
    QTRY_VERIFY(!f.wizard.isVisible());
    QCOMPARE(f.wizard.result(), int(QDialog::Accepted));
}

void BScheduleWizardTest::cancelledReviewWritesNothing()
{
    Fixture f;
    f.ui.confirmAnswer = false;
    f.fillName(QStringLiteral("Nightly"));
    f.wizard.next();
    QVERIFY(f.paint(0, 1, QStringLiteral("Full")));
    f.wizard.next();
    QTest::mouseClick(f.finish(), Qt::LeftButton);
    QVERIFY(f.wait());
    QCOMPARE(f.report().kind, BScheduleReport::Cancelled);
    QVERIFY(f.client.writes().isEmpty());
    QVERIFY(!f.client.files.contains(QStringLiteral("Nightly")));
    QTest::qWait(20);
    QVERIFY(f.wizard.isVisible());
    QCOMPARE(f.wizard.currentId(), int(BScheduleWizard::Page_Review));
    QCOMPARE(f.wizard.reviewPage()->statusText(), QStringLiteral("Nothing was written."));
}

void BScheduleWizardTest::nameIsValidated()
{
    const QStringList existing = {QStringLiteral("WeeklyCycle")};
    QVERIFY(BScheduleWizard::nameProblem(QStringLiteral("Nightly"), existing).isEmpty());
    QVERIFY(BScheduleWizard::nameProblem(QStringLiteral("Night-2_a.b:c d"), existing).isEmpty());
    QVERIFY(!BScheduleWizard::nameProblem(QString(), existing).isEmpty());
    QVERIFY(!BScheduleWizard::nameProblem(QStringLiteral("WeeklyCycle"), existing).isEmpty());
    QVERIFY(!BScheduleWizard::nameProblem(QStringLiteral("a\"b"), existing).isEmpty());
    QVERIFY(!BScheduleWizard::nameProblem(QStringLiteral("a{b}"), existing).isEmpty());
    QVERIFY(!BScheduleWizard::nameProblem(QStringLiteral("Wöchentlich"), existing).isEmpty());
    QVERIFY(!BScheduleWizard::nameProblem(QString(128, QLatin1Char('a')), existing).isEmpty());
    QVERIFY(BScheduleWizard::nameProblem(QString(127, QLatin1Char('a')), existing).isEmpty());

    // On the page: Next only for a name that can be taken, the reason shown
    Fixture f;
    QLabel *problem = f.wizard.findChild<QLabel *>(QStringLiteral("scheduleWizardNameProblem"));
    QVERIFY(problem);
    QVERIFY(!f.next()->isEnabled());
    QVERIFY(problem->text().isEmpty());
    f.fillName(QStringLiteral("WeeklyCycle"));
    QVERIFY(!f.next()->isEnabled());
    QVERIFY(problem->text().contains(QStringLiteral("exists already")));
    f.fillName(QStringLiteral("Weekly;Cycle"));
    QVERIFY(!f.next()->isEnabled());
    QVERIFY(!problem->text().isEmpty());
    f.fillName(QStringLiteral("  Nightly  "));
    QVERIFY(f.next()->isEnabled());
    QVERIFY(problem->text().isEmpty());
    QCOMPARE(f.wizard.scheduleName(), QStringLiteral("Nightly"));
}

void BScheduleWizardTest::emptyGridIsRefused()
{
    Fixture f;
    f.fillName(QStringLiteral("Nightly"));
    f.wizard.next();
    QCOMPARE(f.wizard.currentId(), int(BScheduleWizard::Page_Grid));
    QVERIFY(!f.next()->isEnabled());
    QString error;
    QVERIFY(!f.wizard.runTexts(nullptr, &error));
    QVERIFY(!error.isEmpty());
    QVERIFY(f.wizard.generateConfigText().isEmpty());
    QVERIFY(f.wizard.generateConfigureCommand().isEmpty());

    // Painted and erased again: empty again
    QVERIFY(f.paint(3, 4, QStringLiteral("Full")));
    QVERIFY(f.next()->isEnabled());
    QVERIFY(f.wizard.gridView()->editor()->erase(BGridCellSet::rect(3, 4, 3, 4)));
    QVERIFY(!f.next()->isEnabled());

    // Back to the name and on again: the grid keeps what was painted
    QVERIFY(f.paint(3, 4, QStringLiteral("Full")));
    f.wizard.back();
    f.fillName(QStringLiteral("Nightly2"));
    f.wizard.next();
    QVERIFY(f.wizard.runTexts(nullptr));
    QVERIFY(f.client.requests.isEmpty());
}

void BScheduleWizardTest::withoutCreateResourceTheCommandCarriesTheRuns()
{
    Fixture f(false);
    f.fillName(QStringLiteral("Nightly"), QStringLiteral("Night runs"));
    f.wizard.namePage()->enabledCheck()->setChecked(false);
    f.wizard.next();
    QVERIFY(f.paint(6, 5, QStringLiteral("Full")));
    f.wizard.next();

    QStringList runs;
    QVERIFY(f.wizard.runTexts(&runs));
    QCOMPARE(runs.size(), 1);
    const QString text = f.wizard.generateConfigText();
    QVERIFY(text.contains(QStringLiteral("  Enabled = no\n")));
    QCOMPARE(text, BScheduleResource::createText(QStringLiteral("Nightly"), QStringLiteral("Night runs"), false, runs));
    QCOMPARE(f.wizard.generateConfigureCommand(),
             QStringLiteral("add schedule name=\"Nightly\" description=\"Night runs\" enabled=no run=")
                 + BRunWriter::quotedForCommand(runs.first()));
    QCOMPARE(f.wizard.reviewPage()->commandText(), QStringLiteral("configure ") + f.wizard.generateConfigureCommand());

    // No Director: Finish closes without sending anything
    QTest::mouseClick(f.finish(), Qt::LeftButton);
    QTRY_VERIFY(!f.wizard.isVisible());
    QVERIFY(f.client.requests.isEmpty());
}

QTEST_MAIN(BScheduleWizardTest)
#include "bschedulewizard_test.moc"
