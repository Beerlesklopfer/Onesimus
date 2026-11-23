// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

// BDirectiveSchema::validateValue(): more than integers are checked (P1-30)

#include <QtTest>

#include "config/bdirectiveschema.h"

class BDirectiveSchemaTest : public QObject
{
    Q_OBJECT

private slots:
    void booleans();
    void sizes();
    void times();
    void otherTypesAreNotChecked();
};

void BDirectiveSchemaTest::booleans()
{
    for (const char *ok : {"yes", "No", "TRUE", "false", "on", "off", "1", "0"})
        QVERIFY2(BDirectiveSchema::validateValue("boolean", "Enabled", ok).isEmpty(), ok);
    QVERIFY(!BDirectiveSchema::validateValue("boolean", "Enabled", "maybe").isEmpty());
}

void BDirectiveSchemaTest::sizes()
{
    for (const char *ok : {"500", "500 mb", "2gb", "1.5 GB", "10k", "3 T"})
        QVERIFY2(BDirectiveSchema::validateValue("size", "Maximum Volume Bytes", ok).isEmpty(), ok);
    for (const char *bad : {"mb", "big", "10 parsecs"})
        QVERIFY2(!BDirectiveSchema::validateValue("size", "Maximum Volume Bytes", bad).isEmpty(), bad);
}

void BDirectiveSchemaTest::times()
{
    for (const char *ok : {"3600", "30 days", "1 hour 30 min", "6 months", "1y"})
        QVERIFY2(BDirectiveSchema::validateValue("time", "Volume Retention", ok).isEmpty(), ok);
    for (const char *bad : {"days", "soon", "-5 days"})
        QVERIFY2(!BDirectiveSchema::validateValue("time", "Volume Retention", bad).isEmpty(), bad);
}

void BDirectiveSchemaTest::otherTypesAreNotChecked()
{
    QVERIFY(BDirectiveSchema::validateValue("string", "Description", "anything at all").isEmpty());
    QVERIFY(BDirectiveSchema::validateValue("boolean", "Enabled", "").isEmpty());
}

QTEST_GUILESS_MAIN(BDirectiveSchemaTest)
#include "bdirectiveschema_test.moc"
