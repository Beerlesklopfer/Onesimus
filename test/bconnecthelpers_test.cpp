// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

// Connection handling of the main window (P1-14): the one way from a
// profile to the TLS configuration, and a wait cursor that stays balanced
// however often a connection attempt starts and ends.

#include <QtTest>
#include <QGuiApplication>

#include "bprofiletls.h"
#include "bwaitcursor.h"

class BConnectHelpersTest : public QObject
{
    Q_OBJECT

private slots:
    void pskProfile();
    void certificateProfile();
    void legacyProfileHasNoTls();
    void nothingOfThePreviousProfileSurvives();
    void waitCursorStaysBalanced();
};

void BConnectHelpersTest::pskProfile()
{
    BConnectionProfile p = BConnectionProfile::create("psk");
    p.tlsUsePSK = true;
    p.tlsVerifyPeer = true;     // ignored with PSK
    p.tlsCipherList = QStringLiteral("PSK-AES256-GCM-SHA384");
    const BDirector::TLSConfig c = bTlsConfigFromProfile(p);
    QVERIFY(c.tlsEnable);
    QVERIFY(c.tlsRequire);
    QVERIFY(c.tlsPSKEnable);
    QVERIFY(!c.tlsVerifyPeer);
    QCOMPARE(c.tlsCipherList, p.tlsCipherList);
}

void BConnectHelpersTest::certificateProfile()
{
    BConnectionProfile p = BConnectionProfile::create("cert");
    p.tlsUsePSK = false;
    p.tlsVerifyPeer = true;
    p.tlsCaCertFile = QStringLiteral("/etc/ca.pem");
    p.tlsCertFile = QStringLiteral("/etc/cert.pem");
    p.tlsKeyFile = QStringLiteral("/etc/key.pem");
    p.tlsPfxFile = QStringLiteral("C:/cert.pfx");
    p.tlsPfxPassword = QStringLiteral("secret");
    p.tlsAllowedCn = {QStringLiteral("dir.example")};
    const BDirector::TLSConfig c = bTlsConfigFromProfile(p);
    QVERIFY(c.tlsEnable);
    QVERIFY(!c.tlsPSKEnable);
    QVERIFY(c.tlsVerifyPeer);
    QCOMPARE(c.tlsAllowedCn, p.tlsAllowedCn);
    QCOMPARE(c.tlsCaCertFile->fileName(), p.tlsCaCertFile);
#ifdef Q_OS_WINDOWS
    QCOMPARE(c.tlsPfxFile->fileName(), p.tlsPfxFile);
    QCOMPARE(c.tlsPfxPassword, p.tlsPfxPassword);
#else
    QCOMPARE(c.tlsCertFile->fileName(), p.tlsCertFile);
    QCOMPARE(c.tlsKeyFile->fileName(), p.tlsKeyFile);
#endif
}

void BConnectHelpersTest::legacyProfileHasNoTls()
{
    BConnectionProfile p = BConnectionProfile::create("legacy");
    p.legacyAuth = true;
    const BDirector::TLSConfig c = bTlsConfigFromProfile(p);
    QVERIFY(!c.tlsEnable);
    QVERIFY(!c.tlsRequire);
    QVERIFY(!c.tlsPSKEnable);
}

void BConnectHelpersTest::nothingOfThePreviousProfileSurvives()
{
    BConnectionProfile cert = BConnectionProfile::create("cert");
    cert.tlsUsePSK = false;
    cert.tlsCaCertFile = QStringLiteral("/etc/ca.pem");
    cert.tlsCertFile = QStringLiteral("/etc/cert.pem");
    cert.tlsKeyFile = QStringLiteral("/etc/key.pem");
    const BDirector::TLSConfig first = bTlsConfigFromProfile(cert);

    // The next profile has no files: none of the first may remain, and the
    // first configuration's files are not changed through shared pointers
    BConnectionProfile psk = BConnectionProfile::create("psk");
    const BDirector::TLSConfig second = bTlsConfigFromProfile(psk);
    QVERIFY(second.tlsCaCertFile->fileName().isEmpty());
#ifndef Q_OS_WINDOWS
    QVERIFY(second.tlsCertFile->fileName().isEmpty());
    QVERIFY(second.tlsKeyFile->fileName().isEmpty());
    QCOMPARE(first.tlsCertFile->fileName(), cert.tlsCertFile);
#endif
    QVERIFY(first.tlsCaCertFile != second.tlsCaCertFile);
}

void BConnectHelpersTest::waitCursorStaysBalanced()
{
    QVERIFY(!QGuiApplication::overrideCursor());
    {
        BWaitCursor cursor;
        cursor.show();
        cursor.show();      // connect, then a new pin before the first ended
        QVERIFY(QGuiApplication::overrideCursor());
        cursor.hide();
        QVERIFY(!QGuiApplication::overrideCursor());
        cursor.hide();      // error after disconnect: nothing more to pop
        QVERIFY(!QGuiApplication::overrideCursor());

        // A dialog's cursor below it is left alone
        QGuiApplication::setOverrideCursor(Qt::BusyCursor);
        cursor.show();
        cursor.hide();
        cursor.hide();
        QVERIFY(QGuiApplication::overrideCursor());
        QCOMPARE(QGuiApplication::overrideCursor()->shape(), Qt::BusyCursor);
        QGuiApplication::restoreOverrideCursor();

        cursor.show();
    }
    // Destroyed while shown (window closed during an attempt)
    QVERIFY(!QGuiApplication::overrideCursor());
}

QTEST_MAIN(BConnectHelpersTest)
#include "bconnecthelpers_test.moc"
