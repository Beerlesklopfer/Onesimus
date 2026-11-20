#include "director/baclmatrix.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

#include <algorithm>

namespace {

/// A field of a console: a JSON value, or a JSON text in extra_fields
QJsonValue field(const QJsonObject &console, const QString &key)
{
    if (console.contains(key))
        return console.value(key);
    const QJsonValue extra = console.value(QStringLiteral("extra_fields")).toObject().value(key);
    if (!extra.isString())
        return extra;
    const QString text = extra.toString();
    const QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
    if (doc.isObject()) return doc.object();
    if (doc.isArray()) return doc.array();
    return extra;   // "true", "false" or plain text
}

bool truth(const QJsonValue &value)
{
    if (value.isBool()) return value.toBool();
    return value.toString().compare(QLatin1String("true"), Qt::CaseInsensitive) == 0;
}

bool matches(const QString &pattern, const QString &name)
{
    if (pattern.compare(name, Qt::CaseInsensitive) == 0)
        return true;
    const QRegularExpression re(QRegularExpression::anchoredPattern(pattern),
                                QRegularExpression::CaseInsensitiveOption);
    return re.isValid() && re.match(name).hasMatch();
}

/// A plain name, not a pattern: it names a resource
bool isPlainName(const QString &value)
{
    static const QRegularExpression special(QStringLiteral("[\\\\^$.|?*+()\\[\\]{}!]"));
    return !value.isEmpty() && !value.contains(special);
}

} // namespace

QList<BAclMatrix::Type> BAclMatrix::types()
{
    return {
        {QStringLiteral("jobacl"), tr("Jobs")},
        {QStringLiteral("clientacl"), tr("Clients")},
        {QStringLiteral("storageacl"), tr("Storages")},
        {QStringLiteral("scheduleacl"), tr("Schedules")},
        {QStringLiteral("poolacl"), tr("Pools")},
        {QStringLiteral("filesetacl"), tr("FileSets")},
        {QStringLiteral("catalogacl"), tr("Catalogs")},
        {QStringLiteral("commandacl"), tr("Commands")},
    };
}

QList<BAclMatrix::Type> BAclMatrix::patternTypes()
{
    return {
        {QStringLiteral("whereacl"), tr("Where")},
        {QStringLiteral("pluginoptionsacl"), tr("Plugin Options")},
    };
}

bool BAclMatrix::parse(const QString &reply, QList<BConsoleAcl> *consoles, QString *error)
{
    auto fail = [error](const QString &why) {
        if (error) *error = why;
        return false;
    };
    const int start = reply.indexOf(QLatin1Char('{'));
    const QJsonDocument doc = QJsonDocument::fromJson(start >= 0 ? reply.mid(start).toUtf8() : QByteArray());
    if (!doc.isObject()) {
        const QString text = reply.trimmed();
        return fail(text.isEmpty() ? tr("The Director sent no reply.")
                                   : tr("The Director did not answer with ACLs: %1").arg(text.left(300)));
    }
    QJsonObject root = doc.object();
    if (root.contains(QStringLiteral("error"))) {
        const QJsonObject e = root.value(QStringLiteral("error")).toObject();
        QString message = e.value(QStringLiteral("message")).toString();
        const QJsonValue data = e.value(QStringLiteral("data"));
        const QString detail = QString::fromUtf8(QJsonDocument(data.toObject()).toJson(QJsonDocument::Compact));
        if (data.isObject() && !data.toObject().isEmpty()) message += QStringLiteral(" ") + detail;
        return fail(tr("The Director refused \".acl list\": %1").arg(message));
    }
    if (root.contains(QStringLiteral("result")) && !root.contains(QStringLiteral("acl")))
        root = root.value(QStringLiteral("result")).toObject();
    if (!root.value(QStringLiteral("acl")).isArray())
        return fail(tr("The reply holds no list of consoles (\"acl\")."));

    QList<BConsoleAcl> out;
    const QList<Type> all = types() + patternTypes();
    for (const QJsonValue &value : root.value(QStringLiteral("acl")).toArray()) {
        const QJsonObject c = value.toObject();
        BConsoleAcl console;
        console.name = c.value(QStringLiteral("console")).toString();
        if (console.name.isEmpty()) continue;
        for (const QJsonValue &p : c.value(QStringLiteral("profiles")).toArray())
            console.profiles.append(p.toString());
        console.locked = truth(field(c, QStringLiteral("locked")));
        const QJsonObject state = field(c, QStringLiteral("catalog_state")).toObject();
        console.catalogState = state.value(QStringLiteral("state")).toString();
        console.catalogError = state.value(QStringLiteral("error")).toString();
        const QJsonObject catalog = field(c, QStringLiteral("catalog")).toObject();
        const QJsonObject effective = field(c, QStringLiteral("effective")).toObject();
        for (const Type &type : all) {
            QList<BAclEntry> entries;
            for (const QJsonValue &e : c.value(type.key).toArray()) {
                const QJsonObject o = e.toObject();
                entries.append(BAclEntry{o.value(QStringLiteral("value")).toString(),
                                         o.value(QStringLiteral("source")).toString()});
            }
            console.file.insert(type.key, entries);
            QList<BAclCatalogEntry> rows;
            for (const QJsonValue &e : catalog.value(type.key).toArray()) {
                const QJsonObject o = e.toObject();
                rows.append(BAclCatalogEntry{o.value(QStringLiteral("value")).toString(),
                                             o.value(QStringLiteral("position")).toInt(),
                                             o.value(QStringLiteral("ineffective")).toBool()});
            }
            if (!rows.isEmpty()) console.catalog.insert(type.key, rows);
            if (effective.contains(type.key)) {
                const QJsonObject o = effective.value(type.key).toObject();
                BAclEffective eff;
                eff.source = o.value(QStringLiteral("source")).toString();
                if (o.value(QStringLiteral("allowed")).isArray()) {
                    QStringList allowed;
                    for (const QJsonValue &a : o.value(QStringLiteral("allowed")).toArray())
                        allowed.append(a.toString());
                    eff.allowed = allowed;
                }
                if (o.value(QStringLiteral("configured")).isDouble())
                    eff.configured = o.value(QStringLiteral("configured")).toInt();
                console.effective.insert(type.key, eff);
            }
        }
        out.append(console);
    }
    if (consoles) *consoles = out;
    return true;
}

BAclMatrix::Cell BAclMatrix::fileDecision(const QList<BAclEntry> &entries, const QString &name,
                                          const BAclEntry **decidedBy)
{
    // Bareos AclAccessOk(): a negation that matches denies first
    for (const BAclEntry &entry : entries) {
        if (entry.value.startsWith(QLatin1Char('!')) && matches(entry.value.mid(1), name)) {
            if (decidedBy) *decidedBy = &entry;
            return Cell::Denied;
        }
    }
    for (const BAclEntry &entry : entries) {
        if (entry.value.startsWith(QLatin1Char('!'))) continue;
        if (entry.value.compare(QLatin1String("*all*"), Qt::CaseInsensitive) == 0 || matches(entry.value, name)) {
            if (decidedBy) *decidedBy = &entry;
            return Cell::Allowed;
        }
    }
    if (decidedBy) *decidedBy = nullptr;
    return Cell::NotListed;
}

QList<BAclMatrix::Row> BAclMatrix::rows(const QList<BConsoleAcl> &consoles, const QString &type,
                                        const QStringList &known)
{
    // Every resource some console may use or names, and the known ones
    QStringList names;
    auto add = [&names](const QString &name) {
        if (name.isEmpty()) return;
        for (const QString &n : names) {
            if (n.compare(name, Qt::CaseInsensitive) == 0) return;
        }
        names.append(name);
    };
    for (const QString &name : known) add(name);
    for (const BConsoleAcl &console : consoles) {
        const auto eff = console.effective.constFind(type);
        if (eff != console.effective.constEnd() && eff->allowed) {
            for (const QString &name : *eff->allowed) add(name);
        }
        for (const BAclEntry &entry : console.file.value(type)) {
            const QString value = entry.value.startsWith(QLatin1Char('!')) ? entry.value.mid(1) : entry.value;
            if (isPlainName(value)) add(value);
        }
    }
    std::sort(names.begin(), names.end(), [](const QString &a, const QString &b) {
        return a.compare(b, Qt::CaseInsensitive) < 0;
    });

    QList<Row> out;
    for (const QString &name : names) {
        Row row;
        row.type = type;
        row.resource = name;
        for (const BConsoleAcl &console : consoles) {
            const QList<BAclEntry> entries = console.file.value(type);
            const BAclEntry *decided = nullptr;
            const Cell byFile = fileDecision(entries, name, &decided);
            const QString by = decided ? tr("%1 (%2)").arg(decided->value, decided->source) : QString();
            const auto eff = console.effective.constFind(type);
            const bool reported = eff != console.effective.constEnd() && eff->allowed;
            Cell cell = byFile;
            QString reason;
            if (console.locked) {
                cell = Cell::Locked;
                reason = tr("Locked: the catalog ACL of the console could not be read; it may run "
                            "status, .status, version and .api only.");
            } else if (reported) {
                bool allowed = false;
                for (const QString &a : *eff->allowed) {
                    if (a.compare(name, Qt::CaseInsensitive) == 0) allowed = true;
                }
                if (allowed) {
                    cell = Cell::Allowed;
                    reason = by.isEmpty() ? tr("Allowed (decided by the Director).") : tr("Allowed by %1.").arg(by);
                } else if (byFile == Cell::Allowed) {
                    cell = Cell::NarrowedByCatalog;
                    reason = tr("The file ACL allows it (%1), the catalog ACL does not.").arg(by);
                } else if (byFile == Cell::Denied) {
                    reason = tr("Denied by %1.").arg(by);
                } else {
                    cell = Cell::NotListed;
                    reason = entries.isEmpty() ? tr("The console has no ACL of this type: nothing is allowed.")
                                               : tr("No entry of the ACL allows it.");
                }
            } else {
                switch (byFile) {
                case Cell::Allowed: reason = tr("Allowed by %1.").arg(by); break;
                case Cell::Denied: reason = tr("Denied by %1.").arg(by); break;
                default:
                    reason = entries.isEmpty() ? tr("The console has no ACL of this type: nothing is allowed.")
                                               : tr("No entry of the ACL allows it.");
                    break;
                }
            }
            row.cells.append(cell);
            row.reasons.append(reason);
        }
        for (const Cell cell : row.cells) {
            if (cell != row.cells.first()) row.differs = true;
        }
        out.append(row);
    }
    return out;
}

QString BAclMatrix::consoleSummary(const BConsoleAcl &console)
{
    QStringList lines;
    lines.append(tr("Console %1").arg(console.name));
    lines.append(console.profiles.isEmpty() ? tr("Profiles: none")
                                            : tr("Profiles: %1").arg(console.profiles.join(QStringLiteral(", "))));
    if (console.locked)
        lines.append(tr("Locked: its catalog ACL could not be read."));
    if (!console.catalogState.isEmpty()) {
        QString state = console.catalogState;
        if (state == QLatin1String("loaded")) state = tr("loaded");
        else if (state == QLatin1String("off")) state = tr("off");
        else if (state == QLatin1String("table_missing")) state = tr("table missing");
        else if (state == QLatin1String("failed")) state = tr("failed");
        lines.append(console.catalogError.isEmpty() ? tr("Catalog ACL: %1").arg(state)
                                                    : tr("Catalog ACL: %1 (%2)").arg(state, console.catalogError));
    }
    int narrowing = 0;
    for (auto it = console.catalog.cbegin(); it != console.catalog.cend(); ++it) narrowing += it->size();
    if (narrowing > 0)
        lines.append(tr("%n catalog ACL entry(s) narrow the file ACL.", "", narrowing));
    for (const Type &type : patternTypes()) {
        QStringList values;
        for (const BAclEntry &entry : console.file.value(type.key))
            values.append(tr("%1 (%2)").arg(entry.value, entry.source));
        lines.append(tr("%1: %2").arg(type.title, values.isEmpty() ? tr("none") : values.join(QStringLiteral(", "))));
    }
    return lines.join(QLatin1Char('\n'));
}
