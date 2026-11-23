// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

// The settings store of the connection profiles: secrets in the keychain
// and not in the profile JSON (P1-28), a reset that keeps the profiles
// (P1-16), and the export/import file (P1-15). A keychain in memory
// stands in for the system's; the settings live in a scratch directory.

#include <QtTest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QSignalSpy>
#include <QTemporaryDir>

#include "config/bsecretstore.h"
#include "config/bsettings.h"

namespace {
const QString HashA = QStringLiteral("0123456789abcdef0123456789abcdef");
const QString HashB = QStringLiteral("fedcba9876543210fedcba9876543210");

BConnectionProfile profile(const QString &name, const QString &hash)
{
    BConnectionProfile p = BConnectionProfile::create(name);
    p.host = name.toLower() + QStringLiteral(".example");
    p.passwordHash = hash;
    return p;
}

/// The profile JSON as it is in the settings file
QJsonArray storedJson()
{
    QSettings raw(QSettings::defaultFormat(), QSettings::UserScope,
                  QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
    return QJsonDocument::fromJson(raw.value("ConnectionProfiles/data").toByteArray()).array();
}

/// Writes profile JSON straight into the file, as an older version did
void writeRawProfiles(const QList<BConnectionProfile> &profiles)
{
    QJsonArray array;
    for (const BConnectionProfile &p : profiles)
        array.append(p.toJson());
    BSettings::instance().sync();
    {
        QSettings raw(QSettings::defaultFormat(), QSettings::UserScope,
                      QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
        raw.setValue("ConnectionProfiles/data", QJsonDocument(array).toJson(QJsonDocument::Compact));
        raw.sync();
    }
}
} // namespace

class BSettingsStoreTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    void secretsGoToTheKeychain();
    void profilesReadSecretsBackFromTheKeychain();
    void unchangedSecretsAreNotWrittenAgain();
    void deletedProfileRemovesItsSecrets();
    void secretsMigratedOutOfTheJson();
    void noKeychainKeepsSecretsWithAWarning();
    void refusedKeychainDoesNotLoseSecrets();
    void resetKeepsProfiles();
    void resetWithProfilesRemovesSecrets();
    void exportImportRoundTrip();
    void exportLeavesSecretsOutUnlessAsked();
    void importReadsTheOldKeyValueExport();

private:
    QTemporaryDir m_config;
    BMemorySecretBackend *m_keychain = nullptr;
};

void BSettingsStoreTest::initTestCase()
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

void BSettingsStoreTest::init()
{
    auto keychain = std::make_unique<BMemorySecretBackend>();
    m_keychain = keychain.get();
    BSettings &s = BSettings::instance();
    s.setSecretBackend(std::move(keychain));
    s.resetToDefaults(true);
}

void BSettingsStoreTest::secretsGoToTheKeychain()
{
    BConnectionProfile p = profile("Alpha", HashA);
    p.tlsPfxPassword = QStringLiteral("pfx-secret");
    BSettings::instance().setConnectionProfiles({p});

    const QJsonObject stored = storedJson().at(0).toObject();
    QVERIFY(!stored.contains("passwordHash"));
    QVERIFY(!stored.contains("tlsPfxPassword"));
    const QByteArray file = QJsonDocument(storedJson()).toJson();
    QVERIFY(!file.contains(HashA.toLatin1()));
    QVERIFY(!file.contains("pfx-secret"));

    QCOMPARE(m_keychain->entries().value(p.id + "/passwordHash"), HashA);
    QCOMPARE(m_keychain->entries().value(p.id + "/tlsPfxPassword"), QStringLiteral("pfx-secret"));
    QVERIFY(BSettings::instance().secretStorageWarning().isEmpty());
}

void BSettingsStoreTest::profilesReadSecretsBackFromTheKeychain()
{
    const BConnectionProfile p = profile("Alpha", HashA);
    BSettings::instance().setConnectionProfiles({p});

    // A new session: nothing cached, the keychain is asked
    auto keychain = std::make_unique<BMemorySecretBackend>();
    keychain->write(p.id + "/passwordHash", HashA, nullptr);
    BMemorySecretBackend *fresh = keychain.get();
    BSettings::instance().setSecretBackend(std::move(keychain));

    const BConnectionProfile read = BSettings::instance().connectionProfile(p.id);
    QCOMPARE(read.passwordHash, HashA);
    QVERIFY(read.isValid());
    // Asked once, then from memory
    BSettings::instance().connectionProfiles();
    QCOMPARE(fresh->reads(), 1);
}

void BSettingsStoreTest::unchangedSecretsAreNotWrittenAgain()
{
    BConnectionProfile p = profile("Alpha", HashA);
    BSettings::instance().setConnectionProfiles({p});
    QCOMPARE(m_keychain->writes(), 1);

    p.grpcCertFingerprint = QStringLiteral("abcd");    // e.g. a pinned certificate
    BSettings::instance().updateConnectionProfile(p);
    QCOMPARE(m_keychain->writes(), 1);

    p.passwordHash = HashB;
    BSettings::instance().updateConnectionProfile(p);
    QCOMPARE(m_keychain->writes(), 2);
    QCOMPARE(m_keychain->entries().value(p.id + "/passwordHash"), HashB);
}

void BSettingsStoreTest::deletedProfileRemovesItsSecrets()
{
    const BConnectionProfile a = profile("Alpha", HashA);
    const BConnectionProfile b = profile("Beta", HashB);
    BSettings::instance().setConnectionProfiles({a, b});
    QCOMPARE(m_keychain->entries().size(), 2);

    QVERIFY(BSettings::instance().removeConnectionProfile(a.id));
    QVERIFY(!m_keychain->entries().contains(a.id + "/passwordHash"));
    QVERIFY(m_keychain->entries().contains(b.id + "/passwordHash"));

    // A password that is no longer saved goes from the keychain too
    BConnectionProfile cleared = b;
    cleared.passwordHash.clear();
    BSettings::instance().updateConnectionProfile(cleared);
    QVERIFY(m_keychain->entries().isEmpty());
}

void BSettingsStoreTest::secretsMigratedOutOfTheJson()
{
    BConnectionProfile a = profile("Alpha", HashA);
    a.tlsPfxPassword = QStringLiteral("pfx-secret");
    const BConnectionProfile b = profile("Beta", QString());
    writeRawProfiles({a, b});
    QCOMPARE(storedJson().at(0).toObject().value("passwordHash").toString(), HashA);

    QCOMPARE(BSettings::instance().migrateSecretsToKeychain(), 2);

    const QByteArray file = QJsonDocument(storedJson()).toJson();
    QVERIFY(!file.contains(HashA.toLatin1()));
    QVERIFY(!file.contains("pfx-secret"));
    QCOMPARE(m_keychain->entries().value(a.id + "/passwordHash"), HashA);
    QCOMPARE(BSettings::instance().connectionProfile(a.id).passwordHash, HashA);
    QCOMPARE(BSettings::instance().connectionProfile(a.id).tlsPfxPassword, QStringLiteral("pfx-secret"));

    // Nothing left to move the next time
    QCOMPARE(BSettings::instance().migrateSecretsToKeychain(), 0);
}

void BSettingsStoreTest::noKeychainKeepsSecretsWithAWarning()
{
    m_keychain->setAvailable(false);
    const BConnectionProfile a = profile("Alpha", HashA);
    writeRawProfiles({a});
    QSignalSpy warnings(&BSettings::instance(), &BSettings::secretStorageWarningChanged);

    QCOMPARE(BSettings::instance().migrateSecretsToKeychain(), 0);

    // Kept (the user can still connect), and said so
    QCOMPARE(BSettings::instance().connectionProfile(a.id).passwordHash, HashA);
    QVERIFY(!BSettings::instance().secretStorageWarning().isEmpty());
    QVERIFY(BSettings::instance().secretStorageWarning().contains("keychain"));
    QCOMPARE(warnings.count(), 1);
#ifndef Q_OS_WIN
    // The file with the secret is readable by its owner only
    BSettings::instance().sync();
    const QSettings raw(QSettings::defaultFormat(), QSettings::UserScope,
                        QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
    const QFileDevice::Permissions perms = QFile::permissions(raw.fileName());
    QVERIFY(!(perms & (QFileDevice::ReadGroup | QFileDevice::ReadOther)));
#endif

    // The keychain comes back: the next start moves the secret
    m_keychain->setAvailable(true);
    QCOMPARE(BSettings::instance().migrateSecretsToKeychain(), 1);
    QVERIFY(BSettings::instance().secretStorageWarning().isEmpty());
}

void BSettingsStoreTest::refusedKeychainDoesNotLoseSecrets()
{
    BConnectionProfile p = profile("Alpha", HashA);
    BSettings::instance().setConnectionProfiles({p});

    // Next session: the keychain refuses (locked, unlock cancelled)
    auto keychain = std::make_unique<BMemorySecretBackend>();
    keychain->write(p.id + "/passwordHash", HashA, nullptr);
    keychain->setAvailable(false);
    BMemorySecretBackend *locked = keychain.get();
    BSettings::instance().setSecretBackend(std::move(keychain));

    BConnectionProfile read = BSettings::instance().connectionProfile(p.id);
    QVERIFY(read.passwordHash.isEmpty());
    QVERIFY(!BSettings::instance().secretStorageWarning().isEmpty());

    // Another change to the profile must not delete the stored secret
    read.name = QStringLiteral("Renamed");
    BSettings::instance().updateConnectionProfile(read);
    locked->setAvailable(true);
    QCOMPARE(locked->entries().value(p.id + "/passwordHash"), HashA);
    QVERIFY(storedJson().at(0).toObject().value("secretsInKeychain").toArray()
                .contains(QJsonValue("passwordHash")));
}

void BSettingsStoreTest::resetKeepsProfiles()
{
    BSettings &s = BSettings::instance();
    const BConnectionProfile p = profile("Alpha", HashA);
    s.setConnectionProfiles({p});
    s.setLastUsedProfileId(p.id);
    s.setBehaviorClientStaleHours(99);
    s.setAppearanceUiZoom(150);

    s.resetToDefaults();

    QCOMPARE(s.behaviorClientStaleHours(), 24);
    QCOMPARE(s.connectionProfiles().size(), 1);
    QCOMPARE(s.connectionProfile(p.id).passwordHash, HashA);
    QCOMPARE(s.lastUsedProfileId(), p.id);
    QVERIFY(m_keychain->entries().contains(p.id + "/passwordHash"));
}

void BSettingsStoreTest::resetWithProfilesRemovesSecrets()
{
    BSettings &s = BSettings::instance();
    const BConnectionProfile p = profile("Alpha", HashA);
    s.setConnectionProfiles({p});
    s.setLastUsedProfileId(p.id);

    s.resetToDefaults(true);

    QVERIFY(s.connectionProfiles().isEmpty());
    QVERIFY(s.lastUsedProfileId().isEmpty());
    QVERIFY(m_keychain->entries().isEmpty());
}

void BSettingsStoreTest::exportImportRoundTrip()
{
    BConnectionProfile a = profile("Alpha", HashA);
    a.transport = BConnectionProfile::Transport::Grpc;
    a.grpcPort = 12345;
    const BConnectionProfile b = profile("Beta", HashB);

    const QByteArray file = BSettings::profilesExportJson({a, b}, true);
    QString error;
    const QList<BConnectionProfile> read = BSettings::profilesFromExportJson(file, &error);
    QVERIFY2(error.isEmpty(), qPrintable(error));
    QCOMPARE(read.size(), 2);
    QCOMPARE(read.at(0).id, a.id);
    QCOMPARE(read.at(0).host, a.host);
    QCOMPARE(read.at(0).grpcPort, 12345);
    QCOMPARE(read.at(0).transport, BConnectionProfile::Transport::Grpc);
    QCOMPARE(read.at(1).passwordHash, HashB);

    // Into the store: both profiles, secrets in the keychain
    QList<BConnectionProfile> profiles;
    QCOMPARE(BSettings::mergeImportedProfiles(profiles, read), 2);
    BSettings::instance().setConnectionProfiles(profiles);
    QCOMPARE(BSettings::instance().connectionProfiles().size(), 2);
    QCOMPARE(BSettings::instance().connectionProfile(b.id).passwordHash, HashB);
    QCOMPARE(m_keychain->entries().value(b.id + "/passwordHash"), HashB);
}

void BSettingsStoreTest::exportLeavesSecretsOutUnlessAsked()
{
    BConnectionProfile a = profile("Alpha", HashA);
    a.tlsPfxPassword = QStringLiteral("pfx-secret");
    const QByteArray without = BSettings::profilesExportJson({a}, false);
    QVERIFY(!without.contains(HashA.toLatin1()));
    QVERIFY(!without.contains("pfx-secret"));

    // Importing it over the existing profile keeps the local secret
    QList<BConnectionProfile> profiles{a};
    BConnectionProfile changed = BSettings::profilesFromExportJson(without).value(0);
    changed.host = QStringLiteral("moved.example");
    QCOMPARE(BSettings::mergeImportedProfiles(profiles, {changed}), 1);
    QCOMPARE(profiles.size(), 1);
    QCOMPARE(profiles.at(0).host, QStringLiteral("moved.example"));
    QCOMPARE(profiles.at(0).passwordHash, HashA);
    QCOMPARE(profiles.at(0).tlsPfxPassword, QStringLiteral("pfx-secret"));
}

void BSettingsStoreTest::importReadsTheOldKeyValueExport()
{
    const BConnectionProfile a = profile("Alpha", HashA);
    QJsonObject old;
    old["Widgets/Jobs/header_state"] = QStringLiteral("@ByteArray(...)");
    old["ConnectionProfiles/data"] = QString::fromUtf8(
        QJsonDocument(QJsonArray{a.toJson()}).toJson(QJsonDocument::Compact));
    QString error;
    const QList<BConnectionProfile> read =
        BSettings::profilesFromExportJson(QJsonDocument(old).toJson(), &error);
    QCOMPARE(read.size(), 1);
    QCOMPARE(read.at(0).host, a.host);

    // A file without profiles says so
    QVERIFY(BSettings::profilesFromExportJson("{\"Widgets/x\": \"1\"}", &error).isEmpty());
    QVERIFY(!error.isEmpty());
    QVERIFY(BSettings::profilesFromExportJson("not json", &error).isEmpty());
    QVERIFY(!error.isEmpty());
}

QTEST_MAIN(BSettingsStoreTest)
#include "bsettingsstore_test.moc"
