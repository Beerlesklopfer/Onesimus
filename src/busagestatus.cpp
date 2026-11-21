/**
 * @file busagestatus.cpp
 * @brief Text, tool tip and level of the free tier usage in the status bar
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "busagestatus.h"
#include "bbyteformat.h"

#include <QDate>
#include <QStringList>
#include <QTimeZone>

namespace {

constexpr int NearPercent = 80;     // odir's UsageState NEAR (usage.proto)
constexpr qint64 SecondsPerDay = 86400;

// First day of "2026-11"; invalid when the text is not a month
QDate monthStart(const QString &month)
{
    return QDate::fromString(month + QStringLiteral("-01"), QStringLiteral("yyyy-MM-dd"));
}

} // namespace

BUsageStatus::Level BUsageStatus::level(const BUsage::Info &info)
{
    if (!info.present) {
        return Level::Hidden;
    }
    switch (info.state) {
    case BUsage::State::Ok:
        return Level::Ok;
    case BUsage::State::Near:
        return Level::Near;
    case BUsage::State::Reached:
        return Level::Reached;
    case BUsage::State::Unspecified:
        break;
    }
    // odir always sets the state; the same rule as odir's usage.rs otherwise
    if (info.limitBytes == 0) {
        return Level::Ok;
    }
    if (info.used() >= info.limitBytes) {
        return Level::Reached;
    }
    return percent(info) >= NearPercent ? Level::Near : Level::Ok;
}

int BUsageStatus::percent(const BUsage::Info &info)
{
    if (info.limitBytes == 0) {
        return -1;
    }
    return int(double(info.used()) * 100.0 / double(info.limitBytes));
}

QString BUsageStatus::text(const BUsage::Info &info, const QLocale &locale)
{
    if (!info.present) {
        return QString();
    }
    const QString used = BByteFormat::formatDecimal(info.used());
    const QDate start = monthStart(info.month);
    const QString month = start.isValid() ? locale.monthName(start.month(), QLocale::ShortFormat)
                                          : QString();
    if (info.limitBytes > 0) {
        const QString limit = BByteFormat::formatDecimal(info.limitBytes);
        return month.isEmpty() ? tr("Usage: %1 of %2").arg(used, limit)
                               : tr("Usage: %1 of %2 (%3)").arg(used, limit, month);
    }
    return month.isEmpty() ? tr("Usage: %1").arg(used) : tr("Usage: %1 (%2)").arg(used, month);
}

std::optional<quint64> BUsageStatus::projection(const BUsage::Info &info, const QDateTime &now)
{
    const QDate start = monthStart(info.month);
    if (!info.present || !start.isValid() || info.resetsAt <= 0) {
        return std::nullopt;
    }
    const qint64 length = qint64(start.daysInMonth()) * SecondsPerDay;
    const qint64 begin = info.resetsAt - length;
    const qint64 elapsed = now.toSecsSinceEpoch() - begin;
    if (elapsed < SecondsPerDay || elapsed >= length) {
        return std::nullopt;
    }
    const double projected = double(info.used()) * double(length) / double(elapsed);
    if (projected >= 18446744073709551615.0) {
        return ~quint64(0);
    }
    return quint64(projected);
}

QString BUsageStatus::toolTip(const BUsage::Info &info, const QDateTime &now, const QLocale &locale)
{
    if (!info.present) {
        return QString();
    }
    const auto dateOf = [&](qint64 secs, QLocale::FormatType format) {
        return locale.toString(QDateTime::fromSecsSinceEpoch(secs).toTimeZone(now.timeZone()), format);
    };
    const auto bytes = [](quint64 value) { return BByteFormat::formatDecimal(value); };

    QStringList lines;

    // Licence and limit
    switch (info.licence) {
    case BUsage::Licence::Free:
        lines << tr("Licence: free tier");
        break;
    case BUsage::Licence::Licensed: {
        QString line = info.customer.isEmpty() ? tr("Licence: licensed")
                                               : tr("Licence: %1").arg(info.customer);
        if (info.validUntil > 0) {
            line = info.customer.isEmpty()
                ? tr("Licence: licensed, valid until %1").arg(dateOf(info.validUntil, QLocale::ShortFormat))
                : tr("Licence: %1, valid until %2").arg(info.customer, dateOf(info.validUntil, QLocale::ShortFormat));
        }
        lines << line;
        break;
    }
    case BUsage::Licence::Expired:
        lines << tr("Licence: expired, the free tier applies");
        break;
    case BUsage::Licence::Invalid:
        lines << tr("Licence: not valid, the free tier applies");
        break;
    case BUsage::Licence::Unspecified:
        break;
    }
    if ((info.licence == BUsage::Licence::Expired || info.licence == BUsage::Licence::Invalid)
        && !info.licenceMessage.isEmpty()) {
        lines << info.licenceMessage;
    }
    lines << (info.limitBytes > 0 ? tr("Limit: %1 per month").arg(bytes(info.limitBytes))
                                  : tr("Limit: none"));

    // The month so far
    lines << tr("Written by finished jobs: %1").arg(bytes(info.bytesWritten));
    lines << tr("Written by running jobs so far: %1").arg(bytes(info.bytesRunning));
    if (info.limitBytes > 0) {
        lines << tr("Used: %1 % of the limit").arg(percent(info));
    }
    if (const auto projected = projection(info, now)) {
        lines << (info.limitBytes > 0 && *projected > info.limitBytes
                      ? tr("Projected by the end of the month: %1, above the limit").arg(bytes(*projected))
                      : tr("Projected by the end of the month: %1").arg(bytes(*projected)));
    }
    lines << tr("Backup jobs held this month: %1").arg(info.heldJobs);
    if (info.resetsAt > 0) {
        lines << tr("Resets on %1").arg(dateOf(info.resetsAt, QLocale::ShortFormat));
    }

    switch (level(info)) {
    case Level::Reached:
        lines << tr("Limit reached: new backup jobs are held until the reset. Restores are not affected.");
        break;
    case Level::Near:
        lines << tr("Close to the limit: at the limit new backup jobs are held.");
        break;
    case Level::Ok:
    case Level::Hidden:
        break;
    }
    return lines.join(QLatin1Char('\n'));
}
