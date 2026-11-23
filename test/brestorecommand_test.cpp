// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file brestorecommand_test.cpp
 * @brief The `restore` command of the restore wizard (P0-4)
 *
 * The expected splitting follows Bareos core/src/lib/scan.cc next_arg()
 * (double quotes group, a backslash escapes the next character), which is
 * re-implemented here to check that every value comes back as one argument.
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>

#include "jobs/brestorecommand.h"

using BRestoreCommand::Error;
using BRestoreCommand::Params;
using BRestoreCommand::Result;

namespace {

/// Bareos next_arg() + ParseArgs(): keyword -> value of every argument
QList<QPair<QString, QString>> parseLikeBareos(const QString &line)
{
    QList<QPair<QString, QString>> args;
    int i = 0;
    const int n = line.size();
    while (i < n) {
        while (i < n && line.at(i).isSpace()) ++i;
        if (i >= n) break;
        QString arg;
        bool inQuote = false;
        while (i < n) {
            const QChar c = line.at(i);
            if (c == '\\') { ++i; if (i < n) arg += line.at(i++); continue; }
            if (c == '"') { inQuote = !inQuote; ++i; continue; }
            if (!inQuote && c.isSpace()) { ++i; break; }
            arg += c; ++i;
        }
        const int eq = arg.indexOf('=');
        args.append(eq < 0 ? qMakePair(arg, QString()) : qMakePair(arg.left(eq), arg.mid(eq + 1)));
    }
    return args;
}

QString valueOf(const QList<QPair<QString, QString>> &args, const QString &key)
{
    for (const auto &a : args)
        if (a.first == key) return a.second;
    return QStringLiteral("<missing>");
}

Params basic()
{
    Params p;
    p.table = QStringLiteral("b2123456");
    p.backupClient = QStringLiteral("web-fd");
    p.where = QStringLiteral("/tmp/bareos-restores");
    p.replace = QStringLiteral("always");
    return p;
}

} // namespace

class BRestoreCommandTest : public QObject
{
    Q_OBJECT

private slots:
    void basicCommand();
    void restoreJobIsPassed();
    void targetClientGoesToRestoreClient();
    void pathWithSpacesStaysOneArgument();
    void quotesAndBackslashesAreEscaped();
    void valueCannotAddKeywords();
    void controlCharactersAreRejected();
    void invalidInputIsRejected();
};

void BRestoreCommandTest::basicCommand()
{
    const Result r = BRestoreCommand::build(basic());
    QVERIFY(r.ok());
    QCOMPARE(r.arguments, QStringLiteral(
        "file=?b2123456 client=\"web-fd\" restoreclient=\"web-fd\" "
        "where=\"/tmp/bareos-restores\" replace=always yes"));
}

void BRestoreCommandTest::restoreJobIsPassed()
{
    Params p = basic();
    p.restoreJob = QStringLiteral("Restore Files");
    const Result r = BRestoreCommand::build(p);
    QVERIFY(r.ok());
    QVERIFY(r.arguments.contains(QStringLiteral("restorejob=\"Restore Files\"")));
    QCOMPARE(valueOf(parseLikeBareos(r.arguments), "restorejob"), QStringLiteral("Restore Files"));
}

void BRestoreCommandTest::targetClientGoesToRestoreClient()
{
    Params p = basic();
    p.restoreClient = QStringLiteral("other-fd");
    const auto args = parseLikeBareos(BRestoreCommand::build(p).arguments);
    // client= stays the client of the backup: Bareos looks it up in the catalog
    QCOMPARE(valueOf(args, "client"), QStringLiteral("web-fd"));
    QCOMPARE(valueOf(args, "restoreclient"), QStringLiteral("other-fd"));
}

void BRestoreCommandTest::pathWithSpacesStaysOneArgument()
{
    Params p = basic();
    p.where = QStringLiteral("/srv/restore here");
    p.backupClient = QStringLiteral("my client");
    const auto args = parseLikeBareos(BRestoreCommand::build(p).arguments);
    QCOMPARE(valueOf(args, "where"), QStringLiteral("/srv/restore here"));
    QCOMPARE(valueOf(args, "client"), QStringLiteral("my client"));
    QCOMPARE(args.size(), 6);   // file client restoreclient where replace yes
}

void BRestoreCommandTest::quotesAndBackslashesAreEscaped()
{
    Params p = basic();
    p.where = QStringLiteral("C:\\Restore \"new\"");
    const Result r = BRestoreCommand::build(p);
    QVERIFY(r.ok());
    QVERIFY(r.arguments.contains(QStringLiteral("where=\"C:\\\\Restore \\\"new\\\"\"")));
    QCOMPARE(valueOf(parseLikeBareos(r.arguments), "where"), QStringLiteral("C:\\Restore \"new\""));
}

void BRestoreCommandTest::valueCannotAddKeywords()
{
    Params p = basic();
    p.where = QStringLiteral("/tmp\" restoreclient=evil-fd \"x");
    const auto args = parseLikeBareos(BRestoreCommand::build(p).arguments);
    QCOMPARE(valueOf(args, "restoreclient"), QStringLiteral("web-fd"));
    QCOMPARE(valueOf(args, "where"), p.where);
}

void BRestoreCommandTest::controlCharactersAreRejected()
{
    Params p = basic();
    p.where = QStringLiteral("/tmp\nrestore all");
    const Result r = BRestoreCommand::build(p);
    QCOMPARE(r.error, Error::ControlCharacter);
    QCOMPARE(r.field, QStringLiteral("where"));
    QVERIFY(r.arguments.isEmpty());
}

void BRestoreCommandTest::invalidInputIsRejected()
{
    Params p = basic();
    p.table = QStringLiteral("b2 1");
    QCOMPARE(BRestoreCommand::build(p).error, Error::InvalidTable);

    p = basic();
    p.backupClient.clear();
    QCOMPARE(BRestoreCommand::build(p).error, Error::MissingClient);

    p = basic();
    p.where = QStringLiteral("  ");
    QCOMPARE(BRestoreCommand::build(p).error, Error::MissingWhere);

    p = basic();
    p.replace = QStringLiteral("always yes");
    QCOMPARE(BRestoreCommand::build(p).error, Error::InvalidReplace);
}

QTEST_MAIN(BRestoreCommandTest)
#include "brestorecommand_test.moc"
