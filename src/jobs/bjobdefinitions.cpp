/**
 * @file bjobdefinitions.cpp
 * @brief The Job and JobDefs resources with their directives resolved over
 *        the JobDefs chain
 */

#include "jobs/bjobdefinitions.h"

#include <QHash>
#include <QJsonArray>
#include <QSet>

#include <algorithm>

namespace {

constexpr int MaxChainDepth = 8;

const QHash<QString, QString> &labels()
{
    static const QHash<QString, QString> table = {
        {QStringLiteral("name"), QStringLiteral("Name")},
        {QStringLiteral("description"), QStringLiteral("Description")},
        {QStringLiteral("jobdefs"), QStringLiteral("JobDefs")},
        {QStringLiteral("type"), QStringLiteral("Type")},
        {QStringLiteral("level"), QStringLiteral("Level")},
        {QStringLiteral("client"), QStringLiteral("Client")},
        {QStringLiteral("fileset"), QStringLiteral("FileSet")},
        {QStringLiteral("schedule"), QStringLiteral("Schedule")},
        {QStringLiteral("storage"), QStringLiteral("Storage")},
        {QStringLiteral("pool"), QStringLiteral("Pool")},
        {QStringLiteral("messages"), QStringLiteral("Messages")},
        {QStringLiteral("priority"), QStringLiteral("Priority")},
        {QStringLiteral("enabled"), QStringLiteral("Enabled")},
        {QStringLiteral("fullbackuppool"), QStringLiteral("Full Backup Pool")},
        {QStringLiteral("incrementalbackuppool"), QStringLiteral("Incremental Backup Pool")},
        {QStringLiteral("differentialbackuppool"), QStringLiteral("Differential Backup Pool")},
        {QStringLiteral("nextpool"), QStringLiteral("Next Pool")},
        {QStringLiteral("catalog"), QStringLiteral("Catalog")},
        {QStringLiteral("maximumconcurrentjobs"), QStringLiteral("Maximum Concurrent Jobs")},
        {QStringLiteral("writebootstrap"), QStringLiteral("Write Bootstrap")},
        {QStringLiteral("bootstrap"), QStringLiteral("Bootstrap")},
        {QStringLiteral("maxruntime"), QStringLiteral("Max Run Time")},
        {QStringLiteral("maxwaittime"), QStringLiteral("Max Wait Time")},
        {QStringLiteral("maxstartdelay"), QStringLiteral("Max Start Delay")},
        {QStringLiteral("fullmaxruntime"), QStringLiteral("Full Max Runtime")},
        {QStringLiteral("incrementalmaxruntime"), QStringLiteral("Incremental Max Runtime")},
        {QStringLiteral("differentialmaxruntime"), QStringLiteral("Differential Max Runtime")},
        {QStringLiteral("maxfullinterval"), QStringLiteral("Max Full Interval")},
        {QStringLiteral("accurate"), QStringLiteral("Accurate")},
        {QStringLiteral("spooldata"), QStringLiteral("Spool Data")},
        {QStringLiteral("spoolattributes"), QStringLiteral("Spool Attributes")},
        {QStringLiteral("rescheduleonerror"), QStringLiteral("Reschedule On Error")},
        {QStringLiteral("rescheduleinterval"), QStringLiteral("Reschedule Interval")},
        {QStringLiteral("rescheduletimes"), QStringLiteral("Reschedule Times")},
        {QStringLiteral("allowmixedpriority"), QStringLiteral("Allow Mixed Priority")},
        {QStringLiteral("allowduplicatejobs"), QStringLiteral("Allow Duplicate Jobs")},
        {QStringLiteral("prefermountedvolumes"), QStringLiteral("Prefer Mounted Volumes")},
        {QStringLiteral("prunejobs"), QStringLiteral("Prune Jobs")},
        {QStringLiteral("prunefiles"), QStringLiteral("Prune Files")},
        {QStringLiteral("prunevolumes"), QStringLiteral("Prune Volumes")},
        {QStringLiteral("runscript"), QStringLiteral("Run Script")},
        {QStringLiteral("run"), QStringLiteral("Run")},
        {QStringLiteral("where"), QStringLiteral("Where")},
        {QStringLiteral("replace"), QStringLiteral("Replace")},
        {QStringLiteral("jobtoverify"), QStringLiteral("Job To Verify")},
        {QStringLiteral("selectiontype"), QStringLiteral("Selection Type")},
        {QStringLiteral("selectionpattern"), QStringLiteral("Selection Pattern")},
        {QStringLiteral("protocol"), QStringLiteral("Protocol")},
        {QStringLiteral("backupformat"), QStringLiteral("Backup Format")},
    };
    return table;
}

} // namespace

// ============================================================================
// Directives
// ============================================================================

QStringList BJobDefinitions::commonKeys()
{
    return {QStringLiteral("jobdefs"), QStringLiteral("type"), QStringLiteral("level"),
            QStringLiteral("client"), QStringLiteral("fileset"), QStringLiteral("schedule"),
            QStringLiteral("storage"), QStringLiteral("pool"), QStringLiteral("messages"),
            QStringLiteral("priority"), QStringLiteral("enabled"), QStringLiteral("description")};
}

QString BJobDefinitions::label(const QString &key)
{
    const QString normalized = BResourceText::normalizedKey(key);
    return labels().value(normalized, key);
}

QString BJobDefinitions::displayText(const QJsonValue &value)
{
    switch (value.type()) {
    case QJsonValue::String:
        return value.toString();
    case QJsonValue::Bool:
        return value.toBool() ? QStringLiteral("yes") : QStringLiteral("no");
    case QJsonValue::Double: {
        const double d = value.toDouble();
        if (d == double(qint64(d)))
            return QString::number(qint64(d));
        return QString::number(d);
    }
    case QJsonValue::Object: {
        const QJsonObject object = value.toObject();
        if (object.value(QLatin1String("name")).isString())
            return object.value(QLatin1String("name")).toString();
        return tr("(block)");
    }
    case QJsonValue::Array: {
        const QJsonArray array = value.toArray();
        QStringList parts;
        int blocks = 0;
        for (const QJsonValue &item : array) {
            if (item.isObject() && !item.toObject().value(QLatin1String("name")).isString())
                ++blocks;
            else
                parts.append(displayText(item));
        }
        if (blocks > 0)
            parts.append(tr("(%n block(s))", "", blocks));
        return parts.join(QLatin1String(", "));
    }
    case QJsonValue::Null:
    case QJsonValue::Undefined:
        break;
    }
    return QString();
}

QJsonValue BJobDefinitions::directiveOf(const QJsonObject &config, const QString &key)
{
    const QString wanted = BResourceText::normalizedKey(key);
    const auto direct = config.constFind(wanted);
    if (direct != config.constEnd())
        return direct.value();
    for (auto it = config.constBegin(); it != config.constEnd(); ++it) {
        if (BResourceText::normalizedKey(it.key()) == wanted)
            return it.value();
    }
    return QJsonValue(QJsonValue::Undefined);
}

// ============================================================================
// Resources
// ============================================================================

void BJobDefinitions::setResources(const QMap<QString, QJsonObject> &jobs, const QMap<QString, QJsonObject> &jobDefs)
{
    m_jobs = jobs;
    m_jobDefs = jobDefs;
}

QList<BJobDefinitions::Resource> BJobDefinitions::resources() const
{
    QList<Resource> out;
    for (auto it = m_jobs.constBegin(); it != m_jobs.constEnd(); ++it)
        out.append(Resource{Kind::Job, it.key(), it.value()});
    for (auto it = m_jobDefs.constBegin(); it != m_jobDefs.constEnd(); ++it)
        out.append(Resource{Kind::JobDefs, it.key(), it.value()});
    return out;
}

bool BJobDefinitions::contains(Kind kind, const QString &name) const
{
    return map(kind).contains(name);
}

QJsonObject BJobDefinitions::config(Kind kind, const QString &name) const
{
    return map(kind).value(name);
}

QString BJobDefinitions::jobDefsOf(Kind kind, const QString &name) const
{
    return displayText(directiveOf(config(kind, name), QStringLiteral("jobdefs")));
}

QStringList BJobDefinitions::chain(Kind kind, const QString &name) const
{
    QStringList out;
    QString next = jobDefsOf(kind, name);
    QSet<QString> seen;
    if (kind == Kind::JobDefs)
        seen.insert(name);
    while (!next.isEmpty() && out.size() < MaxChainDepth && !seen.contains(next)) {
        seen.insert(next);
        out.append(next);
        if (!m_jobDefs.contains(next))
            break;      // not reported: the chain ends here, unknown beyond
        next = jobDefsOf(Kind::JobDefs, next);
    }
    return out;
}

BJobDefinitions::Value BJobDefinitions::value(Kind kind, const QString &name, const QString &key) const
{
    Value result;
    result.key = BResourceText::normalizedKey(key);
    result.label = label(result.key);

    const QJsonValue own = directiveOf(config(kind, name), result.key);
    if (!own.isUndefined() && !own.isNull()) {
        result.set = true;
        result.here = true;
        result.text = displayText(own);
        return result;
    }
    // The JobDefs a resource names is its own directive, never inherited
    if (result.key == QLatin1String("jobdefs"))
        return result;
    for (const QString &defs : chain(kind, name)) {
        if (!m_jobDefs.contains(defs)) {
            result.uncertain = true;
            break;
        }
        const QJsonValue inherited = directiveOf(m_jobDefs.value(defs), result.key);
        if (!inherited.isUndefined() && !inherited.isNull()) {
            result.set = true;
            result.from = defs;
            result.text = displayText(inherited);
            return result;
        }
    }
    return result;
}

namespace {

/// The common keys first, then the others by label
QStringList orderedKeys(const QSet<QString> &keys)
{
    QStringList out = BJobDefinitions::commonKeys();
    QStringList rest;
    for (const QString &key : keys) {
        if (!out.contains(key) && key != QLatin1String("name"))
            rest.append(key);
    }
    std::sort(rest.begin(), rest.end(), [](const QString &a, const QString &b) {
        return BJobDefinitions::label(a).compare(BJobDefinitions::label(b), Qt::CaseInsensitive) < 0;
    });
    out.append(rest);
    return out;
}

} // namespace

QList<BJobDefinitions::Value> BJobDefinitions::values(Kind kind, const QString &name) const
{
    QSet<QString> keys;
    auto collect = [&keys](const QJsonObject &object) {
        for (auto it = object.constBegin(); it != object.constEnd(); ++it)
            keys.insert(BResourceText::normalizedKey(it.key()));
    };
    collect(config(kind, name));
    for (const QString &defs : chain(kind, name))
        collect(m_jobDefs.value(defs));

    QList<Value> out;
    for (const QString &key : orderedKeys(keys))
        out.append(value(kind, name, key));
    return out;
}

QList<BJobDefinitions::Value> BJobDefinitions::resolveWithText(Kind kind, const QString &name,
                                                               const BResourceText::Parsed &text) const
{
    if (!text.ok)
        return values(kind, name);

    QSet<QString> keys;
    for (const BResourceText::Directive &directive : text.directives)
        keys.insert(directive.normalized);
    for (const QString &key : text.opaqueKeys)
        keys.insert(key);
    for (const Value &value : values(kind, name))
        keys.insert(value.key);

    QList<Value> out;
    for (const QString &key : orderedKeys(keys)) {
        Value result;
        result.key = key;
        result.label = label(key);
        QStringList written;
        for (const BResourceText::Directive &directive : text.directives) {
            if (directive.normalized == key) {
                written.append(directive.value);
                if (result.label == key)
                    result.label = directive.key;   // an unknown key as the text spells it
            }
        }
        if (!written.isEmpty() || text.opaqueKeys.contains(key)) {
            result.set = true;
            result.here = true;
            result.text = written.isEmpty() ? tr("(see the text)") : written.join(QLatin1String(", "));
            out.append(result);
            continue;
        }
        if (key == QLatin1String("jobdefs")) {
            out.append(result);
            continue;
        }
        // Not in the text: from the chain, whatever "show" printed for the resource
        for (const QString &defs : chain(kind, name)) {
            if (!m_jobDefs.contains(defs)) {
                result.uncertain = true;
                break;
            }
            const QJsonValue inherited = directiveOf(m_jobDefs.value(defs), key);
            if (!inherited.isUndefined() && !inherited.isNull()) {
                result.set = true;
                result.from = defs;
                result.text = displayText(inherited);
                break;
            }
        }
        out.append(result);
    }
    return out;
}

QStringList BJobDefinitions::usersOf(const QString &jobDefsName) const
{
    QStringList out;
    for (auto it = m_jobs.constBegin(); it != m_jobs.constEnd(); ++it) {
        if (chain(Kind::Job, it.key()).contains(jobDefsName))
            out.append(QStringLiteral("Job %1").arg(it.key()));
    }
    for (auto it = m_jobDefs.constBegin(); it != m_jobDefs.constEnd(); ++it) {
        if (it.key() != jobDefsName && chain(Kind::JobDefs, it.key()).contains(jobDefsName))
            out.append(QStringLiteral("JobDefs %1").arg(it.key()));
    }
    return out;
}

QStringList BJobDefinitions::jobsUsing(const QString &jobDefsName) const
{
    QStringList out;
    for (auto it = m_jobs.constBegin(); it != m_jobs.constEnd(); ++it) {
        if (chain(Kind::Job, it.key()).contains(jobDefsName))
            out.append(it.key());
    }
    return out;
}
