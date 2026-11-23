// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BWAITSTAGGER_H
#define BWAITSTAGGER_H

#include <QDateTime>
#include <QHash>
#include <QList>
#include <QSet>
#include <QString>

#include "schedules/model/bschedulewaitmodel.h"

/**
 * @file bwaitstagger.h
 * @brief Later start times for jobs that wait for each other, without widgets
 *
 * Jobs that are scheduled for the same moment and share a limit (a storage,
 * a client, the slots of the Director, the parallel writers of a device)
 * wait for each other. This file looks for start times, later per job,
 * under which the replayed queue (BScheduleWaitModel::predict()) waits
 * less. It changes nothing: it proposes.
 *
 * What counts as waiting ("avoidable"): the time a run is held by a limit
 * (BWaitHold::Reason DirectorLimit, StorageLimit, ClientLimit, JobLimit)
 * and the time it waits inside the storage daemon for a device. A run held
 * by priority (Priority, QueueOrder) waits on purpose, e.g. a catalog
 * backup behind the backups of the night: that is not counted, and such a
 * run is not moved for it.
 *
 * Where a job may start: all runs of a job move together by the same number
 * of minutes (the job gets a schedule of its own, later by that much), only
 * later, never earlier. With Options::gridMinutes (the GUI: 15) the first
 * run of the job lands on a multiple of it on the clock (23:05 -> 23:15,
 * 23:30 ...), which leaves room for a run that lasts longer than measured;
 * without it the offsets go in steps of Options::stepMinutes.
 *
 * The order of priorities is kept: a run stays before (or at) every run of
 * a lower priority (a higher number) that was scheduled after it on its
 * day, so that a catalog backup still runs after the backups. A job that
 * moves past such a run takes that job along to its own new time (on the
 * grid as well); at the same moment the queue puts the higher priority
 * first. A move that would push such a job beyond its bound is not tried.
 *
 * The search, greedy and repeatable:
 *  - The job that waits longest is tried first. For it every offset up to
 *    its bound is replayed with the others as they are; the smallest offset
 *    with the least avoidable wait of all counted runs wins if it is less
 *    than before. Then the next job; then everything again, at most
 *    Options::passes times, until nothing gets better.
 *  - At the end every moved job is tried again with smaller offsets: one
 *    that waits no more for it moves less.
 * A job's bound is the smaller of Options::maxMinutes and the bound the
 * caller gives (Run lines that would cross midnight, lines that cannot be
 * moved).
 *
 * The user's constraints (kept per Director profile by the caller):
 *  - Options::fixedJobs: the job never moves, neither for its own wait nor
 *    taken along. A move of a higher priority that would have to take it
 *    along is therefore not tried (as for any job whose bound is 0).
 *  - Options::latestStartMinutes: no counted run of the job starts later
 *    than this minute of the day; it bounds the offset. A job that already
 *    starts after it stays. An "earliest start" is not offered: moves are
 *    only later, so it could forbid nothing.
 * Shift::fixed and Shift::latestStartMinute say when one of them bounded
 * the job, so that the caller can show it.
 *
 * The prediction is the first replay of BScheduleWaitModel (in favour of
 * the runs): a run that was never measured ends the moment it starts. What
 * is not known is not made up here either.
 */
namespace BWaitStagger {

struct Options
{
    int stepMinutes = 1;
    /// > 0: a job's first run lands on a multiple of this on the clock
    int gridMinutes = 0;
    int maxMinutes = 60;
    int passes = 3;
    /// Per job: the most it may move, in minutes; a job not in the map
    /// has Options::maxMinutes. 0: the job stays.
    QHash<QString, int> jobMaxMinutes;
    /// The user's constraint: these jobs never move, also not taken along
    QSet<QString> fixedJobs;
    /// The user's constraint per job: the latest minute of the day
    /// (0 to 1439) at which a counted run of the job may start
    QHash<QString, int> latestStartMinutes;
};

/** The proposal for one job. */
struct Shift
{
    QString job;
    int minutes = 0;            ///< Later by this much; 0: stays
    qint64 waitBeforeSecs = 0;  ///< Avoidable wait of its counted runs, as scheduled now
    qint64 waitAfterSecs = 0;   ///< ... with every proposed shift
    int boundMinutes = 0;       ///< The most it was allowed to move
    /// Moved only to stay after this job of a higher priority (empty: moved
    /// for its own wait, or not at all)
    QString follows;
    /// Bound to 0 by Options::fixedJobs
    bool fixed = false;
    /// Options::latestStartMinutes bounded it (its bound is no larger than
    /// the others): that minute of the day; -1 otherwise
    int latestStartMinute = -1;
};

struct Proposal
{
    QList<Shift> shifts;        ///< Every job of the runs, in the order of their first run
    qint64 totalBeforeSecs = 0; ///< Avoidable wait of all counted runs, now
    qint64 totalAfterSecs = 0;  ///< ... with the proposal
    QList<BWaitRun> runs;       ///< The runs at their proposed times
    QList<BWaitResult> results; ///< Their prediction

    int moved() const;          ///< Jobs that move
};

/// The time a run is held by a limit plus its wait for a device
qint64 avoidableSecs(const BWaitResult &result);

/**
 * @brief Looks for later start times
 * @param runs    As for BScheduleWaitModel::predict(), in its order
 * @param limits  As for predict()
 * @param from    Only runs scheduled from here on are counted (the runs of
 *                the day before are passed so that they can be running)
 * @param options How far and in what steps
 */
Proposal propose(const QList<BWaitRun> &runs, const BWaitLimits &limits,
                 const QDateTime &from, const Options &options = Options());

} // namespace BWaitStagger

#endif // BWAITSTAGGER_H
