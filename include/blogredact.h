/**
 * @file blogredact.h
 * @brief Removes secrets from text before it is logged
 *
 * Command lines sent to the Director can carry a password in clear text
 * (configure add console ... password="secret"). Every place that logs a
 * command line passes it through bRedactCommand() first.
 */
#ifndef BLOGREDACT_H
#define BLOGREDACT_H

#include <QRegularExpression>
#include <QString>

/**
 * @brief Mask the value of every "password=" in a command line
 *
 * Matches any key ending in "password" in any case (password=, Password = ,
 * tlspskpassword=), with a quoted or an unquoted value; a quoted value may
 * contain spaces and escaped quotes, and a missing closing quote masks the
 * rest of the line. White space after the '=' belongs to the match, so
 * "password= name=x" loses "name=x" too: masking too much is the safe side.
 *
 * @return the text with each value replaced by ***
 */
inline QString bRedactCommand(const QString &commandLine)
{
    static const QRegularExpression passwordValue(
        QStringLiteral("(password\\s*=\\s*)"           // key, kept
                       "(?:\"(?:[^\"\\\\]|\\\\.)*\"?"   // quoted value
                       "|[^\\s\"](?:[^\\s\"]|\"[^\"]*\"?)*"  // unquoted value
                       ")?"),
        QRegularExpression::CaseInsensitiveOption);

    QString redacted = commandLine;
    redacted.replace(passwordValue, QStringLiteral("\\1***"));
    return redacted;
}

#endif // BLOGREDACT_H
