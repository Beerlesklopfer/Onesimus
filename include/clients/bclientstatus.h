// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BCLIENTSTATUS_H
#define BCLIENTSTATUS_H

#include <QDateTime>
#include <QString>
#include <QTimeZone>

/**
 * @brief Age of a client's last backup or contact, without widgets and without a Director
 *
 * The GUI does not know whether a client is reachable right now. What it
 * has is a timestamp: the Director's "lastconnection" or the start of the
 * client's newest job in the loaded job list. This module turns that
 * timestamp into an age and a state. It is deliberately not called
 * online/offline: a client that was backed up an hour ago can be switched
 * off, and one without a recent job can be perfectly reachable.
 */
namespace BClientStatus {

enum class Activity {
    Unknown,    ///< A timestamp is there but cannot be read
    Never,      ///< No timestamp: no backup or contact is known
    Recent,     ///< Younger than the threshold
    Stale       ///< As old as the threshold or older
};

constexpr int DefaultThresholdHours = 24;

struct Info {
    Activity activity = Activity::Never;
    QDateTime timestamp;    ///< In the Director's time zone; invalid for Never and Unknown
    qint64 ageSecs = -1;    ///< Never negative for Recent and Stale; -1 otherwise
    QString raw;            ///< The text the Director sent
};

/** True for the ways the Director writes "no time": empty, "0", "0000-00-00 00:00:00". */
bool isMissingTimestamp(const QString &text);

/**
 * @brief Reads a Director timestamp
 *
 * The Director sends its local wall-clock time without a zone
 * ("2026-11-18 09:12:33"). Such a time is interpreted in @p directorZone.
 * A time that names its zone (ISO 8601 with "Z" or an offset) keeps it.
 * @return An invalid QDateTime if the text is missing or unreadable
 */
QDateTime parseTimestamp(const QString &text, const QTimeZone &directorZone);

/**
 * @brief Classifies a timestamp against a fixed clock
 * @param timestamp      Text from the Director
 * @param now            Current time (any zone; compared as an instant)
 * @param thresholdHours Age from which the state is Stale; values < 1 mean the default
 * @param directorZone   Zone of timestamps that do not name one
 *
 * The age is never negative: a timestamp ahead of @p now (clock skew, or
 * a Director in another zone than assumed) counts as age 0.
 */
Info classify(const QString &timestamp, const QDateTime &now, int thresholdHours,
              const QTimeZone &directorZone);

/** "just now", "12 min ago", "3 h ago", "5 d ago" */
QString ageText(qint64 ageSecs);

/** Cell text: the age for Recent and Stale, "Never" or "Unknown" otherwise. */
QString label(const Info &info);

/** Name of the state: "Recent", "Stale", "Never", "Unknown". */
QString stateName(Activity activity);

/** Explanation with the timestamp, the limit and what the value is based on. */
QString toolTip(const Info &info, int thresholdHours);

/** Sort key: youngest first when ascending, then Never, then Unknown. */
qint64 sortKey(const Info &info);

} // namespace BClientStatus

#endif // BCLIENTSTATUS_H
