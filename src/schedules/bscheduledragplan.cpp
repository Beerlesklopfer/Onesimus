#include "schedules/bscheduledragplan.h"

#include "schedules/model/brunspec.h"

namespace BScheduleDragPlan {

bool sameRun(const QString &original, const QString &generated, int newHour, int newMinute)
{
    BRunLine a = BRunParser::parse(original);
    const BRunLine b = BRunParser::parse(generated);
    if (!a.spec || !b.spec)
        return false;
    if (newHour >= 0) {
        if (newHour > 23)
            return false;
        a.spec->mask.hour = (1u << newHour);
        a.spec->minute = qMax(0, newMinute);
    }
    return *a.spec == *b.spec;
}

Plan plan(const QStringList &originalRuns, const BScheduleEntry &dragged,
          int newHour, int newMinute)
{
    Plan result;
    result.originalRuns = originalRuns;

    auto refuse = [&result](Reason reason, int line = -1, const QString &text = QString()) {
        result.verdict = Verdict::Refused;
        result.reason = reason;
        result.reasonLine = line;
        result.reasonText = text;
        result.generatedRuns.clear();
        result.differingLines.clear();
        return result;
    };

    if (originalRuns.isEmpty())
        return refuse(Reason::NoRunLines);
    if (newHour < 0 || newHour > 23 || newMinute < 0 || newMinute > 59)
        return refuse(Reason::BadTime);

    // A schedule with a line that is not understood is not written at all
    QList<BRunLine> lines;
    lines.reserve(originalRuns.size());
    for (int i = 0; i < originalRuns.size(); ++i) {
        const BRunLine line = BRunParser::parse(originalRuns[i]);
        if (!line.spec)
            return refuse(Reason::NotUnderstood, i, line.parseError);
        lines.append(line);
    }

    // Which original line was dragged? The one that says what the entry's
    // own line says and runs at the entry's hour. Pool, storage and priority
    // of the entry may have been filled in from the job and do not count.
    if (dragged.source.spec) {
        for (int i = 0; i < lines.size(); ++i) {
            if (*lines[i].spec != *dragged.source.spec)
                continue;
            if (!lines[i].spec->hours().contains(dragged.hour))
                continue;
            if (result.draggedLine >= 0)
                return refuse(Reason::DraggedLineAmbiguous, i);
            result.draggedLine = i;
        }
    }
    if (result.draggedLine < 0)
        return refuse(Reason::DraggedLineNotFound);

    // The dragged line: the same run at the new time
    BRunLine &moved = lines[result.draggedLine];
    if (moved.spec->hours().size() != 1)
        return refuse(Reason::SeveralHours, result.draggedLine);
    moved.spec->mask.hour = (1u << newHour);
    moved.spec->minute = newMinute;
    moved.dirty = true;

    for (int i = 0; i < lines.size(); ++i) {
        QString error;
        const QString text = BRunWriter::lineText(lines[i], &error);
        if (text.isEmpty())
            return refuse(Reason::WriteCheckFailed, i, error);
        result.generatedRuns.append(text);

        // An untouched line is the original text, except where Bareos does
        // not read back what the Director reported
        if (i != result.draggedLine && text != originalRuns[i])
            result.differingLines.append(i);
    }

    result.verdict = result.differingLines.isEmpty() ? Verdict::Exact
                                                     : Verdict::NeedsConfirmation;
    return result;
}

} // namespace BScheduleDragPlan
