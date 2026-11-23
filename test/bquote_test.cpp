// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bquote_test.cpp
 * @brief The one quoting helper: escaping, control characters, Bareos name
 *        rule, and a round trip through ports of the two Bareos readers
 *        (next_arg() for command lines, the quoted-string state of the
 *        configuration lexer)
 */

#include "bquote.h"

#include <QStringList>
#include <QtTest>

namespace {

// Port of next_arg() (bareos core/src/lib/scan.cc, Release/25.1.2): a
// backslash takes the next character, a double quote toggles quoting,
// blanks outside quotes end the argument
QStringList bareosArgs(const QString &line)
{
    QStringList args;
    int p = 0;
    const int n = line.size();
    while (true) {
        while (p < n && line[p].isSpace()) ++p;
        if (p >= n) break;
        QString arg;
        bool inQuote = false;
        while (p < n) {
            const QChar c = line[p];
            if (c == QLatin1Char('\\')) {
                ++p;
                if (p < n) arg += line[p++];
                continue;
            }
            if (c == QLatin1Char('"')) {
                ++p;
                inQuote = !inQuote;
                continue;
            }
            if (!inQuote && c.isSpace()) {
                ++p;
                break;
            }
            arg += c;
            ++p;
        }
        args << arg;
    }
    return args;
}

// The lexer's quoted-string state (core/src/lib/lex.cc): starts after the
// opening quote; returns the value and the index after the closing quote
QString lexQuoted(const QString &text, int *end)
{
    QString out;
    bool esc = false;
    for (int i = 1; i < text.size(); ++i) {
        const QChar c = text[i];
        if (esc) { out += c; esc = false; continue; }
        if (c == QLatin1Char('\\')) { esc = true; continue; }
        if (c == QLatin1Char('"')) { *end = i + 1; return out; }
        out += c;
    }
    *end = -1;
    return out;
}

} // namespace

class BQuoteTest : public QObject
{
    Q_OBJECT

private slots:
    void escaping_data()
    {
        QTest::addColumn<QString>("value");
        QTest::addColumn<QString>("expected");
        QTest::newRow("plain") << "Full" << "\"Full\"";
        QTest::newRow("empty") << "" << "\"\"";
        QTest::newRow("blank") << "My Job" << "\"My Job\"";
        QTest::newRow("quote") << "a\"b" << "\"a\\\"b\"";
        QTest::newRow("backslash") << "C:\\bareos" << "\"C:\\\\bareos\"";
        QTest::newRow("trailing backslash") << "x\\" << "\"x\\\\\"";
        QTest::newRow("keyword inside") << "x yes job=other" << "\"x yes job=other\"";
        QTest::newRow("umlaut") << "Jörg" << "\"Jörg\"";
    }

    void escaping()
    {
        QFETCH(QString, value);
        QFETCH(QString, expected);
        bool ok = false;
        QCOMPARE(BQuote::quoted(value, &ok), expected);
        QVERIFY(ok);
    }

    void controlCharactersReported_data()
    {
        QTest::addColumn<QString>("value");
        QTest::newRow("newline") << "a\nb";
        QTest::newRow("cr") << "a\rb";
        QTest::newRow("tab") << "a\tb";
        QTest::newRow("nul") << QString(QChar(0));
        QTest::newRow("del") << QString(QChar(0x7f));
    }

    void controlCharactersReported()
    {
        QFETCH(QString, value);
        bool ok = true;
        BQuote::quoted(value, &ok);
        QVERIFY(!ok);
        QVERIFY(BQuote::hasControlCharacter(value));
    }

    void commandLineRoundTrip_data()
    {
        QTest::addColumn<QString>("value");
        for (const char *v : { "Full", "My Job", "pass\"word", "back\\slash", "\\\"\\\"", "a=b c=d",
                               "x\\", "\"", "  lead and trail  ", "yes" })
            QTest::newRow(v) << QString::fromUtf8(v);
    }

    void commandLineRoundTrip()
    {
        QFETCH(QString, value);
        const QString line = QStringLiteral("add console name=") + BQuote::quoted(value)
                           + QStringLiteral(" password=") + BQuote::quoted(value) + QStringLiteral(" yes");
        const QStringList args = bareosArgs(line);
        QCOMPARE(args.size(), 5);
        QCOMPARE(args[2], QStringLiteral("name=") + value);
        QCOMPARE(args[3], QStringLiteral("password=") + value);
        QCOMPARE(args[4], QStringLiteral("yes"));
    }

    void resourceTextRoundTrip()
    {
        for (const QString &value : { QStringLiteral("/home/a b"), QStringLiteral("p\"w\\"),
                                      QStringLiteral("\\\\server\\share"), QString() }) {
            const QString text = BQuote::quoted(value) + QStringLiteral("  # comment");
            int end = 0;
            QCOMPARE(lexQuoted(text, &end), value);
            QCOMPARE(text.mid(end), QStringLiteral("  # comment"));
            QCOMPARE(BQuote::unescape(BQuote::quoted(value).mid(1).chopped(1)), value);
        }
    }

    void configureAdd()
    {
        QCOMPARE(BQuote::configureAddForbidden(QStringLiteral("Full Set /home")), QChar());
        QCOMPARE(BQuote::configureAddForbidden(QStringLiteral("a;b")), QChar(';'));
        QCOMPARE(BQuote::configureAddForbidden(QStringLiteral("@/etc/x")), QChar('@'));
        QCOMPARE(BQuote::configureAddForbidden(QStringLiteral("pa\"ss")), QChar('"'));
        QCOMPARE(BQuote::configureAddForbidden(QStringLiteral("C:\\x")), QChar('\\'));
        QCOMPARE(BQuote::configureAddForbidden(QStringLiteral("a\nb")), QChar('\n'));
        // A Run directive is written as it is: quotes inside are its own
        QCOMPARE(BQuote::configureAddForbidden(QStringLiteral("Pool=\"My Pool\" at 1:00"), false), QChar());
        QCOMPARE(BQuote::configureAddForbidden(QStringLiteral("at 1:00;"), false), QChar(';'));
    }

    void names_data()
    {
        QTest::addColumn<QString>("name");
        QTest::addColumn<bool>("valid");
        QTest::newRow("plain") << "backup-fd" << true;
        QTest::newRow("blank inside") << "Full Set" << true;
        QTest::newRow("all specials") << "a:b.c-d_e/f g" << true;
        QTest::newRow("empty") << "" << false;
        QTest::newRow("leading blank") << " x" << false;
        QTest::newRow("trailing blank") << "x " << false;
        QTest::newRow("quote") << "a\"b" << false;
        QTest::newRow("backslash") << "a\\b" << false;
        QTest::newRow("umlaut") << "Jörg" << false;
        QTest::newRow("newline") << "a\nb" << false;
        QTest::newRow("127") << QString(127, QLatin1Char('a')) << true;
        QTest::newRow("128") << QString(128, QLatin1Char('a')) << false;
    }

    void names()
    {
        QFETCH(QString, name);
        QFETCH(bool, valid);
        QCOMPARE(BQuote::isValidName(name), valid);
    }
};

QTEST_GUILESS_MAIN(BQuoteTest)
#include "bquote_test.moc"
