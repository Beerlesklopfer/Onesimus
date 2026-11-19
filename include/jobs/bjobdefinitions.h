#ifndef BJOBDEFINITIONS_H
#define BJOBDEFINITIONS_H

#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>

#include "config/bresourcetext.h"

/**
 * @file bjobdefinitions.h
 * @brief The Job and JobDefs resources of the Director with what each
 *        directive is set to and where: here, or inherited from a JobDefs
 *
 * The source is what BJobConfigModel loaded ("show jobs", "show jobdefs"),
 * by name. Without "verbose" the Director prints in a Job only what is set
 * in the Job; a directive it does not print is taken from the JobDefs the
 * job names, and from the JobDefs that one names, a few steps deep and never
 * in a circle (BWaitLimits::jobDirective() follows the same rule).
 *
 * Where the raw text of the resource is known (ReadResource), it decides
 * what is set here: a directive in the text is set here, any other one is
 * inherited, whatever "show" printed (resolveWithText()). QtCore only.
 */
class BJobDefinitions
{
    Q_DECLARE_TR_FUNCTIONS(BJobDefinitions)

public:
    enum class Kind { Job, JobDefs };

    /// One resource as the Director reported it
    struct Resource
    {
        Kind kind = Kind::Job;
        QString name;
        QJsonObject config;     ///< The object of "show", keys as printed (lower case)
    };

    /// One directive of a resource, resolved
    struct Value
    {
        QString key;            ///< normalized ("fileset")
        QString label;          ///< "FileSet"
        QString text;           ///< What it says, for display ("" if set nowhere)
        bool set = false;       ///< Set here or in a JobDefs of the chain
        bool here = false;      ///< Set in this resource
        QString from;           ///< The JobDefs it is inherited from (set && !here)
        bool uncertain = false; ///< The chain names a JobDefs that was not reported
    };

    void setResources(const QMap<QString, QJsonObject> &jobs, const QMap<QString, QJsonObject> &jobDefs);

    QList<Resource> resources() const;      ///< Jobs, then JobDefs, each by name
    bool contains(Kind kind, const QString &name) const;
    QJsonObject config(Kind kind, const QString &name) const;
    QStringList jobNames() const { return m_jobs.keys(); }
    QStringList jobDefsNames() const { return m_jobDefs.keys(); }

    /// The JobDefs a resource names directly ("" if none)
    QString jobDefsOf(Kind kind, const QString &name) const;
    /// The JobDefs a resource inherits from, nearest first
    QStringList chain(Kind kind, const QString &name) const;

    /// One directive, resolved over the chain
    Value value(Kind kind, const QString &name, const QString &key) const;

    /**
     * @brief Every directive of a resource and its chain, resolved
     *
     * The common directives first (commonKeys(), in that order, also when
     * set nowhere), then the others by label.
     */
    QList<Value> values(Kind kind, const QString &name) const;

    /**
     * @brief The same, with the raw text of the resource as the judge of
     *        what is set here
     *
     * A directive in the text is set here with the value of the text; a
     * directive "show" printed but the text does not have is taken from the
     * chain (or shown as set nowhere). Keys the text has and "show" does
     * not print are added.
     */
    QList<Value> resolveWithText(Kind kind, const QString &name, const BResourceText::Parsed &text) const;

    /**
     * @brief The resources that use a JobDefs, directly or through another
     *        JobDefs: "Job nightly", "JobDefs Defaults2"
     */
    QStringList usersOf(const QString &jobDefsName) const;
    /// Only the jobs among them
    QStringList jobsUsing(const QString &jobDefsName) const;

    // --- Directives ---------------------------------------------------------

    /// The directives the table and the form show, normalized, in order
    static QStringList commonKeys();
    /// "fileset" -> "FileSet"; an unknown key as it was printed
    static QString label(const QString &key);
    /// A value of "show" for display: a name of a reference, yes/no, a list joined
    static QString displayText(const QJsonValue &value);
    /// The value of a normalized key in an object of "show" (Undefined if absent)
    static QJsonValue directiveOf(const QJsonObject &config, const QString &key);

private:
    const QMap<QString, QJsonObject> &map(Kind kind) const { return kind == Kind::Job ? m_jobs : m_jobDefs; }

    QMap<QString, QJsonObject> m_jobs;
    QMap<QString, QJsonObject> m_jobDefs;
};

#endif // BJOBDEFINITIONS_H
