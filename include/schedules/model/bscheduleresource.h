#ifndef BSCHEDULERESOURCE_H
#define BSCHEDULERESOURCE_H

#include <QCoreApplication>
#include <QList>
#include <QString>
#include <QStringList>

#include "schedules/model/brunspec.h"

/**
 * @file bscheduleresource.h
 * @brief The text of a Schedule resource as it stands in its file, and how
 *        a change of its Run lines is written into it
 *
 * ReadResource of the Onesimus Director returns a resource exactly as it
 * stands in the configuration file, from "Schedule" to the line end after
 * the closing brace, with its comments, its order and its line ends
 * (CRLF stays CRLF). A change is written back into that text, not into a
 * text made afresh:
 *
 *  - every line that is not a Run directive stays byte for byte;
 *  - a Run line that was not touched stays byte for byte, comment included;
 *  - a changed Run line keeps its indentation, its "Run =" with its spacing,
 *    its quoting and its comment; only the value is replaced;
 *  - a removed Run line goes with its line end;
 *  - a new Run line follows the line it was cut from or painted after, with
 *    the indentation, the quoting and the line end of the Run lines there;
 *  - the result is read again and checked: its Run values are exactly the
 *    intended ones and everything that is not a Run line is unchanged.
 *
 * What cannot be done safely is refused, never guessed: a Run directive
 * that shares its line with another directive, a value that does not end
 * where the line ends, a resource that is not one Schedule. QtCore only.
 */
class BScheduleResource
{
    Q_DECLARE_TR_FUNCTIONS(BScheduleResource)

public:
    /// One Run directive of the text
    struct RunDirective
    {
        int lineIndex = -1;     ///< Index into lines
        int valueStart = 0;     ///< In the line, where the value starts (at the quote if quoted)
        int valueEnd = 0;       ///< In the line, where the value ends (after the quote if quoted)
        bool quoted = false;
        QString value;          ///< Without quotes
    };

    /// The text split into its lines and Run directives
    struct Parsed
    {
        bool ok = false;
        QString error;
        QStringList lines;          ///< Each with its own line end ("" for the last if there is none)
        QStringList lineEnds;       ///< "\n", "\r\n" or ""
        QList<RunDirective> runs;
        QString name;               ///< Name = ... (unquoted)
        int closingLine = -1;       ///< Line of the closing brace
        QString lineEnd;            ///< The style of the text: "\r\n" if its first line end is one
    };

    static Parsed parse(const QString &text);

    /// The Run values of a resource text, in order; false with @p error if
    /// the text cannot be read safely
    static bool runValues(const QString &text, QStringList *values, QString *error = nullptr);

    /**
     * @brief Do the Run lines of the file say what the GUI loaded?
     *
     * The loaded lines come from "show schedules". Each line of the file has
     * to be the loaded line at the same position: the same text, or a text
     * that is understood and means the same (BRunSpec equal). A line the GUI
     * does not understand matches its text only.
     */
    static bool matchesLoaded(const Parsed &parsed, const QList<BRunLine> &loaded, QString *error = nullptr);

    /// One Run line of the schedule after the change
    struct Line
    {
        int origin = -1;    ///< Index of the line in the loaded schedule (= in the file); -1 for a new line
        bool keep = true;   ///< origin >= 0: the line of the file stays as it is
        QString text;       ///< What to write if not kept (a Run value, without "Run =")
    };

    /**
     * @brief Writes the Run lines into the resource text
     * @param raw    The text from ReadResource
     * @param lines  The Run lines in their new order; origins of kept and
     *               changed lines in rising order
     * @param result The new text
     * @return false with @p error if it cannot be done safely; @p result is
     *         then untouched
     */
    static bool rewrite(const QString &raw, const QList<Line> &lines, QString *result,
                        QString *error = nullptr);

    /**
     * @brief The text of a new Schedule resource, for CreateResource
     *
     *     Schedule {
     *       Name = "<name>"
     *       Description = "<description>"     (if not empty)
     *       Run = <run>
     *       ...
     *     }
     */
    static QString createText(const QString &name, const QString &description, const QStringList &runs,
                              const QString &lineEnd = QStringLiteral("\n"));

    /// A value as Bareos reads it in a configuration file: quoted, with \ and " escaped
    static QString quoted(const QString &value);
};

#endif // BSCHEDULERESOURCE_H
