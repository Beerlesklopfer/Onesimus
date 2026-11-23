// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BUSAGESTATUS_H
#define BUSAGESTATUS_H

#include <QCoreApplication>
#include <QDateTime>
#include <QLocale>
#include <QString>

#include <optional>

#include "director/busage.h"

/**
 * @file busagestatus.h
 * @brief Text, tool tip and level of the free tier usage in the status bar
 *
 * Without widgets, so that the unit test reads exactly what the status bar
 * shows. Bytes are decimal (BByteFormat::formatDecimal()): the limit is
 * 1 TB = 10^12 bytes, as odir defines and prints it.
 *
 * Nothing is shown (empty text, Level::Hidden) when the Director did not
 * report usage: BNET, a classic Bareos Director, an older odir.
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */
class BUsageStatus
{
    Q_DECLARE_TR_FUNCTIONS(BUsageStatus)

public:
    /// What the status bar shows; the colour of a level comes from the theme
    enum class Level {
        Hidden,     ///< Nothing reported: the label is blank
        Ok,         ///< Below 80 % or no limit: no colour
        Near,       ///< >= 80 % of the limit: warning
        Reached     ///< >= 100 %: new backups are held, error
    };

    /// odir's state; computed from used() and the limit when it is unspecified
    static Level level(const BUsage::Info &info);

    /// "Usage: 73.6 GB of 1 TB (Nov)", "Usage: 2.3 TB (Nov)" without limit, empty when hidden
    static QString text(const BUsage::Info &info, const QLocale &locale = QLocale());

    /**
     * @brief Usage expected at the end of the month, from the elapsed share
     *
     * used() divided by the elapsed share of the month. The month runs from
     * the first of @c month to @c resetsAt (both in the Director's time
     * zone; its start is resetsAt minus the days of the month). No value in
     * the first day of the month (too little to go by), after resetsAt, or
     * when month or resetsAt is not known.
     */
    static std::optional<quint64> projection(const BUsage::Info &info, const QDateTime &now);

    /// Several lines: licence, limit, written, running, projection, held jobs, reset date
    static QString toolTip(const BUsage::Info &info, const QDateTime &now,
                           const QLocale &locale = QLocale());

    /// used() as a share of the limit in whole percent (rounded down); -1 without limit
    static int percent(const BUsage::Info &info);

    /// Days before the annual licence ends at which the status bar starts to show it
    static constexpr int LicenceNoticeDays = 30;
    /// Days before the end at which the notice turns from warning to error
    static constexpr int LicenceUrgentDays = 7;

    /**
     * @brief Whole days until the licence ends, rounded up; nullopt without an end date
     *
     * 1 means it ends within the next 24 hours, 0 or less that it has ended.
     */
    static std::optional<int> licenceDaysLeft(const BUsage::Info &info, const QDateTime &now);

    /**
     * @brief How the licence notice is shown
     *
     * Hidden for the free tier, without an end date, and while more than
     * LicenceNoticeDays are left; Near up to LicenceUrgentDays before the
     * end, Reached in the last days and once the licence has expired or is
     * not valid (the free tier applies then).
     */
    static Level licenceLevel(const BUsage::Info &info, const QDateTime &now);

    /// "Licence ends in 12 days, on 31.12.2026", "Licence expired: free tier"; empty when hidden
    static QString licenceText(const BUsage::Info &info, const QDateTime &now,
                               const QLocale &locale = QLocale());
};

#endif // BUSAGESTATUS_H
