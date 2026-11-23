// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BSTAGGERCONSTRAINTSDIALOG_H
#define BSTAGGERCONSTRAINTSDIALOG_H

#include <QDialog>
#include <QHash>
#include <QString>
#include <QStringList>

#include "config/bsettings.h"

class QCheckBox;
class QTimeEdit;

/**
 * @brief The user's constraints for the stagger proposal, per job
 *
 * Lists the jobs of the predicted waits, each with "Fixed" (never moved,
 * also not taken along by a job of a higher priority) and an optional
 * latest start on the clock. The caller keeps the result per Director
 * profile (BSettings::setSchedulesStaggerConstraints()); nothing goes to
 * the Director.
 *
 * Constraints of jobs that are not listed (not among the waits on display)
 * are kept as they are.
 */
class BStaggerConstraintsDialog : public QDialog
{
    Q_OBJECT

public:
    BStaggerConstraintsDialog(const QStringList &jobs,
                              const QHash<QString, BSettings::StaggerConstraint> &current,
                              QWidget *parent = nullptr);

    /// The constraints as edited, with those of the jobs not listed
    QHash<QString, BSettings::StaggerConstraint> constraints() const;

private:
    struct Row
    {
        QCheckBox *fixed = nullptr;
        QCheckBox *latestOn = nullptr;
        QTimeEdit *latest = nullptr;
    };
    QHash<QString, BSettings::StaggerConstraint> m_current;
    QStringList m_jobs;
    QHash<QString, Row> m_rows;
};

#endif // BSTAGGERCONSTRAINTSDIALOG_H
