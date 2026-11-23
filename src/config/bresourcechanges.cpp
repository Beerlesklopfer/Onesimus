// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bresourcechanges.cpp
 * @brief Changes of the resource form as directive changes
 */

#include "config/bresourcechanges.h"
#include "config/bconfigparser.h"

#include <QMap>

QString BResourceChanges::comparable(const QString &value)
{
    QString v = value.trimmed();
    if (v.size() >= 2 && v.startsWith(QLatin1Char('"')) && v.endsWith(QLatin1Char('"')))
        v = v.mid(1, v.size() - 2).trimmed();
    const QString lower = v.toLower();
    if (lower == QLatin1String("true") || lower == QLatin1String("yes"))
        return QStringLiteral("yes");
    if (lower == QLatin1String("false") || lower == QLatin1String("no"))
        return QStringLiteral("no");
    return v;
}

BResourceChanges BResourceChanges::between(const BConfigResource &before, const BConfigResource &after,
                                           const QStringList &formDirectives, const DefaultOf &defaultOf)
{
    BResourceChanges result;

    // normalized key -> key as the resource has it
    QMap<QString, QString> beforeKeys;
    for (const QString &key : before.keys())
        beforeKeys.insert(BResourceText::normalizedKey(key), key);
    QMap<QString, QString> afterKeys;
    for (const QString &key : after.keys())
        afterKeys.insert(BResourceText::normalizedKey(key), key);

    const QString nameKey = BResourceText::normalizedKey(QStringLiteral("Name"));

    // BConfigResource keeps its keys in lower case: a directive is written
    // and named as the form spells its field ("FileSet")
    QMap<QString, QString> spelling;
    for (const QString &directive : formDirectives)
        spelling.insert(BResourceText::normalizedKey(directive), directive);

    // Set or changed in the form
    for (auto it = afterKeys.constBegin(); it != afterKeys.constEnd(); ++it) {
        if (it.key() == nameKey)
            continue;               // renaming is not a change of a directive
        const QString directive = spelling.value(it.key(), it.value());
        const QString now = after.value(it.value()).toString().trimmed();
        const auto old = beforeKeys.constFind(it.key());
        if (old == beforeKeys.constEnd()) {
            const QString def = defaultOf ? defaultOf(directive) : QString();
            if (!def.isEmpty() && comparable(def) == comparable(now))
                continue;           // a default the form shows, not a change
            result.changes.append(BResourceText::Change::set(directive, now));
            result.descriptions.append(QStringLiteral("%1: (not set) -> %2").arg(directive, now));
            continue;
        }
        const BConfigValue was = before.value(old.value());
        if (was.type() != BConfigValue::Simple) {
            if (comparable(was.toString()) != comparable(now))
                result.skipped.append(directive);
            continue;
        }
        if (comparable(was.simpleValue()) == comparable(now))
            continue;
        result.changes.append(BResourceText::Change::set(directive, now));
        result.descriptions.append(QStringLiteral("%1: %2 -> %3").arg(directive, was.simpleValue().trimmed(), now));
    }

    // Emptied in the form. The form drops empty fields, so a key that is
    // missing in "after" was emptied only if the form has a field for it.
    for (const QString &directive : formDirectives) {
        const QString key = BResourceText::normalizedKey(directive);
        if (key == nameKey || afterKeys.contains(key))
            continue;
        const auto old = beforeKeys.constFind(key);
        if (old == beforeKeys.constEnd())
            continue;
        const BConfigValue was = before.value(old.value());
        // An empty field for a list may only mean the form cannot show it
        if (was.type() != BConfigValue::Simple)
            continue;
        if (was.simpleValue().trimmed().isEmpty())
            continue;
        result.changes.append(BResourceText::Change::unset(directive));
        result.descriptions.append(QStringLiteral("%1: %2 -> (removed)").arg(directive, was.simpleValue().trimmed()));
    }
    return result;
}
