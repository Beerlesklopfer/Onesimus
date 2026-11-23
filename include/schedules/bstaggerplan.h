// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BSTAGGERPLAN_H
#define BSTAGGERPLAN_H

#include <QCoreApplication>
#include <QList>
#include <QString>
#include <QStringList>
#include <QTime>

#include "schedules/model/brunspec.h"

/**
 * @brief The resources that make a stagger proposal (BWaitStagger) real
 *
 * A Job names exactly one Schedule (Bareos dird_conf.cc: "Schedule" is a
 * single resource reference), and a schedule starts only the jobs that name
 * it. Moving the Run lines of a shared schedule would move every job that
 * uses it. A job that starts later therefore gets a schedule of its own: a
 * copy of its schedule with every Run line later by the proposed minutes,
 * named after the base schedule and its new time ("WeeklyCycle-2306"). Jobs
 * of the same schedule that move by the same minutes share that copy. The
 * job's Schedule directive is then set to it; nothing else in the job
 * changes.
 *
 * Plain functions without widgets and without a Director.
 */
namespace BStaggerPlan {

/**
 * @brief The Run lines of @p schedule, later by @p minutes
 * @return false with @p error set if a line is not understood, runs at
 *         several hours, would cross midnight, or does not read back as
 *         written
 */
bool shiftedRunLines(const BSchedule &schedule, int minutes, QStringList *lines,
                     QString *error = nullptr);

/// The earliest start of the lines of @p schedule, later by @p minutes
QTime firstStart(const BSchedule &schedule, int minutes);

/**
 * @brief "<base>-HHMM", with "-2", "-3" ... if that name is taken
 * @param taken Names of existing schedules (compared without case)
 */
QString scheduleName(const QString &base, const QTime &start, const QStringList &taken);

/// The text of a new Schedule resource
QString scheduleText(const QString &name, const QString &description, const QStringList &runLines);

/**
 * @brief The raw text of a Job resource with its Schedule set to @p schedule
 *
 * An existing Schedule directive (spelled in any case, with or without
 * blanks: Bareos ignores both in keywords) is replaced in place, keeping
 * its indentation; a job without one (it may inherit one from its JobDefs)
 * gets the line before its closing brace.
 * @return an empty string if the text has no closing brace
 */
QString jobWithSchedule(const QString &jobText, const QString &schedule);

/// The Schedule directive of a Job resource's raw text, trimmed; empty if
/// it has none
QString scheduleLine(const QString &jobText);

/**
 * @brief The schedule a Job resource's raw text names, without quotes
 *
 * Empty if it has no Schedule directive (it may take one from its JobDefs).
 */
QString scheduleValue(const QString &jobText);

} // namespace BStaggerPlan

#endif // BSTAGGERPLAN_H
