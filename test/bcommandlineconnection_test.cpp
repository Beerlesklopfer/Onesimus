// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bcommandlineconnection_test.cpp
 * @brief Command line options to Director configuration: --tls* and the password sources
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>
#include <QTemporaryDir>

#include "bcommandlineconnection.h"

class BCommandLineConnectionTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void defaultsAreWithoutTls();
    void noTlsLeavesConfigurationUntouched();
    void tlsAloneIsPsk();
    void certificateOptionsGoToTheirOwnFields();
    void caOnlyIsCertificateModeWithVerification();
    void certificateOptionWithoutTlsIsAnError();
    void certificateWithoutCaIsAnError();
    void certificateWithoutKeyIsAnError();
    void keyWithoutCertificateIsAnError();
    void missingFileIsAnError();
    void wrongPlatformOptionIsAnError();
    void errorLeavesConfigurationUntouched();

    void passwordFromOption();
    void passwordFromFile();
    void passwordFileWithCrLf();
    void passwordFromEnvironment();
    void passwordOptionWinsOverEnvironment();
    void passwordFileWinsOverEnvironment();
    void passwordOptionAndFileIsAnError();
    void passwordFileMissing();
    void passwordFileEmpty();
    void noPassword();

private:
    QString writeFile(const QString &name, const QByteArray &content);

    QTemporaryDir m_dir;
    QString m_ca;
    QString m_cert;
    QString m_key;
    QString m_pfx;
};

QString BCommandLineConnectionTest::writeFile(const QString &name, const QByteArray &content)
{
    const QString path = m_dir.filePath(name);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return QString();
    }
    file.write(content);
    return path;
}

void BCommandLineConnectionTest::initTestCase()
{
    QVERIFY(m_dir.isValid());
    // Only their existence matters here
    m_ca = writeFile("ca.pem", "ca");
    m_cert = writeFile("cert.pem", "cert");
    m_key = writeFile("key.pem", "key");
    m_pfx = writeFile("client.pfx", "pfx");
    QVERIFY(!m_ca.isEmpty() && !m_cert.isEmpty() && !m_key.isEmpty() && !m_pfx.isEmpty());
}

// ---------------------------------------------------------------------------
// TLS
// ---------------------------------------------------------------------------

void BCommandLineConnectionTest::defaultsAreWithoutTls()
{
    const BareosDirector::TLSConfig config;
    QVERIFY(!config.tlsEnable);
    QVERIFY(!config.tlsRequire);
    QVERIFY(!config.tlsPSKEnable);
    QVERIFY(!config.tlsVerifyPeer);
    QVERIFY(config.tlsCaCertFile->fileName().isEmpty());
}

void BCommandLineConnectionTest::noTlsLeavesConfigurationUntouched()
{
    BareosDirector::TLSConfig config;
    QCOMPARE(bApplyCommandLineTls(BCommandLineTlsOptions(), config), QString());
    QVERIFY(!config.tlsEnable);
    QVERIFY(!config.tlsRequire);
    QVERIFY(!config.tlsPSKEnable);
}

void BCommandLineConnectionTest::tlsAloneIsPsk()
{
    BCommandLineTlsOptions options;
    options.tls = true;

    BareosDirector::TLSConfig config;
    QCOMPARE(bApplyCommandLineTls(options, config), QString());
    QVERIFY(config.tlsEnable);
    QVERIFY(config.tlsRequire);
    QVERIFY(config.tlsPSKEnable);
    QVERIFY(!config.tlsVerifyPeer);   // PSK has no certificate to verify
    QVERIFY(config.tlsCaCertFile->fileName().isEmpty());
}

void BCommandLineConnectionTest::certificateOptionsGoToTheirOwnFields()
{
    BCommandLineTlsOptions options;
    options.tls = true;
    options.caFile = m_ca;
#ifdef Q_OS_WINDOWS
    options.pfxFile = m_pfx;
    options.pfxPassword = "pfx-secret";
#else
    options.certFile = m_cert;
    options.keyFile = m_key;
#endif

    BareosDirector::TLSConfig config;
    QCOMPARE(bApplyCommandLineTls(options, config), QString());

    QVERIFY(config.tlsEnable);
    QVERIFY(config.tlsRequire);
    QVERIFY(!config.tlsPSKEnable);
    QVERIFY(config.tlsVerifyPeer);

    // The old code wrote every value into the CA field
    QCOMPARE(config.tlsCaCertFile->fileName(), m_ca);
#ifdef Q_OS_WINDOWS
    QCOMPARE(config.tlsPfxFile->fileName(), m_pfx);
    QCOMPARE(config.tlsPfxPassword, QString("pfx-secret"));
#else
    QCOMPARE(config.tlsCertFile->fileName(), m_cert);
    QCOMPARE(config.tlsKeyFile->fileName(), m_key);
#endif
}

void BCommandLineConnectionTest::caOnlyIsCertificateModeWithVerification()
{
    BCommandLineTlsOptions options;
    options.tls = true;
    options.caFile = m_ca;

    BareosDirector::TLSConfig config;
    QCOMPARE(bApplyCommandLineTls(options, config), QString());
    QVERIFY(config.tlsEnable);
    QVERIFY(config.tlsRequire);
    QVERIFY(!config.tlsPSKEnable);
    QVERIFY(config.tlsVerifyPeer);
    QCOMPARE(config.tlsCaCertFile->fileName(), m_ca);
}

void BCommandLineConnectionTest::certificateOptionWithoutTlsIsAnError()
{
    BCommandLineTlsOptions options;
    options.caFile = m_ca;   // --tls-ca without --tls

    BareosDirector::TLSConfig config;
    QVERIFY(!bApplyCommandLineTls(options, config).isEmpty());
    QVERIFY(!config.tlsEnable);
}

void BCommandLineConnectionTest::certificateWithoutCaIsAnError()
{
    // P0-11: certificate TLS verifies against the given CA only
    BCommandLineTlsOptions options;
    options.tls = true;
#ifdef Q_OS_WINDOWS
    options.pfxFile = m_pfx;
#else
    options.certFile = m_cert;
    options.keyFile = m_key;
#endif

    BareosDirector::TLSConfig config;
    const QString error = bApplyCommandLineTls(options, config);
    QVERIFY(error.contains("--tls-ca"));
    QVERIFY(!config.tlsEnable);
}

void BCommandLineConnectionTest::certificateWithoutKeyIsAnError()
{
    BCommandLineTlsOptions options;
    options.tls = true;
    options.certFile = m_cert;

    BareosDirector::TLSConfig config;
    QVERIFY(!bApplyCommandLineTls(options, config).isEmpty());
}

void BCommandLineConnectionTest::keyWithoutCertificateIsAnError()
{
    BCommandLineTlsOptions options;
    options.tls = true;
    options.keyFile = m_key;

    BareosDirector::TLSConfig config;
    QVERIFY(!bApplyCommandLineTls(options, config).isEmpty());
}

void BCommandLineConnectionTest::missingFileIsAnError()
{
    BCommandLineTlsOptions options;
    options.tls = true;
    options.caFile = m_dir.filePath("does-not-exist.pem");

    BareosDirector::TLSConfig config;
    const QString error = bApplyCommandLineTls(options, config);
    QVERIFY(error.contains("does-not-exist.pem"));
}

void BCommandLineConnectionTest::wrongPlatformOptionIsAnError()
{
    BCommandLineTlsOptions options;
    options.tls = true;
#ifdef Q_OS_WINDOWS
    options.certFile = m_cert;
    options.keyFile = m_key;
#else
    options.pfxFile = m_pfx;
#endif

    BareosDirector::TLSConfig config;
    QVERIFY(!bApplyCommandLineTls(options, config).isEmpty());
}

void BCommandLineConnectionTest::errorLeavesConfigurationUntouched()
{
    BCommandLineTlsOptions options;
    options.tls = true;
    options.caFile = m_ca;
    options.certFile = m_cert;   // key missing

    BareosDirector::TLSConfig config;
    QVERIFY(!bApplyCommandLineTls(options, config).isEmpty());
    QVERIFY(!config.tlsEnable);
    QVERIFY(!config.tlsRequire);
    QVERIFY(config.tlsCaCertFile->fileName().isEmpty());
}

// ---------------------------------------------------------------------------
// Password
// ---------------------------------------------------------------------------

void BCommandLineConnectionTest::passwordFromOption()
{
    QString error;
    QCOMPARE(bResolveCommandLinePassword(true, "from-option", QString(), QString(), &error),
             QString("from-option"));
    QVERIFY(error.isEmpty());
}

void BCommandLineConnectionTest::passwordFromFile()
{
    const QString path = writeFile("password", "from file \n second line\n");
    QString error;
    // Only the line ending is removed; a trailing blank belongs to the password
    QCOMPARE(bResolveCommandLinePassword(false, QString(), path, QString(), &error),
             QString("from file "));
    QVERIFY(error.isEmpty());
}

void BCommandLineConnectionTest::passwordFileWithCrLf()
{
    const QString path = writeFile("password-crlf", "from-file\r\n");
    QString error;
    QCOMPARE(bResolveCommandLinePassword(false, QString(), path, QString(), &error),
             QString("from-file"));
    QVERIFY(error.isEmpty());
}

void BCommandLineConnectionTest::passwordFromEnvironment()
{
    QString error;
    QCOMPARE(bResolveCommandLinePassword(false, QString(), QString(), "from-env", &error),
             QString("from-env"));
    QVERIFY(error.isEmpty());
}

void BCommandLineConnectionTest::passwordOptionWinsOverEnvironment()
{
    QString error;
    QCOMPARE(bResolveCommandLinePassword(true, "from-option", QString(), "from-env", &error),
             QString("from-option"));
    QVERIFY(error.isEmpty());
}

void BCommandLineConnectionTest::passwordFileWinsOverEnvironment()
{
    const QString path = writeFile("password-env", "from-file\n");
    QString error;
    QCOMPARE(bResolveCommandLinePassword(false, QString(), path, "from-env", &error),
             QString("from-file"));
    QVERIFY(error.isEmpty());
}

void BCommandLineConnectionTest::passwordOptionAndFileIsAnError()
{
    const QString path = writeFile("password-both", "from-file\n");
    QString error;
    QVERIFY(bResolveCommandLinePassword(true, "from-option", path, QString(), &error).isEmpty());
    QVERIFY(!error.isEmpty());
}

void BCommandLineConnectionTest::passwordFileMissing()
{
    QString error;
    QVERIFY(bResolveCommandLinePassword(false, QString(), m_dir.filePath("no-such-file"),
                                        "from-env", &error).isEmpty());
    QVERIFY(error.contains("no-such-file"));
}

void BCommandLineConnectionTest::passwordFileEmpty()
{
    const QString path = writeFile("password-empty", "\n");
    QString error;
    QVERIFY(bResolveCommandLinePassword(false, QString(), path, "from-env", &error).isEmpty());
    QVERIFY(!error.isEmpty());
}

void BCommandLineConnectionTest::noPassword()
{
    QString error;
    QVERIFY(bResolveCommandLinePassword(false, QString(), QString(), QString(), &error).isEmpty());
    QVERIFY(error.isEmpty());
}

QTEST_GUILESS_MAIN(BCommandLineConnectionTest)
#include "bcommandlineconnection_test.moc"
