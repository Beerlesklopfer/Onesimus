// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BJOBPAGING_H
#define BJOBPAGING_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QtGlobal>

/**
 * @brief Page arithmetic of the server-side job list pagination
 *
 * Plain functions without widgets, so the job widget and the pagination
 * widget compute the same page for the label and for the request.
 * Pages are 0-based.
 */
namespace BJobPaging {

/** Number of pages for @p totalJobs entries; 0 when there is nothing to page. */
inline int pageCount(int totalJobs, int pageSize)
{
    if (totalJobs <= 0 || pageSize <= 0) {
        return 0;
    }
    return (totalJobs + pageSize - 1) / pageSize;
}

/** @p page limited to the existing pages; the last page if the list became shorter. */
inline int clampPage(int page, int totalJobs, int pageSize)
{
    const int pages = pageCount(totalJobs, pageSize);
    if (pages <= 0 || page < 0) {
        return 0;
    }
    return qMin(page, pages - 1);
}

/** Arguments of "list jobs limit=<limit> offset=<offset>" for one page. */
struct Range {
    int limit = 0;
    int offset = 0;
};

/**
 * @brief Limit and offset of @p page
 *
 * The Director lists jobs oldest first. With @p newestFirst page 0 is the
 * end of that list, and the last page holds the remainder at its start.
 * @p page is clamped to the existing pages first.
 */
inline Range pageRange(int page, int pageSize, int totalJobs, bool newestFirst)
{
    Range range;
    range.limit = pageSize;
    if (totalJobs <= 0 || pageSize <= 0) {
        return range;
    }

    page = clampPage(page, totalJobs, pageSize);
    if (!newestFirst) {
        range.offset = page * pageSize;
        return range;
    }

    const int remaining = totalJobs - page * pageSize;  // > 0 after clamping
    range.limit = qMin(pageSize, remaining);
    range.offset = qMax(0, remaining - pageSize);
    return range;
}

/**
 * @brief Number of jobs in the reply to "list jobs count"; -1 if it holds none
 *
 * The page total must count what "list jobs" lists: every job type
 * (backups, restores, admin, copy and migration control jobs).
 * "list jobtotals" counts only some types (issue #13: 526 instead of 720
 * on one catalog), so the newest jobs fell off the first page.
 * Bareos answers {"jobs": [{"count": "720"}]}; a typed reply may carry
 * {"count": 720}. Both are accepted, numbers as JSON numbers or strings.
 * @p result is the "result" object of the JSON reply.
 */
inline int jobCountFromReply(const QJsonObject &result)
{
    auto toCount = [](const QJsonValue &v) -> int {
        bool ok = false;
        qlonglong n = -1;
        if (v.isDouble()) {
            const double d = v.toDouble();
            ok = d >= 0 && d == static_cast<double>(static_cast<qlonglong>(d));
            n = static_cast<qlonglong>(d);
        } else if (v.isString()) {
            n = v.toString().trimmed().toLongLong(&ok);
        }
        if (!ok || n < 0 || n > 0x7fffffff)
            return -1;
        return static_cast<int>(n);
    };
    if (result.contains(QLatin1String("count")))
        return toCount(result.value(QLatin1String("count")));
    const QJsonArray jobs = result.value(QLatin1String("jobs")).toArray();
    if (jobs.size() == 1 && jobs.at(0).isObject()) {
        const QJsonObject row = jobs.at(0).toObject();
        if (row.contains(QLatin1String("count")))
            return toCount(row.value(QLatin1String("count")));
    }
    return -1;
}

} // namespace BJobPaging

#endif // BJOBPAGING_H
