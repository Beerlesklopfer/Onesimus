#ifndef BRESOURCETEXT_H
#define BRESOURCETEXT_H

#include <QCoreApplication>
#include <QList>
#include <QString>
#include <QStringList>

/**
 * @file bresourcetext.h
 * @brief The text of one configuration resource as it stands in its file,
 *        and how a change of single directives is written into it
 *
 * ReadResource of the Onesimus Director returns a resource exactly as it
 * stands in the configuration file, from the type word ("Job", "JobDefs")
 * to the line end after the closing brace, with its comments, its order,
 * its quoting and its line ends (CRLF stays CRLF). A change is written back
 * into that text, never into a text made afresh:
 *
 *  - a line that holds no edited directive stays byte for byte, comments,
 *    unknown directives and nested blocks (Run Script { ... }) included;
 *  - a changed directive keeps its indentation, its spelling of the key, the
 *    spacing around "=", its quoting and its comment; only the value is
 *    replaced;
 *  - a removed directive goes with its line (and the comment on it);
 *  - a new directive is put in front of the closing brace, with the
 *    indentation and the line end of the directives there;
 *  - the result is read again and checked: the edited directives have
 *    exactly the intended values and every other line is unchanged.
 *
 * What cannot be done safely is refused, never guessed: a directive that
 * appears more than once, one that shares its line with another directive,
 * a list value ("a", "b"), a text that is not one resource. The lexical
 * helpers are shared with BScheduleResource. QtCore only.
 */
class BResourceText
{
    Q_DECLARE_TR_FUNCTIONS(BResourceText)

public:
    // --- Lexical helpers (also used by BScheduleResource) ------------------

    /// Where the comment of a line starts (# or // outside quotes); -1 if none
    static int commentStart(const QString &line);
    /// The line without its line end
    static QString lineBody(const QString &line);
    /// The line with the inside of quoted parts blanked, to look for
    /// keywords and braces without matching inside a value
    static QString unquotedPart(const QString &line);
    /// The inside of a quoted value without its escapes
    static QString unescape(const QString &quotedBody);
    /// A value as Bareos reads it in a configuration file: quoted, with \ and " escaped
    static QString quoted(const QString &value);
    /// "Full Backup Pool" -> "fullbackuppool": how keys are compared
    static QString normalizedKey(const QString &key);
    /// Does a value need quotes to be read back as the same value?
    static bool needsQuotes(const QString &value);

    /**
     * @brief Splits a text into its lines, each with its own line end
     * @param lineEnds "\n", "\r\n" or "" (the last line without one)
     * @param lineEnd  The style of the text: the first line end found, else "\n"
     */
    static void splitLines(const QString &text, QStringList *lines, QStringList *lineEnds,
                           QString *lineEnd);

    // --- One resource -------------------------------------------------------

    /// One directive on a line of its own, at the top level of the resource
    struct Directive
    {
        int lineIndex = -1;
        QString key;            ///< As written ("FileSet", "Full Backup Pool")
        QString normalized;     ///< normalizedKey(key)
        int valueStart = 0;     ///< In the line, where the value starts (at the quote if quoted)
        int valueEnd = 0;       ///< In the line, where the value ends (after the quote if quoted)
        bool quoted = false;
        QString value;          ///< Without quotes and escapes
    };

    struct Parsed
    {
        bool ok = false;
        QString error;
        QString typeWord;           ///< "Job", "JobDefs", as written
        QString name;               ///< Name = ... (unquoted)
        QStringList lines;          ///< Each with its own line end
        QStringList lineEnds;
        QString lineEnd;            ///< Style of the text
        int openLine = -1;          ///< Line of "Job {"
        int closingLine = -1;       ///< Line of the closing brace
        QList<Directive> directives;///< Top level, in file order
        /// Normalized keys that appear on lines this class does not edit
        /// (several directives on a line, list values, nested blocks); an
        /// edit of such a key is refused
        QStringList opaqueKeys;
        QList<int> opaqueLines;     ///< Those lines (nested blocks included)

        /// Index into directives of the only directive with this key; -1 if
        /// there is none, -2 if there are several
        int find(const QString &key) const;
        /// The value of a directive set here ("" if not set)
        QString value(const QString &key) const;
        bool has(const QString &key) const { return find(key) != -1; }
    };

    static Parsed parse(const QString &text);

    /// A change of one directive
    struct Change
    {
        QString key;            ///< As it is to be written if the directive is new ("FileSet")
        QString value;          ///< Without quotes; ignored for remove
        bool remove = false;

        static Change set(const QString &key, const QString &value) { return Change{key, value, false}; }
        static Change unset(const QString &key) { return Change{key, QString(), true}; }
    };

    /**
     * @brief Writes the changes into the resource text
     * @return false with @p error if it cannot be done safely; @p result is
     *         then untouched. A change that changes nothing is no error.
     */
    static bool edit(const QString &raw, const QList<Change> &changes, QString *result,
                     QString *error = nullptr);

    /**
     * @brief The text of a new resource, for CreateResource
     *
     *     <type> {
     *       <key> = <value>
     *       ...
     *     }
     *
     * Values are quoted where needQuotes() says so; the name always is.
     */
    static QString createText(const QString &typeWord, const QList<Change> &directives,
                              const QString &lineEnd = QStringLiteral("\n"));

    // --- Comparing two texts -------------------------------------------------

    struct DiffLine
    {
        enum Kind { Same, Removed, Added };
        Kind kind = Same;
        QString text;           ///< Without its line end
    };

    /// A line diff of two texts (longest common subsequence of lines)
    static QList<DiffLine> diff(const QString &oldText, const QString &newText);
};

#endif // BRESOURCETEXT_H
