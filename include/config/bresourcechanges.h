// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BRESOURCECHANGES_H
#define BRESOURCECHANGES_H

#include <QList>
#include <QString>
#include <QStringList>

#include <functional>

#include "config/bresourcetext.h"

class BConfigResource;

/**
 * @file bresourcechanges.h
 * @brief What the resource form (Edit Job..., Edit JobDefs...) changed,
 *        as directive changes for BResourceText::edit()
 *
 * The form starts from the resource as "show jobs" reports it: keys in lower
 * case, values as the Director prints them, inherited values included. It
 * returns the values of its fields under the directive names of the schema
 * ("FileSet"), empty fields left out. Only what the user really changed
 * becomes a change:
 *
 *  - keys are compared without case and blanks ("fileset" = "File Set");
 *  - yes/true and no/false are the same, so are quoting and outer blanks;
 *  - a field the resource did not have and that holds its default is no
 *    change (a spin box always shows a number);
 *  - a directive the resource had and the form left empty is removed;
 *  - a directive with a list or a nested block is never written from the
 *    form: if its field differs, it is named in @c skipped instead.
 *
 * QtCore only (bresourcechanges_test).
 */
struct BResourceChanges
{
    QList<BResourceText::Change> changes;
    QStringList descriptions;   ///< "Client: a-fd -> b-fd", one per change
    QStringList skipped;        ///< Directives the form cannot write ("Run Script")

    bool isEmpty() const { return changes.isEmpty(); }

    /// The default of a directive as text ("" if none or unknown)
    using DefaultOf = std::function<QString(const QString &directive)>;

    /**
     * @param formDirectives The directives the form has a field for, as the
     *        schema spells them: only these can have been emptied (the form
     *        drops empty fields), and their spelling is the one written
     */
    static BResourceChanges between(const BConfigResource &before, const BConfigResource &after,
                                    const QStringList &formDirectives,
                                    const DefaultOf &defaultOf = DefaultOf());

    /// Values the Director and the form spell differently, made comparable
    static QString comparable(const QString &value);
};

#endif // BRESOURCECHANGES_H
