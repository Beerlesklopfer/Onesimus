// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

/**
 * @file bquote.h
 * @brief The one quoting helper for bconsole command lines and generated
 *        resource text
 *
 * Bareos reads both the same way: a double quote starts or ends a quoted
 * value and a backslash takes the next character literally (next_arg() in
 * core/src/lib/scan.cc for command lines, the lex_quoted_string state in
 * core/src/lib/lex.cc for configuration text). So one escaping serves both:
 * backslash and double quote get a backslash, everything is put in double
 * quotes. Control characters (a newline ends a command line and would start
 * a new directive in a resource) cannot be escaped; they are reported.
 */
#ifndef BQUOTE_H
#define BQUOTE_H

#include <QString>

namespace BQuote {

/// true when @p value contains a character below U+0020 or DEL
inline bool hasControlCharacter(const QString &value)
{
    for (const QChar c : value) {
        if (c.unicode() < 0x20 || c.unicode() == 0x7f)
            return true;
    }
    return false;
}

/**
 * @brief @p value in double quotes with backslash and double quote escaped
 * @param ok set to false when @p value contains a control character; the
 *        caller must then not send or write the result
 */
inline QString quoted(const QString &value, bool *ok = nullptr)
{
    if (ok)
        *ok = !hasControlCharacter(value);
    QString out;
    out.reserve(value.size() + 2);
    out += QLatin1Char('"');
    for (const QChar c : value) {
        if (c == QLatin1Char('\\') || c == QLatin1Char('"'))
            out += QLatin1Char('\\');
        out += c;
    }
    out += QLatin1Char('"');
    return out;
}

/// The value between the quotes of a quoted value, escapes removed
inline QString unescape(const QString &quotedBody)
{
    QString out;
    out.reserve(quotedBody.size());
    for (int i = 0; i < quotedBody.size(); ++i) {
        if (quotedBody[i] == QLatin1Char('\\') && i + 1 < quotedBody.size())
            ++i;
        out.append(quotedBody[i]);
    }
    return out;
}

/**
 * @brief The first character of @p value that `configure add` cannot take,
 *        or a null QChar when it can take every character
 *
 * The Director (Bareos config_get_res_item()/config_add_directive() in
 * core/src/dird/ua_configure.cc, the same in the Onesimus Director) refuses
 * ';' and '@', and writes the value of a string, name, path or password
 * directive between double quotes without escaping it: a '"' would end the
 * value early and a '\' would escape the next character when the file is
 * read again. With @p writtenQuoted false (directives written as they are,
 * such as Run) only ';', '@' and control characters are refused.
 */
inline QChar configureAddForbidden(const QString &value, bool writtenQuoted = true)
{
    for (const QChar c : value) {
        if (c.unicode() < 0x20 || c.unicode() == 0x7f || c == QLatin1Char(';') || c == QLatin1Char('@'))
            return c;
        if (writtenQuoted && (c == QLatin1Char('"') || c == QLatin1Char('\\')))
            return c;
    }
    return QChar();
}

/**
 * @brief true when @p name is a valid Bareos resource name
 *
 * The rule of IsNameValid() (core/src/lib/edit.cc): ASCII letters, digits
 * and ":.-_/ " only, no leading or trailing blank, 1 to 127 characters
 * (MAX_NAME_LENGTH 128 including the terminating zero).
 */
inline bool isValidName(const QString &name)
{
    if (name.isEmpty() || name.size() > 127)
        return false;
    if (name.startsWith(QLatin1Char(' ')) || name.endsWith(QLatin1Char(' ')))
        return false;
    for (const QChar c : name) {
        const ushort u = c.unicode();
        const bool ok = (u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z') || (u >= '0' && u <= '9')
                     || u == ':' || u == '.' || u == '-' || u == '_' || u == '/' || u == ' ';
        if (!ok)
            return false;
    }
    return true;
}

} // namespace BQuote

#endif // BQUOTE_H
