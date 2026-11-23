// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BRUNJOBCOMMAND_H
#define BRUNJOBCOMMAND_H

#include <QString>

/**
 * @brief Builds the bconsole `run` command line of the "Run Job" dialog
 *
 * Plain functions without widgets, so that the command can be tested
 * without a Director. The dialog shows and sends exactly the string
 * returned by build().
 */
namespace BRunJobCommand {

/// Input values as the dialog shows them; an empty value is left out
struct Params {
    QString job;
    QString client;
    QString fileset;
    QString pool;
    QString storage;
    QString level;
    int priority = 0;       ///< 0 = not given, the Job resource decides
    QString when;           ///< "yyyy-MM-dd HH:mm:ss" or empty for immediately
    QString bootstrap;      ///< Path on the Director or empty
};

/// Reason why no command could be built
enum class Error {
    None,
    MissingJob,
    ControlCharacter,   ///< A value contains a newline or another control character
    InvalidWhen,        ///< `when` is not a valid "yyyy-MM-dd HH:mm:ss" time
    InvalidPriority
};

struct Result {
    QString command;        ///< Empty when error != None
    Error error = Error::None;
    QString field;          ///< Keyword of the offending value (e.g. "pool")

    bool ok() const { return error == Error::None; }
};

/**
 * @brief Put a value in double quotes for a bconsole command line (BQuote::quoted())
 * @param value Raw value
 * @param ok Set to false when the value contains a control character
 * @return "value" with backslash and double quote escaped by a backslash
 *
 * A quoted value stays one argument even if it contains spaces, `=` or
 * keywords such as `yes`.
 */
QString quote(const QString &value, bool *ok = nullptr);

/**
 * @brief Build `run job=... yes` from the given values
 *
 * Names, the time and the path are always quoted. The level is quoted
 * unless it is a plain word. Nothing is sent to a Director here.
 */
Result build(const Params &params);

/**
 * @brief Build `estimate job=...` from job, client, fileset and level
 *
 * Quoted like build(); the other values are not part of the command and
 * are neither used nor checked. Nothing is sent to a Director here.
 */
Result buildEstimate(const Params &params);

} // namespace BRunJobCommand

#endif // BRUNJOBCOMMAND_H
