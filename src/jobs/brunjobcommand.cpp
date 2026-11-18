#include "jobs/brunjobcommand.h"

#include <QDateTime>
#include <QRegularExpression>

namespace BRunJobCommand {

namespace {

bool hasControlCharacter(const QString &value)
{
    for (const QChar c : value) {
        if (c.unicode() < 0x20 || c.unicode() == 0x7f)
            return true;
    }
    return false;
}

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
    if (ok)
        *ok = !hasControlCharacter(value);

    QString escaped;
    escaped.reserve(value.size() + 2);
    escaped += '"';
    for (const QChar c : value) {
        if (c == '\\' || c == '"')
            escaped += '\\';
        escaped += c;
    }
    escaped += '"';
    return escaped;
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

} // namespace BRunJobCommand
