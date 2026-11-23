// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "jobs/brunjobcommand.h"
#include "bquote.h"

#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>

namespace BRunJobCommand {

namespace {

bool isPlainWord(const QString &value)
{
    static const QRegularExpression plain(QStringLiteral("^[A-Za-z0-9_-]+$"));
    return plain.match(value).hasMatch();
}

/// One surrounding pair of double quotes typed by the user is not part of the value
QString stripSurroundingQuotes(const QString &value)
{
    if (value.size() >= 2 && value.startsWith('"') && value.endsWith('"'))
        return value.mid(1, value.size() - 2).trimmed();
    return value;
}

} // namespace

QString quote(const QString &value, bool *ok)
{
    return BQuote::quoted(value, ok);
}

Result build(const Params &params)
{
    Result result;
    bool failed = false;

    auto fail = [&](Error error, const char *field) {
        if (!failed) {
            failed = true;
            result.error = error;
            result.field = QString::fromLatin1(field);
        }
    };

    const QString job = params.job.trimmed();
    if (job.isEmpty())
        fail(Error::MissingJob, "job");

    QString command = QStringLiteral("run");

    auto addQuoted = [&](const char *keyword, const QString &value) {
        if (value.isEmpty())
            return;
        bool ok = true;
        const QString quoted = quote(value, &ok);
        if (!ok)
            fail(Error::ControlCharacter, keyword);
        command += ' ' + QString::fromLatin1(keyword) + '=' + quoted;
    };

    addQuoted("job", job);
    addQuoted("client", params.client.trimmed());
    addQuoted("fileset", params.fileset.trimmed());
    addQuoted("pool", params.pool.trimmed());
    addQuoted("storage", params.storage.trimmed());

    const QString level = params.level.trimmed();
    if (!level.isEmpty()) {
        if (isPlainWord(level))
            command += QStringLiteral(" level=") + level;
        else
            addQuoted("level", level);
    }

    if (params.priority != 0) {
        if (params.priority < 0)
            fail(Error::InvalidPriority, "priority");
        command += QStringLiteral(" priority=") + QString::number(params.priority);
    }

    const QString when = stripSurroundingQuotes(params.when.trimmed());
    if (!when.isEmpty()) {
        // The Director starts the job immediately when it cannot read the
        // time, so only a complete, valid time is accepted here.
        static const QRegularExpression format(
            QStringLiteral("^\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}$"));
        const QDateTime time = QDateTime::fromString(when, QStringLiteral("yyyy-MM-dd HH:mm:ss"));
        if (!format.match(when).hasMatch() || !time.isValid())
            fail(Error::InvalidWhen, "when");
        addQuoted("when", when);
    }

    addQuoted("bootstrap", params.bootstrap.trimmed());

    command += QStringLiteral(" yes");

    if (!failed)
        result.command = command;
    return result;
}

Result buildEstimate(const Params &params)
{
    Result result;

    auto fail = [&result](Error error, const char *field) {
        if (result.error == Error::None) {
            result.error = error;
            result.field = QString::fromLatin1(field);
        }
    };

    const QString job = params.job.trimmed();
    if (job.isEmpty())
        fail(Error::MissingJob, "job");

    QString command = QStringLiteral("estimate");

    auto addQuoted = [&](const char *keyword, const QString &value) {
        if (value.isEmpty())
            return;
        bool ok = true;
        const QString quoted = quote(value, &ok);
        if (!ok)
            fail(Error::ControlCharacter, keyword);
        command += ' ' + QString::fromLatin1(keyword) + '=' + quoted;
    };

    addQuoted("job", job);
    addQuoted("client", params.client.trimmed());
    addQuoted("fileset", params.fileset.trimmed());

    const QString level = params.level.trimmed();
    if (!level.isEmpty()) {
        if (isPlainWord(level))
            command += QStringLiteral(" level=") + level;
        else
            addQuoted("level", level);
    }

    if (result.ok())
        result.command = command;
    return result;
}

Estimate parseEstimateReply(const QString &reply)
{
    Estimate estimate;
    const QJsonDocument doc = QJsonDocument::fromJson(reply.toUtf8());
    const QJsonObject root = doc.object();
    auto number = [](const QJsonValue &v) -> qint64 {
        if (v.isString())
            return v.toString().remove(QLatin1Char(',')).toLongLong();
        return qint64(v.toDouble());
    };

    const QJsonObject result = root.value(QStringLiteral("result")).toObject();
    const QJsonObject est = result.value(QStringLiteral("estimate")).toObject();
    if (est.contains(QStringLiteral("files")) && est.contains(QStringLiteral("bytes"))) {
        estimate.ok = true;
        estimate.files = number(est.value(QStringLiteral("files")));
        estimate.bytes = number(est.value(QStringLiteral("bytes")));
        return estimate;
    }

    // The text form, anywhere in the reply
    static const QRegularExpression textRx(
        QStringLiteral("files=([\\d,]+)\\s+bytes=([\\d,]+)"));
    const QRegularExpressionMatch match = textRx.match(reply);
    if (match.hasMatch()) {
        estimate.ok = true;
        estimate.files = match.captured(1).remove(QLatin1Char(',')).toLongLong();
        estimate.bytes = match.captured(2).remove(QLatin1Char(',')).toLongLong();
        return estimate;
    }

    const QJsonObject error = root.value(QStringLiteral("error")).toObject();
    estimate.error = error.value(QStringLiteral("data")).toObject()
                         .value(QStringLiteral("messages")).toObject()
                         .value(QStringLiteral("error")).toArray().first().toString();
    if (estimate.error.isEmpty())
        estimate.error = error.value(QStringLiteral("message")).toString();
    return estimate;
}

} // namespace BRunJobCommand
