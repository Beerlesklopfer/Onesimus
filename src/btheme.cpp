// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "btheme.h"

#include <QGuiApplication>
#include <QStyleHints>

namespace BTheme {

QString resolve(const QString &setting, bool systemIsDark)
{
    if (setting == QLatin1String("light"))
        return QStringLiteral("light");
    if (setting == QLatin1String("system"))
        return systemIsDark ? QStringLiteral("dark") : QStringLiteral("light");
    return QStringLiteral("dark");     // the default, also for unknown values
}

bool systemIsDark()
{
    return QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
}

QPalette palette(const QString &resolvedTheme)
{
    // The colours of resources/themes/dark.qss and light.qss
    const bool dark = resolvedTheme == QLatin1String("dark");
    const QColor window = dark ? QColor(0x1e, 0x1e, 0x1e) : QColor(0xf5, 0xf5, 0xf5);
    const QColor base = dark ? QColor(0x1e, 0x1e, 0x1e) : QColor(0xff, 0xff, 0xff);
    const QColor alternate = dark ? QColor(0x25, 0x25, 0x26) : QColor(0xf0, 0xf0, 0xf0);
    const QColor text = dark ? QColor(0xd4, 0xd4, 0xd4) : QColor(0x00, 0x00, 0x00);
    const QColor button = dark ? QColor(0x3e, 0x3e, 0x42) : QColor(0xe0, 0xe0, 0xe0);
    const QColor highlight = dark ? QColor(0x09, 0x47, 0x71) : QColor(0x00, 0x78, 0xd4);
    const QColor disabled = dark ? QColor(0x80, 0x80, 0x80) : QColor(0x90, 0x90, 0x90);
    const QColor link = dark ? QColor(0x4e, 0xa1, 0xff) : QColor(0x00, 0x5a, 0x9e);

    QPalette p;
    p.setColor(QPalette::Window, window);
    p.setColor(QPalette::WindowText, text);
    p.setColor(QPalette::Base, base);
    p.setColor(QPalette::AlternateBase, alternate);
    p.setColor(QPalette::Text, text);
    p.setColor(QPalette::Button, button);
    p.setColor(QPalette::ButtonText, text);
    p.setColor(QPalette::BrightText, Qt::white);
    p.setColor(QPalette::Highlight, highlight);
    p.setColor(QPalette::HighlightedText, Qt::white);
    p.setColor(QPalette::ToolTipBase, alternate);
    p.setColor(QPalette::ToolTipText, text);
    p.setColor(QPalette::PlaceholderText, disabled);
    p.setColor(QPalette::Link, link);
    p.setColor(QPalette::LinkVisited, link);
    p.setColor(QPalette::Light, dark ? button.lighter(130) : QColor(Qt::white));
    p.setColor(QPalette::Midlight, dark ? button.lighter(115) : button.lighter(110));
    p.setColor(QPalette::Mid, dark ? button.darker(130) : button.darker(130));
    p.setColor(QPalette::Dark, dark ? button.darker(160) : button.darker(160));
    p.setColor(QPalette::Shadow, Qt::black);
    for (QPalette::ColorRole role : {QPalette::WindowText, QPalette::Text, QPalette::ButtonText})
        p.setColor(QPalette::Disabled, role, disabled);
    return p;
}

} // namespace BTheme
