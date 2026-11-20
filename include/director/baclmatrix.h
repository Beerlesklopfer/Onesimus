#ifndef BACLMATRIX_H
#define BACLMATRIX_H

#include <QCoreApplication>
#include <QHash>
#include <QList>
#include <QString>
#include <QStringList>

#include <optional>

/**
 * @file baclmatrix.h
 * @brief The ACLs of every console as a matrix of resources and consoles,
 *        without widgets and without a Director
 *
 * Read from the reply of ".acl list" (Onesimus Director, API mode 2): per
 * console its file ACL (the entries of the Console and of its Profiles,
 * each with its source), and since odir 7e423f6 in "extra_fields" the
 * catalog ACL (table onesimus.console_acl, it can only narrow the file
 * ACL), the effective ACL (the resources the console may use, as the
 * Director decided it), the state of the catalog ACL and whether the
 * console is locked (its catalog ACL could not be read: only status,
 * .status, version and .api).
 *
 * The matrix shows, per type of ACL, one row per resource and one column
 * per console. Whether a cell is allowed is the Director's effective ACL
 * where it reports one; the file entries only explain it (the first entry
 * that decides, with its source). Without an effective ACL (an older
 * Director) the file entries decide as Bareos does (core/src/dird/ua_acl.cc
 * AclAccessOk): the first entry that matches decides, "!" denies, "*all*"
 * or a name allows; no match denies. Entries match the name exactly or as a
 * regular expression, both without regard to case.
 *
 * Editing (odir crates/odir/src/commands/acl.rs): only the catalog ACL can
 * be changed from here, with ".acl set console=<c> <type>acl=<e1>,<e2>" and
 * ".acl delete console=<c> <type>acl". A type without catalog entries
 * restricts nothing; a list is evaluated like a file list (first match).
 * So a cell that is allowed can be denied (an entry "!<name>" in front, or
 * "!<name>,*all*" for a type without entries), and a cell the catalog ACL
 * narrows can be allowed again (its "!<name>" removed, or "<name>" put in
 * front). What the file ACL denies cannot be allowed here. odir lets a
 * console change its own catalog ACL only to a narrower one.
 */

/** One entry of a file ACL. */
struct BAclEntry
{
    QString value;      ///< As written, "!" included
    QString source;     ///< "Console" or "Profile <name>"
};

/** One row of the catalog ACL. */
struct BAclCatalogEntry
{
    QString value;
    int position = 0;
    bool ineffective = false;   ///< Has no effect within the file ACL
};

/** What the Director decided for one type of ACL. */
struct BAclEffective
{
    QString source;                     ///< "file", "file+catalog" or "locked"
    std::optional<QStringList> allowed; ///< nullopt: not a list of resources (Where, Plugin Options)
    std::optional<int> configured;      ///< Resources of the type in the configuration
};

/** The ACLs of one console, or of one profile (then without catalog ACL). */
struct BConsoleAcl
{
    QString name;
    QString description;    ///< Profiles: their Description
    QStringList profiles;
    bool locked = false;
    QString catalogState;   ///< "off", "table_missing", "loaded", "failed"; empty: not reported
    QString catalogError;
    QHash<QString, QList<BAclEntry>> file;              ///< By key ("jobacl")
    QHash<QString, QList<BAclCatalogEntry>> catalog;
    QHash<QString, BAclEffective> effective;            ///< Empty: not reported
};

class BAclMatrix
{
    Q_DECLARE_TR_FUNCTIONS(BAclMatrix)

public:
    /** A type of ACL with a list of resources. */
    struct Type
    {
        QString key;        ///< "jobacl"
        QString title;      ///< "Jobs", translated
    };

    /// The types shown as rows, in this order: jobs, clients, storages,
    /// schedules, pools, filesets, catalogs, commands
    static QList<Type> types();
    /// Where and Plugin Options: patterns, not resources; shown as text
    static QList<Type> patternTypes();

    enum class Cell {
        Allowed,
        NotListed,          ///< No entry allows it
        Denied,             ///< An entry with "!" denies it
        NarrowedByCatalog,  ///< The file ACL allows it, the catalog ACL does not
        Locked,             ///< The console is locked
    };

    struct Row
    {
        QString type;           ///< Key of the type; empty for nothing
        QString resource;
        QList<Cell> cells;      ///< One per console, in their order
        QStringList reasons;    ///< One per console: why, in words
        bool differs = false;   ///< Not every console has the same cell
    };

    /**
     * @brief Reads the reply of ".acl list"
     * @return false with @p error if it is no such reply (an error of the
     *         Director, a Director without the command)
     */
    static bool parse(const QString &reply, QList<BConsoleAcl> *consoles, QString *error = nullptr);

    /**
     * @brief The rows of one type
     * @param known Further names of resources of the type (from the
     *              resources the GUI loaded): rows even if no console may
     *              use them
     */
    static QList<Row> rows(const QList<BConsoleAcl> &consoles, const QString &type,
                           const QStringList &known = QStringList());

    /// Does the entry list of a file ACL allow @p name? With the entry that decides
    static Cell fileDecision(const QList<BAclEntry> &entries, const QString &name,
                             const BAclEntry **decidedBy = nullptr);

    /// The column's tool tip: profiles, catalog ACL, locked, the pattern ACLs
    static QString consoleSummary(const BConsoleAcl &console);

    // --- Editing the catalog ACL ----------------------------------------------

    // --- Profiles (file only: the GUI proposes the text, a person puts it in) --

    /// Reads the reply of "show profiles": one entry per profile, its ACL
    /// directives as file entries (source "Profile <name>")
    static bool parseProfiles(const QString &reply, QList<BConsoleAcl> *profiles, QString *error = nullptr);

    /// The entries of a file list after one cell is changed: denied (its
    /// own entry removed, "!<name>" in front if still allowed) or allowed
    /// (its negation removed, its name in front if still denied)
    static QStringList fileListAfter(const QStringList &entries, const QString &name, bool allow);

    /// The entries of a type in a file ACL as plain values
    static QStringList fileList(const BConsoleAcl &resource, const QString &type);

    /// The directive as Bareos spells it: "jobacl" -> "JobAcl"
    static QString directiveName(const QString &type);

    /**
     * @brief The text of a Profile resource with these ACL lists
     * @param lists By type; a type not in it keeps the profile's entries
     */
    static QString profileText(const BConsoleAcl &profile, const QHash<QString, QStringList> &lists);

    /// Does a list allow @p name (first match decides, no match denies)?
    static bool listAllows(const QStringList &entries, const QString &name);

    /// Can this cell be changed through the catalog ACL? Allowed or narrowed
    /// by the catalog, on a console whose catalog ACL is loaded
    static bool editable(const BConsoleAcl &console, Cell cell);

    /// The catalog list of a type as plain entries
    static QStringList catalogList(const BConsoleAcl &console, const QString &type);

    /**
     * @brief The catalog list after one cell is changed
     * @param allow false: deny @p name; true: allow it again
     * @return the new list; empty means "no catalog entries" (.acl delete)
     */
    static QStringList catalogAfter(const QStringList &current, const QString &name, bool allow);

    /// ".acl set console=... jobacl=..." or ".acl delete console=... jobacl"
    static QString changeCommand(const QString &console, const QString &type, const QStringList &entries);

    /// Did the Director accept a change? @p message: its words, if any
    static bool changeSucceeded(const QString &reply, QString *message = nullptr);
};

#endif // BACLMATRIX_H
