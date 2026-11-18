#include "clients/bclientstatus.h"

#include <QCoreApplication>
#include <QLocale>
#include <limits>

namespace BClientStatus {

namespace {

QString trText(const char *text)
{
    return QCoreApplication::translate("BClientStatus", text);
}

int effectiveThreshold(int thresholdHours)
{
    return thresholdHours >= 1 ? thresholdHours : DefaultThresholdHours;
}

} // namespace

bool isMissingTimestamp(const QString &text)
{
    const QString trimmed = text.trimmed();
    return trimmed.isEmpty()
           || trimmed == QLatin1String("0")
           || trimmed.startsWith(QLatin1String("0000-00-00"));
}

QDateTime parseTimestamp(const QString &text, const QTimeZone &directorZone)
{
    if (isMissingTimestamp(text)) {
        return QDateTime();
    }

    const QString trimmed = text.trimmed();
    QDateTime parsed = QDateTime::fromString(trimmed, Qt::ISODate);
    if (!parsed.isValid()) {
        parsed = QDateTime::fromString(trimmed, QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    }
    if (!parsed.isValid()) {
        return QDateTime();
    }

    // No zone in the text: it is the Director's wall-clock time
    if (parsed.timeSpec() == Qt::LocalTime) {
        return QDateTime(parsed.date(), parsed.time(), directorZone);
    }
    return parsed;
}

Info classify(const QString &timestamp, const QDateTime &now, int thresholdHours,
              const QTimeZone &directorZone)
{
    Info info;
    info.raw = timestamp.trimmed();

    if (isMissingTimestamp(timestamp)) {
        info.activity = Activity::Never;
        return info;
    }

    const QDateTime when = parseTimestamp(timestamp, directorZone);
    if (!when.isValid() || !now.isValid()) {
        info.activity = Activity::Unknown;
        return info;
    }

    info.timestamp = when;
    info.ageSecs = qMax<qint64>(0, when.secsTo(now));
    info.activity = info.ageSecs < qint64(effectiveThreshold(thresholdHours)) * 3600
                        ? Activity::Recent : Activity::Stale;
    return info;
}

QString ageText(qint64 ageSecs)
{
    if (ageSecs < 0) {
        return QString();
    }
    if (ageSecs < 60) {
        return trText("just now");
    }
    if (ageSecs < 3600) {
        return trText("%1 min ago").arg(ageSecs / 60);
    }
    if (ageSecs < 48 * 3600) {
        return trText("%1 h ago").arg(ageSecs / 3600);
    }
    return trText("%1 d ago").arg(ageSecs / 86400);
}

QString label(const Info &info)
{
    switch (info.activity) {
    case Activity::Recent:
    case Activity::Stale:
        return ageText(info.ageSecs);
    case Activity::Never:
        return trText("Never");
    case Activity::Unknown:
        break;
    }
    return trText("Unknown");
}

QString stateName(Activity activity)
{
    switch (activity) {
    case Activity::Recent:
        return trText("Recent");
    case Activity::Stale:
        return trText("Stale");
    case Activity::Never:
        return trText("Never");
    case Activity::Unknown:
        break;
    }
    return trText("Unknown");
}

QString toolTip(const Info &info, int thresholdHours)
{
    const int limit = effectiveThreshold(thresholdHours);
    const QString basis = trText("Based on the Director's job history, not on a live "
                                 "connection check: the client may or may not be reachable now.");

    switch (info.activity) {
    case Activity::Recent:
        return trText("Last backup or contact: %1 (%2).\nRecent: within the last %3 h.\n%4")
            .arg(QLocale().toString(info.timestamp, QLocale::ShortFormat),
                 ageText(info.ageSecs))
            .arg(limit)
            .arg(basis);
    case Activity::Stale:
        return trText("Last backup or contact: %1 (%2).\nStale: older than %3 h.\n%4")
            .arg(QLocale().toString(info.timestamp, QLocale::ShortFormat),
                 ageText(info.ageSecs))
            .arg(limit)
            .arg(basis);
    case Activity::Never:
        return trText("No backup or contact is known for this client.\n"
                      "The Director reported no connection time and the loaded job list "
                      "contains no job of this client.");
    case Activity::Unknown:
        break;
    }
    return trText("The time of the last backup or contact cannot be read: \"%1\"").arg(info.raw);
}

qint64 sortKey(const Info &info)
{
    switch (info.activity) {
    case Activity::Recent:
    case Activity::Stale:
        return info.ageSecs;
    case Activity::Never:
        return std::numeric_limits<qint64>::max() - 1;
    case Activity::Unknown:
        break;
    }
    return std::numeric_limits<qint64>::max();
}

} // namespace BClientStatus
