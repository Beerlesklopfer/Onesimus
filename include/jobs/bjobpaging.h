// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BJOBPAGING_H
#define BJOBPAGING_H

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

} // namespace BJobPaging

#endif // BJOBPAGING_H
