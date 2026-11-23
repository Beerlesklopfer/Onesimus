// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BSTATUSCHIP_H
#define BSTATUSCHIP_H

#include <QColor>
#include <QString>
#include <Qt>

#include "clients/bclientstatus.h"

/**
 * @brief What a status cell says and which semantic colour it gets, without widgets
 *
 * A status is shown as a chip: a short code (a letter or a sign), a short
 * word, and a tone. The tone names the meaning (success, warning, error,
 * ...); the colour of a tone comes from the theme (BStatusPalette reads the
 * tokens of resources/themes/<theme>.qss), so code never picks a colour that
 * fits one theme only. Colour is never the only cue: the code and the word
 * differ for every state.
 *
 * Green (Tone::Success) is used for one thing only: a job that terminated
 * normally (Bareos JobStatus 'T'). Nothing that is a guess or merely
 * recent is green. The one other use: a criterion of the 3-2-1 verifier
 * (B321Dialog) that the catalog shows as met.
 */
namespace BStatus {

enum class Tone {
    Neutral,    ///< Canceled, never, unknown, nothing to report
    Success,    ///< Verified success: job status 'T' only
    Warning,    ///< Done with warnings, blocked, differences, incomplete, stale
    Error,      ///< Error or fatal error
    Running,    ///< Working right now
    Waiting,    ///< Created or queued, waiting for a resource or a time
    Info        ///< Usable or recently active, but not a verified success
};

constexpr int ToneCount = 7;

/// Parts of a tone's colour
enum class Part {
    Background,     ///< Fill of the chip
    Foreground,     ///< Text on the fill
    Accent          ///< Border of the chip, chunk of a progress bar
};

/// Model roles read by BStatusDelegate. Far from Qt::UserRole, where the
/// models keep their sort role.
enum Role {
    ToneRole = Qt::UserRole + 0x5170,   ///< int(Tone)
    CodeRole,                           ///< Letter or sign at the start of the chip
    ChipColorRole                       ///< Explicit fill (QColor) instead of a tone, e.g. a backup level
};

struct Chip {
    Tone tone = Tone::Neutral;
    QString code;       ///< "T", "E", "?" ...
    QString text;       ///< Short word for the cell: "OK", "Error", "Wait mount"
    QString fullText;   ///< Full word for screen readers and tool tips
};

/// Bareos JobStatus letter (core/src/include/job_status.h) as a chip
Chip jobStatus(const QString &letter);

/// Full description of a Bareos JobStatus letter ("Terminated normally")
QString jobStatusText(const QString &letter);

/// True for the letters of a job that ended in an error (E, e, f)
bool isJobError(const QString &letter);

/// Age of a client's last backup or contact as a chip; @p label is the cell text
Chip clientActivity(BClientStatus::Activity activity, const QString &label);

/// Tone of a volume status ("Append", "Full", "Error", ...)
Tone volumeTone(const QString &status);

/// Token name of a tone, as used in the themes: "success", "warning", ...
QString toneName(Tone tone);

/**
 * @brief Colour of a tone
 *
 * The theme's token if one was set (setColor(), done by BStatusPalette when
 * the theme is applied), otherwise the built-in value for a dark or light
 * background, chosen from the application palette.
 */
QColor color(Tone tone, Part part);

/// Built-in colour of a tone for a dark or light theme (what the themes set too)
QColor defaultColor(Tone tone, Part part, bool dark);

/// Sets the theme's token; an invalid colour returns to the built-in value
void setColor(Tone tone, Part part, const QColor &color);

/// Forgets every token set by setColor()
void resetColors();

} // namespace BStatus

#endif // BSTATUSCHIP_H
