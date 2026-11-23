// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BJOBHISTORYPROTO_H
#define BJOBHISTORYPROTO_H

/**
 * @file bjobhistoryproto.h
 * @brief The job history over the typed DirectorApi.ListJobs
 *
 * The job history ("llist jobs days=<n> reverse limit=<m>", Command::LlistJobs)
 * is read by BJobHistoryReply and B321Verifier from the JSON the Director
 * gives in API mode 2. Over gRPC the transport asks the typed ListJobs
 * instead and turns its reply into that same JSON, so the readers stay as
 * they are and a Director without the typed call (or without `reverse`) can
 * still be served by the command line.
 */

#include <QString>

#include "director_api.qpb.h"

namespace BJobHistoryProto {

namespace pb = onesimus::director::api;

/** The history request of a command line; valid only for the exact form BareosDirector builds. */
struct Request
{
    bool valid = false;
    quint32 days = 0;
    quint32 limit = 0;
};

/** Reads "llist jobs days=<n> reverse limit=<m>"; every other line gives an invalid Request. */
Request parseCommandLine(const QString &commandLine);

/** The typed command: llist, days, limit, newest first. */
pb::ListJobsCommand toCommand(const Request &request);

/**
 * @brief The reply as the command line gives it in API mode 2
 *
 * {"jsonrpc":"2.0","id":null,"result":{"jobs":[{...}]}} with every value a
 * string, codes as their letters ("type":"B", "level":"I", "jobstatus":"T").
 */
QString toLlistReply(const pb::ListJobsResponse &response);

/**
 * @brief Whether the rows come newest first
 *
 * A Director that does not know `reverse` ignores it and sends the oldest
 * first: with a limit that is the wrong window, so the caller goes back to
 * the command line. Rows without a start time are not compared.
 */
bool newestFirst(const pb::ListJobsResponse &response);

} // namespace BJobHistoryProto

#endif // BJOBHISTORYPROTO_H
