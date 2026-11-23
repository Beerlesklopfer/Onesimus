// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BSORTVALUE_H
#define BSORTVALUE_H

#include <QDateTime>
#include <QJsonValue>
#include <QRegularExpression>
#include <QString>
#include <limits>

/**
 * @brief Sort keys for table columns, without widgets
 *
 * Tables show numbers, sizes and times as formatted text ("1.50 GB",
 * "18.11.26 09:12"). Sorting that text orders 1, 10, 100, 2. Models
 * therefore return these keys under their sort role and the view sorts
 * by that role. All keys of one column have the same type (qint64), so
 * QVariant comparison is numeric.
 */
namespace BSortValue {

/** Key of a missing or unreadable value: sorts before every real value. */
constexpr qint64 Missing = std::numeric_limits<qint64>::min();

/** Whole number from text ("42", " 1024 "); Missing if it is not a number. */
inline qint64 integer(const QString &text)
{
    bool ok = false;
    const qint64 value = text.trimmed().toLongLong(&ok);
    return ok ? value : Missing;
}

/** Whole number from a JSON value; the Director sends numbers as strings or as numbers. */
inline qint64 integer(const QJsonValue &value)
{
    if (value.isString()) {
        return integer(value.toString());
    }
    if (value.isDouble()) {
        return static_cast<qint64>(value.toDouble());
    }
    return Missing;
}

/**
 * @brief Milliseconds since the epoch of "yyyy-MM-dd HH:mm:ss" or an ISO 8601 time
 *
 * A time without zone is read as local time, which keeps the order of
 * values from one Director. Missing for empty, "0" and unreadable text.
 */
inline qint64 timestamp(const QString &text)
{
    const QString trimmed = text.trimmed();
    if (trimmed.isEmpty() || trimmed == QLatin1String("0")) {
        return Missing;
    }
    QDateTime dateTime = QDateTime::fromString(trimmed, Qt::ISODate);
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(trimmed, QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    }
    return dateTime.isValid() ? dateTime.toMSecsSinceEpoch() : Missing;
}

/**
 * @brief Seconds of a duration "HH:MM:SS", also with a leading "N day(s) "
 *
 * Hours may exceed 23 ("120:05:00"). Missing for empty or unreadable text.
 */
inline qint64 durationSeconds(const QString &text)
{
    static const QRegularExpression pattern(
        QStringLiteral(R"(^\s*(?:(\d+)\s+days?,?\s+)?(\d+):(\d{1,2}):(\d{1,2})(?:\.\d+)?\s*$)"));
    const QRegularExpressionMatch match = pattern.match(text);
    if (!match.hasMatch()) {
        return Missing;
    }
    return match.captured(1).toLongLong() * 86400
           + match.captured(2).toLongLong() * 3600
           + match.captured(3).toLongLong() * 60
           + match.captured(4).toLongLong();
}

} // namespace BSortValue

#endif // BSORTVALUE_H
