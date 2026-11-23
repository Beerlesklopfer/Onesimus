// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

// Widgets of the application on their real classes (no Director, no
// network, scratch settings, a keychain in memory):
//  - settings dialog (P1-16, P1-15): Cancel reverts, the edited row is
//    relabelled, Reset keeps the profiles, export/import on the app's store

#include <QtTest>
#include <QApplication>
#include <QCheckBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSettings>
#include <QTemporaryDir>

#include "config/bsecretstore.h"
#include "config/bsettings.h"
#include "config/bsettingsdialog.h"

namespace {
const QString Hash = QStringLiteral("0123456789abcdef0123456789abcdef");

BConnectionProfile profile(const QString &name)
{
    BConnectionProfile p = BConnectionProfile::create(name);
    p.host = name.toLower() + QStringLiteral(".example");
    p.passwordHash = Hash;
    return p;
}

QStringList storedNames()
{
    QStringList names;
    for (const BConnectionProfile &p : BSettings::instance().connectionProfiles())
        names << p.name;
    return names;
}

} // namespace

class BAppWidgetsTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    // Settings dialog
    void cancelRevertsProfilesAndSettings();
    void saveWritesTheWorkingCopy();
    void editedRowIsRelabelled();
    void resetKeepsProfiles();
    void resetCanIncludeProfiles();
    void exportImportUsesTheAppStore();

private:
    QTemporaryDir m_config;
};

void BAppWidgetsTest::initTestCase()
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
}

void BAppWidgetsTest::init()
{
    BSettings &s = BSettings::instance();
    s.setSecretBackend(std::make_unique<BMemorySecretBackend>());
    s.resetToDefaults(true);
    const BConnectionProfile a = profile("Alpha");
    s.setConnectionProfiles({a, profile("Beta")});
    s.setLastUsedProfileId(a.id);
    s.setBehaviorConfirmJobCancel(true);
}

void BAppWidgetsTest::cancelRevertsProfilesAndSettings()
{
    BSettingsDialog dialog(nullptr);
    auto *add = dialog.findChild<QPushButton *>("addProfileButton");
    auto *name = dialog.findChild<QLineEdit *>("profileNameEdit");
    auto *confirm = dialog.findChild<QCheckBox *>("confirmJobCancelCheck");
    QVERIFY(add && name && confirm);

    add->click();
    name->setText(QStringLiteral("Gamma"));
    confirm->setChecked(false);
    QCOMPARE(dialog.workingProfiles().size(), 2 + 1);

    dialog.reject();

    QCOMPARE(storedNames(), (QStringList{"Alpha", "Beta"}));
    QVERIFY(BSettings::instance().behaviorConfirmJobCancel());
}

void BAppWidgetsTest::saveWritesTheWorkingCopy()
{
    BSettingsDialog dialog(nullptr);
    dialog.findChild<QPushButton *>("addProfileButton")->click();
    dialog.findChild<QLineEdit *>("profileNameEdit")->setText(QStringLiteral("Gamma"));
    dialog.findChild<QCheckBox *>("confirmJobCancelCheck")->setChecked(false);

    dialog.saveSettings();

    QCOMPARE(storedNames(), (QStringList{"Alpha", "Beta", "Gamma"}));
    QVERIFY(!BSettings::instance().behaviorConfirmJobCancel());
}

void BAppWidgetsTest::editedRowIsRelabelled()
{
    BSettingsDialog dialog(nullptr);
    auto *list = dialog.findChild<QListWidget *>("profileList");
    auto *name = dialog.findChild<QLineEdit *>("profileNameEdit");
    QVERIFY(list && name);
    list->setCurrentRow(0);
    name->setText(QStringLiteral("Renamed"));

    // Moving to the next row takes the edit into the row that was edited
    list->setCurrentRow(1);
    QVERIFY2(list->item(0)->text().startsWith("Renamed"), qPrintable(list->item(0)->text()));
    QVERIFY2(list->item(1)->text().startsWith("Beta"), qPrintable(list->item(1)->text()));
    QCOMPARE(name->text(), QStringLiteral("Beta"));
}

void BAppWidgetsTest::resetKeepsProfiles()
{
    BSettings::instance().setBehaviorClientStaleHours(99);
    BSettingsDialog dialog(nullptr);
    dialog.resetToDefaults(false);

    QCOMPARE(BSettings::instance().behaviorClientStaleHours(), 24);
    QCOMPARE(storedNames(), (QStringList{"Alpha", "Beta"}));
    QCOMPARE(dialog.workingProfiles().size(), 2);
    QCOMPARE(BSettings::instance().connectionProfiles().at(0).passwordHash, Hash);
}

void BAppWidgetsTest::resetCanIncludeProfiles()
{
    BSettingsDialog dialog(nullptr);
    dialog.resetToDefaults(true);
    QVERIFY(storedNames().isEmpty());
    QVERIFY(dialog.workingProfiles().isEmpty());
}

void BAppWidgetsTest::exportImportUsesTheAppStore()
{
    const QString file = m_config.filePath("profiles.json");
    {
        BSettingsDialog dialog(nullptr);
        QString error;
        QVERIFY2(dialog.exportProfiles(file, false, &error), qPrintable(error));
    }
    QFile written(file);
    QVERIFY(written.open(QIODevice::ReadOnly));
    const QByteArray data = written.readAll();
    QVERIFY(data.contains("alpha.example"));
    QVERIFY(!data.contains(Hash.toLatin1()));   // asked without secrets

    // Into an empty store: the profiles appear there once saved
    BSettings::instance().resetToDefaults(true);
    BSettingsDialog dialog(nullptr);
    QString error;
    QCOMPARE(dialog.importProfiles(file, &error), 2);
    QVERIFY(storedNames().isEmpty());            // not before saving
    dialog.saveSettings();
    QCOMPARE(storedNames(), (QStringList{"Alpha", "Beta"}));

    // Nothing went to a default QSettings of the process (the old bug)
    QSettings defaults;
    QVERIFY(!defaults.contains("ConnectionProfiles/data"));
}

QTEST_MAIN(BAppWidgetsTest)
#include "bappwidgets_test.moc"
