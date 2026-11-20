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
 * AclAccessOk): an entry with "!" that matches denies, then "*all*" or a
 * matching entry allows; entries match the name exactly or as a regular
 * expression, both without regard to case.
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

/** The ACLs of one console. */
struct BConsoleAcl
{
    QString name;
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
};

#endif // BACLMATRIX_H
