// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BTHEME_H
#define BTHEME_H

#include <QPalette>
#include <QString>

/**
 * @brief The colour themes: "light", "dark" and "system"
 *
 * The style sheets (the *.qss files in resources/themes) do not cover every part of
 * every widget; what they leave out is drawn from the palette. The main
 * window therefore uses the Fusion style, which draws everything from the
 * palette, and a palette with the colours of the style sheet, so that a
 * dark theme is dark on every desktop (issue #6).
 */
namespace BTheme {

/// "light" or "dark" for a setting; "system" follows @p systemIsDark
QString resolve(const QString &setting, bool systemIsDark);

/// True when the desktop asks for a dark colour scheme (Qt::ColorScheme)
bool systemIsDark();

/// Palette with the colours of the style sheet of a resolved theme
QPalette palette(const QString &resolvedTheme);

} // namespace BTheme

#endif // BTHEME_H
