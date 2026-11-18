#include "schedules/bscheduledragplan.h"

#include <QRegularExpression>

namespace BScheduleDragPlan {

namespace {

/// What a Run line says, reduced to what is needed to compare two lines
struct RunFacts {
    bool timeOk = false;
    int minutes = -1;           ///< Minutes since midnight
    QString level;              ///< Lowercase, empty if the line names none
    int dayMask = 0;            ///< Bit 0 = mon ... bit 6 = sun; 0 = no day named
    QStringList weeks;          ///< Week-of-month tokens, sorted
    QStringList rest;           ///< Everything else, in the order found

    // First token of each form the regeneration is known to lose
    QString overrideToken;
    QString weekToken;
    QString rangeToken;
    QString dayOfMonthToken;

    /// Bareos runs a line without a day on every day
    int effectiveDays() const { return dayMask == 0 ? 0x7f : dayMask; }
};

int dayIndex(const QString &token)
{
    static const QStringList names = {
        "mon", "tue", "wed", "thu", "fri", "sat", "sun"
    };
    static const QStringList longNames = {
        "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"
    };
    int index = names.indexOf(token);
    if (index < 0)
        index = longNames.indexOf(token);
    return index;
}

bool isWeekOfMonth(const QString &token)
{
    static const QStringList words = {
        "1st", "2nd", "3rd", "4th", "5th", "last",
        "first", "second", "third", "fourth", "fifth"
    };
    return words.contains(token);
}

bool isNumber(const QString &token)
{
    if (token.isEmpty())
        return false;
    for (const QChar c : token) {
        if (!c.isDigit())
            return false;
    }
    return true;
}

bool isLevel(const QString &token)
{
    static const QStringList levels = {
        "full", "differential", "incremental", "virtualfull"
    };
    return levels.contains(token);
}

void note(QString &slot, const QString &token)
{
    if (slot.isEmpty())
        slot = token;
}

RunFacts factsOf(const QString &run)
{
    RunFacts facts;
    static const QRegularExpression separators(QStringLiteral("[\\s,]+"));
    static const QRegularExpression timeRx(QStringLiteral("^(\\d{1,2}):(\\d{2})$"));

    // Compared in lowercase; a refusal quotes the token as it was written
    const QStringList written = run.split(separators, Qt::SkipEmptyParts);
    QStringList tokens;
    tokens.reserve(written.size());
    for (const QString &word : written)
        tokens.append(word.toLower());

    for (int i = 0; i < tokens.size(); ++i) {
        const QString &token = tokens[i];
        const QString &asWritten = written[i];

        const int eq = token.indexOf('=');
        if (eq >= 0) {
            const QString key = token.left(eq);
            const QString value = token.mid(eq + 1);
            if (key == "level" && !value.isEmpty() && facts.level.isEmpty()) {
                facts.level = value;
            } else {
                note(facts.overrideToken, asWritten);
                facts.rest.append(token);
            }
            continue;
        }

        if (token == "at") {
            const QRegularExpressionMatch match =
                (i + 1 < tokens.size()) ? timeRx.match(tokens[i + 1]) : QRegularExpressionMatch();
            if (match.hasMatch() && !facts.timeOk) {
                const int hour = match.captured(1).toInt();
                const int minute = match.captured(2).toInt();
                if (hour < 24 && minute < 60) {
                    facts.timeOk = true;
                    facts.minutes = hour * 60 + minute;
                    ++i;
                    continue;
                }
            }
            facts.rest.append(token);
            continue;
        }

        if (isLevel(token) && facts.level.isEmpty()) {
            facts.level = token;
            continue;
        }

        if (token == "daily") {
            facts.dayMask = 0x7f;
            continue;
        }

        const int day = dayIndex(token);
        if (day >= 0) {
            facts.dayMask |= (1 << day);
            continue;
        }

        if (isWeekOfMonth(token)) {
            note(facts.weekToken, asWritten);
            facts.weeks.append(token);
            continue;
        }

        if (isNumber(token)) {
            note(facts.dayOfMonthToken, asWritten);
            facts.rest.append(token);
            continue;
        }

        const int dash = token.indexOf('-');
        if (dash > 0 && dash < token.size() - 1) {
            const QString from = token.left(dash);
            const QString to = token.mid(dash + 1);
            const int fromDay = dayIndex(from);
            const int toDay = dayIndex(to);
            if (fromDay >= 0 && toDay >= 0) {
                note(facts.rangeToken, asWritten);
                // A range may wrap around the end of the week (sun-fri)
                for (int d = fromDay; ; d = (d + 1) % 7) {
                    facts.dayMask |= (1 << d);
                    if (d == toDay)
                        break;
                }
                continue;
            }
            if (isWeekOfMonth(from) && isWeekOfMonth(to)) {
                note(facts.weekToken, asWritten);
                facts.weeks.append(token);
                continue;
            }
            if (isNumber(from) && isNumber(to)) {
                note(facts.dayOfMonthToken, asWritten);
                facts.rest.append(token);
                continue;
            }
        }

        // hourly, weekly, monthly, month names, w01, unknown levels, ...
        facts.rest.append(token);
    }

    facts.weeks.sort();
    return facts;
}

bool sameFacts(const RunFacts &a, const RunFacts &b)
{
    return a.timeOk && b.timeOk
        && a.minutes == b.minutes
        && a.level == b.level
        && a.effectiveDays() == b.effectiveDays()
        && a.weeks == b.weeks
        && a.rest == b.rest;
}

/// The dragged entry comes from the Gantt view, where pool, storage and
/// priority may have been filled in from the job; only what a Run line
/// without overrides says is compared.
bool sameEntry(const BScheduleEntry &a, const BScheduleEntry &b)
{
    return a.level == b.level
        && a.hour == b.hour
        && a.minute == b.minute
        && a.daysOfWeek == b.daysOfWeek
        && a.weekSpec == b.weekSpec;
}

} // namespace

bool sameRun(const QString &original, const QString &generated, int newHour, int newMinute)
{
    RunFacts a = factsOf(original);
    const RunFacts b = factsOf(generated);
    if (newHour >= 0 && a.timeOk)
        a.minutes = newHour * 60 + qMax(0, newMinute);
    return sameFacts(a, b);
}

Plan plan(const QStringList &originalRuns, const BScheduleEntry &dragged,
          int newHour, int newMinute)
{
    Plan result;
    result.originalRuns = originalRuns;

    auto refuse = [&result](Reason reason, int line = -1, const QString &token = QString()) {
        result.verdict = Verdict::Refused;
        result.reason = reason;
        result.reasonLine = line;
        result.reasonToken = token;
        result.generatedRuns.clear();
        result.differingLines.clear();
        return result;
    };

    if (originalRuns.isEmpty())
        return refuse(Reason::NoRunLines);

    // Overrides are never regenerated: toRunDirective() writes them behind
    // the time, where the original line does not have them, and
    // fromRunDirective() reads day and level names out of their values.
    // A line without a readable time cannot be compared at all.
    QList<RunFacts> facts;
    facts.reserve(originalRuns.size());
    for (int i = 0; i < originalRuns.size(); ++i) {
        const RunFacts f = factsOf(originalRuns[i]);
        if (!f.overrideToken.isEmpty())
            return refuse(Reason::Override, i, f.overrideToken);
        if (!f.timeOk)
            return refuse(Reason::NoTime, i);
        facts.append(f);
    }

    // Which original line was dragged?
    QList<BScheduleEntry> entries;
    entries.reserve(originalRuns.size());
    for (int i = 0; i < originalRuns.size(); ++i) {
        const BScheduleEntry entry = BScheduleEntry::fromRunDirective(originalRuns[i]);
        if (sameEntry(entry, dragged)) {
            if (result.draggedLine >= 0)
                return refuse(Reason::DraggedLineAmbiguous, i);
            result.draggedLine = i;
        }
        entries.append(entry);
    }
    if (result.draggedLine < 0)
        return refuse(Reason::DraggedLineNotFound);

    for (int i = 0; i < originalRuns.size(); ++i) {
        const bool isDragged = (i == result.draggedLine);
        BScheduleEntry entry = entries[i];
        if (isDragged) {
            entry.hour = newHour;
            entry.minute = newMinute;
        }
        const QString generated = entry.toRunDirective();
        result.generatedRuns.append(generated);

        RunFacts expected = facts[i];
        if (isDragged)
            expected.minutes = newHour * 60 + newMinute;

        if (sameFacts(expected, factsOf(generated)))
            continue;

        // The line changed in more than the dragged time. If it has one of
        // the forms the regeneration is known to lose, that is the cause.
        const RunFacts &f = facts[i];
        if (!f.weekToken.isEmpty())
            return refuse(Reason::WeekOfMonth, i, f.weekToken);
        if (!f.rangeToken.isEmpty())
            return refuse(Reason::DayRange, i, f.rangeToken);
        if (!f.dayOfMonthToken.isEmpty())
            return refuse(Reason::DayOfMonth, i, f.dayOfMonthToken);

        result.differingLines.append(i);
    }

    result.verdict = result.differingLines.isEmpty() ? Verdict::Exact
                                                     : Verdict::NeedsConfirmation;
    return result;
}

} // namespace BScheduleDragPlan
