// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "bstatuschip.h"

#include <QCoreApplication>
#include <QGuiApplication>
#include <QPalette>

namespace BStatus {

namespace {

QString trText(const char *text)
{
    return QCoreApplication::translate("BStatus", text);
}

struct JobEntry {
    char letter;
    Tone tone;
    const char *shortText;
    const char *fullText;
};

// Every JobStatus of Bareos 25 (core/src/include/job_status.h)
const JobEntry kJobEntries[] = {
    {'T', Tone::Success, QT_TRANSLATE_NOOP("BStatus", "OK"), QT_TRANSLATE_NOOP("BStatus", "Terminated normally")},
    {'W', Tone::Warning, QT_TRANSLATE_NOOP("BStatus", "Warnings"), QT_TRANSLATE_NOOP("BStatus", "Terminated with warnings")},
    {'E', Tone::Error, QT_TRANSLATE_NOOP("BStatus", "Error"), QT_TRANSLATE_NOOP("BStatus", "Terminated in error")},
    {'e', Tone::Error, QT_TRANSLATE_NOOP("BStatus", "Errors"), QT_TRANSLATE_NOOP("BStatus", "Non-fatal error")},
    {'f', Tone::Error, QT_TRANSLATE_NOOP("BStatus", "Fatal"), QT_TRANSLATE_NOOP("BStatus", "Fatal error")},
    {'A', Tone::Neutral, QT_TRANSLATE_NOOP("BStatus", "Canceled"), QT_TRANSLATE_NOOP("BStatus", "Canceled by user")},
    {'R', Tone::Running, QT_TRANSLATE_NOOP("BStatus", "Running"), QT_TRANSLATE_NOOP("BStatus", "Running")},
    {'L', Tone::Running, QT_TRANSLATE_NOOP("BStatus", "Committing"), QT_TRANSLATE_NOOP("BStatus", "Committing data (last despool)")},
    {'l', Tone::Running, QT_TRANSLATE_NOOP("BStatus", "Despooling"), QT_TRANSLATE_NOOP("BStatus", "Despooling data")},
    {'C', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Created"), QT_TRANSLATE_NOOP("BStatus", "Created, not yet running")},
    {'F', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Wait FD"), QT_TRANSLATE_NOOP("BStatus", "Waiting on the File daemon")},
    {'S', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Wait SD"), QT_TRANSLATE_NOOP("BStatus", "Waiting on the Storage daemon")},
    {'m', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Wait media"), QT_TRANSLATE_NOOP("BStatus", "Waiting for new media")},
    {'M', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Wait mount"), QT_TRANSLATE_NOOP("BStatus", "Waiting for mount")},
    {'s', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Wait storage"), QT_TRANSLATE_NOOP("BStatus", "Waiting for a storage resource")},
    {'j', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Wait job"), QT_TRANSLATE_NOOP("BStatus", "Waiting for a job resource")},
    {'c', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Wait client"), QT_TRANSLATE_NOOP("BStatus", "Waiting for a client resource")},
    {'d', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Wait max jobs"), QT_TRANSLATE_NOOP("BStatus", "Waiting for maximum jobs")},
    {'t', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Wait start"), QT_TRANSLATE_NOOP("BStatus", "Waiting for the start time")},
    {'p', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Wait priority"), QT_TRANSLATE_NOOP("BStatus", "Waiting for higher priority jobs to finish")},
    {'q', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Wait device"), QT_TRANSLATE_NOOP("BStatus", "Queued, waiting for a device")},
    {'a', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Despool attr."), QT_TRANSLATE_NOOP("BStatus", "Storage daemon despooling attributes")},
    {'i', Tone::Waiting, QT_TRANSLATE_NOOP("BStatus", "Inserting"), QT_TRANSLATE_NOOP("BStatus", "Doing batch insert of file records")},
    {'B', Tone::Warning, QT_TRANSLATE_NOOP("BStatus", "Blocked"), QT_TRANSLATE_NOOP("BStatus", "Blocked")},
    {'D', Tone::Warning, QT_TRANSLATE_NOOP("BStatus", "Differences"), QT_TRANSLATE_NOOP("BStatus", "Verify found differences")},
    {'I', Tone::Warning, QT_TRANSLATE_NOOP("BStatus", "Incomplete"), QT_TRANSLATE_NOOP("BStatus", "Incomplete job")},
};

const JobEntry *findJob(const QString &letter)
{
    if (letter.size() != 1) {
        return nullptr;
    }
    const QChar c = letter.at(0);
    for (const JobEntry &entry : kJobEntries) {
        if (c == QLatin1Char(entry.letter)) {
            return &entry;
        }
    }
    return nullptr;
}

// Built-in tokens: the same values as resources/themes/dark.qss and light.qss.
// Text on its fill has a contrast of at least 6:1 in both themes.
struct ToneColors {
    const char *background;
    const char *foreground;
    const char *accent;
};

// Order of Tone
const ToneColors kDark[ToneCount] = {
    {"#333337", "#cccccc", "#6e6e73"},  // Neutral
    {"#1d3b24", "#8fe39c", "#3fa452"},  // Success
    {"#45360f", "#ffd36b", "#d89b14"},  // Warning
    {"#4a1d1d", "#ff9e9e", "#d9473f"},  // Error
    {"#11314d", "#8fc9ff", "#3a8fd9"},  // Running
    {"#2a313b", "#b7c4d4", "#6c7d93"},  // Waiting
    {"#123a38", "#82d9cf", "#2f9e92"},  // Info
};

const ToneColors kLight[ToneCount] = {
    {"#ebebeb", "#444444", "#9a9a9a"},  // Neutral
    {"#dcf2df", "#1a5c25", "#2e8b3e"},  // Success
    {"#fff0c4", "#6e4a00", "#c88a00"},  // Warning
    {"#fde1df", "#a01c16", "#d0342c"},  // Error
    {"#dceafa", "#0b4a85", "#2f7fd0"},  // Running
    {"#e5eaf0", "#3a4858", "#7c8a9c"},  // Waiting
    {"#d8f1ee", "#0e5750", "#23897e"},  // Info
};

constexpr int kPartCount = 3;

QColor &overrideSlot(Tone tone, Part part)
{
    static QColor overrides[ToneCount][kPartCount];
    return overrides[static_cast<int>(tone)][static_cast<int>(part)];
}

bool paletteIsDark()
{
    if (!qobject_cast<QGuiApplication *>(QCoreApplication::instance())) {
        return false;
    }
    return QGuiApplication::palette().color(QPalette::Window).lightness() < 128;
}

} // namespace

Chip jobStatus(const QString &letter)
{
    Chip chip;
    if (const JobEntry *entry = findJob(letter)) {
        chip.tone = entry->tone;
        chip.code = letter;
        chip.text = trText(entry->shortText);
        chip.fullText = trText(entry->fullText);
        return chip;
    }
    // A letter this version does not know: shown as it came, never green
    chip.tone = Tone::Neutral;
    chip.code = letter.isEmpty() ? QStringLiteral("?") : letter;
    chip.text = letter.isEmpty() ? trText("Unknown") : letter;
    chip.fullText = letter.isEmpty() ? trText("Unknown status")
                                     : trText("Status %1").arg(letter);
    return chip;
}

QString jobStatusText(const QString &letter)
{
    return jobStatus(letter).fullText;
}

bool isJobError(const QString &letter)
{
    return letter == QLatin1String("E") || letter == QLatin1String("e")
           || letter == QLatin1String("f");
}

Chip clientActivity(BClientStatus::Activity activity, const QString &label)
{
    Chip chip;
    chip.text = label;
    const QString state = BClientStatus::stateName(activity);
    switch (activity) {
    case BClientStatus::Activity::Recent:
        // A recent backup or contact is a fact about the past, not a
        // verified "online" or a successful backup: not green
        chip.tone = Tone::Info;
        chip.code = QStringLiteral("●");   // filled circle
        break;
    case BClientStatus::Activity::Stale:
        chip.tone = Tone::Warning;
        chip.code = QStringLiteral("!");
        break;
    case BClientStatus::Activity::Never:
        chip.tone = Tone::Neutral;
        chip.code = QStringLiteral("–");   // en dash
        break;
    case BClientStatus::Activity::Unknown:
        chip.tone = Tone::Neutral;
        chip.code = QStringLiteral("?");
        break;
    }
    chip.fullText = (label.isEmpty() || label == state) ? state
                                                        : trText("%1, %2").arg(state, label);
    return chip;
}

Tone volumeTone(const QString &status)
{
    const QString s = status.trimmed().toLower();
    if (s == QLatin1String("error")) return Tone::Error;
    if (s == QLatin1String("append")) return Tone::Info;
    if (s == QLatin1String("busy")) return Tone::Running;
    if (s == QLatin1String("purged") || s == QLatin1String("recycle")
        || s == QLatin1String("cleaning")) {
        return Tone::Waiting;
    }
    if (s == QLatin1String("unlabeled")) return Tone::Warning;
    // Full, Used, Read-Only, Disabled, Archive, unknown
    return Tone::Neutral;
}

QString toneName(Tone tone)
{
    switch (tone) {
    case Tone::Neutral: return QStringLiteral("neutral");
    case Tone::Success: return QStringLiteral("success");
    case Tone::Warning: return QStringLiteral("warning");
    case Tone::Error:   return QStringLiteral("error");
    case Tone::Running: return QStringLiteral("running");
    case Tone::Waiting: return QStringLiteral("waiting");
    case Tone::Info:    return QStringLiteral("info");
    }
    return QStringLiteral("neutral");
}

QColor defaultColor(Tone tone, Part part, bool dark)
{
    const int t = static_cast<int>(tone);
    if (t < 0 || t >= ToneCount) {
        return QColor();
    }
    const ToneColors &colors = dark ? kDark[t] : kLight[t];
    switch (part) {
    case Part::Background: return QColor(QLatin1String(colors.background));
    case Part::Foreground: return QColor(QLatin1String(colors.foreground));
    case Part::Accent:     return QColor(QLatin1String(colors.accent));
    }
    return QColor();
}

QColor color(Tone tone, Part part)
{
    const QColor &token = overrideSlot(tone, part);
    if (token.isValid()) {
        return token;
    }
    return defaultColor(tone, part, paletteIsDark());
}

void setColor(Tone tone, Part part, const QColor &value)
{
    const int t = static_cast<int>(tone);
    if (t < 0 || t >= ToneCount) {
        return;
    }
    overrideSlot(tone, part) = value;
}

void resetColors()
{
    for (int t = 0; t < ToneCount; ++t) {
        for (int p = 0; p < kPartCount; ++p) {
            overrideSlot(static_cast<Tone>(t), static_cast<Part>(p)) = QColor();
        }
    }
}

} // namespace BStatus
