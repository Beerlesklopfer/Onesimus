// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "jobs/brestorecommand.h"
#include "jobs/brunjobcommand.h"

#include <QRegularExpression>
#include <QStringList>

namespace BRestoreCommand {

Result build(const Params &params)
{
    Result result;
    auto fail = [&](Error error, const char *field) {
        result.error = error;
        result.field = QString::fromLatin1(field);
        return result;
    };

    static const QRegularExpression tableName(QStringLiteral("^[A-Za-z0-9_]+$"));
    if (!tableName.match(params.table).hasMatch())
        return fail(Error::InvalidTable, "file");

    const QString backupClient = params.backupClient.trimmed();
    if (backupClient.isEmpty())
        return fail(Error::MissingClient, "client");
    QString restoreClient = params.restoreClient.trimmed();
    if (restoreClient.isEmpty())
        restoreClient = backupClient;

    if (params.where.trimmed().isEmpty())
        return fail(Error::MissingWhere, "where");

    static const QStringList replaceValues{
        QStringLiteral("always"), QStringLiteral("ifnewer"),
        QStringLiteral("ifolder"), QStringLiteral("never")};
    if (!replaceValues.contains(params.replace))
        return fail(Error::InvalidReplace, "replace");

    QStringList parts;
    parts << QStringLiteral("file=?%1").arg(params.table);

    struct Value { const char *keyword; QString value; };
    const Value quoted[] = {
        {"client", backupClient},
        {"restoreclient", restoreClient},
        {"restorejob", params.restoreJob.trimmed()},
        {"where", params.where.trimmed()},
    };
    for (const Value &v : quoted) {
        if (v.value.isEmpty())
            continue;   // only restorejob may be empty here
        bool ok = true;
        const QString text = BRunJobCommand::quote(v.value, &ok);
        if (!ok)
            return fail(Error::ControlCharacter, v.keyword);
        parts << QString::fromLatin1(v.keyword) + '=' + text;
    }

    parts << QStringLiteral("replace=%1").arg(params.replace);
    parts << QStringLiteral("yes");

    result.arguments = parts.join(' ');
    return result;
}

} // namespace BRestoreCommand
