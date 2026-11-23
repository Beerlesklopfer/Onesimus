// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bthirdpartynotices_test.cpp
 * @brief The third-party notices compiled into the program
 *
 * THIRD-PARTY-NOTICES.md, LICENSE and the *.txt files in LICENSES are Qt resources under
 * :/legal/ (the same list CMakeLists.txt installs into every package). Every
 * licence file the notices link must be there, every licence file there
 * must be linked, and the dialog of the About box shows them and follows a
 * link to a licence. Offscreen, no Director. With
 * ONESIMUS_NOTICES_SCREENSHOT_DIR set to a directory the dialog test saves a
 * picture of the notices and of a licence there.
 */

#include <QtTest>

#include <QComboBox>
#include <QRegularExpression>
#include <QSet>
#include <QTextBrowser>

#include "bthirdpartynotices.h"

class BThirdPartyNoticesTest : public QObject
{
    Q_OBJECT

private slots:
    void resourcesArePresent();
    void linksAndFilesMatch();
    void noticesNameTheComponents();
    void linkResolution();
    void dialogShowsDocuments();
};

void BThirdPartyNoticesTest::resourcesArePresent()
{
    const QStringList docs = BThirdPartyNoticesDialog::documents();
    QVERIFY2(!docs.isEmpty(), "no :/legal/ resources");
    QCOMPARE(docs.first(), QStringLiteral("THIRD-PARTY-NOTICES.md"));
    for (const char *name : {"LICENSE", "LICENSES/Apache-2.0.txt", "LICENSES/GPL-3.0-only.txt",
                             "LICENSES/LGPL-3.0-only.txt", "LICENSES/MIT.txt"}) {
        QVERIFY2(docs.contains(QLatin1String(name)), name);
        QVERIFY2(BThirdPartyNoticesDialog::documentText(QLatin1String(name)).size() > 500, name);
    }
}

void BThirdPartyNoticesTest::linksAndFilesMatch()
{
    const QString notices = BThirdPartyNoticesDialog::documentText(QStringLiteral("THIRD-PARTY-NOTICES.md"));
    static const QRegularExpression re(QStringLiteral("LICENSES/[A-Za-z0-9.+-]+\\.txt"));
    QSet<QString> linked;
    for (auto it = re.globalMatch(notices); it.hasNext();)
        linked.insert(it.next().captured(0));
    QSet<QString> shipped;
    for (const QString &doc : BThirdPartyNoticesDialog::documents())
        if (doc.startsWith(QLatin1String("LICENSES/")))
            shipped.insert(doc);
    QVERIFY(!linked.isEmpty());
    for (const QString &l : linked)
        QVERIFY2(shipped.contains(l), qPrintable(l + QStringLiteral(" is named in the notices but not shipped")));
    for (const QString &s : shipped)
        QVERIFY2(linked.contains(s), qPrintable(s + QStringLiteral(" is shipped but not named in the notices")));
}

void BThirdPartyNoticesTest::noticesNameTheComponents()
{
    const QString notices = BThirdPartyNoticesDialog::documentText(QStringLiteral("THIRD-PARTY-NOTICES.md"));
    for (const char *word : {"OpenSSL", "Apache-2.0", "Qt", "LGPL-3.0-only", "QtGrpc", "GPL-3.0-only",
                             "Feather", "MIT", "/usr/share/doc/onesimus/", "Contents/Resources"})
        QVERIFY2(notices.contains(QLatin1String(word)), word);
}

void BThirdPartyNoticesTest::linkResolution()
{
    QCOMPARE(BThirdPartyNoticesDialog::documentForLink(QUrl(QStringLiteral("LICENSES/MIT.txt"))),
             QStringLiteral("LICENSES/MIT.txt"));
    QCOMPARE(BThirdPartyNoticesDialog::documentForLink(QUrl(QStringLiteral("./LICENSE"))),
             QStringLiteral("LICENSE"));
    QVERIFY(BThirdPartyNoticesDialog::documentForLink(QUrl(QStringLiteral("https://www.openssl.org/"))).isEmpty());
    QVERIFY(BThirdPartyNoticesDialog::documentForLink(QUrl(QStringLiteral("LICENSES/none.txt"))).isEmpty());
}

void BThirdPartyNoticesTest::dialogShowsDocuments()
{
    BThirdPartyNoticesDialog dialog;
    const QString shots = qEnvironmentVariable("ONESIMUS_NOTICES_SCREENSHOT_DIR");
    auto save = [&](const QString &name) {
        if (shots.isEmpty())
            return;
        dialog.show();
        QCoreApplication::processEvents();
        dialog.grab().save(QDir(shots).filePath(name));
    };
    auto *browser = dialog.findChild<QTextBrowser *>();
    QVERIFY(browser);
    QCOMPARE(dialog.currentDocument(), QStringLiteral("THIRD-PARTY-NOTICES.md"));
    QVERIFY(browser->toPlainText().contains(QLatin1String("OpenSSL")));
    QVERIFY(!browser->toPlainText().contains(QLatin1String("SPDX-License-Identifier")));
    save(QStringLiteral("notices.png"));

    // A click on a licence link in the notices opens that licence
    emit browser->anchorClicked(QUrl(QStringLiteral("LICENSES/Apache-2.0.txt")));
    QCOMPARE(dialog.currentDocument(), QStringLiteral("LICENSES/Apache-2.0.txt"));
    QVERIFY(browser->toPlainText().contains(QLatin1String("Apache License")));
    save(QStringLiteral("apache.png"));

    QVERIFY(dialog.showDocument(QStringLiteral("LICENSE")));
    QVERIFY(browser->toPlainText().contains(QLatin1String("GNU AFFERO GENERAL PUBLIC LICENSE")));
    QVERIFY(!dialog.showDocument(QStringLiteral("LICENSES/none.txt")));
    QCOMPARE(dialog.currentDocument(), QStringLiteral("LICENSE"));

    auto *combo = dialog.findChild<QComboBox *>();
    QVERIFY(combo);
    QCOMPARE(combo->count(), BThirdPartyNoticesDialog::documents().size());
}

QTEST_MAIN(BThirdPartyNoticesTest)
#include "bthirdpartynotices_test.moc"
