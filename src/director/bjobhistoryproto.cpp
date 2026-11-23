// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bjobhistoryproto.cpp
 * @brief The job history over the typed DirectorApi.ListJobs
 */

#include "director/bjobhistoryproto.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

namespace BJobHistoryProto {

namespace {

/// An enum value of the proto is the catalog's letter ('B', 'I', 'T', ' ');
/// 0 (unspecified) is no letter at all.
QString letter(int code)
{
    if (code <= 0 || code > 0xFFFF) return QString();
    return QString(QChar(code));
}

QString number(quint64 value) { return QString::number(value); }

} // namespace

Request parseCommandLine(const QString &commandLine)
{
    static const QRegularExpression form(
        QStringLiteral(R"(^\s*llist\s+jobs\s+days=(\d+)\s+reverse\s+limit=(\d+)\s*$)"));
    Request request;
    const QRegularExpressionMatch match = form.match(commandLine);
    if (!match.hasMatch()) return request;
    bool okDays = false;
    bool okLimit = false;
    request.days = match.captured(1).toUInt(&okDays);
    request.limit = match.captured(2).toUInt(&okLimit);
    request.valid = okDays && okLimit && request.days > 0 && request.limit > 0;
    return request;
}

pb::ListJobsCommand toCommand(const Request &request)
{
    pb::ListJobsCommand command;
    command.setLlist(true);
    command.setDays(request.days);
    command.setLimit(request.limit);
    command.setReverse(true);
    return command;
}

QString toLlistReply(const pb::ListJobsResponse &response)
{
    QJsonArray jobs;
    for (const pb::JobInfo &job : response.jobs()) {
        QJsonObject row;
        // Extra fields first: the typed fields win where both name a value
        const auto extra = job.extraFields();
        for (auto it = extra.cbegin(); it != extra.cend(); ++it) {
            row.insert(it.key(), it.value());
        }
        row.insert(QStringLiteral("jobid"), number(job.jobid()));
        row.insert(QStringLiteral("job"), job.job());
        row.insert(QStringLiteral("name"), job.name());
        row.insert(QStringLiteral("purgedfiles"), job.purgedfiles() ? QStringLiteral("1") : QStringLiteral("0"));
        row.insert(QStringLiteral("type"), letter(int(job.type())));
        row.insert(QStringLiteral("level"), letter(int(job.level())));
        row.insert(QStringLiteral("clientid"), number(job.clientid()));
        row.insert(QStringLiteral("client"), job.client());
        row.insert(QStringLiteral("jobstatus"), letter(int(job.jobstatus())));
        row.insert(QStringLiteral("schedtime"), job.schedtime());
        row.insert(QStringLiteral("starttime"), job.starttime());
        row.insert(QStringLiteral("endtime"), job.endtime());
        row.insert(QStringLiteral("realendtime"), job.realendtime());
        row.insert(QStringLiteral("duration"), job.duration());
        row.insert(QStringLiteral("jobtdate"), QString::number(job.jobtdate()));
        row.insert(QStringLiteral("volsessionid"), number(job.volsessionid()));
        row.insert(QStringLiteral("volsessiontime"), number(job.volsessiontime()));
        row.insert(QStringLiteral("jobfiles"), number(job.jobfiles()));
        row.insert(QStringLiteral("jobbytes"), number(job.jobbytes()));
        row.insert(QStringLiteral("joberrors"), number(job.joberrors()));
        row.insert(QStringLiteral("jobmissingfiles"), number(job.jobmissingfiles()));
        row.insert(QStringLiteral("poolid"), number(job.poolid()));
        row.insert(QStringLiteral("poolname"), job.poolname());
        row.insert(QStringLiteral("priorjobid"), number(job.priorjobid()));
        row.insert(QStringLiteral("filesetid"), number(job.filesetid()));
        row.insert(QStringLiteral("fileset"), job.fileset());
        jobs.append(row);
    }
    QJsonObject root;
    root.insert(QStringLiteral("jsonrpc"), QStringLiteral("2.0"));
    root.insert(QStringLiteral("id"), QJsonValue::Null);
    root.insert(QStringLiteral("result"), QJsonObject{{QStringLiteral("jobs"), jobs}});
    return QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
}

bool newestFirst(const pb::ListJobsResponse &response)
{
    // Catalog times "yyyy-MM-dd HH:mm:ss" compare as text
    QString previous;
    for (const pb::JobInfo &job : response.jobs()) {
        const QString start = job.starttime().trimmed();
        if (start.isEmpty() || start == QLatin1String("0")) continue;
        if (!previous.isEmpty() && start > previous) return false;
        previous = start;
    }
    return true;
}

} // namespace BJobHistoryProto
