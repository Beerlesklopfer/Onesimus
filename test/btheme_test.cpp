// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

// Themes (issue #6, P1-3): "system" follows the desktop, and the palette
// of each theme has readable text on its backgrounds

#include <QtTest>

#include "btheme.h"

namespace {
/// WCAG contrast ratio of two colours
double contrast(const QColor &a, const QColor &b)
{
    auto lum = [](const QColor &c) {
        auto ch = [](double v) {
            v /= 255.0;
            return v <= 0.03928 ? v / 12.92 : std::pow((v + 0.055) / 1.055, 2.4);
        };
        return 0.2126 * ch(c.red()) + 0.7152 * ch(c.green()) + 0.0722 * ch(c.blue());
    };
    const double la = lum(a) + 0.05;
    const double lb = lum(b) + 0.05;
    return la > lb ? la / lb : lb / la;
}
} // namespace

class BThemeTest : public QObject
{
    Q_OBJECT

private slots:
    void systemFollowsTheDesktop();
    void paletteIsReadable_data();
    void paletteIsReadable();
};

void BThemeTest::systemFollowsTheDesktop()
{
    QCOMPARE(BTheme::resolve("system", true), QStringLiteral("dark"));
    QCOMPARE(BTheme::resolve("system", false), QStringLiteral("light"));
    QCOMPARE(BTheme::resolve("light", true), QStringLiteral("light"));
    QCOMPARE(BTheme::resolve("dark", false), QStringLiteral("dark"));
    QCOMPARE(BTheme::resolve(QString(), false), QStringLiteral("dark"));    // the default
}

void BThemeTest::paletteIsReadable_data()
{
    QTest::addColumn<QString>("theme");
    QTest::newRow("dark") << QStringLiteral("dark");
    QTest::newRow("light") << QStringLiteral("light");
}

void BThemeTest::paletteIsReadable()
{
    QFETCH(QString, theme);
    const QPalette p = BTheme::palette(theme);
    QVERIFY(contrast(p.color(QPalette::Text), p.color(QPalette::Base)) >= 7.0);
    QVERIFY(contrast(p.color(QPalette::WindowText), p.color(QPalette::Window)) >= 7.0);
    QVERIFY(contrast(p.color(QPalette::ButtonText), p.color(QPalette::Button)) >= 4.5);
    QVERIFY(contrast(p.color(QPalette::HighlightedText), p.color(QPalette::Highlight)) >= 4.5);
    QVERIFY(contrast(p.color(QPalette::ToolTipText), p.color(QPalette::ToolTipBase)) >= 4.5);
    const bool darkBase = p.color(QPalette::Base).lightness() < 128;
    QCOMPARE(darkBase, theme == QLatin1String("dark"));
}

QTEST_MAIN(BThemeTest)
#include "btheme_test.moc"
