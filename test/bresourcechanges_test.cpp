/**
 * @file bresourcechanges_test.cpp
 * @brief What the resource form changed, as directive changes
 */

#include <QtTest>

#include "config/bconfigparser.h"
#include "config/bresourcechanges.h"

class BResourceChangesTest : public QObject
{
    Q_OBJECT

private:
    // As "show jobs" reports it: lower case keys, inherited values included
    static BConfigResource shown()
    {
        BConfigResource r(QStringLiteral("Job"), QStringLiteral("forgejo"));
        r.setValue(QStringLiteral("client"), BConfigValue(QStringLiteral("PDC-fd")));
        r.setValue(QStringLiteral("fileset"), BConfigValue(QStringLiteral("Forgejo")));
        r.setValue(QStringLiteral("enabled"), BConfigValue(QStringLiteral("true")));
        r.setValue(QStringLiteral("priority"), BConfigValue(QStringLiteral("10")));
        r.setValue(QStringLiteral("runscript"),
                   BConfigValue(QStringList{QStringLiteral("a.sh"), QStringLiteral("b.sh")}));
        return r;
    }
    // As the form returns it: schema spelling, empty fields left out
    static BConfigResource form()
    {
        BConfigResource r(QStringLiteral("Job"));
        r.setValue(QStringLiteral("Name"), BConfigValue(QStringLiteral("forgejo")));
        r.setValue(QStringLiteral("Client"), BConfigValue(QStringLiteral("PDC-fd")));
        r.setValue(QStringLiteral("FileSet"), BConfigValue(QStringLiteral("Forgejo")));
        r.setValue(QStringLiteral("Enabled"), BConfigValue(QStringLiteral("yes")));
        r.setValue(QStringLiteral("Priority"), BConfigValue(QStringLiteral("10")));
        return r;
    }
    static QStringList fields()
    {
        return {QStringLiteral("Name"), QStringLiteral("Client"), QStringLiteral("FileSet"),
                QStringLiteral("Enabled"), QStringLiteral("Priority"), QStringLiteral("Pool"),
                QStringLiteral("Maximum Concurrent Jobs"), QStringLiteral("RunScript")};
    }

private slots:
    void unchangedFormIsNoChange()
    {
        const BResourceChanges c = BResourceChanges::between(shown(), form(), fields());
        QVERIFY2(c.isEmpty(), qPrintable(c.descriptions.join(QLatin1Char('\n'))));
        QVERIFY(c.skipped.isEmpty());
    }

    void changedValueKeepsSchemaSpelling()
    {
        BConfigResource after = form();
        after.setValue(QStringLiteral("Client"), BConfigValue(QStringLiteral("CLOUD-fd")));
        const BResourceChanges c = BResourceChanges::between(shown(), after, fields());
        QCOMPARE(c.changes.size(), 1);
        QCOMPARE(c.changes.first().key, QStringLiteral("Client"));
        QCOMPARE(c.changes.first().value, QStringLiteral("CLOUD-fd"));
        QVERIFY(!c.changes.first().remove);
        QCOMPARE(c.descriptions.first(), QStringLiteral("Client: PDC-fd -> CLOUD-fd"));
    }

    void defaultShownByTheFormIsNoChange()
    {
        BConfigResource after = form();
        after.setValue(QStringLiteral("Maximum Concurrent Jobs"), BConfigValue(QStringLiteral("1")));
        auto defaults = [](const QString &d) {
            return d == QLatin1String("Maximum Concurrent Jobs") ? QStringLiteral("1") : QString();
        };
        QVERIFY(BResourceChanges::between(shown(), after, fields(), defaults).isEmpty());

        after.setValue(QStringLiteral("Maximum Concurrent Jobs"), BConfigValue(QStringLiteral("3")));
        const BResourceChanges c = BResourceChanges::between(shown(), after, fields(), defaults);
        QCOMPARE(c.changes.size(), 1);
        QCOMPARE(c.changes.first().key, QStringLiteral("Maximum Concurrent Jobs"));
    }

    void emptiedFieldRemovesTheDirective()
    {
        BConfigResource after(QStringLiteral("Job"));
        const BConfigResource f = form();
        for (const QString &key : f.keys()) {
            if (key.compare(QLatin1String("Priority"), Qt::CaseInsensitive) != 0)
                after.setValue(key, f.value(key));
        }
        const BResourceChanges c = BResourceChanges::between(shown(), after, fields());
        QCOMPARE(c.changes.size(), 1);
        QVERIFY(c.changes.first().remove);
        QCOMPARE(c.changes.first().key, QStringLiteral("Priority"));
    }

    void keyWithoutFieldIsNotRemoved()
    {
        // "runscript" has no field in the form: missing in "after" is no removal
        const BResourceChanges c = BResourceChanges::between(shown(), form(), fields());
        QVERIFY(c.isEmpty());
    }

    void listValueIsNotWrittenButNamed()
    {
        BConfigResource after = form();
        after.setValue(QStringLiteral("RunScript"), BConfigValue(QStringLiteral("c.sh")));
        const BResourceChanges c = BResourceChanges::between(shown(), after, fields());
        QVERIFY(c.isEmpty());
        QCOMPARE(c.skipped, QStringList{QStringLiteral("RunScript")});
    }

    void comparableValues()
    {
        QCOMPARE(BResourceChanges::comparable(QStringLiteral(" \"x y\" ")), QStringLiteral("x y"));
        QCOMPARE(BResourceChanges::comparable(QStringLiteral("True")), QStringLiteral("yes"));
        QCOMPARE(BResourceChanges::comparable(QStringLiteral("no")), QStringLiteral("no"));
    }
};

QTEST_GUILESS_MAIN(BResourceChangesTest)
#include "bresourcechanges_test.moc"
